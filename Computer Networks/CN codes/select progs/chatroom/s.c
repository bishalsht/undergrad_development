#include <stdio.h>
#include <stdlib.h> 
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAXCLIENT 10
#define CHATROOMS 4

int serverFd, newClientListener, clientCount = 0;
int client[MAXCLIENT], clientRoom[MAXCLIENT];
int chatrooms[CHATROOMS];
char clientList[MAXCLIENT][100];

int main(int argc, char const *argv[])
{
	printf("Run helper program before running other clients\n");

	mkfifo("newClientListener", 0666);
	newClientListener = open("newClientListener",O_RDONLY);

	fd_set rfds;
	int retval=1;

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	FD_SET(newClientListener, &rfds);

	int i;
	for (i=0;i<CHATROOMS;i++)
	{
		char chatroomName[10];
		strcpy(chatroomName, "CHATROOM");
		chatroomName[8] = i+'1';
		chatroomName[9] = '\0';
		mkfifo(chatroomName, 0666);
		chatrooms[i] = open (chatroomName, O_RDONLY);
		FD_SET(chatrooms[i], &rfds);
		printf("%s is ready\n", chatroomName);
	}

	while(1)
	{
		if (clientCount==0)
			retval = select(newClientListener+1, &rfds, NULL, NULL, NULL);		
		else
			retval = select(client[clientCount-1]+1, &rfds, NULL, NULL, NULL);
	
		if (retval!=-1)
		{
			int i;
			for (i=0;i<CHATROOMS;i++)
			{
				if (FD_ISSET(chatrooms[i], &rfds))
				{
					char buff[100];
					int n=read(chatrooms[i], buff, 100);
					if (n==0) 
					{
						perror("All clients exited\n");
						return 0;
					}
					buff[n]='\0';
					printf("Chatroom %d: %s", i+1, buff);
					int j;
					for (j=0;j<clientCount;j++)
					{
						if (clientRoom[j] == i+1)
						write(client[j], buff, n);
					}
				}
			}
			if(FD_ISSET(newClientListener, &rfds))
			{
				char buff[100];
				int n=read(newClientListener, buff, 100);
				buff[n-1]='\0';
				clientRoom[clientCount] = buff[0]-'1';
				buff[0] = 'c';
				mkfifo(buff, 0666);
				client[clientCount] = open(buff, O_WRONLY);
				strcpy(clientList[clientCount], buff);
				clientCount++;
			}
			else if (FD_ISSET(0, &rfds))
			{
				char buff[100];
				int n=read(0, buff, 100);
				buff[n-1]='\0';
				if (!strcmp(buff, "exit")) return 0;
			}
		}
		else
		{
			perror("select server error\n");
			exit(1);
		}
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		FD_SET(newClientListener, &rfds);
		int i;
		for (i=0;i<CHATROOMS;i++)
			FD_SET(chatrooms[i], &rfds);
	}

	return 0;
}
