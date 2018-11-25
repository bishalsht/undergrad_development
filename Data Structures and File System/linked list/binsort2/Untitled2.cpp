#include <iostream>
using namespace std;

class inode{
	int in;
	char ch;
	inode *next;
};

void add_end(inode *S, char ch, int in)
{
	inode *T;
	T=new (inode);
	T->ch=ch;
	T->in=in;
	T->next=NULL;
	while (S->next!=NULL)
	S=S->next;
	S->next=T;
}

void print(inode *S)
{
	while (S!=NULL)
	{
		cout<<S->ch<<" - "<<S->in<<endl;
		S=S->next;
	}
}

struct lnode{
	int data;
	lnode *next;
};

struct onode{
	lnode *integer;
	onode *next;
	char data;
};


