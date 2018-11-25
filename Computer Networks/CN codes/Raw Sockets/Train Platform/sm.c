#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

#ifndef TRAINCOUNT
#define TRAINCOUNT 3
#endif

#ifndef PLATFORMCOUNT
#define PLATFORMCOUNT 3
#endif

#ifndef IP1
#define IP1 "127.0.0.1"
#endif

#ifndef IP2
#define IP2 "127.11.11.11"
#endif

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

int main(int argc, char const *argv[])
{
	//Getting Platforms ready

	int usfd[PLATFORMCOUNT];
	int nsfd[PLATFORMCOUNT];
	struct sockaddr_un address;
	int socklen, m;
	socklen = sizeof(address);

	for (m = 0; m < PLATFORMCOUNT; m++)
	{
		usfd[m] = socket(AF_UNIX, SOCK_STREAM, 0);

		char filename[] = "./fileX\0";
		filename[6] = m+'0';

	    unlink(filename);
	    memset(&address, 0, sizeof(struct sockaddr_un));
	    address.sun_family = AF_UNIX;
	    snprintf(address.sun_path,sizeof(address.sun_path), "%s", filename);
		bind (usfd[m], (struct sockaddr*)&address, sizeof(struct sockaddr_un));

		listen(usfd[m], 5);

		nsfd[m] = accept(usfd[m], (struct sockaddr*)&address, &socklen);

		char platformNum[] = "X\0";
		platformNum[0] = m+'0';
		send(nsfd[m], platformNum, strlen(platformNum), 0);

		printf("Platform %d ready\n", m);
	}

	//Getting the routes installed :P

	int sfd;
	int i;
	int incomingTrain[TRAINCOUNT];
	struct sockaddr_in sock;

	for(i=0;i<TRAINCOUNT;i++)
	{
		if ((incomingTrain[i] = socket(AF_INET, SOCK_STREAM, 0))==-1)
		{
			perror("sm train socket: ");
			exit(0);
		}
		sock.sin_family = AF_INET;
		sock.sin_port = htons(atoi(argv[1])+i);
		sock.sin_addr.s_addr = inet_addr(IP1);
		bzero(&sock.sin_zero,8);
		if (bind (incomingTrain[i], (struct sockaddr*)&sock, sizeof(struct sockaddr_in)) == -1)
		{
			perror("sm train bind: ");
			exit(0);
		}
		if (listen(incomingTrain[i], 3)==-1)
		{
			perror("sm train listen: ");
			exit(0);
		}
	}

	//starting the trains

	int pid = fork();
	for (i = 0; i < TRAINCOUNT; ++i)
	{
		if (pid>0)
		pid = fork();
		else
		{
			char temp[6];
			strcpy(temp, argv[1]);
			temp[strlen(temp)-1]+=i;
			execlp("./train", "./train", temp, NULL);
		}
	}

	if (pid == 0) exit(0);

	int trainsfd;
	int trainCount = 0;
	int trainPresent[PLATFORMCOUNT];
	for (i = 0;i < PLATFORMCOUNT;++i)
		trainPresent[i] = 0;
	
	while(1)
	{
		fd_set rfds;
		FD_ZERO(&rfds);
		for (i = 0; i < PLATFORMCOUNT; ++i)
			FD_SET(nsfd[i], &rfds);
		for (i = 0; i < TRAINCOUNT; ++i)
			FD_SET(incomingTrain[i], &rfds);

		select(incomingTrain[TRAINCOUNT-1]+1, &rfds, NULL, NULL, NULL);

		for (i = 0; i < TRAINCOUNT; ++i)
		{
			if (FD_ISSET(incomingTrain[i], &rfds))
			{
				
				if ((trainsfd = accept(incomingTrain[i], (struct sockaddr*)&sock, &socklen))==-1)
				{
					perror("accept: ");
					exit(0);
				}

				//add a line to announce in all platform that a train is arriving (raw socket)

				int j;
				for (j=0;j<PLATFORMCOUNT;j++)
				{
					if (trainPresent[j] == 0)
					{
						send_fd(nsfd[j], trainsfd);
						trainPresent[j] = 1;
						close(trainsfd);
						break;
					}
				}
			}
		}
		for (i = 0; i < PLATFORMCOUNT; ++i)
		{
			if (FD_ISSET(nsfd[i], &rfds))
			{
				char buff[2];
				
				if (read(nsfd[i], buff, 2)>0)
				{
					trainPresent[i] = 0;
				}
				else 
				{
					perror("Platform:");
					exit(0);
				}

			}
		}
	}

	return 0;
}