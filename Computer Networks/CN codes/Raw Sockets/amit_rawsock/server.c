#include "socket_world.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//#define DEST "172.30.106.71"
#define DEST "127.0.0.1"
#define SRC "127.0.0.1"
//#define SRC "172.30.106.71"

int main(int argc, char **argv)
{

	int s, n;
 	struct sockaddr_in dst_addr, src_addr;
 	char packet[60];

 	struct iphdr *ip = (struct iphdr *)packet;                // first 20 bytes of packet go to header

 	if((s = socket(AF_INET, SOCK_RAW, 165)) < 0)
 	  err_sys ("eRror", errno);

    const int optval = 1;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, &optval, sizeof (optval)) < 0)
    err_sys ("error", errno);

 	dst_addr.sin_family = AF_INET;
 	dst_addr.sin_port = 0; 				// not needed in SOCK_RAW
 	inet_pton(AF_INET, DEST, (struct in_addr *)&dst_addr.sin_addr.s_addr);
 	memset(dst_addr.sin_zero, 0, sizeof(dst_addr.sin_zero));

 	src_addr.sin_family = AF_INET;
 	src_addr.sin_port = 0;
 	inet_pton(AF_INET, SRC, (struct in_addr *)&src_addr.sin_addr.s_addr);

 	if (bind (s, (struct sockaddr*) &src_addr, sizeof(src_addr)) == -1)
		err_sys("error on bind", errno);

 	if (connect (s, (struct sockaddr*) &dst_addr, sizeof (dst_addr)) < 0)
        err_sys("error", errno);

 	//memset(packet, '%', sizeof(packet));   /* payload will be all As */

 	ip->ihl = 5;
 	ip->version = 4;
 	ip->tos = 0;
 	ip->tot_len = htons(40);
 	ip->frag_off = 0;  // NF 
 	ip->ttl = 64;
 	ip->protocol = 165; // higher layer protocol
 	ip->check = 0;
 	ip->saddr = dst_addr.sin_addr.s_addr;
 	ip->daddr = src_addr.sin_addr.s_addr;

 	while(42)
 	{  
  		write (1, "enter: ", 7);
  		n = read (0, packet, sizeof (packet));
  		packet[n-1] = '\0';
  		printf("packet+20 = %s\n", packet);
   
  		//memset(packet, '%', sizeof(packet));

  		//if (sendto(s, packet, sizeof(packet), 0, (struct sockaddr *)&dst_addr, (socklen_t)sizeof(dst_addr)) < 0)
   	    //    perror("uh oh:");
   		if (send (s, packet, sizeof (packet), 0) < 0)
   			err_sys ("send error", errno);
 	}
 	return(0);
}