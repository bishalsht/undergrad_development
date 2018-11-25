#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include <math.h>

#define NO_OF_ITEMS 5

using namespace std;

struct pbnode{
    vector<int> items;
    int count;
    pbnode(){
        items.reserve(NO_OF_ITEMS);
        count=0;
    }
};
struct fpnode{
    int item;
    int count;
    fpnode *parent;
    fpnode *first_child;
//    fpnode *previous_sibling;
    fpnode *next_sibling;
    fpnode *next_same;
    fpnode(){
        item=-1;
        count=0;
        parent=NULL;
        first_child=NULL;
//        previous_sibling=NULL;
        next_sibling=NULL;
        next_same=NULL;
    }
};
struct support{
    vector<int> items;
    int count;
    fpnode *next;
    support(){
        count=0;
        next=NULL;
    }
};

vector<support>*L;

vector<support> *calculate_1_itemset_support();
vector<support> *increase_count(vector<support> *F,int n);

fpnode *create_tree(const vector<support> *F);
fpnode *insert(fpnode *T, vector<int> *items);
fpnode *insert_wo_L(fpnode *T, vector<int> *items);
void insert_in_L(fpnode *T);

vector<int> *make_l_order(vector<int> *items);

void display(vector<int> vct);
void displayc(vector<int> vct);
void display(vector<support> vct,bool out);
void display(fpnode *T);
void display(vector<pbnode> pb);

bool l_count_find(support sup);
bool l_order(int a,int b);
bool item_cmp(support s1,support s2){
    return (s1.items.front()<s2.items.front());
}
bool count_cmp(support s1,support s2){
    return (s1.count>s2.count);
}


void mine(fpnode *T);
fpnode *prune(fpnode *T);
vector<vector<int>> generate_frequent_pattern(fpnode *T,vector<int> vct);
vector<vector<int>> process(vector<vector<int>> frequent_pattern,int n);

ifstream fin("data.txt");
ofstream fout("output.txt");

int main(int argc, const char * argv[]){
    vector<support> *F=calculate_1_itemset_support();
    
    fpnode *T=create_tree(F);
    //fout.close();
    
    //display(T);
    
    fout<<"\nTable:\n";
    
    
    mine(T);
    return 0;
}


void display(vector<int> vct){
    //vct.erase(vct.begin());
    fout<<"<";
    for(int i:vct){
        fout<<i<<" ";
    }
    fout<<">";
}
void displayc(vector<int> vct){
    //vct.erase(vct.begin());
    cout<<"<";
    for(int i:vct){
        cout<<i<<" ";
    }
    cout<<">";
}

void display(vector<vector<int>> vct){
    if (vct.size()>0) {
        fout<<"<";
        for(vector<int> vc:vct){
            display(vc);
        }
        fout<<">";
    }
}
void display(vector<support> vct, bool out ){
    if(out)fout<<"<";
    for(support sup:vct){
        if(out)display(sup.items);
        if(out)fout<<sup.count<<" ";
        if (sup.next==NULL) {
        }else{
            //display something here
            if(out)fout<<sup.next;
        }
    }
    if(out)fout<<">";
}
void display(vector<pbnode> pb){
    for (pbnode pbn:pb) {
        display(pbn.items);
        fout<<":"<<pbn.count<<" ";
    }
}

void display(fpnode *T){
    //endl(cout);
    if (T!=NULL) {
        if (T->count!=-1)
        fout<<"\tI"<<T->item<<":"<<T->count<<" ";
        display(T->next_sibling);
        display(T->first_child);
    }
    else{
        fout<<"\t";
    }
}

vector<support> *calculate_1_itemset_support(){
    vector<support> *F=new vector<support>;
    F->reserve(NO_OF_ITEMS);
    
    vector<int> items_from_one_transaction;
    items_from_one_transaction.reserve(NO_OF_ITEMS);
    //trying to parse transactions and items at this point
    char ch='\0';
    int num;
    while (!fin.eof()) {
        if(ch!='T'&&ch!='I')fin>>ch;
        //if it is new transaction, the previous transaction data must be cleared and new one should be made after increasing count
        //its probably missed for the last one so copying it out of the loop
        if (ch=='T' && items_from_one_transaction.size()!=0) {
            for (int i:items_from_one_transaction) {
                //fout<<i;
                F=increase_count(F,i);
                //fout<<"*"<<C1->count<<"*\n";
            }
            //endl(fout);
            //endl(fout);
            items_from_one_transaction.clear();
        }
        //this num can be transaction number (if ch=='T') or Item number (if ch=='I')
        fin>>num;
        //fout<<ch<<num<<endl;
        //this serves two purposes if ch is I 1. to get extra char 2. to increase count
        if(ch=='I'){
            //C1=add_item(C1,num);
            //fout<<C1->count;
            items_from_one_transaction.push_back(num);
            fin>>ch;
        }else{
            ch='\0';
        }
    }
    for (int i:items_from_one_transaction) {
        //fout<<i;
        F=increase_count(F,i);
        //fout<<"*"<<C1->count<<"*\n";
    }
    //endl(fout);
    //endl(fout);
//    items_from_one_transaction.clear();
    
    //fout<<"C1\n";
    //display(C1);
    //endl(fout);
    
    //anode *L1=remove_those_with_less_support(C1);
    
    fout<<"F:\n";
    display(*F,true);
    
    L=F;
    sort(L->begin(), L->end(), item_cmp);
    sort(L->begin(), L->end(), count_cmp);
    fout<<"\nL:\n";
    display(*L,true);
    
    endl(fout);
    
    
    return L;
}

fpnode *create_cftree(vector<pbnode> pb){
    fpnode *T=new fpnode;
    T->item=-1;
    T->count=-1;
    
    for (pbnode p:pb) {
        //cout<<"LOL";
//        vector<int>*items=&(p.items);
        for (int i=0; i<p.count; i++) {
            //cout<<"-";
            vector<int>*items=new vector<int>(p.items.begin(),p.items.end());
            //displayc(*items);
            T->first_child=insert_wo_L(T->first_child, items);
        }
    }
    T=prune(T);
    display(T);
    
    
    return T;
}
fpnode *prune(fpnode *T){
    if (T!=NULL) {
        while (T->first_child!=NULL&&T->first_child->count<2) {
            T->first_child=T->first_child->next_sibling;
        }
        while (T->next_sibling!=NULL&&T->next_sibling->count<2) {
            T->next_sibling=T->next_sibling->next_sibling;
        }
        T->next_sibling=prune(T->next_sibling);
        T->first_child=prune(T->first_child);
        
    }
    return T;
}

fpnode *create_tree(const vector<support> *F){
    fin.close();
    fin.open("/users/Nagendra/Documents/workspace/mining/fp_tree/fp_tree/data.txt");
    
    fpnode *T=new fpnode;
    T->item=-1;
    T->count=-1;
    
    vector<int> *items_from_one_transaction=new vector<int>;
    items_from_one_transaction->reserve(NO_OF_ITEMS);
    //trying to parse transactions and items at this point
    char ch='\0';
    int num;
    while (!fin.eof()) {
        if(ch!='T'&&ch!='I')fin>>ch;
        //if it is new transaction, the previous transaction data must be cleared and new one should be made after increasing count
        //its probably missed for the last one so copying it out of the loop
        if (ch=='T' && items_from_one_transaction->size()!=0) {
            
            //sort and put in tree
            items_from_one_transaction=make_l_order(items_from_one_transaction);
            
            
            T->first_child=insert(T->first_child,items_from_one_transaction);
            if (T->first_child!=NULL) {
                T->first_child->parent=T;
            }
//            for (int i:*items_from_one_transaction) {
//                fout<<i;
//                F=increase_count(F,i);
                //fout<<"*"<<C1->count<<"*\n";
//            }
//            endl(fout);
            //endl(fout);
            items_from_one_transaction->clear();
        }
        //this num can be transaction number (if ch=='T') or Item number (if ch=='I')
        fin>>num;
        //fout<<ch<<num<<endl;
        //this serves two purposes if ch is I 1. to get extra char 2. to increase count
        if(ch=='I'){
            //C1=add_item(C1,num);
            //fout<<C1->count;
            items_from_one_transaction->push_back(num);
            fin>>ch;
        }else{
            ch='\0';
        }
    }
    
    //sort and put in tree
    items_from_one_transaction=make_l_order(items_from_one_transaction);
    
    T->first_child=insert(T->first_child,items_from_one_transaction);
//    for (int i:*items_from_one_transaction) {
//        fout<<i;
//        F=increase_count(F,i);
        //fout<<"*"<<C1->count<<"*\n";
//    }
    items_from_one_transaction->clear();
    
    
    return T;
}
vector<support> *increase_count(vector<support> *F,int n){
//    for (support sup: *F) {
//        if (sup.items.front()==n) {
//            cout<<n;
//            sup.count++;
//        }
//    }
    for (vector<support>::iterator i=F->begin(); i<=F->end(); ++i) {
        if (i->items.size()>0 &&i->items.front()==n) {
            (i->count)++;
            return F;
        }
    }
    support sup;
    sup.items.reserve(1);
    sup.items.push_back(n);
    sup.count=1;
    
    F->push_back(sup);
    return F;
}

vector<int> *make_l_order(vector<int> *items){
    sort(items->begin(), items->end(), l_order);
    return items;
}

bool l_order(int a,int b){
    bool adone=false,bdone=false;
    int acount=0,bcount=0;
    for (support sup:*L) {
        if (sup.items.front()==a) {
            acount=sup.count;
            adone=true;
            if (bdone) {
                break;
            }
        }else if (sup.items.front()==b) {
            bcount=sup.count;
            bdone=true;
            if (adone) {
                break;
            }
        }
    }
    if (!adone||!bdone) {
        cout<<"We got a problem";
    }
    if (acount==0||bcount==0) {
        cout<<"We got a problemma";
    }
    return (acount>bcount);
}
fpnode *insert(fpnode *T, vector<int> *items){
    if(items->size()>0){
        if (T==NULL) {
            T=new fpnode;
            //            fout<<(*items).front()<<"added";
            T->item=(*items).front();
            T->count=1;
            insert_in_L(T);
            (*items).erase((*items).begin());
            T->first_child=insert(T->first_child, items);
            if (T->first_child!=NULL) {
                T->first_child->parent=T;
            }
            
        }else if(T->item==(*items).front()){
            T->count++;
            //            fout<<(*items).front()<<"increased";
            (*items).erase((*items).begin());
            T->first_child=insert(T->first_child, items);
            if (T->first_child!=NULL) {
                T->first_child->parent=T;
            }
        }else{
            T->next_sibling=insert(T->next_sibling, items);
            if (T->next_sibling->parent==NULL) {
                T->next_sibling->parent=T->parent;
            }
        }
    }
    
    //    display(*items);
    return T;
}
fpnode *insert_wo_L(fpnode *T, vector<int> *items){
    if(items->size()>0){
        if (T==NULL) {
            T=new fpnode;
            //            fout<<(*items).front()<<"added";
            T->item=(*items).front();
            T->count=1;
            (*items).erase((*items).begin());
            T->first_child=insert_wo_L(T->first_child, items);
            if (T->first_child!=NULL) {
                T->first_child->parent=T;
            }
            
        }else if(T->item==(*items).front()){
            T->count++;
            //            fout<<(*items).front()<<"increased";
            (*items).erase((*items).begin());
            T->first_child=insert_wo_L(T->first_child, items);
            if (T->first_child!=NULL) {
                T->first_child->parent=T;
            }
        }else{
            T->next_sibling=insert_wo_L(T->next_sibling, items);
            if (T->next_sibling->parent==NULL) {
                T->next_sibling->parent=T->parent;
            }
        }
    }
    
    //    display(*items);
    return T;
}
void insert_in_L(fpnode *T){
    //cout<<"lll";
//    fflush(stdout);
    display(*L,false);
    //fout.close();
    for (vector<support>::iterator i=L->begin(); i<=L->end(); ++i) {
        if (i->count!=-1 && i->items.size()!=0 && i->items.front()==T->item ) {
//            cout<<"\""<<i->items.front()<<"\"  ";
            if (i->next==NULL) {
                i->next=T;
//                cout<<"inser ||| ";
//                fflush(stdout);
            }else{
                fpnode * P=i->next;
                while (P->next_same!=NULL) {
                    P=P->next_same;
                }
                P->next_same=T;
//                cout<<"ted";
//                fflush(stdout);
            }
        }
    }
//    for (support sup:*L){
//        if (T->item==sup.items.front()) {
//            sup.next=T;
//            cout<<"\""<<sup.items.front()<<"\"  ";
//        }
//    }
}

void mine(fpnode *T){
//    for (support s:*L) {
//        cout<<s.next->item;
//    }
    vector<pbnode> pattern_base;
    vector<int> items;
    pattern_base.reserve(3);
    for (vector<support>::iterator i=L->end()-1; i>=L->begin(); --i) {
//        cout<<(*i).next->item;
        fpnode *F=(*i).next;
        
        //reverse(pattern_base.begin(), pattern_base.end());
//        display(pattern_base);
        
        //cout<<F->item;
        //saving it because F is changed below and we need F->item
        int n=F->item;
        //endl(cout);
        
//        endl(cout);
        pattern_base.clear();
        
        while (F!=NULL) {
            items.clear();
//            cout<<F->item;
            items.push_back(F->item);
            fpnode *parent=F->parent;
            while (parent->item!=-1) {
//                cout<<parent->item;
                items.push_back(parent->item);
                parent=parent->parent;
            }
//            cout<<":";
//            cout<<F->count;
            pbnode pb;
            pb.count=F->count;
            items.erase(items.begin());
            if (items.size()>0) {
                reverse(items.begin(), items.end());
                pb.items=items;
                pattern_base.push_back(pb);
            }
//            pattern_base.push_back(pb);
//            displayc(items);
//            endl(cout);
            F=F->next_same;
            

        }
        
        //cout<<n<<endl;
        display(pattern_base);
        fpnode * cft=create_cftree(pattern_base);
        
        vector<int> vct;
        vct.reserve(NO_OF_ITEMS);
        vector<vector<int>> frequent_pattern = generate_frequent_pattern(cft,vct);
        
        vector<vector<int>> final_pattern=process(frequent_pattern,n);
        fout<<"\t\t";
        display(final_pattern);
        endl(fout);
        //create conditional fp tree here
        
        
        endl(cout);
    }
}
vector<vector<int>> generate_frequent_pattern(fpnode *T,vector<int> vct){
    vector<vector<int>> ans;
    if (T!=NULL) {
        if(T->item!=-1)vct.push_back(T->item);
        ans= generate_frequent_pattern(T->first_child,vct);
        if (T->next_sibling!=NULL) {
            //vct.erase(vct.front());
//            if (!vct.empty()) {
            vector<int>v2=vector<int>(vct.begin(),vct.end());
            //cout<<v2.back();
            v2.erase(v2.begin());
            //displayc(vct);

                //vct.erase(vct.end());
//            }
             vector<vector<int>> ans2=generate_frequent_pattern(T->next_sibling, v2);
            for(vector<int> an:ans2){
                ans.push_back(an);
            }
        }
        return ans;
    }
    ans.reserve(3);
    ans.push_back(vct);
    return ans;
}
vector<vector<int>> process(vector<vector<int>> frequent_pattern,int n){
    vector<vector<int>> ans;
    ans.reserve(NO_OF_ITEMS);
    //displayc(frequent_pattern);
    for(vector<int>vct:frequent_pattern){
        int subset_count=pow(2, vct.size());
        
        vector<int> subset;
        subset.reserve(NO_OF_ITEMS);
        
        for (int i=1; i<subset_count; i++) {
            subset.clear();
            //cout<<"i";
            int number=i;
            int index=0;
            while (number!=0) {
                int ins=number%2;
                number=number/2;
                if (ins==1) {
                    subset.push_back(vct.at(index));
                }
                index++;
            }
            subset.push_back(n);
//            cout<<"jkjkjk";displayc(subset);
            bool exists=false;
            for(vector<int> vct:ans){
//                if (subset.front()==1) {
//                    cout<<"yay";
//                    displayc(vct);
//                    //displayc(subset);
//                }
                if (equal(subset.begin(), subset.end(), vct.begin())) {
                    exists=true;
                    
                }
            }
            if (!exists) {
                ans.push_back(subset);
            }
//            displayc(subset);
        }
    }
    return ans;
}