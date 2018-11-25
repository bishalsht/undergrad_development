#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class student{
	public:
		char roll[4], name[10], index[3];
		student()
		{
			for (int i=0;i<4;i++)
			roll[i]='\0';
			for (int i=0;i<10;i++)
			name[i]='\0';
			for (int i=0;i<3;i++)
			index[i]='\0';
		}
};

class course{
	public:
		char cname[4], name[10], index[3];
		course()
		{
			for (int i=0;i<4;i++)
			cname[i]='\0';
			for (int i=0;i<10;i++)
			name[i]='\0';
			for (int i=0;i<3;i++)
			index[i]='\0';
		}
};

struct reg{
	public:
		char index[3], roll[4], stfrwd[3], stbckwrd[3], cname[4], cfrwd[3], cbckwrd[3];
		reg()
		{
			for (int i=0;i<4;i++)
			{roll[i]='\0'; cname[i]='\0';}
			for (int i=0;i<3;i++)
			index[i]='\0';
			stfrwd[0]='-';stfrwd[1]='1';stfrwd[2]='\0';
			stbckwrd[0]='-';stbckwrd[1]='1';stbckwrd[2]='\0';
			cfrwd[0]='-';cfrwd[1]='1';cfrwd[2]='\0';
			cbckwrd[0]='-';cbckwrd[1]='1';cbckwrd[2]='\0';
		}
};

int main()
{
	ofstream fout,out,outfile;
	ifstream fin,in,infile;
	student s;
	course c;
	reg r;
	int j=0;
	
	fout.open ("index.txt");
	out.open("student.txt");
	outfile.open("course.txt");
		fin.open("index.txt");
	bool finis=1;char a;
	while (finis==1)
	{
		cout<<"Enter name: ";
		cin>>s.name;
		cout<<"Enter roll(3): ";
		cin>>s.roll;
		for (int i=0;i<3;i++)
		r.roll[i]=s.roll[i];
		cout<<"Enter course(3): ";
		cin>>c.cname;
		for (int i=0;i<4;i++)
		r.cname[i]=c.cname[i];
		cout<<"Enter full name: ";
		cin>>c.name;
		r.index[0]=static_cast<char>(j/10)+48;
		r.index[1]=static_cast<char>(j%10)+48;
		
		//check reg file for student
		fin.seekg(ios::beg);
		char ind[3]={'\0'},stroll[4]={'\0'},strfr[3]={'\0'},stnull[3]={'-','1','\0'};
		bool stfound=0;
		while (!fin.eof())
		{
			if (fin>>a)
			{
				cout<<1;
				long pos=fin.tellg();
				fin.seekg(pos-1);
				fin>>ind[0]>>ind[1]>>stroll[0]>>stroll[1]>>stroll[2];
				if (strcmp(stroll,r.roll)==0)
				{	
					pos=fin.tellg();
					fin.seekg(pos+2);
					fin>>strfr[0]>>strfr[1];
					if (strcmp(strfr,stnull)==0)
					{
						stfound=1;
						pos=fin.tellg();
						long pos2=fout.tellp();
						fout.seekp(pos-2);
						fout<<r.index;
						fout.seekp(pos2);
						r.stbckwrd[0]=ind[0];
						r.stbckwrd[1]=ind[1];
					}
					fin.seekg(pos-4);
				}
				pos=fin.tellg();
				fin.seekg(pos+12);
			}
		}
		
	
		fout<<r.index<<r.roll<<r.stbckwrd<<r.stfrwd<<r.cname<<r.cbckwrd<<r.cfrwd;
		
	
		cout<<"Enter again?(1 or 0)";
		cin>>finis;
		j++;
	}
	fin.close();
	fout.close();
	out.close();
	outfile.close();
	return 0;
}
