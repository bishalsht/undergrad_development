#include <iostream>
using namespace std;

void init(int heap[])
{
	for (int i=0;i<30;i++)
	heap[i]=-1;
}

void heapify(int heap[],int i)
{
	if (i==0) return;
	int in;
	if (heap[i]%2==0) in=i/2-1;
	else in=(i-1)/2;
	if (heap[in]>heap[i])
	{
		heap[i]=heap[in]+heap[i];
		heap[in]=heap[i]-heap[in];
		heap[i]=heap[i]-heap[in];
		heapify(heap,in);
	}
}

void add(int heap[],int data)
{
	int i;
	for (i=0;heap[i]!=-1;i++);
	heap[i]=data;
	heapify(heap,i);
}

void print(int heap[],int i)
{
	if (i<30&&heap[i]>=0)
	{
		print(heap,2*i+1);
		print(heap,2*i+2);
		cout<<heap[i]<<" ";
	}
}

int minimum(int heap[],int pos)
{
	if (heap[pos*2+1]==min(heap[pos*2+1],heap[pos*2+2])) return pos*2+1;
	return pos*2+2;
}

void del_heapify(int heap[],int pos, int limit)
{
	if (pos<limit)
	{
		int x;
		if (pos*2+1<limit&&pos*2+2<limit)
		x=minimum(heap,pos);
		else if (pos*2+1<limit) x=pos*2+1;
		else return;
		if (heap[pos]>heap[x])
		{
			int m=heap[pos];
			heap[pos]=heap[x];
			heap[x]=m;
			del_heapify(heap,x,limit);
		}
	}
}

void del(int heap[])
{
	if (heap[0]==-1) return;
	int i;
	for (i=0;heap[i]!=-1;i++);
	heap[0]=heap[i-1];
	heap[i-1]=-1;
	del_heapify(heap,0,i-1);
}

int main()
{
	int heap[30];
	init(heap);
	int data;
	while (cin>>data&&data!=-1)
	add(heap,data);
	
	print(heap,0);
	
	del(heap);
	cout<<"del\n";
	print(heap,0);
	
	return 0;
}
