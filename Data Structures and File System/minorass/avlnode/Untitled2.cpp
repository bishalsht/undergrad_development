#include <iostream>
using namespace std;
class queue{
	
	public:
		int size, front, rear, count;
		int element[50];
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
		int delqueue(){
			if (isqEmpty())
			   cout<<"Queue Underflow";
			else{
				count--;
				int l=element[front];
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
		int frontelement(){
			if (!isqEmpty())
			return element[front];
			else cout<<"no front element";
			return 0;
		}
};

struct avlnode{
	avlnode *lchild, *rchild;
	int data;
};

int height(avlnode *L)
{
	if (L==NULL)
	return (-1);
	else 
	{
		int a=height(L->lchild);
		int b=height(L->rchild);
		if (a>b) return (a+1);
		else return (b+1);
	}
}

void print(avlnode *A)
{
	if (A!=NULL)
	{
		print(A->lchild);
		cout<<A->data<<" ";
		print(A->rchild);
	}
}

bool search(avlnode *a, int d)
{
	if (a!=NULL)
	{
		while (a!=NULL)
		{
			if (a->data>d)
			a=a->lchild;
			else if (a->data<d)
			a=a->rchild;
			else if (a->data==d)
			return 1;
		}
		return 0;
	}
}

void rotate_left(avlnode *&k1, avlnode *&k2)
{
	k1->rchild=k2->lchild;
	k2->lchild=k1;
	k1=k2;
}

void rotate_right(avlnode *&k1, avlnode *&k2)
{
	k1->lchild=k2->rchild;
	k2->rchild=k1;
	k1=k2;
}

void add_end(avlnode *&A, int d)
{
	if (A==NULL)
	{
		A=new avlnode;
		A->data=d;
		A->lchild=NULL;
		A->rchild=NULL;
	}
	else if (A->data>d)
	add_end(A->lchild,d);
	else if (A->data<d)
	add_end(A->rchild,d);
}

void balance(avlnode *&A)
{
	if (height(A->lchild)>height(A->rchild)+1)
	{
		cout<<A->data<<" ";
		avlnode *k1,*k2,*k3;
		k1=A;
		k3=k1->lchild;
		k2=k3->rchild;
		if (k2==NULL)
		rotate_right(A,k3);
		else
		{
			rotate_left(A->lchild,k2);
			rotate_right(A,k2);
		}
	}
	else if (height(A->rchild)>height(A->lchild)+1)
	{
		cout<<A->data<<" ";
		avlnode *k1,*k2,*k3;
		k1=A;
		k3=k1->rchild;
		k2=k3->lchild;
		if (k2==NULL)
		rotate_left(A,k3);
		else
		{
			rotate_right(A->rchild,k2);
			rotate_left(A,k2);
		}
	}
	if (A->lchild!=NULL)
	balance(A->lchild);
	if (A->rchild!=NULL)
	balance(A->rchild);
}

void del(avlnode *&a, int d)
{
	if (a!=NULL)
	{
		if (a->data>d)
		del (a->lchild,d);
		else if (a->data<d)
		del (a->rchild,d);
		if (a->data==d&&(a->lchild!=NULL&&a->rchild!=NULL))
		{
			avlnode *b;
			b=a->rchild;
			while (b->lchild!=NULL)
			b=b->lchild;
			int e=b->data;
			del(a,e);
			a->data=e;
		}
		else if (a->data==d&&(a->lchild==NULL||a->rchild==NULL))
		{
			if (a->lchild==NULL)
			a=a->rchild;
			else if (a->rchild==NULL)
			a=a->lchild;
		}
	}
}

void store_child(avlnode *b,int in, int *&a)
{
	if (b->data!=in)
	{
		if (b->lchild!=NULL)
		store_child(b->lchild,in,a);
		if (b->rchild!=NULL)
		store_child(b->rchild,in,a);
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

void print(avlnode *b, queue &q)
{
	int temp;
	int a[50]={'\0'};
	int *array;
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
	avlnode *a;
	a=NULL;
	int d;
	cout<<"Enter data:";
	cin>>d;
	while (d!=-1)
	{
		add_end(a,d);
		balance(a);
		cout<<"Enter data:";
		cin>>d;
	}
	
	cout<<"\nElements :";
	queue q;
	q.enqueue(a->data);
	cout<<a->data<<endl;
	print (a,q);
	
	cout<<"\nElement to search: ";
	cin>>d;
	if (search(a,d))
	cout<<"Element found.";
	else cout<<"Element not found.";
	
	cout<<"\nEnter data to delete: ";
	cin>>d;
	del(a,d);
	balance(a);
	cout<<"\nElements:";
	print(a);
	
	return 0;
}
