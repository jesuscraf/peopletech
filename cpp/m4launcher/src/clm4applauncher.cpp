//
// clm4applauncher.cpp
//


#ifdef _WINDOWS	
#include <direct.h>
#include <process.h>
#include <io.h>
#else
#include <unistd.h>
#endif


#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include "m4stl.hpp"
#include "clm4applauncher.hpp"
#include "cmdlnlauncher.hpp"
#include "clappinfo.hpp"
#include "m4trace.hpp"
#include "m4ini.hpp"
#include <m4cryptc.hpp>
#include "m4srvtrace.h"
#include "basiclog.hpp"

#define M4_SRV_TRACE_COMP_ID "LNCHR"



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





ClM4appLauncher::ClM4appLauncher(cCmdlnLauncher* ai_InputOptions, const char* ai_processName) :
	m_name(NULL), m_workingDirectory(NULL), m_inputArguments(NULL)
{

	M4_SRV_DEBUG( "Process Name %0:s", ai_processName );

	m_InputOptions = ai_InputOptions ;

	// initialize the appInfo unique instance.
	ClAppInfo* appInfoInstance = ClAppInfo::Instance(m_InputOptions);

	iError = appInfoInstance -> Init() ;

//	m_pcPathEnv= new char[65536];
	
}


ClM4appLauncher::~ClM4appLauncher() 
{
	
}


int ClM4appLauncher::Run()
{ 
	int retCode = 0;
	
	if (retCode = SetName())
		return retCode;
	if (retCode = SetArgv())
		return retCode;
	if (retCode = SetEnvironment())
		return retCode;

	// set the working directory.
	// this parameter should be the last read in the INI file,
	// due this file change the work directory, and if the INI file has
	// a relative path, may be not found in the next operation.
	if (retCode = SetWorkingDirectory())
		return retCode;


	m4string_t CallStr ;
	m4int16_t B ;

	CallStr = "" ;
	for (B=0; m_argv[B] ; ++B) 
	{
		CallStr += m_argv[B];
		CallStr += " ";
	}
	M4_SRV_GENERAL( "Calling to %0:s , args %1:s", m_name << CallStr.c_str()  ) ;


	// Cambiamos la forma de lanzar el server, ya que ahora ejecutamos el script.
#ifdef _WINDOWS

	// bg 0117962
    STARTUPINFO si ;
    PROCESS_INFORMATION pi ;

    ZeroMemory( &si, sizeof( si ) ) ;
    si.cb = sizeof( si ) ;
    ZeroMemory( &pi, sizeof( pi ) ) ;

    // Start the child process. 
    if( !CreateProcess( NULL,	// No module name (use command line). 
        (char*)CallStr.c_str(), // Command line. 
        NULL,					// Process handle not inheritable. 
        NULL,					// Thread handle not inheritable. 
        FALSE,					// Set handle inheritance to FALSE. 
        0,						// No creation flags. 
        NULL,					// Use parent's environment block. 
        NULL,					// Use parent's starting directory. 
        &si,					// Pointer to STARTUPINFO structure.
        &pi )					// Pointer to PROCESS_INFORMATION structure.
    ) 
    {        
		M4_SRV_ERROR(0,"Error %0:s launching server process", GetLastError() ) ;
    }

    // Close process and thread handles. 
    CloseHandle( pi.hProcess ) ;
    CloseHandle( pi.hThread ) ;

#else 
	CallStr = CallStr + " &";
	system(CallStr.c_str());
#endif

	return M4_SUCCESS;
}


int ClM4appLauncher::SetName()
{

	// get the appinfo instance.
	ClAppInfo* appInfoInstance = ClAppInfo::Instance();
	if (NULL == appInfoInstance)
	{
		M4_SRV_ERROR_N( 0, "Can't get ClAppInfo::Instance()" );
		return 1;
	}
	

	// get info from INI file.
	char bufferCommand[ARGV_VALUE_MAX_SIZE];
	if (appInfoInstance->GetValue("bin", bufferCommand , ARGV_VALUE_MAX_SIZE))
	{
		M4_SRV_ERROR( 0, "Variable %0:s not found!", "bin" ) ;
		return M4_ERROR ;
	}
	
	m_name = new char[strlen(bufferCommand) + 1];

	if (NULL == m_name)
	{
		M4_SRV_ERROR_N( 0, "Memory allocation error" ) ;
		return M4_ERROR;
	}

//	strcpy ( m_name , bufferCommand) ;
	sprintf ( m_name , "%s" , bufferCommand) ;

	M4_SRV_DEBUG( "m_name = %0:s",  m_name );

	return 0;
}


int ClM4appLauncher::SetWorkingDirectory()
{
	// get the appinfo instance.
	ClAppInfo* appInfoInstance = ClAppInfo::Instance();
	if (NULL == appInfoInstance)
	{
		M4_SRV_ERROR_N( 0, "Can't get ClAppInfo::Instance()" );
		return 1;
	}
	
	// get info from INI file.
	char buffer[ARGV_VALUE_MAX_SIZE];


	if (appInfoInstance->GetValue("configuration_root", buffer,ARGV_VALUE_MAX_SIZE))
	{
		return 1;
	}

	m_workingDirectory = new char[strlen(buffer)+1];
	if (NULL == m_workingDirectory)
	{
		M4_SRV_ERROR_N( 0, "Memory allocation error");
		return 1;
	}

	strcpy(m_workingDirectory, buffer);

	M4_SRV_DEBUG( "m_workingDirectory = %0:s", m_workingDirectory);

	
#ifdef _UNIX
	if (chdir(m_workingDirectory))
#else
	if (_chdir(m_workingDirectory))
#endif
	{
		M4_SRV_ERROR( 0, "Can'find working directory %0:s ", m_workingDirectory ) ;
		return -1;
	}

	return 0;
}


int ClM4appLauncher::SetArgv()
{
	// get the appinfo instance.
	char buffer[APP_INFO_MAX_BUFFER];
	char szPassword[APP_INFO_MAX_BUFFER];
	char szConfigName [APP_INFO_MAX_BUFFER];
	m4char_t szNewPawd [ APP_INFO_MAX_BUFFER ];
	ClAppInfo* appInfoInstance = ClAppInfo::Instance();
	m4int_t iLenOut ;
	m4return_t iRet ;


	
	if (NULL == appInfoInstance)
	{
		M4_SRV_ERROR_N( 0, "Can't get ClAppInfo::Instance()");
		return 1;
	}

	
	if (M4_ERROR == appInfoInstance->GetValue( passwordstr, szPassword ,APP_INFO_MAX_BUFFER))
	{
		return M4_ERROR ;
	}

	if (M4_ERROR == appInfoInstance->GetValue( namestr , szConfigName  ,APP_INFO_MAX_BUFFER))
	{
		return M4_ERROR ;
	}

	ToLower ( szConfigName ) ;

	iRet = SynDeCrypt ( szPassword , strlen ( szPassword ) , szConfigName , strlen ( szConfigName ) , szNewPawd , APP_INFO_MAX_BUFFER, iLenOut ) ;
	
	if (appInfoInstance->SetValue(passwordstr, szNewPawd ))
	{
		return M4_ERROR ;
	}

	// get info from INI file.
	if (!appInfoInstance->GetValue("argv", buffer,APP_INFO_MAX_BUFFER))
	{
		if ( strlen(buffer)+1 > MAX_PARAM_LEN )
		{
			return M4_ERROR ;
		}

		m_inputArguments = new char[MAX_PARAM_LEN ];
		if (NULL == m_inputArguments)
		{
			M4_SRV_ERROR_N( 0, "Memory allocation error" );
			return 1;
		}

		strcpy(m_inputArguments, buffer);
		M4_SRV_DEBUG( "m_inputArguments = %0:s" , m_inputArguments );
	}



	BuildArgv();
	return 0;
}


int ClM4appLauncher::SetEnvironment()
{
	m4return_t iRet;
	m4int_t sectionsize , iter;

	// get the appinfo instance.
	ClAppInfo* appInfoInstance = ClAppInfo::Instance();
	if (NULL == appInfoInstance)
	{
		M4_SRV_ERROR_N( 0, "Can't get ClAppInfo::Instance()" );
		return 1;
	}
	
	// get info from INI file.
	char * Section [APP_INFO_MAX_BUFFER] ;

	if (M4_ERROR == appInfoInstance->GetSection("environment", Section  , sectionsize))	//Mayusculas, UNIX
	{
		M4_SRV_ERROR( 0, "Variable %0:s not found!", "PATH" )  ;
		return M4_ERROR ;
	}
	for ( iter = 0 ;iter < sectionsize ;iter++)
	{

/*
	if (M4_ERROR == appInfoInstance->GetValue("PATH", Path  , 4096))	//Mayusculas, UNIX
	{
		M4_SRV_ERROR( 0, "Variable %0:s not found!", "PATH" )  ;
		return M4_ERROR ;
	}

	pOldPath = getenv("PATH");	//Importante en mayusculas para UNIX
	
	strcpy(m_cEnvPath,"PATH=");

	strcat(m_cEnvPath , Path);

	strcat(m_cEnvPath, ";" ) ;

	strcat(m_cEnvPath, pOldPath ? pOldPath : "" ) ;
*/
		M4_SRV_DEBUG( "putenv(%0:s)", Section [ iter ]  ) ;
		iRet = putenv( Section [ iter ] );
	}

	return M4_SUCCESS ;
}



int ClM4appLauncher::BuildArgv()
{
	int argc = 0;
	m_argv[argc] = new char [strlen(m_name)+1];
	sprintf (m_argv[argc++], "%s" , m_name);
	char bufferaux[MAX_PARAM_LEN ];
	const char* ini_default_value = "ini_default_value";
	ClAppInfo * poAppInfo = ClAppInfo::Instance () ;

	
	if (NULL != m_inputArguments)
	{
		if ( M4_ERROR == poAppInfo -> Parse ( m_inputArguments , MAX_PARAM_LEN ) )
		{
			return M4_ERROR ;
		}

		char seps[] = "\"";
		char *token;

		strcpy (bufferaux , m_inputArguments ) ;

		token = strtok(bufferaux, seps);

		while(token != NULL)
		{
			if ( token[0] != ' ')
			{
				M4_SRV_DEBUG( "argv token = %0:s ", token );
				m_argv[argc] = new char [strlen(token)+3];
//				strcpy(m_argv[argc++], token);
#ifdef _WINDOWS
				sprintf(m_argv[argc++], "\"%s\"" , token);
#else
				sprintf(m_argv[argc++], "%s" , token);
#endif
			}
				/* Get next token:  */
			token = strtok(NULL, seps);

		}
	}

	// set the last argv to NULL. This is a requirement for execvp function.
	m_argv[argc] = NULL;


	return 0;
}

