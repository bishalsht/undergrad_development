/*
*	This header consists of all the header files, function definitions, macros that would be commonly used in the programs
*	made during the course of the syllabus.
*	System V semaphore is implemented.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>			/* for pipes */
#include <string.h>			/* for null terminated string functions */
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <poll.h>
#include <sys/select.h>
#include <netinet/in.h>		/* for sockaddr_in of IPv4 */
#include <strings.h>		/* for bzero() */
#include <sys/socket.h>
#include <time.h>
#include <arpa/inet.h>		/* for inet_pton() */
#include <netinet/in.h>		/* for hton() and ntoh() */
#include <sys/un.h>			/* for unix domain sockets */
#include <sys/uio.h>		/* for struct iovec */

#define LISTENQ 1024

char IPADDR[] = "127.0.0.1";
//char IPADDR[] = "172.30.106.71";	// using ifconfig eth0

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

void semwait(int semid, int semindex)
{
	struct sembuf sb;
	sb.sem_num = semindex;
	sb.sem_op = -1;			/* -1 for wait operation */
	sb.sem_flg = 0;
	if((semop(semid, &sb, 1)) == -1)
	{
		perror("semwait error!\n");
		exit(0);
	}
}

void sempost(int semid, int semindex)
{
	struct sembuf sb;
	sb.sem_num = semindex;
	sb.sem_op = 1;			/* 1 for post operation */
	sb.sem_flg = 0;
	if((semop(semid, &sb, 1)) == -1)
	{
		perror("sempost error!\n");
		exit(0);
	}
}

/* takes a pointer to a string and returns a pointer to a reversed string */
char* strrev(char *str)
{
	char *p;
	int l, i, j;
	
	l = strlen(str);
	p = (char*)malloc(l+1);
	p[l] = '\0';
	for (i=0, j=l-1 ; i<l ; ++i,--j)
		p[i] = str[j];
	return p;	
}

void err_sys(char *msg, int err)
{
	printf("err = %d\n", err);
	perror (msg);		// prints message corresponding to errno variable
	exit(1);
}

