#include <iostream>
using namespace std;

struct dnode{
	dnode *left, *right;
	char data;
};

void dnode_create(dnode *&d, char dt)
{
	if (d->right==NULL)
	{
		d->right=new dnode;
		d->right->right=NULL;
		d->right->left=d;
		d->right->data=dt;
	}
	else if (d->right!=NULL)
	{
		dnode_create(d->right,dt);
	}
}

bool rec_palin(dnode *d1,dnode *d2)
{
	bool m=1;
	if (d1->data==d2->data)	
	{
		if (d1->right!=NULL&&d2->left!=NULL)
		m=rec_palin(d1->right,d2->left);
		else 
		return 1;
	}
	else return 0;
	if (m)
	return 1;
	else return 0;
}

int main()
{
	dnode *d1,*d2;
	char dt;
	d1=new (dnode);
	cin>>dt;
	d1->data=dt;
	d1->left=NULL;
	d1->right=NULL;
	cin>>dt;
	while (dt!='$')
	{
		dnode_create(d1,dt);
		cin>>dt;
	}
	d2=d1;
	while (d2->right!=NULL)
	d2=d2->right;
	if(rec_palin(d1,d2))
	cout<<"palindrome.";
	else cout<<"not palindrome.";
	
	return 0;
}
