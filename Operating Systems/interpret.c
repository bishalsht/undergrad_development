#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>


char* get_args(char *buf, char **args, int *io);
void execute(char **args,int i, char *file);

int main()
{
	char buf[1024];
	char *args[64];
	char *file;
	while (1)
	{		
		file=NULL;
		char *pwd=getenv("PWD");
		printf("%s $ ",pwd);
		scanf("%[^\n]%*c", buf);
		if(!(strncmp(buf,"exit",4))) return 0;
		int i;
		i=0;
		int *io=&i;
		file=get_args(buf,args,io);
		execute(args,i,file);
	}
}

char* get_args(char *buf, char **args, int *io)
{
	while (*buf!='\0')
	{
		while((*buf!='<')&&(*buf!='>')&&((*buf==' ')||(*buf=='\t')))
		*buf++ = '\0';
		if (*buf=='>') 
		{
			*buf='\0';
			char *file;
			*io = 2; 
			file = buf+1;
			while (*file!='\0'&&((*file==' ')||(*file=='\t')))
				file++;
			return file;
		}
		else if (*buf=='<') 
		{
			*buf='\0';
			char *file;
			*io=1;
			file=buf+1;
			while ((*file==' ')||(*file=='\t'))
				file++;
			return file;
		}
		*args++=buf;
		while((*buf!='\0')&&(*buf!=' ')&&(*buf!='\t')&&(*buf!='>')&&(*buf!='<'))
			buf++;
	}
	*args=NULL;
	return NULL;
}

void execute(char **args,int i,char *file)
{
	int pid, status;
	pid=fork();
	if(pid==0){
		if (i==1)
		{
			int fd0 = open(file, O_RDONLY, 0);
	        dup2(fd0, STDIN_FILENO);
	        close(fd0);
		}
		else if (i==2)
		{
			int fd1 = creat(file, 0644);
	        dup2(fd1, STDOUT_FILENO);
	        close(fd1);
		}
		execvp(*args,args);
		perror(*args);
		exit(1);
	}
	while(wait(&status)!=pid);
}
