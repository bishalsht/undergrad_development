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
	key_t semin_key = ftok("/home/bishal/Documents/CN codes/prod cons/prod.c", 'a');
	int seminid = semget(semin_key, 1, IPC_CREAT|0666);
	if (seminid == -1) exit(0);
	if (semctl(seminid, 0, SETVAL, 1) == -1) exit(0);

	key_t semout_key = ftok("/home/bishal/Documents/CN codes/prod cons/prod.c", 'b');
	int semoutid = semget(semout_key, 1, IPC_CREAT|0666);
	if (semoutid == -1) exit(0);
	if (semctl(semoutid, 0, SETVAL, 0) == -1) exit(0);

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

	*in = 5;
	*out = 1;

	while (1)
	{
		sem_acq(seminid);
		printf("Process 1 acquired resource 1\n");
		*in = *out+1;
		printf("Process 1 changed value in resource 1\n");
		printf("resource 1: %d\tresource 2:%d\n", *in, *out);
		sem_rel(semoutid);
		printf("Process 1 released resource 2\n");
		sleep(2);
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