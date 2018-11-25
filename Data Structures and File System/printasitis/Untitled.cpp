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

int height(btree *T)
{
	if (T==NULL) return -1;
	else 
	{
		int h1,h2;
		h1=height(T->lchild);
		h2=height(T->rchild);
		return (max(h1,h2)+1);
	}
}
void levelprint(btree *T, int h)
{
	if (T!=NULL)
	{
		if (h==0)
		{
			int i;
			int m=height (T);
			for (i=0;i<m*m+m;i++) cout<<" ";
			cout<<T->data;
		}
		else
		{
			levelprint(T->lchild,h-1);
			int j;
			int x=height(T);
			for (j=0;j<x*x+x;j++)
			cout<<" ";
			levelprint(T->rchild,h-1);
		}
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
	cout<<"\narray printing according to level:";
	int h=height(b);
	for (int i=0;i<h;i++)
	{
		cout<<endl;
		levelprint(b,i);
	}
	
	return 0;
}
