#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

void pack(char a[], char b[])
{
	strcat(a,b);
	strcat(a,"$");
}

class student{
	public:
	char name[25];
	char rno[7];
	char gpa[5];
};

int main()
{
	ifstream fin;
	ofstream fout;
	fout.open("data.txt");
	char ans;
	do{
        student s1;
		char buffer[100]={'\0'};
		cout<<"Name :";
		cin>>s1.name;
		cout<<"Roll Number :";
		cin>>s1.rno;
		cout<<"CGPA :";
		cin>>s1.gpa;
		pack (buffer,s1.name);
		pack (buffer,s1.rno);
		pack (buffer,s1.gpa);
		fout<<buffer;
		cout<<"Enter more data ? (y or n)";
		cin>>ans;
	} while (ans=='y'||ans=='Y');
	fout.close();
	
	fin.open("data.txt");
	int cgpa_fin=0;
	char got[50]={'\0'};
	double cgpa_list[50]={0};
	int m=0;
	while (fin.getline(got,50,'$'))
	{
		if (cgpa_fin==0)
		cgpa_fin=1;
		else if (cgpa_fin==1)
		cgpa_fin=2;
		else if (cgpa_fin==2)
		{
			cgpa_fin=0;
			for (int k=0;k<4;k++)
			{
				if (k==0)
				cgpa_list[m]=cgpa_list[m]+static_cast<double>(got[k])-48;
				else if (k==2)
				cgpa_list[m]=cgpa_list[m]+((static_cast<double>(got[k])-48)/10);
				else if (k==3)
				cgpa_list[m]=cgpa_list[m]+((static_cast<double>(got[k])-48)/100);
			}
			m++;
		}
		for (int i=0;i<50;i++)
		got[i]='\0';
	}
	
	for (int i=0;cgpa_list[i+1]!='\0';i++)
	{
		for (int c=0;cgpa_list[c+1]!='\0';c++)
		{
			if (cgpa_list[c]<cgpa_list[c+1])
			{
				double tempo;
				tempo=cgpa_list[c];
				cgpa_list[c]=cgpa_list[c+1];
				cgpa_list[c+1]=tempo;
			}
		}
	}
	fin.close();
	
	fin.open("data.txt");
	fout.open("index.txt");
	double cgpa;
	while (fin.getline(got,50,'$'))
	{
		if (cgpa_fin==0)
		cgpa_fin=1;
		else if (cgpa_fin==1)
		cgpa_fin=2;
		else if (cgpa_fin==2)
		{
			cgpa_fin=0;
			for (int k=0;k<4;k++)
			{
				if (k==0)
				cgpa=cgpa+static_cast<double>(got[k])-48;
				else if (k==2)
				cgpa=cgpa+((static_cast<double>(got[k])-48)/10);
				else if (k==3)
				cgpa=cgpa+((static_cast<double>(got[k])-48)/100);
			}
			m++;
			for (int i=0;cgpa_list[i]!='\0';i++)
			{
				if (cgpa_list[i]==cgpa)
				{
					fout<<cgpa<<"$"<<i<<"$";
					cgpa_list[i]=999;
					break;
				}
			}
			cgpa=0;
		}
		for (int i=0;i<50;i++)
		got[i]='\0';
	}
	fout.close();
	fin.close();
	
	char ans2;
	do{
		fin.open("index.txt");
		int count=0;
		char index, compare;
		cout<<"Enter the index whose data you want to see :";
		cin>>index;
		while (fin>>compare)
		{
			if (compare==index)
			break;
			if (compare=='$')
			count++;
		}
		fin.close();
		
		fin.open("data.txt");
		count=(count/2)*3;
		for(int i=0;i<count;i++)
		{
			char print[50];
			fin.getline(print,50,'$');
		}
		for (int i=0;i<3;i++)
		{
			if (i==0)
			cout<<"Name: ";
			else if (i==1)
			cout<<"Roll No.: ";
			else if (i==2)
			cout<<"CGPA: ";
			char print[50];
			fin.getline(print,50,'$');
			cout<<print<<endl;
		}
		fin.close();
		cout<<"More data access ? (y or n) :";
		cin>>ans2;
	} while (ans2=='y'||ans2=='Y');
	
	return 0;
}
