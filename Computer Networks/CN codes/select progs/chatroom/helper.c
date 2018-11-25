#include <stdio.h>
#include <stdlib.h> 
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define CHATROOMS 4

int chatrooms[CHATROOMS];
int newClientListener;

int main(int argc, char const *argv[])
{
	mkfifo("newClientListener", 0666);
	newClientListener = open("newClientListener",O_WRONLY);

	int i;
	for (i=0;i<CHATROOMS;i++)
	{
		char chatroomName[10];
		strcpy(chatroomName, "CHATROOM");
		chatroomName[8] = i+'1';
		chatroomName[9] = '\0';
		mkfifo(chatroomName, 0666);
		chatrooms[i] = open (chatroomName, O_WRONLY);
		printf("%s is ready\n", chatroomName);
	}
	while(1);

	return 0;
}