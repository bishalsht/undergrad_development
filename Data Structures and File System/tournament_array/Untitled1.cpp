#include <iostream>
#include <cmath>
using namespace std;

int i=0,j=0,n=5,x=1;

int height(int n)
{
	int m;
	for (m=0;pow(2,m)<n;m++);
	return m;
}

void print(int tree[],int index)
{
	if (index<x)
	{
		print(tree,2*index+1);
		cout<<tree[index]<<" ";
		print(tree,2*index+2);
	}
}

int maximum(int x, int y, int z)
{
	if (x>y&&x>z) return x;
	else if (y>x&&y>z) return y;
	else return z;
}

void tournament(int tree[],int index)
{
	if (index<15)
	{
		tournament(tree,2*index+1);
		tournament(tree,2*index+2);
		if (2*index+1<15)
		{
			int k=maximum(tree[2*index+1],tree[2*index+2],tree[index]);
			tree[index]=k;
			if (tree[2*index+1]==k) tree[2*index+1]=-1;
			else if (tree[2*index+2]==k) tree[2*index+2]=-1;
		}
	}
}

int main()
{
	int a[5]={3,4,5,2,1};
	int m;
	for (m=1;m<=height(n);m++)
	x=pow(2,m)+x;
	int *tree;
	tree=new int[x];
	for (int j=0;j<x;j++)
	tree[j]=-1;
	int b=x-pow(2,--m);
	for (m=0;m<n;m++,b++)
	tree[b]=a[m];
	
	print(tree,0);
	cout<<endl;
	for (int count=0;count<n;count++)
	{
		tournament(tree,0);
		a[count]=tree[0];
		tree[0]=-1;
	}
	for (int count=0;count<n;count++)
	cout<<a[count]<<" ";
	
	return 0;
}
