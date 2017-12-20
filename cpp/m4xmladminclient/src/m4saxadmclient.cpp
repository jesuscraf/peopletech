//## begin module%39085E010338.cm preserve=no
//## end module%39085E010338.cm

//## begin module%39085E010338.cp preserve=no
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
//## end module%39085E010338.cp

//## Module: M4SAXAdmClient%39085E010338; Package body
//## Subsystem: M4XMLAdminClient::src%39084D03030B
//## Source file: E:\INTEGRATION\m4xmladminclient\src\m4saxadmclient.cpp

//## begin module%39085E010338.additionalIncludes preserve=no
//## end module%39085E010338.additionalIncludes

//## begin module%39085E010338.includes preserve=yes

#include <m4commondeclar.hpp>

//## end module%39085E010338.includes

// m4xmlbase
#include <m4xmlbase.hpp>
// M4SAXAdmClient
#include <m4saxadmclient.hpp>
//## begin module%39085E010338.declarations preserve=no
//## end module%39085E010338.declarations

//## begin module%39085E010338.additionalDeclarations preserve=yes

#include <xercesc/sax/AttributeList.hpp>

#include <m4log.hpp>
#include "m4unicode.hpp"
#include "m4cryptc.hpp"

m4return_t HEXtoASCII ( m4pcchar_t ai_szHEX , m4int_t ai_iLen , m4pchar_t ao_szASCII , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut ) ;
//## end module%39085E010338.additionalDeclarations


// Class M4SAXAdmClient 


M4SAXAdmClient::M4SAXAdmClient ()
  //## begin M4SAXAdmClient::M4SAXAdmClient%956910565.hasinit preserve=no
  //## end M4SAXAdmClient::M4SAXAdmClient%956910565.hasinit
  //## begin M4SAXAdmClient::M4SAXAdmClient%956910565.initialization preserve=yes
  //## end M4SAXAdmClient::M4SAXAdmClient%956910565.initialization
{
  //## begin M4SAXAdmClient::M4SAXAdmClient%956910565.body preserve=yes

	m_oOutStream.clear();

	RegisterElementProc ( "command" , (ElementProcStart)&M4SAXAdmClient::ProcCommand );
	RegisterElementProc ( "configuration" , (ElementProcStart)&M4SAXAdmClient::ProcConfiguration );
	RegisterElementProc ( "params" , (ElementProcStart)&M4SAXAdmClient::ProcParams );

	
  //## end M4SAXAdmClient::M4SAXAdmClient%956910565.body
}


M4SAXAdmClient::~M4SAXAdmClient ()
{
  //## begin M4SAXAdmClient::~M4SAXAdmClient%956910566.body preserve=yes
  //## end M4SAXAdmClient::~M4SAXAdmClient%956910566.body
}



//## Other Operations (implementation)
void M4SAXAdmClient::ProcCommand (const XMLCh* const ai_oName, AttributeList* ai_oAttributes)
{
  //## begin M4SAXAdmClient::ProcCommand%956910560.body preserve=yes

	m4uint32_t len ;
	
	if (! ( len = ai_oAttributes -> getLength() ) )
	{
		return ;
	}

	// UNICODE XML
	ProcAttValue((XMLCh *)L"name", ai_oAttributes->getValue("name"));
  //## end M4SAXAdmClient::ProcCommand%956910560.body
}

void M4SAXAdmClient::ProcConfiguration (const XMLCh* const ai_oName, AttributeList* ai_oAttributes)
{
  //## begin M4SAXAdmClient::ProcConfiguration%956910561.body preserve=yes

	m4uint32_t len ;

	
	if (! ( len = ai_oAttributes -> getLength() ) )
	{
		return;
	}

	// UNICODE XML
	ProcAttValue( (XMLCh *)L"host",  ai_oAttributes->getValue( "host" ) ) ;
	ProcAttValue( (XMLCh *)L"servername",  ai_oAttributes->getValue( "servername" ) ) ;
	ProcAttValue( (XMLCh *)L"baseport",  ai_oAttributes->getValue( "baseport" ) ) ;
	ProcAttValue( (XMLCh *)L"oltpload",  ai_oAttributes->getValue( "oltpload" ) ) ;
	ProcAttValue( (XMLCh *)L"xmlload",  ai_oAttributes->getValue( "xmlload" ) ) ;
	ProcAttValue( (XMLCh *)L"cpu",  ai_oAttributes->getValue( "cpu" ) ) ;
	ProcAttValue( (XMLCh *)L"system_memory",  ai_oAttributes->getValue( "system_memory" ) ) ;
	ProcAttValue( (XMLCh *)L"sessions",  ai_oAttributes->getValue( "sessions" ) ) ;
	ProcAttValue( (XMLCh *)L"status",  ai_oAttributes->getValue( "status" ) ) ;
	ProcAttValue( (XMLCh *)L"discarded",  ai_oAttributes->getValue( "discarded" ) ) ;
	ProcAttValue( (XMLCh *)L"oltptime",  ai_oAttributes->getValue( "oltptime" ) ) ;
	ProcAttValue( (XMLCh *)L"xmltime",  ai_oAttributes->getValue( "xmltime" ) ) ;


  //## end M4SAXAdmClient::ProcConfiguration%956910561.body
}

void M4SAXAdmClient::ProcAttValue (const XMLCh *ai_szName, const XMLCh *ai_szValue)
{
  //## begin M4SAXAdmClient::ProcAttValue%956910564.body preserve=yes
	m4pchar_t pcASCIILog = NULL;	
	m4int_t iASCIISize ;

	// UNICODE XML
	string sName;
	string sValue;

	M4XMLToSTL( (M4XMLCh*)ai_szName, sName ) ;
	M4XMLToSTL( (M4XMLCh*)ai_szValue, sValue ) ;

	if ("log" == sName.c_str())
	{
		if (0 < sValue.size()) 
		{
			m4pchar_t szLog = new m4char_t[sValue.size() + 1];
			
			strcpy(szLog, sValue.c_str());

			//Conversion from hexadecimal log string to binary log.

			iASCIISize = strlen ( szLog ) + 1 ;

			pcASCIILog = new m4char_t [ iASCIISize ] ;

			M4_ASSERT ( pcASCIILog != NULL ) ;

			m4return_t iRet = HEXtoASCII ( szLog , strlen( szLog ) , pcASCIILog , iASCIISize , iASCIISize ) ;
			
			if ( iRet == M4_SUCCESS )
			{
				SETSERIALCODESTRING ( pcASCIILog ) ;
			}
			
			delete pcASCIILog ;
			delete szLog;
		}
	}
	else
	{
		// Bug 0132623. Se evita el volcar valores propiedades cuyos valores estén vacios.
		if (0 < sValue.size()) 
		{
			m_oOutStream << sValue.c_str() << M4StreamSeparator;
		}
	}

  //## end M4SAXAdmClient::ProcAttValue%956910564.body
}

m4return_t M4SAXAdmClient::GetOutStream (char *aio_strBuffer, long ai_lBufferSize)
{
  //## begin M4SAXAdmClient::GetOutStream%960193451.body preserve=yes

	m4pchar_t szTerminatorPos = NULL;


	if (ai_lBufferSize > 1 && ai_lBufferSize > m_oOutStream.pcount() && (aio_strBuffer))
	{
		if (0 != m_oOutStream.str())
		{

			(void) strncpy(aio_strBuffer, m_oOutStream.str(), (m_oOutStream.pcount()));

			// Insert the character '\0' to end the string !!!
			aio_strBuffer[m_oOutStream.pcount()] = '\0';

			// Search the last separator character
			
			szTerminatorPos = strrchr(aio_strBuffer, M4StreamSeparator);

			// Change the last separator to the terminator character

			if (NULL != szTerminatorPos)
			{
				*szTerminatorPos = M4StreamTerminator;
			}
			else
			{
				return M4_ERROR;
			}

			return M4_SUCCESS;
		}
		else
		{
			return M4_ERROR;
		}
	}
	else
	{
		return M4_ERROR;
	}

  //## end M4SAXAdmClient::GetOutStream%960193451.body
}

long M4SAXAdmClient::GetOutStreamSize ()
{
  //## begin M4SAXAdmClient::GetOutStreamSize%961674443.body preserve=yes

	if (0 < m_oOutStream.pcount())
	{
		return (m_oOutStream.pcount() + 1);
	}
	else
	{
		return 0;
	}

  //## end M4SAXAdmClient::GetOutStreamSize%961674443.body
}

void M4SAXAdmClient::ProcParams (const XMLCh* const ai_oName, AttributeList* ai_oAttributes)
{
  //## begin M4SAXAdmClient::ProcParams%1003250411.body preserve=yes

	m4uint32_t len ;

	
	if (! ( len = ai_oAttributes -> getLength() ) )
	{
		return;
	}


	// Status parameter (the status of a Server controlled by the Dispatcher)
	// UNICODE XML
	ProcAttValue( (XMLCh *)L"status",  ai_oAttributes->getValue( "status" ) ) ;

	// Result parameter (the result of a command executed by the Dispatcher)
	// UNICODE XML
	ProcAttValue( (XMLCh *)L"result",  ai_oAttributes->getValue( "result" ) ) ;

	// Log stream parameter (a log trace that accomplish the a Dispatcher command result)	
	// UNICODE XML
	ProcAttValue( (XMLCh *)L"log",  ai_oAttributes->getValue( "log" ) ) ;


  //## end M4SAXAdmClient::ProcParams%1003250411.body
}

// Additional Declarations
  //## begin M4SAXAdmClient%39085A93030D.declarations preserve=yes
  //## end M4SAXAdmClient%39085A93030D.declarations

//## begin module%39085E010338.epilog preserve=yes
//## end module%39085E010338.epilog

