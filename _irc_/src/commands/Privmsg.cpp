#include "../../include/Commands.hpp"

std::string merge_privmsg(const std::vector<std::string> &parts, const std::string delim)
{
    std::string result;
    for (size_t i = 0; i < parts.size() ; ++i)
    {
        if (i > 0)
            result += delim;
        result += parts[i];
    }
    return result;
}

std::string clinet_find(std::string target, Client *client, Server *srv)
{
    std::map<int, Client> client_map = srv->getClientMap();

    for (std::map<int, Client>::iterator it = client_map.begin(); it != client_map.end(); it++)
    {
        if (it->second.getNickname() == target)
            return target;
    }
    return "";
}

void Privmsg::privmsg(Client *client, const std::vector<std::string> commandParts, Server *srv) 
{
    Client *targetClient = NULL;

    if (commandParts.size() < 2) {
        std::cerr << "[ERROR] Target not found." << std::endl;
        return;
    }
    std::string message;
    if (commandParts.size() < 3) {
        std::cerr << "[ERROR] Message not found." << std::endl;
        return;
    }
    else {
        message = merge_privmsg(std::vector<std::string>(commandParts.begin() + 2, commandParts.end()), " ");
        if(message[0] == ':')
            message.erase(0, 1);
    }

    if((commandParts[0] == "PRIVMSG"  || commandParts[0] == "/PRIVMSG") && commandParts.at(1)[0] != '#') {
        targetClient = srv->getClient(clinet_find(commandParts.at(1), client, srv));
        if (targetClient != NULL) {
            if(targetClient->getFd() != client->getFd()) 
                targetClient->sendMessage(":" + client->getNickname() + " PRIVMSG " + commandParts.at(1) + " :" + message);
            else 
                std::cerr << "[ERROR] Cannot send message to self." << std::endl;
        }
        else {
            std::cerr << "[ERROR] The destination client is not registered." << std::endl;
            return;
        }
    }
    else if (srv->getChannel(commandParts.at(1))) {
        std::vector<std::string> channelTargetList = srv->getChannel(commandParts.at(1))->getChannelClients();
        
        for(size_t i = 0; i < channelTargetList.size(); i++) {
                targetClient = srv->getClient(clinet_find(channelTargetList.at(i), client, srv));
            if(targetClient->getFd() != client->getFd())
                targetClient->sendMessage(":" + client->getNickname() + " PRIVMSG " + commandParts.at(1) + " :" + message);
        }
    }
}