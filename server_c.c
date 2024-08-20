#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    int clients[FD_SETSIZE];
    int num_clients = 0;

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

    while (1) {
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        FD_SET(server_socket, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        max_fd = server_socket;

        for (int i = 0; i < num_clients; ++i) {
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
            printf("New connection established\n");
            if (num_clients < FD_SETSIZE) {
                clients[num_clients++] = client_socket;
            } else {
                printf("Too many clients\n");
                close(client_socket);
            }
        }

        // Terminalden veri okuma
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            printf("Enter message to send to all clients: ");
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                // Mesajın sonuna yeni bir satır karakteri ekleyin
                size_t len = strlen(buffer);
                if (len > 0 && buffer[len - 1] == '\n') {
                    buffer[len - 1] = '\0'; // '\n' karakterini kaldır
                }
                // Mesajı tüm client'lara gönder
                for (int i = 0; i < num_clients; ++i) {
                    if (send(clients[i], buffer, strlen(buffer), 0) == -1) {
                        perror("send");
                    }
                }
            }
        }

        // Client'lardan veri okuma
        for (int i = 0; i < num_clients; ++i) {
            int sd = clients[i];
            if (FD_ISSET(sd, &read_fds)) {
                int valread = read(sd, buffer, BUFFER_SIZE - 1);
                if (valread > 0) {
                    buffer[valread] = '\0'; // Null-terminate the buffer
                    printf("Received from client: %s\n", buffer);

                    // Mesajı diğer client'lara gönder
                    for (int j = 0; j < num_clients; ++j) {
                        if (clients[j] != sd) {
                            if (send(clients[j], buffer, valread, 0) == -1) {
                                perror("send");
                            }
                        }
                    }
                } else if (valread == 0) {
                    printf("Client disconnected\n");
                    close(sd);
                    // Client'ı listeden çıkar
                    for (int k = i; k < num_clients - 1; ++k) {
                        clients[k] = clients[k + 1];
                    }
                    --num_clients;
                    --i; // İndeksi güncelle
                } else {
                    perror("read error");
                    close(sd);
                    // Client'ı listeden çıkar
                    for (int k = i; k < num_clients - 1; ++k) {
                        clients[k] = clients[k + 1];
                    }
                    --num_clients;
                    --i; // İndeksi güncelle
                }
            }
        }
    }

    close(server_socket);
    return 0;
}
