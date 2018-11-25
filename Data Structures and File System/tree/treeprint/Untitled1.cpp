#include <iostream>
using namespace std;

struct btree{
	btree *lchild;
	char data;
	btree *rchild;
};

class queue{
	
	public:
		int size, front, rear, count;
		char element[50];
		queue(){
			rear=-1;
			front=-1;
			size=50;
			count=0;
		}
		void enqueue(int e){
			if (isqFull())
			   cout<<"Queue Overflow";
            else
            {
            	rear=(rear+1)%size;
            	element[rear]=e;
            	if (front==-1) front++;
            	count++;
            }
		}
		char delqueue(){
			if (isqEmpty())
			   cout<<"Queue Underflow";
			else{
				count--;
				char l=element[front];
				if (front==rear)
				{
					front=-1;
					rear=-1;
				}
				else
				front=(front+1)%size;
				return l;
			}
			return NULL;
		}
		bool isqFull(){
			if ((front==rear+1)||(front==0&&rear==size-1))
			return 1;
			return 0;
		}
		bool isqEmpty(){
			if (rear==-1)
			return 1;
			return 0;
		}
		char frontelement(){
			if (!isqEmpty())
			return element[front];
			else cout<<"no front element";
			return 0;
		}
};

void btcreate(btree *&b, char d)
{
	if (b==NULL)
	{
		b=new (btree);
		b->data=d;
		b->rchild=NULL;
		b->lchild=NULL;
	}
	cout<<"Enter left child of "<<b->data<<": ";
	cin>>d;
	if (d!='$')
	btcreate(b->lchild,d);
	cout<<"Enter right child of "<<b->data<<": ";
	cin>>d;
	if (d!='$')
	btcreate(b->rchild,d);
}

void btprint(btree *b)
{
	if (b!=NULL)
	{
		btprint(b->lchild);
		cout<<b->data<<" ";
		btprint(b->rchild);
	}
}

void store_child(btree *b,char ch, char *&a)
{
	if (b->data!=ch)
	{
		if (b->lchild!=NULL)
		store_child(b->lchild,ch,a);
		if (b->rchild!=NULL)
		store_child(b->rchild,ch,a);
	}
	else
	{
		if (b->lchild!=NULL)
		{
			   int i=0;
			   while (a[i]!='\0')
			   i++;
			   a[i]=b->lchild->data;
  	    }
		if (b->rchild!=NULL)
		{
			   int i=0;
			   while (a[i]!='\0')
			   i++;
			   a[i]=b->rchild->data;
		}
	}
}

int child_gen(btree *b,int &n)
{
	int m;
	if (b->rchild!=NULL)
	{
		n++;
		child_gen(b->rchild,n);
	}
	if (b->lchild!=NULL)
	if (b->lchild->lchild!=NULL)
	{
	    m=n;
		n=child_gen(b->lchild,n);
		if (m>n-1)
		return m;
		else return n;
	}
	return n;
}

void print(btree *b, queue &q)
{
	char temp;
	char a[50]={'\0'};
	char *array;
	array=&a[0];
	while (!q.isqEmpty())
	{
		temp=q.delqueue();
		store_child (b,temp,array);
	}
	for (int i=0;a[i]!='\0';i++)
	{
		q.enqueue(a[i]);
		cout<<a[i];
	}
	cout<<endl;
	for (int i=0;a[i]!='\0';i++)
	print (b,q);
}



int main()
{
	btree *b;
	char d;
	cout<<"Enter root for b1: ";
	cin>>d;
	b=NULL;
	btcreate(b,d);
	queue q;
	cout<<"\narray printing according to level:";
	q.enqueue(b->data);
	cout<<endl<<b->data<<endl;
	print(b,q);
	
	return 0;
}
