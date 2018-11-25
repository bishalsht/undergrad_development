#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct reg{
	char index[3],cname[3],bindex[3], findex[3], sRoll[4], backindex[3], forindex[3];
};

struct student{
	char Name[10], rollNo[4], index[4];
};

struct course{
	char abbrName[3], cName[10], index[4];
};

int main()
{
	ofstream fout,out,outfile;
	ifstream fin,in,infile;
	fout.open("student.txt");
	out.open("course.txt");
	outfile.open("reg.txt");
	fin.open("student.txt");
	in.open("course.txt");
	infile.open("reg.txt");
	
	int ri=0;
	char a;
	bool sfound=0, cfound=0;
	
	student s;
	reg r;
	course c;
	int b=0;
	
	while (b==0)
	{
		cout<<"Enter name:";
		cin>>s.Name;
		if (s.Name[0]=='$')
		break;
		cout<<"Enter roll no.";
		cin>>s.rollNo;
		fin.seekg(ios::beg);
		while (!fin.eof())
		{
			cout<<1;
			long pos=fin.tellg();
			cout<<pos;
			fin.seekg(pos-1);
			cout<<fin.tellg();
			char name[40]={'\0'};
			fin.getline(name,40,'$');
			if (name[0]==s.rollNo[0]&&name[2]==s.rollNo[2]&&name[1]==s.rollNo[1])
			{
				sfound=1;
				break;
			}
			else 
			fin.getline(name,30,'$');
			fin.getline(name,20,'$');
		}
		
		if (!sfound)
		fout<<s.rollNo<<'$'<<s.Name<<'$'<<ri<<'$';
		sfound=0;
		
		while (b==0)
		{
			cout<<"subject: ";
			cin>>c.cName;
			if (c.cName[0]=='$')
			break;
			cout<<"subject code (2): ";
			cin>>c.abbrName;
			in.seekg(0);
			while (in>>a)
			{
				if (a==c.abbrName[0])
				{
					in>>a;
					if (a==c.abbrName[1])
					{
						cfound=1;
						break;
					}
				}
			}
			if (!cfound)
			out<<c.abbrName<<'$'<<c.cName<<'$'<<ri<<'$';
			cfound=0;
			if (ri>9)
			outfile<<ri++;
			else
			outfile<<'+'<<ri++;
			outfile<<c.abbrName<<"-1-1"<<s.rollNo<<"-1-1";
		}
	}
	
	in.close();
	fin.close();
	infile.close();
	fout.close();
	out.close();
	outfile.close();
	
	return 0;
}
