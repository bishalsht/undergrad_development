#include <iostream>
using namespace std;

struct lnode{
	int data;
	lnode *next;
};

void lnode_create(lnode *l,int d)
{
	while (l->next!=NULL)
	l=l->next;
	l->next=new lnode;
	l=l->next;
	l->data=d;
	l->next=NULL;
}

void rec_ins_sort(lnode *&l, lnode *b)
{
	if (b!=NULL)
	{
		lnode *n;
		n=l;
		while (n!=b)
		{
			if (n->data>b->data)
			{
				if (n!=l)
				{
					lnode *x;
					x=l;
					while (x->next!=n)
					x=x->next;
					x->next=b;
					lnode *m;
					m=b->next;
					b->next=n;
					while (n->next!=b)
					n=n->next;
					n->next=m;
				}
				else
				{
					while (n->next!=b)
					n=n->next;
					n->next=b->next;
					b->next=l;
					l=b;
				}
				break;
			}
			n=n->next;
		}
		rec_ins_sort(l,b->next);
	}
}

void print(lnode *l)
{
	if (l!=NULL)
	{
		cout<<l->data<<" ";
		print(l->next);
	}
}

int main()
{
	lnode *l;
	cout<<"Lnode creation\nEnter data: ";
	l=new lnode;
	cin>>l->data;
	l->next=NULL;
	int d;
	cin>>d;
	while (d!=-1)
	{
		lnode_create(l,d);
		cin>>d;
	}
	cout<<"Unsorted LL\n";
	print(l);
	rec_ins_sort(l,l->next);
	cout<<"Sorted LL\n";
	print(l);
	cout<<endl;
	
	return 0;
}
