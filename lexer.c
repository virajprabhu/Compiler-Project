/*PUSHKARA RAVINDRA 2011A7PS150P
 *VIRAJ PRABHU	    2011A7PS044P
 * BATCH-1*/
/*LEXER_C*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"
#include "lexer.h"

buffer A;	//Twin buffers A and B
buffer B;
int begin;	//The value of begin ptr
int beginC;	//Which of the twin buffers begin is in
int forward;	//The valur of forward ptr
int forwardC;	//Which of the twin buffers forward is in
int spillFlag;	//Tells if a lexeme spilled off of a buffer in the previous iteration of getTokens
int lineNumber;
buffersize bufsiz;

FILE *initLexer( char* filename )
{
	//printf("Initializing\n");
	FILE *fp;
	fp = fopen ( filename, "r" );
	int i = 0;
	bufsiz = 512;				//Assumed buffersize fixed at 22
	//printf("allocating memory\n");
	A = (char*)calloc(bufsiz, sizeof(char)); 
	B = (char*)calloc(bufsiz, sizeof(char));
	//printf("getting streams\n");
	fp = getStream( fp, A, bufsiz);	
	fp = getStream( fp, B, bufsiz);
	//printf("got streams\n");
	begin = 0;
	beginC = 0;
	forward = 0;
	forwardC = 0;
	spillFlag = 0;
	lineNumber = 1;
	return fp;
}

FILE *getStream( FILE *fp, buffer C, buffersize k)
{
	int i = 0;
	for ( i = 0; i<k; i++ )
		*(C+i) = (char)fgetc(fp);
	return fp;
}

int endLexer(FILE *fp)
{
	fclose(fp);
	free(A);
	free(B);
	return 1;
}

void getStr(char **ptr)
{
	char *toReturn = (char*)calloc(22, sizeof(char));	
	strcpy(toReturn,"\0");
	int i;
	int j =0;
	if (beginC == 0){
		i = begin;
		if(forwardC != 1){
			while(i < 22)
			{
				toReturn[j]= *(A+i);
				j++;
				i++;
			}
			i == 0;
			while(i < forward){
				toReturn[j]= *(B+i);
				j++;
				i++;
			}
		}
		else{
			while(i<forward){
				toReturn[j]= *(A+i);
				j++;
				i++;
			}
		}
	}
	else{
		i = begin;
		if(forwardC != 1){
			while(i < 22)
			{
				toReturn[j]= *(A+i);
				j++;
				i++;
			}
			i == 0;
			while(i < forward){
				toReturn[j] = *(A+i);
				 j++;
				i++;
			}
		}
		else{
			while(i<forward){
				toReturn[j] = *(B+i);
				j++;
				i++;
			}
		}
	}
	toReturn[i] = '\0';	
	*ptr = toReturn;
	return;
}

void getStrR(char **ptr)
{
	char *toReturn = (char*)calloc(22, sizeof(char));
	strcpy(toReturn,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
	int i;
	int j = 0;
	if (beginC == 0){
		i = begin;
		if(forwardC != 0){
			while(i < 22)
			{
				toReturn[j]= *(A+i);
				j++;
				i++;
			}
			i == 0;
			while(i < (forward-1)){
				toReturn[j]= *(B+i);
				j++;
				i++;
			}
		}
		else{
			while(i<(forward - 1)){
				toReturn[j]= *(A+i);
				j++;
				i++;
			}
		}
	}
	else{
		i = begin;
		if(forwardC != 1){
			while(i < 22)
			{
				toReturn[j]= *(B+i);
				j++;
				i++;
			}
			i == 0;
			while(i < (forward - 1)){
				toReturn[j]= *(A+i);
				j++;
				i++;
			}
		}
		else{
			while(i<(forward - 1)){
				toReturn[j]= *(B+i);
				j++;
				i++;
			}
		}
	}
	toReturn[i] = '\0';
	*ptr = toReturn;
	return;
}

int getNumR()
{
	int toReturn = 0;
	int i;
	if (beginC == 0){
		i = begin;
		if(forwardC != 0){
			while(i < 22)
			{
			toReturn *= 10;
			toReturn += (int)(*(A+i) - '0');
				i++;
			}
			i == 0;
			while(i < (forward-1)){
			toReturn *= 10;
			toReturn += (int)(*(B+i) - '0');
				i++;
			}
		}
		else{
			while(i<(forward - 1)){
			toReturn *= 10;
			toReturn += (int)(*(A+i) - '0');
				i++;
			}
		}
	}
	else{
		i = begin;
		if(forwardC != 1){
			while(i < 22)
			{
			toReturn *= 10;
			toReturn += (int)(*(B+i) - '0');
				i++;
			}
			i == 0;
			while(i < (forward - 1)){
			toReturn *= 10;
			toReturn += (int)(*(A+i) - '0');
				i++;
			}
		}
		else{
			while(i<(forward-1)){
			toReturn *= 10;
			toReturn += (int)(*(B+i) - '0');
				i++;
			}
		}
	}

	return toReturn;
}

int getNumRR(){
	
	int toReturn = 0;
	int i;
	if (beginC == 0){
		i = begin;
		if(forwardC != 0){
			while(i < 22)
			{
			toReturn *= 10;
			toReturn += (int)(*(A+i) - '0');
				i++;
			}
			i == 0;
			while(i < (forward-2)){
			toReturn *= 10;
			toReturn += (int)(*(B+i) - '0');
				i++;
			}
		}
		else{
			while(i<(forward-2)){
			toReturn *= 10;
			toReturn += (int)(*(A+i) - '0');
				i++;
			}
		}
	}
	else{
		i = begin;
		if(forwardC != 1){
			while(i < 22)
			{
			toReturn *= 10;
			toReturn += (int)(*(B+i) - '0');
				i++;
			}
			i == 0;
			while(i < (forward - 2)){
			toReturn *= 10;
			toReturn += (int)(*(A+i) - '0');
				i++;
			}
		}
		else{
			while(i<(forward-2)){
			toReturn *= 10;
			toReturn += (int)(*(B+i) - '0');
				i++;
			}
		}
	}

	return toReturn;
}

float getRnum(){
	
	float toReturn = 0.0;
	int i;

	if (beginC == 0){
		i = begin;
		if(forwardC != 0){
			while(i < 22)
			{
				if(strcmp((A+i), ".") != 0)
				{
					toReturn *= 10;
					toReturn += (int)(*(A+i) - '0');
				}
				i++;
			}

			i == 0;

			while(i < (forward-2))
			{
				if(strcmp((A+i), ".") != 0)
				{
					toReturn *= 10;
					toReturn += (int)(*(B+i) - '0');
				}
				i++;
			}
		}
		else{
			while(i<(forward-2))
			{
				if(strcmp((A+i), ".") != 0)
				{
				toReturn *= 10;
				toReturn += (int)(*(A+i) - '0');
				}
				i++;
			}
		}
	}
	else{
		i = begin;
		if(forwardC != 1){
			while(i < 22)
			{
				if(strcmp((A+i), ".") != 0)
				{
				toReturn *= 10;
				toReturn += (int)(*(B+i) - '0');
				}
				i++;
			}
			i == 0;
			while(i < (forward - 2))
			{
				if(strcmp((A+i), ".") != 0)
				{
					toReturn *= 10;
					toReturn += (int)(*(A+i) - '0');
				}
				i++;
			}
		}
		else{
			while(i<(forward-2))
			{
				if(strcmp((A+i), ".") != 0)
				{
					toReturn *= 10;
					toReturn += (int)(*(B+i) - '0');
				}
				i++;
			}
		}
	}
	toReturn = toReturn/100.0;
	return toReturn;
}
tokenInfo getNextToken( FILE *fp )
{
	int state = 0;
	char curr = '\0';
	tokenInfo toReturn;
	int i = 0;
	int overflow = 0;
	int length = 0;
	toReturn.dat.str = NULL;

	while(1)
	{
		toReturn.line = lineNumber;
		switch(state){
			
			case(0):
			//printf("state 0->");
			if( spillFlag )
			{
				//printf("getting new streams\n");
				if ( beginC )
					fp = getStream( fp, A, 22 );
				else fp = getStream ( fp, B, 22);
				//printf("got new stream\n");
			}
			spillFlag = 0;
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			//printf("input\n");
			if (curr == ' '|| curr == '\t') 
			{
				HIT();
				break;
			}
			else if (curr == '\n')
			{
				HIT();
				lineNumber++;
				break;
			}
			else if ( curr == '=')
			{
				state = 1;
				break;
			}
			else if ( curr == '#' )
			{
				state = 2;
				break;
			}
			else if ( curr == '[')
			{
				state = 3;
				break;
			}
			else if ( curr == ']' ) {
				state = 4;
				break;
			}
			else if ( curr == '(' )
		 	{
				state = 5;
				break;
			}
			else if ( curr == ')' )
			{
				state = 6;
				break;
			}
			else if ( curr == ';' )
			{
				state = 7;
				break;
			}
			else if ( curr == ',' )
			{
				state = 8;
				break;
			}
			else if ( curr == '+' )
			{
				state = 9;
				break;
			}
			else if ( curr == '-' )
			{
				state = 10;
				break;
			}
			else if ( curr == '*' )
			{
				state = 11;
				break;
			}
			else if ( curr == '/' )
			{
				state = 12;
				break;
			}
			else if ( curr == '@' )
			{
				state = 13;
				break;
			}
			else if ( curr == '<' )
			{
				state = 14;
				break;
			}
			else if (curr == '>')
			{
				state = 15;
				break;
			}
			else if (curr == '.')
			{
				state = 16;
				break;
			}
			else if (curr == '"')
			{
				state = 17;
				break;
			}
			else if (curr == '_')
			{
				state = 18;
				break;
			}
			else if ('a'<= curr && curr <= 'z')
			{
				if(curr == 'e')
				{
					state = 19;
					break;
				}
				else if(curr == 'i')
				{
					state = 20;
					break;
				}
				else if (curr == 'm')
				{
					state = 21;
					break;
				}
				else if (curr == 'p')
				{
					state = 22;
					break;
				}
				else if (curr == 'r')
				{
					state = 23;
					break;
				}
				else if(curr =='s')
				{
					state = 45;
					break;
				}
				else if(curr == 'f')
				{
					state = 97;
					break;
				}
				else 
				{
					state = 24;
					break;
				}
			}
			else if ('A'<= curr && curr<='Z')
			{
				state = 24;
				break;
			}
			else if ('0'<= curr && curr<='9')
			{
				state = 25;
				break;
			}
			else if(curr == EOF){
				toReturn.token = "$";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "$");
				return toReturn;
			}
			else
			{
				printf("Unkown Symbol %c at %d\n", curr, lineNumber);
				toReturn.token = "!";
				HIT();
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
			break;	
			}
			
			
			
			
			case (1):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if ( curr == '=' )
			{
				state = 26;
				break;
			}
			else if ( curr == '/' )
			{
				state = 27;
				break;
			}
			else
			{
				HITR();
				toReturn.token = "ASSIGNOP";
				toReturn.dat.str = (char *)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str,"=");
				state = 0;
				return toReturn;
			}
			
			case (2):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == '\n'){
				HIT();
				lineNumber++;
				state = 0;
			}
			HIT();
			break;
			
			case (3):
			HIT();
			toReturn.token = "SQO";
			toReturn.dat.str = (char *)calloc(2, sizeof(char));
			strcpy(toReturn.dat.str,"[");
			state = 0;
			return toReturn;

			case (4):
			HIT();
			toReturn.token = "SQC";
			toReturn.dat.str = (char *)calloc(2, sizeof(char));
			strcpy(toReturn.dat.str,"]");
			state = 0;
			return toReturn;
			
			case (5):
			HIT();
			toReturn.token = "OP";
			toReturn.dat.str = (char *)calloc(2, sizeof(char));
			strcpy(toReturn.dat.str,"(");
			state = 0;
			return toReturn;

			case (6):
			HIT();
			toReturn.token = "CL";
			toReturn.dat.str = (char *)calloc(2, sizeof(char));
			strcpy(toReturn.dat.str,")");
			state = 0;
			return toReturn;
		
			case (7): HIT();
			 toReturn.token = "SEMICOLON"; 
			toReturn.dat.str = (char *)calloc(2, sizeof(char));
			strcpy(toReturn.dat.str,";");
			state = 0;
			return toReturn;
			
			case (8):
			HIT();
			toReturn.token = "COMMA";
			toReturn.dat.str = (char *)calloc(2, sizeof(char));
			strcpy(toReturn.dat.str,",");
			state = 0;
			return toReturn;
			
			case (9):
			HIT();
			toReturn.token = "PLUS";
			toReturn.dat.str = (char *)calloc(2, sizeof(char));
			strcpy(toReturn.dat.str,"+");
			state = 0;
			return toReturn;
			
			case (10):
			HIT();
			toReturn.token = "MINUS";
			toReturn.dat.str = (char *)calloc(2, sizeof(char));
			strcpy(toReturn.dat.str,"-");
			state = 0;
			return toReturn;
			
			case(11):
			HIT();
			toReturn.token = "MUL";
			toReturn.dat.str = (char *)calloc(2, sizeof(char));
			strcpy(toReturn.dat.str,"*");
			state = 0;
			return toReturn;
			
			case (12):
			HIT();
			toReturn.token = "DIV";
			toReturn.dat.str = (char *)calloc(2, sizeof(char));
			strcpy(toReturn.dat.str,"/");
			state = 0;
			return toReturn;
			
			case (13):
			HIT();
			toReturn.token = "SIZE";
			toReturn.dat.str = (char *)calloc(2, sizeof(char));
			strcpy(toReturn.dat.str,"@");
			state = 0;
			return toReturn;
			
			case (14):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if (curr == '=')
			{
				state = 28;
				break;
			}
			else
			{
				HITR();
				toReturn.token = "LT";
				toReturn.dat.str = (char *)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str,"<");
				state = 0;
				return toReturn;
			}
			
			case (15):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if (curr == '=')
			{
				state = 29;
				break;
			}
			else
			{
				HITR();
				toReturn.token = "GT";
				toReturn.dat.str = (char *)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str,">");
				state = 0;
				return toReturn;
			}
			
			case (16):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if (curr == 'a')
			{
				state = 30;
				break;
			}
			else if (curr == 'o')
			{
				state = 31;
				break;
			}
			else if (curr == 'n')
			{
				state = 32;
				break;
			}
			else
			{
				
				//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
				getStr(&toReturn.dat.str);
				printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token = "!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
			}
			
			case (17):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if (('a' <= curr && curr <= 'z')||curr==' ')
			{
				state = 33;
				break;
			}
			else
			{
				 	
				//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
				getStr(&toReturn.dat.str);
				 printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token = "!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
			}

			case (18):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'm')
			{
				state = 34;
				break;
			}
			else if(('a'<=curr && curr<='z')||('A'<=curr && curr<='Z'))
			{
				state = 35;
				break;
			}
			else
			{
				
				 printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token = "!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
			}
			
			case (19):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'n')
			{
				state = 36;
				break;
			}
			else if(curr == 'l')
			{
				state = 37;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}
			
			case (20):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr=='n')
			{
				state = 40;
				break;
			}
			else if(curr=='f')
			{
				state = 41;
				break;
			}
			else if (('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if ('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			
			case (21):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'a')
			{
				state=42;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state=24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}
			
			case (22):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'r')
			{
				state = 43;
				break;
			}
			 else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state=24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (23):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'e')
			{
				state = 44;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state=24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (24):	//HAVE TO UPDATE THIS TO HANDLE 20 CHARACTER COUNT LIMIT
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state=24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (25):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if('0'<=curr && curr<='9')
			{
				state = 25;
				break;
			}
			else if(curr == '.')
			{
				state = 64;
				break;
			}
			else
			{
				state = 63;
				break;
			}
			
			case (26):
			HIT();
			toReturn.token = "EQ";
			toReturn.dat.str = (char *)calloc(3, sizeof(char));
			strcpy(toReturn.dat.str,"==");
			return toReturn;
			
			case (27):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == '=')
			{
				state = 46;
				break;
			}
			else
			{
				
				//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
				getStr(&toReturn.dat.str);
				
				 printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token = "!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
			}
			
			case (28):
			HIT();
			toReturn.token = "LE";
			toReturn.dat.str = (char *)calloc(3, sizeof(char));
			strcpy(toReturn.dat.str,"<=");
			return toReturn;
			
			case (29):
			HIT();
			toReturn.token = "GE";
			toReturn.dat.str = (char *)calloc(3, sizeof(char));
			strcpy(toReturn.dat.str,">=");
			return toReturn;
			
			case (30):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'n')
			{
				state = 47;
				break;
			}
			else
			{
				
				//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
				getStr(&toReturn.dat.str);
				
				 printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token = "!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
			}
			
			case (31):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'r')
			{
				state = 48;
				break;
			}
			else
			{
				
				//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
				getStr(&toReturn.dat.str);
				
				 printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token = "!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;

				//error
			}
			
			case (32): curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'o')
			{
				state = 49;
				break;
			}
			else
			{
				
				//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
				getStr(&toReturn.dat.str);
					
				 printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token = "!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;

				//error
			}
		
			case (33):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a' <= curr && curr <= 'z')||curr==' ')
			{
				state = 33;
				//MANAGE SIZE OF THINGY
				break;
			}
			else if(curr == '"')
			{
				state = 50;
				break;
			}
			else
			{
				
				//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
				getStr(&toReturn.dat.str);
				
				 printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token = "!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;

				//error
			}
			
			case (34):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'a')
			{
				state = 51;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z') || ('0'<=curr && curr<='9'))
			{
				state = 52;
				break;
			}
			else
			{
				state = 53;
				break;
			}
			
			case (35):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z') || ('0'<=curr && curr<='9'))
			{
				state = 52;
				break;
			}
			else
			{
				state = 53;
				break;
			}

			case (36):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr=='d')
			{
				state = 54;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}
			
			case (37):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 's')
			{
				state = 55;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}
				

			case (38):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr <='z')||('0'<=curr && curr <= '9'))
			{
				printf("Lexical Error: No Space between ID and NUM in line %d\n", lineNumber);
				toReturn.token = "!";
				HITR();
				state = 0;
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
			}
			else
			{
			  state = 105;
			  break;
			}
			
			case (39):
				
			length = length();
			if(length > 20){
				printf("Identifier at line %d is longer than the prescribed length of 20 characters\n", lineNumber);
				toReturn.token = "!";
				HITR();
				state = 0;
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
			}	
			toReturn.token = "ID";
			//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
			getStrR(&toReturn.dat.str);
			HITR();
			state = 0;
			return toReturn;
			
			case (40):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 't')
			{
				state = 85;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (41):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 56;
				break;
			}

			case (42):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr = 't')
			{
				state = 84;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (43):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'i')
			{
				state = 57;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (44):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'a')
			{
				state = 58;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (45):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 't')
			{
				state = 59;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (46):
			HIT();
			toReturn.token = "NE";
			toReturn.dat.str = (char *)calloc(4, sizeof(char));
			strcpy(toReturn.dat.str,"=/=");
			state = 0;
			return toReturn;
			
			case (47):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'd')
			{
				state = 60;
				break;
			}
			else
			{
				
				//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
				getStr(&toReturn.dat.str);
				
				 printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token = "!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;	
			}

			case (48):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == '.')
			{
				state = 61;
				break;
			}
			else
			{
				
				//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
				getStr(&toReturn.dat.str);
				
				 printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token = "!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;	
				//error
			}
			
			case (49):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 't')
			{
				state = 62;
				break;
			}
			else
			{
				//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
				
				getStr(&toReturn.dat.str);
				
				 printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token = "!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;	
				//error
			}
			
			case (50):
			toReturn.token = "STR";
			
			INCPTR();
			length = length();
			if(length > 20){
				printf("Identifier at line %d is longer than the prescribed length of 20 characters\n", lineNumber);
				toReturn.token = "!";
				HITR();
				state = 0;
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
			}	
			//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
			getStrR(&toReturn.dat.str);
			HITR();
			state = 0;
			return toReturn;
			
			case (51):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'i')
			{
				state = 65;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z') || ('0'<=curr && curr<='9'))
			{
				state = 52;
				break;
			}
			else
			{
				state = 53;
				break;
			}

			case (52):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z') || ('0'<=curr && curr<='9'))
			{
				state = 52;
				break;
			}
			else
			{
				state = 53;
				break;
			}
			
			case (53):
			length = length();
			if(length > 20){
				printf("Identifier at line %d is longer than the prescribed length of 20 characters\n", lineNumber);
				toReturn.token = "!";
				HITR();
				state = 0;
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
			}	

			toReturn.token = "FUNID";
			
			//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
			getStrR(&toReturn.dat.str);
			HITR();
			state = 0;
			return toReturn;
			
			case (54):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'i')
			{
				state = 82;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 66;
				break;
			}
	
			case (55):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'e')
			{
				state = 83;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (56):
			toReturn.token = "IF";
			toReturn.dat.str = (char *)calloc(3, sizeof(char));
			strcpy(toReturn.dat.str,"if");
			HITR();
			state = 0;
			return toReturn;
			
			case (57):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'n')
			{
				state = 67;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (58):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'd')
			{
				state = 68;
				break;
			}
			else if(curr == 'l')
			{
			state = 69;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (59):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'r')
			{
				state = 70;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (60):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == '.')
			{
				state = 71;
				break;
			}
			else
			{
				
				//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
				getStr(&toReturn.dat.str);
				
				 printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token ="!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
				//error
			}
			
			case (61):
			HIT();
			toReturn.token = "OR";
			toReturn.dat.str = (char *)calloc(5, sizeof(char));
			strcpy(toReturn.dat.str,".or.");
			state = 0;
			return toReturn;
	
			case (62):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == '.')
			{
				state = 72;
				break;
			}
			else
			{
				
				//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
				getStr(&toReturn.dat.str);
				
				
				 printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token ="!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
				//error
			}
	
			case (63):
			length = length();
			if (length > 5)
			{
				printf("NUM in line %d is too long\n", lineNumber);
				toReturn.token = "!";
				HIT();
				state = 0;
				return toReturn;
			}
			toReturn.token = "NUM";
			toReturn.dat.num = getNumR();
			HITR();
			state = 0;
			return toReturn;
			
			case (64):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if('0'<=curr && curr<='9')
			{
				state = 73;
				break;
			}
			else
			{
				length = length();
				if (length>5)
				{
					printf("NUM in line %d is too long\n", lineNumber);
					toReturn.token = "!";
					HITRR();
					state = 0;
					toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
					return toReturn;
				}
				toReturn.token = "NUM";
				toReturn.dat.num = getNumRR();
				HITRR();
				state = 0;
				return toReturn;
			}
			
			case (65):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'n'){
				state = 74;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z') || ('0'<=curr && curr<='9'))
			{
				state = 52;
				break;
			}
			else
			{
				state = 53;
				break;
			}

			case (66):
			toReturn.token = "END";
			toReturn.dat.str = (char *)calloc(4, sizeof(char));
			strcpy(toReturn.dat.str,"end");
			HITR();
			state = 0;
			return toReturn;

			case (67):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 't')
			{
				state = 75;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (68):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 76;
				break;
			}

			case (69):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 77;
				break;
			}

			case (70):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'i'){
				state = 78;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (71):
			HIT();
			toReturn.token = "AND";
			toReturn.dat.str = (char *)calloc(6, sizeof(char));
			strcpy(toReturn.dat.str,".and.");
			state = 0;
			return toReturn;
			
			case (72):
			HIT();
			toReturn.token = "NOT";
			toReturn.dat.str = (char *)calloc(6, sizeof(char));
			strcpy(toReturn.dat.str,".not.");
			state = 0;
			return toReturn;
			
			case (73):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if('0'<=curr && curr<='9')
			{
				state = 79;
				break;
			}
			else	
			{
				
				//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
				getStr(&toReturn.dat.str);
				
				 printf("Unknown pattern %s at line %d\n",toReturn.dat.str, lineNumber);
				HIT();
				toReturn.token = "!";
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
				//error
			}
			
			case (74):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z') || ('0'<=curr && curr<='9'))
			{
				state = 52;
				break;
			}
			else
			{
				state = 80;
				break;
			}

			case (75):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 81;
				break;
			}

			case (76):
			toReturn.token = "READ";
			toReturn.dat.str = (char *)calloc(5, sizeof(char));
			strcpy(toReturn.dat.str,"read");
			HITR();
			state = 0;
			return toReturn;
		
			case (77):
			toReturn.token = "REAL";
			toReturn.dat.str = (char *)calloc(5, sizeof(char));
			strcpy(toReturn.dat.str,"real");
			HITR();
			state = 0;
			return toReturn;

			case (78):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'n')
			{
				state = 86;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (79):
			length = length();
			if(length > 8)
			{
				printf("RNUM in line %d is too long\n", lineNumber);
				toReturn.token = "!";
				HIT();
				state = 0;
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
			}
			toReturn.token = "RNUM";
			toReturn.dat.rnum = getRnum();
			HIT();
			state = 0;
			return toReturn;

			case (80):
			toReturn.token = "MAIN";
			toReturn.dat.str = (char *)calloc(5, sizeof(char));
			strcpy(toReturn.dat.str,"main");
			HITR();
			state = 0;
			return toReturn;

			case (81):
			toReturn.token = "PRINT";
			toReturn.dat.str = (char *)calloc(5, sizeof(char));
			strcpy(toReturn.dat.str,"print");
			HITR();
			state =0;
			return toReturn;

			case (82):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'f'){
				state = 87;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (83):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 88;
				break;
			}

			case (84):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'r')
			{
				state = 89;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (85):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 90;
				break;
			}	

			case (86):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'g'){
				state = 91;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}  

			case (87):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 92;
				break;
			}

			case (88):
			toReturn.token = "ELSE";
			toReturn.dat.str = (char *)calloc(5, sizeof(char));
			strcpy(toReturn.dat.str,"else");
			HITR();
			state = 0;
			return toReturn;

			case (89):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'i')
			{
				state = 93;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (90):
			toReturn.token = "INT";
			toReturn.dat.str = (char *)calloc(4, sizeof(char));
			strcpy(toReturn.dat.str,"int");
			HITR();
			state = 0;
			return toReturn;

			case (91):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 94;
				break;
			}

			case (92):
			toReturn.token = "ENDIF";
			toReturn.dat.str = (char *)calloc(6, sizeof(char));
			strcpy(toReturn.dat.str,"endif");
			HITR();
			state = 0;
			return toReturn;

			case (93):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'x')
			{
				state = 95;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (94):
			toReturn.token = "STRING";
			toReturn.dat.str = (char *)calloc(7, sizeof(char));
			strcpy(toReturn.dat.str,"string");
			HITR();
			state = 0;
			return toReturn;
			
			case (95):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 96;
				break;
			}

			case (96):
			toReturn.token = "MATRIX";
			toReturn.dat.str = (char *)calloc(7, sizeof(char));
			strcpy(toReturn.dat.str,"matrix");
			HITR();
			state = 0;
			return toReturn;
			
			case (97):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'u')
			{
				state = 98;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (98):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'n')
			{
				state = 99;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (99):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'c')
			{
				state = 100;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (100):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 't')
			{
				state = 101;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

			case (101):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'i')
			{
				state = 102;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

				case (102):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'o')
			{
				state = 103;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

				case (103):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(curr == 'n')
			{
				state = 104;
				break;
			}
			else if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				state = 39;
				break;
			}

				case (104):
			curr = GETCHAR(forwardC, forward);
			INCPTR();
			if(('a'<=curr && curr<='z') || ('A'<=curr && curr<='Z'))
			{
				state = 24;
				break;
			}
			else if('0'<=curr && curr<='9')
			{
				state = 38;
				break;
			}
			else
			{
				toReturn.token = "FUNCTION";
				toReturn.dat.str = (char *)calloc(9, sizeof(char));
				strcpy(toReturn.dat.str, "function");
				HITR();
				state = 0;
				return toReturn;
			}
			
			case (105):
			length = length()
			if(length > 20){
				printf("Identifier at line %d is longer than the prescribed length of 20 characters\n", lineNumber);
			toReturn.token = "!";
				HITR();
				state = 0;
				toReturn.dat.str = (char*)calloc(2, sizeof(char));
				strcpy(toReturn.dat.str, "!");
				return toReturn;
			}
			
			toReturn.token = "ID";
			//strcpy(toReturn.dat.str,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
			getStrR(&toReturn.dat.str);
			HITR();
			state = 0;
			return toReturn;
				  

			
		}
	}
}



// int main(int argc, char **argv)
// {
// 	//char *filename = argv[1];
// 	char *filename = "../file/example.sr";
// 	tokenInfo tok;
// 	//printf("initializing\n");
// 	FILE *fp = initLexer(filename);
// 	//printf("Getting first token \n");
// 	tok = getNextToken(fp);
// 	//printf("starting lexer\n");
// 	while(strcmp(tok.token, "$")){
// 
// 		
// 		if((strcmp(tok.token,"ID") == 0) || (strcmp(tok.token, "FUNID")==0)){
// 			printf("%s, %s, %d\n", tok.token, tok.dat.str, tok.line);
// 		}
// 		else if(strcmp(tok.token, "NUM") == 0){
// 			printf("%s, %d, %d\n", tok.token, tok.dat.num, tok.line);
// 		}
// 		else if(strcmp(tok.token, "RNUM") == 0){
// 			printf("%s, %.2f, %d\n", tok.token, tok.dat.rnum, tok.line);
// 		}
// 		else{
// 			printf("%s, %d\n", tok.token, tok.line);
// 		}
// 		tok.dat.num = 0;
// 		tok = getNextToken(fp);
// 	}
// 	endLexer(fp);
// 	//printf("done");
// 	return 1;
// }