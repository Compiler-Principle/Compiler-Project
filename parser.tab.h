/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     EQUAL = 258,
     LE = 259,
     GE = 260,
     NE = 261,
     LESS = 262,
     GREATER = 263,
     ASSIGN = 264,
     OR = 265,
     AND = 266,
     NOT = 267,
     ADD = 268,
     MINUS = 269,
     MULT = 270,
     DIV = 271,
     QUOT = 272,
     PERCENT = 273,
     LOGICAND = 274,
     LINEBREAK = 275,
     PERCENTD = 276,
     PERCENTF = 277,
     SEMI = 278,
     COMMA = 279,
     DOT = 280,
     LP = 281,
     RP = 282,
     LB = 283,
     RB = 284,
     LC = 285,
     RC = 286,
     IF = 287,
     ELSE = 288,
     WHILE = 289,
     TYPEINT = 290,
     TYPEFLOAT = 291,
     TYPEVOID = 292,
     RETURN = 293,
     BREAK = 294,
     CONTINUE = 295,
     FUNCTION = 296,
     INPUT = 297,
     OUTPUT = 298,
     FormatStr = 299,
     FLOAT = 300,
     INT = 301,
     ID = 302
   };
#endif
/* Tokens.  */
#define EQUAL 258
#define LE 259
#define GE 260
#define NE 261
#define LESS 262
#define GREATER 263
#define ASSIGN 264
#define OR 265
#define AND 266
#define NOT 267
#define ADD 268
#define MINUS 269
#define MULT 270
#define DIV 271
#define QUOT 272
#define PERCENT 273
#define LOGICAND 274
#define LINEBREAK 275
#define PERCENTD 276
#define PERCENTF 277
#define SEMI 278
#define COMMA 279
#define DOT 280
#define LP 281
#define RP 282
#define LB 283
#define RB 284
#define LC 285
#define RC 286
#define IF 287
#define ELSE 288
#define WHILE 289
#define TYPEINT 290
#define TYPEFLOAT 291
#define TYPEVOID 292
#define RETURN 293
#define BREAK 294
#define CONTINUE 295
#define FUNCTION 296
#define INPUT 297
#define OUTPUT 298
#define FormatStr 299
#define FLOAT 300
#define INT 301
#define ID 302




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 6 "parser.y"
{
    int type_int;
    float type_float;
    char* type_str;
}
/* Line 1529 of yacc.c.  */
#line 149 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

