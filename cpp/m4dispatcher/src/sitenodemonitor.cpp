#include <sitenodemonitor.hpp>
#include <clienttag.hpp>
#include <m4clock.hpp>
#include <blocksynchronization.hpp>
#include <stdarg.h>
#include <m4cryptc.hpp>
#include <m4log.hpp>
#include <m4dpchres.hpp>
#include <logsys.hpp>
#include "datastorages.hpp"



// -----------------------------------------------------------------------------------------------------------------------------
// -- Class ClRunningSrvMonitor
// -----------------------------------------------------------------------------------------------------------------------------


// -- Static class members
FILE*				ClRunningSrvMonitor::s_pfPollingTraceFile = NULL;
ClClientAPI*		ClRunningSrvMonitor::s_poClientAPI = NULL;
ClCriticalSection	ClRunningSrvMonitor::m_oTraceSynch( M4_TRUE );

#define NO_PASSWORD_LOGIN_RETURN -88

#ifdef _WINDOWS
#define	TRACE_FMT_1	"OLTP execution time\tmsecs\t%I64d\n"
#define	TRACE_FMT_2	"XML execution time\tmsecs\t%I64d\n"
#define	TRACE_FMT_3	"Reset polling delay\tmsecs\t%I64d\n"
#define	TRACE_FMT_4	"Synchronizing\tmsecs\t%I64d\n"
#define	TRACE_FMT_5	"Increase polling delay\tmsecs\t%I64d\n"
#define	TRACE_FMT_6	"Sleeping polling delay\tmsecs\t%I64d\n"
#define	TRACE_FMT_7	"Sleeping executor delay\tmsecs\t%I64d\n"
#else
#define	TRACE_FMT_1	"OLTP execution time\tmsecs\t%lld\n"
#define	TRACE_FMT_2	"XML execution time\tmsecs\t%lld\n"
#define	TRACE_FMT_3	"Reset polling delay\tmsecs\t%lld\n"
#define	TRACE_FMT_4	"Synchronizing\tmsecs\t%lld\n"
#define	TRACE_FMT_5	"Increase polling delay\tmsecs\t%lld\n"
#define	TRACE_FMT_6	"Sleeping polling delay\tmsecs\t%lld\n"
#define	TRACE_FMT_7	"Sleeping executor delay\tmsecs\t%lld\n"
#endif


#define	M4CH_DUMMY_B5	"\x8\xA\x5\xB\xE\xF\x2\xD\xE\x5\x4\x4\xC\x8\xB\xE\xA\xD\xF\xE\xD\xC\x8\xD\xB\xC\x4\x8\x4\x3\x4\xD\xF\xE\xB\xB\x4\xB\x5\xB\xF\x1\xA\xD\x3\xB\x6\xA\x3\xD\xF\x3\x6\x1\xD\x6\x2\x8\xB\x3\x6\x2\xF\x0\xD\x5\x8\xD\xB\xA\x3\x3\x7\x9\x1\xE\x4\x5\x2\xF\xC\x2\xD\xA\x7\xD\xB\x8\x5\xA\x2\x7\xA\xF\x1\xF\xD\xA\x6\x6\x4\xD\xE\x8\x0\x3\x5\xF\x1\xB\x5\x7\x9\x7\xC\x7\xB\xB\xF\x8\xF\xF\xC\x5\x2\xA\xF\xB\x9"


ClRunningSrvMonitor::ClRunningSrvMonitor( const ClSiteNodeInformation &ai_oSiteNode, ClSiteHealthVerifier* ai_pHealthMonitor )
{
	m_oSiteNode				= ai_oSiteNode;
	m_pSiteHealthVerifier	= ai_pHealthMonitor;
	m_strPwd				= "";
	m_hSessionId			= 0;
	
	_DumpPollingTrace( "Create server monitor\n" );
}


ClRunningSrvMonitor::~ClRunningSrvMonitor()
{
	_DumpPollingTrace( "Destroying server monitor\n" );

	if( m_hSessionId != 0 )
	{
		_ServerLogout();
	}

	_DumpPollingTrace( "Destroyed server monitor\n" );
}


// -- Main thread method.
void ClRunningSrvMonitor::Run()
{
	m4millisec_t	iDelay;
	m4return_t		iRet;
	m4bool_t		bDiscarded;
	m4bool_t		bMustSleep;
	m4millisec_t	iRemainingTime;
	
	
	SETCODEF( M4_ERR_DPCH_START_SRV_MONITOR, DEBUGINFOLOG, "#*s1*#%s#%s#", m_oSiteNode.GetSiteNodeID().c_str(), m_oSiteNode.GetNodeHost().c_str() );
	_DumpPollingTrace( "Start server monitor\n" );

	iDelay = m_pSiteHealthVerifier->GetDelay();

	
	// Polling loop.
	while( ! IsTerminate () )
	{
		bMustSleep = M4_FALSE;


		// -- Do the login.
		_DumpPollingTrace( "Try user login\tUserId\t%s\n", m_oSiteNode.GetUserName().c_str() );

		iRet = _ServerLogin();


		// -- Login success
		if( iRet == M4_SUCCESS || iRet >= 20 )
		{
			bDiscarded = M4_FALSE;
			
			_DumpPollingTrace( "User login success\tReturn\t%d\tSessionId\t%d\n", iRet, m_hSessionId );

			if( m_oSiteNode.GetDiscarded() == M4_TRUE )
			{
				// Reset loop delay.
				iDelay = m_pSiteHealthVerifier->GetDelay();

				SETCODEF( M4_ERR_DPCH_UNSET_SRV_DISCARDED, WARNINGLOG, "#*s1*#%s#%s#", m_oSiteNode.GetSiteNodeID().c_str(), m_oSiteNode.GetNodeHost().c_str() );
				_DumpPollingTrace( "Discarded server recovered\n" );
				_DumpPollingTrace( TRACE_FMT_3, iDelay );
			}


			// -- Launch the server executor clients.
			iRemainingTime = iDelay;
			iRet = _CalculateExecutorsTime( iRemainingTime );

			if( iRet == M4_ERROR)
			{
				_DumpPollingTrace( "Error calculating execution time\n" );
			}

			if( iRemainingTime > 0)
			{
				_DumpPollingTrace( TRACE_FMT_4, iRemainingTime );
				_SleepNoLock( iRemainingTime );
			}
			else
			{
				_DumpPollingTrace( "Synchronization not required\n" );
			}


			// -- Do the logout.
			_DumpPollingTrace( "Try user logout\tSessionId\t%d\n", m_hSessionId );

			iRet = _ServerLogout();


			if( iRet == M4_SUCCESS )
			{
				_DumpPollingTrace( "User logout success\tSessionId\t%d\n", m_hSessionId );
			}
			else
			{
				_DumpPollingTrace( "User logout failed\tSessionId\t%d\n", m_hSessionId );
				// De momento no lo marcamos como discarded, el login fue satisfactorio.
			}
		}

		// -- Login error.
		else if( iRet == M4_ERROR || iRet == 1 ) // Controlar cuando es contraseña expirada o time-out.
		{
			bDiscarded = M4_TRUE;
			bMustSleep = M4_TRUE;

			_DumpPollingTrace( "User login error\tReturn\t%d\n", iRet );
			
			if( m_oSiteNode.GetDiscarded() == M4_FALSE )
			{
				iDelay *= 2;

				SETCODEF( M4_ERR_DPCH_SET_SRV_DISCARDED, ERRORLOG, "#*s1*#%s#%s#", m_oSiteNode.GetSiteNodeID().c_str(), m_oSiteNode.GetNodeHost().c_str() );
				_DumpPollingTrace( "Setting discarded server\n" );
				_DumpPollingTrace( TRACE_FMT_5, iDelay );

			}
			else
			{
				_DumpPollingTrace( "Server remains discarded\n" );
			}
		}

		// -- Login failed.
		else
		{
			if( iRet != NO_PASSWORD_LOGIN_RETURN )
			{
				SETCODEF( M4_ERR_DPCH_POLLING_LOGIN_FAILED, ERRORLOG, "#*s1*#%d#%s#%s#%s#", iRet, m_oSiteNode.GetSiteNodeID().c_str(), m_oSiteNode.GetNodeHost().c_str(), m_oSiteNode.GetUserName().c_str() );
				_DumpPollingTrace( "User login failed\tReturn\t%d\n", iRet );
			}
						
			// No se elimina del mapa de monitores para que no se aranque de nuevo cuando lleguen
			// notificaciones de estado mientras esté ready.
			break;
		}

		// -- Refresh discarded flag.
		m_oSiteNode.SetDiscarded( bDiscarded );
		ClSiteRepositorySingleton::GetInstance()->UpdateDiscardedNode( m_oSiteNode );
	
		if( bMustSleep == M4_TRUE )
		{
			_DumpPollingTrace( TRACE_FMT_6, iDelay );
			_SleepNoLock( iDelay );
		}
	
	}

	SETCODEF( M4_ERR_DPCH_END_SRV_MONITOR, DEBUGINFOLOG, "#*s1*#%s#%s#", m_oSiteNode.GetSiteNodeID().c_str(), m_oSiteNode.GetNodeHost().c_str() );
	_DumpPollingTrace( "End server monitor\n" );
}


// -- Activity log helper method.
void ClRunningSrvMonitor::_DumpPollingTrace( m4pchar_t ai_pcFormat, ... )
{
	static m4char_t	acHeader[] = "Time\tHost\tServer\tPort\tEvent\tParamName\tParamValue\tParamName\tParamValue\n";
	m4bool_t		bHeader;
	static m4char_t	acMode[3];


	// -- Not tracing.
	if( m_pSiteHealthVerifier->GetDumpPollingTrace() == M4_FALSE )
	{
		return;
	}

	{
		ClBlockSync	oBlocker( m_oTraceSynch );

		if( s_pfPollingTraceFile == NULL )
		{
			strcpy( acMode, "wt" );
			bHeader = M4_TRUE;
		}
		else
		{
			strcpy( acMode, "at" );
			bHeader = M4_FALSE;
		}

		s_pfPollingTraceFile = fopen( "../temp/srvspolling.log", acMode );

		// -- Can't open.
		if( s_pfPollingTraceFile == NULL )
		{
			SETCODEF( M4_ERR_DPCH_OPEN_TRACE_FILE_ERROR, ERRORLOG, "#*s1*#" );
			
			m_pSiteHealthVerifier->SetDumpPollingTrace( M4_FALSE );

			return;
		}

		if( bHeader == M4_TRUE )
		{
			M4ClTimeStamp	oTime;
			M4ClString		sTime;
			oTime.now ();
			sTime = oTime;
			fprintf( s_pfPollingTraceFile, "%s\n", (char*)sTime );

			fprintf( s_pfPollingTraceFile, acHeader );
		}

		#ifdef _WINDOWS
			fprintf( s_pfPollingTraceFile, "%I64d\t",  ClPrecisionTicker::GetTimeTick());
		#else
			fprintf( s_pfPollingTraceFile, "%lld\t",  ClPrecisionTicker::GetTimeTick());
		#endif

		fprintf( s_pfPollingTraceFile, "%s\t%s\t%d\t",  m_oSiteNode.GetNodeHost().c_str(), m_oSiteNode.GetSiteNodeID().c_str(), m_oSiteNode.GetBasePort() );
			
		va_list args;
		va_start( args, ai_pcFormat );
		vfprintf( s_pfPollingTraceFile, ai_pcFormat, args );
		va_end( args );	
		fclose( s_pfPollingTraceFile );
	}
}


// -- Gets decrypted user password.
m4return_t	ClRunningSrvMonitor::_GetPwd( M4ClString& ao_strPwd )
{
	m4pcchar_t	pcCryptedPwd;
	m4pchar_t	pcPwd = NULL;
	m4int_t		iSize, iReqSize;
	m4return_t	iRet = M4_SUCCESS;


	if( m_strPwd.empty() )
	{
		pcCryptedPwd = m_oSiteNode.GetPassword().c_str();

		iSize = strlen ( pcCryptedPwd );
		
		iRet = DecryptTwoWaySecondKey( pcCryptedPwd, iSize, M4CH_DUMMY_B5 + 23, strlen( M4CH_DUMMY_B5 + 23 ), pcPwd, iReqSize );

		if( M4_SUCCESS == iRet )
		{
			m_strPwd = pcPwd;
		}
		else
		{
			SETCODEF( M4_ERR_DPCH_POLLING_DECRYPT_ERROR, ERRORLOG, "#*s1*#%s#%s#%s#", m_oSiteNode.GetSiteNodeID().c_str(), m_oSiteNode.GetNodeHost().c_str(), m_oSiteNode.GetUserName().c_str() );
		}

		delete pcPwd;
	}

	ao_strPwd = m_strPwd;


	return iRet;
}


// -- Returns the client API instance.
ClClientAPI* ClRunningSrvMonitor::_GetClientAPI()
{
	if( s_poClientAPI == NULL )
	{
		s_poClientAPI = new ClClientAPI( NULL, 3, 4444, 1 );

		if( m_pSiteHealthVerifier->GetTimeout() > 0 )
		{
			s_poClientAPI->SetControlTimeout( m_pSiteHealthVerifier->GetTimeout() * 1000 );
		}
	}


	return s_poClientAPI;
}


// -- Logs in into current server.
m4return_t ClRunningSrvMonitor::_ServerLogin()
{
	m4return_t			iRet;
	ClClientAPI*		poClientAPI;
	M4ClString			strPwd;
	M4ClString			strNodeHost;
	M4ClString			strUserName;
	ClTagList			oArgs;
	static m4bool_t		s_bDoSetEnconding = M4_TRUE;
	static m4char_t		s_acUserName [ 64 ] = "";
	m4pchar_t			pcUserName = s_acUserName;


	if( m_hSessionId != 0 )
	{
		_ServerLogout();
	}

	poClientAPI = _GetClientAPI();

	oArgs.Clear();
	oArgs.SetArg( M4_LOGON_TYPE_PROP_NAME, "0" );
	oArgs.SetTag( M4_PROP_SERVER_ARCHITECTURE );
	oArgs.SetTag( M4_PROP_SESSION_CREDENTIAL );
	oArgs.SetTag( M4_PROP_SESSION_TIMEOUT );
	oArgs.SetArg( M4_COMPRESSION_TYPE, M4_COMPRESSION_TYPE_NULL );
	oArgs.SetArg( M4_SESS_USER_LANGUAGE,"3" );

	if( s_bDoSetEnconding == M4_TRUE )
	{
		oArgs.SetArg( M4_SET_CLIENT_ENCODING, 1 );
	}

	iRet = _GetPwd( strPwd );

	if( iRet == M4_SUCCESS )
	{
		strNodeHost = m_oSiteNode.GetNodeHost().c_str();

		// Inicialmente probamos con la codificación que tenga por defecto (unicode).
		if( *pcUserName == '\0' )
		{
			strcpy( pcUserName, m_oSiteNode.GetUserName().c_str() );
		}

		strUserName = pcUserName;

		iRet = poClientAPI->OpenSession( strNodeHost, m_oSiteNode.GetBasePort(), strUserName, strPwd, 1, m_hSessionId, &oArgs, false );

		if( s_bDoSetEnconding == M4_TRUE )
		{
			s_bDoSetEnconding = M4_FALSE;

			// Hemos cambiado al encoding del server y nos han recodificado el id de usuario.
			if( oArgs.GetArg( M4_RE_ENCODED_USER_ID, pcUserName, 64 ) == M4_SUCCESS )
			{
				oArgs.DelTag( M4_RE_ENCODED_USER_ID );
			}
		}
	}
	else
	{
		_DumpPollingTrace( "Decrypt password error\n" );
		iRet = NO_PASSWORD_LOGIN_RETURN;
	}


	return iRet;
}


// -- Logs out from current server.
m4return_t ClRunningSrvMonitor::_ServerLogout()
{
	m4return_t		iRet;
	ClClientAPI*	poClientAPI;

	
	if( m_hSessionId == 0 )
	{
		return M4_SUCCESS;
	}

	poClientAPI = _GetClientAPI();

	iRet = poClientAPI->CloseSession( m_hSessionId );

	if( iRet == M4_SUCCESS )
	{
		m_hSessionId = 0;
	}


	return iRet;
}


// -- Calculates execution times for each target server executor.
m4return_t ClRunningSrvMonitor::_CalculateExecutorsTime( m4millisec_t& ao_iRemainingTime )
{
	m4return_t				iRet = M4_SUCCESS;
	ClSiteNodeRepository*	pRepository;
	m4millisec_t			iStartTime = ClPrecisionTicker::GetTimeTick(), iStartExec, iExecTime, iExecutorDelay;
	m4bool_t				bFirst = M4_TRUE ;


	pRepository		= ClSiteRepositorySingleton::GetInstance();
	iExecutorDelay	= m_pSiteHealthVerifier->GetExecutorDelay();


	while( true )
	{

		if( bFirst == M4_TRUE || ao_iRemainingTime > ( ClPrecisionTicker::GetTimeTick() - iStartTime ) + m_oSiteNode.GetOLTPTime() )
		{
			iStartExec =  ClPrecisionTicker::GetTimeTick();
			iRet = _LaunchOLTPExecution();

			if( iRet == M4_ERROR )
			{
				_DumpPollingTrace( "Error calculating OLTP execution time\n" );
				break;
			}
			else if( iRet == M4_WARNING )
			{
				_DumpPollingTrace( "Timeout expired waiting OLTP execution\n" );
				break;
			}

			iExecTime = ClPrecisionTicker::GetTimeTick() - iStartExec;
			m_oSiteNode.SetOLTPTime( iExecTime );
			pRepository->UpdateOLTPTimeNode( m_oSiteNode );

			_DumpPollingTrace( TRACE_FMT_1, iExecTime );
		}
		else
		{
			break;
		}


		if( bFirst == M4_TRUE || ao_iRemainingTime > ( ClPrecisionTicker::GetTimeTick() - iStartTime ) + m_oSiteNode.GetXMLTime() )
		{
			iStartExec =  ClPrecisionTicker::GetTimeTick();
			iRet = _LaunchXMLExecution();

			if( iRet == M4_ERROR )
			{
				_DumpPollingTrace( "Error calculating XML execution time\n" );
				break;
			}
			else if( iRet == M4_WARNING )
			{
				_DumpPollingTrace( "Timeout expired waiting XML execution\n" );
				break;
			}
			
			iExecTime = ClPrecisionTicker::GetTimeTick() - iStartExec;
			m_oSiteNode.SetXMLTime( iExecTime );
			pRepository->UpdateXMLTimeNode( m_oSiteNode );

			_DumpPollingTrace( TRACE_FMT_2, iExecTime );
		}
		else
		{
			break;
		}


		if( ao_iRemainingTime > ( ClPrecisionTicker::GetTimeTick() - iStartTime ) + iExecutorDelay )
		{
			_DumpPollingTrace( TRACE_FMT_7, iExecutorDelay );
			_SleepNoLock( iExecutorDelay );
		}
		else
		{
			break;
		}

		bFirst = M4_FALSE;
	}

	ao_iRemainingTime -= ClPrecisionTicker::GetTimeTick() - iStartTime;


	return iRet;
}


// -- Launchs a single OLTP execution.
m4return_t ClRunningSrvMonitor::_LaunchOLTPExecution()
{
	m4return_t		iRet;
	ClClientAPI*	poClientAPI;
	m4handle_t		hContext;
	M4DataStorage*	poOutputDataStorage;
	M4DataStorage*	poInputDataStorage;
	ClCheckUnit*	poCheckUnit = NULL;
	m4handle_t		hRequest = 0;
	m4uint32_t		uiTimeout;


	if( m_hSessionId == 0 )
	{
		return M4_ERROR;
	}

	poClientAPI = _GetClientAPI();

	iRet = poClientAPI->OpenService( m_hSessionId, "OLTP", hContext);

	if( iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}


	poOutputDataStorage = M4DataStorage::GetNewDataStorage();
	poInputDataStorage  = M4DataStorage::GetNewDataStorage();

	poOutputDataStorage->Clear();
	poOutputDataStorage->GetNewCheckUnit( poCheckUnit );
	
	poClientAPI->GetControlTimeout( uiTimeout );

	iRet = poClientAPI->SyncRequest( m_hSessionId, hContext, 0, 0, poOutputDataStorage, poInputDataStorage, hRequest, uiTimeout );


	return iRet;
}


#define M4_XML_REQUEST	"<?xml version=\"1.0\" encoding=\"UTF-8\"?><m4xml version=\"1.0\"></m4xml>"

// -- Launchs a single XML execution.
m4return_t ClRunningSrvMonitor::_LaunchXMLExecution()
{
	m4return_t		iRet;
	ClClientAPI*	poClientAPI;
	m4handle_t		hContext;
	M4DataStorage*	poOutputDataStorage;
	M4DataStorage*	poInputDataStorage;
	ClDataUnit*		poDataUnit = NULL;
	m4uint_t		iLen;
	m4handle_t		hRequest = 0;
	m4uint32_t		uiTimeout;
	

	if( m_hSessionId == 0 )
	{
		return M4_ERROR;
	}

	poClientAPI = _GetClientAPI();

	iRet = poClientAPI->OpenService( m_hSessionId, "XML", hContext);

	if( iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}


	poOutputDataStorage = M4DataStorage::GetNewDataStorage();
	poInputDataStorage  = M4DataStorage::GetNewDataStorage();

	poOutputDataStorage->Clear();
	poOutputDataStorage->GetNewDataUnit( poDataUnit );
	
	iLen = strlen( M4_XML_REQUEST );
	poDataUnit->SetSize( iLen );
	memcpy( poDataUnit->GetBuffer(), M4_XML_REQUEST, iLen );
		
	poClientAPI->GetControlTimeout( uiTimeout );

	iRet = poClientAPI->SyncRequest( m_hSessionId, hContext, 0, 0, poOutputDataStorage, poInputDataStorage, hRequest, uiTimeout );


	return iRet;
}

// bg 258200
void ClRunningSrvMonitor::_SleepNoLock( m4millisec_t ai_iMsecs )
{
	m4millisec_t iStartTime = ClPrecisionTicker::GetTimeTick();

	if( ai_iMsecs <= 0 )
	{
		return;
	}

	while( !IsTerminate() && ( ClPrecisionTicker::GetTimeTick() - iStartTime ) < ai_iMsecs )
	{
		Sleep( 100 );
	}
}


// -----------------------------------------------------------------------------------------------------------------------------
// -- Class ClSiteHealthVerifier
// -----------------------------------------------------------------------------------------------------------------------------

// -- Static class members
ClCriticalSection	ClSiteHealthVerifier::m_oSynch( M4_TRUE );


ClSiteHealthVerifier::ClSiteHealthVerifier( m4bool_t ai_bEnabled, m4millisec_t ai_iDelay, m4bool_t ai_bDumpPollingTrace, m4uint32_t ai_iTimeout, m4millisec_t ai_iExecutorDelay )
{
	m_bEnabled			= ai_bEnabled;
	m_iDelay			= ai_iDelay;
	m_bDumpPollingTrace	= ai_bDumpPollingTrace;
	m_iTimeout			= ai_iTimeout;
	m_iExecutorDelay	= ai_iExecutorDelay;
}


ClSiteHealthVerifier::~ClSiteHealthVerifier()
{
	RemoveAllServerMonitors();
}


// -- Adds a new server node to monitorize.
m4return_t ClSiteHealthVerifier::AddServerMonitor( const ClSiteNodeInformation &ai_oSiteNode )
{
	TMonitorsMap::iterator				Iterator;
	ClRunningSrvMonitor*				pRunningSrvMonitor;
	pair<TMonitorsMap::iterator, bool>	mapRes;
	m4return_t							iRet;
	ClBlockSync							oBlocker( m_oSynch ); // -- Lock.


	// -- Not monitoring.
	if( m_bEnabled == M4_FALSE )
	{
		return M4_SUCCESS;
	}
	
	
	Iterator = m_oRunningSrvMonitors.find( ai_oSiteNode.GetSiteNodeMapKey() );

	// -- Server already monitorized.
	if ( Iterator != m_oRunningSrvMonitors.end () )
	{
		return M4_SUCCESS;
	}
	
	
	// -- New monitor.
	pRunningSrvMonitor = new ClRunningSrvMonitor( ai_oSiteNode, this );

	// -- Stores it.
	TMonitorsMap::value_type mapEntry( ai_oSiteNode.GetSiteNodeMapKey(), pRunningSrvMonitor );
	mapRes = m_oRunningSrvMonitors.insert( mapEntry );

	if( M4_FALSE == mapRes.second )
	{
		SETCODEF( M4_ERR_DPCH_STORE_MONITOR_ERROR, ERRORLOG, "#*s1*#%s#%s#", ai_oSiteNode.GetSiteNodeID().c_str(), ai_oSiteNode.GetNodeHost().c_str() );

		delete pRunningSrvMonitor;
		return M4_ERROR;
	}

	// -- Starts the watcher.
	iRet = pRunningSrvMonitor->Start();

	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_ERR_DPCH_START_MONITOR_ERROR, ERRORLOG, "#*s1*#%s#%s#", ai_oSiteNode.GetSiteNodeID().c_str(), ai_oSiteNode.GetNodeHost().c_str() );
	}


	return iRet;
}


// -- Stops monitorizing a server node.
m4return_t ClSiteHealthVerifier::RemoveServerMonitor( const ClSiteNodeInformation &ai_oSiteNode )
{
	TMonitorsMap::iterator	Iterator;
	ClBlockSync				oBlocker( m_oSynch ); // -- Lock.


	// -- Not monitoring.
	if( m_bEnabled == M4_FALSE )
	{
		return M4_SUCCESS;
	}


	Iterator = m_oRunningSrvMonitors.find( ai_oSiteNode.GetSiteNodeMapKey() );

	// -- Not found
	if( Iterator == m_oRunningSrvMonitors.end () )
	{
		return M4_ERROR;
	}
	

	// -- Stops the monitor.
	(*Iterator).second->Terminate();

	// -- Waits until monitor has finished.
	while( !(*Iterator).second->IsExit () )
	{
		Sleep( 100 );
	}

	// -- Cleaning.
	delete (*Iterator).second;
	m_oRunningSrvMonitors.erase( Iterator );


	return M4_SUCCESS;
}


// -- Stops monitorizing all server nodes.
void ClSiteHealthVerifier::RemoveAllServerMonitors()
{
	TMonitorsMap::iterator	Iterator;
	
	// Avoid create new server monitors while dispatcher shuts down.
	{
		ClBlockSync	oBlocker( m_oSynch ); // -- Lock.
		m_bEnabled = M4_FALSE;
	}

	// -- Nothing to stop.
	if (m_oRunningSrvMonitors.empty())
	{
		return;
	}
		
	for( Iterator = m_oRunningSrvMonitors.begin(); Iterator != m_oRunningSrvMonitors.end(); Iterator++ )
	{
		// -- Stops the monitor.
		(*Iterator).second->Terminate();

		// -- Waits until monitor has finished.
		while( !(*Iterator).second->IsExit () )
		{
			Sleep( 100 );
		}

		// -- Cleaning.
		delete (*Iterator).second;
	}

	m_oRunningSrvMonitors.clear();


	return;
}
