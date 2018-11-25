#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

void pack(char b[],char c[]){
	strcat(b,c);
	strcat(b,"$");
}

union addr{
	char salary1[8],salary2[20];
};

class employee{
	public:
		char id[5], name[50], des[20];
		int tag;
		addr salary;
		int numofson;
};

class son{
	public:
	char name[20],age[2];
};

int main()
{
	ofstream fout("employee.txt");
	char ans;
	int i,n;
	employee e;
	son s[10];
	do{
		cout<<"Enter emp id:";
		cin>>e.id;
		cout<<"Enter name:";
		cin>>e.name;
		cout<<"Enter designation:";
		cin>>e.des;
		cout<<"Enter 1 for permanent and 0 temporary:";
		cin>>e.tag;
		cout<<"Enter salary:";
		if (e.tag==1)
		cin>>e.salary.salary2;
		else cin>>e.salary.salary1;
		cout<<"Enter number of sons:";
		cin>>e.numofson;
		if (e.numofson>=1)
		{
			for (i=0;i<e.numofson;i++)
			{
				cout<<"Name of son:";
				cin>>s[i].name;
				cout<<"age:";
				cin>>s[i].age;
			}
		}
		char buffer[100]="";
		pack(buffer,e.id);
		pack(buffer,e.name);
		pack(buffer,e.des);
		if (e.tag==1)
		pack(buffer,e.salary.salary1);
		else pack(buffer,e.salary.salary2);
		for (i=0;i<e.numofson;i++)
		{
			pack(buffer,s[i].name);
			pack(buffer,s[i].age);
		}
		fout<<buffer;
		cout<<"Enter other information? (y or n)";
		cin>>ans;
	} while (ans=='y'||ans=='Y');
	fout.close();
	char got[30];
	ifstream fin("employee.txt");
	while (fin.getline(got,50,'$'))
	cout<<got<<endl;
	fin.close();
	return 0;
}
