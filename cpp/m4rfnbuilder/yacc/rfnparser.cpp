
/*  A Bison parser, made from rfnparser.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	RFN_LITERAL_INTEGER	258
#define	RFN_LITERAL_FLOAT	259
#define	RFN_LITERAL_STR	260
#define	RFN_LITERAL_BOOLEAN	261
#define	RFN_NOT	262
#define	RFN_HEADER	263
#define	RFN_OUTPUT_NAME	264
#define	RFN_FONT_NAME	265
#define	RFN_FIXED	266
#define	RFN_PCL	267
#define	RFN_PSC	268
#define	RFN_RVW	269
#define	RFN_ESCP	270
#define	RFN_MAP_NAME	271
#define	RFN_DVC_FONT_ID	272
#define	RFN_DVC_CHARSET	273
#define	RFN_DVC_PITCH	274
#define	RFN_DVC_2Y	275
#define	RFN_DVC_2X	276
#define	RFN_DVC_HEIGHT	277
#define	RFN_SELECTION_HEIGHT	278
#define	RFN_DVC_SOFT_FONT	279
#define	RFN_DVC_SOFT_FONT_TRANS	280
#define	RFN_DVC_COMPR	281
#define	RFN_HEIGHT	282
#define	RFN_MAP_HEIGHT	283
#define	RFN_ALL	284
#define	RFN_BOLD_ON	285
#define	RFN_BOLD_OFF	286
#define	RFN_BOLD_ALL	287
#define	RFN_ITALIC_ON	288
#define	RFN_ITALIC_OFF	289
#define	RFN_ITALIC_ALL	290
#define	RFN_UNDERLINE_ON	291
#define	RFN_UNDERLINE_OFF	292
#define	RFN_UNDERLINE_ALL	293
#define	RFN_DESIGN_HEIGHT	294
#define	RFN_DESIGN_WIDTH	295
#define	RFN_FOR_ALL	296
#define	RFN_OPEN_BLOCK	297
#define	RFN_CLOSE_BLOCK	298
#define	RFN_OPE_LET	299



#include "rfn.h"
#include "rfndf.inl"
#include "flexlexer.h"	//Para acceder a yylex()
#include "rfntrans.h"
#include "rfnstrt.h"
#include "rfnparser.h"


#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		96
#define	YYFLAG		-32768
#define	YYNTBASE	45

#define YYTRANSLATE(x) ((unsigned)(x) <= 299 ? yytranslate[x] : 68)

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
    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     4,     6,     8,    10,    12,    14,    16,    18,
    20,    22,    24,    26,    28,    30,    32,    34,    38,    42,
    46,    50,    52,    55,    57,    58,    60,    62,    64,    66,
    68,    70,    72,    74,    76,    80,    84,    88,    92,    96,
   100,   102,   107,   109,   112,   114,   115,   117,   119,   121,
   123,   128,   130,   133,   135,   138,   140,   141,   143,   146,
   148,   149,   154,   160,   166,   168,   171
};

static const short yyrhs[] = {     9,
     0,    10,     0,    16,     0,    17,     0,    18,     0,    19,
     0,    28,     0,    39,     0,    40,     0,    11,     0,    20,
     0,    21,     0,    26,     0,    22,     0,    23,     0,    24,
     0,    25,     0,    45,    44,     3,     0,    45,    44,     4,
     0,    45,    44,     5,     0,    45,    44,     6,     0,    46,
     0,    47,    46,     0,    47,     0,     0,    30,     0,    31,
     0,    32,     0,    33,     0,    34,     0,    35,     0,    36,
     0,    37,     0,    38,     0,    49,    50,    51,     0,    49,
    51,    50,     0,    50,    49,    51,     0,    50,    51,    49,
     0,    51,    49,    50,     0,    51,    50,    49,     0,    29,
     0,    52,    42,    48,    43,     0,    53,     0,    54,    53,
     0,    54,     0,     0,    12,     0,    13,     0,    14,     0,
    15,     0,    56,    42,    55,    43,     0,    57,     0,     7,
    57,     0,    57,     0,    59,    57,     0,    59,     0,     0,
    58,     0,    61,    58,     0,    61,     0,     0,    41,    42,
    48,    43,     0,     8,    42,    63,    62,    43,     0,    27,
    42,    63,    60,    43,     0,    65,     0,    66,    65,     0,
    64,    66,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
    69,    70,    71,    72,    73,    74,    75,    78,    87,    95,
   103,   113,   114,   116,   117,   122,   123,   124,   126,   127,
   128,   130,   131,   132,   135,   136,   137,   138,   139,   140,
   141,   149,   162,   163,   166,   167,   173,   174,   175,   176,
   179,   188,   189,   199,   200,   203,   204,   208,   209,   213,
   214,   219,   229,   239,   249,   250,   256
};

static const char * const yytname[] = {   "$","error","$undefined.","RFN_LITERAL_INTEGER",
"RFN_LITERAL_FLOAT","RFN_LITERAL_STR","RFN_LITERAL_BOOLEAN","RFN_NOT","RFN_HEADER",
"RFN_OUTPUT_NAME","RFN_FONT_NAME","RFN_FIXED","RFN_PCL","RFN_PSC","RFN_RVW",
"RFN_ESCP","RFN_MAP_NAME","RFN_DVC_FONT_ID","RFN_DVC_CHARSET","RFN_DVC_PITCH",
"RFN_DVC_2Y","RFN_DVC_2X","RFN_DVC_HEIGHT","RFN_SELECTION_HEIGHT","RFN_DVC_SOFT_FONT",
"RFN_DVC_SOFT_FONT_TRANS","RFN_DVC_COMPR","RFN_HEIGHT","RFN_MAP_HEIGHT","RFN_ALL",
"RFN_BOLD_ON","RFN_BOLD_OFF","RFN_BOLD_ALL","RFN_ITALIC_ON","RFN_ITALIC_OFF",
"RFN_ITALIC_ALL","RFN_UNDERLINE_ON","RFN_UNDERLINE_OFF","RFN_UNDERLINE_ALL",
"RFN_DESIGN_HEIGHT","RFN_DESIGN_WIDTH","RFN_FOR_ALL","RFN_OPEN_BLOCK","RFN_CLOSE_BLOCK",
"RFN_OPE_LET","VBLE","let","let_nelist","let_list","SELECTOR_ITEM_BOLD","SELECTOR_ITEM_ITALIC",
"SELECTOR_ITEM_UNDERLINE","MODE_SELECTOR","selector_block","selector_nelist",
"selector_list","DVC_TYPE","DvcBlock","DvcQualifiedBlock","DvcBlock_nelist",
"DvcBlock_list","DvcQualifiedBlock_nelist","DvcQualifiedBlock_list","ForAllBlock",
"Header","HeightBlock","Height_nelist","program",""
};
#endif

static const short yyr1[] = {     0,
    45,    45,    45,    45,    45,    45,    45,    45,    45,    45,
    45,    45,    45,    45,    45,    45,    45,    46,    46,    46,
    46,    47,    47,    48,    48,    49,    49,    49,    50,    50,
    50,    51,    51,    51,    52,    52,    52,    52,    52,    52,
    52,    53,    54,    54,    55,    55,    56,    56,    56,    56,
    57,    58,    58,    59,    59,    60,    60,    61,    61,    62,
    62,    63,    64,    65,    66,    66,    67
};

static const short yyr2[] = {     0,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
     3,     1,     2,     1,     0,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
     1,     4,     1,     2,     1,     0,     1,     1,     1,     1,
     4,     1,     2,     1,     2,     1,     0,     1,     2,     1,
     0,     4,     5,     5,     1,     2,     2
};

static const short yydefact[] = {     0,
     0,     0,     0,     0,    65,    67,     0,    61,     0,    66,
    25,     0,    47,    48,    49,    50,     0,    52,    58,    60,
     0,    57,     1,     2,    10,     3,     4,     5,     6,    11,
    12,    14,    15,    16,    17,    13,     7,     8,     9,     0,
    22,    24,     0,    53,    46,    59,    63,    54,    56,     0,
     0,    23,    62,    41,    26,    27,    28,    29,    30,    31,
    32,    33,    34,     0,     0,     0,     0,    43,    45,     0,
    55,    64,    18,    19,    20,    21,     0,     0,     0,     0,
     0,     0,    25,    44,    51,    35,    36,    37,    38,    39,
    40,     0,    42,     0,     0,     0
};

static const short yydefgoto[] = {    40,
    41,    42,    43,    64,    65,    66,    67,    68,    69,    70,
    17,    18,    19,    49,    50,    20,    21,     8,     2,     5,
     6,    94
};

static const short yypact[] = {     1,
   -16,     9,    -3,    -2,-32768,     9,    17,    51,    -3,-32768,
    -5,    18,-32768,-32768,-32768,-32768,    47,-32768,-32768,    51,
    14,    18,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    46,
-32768,    -5,    48,-32768,    15,-32768,-32768,-32768,    18,    49,
    76,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,    34,    24,    43,    52,-32768,    15,    53,
-32768,-32768,-32768,-32768,-32768,-32768,   -35,    50,   -35,    56,
    50,    56,    -5,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,    54,-32768,    93,    95,-32768
};

static const short yypgoto[] = {-32768,
    57,-32768,    19,   -58,   -39,   -36,-32768,    29,-32768,-32768,
-32768,   -12,    80,-32768,-32768,-32768,-32768,    92,-32768,    97,
-32768,-32768
};


#define	YYLAST		103


static const short yytable[] = {    44,
    61,    62,    63,    23,    24,    25,    79,    81,     1,    48,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    89,    37,    91,    77,     3,    82,    78,    80,    13,
    14,    15,    16,    38,    39,     4,    71,     7,    87,     9,
    86,    90,    88,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    63,    55,    56,    57,    47,    12,    11,    61,
    62,    63,    13,    14,    15,    16,    58,    59,    60,    61,
    62,    63,    55,    56,    57,    58,    59,    60,    73,    74,
    75,    76,    58,    59,    60,    55,    56,    57,    45,    51,
    53,    72,    95,    83,    96,    85,    93,    84,    52,    46,
    22,    92,    10
};

static const short yycheck[] = {    12,
    36,    37,    38,     9,    10,    11,    65,    66,     8,    22,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    80,    28,    82,    64,    42,    66,    64,    65,    12,
    13,    14,    15,    39,    40,    27,    49,    41,    78,    42,
    77,    81,    79,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    30,    31,    32,    43,     7,    42,    36,
    37,    38,    12,    13,    14,    15,    33,    34,    35,    36,
    37,    38,    30,    31,    32,    33,    34,    35,     3,     4,
     5,     6,    33,    34,    35,    30,    31,    32,    42,    44,
    43,    43,     0,    42,     0,    43,    43,    69,    42,    20,
     9,    83,     6
};
/* -*-C-*-  Note some compilers choke on comments on `//#line' lines.  */
//#line 3 "bison.simple"

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


//#line 192 "bison.simple"

#include <malloc.h>


void ClRFN_Parser::yyerror(char *Msg)
{
	printf("Parser error:<%s>",Msg) ;
}


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

//--------------------------------------

#define YYLEX		Lexer->yylex()

//--------------------------------------?? yylex

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

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
void ClRFN_Parser::__yy_memcpy(char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

//#line 192 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

int ClRFN_Parser::yyparse(YYPARSE_PARAM)
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

  int yychar;
  int yynerrs;
  #ifdef YYLSP_NEEDED
  	YYLTYPE yylloc;
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
      int size = yyssp - yyss + 1;

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
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
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
{ PAR->m_oVbleName.StrCpy("OutputName") ; ;
    break;}
case 2:
{ PAR->m_oVbleName.StrCpy("FontName") ; ;
    break;}
case 3:
{ PAR->m_oVbleName.StrCpy("MapName") ; ;
    break;}
case 4:
{ PAR->m_oVbleName.StrCpy("DvcFontId") ; ;
    break;}
case 5:
{ PAR->m_oVbleName.StrCpy("DvcCharSet") ; ;
    break;}
case 6:
{ PAR->m_oVbleName.StrCpy("DvcPitch") ; ;
    break;}
case 7:
{ PAR->m_oVbleName.StrCpy("MapHeight") ; ;
    break;}
case 8:
{ PAR->m_oVbleName.StrCpy("DesignHeight") ; ;
    break;}
case 9:
{ PAR->m_oVbleName.StrCpy("DesignWidth") ; ;
    break;}
case 10:
{ PAR->m_oVbleName.StrCpy("Fixed") ; ;
    break;}
case 11:
{ PAR->m_oVbleName.StrCpy("DvcDY") ; ;
    break;}
case 12:
{ PAR->m_oVbleName.StrCpy("DvcDX") ; ;
    break;}
case 13:
{ PAR->m_oVbleName.StrCpy("DvcCompr") ; ;
    break;}
case 14:
{ PAR->m_oVbleName.StrCpy("DvcHeight") ; ;
    break;}
case 15:
{ PAR->m_oVbleName.StrCpy("SelectionHeight") ; ;
    break;}
case 16:
{ PAR->m_oVbleName.StrCpy("DvcSoftFont") ; ;
    break;}
case 17:
{ PAR->m_oVbleName.StrCpy("DvcSoftFontTrans") ; ;
    break;}
case 18:
{
			//Asignar INTEGER

			if ( ! PAR->PushVbleOnStack(PAR->m_oVbleName,ClRFN_ParsePAR::NUM_VBLE, PAR->m_oLiteralData) )
			{
				YYERROR ;
			} ;
		;
    break;}
case 19:
{
			//Asignar FLOAT
			if ( ! PAR->PushVbleOnStack(PAR->m_oVbleName,ClRFN_ParsePAR::NUM_VBLE, PAR->m_oLiteralData) )
			{
				YYERROR ;
			};
		;
    break;}
case 20:
{
			//Asignar STRING
			if ( ! PAR->PushVbleOnStack(PAR->m_oVbleName,ClRFN_ParsePAR::STR_VBLE, PAR->m_oLiteralData) )
			{
				YYERROR ;
			} ;
		;
    break;}
case 21:
{
			//Asignar BOOLEAN
			if ( ! PAR->PushVbleOnStack(PAR->m_oVbleName,ClRFN_ParsePAR::BOOL_VBLE, PAR->m_oLiteralData) )
			{
				YYERROR ;
			} ;
		;
    break;}
case 26:
{ PAR->m_BoldSelect=ClRFN_ParsePAR::BOLD_ON ; ;
    break;}
case 27:
{ PAR->m_BoldSelect=ClRFN_ParsePAR::BOLD_OFF ; ;
    break;}
case 28:
{ PAR->m_BoldSelect=ClRFN_ParsePAR::BOLD_ALL ; ;
    break;}
case 29:
{ PAR->m_ItalicSelect=ClRFN_ParsePAR::ITALIC_ON ; ;
    break;}
case 30:
{ PAR->m_ItalicSelect=ClRFN_ParsePAR::ITALIC_OFF ; ;
    break;}
case 31:
{ PAR->m_ItalicSelect=ClRFN_ParsePAR::ITALIC_ALL ; ;
    break;}
case 32:
{ PAR->m_UnderlineSelect=ClRFN_ParsePAR::UNDERLINE_ON ; ;
    break;}
case 33:
{ PAR->m_UnderlineSelect=ClRFN_ParsePAR::UNDERLINE_OFF ; ;
    break;}
case 34:
{ PAR->m_UnderlineSelect=ClRFN_ParsePAR::UNDERLINE_ALL ; ;
    break;}
case 41:
{ 
						PAR->m_BoldSelect=ClRFN_ParsePAR::BOLD_ALL ; 
						PAR->m_ItalicSelect=ClRFN_ParsePAR::ITALIC_ALL ;
						PAR->m_UnderlineSelect=ClRFN_ParsePAR::UNDERLINE_ALL ;
					;
    break;}
case 42:
{
						if ( ! PAR->ReduceSelection( PAR->m_BoldSelect, PAR->m_ItalicSelect, PAR->m_UnderlineSelect ) )
						{
							YYERROR ;
						} ;
					;
    break;}
case 47:
{ PAR->m_oBlockType=ClFONT_M_RFNStruct::PCL; ;
    break;}
case 48:
{ PAR->m_oBlockType=ClFONT_M_RFNStruct::PSC; ;
    break;}
case 49:
{ PAR->m_oBlockType=ClFONT_M_RFNStruct::RVW; ;
    break;}
case 50:
{ PAR->m_oBlockType=ClFONT_M_RFNStruct::ESCP; ;
    break;}
case 51:
{
					if ( ! PAR->ReduceForDvcBlock() ) {
						YYERROR ;
					}  ;
				;
    break;}
case 53:
{
							PAR->ClearDvcActivatedFlag() ;
						;
    break;}
case 62:
{
					if ( ! PAR->ReduceForAllBlock() ) {
						YYERROR ;
					}  ;
				;
    break;}
case 63:
{
				if ( ! PAR->ReduceHeaderBlock() ) {
					YYERROR ;
				}  ;
			;
    break;}
case 64:
{
					if ( ! PAR->ReduceHeightBlock() ) {
						YYERROR ;
					}  ;
				;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
//#line 487 "bison.simple"

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

//#include "parser.h"
