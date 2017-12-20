//## begin module%3498F0F00284.cm preserve=no
//## end module%3498F0F00284.cm

//## begin module%3498F0F00284.cp preserve=no
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
//## end module%3498F0F00284.cp

//## Module: FileReader%3498F0F00284; Package body
//## Subsystem: M4Parser::src%379D926A02BD
//## Source file: D:\FuentesServidor\M4Parser\version\src\filereader.cpp

//## begin module%3498F0F00284.additionalIncludes preserve=no
//## end module%3498F0F00284.additionalIncludes

//## begin module%3498F0F00284.includes preserve=yes
//## end module%3498F0F00284.includes

// ParserDefs
#include <parserdefs.hpp>
// FileReader
#include <filereader.hpp>
#include "m4unicode.hpp"
//## begin module%3498F0F00284.declarations preserve=no
//## end module%3498F0F00284.declarations

//## begin module%3498F0F00284.additionalDeclarations preserve=yes
//## end module%3498F0F00284.additionalDeclarations


// Class ClFileReader 




ClFileReader::ClFileReader ()
  //## begin ClFileReader::ClFileReader%882438502.hasinit preserve=no
  //## end ClFileReader::ClFileReader%882438502.hasinit
  //## begin ClFileReader::ClFileReader%882438502.initialization preserve=yes
  //## end ClFileReader::ClFileReader%882438502.initialization
{
  //## begin ClFileReader::ClFileReader%882438502.body preserve=yes

	m_szSection = new m4char_t[1];

  //## end ClFileReader::ClFileReader%882438502.body
}


ClFileReader::~ClFileReader ()
{
  //## begin ClFileReader::~ClFileReader%882438503.body preserve=yes
	if (m_File.rdbuf()->is_open())	
		m_File.close();
	delete[] m_szSection;

  //## end ClFileReader::~ClFileReader%882438503.body
}



//## Other Operations (implementation)
m4token_t ClFileReader::GetToken ()
{
  //## begin ClFileReader::GetToken%882438497.body preserve=yes

	// String to store read word
	m4char_t szToken[255];

	// Return variable
	m4token_t RetVal;

	// Pointer to current position in szToken
	m4uchar_t cPosition = 0;

	// Char to read from file
	m4char_t cAux;

	// Skips blanks
	EliminaEspacios();

	// Read word
	m_File.get(cAux);
	while ((cAux != ' ') && (cAux != '\n') && (cAux != '\t')) {
		szToken[cPosition] = cAux;
		m_File.get(cAux);
		cPosition++;
	}
	szToken[cPosition] = '\0';

	// Step backwards for not loosing any character
	m_File.putback(cAux);
/*	m_File.seekg(-1, m_File.tellg());*/

	RetVal= new m4char_t[strlen(szToken)+1];
	strcpy(RetVal, szToken);
	
	return RetVal;
	
  //## end ClFileReader::GetToken%882438497.body
}

m4command_t ClFileReader::GetIdCommand ()
{
  //## begin ClFileReader::GetIdCommand%882438498.body preserve=yes

	return GetToken();
	
  //## end ClFileReader::GetIdCommand%882438498.body
}

m4bool_t ClFileReader::InSection (m4pchar_t a_szSection)
{
  //## begin ClFileReader::InSection%882438499.body preserve=yes

	// char to read from file
	m4char_t cAux;

	// Return variable
	m4bool_t RetVal;

	// Skips blanks
	EliminaEspacios();

	m_File.get(cAux);

	// If it's '[', change section
	if (cAux=='[') {
		delete[] m_szSection;
		m_szSection = GetToken();
		m_szSection[strlen(m_szSection)-1] = '\0';
	}

	// else, step backwards
	else
		m_File.putback(cAux);
/*	m_File.seekg(-1, m_File.tellg());*/

	// Gets RetVal
	RetVal = M4_BOOL( !strcmp(a_szSection, m_szSection) );

	return RetVal;

  //## end ClFileReader::InSection%882438499.body
}

m4return_t ClFileReader::InicFile (m4pchar_t a_szFileName)
{
  //## begin ClFileReader::InicFile%882438500.body preserve=yes

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
    M4Open( m_File, a_szFileName, M4UniRead, eEncoding );
	
	if (m_File.fail())
		return M4_ERROR;
	else
		return M4_SUCCESS;

  //## end ClFileReader::InicFile%882438500.body
}

m4void_t ClFileReader::EliminaEspacios ()
{
  //## begin ClFileReader::EliminaEspacios%882438501.body preserve=yes
	m4char_t cAux;

	m_File.get(cAux);
	while ((cAux == ' ') || (cAux == '\n') || (cAux == '\t'))
		m_File.get(cAux);
	m_File.putback(cAux);
/*	m_File.seekg(-1, m_File.tellg());*/

  //## end ClFileReader::EliminaEspacios%882438501.body
}

m4bool_t ClFileReader::EndLine ()
{
  //## begin ClFileReader::EndLine%882438504.body preserve=yes
	m4char_t cAux = ' ';	// initialize to whitespace
	m4bool_t RetVal;

	// Removes Blanks
	//----m_File.get(cAux);
	//----while (cAux == ' ') 
	while (isspace(cAux) && cAux != '\n')
		m_File.get(cAux);

	m_File.putback(cAux);
/*	m_File.seekg(-1, m_File.tellg());*/
	RetVal = M4_BOOL( cAux=='\n' );

	return RetVal;
  //## end ClFileReader::EndLine%882438504.body
}

m4return_t ClFileReader::CloseFile ()
{
  //## begin ClFileReader::CloseFile%882438506.body preserve=yes
	if (m_File.rdbuf()->is_open())
		m_File.close();
	return M4_SUCCESS;
  //## end ClFileReader::CloseFile%882438506.body
}

// Additional Declarations
  //## begin ClFileReader%3498E90C01C1.declarations preserve=yes
  //## end ClFileReader%3498E90C01C1.declarations

//## begin module%3498F0F00284.epilog preserve=yes
//## end module%3498F0F00284.epilog
