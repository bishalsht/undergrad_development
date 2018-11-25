#include <iostream>
using namespace std;

struct bstnode{
	bstnode *lchild;
	int data;
	bstnode *rchild;
};

void bst_create(bstnode *&b, int d)
{
	if (b==NULL)
	{
		b=new(bstnode);
		b->data=d;
		b->lchild=NULL;
		b->rchild=NULL;
	}
	else if (d>b->data)
	{
		bst_create(b->rchild,d);
    }
	else if(d<b->data){
		bst_create(b->lchild,d);
	}
}
class stack{
	bstnode *element[50];
	bstnode *b;
	int top,size;
	public:
		stack(){
			top=-1;
			size=50;
			b=NULL;
		}
		void push(bstnode *a){
  			 if (!isFull()) element[++top]=a;
		     else cout<<"Overflow";
		}
		bstnode* pop(){
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
		bstnode* topelement(){
		    return element[top];
		}
};

void preorder_print(bstnode *b)
{
	if (b!=NULL)
	{
		stack a;
		bstnode *c;
		a.push(b);
		while (!a.isEmpty())
		{
			c=a.pop();
			cout<<c->data<<" ";
			if (c->rchild!=NULL)
			a.push(c->rchild);
			if (c->lchild!=NULL)
			a.push(c->lchild);
		}
	}
}

	
void inorder_print(bstnode *b)
{
	stack s;
	bstnode *temp=b;
	bool fin=0;
	while (!fin)
	{
		  if (temp!=NULL)
	   	 {
	   	 	s.push(temp);
			temp=temp->lchild;
		 }
	  	 else
	  	 {
		  	   if (s.isEmpty())
		   	   fin=true;
			   else
			   {
				   	temp=s.pop();
			        cout<<temp->data<<" ";
			        temp=temp->rchild;
		       }
	       }
    }
}

void postorder_print(bstnode *b)
{
  if (b!=NULL)
  {
	  stack s;
	  s.push(b);
	  bstnode *prev = NULL;
	  while (!s.isEmpty())
	  {
	    bstnode *curr = s.topelement();
	    if (!prev||prev->lchild==curr||prev->rchild==curr)
		{
	      if (curr->lchild!=NULL)
	        s.push(curr->lchild);
	      else if (curr->rchild!=NULL)
	        s.push(curr->rchild);
	      else
		  {
	        cout<<curr->data<<" ";
	        s.pop();
	      }
	    } 
	    else if(curr->lchild==prev)
		{
	      if (curr->rchild!=NULL)
	        s.push(curr->rchild);
	      else
		  {
	        cout<<curr->data<<" ";
	        s.pop();
	      }
	    }
	    else if (curr->rchild==prev)
		{
	      cout<<curr->data<<" ";
	      s.pop();
	    }
	    prev=curr;
	  }
  }
}

int main()
{
	bstnode *b;
	int d;
	cout<<"Enter data: ";
	cin>>d;
	b=NULL;
	while (d!=-1)
	{
		bst_create(b,d);
		cin>>d;
	}
	cout<<"Preorder Printing:\n";
	preorder_print(b);
	cout<<"\nInorder Printing:\n";
	inorder_print(b);
	cout<<"\nPostorder Printing:\n";
	postorder_print(b);
	return 0;
}
