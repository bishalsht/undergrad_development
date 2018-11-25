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

bool all_visited(bool visited[])
{
	for (int i=0;i<6;i++)
	{
		if (visited[i]==false)
		return false;
	}
	return true;
}

void bft(int graph[6][6],bool visited[],int prev[],queue &q)
{
	int curr=q.delqueue();
	if (!all_visited(visited))
	{
		for (int i=0;i<6;i++)
		{
			if (i!=curr&&visited[i]==false)
			{
				if (graph[curr][i]!=0)
				{
					visited[i]=true;
					prev[i]=curr;
					q.enqueue(i);
				}
			}
		}
	}
	if (!q.isqEmpty())
	bft(graph,visited,prev,q);
}


int main()
{
	bool visited[6]={false};
	int graph[6][6];
	int prev[6];
	int src;
	for (int i=0;i<6;i++)
	{
		for (int j=0;j<6;j++)
		graph[i][j]=0;
	}
	graph[0][1]=1;
	graph[0][5]=1;
	graph[0][3]=1;
	graph[1][0]=1;
	graph[1][2]=1;
	graph[1][3]=1;
	graph[2][1]=1;
	graph[2][4]=1;
	graph[2][5]=1;
	graph[3][0]=1;
	graph[3][1]=1;
	graph[3][4]=1;
	graph[4][3]=1;
	graph[4][2]=1;
	graph[4][5]=1;
	graph[5][4]=1;
	graph[5][2]=1;
	graph[5][0]=1;
	cout<<"Adjacency matrix\n";
	for (int i=0;i<6;i++)
	{
		for (int j=0;j<6;j++)
		cout<<graph[i][j]<<"\t";
		cout<<endl;
	}
	
	cout<<"Enter beginning node:";
	cin>>src;
	prev[src]=-1;

	queue q;
	q.enqueue(src);

	bft(graph, visited, prev, q);
	
	prev[src]=-1;
	
	cout<<"Node\tPrevious\n";
	for (int i=0;i<6;i++)
	cout<<i<<"\t"<<prev[i]<<endl;

	return 0;
}
