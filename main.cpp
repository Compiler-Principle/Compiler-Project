#include<stdio.h>
#include "AST/AST.h"
// #include "gen/gen.h"
baseAST *head;
std::map<std::string, Var *> globalVars;
std::map<std::string, Func *> globalFuncs;
extern FILE* yyin;
extern int yyparse ();
int main(int argc,char** argv){
    if(argc<=1) return 1;
    FILE* f=fopen(argv[1],"r");
    if(!f){
        perror(argv[1]);
        return 1;
    }
    yyin=f;
    yyparse();
    head->print();
    head->buildTable(nullptr);
    printTable();
    head->scanTree(nullptr);
    checkVars();

    // InitIOFunc();
    // genCode(head);
    // genBC();
    return 0;
}