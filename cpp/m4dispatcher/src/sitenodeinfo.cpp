//## begin module%3923BC11017B.cm preserve=no
//## end module%3923BC11017B.cm

//## begin module%3923BC11017B.cp preserve=no
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
//## end module%3923BC11017B.cp

//## Module: SiteNodeInfo%3923BC11017B; Subprogram body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: z:\m4dispatcher\src\sitenodeinfo.cpp

//## begin module%3923BC11017B.additionalIncludes preserve=no
//## end module%3923BC11017B.additionalIncludes

//## begin module%3923BC11017B.includes preserve=yes
#pragma warning (disable : 4284)
#include <blocksynchronization.hpp>
#include <dspparams.hpp>
#include <basiclog.hpp>
#include <m4dpchres.hpp>
#include <m4memdbg.h>
#include <m4srvtrace.h>
#include <snmpadmininstance.hpp>
#include "m4dspapi4snmp.hpp"

#define M4_SRV_TRACE_COMP_ID "DPCH"

//## end module%3923BC11017B.includes

// snmpinterfaces
#include <snmpinterfaces.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// SiteNodeInfo
#include <sitenodeinfo.hpp>
// CriticalSection
#include <criticalsection.hpp>
// m4date
#include <m4date.hpp>
//## begin module%3923BC11017B.declarations preserve=no
//## end module%3923BC11017B.declarations

//## begin module%3923BC11017B.additionalDeclarations preserve=yes
m4pchar_t ToLower (m4pchar_t aio_string)
{
	m4pchar_t szString ;


	szString = aio_string ;
	for( ; *aio_string; ++aio_string) 
	{
		*aio_string=tolower(*aio_string) ;
	} ;

	return szString ;
}

m4int_t CompareHostName(const char* ai_pcHost, const char* ai_pcNodeHost)
{
	// chequeo argumentos.
	if ((NULL == ai_pcHost) || (NULL == ai_pcNodeHost))
	{
		return -1;
	}

	m4string_t sHost = ai_pcHost;
	m4string_t sNodeHost = ai_pcNodeHost;

    // me quedo únicamente con el nombre del host, sin dominio.
	sHost = sHost.substr(0, sHost.find_first_of ("."));
	sNodeHost = sNodeHost.substr(0, sNodeHost.find_first_of ("."));

	return sHost.compare(sNodeHost);
}

//## end module%3923BC11017B.additionalDeclarations


// Class ClSiteNodeInformation 









//## begin ClSiteNodeInformation::M4NullSite%40B227830272.attr preserve=no  public: static ClSiteNodeInformation {UA} 
ClSiteNodeInformation ClSiteNodeInformation::M4NullSite;
//## end ClSiteNodeInformation::M4NullSite%40B227830272.attr











ClSiteNodeInformation::ClSiteNodeInformation (const m4string_t &ai_strSiteNodeID, const m4string_t ai_strHost, const m4uint32_t ai_iBasePort, const m4string_t ai_strUserName, const m4string_t ai_strPassword, const m4string_t ai_szState, const m4double_t  ai_dCPUUsage, const m4uint32_t ai_iNumSessions, const m4uint32_t ai_dServerMemory, const m4double_t ai_dSystemMemory, const m4uint32_t ai_iConnectionID, m4double_t ai_dPrivateMB, m4double_t ai_dMaxProcSize, m4double_t ai_dFreeMem)
  //## begin ClSiteNodeInformation::ClSiteNodeInformation%958658601.hasinit preserve=no
      : m_bIsEnabled(M4_TRUE), m_pM4ISnmpObject(NULL)
  //## end ClSiteNodeInformation::ClSiteNodeInformation%958658601.hasinit
  //## begin ClSiteNodeInformation::ClSiteNodeInformation%958658601.initialization preserve=yes
  //## end ClSiteNodeInformation::ClSiteNodeInformation%958658601.initialization
{
  //## begin ClSiteNodeInformation::ClSiteNodeInformation%958658601.body preserve=yes
	m_strSiteNodeID = ai_strSiteNodeID ;
	m_dCPU = ai_dCPUUsage ;
	m_iNumSessions = ai_iNumSessions ;
	m_iServerMemory = ai_dServerMemory ;
	m_dSystemMemory = ai_dSystemMemory ;
	m_strHost = ai_strHost ;
	m_iBasePort = ai_iBasePort ;
	m_strNodeState = ai_szState ;
	m_iConnectionID = ai_iConnectionID;
	m_strUserName = ai_strUserName;
	m_strPassword = ai_strPassword;
	m_bPersistable = M4_TRUE ;
	m_dCppLoadFactor = 0 ;
	m_dJavaLoadFactor = 0 ;
	m_iStartingtries = 0 ;
	m_iStopReason = 0 ;
    m_tLastUpdate = 0.0;
    
	m_dPrivateMB = ai_dPrivateMB ;
    m_dMaxProcSize = ai_dMaxProcSize ;
    m_dFreeMem = ai_dFreeMem ;

	m_bDiscarded = M4_FALSE ;

	m_iOLTPTime = 0;
	m_iXMLTime = 0;

  //## end ClSiteNodeInformation::ClSiteNodeInformation%958658601.body
}

ClSiteNodeInformation::ClSiteNodeInformation (const ClSiteNodeInformation &ai_oSiteNode)
  //## begin ClSiteNodeInformation::ClSiteNodeInformation%959009686.hasinit preserve=no
      : m_bIsEnabled(M4_TRUE), m_pM4ISnmpObject(NULL)
  //## end ClSiteNodeInformation::ClSiteNodeInformation%959009686.hasinit
  //## begin ClSiteNodeInformation::ClSiteNodeInformation%959009686.initialization preserve=yes
  //## end ClSiteNodeInformation::ClSiteNodeInformation%959009686.initialization
{
  //## begin ClSiteNodeInformation::ClSiteNodeInformation%959009686.body preserve=yes
	*this = (ai_oSiteNode);
  //## end ClSiteNodeInformation::ClSiteNodeInformation%959009686.body
}

ClSiteNodeInformation::ClSiteNodeInformation ()
  //## begin ClSiteNodeInformation::ClSiteNodeInformation%958658602.hasinit preserve=no
      : m_bIsEnabled(M4_TRUE), m_pM4ISnmpObject(NULL)
  //## end ClSiteNodeInformation::ClSiteNodeInformation%958658602.hasinit
  //## begin ClSiteNodeInformation::ClSiteNodeInformation%958658602.initialization preserve=yes
  //## end ClSiteNodeInformation::ClSiteNodeInformation%958658602.initialization
{
  //## begin ClSiteNodeInformation::ClSiteNodeInformation%958658602.body preserve=yes
	m_iStartingtries = 0;
	m_iStopReason = 0 ;
    m_tLastUpdate = 0.0;
	*this = (M4NullSite);
  //## end ClSiteNodeInformation::ClSiteNodeInformation%958658602.body
}



//## Other Operations (implementation)
m4return_t ClSiteNodeInformation::UpdateInformation (const ClSiteNodeInformation &ai_oNodeInformation)
{
  //## begin ClSiteNodeInformation::UpdateInformation%958643328.body preserve=yes
	m4bool_t bUpdated = M4_FALSE ;
	M4ClTimeStamp oCrono;
	m4string_t newState ;


	if (ai_oNodeInformation.GetSiteNodeMapKey( ) != this->GetSiteNodeMapKey( ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSITENODEID,
			"Error updating site node information. Cannot get the site node ID." );

		return M4_ERROR;
	}


	oCrono.now () ;

	oCrono.get ( m_tLastUpdate ) ;

	if ( "" == m_strHost )
	{
		m_strHost = ai_oNodeInformation.GetNodeHost() ;
		bUpdated = M4_TRUE ;
	} 
	

	if ( m_strNodeState != (newState = ai_oNodeInformation.GetNodeState() ) )
	{
		// fix bug 0088880
		// gets the local host definition
		if ( ! stricmp ( newState.c_str() , STATE_READY ) ) {
			if (m_strLocalHost != ai_oNodeInformation.GetNodeHost()) {
				m_strLocalHost = ai_oNodeInformation.GetNodeHost() ;
			}
		}
		else {
			if (m_strHost != m_strLocalHost) {
				m_strLocalHost = m_strHost;
			}
		}

		if ( ! stricmp ( newState.c_str() , "SHUTTING DOWN/MEMORY ALERT" ) )
		{
			m_iStopReason = STOP_MEMORY ;
		}
		if ( ! stricmp ( newState.c_str() , "SHUTTING DOWN/ADMIN SHUTDOWN" ) )
		{
			m_iStopReason = STOP_ADMIN ;
		}
		if ( ! stricmp ( newState.c_str() , "SHUTTING DOWN/LDB SHUTDOWN" ) )
		{
			m_iStopReason = STOP_LDB ;
		}
		if ( ! stricmp ( newState.c_str() , "SHUTTING DOWN" ) )
		{
			m_iStopReason = STOP_GENERAL ;
		}

		m_strNodeState = newState ;
		bUpdated = M4_TRUE ;
	}

	if ( m_dSystemMemory != ai_oNodeInformation.GetSystemMemory () )
	{
		m_dSystemMemory = ai_oNodeInformation.GetSystemMemory () ;
		bUpdated = M4_TRUE ;
	}
	if ( m_dCPU != ai_oNodeInformation.GetCPUUsage () )
	{
		m_dCPU = ai_oNodeInformation.GetCPUUsage () ;
		bUpdated = M4_TRUE ;
	}
	if ( m_iNumSessions != ai_oNodeInformation.GetNumSessions () )
	{
		m_iNumSessions = ai_oNodeInformation.GetNumSessions () ;
		bUpdated = M4_TRUE ;
	}
	if ( m_iServerMemory != ai_oNodeInformation.GetServerMemory() )
	{
		m_iServerMemory = ai_oNodeInformation.GetServerMemory() ;
		bUpdated = M4_TRUE ;
	}

	// eh 178555
	if ( m_dPrivateMB != ai_oNodeInformation.GetPrivateMB() )
	{
		m_dPrivateMB = ai_oNodeInformation.GetPrivateMB() ;
		bUpdated = M4_TRUE ;
	}

	if ( m_dMaxProcSize != ai_oNodeInformation.GetMaxProcSize() )
	{
		m_dMaxProcSize = ai_oNodeInformation.GetMaxProcSize() ;
		bUpdated = M4_TRUE ;
	}

	if ( m_dFreeMem != ai_oNodeInformation.GetFreeMem() )
	{
		m_dFreeMem = ai_oNodeInformation.GetFreeMem() ;
		bUpdated = M4_TRUE ;
	}


	if ( m_iConnectionID != ai_oNodeInformation.GetConnectionID () )
	{
		m_iConnectionID = ai_oNodeInformation.GetConnectionID () ;
	}

	if ( ! m_iBasePort )
	{
		m_iBasePort = ai_oNodeInformation.GetBasePort () ;
	}

	if ( m_bPersistable != ai_oNodeInformation.IsPersistable () )
	{
		m_bPersistable = ai_oNodeInformation.IsPersistable () ;
	}

	if( m_iStartingtries != ai_oNodeInformation.m_iStartingtries )
	{
		m_iStartingtries = ai_oNodeInformation.m_iStartingtries ;
	}

    m_bIsEnabled = ai_oNodeInformation.m_bIsEnabled;

	if ( bUpdated )
		CalculateLoad( BOTH_CLIENTS );
	//	Update the rest of the fields

	return M4_SUCCESS;
  //## end ClSiteNodeInformation::UpdateInformation%958643328.body
}


ClSiteNodeInformation & ClSiteNodeInformation::operator = (const ClSiteNodeInformation &ai_oRHSNode)
{
  //## begin ClSiteNodeInformation::operator=%959009687.body preserve=yes
	if (&ai_oRHSNode != this)
	{
		m_dCppLoadFactor = ai_oRHSNode.GetCppLoadFactor();
		m_dJavaLoadFactor = ai_oRHSNode.GetJavaLoadFactor();
		m_strSiteNodeID = ai_oRHSNode.GetSiteNodeID();
		m_strHost = ai_oRHSNode.GetNodeHost ();
		m_strLocalHost = ai_oRHSNode.GetLocalHost ();
		m_iBasePort = ai_oRHSNode.GetBasePort ();
		m_strNodeState = ai_oRHSNode.GetNodeState () ;
		m_tLastUpdate = ai_oRHSNode.m_tLastUpdate ;
		m_strUserName = ai_oRHSNode.GetUserName();
		m_strPassword = ai_oRHSNode.GetPassword();
		m_dSystemMemory = ai_oRHSNode.GetSystemMemory();
		m_dCPU = ai_oRHSNode.GetCPUUsage();
		m_iServerMemory = ai_oRHSNode.GetServerMemory();
		m_iNumSessions = ai_oRHSNode.GetNumSessions();
		m_iConnectionID = ai_oRHSNode.GetConnectionID () ;
		m_bPersistable = ai_oRHSNode.IsPersistable () ;
		m_iStartingtries = ai_oRHSNode.m_iStartingtries ;
		m_iStopReason = ai_oRHSNode.m_iStopReason ;
        m_pM4ISnmpObject = ai_oRHSNode.m_pM4ISnmpObject;
        m_bIsEnabled = ai_oRHSNode.m_bIsEnabled;

		m_dPrivateMB = ai_oRHSNode.GetPrivateMB();
		m_dMaxProcSize = ai_oRHSNode.GetMaxProcSize();
		m_dFreeMem = ai_oRHSNode.GetFreeMem();

		m_bDiscarded = ai_oRHSNode.GetDiscarded() ;

		m_iOLTPTime = ai_oRHSNode.GetOLTPTime();
		m_iXMLTime = ai_oRHSNode.GetXMLTime();
	}

	return *this;
  //## end ClSiteNodeInformation::operator=%959009687.body
}

m4bool_t ClSiteNodeInformation::IsAlive (m4uint32_t ai_iMaxStartRetries, m4uint32_t ai_iServerTimeout)
{
  //## begin ClSiteNodeInformation::IsAlive%963248410.body preserve=yes
	M4ClTimeStamp oCrono;
	m4double_t dTime, dElapseTime;
	m4double_t iseg;
	m4bool_t bIsAlive ;

	oCrono.now () ;

	oCrono.get ( dTime ) ;

	dElapseTime = dTime - m_tLastUpdate  ; //En días
	dElapseTime *= 24 ; //Se pasa a horas
	dElapseTime *= 60 ; //Se pasa a minutos
	dElapseTime *= 60 ; //Se pasa a segundos
	iseg = dElapseTime ;

	if (! ai_iServerTimeout )
		ai_iServerTimeout =  SERVER_STARTUP_TIMEOUT  ;


	if ( ! stricmp ( m_strNodeState.c_str() , STATE_STARTING ) )
	{
		bIsAlive = ( iseg > ai_iServerTimeout ) ? M4_FALSE : M4_TRUE ;

		if (! bIsAlive )
		{

			if ( m_iStartingtries  >= ai_iMaxStartRetries )
			{
				SetNodeState ( STATE_INACTIVE ) ;
				bIsAlive = M4_TRUE ;

				M4_SRV_ERROR (	M4_ERR_DPCH_STARTCONFIG_MAXRETRIES
					, "M4 Application Server '%0:s' did not started property after '%1:d' timeout/s of '%2:d' seconds. Maximun number of retries reached. Configuration inactive" ,
					m_strSiteNodeID.c_str() << m_iStartingtries<< SERVER_STARTUP_TIMEOUT  ) ;
			}
			else
			{
				M4_SRV_ERROR (	 M4_ERR_DPCH_STARTCONFIG_TIMEOUT 
					, "M4 Application Server '%0:s' did not started property after '%1:d' timeout/s of '%2:d' seconds. Restarting" ,
					m_strSiteNodeID.c_str() << m_iStartingtries << SERVER_STARTUP_TIMEOUT  ) ;
			}
		}
	}
	else
	{
		if (!stricmp(m_strNodeState.c_str(), STATE_HALTED))
		{
			bIsAlive = M4_FALSE;
		}
		else
		{
			bIsAlive = M4_TRUE;
		
			//	bIsAlive = ( iseg > SERVER_UPDATE_SLAPSED_TIME ) ? M4_FALSE : M4_TRUE ;
		}
	}

	if (! bIsAlive ) {
		M4_SRV_GENERAL( " Node %0:s does not reply", m_strSiteNodeID.c_str() ) ;
		
	} ;

	return bIsAlive ;
  //## end ClSiteNodeInformation::IsAlive%963248410.body
}

m4void_t ClSiteNodeInformation::Start ()
{
  //## begin ClSiteNodeInformation::Start%963302394.body preserve=yes
	M4ClTimeStamp oCrono;

	oCrono.now () ;

	oCrono.get ( m_tLastUpdate ) ;

	m_iStartingtries ++ ;
  //## end ClSiteNodeInformation::Start%963302394.body
}

m4return_t ClSiteNodeInformation::CalculateLoad (eDspClientTypes ai_iType)
{
  //## begin ClSiteNodeInformation::CalculateLoad%972387803.body preserve=yes
	ClSiteNodeRepository* pSiteRepository = NULL;
	m4int32_t iSessionsWeight, iSrvMemWeight, iSysMemWeight, iCPUWeight, iExecTimeWeight, iExecTimeThreshold;

	m4double_t dLoadFactor = 0;
	m4double_t dloadaux =0 ;

	// -- Recover sum weights.
	pSiteRepository		= ClSiteRepositorySingleton::GetInstance();
	iSessionsWeight		= pSiteRepository->GetSessionsWeight();
	iSrvMemWeight		= pSiteRepository->GetSrvMemWeight();
	iSysMemWeight		= pSiteRepository->GetSysMemWeight();
	iCPUWeight			= pSiteRepository->GetCPUWeight();
	iExecTimeWeight		= pSiteRepository->GetExecTimeWeight();
	iExecTimeThreshold	= pSiteRepository->GetExecTimeThreshold();

	dLoadFactor=0 ;
	
	dloadaux = iSessionsWeight * m_iNumSessions ;
	dloadaux /= 100 ;
	dLoadFactor+= dloadaux ;

	dloadaux = iSrvMemWeight * m_iServerMemory ;
	dloadaux /= 100 ;
	dLoadFactor+= dloadaux ;

	// eh 178555
	// Si queda menos memoria física de la que el proceso potencialmente puede crecer.
	if( m_dFreeMem > 0 && m_dFreeMem < m_dMaxProcSize - m_dPrivateMB )
	{
		dloadaux = iSysMemWeight * m_dPrivateMB / m_dFreeMem ;
		dloadaux /= 100 ;
		dLoadFactor+= dloadaux ;
	}

	dloadaux = iCPUWeight * m_dCPU ;
	dloadaux /= 100  ;
	dLoadFactor+= dloadaux ;
	
	if( ai_iType & CPP_CLIENT )
	{
		m_dCppLoadFactor = dLoadFactor;

		if( m_iOLTPTime > iExecTimeThreshold )
		{
			dloadaux = iExecTimeWeight * m_iOLTPTime;
			dloadaux /= 100;
			m_dCppLoadFactor += dloadaux;
		}
	}

	if( ai_iType & JAVA_CLIENT )
	{
		m_dJavaLoadFactor = dLoadFactor;

		if( m_iXMLTime > iExecTimeThreshold )
		{
			dloadaux = iExecTimeWeight * m_iXMLTime;
			dloadaux /= 100;
			m_dJavaLoadFactor += dloadaux;
		}
	}

	// -- only for debugging purposes ------------------------
	/*
	FILE* pfCandidate = fopen("..//temp//oltpcandidate","rt");
	if( pfCandidate != NULL )
	{
		static char acNode[256];
		acNode[0] = '\0';
		fscanf(pfCandidate,"%s", acNode);
		if( !stricmp(m_strSiteNodeID.c_str(),acNode) )
		{
			m_dCppLoadFactor = 0.01;
		}
		fclose(pfCandidate);
	}
	pfCandidate = fopen("..//temp//xmlcandidate","rt");
	if( pfCandidate != NULL )
	{
		static char acNode[256];
		acNode[0] = '\0';
		fscanf(pfCandidate,"%s", acNode);
		if( !stricmp(m_strSiteNodeID.c_str(),acNode) )
		{
			m_dJavaLoadFactor = 0.01;
		}
		fclose(pfCandidate);
	}
	*/
	// -- only for debugging purposes ------------------------


	return M4_SUCCESS ;
  //## end ClSiteNodeInformation::CalculateLoad%972387803.body
}

m4return_t ClSiteNodeInformation::GetProperty (const m4string_t& ai_szProperty, m4string_t &ai_szValue)
{
  //## begin ClSiteNodeInformation::GetProperty%1067431241.body preserve=yes
m4uint32_t iVar ;
m4char_t szVar [50] ;
m4double_t dVar ;

	if (! stricmp ( ai_szProperty.c_str() , "NODE_ID" ) )
	{
        ai_szValue = this->GetSiteNodeID();
	}
	else if ( ! stricmp ( ai_szProperty.c_str() , "NODE_HOST" ) )
	{
		ai_szValue = GetNodeHost () ;
	}
	else if ( ! stricmp ( ai_szProperty.c_str() , "BASE_PORT" ) )
	{
		iVar = GetBasePort () ;
		sprintf ( szVar , "%d" , iVar ) ;
		ai_szValue = szVar ;
	}
	else if ( ! stricmp ( ai_szProperty.c_str() , "NODE_STATE" ) )
	{
		ai_szValue = GetNodeState () ;
	}
	else if ( ! stricmp ( ai_szProperty.c_str() , "OLTP_LOAD_FACTOR" ) )
	{
		dVar = GetCppLoadFactor () ;
		sprintf ( szVar , "%f" , dVar ) ;
		ai_szValue = szVar ;
	}
	else if ( ! stricmp ( ai_szProperty.c_str() , "XML_LOAD_FACTOR" ) )
	{
		dVar = GetJavaLoadFactor () ;
		sprintf ( szVar , "%f" , dVar ) ;
		ai_szValue = szVar ;
	}
	else if ( ! stricmp ( ai_szProperty.c_str() , "SERVER_MEMORY" ) )
	{
		iVar = GetServerMemory () ;
		sprintf ( szVar , "%d" , iVar ) ;
		ai_szValue = szVar ;
	}
	else if ( ! stricmp ( ai_szProperty.c_str() , "NUM_SESSIONS" ) )
	{
		iVar = GetNumSessions () ;
		sprintf ( szVar , "%d" , iVar ) ;
		ai_szValue = szVar ;
	}
	else if ( ! stricmp ( ai_szProperty.c_str() , "SYSTEM_MEMORY" ) )
	{
		dVar = GetSystemMemory () ;
		sprintf ( szVar , "%l" , dVar ) ;
		ai_szValue = szVar ;
	}
	else if ( ! stricmp ( ai_szProperty.c_str() , "CPU_USAGE" ) )
	{
		dVar = GetCPUUsage () ;
		sprintf ( szVar , "%f" , dVar ) ;
		ai_szValue = szVar ;
	}
	else if ( ! stricmp ( ai_szProperty.c_str() , "OLTP_TIME" ) )
	{
		iVar = GetOLTPTime () ;
		sprintf ( szVar , "%d" , iVar ) ;
		ai_szValue = szVar ;
	}
	else if ( ! stricmp ( ai_szProperty.c_str() , "XML_TIME" ) )
	{
		iVar = GetXMLTime () ;
		sprintf ( szVar , "%d" , iVar ) ;
		ai_szValue = szVar ;
	}
	else
		return M4_ERROR ;

	return M4_SUCCESS ;
  //## end ClSiteNodeInformation::GetProperty%1067431241.body
}

M4ISnmpObject* ClSiteNodeInformation::GetSnmpObject ()
{
  //## begin ClSiteNodeInformation::GetSnmpObject%1084868904.body preserve=yes
    return m_pM4ISnmpObject;
  //## end ClSiteNodeInformation::GetSnmpObject%1084868904.body
}

m4void_t ClSiteNodeInformation::SetSnmpObject (M4ISnmpObject* ai_poSnmpObject)
{
  //## begin ClSiteNodeInformation::SetSnmpObject%1084868905.body preserve=yes
    m_pM4ISnmpObject = ai_poSnmpObject;
  //## end ClSiteNodeInformation::SetSnmpObject%1084868905.body
}

// Additional Declarations
  //## begin ClSiteNodeInformation%3923B60D0267.declarations preserve=yes
  //## end ClSiteNodeInformation%3923B60D0267.declarations

// Class ClSiteNodeRepository 



// eh 178555
FILE* ClSiteNodeRepository::pLoadTraceFile = NULL ;




ClSiteNodeRepository::ClSiteNodeRepository ()
  //## begin ClSiteNodeRepository::ClSiteNodeRepository%958658604.hasinit preserve=no
      : m_uiAdminPort(0)
  //## end ClSiteNodeRepository::ClSiteNodeRepository%958658604.hasinit
  //## begin ClSiteNodeRepository::ClSiteNodeRepository%958658604.initialization preserve=yes
  //## end ClSiteNodeRepository::ClSiteNodeRepository%958658604.initialization
{
  //## begin ClSiteNodeRepository::ClSiteNodeRepository%958658604.body preserve=yes
	m_oSynchronizationObj.Init();
	m_oCandidateJavaIterator = m_oCandidateCppIterator = m_oSiteNodeContainer.end();
	m_iServerTimeout = SERVER_STARTUP_TIMEOUT ;
  //## end ClSiteNodeRepository::ClSiteNodeRepository%958658604.body
}



//## Other Operations (implementation)
m4return_t ClSiteNodeRepository::GetCandidateNode (ClSiteNodeInformation &ao_oSiteNode, eDspClientTypes ai_iClientType)
{
  //## begin ClSiteNodeRepository::GetCandidateNode%958643330.body preserve=yes
	TSiteNodeMap::iterator*	pCandidateIterator;
	ClMutBlock oBlocker(m_oSynchronizationObj);

	
	if( ai_iClientType == CPP_CLIENT )
	{
		pCandidateIterator = &m_oCandidateCppIterator;
	}
	else
	{
		pCandidateIterator = &m_oCandidateJavaIterator;
	}


	if( *pCandidateIterator == m_oSiteNodeContainer.end() )
	{
		M4_SRV_ERROR( M4_ERR_DPCH_NOCANDIDATESITENODE,
			"Error selecting site node to the App. client. Cannot get a candidate site node for '%d' client type.", ai_iClientType );

		ao_oSiteNode = ClSiteNodeInformation::M4NullSite;
	}
	else
	{
		ao_oSiteNode = (**pCandidateIterator).second;
	}
	return M4_SUCCESS;
  //## end ClSiteNodeRepository::GetCandidateNode%958643330.body
}

m4return_t ClSiteNodeRepository::AddNode (const ClSiteNodeInformation &ai_oSiteNode)
{
  //## begin ClSiteNodeRepository::AddNode%958643331.body preserve=yes
	ClMutBlock oBlocker(m_oSynchronizationObj);
    m4return_t retCode = M4_ERROR;

    // insert this node in snmp repository if does not exist in the container.
    ClSiteNodeInformation sni = ai_oSiteNode;
    M4ISnmpAdministrator* poSnmpAdmin = NULL;
    M4ISnmpObject* poSnmpSubsystem = NULL;
	TSiteNodeMap::iterator it;
    it = m_oSiteNodeContainer.find(sni.GetSiteNodeMapKey());
    if (m_oSiteNodeContainer.end() == it)
    {
        poSnmpAdmin = M4SnmpAdminInstance::Get();
        if (NULL != poSnmpAdmin)
        {
            poSnmpSubsystem = poSnmpAdmin->RegisterSubsystem(NULL, new M4DspApi4Snmp(sni, this), "appserverconf", sni.GetSiteNodeID().c_str());
        }
        sni.SetSnmpObject(poSnmpSubsystem);
    }
        
    // add this SiteNodeInfo to the container.
    retCode = _AddNode (sni);
    if (M4_ERROR == retCode)
	{
		return M4_ERROR;
	}

	M4_SRV_GENERAL(  "Add Site Node ==> node: %0:s state: %1:s  loadOLTP: %2:s  loadXML: %3:s ",
		ai_oSiteNode.GetSiteNodeMapKey().c_str() << ai_oSiteNode.GetNodeState().c_str() << 
		ai_oSiteNode.GetCppLoadFactor() << ai_oSiteNode.GetJavaLoadFactor()  ) ;

    // register node properties in snmp.
    m4int32_t propertyIndex = 0;
    if ((NULL != poSnmpAdmin) && (NULL != poSnmpSubsystem))
    {
        poSnmpAdmin->RegisterProperty("node_id", propertyIndex++, poSnmpSubsystem, NULL);
        poSnmpAdmin->RegisterProperty("node_host", propertyIndex++, poSnmpSubsystem, NULL);
        poSnmpAdmin->RegisterProperty("base_port", propertyIndex++, poSnmpSubsystem, NULL);
        poSnmpAdmin->RegisterProperty("node_state", propertyIndex++, poSnmpSubsystem, NULL);
        poSnmpAdmin->RegisterProperty("oltp_load_factor", propertyIndex++, poSnmpSubsystem, NULL);
		poSnmpAdmin->RegisterProperty("xml_load_factor", propertyIndex++, poSnmpSubsystem, NULL);
        poSnmpAdmin->RegisterProperty("server_memory", propertyIndex++, poSnmpSubsystem, NULL);
        poSnmpAdmin->RegisterProperty("num_sessions", propertyIndex++, poSnmpSubsystem, NULL);
        poSnmpAdmin->RegisterProperty("system_memory", propertyIndex++, poSnmpSubsystem, NULL);
        poSnmpAdmin->RegisterProperty("cpu_usage", propertyIndex++, poSnmpSubsystem, NULL);
		poSnmpAdmin->RegisterProperty("oltp_time", propertyIndex++, poSnmpSubsystem, NULL);
		poSnmpAdmin->RegisterProperty("xml_time", propertyIndex++, poSnmpSubsystem, NULL);
    }


	return M4_SUCCESS;
  //## end ClSiteNodeRepository::AddNode%958643331.body
}

m4return_t ClSiteNodeRepository::UpdateNode (ClSiteNodeInformation &ai_oSiteNode)
{
  //## begin ClSiteNodeRepository::UpdateNode%958643332.body preserve=yes
	TSiteNodeMap::iterator					mapIter;
	m4bool_t bMustUpdate = M4_FALSE;
	m4double_t dLastLoadCppClients = 9999999 ;
	m4double_t dLastLoadJavaClients = 9999999 ;
	m4return_t R = M4_SUCCESS;


	// eh 197951
	ClDpchMain* poDpchMain = ClDpchMainSingleton::Instance()->GetDpchMain();
	ClSiteHealthVerifier* pVerifier = poDpchMain->m_pSiteHealthVerifier;

	if( pVerifier != NULL )
	{
		if( !stricmp( ai_oSiteNode.GetNodeState().c_str(), STATE_READY ) )
		{
			// En caso de reiniciar el dispatcher y estando los servidores levantados, pueden llegar 
			// notificaciones en las que su SiteNodeInfo aún no tiene usuario y contraseña. 
			// Las descartamos hasta que en el repositorio se haya actualizado toda la información.
			if( ai_oSiteNode.GetUserName() != "" )
			{
				poDpchMain->m_pSiteHealthVerifier->AddServerMonitor( ai_oSiteNode );
			}
		}
		else
		{
			poDpchMain->m_pSiteHealthVerifier->RemoveServerMonitor( ai_oSiteNode );
			
			// Resetea el flag de servidor no operativo.
			if ( !stricmp( ai_oSiteNode.GetNodeState().c_str(), STATE_STARTING ) || !stricmp( ai_oSiteNode.GetNodeState().c_str() , STATE_SHUTING_DOWN ) || !stricmp( ai_oSiteNode.GetNodeState().c_str() , STATE_SHUTING_DOWN_MC ) )
			{
				ai_oSiteNode.SetDiscarded( M4_FALSE );
			}
		}
	}

	
	ClMutBlock oBlocker(m_oSynchronizationObj);


	if (m_oSiteNodeContainer.empty())
	{
		return _AddNode ( ai_oSiteNode ) ;
	}


	if( m_oCandidateCppIterator != m_oSiteNodeContainer.end() )
		dLastLoadCppClients = (*m_oCandidateCppIterator).second.GetCppLoadFactor();

	if( m_oCandidateJavaIterator != m_oSiteNodeContainer.end() )
		dLastLoadJavaClients = (*m_oCandidateJavaIterator).second.GetJavaLoadFactor();

	mapIter = m_oSiteNodeContainer.find(ai_oSiteNode.GetSiteNodeMapKey());

	if (m_oSiteNodeContainer.end() == mapIter)
	{
		return _AddNode ( ai_oSiteNode ) ;
	}

	if ( M4_ERROR == (*mapIter).second.UpdateInformation(ai_oSiteNode) ) 
	{
		return M4_ERROR ;
	}

	if ( m_oCandidateCppIterator == mapIter )
	{
		// bg 235866
		if ( stricmp ( (*m_oCandidateCppIterator).second.GetNodeState().c_str() , STATE_READY ) || dLastLoadCppClients < ai_oSiteNode.GetCppLoadFactor() )
		{
			bMustUpdate = M4_TRUE ;
		}
	}
	else
	{
		if (dLastLoadCppClients > ai_oSiteNode.GetCppLoadFactor() && ai_oSiteNode.GetDiscarded() == M4_FALSE )
		{
			bMustUpdate = M4_TRUE ;
		}
	}

	if( bMustUpdate == M4_FALSE)
	{
		if ( m_oCandidateJavaIterator == mapIter )
		{
			// bg 235866
			if ( stricmp ( (*m_oCandidateJavaIterator).second.GetNodeState().c_str() , STATE_READY ) || dLastLoadJavaClients < ai_oSiteNode.GetJavaLoadFactor() )
			{
				bMustUpdate = M4_TRUE ;
			}
		}
		else
		{
			if (dLastLoadJavaClients > ai_oSiteNode.GetJavaLoadFactor() && ai_oSiteNode.GetDiscarded() == M4_FALSE )
			{
				bMustUpdate = M4_TRUE ;
			}
		}
	}

	if ( bMustUpdate ) {
		M4_SRV_DETAILED(  "Update Site Node ==>  node: %0:s state: %1:s  loadOLTP: %2:s  loadXML: %3:s ",
			ai_oSiteNode.GetSiteNodeMapKey().c_str() << ai_oSiteNode.GetNodeState().c_str() << 
			ai_oSiteNode.GetCppLoadFactor() << ai_oSiteNode.GetJavaLoadFactor()  ) ;

		R =  _FindCandidateNode( &m_oCandidateCppIterator, &m_oCandidateJavaIterator );
		if ( R==M4_ERROR ) {
			M4_SRV_ERROR_N(M4_ERR_DPCH_NOFINDSITENODE,
				"Error finding candidate site node. There is not any site node selected." );
		} ;
	} ;

	// eh 178555
	m4bool_t bIsCppCandidate = M4_FALSE ;
	m4bool_t bIsJavaCandidate = M4_FALSE ;

	if ( m_oCandidateCppIterator == mapIter )
	{
		bIsCppCandidate = M4_TRUE ;
	}
	if ( m_oCandidateJavaIterator == mapIter )
	{
		bIsJavaCandidate = M4_TRUE ;
	}

	_DumpLoadTrace( "SRV_NOTIFICATION", (*mapIter).second, bIsCppCandidate, bIsJavaCandidate ) ;


	return R ;
  //## end ClSiteNodeRepository::UpdateNode%958643332.body
}

m4return_t ClSiteNodeRepository::RemoveNode (ClSiteNodeInformation &ai_oSiteNode)
{
  //## begin ClSiteNodeRepository::RemoveNode%958658603.body preserve=yes

	ClMutBlock oBlocker(m_oSynchronizationObj);


	M4_SRV_GENERAL(  "Remove Site Node ==>  node: %0:s state: %1:s  loadOLTP: %2:s  loadXML: %3:s ",
		ai_oSiteNode.GetSiteNodeMapKey().c_str() << ai_oSiteNode.GetNodeState().c_str() << 
		ai_oSiteNode.GetCppLoadFactor() << ai_oSiteNode.GetJavaLoadFactor()  ) ;

    /*
    // remove this node in snmp repository.
    M4ISnmpAdministrator* poSnmpAdmin = M4SnmpAdminInstance::Get();
    M4ISnmpObject* poSnmpSubsystem = ai_oSiteNode.GetSnmpObject();
    if ((NULL != poSnmpAdmin) && (NULL != poSnmpSubsystem))
    {
        poSnmpAdmin->UnregisterSubsystem(poSnmpSubsystem);
    }
    */

    // the remove operation only set the state of this node to disabled.
    // This allow us to see in the administrator all configurations (stopped and running)
    ai_oSiteNode.SetIsEnabled(M4_FALSE);
    return _AddNode(ai_oSiteNode);
  //## end ClSiteNodeRepository::RemoveNode%958658603.body
}

m4return_t ClSiteNodeRepository::_DumpLoadTrace(m4pcchar_t ai_pccEvent, const ClSiteNodeInformation &ai_oSiteNode, m4bool_t ai_bIsCppCandidate, m4bool_t ai_bIsJavaCandidate)
{
	static m4char_t acHeader[] = "Time\tEvent\tServerHost\tServerInstance\tNumSessions\tServerMemory(%%)\tSystemMemory(%%)\tCPU\tRunningLowMem\tServerUsedMemory\tFreePhysMemory\tProcSizeLimit\tOLTPTime\tXMLTime\tSessionsWeight\tSrvWeight\tSysWeight\tCPUWeight\tExecTimeWeight\tExecTimeThreshold\tOLTPLoadFactor\tXMLLoadFactor\tIsOLTPCandidate\tIsXMLCandidate\n" ;


	if( m_iDumpLoadTrace == M4_FALSE )
	{
		return M4_SUCCESS ;
	}

	
	#ifdef _WINDOWS
		static m4char_t acFormat[] = "%I64d\t%s\t%s\t%s\t%d\t%d\t%f\t%f\t%c\t%f\t%f\t%f\t%I64d\t%I64d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%f\t%s\t%s\n" ;
	#else
		static m4char_t acFormat[] = "%lld\t%s\t%s\t%s\t%d\t%d\t%f\t%f\t%c\t%f\t%f\t%f\t%lld\t%lld\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%f\t%s\t%s\n" ;
	#endif
	
	if( pLoadTraceFile == NULL )
	{
		pLoadTraceFile = fopen("../temp/loadfactor.log", "wt") ;

		if( pLoadTraceFile == NULL )
		{
			return M4_ERROR;
		}

		M4ClTimeStamp	oTime;
		M4ClString		sTime;
		oTime.now ();
		sTime = oTime;
		fprintf( pLoadTraceFile, "%s\n", (char*)sTime );

		fprintf(pLoadTraceFile, acHeader) ;
	}
	else
	{
		pLoadTraceFile = fopen("../temp/loadfactor.log", "at") ;

		if( pLoadTraceFile == NULL )
		{
			return M4_ERROR;
		}
	}
	

	m4char_t	cRunningLowMem = '0' ;
	if( ai_oSiteNode.GetFreeMem() > 0 && ai_oSiteNode.GetFreeMem() < ai_oSiteNode.GetMaxProcSize() - ai_oSiteNode.GetPrivateMB() )
	{
		cRunningLowMem = '1' ;
	}

	fprintf( pLoadTraceFile, acFormat, ClPrecisionTicker::GetTimeTick(), ai_pccEvent, ai_oSiteNode.GetNodeHost().c_str(), ai_oSiteNode.GetSiteNodeID().c_str(), ai_oSiteNode.GetNumSessions(), ai_oSiteNode.GetServerMemory(), ai_oSiteNode.GetSystemMemory(), ai_oSiteNode.GetCPUUsage(), cRunningLowMem, ai_oSiteNode.GetPrivateMB(), ai_oSiteNode.GetFreeMem(), ai_oSiteNode.GetMaxProcSize(), ai_oSiteNode.GetOLTPTime(), ai_oSiteNode.GetXMLTime(), m_iSessionsWeight, m_iSrvMemWeight, m_iSysMemWeight, m_iCPUWeight, m_iExecTimeWeight, m_iExecTimeThreshold, ai_oSiteNode.GetCppLoadFactor(), ai_oSiteNode.GetJavaLoadFactor(), (ai_bIsCppCandidate == M4_TRUE)? "1": "0", (ai_bIsJavaCandidate == M4_TRUE)? "1": "0" );
	fclose(pLoadTraceFile) ;


	return M4_SUCCESS ;
}

m4return_t ClSiteNodeRepository::EraseNode ( ClSiteNodeInformation &ai_oSiteNode )
{
	m4return_t iRet = M4_ERROR;	

	iRet = _RemoveNode ( ai_oSiteNode );

	return iRet;
}

m4return_t ClSiteNodeRepository::UpdateDiscardedNode(ClSiteNodeInformation &ai_oSiteNode)
{
	TSiteNodeMap::iterator	mapIter;
	ClMutBlock oBlocker( m_oSynchronizationObj );

	mapIter = m_oSiteNodeContainer.find( ai_oSiteNode.GetSiteNodeMapKey() );

	if( m_oSiteNodeContainer.end() != mapIter )
	{
		(*mapIter).second.SetDiscarded( ai_oSiteNode.GetDiscarded() );
	}
	else
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClSiteNodeRepository::UpdateOLTPTimeNode(ClSiteNodeInformation &ai_oSiteNode)
{
	TSiteNodeMap::iterator	mapIter;
	ClMutBlock				oBlocker( m_oSynchronizationObj );
	m4return_t				R;
	m4bool_t				bIsCppCandidate = M4_FALSE;
	m4bool_t				bIsJavaCandidate = M4_FALSE;
	m4double_t				dLastLoadCppClients = 9999999 ;


	mapIter = m_oSiteNodeContainer.find( ai_oSiteNode.GetSiteNodeMapKey() );

	if( m_oSiteNodeContainer.end() != mapIter )
	{
		if( (*mapIter).second.GetOLTPTime() != ai_oSiteNode.GetOLTPTime() )
		{
			if( m_oCandidateCppIterator != m_oSiteNodeContainer.end() )
			{
				dLastLoadCppClients = (*m_oCandidateCppIterator).second.GetCppLoadFactor();
			}

			// -- Update executor time.
			(*mapIter).second.SetOLTPTime( ai_oSiteNode.GetOLTPTime() );
			
			// -- Calculate OLTP load factor.
			(*mapIter).second.CalculateLoad( CPP_CLIENT );

			// -- No es necesario comprobar que sea un servidor opertativo (!discarded) porque
			// -- en ese caso no se lanza la recolección de tiempos de los ejecutores.
			if( dLastLoadCppClients > (*mapIter).second.GetCppLoadFactor() )
			{
				M4_SRV_DETAILED(  "Update Site Node (oltp exec) ==>  node: %0:s state: %1:s  loadOLTP: %2:s  loadXML: %3:s ",
					(*mapIter).second.GetSiteNodeMapKey().c_str() << (*mapIter).second.GetNodeState().c_str() << 
					(*mapIter).second.GetCppLoadFactor() << (*mapIter).second.GetJavaLoadFactor()  ) ;

				R =  _FindCandidateNode( &m_oCandidateCppIterator, NULL );

				if ( R==M4_ERROR ) {
					M4_SRV_ERROR_N(M4_ERR_DPCH_NOFINDSITENODE,
						"Error finding candidate site node. There is not any site node selected." );
				} ;
			}

			if ( m_oCandidateCppIterator == mapIter )
			{
				bIsCppCandidate = M4_TRUE ;
			}
			if ( m_oCandidateJavaIterator == mapIter )
			{
				bIsJavaCandidate = M4_TRUE ;
			}

			_DumpLoadTrace( "OLTP_EXEC_TIME", (*mapIter).second, bIsCppCandidate, bIsJavaCandidate ) ;
		}
	}
	else
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClSiteNodeRepository::UpdateXMLTimeNode(ClSiteNodeInformation &ai_oSiteNode)
{
	TSiteNodeMap::iterator	mapIter;
	ClMutBlock				oBlocker( m_oSynchronizationObj );
	m4return_t				R;
	m4bool_t				bIsCppCandidate = M4_FALSE;
	m4bool_t				bIsJavaCandidate = M4_FALSE;
	m4double_t				dLastLoadJavaClients = 9999999 ;


	mapIter = m_oSiteNodeContainer.find( ai_oSiteNode.GetSiteNodeMapKey() );

	if( m_oSiteNodeContainer.end() != mapIter )
	{
		if( (*mapIter).second.GetXMLTime() != ai_oSiteNode.GetXMLTime() )
		{
			if( m_oCandidateJavaIterator != m_oSiteNodeContainer.end() )
			{
				dLastLoadJavaClients = (*m_oCandidateJavaIterator).second.GetJavaLoadFactor();
			}

			// -- Update executor time.
			(*mapIter).second.SetXMLTime( ai_oSiteNode.GetXMLTime() );
			
			// -- Calculate XML load factor.
			(*mapIter).second.CalculateLoad( JAVA_CLIENT );

			// -- No es necesario comprobar que sea un servidor opertativo (!discarded) porque
			// -- en ese caso no se lanza la recolección de tiempos de los ejecutores.
			if( dLastLoadJavaClients > (*mapIter).second.GetJavaLoadFactor() )
			{
				M4_SRV_DETAILED(  "Update Site Node (xml exec) ==>  node: %0:s state: %1:s  loadOLTP: %2:s  loadXML: %3:s ",
					(*mapIter).second.GetSiteNodeMapKey().c_str() << (*mapIter).second.GetNodeState().c_str() << 
					(*mapIter).second.GetCppLoadFactor() << (*mapIter).second.GetJavaLoadFactor()  ) ;

				R =  _FindCandidateNode( NULL, &m_oCandidateJavaIterator );

				if ( R==M4_ERROR ) {
					M4_SRV_ERROR_N(M4_ERR_DPCH_NOFINDSITENODE,
						"Error finding candidate site node. There is not any site node selected." );
				} ;
			}

			if ( m_oCandidateCppIterator == mapIter )
			{
				bIsCppCandidate = M4_TRUE ;
			}
			if ( m_oCandidateJavaIterator == mapIter )
			{
				bIsJavaCandidate = M4_TRUE ;
			}

			_DumpLoadTrace( "XML_EXEC_TIME", (*mapIter).second, bIsCppCandidate, bIsJavaCandidate ) ;
		}
	}
	else
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4size_t ClSiteNodeRepository::Count ()
{
  //## begin ClSiteNodeRepository::Count%958658605.body preserve=yes
	ClMutBlock oBlocker(m_oSynchronizationObj);

	return m_oSiteNodeContainer.size();
  //## end ClSiteNodeRepository::Count%958658605.body
}

void ClSiteNodeRepository::Clear ()
{
  //## begin ClSiteNodeRepository::Clear%958658606.body preserve=yes
	ClMutBlock oBlocker(m_oSynchronizationObj);

	m_oSiteNodeContainer.clear();
  //## end ClSiteNodeRepository::Clear%958658606.body
}

m4return_t ClSiteNodeRepository::_FindCandidateNode (TSiteNodeMap::iterator* ao_pCppIterator, TSiteNodeMap::iterator* ao_pJavaIterator)
{
  //## begin ClSiteNodeRepository::_FindCandidateNode%959009680.body preserve=yes
	m4bool_t				bFindCpp = M4_FALSE;
	m4bool_t				bFindJava = M4_FALSE;
	TSiteNodeMap::iterator	oRepositoryIterator;
	TSiteNodeMap::iterator	oEndIter = m_oSiteNodeContainer.end() ;
	

	if (m_oSiteNodeContainer.empty())
	{
		return M4_ERROR;
	}

	if( ao_pCppIterator != NULL )
	{
		bFindCpp = M4_TRUE;
		*ao_pCppIterator = oEndIter ;
	}

	if( ao_pJavaIterator != NULL )
	{
		bFindJava = M4_TRUE;
		*ao_pJavaIterator = oEndIter ;
	}


	for (	oRepositoryIterator = m_oSiteNodeContainer.begin();
			oRepositoryIterator != m_oSiteNodeContainer.end();
			oRepositoryIterator++
		)
			{
				if ( (*oRepositoryIterator).second.GetNodeState () == STATE_READY && (*oRepositoryIterator).second.GetDiscarded() == M4_FALSE )
				{

					if( bFindCpp == M4_TRUE )
					{
						if ( *ao_pCppIterator == oEndIter )
						{
							*ao_pCppIterator = oRepositoryIterator ;
						}
						else
						{
							if( (*oRepositoryIterator).second.GetCppLoadFactor() < (**ao_pCppIterator).second.GetCppLoadFactor() )
								*ao_pCppIterator = oRepositoryIterator;
						}
					}

					if( bFindJava == M4_TRUE )
					{
						if ( *ao_pJavaIterator == oEndIter )
						{
							*ao_pJavaIterator = oRepositoryIterator ;
						}
						else
						{
							if( (*oRepositoryIterator).second.GetJavaLoadFactor() < (**ao_pJavaIterator).second.GetJavaLoadFactor() )
								*ao_pJavaIterator = oRepositoryIterator;
						}
					}

				}
			}

	return M4_SUCCESS;
  //## end ClSiteNodeRepository::_FindCandidateNode%959009680.body
}

m4return_t ClSiteNodeRepository::GetNodeList (ClSiteNodeContainer &ai_oSiteNodeList)
{
  //## begin ClSiteNodeRepository::GetNodeList%961054358.body preserve=yes
	TSiteNodeMap::iterator oRepositoryIterator;


	ClMutBlock oBlocker(m_oSynchronizationObj);

	if (m_oSiteNodeContainer.empty())
	{
		return M4_SUCCESS;
	}

	for (	oRepositoryIterator = m_oSiteNodeContainer.begin();
			oRepositoryIterator != m_oSiteNodeContainer.end();
			oRepositoryIterator++
		)
		{
			ai_oSiteNodeList.AddNode( (*oRepositoryIterator).second ) ;
		}


	return M4_SUCCESS ;
  //## end ClSiteNodeRepository::GetNodeList%961054358.body
}

m4bool_t ClSiteNodeRepository::IsConsistent (ClSiteNodeContainer &ao_oRemovableNodes)
{
  //## begin ClSiteNodeRepository::IsConsistent%963302387.body preserve=yes
	TSiteNodeMap::iterator oRepositoryIterator;
	m4bool_t bIsConsistent = M4_TRUE ;


	ClMutBlock oBlocker(m_oSynchronizationObj);

	if (m_oSiteNodeContainer.empty())
		return M4_TRUE ;

	for (	oRepositoryIterator = m_oSiteNodeContainer.begin();
			oRepositoryIterator != m_oSiteNodeContainer.end();
			oRepositoryIterator++
		)
			{
                if ((*oRepositoryIterator).second.GetIsEnabled())
				{
				    if ( ! (*oRepositoryIterator).second.IsAlive ( m_iMaxStartRetries , m_iServerTimeout )  )
                    {
					    ao_oRemovableNodes.AddNode ( (*oRepositoryIterator).second ) ;
					    bIsConsistent = M4_FALSE ;
                    }
				}
			}

	return bIsConsistent ;
  //## end ClSiteNodeRepository::IsConsistent%963302387.body
}

m4return_t ClSiteNodeRepository::RemoveNodeList (ClSiteNodeContainer &ai_oNodeList)
{
  //## begin ClSiteNodeRepository::RemoveNodeList%963302388.body preserve=yes
	ClSiteNodeInformation oSiteNodeInfo ;

	M4_SRV_GENERAL_N( "Remove Node List" ) ;

	while ( M4_SUCCESS == ai_oNodeList.GetNextNode( oSiteNodeInfo ) )
	{
		M4_SRV_GENERAL(  "Remove Site Node ==>  node: %0:s state: %1:s  loadOLTP: %2:s  loadXML: %3:s ",
			oSiteNodeInfo.GetSiteNodeMapKey().c_str() << oSiteNodeInfo.GetNodeState().c_str() << 
			oSiteNodeInfo.GetCppLoadFactor() << oSiteNodeInfo.GetJavaLoadFactor()  ) ;

		RemoveNode ( oSiteNodeInfo ) ;
	}

	_FindCandidateNode( &m_oCandidateCppIterator, &m_oCandidateJavaIterator);

	return M4_SUCCESS ;
  //## end ClSiteNodeRepository::RemoveNodeList%963302388.body
}

m4return_t ClSiteNodeRepository::_AddNode (const ClSiteNodeInformation &ai_oSiteNode)
{
  //## begin ClSiteNodeRepository::_AddNode%964092454.body preserve=yes
	TSiteNodeMap::iterator					Iterator ;
	pair<TSiteNodeMap::iterator, bool>		mapRes;
	TSiteNodeMap::value_type				mapEntry (ai_oSiteNode.GetSiteNodeMapKey(), ai_oSiteNode);
	m4return_t  R ;



	Iterator = m_oSiteNodeContainer.find ( ai_oSiteNode.GetSiteNodeMapKey() ) ;

	if ( Iterator != m_oSiteNodeContainer.end () )
	{
		/*
        if ( stricmp ( (* Iterator).second.GetNodeState ().c_str() , "inactive" ) )
		{
			return M4_ERROR ;
		}
		else
		{
            m_oSiteNodeContainer.erase ( Iterator) ;
		}
        */

        m_oSiteNodeContainer.erase ( Iterator) ;
	}

	mapRes = m_oSiteNodeContainer.insert(mapEntry);

	Iterator = m_oSiteNodeContainer.find ( ai_oSiteNode.GetSiteNodeMapKey() ) ;


				
	if (M4_FALSE == mapRes.second)
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADDSITENODE,
			"Error adding site node. Cannot add the new site node %0:s.",
			ai_oSiteNode.GetSiteNodeMapKey().c_str() );

		return M4_ERROR;
	}

	R = _FindCandidateNode( &m_oCandidateCppIterator, &m_oCandidateJavaIterator );
	if (R == M4_ERROR ) {
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOFINDSITENODE,
			"Error finding candidate site node. There is not any site node selected." );
	} ;

	return R  ;
  //## end ClSiteNodeRepository::_AddNode%964092454.body
}

m4return_t ClSiteNodeRepository::_RemoveNode (const ClSiteNodeInformation &ai_oSiteNode)
{
  //## begin ClSiteNodeRepository::_RemoveNode%964092455.body preserve=yes
	TSiteNodeMap::iterator					mapIter;


	if ( m_oSiteNodeContainer.end() == (mapIter = m_oSiteNodeContainer.find(ai_oSiteNode.GetSiteNodeMapKey())))
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOREMOVESITENODE,
			"Error removing site node. Cannot find the site node to remove." );

		return M4_ERROR;
	}

    m_oSiteNodeContainer.erase(mapIter);

	_FindCandidateNode( &m_oCandidateCppIterator, &m_oCandidateJavaIterator );

	return M4_SUCCESS;
  //## end ClSiteNodeRepository::_RemoveNode%964092455.body
}

m4return_t ClSiteNodeRepository::GetNodeInfo (ClSiteNodeInformation &aio_oSiteNode)
{
  //## begin ClSiteNodeRepository::GetNodeInfo%965823269.body preserve=yes
	TSiteNodeMap::iterator					mapIter;


	if ( m_oSiteNodeContainer.end() == (mapIter = m_oSiteNodeContainer.find(aio_oSiteNode.GetSiteNodeMapKey())))
	{
		M4_SRV_ERROR (M4_ERR_DPCH_NOGETSITENODEINFO,
			"Error getting site node information. No such Configuration : '%0:s'." , aio_oSiteNode.GetSiteNodeMapKey().c_str() );
		
		return M4_ERROR;
	}

	aio_oSiteNode = (*mapIter).second ;

	return M4_SUCCESS ;
  //## end ClSiteNodeRepository::GetNodeInfo%965823269.body
}

// Additional Declarations
  //## begin ClSiteNodeRepository%3923B6990240.declarations preserve=yes
  //## end ClSiteNodeRepository%3923B6990240.declarations

// Class ClSiteNodeContainer 



//## Other Operations (implementation)
m4void_t ClSiteNodeContainer::AddNode (ClSiteNodeInformation &ai_oSiteNode)
{
  //## begin ClSiteNodeContainer::AddNode%961054356.body preserve=yes
	m_oSiteNodeList.push_back ( ai_oSiteNode ) ;
  //## end ClSiteNodeContainer::AddNode%961054356.body
}

m4return_t ClSiteNodeContainer::GetNextNode (ClSiteNodeInformation &ao_oSiteNode)
{
  //## begin ClSiteNodeContainer::GetNextNode%961054357.body preserve=yes

	if( m_oSiteNodeList.empty ( ) )
	{
		//No lanzar mensaje de error: el que sea o no un error depende de para que se llame
		return M4_ERROR ;
	}

	m4uint32_t size  = m_oSiteNodeList.size () ;

	ao_oSiteNode = m_oSiteNodeList.back ( ) ;

	m_oSiteNodeList.pop_back ( ) ;
	

	return M4_SUCCESS ;
  //## end ClSiteNodeContainer::GetNextNode%961054357.body
}

m4bool_t ClSiteNodeContainer::FindNodeConnectionID (m4uint32_t ai_iConnectionID, ClSiteNodeInformation &ao_oSiteNode)
{
  //## begin ClSiteNodeContainer::FindNodeConnectionID%969372805.body preserve=yes

	m4bool_t bResult = M4_FALSE;
	m4uint32_t size  = m_oSiteNodeList.size () ;


	if( size > 0 )
	{
		ClSiteNodeList::const_iterator i = m_oSiteNodeList.begin();
		while (i != m_oSiteNodeList.end())
		{
			if (ai_iConnectionID == (*i).GetConnectionID())
			{
				ao_oSiteNode = *i;
				bResult = M4_TRUE;
				break;
			}
			i++;
		}
	}
	
	return bResult;

  //## end ClSiteNodeContainer::FindNodeConnectionID%969372805.body
}

// Additional Declarations
  //## begin ClSiteNodeContainer%3948D8AE0368.declarations preserve=yes
  //## end ClSiteNodeContainer%3948D8AE0368.declarations

// Class ClSiteRepositorySingleton 

//## begin ClSiteRepositorySingleton::m_poRepository%3A12A9980163.role preserve=no  private: static ClSiteNodeRepository { -> RHAN}
ClSiteNodeRepository *ClSiteRepositorySingleton::m_poRepository = NULL;
//## end ClSiteRepositorySingleton::m_poRepository%3A12A9980163.role


//## Other Operations (implementation)
ClSiteNodeRepository * ClSiteRepositorySingleton::GetInstance ()
{
  //## begin ClSiteRepositorySingleton::GetInstance%974303782.body preserve=yes
	if ( ! m_poRepository ) {
		M4_MDBG_NEW(  m_poRepository, m_poRepository = new ClSiteNodeRepository ) ;
	} ;
	return m_poRepository ;
  //## end ClSiteRepositorySingleton::GetInstance%974303782.body
}

void ClSiteRepositorySingleton::Destroy ()
{
  //## begin ClSiteRepositorySingleton::Destroy%974303783.body preserve=yes
	M4_MDBG_DELETE( m_poRepository, delete m_poRepository ) ;
	m_poRepository=NULL ;
  //## end ClSiteRepositorySingleton::Destroy%974303783.body
}

// Additional Declarations
  //## begin ClSiteRepositorySingleton%3A12A8610101.declarations preserve=yes
  //## end ClSiteRepositorySingleton%3A12A8610101.declarations

//## begin module%3923BC11017B.epilog preserve=yes
//## end module%3923BC11017B.epilog
