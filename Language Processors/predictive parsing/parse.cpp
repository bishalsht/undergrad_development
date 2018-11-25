#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>
using namespace std;

char termList[100]={'\0'}, nonTermList[100]={'\0'};

struct inference{
	char *destination;
	inference *next;
	inference()
	{
		destination = NULL;
		next = NULL;
	}
};

struct rules{
	char source;
	inference *dest;
	rules *next;
	rules()
	{
		source = '\0';
		dest = NULL;
		next = NULL;
	}
};

bool notFoundInList(char item, char *list)
{
	while (list[0]!='\0')
	{
		if (list[0] == item) return false;
		list++;
	}
	return true;
}

void addToList(char item, char *list)
{
	if (list[0]=='\0') {
		list[0] = item;
		return;
	}
	while (list[1]!='\0')
	list++;
	list[1] = item;
}

void productionRulesPrinting(rules *R)
{	
	char i = 'a';
	while (R)
	{
		cout<<i++<<". "<<R->source<<"->";
		inference *tempInf = R->dest;
		while (tempInf)
		{
			cout<<tempInf->destination;
			tempInf = tempInf->next;
			if (tempInf) cout<<"|";
		}
		cout<<endl;
		R=R->next;
	}
}

bool checkForLeftRecursion(rules *r)
{
	inference *tempInf = r->dest;
	while (tempInf)
	{
		if (tempInf->destination[0] == r->source) return true;
		tempInf = tempInf->next;
	}
	return false;
}

void GNFConversion(rules *rulesList, rules *r)
{
	char i;
	for (i = 'A'; i<= 'Z'; i++)
	{
		if (notFoundInList(i,nonTermList)) break;
	}
	if (i=='Z'+1)
	{
		cout<<"Too many nonterminals";
		exit(0);
	}
	while (rulesList->next) rulesList = rulesList->next;
	rulesList->next = new rules;
	rulesList = rulesList->next;
	addToList(i, nonTermList);
	rulesList->source = i;
	//new rule
	inference *temp = r->dest;
	rulesList->dest = new inference;
	inference *temp2 = rulesList->dest;
	while (temp)
	{
		if (temp->destination[0] == r->source)
		{
			temp2->next = new inference;
			temp2 = temp2->next;
			int len = strlen(temp->destination);
			char arr[len+1];
			strncpy(arr, &temp->destination[1], len-1);
			arr[len-1] = '\0';
			temp2->destination = new char[len];
			strcpy(temp2->destination, arr);
			arr[len-1] = i;
			arr[len] = '\0';
			temp2->next = new inference;
			temp2 = temp2->next;
			temp2->destination = new char[len+1];
			strcpy(temp2->destination, arr);
			if (temp == r->dest) 
			{
				r->dest = r->dest->next;
				temp = r->dest;
			}
			else 
			{
				inference *temp2 = r->dest;
				while (temp2->next!= temp) temp2 = temp2->next;
				temp2->next = temp2->next->next;
				temp = temp2->next;
			}
		}
		else temp = temp->next;
	}
	rulesList->dest = rulesList->dest->next;
	
	//old rules
	temp = r->dest;
	temp2 = temp;
	while (temp2->next) temp2 = temp2->next;
	inference *temp3;
	temp3 = temp2;
	
	while (temp!=temp3)
	{
		temp3->next = new inference;
		temp3 = temp3->next;
		char arr[100]={'\0'};
		strcpy(arr, temp->destination);
		strcat(arr, &i);
		arr[strlen(temp->destination)+1] ='\0';
		temp3->destination = new char[strlen(temp->destination)+2];
		strcpy(temp3->destination, arr);
		temp = temp->next;
	}
	temp3->next = new inference;
	temp3 = temp3->next;
	char arr[100]={'\0'};
	strcpy(arr, temp->destination);
	strcat(arr, &i);
	arr[strlen(temp->destination)+1] ='\0';
	temp3->destination = new char[strlen(temp->destination)+2];
	strcpy(temp3->destination, arr);	
}

inference* checkForCommonPrefix(rules *r)
{
	inference *t1, *t2;
	if (r)
	t1 = r->dest;
	else return NULL;
	if (r->dest)
	t2 = r->dest->next;
	else return NULL;
	while (t1->next)
	{
		while (t2)
		{
			if (t1->destination[0]==t2->destination[0])
			return t1;
			t2 = t2->next;
		}
		t1 = t1->next;
	}
	return NULL;
}

void leftFactor(rules *rulesList, inference *common)
{
	char i;
	for (i = 'A'; i<= 'Z'; i++)
	{
		if (notFoundInList(i,nonTermList)) break;
	}
	if (i=='Z'+1)
	{
		cout<<"Too many nonterminals";
		exit(0);
	}
	while (rulesList->next) rulesList = rulesList->next;
	rulesList->next = new rules;
	rulesList = rulesList->next;
	addToList(i, nonTermList);
	rulesList->source = i;
	rulesList->dest = new inference;
	inference *tempInf = rulesList->dest;
	
	char cmmn = common->destination[0];
	if (strlen(common->destination)==1)
	{
		tempInf->destination = new char[2];
		strcpy(tempInf->destination, "#\0");
	}
	else
	{
		tempInf->destination = new char[strlen(common->destination)];
		strcpy(tempInf->destination, &common->destination[1]);
		tempInf->destination[strlen(common->destination)-1] = '\0';
	}
	common->destination[1] = i;
	common->destination[2] = '\0';
	
	inference *temp = common->next;
	while(temp)
	{
		if (temp->destination[0]==cmmn)
		{
			tempInf->next = new inference;
			tempInf = tempInf->next;
			tempInf->destination = new char[strlen(temp->destination)];
			strcpy(tempInf->destination, &temp->destination[1]);
			tempInf->destination[strlen(temp->destination)-1]='\0';
			
			inference *temp2 = common;
			while (temp != temp2->next) temp2 = temp2->next;
			temp2->next = temp2->next->next;
			temp = common->next;
		}
		
	}
}

void first(rules *rulesList, rules *r, char *masterList[], char *list)
{
	inference *inf = r->dest;
	while(inf)
	{
		if (inf->destination[0]>='A'&&inf->destination[0]<='Z')
		{
			rules *tempR = rulesList;
			char **tempL = masterList;
			while (tempR && tempR->source!=inf->destination[0]) 
			{
				tempR = tempR->next;
				tempL++;
			}
			first(rulesList, tempR, masterList, *tempL);
			char *tempList = *tempL;
			while (*tempList!='\0')
			{
				char *tempList2 = list;
				while (*tempList2!='\0'&&*tempList2!=*tempList) *tempList2++;
				if (*tempList2 == '\0') *tempList2 = *tempList;
				tempList++;
			}
		}
		else
		{
			char *temp = list;
			while (*temp!=inf->destination[0] && *temp!='\0') temp++;
			if (*temp == '\0')
			*temp = inf->destination[0];
		}
		inf = inf->next;
	}
}

void follow(rules *R, char toCheck, char **firstList, char **followList, int ind, bool *followCalls)
{
	if (followCalls[ind]==true) return;
	followCalls[ind] = true;
	rules *temp = R;
	int currRule = 0;
	while (temp)
	{
		inference *inf;
		inf = temp->dest;
		while (inf)
		{
			for (int i=0;i<strlen(inf->destination);i++)
			{
				if (inf->destination[i] == toCheck) 
				{			
					if (inf->destination[i+1]=='\0'){
						//write code for last character						
						follow(R, temp->source, firstList, followList, currRule, followCalls);
						followCalls[currRule] = true;						
						for (int m=0;m<strlen(followList[currRule]);m++)
						{
							if (notFoundInList(followList[currRule][m], followList[ind]))
							addToList(followList[currRule][m], followList[ind]);
						}
					}
					else if (!notFoundInList(inf->destination[i+1], termList))
					{
						if (notFoundInList(inf->destination[i+1], followList[ind]))
						addToList(inf->destination[i+1], followList[ind]);
					}
					else
					{
						bool flag = true;
						while (flag)
						{
							flag = false;
							rules *temp2 = R;
							int tempVar = 0;
							while (temp2->source != inf->destination[i+1]) 
							{
								tempVar++;
								temp2 = temp2->next;
							}
							for (int j=0;j<strlen(firstList[tempVar]);j++)
							{
								if (firstList[tempVar][j]=='#')
								{
									flag = true;
								}
								else if (notFoundInList(firstList[tempVar][j], followList[ind]))
								addToList(firstList[tempVar][j], followList[ind]);
							}
							if (flag == true)
							{
								if (inf->destination[i+2]=='\0')
								{
									//do something				
									follow(R, temp->source, firstList, followList, currRule, followCalls);
									followCalls[currRule] = true;						
									for (int m=0;m<strlen(followList[currRule]);m++)
									{
										if (notFoundInList(followList[currRule][m], followList[ind]))
										addToList(followList[currRule][m], followList[ind]);
									}
									flag = false;
								}
								else if (!notFoundInList(inf->destination[i+2], termList))
								{
									if (notFoundInList(inf->destination[i+2], followList[ind]))
									addToList(inf->destination[i+2], followList[ind]);
									flag = false;
								}
								else
								{
									i=i+1;
								}
							}
						}
					
					}
				}
			}
			inf = inf->next;
		}
		currRule++;
		temp = temp->next;
	}
}

int main()
{
	rules *R, *temp;
	R = NULL;
	
	//Input From Files
	char array[100]={'\0'};
	ifstream fin("rules.txt");
	
	while (fin.getline(array,100,'\n'))
	{
		if (R==NULL)
		{
			R = new rules;
			temp = R;
		}
		else
		{
			
			temp->next = new rules;
			temp = temp->next;
		}
		int i=0;
		if (notFoundInList(array[0], nonTermList))
			addToList(array[0], nonTermList);
		temp->source = array[0];
		temp->dest = new inference;
		inference *tempInf = temp->dest;
		for (i=3;array[i]!='\0';)
		{
			char *start, *end;
			start = &array[i];
			int t = i;
			while (array[i]!='\0' && array[i]!='|') i++;
			tempInf->destination = new char[i-t+1];
			strncpy(tempInf->destination, &array[t], i-t);
			tempInf->destination[i-t] = '\0';
			cout<<temp->source<<"->"<<tempInf->destination<<endl;
			end = &array[i-1];
			while (start!=end+1)
			{
				if (*start>='A' && *start<='Z')
				{
					if (notFoundInList(*start, nonTermList))
						addToList(*start, nonTermList);
				}
				else
				{
					if (notFoundInList(*start, termList))
						addToList(*start, termList);
				}
				start++;
			}
			i++;
			if (array[i]=='\0') break;
			tempInf->next = new inference;
			tempInf = tempInf->next; 
		}
		for (int j=0;j<100;j++) array[j]='\0';
	}
	
	cout<<"list of terminals: "<<termList<<endl<<endl<<"list of non-terminals:"<<nonTermList<<endl<<endl;	
	
	cout<<"production rules:\n";
	productionRulesPrinting(R);
	
	//Left Recursion
	temp=R;
	while (temp)
	{
		if (checkForLeftRecursion(temp))
		{
			GNFConversion(R, temp);
		}
		temp = temp->next;
	}
	
	cout<<"list of terminals: "<<termList<<endl<<endl<<"list of non-terminals:"<<nonTermList<<endl<<endl;	
	
	cout<<"production rules:\n";
	productionRulesPrinting(R);	
	
	//Left Factoring
	
	temp = R;
	while (temp)
	{
		inference *common=NULL;
		while ((common = checkForCommonPrefix(temp))!=NULL)
		{		
			leftFactor(R, common);
		}
		temp = temp->next;
	}
	
	cout<<"list of terminals: "<<termList<<endl<<endl<<"list of non-terminals:"<<nonTermList<<endl<<endl;	
	
	cout<<"production rules:\n";
	productionRulesPrinting(R);
	
	char **firstList;
	firstList = new char*[strlen(nonTermList)];
	for (int t=0;t<strlen(nonTermList);t++)
	{
		firstList[t] = new char[strlen(termList)+1];
		for (int u=0;u<strlen(termList)+1;u++) 
			firstList[t][u] = '\0';
	}
	
	temp = R;
	for(int i=0;temp;i++)
	{
		first(R, temp, firstList, firstList[i]);
		temp = temp->next;
	}	
	
	cout<<"\nFirst of all the non-terminals: \n";
	temp = R;
	for (int i=0;temp;i++)
	{
		cout<<temp->source<<" : "<<firstList[i]<<endl;
		temp = temp->next;
	}
	
	char **followList;
	bool *followCalls;
	followCalls = new bool[strlen(nonTermList)];
	followList = new char*[strlen(nonTermList)];
	for (int i=0;i<strlen(nonTermList);i++)
	{
		followCalls[i] = false;
		followList[i] = new char[strlen(termList)+1];
		for (int u=0;u<strlen(termList);u++)
			followList[i][u] = '\0';
	}
	
	followList[0][0] = '$';
	for (int i=0;nonTermList[i]!='\0';i++)
	{
		follow(R, nonTermList[i], firstList, followList, i, followCalls);
	}
	
	for (int i=0;i<strlen(nonTermList);i++)
		followCalls[i] = false;
	for (int i=0;nonTermList[i]!='\0';i++)
	{
		follow(R, nonTermList[i], firstList, followList, i, followCalls);
	}
	
	cout<<"\nFollow of all the non-terminals: \n";
	temp = R;
	for (int i=0;temp;i++)
	{
		cout<<temp->source<<" : "<<followList[i]<<endl;
		temp = temp->next;
	}
	
	termList[strlen(termList)] = '$';
	char ***table;
	table = new char**[strlen(termList)];
	for (int i=0;i<strlen(termList);i++)
	{
		table[i] = new char*[strlen(nonTermList)];	
		for (int j=0;j<strlen(nonTermList);j++)
		{
			table[i][j] = new char[3];
			table[i][j][0] = '\0';
			table[i][j][1] = '\0';
			table[i][j][2] = '\0';
		}
	}

	temp = R;
	int var = 0;
	
	while (temp)
	{
		for (int i=0;i<strlen(firstList[var]);i++)
		{
			if (firstList[var][i]!='#')
			{
				for (int j=0;j<strlen(termList);j++)
				{
					if (firstList[var][i] == termList[j])
					{
						table[j][var][0] = var+'a';
						rules *temporary = R;
						for (int q=0;q<var;q++) temporary = temporary->next;
						char op = '1';
						inference *inff = temporary->dest;
						while (inff&&inff->destination[0]=='#')
						{
							inff = inff->next;
							op++;
						}
						if (!inff) op = '1';
						table[j][var][1] = op;
					}
				}
			}
			else
			{
				for (int j=0;j<strlen(followList[i]);j++)
				{
					for (int u=0;u<strlen(termList);u++)
					{
						if (followList[i][j] == termList[u])
						{
							table[u][var][0] = var+'a';
							rules *temporary = R;
							for (int q=0;q<var;q++) temporary = temporary->next;
							char op = '1';
							inference *inff = temporary->dest;
							while (inff&&inff->destination[0]!='#')
							{
								inff = inff->next;
								op++;
							}
							if (!inff) op = '1';
							table[u][var][1] = op;
						}
					}
				}
			}
		}
		var++;
		temp = temp->next;
	}
	
	cout<<endl<<endl<<"All production Rules:\n";
	productionRulesPrinting(R);
	
	for (int i=0;i<strlen(nonTermList)+1;i++)
	{
		for (int j=0;j<strlen(termList)+1;j++)
		{
			if (i==0 && j==0) cout<<" \t";
			else if (i==0) cout<<termList[j-1]<<"\t";
			else if (j==0) cout<<nonTermList[i-1]<<"\t";
			else
			{
				if (table[j-1][i-1][0]=='\0') cout<<"\t";
				else cout<<table[j-1][i-1]<<"\t";
			}
		}
		cout<<endl;
	}
	
	return 0;	
}
