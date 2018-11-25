#include <iostream>
#include <fstream>

using namespace std;

struct decNode{
	int age;
	int income;
	int student;
	int credit_rating;
	int buys_comp;
	decNode *next;
};

int main()
{
	ifstream fin("input");

	while (fin.getline(tuple,100,'\n'))
	{
		
	}

	fin.close();

	return 0;
}