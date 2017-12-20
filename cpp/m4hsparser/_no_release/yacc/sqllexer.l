%{

/****************************************************************************
lexer.l
****************************************************************************/
#include "selectinfo.hpp"
#include "sqlpartk.h"

#include <string.h>

%}

/* macros */

EXPONENT	([Ee][+-]?[0-9]+)                                                                                          
IDENTIFIER_M  ([a-zA-Z][a-zA-Z0-9_$#]*)	

/* para case-insensitive */

A 	[Aa]                                                 
B 	[Bb]                                                 
C 	[Cc]                                                 
D 	[Dd]                                                 
E 	[Ee]                                                 
F 	[Ff]                                                 
G 	[Gg]                                                 
H 	[Hh]                                                 
I 	[Ii]                                                 
J 	[Jj]                                                 
K 	[Kk]                                                 
L 	[Ll]                                                 
M 	[Mm]                                                 
N 	[Nn]                                                 
O 	[Oo]                                                 
P 	[Pp]                                                 
Q 	[Qq]                                                 
R 	[Rr]                                                 
S 	[Ss]                                                 
T 	[Tt]                                                 
U 	[Uu]                                                 
V 	[Vv]                                                 
W 	[Ww]                                                 
X 	[Xx]                                                 
Y 	[Yy]                                                 
Z	[Zz]    

/* White space entre keywords y operadores */
WS  [ \t]*                                           

%%       

[0-9]+ { 

		// INTEGER 
		//
		// NO se pone [+-]?, porque operaciones a+2 devolveria un a,+2, en vez de a,+,2

		strcpy (yylval.pcValue, yytext);
		return LITERAL_INTEGER ; 
	}


[0-9]*"."[0-9]+{EXPONENT}?	|
[0-9]+"."[0-9]*{EXPONENT}?	|
[0-9]+EXPONENT {

		// 	FLOAT 
		//
	 	//-Si aparece ".":
 		//	-Debe aparecer la parte entera O la decimal (no pueden omitirse ambas).
	 	//	-El exponente SIEMPRE es opcional.    
 		//-Si NO aparece ".":                                   
		//	-Debe aparecer alguna parte entera.
		//	-Debe aparecer el exponente (si no, es un entero).
		//
		// NO se pone [+-]?, porque operaciones a+2 devolveria un a,+2, en vez de a,+,2

		strcpy (yylval.pcValue, yytext);
		return LITERAL_FLOAT ; 
	}


"'"[^"'"\n]*"'" { 

		//	LITERAL_STR
		//
		// Cualquier ocurrencia 'cadena' es un literal tipo texto. 
		//

		strcpy (yylval.pcValue, yytext);
		return LITERAL_STR ; 
	}

"{"{WS}d{WS}"'"[0-9]{4,4}"-"[0-9]{2,2}"-"[0-9]{2,2}"'"{WS}"}" {

		strcpy (yylval.pcValue, yytext);
		return LITERAL_DATE ; 
	}


"{"{WS}ts{WS}"'"[0-9]{4,4}"-"[0-9]{2,2}"-"[0-9]{2,2}" "[0-9]{2,2}":"[0-9]{2,2}":"[0-9]{2,2}"'"{WS}"}" {

		//	LITERAL_TIME_STAMP
		//
		//	Cualquier ocurrencia { ts 'yyyy-mm-aa hh:mm:ss' }  es un literal tipo date. 
		//
		
		strcpy (yylval.pcValue, yytext);
		return LITERAL_TIME_STAMP ; 
	}

{S}{E}{L}{E}{C}{T} { 

		//	KEYWORDS
		//
		//	Definirlas ANTES de  IDENTIFIER_M, o pueden interpretarse como IDENTIDIER

		strcpy (yylval.pcValue, yytext);
		return SELECT ; 
	} 

{D}{I}{S}{T}{I}{N}{C}{T} 					{	strcpy (yylval.pcValue, yytext);
												return DISTINCT ; } 
{F}{R}{O}{M}						 		{	strcpy (yylval.pcValue, yytext);
												return FROM; } 
{G}{R}{O}{U}{P}{WS}{B}{Y}					{	strcpy (yylval.pcValue, yytext);
												return GROUP_BY ; } 
{W}{H}{E}{R}{E}								{ 	strcpy (yylval.pcValue, yytext);
												return WHERE ; }
{U}{N}{I}{O}{N}								{ 	strcpy (yylval.pcValue, yytext);
												return UNION ; }
{U}{N}{I}{O}{N}{WS}{A}{L}{L}				{ 	strcpy (yylval.pcValue, yytext);
												return UNION_ALL ; }
{I}{N}{T}{E}{R}{S}{E}{C}{T}					{ 	strcpy (yylval.pcValue, yytext);
												return INTERSECT ; }
{I}{N}										{ 	strcpy (yylval.pcValue, yytext);
												return IN_KW ; }
{A}{L}{L}									{ 	strcpy (yylval.pcValue, yytext);
												return ALL ; }
{B}{E}{T}{W}{E}{E}{N}        	           	{ 	strcpy (yylval.pcValue, yytext);
												return OPE_BETWEEN ; }
{N}{O}{T}{WS}{B}{E}{T}{W}{E}{E}{N}	      	{ 	strcpy (yylval.pcValue, yytext);
												return OPE_NOT_BETWEEN ; }
{E}{X}{I}{S}{T}{S}							{ 	strcpy (yylval.pcValue, yytext);
												return EXISTS ; }
{L}{I}{K}{E}								{ 	strcpy (yylval.pcValue, yytext);
												return OPE_LIKE ; }
{N}{O}{T}{WS}{L}{I}{K}{E}					{ 	strcpy (yylval.pcValue, yytext);
												return OPE_NOT_LIKE ; }
{I}{S}{WS}{N}{U}{L}{L}						{ 	strcpy (yylval.pcValue, yytext);
												return OPE_IS_NULL ; }									
{I}{S}{WS}{N}{O}{T}{WS}{N}{U}{L}{L}			{ 	strcpy (yylval.pcValue, yytext);
												return OPE_IS_NOT_NULL ; }									
{H}{A}{V}{I}{N}{G}							{ 	strcpy (yylval.pcValue, yytext);
												return HAVING ; }
{O}{R}{D}{E}{R}{WS}{B}{Y}					{ 	strcpy (yylval.pcValue, yytext);
												return ORDER_BY ; }
{A}{S}{C}									{ 	strcpy (yylval.pcValue, yytext);
												return ASC ; }
{D}{E}{S}{C}								{ 	strcpy (yylval.pcValue, yytext);
												return DESC ; }


"("			{ 
				// OPERATORS 
				strcpy (yylval.pcValue, yytext);
				return OPEN_PAREN ; 
			}
")"					{ 	strcpy (yylval.pcValue, yytext);
						return CLOSE_PAREN ; }
"("{WS}"+"{WS}")"   {  	strcpy (yylval.pcValue, yytext);
						return OPE_OUTER_JOIN; }	
","					{  	strcpy (yylval.pcValue, yytext);
						return OPE_COMMA ; }
"."					{  	strcpy (yylval.pcValue, yytext);
						return OPE_POINT ; }
"@"					{  	strcpy (yylval.pcValue, yytext);
						return OPE_AT ; }
"&"					{  	strcpy (yylval.pcValue, yytext);
						return OPE_AMP ; }
"#"					{  	strcpy (yylval.pcValue, yytext);
						return OPE_ALM ; }
"$"					{  	strcpy (yylval.pcValue, yytext);
						return OPE_DOL ; }



{N}{O}{T}			{  	strcpy (yylval.pcValue, yytext);
						return OPE_NOT ; }
{A}{N}{D}			{  	strcpy (yylval.pcValue, yytext);
						return OPE_AND ; }
{O}{R}				{  	strcpy (yylval.pcValue, yytext);
						return OPE_OR ; }

"+"					{  	strcpy (yylval.pcValue, yytext);
						return OPE_ADD ; }		
"-"					{  	strcpy (yylval.pcValue, yytext);
						return OPE_SUBS ; }       
"*"					{  	strcpy (yylval.pcValue, yytext);
						return OPE_MULT ; }
"/"					{  	strcpy (yylval.pcValue, yytext);
						return OPE_DIV ; }
"|"{WS}"|"			{  	strcpy (yylval.pcValue, yytext);
						return OPE_CONCAT_STR ; }

"="				{  	strcpy (yylval.pcValue, yytext);
					return OPE_EQU ; }
"^"{WS}"="		{  	strcpy (yylval.pcValue, yytext);
					return OPE_NOT_EQU ; }
"¬"{WS}"="		{  	strcpy (yylval.pcValue, yytext);
					return OPE_NOT_EQU ; }
"<"{WS}">"		{  	strcpy (yylval.pcValue, yytext);
					return OPE_NOT_EQU ; }
">"				{   strcpy (yylval.pcValue, yytext);
					return OPE_GT ; }
"<"				{   strcpy (yylval.pcValue, yytext);
					return OPE_LT ; }
"<"{WS}"="		{   strcpy (yylval.pcValue, yytext);
					return OPE_LE ; }
">"{WS}"="		{   strcpy (yylval.pcValue, yytext);
					return OPE_GE ; } 



{IDENTIFIER_M} {

		strcpy (yylval.pcValue, yytext);
		return IDENTIFIER ; 

	}


[ \t]						{ /* do nothing */ }
\n							{ return -1; }
.							{ return LEXER_ERROR ; }

%%

/* 
*************************************************************************************************
*										PROGRAM SECTION											*
************************************************************************************************* 
*/


int yywrap () 
{
	return -1;
}

