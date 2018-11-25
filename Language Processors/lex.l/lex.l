digit [0-9]
digits {digit}+
letter [a-zA-Z]
letters {letter}+
sum \+
mult \*

%{
	#include <stdio.h>
	#include <stdlib.h>
	int idCount = 0;
	int sumCount = 0;
	int mulCount = 0;
%}

%%
^integer[ ]+{letters}[\,{letters}]*; idCount++;
^{letters}{sum}{letters} sumCount++;
^{letters}{mult}{letters} mulCount++;
.* ;
%%

int main(int argc, char *argv[])
{
	yyin = fopen(argv[1], "r");
	yylex();
	fclose(yyin);
	printf("Identifier count: %d\n", idCount);
	printf("Sum: %d\n", sumCount);
	printf("Mul: %d\n", mulCount);
	return 0;
}