#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define philospher 5
pthread_mutex_t spoon[philospher];
pthread_t philo[philospher];
void philo_init(int id,int *right,int *left)
{
	*right=(id +1)%philospher;
	*left=id;
	printf("philospher %d sat down ",id+1);
}
int pick_up_fork(int id,int right,int left)
{
	int sum;
	if(id%2)
	{
		sum=(pthread_mutex_trylock(&spoon[right])? 1:0);
		sum+=(pthread_mutex_trylock(&spoon[left])? 10:0);
	}
	else{
		sum=(pthread_mutex_trylock(&spoon[left])?10:0);
		sum+=(pthread_mutex_trylock(&spoon[right])?1:0);
		return sum;

	}
}
void wait_for_others(int id,int right,int left,int temp)
	{
		switch(temp)
		{
			case 10:
			printf("philospher %d acquired right spoon and waiting for left spoon \n",id+1);
			pthread_mutex_lock(&spoon[left]);
			break;
			case 1:
			printf("philospher %d acquired left spoon and waiting for right spoon \n",id+1);
			if(id%2)
			{
				pthread_mutex_lock(&spoon[right]);
				pthread_mutex_lock(&spoon[left]);

			}
			else
			{
				pthread_mutex_lock(&spoon[left]);
				pthread_mutex_lock(&spoon[right]);
			}
			break;



		}
	}

void eat(int id)
{
	printf("philospher %d is eating\n",id+1);
	sleep(rand()%5);
}

void release_fork(int id,int right,int left)
{
	if(id%2)
	{
		pthread_mutex_unlock(&spoon[left]);
		pthread_mutex_unlock(&spoon[right]);

	}
	else
	{
		pthread_mutex_unlock(&spoon[right]);
		pthread_mutex_unlock(&spoon[left]);

	}

	printf("philospher %d has released forks \n",id+1);
}

void philos(void *args)
{
	int id ;
	id=*(int*)args;
	int right,left,temp;
	philo_init(id,&right,&left);
	while(1)
	{
		printf("philospher %d is thinking \n",id+1);
		sleep(rand()%5);
		temp=pick_up_fork(id,right,left);
		if(temp!=0)
			wait_for_others(id,right,left,temp);
			eat(id);
			release_fork(id,right,left);
	}
}	
int main()
{
	int i;
	int *g;
	g=(int *)malloc(sizeof(int) * philospher);
	for(i=0;i<philospher;i++)
	{
		pthread_mutex_init(&spoon[i],NULL);

	}
	for(i=0;i<philospher;i++)
	{
		g[i]=i;
		pthread_create(&philo[i],NULL,(void*)&philos,(void *)&g[i]);

	}
	for(i=0;i<philospher;i++)
		pthread_join(philo[i],NULL);
	return 0;
}
