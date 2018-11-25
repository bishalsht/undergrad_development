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

#ifndef IP1
#define IP1 "127.0.0.1"
#endif

#ifndef IP2
#define IP2 "127.10.10.10"
#endif

#ifndef IP3
#define IP3 "127.111.111.111"
#endif

int main(int argc, char const *argv[])
{
    int rsfd,rsfd2, rsfd3;
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

    if ((rsfd3 = socket(AF_INET, SOCK_RAW, 165))==-1)
    {
        perror("socket");
        exit(0);
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(0);
    char net[]=IP1;
    inet_pton(AF_INET, net, (struct in_addr *)&sock.sin_addr.s_addr);
    bzero(&sock.sin_zero, 8);

    if (bind(rsfd, (struct sockaddr*)&sock, sizeof(struct sockaddr_in))==-1)
    {
        perror("bind");
        exit(0);
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(0);
    char net3[]=IP1;
    inet_pton(AF_INET, net3, (struct in_addr *)&sock.sin_addr.s_addr);
    bzero(&sock.sin_zero, 8);

    if (connect(rsfd3, (struct sockaddr*)&sock, sizeof(struct sockaddr_in))==-1)
    {
        perror("connect");
        exit(0);
    }

    fd_set rfds;

    while(1)
    {
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        FD_SET(rsfd, &rfds);
        FD_SET(rsfd2, &rfds);
        FD_SET(rsfd3, &rfds);

        select(rsfd3+1, &rfds, NULL, NULL, NULL);

        if (FD_ISSET(0, &rfds))
        {
            char buff[1024];
            int n = read(0, buff, 1024);
            buff[n] = '\0';

            // printf("send to everyone: %s", buff);

            sock.sin_family = AF_INET;
            sock.sin_port = htons(0);
            char net1[]=IP1;
            inet_pton(AF_INET, net1, (struct in_addr *)&sock.sin_addr.s_addr);
            bzero(&sock.sin_zero, 8);
            sendto(rsfd2, buff, strlen(buff), 0, (struct sockaddr*)&sock, sizeof(struct sockaddr_in));

            sock.sin_family = AF_INET;
            sock.sin_port = htons(0);
            char net2[]=IP2;
            inet_pton(AF_INET, net2, (struct in_addr *)&sock.sin_addr.s_addr);
            bzero(&sock.sin_zero, 8);
            sendto(rsfd2, buff, strlen(buff), 0, (struct sockaddr*)&sock, sizeof(struct sockaddr_in));

            // sock.sin_family = AF_INET;
            // sock.sin_port = htons(0);
            // char net3[]=IP3;
            // inet_pton(AF_INET, net3, (struct in_addr *)&sock.sin_addr.s_addr);
            // bzero(&sock.sin_zero, 8);
            // sendto(rsfd2, buff, strlen(buff), 0, (struct sockaddr*)&sock, sizeof(struct sockaddr_in));

            send(rsfd3, buff, strlen(buff), 0);
        }
        else if (FD_ISSET(rsfd, &rfds))
        {
            char buff[100];
            int n = recv (rsfd, &buff, sizeof (buff), 0);
            buff[n] = '\0';
            int i = sizeof(struct iphdr);
            printf("rsfd bind\n");
            printf("message recd at rsfd + size of iphdr: %s", buff+i);
        }
        else if (FD_ISSET(rsfd2, &rfds))
        {
            char buff[100];
            int n = recv (rsfd2, &buff, sizeof (buff), 0);
            buff[n] = '\0';
            int i = sizeof(struct iphdr);
            printf("rsfd2 socket\n");
            printf("message recd at rsfd2 + size of iphdr: %s", buff+i);
        }
        else if (FD_ISSET(rsfd3, &rfds))
        {
            char buff[100];
            int n = recv (rsfd3, &buff, sizeof (buff), 0);
            buff[n] = '\0';
            int i = sizeof(struct iphdr);
            printf("rsfd3 connect\n");
            printf("message recd at rsfd3 + size of iphdr: %s", buff+i);
        }
    }

    return 0;
}