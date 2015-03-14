/*codegen.h*/
extern FILE* initFile(char* outfile);
extern void assignType(ast_node* root);
extern int makeStaticVars(symbolTable* stab, FILE* out);
extern void makeCode(symbolTable* stab, ast_node* root, FILE* out);
extern void makeTempVars(ast_node* root, FILE* out, int j, int i);
extern void genCode(symbolTable* stab, ast_node* root, char* outfile);
