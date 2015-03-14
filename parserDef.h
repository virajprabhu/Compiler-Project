/*PUSHKARA RAVINDRA 2011A7PS150P
 *VIRAJ PRABHU	    2011A7PS044P
 * BATCH-1*/
/*parserDef.h*/
#define ntCount 45
#define tCount 39 
int ruleCount;
int flag;			//Error flag

typedef struct node node;
struct node
{
  char symbol[30];
  struct node* next;		//Points to next node
  struct node* previous;	//Points to previous node
  struct node* last;		//Points to last node
};				

typedef node** grammar;		//An array of pointers to the head of each production
grammar G;
typedef struct NT NT;
struct NT
{
  char name[30];
  int has_e;			//Whether e is a part of the first set
  struct node* first;
  struct node* follow;
};

typedef struct tnode tnode;
struct tnode
{
  char name[30];
  tokenInfo tok;
  struct tnode* peer;
  struct tnode* child;
  struct ast_node* astnode;
};
typedef tnode *parseTree;
typedef int Table;

typedef struct _stack _stack;
typedef _stack *Stack;
struct _stack
{
  tnode *tno;
  Stack next;
};


