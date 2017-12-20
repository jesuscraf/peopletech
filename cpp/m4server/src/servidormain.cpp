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
//    This module defines the main function for Meta4 App Server
//
//==============================================================================


#include <stdlib.h>
#include "m4stl.hpp"
#include "m4types.hpp"
#include "subsystemids.hpp"
#include "clsubsystem.hpp"
#include "tsap.hpp"
#include "m4cryptc.hpp"
#include "m4cryptstore.hpp"
#include <serverservice.hpp>
#include "m4string.hpp"
#include "m4trace.hpp"
#include "regedit.hpp"
#include "localdefs.hpp"
#include "fileutil.hpp"
#include "clm4objservice.hpp"
#include "clfactm4objservice.hpp"
#include "snmpadmininstance.hpp"
#include "m4meminfo.hpp"
#include "m4xmlreg.hpp"
#include "m4java.h"
#include "m4jcontext.hpp"
#include "m4objreg.hpp"


#ifdef _WINDOWS
#	include <process.h>
#	include <windows.h>
#else
#	include <unistd.h>
#endif


#define	M4CH_DUMMY_B2	"\x2\x3\x8\x1\x9\x6\x5\xF\xD\x8\xF\x8\xB\x4\x7\x7\xF\x6\x4\xE\x4\xE\xE\x5\xB\xE\x6\x7\xF\x3\x7\x2\xB\x6\xC\x4\xD\xB\x6\x7\x8\x3\xE\x8\xA\xB\x7\x9\x6\x6\x7\x9\xB\x6\x4\x8\xE\xF\x3\x8\x8\xC\xA\x0\x4\x2\x6\x8\x4\xC\x4\xD\x9\x2\x8\x4\x8\x8\x6\x8\xF\x1\x1\xF\xC\x1\x7\xE"

m4bool_t close_service = M4_FALSE ;
ClCondition	stopper ;
extern m4bool_t service_started ;

#define OBLPATH			"startup.obl"

void GetWelcomeMessage(M4ClString& ao_wellcome);



void Usage(const char *filename)
{
	cout << "Usage: " << endl;
	cout << "   " << filename << " startup-obl-name encrypted-file" << endl; 	
	cout << "   " << filename << " startup-obl-name user password" << endl; 
	
	exit(-1);
}


void SendSignalReady()
{
#ifndef _UNIX
	return;
#else
	// get parent pid.
	pid_t parentPID = getppid();
	m4TraceHeaderLevel(4, cout << "Server parent PID: " << parentPID << endl);
	
	// get parent PID from environment. This variable is set from m4appctl
	// application.
	char *cParentPID = getenv("M4APPCTL_PID");
	if (NULL == cParentPID)
	{
		m4TraceHeaderLevel(3, cout << "M4APPCTL_PID environment variable not defined" << endl);
		return;
	}
	
	// convert to integer
	int iParentPID = atoi(cParentPID);
	if (iParentPID != parentPID)
	{
		m4TraceHeaderLevel(4, cout << "environment PID: " << iParentPID << endl);
		return;
	}
	
	// send signal SIGUSR1 to my parent.
	m4TraceHeaderLevel(3, cout << "Sending SIGUSR1 to server parent... " << endl);
	kill(parentPID, SIGUSR1);	
#endif
}



m4return_t Decrypt ( m4pchar_t ai_szFile , m4pchar_t ao_szUser , m4int_t ai_iUserLen , m4pchar_t ao_szPassword , m4int_t ai_iPasswordLen )
{
	ClCryptStore * cStore;
	m4return_t iRet ;
	m4int_t iUserLen , iPasswordLen ;
	
	// Se inizializan los sockets para poder llamar a gethostname().
	SocketInit ( );		
	
	// Conseguir el Pin
	m4char_t szPin [200];
#ifdef _WINDOWS
	gethostname ( szPin, 200 );
#else
	m4uint32_t hostid = gethostid ();
	sprintf (szPin, "%u", hostid);
#endif
	
	
	cStore = new ClCryptStore ( ai_szFile , szPin , strlen (szPin ) );
	
	
	iRet = cStore -> ExtractData( USER, 4,
		ao_szUser,ai_iUserLen ,iUserLen );
	
	if ( iRet != M4_SUCCESS )
	{
		cout << "USER not found in encrypted file " << ai_szFile << endl;		
		return M4_ERROR ;
	}
	
	ao_szUser [iUserLen ] = 0 ;

	iRet = cStore -> ExtractData( PASSWORD, 8,
		ao_szPassword,ai_iPasswordLen , iPasswordLen );
	
	if ( iRet != M4_SUCCESS )
	{
		cout << "PASSWORD not found in encrypted file " << ai_szFile << endl;		
		return M4_ERROR;
	}
	ao_szPassword [iPasswordLen ] = 0 ;
	
	delete cStore ;
	
	return M4_SUCCESS ;
}



// bg 190896
#ifdef _WINDOWS

	typedef HANDLE( *pfGetCRTHeap_t )( void ) ;

	m4return_t _EnableLFH()
	{
		HMODULE			hModule;
		pfGetCRTHeap_t	pfGetCRTHeap;
		HANDLE			hCRTHeap;
		BOOL			bResult;
		ULONG			HeapInfo;


		hModule = LoadLibrary( "msvcrt.dll" );

		if( hModule == NULL )
		{
			cerr << "Failed to load CRT library with error " << GetLastError() << endl;
			return M4_ERROR;
		}

		pfGetCRTHeap = (pfGetCRTHeap_t)( GetProcAddress( hModule, "_get_heap_handle" ) );

		if( pfGetCRTHeap == NULL )
		{
			cerr << "Failed to get function address in CRT library with error " << GetLastError() << endl;
			FreeLibrary( hModule );
			return M4_ERROR;
		}

		hCRTHeap = pfGetCRTHeap();

		FreeLibrary( hModule );

		if( hCRTHeap == NULL )
		{
			cerr << "Failed to retrieve CRT heap with error " << GetLastError()  << endl;
			return M4_ERROR;
		}

		
		bResult = HeapQueryInformation( hCRTHeap, HeapCompatibilityInformation, &HeapInfo, sizeof( HeapInfo ), NULL );

		if( bResult == FALSE )
		{
			cerr << "Failed to retrieve heap features with error " << GetLastError() << endl;
			return M4_ERROR;
		}

		if( HeapInfo != 2 )
		{
			HeapInfo = 2;

			bResult = HeapSetInformation( hCRTHeap, HeapCompatibilityInformation, &HeapInfo, sizeof( HeapInfo ) );
			
			if (bResult == FALSE) 
			{
				cerr << "Failed to enable LFH (" << hCRTHeap << ") with error " << GetLastError() << endl;
				return M4_ERROR;
			}
		}


		return M4_SUCCESS;
	}

#endif




int server_main (int argc, char** argv)
{	
	m4char_t				OblPath [200]	= OBLPATH ;
	ClSystem				*poSystem ;
	m4char_t				szUser [1000];
	m4char_t				szPassword[1000];
	m4pchar_t				pcCryptPassword = NULL;
	m4int_t					iszCryptPasswordLen;
	m4uint32_t				iAppctlTimeControl = 0;
	m4pchar_t				szProcessName =NULL;
	m4pchar_t				szConfigName =NULL;
	m4pchar_t				szPropertiesObl =NULL;
	m4pchar_t				szLiteralsObl =NULL;
	m4pchar_t				szLog =NULL;
	m4pchar_t				szTablesObl =NULL;
	m4pchar_t				szDescripcion =NULL;
	m4pchar_t				szDispatcher =NULL;
	m4uint32_t				index = 1 ;
	m4uint32_t				iAppctlControlport = 0;
	m4uint32_t				iBaseport = 0 ;
	m4return_t iRet ;
	m4bool_t SwErr ;
	
	/* Bug 0113374
	Se establece el número de descriptores a 1024
	*/
	SetFileDescriptorsLimit( 1024 ) ;

	M4ClString wellcomeMessage;
	GetWelcomeMessage(wellcomeMessage);
	cout << wellcomeMessage << endl;

	
	// bg 190896
	#ifdef _WINDOWS
		#ifndef _DEBUG
			_EnableLFH();
		#endif
	#endif


    // check if another appserver is running from the current directory.
    if (isAlreadyRunning())
    {
        int sleepTime = 5;
        cerr << "Another instance of " << argv[0] << " is running from this directory." << endl;
        cerr << "Delete the file called 'pid' in the current directory and try again." << endl;
        cerr << "Exiting in " << sleepTime << " seconds..." << endl;
        m4sleep(sleepTime);
        return -1;
    }


	while(argv[index] )
	{
		SwErr= M4_FALSE ;

		if ( argv[index][0]=='-' )
		{
			switch(argv[index][1])
			{
			case 't':
			case 'T':
				if ( ! stricmp( &argv[index][1] , "time" ) )
				{
					argc--;
					if (! argv[++index] )
						break ;
					if ( argv[index][0] =='-' )
						break ;
					iAppctlTimeControl = atoi(argv[index++]);
					argc--;
				} else if ( ! stricmp( &argv[index][1] , "tables" ) ) {
					argc--;
					if (! argv[++index] )
						break ;
					if ( argv[index][0] =='-' )
						break ;
					szTablesObl = new m4char_t [ strlen (argv [index] ) +1 ] ;
					strcpy ( szTablesObl , argv[(index)++] );
					argc--;
				} else SwErr=M4_TRUE ;
				break ;
			case 'c':
			case 'C':
				if ( ! stricmp( &argv[index][1] , "config" ) )
				{
					argc--;
					if (! argv[++index] )
						break ;
					if ( argv[index][0] =='-' )
						break ;
					szConfigName = new m4char_t [ strlen (argv [index ] ) +1 ] ;
					strcpy ( szConfigName , argv[(index)++] );
					argc--;
				} else SwErr=M4_TRUE ;
				break;
			case 'n':
			case 'N':
				if ( ! stricmp( &argv[index][1] , "name" ) )
				{
					argc--;
					if (! argv[++index] )
						break ;
					if ( argv[index][0] =='-' )
						break ;
					szProcessName = new m4char_t [ strlen (argv [index ] ) +1 ] ;
					strcpy ( szProcessName , argv[index++] );
					argc--;
				} else SwErr=M4_TRUE ;
				break;
			case 'b' :
				if ( ! stricmp( &argv[index][1] , "baseport" ) )
				{
					argc--;
					if (! argv[++index] )
						break ;
					if ( argv[index][0] =='-' )
						break ;
					iBaseport = atoi(argv[index++]);
					argc--;
				} else SwErr=M4_TRUE ;
				break;

			case 'p':
			case 'P':
				if ( ! stricmp( &argv[index][1] , "properties" ) )
				{
					argc--;
					if (! argv[++index] )
						break ;
					if ( argv[index][0] =='-' )
						break ;
					szPropertiesObl = new m4char_t [ strlen (argv [index ] ) +1 ] ;
					strcpy ( szPropertiesObl, argv[(index)++] );
					argc--;
				} else if ( ! stricmp( &argv[index][1] , "port" ) ) {
					argc--;
					if (! argv[++index] )
						break ;
					if ( argv[index][0] =='-' )
						break ;
					iAppctlControlport = atoi(argv[(index)++]);
					argc--;
				} else SwErr=M4_TRUE ;
				break;
			case 'l':
			case 'L':
				if ( ! stricmp( &argv[index][1] , "literals" ) )
				{
					argc--;
					if (! argv[++index] )
						break ;
					if ( argv[index][0] =='-' )
						break ;
					szLiteralsObl = new m4char_t [ strlen (argv [index ] ) +1 ] ;
					strcpy ( szLiteralsObl, argv[(index)++] );
					argc--;
				} else  if ( ! stricmp( &argv[index][1] , "log" ) )	{
					if (! argv[++index] )
						break ;
					if ( argv[index][0] =='-' )
						break ;
					szLog= new m4char_t [ strlen (argv [index ] ) +1 ] ;
					strcpy ( szLog, argv[(index)++] );
					argc--;
				} else SwErr=M4_TRUE ;
				break;
			case 'd':
			case 'D':
				if ( ! stricmp( &argv[index][1] , "description" ) )
				{
					argc--;
					if (! argv[++index] )
						break ;
					if ( argv[index][0] =='-' )
						break ;
					szDescripcion = new m4char_t [ strlen (argv [index ] ) +1 ] ;
					strcpy ( szDescripcion , argv[(index)++] );
					argc--;
				} else if ( ! stricmp( &argv[index][1] , "dispatcher" ) ) {
					argc--;
					if (! argv[++index] )
						break ;
					if ( argv[index][0] =='-' )
						break ;
					szDispatcher = new m4char_t [ strlen (argv [index ] ) +1 ] ;
					strcpy ( szDispatcher , argv[(index)++] );
					argc--;
				} else SwErr=M4_TRUE ;
				break;
			default :
				SwErr=M4_TRUE ;
			}
			if (SwErr) {
				cout << "Unknown parameter : " << argv[index] << "\n" << flush ;
				argc--;
				if (! argv[++index] )
					break ;
				if ( argv[index][0] !='-' )
				{
					index ++ ;
					argc -- ;
				}
			} ;
		} else index++;
	}

	// input argument process.
	switch (argc)
	{
	case 3 :
		strcpy ( OblPath , argv[1] ) ;		
		iRet = Decrypt (argv[2], szUser, 1000, szPassword, 1000);
		if (iRet == M4_ERROR ) 
		{
			m4sleep (5);
			return -1;
		}
		break;
		
	case 4 :
		strcpy ( OblPath , argv[1] ) ;		
		strcpy ( szUser , argv [2] ) ;		
		strcpy ( szPassword , argv [3] ) ;		
		break ;
		
	default :
		Usage(argv[0]);
		break;
		
	}
	
    // bugid 0099545.
    // Inicializo aquí el control de memoria para evitar problemas de locale en
    // multithreading.
    m4uint64_t uiPrivateBytes;
    m4double_t dCPU;
    M4GetProcessMem(uiPrivateBytes);
    M4GetSystemCPU(dCPU);


	iRet = EncryptTwoWay(szPassword, strlen (szPassword), M4CH_DUMMY_B2 + 23, strlen(M4CH_DUMMY_B2 + 23), pcCryptPassword, iszCryptPasswordLen);
	
	// inizializacion del server.	
	M4XmlRegistry::Instance();
	poSystem = ClSystem::Instance (  ) ;

	stopper.Init () ;

	iRet = poSystem -> StartUp ( & stopper , OblPath , szUser , pcCryptPassword , NULL , szPropertiesObl , szLiteralsObl , szTablesObl , szLog , szProcessName , szConfigName  , iAppctlTimeControl , iAppctlControlport , szDispatcher , iBaseport  );

	// Borrar el buffer de la password encriptada.
	delete pcCryptPassword;

	if (iRet  == M4_SUCCESS )
	{
        // register the startup user in snmp.
        M4ISnmpAdministrator* poSnmpAdmin = M4SnmpAdminInstance::Get();
        if (NULL != poSnmpAdmin)
        {
            poSnmpAdmin->RegisterUser(szUser, szPassword);
        }

		// Se inicializa el JAVA
		bool	bEnabled = M4_FALSE ;

		ClM4ObjService	*poService = ClFactM4ObjService::GetMasterObjService() ;

		if( poService != NULL )
		{
			ClVMBaseEnv	*poEnvironment = poService->GetpRegistry() ;

			if( poEnvironment != NULL )
			{
				if( poEnvironment->GetJavaAllowInProcess() == M4_TRUE )
				{
					// bugid 149341.
					// La inicialización bajo demanda de la JVM causa problemas de estabilidad
					// por lo tanto inicializamos desde la máquina virtual si procede. Es necesario
					// que esté configurada la variable M4JAVAHOME
					// (la invocación posterior ya será en el thread correspondiente)
					// No me interesa el contexto. Sólo que exista
					M4JVM_GetJavaEnv();
				}

				M4JavaContext	oContext ;

				iRet = oContext.Open( poEnvironment, "m4server", NULL, NULL ) ;

				if( iRet == M4_SUCCESS )
				{
					m4uint32_t	iMethod = 0 ;

					iRet = oContext.NewMethod( "com.meta4.javaservice.M4JavaService", "ping", NULL, 0, iMethod, M4_FALSE, "m4server" ) ;

					if( iRet == M4_SUCCESS )
					{
						bEnabled = M4_TRUE ;
					}
				}

				oContext.Close( M4_TRUE ) ;
			}
		}

		if( bEnabled == M4_TRUE )
		{
			cout << "Java Virtual Machine enabled" << endl ;
		}
		else
		{
			cout << "Java Virtual Machine disabled" << endl ;
		}
		cout << "-----------------------------------------------------------" << endl ;

        SendSignalReady();
#ifdef _WINDOWS
		//
		if ( service_started )
		ReportStatusToSCMgr(SERVICE_RUNNING , NO_ERROR, 0)  ;
#endif
		stopper.Wait ( ) ;
	}

	poSystem -> ShutDown ( ) ;

	delete poSystem;

	M4JVM_FreeJVM();

	cout << endl << "Meta4 Application Server shuted down" << endl;	
	m4sleep (5);
			 
	return M4_SUCCESS;
}

m4return_t StopServer()
{
	/*ClSystem * poSystem ;*/


	close_service = M4_TRUE ;

	stopper.Signal ( ) ;

/*
	poSystem = ClSystem::Instance() ;
			poSystem -> ShutDown ( ) ;
*/
	return M4_SUCCESS;
}




