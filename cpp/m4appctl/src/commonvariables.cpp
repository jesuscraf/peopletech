//## begin module%37EB9032011E.cm preserve=no
//## end module%37EB9032011E.cm

//## begin module%37EB9032011E.cp preserve=no
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
//## end module%37EB9032011E.cp

//## Module: CommonVariables%37EB9032011E; Package body
//## Subsystem: M4Appctl::src%37EA676703E6
//## Source file: F:\integration\M4Appctl\src\commonvariables.cpp

//## begin module%37EB9032011E.additionalIncludes preserve=no
//## end module%37EB9032011E.additionalIncludes

//## begin module%37EB9032011E.includes preserve=yes
#include <m4ini.hpp>

#include "m4memdbg.h"

#define M4_SRV_TRACE_COMP_ID "APPCTL"


//## end module%37EB9032011E.includes

// syncro
#include <syncro.hpp>
// m4condition
#include <m4condition.hpp>
// eStatus
#include <estatus.hpp>
// CommonVariables
#include <commonvariables.hpp>
//## begin module%37EB9032011E.declarations preserve=no
//## end module%37EB9032011E.declarations

//## begin module%37EB9032011E.additionalDeclarations preserve=yes
//## end module%37EB9032011E.additionalDeclarations


// Class ClCommonVariables 











ClCommonVariables::ClCommonVariables ()
  //## begin ClCommonVariables::ClCommonVariables%938158802.hasinit preserve=no
      : m_bStop_Demon(M4_FALSE), m_ulNumOfAdminConnections(0)
  //## end ClCommonVariables::ClCommonVariables%938158802.hasinit
  //## begin ClCommonVariables::ClCommonVariables%938158802.initialization preserve=yes
  //## end ClCommonVariables::ClCommonVariables%938158802.initialization
{
  //## begin ClCommonVariables::ClCommonVariables%938158802.body preserve=yes
//	m_pcControl_Port = ai_pcControl_Port;
//	m_pcAdmin_Port = ai_pcAdmin_Port;
	M4_MDBG_NEW(m_poMutex, m_poMutex = new ClMutex);
	m_poMutex->Init();
	M4_MDBG_NEW(m_poCondition, m_poCondition = new ClCondition);
	m_poCondition->Init();
	M4_MDBG_NEW(m_poControlCondConnection, m_poControlCondConnection = new ClCondition);
	m_poControlCondConnection->Init();
	M4_MDBG_NEW(m_poAdminCondConnection, m_poAdminCondConnection = new ClCondition);
	m_poAdminCondConnection->Init();

	m_strHost = "" ;
  //## end ClCommonVariables::ClCommonVariables%938158802.body
}


ClCommonVariables::~ClCommonVariables ()
{
  //## begin ClCommonVariables::~ClCommonVariables%941022967.body preserve=yes
	M4_MDBG_DELETE( m_poMutex, delete m_poMutex ); m_poMutex=0x0 ;
	M4_MDBG_DELETE( m_poCondition, delete m_poCondition); m_poCondition=0x0 ;
	M4_MDBG_DELETE( m_poControlCondConnection, delete m_poControlCondConnection);
			m_poControlCondConnection=0x0 ;
	M4_MDBG_DELETE( m_poAdminCondConnection, delete m_poAdminCondConnection);
		m_poAdminCondConnection=0x0 ;

  //## end ClCommonVariables::~ClCommonVariables%941022967.body
}



//## Other Operations (implementation)
void ClCommonVariables::SetHost (m4pchar_t ai_szHost)
{
  //## begin ClCommonVariables::SetHost%982239795.body preserve=yes
	m_strHost = new m4char_t [strlen ( ai_szHost) +1 ] ;
	strcpy ( m_strHost , ai_szHost ) ;
  //## end ClCommonVariables::SetHost%982239795.body
}

m4pchar_t ClCommonVariables::GetHost ()
{
  //## begin ClCommonVariables::GetHost%982239796.body preserve=yes
	return m_strHost ;
  //## end ClCommonVariables::GetHost%982239796.body
}

// Additional Declarations
  //## begin ClCommonVariables%37EB8D010123.declarations preserve=yes
  //## end ClCommonVariables%37EB8D010123.declarations

// Class ClProcessNameLess 


//## Other Operations (implementation)
m4bool_t ClProcessNameLess::operator () (const m4pchar_t pcProcessNameA, const m4pchar_t pcProcessNameB) const
{
  //## begin ClProcessNameLess::operator()%939052189.body preserve=yes
	return strcmp(pcProcessNameA,pcProcessNameB)<0 ? M4_TRUE:M4_FALSE;
  //## end ClProcessNameLess::operator()%939052189.body
}

// Additional Declarations
  //## begin ClProcessNameLess%37F8C95C0110.declarations preserve=yes
  //## end ClProcessNameLess%37F8C95C0110.declarations

// Class ClTSAPIdLess 


//## Other Operations (implementation)
m4bool_t ClTSAPIdLess::operator () (const m4uint32_t TSAPIdA, const m4uint32_t TSAPIdB) const
{
  //## begin ClTSAPIdLess::operator()%939052198.body preserve=yes
	if ( TSAPIdA < TSAPIdB ) return M4_TRUE;

	return M4_FALSE;

  //## end ClTSAPIdLess::operator()%939052198.body
}

// Additional Declarations
  //## begin ClTSAPIdLess%37FB6728034F.declarations preserve=yes
  //## end ClTSAPIdLess%37FB6728034F.declarations

//## begin module%37EB9032011E.epilog preserve=yes
//## end module%37EB9032011E.epilog
