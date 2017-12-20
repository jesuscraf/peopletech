//## begin module%3492936201C4.cm preserve=no
//## end module%3492936201C4.cm

//## begin module%3492936201C4.cp preserve=no
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
//## end module%3492936201C4.cp

//## Module: ClSubsystem%3492936201C4; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\clsubsystem.cpp

//## begin module%3492936201C4.additionalIncludes preserve=no
//## end module%3492936201C4.additionalIncludes

//## begin module%3492936201C4.includes preserve=yes
//#include <m4versioninfo.hpp>
#include <m4srvres.hpp>
#include <m4types.hpp>
#include <subsystemids.hpp>
#include <stdio.h>
#include <m4servenv.hpp>
#include <usservice.hpp>
#include <clpconfiguration.hpp>
#include <fileutil.hpp>
#include <logoninterface.hpp>
#include <tsap.hpp>
#include <cloblconfiguration.hpp>
#include <userss.hpp>
#include <m4filemanager.hpp>
#include <ssnames.hpp>
#include <m4log.hpp> 
#include <clarraylibres.hpp>
#include <clloadtables.hpp>
#include <navigatorss.hpp>
#include <fcntl.h>
#include <sys/stat.h>
#include <controlpipesmkr.hpp>
#include <m4eventhandlers.hpp>
#include <clconfiguration.hpp>
#include <clssinvoker.hpp>
#include <ssfactory.hpp>
#include <request.hpp>
#include <m4srvtrace.h>
#include <exvariables.hpp>
#include <m4exception.hpp>
#include "m4unicode.hpp"

extern int	M4IsUnicode( void ) ;


#ifdef _UNIX
#include <sys/types.h>
#else
#include <io.h>
#endif

//## end module%3492936201C4.includes

// m4condition
#include <m4condition.hpp>
// Statistics
#include <statistics.hpp>
// ClAdminTag
#include <cladmintag.hpp>
// SystemInterface
#include <systeminteface.hpp>
// ClSubsystem
#include <clsubsystem.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>

class ClServEnv;
class ClCSSessManager;
class ClUSServiceSS;

//## begin module%3492936201C4.declarations preserve=no
//## end module%3492936201C4.declarations

//## begin module%3492936201C4.additionalDeclarations preserve=yes

//Esto debe de cambiar al fichero m4srvres.h en la siguiente versión


#define MAXBLOCKS 100
#define SD_SECUENCE "SERVER_SESSION;SUPERVISOR;USERS;COMMUNICATION_SERVER;SERVICES;OLTP_SUBSYSTEM;PROXY_SUBSYSTEM;JOB_SCHEDULER_SUBSYSTEM;METADATA_SUBSYSTEM;"
// SSL Files
#define CERTF	"servercert.pem"
#define KEYF	"serverpriv.pem"

//por defecto

#ifdef _M4SOLSPARC_ 
	#define M4_SERVER_ENV_VAR_ARCH	M4_ARCH_STR_SPARC
#endif
#ifdef _WINDOWS
	#define M4_SERVER_ENV_VAR_ARCH	M4_ARCH_STR_INTEL 
#endif
#ifdef _M4SOLX86_
	#define M4_SERVER_ENV_VAR_ARCH	M4_ARCH_STR_INTEL 
#endif
#ifdef _M4AIX_
	#define M4_SERVER_ENV_VAR_ARCH	M4_ARCH_STR_AIX
#endif

#ifndef M4_SERVER_ENV_VAR_ARCH	
#define M4_SERVER_ENV_VAR_ARCH	NULL
#endif


#define M4_SRV_TRACE_COMP_ID	"COMSRV"

//## end module%3492936201C4.additionalDeclarations


// Class ClSystem 








//## begin ClSystem::m_poSystem%36D5A7BC0024.role preserve=no  protected: static ClSystem { -> RHAN}
ClSystem *ClSystem::m_poSystem = NULL;
//## end ClSystem::m_poSystem%36D5A7BC0024.role





ClSystem::ClSystem ()
  //## begin ClSystem::ClSystem%882023701.hasinit preserve=no
      : m_bServerStarted(M4_FALSE), m_bStopExecution(M4_FALSE), m_lDeferedTime(0)
  //## end ClSystem::ClSystem%882023701.hasinit
  //## begin ClSystem::ClSystem%882023701.initialization preserve=yes
  ,	ClSSImplementation ( NULL , NULL , NULL ) , m_szConfigPath ( NULL ) , m_poCondition ( NULL ) 
  //## end ClSystem::ClSystem%882023701.initialization
{
  //## begin ClSystem::ClSystem%882023701.body preserve=yes
	m_szPassword = NULL ;
	m_szUserName = NULL ;
	m_poVisitor = NULL ;
	m_poSSFactory = new ClSSFactory ;
  //## end ClSystem::ClSystem%882023701.body
}


ClSystem::~ClSystem ()
{
  //## begin ClSystem::~ClSystem%882023702.body preserve=yes
	
	if ( m_szConfigPath )
	{
		delete [] m_szConfigPath ;

		m_szConfigPath = NULL ;
	}

	if ( m_poTables )
	{

		m_poTables = NULL ;
	}

	if  ( m_szUserName )
	{
		delete m_szUserName ;

		m_szUserName =NULL;
	}

	if ( m_szPassword )
	{
		delete m_szPassword ;

		m_szPassword = NULL ;
	}

  //## end ClSystem::~ClSystem%882023702.body
}



//## Other Operations (implementation)
void ClSystem::Accept (ClSystemVisitor &ai_oVisitor)
{
  //## begin ClSystem::Accept%884777573.body preserve=yes
	ai_oVisitor.VisitSystem ( * this ) ;
  //## end ClSystem::Accept%884777573.body
}

m4return_t ClSystem::Synchronize ()
{
  //## begin ClSystem::Synchronize%884942686.body preserve=yes
	M4ClTimeStamp clock ;
	M4ClString szCurrentDate ;
	m4return_t iRet;
	m4time_t GMTime ;
	m4time_t DifTime ;
	m4int32_t iyear, imon, iday, ihour, imin, isec ;

	// reset ClStatus time
	ClStatus::UpdateTime();

	clock.now ( ) ;
    clock.get (GMTime);

	m4date_t oActualDate = ClActualDate(  (m4uint8_t)M4CL_TIME_LOCAL_TIME) ;

	DifTime = oActualDate - GMTime ;

	M4ClTimeStamp oDifStamp ( DifTime );

	M4ClTimeStamp oLocalTime ( oActualDate );

	oLocalTime.get ( szCurrentDate  ) ;

    oDifStamp.get ( iyear, imon, iday, ihour, imin, isec);

	szCurrentDate += " ( GMT + " ;

	szCurrentDate += ihour ;

	szCurrentDate += " )" ;

	iRet = GetConfiguration() -> SetStringValue ( "" , CURRENT_TIME , (m4pchar_t) szCurrentDate ) ;

	if ( m_bServerStarted )
	{
		if ( m_poVisitor )
		{
			Accept (  * ( (ClSystemVisitor * ) m_poVisitor )  ) ;
		}

	}

	return ClSSImplementation::Synchronize( ) ;
  //## end ClSystem::Synchronize%884942686.body
}

m4return_t ClSystem::Initialize ()
{
  //## begin ClSystem::Initialize%885806506.body preserve=yes
		m4uint32_t iType = M4_SERVER_SUBSYSTEM ;
		m4char_t szFileManPath [ OBL_PROPVALUE_MAX_LEN ] ;
		m4char_t szLogFile [ MAX_CONFIG_PATH_LEN ] ;
		m4char_t szTables [ MAX_CONFIG_PATH_LEN ] ;
		m4char_t szLiterals [ MAX_CONFIG_PATH_LEN] ;
		m4char_t szProperties [ MAX_CONFIG_PATH_LEN] ;
//		m4char_t szCertPath [ MAX_CONFIG_PATH_LEN ] = "" ;
		m4char_t szHostName [OBL_PROPVALUE_MAX_LEN] ;
//		m4uint_t iLenInstance;
		size_t iLen ;
//		m4return_t iRet ;
		m4uint32_t iLenOut ;
		M4ClTimeStamp clock ;
		M4ClString szCurrentDate ;
		M4ClString szKeyf ;
		M4ClString szCertf ;


		m_poStatistics = new ClSystemStats ;

	SocketInit ( ) ;

	if ( M4_SUCCESS == gethostname ( szHostName , OBL_PROPVALUE_MAX_LEN ) )
	{
		GetConfiguration() -> SetStringValue ("" , "SERVER_HOST_NAME" , szHostName ) ;
	}

	//leeer el path al fichero de inicialización de log
	if ( GetConfiguration() -> GetStringValue ( "" , "LOG_FILE" , szLogFile , MAX_CONFIG_PATH_LEN, iLen ) == M4_ERROR )
	{

		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", "LOG_FILE" << GetName ( ) );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "No 'LOG_FILE' property found in configuration file: %s" , m_szConfigPath ) ;

		cout << m_szConfigPath <<": Log File not specified\n" << flush ;

	}
	//Inicializar la log con el fichero dado en el path
	if ( INITIALIZELOGMANAGER( szLogFile )== M4_ERROR )
	{
		cout <<"\nLog Manager was not initialiced with file " << szLogFile << "\n" << flush ;
	}

	m_poVisitor =   (ClVisitor*)new ClSystemVisitor ;


//Inicializar el Log manager a partir del path que estará en la obl
	if ( GetConfiguration() -> GetStringValue ( "" , FILE_MANAGER_PATH , szFileManPath , MAX_CONFIG_PATH_LEN, iLen ) == M4_ERROR )
	{
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", FILE_MANAGER_PATH  << GetName ( ) );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "No %s property found in configuration file: %s", FILE_MANAGER_PATH , m_szConfigPath ) ;

		cout <<  m_szConfigPath  << ": No path specified for File Manager initialization\n" << flush ;

		m4sleep (3 ) ;
	}
	else
	{
		ClFileManager::Instance()->SetDefaultDirectory( szFileManPath );
	}


	//Leer el path a fichero de propiedades
	if ( GetConfiguration() -> GetStringValue ( "" , PROPERTIES_FILE , szProperties , MAX_CONFIG_PATH_LEN, iLen ) == M4_ERROR )
	{

		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s",PROPERTIES_FILE << GetName ( ) );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "No %s property found in configuration file: %s", PROPERTIES_FILE , m_szConfigPath ) ;

		cout << m_szConfigPath << ": Properties file not specified\n" << flush ;

		m4sleep (3 ) ;

		return M4_ERROR ;
	}

	if ( GetConfiguration() -> GetStringValue ( "" , TABLES_FILE , szTables , MAX_CONFIG_PATH_LEN, iLen ) == M4_ERROR )
	{
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s",TABLES_FILE << GetName ( ) );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "No %s property found in configuration file: %s", TABLES_FILE , m_szConfigPath ) ;

		cout << m_szConfigPath  << ": Tables file not specified\n" << flush ;

		m4sleep (3 ) ;

		return M4_ERROR ;
	}

	if ( GetConfiguration() -> GetStringValue ( "" , LITERALS_FILE , szLiterals , MAX_CONFIG_PATH_LEN, iLen ) == M4_ERROR )
	{
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s",LITERALS_FILE << GetName ( ) );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "No %s property found in configuration file: %s", LITERALS_FILE , m_szConfigPath ) ;

		cout << "En " << m_szConfigPath  << " no se especifica un path para el fichero de literales\n" << flush ;

		m4sleep (3 ) ;

		return M4_ERROR ;
	}

	m_poTables = new ClLoadTables ;


	m_poInterfaz = new ClSSInterfaz ( this ) ;

	m_poInterfaz -> SetType ( iType ) ;

	//Chequeo de la existencia de los ficheros de configuración
	if ( M4_ERROR == GetSizeFile ( szTables , iLenOut ) )
	{
		M4_SRV_ERROR ( M4_SRV_SS_OBL_ERROR , "It was not possible to load configuration file %s" , szTables ) ;

		cout << "File not found: " << szTables << "\n" << flush ;

		m4sleep (3 ) ;

		return M4_ERROR ;
	}

	if ( M4_ERROR == GetSizeFile ( szLiterals , iLenOut ) )
	{
		M4_SRV_ERROR ( M4_SRV_SS_OBL_ERROR , "It was not possible to load configuration file %s" , szLiterals ) ;

		cout << "File not found: " << szLiterals << flush;

		m4sleep (3 ) ;

		return M4_ERROR ;
	}

	if (  ((ClLoadTables*)m_poTables )-> InitializeTables ( szTables ) == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_SS_OBL_ERROR , "It was not possible to load configuration file %s" , szTables ) ;

		return M4_ERROR ;
	}
	if (  ((ClLoadTables*)m_poTables )-> InitializeLiterals ( szLiterals ) == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_SS_OBL_ERROR, "It was not possible to load properties file %s" , szLiterals ) ;

		return M4_ERROR ;
	}
	if (  ((ClLoadTables*)m_poTables )-> InitializeProperties ( szProperties ) == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_SS_OBL_ERROR, "It was not possible to load properties file %s" , szProperties  ) ;

		return M4_ERROR ;
	}


	clock.now ( ) ;
	clock.get ( szCurrentDate ) ;

	GetConfiguration()  -> SetStringValue ( "" , CURRENT_TIME , (m4pchar_t) szCurrentDate ) ;
	GetConfiguration()  -> SetStringValue ( "" , "M4SERVER_MEMORY_COUNTABILITY" , "Unavailable" ) ;
	GetConfiguration()  -> SetStringValue ( "" , "CONSUMED_SYSTEM_MEMORY" , "Unavailable" ) ;
	GetConfiguration()  -> SetStringValue ( "" , SERVER_MEMORY_LEVEL , "Unavailable" ) ;
	GetConfiguration()  -> SetStringValue ( "" , "M4SERVER_MEMORY_PERCENT_COUNTABILITY" , "Unavailable" ) ;
	GetConfiguration()  -> SetStringValue ( "" , "CPU_USAGE" , "Unavailable" ) ;
	GetConfiguration()  -> SetStringValue ( "" , "MAX_PROC_SIZE" , "Unavailable" ) ;
	GetConfiguration()  -> SetStringValue ( "" , "FREE_RAM_MEM" , "Unavailable" ) ;
	return M4_SUCCESS ;
  //## end ClSystem::Initialize%885806506.body
}

m4return_t ClSystem::ShutDown ()
{
  //## begin ClSystem::ShutDown%886498837.body preserve=yes
	m4return_t iRet = M4_ERROR ;
	m4objid_t lId ;
	m4uint32_t isd_len , iTerations = 0 ;
	double dStop = 0;
	m4uint32_t iStop ;
	ClSSInterfaz * poSubsystem ;
	static ClMutex oMutex (M4_TRUE ) ;
	m4pchar_t sd_stream , subsystem ;
	m4bool_t bReadyToClose ;
	size_t iLen ;
	M4ClCrono	tickCounter;
	m4char_t szMessage [100] ;
	M4CheckJump oJump;
	double dShutdownTimeout = 0;
	M4ClTimeStamp oCrono;
	m4double_t dShutdownStart, dElapsedTime;


//No se hará el unlock del mutex para evitar que nadie más pueda hacer el shutdown
		oMutex.Lock ( ) ;
	
	SETCODEF ( M4_SERVER_SHUTDOWN , DEBUGINFOLOG , "SHUTTING DOWN SERVER" ) ;
	//PRIMERO SE HACE EL STOP DE TODOS LOS SUBSYSTEMAS

	SetState ( SS_SHUTING_DOWN , SS_STR_SHUTTING_DOWN ) ;

	Notify ( SHUTTING_DOWN );
	//Buscar la secuencia de intevalos de stop en la obl

	if (  GetConfiguration() )
	{
		if ( M4_ERROR == GetConfiguration()  -> GetNumericValue ( "" , "STOP_NUM_INTERVALS" ,  dStop ) )
		{
			 dStop = m_lDeferedTime ;
		}
	}

	iStop = (m4uint32_t) dStop ;

	while ( iStop > 0 )  
	{
		sprintf ( szMessage , "%d#" , iStop ) ;

		iRet = M4PublishEvent("65549", szMessage) ;

		tickCounter.Start() ;

		Stop (iStop * 60 ) ;

		m4sleep ( 60 - ( tickCounter.Stop()/1000 ) );

		iStop -- ;

	} 

	Notify (  SERVER_STOPPED ) ;


	//Tiempo que queda = 0
	Stop (0) ;


	// Eh 168508
	// Timeout para evitar esperar infinitamente al shutdown del servidor
	if( GetConfiguration() )
	{
		if( M4_ERROR == GetConfiguration()->GetNumericValue( "" ,"SHUTDOWN_TIMEOUT_MN" ,dShutdownTimeout ) )
		{
			 dShutdownTimeout = 0 ;
		}
		else
		{
			if( dShutdownTimeout < 0 )
			{
				dShutdownTimeout = 0 ;
			}

			oCrono.now() ;
			oCrono.get( dShutdownStart ) ;
		}
	}

	do
	{
		if( dShutdownTimeout > 0 )
		{
			oCrono.now () ;
			oCrono.get( dElapsedTime ) ;
			dElapsedTime = ( dElapsedTime - dShutdownStart ) * 24 * 60 ;

			if( dElapsedTime >= dShutdownTimeout )
			{
				SETCODEF( M4_SERVER_SHUTDOWN , WARNINGLOG, "Shutdown timeout reached. Process exiting." ) ;
				exit( 0 ) ;
			}
		}

		bReadyToClose = IsReadyToClose () ;

		m4sleep (1) ;

		cout << "." << flush ;

		iTerations ++;

	} while (! bReadyToClose ) ;


//Se hace el shutdown controlado de forma ordenada

	isd_len = strlen ( SD_SECUENCE ) +10 ;

	sd_stream = new m4char_t [MAXNAMEPROP];

	memset (sd_stream , 0 , isd_len ) ;

	iRet = M4_ERROR ;

	if ( ! GetConfiguration() )
	{
		return M4_ERROR ;
	}
	
	iRet = GetConfiguration()  -> GetStringValue ( "" , SHUTDOWN_SECUENCE , sd_stream , MAXNAMEPROP , iLen)  ;
	

	if (iRet == M4_ERROR ) 
	{
		strcpy ( sd_stream , SD_SECUENCE ) ;
	}

	try
	{
		// fix bug 0079564
		oJump.Check();

		while ( sd_stream )
		{
			//		iRet = sscanf ( sd_stream , "%s;" , subsystem ) ;

			subsystem = sd_stream ;
			
			if ( sd_stream = strstr (subsystem , ";" ) )
			{
				sd_stream [0] = 0;
				
				sd_stream ++ ;
				
				if ( ! ( poSubsystem = FindSubsystemByName ( subsystem ) ) )
				{
					// fix aix exception bug
					M4CheckJump *ref = M4CheckJump::CheckThrow();
					if (ref != NULL) {
						throw ref->getException();
					}
					SETCODEF ( M4_SRV_SS_TREE_ERROR , WARNINGLOG , "'GetSubsystemType' call failed searching for '%s'" , subsystem  ) ;
					
				}
				else
				{
					lId = poSubsystem -> GetId ()  ;
					
					RemoveSubsystem ( 0, lId) ;
					
					// fix aix exception bug
					M4CheckJump *ref = M4CheckJump::CheckThrow();
					if (ref != NULL) {
						throw ref->getException();
					}
				}
			}
		}

	} 
    catch ( M4RuntimeException &e )
	{
        // bugid: 0087079
		SETCODEF (M4_EXCEPTION_CAUGHT, ERRORLOG, "#*s1*#%d#%s#%s#%s#", e.GetException(), e.what(), "MAIN" , "Shutting down" );

		exit ( -1 ) ;
	}
	//Ahora se hace el shut down de lo que queda
	iRet = ClSSImplementation::ShutDown ( ) ;

	if ( iRet== M4_ERROR )
	{
		return M4_ERROR ;
	}

	STORELOGS( ) ;

	DESTROYLOGMANAGER();

	m_poCondition -> Signal ( ) ;
	

	SETCODEF ( M4_SERVER_SHUTDOWN , DEBUGINFOLOG , "SERVER SHUT DOWN" ) ;

//Se debería de borrar el navegador del servicio de variables de entorno
//	delete ( ClServEnv::m_pClNavigatorFirewall ) ;

	return M4_SUCCESS ;

  //## end ClSystem::ShutDown%886498837.body
}

m4return_t ClSystem::ExecuteCommandRoot (m4pchar_t ai_szPath, m4char_t ai_cCommand, M4DataStorage *ai_poDataParams, M4DataStorage *ao_poDataOut)
{
  //## begin ClSystem::ExecuteCommandRoot%889700967.body preserve=yes


	return M4_ERROR;
  //## end ClSystem::ExecuteCommandRoot%889700967.body
}

m4return_t ClSystem::RemoveSubsystem (m4objid_t ai_lIdBase, m4objid_t ai_lSubsystemId)
{
  //## begin ClSystem::RemoveSubsystem%889784166.body preserve=yes
	ClSSInterfaz * poSubsystem ;
	m4return_t iRet ;




	poSubsystem = FindSubsystemById ( ai_lIdBase ) ;

	if ( ! poSubsystem )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "'RemoveSubsystem' call failed searching for subsystem %d" , ai_lIdBase ) ;

		return M4_ERROR ;
	}

	iRet = poSubsystem -> RemoveSubsystem (ai_lSubsystemId) ;


	return iRet ;
  //## end ClSystem::RemoveSubsystem%889784166.body
}

ClSystem * ClSystem::Instance ()
{
  //## begin ClSystem::Instance%895825798.body preserve=yes
	if ( ! m_poSystem )
	{
		m_poSystem  = new ClSystem ;
	}

	return m_poSystem ;
  //## end ClSystem::Instance%895825798.body
}

m4return_t ClSystem::StartUp (ClCondition *ai_poCondition, m4pchar_t ai_szConfigPath, m4pchar_t ai_szUser, m4pchar_t ai_szPassword, ClCryptStore *ai_poCryptStore, m4pchar_t ai_szPropertiesFile, m4pchar_t ai_szLiterals, m4pchar_t ai_szTables, m4pchar_t ai_szLogSys, m4pchar_t ai_szProcessName, m4pchar_t ai_szConfigName, m4uint32_t ai_uControlRetryTime, m4uint32_t ai_iDispatcherPort, m4pchar_t ai_szDispatcherHost, m4uint32_t ai_iBasePort)
{
  //## begin ClSystem::StartUp%895825799.body preserve=yes
	ClOblConfiguration * poConfiguration ;
	ClPropertiesConfiguration * poLogConfiguration ;
	ClPropertiesConfiguration * poPConfiguration ;
	ClSSInterfaz * poLogSubsystem ;
//	ClNavigatorSS * poNavigator  = NULL ;
	m4return_t iRet; 
	m4uint32_t iSize = 1024  , iLenOut ;
	m4uint32_t iSizeObl = 1 ;
	UINT hBlockHandle   ;
	m4char_t szServerName [OBL_PROPVALUE_MAX_LEN] ;
	m4uint_t ilen ;


	// reset ClStatus time
	ClStatus::UpdateTime();

//	m4sleep (30 ) ;
	//Copiar los atributos de usuario y password

	m_szUserName = new m4char_t [strlen ( ai_szUser ) +1 ] ;

	m_szPassword = new m4char_t [strlen (ai_szPassword) +1 ] ;

	strcpy ( m_szUserName , ai_szUser ) ;

	strcpy ( m_szPassword , ai_szPassword ) ;


//Asigna un navegador instanciable a la dll servenv para que cualquier módulo pueda acceder a ella
	m_poNavigator = new ClNavigatorSS ;

	SetNavigator ( ( ClNavigatorFirewall * ) m_poNavigator ) ;

//Guarda el parámetro del semáforo que se le pasa para liberarlo en el shutdown
	m_poCondition = ai_poCondition ;

	m_szConfigPath = new m4char_t [ strlen ( ai_szConfigPath ) + 1 ] ;

	strcpy ( m_szConfigPath ,ai_szConfigPath ) ;


//	m_poConfiguration
	

	if ( M4_ERROR == GetSizeFile ( m_szConfigPath, iLenOut ) )
	{
		M4_SRV_ERROR ( M4_SRV_SS_OBL_ERROR, "It was not possible to load properties file %s" , m_szConfigPath) ;
//		SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "It was not possible to open configuration file %s" , m_szConfigPath ) ;

		cout << "Error openning the startup obl: '" << m_szConfigPath << "'\n" << flush;

		return M4_ERROR ;
	}


	//initialize Obl
	poConfiguration = new ClOblConfiguration ;

	//inicializa la obl en memoria
	iRet = poConfiguration -> Init ( m_szConfigPath  , iLenOut + 10 , iLenOut + 10) ;

	if ( iRet == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_SS_OBL_ERROR, "It was not possible to load properties file %s" , m_szConfigPath) ;
//		SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "It was not possible to load configuration file %s" , m_szConfigPath ) ;

		cout << "Error at the obl " << m_szConfigPath << " startup" << flush;

		m4sleep (3 ) ;

		return M4_ERROR;
	}

	M4AOblGetHandBlq ( 	poConfiguration -> GetHandle ( ) , "" , &hBlockHandle ) ;

	poPConfiguration = new ClPropertiesConfiguration ( poConfiguration , hBlockHandle , ai_poCryptStore ) ;

	SetConfiguration ( poPConfiguration );

	//Puerto base del servidor, todos los puertos son correlativos a este
	if ( m_iBasePort = ai_iBasePort )
	{
		m4char_t szPort [10] ;
		sprintf ( szPort , "%d" , m_iBasePort ) ;
//		poPConfiguration -> SetStringValue (""  , BASE_PORT , szPort , M4_FALSE ) ;
		poPConfiguration -> SetNumericValue ( "" , BASE_PORT , m_iBasePort ) ;
	}

	//Set de la configuración en las variables de entorno
	poPConfiguration -> SetStringValue ( "" , SERVER_ARCHITECTURE , M4_SERVER_ENV_VAR_ARCH ) ;

	if ( ai_szProcessName )
		poPConfiguration -> SetStringValue ("" , PROCESS_NAME , ai_szProcessName ) ;

	if ( ai_szConfigName )
		poPConfiguration -> SetStringValue ("" , SERVER_NAME ,  ai_szConfigName ) ;

	if ( poPConfiguration )
		poPConfiguration -> SetStringValue ("" , STATE ,  "ACTIVE" ) ;

	if ( ai_uControlRetryTime ) 
		poPConfiguration -> SetNumericValue ("" , M4DISPATCHER_CONTROL_RETRY_TIME , ai_uControlRetryTime /2 ) ;

	if ( ai_iDispatcherPort) 
		poPConfiguration -> SetNumericValue ("" , M4DISPATCHER_ADMIN_PORT , ai_iDispatcherPort) ;

	if ( ai_szDispatcherHost ) 
		poPConfiguration -> SetStringValue ("" , M4DISPATCHER_HOST , ai_szDispatcherHost ) ;

	m4size_t iTempSize;
	if( M4_ERROR == GetConfigStringValue ( SERVER_NAME , szServerName , OBL_PROPVALUE_MAX_LEN , iTempSize ) )
	{
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", SERVER_NAME << GetName ( ) );

		return M4_ERROR ;
	}
	ilen=iTempSize;

	ClIdGenerator::SetInstanceName(szServerName) ;

	// get appServerVersion.
	// this function is in m4serverversion.cpp file
	void GetM4AppServerVersion(M4ClString& ao_appServerVersion);
	M4ClString appServerVersion;
	GetM4AppServerVersion(appServerVersion);
	
	if (appServerVersion == M4ClString(""))
	{
		iRet =  poPConfiguration -> SetStringValue ( "" , VERSION,  "Unknown" ) ;
	}
	else
	{
		iRet =  poPConfiguration -> SetStringValue ( "" , VERSION,  (m4pchar_t)appServerVersion.c_str()) ;
	}
	
	if(iRet==M4_ERROR)
	{
		m4Trace(cerr<<"Error setting version"<<endl);	
		SETCODE( M4_SRV_SS_OBL_ERROR , ERRORLOG , "Error setting version") ;
	}



	if ( M4_ERROR == Initialize ( ) )
	{
		return M4_ERROR ;
	}



	
	//Build subsystems tree
	if ( Init (GetConfiguration() ) == M4_ERROR )
	{

		return M4_ERROR ;
	}

	poLogConfiguration = new ClPropertiesConfiguration ( PROPTRACESSCONFIGURATION ) ;


	poLogSubsystem = m_poSSFactory -> GenerateSubsystem ( M4_TRACE_SUBSYSTEM , poLogConfiguration, m_poTables) ;

	if (! poLogSubsystem )
	{
		cout << "ERROR!!" << endl ;
		return M4_ERROR ;
	}


	poLogSubsystem -> SetConfiguration ( poLogConfiguration ) ;

	poLogSubsystem -> SetId ( ClIdGenerator::GetNewId ()  , GetPath( ) ) ;

	poLogSubsystem -> SetName ( "TRACESS" ) ;

	AddSubsystem ( poLogSubsystem ) ;

	poLogSubsystem -> Initialize ( ) ;

	if ( Start( ) == M4_ERROR )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == ClControlPipesMaker::Close ( ) )
	{
		return M4_ERROR ;
	}

	//End Action Registration
	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance ( ) ;
	iRet = poInvoker -> FinishRegistration () ;

	//SetConfiguration ( poConfiguration ) ;
	SETCODEF ( M4_SERVER_READY , DEBUGINFOLOG , "SERVER READY" ) ;

	iRet = M4PublishEvent("65544","") ;

	Notify (SERVER_READY);


	cout << endl << "-----------------------------------------------------------" << endl ;

#ifdef _WINDOWS
#ifdef _WIN64
	cout << "Windows 64 bits" ;
#else
	cout << "Windows 32 bits" ;
#endif
#else
#ifdef _AIX
	cout << "Aix" ;
#else
	cout << "Solaris" ;
#endif
#endif

	if( M4IsUnicode() == 1 )
	{
		cout << " Unicode Server ready" << endl ;
	}
	else
	{
		cout << " ANSI Server ready" << endl ;
	}

	m_bServerStarted = M4_TRUE ;

	return M4_SUCCESS ;
	
  //## end ClSystem::StartUp%895825799.body
}

m4return_t ClSystem::LogOn ()
{
  //## begin ClSystem::LogOn%896170609.body preserve=yes
	ClSubsystem * poSubsystem = NULL ;
	ClUSSessionInterface * poServerSession = NULL ;
	ClConfiguration * poConfiguration =NULL ;
	ClSSInterfaz * poUserSS = NULL ;
	ClSSInterfaz * poSessionSubsystem = NULL ;
//	ClNavigatorSS oNavigator ;
	ClLogonInterface * poLogonInterface = NULL ;
	m4uint64_t lIdSession ;
	double iSecurity = 1;
	m4return_t iRet ;
	m4char_t car = 0 ;
	m4uint32_t iCar = 0 ;
	static m4bool_t bLoged = M4_FALSE ;

	if ( bLoged )
		return M4_SUCCESS ;

	cout << "Logging in ..." <<endl ;

		if ( ! ( poUserSS = (ClSSInterfaz*) m_poNavigator ->GetSubsystemByType ( M4_USERS_SUBSYSTEM ) ) )
		{
			SETCODEF ( M4_SRV_SS_UUSS_ERROR , ERRORLOG , "Cannot find subsystem \"%s\"" , USERS ) ;

			cout << "ERROR!!" << endl ;
			return M4_ERROR;
		}

//		if ( ! ( poLogonInterface = ( ClLogonInterface * ) oNavigator.GetSubsystemElement ( USERS , ID_UUSS_LOGON ) ) )
		if ( ! ( poLogonInterface = ( ClLogonInterface * ) poUserSS->GetElement ( ID_UUSS_LOGON ) ) )
		{
			SETCODEF ( M4_SRV_SS_UUSS_ERROR , ERRORLOG , "Cannot find Element \"%s\" in subsystem \"%s\"" , ID_UUSS_LOGON  , USERS ) ;

			cout << "ERROR!!" << endl ;
			return M4_ERROR;
		}

		// Auditoría de sesión
		// Se borran las sesiones auditadas al arrancar el server
		// No se comprueba error
		iRet = poLogonInterface->CleanAuditory() ;

		iRet = poLogonInterface->OpenSession (m_szUserName, m_szPassword, poServerSession, M4_TRUE, NULL) ;

		poUserSS -> Activate ( ) ;

		if ( iRet != M4_SUCCESS )
		{
			SETCODEF ( M4_SRV_SS_UUSS_ERROR , ERRORLOG , "M4 APPLICATION SERVER: Server Logon Error. Error Starting Application Server" ) ;

			cout << "ERROR!!" << endl ;
			return M4_ERROR ;
		}


	lIdSession = ClIdGenerator::GetNewId ( ) ;

	poConfiguration = new ClPropertiesConfiguration ( PROPSERVERSESSIONCONFIGURATION ) ;

	poConfiguration -> AddStringProperty ( "" , "STRINGID" , "Server Session" ) ;


	poSessionSubsystem = m_poSSFactory -> GenerateSubsystem ( M4_SESSION_SUBSYSTEM , poConfiguration , m_poTables) ;

	if (! poSessionSubsystem )
	{
		cout << "ERROR!!" << endl ;
		return M4_ERROR ;
	}


	poSessionSubsystem -> SetConfiguration ( poConfiguration ) ;

	poSessionSubsystem -> SetId ( lIdSession , GetPath( ) ) ;

	poSessionSubsystem -> SetName ( SERVER_SESSION ) ;

	if ( M4_ERROR == ((ClSSInterfaz*)poSessionSubsystem ) -> SetElement ( "SESSION" , poServerSession ) )
	{
		cout << "ERROR!!" << endl ;
		return M4_ERROR ;
	}

	AddSubsystem ( poSessionSubsystem ) ;

	//poSessionSubsystem -> Initialize ( ) ;

	bLoged = M4_TRUE ;


	return M4_SUCCESS ;

  //## end ClSystem::LogOn%896170609.body
}

m4return_t ClSystem::GetCommandParamsRoot (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClSystem::GetCommandParamsRoot%901005786.body preserve=yes
	m4return_t iRet;
/*
	ClSSInterfaz *poSubsystem;
	poSubsystem = FindSubsystem (ai_szPath);

	if (!poSubsystem)
	{
		SETCODEF(M4_SRV_SS_TREE_ERROR, ERRORLOG, "'GetCommandParams' call failed searching for path %s", ai_szPath);
		return M4_ERROR ;
	}
*/
	iRet = ClSSImplementation::GetCommandParams(ai_szPath , ai_cCommand, aio_oParamList);

	return iRet ;
  //## end ClSystem::GetCommandParamsRoot%901005786.body
}

m4return_t ClSystem::ExecuteCommand (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClSystem::ExecuteCommand%901005793.body preserve=yes
/*	ClSSInterfaz *poSubsystem;

	poSubsystem = FindSubsystem(ai_szPath);

	if (!poSubsystem)
	{
		SETCODEF (M4_SRV_SS_TREE_ERROR, ERRORLOG, "'ExecuteCommand' call failed searching for path %s", ai_szPath);
		return M4_ERROR ;
	}
*/
	return ClSSImplementation::ExecuteCommand(ai_szPath , ai_cCommand, ai_oParamList, ao_oResultList);
  //## end ClSystem::ExecuteCommand%901005793.body
}

m4return_t ClSystem::PersistConfiguration (m4pchar_t ai_szProperty, m4pchar_t ai_szValue, UINT ai_hBlock)
{
  //## begin ClSystem::PersistConfiguration%901740423.body preserve=yes
	m4uint32_t iLenBuf ;
	m4uint_t  iLenOut ;
	FILE * fOblFile = NULL ;
	m4pchar_t szTextOut = NULL ;
	ClOblConfiguration * poConfiguration = NULL ;
	m4return_t iRet ;
	m4int_t hOblFile ;


	//obtener 
	if ( M4_ERROR == GetSizeFile ( m_szConfigPath , iLenBuf ) )
	{
		SETCODEF ( M4_SRV_STARTUPOBL_ERROR, ERRORLOG , "Error opening obl file : %s for persisting" , m_szConfigPath );

		return M4_ERROR ;
	}

	poConfiguration = new ClOblConfiguration ;

	//inicializa la obl en memoria
	iRet = poConfiguration -> Init ( m_szConfigPath  , iLenBuf+ 10 , iLenBuf + 10) ;

	if (iRet == M4_SUCCESS )
	{
		if ( ai_hBlock )
		{
			iRet = poConfiguration -> SetStringValue (ai_hBlock , ai_szProperty, ai_szValue)  ;
		}
		else
		{
			iRet = poConfiguration -> SetStringValue ("", ai_szProperty, ai_szValue)  ;
		}


		if ( iRet == M4_ERROR )
		{
			if ( ai_hBlock )
			{
				iRet = poConfiguration -> AddStringProperty ( ai_hBlock , ai_szProperty, ai_szValue) ;
			}
			else
			{
				iRet = poConfiguration -> AddStringProperty ( "", ai_szProperty, ai_szValue) ;
			}

		}
	}

	if ( M4_ERROR == iRet )
	{
		return M4_ERROR ;
	}

	iLenBuf += 1024 ;
	 
	szTextOut = new m4char_t [ iLenBuf ] ;

      if ( poConfiguration -> WriteOut (szTextOut, iLenBuf, iLenOut) ==M4_ERROR )
	  {
		  SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "Error getting configuration on buffer" ) ;

		  return M4_ERROR ;
	  }


	// UNICODE FILE
	int iLength = -1 ;
	char* pcFileName = M4CppToANSI( m_szConfigPath, iLength ) ;
	hOblFile = open ( pcFileName, O_RDWR, S_IREAD | S_IWRITE ) ;
	delete [] pcFileName ;

	if ( hOblFile == M4_ERROR )
	{
		SETCODEF ( M4_SRV_STARTUPOBL_ERROR, ERRORLOG , "Error opening obl file : %s for persisting" , m_szConfigPath );

		return M4_ERROR ;
	}

	write (hOblFile , szTextOut , strlen (szTextOut ));
	close ( hOblFile ) ;

	return M4_SUCCESS ;


  //## end ClSystem::PersistConfiguration%901740423.body
}

m4return_t ClSystem::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClSystem::ExecuteCommand%910862269.body preserve=yes

	switch ( ai_cCommand )
	{

	case SYNCHRONIZE_CALL :
		Synchronize () ;

		break ;

	case GARBAGE_COLLECTION :
		GarbageCollection () ;
		break ;

	case ESTIMATE_LOAD :
		EstimateLoad ( ao_oResultList ) ;
		break ;

	default :
		M4_SRV_ERROR ( M4_SRV_COMMAND_ID_ERROR, "Error Executing command in '%0:s' Subsystem. Unknown command id: %1:d" , GetName() << ai_cCommand ) ;
		return M4_ERROR ;

	}

	return M4_SUCCESS ;
  //## end ClSystem::ExecuteCommand%910862269.body
}

m4return_t ClSystem::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oCommnadList)
{
  //## begin ClSystem::GetCommandParams%910862270.body preserve=yes
	ClAdminCommandTag * newParam ;

	switch ( ai_cCommand )
	{
	case ESTIMATE_LOAD :

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "CPU_USAGE", admindouble, "Percent system CPU usage" , M4_TRUE);

		if (newParam == NULL) 
			return M4_ERROR;
		aio_oCommnadList.push_back(newParam);

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(1, "CONSUMED_SYSTEM_MEMORY", admindouble, "Percent system memory usage" , M4_TRUE);

		if (newParam == NULL) 
			return M4_ERROR;
		aio_oCommnadList.push_back(newParam);

		// bg 165010
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(2, "M4SERVER_MEMORY_PERCENT_COUNTABILITY", adminLong, "M4Server memory usage" , M4_TRUE);

		if (newParam == NULL) 
			return M4_ERROR;
		aio_oCommnadList.push_back(newParam);

		break ;

	default :
		return M4_ERROR ;

	}

	return M4_SUCCESS ;
  //## end ClSystem::GetCommandParams%910862270.body
}

m4pchar_t ClSystem::GetLiteral (m4pchar_t ai_tag)
{
  //## begin ClSystem::GetLiteral%916246420.body preserve=yes
	return m_poTables -> GetLiteral ( ai_tag ) ;
  //## end ClSystem::GetLiteral%916246420.body
}

m4return_t ClSystem::GetStringSpecificationValue (char *ai_szpath, char *ai_szProperty, char *ao_szValue, size_t ai_iLenBuffer, size_t &ao_iLenOut)
{
  //## begin ClSystem::GetStringSpecificationValue%916246421.body preserve=yes
	return m_poTables -> m_oBook -> GetStringValue ( ai_szpath, ai_szProperty, ao_szValue, ai_iLenBuffer, ao_iLenOut ) ;
  //## end ClSystem::GetStringSpecificationValue%916246421.body
}

m4objid_t ClSystem::GetId ()
{
  //## begin ClSystem::GetId%916246422.body preserve=yes
	return ClSSImplementation::GetId() ;
  //## end ClSystem::GetId%916246422.body
}

ClSSInterfaz * ClSystem::GenerateSubsystem (m4uint16_t ai_IType, ClConfiguration *ai_poConfig, ClTables *ai_poTables)
{
  //## begin ClSystem::GenerateSubsystem%919968179.body preserve=yes
		return m_poSSFactory -> GenerateSubsystem (ai_IType, ai_poConfig, ai_poTables) ;
  //## end ClSystem::GenerateSubsystem%919968179.body
}

void ClSystem::Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject)
{
  //## begin ClSystem::Update%919968182.body preserve=yes
	m4return_t iRet ;
	m4char_t szCritical [OBL_PROPVALUE_MAX_LEN ];
	static m4uint32_t iRedIterations = 0 ;
	static m4uint32_t iYellowIterations = 0 ;
	static m4uint32_t iGreenIterations = 0 ;
	m4size_t iLen ;
	m4bool_t bCritical = M4_FALSE ;


	switch ( ai_iEvent )
	{
	case LDB_SHUTDOWN :
		iRet = M4PublishEvent("65545","") ;
		Notify ( ai_iEvent ) ;
		Exit ( 1 ) ;
//ahora Joseco controla el retorno para que devuelva hacia arriba el error
//		m4sleep(1000);
		break;
	case SERVICE_SHUTDOWN :

		iRet = ( ( ClSSImplementation *) ai_oSubject ) -> GetConfigStringValue ( CRITICAL_SERVICE , szCritical , OBL_PROPVALUE_MAX_LEN , iLen ) ;

		if ( ! stricmp (szCritical , SS_STR_TRUE ) )
		{
			iRet = M4PublishEvent("65546","") ;
			Notify ( ai_iEvent ) ;
			Exit ( 2 ) ;
		}
		break ;
	case GREEN_MEMORY_LEVEL :
		iGreenIterations  ++ ;
		iYellowIterations = 0 ;
		iRedIterations = 0;

		if ( iGreenIterations  ==1 )
			iRet = M4PublishEvent("65536","Server memory level is NORMAL#") ;
		break;

	case YELLOW_MEMORY_LEVEL :
		iGreenIterations  = 0 ;
		iYellowIterations ++;
		iRedIterations = 0;

		if ( iYellowIterations == 1)
			iRet = M4PublishEvent("65536","Server memory level is ALERT#") ;
		break;
	case RED_MEMORY_LEVEL :
		iRedIterations ++ ;
		iYellowIterations =0;
		iGreenIterations  = 0 ;

		iRet = M4PublishEvent("65536","Server memory level is CRITICAL#") ;
		
		if ( iRedIterations > 3 )
		{
			iRet = M4PublishEvent("65547","") ;
			Notify ( MEMORY_ALERT ) ;
			Exit ( 0 ) ;
		}

		break;
	}
  //## end ClSystem::Update%919968182.body
}

m4return_t ClSystem::GetSubsystemsId (m4pchar_t ai_szPath, vector <m4objid_t > &ao_poTags)
{
  //## begin ClSystem::GetSubsystemsId%920285555.body preserve=yes
	m4return_t iRet ;
/*
	ClSSInterfaz * poSubsystem ;
	poSubsystem = FindSubsystem(ai_szPath);

	if ( ! poSubsystem )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "'GetSubsystemsId' call failed searching for path %s" , ai_szPath ) ;

		return M4_ERROR ;
	}
*/
	iRet = ClSSImplementation::GetSubsystemsId ( ai_szPath, ao_poTags ) ;
	return iRet ;
	
  //## end ClSystem::GetSubsystemsId%920285555.body
}

m4bool_t ClSystem::StopExecution ()
{
  //## begin ClSystem::StopExecution%929965060.body preserve=yes
	return m_bStopExecution ;
  //## end ClSystem::StopExecution%929965060.body
}

m4return_t ClSystem::Exit (m4uint32_t ai_lDeferedTime)
{
  //## begin ClSystem::Exit%929965061.body preserve=yes
	m_lDeferedTime = ai_lDeferedTime ;

	if (m_lDeferedTime ==3 )
		Notify ( ADMIN_SHUTDOWN );

	m_bStopExecution = M4_TRUE ;

	return m_poCondition -> Signal( ) ;
  //## end ClSystem::Exit%929965061.body
}

m4return_t ClSystem::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClSystem::GetCommandsId%933847946.body preserve=yes
	ClAdminTag *newCommand;

	ClSSImplementation::GetCommandsId ( aio_oCommandList ) ;

	newCommand = ClAdminTag::GetNewAdminTag(SYNCHRONIZE_CALL, "SYNCHRONIZE", M4_FALSE);

	if (newCommand == NULL) return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(GARBAGE_COLLECTION, "GARBAGE_COLLECTION", M4_FALSE);

	if (newCommand == NULL) return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(ESTIMATE_LOAD, "ESTIMATE_LOAD", M4_TRUE);

	if (newCommand == NULL) return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	return M4_SUCCESS;

  //## end ClSystem::GetCommandsId%933847946.body
}

void ClSystem::GarbageCollection ()
{
  //## begin ClSystem::GarbageCollection%936257855.body preserve=yes

    // este metodo se ejecuta por la línea
    //      SERVER = "GARBAGE_COLLECTION"
    // del startup.obl.

    // synchronize.
    ((ClSSFactory*)m_poSSFactory) -> Synchronize ();

    // purge.
    ClRequest::s_oGarbageCollector.Purge();

  //## end ClSystem::GarbageCollection%936257855.body
}

void ClSystem::SetType (m4uint16_t ai_iType)
{
  //## begin ClSystem::SetType%936613843.body preserve=yes
  //## end ClSystem::SetType%936613843.body
}

m4return_t ClSystem::CheckSystemMemory (m4uint32_t ai_lPrevRefreshRatio, m4uint32_t ao_lPosRefreshRatio)
{
  //## begin ClSystem::CheckSystemMemory%937580167.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSystem::CheckSystemMemory%937580167.body
}

m4return_t ClSystem::EstimateLoad (ClAdminTagList &ao_oResultList)
{
  //## begin ClSystem::EstimateLoad%962873113.body preserve=yes
	m4size_t iLen ;
	m4char_t szBuffer [ OBL_PROPVALUE_MAX_LEN ] ;
	ClAdminCommandTag *newParam;


	if ( M4_SUCCESS == GetConfigStringValue ( "CPU_USAGE" , szBuffer , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "CPU_USAGE", admindouble, szBuffer );

		if (newParam == NULL) 
			return M4_ERROR;
		ao_oResultList.push_back(newParam);
	}

	if ( M4_SUCCESS == GetConfigStringValue ( "CONSUMED_SYSTEM_MEMORY" , szBuffer , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(1, "CONSUMED_SYSTEM_MEMORY", admindouble, szBuffer );

		if (newParam == NULL) 
			return M4_ERROR;
		ao_oResultList.push_back(newParam);
	}

	if ( M4_SUCCESS == GetConfigStringValue ( "M4SERVER_MEMORY_PERCENT_COUNTABILITY" , szBuffer , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(2, "M4SERVER_MEMORY_PERCENT_COUNTABILITY", adminLong, szBuffer );

		if (newParam == NULL) 
			return M4_ERROR;
		ao_oResultList.push_back(newParam);
	}

	
	// eh 178555
	if ( M4_SUCCESS == GetConfigStringValue ( "M4SERVER_MEMORY_COUNTABILITY" , szBuffer , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(3, "M4SERVER_MEMORY_COUNTABILITY", admindouble, szBuffer );

		if (newParam == NULL) 
			return M4_ERROR;
		ao_oResultList.push_back(newParam);
	}

	if ( M4_SUCCESS == GetConfigStringValue ( "MAX_PROC_SIZE" , szBuffer , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(4, "MAX_PROC_SIZE", admindouble, szBuffer );

		if (newParam == NULL) 
			return M4_ERROR;
		ao_oResultList.push_back(newParam);
	}

	if ( M4_SUCCESS == GetConfigStringValue ( "FREE_RAM_MEM" , szBuffer , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(5, "FREE_RAM_MEM", admindouble, szBuffer );

		if (newParam == NULL) 
			return M4_ERROR;
		ao_oResultList.push_back(newParam);
	}

return M4_SUCCESS ;

  //## end ClSystem::EstimateLoad%962873113.body
}

m4return_t ClSystem::GetCommandsIdRoot (m4pchar_t ai_szPath, ClAdminTagList &aio_oCommandList)
{
  //## begin ClSystem::GetCommandsIdRoot%994763185.body preserve=yes
	m4return_t iRet;
	ClSSInterfaz *poSubsystem;
	m4char_t szType [OBL_PROPVALUE_MAX_LEN];

	m4pchar_t pcRef = strrchr(ai_szPath,'.');
	if (pcRef == NULL)
		pcRef = ai_szPath;
	else
		pcRef++;

	// we have to change it to I64 support
	m4uint32_t iId = 0;
	iId = atol(pcRef);

	//-----------------------------------------------------------------
	// get subsystem
	//-----------------------------------------------------------------

	// poSubsystem = FindSubsystemByName(ai_szPath);
	poSubsystem = FindSubsystemById(iId);
	if (!poSubsystem)
	{
		SETCODEF (M4_SRV_SS_TREE_ERROR, WARNINGLOG, "'GetSubsystemsId' call failed searching for path %s", ai_szPath);
		return M4_ERROR;
	}

	if (M4_SUCCESS== poSubsystem -> GetConfiguration () ->GetType (szType ,OBL_PROPVALUE_MAX_LEN)  )
	{
		strcpy ( aio_oCommandList.m_subsystemPath , szType) ;
	}

	//-----------------------------------------------------------------
	// exec GetCommandsId for the subsystem
	//-----------------------------------------------------------------
	iRet = ClSSImplementation::GetCommandsId(ai_szPath , aio_oCommandList);

	return iRet;		
  //## end ClSystem::GetCommandsIdRoot%994763185.body
}

m4return_t ClSystem::GetSubsystemAttribValue (m4pchar_t ai_szPath, m4uint16_t ai_iAttribId, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSystem::GetSubsystemAttribValue%994933976.body preserve=yes
	return ClSSImplementation::GetSubsystemAttribValue (ai_szPath, ai_iAttribId, ao_poDataStorage) ;
  //## end ClSystem::GetSubsystemAttribValue%994933976.body
}

m4return_t ClSystem::GetSubsystemAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSystem::GetSubsystemAttribsId%994933977.body preserve=yes
	return ClSSImplementation::GetSubsystemAttribsId (ai_szPath, ao_poDataStorage) ;
  //## end ClSystem::GetSubsystemAttribsId%994933977.body
}

m4return_t ClSystem::GetStatisticAttribValue (m4pchar_t ai_szPath, m4uint32_t ai_iStatId, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSystem::GetStatisticAttribValue%994933978.body preserve=yes
	return ClSSImplementation::GetStatisticAttribValue (ai_szPath, ai_iStatId, ao_poDataStorage) ;
  //## end ClSystem::GetStatisticAttribValue%994933978.body
}

m4return_t ClSystem::GetStatisticAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSystem::GetStatisticAttribsId%994933979.body preserve=yes
	return ClSSImplementation::GetStatisticAttribsId (ai_szPath, ao_poDataStorage) ;
  //## end ClSystem::GetStatisticAttribsId%994933979.body
}

// Additional Declarations
  //## begin ClSystem%348D6FE90173.declarations preserve=yes

	  ClSystemStats::ClSystemStats () 
	  {
		  m_iSystemMemoryUsage = 0;
		  m_iSystemCPUUsage = 0;
	  	  m_iServerMemory = 0 ;
		  m_uiNumberOfStats = TOTAL_NUM ;
	  };

      m4return_t ClSystemStats::Reset ()
	  {
		  return M4_ERROR ;
	  }

      m4return_t ClSystemStats::ResetStat (ClStatHandle &ai_hStatId)
	  {
			m4uint32_t lowIndex ;
			lowIndex = ai_hStatId.GetLowID();

			switch ( lowIndex )
			{
			case SYSTEM_MEMORY_USAGE :
				m_iSystemMemoryUsage = 0;
				break ;
			case SYSTEM_CPU_USAGE :
				m_iSystemCPUUsage = 0;
				break ;
			case SERVER_MEMORY :
		  		m_iServerMemory = 0 ;
				break ;
			default  :
				return M4_ERROR ;
			}

			return M4_SUCCESS ;
	  }

      m4return_t ClSystemStats::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_oValue)
	  {
		  m4double_t value ;
		  m4uint32_t lowIndex ;
		  

		ao_oValue.SetSize ( 8 ) ;
		m4pchar_t	buffer = ao_oValue.GetBuffer();
		  
	lowIndex = ai_hStatId.GetLowID();
		  
		  
		  switch ( lowIndex )
		  {
			  
		  case SYSTEM_MEMORY_USAGE :
			  GetSystemMemoryUsage ( value );

			  sprintf ( buffer, "%3.1f" , value )  ;
			  
			  break ;
		  case SYSTEM_CPU_USAGE :
			  GetSystemCPUUsage (value) ;
			  sprintf ( buffer, "%3.0f" , value)  ;
			  break ;
		  case SERVER_MEMORY :
			  GetServerMemory ( value ) ;
			  sprintf ( buffer, "%4.2f" , value)  ;
			  break;
		  default :
			  return M4_ERROR ;
			  break;
		  }

		  return M4_SUCCESS ;
	  }

      m4return_t ClSystemStats::SetStat (ClStatHandle &ai_hStatId, const m4pchar_t ao_pszValue)
	  {
		  return M4_SUCCESS ;
	  }

//      m4return_t ResetStat (ClStatHandle &ai_hStatId);

      m4void_t ClSystemStats::SetSystemMemoryUsage (m4double_t ai_NewValue)
	  {
		  m_iSystemMemoryUsage =ai_NewValue ;
	  }

      m4void_t ClSystemStats::SetSystemCPUUsage (m4double_t ai_NewValue)
	  {
		  m_iSystemCPUUsage = ai_NewValue ;
	  }
      
	  m4void_t ClSystemStats::SetServerMemory (m4double_t ai_NewValue)
	  {
		  	  m_iServerMemory = ai_NewValue ;
	  }

	  m4void_t ClSystemStats::SetPercentServerMemory (m4double_t ai_NewValue)
	  {
		  	  m_iPercentServerMemory = ai_NewValue ;
	  }

      m4void_t ClSystemStats::GetSystemMemoryUsage (m4double_t &ao_NewValue)
	  {
		  ao_NewValue = m_iSystemMemoryUsage ;
	  }

      m4void_t ClSystemStats::GetSystemCPUUsage (m4double_t &ao_NewValue)
	  {
		  ao_NewValue = m_iSystemCPUUsage ;
	  }
      
	  m4void_t ClSystemStats::GetServerMemory (m4double_t &ao_NewValue)
	  {
		  ao_NewValue = m_iServerMemory ;
	  }

	  m4void_t ClSystemStats::GetPercentServerMemory (m4double_t &ao_NewValue)
	  {
		  ao_NewValue = m_iPercentServerMemory ;
	  }


  //## end ClSystem%348D6FE90173.declarations

// Class ClSystemVisitor 


//## Other Operations (implementation)
void ClSystemVisitor::VisitSystem (ClSystem &ai_oSsystem)
{
  //## begin ClSystemVisitor::VisitSystem%884863683.body preserve=yes
  //## end ClSystemVisitor::VisitSystem%884863683.body
}

// Additional Declarations
  //## begin ClSystemVisitor%34BDFB4000F8.declarations preserve=yes
  //## end ClSystemVisitor%34BDFB4000F8.declarations

//## begin module%3492936201C4.epilog preserve=yes
//## end module%3492936201C4.epilog
