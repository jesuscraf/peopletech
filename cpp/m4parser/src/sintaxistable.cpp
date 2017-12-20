//## begin module%34967878001E.cm preserve=no
//## end module%34967878001E.cm

//## begin module%34967878001E.cp preserve=no
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
//## end module%34967878001E.cp

//## Module: SintaxisTable%34967878001E; Package body
//## Subsystem: M4Parser::src%379D926A02BD
//## Source file: D:\FuentesServidor\M4Parser\version\src\sintaxistable.cpp

//## begin module%34967878001E.additionalIncludes preserve=no
//## end module%34967878001E.additionalIncludes

//## begin module%34967878001E.includes preserve=yes
//## end module%34967878001E.includes

// ParserDefs
#include <parserdefs.hpp>
// SintaxisTable
#include <sintaxistable.hpp>
//## begin module%34967878001E.declarations preserve=no
//## end module%34967878001E.declarations

//## begin module%34967878001E.additionalDeclarations preserve=yes
#ifdef _UNIX
#include "m4stl.hpp"
#endif
//## end module%34967878001E.additionalDeclarations


// Class ClSintaxisTable 




ClSintaxisTable::ClSintaxisTable ()
  //## begin ClSintaxisTable::ClSintaxisTable%882271173.hasinit preserve=no
  //## end ClSintaxisTable::ClSintaxisTable%882271173.hasinit
  //## begin ClSintaxisTable::ClSintaxisTable%882271173.initialization preserve=yes
  //## end ClSintaxisTable::ClSintaxisTable%882271173.initialization
{
  //## begin ClSintaxisTable::ClSintaxisTable%882271173.body preserve=yes
  //## end ClSintaxisTable::ClSintaxisTable%882271173.body
}


ClSintaxisTable::~ClSintaxisTable ()
{
  //## begin ClSintaxisTable::~ClSintaxisTable%882271174.body preserve=yes

	// Iterator to go along the syntaxis table
	CLSINTAXISTABLE::iterator oTableIterator = m_oSintaxisTable.begin();

	// Iterator to go along the sentence frame
	ClSentenceFrame::iterator oSentenceIterator;

	// Clearing memory
	while (oTableIterator != m_oSintaxisTable.end())
	{
		delete ((*oTableIterator).first);
		oSentenceIterator = ((*oTableIterator).second).begin();
		while (oSentenceIterator != ((*oTableIterator).second).end())
		{
			delete (*oSentenceIterator);
			oSentenceIterator++;
		}

		oTableIterator++;
	}

  //## end ClSintaxisTable::~ClSintaxisTable%882271174.body
}



//## Other Operations (implementation)
m4return_t ClSintaxisTable::AddToken (m4token_t a_szToken)
{
  //## begin ClSintaxisTable::AddToken%882271175.body preserve=yes

	m_SentenceAux.push_back(a_szToken);
	return M4_SUCCESS;

  //## end ClSintaxisTable::AddToken%882271175.body
}

m4return_t ClSintaxisTable::NewSentenceFrame (m4command_t a_oIdCommand)
{
  //## begin ClSintaxisTable::NewSentenceFrame%882271176.body preserve=yes
#ifdef _aCC
	typedef pair <char *const,ClSentenceFrame> Pareja;
	Pareja x( a_oIdCommand, m_SentenceAux);
#else
	SintaxPair x( a_oIdCommand, m_SentenceAux);
#endif	//m_oSintaxisTable.insert( make_pair( a_oIdCommand, m_SentenceAux));
	m_oSintaxisTable.insert(x);
	m_SentenceAux.clear();
	return M4_SUCCESS;

  //## end ClSintaxisTable::NewSentenceFrame%882271176.body
}

ClSintaxisTable::CLSINTAXISTABLE ClSintaxisTable::GetTable ()
{
  //## begin ClSintaxisTable::GetTable%882292379.body preserve=yes
	return m_oSintaxisTable;
  //## end ClSintaxisTable::GetTable%882292379.body
}

m4void_t ClSintaxisTable::ImprimeSentences ()
{
  //## begin ClSintaxisTable::ImprimeSentences%882783680.body preserve=yes
	CLSINTAXISTABLE::iterator STaux = m_oSintaxisTable.begin();
	ClSentenceFrame::iterator SFaux = ((*STaux).second).begin();
	while (STaux != m_oSintaxisTable.end()) {
		cout << "Sentencia " << ((*STaux).first) << ": ";
		while (SFaux != ((*STaux).second).end()) {
			cout << (*SFaux) << ' ';
			SFaux++;
		}
		STaux++;
		SFaux = ((*STaux).second).begin();
		cout << endl;
	}
	cout.flush();
  //## end ClSintaxisTable::ImprimeSentences%882783680.body
}

// Additional Declarations
  //## begin ClSintaxisTable%3494FE61030C.declarations preserve=yes
  //## end ClSintaxisTable%3494FE61030C.declarations

//## begin module%34967878001E.epilog preserve=yes
//## end module%34967878001E.epilog
