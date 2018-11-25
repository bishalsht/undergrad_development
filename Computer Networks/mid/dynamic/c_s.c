#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<strings.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
    int sad,sad_ser;
    socklen_t len;
    struct sockaddr_in address,ser_address;
    size_t sent;
    
    char msg[100];
    
    if((sad=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("socket");
        exit(1);
    }
    printf("socket created\n");
    
    address.sin_family=PF_INET;
    address.sin_port=htons(atoi(argv[1]));
    address.sin_addr.s_addr=INADDR_ANY;
    bzero(&address.sin_zero,8);
    
    len=sizeof(address);
    if(connect(sad,(struct sockaddr*)&address,len)==-1){
        perror("connect");
        exit(1);
    }
    printf("connect completed\n");
    
    ssize_t siz;
    for (; ; ) {
//        bzero(msg,100);
        scanf("%s",msg);
        siz=send(sad,msg,strlen(msg),0);
        bzero(msg,100);
        siz=recv(sad,msg,100,0);
        printf("%s\n",msg);

    }
    
    return 0;
}