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
    int rsfd,rsfd2;
    struct sockaddr_in sock;

    if ((rsfd = socket(AF_INET, SOCK_RAW, 165))==-1)
    {
        perror("socket");
        exit(0);
    }
	if ((rsfd2 = socket(AF_INET, SOCK_RAW, 165))==-1)
    {
        perror("socket");
        exit(0);
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(0);
    char net[]="127.0.0.1";
    inet_pton(AF_INET, net, (struct in_addr *)&sock.sin_addr.s_addr);
    bzero(&sock.sin_zero, 8);

    if (bind(rsfd, (struct sockaddr*)&sock, sizeof(struct sockaddr_in))==-1)
    {
        perror("bind");
        exit(0);
    }

    char buff[100];
    char buff2[100];
    int n = recv (rsfd, &buff, sizeof (buff), 0);
    int n2 = recv (rsfd2, &buff2, sizeof (buff2), 0);
    buff[n] = '\0';
    buff2[n2] = '\0';
    int i = sizeof(struct iphdr);
    printf("%s\n%s\n", buff+i,buff2+i);
    return 0;
}