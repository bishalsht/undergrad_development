#include <stdio.h>
#include <stdlib.h> 
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int chatroomId, newClientListener, serverFd;

int main(int argc, char const *argv[])
{
	mkfifo("newClientListener", 0666);
	newClientListener = open("newClientListener",O_WRONLY);

	printf("Enter which chatroom you would like to join (1-4): ");
	scanf("%d", &chatroomId);

	char chatroomName[10];
	strcpy(chatroomName, "CHATROOM");
	chatroomName[8] = chatroomId+'0';
	chatroomName[9] = '\0';
	mkfifo(chatroomName, 0666);
	printf("%s\n", chatroomName);
	serverFd = open (chatroomName, O_WRONLY);
	
	int pid = getpid();
	char id[20];
	int n;
	n=snprintf(id, 20, "clients/%d",  pid);
	id[n] = '\0';
	id[0] = chatroomId+'1';
	write(newClientListener, id, 20);
	id[0] = 'c';
	mkfifo(id, 0666);
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
				write(serverFd, buff, n+1);
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
			perror("select client error\n");
			exit(1);
		}
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		FD_SET(pid, &rfds);
	}

	return 0;
}
