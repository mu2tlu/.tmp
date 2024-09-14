#include "../include/Bot.hpp"
#include "../include/Utils.hpp"

Bot::Bot(const std::string &serv, int port, const std::string &pass)
	: serv(serv),
	  port(port),
	  botPass(pass),
	  nick("BotNICK"),
	  user("IRC"),
	  realname("Bot")
{
	connectServ();
}

Bot::~Bot()
{
	close(sock);
}

void Bot::connectServ()
{
	struct sockaddr_in server_addr;
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		write(STDOUT_FILENO, "Error: Bot socket creation failed.\n", 34);
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(server_addr.sin_zero), '\0', 8);

	if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		close(sock);
		write(STDOUT_FILENO, "Error: Bot socket connection failed.\n", 36);
		exit(1);
	}

	if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
	{	
		close(sock);
		write(STDOUT_FILENO, "Error: Bot socket fcntl failed.\n", 32);
		exit(1);
	}

	if (sock != -1)
	{
		sendRegMsg("PASS " + botPass + "\r\n");
		sendRegMsg("NICK " + nick + "\r\n");
		sendRegMsg("USER " + user + " 0 * :" + realname + "\r\n");
	}
}

void Bot::listen()
{
	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	std::string accumulated;

	int bytes_read = recv(sock, buffer, BUFFER_SIZE - 1, 0);
	if (bytes_read < 0)
	{
		std::string errrecv = "Error in recv: " + std::string(strerror(errno)) + "\n";
		write(STDOUT_FILENO, errrecv.c_str(), errrecv.length());
		return;
	}
	else if (bytes_read == 0)
	{
		write(STDOUT_FILENO, "Connection closed by peer.\n", 27);
		return;
	}
	buffer[bytes_read] = '\0';
	accumulated += buffer;

	size_t pos;
	while ((pos = accumulated.find("\r\n")) != std::string::npos)
	{
		std::string msg = accumulated.substr(0, pos);
		accumulated.erase(0, pos + 2);
		processMessage(msg);
	}
}

std::string censorMessage(const std::string& message, const std::string blacklist[], int blacklistSize) {
    
	std::string censoredMessage = message;
    for (int i = 0; i < blacklistSize; ++i) {
        size_t pos = 0;
        std::string replacement(blacklist[i].length(), '*');
        while ((pos = censoredMessage.find(blacklist[i], pos)) != std::string::npos) {
            censoredMessage.replace(pos, blacklist[i].length(), replacement);
            pos += replacement.length();
        }
    }
    return censoredMessage;
}


std::string Bot::processMessage(const std::string &message)
{
	std::string senderNick, msg;
	
    if (!message.empty())
	{
    	senderNick = message.substr(0, message.find("!"));
    	msg = message.substr(message.find("!") + 1);

    	if (msg.find("Bot") != std::string::npos) {
        	sendMsg(senderNick, "Hello " + senderNick + "!");
		}

		if (msg.find("time") != std::string::npos || msg.find("date") != std::string::npos)
		{
			time_t rawtime;
			struct tm *timeinfo;
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			sendMsg(senderNick, asctime(timeinfo));
		}
		
		std::string blacklist[] = {"uglyasf", "screw", "dumb", "stupid", "idiot", "amk"};
		int blacklistSize = sizeof(blacklist) / sizeof(blacklist[0]);

		std::string censoredMsg = censorMessage(msg, blacklist, blacklistSize);
		if (censoredMsg != msg)
		{
			sendMsg(senderNick, "Please be more respectful!");
			return censoredMsg;
		}
	}
	return msg;
}

void Bot::sendMsg(const std::string &nick, const std::string &message)
{
	std::string fullMessage = "PRIVMSG " + nick + " :" + message + "\r\n";
	send(sock, fullMessage.c_str(), fullMessage.length(), 0);
}

void Bot::sendRegMsg(const std::string &message)
{
	send(sock, message.c_str(), message.length(), 0);
}

int Bot::getSocket() const
{
	return sock;
}

void Bot::sendWelMsg(Client* client)
{
	std::string buffer;
	buffer = ":" + getBotnick() + " PRIVMSG " + client->getNickname() + " :" + "Welcome Our IRC Server !!" + "\r\n";

	send(client->getFd(), buffer.c_str(), buffer.length(), 0);
}
