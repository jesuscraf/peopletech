//## begin module%355AD78F03BA.cm preserve=no
//## end module%355AD78F03BA.cm

//## begin module%355AD78F03BA.cp preserve=no
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
//## end module%355AD78F03BA.cp

//## Module: CacheSubsystem%355AD78F03BA; Package body
//## Subsystem: M4CacheSubsystem::src%37987870022B
//## Source file: f:\v400\M4CacheSubsystem\src\cachesubsystem.cpp

//## begin module%355AD78F03BA.additionalIncludes preserve=no
//## end module%355AD78F03BA.additionalIncludes

//## begin module%355AD78F03BA.includes preserve=yes
//#include <inc/clcache.hpp>
//#include <inc/m4srvres.hpp>
#include <subsystemids.hpp>
#include <m4log.hpp>
#include <m4srvres.hpp>
#include <m4eventhandlers.hpp>
#include <clcache.hpp>
//## end module%355AD78F03BA.includes

// ClAdminTag
#include <cladmintag.hpp>
// CacheSubsystem
#include <cachesubsystem.hpp>
// Statistics
#include <statistics.hpp>
// propertiestable
#include <propertiestable.hpp>
// ClOblConfiguration
#include <cloblconfiguration.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>


//## begin module%355AD78F03BA.declarations preserve=no
//## end module%355AD78F03BA.declarations

//## begin module%355AD78F03BA.additionalDeclarations preserve=yes

#define M4CACHESUBSYSTEM_MEGABYTE						0x100000

ClCacheSubSystem *  CreateCacheSubSystemCpp()
{


	ClOblConfiguration * Configuration = new ClOblConfiguration;

	ClCacheSubSystem * ExportClCacheSubSystem= new ClCacheSubSystem((ClConfiguration *)Configuration);


	return ExportClCacheSubSystem;

}

ClCacheSubSystem *  CreateCacheSubSystem()

{
	return CreateCacheSubSystemCpp();
}


//## end module%355AD78F03BA.additionalDeclarations


// Class ClCacheSubSystem 










ClCacheSubSystem::ClCacheSubSystem (ClConfiguration *  ai_poConfiguration, ClTables *  ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClCacheSubSystem::ClCacheSubSystem%927543267.hasinit preserve=no
  //## end ClCacheSubSystem::ClCacheSubSystem%927543267.hasinit
  //## begin ClCacheSubSystem::ClCacheSubSystem%927543267.initialization preserve=yes
  :ClSSImplementation(ai_poConfiguration, ai_poTables , ai_poNavigator)
  //## end ClCacheSubSystem::ClCacheSubSystem%927543267.initialization
{
  //## begin ClCacheSubSystem::ClCacheSubSystem%927543267.body preserve=yes
	m_eMemoryLevel = GREEN_MEMORY_LEVEL;

	m_ulOldMaxSizeCache = 0;
  //## end ClCacheSubSystem::ClCacheSubSystem%927543267.body
}


ClCacheSubSystem::~ClCacheSubSystem ()
{
  //## begin ClCacheSubSystem::~ClCacheSubSystem%889088398.body preserve=yes
  //## end ClCacheSubSystem::~ClCacheSubSystem%889088398.body
}



//## Other Operations (implementation)
m4return_t  ClCacheSubSystem::Synchronize ()
{
  //## begin ClCacheSubSystem::Synchronize%889088400.body preserve=yes

		if(m_poVisitor) 
		{
			if(Accept(* ((ClCacheStatsVisitor *) m_poVisitor))==M4_ERROR)
				return M4_ERROR;
		}
		else
		{
			SETCODE(M4_SRV_SS_CACHE_ERROR , WARNINGLOG , "Cannot update cache statistics, visitor pointer not initialized") ;
			return M4_ERROR;
		}

/*		if(m_poRecycleVisitor) 
		{
			if(Accept(* ((ClCacheRecycleVisitor *) m_poRecycleVisitor))==M4_ERROR)
				return M4_ERROR;
		}
		else
		{
			SETCODE(M4_SRV_SS_CACHE_ERROR , WARNINGLOG , "Cannot	Recycle cache, visitor pointer not initialized") ;
			return M4_ERROR;
		}*/


		return M4_SUCCESS;
  //## end ClCacheSubSystem::Synchronize%889088400.body
}

m4return_t ClCacheSubSystem::Initialize ()
{
  //## begin ClCacheSubSystem::Initialize%890218027.body preserve=yes

	m4uint32_t iCaches = 0, iNumCaches = 0, iCacheStyle = 0 ;
	ClOblConfiguration * poConfiguration = NULL ;
	size_t iLen ;
	m4char_t szCacheType [OBL_PROPERTY_MAX_LEN];
	m4char_t szCacheName [OBL_PROPERTY_MAX_LEN] ;
	m4double_t iMaxMem = 0, iMaxItems = 0 , iRefreshRatio = 0, dDefaultMaxPeriod = 0, dDefaultMaxUnusedPeriod = 0;
	m4char_t szDefaultMaxPeriod[OBL_PROPERTY_MAX_LEN];
	m4char_t szDefaultMaxUnusedPeriod[OBL_PROPERTY_MAX_LEN];
//	ClSystem * poSystem = ClSystem::Instance() ;		

	ClSSInterfaz * poControlMemorySS = NULL ;

	m_poVisitor=new ClCacheStatsVisitor;
	if(!m_poVisitor)
			SETCODE(M4_SRV_SS_CACHE_ERROR , WARNINGLOG , "Bad memory allocation in m_poVisitor") ;
		


	m_poRecycleVisitor=new ClCacheRecycleVisitor;
	if(!m_poRecycleVisitor)
			SETCODE(M4_SRV_SS_CACHE_ERROR , WARNINGLOG , "Bad memory allocation in m_poRecycleVisitor") ;
	


	m_poStatistics=new ClCacheStats;
	if(!m_poStatistics)
			SETCODE(M4_SRV_SS_CACHE_ERROR , WARNINGLOG , "Bad memory allocation in m_poStatistics") ;



	if ( m_poNavigator)
	{
		if ( poControlMemorySS = (ClSSInterfaz*)  m_poNavigator -> GetSubsystemByType ( M4_MEMORY_CONTROL_SUBSYSTEM ) )
		{
			poControlMemorySS -> AttachObserver ( this ) ;
		}
	}


	//valores por defecto
	strcpy(szCacheName,"NO NAME");
	iCacheStyle = 0;
	iMaxMem = 10;
	iMaxItems = 1000;
	iRefreshRatio = 50;

	//CACHE NAME
	if ( M4_ERROR == GetConfigStringValue ( CACHE_NAME , szCacheName , OBL_PROPERTY_MAX_LEN , iLen ) )
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "none cache name for cache, it was named %s" , M4_CACHE_TREE ) ;
	}

	//CACHE TYPE
	if ( M4_ERROR == GetConfigStringValue ( CACHE_TYPE , szCacheType , OBL_PROPERTY_MAX_LEN , iLen ) )
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "none cache style for cache, Cache tree style asumed" ) ;
	}
	else
	{
		if ( M4_ERROR == (iCacheStyle = m_poTables -> GetId ( szCacheType ) ) )
		{
			SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "none cache style for cache, Cache tree style asumed" ) ;
		}
	}

	
	//MAX CACHE SIZE MEMORY
	if ( M4_ERROR == GetConfigNumericValue ( MAX_CACHE_SIZE_MEMORY  , iMaxMem ) )
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "none cache memory size especified" ) ;
	}
	else
	{
		if ( iMaxMem < 0 )
		{
			//No se admiten valores negativos
			SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "Cache memory size does not accept negative values" ) ;
		}
	}

	//MAX CACHE NUM ITEMS
	if ( M4_ERROR == GetConfigNumericValue ( MAX_CACHE_NUM_ITEMS  , iMaxItems ) ) 
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "None max items in cache property especified" ) ;
	}
	else
	{
		if ( iMaxItems < 0 )
		{
			//No se admiten valores negatiovos
			SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "Maximun number of items property, does not accept negative values" ) ;
		}
	}

	//REFRESH RATIO
	if ( M4_ERROR == GetConfigNumericValue ( "REFRESH_RATIO"  , iRefreshRatio ) ) 
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "Refresh Ratio in cache property not especified" ) ;
	}
	else
	{
		//No se admiten valores negativos
		if ( iRefreshRatio < 0 )
		{
			SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "Refresh Ratio property, does not accept negative values" ) ;
		}
	}


	if ( M4_ERROR == GetConfigStringValue ( "DEFAULT_MAX_PERIOD" , szDefaultMaxPeriod, OBL_PROPERTY_MAX_LEN ,  iLen)) 	
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "DEFAULT_MAX_PERIOD property not especified");
		dDefaultMaxPeriod = 0;
	}
	else
	{
		dDefaultMaxPeriod = atof(szDefaultMaxPeriod);
	}


	//DEFAULT_MAX_UNUSED_PERIOD	      
	if ( M4_ERROR == GetConfigStringValue ( "DEFAULT_MAX_UNUSED_PERIOD" , szDefaultMaxUnusedPeriod, OBL_PROPERTY_MAX_LEN ,  iLen)) 	
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "DEFAULT_MAX_UNUSED_PERIOD property not especified");
		dDefaultMaxUnusedPeriod = 0;
	}
	else
	{
		dDefaultMaxUnusedPeriod = atof(szDefaultMaxUnusedPeriod);
	}

	CreateCache(szCacheName , iCacheStyle, (m4uint32_t) iMaxMem * M4CACHESUBSYSTEM_MEGABYTE, (m4uint32_t) iMaxItems , (m4uint32_t) iRefreshRatio , dDefaultMaxPeriod, dDefaultMaxUnusedPeriod);
	
	m_eMemoryLevel = GREEN_MEMORY_LEVEL;

	m_ulOldMaxSizeCache = m_poClCacheTree->GetMaxSizeCache();

	return M4_SUCCESS;

  //## end ClCacheSubSystem::Initialize%890218027.body
}

m4return_t ClCacheSubSystem::ShutDown ()
{
  //## begin ClCacheSubSystem::ShutDown%890218028.body preserve=yes
	delete m_poVisitor;

	m_poVisitor=NULL;

	delete m_poRecycleVisitor;

	m_poRecycleVisitor=NULL;

	delete m_poStatistics;

	m_poStatistics=NULL;

	delete m_poClCacheTest;

	m_poClCacheTest=NULL;

	return M4_SUCCESS;
  //## end ClCacheSubSystem::ShutDown%890218028.body
}

m4return_t ClCacheSubSystem::Accept (ClCacheStatsVisitor &   ai_oVisitor)
{
  //## begin ClCacheSubSystem::Accept%892555075.body preserve=yes

	return ai_oVisitor.VisitCache(* m_poClCacheTree);
	 
  //## end ClCacheSubSystem::Accept%892555075.body
}

ClCacheTest *  ClCacheSubSystem::GetCache ()
{
  //## begin ClCacheSubSystem::GetCache%895146046.body preserve=yes

	if(!m_poClCacheTest)
		SETCODE(M4_SRV_SS_CACHE_ERROR , WARNINGLOG , "User cache pointer ClCacheTest * not Initialized") ;


	return m_poClCacheTest;
  //## end ClCacheSubSystem::GetCache%895146046.body
}

void ClCacheSubSystem::CreateCache (m4pchar_t ai_pcID_Cache, m4uint32_t ai_ulCacheStyle, m4uint32_t ai_ulMaxSizeCache, m4uint32_t ai_ulMaxNumOfObjs, m4uint32_t ai_ulRefreshRatio, m4double_t ai_dDefaultMaxPeriod, m4double_t ai_dDefaultMaxUnusedPeriod)
{
  //## begin ClCacheSubSystem::CreateCache%895146047.body preserve=yes

	m_pcCacheName=ai_pcID_Cache;

	SetConfigNumericProperty("REFRESH_RATIO",ai_ulRefreshRatio);			

	switch(ai_ulCacheStyle)
	{
		//Solo hay un estilo!!
		case 0:
		default:
			m_poClCacheTree=new ClCacheTree(ai_dDefaultMaxPeriod,ai_dDefaultMaxUnusedPeriod,ai_ulMaxSizeCache, ai_ulMaxNumOfObjs, ai_ulRefreshRatio);
			m_poClCacheTree->SetCacheStats((void*)m_poStatistics);
			m_poClCacheTree->SetCacheName(m_pcCacheName);
			m_poClCacheTest=new ClCacheTest(m_poClCacheTree);
			break;
	}

// Inicializo las estadísticas
	if(m_poVisitor) 
		{
			Accept(* ((ClCacheStatsVisitor *) m_poVisitor));
			
		}
		else
		{
			SETCODE(M4_SRV_SS_CACHE_ERROR , WARNINGLOG , "Cannot update cache statistics, visitor pointer not initialized") ;
			
		}		
	
  //## end ClCacheSubSystem::CreateCache%895146047.body
}

ClCacheTree *  ClCacheSubSystem::GetCacheTree ()
{
  //## begin ClCacheSubSystem::GetCacheTree%895680861.body preserve=yes
	if(!m_poClCacheTree)
		SETCODE(M4_SRV_SS_CACHE_ERROR , WARNINGLOG , "Cache pointer ClCacheTree * not Initialized") ;


	return m_poClCacheTree;
  //## end ClCacheSubSystem::GetCacheTree%895680861.body
}

m4pchar_t ClCacheSubSystem::GetCacheName ()
{
  //## begin ClCacheSubSystem::GetCacheName%895680862.body preserve=yes
	if(!m_pcCacheName)
		SETCODE(M4_SRV_SS_CACHE_ERROR , WARNINGLOG , "Cache name pointer not Initialized") ;


	return m_pcCacheName;
  //## end ClCacheSubSystem::GetCacheName%895680862.body
}

m4return_t ClCacheSubSystem::Accept (ClCacheRecycleVisitor &  ai_poVisitor)
{
  //## begin ClCacheSubSystem::Accept%897034126.body preserve=yes

		return ai_poVisitor.VisitCache(* m_poClCacheTree);

  //## end ClCacheSubSystem::Accept%897034126.body
}

m4return_t ClCacheSubSystem::GetCommandsId (ClAdminTagList &  aio_oCommandList)
{
  //## begin ClCacheSubSystem::GetCommandsId%911234943.body preserve=yes
//		if(ClSubsystemBase::GetCommandsId(aio_oCommandList) !=M4_SUCCESS)
//		return M4_ERROR;

	ClAdminTag *newCommand;

	

	newCommand = ClAdminTag::GetNewAdminTag(CLEAN_CACHE,"CLEAN_CACHE",M4_FALSE);
	M4_ASSERT(newCommand!=NULL);
	if (newCommand==NULL)return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(REFRESH_CACHE,"REFRESH_CACHE",M4_FALSE);
	M4_ASSERT(newCommand!=NULL);
	if (newCommand==NULL)return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(DUMP_REPORT,"DUMP_REPORT",M4_FALSE);
	M4_ASSERT(newCommand!=NULL);
	if (newCommand==NULL)return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(PERSIST_CACHE,"PERSIST_CACHE",M4_FALSE);
	M4_ASSERT(newCommand!=NULL);
	if (newCommand==NULL)return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(CHANGE_REFRESH_RATIO,"CHANGE_REFRESH_RATIO",M4_TRUE);
	M4_ASSERT(newCommand!=NULL);
	if (newCommand==NULL)return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(CHANGE_MAX_SIZE_CACHE,"CHANGE_MAX_SIZE_CACHE",M4_TRUE);
	M4_ASSERT(newCommand!=NULL);
	if (newCommand==NULL)return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(CHANGE_MAX_ELEM_IN_CACHE,"CHANGE_MAX_ELEM_IN_CACHE",M4_TRUE);
	M4_ASSERT(newCommand!=NULL);
	if (newCommand==NULL)return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(CHANGE_DEFAULT_MAX_PERIOD,"CHANGE_DEFAULT_MAX_PERIOD",M4_TRUE);
	M4_ASSERT(newCommand!=NULL);
	if (newCommand==NULL)return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(CHANGE_DEFAULT_MAX_UNUSED_PERIOD,"CHANGE_DEFAULT_MAX_UNUSED_PERIOD",M4_TRUE);
	M4_ASSERT(newCommand!=NULL);
	if (newCommand==NULL)return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	return M4_SUCCESS;
  //## end ClCacheSubSystem::GetCommandsId%911234943.body
}

m4return_t ClCacheSubSystem::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &  aio_oParamList)
{
  //## begin ClCacheSubSystem::GetCommandParams%911234944.body preserve=yes
		if(ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::GetCommandParams(ai_cCommand, aio_oParamList);
	
	ClAdminCommandTag *newParam;

	switch(ai_cCommand)
	{
	case CLEAN_CACHE:
		break;

	case REFRESH_CACHE:
		break;

	case DUMP_REPORT:
		break;

	case PERSIST_CACHE:
		break;
		
	case CHANGE_REFRESH_RATIO:

		newParam= ClAdminCommandTag::GetNewAdminCommandTag(CACHE_REFRESH_RATIO,"CACHE_REFRESH_RATIO",adminLong,"5");
		M4_ASSERT(newParam!=NULL);
		if (newParam==NULL)return M4_FALSE;
		aio_oParamList.push_back(newParam);
		
		break;

	case CHANGE_MAX_SIZE_CACHE:
		newParam= ClAdminCommandTag::GetNewAdminCommandTag(MAX_SIZE_CACHE,"MAX_SIZE_CACHE",adminLong,"20");
		M4_ASSERT(newParam!=NULL);
		if (newParam==NULL)return M4_FALSE;
		aio_oParamList.push_back(newParam);

		break;

	case CHANGE_MAX_ELEM_IN_CACHE:

		newParam= ClAdminCommandTag::GetNewAdminCommandTag(MAX_ELEM_IN_CACHE,"MAX_ELEM_IN_CACHE",adminLong,"1000");
		M4_ASSERT(newParam!=NULL);
		if (newParam==NULL)return M4_FALSE;
		aio_oParamList.push_back(newParam);

		break;

	case CHANGE_DEFAULT_MAX_PERIOD:

		newParam= ClAdminCommandTag::GetNewAdminCommandTag(DEFAULT_MAX_PERIOD,"DEFAULT_MAX_PERIOD",admindouble,"1");
		M4_ASSERT(newParam!=NULL);
		if (newParam==NULL)return M4_FALSE;
		aio_oParamList.push_back(newParam);

		break;

	case CHANGE_DEFAULT_MAX_UNUSED_PERIOD:

		newParam= ClAdminCommandTag::GetNewAdminCommandTag(DEFAULT_MAX_UNUSED_PERIOD,"DEFAULT_MAX_UNUSED_PERIOD",admindouble,"0.04");
		M4_ASSERT(newParam!=NULL);
		if (newParam==NULL)return M4_FALSE;
		aio_oParamList.push_back(newParam);

		break;
				
	default:
		M4_ASSERT(" Cache command not recognized " != NULL);
		break;
	}

	return  M4_SUCCESS;

  //## end ClCacheSubSystem::GetCommandParams%911234944.body
}

m4return_t ClCacheSubSystem::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &  ai_oParamList, ClAdminTagList &  ao_oResultList)
{
  //## begin ClCacheSubSystem::ExecuteCommand%911234945.body preserve=yes

	m4return_t iRet=M4_ERROR;
	ClAdminTagList::iterator it;

	if(ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::ExecuteCommand(ai_cCommand,ai_oParamList,ao_oResultList);

	switch(ai_cCommand)
	{
	case CLEAN_CACHE:
		
			M4PublishEvent( "65556", "Admin tool command \"Empty Out\"#" );

			iRet=m_poClCacheTree->RemoveAll();

			//De momento no sacamos error.
			/*if(	iRet == M4_ERROR)

				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Empty Out command");*/
			iRet = M4_SUCCESS;

			break;

	case REFRESH_CACHE:
		
			iRet=m_poClCacheTree->Recycle();

			//De momento no sacamos error.
			/*if(	iRet == M4_ERROR)

				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Refresh command");*/
			iRet = M4_SUCCESS;

			break;

	case DUMP_REPORT:
		
			break;

	case PERSIST_CACHE:
		
			iRet=m_poClCacheTest->Persist();

			//De momento no sacamos error.
			/*if(	iRet == M4_ERROR)

				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Synchronize to disc command, persistance not initialized yet");*/
			iRet = M4_SUCCESS;

			break;


	case CHANGE_REFRESH_RATIO:

		
			m4uint32_t ulRefresh_Ratio;

			it=find_if (ai_oParamList.begin(),ai_oParamList.end(),search_by_tag_id(CACHE_REFRESH_RATIO));
			
			if(it!=ai_oParamList.end())
			{
				ulRefresh_Ratio=	atoi((*it)->m_tagValue);
			}
			else{
				iRet=M4_ERROR;

				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Change Refresh Ratio command, parameter as a Refresh Ratio is not in input parameters list");


				break;
			}

			if ((m4int32_t)ulRefresh_Ratio<=0)
			{

				iRet=M4_ERROR;

				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Change Refresh Ratio command, invalid argument value");

				break;
			}


			iRet=m_poClCacheTree->SetRefreshRatio(ulRefresh_Ratio );

			SetConfigNumericProperty("REFRESH_RATIO",ulRefresh_Ratio);			

			break;

		
	case CHANGE_MAX_SIZE_CACHE:
		

	
			m4uint32_t ulMax_Size_Cache;

			it=find_if (ai_oParamList.begin(),ai_oParamList.end(),search_by_tag_id(MAX_SIZE_CACHE));
			
			if(it!=ai_oParamList.end())
			{
			ulMax_Size_Cache=atoi((*it)->m_tagValue);
			}
			else{
				iRet=M4_ERROR;

				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Set Max. Size Cache command, parameter as a Max. size cache is not in input parameters list");


				break;
			}

			if ((m4int32_t)ulMax_Size_Cache<=0)
			{

				iRet=M4_ERROR;

				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Set Max. Size Cache command, invalid argument value");

				break;

			}

			iRet=m_poClCacheTree->SetMaxSizeCache(ulMax_Size_Cache * M4CACHESUBSYSTEM_MEGABYTE);

			SetConfigNumericProperty( "MAX_CACHE_SIZE_MEMORY",ulMax_Size_Cache);			

			//Pasamos a nivel Verde, ya que se pone la mem max definida por el administrador.
			m_eMemoryLevel = GREEN_MEMORY_LEVEL;

			m_ulOldMaxSizeCache = ulMax_Size_Cache * M4CACHESUBSYSTEM_MEGABYTE;

			break;


	case CHANGE_MAX_ELEM_IN_CACHE:
			m4uint32_t ulMax_NOf_Objs;

			it=find_if (ai_oParamList.begin(),ai_oParamList.end(),search_by_tag_id(MAX_ELEM_IN_CACHE));
			
			if(it!=ai_oParamList.end())
			{
			ulMax_NOf_Objs=atoi((*it)->m_tagValue);
			}
			else{
				iRet=M4_ERROR;

				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Set max. number of elements command, parameter as a Max number of elements is not in input parameters list");


				break;
			}

			if ((m4int32_t)ulMax_NOf_Objs<=0)
			{

				iRet=M4_ERROR;

				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Set max. number of elements command, invalid argument value");

				break;
			}


			iRet=m_poClCacheTree->SetMaxNOfObjs(ulMax_NOf_Objs);

			SetConfigNumericProperty( "MAX_CACHE_NUM_ITEMS",ulMax_NOf_Objs);			

			break;

	case CHANGE_DEFAULT_MAX_PERIOD:
			m4double_t dDefaultMaxPeriod;

			it=find_if (ai_oParamList.begin(),ai_oParamList.end(),search_by_tag_id(DEFAULT_MAX_PERIOD));
			
			if(it!=ai_oParamList.end())
			{
				dDefaultMaxPeriod=atof((*it)->m_tagValue);
			}
			else
			{
				iRet=M4_ERROR;
				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Set default max period command, parameter as a Max number of elements is not in input parameters list");
				break;
			}

			if (dDefaultMaxPeriod<0)
			{
				iRet=M4_ERROR;
				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Set default max period command, invalid argument value");
				break;
			}

			iRet=m_poClCacheTree->SetMaxPeriod(dDefaultMaxPeriod);

			SetConfigStringProperty( "DEFAULT_MAX_PERIOD",(*it)->m_tagValue);			
			break;

	case CHANGE_DEFAULT_MAX_UNUSED_PERIOD:
			m4double_t dDefaultMaxUnusedPeriod;

			it=find_if (ai_oParamList.begin(),ai_oParamList.end(),search_by_tag_id(DEFAULT_MAX_UNUSED_PERIOD));
			
			if(it!=ai_oParamList.end())
			{
				dDefaultMaxUnusedPeriod=atof((*it)->m_tagValue);
			}
			else
			{
				iRet=M4_ERROR;
				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Set default max period command, parameter as a Max number of elements is not in input parameters list");
				break;
			}

			if (dDefaultMaxUnusedPeriod<0)
			{
				iRet=M4_ERROR;
				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Set default max period command, invalid argument value");
				break;
			}

			iRet=m_poClCacheTree->SetMaxUnusedPeriod(dDefaultMaxUnusedPeriod);

			SetConfigStringProperty( "DEFAULT_MAX_UNUSED_PERIOD",(*it)->m_tagValue);			
			break;


	case CHANGE_CACHE_STATUS:

			m4uint32_t ulNewStatus;
			m4pchar_t pcStatus;

			it=find_if (ai_oParamList.begin(),ai_oParamList.end(),search_by_tag_id(CACHE_STATUS));
			
			if(it!=ai_oParamList.end())
			{
				ulNewStatus=atoi((*it)->m_tagValue);
			}
			else
			{
				iRet=M4_ERROR;
				SETCODE(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,"Cannot execute Set cache status command, parameter is not in input parameters list");
				break;
			}

			
			switch(ulNewStatus)
			{
				case SS_INACTIVE:
					pcStatus = SS_STR_INACTIVE;
					break;
				case SS_ACTIVE:
					pcStatus = SS_STR_ACTIVE;
					break;
				case SS_PAUSED:
					pcStatus = SS_STR_PAUSED;
					break;
				case SS_SHUTING_DOWN:
					pcStatus = SS_STR_SHUTTING_DOWN;
					break;
				case SS_NOT_ACCESIBLE:
					pcStatus = SS_STR_NOT_ACCESIBLE;
					break;
				default:
					pcStatus = SS_STR_INACTIVE;
					break;
			}

			SetState((eStatus)ulNewStatus, pcStatus);
			break;


	default:

		SETCODEF ( M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "Invalid Administrator List SubSystem command Id: %d", ai_cCommand );

		iRet=M4_ERROR;
		
	}

	

	return iRet;

  //## end ClCacheSubSystem::ExecuteCommand%911234945.body
}

void ClCacheSubSystem::Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject)
{
  //## begin ClCacheSubSystem::Update%942334657.body preserve=yes
	 
	switch(ai_iEvent)
	{

	case GREEN_MEMORY_LEVEL:
		if (m_eMemoryLevel != GREEN_MEMORY_LEVEL)
		{
			m_poClCacheTree->SetMaxSizeCache( m_ulOldMaxSizeCache );
		}
		break;

	case RED_MEMORY_LEVEL:
	case YELLOW_MEMORY_LEVEL:
		if ((m_eMemoryLevel != RED_MEMORY_LEVEL) && (m_eMemoryLevel != YELLOW_MEMORY_LEVEL))
		{
			m_ulOldMaxSizeCache = m_poClCacheTree->GetMaxSizeCache();
			m_poClCacheTree->SetMaxSizeCache( 1 );
		}
		break;
	
	default:
		m_ulOldMaxSizeCache = m_poClCacheTree->GetMaxSizeCache();
		break;
	}

	m_eOldMemoryLevel = m_eMemoryLevel;
	m_eMemoryLevel = ai_iEvent;

  //## end ClCacheSubSystem::Update%942334657.body
}

void * ClCacheSubSystem::GetElement (m4pchar_t ai_szElement)
{
  //## begin ClCacheSubSystem::GetElement%953806192.body preserve=yes
	if ( ! stricmp ( ai_szElement , "CACHE" ) )
	{
		return GetCache () ;
	}
	else
	{
		return NULL ;
	}
  //## end ClCacheSubSystem::GetElement%953806192.body
}

m4return_t ClCacheSubSystem::Start ()
{
  //## begin ClCacheSubSystem::Start%958033384.body preserve=yes
	return ClSSImplementation::Start();
  //## end ClCacheSubSystem::Start%958033384.body
}

// Additional Declarations
  //## begin ClCacheSubSystem%34FD993D00F4.declarations preserve=yes
  //## end ClCacheSubSystem%34FD993D00F4.declarations

// Class ClCacheStatsVisitor 

ClCacheStatsVisitor::ClCacheStatsVisitor ()
  //## begin ClCacheStatsVisitor::ClCacheStatsVisitor%889629077.hasinit preserve=no
  //## end ClCacheStatsVisitor::ClCacheStatsVisitor%889629077.hasinit
  //## begin ClCacheStatsVisitor::ClCacheStatsVisitor%889629077.initialization preserve=yes
  //## end ClCacheStatsVisitor::ClCacheStatsVisitor%889629077.initialization
{
  //## begin ClCacheStatsVisitor::ClCacheStatsVisitor%889629077.body preserve=yes
  //## end ClCacheStatsVisitor::ClCacheStatsVisitor%889629077.body
}



//## Other Operations (implementation)
m4return_t ClCacheStatsVisitor::VisitCache (ClCacheTree &  ai_oVisitCache)
{
  //## begin ClCacheStatsVisitor::VisitCache%889088404.body preserve=yes


		ClStatPercentType fNewHitRatio, fNewSizeCache;

		m4uint32_t NewNOfObjsInDelQueue,NewNumOfRef,NewNumOfObjs,NewRefreshRatio,NOfObjsInLRU;

		ClCacheStats *pStats;

		pStats=(ClCacheStats *)ai_oVisitCache.GetCacheStats();

		NewNumOfObjs= ai_oVisitCache.GetNObjInCache ();

		pStats->SetNOfObjInCache(NewNumOfObjs);

		NewNOfObjsInDelQueue= ai_oVisitCache.GetNObjInQueue ();

		pStats->SetNOfObjInQueue(NewNOfObjsInDelQueue);

		NewNumOfRef= ai_oVisitCache.GetNOfRef ();

//		if(NewNumOfRef)
//			pStats->SetNOfRef((NewNumOfRef/(NewNOfObjsInDelQueue+NewNumOfObjs))*100);
//		else
		pStats->SetNOfRef(NewNumOfRef);

		fNewHitRatio=ai_oVisitCache.GetHitsRatio();

		pStats -> SetHitsRatio(fNewHitRatio);

		fNewSizeCache=(m4float_t)ai_oVisitCache.GetSizeCache();

		pStats -> SetSizeCache(fNewSizeCache/(m4float_t)M4CACHESUBSYSTEM_MEGABYTE);

		NewRefreshRatio=ai_oVisitCache.GetRefreshRatio();

		pStats -> SetRefreshRatio(NewRefreshRatio);

//		NewMaxNOfObjs=ai_oVisitCache.GetMaxNOfObjs();

//		pStats -> SetMaxNOfObjs(NewMaxNOfObjs);

//		NewMaxSizeCache=ai_oVisitCache.GetMaxSizeCache();

//		pStats -> SetMaxSizeCache(NewMaxSizeCache/1000);

		NOfObjsInLRU=ai_oVisitCache.GetNOfObjInLRU ();

		pStats -> SetNOfObjInLRU(NOfObjsInLRU);

//		m4uint32_t DefaultPolicy;

//		ai_oVisitCache.GetDefaultPolicy(DefaultPolicy);
		
//		pStats->SetDefaultPolicy(DefaultPolicy);


		return M4_SUCCESS;


  //## end ClCacheStatsVisitor::VisitCache%889088404.body
}

// Additional Declarations
  //## begin ClCacheStatsVisitor%34FD9E4801D7.declarations preserve=yes
  //## end ClCacheStatsVisitor%34FD9E4801D7.declarations

// Class ClCacheRecycleVisitor 


//## Other Operations (implementation)
m4return_t ClCacheRecycleVisitor::VisitCache (ClCacheTree &  ai_oVisitCache)
{
  //## begin ClCacheRecycleVisitor::VisitCache%897034125.body preserve=yes

	return ai_oVisitCache.Recycle();


  //## end ClCacheRecycleVisitor::VisitCache%897034125.body
}

// Additional Declarations
  //## begin ClCacheRecycleVisitor%3577DC33018C.declarations preserve=yes
  //## end ClCacheRecycleVisitor%3577DC33018C.declarations

// Class ClCacheStats 














ClCacheStats::ClCacheStats ()
  //## begin ClCacheStats::ClCacheStats%889521624.hasinit preserve=no
      : m_onOfReferences(0), m_onObjInQueue(0), m_onObjInCache(0), m_oHitRatio(0), m_ulObjInLRU(0)
  //## end ClCacheStats::ClCacheStats%889521624.hasinit
  //## begin ClCacheStats::ClCacheStats%889521624.initialization preserve=yes
  //## end ClCacheStats::ClCacheStats%889521624.initialization
{
  //## begin ClCacheStats::ClCacheStats%889521624.body preserve=yes
	m_uiNumberOfStats = TOTAL_NUM ;

	
  //## end ClCacheStats::ClCacheStats%889521624.body
}


ClCacheStats::~ClCacheStats ()
{
  //## begin ClCacheStats::~ClCacheStats%889521625.body preserve=yes
  //## end ClCacheStats::~ClCacheStats%889521625.body
}



//## Other Operations (implementation)
m4return_t ClCacheStats::Reset ()
{
  //## begin ClCacheStats::Reset%889629076.body preserve=yes
	
	return M4_SUCCESS;
  //## end ClCacheStats::Reset%889629076.body
}

m4return_t ClCacheStats::SetNOfRef (ClStatNumericalType ai_oNOfRef)
{
  //## begin ClCacheStats::SetNOfRef%889694507.body preserve=yes
	m_onOfReferences=ai_oNOfRef;

	return M4_SUCCESS;
  //## end ClCacheStats::SetNOfRef%889694507.body
}

m4return_t ClCacheStats::SetNOfObjInCache (ClStatNumericalType ai_oNObjInCache)
{
  //## begin ClCacheStats::SetNOfObjInCache%889694509.body preserve=yes
	m_onObjInCache=ai_oNObjInCache;

	return M4_SUCCESS;
  //## end ClCacheStats::SetNOfObjInCache%889694509.body
}

m4return_t ClCacheStats::SetNOfObjInQueue (ClStatNumericalType ai_oNObjInQueue)
{
  //## begin ClCacheStats::SetNOfObjInQueue%889694508.body preserve=yes
	m_onObjInQueue=ai_oNObjInQueue;

	return M4_SUCCESS;
  //## end ClCacheStats::SetNOfObjInQueue%889694508.body
}

m4return_t ClCacheStats::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData)
{
  //## begin ClCacheStats::GetStat%889777601.body preserve=yes
	m4uint16_t				lowIndex;
	eCacheStatsID			statIndex;
	m4return_t				res = M4_SUCCESS;

	lowIndex = ai_hStatId.GetLowID();
	statIndex = (eCacheStatsID)lowIndex;

	if (0 < ao_poData.GetSize())
		return M4_ERROR;

	ao_poData.SetSize(DEF_STRING_SIZE);
	m4pchar_t	buffer = ao_poData.GetBuffer();

	switch(statIndex)
	{

	case NUM_OF_OBJS_IN_CACHE:
		sprintf ( buffer , "%d" , m_onObjInCache ) ;

		break ;

	case NUM_OF_REF_TO_CACHE:
		sprintf ( buffer , "%d" , m_onOfReferences ) ;
		break;

	case HIT_RATIO:
		sprintf ( buffer , "%f" ,m_oHitRatio) ;
		break ;

	case SIZE_CACHE:
		sprintf ( buffer , "%f" , m_oSizeCache) ;
		break ;

/*	case RECYCLE_INT:
		sprintf ( buffer , "%s" , (m4pchar_t)m_oRecycleInterval) ;
		break ;*/

	case OBJS_IN_QUEUE:
		sprintf ( buffer , "%d" , m_onObjInQueue ) ;
		break;

/*	case MAX_NUM_OF_OBJS:
		sprintf ( buffer , "%d" , m_ulMaxNOfObjs ) ;
		break;*/

	/*case MAX_SIZE_CACHE:
		sprintf ( buffer , "%d" , m_ulMaxSizeCache) ;
		break;*/

	/*case REFRESH_RATIO:
		sprintf ( buffer , "%d" , m_ulRefreshRatio) ;
		break ;*/

	case OBJS_IN_LRU:
		sprintf ( buffer , "%d" , m_ulObjInLRU ) ;
		break;

	/*case DEFAULT_POLICY:
		sprintf ( buffer , "%d" , m_ulDefaultPolicy ) ;
		break;*/


	default:
		sprintf(buffer, "%010d", 0);
		res = M4_ERROR;
	}

	return res;
  //## end ClCacheStats::GetStat%889777601.body
}

m4uint32_t ClCacheStats::GetNumberOfStats ()
{
  //## begin ClCacheStats::GetNumberOfStats%889777602.body preserve=yes
	return TOTAL_NUM;
  //## end ClCacheStats::GetNumberOfStats%889777602.body
}

m4return_t ClCacheStats::ResetStat (ClStatHandle &  ai_hStatld)
{
  //## begin ClCacheStats::ResetStat%889777603.body preserve=yes
	m4uint16_t				lowIndex;
	eCacheStatsID			statIndex;
	m4return_t				res = M4_SUCCESS;

	if (M4_ERROR == _CheckStatID(ai_hStatld, M4_CACHES_SUBSYSTEM))
		return M4_ERROR;

	lowIndex = ai_hStatld.GetLowID();
	statIndex = (eCacheStatsID)lowIndex;

	switch(statIndex)
	{
	case NUM_OF_OBJS_IN_CACHE:
		m_onObjInCache=0;
		break ;

	case OBJS_IN_QUEUE:
		m_onObjInQueue=0;
		break;

	case OBJS_IN_LRU:
		m_ulObjInLRU=0;
		break;

	case NUM_OF_REF_TO_CACHE:
		m_onOfReferences=0;
		break;
	
	case HIT_RATIO:
		m_oHitRatio=0;
		break ;

	default:
		res = M4_ERROR;
	}

	return res;

  //## end ClCacheStats::ResetStat%889777603.body
}

m4return_t ClCacheStats::SetHitsRatio (ClStatPercentType ai_oHitRatio)
{
  //## begin ClCacheStats::SetHitsRatio%889863966.body preserve=yes

	m_oHitRatio=ai_oHitRatio;

	return M4_SUCCESS;

  //## end ClCacheStats::SetHitsRatio%889863966.body
}

m4return_t ClCacheStats::SetDefaultPolicy (m4uint32_t ai_ulDefaultPolicy)
{
  //## begin ClCacheStats::SetDefaultPolicy%889863967.body preserve=yes

	m_ulDefaultPolicy=ai_ulDefaultPolicy;

	return M4_SUCCESS;

  //## end ClCacheStats::SetDefaultPolicy%889863967.body
}

m4return_t ClCacheStats::SetSizeCache (ClStatPercentType ai_ulSizeCache)
{
  //## begin ClCacheStats::SetSizeCache%895680863.body preserve=yes
	m_oSizeCache=ai_ulSizeCache;

	return M4_SUCCESS;
  //## end ClCacheStats::SetSizeCache%895680863.body
}

m4return_t ClCacheStats::SetMaxNOfObjs (m4uint32_t ai_ulMaxNOfObjs)
{
  //## begin ClCacheStats::SetMaxNOfObjs%897498325.body preserve=yes

	m_ulMaxNOfObjs=ai_ulMaxNOfObjs;

	return M4_SUCCESS;
  //## end ClCacheStats::SetMaxNOfObjs%897498325.body
}

m4return_t ClCacheStats::SetMaxSizeCache (m4uint32_t ai_ulMaxSizeCache)
{
  //## begin ClCacheStats::SetMaxSizeCache%897498326.body preserve=yes
	m_ulMaxSizeCache=ai_ulMaxSizeCache;

	return M4_SUCCESS;
  //## end ClCacheStats::SetMaxSizeCache%897498326.body
}

m4return_t ClCacheStats::SetRefreshRatio (m4uint32_t ai_ulRefreshRatio)
{
  //## begin ClCacheStats::SetRefreshRatio%897498327.body preserve=yes
	m_ulRefreshRatio=ai_ulRefreshRatio;

	return M4_SUCCESS;
  //## end ClCacheStats::SetRefreshRatio%897498327.body
}

m4return_t ClCacheStats::SetNOfObjInLRU (m4uint32_t ai_ulNOfObjInLRU)
{
  //## begin ClCacheStats::SetNOfObjInLRU%911234946.body preserve=yes
	m_ulObjInLRU=ai_ulNOfObjInLRU;
	return M4_SUCCESS;
  //## end ClCacheStats::SetNOfObjInLRU%911234946.body
}

// Additional Declarations
  //## begin ClCacheStats%35055B4B00E7.declarations preserve=yes
  //## end ClCacheStats%35055B4B00E7.declarations

// Class ClCacheRecycleVisit 

ClCacheRecycleVisit::ClCacheRecycleVisit ()
  //## begin ClCacheRecycleVisit::ClCacheRecycleVisit%897034124.hasinit preserve=no
  //## end ClCacheRecycleVisit::ClCacheRecycleVisit%897034124.hasinit
  //## begin ClCacheRecycleVisit::ClCacheRecycleVisit%897034124.initialization preserve=yes
  //## end ClCacheRecycleVisit::ClCacheRecycleVisit%897034124.initialization
{
  //## begin ClCacheRecycleVisit::ClCacheRecycleVisit%897034124.body preserve=yes
  //## end ClCacheRecycleVisit::ClCacheRecycleVisit%897034124.body
}


// Additional Declarations
  //## begin ClCacheRecycleVisit%3577D60B0258.declarations preserve=yes
  //## end ClCacheRecycleVisit%3577D60B0258.declarations

//## begin module%355AD78F03BA.epilog preserve=yes
//## end module%355AD78F03BA.epilog
