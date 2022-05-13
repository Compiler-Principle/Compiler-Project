parser : main.cpp parser.tab.c lex.yy.c
	@g++-11 main.cpp parser.tab.c AST/AST.cpp info/info.cpp -o parser

parser.tab.c : parser.y
	@bison -d parser.y -v --debug

lex.yy.c : scanner.l
	@flex scanner.l

clean : 
	@rm lex.yy.c parser.tab.c parser.tab.h parser parser.output

run : parser
	@./parser test.txt