#include <iostream>
#include <cstring>
#include <unistd.h>
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

struct item{
	rules *r;
	int index;
	item()
	{
		r=NULL;
		index = -1;
	}
};

struct closure{
	item i;
	closure *next;
	closure()
	{
		next = NULL;
	}
};

struct states{
	int stateNum;
	closure *c;
	states *next;
	states()
	{
		stateNum = -1;
		c=NULL;
		next=NULL;
	}
};

rules *R2;

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
	int i = 0;
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

void first(rules *rulesList, rules *r, char *masterList[], char *list)
{
	inference *inf = r->dest;
	while(inf)
	{
		if (inf->destination[0]>='A'&&inf->destination[0]<='Z')
		{
			if (inf->destination[0] == r->source)
			{
				inf = inf->next;
				continue;
			}
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

bool findItemInClosure(closure *c, item *it)
{
	while (c)
	{
		if (c->i.r == it->r && c->i.index == it->index)	return true;
		c=c->next;
	}
	return false;
}

void find_closure(closure *source)
{
	closure *tempClos = source;
	closure *endClos = source;
	while (endClos->next) endClos = endClos->next;
	
	while (tempClos)
	{
		rules *temp = R2;
		while (temp)
		{
			if (temp->source == tempClos->i.r->dest->destination[tempClos->i.index])
			{
				item it;
				it.index = 0;
				it.r = temp;
				if (!findItemInClosure(source, &it))
				{
					endClos->next = new closure;
					endClos = endClos->next;
					endClos->i = it;
				}
			}
			temp = temp->next;
		}
		tempClos = tempClos->next;
	}
}

void printClosure(closure *c)
{
	while (c)
	{
		cout<<c->i.r->source<<"->";
		for (int i=0;i<strlen(c->i.r->dest->destination);i++)
		{
			if (i == c->i.index)
			cout<<".";
			cout<<c->i.r->dest->destination[i];
		}
		if (c->i.index == strlen(c->i.r->dest->destination)) cout<<".";
		cout<<endl;
		c=c->next;
	}
}

void find_next_transition_elements(closure *c, char* transition_elements)
{
	while (c)
	{
		if (strlen(c->i.r->dest->destination) != c->i.index)
		{
			if (notFoundInList(c->i.r->dest->destination[c->i.index], transition_elements))
				addToList(c->i.r->dest->destination[c->i.index], transition_elements);
		}
		c=c->next;
	}
}

void findGoto(closure *source, closure *&destination, char transition)
{
	closure *temp1 = source, *temp2 = destination;
	while (temp1)
	{
		if (strlen(temp1->i.r->dest->destination) != temp1->i.index && temp1->i.r->dest->destination[temp1->i.index] == transition)
		{
			temp2->next = new closure;
			temp2 = temp2->next;
			temp2->i.index = temp1->i.index+1;
			temp2->i.r = temp1->i.r;
		}
		temp1 = temp1->next;
	}
	destination = destination -> next;
}

bool closureMatch(closure *c1, closure *c2)
{
	while (c1 && c2 && c1->i.index == c2->i.index && c1->i.r == c2->i.r)
	{
		c1 = c1->next;
		c2 = c2->next;
	}
	if (c1 == NULL && c2==NULL) return true;
	else return false;
}

int foundInStateList(states *domain, states *toSearch)
{
	while (domain)
	{
		closure *searchClos = toSearch->c;
		closure *domainClos = domain->c;
		if (closureMatch(domainClos, searchClos)) break;
		domain = domain->next;
	}
	if (domain) return domain->stateNum;
	else return -1;
}