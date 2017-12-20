//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                parser.h
// Project:        calculo
// Author:         Meta Software M.S. , S.A
// Date:            20 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  EXTCONCI.DOC
//
//
// Definition:
//
//    Encapsula en un object el parser
////
//==============================================================================

#ifndef _PARSER_LN4_H_
#define _PARSER_LN4_H_

#include "flexlexer.h"
#include "cltypes.hpp"
#include "cldefine.hpp"

#include <ctype.h>
#include <stdio.h>



#include "m4string.hpp"

#include "gram_tab.h"
#include "trad.h"
class ClParser {
	
public:
	ClParser(yyFlexLexer &ai_oScanner, ClTrad *ai_Traductor);
	virtual ~ClParser();
	m4return_t Parse();
	void yyerror(char *ai_str, m4uint32_t ai_iNotSyntaxError=0);
	int yylex(void);
	int GetNumLine(){return m_oScanner.GetNumLine();};
	YYSTYPE	yylval;			/*  the semantic value of the		*/
protected:
	yyFlexLexer &m_oScanner;	//objeto scanner que usara para ller los tokens.
	ClTrad		*m_poTraductor;
	int	yychar;			/*  the lookahead symbol		*/
	
	/*  lookahead symbol			*/
	
protected:
	//**************************************
	//Declar. Funciones que genern el código
	//Estas funciones llaman a las del traductor 
	//que lo usa
	//**************************************

	/*operaciones*/
	virtual m4return_t StoreNumber (char *ai_number){return m_poTraductor->StoreNumber (ai_number);};
	virtual m4return_t StoreString (char *ai_string){return m_poTraductor->StoreString (ai_string);};
	virtual m4return_t StoreDate (char *ai_date){return m_poTraductor->StoreDate(ai_date);};
	virtual m4return_t LogOpAnd(void){ return m_poTraductor->LogOpAnd();};
	virtual m4return_t LogOpOr(void){ return m_poTraductor->LogOpOr();};
	virtual m4return_t LogOpNot(void){ return m_poTraductor->LogOpNot();};
	virtual m4return_t LogOpEqual(void){ return m_poTraductor->LogOpEqual();};
	virtual m4return_t LogOpNotEqual(void){ return m_poTraductor->LogOpNotEqual();};
	virtual m4return_t LogOpGreater(void){ return m_poTraductor->LogOpGreater();};
	virtual m4return_t LogOpGreaterEqual(void){ return m_poTraductor->LogOpGreaterEqual();};
	virtual m4return_t LogOpLess(void){ return m_poTraductor->LogOpLess();};
	virtual m4return_t LogOpLessEqual(void){ return m_poTraductor->LogOpLessEqual();};
	virtual m4return_t Assign(void){ return m_poTraductor->Assign();};
	virtual m4return_t Return(void){ return m_poTraductor->Return();};
	virtual m4return_t AritmOpAdd(void){ return m_poTraductor->AritmOpAdd();};
	virtual m4return_t AritmOpSub(void){ return m_poTraductor->AritmOpSub();};
	virtual m4return_t AritmOpMul(void){ return m_poTraductor->AritmOpMul();};
	virtual m4return_t AritmOpDiv(void){ return m_poTraductor->AritmOpDiv();};
	virtual m4return_t AritmOpSigno(void){ return m_poTraductor->AritmOpSigno();};

	/*relativo a nodos,items...*/
	virtual m4return_t AtributoMetodo(char *ai_celda, char* ai_metodo, char *ai_nArgs){return m_poTraductor->AtributoMetodo(ai_celda,ai_metodo,ai_nArgs);};
	virtual m4return_t Atributo(char *ai_celda, char *ai_atributo){return m_poTraductor->Atributo(ai_celda,ai_atributo);};
	virtual m4return_t Metodo(char *ai_metodo,char* ai_nArgs){return m_poTraductor->Metodo(ai_metodo,ai_nArgs);};
	virtual m4return_t MetodoRegla(char *ai_metodo, char *ai_regla,char* ai_nArgs){return m_poTraductor->MetodoRegla(ai_metodo,ai_regla, ai_nArgs);};
	virtual m4return_t MetodoPadre(char *ai_metodo,char* ai_nArgs){return m_poTraductor->MetodoPadre(ai_metodo,ai_nArgs);};
	virtual m4return_t MetodoReglaPadre(char *ai_metodo, char *ai_regla,char* ai_nArgs){return m_poTraductor->MetodoReglaPadre(ai_metodo,ai_regla,ai_nArgs);};
	virtual m4return_t ItemValorRead (char *ai_celda){return m_poTraductor->ItemValorRead(ai_celda);};
	virtual m4return_t ChannelFunction (char* ai_channel, char *ai_function, char *ai_nArgs) {return m_poTraductor->ChannelFunction(ai_channel,ai_function,ai_nArgs);};

	/*relativo al control*/
	virtual m4return_t If(void){return m_poTraductor->If();};
	virtual m4return_t Then (void){return m_poTraductor->Then();};
	virtual m4return_t Else (void){return m_poTraductor->Else();};
	virtual m4return_t Elseif (void){return m_poTraductor->Elseif();};
	virtual m4return_t Endif (void){return m_poTraductor->Endif();};
	virtual m4return_t Do (void){return m_poTraductor->Do();};
	virtual m4return_t For (void){return m_poTraductor->For();};
	virtual m4return_t To (void){return m_poTraductor->To();};
	virtual m4return_t Next (void){return m_poTraductor->Next();};
	virtual m4return_t While (void){return m_poTraductor->While();};
	virtual m4return_t Wend (void){return m_poTraductor->Wend();};
	virtual m4return_t Until (void){return m_poTraductor->Until();};
	virtual m4return_t StoreFor(m4char_t *ai_identifier){return m_poTraductor->StoreFor(ai_identifier);};
	virtual m4return_t FinFor(void){return m_poTraductor->FinFor();};
	virtual m4return_t Purge(void){return m_poTraductor->Purge();};
	virtual m4return_t Target(char *ai_target){return m_poTraductor->Target(ai_target);};
	virtual m4return_t Condition(void){return m_poTraductor->Condition();};
	virtual m4return_t WriteLine(void){return m_poTraductor->WriteLine();};

    //de argumentos
   	virtual m4return_t Arg(void){return m_poTraductor->Arg();};
    virtual m4return_t InitArgList(void){return m_poTraductor->InitArgList();}
    virtual m4uint32_t EndArgList(void){return m_poTraductor->EndArgList();}

	//sinonimos
	virtual m4return_t PopIdentifier(void){ return m_poTraductor->PopIdentifier();}
	virtual m4return_t NotifyPosibleItem(void){return m_poTraductor->NotifyPosibleItem();}
	virtual m4return_t NotifyPosibleTI(void){return m_poTraductor->NotifyPosibleTI();}


    //******************************************
	//Fin de las funciones que generan el codigo
	//*******************************************
#ifdef YYLSP_NEEDED
	YYLTYPE yylloc;			/*  location data for the lookahead	*/
	/*  symbol				*/
#endif
	int yynerrs;			/*  number of parse errors so far       */
#if YYDEBUG != 0
	int yydebug;			/*  nonzero means print parse trace	*/
							/* Since this is uninitialized, it does not stop multiple parsers
	from coexisting.  */
#endif
public:
	
	static const char yytranslate[] ;
#if YYDEBUG != 0
	static const short yyprhs[];
	static const short yyrhs[];
	static const short yyrline[];
	static const char * const yytname[];
#endif
	static const short yyr1[] ;
	static const short yyr2[] ;
	static const short yydefact[];
	static const short yydefgoto[] ;
	static const short yypact[] ;
	static const short yypgoto[] ;
	static const short yytable[] ;
	static const short yycheck[] ;
	//*******************
	//copieteo de memoria
	//*******************
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
	
};
//*********************************
//FIN DE LA DECLARACION DE LA CLASE
//*********************************




//*****************************************
//*****************************************
// Macros, includes,defines y otra morralla
// que incluía el original creado por BISON
//*****************************************
//*****************************************
#define YYBISON 1  /* Identify Bison output.  */

#define	IDENTIFICADOR	258
#define	NUMERO	259
#define	FECHA	260
#define	LITERAL	261
#define	NL	262
#define	EOF2	263
#define	IF	264
#define	THEN	265
#define	ELSE	266
#define	ENDIF	267
#define	WHILE	268
#define	WEND	269
#define	FOR	270
#define	TO	271
#define	NEXT	272
#define	DO	273
#define	RETURN	274
#define	UNTIL	275
#define	ELSEIF	276
#define	IGUAL	277
#define	AND	278
#define	OR	279
#define	MAYIGUAL	280
#define	MENIGUAL	281
#define	MAYOR	282
#define	MENOR	283
#define	DISTINTO	284
#define	NOT	285
#define	MAS	286
#define	MENOS	287
#define	MULTI	288
#define	DIVIDE	289
#define	ACCMIEMBRO	290
#define	ACCPADRE	291
#define	ACCATRIB	292
#define	ACCCHANNEL	293
#define	SEPINST	294
#define	MENOSUNARIO	295


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

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		222
#define	YYFLAG		-32768
#define	YYNTBASE	46

#define YYTRANSLATE(x) ((unsigned)(x) <= 295 ? yytranslate[x] : 95)




extern int yylex();
extern void yyerror();


#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0

/* Bug 0151800
Se elimina el uso de alloca porque desborda el stack
*/
#ifdef YYLSP_NEEDED
#define YYRETURN(x)			\
{							\
	free(yyss);				\
	free(yyvs);				\
	free(yyls);				\
	return(x);				\
}
#else
#define YYRETURN(x)			\
{							\
	free(yyss);				\
	free(yyvs);				\
	return(x);				\
}
#endif

#define YYACCEPT	YYRETURN(0)
#define YYABORT 	YYRETURN(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
This remains here temporarily to ease the
transition to the new meaning of YYERROR, for GCC.
Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
/*
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
*/
// El prepocesador aCC de HP-UX no entiende la barra \ 
// con ficheros en formato MS-DOS, asi que la escribimos en una sola linea
// arriba esta comentada la original
#define YYBACKUP(token, value) 	do	if (yychar == YYEMPTY && yylen == 1) { yychar = (token), yylval = (value);	yychar1 = YYTRANSLATE (yychar);	YYPOPSTACK;	goto yybackup; } else { yyerror ("syntax error: cannot back up"); YYERROR; } while (0)

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

/* If nonreentrant, generate the variables here */


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


#endif // _PARSER_LN4_H_
