#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <netinet/udp.h>

#define MAXCLIENTS 100


int main(int argc, char const *argv[])
{
	int sfd;
	fd_set rdfs;
	struct sockaddr_in server, from;
	int socklen = sizeof(struct sockaddr_in);

	if ((sfd = socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
		printf("Socket error\n");
		exit(0);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&server.sin_zero, 8);
	
	while (1)
	{
		char buff[1024];
		FD_ZERO(&rdfs);
		FD_SET(0, &rdfs);
		FD_SET(sfd, &rdfs);
		select(sfd+1, &rdfs, NULL, NULL, NULL);
		if (FD_ISSET(0, &rdfs))
		{
			scanf("%s", buff);
			sendto(sfd, buff, 1024, 0, (struct sockaddr *)&server, socklen);
		}
		else if (FD_ISSET(sfd, &rdfs))
		{
			recvfrom(sfd, buff, 1024, 0, (struct sockaddr *)&from, &socklen);
			printf("Recd from server: %s\n", buff);
		}
	}

	return 0;
}