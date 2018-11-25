#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#define BUFFERSIZE 5
int count=0;
pthread_mutex_t varlock;


void *produce(void *args)
{
while(1)
{
sleep(rand()%2);
if(!pthread_mutex_trylock(&varlock))
{
if(count<BUFFERSIZE)
{
count++;
printf("producer working count =%d \n",count);
}
else printf("BUFFER FULL\n");
pthread_mutex_unlock(&varlock);
}
}
}

void *consume(void *args)
{
while(1)
{
sleep(rand()%3);
if(!pthread_mutex_trylock(&varlock))
{
if(count>0)
{
count--;
printf("consumer working  count = %d \n",count);
}
else printf("BUFFER EMPTY\n");
pthread_mutex_unlock(&varlock);
}

}
}


int main()
{
pthread_mutex_init(&varlock,NULL);
pthread_t prod,cons;
pthread_create(&prod,NULL,produce,NULL);
pthread_create(&prod,NULL,consume,NULL);
pthread_join(prod,NULL);
pthread_join(cons,NULL);
return 0;
}

