#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NN 0x00800000

//int args[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
int args[64] = {0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15,
                16, 17, 18, 19 ,20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
                32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
                48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63};

int N;
int P;
int A[NN];
int sum;
pthread_mutex_t mut;
pthread_barrier_t barr;

void * thread_func(void *arg)
{
	int thrno;
	int i;
	int chunksize;
	int partialsum = 0;

	pthread_barrier_wait(&barr);
	chunksize = N / P;
	thrno = *(int *)arg;
	printf("Thread starting : %d\n",thrno);
	for (i=thrno * chunksize; i<((thrno+1) * chunksize); i++)
		partialsum += A[i];

	//printf("Thread id : %d\n",thrno);
	pthread_mutex_lock(&mut);
	//printf("Thread id lock : %d\n",thrno);
	sum += partialsum;
	//printf("Thread id add : %d\n",thrno);
	pthread_mutex_unlock(&mut);
	//printf("Thread id terminating : %d\n",thrno);

	return NULL;
}

long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

int main(int argc, char **argv)
{
	int i, mcmodel;
	void *retval;
	long t1, t2, t3, t4;
	pthread_t thr[64];
	struct timespec start, end;

	if (argc != 3)
	{
		printf("Usage : reduct <NPROC> <NELEMENTS>\n");
		exit(0);
	}

	pthread_mutex_init(&mut, NULL);

	P = atoi(argv[1]);
	N = atoi(argv[2]);
	
	pthread_barrier_init(&barr, NULL, P);
	// t1 = time(NULL);

	clock_gettime(CLOCK_MONOTONIC, &start);
	for (i=1; i<P; i++)
		pthread_create(&thr[i], NULL, thread_func, &args[i]);
	clock_gettime(CLOCK_MONOTONIC, &end);
	t1 = timespecDiff(&end, &start);
	// t2 = time(NULL);
	clock_gettime(CLOCK_MONOTONIC, &start);
	thread_func(&args[0]);
	clock_gettime(CLOCK_MONOTONIC, &end);
	t2 = timespecDiff(&end, &start);
	// t3 = time(NULL);

	clock_gettime(CLOCK_MONOTONIC, &start);	
	//printf("Thread id waiting \n");
	for (i=1; i<P; i++)
		pthread_join(thr[i], &retval);
	clock_gettime(CLOCK_MONOTONIC, &end);
	t3 = timespecDiff(&end, &start);
	// t4 = time(NULL);


	printf("Time for completion : creation %lu, reduction : %lu, joining : %lu\n",t1, t2, t3);
	return 0;
}