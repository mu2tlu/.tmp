#include "../../include/Commands.hpp"
void Kick::kick(Client* client, const std::vector<std::string> commandParts, Server* srv)
{
    if (commandParts.size() < 3)
        return client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "KICK"));
    std::string channelName = commandParts[1];
    std::string targetNickname = commandParts[2];
    std::string reason = "No reason specified";
       if (commandParts.size() > 3)
    {
        reason = commandParts[3];
        for (size_t i = 4; i < commandParts.size(); ++i)
            reason += " " + commandParts[i];
    }
    Channel* channel = srv->getChannel(channelName);
    if (!channel)
        return client->sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
    if (channel->getOp() != client->getNickname())
        return client->sendReply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName));
    Client* targetClient = srv->getClient(targetNickname);
    if (!targetClient)  // targetClient NULL controll
        return client->sendMessage(ERR_NOSUCHNICK(client->getNickname(), targetNickname));
    if (!channel->isUserOnChannel(targetClient))
        return client->sendReply(ERR_USERNOTINCHANNEL(client->getNickname(), targetNickname, channelName));
    channel->removeClientFromChannel(targetClient, srv);
    std::string message = ":" + client->getPrefix() + " KICK " + channelName + " " + targetNickname + " :" + reason + "\n";
    channel->broadcastMessage(RPL_KICK(client->getPrefix(), channelName, targetClient->getNickname(),reason));
    targetClient->sendMessage(RPL_KICK(client->getPrefix(), channelName, targetClient->getNickname(),reason));
    targetClient->sendMessage("You have been kicked from the channel " + channelName + " by " + client->getNickname() + " Reason: " + reason);
    channel->sendChannelUserList(client);
}