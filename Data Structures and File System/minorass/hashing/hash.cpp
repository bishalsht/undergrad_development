#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

int main()
{
	char roll[4]={'\0'};
	ofstream fout;
	ifstream fin;
	fout.open("hash.txt");
	fin.open("hash.txt");
	for(int i=0;i<10;i++)
	fout<<"$$$";
	cout<<"3 digit:";
	cin>>roll;
	while (roll[0]!='$')
	{
		fout.seekp(ios::beg);
		int m=static_cast<int>(roll[2])-48;
		for (int i=0;i<m;i++)
		{
			long pos=fout.tellp();
			fout.seekp(pos+3);
		}
		long pos2=fout.tellp();
		fin.seekg(pos2);
		char check;
		fin>>check;
		if (check=='$')
		fout<<roll;
		else
		{
			while (check!='$')
			{
				pos2=fin.tellg();
				fin.seekg(pos2+2);
				fin>>check;
				if (fin.eof())
				{
					cout<<"Hash is full.";
					break;
				}
				else if (check=='$')
				{
					pos2=fin.tellg();
					fout.seekp(pos2-1);
					fout<<roll;
				}
			}
		}
		cout<<"3 digit: ";
		cin>>roll;
	}
	fout.close();
	fin.close();
	return 0;
}
