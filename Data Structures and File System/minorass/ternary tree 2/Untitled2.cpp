#include <iostream>
using namespace std;

struct tnode{
	char data1,data2;
	tnode *lchild, *mchild, *rchild;
	tnode()
	{
		data1='$';
		data2='$';
	}
};

int height(tnode *L)
{
	if (L==NULL)
	return (-1);
	else 
	{
		int a=height(L->lchild);
		int b=height(L->mchild);
		int c=height(L->rchild);
		if (a>b&&a>c) return (a+1);
		else if (b>a&&b>c) return (b+1);
		else if (c>a&c>b) return (c+1);
	}
}

bool tnode_create(tnode *&t, char d)
{
	bool m=1;
	if (d=='$');
	else if (t==NULL)
	{
		t=new tnode;
		t->data1=d;
		t->lchild=NULL;
		t->rchild=NULL;
		t->mchild=NULL;
		cout<<"Enter data: ";
		cin>>d;
		if (d!='$')
			t->data2=d;
		else
		m=0;
	}
	else if (height(t->rchild)<height(t->lchild)&&height(t->rchild)<height(t->mchild))
	m=m&&tnode_create(t->rchild,d);
	else if (height(t->mchild)<height(t->lchild))
	m=m&&tnode_create(t->mchild,d);
	else
	m=m&&tnode_create(t->lchild,d);
	return m;
}

void print(tnode *t)
{
	if (t!=NULL)
	{
		cout<<t->data1<<" ";
		if (t->data2!='$')
		cout<<t->data2<<" ";
		print(t->lchild);cout<<endl<<1;
		print(t->mchild);cout<<endl<<1;
		print(t->rchild);cout<<endl<<1;
	}
}

int main()
{
	tnode *t;
	t=NULL;
	int elements=0,line=0;
	char ch;
	cout<<"Enter data: ";
	cin>>ch;
	bool x=1;
	while (ch!='$'&&x==1)
	{
		x=tnode_create(t,ch);
		cout<<"Enter data: ";
		cin>>ch;
	}
	print (t);
	
	return 0;
}
