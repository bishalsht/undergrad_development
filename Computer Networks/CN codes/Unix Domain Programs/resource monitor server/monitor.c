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
#include <errno.h>

#ifndef RESOURCECOUNT
#define RESOURCECOUNT 3
#endif

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

int main(int argc, char *const *argv)
{
	int usfd;
	int busy[RESOURCECOUNT];
	int resFd[RESOURCECOUNT];
	struct sockaddr_un address;
	int socklen;

	struct sockaddr_in server, client;
	int cfd;
	int clientCount = 0;
	socklen = sizeof(struct sockaddr_in);
	fd_set rfds;

	int sfd;
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

	socklen = sizeof(address);
	int q;
	for (q=0;q<RESOURCECOUNT;q++)
		busy[q] = 0;

	usfd = socket(AF_UNIX, SOCK_STREAM, 0);

    unlink("./commFile");
    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path,sizeof(address.sun_path), "./commFile");
	bind (usfd, (struct sockaddr*)&address, sizeof(struct sockaddr_un));

	listen(usfd, 5);

	int pid = fork();
	int i;
	for(i=0;i<RESOURCECOUNT;i++)
	{
		if (pid == 0)
		{
			char val[3];
			sprintf(val, "%c", i+'0');
			char* const arg[2]={val};
			execv("./resource",arg);
		}
		else
		{
			resFd[i] = accept(usfd, (struct sockaddr*)&address, &socklen);
			pid = fork();
		}
	}

	if (pid == 0) return 0;
	
	while (1)
	{
		FD_ZERO(&rfds);
		FD_SET(sfd, &rfds);
		for (i=0;i<RESOURCECOUNT;i++)
			FD_SET(resFd[i], &rfds);

		select(resFd[RESOURCECOUNT-1]+1, &rfds, NULL, NULL, NULL);

		if (FD_ISSET(sfd, &rfds))
		{
			if ((cfd = accept(sfd, (struct sockaddr*)&client, &socklen))==-1)
			{
				printf("Accept Error\n");
				exit(0);
			}

			char buff[2];
			recv(cfd, buff, 2, 0);
			i = buff[0]-'0';
			printf("%d\n", i);
			if (busy[i] == 0)
			{
				busy[i] = 1;
				printf("%d is allocated\n", i);
				send_fd(resFd[i], cfd);
				close(cfd);
			}
			else
			{
				send(cfd, "Res Busy\0", 10, 0);
				close(cfd);
			}
		}
		else
		{
			for (i=0;i<RESOURCECOUNT;i++)
			{
				if (FD_ISSET(resFd[i],&rfds))
				{
					char buff[2];
					recv(resFd[i], buff, 2, 0);
					busy[i] = 0;
					printf("Server: %d resource free\n", i);
					break;
				}
			}
		}
	}

	return 0;
}