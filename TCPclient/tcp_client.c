#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 'strings.h' yerine 'string.h' kullanılmalı

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main() {
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
    server_address.sin_port = htons(9002);
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
    memset(server_response, 0, 256); // 'bzero' yerine 'memset' kullanımı önerilir
    if (recv(network_socket, server_response, sizeof(server_response), 0) == -1) {
        perror("receive failed");
    } else {
        // Print out the server's response
        printf("The server sent the data: %s\n", server_response);
    }
    
    // Close the socket
    close(network_socket);
    
    return 0;
}
