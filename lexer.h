/*PUSHKARA RAVINDRA 2011A7PS150P
 *VIRAJ PRABHU	    2011A7PS044P
 * BATCH-1*/
/*LEXER_H*/
#define INCPTR() if ( forward<bufsiz-1 ) forward++; else { if ( forwardC == 1 ) forwardC = 0; else forwardC = 1; forward=0;} //Assumed buffersized fixed at 22
#define GETCHAR(a, b) (a==1) ? *(B+b) : *(A+b)	//See if I can make it only GETCHAR and use forwardC and forward instead of a and b
#define HIT() if(beginC != forwardC) spillFlag = 1; beginC=forwardC; begin=forward;
#define HITR() if ( forward>0 ) forward--; else { if (forwardC == 1 ) forwardC = 0; else forwardC = 1; forward = bufsiz-1;} HIT();
#define HITRR() if ( forward>0 ) forward--; else { if (forwardC == 1 ) forwardC = 0; else forwardC = 1; forward = bufsiz-1;} HITR();
#define length() ( forwardC == beginC ) ? (forward - begin) : (bufsiz - begin) + forward;
extern FILE *initLexer( char* filename );
extern FILE *getStream( FILE* fp, buffer B, buffersize k );
extern tokenInfo getNextToken( FILE* fp );
extern int endLexer(FILE *fp);

extern void getStr(char **ptr);
extern void getStrR(char **ptr);
extern int getNumR();
extern int getNumRR();
extern FILE *MIDHIT();
extern float getRnum();