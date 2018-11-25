#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *const argv[])
{
	printf("Text 1\n");

	int c = fork();

	if (c>0)
	{
		printf("parent text\n");
		wait(c);
	}
	else 
	{
		sleep(1);
		execv("./ab",argv);
		printf("child text\n");
	}

	return 0;
}