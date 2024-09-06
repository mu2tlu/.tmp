#ifndef FILE_HPP
#define FILE_HPP

#include <string>

#define BUFF_SIZE 1024

class File
{
	private:
		File();
		std::string	_name;
		std::string _path;
		std::string _sender;
		std::string _receiver;
		std::string _buffer[BUFF_SIZE];
	public:
		File(std::string Name, std::string Path, std::string Sender, std::string Receiver );
		File(const File &copy);
		~File();
		File & operator = ( const File &copy );
		static std::string getName(const std::string & name);
		static std::string getPath(const std::string & path);
		static std::string getSender(const std::string & sender);
		static std::string getReceiver(const std::string & receiver);
		static std::string getBuffer(const std::string & buffer);
};

#endif
