#include "socket_world.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SRC "127.0.0.1"
//#define SRC "172.30.106.72"

int main(int argc, char **argv)
{
    int s;
    struct sockaddr_in seraddr;
    char packet[60];

    if ((s = socket(AF_INET, SOCK_RAW, 165)) < 0)
        err_sys ("socket error", errno);

    bzero (&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(0);   // not needed
    if (inet_pton (AF_INET, SRC, (void*)&(seraddr.sin_addr.s_addr)) == -1)  // foreign ip is set
        err_sys("invalid IP address", errno);
    if (connect (s, (struct sockaddr*) &seraddr, sizeof (seraddr)) < 0)
        err_sys("error", errno);

    memset(packet, 0, sizeof(packet));
    socklen_t *len = (socklen_t *)sizeof(seraddr);
    int fromlen = sizeof(seraddr);

    while(42)
    {
        //if (recvfrom (s, &packet, sizeof(packet), 0, (struct sockaddr *)&seraddr, &fromlen) < 0)
        //    err_sys ("eRRor", errno);

        if (recv (s, &packet, sizeof (packet), 0) < 0)
            err_sys ("errno", errno);

        int i = sizeof(struct iphdr); // print the payload
        printf("i = %d\n", i);
        printf("packet+%d = %s<--\n", i, packet+i);
    
        /*for(; i < sizeof(packet); i++)
        {
            printf("%c", packet[i]);
        }
            printt("\n");*/

        //print complete packet
        //write (1, packet, sizeof (packet));
    }

    return(0);
}