#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *const argv[])
{
	printf("Pid : %d\n", getpid());
	int pipes[2];
	pipe(pipes);
	int c=1;
	c=fork();
	if (c>0)
	{
		close(pipes[0]);
		// int fd;
		// dup2(0,fd);
		char buff[1024]={'\0'};
		read(0, buff, 1024);
		printf("Recieved by %d : %s\n", getpid(),buff);
		dup2(pipes[1],1);
		printf("%s",buff);
		wait(NULL);
	}
	else if(c==0)
	{
		close(pipes[1]);
		argv[1][0]--;
		if (argv[1][0]=='0') return 0;
		dup2(pipes[0],0);
		execv("./pipe", argv);
	}

	return 0;
}