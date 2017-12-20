//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             SQLParser.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:20/10/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//
//
//==============================================================================

#ifndef __RFNPARSER__H__
#define __RFNPARSER__H__


//==========================================================================ClRFN_Parser

class ClRFN_ParsePAR ;
class ClRFN_FlexLexer ;

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

#define YYSTYPE m4uint32_t    //Indices de ClTkStack 


class ClRFN_Parser 
{
MIT_PUBLIC:

	#if YYDEBUG != 0
	int yydebug;			/*  nonzero means print parse trace	*/
	/* Since this is uninitialized, it does not stop multiple parsers
		from coexisting.  */
	#endif

MIT_PUBLIC:

	ClRFN_Parser() {}  
	virtual ~ClRFN_Parser() {}

	int yyparse(YYPARSE_PARAM) ;

	YYSTYPE yylval;	//Asi lo puede ver el lexer, via puntero a la clase

MIT_PUBLIC:

	static void __yy_memcpy(char *from, char *to, int count) ;
	static void yyerror(char *Msg);


MIT_PUBLIC:

	// ESTO  ....

	ClRFN_ParsePAR *PAR ; //Simple Link
	ClRFN_FlexLexer *Lexer ;	//Simple Link

MIT_PUBLIC:

	void LinkParserPAR(ClRFN_ParsePAR *ai_PAR) { PAR=ai_PAR ; }
	void LinkLexer(ClRFN_FlexLexer *ai_Lexer) { Lexer=ai_Lexer ; }


	//	.... es necesario para poder poner simbolos

	void SetDebug(m4bool_t ai_bSw)  
		{
			#if YYDEBUG != 0
			yydebug = ai_bSw ? 1 : 0 ;			/*  nonzero means print parse trace	*/
			#endif
			;
		} 
} ;



#endif

