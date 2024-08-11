#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>


int main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	int serverfd = socket(AF_INET, SOCK_STREAM, 0);

	struct  sockaddr_in serveraddr, client_addr;
	socklen_t len;

	bzero(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(atoi(av[1]));

	bind(serverfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr));

	listen(serverfd, 5);

	int clientfd = accept(serverfd, (struct sockaddr *)&client_addr, &len);

	char recv_buff[100];

	recv(clientfd, recv_buff, sizeof(recv_buff), 0);
	write(1, recv_buff, strlen(recv_buff));
	printf("%s\n", inet_ntoa(client_addr.sin_addr));
	send(clientfd,"vahdet",6,0);
}
