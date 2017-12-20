//## begin module%383171C4024F.cm preserve=no
//## end module%383171C4024F.cm

//## begin module%383171C4024F.cp preserve=no
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
//## end module%383171C4024F.cp

//## Module: SubSystem_M4XML%383171C4024F; Package body
//## Subsystem: M4ExeForChannel::XML::src%3826C5A30000
//## Source file: D:\Work\v400\m4exeforchannel\src\m4xmlsubsys.cpp

//## begin module%383171C4024F.additionalIncludes preserve=no
//## end module%383171C4024F.additionalIncludes

//## begin module%383171C4024F.includes preserve=yes
#include "m4stl.hpp"
#include "m4srvefcres.hpp"
#include <xercesc/util/PlatformUtils.hpp>
//#include <parsers/SAXParser.hpp>
//	Rose 98 Body Include Dependences
#include "cloblconfiguration.hpp"
#include <oltplauncher.hpp>
#include <clexecutorstatemanager.hpp>
#include "exeform4xml.hpp"
//## end module%383171C4024F.includes

// ExecutiveForOESS
#include <executiveforoess.hpp>
// SubSystem_M4XML
#include <m4xmlsubsys.hpp>
// clservicebasess
#include <clservicebasess.hpp>




//## begin module%383171C4024F.declarations preserve=no
//## end module%383171C4024F.declarations

//## begin module%383171C4024F.additionalDeclarations preserve=yes
// OBL defines
#define M4_M4XMLSUBSYS_MAX_MEMORY		"MAX_MEMORY"
#define M4_M4XMLSUBSYS_MAX_MEMORY_MAX	2147483648	//	2 Gb
#define M4_M4XMLSUBSYS_MAX_MEMORY_MIN	1048576		//	1 Mb
// Suggested value:	268435456	(256 Mb)

#define M4_M4XMLSUBSYS_MAX_NOBJS "MAX_N_OBJS"
#define M4_M4XMLSUBSYS_MAX_NOBJS_MAX	2147483648	// 31 bits = 2^31
#define M4_M4XMLSUBSYS_MAX_NOBJS_MIN	0			// 0 = Infinite (not checked)
// Suggested value:	0 - Infinito

#define M4_M4XMLSUBSYS_REFR_RATIO		"REFR_RATIO"
#define M4_M4XMLSUBSYS_REFR_RATIO_MAX	2147483648	// 31 bits = 2^31 (Cada 2 Gb de PutObject's)
#define M4_M4XMLSUBSYS_REFR_RATIO_MIN	0			// 0 = refresh in all PutObject
// Suggested value:	20 - Cada 20 PutObject's

#define M4_M4XMLSUBSYS_CACHE_DIR "STATE_CACHE_DIR"

m4uint32_t ConvRange(m4pcchar_t ai_pcString, const m4uint32_t ai_iMin, const m4uint32_t ai_iMax)
{
	m4uint32_t iVal = atol(ai_pcString);
	if (iVal < ai_iMin)
		return ai_iMin;
	if (iVal > ai_iMax)
		return ai_iMax;
	return iVal;
}
//## end module%383171C4024F.additionalDeclarations


// Class ClM4XMLSubSystem 




ClM4XMLSubSystem::ClM4XMLSubSystem (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_poOblConfiguration, ClTables* ai_poTables)
  //## begin ClM4XMLSubSystem::ClM4XMLSubSystem%942765228.hasinit preserve=no
      : m_bIsEnabled(M4_TRUE), m_poStateManager(NULL)
  //## end ClM4XMLSubSystem::ClM4XMLSubSystem%942765228.hasinit
  //## begin ClM4XMLSubSystem::ClM4XMLSubSystem%942765228.initialization preserve=yes
	  , ClServiceBaseSS (ai_poOblConfiguration, ai_poTables, ai_pNavigator )
  //## end ClM4XMLSubSystem::ClM4XMLSubSystem%942765228.initialization
{
  //## begin ClM4XMLSubSystem::ClM4XMLSubSystem%942765228.body preserve=yes

	// initializes the state manager
	m_poStateManager = new ClExecutorStateManager;
	if (m_poStateManager == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_OUT_OF_MEMORY );
		return;
	}
	
	// inits the cache parameters
	ClCacheConfigInfo oCacheConfig;
	
	size_t iLenValue = 0;
	m4char_t szValue [ OBL_PATHVALUE_MAX_LEN ] = "";
	m4return_t iRet = 0;

	iRet = ai_poOblConfiguration -> GetStringValue ( "" , M4_M4XMLSUBSYS_MAX_MEMORY ,   szValue , sizeof(szValue) ,iLenValue ) ;
	
	if (iRet == M4_SUCCESS)
		oCacheConfig.m_iMaxMemory = ConvRange(szValue,M4_M4XMLSUBSYS_MAX_MEMORY_MIN,M4_M4XMLSUBSYS_MAX_MEMORY_MAX);
	else
		oCacheConfig.m_iMaxMemory = M4_M4XMLSUBSYS_MAX_MEMORY_MAX;
	
	iRet = ai_poOblConfiguration -> GetStringValue ( "" , M4_M4XMLSUBSYS_MAX_NOBJS ,   szValue , sizeof(szValue) ,iLenValue ) ;
	if (iRet == M4_SUCCESS)
		oCacheConfig.m_iMaxNumObjects = ConvRange(szValue,M4_M4XMLSUBSYS_MAX_NOBJS_MIN,M4_M4XMLSUBSYS_MAX_NOBJS_MAX);
	else
		oCacheConfig.m_iMaxNumObjects = M4_M4XMLSUBSYS_MAX_NOBJS_MAX;

	// no limit
	//oCacheConfig.m_iMaxNumObjects = 0; 
	
	/*
	iRet = ai_poOblConfiguration -> GetStringValue ( "" , M4_M4XMLSUBSYS_REFR_RATIO ,   szValue , sizeof(szValue) ,iLenValue ) ;
	if (iRet == M4_SUCCESS)
		oCacheConfig.m_iRefreshRatio = ConvRange(szValue,M4_M4XMLSUBSYS_REFR_RATIO_MIN,M4_M4XMLSUBSYS_REFR_RATIO_MAX);
	else
		oCacheConfig.m_iRefreshRatio = M4_M4XMLSUBSYS_REFR_RATIO_MIN;
	*/
	// En la nueva cache de ChannelManager's ya no tiene sentido.
	oCacheConfig.m_iRefreshRatio = 0;
	
	iRet = ai_poOblConfiguration -> GetStringValue ( "" , M4_M4XMLSUBSYS_CACHE_DIR ,   szValue , sizeof(szValue) ,iLenValue ) ;
	if (iRet == M4_SUCCESS)
		oCacheConfig.m_sCacheDirectory = szValue;
	else
		oCacheConfig.m_sCacheDirectory = ".";
	
	// inits the caches
	if (m_poStateManager->InitCache(oCacheConfig) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_INIT_STATE_CACHE );
		return;
	}
	
	// resets caches
	if (m_poStateManager->CleanCache() != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_STATE_CLEAN );
		return;
	}

  //## end ClM4XMLSubSystem::ClM4XMLSubSystem%942765228.body
}


ClM4XMLSubSystem::~ClM4XMLSubSystem ()
{
  //## begin ClM4XMLSubSystem::~ClM4XMLSubSystem%942765223.body preserve=yes

	// makes a shutdown
	ShutDown();

  //## end ClM4XMLSubSystem::~ClM4XMLSubSystem%942765223.body
}



//## Other Operations (implementation)
m4return_t ClM4XMLSubSystem::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClM4XMLSubSystem::GetCommandsId%944132612.body preserve=yes
	if (ClServiceBaseSS::GetCommandsId(aio_oCommandList) != M4_SUCCESS)
		return M4_ERROR;
	
	//---------------------------------------------------------------
	// insert commands for this subsystem
	//---------------------------------------------------------------

	ClAdminTag *newCommand;
	
	newCommand = ClAdminTag::GetNewAdminTag(XML_SS_STATE_GET_RAM_CURRENT_SIZE, "XML_STATE_GET_RAM_CURRENT_SIZE", M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);
	
	newCommand = ClAdminTag::GetNewAdminTag(XML_SS_STATE_FLUSH, "XML_STATE_FLUSH", M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(XML_SS_STATE_GET_N_OBJECTS, "XML_STATE_GET_N_OBJECTS", M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(XML_SS_STATE_DEL_BY_SESSION_ID, "XML_STATE_DEL_BY_SESSION_ID", M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	return  M4_SUCCESS;
  //## end ClM4XMLSubSystem::GetCommandsId%944132612.body
}

m4return_t ClM4XMLSubSystem::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClM4XMLSubSystem::GetCommandParams%945083773.body preserve=yes
	// checks if the command is a base class command.
	if(ai_cCommand < ClServiceBaseSS::TOTAL_COMMANDS)
		return ClServiceBaseSS::GetCommandParams (ai_cCommand, aio_oParamList);
	
	// builds the parameters list.
	ClAdminCommandTag *newParam = NULL;
	m4return_t iResult = M4_SUCCESS;
	switch (ai_cCommand)
	{
    case XML_SS_STATE_GET_RAM_CURRENT_SIZE:
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "XML_SS_STATE_RAM_CURRENT_SIZE", adminLong, "0",M4_TRUE);
		if (newParam == NULL) 
		{
			iResult = M4_ERROR;
			break;
		}
		aio_oParamList.push_back(newParam);
		break;

    case XML_SS_STATE_FLUSH:
		break;

    case XML_SS_STATE_GET_N_OBJECTS:
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "XML_SS_STATE_N_OBJECTS", adminLong, "0",M4_TRUE);
		if (newParam == NULL) 
		{
			iResult = M4_ERROR;
			break;
		}
		aio_oParamList.push_back(newParam);
		break;


    case XML_SS_STATE_DEL_BY_SESSION_ID:
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "XML_SS_SESSION_ID", adminText, "0");
		if (newParam == NULL) 
		{
			iResult = M4_ERROR;
			break;
		}
		aio_oParamList.push_back(newParam);
		break;
		
	default:
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_INVALID_ADMIN_CMD, ai_cCommand );
		return M4_ERROR;
	}

	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_INTERNAL_CMD_ERROR, ai_cCommand );
	}

	return iResult;
  //## end ClM4XMLSubSystem::GetCommandParams%945083773.body
}

m4return_t ClM4XMLSubSystem::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClM4XMLSubSystem::ExecuteCommand%942765224.body preserve=yes
	if (ai_cCommand < ClServiceBaseSS::TOTAL_COMMANDS)
		return ClServiceBaseSS::ExecuteCommand(ai_cCommand, ai_oParamList, ao_oResultList);
	
	m4return_t iResult = M4_SUCCESS;
	
	ClExecutorStateCache * poStateCache = m_poStateManager->GetCache();
	if (poStateCache == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CACHE_INIT_ERROR );
		return M4_ERROR;
	}
	
	// EFV : cambio temporal
	ClCache *poCache = NULL;
	/* poCache = m_poStateManager->GetCache()->m_poCache;
	if (poCache == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_CACHE_INIT_ERROR );
		return M4_ERROR;
	}
	*/
	
	ClAdminCommandTag *newParam = NULL;
	m4char_t pcBuffer[64] = "";
	ClAdminTagList::iterator it;

	switch (ai_cCommand)
	{
    case XML_SS_STATE_GET_RAM_CURRENT_SIZE:

		sprintf(pcBuffer,"%ld",poCache->GetSizeCache());
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "XML_SS_STATE_RAM_CURRENT_SIZE", adminLong, pcBuffer ,M4_TRUE);
		ao_oResultList.push_back(newParam);
		break;

    case XML_SS_STATE_FLUSH:
		iResult = m_poStateManager->PersistCache();
		if (iResult != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR ( M4_ERR_XML_COMMAND_FLUSH );
		}
		break;

    case XML_SS_STATE_GET_N_OBJECTS:
		{
			m4uint32_t iNObjs = 0;
			iResult = poStateCache->GetNumObjects(iNObjs);
			if (iResult != M4_SUCCESS)
			{
				DUMP_CHLOG_ERROR ( M4_ERR_XML_COMMAND_GETNOBJS );
				break;
			}
			
			sprintf(pcBuffer,"%ld",iNObjs);
			newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "XML_SS_STATE_N_OBJECTS", adminLong, pcBuffer ,M4_TRUE);
			ao_oResultList.push_back(newParam);
			break;
		}

    case XML_SS_STATE_DEL_BY_SESSION_ID:
		{
			// gets the parameter
			it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
			if (it == ai_oParamList.end())
			{
				DUMP_CHLOG_ERROR ( M4_ERR_XML_COMMAND_REMOVE_PARAM );
				iResult = M4_ERROR;
				break;
			}
			
			// converts the value
			m4pcchar_t pcParam = (*it)->m_tagValue;
			m4int32_t iSessionId = atol(pcParam);
			
			if (iSessionId == 0 || iSessionId == -1)
			{ 
				DUMP_CHLOG_ERRORF ( M4_ERR_XML_COMMAND_SESSION_PARAM, pcParam );
				iResult = M4_ERROR;
				break;
			}
			
			// generates a new key
			string sSessionId = (*it)->m_tagValue;
			sSessionId += "-";

			// deletes object
			m4uint32_t iNObj = 0;
			iResult = m_poStateManager->RemoveState((m4pchar_t)sSessionId.c_str(),&iNObj);
			if (iResult != M4_SUCCESS)
			{ 
				DUMP_CHLOG_ERROR ( M4_ERR_XML_COMMAND_REMOVE );
				iResult = M4_ERROR;
				break;
			}
			
			if (iNObj == 0)
			{ 
				DUMP_CHLOG_ERROR ( M4_ERR_XML_COMMAND_SESSION_NOT_FOUND );
				iResult = M4_ERROR;
				break;
			}

			break;
		}
			
	default:
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_INVALID_ADMIN_CMD, ai_cCommand );
		return M4_ERROR;
	}
	
	return iResult;
  //## end ClM4XMLSubSystem::ExecuteCommand%942765224.body
}

m4return_t ClM4XMLSubSystem::ShutDown ()
{
  //## begin ClM4XMLSubSystem::ShutDown%942765225.body preserve=yes
	if (!m_bIsEnabled)
		return M4_SUCCESS;

	// sets the flag
	m_bIsEnabled = M4_FALSE;

	if (m_poStateManager != NULL)
	{
		delete m_poStateManager;
		m_poStateManager = NULL;
	}

	return M4_SUCCESS;
  //## end ClM4XMLSubSystem::ShutDown%942765225.body
}

m4return_t ClM4XMLSubSystem::Initialize ()
{
  //## begin ClM4XMLSubSystem::Initialize%942765227.body preserve=yes
	if (!m_bIsEnabled)
		return M4_ERROR;

    // Initialize the XML system
	try
	{
		XMLPlatformUtils::Initialize();
	}
	
	catch (const XMLException& toCatch)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_SAX_INIT );
		
		cerr << "Error during initialization! :\n"
			<< toCatch.getMessage() << endl;
		return M4_ERROR;
	} 
	
	return ClServiceBaseSS::Initialize () ;
  //## end ClM4XMLSubSystem::Initialize%942765227.body
}

void ClM4XMLSubSystem::Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject)
{
  //## begin ClM4XMLSubSystem::Update%944126336.body preserve=yes
	if (ai_iEvent != CLOSE_SESSION)
		return;

	// releases the space
	m4uint32_t iSessionId = ( (ClServiceBaseSS*) ai_oSubject ) ->GetId();

	//  processes event
#ifdef _DEBUG
	cout << "Warning: XML: desconnection event for session id " << iSessionId << endl;
#endif

	// clears the session
	ClearSession(iSessionId);

  //## end ClM4XMLSubSystem::Update%944126336.body
}

m4return_t ClM4XMLSubSystem::ClearSession (m4uint32_t ai_iSessionId)
{
  //## begin ClM4XMLSubSystem::ClearSession%944132616.body preserve=yes
	if (!ai_iSessionId)
	{
#ifdef _DEBUG
		cout << "Warning: XML: Event returns NULL Sessiond id" << endl;
#endif
		return M4_SUCCESS;
	}

	// erases the session
	m4char_t pcSessionId[32]="";
	sprintf(pcSessionId,"%lu-",ai_iSessionId);

	if (m_poStateManager->RemoveState(pcSessionId) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_CLEAR_SESSION , ai_iSessionId);
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClM4XMLSubSystem::ClearSession%944132616.body
}

ClSSInterfaz * ClM4XMLSubSystem::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClM4XMLSubSystem::GenerateSubsystem%957882365.body preserve=yes
	ClExecutiveForOESS *poExecutiveForOESS;

	if ( ai_lType != M4_EXECUTOR_M4XML)
	{
		SETCODEF ( M4_SRV_SS_FACTORY_ERROR , ERRORLOG , "XML Service Subsystem Don't know how to make subsystem type %d" , ai_lType ) ;
		return NULL ;
	}

	poExecutiveForOESS = new ClExecutiveForM4XMLSS( ai_poConfiguration , ai_poTables , m_poNavigator , this) ;
	poExecutiveForOESS -> SetUseExceptions ( m_bUseExceptions ) ;
	poExecutiveForOESS -> SetServiceQ ( m_poServiceQ ) ;

	return poExecutiveForOESS -> GetInterfaz ();
  //## end ClM4XMLSubSystem::GenerateSubsystem%957882365.body
}

void * ClM4XMLSubSystem::GetElement (m4pchar_t ai_szElement)
{
  //## begin ClM4XMLSubSystem::GetElement%959683121.body preserve=yes
	if (!strcmp(ai_szElement,M4XML_SS_ELM_STATE_CACHE))
		return GetStateCache();

	return NULL;
  //## end ClM4XMLSubSystem::GetElement%959683121.body
}

// Additional Declarations
  //## begin ClM4XMLSubSystem%383170EA0223.declarations preserve=yes
  //## end ClM4XMLSubSystem%383170EA0223.declarations

// Class ClExecutiveForM4XMLSS 

//## begin ClExecutiveForM4XMLSS::s_szXMLInstanceOrder%391855300143.attr preserve=no  protected: static m4size_t {UA} 0
m4size_t ClExecutiveForM4XMLSS::s_szXMLInstanceOrder = 0;
//## end ClExecutiveForM4XMLSS::s_szXMLInstanceOrder%391855300143.attr

ClExecutiveForM4XMLSS::ClExecutiveForM4XMLSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS)
  //## begin ClExecutiveForM4XMLSS::ClExecutiveForM4XMLSS%957882366.hasinit preserve=no
  //## end ClExecutiveForM4XMLSS::ClExecutiveForM4XMLSS%957882366.hasinit
  //## begin ClExecutiveForM4XMLSS::ClExecutiveForM4XMLSS%957882366.initialization preserve=yes
  :ClExecutiveForOESS (ai_poConfiguratoin, ai_poTables, ai_poNavigator , ai_poServiceSS)
  //## end ClExecutiveForM4XMLSS::ClExecutiveForM4XMLSS%957882366.initialization
{
  //## begin ClExecutiveForM4XMLSS::ClExecutiveForM4XMLSS%957882366.body preserve=yes
  //## end ClExecutiveForM4XMLSS::ClExecutiveForM4XMLSS%957882366.body
}


ClExecutiveForM4XMLSS::~ClExecutiveForM4XMLSS ()
{
  //## begin ClExecutiveForM4XMLSS::~ClExecutiveForM4XMLSS%957882367.body preserve=yes
  //## end ClExecutiveForM4XMLSS::~ClExecutiveForM4XMLSS%957882367.body
}



//## Other Operations (implementation)
m4return_t ClExecutiveForM4XMLSS::Initialize ()
{
  //## begin ClExecutiveForM4XMLSS::Initialize%957882368.body preserve=yes
	m4return_t		iRet = ClExecutiveForOESS::Initialize ();
	if (M4_SUCCESS != iRet)
		return M4_ERROR;
	
	ClLauncherStats *    poStats ;
	ClExecutive	*		poExecutive;

	m_poStatistics = poStats = new ClLauncherStats ;

	poExecutive = new ClExecutiveForM4XML(NULL);

	//Para que los miembros m_poOutputQ y m_poServiceQ estén inicializados se los ha tenido que dar 
	//el SS de servicio correspondiente en el Generate SS

	m_poLauncher = new ClLauncherForOLTP(  poExecutive , poStats, /*ai_poLog, m_poServiceQ*/m_poServiceSS);

	m_szInstanceOrder = ++s_szXMLInstanceOrder;

	return M4_SUCCESS ;
  //## end ClExecutiveForM4XMLSS::Initialize%957882368.body
}

// Additional Declarations
  //## begin ClExecutiveForM4XMLSS%391854F90252.declarations preserve=yes
  //## end ClExecutiveForM4XMLSS%391854F90252.declarations

//## begin module%383171C4024F.epilog preserve=yes
//## end module%383171C4024F.epilog
