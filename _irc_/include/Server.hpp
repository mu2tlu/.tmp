#ifndef SERVER_HPP
#define SERVER_HPP



#define GREEN "\e[1;32m"
#define RED "\e[1;31m"
#define WHITE "\e[1;37m"
#define YELLOW "\e[1;33m"

#include "../include/SignalHandler.hpp"
#include "../include/Client.hpp"
#include "../include/ServerException.hpp"
#include "../include/PollManager.hpp"
#include "../include/Commands.hpp"


#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <cstdlib>
#include <poll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <map>
#include <algorithm> // std::remove için gerekli
#include "Utils.hpp"


class Client;
class Channel;
class Server {

    private:
        int _serverSocket;
        int _portNumber;
        const std::string& _password;
        const std::string& _host;
        std::vector<pollfd> fds;
        std::map<int, Client> _clients;
        std::map<std::string, Channel*> _channels;
        
        PollManager _pollManager;
        static const size_t BUFFER_SIZE = 1024;
        //Namelist
        std::vector<std::pair<int, std::string> > _nickList;
        


    
    public:
        Server(int port, const  std::string& password, const std::string& host);
        ~Server();
        void run();
        void initialize();
        void addNewClient(struct sockaddr_in* clientAddr, int clientSocket);
        std::string getSrvPass() const{return _password;};
        std::string getHost() const{return _host;};
        //Namelist
        bool isNickTaken(const std::string& nick);
        bool registerNick(const std::string& nick, int id);
        void addChannel(Channel* channel);
        Channel* getChannel(const std::string& channelName);
        bool hasChannel(const std::string& channelName);
        std::map<std::string, Channel*> getChannels();
        Client* getClient(const std::string  &nickname);
        


    private:
        void setPollFd();
        //Socket Fonksiyonları:
        void setupServerSocket();
        void setNonBlocking(int socket);
        void createSocket();
        void configureSocket();
        void bindSocket();
        void listenMode();


        void acceptConnection();
        void handleClientMessage(int clientSocket);
        void closeConnection(int clientSocket);

        //Authenticate
        bool authClient(int clientSocket);
        bool verifyPassword(const std::string& password);

        //Message
        bool sendMessage(int clientSocket, const std::string& msg);
        std::string receiveMessage(int clientSocket);




};

#endif