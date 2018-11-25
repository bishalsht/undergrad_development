#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <string.h>
#include <fcntl.h>
#include <sys/uio.h>

#define SOCK_MAX 5

int main(){
    int sock_s_count=1,sock_d_count=1;
    
    int i;
    
    //make fifo
    mkfifo("add",IPC_CREAT|0666);
    mkfifo("del",IPC_CREAT|0666);
    
    int pid=fork();
    if (pid==0) {
        int addw=open("add",O_WRONLY);
        int delw=open("del",O_WRONLY);
        return 0;
    }
    int add=open("add",O_RDONLY);
    int del=open("del",O_RDONLY);
    
    //variables for the server
    int length;
    int sock_s[SOCK_MAX]={0},sock_d[SOCK_MAX]={0};
    int sock_s_port[SOCK_MAX]={80,0,0,0,0},sock_d_port[SOCK_MAX]={80,0,0,0,0};
    struct sockaddr_in address;
    
    //connection oriented
    if((sock_s[0]=socket(AF_INET, SOCK_STREAM, 0))==-1){
        perror("sock_stream");
        exit(-1);
    }
    printf("sock_stream ok\n");

    //connection less
    if((sock_d[0]=socket(AF_INET, SOCK_DGRAM, 0))==-1){
        perror("sock_dgram");
        exit(-1);
    }
    printf("sock_dgram ok\n");
    
    //address
    address.sin_family=AF_INET;
    address.sin_port=htons(80);
    address.sin_addr.s_addr=INADDR_ANY;
    bzero(&address.sin_zero,8);
    
    //bind
    length=sizeof(address);
    if (bind(sock_s[0],(struct sockaddr*)&address,length)==-1) {
        perror("bind");
        exit(1);
    }
    printf("bind_s completed\n");

    length=sizeof(address);
    if (bind(sock_d[0],(struct sockaddr*)&address,length)==-1) {
        perror("bind");
        exit(1);
    }
    printf("bind_d completed\n");
    
    //listen
    if(listen(sock_s[0],5)==-1){
        perror("listen");
        exit(1);
    }
    printf("now listening\n");

    
    char msg[100];
    int size=sizeof(address);

    //select
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(add,&rfds);
    FD_SET(del,&rfds);
    for (i=0; i<sock_s_count; i++) {
        FD_SET(sock_s[i],&rfds);
    }
    for (i=0; i<sock_s_count; i++) {
        FD_SET(sock_d[i],&rfds);
    }
    
    for (; ; ) {
        select(32,&rfds,NULL,NULL,NULL);//change this
        
        for (i=0; i<sock_s_count; i++) {
            printf("%d:%d ",sock_s[i],sock_s_port[i]);
        }
        printf("\n");
        for (i=0; i<sock_d_count; i++) {
            printf("%d:%d ",sock_d[i],sock_d_port[i]);
        }
        printf("\n");
        
        
        
        if (FD_ISSET(add,&rfds)) {//add
            int n=read(add,msg,100);
            if(n>0){
                msg[n]='\0';
                printf("add %s\n",msg);
                printf("s:%d d:%d\n",sock_s_count,sock_d_count);
                
                printf("port: %s\n",msg+1);
                int port=atoi(msg+1);
                address.sin_port=htons(port);
                
                switch (msg[0]) {
                    case 's':
                        if ((sock_s[sock_s_count]=socket(PF_INET,SOCK_STREAM,0))==-1) {
                            perror("socket_s");
                            exit(1);
                        }
                        printf("socket_s\n");
                        
                        sock_s_port[sock_s_count]=port;
                        if ((bind(sock_s[sock_s_count],(struct sockaddr*)&address,length))==-1) {
                            perror("bind_s");
                            exit(1);
                        }
                        printf("bound_s\n");
                        
                        if (listen(sock_s[sock_s_count],5)==-1) {
                            perror("listen_s");
                        }
                        printf("listen_s\n");
                        
                        //increase count
                        sock_s_count++;
                        break;
                    case 'd':
                        if ((sock_d[sock_d_count]=socket(PF_INET,SOCK_DGRAM,0))==-1) {
                            perror("socket_d");
                            exit(1);
                        }
                        printf("socket_d\n");
                        
                        sock_d_port[sock_d_count]=port;
                        if ((bind(sock_d[sock_d_count],(struct sockaddr*)&address,length))==-1) {
                            perror("bind_d");
                            exit(1);
                        }
                        printf("bound_d\n");
                        //increase count
                        sock_d_count++;
                        break;
                }
            }
        }else if (FD_ISSET(del,&rfds)) {//del
            int n=read(del,msg,100);
            if(n>0){
                msg[n]='\0';
                printf("del %s\n",msg);
                printf("s:%d d:%d\n",sock_s_count,sock_d_count);
                int index=-1;
                int port=atoi(msg+1);
                switch (msg[0]) {
                    case 's':
                        for (i=0; i<sock_s_count; i++) {
                            if (port==sock_s_port[i]) {
                                index=i;
                            }
                        }
                        if (index!=-1) {
                            sock_s[index]=sock_s[sock_s_count-1];
                            sock_s_port[index]=sock_s_port[sock_s_count-1];
                        }
                        sock_s_count--;
                        break;
                    case 'd':
                        for (i=0; i<sock_d_count; i++) {
                            if (port==sock_d_port[i]) {
                                index=i;
                            }
                        }
                        if (index!=-1&&index!=sock_d_count) {
                            sock_d[index]=sock_d[sock_d_count-1];
                            sock_d_port[index]=sock_d_port[sock_d_count-1];
                        }
                        sock_d_count--;
                        break;
                }
            }
        }else{//checking the ports
            for (i=0; i<sock_s_count; i++) {
                if (FD_ISSET(sock_s[i],&rfds)) {
                    int client;
                    if((client=accept(sock_s[i],(struct sockaddr*)&address,(socklen_t*)&size))==-1){
                        perror("listen");
                        exit(-1);
                    }
                    printf("client connected\n");
                    
                    int pid=fork();
                    if (pid>0) {    //parent
                        close(client);
                        //        waitpid(pid,NULL,0);
                    }else{          //child
                        close(sock_s[i]);
                        close(sock_d[i]);
                        dup2(client,0);
                        execl("./s1","./s1",NULL);
                    }
                    
                }
            }
            for (i=0; i<sock_d_count; i++) {
                if (FD_ISSET(sock_d[i],&rfds)) {
                    int n=recvfrom(sock_d[i], msg, 100, 0,(struct sockaddr *)&address, (socklen_t *) &size);
                    msg[n]='\0';
                    printf("%s received from client\n",msg);
                    n=sendto(sock_d[i], msg, strlen(msg), 0,(struct sockaddr *)&address, sizeof(address));
                    printf("%s sent from client\n",msg);
                }
            }
        }
        FD_SET(add,&rfds);
        FD_SET(del,&rfds);
        for (i=0; i<sock_s_count; i++) {
            FD_SET(sock_s[i],&rfds);
        }
        for (i=0; i<sock_s_count; i++) {
            FD_SET(sock_d[i],&rfds);
        }
    }
    
    close(sock_s[0]);
    close(sock_d[0]);
    
}