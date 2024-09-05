#include "../../include/Commands.hpp"

void User::user(Client *client, const std::vector<std::string> commandParts, Server *srv)
{
    (void)srv; //Dikkat
    if (!client->getHasNick())
        return(client->sendMessage(": You must set a nickname before this"));
    if (commandParts.size() < 4)
        return (client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "USER")));
    else if (!client->getUsername().empty())
        return (client->sendReply(ERR_ALREADYREGISTRED));
    else
    {
        client->setUsername(commandParts.at(1));
        client->setHostname(commandParts.at(2));

        
        if(commandParts.size() > 4)
        {
            std::string realname;
            for (size_t i = 3; i < commandParts.size(); ++i)
            {
                realname += commandParts[i];
                if (i != commandParts.size() - 1)
                {
                    realname += " ";
                }
            }
            if (!realname.empty() && realname[0] == ':')
            {
                realname.erase(0, 1);
            }
            client->setRealname(realname);
        }
    }
    return;
}
