
#include <tracess.hpp>
#include <subsystemids.hpp>
#include <clpconfiguration.hpp>
#include <subsystemids.hpp>
#include <clarraylibres.hpp>
#include <m4log.hpp>
#include <logmanag.hpp>
#include <perslog.hpp>
#include <request.hpp>
#include <m4ldb.hpp>
#include <transportlayer.hpp>
#include "clfactm4objservice.hpp"
#include "m4ldb_notif.hpp"
#include "m4regimpl.hpp"
#include "m4jconfiguration.hpp"
#include "clm4objservice.hpp"


#define LDB_DEBUG_LEVEL					"LDB_DEBUG_LEVEL"
#define LDB_DEBUG_DETAIL_LEVEL			"LDB_DEBUG_DETAIL_LEVEL"

// properties
#define TRANSPORT_ENABLE_TRANSPORT_DUMP		"ENABLE_TRANSPORT_DUMP"
#define TRANSPORT_DUMP_STATUS				"TRANSPORT_DUMP_STATUS"
#define TRANSPORT_DUMP_DIRECTORY			"TRANSPORT_DUMP_DIRECTORY"
#define TRANSPORT_DUMP_PREFIX				"TRANSPORT_DUMP_PREFIX"
#define TRANSPORT_DUMP_MAX_FILE_SIZE		"TRANSPORT_DUMP_MAX_FILE_SIZE"
#define TRANSPORT_DUMP_MAX_FILES			"TRANSPORT_DUMP_MAX_FILES"

ClTraceSS::ClTraceSS(ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator )
:ClSSImplementation (ai_poConfiguratoin, ai_poTables, ai_poNavigator )

{
}
      
m4return_t ClTraceSS::Initialize ()
{
	ClSSInterfaz * poLogSubsystem ;
	m4uint32_t iSSType , iConf = 0;
	m4pchar_t configs[2] = { PROPLOGSSCONFIGURATION, PROPLDBINSPSSCONFIGURATION } ;
	m4pchar_t names [2] = { "LOG" , "VMLOG" } ;
	ClPropertiesConfiguration * poLogConfiguration ;

	for ( iSSType = M4_LOG_SUBSYSTEM ; iSSType <= M4_LDBINSPECTOR_SUBSYSTEM ; iSSType ++ )
	{
		poLogConfiguration = new ClPropertiesConfiguration ( configs[iConf] ) ;
		
		poLogSubsystem = GenerateSubsystem ( iSSType , poLogConfiguration, m_poTables) ;
		
		if (! poLogSubsystem )
		{
			cout << "ERROR!!" << endl ;
			return M4_ERROR ;
		}
		
		
		poLogSubsystem -> SetConfiguration ( poLogConfiguration ) ;
		
		poLogSubsystem -> SetId ( ClIdGenerator::GetNewId ()  , GetPath( ) ) ;
		
		poLogSubsystem -> SetName ( names [iConf] ) ;
		
		AddSubsystem ( poLogSubsystem ) ;

		poLogSubsystem  -> Initialize () ;

		iConf++ ;
	}


	return M4_SUCCESS ;
}


ClSSInterfaz * ClTraceSS::GenerateSubsystem ( m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
	ClSSInterfaz * poInterfaz ;
	ClSSImplementation * poSubsystem ;

	switch ( ai_lType )
	{
	case M4_LOG_SUBSYSTEM :


		poSubsystem = new ClLogSS ( ai_poConfiguration , ai_poTables , m_poNavigator ) ;
		poInterfaz = new ClSSInterfaz ( poSubsystem  );
		poSubsystem -> SetInterfaz ( poInterfaz ) ;

		break ;

	case M4_LDBINSPECTOR_SUBSYSTEM	:
		
		poSubsystem = new ClLDBInspectorSS ( ai_poConfiguration , ai_poTables , m_poNavigator ) ;
		poInterfaz = new ClSSInterfaz ( poSubsystem  );
		poSubsystem -> SetInterfaz ( poInterfaz ) ;

		break ;

	default:
		return NULL ;
	}
	
	return poInterfaz ;
}


	ClLogSS::ClLogSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator ) 
		:ClSSImplementation ( ai_poConfiguratoin, ai_poTables, ai_poNavigator )
	{
	}

	m4return_t ClLogSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
	{
		ClLogManager * poLogManager ;

		poLogManager = GETLOGMANAGER() ;
		if (! poLogManager  )
			return M4_ERROR ;

		switch (ai_cCommand)
		{
		case ENABLE_ERRORS :
			poLogManager -> ActiveError () ;
			break ;
		case DISABLE_ERRORS :
			poLogManager -> DefuseError () ;
			break;
		case ENABLE_WARNINGS :
			poLogManager -> ActiveWarning () ;
			break ;
		case DISABLE_WARNINGS :
			poLogManager  -> DefuseWarning () ;
			break ;
		case ENABLE_DEBUG_LOG :
			poLogManager -> ActiveDebugInfo () ;
			break ;
		case DISABLE_DEBUG_LOG :
			poLogManager -> DefuseDebugInfo () ;
			break ;
			
		default :
			return M4_ERROR ;

		}

		return M4_SUCCESS ;
	}



	m4return_t ClLogSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
	{
		return M4_ERROR ;
	}


	m4return_t ClLogSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
	{
		eStatus eState ;
		ClLogManager * poLogManager = GETLOGMANAGER() ;
	
		
		GetState(eState) ;
		if ( SS_NOT_ACCESIBLE == eState )
			return M4_ERROR ;
		
		ClAdminTag *newCommand;
		
		//Habilitar o deshabilitar Errores
		if ( poLogManager -> Error () )
		{
			newCommand = ClAdminTag::GetNewAdminTag(DISABLE_ERRORS , "DISABLE_ERRORS", M4_FALSE);
		}
		else
		{
			newCommand = ClAdminTag::GetNewAdminTag(ENABLE_ERRORS , "ENABLE_ERRORS", M4_FALSE);
		}
		if (newCommand == NULL) 
			return M4_ERROR ;
		aio_oCommandList.push_back(newCommand);
	
		//Habilitar o deshabilitar Warnings
		if ( poLogManager -> Warning () )
		{
			newCommand = ClAdminTag::GetNewAdminTag(DISABLE_WARNINGS, "DISABLE_WARNINGS", M4_FALSE);
		}
		else
		{
			newCommand = ClAdminTag::GetNewAdminTag(ENABLE_WARNINGS , "ENABLE_WARNINGS", M4_FALSE);
		}
		if (newCommand == NULL) 
			return M4_ERROR ;
		aio_oCommandList.push_back(newCommand);
	
		//Habilitar o deshabilitar Errores
		if ( poLogManager -> DebugInfo () )
		{
			newCommand = ClAdminTag::GetNewAdminTag(DISABLE_DEBUG_LOG , "DISABLE_DEBUG_LOG", M4_FALSE );
		}
		else
		{
			newCommand = ClAdminTag::GetNewAdminTag(ENABLE_DEBUG_LOG , "ENABLE_DEBUG_LOG", M4_FALSE);
		}
		if (newCommand == NULL) 
			return M4_ERROR ;
		aio_oCommandList.push_back(newCommand);
	

		return  M4_SUCCESS;
	}



    m4return_t ClLogSS::Initialize ()
	{
		return M4_SUCCESS ;
	}




	ClLDBInspectorSS::ClLDBInspectorSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator ) 
		:ClSSImplementation ( ai_poConfiguratoin, ai_poTables, ai_poNavigator )
	{
	}

	m4return_t ClLDBInspectorSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
	{

//		ILdb *pLDB = ClFactM4ObjService::GetpLDB();
		m4char_t szId [10] ;
		m4return_t iRet = M4_SUCCESS;
		ClSSInterfaz * poSubsystem ;
		m4size_t iLen ;
		m4char_t szLevel [200] ;
		ClAdminTagList::iterator it;
		m4int_t iDebugLevel = 0;
		m4int_t iMaxDebugPeriod = 24;
		M4JavaConfiguration	*poConfiguration = NULL ;


	switch(ai_cCommand)
	{

	case SET_DEBUG_LEVEL:
	case SET_DEBUG_DETAIL_LEVEL:

		poSubsystem = (ClSSInterfaz *) m_poNavigator -> GetSubsystemByType ( M4_LDB_SUBSYSTEM ) ;

		sprintf ( szId , "%d" , (m4uint32_t) poSubsystem -> GetId () ) ;
		iRet = poSubsystem -> ExecuteCommand ( szId , ai_cCommand, ai_oParamList, ao_oResultList ) ;

		poSubsystem -> GetConfigStringValue	( LDB_DEBUG_LEVEL , szLevel , 200 , iLen );

		SetConfigStringProperty	( LDB_DEBUG_LEVEL , szLevel );

		poSubsystem -> GetConfigStringValue	( LDB_DEBUG_DETAIL_LEVEL , szLevel , 200 , iLen );
		
		SetConfigStringProperty	( LDB_DEBUG_DETAIL_LEVEL , szLevel );

		break ;

	case VM_DEBUG_ENABLED:

		// gets the parameter
		it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}
		
		// converts the value
		iDebugLevel = atol((*it)->m_tagValue);

		/* Bug 0108688
		Hay que devolver error si no se puede asignar
		*/
		iRet = ClVMDebugEnv::getInstance()->setDebugLevel(iDebugLevel);
		SetConfigNumericProperty( "VM_DEBUG_LEVEL" , ClVMDebugEnv::getInstance()->getDebugLevel() );
		break;
			
	case SET_VM_MAX_DEBUG_PERIOD:

		// gets the parameter
		it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}
		
		// converts the value
		iMaxDebugPeriod = atol((*it)->m_tagValue);

		iRet = ClVMDebugEnv::getInstance()->setDebugMaxPeriod(iMaxDebugPeriod);
		SetConfigNumericProperty( "VM_MAX_DEBUG_PERIOD" , ClVMDebugEnv::getInstance()->getDebugMaxPeriod() );
		break;

	case ENABLE_JAVA_TRACE:

		poConfiguration = M4JavaConfiguration::Instance( NULL ) ;

		if( poConfiguration != NULL )
		{
			poConfiguration->SetTrace( M4_TRUE ) ;
			SetConfigStringProperty( "JAVA_TRACE", "Enabled" ) ;
		}

		break;

	case DISABLE_JAVA_TRACE:

		poConfiguration = M4JavaConfiguration::Instance( NULL ) ;

		if( poConfiguration != NULL )
		{
			poConfiguration->SetTrace( M4_FALSE ) ;
			SetConfigStringProperty( "JAVA_TRACE", "Disabled" ) ;
		}

		break;

	case SET_JAVA_TRACE:

		// gets the parameter
		it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}
		
		poConfiguration = M4JavaConfiguration::Instance( NULL ) ;

		if( poConfiguration != NULL )
		{
			if( strcmpi( (*it)->m_tagValue, "Enabled" ) == 0 )
			{
				poConfiguration->SetTrace( M4_TRUE ) ;
				SetConfigStringProperty( "JAVA_TRACE", "Enabled" ) ;
			}
			else if( strcmpi( (*it)->m_tagValue, "Disabled" ) == 0 )
			{
				poConfiguration->SetTrace( M4_FALSE ) ;
				SetConfigStringProperty( "JAVA_TRACE", "Disabled" ) ;
			}
		}

		break;

	case ENABLE_TRANSPORT_DUMP: 
	{
		// gets the base dir
		it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}
		m4pchar_t pcDumpDir = (m4pchar_t)((*it)->m_tagValue);

		// gets the prefix
		++it;
		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}
		m4pchar_t pcDumpPrefix = (m4pchar_t)((*it)->m_tagValue);


		// get the max size
		++it;
		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}
		m4pchar_t pcMaxDumpSize = (*it)->m_tagValue;
		m4uint32_t iDumpMaxSize = atoi(pcMaxDumpSize);

		// get the max number of files
		++it;
		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}
		m4pchar_t pcMaxFiles = (*it)->m_tagValue;
		m4uint32_t iMaxFiles = atoi(pcMaxFiles);

		// calls the transport method
		// Bug 0099587.
		if (ClTransportLayer::EnableDumpData(pcDumpDir,iDumpMaxSize,pcDumpPrefix,iMaxFiles) != M4_SUCCESS)
		{
			return M4_ERROR;
		}

		SetConfigStringProperty	( TRANSPORT_DUMP_STATUS , "True" );

		SetConfigStringProperty	( TRANSPORT_DUMP_DIRECTORY , pcDumpDir );

		SetConfigStringProperty	( TRANSPORT_DUMP_PREFIX , pcDumpPrefix );

		SetConfigStringProperty	( TRANSPORT_DUMP_MAX_FILE_SIZE , pcMaxDumpSize );

		SetConfigStringProperty	( TRANSPORT_DUMP_MAX_FILES , pcMaxFiles );

		iRet = M4_SUCCESS;

		break;
	}

	case DISABLE_TRANSPORT_DUMP: 
	{
		it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
		//Escenarios:
		//Caso1: Si no vienen argumentos es pq desde el monitor deseamos inhabilitar.
		//Caso11: Si viene un argumento y el argumento es False (0) es pq desde SNMP deseamos inhabilitar.
		//Caso2: Si viene un argumento y es True (1) es pq desde SNMP deseamos habilitar.
		//Si no hay argumentos es pq estamos ejecutando el comando desde el monitor.
		if (it == ai_oParamList.end()) {

			// calls the transport method
			ClTransportLayer::DisableDumpData();
	
			SetConfigStringProperty	( TRANSPORT_DUMP_STATUS , "False" );
	
			iRet = M4_SUCCESS;
		}
		else {
			//Si hay argumentos tenemos que comprobar si queremos habilitar o inhabilitar.
			m4pchar_t pcStatus = (m4pchar_t)((*it)->m_tagValue);
			if (pcStatus != NULL) {
				if (!stricmp(pcStatus,"1")) {
					//Habilitamos.
					//Para no modificar los valores que tienen establecidos pasamos unos argumentos especificos.					
					// Bug 0099587.
					if (ClTransportLayer::EnableDumpData( NULL, 0, NULL, -1 ) != M4_SUCCESS)
					{
						iRet = M4_ERROR;
					}
					else
					{
						SetConfigStringProperty	( TRANSPORT_DUMP_STATUS , "True" );					
						iRet = M4_SUCCESS;
					}
				}
				else {
					if (!stricmp(pcStatus,"0")) {
						//Inhabilitamos.
						ClTransportLayer::DisableDumpData();			
						SetConfigStringProperty	( TRANSPORT_DUMP_STATUS , "False" );
						iRet = M4_SUCCESS;
					}
					else {
						//Si no es ni 1 ni 0 damos error.
						iRet = M4_ERROR;
					}
				}				
			}
			else {
				//El argumento no puede ser nulo.
				iRet = M4_ERROR;
			}
		}

		break;
	}

	default:
		return M4_ERROR ;
	}

	return iRet;

	}

	m4return_t ClLDBInspectorSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
	{
	ClAdminCommandTag *newParam;
	m4pchar_t pcTransportDumpDir = NULL;
	m4pchar_t pcTransportDumpPrefix = NULL;
	m4uint32_t uiDumpMaxFileSize = 0;
	m4int32_t iDumpMaxFiles = 0;
	m4char_t pszAux[32];
	switch(ai_cCommand)
	{

	case SET_DEBUG_LEVEL:

		newParam= ClAdminCommandTag::GetNewAdminCommandTag(DEBUG_LEVEL,"DEBUG_LEVEL",adminLong,"0");
		if (newParam) aio_oParamList.push_back(newParam);
		
		break;

	case SET_DEBUG_DETAIL_LEVEL:

		newParam= ClAdminCommandTag::GetNewAdminCommandTag(DEBUG_DETAIL_LEVEL,"DEBUG_DETAIL_LEVEL",adminLong,"0");
		if (newParam) aio_oParamList.push_back(newParam);
		
		break;

    case VM_DEBUG_ENABLED:
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "VM_DEBUG_LEVEL", adminLong, "0");
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);
		break;

    case SET_VM_MAX_DEBUG_PERIOD:
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "VM_MAX_DEBUG_PERIOD", adminLong, "24");
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);
		break;

    case ENABLE_TRANSPORT_DUMP:
		// Bug 0099587. Obtener el valor de los parámetros del volcado de PDUs.
		ClTransportLayer::GetDumpStatus((m4pcchar_t&)pcTransportDumpDir, uiDumpMaxFileSize, (m4pcchar_t&)pcTransportDumpPrefix, iDumpMaxFiles);
		
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, TRANSPORT_DUMP_DIRECTORY, adminText, pcTransportDumpDir);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, TRANSPORT_DUMP_PREFIX, adminText, pcTransportDumpPrefix);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);
		
		sprintf(pszAux, "%u", uiDumpMaxFileSize);
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, TRANSPORT_DUMP_MAX_FILE_SIZE, adminLong, pszAux);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);

		sprintf(pszAux, "%d", iDumpMaxFiles);
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, TRANSPORT_DUMP_MAX_FILES, adminLong, pszAux);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);
break;

    case DISABLE_TRANSPORT_DUMP:
		break;

	default:
		return M4_ERROR ;
		break;
	}

	return  M4_SUCCESS;

	}

	m4return_t ClLDBInspectorSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
	{
	ClAdminTag *newCommand;
	M4JavaConfiguration	*poConfiguration = NULL ;


	newCommand = ClAdminTag::GetNewAdminTag(SET_DEBUG_LEVEL,"SET_DEBUG_LEVEL",M4_TRUE);
	if (newCommand) aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(SET_DEBUG_DETAIL_LEVEL,"SET_DEBUG_DETAIL_LEVEL",M4_TRUE);
	if (newCommand) aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(VM_DEBUG_ENABLED, "VM_DEBUG_ENABLED", M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(SET_VM_MAX_DEBUG_PERIOD,"SET_VM_MAX_DEBUG_PERIOD",M4_TRUE);
	if (newCommand) aio_oCommandList.push_back(newCommand);

	poConfiguration = M4JavaConfiguration::Instance( NULL );

	if( poConfiguration != NULL && poConfiguration->GetTrace() == M4_TRUE )
	{
		newCommand = ClAdminTag::GetNewAdminTag( DISABLE_JAVA_TRACE, "DISABLE_JAVA_TRACE", M4_TRUE ) ;
	}
	else
	{
		newCommand = ClAdminTag::GetNewAdminTag( ENABLE_JAVA_TRACE, "ENABLE_JAVA_TRACE", M4_TRUE ) ;
	}
	if (newCommand) aio_oCommandList.push_back(newCommand);

	// Bug 0099587. Verificar si el volcado de PDUs está activado o no.
	m4pcchar_t pcDummy;
	m4uint32_t uiDummy;
	m4int32_t iDummy;
	m4bool_t bEnableDumpData = ClTransportLayer::GetDumpStatus(pcDummy, uiDummy, pcDummy, iDummy);
	
	if (bEnableDumpData == M4_TRUE)
	{
		newCommand = ClAdminTag::GetNewAdminTag(DISABLE_TRANSPORT_DUMP,"DISABLE_TRANSPORT_DUMP",M4_TRUE);
		if (newCommand) aio_oCommandList.push_back(newCommand);
	}
	else
	{
		newCommand = ClAdminTag::GetNewAdminTag(ENABLE_TRANSPORT_DUMP,"ENABLE_TRANSPORT_DUMP",M4_TRUE);
		if (newCommand) aio_oCommandList.push_back(newCommand);
	}

	
	return M4_SUCCESS;
	}

    m4return_t ClLDBInspectorSS::Initialize ()
	{

		m4size_t iLen ;
		m4uint32_t iDebugLevel ;
		m4uint32_t iMaxDebugPeriod ;
		m4pchar_t pcJavaTrace = "Disabled";
		ClM4ObjService *poService = NULL ;
		ClVMBaseEnv	*poEnvironment = NULL ;
		M4JavaConfiguration	*poConfiguration = NULL ;


		// gets the properties from the comm subsystem
		m4char_t szBaseDir [200], szBasePrefix[200], szMode [20], szMaxSize[20];
		m4bool_t bDumpStatus = M4_FALSE;
		
		ClSSInterfaz * poSubsystem = (ClSSInterfaz *) m_poNavigator -> GetSubsystemByType ( M4_LM_SUBSYSTEM ) ;
		if (poSubsystem == NULL) {
			return M4_ERROR;
		}
		
		if (poSubsystem -> GetConfigStringValue	( TRANSPORT_ENABLE_TRANSPORT_DUMP , szMode , sizeof(szMode) , iLen ) != M4_SUCCESS) {
			return M4_ERROR;
		}
		
		bDumpStatus = (tolower(*szMode) == 't');
		
		SetConfigStringProperty	( TRANSPORT_DUMP_STATUS , (bDumpStatus ? "True" : "False") );
		
		if (poSubsystem -> GetConfigStringValue	( TRANSPORT_DUMP_DIRECTORY , szBaseDir , sizeof(szBaseDir) , iLen ) != M4_SUCCESS) {
			return M4_ERROR;
		}
		
		SetConfigStringProperty	( TRANSPORT_DUMP_DIRECTORY , szBaseDir );
		
		if (poSubsystem -> GetConfigStringValue	( TRANSPORT_DUMP_PREFIX , szBasePrefix , sizeof(szBasePrefix) , iLen ) != M4_SUCCESS) {
			return M4_ERROR;
		}
		
		SetConfigStringProperty	( TRANSPORT_DUMP_PREFIX , szBasePrefix );
		
		if (poSubsystem -> GetConfigStringValue	( TRANSPORT_DUMP_MAX_FILE_SIZE , szMaxSize , sizeof(szMaxSize) , iLen ) != M4_SUCCESS) {
			return M4_ERROR;
		}
		
		SetConfigStringProperty	( TRANSPORT_DUMP_MAX_FILE_SIZE , szMaxSize );
		
		m4uint32_t iDumpSize = atoi(szMaxSize);
		
		if (poSubsystem -> GetConfigStringValue	( TRANSPORT_DUMP_MAX_FILES , szMaxSize , sizeof(szMaxSize) , iLen ) != M4_SUCCESS) {
			return M4_ERROR;
		}
		
		SetConfigStringProperty	( TRANSPORT_DUMP_MAX_FILES , szMaxSize );
		
		m4uint32_t iMaxFiles = atoi(szMaxSize);
		
		// sets the command
		if (bDumpStatus){
			ClTransportLayer::EnableDumpData(szBaseDir,iDumpSize,szBasePrefix,iMaxFiles);
		}
		
		// end of initial properties propagation
		
		/* Bug 0143375
		Se leen los valores de la BDL y no del startup.obl
		*/
		ILdb *pLDB = ClFactM4ObjService::GetpLDB();
		if (pLDB == NULL ) {
			return M4_ERROR;
		}

		SetConfigStringProperty	( LDB_DEBUG_LEVEL , (m4pchar_t)pLDB->GetDebugLevel() );

		SetConfigStringProperty	( LDB_DEBUG_DETAIL_LEVEL , (m4pchar_t)pLDB->GetStrSystemDebugDetailLevel() );

		iDebugLevel  = ClVMDebugEnv::getInstance()->getDebugLevel();
		SetConfigNumericProperty( "VM_DEBUG_LEVEL", iDebugLevel );

		iMaxDebugPeriod  = ClVMDebugEnv::getInstance()->getDebugMaxPeriod();
		SetConfigNumericProperty( "VM_MAX_DEBUG_PERIOD", iMaxDebugPeriod );

		poService = ClFactM4ObjService::GetMasterObjService() ;

		if( poService != NULL )
		{
			poEnvironment = poService->GetpRegistry() ;

			if( poEnvironment != NULL )
			{
				poConfiguration = M4JavaConfiguration::Instance( poEnvironment );

				if( poConfiguration != NULL && poConfiguration->GetTrace() == M4_TRUE )
				{
					pcJavaTrace = "Enabled" ;
				}
			}
		}

		SetConfigStringProperty( "JAVA_TRACE", pcJavaTrace );

		return M4_SUCCESS ;
	}


