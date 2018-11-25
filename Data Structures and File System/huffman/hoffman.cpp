#include <iostream>
#include <fstream>
using namespace std;

struct bnode_un{
	int weight;
	char data;
	bnode_un()
	{
		weight=-1;
		data='\0';
	}
};

struct bnode{
	bnode *lchild,*rchild,*parent;
	bnode_un info;
	bnode()
	{
		lchild=NULL;
		rchild=NULL;
		parent=NULL;
	}
};

void sort(bnode *b[],int last)
{
	for (int i=0;i<last-1;i++)
	{
		for (int j=0;j<last-2;j++)
		{
			if (b[j]->info.weight<b[j+1]->info.weight)
			{
				bnode *temp;
				temp=b[j];
				b[j]=b[j+1];
				b[j+1]=temp;
			}
		}
	}
}

void create(bnode *b[],int &last)
{
	bnode *n;
	n=new bnode;
	n->info.weight;
	n->lchild=b[last-2];
	n->rchild=b[last-1];
	n->lchild->parent=n;
	n->rchild->parent=n;
	n->info.weight=n->lchild->info.weight+n->rchild->info.weight;
	n->info.data='\0';
	b[last-2]=n;
	b[last-1]=NULL;
	sort(b,last);
}

bool print(bnode *b,ofstream &fout)
{
	bool m=true;
	if (b->lchild&&m)
	{
		fout<<0;
		m=print(b->lchild,fout);
	}
	if (b->rchild&&m)
	{
		fout<<1;
		m=m&&print(b->rchild,fout);
	}
	if (m&&b->lchild==NULL&&b->rchild==NULL&&b->info.data!='\0')
	{
		fout<<":"<<b->info.data<<" "<<b->info.weight<<endl;
		bnode *t=b->parent;
		if (b==t->lchild)
		t->lchild=NULL;
		else t->rchild=NULL;
		b=t->parent;
		while (b!=NULL&&(t->lchild==NULL&&t->rchild==NULL)&&t->info.data=='\0')
		{
			if (t==b->lchild)
			b->lchild=NULL;
			else b->rchild=NULL;
			t=b;
			b=b->parent;
		}
		return false;
	}
	if (m)
	return true;
	else return false;
}

int main()
{
	remove("code.txt");
	bnode *b[7];
	for (int i=0;i<7;i++)
	b[i]=new bnode;
	b[0]->info.weight=10;
	b[0]->info.data='a';
	b[1]->info.weight=15;
	b[1]->info.data='e';
	b[2]->info.weight=12;
	b[2]->info.data='i';
	b[3]->info.weight=3;
	b[3]->info.data='s';
	b[4]->info.weight=4;
	b[4]->info.data='t';
	b[5]->info.weight=13;
	b[5]->info.data=' ';
	b[6]->info.weight=1;
	b[6]->info.data='\n';
	
	sort (b,7);
	
	for (int last=7;last!=1;last--)
	create(b,last);
	
	ofstream fout("code.txt");
	for (int i=0;i<7;i++)
	print(b[0],fout);
	fout.close();
	
	return 0;
}
