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

int main(int argc, char const *argv[])
{
    int busy = 0;
	int usfd;
	struct sockaddr_un address;
	int socklen;
	socklen = sizeof(address);
	int cfd;
    fd_set rfds;    

	usfd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path,sizeof(address.sun_path), "./commFile");
	bind (usfd, (struct sockaddr*)&address, sizeof(struct sockaddr_un));
	connect(usfd, (struct sockaddr*)&address, sizeof(struct sockaddr_un));

    printf("Resource %s connected\n", argv[0]);

    while (1)
    {
        FD_ZERO(&rfds);
        FD_SET(usfd, &rfds);
        if (busy == 1)
        {
            FD_SET(cfd, &rfds);
            select(cfd+1, &rfds, NULL, NULL, NULL);
        }
        else
            select(usfd+1, &rfds, NULL, NULL, NULL);   
        if (FD_ISSET(usfd, &rfds))
        {
            if (busy == 0)
            {
                busy = 1;
                cfd = recv_fd(usfd);
                char buff[1024];
                memset(buff, '\0', 1024);
                recv(cfd, buff, 1024, 0);
            }
            else
            {
                printf("something's not right\n");
            }
        }
        else
        {
            char buff[1024];
            memset(buff, '\0', 1024);
            recv(cfd, buff, 1024, 0);
            if (!strncmp(buff,"exit", 4))
            {
                busy = 0;
                close(cfd);
                send(usfd, "d", 1, 0);
            }
            else
            {
                printf("Msg recd by res%s: %s\n", argv[0], buff);
            }
        }
    }

	return 0;
}