#include <iostream>
using namespace std;

struct node{
	char data1;
	int data2;
	node *next;
};

void add_end(node *S, char k, int l)
{
	node *T;
	T=new (node);
	while (S->next!=NULL)
		  S=S->next;
    T->data1=k;
    T->data2=l;
    T->next=NULL;
    S->next=T;
}

void print(node *S)
{
	cout<<"Elements of the node :\n";
	while (S->next!=NULL)
	{
		cout<<S->data1<<" - "<<S->data2<<endl;
		S=S->next;
	}
	cout<<S->data1<<" - "<<S->data2<<endl;
}

void del(node *S, char k, int l)
{
	while (S->next->data1!=k&&S->next->data2!=l)
	S=S->next;
	S->next=S->next->next;
}

int main()
{
	node *L;
	char d1, input, temp;
	int d2;
	L=new (node);
	cout<<"Enter data :";
	cin>>d1>>d2;
	L->data1=d1;
	L->data2=d2;
	L->next=NULL;
	cout<<"Enter more data ? (y or n) :";
	cin>>input;
	while (input=='y'||input=='Y')
	{
		cout<<"Enter data :";
		cin>>d1>>d2;
		add_end(L,d1,d2);
		cout<<"Enter more data ? (y or n) :";
		cin>>input;
	}
	print (L);
	cout<<"\nBinSort\n";
	do{
		int sort[50]={'\0'};
		node *T;
		T=L->next;
		temp=L->data1;
		sort[0]=L->data2;
		cout<<temp<<" - ";
		int i=0;
		while (T!=NULL)
		{
			if (T->data1==temp)
			{
				i++;
				sort[i]=T->data2;
				T=T->next;
				del(L,temp,sort[i]);
			}
			else
			T=T->next;
		}
		for (int j=0;j<=i;j++)
		{
			for (int m=0;m<i;m++)
			{
				if (sort[m]>sort[m+1])
				{
					int tempor=sort[m];
					sort[m]=sort[m+1];
					sort[m+1]=tempor;
				}
			}
		}
		for (int j=0;j<=i;j++)
		cout<<sort[j]<<" ";
		cout<<endl;
		L=L->next;
	} while (L!=NULL);
	return 0;
}
