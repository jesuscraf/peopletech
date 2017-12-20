//## begin module%37A556160015.cm preserve=no
//## end module%37A556160015.cm

//## begin module%37A556160015.cp preserve=no
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
//## end module%37A556160015.cp

//## Module: clssimplementacion%37A556160015; Package body
//## Subsystem: M4Subsystems::src%37D3A22003E4
//## Source file: z:\m4subsystems\src\clssimplementacion.cpp

//## begin module%37A556160015.additionalIncludes preserve=no
//## end module%37A556160015.additionalIncludes

//## begin module%37A556160015.includes preserve=yes
#include <clssinterfaz.hpp>
#include <clarraylibres.hpp>
#include <m4srvres.hpp>
#include <m4log.hpp>
#include <clconfiguration.hpp>
#include <propertiestable.hpp>
#include <ssnames.hpp>
#include <configtree.hpp>
#include <subsystemids.hpp>
#include <m4srvtrace.h>

#include <snmpinterfaces.hpp>
#include <snmpadmininstance.hpp>
#include "snmpres.hpp"
//## end module%37A556160015.includes

// m4types
#include <m4types.hpp>
// syncronization
#include <m4syncronization.hpp>
// Statistics
#include <statistics.hpp>
// clbaseobject
#include <clbaseobject.hpp>
// ClAdminTag
#include <cladmintag.hpp>
// NavigatorF
#include <navigatorf.hpp>
// clbaseaction
#include <clbaseaction.hpp>
// serverevent
#include <serverevent.hpp>
// clssfactoryinterfaz
#include <clssfactoryinterfaz.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
// m4ssapi4snmp
#include <m4ssapi4snmp.hpp>

class M4_DECL_M4SUBSYSTEMS ClSSAction;

//## begin module%37A556160015.declarations preserve=no
//## end module%37A556160015.declarations

//## begin module%37A556160015.additionalDeclarations preserve=yes
#define MAXBLOCKS 100
#ifndef OBL_PROPVALUE_MAX_LEN
// bugid: 0090961
#   define OBL_PROPVALUE_MAX_LEN	512
#endif 


#define M4_SRV_TRACE_COMP_ID	"SUBSYS"
//## end module%37A556160015.additionalDeclarations


// Class ClSSImplementation 











//## begin ClSSImplementation::m_poSSFactory%37A5B0110144.role preserve=yes  protected: static ClSSFactoryInterfaz { -> RHAN}
ClSSFactoryInterfaz *ClSSImplementation::m_poSSFactory = NULL;
//## end ClSSImplementation::m_poSSFactory%37A5B0110144.role








ClSSImplementation::ClSSImplementation (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, m4bool_t ai_bHasOwner)
  //## begin ClSSImplementation::ClSSImplementation%933665011.hasinit preserve=no
      : m_szSSName(NULL), m_ssId("0"), m_ssPath("0"), m_poSnmpObject(NULL), m_poStatistics(NULL), m_poVisitor(NULL), m_poInterfaz(NULL)
  //## end ClSSImplementation::ClSSImplementation%933665011.hasinit
  //## begin ClSSImplementation::ClSSImplementation%933665011.initialization preserve=yes
  //## end ClSSImplementation::ClSSImplementation%933665011.initialization
{
  //## begin ClSSImplementation::ClSSImplementation%933665011.body preserve=yes
	m4char_t szType [ OBL_PROPVALUE_MAX_LEN ];
	m4uint32_t lType ;
	m_poConfiguration = ai_poConfiguratoin ;
	m_poTables = ai_poTables ;
	m_poNavigator =  ai_poNavigator ;
	m_bActive = SS_ACTIVE ;
	m_lType = 0 ;
	m_lId = 0 ;
	m_bHasOwner = ai_bHasOwner ;
	static m4uint32_t aiInstancesIndex [200] ;
	static m4bool_t bInit = M4_FALSE ;


	if  (! bInit )
	{
		memset ( aiInstancesIndex , 0 , 200*sizeof ( m4uint32_t) ) ;

		bInit = M4_TRUE ;
	}

	if ( m_poConfiguration )
	{
		m_poConfiguration -> AddStringProperty ( "" , "STATE" , "ACTIVE" ) ;
		if ( m_poTables)
		{
			if ( M4_SUCCESS ==  m_poConfiguration -> GetType ( szType , OBL_PROPVALUE_MAX_LEN ) )
			{
				if ( M4_ERROR != (lType = m_poTables -> GetId ( szType ) ) )
				{
					m_lType = lType ;
				}
			}
		}


	}

	ClSSInterfaz * poInterfaz = new ClSSInterfaz ( this );

	SetInterfaz ( poInterfaz ) ;

  //## end ClSSImplementation::ClSSImplementation%933665011.body
}

ClSSImplementation::ClSSImplementation ()
  //## begin ClSSImplementation::ClSSImplementation%954148131.hasinit preserve=no
      : m_szSSName(NULL), m_ssId("0"), m_ssPath("0"), m_poSnmpObject(NULL), m_poStatistics(NULL), m_poVisitor(NULL), m_poInterfaz(NULL)
  //## end ClSSImplementation::ClSSImplementation%954148131.hasinit
  //## begin ClSSImplementation::ClSSImplementation%954148131.initialization preserve=yes
  //## end ClSSImplementation::ClSSImplementation%954148131.initialization
{
  //## begin ClSSImplementation::ClSSImplementation%954148131.body preserve=yes
	m_iInstance = 0 ;
  //## end ClSSImplementation::ClSSImplementation%954148131.body
}


ClSSImplementation::~ClSSImplementation ()
{
  //## begin ClSSImplementation::~ClSSImplementation%960286623.body preserve=yes
    
    // Delete this node in snmp repository.
    UnRegisterSnmp();
	
    if ( m_poVisitor )
		delete m_poVisitor ;

	if ( m_szSSName )
		delete []  m_szSSName ;
  //## end ClSSImplementation::~ClSSImplementation%960286623.body
}



//## Other Operations (implementation)
m4return_t ClSSImplementation::AddSubsystem (ClSSInterfaz *ai_poSubsystem)
{
  //## begin ClSSImplementation::AddSubsystem%933578383.body preserve=yes
	m4objid_t lId ;

	if ( ! ai_poSubsystem )
		return M4_ERROR ;


	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;

	m_oSyncBellowLevel.EnterWriter ();

	lId = ai_poSubsystem -> GetId ( ) ;

	if ( lId == 0)
	{
		lId = ClIdGenerator::GetNewId () ; 

		ai_poSubsystem -> SetId ( lId , GetPath () ) ;
	}

	m_oBellowLevel.insert ( INTERFACESMAP::value_type ( lId , ai_poSubsystem ) ) ;

	m_oSyncBellowLevel.LeaveWriter ();

	return M4_SUCCESS ;
  //## end ClSSImplementation::AddSubsystem%933578383.body
}

m4return_t ClSSImplementation::RemoveSubsystem (m4objid_t ai_lSubsystemId)
{
  //## begin ClSSImplementation::RemoveSubsystem%933578384.body preserve=yes
	ClSSInterfaz * poSubsystem ;
	INTERFACESMAP::iterator Iterator ;
	m4return_t iRet ;

	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;

	m_oSyncBellowLevel.EnterWriter ();

	Iterator = m_oBellowLevel.find ( ai_lSubsystemId ) ;

	if ( Iterator == m_oBellowLevel.end ( ) )
	{
//		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Subsystem nº %d is not a son of subsystem nº %d" , ai_lSubsystemId , GetId () ) ;

		m_oSyncBellowLevel.LeaveWriter ();

		return M4_ERROR ;
	}

  if ( ! ( poSubsystem = (*Iterator).second ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Cannot remove subsystem nº %d that is NULL" , ai_lSubsystemId ) ;

		m_oSyncBellowLevel.LeaveWriter ();

		return M4_ERROR ;
	}
  //primero se borra el subsystema y luego se hace el shutdown por probemas de sincronización
	m_oBellowLevel.erase  ( Iterator ) ;

	m_oSyncBellowLevel.LeaveWriter ();

	iRet = poSubsystem -> ShutDown ( ) ;

	delete poSubsystem ;

	return iRet ;
  //## end ClSSImplementation::RemoveSubsystem%933578384.body
}

m4return_t ClSSImplementation::ShutDown ()
{
  //## begin ClSSImplementation::ShutDown%933578385.body preserve=yes
	INTERFACESMAP::iterator Iterator , iterAux;
	m4return_t iRet = M4_SUCCESS ;;
	ClSSInterfaz * poSubsystem ;
	m4objid_t lId ;
	m4uint32_t iSubsystems , i;
	vector <m4objid_t> vSubsystems ;

	//Desactivar de flag de activo de ese subsystema
	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;
	
	//Sincronizar el acceso para impedir el acceso a otro thread

	GetSubsystemsId ( vSubsystems ) ;

	iSubsystems = vSubsystems.size () ;

	for ( i =0 ; i< iSubsystems  ; i++ )
	//Propagar el shutdown a todos los subsystemas hijos de este
	{
		poSubsystem = m_oBellowLevel [ vSubsystems[i] ] ;

		if ( poSubsystem )
		{
			lId = poSubsystem -> GetId ()  ;

			iRet = RemoveSubsystem ( lId ) ;
				
			//	poSubsystem -> ShutDown ( ) ;

			if ( iRet == M4_ERROR )
			{
				if ( GetName () )
					SETCODEF ( M4_SRV_SS_SHUTDOWN_ERROR , ERRORLOG , "Error making %s subsystem shut down" , GetName () ) ;
			}

		}

	}

	if ( M4_ERROR == iRet )
	{
		return iRet ;
	}
 
	m_bActive = SS_NOT_ACCESIBLE ;

//OSCAR 30-11-99
// Trato de desacoplarla factoría de la nueva implementación de subsistemas
//	iRet = m_poSSFactory -> ShutDownSubsystem ( m_poInterfaz ) ;

	if ( M4_ERROR == iRet )
	{
		m_bActive = SS_INACTIVE ;
		return iRet ;
	}

	//Desbloquer el acceso

	return iRet ;
  //## end ClSSImplementation::ShutDown%933578385.body
}

m4return_t ClSSImplementation::Synchronize ()
{
  //## begin ClSSImplementation::Synchronize%933578387.body preserve=yes
	m4return_t iRet = M4_SUCCESS ;
  	INTERFACESMAP::iterator Iterator ;
//	vector <m4objid_t> vBranches;
	ClSSInterfaz * poSubsystem ;

	
	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;

	m_oSyncBellowLevel.EnterReader ();
/*
	if ( M4_SUCCESS == GetSubsystemsId( vBranches) )
	{
		iBranches = vBranches.size() ;

		for ( i = 0 ; i < iBranches ; i++ )
		{
			poSubsystem = FindSubsystemById(vBranches[i]) ;

			if ( poSubsystem )
			{
				if ( M4_ERROR == poSubsystem -> Synchronize ( ) )
				{
					iRet = M4_ERROR ;
				}
			}
		}
	}
*/
	for ( Iterator = m_oBellowLevel.begin () ; Iterator != m_oBellowLevel.end ( ) ; Iterator ++ )
	{
		poSubsystem = (* Iterator).second ;

		if ( M4_ERROR == poSubsystem -> Synchronize ( ) )
		{
			iRet = M4_ERROR ;
		}
	}

	m_oSyncBellowLevel.LeaveReader ();

	return iRet ;
  //## end ClSSImplementation::Synchronize%933578387.body
}

m4return_t ClSSImplementation::Initialize ()
{
  //## begin ClSSImplementation::Initialize%933578388.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSSImplementation::Initialize%933578388.body
}

m4return_t ClSSImplementation::GetStatisticAttribValue (m4uint32_t ai_iStatId, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSSImplementation::GetStatisticAttribValue%933578389.body preserve=yes
	ClStatHandle oStatHandle ;
	ClDataUnit * poData ;
	m4uint32_t lAux = 65535 ;
	m4uint16_t iType ;
	ClStatistic *poStatistic = NULL;
	m4return_t retCode = M4_ERROR;

	
	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;

	m_oSyncBellowLevel.EnterReader ();

		oStatHandle.SetLowID ( (m4uint16_t) (ai_iStatId & lAux ) ) ;

		lAux <<= 16 ;

		iType = (m4uint16_t)GetType();

		switch ( iType )
		{
		case M4_LAUNCHERLIST_SUBSYSTEM :
			iType = M4_LAUNCHER_SUBSYSTEM ;
			break ;

		case M4_RECEIVERLIST_SUBSYSTEM :
			 iType = M4_RECEIVER_SUBSYSTEM	;
			 break ;

		case M4_SENDERLIST_SUBSYSTEM :
			iType = M4_SENDER_SUBSYSTEM	;
			break ;

		case M4_ACCEPTORLIST_SUBSYSTEM :
			iType = M4_ACEPTOR_SUBSYSTEM ;
			break ;

		case M4_SESSMANAGERLIST_SUBSYSTEM :
			iType = M4_SESSMANAGER_SUBSYSTEM ;
			break ;


		case M4_SCHEDULERLIST_SUBSYSTEM	:
			iType = M4_SCHEDULER_SUBSYSTEM ;
			break ;

		case M4_DISTRIBUTORLIST_SUBSYSTEM :
			iType = M4_DISTRIBUTOR_SUBSYSTEM ;
		}

		oStatHandle.SetHighID ( iType ) ;

		ao_poDataStorage -> GetNewDataUnit ( poData ) ;

		if (NULL == (poStatistic = GetStats()))
		{
			retCode = M4_ERROR;
		}
		else
		{
			if (0 == poStatistic-> GetNumberOfStats())
			{
				retCode = M4_ERROR;
			}
			else
			{ 
				retCode = poStatistic->GetStat(oStatHandle, *poData);
			}
		}

		if (M4_ERROR == retCode)
		{
			m_oSyncBellowLevel.LeaveReader ();

			return M4_ERROR ;
		}

		//----------------------------------------------------------
		// Check for the syntax of the data in this statistic.
		// Some values conflicts with the VB client administrator 
		// protocol
		//----------------------------------------------------------
		m4pchar_t pcBuffer = poData->GetBuffer();
		for (m4uint32_t i=0; i<strlen(pcBuffer); i++)
		{
			if ((pcBuffer[i] == ':') ||
				(pcBuffer[i] == ',') ||
				(pcBuffer[i] == ';'))
			{
				pcBuffer[i] = '\'';
			}
		}


//		ao_poDataStorage -> Add ( poData ) ;
	m_oSyncBellowLevel.LeaveReader ();
		
	return M4_SUCCESS;
  //## end ClSSImplementation::GetStatisticAttribValue%933578389.body
}

m4return_t ClSSImplementation::GetSubsystemAttribValue (m4uint16_t ai_iAttribId, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSSImplementation::GetSubsystemAttribValue%933578390.body preserve=yes
	size_t iLen ;
	double dValue ;
	m4pchar_t szValue ;
	m4char_t szName [OBL_PROPVALUE_MAX_LEN] ;
	m4char_t szPath [OBL_PROPVALUE_MAX_LEN]  ;
	m4char_t szAttribute [20] ;
	m4return_t iRet = M4_SUCCESS;
	double iAttribs  ;

	
	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;

	m_oSyncBellowLevel.EnterReader ();
	
	m4char_t szSubsytemType [100] ;
	m_poConfiguration -> GetType ( szSubsytemType , 100 ) ;
	
	//sprintf ( szPath , "%s.%s.%s" , PROPERTIES_ATTRIBUTES_PATH , GetName () , ATTRIBUTES ) ;
	sprintf ( szPath , "%s.%s.%s" , PROPERTIES_ATTRIBUTES_PATH , szSubsytemType , ATTRIBUTES ) ;
	sprintf ( szAttribute , "%s%d" , ATTRIBUTE , ai_iAttribId ) ;


	iRet = m_poTables -> m_oBook -> GetNumericValue ( szPath , "NUM_ATTRIBUTES" , iAttribs ) ;
	
	if (iRet == M4_ERROR )
	{
		m_oSyncBellowLevel.LeaveReader ();			

		return M4_ERROR ;
	}

	if ( iAttribs <= ai_iAttribId )
	{
		m_oSyncBellowLevel.LeaveReader ();			

		return M4_ERROR ;
	}

	iRet = m_poTables -> m_oBook -> GetStringValue ( szPath , szAttribute , szName , 100 , iLen ) ;		

	if ( iRet == M4_ERROR )
	{

		if ( GetName () )
			M4_SRV_WARNING ( M4_SRV_TABLES_PROP_ERROR, "No %0:s value, specified for property: '%1:s' in obl '%2:s'",szSubsytemType  << szAttribute << "properties" );
//			SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , DEBUGINFOLOG , "Property %s not found in %s subsystem configuration" , szAttribute , GetName () ) ;			
		m_oSyncBellowLevel.LeaveReader ();			
		return M4_ERROR ;
	}

		
	m4char_t acValue[ OBL_PROPVALUE_MAX_LEN ] = "";
	memset( acValue, 0, OBL_PROPVALUE_MAX_LEN );

	if ( m_poConfiguration -> GetStringValue ( "" , szName , acValue , OBL_PROPVALUE_MAX_LEN , iLen ) == M4_ERROR )
	{
		iRet = m_poConfiguration -> GetNumericValue ( "" , szName , dValue ) ;
		if ( iRet == M4_ERROR )
		{
			m_oSyncBellowLevel.LeaveReader ();			
			return M4_ERROR ;
		}
		sprintf ( acValue , "%ld" , (m4uint32_t) dValue ) ;
	}

	ClDataUnit *poDU;
	ao_poDataStorage -> GetNewDataUnit(poDU);
	poDU->SetSize(OBL_PROPVALUE_MAX_LEN );
	szValue = poDU->GetBuffer () ;

	strncpy( szValue, acValue, OBL_PROPVALUE_MAX_LEN );

//	}

	//----------------------------------------------------------
	// Check for the syntax of the data in this attribute.
	// Some values conflicts with the VB client administrator 
	// protocol
	//----------------------------------------------------------
	m4pchar_t pcBuffer = szValue;
	for (m4uint32_t i=0; i<OBL_PROPVALUE_MAX_LEN; i++)
	{
		if ((pcBuffer[i] == ':') ||
			(pcBuffer[i] == ',') ||
			(pcBuffer[i] == ';'))
		{
			pcBuffer[i] = '\'';
		}
	}
	
//	ao_poDataStorage -> Add ( szValue , strlen ( szValue ) +1 , M4_TRUE ) ;

	m_oSyncBellowLevel.LeaveReader ();

	return iRet ;
  //## end ClSSImplementation::GetSubsystemAttribValue%933578390.body
}

ClSSInterfaz * ClSSImplementation::FindSubsystemByType (m4uint32_t ai_iType)
{
  //## begin ClSSImplementation::FindSubsystemByType%933578391.body preserve=yes
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz * poSubsystem ;

	

	if ( ai_iType == GetType ( ) ) 
	{
		return m_poInterfaz ;
	}
/* Necesito hacer esto aunque no esté añun activo
	if ( m_bActive == SS_NOT_ACCESIBLE )
		return NULL ;
*/
	m_oSyncBellowLevel.EnterReader ();

	for ( Iterator = m_oBellowLevel.begin ( ) ; Iterator != m_oBellowLevel.end ( ) ; Iterator ++ )
	{
		poSubsystem = (ClSSInterfaz*)( * Iterator).second -> FindSubsystemByType ( ai_iType ) ;

		if ( poSubsystem )
		{
			m_oSyncBellowLevel.LeaveReader ();

			return poSubsystem ;
		}
	}

	m_oSyncBellowLevel.LeaveReader ();

	return NULL ;
  //## end ClSSImplementation::FindSubsystemByType%933578391.body
}

ClSSInterfaz * ClSSImplementation::FindSubsystemById (m4objid_t ai_lId)
{
  //## begin ClSSImplementation::FindSubsystemById%933578392.body preserve=yes
	ClSSInterfaz * poSubsystem = NULL;
	INTERFACESMAP::iterator Iterator ;


	
	if ( ai_lId == GetId () )
	{
		return m_poInterfaz ;
	}


	if ( m_bActive == SS_NOT_ACCESIBLE )
		return NULL ;


	m_oSyncBellowLevel.EnterReader ();

	for ( Iterator = m_oBellowLevel.begin () ; Iterator != m_oBellowLevel.end ( ) ; Iterator ++ )
	{
		poSubsystem = (* Iterator).second -> FindSubsystemById ( ai_lId ) ;

		if ( poSubsystem )
		{
			m_oSyncBellowLevel.LeaveReader ();

			return poSubsystem ;
		}
	}

	m_oSyncBellowLevel.LeaveReader ();

	return NULL ;
  //## end ClSSImplementation::FindSubsystemById%933578392.body
}

ClSSInterfaz * ClSSImplementation::FindSubsystemByName (m4pchar_t ai_szName)
{
  //## begin ClSSImplementation::FindSubsystemByName%933578393.body preserve=yes
	ClSSInterfaz * poSubsystem ;
	INTERFACESMAP::iterator Iterator ;



	if (GetName () )
	{
		if ( ! strcmp ( ai_szName , GetName () ) )
		{
			return m_poInterfaz;
		}
	}


	if ( m_bActive == SS_NOT_ACCESIBLE )
		return NULL ;


	m_oSyncBellowLevel.EnterReader ();

	for ( Iterator = m_oBellowLevel.begin () ; Iterator != m_oBellowLevel.end ( ) ; Iterator ++ )
	{
		poSubsystem = (ClSSInterfaz*)(* Iterator).second -> FindSubsystemByName ( ai_szName ) ;

		if ( poSubsystem )
		{
			m_oSyncBellowLevel.LeaveReader ();

			return poSubsystem ;
		}
	}
	m_oSyncBellowLevel.LeaveReader ();

	return NULL ;
  //## end ClSSImplementation::FindSubsystemByName%933578393.body
}

m4return_t ClSSImplementation::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClSSImplementation::ExecuteCommand%933578394.body preserve=yes
	m4return_t iRet;

	

	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;

	switch ( ai_cCommand ) 
	{
	case RESET_STATISTICS :

		iRet = m_poStatistics -> Reset () ;

		break ;
	case SHUT_DOWN :
	
		iRet = ShutDown();

	}

	return iRet ;
  //## end ClSSImplementation::ExecuteCommand%933578394.body
}

m4return_t ClSSImplementation::Start ()
{
  //## begin ClSSImplementation::Start%933578395.body preserve=yes
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz * poInterfaz ;
	M4ClString szsessionSartDate ;
	M4ClTimeStamp clock ;
	m4uint32_t i = 5;
	m4uint32_t  iSubsystems ;
	vector <m4objid_t> vSubsystems ;
	
	if ( m_poConfiguration )
	{

		clock.now ( ) ;
		clock.get ( szsessionSartDate ) ;
		
		m_poConfiguration -> AddStringProperty ( "" , "START_DATE" , szsessionSartDate ) ;
	}


    // registrar el subsistema en SNMP.
    RegisterSnmp();


    GetSubsystemsId ( vSubsystems ) ;
	iSubsystems = vSubsystems.size () ;

	for ( i =0 ; i< iSubsystems  ; i++ )
	//Propagar el shutdown a todos los subsystemas hijos de este
	{
		m_oSyncBellowLevel.EnterReader () ;

		poInterfaz  = m_oBellowLevel [vSubsystems[i]] ;

//		poInterfaz  = FindSubsystemById ( vSubsystems[i] ) ;
		m_oSyncBellowLevel.LeaveReader () ;

		if ( poInterfaz )
		{
			if ( M4_ERROR == poInterfaz -> Start () )
			{
				return M4_ERROR ;
			}
		}

	}

/*
	//Sincronizar el acceso para impedir el acceso a otro thread
	m_oSyncBellowLevel.EnterReader () ;

	if ( m_poConfiguration )
	{

		clock.now ( ) ;
		clock.get ( szsessionSartDate ) ;
		
		m_poConfiguration -> AddStringProperty ( "" , "START_DATE" , szsessionSartDate ) ;
	}

	for ( Iterator = m_oBellowLevel.begin() ; Iterator!=m_oBellowLevel.end() ; Iterator ++ )
	{
		poInterfaz = (*Iterator).second ;

		if ( poInterfaz )
		{
			if ( M4_ERROR == poInterfaz -> Start () )
			{
				m_oSyncBellowLevel.LeaveReader () ;
				return M4_ERROR ;
			}
		}
	}

	m_oSyncBellowLevel.LeaveReader () ;
*/
	return M4_SUCCESS ;
  //## end ClSSImplementation::Start%933578395.body
}

m4return_t ClSSImplementation::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClSSImplementation::Stop%933578396.body preserve=yes
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz * poInterfaz ;


	m_oSyncBellowLevel.EnterReader () ;

	for ( Iterator = m_oBellowLevel.begin() ; Iterator!=m_oBellowLevel.end() ; Iterator ++ )
	{
		poInterfaz = (*Iterator).second ;

		if ( poInterfaz )
		{
			poInterfaz -> Stop (ai_lDeferedTime) ;
		}
	}

	m_oSyncBellowLevel.LeaveReader () ;

	return M4_SUCCESS ;
  //## end ClSSImplementation::Stop%933578396.body
}

m4bool_t ClSSImplementation::IsReadyToClose ()
{
  //## begin ClSSImplementation::IsReadyToClose%933578397.body preserve=yes
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz * poInterfaz ;


	m_oSyncBellowLevel.EnterReader () ;

	for ( Iterator = m_oBellowLevel.begin() ; Iterator!=m_oBellowLevel.end() ; Iterator ++ )
	{
		poInterfaz = (*Iterator).second ;

		if ( poInterfaz )
		{
			if ( ! poInterfaz -> IsReadyToClose () )
			{
				m_oSyncBellowLevel.LeaveReader () ;
				return M4_FALSE ;
			}
		}
	}

	m_oSyncBellowLevel.LeaveReader () ;

	return M4_TRUE ;
  //## end ClSSImplementation::IsReadyToClose%933578397.body
}

m4return_t ClSSImplementation::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClSSImplementation::GetCommandParams%933578399.body preserve=yes
	switch (ai_cCommand)
	{
	case RESET_STATISTICS :
	case SHUT_DOWN:
		// there is not parameters for shutdown command.
		break;
	default:
		M4_ASSERT("Command not recognized" == NULL);
		break;		
	}
	return M4_SUCCESS;
  //## end ClSSImplementation::GetCommandParams%933578399.body
}

m4return_t ClSSImplementation::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClSSImplementation::GetCommandsId%933578400.body preserve=yes
	ClAdminTag *newCommand;
	m4char_t szType [OBL_PROPVALUE_MAX_LEN ] ;


	GetConfiguration () ->GetType (szType ,OBL_PROPVALUE_MAX_LEN) ;

	strcpy ( aio_oCommandList.m_subsystemPath , szType ) ;

	newCommand = ClAdminTag::GetNewAdminTag(SHUT_DOWN, "SHUT_DOWN", M4_FALSE);
	M4_ASSERT(newCommand != NULL);
	if (newCommand == NULL) return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	
	newCommand = ClAdminTag::GetNewAdminTag(RESET_STATISTICS, "RESET_STATISTICS", M4_FALSE);
	M4_ASSERT(newCommand != NULL);
	if (newCommand == NULL) return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	return M4_SUCCESS;
  //## end ClSSImplementation::GetCommandsId%933578400.body
}

m4pchar_t ClSSImplementation::GetName ()
{
  //## begin ClSSImplementation::GetName%933578401.body preserve=yes
	return m_szSSName ;
  //## end ClSSImplementation::GetName%933578401.body
}

m4return_t ClSSImplementation::GetStatisticAttribsId (M4DataStorage *ao_poDataStorage)
{
  //## begin ClSSImplementation::GetStatisticAttribsId%933578402.body preserve=yes
	m4uint32_t iNumOfStats = 0 , lHandle;
	m4char_t szPath [100]  ;
	m4char_t szStatistic [20] ;
	m4char_t szScale [10] ;
	m4char_t szType [10] ;
	m4char_t szValue [MAXNAMEPROP] ;
	m4pchar_t szLiteral  ;
	m4char_t szId [10] ;
	size_t iLen ;
	m4return_t iRet; 
	m4uint16_t  iType , i;
	m4uint32_t iSize ;
	m4char_t iScale ;
	m4pchar_t szIds ;
	m4char_t szSubsytemType [100] ;

	ClDataUnit *poDataUnit ;
	ao_poDataStorage -> GetNewDataUnit ( poDataUnit ) ;


//	szIds [ 0 ] = 0 ;

	m_oSyncBellowLevel.EnterReader ();			

	if ( GetStats ( ) )
	{
		iNumOfStats = GetStats ( ) -> GetNumberOfStats ( ) ;
	}

	m_poConfiguration -> GetType ( szSubsytemType , 100 ) ;

	poDataUnit -> SetSize ( MAXNAMEPROP ) ;

	szIds = poDataUnit -> GetBuffer ( ) ;

	szIds [0]= 0 ;

	// properties.obl access-permissions path.
	m4char_t accessOblPath[256];
	sprintf (accessOblPath, "M4ADMINISTRATOR.ACCESS-PERMISSIONS.%s.STATISTICS", szSubsytemType);
	
	for ( i = 0 ; i < iNumOfStats ; i ++ )
	{
		ClStatHandle oStatHandle ;

		sprintf ( szStatistic , "%s%d" , STATISTIC , i ) ;

		//------------
		// get in the properties.obl the attribute access-permisions.
		iRet = m_poTables->m_oBook->GetStringValue (accessOblPath, szStatistic, szValue, 100, iLen);

		// check if the command is enable.
		if ((iRet==M4_SUCCESS) && (!strcmp(szValue, "NOT-ACCESSIBLE")))
			continue;
		//------------

		sprintf ( szPath , "%s.%s.%s" , PROPERTIES_STATISTICS_PATH , szSubsytemType , "STATISTICS" ) ;

		iRet = m_poTables -> m_oBook -> GetStringValue ( szPath , szStatistic , szValue , 100 , iLen ) ;

		if ( iRet == M4_ERROR )
		{
			SETCODEF ( M4_SRV_SS_STATS_ERROR , ERRORLOG , "Unknown statistic %s in subsystem %s" , szStatistic , szSubsytemType ) ;

			m_oSyncBellowLevel.LeaveReader ();			
			return M4_ERROR ;
		}


		iRet = m_poTables -> GetAttrib ( szValue , iType , iScale , szLiteral) ;

		if ( iRet == M4_ERROR )
		{
			SETCODEF ( M4_SRV_SS_STATS_ERROR , ERRORLOG , "No attributes (Type or Scale) for statistic %s in subsystem %s" , szValue , szSubsytemType ) ;

			m_oSyncBellowLevel.LeaveReader ();			
			return M4_ERROR ;
		}

		lHandle = GetType ( ) ;

		lHandle <<= 16 ;

		lHandle += i  ;

		//Aquí no solo hay que pasar la palabra de menos peso 
		//sino el largo completo
		//Hay que convertir el ClStatHandle a long
//		sprintf ( szId , "%d," , (m4uint32_t) oStatHandle ) ;
//		sprintf ( szId , "%d," , lHandle ) ;
		sprintf ( szId , "%d," , i ) ;

		sprintf ( szScale , ",%d;" , iScale ) ;

		sprintf ( szType , "%d," , iType ) ;

		iSize = strlen ( szScale ) + strlen ( szType ) + strlen ( szId ) + strlen ( szLiteral ) + 1 ;

		if ( iSize + strlen (szIds) > poDataUnit -> GetSize ( ) )
		{
			poDataUnit -> SetSize ( poDataUnit -> GetSize ( ) + MAXNAMEPROP) ;

			szIds = poDataUnit -> GetBuffer ( ) ;
		}

		strcat ( szIds , szId ) ;

		strcat ( szIds , szType ) ;

		strcat ( szIds , szLiteral ) ;

		strcat ( szIds , szScale ) ;

	}

	//ao_poDataStorage -> Add ( poDataUnit ) ;
	m_oSyncBellowLevel.LeaveReader ();			

	return M4_SUCCESS ;
  //## end ClSSImplementation::GetStatisticAttribsId%933578402.body
}

m4return_t ClSSImplementation::GetSubsystemAttribsId (M4DataStorage *ao_poDataStorage)
{
  //## begin ClSSImplementation::GetSubsystemAttribsId%933578403.body preserve=yes
	double iAttribs  ;
	m4return_t iRet ;
	m4uint16_t i , iType ;
	m4char_t iScale ;
	m4char_t szPath [MAXNAMEPROP] ;
	m4char_t szAttribute [20] ;
	m4char_t szValue [MAXNAMEPROP] ;
	m4char_t szLiteral [MAXNAMEPROP];
	m4pchar_t pLiteral ;
	m4pchar_t szIds ;
	m4char_t szId [10] ;
	m4char_t szScale [10] ;
	m4char_t szType [10] ;
	m4uint32_t iSize = 0;
	ClDataUnit * poDataUnit ;
	size_t iLen ;
	m4char_t szSubsytemType [100] ;


	
	m_oSyncBellowLevel.EnterReader ();			

	m_poConfiguration -> GetType ( szSubsytemType , 100 ) ;

	sprintf ( szPath , "%s.%s.%s" , PROPERTIES_ATTRIBUTES_PATH , szSubsytemType , ATTRIBUTES ) ;

	// properties.obl access-permissions path.
	m4char_t accessOblPath[256];
	sprintf (accessOblPath, "M4ADMINISTRATOR.ACCESS-PERMISSIONS.%s.ATTRIBUTES", szSubsytemType );

	iRet = m_poTables -> m_oBook -> GetNumericValue ( szPath , "NUM_ATTRIBUTES" , iAttribs ) ;

	if ( iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_ATTRIBUTES_ERROR , ERRORLOG , "No attributes NUM_ATTRIBUTES in subsystem %s" , m_szSSName ) ;

		m_oSyncBellowLevel.LeaveReader ();			
		return M4_ERROR ;
	}

	poDataUnit = new ClDataUnit ;

	poDataUnit -> SetSize ( MAXNAMEPROP ) ;

	szIds = poDataUnit -> GetBuffer ( ) ;

	szIds [0] = 0;

	for ( i = 0 ; i < iAttribs ; i++ )
	{
		sprintf ( szId , "%d," , i ) ;

		sprintf ( szAttribute , "%s%d" , ATTRIBUTE , i ) ;

		//------------
		// get in the properties.obl the attribute access-permisions.
		iRet = m_poTables->m_oBook->GetStringValue (accessOblPath, szAttribute, szValue, 100, iLen);

		// check if the command is enable.
		if ((iRet==M4_SUCCESS) && (!strcmp(szValue, "NOT-ACCESSIBLE")))
			continue;
		//------------

		iRet = m_poTables -> m_oBook -> GetStringValue ( szPath , szAttribute , szValue , 100 , iLen ) ;

		if ( iRet == M4_ERROR )
		{
			if (GetName())
				SETCODEF ( M4_SRV_SS_ATTRIBUTES_ERROR , ERRORLOG , "No %s in subsystem %s " , szAttribute , GetName() ) ;
			delete poDataUnit;
			m_oSyncBellowLevel.LeaveReader ();			
			return M4_ERROR ;
		}


		iRet = m_poTables -> GetAttrib ( szValue , iType , iScale , pLiteral ) ;

		if ( iRet == M4_ERROR )
		{
			if (GetName())
				SETCODEF ( M4_SRV_SS_ATTRIBUTES_ERROR , ERRORLOG , "No attributes (Type or Scale) for attribute %s in subsystem %s" , szValue , GetName () ) ;
			delete poDataUnit;
			m_oSyncBellowLevel.LeaveReader ();			
			return M4_ERROR ;
		}

		if ( ! strcmp ( szValue , M4_SUBSYSTEM_TYPE ) )
		{
			if ( m_poConfiguration -> GetStringValue ( "" , M4_SUBSYSTEM_TYPE , szLiteral , 100 , iLen ) == M4_ERROR )
			{
				delete poDataUnit;
				m_oSyncBellowLevel.LeaveReader ();			
				return M4_ERROR ;
			}
			pLiteral = szLiteral ;
		}

		sprintf ( szScale , ",%d;" , iScale ) ;

		sprintf ( szType , "%d," , iType ) ;

		iSize = strlen ( szId ) + strlen (szType ) + strlen ( pLiteral ) + strlen ( szScale ) + 1 ;
	
		//Si el tamaño del buffer no es suficiente para copiar todo, realoco el buffer.
		if ( iSize + strlen ( szIds ) > poDataUnit -> GetSize ( ) )
		{
			//Copio lo que había ya en un buffer auxiliar
			m4pchar_t szAux = new m4char_t [strlen ( szIds ) +1 ] ;

			strcpy ( szAux , szIds ) ;
		
			//pido un nuevo buffer
			poDataUnit -> SetSize ( poDataUnit -> GetSize ( ) + MAXNAMEPROP ) ;

			szIds = poDataUnit -> GetBuffer ( ) ;

			//Copìo de nuevo lo que ya tenía antes
			strcpy ( szIds , szAux ) ;

			//borro el buffer auxiliar
			delete [] szAux  ;
		}

		strcat ( szIds , szId ) ;

		strcat ( szIds , szType ) ;

		strcat ( szIds , pLiteral ) ;

		strcat ( szIds , szScale ) ;
	}
	
	ao_poDataStorage -> Add ( poDataUnit ) ;

	m_oSyncBellowLevel.LeaveReader ();			

	return  M4_SUCCESS; 
  //## end ClSSImplementation::GetSubsystemAttribsId%933578403.body
}

m4return_t ClSSImplementation::GetSubsystemsId (vector <m4objid_t > &ao_poTags)
{
  //## begin ClSSImplementation::GetSubsystemsId%933578404.body preserve=yes
	INTERFACESMAP::iterator Iterator;
	m4objid_t lId ;

	m_oSyncBellowLevel.EnterReader ();			
	
	for (Iterator = m_oBellowLevel.begin(); Iterator != m_oBellowLevel.end(); Iterator++)
	{		
		lId = (*Iterator).first ;		
		ao_poTags.push_back(lId);	
	}

	m_oSyncBellowLevel.LeaveReader ();			
	
	return M4_SUCCESS;
  //## end ClSSImplementation::GetSubsystemsId%933578404.body
}

m4uint32_t ClSSImplementation::GetType ()
{
  //## begin ClSSImplementation::GetType%933578405.body preserve=yes
	return m_lType ;
  //## end ClSSImplementation::GetType%933578405.body
}

void ClSSImplementation::SetName (m4pchar_t ai_szName)
{
  //## begin ClSSImplementation::SetName%933578406.body preserve=yes
	if  ( m_szSSName )
	{
		delete [] m_szSSName ;
	}
	if( m_szSSName = new m4char_t [strlen(ai_szName ) +1] )
	{
		strcpy (m_szSSName , ai_szName ) ;
	}
  //## end ClSSImplementation::SetName%933578406.body
}

void ClSSImplementation::SetType (m4uint32_t ai_iType)
{
  //## begin ClSSImplementation::SetType%933578407.body preserve=yes
	m_lType = ai_iType ;
  //## end ClSSImplementation::SetType%933578407.body
}

void ClSSImplementation::SetType (m4pchar_t ai_szType)
{
  //## begin ClSSImplementation::SetType%933578408.body preserve=yes
  //## end ClSSImplementation::SetType%933578408.body
}

void ClSSImplementation::SetConfiguration (ClConfiguration *ai_poConfiguration)
{
  //## begin ClSSImplementation::SetConfiguration%933578425.body preserve=yes
	m_poConfiguration = ai_poConfiguration ;
  //## end ClSSImplementation::SetConfiguration%933578425.body
}

void * ClSSImplementation::GetElement (m4pchar_t ai_szElement)
{
  //## begin ClSSImplementation::GetElement%933578426.body preserve=yes
	return NULL ;
  //## end ClSSImplementation::GetElement%933578426.body
}

m4objid_t ClSSImplementation::GetId ()
{
  //## begin ClSSImplementation::GetId%933578427.body preserve=yes
	return m_lId ;
  //## end ClSSImplementation::GetId%933578427.body
}

void ClSSImplementation::SetId (m4objid_t ai_lId, m4pchar_t ai_szFatherPath)
{
  //## begin ClSSImplementation::SetId%933578428.body preserve=yes
	m4char_t szId [20] ;

	sprintf ( szId , "%ld", (m4uint32_t) ai_lId ) ;

	m_lId = ai_lId ;
	m_ssId = szId ;
	m_ssPath = ai_szFatherPath ;
	m_ssPath += "." ;
	m_ssPath +=  szId ;
  //## end ClSSImplementation::SetId%933578428.body
}

void ClSSImplementation::Activate ()
{
  //## begin ClSSImplementation::Activate%933578429.body preserve=yes
	m_bActive = SS_ACTIVE ;
  //## end ClSSImplementation::Activate%933578429.body
}

m4return_t ClSSImplementation::Init (ClConfiguration *ai_poConfiguration)
{
  //## begin ClSSImplementation::Init%933665012.body preserve=yes
	ClSSInterfaz * poSubsystem ;
	UINT * aiHandles = new UINT [MAXBLOCKS] ;
	m4size_t iNumBlocks = 0 ;
	m4uint16_t  iBlocks , iInst ;
	m4double_t iNumInstances ;
	m4return_t iRet ;
	ClConfiguration * poConfiguration ;
	m4objid_t  lId ;
	m4int16_t lType ;
	m4char_t szType [OBL_PROPVALUE_MAX_LEN] ;
	m4pchar_t szNumInstances  = "NUM_INSTANCES_";
	m4char_t szNumInsPropName  [OBL_PROPVALUE_MAX_LEN*2] ;
	m4char_t szNameId [OBL_PROPVALUE_MAX_LEN] ;
	size_t iLen ;




	iRet = m_poConfiguration -> GetType ( szType , OBL_PROPVALUE_MAX_LEN ) ;

	if ( iRet == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s","TYPE" << "??" );
		return M4_ERROR ;
	}

	iRet = m_poConfiguration -> GetStringValue ( "" , "STRINGID" , szNameId , OBL_PROPVALUE_MAX_LEN, iLen )  ;

	if (iRet == M4_ERROR )
	{
		iRet = m_poConfiguration -> SetStringValue ( "" , "STRINGID", szType ) ;
	}


	if ( ! GetName())
		SetName ( szType ) ;

	iRet = m_poConfiguration -> GetAllMatchingBlocks ( "*.*" , aiHandles , MAXBLOCKS , iNumBlocks ) ;

	if ( iRet == M4_ERROR ) 
	{
		SETCODEF ( M4_SRV_STARTUPOBL_ERROR, ERRORLOG , "Cannot find blocks into the subsystem %s" , szType ) ;
		
		return M4_ERROR ;
	}

	for ( iBlocks = 0 ; iBlocks < iNumBlocks ; iBlocks ++ ) 
	{
		if ( ! (poConfiguration = m_poConfiguration -> GetBloqConfiguration ( aiHandles [ iBlocks ] ) ) )
		{
			SETCODEF ( M4_SRV_STARTUPOBL_ERROR, ERRORLOG , "Cannot generate a configuration object from block handle %l into subsystem %s" , aiHandles [ iBlocks ] , szType ) ;

			return M4_ERROR ;
		}

		//Asigna el bloque de la obl que se trata
		poConfiguration -> m_hBlock = aiHandles [ iBlocks ] ;

		if ( poConfiguration -> GetType ( szType , OBL_PROPVALUE_MAX_LEN) == M4_ERROR )
		{
			M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s","TYPE" << "??" );
//			SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Cannot find the property 'Type' in subsystem configuration" ) ;
	
			return M4_ERROR ;
		}

		if ( M4_ERROR == (lType = m_poTables -> GetId ( szType ) ) )
		{
			SETCODEF ( M4_SRV_SS_UNKNOWN , ERRORLOG , "Subsystem %s, not defined in 'tables.obl'" , szType ) ;
			
			return M4_ERROR ;
		}

		sprintf ( szNumInsPropName , "%s%s" , szNumInstances , szType ) ;
		//La propiedad NUM_INSTANCES la miramos ahora en el subsystema padre, para saber el nº de instancias de un SS
		if ( m_poConfiguration -> GetNumericValue ( "" , szNumInsPropName  , iNumInstances ) == M4_ERROR )
		{
			iNumInstances = 1 ;
		}

		if ( iNumInstances == 0)
		{
			SETCODEF ( M4_SRV_STARTUPOBL_ERROR, ERRORLOG , "NUM_INSTANCES property of subsystem %s is asigned to 0, and it is and illegal value", szType ) ;
			return M4_ERROR ;
		}

		for ( iInst = 0 ; iInst < iNumInstances ; iInst ++ )
		{
			if ( iInst > 0 )
			{
//Aquí se debe de implementar la creación de un objeto con copia
				poConfiguration = new ClM4ConfigTree ( poConfiguration ) ;
			}

			if (! (  poSubsystem = GenerateSubsystem ( lType , poConfiguration , m_poTables ) ) )
			{
				poSubsystem = m_poSSFactory -> GenerateSubsystem ( lType , poConfiguration , m_poTables )  ;
			}
			if ( poSubsystem )
			{
//				{

					lId = ClIdGenerator::GetNewId ( ) ;


					m_oBellowLevel.insert ( INTERFACESMAP::value_type ( lId , poSubsystem ) ) ;

					//Asignar el id al eobjeto clsubsystem y al derivado de clssstatscontainer
					poSubsystem -> SetId ( lId , GetPath ( ) ) ;

					if ( M4_ERROR == poSubsystem -> Initialize () )
					{
						return M4_ERROR ;
					}

					if ( poSubsystem -> Init ( poConfiguration ) == M4_ERROR ) 
					{
						return M4_ERROR ;
					}
//				}
			}

		}

	}

	if  ( iRet == M4_ERROR )
	{
		return M4_ERROR ;
	}

	delete [] aiHandles ;



	return M4_SUCCESS ;
  //## end ClSSImplementation::Init%933665012.body
}

ClSSInterfaz * ClSSImplementation::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClSSImplementation::GenerateSubsystem%933665013.body preserve=yes
	return NULL ;
  //## end ClSSImplementation::GenerateSubsystem%933665013.body
}

void ClSSImplementation::SetInterfaz (ClSSInterfaz *ai_poInterfaz)
{
  //## begin ClSSImplementation::SetInterfaz%933665017.body preserve=yes
	m_poInterfaz = ai_poInterfaz ;
	ai_poInterfaz -> SetConfiguration ( m_poConfiguration ) ;
  //## end ClSSImplementation::SetInterfaz%933665017.body
}

ClSSInterfaz * ClSSImplementation::FindSubsystem (m4objid_t ai_lId)
{
  //## begin ClSSImplementation::FindSubsystem%936613840.body preserve=yes
	ClSSInterfaz * poSubsystem ;
	INTERFACESMAP::iterator Iterator ;



	if ( m_bActive == SS_NOT_ACCESIBLE )
		return NULL ;

	if ( m_lId == ai_lId )
		return m_poInterfaz ;

	m_oSyncBellowLevel.EnterReader ();

	Iterator = m_oBellowLevel.find ( ai_lId ) ;

	if ( Iterator != m_oBellowLevel.end ( ) )
	{
		m_oSyncBellowLevel.LeaveReader ();

		return (*Iterator).second ;
	}


	for ( Iterator = m_oBellowLevel.begin () ; Iterator != m_oBellowLevel.end ( ) ; Iterator ++ )
	{
		poSubsystem = (ClSSInterfaz*)(* Iterator).second -> FindSubsystemById ( ai_lId ) ;

		if ( poSubsystem )
		{
			m_oSyncBellowLevel.LeaveReader ();

			return poSubsystem ;
		}
	}
	m_oSyncBellowLevel.LeaveReader ();

	return NULL ;
  //## end ClSSImplementation::FindSubsystem%936613840.body
}

void ClSSImplementation::SetFactory (ClSSFactoryInterfaz *ai_poSSFactory)
{
  //## begin ClSSImplementation::SetFactory%937323528.body preserve=yes
	m_poSSFactory = ai_poSSFactory ;
  //## end ClSSImplementation::SetFactory%937323528.body
}

m4return_t ClSSImplementation::RegisterSnmp ()
{
  //## begin ClSSImplementation::RegisterSnmp%1081845655.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // gets the snmp administrator.
    M4ISnmpAdministrator* poSnmpAdministrator = M4SnmpAdminInstance::Get();
    if (NULL == poSnmpAdministrator)
    {
        // SNMP subsystem is not active.
        return M4_SUCCESS;
    }

    // gets parent subsystem.
    ClSSInterfaz* poParentSubsystem = NULL;
    m4char_t szPath[100];  
    m4int32_t iId = 0;
    strcpy(szPath , m_ssPath.c_str());
	m4pchar_t pcRef = strrchr(szPath, '.');
	if (pcRef != NULL)
	{
		(*pcRef) = 0;
		pcRef = strrchr(szPath, '.');

		if (pcRef == NULL)
		{
			pcRef = szPath;
		}
        else
		{
			pcRef++;
		}
		iId = atol(pcRef);
    }

    if (0 != iId)
    {
        if (NULL == m_poNavigator)
        {
            DUMP_CHLOG_ERRORF(M4_SNMP_NAVIGATOR_NOT_SET_IN_SUBSYSTEM, m_ssPath);
            return M4_ERROR;
        }
        poParentSubsystem = (ClSSInterfaz*)m_poNavigator->GetSubsystemById(iId);
        if (NULL == poParentSubsystem)
        {
			/* Bug 0161772
			No hay que volcar error. Puede ser que uno de los subsistemas no tenga padre snmp
            DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_PARENT_SUBSYSTEM, m_ssPath);
            return M4_ERROR;
			*/
			return M4_SUCCESS ;
        }
    }

    // read configuration.
	m4char_t szType [OBL_PROPVALUE_MAX_LEN];
	m4char_t szName [OBL_PROPVALUE_MAX_LEN];
    m4size_t iLen = 0;
    if (NULL == m_poConfiguration)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_CONFIGURATION_NOT_SET_IN_SUBSYSTEM, m_ssPath);
        return M4_ERROR;
    }
    retCode = m_poConfiguration->GetType(szType, OBL_PROPVALUE_MAX_LEN);
	if (M4_SUCCESS != retCode)
	{
        DUMP_CHLOG_ERRORF(M4_SNMP_CONFIGURATION_NOT_SET_IN_SUBSYSTEM, "szType");
		return M4_ERROR;
	}
    retCode = GetConfigStringValue ("STRINGID", szName, OBL_PROPVALUE_MAX_LEN, iLen);
	if (M4_SUCCESS != retCode)
	{
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_OBL_ITEM, "STRINGID");
		return M4_ERROR;
	}


    // register the subsystem.
    M4ISnmpObject* poParentSnmpObject = NULL;
    if (NULL != poParentSubsystem)
    {
        poParentSnmpObject = poParentSubsystem->GetSnmpObject();
    }
    m_poSnmpObject = poSnmpAdministrator->RegisterSubsystem(poParentSnmpObject, new M4SSApi4Snmp(GetInterfaz()), szType, szName);
    if (NULL != m_poSnmpObject)
    {
        RegisterSnmpProperties(poSnmpAdministrator, m_poSnmpObject);
        RegisterSnmpStatistics(poSnmpAdministrator, m_poSnmpObject);
    }

    return M4_SUCCESS;

  //## end ClSSImplementation::RegisterSnmp%1081845655.body
}

void ClSSImplementation::SetState (eStatus ai_iStatus, m4pchar_t ai_szState)
{
  //## begin ClSSImplementation::SetState%937811411.body preserve=yes
	if ( ai_iStatus > MAX_STATUS )
		return;

	m_bActive = ai_iStatus ;

	if( m_poConfiguration )
	{
		m_poConfiguration -> SetStringValue ( "" , "STATE" , ai_szState ) ;
	}
	
	return;
  //## end ClSSImplementation::SetState%937811411.body
}

void ClSSImplementation::GetState (eStatus &ao_eStatus)
{
  //## begin ClSSImplementation::GetState%939143525.body preserve=yes
	ao_eStatus = m_bActive ;
  //## end ClSSImplementation::GetState%939143525.body
}

ClSSInterfaz * ClSSImplementation::GetInterfaz ()
{
  //## begin ClSSImplementation::GetInterfaz%954148132.body preserve=yes
	return m_poInterfaz ;
  //## end ClSSImplementation::GetInterfaz%954148132.body
}

m4return_t ClSSImplementation::GetConfigNumericValue (m4pchar_t ai_pszName, m4double_t &ao_rdNumber)
{
  //## begin ClSSImplementation::GetConfigNumericValue%954230998.body preserve=yes
	if ( ! m_poConfiguration )
		return M4_ERROR ;

	return m_poConfiguration -> GetNumericValue ( "",ai_pszName, ao_rdNumber ) ;
  //## end ClSSImplementation::GetConfigNumericValue%954230998.body
}

m4return_t ClSSImplementation::GetConfigStringValue (m4pchar_t ai_pszName, m4pchar_t ao_pszString, m4size_t ai_iStringSize, m4size_t& ao_riStringLength)
{
  //## begin ClSSImplementation::GetConfigStringValue%954230999.body preserve=yes
	if ( ! m_poConfiguration )
		return M4_ERROR ;

	return m_poConfiguration -> GetStringValue ( "" , ai_pszName, ao_pszString, ai_iStringSize, ao_riStringLength ) ;
  //## end ClSSImplementation::GetConfigStringValue%954230999.body
}

m4return_t ClSSImplementation::SetConfigNumericProperty (m4pchar_t ai_szProperty, m4double_t ai_iValue)
{
  //## begin ClSSImplementation::SetConfigNumericProperty%954231003.body preserve=yes
	if ( ! m_poConfiguration )
		return M4_ERROR ;

	return m_poConfiguration -> SetNumericValue ( "" , ai_szProperty, ai_iValue) ;
  //## end ClSSImplementation::SetConfigNumericProperty%954231003.body
}

m4return_t ClSSImplementation::SetConfigStringProperty (m4pchar_t ai_szProperty, m4pchar_t ai_szValue)
{
  //## begin ClSSImplementation::SetConfigStringProperty%954231004.body preserve=yes
	if ( ! m_poConfiguration )
		return M4_ERROR ;

	return m_poConfiguration -> SetStringValue ( "" , ai_szProperty, ai_szValue) ;
  //## end ClSSImplementation::SetConfigStringProperty%954231004.body
}

int ClSSImplementation::GetInstance (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput)
{
  //## begin ClSSImplementation::GetInstance%954231007.body preserve=yes
	if ( ! m_poConfiguration )
		return M4_ERROR ;

	return m_poConfiguration -> GetInstance ( ao_pszString, ai_iStringSize, ao_riOutput ) ;
  //## end ClSSImplementation::GetInstance%954231007.body
}

ClConfiguration * ClSSImplementation::GetConfiguration ()
{
  //## begin ClSSImplementation::GetConfiguration%954324478.body preserve=yes
	return m_poConfiguration ;
  //## end ClSSImplementation::GetConfiguration%954324478.body
}

m4return_t ClSSImplementation::SetElement (m4pchar_t ai_szElement, void *ai_poElement)
{
  //## begin ClSSImplementation::SetElement%955100217.body preserve=yes
	return M4_ERROR ;
  //## end ClSSImplementation::SetElement%955100217.body
}

m4return_t ClSSImplementation::ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction)
{
  //## begin ClSSImplementation::ProcessAction%957943670.body preserve=yes
	return M4_ERROR ;
  //## end ClSSImplementation::ProcessAction%957943670.body
}

m4return_t ClSSImplementation::GetStatisticAttribValue (m4pchar_t ai_szPath, m4uint32_t ai_iStatId, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSSImplementation::GetStatisticAttribValue%994763173.body preserve=yes
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz * poSubsystem ;
	m4objid_t lId = 0;
	m4uint32_t iId = 0;
	m4return_t iRet ;
	m4char_t szPath[1024];
	m4char_t szPathAux[1024];


	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;

	szPath [ 0 ] = 0 ;

	iRet = sscanf ( ai_szPath , "%d%s" , &iId , szPath ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}
	
	if ( lId  != GetId ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}

	lId = 0 ;

	iRet = sscanf ( szPath , ".%d%s" , &iId , szPathAux ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		return GetStatisticAttribValue ( ai_iStatId, ao_poDataStorage) ;
	}

	
	//Sincronizacion de los mapas de los subsystemas

	m_oSyncBellowLevel.EnterReader ();
	
	Iterator = m_oBellowLevel.find ( lId ) ;

	if ( Iterator == m_oBellowLevel.end ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		m_oSyncBellowLevel.LeaveReader ();

		return M4_ERROR ;
	}

	poSubsystem = ( *Iterator).second ;

	iRet = poSubsystem -> GetStatisticAttribValue (szPath+1 , ai_iStatId, ao_poDataStorage) ;

	m_oSyncBellowLevel.LeaveReader ();

	return iRet ;
  //## end ClSSImplementation::GetStatisticAttribValue%994763173.body
}

m4return_t ClSSImplementation::GetStatisticAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSSImplementation::GetStatisticAttribsId%994763174.body preserve=yes
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz * poSubsystem ;
	m4objid_t lId = 0;
	m4uint32_t iId = 0;
	m4return_t iRet ;
	m4char_t szPath[1024];
	m4char_t szPathAux[1024];


	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;

	szPath [ 0 ] = 0 ;

	iRet = sscanf ( ai_szPath , "%d%s" , &iId , szPath ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}
	
	if ( lId  != GetId ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}

	lId = 0 ;

	iRet = sscanf ( szPath , ".%d%s" , &iId , szPathAux ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		return GetStatisticAttribsId (ao_poDataStorage) ;
	}

	
	//Sincronizacion de los mapas de los subsystemas

	m_oSyncBellowLevel.EnterReader ();
	
	Iterator = m_oBellowLevel.find ( lId ) ;

	if ( Iterator == m_oBellowLevel.end ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		m_oSyncBellowLevel.LeaveReader ();

		return M4_ERROR ;
	}

	poSubsystem = ( *Iterator).second ;

	iRet = poSubsystem -> GetStatisticAttribsId ( szPath + 1, ao_poDataStorage);

	m_oSyncBellowLevel.LeaveReader ();

	return iRet ;
  //## end ClSSImplementation::GetStatisticAttribsId%994763174.body
}

m4return_t ClSSImplementation::GetSubsystemAttribValue (m4pchar_t ai_szPath, m4uint16_t ai_iAttribId, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSSImplementation::GetSubsystemAttribValue%994763175.body preserve=yes
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz * poSubsystem ;
	m4objid_t lId = 0;
	m4uint32_t iId = 0;
	m4return_t iRet ;
	m4char_t szPath[1024];
	m4char_t szPathAux[1024];


	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;

	szPath [ 0 ] = 0 ;

	iRet = sscanf ( ai_szPath , "%d%s" , &iId , szPath ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}
	
	if ( lId  != GetId ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}

	lId = 0 ;

	iRet = sscanf ( szPath , ".%d%s" , &iId , szPathAux ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		return GetSubsystemAttribValue ( ai_iAttribId, ao_poDataStorage);
	}

	
	//Sincronizacion de los mapas de los subsystemas

	m_oSyncBellowLevel.EnterReader ();
	
	Iterator = m_oBellowLevel.find ( lId ) ;

	if ( Iterator == m_oBellowLevel.end ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		m_oSyncBellowLevel.LeaveReader ();

		return M4_ERROR ;
	}

	poSubsystem = ( *Iterator).second ;

	iRet = poSubsystem -> GetSubsystemAttribValue ( szPath + 1, ai_iAttribId, ao_poDataStorage);

	m_oSyncBellowLevel.LeaveReader ();

	return iRet ;
  //## end ClSSImplementation::GetSubsystemAttribValue%994763175.body
}

m4return_t ClSSImplementation::GetSubsystemAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSSImplementation::GetSubsystemAttribsId%994763176.body preserve=yes
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz * poSubsystem ;
	m4objid_t lId = 0;
	m4uint32_t iId = 0;
	m4return_t iRet ;
	m4char_t szPath[1024];
	m4char_t szPathAux[1024];


	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;

	szPath [ 0 ] = 0 ;

	iRet = sscanf ( ai_szPath , "%d%s" , &iId , szPath ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}
	
	if ( lId  != GetId ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}

	lId = 0 ;

	iRet = sscanf ( szPath , ".%d%s" , &iId , szPathAux ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		return GetSubsystemAttribsId (ao_poDataStorage);
	}

	
	//Sincronizacion de los mapas de los subsystemas

	m_oSyncBellowLevel.EnterReader ();
	
	Iterator = m_oBellowLevel.find ( lId ) ;

	if ( Iterator == m_oBellowLevel.end ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		m_oSyncBellowLevel.LeaveReader ();

		return M4_ERROR ;
	}

	poSubsystem = ( *Iterator).second ;

	iRet = poSubsystem -> GetSubsystemAttribsId ( szPath + 1, ao_poDataStorage);

	m_oSyncBellowLevel.LeaveReader ();

	return iRet ;
  //## end ClSSImplementation::GetSubsystemAttribsId%994763176.body
}

m4return_t ClSSImplementation::ExecuteCommand (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClSSImplementation::ExecuteCommand%994763181.body preserve=yes
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz * poSubsystem ;
	m4objid_t lId = 0;
	m4uint32_t iId = 0;
	m4return_t iRet ;
	m4char_t szPath[1024];
	m4char_t szPathAux[1024];


	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;

	szPath [ 0 ] = 0 ;

	iRet = sscanf ( ai_szPath , "%d%s" , &iId , szPath ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}
	
	if ( lId  != GetId ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}

	lId = 0 ;

	iRet = sscanf ( szPath , ".%d%s" , &iId , szPathAux ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		return ExecuteCommand ( ai_cCommand, ai_oParamList, ao_oResultList);
	}

	
	//Sincronizacion de los mapas de los subsystemas

	m_oSyncBellowLevel.EnterReader ();
	
	Iterator = m_oBellowLevel.find ( lId ) ;

	if ( Iterator == m_oBellowLevel.end ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		m_oSyncBellowLevel.LeaveReader ();

		return M4_ERROR ;
	}

	poSubsystem = ( *Iterator).second ;

	iRet = poSubsystem -> ExecuteCommand ( szPath+1 , ai_cCommand, ai_oParamList, ao_oResultList);

	m_oSyncBellowLevel.LeaveReader ();

	return iRet ;
  //## end ClSSImplementation::ExecuteCommand%994763181.body
}

m4return_t ClSSImplementation::GetCommandParams (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClSSImplementation::GetCommandParams%994763182.body preserve=yes
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz * poSubsystem ;
	m4objid_t lId = 0;
	m4uint32_t iId = 0;
	m4return_t iRet ;
	m4char_t szPath[1024];
	m4char_t szPathAux[1024];


	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;

	szPath [ 0 ] = 0 ;

	iRet = sscanf ( ai_szPath , "%d%s" , &iId , szPath ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}
	
	if ( lId  != GetId ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}

	lId = 0 ;

	iRet = sscanf ( szPath , ".%d%s" , &iId , szPathAux ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		return GetCommandParams ( ai_cCommand, aio_oParamList);
	}

	
	//Sincronizacion de los mapas de los subsystemas

	m_oSyncBellowLevel.EnterReader ();
	
	Iterator = m_oBellowLevel.find ( lId ) ;

	if ( Iterator == m_oBellowLevel.end ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		m_oSyncBellowLevel.LeaveReader ();

		return M4_ERROR ;
	}

	poSubsystem = ( *Iterator).second ;

	iRet = poSubsystem -> GetCommandParams ( szPath + 1, ai_cCommand, aio_oParamList);

	m_oSyncBellowLevel.LeaveReader ();

	return iRet ;
  //## end ClSSImplementation::GetCommandParams%994763182.body
}

m4return_t ClSSImplementation::GetCommandsId (m4pchar_t ai_szPath, ClAdminTagList &aio_oCommandList)
{
  //## begin ClSSImplementation::GetCommandsId%994763183.body preserve=yes
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz * poSubsystem ;
	m4objid_t lId = 0;
	m4uint32_t iId = 0;
	m4return_t iRet ;
	m4char_t szPath[1024];
	m4char_t szPathAux[1024];


/*	if ( m_bActive == SS_NOT_ACCESIBLE )
		return NULL ;
*/
	szPath [ 0 ] = 0 ;

	iRet = sscanf ( ai_szPath , "%d%s" , &iId , szPath ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}
	
	if ( lId  != GetId ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}

	lId = 0 ;

	iRet = sscanf ( szPath , ".%d%s" , &iId , szPathAux ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		return GetCommandsId ( aio_oCommandList);
	}

	
	//Sincronizacion de los mapas de los subsystemas

	m_oSyncBellowLevel.EnterReader ();
	
	Iterator = m_oBellowLevel.find ( lId ) ;

	if ( Iterator == m_oBellowLevel.end ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		m_oSyncBellowLevel.LeaveReader ();

		return M4_ERROR ;
	}

	poSubsystem = ( *Iterator).second ;

	iRet = poSubsystem -> GetCommandsId ( szPath + 1, aio_oCommandList);

	m_oSyncBellowLevel.LeaveReader ();

	return iRet ;
  //## end ClSSImplementation::GetCommandsId%994763183.body
}

m4return_t ClSSImplementation::GetSubsystemsId (m4pchar_t ai_szPath, vector <m4objid_t > &ao_poTags)
{
  //## begin ClSSImplementation::GetSubsystemsId%994763184.body preserve=yes
	INTERFACESMAP::iterator Iterator ;
	ClSSInterfaz * poSubsystem ;
	m4objid_t lId = 0;
	m4uint32_t iId = 0;
	m4return_t iRet ;
	m4char_t szPath[1024];
	m4char_t szPathAux[1024];


	if ( m_bActive == SS_NOT_ACCESIBLE )
		return M4_ERROR ;

	szPath [ 0 ] = 0 ;

	iRet = sscanf ( ai_szPath , "%d%s" , &iId , szPath ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}
	
	if ( lId  != GetId ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		return M4_ERROR ;
	}

	lId = 0 ;

	iRet = sscanf ( szPath , ".%d%s" , &iId , szPathAux ) ;

	lId = iId ;

	if  ( iRet < 1 )
	{
		return GetSubsystemsId (ao_poTags);
	}

	
	//Sincronizacion de los mapas de los subsystemas

	m_oSyncBellowLevel.EnterReader ();
	
	Iterator = m_oBellowLevel.find ( lId ) ;

	if ( Iterator == m_oBellowLevel.end ( ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Invalid subsystem path" ) ;

		m_oSyncBellowLevel.LeaveReader ();

		return M4_ERROR ;
	}

	poSubsystem = ( *Iterator).second ;

	iRet = poSubsystem -> GetSubsystemsId ( szPath + 1, ao_poTags);

	m_oSyncBellowLevel.LeaveReader ();

	return iRet ;
  //## end ClSSImplementation::GetSubsystemsId%994763184.body
}

m4pchar_t ClSSImplementation::GetssId ()
{
  //## begin ClSSImplementation::GetssId%994922158.body preserve=yes
	return (m4pchar_t) m_ssId.c_str() ;
  //## end ClSSImplementation::GetssId%994922158.body
}

m4pchar_t ClSSImplementation::GetPath ()
{
  //## begin ClSSImplementation::GetPath%994922159.body preserve=yes
	return (m4pchar_t) m_ssPath.c_str() ;
  //## end ClSSImplementation::GetPath%994922159.body
}

m4bool_t ClSSImplementation::HasOwner ()
{
  //## begin ClSSImplementation::HasOwner%996589393.body preserve=yes
	return m_bHasOwner ;
  //## end ClSSImplementation::HasOwner%996589393.body
}

m4return_t ClSSImplementation::RegisterSnmpProperties (M4ISnmpAdministrator* ai_poSnmpAdmin, M4ISnmpObject* ai_poParentSnmpObject)
{
  //## begin ClSSImplementation::RegisterSnmpProperties%1066226438.body preserve=yes
	m4double_t iNumOfAttribs = 0;
	m4char_t szPath[100];
	m4char_t szAttribute[20];
	m4char_t szName[100];
	size_t iLen;
	m4return_t iRet; 
	m4char_t szSubsytemType[100];
	
    // get subsystem type.
    m_poConfiguration->GetType(szSubsytemType, 100);
	
    // get number of attributes.
    sprintf(szPath, "%s.%s.%s", PROPERTIES_ATTRIBUTES_PATH, szSubsytemType, ATTRIBUTES);
    m_poTables->m_oBook->GetNumericValue(szPath, "NUM_ATTRIBUTES", iNumOfAttribs);

	for (m4uint16_t i=0; i<iNumOfAttribs; i++)
	{
		sprintf(szAttribute, "%s%d", ATTRIBUTE, i);
		iRet = m_poTables->m_oBook->GetStringValue(szPath, szAttribute, szName, 100, iLen);
		if (iRet == M4_ERROR)
		{
			SETCODEF(M4_SRV_SS_STATS_ERROR, ERRORLOG, "Unknown attribute %s in subsystem %s", szAttribute, szSubsytemType);
			continue;
		}

        ai_poSnmpAdmin->RegisterProperty(szName, i, ai_poParentSnmpObject, m_poConfiguration);
	}

	return M4_SUCCESS ;
    //## end ClSSImplementation::RegisterSnmpProperties%1066226438.body
}

m4return_t ClSSImplementation::RegisterSnmpStatistics (M4ISnmpAdministrator* ai_poSnmpAdmin, M4ISnmpObject* ai_poParentSnmpObject)
{
  //## begin ClSSImplementation::RegisterSnmpStatistics%1066226439.body preserve=yes
	m4uint32_t iNumOfStats = 0;
	m4char_t szPath[100];
	m4char_t szStatistic[20];
	m4char_t szName[100];
	m4char_t szValue[100];
	size_t iLen;
	m4return_t iRet; 
	m4char_t szSubsytemType[100];
	m4char_t accessOblPath[256];

    ClStatistic* poStats = GetStats();
    if (NULL == poStats)
    {
        // this subsystem has not statistics.
        return M4_SUCCESS;
    }

	// properties.obl access-permissions path.
	m_poConfiguration->GetType(szSubsytemType, 100);
	sprintf (accessOblPath, "M4ADMINISTRATOR.ACCESS-PERMISSIONS.%s.STATISTICS", szSubsytemType);
	
	for (m4uint16_t i=0; i<poStats->GetNumberOfStats(); i++)
	{
		sprintf(szStatistic, "%s%d", STATISTIC, i);

		//------------
		// check if the statistic is enable.
		// get in the properties.obl the attribute access-permisions.
		iRet = m_poTables->m_oBook->GetStringValue (accessOblPath, szStatistic, szValue, 100, iLen);
		if ((iRet==M4_SUCCESS) && (!strcmp(szValue, "NOT-ACCESSIBLE")))
        {
			continue;
        }
		//------------

		sprintf(szPath, "%s.%s.%s", PROPERTIES_STATISTICS_PATH, szSubsytemType, "STATISTICS");
		iRet = m_poTables->m_oBook->GetStringValue(szPath, szStatistic, szName, 100, iLen);
		if (iRet == M4_ERROR)
		{
			SETCODEF(M4_SRV_SS_STATS_ERROR, ERRORLOG, "Unknown statistic %s in subsystem %s", szStatistic, szSubsytemType);
			continue;
		}

        ai_poSnmpAdmin->RegisterStatistic(szName, i, ai_poParentSnmpObject, m_poConfiguration);
	}

	return M4_SUCCESS ;
  //## end ClSSImplementation::RegisterSnmpStatistics%1066226439.body
}

m4return_t ClSSImplementation::RegisterSnmpCommands (M4ISnmpAdministrator* ai_poSnmpAdmin, M4ISnmpObject* ai_poParentSnmpObject)
{
  //## begin ClSSImplementation::RegisterSnmpCommands%1066226440.body preserve=yes
	return M4_ERROR;
  //## end ClSSImplementation::RegisterSnmpCommands%1066226440.body
}

M4ISnmpObject* ClSSImplementation::GetSnmpObject ()
{
  //## begin ClSSImplementation::GetSnmpObject%1081862806.body preserve=yes
    return m_poSnmpObject;
  //## end ClSSImplementation::GetSnmpObject%1081862806.body
}

m4return_t ClSSImplementation::UnRegisterSnmp ()
{
  //## begin ClSSImplementation::UnRegisterSnmp%1083593888.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // checks if exist a snmp object.
    if (NULL == m_poSnmpObject)
    {
        return M4_SUCCESS;
    }

    // gets the snmp administrator.
    M4ISnmpAdministrator* poSnmpAdministrator = M4SnmpAdminInstance::Get();
    if (NULL == poSnmpAdministrator)
    {
        // SNMP subsystem is not active.
        return M4_SUCCESS;
    }

    retCode = poSnmpAdministrator->UnregisterSubsystem(m_poSnmpObject);

	// bg 179508
	m_poSnmpObject = NULL ;

    return retCode;
  //## end ClSSImplementation::UnRegisterSnmp%1083593888.body
}

// Additional Declarations
  //## begin ClSSImplementation%37A54BCB0070.declarations preserve=yes
  //## end ClSSImplementation%37A54BCB0070.declarations

//## begin module%37A556160015.epilog preserve=yes
//## end module%37A556160015.epilog
