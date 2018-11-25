#include <iostream>
#include <fstream>
using namespace std;

int index1=0;

struct trie;

struct alpha{
	char ch;
	trie *next;
};

struct non_leaf{
	trie *empty;
	alpha a[26];
};

struct un{
	int fptr;
	non_leaf data;
};

struct trie{
	int tag;
	int ind;
	un dt;
};

void init_nonleaf(trie *&T)
{
	T->tag=1;
	T->dt.data.empty=NULL;
	for (int i=0;i<26;i++)
	{
		T->dt.data.a[i].ch='a'+i;
		T->dt.data.a[i].next=NULL;
	}
}

void init_leaf(trie *&T)
{
	T->tag=0;
	T->dt.fptr=-1;
}

void print(trie *T)
{
	if (T!=NULL)
	{
		T->ind=index1++;
		if (T->tag==1)
		{
			for (int i=0;i<26;i++)
			print(T->dt.data.a[i].next);
			print (T->dt.data.empty);
		}
	}
}

void init(char word[])
{
	for (int i=0;i<10;i++)
	word[i]='\0';
}

void split(trie *&T, char word[], int x, int index)
{
	int orig=T->dt.data.a[x].next->dt.fptr;
	delete(T->dt.data.a[x].next);
	T->dt.data.a[x].next=NULL;
	ifstream in("input.txt");
	char word2[10];
	init(word2);
	for (int p=0;p<=orig;p++)
	{
		in>>word2;
		if (p==orig) break;
		init(word2);
	}
	T->dt.data.a[x].next=new trie;
	init_nonleaf(T->dt.data.a[x].next);
	trie *temp=T;
	while (word2[x]==word[x])//&&(word[x]!='\0'||word2[x]!='\0'))
	{
		int path=word[x]-'a';
		temp->dt.data.a[path].next=new trie;
		temp=temp->dt.data.a[path].next;
		init_nonleaf(temp);
		x++;
	}
	if (word[x]=='\0')
	{
		temp->dt.data.empty=new trie;
		init_leaf(temp->dt.data.empty);
		temp->dt.data.empty->dt.fptr=index;
		int path=word2[x]-'a';
		temp->dt.data.a[path].next=new trie;
		temp=temp->dt.data.a[path].next;
		init_leaf(temp);
		temp->dt.fptr=orig;
	}
	else if (word2[x]=='\0')
	{
		temp->dt.data.empty=new trie;
		init_leaf(temp->dt.data.empty);
		temp->dt.data.empty->dt.fptr=orig;
		int path=word[x]-'a';
		temp->dt.data.a[path].next=new trie;
		temp=temp->dt.data.a[path].next;
		init_leaf(temp);
		temp->dt.fptr=index;
	}
	else
	{
		int path=word[x]-'a';
		temp->dt.data.a[path].next=new trie;
		init_leaf(temp->dt.data.a[path].next);
		temp->dt.data.a[path].next->dt.fptr=index;
		path=word2[x]-'a';
		temp->dt.data.a[path].next=new trie;
		temp=temp->dt.data.a[path].next;
		init_leaf(temp);
		temp->dt.fptr=orig;
	}
}

void fileprint(trie *T,ofstream &fout)
{
	if (T!=NULL)
	{
		if (T->tag==1)
		{
			fout<<1;
			for (int i=0;i<26;i++)
			{
				fout<<T->dt.data.a[i].ch;
				if (T->dt.data.a[i].next==NULL) fout<<"-1";
				else if (T->dt.data.a[i].next->ind>9)
				fout<<T->dt.data.a[i].next->ind;
				else fout<<"0"<<T->dt.data.a[i].next->ind;
			}
			if (T->dt.data.empty==NULL) fout<<"-1";
			else if (T->dt.data.empty->ind<10) fout<<"0"<<T->dt.data.empty->ind;
			else fout<<T->dt.data.empty->ind;
			fout<<endl;
			for (int i=0;i<26;i++)
			fileprint(T->dt.data.a[i].next,fout);
			fileprint (T->dt.data.empty,fout);
		}
		if (T->tag==0)
		{
			fout<<0;
			if (T->dt.fptr<10) fout<<"0";
			fout<<T->dt.fptr<<endl;
		}
	}
}

int main()
{
	trie *T;
	int index=0;
	T=new trie;
	init_nonleaf(T);
	char word[10]={'\0'};
	ifstream fin("input.txt");
	
	while (fin>>word)
	{
		trie *temp;
		temp=T;
		for (int i=0;word[i]!='\0';i++)
		{
			int x=word[i]-'a';
			if (temp->dt.data.a[x].next==NULL)
			{
				temp->dt.data.a[x].next=new trie;
				trie *temp2=temp->dt.data.a[x].next;
				init_leaf(temp2);
				temp2->dt.fptr=index++;
				break;
			}
			else if (temp->dt.data.a[x].next->tag==1)
			temp=temp->dt.data.a[x].next;
			else if (temp->dt.data.a[x].next->tag==0)
			{
				split(temp,word,x,index);
				index++;
				break;
			}
		}
		init(word);
	}
	
	ofstream fout("index.txt");
	print (T);
	fileprint(T,fout);
	fout.close();
	
	fin.close();
	return 0;
}
