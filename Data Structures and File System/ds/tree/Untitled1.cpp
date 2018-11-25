#include <iostream>
using namespace std;

class queue{
	int size, front, rear;
	char element[50];
	public:
		queue(){
			rear=-1;
			front=-1;
			size=50;
		}
		void enqueue(char e){
			if (isqFull())
			   cout<<"Queue Overflow";
            else
            {
            	rear=(rear+1)%size;
            	element[rear]=e;
            	if (front==-1) front++;
            }
		}
		char delqueue(){
			if (isqEmpty())
			   cout<<"Queue Underflow";
			else{
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
};

void input(queue&,int&);
void print(queue&);

int main()
{
	char ans;
	queue q;
	int pos=1;
	cout<<"Tree Program"<<endl;
	input(q,pos);
	print(q);
	return 0;
}

void input(queue &q, int &pos)
{
	bool ans,ans2,ans3,ans4;
	char data;
	cout<<"Want to enter?";
	cin>>ans;
	if (ans)
	{
		cin>>data;
		q.enqueue(pos+48);
		q.enqueue(data);
		cout<<"Subbranch?";
		cin>>ans2;
		if (ans2)
		{
			input(q,++pos);
		}
		else
		{
			cout<<"same branch?";
			cin>>ans3;
			if (ans3)
			input(q,pos);
			else
			{
xy:				cout<<"upper branch?";
				cin>>ans4;
				if (ans4)
				input(q,--pos);
				else
				{
					pos--;
					if (pos-1)
					goto xy;
				}
			}
		}
	}
	else cout<<"fail";
}

void print(queue &q)
{
	char num,alpha;
	while (!q.isqEmpty())
	{
		num=q.delqueue();
		alpha=q.delqueue();
		for (char i='0';i<num;i++)
		cout<<"|";
		cout<<alpha<<endl;
	}
}
