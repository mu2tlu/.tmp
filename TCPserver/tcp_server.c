#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 'strings.h' yerine 'string.h' kullanılmalı

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int try_socket() {
    char server_message[256] = "You have reached the server!"; 

    // Create the server socket
    int server_socket; 
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket creation failed");
        exit(1);
    }
    
    // Define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to our specified IP and port
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("bind failed");
        close(server_socket);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("listen failed");
        close(server_socket);
        exit(1);
    }

    // Accept a connection
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        perror("accept failed");
        close(server_socket);
        exit(1);
    }

    // Send the message
    if (send(client_socket, server_message, sizeof(server_message), 0) == -1) {
        perror("send failed");
    }
    // Close the client socket

    close(client_socket);

    // Close the server socket
    close(server_socket);
    
    return 0;
}

int main() {
    while (1)
    {
        try_socket();
        getchar();
    }
    
}