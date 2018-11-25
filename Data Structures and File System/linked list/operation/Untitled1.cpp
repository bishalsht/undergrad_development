#include <iostream>
using namespace std;

struct node{
	int data;
	node *next;
};

void add_begin(node *&S, int k)
{
	node *T;
	T=new (node);
	T->data=k;
	T->next=S;
 	S=T;
}
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
void add_af(node *S, int g, int k)
{
	node *T;
	T=new (node);
	while (S->data!=g)
	S=S->next;
	T->data=k;
	T->next=S->next;
	S->next=T;
}
void add_bef(node *S, int g, int k)
{
	node *T;
	T=new (node);
	while (S->next->data!=g)
	S=S->next;
	T->data=k;
	T->next=S->next;
	S->next=T;
}
void del(node *S, int x)
{
	while (S->next->data!=x)
	S=S->next;
	S->next=S->next->next;
}
void append(node *S, node *T)
{
	while (S->next!=NULL)
	S=S->next;
	S->next=T;
}
void print(node *S)
{
	cout<<"Elements of the node :\n";
	while (S->next!=NULL)
	{
		cout<<S->data<<endl;
		S=S->next;
	}
	cout<<S->data<<endl;
}


int main()
{
	node *L;
	int d,del_data,data_af,data_bef,data;
	char input;
	L=new (node);
	cout<<"Enter data :";
	cin>>d;
	L->data=d;
	L->next=NULL;
	cout<<"Enter more data ? (y or n) :";
	cin>>input;
	while (input=='y'||input=='Y')
	{
		cout<<"Enter data :";
		cin>>d;
		add_end(L,d);
		cout<<"Enter more data ? (y or n) :";
		cin>>input;
	}
	print (L);
	cout<<"Enter the data to be deleted :";
	cin>>del_data;
	del(L,del_data);
	print (L);
	cout<<"Enter after which data you want to add more data :";
	cin>>data_af;
	cout<<"Enter the data to be added :";
	cin>>data;
	add_af(L,data_af,data);
	print (L);
	cout<<"Enter before which data you want to add more data :";
	cin>>data_bef;
	cout<<"Enter the data to be added :";
	cin>>data;
	add_bef(L,data_bef,data);
	print (L);
	cout<<"Enter data to be added at the beginning of the list :";
	cin>>data;
	add_begin(L,data);
	print (L);
	cout<<"Enter data to be added at the end of the list :";
	cin>>data;
	add_end (L,data);
	print (L);
	node *N;
	cout<<"Enter new data for the node that is to be appended in the previously formed node :";
	N=new (node);
	cin>>d;
	N->data=d;
	N->next=NULL;
	cout<<"Enter more data ? (y or n) :";
	cin>>input;
	while (input=='y'||input=='Y')
	{
		cout<<"Enter data :";
		cin>>d;
		add_end(N,d);
		cout<<"Enter more data ? (y or n) :";
		cin>>input;
	}
	print (N);
	cout<<"The node after appending the new node :\n";
	append(L,N);
	print(L);
	return 0;
}
