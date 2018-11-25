#include <iostream>
#include <fstream>
#include <stdlib.h>
#define limit 5
#define threshold 2
using namespace std;

ofstream fout("output.txt");

struct transDetails
{
       int item;
       transDetails *next;
       transDetails()
       {
                     item=-1;
                     next=NULL;
       }
};
struct transList
{
       transDetails *det;
       transList *next;
       transList()
       {
                  next=NULL;
                  det=NULL;
       }
};

transList *list=NULL;

void comb(int num,int arr[], int index,int prev);
int findFreq(int arr[],int index);

int main()
{
    ifstream fin("trans.txt");
    int cnt[limit]={0};
    char a[5];
    int currTrans=0;
    
    transList *temp=NULL;
    transDetails *temp2=NULL;
    
    while (fin>>a)
    {
          if (a[0]=='T')
          {
                        if (!list && !temp)
                        {
                                  list = new transList();
                                  temp=list;
                                  temp->det=new transDetails();
                                  temp2=temp->det;
                        }
                        else
                        {
                            temp->next=new transList();
                            temp=temp->next;
                            temp->det=new transDetails();
                            temp2=temp->det;
                        }
          }
          else
          {
              int item=atoi(a);
              temp2->item=item;
              temp2->next=new transDetails();
              temp2=temp2->next;
          }
    }    
    int arr[limit];
    int prev=-1;
    for (int i=1;i<=limit;i++)
    comb(i,arr,0,prev);    
       
    fin.close();
    return 0;
}


void comb(int num,int arr[], int index,int prev)
{
     if (num>0)
     {
               int i;
               for (i=0;i<limit;i++)
               if (i>prev)
               {
                          arr[index]=i+1;
                          comb(num-1,arr,index+1,i);
               }
     }
     else
     {
         int j=findFreq(arr,index);
        // if (j>=threshold)
         {
            for (int i=0;i<index;i++)
                fout<<arr[i]<<" ";
            fout<<"Frequency: "<<j<<endl;
         }
     }
}

bool frequency(transList *t,int arr[],int index)
{
     transDetails *temp=t->det;
     int i=0;
     while (temp && i<index)
     {
           if (arr[i]==temp->item)  i++;
           else if (arr[i]>temp->item) return false;
           temp=temp->next;
     }
     if (i==index) return true;
     return false;
}

int findFreq(int arr[],int index)
{
    int count=0;
    transList *temp;
    temp=list;
    while (temp!=NULL)
    {
          if (frequency(temp,arr,index)) count++;
          temp=temp->next;
    }
    return count;
}
