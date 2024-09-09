#include "../../include/Commands.hpp"

void User::user(Client *client, const std::vector<std::string> commandParts, Server *srv)
{
if (!client->getHasNick())
        return(client->sendMessage(": You must set a nickname before this"));
    if (commandParts.size() < 4)
        return (client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "USER")));
    if (!client->getUsername().empty())
        return (client->sendReply(ERR_ALREADYREGISTERED(client->getNickname())));


    client->setUsername(commandParts.at(1));
    client->setClientIp(commandParts.at(2));

     // realname birden fazla kelimeden olusuyorsa kelimeyi birlestirir
    std::string realname = commandParts.at(3);
    for (size_t i = 4; i < commandParts.size(); ++i)
    {
        realname += " " + commandParts[i];
    }
    if (realname[0] == ':') {
        realname.erase(0, 1);  // ':' karakterini kaldır
    }
    client->setRealname(realname);
    
    client->sendReply(RPL_WELCOME(srv->getHost(), client->getNickname(), client->getPrefix()));
    client->setLoged(true);
}
