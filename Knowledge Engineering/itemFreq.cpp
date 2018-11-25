#include <iostream>
#include <stdlib.h>
#include <fstream>
#define limit 5
using namespace std;

struct lnode
{
   int trans;
   lnode *next;
   lnode()
   {
      trans=-1;
      next=NULL;
   }
};

int main()
{
    ifstream fin("trans.txt");
    lnode *t[limit];
    for (int i=0;i<limit;i++) t[i]=new lnode;
    char a[5];
    int currTrans=0;
    while (fin>>a)
    {
      if (atoi(a))
      {
             int item=atoi(a);
             lnode *temp=t[item-1];
             while (temp->next!=NULL)
             temp=temp->next;
             temp->next=new lnode;
             temp->trans=currTrans;
      }
      if (a[0]=='T') currTrans++;
    }
    
    for (int i=0;i<limit;i++)
    {
        cout<<"I"<<i+1<<": ";
        lnode *temp=t[i];
        int count=0;
        while (temp->trans!=-1)
        {
              cout<<temp->trans<<" ";
              temp=temp->next;
              count++;
        }
              
        cout<<"\t\tFrequency: "<<count<<endl;
    }    
    
    cout<<endl<<endl<<endl;
    
    for (int i=0;i<limit-1;i++)
    {
        for (int j=i+1;j<limit;j++)
        {
            cout<<"I"<<i+1<<" and I"<<j+1<<":  ";
            lnode *temp,*temp2;
            temp=t[i];
            temp2=t[j];
            int count=0;
            while (temp->trans!=-1&&temp2->trans!=-1)
            {
                  if (temp->trans<temp2->trans) temp=temp->next;
                  else if (temp->trans>temp2->trans) temp2=temp2->next;
                  else 
                  {
                       cout<<temp->trans<<" ";
                       temp=temp->next;
                       temp2=temp2->next;
                       count++;
                  }
            }
            cout<<"\t\t\tFrequency: "<<count<<endl;
        }
    }
    
    fin.close();
    return 0;
}
