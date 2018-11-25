#include <iostream>
using namespace std;

void dpt(int graph[6][6], int prev[], bool visited[], int src)
{
	int min=99;
	cout<<src<<"\t";
	if (prev[src]!=-1)
	cout<<prev[src]<<"\t\t"<<graph[src][prev[src]];
	else
	cout<<"Source node\t0";
	cout<<endl;
	while (min!=src)
	{
		min=src;
		for (int i=0;i<6;i++)
		{
			if (src==i||graph[src][i]==0) continue;
			if (min==src&&graph[src][i]>0&&visited[i]==false)
			min=i;
			else if (min!=src&&graph[src][i]<graph[src][min]&&visited[i]==false)
			min=i;
		}
		visited[src]=true;
		if (min!=src)
		{
			prev[min]=src;
			dpt(graph,prev,visited,min);
		}
	}
}

int main()
{
	bool visited[6]={false,false,false,false,false};
	int graph[6][6];
	int prev[6];
	int src;
	for (int i=0;i<6;i++)
	{
		for (int j=0;j<6;j++)
		graph[i][j]=0;
	}
	graph[0][1]=5;
	graph[0][5]=10;
	graph[0][3]=14;
	graph[1][0]=5;
	graph[1][2]=2;
	graph[1][3]=6;
	graph[2][1]=2;
	graph[2][4]=11;
	graph[2][5]=3;
	graph[3][0]=14;
	graph[3][1]=6;
	graph[3][4]=13;
	graph[4][3]=13;
	graph[4][2]=11;
	graph[4][5]=4;
	graph[5][4]=4;
	graph[5][2]=3;
	graph[5][0]=10;
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
	cout<<"Minimum spanning tree beginning from "<<src<<":\n";
	cout<<"Node\tPrevious\tLength\n";
	dpt(graph,prev,visited,src);
	
	return 0;
}
