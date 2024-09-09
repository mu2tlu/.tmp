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

void Privmsg::privmsg(Client *client, const std::vector<std::string> commandParts, Server *srv) 
{
    if (client == NULL || srv == NULL) {
        std::cerr << "[ERROR] Null pointer encountered for client or server." << std::endl;
        return;
    }

    if (commandParts.size() < 2) {
        std::cerr << "[ERROR] Target not found." << std::endl;
        return;
    }

    Client *targetClient = srv->getClient(commandParts.at(1));
    if (targetClient == NULL) {
        std::cerr << "[ERROR] The destination client is not registered." << std::endl;
        return;
    }
    
    if (commandParts.size() < 3) {
        std::cerr << "[ERROR] Message not found." << std::endl;
        return;
    }

    std::string message = merge_privmsg(
        std::vector<std::string>(commandParts.begin() + 2, commandParts.end()), " "
    );

    targetClient->sendMessage(
        ":" + client->getUsername() + " PRIVMSG " + commandParts.at(1) + " :" + message
    );
}
