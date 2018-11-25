#include <iostream>
#include <cstdlib>
using namespace std;

struct node{
    int value;
    node *next;
};
struct lnQu{
    node *frnt;
    node *last;
};

void addEnd(node *&L,int n){
    if(L==NULL){
        L=new node;
        L->value=n;
        L->next=NULL;
    }
    else{
        L->next=new node;
        L->next->value=n;
        L->next->next=NULL;
    }
}

int delBegin(node *&L){
    int a=L->value;
    node *P=new node;
    P=L;
    L=L->next;
    delete(P);
    return a;
}

void enQu(lnQu *Q,int n){
    addEnd(Q->last,n);
    if(Q->frnt==NULL) Q->frnt=Q->last;
    else Q->last=Q->last->next;
}

int deQu(lnQu *&Q){
    if(Q->frnt==NULL){
        cout<<"Underflow";
        exit(1);
    }
    if(Q->frnt==Q->last){
        int a=delBegin(Q->frnt);
        Q->frnt=NULL;
        Q->last=NULL;
        return a;
    }
    return delBegin(Q->frnt);
}

void disp(lnQu *Q){
    node *P=new node;
    P=Q->frnt;
    while(P!=NULL){
        cout<<P->value<<" ";
        P=P->next;
    }
    cout<<endl;
}

int main(){
    int n,ch;
    lnQu *Q1;

    Q1=new lnQu;
    Q1->frnt=NULL;
    Q1->last=NULL;
    cout<<"Enter number";
    cin>>n;
    while(n!=-1){
        enQu(Q1,n);
        cout<<"Enter number";
        cin>>n;
    }
    do{
        cout<<"1. Enqueue\n2. Deque\n3. Display\n4. Exit\nChoice: ";
        cin>>ch;

        switch(ch){
        case 1:
            cout<<"Enter:";
            cin>>n;
            enQu(Q1,n);
            break;
        case 2:
            cout<<deQu(Q1)<<endl;
            break;
        case 3:
            disp(Q1);
        case 4:
            break;
        default:
            cout<<"Invalid";
        }
    }while (ch!=4);
    return 0;
}




