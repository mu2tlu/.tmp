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
		std::string _buffer;
	public:
		File(std::string Name, std::string Path, std::string Sender, std::string Receiver );
		File(const File &copy);
		~File();
		File & operator = ( const File &copy );
		std::string getName() const;
		std::string getPath();
		std::string getSender();
		std::string getReceiver();
		std::string getBuffer();
};

#endif
