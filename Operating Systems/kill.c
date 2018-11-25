#!/usr/bin/tcc -run

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

int main(int argc, char **argv)
{
	if (argc<2)
	{
		printf("No Pid given to kill\n");
		exit(1);
	}

	int i;
	int sgnl;
	if (argv[1][0]=='-') {
		i=2;
		if (argv[1][1]>='0'&&argv[1][1]<='9') 
		{
			sgnl = atoi(&argv[1][1]);
		}
		else
		{
			char *temp=&argv[1][1];
			if (!strcmp(temp,"SIGKILL")) sgnl=SIGKILL; 
			else if (!strcmp(temp,"SIGSTOP")) sgnl=SIGSTOP; 
			else if (!strcmp(temp,"SIGCONT")) sgnl=SIGCONT; 
			else if (!strcmp(temp,"SIGCHLD")) sgnl=SIGCHLD; 
			else if (!strcmp(temp,"SIGTSTP")) sgnl=SIGTSTP; 
			else if (!strcmp(temp,"SIGTTIN")) sgnl=SIGTTIN; 
			else if (!strcmp(temp,"SIGTTOU")) sgnl=SIGTTOU; 
			else 
			{
				printf("Given signal is not available\n");
				exit(1);
			}
		}
	}
	else{
		sgnl=SIGKILL;
		i=1;
	}
	for (;i<argc;i++)
		kill (atoi(argv[i]),sgnl);
	return 0;
}