#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <stdlib.h>
#define limit 5
#define threshold 1
using namespace std;

struct fptree;
typedef map <vector <int>, int> mapStruct;
typedef pair <vector <int>, int> pairStruct;

struct fplist{
	fptree *child;
	fplist *next;
	fplist(){
		child=NULL;
		next=NULL;
	}
};

struct fptree
{
	int index;
	int count;
	fplist *list;
	fptree *nextSameNode;
	fptree()
	{
		index = -1;
		count = 0;
		list=NULL;
		nextSameNode=NULL;
	}
};

struct conditionalPB{
	int array[limit];
	int count;
	conditionalPB *next;
	conditionalPB()
	{
		next = NULL;
		count = 0;
		for (int i=0;i<limit;i++)
			array[i] = 0;
	}
};

struct lnode
{
	bool array[limit];
	lnode *next;
	fptree *first_item;
	conditionalPB *condPB;
	fptree *conditionalFPTree;
	int count;
	lnode()
	{
		first_item = NULL;
		for (int i=0;i<limit;i++)
			array[i]=false;
		next=NULL;
		count=0;
		condPB = NULL;
		conditionalFPTree = NULL;
	}
}; 

ofstream fout("apriori_output");
char D[15];
fptree *tree=new fptree;

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

void print(lnode *temp)
{
	while (temp)
	{
		for (int i=0;i<limit;i++) cout<<temp->array[i]<<"\t";
		cout<<"\t"<<temp->count<<endl;
		temp=temp->next;
	}
}

void desc_order(lnode *&l)
{
	for (lnode *temp=l;temp->next;temp=temp->next)
	{	
		for (lnode *temp2=temp->next;temp2->next;temp2=temp2->next)
		{
			if (temp->count<temp2->count)
			{
				if (temp==l)
					l=temp2;
				else
				{
					lnode *temp5=l;
					while (temp5->next!=temp) temp5=temp5->next;
					temp5->next = temp2;
				}
				lnode *temp3=temp;
				while (temp3->next!=temp2) temp3=temp3->next;
				temp3->next = temp;
				lnode *temp4 = temp->next;
				temp->next = temp2->next;
				temp2->next = temp4;
				temp3 = temp;
				temp = temp2;
				temp2 = temp3;
			}
		}	
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

lnode * sortAccordingly(lnode *l, lnode *temp)
{
	lnode *newNode,*temp2;
	newNode = new lnode;
	temp2=newNode;
	while (l)
	{
		lnode *temp3;
		temp3 = temp;
		while (temp3)
		{
			for (int i=0;i<limit;i++)
			{
				if (l->array[i]==true && temp3->array[i]==true)
				{
					temp2->array[i]=true;
					temp2->next=new lnode;
					temp2=temp2->next;
					break;
				}
			}
			temp3=temp3->next;
		}
		l=l->next;
	}
	return newNode;
}

void addToTree(fptree *t,lnode *l)//,lnode *lst)
{
	if (!l) return;
	fptree *nextTree;
	if (t->list==NULL)
	{
		t->list=new fplist;
		t->list->child = new fptree;
		nextTree = t->list->child;
		t->list->child->count=1;
		for (int i=0;i<limit;i++)
		{
			if (l->array[i])
			{
				t->list->child->index = i+1;
				break;
			}
		}
	}
	else
	{
		fplist *temp;
		temp = t->list;
		int ind;
		bool notfound=false;
		for (int i=0;i<limit;i++)
		{
			if (l->array[i])
			{
				ind=i+1;
				break;
			}
		}
		while (temp)
		{
			if (temp->child->index == ind)
			{
				temp->child->count++;
				nextTree = temp->child;
				break;
			}
			else temp=temp->next;
		}
		if (temp==NULL)
		{
			temp = t->list;
			while (temp->next) temp=temp->next;
			temp->next = new fplist;
			temp=temp->next;
			temp->child = new fptree;
			temp->child->count = 1;
			temp->child->index = ind;
			nextTree = temp->child;
		}
	}

	//don't worry much about the code from 
	//here

	// lnode *tmp=lst;
	// while (!tmp->array[nextTree->index-1]) tmp=tmp->next;
	// if (!tmp->first_item) tmp->first_item = nextTree;
	// else
	// {
	// 	//error hhere ... thik karlena
	// 	fptree *temporary = tmp->first_item;
	// 	// if (!temporary->nextSameNode) cout<<temporary->index<<" ";
	// 	while (temporary->nextSameNode) temporary=temporary->nextSameNode;
	// 	// temporary->nextSameNode = nextTree;
	// }

	//to here

	addToTree(nextTree,l->next);//,lst);
}

void create_tree (lnode *l)
{
	ifstream fin(D);
	char ch[5];
	bool count=true;
	while (fin.getline(ch,5,' '))
	{
		lnode *temp, *temp2;
		temp = new lnode;
		temp2 = temp;
		char *chh;
		chh=(char*)malloc(sizeof(char)*100);
		fin.getline(chh,100,'\n');
		while (*chh!='\0')
		{
			int x = atoi(chh);
			temp2->array[x-1] = true;
			temp2->next = new lnode;
			temp2 = temp2->next;
			while (*chh!=' ' && *chh!='\0') chh++;
			chh++;
		}
		lnode *temp3=sortAccordingly(l,temp);
		temp2 = temp3;
		while (temp2->next->next!=NULL) temp2=temp2->next;
		temp2->next=NULL;
		addToTree(tree,temp3);//,l);
	}

	fin.close();
}

void print_tree(fptree *t, int level)
{
	if (!t) return;
	cout<<"Level:"<<level<<" Item:"<<t->index<<" Count:"<<t->count<<endl;
	if (t->list)
	{
		fplist *temp = t->list;
		while (temp)
		{
			print_tree(temp -> child,level+1);
			temp = temp->next;
		}
	}
}

void traverse (lnode *l, fptree *t, int arr[limit], int searchItem)
{
	if (!t) return;
	if (t->index != searchItem)
	{
		fplist *temp = t->list;
		int newArr[limit] = {0};
		if (t->index != -1) 
		{
			int i;
			for (i=0;i<limit;i++)
			{
				if (arr[i]==0) break;
				newArr[i]=arr[i];
			}
			newArr[i] = t->index;
		}
		while(temp)
		{
			traverse(l,temp->child,newArr,searchItem);
			temp = temp->next;
		}
	}
	else
	{
		if (arr[0]!=0)
		{
			if (!l->condPB) l->condPB = new conditionalPB;
			conditionalPB *temp = l->condPB;
			while (temp->next) temp = temp->next;
			temp->next = new conditionalPB;
			temp = temp->next;
			temp->count = t->count;
			for (int i=0;i<limit;i++) temp->array[i] = arr[i]; 
		}
	}
}

void createConditionalPatternBase(lnode *l)
{
	if (!l) return;
	createConditionalPatternBase(l->next);
	int item;
	for (int i=0;i<limit;i++)
	{
		if (l->array[i])
		{
			item = i+1;
			break;
		}
	}
	int arr[limit] = {0};
	traverse(l, tree, arr, item);
	if (l->condPB && l->condPB->count == 0) l->condPB = l->condPB->next;
}

void createConditionalFPTree(lnode *l)
{
	if (!l) return;
	createConditionalFPTree(l->next);
	conditionalPB *temp = l->condPB;
	while (temp)
	{
		if (temp->count >= threshold)
		{
			if (!l->conditionalFPTree) l->conditionalFPTree = new fptree;
			for (int i=0;i<temp->count;i++)
			{
				lnode *newNode, *tmpNode;
				newNode = new lnode;
				tmpNode = newNode;
				for (int j=0;temp->array[j]!=0;j++)
				{
					tmpNode -> next = new lnode;
					tmpNode = tmpNode -> next;
					tmpNode -> array[temp->array[j]-1] = true;
				}
				newNode = newNode->next;
				addToTree (l->conditionalFPTree , newNode);
			}
		}
		temp = temp->next;
	}
}

void traverse_and_frequent_itemset_generation(fptree *t, int arr[limit])
{
	if (!t) return;
	if (t->count == 0 && t->list) 
	{
		fplist *tempList = t->list;
		while (tempList) 
		{
			traverse_and_frequent_itemset_generation(tempList->child,arr);
			tempList = tempList -> next;
		}
	}

}

void frequent_items_generation(lnode *l)
{
	if (!l) return;
	frequent_items_generation(l->next);
	int array[limit]={0};
	traverse_and_frequent_itemset_generation(l->conditionalFPTree,array);
}

int main()
{
	lnode *L;
	strcpy(D,"trans.txt");

	L=new lnode;

	find_frequent_one_itemsets(L);
	prune (L);
	desc_order (L);
	cout<<"List ordering is\n";
	print(L);
	cout<<endl<<endl;
	create_tree (L);
	cout<<"Tree Elements are:\n";
	print_tree(tree,0);
	endl(cout);

	createConditionalPatternBase(L);

	// cout<<L->next->next->next->next->condPB->next->next->next->array[0];

	createConditionalFPTree(L);

	lnode *printTemp = L;

	for (int i=0;i<limit;i++)
	{
		int j;
		for (j=0;j<limit;j++) if (printTemp->array[j]) break;
		cout<<"Conditional FP Tree of "<<j+1<<": \n";
		print_tree(printTemp->conditionalFPTree, 0);
		printTemp = printTemp -> next;
		endl(cout);
	}

	frequent_items_generation(L);//this function yet to be completed

	fout.close();

	endl(cout);
	return 0;
}