#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	mkfifo("listen", 0666);
	mkfifo("client3",0666);

	int serverFd, client3Fd;
	serverFd = open("listen",O_WRONLY);
	client3Fd = open("client3",O_RDONLY);

	int pid;
	pid = fork();

	if (pid>0)
	{
		while(1)
		{
			char buff[100]={'\0'};
			read(client3Fd, buff, 100);
			printf("Message recieved by client 3 : %s\n", buff);
		}
	}
	else
	{
		while (1)
		{
			char buff[100]={'\0'};
			// printf("Enter message to send to server : ");
			scanf("%s",buff);
			write(serverFd, buff, 100);
		}
	}

	return 0;
}