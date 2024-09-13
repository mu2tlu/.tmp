#include "../../include/Commands.hpp"


void Cap::cap(Client *client, const std::vector<std::string> commandParts)
{
    if (commandParts.size() < 2)
		return (client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "CAP")));

    std::string cap = commandParts.at(1);
    if (cap == "LS" || cap == "ls" )
    {
        client->sendMessage("/PASS\n/NICK\n/USER\n/JOIN\n/PRIVMSG\n/LIST\n/PART\n/TOPIC\n/KICK\n/QUIT");
    }
    	else
		client->sendReply(ERR_INVALIDCAPCMD(client->getNickname()));
}
