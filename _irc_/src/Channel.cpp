#include "../include/Channel.hpp"
#include "../include/Server.hpp"

Channel::Channel(const std::string& name, const std::string& key)
: _name(name), _key(key), _userLimit(10), _topic(""){}

Channel::~Channel() {}

bool Channel::isUserOnChannel(Client* client) const
{
    for (std::vector<Client*>::const_iterator it = _curClients.begin(); it != _curClients.end(); ++it)
    {
        if (*it == client)
            return true;
    }
    return false;
}

    void Channel::addClient(Client* client) {
        _curClients.push_back(client);
    }


Channel* Server::getChannel(const std::string& channelName)
{
    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (it != _channels.end())
        return it->second;
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
        if (getOp() == *it) // @ işareti operatör için
            nickList += "@";
        nickList += *it + " ";
    }
    // Boşluk bırakmamak için son boşluğu kaldır
    if (!nickList.empty())
        nickList.pop_back();
        
    broadcastMessage(RPL_NAMREPLY(client->getNickname(), getChannelName(), nickList));
    broadcastMessage(RPL_ENDOFNAMES(client->getNickname(), getChannelName()));
}

void Channel::removeClientFromChannel(Client* client, Server* srv)
{
    std::vector<Client*>::iterator it = _curClients.begin();
    while (it != _curClients.end()) {
        if (*it == client) {
            _curClients.erase(it);
            break; 
        }
        ++it;
    }
    
    // Eğer kanal boşsa ve silinmesi gerekiyorsa, kanalı sil
    if (_curClients.empty())
        srv->removeChannel(getChannelName());  // Kanalı sunucudan kaldır

    else if (!_curClients.empty() && (getOp() == client->getNickname()))
    {
        // Eğer kanal boş değilse ve kullanıcı operator ise, yeni bir operator atama
        setOp(_curClients.front()->getNickname());
        _curClients.front()->sendReply("You are now the operator of " + getChannelName());
    }
    if(!_curClients.empty())
        sendChannelUserList(client);

}
