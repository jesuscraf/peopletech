#ifndef _SERVICE_H_
#define _SERVICE_H_

//////////////////////////////////////////////////////////////////////////////
//// Strings generales
////

//CAMBIO
//Cambiar dependiendo del servicio

// Nombre del ejecutable del servicio
#define SZAPPNAME            "GeneralService"
// Nombre interno del servicio
#define SZSERVICENAME        "M4Server"
// Nombre que se mostrará del servicio
#define SZSERVICEDISPLAYNAME "Meta4 Mind Server"
// Lista de dependencias de la forma: - "dep1\0dep2\0\0"
#define SZDEPENDENCIES       ""
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//// FUNCIONES GENERALES
////

//Instala el servico en el SCM (service control manager) y lo añade
//al SCMB (service control manager database)
void InstallService();

//Para el servicio si está en ejecución y lo desinstala del SCM
void RemoveService();

// Rutina que se encarga de establecer la rutina de control, y
// llamar a la función que ejecuta lo que queremos que haga
// nuestro servicio.
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);

//Rutina que va a manejar los codigos de control que envie
//el SCM al servicio
VOID WINAPI service_ctrl(DWORD dwCtrlCode);

// Comunica al SCM el nuevo estado del servicio y cambia los checkpoint del servicio
BOOL ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);

#endif
