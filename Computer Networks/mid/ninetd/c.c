#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int is_stream(int i);

int main(int argc, char *argv[]){
    int n;
    char buff[100]="hello";
    
    printf("%c\n",argv[1][0]);
    int i=argv[1][0]-49;
    
	struct sockaddr_in add;
	socklen_t len;
	int sfd;
    if (is_stream(i)) {
        sfd=socket(PF_INET,SOCK_STREAM,0);
    }else{
        sfd=socket(PF_INET,SOCK_DGRAM,0);
    }
	
    add.sin_family=PF_INET;
    add.sin_port=htons(11*(i+1));
    inet_aton("127.0.0.1",&(add.sin_addr.s_addr));
    len=sizeof(add);
    
    if (is_stream(i)) {
        if(connect(sfd,(struct sockaddr *) &add, len)==-1){
            perror("connect");
            exit(1);
        }
//        char buff[100];
        send(sfd,buff,strlen(buff),0);
        int n=recv( sfd,buff,100,0);
        buff[n]='\0';
        printf("%s\n",buff);
    }else{
        sendto(sfd,buff,strlen(buff),0,(struct sockaddr*)&add,len);
        printf("sent\n");
        n=recvfrom(sfd,buff,100,0,(struct sockaddr*)&add,&len);
        buff[n]='\0';
        printf("%s\n",buff);
    }
    
	fd_set rfds;
	FD_ZERO(&rfds);
    
    FD_SET(0,&rfds);
    FD_SET(sfd,&rfds);
    
    for(;;){
		select(sfd+1,&rfds,NULL,NULL,NULL);

        if(FD_ISSET(sfd,&rfds)){
            if (is_stream(i)) {
                int n=recv( sfd,buff,100,0);
                buff[n]='\0';
                if(n>0)
                    printf("%s\n",buff);
            }else{
                n=recvfrom(sfd,buff,100,0,(struct sockaddr*)&add,&len);
                buff[n]='\0';
                if(n>0)
                    printf("%s\n",buff);
            }
        }else if(FD_ISSET(0,&rfds)){
            n=read(0,buff,100);
            buff[n]='\0';
            if (is_stream(i)) {
                send(sfd,buff,strlen(buff),0);
                printf("sent\n");
            }else{
                sendto(sfd,buff,strlen(buff),0,(struct sockaddr*)&add,len);
                printf("senty\n");
            }
            
		}
        FD_SET(0,&rfds);
        FD_SET(sfd,&rfds);
    }
	return 0;
}
int is_stream(int i){
	if(i==0||i==1||i==3) return 1;
	return 0;
}
