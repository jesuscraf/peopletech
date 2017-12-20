#ifndef _DSPSERVICE_H_
#define _DSPSERVICE_H_


#ifdef _UNIX

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stropts.h>

	#ifdef _SOLARIS 
#include <inet/nd.h>
	#endif

#endif

//////////////////////////////////////////////////////////////////////////////
//// Strings generales
////


#ifdef _WINDOWS
#define DPS_CONFIG_RELATIVE_FILE_PATH "conf/m4dspini.xml"
#endif


//CAMBIO
//Cambiar dependiendo del servicio

// Nombre del ejecutable del servicio
#define SZAPPNAME            "GeneralService" //LAJ cambiar
// Nombre interno del servicio
#define SZSERVICENAME        "M4SiteDispatcher" //LAJ cambiar
// Nombre que se mostrará del servicio
#define SZSERVICEDISPLAYNAME "Meta4 Application Server Site Dispatcher"
// Lista de dependencias de la forma: - "dep1\0dep2\0\0"
#define SZDEPENDENCIES       ""
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//// FUNCIONES GENERALES
////

//Instala el servico en el SCM (service control manager) y lo añade
//al SCMB (service control manager database)
int InstallService( m4pcchar_t ai_pccServiceName, m4pcchar_t ai_pccDisplayName );

//Para el servicio si está en ejecución y lo desinstala del SCM
int RemoveService( m4pcchar_t ai_pccServiceName, m4pcchar_t ai_pccDisplayName );

// Rutina que se encarga de establecer la rutina de control, y
// llamar a la función que ejecuta lo que queremos que haga
// nuestro servicio.
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
//void WINAPI ServiceMain();

//Rutina que va a manejar los codigos de control que envie
//el SCM al servicio
VOID WINAPI service_ctrl(DWORD dwCtrlCode);

// Comunica al SCM el nuevo estado del servicio y cambia los checkpoint del servicio
BOOL ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);

#ifdef _UNIX

typedef enum{

	VAR_INT_T,

	VAR_STRING_T

} var_type_t;

typedef struct{

	char * var_name;

	var_type_t var_type;

	union{

		int var_int;

		char * var_string;

	}var_un;

} var_t;

typedef struct{

	char * ndd_dev_name;

	int ndd_sd;

} ndd_dev_t;


static char ndd_buf[65536];



int tcp_close_wait_interval(void);


static int ndd_name_io( ndd_dev_t * np, int cmd, var_t * vp);

#endif


#endif
