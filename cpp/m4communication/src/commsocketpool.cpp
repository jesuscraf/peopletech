//## begin module%39D362BF0142.cm preserve=no
//## end module%39D362BF0142.cm

//## begin module%39D362BF0142.cp preserve=no
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
//## end module%39D362BF0142.cp

//## Module: commsocketpool%39D362BF0142; Package body
//## Subsystem: M4Communication::M4COML::src%38D1FCE80326
//## Source file: e:\mybuild\m4communication\src\commsocketpool.cpp

//## begin module%39D362BF0142.additionalIncludes preserve=no
//## end module%39D362BF0142.additionalIncludes

//## begin module%39D362BF0142.includes preserve=yes
#define M4_COMM_DEFAULT_POOL_N_SOCKET	100000
#include <blocksynchronization.hpp>
#include <m4trace.hpp>
#include <tsap.hpp>
#include <communicationlayer.hpp>
//## end module%39D362BF0142.includes

// m4types
#include <m4types.hpp>
#include <basiclog.hpp>
// m4string
#include <m4string.hpp>
// CriticalSection
#include <criticalsection.hpp>
// commsocketpool
#include <commsocketpool.hpp>
// CommComun
#include <commcomun.hpp>
//## begin module%39D362BF0142.declarations preserve=no
//## end module%39D362BF0142.declarations

//## begin module%39D362BF0142.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID "COMM"
#include <m4srvtrace.h>

void msecPausa(m4int_t ai_iMiliSeconds)
{
#ifdef _WINDOWS
		Sleep(ai_iMiliSeconds);
#else
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = ai_iMiliSeconds*1000;
		(void) select(2, NULL, NULL, NULL, &timeout);
#endif
}
//## end module%39D362BF0142.additionalDeclarations


// Class ClCommSocketPool 



ClCommSocketPool::ClCommSocketPool ()
  //## begin ClCommSocketPool::ClCommSocketPool%970156025.hasinit preserve=no
  //## end ClCommSocketPool::ClCommSocketPool%970156025.hasinit
  //## begin ClCommSocketPool::ClCommSocketPool%970156025.initialization preserve=yes
  //## end ClCommSocketPool::ClCommSocketPool%970156025.initialization
{
  //## begin ClCommSocketPool::ClCommSocketPool%970156025.body preserve=yes
	m_oMutex.Init();
  //## end ClCommSocketPool::ClCommSocketPool%970156025.body
}


ClCommSocketPool::~ClCommSocketPool ()
{
  //## begin ClCommSocketPool::~ClCommSocketPool%970156026.body preserve=yes
	CommPoolMap::iterator itePM=m_PoolMap.begin();
	while(itePM!=m_PoolMap.end())
	{
		delete (*itePM).second;
		itePM=m_PoolMap.begin();
	}
  //## end ClCommSocketPool::~ClCommSocketPool%970156026.body
}



//## Other Operations (implementation)
m4tliid_t ClCommSocketPool::GetSocket (m4string_t &ai_strURL, m4int_t ai_imsecWaitTime)
{
  //## begin ClCommSocketPool::GetSocket%970156027.body preserve=yes
	if(ai_imsecWaitTime<0)
		return NULL;
	m4tliid_t theTLI=NULL;
	m_oMutex.Lock();

	while((NULL==theTLI) && (ai_imsecWaitTime>=0))
	{
		theTLI=_GetSocket (ai_strURL, ai_imsecWaitTime);
		m_oMutex.Unlock();
		if(NULL==theTLI)
		{
			ai_imsecWaitTime-=10;
			if(ai_imsecWaitTime>=0)
			{
				msecPausa(10);
				m_oMutex.Lock();
			}
		}
	}

	if(NULL==theTLI)
	{
		m_oMutex.Lock();
		theTLI=_GetNewSocket (ai_strURL, ai_imsecWaitTime);
		m_oMutex.Unlock();
	}

	return theTLI;

  //## end ClCommSocketPool::GetSocket%970156027.body
}

m4return_t ClCommSocketPool::ReleaseSocket (m4string_t &ai_strUrl, m4tliid_t ai_TLIID)
{
  //## begin ClCommSocketPool::ReleaseSocket%970156028.body preserve=yes
	ClBlockSync	oBlocker(m_oMutex);
	CommPoolMap::iterator itePM=m_PoolMap.find(ai_strUrl);
	if(itePM==m_PoolMap.end())
	{
		M4_SRV_ERROR(0,"Couldn't find Socket properties, url %0:s", ai_strUrl.c_str() );
		return M4_ERROR;
	}

	ClCommPoolProperties *thePoolP=(*itePM).second;
	return thePoolP->ReleaseSocket(ai_TLIID);
  //## end ClCommSocketPool::ReleaseSocket%970156028.body
}

m4return_t ClCommSocketPool::AddNewSocket (m4string_t &ai_strUrl, m4tliid_t ai_TLIID)
{
  //## begin ClCommSocketPool::AddNewSocket%970156029.body preserve=yes
	CommPoolMap::iterator itePM=m_PoolMap.find(ai_strUrl);
	if(itePM==m_PoolMap.end())
	{
		M4_SRV_ERROR(0,"Couldn't find Socket properties, url %0:s", ai_strUrl.c_str() );
		return M4_ERROR;
	}

	ClCommPoolProperties *thePoolP=(*itePM).second;
	return thePoolP->AddNewSocket (ai_TLIID);
  //## end ClCommSocketPool::AddNewSocket%970156029.body
}

m4return_t ClCommSocketPool::SetNSocketPool (m4string_t &ai_strURL, m4int_t ai_iNSocket)
{
  //## begin ClCommSocketPool::SetNSocketPool%971164985.body preserve=yes
	ClBlockSync	oBlocker(m_oMutex);
	return _SetNSocketPool (ai_strURL, ai_iNSocket);

  //## end ClCommSocketPool::SetNSocketPool%971164985.body
}

void ClCommSocketPool::DumpStats (m4string_t &ai_strURL)
{
  //## begin ClCommSocketPool::DumpStats%993554206.body preserve=yes
	CommPoolMap::iterator itePM=m_PoolMap.find(ai_strURL);
	if(itePM==m_PoolMap.end())
	{
		M4_SRV_ERROR(0,"Couldn't find Socket properties, url %0:s", ai_strURL.c_str() );
		return;
	}
	ClCommPoolProperties *thePoolP=(*itePM).second;
	thePoolP->DumpStats();
  //## end ClCommSocketPool::DumpStats%993554206.body
}

m4return_t ClCommSocketPool::_SetNSocketPool (m4string_t &ai_strURL, m4int_t ai_iNSocket)
{
  //## begin ClCommSocketPool::_SetNSocketPool %993554207.body preserve=yes
	CommPoolMap::iterator itePM=m_PoolMap.find(ai_strURL);
	if(itePM!=m_PoolMap.end())
	{
		M4_SRV_ERROR(0,"Couldn't change Socket properties, url %0:s", ai_strURL.c_str() );
		return M4_ERROR;
	}

	ClCommPoolProperties *thePoolP=new ClCommPoolProperties;

	thePoolP->SetNMaxSocket( ai_iNSocket);
	m_PoolMap[ai_strURL]=thePoolP;

	return M4_SUCCESS;
  //## end ClCommSocketPool::_SetNSocketPool %993554207.body
}

m4tliid_t ClCommSocketPool::_GetSocket (m4string_t &ai_strURL, m4int_t ai_imsecWaitTime)
{
  //## begin ClCommSocketPool::_GetSocket%993625534.body preserve=yes
	CommPoolMap::iterator itePM=m_PoolMap.find(ai_strURL);
	if(itePM==m_PoolMap.end())
	{
		M4_SRV_DEBUG("Couldn't find  Socket properties. Creating %0 socket Max",M4_COMM_DEFAULT_POOL_N_SOCKET);
		_SetNSocketPool(ai_strURL,M4_COMM_DEFAULT_POOL_N_SOCKET);
		itePM=m_PoolMap.find(ai_strURL);
	}

	ClCommPoolProperties *thePoolP=(*itePM).second;
	return thePoolP->GetSocket ();
  //## end ClCommSocketPool::_GetSocket%993625534.body
}

m4tliid_t ClCommSocketPool::_GetNewSocket (m4string_t &ai_strURL, m4int_t ai_imsecWaitTime)
{
  //## begin ClCommSocketPool::_GetNewSocket%993625535.body preserve=yes
	CommPoolMap::iterator itePM=m_PoolMap.find(ai_strURL);
	if(itePM==m_PoolMap.end())
	{
		M4_SRV_DEBUG("Couldn't find  Socket properties. Creating %0 socket Max",M4_COMM_DEFAULT_POOL_N_SOCKET);
		_SetNSocketPool(ai_strURL,M4_COMM_DEFAULT_POOL_N_SOCKET);
		itePM=m_PoolMap.find(ai_strURL);
	}

	ClCommPoolProperties *thePoolP=(*itePM).second;
	m4tliid_t theTLI=thePoolP->GetSocket ();

	if(NULL==theTLI)
	{
		if(thePoolP->GetNMaxSocket()>thePoolP->GetNSocket())
		{
			if(M4_SUCCESS==CreateSocket(ai_strURL,theTLI))
			{
				if(M4_SUCCESS==thePoolP->AddNewSocket(theTLI))
					theTLI=thePoolP->GetSocket ();
				else
				{
					M4_SRV_ERROR(0,"Error AddingNewSocket, URL %0:s", ai_strURL.c_str() );
					theTLI=NULL;
				}
			}
			else
				M4_SRV_ERROR(0,"Error creating socket %0:s" ,ai_strURL.c_str() );
		}
		else
			M4_SRV_DEBUG_N("Max num of socket Reach" );
	}

	return theTLI;
  //## end ClCommSocketPool::_GetNewSocket%993625535.body
}

// Additional Declarations
  //## begin ClCommSocketPool%39D364D40333.declarations preserve=yes
  //## end ClCommSocketPool%39D364D40333.declarations

// Class ClCommPoolProperties 







ClCommPoolProperties::ClCommPoolProperties ()
  //## begin ClCommPoolProperties::ClCommPoolProperties%972041784.hasinit preserve=no
      : m_iMaxSocket(0), m_iNSocket(0)
  //## end ClCommPoolProperties::ClCommPoolProperties%972041784.hasinit
  //## begin ClCommPoolProperties::ClCommPoolProperties%972041784.initialization preserve=yes
  //## end ClCommPoolProperties::ClCommPoolProperties%972041784.initialization
{
  //## begin ClCommPoolProperties::ClCommPoolProperties%972041784.body preserve=yes
  //## end ClCommPoolProperties::ClCommPoolProperties%972041784.body
}


ClCommPoolProperties::~ClCommPoolProperties ()
{
  //## begin ClCommPoolProperties::~ClCommPoolProperties%972041785.body preserve=yes
	m_AvailableRes.clear();
  //## end ClCommPoolProperties::~ClCommPoolProperties%972041785.body
}



//## Other Operations (implementation)
m4return_t ClCommPoolProperties::SetNMaxSocket (m4int_t ai_iNMaxSocket)
{
  //## begin ClCommPoolProperties::SetNMaxSocket%972041780.body preserve=yes
	m_iMaxSocket=ai_iNMaxSocket;
	return M4_SUCCESS;
  //## end ClCommPoolProperties::SetNMaxSocket%972041780.body
}

m4return_t ClCommPoolProperties::ReleaseSocket (m4tliid_t ai_TLI)
{
  //## begin ClCommPoolProperties::ReleaseSocket%972041781.body preserve=yes
	m_AvailableRes.push_back (ai_TLI);
	return M4_SUCCESS;
  //## end ClCommPoolProperties::ReleaseSocket%972041781.body
}

m4tliid_t ClCommPoolProperties::GetSocket ()
{
  //## begin ClCommPoolProperties::GetSocket%972041782.body preserve=yes
	m4tliid_t theTLI=NULL;
	if(m_AvailableRes.size ())
	{
		theTLI=m_AvailableRes.back();
		m_AvailableRes.pop_back();
//cerr <<(int)theTLI<<endl;
ClResourcePoll::iterator theIte;
		theIte=m_ResourcePoll.find(theTLI);
		if(theIte==m_ResourcePoll.end())
		{
			ClResourcePoll::iterator theIte2;
			theIte2=m_ResourcePoll.find(theTLI);
		//	if(theIte2==m_ResourcePoll.end())
		//		cerr << "Y eso?"<<endl;
		}
		else
			(*theIte).second->AddHit();
	}
	return theTLI;
  //## end ClCommPoolProperties::GetSocket%972041782.body
}

m4int_t ClCommPoolProperties::GetNSocket ()
{
  //## begin ClCommPoolProperties::GetNSocket%972041783.body preserve=yes
	return m_iNSocket;
  //## end ClCommPoolProperties::GetNSocket%972041783.body
}

m4int_t ClCommPoolProperties::GetNFreeSocket ()
{
  //## begin ClCommPoolProperties::GetNFreeSocket%972292995.body preserve=yes
	return m_AvailableRes.size();
  //## end ClCommPoolProperties::GetNFreeSocket%972292995.body
}

m4return_t ClCommPoolProperties::AddNewSocket (m4tliid_t ai_TLI)
{
  //## begin ClCommPoolProperties::AddNewSocket%972292996.body preserve=yes
	m_AvailableRes.push_back (ai_TLI);
	ClResource *theres=new ClResource (ai_TLI);
//	cerr<< "Add:"<<(int)ai_TLI<<endl;
	m_ResourcePoll[ai_TLI]=theres;
	m_iNSocket++;
//	M4_ASSERT(m_iNSocket==m_ResourcePoll.size());
	return M4_SUCCESS;
  //## end ClCommPoolProperties::AddNewSocket%972292996.body
}

m4int_t ClCommPoolProperties::GetNMaxSocket ()
{
  //## begin ClCommPoolProperties::GetNMaxSocket%972292997.body preserve=yes
	return m_iMaxSocket;
  //## end ClCommPoolProperties::GetNMaxSocket%972292997.body
}

void ClCommPoolProperties::DumpStats ()
{
  //## begin ClCommPoolProperties::DumpStats%993554208.body preserve=yes
  //## end ClCommPoolProperties::DumpStats%993554208.body
}

// Additional Declarations
  //## begin ClCommPoolProperties%39E322F3031F.declarations preserve=yes
  //## end ClCommPoolProperties%39E322F3031F.declarations

// Class ClCommSocketPoolBasicTSAP 

ClCommSocketPoolBasicTSAP::ClCommSocketPoolBasicTSAP ()
  //## begin ClCommSocketPoolBasicTSAP::ClCommSocketPoolBasicTSAP%973524719.hasinit preserve=no
  //## end ClCommSocketPoolBasicTSAP::ClCommSocketPoolBasicTSAP%973524719.hasinit
  //## begin ClCommSocketPoolBasicTSAP::ClCommSocketPoolBasicTSAP%973524719.initialization preserve=yes
  //## end ClCommSocketPoolBasicTSAP::ClCommSocketPoolBasicTSAP%973524719.initialization
{
  //## begin ClCommSocketPoolBasicTSAP::ClCommSocketPoolBasicTSAP%973524719.body preserve=yes
  //## end ClCommSocketPoolBasicTSAP::ClCommSocketPoolBasicTSAP%973524719.body
}


ClCommSocketPoolBasicTSAP::~ClCommSocketPoolBasicTSAP ()
{
  //## begin ClCommSocketPoolBasicTSAP::~ClCommSocketPoolBasicTSAP%973524720.body preserve=yes
  //## end ClCommSocketPoolBasicTSAP::~ClCommSocketPoolBasicTSAP%973524720.body
}



//## Other Operations (implementation)
m4return_t ClCommSocketPoolBasicTSAP::CreateSocket (m4string_t &ai_strURL, m4tliid_t &ao_TLIID)
{
  //## begin ClCommSocketPoolBasicTSAP::CreateSocket%973524721.body preserve=yes
	if(ai_strURL.empty())
		return M4_ERROR;
	
	m4pchar_t pcHost;
	m4int_t iPort;
	m4int_t retvalue=sscanf(ai_strURL.c_str(),"pdu:%d//%s",&iPort,&pcHost);
	if(1)
	{
		iPort=atoi(ai_strURL.c_str()+ai_strURL.find(':')+1);
		pcHost=(char *)(ai_strURL.c_str()+ai_strURL.find_last_of('/')+1);
	}
	cout << "me piden un socket con url "<<ai_strURL<<endl;
	cout << "Puerto"<<iPort<<" Host:"<<pcHost<<endl;
	ClTSAP *theTSAP=new ClTSAP (pcHost,iPort);
	ao_TLIID=(m4tliid_t)theTSAP;
	return M4_SUCCESS;
  //## end ClCommSocketPoolBasicTSAP::CreateSocket%973524721.body
}

// Additional Declarations
  //## begin ClCommSocketPoolBasicTSAP%3A06CDA40018.declarations preserve=yes
  //## end ClCommSocketPoolBasicTSAP%3A06CDA40018.declarations

// Class ClCommSocketPoolComm 



//## Other Operations (implementation)
m4return_t ClCommSocketPoolComm::CreateSocket (m4string_t &ai_strURL, m4tliid_t &ao_TLIID)
{
  //## begin ClCommSocketPoolComm::CreateSocket%975323612.body preserve=yes
	return M4_ERROR;
  //## end ClCommSocketPoolComm::CreateSocket%975323612.body
}

// Additional Declarations
  //## begin ClCommSocketPoolComm%3A227280004F.declarations preserve=yes
  //## end ClCommSocketPoolComm%3A227280004F.declarations

// Class ClResource 



ClResource::ClResource (m4tliid_t ai_TLI)
  //## begin ClResource::ClResource%993554202.hasinit preserve=no
      : m_iHits(0), m_TLI(NULL)
  //## end ClResource::ClResource%993554202.hasinit
  //## begin ClResource::ClResource%993554202.initialization preserve=yes
  //## end ClResource::ClResource%993554202.initialization
{
  //## begin ClResource::ClResource%993554202.body preserve=yes
	m_TLI=ai_TLI;
  //## end ClResource::ClResource%993554202.body
}



//## Other Operations (implementation)
void ClResource::AddHit ()
{
  //## begin ClResource::AddHit%993554203.body preserve=yes
	m_iHits++;
  //## end ClResource::AddHit%993554203.body
}

m4int_t ClResource::Hits ()
{
  //## begin ClResource::Hits%993554204.body preserve=yes
	return m_iHits;
  //## end ClResource::Hits%993554204.body
}

m4tliid_t ClResource::GetTLI ()
{
  //## begin ClResource::GetTLI%993554205.body preserve=yes
	return m_TLI;
  //## end ClResource::GetTLI%993554205.body
}

// Additional Declarations
  //## begin ClResource%3B386D70037D.declarations preserve=yes
  //## end ClResource%3B386D70037D.declarations

//## begin module%39D362BF0142.epilog preserve=yes
//## end module%39D362BF0142.epilog
