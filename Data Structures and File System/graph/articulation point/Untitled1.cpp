#include <iostream>
using namespace std;

int no=1;

void init(int graph[7][7])
{
	for (int i=0;i<7;i++)
	{
		for (int j=0;j<7;j++)
		graph[i][j]=0;
	}
	graph[0][1]=1;
	graph[0][3]=1;
	graph[1][0]=1;
	graph[1][2]=1;
	graph[2][1]=1;
	graph[2][3]=1;
	graph[2][6]=1;
	graph[3][0]=1;
	graph[3][2]=1;
	graph[3][4]=1;
	graph[3][5]=1;
	graph[4][3]=1;
	graph[4][5]=1;
	graph[5][3]=1;
	graph[5][4]=1;
	graph[6][2]=1;
}

void low_find(int graph[7][7],int child[7][7],int back[7][7],int low[7],int num[7],bool visited[7],int m)
{
	int back_num=99;
	for (int i=0;i<7;i++)
	{
		if (back[m][i]==1&&num[i]<back_num)
		back_num=num[i];
	}
	for (int i=num[m];i<8;i++)
	{
		for (int j=0;j<7;j++)
		if (child[m][j]==1)
		low_find(graph,child,back,low,num,visited,j);
	}
	for (int j=0;j<7;j++)
	{
		if (child[m][j]&&low[m]>low[j])
		low[m]=low[j];
	}
	if (back_num<low[m]&&back_num<num[m])
	low[m]=back_num;
	else if (num[m]<low[m]&&num[m]<back_num)
	low[m]=num[m];
}

void dft(int graph[7][7],int child[7][7],bool visited[],int back[7][7],int num[],int low[],int m)
{
	for (int i=0;i<7;i++)
	{
		if (graph[m][i]==1&&!visited[i])
		{
			child[m][i]=1;
			visited[i]=true;
			num[i]=no++;
			for (int l=0;l<7;l++)
			{
				if (l!=m&&visited[l]==true&&graph[i][l]==1)
				back[i][l]=1;
			}
			dft(graph,child,visited,back,num,low,i);
		}
	}
	low_find(graph,child,back,low,num,visited,m);
}

int main()
{
	int graph[7][7],child[7][7],back[7][7],low[7],num[7];
	bool visited[7];
	for (int i=0;i<7;i++)
	{
		visited[i]=false;
		low[i]=99;
		num[i]=-1;
	}
	for (int i=0;i<7;i++)
	{
		for (int j=0;j<7;j++)
		{
			back[i][j]=0;
			child[i][j]=0;
		}
	}
	init (graph);
	
	visited[0]=true;
	num[0]=no++;
	dft(graph,child,visited,back,num,low,0);
	
	cout<<"Graph\n";
	for (int i=0;i<7;i++)
	{
		for (int j=0;j<7;j++)
		cout<<graph[i][j]<<" ";
		cout<<endl;
	}
	cout<<"\n\nBack Graph\n";
	for (int i=0;i<7;i++)
	{
		for (int j=0;j<7;j++)
		cout<<back[i][j]<<" ";
		cout<<endl;
	}
	cout<<"\n\nChild graph\n";
	for (int i=0;i<7;i++)
	{
		for (int j=0;j<7;j++)
		cout<<child[i][j]<<" ";
		cout<<endl;
	}
	
	cout<<"Articulation points are nodes: ";
	
	int x=0;
	for (int i=0;i<7;i++)
	{
		if (child[0][i]==1) x++;
	}
	
	if (x>1) cout<<1<<" ";
	
	for (int i=1;i<7;i++)
	{
		for (int j=0;j<7;j++)
		if (child[i][j]==1&&num[i]<=low[j])
		cout<<j<<" ";
	}
	
	return 0;
}
