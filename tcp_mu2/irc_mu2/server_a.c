#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define MAX_CLIENTS 1024
#define BUFFER_SIZE 300000

char buf[BUFFER_SIZE];
int maxfd = 0;
int i = 0;

typedef struct s_client {
    int socketfd;
    socklen_t len;
} t_client;

typedef struct s_server {
    struct sockaddr_in serveraddr;
    int port;
    int serversocket;
    int host;
    fd_set read_set, write_set, current;
    t_client clients[MAX_CLIENTS];
} t_server;

void init_serv(char *port, t_server *server) {
    int serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket == -1) {
        perror("socket");
        exit(1);
    }
    server->serversocket = serversocket;
    FD_ZERO(&server->current);
    maxfd = serversocket;
    FD_SET(serversocket, &server->current);
    bzero(&server->serveraddr, sizeof(server->serveraddr));
    server->serveraddr.sin_addr.s_addr = INADDR_ANY;
    server->serveraddr.sin_port = htons(atoi(port));
    server->serveraddr.sin_family = AF_INET;
}

void socket_reuse(t_server *server) {
    int reuse = 1;
    if (setsockopt(server->serversocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
        perror("setsockopt SO_REUSEADDR");
        exit(1);
    }
}

void init_nonblock(t_server *server) {
    if (fcntl(server->serversocket, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl O_NONBLOCK");
        exit(1);
    }
}

void bind_serv(t_server *server) {
    if (bind(server->serversocket, (const struct sockaddr*)&server->serveraddr, sizeof(server->serveraddr)) == -1) {
        perror("bind");
        exit(1);
    }
}

void listen_serv(t_server *server) {
    if (listen(server->serversocket, 5) == -1) {
        perror("listen");
        exit(1);
    }
}

void accept_serv(t_server *server) {
    int clisock = accept(server->serversocket, (struct sockaddr *)&server->serveraddr, &server->clients[i].len);
    if (clisock == -1) {
        perror("accept");
        return;
    }
    server->clients[i].socketfd = clisock;

    FD_SET(server->clients[i].socketfd, &server->current);
    if (clisock > maxfd) 
        maxfd = clisock;
    i++;
}

void ft_loop(t_server *server) {
    while (1) {
        server->read_set = server->current;
        server->write_set = server->current;
        int activity = select(maxfd + 1, &server->read_set, &server->write_set, NULL, NULL);
        if (activity == -1) {
            perror("select");
            exit(1);
        }

        for (int fd = 0; fd <= maxfd; fd++) {
            if (FD_ISSET(fd, &server->read_set)) {
                if (fd == server->serversocket) {
                    accept_serv(server);
                } else {
                    int ret = recv(fd, buf, sizeof(buf) - 1, 0);
                    if (ret <= 0) {
                        if (ret == 0) {
                            printf("Client disconnected\n");
                        } else {
                            perror("recv");
                        }
                        close(fd);
                        FD_CLR(fd, &server->current);
                    } else {
                        buf[ret] = '\0';
                        printf("Received: %s\n", buf);
                        for (int i = 0; i <= maxfd; i++) {
                            if (FD_ISSET(i, &server->current) && i != server->serversocket && i != fd) {
                                if (send(i, buf, ret, 0) == -1) {
                                    perror("send");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int main(int ac, char **av) {
    if (ac != 2) {
        fprintf(stderr, "Usage: %s <port>\n", av[0]);
        exit(1);
    }
    t_server server;
    memset(&server, 0, sizeof(server));
    init_serv(av[1], &server);
    socket_reuse(&server);
    init_nonblock(&server);
    bind_serv(&server);
    listen_serv(&server);
    ft_loop(&server);
}
