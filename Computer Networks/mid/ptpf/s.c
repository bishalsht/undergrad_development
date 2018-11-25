#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

void *serve(void *info);

int sfd;
socklen_t len;
struct sockaddr_in addr;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

struct thread_info{
    short process_no;
    short thread_no;
};

int main(){
    
    int child_pid[3];
    
    pthread_t thread[4];
    
    //socket
    if((sfd=socket(PF_INET,SOCK_STREAM,0))==-1){
        perror("socket");
        exit(1);
    }
    printf("socket\n");
    
    addr.sin_family=PF_INET;
    addr.sin_port=htons(8080);
    addr.sin_addr.s_addr=INADDR_ANY;
    len=sizeof(addr);
    
    //bind
    if ((bind(sfd,(struct sockaddr *)&addr,len))==-1) {
        perror("bind");
        exit(1);
    }
    printf("bind\n");
    
    //listen
    if ((listen(sfd,10))==-1) {
        perror("listen");
        exit(1);
    }
    printf("listen\n");
    
    //forking 3 times
    int i;
    for (i=0; i<3; i++) {
        int pid=fork();
        if (pid==0) {//child
            printf("child [%d] %d\n",i,getpid());
            
            //creating threads
            int j;
            for (j=0; j<4; j++) {
                struct thread_info *info;
                info->process_no=i;
                info->thread_no=j;
                pthread_create(&thread[i],NULL,serve,(void*)info);
            }
            for (j=0; j<4; j++) {
                pthread_join(thread[i],NULL);
            }
            break;
        }else{//parent
            printf("parent %d %d %d\n",i,getpid(),pid);
            child_pid[i]=pid;
        }
    }
    
    //wait
    for (i=0; i<3; i++) {
        waitpid(child_pid[i],NULL,0);
    }
    
}
void *serve(void *info){
    int pno=((struct thread_info *) info)->process_no;
    int tno=((struct thread_info *) info)->thread_no;
    char msg[100];
    int nsfd;
    printf("thread\n");
    for (;;) {
        pthread_mutex_lock(&mutex);
        printf("got mutex\n");
        if ((nsfd=accept(sfd,(struct sockaddr*)&addr,&len))==-1) {
            perror("accept");
            exit(1);
        }
        printf("accept\n");
        pthread_mutex_unlock(&mutex);
        
        for (; ; ) {
            int n=recv(nsfd,msg,100,0);
            if (n>0) {
                msg[n]='\0';
                printf("p[%d] t[%d] : %s\n",pno,tno,msg);
                if (msg[0]=='.') {
                    break;
                }
            }
        }
    }
    
    return NULL;
}