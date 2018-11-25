#!/usr/bin/tcc -run

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

int main()
{
	DIR *dp=NULL;
	struct dirent *dirPtr=NULL;

	dp=opendir("/proc");
	if (dp==NULL)
	{
		printf("The directory couldn't be opened\n"); 
		exit(1);
	}
	printf("%s\t\t\t%s\n", "PID","CMD");
	dirPtr=readdir(dp);
	while(dirPtr!=NULL)
	{
		if (dirPtr->d_type==DT_DIR && dirPtr->d_name[0]>='0' && dirPtr->d_name[0]<='9') 
		{
			printf("%s\t\t\t", dirPtr->d_name);
			char cmd[100],ch;
			FILE *fp;
			strcpy(cmd,"/proc/");
			strcat(cmd,dirPtr->d_name);
			strcat(cmd,"/status");
			fp=fopen(cmd,"r");
			int flag=0;
			while((ch=getc(fp))!='\n')
			{
				if (flag)
				printf("%c", ch);
				if (ch=='\t') flag=1;
			}
			printf("\n");
			fclose(fp);
		}
		dirPtr = readdir(dp);
	}
	printf("\n");
	
	if(closedir(dp)!=0)
		printf("Could not close the directory.\n");

	return 0;
}