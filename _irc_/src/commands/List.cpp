// #include "../../include/Commands.hpp"

// void List::list( Client* client, Server* srv)
// {
// 	if (srv == NULL)
// 		return (client->sendReply(ERR_NOSUCHSERVER(client->getNickname(), client->getClientIp())));
// 	if (srv->getChannels().empty())
// 		return (client->sendReply(RPL_LISTEND(client->getNickname())));
// 	std::map<std::string, Channel*> channels = srv->getChannels();
// 	std::map<std::string, Channel*>::iterator it = channels.begin();
// 	while (it != channels.end())
// 	{
// 		if (it->second->isUserOnChannel(client))
// 		{
// 			Channel *channel = it->second;
// 			std::string channelName = channel->getChannelName();
// 			std::string channelInfo = channel->getTopic();
// 			client->sendReply(RPL_LIST(client->getNickname(), channelName, std::to_string(channel->getChannelClientCount()), channelInfo));
// 		}
// 		else 
// 		{
// 			Channel *channel = it->second;
// 			std::string channelName = channel->getChannelName();
// 			std::string channelInfo = channel->getTopic();
// 			client->sendReply(RPL_LIST(client->getNickname(), channelName, std::to_string(channel->getChannelClientCount()), channelInfo));
// 		}
// 		it++;
// 	}
// 	client->sendReply(RPL_LISTEND(client->getNickname()));
// }

