//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             servidor.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:9/26/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines the main function for Meta4 App Server Controller
//
//==============================================================================


#include <stdlib.h>
#include <m4types.hpp>
#include <m4stl.hpp>
#include "appctldemon.hpp"                   
#include <stdio.h>
#include <m4thread.hpp>
//#include "acceptcon.hpp"
//#include "listencon.hpp"
#include "launchappserver.hpp"
#include "commonvariables.hpp"
#include "estatus.hpp"
#include "m4log.hpp"
#include "m4date.hpp"

#include "m4memdbg.h"
#include "m4srvtrace.h"

#define M4_SRV_TRACE_COMP_ID "APPCTL"


#ifdef _WINDOWS

#include "appctlservice.hpp"                   

#	include <process.h>
#	include <windows.h>

#else

#include <unistd.h>
#define SERVICE_STOPPED                0x00000001
#define SERVICE_START_PENDING          0x00000002
#define SERVICE_STOP_PENDING           0x00000003
#define SERVICE_RUNNING                0x00000004
#define SERVICE_CONTINUE_PENDING       0x00000005
#define SERVICE_PAUSE_PENDING          0x00000006
#define SERVICE_PAUSED                 0x00000007

#define ERROR							0

#define ERROR_SUCCESS                   0L
#define NO_ERROR						0L                                                 // dderror

#endif


#define MAX_LINE 1024


// Variables globales....
// las necesito para stop_demon()

//ClCommonVariables * poCommonVariables; 
ClTSAP * potsap;
ClLaunchAppServer * poLaunchAppServer = 0;




m4pchar_t ComposeAppCtlConfigPath(m4pchar_t ai_psAppCtlExePath)
{
	m4pchar_t psResult = M4APPCTL_CONFIGURATION_FILE;
	m4pchar_t psCurrentLimit = 0;
	m4pchar_t psForeLimit = 0;


	#ifdef _WINDOWS

	if (0 != ai_psAppCtlExePath)
	{

		// Get the configuration file parent directory

		psCurrentLimit = ai_psAppCtlExePath;
		do
		{
			psCurrentLimit = strstr(psCurrentLimit, M4APPCTL_BIN_RELATIVE_DIR);

			if (0 != psCurrentLimit)
			{
				psForeLimit = psCurrentLimit;
				psCurrentLimit += strlen(M4APPCTL_BIN_RELATIVE_DIR);
			}
		} while (0 != psCurrentLimit);

		if (0 != psForeLimit)
		{
			M4_MDBG_NEW(psResult, psResult = new m4char_t[psForeLimit - ai_psAppCtlExePath + strlen(M4APPCTL_CONFIG_RELATIVE_PATH) + 1]);
			(void) strncpy(psResult, ai_psAppCtlExePath, psForeLimit - ai_psAppCtlExePath);
			psResult[psForeLimit - ai_psAppCtlExePath] = '\0';
			(void) strcat(psResult, M4APPCTL_CONFIG_RELATIVE_PATH);
			psResult[psForeLimit - ai_psAppCtlExePath + strlen(M4APPCTL_CONFIG_RELATIVE_PATH)] = '\0';	
		}
		else
		{
				psResult = 0;
		}
	}

	#endif

	#ifdef _UNIX

		// Suppose always that the Current Working Directory is "<some path>/bin"
		// and the configuration directory is "../conf/m4appctl.conf"
		m4pchar_t psCwd;
		if (0 != (psCwd = getcwd(NULL, APP_INFO_MAX_BUFFER))) {

			M4_MDBG_NEW(psResult, psResult = new m4char_t[strlen(psCwd) + strlen(M4APPCTL_CONFIGURATION_FILE) + 2]);
			(void) strcpy(psResult, psCwd);
			(void) strcat(psResult, "/");
			(void) strcat(psResult, M4APPCTL_CONFIGURATION_FILE);
		}
		else
			psResult = 0;

	#endif

	return (psResult);
}


m4pchar_t GetAppCtlConfigPath()
{
	m4pchar_t psResult = 0;

	#ifdef _WINDOWS

		TCHAR szPath[512];

		if (0 !=  GetModuleFileName( NULL, szPath, 512 ))
		{
			psResult = ComposeAppCtlConfigPath(szPath);
		}

	#endif


	#ifdef _UNIX

		psResult = ComposeAppCtlConfigPath(NULL);

	#endif

	return (psResult);
}


m4int8_t Daemon_main (m4pchar_t ai_pszConfigFilePath)
{
	m4int8_t iRet ;
	m4pchar_t pBuffer=HOST;
	m4pchar_t psAppCtlConfigPath = 0;


	M4_SRV_GENERAL_BLOCK_N( Blk, "M4 Application Server Controller Starting" ) ;

	//SocketInit();

	M4_MDBG_NEW(poLaunchAppServer, poLaunchAppServer = new ClLaunchAppServer); 


	//Leo el puerto de adminstracion
	
	#ifdef _WINDOWS

	ReportStatusToSCMgr( SERVICE_RUNNING , NO_ERROR , 0 );

	#endif

	if (0 == ai_pszConfigFilePath)
		psAppCtlConfigPath = GetAppCtlConfigPath();
	else
	{
		M4_MDBG_NEW(psAppCtlConfigPath, psAppCtlConfigPath = new m4char_t[strlen(ai_pszConfigFilePath) + 1]);
		(void) strcpy(psAppCtlConfigPath, ai_pszConfigFilePath);
	}

	poLaunchAppServer -> SetAppCtlConfigPath(psAppCtlConfigPath);

	if ( M4_SUCCESS == ( iRet = poLaunchAppServer -> Initialize() ) )
	{
		Blk.End() ;

		M4_SRV_GENERAL_N("Running ...") ;

		iRet = poLaunchAppServer -> Run () ;

	} else {
		Blk.End() ;
	} ;


	M4_SRV_GENERAL_BLOCK_N( BlkEnd, "M4 Application Server Controller Stopping" ) ;

	M4_MDBG_DELETE(poLaunchAppServer,delete poLaunchAppServer);
	poLaunchAppServer=NULL ;


	//SocketFinish();

	BlkEnd.End() ;

#ifdef _WINDOWS

	ReportStatusToSCMgr( SERVICE_STOPPED , NO_ERROR , 0 );

#endif


#ifdef _UNIX

	// codigo de salida para que se entere el padre de que ha acabado bien
	_exit(0);

#endif

	return iRet ;

}





m4return_t Stop_Daemon()
{

	m4char_t Temp_Status_File[64];

	m4char_t * pTemp_Status_File = Temp_Status_File;

#ifdef _WINDOWS	
	ReportStatusToSCMgr(
			SERVICE_STOP_PENDING, // Estado del servicio
			NO_ERROR,              // codigo de salida
			30000);                 // Tiempo de espera en milisegundos	
#endif

	M4_SRV_GENERAL_N( "Iniciando shutdown del M4 Application server controller" );

	//M4_SRV_GENERAL_N( "Iniciando shutdown de los servidores" );
	
	// tiro abajo todos los servers
//	poLaunchAppServer->ShutDownAll (poCommonVariables->m_ulTimeout_ShutDown);
	// condicion de los bucles accept y select
	poLaunchAppServer -> Stop () ;

	return M4_SUCCESS;
}





