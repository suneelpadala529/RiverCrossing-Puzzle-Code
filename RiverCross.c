#include<stdlib.h>
#include <stdio.h>
#include <limits.h>
#include<stdbool.h>
/* (WCGM)
0000-0: 1111:15
WCSF- 0000 and 1111 - state 0 and 15
WC|SF- 0011 and 1100- state 3 and 12
W|CSF- 0111 and 1000- state 7 and 8
WSF|C- 0100 and 1011- state 4 and 11
WCF|S- 0010 and 1101- state 2 and 13
other states defy rules of the puzzle
valid or finite state moves: 0,3; 12,15; 7,4; 8,11; 3,2; 12,13; 7,2; 8,13; 4,13; 2,11; 
*/
#define STARTST 0  // start state is 0 
#define ENDST 15   // end state is 15 
#define TOTSTATES 16


typedef struct node
{
	int val;
	struct node *link;
} NODE;

void printallstates (int s, int e, int *prd);
void printstate(int s);
void traverse ( NODE *p );
void printgraph ( NODE **a );
int validornot(int j);
int validedges(int i, int j);
NODE** state_adjacency_list( );
void possible_paths_to_b2(int , int , NODE**); 
void search_for_paths(int u, int d, int path[], bool visited[], int path_index, NODE **adjlist);


int validornot(int j)
{
	//check wheather someone is eaten or not
	int wolfeatsgoat = (( (j) >> (0) ) & 1)^(( (j) >> (3) ) & 1) && (( (j) >> (0) ) & 1)^(( (j) >> (1) ) & 1);
	int goateatscabbage = (( (j) >> (0) ) & 1)^(( (j) >> (1) ) & 1) && (( (j) >> (0) ) & 1)^(( (j) >> (2) ) & 1);
	int forbidden = wolfeatsgoat || goateatscabbage;
	return forbidden;
}

int validedges(int i, int j)
{
	/* returns 1 if i-->j is edge (valid move), 0 else */
	int s;
	int Fmoves, moveswithF, k;
	//check if man moves
	Fmoves = (( (i) >> (0) ) & 1) ^ (( (j) >> (0) ) & 1);
	// check if man moves with items
	moveswithF = 1;    /* initially assume correct */
	for (k=0;k<4;k++)
		if ( (( (i) >> (k) ) & 1)^(( (j) >> (k) ) & 1)&& (( (j) >> (0) ) & 1)^(( (j) >> (k) ) & 1))
		{
			moveswithF = 0;
			break;
		}
	//count items moved
	int r=i ^ j;
	s= (( (r) >> (0) ) & 1) +(( (r) >> (1) ) & 1) +(( (r) >> (2) ) & 1) +(( (r) >> (3) ) & 1) ;
	int forbidden = validornot(j);
	int forbstartstate= validornot(i);
	return(Fmoves && !forbidden && s < 3 && moveswithF && !forbstartstate);
}

NODE** state_adjacency_list( )
{
	/* state_adjacency_list() return an array of pointers to NODE, */

	int i,j;
	NODE **a;   /* a is array of pointers to NODE */
	NODE *p;    /* pointer p used for creating new node */
	int validedges(int i, int j); /* prototype */

	a = (NODE **) calloc(TOTSTATES,sizeof(NODE*));
	for (i=0;i<TOTSTATES;i++)
	{
		a[i] = NULL;    /* initialize adjacency list for i to be empty */
		for (j=TOTSTATES-1;j>=0;j--)
		{
			if ( !validedges(i,j) )
				continue;  /* go on to next j if i-->j not edge */
			p = (NODE *) malloc(sizeof(NODE));
			p->val = j;
			p->link = a[i]; /* if adj list empty then p->link is NULL */
			a[i] = p;
		}
	}
	return a;
}

void printstate(int x)
{
	/* prints the state with occupants  at both sides of the river*/
	if ((( (x) >> (3) ) & 1)  == 0) printf("Wolf ");
	if ((( (x) >> (2) ) & 1)  == 0) printf("Cabbage ");
	if ((( (x) >> (1) ) & 1)  == 0) printf("Goat ");
	if ((( (x) >> (0) ) & 1)  == 0) printf("man");
	printf(" || ");
	if ((( (x) >> (3) ) & 1)  == 1) printf("Wolf ");
	if ((( (x) >> (2) ) & 1)  == 1) printf("Cabbage ");
	if ((( (x) >> (1) ) & 1)  == 1) printf("Goat ");
	if ((( (x) >> (0) ) & 1)  == 1) printf("man");
	printf("\n");
}

void traverse ( NODE *p )
{
	//traverses adjacency list pointed to by p 
	if (p != NULL)
	{
		printf("%d, ",p->val);
		traverse(p->link);
	}
}

void printgraph ( NODE **a )
{
	// for each vertex i of the graph G prints adjacency list for i 
	int i;
	for (i=0;i<TOTSTATES;i++)
	{
		printf("%d:\t",i);
		traverse(a[i]);
		printf("\n");
	}
}

void possible_paths_to_b2(int s, int d, NODE ** adjlist) 
{ 
    // Mark all the vertices as not visited 
   bool *visited =(bool *) calloc(TOTSTATES,sizeof(bool));
  
    //list to store states which are visited
    int *path = (int *) calloc(TOTSTATES,sizeof(int));
    int path_index = 0; // Initialize path[] as empty 
  
    // Initialize all states as not visited 
    for (int i = 0; i < TOTSTATES; i++) 
        visited[i] = false; 
  
    // to print all paths from source to destination
    search_for_paths(s, d, path, visited, path_index,adjlist); 
} 

void search_for_paths(int source, int dest, int path[], bool visited[], int path_index, NODE **adjlist) 
{ 
    // current node marked as visited and stored in path[] 
    visited[source] = true; 
    path[path_index] = source; 
    path_index++; 
    // If current state is same as destination state, then print path
    if (source == dest) 
    { 
        printf("one possible path to reach other side of river \n ");
        printf("====================================================\n ");
        for (int i = 0; i<path_index; i++) 
        {
            printf("state =%d =>", path[i]);
       		printstate(path[i]);
       	}
        printf("\n");           
    } 
    // If current vertex is not destination 
    else 
    { 	
        NODE *i;
    	i=adjlist[source];
        // Recur for all the vertices adjacent to current vertex 
        while(i!= NULL)
        {
            if (!(visited[i->val])) 
                search_for_paths(i->val, dest, path, visited, path_index, adjlist); 
            i = i->link;
        }
    }  
    // Remove current vertex from path[] and mark it as unvisited 
    path_index--; 
    visited[source] = false; 
}

int main(int argc, char **argv)
{
	NODE **adjlist; //adj list
	adjlist = state_adjacency_list();
    printf("==================== Adjacency list representation of obtained Graph========================\n");
	printgraph(adjlist);
    printf("============================================================================================\n");
    printf("==========The paths from INITIAL STATE:All on the left bank of the river to FINAL STATE: All on the right bank ============== \n");
    possible_paths_to_b2(STARTST, ENDST, adjlist); 
	return 0;
}