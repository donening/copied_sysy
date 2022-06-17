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
#line 1 "parser.y"

	#include "common.h"
	#define YYSTYPE TreeNode *

	TreeNode* root = new TreeNode(0, NODE_PROG);
	extern int lineno;

	extern bool parserError;

	// max_scope_id 是堆栈下一层结点的最大编号
	unsigned char max_scope_id = SCOPT_ID_BASE;
	string presentScope = "1";
	unsigned int top = 0;

	// multimap <标识符名称， 作用域> 变量名列表
	extern multimap<string, string> idNameList;
	// map <<标识符名称， 作用域>, 结点指针> 变量列表
	extern map<pair<string, string>, TreeNode*> idList;
    // map<函数名， 返回类型> 函数名列表
    extern map<string, Type*> FuncList;

	// 用于检查continue和break是否在循环内部
	bool inCycle = false;

	int yylex();
	int yyerror( char const * );
	int scopeCmp(string preScope, string varScope);
	void scopePush();
	void scopePop();

#line 102 "parser.cpp"

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
  YYSYMBOL_T_CHAR = 3,                     /* T_CHAR  */
  YYSYMBOL_T_INT = 4,                      /* T_INT  */
  YYSYMBOL_T_VOID = 5,                     /* T_VOID  */
  YYSYMBOL_ADDR = 6,                       /* ADDR  */
  YYSYMBOL_ASSIGN = 7,                     /* ASSIGN  */
  YYSYMBOL_PLUSASSIGN = 8,                 /* PLUSASSIGN  */
  YYSYMBOL_MINUSASSIGN = 9,                /* MINUSASSIGN  */
  YYSYMBOL_MULASSIGN = 10,                 /* MULASSIGN  */
  YYSYMBOL_DIVASSIGN = 11,                 /* DIVASSIGN  */
  YYSYMBOL_SEMICOLON = 12,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 13,                     /* COMMA  */
  YYSYMBOL_LPAREN = 14,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 15,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 16,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 17,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 18,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 19,                  /* RBRACKET  */
  YYSYMBOL_CONST = 20,                     /* CONST  */
  YYSYMBOL_IF_ = 21,                       /* IF_  */
  YYSYMBOL_ELSE = 22,                      /* ELSE  */
  YYSYMBOL_WHILE_ = 23,                    /* WHILE_  */
  YYSYMBOL_BREAK = 24,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 25,                  /* CONTINUE  */
  YYSYMBOL_RETURN = 26,                    /* RETURN  */
  YYSYMBOL_EQ = 27,                        /* EQ  */
  YYSYMBOL_GRAEQ = 28,                     /* GRAEQ  */
  YYSYMBOL_LESEQ = 29,                     /* LESEQ  */
  YYSYMBOL_NEQ = 30,                       /* NEQ  */
  YYSYMBOL_GRA = 31,                       /* GRA  */
  YYSYMBOL_LES = 32,                       /* LES  */
  YYSYMBOL_PLUS = 33,                      /* PLUS  */
  YYSYMBOL_MINUS = 34,                     /* MINUS  */
  YYSYMBOL_MUL = 35,                       /* MUL  */
  YYSYMBOL_DIV = 36,                       /* DIV  */
  YYSYMBOL_MOD = 37,                       /* MOD  */
  YYSYMBOL_AND = 38,                       /* AND  */
  YYSYMBOL_OR = 39,                        /* OR  */
  YYSYMBOL_NOT = 40,                       /* NOT  */
  YYSYMBOL_INC = 41,                       /* INC  */
  YYSYMBOL_DEC = 42,                       /* DEC  */
  YYSYMBOL_IDENTIFIER = 43,                /* IDENTIFIER  */
  YYSYMBOL_INTEGER = 44,                   /* INTEGER  */
  YYSYMBOL_YYACCEPT = 45,                  /* $accept  */
  YYSYMBOL_program = 46,                   /* program  */
  YYSYMBOL_basicType = 47,                 /* basicType  */
  YYSYMBOL_literalConst = 48,              /* literalConst  */
  YYSYMBOL_compIdentifier = 49,            /* compIdentifier  */
  YYSYMBOL_pIdentifier = 50,               /* pIdentifier  */
  YYSYMBOL_arrayIdentifier = 51,           /* arrayIdentifier  */
  YYSYMBOL_identifier = 52,                /* identifier  */
  YYSYMBOL_declCompIdentifier = 53,        /* declCompIdentifier  */
  YYSYMBOL_pDeclIdentifier = 54,           /* pDeclIdentifier  */
  YYSYMBOL_arrayDeclIdentifier = 55,       /* arrayDeclIdentifier  */
  YYSYMBOL_declIdentifier = 56,            /* declIdentifier  */
  YYSYMBOL_decl = 57,                      /* decl  */
  YYSYMBOL_varDecl = 58,                   /* varDecl  */
  YYSYMBOL_varDefs = 59,                   /* varDefs  */
  YYSYMBOL_varDef = 60,                    /* varDef  */
  YYSYMBOL_funcDef = 61,                   /* funcDef  */
  YYSYMBOL_funcLPAREN = 62,                /* funcLPAREN  */
  YYSYMBOL_funcFParams = 63,               /* funcFParams  */
  YYSYMBOL_funcFParam = 64,                /* funcFParam  */
  YYSYMBOL_block = 65,                     /* block  */
  YYSYMBOL_blockLBRACE = 66,               /* blockLBRACE  */
  YYSYMBOL_blockRBRACE = 67,               /* blockRBRACE  */
  YYSYMBOL_blockItems = 68,                /* blockItems  */
  YYSYMBOL_blockItem = 69,                 /* blockItem  */
  YYSYMBOL_stmt_ = 70,                     /* stmt_  */
  YYSYMBOL_stmt = 71,                      /* stmt  */
  YYSYMBOL_IF = 72,                        /* IF  */
  YYSYMBOL_WHILE = 73,                     /* WHILE  */
  YYSYMBOL_expr = 74,                      /* expr  */
  YYSYMBOL_cond = 75,                      /* cond  */
  YYSYMBOL_addExpr = 76,                   /* addExpr  */
  YYSYMBOL_mulExpr = 77,                   /* mulExpr  */
  YYSYMBOL_unaryExpr = 78,                 /* unaryExpr  */
  YYSYMBOL_primaryExpr = 79,               /* primaryExpr  */
  YYSYMBOL_funcRParams = 80,               /* funcRParams  */
  YYSYMBOL_LOrExpr = 81,                   /* LOrExpr  */
  YYSYMBOL_LAndExpr = 82,                  /* LAndExpr  */
  YYSYMBOL_eqExpr = 83,                    /* eqExpr  */
  YYSYMBOL_relExpr = 84                    /* relExpr  */
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
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   288

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  96
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  172

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   299


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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    61,    61,    62,    63,    64,    70,    71,    75,    80,
      81,    90,    91,    92,   103,   133,   167,   196,   197,   201,
     202,   203,   208,   215,   223,   242,   246,   271,   272,   276,
     282,   305,   322,   325,   326,   330,   341,   347,   348,   351,
     352,   356,   357,   361,   366,   370,   371,   372,   373,   384,
     394,   405,   413,   421,   422,   425,   426,   431,   432,   442,
     448,   454,   460,   469,   473,   474,   475,   480,   481,   482,
     483,   488,   489,   490,   491,   492,   493,   498,   499,   500,
     501,   509,   517,   522,   523,   528,   529,   534,   535,   540,
     541,   542,   547,   548,   549,   550,   551
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
  "\"end of file\"", "error", "\"invalid token\"", "T_CHAR", "T_INT",
  "T_VOID", "ADDR", "ASSIGN", "PLUSASSIGN", "MINUSASSIGN", "MULASSIGN",
  "DIVASSIGN", "SEMICOLON", "COMMA", "LPAREN", "RPAREN", "LBRACE",
  "RBRACE", "LBRACKET", "RBRACKET", "CONST", "IF_", "ELSE", "WHILE_",
  "BREAK", "CONTINUE", "RETURN", "EQ", "GRAEQ", "LESEQ", "NEQ", "GRA",
  "LES", "PLUS", "MINUS", "MUL", "DIV", "MOD", "AND", "OR", "NOT", "INC",
  "DEC", "IDENTIFIER", "INTEGER", "$accept", "program", "basicType",
  "literalConst", "compIdentifier", "pIdentifier", "arrayIdentifier",
  "identifier", "declCompIdentifier", "pDeclIdentifier",
  "arrayDeclIdentifier", "declIdentifier", "decl", "varDecl", "varDefs",
  "varDef", "funcDef", "funcLPAREN", "funcFParams", "funcFParam", "block",
  "blockLBRACE", "blockRBRACE", "blockItems", "blockItem", "stmt_", "stmt",
  "IF", "WHILE", "expr", "cond", "addExpr", "mulExpr", "unaryExpr",
  "primaryExpr", "funcRParams", "LOrExpr", "LAndExpr", "eqExpr", "relExpr", YY_NULLPTR
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
     295,   296,   297,   298,   299
};
#endif

#define YYPACT_NINF (-149)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     139,  -149,  -149,    45,    -3,  -149,  -149,  -149,  -149,  -149,
    -149,    -3,    -3,  -149,  -149,    53,    17,  -149,   156,  -149,
    -149,  -149,  -149,    -2,    29,     7,  -149,    -3,     1,    59,
      -3,    96,  -149,    70,    76,  -149,  -149,   191,  -149,   139,
      83,  -149,    84,  -149,   230,  -149,  -149,  -149,    94,   106,
       3,    25,    25,    84,   230,  -149,  -149,    -3,  -149,   274,
     205,    86,  -149,  -149,  -149,   191,    91,  -149,  -149,   109,
     115,   136,   160,   231,  -149,  -149,  -149,   191,  -149,   137,
     146,  -149,   124,   135,   248,  -149,  -149,  -149,   142,  -149,
      56,  -149,  -149,  -149,  -149,  -149,   230,   230,   230,   230,
     230,    58,   230,  -149,  -149,   230,   116,  -149,  -149,   230,
     230,  -149,   230,   230,   230,   230,   230,   141,  -149,  -149,
     230,   230,   230,   230,   230,   230,   230,   230,  -149,  -149,
    -149,  -149,  -149,  -149,  -149,  -149,   123,   158,   167,  -149,
    -149,   164,   173,   231,   231,  -149,  -149,  -149,  -149,  -149,
    -149,  -149,  -149,  -149,  -149,  -149,  -149,   230,  -149,  -149,
    -149,   216,   216,  -149,   191,   176,  -149,  -149,   166,   216,
    -149,  -149
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     6,     7,     0,     0,     2,    25,     3,     1,     4,
       5,     0,     0,    24,    29,    17,    18,    19,     0,    27,
      21,    20,    32,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    33,     0,    17,    28,    22,     0,    35,     0,
       0,    23,     0,    45,     0,    37,    55,    56,     0,     0,
       0,     0,     0,     0,     0,    16,     8,     0,    77,    82,
       9,    10,    11,    41,    47,     0,     0,    39,    42,     0,
       0,     0,    57,    64,    67,    71,    34,     0,    13,    92,
       0,    63,    85,    87,    89,    51,    52,    53,     0,    82,
       9,    73,    74,    12,    92,    72,     0,     0,     0,     0,
       0,     0,     0,    75,    76,     0,     0,    31,    40,     0,
       0,    46,     0,     0,     0,     0,     0,     0,    78,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    58,
      59,    60,    61,    62,    81,    83,     0,     0,     0,    38,
      36,     0,     0,    65,    66,    68,    69,    70,    30,    86,
      88,    90,    95,    96,    91,    93,    94,     0,    80,    14,
      15,     0,     0,    84,     0,    49,    44,    50,     0,     0,
      43,    48
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -149,  -149,     2,  -149,   -28,   -24,  -149,  -149,  -149,     0,
    -149,  -149,    16,  -149,  -149,   175,   201,  -149,  -149,   169,
    -149,  -149,  -149,   -64,   -58,  -148,    92,  -149,  -149,   -44,
     -47,  -149,   108,   172,   206,  -149,    93,    90,  -100,  -149
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,    57,    58,    59,    60,    61,    62,    14,    34,
      16,    17,    63,     6,    18,    19,     7,    24,    31,    32,
      64,    65,   140,    66,    67,   165,    68,    69,    70,    71,
      80,    72,    73,    74,    75,   136,    81,    82,    83,    84
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      79,   106,     4,    11,    15,     4,    88,    95,   108,    42,
      94,    20,    21,   117,   167,    87,     5,    44,    78,     9,
      36,   171,   151,    89,    89,   154,    30,    90,    90,    93,
      38,    42,    12,     1,     2,    25,    51,    52,    53,    44,
      13,    30,    28,    54,    29,     8,    55,    56,   108,     1,
       2,    33,   129,   130,   131,   132,   133,   135,   137,   108,
      53,   138,   141,   142,    42,    94,    94,    22,    55,    56,
     101,    23,    44,   134,   102,    37,    94,    94,    94,   152,
     153,    94,   155,   156,    89,    89,    89,    89,    89,    41,
      42,    51,    52,    53,    23,     1,     2,    42,    54,    77,
     168,    55,    56,    43,   105,    44,    85,    45,   107,    39,
     108,    40,    46,   163,    47,    48,    49,    50,    86,    53,
       1,     2,    42,   109,    51,    52,    53,    55,    43,   110,
      44,    54,    45,   139,    55,    56,   157,    46,   158,    47,
      48,    49,    50,     1,     2,     1,     2,    42,   111,    51,
      52,    53,   118,    43,   128,    44,    54,    45,   148,    55,
      56,   119,    46,   120,    47,    48,    49,    50,    26,    27,
       1,     2,    42,   121,    51,    52,    53,   159,    43,   161,
      44,    54,    45,   170,    55,    56,   160,    46,   162,    47,
      48,    49,    50,   112,   113,     1,     2,    42,   169,    51,
      52,    53,    35,    43,    10,    44,    54,    45,    76,    55,
      56,   150,    46,   149,    47,    48,    49,    50,     0,   101,
     143,   144,    42,   102,    51,    52,    53,     0,    43,     0,
      44,    54,   164,     0,    55,    56,    42,    46,     0,    47,
      48,    49,    50,     0,    44,     0,   103,   104,     0,    51,
      52,    53,     0,   166,   166,     0,    54,    91,    92,    55,
      56,   166,     0,    51,    52,    53,   114,   115,   116,     0,
      54,     0,     0,    55,    56,   122,   123,   124,   125,   126,
     127,    96,    97,    98,    99,   100,   145,   146,   147
};

static const yytype_int16 yycheck[] =
{
      44,    65,     0,     6,     4,     3,    50,    54,    66,     6,
      54,    11,    12,    77,   162,    12,     0,    14,    42,     3,
      19,   169,   122,    51,    52,   125,    24,    51,    52,    53,
      30,     6,    35,     4,     5,    18,    33,    34,    35,    14,
      43,    39,    44,    40,    15,     0,    43,    44,   106,     4,
       5,    44,    96,    97,    98,    99,   100,   101,   102,   117,
      35,   105,   109,   110,     6,   109,   110,    14,    43,    44,
      14,    18,    14,    15,    18,    16,   120,   121,   122,   123,
     124,   125,   126,   127,   112,   113,   114,   115,   116,    19,
       6,    33,    34,    35,    18,     4,     5,     6,    40,    16,
     164,    43,    44,    12,    18,    14,    12,    16,    17,    13,
     168,    15,    21,   157,    23,    24,    25,    26,    12,    35,
       4,     5,     6,    14,    33,    34,    35,    43,    12,    14,
      14,    40,    16,    17,    43,    44,    13,    21,    15,    23,
      24,    25,    26,     4,     5,     4,     5,     6,    12,    33,
      34,    35,    15,    12,    12,    14,    40,    16,    17,    43,
      44,    15,    21,    39,    23,    24,    25,    26,    12,    13,
       4,     5,     6,    38,    33,    34,    35,    19,    12,    15,
      14,    40,    16,    17,    43,    44,    19,    21,    15,    23,
      24,    25,    26,    33,    34,     4,     5,     6,    22,    33,
      34,    35,    27,    12,     3,    14,    40,    16,    39,    43,
      44,   121,    21,   120,    23,    24,    25,    26,    -1,    14,
     112,   113,     6,    18,    33,    34,    35,    -1,    12,    -1,
      14,    40,    16,    -1,    43,    44,     6,    21,    -1,    23,
      24,    25,    26,    -1,    14,    -1,    41,    42,    -1,    33,
      34,    35,    -1,   161,   162,    -1,    40,    51,    52,    43,
      44,   169,    -1,    33,    34,    35,    35,    36,    37,    -1,
      40,    -1,    -1,    43,    44,    27,    28,    29,    30,    31,
      32,     7,     8,     9,    10,    11,   114,   115,   116
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,     5,    46,    47,    57,    58,    61,     0,    57,
      61,     6,    35,    43,    53,    54,    55,    56,    59,    60,
      54,    54,    14,    18,    62,    18,    12,    13,    44,    15,
      47,    63,    64,    44,    54,    60,    19,    16,    54,    13,
      15,    19,     6,    12,    14,    16,    21,    23,    24,    25,
      26,    33,    34,    35,    40,    43,    44,    47,    48,    49,
      50,    51,    52,    57,    65,    66,    68,    69,    71,    72,
      73,    74,    76,    77,    78,    79,    64,    16,    50,    74,
      75,    81,    82,    83,    84,    12,    12,    12,    74,    49,
      50,    79,    79,    50,    74,    75,     7,     8,     9,    10,
      11,    14,    18,    41,    42,    18,    68,    17,    69,    14,
      14,    12,    33,    34,    35,    36,    37,    68,    15,    15,
      39,    38,    27,    28,    29,    30,    31,    32,    12,    74,
      74,    74,    74,    74,    15,    74,    80,    74,    74,    17,
      67,    75,    75,    77,    77,    78,    78,    78,    17,    81,
      82,    83,    74,    74,    83,    74,    74,    13,    15,    19,
      19,    15,    15,    74,    16,    70,    71,    70,    68,    22,
      17,    70
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    45,    46,    46,    46,    46,    47,    47,    48,    49,
      49,    50,    50,    50,    51,    51,    52,    53,    53,    54,
      54,    54,    55,    55,    56,    57,    58,    59,    59,    60,
      61,    61,    62,    63,    63,    64,    65,    66,    67,    68,
      68,    69,    69,    70,    70,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    72,    73,    74,    74,    74,
      74,    74,    74,    75,    76,    76,    76,    77,    77,    77,
      77,    78,    78,    78,    78,    78,    78,    79,    79,    79,
      79,    79,    79,    80,    80,    81,    81,    82,    82,    83,
      83,    83,    84,    84,    84,    84,    84
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     2,     2,     4,     4,     1,     1,     1,     1,
       2,     2,     4,     4,     1,     1,     3,     1,     3,     1,
       8,     7,     1,     1,     3,     2,     3,     1,     1,     1,
       2,     1,     1,     3,     1,     1,     2,     1,     7,     5,
       5,     2,     2,     2,     3,     1,     1,     1,     3,     3,
       3,     3,     3,     1,     1,     3,     3,     1,     3,     3,
       3,     1,     2,     2,     2,     2,     2,     1,     3,     3,
       4,     3,     1,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3
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
  case 2: /* program: decl  */
#line 61 "parser.y"
       {root->addChild(yyvsp[0]);}
#line 1326 "parser.cpp"
    break;

  case 3: /* program: funcDef  */
#line 62 "parser.y"
          {root->addChild(yyvsp[0]);}
#line 1332 "parser.cpp"
    break;

  case 4: /* program: program decl  */
#line 63 "parser.y"
               {root->addChild(yyvsp[0]);}
#line 1338 "parser.cpp"
    break;

  case 5: /* program: program funcDef  */
#line 64 "parser.y"
                  {root->addChild(yyvsp[0]);}
#line 1344 "parser.cpp"
    break;

  case 6: /* basicType: T_INT  */
#line 70 "parser.y"
        {yyval = new TreeNode(lineno, NODE_TYPE); yyval->type = TYPE_INT;}
#line 1350 "parser.cpp"
    break;

  case 7: /* basicType: T_VOID  */
#line 71 "parser.y"
         {yyval = new TreeNode(lineno, NODE_TYPE); yyval->type = TYPE_VOID;}
#line 1356 "parser.cpp"
    break;

  case 8: /* literalConst: INTEGER  */
#line 75 "parser.y"
          {yyval = new TreeNode(lineno, NODE_EXPR); yyval->addChild(yyvsp[0]);}
#line 1362 "parser.cpp"
    break;

  case 9: /* compIdentifier: pIdentifier  */
#line 80 "parser.y"
              {yyval = yyvsp[0];}
#line 1368 "parser.cpp"
    break;

  case 10: /* compIdentifier: arrayIdentifier  */
#line 81 "parser.y"
                  {
	yyval = yyvsp[0];
	// 归约完成，清除下标计数器，为下一次做准备
	yyval->child->type->visitDim = 0;
  }
#line 1378 "parser.cpp"
    break;

  case 11: /* pIdentifier: identifier  */
#line 90 "parser.y"
             {yyval = new TreeNode(yyvsp[0]);}
#line 1384 "parser.cpp"
    break;

  case 12: /* pIdentifier: MUL pIdentifier  */
#line 91 "parser.y"
                  {yyval = yyvsp[0]; yyval->pointLevel++;}
#line 1390 "parser.cpp"
    break;

  case 13: /* pIdentifier: ADDR pIdentifier  */
#line 92 "parser.y"
                   {
	yyval = yyvsp[0]; 
	yyval->pointLevel--;
	if (yyval->pointLevel < -2) {
		yyerror("Continuous addr operator");
	}
  }
#line 1402 "parser.cpp"
    break;

  case 14: /* arrayIdentifier: pIdentifier LBRACKET expr RBRACKET  */
#line 103 "parser.y"
                                     {
	yyval = new TreeNode(lineno, NODE_OP);
	yyval->optype = OP_INDEX;
	yyval->addChild(yyvsp[-3]);

	// 计算数组偏移量倍数
	int biasRate = 1;
	for (unsigned int i = yyvsp[-3]->type->visitDim + 1; i < yyvsp[-3]->type->dim; i++) {
		biasRate *= yyvsp[-3]->type->dimSize[i];
	}
	TreeNode* biasNode;
	if (biasRate == 1) {
		// 偏移倍数为1时省略乘法结点
		biasNode = yyvsp[-1];
	}
	else {
		biasNode = new TreeNode(lineno, NODE_OP);
		biasNode->optype = OP_MUL;
		biasNode->addChild(yyvsp[-1]);
		TreeNode* biasRateExpr = new TreeNode(lineno, NODE_EXPR);
		TreeNode* biasRateConst = new TreeNode(lineno, NODE_CONST);
		biasRateConst->type = TYPE_INT;
		biasRateConst->int_val = biasRate;
		biasRateExpr->addChild(biasRateConst);
		biasNode->addChild(biasRateExpr);
	}
	yyvsp[-3]->type->visitDim++;

	yyval->addChild(biasNode);
  }
#line 1437 "parser.cpp"
    break;

  case 15: /* arrayIdentifier: arrayIdentifier LBRACKET expr RBRACKET  */
#line 133 "parser.y"
                                         {
	yyval = yyvsp[-3];
	TreeNode* newBiasNode = new TreeNode(lineno, NODE_OP);
	newBiasNode->optype = OP_ADD;
	newBiasNode->addChild(yyval->child->sibling);
	yyval->child->sibling = newBiasNode;

	// 计算数组偏移量倍数
	int biasRate = 1;
	for (unsigned int i = yyval->child->type->visitDim + 1; i < yyval->child->type->dim; i++) {
		biasRate *= yyval->child->type->dimSize[i];
	}

	TreeNode* biasNode;
	if (biasRate == 1) {
		// 偏移倍数为1时省略乘法结点
		biasNode = yyvsp[-1];
	}
	else {
		biasNode->optype = OP_MUL;
		biasNode->addChild(yyvsp[-1]);
		TreeNode* biasRateExpr = new TreeNode(lineno, NODE_EXPR);
		TreeNode* biasRateConst = new TreeNode(lineno, NODE_CONST);
		biasRateConst->type = TYPE_INT;
		biasRateConst->int_val = biasRate;
		biasRateExpr->addChild(biasRateConst);
		biasNode->addChild(biasRateExpr);
	}
	yyval->child->type->visitDim++;
	newBiasNode->addChild(biasNode);
  }
#line 1473 "parser.cpp"
    break;

  case 16: /* identifier: IDENTIFIER  */
#line 167 "parser.y"
             {
	yyval = yyvsp[0];
	int idNameCount = idNameList.count(yyval->var_name);
	int declCnt = 0;
	int minDefDis = MAX_SCOPE_STACK;

	// 搜索变量是否已经声明
	auto it = idNameList.find(yyval->var_name);
	int resScoptCmp;
	while (idNameCount--) {
		resScoptCmp = scopeCmp(presentScope, it->second);
		if (resScoptCmp >= 0){
			// 寻找最近的定义
			if (resScoptCmp < minDefDis) {
				minDefDis = resScoptCmp;
				yyval = idList[make_pair(it->first, it->second)];
			}
			declCnt++;
		}
		it++;
	}
	if (declCnt == 0) {
		string t = "Undeclared identifier :'" + yyvsp[0]->var_name + "', scope : " + to_string(resScoptCmp);
		yyerror(t.c_str());
	}
}
#line 1504 "parser.cpp"
    break;

  case 17: /* declCompIdentifier: pDeclIdentifier  */
#line 196 "parser.y"
                  {yyval = yyvsp[0];}
#line 1510 "parser.cpp"
    break;

  case 18: /* declCompIdentifier: arrayDeclIdentifier  */
#line 197 "parser.y"
                      {yyval = yyvsp[0];}
#line 1516 "parser.cpp"
    break;

  case 19: /* pDeclIdentifier: declIdentifier  */
#line 201 "parser.y"
                 {yyval = yyvsp[0];}
#line 1522 "parser.cpp"
    break;

  case 20: /* pDeclIdentifier: MUL pDeclIdentifier  */
#line 202 "parser.y"
                      {yyval = yyvsp[0]; yyval->type->pointLevel++;}
#line 1528 "parser.cpp"
    break;

  case 21: /* pDeclIdentifier: ADDR pDeclIdentifier  */
#line 203 "parser.y"
                       {yyval = yyvsp[0]; yyval->type->pointLevel--;}
#line 1534 "parser.cpp"
    break;

  case 22: /* arrayDeclIdentifier: pDeclIdentifier LBRACKET INTEGER RBRACKET  */
#line 208 "parser.y"
                                            {
  yyval = yyvsp[-3];
  yyval->type->type = VALUE_ARRAY;
  yyval->type->elementType = yyvsp[-3]->type->type;
  yyval->type->dimSize[yyval->type->dim] = yyvsp[-1]->int_val;
  yyval->type->dim++;
}
#line 1546 "parser.cpp"
    break;

  case 23: /* arrayDeclIdentifier: arrayDeclIdentifier LBRACKET INTEGER RBRACKET  */
#line 215 "parser.y"
                                                {
  yyval = yyvsp[-3];
  yyval->type->dimSize[yyval->type->dim] = yyvsp[-1]->int_val;
  yyval->type->dim++;
}
#line 1556 "parser.cpp"
    break;

  case 24: /* declIdentifier: IDENTIFIER  */
#line 223 "parser.y"
             {
	yyval = yyvsp[0];
	yyval->var_scope = presentScope;
	yyval->type = new Type(NOTYPE);
	#ifdef ID_REDUCE_DEBUG
		cout<<"# $ reduce declIdentifier : "<<yyval->var_name<<", at scope :"<<presentScope<<endl;
	#endif
	if (idList.count(make_pair(yyval->var_name, yyval->var_scope)) != 0) {
		string t = "Redeclared identifier : " + yyval->var_name;
		yyerror(t.c_str());
	}
	idNameList.insert(make_pair(yyval->var_name, yyval->var_scope));
	idList[make_pair(yyval->var_name, yyval->var_scope)] = yyval;
}
#line 1575 "parser.cpp"
    break;

  case 25: /* decl: varDecl  */
#line 242 "parser.y"
          {yyval = yyvsp[0];}
#line 1581 "parser.cpp"
    break;

  case 26: /* varDecl: basicType varDefs SEMICOLON  */
#line 246 "parser.y"
                              {
  yyval = new TreeNode(lineno, NODE_STMT);
  yyval->stype = STMT_DECL;
  yyval->type = TYPE_NONE;
  yyval->addChild(yyvsp[-2]);
  yyval->addChild(yyvsp[-1]);
  TreeNode* p = yyvsp[-1]->child;
  while(p != nullptr) {
	  if (p->nodeType == NODE_OP) {
		  p->child->type->copy(yyvsp[-2]->type);
	  }
	  else {
	  	p->type->copy(yyvsp[-2]->type);
	  }
	  p = p->sibling;
  }
  #ifdef DECL_DEBUG
	cout << "# $ reduce varDecl type = " << yyvsp[-2]->type->getTypeInfo() << endl;
	// $$->printAST();
  #endif
}
#line 1607 "parser.cpp"
    break;

  case 27: /* varDefs: varDef  */
#line 271 "parser.y"
         {yyval = new TreeNode(lineno, NODE_VARLIST); yyval->addChild(yyvsp[0]);}
#line 1613 "parser.cpp"
    break;

  case 28: /* varDefs: varDefs COMMA varDef  */
#line 272 "parser.y"
                       {yyval = yyvsp[-2]; yyval->addChild(yyvsp[0]);}
#line 1619 "parser.cpp"
    break;

  case 29: /* varDef: declCompIdentifier  */
#line 276 "parser.y"
                     {yyval = yyvsp[0];}
#line 1625 "parser.cpp"
    break;

  case 30: /* funcDef: basicType pDeclIdentifier funcLPAREN funcFParams RPAREN LBRACE blockItems RBRACE  */
#line 282 "parser.y"
                                                                                   {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_FUNCDECL;
	yyvsp[-6]->type->type = COMPOSE_FUNCTION;
	TreeNode* param = yyvsp[-4];
	while (param != nullptr) {
		yyvsp[-6]->type->paramType[yyvsp[-6]->type->paramNum] = param->child->type;
		yyvsp[-6]->type->paramNum++;
		param = param->sibling;
	}
    FuncList[yyvsp[-6]->var_name] = yyvsp[-7]->type;
	yyvsp[-6]->type->retType = yyvsp[-7]->type;
	yyval->addChild(yyvsp[-7]);
	yyval->addChild(yyvsp[-6]);
	TreeNode* params = new TreeNode(lineno, NODE_VARLIST);
	params->addChild(yyvsp[-4]);
	yyval->addChild(params);
	TreeNode* funcBlock = new TreeNode(lineno, NODE_STMT);
	funcBlock->stype = STMT_BLOCK;
	funcBlock->addChild(yyvsp[-1]);
	yyval->addChild(funcBlock);
	scopePop();
  }
#line 1653 "parser.cpp"
    break;

  case 31: /* funcDef: basicType pDeclIdentifier funcLPAREN RPAREN LBRACE blockItems RBRACE  */
#line 305 "parser.y"
                                                                       {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_FUNCDECL;
	yyvsp[-5]->type->type = COMPOSE_FUNCTION;
	yyvsp[-5]->type->retType = yyvsp[-6]->type;
    FuncList[yyvsp[-5]->var_name] = yyvsp[-6]->type;
	yyval->addChild(yyvsp[-6]);
	yyval->addChild(yyvsp[-5]);
	yyval->addChild(new TreeNode(lineno, NODE_VARLIST));
	TreeNode* funcBlock = new TreeNode(lineno, NODE_STMT);
	funcBlock->stype = STMT_BLOCK;
	funcBlock->addChild(yyvsp[-1]);
	yyval->addChild(funcBlock);
	scopePop();
  }
#line 1673 "parser.cpp"
    break;

  case 32: /* funcLPAREN: LPAREN  */
#line 322 "parser.y"
                    {scopePush();}
#line 1679 "parser.cpp"
    break;

  case 33: /* funcFParams: funcFParam  */
#line 325 "parser.y"
             {yyval = yyvsp[0];}
#line 1685 "parser.cpp"
    break;

  case 34: /* funcFParams: funcFParams COMMA funcFParam  */
#line 326 "parser.y"
                               {yyval = yyvsp[-2]; yyval->addSibling(yyvsp[0]);}
#line 1691 "parser.cpp"
    break;

  case 35: /* funcFParam: basicType pDeclIdentifier  */
#line 330 "parser.y"
                            {
	yyval = new TreeNode(lineno, NODE_PARAM); 
	yyval->addChild(yyvsp[-1]); 
	yyval->addChild(yyvsp[0]);
	yyvsp[0]->type->copy(yyvsp[-1]->type);
  }
#line 1702 "parser.cpp"
    break;

  case 36: /* block: blockLBRACE blockItems blockRBRACE  */
#line 341 "parser.y"
                                     {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_BLOCK;
	yyval->addChild(yyvsp[-1]);
}
#line 1712 "parser.cpp"
    break;

  case 37: /* blockLBRACE: LBRACE  */
#line 347 "parser.y"
                     {scopePush();}
#line 1718 "parser.cpp"
    break;

  case 38: /* blockRBRACE: RBRACE  */
#line 348 "parser.y"
                     {scopePop();}
#line 1724 "parser.cpp"
    break;

  case 39: /* blockItems: blockItem  */
#line 351 "parser.y"
            {yyval = yyvsp[0];}
#line 1730 "parser.cpp"
    break;

  case 40: /* blockItems: blockItems blockItem  */
#line 352 "parser.y"
                       {yyval = yyvsp[-1]; yyval->addSibling(yyvsp[0]);}
#line 1736 "parser.cpp"
    break;

  case 41: /* blockItem: decl  */
#line 356 "parser.y"
       {yyval = yyvsp[0];}
#line 1742 "parser.cpp"
    break;

  case 42: /* blockItem: stmt  */
#line 357 "parser.y"
       {yyval = yyvsp[0];}
#line 1748 "parser.cpp"
    break;

  case 43: /* stmt_: LBRACE blockItems RBRACE  */
#line 361 "parser.y"
                           {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_BLOCK;
	yyval->addChild(yyvsp[-1]);
  }
#line 1758 "parser.cpp"
    break;

  case 44: /* stmt_: stmt  */
#line 366 "parser.y"
       {yyval = yyvsp[0];}
#line 1764 "parser.cpp"
    break;

  case 45: /* stmt: SEMICOLON  */
#line 370 "parser.y"
            {yyval = new TreeNode(lineno, NODE_STMT); yyval->stype = STMT_SKIP;}
#line 1770 "parser.cpp"
    break;

  case 46: /* stmt: expr SEMICOLON  */
#line 371 "parser.y"
                 {yyval = yyvsp[-1];}
#line 1776 "parser.cpp"
    break;

  case 47: /* stmt: block  */
#line 372 "parser.y"
        {yyval = yyvsp[0];}
#line 1782 "parser.cpp"
    break;

  case 48: /* stmt: IF LPAREN cond RPAREN stmt_ ELSE stmt_  */
#line 373 "parser.y"
                                         {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_IFELSE;
	yyval->addChild(yyvsp[-4]);
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(yyvsp[0]);
	scopePop();
	#ifdef IFELSE_DEBUG
		cout << "# $ reduce IF-ELSE at scope : " << presentScope << ", at line " << lineno << endl;
	#endif
  }
#line 1798 "parser.cpp"
    break;

  case 49: /* stmt: IF LPAREN cond RPAREN stmt_  */
#line 384 "parser.y"
                              {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_IF;
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(yyvsp[0]);
	scopePop();
	#ifdef IF_DEBUG
		cout << "% # $ reduce IF at scope : " << presentScope << ", at line " << lineno << endl;
	#endif
  }
#line 1813 "parser.cpp"
    break;

  case 50: /* stmt: WHILE LPAREN cond RPAREN stmt_  */
#line 394 "parser.y"
                                 {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_WHILE;
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(yyvsp[0]);
	inCycle = false;
	scopePop();
	#ifdef WHILE
		cout << "# $ reduce WHILE at scope : " << presentScope << ", at line " << lineno << endl;
	#endif
  }
#line 1829 "parser.cpp"
    break;

  case 51: /* stmt: BREAK SEMICOLON  */
#line 405 "parser.y"
                  {
	if (!inCycle) {
		yyerror("break statement outside loop");
	}
	yyval = new TreeNode(lineno, NODE_STMT); 
	yyval->stype = STMT_BREAK; 
	yyval->type = TYPE_NONE;
  }
#line 1842 "parser.cpp"
    break;

  case 52: /* stmt: CONTINUE SEMICOLON  */
#line 413 "parser.y"
                    {
	if (!inCycle) {
		yyerror("continue statement outside loop");
	}
	yyval = new TreeNode(lineno, NODE_STMT); 
	yyval->stype = STMT_CONTINUE; 
	yyval->type = TYPE_NONE;
  }
#line 1855 "parser.cpp"
    break;

  case 53: /* stmt: RETURN SEMICOLON  */
#line 421 "parser.y"
                   {yyval = new TreeNode(lineno, NODE_STMT); yyval->stype = STMT_RETURN; yyval->type = TYPE_NONE;}
#line 1861 "parser.cpp"
    break;

  case 54: /* stmt: RETURN expr SEMICOLON  */
#line 422 "parser.y"
                        {yyval = new TreeNode(lineno, NODE_STMT); yyval->stype = STMT_RETURN; yyval->addChild(yyvsp[-1]); yyval->type = TYPE_NONE;}
#line 1867 "parser.cpp"
    break;

  case 55: /* IF: IF_  */
#line 425 "parser.y"
         {scopePush();}
#line 1873 "parser.cpp"
    break;

  case 56: /* WHILE: WHILE_  */
#line 426 "parser.y"
               {inCycle = true; scopePush();}
#line 1879 "parser.cpp"
    break;

  case 57: /* expr: addExpr  */
#line 431 "parser.y"
          {yyval = yyvsp[0];}
#line 1885 "parser.cpp"
    break;

  case 58: /* expr: compIdentifier ASSIGN expr  */
#line 432 "parser.y"
                             {
	yyval = new TreeNode(lineno, NODE_OP);
	yyval->optype = OP_ASSIGN;
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(yyvsp[0]);
	#ifdef ASSIGN_DEBUG
		cout << "# $ reduce ASSIGN at scope : " << presentScope << ", at line " << lineno << endl;
		// $$->printAST();
	#endif
  }
#line 1900 "parser.cpp"
    break;

  case 59: /* expr: compIdentifier PLUSASSIGN expr  */
#line 442 "parser.y"
                                 {
	yyval = new TreeNode(lineno, NODE_OP);
	yyval->optype = OP_ADDASSIGN;
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(yyvsp[0]);
  }
#line 1911 "parser.cpp"
    break;

  case 60: /* expr: compIdentifier MINUSASSIGN expr  */
#line 448 "parser.y"
                                  {
	yyval = new TreeNode(lineno, NODE_OP);
	yyval->optype = OP_SUBASSIGN;
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(yyvsp[0]);
  }
#line 1922 "parser.cpp"
    break;

  case 61: /* expr: compIdentifier MULASSIGN expr  */
#line 454 "parser.y"
                                {
	yyval = new TreeNode(lineno, NODE_OP);
	yyval->optype = OP_MULASSIGN;
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(yyvsp[0]);
  }
#line 1933 "parser.cpp"
    break;

  case 62: /* expr: compIdentifier DIVASSIGN expr  */
#line 460 "parser.y"
                                {
	yyval = new TreeNode(lineno, NODE_OP);
	yyval->optype = OP_DIVASSIGN;
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(yyvsp[0]);
  }
#line 1944 "parser.cpp"
    break;

  case 63: /* cond: LOrExpr  */
#line 469 "parser.y"
          {yyval = yyvsp[0];}
#line 1950 "parser.cpp"
    break;

  case 64: /* addExpr: mulExpr  */
#line 473 "parser.y"
          {yyval = yyvsp[0];}
#line 1956 "parser.cpp"
    break;

  case 65: /* addExpr: addExpr PLUS mulExpr  */
#line 474 "parser.y"
                       {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_ADD; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 1962 "parser.cpp"
    break;

  case 66: /* addExpr: addExpr MINUS mulExpr  */
#line 475 "parser.y"
                        {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_SUB; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 1968 "parser.cpp"
    break;

  case 67: /* mulExpr: unaryExpr  */
#line 480 "parser.y"
            {yyval = yyvsp[0];}
#line 1974 "parser.cpp"
    break;

  case 68: /* mulExpr: mulExpr MUL unaryExpr  */
#line 481 "parser.y"
                        {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_MUL; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 1980 "parser.cpp"
    break;

  case 69: /* mulExpr: mulExpr DIV unaryExpr  */
#line 482 "parser.y"
                        {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_DIV; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 1986 "parser.cpp"
    break;

  case 70: /* mulExpr: mulExpr MOD unaryExpr  */
#line 483 "parser.y"
                        {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_MOD; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 1992 "parser.cpp"
    break;

  case 71: /* unaryExpr: primaryExpr  */
#line 488 "parser.y"
              {yyval = yyvsp[0];}
#line 1998 "parser.cpp"
    break;

  case 72: /* unaryExpr: NOT cond  */
#line 489 "parser.y"
           {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_NOT; yyval->addChild(yyvsp[0]);}
#line 2004 "parser.cpp"
    break;

  case 73: /* unaryExpr: PLUS primaryExpr  */
#line 490 "parser.y"
                   {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_POS; yyval->addChild(yyvsp[0]);}
#line 2010 "parser.cpp"
    break;

  case 74: /* unaryExpr: MINUS primaryExpr  */
#line 491 "parser.y"
                    {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_NAG; yyval->addChild(yyvsp[0]);}
#line 2016 "parser.cpp"
    break;

  case 75: /* unaryExpr: pIdentifier INC  */
#line 492 "parser.y"
                  {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_INC; yyval->addChild(yyvsp[-1]);}
#line 2022 "parser.cpp"
    break;

  case 76: /* unaryExpr: pIdentifier DEC  */
#line 493 "parser.y"
                  {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_DEC; yyval->addChild(yyvsp[-1]);}
#line 2028 "parser.cpp"
    break;

  case 77: /* primaryExpr: literalConst  */
#line 498 "parser.y"
               {yyval = yyvsp[0];}
#line 2034 "parser.cpp"
    break;

  case 78: /* primaryExpr: LPAREN expr RPAREN  */
#line 499 "parser.y"
                     {yyval = yyvsp[-1];}
#line 2040 "parser.cpp"
    break;

  case 79: /* primaryExpr: LPAREN cond RPAREN  */
#line 500 "parser.y"
                     {yyval = yyvsp[-1];}
#line 2046 "parser.cpp"
    break;

  case 80: /* primaryExpr: pIdentifier LPAREN funcRParams RPAREN  */
#line 501 "parser.y"
                                        {
	yyval = new TreeNode(lineno, NODE_FUNCALL);
	yyval->addChild(yyvsp[-3]);
	yyval->addChild(yyvsp[-1]);
	#ifdef FUNCALL_DEBUG
		cout << "# $ reduce function call at scope : " << presentScope << ", at line " << lineno << endl;
	#endif
  }
#line 2059 "parser.cpp"
    break;

  case 81: /* primaryExpr: pIdentifier LPAREN RPAREN  */
#line 509 "parser.y"
                            {
	yyval = new TreeNode(lineno, NODE_FUNCALL);
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(new TreeNode(lineno, NODE_VARLIST));
	#ifdef FUNCALL_DEBUG
		cout << "# $ reduce function call at scope : " << presentScope << ", at line " << lineno << endl;
	#endif
  }
#line 2072 "parser.cpp"
    break;

  case 82: /* primaryExpr: compIdentifier  */
#line 517 "parser.y"
                 {yyval = new TreeNode(lineno, NODE_EXPR); yyval->addChild(yyvsp[0]);}
#line 2078 "parser.cpp"
    break;

  case 83: /* funcRParams: expr  */
#line 522 "parser.y"
       {yyval = new TreeNode(lineno, NODE_VARLIST); yyval->addChild(yyvsp[0]);}
#line 2084 "parser.cpp"
    break;

  case 84: /* funcRParams: funcRParams COMMA expr  */
#line 523 "parser.y"
                         {yyval = yyvsp[-2]; yyval->addChild(yyvsp[0]);}
#line 2090 "parser.cpp"
    break;

  case 85: /* LOrExpr: LAndExpr  */
#line 528 "parser.y"
           {yyval = yyvsp[0];}
#line 2096 "parser.cpp"
    break;

  case 86: /* LOrExpr: LAndExpr OR LOrExpr  */
#line 529 "parser.y"
                      {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_OR; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2102 "parser.cpp"
    break;

  case 87: /* LAndExpr: eqExpr  */
#line 534 "parser.y"
         {yyval = yyvsp[0];}
#line 2108 "parser.cpp"
    break;

  case 88: /* LAndExpr: eqExpr AND LAndExpr  */
#line 535 "parser.y"
                      {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_AND; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2114 "parser.cpp"
    break;

  case 89: /* eqExpr: relExpr  */
#line 540 "parser.y"
          {yyval = yyvsp[0];}
#line 2120 "parser.cpp"
    break;

  case 90: /* eqExpr: relExpr EQ eqExpr  */
#line 541 "parser.y"
                    {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_EQ; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2126 "parser.cpp"
    break;

  case 91: /* eqExpr: relExpr NEQ eqExpr  */
#line 542 "parser.y"
                     {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_NEQ; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2132 "parser.cpp"
    break;

  case 92: /* relExpr: expr  */
#line 547 "parser.y"
       {yyval = yyvsp[0];}
#line 2138 "parser.cpp"
    break;

  case 93: /* relExpr: relExpr GRA expr  */
#line 548 "parser.y"
                   {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_GRA; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2144 "parser.cpp"
    break;

  case 94: /* relExpr: relExpr LES expr  */
#line 549 "parser.y"
                   {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_LES; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2150 "parser.cpp"
    break;

  case 95: /* relExpr: relExpr GRAEQ expr  */
#line 550 "parser.y"
                     {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_GRAEQ; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2156 "parser.cpp"
    break;

  case 96: /* relExpr: relExpr LESEQ expr  */
#line 551 "parser.y"
                     {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_LESEQ; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2162 "parser.cpp"
    break;


#line 2166 "parser.cpp"

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

#line 554 "parser.y"


int yyerror(char const * message)
{
	cout << "error: " << message << ", at line " << lineno << endl;
	parserError = true;
#ifdef EXIT_AT_GRAMMA_ERROR
	exit(0);
#else
	return 0;
#endif
}

/*
 *	作用域比较函数 int scopeCmp (string, string)
 *
 *  输入参数： 
 *    presScope： 当前变量所处的作用域
 *    varScope:   希望进行比较的已声明变量作用域
 *
 *  返回值：
 *    0： 作用域相同，
 *          若为变量声明语句，为变量重定义。
 *   >0： 已声明变量作用域在当前作用域外层，返回作用域距离（堆栈层数）
 *          若为声明语句，不产生冲突，当前变量为新定义变量，
 *          若为使用语句，当前变量为新定义变量。
 *   -1：已声明变量作用域在当前作用域内层，
 *          若为声明语句，不可能出现这种情况，
 *          若为使用语句，不产生冲突。
 *   -2：两个作用域互不包含，任何情况下都不会互相干扰
 */
int scopeCmp(string presScope, string varScope) {
	unsigned int plen = presScope.length(), vlen = varScope.length();
	unsigned int minlen = min(plen, vlen);
	if (presScope.substr(0, minlen) == varScope.substr(0, minlen)) {
		if (plen >= vlen)
			return plen - vlen;
		else
			return -1;
	}
	return -2;
}

void scopePush() {
	presentScope += max_scope_id;
	max_scope_id = SCOPT_ID_BASE;
	top++;
#ifdef SCOPE_DEBUG
	cout << "* push -> " << presentScope << ", at line " << lineno << endl;
#endif
}

void scopePop() {
	max_scope_id = presentScope[top] + 1;
	presentScope = presentScope.substr(0, presentScope.length() - 1);
	top--;
#ifdef SCOPE_DEBUG
	cout << "* pop -> " << presentScope << ", at line " << lineno << endl;
#endif
}

/*
 *	变量作用域切换只会发生在以下地方：
 *
 *		函数体		type funcName ( params ) block
 *								  ↑ push		  ↑ pop
 *
 *		block块		{ stmts }
 *					↑ push	↑ pop
 *
 *		if语句		IF ( cond ) block
 *					↑ push			  ↑ pop
 *
 *					IF ( cond ) block ELSE block
 *					↑ push						 ↑ pop
 *
 *		while语句	WHILE ( cond ) block
 *					↑ push				 ↑ pop
 *
 *		for语句		FOR ( expr ; cond ; expr ) block
 *					↑ push							 ↑ pop
 *
 *					FOR ( decl ; cond ; expr ) block
 *					↑ push							 ↑ pop
 *
 * 	可得作用域推进表：
 *
 *		push:
 *			IF
 *			WHILE
 *			FOR
 *			funcLPAREN
 *		pop:
 *			ifStmt(代码段尾部)
 *			whileStmt(代码段尾部)
 *			forStmt(代码段尾部)
 *			funcDef(代码段尾部)
 */
