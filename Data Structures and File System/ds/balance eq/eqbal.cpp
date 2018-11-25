//Balancing Equation
#include <iostream>
using namespace std;

class stack{
    public:
    char element[100];
    int top,size;
    void push(char x)
    {
    if (top<size)
    element[++top]=x;
    else
    cout<<"Stack is full";
    }
    char pop()        
    {
        return (element[top--]);
    }
    stack (int s)
    {
        size=s;
        top=-1;
        for (int m=0;m<100;m++)
        element[m]='\0';
    }
};

int main()
{
    char eq[100];
    stack b(100);
    cout<<"Enter the equation:";
    cin>>eq;
    for(int i=0;eq[i]!='\0';i++)
    {
        if (eq[i]=='('||eq[i]=='{'||eq[i]=='[')
           b.push(eq[i]);
        else if (eq[i]==')'||eq[i]=='}'||eq[i]==']')
        {
            if (eq[i]==')' && b.element[b.top]=='(')
            b.pop();
            else if (eq[i]=='}'&&b.element[b.top]=='{')
            b.pop();
            else if (eq[i]==']'&&b.element[b.top]=='[')
            b.pop();
            else if (eq[i]==')')
            {
                if (b.element[b.top]!='(')
                        break;
            }
            else if (eq[i]=='}'&&b.element[b.top]!='{')
                break;
            else if (eq[i]==']'&&b.element[b.top]!='[')
                break;
        }
    }
    if (b.top==-1)
    cout<<"Your eq is correct";
    else
    cout<<"Unbalanced";
    return 0;
}
