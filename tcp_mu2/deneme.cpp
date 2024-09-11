#include <iostream>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    const char* hostname = "www.example.com"; // Test etmek istediğiniz hostname

    // gethostbyname ile hostname'i çözümle
    struct hostent* host = gethostbyname(hostname);

    // Eğer host verisi alınamazsa hata mesajı ver
    if (host == nullptr) {
        std::cerr << "gethostbyname failed" << std::endl;
        return 1;
    }

    // Host IP adresini al
    struct in_addr** addr_list = (struct in_addr**)host->h_addr_list;

    std::cout << "Hostname: " << hostname << std::endl;

    // IP adreslerini ekrana yazdır
    for (int i = 0; addr_list[i] != nullptr; ++i) {
        std::cout << "IP Address: " << inet_ntoa(*addr_list[i]) << std::endl;
    }

    return 0;
}
