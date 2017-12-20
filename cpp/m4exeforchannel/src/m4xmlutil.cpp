//## begin module%3856118A00F5.cm preserve=no
//## end module%3856118A00F5.cm

//## begin module%3856118A00F5.cp preserve=no
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
//## end module%3856118A00F5.cp

//## Module: Util_M4XML%3856118A00F5; Package body
//## Subsystem: M4ExeForChannel::XML::src%3826C5A30000
//## Source file: D:\Work\v600\m4exeforchannel\src\m4xmlutil.cpp

//## begin module%3856118A00F5.additionalIncludes preserve=no
//## end module%3856118A00F5.additionalIncludes

//## begin module%3856118A00F5.includes preserve=yes
#include "m4stl.hpp"
#include "clm4objservice.hpp"
#include "m4regimpl.hpp"
#include "dm.hpp"
#include "conn_provider.hpp"
#include "iexecutor.hpp"
#include "exvariables.hpp"
#include "execdumpinfo.hpp"
#include "clexecutorstate.hpp"
//## end module%3856118A00F5.includes

// Util_M4XML
#include <m4xmlutil.hpp>
// m4VariantType
#include <m4var.hpp>
// MetaData_SubSystem
#include <metadatasubsys.hpp>
// Executive_MDClient

class ClM4XMLContext;
class ClExecutorStateManager;
class ClChannel;

//## begin module%3856118A00F5.declarations preserve=no
//## end module%3856118A00F5.declarations

//## begin module%3856118A00F5.additionalDeclarations preserve=yes
//## end module%3856118A00F5.additionalDeclarations


// Class ClCMInitParameters 














ClCMInitParameters::ClCMInitParameters ()
  //## begin ClCMInitParameters::ClCMInitParameters%971105358.hasinit preserve=no
      : m_poStatus(NULL), m_poStateManager(NULL), m_iSessionId(0), 
	  m_poSessionChannel(NULL), m_poExecutorState(NULL), m_poInputDataStorage(NULL), 
	  m_poExecInfo(NULL), m_poMetadataSubsystem(NULL), m_bParseStatus(M4_TRUE)
  //## end ClCMInitParameters::ClCMInitParameters%971105358.hasinit
  //## begin ClCMInitParameters::ClCMInitParameters%971105358.initialization preserve=yes
  //## end ClCMInitParameters::ClCMInitParameters%971105358.initialization
{
  //## begin ClCMInitParameters::ClCMInitParameters%971105358.body preserve=yes
  //## end ClCMInitParameters::ClCMInitParameters%971105358.body
}



//## Other Operations (implementation)
m4return_t ClCMInitParameters::Reset (m4bool_t ai_bOnlyReset, m4bool_t ai_bErase)
{
  //## begin ClCMInitParameters::Reset%971105363.body preserve=yes

	m4return_t iResult = M4_SUCCESS;

	// leaves the logical connection
	if (ai_bOnlyReset == M4_FALSE && GetExecutorState() != NULL &&
		GetExecutorState()->GetChannelManager() != NULL)
	{
		ClM4ObjService *poOS = GetExecutorState()->GetM4ObjService();

		if (poOS != NULL)
		{
			iResult = poOS->GetpConnProvider()->SetConnection(NULL,M4_TRUE);
			poOS->GetpExecutor()->RequestToAbortWithClStatus(M4_FALSE);
			poOS->GetpExecutor()->SetpCancelStatus(NULL);
		}
		else
			iResult = M4_ERROR;
	}

	if (ai_bErase == M4_TRUE)
	{
		SetExecutorState(NULL);
		m_poStatus = NULL;
	}

	return iResult;
  //## end ClCMInitParameters::Reset%971105363.body
}

// Additional Declarations
  //## begin ClCMInitParameters%39E2023C0345.declarations preserve=yes
  //## end ClCMInitParameters%39E2023C0345.declarations

// Class ClM4XMLInfo 

//## begin ClM4XMLInfo::m_bInit%39E2F1F90309.attr preserve=no  private: static m4bool_t {UA} M4_FALSE
m4bool_t ClM4XMLInfo::m_bInit = M4_FALSE;
//## end ClM4XMLInfo::m_bInit%39E2F1F90309.attr


//## begin ClM4XMLInfo::m_oMap%39E2F1F90345.role preserve=no  public: static ExecInfoMap { -> VHAN}
ExecInfoMap ClM4XMLInfo::m_oMap;
//## end ClM4XMLInfo::m_oMap%39E2F1F90345.role


//## Other Operations (implementation)
void ClM4XMLInfo::AddInstanceByThread ()
{
  //## begin ClM4XMLInfo::AddInstanceByThread%971105386.body preserve=yes
	m4iden_t id = M4Thread::GetCurrentIdThread();
	
	/*
	ExecInfoMapIt it = m_oMap.find(id);

	if (it != m_oMap.end())
		m_oMap.erase(it);
	*/

	m_oMap.insert(ExecInfoMap::value_type(id,this));
  //## end ClM4XMLInfo::AddInstanceByThread%971105386.body
}

// Additional Declarations
  //## begin ClM4XMLInfo%39E2F1F902E1.declarations preserve=yes
  //## end ClM4XMLInfo%39E2F1F902E1.declarations

// Class ClM4XMLVariant 


ClM4XMLVariant::ClM4XMLVariant ()
  //## begin ClM4XMLVariant::ClM4XMLVariant%971793327.hasinit preserve=no
      : m_bIsUsed(M4_FALSE)
  //## end ClM4XMLVariant::ClM4XMLVariant%971793327.hasinit
  //## begin ClM4XMLVariant::ClM4XMLVariant%971793327.initialization preserve=yes
  //## end ClM4XMLVariant::ClM4XMLVariant%971793327.initialization
{
  //## begin ClM4XMLVariant::ClM4XMLVariant%971793327.body preserve=yes
  //## end ClM4XMLVariant::ClM4XMLVariant%971793327.body
}


// Additional Declarations
  //## begin ClM4XMLVariant%39EC630800BA.declarations preserve=yes
  //## end ClM4XMLVariant%39EC630800BA.declarations

//## begin module%3856118A00F5.epilog preserve=yes
//## end module%3856118A00F5.epilog
