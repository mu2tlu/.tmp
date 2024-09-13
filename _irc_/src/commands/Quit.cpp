#include "../../include/Commands.hpp"

std::string msgmerge(std::vector<std::string> commandParts)
{
    std::string message;
    message = commandParts[1];
        for (size_t i = 2; i < commandParts.size(); ++i)
            message += " " + commandParts[i];
    return message;
}


void Quit::quit(Client* client, const std::vector<std::string> commandParts, Server* srv)
{
    std::string msg;
        if (commandParts.size() > 1)
            msg = msgmerge(commandParts);

        std::map<int, Client> clients = srv->getClientMap();
        for(std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
        {
            if(client->getFd() != it->second.getFd() && client->getFd() != srv->getBot()->getSocket())
                srv->sendMessage(it->second.getFd(),RPL_QUIT(client->getPrefix(), msg));
        }
        srv->closeConnection(client->getFd());
    
    
}