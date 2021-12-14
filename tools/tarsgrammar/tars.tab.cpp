/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 17 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"

#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

#define YYSTYPE GrammarBasePtr

#include "parse.h"
#define YYDEBUG 1
#define YYINITDEPTH 10000

/* Line 371 of yacc.c  */
#line 82 "tars.tab.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "tars.tab.hpp".  */
#ifndef YY_YY_TARS_TAB_HPP_INCLUDED
# define YY_YY_TARS_TAB_HPP_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TARS_VOID = 258,
     TARS_STRUCT = 259,
     TARS_BOOL = 260,
     TARS_BYTE = 261,
     TARS_SHORT = 262,
     TARS_INT = 263,
     TARS_DOUBLE = 264,
     TARS_FLOAT = 265,
     TARS_LONG = 266,
     TARS_STRING = 267,
     TARS_VECTOR = 268,
     TARS_MAP = 269,
     TARS_NAMESPACE = 270,
     TARS_INTERFACE = 271,
     TARS_IDENTIFIER = 272,
     TARS_OUT = 273,
     TARS_OP = 274,
     TARS_KEY = 275,
     TARS_ROUTE_KEY = 276,
     TARS_REQUIRE = 277,
     TARS_OPTIONAL = 278,
     TARS_CONST_INTEGER = 279,
     TARS_CONST_FLOAT = 280,
     TARS_FALSE = 281,
     TARS_TRUE = 282,
     TARS_STRING_LITERAL = 283,
     TARS_SCOPE_DELIMITER = 284,
     TARS_CONST = 285,
     TARS_ENUM = 286,
     TARS_UNSIGNED = 287,
     BAD_CHAR = 288
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_TARS_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 181 "tars.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  75
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   565

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  137
/* YYNRULES -- Number of states.  */
#define YYNSTATES  199

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   288

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    41,    42,     2,    37,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    43,    34,
      44,    38,    45,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    39,     2,    40,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,    36,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,    11,    12,    17,    19,    20,
      22,    24,    26,    28,    30,    32,    33,    39,    42,    45,
      49,    51,    53,    55,    59,    60,    61,    68,    69,    77,
      79,    83,    84,    90,    93,    96,   100,   104,   106,   107,
     109,   113,   116,   118,   120,   121,   123,   127,   130,   135,
     138,   143,   145,   147,   149,   151,   152,   158,   161,   164,
     167,   171,   173,   174,   176,   180,   186,   192,   196,   199,
     202,   205,   207,   209,   211,   213,   215,   217,   219,   223,
     228,   231,   237,   241,   246,   249,   251,   253,   257,   259,
     263,   265,   267,   270,   272,   275,   277,   280,   282,   284,
     286,   288,   290,   292,   294,   299,   303,   308,   311,   318,
     322,   324,   327,   331,   333,   335,   337,   339,   341,   343,
     345,   347,   349,   351,   353,   355,   357,   359,   361,   363,
     365,   367,   369,   371,   373,   375,   377,   379
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      47,     0,    -1,    48,    -1,    -1,    51,    49,    34,    48,
      -1,    -1,     1,    34,    50,    48,    -1,    51,    -1,    -1,
      57,    -1,    62,    -1,    73,    -1,    59,    -1,    52,    -1,
      80,    -1,    -1,    54,    53,    35,    55,    36,    -1,    31,
      17,    -1,    31,    87,    -1,    56,    37,    55,    -1,    56,
      -1,    17,    -1,    87,    -1,    17,    38,    79,    -1,    -1,
      -1,    15,    17,    58,    35,    48,    36,    -1,    -1,    20,
      39,    86,    37,    60,    61,    40,    -1,    17,    -1,    61,
      37,    17,    -1,    -1,    64,    63,    35,    65,    36,    -1,
      16,    17,    -1,    16,    87,    -1,    66,    34,    65,    -1,
       1,    34,    65,    -1,    66,    -1,    -1,    67,    -1,    68,
      70,    41,    -1,    69,    19,    -1,    82,    -1,     3,    -1,
      -1,    81,    -1,    70,    37,    81,    -1,    72,    81,    -1,
      70,    37,    72,    81,    -1,    71,    81,    -1,    70,    37,
      71,    81,    -1,    72,    -1,    71,    -1,    21,    -1,    18,
      -1,    -1,    75,    74,    35,    76,    36,    -1,     4,    17,
      -1,     4,    87,    -1,     4,     1,    -1,    77,    34,    76,
      -1,    77,    -1,    -1,    78,    -1,    24,    22,    81,    -1,
      24,    22,    81,    38,    79,    -1,    24,    23,    81,    38,
      79,    -1,    24,    23,    81,    -1,    22,    81,    -1,    23,
      81,    -1,    24,    81,    -1,    81,    -1,    24,    -1,    25,
      -1,    28,    -1,    26,    -1,    27,    -1,    17,    -1,    86,
      29,    17,    -1,    30,    81,    38,    79,    -1,    82,    17,
      -1,    82,    17,    39,    24,    40,    -1,    82,    42,    17,
      -1,    82,    17,    43,    24,    -1,    82,    87,    -1,    82,
      -1,     1,    -1,    83,    43,    24,    -1,    83,    -1,    83,
      43,     1,    -1,     5,    -1,     6,    -1,    32,     6,    -1,
       7,    -1,    32,     7,    -1,     8,    -1,    32,     8,    -1,
      11,    -1,    10,    -1,     9,    -1,    12,    -1,    84,    -1,
      85,    -1,    86,    -1,    13,    44,    82,    45,    -1,    13,
      44,     1,    -1,    13,    44,    82,     1,    -1,    13,     1,
      -1,    14,    44,    82,    37,    82,    45,    -1,    14,    44,
       1,    -1,    17,    -1,    29,    17,    -1,    86,    29,    17,
      -1,     4,    -1,     3,    -1,     5,    -1,     6,    -1,     7,
      -1,     8,    -1,    10,    -1,     9,    -1,    12,    -1,    13,
      -1,    20,    -1,    14,    -1,    15,    -1,    16,    -1,    18,
      -1,    22,    -1,    23,    -1,    24,    -1,    25,    -1,    26,
      -1,    27,    -1,    28,    -1,    30,    -1,    31,    -1,    32,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    68,    68,    75,    74,    79,    78,    83,    88,    95,
      99,   103,   107,   110,   114,   124,   123,   146,   159,   170,
     174,   182,   193,   198,   212,   220,   219,   253,   252,   271,
     284,   304,   303,   337,   341,   352,   355,   358,   363,   370,
     376,   393,   422,   423,   434,   436,   447,   458,   470,   482,
     494,   506,   510,   519,   530,   542,   541,   583,   587,   593,
     602,   606,   611,   620,   629,   647,   669,   691,   708,   712,
     716,   720,   729,   739,   749,   757,   765,   773,   786,   806,
     824,   833,   843,   853,   862,   867,   871,   880,   889,   893,
     902,   906,   910,   914,   918,   922,   926,   930,   934,   938,
     942,   946,   950,   954,   972,   976,   980,   984,   993,   997,
    1006,  1009,  1015,  1028,  1031,  1034,  1037,  1040,  1043,  1046,
    1049,  1052,  1055,  1058,  1061,  1064,  1067,  1070,  1073,  1076,
    1079,  1082,  1085,  1088,  1091,  1094,  1097,  1100
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TARS_VOID", "TARS_STRUCT", "TARS_BOOL",
  "TARS_BYTE", "TARS_SHORT", "TARS_INT", "TARS_DOUBLE", "TARS_FLOAT",
  "TARS_LONG", "TARS_STRING", "TARS_VECTOR", "TARS_MAP", "TARS_NAMESPACE",
  "TARS_INTERFACE", "TARS_IDENTIFIER", "TARS_OUT", "TARS_OP", "TARS_KEY",
  "TARS_ROUTE_KEY", "TARS_REQUIRE", "TARS_OPTIONAL", "TARS_CONST_INTEGER",
  "TARS_CONST_FLOAT", "TARS_FALSE", "TARS_TRUE", "TARS_STRING_LITERAL",
  "TARS_SCOPE_DELIMITER", "TARS_CONST", "TARS_ENUM", "TARS_UNSIGNED",
  "BAD_CHAR", "';'", "'{'", "'}'", "','", "'='", "'['", "']'", "')'",
  "'*'", "':'", "'<'", "'>'", "$accept", "start", "definitions", "$@1",
  "$@2", "definition", "enum_def", "@3", "enum_id", "enumerator_list",
  "enumerator", "namespace_def", "@4", "key_def", "$@5", "key_members",
  "interface_def", "@6", "interface_id", "interface_exports",
  "interface_export", "operation", "operation_preamble", "return_type",
  "parameters", "routekey_qualifier", "out_qualifier", "struct_def", "@7",
  "struct_id", "struct_exports", "data_member", "struct_type_id",
  "const_initializer", "const_def", "type_id", "type", "type_no", "vector",
  "map", "scoped_name", "keyword", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,    59,   123,   125,    44,    61,    91,
      93,    41,    42,    58,    60,    62
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    49,    48,    50,    48,    48,    48,    51,
      51,    51,    51,    51,    51,    53,    52,    54,    54,    55,
      55,    56,    56,    56,    56,    58,    57,    60,    59,    61,
      61,    63,    62,    64,    64,    65,    65,    65,    65,    66,
      67,    68,    69,    69,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    71,    72,    74,    73,    75,    75,    75,
      76,    76,    76,    77,    78,    78,    78,    78,    78,    78,
      78,    78,    79,    79,    79,    79,    79,    79,    79,    80,
      81,    81,    81,    81,    81,    81,    81,    82,    82,    82,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    84,    84,    84,    84,    85,    85,
      86,    86,    86,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     4,     0,     4,     1,     0,     1,
       1,     1,     1,     1,     1,     0,     5,     2,     2,     3,
       1,     1,     1,     3,     0,     0,     6,     0,     7,     1,
       3,     0,     5,     2,     2,     3,     3,     1,     0,     1,
       3,     2,     1,     1,     0,     1,     3,     2,     4,     2,
       4,     1,     1,     1,     1,     0,     5,     2,     2,     2,
       3,     1,     0,     1,     3,     5,     5,     3,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       2,     5,     3,     4,     2,     1,     1,     3,     1,     3,
       1,     1,     2,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     4,     3,     4,     2,     6,     3,
       1,     2,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       7,    13,    15,     9,    12,    10,    31,    11,    55,    14,
       5,    59,   114,   113,   115,   116,   117,   118,   120,   119,
     121,   122,   124,   125,   126,    57,   127,   123,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    58,    25,
      33,    34,     0,    86,    90,    91,    93,    95,    99,    98,
      97,   100,     0,     0,   110,     0,     0,     0,    85,    88,
     101,   102,   103,    17,    18,     1,     0,     0,     0,     0,
       0,     0,     0,   107,     0,     0,   111,    92,    94,    96,
       0,    80,     0,    84,     0,     0,     0,    24,     0,     0,
       6,     0,    27,   105,     0,   109,     0,    77,    72,    73,
      75,    76,    74,    79,     0,     0,     0,    82,    89,    87,
     112,     4,    21,     0,    20,    22,     0,    43,     0,    37,
      39,     0,     0,    42,     0,     0,     0,     0,    61,    63,
      71,     0,     0,   106,   104,     0,     0,     0,    83,     0,
      16,    24,     0,    32,     0,    54,    53,     0,     0,     0,
      45,    41,    68,    69,     0,     0,    70,    56,     0,    26,
      29,     0,     0,    78,    81,    23,    19,    36,    35,     0,
      40,    49,    47,    64,    67,    60,     0,    28,   108,     0,
       0,    46,     0,     0,    30,    50,    48,    65,    66
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     8,     9,    76,    80,    10,    11,    77,    12,   123,
     124,    13,    81,    14,   142,   171,    15,    78,    16,   128,
     129,   130,   131,   132,   157,   158,   159,    17,    79,    18,
     137,   138,   139,   113,    19,   140,    68,    69,    70,    71,
      72,   125
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -146
static const yytype_int16 yypact[] =
{
     145,   -22,   286,    20,   445,    16,   372,   475,    42,  -146,
      26,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,    11,  -146,  -146,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,     7,     2,  -146,    37,    51,    23,   182,    19,
    -146,  -146,    35,  -146,  -146,  -146,    31,    32,    33,    49,
     102,    66,    -2,  -146,   386,   415,  -146,  -146,  -146,  -146,
     153,    -5,    69,  -146,     8,    87,   102,   505,   252,   220,
    -146,   120,  -146,  -146,     5,  -146,    29,    78,  -146,  -146,
    -146,  -146,  -146,  -146,    79,    86,    88,  -146,  -146,  -146,
    -146,  -146,    75,    80,    83,  -146,    81,  -146,    90,    93,
    -146,    12,   112,  -146,   372,   372,   314,    98,   103,  -146,
    -146,   107,   122,  -146,  -146,   533,   127,   108,  -146,   153,
    -146,   505,   252,  -146,   252,  -146,  -146,    15,    68,    82,
    -146,  -146,  -146,  -146,   372,   372,  -146,  -146,   220,  -146,
    -146,   -26,   109,   118,  -146,  -146,  -146,  -146,  -146,   343,
    -146,  -146,  -146,   117,   119,  -146,   146,  -146,  -146,   372,
     372,  -146,   153,   153,  -146,  -146,  -146,  -146,  -146
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -146,  -146,   -65,  -146,  -146,  -146,  -146,  -146,  -146,    13,
    -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -146,  -109,
    -146,  -146,  -146,  -146,  -146,   -17,   -13,  -146,  -146,  -146,
      -1,  -146,  -146,  -145,  -146,    -6,   -82,  -146,  -146,  -146,
     -51,     3
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -113
static const yytype_int16 yytable[] =
{
      67,    82,   104,   106,   175,    48,   143,    51,    83,   118,
      74,   186,    20,    53,   187,   100,   133,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    95,    64,    64,
     155,   121,   119,   156,   115,   102,   141,    49,   116,   114,
      65,    65,    75,   177,    66,   178,    85,   197,   198,   -44,
     144,    84,   179,   -44,    86,    52,   180,    87,    88,    89,
      -3,    90,    94,   172,    95,    96,   145,    97,    98,    53,
     133,    93,   133,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    53,    99,    64,   117,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    65,   114,    64,
      66,   101,    -8,     1,   120,   -52,     2,  -110,   146,   -52,
     147,    65,   148,   149,    66,   152,   150,     3,     4,   -51,
     151,     1,     5,   -51,     2,   160,   153,   154,   162,   163,
     166,   161,     6,     7,   167,     3,     4,   168,    -8,   170,
       5,   114,   114,   169,   173,    -8,     1,  -112,   174,     2,
       6,     7,   181,   182,   188,   192,    -8,   193,   183,   184,
       3,     4,   189,   194,   176,     5,   190,   185,     0,     0,
     107,     0,     0,   191,     0,     6,     7,   108,   109,   110,
     111,   112,    65,   195,   196,    22,    23,    24,    25,    26,
      27,    28,    29,     0,    30,    31,    32,    33,    34,    91,
      36,     0,    37,     0,    38,    39,    40,    41,    42,    43,
      44,     0,    45,    46,    47,     0,     0,     0,     0,     0,
       0,    53,     0,     0,    92,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,     0,     0,    64,     0,     0,
       0,     0,   134,   135,   136,     0,     0,     0,     0,    65,
       0,     0,    66,   126,     0,   127,   -62,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,    66,     0,     0,    21,   -38,    22,
      23,    24,    25,    26,    27,    28,    29,     0,    30,    31,
      32,    33,    34,    35,    36,     0,    37,     0,    38,    39,
      40,    41,    42,    43,    44,    53,    45,    46,    47,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,     0,
       0,    64,     0,     0,     0,     0,   164,   165,     0,     0,
       0,     0,     0,    65,    53,     0,    66,     0,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,     0,     0,
      64,   155,     0,     0,   156,     0,     0,     0,     0,     0,
       0,     0,    65,    53,     0,    66,     0,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,   103,     0,    64,
       0,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    65,     0,    64,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,   105,     0,    66,     0,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,    66,    22,    23,
      24,    25,    26,    27,    28,    29,     0,    30,    31,    32,
      33,    34,    50,    36,     0,    37,     0,    38,    39,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    22,    23,
      24,    25,    26,    27,    28,    29,     0,    30,    31,    32,
      33,    34,    73,    36,     0,    37,     0,    38,    39,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    22,    23,
      24,    25,    26,    27,    28,    29,     0,    30,    31,    32,
      33,    34,   122,    36,     0,    37,     0,    38,    39,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,     0,    66
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-146)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       6,    52,    84,    85,   149,     2,     1,     4,     1,     1,
       7,    37,    34,     1,    40,    80,    98,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    29,    17,    17,
      18,    96,    24,    21,    39,    37,   101,    17,    43,    90,
      29,    29,     0,   152,    32,   154,    44,   192,   193,    37,
      45,    44,    37,    41,    17,    39,    41,     6,     7,     8,
      34,    38,    43,   145,    29,    34,    37,    35,    35,     1,
     152,    68,   154,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,     1,    35,    17,    17,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    29,   149,    17,
      32,    35,     0,     1,    17,    37,     4,    29,    29,    41,
      24,    29,    24,    38,    32,    34,    36,    15,    16,    37,
      37,     1,    20,    41,     4,   131,    36,    34,   134,   135,
     136,    19,    30,    31,    36,    15,    16,    34,    36,    17,
      20,   192,   193,    36,    17,     0,     1,    29,    40,     4,
      30,    31,   158,   159,    45,    38,    36,    38,   164,   165,
      15,    16,   179,    17,   151,    20,   179,   168,    -1,    -1,
      17,    -1,    -1,   179,    -1,    30,    31,    24,    25,    26,
      27,    28,    29,   189,   190,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    12,    13,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    -1,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,    -1,    42,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    22,    23,    24,    -1,    -1,    -1,    -1,    29,
      -1,    -1,    32,     1,    -1,     3,    36,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    32,    -1,    -1,     1,    36,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    13,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,     1,    30,    31,    32,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    22,    23,    -1,    -1,
      -1,    -1,    -1,    29,     1,    -1,    32,    -1,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    -1,    -1,
      17,    18,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,     1,    -1,    32,    -1,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,     1,    -1,    17,
      -1,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    29,    -1,    17,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,     1,    -1,    32,    -1,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    -1,    32,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    12,    13,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    -1,    30,    31,    32,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    12,    13,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    -1,    30,    31,    32,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    12,    13,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    -1,    30,    31,    32,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    32
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     4,    15,    16,    20,    30,    31,    47,    48,
      51,    52,    54,    57,    59,    62,    64,    73,    75,    80,
      34,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      12,    13,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    30,    31,    32,    87,    17,
      17,    87,    39,     1,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    17,    29,    32,    81,    82,    83,
      84,    85,    86,    17,    87,     0,    49,    53,    63,    74,
      50,    58,    86,     1,    44,    44,    17,     6,     7,     8,
      38,    17,    42,    87,    43,    29,    34,    35,    35,    35,
      48,    35,    37,     1,    82,     1,    82,    17,    24,    25,
      26,    27,    28,    79,    86,    39,    43,    17,     1,    24,
      17,    48,    17,    55,    56,    87,     1,     3,    65,    66,
      67,    68,    69,    82,    22,    23,    24,    76,    77,    78,
      81,    48,    60,     1,    45,    37,    29,    24,    24,    38,
      36,    37,    34,    36,    34,    18,    21,    70,    71,    72,
      81,    19,    81,    81,    22,    23,    81,    36,    34,    36,
      17,    61,    82,    17,    40,    79,    55,    65,    65,    37,
      41,    81,    81,    81,    81,    76,    37,    40,    45,    71,
      72,    81,    38,    38,    17,    81,    81,    79,    79
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
/* Line 1792 of yacc.c  */
#line 75 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 79 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    yyerrok;
}
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 84 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    g_parse->error("`;' missing after definition");
}
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 88 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 96 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    assert((yyvsp[(1) - (1)]) == 0 || NamespacePtr::dynamicCast((yyvsp[(1) - (1)])));
}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 100 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    assert((yyvsp[(1) - (1)]) == 0 || InterfacePtr::dynamicCast((yyvsp[(1) - (1)])));
}
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 104 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    assert((yyvsp[(1) - (1)]) == 0 || StructPtr::dynamicCast((yyvsp[(1) - (1)])));
}
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 108 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 111 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    assert((yyvsp[(1) - (1)]) == 0 || EnumPtr::dynamicCast((yyvsp[(1) - (1)])));
}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 115 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    assert((yyvsp[(1) - (1)]) == 0 || ConstPtr::dynamicCast((yyvsp[(1) - (1)])));
}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 124 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = (yyvsp[(1) - (1)]);
}
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 128 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    if((yyvsp[(3) - (5)]))
    {
        g_parse->popContainer();
        (yyval) = (yyvsp[(3) - (5)]);
    }
    else
    {
        (yyval) = 0;
    }

    (yyval) = (yyvsp[(2) - (5)]);
}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 147 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    NamespacePtr c = NamespacePtr::dynamicCast(g_parse->currentContainer());
    if(!c)
    {
        g_parse->error("enum must define in namespace");
    }
    StringGrammarPtr ident  = StringGrammarPtr::dynamicCast((yyvsp[(2) - (2)]));
    EnumPtr e = c->createEnum(ident->v);
    g_parse->pushContainer(e);

    (yyval) = e;
}
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 160 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(2) - (2)]));
    g_parse->error("keyword `" + ident->v + "' cannot be used as enumeration name");
    (yyval) = (yyvsp[(2) - (2)]);
}
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 171 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = (yyvsp[(2) - (3)]);
}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 175 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 183 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    TypePtr type        = TypePtr::dynamicCast(g_parse->createBuiltin(Builtin::KindLong));
    StringGrammarPtr ident  = StringGrammarPtr::dynamicCast((yyvsp[(1) - (1)]));
    TypeIdPtr tPtr      = new TypeId(type, ident->v);
    tPtr->disableDefault();
    EnumPtr e = EnumPtr::dynamicCast(g_parse->currentContainer());
    assert(e);
    e->addMember(tPtr);
    (yyval) = e;
}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 194 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(1) - (1)]));
    g_parse->error("keyword `" + ident->v + "' cannot be used as enumerator");
}
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 199 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    TypePtr type        = TypePtr::dynamicCast(g_parse->createBuiltin(Builtin::KindLong));
    StringGrammarPtr ident  = StringGrammarPtr::dynamicCast((yyvsp[(1) - (3)]));
    TypeIdPtr tPtr      = new TypeId(type, ident->v);
    ConstGrammarPtr sPtr    = ConstGrammarPtr::dynamicCast((yyvsp[(3) - (3)]));
    g_parse->checkConstValue(tPtr, sPtr->t);
    tPtr->setDefault(sPtr->v);
    EnumPtr e = EnumPtr::dynamicCast(g_parse->currentContainer());
    assert(e);
    e->addMember(tPtr);
    (yyval) = e;
}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 212 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 220 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident  = StringGrammarPtr::dynamicCast((yyvsp[(2) - (2)]));
    ContainerPtr c      = g_parse->currentContainer();
    NamespacePtr n      = c->createNamespace(ident->v);
    if(n)
    {
        g_parse->pushContainer(n);
        (yyval) = GrammarBasePtr::dynamicCast(n);
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 235 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    if((yyvsp[(3) - (6)]))
    {
        g_parse->popContainer();
        (yyval) = (yyvsp[(3) - (6)]);
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 253 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(3) - (4)]));
    StructPtr sp = StructPtr::dynamicCast(g_parse->findUserType(ident->v));
    if(!sp)
    {
        g_parse->error("struct '" + ident->v + "' undefined!");
    }

    g_parse->setKeyStruct(sp);
}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 264 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 272 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(1) - (1)]));
    StructPtr np = g_parse->getKeyStruct();
    if(np)
    {
        np->addKey(ident->v);
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 285 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(3) - (3)]));
    StructPtr np = g_parse->getKeyStruct();
    if(np)
    {
        np->addKey(ident->v);
    }
    else
    {
        (yyval) = 0;
    }   
}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 304 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(1) - (1)]));

    NamespacePtr c = NamespacePtr::dynamicCast(g_parse->currentContainer());

    InterfacePtr cl = c->createInterface(ident->v);
    if(cl)
    {
        g_parse->pushContainer(cl);
        (yyval) = GrammarBasePtr::dynamicCast(cl);
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 321 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    if((yyvsp[(2) - (5)]))
    {
       g_parse->popContainer();
       (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(2) - (5)]));
    }
    else
    {
       (yyval) = 0;
    }
}
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 338 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = (yyvsp[(2) - (2)]);
}
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 342 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(2) - (2)]));
    g_parse->error("keyword `" + ident->v + "' cannot be used as interface name");
    (yyval) = (yyvsp[(2) - (2)]);
}
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 353 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 356 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 359 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    g_parse->error("`;' missing after definition");
}
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 363 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 377 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    if((yyvsp[(1) - (3)]))
    {
        g_parse->popContainer();
        (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(1) - (3)]));
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 394 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    TypePtr returnType = TypePtr::dynamicCast((yyvsp[(1) - (2)]));
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(2) - (2)]));
    string name        = ident->v;
    InterfacePtr cl    = InterfacePtr::dynamicCast(g_parse->currentContainer());
    if(cl)
    {
         OperationPtr op = cl->createOperation(name, returnType);
         if(op)
         {
             g_parse->pushContainer(op);
             (yyval) = GrammarBasePtr::dynamicCast(op);
         }
         else
         {
             (yyval) = 0;
         }
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 424 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = 0;
}
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 434 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 437 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    TypeIdPtr  tsp         = TypeIdPtr::dynamicCast((yyvsp[(1) - (1)]));

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
        op->createParamDecl(tsp, false, false);
    }
}
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 448 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    TypeIdPtr  tsp         = TypeIdPtr::dynamicCast((yyvsp[(3) - (3)]));

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
        op->createParamDecl(tsp, false, false);
    }
}
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 459 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    BoolGrammarPtr isOutParam  = BoolGrammarPtr::dynamicCast((yyvsp[(1) - (2)]));
    TypeIdPtr  tsp         = TypeIdPtr::dynamicCast((yyvsp[(2) - (2)]));

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
        op->createParamDecl(tsp, isOutParam->v, false);
    }
}
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 471 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    BoolGrammarPtr isOutParam  = BoolGrammarPtr::dynamicCast((yyvsp[(3) - (4)]));
    TypeIdPtr  tsp         = TypeIdPtr::dynamicCast((yyvsp[(4) - (4)]));

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
        op->createParamDecl(tsp, isOutParam->v, false);
    }
}
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 483 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    BoolGrammarPtr isRouteKeyParam  = BoolGrammarPtr::dynamicCast((yyvsp[(1) - (2)]));
    TypeIdPtr  tsp              = TypeIdPtr::dynamicCast((yyvsp[(2) - (2)]));

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
         op->createParamDecl(tsp, false, isRouteKeyParam->v);
    }
}
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 495 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    BoolGrammarPtr isRouteKeyParam = BoolGrammarPtr::dynamicCast((yyvsp[(3) - (4)]));
    TypeIdPtr  tsp             = TypeIdPtr::dynamicCast((yyvsp[(4) - (4)]));

    OperationPtr op = OperationPtr::dynamicCast(g_parse->currentContainer());
    assert(op);
    if(op)
    {
         op->createParamDecl(tsp, false, isRouteKeyParam->v);
    }
}
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 507 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    g_parse->error("'out' must be defined with a type");
}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 511 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    g_parse->error("'routekey' must be defined with a type");
}
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 520 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    BoolGrammarPtr routekey = new BoolGrammar;
    routekey->v = true;
    (yyval) = GrammarBasePtr::dynamicCast(routekey);
}
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 531 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    BoolGrammarPtr out = new BoolGrammar;
    out->v = true;
    (yyval) = GrammarBasePtr::dynamicCast(out);
}
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 542 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(1) - (1)]));
    NamespacePtr np = NamespacePtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
         StructPtr sp = np->createStruct(ident->v);
         if(sp)
         {
             g_parse->pushContainer(sp);
             (yyval) = GrammarBasePtr::dynamicCast(sp);
         }
         else
         {
             (yyval) = 0;
         }
    }
    else
    {
       g_parse->error("struct '" + ident->v + "' must definition in namespace");
    }
}
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 564 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    if((yyvsp[(2) - (5)]))
    {
        g_parse->popContainer();
    }
    (yyval) = (yyvsp[(2) - (5)]);

    StructPtr st = StructPtr::dynamicCast((yyval));
    assert(st);
    if(st->getAllMemberPtr().size() == 0)
    {
        g_parse->error("struct `" + st->getSid() + "' must have at least one member");
    }
}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 584 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = (yyvsp[(2) - (2)]);
}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 588 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(2) - (2)]));

    g_parse->error("keyword `" + ident->v + "' cannot be used as struct name");
}
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 594 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    g_parse->error("abstract declarator '<anonymous struct>' used as declaration");
}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 603 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {

}
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 607 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
   g_parse->error("';' missing after definition");
}
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 611 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 621 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(1) - (1)]));
}
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 630 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StructPtr np = StructPtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
        IntergerGrammarPtr iPtr = IntergerGrammarPtr::dynamicCast((yyvsp[(1) - (3)]));
        g_parse->checkTag(iPtr->v);

        TypeIdPtr tPtr  = TypeIdPtr::dynamicCast((yyvsp[(3) - (3)]));
        tPtr->setRequire(iPtr->v);
        np->addTypeId(tPtr);
        (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(3) - (3)]));
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 648 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StructPtr np = StructPtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
        IntergerGrammarPtr iPtr = IntergerGrammarPtr::dynamicCast((yyvsp[(1) - (5)]));
        g_parse->checkTag(iPtr->v);

        TypeIdPtr tPtr   = TypeIdPtr::dynamicCast((yyvsp[(3) - (5)]));
        ConstGrammarPtr sPtr = ConstGrammarPtr::dynamicCast((yyvsp[(5) - (5)]));
        g_parse->checkConstValue(tPtr, sPtr->t);

        tPtr->setRequire(iPtr->v);
        tPtr->setDefault(sPtr->v);
        np->addTypeId(tPtr);
        (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(3) - (5)]));
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 670 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StructPtr np = StructPtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
        IntergerGrammarPtr iPtr = IntergerGrammarPtr::dynamicCast((yyvsp[(1) - (5)]));
        g_parse->checkTag(iPtr->v);

        TypeIdPtr tPtr   = TypeIdPtr::dynamicCast((yyvsp[(3) - (5)]));
        ConstGrammarPtr sPtr = ConstGrammarPtr::dynamicCast((yyvsp[(5) - (5)]));
        g_parse->checkConstValue(tPtr, sPtr->t);

        tPtr->setOptional(iPtr->v);
        tPtr->setDefault(sPtr->v);
        np->addTypeId(tPtr);
        (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(3) - (5)]));
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 692 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StructPtr np = StructPtr::dynamicCast(g_parse->currentContainer());
    if(np)
    {
        IntergerGrammarPtr iPtr = IntergerGrammarPtr::dynamicCast((yyvsp[(1) - (3)]));
        g_parse->checkTag(iPtr->v);
        TypeIdPtr tPtr = TypeIdPtr::dynamicCast((yyvsp[(3) - (3)]));
        tPtr->setOptional(iPtr->v);
        np->addTypeId(tPtr);
        (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(3) - (3)]));
    }
    else
    {
        (yyval) = 0;
    }
}
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 709 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    g_parse->error("struct member need 'tag'");
}
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 713 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    g_parse->error("struct member need 'tag'");
}
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 717 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    g_parse->error("struct member need 'require' or 'optional'");
}
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 721 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    g_parse->error("struct member need 'tag' or 'require' or 'optional'");
}
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 730 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    IntergerGrammarPtr intVal = IntergerGrammarPtr::dynamicCast((yyvsp[(1) - (1)]));
    ostringstream sstr;
    sstr << intVal->v;
    ConstGrammarPtr c = new ConstGrammar();
    c->t = ConstGrammar::VALUE;
    c->v = sstr.str();
    (yyval) = c;
}
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 740 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    FloatGrammarPtr floatVal = FloatGrammarPtr::dynamicCast((yyvsp[(1) - (1)]));
    ostringstream sstr;
    sstr << floatVal->v;
    ConstGrammarPtr c = new ConstGrammar();
    c->t = ConstGrammar::VALUE;
    c->v = sstr.str();
    (yyval) = c;
}
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 750 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(1) - (1)]));
    ConstGrammarPtr c = new ConstGrammar();
    c->t = ConstGrammar::STRING;
    c->v = ident->v;
    (yyval) = c;
}
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 758 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(1) - (1)]));
    ConstGrammarPtr c = new ConstGrammar();
    c->t = ConstGrammar::BOOL;
    c->v = ident->v;
    (yyval) = c;
}
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 766 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(1) - (1)]));
    ConstGrammarPtr c = new ConstGrammar();
    c->t = ConstGrammar::BOOL;
    c->v = ident->v;
    (yyval) = c;
}
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 774 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(1) - (1)]));

    if (g_parse->checkEnum(ident->v) == false)
    {
        g_parse->error("error enum default value, not defined yet");
    }
    ConstGrammarPtr c = new ConstGrammar();
    c->t = ConstGrammar::ENUM;
    c->v = ident->v;
    (yyval) = c;
}
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 787 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {

    StringGrammarPtr scoped = StringGrammarPtr::dynamicCast((yyvsp[(1) - (3)]));
    StringGrammarPtr ident  = StringGrammarPtr::dynamicCast((yyvsp[(3) - (3)]));
    
    if (g_parse->checkEnum(ident->v) == false)
    {
        g_parse->error("error enum default value, not defined yet");
    }
    ConstGrammarPtr c = new ConstGrammar();
    c->t = ConstGrammar::ENUM;
    c->v = scoped->v + "::" + ident->v;
    (yyval) = c;
}
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 807 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    NamespacePtr np = NamespacePtr::dynamicCast(g_parse->currentContainer());
    if(!np)
    {
        g_parse->error("const type must define in namespace");
    }

    TypeIdPtr t   = TypeIdPtr::dynamicCast((yyvsp[(2) - (4)]));
    ConstGrammarPtr c = ConstGrammarPtr::dynamicCast((yyvsp[(4) - (4)]));
    ConstPtr cPtr = np->createConst(t, c);
    (yyval) = cPtr;
}
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 825 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    TypePtr type = TypePtr::dynamicCast((yyvsp[(1) - (2)]));
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(2) - (2)]));

    TypeIdPtr typeIdPtr = new TypeId(type, ident->v);

    (yyval) = GrammarBasePtr::dynamicCast(typeIdPtr);
}
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 834 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    TypePtr type = g_parse->createVector(TypePtr::dynamicCast((yyvsp[(1) - (5)])));
    IntergerGrammarPtr iPtrSize = IntergerGrammarPtr::dynamicCast((yyvsp[(4) - (5)]));
    g_parse->checkArrayVaid(type,iPtrSize->v);
    type->setArray(iPtrSize->v);
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(2) - (5)]));
    TypeIdPtr typeIdPtr = new TypeId(type, ident->v);
    (yyval) = GrammarBasePtr::dynamicCast(typeIdPtr);
}
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 844 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    TypePtr type = g_parse->createVector(TypePtr::dynamicCast((yyvsp[(1) - (3)])));
    //IntergerGrammarPtr iPtrSize = IntergerGrammarPtr::dynamicCast($4);
    g_parse->checkPointerVaid(type);
    type->setPointer(true);
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(3) - (3)]));
    TypeIdPtr typeIdPtr = new TypeId(type, ident->v);
    (yyval) = GrammarBasePtr::dynamicCast(typeIdPtr);
}
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 854 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    TypePtr type = TypePtr::dynamicCast((yyvsp[(1) - (4)]));
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(2) - (4)]));
    TypeIdPtr typeIdPtr = new TypeId(type, ident->v);
    IntergerGrammarPtr iPtrSize = IntergerGrammarPtr::dynamicCast((yyvsp[(3) - (4)]));
    g_parse->checkArrayVaid(type,iPtrSize->v);
    (yyval) = GrammarBasePtr::dynamicCast(typeIdPtr);
}
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 863 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(2) - (2)]));
    g_parse->error("keyword `" + ident->v + "' cannot be used as data member name");
}
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 868 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    g_parse->error("missing data member name");
}
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 872 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    g_parse->error("unkown type");
}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 881 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {

    TypePtr type = TypePtr::dynamicCast((yyvsp[(1) - (3)]));
    IntergerGrammarPtr iPtrSize = IntergerGrammarPtr::dynamicCast((yyvsp[(3) - (3)]));
    g_parse->checkArrayVaid(type,iPtrSize->v);
    type->setArray(iPtrSize->v);
    (yyval) = type;
}
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 890 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = (yyvsp[(1) - (1)]);
}
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 894 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
   g_parse->error("array missing size");
}
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 903 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindBool);
}
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 907 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindByte);
}
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 911 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindShort,true);
}
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 915 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindShort);
}
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 919 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindInt,true);
}
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 923 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindInt);
}
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 927 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindLong,true);
}
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 931 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindLong);
}
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 935 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindFloat);
}
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 939 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindDouble);
}
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 943 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    (yyval) = g_parse->createBuiltin(Builtin::KindString);
}
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 947 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
   (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(1) - (1)]));
}
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 951 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
   (yyval) = GrammarBasePtr::dynamicCast((yyvsp[(1) - (1)]));
}
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 955 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(1) - (1)]));
    TypePtr sp = g_parse->findUserType(ident->v);
    if(sp)
    {
        (yyval) = GrammarBasePtr::dynamicCast(sp);
    }
    else
    {
        g_parse->error("'" + ident->v + "' undefined!");
    }
}
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 973 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
   (yyval) = GrammarBasePtr::dynamicCast(g_parse->createVector(TypePtr::dynamicCast((yyvsp[(3) - (4)]))));
}
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 977 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
   g_parse->error("vector error");
}
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 981 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
   g_parse->error("vector missing '>'");
}
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 985 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
   g_parse->error("vector missing type");
}
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 994 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
   (yyval) = GrammarBasePtr::dynamicCast(g_parse->createMap(TypePtr::dynamicCast((yyvsp[(3) - (6)])), TypePtr::dynamicCast((yyvsp[(5) - (6)]))));
}
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 998 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
   g_parse->error("map error");
}
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 1007 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 1010 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr ident = StringGrammarPtr::dynamicCast((yyvsp[(2) - (2)]));
    ident->v = "::" + ident->v;
    (yyval) = GrammarBasePtr::dynamicCast(ident);
}
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 1016 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
    StringGrammarPtr scoped = StringGrammarPtr::dynamicCast((yyvsp[(1) - (3)]));
    StringGrammarPtr ident  = StringGrammarPtr::dynamicCast((yyvsp[(3) - (3)]));
    scoped->v += "::";
    scoped->v += ident->v;
    (yyval) = GrammarBasePtr::dynamicCast(scoped);
}
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 1029 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 1032 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 1035 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 1038 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 1041 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 1044 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 1047 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 1050 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 1053 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 1056 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 1059 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 1062 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 1065 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 1068 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 1071 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 1074 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 1077 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 1080 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 1083 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 1086 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 1089 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 1092 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 1095 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 1098 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 1101 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"
    {
}
    break;


/* Line 1792 of yacc.c  */
#line 3029 "tars.tab.cpp"
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 1105 "/home/worker/Centos_share/src/TarsCloud/UP-Tars/tars_kiven_dev/TarsCpp/tools/tarsgrammar/tars.y"



