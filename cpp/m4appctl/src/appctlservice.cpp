#include <m4win.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <m4stl.hpp>
#include <m4types.hpp>
#include <m4log.hpp>
#include <appctlservice.hpp>
#include <m4date.hpp>
#include <appctldemon.hpp>
#include <fileutil.hpp>


#include "m4memdbg.h"
#include "m4srvtrace.h"
#include <m4appctlres.hpp>

#include "m4unicode.hpp"


#define M4_SRV_TRACE_COMP_ID "APPCTL"


#ifdef _WINDOWS

#include <process.h>
#include <tchar.h>

#include <direct.h>

#include "estatus.hpp"


#endif

// Variables internas
#ifdef _WINDOWS

m4return_t Stop_Daemon() ;

m4bool_t service_started = M4_TRUE ;

SERVICE_STATUS ssStatus; // Estado actual del servicio

SERVICE_STATUS_HANDLE   sshStatusHandle; //handle del manejador de los codigos de control


// Máximo para el nombre de un servicio.
#define MAX_SERVICE_NAME_LEN	256

// Máximo para el nombre visible de un servicio.
#define MAX_SERVICE_DISPLAY_NAME_LEN	256

// ----------------------------------------------------------------------------
// -- Soporte multisite
// -- Construye el nombre del servicio a partir del id del site.
// -- 
// -- "M4appctl-SiteId"
// ----------------------------------------------------------------------------
static m4return_t	_BuildServiceName( m4pchar_t ai_pcSiteId, m4pchar_t& ao_pcServiceName )
{
	m4return_t		iRet = M4_SUCCESS ;
	static m4char_t	acName[ MAX_SERVICE_NAME_LEN ] ;
	size_t			iSize ;


	if( ai_pcSiteId == NULL || *ai_pcSiteId == 0 )
	{
		ao_pcServiceName = TEXT(SZSERVICENAME) ;
	}
	else
	{
		memset( acName, 0, MAX_SERVICE_NAME_LEN ) ;
		ao_pcServiceName = acName ;

		iSize = strlen( TEXT(SZSERVICENAME) ) + strlen( ai_pcSiteId ) + 2 ;

		if( iSize <= MAX_SERVICE_NAME_LEN )
		{
			sprintf( acName, "%s-%s", TEXT(SZSERVICENAME), ai_pcSiteId ) ;
		}
		else
		{
			iRet = M4_ERROR ;
		}
	}


	return iRet ;
}

// ----------------------------------------------------------------------------
// -- Soporte multisite
// -- Construye el nombre visible del servicio a partir del nombre del servicio.
// -- 
// -- "Meta4 Application Server Controller (ServiceId)"
// ----------------------------------------------------------------------------
static m4return_t	_BuildServiceDisplayName( m4pchar_t ai_pcServiceName, m4pchar_t& ao_pcDisplayName )
{
	m4return_t		iRet = M4_SUCCESS ;
	static m4char_t	acDisplayName[ MAX_SERVICE_DISPLAY_NAME_LEN ] ;
	size_t			iSize ;


	if( _stricmp( ai_pcServiceName, TEXT(SZSERVICENAME) ) == 0 )
	{
		ao_pcDisplayName = TEXT(SZSERVICEDISPLAYNAME) ;
	}
	else
	{
		memset( acDisplayName, 0, MAX_SERVICE_DISPLAY_NAME_LEN ) ;
		ao_pcDisplayName = acDisplayName ;

		iSize = strlen( TEXT(SZSERVICEDISPLAYNAME) ) + strlen( ai_pcServiceName ) + 4 ;

		if( iSize <= MAX_SERVICE_DISPLAY_NAME_LEN )
		{
			sprintf( acDisplayName, "%s (%s)", TEXT(SZSERVICEDISPLAYNAME), ai_pcServiceName ) ;
		}
		else
		{
			iRet = M4_ERROR ;
		}
	}

	return iRet ;
}


// ----------------------------------------------------------------------------
// -- Soporte multisite
// -- Recuperar el identificador del servicio actual desde el fichero de
// -- configuración.
// ----------------------------------------------------------------------------
static m4return_t	_GetCurrentServiceName( m4pchar_t& ao_pcServiceName )
{
	m4return_t		iRet ;
	DWORD			dwRet ;
	m4pcchar_t		pccConfigFile = NULL ;
	static m4char_t	acSiteId[ MAX_SERVICE_NAME_LEN ] ;


	pccConfigFile = GetAppCtlConfigPath() ;
	
	memset( acSiteId, 0, MAX_SERVICE_NAME_LEN ) ;

	dwRet = GetPrivateProfileString( GLOBAL_SECTION, SITE_ID, "", acSiteId, MAX_SERVICE_NAME_LEN, pccConfigFile ) ;


	iRet = _BuildServiceName( acSiteId, ao_pcServiceName ) ;


	return iRet ;
}

#endif


void PrintOptions()
{
	cout << endl << "Use these options: " << endl << endl ;

	cout << "	No parameters (to start the current App. Controller service)" << endl ;
	cout << "	-install (to install the App. Controller as a service)" << endl ;
	cout << "	-install SiteId (to install the App. Controller as a service specifying the Server Site Id)" << endl ;
	cout << "	-erase   (to stop and uninstall or uninstall the App. Controller service)" << endl ;
	cout << "	-erase SiteId (to stop and uninstall or uninstall the App. Controller service specifying the Server Site Id)" << endl ;
	cout << "	-run <Configuration_file_path>  (to run the App. Controller as an executable - no Service)" << endl ;
	m4sleep (4) ;
}


//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
int main( int argc, char *argv[])
{ 
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

	/* Bug 0113374
	Se establece el número de descriptores a 1024
	*/
	SetFileDescriptorsLimit( 1024 ) ;


	int iErrorCode = 0;

	M4SrvConfigDevelopEnvByEnvVble( M4_SRV_TRACE_COMP_ID) ;


#ifdef _WINDOWS


// si no pasamos parámetros, el SCM comienza la ejecución
// del servcicio mediante la llamada a StartServiceCtrlDispatcher

		m4pchar_t	pcServiceName = NULL ;
		m4pchar_t	pcDisplayName = NULL ;


		SERVICE_TABLE_ENTRY dispatchTable[] =
		{
			{ TEXT(SZSERVICENAME), (LPSERVICE_MAIN_FUNCTION)ServiceMain },
			{ NULL, NULL }
		};

		switch ( argc )
		{
        case 1:
			// Ejecución sin parámetros: hay que arrancar el servicio actual.
			_GetCurrentServiceName( pcServiceName ) ;

			dispatchTable[0].lpServiceName = pcServiceName ;

			if (0 != StartServiceCtrlDispatcher(dispatchTable))
            {
				service_started = M4_TRUE;
            }
			else
			{
                // Get error on start.
                LPVOID lpMsgBuf;

				FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					iErrorCode,
					0, //MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
				);

				// Process any inserts in lpMsgBuf.
				// ...
				M4_SRV_GENERAL("Error registering service: %0:s", TEXT((LPCTSTR)lpMsgBuf));
                //cout << "Error registering service: " << TEXT((LPCTSTR)lpMsgBuf) << endl;

				// Free the buffer.
				LocalFree( lpMsgBuf );
				
                
                // try another method.
                SC_HANDLE hSCM = OpenSCManager( NULL, NULL, SC_MANAGER_CONNECT ); 
                SC_HANDLE hService = OpenService( hSCM, pcServiceName, SERVICE_START ); 

                if (!StartService( hService, 0, NULL ))
                {
                    iErrorCode = GetLastError();
					LPVOID lpMsgBuf;

					FormatMessage( 
						FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						iErrorCode,
						0, //MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf,
						0,
						NULL 
					);
				    M4_SRV_GENERAL("Error open service: %0:s", TEXT((LPCTSTR)lpMsgBuf));
					service_started = M4_FALSE ;
                }
                else
                {
                    service_started = M4_TRUE ;
                }

                CloseServiceHandle( hService ); 
                CloseServiceHandle( hSCM ); 
			}
			
			break;
		case 2:
			if((*argv[1] == '-') || (*argv[1] == '/')) 
			{
				// Instalación/desinstalación del servicio con el nombre por defecto.
				_BuildServiceName( NULL, pcServiceName ) ;
				_BuildServiceDisplayName( pcServiceName, pcDisplayName ) ;

				if ( _stricmp( "install", argv[1]+1 ) == 0 )
				{
					return (InstallService( pcServiceName, pcDisplayName ));
				}
				else if ( _stricmp( "erase", argv[1]+1 ) == 0 )
				{
					return (RemoveService( pcServiceName, pcDisplayName ));
				}
				else
				{
					M4_SRV_ERROR_N(M4_ERR_APPCTL_COMMAND_LINE_OPTION, "Invalid option");
					PrintOptions();
				}
			}
			else
			{
				M4_SRV_ERROR_N(M4_ERR_APPCTL_COMMAND_LINE_OPTION, "Invalid option");
				PrintOptions();
			}
			break ;
		case 3:
			if((*argv[1] == '-') || (*argv[1] == '/')) 
			{
				if ( _stricmp( "run", argv[1]+1 ) == 0 )
				{
					if (argv[2])
						Daemon_main ( argv[2] ) ;
					else
					{
						M4_SRV_ERROR_N(M4_ERR_APPCTL_CONFIG_FILE_OPTION, "No configuration file specified");
						PrintOptions();
					}
				}
				else if( _stricmp( "install", argv[1]+1 ) == 0 )
				{
					// Instalación del servicio con el nombre de site especificado.
					_BuildServiceName( argv[2], pcServiceName ) ;
					_BuildServiceDisplayName( pcServiceName, pcDisplayName ) ;

					return( InstallService( pcServiceName, pcDisplayName ) ) ;
				}
				else if( _stricmp( "erase", argv[1]+1 ) == 0 )
				{
					// Desinstalación del servicio con el nombre de site especificado.
					_BuildServiceName( argv[2], pcServiceName ) ;
					_BuildServiceDisplayName( pcServiceName, pcDisplayName ) ;

					return( RemoveService( pcServiceName, pcDisplayName ) ) ;
				}
				else
				{
					M4_SRV_ERROR_N(M4_ERR_APPCTL_COMMAND_LINE_OPTION, "Invalid option");
					PrintOptions();
				}
			}
			else
			{
				M4_SRV_ERROR_N(M4_ERR_APPCTL_COMMAND_LINE_OPTION, "Invalid option");
				PrintOptions();
			}
			break;
		default:
			M4_SRV_ERROR_N(M4_ERR_APPCTL_COMMAND_LINE_OPTION, "Invalid option");
			PrintOptions();

			service_started = M4_FALSE ;
			break ;
		}

  
#else


	int pid;
	int estado=1;
	int tcp_close_wait;

	while(estado!=0)
	{

		M4_SRV_GENERAL_N("Comenzando el servicio\n\n")

		if((pid=fork()) == -1)
		{
		// error en creacíón de proceso demonio;

		}
		else if(pid==0)
		{

				tcp_close_wait = tcp_close_wait_interval();

				M4_SRV_GENERAL("esperando %0:d segundos a que se cierren los puertos", tcp_close_wait)

				m4sleep(tcp_close_wait);

				Daemon_main ( 0 );
					exit(1);

		}
		else{

				wait(&estado);		
				M4_SRV_GENERAL( "El controller ha acabado con estado %0:s" , estado );

		}


	}

	M4_SRV_GENERAL_N( "Sólo puedo salir del while si me han hecho un Stop_Daemon()" ) ;


#endif

 return 0;
}

#ifdef _UNIX

#ifdef _SOLARIS 

static int ndd_name_io( ndd_dev_t * np, int cmd, var_t * vp)
{
	char * p;

	int i;

	struct strioctl str_cmd;

	// abre el dispositivo si no esta abierto todavia
	if(np->ndd_sd == 0)
	{

		np->ndd_sd = open(np->ndd_dev_name, O_RDWR);

		if (np->ndd_sd == -1)
		{
			return -1;
		}

	}

	// limpiamos el buffer

	memset( ndd_buf, '\0', sizeof( ndd_buf ) ); 

	// se actuaiza el stream cmd

	str_cmd.ic_cmd = cmd;

	str_cmd.ic_timout = 0;

	str_cmd.ic_dp = ndd_buf;

	str_cmd.ic_len = sizeof(ndd_buf);

	switch(cmd)
	{
	case ND_GET:

		strcpy(ndd_buf,vp->var_name);

		break;

	case ND_SET:

		switch (vp->var_type)
		{
		case VAR_INT_T:

			sprintf(ndd_buf, "%s%c%d", vp->var_name, '\0' , vp->var_un.var_int);
			break;

		case VAR_STRING_T:

			sprintf(ndd_buf, "%s%c%s", vp->var_name, '\0' , vp->var_un.var_string);
			break;

		default:
			break;
		}

	default:

		return -1;

		break;

	}

	if( ioctl(np->ndd_sd, I_STR, &str_cmd) == -1)
	{
		return -1;
	}


	if(cmd == ND_GET)
	{
		switch(vp->var_type)
		{
		case VAR_INT_T:

			vp->var_un.var_int = atoi(ndd_buf);

			break;

		case VAR_STRING_T:

			for(i=0, p=ndd_buf; i<str_cmd.ic_len-1; i++, p++)
			{
				if('\0' == *p)
				{
					*p='\n';
				}
			}

			if(vp->var_un.var_string)
			{
				free(vp->var_un.var_string);
			}

			vp->var_un.var_string = strdup(ndd_buf);
			break;
		default:
			return -1;

		}	
	}
	return 0;
}


#endif //_SOLARIS 

// devuelve el tiempo de espera en segundos
int tcp_close_wait_interval(void)
{

#ifdef _SOLARIS 

	int retCode = 0;

	const int default_interval = 240;

	ndd_dev_t dev_name = { "/dev/tcp", 0};

    // bugid 0098046. tcp_close_wait_interval is obsoleted.(solaris > 2.6)
	var_t value = {"tcp_time_wait_interval", VAR_INT_T, 0 };

	retCode = ndd_name_io ( &dev_name, ND_GET, &value);

	if(-1 == retCode)
	{

		M4_SRV_WARNING_N( 0, "Error en ndd_name_io()" ) ;

		return default_interval;
	}


	M4_SRV_DEBUG ( "tcp_close_wait_interval : %0:s",  value.var_un.var_int ) ;

	return ( value.var_un.var_int/1000);

#else //_SOLARIS 

	return 60 ;
	
#endif //_SOLARIS 
}

		
#endif //_UNIX



#ifdef _WINDOWS

// Only for debug.
void SvcError(const char* ai_message, long ai_code, ostream& ai_dev=cout)
{ 
    char buffer[2048];
    ofstream ofs;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
    M4Open( ofs, "c:\\temp\\m4dispatchersvc.log", M4UniAppend, eEncoding );

    sprintf(buffer, ai_message, ai_code); 
    if (ofs.is_open())
    { 
        ofs << buffer << endl;
        ai_dev << buffer << endl;

        if (ai_code != 0)
        {
		    LPVOID lpMsgBuf;

		    DWORD errorCode = FormatMessage( 
			    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			    FORMAT_MESSAGE_FROM_SYSTEM | 
			    FORMAT_MESSAGE_IGNORE_INSERTS,
			    NULL,
			    ai_code,
                0, //MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			    (LPTSTR) &lpMsgBuf,
			    0,
			    NULL 
		    );

            ai_dev << "FormatMessage error: " << errorCode << endl;
            ofs << "FormatMessage error: " << errorCode << endl;
		    if (lpMsgBuf!= NULL)
            {
                ai_dev << TEXT((LPCTSTR)lpMsgBuf) << endl;
                ofs << TEXT((LPCTSTR)lpMsgBuf) << endl;

		        // Free the buffer.
		        LocalFree( lpMsgBuf );
            }
        }
        ofs.close();
    }

}

int getInstallDir( char* ai_pcServiceName, char *ao_installdir )
{
    int retCode = -1;

    SC_HANDLE schService;
    SC_HANDLE schSCManager;

    schSCManager = OpenSCManager(
                        NULL,                   // host
                        NULL,                   // database
                        SC_MANAGER_ALL_ACCESS   // access permisions
                        );

    // Query the service to get the service binary fullname 
    LPQUERY_SERVICE_CONFIG lpqscBuf; 
    DWORD dwBytesNeeded; 
 
    // Open a handle to the service.  
    schService = OpenService( 
        schSCManager,           // SCManager database 
        ai_pcServiceName,       // name of service 
        SERVICE_QUERY_CONFIG);  // need QUERY access 

    if (schService == NULL) 
    {
        return retCode;
    }
 
    // Allocate a buffer for the configuration information. 
    lpqscBuf = (LPQUERY_SERVICE_CONFIG) LocalAlloc(LPTR, 4096); 
    if (lpqscBuf == NULL) 
    {
        return retCode;
    }
  
    // Get the configuration information.  
    if (! QueryServiceConfig( 
        schService, 
        lpqscBuf, 
        4096, 
        &dwBytesNeeded) ) 
    {
        return retCode;
    }
  
    if ( schService )
        CloseServiceHandle( schService );

    if ( schSCManager )
        CloseServiceHandle( schSCManager );
 
    // get binary filepath.
    strcpy(ao_installdir, lpqscBuf->lpBinaryPathName);
    char *p = strrchr(ao_installdir, '\\');
    if ((p != NULL) && (p != ao_installdir))
    {
        // if character was found and there is not the first character.
        *p = '\0';
        retCode = 0;
    }
    LocalFree(lpqscBuf); 
   
    return retCode;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv) {
//void WINAPI ServiceMain() {
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////


    // Registramos el handle del manejador del cotrol del servicio:
	//
    sshStatusHandle = RegisterServiceCtrlHandler( *lpszArgv, service_ctrl);


    if (sshStatusHandle)
	{

		// miembros del SERVICE_STATUS que no cambian
		//
		ssStatus.dwServiceType = SERVICE_INTERACTIVE_PROCESS | SERVICE_WIN32_OWN_PROCESS;
		ssStatus.dwServiceSpecificExitCode = 0;


		// Informamos al SCM del nuevo estado del servicio
		//
		if (ReportStatusToSCMgr(
			SERVICE_START_PENDING, // Estado del servicio
			NO_ERROR,              // codigo de salida
			30000))                 // Tiempo de espera en milisegundos
		{					   // cuando se pasa un estado PENDING
			//*************************************************
			//*************************************************
			//CAMBIO
			//Rutina personal del servicio
			//aqui tenemos que incluir lo que queremos que realice
			//nuestro servcio
 
            // nos cambiamos al directorio donde está instalado.
            // El directorio (en NT) por defecto es: c:\winnt\system32 lo que hace que no
            // se lean los ficheros de configuración. bugid:0088228
            char installdir[2048];
            if (!getInstallDir( *lpszArgv, installdir ))
            {
                // change current directory to install directory.
                _chdir(installdir);
            }

			Daemon_main ( 0 );
			//*************************************************
			//*************************************************
		}	

	}
    // Indicamos al SCM que el servicio no ha podido ser arrancado
    //
    if (sshStatusHandle)
        (VOID)ReportStatusToSCMgr(
                            SERVICE_STOPPED,
                            0,
                            0);

    return;
}


//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
void WINAPI service_ctrl(DWORD dwCtrlCode) {
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
    // Maneja el codigo de control:
    //
    switch(dwCtrlCode)
    {
        // Parar el servicio.
        //
        case SERVICE_CONTROL_STOP:
            ssStatus.dwCurrentState = SERVICE_STOP_PENDING;
			//**********************************************
			//**********************************************
			//CAMBIO
			//Rutina adicional que se debe ejecutar cuando
			//paramos el servicio.
			//Ejemplo seria salirnos de lo que estuviera haciendo
			//el servicio.

            Stop_Daemon();
			//**********************************************
			//**********************************************
            break;
        default:
            break;

    }

	//Comunicamos al SCM el nuevo estado del servicio
    ReportStatusToSCMgr(ssStatus.dwCurrentState, NO_ERROR, 60000);
}


//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
int InstallService( m4pcchar_t ai_pccServiceName, m4pcchar_t ai_pccDisplayName ) {
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
    SC_HANDLE   schService;
    SC_HANDLE   schSCManager;

	TCHAR szPath[512];

	int iRet = 0;

	SvcError ("\nInstalling the Application Controller as a Windows Service...", 0);

	//************************************************************
	//************************************************************
	//CAMBIO
	//Solo es necesario cambiar esto si el ejecutable del servicio
	//es diferente del que hace la instalación

	//guardaremos la ruta completa y nombre del ejecutable del servicio
    if ( GetModuleFileName( NULL, szPath, 512 ) == 0 )
    {
		M4_SRV_GENERAL( "Imposible instalar %0:s\n" , ai_pccDisplayName );
		SvcError ("\nERROR when installing Application Controller as a Windows Service:\n", 0, cerr);
        SvcError("Error in GetModuleFileName. ErrorCode: %ld", GetLastError(), cerr);		
		SvcError ("Application Controller could not be installed as a Windows Service.", 0, cerr);   
        return 1;
    }

	SvcError ("\nThe service file name is:", 0);
    SvcError (szPath, 0);

	//************************************************************
	//************************************************************

    schSCManager = OpenSCManager(
                        NULL,                   // Maquina (NULL == local)
                        NULL,                   // Base de datos (NULL == defecto)
                        SC_MANAGER_ALL_ACCESS   // Accesos que se van a realizar
                        );
    if ( schSCManager )
    {
        schService = CreateService(
            schSCManager,               // Handle del SCM
            ai_pccServiceName,			// Nombre interno del servicio
            ai_pccDisplayName,			// Nombre del servicio
            SERVICE_ALL_ACCESS,         // Accesos del servicio
			SERVICE_INTERACTIVE_PROCESS | SERVICE_WIN32_OWN_PROCESS,  // Tipo de servicio
			SERVICE_AUTO_START, 
			//SERVICE_DEMAND_START,       // Tipo de comienzo		 
            SERVICE_ERROR_NORMAL,       // Tipo de control de errores
			szPath,                     // Ruta completa del servicio
            NULL,                        // Grupo del servicio
            NULL,                       // Identificador del grupo
            NULL,				       // Dependencias
            NULL,                       // Cuenta: NULL = LocalSystem
            NULL);                      // Password: NULL = Ninguna

        if ( schService )
        {
			SvcError ("\nApplication Controller has been installed as a Windows Service with name: ", 0);
			SvcError (ai_pccDisplayName, 0);
			M4_SRV_GENERAL( "%0:s instalado.\n" , ai_pccDisplayName );
            CloseServiceHandle(schService);
			iRet = 0;
        }
        else
        {
			SvcError ("\nERROR when installing Application Controller as a Windows Service:\n", 0, cerr);
		    SvcError("Error in CreateService. ErrorCode: %ld", GetLastError(), cerr);
			SvcError ("Application Controller could not be installed as a Windows Service.", 0, cerr);
			M4_SRV_GENERAL_N("Fallo CreateService\n" );
			iRet = 1;
        }

        CloseServiceHandle(schSCManager);
    }
    else{
		SvcError ("\nERROR when installing Application Controller as a Windows Service:\n", 0, cerr);
		SvcError("Error in OpenSCManager. ErrorCode: %ld", GetLastError(), cerr);
		SvcError ("Application Controller could not be installed as a Windows Service.", 0, cerr);
		M4_SRV_GENERAL_N("Fallo OpenSCManager\n");
		iRet = 1;
	}
	return iRet;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
int RemoveService( m4pcchar_t ai_pccServiceName, m4pcchar_t ai_pccDisplayName ) {
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
    SC_HANDLE   schService;
    SC_HANDLE   schSCManager;


	int iRet = 0;

	SvcError ("\nRemoving the Application Controller from the Windows Services list...", 0);

    schSCManager = OpenSCManager(
                        NULL,                   // Maquina (NULL == local)
                        NULL,                   // Base de datos (NULL == defecto)
                        SC_MANAGER_ALL_ACCESS   // Accesos que se van a realizar
                        );
    if ( schSCManager )
    {
        schService = OpenService(schSCManager, ai_pccServiceName, SERVICE_ALL_ACCESS);

        if (schService)
        {
	        // Intentamos parar el servicio
            if ( ControlService( schService, SERVICE_CONTROL_STOP, &ssStatus ) )
            {
                //_tprintf(TEXT("Parando %s."), TEXT(SZSERVICEDISPLAYNAME));
				M4_SRV_GENERAL("Parando %0:s.", ai_pccDisplayName);
				SvcError("\nStopping service.", 0);

                Sleep( 1000 );

                while( QueryServiceStatus( schService, &ssStatus ) )
                {
                    if ( ssStatus.dwCurrentState == SERVICE_STOP_PENDING )
                    {
						M4_SRV_GENERAL_N(".");
                        Sleep( 1000 );
                    }
                    else
                        break;
                }

                if ( ssStatus.dwCurrentState == SERVICE_STOPPED )
				{	
					SvcError("Service stopped", 0);
					M4_SRV_GENERAL("%0:s detendido.\n", ai_pccDisplayName );
				}
                else
				{
					SvcError("ERROR Stopping service.", 0, cerr);
					M4_SRV_GENERAL("%0:s fallo la detencion.\n", ai_pccDisplayName);
				}

            }

            // Eliminamos el servicio
            if( DeleteService(schService) )
			{
				SvcError ("\nApplication Controller has been removed from the Windows Services list.", 0);
				M4_SRV_GENERAL("%0:s eliminado.\n", ai_pccDisplayName );
				iRet = 0;
			}
            else
			{
				SvcError ("\nERROR when removing Application Controller Windows Service:\n", 0, cerr);
				SvcError("Error in DeleteService. ErrorCode: %ld", GetLastError(), cerr);
				SvcError ("Application Controller could not be removed.", 0, cerr);
				M4_SRV_GENERAL_N("Fallo el borrado del servicio\n");
				iRet = 1;
			}

            CloseServiceHandle(schService);
        }
        else{
			SvcError ("\nERROR when removing Application Controller Windows Service:\n", 0, cerr);
			SvcError("Error in OpenService. ErrorCode: %ld", GetLastError(), cerr);
			SvcError ("Application Controller could not be removed.", 0, cerr);
			M4_SRV_GENERAL_N("Fallo OpenService\n");
			iRet = 1;
		}

        CloseServiceHandle(schSCManager);
    }
    else{
		SvcError ("\nERROR when removing Application Controller Windows Service:\n", 0, cerr);
		SvcError("Error in OpenSCManager. ErrorCode: %ld", GetLastError(), cerr);
		SvcError ("Application Controller could not be removed.", 0, cerr);
		M4_SRV_GENERAL_N("Fallo OpenSCManager\n");
		iRet = 1;
	}

	return iRet;
}


#endif

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
BOOL ReportStatusToSCMgr(DWORD dwCurrentState,
                         DWORD dwWin32ExitCode,
                         DWORD dwWaitHint) {
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
    BOOL fResult = M4_TRUE ;
#ifdef _WINDOWS

    static DWORD dwCheckPoint = 1;

    if (dwCurrentState == SERVICE_START_PENDING)
        ssStatus.dwControlsAccepted = 0;
    else
        ssStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    ssStatus.dwCurrentState = dwCurrentState;
    ssStatus.dwWin32ExitCode = dwWin32ExitCode;
    ssStatus.dwWaitHint = dwWaitHint;

    if ( ( dwCurrentState == SERVICE_RUNNING ) ||
         ( dwCurrentState == SERVICE_STOPPED ) )
        ssStatus.dwCheckPoint = 0;
    else
        ssStatus.dwCheckPoint = dwCheckPoint++;


    // Enviamos el estado del servicio al SCM
    // Bug 142773
	if (sshStatusHandle)
	{
		fResult = SetServiceStatus( sshStatusHandle, &ssStatus);
	}

#endif
    return fResult;
}


