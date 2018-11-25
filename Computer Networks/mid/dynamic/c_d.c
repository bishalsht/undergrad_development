#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<strings.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/select.h>
#include<string.h>

int main(int argc, char *argv[]){
    int length;
    int sock_s,sock_d;
    struct sockaddr_in address;
    
    if((sock_d=socket(AF_INET, SOCK_DGRAM, 0))==-1){
        perror("sock_dgram");
        exit(-1);
    }
    printf("sock_dgram ok\n");
    
    address.sin_family=AF_INET;
    address.sin_port=htons(atoi(argv[1]));
    address.sin_addr.s_addr=INADDR_ANY;
    bzero(&address.sin_zero,8);
    

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0,&rfds);
    FD_SET(sock_d,&rfds);
    
    char msg[100];
    int size=sizeof(address);
    
    
    for (; ; ) {
        select(sock_d+1,&rfds,NULL,NULL,NULL);
        if (FD_ISSET(0,&rfds)) {
            bzero(msg,100);
            int n=read(0,msg,100);
            msg[n-1]='\0';
            n=sendto(sock_d, msg, strlen(msg), 0,(struct sockaddr *)&address, sizeof(address));
            printf("%s sent to server\n",msg);
        }else if (FD_ISSET(sock_d,&rfds)) {
            int n=recvfrom(sock_d, msg, 100, 0,(struct sockaddr *)&address, (socklen_t *) &size);
            msg[n]='\0';
            printf("%s received from server\n",msg);
        }
        FD_SET(0,&rfds);
        FD_SET(sock_d,&rfds);
    }
    
    
    
    close(sock_d);
    
}