/*PUSHKARA RAVINDRA 2011A7PS150P
 *VIRAJ PRABHU	    2011A7PS044P
 * BATCH-1*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "lexerDef.h"
#include "symbolTable.h"
#include "astDef.h"

/*Global Declarations*/
char curType[20];
char *curTypeFunc;
char *tempTypeFunc[5];	//Upto 5 temporary names and types assumed for now
char *tempNameFunc[5];
int i = 0;
int STerror = 0;
int symTableCount = 1;
int flagFunc = 0; 			//To distinguish between whether the two parameter lists in the function definition
int outVarCount = 0;
/*Prototype declarations*/
extern int hash(char *s);
extern symbolTable* createSymbolTable(ast_node *a, symbolTable* ST);
extern void printSymbolTable( ast_node* a, symbolTable* ST);
extern ast_node* populateInVars(ast_node *a, symbolTable* newS, char* curType);
extern void createSymbolTableEntryVar(ast_node *a, symbolTable* ST, char *curType);
extern void createSymbolTableEntryFunc(ast_node *a, symbolTable* ST);
//extern symbolTable* ST;
/*TODO: Resize symbol table
  TODO: Handle hashing collisions
  TODO: Pointers from AST to symbol table entries*/
symbolTable* createSymbolTable(ast_node* a, symbolTable* ST)
{
  //printf("On node %s\n", a->name);
  /*Make a symbol table entry only for all variable and function declarations (Declaration statement)
   * For any other statement, look up for the variable in the symbol table. If not found, there is an error.
   * For Assignment, If, IO and Function Call, do a look up*/
      if(strcmp(a->name, "DECLST") == 0)	//This is a declaration statement, so create symbol table entries(Only variable decaration)
      {
	if(a->child != NULL)
	createSymbolTableEntryVar(a->child, ST, NULL);	//Create symbol table entries for all the variables in this declaration statement
	//printf("Succesful insertion of variable into symbol table!\n");
      }
      else if(strcmp(a->name, "FUNDEF") == 0)
      {
	/*For a function declaration, make a symbol table entry and add to inVar and outVar*/
	createSymbolTableEntryFunc(a->child, ST);	//Create symbol table entry for function definition
      }
      else if(a->child != NULL)
      {
	createSymbolTable(a->child, ST);
      }
      if(a->peer != NULL)
	  createSymbolTable(a->peer, ST);  
      return ST;
}

void createSymbolTableEntryVar(ast_node *a, symbolTable* ST, char *curType)
{
      /*This function traverses the declaration statement subtree and creates symbol table entries for it*/
      /*<declarationStmt> --> <type> <var_list> SEMICOLON*/ 
      /*Handle hash collisions*/
      //printf("On node %s inside createSymbolTableEntryVar\n", a->name);
      if(a!= NULL && (strcmp(a->name, "INT") == 0) || (strcmp(a->name, "REAL") == 0) || (strcmp(a->name, "STRING") == 0) || (strcmp(a->name, "MATRIX") == 0))
	  curType = a->name;		//The type of the variable(s)
      else if(a != NULL && strcmp(a->name, "ID") == 0)
      {
	//printf("Inside this ID\n");
	int pos = hash(a->tok.dat.str)%50;
	if(curType == NULL)
	  printf("##################ERROR:No type assigned yet to variable %s in line %d !##############################\n", a->tok.dat.str, a->tok.line);
	if(ST->H[pos].pt != NULL && (strcmp(ST->H[pos].name, a->tok.dat.str) == 0))
	{
	  printf("#######################ERROR: A variable with the name %s is already defined in this scope in line %d###################\n", a->tok.dat.str, a->tok.line);
	  STerror = 1;
	  
	}
	else
	{
	  //printf("Inside this else if inside this ID\n");
	  strcpy(ST->H[pos].type, curType);
	  strcpy(ST->H[pos].name, a->tok.dat.str);
	  a->ptr = &(ST->H[pos]);
	  ST->H[pos].pt = a;
	  //printf("a->ptr->type = %s\n", a->ptr->type);
	}
      }
      if(a->child != NULL)
      {
	createSymbolTableEntryVar(a->child, ST, curType);
      }
      if(a->peer != NULL)
      {
	createSymbolTableEntryVar(a->peer, ST, curType);
      }    
}

void createSymbolTableEntryFunc(ast_node *a, symbolTable* ST)
{
      /* 1. Adds all output variables to outVar
      * 2. Adds entry for FUNID 
      * 3. Adds all input variables to inVar	
      <functionDef> --> FUNCTION SQO <parameter_list> SQC ASSIGNOP FUNID SQO <parameter_list> SQC <stmtsAndFunctionDefs> END SEMICOLON	*/
      
      //First, to make entries for output variables
      //printf("Current node:%s\n", a->name);
      if(a!= NULL && ((strcmp(a->name, "INT") == 0) || (strcmp(a->name, "REAL") == 0) || (strcmp(a->name, "STRING") == 0) || (strcmp(a->name, "MATRIX") == 0)) && flagFunc == 0)
      {
	  curTypeFunc = a->name;
      }
      else if(a != NULL && strcmp(a->name, "ID") == 0 && flagFunc == 0)
      {
	  int pos = hash(a->tok.dat.str)%50;
	  if(curTypeFunc == NULL)
	      printf("############################ERROR:No type assigned yet to var %s in line %d!####################################\n", a->name, a->tok.line);
	  else
	  {
	    tempTypeFunc[outVarCount] = curTypeFunc;
	    tempNameFunc[outVarCount] = a->tok.dat.str;
	    outVarCount++;
	  }
      }
      else if(a != NULL && (strcmp(a->name, "FUNCALL") == 0))
      {
	  //printf("Entered this thing");
	  int pos = hash(a->peer->tok.dat.str)%50;
	  symbolTable *temp = ST->H[pos].value;
	  while(temp != NULL)
	  { 
		    if(ST->H[pos].pt != NULL && strcmp(ST->H[pos].name, a->peer->tok.dat.str) == 0 && (strcmp(a->peer->name, "IPLST") == 0))
		    {
			printf("#############################ERROR:Recursive declaration is not allowed: Function %s in line %d##########################\n", a->name, a->tok.line);
			STerror = 1; 
			return;
		    }
		    temp = temp->parent;
	  }
	  //printf("a->tok.dat.str:%s and ST->H[pos].pt->name:%s\n", a->tok.dat.str, ST->H[pos].pt->name);
	  if(ST->H[pos].pt == NULL)
	  {	//This function has not been defined yet
		printf("#############################ERROR:There is no definition for this function in the present scope: Function %s in line %d##########################\n", a->name, a->tok.line);
		STerror = 1; 
		return;
	  }	
      }
      else if(a != NULL && (strcmp(a->name, "FUNID") == 0))
      {
	  int pos = hash(a->tok.dat.str)%50;
	  if(curTypeFunc == NULL)
	      printf("############ERROR:No type assigned yet to function %s in line %d!#################\n", a->name, a->tok.line); 
	  else if(ST->H[pos].pt != NULL && strcmp(ST->H[pos].name, a->tok.dat.str) == 0 && (strcmp(a->peer->name, "IPLST") != 0))
	  {
		printf("#######################ERROR: No function overloading:A function with the name %s is already defined in this scope in line %d###################\n", a->tok.dat.str, a->tok.line);
		STerror = 1;
	  }
	  else
	  {
		strcpy(ST->H[pos].type, "F");		//What type?!
		strcpy(ST->H[pos].name, a->tok.dat.str);
		ST->H[pos].pt = a;
		symbolTable* newST = (symbolTable*)calloc(1, sizeof(symbolTable));
		int p;
		ST->H[pos].value = newST;
		for(p = 0; p<50; p++)
		{
		  newST->H[p].pt = NULL;
		}
		a->ptr = &(ST->H[pos]);
		newST->parent = ST;
		newST->funcName = a->tok.dat.str;
		p = 0;
		for(p = 0; p<5; p++)
		{
		    strcpy(newST->inVar[p].name, "$");
		    strcpy(newST->outVar[p].name, "$");;
		}
		//printf("New symbol table created and assigned for function %s\n", newST->funcName);
		/*Until end of function prototype, populate the inVar and outVar, then move to symbol table of the function*/
		//First, out vars
		int j;
		for(j = 0; j < outVarCount; j++)
		{  
		  //printf("OUTVAR DEBUG %s\n", tempNameFunc[j]);
		  strcpy(newST->outVar[j].type, tempTypeFunc[j]); 
		  strcpy(newST->outVar[j].name, tempNameFunc[j]);     
		}
		for(j = 0; j < outVarCount; j++)
		{  
		  //printf("OUTVAR DEBUG %s\n", tempNameFunc[j]);
		  //printf("OUTVAR:%s %s \n", newST->outVar[j].type, newST->outVar[j].name);    
		}
		outVarCount = 0;
		//printf("Outvars populated\n");
		i = 0;	//Setting it back to 0
		ast_node* temp = populateInVars(a->peer, newST, NULL);
		a = temp;
		i = 0; 	//Setting it to 0 again as it is used in populateInVars
		//printf("Invars populated\n");
		flagFunc = 1;		//Now the parameter list will refer to the second one after the assignment operator
		if(a != NULL && (strcmp(a->name, "SAFD") == 0))
		{
		    /*The body of the function. */
		    symTableCount++;
		    //printf("Current sym table count: %d\n", symTableCount);
		    createSymbolTable(a, newST);
		}
	  }
      }
      if(a->child != NULL)
      {
	  createSymbolTableEntryFunc(a->child, ST);
      }
      if(a->peer != NULL)
      {
	  createSymbolTableEntryFunc(a->peer, ST);
      }
}

ast_node* populateInVars(ast_node *a, symbolTable* newS, char* curType)
{      
      /*Populates the invars for a function's symbol table*/
      if(a != NULL && (strcmp(a->name, "SAFD") == 0))
      {
	return a;
      }
      else if(a!= NULL && (strcmp(a->name, "INT") == 0) || (strcmp(a->name, "REAL") == 0) || (strcmp(a->name, "STRING") == 0) || (strcmp(a->name, "MATRIX") == 0))
      {
	curType = a->name;
      }
      else if(a != NULL && strcmp(a->name, "ID") == 0)
      {
	
	if(curType == NULL)
	{
	  printf("ERROR:No type assigned yet in line %d!\n", a->tok.line);
	  return;
	}
	if(i < 5)
	{
	  strcpy(newS->inVar[i].type, curType);
	  strcpy(newS->inVar[i].name, a->tok.dat.str);
	  i++;
	}
	else
	   printf("ERROR:Invar is  full in line %d!\n", a->tok.line);
	  
      }
      if(a->child != NULL)
      {
	populateInVars(a->child, newS, curType);
      }
      if(a->peer != NULL)
      {
	populateInVars(a->peer, newS, curType);
      }
}

void printSymbolTable(ast_node* a, symbolTable* ST)
{
      /*Prints the input symbol table, as  Identifier name       scope                     type           offset ***/
	hashTable* temp;
	if((temp = a->ptr) != NULL)
	{
	  int i;
	  if(strcmp(temp->type, "F") == 0)
	  {
	      symbolTable* temp1 = temp->value;
	      for(i = 0; i<5; i++)
	      {
		  if(strcmp(temp1->inVar[i].name, "$") != 0)
		  {
		    printf("%s\t%s\t%s\n", temp1->inVar[i].name, temp1->funcName, temp1->inVar[i].type);
		  }
	      }
	      for(i = 0; i<5; i++)
	      {
		  if(strcmp(temp1->outVar[i].name, "$") != 0)
		  {
		    printf("%s\t%s\t%s\n", temp1->outVar[i].name, temp1->funcName, temp1->outVar[i].type);
		  }
	      }
	  }
      
	  else
	  {
	      printf("%s\t%s\t%s\n", temp->name, ST->funcName, temp->type);
	  }
	}
	if(a->child != NULL)
	{
	    printSymbolTable(a->child, ST);
	}
	if(a->peer != NULL)
	{
	    printSymbolTable(a->peer, ST);
	}

}

int hash(char *s)
{
      /*Generates a hash value for an input string*/
      int h = 0;
      int i;
      for(i = 0; i < sizeof(s)/sizeof(s[0]); i++)
      {
	h = (31*h) + s[i];
      }
      return h;
}
