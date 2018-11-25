//A input file named inp.txt is required.

#include <iostream>
#include <fstream>

#define d 3

using namespace std;

int integer(char a[])
{
	return ((static_cast<int>(a[0])-48)*10+(static_cast<int>(a[1])-48));
}

void sort(int a[])
{
	for (int i=0;i<3;i++)
	{
		for (int j=0;j<2;j++)
		{
			if (a[j]>a[j+1])
			{
				a[j]=a[j]+a[j+1];
				a[j+1]=a[j]-a[j+1];
				a[j]=a[j]-a[j+1];
			}
		}
	}
}

int minimum(int a[])
{
	int mini=0;
	while (a[mini]<0&&mini<d)
	mini++;
	for (int i=0;i<d;i++)
	{
		if (a[i]<a[mini]&&a[i]>0)
		mini=i;
	}
	if (mini<d&&a[mini]>0)
	return mini;
	else return -1;
}

fstream &operator<<(fstream &file1,int a[])
{
	if (a[0]<10)
	file1<<'0'<<a[0]<<" ";
	else file1<<a[0]<<" ";
	if (a[1]<10)
	file1<<'0'<<a[1]<<" ";
	else file1<<a[1]<<" ";
	if (a[2]<10)
	file1<<'0'<<a[2]<<" ";
	else file1<<a[2]<<" ";
}

int main()
{
	int data[3];
	char filename[10]={'f','i','l','e','0','.','t','x','t','\0'};
	ofstream fout;
	char o=d+'0';
	for (char c='0';c<d+o;c++)
	{
		fout.open(filename);
		fout.close();
		filename[4]=c+1;
	}
	fstream file, file1[2*d];
	file.open("inp.txt", ios::in|ios::out);
	if (file.fail())
	{
		cout<<"inp.txt file not found";
		return 0;
	}
	for (int c=0;c<2*d;c++)
	{
	 	filename[4]='0'+c;
		file1[c].open(filename, ios::in|ios::out);
	}
	
	file.seekg(0);
	
	int l;
	int current=0;
	char b[3]={'\0'};
	
	while (!file.eof())
	{
		if (file>>b[0])
		{
			int i;
			file.seekg(file.tellg()-1);
			for (i=0;i<3;i++)
			{
				if (file>>b[0]>>b[1])
				{
					int p=integer(b);
					data[i]=p;
				}
				else break;
			}
			if (i==3)
			{
				sort(data);
				file1[current]<<data;
				file1[current]<<"-1 ";
			}
			else if (i==2)
			{
				if (data[0]<data[1])
				{
					if (data[0]<10) file1[current]<<'0';
					file1[current]<<data[0]<<" ";
					if (data[1]<10) file1[current]<<'0';
					file1[current]<<data[1]<<" ";
				}
				else 
				{
					if (data[1]<10) file1[current]<<'0';
					file1[current]<<data[1]<<" ";
					if (data[0]<10) file1[current]<<'0';
					file1[current]<<data[0]<<" ";
				}
				file1[current]<<"-1 ";
			}
			else 
			{
				if (data[0]<10) file1[current]<<'0';
				file1[current]<<data[0]<<" ";
				file1[current]<<"-1 ";
			}
		}
		current=(current+1)%d;
	}
	
	for (int i=0;i<d;i++)
	file1[i].seekg(ios::beg);
	
	int y;
	int a[d];
	for (int b=0;b<d;b++)
	a[b]=-1;
	bool filenum=true, fin=false;
	
	while (!fin)
	{
		if (filenum)
		{
			for (y=0;y<d;y++)
			{
				if (file1[y]>>a[y]);
				else break;
			}
			if (y==1)
			{
				fin=!fin;
				break;
			}
		}
		else
		{
			for (y=d;y<2*d;y++)
			{
				if (file1[y]>>a[y-d]);
				else break;
			}
			if (y==d+1)
			{
				fin=!fin;
				break;
			}
		}
		bool fin2=false;
		int c=0;
		while (!fin2)
		{
			int mini;
			mini=minimum(a);
			while (mini!=-1)	
			{
				if (filenum)
				{
					file1[d+c]<<a[mini]<<" ";
					file1[mini]>>a[mini];
				}
				else
				{
					file1[c]<<a[mini]<<" ";
					file1[mini+d]>>a[mini];
				}
				mini=minimum(a);
			}
			if (filenum) file1[d+c]<<"-1 ";
			else file1[c]<<"-1 ";
			c++;
			for (int b=0;b<d;b++)
			a[b]=-1;
			if (filenum)
			{
				for (y=0;y<d;y++)
				{
					if (file1[y]>>a[y]);
					else break;
				}
				if (y==0)
				{
					fin2=!fin2;
					break;
				}
			}
			else
			{
				for (y=d;y<2*d;y++)
				{
					if (file1[y]>>a[y-d]);
					else break;
				}
				if (y==d)
				{
					fin2=!fin2;
					break;
				}
			}
		}
		if (filenum)
		{
			for (int i=0;i<d;i++)
			{
				file1[i].close();
				ofstream clear;
				filename[4]='0'+i;
				clear.open(filename);
				clear.close();
				file1[i].open(filename, ios::in|ios::out);
			}
		}
		else
		{
			for (int i=d;i<2*d;i++)
			{
				file1[i].close();
				ofstream clear;
				filename[4]='0'+i;
				clear.open(filename);
				clear.close();
				file1[i].open(filename, ios::in|ios::out);
			}
		}
		if (!filenum)
		{
			for (int i=0;i<d;i++)
			file1[i].seekg(ios::beg);
		}
		else
		{
			for (int i=d;i<2*d;i++)
			file1[i].seekg(ios::beg);			
		}
		filenum=!filenum;
	}
	
	file.close();
	file1[0].close();
	file1[1].close();
	file1[2].close();
	file1[3].close();
	remove("output.txt");
	if (filenum)
	rename("file0.txt", "output.txt");
	else
	{
		filename[4]=d+48;
		rename(filename,"output.txt");
	}
	
	return 0;
}
