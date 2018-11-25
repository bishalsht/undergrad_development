#include <iostream>
#include <fstream>
using namespace std;

struct btnode{
	int index;
	int data[2];
	int parentptr;
	int nodeptr[3];
	int dataptr[2];
	btnode()
	{
		for (int i=0;i<2;i++)
		{
			data[i]=-1;
			nodeptr[i]=-1;
			dataptr[i]=-1;
		}
		nodeptr[2]=-1;
		parentptr=-1;
		index=-1;
	}
};

int integer(char a[])
{
	if (a[0]=='-')
	return -1;
	return ((static_cast<int>(a[0])-48)*10+(static_cast<int>(a[1])-48));
}

long root_search()
{
	fstream file;
	file.open("index.txt", ios::in);
	file.seekg(ios::beg);
	char b[3]={'\0'};
	long pos;
	while (!file.eof())
	{
		if (file>>b[0])
		{
			file>>b[0]>>b[0]>>b[1];
			int t=integer(b);
			if (t==-1)
			{
				pos=file.tellg();
				file.seekg(pos-4);
				pos=file.tellg();
				file.close();
				return pos;
			}
			else
			{
				pos=file.tellg();
				file.seekg(pos+14);
			}
		}
	}
	file.close();
	return -1;
}

long leaf_search(long pos, int m)
{
	fstream file;
	file.open("index.txt", ios::in);
	file.seekg(pos);
	char b[3]={'\0'},c[3]={'\0'};
	while (!file.eof())
	{
		file.seekg(pos+4);
		file>>b[0]>>b[1];
		int n=integer(b);
		if (n==-1)
		{
			file.close();
			return pos;
		}
		else
		{
			file>>c[0]>>c[1];
			int k=integer(c);
			if (k>m) pos=n*18;
			else
			{
				long pos2;
				pos2=file.tellg();
				file.seekg(pos2+2);
				file>>b[0]>>b[1]>>c[0]>>c[1];
				k=integer(c);
				if (k==-1)
				{
					n=integer(b);
					pos=n*18;					
				}
				else if (k>m)
				{
					n=integer(b);
					pos=n*18;					
				}
				else
				{
					file>>b[0]>>b[0]>>b[0]>>b[1];
					n=integer(b);
					pos=n*18;
				}
			}
		}
	}
}

void add_data(long pos,int dt)
{
	fstream file;
	file.open("index.txt", ios::in|ios::out);
	file.seekg(pos+6);
	char d[3]={'\0'},e[3]={'\0'};
	file>>d[0]>>d[1];
	int k=integer(d);
	file.seekg(pos+12);
	file>>e[0]>>e[1];
	int m=integer(e);
	if (m==-1)
	{
		if (dt>k)
		{
			file.seekp(pos+12);
			file<<dt/10<<dt%10<<"in";
		}
		else
		{
			file.seekg(pos+6);
			char a[5]={'\0'};
			file>>a[0]>>a[1]>>a[2]>>a[3];
			file.seekp(pos+6);
			file<<dt/10<<dt%10<<"in";
			file.seekp(pos+12);
			file<<a[0]<<a[1]<<a[2]<<a[3];
		}
	}
	file.close();
}

int main()
{	
	ofstream fcreate;
	//fcreate.open("index.txt");
	//fcreate.close();
	
	fstream file;
	ifstream fin;
	fin.open("datafile.txt");
	file.open("index.txt", ios::in|ios::out);
	
	while (!fin.eof())
	{
		char data[3]={'\0'};
		btnode T;
		if (fin>>data[0])
		{
			fin>>data[1];
			T.data[0]=integer(data);
			long root_pos=root_search();
			file.seekp(root_pos);
			long leaf_pos=leaf_search(root_pos, T.data[0]);
			add_data(leaf_pos, T.data[0]);
		}
	}
	
	fin.close();
	file.close();
	return 0;
}
