#include<iostream>
#include<fstream>

using namespace std;

union u
{
      int i;
      char j;
      float k;
};      

class stack
{public:
      u element[10];
      int top, size,tag[10];
      
      void push(char);
      void push(int);
      void push(float);
      int pop(int);
      char pop(char);
      float pop(float);

      
      int isfull();
      int isempty();
      stack (int x)
      { size=x;}
};

void stack::push(char x)
{
     if(isfull())
     cout<<" stack is full";
     else
     {element[++top].j=x;tag[top]=2;}
     }
void stack::push(int x)
{
     if(isfull())
     cout<<" stack is full";
     else
     element[++top].i=x;tag[top]=1;
     }
void stack::push(float x)
{
     if(isfull())
     cout<<" stack is full";
     else
     element[++top].k=x;tag[top]=3;
     }
int stack::pop(int x)
{
    if(isempty())
    cout<<"error";
    else return (element[top--].i);
}   

char stack::pop(char x)
{
    if(isempty())
    cout<<"error";
    else return (element[top--].j);
}  

float stack::pop(float x)
{
    if(isempty())
    cout<<"error";
    else return (element[top--].k);
}  
int stack::isfull()
{
    if(top<size) return (0);
    else  return (1);
}


int stack::isempty()
{ if(top>-1) return (0);
  else return (1);
}

int main()
{
    stack s(30);
    s.top=-1;
    int a;
    char b,n;
    float c;
    b='k';
    a=12;c=2.3;
    s.push(b);
    s.push(a);
    s.push(c);
    cout<<s.pop(c);
    cout<<s.pop(a);
    cout<<s.pop(b);
     cin>>n;
     return (0);
}                          