#include <iostream>
using namespace std;

//bool add=false;

//B-2 tree node
struct btnode{
	int count;
	int key[4];
	btnode *child[5];
	btnode(){
		count=0;
	}
};

void print(btnode *T)
{
	if (T!=NULL)
	{
		for (int i=0;i<4;i++)
		{
			print (T->child[i]);
			if (T->key[i]!=-1)
			cout<<T->key[i]<<" ";
		}
		print (T->child[4]);
	}
}

btnode* btnode_create(btnode *&T, int key)
{
	if (T!=NULL)
	{
		btnode *m;
		if (T->key[0]>key)
		m=btnode_create(T->child[0],key);
		else if (T->key[1]>key||T->count==1)
		m=btnode_create(T->child[1],key);
		else if (T->key[2]>key||T->count==2)
		m=btnode_create(T->child[2],key);
		else if (T->key[3]>key||T->count==3)
		m=btnode_create(T->child[3],key);
		else if (T->key[3]<key)
		m=btnode_create(T->child[4],key);
		
		if (m)
		{
			if (T->count==1)
			{
				T->count++;
				if (T->key[0]<m->key[0])
				{
					T->key[1]=m->key[0];
					T->child[1]=m->child[0];
					T->child[2]=m->child[1];
					return NULL;
				}
				else 
				{
					T->key[1]=T->key[0];
					T->key[0]=m->key[0];
					T->child[2]=T->child[1];
					T->child[0]=m->child[0];
					T->child[1]=m->child[1];
					return NULL;
				}
			}
			else if (T->count==2)
			{
				T->count++;
				if (T->key[1]<m->key[0])
				{
					T->key[2]=m->key[0];
					T->child[2]=m->child[0];
					T->child[3]=m->child[1];
					return NULL;
				}
				else if (T->key[0]<m->key[0])
				{
					T->key[2]=T->key[1];
					T->key[1]=m->key[0];
					T->child[3]=T->child[2];
					T->child[1]=m->child[0];
					T->child[2]=m->child[1];
					return NULL;
				}
				else
				{
					T->key[2]=T->key[1];
					T->key[1]=T->key[0];
					T->key[0]=m->key[0];
					T->child[3]=T->child[2];
					T->child[2]=T->child[1];
					T->child[1]=m->child[1];
					T->child[0]=m->child[0];
					return NULL;
				}
			}
			else if (T->count==3)
			{
				T->count++;
				if (T->key[2]<m->key[0])
				{
					T->key[3]=m->key[0];
					T->child[4]=m->child[1];
					T->child[3]=m->child[0];
					return NULL;
				}
				else if (T->key[1]<m->key[0])
				{
					T->key[3]=T->key[2];
					T->key[2]=m->key[0];
					T->child[4]=T->child[3];
					T->child[3]=m->child[1];
					T->child[2]=m->child[0];
					return NULL;
				}
				else if (T->key[0]<m->key[0])
				{
					T->key[3]=T->key[2];
					T->key[2]=T->key[1];
					T->key[1]=m->key[0];
					T->child[4]=T->child[3];
					T->child[3]=T->child[2];
					T->child[2]=m->child[1];
					T->child[1]=m->child[0];
					return NULL;
				}
				else
				{
					T->key[3]=T->key[2];
					T->key[2]=T->key[1];
					T->key[1]=T->key[0];
					T->key[0]=m->key[0];
					T->child[4]=T->child[3];
					T->child[3]=T->child[2];
					T->child[2]=T->child[1];
					T->child[1]=m->child[1];
					T->child[0]=m->child[0];
					return NULL;
				}
			}
			else  if (T->count==4)
			{
				btnode *n;
				n=new btnode;
				n->count=1;
				for (int i=0;i<4;i++)
				{
					n->child[i]=NULL;
					n->key[i]=-1;
				}
				n->child[4]=NULL;
				if (T->child[0]==NULL)
				{
					if (T->key[1]>m->key[0])
					{
						n->key[0]=T->key[1];
						n->child[1]=new btnode;
						for (int j=0;j<4;j++)
						{
							n->child[1]->child[j]=NULL;
							n->child[1]->key[j]=-1;
						}
						n->child[1]->child[4]=NULL;
						n->child[1]->key[0]=T->key[2];
						n->child[1]->key[1]=T->key[3];
						if (T->key[0]>m->key[0])
						{
							T->key[1]=T->key[0];
							T->key[0]=m->key[0];
						}
						else
						T->key[1]=m->key[0];
					}
					else
					{
						n->child[1]=new btnode;
						for (int j=0;j<4;j++)
						{
							n->child[1]->child[j]=NULL;
							n->child[1]->key[j]=-1;
						}
						n->child[1]->child[4]=NULL;
						if (T->key[2]<m->key[0])
						{
							n->key[0]=T->key[2];
							if (T->key[3]>m->key[0])
							{
								n->child[1]->key[0]=m->key[0];
								n->child[1]->key[1]=T->key[3];
							}
							else
							{
								n->child[1]->key[1]=m->key[0];
								n->child[1]->key[0]=T->key[3];
							}
						}
						else
						{
							n->key[0]=m->key[0];
							n->child[1]->key[0]=T->key[2];
							n->child[1]->key[1]=T->key[3];
						}
					}
					n->child[0]=T;
					n->child[1]->count=2;
					T->count=2;
					T->key[3]=-1;
					T->key[2]=-1;
					T=n;
					return n;
				}
				else
				{
					int k;
					for (k=0;T->child[k]->key[0]!=m->key[0];k++);
					n->child[0]=T;
					n->child[1]=new btnode;
					for (int j=0;j<4;j++)
					{
						n->child[1]->child[j]=NULL;
						n->child[1]->key[j]=-1;
					}
					n->child[1]->child[4]=NULL;
					if (k==2)
					{
						n->key[0]=m->key[0];
						n->child[1]->key[0]=T->key[2];
						n->child[1]->key[1]=T->key[3];
						n->child[1]->child[0]=m->child[1];
						n->child[1]->child[1]=T->child[3];
						n->child[1]->child[2]=T->child[4];
						T->child[2]=m->child[0];
					}
					else if (k<2)
					{
						n->key[0]=T->key[1];
						if (k==0)
						T->key[1]=T->key[0];
						T->key[k]=m->key[0];
						n->child[1]->key[0]=T->key[2];
						n->child[1]->key[1]=T->key[3];
						n->child[1]->child[0]=T->child[2];
						if (k==0)
						T->child[2]=T->child[1];
						n->child[1]->child[1]=T->child[3];
						n->child[1]->child[2]=T->child[4];
						T->child[k+1]=m->child[1];
						T->child[k]=m->child[0];
					}
					else if (k>2)
					{
						n->key[0]=T->key[2];
						n->child[1]->key[0]=m->key[0];
						n->child[1]->key[1]=T->key[3];
						if (k==4)
						{
							n->child[1]->key[1]=m->key[0];
							n->child[1]->key[0]=T->key[3];
						}
						n->child[1]->child[0]=m->child[0];
						n->child[1]->child[1]=m->child[1];
						n->child[1]->child[2]=T->child[4];
						if (k==4)
						{
							n->child[1]->child[1]=m->child[0];
							n->child[1]->child[2]=m->child[1];
							n->child[1]->child[0]=T->child[3];
						}
					}
					T->key[2]=-1;
					T->key[3]=-1;
					T->child[3]=NULL;
					T->child[4]=NULL;
					T=n;
				}
			}
		}
		else return NULL;
	}
	else
	{
		btnode *b;
		b=new btnode;
		for (int i=1;i<4;i++)
		b->key[i]=-1;
		b->key[0]=key;
		for (int i=0;i<5;i++)
		b->child[i]=NULL;
		b->count=1;
		return b;
	}
}

int main()
{
	btnode *T;
	int key;
	cout<<"Enter data:";
	cin>>key;
	
	//initialization
	T=new btnode;
	T->key[0]=key;
	for (int i=1;i<4;i++)
	T->key[i]=-1;
	for (int i=0;i<5;i++)
	T->child[i]=NULL;
	T->count=1;
	
	//additional data
	cout<<"Enter data:";
	cin>>key;
	do {
		btnode_create(T,key);
		cout<<"Enter data:";
		cin>>key;
//		add=false;
	}while (key!=-1);
	
	//print
	print(T);	
	return 0;
}
