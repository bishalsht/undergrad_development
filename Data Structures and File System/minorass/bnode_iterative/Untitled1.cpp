#include <iostream>
using namespace std;

struct bnode{
	bnode *lchild, *rchild;
	int data;
};

bnode* search(bnode *b,bnode *t)
{
	bnode *c;
	if (b->lchild==t)
	return (b);
	else if (b->rchild==t)
	return (b);
	if (b->lchild!=NULL)
	c=search (b->lchild,t);
	if (b->rchild!=NULL)
	c=search (b->rchild,t);
	return c;
}

void bnode_create(bnode *b)
{
	bnode *t;
	t=b;
	int d;
	bool fin=0;
	while (fin==0)
	{
		cout<<"left of "<<t->data<<":";
		cin>>d;
		while (d!=-1)
		{
			t->lchild=new bnode;
			t=t->lchild;
			t->data=d;
			t->lchild=NULL;
			t->rchild=NULL;
			cout<<"left of "<<t->data<<":";
			cin>>d;
		}
		cout<<"right of "<<t->data<<":";
		cin>>d;
		while (d==-1)
		{
			if (t==b)
			{
				fin=1;
				break;
			}
			t=search(b,t);
			while (t->rchild!=NULL)
			{
				if (t==b)
				{
					fin=1;
					break;
				}
				t=search(b,t);
			}
			if (!fin)
			{
				cout<<"right of "<<t->data<<":";
				cin>>d;
			}
		}
		if (!fin)
		{
			t->rchild=new bnode;
			t=t->rchild;
			t->data=d;
			t->lchild=NULL;
			t->rchild=NULL;
		}
	}
}

void print(bnode *b)
{
	if (b!=NULL)
	{
		print(b->lchild);
		cout<<b->data<<" ";
		print(b->rchild);
	}
}

int main()
{
	bnode *b;
	cout<<"root:";
	b=new bnode;
	b->lchild=NULL;
	b->rchild=NULL;
	cin>>b->data;	
	bnode_create(b);
	print (b);
	return 0;
}
