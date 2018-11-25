#include<stdio.h>
#i/nclude<stdlib.h>
#include<pthread.h>
#include<sys/types.h>

#define max 10
int waiting_customers =0;
pthread_mutex_t counter,cutting;
void customer(void *arg)
{
	int id=*((int *)arg);
	while(1)
	{
		sleep(rand()%4+5);
		pthread_mutex_lock(&counter);
		if(waiting_customers==0)
			printf("customer %d is waking up barber \n ",id);
		if(waiting_customers==max)
		{
			printf("No space at waiting room for customer %d \n",id);
			pthread_mutex_unlock(&counter);
		}

		else
		{
			waiting_customers++;
			if(waiting_customers>1)
				printf("customer %d is waiting \n ",id);
			pthread_mutex_unlock(&counter);
			pthread_mutex_lock(&cutting);
			printf("Barber is cutting hair of customer %d \n",id);
			sleep(1);
			printf("cutting hair of customer %d completed \n",id);
			pthread_mutex_unlock(&cutting);
			pthread_mutex_lock(&counter);
			waiting_customers--;
			if(waiting_customers==0)
				printf("barber is sleeping \n");
			pthread_mutex_unlock(&counter);
		}
	}
}

int main()
{
	int ids[10];
	pthread_t cust[10];
	int i;
	for(i=0;i<10;i++)
	{
		ids[i]=i+1;
		pthread_create(&cust[i],NULL,(void*)&customer,(void*)&ids[i]);
	}
	pthread_join(cust[0],NULL);
	return 0;

}

