/*PUSHKARA RAVINDRA 2011A7PS150P
 *VIRAJ PRABHU	    2011A7PS044P
 * BATCH-1*/
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include "lexerDef.h"
#include "parser.h"
#include "symbolTable.h"
#include "astDef.h"
#define sruleCount 88
extern ast_node* a;
/*Takes input as parseTree(root), CFG(G) and Sematic Rules(S)
 * to return a abstract syntax tree for the given parse tree*/
ast_node* createAst( parseTree root, grammar G, sRules S)
{
	//Bottom up access the tree and make them rules.
	if (root == NULL)
	{
		//printf("Inside NULL\n");					//DEBUG
		return NULL;
	}
		createAst( root->child, G, S );

	tnode* Pcurrent;		//Current Parse Tree Node
	snode* Scurrent;		//Current Semantic Rule
	ast_node* Acurrent;		//Current AST Node

	if(get_t_index(root->name) != -1)	//If the node is a terminal
	{
		//printf("Inside the 'get_t_index' if statement\n");		//DEBUG
		Acurrent = (ast_node*)calloc(1, sizeof(ast_node));
		Acurrent->ptr = NULL;
		root->astnode = Acurrent;
		Acurrent->tok = root->tok;
		strcpy(Acurrent->name, root->name);
		//printf("Leaf: %s\n", Acurrent->name);	//DEBUG 
		Acurrent->leaf = 1;		//Mark the AST node as a leaf
	}
	else if(strcmp(root->name, "e")==0)
	{	
	  //Do nothing if it is a null production      
	}
	else					//The node is a non terminal
	{	
		//The following is done for a non-terminal
		int i; // holds the index of the CFG after the search is complete.
		for( i = 0; i < ruleCount; i++ )
		{
			if( strcmp(root->name, G[i]->symbol ) == 0)
				if( strcmp(root->child->name, G[i]->next->symbol) == 0)	//This determines the grammar production being used
					break;
		}
		
		Pcurrent = root->child;		
		Scurrent = S[i];		//Semantic rule corresponding to the grammar production just found
		
		Scurrent = Scurrent->next;
		if ( strcmp(Scurrent->symbol, "=") == 0)	//Do not make new node
		{	//printf("Inside the '=' if statement\n");		//DEBUG
			Scurrent = Scurrent->next;
			if( strcmp(Scurrent->symbol, "e") == 0)	//If NULL production
				root->astnode = NULL;
			else{						//Unit production
				while (Pcurrent != NULL)
				{
					if (strcmp(Pcurrent->name, Scurrent->symbol) == 0)	//Find the correct(non terminal) parse tree node that appears in the semantic rule
						break;
					Pcurrent = Pcurrent->peer;
				}
				//symbol-not-in-children error not handled
				root->astnode = Pcurrent->astnode;		
			}
		}
		else						//Make a new node
		{
			Scurrent = Scurrent->next; 		//Next one is the name of the AST node
			Acurrent = (ast_node*)calloc(1, sizeof(ast_node));
			Acurrent->ptr = NULL;
			strcpy(Acurrent->name, Scurrent->symbol);	//Store the 'name' of the node of the AST being created
// 			printf("Intermediate node: %s\n", Acurrent->name);	//DEBUG 
			Scurrent = Scurrent->next; 		//Next is the first child of the node. So child.
			while (Pcurrent != NULL)
			{
				if (strcmp(Pcurrent->name, Scurrent->symbol) == 0)
				  break;
				Pcurrent = Pcurrent->peer;
			}
			Acurrent->child = Pcurrent->astnode;	//First child of the ast node being created
			Pcurrent = Pcurrent->peer;
			Scurrent = Scurrent->next; 		// Henceforth, peers. So henceforth, in a loop
			
			ast_node* Acurrent2 = Acurrent->child;
			while (Scurrent != NULL)
			{
				while (Pcurrent != NULL)
				{
					if (strcmp(Pcurrent->name, Scurrent->symbol) == 0)
						break;
					Pcurrent = Pcurrent->peer;
				}	
				if(Pcurrent->astnode != NULL)
				{	
					//Acurrent2->peer = (ast_node*)calloc(1, sizeof(ast_node));
					Acurrent2->peer = Pcurrent->astnode;
					Acurrent2 = Acurrent2->peer;
				}
				
				Pcurrent = Pcurrent->peer;
				Scurrent = Scurrent->next;
			}	
			root->astnode = Acurrent;
		}	
	}
		
	
	createAst( root->peer, G, S);
	

	return root->astnode;
} 
/*Print the Semantic rules stored in the array of linked lists*/
// void printSrules()
// {
//   int i;
//   for(i = 0; i<sruleCount; i++)
//   {
//     while(S[i]->next != NULL)
//     {
//       printf("%s ", S[i]->symbol);
//       S[i] = S[i]->next;
//     }
//     printf("%s\n", S[i]->symbol);
//   }
// }

/*Print the Abstract Syntax Tree*/
void printAST(ast_node* a)
{
  printf("%s\n", a->name);
  if(a->child != NULL)
      printAST(a->child);
  if(a->peer != NULL)
      printAST(a->peer);    
}

/*Populate S and create the AST*/
void initAST(parseTree R)
{
      FILE *fp;
      fp = fopen("semantic_rules", "r+"); 
      S = (snode**)calloc(sruleCount, sizeof(snode*)); 		//An array of linked lists to store the semantic rules
      S[0] = (snode*)calloc(1, sizeof(snode));
      snode *temp;
      int i = 0;
      char c;
      while((c = fscanf(fp, "%s", (S[i]->symbol))) != EOF)
      {
	  temp = S[i];
	  S[i]->next = (snode*)calloc(1, sizeof(snode));
	  S[i] = S[i]->next;
	  fscanf(fp, "%s", (S[i]->symbol));
	  char tmp[30];
	  strcpy(tmp, S[i]->symbol); 
	  S[i]->next = (snode*)calloc(1, sizeof(snode));
	  S[i] = S[i]->next;
	  if(strcmp(tmp,"=") == 0)
	  {  
	      fscanf(fp, "%s", (S[i]->symbol));
	  }
	  else if(strcmp(tmp, "%") == 0)
	  {
	      while((fscanf(fp, "%s", (S[i]->symbol))) != EOF)
	      {
		if(fgetc(fp) == '\n') break;
		S[i]->next = (snode* )calloc(1, sizeof(snode));
		S[i] = S[i]->next;
	      }  
	  }
	  S[i] = temp;
	  i++;
	  if(i<88)
	    S[i] = (snode*)calloc(1, sizeof(snode));
	  else break;
      }
      fclose(fp);
      //printSrules();
      a = createAst(R, G, S);
      printf("Done creating the AST!\n");
     
}