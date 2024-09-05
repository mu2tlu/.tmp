#include "../../include/Commands.hpp"

void Privmsg::privmsg(Client *client, const std::vector<std::string> commandParts, Server *srv )
{
    if (commandParts.size() < 2)
        return (client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG")));
    if(commandParts.at(0) == "#")
        channelmessage();
    else
        privmessage();
        
}
