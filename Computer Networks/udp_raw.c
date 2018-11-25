//compile with
//gcc `libnet-config --defines` udp_raw.c -o c `libnet-config --libs`
#include <stdio.h>
#include <stdlib.h>
#include <libnet.h>
#include <stdint.h>
#include <libnet.h>

#define MAX_PAYLOAD_SIZE    1024
libnet_t *l;
int main() 
{ 

        int i; int c=1;
	char errbuf[LIBNET_ERRBUF_SIZE];
	char payload[]="Sent using libnet"; //payload bytes
        char ip_addr_str[16], sip_addr_str[16];
	u_int32_t ip_addr, src_addr;
	u_int16_t ip_id,dport;
        char port[5];
	int pload_offset; 
	int bytes_written, max_pload_size, packet_pload_size;
	libnet_ptag_t ip_tag,udp_tag;

          printf("packet type:\tUDP [with payload] using port \n");
	l = libnet_init(LIBNET_RAW4, NULL, errbuf);
	if ( l == NULL ) 
        {
		fprintf(stderr, "libnet_init() failed (raw4, 1st call): %s\n",
			 	errbuf);
		exit(EXIT_FAILURE);
	}else printf("successful\n");

        max_pload_size = ( MAX_PAYLOAD_SIZE  - LIBNET_IPV4_H-LIBNET_UDP_H);
	max_pload_size -= (max_pload_size % 8);  //byte formatting
	pload_offset = LIBNET_IPV4_H+LIBNET_UDP_H;
       
      
//---------------------------------------------------------------------------------------------------
//BUILDING UDP HEADER
        printf("Destination port:(note:client port as 4096 )\n");
        scanf("%s",port);
        dport=htons(atoi(port));
        if(libnet_build_udp(2482,           //source port 
                    dport,                  // dest. port 
                    LIBNET_UDP_H+max_pload_size,                //udp packet length 
                    0,
		    (u_int8_t*)payload,
		    sizeof(payload),           
                    l,
                    0)==-1)
        {
		fprintf(stderr, "Error building IP header: %s\n", \
				libnet_geterror(l));
		libnet_destroy(l);
		exit(EXIT_FAILURE);
	}

 
//---------------------------------------------------------------------------------------------------
   //Building IP header
        ip_id = (u_int16_t)libnet_get_prand(LIBNET_PR16);
          printf("source IP address: \n");
	scanf("%15s",sip_addr_str);


        ip_addr = libnet_name2addr4(l, sip_addr_str, LIBNET_DONT_RESOLVE);
	if ( src_addr == -1 ) {
		fprintf(stderr, "Couldn't get own IP address: %s\n", \
				libnet_geterror(l));
		libnet_destroy(l);
		exit(EXIT_FAILURE);
	}
 
        printf("Destination IP address: \n");
	scanf("%15s",ip_addr_str);


        ip_addr = libnet_name2addr4(l, ip_addr_str, LIBNET_DONT_RESOLVE);
	if ( ip_addr == -1 ) {
		fprintf(stderr, "Error converting IP address.\n");
		libnet_destroy(l);
		exit(EXIT_FAILURE);
	}

        if ( libnet_build_ipv4((LIBNET_IPV4_H +LIBNET_UDP_H+max_pload_size), 0, ip_id,
				0, 255, IPPROTO_UDP, 0, src_addr,
				ip_addr, NULL,0, l, 0) == -1 )
	{
		fprintf(stderr, "Error building IP header: %s\n", \
				libnet_geterror(l));
		libnet_destroy(l);
		exit(EXIT_FAILURE);
	}

//----------------------------------------------------------------------------------------
      
         bytes_written = libnet_write(l);

	if ( bytes_written != -1 )
		printf("%d bytes written.\n", bytes_written);
	else
		fprintf(stderr, "Error writing packet: %s\n", \
					libnet_geterror(l));

	libnet_clear_packet(l);
        libnet_destroy(l);
        return 0;
}



