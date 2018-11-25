#include <iostream>
#include <fstream>
using namespace std;

struct bstnode{
	int lchild,rchild,data,index;
};

int main()
{
	ofstream fout;
	fout.open("bstfile.txt");
	ifstream fin;
	bstnode b[5];
	int d;
	for (int i=0;i<5;i++)
	{
		cout<<"data: ";
		cin>>d;
		b[i].data=d;
		b[i].index=i;
		b[i].lchild=-1;
		b[i].rchild=-1;
		if (i>0)
		{
			for (int m=0,j=0;m==0;)
			{
				if (b[j].data>d)
				{
					if (b[j].lchild==-1)
					{
						b[j].lchild=i;
						break;
					}
					j=b[j].lchild;
				}
				else if (b[j].data<d)
				{
					if (b[j].rchild==-1)
					{
						b[j].rchild=i;
						break;
					}
					j=b[j].rchild;
				}
			}
		}
	}
	
	for (int i=0;i<5;i++)
	fout<<b[i].lchild<<" "<<b[i].data<<" "<<b[i].index<<" "<<b[i].rchild<<endl;
	fout.close();
	
	return 0;
}
