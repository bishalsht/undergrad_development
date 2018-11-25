#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

int main(int argc,char **argv)
{
	DIR *dp=NULL;
	struct dirent *dirPtr=NULL;

	dp=opendir("/proc");
	if (dp==NULL)
	{
		printf("The directory couldn't be opened\n"); 
		exit(1);
	}
	dirPtr=readdir(dp);
	while(dirPtr!=NULL)
	{
		if (dirPtr->d_type==DT_DIR && dirPtr->d_name[0]>='0' && dirPtr->d_name[0]<='9') 
		{
			char cmd[100],ch;
			FILE *fp;
			strcpy(cmd,"/proc/");
			strcat(cmd,dirPtr->d_name);
			strcat(cmd,"/stat");
			fp=fopen(cmd,"r");

			int pid;
			char *comm;
			char state;
			int ppid;
			int pgrp;
			int session;
			int tty_nr;
			int tpgid;
			unsigned int flags;
			unsigned long int minflt;
			unsigned long int cminflt;
			unsigned long int majflt;
			unsigned long int cmajflt;
			unsigned long int utime;
			unsigned long int stime;
			long int cutime;
			long int cstime;
			long int priority;
			long int nice;
			long int num_threads;
			long int itrealvalue;
			unsigned long long int starttime;
			unsigned long int vsize;
			long int rss;
			unsigned long int rsslim;
			unsigned long int startcode;
			unsigned long int endcode;
			unsigned long int startstack;
			unsigned long int kstkesp;
			unsigned long int kstkeip;
			unsigned long int signal;
			unsigned long int blocked;
			unsigned long int sigignore;
			unsigned long int sigcatch;
			unsigned long int wchan;
			unsigned long int nswap;
			unsigned long int cnswap;
			int exit_signal;
			int processor;
			unsigned int rt_priority;
			unsigned int policy;
			unsigned long long int delayacct_blkio_ticks;
			unsigned long int guest_time;
			long int cguest_time;

			fscanf(fp,"%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu ",&pid,comm,&state,&ppid, &pgrp, &session, &tty_nr, &tpgid, &flags, &minflt, &cminflt, &majflt, &cmajflt);
			fscanf(fp,"%lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld %lu ", &utime, &stime, &cutime, &cstime, &priority, &nice, &num_threads, &itrealvalue, &starttime, &vsize, &rss, &rsslim);
			fscanf(fp,"%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu ",&startcode, &endcode, &startstack, &kstkesp, &kstkeip, &signal, &blocked, &sigignore, &sigcatch, &wchan, &nswap, &cnswap);
			fscanf(fp,"%d %d %u %u %llu %lu %ld ",&exit_signal ,&processor, &rt_priority, &policy, &delayacct_blkio_ticks, &guest_time, &cguest_time);

			if (argc==1)
			{	
				if (state=='R')
				printf("%d\t%d\t%s\n",pid, tty_nr, comm );		
			}
			else if (argc==2 && !strcmp(argv[1],"-e"))
			{
				printf("%d\t%s\t",pid, ttyname(tty_nr));
				if (!ttyname(tty_nr)) printf("\t");
				printf("%s\n", comm);
			}
			else if (argc==2 && !strcmp(argv[1],"-d"))
			{
				if (pid!=session)
				{
					printf("%d\t%s\t",pid, ttyname(tty_nr));
					if (!ttyname(tty_nr)) printf("\t");
					printf("%s\n", comm);
				}
			}
			else if (argc==2 && !strcmp(argv[1],"-a"))
			{
				if (pid!=session &&  !ttyname(tty_nr))
				{
					printf("%d\t%s\n",pid, comm);
				}
			}
			else
			{
				printf("Unknown option given\n");
				return -1;
			}

			fclose(fp);
		}
		dirPtr = readdir(dp);
	}
	printf("\n");
	
	if(closedir(dp)!=0)
		printf("Could not close the directory.\n");

	return 0;
}