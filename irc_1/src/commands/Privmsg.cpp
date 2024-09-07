#include "../../include/Commands.hpp"

void Privmsg::privmsg(Client *client, const std::vector<std::string> commandParts, Server *srv )
{



    // if (commandParts.size() < 2)
    //     return (client->sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG")));
    // if(commandParts.at(0) == "#")
    //     channelmessage();
    // else
    //     privmessage();
    std::cout << "*****************************************************************" << std::endl;
    std::cout << "---------------------- commandParts -----------------------------" << std::endl;
    for (size_t i = 0; i < commandParts.size(); i++)
    {
        std::cout << "commandParts[" << i << "]: " << commandParts[i] << std::endl;
    }
    std::cout << "---------------------- commandParts -----------------------------" << std::endl;
    
    std::cout << "---------------------- client.getList() -------------------------" << std::endl;
    std::cout << "client->getClientIp(): "  << client->getClientIp() << std::endl;
    std::cout << "client->getFd(): "        << client->getFd() << std::endl;
    std::cout << "client->getHasNick(): "   << client->getHasNick() << std::endl;
    std::cout << "client->getHostname(): "  << client->getHostname() << std::endl;
    std::cout << "client->getLoged(): "     << client->getLoged() << std::endl;
    std::cout << "client->getNickname(): "  << client->getNickname() << std::endl;
    std::cout << "client->getPrefix(): "    << client->getPrefix() << std::endl;
    std::cout << "client->getRealname(): "  << client->getRealname() << std::endl;
    std::cout << "client->getUsername(): "  << client->getUsername() << std::endl;
    std::cout << "---------------------- client.getList() -------------------------" << std::endl;

    
    std::cout << "---------------------- srv.getList() ----------------------------" << std::endl;
    // std::cout << "srv->getChannels(): " << srv->getChannel(commandParts[1])->getChannelClientCount() << std::endl;
    // std::cout << "srv->getSrvPass(): " << srv->getSrvPass() << std::endl;
    std::cout << "---------------------- srv.getList() ----------------------------" << std::endl;
    std::cout << "*****************************************************************" << std::endl;

    (void)client;
    (void)commandParts;
    (void)srv;
        
}
