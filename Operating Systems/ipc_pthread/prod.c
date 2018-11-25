#include<stdio.h>
#include<pthread.h>
#define buffersize 10
void *producer();
void *consumer();
int bufferindex=0;
char *buffer;


pthread_cond_t buffer_not_full=PTHREAD_COND_INITIALIZER;
pthread_cond_t	buffer_not_empty=PTHREAD_COND_INITIALIZER;

pthread_mutex_t mvar=PTHREAD_COND_INITIALIZER;

int main()
{
	pthread_t ptid,ctid;

	
	buffer=(char*)malloc (sizeof(char) *buffersize);
	pthread_create(&ptid,NULL,producer,NULL);
	pthread_create(&ctid,NULL,consumer,NULL);

	pthread_join(ptid,NULL);
	pthread_join(ctid,NULL);

	return 0;

}

void *producer()
{
	for(;;)
	{
		pthread_mutex_lock(&mvar);
		if(bufferindex==buffersize)
			pthread_cond_wait(&buffer_not_full,&mvar);
		bufferindex++;
		printf("produce: %d \n ",bufferindex);
		pthread_mutex_unlock(&mvar);
		pthread_cond_signal(&buffer_not_empty);
	}
}

void* consumer()
{
	for(;;)
	{
		pthread_mutex_lock(&mvar);
		if(bufferindex==1)
			pthread_cond_wait(&buffer_not_empty,&mvar);
		printf("consumer %d \n",bufferindex--);
		pthread_mutex_unlock(&mvar);
		pthread_cond_signal(&buffer_not_full);
	}
}
