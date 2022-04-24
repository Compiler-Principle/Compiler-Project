%{
    #include<stdio.h>
    #include "lex.yy.c"
    void yyerror(const char*);
%}
%union {
    int type_int;
    float type_float;
    char* type_str;
}
%token EQUAL LE GE NE LESS GREATER
%token ASSIGN
%token OR AND NOT
%token ADD MINUS MULT DIV
%token QUOT PERCENT LOGICAND LINEBREAK PERCENTD PERCENTF
%token SEMI COMMA DOT LP RP LB RB LC RC
%token IF ELSE WHILE TYPEINT TYPEFLOAT TYPEVOID RETURN BREAK CONTINUE FUNCTION INPUT OUTPUT
%token FLOAT INT ID

%%
CAL:INT
    ;

%%
void yyerror(const char*s)
{
    printf("%s\n", s);
    printf("line %d:", yylineno);
    printf("text \"%s\" \n", yytext);
    exit(1);
}