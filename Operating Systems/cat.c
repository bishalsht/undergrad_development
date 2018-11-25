#!/usr/bin/tcc -run

#include <stdio.h>

int main(int argc,char **argv)
{
	FILE *fp;
	for (int i=1;i<argc;i++)
	{
		fp=fopen(argv[i],"r");
		if (fp==NULL)
		{
			printf("Error in opening '%s'\n", argv[i]);
			return -1;
		}
		char ch;
		while((ch=getc(fp))!=EOF)
			printf("%c", ch);
		if (fclose(fp)==EOF)
		{
			printf("Error in closing '%s'\n", argv[i]);
			return -1;
		}

	}
	if (argc==1)
	{
		char buf[256];
		while (1)
		{
			gets(buf);
			if (buf[0]==EOF) break;
			printf("%s\n", buf);
		}
	}
	return 0;
}