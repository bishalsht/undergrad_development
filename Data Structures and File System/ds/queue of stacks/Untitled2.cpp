#include <iostream>
using namespace std;

class stack{
	int element[50], size, top;
	public:
		void push (int x)
		{
			if (!isFull())
			element[++top]=x;
			else cout<<"Stack Overflow";
		}
		int pop()
		{
			if (!isEmpty())
			return element[top--];
			else cout<<"Stack Underflow";
		}
		bool isEmpty()
		{
			if (top==-1) return 1;
			return 0;
		}	
		bool isFull()
		{
			if (top==49) return 1;
			return 0;
		}
		char topelement()
		{
			return element[top];
		}
		stack()
		{
			top=-1;
			size=50;
		}
};

class queue{
	stack element[50],empty;
	int size, front, rear;
	public:
		queue(){
			rear=-1;
			front=-1;
			size=50;
		}
		void enqueue(stack e){
			if (isqFull())
			   cout<<"Queue Overflow";
            else
            {
            	rear=(rear+1)%size;
            	element[rear]=e;
            	if (front==-1) front++;
            }
		}
		stack delqueue(){
			if (isqEmpty())
			   cout<<"Queue Underflow";
			else{
				stack l=element[front];
				if (front==rear)
				{
					front=-1;
					rear=-1;
				}
				else
				front=(front+1)%size;
				return l;
			}
			return empty;
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
};


int main()
{
	queue q;
	stack s1,s2,s3;
	for (int i=0;i<5;i++)
	s1.push(i);
	for (int i=0;i<5;i++)
	s2.push(i+1000);
	for (int i=0;i<5;i++)
	s3.push(i*1000);
	q.enqueue(s1);
	q.enqueue(s2);
	q.enqueue(s3);
	stack b=q.delqueue();
	for (int i=0;i<5;i++)
	cout<<b.pop()<<" ";
	cout<<endl;
	b=q.delqueue();
	for (int i=0;i<5;i++)
	cout<<b.pop()<<" ";
	cout<<endl;
	b=q.delqueue();
	for (int i=0;i<5;i++)
	cout<<b.pop()<<" ";
	cout<<endl;
	return 0;
}
