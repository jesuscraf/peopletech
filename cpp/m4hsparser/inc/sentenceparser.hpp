//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                sentenceparser.hpp
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
//	  Encapsula la funci�n y el c�digo generado por yacc convirti�ndolo en C++
//	  para poder trabajar con el analizador l�xico en C++.
//	  Genera la estructura de la select parseada que luego habr� que liberar.
//
//
//==============================================================================



// Pasos para utilizar los ficheros generados por FLEX y BISON:
//	1. Generar los ficheros con Flex y Bison.
//	2. Eliminar el include de <unistd.h> para win32 en sqllexer.cpp usando un #ifdef
//	3. A�adir a la clase FlexLexer la variable yyval del tipo YYSTYPE e incluir ah� el fichero "sqlpartk.h"
//	4. En el fichero sqlpartk.h meter un #ifdef para evitar compilaciones m�ltiples,
//		eliminar la declaraci�n extern de yylval e incluir el fichero selectinfo.hpp
//	5. En la clase SentenceParser:
//			- Incluir como miembros las variables globales generadas en sqlparser.cpp y eliminarlas de ah�.
//			- Incluir antes de la clase la definici�n de tipos proveniente del mismo fichero, quitando el YYSTYPE			
//  6. Cambiar el prototipo de la funci�n yyparse, haciendo que sea mienbro de SentenceParser.
//				SentenceParser::yyparse ......
//	   El include SentenceParser.hpp lo debe haber a�adido autom�ticamente Bison en el c�digo.
//	7. Eliminar las declaraciones extern de yylex y yyerror de sqlparser.cpp
//	8. Justo despu�s de estas declaraciones hay que meter un trozo de c�digo que permite la portabilidad
//		a UNIX, coger este c�digo de m4vm, del fichero ClPArser, donde se encuentran todos los defines
//		para la portabilidad de "alloca".
//
//	..........Rezar un poquito y saber que se hace.........




#if !defined(AFX_SENTECEPARSER_HPP__8380F1F1_1C88_11D5_BEE4_00C04FA913F4__INCLUDED_)
#define AFX_SENTECEPARSER_HPP__8380F1F1_1C88_11D5_BEE4_00C04FA913F4__INCLUDED_


#include "m4stl.hpp"
#include "selectinfo.hpp"
#include "elementfactory.hpp"

#include "flexlexer.h"

/*
typedef union { 
		char pcValue[80];
		ColumnInfo * poColumn;
		TableInfo * poTable;
		ItemInfo * poItem;
		TextInfo * poText;
		ElementInfo * poElement;
		SelectInfo * poSelect;
		FunctionInfo * poFunction;
		JoinInfo * poJoin;
		} YYSTYPE;
*/
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

#ifndef YYDEBUG
#define YYDEBUG 1
#endif


class SentenceParser  
{

public:

	SentenceParser (yyFlexLexer &ai_oLex);
	virtual ~SentenceParser();

	m4int_t yyparse ();

	SelectInfo * GetFinalSelect ();

	m4bool_t HasBeenError ();

private:

	m4int_t yylex ();

	m4void_t yyerror (m4char_t * ai_pc);

	// Analizador l�xico
	yyFlexLexer m_oLex;

	// Lista con las select. Implementa un pila de SELECTs donde se van anidando 
	// todas las que se encuentran para tratar las subselects.
	// La cima es la select actual. La select m�s profunda en la pila es la select principal.
	list<SelectInfo *> vpoSelect;

	// Variable temporal para almacenar la �ltima select procesada
	SelectInfo * poFinalSelect;

	// Contador que cuenta el nivel l�gico al que se encuentra el join dentro de una expresi�n
	//			AND				Nivel 0
	//		OR					Nivel 1
	//	AND						Nivel 2
	//		AND					Nivel 2
	//
	//	Se cambia el nivel l�gico cuando hay un cambio de OR a AND o al contrario en una expresi�n.
	list <m4int_t> viTreeExpresionLevel;

	// Ultimo operador encontrado, ser� true cuando el �ltimo sea un AND
	// Lo usamos para saber cuando aunmentar el nivel
	// Mantenemos una pila para controlar las subselects
	list <m4bool_t> vbLastIsAnd;

	// Flag que se activa cuando ha habido un error
	// Mantenemos una pila para controlar las subselects
	m4bool_t bHasBeenError;

	// Factor�a de elementos
	ClElementFactory	m_oFactory ;

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

};

#endif // !defined(AFX_SENTECEPARSER_HPP__8380F1F1_1C88_11D5_BEE4_00C04FA913F4__INCLUDED_)
