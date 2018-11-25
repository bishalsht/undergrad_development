#include <iostream>
using namespace std;

int i=1;

struct bnode{
	char data;
	bnode *lchild, *rchild;
};

void btcreate(bnode *&b, char input[], char d)
{
	if (b==NULL)
	{
		b=new (bnode);
		b->data=d;
		b->rchild=NULL;
		b->lchild=NULL;
	}
	d=input[i++];
	if (d!='$')
	btcreate(b->lchild,input,d);
	else
	{
		b->lchild = NULL;
		i++;
	} 
	d=input[i++];
	if (d!='$')
	btcreate(b->rchild,input,d);
	else
	{
		b->rchild = NULL;
		i++;
	}
	
}

void printnode(bnode *b)
{
	if (b!=NULL)
	{
		printnode(b->lchild);
		cout<<b->data<<" ";
		printnode(b->rchild);
	}
}

int main()
{
	char pre[50]={'\0'};
	char in[50]={'\0'};
	char input[50]={'\0'};
	char pos[50]={'\0'};
	cout<<"Enter preorder sequence:";
	cin>>pre;
	cout<<"\nEnter inorder sequence:";
	cin>>in;
	
	int k=0, l=0, n, p=0;
	char m=0;
	/*for (int i=0;pre[i]!='\0';i++)
	{
		int j;
		for (j=0;in[j]!=pre[i];j++);
		if (k>0)
		{
			if (j<pos[k-1])
			{
				input[l++]=pre[i];
				pos[k++]=j;
			}
			else if (j>pos[k-1])
			{
				input[l++]='$';
				for (n=k-2;n!=-1;n--)
				{
					if (n==0)
					{
						input[l++]=pre[i];
						p=p+1;
						break;
					}
					else if (j>pos[n])
					{
						input[l++]='$';
						p++;
					}
					else
					{
						input[l++]=pre[i];
						break;
					}
				}
				for (int q=1;q<p;q++)
				pos[--k]='\0';
				p=0;
				pos[k++]=j;
			}
		}
		else if (k==0)
		{
			input[l++]=pre[i];
			pos[k++]=j;
		}
	}*/
	
	for (int i=0;pre[i]!='\0';i++)
	{
		int j;
		for (j=0;in[j]!=pre[i];j++);
		if (i==0||pos[l-1]>j)
		{
			pos[l++]=j;
			input[m++]=pre[i];
		}
		else
		{
			while (j>pos[l-1])
			{
				input[m++]='$';
				pos[l-1]='\0';
				l=l-1;
				if (l-1==0)
				break;
			}
			input[m++]=pre[i];
			pos[l++]=j;
		}
	}
	input[m++]='$';
	input[m++]='$';
	
	cout<<input;
	
	//char inut[25]={'x','y','m','$','r','$','$','n','q','$','$','$','z','o','s','t','$','u','$','$','$','$','p','$','$'};
	
	
	bnode *b;
	b=NULL;
	btcreate(b,input,input[0]);
	cout<<"\nTree elements: ";
	printnode(b);
	
	return 0;
}
