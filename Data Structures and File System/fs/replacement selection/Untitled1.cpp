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
	file1<<'0'<<a[0];
	else file1<<a[0];
	if (a[1]<10)
	file1<<'0'<<a[1];
	else file1<<a[1];
	if (a[2]<10)
	file1<<'0'<<a[2];
	else file1<<a[2];
}

int main()
{
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
	
	int l,j=0;
	int a[3]={0};

	file>>a[0]>>a[1]>>a[2];
	int maxm;
	while (!file.eof())
	{
		sort(a);
		maxm=a[0];
		for (int i=0;i<3;i++)
		{
			file1[j]<<a[i]<<" ";
			while (file>>a[i])
			{
				sort(a);
				if (a[i]<maxm)
				{
					a[i]=-a[i];
					break;
				}
				file1[j]<<a[i]<<" ";
				maxm=a[i];			
			}
			if (file.eof())
			maxm=a[i];
		}
		a[0]=-a[0];
		a[1]=-a[1];
		a[2]=-a[2];	
		file1[j]<<"-1 ";	
		j=(j+1)%2;
	}
	a[2]=-a[2];
	sort(a);
	if (a[0]!=maxm)
	file1[j]<<a[0]<<" ";
	if (a[1]!=maxm)
	file1[j]<<a[1]<<" ";
	if (a[2]!=maxm)
	file1[j]<<a[2]<<" ";

	
	file1[0].seekg(ios::beg);
	file1[1].seekg(ios::beg);

	int k=1, q;
	j=0;
	bool filenum=true;
	maxm=-1;
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
	
	return 0;
}
