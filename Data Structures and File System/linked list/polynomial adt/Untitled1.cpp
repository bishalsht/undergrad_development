#include <iostream>
using namespace std;

struct pnode{
	int coeff;
	int exp;
	pnode *next;
};

void add_begin(pnode *&S, int k, int m)
{
	pnode *T;
	T=new (pnode);
	T->coeff=k;
	T->exp=m;
	T->next=S;
 	S=T;
}

void add_end(pnode *S, int k, int m)
{
	pnode *T;
	T=new (pnode);
	T->coeff=k;
	T->exp=m;
	T->next=NULL;
	while (S->next!=NULL)
	S=S->next;
	S->next=T;
}

void print(pnode *P)
{
	while (P->next!=NULL)
	{
		if (P->exp)
		cout<<P->coeff<<"x^"<<P->exp;
		else if (!P->exp)
		cout<<P->coeff;
		if (P->next->coeff>0)
		cout<<"+";
		P=P->next;
	}
	if (P->exp)
	cout<<P->coeff<<"x^"<<P->exp;
	else if (!P->exp)
	cout<<P->coeff;
}

void addition(pnode *P1, pnode *P2)
{
	pnode *T,*sum;
	sum=new (pnode);
	sum->next=NULL;
	T=P2;
	while (P1->exp!=T->exp&&T->next!=NULL)
	 	 T=T->next; 
	if (P1->exp==T->exp)
		sum->coeff=P1->coeff+T->coeff;
	else
	sum->coeff=P1->coeff;
	sum->exp=P1->exp;
	P1=P1->next;
	while (P1!=NULL)
	{
	    T=P2;
		while (P1->exp!=T->exp&&T->next!=NULL)
			T=T->next;
		if (P1->exp==T->exp)
		add_end(sum,P1->coeff+T->coeff,P1->exp);
		else
		add_end (sum,P1->coeff,P1->exp);
		P1=P1->next;
	}
	pnode *T2;
	T=P2;
	while (T!=NULL)
	{
		T2=sum;
		int tag=1;
		while (T2!=NULL)		
		{
			if (T->exp==T2->exp)
			{
				tag=0;
				break;
			}
			T2=T2->next;
		}
		if (tag)
		add_end(sum,T->coeff,T->exp);
		T=T->next;
	}
	cout<<endl<<"Sum: ";
	print (sum);
}

void subtraction(pnode *P1, pnode *P2)
{
	pnode *T,*diff;
	diff=new (pnode);
	diff->next=NULL;
	T=P2;
	while (P1->exp!=T->exp&&T->next!=NULL)
	 	 T=T->next; 
	if (P1->exp==T->exp)
		diff->coeff=P1->coeff-T->coeff;
	else
	diff->coeff=P1->coeff;
	diff->exp=P1->exp;
	P1=P1->next;
	while (P1!=NULL)
	{
	    T=P2;
		while (P1->exp!=T->exp&&T->next!=NULL)
			T=T->next;
		if (P1->exp==T->exp)
		add_end(diff,P1->coeff-T->coeff,P1->exp);
		else
		add_end(diff,P1->coeff,P1->exp);
		P1=P1->next;
	}
	pnode *T2;
	T=P2;
	while (T!=NULL)
	{
		T2=diff;
		int tag=1;
		while (T2!=NULL)		
		{
			if (T->exp==T2->exp)
			{
				tag=0;
				break;
			}
			T2=T2->next;
		}
		if (tag)
		add_end(diff,-T->coeff,T->exp);
		T=T->next;
	}
	cout<<endl<<"Difference: ";
	print (diff);
}

void multiplication(pnode *P1,pnode *P2)
{
	pnode *product, *T, *temp;
	product=new (pnode);
	product->next=NULL;
	product->coeff=P1->coeff*P2->coeff;
	product->exp=P1->exp+P2->exp;
	temp=P2;
	P2=P2->next;
	while (P1!=NULL)
	{
		while (P2!=NULL)
		{
			T=product;
			while (T!=NULL)
			{
				if (T->exp==P1->exp+P2->exp)
				{
					T->coeff=T->coeff+(P1->coeff*P2->coeff);
					break;
				}
				T=T->next;
			}
			if (T==NULL){
			add_end(product,P1->coeff*P2->coeff,P1->exp+P2->exp);
			}
			P2=P2->next;
		}
		P1=P1->next;
		P2=temp;
	}
	cout<<"\nProduct: ";
	print (product);
}

int main()
{
	pnode *P1, *P2;
	int coeff,exp;
	cout<<"enter coeff and exp (enter exp -1 to stop): ";
	cin>>coeff>>exp;
	P1=new (pnode);
	P1->next=NULL;
	P1->coeff=coeff;
	P1->exp=exp;
	cin>>coeff>>exp;
	while (exp!=-1)
	{
		add_end(P1,coeff,exp);
		cin>>coeff>>exp;
	}
	cout<<"1st expression: ";
	print (P1);
	
	cout<<"\nenter coeff and exp (enter exp -1 to stop): ";
	cin>>coeff>>exp;
	P2=new (pnode);
	P2->next=NULL;
	P2->coeff=coeff;
	P2->exp=exp;
	cin>>coeff>>exp;
	while (exp!=-1)
	{
		add_end(P2,coeff,exp);
		cin>>coeff>>exp;
	}
	cout<<"2nd expression: ";
	print (P2);
	
	addition(P1,P2);
	
	subtraction(P1,P2);
	
	multiplication(P1,P2);
	
	return 0;
}
