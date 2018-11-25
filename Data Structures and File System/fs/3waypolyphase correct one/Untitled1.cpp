#include <iostream>
#include <fstream>

using namespace std;

int minimum(int data[],int out)
{
	int min=999, index=out;
	for (int i=0;i<4;i++)
	{
		if (data[i]<min&&i!=out&&data[i]!=-1) 
		{
			min=data[i];
			index=i;
		}
	}
	if (min!=999)
	return index;
	return min;
}

int it(int iteration[])
{
	int min=99,ind=0;
	for (int i=0;i<4;i++)
	{
		if (iteration[i]<min&&iteration[i]!=0)
		{
			min=iteration[i];
			ind=i;
		}
	}
	int count=0;
	for (int i=0;i<4;i++)
	{
		if (iteration[i]!=0)
		{
			iteration[i]=iteration[i]-min;
			if (iteration[i]!=0)
			count++;
		}
		else
		{
			iteration[i]=min;
			count++;
		}
	}
	return count;
}

int main()
{
	fstream file[4];
	ifstream fin("input.txt");
	if (fin.fail()) return 0;
	ofstream fout;
	fout.open("file0.txt"); fout.close();
	fout.open("file1.txt"); fout.close();
	fout.open("file2.txt"); fout.close();
	fout.open("file3.txt"); fout.close();
	
	for (int i=0;i<4;i++) 
	{
		char val='0'+i;
		char filename[10]={'f','i','l','e',val,'.','t','x','t','\0'};
		file[i].open(filename, ios::in|ios::out);
		if (file[i].fail()) cout<<i<<"fail vayo";
	}
	
	for (int i=0;i<7;i++)
	{
		int a;
		fin>>a;
		file[0]<<a<<" -1 ";
	}
	for (int i=0;i<6;i++)
	{
		int a;
		fin>>a;
		file[1]<<a<<" -1 ";
	}
	for (int i=0;i<4;i++)
	{
		int a;
		fin>>a;
		file[2]<<a<<" -1 ";
	}
	
	for (int i=0;i<4;i++) 
	{
		file[i].seekp(ios::beg);
	}
	
	int out=3;
	int data[4];
	file[0]>>data[0];
	file[1]>>data[1];
	file[2]>>data[2];
	data[out]=-1;
	int q;
	
	int iteration[4]={7,6,4,0};
	bool finish=false;
	
	while (!finish)
	{
		if (it(iteration)==1) finish=true;
		while (!file[(out+1)%4].fail()&&!file[(out+2)%4].fail()&&!file[(out+3)%4].fail())
		{
			q=minimum(data,out);
			if (q!=999)
			{
				file[out]<<data[q]<<" ";
				file[q]>>data[q];
			}
			else
			{
				file[out]<<"-1 ";
				file[(out+1)%4]>>data[(out+1)%4];
				file[(out+2)%4]>>data[(out+2)%4];
				file[(out+3)%4]>>data[(out+3)%4];
			}
		}
		for (int i=0;i<4;i++)
		{
			if (i!=out&&file[i].fail())
			{
				file[out].seekg(ios::beg);
				file[out]>>data[out];
				out=i;
				char val=out+'0';
				file[out].close();
				char filename[10]={'f','i','l','e',val,'.','t','x','t'};
				fout.open(filename);
				fout.close();
				file[out].open(filename, ios::in|ios::out);
			}
		}
	}
	
	int z;
	for (z=0;z<4;z++)
	{
		if (iteration[z]==1)
		break;
	}
	
	char filenm[10]={'f','i','l','e',z+'0','.','t','x','t','\0'};
	
	for (int i=0;i<4;i++) file[i].close();
	remove("output.txt");
	rename(filenm,"output.txt");
	
	return 0;
}
