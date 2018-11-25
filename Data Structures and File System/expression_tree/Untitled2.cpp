#include <iostream>
using namespace std;

union un
{
	int in;
	char ch;
};

struct bnode{
	bnode *lchild;
	un data;
	int tag;
	bnode *rchild;
};

class stack{
	bnode *element[50];
	bnode *b;
	int top,size;
	public:
		stack(){
			top=-1;
			size=50;
			b=NULL;
		}
		void push(bnode *a){
  			 if (!isFull()) element[++top]=a;
		     else cout<<"Overflow";
		}
		bnode* pop(){
		    if (!isEmpty()) return element[top--];
		    else cout<<"Underflow";
		    return b;
		}
		bool isEmpty(){
		    if (top==-1) return 1;
		    return b;
		}
		bool isFull(){
		    if (top==49) return 1;
		    return b;
		}
		bnode* topelement(){
		    return element[top];
		}
};

void bnode_print(bnode *b)
{
	if (b!=NULL)
	{
		bnode_print(b->lchild);
		bnode_print(b->rchild);
		if (b->tag)
		cout<<b->data.in;
		else cout<<b->data.ch;
		cout<<" ";
	}
}

int calc(bnode *b,int a,int c)
{
	int m;
	switch (b->data.ch)
	{
		case '+':m=a+c;
  			     break;
		case '-':m=c-a;
  			     break;
		case '*':m=a*c;
  			     break;
		case '/':m=a/c;
  			     break;
		case '%':m=a%c;
  			     break;
	}
	return m;
}

int eval(bnode *b)
{
	if (b->lchild==NULL&&b->rchild==NULL)
	return b->data.in;
	if (b!=NULL)
	{
		int a=eval(b->lchild);
		int c=eval(b->rchild);
		return calc(b,a,c);
	}
}

int main()
{
	char a[50]={'\0'};
	stack eq;
	bnode *b, *c;
	cout<<"Enter Postfix Equation:";
	cin>>a;
	for (int i=0;a[i]!='\0';i++)
	{
		c=NULL;
		if (a[i]>='0'&&a[i]<='9')
		{
			c=new (bnode);
			c->tag=1;
			c->data.in=static_cast<int>(a[i])-48;
			c->lchild=NULL;
			c->rchild=NULL;
			eq.push(c);
		}
		else
		{
			c=new (bnode);
			c->tag=0;
			c->data.ch=a[i];
			c->rchild=eq.pop();
			c->lchild=eq.pop();
			eq.push(c);
		}
	}
	b=eq.pop();
	cout<<"Tree Printing in postorder: ";
	bnode_print(b);
	cout<<"\nSolution: "<<eval(b);
		
	return 0;
}
