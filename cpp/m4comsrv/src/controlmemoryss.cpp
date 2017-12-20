//## begin module%37E7AE170269.cm preserve=no
//## end module%37E7AE170269.cm

//## begin module%37E7AE170269.cp preserve=no
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
//## end module%37E7AE170269.cp

//## Module: controlmemoryss%37E7AE170269; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\controlmemoryss.cpp

//## begin module%37E7AE170269.additionalIncludes preserve=no
//## end module%37E7AE170269.additionalIncludes

//## begin module%37E7AE170269.includes preserve=yes
#include <subsystemids.hpp>
#include <clssinterfaz.hpp>
#include <m4meminfo.hpp>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <m4log.hpp>
#include <clsubsystem.hpp>
#include <m4srvres.hpp>
#include <m4servenv.hpp>
#include <cloblconfiguration.hpp>
#include <m4srvtrace.h>
#ifdef _UNIX
#include <unistd.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif
#else
#include <io.h>
#endif
//## end module%37E7AE170269.includes

// clmemorylevelss
#include <clmemorylevelss.hpp>
// controlmemoryss
#include <controlmemoryss.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
//## begin module%37E7AE170269.declarations preserve=no
//## end module%37E7AE170269.declarations

#ifdef _WINDOWS
#include <windows.h>
#endif

//## begin module%37E7AE170269.additionalDeclarations preserve=yes

#ifndef  M4_SRV_MEMORY_LEVEL 
#define M4_SRV_MEMORY_LEVEL							0x0018	+	M4_SRV_ERROR_BASE
#endif


#define M4_SRV_TRACE_COMP_ID	"COMSRV"

//## end module%37E7AE170269.additionalDeclarations


// Class ClCotrolMemorySS 



ClCotrolMemorySS::ClCotrolMemorySS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClCotrolMemorySS::ClCotrolMemorySS%937897772.hasinit preserve=no
  //## end ClCotrolMemorySS::ClCotrolMemorySS%937897772.hasinit
  //## begin ClCotrolMemorySS::ClCotrolMemorySS%937897772.initialization preserve=yes
  :ClSSImplementation ( ai_poConfiguratoin, ai_poTables, ai_poNavigator )
  //## end ClCotrolMemorySS::ClCotrolMemorySS%937897772.initialization
{
  //## begin ClCotrolMemorySS::ClCotrolMemorySS%937897772.body preserve=yes
	m_iActualLevel = m_oBellowLevel.end() ;
    m_iColor = MEMORY_LEVEL_GREEN; 
  //## end ClCotrolMemorySS::ClCotrolMemorySS%937897772.body
}


ClCotrolMemorySS::~ClCotrolMemorySS ()
{
  //## begin ClCotrolMemorySS::~ClCotrolMemorySS%937897773.body preserve=yes
  //## end ClCotrolMemorySS::~ClCotrolMemorySS%937897773.body
}



//## Other Operations (implementation)
m4return_t ClCotrolMemorySS::Initialize ()
{
  //## begin ClCotrolMemorySS::Initialize%937897774.body preserve=yes
	m4char_t szActive [OBL_PROPVALUE_MAX_LEN ];
	m4size_t iLen;


	if ( M4_SUCCESS == GetConfigStringValue ( "ACTIVE" , szActive , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{
		if ( ! stricmp ( szActive , "false" ) )
		{
			SetState ( SS_INACTIVE , SS_STR_INACTIVE ) ;
		}
		else 
		{
			if (  stricmp ( szActive , "true" ) )
			{
//				M4_SRV_ERROR( M4_SRV_CONFIG_PROP_VALUEERROR , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s","ACTIVE"<< GetName ( ) << szActive << "TRUE" << "FALSE" );
				SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "ACTIVE property in Memory Controler Subsystem. Wrong Value %s. Only accepted TRUE or FALSE",  szActive ) ;
			}
		}
	}
	else
	{
//		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s","ACTIVE" << GetName ( ) );
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , DEBUGINFOLOG , "ACTIVE property not found in Memory Controler Subsystem. Default TRUE (ACTIVE)" ) ;
	}
	
	return M4_SUCCESS ;
  //## end ClCotrolMemorySS::Initialize%937897774.body
}

m4return_t ClCotrolMemorySS::Start ()
{
  //## begin ClCotrolMemorySS::Start%937897775.body preserve=yes
	ClSSInterfaz * poSubsystem ;
	ClSystem * poSystem = ClSystem::Instance () ;


//registrar el js a efectos de notificar el estado de la memoria
	AttachObserver ( poSystem) ;

//registrar el js a efectos de notificar el estado de la memoria
	poSubsystem = poSystem -> FindSubsystemByType ( M4_JOB_SCHEDULER_SUBSYSTEM ) ;

	if ( poSubsystem )
	{
		AttachObserver ( poSubsystem -> GetSubsystem()  ) ;
	}
//registrar el SESSION MANAGER a efectos de notificar el estado de la memoria
	poSubsystem = poSystem -> FindSubsystemByType ( M4_SESSMANAGER_SUBSYSTEM ) ;

	if ( poSubsystem )
	{
		AttachObserver ( poSubsystem -> GetSubsystem() ) ;
	}

	ClSSImplementation::Start() ;

	return M4_SUCCESS ;
  //## end ClCotrolMemorySS::Start%937897775.body
}

m4return_t ClCotrolMemorySS::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClCotrolMemorySS::Stop%937897776.body preserve=yes
	return M4_SUCCESS ;
  //## end ClCotrolMemorySS::Stop%937897776.body
}

m4return_t ClCotrolMemorySS::CheckMemoryLevel (eMemoryColor ao_iMemoryColor)
{
  //## begin ClCotrolMemorySS::CheckMemoryLevel%937897777.body preserve=yes
	ClAdminCommandTag *newParam;
	ClAdminTag *AuxParam;
	m4int32_t  iRet ;
	m4uint64_t PrivateBytes;
	m4double_t dPrivateMB ;
	m4double_t PercentMemory;
	m4double_t PercentServerMemory ;
	m4uint64_t lTotalPhysMem ;
	m4uint64_t lAvailPhysMem ;
	m4uint64_t lTotalSwapMem ;
	m4uint64_t lAvailSwapMem ;
	m4uint64_t lTotalMem ;
	m4uint64_t lAvailMem;
	static m4uint32_t iAbsHighYellow = 0 ;
	INTERFACESMAP::iterator Iterator ;
//	m4return_t lLevel = -1;
	m4int_t lLevel = -1;
	ClAdminTagList oTagList , oTagListReturn;
	m4char_t szPercent [100];
	ClAdminTagList::iterator it;
	m4bool_t bchange = M4_FALSE ;
	m4char_t szBytes[100];
	m4char_t szCPU[100];
	m4double_t cpu ;
	eStatus eState ;
	m4char_t szId [20];
	ClSystemStats * poSystemStats  = NULL ;
	ClSystem * poSystem = ClSystem::Instance () ;


	if ( ! iAbsHighYellow  )
	{
		Iterator = m_oBellowLevel.begin () ;

		Iterator ++ ;
		sprintf (szId , "%d" , (m4uint32_t)(*Iterator).second -> GetId () );

		m4char_t szPolicy [10] ;
		sprintf ( szPolicy , "%d" , (m4uint32_t) ABS_MEMORY );
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "MEMORY_POLICY", adminLong, szPolicy );
		oTagList.push_back(newParam);

		if ( M4_ERROR == (*Iterator).second -> ExecuteCommand ( szId ,  ClMemoryLevelSS::GET_LEVEL_CONFIG , oTagList , oTagListReturn ) )
			return M4_ERROR ;

		delete newParam ;
		oTagList.clear () ;

		it = find_if(oTagListReturn.begin(), oTagListReturn.end(), search_by_tag_id(2));
		if (it == oTagListReturn.end())
		{
			return M4_ERROR;
		}

		if( ! sscanf((*it)->m_tagValue , "%d" , &iAbsHighYellow  ) )
		{
			return M4_ERROR ;
		}

		while ( ( it = oTagListReturn.begin() ) != oTagListReturn.end() )
		{
			AuxParam = ( *it) ;
			oTagListReturn.erase ( it ) ;
			delete AuxParam ;
		}

		oTagListReturn.clear();

	}

	GetState ( eState ) ;

	if ( ! ( poSystemStats = (ClSystemStats *) poSystem -> GetStats () ) )
	{
		return M4_ERROR ;
	}
	
	if ( M4_SUCCESS == M4GetSystemCPU ( cpu) )
	{
		sprintf ( szCPU , "%2.2f" , cpu ) ;

		poSystemStats ->SetSystemCPUUsage ( cpu ) ;
		SetEnviron ( "SERVER" , "CPU_USAGE" , szCPU , M4_FALSE ) ;
	}


	if (M4_ERROR == M4GetProcessMem (PrivateBytes) )
	{
		return M4_ERROR ;
	}

	// bg 264157
	PrivateBytes/= (1024*1024);
	dPrivateMB = (m4int64_t) PrivateBytes;
	
	
	sprintf ( szBytes , "%10.2f" , dPrivateMB ) ;

	poSystemStats ->SetServerMemory ( dPrivateMB ) ;

	PercentServerMemory = (dPrivateMB*100) / iAbsHighYellow ;

	poSystemStats ->SetPercentServerMemory ( PercentServerMemory  ) ;

	sprintf ( szBytes , "%10.2f" , dPrivateMB ) ;

	SetEnviron ( "SERVER" , "M4SERVER_MEMORY_COUNTABILITY" , szBytes , M4_FALSE ) ;

	sprintf ( szBytes , "%3.2f" , PercentServerMemory  ) ;

	SetEnviron ( "SERVER" , "M4SERVER_MEMORY_PERCENT_COUNTABILITY" , szBytes , M4_FALSE ) ;

	if ( M4_ERROR ==  M4GetSystemMem (  lAvailPhysMem , lTotalPhysMem , lAvailSwapMem , lTotalSwapMem ) )
	{
		return M4_ERROR ;
	}

/*
	ao_lTotalVirtualMem += ao_lTotalPhysMem ;
	ao_lAvailVirtualMem += ao_lAvailPhysMem  ;
*/
	lAvailMem = lAvailSwapMem + lAvailPhysMem  ;
	lTotalMem = lTotalSwapMem + lTotalPhysMem ;

	lTotalMem /= 100 ;
	PercentMemory = (m4int64_t) (lAvailMem / lTotalMem ) ;

	PercentMemory =100 - PercentMemory  ;

//	PercentMemory = % de memoria del sistema consumida

	sprintf ( szBytes , "%3.0f" ,  PercentMemory ) ;

	poSystemStats  ->SetSystemMemoryUsage ( PercentMemory ) ;
	SetEnviron ( "SERVER" , "CONSUMED_SYSTEM_MEMORY" , szBytes , M4_FALSE ) ;

//PercentMemory = % DE MEMORIA OCUPADO 


	// eh 178555
	// Guardamos en el SS los nuevos parámetros para el cálculo del factor de carga.
	static m4double_t dMaxProcSizeMB = 0 ;

	if( dMaxProcSizeMB == 0 )
	{

	#ifdef _WINDOWS

		MEMORYSTATUS stStatus ;
		stStatus.dwLength = sizeof (stStatus) ;

		GlobalMemoryStatus( &stStatus ) ;
		dMaxProcSizeMB = stStatus.dwTotalVirtual / (1024 * 1024) ;

	#else

		struct rlimit stLimit ;
		int iRes = getrlimit( RLIMIT_DATA, &stLimit ) ;
		if( iRes == 0 || stLimit.rlim_cur != RLIM_INFINITY )
		{
			dMaxProcSizeMB = stLimit.rlim_cur / (1024 * 1024) ;
		}
		else
		{
			dMaxProcSizeMB = 2 * 1024 ;
		}

	#endif

	}

	sprintf ( szBytes , "%10.0f" , dMaxProcSizeMB ) ;
	SetEnviron ( "SERVER" , "MAX_PROC_SIZE" , szBytes , M4_FALSE ) ;

	m4double_t dFreeRAMMemMB = (m4int64_t)lAvailPhysMem / (1024*1024) ;

	sprintf ( szBytes , "%10.2f" , dFreeRAMMemMB ) ;
	SetEnviron ( "SERVER" , "FREE_RAM_MEM" , szBytes , M4_FALSE ) ;


	if ( m_iActualLevel == m_oBellowLevel.end())
	{
		m_iActualLevel = m_oBellowLevel.begin();
	}

	sprintf ( szPercent , "%4.0f" , dPrivateMB );
	newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "ABS_PROCESS_MEMORY", adminLong, szPercent );
	oTagList.push_back(newParam);

	sprintf ( szPercent , "%3.0f" , PercentMemory );
	newParam = ClAdminCommandTag::GetNewAdminCommandTag(1, "PERCENT_FREE_MEMORY", adminLong, szPercent );
	oTagList.push_back(newParam);

	sprintf ( szPercent , "%d" , PERCENT_MEMORY  );
	newParam = ClAdminCommandTag::GetNewAdminCommandTag(2, "MEMORY_POLICY", adminLong, szPercent );
	oTagList.push_back(newParam);

	while ( lLevel != M4_MATCH_LEVEL )
	{
//		lLevel=( (ClMemoryLevelSS*)((*m_iActualLevel).second ))-> IsMachingLevel ( PrivateBytes , PercentMemory , PERCENT_MEMORY ) ;
//		ClAdminTagList oTagListReturn ;

		sprintf ( szId , "%ld" , (m4uint32_t) (*m_iActualLevel).second -> GetId () );

		iRet =(*m_iActualLevel).second -> ExecuteCommand( szId , ClMemoryLevelSS::IS_MATCHING_LEVEL, oTagList , oTagListReturn ) ;

		
		
		it = find_if(oTagListReturn.begin(), oTagListReturn.end(), search_by_tag_id(0));
		if (it == oTagListReturn.end())
		{
			return M4_ERROR;
		}
				// converts the value
//		lLevel = atoi((*it)->m_tagValue);
		if (! sscanf( (*it)->m_tagValue , "%d" , &lLevel ) )
		{
			return M4_ERROR ;
		}

//		for ( it = oTagListReturn.begin() ; it != oTagListReturn.end() ;it ++)
		while ( ( it = oTagListReturn.begin() ) != oTagListReturn.end() )
		{
			AuxParam = ( *it) ;
			oTagListReturn.erase ( it ) ;
			delete AuxParam ;
		}

		oTagListReturn.clear();

	
		switch (lLevel)
		{
		case M4_BELLOW_LEVEL:

			if ( m_iActualLevel == m_oBellowLevel.begin() )
			{
				return M4_SUCCESS ;
			}

			m_iActualLevel -- ;

			break ;
		case M4_ABOVE_LEVEL	:

			m_iActualLevel ++ ;

			if ( m_iActualLevel == m_oBellowLevel.end() )
				lLevel = M4_MATCH_LEVEL ;

			break ;
		default :
			break;
		}
	}

	sprintf ( szId , "%ld" , (m4uint32_t) (*m_iActualLevel).second -> GetId () );

	iRet =(*m_iActualLevel).second -> ExecuteCommand( szId , ClMemoryLevelSS::GET_LEVEL, oTagList , oTagListReturn ) ;

	it = find_if(oTagListReturn.begin(), oTagListReturn.end(), search_by_tag_id(0));
	if (it == oTagListReturn.end())
	{
//		for ( it = oTagListReturn.begin() ; it !=oTagListReturn.end() ;it++)
		while ( ( it = oTagListReturn.begin() ) != oTagListReturn.end() )
		{
			AuxParam = ( *it) ;
			oTagListReturn.erase (it ) ;
			delete AuxParam ;
		}

		oTagListReturn.clear();

		return M4_ERROR;
	}
				// converts the value
//	lLevel = atoi((*it)->m_tagValue);
	if( ! sscanf((*it)->m_tagValue , "%d" , &lLevel ) )
	{
		return M4_ERROR ;
	}

//	for ( it = oTagListReturn.begin() ; it !=oTagListReturn.end() ;it++)
	while ( ( it = oTagListReturn.begin() ) != oTagListReturn.end() )
	{
			AuxParam = ( *it) ;
			oTagListReturn.erase ( it ) ;
			delete AuxParam ;
	}
	oTagListReturn.clear();

//	for ( it = oTagList.begin() ; it !=oTagList.end() ; it++)
	while ( ( it = oTagList.begin() ) != oTagList.end() )
	{
			AuxParam = ( *it) ;
			oTagList.erase ( it ) ;
			delete AuxParam ;
	}

	oTagList.clear();

	switch ( lLevel )
	{
	case MEMORY_LEVEL_GREEN :

		if ( m_iColor != MEMORY_LEVEL_GREEN )
		{
			SETCODEF ( M4_SRV_MEMORY_LEVEL , DEBUGINFOLOG , "Server memory level returning to NORMAL value. %l MB of used memory. %l%% of system free memory" , dPrivateMB , lAvailMem ) ;
			bchange = M4_TRUE ;
			//solamente notifico cuando estoy activo
			if (eState == SS_ACTIVE)
			{
				Notify ( GREEN_MEMORY_LEVEL ) ;
			}
		}

		m_iColor = MEMORY_LEVEL_GREEN ;
		break ;

	case MEMORY_LEVEL_YELLOW :

		SETCODEF (	M4_SRV_MEMORY_LEVEL , DEBUGINFOLOG , "Server is in ALERT memory level.\n Consuming %l MB of Memory. Free System Memory = %l%% " , dPrivateMB , lAvailMem ) ;

		if ( m_iColor != MEMORY_LEVEL_YELLOW )
		{
			bchange = M4_TRUE ;
			m_iColor = MEMORY_LEVEL_YELLOW ;
		}

			//solamente notifico cuando estoy activo
			if (eState == SS_ACTIVE)
			{
				Notify ( YELLOW_MEMORY_LEVEL ) ;
			}

		break;

	case MEMORY_LEVEL_RED :

		SETCODEF ( M4_SRV_MEMORY_LEVEL , DEBUGINFOLOG, "RED ALARM. Meta4 Server is in CRITICAL memory level.\nConsuming %l MB of Memory. Free System Memory = %l%% " , dPrivateMB , lAvailMem ) ;
		if ( m_iColor != MEMORY_LEVEL_RED )
		{
			m_iColor = MEMORY_LEVEL_RED ;
			bchange = M4_TRUE;
		}

		//solamente notifico cuando estoy activo
		if (eState == SS_ACTIVE)
		{
			Notify ( RED_MEMORY_LEVEL ) ;
		}

		break;
	}

	switch ( m_iColor ) 
	{
	case MEMORY_LEVEL_GREEN :
		SetEnviron ( "SERVER" , SERVER_MEMORY_LEVEL , "Normal" , M4_FALSE ) ;
		break ;
	case MEMORY_LEVEL_YELLOW :
		SetEnviron ( "SERVER" , SERVER_MEMORY_LEVEL , "Alert" , M4_FALSE ) ;
		break ;
	case MEMORY_LEVEL_RED :
		SetEnviron ( "SERVER" , SERVER_MEMORY_LEVEL , "Critical" , M4_FALSE ) ;
		break ;
	}

	return M4_SUCCESS ;
  //## end ClCotrolMemorySS::CheckMemoryLevel%937897777.body
}

ClSSInterfaz * ClCotrolMemorySS::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClCotrolMemorySS::GenerateSubsystem%937897782.body preserve=yes
	ClSSInterfaz * poInterfaz ;
	ClMemoryLevelSS * poMemoryLevelSS ;

	switch ( ai_lType )
	{
	case M4_MEMORY_LEVEL_SUBSYSTEM :

		poMemoryLevelSS = new ClMemoryLevelSS ( ai_poConfiguration , ai_poTables , m_poNavigator ) ;

		poInterfaz = new ClSSInterfaz ( poMemoryLevelSS  );

		poMemoryLevelSS -> SetInterfaz ( poInterfaz ) ;


		break ;
	}
	
	return poInterfaz ;
  //## end ClCotrolMemorySS::GenerateSubsystem%937897782.body
}

m4return_t ClCotrolMemorySS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClCotrolMemorySS::ExecuteCommand%937994349.body preserve=yes
	m4return_t iRet ;
	eStatus eState ;
	m4uint32_t iGreenHigh , iYellowLow , iYellowHigh , iRedLow ;
	eMemoryPolicy policy ;
	ClAdminTagList::iterator it;



	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	if (ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::ExecuteCommand (ai_cCommand, ai_oParamList, ao_oResultList);
	

	switch ( ai_cCommand )
	{
	case CHECK_MEMORY :

		iRet = CheckMemoryLevel  ( m_iColor ) ;
		break ;

	case DESACTIVATE_CONTROL_MEMORY :

		iRet = ActivateControlMemory ( M4_FALSE ) ;

		break ;

	case ACTIVATE_CONTROL_MEMORY :
		
		iRet = ActivateControlMemory ( M4_TRUE ) ;

		break ;

	case CHANGE_ABS_MEMORY_LEVELS :
	case CHANGE_PERCENT_MEMORY_LEVELS :

		it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));

		if (it == ai_oParamList.end())
		{
			SETCODEF (M4_SRV_LM_NULL_ARGUMENT , ERRORLOG , "No parameters found for memory property command.");
			return M4_ERROR;
		}

		if( ! sscanf((*it)->m_tagValue , "%d" , &iGreenHigh ) )
		{
			return M4_ERROR ;
		}

		it ++ ;

		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}

		if( ! sscanf((*it)->m_tagValue , "%d" , &iYellowLow ) )
		{
			return M4_ERROR ;
		}

		it ++ ;

		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}

		if( ! sscanf((*it)->m_tagValue , "%d" , &iYellowHigh ) )
		{
			return M4_ERROR ;
		}
		it ++ ;

		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}

		if( ! sscanf((*it)->m_tagValue , "%d" , &iRedLow ) )
		{
			return M4_ERROR ;
		}

		if ( (! iGreenHigh ) || (! iYellowLow ) || (! iYellowHigh ) || (! iRedLow ) )
		{
			return M4_ERROR ;
		}

		if ( CHANGE_ABS_MEMORY_LEVELS == ai_cCommand )
			policy = ABS_MEMORY ;
		else
			policy = PERCENT_MEMORY ;

		iRet = _ChangeMemoryLevels ( policy , iGreenHigh , iYellowLow , iYellowHigh , iRedLow ) ;

		break ;

	default :

		return M4_ERROR ;
	}

	return iRet ;
  //## end ClCotrolMemorySS::ExecuteCommand%937994349.body
}

m4return_t ClCotrolMemorySS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClCotrolMemorySS::GetCommandParams%937994350.body preserve=yes
	m4return_t retVal=M4_ERROR;
	eStatus eState ;
	ClAdminCommandTag *newParam;
	m4char_t szHighGreen [15] ;
	m4char_t szLowYellow [15] ;
	m4char_t szHighYellow [15] ;
	m4char_t szLowRed [15] ;
	m4uint32_t iHighGreen ;
	m4uint32_t iLowYellow ;
	m4uint32_t iHighYellow ;
	m4uint32_t iLowRed ;
	eMemoryPolicy iPolicy = ABS_MEMORY ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;


	//-----------------------------------------------------------------------
	// check if the command is a base class command.
	//-----------------------------------------------------------------------
	if(ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::GetCommandParams (ai_cCommand, aio_oParamList);
	
	//-----------------------------------------------------------------------
	// build the parameters list.
	//-----------------------------------------------------------------------
	
	switch (ai_cCommand)
	{
    case CHECK_MEMORY:
		// There is not parameters for this command.
		retVal=M4_SUCCESS;
		break;
		
	case CHANGE_PERCENT_MEMORY_LEVELS :
		iPolicy = PERCENT_MEMORY ;

	case CHANGE_ABS_MEMORY_LEVELS :

		_GetMemoryLevels ( 	iPolicy , iHighGreen , iLowYellow  , iHighYellow , iLowRed ) ;

		sprintf ( szHighGreen , "%d" , iHighGreen ) ;
		sprintf ( szLowYellow , "%d" , iLowYellow ) ;
		sprintf ( szHighYellow , "%d" , iHighYellow ) ;
		sprintf ( szLowRed , "%d" , iLowRed ) ;

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "GREEN_HIGH_MEMORY", adminLong, szHighGreen);
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(1, "YELLOW_LOW_MEMORY", adminLong, szLowYellow );
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(2, "YELLOW_HIGH_MEMORY", adminLong, szHighYellow );
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(3, "RED_LOW_MEMORY", adminLong, szLowRed );
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);

	default:
		return M4_ERROR;
		break;		
	}

	return retVal;
  //## end ClCotrolMemorySS::GetCommandParams%937994350.body
}

m4return_t ClCotrolMemorySS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClCotrolMemorySS::GetCommandsId%937994351.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	ClAdminTag *newCommand;

	newCommand = ClAdminTag::GetNewAdminTag(CHECK_MEMORY, "CHECK_MEMORY", M4_FALSE);

	if (newCommand == NULL) 
		return M4_ERROR ;
	aio_oCommandList.push_back(newCommand);

	if ( SS_ACTIVE == eState )
	{

		newCommand = ClAdminTag::GetNewAdminTag(DESACTIVATE_CONTROL_MEMORY, "DESACTIVATE_CONTROL_MEMORY", M4_FALSE);

		if (newCommand == NULL) 
			return M4_ERROR ;
		aio_oCommandList.push_back(newCommand);
	}
	else
	{
		newCommand = ClAdminTag::GetNewAdminTag(ACTIVATE_CONTROL_MEMORY, "ACTIVATE_CONTROL_MEMORY", M4_FALSE);

		if (newCommand == NULL) 
			return M4_ERROR ;
		aio_oCommandList.push_back(newCommand);
	}

	newCommand = ClAdminTag::GetNewAdminTag(CHANGE_PERCENT_MEMORY_LEVELS , "CHANGE_PERCENT_MEMORY_LEVELS", M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR ;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(CHANGE_ABS_MEMORY_LEVELS , "CHANGE_ABS_MEMORY_LEVELS", M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR ;
	aio_oCommandList.push_back(newCommand);
	
	return  M4_SUCCESS;
  //## end ClCotrolMemorySS::GetCommandsId%937994351.body
}

m4return_t ClCotrolMemorySS::ActivateControlMemory (m4bool_t ai_bActivate)
{
  //## begin ClCotrolMemorySS::ActivateControlMemory%948896969.body preserve=yes


	if ( ai_bActivate )
	{
		SetState ( SS_ACTIVE, SS_STR_ACTIVE) ;
	}
	else
	{
		SetState ( SS_INACTIVE, SS_STR_INACTIVE) ;
	}

	return M4_SUCCESS	;

  //## end ClCotrolMemorySS::ActivateControlMemory%948896969.body
}

// Additional Declarations
  //## begin ClCotrolMemorySS%37E7A9A50213.declarations preserve=yes
m4return_t ClCotrolMemorySS::_ChangeMemoryLevels ( eMemoryPolicy ai_iPolicy , m4uint32_t ai_HighGreenLevel , m4uint32_t ai_iLowYellowLevel , m4uint32_t ai_iHighYlleowLevel , m4uint32_t ai_iLowRedLevel ) 
{
	INTERFACESMAP::iterator Iterator ;
	ClAdminTagList::iterator it;
	m4int_t lLevel ;
	ClAdminTagList oTagListReturn , oTagList ;
	ClAdminCommandTag * new1Param , * new2Param , * new3Param ;
	m4char_t szPolicy [10] ;
	m4char_t szId [10] ;
	m4char_t szLow [15] ;
	m4char_t szHigh [15] ;


	for ( Iterator = m_oBellowLevel.begin() ; Iterator != m_oBellowLevel.end () ;Iterator ++ )
	{
		sprintf ( szId , "%d" , (m4uint32_t) (*Iterator).second -> GetId () ) ;

		if ( M4_ERROR == (*Iterator).second -> ExecuteCommand(  szId , ClMemoryLevelSS::GET_LEVEL, oTagList , oTagListReturn ) )
			return M4_ERROR ;

		it = find_if(oTagListReturn.begin(), oTagListReturn.end(), search_by_tag_id(0));
		if (it == oTagListReturn.end())
		{
			return M4_ERROR;
		}

		if( ! sscanf((*it)->m_tagValue , "%d" , &lLevel ) )
		{
			return M4_ERROR ;
		}

		delete (*it) ;
		oTagListReturn.clear () ;

		switch ( lLevel )
		{
			case MEMORY_LEVEL_GREEN :
				sprintf ( szLow , "%d" , 0 );
				sprintf ( szHigh , "%d" , ai_HighGreenLevel );
				break ;

			case MEMORY_LEVEL_YELLOW :
				sprintf ( szLow , "%d" , ai_iLowYellowLevel  );
				sprintf ( szHigh , "%d" , ai_iHighYlleowLevel );
				break ;
			
			case MEMORY_LEVEL_RED :
				sprintf ( szLow , "%d" , ai_iLowRedLevel  );
				sprintf ( szHigh , "%d" , 0 );
				break ;
			default :
				return M4_ERROR ;
		}

		sprintf ( szPolicy , "%d" , (m4uint32_t) ai_iPolicy ) ;
		new1Param = ClAdminCommandTag::GetNewAdminCommandTag(0, "POLICY", adminLong, szPolicy );
		oTagList.push_back(new1Param);

		new2Param = ClAdminCommandTag::GetNewAdminCommandTag(1, "LOW_LEVEL", adminLong, szLow );
		oTagList.push_back(new2Param);

		new3Param = ClAdminCommandTag::GetNewAdminCommandTag(2, "HIGH_LEVEL", adminLong, szHigh );
		oTagList.push_back(new3Param);

		if ( M4_ERROR == (*Iterator).second -> ExecuteCommand ( szId , ClMemoryLevelSS::CHANGE_LEVEL, oTagList , oTagListReturn ) )
		{
			return M4_ERROR ;
		}

		delete new1Param ;
		delete new2Param ;
		delete new3Param ;

		oTagList.clear () ;
	}

	return M4_SUCCESS ;

}

m4return_t ClCotrolMemorySS::_GetMemoryLevels ( eMemoryPolicy ai_iPolicy , m4uint32_t &ao_HighGreenLevel , m4uint32_t &ao_iLowYellowLevel , m4uint32_t &ao_iHighYlleowLevel , m4uint32_t &ao_iLowRedLevel ) 
{
	m4uint32_t lLevel , low , high ;
	INTERFACESMAP::iterator Iterator ;
	ClAdminTagList::iterator it;
	m4char_t szId [10] ;
	m4char_t szPolicy [10] ;
	ClAdminTagList oTagListReturn , oTagList ;
	ClAdminCommandTag * newParam ;

	
	sprintf ( szPolicy , "%d" , (m4uint32_t) ai_iPolicy ) ;
	newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "POLICY", adminLong, szPolicy );
	oTagList.push_back(newParam);

	for ( Iterator = m_oBellowLevel.begin() ; Iterator != m_oBellowLevel.end () ;Iterator ++ )
	{
		sprintf ( szId , "%d" , (m4uint32_t) (*Iterator).second -> GetId () ) ;

		if ( M4_ERROR == (*Iterator).second -> ExecuteCommand(  szId , ClMemoryLevelSS::GET_LEVEL_CONFIG , oTagList , oTagListReturn ) )
			return M4_ERROR ;

		it = find_if(oTagListReturn.begin(), oTagListReturn.end(), search_by_tag_id(0));
		if (it == oTagListReturn.end())
		{
			return M4_ERROR;
		}

		if( ! sscanf((*it)->m_tagValue , "%d" , &lLevel ) )
		{
			return M4_ERROR ;
		}
		delete (*it) ;
		it++ ;

		if( ! sscanf((*it)->m_tagValue , "%d" , &low ) )
		{
			return M4_ERROR ;
		}
		delete (*it) ;
		it++ ;

		if( ! sscanf((*it)->m_tagValue , "%d" , &high ) )
		{
			return M4_ERROR ;
		}
		delete (*it) ;

		oTagListReturn.clear () ;
		
		switch ( lLevel )
		{
			case MEMORY_LEVEL_GREEN :
				ao_HighGreenLevel = high ;
				break ;
			case MEMORY_LEVEL_YELLOW :
				ao_iLowYellowLevel = low ;
				ao_iHighYlleowLevel = high ;
				break ;
			case MEMORY_LEVEL_RED :
				ao_iLowRedLevel = low ;
				break ;

		}
	}

return M4_SUCCESS ;
}
  //## end ClCotrolMemorySS%37E7A9A50213.declarations

//## begin module%37E7AE170269.epilog preserve=yes
//## end module%37E7AE170269.epilog
