#include "slr.h"

int main()
{
	rules *R, *temp;
	R = NULL;
	R2 = NULL;
	
	//Input From Files
	char array[100]={'\0'};
	ifstream fin("rules");
	
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
	
	fin.close();
	
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
	
	fin.open("rules");
	for (int i=0;i<100;i++) array[i] = '\0';
	while (fin.getline(array,100,'\n'))
	{
		for (int i=3;array[i]!='\0';)
		{
			if (R2==NULL)
			{
				R2 = new rules;
				temp = R2;
			}
			else
			{		
				temp->next = new rules;
				temp = temp->next;
			}
			if (notFoundInList(array[0], nonTermList))
				addToList(array[0], nonTermList);
			temp->source = array[0];
			temp->dest = new inference;
			inference *tempInf = temp->dest;
			char *start, *end;
			start = &array[i];
			int t = i;
			while (array[i]!='\0' && array[i]!='|') i++;
			tempInf->destination = new char[i-t+1];
			strncpy(tempInf->destination, &array[t], i-t);
			tempInf->destination[i-t] = '\0';
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
		}
		for (int j=0;j<100;j++) array[j]='\0';
	}
	fin.close();
	
	//augmented grammer
	for(char i = 'A'; i <= 'Z' ; i++)
	{
		if (notFoundInList(i, nonTermList))
		{
			rules *augmentedRule = new rules;
			augmentedRule->source = i;
			augmentedRule->next = R2;
			augmentedRule->dest = new inference;
			augmentedRule->dest->destination = new char[2];
			augmentedRule->dest->destination[0] = R2->source;
			augmentedRule->dest->destination[1] = '\0';
			R2 = augmentedRule;
			break;
		}
	}
	
	cout<<"Augmented Grammar:\n";
	productionRulesPrinting(R2);
	
	states *s, *tempState, *endState;
	s=new states;
	s->stateNum = 0;
	s->c = new closure;	
	s->c->i.index=0;
	s->c->i.r = R2;
	
	find_closure(s->c);
	
	tempState = s;
	endState = s;

	cout<<"\nState 0\n";
	printClosure(s->c);
	cout<<endl;
	
	while (tempState)
	{
		int transLen = strlen(termList)+strlen(nonTermList)+1;
		char transition_elements[transLen];
		for (int k=0;k<transLen;k++)
			transition_elements[k] = '\0';
		find_next_transition_elements(tempState->c, transition_elements);
		for (int i=0;i<strlen(transition_elements);i++)
		{
			states *t;
			t=new states;
			t->c = new closure;
			findGoto(tempState->c, t->c, transition_elements[i]);
			find_closure(t->c);
			int foundIn = foundInStateList(s,t);
			if (foundIn == -1)
			{
				endState->next = t;
				t->stateNum = endState->stateNum+1;
				endState = endState->next;
				cout<<"Goto["<<tempState->stateNum<<","<<transition_elements[i]<<"] = State"<<t->stateNum<<endl;
				printClosure(t->c);
				cout<<endl;
			}
			else
			{
				cout<<"Goto["<<tempState->stateNum<<","<<transition_elements[i]<<"] = State"<<foundIn<<endl;
				printClosure(t->c);
				cout<<endl;
			}
		}
		tempState = tempState->next;
	}
	
	return 0;
}
