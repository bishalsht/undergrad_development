#include <iostream>
using namespace std;

#define n 6

struct gr{
	bool visited;
	int length;
	int prev;
	gr()
	{
		visited=false;
		length=99999;
		prev=-1;
	}
};

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

void bft(int graph[n][n],gr gph[],int src,int dest, queue &q)
{
	int curr=q.delqueue();
	if (curr!=dest)
	{
		for (int i=0;i<n;i++)
		{
			if (i!=curr&&!gph[i].visited)
			{
				if (graph[curr][i]!=0&&gph[i].length>gph[curr].length+graph[curr][i])
				{
					gph[i].length=gph[curr].length+graph[curr][i];
					gph[i].prev=curr;
					q.enqueue(i);
				}
			}
		}
	}
	if (!q.isqEmpty())
	bft(graph,gph,src,dest,q);
}

void path(gr gph[],int src,int dest)
{
	int stack[10]={-1},i=0;
	while (dest!=src)
	{
		stack[i++]=dest;
		dest=gph[dest].prev;
	}
	stack[i]=src;
	while (i!=-1)
	cout<<stack[i--]<<" ";
}

int main()
{
	gr gph[n];
	int src,dest;
	int graph[n][n];
	queue q;
	/*
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<n;j++)
		{
			if (i==j)
			graph[i][j]=0;
			else
			{
				int d;
				cout<<"Enter graph length between "<<i<<" & "<<j<<":";
				cin>>d;
				graph[i][j]=d;
			}
		}
	}
	*/
	
	graph[0][0]=0;
	graph[0][1]=7;
	graph[0][2]=9;
	graph[0][3]=0;
	graph[0][4]=0;
	graph[0][5]=14;
	graph[1][0]=7;
	graph[1][1]=0;
	graph[1][2]=10;
	graph[1][3]=15;
	graph[1][4]=0;
	graph[1][5]=0;
	graph[2][0]=9;
	graph[2][1]=10;
	graph[2][2]=0;
	graph[2][3]=11;
	graph[2][4]=0;
	graph[2][5]=2;
	graph[3][0]=0;
	graph[3][1]=15;
	graph[3][2]=11;
	graph[3][3]=0;
	graph[3][4]=6;
	graph[3][5]=0;
	graph[4][0]=0;
	graph[4][1]=0;
	graph[4][2]=0;
	graph[4][3]=6;
	graph[4][4]=0;
	graph[4][5]=9;
	graph[5][0]=14;
	graph[5][1]=0;
	graph[5][2]=2;
	graph[5][3]=0;
	graph[5][4]=9;
	graph[5][5]=0;
	
	cout<<"Adjancency matrix: \n";
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<n;j++)
		cout<<graph[i][j]<<"\t";
		cout<<endl;
	}
	
	cout<<"Enter source node:";
	cin>>src;
	cout<<"Enter destination node:";
	cin>>dest;
	q.enqueue(src);
	gph[src].visited=true;
	gph[src].length=0;
	bft(graph,gph,src,dest,q);
	cout<<"The shortest path by Dijkstra's algorithm: ";
	cout<<gph[dest].length;
	cout<<"\nPath followed:";
	path(gph,src,dest);
	return 0;
}
