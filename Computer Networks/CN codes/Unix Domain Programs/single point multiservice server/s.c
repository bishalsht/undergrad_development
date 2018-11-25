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
#define SERVICES 5
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

int main(int argc, char *const *argv)
{
    struct sockaddr_un commSocket[SERVICES];
    int serviceStarted[SERVICES];
    int commFd[SERVICES];
    int socketFd[SERVICES];
    int socklen;
    socklen = sizeof(struct sockaddr_un);

    struct sockaddr_in server;
    struct sockaddr_in client;
    memset(serviceStarted, 0, sizeof(int)*SERVICES);
    int sock;
    int new;
    int sockaddr_len=sizeof(struct sockaddr_in);
    int data_len;
    char data[MAX_DATA];

    if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("server socket:  ");
        exit(-1);
    }

    server.sin_family=AF_INET;
    server.sin_port=htons(atoi(argv[1]));
    server.sin_addr.s_addr=INADDR_ANY;
    bzero(&server.sin_zero,8);

    if(bind(sock,(struct sockaddr *)&server,sockaddr_len)==-1)
    {
        perror("bind : ");
        exit(-1);
    }

    if((listen(sock,MAX_CLIENTS))==-1)
    {
        perror("listen : ");
        exit(-1);
    }
    
    while(1)
    {
        char buff[2];
        memset(buff, '\0', 2);
        if((new=accept(sock,(struct sockaddr *)&client,&sockaddr_len))==-1)
        {
            perror("accept : ");
            exit(-1);
        }
        read(new, buff, 2);
        int i;
        for (i = 0; i < SERVICES; ++i)
        {
            if (buff[0]-'0' == i+1)
            {
                if (serviceStarted[i]==0)
                {
                    serviceStarted[i]=1;
                    pid_t pid;
                    pid = fork();
                    if (pid == 0)
                    {
                        sleep(1);
                        char execProg[5];
                        sprintf(execProg, "./s%d", i+1);printf("exec: %s\n", execProg);
                        execv(execProg,argv);
                    }
                    else
                    {
                        socketFd[i] = socket(AF_UNIX, SOCK_STREAM, 0);

                        char fileName[12];
                        sprintf(fileName, "./commFile%d", i+1);
                        unlink(fileName);
                        memset(&commSocket[i], 0, sizeof(struct sockaddr_un));
                        commSocket[i].sun_family = AF_UNIX;
                        snprintf(commSocket[i].sun_path,sizeof(commSocket[i].sun_path), "%s", fileName);
                        bind (socketFd[i], (struct sockaddr*)&commSocket[i], sizeof(struct sockaddr_un));

                        listen(socketFd[i], 5);
                        printf("not accepted yet: %s\n", fileName);
                        commFd[i] = accept(socketFd[i], (struct sockaddr*)&commSocket[i], &socklen);
                        send_fd(commFd[i], new);printf("fd sent\n");
                        close(new);
                    }
                }
                else
                {
                    send_fd(commFd[i], new);
                    close(new);
                }
                break;
            }
        }
    }

	return 0;
}