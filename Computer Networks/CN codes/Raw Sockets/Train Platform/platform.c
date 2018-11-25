#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <sys/un.h>
#include <pthread.h>

int usfd, rsfd;
int platformId;

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

void *advertisement()
{
    while(1)
    {
        char buff[100];
        memset(buff, '\0', sizeof(buff));
        int n = recv(rsfd, buff, sizeof(buff), 0);
        buff[n] = '\0';
        printf("Advertisement: %s\n", buff + sizeof(struct iphdr));
    }
}

void *announcement()
{
    while(1)
    {
        int trainsfd;
        trainsfd = recv_fd(usfd);

        printf("new Train\n");
        sleep(1);

        char sendj[2];
        sendj[0] = platformId+'0';
        sendj[1] = '\0';
        send(trainsfd, sendj, strlen(sendj), 0);

        char buff[100];
        memset(buff, '\0', 100);
        while(1)
        {
            if (read(trainsfd, buff, 100) == 0) break;
            printf("Platform %d: train msg: %s\n",platformId, buff);
            memset(buff, '\0', 100);
        }
        printf("Train leaving\n");
        send(usfd, "m\0", 2, 0);
    }

}

int main(int argc, char const *argv[])
{
    struct sockaddr_in socketaddr;

    if ((rsfd = socket(AF_INET, SOCK_RAW, 165))==-1)
    {
        perror("socket");
        exit(0);
    }

    socketaddr.sin_family = AF_INET;
    socketaddr.sin_port = htons(0);
    char net[]="127.0.0.1";
    inet_pton(AF_INET, net, (struct in_addr *)&socketaddr.sin_addr.s_addr);
    bzero(&socketaddr.sin_zero, 8);

    if (bind(rsfd, (struct sockaddr*)&socketaddr, sizeof(struct sockaddr_in))==-1)
    {
        perror("bind");
        exit(0);
    }


    pthread_t announcements, advertisements;
    struct sockaddr_un address;
    int socklen;

    if ((usfd = socket(AF_UNIX, SOCK_STREAM, 0))==-1)
    {
        perror("socket:");
        exit(0);
    }

    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path,sizeof(address.sun_path), "%s", argv[1]);

    if (connect(usfd, (struct sockaddr*)&address, sizeof(struct sockaddr_un))==-1)
    {
        perror("connect:");
        exit(0);
    }

    printf("Platform connected\n");

    char tempBuff[2];
    read(usfd, tempBuff, 2);
    platformId = tempBuff[0] - '0';

    printf("platformId: %d\n", platformId);

    pthread_create(&announcements, NULL, announcement, NULL);
    pthread_create(&advertisements, NULL, advertisement, NULL);

    pthread_join(announcements, NULL);
    pthread_join(advertisements, NULL);

    return 0;
}