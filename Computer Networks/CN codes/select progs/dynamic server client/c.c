#include <stdio.h>
#include <stdlib.h> 
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int serverFd, newClientListener;

int main(int argc, char const *argv[])
{
	mkfifo("broadcastReciever", 0666);
	mkfifo("newClientListener", 0666);

	serverFd = open("broadcastReciever",O_WRONLY);
	newClientListener = open("newClientListener",O_WRONLY);

	int pid = getpid();

	char id[20];
	int n;
	n=snprintf(id, 20, "clients/%d",  pid);
	id[n] = '\0';
	mkfifo(id, 0666);
	write(newClientListener, id, 20);
	pid = open(id, O_RDONLY);

	fd_set rfds;
	struct timeval tv;
	int retval=1;

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	FD_SET(pid, &rfds);

	while(1)
	{
		retval = select(pid+1, &rfds, NULL, NULL, NULL);
		if (retval!=-1)
		{
			if (FD_ISSET(0, &rfds))
			{
				char buff[100];
				int n=read(0, buff, 100);
				buff[n] = '\0';
				write(serverFd, buff, n);
			}
			else if (FD_ISSET(pid, &rfds))
			{
				char buff[100];
				int n=read(pid, buff, 100);
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
			perror("select client 1 error\n");
			exit(1);
		}
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		FD_SET(pid, &rfds);
	}

	return 0;
}
