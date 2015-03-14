/*typeExtractor.c*/
/*PUSHKARA RAVINDRA 2011A7PS150P
 *VIRAJ PRABHU	    2011A7PS044P
 * BATCH-1*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "lexerDef.h"
#include "symbolTable.h"
#include "astDef.h"

/*Global variables*/
typedef enum {false, true} bool;
int typeFlagCheck = 0;
char *curTypeTC;
char *curTypeTC2;
int divFlag = 0;
int errorFlag = 0;
int isNotSizeExpression = 0;
char *tempTypeCheck[5];				//Upto 5 temporary types assumed for now
int typeCheckCount = 0;
int curMatrixSize = 0;
int notFirstMatrixTypeCheck = 0;
int matrixSize = 0;

/*Prototype declarations*/
extern int hash(char *s);
extern int initTypeChecker(ast_node* a, symbolTable* S);
extern bool checkAST1type(ast_node* a, symbolTable* S, char* lvalue);
extern bool checkAST2type(ast_node* a, symbolTable* S);
extern char* evaluateRvalueAST1(ast_node* a, symbolTable* S, char* lvalue);
extern char* checkInOutVar(ast_node *a, symbolTable *S);				//Looks for the variable in a in the in and out vars of S
extern void evaluateMatrix(ast_node *a, symbolTable *S);

/*TODO: Handle Boolean Expressions
  TODO: Set global variables back to 0 after every run
  TODO: Looking in parent scope for a variable
  TODO: Handle function contents
  TODO: Deal with functions with no outvars and invars. Are empty function bodies allowed?
  TODO: Variables have to be initialized before use
  TODO: Handle matrix elements in an assignment*/
int initTypeChecker(ast_node* a, symbolTable* S)
{
      //printf("Type checking node %s\n", a->name);
  /*Traverses the Abstract Syntax Tree and for each kind of statement, matches the types of the lvalues and rvalues:*/
      if(strcmp(a->name, "AST1") == 0)
      {
	curTypeTC = NULL;
	checkAST1type(a->child, S, NULL);
      }
      else if(strcmp(a->name, "AST2") == 0)
      {
	curTypeTC = NULL;
	checkAST2type(a->child, S);
      }
      else if(strcmp(a->name, "FUNDEF") == 0)
      {
	 initTypeChecker(a->child, S);	 
      }
      else if((strcmp(a->name, "FUNID") == 0) && strcmp(a->peer->name, "PARAMLIST") == 0)
      {
	  S = a->ptr->value;
	  
      }
//       if(strcmp(a->name, "BEXP1") == 0)
//       {
// 	checkBEXP1(a->child, S);
//       }
//       else if(strcmp(a->name, "BEXP2") == 0)
//       {
// 	checkBEXP1(a->child, S);
//       }
//       else if(strcmp(a->name, "BEXPNOT") == 0)
//       {
// 	checkBEXPNOT(a->child, S);
//       }
      if(a->child != NULL)
      {
	initTypeChecker(a->child, S);
      }
      if(a->peer != NULL)
      {
	initTypeChecker(a->peer, S);
      }
      return errorFlag;
}
bool checkAST1type(ast_node* a, symbolTable* S, char* lvalue)
{

      //printf("Inside checkAST1type and a->name is %s!\n", a->name);
      /*Does type checking for Assignment Statement of type 1
      <assignmentStmt_type1> --> <leftHandSide_singleVar> ASSIGNOP <rightHandSide_type1> SEMICOLON*/
      if((strcmp(a->name, "ID") == 0) && typeFlagCheck == 0)		//On left side of assignment statement
      {
	//printf("In this ID %s with function name %s\n", a->tok.dat.str, S->funcName);
	int pos = hash(a->tok.dat.str)%50;
	symbolTable* temp = S;
	//printf("S->funcName:%s\n", S->funcName);
	hashTable* tempHash;
	while(temp != NULL)
	{   
// 	    printf("temp->H[pos].pt:%d", temp->H[pos].pt);
	    if(temp->H[pos].pt != NULL && (strcmp(temp->H[pos].name, a->tok.dat.str) == 0))
	    {
		lvalue = temp->H[pos].type;
		tempHash = &(temp->H[pos]);
		typeFlagCheck = 1;
		//break;
	    }

	temp = temp->parent;
	}
	if(lvalue == NULL)
	{
	    char* t;
	    if((t = checkInOutVar(a, S)) != NULL);
	    {
		lvalue = checkInOutVar(a, S);
		if(lvalue != NULL) typeFlagCheck = 1;
	    }
	}
// 	printf("lvalue:%s and tempHash->size:%d\n", lvalue, tempHash->size);
	if(lvalue != NULL && (strcmp(lvalue, "MATRIX") == 0))
	{
		evaluateMatrix(a, S);
		//printf("Matrix has been evaluated\n");
		tempHash->size = matrixSize;
		matrixSize = 0;
// 		printf("LOOK Size of %s is %d\n", a->tok.dat.str, tempHash->size);
		//printf("Done with matrix initialization here \n");
	}
	if(typeFlagCheck == 0)
	{	
		printf("############################ERROR:Variable %s is not declared in line %d############################\n", a->tok.dat.str, a->tok.line);
		errorFlag = 1;
		return;
	}
	
      }
      else if((strcmp(a->name, "AEXP") == 0) && typeFlagCheck == 1)		//On right side of assignment statement
      {
	/*Arithmetic expression statement*/
	if(lvalue == NULL)
	{
	    printf("#########################ERROR:No type assigned to lvalue in line %d###############################\n", a->tok.line);
	    errorFlag = 1;
	    return;
	}
	else
	{

	    char* rvalue = evaluateRvalueAST1(a, S, lvalue);
	    typeFlagCheck = 0;
	    if(errorFlag == 1) return false;
	    if(strcmp(rvalue, lvalue) == 0)
	    {
		return true;
	    }
	    if(strcmp(lvalue, "MATRIX") == 0)
	    {
	      /*TODO: Handle updating of size of LHS */
// 		tempHash->size = curMatrixSize;
// 		printf("Updated size of %s is %d", tempHash->name, curMatrixSize); 
		return true;
	    }
	    else if(strcmp(lvalue, "MATRIX") != 0)
	    {
	      printf("#########################ERROR:Lvalue and Rvalue types dont match in line %d!###############################\n", a->tok.line);
	      errorFlag = 1;
	      return false;
	    }
	      
	}
      }
      else if((strcmp(a->name, "ID") == 0) && typeFlagCheck == 1)
      {
	/*This is a size expression*/
	int pos = hash(a->tok.dat.str)%50;
	typeFlagCheck = 0;
	symbolTable* temp = S;
	char *typeFound = NULL;
	while(temp != NULL)
	{
	    if(strcmp(temp->H[pos].name, a->tok.dat.str) == 0)
	    {
		typeFound = temp->H[pos].type;

		break;
	    }

	temp = temp->parent;
	}
	if(typeFound == NULL)
	{
	    char* t;
	    if((t = checkInOutVar(a, S)) != NULL);
	    {
		typeFound = checkInOutVar(a, S);
	    }
	}
	if(typeFound == NULL)
	{
	  printf("#######################################Type Error in Size Expression!######################################\n");
	  errorFlag = 1;
	  return false;
	}
	
	else return true;
      }
      else if((strcmp(a->name, "FUNID") == 0) && typeFlagCheck == 1)
      {
	/*Function call statement*/
	/*<funCallStmt> % FUNCALL FUNID <inputParameterList>*/
	typeFlagCheck = 0;
	int pos = hash(a->tok.dat.str)%50;
	symbolTable *func = S->H[pos].value;
	/*TODO: Check for recursive invocation!*/
	if((func->outVar[0].type, curTypeTC) != 0)
	{
	  printf("#######################################Return type and receiving variable type do not match in function %s!######################################\n", a->name);
	  errorFlag = 1;
	  return false;
	}	
	return true;
      }
      if(a->child != NULL)
      {
	checkAST1type(a->child, S, lvalue);
      }
      if(a->peer != NULL)
      {
	checkAST1type(a->peer, S, lvalue);
      }
}
char* evaluateRvalueAST1(ast_node* a, symbolTable *S, char* lvalue)
{
      //printf("Inside RvalueAST1 with curtype %s and a->name:%s!\n", curTypeTC, a->name);
      /*Evaluates the type of the rvalue and returns it.*/
      if(a->child != NULL)
      {
	evaluateRvalueAST1(a->child, S, lvalue);
      }
      if(a->peer != NULL)
      {
	evaluateRvalueAST1(a->peer, S, lvalue);
      }
      if(strcmp(a->name, "ID") == 0)
      {
	//printf("Inside the Rvalue ID place with lvalue as %s for ID name as %s\n", lvalue, a->tok.dat.str);
	int pos = hash(a->tok.dat.str)%50;
	symbolTable* temp = S;
	char *typeFound = NULL;
	hashTable tempHash;
	while(temp != NULL)
	{
	    if(strcmp(temp->H[pos].name, a->tok.dat.str) == 0)
	    {
		typeFound = temp->H[pos].type;
		tempHash = temp->H[pos];
		break;
	    }
	temp = temp->parent;
	}
	if(typeFound == NULL)
	{
	    char* t;
	    if((t = checkInOutVar(a, S)) != NULL);
	    {
		typeFound = checkInOutVar(a, S);
	    }
	}
	//printf("typeFound is %s\n", typeFound);
	//while(temp != NULL)
// 	{
// 	    if(strcmp(temp->H[pos].name, a->tok.dat.str) == 0)
// 	    {
// 		typeFound = temp->H[pos].type;
// 		tempHash = temp->H[pos];
// 		printf("Size of %s is %d\n", a->tok.dat.str, tempHash.size);
// 		temp = temp->parent;
// 		break;
// 	    }
// 
// 	temp = temp->parent;
// 	}
	if(typeFound != NULL)
	{
	  
	  if (curTypeTC == NULL) 
	  {
	    curTypeTC = typeFound;
	    //printf("Assigning a current type %s!\n", curTypeTC);
	  }
	  else if(((strcmp(typeFound,curTypeTC) != 0) || ((divFlag == 1 && (strcmp(typeFound, "INT") == 0) != 1))) && (strcmp("MATRIX" ,lvalue) != 0))
	  {
	      divFlag = 0;
	      printf("###################Error: Type of %s does not match the declared type, or that of the rest of the expression##################\n", a->tok.dat.str);
	      //TODO: Handle this error better
	      errorFlag = 1;
	      return;
	  } 
	  else if(divFlag == 1)
	      divFlag = 0;		//Setting it back to 0
	  if(strcmp(lvalue, "MATRIX") == 0 && notFirstMatrixTypeCheck == 0)
	  {
	      
	      curMatrixSize = tempHash.size;
	      //printf("Entered here\n with curMatrixSize:%d", curMatrixSize);
	      notFirstMatrixTypeCheck = 1;
	  }
	  else if(strcmp(lvalue, "MATRIX") == 0 && notFirstMatrixTypeCheck == 1)
	  {
	    //printf("Entered here\n with curMatrixSize:%d", curMatrixSize);
		  if(curMatrixSize != tempHash.size)
		  {
		      printf("####################ERROR: Matrices of unequal sizes cannot be added in line %d#################\n", a->tok.line);
		      errorFlag = 1;
		      return;
		  }
	  }
	  }
	  else
	  {
		printf("#######################################Variable %s not declared in line %d!######################################\n", a->tok.dat.str, a->tok.line);
		errorFlag = 1;
		return;
	  }
      }
      else if(strcmp(a->name, "NUM") == 0)
      {
	  if (curTypeTC == NULL) curTypeTC = "INT";
	  else if(strcmp(curTypeTC, "INT") != 0)
	  {
	    printf("###################Error: Type of %d does not match declared type in line %d###################\n", a->tok.dat.num, a->tok.line);
	    errorFlag = 1;
	    return;
	  }
      }
      else if(strcmp(a->name, "STR") == 0)
      {
// printf("String= %s\n", a->tok.dat.str);
	  if(curTypeTC == NULL) curTypeTC = "STRING";
	  else if(strcmp(curTypeTC, "STRING") != 0)
	  {
	    printf("###################Error: Type of %s does not match declared type in line %d###################\n", a->name, a->tok.line);
	    errorFlag = 1;
	    return;
	  }
      }
      else if(strcmp(a->name, "RNUM") == 0)
      {
	  if (curTypeTC == NULL) curTypeTC = "REAL";
	  else if(strcmp(curTypeTC, "REAL") != 0)
	  {
	    printf("###################Error: Type of %s does not match declared type##################\n", a->name);
	    errorFlag = 1;
	    return;
	  }
      }
      else if(strcmp(a->name, "STRING") == 0)
      {
	  //printf("Inside the STRING elseif!\n");
	  if (curTypeTC == NULL) curTypeTC = "STRING";
	  else if(strcmp(curTypeTC, "STRING") != 0)
	  {
	    printf("###################Error: Type of %s does not match declared type in line %d###############3###\n", a->name, a->tok.line);
	    errorFlag = 1;
	    return;
	  }
      }
      else if((strcmp(a->name, "MINUS") == 0 || strcmp(a->name, "PLUS") == 0 || strcmp(a->name, "MUL") == 0) || (strcmp(a->name, "DIV") == 0))
      {
	if(curTypeTC == NULL)
	{
	  printf("#########################ERROR: Current type is NULL in line %d!########################\n", a->tok.line);
	  errorFlag = 1;
	  return;
	}
	if(((strcmp(a->name, "MINUS") == 0) || strcmp(a->name, "PLUS") == 0) && (strcmp(curTypeTC, "MATRIX") == 0))
	{
	    /*Match sizes*/
	}
	else if(((strcmp(a->name, "MUL") == 0) || (strcmp(a->name, "DIV") == 0)) && (strcmp(curTypeTC, "MATRIX") == 0))
	{
	  printf("#########################ERROR: %s not defined for MATRIX in line %d!####################\n", a->name, a->tok.line);
	  errorFlag = 1;
	  return;
	}
	else if((strcmp(curTypeTC, "STRING") == 0) && strcmp(a->name, "PLUS") != 0)
	{
	  printf("#########################ERROR: %s not defined for strings in line %d!####################\n", a->name, a->tok.line);
	  errorFlag = 1;
	  return;
	}
	else if((strcmp(a->name, "DIV") == 0) && (strcmp(curTypeTC, "INT") == 0))
	{	/*INT/INT = REAL*/
	  {
	    curTypeTC = "REAL";
	    divFlag = 1;
	  }
	}
      }


      return curTypeTC;
}

bool checkAST2type(ast_node* a, symbolTable* S)
{
  /*<assignmentStmt_type2> % AST2 <leftHandSide_listVar> <rightHandSide_type2>
   *<rightHandSide_type2> = <sizeExpression>
    <rightHandSide_type2> = <funCallStmt> */
      //printf("Inside Check AST2:ID->name :%s and Current type is %s and typeCheckCount:%d and isNotSizeExpression:%d\n", a->name, curTypeTC2, typeCheckCount, isNotSizeExpression);
      if(a->child != NULL)
      {
	checkAST2type(a->child, S);
      }
      if(a->peer != NULL)
      {
	checkAST2type(a->peer, S);
      }
      if((strcmp(a->name, "ID") == 0) && isNotSizeExpression == 0)
      {
	  int pos = hash(a->tok.dat.str)%50;
	  if((strcmp(S->H[pos].type, "MATRIX") == 0))
	  {
		  if(isNotSizeExpression == 1 || typeCheckCount != 2 || strcmp(curTypeTC2, "INT") != 0)
		  {
		    //printf("isNotSizeExpression:%d and typeCheckCount:%d\n", isNotSizeExpression, typeCheckCount);
		    printf("#########################ERROR: Error in Size Operator for matrix in line %d!####################\n", a->tok.line);
		    errorFlag = 1;
		    return false;
		  }
		  else return true;
	  }
	  if(curTypeTC2 == NULL) 
	  {
	    curTypeTC2 = S->H[pos].type; 
	  }
	  else if(strcmp(curTypeTC2, S->H[pos].type) != 0)		//Means this can only be a function call, not a size expression
	    isNotSizeExpression = 1;
	  
	  tempTypeCheck[typeCheckCount] = S->H[pos].type;
	  typeCheckCount++;
      }
      else if(strcmp(a->name, "ID") == 0 && isNotSizeExpression == 1)
      {
	  int pos = hash(a->tok.dat.str)%50;
	  strcpy(tempTypeCheck[typeCheckCount], S->H[pos].type);
	  typeCheckCount++;
      }
      else if((strcmp(a->name, "FUNID") == 0))
      {	
	    /*Function call statement*/
	    /*<funCallStmt> % FUNCALL FUNID <inputParameterList>*/
	    int pos = hash(a->tok.dat.str)%50;
	    int j;
	    /*TODO: Check for recursive invocation!*/
	    for(j = 0; j < typeCheckCount; j++)
	    {       symbolTable *func = S->H[pos].value;
		    if(strcmp (func->outVar[j].type, tempTypeCheck[j]) != 0)
		    {
		      printf("#########################ERROR: Type of %s does not match in the function call in line %d!####################\n", a->name, a->tok.line);
		      errorFlag = 1;
		      return false;
		    }
		    else
		    {
		      typeCheckCount = 0;
		      return true;
		    }
	    }
      }
}
char* checkInOutVar(ast_node* a, symbolTable* S)
{
    /*Checks if the variable occurs in the In Or the Out Vars of the Symbol Table S as well as its parents*/
    symbolTable* temp = S;
    int t = 0;
    //printf("Checking In and Out Vars!\n");
    while(temp != NULL)
    {
	while(strcmp(S->inVar[t].name, "$") != 0)
	{
	  //printf("Inside here in var\n");
	    if(strcmp(S->inVar[t].name, a->tok.dat.str) == 0)
	    {
		return S->inVar[t].type;
	    }
	    t++;
	}
	t = 0;
	while(strcmp(S->outVar[t].name, "$") != 0)
	{
	  //printf("Inside here out var\n");
	    if(strcmp(S->outVar[t].name, a->tok.dat.str) == 0)
	    {
		return S->outVar[t].type;
	    }
	    t++;
	}
	temp = temp->parent;
    }
    return NULL;
}

void evaluateMatrix(ast_node *a, symbolTable *S)
{
//       printf("matrixSize:%d\n", matrixSize);
    /*Sets up a matrix if an initialization statement*/
      if(strcmp(a->name, "NUM") == 0)
      {
	  matrixSize++;
      }
      if(a->child != NULL)
      {
	evaluateMatrix(a->child, S);
      }
      if(a->peer != NULL)
      {
	evaluateMatrix(a->peer, S);
      }
}