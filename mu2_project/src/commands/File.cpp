#include "../../include/File.hpp"

#include <iostream>

File::File() {};

File::~File() {};

File::File( const File &copy ) { *this = copy; };

File & File::operator=( const File &copy )
{
	if (this == &copy)
		return (*this);
	this->Name = copy.Name;
	this->Path = copy.Path;
	this->sender = copy.sender;
	this->receiver = copy.receiver;
	return (*this);
};

File::File( std::string Name, std::string Path, std::string Sender, std::string Receiver )
    : Name(Name), Path(Path), sender(Sender), receiver(Receiver) {};