//## begin module%3822A11B03C1.cm preserve=no
//## end module%3822A11B03C1.cm

//## begin module%3822A11B03C1.cp preserve=no
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
//## end module%3822A11B03C1.cp

//## Module: ProxSpacsSS%3822A11B03C1; Package body
//## Subsystem: M4ExeForChannel::Proxy::src%392274340168
//## Source file: D:\Work\v600\m4exeforchannel\src\proxspacsss.cpp

//## begin module%3822A11B03C1.additionalIncludes preserve=no
//## end module%3822A11B03C1.additionalIncludes

//## begin module%3822A11B03C1.includes preserve=yes
#include <metadatasubsys.hpp>
#include <cachesubsystem.hpp>
#include <sergenproxy.hpp>
#include <clconfiguration.hpp>
#include <configtree.hpp>
#include <clssinterfaz.hpp>
#include <m4srvefcres.hpp>
#include <chlog.hpp>
#include <m4ldb.hpp>
#include <clfactm4objservice.hpp>
#include <cancelevent.hpp>
#include <m4srvtrace.h>


#define OBL_PROXY_INFO_ID									"PROXY_SS_ATTRIB_INFO"
#define M4_PROXYSUBSYSTEM_DATACACHE							"DATA_CACHE"
#define M4_PROXYSPACES_SS_MAXNSPACES						"MAX_N_SPACES"
#define M4_PROXYSPACES_SS_NON_PROXY_DDBB_CONECTION_COUNT	"NON_PROXY_DDBB_CONECTION_COUNT"

//## end module%3822A11B03C1.includes

// ProxSpacsSS
#include <proxspacsss.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
// IProxyManager
#include <iproxymanager.hpp>


//## begin module%3822A11B03C1.declarations preserve=no
//## end module%3822A11B03C1.declarations

//## begin module%3822A11B03C1.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID	"EXECHANNEL"
//## end module%3822A11B03C1.additionalDeclarations


// Class ClProxySpacesSS 


ClProxySpacesSS::ClProxySpacesSS (ClConfiguration* ai_poConfiguration, ClTables* ai_poTables, ClNavigatorFirewall* ai_poNavigator)
  //## begin ClProxySpacesSS::ClProxySpacesSS%941744286.hasinit preserve=no
  //## end ClProxySpacesSS::ClProxySpacesSS%941744286.hasinit
  //## begin ClProxySpacesSS::ClProxySpacesSS%941744286.initialization preserve=yes
  : ClSSImplementation(ai_poConfiguration, ai_poTables, ai_poNavigator)
  //## end ClProxySpacesSS::ClProxySpacesSS%941744286.initialization
{
  //## begin ClProxySpacesSS::ClProxySpacesSS%941744286.body preserve=yes
	// Creo el ClServerProxyManager. Encargado de gestionar los espacios proxy.
	m_pServProxMana = new ClServerProxyManager;
	M4_ASSERT(m_pServProxMana != NULL);
	if (m_pServProxMana == NULL)
		return;

	// La indicamos al ClServerProxyManager cual es el subsistema que se encarga de el
	m_pServProxMana->SetProxySpacesSS(*this);

	// Adds a new property to the configuration.
	ai_poConfiguration->AddStringProperty("", OBL_PROXY_INFO_ID, "[]");
  //## end ClProxySpacesSS::ClProxySpacesSS%941744286.body
}


ClProxySpacesSS::~ClProxySpacesSS ()
{
  //## begin ClProxySpacesSS::~ClProxySpacesSS%941744287.body preserve=yes
	delete m_pServProxMana;
  //## end ClProxySpacesSS::~ClProxySpacesSS%941744287.body
}



//## Other Operations (implementation)
ClProxySpace* const ClProxySpacesSS::GetSpace (const m4char_t* const ai_pcClProxySpaceID, const m4bool_t ai_bCreateNew)
{
  //## begin ClProxySpacesSS::GetSpace%941744280.body preserve=yes
	return m_pServProxMana->GetSpace (ai_pcClProxySpaceID, ai_bCreateNew);
  //## end ClProxySpacesSS::GetSpace%941744280.body
}

m4return_t ClProxySpacesSS::ReleaseSpace (ClProxySpace* const ai_poClProxySpace, const m4bool_t ai_bDoLock)
{
  //## begin ClProxySpacesSS::ReleaseSpace%941744281.body preserve=yes
	return m_pServProxMana->ReleaseSpace (ai_poClProxySpace, ai_bDoLock);
  //## end ClProxySpacesSS::ReleaseSpace%941744281.body
}

void ClProxySpacesSS::SetCaches (ClCache* const ai_poCMCRCache, ClCache* const ai_poCSCRCache, ClCache* const ai_poPresentationCache, ClCache* const ai_poMapCache, ClCache* const ai_poDatacache)
{
  //## begin ClProxySpacesSS::SetCaches%941744282.body preserve=yes
	m_pServProxMana->SetCaches (ai_poCMCRCache, ai_poCSCRCache, ai_poPresentationCache, ai_poMapCache, ai_poDatacache);
  //## end ClProxySpacesSS::SetCaches%941744282.body
}

void ClProxySpacesSS::GetCaches (ClCache * &ao_poCMCRCache, ClCache * &ao_poCSCRCache, ClCache * &ao_poPresentationCache, ClCache * &ao_poMapCache, ClCache * &ao_poDatacache) const
{
  //## begin ClProxySpacesSS::GetCaches%941744283.body preserve=yes
	m_pServProxMana->GetCaches (ao_poCMCRCache, ao_poCSCRCache, ao_poPresentationCache, ao_poMapCache, ao_poDatacache);
  //## end ClProxySpacesSS::GetCaches%941744283.body
}

m4bool_t ClProxySpacesSS::IsServerMode () const
{
  //## begin ClProxySpacesSS::IsServerMode%941744284.body preserve=yes
	return m_pServProxMana->IsServerMode ();
  //## end ClProxySpacesSS::IsServerMode%941744284.body
}

m4return_t ClProxySpacesSS::SendEvent ()
{
  //## begin ClProxySpacesSS::SendEvent%941744285.body preserve=yes
	return m_pServProxMana->SendEvent ();
  //## end ClProxySpacesSS::SendEvent%941744285.body
}

m4return_t ClProxySpacesSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClProxySpacesSS::ExecuteCommand%941744288.body preserve=yes
	if (ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::ExecuteCommand(ai_cCommand, ai_oParamList, ao_oResultList);
	
	m4return_t	iRet		= M4_ERROR;
	m4int32_t	iSessionId	= 0;
	
	switch (ai_cCommand)
	{
	case COMMAND_ADD_NEW_PROXY_SPACE:
		{
			// Gets the parameter.
			ClAdminTagList::iterator it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
			if (it == ai_oParamList.end())
			{
				// Se esperaba un parametro y no se ha encontrado
				iRet = M4_ERROR;
				break;
			}
			m4int32_t iNumPxSpacesToAdd = atoi((*it)->m_tagValue);

			iRet = IncrementMaxNSpaces(iNumPxSpacesToAdd);
		}
		break;
		
	case COMMAND_USED_MEMORY:
		// Pendiente CRG
		break;
		
	case DESTROY_PX_SPACE_BY_SESSION_ID:
		{
			// gets the parameter
			ClAdminTagList::iterator it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
			if (it == ai_oParamList.end())
			{
				// Se esperaba un parametro y no se ha encontrado
				iRet = M4_ERROR;
				break;
			}
			
			// converts the value
			iSessionId = atol((*it)->m_tagValue);
			if (iSessionId <= 0)
			{
				DUMP_CHLOG_ERROR(M4_ERR_PROXY_INCORRECT_SESSION_ID);
				iRet = M4_ERROR;
				break;
			}
			
			iRet = m_pServProxMana->ReleaseSpace(iSessionId);
			if (iRet != M4_SUCCESS)
				DUMP_CHLOG_ERROR(M4_ERR_PROXY_INCORRECT_SESSION_ID);
		}
		break;
		
	default:
		M4_SRV_ERROR ( M4_SRV_COMMAND_ID_ERROR, "Error Executing command in '%0:s' Subsystem. Unknown command id: %1:d" , GetName() << ai_cCommand ) ;
/*		SETCODEF(M4_SRV_SS_COMMAND_PARAM_ERROR,ERRORLOG,
			"Proxys Subsystem: Invalid Administrator List SubSystem command Id: %d",
			ai_cCommand);
*/		iRet = M4_ERROR;
	}
	
	return iRet;
  //## end ClProxySpacesSS::ExecuteCommand%941744288.body
}

m4return_t ClProxySpacesSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClProxySpacesSS::GetCommandParams%941744289.body preserve=yes
	// checks if the command is a base class command.
	if (ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::GetCommandParams (ai_cCommand, aio_oParamList);
	
	// builds the parameters list.
	ClAdminCommandTag* newParam = NULL;
	switch (ai_cCommand)
	{
    case DESTROY_PX_SPACE_BY_SESSION_ID: // By Sesseion Id
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "PROXY_SS_SESSION_ID", adminLong, "0");
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);
		break;
		
	case COMMAND_ADD_NEW_PROXY_SPACE:
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "PARAMETER_NUMBER_TO_ADD", adminLong, "1");
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);
		break;
	default:
		// M4_ASSERT("Proxy Command not recognized" == NULL);
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClProxySpacesSS::GetCommandParams%941744289.body
}

m4return_t ClProxySpacesSS::Start ()
{
  //## begin ClProxySpacesSS::Start%941744292.body preserve=yes
	// Precondicion: Ya debe estar creado el ClServerProxyManager
	M4_ASSERT(m_pServProxMana);

	// Inicializo las caches del ClServerProxyManager.
	// Lo hago aqui, en vez de en el constructor o el initialize,
	// para estar seguro de que todos los susbsistemas estan creados.
	ClCache*	poCMCRCache			= NULL;
	ClCache*	poCSCRCache			= NULL;
	ClCache*	poPresentationCache	= NULL;
	ClCache*	poMapCache			= NULL;
	ClCache*	poDatacache			= NULL;

//Get MD caches from MetadataSubsystem
	ClSSInterfaz * pMetadataSubsystemInterfaz = (ClSSInterfaz*)m_poNavigator->GetSubsystemByName(ClMetaDataSubsystem::M4MetaDataSubsystemName);
	if (NULL != pMetadataSubsystemInterfaz)
	{
		poCMCRCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache);
		poCSCRCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache);
		poPresentationCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache);
		poMapCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDMapCache);
	}
	else
	{
		// Cambiar el coderr
		SETCODEF(M4_SRV_SS_OBL_ERROR, ERRORLOG, "Cannot find <%s> Subsystem, in Proxy Spaces Subsystem", ClMetaDataSubsystem::M4MetaDataSubsystemName);
	}

	// Search for the caches in the obl & navigator 
	size_t		iLenValue = 0;
	m4char_t	szValue [OBL_PROPVALUE_MAX_LEN];
	
	m4return_t	iRet = GetConfigStringValue( M4_PROXYSUBSYSTEM_DATACACHE, szValue, sizeof(szValue), iLenValue);

	if (iRet == M4_SUCCESS)
	{
		poDatacache = (ClCacheTest*)m_poNavigator->GetSubsystemElement(szValue , "CACHE");
		if (poDatacache == NULL)
			SETCODEF ( M4_SRV_SS_CACHE_ERROR , ERRORLOG , "Cannot find cache <%s>, in Proxy Spaces Subsystem" , szValue ) ;
	}
	else
	{
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", M4_PROXYSUBSYSTEM_DATACACHE << GetName () );
//		SETCODEF(M4_SRV_SS_OBL_ERROR, ERRORLOG, "Cannot find OblProp <%s>, in Proxy Spaces Subsystem", M4_PROXYSUBSYSTEM_DATACACHE);
		iRet = M4_SUCCESS;
	}

	// Now we may have the pointers to the caches
	m_pServProxMana->SetCaches(poCMCRCache, poCSCRCache, poPresentationCache, poMapCache, poDatacache);

	// Llamo al start de mi padre
	if (iRet == M4_SUCCESS)
		iRet = ClSSImplementation::Start ();

	return iRet;
  //## end ClProxySpacesSS::Start%941744292.body
}

m4return_t ClProxySpacesSS::Synchronize ()
{
  //## begin ClProxySpacesSS::Synchronize%941744294.body preserve=yes
	// releases all pending spaces
	return m_pServProxMana->ReleaseSpace((m4uint32_t)0);
  //## end ClProxySpacesSS::Synchronize%941744294.body
}

void ClProxySpacesSS::Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject)
{
  //## begin ClProxySpacesSS::Update%942076256.body preserve=yes
	m4uint32_t iSessionId = 0 , iRequestId = 0 ;

	switch (ai_iEvent )
	{
	case CLOSE_SESSION :

		// releases the space
		iSessionId = ( (ClSSImplementation*) ai_oSubject ) ->GetId();

		// ignores event in this release
	#ifdef _DEBUG
	//	cout << "Warning: ignoring desconnection event for session id " << iSessionId << endl;
		cout << "Warning: desconnection event for session id " << iSessionId << endl;
	#endif
	//		return;    /////// IGNORES

	//	M4_ASSERT(iSessionId);
		if (!iSessionId)
		{
	#ifdef _DEBUG
			cout << "Warning: Event returns NULL Sessiond id" << endl;
	#endif
			return ;
		}
		
		break ;
	case REQUEST_CANCELED :

		iSessionId = ( (ClCancelRequestEvent *) ai_oSubject ) -> GetSessionId () ;

		iRequestId = ( (ClCancelRequestEvent *) ai_oSubject ) -> GetRequestId () ;

		break ;
	default :
		return;

	}

	m_pServProxMana->ReleaseSpace(iSessionId,iRequestId);
  //## end ClProxySpacesSS::Update%942076256.body
}

m4return_t ClProxySpacesSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClProxySpacesSS::GetCommandsId%942076257.body preserve=yes
	// inserts commands for this subsystem
	ClAdminTag *newCommand;
	
	newCommand = ClAdminTag::GetNewAdminTag(DESTROY_PX_SPACE_BY_SESSION_ID, "DESTROY_PX_SPACE_BY_SESSION_ID", M4_TRUE);
	M4_ASSERT(newCommand != NULL);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(COMMAND_ADD_NEW_PROXY_SPACE, "COMMAND_ADD_NEW_PROXY_SPACE", M4_TRUE);
	M4_ASSERT(newCommand != NULL);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);
	
	return  M4_SUCCESS;
  //## end ClProxySpacesSS::GetCommandsId%942076257.body
}

void * ClProxySpacesSS::GetElement (m4pchar_t ai_szElement)
{
  //## begin ClProxySpacesSS::GetElement%942403829.body preserve=yes
	if ( ! ai_szElement )
		return NULL ;


	if ( ! stricmp ( ai_szElement , "SERVER_PROXY_MANAGER" ) )
		return m_pServProxMana;

	return NULL ;
  //## end ClProxySpacesSS::GetElement%942403829.body
}

ClServerProxySpaceSS* ClProxySpacesSS::CreateServerProxySpaceSS ()
{
  //## begin ClProxySpacesSS::CreateServerProxySpaceSS%943008105.body preserve=yes
	// Preparo la configuaracion (como si la leyera del startup.obl)
	M4ClString strConfig = "TYPE=\"PROXY_SPACE\";STRINGID=\"Proxy Space\";LAUNCHER_USER_ID=\"NO USER\";LAUNCHER_SESSION_ID=\"0\"";
	ClConfiguration* pConfiguration = new ClM4ConfigTree(strConfig);
	if (pConfiguration == NULL)
		return NULL;

	ClServerProxySpaceSS* pProxySpaceSS =
		new ClServerProxySpaceSS(m_pServProxMana, pConfiguration, m_poTables, m_poNavigator);
	if (pProxySpaceSS == NULL)
		return NULL;

	// Creamos un interfaz para el SS y se lo pasamos al ClServerProxySpaceSS
	ClSSInterfaz* pInterfaz = new ClSSInterfaz(pProxySpaceSS);
	if (pInterfaz == NULL)
		return NULL;
	pProxySpaceSS->SetInterfaz(pInterfaz);

	// Añadimos el interfaz del SS hijo al arbol del SS padre
	if (AddSubsystem(pInterfaz) != M4_SUCCESS) 
		return NULL;

	// Inicializamos el SS hijo
	if (pProxySpaceSS->Initialize() != M4_SUCCESS) 
		return NULL;

	if (pProxySpaceSS->Init(pConfiguration) != M4_SUCCESS) 
		return NULL;

	if (pProxySpaceSS->Start() != M4_SUCCESS) 
		return NULL;

	return pProxySpaceSS;
  //## end ClProxySpacesSS::CreateServerProxySpaceSS%943008105.body
}

m4return_t ClProxySpacesSS::IncrementMaxNSpaces (const m4int32_t ai_iIncrement)
{
  //## begin ClProxySpacesSS::IncrementMaxNSpaces%945276897.body preserve=yes
	// Leo de la configuracion el numero de conexiones reservadas para el resto del servidor.
	m4double_t dNumber;
	m4return_t iRet = GetConfigNumericValue( M4_PROXYSPACES_SS_NON_PROXY_DDBB_CONECTION_COUNT, dNumber);
	if (iRet != M4_SUCCESS)
	{
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", M4_PROXYSPACES_SS_NON_PROXY_DDBB_CONECTION_COUNT << GetName () );
//		SETCODEF(M4_SRV_SS_OBL_ERROR, ERRORLOG, "Cannot find OblProp <%s>, in Proxy Spaces Subsystem", M4_PROXYSPACES_SS_NON_PROXY_DDBB_CONECTION_COUNT);
		return iRet;
	}
	m4uint32_t iNonProxyDDBBConectionCount = (m4int32_t)dNumber;
	if (iNonProxyDDBBConectionCount < 0)
	{
		// El numero que se ha leido de la obl es negativo
		return M4_ERROR;
	}
	
	// Calculo el numero de conexiones que puedo utilizar para espacios proxy.
	ILdb* pLdb = ClFactM4ObjService::GetpLDB();
	if (!pLdb) return M4_ERROR;

	m4uint32_t	iMaxNumConn			= pLdb->GetNumConn();
	m4int32_t	iActualMaxNSpaces	= m_pServProxMana->GetMaxNSpaces();
	m4int32_t	iNewMaxNSpaces		= iActualMaxNSpaces + ai_iIncrement;
	m4int32_t	iAvailableConexions	= iMaxNumConn - iNonProxyDDBBConectionCount - iActualMaxNSpaces;
	
	// Compruebo que el formato del parametro introducio por el usuario es correcto.
	if (ai_iIncrement <= 0)
	{
		DUMP_CHLOG_ERRORF(M4_ERR_PROXY_SPACES_TO_ADD_BAD_FORMAT, iAvailableConexions);
		return M4_ERROR;
	}
	
	// Precondicion: No puedo añadir mas espacios proxy del numero de conexiones disponibles a BBDD.
	// Reservando un numero de conexiones (especificado en el startup.obl) para el resto del sistema.
	if (iAvailableConexions - ai_iIncrement >= 0)
	{
		m_pServProxMana->SetMaxNSpaces(iNewMaxNSpaces);
		
		// Cambio el valor del atributo para que se muestre correctamente en el monitor.
		iRet = SetConfigNumericProperty( M4_PROXYSPACES_SS_MAXNSPACES, iNewMaxNSpaces);
		if (iRet != M4_SUCCESS)
			SETCODEF(M4_SRV_SS_OBL_ERROR, ERRORLOG, "Can't reset property value <%s>", M4_PROXYSPACES_SS_MAXNSPACES);
	}
	else
	{
		// No se pueden crear mas proxy spaces porque no se dispone de mas conexiones a BBDD.
		DUMP_CHLOG_ERRORF(M4_ERR_PROXY_CAN_NOT_CREATE_MORE_SPACES, iAvailableConexions);
		iRet = M4_ERROR;
	}

	return iRet;
  //## end ClProxySpacesSS::IncrementMaxNSpaces%945276897.body
}

m4return_t ClProxySpacesSS::Initialize ()
{
  //## begin ClProxySpacesSS::Initialize%945276898.body preserve=yes
	// Limitamos el numero de ClProxySpaces.
	m4double_t dNumber;
	m4int32_t  iMaxNSpaces = -1;

	if ( GetConfigNumericValue(  M4_PROXYSPACES_SS_MAXNSPACES, dNumber) == M4_SUCCESS)
	{
		iMaxNSpaces = (m4int32_t)dNumber;
		if (iMaxNSpaces < 1)
		{
			M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , 
				"Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s",
				M4_PROXYSPACES_SS_MAXNSPACES
				<< GetName () << iMaxNSpaces << 0 << "any" << "no limit" );

			iMaxNSpaces = -1;
//			SETCODEF(M4_SRV_SS_OBL_ERROR, ERRORLOG, "Invalid value for OblProp <%s> = %d: no limit assumed, in Proxy Subsystem", M4_PROXYSPACES_SS_MAXNSPACES, iMaxNSpaces);
		}
	}
	else
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", M4_PROXYSPACES_SS_MAXNSPACES<< GetName () );
//		SETCODEF(M4_SRV_SS_OBL_ERROR, ERRORLOG, "Cannot find OblProp <%s>, in Proxy Spaces Subsystem", M4_PROXYSPACES_SS_MAXNSPACES);

	m_pServProxMana->SetMaxNSpaces(0);
	return IncrementMaxNSpaces(iMaxNSpaces);
  //## end ClProxySpacesSS::Initialize%945276898.body
}

m4return_t ClProxySpacesSS::ReleaseActiveSpace (const m4char_t* const ai_pcClProxySpaceID)
{
  //## begin ClProxySpacesSS::ReleaseActiveSpace%956737462.body preserve=yes
	if (NULL != m_pServProxMana)
		return m_pServProxMana->ReleaseActiveSpace(ai_pcClProxySpaceID);

	return M4_ERROR;
  //## end ClProxySpacesSS::ReleaseActiveSpace%956737462.body
}

void ClProxySpacesSS::Reset ()
{
  //## begin ClProxySpacesSS::Reset%994184752.body preserve=yes
  //## end ClProxySpacesSS::Reset%994184752.body
}

m4return_t ClProxySpacesSS::EraseAllL2Instances (const m4char_t* const ai_pcClProxySpaceID, m4uint32_t ai_iRemoteId)
{
  //## begin ClProxySpacesSS::EraseAllL2Instances%1025261934.body preserve=yes
	return m_pServProxMana->EraseAllL2Instances (ai_pcClProxySpaceID, ai_iRemoteId);
  //## end ClProxySpacesSS::EraseAllL2Instances%1025261934.body
}

// Additional Declarations
  //## begin ClProxySpacesSS%38229C71036A.declarations preserve=yes
  //## end ClProxySpacesSS%38229C71036A.declarations

//## begin module%3822A11B03C1.epilog preserve=yes
//## end module%3822A11B03C1.epilog
