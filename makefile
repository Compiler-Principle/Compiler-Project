parser : main.c parser.tab.c lex.yy.c
	@gcc main.c parser.tab.c -o parser

parser.tab.c : parser.y
	@bison -d parser.y

lex.yy.c : scanner.l
	@flex scanner.l

clean : 
	@rm lex.yy.c parser.tab.c parser.tab.h parser

run : parser
	@./parser test.txt