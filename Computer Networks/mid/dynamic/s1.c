#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
int main(){
    int sent;
    char msg[100];
    for (; ; ) {
        bzero(msg,100);
        sent=recv(0,msg,100,0);
        msg[sent]='\0';
//        if(sent>0)
            printf("%s\n",msg);
        sent=send(0,msg,strlen(msg),0);
    }
    return 0;
}
