#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 'strings.h' yerine 'string.h' kullanılmalı

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main(int ac, char **av) {

    if (ac != 2) {
        fprintf(stderr, "Usage: %s <port>\n", av[0]);
        exit(1);
    }
    // Create a socket
    int network_socket; 
    network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (network_socket == -1) {
        perror("socket creation failed");
        exit(1);
    }
    
    // Specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(av[1])); // Sunucu portunu belirtir
    server_address.sin_addr.s_addr = INADDR_ANY; // Sunucu IP adresini belirtir

    // Connect to the server
    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if (connection_status == -1) {
        perror("connection failed");
        close(network_socket);
        exit(1);
    }

    // Receive data from the server
    char server_response[256];
    memset(server_response, 0, sizeof(server_response)); // 'bzero' yerine 'memset' kullanımı önerilir
    if(recv(network_socket, server_response, sizeof(server_response), 0) == -1) {
        perror("recv failed");
        close(network_socket);
        exit(1);
    } else {
        write(1, server_response, sizeof(server_response));
    }
    while (1)
    {
        memset(server_response, 0, sizeof(server_response)); // 'bzero' yerine 'memset' kullanımı önerilir
        int bytesRead = read(0, server_response, sizeof(server_response) - 1);
        if (bytesRead < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        server_response[bytesRead] = '\0';
        if (send(network_socket, server_response, sizeof(server_response), 0) == -1) {
            perror("receive failed");
        } else {
            // Print out the server's response
            printf("The server sent the data: %s\n", server_response);
        }
    }
    
    // Close the socket
    close(network_socket);
    
    return 0;
}
