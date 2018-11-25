#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

int pfd;
int cpid;

void sighandler()
{
	char buff[100];
	read(pfd, buff, 5);
	printf("Process 1 read: %s\n", buff);

	signal(SIGUSR2, sighandler);
	sleep(1);
	kill(cpid, SIGUSR1);
}

int main(int argc, char *const argv[])
{
	if ((pfd = open("file", O_RDONLY, S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
	{
		perror("Cannot open output file\n"); 
		exit(1);
	}

	dup2(pfd, 0);

	cpid=fork();
	if (cpid>0)
	{
		char buff[100];
		signal(SIGUSR2, sighandler);
		sleep(1);
		kill(cpid, SIGUSR1);
		while(1);
	}
	else
	{
		execv("./p2", argv);
	}

	close(pfd);
	
	return 0;
}