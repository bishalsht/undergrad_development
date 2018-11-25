#include <iostream>
using namespace std;

struct node{
	int data;
	node *next;
};

void add_end(node *S, int k)
{
	node *T;
	T=new (node);
	while (S->next!=NULL)
		  S=S->next;
    T->data=k;
    T->next=NULL;
    S->next=T;
}

void print(node *S)
{
	while (S->next!=NULL)
	{
		cout<<S->data<<endl;
		S=S->next;
	}
	cout<<S->data<<endl;
}

int main()
{
	node *L1,*L2;
	int d,del_data,data_af,data_bef,data;
	char input;
	L1=new (node);
	cout<<"Enter data for first list :";
	cin>>d;
	L1->data=d;
	L1->next=NULL;
	cout<<"Enter more data ? (y or n) :";
	cin>>input;
	while (input=='y'||input=='Y')
	{
		cout<<"Enter data :";
		cin>>d;
		add_end(L1,d);
		cout<<"Enter more data ? (y or n) :";
		cin>>input;
	}
	cout<<"Elements of the node 1 :\n";
	print (L1);
	L2=new (node);
	cout<<"Enter data for second list :";
	cin>>d;
	L2->data=d;
	L2->next=NULL;
	cout<<"Enter more data ? (y or n) :";
	cin>>input;
	while (input=='y'||input=='Y')
	{
		cout<<"Enter data :";
		cin>>d;
		add_end(L2,d);
		cout<<"Enter more data ? (y or n) :";
		cin>>input;
	}
	cout<<"Elements of the node 2 :\n";
	print (L2);
	
	node *T;
	int a[50]={'\0'};
	T=L1;
	int z=0;
	while (T!=NULL)
	{
		a[z++]=T->data;
		T=T->next;
	}
	T=L2;
	while (T!=NULL)
	{
		a[z++]=T->data;
		T=T->next;
	}
	
	//Sorting
	for (int i=0;i<z;i++)
	{
	 for (int j=0;j<z-1;j++)
	 {
	 	if (a[j]>a[j+1])
	 	{
	 		int temp=a[j];
	 		a[j]=a[j+1];
	 		a[j+1]=temp;
	 	}
	 }
	}
	
	//Merging
	node *M;
	M=new (node);
	M->data=a[0];
	M->next=NULL;
	for (int i=1;i<z;i++)
	{
		add_end(M,a[i]);
	}
	cout<<"Elements of the merged list :\n";
	print(M);
	
	//Intersection
	node *I;
	I=new (node);
	int y, temp=1;
	for (y=0;y<z-1;y++)
	{
		if ((temp)&&(a[y]==a[y+1]))
		{
			I->data=a[y];
			I->next=NULL;
			temp=0;
		}
		else if ((!temp)&&(a[y]==a[y+1]))
			add_end(I,a[y]);
	}
	cout<<"Elements of the intersection list :\n";
	print (I);
	
	//Union
	node *U;
	U=new (node);
	U->data=a[0];
	U->next=NULL;
	if (a[0]==a[1])
	y=2;
	else y=1;
	for (;y<z-1;y++)
	{
		add_end(U,a[y]);
		if (a[y]==a[y+1])
		   y++;
	}
	if (a[z-1]!=a[z-2])
	add_end(U,a[z-1]);
	cout<<"Elements of the union list :\n";
	print(U);
	
	return 0;
}
