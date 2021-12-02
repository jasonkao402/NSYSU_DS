/*
Author¡G °ª¸t³Ç(Jason) B093040016
Date¡G2020/11/9
Purpose¡G hw6 binary search tree
*/
#include <bits/stdc++.h>
#define EPS 1e-7
#define pi acos(-1.0)
#define endl '\n'
#define LL long long
#define ULL unsigned long long
#define _ps system("pause")
#define IOS ios::sync_with_stdio(0),cin.tie(0);

using namespace std;

struct node
{
	node(){}
	node(int input){
		v = input;
		l = NULL, r = NULL;
	}
	int v = 0;
	node *l = NULL, *r = NULL;
};

int n, isfirst;
string in, pre;
node* root;
vector<int> numv;

void delNode(node *pre, node *cur);

void addNode(node *pre, node *cur, int nv)
{
	if(cur->v == 0)  cur->v = nv;
	//go left or right
	else if(nv > cur->v)
	{
		if(!cur->r)  cur->r = new node(nv);
		else  addNode(cur, cur->r, nv);
	}
	else if(nv < cur->v)
	{
		if(!cur->l)  cur->l = new node(nv);
		else  addNode(cur, cur->l, nv);
	}
	//find same val, delete!
	else  delNode(pre, cur);
}

void delNode(node *pre, node *cur)
{
	//no l, no r
	if(!cur->l && !cur->r)
	{
		//when cur == root, deleting cur causes bugs(no root to find)
		//set 0 to prevent empty root.
		if(!pre)  cur->v = 0;
		else 
		{
			if(pre->l == cur)
				pre->l = NULL;
			else
				pre->r = NULL;
			
			delete cur;
		}
	}
	//both l and r
	else if(cur->l && cur->r)
	{
		node *succ = cur->r;
        while (succ->l) {
            succ = succ->l; 
        }
        //append current left after succ left
        succ->l = cur->l;
        if(!pre)  root = cur->r;
        //replace cur right tree with current node
        else if(pre->l == cur)
        	pre->l = cur->r;
		else
			pre->r = cur->r;
		
		delete cur;
	}
	//only l or r
	else
	{
		if(!pre)  root = cur->l ? cur->l : cur->r;
		else 
		{
			if(pre->l == cur)
				pre->l = cur->l ? cur->l : cur->r;
			else
				pre->r = cur->l ? cur->l : cur->r;
			
			delete cur;
		}
	}
}

void travel(node *cur, vector<int> &numv)
{
    if(cur!=NULL)
	{
        //visit left node
        travel(cur->l, numv);
        //current node
        if(cur->v != 0)
        {
        	numv.push_back(cur->v);
	        //check l and r
	        if(!cur->l)
	            numv.push_back(0);
	        else
	            numv.push_back(cur->l->v);
			
	        if(!cur->r)
	            numv.push_back(0);
	        else
	            numv.push_back(cur->r->v);	
		}
        //visit right node
        travel(cur->r, numv);
    }
}
//debug function
void inorderTest(node *root)
{
	if(root != NULL)
	{
		inorderTest(root->l);
		printf("%2d ", root->v);
		inorderTest(root->r);
	}
}

int main(){
	//freopen("hw6-b093040016.out", "w", stdout);
	isfirst = 0, root = new node(0);
	while(scanf("%d", &n) != EOF)
	{
		if(n == -1)
		{
			travel(root, numv);
			if(isfirst++)  printf("\n");
			for(int i = 0; i < 3 ; i++)
			{
        		if(i == 0){
                	printf("node :");
					for(int j = i, k = numv.size(); j < k ; j+=3){
		            	printf("%2d ", numv[j]) ;
		        	}
                }
		        else if(i == 1)
				{
		      		printf("left :");
		      		for(int j = i, k = numv.size(); j < k ; j+=3){
		            	printf("%2d ", numv[j]) ;
		        	}
		        }
		        else
				{
		            printf("right:");
		        	for(int j = i, k = numv.size(); j < k ; j+=3){
		            	printf("%2d ", numv[j]) ;
		        	}
		        }
		        printf("\n");
    		}
    		//new tree for next case
    		root = new node(0);
    		numv.clear();
		}
		else
		{
			addNode(NULL, root, n);
			//inorderTest(root);
			//printf("\n");
		}
	}
    return 0;
}
