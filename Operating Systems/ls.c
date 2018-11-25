#!/usr/bin/tcc -run

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
int main()
{
	
	char *curr_dir = NULL;
	DIR *dp = NULL;
	struct dirent * directoryPtr = NULL;
	int count=1;
	curr_dir = getenv("PWD");
	
	if(curr_dir == NULL)
	{
		printf("Could not get the current directory.\n");
		return -1;
	}
	dp = opendir(curr_dir);

	if(dp == NULL)
	{
		printf("Could not open the working directory.\n");
		return -1;
	}
	
	directoryPtr = readdir(dp);
	
	while(directoryPtr!=NULL)
	{

		printf("%s\n",directoryPtr->d_name);
		directoryPtr = readdir(dp);
	}
	printf("\n");
	
	if(closedir(dp)!=0)
		printf("Could not close the directory.\n");
	
	return 0;
}