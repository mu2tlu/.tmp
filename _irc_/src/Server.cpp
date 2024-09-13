#include "../include/Server.hpp"
#include <sstream>

Server::Server(int port, const std::string& password, const std::string& host) 
    :_portNumber(port), _password(password), _host(host) {
}

Server::~Server() 
{
    delete _bot;
    // Sunucu nesnesi yok edilirken tüm kanalları serbest bırak
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        delete it->second;  // Dinamik olarak oluşturulan kanalları serbest bırak
    }
    _channels.clear();
}

bool Server::authClient(int clientSocket){
    
    sendMessage(clientSocket, "PASS: Enter a password\r\n");

    std::string receivedPass = receiveMessage(clientSocket);
    if(receivedPass.empty()){
        return false;
    }

    if(verifyPassword(receivedPass)){
        _clients[clientSocket].setAuth(true);
        sendMessage(clientSocket, "Welcome to the IRC server\r\n");
        return true;
    } else {
        sendMessage(clientSocket, "Invalid Password. Connection Closed!\r\n");
        return false;
    }
}

bool Server::verifyPassword(const std::string& password){
    std::cout << _password << " ==== " << password << std::endl;
    return _password == password;
}

bool Server::sendMessage(int clientSocket, const std::string& msg) {
    size_t totalSend = 0;
    size_t remaining = msg.length();
    ssize_t sent;

    while (totalSend < msg.length()) {
        sent = send(clientSocket, msg.c_str() + totalSend, remaining, 0);
        if (sent < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                // tyr again
                continue;
            } else {
                throw ServerException("Error sending message to client");
            }
        } else if (sent == 0) {
            // socket connection closed.
            return false;
        }
        totalSend += sent;
        remaining -= sent;
    }
    std::cout << std::endl;
    return true;
}

std::string Server::receiveMessage(int clientSocket){
    char buffer[BUFFER_SIZE];
    std::string message;
    ssize_t bytesRead ;

    while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';
        message += buffer;
        if (message.find("\r\n") != std::string::npos) {
            break;
        }
    }

    if(bytesRead == 0){
        //connection closed.
        std::cout << RED << "Client <" << clientSocket - 3 << "> Disconnected" << WHITE << std::endl;
        closeConnection(clientSocket);
        return "";
    } else if (bytesRead < 0 && errno != EWOULDBLOCK && errno != EAGAIN) {
        throw ServerException("Error receiving message from client");
    }
    return message;
}

void Server::createSocket(){
    //1. socket create
     _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(_serverSocket == -1){
        throw ServerException("Failed to create socket");
    } 
}

void Server::configureSocket(){
    //2. Socket conf /SOL_SOCKET = socket level protocols /SO_REUSEADDR = re useble port
    int opt = 1;
    if(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        throw ServerException("Failed to configure socket");
    }
}

void Server::bindSocket(){
    //3. socket connection port and adress
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_portNumber);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(_serverSocket, (const sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        throw ServerException("Failed bind mode maybe port already used");
        close(_serverSocket);
        exit(1);
    }
}

void Server::listenMode(){
    //4.socket listening mode
    if(listen(_serverSocket,10) < 0){
        throw ServerException("Failed to listen on socket");
    }
}

void Server::setNonBlocking(int socket){
    //5.Socket setg nonblocking mode 
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags == -1) {
        throw ServerException("Failed to get server socket flags");
    }
    flags |= O_NONBLOCK;
    std::cout<< YELLOW << socket  << ": ===> Set NonBlocking Mode" << WHITE << std::endl;
    if(fcntl(socket,F_SETFL,flags) < 0)
        throw ServerException("Failed to set NonBlocking mode server socket");
}


void Server::initialize(){
    setupServerSocket();
    setPollFd();
}

void Server::addNewClient(struct sockaddr_in* clientAddr, int clientSocket){
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr->sin_addr),ip, INET_ADDRSTRLEN);
    int port = ntohs(clientAddr->sin_port);

    _clients[clientSocket] = Client(clientSocket,std::string(ip), port);

    std::cout << GREEN << "Client <" << clientSocket - 3 << "> Connected from " << ip << ":" << port << WHITE << std::endl;
}

void Server::setupServerSocket(){
    
    try {
        createSocket();
        configureSocket();
        bindSocket();
        listenMode();
        setNonBlocking(_serverSocket);
    } catch (const ServerException& e){
        std::cerr << "Socket initialization failed: " << e.what() << std::endl;
        close(_serverSocket);
        exit(1);
    }
}
void Server::setPollFd(){
    pollfd serverPollFd;
    serverPollFd.fd = _serverSocket;
    serverPollFd.events = POLLIN;
    fds.push_back(serverPollFd);
}


void Server::acceptConnection(){
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
   
    // Re connection
    int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if(clientSocket == -1){
        if(errno != EWOULDBLOCK && errno != EAGAIN){
            throw ServerException("Failed aceppting connection");
        }
        return;
    }
        setNonBlocking(clientSocket);
        _pollManager.addFd(clientSocket, POLL_IN);

        addNewClient(&clientAddr, clientSocket);
    std::cout << GREEN << "Client <" << clientSocket -3 << "> Connected" << WHITE << std::endl;
}


void Server::handleClientMessage(int clientSocket){
    try {
        std::string message = receiveMessage(clientSocket);
        if (message.empty()) {
            return; // Connection alraedy closed
        }
        	size_t endOfCommand;
            std::string command;

        if(message[0] == '/')
            {
                while ((endOfCommand = message.find('\n')) != std::string::npos)
                {
			    command = message.substr(0, endOfCommand);
			    message.erase(0, endOfCommand + 1);
			    CommandParser::commandParser(command.c_str(), &_clients[clientSocket], this);
                }
                if (!message.find('\n'))
                    message.clear();
            }
        else 
            {
                while ((endOfCommand = message.find("\r\n")) != std::string::npos)
                {
                    command = message.substr(0, endOfCommand);
                    message.erase(0, endOfCommand + 2);
                    CommandParser::commandParser(command.c_str(), &_clients[clientSocket], this);
                }
                message.clear();
            }
    } catch (const ServerException& e) {
        std::cerr << "Error handling client message: " << e.what() << std::endl;
        closeConnection(clientSocket);
    }
}

void Server::run(){

    SignalHandler::setup();
    botConnect();
    std::cout << GREEN << "Server <" << _serverSocket - 2 << "> Connected" << WHITE << std::endl;
    std::cout << "Waiting to accept a connection..." << std::endl;

    _pollManager.addFd(_serverSocket, POLL_IN);

    while (SignalHandler::isRunning()){
        int ready = _pollManager.poll(-1);
        if(ready == -1){
            if(errno == EINTR) continue;
            throw ServerException("Poll Failed");
        }

        const std::vector<pollfd>& fds = _pollManager.getFds();  
        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == _serverSocket) {
                    acceptConnection();
                }
                else if (fds[i].fd == _bot->getSocket()) { //BOT
                    _bot->listen();
                }       
                else {
                    handleClientMessage(fds[i].fd);
                } 
            }
        }
    }
    std::cout << "Server Shutting Down Succesfully" << std::endl;
}


void Server::closeConnection(int clientSocket){
    try {
        if (close(clientSocket) < 0) {
            throw ServerException("Error closing client socket");
        }
        _pollManager.removeFd(clientSocket);

        std::map<int, Client>::iterator it = _clients.find(clientSocket);
        if(it != _clients.end()) {
            _clients.erase(it);
        }
        
        for (std::vector<std::pair<int, std::string> >::iterator it = _nickList.begin(); it != _nickList.end(); ++it)
        {
            if (it->first == clientSocket)
            {
                _nickList.erase(it);
                break;
            }
        }
    
        std::cout << YELLOW << "Client <" << clientSocket - 3 << "> Connection closed" << WHITE << std::endl;
    } catch (const ServerException& e) {
        std::cerr << "Error in closeConnection: " << e.what() << std::endl;
    }
}

Client* Server::getClient(const std::string  &nickname)
{
    std::cout << "Searching for client: " << nickname << std::endl;
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second.getNickname() == nickname){
            std::cout << "Client found: " << nickname << std::endl;
			return &(it->second);
        }
	}
    std::cout << "Client not found: " << nickname << std::endl;
	return NULL;
}


void Server::removeChannel(const std::string& channelName)
{
    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (it != _channels.end()) {
        Channel* channel = it->second;
        std::vector<std::string> clientNicks = channel->getChannelClients();
        for (std::vector<std::string>::iterator nickIt = clientNicks.begin(); nickIt != clientNicks.end(); ++nickIt) {
            Client* client = getClient(*nickIt);  
            if (client) {
                client->sendReply(RPL_PART(client->getNickname(), channelName, "The channel has been removed"));
            }
        }

        // Remove channel from server
        _channels.erase(it);

        // remove channel from memory
        delete channel;

        std::cout << "Channel '" << channelName << "' has been removed from the server." << std::endl;
    } else {
        std::cout << "Channel '" << channelName << "' not found." << std::endl;
    }
}


void Server::botConnect() //BOT
{
     try
    {
        this->_bot = new Bot("localhost", _portNumber, _password);
    }
    catch (const std::exception &e)
    {
        delete _bot;
        _bot = NULL;
        write(STDOUT_FILENO, e.what(), strlen(e.what()));
    }

}

Bot * Server::getBot() const //BOT
{
    return _bot;
}

