#include <iostream>
using namespace std;

struct btree{
	btree *lchild;
	char data;
	btree *rchild;
};

void btcreate(btree *&b, char d)
{
	if (b==NULL)
	{
		b=new (btree);
		b->data=d;
		b->rchild=NULL;
		b->lchild=NULL;
	}
	cout<<"Enter left child of "<<b->data<<": ";
	cin>>d;
	if (d!='$')
	btcreate(b->lchild,d);
	cout<<"Enter right child of "<<b->data<<": ";
	cin>>d;
	if (d!='$')
	btcreate(b->rchild,d);
}

void btdel(btree *b, char d)
{
	if (b->lchild!=NULL&&b->lchild->data!=d)
	btdel(b->lchild,d);
	if (b->rchild!=NULL&&b->rchild->data!=d)
	btdel(b->rchild,d);
	if (b->lchild!=NULL&&b->lchild->data==d)
	{
		if (b->lchild->rchild==NULL)
		   b->lchild=b->lchild->lchild;
        else if (b->lchild->lchild==NULL)
           b->lchild=b->lchild->rchild;
	}
	else if (b->rchild!=NULL&&b->rchild->data==d)
	{
		if (b->rchild->rchild==NULL)
		   b->rchild=b->rchild->lchild;
		else if (b->rchild->lchild==NULL)
		   b->rchild=b->rchild->rchild;
	}
	else if (b->rchild!=NULL&&b->lchild!=NULL&&b->data==d)
	cout<<"Cannot delete.";
}

void btprint(btree *b)
{
	if (b!=NULL)
	{
		btprint(b->lchild);
		cout<<b->data<<" ";
		btprint(b->rchild);
	}
}

int main()
{
	btree *b;
	char d;
	cout<<"Enter root for b1: ";
	cin>>d;
	b=NULL;
	btcreate(b,d);
	cout<<"\nall elements: ";
	btprint(b);
	cout<<"\nEnter number to delete: ";
	cin>>d;
	btdel(b,d);
	cout<<"\nall elements: ";
	btprint(b);
	
	return 0;
}
