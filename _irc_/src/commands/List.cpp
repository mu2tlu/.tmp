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
            continue; 
        
        std::string channelName = channel->getChannelName();
        std::string channelInfo = channel->getTopic();
        // Kullanıcı sayısını string'e dönüştür
        std::stringstream ss;
        ss << channel->getChannelClientCount();
        std::string userCount = ss.str();

        client->sendReply(RPL_LIST(client->getNickname(), channelName, userCount, channelInfo));
    }
    
    client->sendReply(RPL_LISTEND(client->getNickname()));
}
