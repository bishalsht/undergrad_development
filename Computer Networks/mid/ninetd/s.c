#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int is_stream(int i);
void *s2(void *num);

int nsfd_s2[15];

pthread_mutex_t serv_count_mutex=PTHREAD_MUTEX_INITIALIZER;
int serv_count[5]={0};

int main(){
    
    mkfifo("done",IPC_CREAT|0666);
    int p=fork();
    if (p==0) {
        int a=open("done",O_WRONLY);
//        write(a,"1",1);
        return 0;
    }
    int re=open("done",O_RDONLY);
    
    
    
    int serv_max[5]={25,15,-1,-1,1};
    int thread_count=0;
    pthread_t thread[15];
    
	struct sockaddr_in add[5];
	socklen_t len[5];

    int n;
    char buff[100];
    
	int sfd[5];

	int i;
	for(i=0;i<5;i++){
		if(is_stream(i)){
            if((sfd[i]=socket(PF_INET,SOCK_STREAM,0))==-1){
                perror("socket");
            }
		}else{
            if((sfd[i]=socket(PF_INET,SOCK_DGRAM,0))==-1){
                perror("socket");
            }
		}
        printf("socket\n");
		add[i].sin_family=PF_INET;
		add[i].sin_port=htons(11*(i+1));
        printf("port %d\n",11*(i+1));
		inet_aton("10.211.55.2",&(add[i].sin_addr.s_addr));
		len[i]=sizeof(add[i]);
		if(bind(sfd[i],(struct sockaddr *) &add[i], len[i])==-1){
			char ec[]="bind[.]";
			ec[5]=i+48;
			perror(ec);
			exit(1);
		}
        printf("bind\n");
		
		if(is_stream(i) && listen(sfd[i],5)==-1){
			perror("listen");
			exit(1);
		}
        printf("listen\n");
	}
    printf("all done\n");

	fd_set rfds;
	FD_ZERO(&rfds);
	
    
    FD_SET(re,&rfds);
	for(i=0;i<5;i++){
		FD_SET(sfd[i],&rfds);
	}
	for(;;){
		select(sfd[4]+1,&rfds,NULL,NULL,NULL);
        if (FD_ISSET(re,&rfds)) {
            printf("read\n");
            n=read(re,buff,100);
            if (n>0) {
                buff[n]='\0';
                int num=atoi(buff);
                printf("%s %d\n",buff,num);
                serv_count[num-1]--;
            }
            
        }
		for(i=0;i<5;i++){
			if(FD_ISSET(sfd[i],&rfds)){
                printf("this is fd[%d] ;) \n",i);
                
                printf ("serv %d %d\n",serv_max[i],serv_count[i]);
                if (serv_max[i]!=-1) {
                    serv_count[i]++;
                }
                if (i==1) {
                    nsfd_s2[thread_count]=accept(sfd[i],(struct sockaddr*)&add[i],&len[i]);
                    int * num=(int *)malloc(sizeof(int*));
                    *num=thread_count;
                    printf("%d\n",*num);
                    pthread_create(&thread[thread_count],NULL,s2,(void*)num);
                    thread_count=(thread_count+1)%15;
                }
                else if(i==2){
                    n=recvfrom(sfd[i],buff,100,0,(struct sockaddr*)&add[i],&len[i]);
                    buff[n]='\0';
                    sendto(sfd[i],buff,strlen(buff),0,(struct sockaddr*)&add[i],len[i]);

                }
                else if (is_stream(i)) {
                    int nsfd=accept(sfd[i],(struct sockaddr*)&add[i],&len[i]);
                    int pid= vfork();
                    if (pid==0) {//child
                        dup2(nsfd,0);
                        dup2(nsfd,1);
                        char pgm[]="./sn";
                        pgm[3]=i+49;
                        execl(pgm,pgm,NULL);
                        return 0;
                    }else{
                        close(nsfd);
                    }
                }
                else{
                    printf("eudp\n");
                    int pid=vfork();
                    if (pid==0) {//child
                        dup2(sfd[4],0);
                        dup2(sfd[4],1);
                        execl("./s5","./s5","55",NULL);
                    }else{
                        usleep(10000);
                    }
                }
			}
		}
        
        FD_ZERO(&rfds);
        FD_SET(re,&rfds);
        for(i=0;i<5;i++){
            if (serv_count[i]!=serv_max[i]) {
                    FD_SET(sfd[i],&rfds);
            }
		}
	}
	return 0;
}
int is_stream(int i){
	if(i==0||i==1||i==3) return 1;
	return 0;
}
void *s2(void *num){
    int n=*(int*)num;
    printf("%d\n",n);
    char buff[100];
    printf("here\n");
    for (; ; ) {
        int l=recv(nsfd_s2[n],buff,100,0);
        buff[0]=l+48;
        buff[1]='\0';
        send(nsfd_s2[n],buff,sizeof(buff),0);
        if (buff[0]=='.') {
            break;
        }
    }
    pthread_mutex_lock(&serv_count_mutex);
    serv_count[1]--;
    pthread_mutex_unlock(&serv_count_mutex);
    
    return NULL;
}
