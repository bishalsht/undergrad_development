#include <iostream>
#include <fstream>
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
	ofstream fout;
	fout.open("file0.txt");
	fout.close();
	fout.open("file1.txt");
	fout.close();
	fout.open("file2.txt");
	fout.close();
	fout.open("file3.txt");
	fout.close();
	fstream file, file1[4];
	file.open("inp.txt", ios::in|ios::out);
	file1[0].open("file0.txt" , ios::in|ios::out);
	file1[1].open("file1.txt" , ios::in|ios::out);
	file1[2].open("file2.txt" , ios::in|ios::out);
	file1[3].open("file3.txt" , ios::in|ios::out);
	
	file.seekg(0);
	
	int l;
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
				file1[0]<<data;
				file1[0]<<"-1 ";
			}
			else if (i==2)
			{
				if (data[0]<data[1])
				{
					if (data[0]<10) file1[0]<<'0';
					file1[0]<<data[0]<<" ";
					if (data[1]<10) file1[0]<<'0';
					file1[0]<<data[1]<<" ";
				}
				else 
				{
					if (data[1]<10) file1[0]<<'0';
					file1[0]<<data[1]<<" ";
					if (data[0]<10) file1[0]<<'0';
					file1[0]<<data[0]<<" ";
				}
				file1[0]<<"-1 ";
			}
			else 
			{
				if (data[0]<10) file1[0]<<'0';
				file1[0]<<data[0]<<" ";
				file1[0]<<"-1 ";
			}
		}
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
				file1[1]<<data;
				file1[1]<<"-1 ";
			}
			else if (i==2)
			{
				if (data[0]<data[1])
				{
					if (data[0]<10) file1[1]<<'0';
					file1[1]<<data[0]<<" ";
					if (data[1]<10) file1[1]<<'0';
					file1[1]<<data[1]<<" ";
				}
				else 
				{
					if (data[1]<10) file1[1]<<'0';
					file1[1]<<data[1]<<" ";
					if (data[0]<10) file1[1]<<'0';
					file1[1]<<data[0]<<" ";
				}
				file1[1]<<"-1 ";
			}
			else 
			{
				if (data[0]<10) file1[1]<<'0';
				file1[1]<<data[0]<<" -1 ";
			}
		}
	}
	
	file1[0].seekg(ios::beg);
	file1[1].seekg(ios::beg);
	
	int k=1, q;
	int j=0;
	bool filenum=true;
	int maxm=-1;
	int a[2];
	while (file1[j]>>a[0]&&file1[k]>>a[1])
	{
		while (a[0]!=-1&&a[1]!=-1)
		{
			if (min(a[0],a[1])!=-1)
			q=min(a[0],a[1]);
			if (a[0]==q&&a[0]>0)
			{
				if (maxm<a[0])
				{	
					maxm=a[0];
					if (filenum)
					file1[(j+2)%4]<<a[0]<<" ";
					else file1[(k+2)%4]<<a[0]<<" ";
					file1[j]>>a[0];
				}				
			}
			else if ((a[1]==q||a[0]<0)&&a[1]>0)
			{
				if (maxm<a[1])
				{
					maxm=a[1];
					if (filenum)
					file1[(j+2)%4]<<a[1]<<" ";
					else file1[(k+2)%4]<<a[1]<<" ";
					file1[k]>>a[1];
				}
			}
		}
		if (a[0]==-1&&a[1]!=-1)
		{
			do
			{
				if (filenum)
				file1[(j+2)%4]<<a[1]<<" ";
				else file1[(k+2)%4]<<a[1]<<" ";
			}while (file1[k]>>a[1]&&a[1]!=-1);
			if (filenum)
			file1[(j+2)%4]<<"-1";
			else file1[(k+2)%4]<<"-1";
			filenum=!filenum;
			maxm=0;
		}
		else if (a[1]==-1&&a[0]!=-1)
		{
			do
			{
				if (filenum)
				file1[(j+2)%4]<<a[0]<<" ";
				else file1[(k+2)%4]<<a[0]<<" ";
			}while (file1[j]>>a[0]&&a[0]!=-1);
			if (filenum)
			file1[(j+2)%4]<<"-1";
			else file1[(k+2)%4]<<"-1";
			filenum=!filenum;
			maxm=0;
		}
		if (a[0]==-1)
		file1[j]>>a[0];
		else file1[k]>>a[1];
		if (!file1[j]>>a[0]||a[1]!=-1)
		{
			do
			{
				if (filenum)
				file1[(j+2)%4]<<a[1]<<" ";
				else file1[(k+2)%4]<<a[1]<<" ";
			}while (file1[j]>>a[1]);
			if (filenum)
			file1[(j+2)%4]<<"-1";
			else file1[(k+2)%4]<<"-1";
		}
		if (!file1[k]>>a[1]||a[0]!=-1)
		{
			do
			{
				if (filenum)
				file1[(j+2)%4]<<a[0]<<" ";
				else file1[(k+2)%4]<<a[0]<<" ";
			}while (file1[j]>>a[0]);
			if (filenum)
			file1[(j+2)%4]<<"-1";
			else file1[(k+2)%4]<<"-1";
		}
		j=(j+2)%4;
		k=(k+2)%4;
		ofstream clear;
		if (j==2)
		{
			file1[0].close();
			clear.open("file0.txt");
			clear.close();
			file1[0].open("file0.txt", ios::trunc);
		}
		else
		{
			file1[2].close();
			clear.open("file2.txt");
			clear.close();
			file1[2].open("file2.txt");
		}
		if (k==3)
		{
			file1[1].close();
			clear.open("file1.txt");
			clear.close();
			file1[1].open("file1.txt");
		}
		else
		{
			file1[3].close();
			clear.open("file3.txt");
			clear.close();
			file1[3].open("file3.txt");
		}
		file1[0].seekg(ios::beg);
		file1[1].seekg(ios::beg);
		file1[2].seekg(ios::beg);
		file1[3].seekg(ios::beg);
	}
	
	if (j==0)
	{
		if (file1[0]>>a[0])
		{
			file1[0].seekg(file1[0].tellg()-1);
			j=0;
		}
		else j=1;
	}
	else
	{
		if (file1[2]>>a[0])
		{
			file1[2].seekg(file1[2].tellg()-1);
			j=2;
		}
		else j=3;
	}
	
	ofstream out("output.txt");
	file1[j].seekg(ios::beg);
	while (file1[j]>>a[0])
	out<<a[0]<<" ";
	out.close();
	
	file.close();
	file1[0].close();
	file1[1].close();
	file1[2].close();
	file1[3].close();
	
	return 0;
}
