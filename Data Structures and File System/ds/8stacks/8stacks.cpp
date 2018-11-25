#include <iostream>

using namespace std;

class stac{
    int maximum[8];
    int minimum[8];
    int top[8];
    char element[1000];

    public:
    char topelement(int n);
    bool isFull(int n);
    bool isEmpty(int n);
    void push(int n,char a);
    char pop(int n);

    stac(){
        for (int i=0;i<8;i++){
            maximum[i]=(i+1)*125;
            top[i]=maximum[i]-126;
            minimum[i]=top[i];
        }
    }
};

int main()
{
    stac s1;
    s1.push(7,'j');
    s1.push(6,'m');
    s1.push(7,'k');
    cout<<s1.pop(7);
    cout<<s1.pop(7);
    cout<<s1.pop(6);
    return 0;
}

void stac::push(int n,char a){
    if (!isFull(n)) element[++top[n]]=a;
    else cout<<"Overflow";
}
char stac::pop(int n){
    if (!isEmpty(n)) return element[top[n]--];
    else cout<<"Underflow";
    return 0;
}
bool stac::isEmpty(int n){
    if (top[n]==minimum[n]) return 1;
    return 0;
}
bool stac::isFull(int n){
    if (top[n]==maximum[n]-1) return 1;
    return 0;
}
char stac::topelement(int n){
    return element[top[n]];
}
