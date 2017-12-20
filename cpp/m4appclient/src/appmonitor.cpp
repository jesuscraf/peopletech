//	==============================================================================
//
//	 (c) Copyright  1991-2008 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            M4ClientAPI
//	 File:              appmonitor.cpp
//	 Project:           m4appclientdll
//	 Author:            Meta Software M.S. , S.A
//	 Date:				081128
//	 Language:          C++
//	 Operating System:  WINDOWS
//	 Design Document:
//
//	 Definition:
//
//	    This module defines client application monitor manager.
//
//
//	==============================================================================

#ifdef _WINDOWS

#include <appmonitor.hpp>
#include <windows.h>
#include <process.h>
#include <m4string.hpp>
#include <clientconnectionwininet.hpp>
#include <m4cryptc.hpp>
#include "m4unicode.hpp"
#include <shlwapi.h>

#define	M4PRM_NO_SENSE_VAL "\x2\x9\x6\x2\x1\x9\x2\x8\xC\x5\x3\x3\xD\xF\x8\x1\x1\x4\xB\x3\xA\x6\x7\x8\x2\xA\xD\x5\xA\x8\x6\x7\x4\x1\x7\x4\xF\xD\xF\xC\x4\xD\x0\x7\x2\x1\x5\x3\xB\x2\x3\x4\x7\xA\xF\xD\xA\x6\x4\x4\x8\xE\x8\x1\x9\x1\x4\x3\xA\xC\x1\x7\xC\xE\x5\x8\xA\x1\x2\x7\x5\x9\x7\x7\xB\x7\xA\xF"
#define DELIMITER_CHAR "|"
#define NUM_ARGS_SERIALIZED	7

// -- Class ClAppMonitor ------------------------------------------------------


// -- -------------------------------------------------------------------------------
// -- Client application monitor executable file name.
m4pcchar_t ClAppMonitor::ms_pccAppMonitorExeName = "m4systray.exe";
// -- -------------------------------------------------------------------------------


// -- -------------------------------------------------------------------------------
// -- ClAppMonitor cttor.
// --
// -- Args:
// --    ai_strGWSrv: Meta4 gateway host name.
// --    ai_uiGWSrvPort: Gateway listening port.
// --    ai_strAppSrv: Meta4 application server host name.
// --    ai_uiAppSrvPort: Application server listening port.
// --    ai_strServlet: Gateway servlet web location.
// --    ai_bUseSSL: SSL communications.
// --    ai_strCredential: Current user Meta4 credential.
// -- -------------------------------------------------------------------------------
ClAppMonitor::ClAppMonitor( m4string_t ai_strGWSrv, m4uint32_t ai_uiGWSrvPort, m4string_t ai_strAppSrv, m4uint32_t ai_uiAppSrvPort, m4string_t ai_strServlet, m4bool_t ai_bUseSSL, m4string_t ai_strCredential )
{
	m_hdAppMonitor	= 0 ;

	SetMonitorParams( ai_strGWSrv, ai_uiGWSrvPort, ai_strAppSrv, ai_uiAppSrvPort, ai_strServlet, ai_bUseSSL, ai_strCredential ) ;
}


// -- -------------------------------------------------------------------------------
// -- ClAppMonitor dttor.
// -- -------------------------------------------------------------------------------
ClAppMonitor::~ClAppMonitor()
{
}


// -- -------------------------------------------------------------------------------
// -- Executes the monitor application and returns to the caller.
// --
// -- Returns: M4_SUCCESS / M4_ERROR.
// -- -------------------------------------------------------------------------------
m4return_t ClAppMonitor::StartMonitor()
{
	m4return_t	iRet ;
	m4char_t	acMonitorExePath[ MAX_PATH ] ;
	m4char_t	acPid [11] ;
	m4char_t	acStartup [11] ;
	m4int32_t	iErrorCode ;
	string		sErrorMsg ;


	iRet = GetMonitorPath( acMonitorExePath, MAX_PATH ) ;

	if( M4_SUCCESS != iRet )
	{
		return iRet ;
	}

	SETCODEF( M4_DEB_PATH_MIND_MONITOR, DEBUGINFOLOG, "#*s1*#%s#", acMonitorExePath ) ;


	iRet = GetProcessInfo( acPid, acStartup ) ;

	if( M4_SUCCESS != iRet )
	{
		return iRet ;
	}


	// -- Launches the monitor process.
	m_hdAppMonitor = _spawnl( _P_NOWAIT, acMonitorExePath, acMonitorExePath, acPid, acStartup, m_strParameters.c_str(), NULL ) ;


	if( -1 == m_hdAppMonitor )
	{
		GetErrorMessage( iErrorCode, sErrorMsg ) ;
		SETCODEF( M4_ERR_CODE_DESC_WIN_FUNC_ERROR, ERRORLOG, "#*s1*#%d#%s#%s#", iErrorCode, "_spawnl", sErrorMsg.c_str() ) ;

		SETCODEF( M4_ERR_CAN_NOT_LAUNCH_MIND_MONITOR_EXE, ERRORLOG, "#*s1*#%s#", acMonitorExePath ) ;

		iRet = M4_ERROR ;
	}


	return iRet ;
}


// -- -------------------------------------------------------------------------------
// -- Stops the monitor application.
// --
// -- Returns: M4_SUCCESS / M4_ERROR.
// -- -------------------------------------------------------------------------------
m4return_t ClAppMonitor::StopMonitor()
{
	BOOL		bRet ;
	m4int32_t	iErrorCode ;
	string		sErrorMsg ;


	if( m_hdAppMonitor > 0 )
	{
		// -- Stops the monitor process.
		bRet = TerminateProcess( reinterpret_cast<void*>(m_hdAppMonitor), 0 ) ;

		if( 0 == bRet )
		{
			GetErrorMessage( iErrorCode, sErrorMsg ) ;
			SETCODEF( M4_ERR_CODE_DESC_WIN_FUNC_ERROR, ERRORLOG, "#*s1*#%d#%s#%s#", iErrorCode, "TerminateProcess", sErrorMsg.c_str() ) ;

			return M4_ERROR ;
		}

	}


	return M4_SUCCESS ;
}


// -- -------------------------------------------------------------------------------
// -- Checks if the monitor application is running.
// --
// -- Args:
// --    ao_bIsAlive: Output arg with the result of checking if monitor
// --				  is still running.
// --
// -- Returns: M4_SUCCESS / M4_ERROR.
// -- -------------------------------------------------------------------------------
m4return_t ClAppMonitor::IsAlive( m4bool_t& ao_bIsAlive )
{
	BOOL		bRet ;
	DWORD		dwExitCode ;
	m4int32_t	iErrorCode ;
	string		sErrorMsg ;


	if( 0 == m_hdAppMonitor )
	{
		ao_bIsAlive = M4_FALSE ;

		return M4_SUCCESS ;
	}

	ao_bIsAlive = M4_TRUE ;

	bRet = GetExitCodeProcess( reinterpret_cast<void*>(m_hdAppMonitor), &dwExitCode ) ;

	if( 0 == bRet )
	{
		GetErrorMessage( iErrorCode, sErrorMsg ) ;
		SETCODEF( M4_ERR_CODE_DESC_WIN_FUNC_ERROR, ERRORLOG, "#*s1*#%d#%s#%s#", iErrorCode, "GetExitCodeProcess", sErrorMsg.c_str() ) ;

		return M4_ERROR ;
	}

	if( STILL_ACTIVE != dwExitCode )
	{
		ao_bIsAlive = M4_FALSE ;
	}


	return M4_SUCCESS ;
}


// -- -------------------------------------------------------------------------------
// -- Sets communication parameters in a single encrypted string.
// --
// -- Args:
// --    ai_strGWSrv: Meta4 gateway host name.
// --    ai_uiGWSrvPort: Gateway listening port.
// --    ai_strAppSrv: Meta4 application server host name.
// --    ai_uiAppSrvPort: Application server listening port.
// --    ai_strServlet: Gateway servlet web location.
// --    ai_bUseSSL: SSL communications.
// --    ai_strCredential: Current user Meta4 credential.
// -- -------------------------------------------------------------------------------
m4return_t ClAppMonitor::SetMonitorParams( m4string_t ai_strGWSrv, m4uint32_t ai_uiGWSrvPort, m4string_t ai_strAppSrv, m4uint32_t ai_uiAppSrvPort, m4string_t ai_strServlet, m4bool_t ai_bUseSSL, m4string_t ai_strCredential )
{
	m4return_t	iRet = M4_SUCCESS ;
	m4char_t	acGWSrvPort [11] ;
	m4char_t	acAppSrvPort [11] ;
	m4char_t	acUseSSL [4] ;
	m4string_t	strParams ;
	m4pchar_t	pcCryptedParams = NULL ;
	m4int_t		iBufReqSize = 0 ;


	// -- Number to string parameters.
	ultoa( ai_uiGWSrvPort, acGWSrvPort, 10 ) ;
	ultoa( ai_uiAppSrvPort, acAppSrvPort, 10 ) ;
	itoa( ai_bUseSSL, acUseSSL, 10 ) ;


	// -- All parameters in a string.
	strParams = ai_strGWSrv ;
	strParams += DELIMITER_CHAR ;
	strParams += acGWSrvPort ;
	strParams += DELIMITER_CHAR ;
	strParams += ai_strAppSrv ;
	strParams += DELIMITER_CHAR ;
	strParams += acAppSrvPort ;
	strParams += DELIMITER_CHAR ;
	strParams += ai_strServlet ;
	strParams += DELIMITER_CHAR ;
	strParams += acUseSSL ;
	strParams += DELIMITER_CHAR ;
	strParams += ai_strCredential ;


	// -- Encrypt all parameters.
	iRet = EncryptTwoWay(strParams.c_str(), strParams.length(), M4PRM_NO_SENSE_VAL + 18, strlen(M4PRM_NO_SENSE_VAL + 18), pcCryptedParams, iBufReqSize);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_ENCRYPTING_PARAMS, ERRORLOG, "#*s1*#");
		iRet = M4_ERROR;
	}

	m_strParameters = pcCryptedParams;

	delete pcCryptedParams;

	return iRet;
}


// -- -------------------------------------------------------------------------------
// -- Gets the monitor application executable path.
// --
// -- Args:
// --    ao_pcPath: Output buffer.
// --    ai_iSize: Buffer size.
// --
// -- Returns: M4_SUCCESS / M4_ERROR.
// -- -------------------------------------------------------------------------------
m4return_t ClAppMonitor::GetMonitorPath( const m4pchar_t ao_pcPath, const m4int_t ai_iSize )
{
	m4char_t	acExePath[ MAX_PATH ] ;
	DWORD       dwFileAttr ;
	m4int32_t	iErrorCode ;
	string		sErrorMsg ;
	m4int_t		iBufReqSize = 0 ;
	m4char_t	acCurrDir[ MAX_PATH ] ;

	BOOL			bResult = FALSE ;
	DWORD			dwResult = 0 ;

	memset( ao_pcPath, 0, ai_iSize ) ;
	memset( acExePath, 0, ai_iSize ) ;

	memset( acCurrDir, 0, MAX_PATH ) ;

	// -- Check if monitor executable is located in the current working dir.
	dwResult = GetModuleFileName( NULL, acCurrDir, sizeof( acCurrDir ) - 1 ) ;
	if( dwResult > 0 )
	{
		bResult = PathRemoveFileSpec( acCurrDir ) ;
		strcat( acCurrDir, "\\" ) ; 

		// -- spawn does not like blank spaces in the path.
		iBufReqSize = GetShortPathName( acCurrDir, ao_pcPath, ai_iSize ) ;

		if( 0 == iBufReqSize )
		{
			memset( ao_pcPath, 0, ai_iSize ) ;

			GetErrorMessage( iErrorCode, sErrorMsg ) ;
			SETCODEF( M4_ERR_CODE_DESC_WIN_FUNC_ERROR, ERRORLOG, "#*s1*#%d#%s#%s#", iErrorCode, "GetShortPathName", sErrorMsg.c_str() ) ;

			return M4_ERROR ;
		}
	}

	// -- Executable file name will not be in the short way. If not the process will appears as m4syst~1.exe.
	strcat( ao_pcPath, ms_pccAppMonitorExeName ) ;	

	// UNICODE FILE
	int iLength = -1 ;
	
	char* pcFileName = M4CppToANSI( ao_pcPath, iLength ) ;

    dwFileAttr = GetFileAttributes( pcFileName ) ;
	delete [] pcFileName ;

	if( 0xFFFFFFFF == dwFileAttr )
	{

		memset( ao_pcPath, 0, ai_iSize ) ;

		// -- Try with rich web default path installation.
		// Bg 154595
		strcpy( acExePath, getenv( "APPDATA" ) ) ;
		strcat( acExePath, "\\meta4\\M4WindowsClient\\bin\\" ) ;

		// -- spawn does not like blank spaces in the path.
	    iBufReqSize = GetShortPathName( acExePath, ao_pcPath, ai_iSize ) ;
		
		if( 0 == iBufReqSize )
		{
			memset( ao_pcPath, 0, ai_iSize ) ;

			GetErrorMessage( iErrorCode, sErrorMsg ) ;
			SETCODEF( M4_ERR_CODE_DESC_WIN_FUNC_ERROR, ERRORLOG, "#*s1*#%d#%s#%s#", iErrorCode, "GetShortPathName", sErrorMsg.c_str() ) ;

			return M4_ERROR ;
		}

		// -- Executable file name will not be in the short way. If not the process will appears as m4syst~1.exe.
		strcat( ao_pcPath, ms_pccAppMonitorExeName ) ;
		
		// UNICODE FILE
		iLength = -1 ;
		pcFileName = M4CppToANSI( ao_pcPath, iLength ) ;

		dwFileAttr = GetFileAttributes( pcFileName ) ;
		delete [] pcFileName ;

		if( 0xFFFFFFFF == dwFileAttr )
		{
			// -- Nothing to do
			memset( ao_pcPath, 0, ai_iSize ) ;

			GetErrorMessage( iErrorCode, sErrorMsg ) ;
			SETCODEF( M4_ERR_CODE_DESC_WIN_FUNC_ERROR, ERRORLOG, "#*s1*#%d#%s#%s#", iErrorCode, "GetFileAttributes", sErrorMsg.c_str() ) ;

			SETCODEF( M4_ERR_MIND_MONITOR_EXE_NOT_FOUND, ERRORLOG, "#*s1*#%s#%s#%s#", ms_pccAppMonitorExeName, acCurrDir, acExePath ) ;
			

			return M4_ERROR ;
		}
	}

	return M4_SUCCESS ;
}


// -- -------------------------------------------------------------------------------
// -- Gets info about this process.
// --
// -- Args:
// --    ao_acPid: Output process Id.
// --    ao_acStartup: Output start up time.
// --
// -- Returns: M4_SUCCESS / M4_ERROR.
// -- -------------------------------------------------------------------------------
m4return_t ClAppMonitor::GetProcessInfo( m4pchar_t ao_acPid, m4pchar_t ao_acStartup )
{
	DWORD		dwPid ;
	HANDLE		hProcess ;
	BOOL		bRet ;
	FILETIME	stCreatTime ;
	FILETIME	stUnused ;
	m4int32_t	iErrorCode ;
	string		sErrorMsg ;


	// -- Gets current process pid.
	dwPid = GetCurrentProcessId() ;
	ultoa( dwPid, ao_acPid, 10 ) ;


	// -- Gets current application start-up time.
	hProcess = GetCurrentProcess() ;
    bRet = GetProcessTimes( hProcess, &stCreatTime, &stUnused, &stUnused, &stUnused ) ;

	if( 0 == bRet )
	{
		GetErrorMessage( iErrorCode, sErrorMsg ) ;
		SETCODEF( M4_ERR_CODE_DESC_WIN_FUNC_ERROR, ERRORLOG, "#*s1*#%d#%s#%s#", iErrorCode, "GetProcessTimes", sErrorMsg.c_str() ) ;

		return M4_ERROR ;
	}

	ultoa( stCreatTime.dwLowDateTime, ao_acStartup, 10 ) ;


	return M4_SUCCESS ;
}

// -- Class ClAppMonitor ------------------------------------------------------


m4pchar_t BuildMessage( m4pchar_t ai_pcErrorMsg, m4pchar_t ai_sMsg, m4uint16_t ai_iPDUId, int ai_iLine, m4pchar_t ai_sFile )
{
	if( ai_iPDUId == M4_ID_PDU_NEWCANCEL )
	{
		sprintf( ai_pcErrorMsg, "%s. Cancelation PDU. Line <%d> file <%s>", ai_sMsg, ai_iLine, ai_sFile ) ;
	}
	else if( ai_iPDUId == M4_ID_PDU_NEWDISCONNECT )
	{
		sprintf( ai_pcErrorMsg, "%s. Disconnection PDU. Line <%d> file <%s>", ai_sMsg, ai_iLine, ai_sFile ) ;
	}
	else if( ai_iPDUId > 0 )
	{
		sprintf( ai_pcErrorMsg, "%s. PDU Id <%d>. Line <%d> file <%s>", ai_sMsg, ai_iPDUId, ai_iLine, ai_sFile ) ;
	}
	else
	{
		sprintf( ai_pcErrorMsg, "%s. Line <%d> file <%s>", ai_sMsg, ai_iLine, ai_sFile ) ;
	}

	return( ai_pcErrorMsg ) ;
}

m4return_t _SendPDU( ClWinInetConnection* ai_pConnection, ClPDU* ai_pPDU, m4pchar_t ao_acErrorMsg)
{
	m4return_t			iRet = M4_SUCCESS ;
	ClCCRequest*		pRequest = NULL ;
	static m4handle_t	hRequestID = 1 ;
	eRequestStatus		iReqState ;
	ClPDU*				pOutputPDU = NULL ;
	m4uint16_t			iPDUType ;
	m4bool_t			bResend ;
	m4uint16_t			iPDUId ;
	
	
	pRequest = new ClCCRequest( NULL ) ;
	pRequest->SetRequestId( hRequestID ) ;

	ai_pPDU->SetCompressionType( 2 ) ;
	ai_pPDU->SetRequestId( hRequestID++ ) ;

	pRequest->SetPDUInput( ai_pPDU ) ;
	pRequest->SetState( SENDING ) ;

	iPDUId = ai_pPDU->GetIdPDU() ;


	// -- Sending request to application server.
	iRet = ai_pConnection->_SendRawData ( pRequest ) ;

	if( M4_SUCCESS != iRet )
	{
		delete pRequest ;

		BuildMessage( ao_acErrorMsg, "Error sending request", iPDUId, __LINE__, __FILE__ ) ;
		return M4_ERROR ;
	}


	pRequest->SetState( WAITING_FOR_REPLY ) ;

	while( SENDING == pRequest->GetState() )
	{
		Sleep( 50 ) ;
	}


	// -- Receiving response
	pRequest->m_bRetrying = M4_FALSE ;

	iRet = ai_pConnection->_GetAnswerData( pRequest, bResend ) ;

	if( M4_SUCCESS != iRet )
	{
		delete pRequest ;

		BuildMessage( ao_acErrorMsg, "Error getting answer data", iPDUId, __LINE__, __FILE__ ) ;
		return M4_ERROR ;
	}


	// -- Checking response
	iReqState = pRequest->GetState() ;

	switch (iReqState)
	{
		case REPLY_ARRIVED:

			pOutputPDU = pRequest->GetOutputPDU() ;

			if( pOutputPDU )
			{
				iPDUType = pOutputPDU->GetIdPDU() ;


				if( iPDUType == M4_ID_PDU_ACK )
				{
					iRet = M4_SUCCESS ;
				}
				else
				{
					BuildMessage( ao_acErrorMsg, "Response PDU received from server but it is not an ACK", iPDUId, __LINE__, __FILE__ ) ;
					iRet = M4_WARNING ;
				}
			}
			else
			{
				BuildMessage( ao_acErrorMsg, "Response received from server without PDU", iPDUId, __LINE__, __FILE__ ) ;
				iRet = M4_WARNING ;
			}

			break ;

		case ERROR_ARRIVED:
			BuildMessage( ao_acErrorMsg, "Request status: ERROR_ARRIVED", iPDUId, __LINE__, __FILE__ ) ;
			// bg 205475
			// Si es una cancelación y hay algo pendiente, la respuesta es un error.
			iRet = M4_SUCCESS ;
			break ;
		case BROKEN_CONNECTION:
			BuildMessage( ao_acErrorMsg, "Request status: BROKEN_CONNECTION", iPDUId, __LINE__, __FILE__ ) ;
			iRet = M4_ERROR ;
			break ;
		case NO_SERVER:
			BuildMessage( ao_acErrorMsg, "Request status: NO_SERVER", iPDUId, __LINE__, __FILE__ ) ;
			iRet = M4_ERROR ;
			break ;
		case HTTP_ERROR:
			BuildMessage( ao_acErrorMsg, "Request status: HTTP_ERROR", iPDUId, __LINE__, __FILE__ ) ;
			iRet = M4_ERROR ;
			break ;
		case HTTP_TIMEOUT:
			BuildMessage( ao_acErrorMsg, "Request status: HTTP_TIMEOUT", iPDUId, __LINE__, __FILE__ ) ;
			iRet = M4_ERROR ;
			break ;
		default:
			BuildMessage( ao_acErrorMsg, "Unknown request state", iPDUId, __LINE__, __FILE__ ) ;
			iRet = M4_ERROR ;
			break ;
	}

	delete pRequest ;


	return iRet ;
}

// -- -------------------------------------------------------------------------------
// -- Public function for user session disconnection.
// --
// --	ai_pcParameters: Encrypted serialized communication parameters.
// --					 It must contain in the following order:
// --						Meta4 gateway host name.
// --						Gateway listening port.
// --						Meta4 application server host name.
// --						Application server listening port.
// --						Gateway servlet web location.
// --						SSL communications.
// --						Current user Meta4 credential.
// --	ao_acErrorMsg:	In case of error, descriptive error message.
// --
// --	Exported as extern "C" to be called undecorated from m4systray.
// -- -------------------------------------------------------------------------------
m4return_t DisconnectUserSession( m4pcchar_t ai_pcParameters, m4pchar_t ao_acErrorMsg )
{
	m4return_t				iRet = M4_SUCCESS ;
	m4int_t					iBufReqSize = 0;
	m4pchar_t				pcDecrParams = NULL ;
	m4pchar_t				apcParams[NUM_ARGS_SERIALIZED + 1] ;
	m4int_t					iTkIdx = 0 ;
	M4ClString				strGWHost ;
	M4ClString				strAppSrvHost ;
	M4ClString				strCACertPath ;
	m4string_t				strCredential ;
	ClWinInetConnection*	pConnection = NULL ;
	ClProtocol*				pProtocol = NULL ;
	ClNewCancelPDU*			pCancelationPDU = NULL ;
	ClPDU*					pDisconnectionPDU = NULL ;
	

	// -- Decrypt serialized parameters.
	iRet = DecryptTwoWay(ai_pcParameters, strlen(ai_pcParameters), M4PRM_NO_SENSE_VAL + 18, strlen(M4PRM_NO_SENSE_VAL + 18), pcDecrParams, iBufReqSize);
	if (iRet != M4_SUCCESS)
	{
			BuildMessage( ao_acErrorMsg, "Error decrypting input parameters", 0, __LINE__, __FILE__ ) ;
			return M4_ERROR;
	}

	// -- Tokenize parameters.
	apcParams[iTkIdx] = strtok( pcDecrParams, DELIMITER_CHAR ) ;

	while( apcParams[iTkIdx] != NULL && iTkIdx < NUM_ARGS_SERIALIZED )
	{
		apcParams[++iTkIdx] = strtok( NULL, DELIMITER_CHAR ) ;
	}

	
	// -- Check arguments.
	if( NUM_ARGS_SERIALIZED != iTkIdx )
	{
		delete pcDecrParams;
		BuildMessage( ao_acErrorMsg, "Incorrect number of input parameters", 0, __LINE__, __FILE__ ) ;
		return M4_ERROR ;
	}


	strGWHost		= apcParams[0] ;
	strAppSrvHost	= apcParams[2] ;
	strCACertPath	= "" ;
	strCredential	= apcParams[6] ;



	// -- Create the connection.
	pConnection = new ClWinInetConnection( strGWHost, atol(apcParams[1]), strAppSrvHost, atol(apcParams[3]), apcParams[4], (apcParams[5][0]=='0')?false:true, strCACertPath ) ;

	pProtocol = new ClProtocol() ;

	delete pcDecrParams;
	

	// bg 196771
	// Cancelación.
	pCancelationPDU = (ClNewCancelPDU*)pProtocol->GetPDU( M4_ID_PDU_NEWCANCEL ) ;
	// La destrucción de la request en _SendPDU destruirá la pdu.

	if( NULL == pCancelationPDU )
	{
		delete pConnection ;
		delete pProtocol ;

		BuildMessage( ao_acErrorMsg, "Error getting a protocol cancelation PDU", 0, __LINE__, __FILE__ ) ;
		return M4_ERROR ;
	}

	pCancelationPDU->SetCancelRequestId( 0 ) ;
	pCancelationPDU->SetCredential( strCredential ) ;
	
	iRet = _SendPDU( pConnection, pCancelationPDU, ao_acErrorMsg ) ;

	if( M4_SUCCESS != iRet )
	{
		delete pConnection ;
		delete pProtocol ;

		return iRet ;
	}

	// bg 243291
	Sleep( 1000 ) ;

	// -- Desconexión.
	pDisconnectionPDU = pProtocol->GetPDU( M4_ID_PDU_NEWDISCONNECT ) ;
	// La destrucción de la request en _SendPDU destruirá la pdu.

	delete pProtocol ;

	if( NULL == pDisconnectionPDU )
	{
		delete pConnection ;

		BuildMessage( ao_acErrorMsg, "Error getting a protocol disconnection PDU", 0, __LINE__, __FILE__ ) ;
		return M4_ERROR ;
	}


	// -- Setting PDU parameters.
	pDisconnectionPDU->SetCredential( strCredential ) ;

	iRet = _SendPDU( pConnection, pDisconnectionPDU, ao_acErrorMsg ) ;

	delete pConnection ;


	return iRet ;
}

#endif
