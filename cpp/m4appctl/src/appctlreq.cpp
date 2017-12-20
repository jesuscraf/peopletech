//## begin module%3A801D8A020A.cm preserve=no
//## end module%3A801D8A020A.cm

//## begin module%3A801D8A020A.cp preserve=no
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
//## end module%3A801D8A020A.cp

//## Module: appctlreq%3A801D8A020A; Package body
//## Subsystem: M4Appctl::src%37EA676703E6
//## Source file: F:\integration\M4Appctl\src\appctlreq.cpp

//## begin module%3A801D8A020A.additionalIncludes preserve=no
//## end module%3A801D8A020A.additionalIncludes

//## begin module%3A801D8A020A.includes preserve=yes
#include <xmlstreamgenerator.hpp>
#include <m4dpchres.hpp>

#include <m4memdbg.h>
#include <m4srvtrace.h>

//## end module%3A801D8A020A.includes

// logsys
#include <logsys.hpp>
// m4types
#include <m4types.hpp>
// appctlreq
#include <appctlreq.hpp>
//## begin module%3A801D8A020A.declarations preserve=no
//## end module%3A801D8A020A.declarations

//## begin module%3A801D8A020A.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID "APPCTL"
#define strLogFilled		3
//## end module%3A801D8A020A.additionalDeclarations


// Class ClAppCtlRequest 







ClAppCtlRequest::ClAppCtlRequest (m4pchar_t ai_szBuffer, m4uint32_t ai_iConnectionID)
  //## begin ClAppCtlRequest::ClAppCtlRequest%981474748.hasinit preserve=no
  //## end ClAppCtlRequest::ClAppCtlRequest%981474748.hasinit
  //## begin ClAppCtlRequest::ClAppCtlRequest%981474748.initialization preserve=yes
  //## end ClAppCtlRequest::ClAppCtlRequest%981474748.initialization
{
  //## begin ClAppCtlRequest::ClAppCtlRequest%981474748.body preserve=yes
	m_szBufferIn = ai_szBuffer ;
	m_szBufferOut = NULL ;
	m_iConnectionID = ai_iConnectionID ;
	m_iRequestID = 0 ;
	m_iResult = M4_ERROR ;
	m_poLog = NULL;
	M4_MDBG_DELETE( m_poLog , m_poLog = new ClLogSystem (GETLOGMANAGER () , M4_TRUE , M4_TRUE )) ;
  //## end ClAppCtlRequest::ClAppCtlRequest%981474748.body
}


ClAppCtlRequest::~ClAppCtlRequest ()
{
  //## begin ClAppCtlRequest::~ClAppCtlRequest%981474749.body preserve=yes
	M4_MDBG_DELETE( m_szBufferOut, delete m_szBufferOut ) ;
	M4_MDBG_DELETE( m_szBufferIn, delete m_szBufferIn ) ;
	M4_MDBG_DELETE( m_poLog, delete m_poLog ) ;
  //## end ClAppCtlRequest::~ClAppCtlRequest%981474749.body
}



//## Other Operations (implementation)
m4pchar_t ClAppCtlRequest::GetBufferOut ()
{
  //## begin ClAppCtlRequest::GetBufferOut%981474750.body preserve=yes
	return m_szBufferOut ;
  //## end ClAppCtlRequest::GetBufferOut%981474750.body
}

m4pchar_t ClAppCtlRequest::GenerateAnwer ()
{
  //## begin ClAppCtlRequest::GenerateAnwer%981474751.body preserve=yes
	M4XMLStreamGenerator oXMLGenerator ;
	m4string_t szRet = "SUCCESS" ;
	m4char_t szRequestID [50];
	m4pchar_t pLogBuffer ;
	m4return_t iRet ;
	m4uint32_t  iStreamSize , iLenout , iCount;

	
	
	oXMLGenerator.InsertComment("@version");

	oXMLGenerator.MakeElement("m4appctl"); 
	oXMLGenerator.EndElement() ; //element=dispatcher

	oXMLGenerator.MakeElement("command") ;
	oXMLGenerator.AddElementAttribute ( "name" , "M4AppctlReply"); 
	oXMLGenerator.EndElement() ; //element=command

	if ( M4_SUCCESS != m_iResult )
	{
		szRet = "ERROR" ;
	}

	sprintf ( szRequestID , "%d" , m_iRequestID ) ;

	oXMLGenerator.MakeElement("params");
	oXMLGenerator.AddElementAttribute ( "Result" , szRet );
	oXMLGenerator.AddElementAttribute ( "RequestID" , szRequestID );

	if( m_poLog )
	{

		iRet = m_poLog -> GetSerialStringSizeFromMessage ( iStreamSize );


		if ( ( iStreamSize> 0 ) & (iRet != M4_ERROR ) )
		{
			// La log está vacía.
	
			pLogBuffer = new m4char_t [ iStreamSize +1];
			

			M4_ASSERT ( pLogBuffer != NULL );

			pLogBuffer[0] = 0 ;

			iRet = m_poLog -> GetSerialStringFromMessage ( pLogBuffer , iStreamSize , iLenout ) ;
//			iRet = m_poLog -> toString(pLogBuffer, iStreamSize);

			if ( iRet != M4_ERROR ) 
			{
				for ( iCount = 0 ;iCount <iStreamSize; iCount ++ )
				{
					if (!  pLogBuffer[iCount] )
					{
						pLogBuffer[iCount] = strLogFilled ;
					}
				}

				pLogBuffer[iStreamSize] = 0 ; 
				oXMLGenerator.AddEscapedElementAttribute ( "log" , (m4pcchar_t) pLogBuffer );
			}
			
			delete []pLogBuffer;
		}
	}

	oXMLGenerator.CloseSingleClosedElement(); //element=status

	oXMLGenerator.CloseElement("command");

	oXMLGenerator.CloseElement("m4appctl");

	iStreamSize = oXMLGenerator.GetXMLStreamSize ( ) ;

	iStreamSize++ ;

	//M4_MDBG_NEW( m_szBufferOut , m_szBufferOut  = new m4char_t [iStreamSize] ) ;

    iRet = oXMLGenerator.GetXMLStream (m_szBufferOut , iStreamSize );


	return m_szBufferOut ;
  //## end ClAppCtlRequest::GenerateAnwer%981474751.body
}

void ClAppCtlRequest::SetResult (m4return_t ai_iResult)
{
  //## begin ClAppCtlRequest::SetResult%981474752.body preserve=yes
	m_iResult = ai_iResult ;
  //## end ClAppCtlRequest::SetResult%981474752.body
}

m4pchar_t ClAppCtlRequest::GetBufferIn ()
{
  //## begin ClAppCtlRequest::GetBufferIn%981474753.body preserve=yes
	return m_szBufferIn ;
  //## end ClAppCtlRequest::GetBufferIn%981474753.body
}

void ClAppCtlRequest::SetRequestID (m4uint32_t ai_iRequestID)
{
  //## begin ClAppCtlRequest::SetRequestID%981474755.body preserve=yes
	m_iRequestID = ai_iRequestID ;
  //## end ClAppCtlRequest::SetRequestID%981474755.body
}

ClLogSystem * ClAppCtlRequest::GetLog ()
{
  //## begin ClAppCtlRequest::GetLog%982066311.body preserve=yes
	return m_poLog ;
  //## end ClAppCtlRequest::GetLog%982066311.body
}

m4uint32_t ClAppCtlRequest::GetConnectionID ()
{
  //## begin ClAppCtlRequest::GetConnectionID%998490241.body preserve=yes
	return m_iConnectionID ;
  //## end ClAppCtlRequest::GetConnectionID%998490241.body
}

// Additional Declarations
  //## begin ClAppCtlRequest%3A801AFD02F1.declarations preserve=yes
m4pchar_t ClAppCtlRequest::GenerateDispatcherUpdate (m4pchar_t ai_szConfigName , m4pchar_t ai_szAppCtlHost , m4uint32_t ai_iAppCtlPort )
{
	M4XMLStreamGenerator oXMLGenerator ;
	m4char_t szRequestID [50];
	m4char_t szPort [20] ;
	m4return_t iRet ;
	m4uint32_t  iStreamSize;

	
	
	oXMLGenerator.InsertComment("@version");

	oXMLGenerator.MakeElement("M4AppctlUpdate"); 
	oXMLGenerator.EndElement() ; //element=dispatcher

	oXMLGenerator.MakeElement("config") ;
	oXMLGenerator.AddElementAttribute ( "name" , "m4appctl"); 
	oXMLGenerator.EndElement() ; //element=command

	sprintf ( szRequestID , "%d" , m_iRequestID ) ;
	sprintf ( szPort , "%d" , ai_iAppCtlPort ) ;

	oXMLGenerator.MakeElement("params");
	oXMLGenerator.AddElementAttribute ( "RequestID" , szRequestID );
	oXMLGenerator.AddElementAttribute ( "configuration" , ai_szConfigName  );
	oXMLGenerator.AddElementAttribute ( "host" , ai_szAppCtlHost );
	oXMLGenerator.AddElementAttribute ( "port" , szPort );


	oXMLGenerator.CloseSingleClosedElement(); //element=status

	oXMLGenerator.CloseElement("config");

	oXMLGenerator.CloseElement("M4AppctlUpdate");

	iStreamSize = oXMLGenerator.GetXMLStreamSize ( ) ;

	iStreamSize++ ;

	M4_MDBG_NEW( m_szBufferOut , m_szBufferOut  = new m4char_t [iStreamSize] ) ;

    iRet = oXMLGenerator.GetXMLStream (m_szBufferIn , iStreamSize );


	return m_szBufferOut ;
}

void ClAppCtlRequest::SetConnectionID ( m4uint32_t iConnectionId )
{
	m_iConnectionID = iConnectionId ;
}

ClAppCtlRequest::ClAppCtlRequest ()
{}

  //## end ClAppCtlRequest%3A801AFD02F1.declarations

//## begin module%3A801D8A020A.epilog preserve=yes
//## end module%3A801D8A020A.epilog
