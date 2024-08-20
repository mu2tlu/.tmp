#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 'strings.h' yerine 'string.h' kullanılmalı

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int try_socket(char *av) 
{
    // Create the server socket
    int server_socket; 
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket creation failed");
        exit(1);
    }

    int so_reuseaddr_set = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr_set, sizeof(int));

    // Define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(av));
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

    fd_set fd_read;
    FD_ZERO(&fd_read);

    fd_set fd_read_buff;
    FD_ZERO(&fd_read_buff);
    
    fd_set fd_write;
    FD_ZERO(&fd_write);
    
    fd_set fd_write_buff;
    FD_ZERO(&fd_write_buff);


    // FD_CLR(server_socket, &fd_write);
    int client_socket;
    char server_buff[256] = "You have reached the server!"; 
    while (1) 
    {
        fd_read = fd_read_buff;
        fd_write = fd_read_buff;
        int activity = select(server_socket + 1, &fd_read, &fd_write, NULL, NULL);
        if(activity < 0)
            perror("select");
        else 
            if(activity > 0){
                if (FD_ISSET(&fd_read, server_socket)) {
                    memset(server_buff, 0, 256);
                    recv(server_socket, server_buff,)
                }
            }
            





        // if (activity > 0) {
        //     if (FD_ISSET(server_socket, &fd_read)) {
        //         memset(server_message, 0, sizeof(server_message));
        //          if (recv(server_socket, server_message, sizeof(server_message), 0) == -1) {
        //             perror("send failed");
        //          }
        //     }
        //     if (FD_ISSET(server_socket, &fd_write)) {
        //         memset(server_message, 0, sizeof(server_message));
        //          int bytesRead = read(server_socket, server_message, sizeof(server_message) - 1);
        //         if (bytesRead < 0) {
        //             perror("read");
        //             exit(EXIT_FAILURE);
        //         }
        //         write(1, server_message, sizeof(server_message));
        //     }
        //     // Accept a connection
        //     client_socket = accept(server_socket, NULL, NULL);
        //     if (client_socket == -1) {
        //         perror("accept failed");
        //         close(server_socket);
        //         exit(1);
        //     }
        //     if (send(client_socket, server_message, sizeof(server_message), 0) == -1) {
        //         perror("send failed");
        //     }
        //     // recv the message
        //     if (recv(client_socket, server_message, sizeof(server_message), 0) == -1) {
        //         perror("send failed");
        //     }
        // }
    }            
    
    // Close the client socket
    close(client_socket);

    return 0;
}


int main(int ac, char **av) {
    if (ac == 2) {
        try_socket(av[1]);
    } else {
        perror("Usage: ./tcp_server <port>");
        exit(1);
    }
    return 0;
}