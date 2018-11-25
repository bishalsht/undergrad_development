#include "include.h"

int coupons[MAXT][NOOFCOUPON] ;
sockaddr_in servaddr;
socklen_t socksize = sizeof(servaddr); 

void *service(void *arg){
	int couponid = *(int*)arg;
	int k;
	int match =0;
	int rsfd = Socket(FAMILY,RAW,IPPROTO_RAW);
	char packet[50];
	printf("Thread id : %lu strated in process id : %d\n",pthread_self(),getpid());
	while(1) {
	   k = Recvfrom(rsfd,&packet, sizeof(packet), 0,(SA *)&servaddr, (socklen_t*)&socksize);
	   packet[k]='\0';
	   printf("recvd in process id %d in thread %lu : %s\n",getpid(),pthread_self(),&packet[HLEN*4]);
	   k = atoi(&packet[HLEN*4]);
	   for(int i=0;i<NOOFCOUPON;i++){
	   	if(coupons[couponid][i]==k){
	   		match++;
	   		break;
	   	}
	   }
	   if(match==NOOFCOUPON){
	   	printf("%s\n","All matched");
	   	Bzero(&servaddr,socksize);
		int sfd = Socket(FAMILY,TCP,0);
		servaddr.sin_port = htons(PORTNUMBER+4);
		servaddr.sin_family=FAMILY;
		servaddr.sin_addr.s_addr = DADDRESS;
		Connect(sfd,(SA*)&servaddr,socksize);
		int p;
		int k = Recv(sfd,(void*)&p,sizeof(int),0);
		perror("recv");
	   	printf("ALL COUPON MATCHED in thread id: %lu and in process id: %d got prize %d\n",pthread_self(),getpid(),p);
	   	Close(sfd);
	   	exit(1);
	   }
	 }
}
int main(int argc, char const *argv[]){
	Bzero(&servaddr,socksize);
	int sfd = Socket(FAMILY,TCP,0);
	servaddr.sin_port = htons(PORTNUMBER);
	servaddr.sin_family=FAMILY;
	servaddr.sin_addr.s_addr = DADDRESS;
	Connect(sfd,(SA*)&servaddr,socksize);
	int k = Recv(sfd,(void*)&coupons,(sizeof(int))*MAXT*NOOFCOUPON,0);
	cout<<"Recvd coupons\n";
	for(int i=0;i<MAXT;i++){
		for(int j=0;j<NOOFCOUPON;j++){
			printf("%d\t",coupons[i][j]);
		}
		printf("\n");
	}
	Close(sfd);
	pthread_t t[MAXT];
	for(int i=0;i<MAXT;i++){
		int j=i;
		pthread_create(&t[i],NULL,service,(void*)&j);
	}
	while(1);
	return 0;
}