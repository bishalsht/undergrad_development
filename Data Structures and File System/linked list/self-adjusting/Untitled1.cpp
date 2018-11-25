#include <iostream>
using namespace std;

struct lnode{
	int data;
	lnode *next;
};


void add_begin(lnode *&S, int k)
{
	lnode *T;
	T=new (lnode);
	T->data=k;
	T->next=S;
 	S=T;
}

void print(lnode *S)
{
	cout<<"Elements of the node :\n";
	while (S->next!=NULL)
	{
		cout<<S->data<<endl;
		S=S->next;
	}
	cout<<S->data<<endl;
}

int search (lnode *&S, int k)
{
	lnode *T, *M;
	T=S;
	while (T->next->data!=k)
	T=T->next;
	M=T->next;
	T->next=T->next->next;
	M->next=S;
	S=M;
	return (S->data);
}

int main()
{
	lnode *M;
	int d,sear;
	M=new (lnode);
	cout<<"Enter data :";
	cin>>d;
	M->data=d;
	M->next=NULL;
	cin>>d;
	while (d!=-1)
	{
		add_begin(M,d);
		cin>>d;
	}
	print (M);
	cout<<"Enter the data to be searched: ";
	cin>>sear;
	cout<<"data found: "<<search(M,sear);
	print (M);
	return 0;
}
