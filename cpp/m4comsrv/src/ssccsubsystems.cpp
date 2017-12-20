//## begin module%38D787BD00E2.cm preserve=no
//## end module%38D787BD00E2.cm

//## begin module%38D787BD00E2.cp preserve=no
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
//## end module%38D787BD00E2.cp

//## Module: ssccsubsystems%38D787BD00E2; Package body
//## Subsystem: ssccsubsystems::src%38D7880B0333
//## Source file: F:\integration\m4comsrv\src\ssccsubsystems.cpp

//## begin module%38D787BD00E2.additionalIncludes preserve=no
//## end module%38D787BD00E2.additionalIncludes

//## begin module%38D787BD00E2.includes preserve=yes
#include <ssstats.hpp>
#include <m4servenv.hpp>
#include <m4srvres.hpp>
#include <cllocalmonitor.hpp>
#include <ssvisitors.hpp>
#include <cloblconfiguration.hpp>
#include <clserviceinfo.hpp>
#include <ssnames.hpp>
#include <clssinvoker.hpp>
#include <clssinvoker.hpp>
#include <m4actionconstants.hpp>
//## end module%38D787BD00E2.includes

// FIFOQ
#include <fifoq.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
// clssaction
#include <clssaction.hpp>
// ssccsubsystems
#include <ssccsubsystems.hpp>

class ClCSSessManager;
class ClSessManagerVisitor;

//## begin module%38D787BD00E2.declarations preserve=no
//## end module%38D787BD00E2.declarations

//## begin module%38D787BD00E2.additionalDeclarations preserve=yes
//## end module%38D787BD00E2.additionalDeclarations


// Class ClAdminsListSS 


ClAdminsListSS::ClAdminsListSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClAdminsListSS::ClAdminsListSS%882177041.hasinit preserve=no
  //## end ClAdminsListSS::ClAdminsListSS%882177041.hasinit
  //## begin ClAdminsListSS::ClAdminsListSS%882177041.initialization preserve=yes
  : ClSSImplementation ( ai_poConfiguration, ai_poTables , ai_poNavigator )
  //## end ClAdminsListSS::ClAdminsListSS%882177041.initialization
{
  //## begin ClAdminsListSS::ClAdminsListSS%882177041.body preserve=yes
  //## end ClAdminsListSS::ClAdminsListSS%882177041.body
}


ClAdminsListSS::~ClAdminsListSS ()
{
  //## begin ClAdminsListSS::~ClAdminsListSS%882177042.body preserve=yes
  //## end ClAdminsListSS::~ClAdminsListSS%882177042.body
}



//## Other Operations (implementation)
void ClAdminsListSS::SetControlQueue (ClControlQ *ai_poControlQueue)
{
  //## begin ClAdminsListSS::SetControlQueue%882177043.body preserve=yes
  m_poControlQueue = ai_poControlQueue ;
  //## end ClAdminsListSS::SetControlQueue%882177043.body
}

ClControlQ * ClAdminsListSS::GetControlQueue ()
{
  //## begin ClAdminsListSS::GetControlQueue%882177044.body preserve=yes
	return m_poControlQueue ;
  //## end ClAdminsListSS::GetControlQueue%882177044.body
}

m4return_t ClAdminsListSS::Synchronize ()
{
  //## begin ClAdminsListSS::Synchronize%884942688.body preserve=yes
	return  ClSSImplementation::Synchronize () ;
  //## end ClAdminsListSS::Synchronize%884942688.body
}

m4return_t ClAdminsListSS::Initialize ()
{
  //## begin ClAdminsListSS::Initialize%885806466.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	
//	m_poVisitor = new ClAcceptorListVisitor ;

//	m_poStatistics =  new ClAcceptorListStats ;

	return M4_SUCCESS ;
  //## end ClAdminsListSS::Initialize%885806466.body
}

m4return_t ClAdminsListSS::GetCommandsId (M4DataStorage *ai_poOutStorage)
{
  //## begin ClAdminsListSS::GetCommandsId%885806486.body preserve=yes
	return TOTAL_COMMANDS ;
  //## end ClAdminsListSS::GetCommandsId%885806486.body
}

m4return_t ClAdminsListSS::ShutDown ()
{
  //## begin ClAdminsListSS::ShutDown%885806510.body preserve=yes
	return ClSSImplementation::ShutDown () ;
  //## end ClAdminsListSS::ShutDown%885806510.body
}

m4return_t ClAdminsListSS::MoveOutAdministrator (m4uint32_t ai_lIdAdministrator)
{
  //## begin ClAdminsListSS::MoveOutAdministrator%888569455.body preserve=yes
return M4_ERROR ;
  //## end ClAdminsListSS::MoveOutAdministrator%888569455.body
}

m4return_t ClAdminsListSS::GetCommandParams (m4char_t ai_cCommand, M4DataStorage *ao_poDataStorage)
{
  //## begin ClAdminsListSS::GetCommandParams%890917794.body preserve=yes


	M4_ASSERT ( 0 );
/*	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;


	switch ( ai_cCommand )
	{
	case MOVE_OUT_ADMINISTRATOR :
		{
		if (! ( pLiteral = m_poTables -> GetLiteral ( ADMINISTRATOR_ID )  ) )
		{
			pLiteral = ADMINISTRATOR_ID ;
		}


		iSize = strlen ( pLiteral) + 7 ;

		//Tomar un buffer para la respuesta
		ao_poDataStorage -> GetNewBuffer( buffer , iSize );


		//parámetro 0, tipo 2, literal y valor por defecto
		sprintf ( buffer , "0,2,%s,0;" , pLiteral  ) ;

		ao_poDataStorage -> Add ( buffer , iSize , M4_TRUE ) ;

		}

	default :

		ao_poDataStorage -> GetNewBuffer( buffer , 1  );

		buffer [0] = 0 ;

		ao_poDataStorage -> Add ( buffer , strlen (buffer ) +1 , M4_TRUE ) ;
	}
*/
	return M4_SUCCESS ;
  //## end ClAdminsListSS::GetCommandParams%890917794.body
}

m4return_t ClAdminsListSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClAdminsListSS::ExecuteCommand%901709419.body preserve=yes
	return M4_SUCCESS ;
  //## end ClAdminsListSS::ExecuteCommand%901709419.body
}

m4return_t ClAdminsListSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClAdminsListSS::GetCommandsId%901709420.body preserve=yes
	return M4_SUCCESS ;
  //## end ClAdminsListSS::GetCommandsId%901709420.body
}

m4return_t ClAdminsListSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClAdminsListSS::GetCommandParams%901709421.body preserve=yes
	return M4_SUCCESS ;
  //## end ClAdminsListSS::GetCommandParams%901709421.body
}

// Class ClAdminSS 


ClAdminSS::ClAdminSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClAdminSS::ClAdminSS%882177071.hasinit preserve=no
  //## end ClAdminSS::ClAdminSS%882177071.hasinit
  //## begin ClAdminSS::ClAdminSS%882177071.initialization preserve=yes
  : ClSSImplementation ( ai_poConfiguration, ai_poTables , ai_poNavigator)
  //## end ClAdminSS::ClAdminSS%882177071.initialization
{
  //## begin ClAdminSS::ClAdminSS%882177071.body preserve=yes
  //## end ClAdminSS::ClAdminSS%882177071.body
}


ClAdminSS::~ClAdminSS ()
{
  //## begin ClAdminSS::~ClAdminSS%882177072.body preserve=yes
  //## end ClAdminSS::~ClAdminSS%882177072.body
}



//## Other Operations (implementation)
ClCSAdministrator * ClAdminSS::GetAdministrator ()
{
  //## begin ClAdminSS::GetAdministrator%882177073.body preserve=yes
  return m_poAdministrator ;
  //## end ClAdminSS::GetAdministrator%882177073.body
}

void ClAdminSS::SetAdministrator (ClCSAdministrator *ai_poAdministrator)
{
  //## begin ClAdminSS::SetAdministrator%882177074.body preserve=yes
  m_poAdministrator = ai_poAdministrator ;
  //## end ClAdminSS::SetAdministrator%882177074.body
}

void ClAdminSS::Accept (ClVisitor &ai_oVisitor)
{
  //## begin ClAdminSS::Accept%884777556.body preserve=yes
  //## end ClAdminSS::Accept%884777556.body
}

m4return_t ClAdminSS::Synchronize ()
{
  //## begin ClAdminSS::Synchronize%884942694.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	if ( m_poVisitor )
	{
		Accept ( *m_poVisitor ) ;
	}

	return M4_SUCCESS; 
  //## end ClAdminSS::Synchronize%884942694.body
}

m4return_t ClAdminSS::Initialize ()
{
  //## begin ClAdminSS::Initialize%885806471.body preserve=yes
	
	
	return M4_SUCCESS ;
  //## end ClAdminSS::Initialize%885806471.body
}

m4return_t ClAdminSS::GetCommandsId (M4DataStorage *ai_poOutStorage)
{
  //## begin ClAdminSS::GetCommandsId%885806491.body preserve=yes
	return TOTAL_COMMANDS ;
  //## end ClAdminSS::GetCommandsId%885806491.body
}

m4return_t ClAdminSS::ShutDown ()
{
  //## begin ClAdminSS::ShutDown%885806516.body preserve=yes
	return M4_SUCCESS ;
  //## end ClAdminSS::ShutDown%885806516.body
}

m4return_t ClAdminSS::GetCommandParams (m4char_t ai_cCommand, M4DataStorage *ao_poDataStorage)
{
  //## begin ClAdminSS::GetCommandParams%890917800.body preserve=yes

M4_ASSERT ( 0 ) ;
	return M4_SUCCESS ;
  //## end ClAdminSS::GetCommandParams%890917800.body
}

m4return_t ClAdminSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClAdminSS::ExecuteCommand%901709434.body preserve=yes
	return M4_SUCCESS ;
  //## end ClAdminSS::ExecuteCommand%901709434.body
}

m4return_t ClAdminSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClAdminSS::GetCommandsId%901709435.body preserve=yes
	return M4_SUCCESS ;
  //## end ClAdminSS::GetCommandsId%901709435.body
}

m4return_t ClAdminSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClAdminSS::GetCommandParams%901709436.body preserve=yes
	return M4_SUCCESS ;
  //## end ClAdminSS::GetCommandParams%901709436.body
}

m4return_t ClAdminSS::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClAdminSS::Stop%928140263.body preserve=yes

	if (! ai_lDeferedTime )
	{
		if ( m_poAdministrator )
		{
			if (  ! m_poAdministrator -> IsExit () )
			{
				m_poAdministrator -> Terminate () ;

				if ( m_poAdministrator -> GetStringsId () )
					cout << "Stopping " << m_poAdministrator -> GetStringsId () << " ...\n" << flush ;

			}
		}
	}

	return M4_SUCCESS ;
  //## end ClAdminSS::Stop%928140263.body
}

m4bool_t ClAdminSS::IsReadyToClose ()
{
  //## begin ClAdminSS::IsReadyToClose%928140264.body preserve=yes
	
	if (! m_bReadyToClose )
	{
//		m_bReadyToClose = ! m_poAdministrator -> IsRunning ( ) ;
		m_bReadyToClose = ( m_poAdministrator -> IsRunning ( ) ) ? M4_FALSE : M4_TRUE;

		if ( ( m_bReadyToClose ) && ( m_poAdministrator -> GetStringsId ()  ) )
		{
			cout << m_poAdministrator -> GetStringsId () << " Stopped\n" << flush ;
		}
	}

	return m_bReadyToClose;
  //## end ClAdminSS::IsReadyToClose%928140264.body
}

// Class ClSessManagerSS 


ClSessManagerSS::ClSessManagerSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClSessManagerSS::ClSessManagerSS%882177049.hasinit preserve=no
  //## end ClSessManagerSS::ClSessManagerSS%882177049.hasinit
  //## begin ClSessManagerSS::ClSessManagerSS%882177049.initialization preserve=yes
  :ClSSImplementation ( ai_poConfiguration, ai_poTables , NULL)
  //## end ClSessManagerSS::ClSessManagerSS%882177049.initialization
{
  //## begin ClSessManagerSS::ClSessManagerSS%882177049.body preserve=yes
	ClSSInvoker * poInvoker  = ClSSInvoker::GetInstance ( ) ;
	poInvoker -> RegisterAction ( CONTROL_REQUEST_ACTION ,CONTROL_REQUESST_ACTION_ID , this ) ;
	
  //## end ClSessManagerSS::ClSessManagerSS%882177049.body
}


ClSessManagerSS::~ClSessManagerSS ()
{
  //## begin ClSessManagerSS::~ClSessManagerSS%882177050.body preserve=yes
  //## end ClSessManagerSS::~ClSessManagerSS%882177050.body
}



//## Other Operations (implementation)
ClCSSessManager * ClSessManagerSS::GetSessManager ()
{
  //## begin ClSessManagerSS::GetSessManager%882177051.body preserve=yes
  return m_poSessManager ;
  //## end ClSessManagerSS::GetSessManager%882177051.body
}

void ClSessManagerSS::SetSessManager (ClCSSessManager *ai_poSessManager)
{
  //## begin ClSessManagerSS::SetSessManager%882177052.body preserve=yes
  m_poSessManager = ai_poSessManager;
  //## end ClSessManagerSS::SetSessManager%882177052.body
}

void ClSessManagerSS::Accept (ClSessManagerVisitor &ai_oVisitor)
{
  //## begin ClSessManagerSS::Accept%884777552.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return ;

	ai_oVisitor.VisitSessManager ( * GetSessManager ( ) ) ;
  //## end ClSessManagerSS::Accept%884777552.body
}

m4return_t ClSessManagerSS::Synchronize ()
{
  //## begin ClSessManagerSS::Synchronize%884942690.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	if ( m_poVisitor )
	{
		Accept ( * ( ( ClSessManagerVisitor * ) m_poVisitor ) ) ;
	}

	return M4_SUCCESS; 
  //## end ClSessManagerSS::Synchronize%884942690.body
}

m4return_t ClSessManagerSS::Initialize ()
{
  //## begin ClSessManagerSS::Initialize%885806468.body preserve=yes
	eStatus eState ;
	m4return_t iRet;
	ClLocalMonitor * poLocalMonitor = ClLocalMonitor::Instance() ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	
	m_poStatistics = new ClSessManagerStats ;

//	MOD	EMN	980508	poSessManager = new  ClCSSessManager ( (ClLocalMonitor*) m_manager ,( (ClLocalMonitor*) m_manager ) -> GetControlQueue ( ) , poSendQueue , poStats );
	m_poSessManager = new  ClCSSessManager (  poLocalMonitor  ,poLocalMonitor -> GetControlQueue ( ) , m_poStatistics );

	iRet = poLocalMonitor  -> m_oSessionManagerList.AddItem ( m_poSessManager ) ; 
	
	if (iRet == M4_ERROR)
	{
		return M4_ERROR;
	}

	m_poVisitor = new ClSessManagerVisitor ;

	
	return M4_SUCCESS ;
  //## end ClSessManagerSS::Initialize%885806468.body
}

m4return_t ClSessManagerSS::GetCommandsId (M4DataStorage *ai_poOutStorage)
{
  //## begin ClSessManagerSS::GetCommandsId%885806488.body preserve=yes
	return TOTAL_COMMANDS ;
  //## end ClSessManagerSS::GetCommandsId%885806488.body
}

m4return_t ClSessManagerSS::ShutDown ()
{
  //## begin ClSessManagerSS::ShutDown%885806512.body preserve=yes
	m4return_t iRet ;
	ClLocalMonitor * poLocalMonitor ;

	poLocalMonitor = ClLocalMonitor::Instance () ;

	iRet = poLocalMonitor -> ShutDownSessionManager (  GetSessManager ( ) ) ;

	return iRet ;
  //## end ClSessManagerSS::ShutDown%885806512.body
}

m4return_t ClSessManagerSS::GetCommandParams (m4char_t ai_cCommand, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSessManagerSS::GetCommandParams%890917796.body preserve=yes
M4_ASSERT ( 0 ) ;

	return M4_SUCCESS ;
  //## end ClSessManagerSS::GetCommandParams%890917796.body
}

m4return_t ClSessManagerSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClSessManagerSS::ExecuteCommand%901709425.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSessManagerSS::ExecuteCommand%901709425.body
}

m4return_t ClSessManagerSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClSessManagerSS::GetCommandsId%901709426.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSessManagerSS::GetCommandsId%901709426.body
}

m4return_t ClSessManagerSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClSessManagerSS::GetCommandParams%901709427.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSessManagerSS::GetCommandParams%901709427.body
}

m4return_t ClSessManagerSS::Start ()
{
  //## begin ClSessManagerSS::Start%927293861.body preserve=yes
	m4char_t szThreadName [20] ;
	static m4uint32_t IdThread = 1;
//	ClSubsystemBase * poSubsystem ;

/*
	if (!  (poSubsystem = m_poNavigator -> GetSubsystemByType ( M4_LM_SUBSYSTEM ) ) )
		return M4_ERROR ;

	m_poDispatcherSS = poSubsystem ;
*/
	
	sprintf (szThreadName , "Session Manager %d" , IdThread ) ;


	m_poSessManager -> Start ( szThreadName ) ;

	cout << "Session manager started\n" << flush ;

	IdThread ++ ;

	return M4_SUCCESS ;
  //## end ClSessManagerSS::Start%927293861.body
}

m4return_t ClSessManagerSS::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClSessManagerSS::Stop%928140257.body preserve=yes
	if (! ai_lDeferedTime )
	{
		if ( m_poSessManager )
		{
			m_poSessManager -> Terminate () ;
		}

		cout << "Stopping Session Manager ...\n" << flush ;
	}

	return M4_SUCCESS ;
  //## end ClSessManagerSS::Stop%928140257.body
}

m4bool_t ClSessManagerSS::IsReadyToClose ()
{
  //## begin ClSessManagerSS::IsReadyToClose%928140258.body preserve=yes
	
	if (! m_bReadyToClose )
	{
		m_bReadyToClose = ( m_poSessManager -> IsRunning ( ) ) ? M4_FALSE : M4_TRUE;

		if ( ( m_bReadyToClose ) && ( m_poSessManager -> GetStringsId () ) )
		{
			cout << m_poSessManager -> GetStringsId () << " Stopped\n" << flush ;
		}
	}

	if (! m_bReadyToClose )
	{
		return m_bReadyToClose;
	}
	else
	{
		return 	ClSSImplementation::IsReadyToClose() ;
	}

  //## end ClSessManagerSS::IsReadyToClose%928140258.body
}

void ClSessManagerSS::Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject)
{
  //## begin ClSessManagerSS::Update%941813352.body preserve=yes

	switch ( ai_iEvent )
	{
	case GREEN_MEMORY_LEVEL :
	case YELLOW_MEMORY_LEVEL :
		//Solamente si estamos en memoria Critica, se para al session manager
		m_poSessManager -> SetState ( SessManagerActive ) ;
		SetState ( SS_ACTIVE , SS_STR_ACTIVE ) ;
		break ;

	case RED_MEMORY_LEVEL :
		m_poSessManager -> SetState ( SessManagerPaused ) ;
		SetState ( SS_PAUSED , SS_STR_PAUSED ) ;
		break;
	}
	
  //## end ClSessManagerSS::Update%941813352.body
}

m4return_t ClSessManagerSS::ProcessItem (ClBaseRequest *ai_oItem, m4bool_t &ai_bReply)
{
  //## begin ClSessManagerSS::ProcessItem%954935289.body preserve=yes
	return m_poSessManager -> ProcessItem ( ai_oItem ) ;
  //## end ClSessManagerSS::ProcessItem%954935289.body
}

m4return_t ClSessManagerSS::CreateAction (m4pchar_t ai_szActionType, ClBaseAction *&ao_poAction)
{
  //## begin ClSessManagerSS::CreateAction%955440324.body preserve=yes
	
	if ( ! stricmp ( CONTROL_REQUEST_ACTION , ai_szActionType ) )
	{
		ao_poAction = new ClControlRequestAction ( this ) ;

		return M4_SUCCESS ;
	}

	return M4_ERROR ;
  //## end ClSessManagerSS::CreateAction%955440324.body
}

m4return_t ClSessManagerSS::ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction)
{
  //## begin ClSessManagerSS::ProcessAction%957943672.body preserve=yes

	switch ( ai_oActionIdentifier ) 
	{
	case CONTROL_REQUESST_ACTION_ID :

		ao_poAction = new ClControlRequestAction ( this ) ;

		break ;
	}

	ao_poAction -> SetParams ( ai_poParamList ) ;

	return ao_poAction -> Execute ( );
  //## end ClSessManagerSS::ProcessAction%957943672.body
}

// Class ClControlRequestAction 

ClControlRequestAction::ClControlRequestAction (ClSSImplementation *ai_poExecutive)
  //## begin ClControlRequestAction::ClControlRequestAction%955440322.hasinit preserve=no
  //## end ClControlRequestAction::ClControlRequestAction%955440322.hasinit
  //## begin ClControlRequestAction::ClControlRequestAction%955440322.initialization preserve=yes
  :ClSSAction ( ai_poExecutive )
  //## end ClControlRequestAction::ClControlRequestAction%955440322.initialization
{
  //## begin ClControlRequestAction::ClControlRequestAction%955440322.body preserve=yes
  //## end ClControlRequestAction::ClControlRequestAction%955440322.body
}



//## Other Operations (implementation)
m4return_t ClControlRequestAction::Execute ()
{
  //## begin ClControlRequestAction::Execute%955440323.body preserve=yes
	ClBaseRequest  * poRequest ;
	void * poItem ;
	m4bool_t bReply ;


	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , poItem ) )
	{
		return M4_ERROR ;
	}

	if ( ! ( poRequest = (ClBaseRequest*) poItem ) )
	{
		return M4_ERROR ;
	}


	return ((ClSessManagerSS*)m_poExecutive )-> ProcessItem ( poRequest , bReply) ;
  //## end ClControlRequestAction::Execute%955440323.body
}

// Additional Declarations
  //## begin ClControlRequestAction%38F34B9902C7.declarations preserve=yes
  //## end ClControlRequestAction%38F34B9902C7.declarations

//## begin module%38D787BD00E2.epilog preserve=yes
//## end module%38D787BD00E2.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClSessManagerListSS::ClSessManagerListSS%882177045.initialization preserve=no
  : ClSSImplementation ( ai_poConfiguration , ai_poTables , NULL)
//## end ClSessManagerListSS::ClSessManagerListSS%882177045.initialization

//## begin ClSessManagerListSS::ClSessManagerListSS%882177045.body preserve=no
	m_poControlQueue = NULL ;
//## end ClSessManagerListSS::ClSessManagerListSS%882177045.body

//## begin ClSessManagerListSS::GetControlQueue%882177047.body preserve=no
	return m_poControlQueue ;
//## end ClSessManagerListSS::GetControlQueue%882177047.body

//## begin ClSessManagerListSS::SetControlQueue%882177048.body preserve=no
  m_poControlQueue = ai_poControlQueue ;
//## end ClSessManagerListSS::SetControlQueue%882177048.body

//## begin ClSessManagerListSS::Accept%884777551.body preserve=no
  ai_oVisitor.VisitSessManagerList ( * this ) ;
//## end ClSessManagerListSS::Accept%884777551.body

//## begin ClSessManagerListSS::Synchronize%884942689.body preserve=no
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;
	

	if ( m_poVisitor )
	{
		Accept ( * ( ( ClSessManagerListVisitor * ) m_poVisitor ) ) ;
	}

	return 	ClSSImplementation::Synchronize () ;
//## end ClSessManagerListSS::Synchronize%884942689.body

//## begin ClSessManagerListSS::Initialize%885806467.body preserve=no
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;
	
	
	m_poVisitor = new ClSessManagerListVisitor ;

	m_poStatistics = new ClSessManagerListStats ;

	
	return M4_SUCCESS ;
//## end ClSessManagerListSS::Initialize%885806467.body

//## begin ClSessManagerListSS::GetCommandsId%885806487.body preserve=no
	return TOTAL_COMMANDS ;
//## end ClSessManagerListSS::GetCommandsId%885806487.body

//## begin ClSessManagerListSS::ShutDown%885806511.body preserve=no
	return 	ClSSImplementation::ShutDown () ;
//## end ClSessManagerListSS::ShutDown%885806511.body

//## begin ClSessManagerListSS::SetQueueSize%888569456.body preserve=no
	return M4_ERROR ;
//## end ClSessManagerListSS::SetQueueSize%888569456.body

//## begin ClSessManagerListSS::EmptyQueue%888569457.body preserve=no
	return M4_ERROR ;
//## end ClSessManagerListSS::EmptyQueue%888569457.body

//## begin ClSessManagerListSS::GetCommandParams%890917795.body preserve=no

M4_ASSERT ( 0 ) ;

	return M4_SUCCESS ;
//## end ClSessManagerListSS::GetCommandParams%890917795.body

//## begin ClSessManagerListSS::ExecuteCommand%901709422.body preserve=no
	return M4_SUCCESS ;
//## end ClSessManagerListSS::ExecuteCommand%901709422.body

//## begin ClSessManagerListSS::GetCommandsId%901709423.body preserve=no
	return M4_SUCCESS ;
//## end ClSessManagerListSS::GetCommandsId%901709423.body

//## begin ClSessManagerListSS::GetCommandParams%901709424.body preserve=no
	return M4_SUCCESS ;
//## end ClSessManagerListSS::GetCommandParams%901709424.body

//## begin ClSessManagerListSS::IsReadyToClose%928225053.body preserve=no
	return ClSSImplementation::IsReadyToClose() ;
//## end ClSessManagerListSS::IsReadyToClose%928225053.body

//## begin ClSessManagerListSS::Stop%930566406.body preserve=no
	if ( M4_ERROR == ClSSImplementation::Stop (ai_lDeferedTime) )
	{
		return M4_ERROR ;
	}

	if ( ! ai_lDeferedTime )
	{
		if ( m_poControlQueue )
		{
			m_poControlQueue -> Lock () ;

			m_poControlQueue -> ReleaseAll() ;
		}
	}

	return M4_SUCCESS ;


//## end ClSessManagerListSS::Stop%930566406.body

#endif
