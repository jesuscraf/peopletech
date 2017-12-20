//## begin module%3498103002A9.cm preserve=no
//## end module%3498103002A9.cm

//## begin module%3498103002A9.cp preserve=no
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
//## end module%3498103002A9.cp

//## Module: Parser%3498103002A9; Package body
//## Subsystem: M4Parser::src%379D926A02BD
//## Source file: D:\FuentesServidor\M4Parser\version\src\parser.cpp

//## begin module%3498103002A9.additionalIncludes preserve=no
//## end module%3498103002A9.additionalIncludes

//## begin module%3498103002A9.includes preserve=yes
//## end module%3498103002A9.includes

// TokenCommandTable
#include <tokencmdtable.hpp>
// LexAnalyzer
#include <lexanalyzer.hpp>
// ParserDefs
#include <parserdefs.hpp>
// SintaxisTable
#include <sintaxistable.hpp>
// CandidateSentences
#include <candidatesentences.hpp>
// Parser
#include <parser.hpp>
// FileReader
#include <filereader.hpp>
// CommanDefs
#include <commandefs.hpp>
//## begin module%3498103002A9.declarations preserve=no
//## end module%3498103002A9.declarations

//## begin module%3498103002A9.additionalDeclarations preserve=yes
//## end module%3498103002A9.additionalDeclarations


// Class ClParser 






ClParser::ClParser (ClTokenCommandTable* ai_oTokenCommandTable, m4pchar_t ai_szInicFileName)
  //## begin ClParser::ClParser%882380970.hasinit preserve=no
  //## end ClParser::ClParser%882380970.hasinit
  //## begin ClParser::ClParser%882380970.initialization preserve=yes
  //## end ClParser::ClParser%882380970.initialization
{
  //## begin ClParser::ClParser%882380970.body preserve=yes

	if (m_pClFileReader.InicFile(ai_szInicFileName) == M4_ERROR) {
		//cout << "No se pudo inicializar el fichero de parser %s" << ai_szInicFileName << endl;
		m_poTokenCommandTable = NULL;
	}
	else {
		// Auxiliar command id
		m4command_t oCommandId;

		while (m_pClFileReader.InSection("TOKENS"))
			m_pClLexAnalyzer.AddSymbol(m_pClFileReader.GetToken());


			// Commands reading
		while (m_pClFileReader.InSection("COMMANDS")) {
			oCommandId = m_pClFileReader.GetIdCommand();
			while (!m_pClFileReader.EndLine())
				m_oSintaxis.AddToken(m_pClFileReader.GetToken());
			m_oSintaxis.NewSentenceFrame(oCommandId);
		}

		m_pClFileReader.CloseFile();

		m_pClCandidateSentences.InicTable(m_oSintaxis.GetTable());

		m_poTokenCommandTable = ai_oTokenCommandTable;
	}

  //## end ClParser::ClParser%882380970.body
}


ClParser::~ClParser ()
{
  //## begin ClParser::~ClParser%882380971.body preserve=yes
//	m_poTokenCommandTable -> Clear();
//	delete m_poTokenCommandTable;
  //## end ClParser::~ClParser%882380971.body
}



//## Other Operations (implementation)
m4return_t ClParser::Parse (m4pchar_t ai_pSentence, ClSentenceTable *aio_pSentenceTable)
{
  //## begin ClParser::Parse%882380972.body preserve=yes

	//comprobamos que se ha podido abrir el fichero de parser
	if (m_poTokenCommandTable==NULL)
		return M4_ERROR;

	// String value of next word in the command line
	m4token_t oTokenValue;

	// Word identifier
	m4Idtoken_t oTokenId;

	// Return value
	m4return_t RetVal = M4_SUCCESS;

	// Auxiliar table for retreiving candidate sentences table
	ClSintaxisTable::CLSINTAXISTABLE oAuxTable;

	// Variable to retreive a pointer to the command to execute
	m4pcommand_t poComando;

	// Queue to store command arguments
	m4parsercommandargs_t oColaArgumentos;

	// Auxiliar structure to store a pointer_to_command/arguments pair
	ClSenTStruct oSenTStAux;

	// Auxiliar char pointer to store arguments
	m4pchar_t szArgAux=NULL;

	// Initialises string in the lexycal analiser
	m_pClLexAnalyzer.InicString(ai_pSentence);

	// Gets first word of the command line
	oTokenId = m_pClLexAnalyzer.GetWord(&oTokenValue);

	// Main loop (until command line is finished)
	while ((RetVal == M4_SUCCESS) && !FinalDeBloque(oTokenValue)) {

		// Stores first possible candidate sentences
		RetVal = m_pClCandidateSentences.InicSentence(oTokenValue);

		oTokenId = m_pClLexAnalyzer.GetWord(&oTokenValue);

		// Secondary loop (until a ';' is encountered)
		while ((RetVal==M4_SUCCESS) && !FinalDeComando(oTokenValue)
					&& !FinalDeBloque(oTokenValue)) {

			// Stores arguments
			if((szArgAux = new m4char_t[strlen(oTokenValue) + 1]) == NULL) {
				return M4_ERROR;
			}

			strcpy(szArgAux, oTokenValue);
			oColaArgumentos.push_back(szArgAux);

			// Checks if it's an unknown item, removing invalid candidate sentences
			if (oTokenId==0)
				RetVal = m_pClCandidateSentences.AddToken("UK");
		
			// Removes invalid candidate sentences
			else
				RetVal = m_pClCandidateSentences.AddToken(oTokenValue);

			oTokenId = m_pClLexAnalyzer.GetWord(&oTokenValue);
		}

		// Finishes candidate sentences analysing
		oAuxTable = m_pClCandidateSentences.EndSentence();
//		m_pClCandidateSentences.ImprimeSentenciasCandidatas();

		// Checks wether there's only one sentence left
		if (oAuxTable.size() != 1)
			RetVal = M4_ERROR;

		// If so, stores sentence in sentence table
		else {
			poComando = m_poTokenCommandTable -> Get((*(oAuxTable.begin())).first);
			oSenTStAux.m_oPointer = poComando;
			oSenTStAux.m_oArguments = oColaArgumentos;
			aio_pSentenceTable -> push_back(oSenTStAux);
		}

		if (RetVal == M4_ERROR)
			QueueClear(&oColaArgumentos);
		else
			oColaArgumentos.clear();

		if (!FinalDeBloque(oTokenValue))
			oTokenId = m_pClLexAnalyzer.GetWord(&oTokenValue);
	}

	return RetVal;

  //## end ClParser::Parse%882380972.body
}

m4bool_t ClParser::FinalDeComando (m4token_t a_oToken)
{
  //## begin ClParser::FinalDeComando%882438507.body preserve=yes
	return M4_BOOL( !strcmp(a_oToken, ";") );
  //## end ClParser::FinalDeComando%882438507.body
}

m4bool_t ClParser::FinalDeBloque (m4token_t a_oToken)
{
  //## begin ClParser::FinalDeBloque%882438508.body preserve=yes
	return M4_BOOL( !strcmp(a_oToken, "") );
  //## end ClParser::FinalDeBloque%882438508.body
}

m4return_t ClParser::SentenceClear (ClSentenceTable *aio_pSentenceTable)
{
  //## begin ClParser::SentenceClear%884684061.body preserve=yes
	if (NULL == aio_pSentenceTable)
	{
		return M4_ERROR;
	}

	//
	// for each command in the sentence table, delete its arguments.	
	ClSentenceTable::iterator oSentenceTableIterator = aio_pSentenceTable->begin();
	while (oSentenceTableIterator != aio_pSentenceTable->end()) 
	{
		QueueClear (&((*oSentenceTableIterator).m_oArguments));
		
		oSentenceTableIterator++;
	}

	aio_pSentenceTable->clear();
	return M4_SUCCESS;
  //## end ClParser::SentenceClear%884684061.body
}

m4void_t ClParser::QueueClear (m4parsercommandargs_t *aio_oComArg)
{
  //## begin ClParser::QueueClear%884781524.body preserve=yes

	m4parsercommandargs_t::iterator oPCAIterator;

	oPCAIterator = (*aio_oComArg).begin();
	while (oPCAIterator != (*aio_oComArg).end()) {
		if ((*oPCAIterator) != NULL)
			delete  (*oPCAIterator);
		oPCAIterator++;
	}
	(*aio_oComArg).clear();


  //## end ClParser::QueueClear%884781524.body
}

// Class ClSenTStruct 



// Additional Declarations
  //## begin ClSenTStruct%34BA5940034A.declarations preserve=yes
  //## end ClSenTStruct%34BA5940034A.declarations

//## begin module%3498103002A9.epilog preserve=yes
//## end module%3498103002A9.epilog
