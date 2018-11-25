#include <iostream>
#include <cstring>
using namespace std;

int rules_count;

struct cell{
	char array[30];
	int size;
	cell()
	{
		size = 0;
		for (int i=0;i<30;i++)
		array[i] = '\0';
	}
};

union inference{
	char non_term[3];
	char term;
};

struct rules{
	char source;
	inference destination;
	bool is_term_rule;
	rules()
	{
		destination.non_term[2] = '\0';
		source = '\0';
		is_term_rule = false;
	}
};

void print(cell *table[],int n)
{
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<(n-i);j++)
		{
			for (int k=0;k<table[i][j].size;k++)
			cout<<table[i][j].array[k];
			cout<<'\t';
		}
		cout<<endl;
	}
}

bool check_in_table(cell **table,int i,int j,char c)
{
	for (int m=0;m<table[i][j].size;m++)
	{
		if (table[i][j].array[m] == c) return true;
	}
	return false;
}

void generate_bottom_level(char *string,int n,cell **table,rules *R)
{
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<rules_count;j++)
		{
			if (R[j].is_term_rule)
			{
				if (string[i] == R[j].destination.term)				
					table[0][i].array[table[0][i].size++] = R[j].source;
			}
		}
	}
}

void generate_upper_level(int n,cell** table,rules *R)
{
	for (int i=1;i<n;i++)
	{
		for (int j=0;j<n-i;j++)
		{
			for (int k=0;k<i;k++)
			{
				for (int m=0;m<table[k][j].size;m++)
				{
					for (int o=0;o<table[i-k-1][j+k+1].size;o++)
					{
						char arr[3]={table[k][j].array[m],table[i-k-1][j+k+1].array[o],'\0'};
						for (int l=0;l<rules_count;l++)
						{
							if (!strcmp(R[l].destination.non_term,arr))
							{
								if (!check_in_table(table,i,j,R[l].source))
								table[i][j].array[table[i][j].size++] = R[l].source;
							}
						}
					}
				}
			}
		}
	}
}

int main()
{
	cout<<"Enter the total number of production rules: ";
	cin>>rules_count;
	
	rules R[rules_count];
	int n;
	
	//PRODUCTION RULES INITIALIZATION
	
	for(int i=0;i<rules_count;i++)
	{
		cout<<"Enter the source: ";
		cin>>R[i].source;
		cout<<"Enter the type of production body (T for terminal and N for non-terminal): ";
		char body;
		cin>>body;
		if (body=='N')
		{
			R[i].is_term_rule = false;
			cout<<"Enter non terminals :";
			cin>>R[i].destination.non_term[0]>>R[i].destination.non_term[1];	
		}
		else if (body=='T')
		{
			R[i].is_term_rule = true;
			cout<<"Enter terminals :";
			cin>>R[i].destination.term;	
		}
	}
	
	//PRINTING RULES
	cout<<"Production Rules\n";
	for (int i=0;i<rules_count;i++)
	{
		cout<<R[i].source<<" -> ";
		if (R[i].is_term_rule) cout<<R[i].destination.term;
		else cout<<R[i].destination.non_term;
		
		endl(cout);
	}
	
	//USER INPUT AND TABLE DECLARATIONS
	cout<<endl<<endl;
	cout<<"Enter the size of string : ";
	cin>>n;
	
	char string[n+1];
	cout<<"Enter the string to check : ";
	cin>>string;
	
	cell **table;
	table = new cell*[n];
	for (int i=0;i<n;i++)
	table[i] = new cell[n];
	
	//CYK ALGORITHM
	
	//BOTTOM TIER
	generate_bottom_level(string,n,table,R);
	
	//REMAINING TIER
	generate_upper_level(n,table,R);

	//PRINT TABLE
	cout<<"\n\nFinal CYK Table\n";
	print(table,n);
	
	//CHECK IF DERIVABLE OR NOT
	bool found = false;
	for (int i=0;i<table[n-1][0].size;i++)
	{
		if (table[n-1][0].array[i] == 'S') found = true;
	}
	
	cout<<endl<<endl;
	if (found) cout<<"The string can be derived from the grammar rules.";
	else cout<<"The string can't be derived from the grammar rules.";
	
	return 0;	
}
