#include "../include/Channel.hpp"
#include "../include/Server.hpp"

Channel::Channel(const std::string& name, const std::string& key)
: _name(name), _key(key), _userLimit(10), _topic(""){}


Channel::Channel() {}

Channel::~Channel() {}


bool Channel::isUserOnChannel(Client* client) const
{
    for (std::vector<Client*>::const_iterator it = _curClients.begin(); it != _curClients.end(); ++it)
    {
        if (*it == client){
            std::cout << "User " << client->getNickname() << " found in channel " << _name << std::endl;
            return true;
        }
    }
    std::cout << "User " << client->getNickname() << " not found in channel " << _name << std::endl;
    return false;
}

    void Channel::addClient(Client* client) {
        _curClients.push_back(client);
    }


Channel* Server::getChannel(const std::string& channelName)
{
    std::cout << "Searching for channel: " << channelName << std::endl;
    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (it != _channels.end()){
        std::cout << "Channel found: " << channelName << std::endl;
        return it->second;
    }
    std::cout << "Channel not found: " << channelName << std::endl;
    return NULL;
}

bool Server::hasChannel(const std::string& channelName) {
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->first == channelName) {
            return true;  
        }
    }
    return false;
}

std::map<std::string, Channel*> Server::getChannels()
{
	return _channels;
}

void Server::addChannel(Channel* channel) {
    _channels[channel->getChannelName()] = channel;
}

std::vector<std::string> Channel::getChannelClients() const
{
	std::vector<std::string> clients;
	for (std::vector<Client*>::const_iterator it = _curClients.begin(); it != _curClients.end(); ++it)
	{
		clients.push_back((*it)->getNickname());
	}
	return clients;
}

void Channel::broadcastMessage(const std::string& message) const
{
	for (std::vector<Client*>::const_iterator it = _curClients.begin(); it != _curClients.end(); ++it)
	{
		(*it)->sendMessage(message);
	}
}


void Channel::broadcastMessage(const std::string& message, Client* user) const
{
	for (std::vector<Client*>::const_iterator it = _curClients.begin(); it != _curClients.end(); ++it)
	{
		if ((*it) != user)
		{
			(*it)->sendMessage(message);
		}
	}
}


void Channel::sendChannelUserList(Client *client)
{
    std::string nickList;
    std::vector<std::string> nicks = getChannelClients(); 
    for (std::vector<std::string>::iterator it = nicks.begin(); it != nicks.end(); ++it)
    {
        if (getOp() == *it) 
            nickList += "@";
        nickList += *it + " ";
    }
    if (!nickList.empty())
        nickList = nickList.substr(0, nickList.length() - 1);
        
    broadcastMessage(RPL_NAMREPLY(client->getNickname(), getChannelName(), nickList));
    broadcastMessage(RPL_ENDOFNAMES(client->getNickname(), getChannelName()));
}

void Channel::removeClientFromChannel(Client* client, Server* srv)
{
    std::vector<Client*>::iterator it = std::find(_curClients.begin(), _curClients.end(), client);
    if (it != _curClients.end())
    {
        _curClients.erase(it);
        
        std::cout << "Removed user " << client->getNickname() << " from channel " << getChannelName() << std::endl;
        
        if (_curClients.empty())
        {
            std::cout << "Channel " << getChannelName() << " is now empty, removing it." << std::endl;
            srv->removeChannel(getChannelName());
        }
        else if (getOp() == client->getNickname())
        {
            setOp(_curClients.front()->getNickname());
            _curClients.front()->sendReply("You are now the operator of " + getChannelName());
        }
        
        if(!_curClients.empty())
            sendChannelUserList(client);
    }
    else
    {
        std::cout << "User " << client->getNickname() << " not found in channel " << getChannelName() << std::endl;
    }
}
