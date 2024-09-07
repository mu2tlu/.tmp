#include "../../include/Commands.hpp"


void Channel::setTopic(std::string topic, Client* user)
{
	std::string top_ = topic;
	if (user->getNickname()!= getOp())
	{
		user->sendMessage(":" + user->getHostname() + " 482 " + user->getNickname() + " " + getChannelName() + " :You're not channel operator\r\n");
		return;
	}
	if (topic.empty())
	{
		user->sendMessage(":" + user->getHostname() + " 331 " + user->getNickname() + " " + getChannelName() + " :No topic is set\r\n");
		return;
	}
	this->_topic.erase();
	this->_topic = top_;
	broadcastMessage("TOPIC " + getChannelName() + " :" + topic, user);
}

void Topic::topic(Client* client, std::vector<std::string> commandParts, Server* srv) {


    std::string channelName = commandParts.at(1);
    std::string topic = (commandParts.size() > 2) ? commandParts.at(2) : "";
	if (topic[1] == ':')
		topic = topic.substr(1, topic.size() - 1);
    if (channelName.empty() || channelName[0] != '#') {
        return client->sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
    }

    Channel* channel = srv->getChannel(channelName);
    if (channel == NULL) {
        return client->sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
    }

    if (!channel->isUserOnChannel(client)) {
        return client->sendReply(ERR_NOTONCHANNEL(client->getNickname(), channelName));
    }
    if (topic.empty() && commandParts.size() == 2) {
        if (channel->getTopic().empty()) {
            return client->sendReply(RPL_NOTOPIC(client->getNickname(), channelName));
        } else {
            client->sendReply(RPL_TOPIC(client->getNickname(),client->getClientIp(),channelName,channel->getTopic()));
            return;
        }
	}
    std::string absolutetopic = (topic[0] == ':') ? topic.substr(1) : topic.substr(1, topic.size() - 1);
    if (absolutetopic.empty()) {
        absolutetopic = "No topic is set";
    }
    channel->setTopic(absolutetopic, client);
    std::string message = ":" + client->getPrefix() + " TOPIC " + channelName + " :" + absolutetopic + "\n";
    channel->broadcastMessage(message, client);
    client->sendReply(RPL_TOPIC(client->getNickname(),client->getClientIp(),channelName,channel->getTopic()));
}