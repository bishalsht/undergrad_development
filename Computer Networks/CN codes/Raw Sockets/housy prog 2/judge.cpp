#include "include.h"

void *prize(void *arg){
	sockaddr_in servaddr;
	socklen_t socksize = sizeof(servaddr);

	Bzero(&servaddr,socksize);
	int sfd = Socket(FAMILY,TCP,0);
	servaddr.sin_port=htons(PORTNUMBER+4);
    servaddr.sin_family=FAMILY;
	servaddr.sin_addr.s_addr = DADDRESS;
	Bind(sfd,(SA*)&servaddr,socksize);
	Listen(sfd,MAXQSIZE);
	cout<<"started prize\n";
	while(1){
		int k =rand();
		int nsfd = Accept(sfd,(SA*)&servaddr,(socklen_t*)&socksize);
		Send(nsfd,(void*)&k,sizeof(int),0);
		Close(sfd);
		exit(1);
	}
}
int main(int argc, char **argv){
 
 int rsfd = Socket(FAMILY,RAW,IPPROTO_RAW);
 char packet[50];
 struct iphdr *ip = (struct iphdr *)packet;  
 struct sockaddr_in dst_addr;
 socklen_t socksize = sizeof(dst_addr);
 Bzero(&dst_addr,socksize);

 dst_addr.sin_family = FAMILY;
 dst_addr.sin_addr.s_addr = DADDRESS;

 memset(packet, 'A', sizeof(packet));   /* payload will be all As */

 ip->ihl = HLEN; //5*4
 ip->version = 4; 
 ip->tos = 0;     
 ip->frag_off = 0;  
 ip->ttl = 64;     
 ip->protocol = IPPROTO_RAW; // this has to be IPPROTO_RAW 
 ip->check = 0;             
 ip->saddr = SADRRESS;
 ip->daddr = DADDRESS;
 
 char msg[32];
 std::srand(std::time(0));
 pthread_t t;
 pthread_create(&t,NULL,prize,NULL);
 while(1) {
     int t = rand()%100+1;
     sprintf(msg,"%d",t);
 	 strncpy(&packet[20],msg,32);
 	 ip->tot_len = 20+strlen(msg);
	 Sendto(rsfd, packet,ip->tot_len, 0,(SA *)&dst_addr,socksize);
	 cout<<t<<"\n";
	 sleep(1);
 }
 return(0);
}