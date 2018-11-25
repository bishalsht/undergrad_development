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
	remove ("output.txt");
	ofstream fout;
	fout.open("file0.txt");
	fout.close();
	fout.open("file1.txt");
	fout.close();
	fout.open("file2.txt");
	fout.close();
	fstream file, file1[4];
	file.open("inp.txt", ios::in|ios::out);
	file1[0].open("file0.txt" , ios::in|ios::out);
	file1[1].open("file1.txt" , ios::in|ios::out);
	file1[2].open("file2.txt" , ios::in|ios::out);
	
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
	file1[j]<<"-1 ";
	
	file1[0].seekg(ios::beg);
	file1[1].seekg(ios::beg);

	int  k, out=2;
	j=0; k=1;
	maxm=-1;
	int q,ntned;
	int count=0;
	while (1==1)
	{
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
						file1[out]<<a[0]<<" ";
						file1[j]>>a[0];
					}				
				}
				else if ((a[1]==q||a[0]<0)&&a[1]>0)
				{
					if (maxm<a[1])
					{
						maxm=a[1];
						file1[out]<<a[1]<<" ";
						file1[k]>>a[1];
					}
				}
			}
			if (a[0]==-1&&a[1]!=-1)
			{
				do
				{
					file1[out]<<a[1]<<" ";
				}while (file1[k]>>a[1]&&a[1]!=-1);
				file1[out]<<"-1";
				maxm=0;
				break;
			}
			else if (a[1]==-1&&a[0]!=-1)
			{
				do
				{
					file1[out]<<a[0]<<" ";
				}while (file1[j]>>a[0]&&a[0]!=-1);
				file1[out]<<"-1";
				maxm=0;
				break;
			}
		}
		if (!file1[j].fail()&&!file1[k].fail()) continue;
		else if (file1[0].fail()||file1[2].fail())
		{
			file1[2].close();
			file1[1].close();
			file1[0].close();
			switch (out)
			{
				case 0: rename("file0.txt","output.txt"); break;
				case 1: rename("file1.txt","output.txt"); break;
				case 2: rename("file2.txt","output.txt"); break;
			}
			break;
		}
		else if (file1[j].fail())
		{
			int temp=j;
			j=out;
			out=temp;
			char outfi=static_cast<char>(out)+48;
			char filename[10]={'f','i','l','e',outfi,'.','t','x','t','\0'};
			file1[out].close();
			ofstream clear;
			clear.open(filename);
			clear.close();
			file1[out].open(filename , ios::in|ios::out);
			file1[j].seekg(ios::beg);
			//file1[k].seekg(file1[k].tellg()-5);
			//file1[2]>>ntned;cout<<ntned;break;
		}
		else if (file1[k].fail())
		{
			int temp=k;
			k=out;
			out=temp;
			char outfi=static_cast<char>(out)+48;
			char filename[10]={'f','i','l','e',outfi,'.','t','x','t','\0'};
			file1[out].close();
			ofstream clear;
			clear.open(filename);
			clear.close();
			file1[out].open(filename , ios::in|ios::out);
			file1[k].seekg(ios::beg);
			file1[j].seekg(file1[j].tellg()-2);
			
		}
	}
	
	return 0;
}
