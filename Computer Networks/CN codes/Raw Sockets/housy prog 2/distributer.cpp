#include "include.h"
int sfd;
sockaddr_in servaddr;
socklen_t socksize = sizeof(servaddr); 

int main(int argc, char const *argv[]){
	Bzero(&servaddr,socksize);
	sfd = Socket(FAMILY,TCP,0);
	servaddr.sin_port = htons(PORTNUMBER);
	servaddr.sin_family=FAMILY;
	servaddr.sin_addr.s_addr = DADDRESS;
	Bind(sfd,(SA*)&servaddr,socksize);
	Listen(sfd,MAXQSIZE);
	int coupons[MAXT][NOOFCOUPON];

	int cnt=0;
	std::srand(std::time(0));
	while(cnt<MAXC){
		int nsfd = Accept(sfd,(SA*)&servaddr,(socklen_t*)&socksize);
		for(int i=0;i<MAXT;i++){
			for(int j=0;j<NOOFCOUPON;j++){
				coupons[i][j] = rand()%100+1;
				//cout<<coupons[i][j]<<"\t";
			}
		}
		cnt++;
		Send(nsfd,(void*)&coupons,(sizeof(int)*NOOFCOUPON*MAXT),0);
		close(nsfd);
		cout<<"senT";
		fflush(stdout);
    }
    shutdown(sfd,SHUT_RDWR);
    close(sfd);
	execlp("./j","judge",NULL);
	err_quit("exec");
	return 0;
}