%{
    #include<stdio.h>
    #include "lex.yy.c"
    #include "AST/AST.h"
    void yyerror(const char*);
    extern baseAST *head;
    baseAST* temp;
%}
%union {
    int type_int;
    float type_float;
    char* type_str;
    int type_token;
    baseAST *type_ast;
}

%token<type_int> INT
%token<type_float> FLOAT
%token<type_str> ID FormatStr
%token<type_token> EQUAL LE GE NE LESS GREATER
%token<type_token> ASSIGN
%token<type_token> OR AND NOT
%token<type_token> ADD MINUS MULT DIV
%token<type_token> QUOT PERCENT LOGICAND LINEBREAK PERCENTD PERCENTF
%token<type_token> SEMI COMMA DOT LP RP LB RB LC RC
%token<type_token> IF ELSE WHILE TYPEINT TYPEFLOAT TYPEVOID RETURN BREAK CONTINUE FUNCTION INPUT OUTPUT STRUCT

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

%type <type_ast>    Program Def_List Fun_List Var Var_List Fun Fun_Var_List Fun_Var VarDec
%type <type_ast>    AST_Type CBlock Stmt_List Stmt IF_Stmt If_Else_Stmt While_Stmt Input_Exp Output_Exp Exp Args

%%
Program : Def_List Fun_List {
    $$ = new baseAST(AST_Type::T_root, "Program");
    head = $$;
    head->Insert($1);
    head->Insert($2);
}
        ;

Def_List : Def_List Var SEMI{$$=$1;$$->Insert($2);}
         |{$$=new baseAST(AST_Type::T_list,"Def_List");}
         ;

Var : AST_Type Var_List{$$=new baseAST(AST_Type::T_defi,"Def");$$->Insert($1);$$->Insert($2);}
    |STRUCT LC Def_List RC ID{$$=new baseAST(AST_Type::T_struct,$5);delete $5;$$->Insert($3);}
    ;

Var_List : Var_List COMMA VarDec {$$=$1;$$->Insert($3);}
         | VarDec {$$=new baseAST(AST_Type::T_list, "Var_List"); $$->Insert($1);}
         ;



Fun_List : Fun_List Fun{$$=$1;$$->Insert($2);}
         |{$$=new baseAST(AST_Type::T_list,"Fun_List");}
         ;

Fun : FUNCTION AST_Type ID LP Fun_Var_List RP CBlock{$$=new baseAST(AST_Type::T_func,$3);delete $3;$$->Insert($2);$$->Insert($5);$$->Insert($7);}
    |FUNCTION AST_Type ID LP RP CBlock{$$=new baseAST(AST_Type::T_func,$3);delete $3;$$->Insert($2);$$->Insert($6);}
    ;


Fun_Var_List: Fun_Var_List COMMA Fun_Var{$$=$1;$$->Insert($3);}
        |Fun_Var{$$=new baseAST(AST_Type::T_list,"Fun_Var_List");$$->Insert($1);}
        ;

Fun_Var:AST_Type VarDec{$$=new baseAST(AST_Type::T_fvar,"Fun_Var");$$->Insert($1);$$->Insert($2);}
        ;

VarDec : ID{$$=new baseAST(AST_Type::T_var,$1);delete $1;}
       | VarDec LB INT RB {$$=$1;$$->Insert(new constNode($3,AST_DataType::DT_integer));}
       ;

AST_Type : TYPEINT{$$=new baseAST(AST_Type::T_tydf,"int");$$->dataType=DT_integer;}
     | TYPEFLOAT{$$=new baseAST(AST_Type::T_tydf,"float");$$->dataType=DT_float;}
     | TYPEVOID{$$=new baseAST(AST_Type::T_tydf,"void");$$->dataType=DT_void;}
     ;


CBlock : LC Def_List Stmt_List RC{$$=new baseAST(AST_Type::T_block,"CBlock");$$->Insert($2);$$->Insert($3);}
       ;

Stmt_List : Stmt_List Stmt{$$=$1;$$->Insert($2);}
          |Stmt{$$=new baseAST(AST_Type::T_list,"Stmt_List");$$->Insert($1);}
          ;

Stmt : Exp SEMI{$$=$1;}
     | CBlock{$$=$1;}
     | RETURN SEMI{$$=new baseAST(AST_Type::T_expr,"return");}
     | RETURN Exp SEMI{$$=new baseAST(AST_Type::T_expr,"return");$$->Insert($2);}
     | IF_Stmt{$$=$1;}
     | If_Else_Stmt{$$=$1;}
     | While_Stmt{$$=$1;}
     | Input_Exp SEMI {$$=$1;}
     | Output_Exp SEMI {$$=$1;}
     | BREAK SEMI{$$=new baseAST(AST_Type::T_expr,"break");}
     | CONTINUE SEMI{$$=new baseAST(AST_Type::T_expr,"continue");}
     ;

IF_Stmt : IF LP Exp RP CBlock{$$=new baseAST(AST_Type::T_expr,"IF_Stmt");$$->Insert($3);$$->Insert($5);}
        ;

If_Else_Stmt : IF LP Exp RP CBlock ELSE CBlock {$$=new baseAST(AST_Type::T_expr,"If_Else_Stmt");$$->Insert($3);$$->Insert($5);$$->Insert($7);}
             ;

While_Stmt : WHILE LP Exp RP CBlock{$$=new baseAST(AST_Type::T_expr,"While_Stmt");$$->Insert($3);$$->Insert($5);}
           ;

Input_Exp : INPUT LP FormatStr COMMA Args RP{$$=new baseAST(AST_Type::T_expr,"Input_Exp");temp=new baseAST(AST_Type::T_formatstr,$3);$$->Insert(temp);$$->Insert($5);}
          ;

Output_Exp : OUTPUT LP FormatStr COMMA Args RP {$$=new baseAST(AST_Type::T_expr,"Output_Exp");temp=new baseAST(AST_Type::T_formatstr,$3);$$->Insert(temp);$$->Insert($5);}
           | OUTPUT LP FormatStr RP {$$=new baseAST(AST_Type::T_expr,"Output_Exp");temp=new baseAST(AST_Type::T_formatstr,$3);$$->Insert(temp);}
           ;

Exp : Exp ASSIGN Exp{$$=new baseAST(AST_Type::T_expr,"ASSIGN");$$->Insert($1);$$->Insert($3);}
    | Exp AND Exp{$$=new operatorNode(AST_Operator::O_AND,"AND");$$->Insert($1);$$->Insert($3);}
    | Exp OR Exp{$$=new operatorNode(AST_Operator::O_OR,"OR");$$->Insert($1);$$->Insert($3);}
    | Exp ADD Exp{$$=new operatorNode(AST_Operator::O_ADD,"ADD");$$->Insert($1);$$->Insert($3);}
    | Exp MINUS Exp{$$=new operatorNode(AST_Operator::O_MINUS,"MINUS");$$->Insert($1);$$->Insert($3);}
    | Exp MULT Exp{$$=new operatorNode(AST_Operator::O_MULT,"MULT");$$->Insert($1);$$->Insert($3);}
    | Exp DIV Exp{$$=new operatorNode(AST_Operator::O_DIV,"DIV");$$->Insert($1);$$->Insert($3);}
    | Exp EQUAL Exp{$$=new operatorNode(AST_Operator::O_EQUAL,"EQUAL");$$->Insert($1);$$->Insert($3);}
    | Exp LE Exp{$$=new operatorNode(AST_Operator::O_LE,"LE");$$->Insert($1);$$->Insert($3);}
    | Exp GE Exp{$$=new operatorNode(AST_Operator::O_GE,"GE");$$->Insert($1);$$->Insert($3);}
    | Exp NE Exp{$$=new operatorNode(AST_Operator::O_NE,"NE");$$->Insert($1);$$->Insert($3);}
    | Exp LESS Exp{$$=new operatorNode(AST_Operator::O_LESS,"LESS");$$->Insert($1);$$->Insert($3);}
    | Exp GREATER Exp{$$=new operatorNode(AST_Operator::O_GREATER,"GREATER");$$->Insert($1);$$->Insert($3);}
    | LOGICAND Exp{$$=new operatorNode(AST_Operator::O_LOGICAND,"LOGICAND");$$->Insert($2);}
    | LP Exp RP{$$=$2;}
    | MINUS Exp{$$=new operatorNode(AST_Operator::O_MINUSDIGIT,"MINUSDIGIT");$$->Insert($2);}
    | NOT Exp{$$=new operatorNode(AST_Operator::O_NOT,"NOT");$$->Insert($2);}
    | ID LP Args RP{$$=new baseAST(AST_Type::T_expr,$1);delete $1;$$->Insert($3);}
    | ID LP RP{$$=new baseAST(AST_Type::T_expr,$1);delete $1;}
    | Exp LB Exp RB{$$=$1;$$->Insert($3);}
    | Exp DOT Exp{$$=$1;$$->Insert($3);}
    | ID{$$=new baseAST(AST_Type::T_var,$1);delete $1;}
    | INT{$$=new constNode($1,AST_DataType::DT_integer);$$->name="constint";}
    | FLOAT{$$=new constNode($1,AST_DataType::DT_float);$$->name="constfloat";}
    ;

Args : Args COMMA Exp{$$=$1;$$->Insert($3);}
     | Exp{$$=new baseAST(AST_Type::T_list,"Args");$$->Insert($1);}
     ;

%%
void yyerror(const char*s)
{
    printf("%s\n", s);
    printf("line %d:", yylineno);
    printf("text \"%s\" \n", yytext);
    exit(1);
}