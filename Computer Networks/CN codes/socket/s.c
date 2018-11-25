#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <error.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>

#define ERROR  -1
#define MAX_CLIENTS  2
#define MAX_DATA 1024

int main(int argc,char **argv)
{
	struct sockaddr_in server;
	struct sockaddr_in client;
	int sock;
	int new;
	int sockaddr_len=sizeof(struct sockaddr_in);
	int data_len;
	char data[MAX_DATA];


	if((sock=socket(AF_INET,SOCK_STREAM,0))==ERROR)
	{
		perror("server socket:  ");
		exit(-1);
	}


	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[1]));
	server.sin_addr.s_addr=INADDR_ANY;
	bzero(&server.sin_zero,8);

	if(bind(sock,(struct sockaddr *)&server,sockaddr_len)==ERROR)
	{
		perror("bind : ");
		exit(-1);
	}


	if((listen(sock,MAX_CLIENTS))==ERROR)
	{
		perror("listen : ");
		exit(-1);
	}

	while(1)
	{
		printf("going to accept\n");
		if((new=accept(sock,(struct sockaddr *)&client,&sockaddr_len))==ERROR)
		{
			perror("accept : ");
			exit(-1);
		}

		printf("New Client connected from port no %d and IP %s \n",ntohs(client.sin_port),inet_ntoa(client.sin_addr));

		int c;
		c=fork();
		if (c>0)
		{
			recv(sock, data, MAX_DATA,  0);
			printf("parents%d   %s\n",getpid(),data);
			int i;
			for (i=0;i<1024;i++)
			data[i] = '\0';
		}
		else
		{
			recv(new, data, MAX_DATA,  0);
			printf("childs%d  %s\n",getpid(),data);
			int i;
			for (i=0;i<1024;i++)
			data[i] = '\0';
		}

		// printf("Client disconnected \n");

		// close(new);
	}
	close(sock);
}