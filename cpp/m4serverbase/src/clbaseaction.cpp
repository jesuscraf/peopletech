//## begin module%3A13FFAA0212.cm preserve=no
//## end module%3A13FFAA0212.cm

//## begin module%3A13FFAA0212.cp preserve=no
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
//## end module%3A13FFAA0212.cp

//## Module: clbaseaction%3A13FFAA0212; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: F:\integration\m4serverbase\src\clbaseaction.cpp

//## begin module%3A13FFAA0212.additionalIncludes preserve=no
//## end module%3A13FFAA0212.additionalIncludes

//## begin module%3A13FFAA0212.includes preserve=yes
#include <m4memdbg.h>
#include <m4srvtrace.h>
#include <basiclog.hpp>
#include <paramlist.hpp>
#include <clbaseaction.hpp>


#define M4_SRV_TRACE_COMP_ID "SRVB"

//## end module%3A13FFAA0212.includes


//## begin module%3A13FFAA0212.declarations preserve=no
//## end module%3A13FFAA0212.declarations

//## begin module%3A13FFAA0212.additionalDeclarations preserve=yes
//## end module%3A13FFAA0212.additionalDeclarations


// Class ClBaseAction 




ClBaseAction::~ClBaseAction ()
{
  //## begin ClBaseAction::~ClBaseAction%956915209.body preserve=yes
	if(m_pParamList && m_bDeleteParamList ) {
		M4_MDBG_DELETE(  m_pParamList, delete m_pParamList ) ;
		m_pParamList=NULL ;
	} ;
  //## end ClBaseAction::~ClBaseAction%956915209.body
}



//## Other Operations (implementation)
m4return_t ClBaseAction::SetParams (ClParamList *ai_poParamList)
{
  //## begin ClBaseAction::SetParams%956825069.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	if(m_pParamList)
	{
		M4_SRV_ERROR_N( 0, "Parametros ya asignados" );
		if ( m_bDeleteParamList ) { 
			M4_MDBG_DELETE(  m_pParamList, delete m_pParamList ) ;
			m_pParamList=NULL;
		} ;
		iRet=M4_WARNING;
	}
	m_pParamList=ai_poParamList;
	return iRet;
  //## end ClBaseAction::SetParams%956825069.body
}

// Additional Declarations
  //## begin ClBaseAction%38E2441302AC.declarations preserve=yes
  //## end ClBaseAction%38E2441302AC.declarations

//## begin module%3A13FFAA0212.epilog preserve=yes
//## end module%3A13FFAA0212.epilog
