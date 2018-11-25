#include <iostream>

using namespace std;

class stac{
    int maximum;
    int minimum;
    int top;
    int bottom;
    char element[100];

    public:
    char topelement();
    char bottomelement();
    bool isFull1();
    bool isEmpty1();
    void push1(char a);
    char pop1();

    bool isFull2();
    bool isEmpty2();
    void push2(char a);
    char pop2();

    stac(){
        maximum=100;
        minimum=-1;
        top=49;
        bottom=50;
    }
};

int main()
{
    stac s1;
    s1.push1('P');
    cout<<s1.topelement();
    cout<<s1.pop1();
    s1.push2('l');
    cout<<s1.bottomelement();
    s1.pop2();
}

void stac::push1(char a){
    if (!isFull1()) element[++top]=a;
    else cout<<"Overflow";
}
char stac::pop1(){
    if (!isEmpty1()) return element[top--];
    else cout<<"Underflow";
    return 0;
}
bool stac::isEmpty1(){
    if (top==49) return 1;
    return 0;
}
bool stac::isFull1(){
    if (top==99) return 1;
    return 0;
}
void stac::push2(char a){
    if (!isFull2()) element[--bottom]=a;
    else cout<<"Overflow";
}
char stac::pop2(){
    if (!isEmpty2()) return element[bottom++];
    else cout<<"Underflow";
    return 0;
}
bool stac::isEmpty2(){
    if (bottom==50) return 1;
    return 0;
}
bool stac::isFull2(){
    if (bottom==0) return 1;
    return 0;
}
char stac::topelement(){
    return element[top];
}
char stac::bottomelement(){
    return element[bottom];
}
