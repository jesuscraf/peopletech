//## begin module%3907F8F200D3.cm preserve=no
//## end module%3907F8F200D3.cm

//## begin module%3907F8F200D3.cp preserve=no
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
//## end module%3907F8F200D3.cp

//## Module: appctlsaxhandler%3907F8F200D3; Package body
//## Subsystem: M4Appctl::src%37EA676703E6
//## Source file: F:\integration\M4Appctl\src\appctlsaxhandler.cpp

//## begin module%3907F8F200D3.additionalIncludes preserve=no
//## end module%3907F8F200D3.additionalIncludes

//## begin module%3907F8F200D3.includes preserve=yes
#include "m4memdbg.h"
#define  M4_MEM_DBG_COMP_ID 2
//## end module%3907F8F200D3.includes

// appctlsaxhandler
#include <appctlsaxhandler.hpp>
// m4xmlbase
#include <m4xmlbase.hpp>
#include "m4unicode.hpp"
//## begin module%3907F8F200D3.declarations preserve=no
//## end module%3907F8F200D3.declarations

//## begin module%3907F8F200D3.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID "APPCTL"

//## end module%3907F8F200D3.additionalDeclarations


// Class ClAppCtlSAXHandler 





ClAppCtlSAXHandler::ClAppCtlSAXHandler ()
  //## begin ClAppCtlSAXHandler::ClAppCtlSAXHandler%957775211.hasinit preserve=no
      : m_szPathLauncher(NULL), m_szServerName(NULL)
  //## end ClAppCtlSAXHandler::ClAppCtlSAXHandler%957775211.hasinit
  //## begin ClAppCtlSAXHandler::ClAppCtlSAXHandler%957775211.initialization preserve=yes
  //## end ClAppCtlSAXHandler::ClAppCtlSAXHandler%957775211.initialization
{
  //## begin ClAppCtlSAXHandler::ClAppCtlSAXHandler%957775211.body preserve=yes
	RegisterElementProc ( "launcher" , (ElementProcStart)&ClAppCtlSAXHandler::procLauncher );
	RegisterElementProc ( "params" , (ElementProcStart)&ClAppCtlSAXHandler::procServer );
	RegisterElementProc ( "command" , (ElementProcStart)&ClAppCtlSAXHandler::procCommand );
	
  //## end ClAppCtlSAXHandler::ClAppCtlSAXHandler%957775211.body
}


ClAppCtlSAXHandler::~ClAppCtlSAXHandler ()
{
  //## begin ClAppCtlSAXHandler::~ClAppCtlSAXHandler%957775212.body preserve=yes
  //## end ClAppCtlSAXHandler::~ClAppCtlSAXHandler%957775212.body
}



//## Other Operations (implementation)
void ClAppCtlSAXHandler::procLauncher (const XMLCh* const name, AttributeList *attributes)
{
  //## begin ClAppCtlSAXHandler::procLauncher%956823442.body preserve=yes

	// UNICODE XML
	string sBin;
	string sPath;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue( "bin" ), sBin ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue( "path" ), sPath ) ;

	M4_MDBG_NEW(m_szPathLauncher,m_szPathLauncher = new m4char_t [strlen ( sBin.c_str() ) + strlen ( sPath.c_str() ) +2] ) ;

	strcpy ( m_szPathLauncher  , sBin.c_str() ) ;
	strcat ( m_szPathLauncher  , "/" ) ;
	strcat ( m_szPathLauncher  , sBin.c_str() ) ;
  //## end ClAppCtlSAXHandler::procLauncher%956823442.body
}

m4pchar_t ClAppCtlSAXHandler::GetLauncherPath ()
{
  //## begin ClAppCtlSAXHandler::GetLauncherPath%957775213.body preserve=yes
	return m_szPathLauncher ;
  //## end ClAppCtlSAXHandler::GetLauncherPath%957775213.body
}

void ClAppCtlSAXHandler::procServer (const XMLCh* const name, AttributeList *attributes)
{
  //## begin ClAppCtlSAXHandler::procServer%960832019.body preserve=yes
	m4char_t szRequestID[64]; 

	// UNICODE XML
	string sServerName;
	string sPath;
	string sRequestID;

	M4XMLToSTL( (M4XMLCh*)attributes->getValue( "servername" ), sServerName ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue( "path" ), sPath ) ;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue( "RequestID" ), sRequestID ) ;


	m_szServerName = new m4char_t [strlen ( sServerName.c_str() ) + strlen ( sPath.c_str() ) +2] ;

	strcpy ( m_szServerName, sServerName.c_str() ) ;

	strcpy ( szRequestID , sRequestID.c_str() ) ;

	sscanf ( szRequestID , "%d" , &m_iRequestID ) ;
  //## end ClAppCtlSAXHandler::procServer%960832019.body
}

m4pchar_t ClAppCtlSAXHandler::GetServerName ()
{
  //## begin ClAppCtlSAXHandler::GetServerName%960883629.body preserve=yes
	return m_szServerName ;
  //## end ClAppCtlSAXHandler::GetServerName%960883629.body
}

m4uint32_t ClAppCtlSAXHandler::GetRequestID ()
{
  //## begin ClAppCtlSAXHandler::GetRequestID%981474754.body preserve=yes
	return m_iRequestID ;
  //## end ClAppCtlSAXHandler::GetRequestID%981474754.body
}

void ClAppCtlSAXHandler::procCommand (const XMLCh* const name, AttributeList *attributes)
{
  //## begin ClAppCtlSAXHandler::procCommand%998555465.body preserve=yes

	// UNICODE XML
	string sId;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue( "id" ), sId ) ;

	m_szCommand = sId ;
  //## end ClAppCtlSAXHandler::procCommand%998555465.body
}

m4string_t ClAppCtlSAXHandler::GetCommand ()
{
  //## begin ClAppCtlSAXHandler::GetCommand%998555467.body preserve=yes
	return m_szCommand ;
  //## end ClAppCtlSAXHandler::GetCommand%998555467.body
}

// Additional Declarations
  //## begin ClAppCtlSAXHandler%3907F3E0004A.declarations preserve=yes
  //## end ClAppCtlSAXHandler%3907F3E0004A.declarations

//## begin module%3907F8F200D3.epilog preserve=yes
//## end module%3907F8F200D3.epilog
