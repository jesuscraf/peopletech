#include <m4win.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <m4types.hpp>
#include <serverservice.hpp>

#include "m4memdbg.h"
#define M4_SRV_TRACE_COMP_ID "SRV"


#ifdef _WINDOWS
#include <process.h>
#include <tchar.h>
#endif

// Variables internas
#ifdef _WINDOWS
m4return_t StopServer() ;
m4bool_t service_started = M4_TRUE ;

SERVICE_STATUS ssStatus; // Estado actual del servicio
SERVICE_STATUS_HANDLE   sshStatusHandle; //handle del manejador de los codigos de control
#endif
//*******************************************************
//*******************************************************
//BORRAR los dos prototipos
//Prueba de funcionamiento
int server_main (int argc, char** argv) ;
//*******************************************************
//*******************************************************

#ifdef _SOLARIS
//Bugs 0077794 y 0110275
//Si no es posible hacer new el compilador nuevo lanza una excepción 
//pero no devolvía null.
//Con esta modificacion no se lanza excepción y devolvera el null del malloc.
//De esta forma simulamos el comportamiento del antiguo new.
void*	operator new(size_t ai_iSize) throw(std::bad_alloc)
{
	//return( new (std::nothrow) char[ ai_iSize ] );
	return( malloc( ai_iSize ) );
}
#endif

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
	M4SrvConfigDevelopEnvByEnvVble( M4_SRV_TRACE_COMP_ID) ;

#ifdef _WINDOWS
		SERVICE_TABLE_ENTRY dispatchTable[] =
		{
			{ TEXT(SZSERVICENAME), (LPSERVICE_MAIN_FUNCTION)ServiceMain },
			{ NULL, NULL }
		};
		switch ( argc )
		{
		case 1:
			StartServiceCtrlDispatcher(dispatchTable);
			break;
		case 2:
			if((*argv[1] == '-') || (*argv[1] == '/')) 
			{
				if ( _stricmp( "instala", argv[1]+1 ) == 0 )
				{
					InstallService();
				}
				else if ( _stricmp( "borra", argv[1]+1 ) == 0 )
				{
					RemoveService();
				}
			}
		break ;
		default:
			service_started = M4_FALSE ;
#endif
			server_main (argc, argv);
#ifdef _WINDOWS

			break ;
		}
#endif

	// si no pasamos parámetros, el SCM comienza la ejecución
	// del servcicio mediante la llamada a StartServiceCtrlDispatcher
	return 0;
}

#ifdef _WINDOWS

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv) {
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

    // Registramos el handle del manejador del cotrol del servicio:
	//
    sshStatusHandle = RegisterServiceCtrlHandler( TEXT(SZSERVICENAME), service_ctrl);


    if (sshStatusHandle)
	{

		// miembros del SERVICE_STATUS que no cambian
		//
		ssStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
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
			server_main (dwArgc, lpszArgv);
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
VOID WINAPI service_ctrl(DWORD dwCtrlCode) {
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
			//Rutina adiccional que se debe ejecutar cuando
			//paramos el servicio.
			//Ejemplo seria salirnos de lo que estuviera haciendo
			//el servicio.

            StopServer();
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
BOOL ReportStatusToSCMgr(DWORD dwCurrentState,
                         DWORD dwWin32ExitCode,
                         DWORD dwWaitHint) {
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
    static DWORD dwCheckPoint = 1;
    BOOL fResult = TRUE;

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
    //
    fResult = SetServiceStatus( sshStatusHandle, &ssStatus);

    return fResult;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
void InstallService() {
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
    SC_HANDLE   schService;
    SC_HANDLE   schSCManager;

	TCHAR szPath[512];

	//************************************************************
	//************************************************************
	//CAMBIO
	//Solo es necesario cambiar esto si el ejecutable del servicio
	//es diferente del que hace la instalación

	//guardaremos la ruta completa y nombre del ejecutable del servicio
    if ( GetModuleFileName( NULL, szPath, 512 ) == 0 )
    {
        _tprintf(TEXT("Imposible instalar %s\n"), TEXT(SZSERVICEDISPLAYNAME));
        return;
    }
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
            TEXT(SZSERVICENAME),        // Nombre interno del servicio
            TEXT(SZSERVICEDISPLAYNAME), // Nombre del servicio
            SERVICE_ALL_ACCESS,         // Accesos del servicio
            SERVICE_WIN32_OWN_PROCESS,  // Tipo de servicio
            SERVICE_DEMAND_START,       // Tipo de comienzo
            SERVICE_ERROR_NORMAL,       // Tipo de control de errores
            szPath,                     // Ruta completa del servicio
            NULL,                       // Grupo del servicio
            NULL,                       // Identificador del grupo
            TEXT(SZDEPENDENCIES),       // Dependencias
            NULL,                       // Cuenta: NULL = LocalSystem
            NULL);                      // Password: NULL = Ninguna

        if ( schService )
        {
            _tprintf(TEXT("%s instalado.\n"), TEXT(SZSERVICEDISPLAYNAME) );
            CloseServiceHandle(schService);
        }
        else
        {
            _tprintf(TEXT("Fallo CreateService\n"));
        }

        CloseServiceHandle(schSCManager);
    }
    else
        _tprintf(TEXT("Fallo OpenSCManager\n"));
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
void RemoveService() {
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
    SC_HANDLE   schService;
    SC_HANDLE   schSCManager;

    schSCManager = OpenSCManager(
                        NULL,                   // Maquina (NULL == local)
                        NULL,                   // Base de datos (NULL == defecto)
                        SC_MANAGER_ALL_ACCESS   // Accesos que se van a realizar
                        );
    if ( schSCManager )
    {
        schService = OpenService(schSCManager, TEXT(SZSERVICENAME), SERVICE_ALL_ACCESS);

        if (schService)
        {
            // Intentamos parar el servicio
            if ( ControlService( schService, SERVICE_CONTROL_STOP, &ssStatus ) )
            {
                _tprintf(TEXT("Parando %s."), TEXT(SZSERVICEDISPLAYNAME));
                Sleep( 1000 );

                while( QueryServiceStatus( schService, &ssStatus ) )
                {
                    if ( ssStatus.dwCurrentState == SERVICE_STOP_PENDING )
                    {
                        _tprintf(TEXT("."));
                        Sleep( 1000 );
                    }
                    else
                        break;
                }

                if ( ssStatus.dwCurrentState == SERVICE_STOPPED )
                    _tprintf(TEXT("\n%s detendido.\n"), TEXT(SZSERVICEDISPLAYNAME) );
                else
                    _tprintf(TEXT("\n%s fallo la detencion.\n"), TEXT(SZSERVICEDISPLAYNAME) );

            }

            // Eliminamos el servicio
            if( DeleteService(schService) )
                _tprintf(TEXT("%s eliminado.\n"), TEXT(SZSERVICEDISPLAYNAME) );
            else
                _tprintf(TEXT("Fallo el borrado del servicio\n"));


            CloseServiceHandle(schService);
        }
        else
            _tprintf(TEXT("Fallo OpenService\n"));

        CloseServiceHandle(schSCManager);
    }
    else
        _tprintf(TEXT("Fallo OpenSCManager\n"));
}
#endif

//*******************************************************
//*******************************************************
//BORRAR TODO DESDE AQUI HASTA EL FINAL DEL FICHERO
//Prueba de funcionamiento
//*******************************************************
//*******************************************************
