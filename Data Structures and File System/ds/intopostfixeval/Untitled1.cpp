#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctype.h>

using namespace std;

const int MAX = 50 ;

class postfix
{
	private :

		int stack[MAX] ;
		int top, nn ;
		char *s ;

	public :

		postfix( ) ;
		void setexpr ( char *str ) ;
		void push ( int item ) ;
		int pop( ) ;
		void calculate( ) ;
		void show( ) ;
} ;

// initializes data members
postfix :: postfix( )
{
	top = -1 ;
}

// sets s to point to the given expr.
void postfix :: setexpr ( char *str )
{
	s = str ;
}

// adds digit to the stack
void postfix :: push ( int item )
{
	if ( top == MAX - 1 )
		cout << endl << "Stack is full" ;
	else
	{
		top++ ;
		stack[top] = item ;
	}
}

// pops digit from the stack
int postfix :: pop( )
{
	if ( top == -1 )
	{
		cout << endl << "Stack is empty" ;
			return NULL ;
	}
	int data = stack[top] ;
	top-- ;
	return data ;
}

// evaluates the postfix expression
void postfix :: calculate( )
{
	int n1, n2, n3 ;
	while ( *s )
	{
		/* skip whitespace, if any */
		if ( *s == ' ' || *s == '\t' )
		{
			s++ ;
			continue ;
		}

		/* if digit is encountered */
		if ( isdigit ( *s ) )
		{
			nn = *s - '0' ;
			push ( nn ) ;
		}
		else
		{
			/* if operator is encountered */
			n1 = pop( ) ;
			n2 = pop( ) ;
			switch ( *s )
			{
				case '+' :
					  n3 = n2 + n1 ;
					  break ;

				case '-' :
					  n3 = n2 - n1 ;
					  break ;

				case '/' :
					  n3 = n2 / n1 ;
					  break ;

				case '*' :
					  n3 = n2 * n1 ;
					  break ;

				case '%' :
					  n3 = n2 % n1 ;
					  break ;

				case '^' :
					  n3 = pow ( n2 , n1 ) ;
					  break ;

				default :
					  cout << "Unknown operator" ;
					  exit ( 1 ) ;
			}

			push ( n3 ) ;
		}
		s++ ;
	}
}

// displays the result
void postfix :: show( )
{
	nn = pop ( ) ;
	cout << "Result is: " << nn ;
}

class stac{
    int maximum;
    int top;
    char element[50];

    public:
    char topelement();
    bool isFull();
    bool isEmpty();
    void push(char a);
    char pop();

    stac(){
        maximum=50;
        top=-1;
    }
};

int main()
{
    stac s1;
    char expr[100] ;
    int n=0;
    char infix[100];
    cout<<"Enter:";
    cin>>infix;
    for (int i=0;i<strlen(infix);i++){
        if (infix[i]!='+'&&infix[i]!='*'&&infix[i]!='('&&infix[i]!=')'&&infix[i]!='-'&&infix[i]!='/'){
            expr[n++]=infix[i];
        }
        if (infix[i]=='(')
                s1.push(infix[i]);
        if (infix[i]==')'){
            while (s1.topelement()!='('){
                expr[n++]=s1.pop();
            }
            s1.pop();
        }
        if (infix[i]=='*'||infix[i]=='/'){
            s1.push(infix[i]);
        }
        if (infix[i]=='+'||infix[i]=='-'){
            if (s1.topelement()=='*'||s1.topelement()=='/'){
                while (!s1.isEmpty()){
                expr[n++]=s1.pop();
            }
            }
                s1.push(infix[i]);
        }
    }
    while (!s1.isEmpty()){
                expr[n++]=s1.pop();
            }
expr[n]='\0';

	cout << "\nEnter postfix expression to be evaluated : " ;
	cout<<expr;

	postfix q ;

	q.setexpr ( expr ) ;
	q.calculate( ) ;
	q.show( ) ;


    return 0;
}

void stac::push(char a){
    if (!isFull()) element[++top]=a;
    else cout<<"Overflow";
}
char stac::pop(){
    if (!isEmpty()) return element[top--];
    else cout<<"Underflow";
    return 0;
}
bool stac::isEmpty(){
    if (top==-1) return 1;
    return 0;
}
bool stac::isFull(){
    if (top==49) return 1;
    return 0;
}
char stac::topelement(){
    return element[top];
}
