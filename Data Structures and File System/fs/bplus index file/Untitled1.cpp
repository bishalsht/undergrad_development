#include <iostream>
#include <fstream>
using namespace std;

fstream file;
#define d 1
int z=1;

struct keys{
	int key;
	int key_index;
	keys()
	{
		key=-1;
		key_index=-1;
	}
};

keys x;

bool operator == (keys m,keys n)
{
	if (m.key==n.key&&m.key_index==n.key_index)
	return true;
	return false;
}

bool operator < (keys m,keys n)
{
	if (m.key<n.key)
	return true;
	return false;
}

bool operator > (keys m,keys n)
{
	if (m.key>n.key)
	return true;
	return false;
}

bool operator != (keys m, keys n)
{
	if (m.key==n.key)
	return false;
	return true;
}

struct btnode{
	keys key[2*d];
	btnode *child[2*d+1];
	int count,index;
	btnode(){
		for (int i=0;i<2*d;i++)
		{
			key[i]=x;
			child[i]=NULL;
		}
		count=1;
		index=-1;
		child[2*d]=NULL;
	}
};

int array_sort(keys *&a,int size)
{
	keys m=a[size-1];
	for (int i=0;i<size;i++)
	{
		for (int j=0;j<size-1;j++)
		{
			if (a[j]>a[j+1])
			{
				keys temp;
				temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
		}
	}
	for (int i=0;i<size;i++)
	{
		if (a[i]==m)
		return i;
	}
}

btnode* create(btnode *&T, keys data)
{
	if (T!=NULL)
	{
		int i;
		btnode *temp;
		for (i=0;i<2*d&&T->key[i]<data&&T->key[i]!=x;i++);
		temp=create(T->child[i],data);
		if (temp!=NULL)
		{
			if (T->count<2*d)
			{
				keys dt[T->count+1];
				for (i=0;i<T->count;i++)
				dt[i]=T->key[i];
				dt[T->count]=temp->key[0];
				keys *m;
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
				keys dt[T->count+1];
				for (int i=0;i<T->count;i++)
				dt[i]=T->key[i];
				dt[T->count]=temp->key[0];
				keys *m;
				m=&dt[0];
				int s=array_sort(m,2*d+1);
				btnode *k;
				k=new btnode;
				k->count=1;
				k->key[0]=dt[d];
				k->child[0]=T;
				T->count=d;
				k->child[1]=new btnode;
				if (T->child[0]==NULL)
				k->child[1]->count=d+1;
				else k->child[1]->count=d;
				k->child[1]->key[0]=k->key[0];
				if (temp->child[0]==NULL)
				{
					T->child[2*d]=k->child[1];
					for (i=0;i<d;i++)
					T->key[i]=dt[i];
					for (i=1;i<d+1;i++)
					{
						k->child[1]->key[i]=dt[d+i];
						T->key[i+d-1]=x;
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
					    	T->key[i+d]=x;
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
							T->key[i+d]=x;
						}
						T=k;
						return k;
					}
					else if (dt[d]==T->key[d])
					{
						for (i=0;i<d;i++)
						{
							k->child[1]->key[i]=dt[i+d+1];
							T->key[i+d]=x;
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

void node_index(btnode *T, int &h)
{
	if (T!=NULL)
	{
		for (int i=0;i<2*d+1;i++)
		{
			node_index(T->child[i],h);
			int h2=height(T);
			if (h2==h&&T->index==-1)
			T->index=z++;
		}
	}
}

void print(btnode *T, int h)
{
	if (T!=NULL)
	{
		for (int i=0;i<2*d+1;i++)
		{
			int h2=height(T);
			if (h2!=h&&T->child[0]!=NULL)
		 	print(T->child[i],h);
			if (h2==h)
		 	{
				if (T->index<10) file<<'0';
				file<<T->index;
				if (T->child[0])
				{
					if (T->child[0]->index<10) file<<'0';
					file<<T->child[0]->index;
				}
				else file<<"-1";
				if (T->key[0].key<10&&T->key[0].key!=-1) file<<'0';
				file<<T->key[0].key;
				if (T->key[0].key_index<10&&T->key[0].key_index!=-1) file<<'0';
				file<<T->key[0].key_index;
				if (T->child[1])
				{
					if (T->child[1]->index<10) file<<'0';
					file<<T->child[1]->index;
				}
				else file<<"-1";
				if (T->key[1].key<10&&T->key[1].key!=-1) file<<'0';
				file<<T->key[1].key;
				if (T->key[1].key_index<10&&T->key[1].key_index!=-1) file<<'0';
				file<<T->key[1].key_index;
				if (T->child[2])
				{
					if (T->child[2]->index<10) file<<'0';
					file<<T->child[2]->index;
				}
				else file<<"-1";
				break;
			}
		}
	}
}

int integer(char a[])
{
	if (a[0]=='-')
	return -1;
	return ((static_cast<int>(a[0])-48)*10+(static_cast<int>(a[1])-48));
}

long root_search()
{
	file.seekg(ios::beg);
	char b[3]={'\0'};
	long pos;
	while (!file.eof())
	{
		if (file>>b[0])
		{
			file>>b[1];
			int t=integer(b);
			if (t==0)
			{
				pos=file.tellg();
				file.seekg(pos-2);
				pos=file.tellg();
				return pos;
			}
			else
			{
				pos=file.tellg();
				file.seekg(pos+14);
			}
		}
	}
	return -1;
}

int leaf_search(long pos, int m)
{
	file.seekg(pos);
	char b[3]={'\0'},c[3]={'\0'};int l;
	while (!file.eof())
	{
		file.seekg(pos+2);
		file>>b[0]>>b[1]>>c[0]>>c[1];;
		int n=integer(b);
		int k=integer(c);
		
		if (k==m)
		{
			file>>b[0]>>b[1];
			k=integer(b);
			return k;
		}
		else if (k>m&&n!=-1) pos=n*16;
		else if (k>m&&n==-1) return -1;
		else
		{
			long pos2;
			pos2=file.tellg();
			file.seekg(pos2+2);
			file>>b[0]>>b[1]>>c[0]>>c[1];
			k=integer(c);
			if (k==m)
			{
				file>>b[0]>>b[1];
				k=integer(b);
				return k;
			}
			else if (n==-1) return -1;
			else if (k==-1||k>m)
			{
				n=integer(b);
				pos=n*16;					
			}
			else
			{
				file>>b[0]>>b[0]>>b[0]>>b[1];
				n=integer(b);
				pos=n*16;
			}
		}
	}
	return -1;
}

int main()
{
	ifstream fin("data.txt");
	ofstream fout("index.txt");
	fout.close();
	file.open("index.txt", ios::in|ios::out);
	btnode *b;
	keys data;
	fin>>data.key;
	data.key_index=z++;
	b=new btnode;
	b->key[0]=data;
	while (fin>>data.key)
	{
		data.key_index=z++;
		create(b,data);
	}
	
	int k=height(b);
	z=0;
	for (int i=k;i>=0;i--)
	node_index(b,i);
	for (int m=k;m>=0;m--)
	print(b,m);
	
	int dat;
	cout<<"Enter the data you want to search: ";
	cin>>dat;

	long root_pos=root_search();
	file.seekp(root_pos);
	int leaf_pos=leaf_search(root_pos, dat);
	if (leaf_pos!=-1)
	cout<<"Data is at "<<leaf_pos<<"th position.";
	else cout<<"Data is not available.";
	
	file.close();
	fin.close();
	return 0;
}
