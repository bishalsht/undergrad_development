#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

int j=0,n=5,x=1,maxm=0;
int a[12]={58,48,82,31,27,46,18,89,53,66,87,59};

int minimum(int x, int y, int z)
{
	if (x<y&&x<z) return x;
	else if (y<x&&y<z) return y;
	else return z;
}

void selection(int tree[],int index)
{
	if (index<3)
	{
		selection(tree,2*index+1);
		selection(tree,2*index+2);
		if (2*index+1<7)
		{
			if (tree[index*2+1]>maxm&&tree[index*2+2]>maxm)
			tree[index]=min(tree[index*2+1],tree[index*2+2]);
			else if (tree[index*2+1]>maxm)
			tree[index]=tree[index*2+1];
			else if (tree[index*2+2]>maxm)
			tree[index]=tree[index*2+2];
			else tree[index]=-1;
		}
	}
}

int data_remove(int tree[],int index)
{
	if (index<7)
	{
		int i;
		if (tree[index*2+1]==tree[0]) i=data_remove(tree, index*2+1);
		if (tree[index*2+2]==tree[0]) i=data_remove(tree, index*2+2);
		tree[index]=-1;
		if (index>=3)
		return (index);
		else return i;
	}
}

int main()
{
	int tree[7]={-1,-1,-1,-1,-1,-1,-1};
	ofstream fout("file0.txt");
	fout.close();
	fout.open("file1.txt");
	fout.close();
	fout.open("file2.txt");
	fout.close();
	fout.open("file3.txt");
	fout.close();
	
	fstream file[4];
	file[0].open("file0.txt");
	file[1].open("file1.txt");
	file[2].open("file2.txt");
	file[3].open("file3.txt");
	
	for (int i=0;i<4;i++)
	tree[i+3]=a[j++];
	int m=0;
	bool fin=false;
	while (!fin)
	{
		selection(tree,0);
		if (tree[0]>0)
		maxm=tree[0];
		if (tree[0]>-1)
		{
			file[m]<<tree[0]<<" ";
			int q=data_remove(tree,0);
			if (j<12)
			{
				if (a[j]>maxm)
				tree[q]=a[j++];
				else
				tree[q]=-a[j++];
			}
			else if (j<15) j++;
			else fin=!fin;
		}
		else
		{
			maxm=0;
			file[m]<<"-1 ";
			m=(m+1)%2;
			for (int i=0;i<4;i++)
			tree[i+3]=-tree[i+3];
		}
	}
	file[m]<<"-1";
	
	file[0].seekg(ios::beg);
	file[1].seekg(ios::beg);

	int k=1, q;
	j=0;
	bool filenum=true;
	maxm=-1;
	while (file[j]>>a[0]&&file[k]>>a[1])
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
					file[(j+2)%4]<<a[0]<<" ";
					else file[(k+2)%4]<<a[0]<<" ";
					file[j]>>a[0];
				}				
			}
			else if ((a[1]==q||a[0]<0)&&a[1]>0)
			{
				if (maxm<a[1])
				{
					maxm=a[1];
					if (filenum)
					file[(j+2)%4]<<a[1]<<" ";
					else file[(k+2)%4]<<a[1]<<" ";
					file[k]>>a[1];
				}
			}
		}
		if (a[0]==-1&&a[1]!=-1)
		{
			do
			{
				if (filenum)
				file[(j+2)%4]<<a[1]<<" ";
				else file[(k+2)%4]<<a[1]<<" ";
			}while (file[k]>>a[1]&&a[1]!=-1);
			if (filenum)
			file[(j+2)%4]<<"-1";
			else file[(k+2)%4]<<"-1";
			filenum=!filenum;
			maxm=0;
		}
		else if (a[1]==-1&&a[0]!=-1)
		{
			do
			{
				if (filenum)
				file[(j+2)%4]<<a[0]<<" ";
				else file[(k+2)%4]<<a[0]<<" ";
			}while (file[j]>>a[0]&&a[0]!=-1);
			if (filenum)
			file[(j+2)%4]<<"-1";
			else file[(k+2)%4]<<"-1";
			filenum=!filenum;
			maxm=0;
		}
		if (a[0]==-1)
		file[j]>>a[0];
		else file[k]>>a[1];
		if (!file[j]>>a[0]||a[1]!=-1)
		{
			do
			{
				if (filenum)
				file[(j+2)%4]<<a[1]<<" ";
				else file[(k+2)%4]<<a[1]<<" ";
			}while (file[j]>>a[1]);
			if (filenum)
			file[(j+2)%4]<<"-1";
			else file[(k+2)%4]<<"-1";
		}
		if (!file[k]>>a[1]||a[0]!=-1)
		{
			do
			{
				if (filenum)
				file[(j+2)%4]<<a[0]<<" ";
				else file[(k+2)%4]<<a[0]<<" ";
			}while (file[j]>>a[0]);
			if (filenum)
			file[(j+2)%4]<<"-1";
			else file[(k+2)%4]<<"-1";
		}
		j=(j+2)%4;
		k=(k+2)%4;
		ofstream clear;
		if (j==2)
		{
			file[0].close();
			clear.open("file0.txt");
			clear.close();
			file[0].open("file0.txt", ios::trunc);
		}
		else
		{
			file[2].close();
			clear.open("file2.txt");
			clear.close();
			file[2].open("file2.txt");
		}
		if (k==3)
		{
			file[1].close();
			clear.open("file1.txt");
			clear.close();
			file[1].open("file1.txt");
		}
		else
		{
			file[3].close();
			clear.open("file3.txt");
			clear.close();
			file[3].open("file3.txt");
		}
		file[0].seekg(ios::beg);
		file[1].seekg(ios::beg);
		file[2].seekg(ios::beg);
		file[3].seekg(ios::beg);
	}
	
	file[0].close();
	file[1].close();
	file[2].close();
	file[3].close();
	
	return 0;
}
