#include<iostream>
#include <fstream>
#include<cstring>
#include<stdlib.h>
using namespace std;
int n,j1=0;
int t=0;
int j=0;
int cn[20];
int r=0;
int cnt=0;
char symbol[10];
char gsymbol[10],st;
int sym1=0,sym2=0;
struct parse
{
	char content[50];
}stack[50],input[50],output[50];
struct data
{
       int no;
       char cno[2];
       int ino[5];
       char chr[5];
       int samecnt;
       char a;
       char c[10];
       int count;
       data()
       {
             count=0;
             samecnt=0;
       }
}d[10],items[20][10],d1[10];
data itm[10];
struct table
{
	char action[20][20];
	int action1[20][20];
	int got[20][20];
}tab;
struct followfirst
{
    char var;
	char fo[10];
	int fcnt;
	followfirst()
	{
            fcnt=0;
    }
}fol[10],fi[10];
void closure1(data d3[],char b,int p,int q)
{
     if(q==cn[0])
     return;
     if(items[0][q].a==b)
     {
     for(int k=0;k<strlen(items[0][q].c);k++)
     {
             if(items[0][q].c[k]=='.' && k<strlen(items[0][q].c)-1 && (items[0][q].c[k+1]>=65 && items[0][q].c[k+1]<=90))
             {
             //cout<<"hello1\n";
             items[j1][cnt]=items[0][q];
             cnt++;
             closure1(d3,items[0][q].c[k+1],p,q+1);
             }
             else if( items[0][q].c[k]=='.' && k<strlen(items[0][q].c)-1)
             {
             //cout<<"hello2\n";
             items[j1][cnt]=items[0][q];
             cnt++; 
             closure1(d3,b,p,q+1);
             }
     }
     }
     else
     closure1(d3,b,p,q+1);
}
                 
void closure(data d2[],char b,int p,int k)
{
     //for(int i=0;i<d2[0].count;i++)
     //{
     
     if(d2[p].a>=65 && d2[p].a<=90)
     {
     for(int k=0;k<strlen(d2[p].c);k++)
     {
             if(d2[p].c[k]=='.' && k==strlen(d2[p].c)-1 && d2[p].c[k-1]==b)
             {
                 //cout<<"hii1\n";
                 items[j1][cnt]=d2[p];
                 cnt++;
                //cout<<items[j1][0].count<<"\n";
                 return;
             }
             else if(d2[p].c[k]=='.' && k<strlen(d2[p].c)-1)
             {
                 if(d2[p].c[k+1]>=65 && d2[p].c[k+1]<=90)
                 {
                   data ab=d2[p];
                   t=0;
                   items[j1][cnt]=d2[p];
                   cnt++;
                   for(int x=0;x<cn[j];x++)
                   {
                               //items[j][x]=itm[x];
                               cout<<items[j][x].a<<"-"<<items[j][x].c[0]<<items[j][x].c[1]<<"  ";
                   }
                   d2[p]=ab;
                   char z=d2[p].c[k];
                         d2[p].c[k]=d2[p].c[k-1];
                         d2[p].c[k-1]=z;
                   closure1(d2,d2[p].c[k+1],p,0);
                 }
                 else if((d2[p].c[k+1]>=97 && d2[p].c[k+1]<=120) || (d2[p].c[k+1]>=39 && d2[p].c[k+1]<=47))
                 {
                    //cout<<"hii3\n";
                     items[j1][cnt]=d2[p];
                     cnt++;
                     //cout<<items[j1][0].count<<"\n";
                     return;
                 }
             }
         }
     }
}
void goto1(data d1[],char b)
{
    // items[j1][0].count=0;
     for(int i=0;i<d1[0].count;i++)
     {
            // items[j1][0].count=0;
             for(int k=0;k<strlen(d1[i].c);k++)
             {
                  if(d1[i].c[k]=='.' && k<strlen(d1[i].c)-1 && d1[i].c[k+1]==b)
                  {
                         //cout<<b<<"\n";
                         char z=d1[i].c[k];
                         d1[i].c[k]=d1[i].c[k+1];
                         d1[i].c[k+1]=z;
                         t=0;
                         closure(d1,b,i,k);
                  }
             }
     }
}
void first()
{
     for(int j1=1;j1<=2;j1++)
     {
     for(int i=sym2-1;i>=0;i--)
     {
             fi[i].var=gsymbol[i];
             for(int i1=n-1;i1>=0;i1--)
		     {
				if(d1[i1].a==gsymbol[i])
                {
                    if(d1[i1].c[0]>=65 && d1[i1].c[0]<=90)
                    {
                       int i2;
                       for(i2=0;i2<sym2;i2++)
                       {
                           if(d1[i1].c[0]==gsymbol[i2])
                           break;
                       }
                       fi[i]=fi[i2];
                       fi[i].fcnt=fi[i2].fcnt;
                       
                    }
                    else if((d1[i1].c[0]>=97 && d1[i1].c[0]<=120 ) || (d1[i1].c[0]>=33 && d1[i1].c[0]<=47 ))
                    {
                         int f1=0;
                             for(int i4=0;i4<fi[i].fcnt;i4++)
                             {
                                  if(fi[i].fo[i4]==d1[i1].c[0])
                                  {
                                        f1=1;
                                        break;
                                  }
                             } 
                         if(f1==0)
                         {
                         fi[i].fo[fi[i].fcnt]=d1[i1].c[0];
                         fi[i].fcnt++;
                         }
                    }
//                    cout<<i<<"HII "<<fi[i].fcnt<<"\n";
                }
            }
    }
    }
}
int check(char c2)
{
    for(int i=0;i<n;i++)
    {
            if(d1[i].a==c2)
            {
               if(d1[i].c[0]=='?')
               return 0;
               else if(d1[i].c[0]>=65 && d1[i].c[0]<=90)
               return (check(d1[i].c[0]));
            } 
    }
    return 1;
}
void follow()
{
	for(int i=0;i<sym2;i++)
	{
        fol[i].var=gsymbol[i];
        //cout<<"\n"<<gsymbol[i]<<"--";
        if(i==0)
        {
            fol[i].fo[fol[i].fcnt]='$';
            fol[i].fcnt++;
        }    
		for(int i1=0;i1<n;i1++)
		{
			for(int i3=0;i3<strlen(d[i1].c);i3++)
			{
				if(d[i1].c[i3]==gsymbol[i])
                {
                if(i3<strlen(d[i1].c)-1)
                {
                   if((d[i1].c[i3+1]>=97 && d[i1].c[i3+1]<=120 ) || (d[i1].c[i3+1]>=33 && d[i1].c[i3+1]<=47 ))
                   {
                       //cout<<d[i1].c[i3+1]<<",";
                       fol[i].fo[fol[i].fcnt]=d[i1].c[i3+1];
                       fol[i].fcnt++;
                   }
                }
                }
			}
		}
	}
//	cout<<"\n";
   int flag=0;
	for(int i=0;i<sym2;i++)
	{    
        //cout<<"--"<<gsymbol[i]<<"-";
		for(int i1=0;i1<n;i1++)
		{
			for(int i3=0;i3<strlen(d[i1].c);i3++)
			{
                if(d[i1].c[i3]>=65 && d[i1].c[i3]<=90)
                {
                     int j1;
                     char c1=d[i1].c[i3];
                     flag=0;
                     for(j1=i3+1;j1<strlen(d[i1].c);j1++)
                     {
                         flag=check(d[i1].c[j1]); 
                         if(flag==1)
                         break; 
                     } 
                }                 
                if(d[i1].a==gsymbol[i] && i3==strlen(d[i1].c)-1 && flag==0)
                {
                     int o;
                     for(o=0;o<sym2;o++)
                     {
                             if(d[i1].c[i3]==gsymbol[o])
                             break;
                     }
                     for(int o1=0;o1<fol[i].fcnt;o1++)
                     {
                             int f1=0;
                             for(int i4=0;i4<fol[o].fcnt;i4++)
                             {
                                  if(fol[i].fo[o1]==fol[o].fo[i4])
                                  {
                                        f1=1;
                                        break;
                                  }
                             } 
                             if(f1==0)
                             {  
                                fol[o].fo[fol[o].fcnt]=fol[i].fo[o1];
                                fol[o].fcnt++;
                             }
                     }
                }
			}
		}
	}
}
void maketable()
{
     int e;
	for(int i=0;i<=r;i++)
	{
        if(i>=1)
        {
        for(int o1=0;o1<=items[i][0].samecnt;o1++)
	    {
	       if(items[i][0].chr[o1]>=65 && items[i][0].chr[o1]<=90)
	       {
              //cout<<"hii "<<items[i][0].ino[o1]<<" "<<items[i][0].chr[o1]<<"\n";
              for(e=0;e<sym2;e++)
              {
                     if(gsymbol[e]==items[i][0].chr[o1])
			         break;
              }
              if(e<sym2)
              tab.got[items[i][0].ino[o1]][e]=items[i][0].no;
            }
            //cout<<"got "<<items[i][0].ino[o1]<<" "<<e<<" "<<i<<"\n";
        }
        }
		for(int i1=0;i1<items[i][0].count;i1++)
		{
			for(int i2=0;i2<strlen(items[i][i1].c);i2++)
			{
                if(items[i][i1].a=='S' && items[i][i1].c[0]==st && items[i][i1].c[1]=='.' && strlen(items[i][i1].c)==2)
                {
                    for(e=0;e<sym1;e++)
		            {
                     if(symbol[e]=='$')
			         break;
                    }
                   tab.action[i][e]='A';
                }
                if(items[i][i1].a!='S' && items[i][i1].c[i2]=='.' && i2==strlen(items[i][i1].c)-1)
                {
                     //cout<<items[i][i1].no<<","<<items[i][i1].ino[0]<<","<<items[i][i1].chr[0]<<"\n";
                         int i3;
                         for(i3=0;i3<sym2;i3++)
                         {
                              if(items[i][i1].a==fol[i3].var)
                              break;
                         }
                         for(int i4=0;i4<fol[i3].fcnt;i4++)
                         {
                              for(e=0;e<sym1;e++)
					          {
            							if(symbol[e]==fol[i3].fo[i4])
            							break;
                              }  
                              int i5;
                              int flag=0;
                              int val=-1;
                              char str[3];
                              for(i5=0;i5<n;i5++)
                              {
                                 flag=0;
                                 if(d[i5].a==items[i][i1].a)
                                 {
                                    int i7=0,i6;
                                    
                                    for(i6=0;i6<strlen(d[i5].c) && i7<strlen(items[i][i1].c);i6++,i7++)
                                    {
                                         if(items[i][i1].c[i7]=='.')
                                         {
                                          i6--;
                                          continue;
                                         }
                                         if(d[i5].c[i6]=='.')
                                         {
                                          i7--;
                                          continue;
                                         }
                                         if(d[i5].c[i6]!=items[i][i1].c[i7])
                                         {
                                           flag=1;
                                           break;
                                         }
                                    }
                                    if(i6==strlen(d[i5].c) || i7==strlen(items[i][i1].c))
                                    {
                                       val=i5;
                                       break;
                                    }
                                    
                                 }
                              }
//                                 cout<<"val->"<<val<<"\n";
//                                 cout<<"fl-"<<flag<<"\t"<<d[val].a<<"*"<<d[val].c<<"\n";
                                 if(flag==0)
                                 {
                                    tab.action[i][e]='R';
            						tab.action1[i][e]=val+1;
//            						cout<<i<<" "<<e<<" "<<tab.action[i][e]<<tab.action1[i][e]<<"\n";
            					
                                 }
                          }
                  }
                        
				if(i2<strlen(items[i][i1].c)-1 && items[i][i1].c[i2]=='.')
				{
					if((items[i][i1].c[i2+1]>=97 && items[i][i1].c[i2+1]<=120 ) || (items[i][i1].c[i2+1]>=33 && items[i][i1].c[i2+1]<=47 ))
					{
						for(int o=0;o<=r;o++)
						{
							for(int o1=0;o1<=items[o][0].samecnt;o1++)
            				{
            					if(items[o][0].ino[o1]==i && items[o][0].chr[o1]==items[i][i1].c[i2+1])
            					{
                                   for(e=0;e<sym1;e++)
            						{
            							if(symbol[e]==items[o][0].chr[o1])
            							break;
            						}
            						tab.action[i][e]='S';
            						tab.action1[i][e]=items[o][0].cno[0]-'0';
//            						cout<<i<<" "<<e<<" "<<tab.action[i][e]<<tab.action1[i][e]<<"\n";
            					}
            				}
						}
					}
				}
			}
		}
	}
 
}
int main()
{
    
    //data items[20][10];
//    cout<<"enter no of productions\n";
    ifstream fin("rules.txt");
    fin>>n;
    char ch[10];
    char v1[50],v2[50];
    int y=0;
    
    for(int i=0;i<n;i++)
    {
//            cout<<"enter left symbol of production "<<(i+1)<<"\n";
            fin>>d[i].a;
            d1[i].a=d[i].a;
            if(i==0)
            st=d[i].a;
            int flg=0;
            	for(int e1=0;e1<sym2;e1++)
            	{
            		if(gsymbol[e1]==d[i].a)
            		{
            			flg=1;
            			break;
            		}
            	}
            	if(flg==0)
            	gsymbol[sym2++]=d[i].a;
//            cout<<"enter right side of production\n";
            fin>>ch;
            for(int e=0;e<strlen(ch);e++)
            {
            	if((ch[e]>=97 && ch[e]<=120 ) || (ch[e]>=39 && ch[e]<=47 ))
            	{
            	symbol[sym1++]=ch[e];
            	//cout<<"terminal";
            	}
            	else if(ch[e]>=65 && ch[e]<=90)
            	{
            	flg=0;
            	for(int e1=0;e1<sym2;e1++)
            	{
            		if(gsymbol[e1]==ch[e])
            		{
            			flg=1;
            			break;
            		}
            	}
            	if(flg==0)
            	gsymbol[sym2++]=ch[e];
            	//cout<<"non terminal";
            	}
            }
            
            d[i].c[0]='.';
            strcat(d[i].c,ch);
            strcpy(d1[i].c,ch);
    }
    symbol[sym1++]='$';
    items[0][0].a='S';
    items[0][0].c[0]='.';
    items[0][0].c[1]=d[0].a;
    items[0][0].c[2]='\0';
//    cout<<"items are\n";
    for(int i=0;i<n;i++)
    {
//            cout<<d[i].a<<"->"<<d[i].c<<"\n";
            items[0][i+1]=d[i];
    }
    cout<<"LR(0) items\n";
    for(int i=0;i<=n;i++)
    {
            cout<<items[0][i].a<<"->"<<items[0][i].c<<"\n";
    }
    int w=0,w1=0;
    items[0][0].count=n+1;
    items[0][0].no=0;
    cn[0]=n+1;
    data itm[10];
    /*j1++;
    cout<<items[j1][0].count<<"\n";
    cnt=items[j1][0].count;
    goto1(items[0],'E',items);
    items[j1][0].count=cnt;
    for(int i=0;i<items[j1][0].count;i++)
                               {
                                cout<<items[j1][i].a<<"->"<<items[j1][i].c<<"\n";
                               }*/
    while(j<=r)
    {
            int k=0;
            for(k=0;k<items[j][0].count;k++)
            {
                    for(int x=0;x<items[j][0].count;x++)
                    itm[x]=items[j][x];
                    int l;
                    for(l=0;l<strlen(items[j][k].c);l++)
                    {
                        if(items[j][k].c[l]=='.' && l<strlen(items[j][k].c)-1)
                        {
                           //if(items[j][k].c[l+1]>=65 && items[j][k].c[l+1]<=90)
                           //{
                              break;
                           //}
                        }
                    }
                    if(l<strlen(items[j][k].c)-1)
                    {
                       int f=0;
                       for(int m=0;m<y;m++)
                       {
                               if(j==v1[m]-48 && items[j][k].c[l+1]==v2[m])
                               {
                                   f=1;
                                   break;
                               }
                       }
                       if(f==0)
                       {
                               w++;
                               j1++;
//                               cout<<items[j][k].c[l+1]<<"--\n";
                               cnt=items[j1][0].count;
                               goto1(items[j],items[j][k].c[l+1]);
                               items[j1][0].count=cnt;
                               //cout<<"goto exit\n";
                               items[j][0].count=cn[0];
                               //cout<<"count="<<items[j][0].count<<"\n";
                               for(int x=0;x<cn[j];x++)
                               {
                               items[j][x]=itm[x];
                               //cout<<itm[x].a<<"-"<<itm[x].c[0]<<"  ";
                               }
                               /*cout<<"LR(0) items\n";
                               for(int i=0;i<=n;i++)
                               {
                                cout<<items[0][i].a<<"->"<<items[0][i].c<<"\n";
                                }*/
                               v1[y]=j+48;
                               v2[y++]=items[j][k].c[l+1];
                               
                               int fl=0;
                               int i1;
                               for( i1=0;i1<w;i1++)
                               {
                                       int i2;
                                       for(i2=0;i2<items[i1][0].count;i2++)
                                       {
                                               //cout<<"check "<<items[i1][i2].a<<" "<<items[w][i2].a<<","<<items[i1][i2].a<<" "<<items[w][i2].a<<"\n";
                                               if(items[i1][i2].a!=items[w][i2].a)
                                               {
                                                 // fl=1;
                                                  break;
                                               }
                                               if(strcmp(items[i1][i2].c,items[w][i2].c))
                                               {
                                                  //fl=1;
                                                  break;
                                               }
                                       }
                                       if(i2==items[i1][0].count)
                                       {
                                         fl=1;
                                         break;
                                         }
                               }
                               if(fl==0)
                               {
//                               cout<<"not same\n";
                               w1++;
                                //cout<<"w "<<w<<"\n";
                               
                               cn[++r]=items[w][0].count;
                               //cout<<"count "<<r<<"="<<cn[r]<<"\n";
                               char r1=r+'0';
                               items[w][0].cno[0]=r1;
                               items[w][0].cno[1]='\0';
                               items[w][0].no=r;
                               items[w][0].ino[items[w][0].samecnt]=j;
                               items[w][0].chr[items[w][0].samecnt]=items[j][k].c[l+1];
//                               cout<<"I"<<items[w][0].no<<"=goto(I"<<j<<","<<items[j][k].c[l+1]<<")\n";
//                               for(int i=0;i<items[w][0].count;i++)
//                               {
//                                cout<<items[w][i].a<<"->"<<items[w][i].c<<"\n";
//                               }
                               }
                               else
                               {
//                                   cout<<"same\n";
                                   items[i1][0].samecnt++;
                                   items[i1][0].ino[items[i1][0].samecnt]=j;
                                   items[i1][0].chr[items[i1][0].samecnt]=items[j][k].c[l+1];
//                                   cout<<"goto(I"<<j<<","<<items[j][k].c[l+1]<<")="<<i1<<"\n";
                                   items[j1][0].count=0;
                                   w--;
                                   j--;
                                   j1--;
                               }
                       }
                    }
                    
            }
    
    j++;
    items[j][0].count=cn[j];
    }
//    for(int o=1;o<=r;o++)
//    {
//            for(int o1=0;o1<=items[o][0].samecnt;o1++)
//            {
//                     cout<<"I"<<items[o][0].no<<"=goto(I"<<items[o][0].ino[o1]<<","<<items[o][0].chr[o1]<<")\n";
//            }
//    }
//    cout<<sym1<<sym2<<"\n";
//    for(int o=0;o<sym1;o++)
//    cout<<symbol[o]<<" ";
//    cout<<"\n";
//    for(int o=0;o<sym2;o++)
//    cout<<gsymbol[o]<<" ";
    first();
    follow();
    maketable();
    cout<<"----------first-----------\n";
    for(int i=0;i<sym2;i++)
    {
            cout<<gsymbol[i]<<"-";
            for(int i1=0;i1<fi[i].fcnt;i1++)
            {
                    cout<<fi[i].fo[i1]<<",";
            }
            cout<<"\n";
    }
    cout<<"----------follow-----------\n";
    for(int i=0;i<sym2;i++)
    {
            cout<<gsymbol[i]<<"-";
            for(int i1=0;i1<fol[i].fcnt;i1++)
            {
                    cout<<fol[i].fo[i1]<<",";
            }
            cout<<"\n";
    }
    cout<<"\n\t\taction\n  ";
    for(int i=0;i<sym1;i++)
    cout<<symbol[i]<<"\t";
    cout<<"\n";
    for(int i=0;i<=r;i++)
    {
            cout<<i<<" ";
            for(int i1=0;i1<sym1;i1++)
            {
                 cout<<tab.action[i][i1]<<tab.action1[i][i1]<<"\t";
            }
            cout<<"\n";
    }  
    cout<<"\n\tgoto\n  ";
    for(int i=0;i<sym2;i++)
    cout<<gsymbol[i]<<"\t";
    cout<<"\n";
    for(int i=0;i<=r;i++)
    {
            cout<<i<<" ";
            for(int i1=0;i1<sym2;i1++)
            {
                 cout<<tab.got[i][i1]<<"\t";
            }
            cout<<"\n";
    } 
    char st[50];
    int top=0;
    return 0;
	cout<<"enter input string\n"; 
	cin>>st;
	strcpy(stack[0].content,"0");
	strcpy(input[0].content,st);
	strcat(input[0].content,"$");
	int i=0;
	int digit=0;
	while(1)
	{
		cout<<stack[i].content<<"\t\t"<<input[i].content<<"\t";
		char chr=input[i].content[0];
		int i1,i2;
		int i7=stack[i].content[top]-'0';
		int i8=stack[i].content[top-1]-'0';
		if(digit==1)
		{
			i2=i8*10+i7;
			digit=0;
		}
		else
		i2=i7;
		for(i1=0;i1<sym1;i1++)
		{
			if(symbol[i1]==chr)
			break;
		}
		//cout<<"i2 i1 "<<i2<<","<<i1<<"\n";
		
		output[i].content[0]=tab.action[i2][i1];
		output[i].content[1]=tab.action1[i2][i1]+'0';
		output[i].content[2]='\0';
		cout<<output[i].content<<"\n";
		if(output[i].content[0]=='A')
		break;
		if(tab.action[i2][i1]=='S')
		{
			i++;
			for(int j3=0;j3<=top;j3++)
			{
				stack[i].content[j3]=stack[i-1].content[j3];
			}
			stack[i].content[++top]=chr;
			stack[i].content[++top]=tab.action1[i2][i1]+'0';
			int i3;
			for(i3=0;i3<strlen(input[i-1].content)-1;i3++)
			{
				input[i].content[i3]=input[i-1].content[i3+1];
			}
			input[i].content[i3]='\0';
		}
		else if(tab.action[i2][i1]=='R')
		{
			int num=tab.action1[i2][i1];
			int len=strlen(d1[num-1].c);
			for(int i4=0;i4<=top;i4++)
			{
				if(stack[i].content[i4]==d1[num-1].c[0])
				{
					//cout<<"find match\n";
					int flg=0;
					for(int i5=1,i6=i4+1;i5<len;i5++,i6++)
					{
						if((stack[i].content[i6]>=48 && stack[i].content[i6]<=57) )
						{
							i5--;
							continue;
						}
						if(stack[i].content[i6]!=d1[num-1].c[i5])
						{
							flg=1;
							break;
						}
					}
					if(flg==0)
					{
						//cout<<"match\n";
						top=i4;
						i++;
						for(int j3=0;j3<=top;j3++)
						{
							stack[i].content[j3]=stack[i-1].content[j3];
						}
						stack[i].content[top]=d1[num-1].a;
						int j2;
						for(j2=0;j2<sym2;j2++)
						{
							if(gsymbol[j2]==d1[num-1].a)
							break;
						}
						if(tab.got[stack[i].content[i4-1]-'0'][j2]<=9)
						{
						stack[i].content[++top]=tab.got[stack[i].content[i4-1]-'0'][j2]+'0';
						//cout<<stack[i].content[i4-1]<<" "<<j2<<" "<<tab.got[stack[i].content[i4-1]-'0'][j2]<<"\n";
						int x1=top+1;
						stack[i].content[x1]='\0';
						}
						else
						{
							
							int n1=tab.got[stack[i].content[i4-1]-'0'][j2]/10;
							int n2=tab.got[stack[i].content[i4-1]-'0'][j2]%10;
							char q1=n1+'0';
							char q2=n2+'0';
							stack[i].content[++top]=q1;
							stack[i].content[++top]=q2;
							int x1=top+1;
							stack[i].content[x1]='\0';
							digit=1;
							//cout<<digit<<"d\n";
						}
						//cout<<stack[i].content<<"\n";
						int i3;
						for(i3=0;i3<strlen(input[i-1].content);i3++)
						{
							input[i].content[i3]=input[i-1].content[i3];
						}
						input[i].content[i3]='\0';
						
						//cout<<tab.got[0][0]<<tab.got[0][1]<<tab.got[0][2]<<tab.got[1][2]<<"\n";
					}
				}
			}
		}
	}
    system("pause");
    return 0;
}
                               
            
            
                                      
