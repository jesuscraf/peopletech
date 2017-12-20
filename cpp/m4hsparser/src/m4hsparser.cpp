//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                m4hsparser.cpp
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
//    Implementación del interface externo de lal ibrería
//
//
//==============================================================================


#include "m4hsparser.hpp"
#include "sentenceparser.hpp"


// Definido en el código generado por yacc
extern m4int_t yydebug;


M4HsParser::M4HsParser () 
{
}


M4HsParser::~M4HsParser()
{
}


m4return_t M4HsParser::SentenceMerge (	istream &ai_oChild, 
										istream &ai_oParent, 
										ostream &aio_oResult )
{ 
	#ifdef __M4SENTENCEPARSER_DEBUG__
		yydebug = 1;
	#else
		yydebug = 0;
	#endif


	SelectInfo * poChildSelect = 0;
	SelectInfo * poParentSelect = 0;


	m4bool_t ChildIsEmpty = M4_FALSE;
	m4bool_t ParentIsEmpty = M4_FALSE;

	// Debido al bug 0057854 Debeoms controlar que nos pasen una sentencia a NULL
	// Lo hacemos leyendo un caracter y comprobando si la cadena
	// es vacía o no. Si no lo es devolvemos al stream de nuevo ese caracter leido
	// Es la forma más fácil de hacerlo (chapuza). 

	// Comprobamos si alguna sentenca viene vacía
	m4char_t cFirstChar = ' ';
	cFirstChar = ai_oChild.get ();
	if ( (cFirstChar == '\0') || (ai_oChild.eof()) )
		ChildIsEmpty = M4_TRUE;
	else 
		ai_oChild.putback (cFirstChar);
		
	cFirstChar = ai_oParent.get ();
	if ( (cFirstChar == '\0') || (ai_oParent.eof()) )
		ParentIsEmpty = M4_TRUE;
	else
		ai_oParent.putback (cFirstChar);

	
	// Si alguna viene vacía devolvemos lo adecuado
	if (ChildIsEmpty && ParentIsEmpty)
	{
		aio_oResult << '\0';
		aio_oResult.flush();
		
		return M4_SUCCESS;
	} 
	else if ( ChildIsEmpty )
	{
		char c = 0;

		// fix bug 0065708
		for (;;)
		{
			c = (char) ai_oParent.get ();
			if (ai_oParent.eof())
			{
				break;
			}
			aio_oResult << c;
		}
		
		aio_oResult << '\0';
		aio_oResult.flush();

		return M4_SUCCESS;
	} 
	else if ( ParentIsEmpty ) 
	{
		char c = 0;

		// fix bug 0065708
		for (;;)
		{
			c = (char) ai_oChild.get ();
			if (ai_oChild.eof())
			{
				break;
			}
			aio_oResult << c;
		}
		
		aio_oResult << '\0';
		aio_oResult.flush();

		return M4_SUCCESS;
	}


	// Parseo de la select hija
	yyFlexLexer oChildLex (&ai_oChild);
	SentenceParser oChildParser (oChildLex);
	
	oChildParser.yyparse();
	if (oChildParser.HasBeenError () )
	{
		return M4_ERROR;
	}

	poChildSelect = oChildParser.GetFinalSelect();
	
	// Paseo de la select padre
	yyFlexLexer oParentLex (&ai_oParent);
	SentenceParser oParentParser (oParentLex);

	oParentParser.yyparse();
	if (oParentParser.HasBeenError () )
	{
		return M4_ERROR;
	}

	poParentSelect = oParentParser.GetFinalSelect();

	if ( poChildSelect->Merge (poParentSelect) == M4_ERROR)
	{
		return M4_ERROR;
	}

	poChildSelect->Dump (aio_oResult);

	aio_oResult << '\0';
	aio_oResult.flush();

	return M4_SUCCESS;	
};

