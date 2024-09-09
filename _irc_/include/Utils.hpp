#ifndef UTILS_HPP
#define UTILS_HPP


#include <iostream>
#include <string>
#include <cstring>
#include <exception>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <ctime>


#define RPL_JOIN(source, channel) ":" + source + " JOIN :" + channel + "\r\n"

#define RPL_ENDOFNAMES(source, channel) "366 " + source + " " + channel + " :End of /NAMES list.\r\n"
#define RPL_NAMREPLY(source, channel, nickList) ":" + source + " 353 " + source + " = " + channel + " :" + nickList + "\r\n"



#define ERR_NOSUCHCHANNEL(source, channel) "403 " + source + " " + channel + " :No such channel"
#define ERR_NOTONCHANNEL(source, channel) "442 " + source + " " + channel + " :You're not on that channel"
#define RPL_NOTOPIC(source, channel) "331 " + source + " " + channel + " :No topic is set"

#define RPL_TOPIC(nick, ip, channel, topic)         ":" + nick + "!" + nick + "@" + ip + " TOPIC " + channel + " :" + topic + "\r\n"

#define ERR_NOSUCHSERVER(source, server) "402 " + source + " " + server + " :No such server"
#define RPL_LISTEND(source) "323 " + source + " :End of /LIST"/
#define RPL_LIST(source, channel, userCount, topic) "322 " + source + " " + channel + " " + userCount + " :" + topic

#define GET_CURRENT_TIME time(0)
#define FORMAT_TIME(t, buffer) strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", localtime(&t))

#define FAILED_SOCKET_SEND "Failed to send socket"

#define ERR_NEEDMOREPARAMS(source, command) "461 " + source + " " + command + " :Not enough parameters"		//GLOBAL PARAMETER 
#define ERR_INVALIDCAPCMD(source) "410 " + source + " :Invalid CAP subcommand"								//CAP
#define ERR_ALREADYREGISTERED(source) "462 " + source + " :You may not register"							//PASS
#define ERR_ALREADYREGISTRED "Already registered\r\n"

//NICK
#define ERR_NONICKNAMEGIVEN(source) "431 " + source + " :No nickname given"
#define ERR_ERRONEUSNICKNAME(source, nick) "432 " + source + " " + nick + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(source, nick) "433 " + source + " " + nick + " :Nickname is already in use"


//CHANNEL
#define ERR_NOSUCHCHANNEL(source, channel) "403 " + source + " " + channel + " :No such channel"
#define ERR_USERONCHANNEL(source, channel) "443 " + source + " " + channel + " :is already on channel"
#define ERR_BADCHANNELKEY(source, channel) "475 " + source + " " + channel + " :Cannot join channel (+k)"
#define ERR_CHANNELISFULL(source, channel) "471 " + source + " " + channel + " :Cannot join channel (+l)"




static inline void ErrorLogger( std::string messageInfo, const char* fileInfo, int lineInfo, bool isFatal = false )
{
	std::ofstream errorLog;

	errorLog.open("error.log", std::ios::app);
	errorLog << "Error Time: " << __TIME__ << std::endl;
	errorLog << "Error in file: " << fileInfo << ":" << lineInfo << std::endl;
	errorLog << "Error info: " << std::string(strerror(errno)) << std::endl;
	errorLog << "----------------------------------------" << std::endl;
	errorLog.close();

	if (isFatal) {
		throw std::runtime_error(messageInfo);
	}
	write(STDERR_FILENO, messageInfo.c_str(), messageInfo.length());
}
static inline void log(const std::string& message) {
	char buffer[100];
	time_t currentTime = time(NULL);
	std::ofstream logFile;


	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
	std::string timeStr(buffer);

	logFile.open("log.log", std::ios::app);
	logFile << timeStr << " " << message << std::endl;
	logFile.close();


	std::ostringstream oss;
	oss << "\033[0;34m[" << timeStr << "]\033[0m " << message << std::endl;

	std::string outputStr = oss.str();
	write(STDOUT_FILENO, outputStr.c_str(), outputStr.size());
}

#endif