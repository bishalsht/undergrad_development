#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

int pfd;
int ppid;
int pid;

void sighandler()
{
	char buff[100];
	read(pfd, buff, 5);
	printf("Process 2 read: %s\n", buff);

	signal(SIGUSR1, sighandler);
	sleep(1);
	kill(ppid, SIGUSR2);
}

int main(int argc, char const *argv[])
{
	dup2(0,pfd);
	ppid = getppid();
	pid = getpid();
	signal(SIGUSR1, sighandler);
	while(1);

	return 0;
}