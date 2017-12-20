

 //==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                Parser.cpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:            20 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  EXTCONCI.DOC
//
//
// Definition:
//
//    Implementación del parser en un objeto.
////
//==============================================================================
#include "parser.h"

#include "vmres.hpp"

#include "m4win2ux.hpp"

//********************
//función: Constructor
//Retorna: -
//Parametros:-
//Finalidad: Inicializa todos los miembros del Parser
//********************
ClParser::ClParser(yyFlexLexer &ai_scanner, ClTrad *ai_traductor):m_oScanner(ai_scanner){
	//m_oScanner=ai_scanner;
	m_poTraductor=ai_traductor;
}
//*******************
//fin del constructor
//*******************


//**************
//función: yylex
//Retorna: int (tipo de token)
//Parametros:
//Finalidad: Función a la que llama el parser para obtener el siguiente toke
//			 Obtiene el tipo como retorno y en yylval el valor semantico.
//			 En nuestro caso se limita a llamar a yylex del objeto scanner que tiene el parser
//			 y a copiar en yylval del parser el valor de yylval del scanner.
//***************
int ClParser::yylex(void){
	int ret=m_oScanner.yylex();
	yylval=m_oScanner.yylval;
	if (ret==EOF2) 
		return 0;
	return ret;
}
//****************
//fin de yylex
//****************


//**************
//función: Destructor
//Retorna: -
//Parametros:-
//Finalidad: -
//***************
ClParser::~ClParser(){	
}
//**************
//Fin destructor
//**************


//****************************************
//inicializaciones de las tablas estáticas
//****************************************
const char ClParser::yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    39,
    40,     2,     2,    45,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    41,     2,    42,     2,     2,     2,     2,     2,     2,     2,
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
    36,    37,    38,    43,    44
};



#if YYDEBUG != 0

  const short ClParser::yyprhs[] = {     0,
     0,     2,     3,     5,     8,    10,    13,    22,    30,    40,
    49,    56,    65,    73,    83,    90,    98,   102,   109,   115,
   117,   119,   121,   123,   125,   127,   129,   131,   133,   135,
   137,   139,   141,   143,   146,   152,   160,   162,   166,   170,
   173,   175,   177,   179,   183,   185,   187,   191,   195,   197,
   199,   201,   205,   207,   211,   213,   218,   222,   226,   230,
   234,   238,   242,   246,   248,   252,   256,   258,   262,   266,
   268,   271,   275,   277,   279,   281,   283,   285,   287,   289,
   291,   293,   295,   300,   303,   308,   312,   318,   322,   327,
   329,   333,   335,   340,   342,   347,   351,   358,   360,   362,
   366,   368,   370,   371,   373,   375
};



const short ClParser::yyrhs[] = {    47,
     0,     0,    48,     0,     1,    67,     0,    63,     0,    48,
    63,     0,    48,    50,    70,    51,    67,    48,    55,    67,
     0,    50,    70,    51,    67,    48,    55,    67,     0,    48,
    50,    70,    51,    67,    48,    49,    55,    67,     0,    50,
    70,    51,    67,    48,    49,    55,    67,     0,    60,    70,
    67,    48,    61,    67,     0,    57,    66,    58,    68,    67,
    48,    59,    67,     0,    48,    60,    70,    67,    48,    61,
    67,     0,    48,    57,    66,    58,    68,    67,    48,    59,
    67,     0,    56,    67,    48,    62,    70,    67,     0,    48,
    56,    67,    48,    62,    70,    67,     0,    52,    67,    48,
     0,    53,    70,    51,    67,    48,    49,     0,    53,    70,
    51,    67,    48,     0,     9,     0,    10,     0,    11,     0,
    21,     0,    67,     0,    12,     0,    18,     0,    15,     0,
    16,     0,    17,     0,    13,     0,    14,     0,    20,     0,
    67,     0,    64,    67,     0,    50,    70,    51,    64,    54,
     0,    50,    70,    51,    64,    52,    64,    54,     0,    65,
     0,    65,    43,    64,     0,    69,    22,    75,     0,    19,
    75,     0,    82,     0,    84,     0,    81,     0,     3,    22,
    75,     0,     7,     0,    76,     0,    75,    31,    76,     0,
    75,    32,    76,     0,    79,     0,    71,     0,    72,     0,
    71,    24,    72,     0,    73,     0,    72,    23,    73,     0,
    74,     0,    30,    39,    71,    40,     0,    75,    22,    75,
     0,    75,    25,    75,     0,    75,    26,    75,     0,    75,
    27,    75,     0,    75,    28,    75,     0,    75,    29,    75,
     0,    39,    71,    40,     0,    76,     0,    75,    31,    76,
     0,    75,    32,    76,     0,    77,     0,    76,    33,    77,
     0,    76,    34,    77,     0,    78,     0,    32,    78,     0,
    39,    75,    40,     0,     4,     0,     6,     0,     5,     0,
    79,     0,    80,     0,    85,     0,    82,     0,    83,     0,
    84,     0,    81,     0,     3,    38,     3,    90,     0,    85,
    90,     0,    85,    90,    37,     3,     0,    36,    85,    90,
     0,    36,    85,    90,    37,     3,     0,    85,    37,     3,
     0,    85,    37,     3,    90,     0,    86,     0,    87,    35,
    86,     0,     3,     0,     3,    41,    88,    42,     0,     3,
     0,     3,    41,    88,    42,     0,     3,    38,     3,     0,
     3,    38,     3,    41,    88,    42,     0,    75,     0,    75,
     0,    91,    93,    92,     0,    39,     0,    40,     0,     0,
    94,     0,    89,     0,    94,    45,    89,     0
};


#endif

#if YYDEBUG != 0
const short ClParser:: yyrline[] = { 0,
    53,    56,    57,    61,    62,    63,    64,    65,    66,    67,
    68,    69,    70,    71,    72,    73,    76,    77,    78,    81,
    84,    87,    90,    93,    96,    99,   102,   105,   108,   111,
   114,   117,   120,   121,   122,   123,   126,   127,   130,   131,
   133,   134,   135,   138,   141,   143,   144,   145,   148,   152,
   154,   155,   158,   159,   162,   163,   166,   167,   168,   169,
   170,   171,   172,   177,   178,   179,   182,   183,   184,   187,
   188,   191,   192,   193,   195,   197,   198,   201,   204,   205,
   206,   207,   210,   213,   214,   215,   216,   219,   222,   226,
   227,   230,   231,   234,   235,   236,   237,   240,   251,   254,
   256,   259,   262,   263,   266,   267
};


const char * const ClParser::yytname[] = {   "$","error","$undefined.","IDENTIFICADOR",
"NUMERO","FECHA","LITERAL","NL","EOF2","IF","THEN","ELSE","ENDIF","WHILE","WEND",
"FOR","TO","NEXT","DO","RETURN","UNTIL","ELSEIF","IGUAL","AND","OR","MAYIGUAL",
"MENIGUAL","MAYOR","MENOR","DISTINTO","NOT","MAS","MENOS","MULTI","DIVIDE","ACCMIEMBRO",
"ACCPADRE","ACCATRIB","ACCCHANNEL","'('","')'","'['","']'","SEPINST","MENOSUNARIO",
"','","programa","listasentencias","instnlin","elsebody","if","then","else",
"elseif","endifs","endifm","do","for","to","next","while","wend","until","inst1linTE",
"inst1lin","instsimple","asigfor","newline","finfor","target","cond","expcond",
"termC","factorC","primaryC","exp","term","factor","primary","itemvalor","itemrvalue",
"channelfunction","itemmetodo","atributo","atributometodo","celda","itemname",
"nodename","tramo_row","arg","lista_arg","init_lista_arg","end_lista_arg","arglist_rec",
"largs",""
};
#endif

	const short ClParser::yyr1[] = {     0,
    46,    47,    47,    48,    48,    48,    48,    48,    48,    48,
    48,    48,    48,    48,    48,    48,    49,    49,    49,    50,
    51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    63,    63,    63,    63,    64,    64,    65,    65,
    65,    65,    65,    66,    67,    68,    68,    68,    69,    70,
    71,    71,    72,    72,    73,    73,    74,    74,    74,    74,
    74,    74,    74,    75,    75,    75,    76,    76,    76,    77,
    77,    78,    78,    78,    78,    78,    78,    79,    80,    80,
    80,    80,    81,    82,    82,    82,    82,    83,    84,    85,
    85,    86,    86,    87,    87,    87,    87,    88,    89,    90,
    91,    92,    93,    93,    94,    94
};

const short ClParser::yyr2[] = {     0,
     1,     0,     1,     2,     1,     2,     8,     7,     9,     8,
     6,     8,     7,     9,     6,     7,     3,     6,     5,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     2,     5,     7,     1,     3,     3,     2,
     1,     1,     1,     3,     1,     1,     3,     3,     1,     1,
     1,     3,     1,     3,     1,     4,     3,     3,     3,     3,
     3,     3,     3,     1,     3,     3,     1,     3,     3,     1,
     2,     3,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     4,     2,     4,     3,     5,     3,     4,     1,
     3,     1,     4,     1,     4,     3,     6,     1,     1,     3,
     1,     1,     0,     1,     1,     3
};


const short ClParser::yydefact[] = {     0,
     0,    92,    45,    20,    30,    27,    26,     0,     0,     1,
     3,     0,     0,     0,     0,     5,     0,    37,    33,     0,
    49,    43,    41,    42,    78,    90,     0,     4,     0,     0,
    73,    75,    74,     0,     0,    40,    64,    67,    70,    76,
    77,    82,    79,    80,    81,    78,    92,     0,     0,     0,
     0,     0,     6,     0,     0,     0,    50,    51,    53,    55,
     0,     0,     0,     0,     0,    34,     0,     0,     0,   101,
    84,   103,     0,    96,    98,     0,    71,     0,     0,     0,
     0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
     0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    28,     0,     0,    38,    39,     0,
     0,    99,   105,     0,   104,    92,    91,     0,    83,    93,
    72,    65,    66,    68,    69,    88,    96,     0,     0,     0,
     0,     0,     0,    63,     0,     0,    52,    54,    57,    58,
    59,    60,    61,    62,    32,     0,    44,     0,     0,    64,
     0,    89,    85,   102,   100,     0,     0,     0,    87,     0,
     0,     0,     0,    56,    22,     0,    35,    24,     0,     0,
     0,     0,     0,    31,     0,   106,     0,    97,     0,     0,
     0,     0,     0,    25,    23,     0,     0,     0,     0,    15,
     0,    65,    66,    11,    93,     0,     0,    16,     0,    13,
    36,     0,     0,     0,     8,    29,     0,     0,     7,     0,
    10,    17,     0,    12,     9,    14,     0,    19,    18,     0,
     0,     0
};

const short ClParser::yydefgoto[] = {   220,
    10,    11,   186,    12,    94,   187,   188,   167,   189,    13,
    14,   106,   207,    15,   175,   146,    16,    17,    18,    64,
    19,   148,    20,    56,    57,    58,    59,    60,    61,    37,
    38,    39,    40,    41,    42,    43,    44,    45,    46,    26,
    27,    76,   113,    71,    72,   155,   114,   115
};

const short ClParser::yypact[] = {   247,
     8,    87,-32768,-32768,-32768,-32768,-32768,    33,    15,-32768,
   597,    18,     8,    24,    18,-32768,     8,     1,-32768,    25,
-32768,-32768,-32768,-32768,    16,-32768,    36,-32768,    65,    33,
-32768,-32768,-32768,    70,    33,    82,    84,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,    41,   100,    46,    18,     8,
    24,    18,-32768,    49,    18,    85,    37,    79,-32768,-32768,
   609,   527,    88,   107,     8,-32768,    22,    33,   124,-32768,
   105,    33,   134,    92,    82,   103,-32768,    51,    33,    33,
    33,    33,   147,   149,   119,    85,   527,   107,     8,    18,
    -5,   592,-32768,     7,    18,    18,    33,    33,    33,    33,
    33,    33,   473,    33,-32768,    33,   527,-32768,    82,    46,
   154,    82,-32768,   125,   114,   126,-32768,    33,-32768,   129,
-32768,    84,    84,-32768,-32768,    46,   131,   170,     7,   473,
    33,   527,    75,-32768,    89,   527,    79,-32768,    82,    82,
    82,    82,    82,    82,-32768,    18,    82,     8,   123,    26,
   345,-32768,-32768,-32768,-32768,    33,    33,   132,-32768,   527,
    18,     8,   345,-32768,-32768,    22,-32768,-32768,   546,     8,
   527,    33,    33,-32768,     8,-32768,   136,-32768,   546,     8,
   527,     8,     8,-32768,-32768,   164,     8,    18,     8,-32768,
   590,    56,    59,-32768,-32768,   164,     8,-32768,   590,-32768,
-32768,     8,   527,    85,-32768,-32768,     8,     8,-32768,     8,
-32768,   597,     8,-32768,-32768,-32768,   527,   577,-32768,   184,
   187,-32768
};


const short ClParser::yypgoto[] = {-32768,
-32768,   -55,  -178,    31,   -83,    54,-32768,     9,  -166,   101,
   133,   102,    -6,   305,    34,    71,   336,   -65,-32768,   155,
   240,    74,-32768,    -3,   -39,   112,   113,-32768,   471,   -75,
    58,   179,    17,-32768,   108,   214,-32768,   304,     0,   146,
-32768,  -112,    67,   -40,-32768,-32768,-32768,-32768
};



#define	YYLAST		641


const short ClParser::yytable[] = {    25,
   196,   108,   129,   122,   123,   158,   103,    85,    48,     2,
    25,    65,   197,     3,     3,    91,    21,    47,    95,   202,
     2,    31,    32,    33,     2,     8,    63,    21,   135,   208,
   150,   130,   -46,   119,   134,     2,    31,    32,    33,   219,
     8,    49,     9,    67,   177,    86,    68,    54,    89,    34,
   133,   151,    69,     9,    70,   150,    55,     9,    81,    82,
    95,    25,   -47,   135,    34,   -48,    25,    74,     9,   152,
    73,    35,     2,    31,    32,    33,   163,    83,    21,    70,
   169,    79,    80,    21,    70,   152,    25,    90,    81,    82,
   121,    81,    82,    25,    93,     3,   192,   193,    95,   165,
   183,    96,    25,    21,   179,     9,    25,    22,    35,   104,
    21,    50,    79,    80,   164,   191,    81,    82,    22,    21,
   213,   -94,   105,    21,    29,   199,   110,    30,    25,    25,
    70,    25,   118,    49,   -94,    25,   116,    84,   124,   125,
    30,   111,   170,    51,   120,    21,    21,   212,    21,   126,
    25,   127,    21,   172,   173,   128,   153,   180,   156,    25,
    49,   218,    25,   -95,   154,    25,   157,    21,    25,    22,
    25,   118,   159,   178,    22,   184,    21,   195,    25,    21,
    25,    49,    21,   221,   204,    21,   222,    21,   166,   131,
    25,   201,   210,    49,    22,    21,   182,    21,    25,    49,
   161,    22,    25,    50,   162,    88,   137,    21,   138,    49,
    22,    25,    77,    23,    22,    21,    25,    25,   117,    21,
     0,    49,   176,     0,    23,     0,     0,     0,    21,    49,
    50,     0,     0,    21,    21,    51,    22,    22,     0,    22,
    28,     0,    49,    22,     0,     0,    -2,     1,    49,     2,
     0,    50,    62,     3,     0,     4,    66,     0,    22,     5,
     0,     6,    51,    50,     7,     8,     0,    22,     0,    50,
    22,     0,     0,    22,     0,    23,    22,     0,    22,    50,
    23,     0,     9,    51,     0,     0,    22,     0,    22,    87,
     0,    50,     0,     0,     0,    51,     0,     0,    22,    50,
    23,    51,     0,    24,   107,     0,    22,    23,     0,     0,
    22,    51,    50,     0,    24,    52,    23,     0,    50,    22,
    23,     0,     0,    51,    22,    22,     0,     0,   132,     0,
     0,    51,     0,   136,     0,     0,     0,     0,     0,     0,
     0,     0,    23,    23,    51,    23,    53,     2,     0,    23,
    51,     3,     0,     4,     0,     0,     0,     5,   174,     6,
     0,     0,     7,     8,    23,    24,     0,     0,   160,     0,
    24,     0,     0,    23,   168,     0,    23,     0,     0,    23,
     9,     0,    23,     0,    23,     0,     0,   171,     0,     0,
    24,     0,    23,     0,    23,     0,     0,    24,     0,     0,
     0,   181,     0,     0,    23,     0,    24,    52,     0,   190,
    24,     0,    23,     0,   194,     0,    23,     0,     0,   198,
     0,   200,   168,     0,     0,    23,   203,     0,   205,     0,
    23,    23,    24,    24,    52,    24,   209,     0,    53,    24,
     0,   211,     0,     0,     0,     0,   214,   215,     0,   216,
     0,     0,   217,     0,    24,    52,     0,     0,     0,     0,
     0,     0,     0,    24,     0,    53,    24,    52,     0,    24,
     0,     0,    24,    52,    24,     2,     0,     0,    36,     3,
     0,     4,    24,    52,    24,     5,    53,     6,     0,     0,
     7,     8,   145,     0,    24,    52,     0,     0,    53,     0,
    75,     0,    24,    52,    53,    78,    24,     0,     9,     0,
     0,     0,     0,     0,    53,    24,    52,     0,     0,     0,
    24,    24,    52,     0,     0,    92,    53,     1,     0,     2,
     0,     0,     0,     3,    53,     4,     0,     0,   109,     5,
     0,     6,   112,     0,     7,     8,     0,    53,     2,     0,
     0,     0,     3,    53,     4,     0,   165,   184,     5,     0,
     6,     0,     9,     7,     8,     0,   185,   139,   140,   141,
   142,   143,   144,     0,   147,     0,   149,     0,     0,     2,
     0,     9,     0,     3,     0,     4,     0,   165,    75,     5,
     0,     6,     2,     0,     7,     8,     3,   185,     4,     2,
     0,   149,     5,     3,     6,     4,   206,     7,     8,     5,
     0,     6,     9,    97,     7,     8,    98,    99,   100,   101,
   102,     0,    79,    80,     0,     9,   112,    75,     0,     0,
    97,   121,     9,    98,    99,   100,   101,   102,     0,    79,
    80
};

const short ClParser::yycheck[] = {     0,
   179,    67,    86,    79,    80,   118,    62,    48,     9,     3,
    11,    15,   179,     7,     7,    55,     0,     3,    24,   186,
     3,     4,     5,     6,     3,    19,     3,    11,    94,   196,
   106,    87,     7,    74,    40,     3,     4,     5,     6,   218,
    19,    11,    36,    43,   157,    49,    22,    30,    52,    32,
    90,   107,    37,    36,    39,   131,    39,    36,    33,    34,
    24,    62,     7,   129,    32,     7,    67,     3,    36,   110,
    35,    39,     3,     4,     5,     6,   132,    37,    62,    39,
   136,    31,    32,    67,    39,   126,    87,    39,    33,    34,
    40,    33,    34,    94,    10,     7,   172,   173,    24,    11,
   166,    23,   103,    87,   160,    36,   107,     0,    39,    22,
    94,    11,    31,    32,    40,   171,    33,    34,    11,   103,
   204,    35,    16,   107,    38,   181,     3,    41,   129,   130,
    39,   132,    41,   103,    35,   136,     3,    38,    81,    82,
    41,    37,   146,    11,    42,   129,   130,   203,   132,     3,
   151,     3,   136,    31,    32,    37,     3,   161,    45,   160,
   130,   217,   163,    35,    40,   166,    41,   151,   169,    62,
   171,    41,     3,    42,    67,    12,   160,    42,   179,   163,
   181,   151,   166,     0,   188,   169,     0,   171,   135,    88,
   191,   183,   199,   163,    87,   179,   163,   181,   199,   169,
   130,    94,   203,   103,   131,    51,    95,   191,    96,   179,
   103,   212,    34,     0,   107,   199,   217,   218,    73,   203,
    -1,   191,   156,    -1,    11,    -1,    -1,    -1,   212,   199,
   130,    -1,    -1,   217,   218,   103,   129,   130,    -1,   132,
     1,    -1,   212,   136,    -1,    -1,     0,     1,   218,     3,
    -1,   151,    13,     7,    -1,     9,    17,    -1,   151,    13,
    -1,    15,   130,   163,    18,    19,    -1,   160,    -1,   169,
   163,    -1,    -1,   166,    -1,    62,   169,    -1,   171,   179,
    67,    -1,    36,   151,    -1,    -1,   179,    -1,   181,    50,
    -1,   191,    -1,    -1,    -1,   163,    -1,    -1,   191,   199,
    87,   169,    -1,     0,    65,    -1,   199,    94,    -1,    -1,
   203,   179,   212,    -1,    11,    11,   103,    -1,   218,   212,
   107,    -1,    -1,   191,   217,   218,    -1,    -1,    89,    -1,
    -1,   199,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   129,   130,   212,   132,    11,     3,    -1,   136,
   218,     7,    -1,     9,    -1,    -1,    -1,    13,    14,    15,
    -1,    -1,    18,    19,   151,    62,    -1,    -1,   129,    -1,
    67,    -1,    -1,   160,   135,    -1,   163,    -1,    -1,   166,
    36,    -1,   169,    -1,   171,    -1,    -1,   148,    -1,    -1,
    87,    -1,   179,    -1,   181,    -1,    -1,    94,    -1,    -1,
    -1,   162,    -1,    -1,   191,    -1,   103,   103,    -1,   170,
   107,    -1,   199,    -1,   175,    -1,   203,    -1,    -1,   180,
    -1,   182,   183,    -1,    -1,   212,   187,    -1,   189,    -1,
   217,   218,   129,   130,   130,   132,   197,    -1,   103,   136,
    -1,   202,    -1,    -1,    -1,    -1,   207,   208,    -1,   210,
    -1,    -1,   213,    -1,   151,   151,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   160,    -1,   130,   163,   163,    -1,   166,
    -1,    -1,   169,   169,   171,     3,    -1,    -1,     8,     7,
    -1,     9,   179,   179,   181,    13,   151,    15,    -1,    -1,
    18,    19,    20,    -1,   191,   191,    -1,    -1,   163,    -1,
    30,    -1,   199,   199,   169,    35,   203,    -1,    36,    -1,
    -1,    -1,    -1,    -1,   179,   212,   212,    -1,    -1,    -1,
   217,   218,   218,    -1,    -1,    55,   191,     1,    -1,     3,
    -1,    -1,    -1,     7,   199,     9,    -1,    -1,    68,    13,
    -1,    15,    72,    -1,    18,    19,    -1,   212,     3,    -1,
    -1,    -1,     7,   218,     9,    -1,    11,    12,    13,    -1,
    15,    -1,    36,    18,    19,    -1,    21,    97,    98,    99,
   100,   101,   102,    -1,   104,    -1,   106,    -1,    -1,     3,
    -1,    36,    -1,     7,    -1,     9,    -1,    11,   118,    13,
    -1,    15,     3,    -1,    18,    19,     7,    21,     9,     3,
    -1,   131,    13,     7,    15,     9,    17,    18,    19,    13,
    -1,    15,    36,    22,    18,    19,    25,    26,    27,    28,
    29,    -1,    31,    32,    -1,    36,   156,   157,    -1,    -1,
    22,    40,    36,    25,    26,    27,    28,    29,    -1,    31,
    32
};
//***************************
//fin de las inicializaciones
//***************************


//**************
//función: Parse
//Retorna: m4return_t
//Parametros:-
//Finalidad: Parsea LN4. Usa el scanner usado para crear la clase Parser.
//***************
m4return_t ClParser::Parse(){
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  /* Bug 0151800
  Se elimina el uso de alloca porque desborda el stack
  */
  short *yyss = (short *) malloc(YYINITDEPTH * sizeof(*yyss));		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = (YYSTYPE *) malloc(YYINITDEPTH * sizeof(*yyvs));	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE *yyls = (YYLTYPE *) malloc(YYINITDEPTH * sizeof(*yyls));
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
	  YYRETURN(2)
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

	  /* Bug 0151800
	  Se elimina el uso de alloca porque desborda el stack
	  */
      yyss = (short *) malloc (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
	  free(yyss1);
      yyvs = (YYSTYPE *) malloc (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
	  free(yyvs1);
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) malloc (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
	  free(yyls1);
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

case 4:
//#line 61 "gram.y"
{yyerrok;;
    break;}
case 20:
//#line 81 "gram.y"
{If();;
    break;}
case 21:
//#line 84 "gram.y"
{Then();;
    break;}
case 22:
//#line 87 "gram.y"
{Else();;
    break;}
case 23:
//#line 90 "gram.y"
{Elseif();;
    break;}
case 24:
//#line 93 "gram.y"
{Endif();;
    break;}
case 25:
//#line 96 "gram.y"
{Endif();;
    break;}
case 26:
//#line 99 "gram.y"
{Do();;
    break;}
case 27:
//#line 102 "gram.y"
{For();;
    break;}
case 28:
//#line 105 "gram.y"
{To();;
    break;}
case 29:
//#line 108 "gram.y"
{Next();;
    break;}
case 30:
//#line 111 "gram.y"
{While();;
    break;}
case 31:
//#line 114 "gram.y"
{Wend();;
    break;}
case 32:
//#line 117 "gram.y"
{Until();;
    break;}
case 39:
//#line 130 "gram.y"
{Assign();;
    break;}
case 40:
//#line 131 "gram.y"
{Return();;
    break;}
case 41:
//#line 133 "gram.y"
{Purge();;
    break;}
case 42:
//#line 134 "gram.y"
{Purge();;
    break;}
case 43:
//#line 135 "gram.y"
{Purge();;
    break;}
case 44:
//#line 138 "gram.y"
{StoreFor(yyvsp[-2].str);PopIdentifier();;
    break;}
case 45:
//#line 141 "gram.y"
{WriteLine();;
    break;}
case 46:
//#line 143 "gram.y"
{FinFor();;
    break;}
case 47:
//#line 144 "gram.y"
{AritmOpAdd();FinFor();;
    break;}
case 48:
//#line 145 "gram.y"
{AritmOpSub();FinFor();;
    break;}
case 49:
//#line 149 "gram.y"
{strcpy(yyval.str,yyvsp[0].str);Target(yyvsp[0].str);;
    break;}
case 50:
//#line 152 "gram.y"
{Condition();;
    break;}
case 52:
//#line 155 "gram.y"
{LogOpOr();;
    break;}
case 54:
//#line 159 "gram.y"
{LogOpAnd();;
    break;}
case 56:
//#line 163 "gram.y"
{LogOpNot();;
    break;}
case 57:
//#line 166 "gram.y"
{LogOpEqual();;
    break;}
case 58:
//#line 167 "gram.y"
{LogOpGreaterEqual();;
    break;}
case 59:
//#line 168 "gram.y"
{LogOpLessEqual();;
    break;}
case 60:
//#line 169 "gram.y"
{LogOpGreater();;
    break;}
case 61:
//#line 170 "gram.y"
{LogOpLess();;
    break;}
case 62:
//#line 171 "gram.y"
{LogOpNotEqual();;
    break;}
case 65:
//#line 178 "gram.y"
{AritmOpAdd();;
    break;}
case 66:
//#line 179 "gram.y"
{AritmOpSub();;
    break;}
case 68:
//#line 183 "gram.y"
{AritmOpMul();;
    break;}
case 69:
//#line 184 "gram.y"
{AritmOpDiv();;
    break;}
case 71:
//#line 188 "gram.y"
{AritmOpSigno();;
    break;}
case 73:
//#line 192 "gram.y"
{StoreNumber(yyvsp[0].str);;
    break;}
case 74:
//#line 193 "gram.y"
{ //StoreString(yyvsp[0].pstr);;
	if (yyvsp[0].hString != 0) {
		StoreString( (m_oScanner.GetPool())->GetString(yyvsp[0].hString) );
		//Libero memoria reservada en lexyy.cpp: 
		// en yyFlexLexer::yylex() -> case 36!!!
		(m_oScanner.GetPool())->DeleteString(yyvsp[0].hString);	//Borramos la cadena dinamica del LITERAL
	}
    break;}
case 75:
//#line 195 "gram.y"
{StoreDate(yyvsp[0].str);;
    break;}
case 76:
//#line 197 "gram.y"
{ItemValorRead(yyvsp[0].str);;
    break;}
case 78:
//#line 201 "gram.y"
{strcpy(yyval.str,yyvsp[0].str);;
    break;}
case 83:
//#line 210 "gram.y"
{ChannelFunction( yyvsp[-3].str , yyvsp[-1].str , yyvsp[0].str );PopIdentifier();PopIdentifier();;
    break;}
case 84:
//#line 213 "gram.y"
{Metodo(yyvsp[-1].str,yyvsp[0].str);;
    break;}
case 85:
//#line 214 "gram.y"
{MetodoRegla(yyvsp[-3].str,yyvsp[0].str,yyvsp[-2].str);PopIdentifier();;
    break;}
case 86:
//#line 215 "gram.y"
{MetodoPadre(yyvsp[-1].str,yyvsp[0].str);;
    break;}
case 87:
//#line 216 "gram.y"
{MetodoReglaPadre(yyvsp[-3].str,yyvsp[0].str,yyvsp[-2].str);PopIdentifier();;
    break;}
case 88:
//#line 219 "gram.y"
{Atributo(yyvsp[-2].str,yyvsp[0].str);PopIdentifier();;
    break;}
case 89:
//#line 222 "gram.y"
{AtributoMetodo(yyvsp[-3].str,yyvsp[-1].str,yyvsp[0].str);PopIdentifier();;
    break;}
case 90:
//#line 226 "gram.y"
{ strcpy(yyval.str,yyvsp[0].str); ;
    break;}
case 91:
//#line 227 "gram.y"
{ sprintf (yyval.str, "%s.%s", yyvsp[-2].str , yyvsp[0].str); ;
    break;}
case 92:
//#line 230 "gram.y"
{ strncpy(yyval.str, yyvsp[0].str,MAX_TOKEN-1); /*MAX_TOKEN es la máx long de un identificador->variable*/
  yyval.str[MAX_TOKEN-1]=0; /*nulterminamos por si acaso*/
  NotifyPosibleItem();

    break;}
case 93:
//#line 231 "gram.y"
{ sprintf (yyval.str, "%.30s[]", yyvsp[-3].str); ;/*30 es la máx long de item_id*/
  NotifyPosibleItem();
	break;}
case 94:
//#line 234 "gram.y"
{ strncpy(yyval.str,yyvsp[0].str,30); /*30=long máxima*/
	yyval.str[30]=0;/*nulterminamos*/
    NotifyPosibleTI();
	break;}
case 95:
//#line 235 "gram.y"
{ sprintf (yyval.str, "%.30s<>", yyvsp[-3].str); /*30=long maxima*/;
    NotifyPosibleTI();
	break;}
case 96:
//#line 236 "gram.y"
{sprintf (yyval.str, "%.30s!%.30s", yyvsp[-2].str , yyvsp[0].str); ;/*30=long maxima*/
    PopIdentifier();
	PopIdentifier();// En este caso es nombre de nodo, no cambiamos la TI
	break;}
case 97:
//#line 237 "gram.y"
{ sprintf (yyval.str, "%.30s!%.30s<>", yyvsp[-5].str , yyvsp[-3].str); ;/*30=long maxima*/
    PopIdentifier();
	PopIdentifier();// En este caso es nombre de nodo, no cambiamos la TI
	break;}
case 99:
//#line 251 "gram.y"
{Arg();;
    break;}
case 100:
//#line 254 "gram.y"
{strcpy (yyval.str, yyvsp[0].str); ;
    break;}
case 101:
//#line 256 "gram.y"
{InitArgList();;
    break;}
case 102:
//#line 259 "gram.y"
{strcpy ( yyval.str , itoa(EndArgList(), yyval.str, 10)  ) ;;
    break;}
}


     /* the action file gets copied in in place of this dollarsign */
//line 487 "bison.simple"

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
}
//**********************
//Fin del método Parse
//**********************


//**************
//función: yyerror
//Retorna: -
//Parametros:char *
//Finalidad: LLamada por el scanner en caso de error gramatical
//***************


void ClParser::yyerror(char *ai_str, m4uint32_t ai_iNotSyntaxError/*=0*/){

	/*
	Bug 0097958
	El mensaje de error hay que sacarlo siempre
	*/
	if (ai_iNotSyntaxError){
		DUMP_CHLOG_ERRORF( ai_iNotSyntaxError,  m_poTraductor->GetNumLine()+1<<
												m_poTraductor->GetCurrentRule()<<
												m_poTraductor->GetCurrentItem()<<
												m_poTraductor->GetCurrentTi()<<
												m_poTraductor->GetCurrentChannel()<<
												M4LOGSYS_EXT_ERROR_CODE_LINE << m_poTraductor->GetNumLine()+1 );
	}
	else{
		DUMP_CHLOG_ERRORF( M4_CH_VM_SYNTAX_ERROR,  m_poTraductor->GetNumLine()+1<<
												m_poTraductor->GetCurrentRule()<<
												m_poTraductor->GetCurrentItem()<<
												m_poTraductor->GetCurrentTi()<<
												m_poTraductor->GetCurrentChannel()<<
												M4LOGSYS_EXT_ERROR_CODE_LINE << m_poTraductor->GetNumLine()+1 );	// (5): Syntax error in LN4 rule
	}

	yychar=0; //lo ponemos a capon a eof para que si hay un error de sintaxis no intente seguir compilado y pare.
}
//**********************
//Fin del método yyerror
//**********************




