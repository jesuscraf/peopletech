//## begin module%3A815FA80374.cm preserve=no
//## end module%3A815FA80374.cm

//## begin module%3A815FA80374.cp preserve=no
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
//## end module%3A815FA80374.cp

//## Module: reqrepository%3A815FA80374; Package body
//## Subsystem: M4Appctl::src%37EA676703E6
//## Source file: F:\integration\M4Appctl\src\reqrepository.cpp

//## begin module%3A815FA80374.additionalIncludes preserve=no
//## end module%3A815FA80374.additionalIncludes

//## begin module%3A815FA80374.includes preserve=yes
#include <blocksynchronization.hpp>
//## end module%3A815FA80374.includes

// CriticalSection
#include <criticalsection.hpp>
// appctlreq
#include <appctlreq.hpp>
// reqrepository
#include <reqrepository.hpp>
//## begin module%3A815FA80374.declarations preserve=no
//## end module%3A815FA80374.declarations

//## begin module%3A815FA80374.additionalDeclarations preserve=yes
//## end module%3A815FA80374.additionalDeclarations


// Class ClReqRepository 




ClReqRepository::ClReqRepository ()
  //## begin ClReqRepository::ClReqRepository%981535910.hasinit preserve=no
  //## end ClReqRepository::ClReqRepository%981535910.hasinit
  //## begin ClReqRepository::ClReqRepository%981535910.initialization preserve=yes
  //## end ClReqRepository::ClReqRepository%981535910.initialization
{
  //## begin ClReqRepository::ClReqRepository%981535910.body preserve=yes
	m_oExclusive.Init () ;
  //## end ClReqRepository::ClReqRepository%981535910.body
}



//## Other Operations (implementation)
m4return_t ClReqRepository::AddRequest (m4uint32_t ai_iRequestID, ClAppCtlRequest *ai_poRequest)
{
  //## begin ClReqRepository::AddRequest%981535911.body preserve=yes
	ClReqMap::iterator Iterator ;
	ClBlockSync oBlockSynch ( m_oExclusive ) ;

	Iterator = m_oReqMap.find ( ai_iRequestID ) ;

	if ( Iterator != m_oReqMap.end () )
	{
		return M4_ERROR ;
	}

	m_oReqMap.insert ( ClReqMap::value_type ( ai_iRequestID , ai_poRequest ) ) ;


	return M4_SUCCESS ;
  //## end ClReqRepository::AddRequest%981535911.body
}

m4return_t ClReqRepository::GetRequest (m4uint32_t ai_iRequestID, ClAppCtlRequest *&ao_poRequest)
{
  //## begin ClReqRepository::GetRequest%981535912.body preserve=yes
	ClReqMap::iterator Iterator ;
	ClBlockSync oBlockSynch ( m_oExclusive ) ;


	Iterator = m_oReqMap.find ( ai_iRequestID ) ;

	if ( Iterator == m_oReqMap.end () )
	{
		return M4_ERROR ;
	}

	ao_poRequest = ( *Iterator).second ;


	return M4_SUCCESS ;
  //## end ClReqRepository::GetRequest%981535912.body
}

m4return_t ClReqRepository::RemoveRequest (m4uint32_t ai_iRequestID, ClAppCtlRequest *&ao_poRequest)
{
  //## begin ClReqRepository::RemoveRequest%981535913.body preserve=yes
	ClReqMap::iterator Iterator ;
	ClBlockSync oBlockSynch ( m_oExclusive ) ;


	Iterator = m_oReqMap.find ( ai_iRequestID ) ;

	if ( Iterator == m_oReqMap.end () )
	{
		return M4_ERROR ;
	}

	ao_poRequest = ( *Iterator).second ;


	m_oReqMap.erase ( Iterator ) ;

	return M4_SUCCESS ;
  //## end ClReqRepository::RemoveRequest%981535913.body
}

// Additional Declarations
  //## begin ClReqRepository%3A815E55029A.declarations preserve=yes
  //## end ClReqRepository%3A815E55029A.declarations

//## begin module%3A815FA80374.epilog preserve=yes
//## end module%3A815FA80374.epilog
