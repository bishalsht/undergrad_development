#!/usr/bin/tcc -run

#include <stdio.h>

int main(int argc,char **argv)
{

	FILE *fp;
	int i,totLimit;
	if (argv[1][0]=='-') i=2;
	else i=1;
	for (;i<argc;i++)
	{
		fp=fopen(argv[i],"r");
		if (fp==NULL)
		{
			printf("Error in opening '%s'\n", argv[i]);
			return -1;
		}
		char ch;
		int chCount=0, newCount=0, bytesCount=0;
		while((ch=getc(fp))!=EOF)
		{
			bytesCount++;
			switch (ch)
			{
				case '\n': 	newCount++;
							chCount++;
							break;
				case ' ':
				case '\t': 	chCount++;
							break;
			}
		}
		if (fclose(fp)==EOF)
		{
			printf("Error in closing '%s'\n", argv[i]);
			return -1;
		}
		if (!strcmp(argv[1],"-c")) printf("%d %s\n", bytesCount, argv[i]);
		else if (!strcmp(argv[1],"-l")) printf("%d %s\n", newCount, argv[i]);
		else if (!strcmp(argv[1],"-w")) printf("%d %s\n", chCount, argv[i]);
		else if (argv[1][0]=='-') {
			printf("Invalid option %s used", argv[1]);
			return -1;
		}
		else printf("%d %d %d %s\n",newCount, chCount, bytesCount, argv[i] );

	}
	return 0;
}