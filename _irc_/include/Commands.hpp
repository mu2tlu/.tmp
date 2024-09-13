#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Utils.hpp"


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

class Join
{
    private:
		Join();
		~Join();
	public:
        static void join(Client *client, const std::vector<std::string> commandParts, Server *srv);
};

class Part
{
    private:
		Part();
		~Part();
	public:
        static void part(Client *client, const std::vector<std::string> commandParts, Server *srv);
};

class Topic
{
    private:
		Topic();
		~Topic();
	public:
        static void topic(Client *client, const std::vector<std::string> commandParts, Server *srv);
};

class List
{
	private:
		List();
		~List(){};
	public:
		static void list( Client* client, Server* srv );
};

class Privmsg
{
    private:
		Privmsg();
		~Privmsg();
	public:
        static void privmsg(Client *client, const std::vector<std::string> commandParts, Server *srv);
};


class Kick
{
	private:
		Kick();
		~Kick();
	public:
    	static void kick(Client* client, const std::vector<std::string> commandParts, Server* srv);
};

class Quit
{
	private:
		Quit();
		~Quit();
	public:
    	static void quit(Client* client, const std::vector<std::string> commandParts, Server* srv);
};


#endif