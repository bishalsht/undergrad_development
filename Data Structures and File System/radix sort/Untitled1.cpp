#include <iostream>
using namespace std;

struct queue{
	int data[50];
	int size, front, rear;
	public:
		void enque(int e)
		{
			if (!qisFull())
			{
				if (front==-1) front++;
				rear=(rear+1)%50;
				data[rear]=e;
			}
			else cout<<"overflow";
		}
		int deque()
		{
			if (!qisEmpty())
			{
				int x=data[front];
				if (front==rear)
				{
					front=-1;
					rear=-1;
				}
				else
				front=(front+1)%50;
				return x;
			}
			else cout<<"underflow";
		}
		bool qisFull()
		{
			if (rear+1==front||((front==0)&&(rear==49)))
			return true;
			return false;
		}
		bool qisEmpty()
		{
			if (front==-1)
			return true;
			return false;
		}
		queue()
		{
			size=50;
			front=-1;
			rear=-1;
		}
};

int main()
{
	int data[5]={10,4529,2306,675,9887};
	int lsd=10;
	bool fin=false;
	
	queue que[10];
	for (int i=0;i<5;i++)
	{
		int x=data[i]%lsd;
		que[x].enque(data[i]);
	}
	
	lsd=lsd*10;
	int ind=0;
	bool m=false;
	while (!fin)
	{
		if (m) fin=true;
		for (int i=0;i<10;i++)
		{
			while (!que[i].qisEmpty())
			{
				int x=que[i].deque();
				data[ind++]=x;
			}
		}
		ind=0;
		m=true;
		for (int i=0;i<5;i++)
		{
			int x=data[i]%lsd;
			x=x*10;
			x=x/lsd;
			que[x].enque(data[i]);
			if (x!=0) m=false;
		}
		lsd*=10;
	}
	
	for (int i=0;i<5;i++) cout<<data[i]<<" ";
	
	return 0;
}
