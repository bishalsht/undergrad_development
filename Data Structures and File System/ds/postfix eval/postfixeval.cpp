//postfix eval

#include <iostream>
using namespace std;

class stack{
	public:
	int size,top;
	double element[100];
	void push (char x)
	{
		if (top>size)
		cout<<"Stack is full";
		else
		element[++top]=x;
	}
	int pop()
	{
		if (top==-1)
		cout<<"Empty Stack";
		else
		return (element[top--]);
	}
	stack (int s)
	{
		for (int m=0;m<100;m++)
		element[m]='\0';
		size=s;
		top=-1;
	}
};

int main()
{
	char a[100],c[50];
	double i=0,j=0,total=0,num1,num2;
	stack b(100);
	cin.getline(a,100);
	for (int w=0;a[w]!='\0';w++)
	{
		switch (a[w])
		{
			case '1':b.push(1); break;
			case '2':b.push(2); break;
			case '3':b.push(3); break;
			case '4':b.push(4); break;
			case '5':b.push(5); break;
			case '6':b.push(6); break;
			case '7':b.push(7); break;
			case '8':b.push(8); break;
			case '9':b.push(9); break;
			case '0':b.push(0); break;
			case '+': num1=b.pop(); 
				 	  num2=b.pop();
				 	  b.push(num1+num2);
				 	  break;
			case '*': num1=b.pop(); 
				 	  num2=b.pop();
				 	  b.push(num1*num2);
				 	  break;
			case '-': num1=b.pop(); 
				 	  num2=b.pop();
				 	  b.push(num2-num1);
				 	  break;
			case '/': num1=b.pop(); 
				 	  num2=b.pop();
				 	  b.push(num2/num1);
				 	  break;
	        default:break;
		}
	}
	cout<<endl<<b.element[b.top];
	return 0;
}
