/*
	To implement non-preemptive and preemptive shortest-job-first algorithm using threads for process creation and process execution
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define max_size 10

typedef struct heap_node
{
	int num;
	int arr_time;
	int burst_time;
} heap_node;

int done = 0;		// done becomes 1 when all the processes have been added to the heap
pthread_mutex_t done_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t heap_mutex = PTHREAD_MUTEX_INITIALIZER;
heap_node *H;
int *c_s;

//uses the concept of hole movement
void insert_heap2(heap_node* H, int* c_s, heap_node h);

//deletes min element from the heap and returns that element
heap_node delete_min(heap_node* H, int* c_s);

//returns the node having min index value
heap_node find_min(heap_node *H, int* c_s);

void create_process()
{
	int AT = 0, BT, i, halt = 0;
	for(i=0 ; i<5 ; ++i)
	{
		AT += halt;	// dont take random arrival times. It should be equal to the wait
		BT = rand()%8;
		heap_node process = {i+1, AT, BT};
		pthread_mutex_lock(&heap_mutex);
			printf("Process %d arrived. AT = %d, BT = %d\n", i+1, AT, BT);
			insert_heap2(H, c_s, process);
		pthread_mutex_unlock(&heap_mutex);
		
		halt = rand()%6;	// a pause between two process arrivals 
		sleep(halt);
	}
	
	pthread_mutex_lock(&done_mutex);
	done = 1;
	pthread_mutex_unlock(&done_mutex);
}

void execute_process()
{
	heap_node p;
	while(1)
	{
		pthread_mutex_lock(&heap_mutex);
		p = delete_min(H, c_s);

		if(p.arr_time != -1)
		{
			printf("Process %d executing. AT = %d, BT = %d\n", p.num, p.arr_time, p.burst_time);
			pthread_mutex_unlock(&heap_mutex);
			sleep(p.burst_time);	
		}
		else
		{
			pthread_mutex_unlock(&heap_mutex);
			pthread_mutex_lock(&done_mutex);
			if(done == 1)
			{
				pthread_mutex_unlock(&done_mutex);
				break;
			}
			else pthread_mutex_unlock(&done_mutex);
		}
	}
}

void execute_process_preemptive()
{
	heap_node p, q;
	while(1)
	{
		pthread_mutex_lock(&heap_mutex);
		p = delete_min(H, c_s);
		if(p.arr_time != -1)
		{
			printf("Process %d executing. AT = %d, BT = %d\n", p.num, p.arr_time, p.burst_time);
			pthread_mutex_unlock(&heap_mutex);
			once_more:
			sleep(1);
			p.burst_time--;
			if(p.burst_time == 0)
				continue;
			q = find_min(H, c_s);
			if(q.arr_time == -1 || q.burst_time >= p.burst_time)
				goto once_more;
			else // need to preemp 'p'
			{
				pthread_mutex_lock(&heap_mutex);
					insert_heap2(H, c_s, p);
				pthread_mutex_unlock(&heap_mutex);
			}
		}
		else
		{
			pthread_mutex_unlock(&heap_mutex);
			pthread_mutex_lock(&done_mutex);
			if(done == 1)
			{
				pthread_mutex_unlock(&done_mutex);
				break;
			}
			else pthread_mutex_unlock(&done_mutex);
		}
	}
}

int main()
{
	int i, j, curr_size = 0;
	heap_node h;
	H = (heap_node*)malloc(sizeof(heap_node)*max_size);
	
	c_s = &curr_size;
	
	/*
	// to check functionality of heap
	for(i=5 ; i>0 ; --i)
	{
		h.arr_time = 2; h.burst_time = i;
		insert_heap2(H, c_s, h);
	}
	h = delete_min(H, c_s);
	while(h.arr_time != -1)
	{
		printf("%d  ", h.burst_time);
		h = delete_min(H, c_s);
	}*/
	
	pthread_t p, q;
	pthread_create(&p, NULL, (void*)&create_process, 0);
	pthread_create(&q, NULL, (void*)&execute_process, 0);
	//pthread_create(&q, NULL, (void*)&execute_process_preemptive, 0);
	pthread_join(p, NULL);
	pthread_join(q, NULL);
	
	return 0;
}

void insert_heap2(heap_node* H, int* c_s, heap_node h)
{
	int hole = (*c_s)++;      //index where the key has to be added
	while(H[(hole-1)/2].burst_time > h.burst_time && hole>0)
	{
		H[hole] = H[(hole-1)/2];
		hole = (hole-1)/2;           //move the hole up the heap
	}
	H[hole] = h;
}

heap_node delete_min(heap_node* H, int* c_s)
{
	heap_node del = {-1, -1};
	if(!(*c_s))
		return del;
	del = H[0];
	int hole = 0;
	//if lchild exists, then only rchild can exist
	while(2*hole+1 < *c_s)     //while lchild exists
	{
		if(2*hole+2 < *c_s)     //if rchild also exists
		{
			if(H[2*hole+1].burst_time < H[2*hole+2].burst_time)
			{
				H[hole] = H[2*hole+1];
				hole = 2*hole+1;
			}
			else
			{
				H[hole] = H[2*hole+2];
				hole = 2*hole+2;
			}
		}
		else
		{
			H[hole] = H[2*hole+1];
			hole = 2*hole+1;
		}
	}
	H[hole] = H[--(*c_s)];	// move the last element to the hole
	return del;
}

heap_node find_min(heap_node *H, int* c_s)
{
	heap_node ret;
	ret = H[0];     //copy the structure
	return ret;
}
