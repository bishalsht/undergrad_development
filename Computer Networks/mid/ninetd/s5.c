#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    printf("eudp\n");
    int n;
    struct sockaddr_in add;
    
//    int sfd;
//    sfd=socket(PF_INET,SOCK_DGRAM,0);
    char buff[1024];
    socklen_t len;
    
    add.sin_family=PF_INET;
    add.sin_port=htons(55);
    inet_aton("127.0.0.1",&(add.sin_addr.s_addr));
    len=sizeof(add);
    
    for (; ; ) {
        n=recvfrom(0,buff,100,0,(struct sockaddr*)&add,&len);
        buff[n]='.';
        sendto(1,buff,n+5,0,(struct sockaddr*)&add,len);
        if (buff[0]=='.') {
            break;
        }
    }
    
    int a=open("done",O_WRONLY);
    write(a,"5",1);
    close(a);

    return 0;
}