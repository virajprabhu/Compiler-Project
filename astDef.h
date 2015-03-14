/*ast.h*/
#ifndef _ASTDEF_H
#define _ASTDEF_H
typedef struct ast_node ast_node;
typedef struct hashTable hashTable;
typedef struct symbolTable symbolTable;
struct hashTable
{
  char type[20];
  char name[30];
  symbolTable* value;
  ast_node* pt;
  int size;
  int offset;
};
struct symbolTable
{		  //for reference, add the name of the function whose symbol table it is.
  hashTable H[50];
  hashTable inVar[5];
  hashTable outVar[5];
  symbolTable* parent;
  char* funcName;
};
struct ast_node
{
	char name[30];
	ast_node* peer;
	ast_node* child;
	int leaf;	//1 implies this is a leaf node. else it is an internal node
	//symbol table pointer
	tokenInfo tok; //points to the token info stored in the parse tree
	hashTable* ptr;	//Points each AST node to its symbol table entry
	char type[20];
	int offset;
};

typedef struct snode snode;
struct snode
{
  char symbol[30];
  struct snode* next;		//Points to next node
};				

typedef snode** sRules;		//An array of pointers to the head of each production
sRules S;
#endif