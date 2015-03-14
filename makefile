all: mycompiler

mycompiler: driver.o lexer.o parser.o ast.o symbolTable.o codegen.o typeExtractor.o
	gcc driver.o lexer.o parser.o ast.o symbolTable.o typeExtractor.o codegen.o -o mycompiler

driver.o: driver.c
	gcc -c driver.c
	    
codegen.o: codegen.c
	gcc -c codegen.c

typeExtractor.o: typeExtractor.c
	gcc -c typeExtractor.c
	
symbolTable.o: symbolTable.c
	gcc -c symbolTable.c
	
ast.o: ast.c
	gcc -c ast.c

lexer.o: lexer.c
	gcc -c lexer.c

parser.o: parser.c
	gcc -c parser.c
