/*PUSHKARA RAVINDRA 2011A7PS150P
 *VIRAJ PRABHU	    2011A7PS044P
 * BATCH-1*/
/*parser.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef LEXERDEF_H
#include "lexerDef.h"
#ifndef LEXER_H
#include "lexer.h"
#include "parser.h"
//#include "parserDef.h"

/*GLOBAL DEFINITONS*/
NT ffset[ntCount];
char terminals[tCount][30];
Stack s = NULL;
parseTree a;	
Table T[ntCount][tCount]; 
/*END OF GLOBAL DEFINITIONS*/

void push(tnode *t)
{
  Stack temp = (Stack)calloc(1, sizeof(_stack));
  temp->tno = t;
  temp->next = s;
  s = temp; 
}

void pop()
{
  Stack temp = s;
  if(s == NULL) return;
  s = s->next;
  free(temp);
}
/*END OF STACK HELPER FUNCTIONS*/

/*PARSE TABLE CREATION HELPER METHODS*/
int get_nt_Index(char* str)
{
  int i;
  for(i = 0; i<ntCount; i++)
  {
    if(strcmp(str, ffset[i].name) == 0)
      return i;
  }  
}
int get_t_index(char* str)
{	int i;
  for(i = 0; i<tCount; i++)
  {
    if(strcmp(str, terminals[i]) == 0)
      return i;
  }
  return -1;
}
/*END OF HELPER METHODS FOR PARSE TABLE CREATION*/

/*PARSING FUNCTION*/
parseTree parseInputSourceCode(char *testcaseFile, Table T[][tCount])
{
  FILE *fp = initLexer(testcaseFile);
  tokenInfo A;
  int tIndex;					//Index of terminal
  int ntIndex;					//Index of non terminal
  int Rno;					//Rule Number
  
  parseTree root = NULL;
  
  tnode *bottom = (tnode*)calloc(1, sizeof(tnode));
  strcpy(bottom->name, "$");
  root = (parseTree)calloc(1, sizeof(tnode));
  strcpy(root->name, "<mainFunction>");
  push(bottom);					//Pushing bottom of stack marker onto stack
  push(root);					//Pushing start state onto stack 
  FILE *fp1 = fopen("token_lexemes.txt", "w+");
  
      //printStack();
  do
  {    
    tnode *temp1 = NULL;
    tnode* temp3 = NULL;
    tnode* temp2 = NULL;
    
    A = getNextToken(fp);			//To get the next token from input
    if(strcmp(A.token, "!") == 0) break;
    //fprintf(fp1, "%s %s\n", A.token, A.dat.str);
    tIndex = get_t_index(A.token);		//To get column index of token in parsing table
    
    while(get_t_index(s->tno->name) == -1)	//Enter when top of stack is not a terminal
    {
      temp1 = NULL;
      temp2 = NULL;
      temp3 = NULL;
      ntIndex = get_nt_Index(s->tno->name);	//To get row index of non terminal on top of stack
      Rno = T[ntIndex][tIndex];			//Find rule number for current top of stack and input symbol combination
      
      if(Rno == -1)				//No rule for this combination, hence error
      {
	printf("Compilation error:Unexpected symbol.\n");
	return 0;
      }
      if(strcmp(G[Rno]->next->symbol,"e")==0)	//An epsilon rule exists, hence add it to parse table and pop the nonterminal
      {
	temp1 = (tnode*)calloc(1, sizeof(tnode));
	strcpy(temp1->name, "e");
	s->tno->child = temp1;
	pop();
	continue;
      }
      
      temp1 = s->tno;				//Save the parent from the top of stack before popping it off
      pop();					//Only remaining possibility is that a valid non-epsilon rule exists, hence pop off current top of stack nonterminal
      node* temp = G[Rno]->last;		//Obtain the rule in reverse order
      
      while(temp != NULL)
      {	
	if(temp->previous!=NULL)
	{					
	      temp3 = (tnode*)calloc(1, sizeof(tnode));
	      strcpy(temp3->name, temp->symbol);
	      temp3->peer = temp2;		//Create a parse tree node corresponding to the current production being applied
	      temp2 = temp3;
// 	      if(get_t_index(temp2->name) != -1)
// 		temp2->tok = A;
	      push(temp2);			//Push the node on (in reverse so that leftmost symbol appears on top of stack)
	}
	temp=temp->previous;
      }
      temp1->child = temp2;
      
      //printStack();
    }
    
    
    if((strcmp(A.token, s->tno->name)) != 0)		//When token and top of stack do not match after applying rule
    {
      if((strcmp(s->tno->name, "$")) == 0)		//When stack is consumed but input is remaining, error
      {
	printf("ERROR : Expected EOF at line no. %d\n", A.line);
	flag = 1;
	return root;
      }
      flag = 1;
      printf("Error. Expected %s at line no. %d\n", s->tno->name, A.line);
      return root;      
    }
    s->tno->tok = A;
    //printf("%s\n", s->tno->name);
    pop();					
  }while(strcmp(A.token, "$") != 0);		//Loop until input is consumed, as $ is returned as token for EOF by lexer
  
  if(strcmp(A.token, "!") == 0)
  {
   do{
     A = getNextToken(fp);
   }while(strcmp(A.token, "$") != 0);
   strcpy(root->name,"!");
   return root;
  }
  if(s != NULL)					//Input is consumed by stack is not empty, hence error
  {
    //print error
    //mark flag
    printf("ERROR:Expected EOF.\n");
    return root;
  }
  if(flag == 0)
    flag = 5;
    printf(">>>>>>>>>>>>>>>>>>>>PARSING WAS SUCCESSFUL !<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
  
  fclose(fp1);
  endLexer(fp);					//Close lexer
  return root;					//Return parse tree obtained
}
/*END OF PARSING FUNCTION*/

/*PARSE TABLE CREATION*/
void createParseTable(grammar G, Table T[][tCount])
{
    //This function takes as input the grammar G, uses FIRST and FOLLOW information to populate  the table T appropriately. 

    //For storing the first and follow in a data structure
    FILE *f = fopen("terminals.txt", "r+");
    int i, j;
    for(i = 0; i<tCount; i++)
    {
      fscanf(f, "%s", terminals[i]);
    }
    fclose(f);
    FILE *fp = fopen("nonterminals.txt", "r+");
    for(i = 0; i<ntCount; i++)
    {
      ffset[i].has_e = 0;
      fscanf(fp, "%s", ffset[i].name);
//       if(fgetc(fp) != '\n')
//       {
// 	printf("Error in file.\n");
// 	return;
//       }
    
    ffset[i].first = NULL;
    ffset[i].follow = NULL;
    node* temp;
    char s[30];
    
    while(fscanf(fp, "%s", s) != EOF)
    {
      if(strcmp(s, "e") == 0)
      {
	ffset[i].has_e = 1;
	if(fgetc(fp)=='\n') break;
	continue;
      }
      
      temp = (node*)calloc(1, sizeof(node));
      strcpy(temp->symbol, s);
      temp->next = ffset[i].first;
      ffset[i].first = temp;
      if(fgetc(fp)=='\n') break;
    }
    while(fscanf(fp, "%s", s) != EOF)
    {
      temp = (node*)calloc(1, sizeof(node));
      strcpy(temp->symbol, s);
      temp->next = ffset[i].follow;
      ffset[i].follow = temp;
      if(fgetc(fp)=='\n') break;
    }
  }    
  fclose(fp);
  
  //To initialize all entries with -1
  for(i = 0; i<ntCount; i++)
  {
    for(j = 0; j<tCount; j++)
    {
      T[i][j] = -1;
    }
  }
  
//   printf("Printing first and follow:\n");
//   for(i = 0; i<ntCount; i++)
//   {	printf("%s\nFirst:\n", ffset[i].name);
// 	node* temp = ffset[i].first;
// 	while(temp != NULL)
// 	{
// 	  printf("%s ", temp->symbol);
// 	  temp = temp->next;
// 	}
// 	printf("\nFollow:\n");
// 	temp = ffset[i].follow;
// 	while(temp != NULL)
// 	{
// 	  printf("%s ", temp->symbol);
// 	  temp = temp->next;
// 	}
// 	printf("\n");
//   }
  
  //For each production A->a, include it in columns of all terminals in FIRST(a)
  for(i = 0; i<ruleCount; i++)
  { //For Each production rule in Grammar
   // char *str = G[i]->next->symbol;	//To store 
    int lhsIndex = get_nt_Index(G[i]->symbol);	//!!
    char* sym = G[i]->next->symbol;
    node* temp;
    int tIndex;
    if((tIndex = get_t_index(sym)) != -1)
    {
      T[lhsIndex][tIndex] = i;
      continue;
    }
    if(strcmp(sym, "e") == 0)
    {
      temp = ffset[lhsIndex].follow;
      while(temp != NULL)
      { 
	int tIndex = get_t_index(temp->symbol);
        T[lhsIndex][tIndex] = i;
	temp = temp->next;
      }
      continue;
    }
    
    int rhsIndex = get_nt_Index(sym);	
    temp = ffset[rhsIndex].first;
    
    while(temp != NULL)
    { 
      //For every symbol in First of this non terminal
      tIndex = get_t_index(temp->symbol);	
      T[lhsIndex][tIndex] = i;
      temp = temp->next;
    }
    
    if(ffset[lhsIndex].has_e == 1)
    {	//Add follow
      temp = ffset[lhsIndex].follow;
      while(temp != NULL)
      { 
	tIndex = get_t_index(temp->symbol);
        T[lhsIndex][tIndex] = i;
	temp = temp->next;
      }
    }
  }
      //To print the table:Uncomment to print the parsing table generated
//   printf("Parsing table:\n");
//   for(i = 0; i<ntCount; i++)
//   {
//     for(j = 0; j<tCount; j++)
//     {
//       printf("%d\t", T[i][j]);
//     }
//     printf("\n");
//   }
}
/*END OF PARSE TABLE CREATION*/

/*PRINT THE PARSE TREE*/
void printTreeRecurse(parseTree PT, FILE *fp)
{
  if(PT == NULL) return;
  if(s == NULL)
    fprintf(fp, "---- ---- ---- ---- ROOT NO %s\n", PT->name);
  else
  {
    if(get_t_index(PT->name) == -1)
    {
      fprintf(fp, "---- ---- ---- ---- %s NO %s\n", s->tno->name, PT->name);
    }
    else
    {
      if(strcmp(PT->name, "e") == 0)
      {
	fprintf(fp, "---- ---- ---- ---- %s YES e\n", s->tno->name);
      }
      else if( ( strcmp(PT->name, "NUM") == 0 ))
      {
	fprintf(fp, "%d   %d   %s  %d   %s YES %s\n", (PT->tok.dat).num, PT->tok.line, PT->name, (PT->tok.dat).num, s->tno->name, PT->name);
      }
      else if( ( strcmp(PT->name, "RNUM") == 0 ))
      {
	fprintf(fp, "%.2f   %d   %s   %.2f   %s YES %s\n", (PT->tok.dat).rnum, PT->tok.line, PT->name, (PT->tok.dat).rnum, s->tno->name, PT->name);
      }
      else
      {
	fprintf(fp, "%s   %d   %s   %s   %s YES %s\n", (PT->tok).dat.str, PT->tok.line, PT->name, (PT->tok).dat.str, s->tno->name, PT->name);
      }
    }
  }
  push(PT);
  printTreeRecurse(PT->child, fp);
  pop();
  printTreeRecurse(PT->peer, fp);
  return;
}
void printParseTree(parseTree PT, char *outfile)
{	/* lexemeCurrentNode     lineno   token   valueIfNumber   parentNodeSymbol   isLeafNode(yes/no)    NodeSymbol*/
  FILE *f = fopen(outfile, "w+");
  printTreeRecurse(PT, f);
  fclose(f);
}
/*END OF PRINTING*/

parseTree initParser(char* tsf)
{
	ruleCount = 88;
	flag = 0;
	FILE *fp;
	fp = fopen("test_grammar.txt", "r+");  		//Pointer to the file containing the grammar rules
	int i = 0;
 	G = (node**)calloc(ruleCount, sizeof(node*)); 		//An array of linked lists to store the productions141
	G[0] = (node*)calloc(1, sizeof(node));
	node *temp, *temp1;
	
	while(fscanf(fp, "%s", (G[i]->symbol)) != EOF)
	{
	  temp = G[i];
	  G[i]->next = (node*)calloc(1, sizeof(node));
	  G[i]->previous = NULL;
	  temp1 = G[i];
	  G[i] = G[i]->next;
	  int c;					//To store EOF when it is encountered
	
	  while((c = fscanf(fp, "%s", (G[i]->symbol))) != EOF)	//Per line
	  { //printf("The value of i is:%d\n", i);	//Debug
	    if(fgetc(fp) == '\n') break;	//End of line
	    G[i]->next = (node* )calloc(1, sizeof(node));
	    G[i]->previous = temp1;
	    temp1 = G[i];
	    G[i] = G[i]->next;
	  }

	G[i]->previous = temp1;
	G[i]->next = NULL;
	temp->last = G[i];
	G[i] = temp;
	
	i++;	//Next grammar symbol
	if(c == EOF) break;
	G[i] = (node*)calloc(1, sizeof(node));
	}
	fclose(fp);
	createParseTable(G, T);	
	//printf("Parse table created successfully\n");
	return parseInputSourceCode(tsf, T);
	
}
#endif
#endif