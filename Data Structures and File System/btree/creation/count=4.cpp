else if (T->count==4)
			{
				btnode *n;
				n=new btnode;
				for (int i=0;i<4;i++)
				n->key[i]=-1;
				for (int i=0;i<5;i++)
				n->child[i]=NULL;
				T->count=2;
				n->count=1;
				if (T->key[2]>m->key[0])
				{
					n->child[0]=T;
					n->child[1]=new btnode;
					n->child[1]->key[0]=T->key[2];
					n->child[1]->key[1]=T->key[3];
					n->child[1]->child[0]=T->child[2];
					n->child[1]->child[1]=T->child[3];
					n->child[1]->child[2]=T->child[4];
					T->key[2]=-1;
					T->key[3]=-1;
					n->child[1]->key[2]=-1;
					n->child[1]->key[3]=-1;
					if (m->key[0]>T->key[1])
					{
						n->key[0]=m->key[0];
						n->child[1]->key[2]=-1;
						n->child[1]->key[3]=-1;
					}
					else
					{
						n->key[0]=T->key[1];
						if (T->key[0]<m->key[0])
						T->key[1]=m->key[0];
						else
						{
							T->key[1]=T->key[0];
							T->key[0]=m->key[0];
						}
					}
					n->child[1]->count=2;
					T=n;
					return n;
				}
				else 
				{
					n->key[0]=T->key[2];
					n->child[0]=T;
					n->child[1]=new btnode;
					n->child[1]->count=2;
					if (m->key[0]<T->key[3])
					{
						n->child[1]->key[0]=m->key[0];
						n->child[1]->key[1]=T->key[3];
					}
					else
					{
						n->child[1]->key[0]=T->key[3];
						n->child[1]->key[1]=m->key[0];
					}
					T->key[2]=-1;
					T->key[3]=-1;
					n->child[1]->key[2]=-1;
					n->child[1]->key[3]=-1;
					T=n;
					return n;
				}
			}
