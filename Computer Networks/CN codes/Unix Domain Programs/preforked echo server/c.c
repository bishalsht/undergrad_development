#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>

int main(int argc, char const *argv[])
{
	struct sockaddr_in remote_server;
	int sfd; 
	fd_set rfds;

	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Socket Error\n");
		exit(0);
	}

	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons(atoi(argv[1]));
	remote_server.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&remote_server.sin_zero, 8);

	if (connect(sfd, (struct sockaddr *)&remote_server, sizeof(struct sockaddr_in)) == -1)
	{
		printf("Connect Error\n");
		exit(0);
	}

	while (1)
	{
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		FD_SET(sfd, &rfds);

		if (select(sfd+1, &rfds, NULL, NULL, NULL) == -1)
		{
			printf("Select error\n");
			exit(0);
		}

		if (FD_ISSET(0, &rfds))
		{
			char buff[1024];
			int k;
			for (k=0;k<1024;k++) buff[k] = '\0';
			read(0, buff, 1024);
			send(sfd, buff, strlen(buff), 0);
		}
		else if (FD_ISSET(sfd, &rfds))
		{
			char buff[1024];
			int k;
			for (k=0;k<1024;k++) buff[k] = '\0';
			if (recv(sfd, buff, 1024, 0) == 0)
			{
				printf("Server exited\n");
				return 0;
			}
			printf("Recived from server: %s\n", buff);
		}
	}

	return 0;
}