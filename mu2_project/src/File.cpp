#include "../include/File.hpp"

#include <iostream>
#include <vector>

class Client;
class Server;

File::File() {
	_name = "";
	_path = "";
	_sender = "";
	_receiver = "";
	_buffer = "";
};

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

std::string File::getName()const { return _name; };
std::string File::getPath() { return _path; };
std::string File::getSender() { return _sender; };
std::string File::getReceiver() { return _receiver; };
std::string File::getBuffer() { return _buffer; };
