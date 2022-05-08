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
%token IF ELSE WHILE TYPEINT TYPEFLOAT TYPEVOID RETURN BREAK CONTINUE FUNCTION INPUT OUTPUT FormatStr
%token FLOAT INT ID

%right ASSIGN
%left NOT
%left OR
%left AND
%left EQUAL NE
%left LE GE LESS GREATER
%left ADD MINUS
%left MULT DIV
%left LOGICAND
%left LP RP LB RB DOT

%%
Program : Def_list Fun_list
        ;

Def_list : Def_list Var SEMI
         |
         ;

Var : Type VarDec
    ;

VarDec : ID
       | VarDec LB INT RB
       ;

Fun_list : Fun_list Fun
         |
         ;

Fun : FunDec CBlock
    ;

FunDec : FUNCTION Type ID LP Var_List RP
       | FUNCTION Type ID LP RP
       ;

Type : TYPEINT
     | TYPEFLOAT
     | TYPEVOID
     ;

Var_List : Var COMMA Var_List
         | Var
         ;

CBlock : LC Def_list Stmt_list RC
       ;

Stmt_list : Stmt Stmt_list
          |
          ;

Stmt : Exp SEMI
     | CBlock
     | RETURN SEMI
     | RETURN Exp SEMI
     | IF_Stmt
     | If_Else_Stmt
     | While_Stmt
     | Input_Exp SEMI
     | Output_Exp SEMI
     ;

IF_Stmt : IF LP Exp RP CBlock
        ;

If_Else_Stmt : IF LP Exp RP CBlock ELSE CBlock
             ;

While_Stmt : WHILE LP Exp RP CBlock
           ;

Input_Exp : INPUT LP FormatStr COMMA Args RP
          ;

Output_Exp : OUTPUT LP FormatStr COMMA Args RP
           | OUTPUT LP FormatStr RP
           ;

Exp : Exp ASSIGN Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp ADD Exp
    | Exp MINUS Exp
    | Exp MULT Exp
    | Exp DIV Exp
    | Exp EQUAL Exp
    | Exp LE Exp
    | Exp GE Exp
    | Exp NE Exp
    | Exp LESS Exp
    | Exp GREATER Exp
    | LOGICAND Exp
    | LP Exp RP
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID
    | INT
    | FLOAT
    ;

Args : Exp COMMA Args
     | Exp
     ;

%%
void yyerror(const char*s)
{
    printf("%s\n", s);
    printf("line %d:", yylineno);
    printf("text \"%s\" \n", yytext);
    exit(1);
}