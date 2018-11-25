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

bool bt_msimilar(btree *b1, btree *b2)
{
	bool m=1;
	if (b1==NULL&&b2!=NULL)
	return 0;
	else if (b2==NULL&&b1!=NULL)
	return 0;
	else if (b1==NULL&&b2==NULL)
	return 1;
	else if (b1!=NULL&&b2!=NULL)
	{
		if (b1->data==b2->data)
		{
			m=bt_msimilar(b1->lchild,b2->rchild);
			m=m&&bt_msimilar(b1->rchild,b2->lchild);
			if (m)
			return 1;
		}
		else return 0;
	}
}

int main()
{
	btree *b1, *b2;
	char d;
	cout<<"Enter root for b1: ";
	cin>>d;
	b1=NULL;
	b2=NULL;
	btcreate(b1,d);
	cout<<"Enter root for b2: ";
	cin>>d;
	btcreate(b2,d);
	if (bt_msimilar(b1,b2)==1)
	cout<<"Mirror Similar trees.";
	else cout<<"Not mirror similar trees.";
	
	return 0;	
}
