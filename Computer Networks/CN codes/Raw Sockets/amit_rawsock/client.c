#include "/home/amit/Desktop/Dropbox/sockets/socket_world.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int s;
    struct sockaddr_in src_addr;
    char packet[50];

    if ((s = socket(AF_INET, SOCK_RAW, 170)) < 0)
        err_sys ("socket error", errno);

    memset(packet, 0, sizeof(packet));
    socklen_t *len = (socklen_t *)sizeof(src_addr);
    int fromlen = sizeof(src_addr);

    while(42)
    {
        if (recvfrom(s, &packet, sizeof(packet), 0, (struct sockaddr *)&src_addr, &fromlen) < 0)
            perror("uh oh:");

        int i = sizeof(struct iphdr); // print the payload
        printf("i = %d\n", i);
        printf("packet+%d = %s<--\n", i, packet+i);
    }
    
    return(0);
}