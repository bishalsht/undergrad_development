#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void show_usage();
int main(int argc, char *argv[]){
    if (argc<4) {
        show_usage();
        exit(1);
    }
    
    int fifo;
    if (argv[1][1]=='a') {
        fifo=open("add",O_WRONLY);
    }else if(argv[1][1]=='d'){
        fifo=open("del",O_WRONLY);
    }else{
        show_usage();
        exit(1);
    }
    
    if (strcmp(argv[2],"stream")&&strcmp(argv[2],"dgram")) {
        show_usage();
        exit(1);
    }
    
    char message[6];
    message[0]=argv[2][0];
    message[1]='\0';
    strcat(message,argv[3]);
    
    printf("%s sent to server\n",message);
    write(fifo,message,strlen(message));
    
    return 0;
}
void show_usage(){
    printf("usage:\nconf -[a|d] [stream|dgram] <port number>\n");
}