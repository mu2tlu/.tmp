#include "../../include/Commands.hpp"




void Topic::topic(Client* client, std::vector<std::string> commandParts, Server* srv)
{
	if (commandParts.size() < 2)
    return (client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC")));

	std::string channelName = commandParts[1];
    Channel* channel = srv->getChannel(channelName);

    if (!channel)
		return (client->sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName)));
	
	if (commandParts.size() == 2)
	{
        if (channel->getTopic().empty())
            return (client->sendReply(RPL_NOTOPIC(client->getNickname(), channelName)));
        else
            return (client->sendReply(RPL_TOPIC(client->getNickname(), channelName, channel->getTopic())));
    }
	if(commandParts.size() > 2)
	{
		if (channel->getOp() != client->getNickname())
			return (client->sendReply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName)));
		else
		{
			std::string newTopic;
			for (size_t i = 2; i < commandParts.size(); ++i)
			{
				newTopic += commandParts[i];
        		if (i != commandParts.size() - 1)
            		newTopic += " ";
			}
			channel->setTopic(newTopic);  // Yeni topic ayarla
			channel->broadcastMessage(RPL_TOPIC(client->getNickname(), channelName, newTopic));
		}
	}
}
