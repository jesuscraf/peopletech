%{

/****************************************************************************
RFNlexer.l
Interpreta los *.PAR y rellena una estructura en memoria
****************************************************************************/

//IMPORTANTE: Abrir la { de la accion en la misma linea que la regla, o mete break antes del 
//	codigo.

#include "rfnparser.h"	//Va primero, define el YYSTYPE
#include "rfnpartk.h"
#include "rfntrans.h"

#define YYLVAL Parser->yylval


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

[+-]?[0-9]+ { 

		// INTEGER 
		//
		// OJO, se pone [+-]? por que no hay expresiones. Si fuese asi, no se podria poner, es un problema de
		//	parser, pues en el lexer operaciones a+2 devolveria un a,+2, en vez de a,+,2
		 
		PAR->m_oLiteralData.StrCpy(yytext) ;
		return RFN_LITERAL_INTEGER ; 
	}


[+-]?[0-9]*"."[0-9]+{EXPONENT}?	|
[+-]?[0-9]+"."[0-9]*{EXPONENT}?	|
[+-]?[0-9]+EXPONENT {

		// 	FLOAT 
		//
		//Lo del signo, igual que en INTEGER
		//
	 	//-Si aparece ".":
 		//	-Debe aparecer la parte entera O la decimal (no pueden omitirse ambas).
	 	//	-El exponente SIEMPRE es opcional.    
 		//-Si NO aparece ".":                                   
		//	-Debe aparecer alguna parte entera.
		//	-Debe aparecer el exponente (si no, es un entero).
		//
		// NO se pone [+-]?, porque operaciones a+2 devolveria un a,+2, en vez de a,+,2

		PAR->m_oLiteralData.StrCpy(yytext) ;
		return RFN_LITERAL_FLOAT ; 
	}


\"[^\"\n]*\" { 

		//	LITERAL_STR
		//
		//Cualquier ocurrencia 'cadena' es un literal tipo texto. 
		//

		PAR->m_oLiteralData.ToASCIZ(yytext+1,yyleng-2) ;
		return RFN_LITERAL_STR ; 
	}

{O}{N} {

	//LITERAL_BOOLEAN

	PAR->m_oLiteralData.StrCpy("1") ;

	return RFN_LITERAL_BOOLEAN ;
}

{O}{F}{F} {

	//LITERAL_BOOLEAN

	PAR->m_oLiteralData.StrCpy("0") ;

	return RFN_LITERAL_BOOLEAN ;
}

{N}{O}{T} { 

		//	KEYWORDS
		//
		//	Definirlas ANTES de  IDENTIFIER_M, o pueden interpretarse como IDENTIDIER

		return RFN_NOT ; 
	} 

{H}{E}{A}{D}{E}{R}									{ return  RFN_HEADER ; } 
{O}{U}{T}{P}{U}{T}{N}{A}{M}{E}						{ return  RFN_OUTPUT_NAME; } 
{F}{O}{N}{T}{N}{A}{M}{E}							{ return  RFN_FONT_NAME ; } 
{F}{I}{X}{E}{D}										{ return  RFN_FIXED ; } 
{P}{C}{L}											{ return  RFN_PCL ; } 
{P}{S}{C}											{ return  RFN_PSC ; } 
{R}{V}{W}											{ return  RFN_RVW ; } 
{E}{S}{C}{P}										{ return  RFN_ESCP ; } 
{M}{A}{P}{N}{A}{M}{E}								{ return  RFN_MAP_NAME ; } 
{D}{V}{C}{F}{O}{N}{T}{I}{D}							{ return  RFN_DVC_FONT_ID ; } 
{D}{V}{C}{C}{H}{A}{R}{S}{E}{T}						{ return  RFN_DVC_CHARSET ; } 
{D}{V}{C}{P}{I}{T}{C}{H}							{ return  RFN_DVC_PITCH ; } 
{D}{V}{C}{D}{Y}										{ return  RFN_DVC_2Y ; } 
{D}{V}{C}{D}{X}										{ return  RFN_DVC_2X ; } 
{D}{V}{C}{C}{O}{M}{P}{R}							{ return  RFN_DVC_COMPR ; } 
{D}{V}{C}{S}{O}{F}{T}{F}{O}{N}{T}					{ return  RFN_DVC_SOFT_FONT ; } 
{D}{V}{C}{S}{O}{F}{T}{F}{O}{N}{T}{T}{R}{A}{N}{S}	{ return  RFN_DVC_SOFT_FONT_TRANS ; } 
{H}{E}{I}{G}{H}{T}									{ return  RFN_HEIGHT ; } 
{M}{A}{P}{H}{E}{I}{G}{H}{T} 						{ return  RFN_MAP_HEIGHT ; } 
{D}{V}{C}{H}{E}{I}{G}{H}{T}							{ return  RFN_DVC_HEIGHT ; } 
{S}{E}{L}{E}{C}{T}{I}{O}{N}{H}{E}{I}{G}{H}{T} 		{ return  RFN_SELECTION_HEIGHT ; } 
{D}{E}{S}{I}{G}{N}{H}{E}{I}{G}{H}{T}				{ return  RFN_DESIGN_HEIGHT; } 
{D}{E}{S}{I}{G}{N}{W}{I}{D}{T}{H}					{ return  RFN_DESIGN_WIDTH; } 
{F}{O}{R}{WS}{A}{L}{L}								{ return  RFN_FOR_ALL; } 

{A}{L}{L}											{ return  RFN_ALL ; } 
{B}													{ return  RFN_BOLD_ON ; } 
{N}{B}												{ return  RFN_BOLD_OFF; } 
{X}{B}												{ return  RFN_BOLD_ALL; } 
{I}													{ return  RFN_ITALIC_ON ; } 
{N}{I}												{ return  RFN_ITALIC_OFF; } 
{X}{I}												{ return  RFN_ITALIC_ALL; } 
{U}													{ return  RFN_UNDERLINE_ON ; } 
{N}{U}												{ return  RFN_UNDERLINE_OFF; } 
{X}{U}												{ return  RFN_UNDERLINE_ALL; } 

"{"	{ 
		// OPERATORS 

		return RFN_OPEN_BLOCK ; 
	}
"}"					{ return RFN_CLOSE_BLOCK ; }
"="					{ return RFN_OPE_LET ; }

[; \t\n]						{ /* do nothing */ }
[??]
"//".*$							{ /* do nothing */; }
.							{ printf("<ERROR, invalid char %c>\n", (char)yytext[0]); }

%%

/* 
*************************************************************************************************
*										PROGRAM SECTION											*
************************************************************************************************* 
*/

#include "flexlexer.h"
              



