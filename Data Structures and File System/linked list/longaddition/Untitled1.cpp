#include <iostream>
using namespace std;

struct lnode{
	int data;
	lnode *next;
};

void add_char(lnode *S,char a)
{
	lnode *T;
	T=new (lnode);
	T->data=static_cast<int>(a)-48;
	T->next=NULL;
	while (S->next!=NULL)
	S=S->next;
	S->next=T;
}

void print(lnode *S)
{
	while (S!=NULL)
	{
		cout<<S->data;
		S=S->next;
	}
}

void add_begin(lnode *&L, int k)
{
	lnode *T;
	T=new (lnode);
	T->data=k;
	T->next=L;
	L=T;
}

void addition(lnode *L1, lnode *L2)
{
	lnode *sum, *T1, *T2, *M, *N;
	sum=new (lnode);
	T1=L1;
	T2=L2;
	M=L1;
	N=L2;
	while (T1->next!=NULL)
	T1=T1->next;
	while (T2->next!=NULL)
	T2=T2->next;
	sum->data=T1->data+T2->data;
	sum->next=NULL;
	while (T1!=L1&&T2!=L2)
	{
		while (M->next!=T1)
		M=M->next;
		T1=M;
		while (N->next!=T2)
		N=N->next;
		T2=N;
		add_begin(sum,T1->data+T2->data);
		M=L1;
		N=L2;
	}
	while (T1!=L1)
	{
		while (M->next!=T1)
		M=M->next;
		T1=M;
		add_begin (sum, T1->data);
		M=L1;
	}
	while (T2!=L2)
	{
		while (N->next!=T2)
		N=N->next;
		T2=N;
		add_begin (sum, T2->data);
		N=L2;
	}
	T1=sum;
	T2=sum;
	while (T2->next!=NULL)
	T2=T2->next;
	while (T2!=sum)
	{
		while (T1->next!=T2)
		T1=T1->next;
		T2=T1;
		T1=T1->next;
		if (T1->data/10>0)
		{
			T2->data=T2->data+(T1->data/10);
			T1->data=T1->data%10;
		}
		T1=sum;
	}
	print (sum);
}

int main()
{
	char ch;
	lnode *L1, *L2;
	
	L1=new (lnode);
	cout<<"Enter data(any other key to stop):";
	cin>>ch;
	L1->data=static_cast<int>(ch)-48;
	L1->next=NULL;
	cin>>ch;
	while (ch>='0'&&ch<='9')
	{
		add_char(L1,ch);
		cin>>ch;
	}
	cout<<"Data: ";
	print (L1);
	
	L2=new (lnode);
	cout<<"\nEnter data(any other key to stop):";
	cin>>ch;
	L2->data=static_cast<int>(ch)-48;
	L2->next=NULL;
	cin>>ch;
	while (ch>='0'&&ch<='9')
	{
		add_char(L2,ch);
		cin>>ch;
	}
	cout<<"Data: ";
	print (L2);
	
	cout<<endl<<"Sum: ";
	addition(L1,L2);
	
	return 0;
	
}
