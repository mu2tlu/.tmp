#include "../include/Commands.hpp"

std::vector<std::string> splitString(std::string str, const std::string &delimiter)
{
    std::vector<std::string> parts;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        token = str.substr(0, pos);
        parts.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    parts.push_back(str);
    return parts;
}

void CommandParser::commandParser(const char *command, Client *client, Server *srv)
{
    std::string commandString(command);
    if (commandString.empty())
    {
        return;
    }

    commandString.erase(std::remove(commandString.begin(), commandString.end(), '\n'), commandString.end());
    
    std::vector<std::string> commandParts = splitString(commandString, " ");
    if (commandParts.empty())
    {
        return;
    }

    for (size_t i = 0; i < commandParts[0].length(); ++i)
    {
        commandParts.at(0)[i] = toupper(commandParts.at(0)[i]);
    }
    handleCommand(client, commandParts, srv);
}

void CommandParser::handleCommand(Client *client, std::vector<std::string> commandParts, Server *srv)
{
    if (commandParts.size() < 1)
        return client->sendMessage(":" + client->getClientIp() + " 461 " + client->getNickname() + " Command :Not enough parameters");
    
    if (commandParts.at(0) == "/CAP" || commandParts.at(0) == "CAP")
        Cap::cap(client, commandParts);
    else if (commandParts.at(0) == "/PASS" || commandParts.at(0) == "PASS")
        Pass::pass(client, commandParts, srv);
    else if((commandParts.at(0) == "/NICK" || commandParts.at(0) == "NICK"))
        Nick::nick(client, commandParts, srv);
    else if((commandParts.at(0) == "/USER" || commandParts.at(0) == "USER"))
        User::user(client, commandParts, srv);
    else if(client->isAuth() == true && client->getLoged() == true)
    {
        if (commandParts.at(0) == "/JOIN" || commandParts.at(0) == "JOIN")
            Join::join(client, commandParts, srv);
        else if (commandParts.at(0) == "/PRIVMSG" || commandParts.at(0) == "PRIVMSG")
            Privmsg::privmsg(client, commandParts, srv);
        else if (commandParts.at(0) == "/LIST" || commandParts.at(0) == "LIST")
            List::list(client, srv);
        else if (commandParts.at(0) == "/PART" || commandParts.at(0) == "PART")
            Part::part(client, commandParts, srv);
        else if (commandParts.at(0) == "/TOPIC" || commandParts.at(0) == "TOPIC")
            Topic::topic(client, commandParts, srv);
        else 
        {
            std::cout << "giris basarili ve buradasin "<< std::endl;
            return (client->sendMessage("unknown command"));
        }
    }
    else
     return (client->sendMessage("You must login firstly"));

}

/* size=3 
commandParts[0] = "PART"
commandParts[1] = "#deneme"
commandParts[2] = ":Leaving"
*/
