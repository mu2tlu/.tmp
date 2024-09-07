#include "../../include/Commands.hpp"
#include "../../include/File.hpp"


std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void SendF::sendFile(Client *client, const std::vector<std::string> commandParts, Server *srv)
{
    //--------------------DEBUG--------------------
    // for (std::vector<std::string>::const_iterator it = commandParts.begin(); it != commandParts.end(); it++)
    // {
    //     std::cout << "commandParts: " << *it << std::endl; // DEBUG
    // }
    File file(commandParts[1], commandParts[2], commandParts[3], commandParts[4]);


    srv->getFiles().insert(std::make_pair(intToString(client->getFd()), file));

    std::map<std::string, File>& filesMap = srv->getFiles();
    std::map<std::string, File>::const_iterator it = filesMap.find(intToString(client->getFd()));

    if (it != filesMap.end()) {
        // Anahtar bulundu, dosya üzerinde işlem yapabilirsiniz
        std::cout << "File found: " << it->second.getName() << std::endl;
    } else {
        // Anahtar bulunamadı
        std::cerr << "Error: Key not found in files map\n";
    }
    
    //--------------------DEBUG--------------------

	(void)client;
	(void)commandParts;
	(void)srv;
};