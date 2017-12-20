//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                sentenceparser.cpp
// Project:             mind x.x      
// Author:              Meta Software M.S. , S.A
// Date:                22/2/01
// Language:            C++
// Operating System:    UNIX, WIN32
// Design Document:     
//
//
// Definition:
//
//    Esta clase, implementa el parseador.
//	  Encapsula la función y el código generado por yacc convirtiéndolo en C++
//	  para poder trabajar con el analizador léxico en C++.	
//
//
//==============================================================================


#include "sentenceparser.hpp"
#include "m4hsparser_errors.hpp"

#include "basiclog.hpp"


SentenceParser::SentenceParser (yyFlexLexer &ai_oScanner)
{
	m_oLex = ai_oScanner;
	bHasBeenError = M4_FALSE;
	poFinalSelect = NULL;
}


SentenceParser::~SentenceParser()
{
}


m4int_t SentenceParser::yylex()
{
	m4int_t iRet = m_oLex.yylex ();
	yylval = m_oLex.yylval;
	return iRet;
}


m4void_t SentenceParser::yyerror (m4char_t * ai_pc) 
{
	BL_ERRORF (M4HSPARSER_E_SINTAX, "");

	bHasBeenError = M4_TRUE;
}


SelectInfo * SentenceParser::GetFinalSelect ()
{
	return poFinalSelect;
}


m4bool_t SentenceParser::HasBeenError ()
{
	return bHasBeenError;
}
