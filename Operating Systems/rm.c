#!/usr/bin/tcc -run

#include <stdio.h>
#include <dirent.h>

void recDel(char *arg)
{
    struct dirent *d;
    DIR *dir;
	dir = opendir(arg);
	char buf[256];
    if (dir == NULL)
    {
    	printf("Directory '%s' couldn't be opened\n", arg);
    	return -1;
    }
    while (d = readdir(dir))
    {
    	if (!strcmp(d->d_name,".")) continue;
    	if (!strcmp(d->d_name,"..")) continue;
    	if (d->d_type==	DT_DIR)
    	{
    		sprintf(buf,"%s/%s", arg, d->d_name);
    		recDel(buf);
    		if (remove(buf)) printf("Directory '%s/%s' couldn't be deleted.\n", arg , d->d_name);
    	}
    	else
    	{
    		sprintf(buf,"%s/%s", arg, d->d_name);
    		if (remove(buf)) printf("File '%s' couldn't be deleted\n", buf);
    	}
    }
}

int  main(int argc, char **argv)
{
    if (argc>1)
    {
       	int i;
        if (!strcmp(argv[1],"-r"))
        {
        	for (i=2;i<argc;i++)
        	{
        	    recDel(argv[i]);
        	 	if (remove(argv[i])) printf("Directory %s couldn't be successfully deleted\n",argv[i]);   
        	}
    	}
    	else if (!strcmp(argv[1]),"-i")
    	{
	    	for (i=2;i<argc;i++)
	        {
	        	char rep[5];
	        	printf("rm : remove regular file '%s'\n", argv[i]);
	        	scanf("%s",&rep);
	        	if (rep[0]=='y')
	        	{
	        		if (remove(argv[i])) printf("Non empty directory %s given as argument\n",argv[i]);	
	        	}
	        }
    	}
        else
        {
        	for (i=1;i<argc;i++)
        		if (remove(argv[i])) printf("Non empty directory %s given as argument\n",argv[i]);
        }
    }
    else
    {
        printf("No arguments given\n");    
        exit(1);
    }
    return 0;
}