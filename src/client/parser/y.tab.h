/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_Y_TAB_H_INCLUDED
# define YY_YY_SRC_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "src/parser.y"

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include "types.h"
    extern Query q;
	int yylex();
	void yyerror(const char *s);

#line 61 "src/y.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    LPAREN = 258,                  /* LPAREN  */
    RPAREN = 259,                  /* RPAREN  */
    LBRACKET = 260,                /* LBRACKET  */
    RBRACKET = 261,                /* RBRACKET  */
    PIPE = 262,                    /* PIPE  */
    SLASHSLASH = 263,              /* SLASHSLASH  */
    SLASH = 264,                   /* SLASH  */
    AT = 265,                      /* AT  */
    UPDATE = 266,                  /* UPDATE  */
    DELETE = 267,                  /* DELETE  */
    CREATE = 268,                  /* CREATE  */
    ASTERISK = 269,                /* ASTERISK  */
    EOL = 270,                     /* EOL  */
    INT_T = 271,                   /* INT_T  */
    BOOL_T = 272,                  /* BOOL_T  */
    WORD_T = 273,                  /* WORD_T  */
    DOUBLE_T = 274,                /* DOUBLE_T  */
    EQUALS_T = 275,                /* EQUALS_T  */
    NOT_EQUALS_T = 276,            /* NOT_EQUALS_T  */
    LESS_THAN_T = 277,             /* LESS_THAN_T  */
    GREATER_THAN_T = 278           /* GREATER_THAN_T  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define LPAREN 258
#define RPAREN 259
#define LBRACKET 260
#define RBRACKET 261
#define PIPE 262
#define SLASHSLASH 263
#define SLASH 264
#define AT 265
#define UPDATE 266
#define DELETE 267
#define CREATE 268
#define ASTERISK 269
#define EOL 270
#define INT_T 271
#define BOOL_T 272
#define WORD_T 273
#define DOUBLE_T 274
#define EQUALS_T 275
#define NOT_EQUALS_T 276
#define LESS_THAN_T 277
#define GREATER_THAN_T 278

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 13 "src/parser.y"

    int bool_value;
    int int_value;
    double double_value;
    char *string;

    FunctionType func_type;
    Filter *filter;
    Element *el;

#line 138 "src/y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SRC_Y_TAB_H_INCLUDED  */
