#include <iostream>
using namespace std;

int st=0;

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

void bft(int graph[6][6], bool visited[], queue &q, int src)
{
	while (!q.isqEmpty())
	{
		int x=q.delqueue();
		for (int i=0;i<6;i++)
		{
			if (graph[x][i]==1&&visited[i]==false)
			{
				cout<<i<<" ";
				q.enqueue(i);
				visited[i]=true;
			}
		}
	}
}

void dft(int stack[], int graph[6][6],bool visited[],int  curr)
{
	if (curr<6)
	{
		for (int i=0;i<6;i++)
		{
			if (i==5&&graph[curr][i]==0&&visited[curr]==true&&curr!=0)
			{
				visited[curr]=false;
				st++;
				cout<<curr<<" ";
			}
			if (visited[i]==true&&graph[curr][i]==1&&i!=0)
			{
				visited[i]=false;
				cout<<i<<" ";
				st++;
				dft(stack, graph,visited,i);
			}
		}
	}
}

int main()
{
	bool visited[6]={false};
	int graph[6][6];
	int src;
	queue q;
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
	
	src=0;
	q.enqueue(src);
	visited[src]=true;
	cout<<"\nBFT\n";
	cout<<src<<" ";
	
	while (!q.isqEmpty())
	{
		bft(graph,visited,q,src);
		for (int i=0;i<6;i++)
		{
			if (visited[i]==false)
			{
				visited[i]=true;
				cout<<i<<" ";
				q.enqueue(i);
				src=i;
				break;
			}
		}
	}	
	
	cout<<"\nDFT\n";
	int stack[6];
	cout<<"0 ";
	while (st<5)
	{
		for (int i=0;i<6;i++)
		{
			if (visited[i]==true)
	   		{
	   			dft(stack,graph,visited,i);
	   			break;
	   		}
		}
	}
	
	return 0;
}
