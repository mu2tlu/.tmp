#include "../../include/Commands.hpp"

void Part::part(Client *client, const std::vector<std::string> commandParts, Server *srv )
{
    if (commandParts.size() < 2)
        return client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "PART"));

    std::vector<std::string> channels;
    std::string message = "";

    std::string channelsString = commandParts[1];
    size_t pos = 0;
    while ((pos = channelsString.find(',')) != std::string::npos) {
        channels.push_back(channelsString.substr(0, pos));
        channelsString.erase(0, pos + 1);
    }
    channels.push_back(channelsString);

    if (commandParts.size() > 2) {
        for (size_t i = 2; i < commandParts.size(); ++i) {
            message += commandParts[i];
            if (i != commandParts.size() - 1) {
                message += " ";
            }
        }
        if (!message.empty() && message[0] == ':')
            message.erase(0, 1);
    }

    for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it)
    {
        const std::string& channelName = *it;
        if (!srv->hasChannel(channelName))
        {
            client->sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
            continue;
        }
        Channel* channel = srv->getChannel(channelName);
        if (!channel)
            continue;

        if (channel->isUserOnChannel(client))
        {
            client->sendReply(RPL_PART(client->getPrefix(), channel->getChannelName(), message));
            channel->broadcastMessage(RPL_PART(client->getPrefix(), channel->getChannelName(), message));
            channel->sendChannelUserList(client);
            channel->removeClientFromChannel(client,srv);
        }
        else
            client->sendReply(ERR_NOTONCHANNEL(client->getNickname(), channelName));
    }
    
}