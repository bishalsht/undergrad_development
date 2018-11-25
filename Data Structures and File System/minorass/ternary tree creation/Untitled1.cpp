#include <iostream>
#include <cstdlib>
using namespace std;

struct tnode{
	tnode *lchild, *mchild, *rchild;
	char data1,data2;
	tnode()
	{
		data1='$';
		data2='$';
	}
};


class queue{
	
public:
	int size, front, rear;
	tnode *element[50];
	queue(){
		rear=-1;
		front=-1;
		size=50;
	}
	void enqueue(tnode *&e){
		if (isqFull())
		   cout<<"Queue Overflow";
        else
        {
        	rear=(rear+1)%size;
        	element[rear]=e;
        	if (front==-1) front++;
        }
	}
	tnode *&delqueue(){
		if (isqEmpty())
		   cout<<"Queue Underflow";
		else{
			tnode *&l=element[front];
			if (front==rear)
			{
				front=-1;
				rear=-1;
			}
			else
			front=(front+1)%size;
			return l;
		}
		exit(1);
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
	tnode* frontelement(){
		if (!isqEmpty())
		return element[front];
		else cout<<"no front element";
		return 0;
	}
};




void create(tnode *&t, char d, queue &q)
{
	if (t==NULL)
	{
		t=new tnode;
		t->lchild=NULL;
		t->rchild=NULL;
		t->mchild=NULL;
		t->data1=d;
		cout<<"input:";
		cin>>d;
		if (d!='$')
		t->data2=d;
	}
	else
	{
		q.enqueue(t->lchild);
		q.enqueue(t->mchild);
		q.enqueue(t->rchild);
		while (q.frontelement()!=NULL)
		q.delqueue();
		create(q.delqueue(),d,q);
		cout<<1;
	}
}

void print(tnode *t)
{
	if (t!=NULL)
	{
		cout<<t->data1<<" "<<t->data2<<" ";
		print(t->lchild);
		print(t->mchild);
		print(t->rchild);
	}
	else cout<<"NULL";
}

int main()
{
	char d;
	tnode* t;
	queue q;
	t=NULL;
	cout<<"input: ";
	cin>>d;
	while (d!='$')
	{
		create(t,d,q);
		cout<<"input: ";
		cin>>d;
	}
	print(t);
	return 0;
}
