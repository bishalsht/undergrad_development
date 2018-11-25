#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

unsigned int *shm1, *shm2;
pid_t pid;

void sighandler()
{
	*shm2 = *shm1 + 1;
	printf("Updated variable 2 to : %u\n", *shm2);
	signal(SIGUSR2, sighandler);
	sleep(1);
	kill (pid, SIGUSR1);
}

void sem_acq(int semid);
void sem_rel(int semid);

int main(int argc, char const *argv[])
{
	signal(SIGUSR2, sighandler);

	int seminid = semget(1111, 1, IPC_CREAT|0666);
	if (seminid == -1) exit(0);
	if (semctl(seminid, 0, SETVAL, 0) == -1) exit(0);

	int shm1id = shmget(12345, sizeof(unsigned int), IPC_CREAT|0666);
	if (shm1id == -1) exit(0);
	shm1 = shmat(shm1id, NULL, 0);
	if (shm1 == (void *)-1) exit(0);

	int shm2id = shmget(45678, sizeof(unsigned int), IPC_CREAT|0666);
	if (shm2id == -1) exit(0);
	shm2 = shmat(shm2id, NULL, 0);
	if (shm2 == (void *)-1) exit(0);

	*shm2 = getpid();

	sleep(5);
	pid = *shm1;

	*shm1 = 5;
	*shm2 = 6;

	printf("Variable 1 intialized to : %d and variable 2 intialized to : %d\n", *shm1, *shm2);

	kill(pid, SIGUSR1);

	while(1);

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
	buff.sem_op = 2;
	buff.sem_flg = 0;
	if (semop(semid, &buff, 1) == -1) exit(0);
}