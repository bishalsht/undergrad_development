#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[])
{
	char *p;
	int i;
	int size;
	int count;

	if (argc != 3)
		printf("Usage : <prog> <size> <count>\n");

	size = atoi(argv[1]);
	count = atoi(argv[2]);

	for (i=0; i<count; i++)
	{
		p = (char *)malloc(size);
		printf("p : %x\n",p);
		p = p - 0x10;
		printf("1: %x, 2 : %x, 3: %x, 4: %x\n",*(int *)p, *(int *)(p+4), *(int *)(p+8), *(int *)(p+12));
	}
	return 0;
}
	


