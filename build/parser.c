/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "Compiler/parser.y"

#include "../Compiler/bison/bison.h"
#include "../Compiler/targetCode/targetCode.h"

#include "stdio.h"
#include "stdlib.h"
#include <math.h>

int yylex(void);
int yyerror(char* yaccProvidedMessage);

extern FILE* yyin;
extern FILE* yyout;
extern char* yytext;
extern int yylineno;

enum ScopeAction { INCREASE , DECREASE };

char* opcodeToString[] = {
	"assign",         "add",            "sub",
	"mul",            "div",            "mod",
	"jump",           "if_eq",          "if_noteq",
	"if_lesseq",      "if_greatereq",   "if_less",
	"if_greater",     "call",           "param",
	"funcstart",      "funcend",        "tablecreate",
	"tablegetelem",   "tablesetelem",   "getretval",
	"no_op",          "ret"
};

int getCurrentLine();
void print(char *message);

char *genName();
void scopeOps(enum ScopeAction action);

char* getStringValueQuad(struct expr* e, int isResult);
void printAllQuads();

int funcdefStart = 0;
int scope = 0;

int noNameFunctionNum = 0;
int activeFunctions = 0;
int activeLoops = 0;


int globalRuleManage = 0;
int localRuleManage = 0;

int globalBeforeEqual = 0;
int localBeforeEqual = 0;

int memberRuleManage = 0;
int memberManageRule = 0;

int memberBeforeEqual = 0;
int equalFound = 0;

int accessIsValid = 0;
int notRulePassed = 0;
int totalFuncArgs = 0;

#line 134 "build/parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER = 3,                     /* NUMBER  */
  YYSYMBOL_IF = 4,                         /* IF  */
  YYSYMBOL_ELSE = 5,                       /* ELSE  */
  YYSYMBOL_WHILE = 6,                      /* WHILE  */
  YYSYMBOL_FOR = 7,                        /* FOR  */
  YYSYMBOL_FUNCTION = 8,                   /* FUNCTION  */
  YYSYMBOL_RETURN = 9,                     /* RETURN  */
  YYSYMBOL_BREAK = 10,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 11,                  /* CONTINUE  */
  YYSYMBOL_AND = 12,                       /* AND  */
  YYSYMBOL_NOT = 13,                       /* NOT  */
  YYSYMBOL_OR = 14,                        /* OR  */
  YYSYMBOL_local = 15,                     /* local  */
  YYSYMBOL_TRUE = 16,                      /* TRUE  */
  YYSYMBOL_FALSE = 17,                     /* FALSE  */
  YYSYMBOL_NIL = 18,                       /* NIL  */
  YYSYMBOL_STRING = 19,                    /* STRING  */
  YYSYMBOL_ID = 20,                        /* ID  */
  YYSYMBOL_equal = 21,                     /* equal  */
  YYSYMBOL_plus = 22,                      /* plus  */
  YYSYMBOL_minus = 23,                     /* minus  */
  YYSYMBOL_MUL = 24,                       /* MUL  */
  YYSYMBOL_DIV = 25,                       /* DIV  */
  YYSYMBOL_MOD = 26,                       /* MOD  */
  YYSYMBOL_eequal = 27,                    /* eequal  */
  YYSYMBOL_nequal = 28,                    /* nequal  */
  YYSYMBOL_pplus = 29,                     /* pplus  */
  YYSYMBOL_mminus = 30,                    /* mminus  */
  YYSYMBOL_greater = 31,                   /* greater  */
  YYSYMBOL_lesser = 32,                    /* lesser  */
  YYSYMBOL_gequal = 33,                    /* gequal  */
  YYSYMBOL_lequal = 34,                    /* lequal  */
  YYSYMBOL_openBlock = 35,                 /* openBlock  */
  YYSYMBOL_closeBlock = 36,                /* closeBlock  */
  YYSYMBOL_openBracket = 37,               /* openBracket  */
  YYSYMBOL_closeBracket = 38,              /* closeBracket  */
  YYSYMBOL_openPar = 39,                   /* openPar  */
  YYSYMBOL_closePar = 40,                  /* closePar  */
  YYSYMBOL_semicolon = 41,                 /* semicolon  */
  YYSYMBOL_comma = 42,                     /* comma  */
  YYSYMBOL_colon = 43,                     /* colon  */
  YYSYMBOL_ccolon = 44,                    /* ccolon  */
  YYSYMBOL_dot = 45,                       /* dot  */
  YYSYMBOL_ddot = 46,                      /* ddot  */
  YYSYMBOL_UMINUS = 47,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 48,                  /* $accept  */
  YYSYMBOL_program = 49,                   /* program  */
  YYSYMBOL_stmtlist = 50,                  /* stmtlist  */
  YYSYMBOL_stmt = 51,                      /* stmt  */
  YYSYMBOL_expr = 52,                      /* expr  */
  YYSYMBOL_53_1 = 53,                      /* $@1  */
  YYSYMBOL_54_2 = 54,                      /* $@2  */
  YYSYMBOL_55_3 = 55,                      /* $@3  */
  YYSYMBOL_56_4 = 56,                      /* $@4  */
  YYSYMBOL_term = 57,                      /* term  */
  YYSYMBOL_assignexpr = 58,                /* assignexpr  */
  YYSYMBOL_59_5 = 59,                      /* $@5  */
  YYSYMBOL_primary = 60,                   /* primary  */
  YYSYMBOL_lvalue = 61,                    /* lvalue  */
  YYSYMBOL_member = 62,                    /* member  */
  YYSYMBOL_call = 63,                      /* call  */
  YYSYMBOL_callsuffix = 64,                /* callsuffix  */
  YYSYMBOL_normcall = 65,                  /* normcall  */
  YYSYMBOL_methodcall = 66,                /* methodcall  */
  YYSYMBOL_elist = 67,                     /* elist  */
  YYSYMBOL_objectdef = 68,                 /* objectdef  */
  YYSYMBOL_indexed = 69,                   /* indexed  */
  YYSYMBOL_indexedelem = 70,               /* indexedelem  */
  YYSYMBOL_block = 71,                     /* block  */
  YYSYMBOL_72_6 = 72,                      /* $@6  */
  YYSYMBOL_funcArgs = 73,                  /* funcArgs  */
  YYSYMBOL_funcName = 74,                  /* funcName  */
  YYSYMBOL_funcPre = 75,                   /* funcPre  */
  YYSYMBOL_funcPreArgs = 76,               /* funcPreArgs  */
  YYSYMBOL_funcBlockStart = 77,            /* funcBlockStart  */
  YYSYMBOL_funcBlockEnd = 78,              /* funcBlockEnd  */
  YYSYMBOL_funcBody = 79,                  /* funcBody  */
  YYSYMBOL_funcdef = 80,                   /* funcdef  */
  YYSYMBOL_const = 81,                     /* const  */
  YYSYMBOL_idlist = 82,                    /* idlist  */
  YYSYMBOL_ifPrefix = 83,                  /* ifPrefix  */
  YYSYMBOL_elsePrefix = 84,                /* elsePrefix  */
  YYSYMBOL_ifstmt = 85,                    /* ifstmt  */
  YYSYMBOL_loopStart = 86,                 /* loopStart  */
  YYSYMBOL_loopEnd = 87,                   /* loopEnd  */
  YYSYMBOL_loopstmt = 88,                  /* loopstmt  */
  YYSYMBOL_whileStart = 89,                /* whileStart  */
  YYSYMBOL_whileCond = 90,                 /* whileCond  */
  YYSYMBOL_whilestmt = 91,                 /* whilestmt  */
  YYSYMBOL_N = 92,                         /* N  */
  YYSYMBOL_M = 93,                         /* M  */
  YYSYMBOL_forPrefix = 94,                 /* forPrefix  */
  YYSYMBOL_forstmt = 95,                   /* forstmt  */
  YYSYMBOL_returnstmt = 96                 /* returnstmt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   578

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  190

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   302


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   112,   112,   115,   116,   119,   137,   145,   153,   161,
     169,   184,   199,   207,   215,   224,   229,   235,   241,   247,
     253,   262,   272,   282,   292,   302,   302,   324,   324,   346,
     346,   369,   369,   392,   399,   404,   415,   436,   465,   494,
     523,   552,   559,   559,   619,   652,   653,   658,   664,   671,
     720,   778,   794,   803,   812,   835,   836,   839,   844,   872,
     883,   888,   895,   909,   923,   942,   967,   978,   996,  1016,
    1020,  1034,  1056,  1056,  1066,  1074,  1086,  1096,  1119,  1129,
    1139,  1146,  1153,  1174,  1179,  1187,  1192,  1197,  1204,  1215,
    1227,  1232,  1253,  1263,  1271,  1285,  1295,  1305,  1321,  1328,
    1350,  1376,  1384,  1390,  1414,  1440,  1453
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER", "IF", "ELSE",
  "WHILE", "FOR", "FUNCTION", "RETURN", "BREAK", "CONTINUE", "AND", "NOT",
  "OR", "local", "TRUE", "FALSE", "NIL", "STRING", "ID", "equal", "plus",
  "minus", "MUL", "DIV", "MOD", "eequal", "nequal", "pplus", "mminus",
  "greater", "lesser", "gequal", "lequal", "openBlock", "closeBlock",
  "openBracket", "closeBracket", "openPar", "closePar", "semicolon",
  "comma", "colon", "ccolon", "dot", "ddot", "UMINUS", "$accept",
  "program", "stmtlist", "stmt", "expr", "$@1", "$@2", "$@3", "$@4",
  "term", "assignexpr", "$@5", "primary", "lvalue", "member", "call",
  "callsuffix", "normcall", "methodcall", "elist", "objectdef", "indexed",
  "indexedelem", "block", "$@6", "funcArgs", "funcName", "funcPre",
  "funcPreArgs", "funcBlockStart", "funcBlockEnd", "funcBody", "funcdef",
  "const", "idlist", "ifPrefix", "elsePrefix", "ifstmt", "loopStart",
  "loopEnd", "loopstmt", "whileStart", "whileCond", "whilestmt", "N", "M",
  "forPrefix", "forstmt", "returnstmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302
};
#endif

#define YYPACT_NINF (-170)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -170,     6,   197,  -170,  -170,   -23,  -170,    -6,    -8,    11,
      10,    27,   287,    44,  -170,  -170,  -170,  -170,  -170,   287,
      -7,    -7,  -170,   227,   257,  -170,    51,  -170,   311,  -170,
    -170,  -170,    24,  -170,    47,  -170,  -170,     8,  -170,  -170,
    -170,   197,    78,    38,  -170,  -170,  -170,  -170,   287,   287,
    -170,  -170,  -170,   334,  -170,  -170,  -170,  -170,  -170,    70,
      21,    47,    21,  -170,   287,   518,   -33,   -27,  -170,   380,
      53,  -170,  -170,  -170,   287,   287,   287,   287,   287,  -170,
    -170,   287,   287,   287,   287,  -170,  -170,  -170,  -170,   287,
     287,    91,    95,  -170,  -170,  -170,   287,   287,   100,   103,
    -170,    92,  -170,  -170,   197,   287,  -170,   287,   403,    86,
    -170,    89,   158,   110,  -170,   287,  -170,   104,  -170,   101,
    -170,  -170,    -5,    -5,  -170,  -170,  -170,   287,   287,   296,
     296,   296,   296,   287,   449,   -17,  -170,   107,   472,    48,
    -170,  -170,    49,  -170,  -170,  -170,   426,   197,  -170,    72,
    -170,   106,   101,  -170,   287,   518,  -170,   287,   287,   287,
     531,   531,   518,  -170,  -170,   287,  -170,  -170,  -170,   128,
    -170,  -170,  -170,  -170,   287,   495,    76,   544,    73,    79,
    -170,  -170,  -170,   357,  -170,  -170,  -170,  -170,  -170,  -170
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     2,     1,    83,     0,    98,     0,    76,     0,
       0,     0,     0,     0,    86,    87,    85,    84,    49,     0,
       0,     0,    72,    66,     0,    14,     0,     3,     0,    33,
      15,    41,    44,    52,    45,    46,    12,     0,    79,    13,
      48,     0,     6,     0,     7,   101,     8,     9,     0,    66,
      75,    77,   105,     0,    10,    11,    36,    50,    35,     0,
      37,     0,    39,     4,     0,    64,     0,     0,    69,     0,
       0,    51,    29,    31,     0,     0,     0,     0,     0,    25,
      27,     0,     0,     0,     0,     5,    42,    38,    40,     0,
      66,     0,     0,    58,    60,    61,     0,    66,     0,    90,
      78,     0,    93,    92,     0,     0,    95,    66,     0,   102,
     106,     0,     0,     0,    67,     0,    68,     0,    34,    47,
     102,   102,    16,    17,    18,    19,    20,     0,     0,    21,
      23,    22,    24,     0,     0,     0,    53,     0,     0,     0,
      55,    88,     0,    81,    80,    94,     0,     0,   100,     0,
      91,     0,     0,    73,     0,    65,    70,    66,     0,     0,
      26,    28,    43,    54,    62,    66,    56,    57,    74,     0,
      82,    99,    96,   101,     0,     0,     0,    30,    32,     0,
      89,    97,    95,     0,    71,    59,    63,   101,   103,   104
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -170,  -170,    67,   -39,    -2,  -170,  -170,  -170,  -170,  -170,
    -170,  -170,  -170,    15,  -170,    23,  -170,  -170,  -170,   -48,
    -170,  -170,    32,    50,  -170,  -170,  -170,  -170,  -170,  -170,
    -170,  -170,   -21,  -170,  -170,  -170,  -170,  -170,  -170,  -170,
     -32,  -170,  -170,  -170,  -169,   -64,  -170,  -170,  -170
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    27,    65,   127,   128,   120,   121,    29,
      30,   133,    31,    32,    33,    34,    93,    94,    95,    66,
      35,    67,    68,    36,    63,   100,    51,    37,    38,   101,
     170,   144,    39,    40,   142,    41,   104,    42,   147,   181,
     148,    43,   106,    44,   107,   151,    45,    46,    47
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,   109,   102,    70,   182,   114,     3,    53,    13,   115,
      56,   116,    50,    18,     4,   117,    48,    58,   189,    76,
      77,    78,    69,   164,    12,   115,    13,    14,    15,    16,
      17,    18,    59,    49,    19,    60,    62,    26,   111,    28,
      20,    21,   135,    61,    61,    86,   108,    99,    23,   139,
      24,    54,    52,    87,    88,    26,   158,   159,    89,   149,
      90,    89,   113,    90,    57,   145,    91,    92,    55,    91,
      92,    71,   122,   123,   124,   125,   126,   105,     8,   129,
     130,   131,   132,   103,    96,    72,    97,   134,   167,   168,
     115,   169,    98,   119,   138,    74,    75,    76,    77,    78,
      79,    80,    28,   146,    81,    82,    83,    84,   172,   176,
      28,   136,   173,   155,   115,   137,   185,   179,   115,   186,
     140,   115,    72,   141,    73,   160,   161,    22,   115,   152,
     112,   162,    74,    75,    76,    77,    78,    79,    80,    64,
     157,    81,    82,    83,    84,    28,   165,   174,   180,   156,
     187,   143,   175,   154,     0,     0,   177,   178,     0,     0,
       0,     4,     5,     0,     6,     7,     8,     9,    10,    11,
       0,    12,   183,    13,    14,    15,    16,    17,    18,     0,
       0,    19,     0,     0,     0,     0,     0,    20,    21,     0,
       0,     0,     0,    22,   153,    23,     0,    24,     0,    25,
       4,     5,    26,     6,     7,     8,     9,    10,    11,     0,
      12,     0,    13,    14,    15,    16,    17,    18,     0,     0,
      19,     0,     0,     0,     0,     0,    20,    21,     0,     0,
       4,     0,    22,     0,    23,     0,    24,     0,    25,     0,
      12,    26,    13,    14,    15,    16,    17,    18,     0,     0,
      19,     0,     0,     0,     0,     0,    20,    21,     0,     0,
       4,     0,    64,     0,    23,     8,    24,     0,     0,     0,
      12,    26,    13,    14,    15,    16,    17,    18,     0,     0,
      19,     0,     0,     0,     0,     0,    20,    21,     0,     0,
       4,     0,     0,     0,    23,     0,    24,     0,     0,     0,
      12,    26,    13,    14,    15,    16,    17,    18,     0,     0,
      19,     0,     0,     0,     0,     0,    20,    21,    74,    75,
      76,    77,    78,    72,    23,    73,    24,    -1,    -1,    -1,
      -1,    26,     0,    74,    75,    76,    77,    78,    79,    80,
       0,     0,    81,    82,    83,    84,    72,     0,    73,     0,
       0,     0,    85,     0,     0,     0,    74,    75,    76,    77,
      78,    79,    80,     0,     0,    81,    82,    83,    84,    72,
       0,    73,     0,     0,     0,   110,     0,     0,     0,    74,
      75,    76,    77,    78,    79,    80,     0,     0,    81,    82,
      83,    84,    72,     0,    73,     0,     0,     0,   188,     0,
       0,     0,    74,    75,    76,    77,    78,    79,    80,     0,
       0,    81,    82,    83,    84,    72,     0,    73,     0,     0,
     118,     0,     0,     0,     0,    74,    75,    76,    77,    78,
      79,    80,     0,     0,    81,    82,    83,    84,    72,     0,
      73,     0,     0,   150,     0,     0,     0,     0,    74,    75,
      76,    77,    78,    79,    80,     0,     0,    81,    82,    83,
      84,    72,     0,    73,     0,     0,   171,     0,     0,     0,
       0,    74,    75,    76,    77,    78,    79,    80,     0,     0,
      81,    82,    83,    84,    72,     0,    73,   163,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,    79,
      80,     0,     0,    81,    82,    83,    84,    72,     0,    73,
     166,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,    79,    80,     0,     0,    81,    82,    83,    84,
      72,   184,    73,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,    79,    80,     0,     0,    81,
      82,    83,    84,    74,    75,    76,    77,    78,    -1,    -1,
       0,     0,    81,    82,    83,    84,    74,    75,    76,    77,
      78,    79,    80,     0,     0,    81,    82,    83,    84
};

static const yytype_int16 yycheck[] =
{
       2,    49,    41,    24,   173,    38,     0,     9,    15,    42,
      12,    38,    20,    20,     3,    42,    39,    19,   187,    24,
      25,    26,    24,    40,    13,    42,    15,    16,    17,    18,
      19,    20,    39,    39,    23,    20,    21,    44,    59,    41,
      29,    30,    90,    20,    21,    21,    48,    39,    37,    97,
      39,    41,    41,    29,    30,    44,   120,   121,    37,   107,
      39,    37,    64,    39,    20,   104,    45,    46,    41,    45,
      46,    20,    74,    75,    76,    77,    78,    39,     8,    81,
      82,    83,    84,     5,    37,    12,    39,    89,    40,    40,
      42,    42,    45,    40,    96,    22,    23,    24,    25,    26,
      27,    28,   104,   105,    31,    32,    33,    34,   147,   157,
     112,    20,    40,   115,    42,    20,    40,   165,    42,    40,
      20,    42,    12,    20,    14,   127,   128,    35,    42,    40,
      63,   133,    22,    23,    24,    25,    26,    27,    28,    35,
      39,    31,    32,    33,    34,   147,    39,    41,    20,   117,
     182,   101,   154,    43,    -1,    -1,   158,   159,    -1,    -1,
      -1,     3,     4,    -1,     6,     7,     8,     9,    10,    11,
      -1,    13,   174,    15,    16,    17,    18,    19,    20,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    29,    30,    -1,
      -1,    -1,    -1,    35,    36,    37,    -1,    39,    -1,    41,
       3,     4,    44,     6,     7,     8,     9,    10,    11,    -1,
      13,    -1,    15,    16,    17,    18,    19,    20,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,
       3,    -1,    35,    -1,    37,    -1,    39,    -1,    41,    -1,
      13,    44,    15,    16,    17,    18,    19,    20,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,
       3,    -1,    35,    -1,    37,     8,    39,    -1,    -1,    -1,
      13,    44,    15,    16,    17,    18,    19,    20,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,
       3,    -1,    -1,    -1,    37,    -1,    39,    -1,    -1,    -1,
      13,    44,    15,    16,    17,    18,    19,    20,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    29,    30,    22,    23,
      24,    25,    26,    12,    37,    14,    39,    31,    32,    33,
      34,    44,    -1,    22,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    12,    -1,    14,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    22,    23,    24,    25,
      26,    27,    28,    -1,    -1,    31,    32,    33,    34,    12,
      -1,    14,    -1,    -1,    -1,    41,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    -1,    -1,    31,    32,
      33,    34,    12,    -1,    14,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    -1,
      -1,    31,    32,    33,    34,    12,    -1,    14,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    -1,    -1,    31,    32,    33,    34,    12,    -1,
      14,    -1,    -1,    40,    -1,    -1,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    -1,    -1,    31,    32,    33,
      34,    12,    -1,    14,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    22,    23,    24,    25,    26,    27,    28,    -1,    -1,
      31,    32,    33,    34,    12,    -1,    14,    38,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    12,    -1,    14,
      38,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,
      25,    26,    27,    28,    -1,    -1,    31,    32,    33,    34,
      12,    36,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    24,    25,    26,    27,    28,    -1,    -1,    31,
      32,    33,    34,    22,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    22,    23,    24,    25,
      26,    27,    28,    -1,    -1,    31,    32,    33,    34
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    49,    50,     0,     3,     4,     6,     7,     8,     9,
      10,    11,    13,    15,    16,    17,    18,    19,    20,    23,
      29,    30,    35,    37,    39,    41,    44,    51,    52,    57,
      58,    60,    61,    62,    63,    68,    71,    75,    76,    80,
      81,    83,    85,    89,    91,    94,    95,    96,    39,    39,
      20,    74,    41,    52,    41,    41,    52,    20,    52,    39,
      61,    63,    61,    72,    35,    52,    67,    69,    70,    52,
      80,    20,    12,    14,    22,    23,    24,    25,    26,    27,
      28,    31,    32,    33,    34,    41,    21,    29,    30,    37,
      39,    45,    46,    64,    65,    66,    37,    39,    45,    39,
      73,    77,    51,     5,    84,    39,    90,    92,    52,    67,
      41,    80,    50,    52,    38,    42,    38,    42,    40,    40,
      55,    56,    52,    52,    52,    52,    52,    53,    54,    52,
      52,    52,    52,    59,    52,    67,    20,    20,    52,    67,
      20,    20,    82,    71,    79,    51,    52,    86,    88,    67,
      40,    93,    40,    36,    43,    52,    70,    39,    93,    93,
      52,    52,    52,    38,    40,    39,    38,    40,    40,    42,
      78,    40,    51,    40,    41,    52,    67,    52,    52,    67,
      20,    87,    92,    52,    36,    40,    40,    88,    41,    92
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    49,    50,    50,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    53,    52,    54,    52,    55,
      52,    56,    52,    52,    57,    57,    57,    57,    57,    57,
      57,    57,    59,    58,    60,    60,    60,    60,    60,    61,
      61,    61,    61,    62,    62,    62,    62,    63,    63,    63,
      64,    64,    65,    66,    67,    67,    67,    68,    68,    69,
      69,    70,    72,    71,    73,    74,    74,    75,    76,    77,
      78,    79,    80,    81,    81,    81,    81,    81,    82,    82,
      82,    83,    84,    85,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    96
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     0,     4,     0,     4,     0,
       5,     0,     5,     1,     3,     2,     2,     2,     2,     2,
       2,     1,     0,     4,     1,     1,     1,     3,     1,     1,
       2,     2,     1,     3,     4,     3,     4,     4,     2,     6,
       1,     1,     3,     5,     1,     3,     0,     3,     3,     1,
       3,     5,     0,     4,     3,     1,     0,     2,     2,     0,
       0,     1,     4,     1,     1,     1,     1,     1,     1,     3,
       0,     4,     1,     2,     3,     0,     0,     3,     1,     3,
       3,     0,     0,     7,     7,     2,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: stmtlist  */
#line 112 "Compiler/parser.y"
                                                        { print("program"); }
#line 1438 "build/parser.c"
    break;

  case 3: /* stmtlist: stmtlist stmt  */
#line 115 "Compiler/parser.y"
                                                { print("stmtlist"); }
#line 1444 "build/parser.c"
    break;

  case 5: /* stmt: expr semicolon  */
#line 119 "Compiler/parser.y"
                                                {
										print("stmt");
										(yyval.exprValue) = (yyvsp[-1].exprValue);

										if((yyvsp[-1].exprValue)->type == boolexpr_e || (yyvsp[-1].exprValue)->cameFromNot == 1) {
											(yyvsp[-1].exprValue)->sym = newTmp(scope);

											backPatchList((yyvsp[-1].exprValue)->trueList, nextQuadLabel());
											emit(assign, newExprConstBool('1'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
											emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

											backPatchList((yyvsp[-1].exprValue)->falseList, nextQuadLabel());
											emit(assign, newExprConstBool('0'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
										}

										resetTmp();
										print("stmt expr;");
							 		}
#line 1467 "build/parser.c"
    break;

  case 6: /* stmt: ifstmt  */
#line 137 "Compiler/parser.y"
                                                                {
			 							print("stmt ifstmt");

			 							(yyval.exprValue) = (yyvsp[0].exprValue);
			 							resetTmp();

			 							print("stmt ifstmt");
									 }
#line 1480 "build/parser.c"
    break;

  case 7: /* stmt: whilestmt  */
#line 145 "Compiler/parser.y"
                                                                {
		 								print("stmt whilestmt");

		 								(yyval.exprValue) = (yyvsp[0].exprValue);
		 								resetTmp();

		 								print("stmt whilestmt");
									 }
#line 1493 "build/parser.c"
    break;

  case 8: /* stmt: forstmt  */
#line 153 "Compiler/parser.y"
                                                                {
			 							print("stmt forstmt");

			 							(yyval.exprValue) = (yyvsp[0].exprValue);
			 							resetTmp();

			 							print("stmt forstmt");
			 						}
#line 1506 "build/parser.c"
    break;

  case 9: /* stmt: returnstmt  */
#line 161 "Compiler/parser.y"
                                                                {
			 							print("stmt returnstmt");

			 							(yyval.exprValue) = (yyvsp[0].exprValue);
			 							resetTmp();

			 							print("stmt returnstmt");
			 						}
#line 1519 "build/parser.c"
    break;

  case 10: /* stmt: BREAK semicolon  */
#line 169 "Compiler/parser.y"
                                                        {
										print("stmt break;");

										if(activeLoops == 0) {
											encounteredError = 1;
											printf("Error: break in not in a loop. Line: %d", getCurrentLine());
										} else {
											breakPush(nextQuadLabel());
											emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										}

										resetTmp();

										print("stmt break;");
									}
#line 1539 "build/parser.c"
    break;

  case 11: /* stmt: CONTINUE semicolon  */
#line 184 "Compiler/parser.y"
                                                        {
										print("stmt continue");

										if(activeLoops == 0) {
											encounteredError = 1;
											printf("Error: continue in not in a loop. Line: %d", getCurrentLine());
										} else {
											continuePush(nextQuadLabel());
											emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										}

										resetTmp();

										print("stmt continue;");
									}
#line 1559 "build/parser.c"
    break;

  case 12: /* stmt: block  */
#line 199 "Compiler/parser.y"
                                                                {
			 							print("stmt block");

			 							(yyval.exprValue) = (yyvsp[0].exprValue);
			 							resetTmp();

			 							print("stmt block");
								 	}
#line 1572 "build/parser.c"
    break;

  case 13: /* stmt: funcdef  */
#line 207 "Compiler/parser.y"
                                                                {
			 							print("stmt funcdef");

			 							(yyval.exprValue) = (yyvsp[0].exprValue);
			 							resetTmp();

			 							print("stmt funcdef");
			 						}
#line 1585 "build/parser.c"
    break;

  case 14: /* stmt: semicolon  */
#line 215 "Compiler/parser.y"
                                                                {
			 							print("stmt semicolon");

			 							resetTmp();

			 							print("stmt semicolon");
			 						}
#line 1597 "build/parser.c"
    break;

  case 15: /* expr: assignexpr  */
#line 224 "Compiler/parser.y"
                                                        {
 										print("expr assignexpr");
 										(yyval.exprValue) = (yyvsp[0].exprValue);
 										print("expr assignexpr");
								 	}
#line 1607 "build/parser.c"
    break;

  case 16: /* expr: expr plus expr  */
#line 229 "Compiler/parser.y"
                                                        {
										print("expr expt+expr");
										(yyval.exprValue) = lvalueExpr(newTmp(scope));
										emit(add, (yyvsp[-2].exprValue), (yyvsp[0].exprValue), (yyval.exprValue), nextQuadLabel(), getCurrentLine());
										print("expr expt+expr");
									}
#line 1618 "build/parser.c"
    break;

  case 17: /* expr: expr minus expr  */
#line 235 "Compiler/parser.y"
                                                        {
										print("expr expr-expr");
										(yyval.exprValue) = lvalueExpr(newTmp(scope));
										emit(sub, (yyvsp[-2].exprValue), (yyvsp[0].exprValue), (yyval.exprValue), nextQuadLabel(), getCurrentLine());
										print("expr expr-expr");
									}
#line 1629 "build/parser.c"
    break;

  case 18: /* expr: expr MUL expr  */
#line 241 "Compiler/parser.y"
                                                        {
										print("expr expr*expr");
										(yyval.exprValue) = lvalueExpr(newTmp(scope));
										emit(mul, (yyvsp[-2].exprValue), (yyvsp[0].exprValue), (yyval.exprValue), nextQuadLabel(), getCurrentLine());
										print("expr expr*expr");
									}
#line 1640 "build/parser.c"
    break;

  case 19: /* expr: expr DIV expr  */
#line 247 "Compiler/parser.y"
                                                        {
										print("expr expr/expr");
										(yyval.exprValue) = lvalueExpr(newTmp(scope));
										emit(div_, (yyvsp[-2].exprValue), (yyvsp[0].exprValue), (yyval.exprValue), nextQuadLabel(), getCurrentLine());
										print("expr expr/expr");
									}
#line 1651 "build/parser.c"
    break;

  case 20: /* expr: expr MOD expr  */
#line 253 "Compiler/parser.y"
                                                        {
										print("expr expr MOD expr");
										(yyval.exprValue) = lvalueExpr(newTmp(scope));
										emit(mod, (yyvsp[-2].exprValue), (yyvsp[0].exprValue), (yyval.exprValue), nextQuadLabel(), getCurrentLine());
										print("expr expr MOD expr");
									}
#line 1662 "build/parser.c"
    break;

  case 21: /* expr: expr greater expr  */
#line 262 "Compiler/parser.y"
                                                {
										print("expr exp>expr");
										(yyval.exprValue) = newExpr(boolexpr_e);
										(yyval.exprValue)->trueList = makeList(nextQuadLabel());
										(yyval.exprValue)->falseList = makeList(nextQuadLabel()+1);

										emit(if_greater, (yyvsp[-2].exprValue), (yyvsp[0].exprValue), NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										print("expr exp>expr");
									}
#line 1677 "build/parser.c"
    break;

  case 22: /* expr: expr gequal expr  */
#line 272 "Compiler/parser.y"
                                                {
										print("expr expr>=expr");
										(yyval.exprValue) = newExpr(boolexpr_e);
										(yyval.exprValue)->trueList = makeList(nextQuadLabel());
										(yyval.exprValue)->falseList = makeList(nextQuadLabel()+1);

										emit(if_greatereq, (yyvsp[-2].exprValue), (yyvsp[0].exprValue), NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										print("expr expr>=expr");
									}
#line 1692 "build/parser.c"
    break;

  case 23: /* expr: expr lesser expr  */
#line 282 "Compiler/parser.y"
                                                {
										print("expr expr<expr");
										(yyval.exprValue) = newExpr(boolexpr_e);
										(yyval.exprValue)->trueList = makeList(nextQuadLabel());
										(yyval.exprValue)->falseList = makeList(nextQuadLabel()+1);

										emit(if_less, (yyvsp[-2].exprValue), (yyvsp[0].exprValue), NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										print("expr expr<expr");
									}
#line 1707 "build/parser.c"
    break;

  case 24: /* expr: expr lequal expr  */
#line 292 "Compiler/parser.y"
                                                {
										print("expr expr<=expr");
										(yyval.exprValue) = newExpr(boolexpr_e);
										(yyval.exprValue)->trueList = makeList(nextQuadLabel());
										(yyval.exprValue)->falseList = makeList(nextQuadLabel()+1);

										emit(if_lesseq, (yyvsp[-2].exprValue), (yyvsp[0].exprValue), NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										print("expr expr<=expr");
									}
#line 1722 "build/parser.c"
    break;

  case 25: /* $@1: %empty  */
#line 302 "Compiler/parser.y"
                                                { checkForEmitEqEq((yyvsp[-1].exprValue), getCurrentLine(), scope); }
#line 1728 "build/parser.c"
    break;

  case 26: /* expr: expr eequal $@1 expr  */
#line 302 "Compiler/parser.y"
                                                                                                        {
									print("expr expr==expr");

									if((yyvsp[0].exprValue)->type == boolexpr_e && (yyvsp[0].exprValue)->cameFromNot == 1) {
										(yyvsp[0].exprValue)->sym = newTmp(scope);

										backPatchList((yyvsp[0].exprValue)->trueList, nextQuadLabel());
										emit(assign, newExprConstBool('1'), NULL, (yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

										backPatchList((yyvsp[0].exprValue)->falseList, nextQuadLabel());
										emit(assign, newExprConstBool('0'), NULL, (yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine());
									}

									(yyval.exprValue) = newExpr(boolexpr_e);
									(yyval.exprValue)->trueList = makeList(nextQuadLabel());
									(yyval.exprValue)->falseList = makeList(nextQuadLabel() + 1);
									emit(if_eq, (yyvsp[-3].exprValue), (yyvsp[0].exprValue), NULL, nextQuadLabel(), getCurrentLine());
									emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());

									print("expr expr==expr");
								}
#line 1755 "build/parser.c"
    break;

  case 27: /* $@2: %empty  */
#line 324 "Compiler/parser.y"
                                                { checkForEmitNotEq((yyvsp[-1].exprValue), getCurrentLine(), scope); }
#line 1761 "build/parser.c"
    break;

  case 28: /* expr: expr nequal $@2 expr  */
#line 324 "Compiler/parser.y"
                                                                                                         {
									print("expr expr!=expr");

									if((yyvsp[0].exprValue)->type == boolexpr_e && (yyvsp[0].exprValue)->cameFromNot == 1) {
										(yyvsp[0].exprValue)->sym = newTmp(scope);

										backPatchList((yyvsp[0].exprValue)->trueList, nextQuadLabel());
										emit(assign, newExprConstBool('1'), NULL, (yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

										backPatchList((yyvsp[0].exprValue)->falseList, nextQuadLabel());
										emit(assign, newExprConstBool('0'), NULL, (yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine());
									}

									(yyval.exprValue) = newExpr(boolexpr_e);
									(yyval.exprValue)->trueList = makeList(nextQuadLabel());
									(yyval.exprValue)->falseList = makeList(nextQuadLabel() + 1);
									emit(if_noteq, (yyvsp[-3].exprValue), (yyvsp[0].exprValue), NULL, nextQuadLabel(), getCurrentLine());
									emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());

									print("expr expr!=expr");
			 					}
#line 1788 "build/parser.c"
    break;

  case 29: /* $@3: %empty  */
#line 346 "Compiler/parser.y"
                                                        { checkForEmitAND((yyvsp[-1].exprValue), getCurrentLine(), notRulePassed); }
#line 1794 "build/parser.c"
    break;

  case 30: /* expr: expr AND $@3 M expr  */
#line 346 "Compiler/parser.y"
                                                                                                                         {
									print("expr expr&&expr");

									notRulePassed = 0;
									(yyval.exprValue) = newExpr(boolexpr_e);

									// TypeCheck for arguments that are not boolean and create logic lists for them
									if((yyvsp[0].exprValue)->type != boolexpr_e) {
										(yyvsp[0].exprValue)->trueList = makeList(nextQuadLabel());
										(yyvsp[0].exprValue)->falseList = makeList(nextQuadLabel() + 1);

										emit(if_eq, newExprConstBool('1'), (yyvsp[0].exprValue), NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
									}

									if((yyvsp[-4].exprValue)->type == boolexpr_e)
										backPatchList((yyvsp[-4].exprValue)->trueList, (yyvsp[-1].intVal));

									(yyval.exprValue)->trueList = (yyvsp[0].exprValue)->trueList;
									(yyval.exprValue)->falseList = mergeLLs((yyvsp[-4].exprValue)->falseList, (yyvsp[0].exprValue)->falseList);

									print("expr expr&&expr");
								}
#line 1822 "build/parser.c"
    break;

  case 31: /* $@4: %empty  */
#line 369 "Compiler/parser.y"
                                                        { checkForEmitOR((yyvsp[-1].exprValue), getCurrentLine(), notRulePassed); }
#line 1828 "build/parser.c"
    break;

  case 32: /* expr: expr OR $@4 M expr  */
#line 369 "Compiler/parser.y"
                                                                                                                        {
                                	print("expr expr||expr");

                                	(yyval.exprValue) = newExpr(boolexpr_e);

									// TypeCheck for arguments that are not boolean and create logic lists for them
									if((yyvsp[0].exprValue)->type != boolexpr_e) {
										(yyvsp[0].exprValue)->trueList = makeList(nextQuadLabel());
										(yyvsp[0].exprValue)->falseList = makeList(nextQuadLabel() + 1);

										emit(if_eq, newExprConstBool('1'), (yyvsp[0].exprValue), NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
									}

									if((yyvsp[-4].exprValue)->type == boolexpr_e)
										backPatchList((yyvsp[-4].exprValue)->falseList, (yyvsp[-1].intVal));

									(yyval.exprValue)->trueList = mergeLLs((yyvsp[-4].exprValue)->trueList, (yyvsp[0].exprValue)->trueList);
									(yyval.exprValue)->falseList = (yyvsp[0].exprValue)->falseList;
									notRulePassed = 0;

									print("expr expr||expr");
                                }
#line 1856 "build/parser.c"
    break;

  case 33: /* expr: term  */
#line 392 "Compiler/parser.y"
                                                        {
									print("expr term");
									(yyval.exprValue) = (yyvsp[0].exprValue);
			 						print("expr term");
			 					}
#line 1866 "build/parser.c"
    break;

  case 34: /* term: openPar expr closePar  */
#line 399 "Compiler/parser.y"
                                                {
 											print("term (expr)");
 											(yyval.exprValue) = (yyvsp[-1].exprValue);
 											 print("term (expr)");
 										}
#line 1876 "build/parser.c"
    break;

  case 35: /* term: minus expr  */
#line 404 "Compiler/parser.y"
                                                        {
											print("term -expr prec uminus");

											checkUminus((yyvsp[0].exprValue));
											(yyval.exprValue) = newExpr(arithexpr_e);
											(yyval.exprValue)->sym = newTmp(scope);

											emit(mul, newExprConstNumber(-1), (yyvsp[0].exprValue), (yyval.exprValue), nextQuadLabel(), getCurrentLine());

											print("term -expr prec uminus");
										}
#line 1892 "build/parser.c"
    break;

  case 36: /* term: NOT expr  */
#line 415 "Compiler/parser.y"
                                                                        {
											print("term !expr");

											/*Meriki apotimisi*/
											(yyval.exprValue) = newExpr(boolexpr_e);
											(yyval.exprValue)->sym = (yyvsp[0].exprValue)->sym;
											(yyval.exprValue)->cameFromNot = 1;

											if((yyvsp[0].exprValue)->type != boolexpr_e){
												(yyval.exprValue)->trueList = makeList(nextQuadLabel() + 1);
												(yyval.exprValue)->falseList = makeList(nextQuadLabel());

												emit(if_eq, newExprConstBool('1'), (yyvsp[0].exprValue), NULL, nextQuadLabel(), getCurrentLine());
												emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
											} else {
												(yyval.exprValue)->trueList = (yyvsp[0].exprValue)->falseList;
												(yyval.exprValue)->falseList = (yyvsp[0].exprValue)->trueList;
											}

											print("term !expr");
										}
#line 1918 "build/parser.c"
    break;

  case 37: /* term: pplus lvalue  */
#line 436 "Compiler/parser.y"
                                                                {
											print("term ++lvalue");

											char* name = (char*) (yyvsp[0].exprValue)->sym->value.varVal->name;

											// Check for redeclarations
											if(!isLibFunc(name, getCurrentLine())) {
												// Check inside to avoid duplicate errors
												isAlreadyDeclared(name, scope, getCurrentLine());
											}

											memberRuleManage = equalFound;

											if((yyvsp[0].exprValue)->type == tableitem_e) {
												(yyval.exprValue) = emitIfTableItem((yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine(), scope);

												emit(add, (yyval.exprValue), newExprConstNumber(1), (yyval.exprValue) ,nextQuadLabel(), getCurrentLine());
												emit(tablesetelem, (yyvsp[0].exprValue), (yyvsp[0].exprValue)->index, (yyval.exprValue), nextQuadLabel(), getCurrentLine());
											} else {
												emit(add, (yyvsp[0].exprValue), newExprConstNumber(1), (yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine());

												(yyval.exprValue) = newExpr(arithexpr_e);
												(yyval.exprValue)->sym = newTmp(scope);

												emit(assign, (yyvsp[0].exprValue), NULL, (yyval.exprValue), nextQuadLabel(), getCurrentLine());
											}

											print("term ++lvalue");
										}
#line 1952 "build/parser.c"
    break;

  case 38: /* term: lvalue pplus  */
#line 465 "Compiler/parser.y"
                                                                {
											print("term lvalue++");

											char* name = (char*) (yyvsp[-1].exprValue)->sym->value.varVal->name;

											// Check for redeclarations
											if(!isLibFunc(name, getCurrentLine())) {
												// Check inside to avoid duplicate errors
												isAlreadyDeclared(name, scope, getCurrentLine());
											}

											memberRuleManage = equalFound;

											(yyval.exprValue) = newExpr(var_e);
											(yyval.exprValue)->sym = newTmp(scope);

											if((yyvsp[-1].exprValue)->type == tableitem_e) {
												struct expr *value = emitIfTableItem((yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine(), scope);

												emit(assign, value, NULL, (yyval.exprValue), nextQuadLabel(), getCurrentLine());
												emit(add, value, newExprConstNumber(1), value, nextQuadLabel(), getCurrentLine());
												emit(tablesetelem, (yyvsp[-1].exprValue), (yyvsp[-1].exprValue)->index, value, nextQuadLabel(), getCurrentLine());
											} else {
												emit(assign, (yyvsp[-1].exprValue), NULL, (yyval.exprValue), nextQuadLabel(), getCurrentLine());
												emit(add, (yyvsp[-1].exprValue), newExprConstNumber(1), (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
											}

											print("term lvalue++");
										}
#line 1986 "build/parser.c"
    break;

  case 39: /* term: mminus lvalue  */
#line 494 "Compiler/parser.y"
                                                                {
											print("term --lvalue");

											char* name = (char*) (yyvsp[0].exprValue)->sym->value.varVal->name;

											// Check for redeclarations
											if(!isLibFunc(name, getCurrentLine())) {
												// Check inside to avoid duplicate errors
												isAlreadyDeclared(name, scope, getCurrentLine());
											}

											memberRuleManage = equalFound;

											if((yyvsp[0].exprValue)->type == tableitem_e) {
												(yyval.exprValue) = emitIfTableItem((yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine(), scope);

												emit(sub, (yyval.exprValue), newExprConstNumber(1), (yyval.exprValue), nextQuadLabel(), getCurrentLine());
												emit(tablesetelem, (yyvsp[0].exprValue), (yyvsp[0].exprValue)->index, (yyval.exprValue), nextQuadLabel(), getCurrentLine());
											} else {
												emit(sub, (yyvsp[0].exprValue), newExprConstNumber(1), (yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine());

												(yyval.exprValue) = newExpr(arithexpr_e);
												(yyval.exprValue)->sym = newTmp(scope);

												emit(assign, (yyvsp[0].exprValue), NULL, (yyval.exprValue), nextQuadLabel(), getCurrentLine());
											}

											print("term --lvalue");
										}
#line 2020 "build/parser.c"
    break;

  case 40: /* term: lvalue mminus  */
#line 523 "Compiler/parser.y"
                                                                {
											print("term lvalue--");

											char* name = (char*) (yyvsp[-1].exprValue)->sym->value.varVal->name;

											// Check for redeclarations
											if(!isLibFunc(name, getCurrentLine())) {
												// Check inside to avoid duplicate errors
												isAlreadyDeclared(name, scope, getCurrentLine());
											}

											memberRuleManage = equalFound;

											(yyval.exprValue) = newExpr(var_e);
											(yyval.exprValue)->sym = newTmp(scope);

											if((yyvsp[-1].exprValue)->type == tableitem_e) {
												struct expr *value = emitIfTableItem((yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine(), scope);

												emit(assign, value, NULL, (yyval.exprValue), nextQuadLabel(), getCurrentLine());
												emit(sub, value, newExprConstNumber(1), value, nextQuadLabel(), getCurrentLine());
												emit(tablesetelem, (yyvsp[-1].exprValue), (yyvsp[-1].exprValue)->index, value, nextQuadLabel(), getCurrentLine());
											} else {
												emit(assign, (yyvsp[-1].exprValue), NULL, (yyval.exprValue), nextQuadLabel(), getCurrentLine());
												emit(sub, (yyvsp[-1].exprValue), newExprConstNumber(1), (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
											}

											print("term lvalue--");
										}
#line 2054 "build/parser.c"
    break;

  case 41: /* term: primary  */
#line 552 "Compiler/parser.y"
                                                                        {
			 								print("term primary");
			 								(yyval.exprValue) = (yyvsp[0].exprValue);
			 								print("term primary");
			 							}
#line 2064 "build/parser.c"
    break;

  case 42: /* $@5: %empty  */
#line 559 "Compiler/parser.y"
                             { equalFound = 1; }
#line 2070 "build/parser.c"
    break;

  case 43: /* assignexpr: lvalue equal $@5 expr  */
#line 559 "Compiler/parser.y"
                                                        {
				print("assignexpr lvalue=expr");

				char *var = NULL;

				if((yyvsp[0].exprValue)->type == boolexpr_e) {
					(yyvsp[0].exprValue)->sym = newTmp(scope);

					backPatchList((yyvsp[0].exprValue)->trueList, nextQuadLabel());
					emit(assign, newExprConstBool('1'), NULL, (yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine());
					emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

					backPatchList((yyvsp[0].exprValue)->falseList, nextQuadLabel());
					emit(assign, newExprConstBool('0'), NULL, (yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine());
				}

				if((yyvsp[-3].exprValue)->type == tableitem_e) {
					emit(tablesetelem, (yyvsp[-3].exprValue), (yyvsp[-3].exprValue)->index, (yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine());

					(yyval.exprValue) = emitIfTableItem((yyvsp[-3].exprValue), nextQuadLabel(), getCurrentLine(), scope);
					(yyval.exprValue)->type = assignexpr_e;
				} else {
					char* symName = (char*) (yyvsp[-3].exprValue)->sym->value.varVal->name;
					struct SymbolTable *sym = (yyvsp[-3].exprValue)->sym;

					(yyvsp[-3].exprValue) = lvalueExpr(sym);
					emit(assign, (yyvsp[0].exprValue), NULL, (yyvsp[-3].exprValue), nextQuadLabel(), getCurrentLine());

					(yyval.exprValue) = newExpr(assignexpr_e);
					(yyval.exprValue)->sym = newTmp(scope);
					emit(assign, (yyvsp[-3].exprValue), NULL, (yyval.exprValue), nextQuadLabel(), getCurrentLine());
				}

				if(memberRuleManage == 1) {
					if(globalBeforeEqual == 0 && localBeforeEqual == 0 && memberBeforeEqual == 0)
						var = (char*) (yyvsp[-3].exprValue)->sym->value.varVal->name;
				} else if(globalBeforeEqual == 0 && localBeforeEqual == 0 && memberManageRule == 0)
						var = (char*) (yyvsp[-3].exprValue)->sym->value.varVal->name;


				if(var != NULL) {
					if(!isLibFunc(var, getCurrentLine())) {
						if(!lookUpScopeRedeclaration(var, variable, scope, getCurrentLine(), AssignExpr))
						   if(accessIsValid == 1 && !isAlreadyDeclared(var, scope, getCurrentLine()));
					}
				}

				globalRuleManage = 0;
				localRuleManage = 0;
				globalBeforeEqual = 0;
				localBeforeEqual = 0;
				memberRuleManage = 0;
				memberManageRule = 0;
				equalFound = 0;
				accessIsValid = 0;

				print("assignexpr lvalue=expr");
			}
#line 2133 "build/parser.c"
    break;

  case 44: /* primary: lvalue  */
#line 619 "Compiler/parser.y"
                                                                {
											print("primary lvalue");

											if(memberManageRule == 1 && (yyvsp[0].exprValue)->type == tableitem_e)
												(yyval.exprValue) = emitIfTableItem((yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine(), scope);

											char* name = (char*) (yyvsp[0].exprValue)->sym->value.varVal->name;

											if ((globalRuleManage == 1 && globalBeforeEqual == 1 && equalFound == 1)
											 || (localRuleManage == 1 && localBeforeEqual == 1 && equalFound == 1)
											 || (memberManageRule == 1 && memberBeforeEqual == 1 && equalFound == 1)
											 || (globalRuleManage == 0 && localRuleManage == 0 && memberManageRule == 0))
												(yyval.exprValue) = lvalueExpr((yyvsp[0].exprValue)->sym);

											if(equalFound == 1)
												memberRuleManage = 1;
											else {
												 /* e.x.
												  		local x;
												  		function f{ x = 2; }
												  	The local x makes primary = 1, so assignment rule will see primary = 1 and won't
												  	check the x = 2. So if we have not found '=' primary is stand alone.
												*/
												memberManageRule = 0;
												memberRuleManage = 0;
												localRuleManage = 0;
												globalRuleManage = 0;
												globalBeforeEqual = 0;
												localBeforeEqual = 0;
											}

											print("primary lvalue");
										}
#line 2171 "build/parser.c"
    break;

  case 45: /* primary: call  */
#line 652 "Compiler/parser.y"
                                                                        { print("primary call"); }
#line 2177 "build/parser.c"
    break;

  case 46: /* primary: objectdef  */
#line 653 "Compiler/parser.y"
                                                                        {
			 								print("primary objectdef");
			 								(yyval.exprValue) = (yyvsp[0].exprValue);
			 								print("primary objectdef");
			 							}
#line 2187 "build/parser.c"
    break;

  case 47: /* primary: openPar funcdef closePar  */
#line 658 "Compiler/parser.y"
                                                        {
											print("primary (funcdef)");
											(yyval.exprValue) = newExpr(programfunc_e);
											(yyval.exprValue)->sym = (yyvsp[-1].symbolTable);
											print("primary (funcdef)");
										}
#line 2198 "build/parser.c"
    break;

  case 48: /* primary: const  */
#line 664 "Compiler/parser.y"
                                                                        {
											print("primary const");
											(yyval.exprValue) = (yyvsp[0].exprValue);
											print("primary const");
										}
#line 2208 "build/parser.c"
    break;

  case 49: /* lvalue: ID  */
#line 671 "Compiler/parser.y"
                                                                        {
											print("lvalue id");

											struct SymbolTable *sym = NULL;
                                            int symbolExists = 1;
                                            int isFuncCall = 0;

                                            if(!invalidAccess((yyvsp[0].strValue), scope, getCurrentLine())) {
                                                accessIsValid = 1;
                                                if(lvalueIsFunc((yyvsp[0].strValue), scope) == NULL) {
                                                    if(existsInSameBucket(yylval.strValue, scope) == NULL)
                                                        symbolExists = 0;

                                                    if(scope > 0)
                                                        sym = insert(yylval.strValue, variable, LOCAL, scope, getCurrentLine(), 0, currentScopeSpace(), currentScopeOffset());
                                                    else if(scope == 0)
                                                        sym = insert(yylval.strValue, variable, GLOBAL, scope, getCurrentLine(), 0, currentScopeSpace(), currentScopeOffset());

                                                    if(!symbolExists)
                                                        increaseCurrentScopeOffset();
												}
												else
													isFuncCall = 1;
                                            }
                                            else
                                            	accessIsValid = 0;

                                            if(sym == NULL) {
                                                if(isFuncCall) {
                                                    sym = lvalueIsFunc((yyvsp[0].strValue), scope);
                                                    (yyval.exprValue) = lvalueExpr(sym);
                                                }
                                                else
                                                	(yyval.exprValue) = lvalueExpr(
                                                		createBucket(yylval.strValue, variable, GLOBAL, scope, getCurrentLine(), currentScopeSpace(), currentScopeOffset())
													);
                                            } else {
                                                switch(sym->type){
                                                    case LOCAL       : (yyval.exprValue) = lvalueExpr(sym); break;
                                                    case GLOBAL      : (yyval.exprValue) = lvalueExpr(sym); break;
                                                    case LIB_FUNC    : (yyval.exprValue) = lvalueExpr(sym); break;
                                                    case USER_FUNC   : (yyval.exprValue) = lvalueExpr(sym); break;
                                                    case FORMAL      : (yyval.exprValue) = lvalueExpr(sym); break;
                                                    default          : assert(0);
                                                }
                                            }

                                            print("lvalue id");
										}
#line 2262 "build/parser.c"
    break;

  case 50: /* lvalue: local ID  */
#line 720 "Compiler/parser.y"
                                                                        {
											print("lvalue local id");

											localRuleManage = 1;

											if(equalFound == 0)
												localBeforeEqual = 1;
											else localBeforeEqual = 0;

											int symbolExists = 1;
											struct SymbolTable *sym = NULL;

											if(scope > 0) {
												if (!isLibFunc(yylval.strValue, getCurrentLine())) {
													if(findBucketDuplicateLocal(yylval.strValue, scope) == NULL)
														symbolExists = 0;

													if(scope > 0)
														sym = insert(yylval.strValue, variable, LOCAL, scope, getCurrentLine(), 1, currentScopeSpace(), currentScopeOffset());
													else if(scope == 0)
														sym = insert(yylval.strValue, variable, GLOBAL, scope, getCurrentLine(), 1, currentScopeSpace(), currentScopeOffset());

													if(!symbolExists)
														increaseCurrentScopeOffset();

													(yyval.exprValue) = lvalueExpr(sym);
												}
											} else {
												if (!existsLibFunc(yylval.strValue)) {
														if(existsInSameBucket(yylval.strValue, scope) == NULL)
															symbolExists = 0;

														sym = insert(yylval.strValue, variable, GLOBAL, scope, getCurrentLine(), 1, currentScopeSpace(), currentScopeOffset());

														if(!symbolExists)
															increaseCurrentScopeOffset();

														(yyval.exprValue) = lvalueExpr(sym);
													}
											}

											if(sym == NULL)
												(yyval.exprValue) = lvalueExpr(
													createBucket(yylval.strValue, variable, GLOBAL, scope, getCurrentLine(), currentScopeSpace(), currentScopeOffset())
												);
											else {
												switch(sym->type){
													case LOCAL       : (yyval.exprValue) = lvalueExpr(sym); break;
													case GLOBAL      : (yyval.exprValue) = lvalueExpr(sym); break;
													case LIB_FUNC    : (yyval.exprValue) = lvalueExpr(sym); break;
													case USER_FUNC   : (yyval.exprValue) = lvalueExpr(sym); break;
													case FORMAL      : (yyval.exprValue) = lvalueExpr(sym); break;
													default          : assert(0);
												}
											}

											print("lvalue local id");
										}
#line 2325 "build/parser.c"
    break;

  case 51: /* lvalue: ccolon ID  */
#line 778 "Compiler/parser.y"
                                                                        {
											print("lvalue ::id");

											globalBeforeEqual = equalFound == 0;
											globalRuleManage = 1;

											struct SymbolTable *entry = checkGlobalScope(yylval.strValue, getCurrentLine());
											if(entry == NULL)
												(yyval.exprValue) = lvalueExpr(
													createBucket(yylval.strValue, variable, GLOBAL, scope, getCurrentLine(), currentScopeSpace(), currentScopeOffset())
												);
											else
												(yyval.exprValue) = lvalueExpr(entry);

											print("lvalue ::id");
										}
#line 2346 "build/parser.c"
    break;

  case 52: /* lvalue: member  */
#line 794 "Compiler/parser.y"
                                                                        {
											(yyval.exprValue) = (yyvsp[0].exprValue);
											memberManageRule = 1;
											memberBeforeEqual = equalFound == 0;

											print("lvalue member");
										}
#line 2358 "build/parser.c"
    break;

  case 53: /* member: lvalue dot ID  */
#line 803 "Compiler/parser.y"
                                                                                {
														print("member lvalue.id");

														struct SymbolTable *sym;
														char* name = (char*) (yyvsp[-2].exprValue)->sym->value.varVal->name;
														(yyval.exprValue) = memberItem((yyvsp[-2].exprValue), nextQuadLabel(), getCurrentLine(), scope, (yyvsp[0].strValue));

														print("member lvalue.id");
													}
#line 2372 "build/parser.c"
    break;

  case 54: /* member: lvalue openBracket expr closeBracket  */
#line 812 "Compiler/parser.y"
                                                                {
														print("member lvalue[expr]");

														if((yyvsp[-1].exprValue)->type == boolexpr_e) {
															(yyvsp[-1].exprValue)->sym = newTmp(scope);

															backPatchList((yyvsp[-1].exprValue)->trueList, nextQuadLabel());
															emit(assign, newExprConstBool('1'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
															emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

															backPatchList((yyvsp[-1].exprValue)->falseList, nextQuadLabel());
															emit(assign, newExprConstBool('0'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
														}

														char* name = (char*) (yyvsp[-3].exprValue)->sym->value.varVal->name;
														(yyvsp[-3].exprValue) = emitIfTableItem((yyvsp[-3].exprValue), nextQuadLabel(), getCurrentLine(), scope);

														(yyval.exprValue) = newExpr(tableitem_e);
														(yyval.exprValue)->sym = (yyvsp[-3].exprValue)->sym;
														(yyval.exprValue)->index = (yyvsp[-1].exprValue);

														print("member lvalue[expr]");
													}
#line 2400 "build/parser.c"
    break;

  case 55: /* member: call dot ID  */
#line 835 "Compiler/parser.y"
                                                                                        { print("member call.id"); }
#line 2406 "build/parser.c"
    break;

  case 56: /* member: call openBracket expr closeBracket  */
#line 836 "Compiler/parser.y"
                                                                        { print("member call[expr]"); }
#line 2412 "build/parser.c"
    break;

  case 57: /* call: call openPar elist closePar  */
#line 839 "Compiler/parser.y"
                                                                                                {
 																	print("call call(elist)");
 																	(yyval.exprValue) = makeCall((yyval.exprValue), (yyvsp[-1].exprValue), scope, getCurrentLine());
 																	print("call call(elist)");
 																}
#line 2422 "build/parser.c"
    break;

  case 58: /* call: lvalue callsuffix  */
#line 844 "Compiler/parser.y"
                                                                                                                {
                                                                    print("call lvalue callsuffix");

																	char *name = NULL;
                                                            		if((yyvsp[-1].exprValue)->sym->what == function)
                                                            			name = (char*) (yyvsp[-1].exprValue)->sym->value.funcVal->name;
																	else
																		name = (char*) (yyvsp[-1].exprValue)->sym->value.varVal->name;

                                                                    if(existsLibFunc(name))
																		(yyvsp[-1].exprValue)->type = libraryfunc_e;

																	if((yyvsp[0].methodCall)->method) {
																		struct expr *self = (yyvsp[-1].exprValue);

																		struct expr *mi = memberItem(self, nextQuadLabel(), getCurrentLine(), scope, (yyvsp[0].methodCall)->name);
																		(yyvsp[-1].exprValue) = emitIfTableItem(mi, nextQuadLabel(), getCurrentLine(), scope);

																		struct expr *tmp = (yyvsp[0].methodCall)->elist;
																		tmp->prev = self; // TODO - Error at methodCall
																		self->next = tmp;
																		(yyvsp[0].methodCall)->elist = self;
																	}

																	(yyval.exprValue) = makeCall((yyvsp[-1].exprValue), (yyvsp[0].methodCall)->elist, scope, getCurrentLine());

																	print("call lvalue callsuffix");
																}
#line 2455 "build/parser.c"
    break;

  case 59: /* call: openPar funcdef closePar openPar elist closePar  */
#line 872 "Compiler/parser.y"
                                                                                {
																	print("call (funcdef)(elist)");

																	struct expr *func = newExpr(programfunc_e);
																	func->sym = (yyvsp[-4].symbolTable);
																	(yyval.exprValue) = makeCall(func, (yyvsp[-1].exprValue), scope, getCurrentLine());

																	print("call (funcdef)(elist)");
																}
#line 2469 "build/parser.c"
    break;

  case 60: /* callsuffix: normcall  */
#line 883 "Compiler/parser.y"
                                        {
 								print("callsuffix normcall");
 								(yyval.methodCall) = (yyvsp[0].methodCall);
 								print("callsuffix normcall");
 							}
#line 2479 "build/parser.c"
    break;

  case 61: /* callsuffix: methodcall  */
#line 888 "Compiler/parser.y"
                                                {
			 					print("callsuffix methodcall");
			 					(yyval.methodCall) = (yyvsp[0].methodCall);
			 					print("callsuffix methodcall");
			 				}
#line 2489 "build/parser.c"
    break;

  case 62: /* normcall: openPar elist closePar  */
#line 895 "Compiler/parser.y"
                                                        {
												print("normcall (elist)");

												struct MethodCall* mc = malloc(sizeof(struct MethodCall));
												memset(mc, 0, sizeof(struct MethodCall));
												(yyval.methodCall) = mc;
												(yyval.methodCall)->elist = (yyvsp[-1].exprValue);
												(yyval.methodCall)->method = 0;
												(yyval.methodCall)->name = NULL;

												print("normcall (elist)");
											}
#line 2506 "build/parser.c"
    break;

  case 63: /* methodcall: ddot ID openPar elist closePar  */
#line 909 "Compiler/parser.y"
                                                {
												print("methodcall ..id(elist)");

												struct MethodCall* mc = malloc(sizeof(struct MethodCall));
												memset(mc, 0, sizeof(struct MethodCall));
												(yyval.methodCall) = mc;
												(yyval.methodCall)->elist = (yyvsp[-1].exprValue);
												(yyval.methodCall)->method = 1;
												(yyval.methodCall)->name = (yyvsp[-3].strValue);

												print("methodcall ..id(elist)");
											}
#line 2523 "build/parser.c"
    break;

  case 64: /* elist: expr  */
#line 923 "Compiler/parser.y"
                                                        {
										print("elist expr");

										if((yyvsp[0].exprValue)->type == boolexpr_e) {
											(yyvsp[0].exprValue)->sym = newTmp(scope);

											backPatchList((yyvsp[0].exprValue)->trueList, nextQuadLabel());
											emit(assign, newExprConstBool('1'), NULL, (yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine());
											emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

											backPatchList((yyvsp[0].exprValue)->falseList, nextQuadLabel());
											emit(assign, newExprConstBool('0'), NULL, (yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine());
										}

										(yyval.exprValue) = (yyvsp[0].exprValue);
										(yyval.exprValue)->voidParam = 0;

										print("elist expr");
									}
#line 2547 "build/parser.c"
    break;

  case 65: /* elist: elist comma expr  */
#line 942 "Compiler/parser.y"
                                                        {
										print("elist elist,expr");

										if((yyvsp[0].exprValue)->type == boolexpr_e) {
											(yyvsp[0].exprValue)->sym = newTmp(scope);

											backPatchList((yyvsp[0].exprValue)->trueList, nextQuadLabel());
											emit(assign, newExprConstBool('1'), NULL, (yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine());
											emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

											backPatchList((yyvsp[0].exprValue)->falseList, nextQuadLabel());
											emit(assign, newExprConstBool('0'), NULL, (yyvsp[0].exprValue), nextQuadLabel(), getCurrentLine());
										}

										struct expr *tmp = (yyvsp[-2].exprValue);
										while(tmp->next != NULL)
											tmp = tmp->next;

										tmp->next = (yyvsp[0].exprValue);
										(yyvsp[0].exprValue)->prev = tmp;
										(yyval.exprValue) = (yyvsp[-2].exprValue);
										(yyval.exprValue)->voidParam = 0;

										print("elist elist,expr");
									}
#line 2577 "build/parser.c"
    break;

  case 66: /* elist: %empty  */
#line 967 "Compiler/parser.y"
                                                        {
										print("elist /*empty*/");

										(yyval.exprValue) = NULL;
										//$$ = newExprNil();
										//$$->voidParam = 1;

										print("elist /*empty*/");
									}
#line 2591 "build/parser.c"
    break;

  case 67: /* objectdef: openBracket elist closeBracket  */
#line 978 "Compiler/parser.y"
                                                                {
														print("objectdef [elist]");

														struct expr *t = newExpr(newtable_e);
														t->sym  = newTmp(scope);
														emit(tablecreate, NULL, NULL, t, nextQuadLabel(), getCurrentLine());

														struct expr *tmp = (yyvsp[-1].exprValue);
														int i = 0;

														while(tmp != NULL) {
															emit(tablesetelem, t, newExprConstNumber(i++), tmp, nextQuadLabel(), getCurrentLine());
															tmp = tmp->next;
														}
														(yyval.exprValue) = t;

														print("objectdef [elist]");
													}
#line 2614 "build/parser.c"
    break;

  case 68: /* objectdef: openBracket indexed closeBracket  */
#line 996 "Compiler/parser.y"
                                                                        {
														print("objectdef [indexed]");

														struct expr *t = newExpr(newtable_e);
														t->sym = newTmp(scope);

														emit(tablecreate, NULL, NULL, t, nextQuadLabel(), getCurrentLine());

														struct expr *tmp = (yyvsp[-1].exprValue);
														while(tmp != NULL) {
															emit(tablesetelem, t, tmp->index, tmp->indexedVal, nextQuadLabel(), getCurrentLine());
															tmp = tmp->next;
														}

														(yyval.exprValue) = t;

														print("objectdef [indexed]");
													}
#line 2637 "build/parser.c"
    break;

  case 69: /* indexed: indexedelem  */
#line 1016 "Compiler/parser.y"
                                                                                                {
 															print("indexed indexedelem");
 															(yyval.exprValue) = (yyvsp[0].exprValue);
 														}
#line 2646 "build/parser.c"
    break;

  case 70: /* indexed: indexed comma indexedelem  */
#line 1020 "Compiler/parser.y"
                                                                                        {
															print("indexed indexed,indexedelem");

															struct expr *tmp = (yyvsp[-2].exprValue);
															while(tmp->next != NULL)
																tmp = tmp->next;

															tmp->next = (yyvsp[0].exprValue);
															(yyval.exprValue) = (yyvsp[-2].exprValue);

															print("indexed indexed,indexedelem");
														 }
#line 2663 "build/parser.c"
    break;

  case 71: /* indexedelem: openBlock expr colon expr closeBlock  */
#line 1034 "Compiler/parser.y"
                                                                {
															print("indexedelem {expr:expr}");

															if((yyvsp[-1].exprValue)->type == boolexpr_e) {
																(yyvsp[-1].exprValue)->sym = newTmp(scope);

																backPatchList((yyvsp[-1].exprValue)->trueList, nextQuadLabel());
																emit(assign, newExprConstBool('1'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
																emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

																backPatchList((yyvsp[-1].exprValue)->falseList, nextQuadLabel());
																emit(assign, newExprConstBool('0'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
															}

															(yyval.exprValue) = newExpr(tableitem_e);
															(yyval.exprValue)->index = (yyvsp[-3].exprValue);
															(yyval.exprValue)->indexedVal = (yyvsp[-1].exprValue);

															print("indexedelem {expr:expr}");
														}
#line 2688 "build/parser.c"
    break;

  case 72: /* $@6: %empty  */
#line 1056 "Compiler/parser.y"
                          { scopeOps(INCREASE); }
#line 2694 "build/parser.c"
    break;

  case 73: /* block: openBlock $@6 stmtlist closeBlock  */
#line 1056 "Compiler/parser.y"
                                                                        {
																		print("block {stmtlist}");
																		toggleActive(scope, 0);
																		scopeOps(DECREASE);
																		print("block {stmtlist}");
																	}
#line 2705 "build/parser.c"
    break;

  case 74: /* funcArgs: openPar idlist closePar  */
#line 1066 "Compiler/parser.y"
                                        {
										print("funcArgs (idlist)");
										enterScopeSpace();
										resetFunctionLocalOffset();
										print("funcArgs (idlist)");
									}
#line 2716 "build/parser.c"
    break;

  case 75: /* funcName: ID  */
#line 1074 "Compiler/parser.y"
                                                                {
										print("funcName ID");

										activeFunctions++;
										if(!isLibFunc(yylval.strValue, getCurrentLine())) {
											if(!lookUpScopeRedeclaration(yylval.strValue, function, scope, getCurrentLine(), Funcdef));
										}

										(yyval.strValue) = (yyvsp[0].strValue);

										print("funcName ID");
									}
#line 2733 "build/parser.c"
    break;

  case 76: /* funcName: %empty  */
#line 1086 "Compiler/parser.y"
                                                        {
										print("funcName /*empty*/");

										activeFunctions++;
									 	(yyval.strValue) = genName();

									 	print("funcName /*empty*/");
									}
#line 2746 "build/parser.c"
    break;

  case 77: /* funcPre: FUNCTION funcName  */
#line 1096 "Compiler/parser.y"
                                        {
									print("funcPre FUNCTION funcName");

									(yyval.symbolTable) = insert((yyvsp[0].strValue), function, USER_FUNC, scope, getCurrentLine(), 1, -1, -1);
									(yyval.symbolTable)->value.funcVal->iaddress = nextQuadLabel();

									// jump before function so it doesnt run on declaration
									pushFuncJump(nextQuadLabel());
									emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
									emit(funcstart, NULL, NULL, lvalueExpr((yyval.symbolTable)), nextQuadLabel(), getCurrentLine());

									push(functionLocalOffset);

									resetFormalArgOffset();
									enterScopeSpace();

									scopeOps(INCREASE);
									funcdefStart = 1 ;

									print("funcPre FUNCTION funcName");
								}
#line 2772 "build/parser.c"
    break;

  case 78: /* funcPreArgs: funcPre funcArgs  */
#line 1119 "Compiler/parser.y"
                                {
									print("funcPreArgs funcPre funcArgs");

									(yyvsp[-1].symbolTable)->value.funcVal->totalargs = totalFuncArgs;
									totalFuncArgs = 0;

									print("funcPreArgs funcPre funcArgs");
								}
#line 2785 "build/parser.c"
    break;

  case 79: /* funcBlockStart: %empty  */
#line 1129 "Compiler/parser.y"
                {
					print("funcBlockStart");

					pushLoopCounter(activeLoops);
					activeLoops = 0;

					print("funcBlockStart");
				}
#line 2798 "build/parser.c"
    break;

  case 80: /* funcBlockEnd: %empty  */
#line 1139 "Compiler/parser.y"
                {
 					print("funcBlockEnd");
 					activeLoops = popLoopCounter();
 					print("funcBlockEnd");
 				}
#line 2808 "build/parser.c"
    break;

  case 81: /* funcBody: block  */
#line 1146 "Compiler/parser.y"
                        {
 						print("funcBody block");
 						exitScopeSpace();
 						print("funcBody block");
 					}
#line 2818 "build/parser.c"
    break;

  case 82: /* funcdef: funcPreArgs funcBlockStart funcBody funcBlockEnd  */
#line 1153 "Compiler/parser.y"
                                                                        {
																	print("funcdef funcPreArgs funcBlockStart funcBody funcBlockEnd");

																	// Here we will put the default value for non defined functions
																	patchLabel(popFuncJump(), nextQuadLabel() + 1);

																	activeFunctions--;
																	exitScopeSpace();
																	(yyvsp[-3].symbolTable)->value.funcVal->totallocals = functionLocalOffset;

																	functionLocalOffset = pop();

																	(yyval.symbolTable) = (yyvsp[-3].symbolTable);
																	emit(funcend, NULL, NULL, lvalueExpr((yyval.symbolTable)), nextQuadLabel(), getCurrentLine());

																	print("funcdef funcPreArgs funcBlockStart funcBody funcBlockEnd");
																}
#line 2840 "build/parser.c"
    break;

  case 83: /* const: NUMBER  */
#line 1174 "Compiler/parser.y"
                                                {
 									print("const NUMBER");
 									(yyval.exprValue) = newExprConstNumber((yyvsp[0].numVal));
 									print("const NUMBER");
 								}
#line 2850 "build/parser.c"
    break;

  case 84: /* const: STRING  */
#line 1179 "Compiler/parser.y"
                                                        {
			 						print("const STRING");

			 						char* pure = strtok((yyvsp[0].strValue), "\"");
								  	(yyval.exprValue) = newExprConstString(pure);

								  	print("const STRING");
								}
#line 2863 "build/parser.c"
    break;

  case 85: /* const: NIL  */
#line 1187 "Compiler/parser.y"
                                                        {
			 						print("const NIL");
			 						(yyval.exprValue) = newExprNil();
			 						print("const NIL");
			 					}
#line 2873 "build/parser.c"
    break;

  case 86: /* const: TRUE  */
#line 1192 "Compiler/parser.y"
                                                        {
			 						print("const TRUE");
			 						(yyval.exprValue) = newExprConstBool('1');
			 						print("const TRUE");
			 					}
#line 2883 "build/parser.c"
    break;

  case 87: /* const: FALSE  */
#line 1197 "Compiler/parser.y"
                                                        {
									print("const FALSE");
									(yyval.exprValue) = newExprConstBool('0');
									print("const FALSE");
								}
#line 2893 "build/parser.c"
    break;

  case 88: /* idlist: ID  */
#line 1204 "Compiler/parser.y"
                                                        {
									print("idlist ID");

									if(!formalShadow(yylval.strValue, getCurrentLine()))
										insert(yylval.strValue, variable, FORMAL, scope, getCurrentLine(), 1, currentScopeSpace(), currentScopeOffset());

									increaseCurrentScopeOffset();
									totalFuncArgs++;

									print("idlist ID");
								}
#line 2909 "build/parser.c"
    break;

  case 89: /* idlist: idlist comma ID  */
#line 1215 "Compiler/parser.y"
                                                {
									print("idlist idlist,ID");

									totalFuncArgs++;

									if(!formalShadow(yylval.strValue, getCurrentLine()) && !formalRedeclaration(yylval.strValue, scope, getCurrentLine())) {
										insert(yylval.strValue, variable, FORMAL, scope, getCurrentLine(), 1, currentScopeSpace(), currentScopeOffset());
										increaseCurrentScopeOffset();
									}

									print("idlist idlist,ID");
								}
#line 2926 "build/parser.c"
    break;

  case 90: /* idlist: %empty  */
#line 1227 "Compiler/parser.y"
                                                { print("idlist /*empty*/"); }
#line 2932 "build/parser.c"
    break;

  case 91: /* ifPrefix: IF openPar expr closePar  */
#line 1232 "Compiler/parser.y"
                                                {
											print("ifPrefix IF(expr)");

											if((yyvsp[-1].exprValue)->type == boolexpr_e) {
												(yyvsp[-1].exprValue)->sym = newTmp(scope);

												backPatchList((yyvsp[-1].exprValue)->trueList, nextQuadLabel());
												emit(assign, newExprConstBool('1'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
												emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());
												backPatchList((yyvsp[-1].exprValue)->falseList, nextQuadLabel());
												emit(assign, newExprConstBool('0'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
											}

											emit(if_eq, newExprConstBool('1'), (yyvsp[-1].exprValue), newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());
											(yyval.intVal) = nextQuadLabel();
											emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());

											print("ifPrefix IF(expr)");
										}
#line 2956 "build/parser.c"
    break;

  case 92: /* elsePrefix: ELSE  */
#line 1253 "Compiler/parser.y"
                        {
						print("elsePrefix ELSE");

						(yyval.intVal) = nextQuadLabel();
						emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());

						print("elsePrefix ELSE");
					}
#line 2969 "build/parser.c"
    break;

  case 93: /* ifstmt: ifPrefix stmt  */
#line 1263 "Compiler/parser.y"
                                                        {
											print("ifstmt ifPrefix stmt");

											patchLabel((yyvsp[-1].intVal), nextQuadLabel());
											(yyval.intVal) = (yyvsp[-1].intVal);

											print("ifstmt ifPrefix stmt");
										}
#line 2982 "build/parser.c"
    break;

  case 94: /* ifstmt: ifstmt elsePrefix stmt  */
#line 1271 "Compiler/parser.y"
                                                        {
											print("ifstmt ifstmt elseprefix stmt");

											patchLabel((yyvsp[-2].intVal), (yyvsp[-1].intVal) + 1);
											patchLabel((yyvsp[-1].intVal), nextQuadLabel());

											print("ifstmt ifstmt elseprefix stmt");
										}
#line 2995 "build/parser.c"
    break;

  case 95: /* loopStart: %empty  */
#line 1285 "Compiler/parser.y"
                {
				print("loopStart");

				activeLoops++;
				loopPush();

				print("loopStart");
			}
#line 3008 "build/parser.c"
    break;

  case 96: /* loopEnd: %empty  */
#line 1295 "Compiler/parser.y"
                {
 				print("loopEnd");

 				activeLoops--;
 				// loopPop will happen below at stmt

 				print("loopEnd");
			}
#line 3021 "build/parser.c"
    break;

  case 97: /* loopstmt: loopStart stmt loopEnd  */
#line 1305 "Compiler/parser.y"
                                        {
										print("loopstmt");

										struct LoopStack *tmp = loopPop();
										assert(tmp);
										(yyval.loopStack) = tmp;

										print("loopstmt");
									}
#line 3035 "build/parser.c"
    break;

  case 98: /* whileStart: WHILE  */
#line 1321 "Compiler/parser.y"
                        {
 						print("whileStart WHILE");
 						(yyval.intVal) = nextQuadLabel();
 						print("whileStart WHILE");
 					}
#line 3045 "build/parser.c"
    break;

  case 99: /* whileCond: openPar expr closePar  */
#line 1328 "Compiler/parser.y"
                                                        {
												print("whileCond (expr)");

												if((yyvsp[-1].exprValue)->type == boolexpr_e) {
													(yyvsp[-1].exprValue)->sym = newTmp(scope);

													backPatchList((yyvsp[-1].exprValue)->trueList, nextQuadLabel());
													emit(assign, newExprConstBool('1'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
													emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

													backPatchList((yyvsp[-1].exprValue)->falseList, nextQuadLabel());
													emit(assign, newExprConstBool('0'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
												}

												emit(if_eq, (yyvsp[-1].exprValue), newExprConstBool('1'), newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());
												(yyval.intVal) = nextQuadLabel();
												emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());

												print("whileCond (expr)");
											}
#line 3070 "build/parser.c"
    break;

  case 100: /* whilestmt: whileStart whileCond loopstmt  */
#line 1350 "Compiler/parser.y"
                                                {
												print("whilestmt whileStartWhileCond loopstmt");

												emit(jump, NULL, NULL, newExprConstNumber((yyvsp[-2].intVal)), nextQuadLabel(), getCurrentLine());
												patchLabel((yyvsp[-1].intVal), nextQuadLabel());

												struct BCList *blist = (yyvsp[0].loopStack)->breakList;
												while(blist != NULL) {
													patchLabel(blist->QuadNo, nextQuadLabel());
													blist = blist->next;
												}

												struct BCList *clist = (yyvsp[0].loopStack)->contList;
												while(clist!=NULL) {
													patchLabel(clist->QuadNo, (yyvsp[-2].intVal));
													clist = clist->next;
												}

												print("whilestmt whileStartWhileCond loopstmt");
											}
#line 3095 "build/parser.c"
    break;

  case 101: /* N: %empty  */
#line 1376 "Compiler/parser.y"
                {
			print("N");

			(yyval.intVal) = nextQuadLabel();
			emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());

			print("N");
		}
#line 3108 "build/parser.c"
    break;

  case 102: /* M: %empty  */
#line 1384 "Compiler/parser.y"
                {
 			print("M");
 			(yyval.intVal) = nextQuadLabel();
 			print("M");
 		}
#line 3118 "build/parser.c"
    break;

  case 103: /* forPrefix: FOR openPar elist M semicolon expr semicolon  */
#line 1390 "Compiler/parser.y"
                                                                {
				print("forPrefix FOR(elist M; expr;");

				if((yyvsp[-1].exprValue)->type == boolexpr_e) {
					(yyvsp[-1].exprValue)->sym = newTmp(scope);

					backPatchList((yyvsp[-1].exprValue)->trueList, nextQuadLabel());
					emit(assign, newExprConstBool('1'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
					emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

					backPatchList((yyvsp[-1].exprValue)->falseList, nextQuadLabel());
					emit(assign, newExprConstBool('0'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
				}

				struct ForLoop_S *flv = malloc(sizeof(struct ForLoop_S));
				(yyval.forLoopVal) = flv;
				(yyval.forLoopVal)->test = (yyvsp[-3].intVal);
				(yyval.forLoopVal)->enter = nextQuadLabel();
				emit(if_eq, (yyvsp[-1].exprValue), newExprConstBool('1'), NULL, nextQuadLabel(), getCurrentLine());

				print("forPrefix FOR(elist M; expr;");
			}
#line 3145 "build/parser.c"
    break;

  case 104: /* forstmt: forPrefix N elist closePar N loopstmt N  */
#line 1414 "Compiler/parser.y"
                                                                {
				print("forstmt forPrefix N elist) N loopstmt N");

				patchLabel((yyvsp[-6].forLoopVal)->enter, (yyvsp[-2].intVal) + 1);
				patchLabel((yyvsp[-5].intVal), nextQuadLabel());
				patchLabel((yyvsp[-2].intVal), (yyvsp[-6].forLoopVal)->test);
				patchLabel((yyvsp[0].intVal), (yyvsp[-5].intVal) + 1);

				struct BCList *blist = (yyvsp[-1].loopStack)->breakList;
				while(blist != NULL) {
					patchLabel(blist->QuadNo, nextQuadLabel());
					blist = blist->next;
				}

				struct BCList *clist = (yyvsp[-1].loopStack)->contList;
				while(clist != NULL) {
					patchLabel(clist->QuadNo, (yyvsp[-5].intVal) + 1);
					clist = clist->next;
				}

				print("forstmt forPrefix N elist) N loopstmt N");
			}
#line 3172 "build/parser.c"
    break;

  case 105: /* returnstmt: RETURN semicolon  */
#line 1440 "Compiler/parser.y"
                                                {
										print("returnstmt RETURN;");

										if(activeFunctions == 0){
											printf("Error at line %d, return outside of function.\n", getCurrentLine());
											encounteredError = 1;
										}

										emit(ret, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

										print("returnstmt RETURN;");
									}
#line 3190 "build/parser.c"
    break;

  case 106: /* returnstmt: RETURN expr semicolon  */
#line 1453 "Compiler/parser.y"
                                                {
										print("returnstmt RETURN expr;");

										if((yyvsp[-1].exprValue)->type == boolexpr_e) {
											(yyvsp[-1].exprValue)->sym = newTmp(scope);

											backPatchList((yyvsp[-1].exprValue)->trueList, nextQuadLabel());
											emit(assign, newExprConstBool('1'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
											emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

											backPatchList((yyvsp[-1].exprValue)->falseList, nextQuadLabel());
											emit(assign, newExprConstBool('0'), NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
										}

										if(activeFunctions == 0) {
											printf("Error at line %d, return outside of function.\n", getCurrentLine());
											encounteredError = 1;
										}

										emit(ret, NULL, NULL, (yyvsp[-1].exprValue), nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());
										(yyval.exprValue) = (yyvsp[-1].exprValue);

										print("returnstmt RETURN expr;");
									}
#line 3220 "build/parser.c"
    break;


#line 3224 "build/parser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1480 "Compiler/parser.y"


int yyerror(char* yaccProvidedMessage){
    fprintf(stderr, "%s", yaccProvidedMessage);
    fprintf(stderr, "at line %d,  before token: %s\n", yylineno, yytext);
    fprintf(stderr, "INPUT NOT VALID\n");
    encounteredError = 1;
    return -1 ;
}

char *genName() {
	return giveName("$$func_def_", noNameFunctionNum++);
}

void scopeOps(enum ScopeAction action) {
	if(action == INCREASE) {
		if(!funcdefStart)
			scope++;
		else
			funcdefStart = 0;
	} else
		scope--;
}

char* getStringValueQuad(struct expr *e, int isResult) {
	if(e == NULL)
		return "";
	else if (e->type == constnum_e) {
		char *r = malloc(sizeof(numberOfDigits(e->numConst)) + 1);

		// The quad we are going to jump is better to be represented as int in print.
		if(isResult)
			sprintf(r, "%d", (int) e->numConst);
		else {
			double decPart;
			decPart = modf(e->numConst, &decPart);

			if(decPart == 0)
				sprintf(r, "%d", (int) e->numConst);
			else
				sprintf(r, "%lf", e->numConst);
		}
		return r;
	}
	else if(e->type == conststring_e)
		return strdup(e->strConst);
	else if(e->type == constbool_e) {
		if(e->boolConst == '1')
			return "TRUE";
		else
			return "FALSE";
	}
	else if(e->type == nil_e)
		return "nil";

	return strdup(e->sym->value.varVal->name);
}

void printAllQuads() {
	if(encounteredError == 1) {
		print("Finsihed with errors, no quad file will be generated");
		return;
	}

	FILE *file = fopen("quads.txt", "w");
	if(!file) {
		printf("Cannot open quads.txt\n");
		return;
	}

	fprintf(file,"\n%-s%-16s%-15s%-15s%-s\n",
				"#|\t", "OP|", "ARG1|", "ARG2|", "LBL|");
	fprintf(file, "----------------------------------------------------------\n");

	for(int i = 0; i < nextQuadLabel(); i++) {
		fprintf(file, "%-d%-s%-12s\t", quads[i].label, ":\t", opcodeToString[quads[i].op]);
		fprintf(file, "%-15s%-15s%-15s\n",
					getStringValueQuad(quads[i].arg1,0), getStringValueQuad(quads[i].arg2, 0), getStringValueQuad(quads[i].result, 1));
	}

	fclose(file);
}

void print(char *message) {
	printf("Message (line %d): %s\n", getCurrentLine(), message);
}

void makeBinaryFile(char *name) {
	if(encounteredError == 1) {
		print("Finsihed with errors, no binary file will be generated");
		return;
	}

	createBinaryFile(name);
}

int main(int argc, char **argv) {
    FILE *f_in = NULL;
    FILE *f_out = NULL;

    if (argc > 1) {
        if (!(f_in = fopen(argv[1], "r"))) {
            fprintf(stderr, "Cannot open input file: %s\n", argv[1]);
            return 1;
        }
    }

    if (argc > 2) {
        if (!(f_out = fopen(argv[2], "w"))) {
            fprintf(stderr, "Cannot open output file: %s\n", argv[2]);
            return 1;
        }
    }

    yyin = f_in != NULL ? f_in : stdin;
	yyout = f_out != NULL ? f_out : stdout;

	initIntermediate();
	initTable();
    addBuiltinFunctionsTo();

	yyparse();

	emit(no_op, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
	printAllQuads();

	generateTcode(nextQuadLabel());
	makeBinaryFile("a.bin");

    if (argc > 1) fclose(f_in);
    if (argc > 2) fclose(f_out);

    return 0;
}
