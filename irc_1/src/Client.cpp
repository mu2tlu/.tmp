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
const std::string& Client::getHostname () const {return _hostname;}
const std::string& Client::getClientIp () const {return _clientIp;}
bool Client::getHasNick() const {return _hasNick;}
bool Client::getLoged() const {return _loged;}

//set
void Client::setNickname(const std::string& nickname){_nickname = nickname;}
void Client::setRealname(const std::string& realname){_realname = realname;}
void Client::setUsername(const std::string& username){_username = username;}
void Client::setHostname(const std::string& hostname){_hostname = hostname;}
void Client::setClientIp(const std::string& ip){_clientIp = ip;}

void Client::setHasNick(bool hasNick){_hasNick = true;}
void Client::setAuth(bool auth) {_isAuth = auth;}
void Client::setLoged(bool loged) {_loged = loged;}

//Authenticat
void Client::authenticate(){_isAuth = true;}
void Client::deauthenticate(){_isAuth = false;}


std::string Client::getPrefix() const
{
	std::string username = _username.empty() ? "*" : _username;
	std::string hostname = _clientIp.empty() ? "*" : _clientIp;

	return _nickname + "!" + username + "@" + hostname;
}

void Client::sendMessage(const std::string& message) const
{
	std::string buffer = message + "\r\n";

	if (send(_clientSocketFd, buffer.c_str(), buffer.length(), 0) == -1)
		ErrorLogger(FAILED_SOCKET_SEND, __FILE__, __LINE__);
}


void Client::sendReply(const std::string& reply) const
{
	std::cout << getHostname() << " host name" << std::endl;
	std::cout << "DENEME" << std::endl;
	std::cout << " REPLY "<< reply << getPrefix() << std::endl;
	sendMessage(getPrefix() + " " + reply);
}


// REPLY :urqu!akif@127.0.0.1	353 urqu!akif@127.0.0.1 = #31 :urqu urqu!akif@127.0.0.1
// REPLY :abulut!alp@127.0.0.1 353 abulut!alp@127.0.0.1 = #31 :abulut abulut!alp@127.0.0.1
// REPLY :abulut!alp@alp 		353 abulut!alp@alp 		= #31 :abulut abulut!alp@alp

