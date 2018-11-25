#include <iostream>
using namespace std;

#define d 1

struct btnode{
	int key[2*d];
	btnode *child[2*d+1];
	int count;
	btnode(){
		for (int i=0;i<2*d;i++)
		{
			key[i]=-1;
			child[i]=NULL;
		}
		count=1;
		child[2*d]=NULL;
	}
};

int array_sort(int *&a,int size)
{
	int m=a[size-1];
	for (int i=0;i<size;i++)
	{
		for (int j=0;j<size-1;j++)
		{
			if (a[j]>a[j+1])
			{
				a[j]=a[j+1]+a[j];
				a[j+1]=a[j]-a[j+1];
				a[j]=a[j]-a[j+1];
			}
		}
	}
	for (int i=0;i<size;i++)
	{
		if (a[i]==m)
		return i;
	}
}

btnode* create(btnode *&T, int data)
{
	if (T!=NULL)
	{
		int i;
		btnode *temp;
		for (i=0;i<2*d&&T->key[i]<data&&T->key[i]!=-1;i++);
		temp=create(T->child[i],data);
		if (temp!=NULL)
		{
			if (T->count<2*d)
			{
				int dt[T->count+1];
				for (i=0;i<T->count;i++)
				dt[i]=T->key[i];
				dt[T->count]=temp->key[0];
				int *m;
				m=&dt[0];
				T->count++;
				int s=array_sort(m,T->count);
				for (i=T->count;i>s+1;i--)
				T->child[i]=T->child[i-1];
				T->child[s]=temp->child[0];
				T->child[s+1]=temp->child[1];
				for (i=0;i<T->count;i++)
				T->key[i]=dt[i];
				return NULL;
			}
			else if (T->count==2*d)
			{
				int dt[T->count+1];
				for (int i=0;i<T->count;i++)
				dt[i]=T->key[i];
				dt[T->count]=temp->key[0];
				int *m;
				m=&dt[0];
				int s=array_sort(m,2*d+1);
				btnode *k;
				k=new btnode;
				k->count=1;
				k->key[0]=dt[d];
				k->child[0]=T;
				T->count=d;
				k->child[1]=new btnode;
				k->child[1]->count=d;
				if (temp->child[0]==NULL)
				{
					for (i=0;i<d;i++)
					T->key[i]=dt[i];
					for (i=0;i<d;i++)
					{
						k->child[1]->key[i]=dt[d+i+1];
						T->key[i+d]=-1;
					}
					T=k;
					return k;
				}
				else
				{
					if (dt[d]==T->key[d-1])
					{
					    for (i=0;i<d;i++)
					    {
					    	k->child[1]->key[i]=dt[d+i+1];
					    	T->key[i+d]=-1;
					    	k->child[1]->child[i]=T->child[d+i];
					    }
					    k->child[1]->child[d]=T->child[2*d];
					    for (i=0;i<=d;i++)
					    T->child[i+d]=NULL;
					    for (i=d;i>s;i--)
					    T->child[i]=T->child[i-1];
					    T->child[s]=temp->child[0];
					    T->child[s+1]=temp->child[1];
						for (i=0;i<d;i++)
						T->key[i]=dt[i];
					    T=k;
					    return k;
					}
					else if (dt[d]==temp->key[0])
					{
						for (i=1;i<=d;i++)
						{
							k->child[1]->child[i]=T->child[d+i];
							k->child[1]->key[i-1]=T->key[d+i-1];
						}
						k->child[1]->child[0]=temp->child[1];
						T->child[d]=temp->child[0];
						for (i=0;i<d;i++)
						{
							T->child[i+d+1]=NULL;
							T->key[i+d]=-1;
						}
						T=k;
						return k;
					}
					else if (dt[d]==T->key[d])
					{
						for (i=0;i<d;i++)
						{
							k->child[1]->key[i]=dt[i+d+1];
							T->key[i+d]=-1;
						}
						for (i=0;i<s-d-1;i++)
						{
							k->child[1]->child[i]=T->child[d+i+1];
							T->child[d+i+1]=NULL;
						}
						T->child[d+i+1]=NULL;
						k->child[1]->child[i]=temp->child[0];
						k->child[1]->child[i+1]=temp->child[1];
						for (i=i+2;i<d+1;i++)
						{
							k->child[1]->child[i]=T->child[d+i];
							T->child[d+i]=NULL;
						}
						T=k;
						return k;
					}
				}
			}
		}
		else return NULL;
	}
	else
	{
		btnode *n;
		n=new btnode;
		n->key[0]=data;
		return n;
	}
}

int height(btnode *T)
{
	if (T==NULL) return -1;
	return (height(T->child[0])+1);
}

void levelprint(btnode *T, int &h)
{
	if (T!=NULL)
	{
		for (int i=0;i<2*d+1;i++)
		{
			levelprint(T->child[i],h);
			int h2=height(T);
			if (h2==h&&T->key[i]!=-1&&i<2*d)
			cout<<T->key[i]<<" ";
		}
	}
}

void print(btnode *T)
{
	if (T!=NULL)
	{
		for (int i=0;i<2*d+1;i++)
		print(T->child[i]);
		for (int j=0;j<2*d;j++)
		{
			if (T->key[j]!=-1)
			cout<<T->key[j]<<" ";
		}
		cout<<endl;
	}
}

int main()
{
	btnode *b;
	int data;
	cout<<"Enter data:";
	cin>>data;
	b=new btnode;
	b->key[0]=data;
	cout<<"Enter data:";
	cin>>data;
	do{
		create(b,data);
		cout<<"Enter data:";
		cin>>data;
	} while (data!=-1);
	
	cout<<"Level Printing\n";
	int k=height(b);
	for (int i=k;i>=0;i--)
	{
	 	levelprint(b,i);
		cout<<endl;
	}
	
	cout<<"Postorder printing\n";
	print(b);
	
	return 0;
}