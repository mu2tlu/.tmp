#ifndef SERVER_HPP
#define SERVER_HPP



#define GREEN "\e[1;32m"
#define RED "\e[1;31m"
#define WHITE "\e[1;37m"
#define YELLOW "\e[1;33m"

#include "SignalHandler.hpp"
#include "Client.hpp"
#include "ServerException.hpp"
#include "PollManager.hpp"
#include "Commands.hpp"
#include "Bot.hpp" //BOT
#include "Utils.hpp"

#include <netdb.h>
#include <sys/types.h>
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
#include <algorithm> 


class Client;
class Channel;
class Bot; //BOT
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
        Bot* _bot;//BOT
        

    
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
        void removeChannel(const std::string& channelName);
        std::map<int, Client> getClientMap(){return _clients;}
        Bot* getBot() const; //BOT
        void botConnect(); //BOT
        bool sendMessage(int clientSocket, const std::string& msg);
        void closeConnection(int clientSocket);


    private:
        void setPollFd();
        //Socket 
        void setupServerSocket();
        void setNonBlocking(int socket);
        void createSocket();
        void configureSocket();
        void bindSocket();
        void listenMode();


        void acceptConnection();
        void handleClientMessage(int clientSocket);

        //Authenticate
        bool authClient(int clientSocket);
        bool verifyPassword(const std::string& password);

        //Message
        std::string receiveMessage(int clientSocket);

};
#endif