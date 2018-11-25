#include <iostream>
using namespace std;

int st=0;

void dft(int stack[],int graph[8][8],bool visited[],int  curr)
{
	if (curr<8)
	{
		for (int i=0;i<8;i++)
		{
			if (i==7&&graph[curr][i]==0&&visited[curr]==false)
			{
				visited[curr]=true;
				stack[st++]=curr;
			}
			if (visited[i]==false&&graph[curr][i]==1)
			{
				visited[i]=true;
				dft(stack,graph,visited,i);
				stack[st++]=i;
			}
		}
	}
}

void dft_disp(int stack[],int graph[8][8],bool visited[],int curr)
{
	if (curr<8)
	{
		for (int i=0;i<8;i++)
		{
			if (i==7&&graph[curr][i]==0&&visited[curr]==true)
			{
				visited[curr]=false;
				cout<<curr<<" ";
			}
			if (visited[i]==true&&graph[curr][i]==1)
			{
				visited[i]=false;
				dft_disp(stack,graph,visited,i);
				cout<<i<<" ";
			}
		}
	}
}

int main()
{
	bool visited[8];
	for (int i=0;i<8;i++)
	visited[i]=false;
	int graph[8][8],grapht[8][8];
	for (int i=0;i<8;i++)
	{
		for (int j=0;j<8;j++)
		graph[i][j]=0;
	}
	graph[0][1]=1;
	graph[1][4]=1;
	graph[1][5]=1;
	graph[1][2]=1;
	graph[2][3]=1;
	graph[2][6]=1;
	graph[3][2]=1;
	graph[3][7]=1;
	graph[4][0]=1;
	graph[4][5]=1;
	graph[5][6]=1;
	graph[6][5]=1;
	graph[7][6]=1;
	graph[7][3]=1;
//	graph[0][1]=1;
	cout<<"Adjacency matrix of the graph\n";
	for (int i=0;i<8;i++)
	{
		for (int j=0;j<8;j++)
		cout<<graph[i][j]<<" ";
		cout<<endl;
	}
	
	cout<<endl<<endl<<endl;
	
	cout<<"Transpose of adjacency matrix (reversed graph)\n";
	for (int i=0;i<8;i++)
	{
		for (int j=0;j<8;j++)
		grapht[j][i]=graph[i][j];
	}
	for (int i=0;i<8;i++)
	{
		for (int j=0;j<8;j++)
		cout<<grapht[i][j]<<" ";
		cout<<endl;
	}
	
	int stack[8];
	while (st<8)
	{
		for (int i=0;i<8;i++)
		{
			if (visited[i]==false)
	   		{
	   			dft(stack,graph,visited,i);
	   			break;
	   		}
		}
	}
	cout<<stack[0];
	st--;
	cout<<"\nStrongly connected compnents\n";
	while (st>=0)
	{
		if (visited[stack[st]]==true)
		{
			dft_disp(stack,grapht,visited,stack[st--]);
			cout<<endl;
		}
   		else st--;
	}
	
	return 0;
}
