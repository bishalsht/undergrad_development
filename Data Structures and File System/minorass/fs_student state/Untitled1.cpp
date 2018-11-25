#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

ofstream fout;
	
struct Linknode {
	int index;
	Linknode *next;
};

void add_index(Linknode *&l,int i)
{
	if (l!=NULL)
	add_index(l->next,i);
	else
	{
		l=new Linknode;
		l->index=i;
		l->next=NULL;
	}
}

void print_index(Linknode *l)
{
	if (l!=NULL)
	{
		fout<<l->index<<" ";
		print_index(l->next);
	}
}

class lnode {
	char state[3];
	Linknode *student;
	lnode *next;
	public:
		lnode ()
		{
			for (int i=0;i<3;i++)
			state[i]='\0';
		}
		
		void add_data(lnode *&l, char a[], int i)
		{
			if (l==NULL)
			{
				l=new lnode;
				for (int i=0;i<2;i++)
				l->state[i]=a[i];
				l->student=new Linknode;
				l->student->index=i;
				l->student->next=NULL;
				l->next=NULL;
			}
			else
			{
				if (l->state[0]==a[0]&&l->state[1]==a[1])
				{
					add_index(l->student, i);
				}
				else
				add_data(l->next,a,i);
			}
		}
		
		void insertion_sort(lnode *&l, lnode *b)
		{
			if (b!=NULL)
			{
				lnode *n;
				n=l;
				while (n!=b)
				{
					if (n->state[0]>b->state[0]||(n->state[0]==b->state[0]&&n->state[1]>b->state[1]))
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
				insertion_sort(l,b->next);
			}
		}
		
		lnode* next_node(lnode *l)
		{
			return l->next;
		}
		
		void print(lnode *l)
		{
			if (l!=NULL)
			{
				fout<<l->state<<" ";
				print_index (l->student);
				print (l->next);
			}
		}
};

int main()
{
	lnode *l;
	l=NULL;
	ifstream fin;
	int j=0;
	fin.open("std.txt");
	while (!fin.eof())
	{
		char z;
		if (fin>>z)
		{
			long pos=fin.tellg();
			fin.seekg(pos-1);
			char a[3]={'\0'};
			pos=fin.tellg();
			fin.seekg(pos+16);
			fin>>a[0]>>a[1];
			l->add_data(l,a,j);
			j++;
		}
	}
	fin.close();
	
	l->insertion_sort(l,l->next_node(l));
	
	fout.open("index.txt");
	l->print(l);
	fout.close();
	
	cout<<"End of program";
	
	return 0;
}
