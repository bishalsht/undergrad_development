#include <stdio.h>
#include <stdlib.h> 
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int serverFd, client2Fd;

int main(int argc, char const *argv[])
{
	mkfifo("listen", 0666);
	mkfifo("client2",0666);

	serverFd = open("listen",O_WRONLY);
	client2Fd = open("client2",O_RDONLY);

	fd_set rfds;
	struct timeval tv;
	int retval=1;

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	FD_SET(client2Fd, &rfds);

	tv.tv_sec = 0;
	tv.tv_usec = 500000;

	while(1)
	{
		retval = select(client2Fd+1, &rfds, NULL, NULL, NULL);
		if (retval!=-1)
		{
			if (FD_ISSET(0, &rfds))
			{
				char buff[100];
				int n=read(0, buff, 100);
				buff[n] = '\0';
				write(serverFd, buff, n);
			}
			else if (FD_ISSET(client2Fd, &rfds))
			{
				char buff[100];
				int n=read(client2Fd, buff, 100);
				if (n==0)
				{
					printf("Server has shut down\n");
					return 0;
				}
				buff[n] = '\0';
				printf("Recieved from server: %s", buff);
			}
		}
		else
		{
			perror("select client 2 error\n");
			exit(1);
		}
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		FD_SET(client2Fd, &rfds);
	}

	return 0;
}
