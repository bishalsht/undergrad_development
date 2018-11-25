#!/usr/bin/tcc -run

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
	while (1)
	{
		char command[20]={'\0'};
		char syntax[80]={'\0'};
		char cmd[105];
		char *pwd=getenv("PWD");
		printf("\n%s $ ",pwd);
		scanf("%s",command);
		scanf("%[^\n]%*c", syntax);
		if (!(strncmp(command,"exit",4))) return 0;
		strcpy(cmd,"./");
		strcat(cmd,command);
		strcat(cmd,".c ");
		strcat(cmd,syntax);
		system(cmd);
	}
}
