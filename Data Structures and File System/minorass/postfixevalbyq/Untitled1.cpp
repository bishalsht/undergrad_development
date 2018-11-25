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

int main()
{
	queue q;
	char e[50];
	cout<<"Enter postfix equation: ";
	cin>>e;
	for (int i=0;e[i]!='\0';i++)
	{
		if (e[i]>'0'&&e[i]<'9')
			q.enqueue(static_cast<int>(e[i]-48));
		else
		{
			int j=q.count, temp, temp1, temp2;
			for (int b=0;b<j-2;b++)
			{
				temp=q.delqueue();
				q.enqueue(temp);
			}
			switch (e[i])
			{
				case '+': temp1=q.delqueue();
					 	  temp2=q.delqueue();
					 	  q.enqueue(temp1+temp2);
					 	  break;
		        case '-': temp1=q.delqueue();
					 	  temp2=q.delqueue();
					 	  q.enqueue(temp1-temp2);
					 	  break;
 	            case '*': temp1=q.delqueue();
					 	  temp2=q.delqueue();
					 	  q.enqueue(temp1*temp2);
					 	  break;
 	            case '/': temp1=q.delqueue();
					 	  temp2=q.delqueue();
					 	  q.enqueue(temp1/temp2);
					 	  break;
		        default: cout<<"Error equation";
		        		 break;
			}
		}
	}
	cout<<"Solution :"<<q.delqueue();
	return 0;
}
