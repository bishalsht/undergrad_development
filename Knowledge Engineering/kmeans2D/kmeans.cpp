#include <iostream>
#include <fstream>
#include <cmath>

#define binCount 5

using namespace std;

struct lnode{
	int dataX;
	int dataY;
	lnode *next;
	lnode()
	{
		dataX=-1;
		dataY=-1;
		next=NULL;
	}
};

struct bin{
	lnode *bucket;
	float meanX,meanY;
	int count;
	bin()
	{
		meanX = 0;
		meanY = 0;
		count = 0;
		bucket = NULL;
	}
};

char D[15];
int main()
{
	ifstream fin("inp");
	bin bins[binCount];
	int valX,valY;

	for (int i=0;i<binCount;i++)
	{
		fin>>valX;
		bins[i].meanX = valX;
		fin>>valY;
		bins[i].meanY = valY;
		bins[i].count = 1;
		bins[i].bucket = new lnode;
		bins[i].bucket->dataX = valX;
		bins[i].bucket->dataY = valY;
	}

	while (fin>>valX)
	{
		fin>>valY;
		int min, ind=0;
		min = sqrt(pow(abs(bins[0].meanX - (float)valX),2)+pow(abs(bins[0].meanY - (float)valY),2));
		for (int i=1;i<binCount;i++)
		{
			if (min > sqrt(pow(abs(bins[i].meanX - (float)valX),2)+pow(abs(bins[i].meanY - (float)valY),2)))
			{
				min = sqrt(pow(abs(bins[i].meanX - (float)valX),2)+pow(abs(bins[i].meanY - (float)valY),2));
				ind = i;
			}
		}
		bins[ind].meanX = ((bins[ind].meanX * (float)bins[ind].count)+(float)valX)/((float)bins[ind].count+1);
		bins[ind].meanY = ((bins[ind].meanY * (float)bins[ind].count)+(float)valY)/((float)bins[ind].count+1);
		bins[ind].count++;
		lnode *temp = bins[ind].bucket;
		while (temp->next) temp = temp->next;
		temp->next = new lnode;
		temp = temp->next;
		temp->dataX = valX;
		temp->dataY = valY;
	}

	ofstream fout("output");

	for (int i=0;i<binCount;i++)
	{
		fout<<i+1<<"th meanX : "<<bins[i].meanX<<endl;
		fout<<i+1<<"th meanY : "<<bins[i].meanY<<endl;
		fout<<i+1<<"th count : "<<bins[i].count<<endl;
		lnode *temp = bins[i].bucket;
		while (temp)
		{
			fout<<temp->dataX<<" "<<temp->dataY<<endl;
			temp=temp->next;
		}
		fout<<endl<<endl<<endl;
	}
	fout.close();

	return 0;
}