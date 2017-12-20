//## begin module%34C86C1C0035.cm preserve=no
//## end module%34C86C1C0035.cm

//## begin module%34C86C1C0035.cp preserve=no
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
//## end module%34C86C1C0035.cp

//## Module: userss%34C86C1C0035; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\userss.cpp

//## begin module%34C86C1C0035.additionalIncludes preserve=no
//## end module%34C86C1C0035.additionalIncludes

//## begin module%34C86C1C0035.includes preserve=yes
#include <m4srvres.hpp>
#include <subsystemids.hpp>
#include <ssnames.hpp>
#include <connects.hpp>
#include <cllocalmonitor.hpp>
#include <ussession.hpp>
#include <clsubsystem.hpp>
#include <logoninstance.hpp>
#include <request.hpp>
#include <ususer.hpp>
#include <basepdus.hpp>
#include <subsystems.hpp>
#include <m4eventhandlers.hpp>
#include <clssinterfaz.hpp>
#include <clssinvoker.hpp>
#include <blocksynchronization.hpp>
#include <m4credentials.hpp>
#include <rawsessionid.hpp>
#include <m4actionconstants.hpp>
#include <m4srvtrace.h>
#include <usservice.hpp>
//## end module%34C86C1C0035.includes

// syncro
#include <syncro.hpp>
// Statistics
#include <statistics.hpp>
// userss
#include <userss.hpp>
// sessiondic
#include <sessiondic.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
// clssaction
#include <clssaction.hpp>
// SessionIDGenerator
#include <sessionidgenerator.hpp>

class ClLogonInterface;
class ClUSUserInterface;
class ClUSSessionInterface;
class ClCacheTest;

//## begin module%34C86C1C0035.declarations preserve=no
//## end module%34C86C1C0035.declarations

//## begin module%34C86C1C0035.additionalDeclarations preserve=yes
//Variable global para el timeout de cliente
#define M4_USERSS_END_USER_SESSION		10
#define		M4_USSERSS_END_USER_SESSION_ARG_1 01
#define		SEPARATOR_END_USERS_SESSION_ARG_1 '.'
#define MAX_TIMEOUT 480
#define MIN_TIMEOUT 1

#define M4_CS_EVENT_SERVER_SHUTDOWN				1
#define M4_CS_EVENT_SERVER_RUPTURE_CONNECTION	2
#define M4_CS_EVENT_SESSION_TIMEOUT				3
#define M4_CS_EVENT_ADMIN_MESSAGE				4

m4double_t g_dSessoinTimeOut = 0;

#define M4_UUSS_TIMEOUT						0x0055 + M4_SRV_UUSS_ERROR_BASE //	Unknown


#define M4_SRV_TRACE_COMP_ID	"COMSRV"
#define USS_SHUTDOWN_TIMEOUT	60


#ifdef _DEBUG
extern m4uint32_t		g_uiPDUSInside;
#endif

//## end module%34C86C1C0035.additionalDeclarations


// Class ClUsersSS 






ClUsersSS::ClUsersSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClUsersSS::ClUsersSS%890993640.hasinit preserve=no
      : m_oUsersMutex(M4_TRUE)
  //## end ClUsersSS::ClUsersSS%890993640.hasinit
  //## begin ClUsersSS::ClUsersSS%890993640.initialization preserve=yes
  ,ClSSImplementation ( ai_poConfiguration, ai_poTables, ai_poNavigator )
  //## end ClUsersSS::ClUsersSS%890993640.initialization
{
  //## begin ClUsersSS::ClUsersSS%890993640.body preserve=yes
	m_pUSCache = NULL ;
	SetState(  SS_NOT_ACCESIBLE , SS_STR_NOT_ACCESIBLE);
  //## end ClUsersSS::ClUsersSS%890993640.body
}


ClUsersSS::~ClUsersSS ()
{
  //## begin ClUsersSS::~ClUsersSS%890993641.body preserve=yes
  //## end ClUsersSS::~ClUsersSS%890993641.body
}



//## Other Operations (implementation)
void ClUsersSS::Accept (ClUsersSSVisitor &ai_poVisitor)
{
  //## begin ClUsersSS::Accept%885550068.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return  ;
	}

		ai_poVisitor.VisitUsersSS ( *this ) ;
  //## end ClUsersSS::Accept%885550068.body
}

m4return_t ClUsersSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClUsersSS::ExecuteCommand%910862257.body preserve=yes
	ClAdminCommandTag *newParam;
	m4char_t szParam [20];
	m4uint32_t iNumUsers ;
	m4uint32_t iNumSessions ;
	m4uint32_t iTimeout ;
	ClAdminTagList::iterator it;
	m4return_t ret = M4_SUCCESS;
	ClSSInterfaz *pSessionSub;
	ClAdminTagList::iterator pArg ;
	m4pchar_t pszIdSession , szTiemout ;
	m4pchar_t szMessage;
//	ClMutBlock  oMutex( &m_oUsersMutex );
	M4ClString szParams = "";
	m4char_t szUser [ OBL_PROPVALUE_MAX_LEN] = "" ;
	m4size_t iLen ;


	switch  ( ai_cCommand )
	{

		case TURN_ON_USER_TIMEOUT :

			pArg = find_if (	ai_oParamList.begin() , ai_oParamList.end () , search_by_tag_id ( 0) ) ;

			if ( pArg == ai_oParamList.end () )
			{
				return M4_ERROR;
			}
			if( ! ( szTiemout = ( (ClAdminTag *)(*pArg) ) -> m_tagValue ) )
				return M4_ERROR ;
			if( ! sscanf ( szTiemout , "%d" , &iTimeout  ) )
			{
				return M4_ERROR ;
			}

			g_dSessoinTimeOut = iTimeout ;

			if ( ( g_dSessoinTimeOut <MIN_TIMEOUT) || (g_dSessoinTimeOut>MAX_TIMEOUT ) )
			{
				if ( g_dSessoinTimeOut <MIN_TIMEOUT)
				{
					g_dSessoinTimeOut = MIN_TIMEOUT ;
				}
				if ( g_dSessoinTimeOut >MAX_TIMEOUT )
				{
					g_dSessoinTimeOut = MAX_TIMEOUT ;
				}
				
				M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s",USER_TIMEOUT << "USERSS" << iTimeout << MIN_TIMEOUT << MAX_TIMEOUT
					<< g_dSessoinTimeOut);
				
			}
			
			ClUSSession::SetTimeout ( g_dSessoinTimeOut ) ;

			SetConfigNumericProperty ( "USER_TIMEOUT_MN" , g_dSessoinTimeOut ) ;
			SetConfigStringProperty ( "USER_TIMEOUT" , "ENABLED" ) ;

			break ;
		case TURN_OFF_USER_TIMEOUT :
			g_dSessoinTimeOut = 0 ;
			ClUSSession::SetTimeout ( g_dSessoinTimeOut ) ;
			SetConfigStringProperty ( "USER_TIMEOUT" , "DISABLED" ) ;
			break ;

		case M4_USERSS_END_USER_SESSION :


				pArg = find_if (	ai_oParamList.begin() , 
			 						ai_oParamList.end () ,
									search_by_tag_id ( M4_USSERSS_END_USER_SESSION_ARG_1 ) ) ;

				if ( pArg == ai_oParamList.end () )
				{
					M4_SRV_ERROR ( M4_SRV_SS_COMMAND_PARAM_ERROR , "Parameter '%0:s' not found executing command '%1:s' in subsystem '%2:s'" , "SESSION ID" << "CLOSE SESSION" << GetName () ) ;

					//// xxxxxx Devolver a la Log un error de que no encuentra el parametro

					return M4_ERROR;

				}

					pszIdSession = ( (ClAdminTag *)(*pArg) ) -> m_tagValue;

				/// pIdSession = ID_SESSION
				m4objid_t lIdSession;

				lIdSession = atol ( pszIdSession );

					

				if ( lIdSession == 0 )
				{

					/// XXXXX. Devuelve Error porque el identificador a sido erroneo.

					return M4_ERROR;
				}

				//Buscar el SS poara obtener la propiedad username
				pSessionSub = FindSubsystemById ( lIdSession ) ;

				if ( ! pSessionSub )
				{
					return M4_ERROR ;
				}

				//La propiedad se busca en el objeto configuracion
				if ( M4_ERROR == pSessionSub -> GetConfigStringValue ( USERNAME , szUser , OBL_PROPVALUE_MAX_LEN , iLen) )
				{
					return M4_ERROR ;
				}

				//Si el comando se ejecuta con exito se publica el evento
				if ( M4_SUCCESS == ( ret = EndUserSession ( lIdSession , 0) ) )
				{

					szParams += szUser ;
					szParams += "#" ;
					szParams += lIdSession ;
					szParams += "#" ;

					M4PublishEvent("65548", szParams) ;
				
				}

				break;
		case CHECK_TIMEOUT_SESSIONS :

			ret = CheckTimeoutSessions () ;

			break;

		case SEND_MESSAGE_TO_ALL_USERS_SESSIONS :
		
			it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
			if (it == ai_oParamList.end())
			{
				return M4_ERROR;
			}
					// converts the value
			szMessage = ((*it)->m_tagValue) ;

			ret = SendMessageToAllUsers ( szMessage ) ;

			break ;
		case ESTIMATE_LOAD :

			//Numero de usuarios
			iNumUsers = ((ClUsersSSStats *) GetStats() )-> GetCurrentSessions () ;
			sprintf ( szParam , "%d" , iNumUsers ) ;

			newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "NUM_USERS", adminLong, szParam );

			if (newParam == NULL) 
				return M4_ERROR;
			ao_oResultList.push_back(newParam);

			//numero de sessiones

//**** Esta estadística da el total de sesiones desde el ARRANQUE DEL SERVER NO ACTUALMENTE CONECTADAS *//
//			iNumSessions = ((ClUsersSSStats *) GetStats() )-> GetnSessions () ;
			iNumSessions = m_oSessRepository.GetNumItems ( ) ;
			sprintf ( szParam , "%d" , iNumSessions ) ;

			newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "NUM_SESSIONS", adminLong, szParam );

			if (newParam == NULL) 
				return M4_ERROR;
			ao_oResultList.push_back(newParam);
			break;
		case CLOSE_ALL_SESSIONS :

			ret = _CloseAllSessions () ;

			break ;

		default:
			

			M4_SRV_ERROR ( M4_SRV_COMMAND_ID_ERROR, "Error Executing command in '%0:s' Subsystem. Unknown command id: %1:d" , GetName() << ai_cCommand ) ;
			
			ret = M4_ERROR;

	}

	return ret;

  //## end ClUsersSS::ExecuteCommand%910862257.body
}

m4return_t ClUsersSS::Initialize ()
{
  //## begin ClUsersSS::Initialize%885891302.body preserve=yes
	m4return_t iRet ;
	m4char_t szTimeout [OBL_PROPVALUE_MAX_LEN];
	g_dSessoinTimeOut = 0 ;
	m4double_t orgTimeout ;
	m4size_t iLen ;


	m_poVisitor = new ClUsersSSVisitor ;

	m_poStatistics = new ClUsersSSStats ;

	if ( M4_ERROR == GetConfigStringValue ( USER_TIMEOUT , szTimeout , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{
		M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", USER_TIMEOUT << "USERSS" );
//		SETCODEF ( M4_SRV_SS_OBL_ERROR , WARNINGLOG , "USER_TIMEOUT property not defined. No timeout assumed" ) ;
	}
	else
	{
		if ( ! stricmp ( szTimeout , DISABLED ) ) //Si está a DISABLE no se hace nada
		{
			SETCODEF ( M4_UUSS_TIMEOUT , DEBUGINFOLOG , "Inactivity User Timeout disabled. No timeout assumed" ) ;
		}
		else
		{
			if ( stricmp ( szTimeout , ENABLED ) ) //Si tampoco está a ENABLE se da un error
			{
				SETCODEF ( M4_UUSS_TIMEOUT , ERRORLOG , "Inactivity User Timeout neither ENABLED nor DISABLED. No timeout assumed" ) ;
			}
			else
			{

				iRet = GetConfigNumericValue ( USER_TIMEOUT_MN , orgTimeout  ) ;

				g_dSessoinTimeOut = orgTimeout ;

				if ( iRet == M4_SUCCESS )
				{
					if ( ( g_dSessoinTimeOut <MIN_TIMEOUT) || (g_dSessoinTimeOut>MAX_TIMEOUT ) )
					{
						if ( g_dSessoinTimeOut <MIN_TIMEOUT)
						{
							g_dSessoinTimeOut = MIN_TIMEOUT ;
						}
						if ( g_dSessoinTimeOut >MAX_TIMEOUT )
						{
							g_dSessoinTimeOut = MAX_TIMEOUT ;
						}

						M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s",USER_TIMEOUT << "USERSS" << orgTimeout<< MIN_TIMEOUT << MAX_TIMEOUT
<< g_dSessoinTimeOut);
//						SETCODEF ( M4_SRV_SS_OBL_ERROR , WARNINGLOG  , "Invalid User Timeout defined. Correct Values (%d - %d)minutes: -> %d assumed" , MIN_TIMEOUT, MAX_TIMEOUT , g_dSessoinTimeOut);
		
					}

					ClUSSession::SetTimeout ( g_dSessoinTimeOut ) ;
				}
			}
		}
	}

	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;
	 
	poInvoker -> RegisterAction (  NEW_USER_ACTION , NEW_USER_ACTION_ID , this ) ;
	poInvoker -> RegisterAction (  END_USER_SESSION_ACTION , END_USER_SESSION_ACTION_ID , this ) ;
	poInvoker -> RegisterAction (  RECOVER_SESSION_ACTION ,RECOVER_SESSION_ACTION_ID , this ) ;

	m_poCredentialGenerator = new ClRawSessionIDGenerator ;


	return M4_SUCCESS ;
  //## end ClUsersSS::Initialize%885891302.body
}

m4return_t ClUsersSS::ShutDown ()
{
  //## begin ClUsersSS::ShutDown%885891304.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE != eState )
	{
		SetState ( SS_NOT_ACCESIBLE , SS_STR_NOT_ACCESIBLE) ;
	}

	return M4_SUCCESS;
  //## end ClUsersSS::ShutDown%885891304.body
}

m4return_t ClUsersSS::Synchronize ()
{
  //## begin ClUsersSS::Synchronize%886518768.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return M4_ERROR ;
	}

	if ( m_poVisitor )
	{
		Accept ( *(ClUsersSSVisitor*) m_poVisitor ) ;
	}

	return ClSSImplementation::Synchronize() ;
  //## end ClUsersSS::Synchronize%886518768.body
}

ClUSUserInterface * ClUsersSS::GetUserAccount (m4objid_t ai_lUserId)
{
  //## begin ClUsersSS::GetUserAccount%893085117.body preserve=yes
	ClUSUserInterface * poUser ;
	ClSSInterfaz  * poSubsystem ;
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return NULL ;
	}

	if ( ! ( poSubsystem = FindSubsystemById ( ai_lUserId ) ) )
	{
		return NULL ;
	}

	poUser = (ClUSUserInterface *)  poSubsystem -> GetElement ( "USER" ) ;

	return poUser ;
  //## end ClUsersSS::GetUserAccount%893085117.body
}

m4return_t ClUsersSS::EndUserSession (m4objid_t ai_lIdSession, m4objid_t ai_lIdUser)
{
  //## begin ClUsersSS::EndUserSession%893085120.body preserve=yes
//	m4objid_t lIdSession;
	ClUSSessionInterface * poSession ;
	m4return_t iRet ;
	ClSSInterfaz  * poUserSubsystem = NULL , * poSessionSubsystem = NULL , * poSubsystem;
	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;
	vector <m4objid_t> vSessions ;
	eStatus eState ;
	INTERFACESMAP::iterator Iterator ;
	m4char_t szId [20] ;
	m4objid_t idUser = 0 ;
	m4string_t strUserName ;



	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return M4_ERROR ;
	}

	if ( ! ai_lIdSession )
	{
		return M4_ERROR ;
	}
		
	m_oSessRepository.RemoveItem ( ai_lIdSession ) ;

	//Borra el subsistema session del subsistema usuario que lo contiene
	m_oSyncBellowLevel.EnterReader ();


	if ( ai_lIdUser )
	{
		Iterator = m_oBellowLevel.find ( ai_lIdUser ) ;

		if ( Iterator != m_oBellowLevel.end () )
		{
			poUserSubsystem = (*Iterator).second ;
		}

	}
	else 
	{
		for ( Iterator = m_oBellowLevel.begin () ; Iterator != m_oBellowLevel.end () ; Iterator ++ )
		{
			poSubsystem = (*Iterator).second ;
			
			if ( poSubsystem )
			{
				poSessionSubsystem = poSubsystem -> FindSubsystemById ( ai_lIdSession ) ;
				
				if ( poSessionSubsystem  )
				{
					if (  poSession = (ClUSSessionInterface *) poSessionSubsystem -> GetElement ( "SESSION" ) ) 
					{
						if ( poSession -> GetUser () -> GetId ()  == poSubsystem -> GetId () )
						{

							poUserSubsystem = poSubsystem  ;
							break ;
						}
					}
				}
			}
		}
	}


	if ( ! poUserSubsystem )
	{
		m_oSyncBellowLevel.LeaveReader ();

		return M4_ERROR ;
	}


	if ( M4_SUCCESS == ( iRet = poUserSubsystem -> RemoveSubsystem ( ai_lIdSession ) ) )
	{
		//Actualizar el contador de sesiones
		if ( poUserSubsystem ->GetStats() )
		{
			((ClUSUserStats*)poUserSubsystem ->GetStats() ) -> AddnTotalSessions ( -1 ) ;
		}

		if ( GetStats () )
		{
			( ( ClUsersSSStats*) GetStats () ) -> AddCurrentSessions ( -1 ) ;
		}
		/*Aquí se debería sincronizar al usuario */
	
		idUser = poUserSubsystem -> GetId ()  ;
		
		sprintf ( szId , "%ld" , (m4uint32_t) idUser ) ;
		
		poUserSubsystem -> GetSubsystemsId ( szId , vSessions ) ;
	}		

	m_oSyncBellowLevel.LeaveReader ();
		
	if ( vSessions.size ( ) == 0 && idUser > 0 )
	{
		RemoveSubsystem ( idUser ) ;
	}


	return iRet	;
  //## end ClUsersSS::EndUserSession%893085120.body
}

void * ClUsersSS::GetElement (m4pchar_t ai_szElement)
{
  //## begin ClUsersSS::GetElement%899883297.body preserve=yes
	ClLogonInterface	*logonObj = NULL;

/*	if ( !m_bActive )
		return NULL ;
*/
	if (0 == stricmp(ID_UUSS_LOGON, ai_szElement))
	{
		logonObj = new ClLogonInstance(this);

		return (void *)logonObj;
	}

	if ( ! stricmp ( "CACHE" , ai_szElement ) )
	{
		return m_pUSCache ;
	}

	return NULL ;
  //## end ClUsersSS::GetElement%899883297.body
}

m4return_t ClUsersSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oCommnadList)
{
  //## begin ClUsersSS::GetCommandParams%910862259.body preserve=yes
	eStatus eState ;
	ClAdminCommandTag * newParam ;

	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return M4_ERROR ;
	}

	switch ( ai_cCommand )
	{
	case SEND_MESSAGE_TO_ALL_USERS_SESSIONS :
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "MESSAGE", adminText, "Meta4 Admin Message" );

		if (newParam == NULL) 
			return M4_ERROR;
		aio_oCommnadList.push_back(newParam);
		break;
	case ESTIMATE_LOAD :
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "NUM_USERS", adminLong, "Number of users conected" , M4_TRUE);

		if (newParam == NULL) 
			return M4_ERROR;
		aio_oCommnadList.push_back(newParam);

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(1, "NUM_SESSIONS", adminLong, "Number of sessions currently established" , M4_TRUE );

		if (newParam == NULL) 
			return M4_ERROR;
		aio_oCommnadList.push_back(newParam);
		break;
	case TURN_ON_USER_TIMEOUT :
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "USER_TIMEOUT_MN", adminLong, "15" );

		if (newParam == NULL) 
			return M4_ERROR;
		aio_oCommnadList.push_back(newParam);

		break ;
	}

	return M4_SUCCESS ;
  //## end ClUsersSS::GetCommandParams%910862259.body
}

m4return_t ClUsersSS::EndUserAccount (m4objid_t ai_iUserId)
{
  //## begin ClUsersSS::EndUserAccount%917285328.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return M4_ERROR ;
	}

	return M4_SUCCESS;
  //## end ClUsersSS::EndUserAccount%917285328.body
}

m4objid_t ClUsersSS::AddUserSession (ClUSUserInterface * &ai_poUser, ClUSSessionInterface *ai_poSession)
{
  //## begin ClUsersSS::AddUserSession%893085118.body preserve=yes
	m4return_t iRet ;
	m4objid_t lIdUser  , lIdSession , lId ;
	m4pchar_t szHost = "Unknown" ; 
	m4pchar_t sessionStringId = "Session" ; 
	ClSystem * poSystem = NULL ;
	ClUsersSSStats * poStats = NULL ;
	ClSSInterfaz  * poSessionSubsystem , * poUserSubsystem ,  * poSubsystem = NULL ;
	static ClSSInterfaz  * poProxySubsystem = NULL , * poXMLSubsystem = NULL, * poLDBSubsystem = NULL ;
	ClConfiguration * poConfiguration , *poUserConfiguration ;
	ClConnInstance * poConnection = NULL ;
	eConnProtocol eProtocol ;
	M4ClString szsessionSartDate ;
	M4ClTimeStamp clock ;
	m4double_t lTime;
//	ClConnection * poConnection ;
	eStatus eState ;
	ClLocalMonitor * poLocalMonitor ;
	INTERFACESMAP::iterator Iterator ;
	m4string_t		strUsername , strUserID ;
	m4string_t		strIP = "";
	size_t iLen ;
	m4char_t szUserId [100] ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return M4_ERROR ;
	}

	if ( ! ai_poSession ) 
	{
		return M4_ERROR ;
	}

	poSystem = ClSystem::Instance ( );
	poLocalMonitor = ClLocalMonitor::Instance () ;

	//Asociar el observador Proxy subsystem al sujeto session
	if ( ! poProxySubsystem ) 
		poProxySubsystem  = poSystem -> FindSubsystemByType ( M4_PROXY_SPACES_SUBSYSTEM ) ;

	//Asociar el observador XML subsystem al sujeto session
	if ( ! poXMLSubsystem )
		poXMLSubsystem= poSystem -> FindSubsystemByType ( M4_XML_SUBSYSTEM ) ;

	//Asociar el observador LDB subsystem al sujeto session. De esta forma podremos notificar un cierre de conexion
	//a la LDB.
	if ( ! poLDBSubsystem )
		poLDBSubsystem= poSystem -> FindSubsystemByType ( M4_LDB_SUBSYSTEM ) ;

	poConnection = ( ClConnection * )ai_poSession -> GetConnection () ;

	eProtocol = ai_poSession -> GetConnProtocol () ;

	if ( (! poConnection) && ( M4_NON_CONNECTION_ORIENTED_PROTOCOL != eProtocol ) )
	{
		return M4_SUCCESS ;
	}
	
	if ( M4_CONNECTION_ORIENTED_PROTOCOL == eProtocol )
	{
		if ( (poConnection -> GetStatus () ) != ACTIVE )
		{
			ClLocalMonitor * poLocalMonitor = ClLocalMonitor::Instance ( ) ;
	
			if ( poLocalMonitor -> GetSessionDic ( ) ) 
			{
				poLocalMonitor -> GetSessionDic ( ) -> AddItem ( ai_poSession ) ;
			}
				
			return M4_ERROR ;
		}
	}


	ai_poUser -> GetName ( strUsername ) ;
	if (strUsername.empty())
	{
		return M4_ERROR ;
	}

	ai_poUser->GetUserId( strUserID );

//Para saber los ids de los usuarios me construyo el path del subsystema de usuarios
//	sprintf ( szPath + 2 , "%d" , GetId () ) ;

	poSystem = ClSystem::Instance () ;

	m_oSyncBellowLevel.EnterReader ();

	Iterator = m_oBellowLevel.begin () ;

	do
	{
		if ( poUserSubsystem = (*Iterator).second )
		{
			poUserConfiguration = poUserSubsystem -> GetConfiguration () ;

			if ( poUserConfiguration )
			{
				// bg 210771
				if ( M4_SUCCESS == poUserConfiguration -> GetStringValue ( "" , "USERID" , szUserId , 100 , iLen ) )
				{
					if (! stricmp ( szUserId, strUserID.c_str()) )
					{
						ClUSUserInterface * poUser = (ClUSUserInterface*) poUserSubsystem -> GetElement ( "USER" ) ;

						if ( poUser )
						{
							poSubsystem = poUserSubsystem ;
							
							lId = poSubsystem -> GetId () ;
							
							if ( ai_poUser -> GetStats() )
							{
								delete ai_poUser -> GetStats () ;
							}
							
							delete ai_poUser ;
							
							ai_poUser = poUser ;
						}
					}
				}
			}
		}

		Iterator ++ ;

	}while ( (! poSubsystem ) && ( Iterator != m_oBellowLevel.end () ) );

	while (! poSubsystem )
	{
		m_oSyncBellowLevel.LeaveReader ();

		lId = AddUserAccount ( ai_poUser ) ;

		m_oSyncBellowLevel.EnterReader ();

		Iterator = m_oBellowLevel.find ( lId ) ;

		if ( Iterator != m_oBellowLevel.end () )
		{
			poSubsystem = (*Iterator).second ;
		}
	}

	poUserSubsystem = poSubsystem ;

	lIdSession = ai_poSession -> GetId ();

	lIdUser = ai_poUser -> GetId ( ) ;

	ai_poUser -> GetUserId (strUserID) ;

	ai_poSession -> SetUser ( ai_poUser ) ;

	 ++ (ai_poUser -> m_oNumSessions ) ;

	//Crear la configuracion del subsistema de sesion
	poConfiguration = new ClPropertiesConfiguration ( PROPSESSIONCONFIGURATION ) ;


	poConfiguration -> AddStringProperty ( "" , USERNAME , (char *)strUsername.c_str()) ;

	//Genera el subsystema de sesion
	poSessionSubsystem = m_poSSFactory -> GenerateSubsystem ( M4_SESSION_SUBSYSTEM , poConfiguration , m_poTables) ;

	if (! poSessionSubsystem )
	{
		SETCODEF ( M4_SRV_SS_FACTORY_ERROR , ERRORLOG , "Error generating new session subsystem after a new user connection" ) ;

		m_oSyncBellowLevel.LeaveReader ();
		return M4_ERROR ;
	}
	//Asigna la configuracion al subsistema
	poSessionSubsystem -> SetConfiguration ( poConfiguration ) ;

	poSessionSubsystem -> SetConfiguration ( poConfiguration ) ;

	//Actualiza el estado del subsistema
	poSessionSubsystem -> Activate () ;

	//Añade la propiedad host del subsystema de sesion
	if (  poConnection = ai_poSession -> GetConnection () )
	{
		if ( M4_HTTP_PROTOCOL ==  poConnection  -> GetClientType () )
		{
			poConfiguration -> AddStringProperty ( "" , CLIENT_PROTOCOL ,HTTP_PROTOCOL ) ;
		}
		else
		{
			poConfiguration -> AddStringProperty ( "" , CLIENT_PROTOCOL , META4_PROTOCOL ) ;
		}

	}

	poConfiguration -> AddStringProperty ( "" , "HOSTNAME" , (m4pchar_t)ai_poSession ->GetHost () ) ;

	//Añade la propiedad STRINGID del subsystema de sesion (Herramienta administrativa)
	poConfiguration -> AddStringProperty ( "" , "STRINGID" , sessionStringId ) ;

	//Añadir el Id de la sesión
	poConfiguration -> AddNumericProperty ( "" , "SESSIONID" , (m4int64_t) lIdSession) ;

	//Añade la fecha de creación de la session
	clock.now ( ) ;
	clock.get ( szsessionSartDate ) ;
	clock.get ( lTime ) ;
	
	((ClUSSessionStats*)ai_poSession -> GetStats ()) -> SettLastTransaction ( lTime );

	poConfiguration -> AddStringProperty ( "" , "START_DATE" , szsessionSartDate ) ;

	//Asignar la propiedad del tipo de protocolo del cliente
	

	//Asigna el Id
	poSessionSubsystem -> SetId ( lIdSession , poUserSubsystem -> GetPath () ) ;

	poSessionSubsystem -> SetName ( SESSION ) ;

	if ( M4_ERROR == ((ClSSInterfaz*)poSessionSubsystem )-> SetElement ( "SESSION" , ai_poSession ) )
	{
		m_oSyncBellowLevel.LeaveReader ();
		return M4_ERROR ;
	}

	if ( M4_ERROR == poSessionSubsystem -> Initialize ( ) )
	{
		m_oSyncBellowLevel.LeaveReader ();
		return M4_ERROR ;
	}


	
	if ( poUserSubsystem ->GetStats() )
	{
		((ClUSUserStats*)poUserSubsystem ->GetStats() ) -> AddnTotalSessions ( 1 ) ;
	}

	if ( poProxySubsystem )
	{
		poSessionSubsystem -> AttachObserver ( poProxySubsystem -> GetSubsystem ( ) ) ;
	}
	if ( poXMLSubsystem )
	{
		poSessionSubsystem -> AttachObserver ( poXMLSubsystem -> GetSubsystem ( ) ) ;
	}
	if ( poLDBSubsystem )
	{
		poSessionSubsystem -> AttachObserver ( poLDBSubsystem -> GetSubsystem ( ) ) ;
	}

	if ( poUserSubsystem -> AddSubsystem ( poSessionSubsystem ) == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Couldn't insert new session subsystem" ) ;

		m_oSyncBellowLevel.LeaveReader ();
		return M4_ERROR ;
	}

    // register this subsystem in the snmp mib.
    poSessionSubsystem->m_poImplementation->RegisterSnmp();

	if ( poStats = (ClUsersSSStats *) GetStats ( )  ) 
	{
		poStats -> AddnSessions (1 ) ;
		poStats -> AddCurrentSessions ( 1 ) ;
	}

	iRet = m_oSessRepository.AddItem ( lIdSession , ai_poSession ) ;

	m_oSyncBellowLevel.LeaveReader ();

	if ( M4_ERROR == iRet )
	{
		EndUserSession ( lIdSession , lIdUser );
		return M4_ERROR ;
	}

	if (ai_poSession -> GetConnection() )
	{
		strIP = ai_poSession -> GetConnection() -> GetIP () ;
	}
	m4char_t szId [10] ;

	sprintf ( szId , "%lu" , (m4uint32_t)lIdSession );
	/* Bug 0095159 */
	SETCODEF (M4_ERR_AUDIT_OPEN_SESSION,DEBUGINFOLOG ,"#*s1*#%s#%d#%s#%s#%s#" , (char *)strUserID.c_str() , (m4uint32_t)lIdSession , (m4pchar_t)(ai_poSession ->GetHost ()) , strIP.c_str() , szsessionSartDate.c_str() ) ;

	return lIdSession ;
  //## end ClUsersSS::AddUserSession%893085118.body
}

m4objid_t ClUsersSS::AddUserAccount (ClUSUserInterface * &ai_poUser)
{
  //## begin ClUsersSS::AddUserAccount%893085116.body preserve=yes
	m4objid_t lId ;
	ClSSInterfaz  * poSubsystem = NULL ;
	ClUsersSSStats * poStats = NULL ;
	m4return_t iRet ;
	ClConfiguration * poConfiguration = NULL  ;
	ClConfiguration * poUserConfiguration = NULL  ;
	ClSSInterfaz  * poUsersSubsystem = NULL ;
	ClSystem * poSystem = NULL ;
//	m4char_t szPath [10] = "0." ;
	vector <m4objid_t> vUsers ;
	eStatus eState ;
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz  * poUserSubsystem = NULL ;
	size_t iLen ;
	m4char_t szUserId [100] ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return M4_ERROR ;
	}

	m4string_t		strUsername;
	ai_poUser -> GetName ( strUsername ) ;

	if (strUsername.empty())
	{
		return M4_ERROR ;
	}

	m4string_t	strUserId;
	ai_poUser->GetUserId( strUserId );

//Para saber los ids de los usuarios me construyo el path del subsystema de usuarios
//	sprintf ( szPath + 2 , "%d" , GetId () ) ;

	poSystem = ClSystem::Instance () ;

	m_oSyncBellowLevel.EnterReader ();

	for ( Iterator = m_oBellowLevel.begin () ; Iterator != m_oBellowLevel.end () ; Iterator ++ )
	{
		if ( poUserSubsystem = (*Iterator).second )
		{
			poUserConfiguration = poUserSubsystem -> GetConfiguration () ;

			if ( poUserConfiguration )
			{
				// bg 210771
				if ( M4_SUCCESS == poUserConfiguration -> GetStringValue ( "" , "USERID" , szUserId , 100 , iLen ) )
				{
					if (! stricmp ( szUserId , strUserId.c_str()) )
					{
						poSubsystem = poUserSubsystem ;

						lId = poSubsystem -> GetId () ;

						if ( ai_poUser -> GetStats() )
						{
							delete ai_poUser -> GetStats () ;
						}

						delete ai_poUser ;

						ai_poUser = (ClUSUserInterface*) poUserSubsystem -> GetElement ( "USER" ) ;
					}
				}
			}
		}


		if ( poSubsystem )
			break ;
	}

	m_oSyncBellowLevel.LeaveReader ();


	if ( ! poSubsystem )
	{
		poConfiguration = new ClPropertiesConfiguration ( PROPUSERCONFIGURATION ) ;

		poSubsystem = m_poSSFactory -> GenerateSubsystem ( M4_USER_SUBSYSTEM , poConfiguration , m_poTables ) ;

		if ( ! poSubsystem )
		{
			SETCODEF ( M4_SRV_SS_FACTORY_ERROR , ERRORLOG , "Error generating new user subsystem after user connection" ) ;

			return M4_ERROR ;
		}

		lId  = ClIdGenerator::GetNewId ( ) ; //m_oUserDic.AddItem ( ai_poUser ) ;

		poSubsystem -> SetId ( lId , GetPath ( ) ) ;

		ai_poUser -> SetId ( lId ) ;

		poSubsystem -> SetName ( USER ) ;
		poSubsystem -> SetElement ( "USER" , ai_poUser ) ;

		// !!! Cast to non-const to compile
		iRet = poConfiguration -> AddStringProperty ( "" , USERNAME , (char *)strUsername.c_str() ) ;
		if ( iRet == M4_ERROR )
		{
			return M4_ERROR ;
		}

		// !!! Cast to non-const to compile
		iRet = poConfiguration -> AddStringProperty ( "" , "STRINGID" , (char *)strUsername.c_str() ) ;
		if ( iRet == M4_ERROR )
		{
			return M4_ERROR ;
		}

		// bg 210771
		// !!! Cast to non-const to compile
		iRet = poConfiguration -> AddStringProperty ( "" , "USERID" , (char *)strUserId.c_str() ) ;
		if ( iRet == M4_ERROR )
		{
			return M4_ERROR ;
		}

		poSubsystem -> SetConfiguration ( poConfiguration ) ;
		poSubsystem -> Initialize ( ) ;

        // register this subsystem in the snmp mib.
        poSubsystem->m_poImplementation->RegisterSnmp();

		//Cuidado con esto cuando se cambie al nuevo modelo de interfaz-implementacion
		poSubsystem -> SetStats ( ai_poUser -> GetStats ( ) ) ;

		iRet = AddSubsystem (  (ClSSInterfaz*) poSubsystem ) ;

		if ( iRet == M4_ERROR )
		{
			return M4_ERROR ;
		}

		if ( poStats = (ClUsersSSStats *) GetStats ( ) )
		{
			poStats -> AddnUsersLoged (1 ) ;
		}
	}

	return lId;
  //## end ClUsersSS::AddUserAccount%893085116.body
}

m4return_t ClUsersSS::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClUsersSS::Stop%928140275.body preserve=yes
	vector <m4objid_t> vUsers ;
	m4uint32_t iUsers , i ;
	ClSSInterfaz * poSubsystem ;



	if ( ai_lDeferedTime <= 60 ) 
	{
		//Echar a los usuarios
		if ( M4_SUCCESS == GetSubsystemsId (vUsers) )
		{
			iUsers = vUsers.size() ;

			for ( i=0 ; i<iUsers ; i++ )
			{
				if ( poSubsystem = FindSubsystemById ( vUsers[i]) )
				{
					if ( poSubsystem -> GetType () == M4_USER_SUBSYSTEM )
					{
						RemoveSubsystem ( poSubsystem -> GetId() ) ;
					}
				}
			}
		}
	}

	return M4_SUCCESS;
  //## end ClUsersSS::Stop%928140275.body
}

m4bool_t ClUsersSS::IsReadyToClose ()
{
  //## begin ClUsersSS::IsReadyToClose%928140276.body preserve=yes
	M4ClCrono oCrono ;
	static m4double_t lFirstTime = 0 ;
	m4uint32_t lNewUsersLoged ;
	vector <m4uint32_t> vUsers ;
	static m4uint32_t UsersConnected = ((ClUsersSSStats*)m_poStatistics )-> GetnUsersLoged() ;



	lNewUsersLoged = ( (ClUsersSSStats*)m_poStatistics ) -> GetnUsersLoged() ;

	if( lNewUsersLoged < UsersConnected )
	{
		UsersConnected = lNewUsersLoged ;

		cout << "UsersLoged : " << lNewUsersLoged << "\n" << flush ;
	}

	lFirstTime ++ ;

	if ( lFirstTime > USS_SHUTDOWN_TIMEOUT )  
	{
		return M4_TRUE ;
	}

	return ( (0 == lNewUsersLoged)? M4_TRUE : M4_FALSE  ) ;
  //## end ClUsersSS::IsReadyToClose%928140276.body
}

m4return_t ClUsersSS::CheckTimeoutSessions ()
{
  //## begin ClUsersSS::CheckTimeoutSessions%933330708.body preserve=yes
	m4bool_t bLocked = M4_FALSE ;
	ClAdminTagList oParamList, oParamListReturn ;
	INTERFACESMAP::iterator IteratorUsers , IteratorSessions ;
	vector <m4objid_t> vSessions ;
	vector <m4objid_t> vUsers ;
	m4uint32_t iSessions , i;
	
	

	if ( g_dSessoinTimeOut )
	{
		m_oSessRepository.CheckTimeouts ( vSessions , vUsers ) ;

		iSessions = vSessions.size () ;

		for (i = 0 ; i< iSessions ;i++ )
		{
			EndUserSession ( vSessions [i] , vUsers [i] ) ;
		}
	}

	return M4_SUCCESS ;

  //## end ClUsersSS::CheckTimeoutSessions%933330708.body
}

m4return_t ClUsersSS::SendMessageToAllUsers (m4pchar_t ai_szMessage)
{
  //## begin ClUsersSS::SendMessageToAllUsers%943272855.body preserve=yes
	ClSSInterfaz * poUserSubsystem = NULL ;
	m4uint32_t i;
	m4return_t iRet ;
	vector <m4objid_t> users ;
	vector <m4objid_t> sessions ;
	ClAdminCommandTag *newParam;
//	ClMutBlock  oMutex( &m_oUsersMutex );
	ClAdminTagList oParamList , oParamListReturn;
	m4char_t szId [20] ;
	INTERFACESMAP::iterator Iterator ;
	m4bool_t bExecute  ;



	if ( M4_SUCCESS == GetSubsystemsId ( users ) )
	{
		for ( i= 0 ; i< users.size() ; i++ )
		{
			m_oSyncBellowLevel.EnterReader ();

			Iterator = m_oBellowLevel.find ( (m4uint32_t) users [i]  ) ;

			if ( Iterator == m_oBellowLevel.end ()  )
			{
				bExecute = M4_FALSE ;
			}
			else
			{
				poUserSubsystem = (*Iterator).second ;

				bExecute = M4_USER_SUBSYSTEM == poUserSubsystem -> GetType () ;
			}

			m_oSyncBellowLevel.LeaveReader ();

			if ( bExecute  )
			{
				sprintf( szId , "%d.%d" , (m4uint32_t) GetId() , (m4uint32_t) users [i] ) ;
				newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "MESSAGE", adminText, ai_szMessage);

				if (newParam == NULL) 
					return M4_ERROR;
				oParamList.push_back(newParam);
				
				iRet = ClSSImplementation::ExecuteCommand ( szId , ClIdentitySS::SEND_MESSAGE_TO_ALL_SESSIONS , oParamList , oParamListReturn) ;
			}
		}
	}

	return  M4_SUCCESS ;
  //## end ClUsersSS::SendMessageToAllUsers%943272855.body
}

m4return_t ClUsersSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClUsersSS::GetCommandsId%943286018.body preserve=yes
	ClAdminTag *newCommand;

//  No se deben exportar los comandos de la clase base, es decir, el ShutDown
//	ClSubsystemBase::GetCommandsId ( aio_oCommandList ) ;

	newCommand = ClAdminTag::GetNewAdminTag(CHECK_TIMEOUT_SESSIONS, "CHECK_USER_TIMEOUTS", M4_FALSE);

	if (newCommand == NULL) return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(SEND_MESSAGE_TO_ALL_USERS_SESSIONS, "SEND_MESSAGE_TO_ALL_USERS_SESSIONS", M4_TRUE);

	if (newCommand == NULL) return M4_ERROR;
	aio_oCommandList.push_back(newCommand);
	
	newCommand = ClAdminTag::GetNewAdminTag(ESTIMATE_LOAD , "ESTIMATE_LOAD", M4_TRUE);

	if (newCommand == NULL) return M4_ERROR;
	aio_oCommandList.push_back(newCommand);
	

	if ( ! g_dSessoinTimeOut )
	{
		newCommand = ClAdminTag::GetNewAdminTag(TURN_ON_USER_TIMEOUT , "TURN_ON_USER_TIMEOUT", M4_TRUE);
		if (newCommand == NULL) return M4_ERROR;
		aio_oCommandList.push_back(newCommand);
	}
	else
	{
		newCommand = ClAdminTag::GetNewAdminTag(TURN_ON_USER_TIMEOUT , "CHANGE_USER_TIMEOUT", M4_TRUE);
		if (newCommand == NULL) return M4_ERROR;
		aio_oCommandList.push_back(newCommand);

		newCommand = ClAdminTag::GetNewAdminTag(TURN_OFF_USER_TIMEOUT , "TURN_OFF_USER_TIMEOUT", M4_TRUE);

		if (newCommand == NULL) return M4_ERROR;
		aio_oCommandList.push_back(newCommand);
	}

	newCommand = ClAdminTag::GetNewAdminTag(CLOSE_ALL_SESSIONS , "CLOSE_ALL_SESSIONS", M4_FALSE);

	if (newCommand == NULL) return M4_ERROR;
	aio_oCommandList.push_back(newCommand);
	

	return M4_SUCCESS;
  //## end ClUsersSS::GetCommandsId%943286018.body
}

m4return_t ClUsersSS::CreateAction (m4pchar_t ai_szActionType, ClBaseAction *&ao_poAction)
{
  //## begin ClUsersSS::CreateAction%955610144.body preserve=yes
	if ( ! stricmp ( ai_szActionType , NEW_USER_ACTION ) )
	{
		ao_poAction = new ClNewUserAction ( this ) ;

		return M4_SUCCESS ;
	}

	if ( ! stricmp ( ai_szActionType , END_USER_SESSION_ACTION  ) )
	{
		ao_poAction = new ClEndUserSessionAction ( this ) ;

		return M4_SUCCESS ;
	}


	
	return M4_ERROR ;
  //## end ClUsersSS::CreateAction%955610144.body
}

m4return_t ClUsersSS::ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction)
{
  //## begin ClUsersSS::ProcessAction%957943674.body preserve=yes
	switch ( ai_oActionIdentifier )
	{

	case NEW_USER_ACTION_ID :

		ao_poAction = new ClNewUserAction ( this ) ;

		break ;


	case END_USER_SESSION_ACTION_ID  :

		ao_poAction = new ClEndUserSessionAction ( this ) ;

		break ;

	case RECOVER_SESSION_ACTION_ID  :

		ao_poAction = new ClRecoverSesionAction ( this ) ;

		break ;
	}

	ao_poAction -> SetParams ( ai_poParamList ) ;

	return ao_poAction -> Execute () ;
  //## end ClUsersSS::ProcessAction%957943674.body
}

m4return_t ClUsersSS::EstimateLoad (ClAdminTagList &ao_oTagList)
{
  //## begin ClUsersSS::EstimateLoad%962700384.body preserve=yes
	return M4_ERROR;
  //## end ClUsersSS::EstimateLoad%962700384.body
}

m4return_t ClUsersSS::RecoverSessionFromCredential (M4Credentials *ai_poCredential, ClUSSessionInterface *&ao_poSession)
{
  //## begin ClUsersSS::RecoverSessionFromCredential%967564558.body preserve=yes
	m4string_t strSession ;
	m4uint32_t iSessionId ;
	ClUSSessionInterface * poSession = NULL ;
	m4return_t ret = M4_ERROR ;
	m4string_t sUserID;

//Ahora he hecho una añapa para pasar el id en el puesto del username

//	if ( M4_SUCCESS == m_poCredentialGenerator ->ExtractUsername ( ai_poCredential -> m_strSessionID , strSession ) )
//	if ( M4_SUCCESS == m_poCredentialGenerator ->Extract ( ai_poCredential -> m_strSessionID , strSession ) )
//	{
//		if ( 0 < sscanf( strSession.c_str() , "%d" , &iSessionId ) )

	if (! ai_poCredential )
	{
		return M4_ERROR ;
	}

	if ( iSessionId = ai_poCredential -> GetSessId () )
		{
			ao_poSession = (ClUSSessionInterface *) m_oSessRepository.GetAndAddRequest ( iSessionId  );

			// bg 306647
			if( ao_poSession )
			{
				ao_poSession->GetUser()->GetUserId( sUserID );
				if( !strcmp( ai_poCredential->m_strLogin.c_str(), sUserID.c_str() ) )
				{
					ret = M4_SUCCESS;
				}
			}
		}
//	}

	return ret ;
  //## end ClUsersSS::RecoverSessionFromCredential%967564558.body
}

// Additional Declarations
  //## begin ClUsersSS%34C7653103CD.declarations preserve=yes
m4return_t ClUsersSS::_CloseAllSessions ( ) 
{
	ClSSInterfaz * poUserSubsystem = NULL ;
	m4uint32_t i , is;
	m4return_t iRet ;
	vector <m4objid_t> users ;
	vector <m4objid_t> sessions ;
	ClAdminTagList oParamList , oParamListReturn;
	m4char_t szId [20] , szIdSess [20] , szIdUser [20] ;
	INTERFACESMAP::iterator Iterator ;
	m4bool_t bExecute  ;



	if ( M4_SUCCESS == GetSubsystemsId ( users ) )
	{
		for ( i= 0 ; i< users.size() ; i++ )
		{
			m_oSyncBellowLevel.EnterReader ();

			Iterator = m_oBellowLevel.find ( (m4uint32_t) users [i]  ) ;

			if ( Iterator == m_oBellowLevel.end ()  )
			{
				bExecute = M4_FALSE ;
			}
			else
			{
				poUserSubsystem = (*Iterator).second ;

				bExecute = M4_USER_SUBSYSTEM == poUserSubsystem -> GetType () ;
			}

			if ( bExecute )
			{
				sprintf ( szIdSess , "%ld" , GetId () ) ;
				sprintf ( szIdUser , "%ld" , (m4uint32_t) users [i] );
				sprintf ( szId , "%s.%s" , szIdSess , szIdUser );

				poUserSubsystem -> GetSubsystemsId ( szIdUser , sessions ) ;
			}

			m_oSyncBellowLevel.LeaveReader ();

			if ( bExecute  )
			{
				for ( is = 0 ; is < sessions.size () ; is ++ )
				{

					iRet = EndUserSession( sessions [is] , users [i] ) ;
				}
			}
		}
	}

	return  M4_SUCCESS ;
}

  //## end ClUsersSS%34C7653103CD.declarations

// Class ClIdentitySS 


ClIdentitySS::ClIdentitySS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClIdentitySS::ClIdentitySS%890993642.hasinit preserve=no
  //## end ClIdentitySS::ClIdentitySS%890993642.hasinit
  //## begin ClIdentitySS::ClIdentitySS%890993642.initialization preserve=yes
  :ClSSImplementation ( ai_poConfiguration, ai_poTables, ai_poNavigator )
  //## end ClIdentitySS::ClIdentitySS%890993642.initialization
{
  //## begin ClIdentitySS::ClIdentitySS%890993642.body preserve=yes
	M4ClString szsessionSartDate ;

	ClConfiguration *poConfiguration = GetConfiguration();
	if ( poConfiguration )
	{
		M4ClTimeStamp clock ;

		clock.now ( ) ;
		clock.get ( szsessionSartDate ) ;
		
		SetConfigStringProperty ( "START_DATE" , szsessionSartDate ) ;
	}

	m_poUser = NULL ;
  //## end ClIdentitySS::ClIdentitySS%890993642.body
}


ClIdentitySS::~ClIdentitySS ()
{
  //## begin ClIdentitySS::~ClIdentitySS%890993643.body preserve=yes
//	delete m_poUser ;

  //## end ClIdentitySS::~ClIdentitySS%890993643.body
}



//## Other Operations (implementation)
ClUSUserInterface * ClIdentitySS::GetUser ()
{
  //## begin ClIdentitySS::GetUser%885550069.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return NULL ;
	}

	return m_poUser ;
  //## end ClIdentitySS::GetUser%885550069.body
}

void ClIdentitySS::SetUser (ClUSUserInterface *ai_poUser)
{
  //## begin ClIdentitySS::SetUser%885550070.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return ;
	}

	m_poUser = ai_poUser ;
  //## end ClIdentitySS::SetUser%885550070.body
}

void ClIdentitySS::Accept (ClUSUserVisitor &ai_oVisitor)
{
  //## begin ClIdentitySS::Accept%885550071.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return ;
	}

	ai_oVisitor.VisitUser ( * GetUser( ) ) ;
  //## end ClIdentitySS::Accept%885550071.body
}

m4return_t ClIdentitySS::Initialize ()
{
  //## begin ClIdentitySS::Initialize%885891305.body preserve=yes
	m_poVisitor = new ClUSUserVisitor ;
	eStatus eState;

	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState)
		return M4_ERROR ;

	
	return M4_SUCCESS ;
  //## end ClIdentitySS::Initialize%885891305.body
}

m4return_t ClIdentitySS::ShutDown ()
{
  //## begin ClIdentitySS::ShutDown%885891307.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return M4_ERROR ;
	}
	else
	{
		SetState ( SS_NOT_ACCESIBLE , SS_STR_NOT_ACCESIBLE) ;
	}

	ClSystem *poSystem = ClSystem::Instance();
	if (NULL == poSystem) return M4_ERROR;
	
	ClSSInterfaz  *poSubsystem = poSystem -> FindSubsystemByType (M4_USERS_SUBSYSTEM);

	if (NULL == poSubsystem) return M4_ERROR;

//	ClUsersSS *poUsersSubsystem = (ClUsersSS *)poSubsystem->GetSubsystem();
//	if (NULL == poUsersSubsystem) return M4_ERROR;

	ClUsersSSStats *poStats = (ClUsersSSStats *) poSubsystem ->GetStats();
	if (NULL == poStats) return M4_ERROR;
	poStats->AddnUsersLoged(-1);

	m_poStatistics = NULL ;

	m_poUser = NULL ;

	return M4_SUCCESS;
  //## end ClIdentitySS::ShutDown%885891307.body
}

m4return_t ClIdentitySS::Synchronize ()
{
  //## begin ClIdentitySS::Synchronize%886518769.body preserve=yes
	ClSessionSS * poSessionSS = NULL ;
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	if ( m_poVisitor )
	{
		Accept ( *(ClUSUserVisitor*)m_poVisitor ) ;
	}

	
	return ClSSImplementation::Synchronize() ;
  //## end ClIdentitySS::Synchronize%886518769.body
}

m4return_t ClIdentitySS::LockAllSessions ()
{
  //## begin ClIdentitySS::LockAllSessions%890086921.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	return M4_SUCCESS ;
  //## end ClIdentitySS::LockAllSessions%890086921.body
}

m4return_t ClIdentitySS::LogOffAllSessions ()
{
  //## begin ClIdentitySS::LogOffAllSessions%890086922.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	return M4_SUCCESS ;
  //## end ClIdentitySS::LogOffAllSessions%890086922.body
}

m4return_t ClIdentitySS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClIdentitySS::ExecuteCommand%910862260.body preserve=yes
	//
	// there is not necesary insert code to execute the CLOSE_USER_SESSION
	// commands, because really is the SHUTDOWN command and is executed
	// from ClSubsystemBase
	eStatus eState ;
	ClAdminTagList::iterator it;
	m4return_t ret = M4_ERROR ;
	m4pchar_t szMessage ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	if(ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::ExecuteCommand(ai_cCommand,ai_oParamList,ao_oResultList);

	switch ( ai_cCommand ) 
	{
	case SEND_MESSAGE_TO_ALL_SESSIONS :
		
		it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}
				// converts the value
		szMessage = ((*it)->m_tagValue);

		ret = SendMessageToAllSessions ( szMessage ) ;

		break ;

	default :
		M4_SRV_ERROR ( M4_SRV_COMMAND_ID_ERROR, "Error Executing command in '%0:s' Subsystem. Unknown command id: %1:d" , GetName() << ai_cCommand ) ;

		ret = M4_ERROR ;
	}

	return ret ; ;
  //## end ClIdentitySS::ExecuteCommand%910862260.body
}

m4return_t ClIdentitySS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClIdentitySS::GetCommandsId%910862261.body preserve=yes
	ClAdminTag *newCommand;
	//---------------------------------------------------------------
	// call the base class to get its commands
	//---------------------------------------------------------------
	/* 
	Currently, there is only the shutdown command in ClSubsystemBase
	For this subsystem we prefer insert this command in this function,
	and change its string identifier.
	When new commands appears in ClSubsystemBase, we need recall
	the function below.
	*/
	/*
	if (ClSubsystemBase::GetCommandsId(aio_oCommandList) != M4_SUCCESS)
		return M4_ERROR;
	*/
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;


	newCommand = ClAdminTag::GetNewAdminTag(SEND_MESSAGE_TO_ALL_SESSIONS, "SEND_MESSAGE_TO_ALL_SESSIONS", M4_TRUE);
	M4_ASSERT(newCommand != NULL);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	return M4_SUCCESS;
  //## end ClIdentitySS::GetCommandsId%910862261.body
}

m4return_t ClIdentitySS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oCommnadList)
{
  //## begin ClIdentitySS::GetCommandParams%910862262.body preserve=yes
	eStatus eState ;
	ClAdminCommandTag * newParam ;

	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	switch ( ai_cCommand )
	{
	case SEND_MESSAGE_TO_ALL_SESSIONS :
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "MESSAGE", adminText, "Meta4 Admin Message");

		if (newParam == NULL) 
			return M4_ERROR;
		aio_oCommnadList.push_back(newParam);
		break;
	}
	return M4_SUCCESS ;
  //## end ClIdentitySS::GetCommandParams%910862262.body
}

m4return_t ClIdentitySS::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClIdentitySS::Stop%928256386.body preserve=yes
	return M4_SUCCESS ;
  //## end ClIdentitySS::Stop%928256386.body
}

m4return_t ClIdentitySS::SendMessageToAllSessions (m4pchar_t ai_szMessage)
{
  //## begin ClIdentitySS::SendMessageToAllSessions%943272856.body preserve=yes
	ClSSInterfaz * poUserSubsystem = NULL;
	ClSSInterfaz * poSessionSubsystem = NULL ;
	m4uint32_t i;
	vector <m4objid_t> sessions ;
	m4return_t iRet ;
	ClAdminCommandTag *newParam;
	ClAdminTagList oParamList , oParamListReturn;
	ClSystem * poSystem = ClSystem::Instance();
	m4char_t szId [20] ;


//	if( poUserSubsystem = poSystem -> FindSubsystemById ( GetId ( ) ) )
//	{
		if ( M4_SUCCESS == GetSubsystemsId ( sessions ) )
		{
			for ( i= 0 ; i< sessions.size() ; i++ )
			{
				sprintf ( szId , "%d.%d" , (m4uint32_t) GetId () , (m4uint32_t)  sessions [i] ) ;

				newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "MESSAGE", adminText, ai_szMessage);

				if (newParam == NULL) 
					return M4_ERROR;
				oParamList.push_back(newParam);

				iRet = ClSSImplementation::ExecuteCommand ( szId , ClSessionSS::SEND_MESSAGE_TO_SESSION, oParamList , oParamListReturn ) ;
			}
		}
//	}


	return  M4_SUCCESS ;
  //## end ClIdentitySS::SendMessageToAllSessions%943272856.body
}

void * ClIdentitySS::GetElement (m4pchar_t ai_szElement)
{
  //## begin ClIdentitySS::GetElement%967450716.body preserve=yes
	void * poUser = NULL ;

	if ( ! stricmp ( ai_szElement , "USER" ) )
	{
		poUser =  m_poUser  ;
	}

	return poUser ;
  //## end ClIdentitySS::GetElement%967450716.body
}

m4return_t ClIdentitySS::SetElement (m4pchar_t ai_szElement, void *ai_poElement)
{
  //## begin ClIdentitySS::SetElement%967450717.body preserve=yes
	m4return_t error = M4_ERROR ;

	if ( ! stricmp ( ai_szElement , "USER" ) )
	{
		m_poUser = (ClUSUserInterface *) ai_poElement ;
		error = M4_SUCCESS ;
	}

	return error ;
  //## end ClIdentitySS::SetElement%967450717.body
}

// Additional Declarations
  //## begin ClIdentitySS%34C7653A0326.declarations preserve=yes
  //## end ClIdentitySS%34C7653A0326.declarations

// Class ClUsersSSVisitor 


//## Other Operations (implementation)
void ClUsersSSVisitor::VisitUsersSS (ClUsersSS &ai_oUsersSS)
{
  //## begin ClUsersSSVisitor::VisitUsersSS%885550073.body preserve=yes
  //## end ClUsersSSVisitor::VisitUsersSS%885550073.body
}

// Additional Declarations
  //## begin ClUsersSSVisitor%34C86E4802C9.declarations preserve=yes
  //## end ClUsersSSVisitor%34C86E4802C9.declarations

// Class ClSessionSS 


ClSessionSS::ClSessionSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClSessionSS::ClSessionSS%890993644.hasinit preserve=no
  //## end ClSessionSS::ClSessionSS%890993644.hasinit
  //## begin ClSessionSS::ClSessionSS%890993644.initialization preserve=yes
  :ClSSImplementation ( ai_poConfiguration, ai_poTables, ai_poNavigator )
  //## end ClSessionSS::ClSessionSS%890993644.initialization
{
  //## begin ClSessionSS::ClSessionSS%890993644.body preserve=yes
	m_poStatistics = NULL;
  //## end ClSessionSS::ClSessionSS%890993644.body
}


ClSessionSS::~ClSessionSS ()
{
  //## begin ClSessionSS::~ClSessionSS%899809124.body preserve=yes
  //## end ClSessionSS::~ClSessionSS%899809124.body
}



//## Other Operations (implementation)
void ClSessionSS::Accept (ClUSSessionVisitor &ai_oVisitor)
{
  //## begin ClSessionSS::Accept%885550072.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return ;

	ai_oVisitor.VisitSession ( * GetSession ( ) ) ;
  //## end ClSessionSS::Accept%885550072.body
}

ClUSSessionInterface * ClSessionSS::GetSession ()
{
  //## begin ClSessionSS::GetSession%885550074.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return NULL ;

	return m_poSession ;
  //## end ClSessionSS::GetSession%885550074.body
}

void ClSessionSS::SetSession (ClUSSessionInterface *ai_poSession)
{
  //## begin ClSessionSS::SetSession%885550075.body preserve=yes

	m_poSession = ai_poSession ;
  //## end ClSessionSS::SetSession%885550075.body
}

m4return_t ClSessionSS::Initialize ()
{
  //## begin ClSessionSS::Initialize%885891308.body preserve=yes
	eStatus eState ;
	m4string_t sessprotocol , strName;
	ClUSUserInterface * poUser ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	if ( ! m_poVisitor )
	{
		m_poVisitor = new ClUSSessionVisitor ;
	}

	if ( m_poSession )
	{
		m_poStatistics = m_poSession -> GetStats ( ) ;
	
		if ( M4_NON_CONNECTION_ORIENTED_PROTOCOL == m_poSession -> GetConnProtocol () )
		{
			sessprotocol = "Non connection oriented protocol" ;
		}
		else{
			sessprotocol = "Connection oriented protocol" ;
		}

		SetConfigStringProperty ( "CONNECTION_PROTOCOL" , (char*) sessprotocol.c_str() ) ;

		if ( poUser = m_poSession -> GetUser () )
		{
			poUser -> GetName ( strName ) ;

			SetConfigStringProperty ( "USERNAME" , ( char *) strName.c_str() ) ;
		}

	}

	return M4_SUCCESS ;
  //## end ClSessionSS::Initialize%885891308.body
}

m4return_t ClSessionSS::ShutDown ()
{
  //## begin ClSessionSS::ShutDown%885891310.body preserve=yes
	ClLocalMonitor * poLocalMonitor = NULL ;
	ClConnInstance * poConnection = NULL ;
	ClUSSessionStats * poStatistic = NULL ;
	eStatus eState ;
	m4string_t strUserName = "" , strUserID ;
	M4ClString szsessionSartDate ;
	m4string_t strIP ;
	M4ClTimeStamp clock ;
	m4double_t lTime;
	ClUSServiceAPI	*pServiceAPI = NULL ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return M4_ERROR ;
	}
	else
	{
		SetState (SS_NOT_ACCESIBLE , SS_STR_NOT_ACCESIBLE) ;
	}

	// User actions auditory.
	pServiceAPI = ClUSServiceAPI::Instance() ;
	if( pServiceAPI != NULL )
	{
		pServiceAPI->UserAuditoryLogoffAction( m_poSession ) ;
	}

	Notify ( CLOSE_SESSION ) ;


	if ( m_poSession )
	{
		if ( poStatistic = (ClUSSessionStats*) m_poSession -> GetStats () )
		{
			if ( poStatistic ->GetnPendingReqs() == 0)
			{
				m_poSession -> SetStatus ( SessClosed	) ;
			}
			else
			{
				m_poSession -> m_oStatus.SetValue ( SessCanceling ) ;
			}
		}

		if ( m_poSession -> GetUser () )
		{
			 m_poSession -> GetUser () -> GetName ( strUserName ) ;
			 m_poSession -> GetUser () -> GetUserId ( strUserID ) ;
		}

		if ( poConnection = (ClConnection * ) m_poSession -> GetConnection ( ) )
		{
			if ( poConnection )
			{
				poConnection -> SetStatus ( CLOSSING ) ;
			}

			strIP = poConnection -> GetIP () ;
		}

		// Auditoría de sesión
		// Se llama a cancelar la sesión si la sesión está auditada
		// Debe estar aquí porque en el AddItem se cambia el id
		if( m_poSession->GetAuditSession() == M4_TRUE )
		{
			m4return_t		iResult = M4_ERROR ;
			ClUSServiceAPI	*pService = ClUSServiceAPI::Instance() ;

			if( pService != NULL )
			{
				iResult = pService->CloseAuditory( m_poSession ) ;
			}
		}

		clock.now ( ) ;
		clock.get ( szsessionSartDate ) ;
		clock.get ( lTime ) ;

		/* Bug 0095159 */
		SETCODEF (M4_ERR_AUDIT_CLOSE_SESSION,DEBUGINFOLOG ,"#*s1*#%s#%d#%s#%s#%s#" , (char *)strUserID.c_str() , (m4uint32_t)GetId (), (m4pchar_t)(m_poSession ->GetHost ()) , strIP.c_str() , szsessionSartDate.c_str() ) ;

/* OSCAR 23-2-99
	De momento no se ghuarda la session para ser borrada sino que se guarda 
	el subsistema y cuancdo no queden request entonces se guarda la session para borrarla
*/
		poLocalMonitor = ClLocalMonitor::Instance ( ) ;
		
		if ( poLocalMonitor -> GetSessionDic ( ) ) 
		{
			poLocalMonitor -> GetSessionDic ( ) -> AddItem ( m_poSession ) ;
		}

		m_poSession = NULL ;
	}

	return M4_SUCCESS;
  //## end ClSessionSS::ShutDown%885891310.body
}

m4return_t ClSessionSS::Synchronize ()
{
  //## begin ClSessionSS::Synchronize%886518770.body preserve=yes
	ClUSSessionStats * poStats ;
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	if ( m_poVisitor )
	{
		Accept ( *(ClUSSessionVisitor*)m_poVisitor ) ;
	}

	poStats = (ClUSSessionStats * ) GetStats () ;

	if ( poStats )
	{
		ClStatTimeType dTimeLastTransaction ;
		M4ClString szTime;

		dTimeLastTransaction = poStats -> GettLastTransaction ( ) ;

		M4ClTimeStamp oTimeLastTransaction ( dTimeLastTransaction ) ;
		oTimeLastTransaction.get (szTime) ;

		SetConfigStringProperty( LAST_TRANSACTION_TIME , szTime ) ;

	}

	return M4_SUCCESS ;
  //## end ClSessionSS::Synchronize%886518770.body
}

m4return_t ClSessionSS::SetDebugLevel ()
{
  //## begin ClSessionSS::SetDebugLevel%890086923.body preserve=yes

	return M4_SUCCESS ;
  //## end ClSessionSS::SetDebugLevel%890086923.body
}

m4return_t ClSessionSS::ViewTraces ()
{
  //## begin ClSessionSS::ViewTraces%890086924.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	return M4_SUCCESS ;
  //## end ClSessionSS::ViewTraces%890086924.body
}

m4return_t ClSessionSS::Suspend ()
{
  //## begin ClSessionSS::Suspend%890086925.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	return M4_SUCCESS ;
  //## end ClSessionSS::Suspend%890086925.body
}

m4return_t ClSessionSS::Resume ()
{
  //## begin ClSessionSS::Resume%890086926.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	return M4_SUCCESS ;
  //## end ClSessionSS::Resume%890086926.body
}

m4return_t ClSessionSS::LogOff ()
{
  //## begin ClSessionSS::LogOff%890086927.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	return M4_SUCCESS ;
  //## end ClSessionSS::LogOff%890086927.body
}

void * ClSessionSS::GetElement (m4pchar_t ai_szElement)
{
  //## begin ClSessionSS::GetElement%896526474.body preserve=yes

	return ( ( void * )m_poSession ) ;
  //## end ClSessionSS::GetElement%896526474.body
}

m4return_t ClSessionSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClSessionSS::ExecuteCommand%910862263.body preserve=yes
	eStatus eState ;
	ClAdminTagList::iterator it;
	m4pchar_t szMessage = NULL;
	m4return_t ret = M4_ERROR ;



	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	switch ( ai_cCommand )
	{
	case SEND_MESSAGE_TO_SESSION :
		
		it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
		if (it == ai_oParamList.end())
		{
			return M4_ERROR;
		}
				// converts the value
		szMessage = ((*it)->m_tagValue);

		ret = SendCSMessage ( M4_CS_EVENT_ADMIN_MESSAGE , szMessage ) ;

		break ;
	case CHECK_TIMEOUT_SESSION :
		if (  CheckTimeout () )
			return M4_SUCCESS ;

	default :
		M4_SRV_ERROR ( M4_SRV_COMMAND_ID_ERROR, "Error Executing command in '%0:s' Subsystem. Unknown command id: %1:d" , GetName() << ai_cCommand ) ;

		ret = M4_ERROR ;
	}

	return ret ;
  //## end ClSessionSS::ExecuteCommand%910862263.body
}

m4return_t ClSessionSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClSessionSS::GetCommandsId%910862264.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	//---------------------------------------------------------------
	// call the base class to get its commands
	//---------------------------------------------------------------
	/* 
	Currently, there is only the shutdown command in ClSubsystemBase
	For this subsystem we prefer insert this command in this function,
	and change its string identifier.
	When new commands appears in ClSubsystemBase, we need recall
	the function below.
	*/
	/*
	if (ClSubsystemBase::GetCommandsId(aio_oCommandList) != M4_SUCCESS)
		return M4_ERROR;
	*/

	//---------------------------------------------------------------
	// the shutdown command with another string id.
	//---------------------------------------------------------------
	ClAdminTag *newCommand;

	newCommand = ClAdminTag::GetNewAdminTag(SHUT_DOWN, "CLOSE_SESSION", M4_FALSE);

	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(SEND_MESSAGE_TO_SESSION, "SEND_MESSAGE_TO_SESSION", M4_TRUE);

	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	return M4_SUCCESS ;
  //## end ClSessionSS::GetCommandsId%910862264.body
}

m4return_t ClSessionSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oCommnadList)
{
  //## begin ClSessionSS::GetCommandParams%910862265.body preserve=yes
	eStatus eState ;
	ClAdminCommandTag *newParam;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	switch ( ai_cCommand )
	{
	case SEND_MESSAGE_TO_SESSION :
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "MESSAGE", adminText, "Meta4 Admin Message");

		if (newParam == NULL) 
			return M4_ERROR;
		aio_oCommnadList.push_back(newParam);
		break;
	}
	return M4_SUCCESS ;
  //## end ClSessionSS::GetCommandParams%910862265.body
}

m4return_t ClSessionSS::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClSessionSS::Stop%928256387.body preserve=yes
	m4char_t szMessage[OBL_PROPVALUE_MAX_LEN] ;
	m4uint32_t iMin;

	if (! ai_lDeferedTime )
	{
		iMin = 0 ;
	}
	else
	{
		iMin = (ai_lDeferedTime/60) -1 ;
	}


	sprintf ( szMessage , "%d" , iMin );

	return SendCSMessage ( M4_CS_EVENT_SERVER_SHUTDOWN , szMessage ) ;
  //## end ClSessionSS::Stop%928256387.body
}

m4bool_t ClSessionSS::CheckTimeout ()
{
  //## begin ClSessionSS::CheckTimeout%933665003.body preserve=yes
	ClUSSessionStats * poStats ;
	M4ClTimeStamp oCrono;
	m4double_t dTime, dElapseTime;
	m4int32_t imin;
	vector <m4objid_t> aSessionSubsystemIds ;
	m4char_t szMessage [OBL_PROPVALUE_MAX_LEN];
			



	poStats = (ClUSSessionStats * ) GetStats () ;

	if ( poStats )
	{
		ClStatTimeType dTimeLastTransaction ;
		M4ClString szTime;

		if ( 0 < poStats -> GetnPendingReqs () )
		{
			return M4_FALSE ;
		}

		dTimeLastTransaction = poStats -> GettLastTransaction ( ) ;

		if( g_dSessoinTimeOut )
		{
			oCrono.now () ;

			oCrono.get ( dTime ) ;

			dElapseTime = dTime - dTimeLastTransaction  ; //En días
			dElapseTime *= 24 ; //Se pasa a horas
			dElapseTime *= 60 ; //Se pasa a minutos
			imin = dElapseTime ;

//			iRet = oElapse.get ( iyear, imon, iday, ihour, imin, isec);

			if ( imin >= g_dSessoinTimeOut ) 
			{
				imin = g_dSessoinTimeOut ;
				sprintf ( szMessage , "%3.0d" , imin );
				SendCSMessage ( M4_CS_EVENT_SESSION_TIMEOUT	, szMessage ) ;
				return M4_TRUE ;
			}
		}
	}
	return M4_FALSE ;
  //## end ClSessionSS::CheckTimeout%933665003.body
}

m4return_t ClSessionSS::SendCSMessage (m4uint32_t ai_iEventMessage, m4pchar_t ai_pMessage)
{
  //## begin ClSessionSS::SendCSMessage%941113627.body preserve=yes
	ClSSInvoker * poInvoker ;
	ClParamList * poParamList ;
	m4return_t iret = M4_SUCCESS ;
	ClRequest * poRequest ;
	ClConnection * poConnection ;
	M4DataStorage * poDataStore;
	ClDataUnit * poDataUnit ;
	ClPDU * poPDU ;
	m4pchar_t buffer ;
	m4objid_t inewid ;



	inewid = ClIdGenerator::GetNewId () ;

	if ( poConnection = ( ClConnection * ) m_poSession -> GetConnection()  )
	{

		if ( M4_PROTOCOL == poConnection -> GetClientType () )
		{

			poRequest = ClRequest::s_oGarbageCollector.CreateRequest( M4_CONTROL_REQ_TYPE, NULL);
			poRequest->SetConnection(poConnection);

			++ (poConnection -> m_oNumRequest  );

			poPDU = poConnection -> m_oProtocol.GetPDU(M4_ID_PDU_CONNECT);

			poPDU -> SetRequestId  ( 0 ) ;

			poRequest -> SetPDUInput(poPDU ) ;

			poPDU = poConnection -> m_oProtocol.GetPDU(M4_ID_PDU_ACK);

			poPDU -> SetRequestId  ( 0 ) ;

			poDataStore = poPDU->GetDataStorage();

			poRequest -> SetId ( inewid ) ;

			if (poDataStore->GetNewDataUnit(poDataUnit)!=M4_ERROR)
			{
				poDataUnit->SetSize(strlen (ai_pMessage) + 1/* '\0' */ + 1 /*ai_iEventMessage */);//Seleccionamos la talla del buffer de salida
							
				if ((buffer=poDataUnit->GetBuffer())!=NULL)
				{
					//EL PRIMER BIT LLEVA EL CÓDIGO DE EVENTO
					buffer [0] = ai_iEventMessage	;

					strcpy ( buffer +1, ai_pMessage ) ;

					poRequest -> SetPDUOutput(poPDU ) ;

					poInvoker = ClSSInvoker::GetInstance () ;

					poInvoker -> GetParamList ( poParamList  ) ;
				
					poParamList -> SetPointer ( "REQUEST" , poRequest ) ;

					iret = poInvoker ->ExecuteAction ( REGISTER_REQUEST_ACTION , poParamList ) ;

					poInvoker -> GetParamList ( poParamList ) ;
	
					poParamList -> SetPointer ( "REQUEST" , poRequest ) ;

					iret = poInvoker -> ExecuteAction ( SEND_REQUEST_ACTION , poParamList) ;

					m4Trace( g_uiPDUSInside ++ );

//					poInvoker -> SetParamList ( *poParamList ) ;
				}
			}
		}
	}

	return iret ;
  //## end ClSessionSS::SendCSMessage%941113627.body
}

m4return_t ClSessionSS::SetElement (m4pchar_t ai_szElement, void *ai_poElement)
{
  //## begin ClSessionSS::SetElement%955100222.body preserve=yes
	if ( ! stricmp ( ai_szElement , "SESSION" ) )
	{
		m_poSession = (ClUSSession*) ai_poElement  ;
	}
	else
	{
		return M4_FALSE ;
	}

return M4_SUCCESS ;
  //## end ClSessionSS::SetElement%955100222.body
}

// Additional Declarations
  //## begin ClSessionSS%34C76544032A.declarations preserve=yes
  //## end ClSessionSS%34C76544032A.declarations

// Class ClUsersSSStats 




ClUsersSSStats::ClUsersSSStats ()
  //## begin ClUsersSSStats::ClUsersSSStats%890086954.hasinit preserve=no
      : m_lUsersLoged(0), m_lTotalSessions(0), m_lCurrentSessions(0)
  //## end ClUsersSSStats::ClUsersSSStats%890086954.hasinit
  //## begin ClUsersSSStats::ClUsersSSStats%890086954.initialization preserve=yes
  //## end ClUsersSSStats::ClUsersSSStats%890086954.initialization
{
  //## begin ClUsersSSStats::ClUsersSSStats%890086954.body preserve=yes
	m_uiNumberOfStats = TOTAL_NUM ;
  //## end ClUsersSSStats::ClUsersSSStats%890086954.body
}



//## Other Operations (implementation)
void ClUsersSSStats::AddnUsersLoged (ClStatNumericalType ai_lUsersLoged)
{
  //## begin ClUsersSSStats::AddnUsersLoged%890086928.body preserve=yes
	m_lUsersLoged += ai_lUsersLoged ;
  //## end ClUsersSSStats::AddnUsersLoged%890086928.body
}

ClStatNumericalType ClUsersSSStats::GetnUsersLoged ()
{
  //## begin ClUsersSSStats::GetnUsersLoged%890086929.body preserve=yes
	return m_lUsersLoged ;
  //## end ClUsersSSStats::GetnUsersLoged%890086929.body
}

void ClUsersSSStats::AddnSessions (ClStatNumericalType ai_lSessions)
{
  //## begin ClUsersSSStats::AddnSessions%890086930.body preserve=yes
	m_lTotalSessions += ai_lSessions ;
  //## end ClUsersSSStats::AddnSessions%890086930.body
}

ClStatNumericalType ClUsersSSStats::GetnSessions ()
{
  //## begin ClUsersSSStats::GetnSessions%890086931.body preserve=yes
	return m_lTotalSessions ;
  //## end ClUsersSSStats::GetnSessions%890086931.body
}

m4return_t ClUsersSSStats::Reset ()
{
  //## begin ClUsersSSStats::Reset%890086948.body preserve=yes
	return M4_SUCCESS ;
  //## end ClUsersSSStats::Reset%890086948.body
}

m4return_t ClUsersSSStats::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData)
{
  //## begin ClUsersSSStats::GetStat%890086952.body preserve=yes
	m4uint16_t				lowIndex;
	eUsersStats				statIndex;
	m4return_t				res = M4_TRUE;

	if (M4_ERROR == _CheckStatID(ai_hStatId, M4_USERS_SUBSYSTEM))
		return M4_ERROR;

	lowIndex = ai_hStatId.GetLowID();

	statIndex = (eUsersStats)lowIndex;

	if (0 < ao_poData.GetSize())
	{
		SETCODEF ( M4_SRV_SS_STATS_ERROR , ERRORLOG , "Dataunit Passed parameter size is invalid, Size = %d" , ao_poData.GetSize() ) ;

		return M4_ERROR;
	}

	ao_poData.SetSize(DEF_STRING_SIZE);
	m4pchar_t	buffer = ao_poData.GetBuffer();

// 
/*		enum {
				TOTAL_NUM
			} */

	switch(statIndex)
	{
	case USERS_LOGED:
		sprintf ( buffer , "%ld" , m_lUsersLoged ) ;

		break ;

	case TOTAL_SESSIONS:
		sprintf ( buffer , "%ld" , m_lTotalSessions ) ;

		break ;

	case CURRENT_SESSIONS :
		sprintf ( buffer , "%ld" , m_lCurrentSessions ) ;

		break ;

	default:
		sprintf(buffer, "%010d", 0);
		res = M4_FALSE;
	}

	return res;
  //## end ClUsersSSStats::GetStat%890086952.body
}

m4return_t ClUsersSSStats::ResetStat (ClStatHandle &ai_hStatId)
{
  //## begin ClUsersSSStats::ResetStat%890086953.body preserve=yes
	return M4_SUCCESS ;
  //## end ClUsersSSStats::ResetStat%890086953.body
}

void ClUsersSSStats::AddCurrentSessions (m4int32_t ai_iSessions)
{
  //## begin ClUsersSSStats::AddCurrentSessions%1013505804.body preserve=yes
	m_lCurrentSessions += ai_iSessions ;
  //## end ClUsersSSStats::AddCurrentSessions%1013505804.body
}

m4uint32_t ClUsersSSStats::GetCurrentSessions ()
{
  //## begin ClUsersSSStats::GetCurrentSessions%1013505805.body preserve=yes
	return m_lCurrentSessions ;
  //## end ClUsersSSStats::GetCurrentSessions%1013505805.body
}

// Additional Declarations
  //## begin ClUsersSSStats%350D691A00FE.declarations preserve=yes
  //## end ClUsersSSStats%350D691A00FE.declarations

// Class ClNewUserAction 

ClNewUserAction::ClNewUserAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClNewUserAction::ClNewUserAction%955559601.hasinit preserve=no
  //## end ClNewUserAction::ClNewUserAction%955559601.hasinit
  //## begin ClNewUserAction::ClNewUserAction%955559601.initialization preserve=yes
  :ClSSAction ( (ClSSImplementation*)ai_poExecutive )
  //## end ClNewUserAction::ClNewUserAction%955559601.initialization
{
  //## begin ClNewUserAction::ClNewUserAction%955559601.body preserve=yes
  //## end ClNewUserAction::ClNewUserAction%955559601.body
}



//## Other Operations (implementation)
m4return_t ClNewUserAction::Execute ()
{
  //## begin ClNewUserAction::Execute%955559602.body preserve=yes
	ClUSUserInterface * poUser ;
	ClUSSessionInterface * poSession ;
	ClConnInstance * poConnection = NULL ;
	void * poItem ;


	if ( M4_ERROR == m_pParamList -> GetPointer ( "USER" , poItem ) )
	{
		return M4_ERROR ;
	}

	if ( ! ( poUser = (ClUSUserInterface*) poItem ) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetPointer ( "SESSION" , poItem ) )
	{
		return M4_ERROR ;
	}

	if ( ! ( poSession= (ClUSSessionInterface *) poItem ) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR != m_pParamList -> GetPointer ( "CONNECTION" , poItem ) )
	{
		poConnection = (ClConnInstance*) poItem ;
	}

	if ( M4_ERROR == ((ClUsersSS*)m_poExecutive )-> AddUserSession( poUser , poSession ) )
	{
		return M4_ERROR ;
	}

	return M4_SUCCESS;

  //## end ClNewUserAction::Execute%955559602.body
}

// Additional Declarations
  //## begin ClNewUserAction%38F4AE5B03BA.declarations preserve=yes
  //## end ClNewUserAction%38F4AE5B03BA.declarations

// Class ClEndUserSessionAction 

ClEndUserSessionAction::ClEndUserSessionAction (ClSSImplementation *ai_poExecutive)
  //## begin ClEndUserSessionAction::ClEndUserSessionAction%955610165.hasinit preserve=no
  //## end ClEndUserSessionAction::ClEndUserSessionAction%955610165.hasinit
  //## begin ClEndUserSessionAction::ClEndUserSessionAction%955610165.initialization preserve=yes
  :ClSSAction ( (ClSSImplementation*)ai_poExecutive )
  //## end ClEndUserSessionAction::ClEndUserSessionAction%955610165.initialization
{
  //## begin ClEndUserSessionAction::ClEndUserSessionAction%955610165.body preserve=yes
  //## end ClEndUserSessionAction::ClEndUserSessionAction%955610165.body
}



//## Other Operations (implementation)
m4return_t ClEndUserSessionAction::Execute ()
{
  //## begin ClEndUserSessionAction::Execute%955610166.body preserve=yes
	m4objid_t lIdSession , lIdUser ;
	m4double_t d ;

	if ( M4_ERROR == m_pParamList -> GetDouble( "SESSIONID" , d ) )
	{
		return M4_ERROR ;
	}
	lIdSession = d  ;

	if ( M4_ERROR == m_pParamList -> GetDouble( "USERID" , d ) )
	{
		return M4_ERROR ;
	}
	lIdUser = d  ;



	return ((ClUsersSS*)m_poExecutive )-> EndUserSession( lIdSession , lIdUser) ;
  //## end ClEndUserSessionAction::Execute%955610166.body
}

// Additional Declarations
  //## begin ClEndUserSessionAction%38F5CF16029E.declarations preserve=yes
  //## end ClEndUserSessionAction%38F5CF16029E.declarations

// Class ClRecoverSesionAction 

ClRecoverSesionAction::ClRecoverSesionAction (ClSSImplementation *ai_poExecutive)
  //## begin ClRecoverSesionAction::ClRecoverSesionAction%967564559.hasinit preserve=no
  //## end ClRecoverSesionAction::ClRecoverSesionAction%967564559.hasinit
  //## begin ClRecoverSesionAction::ClRecoverSesionAction%967564559.initialization preserve=yes
 :ClSSAction ( ai_poExecutive )
  //## end ClRecoverSesionAction::ClRecoverSesionAction%967564559.initialization
{
  //## begin ClRecoverSesionAction::ClRecoverSesionAction%967564559.body preserve=yes
  //## end ClRecoverSesionAction::ClRecoverSesionAction%967564559.body
}



//## Other Operations (implementation)
m4return_t ClRecoverSesionAction::Execute ()
{
  //## begin ClRecoverSesionAction::Execute%967564560.body preserve=yes
	void * pointer ;
	ClUSSessionInterface * poSession ;
	M4Credentials * poCredential ;


	if ( M4_ERROR == m_pParamList -> GetPointer ( "CREDENTIAL" , pointer ) )
	{
		return M4_ERROR ;
	}

	if ( ! ( poCredential  = (M4Credentials*) pointer ) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR  == ((ClUsersSS*)m_poExecutive )->RecoverSessionFromCredential ( poCredential, poSession ) ) 
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> SetPointer ( "SESSION" , poSession ) )
	{
		return M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClRecoverSesionAction::Execute%967564560.body
}

// Additional Declarations
  //## begin ClRecoverSesionAction%39ABDD7F0371.declarations preserve=yes
  //## end ClRecoverSesionAction%39ABDD7F0371.declarations

//## begin module%34C86C1C0035.epilog preserve=yes
//## end module%34C86C1C0035.epilog
