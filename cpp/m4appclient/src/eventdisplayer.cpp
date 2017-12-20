//## begin module%379856DB0023.cm preserve=no
//## end module%379856DB0023.cm

//## begin module%379856DB0023.cp preserve=no
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
//## end module%379856DB0023.cp

//## Module: eventdisplayer%379856DB0023; Package body
//## Subsystem: M4AppClient::src%3796F8B303AD
//## Source file: e:\source\m4appclient\src\eventdisplayer.cpp

//## begin module%379856DB0023.additionalIncludes preserve=no
//## end module%379856DB0023.additionalIncludes

//## begin module%379856DB0023.includes preserve=yes
//## end module%379856DB0023.includes

// eventdisplayer
#include <eventdisplayer.hpp>
// m4thread
#include <m4thread.hpp>
// clientbasicdef
#include <clientbasicdef.hpp>
//## begin module%379856DB0023.declarations preserve=no
//## end module%379856DB0023.declarations

//## begin module%379856DB0023.additionalDeclarations preserve=yes
//## end module%379856DB0023.additionalDeclarations


// Class ClServerEventDisplayer 




ClServerEventDisplayer::ClServerEventDisplayer (ClServerEventFunction ai_pServerEventFunction, m4uint32_t ai_iEventType, void *ai_pParam)
  //## begin ClServerEventDisplayer::ClServerEventDisplayer%930306996.hasinit preserve=no
      : m_iEventType(ai_iEventType), m_pParam(ai_pParam)
  //## end ClServerEventDisplayer::ClServerEventDisplayer%930306996.hasinit
  //## begin ClServerEventDisplayer::ClServerEventDisplayer%930306996.initialization preserve=yes
  , m_pServerEventFunction( ai_pServerEventFunction )
  //## end ClServerEventDisplayer::ClServerEventDisplayer%930306996.initialization
{
  //## begin ClServerEventDisplayer::ClServerEventDisplayer%930306996.body preserve=yes
	if(ai_pParam)
	{
		m4pchar_t buffer=new m4char_t [strlen((char *)ai_pParam) +1];
		strcpy(buffer,(char *)ai_pParam);
		m_pParam=(void *)buffer;

	}

// bg 114359
#ifndef _UNIX
	m_uiLCID = GetThreadLocale() ;
#endif

  //## end ClServerEventDisplayer::ClServerEventDisplayer%930306996.body
}


ClServerEventDisplayer::~ClServerEventDisplayer ()
{
  //## begin ClServerEventDisplayer::~ClServerEventDisplayer%941097608.body preserve=yes
	if(m_pParam)
		delete (m4pchar_t)m_pParam;
  //## end ClServerEventDisplayer::~ClServerEventDisplayer%941097608.body
}



//## Other Operations (implementation)
void ClServerEventDisplayer::Run (void )
{
  //## begin ClServerEventDisplayer::Run%930306997.body preserve=yes
	m4bool_t bRet = M4_FALSE ;

// bg 114359
#ifndef _UNIX
	bRet = SetThreadLocale( m_uiLCID ) ;
#endif

	if ( m_pServerEventFunction )
	{
		(*m_pServerEventFunction) ( m_iEventType , m_pParam) ;
	}
	
	Terminate (M4_TRUE) ; ;
  //## end ClServerEventDisplayer::Run%930306997.body
}

// Additional Declarations
  //## begin ClServerEventDisplayer%377356290170.declarations preserve=yes
  //## end ClServerEventDisplayer%377356290170.declarations

//## begin module%379856DB0023.epilog preserve=yes
//## end module%379856DB0023.epilog
