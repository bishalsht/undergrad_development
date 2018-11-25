#include <iostream>
using namespace std;

class stack{
	int element[50],top,size;
	public:
		stack(){
			top=-1;
			size=50;
		}
		void push(int a){
  			 if (!isFull()) element[++top]=a;
		     else cout<<"Overflow";
		}
		int pop(){
		    if (!isEmpty()) return element[top--];
		    else cout<<"Underflow";
		    return 0;
		}
		bool isEmpty(){
		    if (top==-1) return 1;
		    return 0;
		}
		bool isFull(){
		    if (top==49) return 1;
		    return 0;
		}
		char topelement(){
		    return element[top];
		}
};

int main()
{
	stack s1,s2;
	int temp2,temp1,count=0,j;
	bool ans=1;
	cout<<"Enter element:";
	while (!s1.isFull()&&ans)
	{
		cin>>j;
		s1.push(j);
		cout<<"Enter again? (1 for yes and 0 for no)";
		cin>>ans;
		count++;
	}
	for (int i=0;i<count;i++)
	{
		while (!s1.isEmpty())
		{
			s2.push(s1.pop());
			if (s1.topelement()>s2.topelement())
			{
				temp1=s1.pop();
				temp2=s2.pop();
				s1.push(temp2);
				s2.push(temp1);
			}
		}
		while (!s2.isEmpty())
		{
			s1.push(s2.pop());
			if (s1.topelement()>s2.topelement())
			{
				temp1=s1.pop();
				temp2=s2.pop();
				s1.push(temp2);
				s2.push(temp1);
			}
		}
	}
	while (!s1.isEmpty())
		s2.push(s1.pop());
	while (!s2.isEmpty())
	cout<<s2.pop()<<endl;
	return 0;
}
