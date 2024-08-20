#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <cstdio>

#define PORT 8080

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];
    std::vector<int> clients;

    // Server socket oluşturma
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
        perror("setsockopt SO_REUSEADDR");
        exit(1);
    }

    // Server adresini yapılandırma
    memset(&server_addr, 0, sizeof(server_addr));
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

    fd_set read_fds, write_fds;
    int max_fd = server_socket;

    while (true) {
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        FD_SET(server_socket, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        max_fd = server_socket;

        for (size_t i = 0; i < clients.size(); ++i) {
            FD_SET(clients[i], &read_fds);
            FD_SET(clients[i], &write_fds);
            if (clients[i] > max_fd) max_fd = clients[i];
        }

        int activity = select(max_fd + 1, &read_fds, &write_fds, NULL, NULL);
        if (activity < 0) {
            perror("select error");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        // Yeni bir bağlantı var mı?
        if (FD_ISSET(server_socket, &read_fds)) {
            client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
            if (client_socket < 0) {
                perror("accept");
                close(server_socket);
                exit(EXIT_FAILURE);
            }
            std::cout << "New connection established" << std::endl;
            clients.push_back(client_socket);
        }

        // Terminalden veri okuma
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            std::cout << "Enter message to send to all clients: ";
            std::cin.getline(buffer, sizeof(buffer));
            // Mesajın sonuna yeni bir satır karakteri ekleyin
            std::string message = std::string(buffer) + "\n";
            for (size_t i = 0; i < clients.size(); ++i) {
                if (send(clients[i], message.c_str(), message.length(), 0) == -1) {
                    perror("send");
                }
            }
        }

        // Client'lardan veri okuma
        for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); ) {
            int sd = *it;
            if (FD_ISSET(sd, &read_fds)) {
                int valread = read(sd, buffer, 1024);
                if (valread > 0) {
                    buffer[valread] = '\0';
                    std::cout << "Received from client: " << buffer << std::endl;

                    // Client'tan gelen veriyi diğer client'lara gönder
                    std::string msg(buffer);
                    msg += "\n";  // Mesajın sonuna yeni bir satır karakteri ekleyin
                    for (size_t i = 0; i < clients.size(); ++i) {
                        if (clients[i] != sd && FD_ISSET(clients[i], &write_fds)) {
                            if (send(clients[i], msg.c_str(), msg.length(), 0) == -1) {
                                perror("send");
                            }
                        }
                    }
                } else if (valread == 0) {
                    std::cout << "Client disconnected" << std::endl;
                    close(sd);
                    it = clients.erase(it);
                    continue;
                } else {
                    perror("read error");
                    close(sd);
                    it = clients.erase(it);
                    continue;
                }
            }
            ++it;
        }
    }

    close(server_socket);
    return 0;
}
