#include <iostream>
#include <fstream>
#include <algorithm>
#define limit 5
using namespace std;

int main()
{
    remove("trans.txt");
    ofstream fout("trans.txt");
    int noTrans;
    cin>>noTrans;
    
    for (int i=0;i<noTrans;i++)
    {
        fout<<"T"<<i+1<<" ";
        int randNum=rand()%8+1;
        int b[randNum];
        for (int j=0;j<randNum;j++)
        {
            b[j]=(rand()%limit)+1;
        }
        sort(b,b+randNum);
        for (int j=0;j<randNum-1;j++)
        {
            if (b[j]!=b[j+1]) fout<<b[j]<<" ";
        }
        fout<<b[randNum-1];
        fout<<endl;
    }
    
    fout.close();
    
    ifstream fin("trans.txt");
    
    char a[100];
    while (fin.getline(a,100,'\n'))
          cout<<a<<endl;
    
    fin.close();
    return 0;
}
