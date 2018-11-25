#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    int a[60], b[60],c[120], temp, m, n, o;
    ifstream in,fin;
    ofstream out,fout;
    
    /*data read*/
    fin.open("data1.txt");
    in.open("data2.txt");
    m=0;
    while (in>>b[m++]);
    n=0;
    while (fin>>a[n++]);
    fin.close();
    in.close();
    o=m+n;
    
    /*data collection and arrangement*/
    for (int i=0;i<m;i++)
    {
        c[i]=b[i];
    }
    for (int i=m;i<o;i++)
    {
        c[i]=a[i-m];
    }
    for (int i=0;i<o;i++)
    {
        for (int j=0;j<o;j++)
            if (c[j]>c[j+1])
            {
               temp=c[j];
               c[j]=c[j+1];
               c[j+1]=temp;
            }
    }
    
    /*final sorted union file*/
    fout.open("output.txt");
    temp=c[0];
    fout<<c[0]<<endl;
    for (int i=0;i<o-1;i++)
    {
        if (temp==c[i])
           i++;
        else
        {
            fout<<c[i]<<endl;
            cout<<c[i]<<endl;
            temp=c[i];
        }
    }
        
    fout.close();
    system ("pause");
    return 0;
}
