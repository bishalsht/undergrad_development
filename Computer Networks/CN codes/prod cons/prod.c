#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

int *shared_buff;
int *in, *out;

void sem_acq(int semid);
void sem_rel(int semid);

int main(int argc, char const *argv[])
{
	key_t sem_key = ftok("/home/bishal/Documents/CN codes/prod cons/prod.c", 'a');

	int semid = semget(sem_key, 1, IPC_CREAT|0666);
	if (semid == -1) exit(0);
	if (semctl(semid, 0, SETVAL, 1) == -1) exit(0);

	key_t shm_key = ftok("/home/bishal/Documents/CN codes/prod cons/prod.c", 'b');
	int shmid = shmget(shm_key, sizeof(int)*100, IPC_CREAT|0666);
	if (shmid == -1) exit(0);
	shared_buff = shmat(shmid, NULL, 0);
	if (shared_buff == (void *)-1) exit(0);

	key_t in_key = ftok("/home/bishal/Documents/CN codes/prod cons/prod.c", 'c');
	int inid = shmget(in_key, sizeof(int), IPC_CREAT|0666);
	if (inid == -1) exit(0);
	in = shmat(inid, NULL, 0);
	if (in == (void *)-1) exit(0);

	key_t out_key = ftok("/home/bishal/Documents/CN codes/prod cons/prod.c", 'd');
	int outid = shmget(out_key, sizeof(int), IPC_CREAT|0666);
	if (outid == -1) exit(0);
	out = shmat(outid, NULL, 0);
	if (out == (void *)-1) exit(0);

	*in = 0;
	*out = 0;

	while (1)
	{
		sem_acq(semid);
		if ((*in+1)%100 == *out)
			printf("No space to produce anything\n");
		else
		{
			shared_buff[*in] = rand();
			printf("Item Produced: %d\n", shared_buff[*in]);
			fflush(stdout);
			*in = (*in+1)%100;
		}
		sem_rel(semid);
		sleep(rand()%2);
	}

	return 0;
}

void sem_acq(int semid)
{
	struct sembuf buff;
	buff.sem_num = 0;
	buff.sem_op = -1;
	buff.sem_flg = 0;
	if (semop(semid, &buff, 1) == -1) exit(0);
}

void sem_rel(int semid)
{
	struct sembuf buff;
	buff.sem_num = 0;
	buff.sem_op = 1;
	buff.sem_flg = 0;
	if (semop(semid, &buff, 1) == -1) exit(0);
}