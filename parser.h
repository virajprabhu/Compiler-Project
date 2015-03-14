/*PUSHKARA RAVINDRA 2011A7PS150P
 *VIRAJ PRABHU	    2011A7PS044P
 * BATCH-1*/
/*parser.h*/
#include "parserDef.h"
extern void createParseTable(grammar G, Table T[][tCount]);
extern parseTree parseInputSourceCode(char *testcaseFile, Table T[][tCount]);
extern void printParseTree(parseTree PT, char *outfile);
extern void push(tnode *t);
extern void pop();
extern int get_nt_Index(char* str);
extern int get_t_index(char* str);
extern void printTreeRecurse(parseTree PT, FILE *fp);
extern parseTree initParser(char *tsf);
extern void initAST(parseTree R);