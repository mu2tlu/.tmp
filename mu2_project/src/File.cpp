#include "../include/File.hpp"

#include <iostream>
#include <vector>

class Client;
class Server;

File::File() {};

File::~File() {};

File::File( const File &copy ) { *this = copy; };

File & File::operator=( const File &copy )
{
	if (this == &copy)
		return (*this);
	this->_name = copy._name;
	this->_path = copy._path;
	this->_sender = copy._sender;
	this->_receiver = copy._receiver;
	return (*this);
};

File::File( std::string name, std::string path, std::string sender, std::string receiver )
    : _name(name), _path(path), _sender(sender), _receiver(receiver) {};

std::string File::getName(const std::string & name) { return name; };
std::string File::getPath(const std::string & path) { return path; };
std::string File::getSender(const std::string & sender) { return sender; };
std::string File::getReceiver(const std::string & receiver) { return receiver; };
std::string File::getBuffer(const std::string & buffer) { return buffer; };
