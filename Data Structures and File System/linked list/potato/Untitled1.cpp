#include <iostream>
#include <cstdlib>
using namespace std;

struct cnode{
	cnode *next;
	int data;
};
void add_end(cnode *c, int k)
{
	cnode *n;
	n=new (cnode);
	n->data=k;
	n->next=c;
	while (c->next->data!=-1)
	c=c->next;
	c->next=n;
}
void add_begin(cnode *c, int k)
{
	cnode *n;
	n=new (cnode);
	n->data=k;
	n->next=c->next;
	c->next=n;
}
int del(cnode *c, int k)
{
	while (c->next->data!=k)
	{
	 c=c->next;
    }
	c->next=c->next->next;
	return k;
}
void print(cnode *c)
{
	while (c->next->data!=-1)
	{
		c=c->next;
		cout<<c->data<<endl;
	}
}

int main()
{
	cnode *c;
	int k;
	c=new (cnode);
	c->next=c;
	c->data=-1;
	cout<<"Adding data at end (Enter -1 to stop)\n";
	cin>>k;
	do
	{
		add_end(c,k);
		cin>>k;
	}while (k!=-1);
	k=5;
	cout<<"Adding data at beginning (Enter -1 to stop)\n";
	do{
		add_begin(c,k);
		cin>>k;
		}while (k!=-1);
	del(c,5);
	print (c);
	
	//potato operation
	
	cnode *temp;
	temp=c;
	while (c->next->next->data!=-1)
	{
		for (int i=0;i<3;i++)
		{
			temp=temp->next;
			if (temp->data==-1)
			temp=temp->next;
		}
		cout<<"The deleted data :"<<del(temp,temp->data)<<endl;
		temp=temp->next;
		if (temp->data==-1)
		temp=temp->next;
	}
	cout<<"The winning number: ";
	if (c->data!=-1)
	cout<<c->data;
	else cout<<c->next->data;
	
	return 0;
}
