#include <iostream>
#include <fstream>
#include <cmath>

#define binCount 5

using namespace std;

struct lnode{
	int data;
	lnode *next;
	lnode()
	{
		data=-1;
		next=NULL;
	}
};

struct bin{
	lnode *bucket;
	float mean;
	int count;
	bin()
	{
		mean = 0;
		count = 0;
		bucket = NULL;
	}
};

char D[15];
int main()
{
	ifstream fin("inp");
	bin bins[binCount];
	int val;

	for (int i=0;i<binCount;i++)
	{
		fin>>val;
		bins[i].mean = val;
		bins[i].count = 1;
		bins[i].bucket = new lnode;
		bins[i].bucket->data = val;
	}

	while (fin>>val)
	{
		int min, ind=0;
		min = abs(bins[0].mean - val);
		for (int i=1;i<binCount;i++)
		{
			if (min > abs(bins[i].mean - (float)val))
			{
				min = abs(bins[i].mean - (float)val);
				ind = i;
			}
		}
		bins[ind].mean = ((bins[ind].mean * (float)bins[ind].count)+(float)val)/((float)bins[ind].count+1);
		bins[ind].count++;
		lnode *temp = bins[ind].bucket;
		while (temp->next) temp = temp->next;
		temp->next = new lnode;
		temp = temp->next;
		temp->data = val;
	}

	for (int i=0;i<binCount;i++)
	{
		cout<<i+1<<"th mean : "<<bins[i].mean<<endl;
		cout<<i+1<<"th count : "<<bins[i].count<<endl;
		lnode *temp = bins[i].bucket;
		while (temp)
		{
			cout<<temp->data<<" ";
			temp=temp->next;
		}
		cout<<endl<<endl<<endl;
	}

	return 0;
}