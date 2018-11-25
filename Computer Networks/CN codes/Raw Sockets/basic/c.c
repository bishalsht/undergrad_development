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

int main(int argc, char const *argv[])
{
    int rsfd;
    struct sockaddr_in sock;

    if ((rsfd = socket(AF_INET, SOCK_RAW, 165))==-1)
    {
        perror("socket");
        exit(0);
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(0);
    char net[]="127.0.0.1";
    inet_pton(AF_INET, net, (struct in_addr *)&sock.sin_addr.s_addr);
    bzero(&sock.sin_zero, 8);

    if (connect(rsfd, (struct sockaddr*)&sock, sizeof(struct sockaddr_in))==-1)
    {
    	perror("connect");
    	exit(0);
    }

    if (send(rsfd, "hey how is it going\0", 21, 0)==-1)
    {
        perror("send");
        exit(0);
    }

	return 0;
}