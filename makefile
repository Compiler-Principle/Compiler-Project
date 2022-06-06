parser : main.cpp parser.tab.c lex.yy.c gen/gen.cpp
	@g++ main.cpp parser.tab.c AST/AST.cpp info/info.cpp gen/gen.cpp -o parser `llvm-config --cflags --ldflags` `llvm-config --libs` `llvm-config --system-libs`

front : main.cpp parser.tab.c lex.yy.c
	@g++-11 main.cpp parser.tab.c AST/AST.cpp info/info.cpp -o parser
	@./parser test.txt

parser.tab.c : parser.y
	@bison -d parser.y -v --debug

lex.yy.c : scanner.l
	@flex scanner.l

clean : 
	@rm -f lex.yy.c parser.tab.c parser.tab.h parser parser.output main.bc main.ll a.out

run : parser test_aa.txt
	@./parser test_aa.txt
	@llvm-dis < main.bc
	@lli main.bc

test : parser test_aa.txt
	@./parser test_aa.txt
	@clang main.bc
	@./linux-amd64 ./a.out