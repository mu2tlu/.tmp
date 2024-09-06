#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include "Client.hpp"
#include "Server.hpp"

class Client;
class Server;

class CommandParser
{
    private:
        CommandParser();
        ~CommandParser();
    public:
        static void commandParser( const char *command, Client* client, Server* srv);
        static void handleCommand( Client* client, std::vector<std::string> commandParts, Server* srv );

};

class Cap
{
	private:
		Cap();
		~Cap();
	public:
		static void cap( Client* client, std::vector<std::string> commandParts);
};

class Pass
{
    private:
		Pass();
		~Pass();
	public:
        static void pass(Client *client, const std::vector<std::string> commandParts, Server *srv);
};

class Nick
{
    private:
		Nick();
		~Nick();
	public:
        static void nick(Client *client, const std::vector<std::string> commandParts, Server *srv);
};

class User
{
    private:
		User();
		~User();
	public:
        static void user(Client *client, const std::vector<std::string> commandParts, Server *srv);
};

class Privmsg
{
    private:
		Privmsg();
		~Privmsg();
	public:
        static void privmsg(Client *client, const std::vector<std::string> commandParts, Server *srv);
};

class Notice
{
    private:
		Notice();
		~Notice();
	public:
        static void notice(Client *client, const std::vector<std::string> commandParts, Server *srv);
};

class SendF
{
	private:
		SendF();
		~SendF();
	public:
		static void sendFile(Client *client, const std::vector<std::string> commandParts, Server *srv);
};




#endif