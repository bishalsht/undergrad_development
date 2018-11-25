#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>

#define MAXCLIENTS 100

int main(int argc, char const *argv[])
{
	struct sockaddr_in server, client[MAXCLIENTS];
	int cfd[MAXCLIENTS];
	int clientCount = 0;
	int socklen = sizeof(struct sockaddr_in);
	fd_set rfds;
	int i,j;

	int sfd;
	if ((sfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
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

	if (listen(sfd, MAXCLIENTS) == -1)
	{
		printf("Listen error\n");
		exit(0);
	}

	if ((cfd[clientCount] = accept(sfd, (struct sockaddr*)&client[clientCount], &socklen))==-1)
	{
		printf("Accept Error\n");
		exit(0);
	}
	printf("Client 1 connected\n");
	clientCount++;

	while (1)
	{
		FD_ZERO(&rfds);
		FD_SET(sfd, &rfds);
		for (i=0;i<clientCount;i++)
			FD_SET(cfd[i], &rfds);
		if (select(cfd[clientCount-1]+1, &rfds, NULL, NULL, NULL) == -1)
		{
			printf("Select Error\n");
			exit(0);
		}
		if (FD_ISSET(sfd, &rfds))
		{
			if ((cfd[clientCount] = accept(sfd, (struct sockaddr*)&client[clientCount], &socklen))==-1)
			{
				printf("Accept Error\n");
				exit(0);
			}
			printf("Client %d connected\n", clientCount+1);
			clientCount++;
			continue;
		}
		for (i=0;i<clientCount;i++)
		{
			if (FD_ISSET(cfd[i], &rfds))
			{
				char buff[1024];
				int k;
				for (k=0;k<1024;k++) buff[k] = '\0';
				if (recv(cfd[i], buff, 1024, 0)==0)
				{
					printf("Client %d exited\n", i+1);
					printf("Client %d replaced the exiting client\n", clientCount);
					clientCount--;
					dup2(cfd[clientCount], cfd[i]);
					client[i] = client[clientCount];
					if (clientCount == 0)
					{
						if ((cfd[clientCount] = accept(sfd, (struct sockaddr*)&client[clientCount], &socklen))==-1)
						{
							printf("Accept Error\n");
							exit(0);
						}
						printf("Client %d connected\n", clientCount+1);
						clientCount++;
					}
				}
				else
				{
					printf("Recieved: %s\t from client: %d\n", buff, i+1);
					for (j=0;j<clientCount;j++)
						send(cfd[j], buff, 1024, 0);
				}
				break;
			}
		}
	}

	return 0;
}