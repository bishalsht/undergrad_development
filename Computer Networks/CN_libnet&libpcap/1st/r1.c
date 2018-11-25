#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{

	int s;
	struct sockaddr_in saddr;
	char packet[50];

	s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	perror("socket ");

	int zero=0;
	const int *val=&zero;

	setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof(zero));
	perror("setsockopt() ");
	

	
	memset(packet, '\0', sizeof(packet));   /* payload will be all As */
	int fromlen = sizeof(saddr);

	
	while(1) {
		if (recvfrom(s, (char *)&packet, sizeof(packet), 0,
			(struct sockaddr *)&saddr, &fromlen) < 0)
			perror("packet receive error:");

         
                   struct iphdr *ip=(struct iphdr *)packet;
	printf("-------------------------------------------------------------------------\n");
        printf("|\t %d  |",ip->version);//ip version
	printf("\t %d   |",ip->ihl);//ip header length
	printf("\t   %d    \t|",ip->tos);//type of service
	printf("\t \t %d\t\t|\n",ip->tot_len);// ip packet total length
        printf("-------------------------------------------------------------------------\n");

        printf("|\t\t     %d    \t\t|",ip->id);
        printf(" |");
         printf(" |");
        printf(" |");
	printf(" \t \t%d   \t|\n",ip->frag_off);//fragment offset

       printf("-------------------------------------------------------------------------\n");
	printf("|\t  %d   \t|",ip->ttl);//time to live
	printf("\t   %d   \t|",ip->protocol);//higher layer protocol
	printf("\t\t %d\t\t|\n",ip->check);//NOT CORRECT??? //header checksum

         printf("-------------------------------------------------------------------------\n");
	printf("|\t\t\t\t%d\t\t\t\t|\n",inet_ntoa(ip->saddr));  //source ip address
       
         printf("-------------------------------------------------------------------------\n");
	printf("|\t\t\t\t%d\t\t\t\t|\n",inet_ntoa(ip->daddr)); //destination ip address
	printf("-------------------------------------------------------------------------\n");	
		printf("message got:%s\n",packet+sizeof(struct iphdr));
	}
	exit(EXIT_SUCCESS);
}
