#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <string>
#include "Server.hpp"


class Server;
class Client;
class Channel
{
private:
    // variables
    std::string _name;
    std::string _key;
    size_t _userLimit;
    std::string _topic;
    std::string _opNick;
    std::vector<Client*> _curClients;


public:
    Channel(const std::string& name, const std::string& key = "");
    Channel();


    ~Channel();

    // GETTERS
    std::string getChannelName() const { return _name; }
    std::string getChannelKey() const { return _key; }
    std::string getTopic() const { return _topic; }
    std::string getOp() const { return _opNick; }


    int getUserLimit() const { return _userLimit; }
    int getChannelClientCount() const {return _curClients.size();}

    // SETTERS
    void setKey(const std::string& key) { _key = key; }
    void setTopic(const std::string& topic) { _topic = topic; }
    void setOp(const std::string& opNick) { _opNick = opNick; }
    void setUserLimit(const int userLimit) { _userLimit = userLimit; }


    // ACTIONS
    bool isUserOnChannel(Client* client) const;
    void addClient(Client* client);
    std::vector<std::string> getChannelClients() const;
    void broadcastMessage(const std::string& message) const;
    void broadcastMessage(const std::string& message, Client* user) const;
    void sendChannelUserList(Client *client);
    void removeClientFromChannel(Client* client, Server* srv);
};

#endif