#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<strings.h>
#include<arpa/inet.h>
#include<unistd.h>
int main(int argc,char **argv){
    int sad,sad_ser,sad1;
    socklen_t len;
    struct sockaddr_in address,ser_address;
    size_t sent;
    
    char msg[100];
    
    if((sad=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("socket");
        exit(1);
    }
    printf("socket created\n");
    
    address.sin_family=AF_INET;
    address.sin_port=htons(8080);
    address.sin_addr.s_addr=INADDR_ANY;
    bzero(&address.sin_zero,8);
    
    len=sizeof(address);
    if(connect(sad,(struct sockaddr*)&address,len)==-1){
        perror("connect");
        exit(1);
    }
    printf("connect completed\n");
    
  

    for (; ; ) {

     int n=read(0,msg,100);
    msg[n]='\0';
     send(sad,msg,100,0);
        
    }
    
    return 0;
}
