#include <iostream>
using namespace std;

struct edge{
	char vertex[2];
	int length;
	edge()
	{
		vertex[0]='\0';
		vertex[1]='\0';
		length=0;
	}
};

edge empty_edge;

void operator&=(edge a,edge b)
{
	a.length=b.length;
	a.vertex[0]=b.vertex[0];
	a.vertex[1]=b.vertex[1];
}

void init(edge heap[])
{
	for (int i=0;i<30;i++)
	{
		heap[i].length=-1;
		heap[i].vertex[0]='\0';
		heap[i].vertex[1]='\0';
	}
}

void heapify(edge heap[],int i)
{
	if (i==0) return;
	int in;
	if (i%2==0) in=i/2-1;
	else in=(i-1)/2;
	if (heap[in].length>heap[i].length)
	{
		edge temp;
		temp=heap[in];
		heap[in]=heap[i];		
		heap[i]=temp;
		heapify(heap,in);
	}
}

void add(edge heap[],edge data)
{
	int i;
	for (i=0;heap[i].length>=0;i++);
	heap[i]=data;
	heapify(heap,i);
}

void print(edge heap[],int i)
{
	if (i<30&&heap[i].length>=0)
	{
		print(heap,2*i+1);
		print(heap,2*i+2);
		cout<<heap[i].vertex[0]<<" "<<heap[i].vertex[1]<<" "<<heap[i].length<<endl;
	}
}

int minimum(edge heap[],int pos)
{
	if (heap[pos*2+1].length==min(heap[pos*2+1].length,heap[pos*2+2].length)) return pos*2+1;
	return pos*2+2;
}

void del_heapify(edge heap[],int pos, int limit)
{
	if (pos<limit)
	{
		int x;
		if (pos*2+1<limit&&pos*2+2<limit)
		x=minimum(heap,pos);
		else if (pos*2+1<limit) x=pos*2+1;
		else return;
		if (heap[pos].length>heap[x].length)
		{
			edge m=heap[pos];
			heap[pos]=heap[x];
			heap[x]=m;
			del_heapify(heap,x,limit);
		}
	}
}

edge del(edge heap[])
{
	if (heap[0].length==-1) return empty_edge;
	int i;
	for (i=0;heap[i].length!=-1;i++);
	edge x=heap[0];
	heap[0]=heap[i-1];
	heap[i-1].length=-1;
	heap[i-1].vertex[0]='\0';
	heap[i-1].vertex[1]='\0';
	del_heapify(heap,0,i-1);
	return x;
}

void init(int a[7][7])
{
	for (int i=0;i<7;i++)
	{
		for (int j=0;j<7;j++)
		a[i][j]=0;
	}/*
	graph[0][1]=10;
	graph[1][0]=10;
	
	graph[0][3]=8;
	graph[3][0]=8;
	
	graph[1][2]=6;
	graph[2][1]=6;
	
	graph[2][3]=11;
	graph[3][2]=11;
	
	graph[2][6]=2;
	graph[6][2]=2;
	
	graph[3][4]=7;
	graph[4][3]=7;
	
	graph[3][5]=14;
	graph[5][3]=14;
	
	graph[4][3]=12;
	graph[3][4]=12;
	
	graph[5][4]=9;
	graph[4][5]=9;*/
	
	a[0][6]=1;
	a[6][0]=1;
	
	a[1][6]=1;
	a[6][1]=1;

	a[3][4]=2;
	a[4][3]=2;

	a[3][6]=5;
	a[6][3]=5;

	a[0][5]=3;
	a[5][0]=3;

	a[0][1]=4;
	a[1][0]=4;

	a[1][2]=4;
	a[2][1]=4;

	a[2][6]=5;
	a[6][2]=5;

	a[4][5]=5;
	a[5][4]=5;

	a[2][3]=6;
	a[3][2]=6;

	a[4][6]=9;
	a[6][4]=9;

	a[5][6]=7;
	a[6][5]=7;
}

int find(int s[],int x)
{
	if(s[x]==0) return x;
	else return find(s,s[x]);
}

void add_to_array(int graph[7][7],edge x)
{
	graph[x.vertex[0]][x.vertex[1]]=x.length;
	graph[x.vertex[1]][x.vertex[0]]=x.length;
}

void un(int s[],int u,int v)
{
	s[u]=v;
}

int main()
{
	int graph[7][7], tree[7][7];
	init(graph);
	
	edge heap[30];
	init(heap);
	
	edge data;
	
	for (int i=0;i<7;i++)
	{
		for (int j=0;j<7;j++)
		{
			tree[i][j]=0;
			if (graph[i][j]>0)
			{
				data.length=graph[i][j];
				data.vertex[0]=i;
				data.vertex[1]=j;
				add(heap,data);
			}
		}
	}
	
	int s[30];
	for (int i=0;i<30;i++) s[i]=0;
	
	for (int i=0;heap[0].length!=-1;i++)
	{
		edge x=del(heap);
		int u=find(s,x.vertex[0]);
		int v=find(s,x.vertex[1]);
		if (u!=v||u==0||v==0)
		{
			add_to_array(tree,x);
			un(s,u,v);
			//cout<<ct++;
		}
	}
	
	cout<<endl;
		
	cout<<"Graph\n";
	for(int i=0;i<7;i++)
	{
		for(int j=0;j<7;j++)
		{
			cout<<graph[i][j]<<"\t";
		}
		cout<<endl;
	}
	
	cout<<endl<<endl<<"Spanning tree:\n";
			
	for(int i=0;i<7;i++)
	{
		for(int j=0;j<7;j++)
		{
			cout<<tree[i][j]<<"\t";
		}
		cout<<endl;
	}
	
	return 0;
}
