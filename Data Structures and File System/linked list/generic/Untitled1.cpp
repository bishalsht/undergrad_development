#include <iostream>
using namespace std;

union un{
	char x;
	int y;
};

struct node{
	int tag;
	un data;
	node *next;
};

struct node_int{
	int data;
	node_int *next;
};

struct node_char{
	char data;
	node_char *next;
};

void add_end(node *S, int k, int m)
{
	node *T;
	T=new (node);
	while (S->next!=NULL)
		  S=S->next;
    T->data.y=k;
    T->tag=m;
    T->next=NULL;
    S->next=T;
}
void add_end(node *S, char k, int m)
{
	node *T;
	T=new (node);
	while (S->next!=NULL)
		  S=S->next;
    T->data.x=k;
    T->tag=m;
    T->next=NULL;
    S->next=T;
}

void add_end(node_int *S, int k)
{
	node_int *T;
	T=new (node_int);
	while (S->next!=NULL)
		  S=S->next;
    T->data=k;
    T->next=NULL;
    S->next=T;
}

void add_end(node_char *S, char k)
{
	node_char *T;
	T=new (node_char);
	while (S->next!=NULL)
		  S=S->next;
    T->data=k;
    T->next=NULL;
    S->next=T;
}

void print_int(node_int *S)
{
	cout<<"Elements of the node :\n";
	while (S->next!=NULL)
	{
		cout<<S->data<<endl;
		S=S->next;
	}
	cout<<S->data<<endl;
}

void print_char(node_char *S)
{
	cout<<"Elements of the node :\n";
	while (S->next!=NULL)
	{
		cout<<S->data<<endl;
		S=S->next;
	}
	cout<<S->data<<endl;
}

void print(node *S)
{
	cout<<"Elements of the node :\n";
	while (S->next!=NULL)
	{
		if (S->tag)
		cout<<S->data.y<<endl;
		else
		cout<<S->data.x<<endl;
		S=S->next;
	}
	if (S->tag)
	cout<<S->data.y<<endl;
	else
	cout<<S->data.x<<endl;
}


int main()
{
	node *L;
	int d_int,tag;
	char input, d_char;
	L=new (node);
	cout<<"1 for integer input and 0 for character input :";
	cin>>tag;
	L->tag=tag;
	cout<<"Enter data :";
	if (tag)
	{
	cin>>d_int;
	L->data.y=d_int;
	}
	else 
	{
	cin>>d_char;
	L->data.x=d_char;
	}
	L->next=NULL;
	cout<<"Enter more data ? (y or n) :";
	cin>>input;
	while (input=='y'||input=='Y')
	{	
		cout<<"1 for integer input and 0 for character input :";
		cin>>tag;
		cout<<"Enter data :";
		if (tag)
		{
		cin>>d_int;
		add_end(L,d_int,tag);
		}
		else 
		{
		cin>>d_char;
		add_end(L,d_char,tag);
		}
		cout<<"Enter more data ? (y or n) :";
		cin>>input;
	}
	print (L);
	
	node *L2;
	L2=L;
	//integer
	node_int *I;
	int empty=1;
	I=new (node_int);
	while (L2!=NULL)
	{
		if (L2->tag)
		{
			I->data=L2->data.y;
			I->next=NULL;
			empty=0;
			break;
		}
		L2=L2->next;
	}
	if (!empty)
	{
		L2=L2->next;
		while (L2!=NULL)
		{
			if (L2->tag)
			add_end(I,L2->data.y);
			L2=L2->next;
		}
		print_int (I);
	}
	else if (empty)
	cout<<"No integers\n";
	
	L2=L;
	empty=1;
	//character
	node_char *C;
	C=new (node_char);
	while (L2!=NULL)
	{
		if (!L2->tag)
		{
			C->data=L2->data.x;
			C->next=NULL;
			empty=0;
			break;
		}
		L2=L2->next;
	}
	if (!empty)
	{
		L2=L2->next;
		while (L2!=NULL)
		{
			if (!L2->tag)
			add_end(C,L2->data.x);
			L2=L2->next;
		}
		print_char (C);
	}
	else if (empty)
	cout<<"No characters \n";
	
	return 0;
}
