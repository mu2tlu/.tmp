#include "../../include/Commands.hpp"
#include "../include/Server.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

bool Server::isNickTaken(const std::string& nick) {
        for (size_t i = 0; i < _nickList.size(); ++i) {
            if (_nickList[i].second == nick) {
                return true;  // Nickname zaten alınmış
            }
        }
        return false;  // Nickname alınmamış
    }

bool Server::registerNick(const std::string& nick, int id) 
{
    _nickList.push_back(std::make_pair(id, nick));
    std::cout << "Nickname '" << nick << "' registered successfully.\n";
    return true;
}


void Nick::nick(Client *client, const std::vector<std::string> commandParts, Server *srv)
{
    if(client->isAuth() == false)
    {
        return(client->sendMessage(": You must log in before this"));
    }
    if (commandParts.size() < 2)
        return (client->sendReply(ERR_NONICKNAMEGIVEN(client->getNickname())));
    std::string nick = commandParts.at(1);
    if (srv->isNickTaken(nick))
        return (client->sendReply(ERR_NICKNAMEINUSE(client->getNickname(), nick)));
    else
    {
        srv->registerNick(nick, client->getFd());
        
        client->setNickname(nick);
        client->setHasNick(true);
        return;
    }
}

