#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#define limit 5
#define threshold 5
using namespace std;

struct lnode
{
	bool array[limit];
	lnode *next;
	int count;
	lnode()
	{
		for (int i=0;i<limit;i++)
			array[i]=false;
		next=NULL;
		count=0;
	}
}; 

ofstream fout("apriori_output");
char D[15];

void find_frequent_one_itemsets(lnode *l)
{
	ifstream fin(D);
	char ch[5];

	lnode *temp;
	temp=l;
	temp->array[0]=true;
	for (int i=1;i<limit;i++)
	{
		temp->next=new lnode;
		temp=temp->next;
		temp->array[i]=true;
	}

	while (fin>>ch)
	{
		temp=l;
		if (ch[0]=='T') continue;
		int item=atoi(ch)-1;
		for (int i=0;i<item;i++) temp=temp->next;
		temp->count++;
	}

	fin.close();
}

bool is_subset(bool *subset,bool *superset)
{
	for (int i=0;i<limit;i++)
	{
		if (subset[i] && !superset[i]) return false;
	}
	return true;
}

void find_count(lnode *l)
{
	ifstream fin(D);
	char ch[5];

	fin>>ch;
	while (ch[0]=='T')
	{
		bool new_array[limit]={false};
		while (fin>>ch && ch[0]!='T')
		{
			int item=atoi(ch)-1;
			new_array[item]=true;
		}
		if (is_subset(l->array,new_array))
			l->count++;
	}

	fin.close();
}

bool match_check(lnode *l1,lnode *l2)
{
	int mismatch=0;
	for (int i=0;i<limit;i++)
	{
		if (l1->array[i]!=l2->array[i])
			mismatch++;
		if (mismatch>2) return false;
	}
	if (mismatch==2)
	return true;
	else return false;
}

bool complete_match(bool *arr1,bool *arr2)
{
	for (int i=0;i<limit;i++)
	{
		if (arr1[i]!=arr2[i]) return false;
	}
	return true;
}

bool has_infrequent_subset(lnode *c,lnode *l,int it_count)
{
	for (int i=0;i<limit;i++)
	{
		if (c->array[i]==false) continue;
		bool new_array[limit];
		for (int j=0;j<limit;j++)
			new_array[j]=c->array[j];
		new_array[i]=false;

		lnode *temp=l;
		while (temp)
		{
			if (complete_match(temp->array,new_array))
				break;
			temp=temp->next;
		}
		if (temp==NULL) return true;
	}

	return false;
}

bool in_the_list(lnode *parent, lnode *l1, lnode *l2)
{
	bool new_array[limit]={false};
	for (int i=0;i<limit;i++)
	{
		if (l1->array[i]||l2->array[i])
			new_array[i]=true;
	}
	lnode *temp=parent;

	while (temp)
	{
		if (complete_match(temp->array,new_array))
			return true;
		temp=temp->next;
	}

	return false;
}

lnode * apriori_gen (lnode *l,int it_count)
{
	lnode *t;
	lnode *c;
	c=NULL;
	t=NULL;
	lnode *temp1=l;
	while (temp1)
	{
		lnode *temp2=temp1->next;
		while (temp2)
		{
			if (temp1!=temp2)
			{
				if (match_check(temp1,temp2) && !in_the_list(c,temp1,temp2))
				{
					if (t==NULL)
					{
						t=new lnode;
						c=t;
					}
					else
					{
						t->next=new lnode;
						t=t->next;
					}
					for (int i=0;i<limit;i++)
					{
						if (temp1->array[i]||temp2->array[i])
							t->array[i]=true;
					}
					if (has_infrequent_subset(t,l,it_count))
					{
						lnode *another_temp=c;
						if (!another_temp->next){ c=NULL; t=NULL; }
						else
						{
							while (another_temp->next->next) another_temp=another_temp->next;
							another_temp->next=NULL;
						}
					}
				}
			}
			temp2=temp2->next;
		}
		temp1=temp1->next;
	}
	return c;
}

void print(lnode *temp)
{
	while (temp)
	{
		for (int i=0;i<limit;i++) fout<<temp->array[i]<<"\t";
		fout<<"\t"<<temp->count<<endl;
		temp=temp->next;
	}
}

void prune(lnode *&l)
{
	lnode *prev=NULL;
	lnode *temp=l;
	while (temp)
		{
			if (temp->count<threshold)
			{
				if (temp==l)
				{
					l=l->next;
					temp=l;
					prev=NULL;
				}
				else
				{
					prev->next=prev->next->next;
					temp=prev->next;
				}
			}
			else
			{
				prev=temp;
				temp=temp->next;
			}
		}
}

int main()
{
	lnode *L[limit];
	strcpy(D,"trans.txt");

	for (int i=0;i<limit;i++) L[i]=NULL;
	L[0]=new lnode;

	find_frequent_one_itemsets(L[0]);
	fout<<"Iteration 1"<<endl;
	fout<<"Before Pruning"<<endl;
	for (int i=0;i<limit;i++)
		fout<<"I"<<i<<"\t";
	fout<<"Frequency"<<endl;
	print(L[0]);
	prune(L[0]);
	fout<<endl<<"After Pruning"<<endl;
	for (int i=0;i<limit;i++)
		fout<<"I"<<i<<"\t";
	fout<<"Frequency"<<endl;
		print (L[0]);

	for (int k=1;L[k-1];k++)
	{
		L[k]=apriori_gen(L[k-1],k);
		lnode *temp=L[k];
		while (temp)
		{
			find_count(temp);
			temp=temp->next;
		}
		if (L[k])
		{
			fout<<endl<<endl<<"Iteration "<<k+1<<endl;
			fout<<"Before Pruning"<<endl;
			for (int i=0;i<limit;i++)
				fout<<"I"<<i<<"\t";
			fout<<"Frequency"<<endl;
			print (L[k]);
		}
		temp=L[k];
		prune(L[k]);
		
		if (L[k])
		{
			fout<<endl<<"After Pruning"<<endl;
			for (int i=0;i<limit;i++)
				fout<<"I"<<i<<"\t";
			fout<<"Frequency"<<endl;
			print (L[k]);
		}
	}

	fout.close();

	return 0;
}