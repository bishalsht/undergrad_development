#include <iostream>
using namespace std;

struct tnode{
	tnode *lchild;
	int data1;
	tnode *mchild;
	int data2;
	tnode *rchild;
};

void tnode_create(tnode *&T, int d)
{
	if (T==NULL)
	{
		T=new (tnode);
		T->lchild=NULL;
		T->mchild=NULL;
		T->rchild=NULL;
		T->data1=d;
		T->data2=-1;
	}
	else if (T->data2==-1&&T->data1<d)
	{
		T->data2=d;
	}
	else if (T->data2==-1&&T->data1>d)
	{
		T->data2=T->data1;
		T->data1=d;
	}
	else if (T->data1!=-1&&T->data2!=-1)
	{
		if (T->data1>d)
			tnode_create(T->lchild,d);
	    else if (T->data1<d&&T->data2>d)
	    	 tnode_create(T->mchild,d);
		else if (T->data2<d)
			 tnode_create(T->rchild,d);
	}
}

void tnode_print(tnode *T)
{
	if (T->lchild!=NULL)
	tnode_print(T->lchild);
	cout<<T->data1<<" ";
	if (T->mchild!=NULL)
	tnode_print(T->mchild);
	if (T->data2!=-1)
	cout<<T->data2<<" ";
	if (T->rchild!=NULL)
	tnode_print(T->rchild);
}

int tnode_min(tnode *T)
{
	int m=0;
	if (T->lchild!=NULL)
	m=tnode_min(T->lchild);
	if (m)
	return m;
	else 
	return T->data1;
}

int tnode_max(tnode *T)
{
	int m=0;
	if (T->rchild!=NULL)
	m=tnode_max(T->rchild);
	if (m)
	return m;
	else 
	{
		if (T->data2!=-1)
		return T->data2;
		else return T->data1;
	}
}

int main()
{
	tnode *T;
	int d;
	T=new(tnode);
	cout<<"Enter data: ";
	cin>>d;
	T->data1=d;
	T->lchild=NULL;
	T->mchild=NULL;
	T->rchild=NULL;
	T->data2=-1;
	do{
		tnode_create(T,d);
		cin>>d;
	}while (d!=-1);
	cout<<"\nElements: ";
	tnode_print(T);
	cout<<"\nSmallest Element: "<<tnode_min(T);
	cout<<"\nLargest Element: "<<tnode_max(T);
	return 0;
}
