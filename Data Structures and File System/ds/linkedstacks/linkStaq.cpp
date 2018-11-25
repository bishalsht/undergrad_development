#include <iostream>
#include <cstdlib>

using namespace std;
struct node{
    int value;
    node *next;
};
struct lnSt{
    node *top;
};
void addBegin(node *&L,int a);
int delBegin(node *&L);

int pop(lnSt *&S);
void push(lnSt *S,int a);
void disp(lnSt *S);

int main(){
    lnSt * S1=new lnSt;
    int n,ch;

    S1->top=new node;
    S1->top=NULL;
    cout<<"Enter number";
    cin>>n;
    while(n!=-1){
        push(S1,n);
        cout<<"Enter number";
        cin>>n;
    }

    do{
        cout<<"1. Push\n2. Pop\n3. Display\n4. Exit\nChoice: ";
        cin>>ch;

        switch(ch){
        case 1:
            cout<<"Enter:";
            cin>>n;
            push(S1,n);
            break;
        case 2:
            cout<<pop(S1)<<endl;
            break;
        case 3:
            disp(S1);
        case 4:
            break;
        default:
            cout<<"Invalid";
        }
    }while (ch!=4);
    return 0;
}
void addBegin(node *&L,int a){
    node *P=new node;
    P->value=a;
    P->next=L;
    L=P;
}
int delBegin(node *&L){
    int a=L->value;
    node *P=new node;
    P=L;
    L=L->next;
    delete(P);
    return a;
}
int pop(lnSt *&S){
    if(S->top==NULL){
        cout<<"Underflow";
        exit(1);
    }
    if(S->top->next==NULL){
        int a=delBegin(S->top);
        S->top=NULL;
        return a;
    }
    return delBegin(S->top);
}
void push(lnSt *S, int a){
    addBegin(S->top,a);
}
void disp(lnSt *S){
    node *P=new node;
    P=S->top;
    while(P!=NULL){
        cout<<P->value<<" ";
        P=P->next;
    }
    cout<<endl;
}
