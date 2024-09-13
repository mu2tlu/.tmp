#include<../include/Client.hpp>
#include<../include/Utils.hpp>
#include <sys/socket.h>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include "../include/Server.hpp"
#include <unistd.h>
#include <sstream>


Client::Client(){}

Client::Client(int socket, const std::string& ip, int port){
    _clientSocketFd = socket;
    _clientIp = ip;
    _clientPort = port;
    _isAuth = false;
	_hasNick = false;
	_loged = false;
}
Client::~Client(){}
bool Client::isAuth() const {return _isAuth;}

//get
int Client::getFd() const {return _clientSocketFd;}
const std::string& Client::getNickname () const {return _nickname;}
const std::string& Client::getRealname () const {return _realname;}
const std::string& Client::getUsername () const {return _username;}
const std::string& Client::getClientIp () const {return _clientIp;}
bool Client::getHasNick() const {return _hasNick;}
bool Client::getLoged() const {return _loged;}

//set
void Client::setNickname(const std::string& nickname){_nickname = nickname;}
void Client::setRealname(const std::string& realname){_realname = realname;}
void Client::setUsername(const std::string& username){_username = username;}
void Client::setClientIp(const std::string& ip){_clientIp = ip;}

void Client::setHasNick(bool hasNick){_hasNick = hasNick;}
void Client::setAuth(bool auth) {_isAuth = auth;}
void Client::setLoged(bool loged) {_loged = loged;}

//Authenticat
void Client::authenticate(){_isAuth = true;}
void Client::deauthenticate(){_isAuth = false;}




std::string Client::getPrefix() const
{
	std::string username = this->getUsername().empty() ? "*" : _username;
	std::string hostname = this->getClientIp().empty() ? "*" : _clientIp;

	return this->getNickname() + "!" + username + "@" + hostname;
}

void Client::sendMessage(const std::string& message) const
{
	std::string buffer = message + "\r\n";

	send(getFd(), buffer.c_str(), buffer.length(), 0);
}


void Client::sendReply(const std::string& reply) const
{
	sendMessage(getPrefix() + " " + reply);
}

std::string Client::clientFind(std::string target, Server *srv)
{
    std::map<int, Client> client_map = srv->getClientMap();

    for (std::map<int, Client>::iterator it = client_map.begin(); it != client_map.end(); it++)
    {
        if (it->second.getNickname() == target)
            return target;
    }
    return "";
}