#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main()
{
	ofstream fout("inp");

	for (int i=0;i<995;i++)
	{
		int x = (rand()%900)+100;
		int y = (rand()%900)+100;
		fout<<x<<" "<<y<<endl;
	}

	fout.close();
	return 0;
}