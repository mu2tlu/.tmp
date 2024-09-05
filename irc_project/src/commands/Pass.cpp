#include "../../include/Commands.hpp"

void Pass::pass(Client *client, const std::vector<std::string> commandParts, Server *srv)
{
     if (commandParts.size() < 2)
        return (client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "PASS")));

    else if(client->isAuth() == true)
        return (client->sendReply(ERR_ALREADYREGISTERED(client->getNickname())));
    else if(commandParts.at(1) == srv->getSrvPass())
    {
        client->authenticate();
        client->sendMessage("Password accepted");
        return;
    }
    if(client->isAuth() == false)
    {
        return (client->sendMessage("Access denied!"));
    }
    return;
}
