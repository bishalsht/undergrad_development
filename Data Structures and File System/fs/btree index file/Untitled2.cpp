#include <iostream>
#include <fstream>
using namespace std;

#define d 1
fstream file;
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

struct btnod_ds{
	keys key[2*d];
	btnod_ds *child[2*d+1];
	btnod_ds *parent;
	int count, index;
	btnod_ds(){
		for (int i=0;i<2*d;i++)
		{
			child[i]=NULL;
		}
		count=1;
		child[2*d]=NULL;
		parent=NULL;
		index=-1;
	}
};

int self_index(btnod_ds *T)
{
	if (T->parent==NULL)
	return -1;
	keys t=T->key[0];
	T=T->parent;
	for (int i=0;i<2*d+1;i++)
	{
		if (T->child[i]->key[0]==t)
		return i;
	}
}

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

btnod_ds* create(btnod_ds *&T, keys data)
{
	if (T!=NULL)
	{
		int i;
		btnod_ds *temp;
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
				if (T->child[s]!=NULL)
				T->child[s]->parent=T;
				T->child[s+1]=temp->child[1];
				if (T->child[s+1]!=NULL)
				T->child[s+1]->parent=T;
				for (i=0;i<T->count;i++)
				T->key[i]=dt[i];
				return NULL;
			}
			else if (T->count==2*d)
			{
				int index=self_index(T);
				if (T->parent!=NULL&&index!=2*d&&T->parent->child[index+1]!=NULL&&T->parent->child[index+1]->count<2*d)
				{
					keys m[2*d+1];
					for (i=0;i<2*d;i++)
					m[i]=T->key[i];
					m[2*d]=temp->key[0];
					keys *q;
					q=&m[0];
					int s=array_sort(q,2*d+1);
					btnod_ds *temp2;
					temp2=T->parent->child[index+1];
					for (i=temp2->count;i>0;i--)
					{
						temp2->key[i]=temp2->key[i-1];
						temp2->child[i+1]=temp2->child[i];
					}
					temp2->child[1]=temp2->child[0];
					temp2->child[0]=T->child[2*d];
					if (temp2->child[0]!=NULL)
					temp2->child[0]->parent=temp2;
					temp2->key[0]=temp2->parent->key[index];
					temp2->parent->key[index]=m[2*d];
					temp2->count++;
					for (i=T->count-1;i>s;i--)
					{
						T->key[i]=T->key[i-1];
						T->child[i+1]=T->child[i];
					}
					if (s<2*d)
					{
						T->key[s]=m[s];
						T->child[s]=temp->child[0];
						T->child[s+1]=temp->child[1];
						if (T->child[s]!=NULL)
						T->child[s]->parent=T;
						if (T->child[s+1]!=NULL)
						T->child[s+1]->parent=T;
					}
					else
					{
						T->child[s]=temp->child[0];
						if (T->child[s]!=NULL)
						T->child[s]->parent=T;
						temp2->child[0]=temp->child[1];
						if (temp2->child[0]!=NULL)
						temp2->child[0]->parent=temp2;
					}
					return NULL;
				}
				else if (T->parent!=NULL&&index!=0&&T->parent->child[index-1]->count<2*d)
				{
					keys m[2*d+1];
					for (i=0;i<2*d;i++)
					m[i]=T->key[i];
					m[2*d]=temp->key[0];
					keys *q;
					q=&m[0];
					int s=array_sort(q,2*d+1);
					btnod_ds *temp2;
					temp2=T->parent->child[index-1];
					temp2->key[temp2->count]=temp2->parent->key[index-1];
					temp2->count++;
					temp2->child[temp2->count]=T->child[0];
					if (temp2->child[temp2->count]!=NULL)
					temp2->child[temp2->count]->parent=temp2;
					T->parent->key[index-1]=m[0];
					for (i=0;i<s;i++)
					{
						T->key[i]=m[i+1];
						T->child[i]=T->child[i+1];
					}
					if (i!=0)
					{
						T->child[i-1]=temp->child[0];
						T->child[i]=temp->child[1];
						if (T->child[i-1]!=NULL)
						T->child[i-1]->parent=T;
						if (T->child[i]!=NULL)
						T->child[i]->parent=T;
					}
					else
					{
						T->child[0]=temp->child[1];
						if (T->child[0]!=NULL)
						T->child[0]->parent=T;
						temp2->child[temp2->count]=temp->child[0];
						if (temp2->child[temp2->count]!=NULL)
						temp2->child[temp2->count]->parent=temp2;
					}
					return NULL;
				}
				else
				{
					keys dt[T->count+1];
					for (int i=0;i<T->count;i++)
					dt[i]=T->key[i];
					dt[T->count]=temp->key[0];
					keys *m;
					m=&dt[0];
					int s=array_sort(m,2*d+1);
					btnod_ds *k;
					k=new btnod_ds;
					k->count=1;
					k->key[0]=dt[d];
					k->child[0]=T;
					T->count=d;
					T->parent=k;
					k->child[1]=new btnod_ds;
					k->child[1]->parent=k;
					k->child[1]->count=d;
					if (temp->child[0]==NULL)
					{
						for (i=0;i<d;i++)
						T->key[i]=dt[i];
						for (i=0;i<d;i++)
						{
							k->child[1]->key[i]=dt[d+i+1];
							T->key[i+d]=x;
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
						    	k->child[1]->child[i]->parent=k->child[1];
						    }
						    k->child[1]->child[d]=T->child[2*d];
						    k->child[1]->child[d]->parent=k->child[1];
						    for (i=0;i<=d;i++)
						    T->child[i+d]=NULL;
						    for (i=d;i>s;i--)
						    T->child[i]=T->child[i-1];
						    T->child[s]=temp->child[0];
						    T->child[s]->parent=T;
						    T->child[s+1]=temp->child[1];
						    T->child[s+1]->parent=T;
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
								k->child[1]->child[i]->parent=k->child[1];
								k->child[1]->key[i-1]=T->key[d+i-1];
							}
							k->child[1]->child[0]=temp->child[1];
							k->child[1]->child[0]->parent=k->child[1];
							T->child[d]=temp->child[0];
							T->child[d]->parent=T;
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
								k->child[1]->child[i]->parent=k->child[1];
								T->child[d+i+1]=NULL;
							}
							T->child[d+i+1]=NULL;
							k->child[1]->child[i]=temp->child[0];
							k->child[1]->child[i]->parent=k->child[1];
							k->child[1]->child[i+1]=temp->child[1];
							k->child[1]->child[i+1]->parent=k->child[1];
							for (i=i+2;i<d+1;i++)
							{
								k->child[1]->child[i]=T->child[d+i];
								k->child[1]->child[i]->parent=k->child[1];
								T->child[d+i]=NULL;
							}
							T=k;
							return k;
						}
					}
				}
			}
		}
		else return NULL;
	}
	else
	{
		btnod_ds *n;
		n=new btnod_ds;
		n->key[0]=data;
		return n;
	}
}

int height(btnod_ds *T)
{
	if (T==NULL) return -1;
	return (height(T->child[0])+1);
}

void print(btnod_ds *T, int h)
{
	if (T!=NULL)
	{
		for (int i=0;i<2*d+1;i++)
		{
			int h2=height(T);
			if (h2!=h)
		 	print(T->child[i],h);
			if (h2==h)
		 	{
				if (T->index<10) file<<'0';
				file<<T->index;
				if (T->parent==NULL) file<<"-1";
				else
				{
					if (T->parent->index<10) file<<'0';
					file<<T->parent->index;
				}
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

void node_index(btnod_ds *T, int &h)
{
	if (T!=NULL)
	{
		for (int l=0;l<2*d+1;l++)
		{
			node_index(T->child[l],h);
			int h2=height(T);
			if (h2==h&&T->index==-1)
			T->index=z++;
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
			file>>b[0]>>b[0]>>b[1];
			int t=integer(b);
			if (t==-1)
			{
				pos=file.tellg();
				file.seekg(pos-4);
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
	char b[3]={'\0'},c[3]={'\0'};
	while (!file.eof())
	{
		file.seekg(pos+4);
		file>>b[0]>>b[1]>>c[0]>>c[1];;
		int n=integer(b);
		int k=integer(c);
		
		if (k==m)
		{
			file>>b[0]>>b[1];
			k=integer(b);
			return k;
		}
		else if (k>m&&n!=-1) pos=n*18;
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
				pos=n*18;					
			}
			else
			{
				file>>b[0]>>b[0]>>b[0]>>b[1];
				n=integer(b);
				pos=n*18;
			}
		}
	}
	return -1;
}

int main()
{
	btnod_ds *b;
	keys k;
	ifstream fin("data.txt");
	ofstream fout("index.txt");
	fout.close();
	file.open("index.txt", ios::in|ios::out);
	fin>>k.key;
	b=new btnod_ds;
	b->key[0]=k;
	b->key[0].key_index=z++;
	int dat;
	while (fin>>dat)
	{
		keys m;
		m.key=dat;
		m.key_index=z++;
		create(b,m);
	}
	
	int he=height(b);
	z=0;
	for (int m=he;m>=0;m--)
	node_index(b,m);
	
	for (int m=he;m>=0;m--)
	print(b,m);
	
	cout<<"Enter the data you want to search: ";
	cin>>dat;

	long root_pos=root_search();
	file.seekp(root_pos);
	int leaf_pos=leaf_search(root_pos, dat);
	if (leaf_pos!=-1)
	cout<<"Data is at "<<leaf_pos<<"th position.";
	else cout<<"Data is not available.";
	
	fin.close();
	file.close();
	return 0;
}
