#include "../../include/Commands.hpp"


void List::list(Client* client, Server* srv)
{
    if (srv == NULL)
        return (client->sendReply(ERR_NOSUCHSERVER(client->getNickname(), client->getClientIp())));
    
    client->sendReply(RPL_LISTSTART(client->getNickname(),srv->getHost()));
    
    std::map<std::string, Channel*> channels = srv->getChannels();
    if (channels.empty())
        return (client->sendReply(RPL_LISTEND(client->getNickname())));

    for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        Channel *channel = it->second;
        if (channel->getChannelClientCount() == 0)
            continue;  // Boş kanalı listeleme
        
        std::string channelName = channel->getChannelName();
        std::string channelInfo = channel->getTopic();
        client->sendReply(RPL_LIST(client->getNickname(), channelName, std::to_string(channel->getChannelClientCount()), channelInfo));
    }
    
    client->sendReply(RPL_LISTEND(client->getNickname()));
}

