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
	int sfd, clientCount;
	struct sockaddr_in server, from[MAXCLIENTS], temp;
	int socklen = sizeof(struct sockaddr_in);
	clientCount = 0;

	if ((sfd = socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
		printf("Socket error\n");
		exit(0);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, 8);
	
	if (bind(sfd, (struct sockaddr *)&server, socklen)==-1)
	{
		printf("Bind error\n");
		exit(0);
	}

	while(1)
	{	
		char buff[1024];
		bzero(&buff, 1024);
		recvfrom(sfd, buff, 1024, 0, (struct sockaddr *)&temp, &socklen);
		int i;
		for (i=0;i<clientCount;i++)
		{
			if (ntohs(temp.sin_port) == ntohs(from[i].sin_port))
				break;
		}
		if (i==clientCount)
		{
			from[clientCount].sin_family = temp.sin_family;
			from[clientCount].sin_port = temp.sin_port;
			from[clientCount].sin_addr.s_addr = temp.sin_addr.s_addr;
			clientCount++;
		}
		printf("Recieved message: %s\n", buff);
		for (i=0;i<clientCount;i++)
			sendto(sfd, buff, 1024, 0, (struct sockaddr *)&from[i], socklen);
	}

	return 0;
}