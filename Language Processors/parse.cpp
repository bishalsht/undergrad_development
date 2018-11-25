#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

void clear(char *buff);
char* remove_spaces (char *buff);
int find_operators(char *buff);
int find_special(char *buff);
int find_identifiers(char *buff);

int main()
{
	ifstream fin("input.cpp");
	char buff[100];
	
	int reserved = 0, special = 0, identifiers = 0, operators = 0;
	
	while (fin.getline(buff, 100, '\n'))
	{
		if (buff[0] == '#' ||  (buff[0] == '\\' && buff[1]=='\\')) continue;
		buff[99] = '~';
		char *temp = remove_spaces(buff);
		
		while (temp[0] == '{' || temp[0] == '}')
		{
			special++;
			temp = temp+1;
			while (temp[0] == '\0') temp = temp+1;
		}
		
		if (!(strcmp(temp, "int")&&strcmp(temp, "char")&&strcmp(temp, "double")&&strcmp(temp, "float")))
		{
			reserved++;
			while (temp[0]!='\0') temp = temp+1;
			temp = temp+1;	
			operators += find_operators(temp);
			special += find_special(temp);	
			identifiers += find_identifiers(temp);	
		}
		else if (!(strcmp(temp, "cout")&&strcmp(temp, "cin")))
		{
			reserved++;
			while (temp[0]!='\0') temp = temp+1;
			temp = temp+1;	
			operators += find_operators(temp);
			special += find_special(temp);		
		}
		else if (!(strcmp(temp, "return")))
		{
			reserved++;
			while (temp[0]!='\0') temp = temp+1;
			temp = temp+1;
			operators += find_operators(temp);
			special += find_special(temp);	
		}
		else if (!(strcmp(temp, "using")))
		{
			reserved = reserved + 3;
			special++;
			while (temp[0]!=';') temp++;
			temp++;
		}
		else
		{
			operators += find_operators(temp);
			special += find_special(temp);
		}
		
		clear(buff);
	}
	
	fin.close();
	
	cout<<"Reserved = "<<reserved<<"\nSpecial = "<<special<<"\nIdentifiers = "<<identifiers<<"\nOperators = "<<operators;
	
	return 0;
}

char* remove_spaces (char *buff)
{
	while (buff[0]==' '||buff[0]=='\t') buff=buff+1;
	for (int i=0;buff[i]!='\0';i++)
	{
		if (buff[i]==' ') buff[i]='\0';
	}
	return buff;
}

void clear(char *buff)
{
	for (int i=0;i<100;i++)
	buff[i] = '\0';
}


int find_operators(char *buff)
{
	int operators = 0;
	while (buff[0]!='~')
	{
		switch(buff[0])
		{
			case '+':
				operators++;
				buff++;
				if (buff[0] == '+' || buff[0] == '=') buff++;
				break;
			case '-':
				operators++;
				buff++;
				if (buff[0] == '-' || buff[0] == '=') buff++;
				break;
			case '*':
				operators++;
				buff++;
				if (buff[0] == '=') buff++;
				break;
			case '/':
				operators++;
				buff++;
				if (buff[0] == '=') buff++;
				break;
			case '=':
				operators++;
				buff++;
				if (buff[0] == '=') buff++;
				break;
			case '%':
				operators++;
				buff++;
				if (buff[0] == '=') buff++;
				break;
			case '|':
				operators++;
				buff++;
				if (buff[0] == '|') buff++;
				break;
			case '&':
				operators++;
				buff++;
				if (buff[0] == '&') buff++;
				break;
			case '<':
				operators++;
				buff++;
				if (buff[0] == '<' || buff[0] == '=') buff++;
				break;
			case '>':
				operators++;
				buff++;
				if (buff[0] == '>' || buff[0] == '=') buff++;
				break;
			case '!':
				operators++;
				buff++;
				if (buff[0] == '=') buff++;
				break;
			default: buff++;
		}
	}
	return operators;
}

int find_special(char *buff)
{
	int special = 0;
	while (buff[0]!='~')
	{
		switch(buff[0])
		{
			case ';':
			case '{':
			case '}':
			case '(':
			case ')':
			case ',':
			case '\'':
			case '\"':
				special++;
				buff++;
				break;
			default: buff++;
		}
	}
	return special;
}

int find_identifiers(char *buff)
{
	int identifier = 1;
	bool fin = false;
	while (buff[0]!='~' && fin==false)
	{
		switch(buff[0])
		{
			case ',' :
				identifier++;
				buff++;
				break;
			case ';' :
				fin = true;
				break;
			default: buff++;
		}
	}
	return identifier;
}
