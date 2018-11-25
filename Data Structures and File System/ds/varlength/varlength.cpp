#include <iostream>
#include <cstring>

using namespace std;

class stac{
    int maximum;
    int top;
    char element[100];

    public:
    char topelement();
    bool isFull();
    bool isEmpty();
    void push(char a);
    char pop();

    stac(){
        maximum=99;
        top=-1;
    }
};

int main()
{
    stac s1;
    char ans,element[9];
    int len;
    cout << "This is stack with variable lenghted items.." << endl;
    do{
        cout<<"Enter the element:";
        cin>>element;

        for (int i=0;i<strlen(element);i++){
            s1.push(element[i]);
        }
        s1.push( (char)(strlen(element)+48) );
        cout<<"again?(y/n)";
        cin>>ans;
    } while (ans=='y'||ans=='Y');
    cout<<s1.topelement();

    len=int(s1.pop())-48;
    for (int i=0;i<len;i++){
        cout<<s1.pop();
    }

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
