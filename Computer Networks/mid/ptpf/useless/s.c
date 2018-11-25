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


#define MAXHOSTNAME 256
#define LISTEN_BACKLOG 50

#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)

int child=5,s1;
int nsfd,n;
int count=0,max_len=50;
char buff[50];
char PORT[5]="8080";

void sem_acq(int,int);
void sem_rel(int,int);

struct std{
    
    int clientfd;
    int childno;
    
    struct sockaddr_in addr;
    
};

void *func(void *arg){
    
    struct std x=*((struct std *)arg);
    
    int y=sizeof(x.addr);
    sem_acq(s1,0);
    
    
    if((nsfd=accept(x.clientfd,(struct sockaddr*)& x.addr,(socklen_t*)&y))==-1){
        perror("listen");
        exit(-1);
        
				}
    
    sem_rel(s1,0);
    
    while(1){
        char buf[5];
        
        int n=recv(nsfd,buff,50,0);
        if (n==0) break;
        buff[n-1]='\0';
        strcat(buff,"    ----->handled by:");
        
        sprintf(buf,"%d",x.childno);
        strcat(buff,buf);
        printf("%s\n",buff);
        
    }
    
    close (x.clientfd);
    
    return NULL;
}



int main()
{
    int length,n;
    int sfd,sfd1,client,fd[5][2];
    pthread_t clients[4];
    struct sockaddr_in address,address_client[5],client_addr;
    
    
    
    if((s1=semget(IPC_PRIVATE,1,0666|IPC_CREAT))==-1)
    {
        perror("\nFailed to create semaphore.");
        exit(0);
    }
    ushort array[]={1,1};
    
    if((semctl(s1,0,SETALL,array))==-1)
    {
        perror("\nFailed to set value for the semaphore.");
        exit(0);
    }
    
    // create socket
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("sock_dgram");
        exit(-1);
    }
    
    // Load system information into socket data structures
    address.sin_family=AF_INET;
    address.sin_port=htons(8080);
    address.sin_addr.s_addr=INADDR_ANY;
    bzero(&address.sin_zero,8);
    
    length=sizeof(address);
    if (bind(sfd,(struct sockaddr*)&address,length)==-1) {
        perror("bind");
        exit(1);
    }
    
    printf("bind complete\n");
    
    if(listen(sfd,5)==-1){
        perror("listen");
        exit(1);
    }
    printf("now listening\n");
    
    
    
    int size=sizeof(address);
    int i,j;
    
    
    
    
    
    
aim:
    pipe(fd[count]);
    int c=fork(),nsfd;
    
    if(c>0){//parent
        for( i=0;i<count;i++)close(fd[i][0]);
        
        if(child!=0){
            child--;
            printf("%d\n",child);
            goto aim;
        }
        //recv(nsfd,buff,2,0);
        ///PORT[3]=buff[0]-'0';
        //send(nsfd,PORT,strlen(PORT),0);
        //write(fd[buff[0]-'0'][1],client_addr,size);
        while(1)pause();
        
        
        
    }
    
    else if(c==0){//child
        close(fd[count][1]);
        
        int no=count;
        count++;
        
        struct std x;
        x.clientfd=sfd;
        x.addr=client_addr;
        x.childno=no;
        
        
        for(i=0;i<4;i++) {
            printf("created thread %d %d\n",count,i);
            pthread_create(&clients[i],NULL,func,(void *)&x);
        }
        
        
        pthread_join(clients[0],NULL);
    }
    
    //read(fd[no][1],client_addr,size);
    
    
    
    
    
    
    return 0;
    
}
void sem_acq(int semid,int semindex)
{ 
    
    struct sembuf sb;
    sb.sem_num=semindex;
    sb.sem_op=-1;
    sb.sem_flg=0;
    if((semop(semid,&sb,1))==-1)    
    {
        perror("\nFailed to acquire semaphore.");
        exit(0);
    }
}

void sem_rel(int semid,int semindex)
{
    struct sembuf sb;
    sb.sem_num=semindex;
    sb.sem_op=1;
    sb.sem_flg=0;
    if((semop(semid,&sb,1))==-1)    
    {
        perror("\nFailed to release semaphore.");
        exit(0);
    }
}
