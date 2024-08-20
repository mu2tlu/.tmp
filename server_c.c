#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080
#define MAX_CLIENTS 10

int main() {
    int server_socket, client_socket, max_fd, activity, valread;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];
    int clients[MAX_CLIENTS];
    fd_set read_fds;
    int i;

    // Soket oluşturma
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // SO_REUSEADDR seçeneğini ayarlama
    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
        perror("setsockopt SO_REUSEADDR");
        close(server_socket);
        exit(EXIT_FAILURE);
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
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Bağlı client'ları sıfırlama
    memset(clients, 0, sizeof(clients));

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        max_fd = server_socket;

        // Client socket'ları fd_set'e ekleme
        for (i = 0; i < MAX_CLIENTS; ++i) {
            if (clients[i] > 0) {
                FD_SET(clients[i], &read_fds);
                if (clients[i] > max_fd) {
                    max_fd = clients[i];
                }
            }
        }

        activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select error");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        // Yeni bağlantı varsa
        if (FD_ISSET(server_socket, &read_fds)) {
            client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
            if (client_socket < 0) {
                perror("accept");
                close(server_socket);
                exit(EXIT_FAILURE);
            }
            printf("New connection established\n");

            // Yeni client'ı ekleme
            for (i = 0; i < MAX_CLIENTS; ++i) {
                if (clients[i] == 0) {
                    clients[i] = client_socket;
                    break;
                }
            }
        }

        // Terminalden veri okuma
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            printf("Enter message to send to all clients: ");
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                // Mesajın sonuna yeni bir satır karakteri ekleme
                size_t len = strlen(buffer);
                if (buffer[len - 1] != '\n') {
                    buffer[len] = '\n';
                    buffer[len + 1] = '\0';
                }
                for (i = 0; i < MAX_CLIENTS; ++i) {
                    if (clients[i] > 0) {
                        if (send(clients[i], buffer, strlen(buffer), 0) == -1) {
                            perror("send");
                        }
                    }
                }
            }
        }

        // Client'lardan veri okuma
        for (i = 0; i < MAX_CLIENTS; ++i) {
            if (FD_ISSET(clients[i], &read_fds)) {
                valread = read(clients[i], buffer, sizeof(buffer) - 1);
                if (valread > 0) {
                    buffer[valread] = '\0';
                    printf("Received from client: %s", buffer);

                    // Client'tan gelen veriyi diğer client'lara gönder
                    for (int j = 0; j < MAX_CLIENTS; ++j) {
                        if (clients[j] > 0 && clients[j] != clients[i]) {
                            if (send(clients[j], buffer, strlen(buffer), 0) == -1) {
                                perror("send");
                            }
                        }
                    }
                } else if (valread == 0) {
                    printf("Client disconnected\n");
                    close(clients[i]);
                    clients[i] = 0;
                } else {
                    perror("read error");
                    close(clients[i]);
                    clients[i] = 0;
                }
            }
        }
    }

    close(server_socket);
    return 0;
}
