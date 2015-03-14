/*codegen.c*/
/*PUSHKARA RAVINDRA 2011A7PS150P
 *VIRAJ PRABHU	    2011A7PS044P
 * BATCH-1*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"
#include "symbolTable.h"
#include "astDef.h"
#include "codegen.h"

int ifcount = 0;
int ifflag = 0;

FILE* initFile(char* outfile)
{
	FILE* out = fopen(outfile, "w");
	fprintf(out, ".model small\n");
	return out;
	
	//PUT HERE ALL THE STUFF THAT HAS TO BE PUT

}

void assignType(ast_node* root)
{
	if(root == NULL)
		return;
	
	assignType(root->child);
	
	if(strcmp(root->name, "AEXP") == 0 || strcmp(root->name, "ATRM") == 0 )
	{
		if(root->child->peer == NULL)
		{
			strcpy(root->type, root->child->type);
		}
		else
		{
			if(strcmp(root->child->peer->child->name, "DIV") == 0)
			{
				strcpy(root->type, "REAL");
			}
			else
			{
				strcpy(root->type, root->child->type);
			}
		}
	}
	else if(strcmp(root->name, "ARST") == 0|| strcmp(root->name, "ATRST") == 0)
	{
		if(root->child->peer->peer == NULL)
		{
			strcpy(root->type, root->child->peer->type);
		}
		else
		{
			if(strcmp(root->child->peer->peer->child->name, "DIV") == 0)
			{
				strcpy(root->type, "REAL");
			}
			else
			{
				strcpy(root->type, root->child->peer->type);
			}
		}
	}
	else if(strcmp(root->name, "ID")==0)
		strcpy(root->type, root->ptr->type);
	else if(strcmp(root->name, "NUM")==0)
		strcpy(root->type, "INT");
	else if(strcmp(root->name, "RNUM")==0)
		strcpy(root->type, "REAL");
	else if(strcmp(root->name, "STR")==0)
		strcpy(root->type, "STRING");
	else if(strcmp(root->name, "BEXP")==0)
		strcpy(root->type, "BOOL");
	else if(strcmp(root->name, "AST2")==0)
		if(strcmp(root->child->peer->name, "ID")==0)
			strcpy(root->type, "INTXINT");

	assignType(root->peer);
	return;
}


int makeStaticVars(symbolTable* stab, FILE* out) //not caring about other functions. Screw it
{
	int i = 0, j=0;
	if(stab == NULL) printf("WWWWWWWWWWTTTTTTTHHHHHHHHH!!!!\n");
	for(i = 0; i < 50; i++)	
	{
		//take stab[i], check its size and allocate data according
		if(stab->H[i].name == NULL)
		        continue;
		        
		if(strcmp(stab->H[i].name, "\0") == 0 || strcmp(stab->H[i].name, "$"))
			continue;
			
		if(strcmp(stab->H[i].type, "INT")==0)
		{
			fprintf(out, "%s dw\n", stab->H[i].name);
			stab->H[i].offset = j;
			j+=2;
		}
		else if(strcmp(stab->H[i].type, "REAL")==0)
		{
			
                        stab->H[i].offset = j;
			j+=4;
		}
		else if(strcmp(stab->H[i].type, "STRING")==0)
		{
			
			stab->H[i].offset = j;
			j+=21;
		}
		else if(strcmp(stab->H[i].type, "MATRIX")==0)
		{
			//
		}
		
	}
	return j;
	//what about the temporaries?
	
}

void makeTempVars(ast_node* root, FILE* out, int j, int i)
{
       if(root == NULL)
                return;
       if(strcmp(root->type, "\0") == 0)
       {
                makeTempVars(root->child, out, j, i);
                makeTempVars(root->peer, out, j, i);
                return;
       }
       if(strcmp(root->type, "INT")==0)
	{
		fprintf(out, "t%d dw\n", i);//things to assign mem
		
		root->offset = i;
		j+=2;
		i++;
	}
	else if(strcmp(root->type, "REAL")==0)
	{
		
                       root->offset = i;
		j+=4;
		i++;
	}
	else if(strcmp(root->type, "STRING")==0)
	{
		
		root->offset = i;
		j+=21;
		i++;
	}
	else if(strcmp(root->type, "MATRIX")==0)
	{
		//
	}
	else if(strcmp(root->type, "BOOL")==0)
	{
	        fprintf(out, "t%d dw\n", i);
	        root->offset = i;
		j+=2;
		i++;
	}
}

void makeCode(symbolTable* stab, ast_node* root, FILE* out)
{
        if(root == NULL)
                return;
        else if(strcmp(root->name, "IFST")==0)
	{
	        ifcount++;
	        ifflag = 1;
	        makeCode(stab, root->child, out);
	        fprintf(out, "mov ax, offset t%d\n", root->child->offset);
	        fprintf(out, "mov dx, [ax]\n");
	        fprintf(out, "cmp dx, 0001h");
	        fprintf(out, "jne else%d", ifcount);
	        
	        makeCode(stab, root->child->peer, out);
	        makeCode(stab, root->child->peer->peer, out);
	        fprintf(out, "jmp endif%d", ifcount);
	        
	        fprintf(out, "else%d : ", ifcount);
	        makeCode(stab, root->child->peer->peer->peer, out);
	        fprintf(out, "endif%d : ", ifcount);
	        ifflag = 0;	        
	}
	else if(strcmp(root->name, "AST1")==0)
	{
	     if(ifflag)
	        {
	                ifflag=0;
        	        makeCode(stab, root->child, out);
        	        ifflag=1;
        	}   
        	
        	fprintf(out, "mov ax, offset %s\n", root->child->ptr->name);
        	fprintf(out, "mov cx, offset t%d\n", root->child->peer->offset);
        	fprintf(out, "call assignint\n");
        	
        	if(!ifflag)
        	        makeCode(stab, root->peer, out);
	}
	/*CODE GENERATIONG FOR BOOLEAN CASES*/
	/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
	else if(strcmp(root->name, "BEXP")==0)
	{       
	        if(ifflag)
	        {
	                ifflag=0;
        	        makeCode(stab, root->child, out);
        	        ifflag=1;
        	}
        	else
        	        makeCode(stab, root->child, out);
	        
	        if(strcmp(root->child->peer->name, "AND")==0)
	        {
	                fprintf(out, "mov ax, offset t%d\n", root->child->offset);
        		fprintf(out, "mov bx, offset t%d\n", root->child->peer->peer->offset);
        		fprintf(out, "mov cx, offset t%d\n", root->offset);
                	fprintf(out, "call logicand\n");
	        }
	        else if(strcmp(root->child->peer->name, "OR")==0)
	        {
	                fprintf(out, "mov ax, offset t%d\n", root->child->offset);
        		fprintf(out, "mov bx, offset t%d\n", root->child->peer->peer->offset);
        		fprintf(out, "mov cx, offset t%d\n", root->offset);
                	fprintf(out, "call logicor\n");
	        }
	        else if(strcmp(root->child->peer->name, "LT")==0)
	        {
	                fprintf(out, "mov ax, offset t%d\n", root->child->offset);
        		fprintf(out, "mov bx, offset t%d\n", root->child->peer->peer->offset);
        		fprintf(out, "mov cx, offset t%d\n", root->offset);fprintf(out, "        MOV DX, [AX]\n");
	                
	                if(strcmp(root->child->peer->name, "LT")==0)
	                {
	                        fprintf(out, "call relationlt");        		
	                }
        	        else if(strcmp(root->child->peer->name, "LE")==0)
        	        {
        	                fprintf(out, "call relationle");        		
        	        }
        	        else if(strcmp(root->child->peer->name, "GT")==0)
        	        {
        	                fprintf(out, "call relationgt");
        	        }
        	        else if(strcmp(root->child->peer->name, "GE")==0)
        	        {
                	        fprintf(out, "call relationge");
        	        }
        	        else if(strcmp(root->child->peer->name, "EQ")==0)
        	        {
        	                fprintf(out, "call relationeq");
        	        }
        	        else if(strcmp(root->child->peer->name, "NE")==0)
        	        {
        	                fprintf(out, "call relationne");
        	        }
        	}
        	
        	if(!ifflag)
        	        makeCode(stab, root->peer, out);
        	
        	         
	}
	
	else if(strcmp(root->name, "VAR") == 0)
	{
                if(root->child->peer == NULL)
                {
                       fprintf(out, "mov ax, offset %s\n", root->child->ptr->name);
        	       fprintf(out, "mov cx, offset t%d\n", root->offset);
        	       fprintf(out, "call assignint\n");
                }
                else
                {
                //matrix
                }
                if(!ifflag)
        	        makeCode(stab, root->peer, out);
        }
        

	else if(strcmp(root->name, "AEXP") == 0 || strcmp(root->name, "ATRM") == 0 )
	{
                if(ifflag)
	        {
	                ifflag=0;
	                makeCode(stab, root->child, out);
        	        ifflag=1;
        	}
        	else
        	        makeCode(stab, root->child, out);
	        
	        if(strcmp(root->child->name, "NUM") == 0 )
        	{
        	       fprintf(out, "mov ax, %d\n", (root->child->tok).dat.num);
        	       fprintf(out, "mov cx, offset t%d\n", root->offset);
        	       fprintf(out, "mov [cx], ax\n");
        	       root->child->offset = root->offset;
        	}
        	                
		if(root->child->peer == NULL)
		{
		        if(strcmp(root->type, "INT")==0)
		        {
		                fprintf(out, "mov ax, offset t%d\n", root->child->offset);
        			fprintf(out, "mov cx, offset t%d\n", root->offset);
        			fprintf(out, "call assignint\n");
        		}
		}
		else
		{
		        if(strcmp(root->type, "INT")==0)
		        {
			        if(strcmp(root->child->peer->child->name, "ADD") == 0)
        			{
        				fprintf(out, "mov ax, offset t%d\n", root->child->offset);
        				fprintf(out, "mov bx, offset t%d\n", root->child->peer->offset);
        			        fprintf(out, "mov cx, offset t%d\n", root->offset);
                			fprintf(out, "call addint\n");
        			}
        			else if(strcmp(root->child->peer->child->name, "SUB") == 0)
        			{
        				fprintf(out, "mov ax, offset t%d\n", root->child->offset);
        				fprintf(out, "mov bx, offset t%d\n", root->child->peer->offset);
        			        fprintf(out, "mov cx, offset t%d\n", root->offset);
                			fprintf(out, "call subint\n");
        			}
        			else if(strcmp(root->child->peer->child->name, "MUL") == 0)
        			{
        				fprintf(out, "mov ax, offset t%d\n", root->child->offset);
        				fprintf(out, "mov bx, offset t%d\n", root->child->peer->offset);
        			        fprintf(out, "mov cx, offset t%d\n", root->offset);
                			fprintf(out, "call mulint\n");
        			}
        			else if(strcmp(root->child->peer->child->name, "DIV") == 0)
        			{
        			        //not handling division
        			}
        		}
		}
		
		if(!ifflag)
        	        makeCode(stab, root->peer, out);
		
	}
	else if(strcmp(root->name, "ARST") == 0|| strcmp(root->name, "ATRST") == 0)
	{
	        
                if(ifflag)
	        {
	                ifflag=0;
        	        makeCode(stab, root->child, out);
        	        ifflag=1;
        	}
        	else
        	        makeCode(stab, root->child, out);
	        
        	if(strcmp(root->child->name, "NUM") == 0 )
        	{
        	       fprintf(out, "mov ax, %d\n", (root->child->tok).dat.num);
        	       fprintf(out, "mov cx, offset t%d\n", root->offset);
        	       fprintf(out, "mov [cx], ax\n");
        	       root->child->peer->offset = root->offset;
        	}
		
		if(root->child->peer->peer == NULL)
		{
        		if(strcmp(root->type, "INT")==0)
		        {
        			fprintf(out, "mov ax, offset t%d\n", root->child->peer->offset);
        			fprintf(out, "mov cx, offset t%d\n", root->offset);
        			fprintf(out, "call assignint\n");
        		}
		}
		else
		{
			if(strcmp(root->type, "INT")==0)
		        {
			        if(strcmp(root->child->peer->peer->child->name, "ADD") == 0)
        			{
        				fprintf(out, "mov ax, offset t%d\n", root->child->peer->offset);
        				fprintf(out, "mov bx, offset t%d\n", root->child->peer->peer->offset);
        			        fprintf(out, "mov cx, offset t%d\n", root->offset);
                			fprintf(out, "call addint\n");
        			}
        			else if(strcmp(root->child->peer->peer->child->name, "SUB") == 0)
        			{
        				fprintf(out, "mov ax, offset t%d\n", root->child->peer->offset);
        				fprintf(out, "mov bx, offset t%d\n", root->child->peer->peer->offset);
        			        fprintf(out, "mov cx, offset t%d\n", root->offset);
                			fprintf(out, "call subint\n");
        			}
        			else if(strcmp(root->child->peer->peer->child->name, "MUL") == 0)
        			{
        				fprintf(out, "mov ax, offset t%d\n", root->child->peer->offset);
        				fprintf(out, "mov bx, offset t%d\n", root->child->peer->peer->offset);
        			        fprintf(out, "mov cx, offset t%d\n", root->offset);
                			fprintf(out, "call mulint\n");
        			}
        			else if(strcmp(root->child->peer->peer->child->name, "DIV") == 0)
        			{
        			        //not handling division
        			}
        		}
			else
			{
				strcpy(root->type, root->child->peer->type);
			}
		}
		
	        if(!ifflag)
        	        makeCode(stab, root->peer, out);
	}
	else
	{
	        if(ifflag)
	        {
	                ifflag = 0;
	                makeCode(stab, root->child, out);
	                ifflag = 1;
	        }
	        else
	        {
	                makeCode(stab, root->child, out);
	                makeCode(stab, root->peer, out);
	        }
	                
	}
	
	
	//else if(strcmp(root->name, "ID")==0)
	//	strcpy(root->type, root->ptr->type);
//	else if(strcmp(root->name, "NUM")==0)
		//strcpy(root->type, "INT");
//	else if(strcmp(root->name, "RNUM")==0)
		//strcpy(root->type, "REAL");
//	else if(strcmp(root->name, "STR")==0)
		//strcpy(root->type, "STRING");
//	else if(strcmp(root->name, "BEXP")==0)
		//strcpy(root->type, "BOOL");
//	else if(strcmp(root->name, "AST2")==0)
		//if(strcmp(root->child->peer->name, "ID")==0)
			//strcpy(root->type, "INTXINT");

	//assignType(root->peer);
	return;
	
	
}

void genCode(symbolTable* stab, ast_node* root, char* outfile)
{
	//printf("making file\n");/////////////////////////////////
	FILE* out = initFile(outfile);
	
	//printf("assigning type\n");/////////////////////
	assignType(root);
        
        //printf("making vars\n");//////////////////////////////////////////////////
        fprintf(out, ".data?\n");
        fprintf(out, "CONSTINT dw");
        int j;
        j = makeStaticVars(stab, out);
        //printf("making temp vars");
	makeTempVars(root, out, j, 0);
	//printf("done making vars\n");///////////////////////////////////
	
	fprintf(out, ".code\n");
	fprintf(out, "org 100h\n");
	        

        fprintf(out, "addint proc near        ;AX has first address, BX the other address, CX has destination index\n");
	fprintf(out, "        MOV DX, [AX]\n");
	fprintf(out, "        MOV AX, DX\n");
	fprintf(out, "        MOV DX, [BX]\n");
	fprintf(out, "        MOV BX, DX\n");
	fprintf(out, "        ADD AX, BX\n");
	fprintf(out, "        MOV [CX], AX\n");
	fprintf(out, "        ret\n");
	fprintf(out, "addint endp\n");
	fprintf(out, "\n");
	fprintf(out, "subint proc near        ;AX has first address, BX the other address, CX has destination index\n");
	fprintf(out, "        MOV DX, [AX]\n");
	fprintf(out, "        MOV AX, DX\n");
	fprintf(out, "        MOV DX, [BX]\n");
	fprintf(out, "        MOV BX, DX\n");
	fprintf(out, "        SUB AX, BX\n");
	fprintf(out, "        MOV [CX], AX\n");
	fprintf(out, "        ret\n");
	fprintf(out, "subint endp\n");
	fprintf(out, "\n");
	fprintf(out, "mulint proc near        ;AX has first address, BX the other address, CX has destination index\n");
	fprintf(out, "        MOV DX, [AX]\n");
	fprintf(out, "        MOV AX, DX\n");
	fprintf(out, "        MOV DX, [BX]\n");
	fprintf(out, "        MOV BX, DX\n");
	fprintf(out, "        MUL AX, BX\n");
	fprintf(out, "        MOV [CX], AX\n");
	fprintf(out, "        ret\n");
	fprintf(out, "mulint endp\n");
	fprintf(out, "\n");
	fprintf(out, ";divint proc near        ;AX has first address, BX the other address, CX has destination index\n");
	fprintf(out, "                        ;this be different\n");      
	fprintf(out, "\n");
	fprintf(out, "	assignint proc near     ;[AX] = [CX]\n");
	fprintf(out, "        MOV DX, [AX]\n");
	fprintf(out, "        MOV [CX], DX\n");
	fprintf(out, "        ret\n");
	fprintf(out, "assighnint endp\n");
	fprintf(out, "\n");
	
	fprintf(out, "	logicand proc near      ;AX and BX be operands, CX be dest\n");
	fprintf(out, "        MOV DX, [AX]\n");
	fprintf(out, "        MOV AX, DX\n");
	fprintf(out, "        MOV DX, [BX]\n");
	fprintf(out, "        MOV BX, DX\n");
	fprintf(out, "        AND AX, BX\n");
	fprintf(out, "        MOV [CX], AX\n");
	fprintf(out, "        ret\n");
	fprintf(out, "logicand endp\n");
	fprintf(out, "\n");
	fprintf(out, "logicor proc near      ;AX and BX be operands, CX be dest\n");
	fprintf(out, "        MOV DX, [AX]\n");
	fprintf(out, "        MOV AX, DX\n");
	fprintf(out, "        MOV DX, [BX]\n");
	fprintf(out, "        MOV BX, DX\n");
	fprintf(out, "        OR AX, BX\n");
	fprintf(out, "        MOV [CX], AX\n");
	fprintf(out, "        ret\n");
	fprintf(out, "logicor endp\n");
	fprintf(out, "\n");
	
      	fprintf(out, "\n");
      	fprintf(out, "relationlt proc near\n");
      	fprintf(out, "        MOV AX, DX\n");
      	fprintf(out, "        MOV [CX], 0000h");
       	fprintf(out, "        MOV DX, [BX]\n");
      	fprintf(out, "        MOV BX, DX\n");
       	fprintf(out, "        CMP AX, BX\n");
        fprintf(out, "        JGE LT\n");
        fprintf(out, "        MOV [CX], 0001h\n");
        fprintf(out, "        LT : ret\n");
        fprintf(out, "relationlt endp\n");
        fprintf(out, "\n");
      	fprintf(out, "relationle proc near\n");
      	fprintf(out, "        MOV AX, DX\n");
      	fprintf(out, "        MOV [CX], 0000h");
       	fprintf(out, "        MOV DX, [BX]\n");
      	fprintf(out, "        MOV BX, DX\n");
       	fprintf(out, "        CMP AX, BX\n");
        fprintf(out, "        JG LE\n");
        fprintf(out, "        MOV [CX], 0001h\n");
        fprintf(out, "        LE : ret\n");
        fprintf(out, "relationle endp\n");
        fprintf(out, "\n");
      	fprintf(out, "relationeq proc near\n");
      	fprintf(out, "        MOV AX, DX\n");
      	fprintf(out, "        MOV [CX], 0000h");
       	fprintf(out, "        MOV DX, [BX]\n");
      	fprintf(out, "        MOV BX, DX\n");
       	fprintf(out, "        CMP AX, BX\n");
        fprintf(out, "        JNE EQ\n");
        fprintf(out, "        MOV [CX], 0001h\n");
        fprintf(out, "        EQ : ret\n");
        fprintf(out, "relationeq endp\n");
        fprintf(out, "\n");
      	fprintf(out, "relationne proc near\n");
      	fprintf(out, "        MOV AX, DX\n");
      	fprintf(out, "        MOV [CX], 0000h");
       	fprintf(out, "        MOV DX, [BX]\n");
      	fprintf(out, "        MOV BX, DX\n");
       	fprintf(out, "        CMP AX, BX\n");
        fprintf(out, "        JE NE\n");
        fprintf(out, "        MOV [CX], 0001h\n");
        fprintf(out, "        NE : ret\n");
        fprintf(out, "relationne endp\n");
        fprintf(out, "\n");
      	fprintf(out, "relationge proc near\n");
      	fprintf(out, "        MOV AX, DX\n");
      	fprintf(out, "        MOV [CX], 0000h");
       	fprintf(out, "        MOV DX, [BX]\n");
      	fprintf(out, "        MOV BX, DX\n");
       	fprintf(out, "        CMP AX, BX\n");
        fprintf(out, "        JL GE\n");
        fprintf(out, "        MOV [CX], 0001h\n");
        fprintf(out, "        GE : ret\n");
        fprintf(out, "relationge endp\n");
        fprintf(out, "\n");
      	fprintf(out, "relationgt proc near\n");
      	fprintf(out, "        MOV AX, DX\n");
      	fprintf(out, "        MOV [CX], 0000h");
       	fprintf(out, "        MOV DX, [BX]\n");
      	fprintf(out, "        MOV BX, DX\n");
       	fprintf(out, "        CMP AX, BX\n");
        fprintf(out, "        JLE GT\n");
        fprintf(out, "        MOV [CX], 0001h\n");
        fprintf(out, "        GT : ret\n");
        fprintf(out, "relationgt endp\n");
      	
      	
      	
      	fprintf(out, "main proc near\n");
	fprintf(out, "	\n");
	fprintf(out, "        MOV ax, seg CONSTINT\n"); //what bout this?
      	fprintf(out, "        MOV ds, ax\n");                   //what bout this?
      	fprintf(out, "        MOV ax, offset CONSTINT\n");
      	fprintf(out, "        MOV [ax], ooooh\n");
      	fprintf(out, "        MOV ax, 00h\n");
      	fprintf(out, "\n");
      	fprintf(out, "\n");
      	fprintf(out, ";generated code\n");
      	
      	printf("making code\n");//////////////////////////////////////////////////
      	fprintf(out, "\n");
	makeCode(stab, root, out);
	
      	fprintf(out, "\n");
      	fprintf(out, "generated code\n");
      	fprintf(out, "\n");
	fprintf(out, "main endp\n");
	fprintf(out, "end\n");
	
	fclose(out);
	return;
}
