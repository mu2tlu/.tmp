#include "../../include/Commands.hpp"

std::string merge_privmsg(const std::vector<std::string> &parts, const std::string &delim)
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

std::string strStrimprivmsg(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
        return str;
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}



void sendPrivateMessage(Client *client, std::string target, std::string message, Server *srv)
{
    Client *targetClient = srv->getClient(target);

    targetClient->sendMessage(":" + client->getPrefix() + " PRIVMSG " + target + " :" + message);
}

void Privmsg::privmsg(Client *client, const std::vector<std::string> commandParts, Server *srv )
{
    std::string commandString = merge_privmsg(commandParts, " ");
    size_t targetStart = commandString.find("PRIVMSG") + 8;
    size_t messageStart = commandString.find(" :", targetStart);
    
    std::string target = commandString.substr(targetStart, messageStart - targetStart);
        target = strStrimprivmsg(target);
    std::string message = commandString.substr(messageStart + 2);

    sendPrivateMessage(client, target, message, srv);
}
