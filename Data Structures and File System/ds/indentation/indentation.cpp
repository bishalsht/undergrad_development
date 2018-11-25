#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

int main()
{
    ifstream fin("random.txt");
    ofstream fout("organized.txt");
    char tab[10]="";
    bool instring=0;
    bool inpara=0;
    char a;
    while (!fin.eof()){
    	fin>>a;
        if (a==' '){
        	fout<<" ";
            instring=!instring;
        }
        if (a=='('||a==')'&&!instring){
            inpara=!inpara;
        }
        if(a=='{'&&!instring&&!inpara){
            strcat(tab,"\t");
            fout<<endl<<tab;
        }
        fout<<a;
        if ((a==';'||a=='{'||a=='>')&&(!instring)&&!inpara){
            fout<<endl<<tab;
        }
        if(a=='}'&&!instring&&!inpara){
            tab[strlen(tab)-1]='\0';
            fout<<endl<<tab;
        }
    }
    fin.close();
    fout.close();
    return 0;
}
