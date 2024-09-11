#include "../../include/Commands.hpp"

// void find(const std::string& name, Client* client, Server* server);

void joinChannel(Client* client, const std::string& channelName, const std::vector<std::string>& commandParts, Server* srv)
{
    std::string channelPass;

    if (commandParts.size() >= 3)
        channelPass = commandParts.at(2);
    else
        channelPass = "";

    if (channelName.size() < 2 || channelName.at(0) != '#')
        return client->sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));

    Channel* channel;
    if (srv->hasChannel(channelName))
    {
        channel = srv->getChannel(channelName);

        if (channel->isUserOnChannel(client))
            return client->sendReply(ERR_USERONCHANNEL(client->getNickname(), channelName));

        if (channel->getChannelKey() != channelPass)
            return client->sendReply(ERR_BADCHANNELKEY(client->getNickname(), channelName));

        if (channel->getUserLimit() <= channel->getChannelClientCount())
            return client->sendReply(ERR_CHANNELISFULL(client->getNickname(), channelName));

        channel->addClient(client); 
        if (channel->getUserLimit() == 1)
            channel->setOp(client->getNickname());
        
        std::cout << "Client " << client->getNickname() << " has entered '" << channelName << "'" << std::endl;
    }
    else
    {
        channel = new Channel(channelName);
        channel->addClient(client);
        channel->setOp(client->getNickname());
        channel->setKey(channelPass);
        srv->addChannel(channel);
        std::cout << "New channel '" << channelName << "' created and client " << client->getNickname() 
              << " has entered as the operator." << std::endl;
    }
    channel->broadcastMessage(RPL_JOIN(client->getPrefix(), channel->getChannelName()));
    channel->sendChannelUserList(client);
}

std::string find(std::string target, Client *client, Server *srv)//BOT isim düzenlencek
{
    std::map<int, Client> client_map = srv->getClientMap();

    for (std::map<int, Client>::iterator it = client_map.begin(); it != client_map.end(); it++)
    {
        if (it->second.getNickname() == target)
            return target;
    }
    return "";
}

int deneme(Server *srv, std::string nickname) //BOT
{
    if(srv->_channelLisTemp.size() == 0) {
        srv->_channelLisTemp.push_back(nickname);
        return 1;
    }

    for(int i = 0; i < srv->_channelLisTemp.size(); i++) {
        if(srv->_channelLisTemp[i] != nickname){
            srv->_channelLisTemp.push_back(nickname);
            return 1;
        }
    }
    return 0;
}

void Join::join(Client *client, const std::vector<std::string> commandParts, Server *srv )
{
    if (commandParts.size() < 2)
        return (client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "JOIN")));
    std::string channel = commandParts.at(1);
    if (!channel.empty())
        joinChannel(client, channel, commandParts, srv);

    if(srv->getChannel(channel)->getChannelClientCount() >= 1) //BOT
    {
        Client *targetClient = NULL;

        std::vector<std::string> channelTargetList = srv->getChannel(commandParts.at(1))->getChannelClients();

        for(size_t i = 0; i < channelTargetList.size(); i++) {
            targetClient = srv->getClient(find(channelTargetList.at(i), client, srv));
            if(deneme(srv, targetClient->getNickname()) == 1) //BOT
               srv->getBot()->WelcomeMsg(commandParts.at(1), targetClient->getNickname());
        }
    }
}
