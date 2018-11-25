#include <stdio.h>
#include <string.h>  
#include <stdlib.h>  
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>   
#include <pthread.h>
#include <sys/select.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/poll.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/un.h>


int recv_fd(int socket)
{
    int sent_fd, available_ancillary_element_buffer_space;
    struct msghdr socket_message;
    struct iovec io_vector[1];
    struct cmsghdr *control_message = NULL;
    char message_buffer[1];
    char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];

    /* start clean */
    memset(&socket_message, 0, sizeof(struct msghdr));
    memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));

    /* setup a place to fill in message contents */
    io_vector[0].iov_base = message_buffer;
    io_vector[0].iov_len = 1;
    socket_message.msg_iov = io_vector;
    socket_message.msg_iovlen = 1;

    /* provide space for the ancillary data */
    socket_message.msg_control = ancillary_element_buffer;
    socket_message.msg_controllen = CMSG_SPACE(sizeof(int));

    recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC);
    perror("recvmsg");
    

    if(message_buffer[0] != 'F')
    {
        /* this did not originate from the above function */
        return -1;
    }

    if((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
    {
        /* we did not provide enough space for the ancillary element array */
        return -1;
    }

    /* iterate ancillary elements */
    for(control_message = CMSG_FIRSTHDR(&socket_message);control_message != NULL;control_message = CMSG_NXTHDR(&socket_message, control_message))
    {
        if( (control_message->cmsg_level == SOL_SOCKET) &&(control_message->cmsg_type == SCM_RIGHTS) )
        {
            sent_fd = *((int *) CMSG_DATA(control_message));
            return sent_fd;
        }
    }
    return -1;
}

int send_fd(int socket, int fd_to_send)
{
    struct msghdr socket_message;
    struct iovec io_vector[1];
    struct cmsghdr *control_message = NULL;
    char message_buffer[1];
    /* storage space needed for an ancillary element with a paylod of length is CMSG_SPACE(sizeof(length)) */
    char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
    int available_ancillary_element_buffer_space;

    /* at least one vector of one byte must be sent */
    message_buffer[0] = 'F';
    io_vector[0].iov_base = message_buffer;
    io_vector[0].iov_len = 1;

    /* initialize socket message */
    memset(&socket_message, 0, sizeof(struct msghdr));
    socket_message.msg_iov = io_vector;
    socket_message.msg_iovlen = 1;

    /* provide space for the ancillary data */
    available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
    memset(ancillary_element_buffer, 0, available_ancillary_element_buffer_space);
    socket_message.msg_control = ancillary_element_buffer;
    socket_message.msg_controllen = available_ancillary_element_buffer_space;

    /* initialize a single ancillary data element for fd passing */
    control_message = CMSG_FIRSTHDR(&socket_message);
    control_message->cmsg_level = SOL_SOCKET;
    control_message->cmsg_type = SCM_RIGHTS;
    control_message->cmsg_len = CMSG_LEN(sizeof(int));
    *((int *) CMSG_DATA(control_message)) = fd_to_send;

    int k=sendmsg(socket, &socket_message, 0);
    perror("sendmsg");
    return k;
}

#ifndef THREADCOUNT
#define THREADCOUNT 3
#endif

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER, mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread[THREADCOUNT];
pthread_t specialThread;

int sfd;
int usfd;
int nsfd;

void *threadFunc(void *args)
{
	int *temp = malloc(sizeof(int));
	temp = (int*) args;
	int threadId = *temp;
	printf("Thread %d started\n", threadId);
	pthread_mutex_lock(&mutex1);
    printf("Thread %d got first mutex\n", threadId);
	pthread_mutex_lock(&mutex2);
	pthread_mutex_unlock(&mutex1);
	int cfd;
	struct sockaddr_in client;
	int socklen = sizeof(struct sockaddr_in);
	fd_set rfds;

	printf("Thread %d waiting to accept\n", threadId);
	if ((cfd = accept(sfd, (struct sockaddr*)&client, &socklen))==-1)
	{
		printf("Accept Error\n");
		exit(0);
	}
	printf("Thread %d accepted\n", threadId);
	pthread_mutex_unlock(&mutex2);

	while (1)
	{
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		FD_SET(cfd, &rfds);
		select (cfd+1, &rfds, NULL, NULL, NULL);
		if (FD_ISSET(0, &rfds))
		{
			
		}
		else
		{
			char buff[1024];
			memset(buff, '\0', 1024);
			read(cfd, buff, 1024);
			printf("ThreadId: %d  Message Recd: %s\n", threadId, buff);
		}
	}
}

void *specialFunc()
{
	pthread_mutex_unlock(&mutex2);
	struct sockaddr_in client;
	int socklen = sizeof(struct sockaddr_in);
	printf("special thread started\n");
	int cfd;
	while(1)
	{
		if ((cfd = accept(sfd, (struct sockaddr*)&client, &socklen))==-1)
		{
			printf("Accept Error\n");
			exit(0);
		}
		send_fd(nsfd, cfd);
		close(cfd);
	}
}

int main(int argc, char const *argv[])
{
	struct sockaddr_un address;
	int socklen;
	socklen = sizeof(address);

	usfd = socket(AF_UNIX, SOCK_STREAM, 0);

    unlink("./commFile");
    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path,sizeof(address.sun_path), "./commFile");
	bind (usfd, (struct sockaddr*)&address, socklen);

	listen(usfd, 5);

	nsfd = accept(usfd, (struct sockaddr*)&address, &socklen);

	struct sockaddr_in server;
	int i;

	if ((sfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		printf("Socket error\n");
		exit(0);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, 8);

	if (bind(sfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in))==-1)
	{
		printf("Bind error\n");
		exit(0);
	}

	if (listen(sfd, 10) == -1)
	{
		printf("Listen error\n");
		exit(0);
	}

	int threadId[THREADCOUNT];

	for (i=0;i<THREADCOUNT;i++)
	{
		threadId[i] = i+1;
		pthread_create(&thread[i], NULL, threadFunc, (void *)&threadId[i]);
		sleep(1);
	}

	pthread_mutex_lock(&mutex2);
	pthread_create(&specialThread, NULL, specialFunc, NULL);
	
	for (i=0;i<THREADCOUNT;i++)
	{
		pthread_join(thread[i], NULL);
	}
	pthread_join(specialThread, NULL);

	return 0;
}