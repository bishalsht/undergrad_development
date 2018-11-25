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
struct bucket{
    lnQu *data;
};
void add(bucket &B,int a);
int pour(bucket &B);
bool empt(bucket B);

void addEnd(node *&L,int n);
int delBegin(node *&L);

void enQu(lnQu *Q,int n);
int deQu(lnQu *&Q);


int main(){
    int n1,n2;
    bucket B[10];


    for(int i=0;i<10;i++){
        B[i].data=new lnQu;
        B[i].data->frnt=NULL;
        B[i].data->last=NULL;
    }

    cout<<"How many numbers?";
    cin>>n1;
    cout<<"How many digits?";
    cin>>n2;

    int n[n1];
    for (int i=0;i<n1;i++){
        cout<<"Enter";
        cin>>n[i];
    }
    for (int i=0;i<n1;i++){
        cout<<n[i]<<" ";
    }
    cout<<endl;

    for (int i=0;i<n2;i++){
        for(int j=0;j<n1;j++){
            int tenpoweri=1;
            for(int l=0;l<i;l++) tenpoweri*=10;
            add(B[(n[j]/tenpoweri)%10],n[j]);
        }
        int co=0;
        for(int k=0;k<10;k++){
            while(!empt(B[k])){
                n[co]=pour(B[k]);
                co++;
            }
        }
    }

    for (int i=0;i<n1;i++) cout<<n[i]<<" ";
    return 0;
}
void add(bucket &B,int a){
    enQu(B.data,a);
}
int pour(bucket &B){
    return deQu(B.data);
}
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
bool empt(bucket B){
    if (B.data->frnt==NULL) return 1;
    return 0;
}
