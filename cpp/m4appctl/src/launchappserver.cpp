//## begin module%37F095DF01C1.cm preserve=no
//## end module%37F095DF01C1.cm

//## begin module%37F095DF01C1.cp preserve=no
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
//## end module%37F095DF01C1.cp

//## Module: LaunchAppServer%37F095DF01C1; Package body
//## Subsystem: M4Appctl::src%37EA676703E6
//## Source file: F:\integration\M4Appctl\src\launchappserver.cpp

//## begin module%37F095DF01C1.additionalIncludes preserve=no
//## end module%37F095DF01C1.additionalIncludes

//## begin module%37F095DF01C1.includes preserve=yes
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <m4memdbg.h>
#include <m4srvtrace.h>
#include <appctlreq.hpp>
#include <basiclog.hpp>
#include <clarraylibres.hpp>


#define M4_SRV_TRACE_COMP_ID "APPCTL"

#ifdef _WINDOWS
#include <process.h>
#endif
#ifdef _UNIX
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <m4ini.hpp>
#endif
#include "m4date.hpp"
#include <m4log.hpp>
#include <tsap.hpp>
#include <m4appctlres.hpp>
//## end module%37F095DF01C1.includes

// m4condition
#include <m4condition.hpp>
// reqrepository
#include <reqrepository.hpp>
// appctlsaxhandler
#include <appctlsaxhandler.hpp>
// eStatus
#include <estatus.hpp>
// LaunchAppServer
#include <launchappserver.hpp>
// CommonVariables
#include <commonvariables.hpp>
// m4communication
#include <m4communication.hpp>
// iexecutable
#include <iexecutable.hpp>
// FastQ
#include <fastfifoq.hpp>
//## begin module%37F095DF01C1.declarations preserve=no
//## end module%37F095DF01C1.declarations

//## begin module%37F095DF01C1.additionalDeclarations preserve=yes
//## end module%37F095DF01C1.additionalDeclarations


// Class ClLaunchAppServer 








ClLaunchAppServer::ClLaunchAppServer ()
  //## begin ClLaunchAppServer::ClLaunchAppServer%938506847.hasinit preserve=no
      : m_ulMaxNumberOfAttempts(5), m_psAppCtlConfigPath(0)
  //## end ClLaunchAppServer::ClLaunchAppServer%938506847.hasinit
  //## begin ClLaunchAppServer::ClLaunchAppServer%938506847.initialization preserve=yes
  //## end ClLaunchAppServer::ClLaunchAppServer%938506847.initialization
{
  //## begin ClLaunchAppServer::ClLaunchAppServer%938506847.body preserve=yes
	m_oStopper.Init () ;
  //## end ClLaunchAppServer::ClLaunchAppServer%938506847.body
}


ClLaunchAppServer::~ClLaunchAppServer ()
{
  //## begin ClLaunchAppServer::~ClLaunchAppServer%939286310.body preserve=yes

//	M4_MDBG_DELETE( m_poStatusBoard_ProcessNameKey, delete m_poStatusBoard_ProcessNameKey );
//	m_poStatusBoard_ProcessNameKey=0x0 ;

//	M4_MDBG_DELETE( m_poStatusBoard_TSAPIdKey, delete m_poStatusBoard_TSAPIdKey );
//	m_poStatusBoard_TSAPIdKey=0x0 ;

  //## end ClLaunchAppServer::~ClLaunchAppServer%939286310.body
}



//## Other Operations (implementation)
m4return_t ClLaunchAppServer::LaunchAppServer (ClAppCtlRequest *ai_poRequest)
{
  //## begin ClLaunchAppServer::LaunchAppServer%938506849.body preserve=yes
	m4return_t iRet ;
	m4uint32_t iRequestID ;
	M4ClString ConfigFile;
	M4ClString LauncherPath ;
	ClXMLDoc XMLDoc;
	m4string_t szServerName , szCommand;


	if (! ( XMLDoc = ai_poRequest ->GetBufferIn () ))
	{
		return M4_ERROR ;
	}


	iRet =  BuildLauncherPath (XMLDoc   , LauncherPath , ConfigFile , iRequestID , szServerName , szCommand) ;

	ai_poRequest ->SetRequestID ( iRequestID ) ;

	if ( M4_ERROR == iRet )
	{
		return M4_ERROR ;
	}


	M4_SRV_GENERAL( "Arrancando el servidor %0:s ConfigFile %1:s XMLDoc %2:s", LauncherPath.c_str() << ConfigFile.c_str() << XMLDoc  ) ; 


	iRet = RunLauncherEXE (LauncherPath, ConfigFile, XMLDoc, szServerName , szCommand ) ;

	return iRet ;
  //## end ClLaunchAppServer::LaunchAppServer%938506849.body
}

m4return_t ClLaunchAppServer::ReadConfigurationFile (m4char_t *  ai_block_name, m4char_t *  ai_property_name, m4char_t * &  ao_pcvalue, m4uint32_t ai_iValueLen, m4pchar_t ai_pcConfigutationFile)
{
  //## begin ClLaunchAppServer::ReadConfigurationFile%938506846.body preserve=yes

	const m4pchar_t ini_default_value = "ini_default_value";
	const int buffer_size = APP_INFO_MAX_BUFFER;
	m4char_t Value[buffer_size];
	m4pchar_t pValue = Value;
	m4return_t iRet;

	iRet=GetPrivateProfileString(ai_block_name, ai_property_name, ini_default_value , ao_pcvalue, ai_iValueLen, ai_pcConfigutationFile);

	if(iRet<=0)
	{
		M4_SRV_WARNING( M4_ERR_APPCTL_UNKNOWN_CFG_FILE_PROP, "M4AppCtl: %0:s.\nError in Init file: Property %1:s not found in file %2:s, block %3:s ",
			m_oCommonVariables.m_strHost << ai_property_name  << ai_pcConfigutationFile << ai_block_name );

		return M4_ERROR;
	}

	if ( ! stricmp ( ini_default_value , ao_pcvalue ) )
	{
		//bg 89275. Dispatcher_host es opcional
		if ( ! stricmp ("dispatcher_host", ai_property_name) )
		{
			M4_SRV_WARNING( M4_ERR_APPCTL_UNKNOWN_CFG_FILE_PROP, 
				"M4AppCtl: %0:s.\nError in Init file: Property %1:s not found in file %2:s, block %3:s ",
				m_oCommonVariables.m_strHost << ai_property_name  << ai_pcConfigutationFile << ai_block_name );
			return M4_ERROR ;
		}
		else
		{
			M4_SRV_ERROR( M4_ERR_APPCTL_UNKNOWN_CFG_FILE_PROP, 
				"M4AppCtl: %0:s.\nError in Init file: Property %1:s not found in file %2:s, block %3:s ",
				m_oCommonVariables.m_strHost << ai_property_name  << ai_pcConfigutationFile << ai_block_name );
			return M4_ERROR ;
		}
	}

	m4char_t before[APP_INFO_MAX_BUFFER], variable[APP_INFO_MAX_BUFFER], after[APP_INFO_MAX_BUFFER];

	while (strchr(ao_pcvalue,'%'))
	{

		memset( before, '\0' , APP_INFO_MAX_BUFFER );

		memset( variable, '\0' , APP_INFO_MAX_BUFFER );

		memset( after, '\0' , APP_INFO_MAX_BUFFER );

		SplitParseVariable( ao_pcvalue , before, variable, after);
		//get the variable value.
		//buscamos el valor de la variable 1º en el entorno, despues en la sección y 
		//por último en el global
		/* Get the value of the environment variable. */
		pValue  = getenv( variable );   
		if( pValue == NULL )
		{
			//buscamos en la seccion server
			iRet = GetPrivateProfileString(
				"server", variable, ini_default_value , Value, APP_INFO_MAX_BUFFER, ai_pcConfigutationFile);

			if(!strcmp(Value,"ini_default_value"))
			{
				//buscamos en la seccion 
				iRet = GetPrivateProfileString(
					ai_block_name, variable, ini_default_value , Value, APP_INFO_MAX_BUFFER, ai_pcConfigutationFile);

				if(!strcmp(Value,"ini_default_value"))
				{
					// buscamos en la global
					iRet = GetPrivateProfileString(
						GLOBAL_SECTION, variable, ini_default_value , Value, APP_INFO_MAX_BUFFER, ai_pcConfigutationFile);

					if(!strcmp(Value,"ini_default_value"))
					{
						iRet = GetPrivateProfileString(
						GLOBAL_SECTION, variable, ini_default_value , Value, APP_INFO_MAX_BUFFER, M4APPCTL_CONFIGURATION_FILE);

						if(!strcmp(Value,"ini_default_value")) {
							M4_SRV_WARNING( M4_ERR_APPCTL_UNKNOWN_CFG_FILE_PROP , 
								"M4AppCtl: %0:s.\nError in Init file: Propertie %1:s not found in file %2:s, block %3:s ",
								m_oCommonVariables.m_strHost << ai_property_name << ai_pcConfigutationFile << ai_block_name  );
//							return M4_ERROR;
							Value [0] = 0 ;
						} ;

					}

				}

			}
		}
		else strcpy( Value, pValue);

		// set the new output string.
		sprintf(ao_pcvalue, "%s%s%s",before, Value, after);
	}

	return M4_SUCCESS;


  //## end ClLaunchAppServer::ReadConfigurationFile%938506846.body
}

m4return_t ClLaunchAppServer::SplitParseVariable (m4pchar_t aio_value, m4pchar_t ao_before, m4pchar_t ao_variable, m4pchar_t ao_after)
{
  //## begin ClLaunchAppServer::SplitParseVariable%942399241.body preserve=yes
	m4pchar_t token = NULL;

	const int buffer_size = APP_INFO_MAX_BUFFER;

	m4char_t Buffer[buffer_size];

	m4char_t BufferAux[buffer_size];

	memset(BufferAux,'\0', buffer_size);

	memset(Buffer,'\0', buffer_size);

	strcpy ( Buffer , aio_value );

	int i=0;

	for(i ; i < buffer_size ; i++)
	{
		if(Buffer[i]!='%')
		{
			BufferAux[i] = Buffer[i];
		}
		else
			break;
	}

	strcpy( ao_before , BufferAux );

	token = strtok( Buffer , "%" );
		
	if(Buffer[0]!='%')
		token = strtok( NULL , "%" );

	if ( !token )

		return M4_ERROR; // no hay %

	else
	{
		strcpy(ao_variable,token);
		// busco token en el entorno, seccion o global
		token = strtok( NULL , "\0");

		if(token) strcpy( ao_after, token );

	}

	return M4_SUCCESS;

  //## end ClLaunchAppServer::SplitParseVariable%942399241.body
}

m4return_t ClLaunchAppServer::BuildLauncherPath (ClXMLDoc ai_XMLDoc, M4ClString &ao_pcLauncherPath, M4ClString &ao_pcConfigFile, m4uint32_t &ao_iRequestID, m4string_t &ao_szServerName, m4string_t &ao_szCommnad)
{
  //## begin ClLaunchAppServer::BuildLauncherPath%956568310.body preserve=yes
	m4uint32_t iBufferLen ;
	m4pchar_t pServerName ;
	m4char_t szBufferPath [1000];
	m4char_t szBufferFile [1000];
	m4pchar_t pBufferPath = szBufferPath;
	m4pchar_t pBufferFile = szBufferFile;
	m4pchar_t psCtlConfigFile = 0;


	iBufferLen = strlen ( ai_XMLDoc ) +1 ;

	SAXParser parser;
	ClAppCtlSAXHandler oAppCtlParser ;

    parser.setDocumentHandler(&oAppCtlParser);
    parser.setErrorHandler(&oAppCtlParser);

    
	MemBufInputSource * poMemBuffer = 0x0 ;
	M4_MDBG_NEW( poMemBuffer, poMemBuffer = 
        new MemBufInputSource((const XMLByte *) ai_XMLDoc,
          iBufferLen, "generic", false)
	) ;

    try
    {
        parser.parse(  *poMemBuffer ) ;
    }

    catch (const XMLException&)
    {

		//M4_SRV_ERROR(0, "File  parsing %0:s", (const XMLByte *) ai_XMLDoc ) ;
		M4_SRV_ERROR (M4_ERR_APPCTL_PARSE_ERROR, 
			"M4AppCtl: %0:s.\nError parssing XML from dispatcher. XML Doc: %1:s" , 
			m_oCommonVariables.m_strHost << ai_XMLDoc );
        return M4_ERROR ;
    }

	if ( !oAppCtlParser.IsOk() )
	{
		return M4_ERROR ;
	}

	pServerName = oAppCtlParser.GetServerName () ;

	ao_szCommnad = oAppCtlParser.GetCommand () ;

	ao_szServerName = pServerName  ;

	ao_iRequestID = oAppCtlParser.GetRequestID () ;

	if (0 != m_psAppCtlConfigPath)
	{
		psCtlConfigFile = m_psAppCtlConfigPath;
	}
	else
	{
		psCtlConfigFile = M4APPCTL_CONFIGURATION_FILE;
	}
	
	
	if( ReadConfigurationFile( pServerName , 
					CONF_FILE , 
					pBufferFile, 1000,
					psCtlConfigFile )==M4_ERROR )
	{
		M4_SRV_ERROR ( M4_ERR_APPCTL_NOCONFIGREGISTERED , 
			"M4AppCtl: %0:s.\nNot found configuration file path for m4server configuration: '%1:s'. No property specified or configuration not registered in site" ,
			m_oCommonVariables.m_strHost << pServerName ) ;
		return M4_ERROR;
	}

	if( ReadConfigurationFile( GLOBAL_SECTION , 
					LAUNCHER_PATH , 
					pBufferPath, 1000,
					pBufferFile )==M4_ERROR )
	{
		// error en la log;
		M4_SRV_ERROR ( M4_ERR_APPCTL_NOCONFIGFILE , 
			"M4AppCtl: %0:s.\nFile %1:s doesn't exist, or property %2:s is missing in that file" ,
			m_oCommonVariables.m_strHost << pBufferFile << LAUNCHER_PATH ) ;
		return M4_ERROR;
	}

	//Path del ejecutable del launcher
	ao_pcLauncherPath = pBufferPath ;

	//fichero de configuración
	
	ao_pcConfigFile = szBufferFile ;


	return M4_SUCCESS ;
  //## end ClLaunchAppServer::BuildLauncherPath%956568310.body
}

m4return_t ClLaunchAppServer::Initialize ()
{
  //## begin ClLaunchAppServer::Initialize%957775214.body preserve=yes

	// fix bug 0093611
	m4char_t LogSys_File[APP_INFO_MAX_BUFFER];

	m4pchar_t pLogSys_File = LogSys_File;
	m4return_t iRet;
	m4char_t BufferVariables [1024];
	m4pchar_t pBufferVariables = BufferVariables ;
	m4return_t oResult = M4_ERROR;
	m4pchar_t psCtlConfigFile = 0;



	if (0 != m_psAppCtlConfigPath)
	{
		psCtlConfigFile = m_psAppCtlConfigPath;
	}
	else
	{
		psCtlConfigFile = M4APPCTL_CONFIGURATION_FILE;
	}
	
	oResult = ReadConfigurationFile( GLOBAL_SECTION , 
		LOGSYS_FILE , pLogSys_File, APP_INFO_MAX_BUFFER, psCtlConfigFile );
	
	// Inicializamos log
	iRet = m4return_t( INITIALIZELOGMANAGER( pLogSys_File ) ) ;
	if (M4_ERROR == iRet)
	{
		M4_SRV_ERROR( 0, "Log Manager not initialized. Is the %0:s file OK?", pLogSys_File ) ;
	}
	else 
	{
		M4_SRV_GENERAL_N( "Log Manager initilized" ) ;
	}
	SETCODEF(M4_ERR_APPCTL_GENERICMSG, DEBUGINFOLOG, "Initializing Meta4 App Controller");

//	m_oCommonVariables.m_pcAdmin_Port = pAdminPort ;

	if( ReadConfigurationFile( GLOBAL_SECTION , 
					ADMIN_SERVICE_PORT , 
					pBufferVariables, 1024, 
					psCtlConfigFile )==M4_ERROR )
	{
	
		return M4_ERROR;
	}

	m_oCommonVariables.m_iAdminPort	= atoi ( pBufferVariables ) ;

	if ( ! m_oCommonVariables.m_iAdminPort )
	{
		M4_SRV_ERROR ( M4_ERR_APPCTL_WRONG_VAR_VALUE , 
			"M4AppCtl: No se ha podido leer la propiedad %1:s" , 
			/* m_oCommonVariables.m_strHost << */ ADMIN_SERVICE_PORT );

		return M4_ERROR;
	}

	M4_SRV_GENERAL( "Puerto de administracion: %0:s" , m_oCommonVariables.m_iAdminPort );
	
	if( ReadConfigurationFile( GLOBAL_SECTION , 
					CONTROL_PORT , 
					pBufferVariables, 1024,
					psCtlConfigFile )==M4_ERROR )
	{
		// error en la log;

		return M4_ERROR;
	}

	m_oCommonVariables.m_iControlPort	= atoi ( pBufferVariables ) ;

	if ( ! m_oCommonVariables.m_iControlPort	)
	{
		M4_SRV_ERROR( M4_ERR_APPCTL_WRONG_VAR_VALUE , 
			"M4AppCtl: No se ha podido leer la propiedad %1:s" , 
			/*m_oCommonVariables.m_strHost << */ CONTROL_PORT  );

		return M4_ERROR;
	}

	M4_SRV_GENERAL( "Puerto de control: %0:s" , m_oCommonVariables.m_iControlPort );

    try
    {
         XMLPlatformUtils::Initialize();
    }

    catch (const XMLException&)
    {
         return M4_ERROR;
    }

	return M4_SUCCESS ;

  //## end ClLaunchAppServer::Initialize%957775214.body
}

m4return_t ClLaunchAppServer::Run ()
{
  //## begin ClLaunchAppServer::Run%957775215.body preserve=yes
//	ClXMLDoc ai_XMLDoc ("startup.xml") ;
	m4return_t iRet ;
	m4char_t szHost [100];


	M4_MDBG_NEW( m_poCommunications, m_poCommunications = new ClCommunication (m_oCommonVariables.m_iControlPort) ) ;

	if ( M4_SUCCESS == gethostname ( szHost , 100 ) )
	{
		m_oCommonVariables.SetHost ( szHost ) ;
	}

	cout << "M4AppCtl: " << m_oCommonVariables.m_strHost << endl ;

	iRet=m_poCommunications -> StartAccepting(m_oCommonVariables.m_iAdminPort,ACCEPT_ASCII_CONNECTION,this,M4_COMM_ASCII_PROTOCOL);
	if(M4_SUCCESS!=iRet)
	{
		cout << endl << "Error start Accepting. AdminPort :" << m_oCommonVariables.m_iAdminPort << " That port could be unavailable" << endl ;
		return M4_ERROR ;
	}

	_InitializeDispatchers () ;

	ProcessRequest () ;

	m_oStopper.Wait () ;

	return M4_SUCCESS ;
  //## end ClLaunchAppServer::Run%957775215.body
}

void ClLaunchAppServer::Stop ()
{
  //## begin ClLaunchAppServer::Stop%957775219.body preserve=yes

	m_oCommonVariables.m_bStop_Demon = M4_TRUE;

	m_oRequestQ.Push ( ( ClAppCtlRequest*) NULL ) ;

	m_oStopper.Signal () ;

	// abro una conexion en el puerto de administración para que se salga del bucle el AdminAccept
	// no es muy bonito pero
	// el select acaba el solo al tener timeout

  //## end ClLaunchAppServer::Stop%957775219.body
}

m4return_t ClLaunchAppServer::ExecuteAction (m4pchar_t ai_oActionString, ClParamList *ai_poParamList)
{
  //## begin ClLaunchAppServer::ExecuteAction%959265102.body preserve=yes
	if ( ! stricmp ( ACCEPT_ASCII_CONNECTION , ai_oActionString) )
	{
		return AcceptedConnection ( ai_poParamList ) ;
	}

	if ( ! stricmp ( READ_ASCII_CONNECTION , ai_oActionString ) )
	{
		return ReadingConnection ( ai_poParamList ) ;
	}

	if ( ! stricmp ( POST_WRITE_ASCII_CONNECTION , ai_oActionString ) )
	{
		return PostWriteConnection ( ai_poParamList ) ;
	}

	if ( ! stricmp ( POST_CLOSE_CONNECTION , ai_oActionString ) )
	{
		return PostCloseConnection ( ai_poParamList ) ;
	}

	return M4_ERROR ;
  //## end ClLaunchAppServer::ExecuteAction%959265102.body
}

m4return_t ClLaunchAppServer::AcceptedConnection (ClParamList *ai_pParamList)
{
  //## begin ClLaunchAppServer::AcceptedConnection%959265103.body preserve=yes
		m4return_t RetValue;


	if(M4_ERROR==ai_pParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
		return M4_ERROR ;
	else
	{
		if(M4_ERROR==RetValue)
		{
			M4_SRV_ERROR ( M4_ERR_APPCTL_ACCEPTING_PORT, 
				"M4AppCtl: %0:s.\nThere has been an error accepting on port %1:s" ,
				m_oCommonVariables.m_strHost << m_oCommonVariables.m_iAdminPort  );
			return M4_ERROR ;
		}
		else
		{
			int iSessionID;
			if(M4_ERROR!=ai_pParamList->GetInt(M4_PARAM_SESSIONID, iSessionID))
			{
				m_poCommunications->StartListening(iSessionID, READ_ASCII_CONNECTION , this );
			}
			else
			{
				M4_SRV_ERROR(M4_ERR_APPCTL_NO_COMM_PARAM, 
					"M4AppCtl: %0:s.\nParam %1:s not found after a communication event", 
					m_oCommonVariables.m_strHost << M4_PARAM_SESSIONID ) ;
				return M4_ERROR ;
			}
		}
	}

	return M4_SUCCESS ;
  //## end ClLaunchAppServer::AcceptedConnection%959265103.body
}

m4return_t ClLaunchAppServer::ReadingConnection (ClParamList *ai_pParamList)
{
  //## begin ClLaunchAppServer::ReadingConnection%959265104.body preserve=yes
		m4return_t RetValue ;
		ClAppCtlRequest * poRequest ;
		void *thepointer;
		m4pchar_t m_pcBuffer = NULL ;
		ClLogSystem * poThreadLog = NULL , * poReqLog = NULL ;
		m4int_t iSessionID;



	if(M4_ERROR==ai_pParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
	{
		M4_SRV_ERROR(M4_ERR_APPCTL_NO_COMM_PARAM, 
			"M4AppCtl: %0:s.\nParam %1:s not found after a communication event" , 
			m_oCommonVariables.m_strHost << M4_PARAM_RETURN_RESULT) ;
		return M4_ERROR ;
	}

	if(M4_ERROR==ai_pParamList->GetInt(M4_PARAM_CONNECTIONID, iSessionID))
	{
		M4_SRV_ERROR(M4_ERR_APPCTL_NO_COMM_PARAM, 
			"M4AppCtl: %0:s.\nParam %1:s not found after a communication event" , 
			m_oCommonVariables.m_strHost << M4_PARAM_SESSIONID) ;
		return M4_ERROR ;
	}

	if(M4_ERROR==RetValue )
	{

//		M4_SRV_ERROR_N(0, "Han cerrado el otro lado de la conexion") ;
		if( M4_ERROR == m_poCommunications -> StopListening(iSessionID) )
		{
			M4_SRV_ERROR( M4_ERR_APPCTL_STOP_LISTENING, 
				"M4AppCtl: %0:s.\nError Stopping Reading" ,
				m_oCommonVariables.m_strHost );
			return M4_ERROR ;
		}
		else
		{
			M4_SRV_GENERAL_N ("Stop Reading OK");
			if ( M4_ERROR == m_poCommunications -> CloseConnection(iSessionID , POST_CLOSE_CONNECTION , this) )
			{
				return M4_ERROR ;
			}
		}
	}
	else
	{
		if(M4_ERROR==ai_pParamList->GetPointer(M4_PARAM_READED_BUFFER,thepointer)) 
		{
			M4_SRV_ERROR(M4_ERR_APPCTL_NO_COMM_PARAM, 
				"M4AppCtl: %0:s.\nParam %1:s not found after a communication event", 
				m_oCommonVariables.m_strHost << M4_PARAM_READED_BUFFER) ;

		} else
		{
			M4_MDBG_NEW( poRequest, poRequest = new ClAppCtlRequest ( (m4pchar_t) thepointer , iSessionID ) );

			m_oRequestQ.Push( poRequest ) ;
		}
	}

	return M4_SUCCESS ;
  //## end ClLaunchAppServer::ReadingConnection%959265104.body
}

m4return_t ClLaunchAppServer::RunLauncherEXE (M4ClString &ai_pcLauncherPath, M4ClString &ai_pcConfigFile, ClXMLDoc &ai_XMLDoc, m4string_t &ai_szServerName, m4string_t &ai_szCommand)
{
  //## begin ClLaunchAppServer::RunLauncherEXE%974113726.body preserve=yes

	int iRet = M4_SUCCESS ;
	m4uint32_t LastError;
	M4ClString CallStr ;
	m4char_t *pPos ;


#ifdef _UNIX
	CallStr = "";
#else
	CallStr = "start " ;
	CallStr += "\"Meta4 Application Server\" " ;
#endif


	CallStr += "\"" ;
	CallStr += ai_pcLauncherPath ; 
	CallStr += "\"" ;
	CallStr += " " ;
	CallStr += "-xml \"" ;  CallStr += ai_XMLDoc ; CallStr += "\" " ;
	CallStr += "-config \"" ; CallStr += ai_pcConfigFile ; CallStr += "\" " ;


	#ifdef _UNIX

	CallStr += " &" ;	//Importante, para que el proceso padre NO espere

	#endif

	//Para el shell, hay que quitar los saltos de linea. POdemos sustituirlos por blancos y ya esta.
	for (pPos=CallStr; *pPos; ++pPos) {
		if ( *pPos=='\n' ) *pPos=' ' ;
	} ;

	M4_SRV_DETAILED(  "Executing %0:s", CallStr.c_str() ) ;
	iRet = system(CallStr) ;

	if( 0 != iRet)
	{

	cout << "ERROR " ;

	#ifdef _WINDOWS
		LastError = GetLastError();
		M4_SRV_ERROR( M4_ERR_APPCTL_STARTING_PROCESS , 
			"M4AppCtl: %0:s.\nError starting process, Error '%1:s' in system call '%2:s'", 
			m_oCommonVariables.m_strHost << LastError << CallStr.c_str() );
	#else
		M4_SRV_ERROR( M4_ERR_APPCTL_STARTING_PROCESS , 
			"M4AppCtl: %0:s.\nError starting process, Error '%1:s' in system call '%2:s'", 
			m_oCommonVariables.m_strHost << iRet << CallStr.c_str());
	#endif

		iRet = M4_ERROR ;
	}
	
	cout << "Executing command '" << ai_szCommand.c_str() << "' Meta4 Application Server '" << ai_szServerName.c_str () << "'" << endl;

	return iRet ;
  //## end ClLaunchAppServer::RunLauncherEXE%974113726.body
}

void ClLaunchAppServer::SetAppCtlConfigPath (m4pchar_t ai_psAppCtlExePath)
{
  //## begin ClLaunchAppServer::SetAppCtlConfigPath%975327348.body preserve=yes
	
	m_psAppCtlConfigPath = ai_psAppCtlExePath;

  //## end ClLaunchAppServer::SetAppCtlConfigPath%975327348.body
}

m4return_t ClLaunchAppServer::PostWriteConnection (ClParamList *ai_pParamList)
{
  //## begin ClLaunchAppServer::PostWriteConnection%981474756.body preserve=yes
	m4int64_t iPetitionID ;
	m4int_t iSessionID ;
	ClAppCtlRequest * poRequest ;



	if ( M4_ERROR == ai_pParamList -> GetInt64 ( M4_PARAM_PETITION_ID , iPetitionID ) )
	{

		return M4_ERROR ;
	}


	if ( M4_SUCCESS == m_oRepository.RemoveRequest ( iPetitionID , poRequest ) )
	{
		M4_MDBG_DELETE( poRequest, delete poRequest ) ;
	}

	if(M4_ERROR==ai_pParamList->GetInt(M4_PARAM_CONNECTIONID, iSessionID))
	{
		M4_SRV_ERROR(M4_ERR_APPCTL_NO_COMM_PARAM, 
			"M4AppCtl: %0:s.\nParam %1:s not found after a communication event", 
			m_oCommonVariables.m_strHost << M4_PARAM_SESSIONID) ;
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_poCommunications -> CloseConnection(iSessionID , POST_CLOSE_CONNECTION , this ) )
	{
		M4_SRV_ERROR_N(0,"Error cerrando conexion");
		return M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClLaunchAppServer::PostWriteConnection%981474756.body
}

m4return_t ClLaunchAppServer::PostCloseConnection (ClParamList *ai_pParamList)
{
  //## begin ClLaunchAppServer::PostCloseConnection %981535914.body preserve=yes
	return M4_SUCCESS ;
  //## end ClLaunchAppServer::PostCloseConnection %981535914.body
}

void ClLaunchAppServer::ProcessRequest ()
{
  //## begin ClLaunchAppServer::ProcessRequest%998490240.body preserve=yes
	ClAppCtlRequest * poRequest = NULL ;
	ClLogSystem * poReqLog = NULL , * poThreadLog = NULL ;
	m4return_t iRet ;
	m4uint32_t iReqID ;
	m4int_t iSessionID;


	do {

		m_oRequestQ.Pop ( poRequest ) ;

		if (  poRequest )
		{		
			if ( poReqLog = poRequest ->GetLog () )
			{
				poThreadLog = SETLOGSYSTEM ( poReqLog ) ;
				poReqLog -> SetAttributeValue ( "M4_LOGSYSTEM_USERID" , m_oCommonVariables.m_strHost) ;
			}
			
//			iRet=m_poCommunications -> StopAccepting(m_oCommonVariables.m_iAdminPort);
			
			iRet = LaunchAppServer (poRequest) ;
			
//			iRet=m_poCommunications -> StartAccepting(m_oCommonVariables.m_iAdminPort,ACCEPT_ASCII_CONNECTION,this,M4_COMM_ASCII_PROTOCOL);
			
			poRequest -> SetResult ( iRet ) ;
			
			poRequest ->GenerateAnwer ( ) ;
			
			m4pchar_t pBuffer = poRequest ->GetBufferOut ( ) ;
			
			do
			{
				iReqID = ClIdGenerator::GetNewId () ;
			}
			while ( M4_ERROR == m_oRepository.AddRequest ( iReqID , poRequest ) ) ;
			
			iSessionID = poRequest -> GetConnectionID () ;

			iRet = m_poCommunications -> Write (iSessionID, iReqID ,  pBuffer ,strlen ( pBuffer ) +1 , POST_WRITE_ASCII_CONNECTION , this) ;
			
			if ( poThreadLog )
			{
				SETLOGSYSTEM ( poThreadLog  ) ;
			}
		}
	}	while ( poRequest ) ;

  //## end ClLaunchAppServer::ProcessRequest%998490240.body
}

// Additional Declarations
  //## begin ClLaunchAppServer%37F0949602C2.declarations preserve=yes
m4return_t ClLaunchAppServer::_InitializeDispatchers ( ) 
{
	m4uint32_t iLen ;
	m4char_t szNames  [1000];
	m4char_t szPort [1000] , szHost [1000];
	m4pchar_t pName = szNames ;
	m4pchar_t pcHostvalue = szHost, pcPortvalue = szPort ;
	m4uint32_t iReqID , iConnectionId ;
	ClAppCtlRequest * poRequest ;
	m4uint32_t iPort ;
	m4return_t iRet ;
	m4string_t strHostvalue ;



	iLen = GetPrivateProfileSectionNames ( szNames , 1000 , m_psAppCtlConfigPath ) ;

	while (pName[0])
	{
		if ( M4_SUCCESS == ReadConfigurationFile (pName, "dispatcher_host", pcHostvalue, 1000, m_psAppCtlConfigPath ) )
		{
			if ( M4_SUCCESS == ReadConfigurationFile (pName, "dispatcher_port", pcPortvalue, 1000, m_psAppCtlConfigPath ) )
			{
				if ( sscanf ( pcPortvalue , "%d" , &iPort ) )
				{
					strHostvalue = pcHostvalue ;

					iConnectionId  = ClIdGenerator::GetNewId () ;

					if ( M4_SUCCESS ==  m_poCommunications -> OpenConnection ( iConnectionId, strHostvalue, iPort ,M4_COMM_ASCII_PROTOCOL ) )
					{
						
						poRequest = new ClAppCtlRequest  ;
						
						do
						{
							iReqID = ClIdGenerator::GetNewId () ;
						}
						while ( M4_ERROR == m_oRepository.AddRequest ( iReqID , poRequest ) ) ;

						poRequest -> GenerateDispatcherUpdate ( pName , m_oCommonVariables.m_strHost , m_oCommonVariables.m_iAdminPort ) ;
						
						do
						{
							iReqID = ClIdGenerator::GetNewId () ;
						}
						while ( M4_ERROR == m_oRepository.AddRequest ( iReqID , poRequest ) ) ;
						
						
						poRequest -> SetConnectionID ( iConnectionId ) ;
						
						
						iRet = m_poCommunications -> Write (iConnectionId, iReqID ,  poRequest->GetBufferIn () ,strlen ( poRequest->GetBufferIn () ) +1 ) ;

					}
				}
				  
			}
		}
		pName += strlen (pName) + 1;
	}

	return M4_SUCCESS ;
}

  //## end ClLaunchAppServer%37F0949602C2.declarations

//## begin module%37F095DF01C1.epilog preserve=yes
//## end module%37F095DF01C1.epilog
