#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

#ifndef IP1
#define IP1 "127.0.0.1"
#endif

#ifndef IP2
#define IP2 "127.11.11.11"
#endif

int main(int argc, char const *argv[])
{
	int seedp = argv[1][4]+rand();
	int sfd;
	struct sockaddr_in sock;

	while(1)
	{
		sleep(rand_r(&seedp)%5);
		if ((sfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
		{
			perror("train socket: ");
			exit(0);
		}
		sock.sin_family = AF_INET;
		sock.sin_port = htons(atoi(argv[1]));
		sock.sin_addr.s_addr = inet_addr(IP1);
		bzero(&sock.sin_zero, 8);
		if (connect(sfd, (struct sockaddr*)&sock, sizeof(struct sockaddr_in))==-1)
		{
			perror("train connect: ");
			exit(0);
		}

		char platform[5];
		int l=read(sfd, platform, 5);
		platform[l] = '\0';
		printf("Train %s printing: Platform %s is allocated\n", argv[1], platform);

		char announcements[50];

		int i,j;
		
		int announcementsCount = rand_r(&seedp)%4;
		for (i = 0; i < announcementsCount; ++i)
		{
			for (j = 0; j < 49; j++)
				announcements[j] = rand_r(&seedp)%24+'a';
			announcements[49] = '\0';
			send(sfd, announcements, 50, 0);
			sleep(rand_r(&seedp)%3);
		}

		printf("train leaving\n");

		close(sfd);
	}

	return 0;
}