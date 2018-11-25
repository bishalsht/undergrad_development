#include <iostream>
using namespace std;

class queue{
	
	public:
		int size, front, rear;
		int element[50];
		queue(){
			rear=-1;
			front=-1;
			size=50;
		}
		void enqueue(int e){
			if (isqFull())
			   cout<<"Queue Overflow";
            else
            {
            	rear=(rear+1)%size;
            	element[rear]=e;
            	if (front==-1) front++;
            }
		}
		int delqueue(){
			if (isqEmpty())
			   cout<<"Queue Underflow";
			else{
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

int main()
{
	queue q1,q2;
	bool ans=1;
	int count=0,min,temp,j;
	cout<<"Enter elements to be sorted";
	while (!q1.isqFull()&&ans)
	{
		cin>>j;
		q1.enqueue(j);
		cout<<"Enter again? (1 for yes and 0 for no)";
		cin>>ans;
		count++;
	}
	while(!q1.isqEmpty())
	{
		min=q1.delqueue();
		for (int i=0;i<count-1;i++)
		{
			temp=q1.delqueue();
			if (min>temp)
			{
				q1.enqueue(min);
				min=temp;
			}
			else q1.enqueue(temp);
		}
		q2.enqueue(min);
		count--;
	}
	while (!q2.isqEmpty())
	cout<<q2.delqueue()<<endl;
	return 0;
}

