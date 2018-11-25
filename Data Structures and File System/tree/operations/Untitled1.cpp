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

int max(bstnode *b)
{
	int k=0;
	if (b->rchild!=NULL)
	k=max(b->rchild);
	if (k)
	return k;
	return b->data;
}

int min(bstnode *b)
{
	while (b->lchild!=NULL)
	b=b->lchild;
	return (b->data);
}

void tprint(bstnode *b)
{
	if (b->lchild==NULL&&b->rchild==NULL)
	cout<<b->data<<" ";
	if(b->lchild!=NULL)
	tprint(b->lchild);
	if(b->rchild!=NULL)
	tprint(b->rchild);
}

int search(bstnode *b, int k)
{
	if(b==NULL) return 0;
	if (b->data==k)
	return 1;
	else if (b->data>k) return search(b->lchild,k);
	else if (b->data<k) return search (b->rchild,k);
}

void printsort(bstnode *b)
{
	if (b!=NULL)
	{
		printsort(b->lchild);
		cout<<b->data<<" ";
		printsort(b->rchild);
	}
}
void del(bstnode *&a, int d)
{
	if (a!=NULL)
	{
		if (a->data>d)
		del (a->lchild,d);
		else if (a->data<d)
		del (a->rchild,d);
		if (a->data==d&&(a->lchild!=NULL&&a->rchild!=NULL))
		{
			bstnode *b;
			b=a->rchild;
			while (b->lchild!=NULL)
			b=b->lchild;
			int e=b->data;
			del(a,e);
			a->data=e;
		}
		else if (a->data==d&&(a->lchild==NULL||a->rchild==NULL))
		{
			if (a->lchild==NULL)
			a=a->rchild;
			else if (a->rchild==NULL)
			a=a->lchild;
		}
	}
}

int main()
{
	bstnode *b;
	int d;
	cout<<"Enter data: ";
	cin>>d;
	b=NULL;
	while (d!=-1)
	{
		bst_create(b,d);
		cin>>d;
	}
	cout<<"maximum: "<<max(b)<<endl;
	cout<<"minimum: "<<min(b)<<endl;
	cout<<"terminal nodes: ";
	tprint(b);
	cout<<"\ndata to search: ";
	cin>>d;
	if (search(b,d)) cout<<"Data found\n";
	else cout<<"Data not found\n";
	cout<<"data in sorted order: ";
	printsort(b);
	cout<<"\nEnter data to delete: ";
	cin>>d;
	if (b->data!=d||b->lchild->data!=d||b->rchild->data!=d)
	del(b,d);
	cout<<"data in sorted order: ";
	printsort(b);
	
	return 0;
}
