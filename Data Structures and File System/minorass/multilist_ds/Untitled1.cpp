#include<iostream>
using namespace std;
struct reg;
class student;
class course;
union un1{
	reg *next;
	course *right;
};
union un2{
	reg *below;
	student *down;
};
class student{
	public:
	char roll[4];
	int tag;
	un2 st;
	student *next;
	student()
	{
		for (int i=0;i<4;i++)
		roll[i]='\0';
	}
};
class course{
	public:
	char cname[4];
	int tag;
	un1 cs;
	course *next;
	course()
	{
		for (int i=0;i<4;i++)
		cname[i]='\0';
	}
};
struct reg{
	int tag1,tag2;
	un1 cs;
	un2 st;
};
void print_course(course *c,course *t){
	if (c!=t)
	{
		cout<<c->cname<<" ";
		print_course(c->next,t);
	}
}
void print_student(student *s, student *t){
	if (s!=t)
	{
		cout<<s->roll<<" ";
		print_student(s->next,t);
	}
}
void reg_create(student *s, course *c, char data[]){
	reg *m, *n;
	if (s->tag==0)
	{
		s->st.below=new reg;
		m=s->st.below;
		s->tag=1;
	}
	else
	{
		m=s->st.below;
		while (m->tag2==1)
		m=m->st.below;
		m->tag2=1;
		m->st.below=new reg;
		m=m->st.below;
	}
	m->tag2=0;
	m->st.down=s;
	while (c->cname[0]!=data[0]||c->cname[1]!=data[1]||c->cname[2]!=data[2])
	c=c->next;
	if (c->tag==0)
	{
		c->tag=1;
		c->cs.next=m;
	}
	else
	{
		n=c->cs.next;
		while (n->tag1==1)
		n=n->cs.next;
		n->tag1=1;
		n->cs.next=m;
	}
	m->tag1=0;
	m->cs.right=c;
}
void course_create(course *&c, course *d, char data[]){
	if (c->next==d)
	{
		c->next=new course;
		for (int i=0;i<3;i++)
		c->next->cname[i]=data[i];
		c->next->next=d;
		c->next->tag=0;
		c->next->cs.right=c->next;
	}
	else course_create(c->next,d,data);
}
void student_create(student *&s, student *d, char data[], course *c){
	if (s->next==d)
	{
		char data1[4]={'\0'};
		s->next=new student;
		for (int i=0;i<3;i++)
		s->next->roll[i]=data[i];
		s->next->next=d;
		s->next->tag=0;
		s->next->st.down=s->next;
		cout<<"Enter course to register: ";
		cin>>data1;
		do{
			reg_create(s->next,c,data1);
			cout<<"Enter course to register: ";
			cin>>data1;
		}while (data1[0]!='$');
	}
	else student_create(s->next,d,data,c);
}
void course_details(course *c,char data[]){
	while (c->cname[0]!=data[0]||c->cname[1]!=data[1]||c->cname[2]!=data[2])
	c=c->next;
	reg *m;
	if (c->tag==1)
	{
		reg *n;
		student *j;
		m=c->cs.next;
		while (m->tag1==1)
		{
			n=m;
			while (n->tag2==1)
			n=n->st.below;
			j=n->st.down;
			cout<<j->roll<<" ";
			m=m->cs.next;
		}
		n=m;
		while (n->tag2==1)
		n=n->st.below;
		j=n->st.down;
		cout<<j->roll<<" ";
	}
}
void student_details(student *s, char data[]){
	while (s->roll[0]!=data[0]||s->roll[1]!=data[1]||s->roll[2]!=data[2])
	s=s->next;
	reg *m;
	if (s->tag==1)
	{
		reg *n;
		course *j;
		m=s->st.below;
		while(m->tag2==1)
		{
			n=m;
			while (n->tag1==1)
			n=n->cs.next;
			j=n->cs.right;
			cout<<j->cname<<" ";
			m=m->st.below;
		}
		n=m;
		while (n->tag1==1)
		n=n->cs.next;
		j=n->cs.right;
		cout<<j->cname<<" ";
	}
}

int main()
{
	//course creation
	course *c;
	cout<<"Enter subject code(3 characters) : ";
	char data[4]={'\0'};
	c=new course;
	cin>>c->cname;
	c->next=c;
	c->tag=0;
	c->cs.right=c;
	cout<<"Enter subject code (3 characters) : ";
	cin>>data;
	while (data[0]!='$')
	{
		course_create(c,c,data);
	    cout<<"Enter subject code (3 characters) : ";
		cin>>data;
	} 
	
	//course printing
	cout<<"List of all course subjects\n";
	cout<<c->cname<<" ";
	print_course(c->next,c);
	
	//student registration
	student *s;
	s=new student;
	cout<<"\nEnter Roll Number (3 digits): ";
	cin>>s->roll;
	s->next=s;
	s->tag=0;
	s->st.down=s;
	char data2[4]={'\0'};
	cout<<"Enter course to register: ";
	cin>>data2;
	do{
		reg_create(s,c,data2);
		cout<<"Enter course to register: ";
		cin>>data2;
	}while (data2[0]!='$');
	cout<<"Enter Roll Number (3 digits): ";
	cin>>data2;
	while (data2[0]!='$')
	{
		student_create(s,s,data2,c);
		cout<<"Enter Roll Number (3 digits): ";
		cin>>data2;
	} 
	
	//student printing
	cout<<"List of all students\n";
	cout<<s->roll<<" ";
	print_student(s->next,s);
	
	//student details
	char name[4]={'\0'};
	cout<<"\nStudent's roll no: ";
	cin>>name;
	while (name[0]!='$')
	{
	 	cout<<"Courses enrolled by the student: \n";
		student_details(s,name);
		cout<<"\nStudent's roll no: ";
		cin>>name;
	}
	
	//courses details
	cout<<"\nSubject's code: ";
	cin>>name;
	while (name[0]!='$')
	{
		cout<<"List of students studying "<<name<<endl;
		course_details(c,name);
		cout<<"\nSubject's code: ";
		cin>>name;
	}
	return 0;
}
            
