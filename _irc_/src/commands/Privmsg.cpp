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
    Client *targetClient = NULL;
    if (commandParts.size() < 2)
        return (client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG")));
    std::string message;
    if (commandParts.size() < 3)
        return (client->sendMessage(ERR_NOTEXTTOSEND(client->getPrefix())));
    else
    {
        message = merge_privmsg(std::vector<std::string>(commandParts.begin() + 2, commandParts.end()), " ");
        if(message[0] == ':')
            message.erase(0, 1);
    }
    if (client->getNickname() != srv->getBot()->getBotnick())
        message = srv->getBot()->processMessage(client->getNickname() + "!" + message);
        
   
    if((commandParts[0] == "PRIVMSG"  || commandParts[0] == "/PRIVMSG") && commandParts.at(1)[0] != '#')
    {
        targetClient = srv->getClient(client->clientFind(commandParts.at(1), srv));
        if (targetClient == NULL)
            return (client->sendMessage(ERR_NOSUCHNICK(client->getPrefix(), commandParts.at(1))));
        else
            targetClient->sendMessage(":" + client->getNickname() + " PRIVMSG " + commandParts.at(1) + " : " + message);
    }

    else if (srv->getChannel(commandParts.at(1)))
    {
        Channel *chan = srv->getChannel(commandParts.at(1));
        if(chan->isUserOnChannel(client)  || (srv->getBot()->getBotnick() == client->getNickname()))
        {
            std::vector<std::string> channelTargetList = srv->getChannel(commandParts.at(1))->getChannelClients();
            for(size_t i = 0; i < channelTargetList.size(); i++)
            {
                targetClient = srv->getClient(client->clientFind(channelTargetList.at(i), srv));
                
                if(targetClient->getFd() != client->getFd() && targetClient->getNickname() != srv->getBot()->getBotnick())
                    targetClient->sendMessage(":" + client->getNickname() + " PRIVMSG " + commandParts.at(1) + " :" + message);
            }
        }
    }
}