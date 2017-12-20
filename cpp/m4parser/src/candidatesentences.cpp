//## begin module%3496C23700D9.cm preserve=no
//## end module%3496C23700D9.cm

//## begin module%3496C23700D9.cp preserve=no
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
//## end module%3496C23700D9.cp

//## Module: CandidateSentences%3496C23700D9; Package body
//## Subsystem: M4Parser::src%379D926A02BD
//## Source file: D:\FuentesServidor\M4Parser\version\src\candidatesentences.cpp

//## begin module%3496C23700D9.additionalIncludes preserve=no
//## end module%3496C23700D9.additionalIncludes

//## begin module%3496C23700D9.includes preserve=yes
//## end module%3496C23700D9.includes

// SintaxisTable
#include <sintaxistable.hpp>
// CandidateSentences
#include <candidatesentences.hpp>
//## begin module%3496C23700D9.declarations preserve=no
//## end module%3496C23700D9.declarations

//## begin module%3496C23700D9.additionalDeclarations preserve=yes
#ifdef _UNIX
#include "m4stl.hpp"
#endif
//## end module%3496C23700D9.additionalDeclarations


// Class ClCandidateSentences 






ClCandidateSentences::ClCandidateSentences ()
  //## begin ClCandidateSentences::ClCandidateSentences%882292380.hasinit preserve=no
      : m_uSentenceCounter(0)
  //## end ClCandidateSentences::ClCandidateSentences%882292380.hasinit
  //## begin ClCandidateSentences::ClCandidateSentences%882292380.initialization preserve=yes
  //## end ClCandidateSentences::ClCandidateSentences%882292380.initialization
{
  //## begin ClCandidateSentences::ClCandidateSentences%882292380.body preserve=yes
  //## end ClCandidateSentences::ClCandidateSentences%882292380.body
}


ClCandidateSentences::~ClCandidateSentences ()
{
  //## begin ClCandidateSentences::~ClCandidateSentences%882292381.body preserve=yes
  //## end ClCandidateSentences::~ClCandidateSentences%882292381.body
}



//## Other Operations (implementation)
m4return_t ClCandidateSentences::InicTable (ClSintaxisTable::CLSINTAXISTABLE a_oTable)
{
  //## begin ClCandidateSentences::InicTable%882292382.body preserve=yes

	m_oSintaxisTable = a_oTable;
	return M4_SUCCESS;

  //## end ClCandidateSentences::InicTable%882292382.body
}

m4return_t ClCandidateSentences::InicSentence (m4token_t a_oToken)
{
  //## begin ClCandidateSentences::InicSentence%882292383.body preserve=yes

	// Iterator to go along the syntax table
	ClSintaxisTable::CLSINTAXISTABLE::iterator oSyntaxTableIterator = m_oSintaxisTable.begin();

	// Iterator to go along a sentence frame
	ClSentenceFrame::iterator oSentenceIterator;

	// Return value
	m4return_t RetVal;
	
	// Initialises structures
	m_oCandidateSentences.clear();
	m_uSentenceCounter = 0;
	m_uSentencePosition = 1;

	// Main loop (along the syntax table)
	while (oSyntaxTableIterator != m_oSintaxisTable.end()) {

		oSentenceIterator = ((*oSyntaxTableIterator).second).begin();

		// If firs token in sentence matches a_oToken, it's a candidate sentence
		if (!(strcmp(*oSentenceIterator, a_oToken))) {
			m_oCandidateSentences.insert(*oSyntaxTableIterator);
			m_uSentenceCounter++;
		}
		oSyntaxTableIterator++;
	}

	if (m_uSentenceCounter == 0)
		RetVal = M4_ERROR;
	else
		RetVal = M4_SUCCESS;

	return RetVal;

  //## end ClCandidateSentences::InicSentence%882292383.body
}

m4return_t ClCandidateSentences::AddToken (m4token_t a_oToken)
{
  //## begin ClCandidateSentences::AddToken%882292384.body preserve=yes

	// Iterator to go along the candidate sentences table
	ClSintaxisTable::CLSINTAXISTABLE::iterator oSyntaxIterator = m_oCandidateSentences.begin();

	// Iterator to go along a sentence frame
	ClSentenceFrame::iterator oSentenceIterator;

	// Auxiliar iterator for the candidate sentences table
	ClSintaxisTable::CLSINTAXISTABLE::iterator oSyntaxIteratorAux;

	// Return value
	m4return_t RetVal;

	// Main loop (along candidate sentences table)
	while (oSyntaxIterator != m_oCandidateSentences.end()) {
		oSentenceIterator = ((*oSyntaxIterator).second).begin();

		// Checks token against sentence frame
		if ((((*oSyntaxIterator).second).size() <= m_uSentencePosition) ||
				(strcmp(*(oSentenceIterator + m_uSentencePosition), a_oToken))) {
			oSyntaxIteratorAux = oSyntaxIterator++;
			m_oCandidateSentences.erase(oSyntaxIteratorAux);
			m_uSentenceCounter--;
		}
		else
			oSyntaxIterator++;
	}


	m_uSentencePosition++;
	if (m_uSentenceCounter == 0)
		RetVal = M4_ERROR;
	else
		RetVal = M4_SUCCESS;

	return RetVal;

  //## end ClCandidateSentences::AddToken%882292384.body
}

ClSintaxisTable::CLSINTAXISTABLE ClCandidateSentences::EndSentence ()
{
  //## begin ClCandidateSentences::EndSentence%882292385.body preserve=yes

	// Iterator to go along the candidate sentences table
	ClSintaxisTable::CLSINTAXISTABLE::iterator oSyntaxTableIterator = m_oCandidateSentences.begin();

	// Auxiliar iterator for the candidate sentences table
	ClSintaxisTable::CLSINTAXISTABLE::iterator oSyntaxIteratorAux;

	// Main loop (along candidate sentences table)
	while (oSyntaxTableIterator != m_oCandidateSentences.end()) {

		// If # elements in command line != # elements in sentence, remove it from
		// candidate sentences table
		if (((*oSyntaxTableIterator).second).size() != m_uSentencePosition) {
			oSyntaxIteratorAux = oSyntaxTableIterator++;
			m_oCandidateSentences.erase(oSyntaxIteratorAux);
			m_uSentenceCounter--;
		}
		else
			oSyntaxTableIterator++;
	}

	// If # sentences remaining in candidate sentences table != 1, returns empty table
	if (m_uSentenceCounter != 1)
		m_oCandidateSentences.clear();

	return m_oCandidateSentences;
  //## end ClCandidateSentences::EndSentence%882292385.body
}

m4void_t ClCandidateSentences::ImprimeTablaSintactica ()
{
  //## begin ClCandidateSentences::ImprimeTablaSintactica%882783677.body preserve=yes
	ClSintaxisTable::CLSINTAXISTABLE::iterator STaux = m_oSintaxisTable.begin();
	ClSentenceFrame::iterator SFaux = ((*STaux).second).begin();

	cout << endl;
	cout << "Tabla sintáctica:" << endl;
	while (STaux != m_oSintaxisTable.end()) {
		cout << "Sentencia: ";
		while (SFaux != ((*STaux).second).end()) {
			cout << (*SFaux) << ' ';
			SFaux++;
		}
		STaux++;
		SFaux = ((*STaux).second).begin();
		cout << endl;
	}
	cout.flush();
  //## end ClCandidateSentences::ImprimeTablaSintactica%882783677.body
}

m4void_t ClCandidateSentences::ImprimeSentenciasCandidatas ()
{
  //## begin ClCandidateSentences::ImprimeSentenciasCandidatas%882783678.body preserve=yes

	cout << endl;
	ClSintaxisTable::CLSINTAXISTABLE::iterator STaux = m_oCandidateSentences.begin();
	ClSentenceFrame::iterator SFaux = ((*STaux).second).begin();
	cout << "Tabla de sentencias candidatas:" << endl;
	while (STaux != m_oCandidateSentences.end()) {
		cout << "Sentencia: ";
		while (SFaux != ((*STaux).second).end()) {
			cout << (*SFaux) << ' ';
			SFaux++;
		}
		STaux++;
		SFaux = ((*STaux).second).begin();
		cout << endl;
	}
	cout.flush();
	
  //## end ClCandidateSentences::ImprimeSentenciasCandidatas%882783678.body
}

// Additional Declarations
  //## begin ClCandidateSentences%3494FE6102EE.declarations preserve=yes
  //## end ClCandidateSentences%3494FE6102EE.declarations

//## begin module%3496C23700D9.epilog preserve=yes
//## end module%3496C23700D9.epilog
