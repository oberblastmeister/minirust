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

#ifndef YY_YY_SRC_TOKEN_H_INCLUDED
# define YY_YY_SRC_TOKEN_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 15 "./src/parser.y"

#include "parser_state.h"
typedef void* yyscan_t;

#line 54 "./src/token.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOKEN_INT = 258,               /* TOKEN_INT  */
    TOKEN_FLOAT = 259,             /* TOKEN_FLOAT  */
    TOKEN_STRING = 260,            /* TOKEN_STRING  */
    TOKEN_LBRACE = 261,            /* TOKEN_LBRACE  */
    TOKEN_RBRACE = 262,            /* TOKEN_RBRACE  */
    TOKEN_LPAREN = 263,            /* TOKEN_LPAREN  */
    TOKEN_RPAREN = 264,            /* TOKEN_RPAREN  */
    TOKEN_COMMA = 265,             /* TOKEN_COMMA  */
    TOKEN_DOT = 266,               /* TOKEN_DOT  */
    TOKEN_PLUS = 267,              /* TOKEN_PLUS  */
    TOKEN_MINUS = 268,             /* TOKEN_MINUS  */
    TOKEN_STAR = 269,              /* TOKEN_STAR  */
    TOKEN_SLASH = 270,             /* TOKEN_SLASH  */
    TOKEN_BANG = 271,              /* TOKEN_BANG  */
    TOKEN_BANG_EQ = 272,           /* TOKEN_BANG_EQ  */
    TOKEN_EQ = 273,                /* TOKEN_EQ  */
    TOKEN_EQ_EQ = 274,             /* TOKEN_EQ_EQ  */
    TOKEN_GT = 275,                /* TOKEN_GT  */
    TOKEN_GT_EQ = 276,             /* TOKEN_GT_EQ  */
    TOKEN_LT = 277,                /* TOKEN_LT  */
    TOKEN_LT_EQ = 278,             /* TOKEN_LT_EQ  */
    TOKEN_IDENT = 279,             /* TOKEN_IDENT  */
    TOKEN_AND = 280,               /* TOKEN_AND  */
    TOKEN_OR = 281,                /* TOKEN_OR  */
    TOKEN_IF = 282,                /* TOKEN_IF  */
    TOKEN_ELSE = 283,              /* TOKEN_ELSE  */
    TOKEN_WHILE = 284,             /* TOKEN_WHILE  */
    TOKEN_LET = 285,               /* TOKEN_LET  */
    TOKEN_FALSE = 286,             /* TOKEN_FALSE  */
    TOKEN_TRUE = 287,              /* TOKEN_TRUE  */
    TOKEN_FUN = 288,               /* TOKEN_FUN  */
    TOKEN_FOR = 289,               /* TOKEN_FOR  */
    TOKEN_PRINT = 290,             /* TOKEN_PRINT  */
    TOKEN_RETURN = 291,            /* TOKEN_RETURN  */
    TOKEN_CLASS = 292,             /* TOKEN_CLASS  */
    TOKEN_SUPER = 293,             /* TOKEN_SUPER  */
    TOKEN_THIS = 294,              /* TOKEN_THIS  */
    TOKEN_ERROR = 295              /* TOKEN_ERROR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 24 "./src/parser.y"

	int int_value;
	double float_value;
	char *string_value;

#line 117 "./src/token.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (yyscan_t scanner, parser_state *parser_state);


#endif /* !YY_YY_SRC_TOKEN_H_INCLUDED  */
