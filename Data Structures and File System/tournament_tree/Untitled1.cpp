#include <iostream>
#include <cmath>
using namespace std;

int i=0,j=0,n=5;

struct btree{
	int data;
	btree *lchild, *rchild, *parent;
	btree()
	{
		data=-1;
		lchild=NULL;
		rchild=NULL;
		parent=NULL;
	}
};

void create(btree *&T, int curr_level, int max_level, int a[])
{
	if (T!=NULL)
	{
		if (curr_level<max_level) create (T->lchild,curr_level+1,max_level,a);
		if (curr_level<max_level) create (T->rchild,curr_level+1,max_level,a);
		if (curr_level==max_level&&i<n) T->data=a[i++];
		if (T->lchild) T->lchild->parent=T;
		if (T->rchild) T->rchild->parent=T;
	}
	else
	{
		T=new btree;
		create(T,curr_level,max_level,a);
	}
}

int height(int n)
{
	int m;
	for (m=0;pow(2,m)<n;m++);
	return m;
}

void print(btree *T)
{
	if (T!=NULL)
	{
		print(T->lchild);
		cout<<T->data<<" ";
		print(T->rchild);
	}
}

int maximum(int x, int y, int z)
{
	if (x>y&&x>z) return x;
	else if (y>x&&y>z) return y;
	else return z;
}

void tournament(btree *T)
{
	if (T!=NULL)
	{
		tournament(T->lchild);
		tournament(T->rchild);
		if (T->lchild!=NULL)
		{
			int k=maximum(T->lchild->data,T->rchild->data,T->data);
			T->data=k;
			if (T->lchild->data==k) T->lchild->data=-1;
			else if (T->rchild->data==k) T->rchild->data=-1;
		}
	}
}

int main()
{
	int a[5]={3,4,5,2,1};
	btree *T;
	T=NULL;
	create (T,0,height(n),a);
	print(T);
	cout<<endl;
	for (int count=0;count<n;count++)
	{
		tournament(T);
		a[count]=T->data;
		T->data=-1;
	}
	for (int count=0;count<n;count++)
	cout<<a[count]<<" ";
	
	return 0;
}
