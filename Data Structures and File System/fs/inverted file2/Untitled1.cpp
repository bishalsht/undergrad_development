#include <iostream>
#include <fstream>
using namespace std;

struct data{
	char name[20]={'\0'};
	char course[3]={'\0'};
	int roll=-1;
};

int i=0;

bool input(data &dt)
{
	cout<<"Name($ to stop):";
	cin>>dt.name;
	if (dt.name[0]=='$')
	return false;
	cout<<"Course:";
	cin>>dt.course;
	cout<<"Roll(3 digits):";
	cin>>dt.roll;
	return true;
}

void empty(data &dt)
{
	int j;
	for (j=0;j<20;j++)
	dt.name[j]='\0';
	for (j=0;j<3;j++)
	dt.course[j]='\0';
	dt.roll=-1;
}

long roll_find(fstream &file,int d1)
{
	file.seekg(ios::beg); char b; file.seekp(ios::beg);
	while (!file.eof())
	{
		if (file>>b)
		{
			char a[100]={'\0'};
			file.getline(a,100,'\n');for (int l=0;a[l]!='\0';l++) cout<<a[l];
			int x=static_cast<int>(a[0]-48)*10+static_cast<int>(a[1]-48)+static_cast<int>(b-48)*100;cout<<" "<<x<<" ";
			if (x==d1)
			{
				file.seekp(file.tellg()-1);
				file<<'$'<<i++<<'$'<<endl;
				return 0;
			}
		}
	}
	return -1;
}

int main()
{
	ofstream create;
	create.open("r_index.txt"); create.close();
	create.open("data.txt"); create.close();
	create.open("buffer.txt"); create.close();
	
	fstream file[3];
	file[0].open("data.txt", ios::in|ios::out);
	file[1].open("r_index.txt", ios::in|ios::out);
	
	data dt;
	input(dt);
	file[0]<<dt.name<<'$'<<dt.course<<'$'<<dt.roll<<'$'<<endl;
	file[1]<<dt.roll<<'$'<<i++<<'$'<<endl;
	empty(dt);
	
	while (input(dt))
	{
		file[0]<<dt.name<<'$'<<dt.course<<'$'<<dt.roll<<'$'<<endl;
		if (roll_find(file[1],dt.roll)==-1)
		{
			file[1].seekp(ios::end);
			file[1]<<dt.roll<<'$'<<i++<<'$'<<endl;		
		}
		empty(dt);
	}
	
	return 0;
}
