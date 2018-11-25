#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    int a[26]={0},max;
    
    char b;
    ifstream in;
    ofstream out;
    
    //data count
    in.open("data.txt");
    while (in>>b)
    {
        a[b-97]++;
    }
    in.close();
    
    //max value
    max=a[0];
    for (int i=0;i<26;i++)
    {
        if (a[i]>max)
        max=a[i];
    }
    
    //bar graph
    out.open("output.txt");
    while(max>0)
    {
	    for (int i=0;i<26;i++)
	    {
	        if (a[i])
	        {
	                 if (a[i]<max)
	                 out<<"  ";
	                 else
	                 out<<" *";
	        }
	        
	    }
	    max--;
	    out<<endl;
    }
    
    for (char i=0;i<26;i++)
    {
        char j=97+i;
        if (a[i]!=0)
        {
                 out<<"--";
        }
    }
    out<<"-"<<endl;
    for (char i=0;i<26;i++)
    {
        char j=97+i;
        if (a[i]!=0)
        {
                 out<<" "<<j;
        }
    }
    out<<endl;
    out.close();
    system ("pause");
    return 0;
}
