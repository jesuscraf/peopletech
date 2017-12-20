//## begin module%3A093D0702B8.cm preserve=no
//## end module%3A093D0702B8.cm

//## begin module%3A093D0702B8.cp preserve=no
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
//## end module%3A093D0702B8.cp

//## Module: dspreqcontainer%3A093D0702B8; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: C:\WorkingFolder\m4dispatcher\src\dspreqcontainer.cpp

//## begin module%3A093D0702B8.additionalIncludes preserve=no
//## end module%3A093D0702B8.additionalIncludes

//## begin module%3A093D0702B8.includes preserve=yes
//## end module%3A093D0702B8.includes

// dspreq
#include <dspreq.hpp>
// CriticalSection
#include <criticalsection.hpp>
// dspreqcontainer
#include <dspreqcontainer.hpp>
//## begin module%3A093D0702B8.declarations preserve=no
//## end module%3A093D0702B8.declarations

//## begin module%3A093D0702B8.additionalDeclarations preserve=yes
#include <blocksynchronization.hpp>
#include <basiclog.hpp>
#include <m4dpchres.hpp>
#include <m4srvtrace.h>

#define M4_SRV_TRACE_COMP_ID "DPCH"

//## end module%3A093D0702B8.additionalDeclarations


// Class DspReqContainer 




DspReqContainer::DspReqContainer ()
  //## begin DspReqContainer::DspReqContainer%963906299.hasinit preserve=no
  //## end DspReqContainer::DspReqContainer%963906299.hasinit
  //## begin DspReqContainer::DspReqContainer%963906299.initialization preserve=yes
  //## end DspReqContainer::DspReqContainer%963906299.initialization
{
  //## begin DspReqContainer::DspReqContainer%963906299.body preserve=yes
	m_oSynchronizer.Init () ;
  //## end DspReqContainer::DspReqContainer%963906299.body
}



//## Other Operations (implementation)
m4return_t DspReqContainer::AddReq (m4uint64_t ai_iReqID, ClDspRequest *ai_poRequest)
{
  //## begin DspReqContainer::AddReq%963906297.body preserve=yes
	DspReqMap::iterator Iterator ;

	ClBlockSync oSynchro ( &m_oSynchronizer ) ;

	//M4_SRV_DEBUG( "DspReqContainer::Adding to list request %0:s",  (m4uint32_t)ai_iReqID ) ;

	Iterator = m_oReqMap.find ( ai_iReqID ) ;

	if ( Iterator != m_oReqMap.end () )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADDDPCHREQUEST,
			"Error adding Dispatcher request.\nCannot consider the request because another request with the same ID exists" );			

		return M4_ERROR ;
	}

	m_oReqMap.insert ( DspReqMap::value_type ( ai_iReqID, ai_poRequest ) ) ;

	return M4_SUCCESS ;

  //## end DspReqContainer::AddReq%963906297.body
}

m4return_t DspReqContainer::RemoveReq (m4uint64_t  ai_iReqID, ClDspRequest *&ao_poRequest)
{
  //## begin DspReqContainer::RemoveReq%963906298.body preserve=yes
	DspReqMap::iterator Iterator ;

	ClBlockSync oSynchro ( &m_oSynchronizer ) ;

	//M4_SRV_DEBUG( "DspReqContainer::Removing from list request %0:s",  (m4uint32_t)ai_iReqID ) ;

	Iterator = m_oReqMap.find ( ai_iReqID ) ;

	if ( Iterator == m_oReqMap.end () )
	{

		return M4_ERROR ;
	}

	ao_poRequest = ( *Iterator).second ;

	m_oReqMap.erase (Iterator ) ;

	return M4_SUCCESS ;
  //## end DspReqContainer::RemoveReq%963906298.body
}

// Additional Declarations
  //## begin DspReqContainer%39744AEC017C.declarations preserve=yes
  //## end DspReqContainer%39744AEC017C.declarations

//## begin module%3A093D0702B8.epilog preserve=yes
//## end module%3A093D0702B8.epilog
