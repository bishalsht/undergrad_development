#include <iostream>
#include <fstream>
using namespace std;

struct data{
	char name[20]={'\0'};
	char course[3]={'\0'};
	int roll=-1;
};

fstream file[3];
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

long roll_find(int d1)
{
	file[1].open("r_index.txt", ios::in|ios::out);
	int d2;
	file[1].seekg(ios::beg);		
	while (!file[1].eof())
	{
		if (file[1]>>d2)
		{
			if (d2==d1)
			{
				file[1].close();
				return (file[1].tellg()-3);
			}
			char a[50];
			file[1].getline(a,50,'$');
		}
	}
	file[1].close();
	return -1;
}

int main()
{
	ofstream clear;
	clear.open("data.txt"); clear.close();
	clear.open("r_index.txt"); clear.close();
	file[0].open("data.txt", ios::in|ios::out);
	file[1].open("r_index.txt", ios::in|ios::out);
	data dt;
	input(dt);
	file[0]<<dt.name<<" "<<dt.roll<<" "<<dt.course<<'$';
	file[1]<<dt.roll<<" "<<i++<<'$';
	file[1].close();
	empty(dt);
	while (input(dt))
	{
		file[0]<<dt.name<<" "<<dt.roll<<" "<<dt.course<<'$';
		long pos=roll_find(dt.roll);
		if (pos!=-1)
		{
			file[1].open("r_index.txt", ios::in|ios::out);
			clear.open("buffer.txt"); clear.close();
			file[2].open("buffer.txt", ios::in|ios::out);
			file[1].seekg(pos);
			char a;
			while (file[1]>>a)
			{
				file[2]<<a;
				if (a=='$')
				{
					int m;
					file[1]>>m;
					if (m==dt.roll)
					{
						file[2]<<m;
						while (file[1]>>a&&a!='$')
						file[2]<<a;
						file[2]<<" "<<i++<<'$';
					}
				}
			}
			file[1].seekg(pos);
			file[2].seekg(ios::beg);
			while (file[2]>>a)
			file[1]<<a;
			file[2].close();
			file[1].close();
		}
		else
		{
			file[1].open("r_index.txt", ios::in|ios::out);
			file[1].seekg(ios::end);
			file[1]<<dt.roll<<" "<<i++<<'$';
			file[1].close();
		}
		empty(dt);
	}
	
	return 0;
}
