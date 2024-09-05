#ifndef UTILS_HPP
#define UTILS_HPP


#include <iostream>
#include <string>
#include <cstring>
#include <exception>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <ctime>

#define GET_CURRENT_TIME time(0)
#define FORMAT_TIME(t, buffer) strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", localtime(&t))

#define FAILED_SOCKET_SEND "Failed to send socket"

#define ERR_NEEDMOREPARAMS(source, command) "461 " + source + " " + command + " :Not enough parameters"
#define ERR_INVALIDCAPCMD(source) "410 " + source + " :Invalid CAP subcommand"
#define ERR_ALREADYREGISTERED(source) "462 " + source + " :You may not register"
#define ERR_NONICKNAMEGIVEN(source) "431 " + source + " :No nickname given"
#define ERR_ERRONEUSNICKNAME(source, nick) "432 " + source + " " + nick + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(source, nick) "433 " + source + " " + nick + " :Nickname is already in use"
#define ERR_ALREADYREGISTRED "Already registered\r\n"



static inline void ErrorLogger( std::string messageInfo, const char* fileInfo, int lineInfo, bool isFatal = false )
{
	std::ofstream errorLog;

	errorLog.open("error.log", std::ios::app);
	errorLog << "Error Time: " << __TIME__ << std::endl;
	errorLog << "Error in file: " << fileInfo << ":" << lineInfo << std::endl;
	errorLog << "Error info: " << std::string(strerror(errno)) << std::endl;
	errorLog << "----------------------------------------" << std::endl;
	errorLog.close();

	if (isFatal) {
		throw std::runtime_error(messageInfo);
	}
	write(STDERR_FILENO, messageInfo.c_str(), messageInfo.length());
}

#endif