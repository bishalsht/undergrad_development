#include <iostream>
using namespace std;

struct lnode{
	union addr{
		int in;
		lnode *sub;
	};
	int tag;
	addr un;
	lnode *next;
};

void print(lnode *L)
{
	if (L->tag==1)
	{
	   cout<<L->un.in;
	   if (L->next!=NULL)
	   {
	   	   cout<<", ";
		   print (L->next);
	   }
    }
	else if (L->tag==0)
	{
		cout<<"{";
		print (L->un.sub);
		cout<<"}";
		if (L->next!=NULL)
		print(L->un.sub);
	}
}

void add(lnode *L)
{
	char choice;
	int in;
	cout<<"Enter 'a' to add an integer, 'b' for new list and 'c' for terminating the current list:";
	cin>>choice;
	if (choice=='a')
	{
		cout<<"Enter number: ";
		cin>>in;
		L->next=new (lnode);
		L=L->next;
		L->tag=1;
		L->next=NULL;
		L->un.in=in;
		add(L);
	}
	else if (choice=='b')
	{
		L->next=new (lnode);
		L=L->next;
		L->tag=0;
		L->un.sub=new (lnode);
		add (L->un.sub);
		add (L);
	}	
	else if (choice=='c')
	L->next=NULL;
}

int main()
{
	lnode *L;
	int tag,in;
	L=new (lnode);
	L->next=NULL;
	L->un.in=13;
	L->tag=1;
	add(L);
	cout<<L->next->next->un.sub->un.in;
	L=L->next;
	cout<<"{";
	print (L);
	cout<<"}";
	return 0;
}
