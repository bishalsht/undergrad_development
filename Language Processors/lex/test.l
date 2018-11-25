alpha [a-zA-Z]
letters {alpha}+
alphanum_under [a-zA-Z0-9_]
names {alphanum_under}+
varnames {letters}{names}*
%s START
%%
[ \t]*int[ \t]+ {BEGIN START; printf("integer declaration starting\n");}
<START>{varnames} {printf("token: %s\n", yytext);}
<START>, ;
<START>\; {BEGIN 0; printf("done declaration\n");}
. ;
%%

int main()
{
	yylex();
	return 0;
}