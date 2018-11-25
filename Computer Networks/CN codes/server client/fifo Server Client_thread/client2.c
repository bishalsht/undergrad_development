#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

pthread_t input_thread,output_thread;
int serverFd, client2Fd;

void *input_func()
{
	while (1)
	{
		char buff[100]={'\0'};
		// printf("Enter message to send to server : ");
		scanf("%s",buff);
		write(serverFd, buff, 100);
	}
}

void *output_func()
{
	while(1)
	{
		char buff[100]={'\0'};
		read(client2Fd, buff, 100);
		printf("Message recieved by client 2 : %s\n", buff);
	}
}

int main(int argc, char const *argv[])
{
	mkfifo("listen", 0666);
	mkfifo("client2",0666);

	serverFd = open("listen",O_WRONLY);
	client2Fd = open("client2",O_RDONLY);

	pthread_create(&input_thread, NULL, input_func, NULL);
	pthread_create(&output_thread, NULL, output_func, NULL);

	pthread_join(input_thread,NULL);

	return 0;
}