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

#ifndef SERVICES
#define SERVICES 2
#endif

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 100
#endif

#ifndef MAX_DATA
#define MAX_DATA 1024
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

int main(int argc, char const *argv[])
{printf("s2 running\n");
    int clientsFd[MAX_CLIENTS], clientCount;
    clientCount = 1;

    int usfd;
    struct sockaddr_un address;
    int socklen;
    socklen = sizeof(address);

    usfd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path,sizeof(address.sun_path), "./commFile2");
    bind (usfd, (struct sockaddr*)&address, socklen);
    connect(usfd, (struct sockaddr*)&address, socklen);

    clientsFd[0] = recv_fd(usfd);

    fd_set rdfs;
    FD_SET(usfd, &rdfs);
    FD_SET(clientsFd[0], &rdfs);

printf("reached here\n");
    while (1)
    {
        select(clientsFd[clientCount-1]+1, &rdfs, NULL, NULL, NULL);
        if (FD_ISSET(usfd, &rdfs))
        {
            clientsFd[clientCount] = recv_fd(usfd);
            if (clientsFd[clientCount] == -1)
            {
                printf("Server exited\n");
                exit(0);
            }
            clientCount++;
        }
        else
        {
            int i;
            for (i=0;i<clientCount;i++)
            {
                if (FD_ISSET(clientsFd[i], &rdfs))
                {
                    char buff[100];
                    memset(buff, '\0', 100);
                    if (read(clientsFd[i],buff,100)>0)
                        printf("Server 2 recd: %s\n", buff);

                    break;
                }
            }
        }
        FD_ZERO(&rdfs);
        FD_SET(usfd, &rdfs);
        int k;
        for (k=0;k<clientCount;k++)
        FD_SET(clientsFd[k], &rdfs);
    }


    close(usfd);

    return 0;
}