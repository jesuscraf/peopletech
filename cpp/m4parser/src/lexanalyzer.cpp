//## begin module%349505E503A9.cm preserve=no
//## end module%349505E503A9.cm

//## begin module%349505E503A9.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%349505E503A9.cp

//## Module: LexAnalyzer%349505E503A9; Package body
//## Subsystem: M4Parser::src%379D926A02BD
//## Source file: D:\FuentesServidor\M4Parser\version\src\lexanalyzer.cpp

//## begin module%349505E503A9.additionalIncludes preserve=no
//## end module%349505E503A9.additionalIncludes

//## begin module%349505E503A9.includes preserve=yes
//## end module%349505E503A9.includes

// LexAnalyzer
#include <lexanalyzer.hpp>
// SymbolTable
#include <symboltable.hpp>
//## begin module%349505E503A9.declarations preserve=no
//## end module%349505E503A9.declarations

//## begin module%349505E503A9.additionalDeclarations preserve=yes
#include <m4string.hpp>
//## end module%349505E503A9.additionalDeclarations


// Class ClLexAnalyzer 







ClLexAnalyzer::ClLexAnalyzer ()
  //## begin ClLexAnalyzer::ClLexAnalyzer%882181783.hasinit preserve=no
      : m_oCountTokens(0)
  //## end ClLexAnalyzer::ClLexAnalyzer%882181783.hasinit
  //## begin ClLexAnalyzer::ClLexAnalyzer%882181783.initialization preserve=yes
  //## end ClLexAnalyzer::ClLexAnalyzer%882181783.initialization
{
  //## begin ClLexAnalyzer::ClLexAnalyzer%882181783.body preserve=yes

	m_szActualToken = new m4char_t[255 * sizeof (m4char_t)];

  //## end ClLexAnalyzer::ClLexAnalyzer%882181783.body
}


ClLexAnalyzer::~ClLexAnalyzer ()
{
  //## begin ClLexAnalyzer::~ClLexAnalyzer%882263090.body preserve=yes

	// Iterator to go along the symbol table
	ClSymbolTable::iterator TableIterator = m_oSymbolTable.begin();

	// Clearing memory
	while (TableIterator != m_oSymbolTable.end())
	{
		delete ((*TableIterator).first);
		TableIterator++;
	}
	delete [] (m_szActualToken);
	
  //## end ClLexAnalyzer::~ClLexAnalyzer%882263090.body
}



//## Other Operations (implementation)
m4return_t ClLexAnalyzer::AddSymbol (m4token_t a_acToken)
{
  //## begin ClLexAnalyzer::AddSymbol%882263087.body preserve=yes
#ifdef _aCC
	typedef pair <char *const,unsigned short> Pareja;
#endif
	// Checks if token belongs to the symbol table
	ClSymbolTable::iterator oFindResult;

	// Looks for token
	oFindResult = Find(a_acToken);

	// If found, returns error
	if (oFindResult != m_oSymbolTable.end())
		return M4_ERROR;

	// else, stores new value
	else {
		++m_oCountTokens;
#ifdef _aCC
		Pareja x(a_acToken, m_oCountTokens);
		m_oSymbolTable.insert(x);
#else
		m_oSymbolTable.insert(make_pair(a_acToken, m_oCountTokens));
#endif
		return M4_SUCCESS;
	}
  
  //## end ClLexAnalyzer::AddSymbol%882263087.body
}

m4Idtoken_t ClLexAnalyzer::GetWord (m4pchar_t* a_szTokenValue)
{
  //## begin ClLexAnalyzer::GetWord%882263088.body preserve=yes

	// Char to read character by character
	m4char_t cAux;

	// Pointer to the actual position in ActualToken
	m4pchar_t pActualTokenPosition = m_szActualToken;

	// String to store word (used to handle commands larger than 254 chars)
	M4ClString oStringToken;

	// Iterator to go along oStringToken
	M4ClString::iterator oStringTokenIterator = oStringToken.begin();

	// Iterator to go along the symbol table
	ClSymbolTable::iterator oFindResult;

	// Return variable
	m4Idtoken_t RetVal;

	// Counter to control the limit of 255 characters per command 
	m4uint16_t nCharCounter = 1;


	// Removing blanks
	do {
		cAux = *m_szActualCadena;
		m_szActualCadena++;
	} while (cAux==' ');

	// Storing first character in the result token
	oStringToken.insert(oStringTokenIterator++, cAux);

	// If end of command, returns 0
	if ((cAux==';') || (cAux=='\0')) {
		oStringTokenIterator = oStringToken.begin();
		while (oStringTokenIterator != oStringToken.end()) {
			*pActualTokenPosition = (*oStringTokenIterator);
			pActualTokenPosition++;
			oStringTokenIterator++;
		}
		*pActualTokenPosition = '\0';
		*a_szTokenValue = m_szActualToken;
		RetVal = 0;
	}

	else {

		// Reads the whole word
		do {
			cAux = *m_szActualCadena;
			m_szActualCadena++;
/*			*pActualTokenPosition = cAux;
			pActualTokenPosition++;*/
			oStringToken.insert(oStringTokenIterator++, cAux);
		} while ((cAux!=' ') && (cAux!=';') && (cAux!='\0'));

		// Recovers the last read character
		m_szActualCadena--;


		// Copies the string to the return pointer
		oStringTokenIterator = oStringToken.begin();
		while ((oStringTokenIterator != oStringToken.end()) &&
				nCharCounter < 254) {
			*pActualTokenPosition = *oStringTokenIterator;
			oStringTokenIterator++;
			pActualTokenPosition++;
			nCharCounter++;
		}
		*(--pActualTokenPosition) = '\0';

		*a_szTokenValue = m_szActualToken;

		// Looks for token in symbol table
		oFindResult = Find(m_szActualToken);

		if (oFindResult != m_oSymbolTable.end())
			RetVal = (*oFindResult).second;
		else
			RetVal = 0;

	}

	return RetVal;

  //## end ClLexAnalyzer::GetWord%882263088.body
}

m4return_t ClLexAnalyzer::InicString (m4pchar_t a_acCadena)
{
  //## begin ClLexAnalyzer::InicString%882263089.body preserve=yes

	m_szCadena = a_acCadena;
	m_szActualCadena = m_szCadena;
	return M4_SUCCESS;

  //## end ClLexAnalyzer::InicString%882263089.body
}

ClSymbolTable::iterator ClLexAnalyzer::Find (m4token_t a_oToken)
{
  //## begin ClLexAnalyzer::Find%882438505.body preserve=yes

	// Iterator to go along the symbol table
	ClSymbolTable::iterator RetVal = m_oSymbolTable.begin();

	// True when the token is found
	m4bool_t bFound = M4_FALSE;

	while ((RetVal != m_oSymbolTable.end()) && !bFound) {
		bFound = M4_BOOL( !strcmp(a_oToken, (*RetVal).first) );
		if (!bFound)
			RetVal++;
	}
	return RetVal;

  //## end ClLexAnalyzer::Find%882438505.body
}

m4void_t ClLexAnalyzer::ImprimeLista ()
{
  //## begin ClLexAnalyzer::ImprimeLista%882783679.body preserve=yes
	ClSymbolTable::iterator TableIterator = m_oSymbolTable.begin();

	while (TableIterator != m_oSymbolTable.end()) {
		cout << (*TableIterator).first << ' ';
		TableIterator++;
	}
	cout.flush();

  //## end ClLexAnalyzer::ImprimeLista%882783679.body
}

// Additional Declarations
  //## begin ClLexAnalyzer%3494FE6102BC.declarations preserve=yes
  //## end ClLexAnalyzer%3494FE6102BC.declarations

//## begin module%349505E503A9.epilog preserve=yes
//## end module%349505E503A9.epilog
