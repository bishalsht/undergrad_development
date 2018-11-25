#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	mkfifo("listen", 0666);
	mkfifo("client1",0666);
	mkfifo("client2",0666);
	mkfifo("client3",0666);

	int serverFd, client1Fd, client2Fd, client3Fd;
	serverFd = open("listen",O_RDONLY);
	client1Fd = open("client1",O_WRONLY);
	client2Fd = open("client2",O_WRONLY);
	client3Fd = open("client3",O_WRONLY);

	while (1)
	{
		char buff[100] = {'\0'};
		read(serverFd, buff, 100);
		printf("Message recieved by server : %s\n", buff);
		write(client1Fd, buff, 100);
		write(client2Fd, buff, 100);
		write(client3Fd, buff, 100);
	}

	return 0;
}