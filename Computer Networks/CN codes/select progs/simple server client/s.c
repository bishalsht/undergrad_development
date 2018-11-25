#include <stdio.h>
#include <stdlib.h> 
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int serverFd, client1Fd, client2Fd, client3Fd;

int main(int argc, char const *argv[])
{
	mkfifo("listen", 0666);
	mkfifo("client1",0666);
	mkfifo("client2",0666);
	mkfifo("client3",0666);

	serverFd = open("listen",O_RDONLY);
	client1Fd = open("client1",O_WRONLY);
	client2Fd = open("client2",O_WRONLY);
	client3Fd = open("client3",O_WRONLY);

	fd_set rfds;
	struct timeval tv;
	int retval=1;

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	FD_SET(serverFd, &rfds);
	FD_SET(client2Fd, &rfds);
	FD_SET(client3Fd, &rfds);

	tv.tv_sec = 0;
	tv.tv_usec = 500000;

	while(1)
	{
		retval = select(serverFd+1, &rfds, NULL, NULL, NULL);
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
				write(client1Fd, buff, n);
				write(client2Fd, buff, n);
				write(client3Fd, buff, n);
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
	}

	return 0;
}
