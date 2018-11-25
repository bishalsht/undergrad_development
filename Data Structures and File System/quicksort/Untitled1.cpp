#include <iostream>
using namespace std;

int array[10]={18,16,3,89,67,12,7,90,34,11};

void swap(int &a, int &b)
{
	int x=a;
	a=b;
	b=x;
}

void print(int a[])
{
	for (int i=0;i<10;i++) cout<<a[i]<<" ";
	cout<<endl;
}

int partition(int low,int l,int h)
{
	int p=array[low];
	while (l<h)
	{
		while (p>array[l])
		l++;
		while (array[h]>p)
		h--;
		if (l<h)
		swap(array[h],array[l]);
	}
	if (array[h]<array[low])
	swap(array[low],array[h]);
	return h;
}

void quicksort(int low,int high)
{
	if (low<high)
	{
		int x=partition(low,low+1,high);
		quicksort(low,x-1);
		quicksort(x+1,high);
	}
}

int main()
{
	cout<<"Unsorted array: ";
	print(array);
	
	quicksort(0, 9);
	
	cout<<"\nSorted array: ";
	print(array);
	
	return 0;
}
