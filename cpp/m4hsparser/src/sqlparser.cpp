
/*  A Bison parser, made from sqlparser.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	LEXER_ERROR	258
#define	LITERAL_INTEGER	259
#define	LITERAL_FLOAT	260
#define	LITERAL_STR	261
#define	LITERAL_DATE	262
#define	LITERAL_TIME_STAMP	263
#define	IDENTIFIER	264
#define	SELECT	265
#define	FROM	266
#define	GROUP_BY	267
#define	WHERE	268
#define	UNION	269
#define	UNION_ALL	270
#define	INTERSECT	271
#define	IN_KW	272
#define	ALL	273
#define	DISTINCT	274
#define	EXISTS	275
#define	HAVING	276
#define	ORDER_BY	277
#define	ASC	278
#define	DESC	279
#define	OPE_OR	280
#define	OPE_AND	281
#define	OPE_NOT	282
#define	OPE_EQU	283
#define	OPE_NOT_EQU	284
#define	OPE_GT	285
#define	OPE_LT	286
#define	OPE_GE	287
#define	OPE_LE	288
#define	OPE_LIKE	289
#define	OPE_NOT_LIKE	290
#define	OPE_BETWEEN	291
#define	OPE_NOT_BETWEEN	292
#define	OPE_IS_NULL	293
#define	OPE_IS_NOT_NULL	294
#define	OPE_ADD	295
#define	OPE_SUBS	296
#define	OPE_CONCAT_STR	297
#define	OPE_MULT	298
#define	OPE_DIV	299
#define	OPE_MINUS_SIGN	300
#define	OPE_PLUS_SIGN	301
#define	OPEN_PAREN	302
#define	CLOSE_PAREN	303
#define	OPE_OUTER_JOIN	304
#define	OPE_COMMA	305
#define	OPE_POINT	306
#define	OPE_AT	307
#define	OPE_AMP	308
#define	OPE_ALM	309
#define	OPE_DOL	310




// Donde se define la clase SentencePArser a la que pertenecerá yyparse
#include "sentenceparser.hpp"

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		145
#define	YYFLAG		-32768
#define	YYNTBASE	56

#define YYTRANSLATE(x) ((unsigned)(x) <= 310 ? yytranslate[x] : 81)

static const char yytranslate[] = {     0,
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
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     4,     6,     8,    10,    14,    18,    21,    24,
    26,    29,    32,    34,    38,    39,    41,    47,    51,    55,
    59,    63,    66,    69,    73,    77,    79,    81,    84,    86,
    88,    90,    92,    94,    96,    98,   100,   104,   107,   111,
   117,   121,   123,   126,   129,   133,   137,   143,   149,   151,
   155,   159,   163,   166,   167,   169,   173,   175,   177,   179,
   181,   185,   187,   191,   192,   195,   196,   199,   204,   206,
   209,   212,   214,   218,   219,   222,   223,   233,   234,   241,
   245,   247,   251,   255
};

static const short yyrhs[] = {     4,
     0,     5,     0,     6,     0,     7,     0,     9,     0,     9,
    51,     9,     0,    53,     9,     9,     0,     9,     9,     0,
    53,     9,     0,     9,     0,    52,     9,     0,    55,     9,
     0,    63,     0,    60,    50,    63,     0,     0,    60,     0,
    54,     9,    47,    61,    48,     0,    63,    40,    63,     0,
    63,    41,    63,     0,    63,    43,    63,     0,    63,    44,
    63,     0,    41,    63,     0,    40,    63,     0,    63,    42,
    63,     0,    47,    63,    48,     0,    59,     0,    57,     0,
    57,    49,     0,    62,     0,    56,     0,    28,     0,    29,
     0,    30,     0,    31,     0,    32,     0,    33,     0,    63,
    64,    80,     0,    20,    80,     0,    63,    17,    80,     0,
    63,    17,    47,    60,    48,     0,    63,    64,    63,     0,
    66,     0,    63,    38,     0,    63,    39,     0,    63,    34,
    63,     0,    63,    35,    63,     0,    63,    36,    63,    26,
    63,     0,    63,    37,    63,    26,    63,     0,    65,     0,
    67,    26,    67,     0,    67,    25,    67,     0,    47,    67,
    48,     0,    27,    67,     0,     0,    19,     0,    59,    28,
    63,     0,    57,     0,    62,     0,    56,     0,    69,     0,
    70,    50,    69,     0,    58,     0,    71,    50,    58,     0,
     0,    13,    67,     0,     0,    12,    60,     0,    12,    60,
    21,    67,     0,    63,     0,    63,    23,     0,    63,    24,
     0,    74,     0,    75,    50,    74,     0,     0,    22,    75,
     0,     0,    10,    78,    68,    70,    11,    71,    72,    73,
    76,     0,     0,    79,    11,    71,    72,    73,    76,     0,
    47,    77,    48,     0,    77,     0,    77,    14,    80,     0,
    77,    16,    80,     0,     3,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    95,    98,   101,   104,   110,   114,   121,   126,   130,   135,
   142,   146,   159,   163,   173,   176,   182,   199,   205,   211,
   217,   223,   229,   235,   241,   249,   253,   257,   262,   266,
   275,   278,   281,   284,   287,   290,   296,   302,   310,   316,
   331,   345,   349,   354,   359,   365,   371,   380,   389,   393,
   412,   431,   439,   451,   454,   462,   469,   473,   477,   484,
   488,   497,   501,   510,   512,   520,   523,   527,   537,   541,
   546,   554,   558,   565,   567,   576,   584,   597,   605,   620,
   626,   631,   637,   643
};

static const char * const yytname[] = {   "$","error","$undefined.","LEXER_ERROR",
"LITERAL_INTEGER","LITERAL_FLOAT","LITERAL_STR","LITERAL_DATE","LITERAL_TIME_STAMP",
"IDENTIFIER","SELECT","FROM","GROUP_BY","WHERE","UNION","UNION_ALL","INTERSECT",
"IN_KW","ALL","DISTINCT","EXISTS","HAVING","ORDER_BY","ASC","DESC","OPE_OR",
"OPE_AND","OPE_NOT","OPE_EQU","OPE_NOT_EQU","OPE_GT","OPE_LT","OPE_GE","OPE_LE",
"OPE_LIKE","OPE_NOT_LIKE","OPE_BETWEEN","OPE_NOT_BETWEEN","OPE_IS_NULL","OPE_IS_NOT_NULL",
"OPE_ADD","OPE_SUBS","OPE_CONCAT_STR","OPE_MULT","OPE_DIV","OPE_MINUS_SIGN",
"OPE_PLUS_SIGN","OPEN_PAREN","CLOSE_PAREN","OPE_OUTER_JOIN","OPE_COMMA","OPE_POINT",
"OPE_AT","OPE_AMP","OPE_ALM","OPE_DOL","CONSTANT","column_name","table_name",
"item_name","expr_nelist","expr_list","function","expr","COMPARISON_OPE","quantified_expr",
"in_expr","condition","select_modif_row","displayed_column","displayed_list",
"table_list","where_clause","group_clause","sorted_def","sorted_list","order_clause",
"select_command","@1","@2","command",""
};
#endif

static const short yyr1[] = {     0,
    56,    56,    56,    56,    57,    57,    58,    58,    58,    58,
    59,    59,    60,    60,    61,    61,    62,    63,    63,    63,
    63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
    64,    64,    64,    64,    64,    64,    65,    65,    66,    66,
    67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
    67,    67,    67,    68,    68,    69,    69,    69,    69,    70,
    70,    71,    71,    72,    72,    73,    73,    73,    74,    74,
    74,    75,    75,    76,    76,    78,    77,    79,    77,    80,
    80,    80,    80,    80
};

static const short yyr2[] = {     0,
     1,     1,     1,     1,     1,     3,     3,     2,     2,     1,
     2,     2,     1,     3,     0,     1,     5,     3,     3,     3,
     3,     2,     2,     3,     3,     1,     1,     2,     1,     1,
     1,     1,     1,     1,     1,     1,     3,     2,     3,     5,
     3,     1,     2,     2,     3,     3,     5,     5,     1,     3,
     3,     3,     2,     0,     1,     3,     1,     1,     1,     1,
     3,     1,     3,     0,     2,     0,     2,     4,     1,     2,
     2,     1,     3,     0,     2,     0,     9,     0,     6,     3,
     1,     3,     3,     1
};

static const short yydefact[] = {    78,
    84,    76,    78,    81,     0,    54,     0,    78,    78,     0,
    55,     0,    80,    82,    83,    10,     0,    62,    64,     1,
     2,     3,     4,     5,     0,     0,     0,    59,    57,     0,
    58,    60,     0,     8,     9,     0,     0,    66,     0,    11,
     0,    12,     0,     0,     0,     7,    78,     0,     0,     0,
     0,    30,    27,    26,    29,     0,    49,    42,    65,    63,
     0,    74,     6,    15,     0,    56,    64,    61,    38,    53,
    23,    22,     0,     0,    28,    78,    31,    32,    33,    34,
    35,    36,     0,     0,     0,     0,    43,    44,     0,     0,
     0,     0,     0,    78,     0,     0,    67,    13,     0,    79,
    16,     0,     0,    66,    25,    52,    78,    39,    45,    46,
     0,     0,    18,    19,    24,    20,    21,    78,    41,    37,
    51,    50,     0,     0,    69,    72,    75,    17,    74,     0,
     0,     0,    68,    14,    70,    71,     0,    77,    40,    47,
    48,    73,     0,     0,     0
};

static const short yydefgoto[] = {    52,
    53,    18,    54,    97,   102,    55,    56,    94,    57,    58,
    59,    12,    32,    33,    19,    38,    62,   126,   127,   100,
     4,     6,     5,    14
};

static const short yypact[] = {     7,
-32768,-32768,    -6,   -11,    10,    37,    36,     7,     7,    -7,
-32768,    31,-32768,-32768,-32768,    62,    70,-32768,    18,-32768,
-32768,-32768,-32768,     8,    88,    92,    97,-32768,-32768,    80,
-32768,-32768,    -5,-32768,   100,    87,    -7,    98,   102,-32768,
    73,-32768,   131,    -7,    31,-32768,     7,    87,   131,   131,
    87,-32768,    74,-32768,-32768,   187,-32768,-32768,    48,-32768,
   131,   104,-32768,   131,   131,   114,    18,-32768,-32768,-32768,
-32768,-32768,   159,    55,-32768,    15,-32768,-32768,-32768,-32768,
-32768,-32768,   131,   131,   131,   131,-32768,-32768,   131,   131,
   131,   131,   131,    23,    87,    87,     3,   114,   131,-32768,
    75,    76,   103,    98,-32768,-32768,   112,-32768,   114,   114,
   206,   211,   -29,   -29,   -29,-32768,-32768,   112,   114,-32768,
   122,-32768,    87,   131,    89,-32768,    99,-32768,   104,    50,
   131,   131,    48,   114,-32768,-32768,   131,-32768,-32768,   114,
   114,-32768,   150,   160,-32768
};

static const short yypgoto[] = {    22,
    27,   124,    54,   -52,-32768,    57,   -42,-32768,-32768,-32768,
   -35,-32768,   117,-32768,   119,   101,    61,    32,-32768,    41,
    -3,-32768,-32768,    11
};


#define	YYLAST		255


static const short yytable[] = {     7,
    66,    16,     8,     2,     9,    44,    71,    72,    73,     1,
   143,   101,    70,    92,    93,    74,     2,     1,    98,    15,
    10,    98,   103,   123,     2,     1,    20,    21,    22,    23,
    36,    24,     2,    28,    20,    21,    22,    23,    29,    24,
   109,   110,   111,   112,    45,    17,   113,   114,   115,   116,
   117,   119,   124,     3,   130,    11,   125,    69,    39,   121,
   122,   107,    49,    50,    98,    30,    28,    37,    31,   118,
    34,    29,    95,    96,    25,   103,    26,    27,    35,    95,
    96,   134,    25,    13,    26,    27,   108,   133,   140,   141,
    20,    21,    22,    23,   125,    24,    40,   139,    30,   124,
    41,    31,   106,     7,   120,    42,    47,    43,    46,    61,
    63,   135,   136,    48,     7,    20,    21,    22,    23,    64,
    24,     2,    75,   128,   124,    99,    49,    50,    89,    90,
    91,    92,    93,    51,    20,    21,    22,    23,    25,    24,
    26,    27,    89,    90,    91,    92,    93,    96,   137,   144,
   105,    49,    50,    89,    90,    91,    92,    93,    65,   145,
    60,    68,    67,    25,   129,    26,    27,   104,   142,   138,
    49,    50,     0,     0,     0,    76,     0,    65,     0,     0,
     0,     0,    25,     0,    26,    27,    77,    78,    79,    80,
    81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
    91,    92,    93,    76,     0,     0,   105,     0,     0,     0,
     0,     0,     0,     0,    77,    78,    79,    80,    81,    82,
    83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
    93,   131,     0,     0,     0,     0,   132,     0,     0,     0,
     0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
    89,    90,    91,    92,    93
};

static const short yycheck[] = {     3,
    43,     9,    14,    10,    16,    11,    49,    50,    51,     3,
     0,    64,    48,    43,    44,    51,    10,     3,    61,     9,
    11,    64,    65,    21,    10,     3,     4,     5,     6,     7,
    13,     9,    10,    12,     4,     5,     6,     7,    12,     9,
    83,    84,    85,    86,    50,    53,    89,    90,    91,    92,
    93,    94,    50,    47,   107,    19,    99,    47,    51,    95,
    96,    47,    40,    41,   107,    12,    45,    50,    12,    47,
     9,    45,    25,    26,    52,   118,    54,    55,     9,    25,
    26,   124,    52,    48,    54,    55,    76,   123,   131,   132,
     4,     5,     6,     7,   137,     9,     9,    48,    45,    50,
     9,    45,    48,   107,    94,     9,    20,    28,     9,    12,
     9,    23,    24,    27,   118,     4,     5,     6,     7,    47,
     9,    10,    49,    48,    50,    22,    40,    41,    40,    41,
    42,    43,    44,    47,     4,     5,     6,     7,    52,     9,
    54,    55,    40,    41,    42,    43,    44,    26,    50,     0,
    48,    40,    41,    40,    41,    42,    43,    44,    47,     0,
    37,    45,    44,    52,   104,    54,    55,    67,   137,   129,
    40,    41,    -1,    -1,    -1,    17,    -1,    47,    -1,    -1,
    -1,    -1,    52,    -1,    54,    55,    28,    29,    30,    31,
    32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
    42,    43,    44,    17,    -1,    -1,    48,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    28,    29,    30,    31,    32,    33,
    34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
    44,    26,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    40,    41,    42,    43,    44,
    40,    41,    42,    43,    44
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */


/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

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
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

//extern int yylex();
//extern void yyerror(char *);

#ifdef __GNUC__
#define alloca __builtin_alloca

#else /* not __GNUC__ */
#if HAVE_ALLOCA_H
#include <alloca.h>

#else /* not HAVE_ALLOCA_H */
#ifdef _AIX
#pragma alloca

#else /* not _AIX */
#ifdef _HP
#include <alloca.h>

#else /* not _HP */
char *alloca ();

#endif /* not _HP */
#endif /* not _AIX */
#endif /* not HAVE_ALLOCA_H */
#endif /* not __GNUC__ */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
#pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
	void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif


#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
SentenceParser::yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      size_t size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, (int)(size * sizeof (*yyssp)));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, (int)(size * sizeof (*yyvsp)));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
{ 
								yyval = yyvsp[0];
							;
    break;}
case 2:
{ 
								yyval = yyvsp[0];
							;
    break;}
case 3:
{  
								yyval = yyvsp[0];
							;
    break;}
case 4:
{ 
								yyval = yyvsp[0];
							;
    break;}
case 5:
{
					yyval.poColumn = m_oFactory.CreateColumnInfo ("", yyvsp[0].pcValue);
				;
    break;}
case 6:
{
					yyval.poColumn = m_oFactory.CreateColumnInfo (yyvsp[-2].pcValue, yyvsp[0].pcValue);
				;
    break;}
case 7:
{
					yyval.poTable = m_oFactory.CreateTableInfo ( yyvsp[-1].pcValue, yyvsp[0].pcValue);
					yyval.poTable->SetIsBase (true);
				;
    break;}
case 8:
{
					yyval.poTable = m_oFactory.CreateTableInfo ( yyvsp[-1].pcValue, yyvsp[0].pcValue);
				;
    break;}
case 9:
{
					yyval.poTable = m_oFactory.CreateTableInfo ( yyvsp[0].pcValue, "");
					yyval.poTable->SetIsBase (true);
				;
    break;}
case 10:
{
					yyval.poTable = m_oFactory.CreateTableInfo ( yyvsp[0].pcValue, "");
				;
    break;}
case 11:
{
					yyval.poItem = m_oFactory.CreateItemInfo (yyvsp[0].pcValue);
				;
    break;}
case 12:
{
					yyval.poItem = m_oFactory.CreateItemInfo (yyvsp[0].pcValue);
					yyval.poItem->SetSessionChannel (M4_TRUE);
				;
    break;}
case 13:
{ 
					yyval = yyvsp[0];
				;
    break;}
case 14:
{   
					yyval.poText = m_oFactory.CreateTextInfo (",");
					yyval.poText->AddLeft (yyvsp[-2].poElement);
					yyval.poText->AddRight (yyvsp[0].poElement);
				;
    break;}
case 15:
{
									yyval.poElement = 0; 
								;
    break;}
case 16:
{ 
									yyval = yyvsp[0];
								;
    break;}
case 17:
{
					yyval.poFunction = m_oFactory.CreateFunctionInfo ( yyvsp[-3].pcValue );
					yyval.poFunction->SetArgument (yyvsp[-1].poElement);
				;
    break;}
case 18:
{
					yyval.poElement = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);
					yyval.poElement->AddLeft (yyvsp[-2].poElement);
					yyval.poElement->AddRight (yyvsp[0].poElement);
				;
    break;}
case 19:
{
					yyval.poElement = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);
					yyval.poElement->AddLeft (yyvsp[-2].poElement);
					yyval.poElement->AddRight (yyvsp[0].poElement);
				;
    break;}
case 20:
{
					yyval.poElement = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);
					yyval.poElement->AddLeft (yyvsp[-2].poElement);
					yyval.poElement->AddRight (yyvsp[0].poElement);
				;
    break;}
case 21:
{
					yyval.poElement = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);
					yyval.poElement->AddLeft (yyvsp[-2].poElement);
					yyval.poElement->AddRight (yyvsp[0].poElement);
				;
    break;}
case 22:
{
					TextInfo * poTextInfoLeft = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);
					yyvsp[0].poElement->AddLeft (poTextInfoLeft);
					yyval = yyvsp[0];
				;
    break;}
case 23:
{
					TextInfo * poTextInfoLeft = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);
					yyvsp[0].poElement->AddLeft (poTextInfoLeft);
					yyval = yyvsp[0];
				;
    break;}
case 24:
{
					yyval.poElement = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);
					yyval.poElement->AddLeft (yyvsp[-2].poElement);
					yyval.poElement->AddRight (yyvsp[0].poElement);
				;
    break;}
case 25:
{
					TextInfo * poTextInfoLeft = m_oFactory.CreateTextInfo (yyvsp[-2].pcValue);
					TextInfo * poTextInfoRight = m_oFactory.CreateTextInfo (yyvsp[0].pcValue);
					yyvsp[-1].poElement->PushLeft (poTextInfoLeft);
					yyvsp[-1].poElement->PushRight (poTextInfoRight);
					yyval = yyvsp[-1];
				;
    break;}
case 26:
{
					yyval = yyvsp[0];
				;
    break;}
case 27:
{
					yyval = yyvsp[0];
				;
    break;}
case 28:
{
					yyval.poColumn->SetHasOuterJoin (M4_TRUE);
					yyval = yyvsp[-1];
				;
    break;}
case 29:
{
					yyval = yyvsp[0];
				;
    break;}
case 30:
{
					yyval.poText = m_oFactory.CreateTextInfo (yyvsp[0].pcValue)
				;
    break;}
case 31:
{  
									yyval = yyvsp[0];
								;
    break;}
case 32:
{   
									yyval = yyvsp[0];
								;
    break;}
case 33:
{   
									yyval = yyvsp[0];
								;
    break;}
case 34:
{   
									yyval = yyvsp[0];
								;
    break;}
case 35:
{   
									yyval = yyvsp[0];
								;
    break;}
case 36:
{   
									yyval = yyvsp[0];
								;
    break;}
case 37:
{
						yyval.poText = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);
						yyval.poText->AddLeft (yyvsp[-2].poElement);
						yyval.poText->AddRight (yyvsp[0].poElement);
					;
    break;}
case 38:
{
						yyval.poText = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);
						yyval.poText->AddRight (yyvsp[0].poElement);
					;
    break;}
case 39:
{
					yyval.poText = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);
					yyval.poText->AddLeft (yyvsp[-2].poElement);
					yyval.poText->AddRight (yyvsp[0].poElement);
			;
    break;}
case 40:
{
					TextInfo * poTextLeft = m_oFactory.CreateTextInfo (yyvsp[-2].pcValue);
					TextInfo * poTextRight = m_oFactory.CreateTextInfo (yyvsp[0].pcValue);
					yyvsp[-1].poElement->PushLeft (poTextLeft);
					yyvsp[-1].poElement->PushRight (poTextRight);
					yyval.poText = m_oFactory.CreateTextInfo (yyvsp[-3].pcValue);
					yyval.poText->AddLeft (yyvsp[-4].poElement);
					yyval.poText->AddRight (yyvsp[-1].poElement);
			;
    break;}
case 41:
{
					// Comprobamos si es un join 
					if ( (yyvsp[-2].poElement->GetType() == COLUMNINFO) && (yyvsp[0].poElement->GetType() == COLUMNINFO) && (!strcmp (yyvsp[-1].pcValue, "=")) )
					{
						yyval.poJoin = m_oFactory.CreateJoinInfo (viTreeExpresionLevel.front());
						vpoSelect.front()->AddJoin (yyval.poJoin);
					}
					else
						yyval.poText = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);  

					yyval.poElement->AddLeft (yyvsp[-2].poElement);
					yyval.poElement->AddRight (yyvsp[0].poElement);
				;
    break;}
case 42:
{
					yyval = yyvsp[0];
				;
    break;}
case 43:
{
					yyval.poText = m_oFactory.CreateTextInfo (yyvsp[0].pcValue);  
					yyval.poElement->AddLeft (yyvsp[-1].poElement);
				;
    break;}
case 44:
{
					yyval.poText = m_oFactory.CreateTextInfo (yyvsp[0].pcValue);  
					yyval.poElement->AddLeft (yyvsp[-1].poElement);
				;
    break;}
case 45:
{
					yyval.poText = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);  
					yyval.poElement->AddLeft (yyvsp[-2].poElement);
					yyval.poElement->AddRight (yyvsp[0].poElement);
				;
    break;}
case 46:
{
					yyval.poText = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);  
					yyval.poElement->AddLeft (yyvsp[-2].poElement);
					yyval.poElement->AddRight (yyvsp[0].poElement);
				;
    break;}
case 47:
{
					TextInfo * poTextInfo2 = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);
					poTextInfo2->AddLeft (yyvsp[-2].poElement);
					poTextInfo2->AddRight (yyvsp[0].poElement);
					yyval.poText = m_oFactory.CreateTextInfo (yyvsp[-3].pcValue);
					yyval.poText->AddLeft (yyvsp[-4].poElement);
					yyval.poText->AddRight (poTextInfo2);
				;
    break;}
case 48:
{
					TextInfo * poTextInfo2 = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);
					poTextInfo2->AddLeft (yyvsp[-2].poElement);
					poTextInfo2->AddRight (yyvsp[0].poElement);
					yyval.poText = m_oFactory.CreateTextInfo (yyvsp[-3].pcValue);
					yyval.poText->AddLeft (yyvsp[-4].poElement);
					yyval.poText->AddRight (poTextInfo2);
				;
    break;}
case 49:
{ 
					yyval = yyvsp[0];
				;
    break;}
case 50:
{
					yyval.poText = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);  
					yyval.poElement->AddLeft (yyvsp[-2].poElement);
					yyval.poElement->AddRight (yyvsp[0].poElement);
					// Si cambiamos de operador aumentamos un nivel lógico en la expresión
					if (!vbLastIsAnd.front())
					{
						viTreeExpresionLevel.front() ++;
						vbLastIsAnd.front() = M4_TRUE;

						// Si ha cambiado comprobamos si alguna de las condiciones
						// es unjoin y si le asignamos este nivel en el árbol a postriori
						if (yyvsp[-2].poElement->GetType() == JOININFO)
							yyvsp[-2].poJoin->SetTreeLevel (viTreeExpresionLevel.front());
						if (yyvsp[0].poElement->GetType() == JOININFO)
							yyvsp[0].poJoin->SetTreeLevel (viTreeExpresionLevel.front());
					}
				;
    break;}
case 51:
{
					yyval.poText = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);  
					yyval.poElement->AddLeft (yyvsp[-2].poElement);
					yyval.poElement->AddRight (yyvsp[0].poElement);
					// Si cambiamos de operador aumentamos un nivel lógico en la expresión
					if (vbLastIsAnd.front())
					{
						viTreeExpresionLevel.front() ++;
						vbLastIsAnd.front() = M4_FALSE;
						
						// Si ha cambiado comprobamos si alguna de las condiciones
						// es unjoin y si le asignamos este nivel en el árbol a postriori
						if (yyvsp[-2].poElement->GetType() == JOININFO)
							yyvsp[-2].poJoin->SetTreeLevel (viTreeExpresionLevel.front());
						if (yyvsp[0].poElement->GetType() == JOININFO)
							yyvsp[0].poJoin->SetTreeLevel (viTreeExpresionLevel.front());
					}
				;
    break;}
case 52:
{
					TextInfo * poTextInfoLeft = m_oFactory.CreateTextInfo(yyvsp[-2].pcValue);
					TextInfo * poTextInfoRight = m_oFactory.CreateTextInfo(yyvsp[0].pcValue);
					yyvsp[-1].poElement->PushLeft (poTextInfoLeft);
					yyvsp[-1].poElement->PushRight (poTextInfoRight);
					yyval = yyvsp[-1];
				;
    break;}
case 53:
{
					yyval.poText = m_oFactory.CreateTextInfo(yyvsp[-1].pcValue);
					yyval.poText->AddRight (yyvsp[0].poElement);
				;
    break;}
case 54:
{  
									vpoSelect.front()->SetHasDistinct (false);
								;
    break;}
case 55:
{ 
									vpoSelect.front()->SetHasDistinct (true);
								;
    break;}
case 56:
{ 
						TextInfo * poTextInfo = m_oFactory.CreateTextInfo (yyvsp[-1].pcValue);
						poTextInfo->AddLeft (yyvsp[-2].poItem);
						poTextInfo->AddRight (yyvsp[0].poElement);
						yyval.poText = poTextInfo;
					;
    break;}
case 57:
{
						yyval = yyvsp[0];
					;
    break;}
case 58:
{
						yyval = yyvsp[0];
					;
    break;}
case 59:
{
						yyval.poText = m_oFactory.CreateTextInfo (yyvsp[0].pcValue);
					;
    break;}
case 60:
{ 
						vpoSelect.front()->AddColumn (yyvsp[0].poElement);
					;
    break;}
case 61:
{   
						vpoSelect.front()->AddColumn (yyvsp[0].poElement);
					;
    break;}
case 62:
{
					vpoSelect.front()->AddTable (yyvsp[0].poTable);
				;
    break;}
case 63:
{   
					vpoSelect.front()->AddTable (yyvsp[0].poTable);
				;
    break;}
case 64:
{ 
									;
    break;}
case 65:
{
										vpoSelect.front()->SetWhere (yyvsp[0].poElement);
									;
    break;}
case 66:
{ 
					;
    break;}
case 67:
{
						vpoSelect.front()->SetGroupBy (yyvsp[0].poElement);						
					;
    break;}
case 68:
{
						vpoSelect.front()->SetGroupBy (yyvsp[-2].poElement);
						vpoSelect.front()->SetHaving (yyvsp[0].poElement);
					;
    break;}
case 69:
{
					yyval = yyvsp[0];
				;
    break;}
case 70:
{
					yyval.poText = m_oFactory.CreateTextInfo ("ASC");
					yyval.poText->AddLeft (yyvsp[-1].poElement);
				;
    break;}
case 71:
{
					yyval.poText = m_oFactory.CreateTextInfo ("DESC");
					yyval.poText->AddLeft (yyvsp[-1].poElement);
				;
    break;}
case 72:
{
					vpoSelect.front()->AddOrderBy (yyvsp[0].poElement);
				;
    break;}
case 73:
{   
					vpoSelect.front()->AddOrderBy (yyvsp[0].poElement);
				;
    break;}
case 74:
{ 
										;
    break;}
case 75:
{ 
										;
    break;}
case 76:
{ 
							// Creamos el objeto select y lo metemos en la pila
							SelectInfo * poSelectInfo = m_oFactory.CreateSelectInfo ();
							vpoSelect.push_front (poSelectInfo);
							// Inicializamos variables temporales para la select					
							vbLastIsAnd.push_front(M4_TRUE);
							viTreeExpresionLevel.push_front(0);
						;
    break;}
case 77:
{
							// Ya hemos procesado la select
							// Sacamos el objeto select de la pila
							yyval.poSelect = vpoSelect.front();
							vpoSelect.pop_front();
							vbLastIsAnd.pop_front();
							viTreeExpresionLevel.pop_front();									
						;
    break;}
case 78:
{ 
							// Creamos el objeto select y lo metemos en la pila
							SelectInfo * poSelectInfo = m_oFactory.CreateSelectInfo ();
							vpoSelect.push_front (poSelectInfo);
							// Inicializamos variables temporales para la select					
							vbLastIsAnd.push_front(M4_TRUE);
							viTreeExpresionLevel.push_front(0);
						;
    break;}
case 79:
{
							// Ya hemos procesado la select
							// Sacamos el objet select de la pila
							yyval.poSelect = vpoSelect.front();
							vpoSelect.pop_front();									
							vbLastIsAnd.pop_front();
							viTreeExpresionLevel.pop_front();									
						;
    break;}
case 80:
{
					yyvsp[-1].poSelect->SetIsSubselect (true);
					yyval = yyvsp[-1];
				;
    break;}
case 81:
{ 
					poFinalSelect = yyvsp[0].poSelect;
					yyval = yyvsp[0];					
				;
    break;}
case 82:
{
					yyvsp[-2].poSelect->AddUnion (yyvsp[0].poSelect);
					poFinalSelect = yyvsp[-2].poSelect;
					yyval = yyvsp[-2];					
				;
    break;}
case 83:
{
					yyvsp[-2].poSelect->AddIntersect (yyvsp[0].poSelect);
					poFinalSelect = yyvsp[-2].poSelect;
					yyval = yyvsp[-2];					
				;
    break;}
case 84:
{ 
					yyerror ( NULL );
				;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}


/* 
*************************************************************************************************
*										PROGRAM SECTION											*
************************************************************************************************* 
*/




