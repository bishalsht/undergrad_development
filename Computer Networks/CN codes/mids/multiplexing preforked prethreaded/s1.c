#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <strings.h>
#include <sys/wait.h>

#define MAXCLIENTS 100
#define FORKPOOLSIZE 4
#define THREADPOOLSIZE 4

int sfd;
pthread_t threadpool[THREADPOOLSIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct sockaddr_in server;
int socklen;

struct threadArgs{
	int forkId;
	int threadId;
};

void *threadFunc(void *args)
{
	struct threadArgs *arg = malloc(sizeof(struct threadArgs));
	arg->forkId = ((struct threadArgs*) args)->forkId;
	arg->threadId = ((struct threadArgs*) args)->threadId;
	int i,j,sockln,clientCount;
	int cfd[MAXCLIENTS];
	clientCount = 0;
	printf("ForkId:%d ThreadId:%d started with clientCount:%d\n", arg->forkId, arg->threadId, clientCount);
	fd_set rfds;
	struct sockaddr_in client[MAXCLIENTS];
	while (1)
	{
		sockln = sizeof(struct sockaddr_in);
		int nfds;
		FD_ZERO(&rfds);
		FD_SET(sfd, &rfds);
		for (i=0;i<clientCount;i++)
			FD_SET(cfd[i], &rfds);
		if (clientCount == 0) nfds = sfd+1;
		else nfds = cfd[clientCount-1]+1;
		if (select(nfds, &rfds, NULL, NULL, NULL) == -1)
		{
			printf("Select Error\n");
			exit(0);
		}
		printf("select invoked of forkId: %d threadId: %d\n", arg->forkId, arg->threadId);	
		if (FD_ISSET(sfd, &rfds))
		{
			if (pthread_mutex_trylock(&mutex)==0)
			{
				if ((cfd[clientCount] = accept(sfd, (struct sockaddr*)&client[clientCount], &sockln))==-1)
				{
					printf("Accept Error\n");
					exit(0);
				}
				printf("accept invoked of forkId: %d threadId: %d\n", arg->forkId, arg->threadId);	
				printf("Client %d connected forkId: %d threadId: %d\n", clientCount+1, arg->forkId, arg->threadId);
				clientCount++;
				printf("clientCount: %d\n", clientCount);
				pthread_mutex_unlock(&mutex);
			}
			else
			{
				printf("mutex acquiring failed of forkId: %d threadId: %d\n", arg->forkId, arg->threadId);	
			}
		}
		else
		{
			printf("read section reached of forkId: %d threadId: %d\n", arg->forkId, arg->threadId);	
			for (i=0;i<clientCount;i++)
			{
				if (FD_ISSET(cfd[i], &rfds))
				{
					char buff[1024];
					int k;
					for (k=0;k<1024;k++) buff[k] = '\0';
					if (recv(cfd[i], buff, 1024, 0)==0)
					{
						// printf("Client %d exited\n", i+1);
						// printf("Client %d replaced the exiting client\n", clientCount);
						// clientCount--;
						// dup2(cfd[clientCount], cfd[i]);
						// client[i] = client[clientCount];
						// if (clientCount == 0)
						// {
						// 	if ((cfd[clientCount] = accept(sfd, (struct sockaddr*)&client[clientCount], &socklen))==-1)
						// 	{
						// 		printf("Accept Error\n");
						// 		exit(0);
						// 	}
						// 	printf("Client %d connected\n", clientCount+1);
						// 	clientCount++;
						// }
					}
					else
					{
						printf("Client: %d in ForkId: %d ThreadId: %d\nRecieved: %s \n", i+1, arg->forkId, arg->threadId, buff);
						for (j=0;j<clientCount;j++)
							send(cfd[j], buff, 1024, 0);
					}
					break;
				}
			}
		}
	}
	printf("Exiting forkId: %d threadId: %d\n", arg->forkId, arg->threadId);
}

int main(int argc, char const *argv[])
{
	int i,j,k;
	int childpid[FORKPOOLSIZE];
	socklen = sizeof(server);
	struct threadArgs *args;
	args = malloc (sizeof(struct threadArgs));
	
	if ((sfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		printf("Socket error\n");
		exit(0);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, 8);

	if (bind(sfd, (struct sockaddr *)&server, socklen)==-1)
	{
		printf("Bind error\n");
		exit(0);
	}

	if (listen(sfd, MAXCLIENTS) == -1)
	{
		printf("Listen error\n");
		exit(0);
	}

	int frkVal;
	frkVal = 5;
	for (i=0;i<FORKPOOLSIZE;i++)
	{
		if (frkVal>0)
		{
			frkVal = fork();
			childpid[i] = frkVal;
		}
		else if (frkVal == 0)
		{
			args->forkId = i;
			for (j=0;j<4;j++)
			{
				args->threadId = j+1;
				pthread_create(&threadpool[j], NULL, threadFunc, (void *)args);
				sleep(1);
			}
			pthread_join(threadpool[0], NULL); 
			break;
		}
		else
		{
			printf("fork error\n");
			exit(0);
		}
	}

	for (k=0;k<FORKPOOLSIZE;k++)
		waitpid(childpid[k], NULL, 0);

	return 0;
}