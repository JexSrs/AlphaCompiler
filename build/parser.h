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

#ifndef YY_YY_BUILD_PARSER_H_INCLUDED
# define YY_YY_BUILD_PARSER_H_INCLUDED
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
    NUMBER = 258,                  /* NUMBER  */
    IF = 259,                      /* IF  */
    ELSE = 260,                    /* ELSE  */
    WHILE = 261,                   /* WHILE  */
    FOR = 262,                     /* FOR  */
    FUNCTION = 263,                /* FUNCTION  */
    RETURN = 264,                  /* RETURN  */
    BREAK = 265,                   /* BREAK  */
    CONTINUE = 266,                /* CONTINUE  */
    AND = 267,                     /* AND  */
    NOT = 268,                     /* NOT  */
    OR = 269,                      /* OR  */
    local = 270,                   /* local  */
    TRUE = 271,                    /* TRUE  */
    FALSE = 272,                   /* FALSE  */
    NIL = 273,                     /* NIL  */
    STRING = 274,                  /* STRING  */
    ID = 275,                      /* ID  */
    equal = 276,                   /* equal  */
    plus = 277,                    /* plus  */
    minus = 278,                   /* minus  */
    MUL = 279,                     /* MUL  */
    DIV = 280,                     /* DIV  */
    MOD = 281,                     /* MOD  */
    eequal = 282,                  /* eequal  */
    nequal = 283,                  /* nequal  */
    pplus = 284,                   /* pplus  */
    mminus = 285,                  /* mminus  */
    greater = 286,                 /* greater  */
    lesser = 287,                  /* lesser  */
    gequal = 288,                  /* gequal  */
    lequal = 289,                  /* lequal  */
    openBlock = 290,               /* openBlock  */
    closeBlock = 291,              /* closeBlock  */
    openBracket = 292,             /* openBracket  */
    closeBracket = 293,            /* closeBracket  */
    openPar = 294,                 /* openPar  */
    closePar = 295,                /* closePar  */
    semicolon = 296,               /* semicolon  */
    comma = 297,                   /* comma  */
    colon = 298,                   /* colon  */
    ccolon = 299,                  /* ccolon  */
    dot = 300,                     /* dot  */
    ddot = 301,                    /* ddot  */
    UMINUS = 302                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define NUMBER 258
#define IF 259
#define ELSE 260
#define WHILE 261
#define FOR 262
#define FUNCTION 263
#define RETURN 264
#define BREAK 265
#define CONTINUE 266
#define AND 267
#define NOT 268
#define OR 269
#define local 270
#define TRUE 271
#define FALSE 272
#define NIL 273
#define STRING 274
#define ID 275
#define equal 276
#define plus 277
#define minus 278
#define MUL 279
#define DIV 280
#define MOD 281
#define eequal 282
#define nequal 283
#define pplus 284
#define mminus 285
#define greater 286
#define lesser 287
#define gequal 288
#define lequal 289
#define openBlock 290
#define closeBlock 291
#define openBracket 292
#define closeBracket 293
#define openPar 294
#define closePar 295
#define semicolon 296
#define comma 297
#define colon 298
#define ccolon 299
#define dot 300
#define ddot 301
#define UMINUS 302

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 64 "Compiler/parser.y"

    int intVal;
    char* strValue;
    double numVal;

    struct SymbolTable *symbolTable;

    struct expr *exprValue;
    struct MethodCall *methodCall;
    struct ForLoop_S *forLoopVal;
    struct LoopStack *loopStack;

#line 174 "build/parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_BUILD_PARSER_H_INCLUDED  */
