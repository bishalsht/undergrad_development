#include <iostream>
using namespace std;

int in_deg(int graph[8][8], int vertex)
{
	int count=0;
	for (int i=0;i<8;i++)
	{
		if (graph[i][vertex]==1) count++;
	}
	return count;
}

int out_deg(int graph[8][8], int vertex)
{
	int count=0;
	for (int i=0;i<8;i++)
	{
		if (graph[vertex][i]==1) count++;
	}
	return count;
}

void init(int available[])
{
	for (int i=0;i<8;i++)
	available[i]=-1;
}

int main()
{
	int m=0;
	int array[8];
	int graph[8][8];
	
	for (int i=0;i<8;i++)
	{
		for (int j=0;j<8;j++)
		graph[i][j]=0;
	}
	graph[0][1]=1;
	graph[0][2]=1;
	graph[1][3]=1;
	graph[1][5]=1;
	graph[2][3]=1;
	graph[2][7]=1;
	graph[3][4]=1;
	graph[4][5]=1;
	graph[5][6]=1;
	graph[3][6]=1;
	for (int i=0;i<8;i++)
	{
		int available[8];
		init (available);
		
		for (int i=0;i<8;i++)
		{
			int in=in_deg(graph,i);
			int out=out_deg(graph,i);
			if (in==0)
			{
				bool avail=true;
				for (int k=0;k<m;k++)
				{
					if (array[k]==i) avail=false;
				}
				if (avail)
				available[i]=i;
			}
		}
		
		cout<<"Choices: ";
		for (int i=0;i<8;i++)
		{
			if (available[i]!=-1) cout<<available[i]<<" ";
		}
		cout<<endl;
		
		bool fin=false;
		int choice;
		while (!fin)
		{
			cout<<"Your Choice: ";
			cin>>choice;
			
			for (int i=0;i<8;i++)
			{
				if (choice==available[i])
				{
					fin=true;
					break;
				}
				else if (i==7)
				{
					cout<<"Your choice is not available. Please re-enter.\n";
				}
			}
		}
		
		array[m++]=choice;
		
		for (int i=0;i<8;i++)
		{
			graph[choice][i]=0;
			graph[i][choice]=0;
		}
	}
	
	cout<<"\nTopological Sorting :";
	for (int i=0;i<8;i++)
	cout<<array[i]<<" ";
	
	
	return 0;
}

