#include <iostream>
using namespace std;

struct sub{
	int subjField;
	sub *next;
};

struct st{
	sub *subj;
	char name[10];
	st *next;
};

void add_subj(sub *b, int k)
{
	sub *c;
	c=new (sub);
	c->subjField=k;
	c->next=NULL;
	while (b->next!=NULL)
	b=b->next;
	b->next=c;
}

void add_end(st *student, char nam[])
{
	st *n;
	int k;
	n=new (st);
	for (int i=0;nam[i]!='\0';i++)
	n->name[i]=nam[i];
	n->next=NULL;
	cout<<"Enter subject number: (-1 to stop): ";
	cin>>k;
	n->subj=new (sub);
	n->subj->subjField=k;
	n->subj->next=NULL;
	cin>>k;
	while (k!=-1)
	{
		add_subj(n->subj,k);
		cin>>k;
	}
	while (student->next!=NULL)
	student=student->next;
	student->next=n;
}

void print_sub(sub *b)
{
	while (b!=NULL)
	{
		cout<<b->subjField<<" ";
		b=b->next;
	}
}

void print(st *student)
{
	while (student->next!=NULL)
	{
		student=student->next;
		cout<<"Student's Name: "<<student->name<<endl;
		cout<<"Subject List: ";
		print_sub(student->subj);
		cout<<endl<<endl;
	}
}

void printNamewise(st *student, char nam[])
{
	student=student->next;
	int i;
	while (student!=NULL)
	{
		for (i=0;i<10;i++)
		{
			if (nam[i]!=student->name[i])
			{
				break;
			}
		}
		if (i==8)
		{
			cout<<"Student's Name: "<<student->name<<endl;
			cout<<"Subject's List: ";
			print_sub(student->subj);
			cout<<endl<<endl;
			break;
		}
		student=student->next;
	}
}

void printClasswise(st *student, int i)
{
	student=student->next;
	while (student!=NULL)
	{
		while (student->subj->next!=NULL)
		{
			if (student->subj->subjField==i)
			{
				cout<<"Student's Name: "<<student->name<<endl;
				cout<<"Subject's List: ";
				print_sub(student->subj);
				cout<<endl<<endl;
				break;
			}
			student->subj=student->subj->next;
		}
		student=student->next;
	}
}

int main()
{
	st *student;
	char name[10]={'\0'};
	student=new (st);
	student->next=NULL;
	student->name[0]='$';
	student->subj=NULL;
	cout<<"Enter student's name:($ to stop)";
	cin>>name;
	do{
		add_end(student,name);
		cout<<"Enter student's name:($ to stop)";
		cin>>name;
	}while(name[0]!='$');
	
	print(student);
	
	char nam[10]={'\0'};
	cout<<"Student's Name to access: ";
	cin>>nam;
	
	printNamewise(student, nam);
	
	int subjectNo;
	cout<<"Subject whose student's data is to be accessed: ";
	cin>>subjectNo;
	cout<<"Student studying subject number "<<subjectNo<<": ";
	printClasswise(student, subjectNo);
	
	return 0;
}
