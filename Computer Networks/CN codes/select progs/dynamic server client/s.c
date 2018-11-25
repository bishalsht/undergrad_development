#include <stdio.h>
#include <stdlib.h> 
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAXCLIENT 10

int serverFd, newClientListener, clientCount = 0;
int client[MAXCLIENT];
char clientList[MAXCLIENT][100];

int main(int argc, char const *argv[])
{
	mkfifo("broadcastReciever", 0666);
	mkfifo("newClientListener", 0666);

	serverFd = open("broadcastReciever",O_RDONLY);
	newClientListener = open("newClientListener",O_RDONLY);

	fd_set rfds;
	struct timeval tv;
	int retval=1;

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	FD_SET(serverFd, &rfds);
	FD_SET(newClientListener, &rfds);

	tv.tv_sec = 0;
	tv.tv_usec = 500000;

	while(1)
	{
		if (clientCount==0)
			retval = select(newClientListener+1, &rfds, NULL, NULL, NULL);		
		else
			retval = select(client[clientCount-1]+1, &rfds, NULL, NULL, NULL);
	
		if (retval!=-1)
		{
			if (FD_ISSET(serverFd, &rfds))
			{
				char buff[100];
				int n=read(serverFd, buff, 100);
				if (n==0) 
				{
					perror("All clients exited\n");
					return 0;
				}
				buff[n]='\0';
				printf("Message recieved by server: %s", buff);
				int i;
				for (i=0;i<clientCount;i++)
					write(client[i], buff, n);
			}
			else if(FD_ISSET(newClientListener, &rfds))
			{
				char buff[100];
				int n=read(newClientListener, buff, 100);
				buff[n-1]='\0';
				mkfifo(buff, 0666);
				client[clientCount] = open(buff, O_WRONLY);
				strcpy(clientList[clientCount], buff);
				clientCount++;
			}
			else if (FD_ISSET(0, &rfds))
			{
				char buff[100];
				int n=read(0, buff, 100);
				buff[n-1]='\0';
				if (!strcmp(buff, "exit")) return 0;
			}
		}
		else
		{
			perror("select server error\n");
			exit(1);
		}
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		FD_SET(serverFd, &rfds);
		FD_SET(newClientListener, &rfds);
		int i;
		for (i=0;i<clientCount;i++)
			FD_SET(client[i], &rfds);
	}

	return 0;
}
