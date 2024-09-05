#ifndef FILE_HPP
#define FILE_HPP

#include <iostream>

class File
{
	public:
		std::string	Name;
		std::string Path;
		std::string sender;
		std::string receiver;

	private:
		File();

	public:
		File( std::string Name, std::string Path, std::string Sender, std::string Receiver );
		File( const File &copy );
		~File();
		File & operator = ( const File &copy );
};

#endif
