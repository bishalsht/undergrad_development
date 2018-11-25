#include <iostream>
using namespace std;

struct bstnode{
	bstnode *lchild;
	int data;
	bstnode *rchild;
};

void bst_create(bstnode *&b, int d)
{
	if (b==NULL)
	{
		b=new(bstnode);
		b->data=d;
		b->lchild=NULL;
		b->rchild=NULL;
	}
	else if (d>b->data)
	{
		bst_create(b->rchild,d);
    }
	else if(d<b->data){
		bst_create(b->lchild,d);
	}
}

void printsort(bstnode *b, int k1, int k2)
{
	if (b!=NULL)
	{
		printsort(b->lchild,k1,k2);
		if (b->data>k1&&b->data<k2)
		cout<<b->data<<" ";
		printsort(b->rchild,k1,k2);
	}
}

int main()
{
	bstnode *b;
	int d,k1,k2;
	cout<<"Enter data: ";
	cin>>d;
	b=NULL;
	while (d!=-1)
	{
		bst_create(b,d);
		cin>>d;
	}
	cout<<"Enter the range you want numbers from :";
	cin>>k1>>k2;
	printsort(b,k1,k2);
	
	return 0;
}
