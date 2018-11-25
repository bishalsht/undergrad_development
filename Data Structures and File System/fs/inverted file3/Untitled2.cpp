#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

int main()
{
	ofstream fout,fout1,fout2;
	ifstream fin,fin1,fin2;
	
	remove("data.txt");
	remove("index.txt");
	
	fout.open("data.txt");
	char course[4],roll[4],name[10],line[100];
	int pos=0;
	
	bool fini=false;
	
	while (!fini)
	{
		cout<<"Roll:";
		cin>>roll;
		if (roll[0]=='$') break;
		cout<<"course:";
		cin>>course;
		
		fout<<roll<<course<<endl;
		
		fin.open("index.txt");
		if (fin.fail())
		{
			fout2.open("index.txt");
			fout2<<roll;
			if (pos<10) fout2<<'0';
			fout2<<pos++<<endl;
			fout2.close();
		}
		else
		{
			fout2.open("buffer.txt");
			while (fin.getline(line,99,'\n'))
			{
				if (strncmp(line,roll,3)<0)
				fout2<<line<<endl;
				else break;
			}
			if (!strncmp(line,roll,3))
			{
				fout2<<line;
				if (pos<10) fout2<<'0';
				fout2<<pos++<<endl;
			}
			else
			{
				fout2<<roll;
				if (pos<10) fout2<<'0';
				fout2<<pos++<<endl;
				fout2<<line;
			}
			while(!fin.eof())
			{
				if (fin.getline(line,99,'\n'))
				fout2<<line<<endl;
			}
			fout2.close();
			fin.close();
			remove("index.txt");
			rename("buffer.txt","index.txt");
		}
	}
	fout.close();
	
	for (int x=0;x<100;x++)
	line[x]='\0';
	fin.open("index.txt");
	cout<<"Enter roll no.:";
	cin>>roll;
	cout<<roll<<": ";
	fin.getline(line,99,'\n');
	while (strncmp(line,roll,3)<0)
	fin.getline(line,99,'\n');
	if (!strncmp(line,roll,3))
	{
		for (int i=3;line[i]!='\0';i=i+2)
		{
			char a[3];
			fin2.open("data.txt");
			int b=static_cast<long>(line[i]-48)*10+static_cast<long>(line[i+1]-48);
			fin2.seekg(b*8+3);
			fin2>>a[0]>>a[1]>>a[2];
			cout<<a<<" ";
			fin2.close();
		}
	}
	fin.close();
	
	return 0;
}
