#include "../../include/Commands.hpp"
#include "../../include/File.hpp"

void SendF::sendFile(Client *client, const std::vector<std::string> commandParts, Server *srv)
{
     //--------------------DEBUG--------------------
    for (std::vector<std::string>::const_iterator it = commandParts.begin(); it != commandParts.end(); it++)
    {
        std::cout << "commandParts: " << *it << std::endl; // DEBUG
    }
    //--------------------DEBUG--------------------
	(void)client;
	// (void)commandParts;
	(void)srv;
};