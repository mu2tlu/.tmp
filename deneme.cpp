#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024] = {0};

    // Server socket oluşturma
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Server adresini yapılandırma
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Adresi bağlama
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Dinlemeye başlama
    if (listen(server_socket, 3) < 0) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Select için fd_set yapılarını oluşturma
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);

    int max_fd = server_socket;
    int activity = select(max_fd + 1, &read_fds, &write_fds, NULL, NULL);
    bool new_connection = false;

    // while (true) {
    //     // Her döngüde fd_set'i sıfırlama
    //     FD_ZERO(&read_fds);
    //     FD_ZERO(&write_fds);
    //     FD_SET(server_socket, &read_fds);

    //     if (new_connection) {
    //         FD_SET(client_socket, &read_fds);
    //         FD_SET(client_socket, &write_fds);
    //     }

    //     // select çağrısını yapma
    //     if (activity < 0) {
    //         perror("select error");
    //         close(server_socket);
    //         exit(EXIT_FAILURE);
    //     }

    //     // Yeni bir bağlantı var mı?
    //     if (FD_ISSET(server_socket, &read_fds)) {
    //         if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len)) < 0) {
    //             perror("accept");
    //             close(server_socket);
    //             exit(EXIT_FAILURE);
    //         }
    //         std::cout << "New connection established" << std::endl;
    //         new_connection = true;
    //     }

    //     // Verileri okuma ve yazma işlemleri
    //     if (new_connection) {
    //         if (FD_ISSET(client_socket, &read_fds)) {
    //             int valread = read(client_socket, buffer, 1024);
    //             if (valread > 0) {
    //                 std::cout << "Received: " << buffer << std::endl;
    //                 // Buffer'daki veriyi geri gönderme (echo)
    //                 if (FD_ISSET(client_socket, &write_fds)) {
    //                     send(client_socket, buffer, strlen(buffer), 0);
    //                     std::cout << "Sent: " << buffer << std::endl;
    //                 }
    //             } else if (valread == 0) {
    //                 // Bağlantı kapandı
    //                 close(client_socket);
    //                 new_connection = false;
    //             } else {
    //                 perror("read error");
    //                 close(client_socket);
    //                 new_connection = false;
    //             }
    //         }
    //     }
    // }

    close(server_socket);
    return 0;
}
