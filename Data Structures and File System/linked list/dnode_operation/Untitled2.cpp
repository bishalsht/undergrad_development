#include <iostream>
using namespace std;

struct dnode{
	dnode *left;
	int data;
	dnode *right;
};

void add_end(dnode *D, int k)
{
	dnode *T;
	T=new (dnode);
	T->right=NULL;
	T->data=k;
	while (D->right!=NULL)
	D=D->right;
	D->right=T;
	T->left=D;
}

void add_begin(dnode *&D, int k)
{
	dnode *T;
	T= new (dnode);
	T->left=NULL;
	T->right=D;
	T->data=k;
	D=T;
}

void del(dnode *&T, int k)
{
	dnode *D;
	D=T;
	if (D->data==k)
	{
		T=T->right;
		T->left=NULL;
	}
	else
	{			
		while (D->right->data!=k)
		D=D->right;
		D->right=D->right->right;
		D->right->left=D;
	}
}

void add_before(dnode *&T,int m ,int k)
{
	dnode *D,*Q;
	Q=new (dnode);
	Q->data=m;
	D=T;
	if (D->data==k)
	add_begin(T,m);
	else
	{
		while (D->data!=k)
			D=D->right;
		Q->left=D->left;
		Q->right=D;
		D->left->right=Q;
		D->left=Q;
	}
}

void add_after(dnode *T, int m, int k)
{
	dnode *D;
	D=new (dnode);
	D->data=m;
	while (T->data!=k)
	T=T->right;
	if (T->right!=NULL)
	{	D->left=T;
		D->right=T->right;
		T->right->left=D;
		T->right=D;
	}
	else
	{
		D->right=NULL;
		D->left=T;
		T->right=D;
	}	
}

void print(dnode *D)
{
	while (D->right!=NULL)
	{
		cout<<D->data<<endl;
		D=D->right;
	}
	cout<<D->data<<endl;
}

void insert (dnode *D, int m, int n, int dt)
{
	while (D->data!=m&&D->right->data!=n)
	D=D->right;
	add_after(D,dt,m);
}

int main()
{
	dnode *D;
	int d;
	D=new (dnode);
	D->left=NULL;
	D->right=NULL;
	cout<<"data (-1 to stop input):";
	cin>>d;
	D->data=d;
	cin>>d;
	while (d!=-1)
	{
		add_end(D,d);
		cin>>d;
	}
	cout<<"data to be added at beginning (-1 to stop): ";
	cin>>d;
	while (d!=-1)
	{
		add_begin(D,d);
		cin>>d;
	}
	print (D);
	cout<<"data to be deleted :";
	cin>>d;
	del (D,d);
	print (D);
	
	cout<<"data to be added: ";
	cin>>d;
	int add_bef;
	cout<<d<<" to be added before: ";
	cin>>add_bef;
	add_before(D,d,add_bef);
	print(D);
	
	cout<<"data to be added: ";
	cin>>d;
	int add_aft;
	cout<<d<<" to be added after: ";
	cin>>add_aft;
	add_after(D,d,add_aft);
	print (D);
	
	cout<<"data to be added: ";
	cin>>d;
	int m,n;
	cout<<"data before "<<d<<":";
	cin>>m;
	cout<<"data after "<<d<<":";
	cin>>n;
	insert(D,m,n,d);
	print (D);
	
	return 0;
}
