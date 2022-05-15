%{
    #include<stdio.h>
    #include <string.h>
    #include "AST/AST.h"
    #include "parser.tab.h"
    int print_lex=1;
    #define DEBUG_PRINT
    #ifndef DEBUG_PRINT
    #define printf(...)  printf("")
    #endif
%}
%option yylineno
%option noyywrap
letter  [A-Za-z]
digit   [0-9]
INT     (0|([1-9]{digit}*))
FLOAT   {INT}"."{digit}+
id      [0-9_a-zA-Z]
idf     [_a-zA-Z]
ID      {idf}{id}*
blank   [ \t\v\n\f\r]

%%
 //忽略
"/*"([^\*]|(\*)*[^\*/])*(\*)*"*/" { if (print_lex==1 ) printf("注释\n");} 
{blank} {}

"==" {if (print_lex==1 ) printf("EQUAL:==\n"); return EQUAL;}  //关系表达式
"<=" {if (print_lex==1 ) printf("LE:<=\n"); return LE;}
">=" {if (print_lex==1 ) printf("GE:>=\n"); return GE;}
"!=" {if (print_lex==1 ) printf("NE:!=\n"); return NE;}
"<" {if (print_lex==1 ) printf("<\n"); return LESS;}
">" {if (print_lex==1 ) printf(">\n"); return GREATER;}

"=" {if (print_lex==1 ) printf("=\n"); return ASSIGN;}   //赋值表达式

"||" {if (print_lex==1 ) printf("||\n"); return OR;}    //逻辑表达式
"&&" {if (print_lex==1 ) printf("&&\n"); return AND;}
"!" {if (print_lex==1 ) printf("NOT\n"); return NOT;}

"+" {if (print_lex==1 ) printf("+\n"); return ADD;}   //四则运算表达式
"-" {if (print_lex==1 ) printf("-\n"); return MINUS;}
"*" {if (print_lex==1 ) printf("*\n"); return MULT;}
"/" {if (print_lex==1 ) printf("/\n"); return DIV;}

"\"" {if (print_lex==1 ) printf("引号\n"); return QUOT;}
"%" {if (print_lex==1 ) printf("百分号\n"); return PERCENT;}
"&" {if (print_lex==1 ) printf("&\n"); return LOGICAND;}
"\\n" {if (print_lex==1 ) printf("换行\n"); return LINEBREAK;}
"%d" {if (print_lex==1 ) printf("百分号d\n"); return PERCENTD;}
"%f" {if (print_lex==1 ) printf("输入浮点数\n"); return PERCENTF;}


";" {if (print_lex==1 ) printf(";\n"); return SEMI;}
"," {if (print_lex==1 ) printf(",\n"); return COMMA;}
"." {if (print_lex==1 ) printf(".\n"); return DOT;}
"(" {if (print_lex==1 ) printf("(\n"); return LP;}
")" {if (print_lex==1 ) printf(")\n"); return RP;}
"[" {if (print_lex==1 ) printf("[\n"); return LB;}
"]" {if (print_lex==1 ) printf("]\n"); return RB;}
"{" {if (print_lex==1 ) printf("左大括号\n"); return LC;}
"}" {if (print_lex==1 ) printf("}\n"); return RC;}

"if" {if (print_lex==1 ) printf("if\n"); return IF;}
"else" {if (print_lex==1 ) printf("else\n"); return ELSE;}
"while" {if (print_lex==1 ) printf("whlie\n"); return WHILE;}
"int" {if (print_lex==1 ) printf("type:整数\n"); return TYPEINT;}
"float" {if (print_lex==1 ) printf("type:浮点数\n"); return TYPEFLOAT;}
"void" {if (print_lex==1 ) printf("type:Void\n"); return TYPEVOID;}
"return" {if (print_lex==1 ) printf("return\n"); return RETURN;}
"break" {if (print_lex==1 ) printf("break\n"); return BREAK;}
"continue" {if (print_lex==1 ) printf("continue\n"); return CONTINUE;}
"function" {if (print_lex==1 ) printf("function\n"); return FUNCTION;}
"cin" {if (print_lex==1 ) printf("cin\n"); return INPUT;}
"cout" {if (print_lex==1 ) printf("cout\n"); return OUTPUT;}

\"(\\.|[^"\\])*\" {if (print_lex==1 ) printf("format string:%s\n",yytext); return FormatStr;}

{FLOAT} {
    if (print_lex==1 ) 
        printf("浮点数值:%s\n",yytext); 
    yylval.type_float = atof(yytext);
    return FLOAT;
}
{INT} {
    if (print_lex==1 ) 
        printf("整数值:%s\n",yytext); 
    yylval.type_int = atoi(yytext); 
    return INT;
}
{ID} {
    if (print_lex==1 ) 
        printf("ID:%s\n",yytext);     
    yylval.type_str=strdup(yytext);
    return ID;
}
{digit}{id}* {
    printf("Error ID:%s\n",yytext);
    printf("At line:%d\n",yylineno);
    yyterminate();
}
. {
    printf("Unknown Token:%s\n",yytext);
    printf("At line:%d\n",yylineno);
    yyterminate();
}
%%

