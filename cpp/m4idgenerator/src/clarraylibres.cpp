//## begin module%34224066021B.cm preserve=no
//## end module%34224066021B.cm

//## begin module%34224066021B.cp preserve=no
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
//## end module%34224066021B.cp

//## Module: clarraylibres%34224066021B; Package body
//## Subsystem: M4IdGenerator::src%37943573005F
//## Source file: F:\integration\m4idgenerator\src\clarraylibres.cpp

//## begin module%34224066021B.additionalIncludes preserve=no
//## end module%34224066021B.additionalIncludes

//## begin module%34224066021B.includes preserve=yes
//## end module%34224066021B.includes

// syncro
#include <syncro.hpp>
// clarraylibres
#include <clarraylibres.hpp>
//## begin module%34224066021B.declarations preserve=no
//## end module%34224066021B.declarations

//## begin module%34224066021B.additionalDeclarations preserve=yes
//## end module%34224066021B.additionalDeclarations


// Class ClIdGenerator 

//## begin ClIdGenerator::m_iMaxId%34742C650398.attr preserve=no  private: static m4uint32_t {UA} 1
m4uint32_t ClIdGenerator::m_iMaxId = 1;
//## end ClIdGenerator::m_iMaxId%34742C650398.attr

//## begin ClIdGenerator::m_strInstance%39AB8C2A0390.attr preserve=no  private: static m4string_t {UA} ""
m4pchar_t ClIdGenerator::m_pcInstance = NULL;
//## end ClIdGenerator::m_strInstance%39AB8C2A0390.attr

//## begin ClIdGenerator::m_strHost%3AD16BC10015.attr preserve=no  private: static m4string_t {VA} ""
m4pchar_t ClIdGenerator::m_pcHost = NULL;
//## end ClIdGenerator::m_strHost%3AD16BC10015.attr

//## begin ClIdGenerator::m_strBasePort%3AD16BD8000E.attr preserve=no  private: static m4string_t {VA} ""
m4pchar_t ClIdGenerator::m_pcBasePort = NULL;
//## end ClIdGenerator::m_strBasePort%3AD16BD8000E.attr

//## begin ClIdGenerator::m_oLibres%36E939DA015D.role preserve=no  protected: static ClFreeArray { -> VHAN}
ClFreeArray ClIdGenerator::m_oLibres;
//## end ClIdGenerator::m_oLibres%36E939DA015D.role



//## Other Operations (implementation)
m4uint32_t ClIdGenerator::GetNewId ()
{
  //## begin ClIdGenerator::GetNewId%880118278.body preserve=yes
	m4uint32_t iId;
	iId = m_oLibres.GetFreeOne ( ) ;

	if (! iId )
	{
		iId = m_iMaxId ++ ;
	}

	return iId ;
  //## end ClIdGenerator::GetNewId%880118278.body
}

m4return_t ClIdGenerator::SetNewId (m4uint32_t ai_iId)
{
  //## begin ClIdGenerator::SetNewId%880118279.body preserve=yes
  m_oLibres.AddFreeOne ( ai_iId ) ;

  return M4_SUCCESS;
  //## end ClIdGenerator::SetNewId%880118279.body
}

m4string_t ClIdGenerator::GetInstanceName ()
{
  //## begin ClIdGenerator::GetInstanceName%967533896.body preserve=yes
	if ( m_pcInstance == NULL )
	{
		return "Meta4" ;
	}

	return m_pcInstance ;
  //## end ClIdGenerator::GetInstanceName%967533896.body
}

m4return_t ClIdGenerator::SetInstanceName (m4string_t ai_strName)
{
  //## begin ClIdGenerator::SetInstanceName%967533897.body preserve=yes
	if ( m_pcInstance != NULL )
	{
		return M4_ERROR ;
	}

	if ( ai_strName.empty() )
	{
		return M4_ERROR ;
	}

	/* Bug 0070216
	Hay que utilizar una char* en vez de un string que no es multithread.
	*/
	size_t iLength = ai_strName.length() ;

	m_pcInstance = new char[ iLength + 1 ] ;

	if( m_pcInstance != NULL )
	{
		memcpy( m_pcInstance, ai_strName.c_str(), iLength ) ;
		m_pcInstance[ iLength ] = '\0' ;
	}

	return M4_SUCCESS ;
  //## end ClIdGenerator::SetInstanceName%967533897.body
}

void ClIdGenerator::SetServerKey (m4string_t ai_strHost, m4string_t ai_strBasePort)
{
  //## begin ClIdGenerator::SetServerKey%986803333.body preserve=yes
	
	/* Bug 0070216
	Hay que utilizar una char* en vez de un string que no es multithread.
	*/

	size_t iLength ;
	
	
	iLength = ai_strHost.length() ;

	m_pcHost = new char[ iLength + 1 ] ;

	if( m_pcHost != NULL )
	{
		memcpy( m_pcHost, ai_strHost.c_str(), iLength ) ;
		m_pcHost[ iLength ] = '\0' ;
	}

	iLength = ai_strBasePort.length() ;

	m_pcBasePort = new char[ iLength + 1 ] ;

	if( m_pcBasePort != NULL )
	{
		memcpy( m_pcBasePort, ai_strBasePort.c_str(), iLength ) ;
		m_pcBasePort[ iLength ] = '\0' ;
	}

  //## end ClIdGenerator::SetServerKey%986803333.body
}

m4return_t ClIdGenerator::GetServerKey (m4string_t &ao_strHost, m4string_t &ao_strBasePort)
{
  //## begin ClIdGenerator::GetServerKey%986803334.body preserve=yes
	 ao_strHost = m_pcHost ;
	
	 ao_strBasePort = m_pcBasePort ;

	 return M4_SUCCESS ;
  //## end ClIdGenerator::GetServerKey%986803334.body
}

// Additional Declarations
  //## begin ClIdGenerator%34742BFC022F.declarations preserve=yes
  //## end ClIdGenerator%34742BFC022F.declarations

// Class ClFreeArray 

//## begin ClFreeArray::m_oMutex%36E934460268.role preserve=no  protected: static ClMutex { -> VHAN}
ClMutex ClFreeArray::m_oMutex = M4_TRUE;
//## end ClFreeArray::m_oMutex%36E934460268.role



ClFreeArray::ClFreeArray ()
  //## begin ClFreeArray::ClFreeArray%627743275.hasinit preserve=no
  //## end ClFreeArray::ClFreeArray%627743275.hasinit
  //## begin ClFreeArray::ClFreeArray%627743275.initialization preserve=yes
  //## end ClFreeArray::ClFreeArray%627743275.initialization
{
  //## begin ClFreeArray::ClFreeArray%627743275.body preserve=yes
	m_oMutex.Init ( );
	m_aiArray.clear ( ) ;
  //## end ClFreeArray::ClFreeArray%627743275.body
}


ClFreeArray::~ClFreeArray ()
{
  //## begin ClFreeArray::~ClFreeArray%1086045097.body preserve=yes
  //## end ClFreeArray::~ClFreeArray%1086045097.body
}



//## Other Operations (implementation)
m4uint32_t ClFreeArray::GetFreeOne ()
{
  //## begin ClFreeArray::GetFreeOne%-1480728616.body preserve=yes
		m4int32_t iIndex;
		dqArray::iterator Iterator;


		//if the array is empty no handles are abailable

		m_oMutex.Lock() ;

		if ( m_aiArray.empty() )
		{
			m_oMutex.Unlock() ;

			return 0;
		}

		//else get the first element in the array 

		Iterator = m_aiArray.begin();

		iIndex = ( * Iterator ) ;

		m_aiArray.pop_front ( ) ;


		m_oMutex.Unlock() ;

		return iIndex;
  //## end ClFreeArray::GetFreeOne%-1480728616.body
}

void ClFreeArray::AddFreeOne (m4uint32_t ai_iFreeIndex)
{
  //## begin ClFreeArray::AddFreeOne%1522906610.body preserve=yes
	m_oMutex.Lock ( ) ;

	m_aiArray.push_back ( ai_iFreeIndex ) ;

	m_oMutex.Unlock ( ) ;
  //## end ClFreeArray::AddFreeOne%1522906610.body
}

// Additional Declarations
  //## begin ClFreeArray%34223FFB0055.declarations preserve=yes
  //## end ClFreeArray%34223FFB0055.declarations

//## begin module%34224066021B.epilog preserve=yes
//## end module%34224066021B.epilog
