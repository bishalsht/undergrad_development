#include <iostream>
#include <cstring>
using namespace std;

class stac{
    int maximum;
    int top;
    char element[50];

    public:
    char topelement();
    bool isFull();
    bool isEmpty();
    void push(char a);
    char pop();

    stac(){
        maximum=50;
        top=-1;
    }
};

int main()
{
    stac s1;
    char infix[100];
    cout<<"Enter:";
    cin>>infix;
    for (int i=0;i<strlen(infix);i++){
        if (infix[i]!='+'&&infix[i]!='*'&&infix[i]!='('&&infix[i]!=')'&&infix[i]!='-'&&infix[i]!='/'){
            cout<<infix[i];
        }
        if (infix[i]=='(')
                s1.push(infix[i]);
        if (infix[i]==')'){
            while (s1.topelement()!='(')
                cout<<s1.pop();
            s1.pop();
        }
        if (infix[i]=='*'||infix[i]=='/'){
            s1.push(infix[i]);
        }
        if (infix[i]=='+'||infix[i]=='-'){
            if (s1.topelement()=='*'||s1.topelement()=='/'){
                while (!s1.isEmpty())
                    cout<<s1.pop();
            }
                s1.push(infix[i]);
        }
    }
    while (!s1.isEmpty())
        cout<<s1.pop();
    return 0;
}

void stac::push(char a){
    if (!isFull()) element[++top]=a;
    else cout<<"Overflow";
}
char stac::pop(){
    if (!isEmpty()) return element[top--];
    else cout<<"Underflow";
    return 0;
}
bool stac::isEmpty(){
    if (top==-1) return 1;
    return 0;
}
bool stac::isFull(){
    if (top==49) return 1;
    return 0;
}
char stac::topelement(){
    return element[top];
}
