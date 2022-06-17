/* A Bison parser, made by GNU Bison 3.7.5.  */

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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    T_CHAR = 258,                  /* T_CHAR  */
    T_INT = 259,                   /* T_INT  */
    T_VOID = 260,                  /* T_VOID  */
    ADDR = 261,                    /* ADDR  */
    ASSIGN = 262,                  /* ASSIGN  */
    PLUSASSIGN = 263,              /* PLUSASSIGN  */
    MINUSASSIGN = 264,             /* MINUSASSIGN  */
    MULASSIGN = 265,               /* MULASSIGN  */
    DIVASSIGN = 266,               /* DIVASSIGN  */
    SEMICOLON = 267,               /* SEMICOLON  */
    COMMA = 268,                   /* COMMA  */
    LPAREN = 269,                  /* LPAREN  */
    RPAREN = 270,                  /* RPAREN  */
    LBRACE = 271,                  /* LBRACE  */
    RBRACE = 272,                  /* RBRACE  */
    LBRACKET = 273,                /* LBRACKET  */
    RBRACKET = 274,                /* RBRACKET  */
    CONST = 275,                   /* CONST  */
    IF_ = 276,                     /* IF_  */
    ELSE = 277,                    /* ELSE  */
    WHILE_ = 278,                  /* WHILE_  */
    BREAK = 279,                   /* BREAK  */
    CONTINUE = 280,                /* CONTINUE  */
    RETURN = 281,                  /* RETURN  */
    EQ = 282,                      /* EQ  */
    GRAEQ = 283,                   /* GRAEQ  */
    LESEQ = 284,                   /* LESEQ  */
    NEQ = 285,                     /* NEQ  */
    GRA = 286,                     /* GRA  */
    LES = 287,                     /* LES  */
    PLUS = 288,                    /* PLUS  */
    MINUS = 289,                   /* MINUS  */
    MUL = 290,                     /* MUL  */
    DIV = 291,                     /* DIV  */
    MOD = 292,                     /* MOD  */
    AND = 293,                     /* AND  */
    OR = 294,                      /* OR  */
    NOT = 295,                     /* NOT  */
    INC = 296,                     /* INC  */
    DEC = 297,                     /* DEC  */
    IDENTIFIER = 298,              /* IDENTIFIER  */
    INTEGER = 299                  /* INTEGER  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
