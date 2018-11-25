#include <stdio.h>
#include <string.h>  
#include <stdlib.h>  
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>   
#include <pthread.h>
#include <sys/select.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/poll.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/un.h>

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

	send(sfd, argv[2], 10, 0);

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
			send(sfd, buff, 1024, 0);
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
			if (!strncmp(buff, "Res", 3))
			{
				printf("Resource is Busy\n");
				exit(0);
			}
		}
	}

	return 0;
}