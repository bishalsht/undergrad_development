#include <iostream>
using namespace std;

struct gnode{
	int data;
	gnode *next;
};

void add_end(gnode *S, int k)
{
	gnode *T;
	T=new gnode;
	T->next=NULL;
	T->data=k;
	while (S->next!=NULL)
		  S=S->next;
    S->next=T;
}

void del(gnode *&S, int x)
{
	if (S->data==x)
	S=S->next;
	else
	{
		gnode *T=S;
		while (T->next->data!=x)
		T=T->next;
		T->next=T->next->next;
	}
}

void splce(gnode *path,gnode *ckt)
{
	while (path->data!=ckt->data)
	path=path->next;
	gnode *temp=ckt;
	while (ckt->next!=NULL)
	ckt=ckt->next;
	ckt->next=path->next;
	path->next=temp->next;
}

class queue{	
	public:
		int size, front, rear, count;
		int element[50];
		queue(){
			rear=-1;
			front=-1;
			size=50;
			count=0;
		}
		void enqueue(int e){
			if (isqFull())
			   cout<<"Queue Overflow";
            else
            {
            	rear=(rear+1)%size;
            	element[rear]=e;
            	if (front==-1) front++;
            	count++;
            }
		}
		int delqueue(){
			if (isqEmpty())
			   cout<<"Queue Underflow";
			else{
				count--;
				int l=element[front];
				if (front==rear)
				{
					front=-1;
					rear=-1;
				}
				else
				front=(front+1)%size;
				return l;
			}
			return NULL;
		}
		bool isqFull(){
			if ((front==rear+1)||(front==0&&rear==size-1))
			return 1;
			return 0;
		}
		bool isqEmpty(){
			if (rear==-1)
			return 1;
			return 0;
		}
		int frontelement(){
			if (!isqEmpty())
			return element[front];
			else cout<<"no front element";
			return 0;
		}
};

int main()
{
	gnode *g[6]={NULL};
	bool visited[6]={false};
	g[0]=new gnode;
	g[0]->data=1;
	g[0]->next=new gnode;
	g[0]->next->data=2;
	g[0]->next->next=new gnode;
	g[0]->next->next->data=3;
	g[0]->next->next->next=new gnode;
	g[0]->next->next->next->data=4;
	g[0]->next->next->next->next=NULL;
	
	g[1]=new gnode;
	g[1]->data=0;
	g[1]->next=new gnode;
	g[1]->next->data=2;
	g[1]->next->next=new gnode;
	g[1]->next->next->data=3;
	g[1]->next->next->next=new gnode;
	g[1]->next->next->next->data=4;
	g[1]->next->next->next->next=NULL;	
	
	g[2]=new gnode;
	g[2]->data=0;
	g[2]->next=new gnode;
	g[2]->next->data=1;
	g[2]->next->next=new gnode;
	g[2]->next->next->data=3;
	g[2]->next->next->next=new gnode;
	g[2]->next->next->next->data=5;
	g[2]->next->next->next->next=NULL;	
	
	g[3]=new gnode;
	g[3]->data=0;
	g[3]->next=new gnode;
	g[3]->next->data=1;
	g[3]->next->next=new gnode;
	g[3]->next->next->data=2;
	g[3]->next->next->next=new gnode;
	g[3]->next->next->next->data=4;
	g[3]->next->next->next->next=NULL;	
	
	g[4]=new gnode;
	g[4]->data=0;
	g[4]->next=new gnode;
	g[4]->next->data=1;
	g[4]->next->next=new gnode;
	g[4]->next->next->data=3;
	g[4]->next->next->next=new gnode;
	g[4]->next->next->next->data=5;
	g[4]->next->next->next->next=NULL;	
	
	g[5]=new gnode;
	g[5]->data=2;
	g[5]->next=new gnode;
	g[5]->next->data=4;
	g[5]->next->next=NULL;
	
	cout<<"Adjacency List\n";
	for (int i=0;i<6;i++)
	{
		gnode *temp;
		temp=g[i];
		cout<<"Links of "<<i<<":  ";
		while  (temp!=NULL)
		{
			cout<<temp->data<<"  ";
			temp=temp->next;
		}
		cout<<endl;
	}
	
	int odd_deg=0, odd_node[2];
	for (int i=0;i<6;i++)
	{
		gnode *temp;
		temp=g[i];
		int deg=0;
		while (temp!=NULL)
		{
			deg++;
			temp=temp->next;
		}
		if (deg%2!=0) odd_node[odd_deg++];
	}
	
	cout<<"Number of odd degree nodes: "<<odd_deg;;
	if (odd_deg>2) return 0;
	
	gnode *e_path=new gnode;
	queue q;
	
	if (odd_deg==0)
	{
		q.enqueue(0);
		e_path->data=0;
		e_path->next=NULL;
	}
	else
	{
		q.enqueue(odd_node[0]);
		visited[odd_node[0]]=true;
		e_path->data=odd_node[0];
		e_path->next=NULL;
		int k=odd_node[0];
		gnode *temp;
		while (k!=odd_node[1])
		{
			temp=g[k];
			while (visited[temp->data]==true)
			temp=temp->next;
			visited[temp->data]=true;
			int z=temp->data;
			del(g[k],z);
			del(g[z],k);
			k=z;
			q.enqueue(k);
			add_end(e_path,k);
		}
	}
	
	while (!q.isqEmpty())
	{
		int m=q.delqueue();
		gnode *ckt=new gnode;
		ckt->data=m;
		ckt->next=NULL;
		while (g[m]!=NULL)
		{
			int n=g[m]->data;
			g[m]=g[m]->next;
			del(g[n],m);
			add_end(ckt,n);
			m=n;
		}
		splce (e_path,ckt);
		for (int i=0;i<6;i++)
		{
			if (g[i]!=NULL)
			{
				q.enqueue(i);
				break;
			}
		}
	}
	
	cout<<"\nPath for the tree: ";
	while (e_path!=NULL)
	{
		cout<<e_path->data<<" ";
		e_path=e_path->next;
	}
	
	return 0;
}
