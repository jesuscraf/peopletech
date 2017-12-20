//==============================================================================
//
// (c) Copyright  1991-2009 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4dm.dll
// File:             usractaudit.cpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             08/10/09
// Language:         C++
// Operating System: Windows, Unix
// Design Document:
//
//
// Definition:
//
//    Implementación de las clases de auditoria de acciones de usuario.
//
//==============================================================================

#include "usractaudit.hpp"
#include "cldmcontext.hpp"
#include "access.hpp"
#include "m4mdrt.hpp"
#include "cllgadap.hpp"
#include "nodedef.hpp"
#include "m4logsys.hpp"
#include "chan_knl.hpp"
#include "iexecutor.hpp"
#include "m4cryptc.hpp"

// -----------------------------------------------------------------------------
// -- Macros generales
// -----------------------------------------------------------------------------
#define CHECK_RETURN(r)		do{ if( r != M4_SUCCESS ){ return M4_ERROR ; } } while( 0 )
#define CHECK_POINTER(p)	do{ if( p == NULL ){ return M4_ERROR ; } } while( 0 )

#define SESSION_CHANNEL_NODE_ID		"ROOT_SESSION"

#define M4_SYSTEM_INFO_FIELDS_NUM	11



// *****************************************************************************
// **
// **	Class ClUserAction
// **
// *****************************************************************************

m4pcchar_t ClUserAction::s_apccAuditEventNames[] = { "LOGON", "LOGOFF", "LOGON_IMPERSONATED", "LOGON_FAILED_USERNAME", "LOGON_FAILED_PASSWORD", "M4OBJECT", "METHOD", "TASK", "PRESENTATION", "WEB_PAGE", "JOB", "LOG", "WEB_MENU" } ;

// -----------------------------------------------------------------------------
// -- Public methods
// -----------------------------------------------------------------------------

ClUserAction::ClUserAction()
{
	m_pcOrganization	= "" ;
	m_eEventID			= EV_NOEV ;
	m_pcEventObject		= "" ;
	m_pcEventObjectName	= "" ;
	m_pcEventArgs		= "" ;
	m_dDate = ClActualDate() ;
	m_bIsValid = M4_FALSE ;
}


ClUserAction::ClUserAction( m4pcchar_t ai_pccOrganization, m4pcchar_t ai_pccRole, eAuditEventID ai_eEventID, m4pcchar_t ai_pccObject, m4pcchar_t ai_pccObjectName, m4pcchar_t ai_pccArgs, m4bool_t ai_bIsValid )
{
	SetOrganization( ai_pccOrganization ) ;
	SetRole ( ai_pccRole ) ;
	SetEventID( ai_eEventID ) ;
	SetEventObject( ai_pccObject ) ;
	SetEventObjectName( ai_pccObjectName ) ;
	SetEventArgs( ai_pccArgs ) ;
	m_dDate = ClActualDate() ;
	m_bIsValid = ai_bIsValid ;
}


ClUserAction::~ClUserAction()
{
}


void	ClUserAction::SetOrganization( m4pcchar_t ai_pccOrganization )
{
	if( ai_pccOrganization == NULL )
	{
		ai_pccOrganization = "" ;
	}

	m_pcOrganization = ai_pccOrganization ;
}

void	ClUserAction::SetRole( m4pcchar_t ai_pccRole ) 
{
	if( ai_pccRole == NULL )
	{
		ai_pccRole = "" ;
	}

	m_pcRole = ai_pccRole ;
}

void	ClUserAction::SetEventID( eAuditEventID ai_eEventID )
{
	m_eEventID = ai_eEventID ;
}


void	ClUserAction::SetEventObject( m4pcchar_t ai_pccObject )
{
	if( ai_pccObject == NULL )
	{
		ai_pccObject = "" ;
	}

	m_pcEventObject = ai_pccObject ;
}


void	ClUserAction::SetEventObjectName( m4pcchar_t ai_pccObjectName )
{
	if( ai_pccObjectName == NULL )
	{
		ai_pccObjectName = "" ;
	}

	m_pcEventObjectName = ai_pccObjectName ;
}


void	ClUserAction::AppendEventObject( m4pcchar_t ai_pccObject )
{
	if( ai_pccObject != NULL )
	{
		m_pcEventObject += ai_pccObject ;
	}
}


void	ClUserAction::AppendEventObjectName( m4pcchar_t ai_pccObjectName )
{
	if( ai_pccObjectName != NULL )
	{
		m_pcEventObjectName += ai_pccObjectName ;
	}
}


void	ClUserAction::SetEventArgs( m4pcchar_t ai_pccArgs )
{
	if( ai_pccArgs == NULL )
	{
		ai_pccArgs = "" ;
	}

	m_pcEventArgs = ai_pccArgs ;
}


void	ClUserAction::AppendEventArgs( m4pcchar_t ai_pccArgs )
{
	if( ai_pccArgs != NULL )
	{
		m_pcEventArgs += ai_pccArgs ;
	}
}


void	ClUserAction::SetDate( m4date_t ai_dDate )
{
	m_dDate = ai_dDate ;
}


void	ClUserAction::SetIsValid( m4bool_t ai_bIsValid )
{
	m_bIsValid = ai_bIsValid ;
}

	
m4pcchar_t ClUserAction::GetOrganization() const
{
	return m_pcOrganization.c_str() ;
}

m4pcchar_t ClUserAction::GetRole() const
{
	return m_pcRole.c_str() ;
}

ClUserAction::eAuditEventID ClUserAction::GetEventID( void ) const
{
	return m_eEventID ;
}


m4pcchar_t	ClUserAction::GetEventObject( void ) const
{
	return m_pcEventObject.c_str() ;
}


m4pcchar_t	ClUserAction::GetEventObjectName( void ) const
{
	return m_pcEventObjectName.c_str() ;
}


m4pcchar_t	ClUserAction::GetEventArgs( void ) const
{
	return m_pcEventArgs.c_str() ;
}


m4date_t	ClUserAction::GetDate( void ) const
{
	return m_dDate ;
}


m4bool_t	ClUserAction::GetIsValid( void ) const
{
	return m_bIsValid ;
}

m4pcchar_t	ClUserAction::GetEventName( void ) const
{
	m4pcchar_t	pcName = NULL ;

	if( m_eEventID > EV_NOEV && m_eEventID < (sizeof(s_apccAuditEventNames)/sizeof(s_apccAuditEventNames[0])) )
	{
		pcName = s_apccAuditEventNames[ m_eEventID ] ;
	}

	return pcName ;
}





// *****************************************************************************
// **
// **	Class ClUserTimedAction
// **
// *****************************************************************************

// -----------------------------------------------------------------------------
// -- Public methods
// -----------------------------------------------------------------------------

ClUserTimedAction::ClUserTimedAction()
{
	m_iEventNum = 0 ;
	m_dtEventTime = ClMinusInfiniteDate() ;
	m_pcOrganization = "" ;
	m_pcEventSenderContainer = "" ;
	m_pcEventSender = "" ;
	m_pcParentEventSenderContainer = "" ;
	m_pcParentEventSender = "" ;
	m_iTimeElapsed = 0 ;
	m_pcBuffer = "" ;
	m_bIsValid = M4_FALSE ;
}


ClUserTimedAction::ClUserTimedAction( long ai_iEventNum, m4date_t ai_dEventTime, m4pcchar_t ai_pccOrganization, m4pcchar_t ai_pccEventSenderContainer, m4pcchar_t ai_pccEventSender, m4pcchar_t ai_pccParentEventSenderContainer, m4pcchar_t ai_pccParentEventSender, long ai_iTimeElapsed, m4pcchar_t ai_pccBuffer, m4bool_t ai_bIsValid )
{
	m_iEventNum = ai_iEventNum ;
	m_dtEventTime = ai_dEventTime ;
	SetOrganization( ai_pccOrganization ) ;
	SetEventSenderContainer( ai_pccEventSenderContainer ) ;
	SetEventSender( ai_pccEventSender ) ;
	SetParentEventSenderContainer( ai_pccParentEventSenderContainer ) ;
	SetParentEventSender( ai_pccParentEventSender ) ;
	SetTimeElapsed( ai_iTimeElapsed );
	SetBuffer( ai_pccBuffer ) ;
	m_bIsValid = ai_bIsValid ;
}


ClUserTimedAction::~ClUserTimedAction()
{
}

void	ClUserTimedAction::SetEventNum( long ai_iEventNum ) 
{
	m_iEventNum = ai_iEventNum ;
}

void	ClUserTimedAction::SetEventTime( m4date_t ai_dEventTime )
{
	m_dtEventTime = ai_dEventTime ;
}

void	ClUserTimedAction::SetOrganization( m4pcchar_t ai_pccOrganization )
{
	if( ai_pccOrganization == NULL )
	{
		ai_pccOrganization = "" ;
	}

	m_pcOrganization = ai_pccOrganization ;
}

void	ClUserTimedAction::SetEventSenderContainer( m4pcchar_t ai_pccEventSenderContainer )
{
	if( ai_pccEventSenderContainer == NULL )
	{
		ai_pccEventSenderContainer = "" ;
	}

	m_pcEventSenderContainer = ai_pccEventSenderContainer ;
}

void	ClUserTimedAction::SetEventSender( m4pcchar_t ai_pccEventSender )
{
	if( ai_pccEventSender == NULL )
	{
		ai_pccEventSender = "" ;
	}

	m_pcEventSender = ai_pccEventSender ;
}

void	ClUserTimedAction::SetParentEventSenderContainer( m4pcchar_t ai_pccParentEventSenderContainer )
{
	if( ai_pccParentEventSenderContainer == NULL )
	{
		ai_pccParentEventSenderContainer = "" ;
	}

	m_pcParentEventSenderContainer = ai_pccParentEventSenderContainer ;
}

void	ClUserTimedAction::SetParentEventSender( m4pcchar_t ai_pccParentEventSender )
{
	if( ai_pccParentEventSender == NULL )
	{
		ai_pccParentEventSender = "" ;
	}

	m_pcParentEventSender = ai_pccParentEventSender ;
}

void	ClUserTimedAction::SetTimeElapsed( long ai_iTimeElapsed )
{
	m_iTimeElapsed = ai_iTimeElapsed ;
}

void	ClUserTimedAction::SetBuffer( m4pcchar_t ai_pccBuffer )
{
	if( ai_pccBuffer == NULL )
	{
		ai_pccBuffer = "" ;
	}

	m_pcBuffer = ai_pccBuffer ;
}

void	ClUserTimedAction::SetIsValid( m4bool_t ai_bIsValid )
{
	m_bIsValid = ai_bIsValid ;
}


long	ClUserTimedAction::GetEventNum() const
{
	return m_iEventNum ;
}

m4date_t	ClUserTimedAction::GetEventTime( void ) const
{
	return m_dtEventTime ;
}

m4pcchar_t ClUserTimedAction::GetOrganization() const
{
	return m_pcOrganization.c_str() ;
}

m4pcchar_t	ClUserTimedAction::GetEventSenderContainer( void ) const
{
	return m_pcEventSenderContainer.c_str() ;
}

m4pcchar_t	ClUserTimedAction::GetEventSender( void ) const
{
	return m_pcEventSender.c_str() ;
}

m4pcchar_t	ClUserTimedAction::GetParentEventSenderContainer( void ) const
{
	return m_pcParentEventSenderContainer.c_str() ;
}

m4pcchar_t	ClUserTimedAction::GetParentEventSender( void ) const
{
	return m_pcParentEventSender.c_str() ;
}

long		ClUserTimedAction::GetTimeElapsed(  void ) const
{
	return m_iTimeElapsed;
}

m4pcchar_t	ClUserTimedAction::GetBuffer( void ) const
{
	return m_pcBuffer.c_str() ;
}

m4bool_t	ClUserTimedAction::GetIsValid( void ) const
{
	return m_bIsValid ;
}





// *****************************************************************************
// **
// **	Class ClUserActionsAudit
// **
// *****************************************************************************

// -----------------------------------------------------------------------------
// -- Public methods
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// -- ClUserActionsAudit constructor
// -----------------------------------------------------------------------------

ClUserActionsAudit::ClUserActionsAudit( ClChannelManager* ai_poChannelManager )
{
	m_bActive = M4_FALSE ;
	m_poChannelManager = ai_poChannelManager ;
	m_poChannel = NULL ;
	m_poAccess = NULL  ;
}


// -----------------------------------------------------------------------------
// -- ClUserActionsAudit destructor
// -----------------------------------------------------------------------------

ClUserActionsAudit::~ClUserActionsAudit( void )
{
	End() ;
}


// -----------------------------------------------------------------------------
// -- Limpia el objeto completamente.
// -----------------------------------------------------------------------------

void	ClUserActionsAudit::End( void )
{
	m_bActive = M4_FALSE ;
	
	if( m_poAccess != NULL )
	{
		m_poAccess->Destroy() ;
		m_poAccess = NULL ;
	}

	if( m_poChannel != NULL )
	{
		m_poChannel->Destroy() ;
		m_poChannel = NULL ;
	}

	Reset() ;
}


// -----------------------------------------------------------------------------
// -- Destruye los eventos almacenados.
// -----------------------------------------------------------------------------

void	ClUserActionsAudit::Reset( void )
{
	_ResetActions() ;
	_ResetTimedActions() ;
}


// -----------------------------------------------------------------------------
// -- Activa internamente la auditoria.
// -----------------------------------------------------------------------------

m4bool_t	ClUserActionsAudit::SwitchOn( void ) 
{
	m4bool_t	bResult = m_bActive ;

	if( m_bActive == M4_FALSE )
	{
		REGISTER_LISTENER( this ) ;
		m_bActive = M4_TRUE ;
	}

	return( bResult ) ;
}


// -----------------------------------------------------------------------------
// -- Desactiva internamente la auditoria y persiste si se pide.
// -----------------------------------------------------------------------------

m4bool_t	ClUserActionsAudit::SwitchOff( m4bool_t ai_bPersist )
{
	m4bool_t	bResult = m_bActive ;

	if( m_bActive == M4_TRUE )
	{
		if( ai_bPersist == M4_TRUE )
		{
			Persist( M4_FALSE ) ;
		}

		m_bActive = M4_FALSE ;
		REGISTER_LISTENER( NULL ) ;
	}

	return( bResult ) ;
}



// -----------------------------------------------------------------------------
// -- Comprueba si una sociedad tiene la auditoría activada para un tipo de evento.
// -----------------------------------------------------------------------------

m4bool_t	ClUserActionsAudit::HasAuditory( ClUserAction::eAuditEventID ai_eEventID, m4pcchar_t ai_pccOrganization )
{
	m4bool_t	bIsValid = M4_FALSE ;

	return( _HasAuditory( ai_eEventID, ai_pccOrganization, bIsValid ) ) ;
}



// -----------------------------------------------------------------------------
// -- Añade una acción a partir de sus valores.
// -----------------------------------------------------------------------------

void	ClUserActionsAudit::AddAction( m4pcchar_t ai_pccOrganization, m4pcchar_t ai_pccRole, ClUserAction::eAuditEventID ai_eEventID, m4pcchar_t ai_pccObject, m4pcchar_t ai_pccObjectName, m4pcchar_t ai_pccArgs, m4bool_t ai_bIsValid )
{

	ClUserAction*	poUserAction = NULL ;


	if( ai_bIsValid == M4_FALSE )
	{
		if( _HasAuditory( ai_eEventID, ai_pccOrganization, ai_bIsValid ) == M4_FALSE )
		{
			return ;
		}
	}

	poUserAction = new ClUserAction( ai_pccOrganization, ai_pccRole, ai_eEventID, ai_pccObject, ai_pccObjectName, ai_pccArgs, ai_bIsValid ) ;
	m_lActions.push_back( poUserAction ) ;
}



// -----------------------------------------------------------------------------
// -- Añade una acción.
// -----------------------------------------------------------------------------

void	ClUserActionsAudit::AddAction( ClUserAction* ai_poUserAction )
{
	_AddAction( ai_poUserAction, M4_FALSE ) ;
}


// -----------------------------------------------------------------------------
// -- Añade una acción cronometrada a partir de sus valores.
// -----------------------------------------------------------------------------

void	ClUserActionsAudit::AddTimedAction( long ai_iEventNum, m4date_t aio_dtEventTime, m4pcchar_t ai_pccOrganization, m4pcchar_t ai_pccEventSenderContainer, m4pcchar_t ai_pccEventSender, m4pcchar_t ai_pccParentEventSenderContainer, m4pcchar_t ai_pccParentEventSender, long ai_iTimeElapsed, m4pcchar_t ai_pccBuffer, m4bool_t ai_bIsValid )
{	 
	ClUserTimedAction*	poUserTimedAction = NULL ;

	if( ai_bIsValid == M4_FALSE )
	{
		if( _HasTimedAuditory( ai_iTimeElapsed, ai_pccOrganization, ai_bIsValid ) == M4_FALSE )
		{
			return ;
		}
	}

	poUserTimedAction = new ClUserTimedAction( ai_iEventNum, aio_dtEventTime, ai_pccOrganization, ai_pccEventSenderContainer, ai_pccEventSender, ai_pccParentEventSenderContainer, ai_pccParentEventSender, ai_iTimeElapsed, ai_pccBuffer, ai_bIsValid ) ;
	m_lTimedActions.push_back( poUserTimedAction ) ;
}


// -----------------------------------------------------------------------------
// -- Guarda los eventos en base datos si el estado es el adecuado.
// -----------------------------------------------------------------------------

m4return_t	ClUserActionsAudit::Persist( m4bool_t ai_bPersistAlways )
{

	m4return_t		iResult = M4_ERROR ;
	m4int32_t		iStatus = -1 ;
	m4date_t		dtLogon = 0 ;
	m4pcchar_t		pccOrganization = NULL ;
	m4VariantType	vResult ;
	ClLogonAdaptor	*poLogonAdaptor = NULL ;
	m4bool_t		bDoPersistActions = M4_TRUE ;
	m4bool_t		bDoPersistTimedActions = M4_TRUE;


	/* Bug 0182712
	Se tiene en cuenta el evento de logon
	*/
	poLogonAdaptor = m_poChannelManager->GetLogonAdaptor() ;

	if( poLogonAdaptor != NULL )
	{
		dtLogon = poLogonAdaptor->GetUserActionsAuditSessionDate() ;
	}

	// Si no hay datos y no hay evento de logon no hay nada que hacer
	if( m_lTimedActions.empty() == M4_TRUE && m_lActions.empty() == M4_TRUE && dtLogon == m4date_t( 0 ) )
	{
		return( M4_SUCCESS ) ;
	}

	// Se chequea que se pueda persistir
	if( ai_bPersistAlways == M4_FALSE )
	{
		if( poLogonAdaptor == NULL )
		{
			// En etapas tempranas no se persiste
			return( M4_SUCCESS ) ;
		}

		// Chequeo de persistencia de acciones de usuario
		iStatus = poLogonAdaptor->GetUserActionsAuditStatus() ;

		if( iStatus == -1 )
		{
			// En etapas tempranas no se persiste
			return( M4_SUCCESS ) ;
		}

		if( iStatus == 0 )
		{
			// Sin auditoría no se persiste y hay que borrar los eventos acumulados
			_ResetActions() ;
			bDoPersistActions = M4_FALSE ;
		}
		else
		{
			if( poLogonAdaptor->GetIsMultiOrganization() == 1 )
			{
				pccOrganization = poLogonAdaptor->GetOrganization() ;

				if( pccOrganization == NULL || *pccOrganization == '\0' )
				{
					// En etapas tempranas no se persiste
					return( M4_SUCCESS ) ;
				}
			}
		}
	}

	if( bDoPersistActions == M4_TRUE )
	{
		// Se hace la persistencia de acciones de usuario
		iResult = _PersistActions() ;
		CHECK_RETURN( iResult ) ;
	}

	// Se chequea que se pueda persistir (acciones cronometradas)
	if( ai_bPersistAlways == M4_FALSE )
	{
		// Chequeo de persistencia de acciones cronometradas
		iStatus = poLogonAdaptor->GetUserTimeRCThreshold() ;

		if( iStatus == 0 )
		{
			// Sin auditoría no se persiste y hay que borrar los eventos acumulados
			_ResetTimedActions() ;
			bDoPersistTimedActions = M4_FALSE ;
		}
		else
		{
			if (m_lTimedActions.empty() == M4_TRUE)
			{
				// No hay acciones en cola => No hay nada que persistir
				bDoPersistTimedActions = M4_FALSE;
			}
		}
	}

	if( bDoPersistTimedActions == M4_TRUE )
	{
		// Se hace persistencia de acciones cronometradas.
		if(( m_poChannelManager->GetDMContext()->ExecutingInClient() == M4_TRUE ) &&
		   ( m_poChannelManager->GetDMContext()->IsSM() == M4_FALSE ))
		{
			// Estoy en parte cliente de un RW. Si se ha hecho la persistencia de actiones
			// entonces las acciones cronometradas habrán ido en la transacción. En caso 
			// contrario estarán aún pendientes por lo que se genera una transacción
			// (Confirm del Session Channel) para se gestione de forma homogénea las pendientes
			// Se lanza el Confirm del Session Channel
			iResult = poLogonAdaptor->Confirm();
		}
		else
		{
			// SM o parte server de un RW. Se invoca directamente la persistencia
			iResult = _PersistTimedActions() ;
		}
		CHECK_RETURN( iResult ) ;
	}

	return( M4_SUCCESS ) ;
}



// -----------------------------------------------------------------------------
// -- Persistencia evento de error en logon de un usuario.
// --
// -- ai_pcUser:		In. Usuario que se conecta.
// -- ai_pcClient:		In. Puesto cliente.
// -----------------------------------------------------------------------------

m4return_t	ClUserActionsAudit::PersistLogonError( m4pchar_t ai_pcUser, m4pchar_t ai_pcClient, m4pchar_t ai_pcClientType )
{

	m4return_t		iResult = M4_ERROR ;
	ClNode			*poNode = NULL ;
	m4VariantType	avArguments[ 3 ] ;
	m4char_t		szNode[] = "SSC_USER_ACTIONS_AUDITORY_API";

	// Se obtiene el canal de auditoría
    iResult = _GetNode( szNode, poNode ) ;
	CHECK_RETURN( iResult ) ;
	CHECK_POINTER( poNode ) ;


	if( ai_pcUser != NULL )
	{
		avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
		avArguments[ 0 ].Data.PointerChar = ai_pcUser ;
	}
	else
	{
		avArguments[ 0 ].Type = M4CL_CPP_TYPE_NULL ;
	}

	if( ai_pcClient != NULL )
	{
		avArguments[ 1 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
		avArguments[ 1 ].Data.PointerChar = ai_pcClient ;
	}
	else
	{
		avArguments[ 1 ].Type = M4CL_CPP_TYPE_NULL ;
	}
	
	if( ai_pcClientType != NULL )
	{
		avArguments[ 2 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
		avArguments[ 2 ].Data.PointerChar = ai_pcClientType ;
	}
	else
	{
		avArguments[ 2 ].Type = M4CL_CPP_TYPE_NULL ;
	}

	iResult = _Execute( szNode, "PERSIST_LOGON_ERROR", avArguments, 3 ) ;
	CHECK_RETURN( iResult ) ;

	return( M4_SUCCESS ) ;
}



// -----------------------------------------------------------------------------
// -- Interfaz ILogListener.
// --
// -- ai_pccEventType:			In. Tipo de evento de log (_error_, _warning_, _info_).
// -- ai_iEventId:				In. Identificador del evento de log.
// -- ai_pccEventType:			In. Parámetros del evento de log.
// --
// -- Traduce el mensaje de log y lo añade a la traza.
// -----------------------------------------------------------------------------

m4return_t	ClUserActionsAudit::LogEvent( m4pcchar_t ai_pccEventType, m4uint32_t ai_iEventId, m4pcchar_t ai_pccEventMessage )
{

	m4return_t		iResult = M4_ERROR ;
	m4bool_t		bIsValid = M4_FALSE ;
	m4uint32_t		iSize = 0 ;
	m4language_t	iLanguage = 0;
	m4pcchar_t		pccOrganization = NULL  ;
	m4pchar_t		pcBuffer = NULL  ;
	m4pcchar_t		pccText = NULL  ;
	m4char_t		acBuffer[ 128 ]  ;
	ClLogonAdaptor	*poLogonAdaptor = NULL ;
	ClUserAction	*poAction = NULL ;


	poLogonAdaptor = m_poChannelManager->GetLogonAdaptor() ;
	CHECK_POINTER( poLogonAdaptor ) ;

	pccOrganization = poLogonAdaptor->GetOrganization() ;

	if( _HasAuditory( ClUserAction::EV_LOG, pccOrganization, bIsValid ) == M4_FALSE )
	{
		return( M4_SUCCESS ) ;
	}

	iResult = poLogonAdaptor->GetLanguage( iLanguage ) ;
	CHECK_RETURN( iResult ) ;

	sprintf( acBuffer, "%d", ai_iEventId ) ;

	// Se formatea el mensaje
	iResult = FormatErrorCode( ai_iEventId, (m4pchar_t) ai_pccEventMessage, 0, NULL, 0, &iSize, iLanguage ) ;

	pccText = ai_pccEventMessage ;

	if( iResult != M4_ERROR )
	{
		pcBuffer = new m4char_t[ iSize + 1 ] ;

		iResult = FormatErrorCode( ai_iEventId, (m4pchar_t) ai_pccEventMessage, 0, pcBuffer, iSize, &iSize, iLanguage ) ;

		if( iResult != M4_ERROR )
		{
			pccText = pcBuffer ;
		}
	}

	// Se vuelca el evento
	poAction = new ClUserAction() ;

	poAction->SetOrganization( pccOrganization ) ;
	poAction->SetRole( NULL ) ;
	poAction->SetIsValid( bIsValid ) ;
	poAction->SetEventID( ClUserAction::EV_LOG ) ;
	poAction->SetEventObject( acBuffer ) ;
	poAction->SetEventObjectName( NULL ) ;
	poAction->SetEventArgs( ai_pccEventType ) ;
	poAction->AppendEventArgs( "=" ) ;
	poAction->AppendEventArgs( pccText ) ;

	if( pcBuffer != NULL )
	{
		delete [] pcBuffer ;
	}

	m_lActions.push_back( poAction ) ;

	return( M4_SUCCESS ) ;
}



// -----------------------------------------------------------------------------
// -- Serializa el canal de auditoria de acciones de usuario.
// --
// -- IOD:	In. IO driver.
// -----------------------------------------------------------------------------

m4return_t	ClUserActionsAudit::Serialize( ClGenericIODriver& IOD )
{

	m4return_t	iResult = M4_ERROR ;
	m4uint32_t	iLength = 0 ;
	m4pcchar_t	pccPointer = NULL ;
	m4date_t	dLocalTimestamp;

	list<ClUserAction*>::iterator	it ;


	// En server se persisten las acciones antes de enviar
	if( m_poChannelManager->GetDMContext()->ExecutingInClient() == M4_FALSE )
	{
		// No se controla error, se continua
		iResult = Persist( M4_FALSE ) ;
	}


	// Se escriben las acciones
	iLength = m_lActions.size() ;
	iResult = IOD.Write( iLength ) ;
	CHECK_RETURN( iResult ) ;

	if( iLength > 0 )
	{
		it = m_lActions.begin() ;

		while( it != m_lActions.end() )
		{
			iResult = _WriteString( IOD, (*it)->GetOrganization() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = _WriteString( IOD, (*it)->GetRole() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = IOD.Write( static_cast<m4int16_t>((*it)->GetEventID()) ) ;
			CHECK_RETURN( iResult ) ;

			iResult = _WriteString( IOD, (*it)->GetEventObject() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = _WriteString( IOD, (*it)->GetEventObjectName() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = _WriteString( IOD, (*it)->GetEventArgs() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = IOD.Write( (*it)->GetDate() ) ;
			CHECK_RETURN( iResult ) ;

			it++ ;
		}

		// Se limpian los eventos
		_ResetActions() ;
	}

	
	list<ClUserTimedAction*>::iterator	itTimedAction ;

	// Se escriben las acciones cronometradas
	iLength = m_lTimedActions.size() ;
	iResult = IOD.Write( iLength ) ;
	CHECK_RETURN( iResult ) ;

	if( iLength > 0 )
	{
		// Se escribe la hora local de serialización en GMT
		dLocalTimestamp = ClActualDate();
		iResult = IOD.Write( dLocalTimestamp ) ;
		CHECK_RETURN( iResult ) ;

		itTimedAction = m_lTimedActions.begin() ;

		while( itTimedAction != m_lTimedActions.end() )
		{
			iResult = IOD.Write( (*itTimedAction)->GetEventNum() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = IOD.Write( (*itTimedAction)->GetEventTime() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = _WriteString( IOD, (*itTimedAction)->GetOrganization() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = _WriteString( IOD, (*itTimedAction)->GetEventSenderContainer() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = _WriteString( IOD, (*itTimedAction)->GetEventSender() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = _WriteString( IOD, (*itTimedAction)->GetParentEventSenderContainer() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = _WriteString( IOD, (*itTimedAction)->GetParentEventSender() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = IOD.Write( (*itTimedAction)->GetTimeElapsed() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = _WriteString( IOD, (*itTimedAction)->GetBuffer() ) ;
			CHECK_RETURN( iResult ) ;

			iResult = IOD.Write( (*itTimedAction)->GetIsValid() ) ;
			CHECK_RETURN( iResult ) ;

			itTimedAction++ ;
		}

		// Se limpian los eventos
		_ResetTimedActions() ;
	}

	return M4_SUCCESS ;
}



// -----------------------------------------------------------------------------
// -- Deserializa el canal de auditoria de acciones de usuario.
// --
// -- IOD:	In. IO driver.
// -----------------------------------------------------------------------------

m4return_t	ClUserActionsAudit::Deserialize( ClGenericIODriver& IOD )
{

	m4return_t		iResult = M4_ERROR ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iLength = 0 ;
	m4uint32_t		iSize = 1024 ;
	m4int16_t		iEventID = ClUserAction::EV_NOEV ;
	m4date_t		dDate = 0 ;
	m4pchar_t		pcPointer = NULL ;
	ClUserAction	*poAction = NULL ;
	long			iNumber;
	ClUserTimedAction	*poTimedAction = NULL ;
	m4date_t		dSourceTimeStamp;
	m4date_t		dDeltaTimeStamp;
	// Se obtiene la hora local en GMT
	m4date_t		dLocalTimeStamp = ClActualDate();

	// Se limpian los eventos
	_ResetActions() ;

	// Se leen las acciones
	iResult = IOD.Read( iLength ) ;
	CHECK_RETURN( iResult ) ;

	if( iLength > 0 )
	{
		pcPointer = new m4char_t[ iSize + 1 ] ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poAction = new ClUserAction() ;

			iResult = _ReadString( IOD, pcPointer, iSize ) ;
			CHECK_RETURN( iResult ) ;
			poAction->SetOrganization( pcPointer ) ;

			iResult = _ReadString( IOD, pcPointer, iSize ) ;
			CHECK_RETURN( iResult ) ;
			poAction->SetRole( pcPointer ) ;

			iResult = IOD.Read( iEventID ) ;
			CHECK_RETURN( iResult ) ;
			poAction->SetEventID( static_cast<ClUserAction::eAuditEventID>(iEventID) ) ;

			iResult = _ReadString( IOD, pcPointer, iSize ) ;
			CHECK_RETURN( iResult ) ;
			poAction->SetEventObject( pcPointer ) ;

			iResult = _ReadString( IOD, pcPointer, iSize ) ;
			CHECK_RETURN( iResult ) ;
			poAction->SetEventObjectName( pcPointer ) ;

			iResult = _ReadString( IOD, pcPointer, iSize ) ;
			CHECK_RETURN( iResult ) ;
			poAction->SetEventArgs( pcPointer ) ;

			iResult = IOD.Read( dDate ) ;
			CHECK_RETURN( iResult ) ;
			poAction->SetDate( dDate ) ;

			m_lActions.push_back( poAction ) ;
		}

		delete( pcPointer ) ;
	}


	// Se limpian los eventos
	_ResetTimedActions() ;

	// Se leen las acciones cronometradas.
	iResult = IOD.Read( iLength ) ;
	CHECK_RETURN( iResult ) ;

	if( iLength > 0 )
	{
		// Se lee la hora local de serialización en GMT
		iResult = IOD.Read( dSourceTimeStamp );
		CHECK_RETURN( iResult ) ;

		// Se obtiene la diferencia de tiempos entre la hora local y la serialización
		// para aplicarla sobre las fechas de las acciones
		dDeltaTimeStamp = dLocalTimeStamp - dSourceTimeStamp;

		pcPointer = new m4char_t[ iSize + 1 ] ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poTimedAction = new ClUserTimedAction() ;

			iResult = IOD.Read( iNumber ) ;
			CHECK_RETURN( iResult ) ;
			poTimedAction->SetEventNum( iNumber ) ;

			iResult = IOD.Read( dDate ) ;
			CHECK_RETURN( iResult ) ;
			// Se aplica el delta
			dDate += dDeltaTimeStamp;
			poTimedAction->SetEventTime( dDate ) ;

			iResult = _ReadString( IOD, pcPointer, iSize ) ;
			CHECK_RETURN( iResult ) ;
			poTimedAction->SetOrganization( pcPointer ) ;
		
			iResult = _ReadString( IOD, pcPointer, iSize ) ;
			CHECK_RETURN( iResult ) ;
			poTimedAction->SetEventSenderContainer( pcPointer ) ;

			iResult = _ReadString( IOD, pcPointer, iSize ) ;
			CHECK_RETURN( iResult ) ;
			poTimedAction->SetEventSender( pcPointer ) ;

			iResult = _ReadString( IOD, pcPointer, iSize ) ;
			CHECK_RETURN( iResult ) ;
			poTimedAction->SetParentEventSenderContainer( pcPointer ) ;

			iResult = _ReadString( IOD, pcPointer, iSize ) ;
			CHECK_RETURN( iResult ) ;
			poTimedAction->SetParentEventSender( pcPointer ) ;

			iResult = IOD.Read( iNumber ) ;
			CHECK_RETURN( iResult ) ;
			poTimedAction->SetTimeElapsed( iNumber ) ;

			iResult = _ReadString( IOD, pcPointer, iSize ) ;
			CHECK_RETURN( iResult ) ;
			poTimedAction->SetBuffer( pcPointer ) ;

			iResult = IOD.Read( iNumber ) ;
			CHECK_RETURN( iResult ) ;
			poTimedAction->SetIsValid( iNumber ) ;

			m_lTimedActions.push_back( poTimedAction ) ;
		}

		delete( pcPointer ) ;
	}

	return M4_SUCCESS ;
}



// -----------------------------------------------------------------------------
// -- Private methods
// -----------------------------------------------------------------------------

m4bool_t	ClUserActionsAudit::_HasAuditory( ClUserAction::eAuditEventID ai_eEventID, m4pcchar_t &aio_rpccOrganization, m4bool_t &ao_rbIsValid )
{

	m4return_t			iResult = M4_ERROR ;
	m4bool_t			bIsNotEof = M4_FALSE ;
	m4int32_t			iStatus = -1 ;
    m4int32_t			iRegister = -1 ;
	m4pchar_t			pcRole = NULL ;
	m4VariantType		vVariant ;
    m4VariantType		avArguments[ 3 ] ;
	ClNode				*poNode = NULL ;
	ClAccess			*poAccess = NULL ;
	ClAccessRecordSet	*poRecordSet = NULL ;
	ClLogonAdaptor		*poLogonAdaptor = NULL ;


	ao_rbIsValid = M4_FALSE ;

	if( m_bActive == M4_FALSE )
	{
		return( M4_FALSE ) ;
	}

	if( ai_eEventID == ClUserAction::EV_NOEV )
	{
		return( M4_FALSE ) ;
	}

	poLogonAdaptor = m_poChannelManager->GetLogonAdaptor() ;

	if( poLogonAdaptor == NULL )
	{
		// En etapas tempranas se audita
		return( M4_TRUE ) ;
	}

	iStatus = poLogonAdaptor->GetUserActionsAuditStatus() ;

	if( iStatus == -1 )
	{
		// En etapas tempranas se audita
		return( M4_TRUE ) ;
	}

	if( iStatus == 0 )
	{
		// Si no hay auditoría no se audita
		return( M4_FALSE ) ;
	}

	/* Bug 0174240
	Si el repositorio es monosocietario no se busca a nivel de sociedad y se basa en la marca general
	*/
	if( poLogonAdaptor->GetIsMultiOrganization() == 0 )
	{
		if( ( iStatus & (1 << ai_eEventID) ) == 0 )
		{
			return( M4_FALSE ) ;
		}

		return( M4_TRUE ) ;
	}


	// Se busca en el canal sesión la sociedad y se comprueba si audita o no
	if( poLogonAdaptor->GetSessionChannelPointer() == NULL )
	{
		// En etapas tempranas se audita
		return( M4_TRUE ) ;
	}

	poRecordSet = poLogonAdaptor->GetSessionRecordSet() ;

	if( poRecordSet == NULL )
	{
		return( M4_FALSE ) ;
	}

	poAccess = poRecordSet->GetpAccess() ;
	
	if( poAccess == NULL )
	{
		return( M4_FALSE ) ;
	}

	bIsNotEof = poRecordSet->Current.Begin() ;

	if( bIsNotEof == M4_FALSE )
	{
		return( M4_FALSE ) ;
	}

	poNode = &( poAccess->Node[ "APP_ROLE_SESSION" ] ) ;

	if( poNode == NULL )
	{
		return( M4_FALSE ) ;
	}

	pcRole = poLogonAdaptor->GetRole() ;

	if( pcRole == NULL )
	{
		// En etapas tempranas se audita
		return( M4_TRUE ) ;
	}

	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 1 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 2 ].Type = M4CL_CPP_TYPE_NUMBER ;
	avArguments[ 2 ].Data.DoubleData = M4CL_EQUAL_FUNCTION ;

	avArguments[ 0 ].Data.PointerChar = "ID_APP_ROLE" ;
	avArguments[ 1 ].Data.PointerChar = pcRole ;

	iResult = poNode->RecordSet.Register.FindSec( avArguments, 3, &iRegister ) ;

	if( iResult == M4_ERROR || iRegister == -1 )
	{
		return( M4_FALSE ) ;
	}

	bIsNotEof = poNode->RecordSet.Current.MoveTo( iRegister ) ;

	if( bIsNotEof == M4_FALSE )
	{
		return( M4_FALSE ) ;
	}

	poNode = &( poAccess->Node[ "APP_ROLE_ORG" ] ) ;

	if( poNode == NULL )
	{
		return( M4_FALSE ) ;
	}

	if( aio_rpccOrganization == NULL || *aio_rpccOrganization == '\0' )
	{
		aio_rpccOrganization = poLogonAdaptor->GetOrganization() ;

		if( aio_rpccOrganization == NULL || *aio_rpccOrganization == '\0' )
		{
			// En etapas tempranas se audita
			return( M4_TRUE ) ;
		}
	}

	avArguments[ 0 ].Data.PointerChar = "ID_ORGANIZATION_USED" ;
	avArguments[ 1 ].Data.PointerChar = (m4pchar_t) aio_rpccOrganization ;

	iResult = poNode->RecordSet.Register.FindSec( avArguments, 3, &iRegister ) ;

	if( iResult == M4_ERROR || iRegister == -1 )
	{
		return( M4_FALSE ) ;
	}

	iResult = poNode->RecordSet.Register[ iRegister ].Item[ "USR_AUDIT_ACTIVE" ].Value.Get( vVariant ) ;

	if( iResult == M4_ERROR || vVariant.Type != M4CL_CPP_TYPE_NUMBER || ( int(vVariant.Data.DoubleData) & (1 << ai_eEventID) ) == 0 )
	{

		return( M4_FALSE ) ;
	}

	ao_rbIsValid = M4_TRUE ;
	return( M4_TRUE ) ;
}



m4bool_t	ClUserActionsAudit::_HasTimedAuditory( long ai_iTimeElapsed, m4pcchar_t &aio_rpccOrganization, m4bool_t &ao_rbIsValid )
{

	m4return_t			iResult = M4_ERROR ;
	m4bool_t			bIsNotEof = M4_FALSE ;
	m4int32_t			iStatus = -1 ;
    m4int32_t			iRegister = -1 ;
	m4pchar_t			pcRole = NULL ;
	m4VariantType		vVariant ;
    m4VariantType		avArguments[ 3 ] ;
	ClNode				*poNode = NULL ;
	ClAccess			*poAccess = NULL ;
	ClAccessRecordSet	*poRecordSet = NULL ;
	ClLogonAdaptor		*poLogonAdaptor = NULL ;
	m4bool_t			bIsDevClient = M4_FALSE ;


	ao_rbIsValid = M4_FALSE ;

	
	poLogonAdaptor = m_poChannelManager->GetLogonAdaptor() ;

	if( poLogonAdaptor == NULL )
	{
		// En etapas tempranas se audita
		return( M4_TRUE ) ;
	}


	// Comprobar auditoria activa para cliente Windows a nivel general.
	iStatus = poLogonAdaptor->GetUserTimeRCThreshold() ;

	if( iStatus == -1 )
	{
		// En etapas tempranas se audita
		return( M4_TRUE ) ;
	}

	if( iStatus == 0 )
	{
		// Si no hay auditoría no se audita
		return( M4_FALSE ) ;
	}


	if( poLogonAdaptor->GetSessionChannelPointer() == NULL )
	{
		// En etapas tempranas se audita
		return( M4_TRUE ) ;
	}

	poRecordSet = poLogonAdaptor->GetSessionRecordSet() ;

	if( poRecordSet == NULL )
	{
		return( M4_FALSE ) ;
	}


	// Comprobar auditoria activa para cliente de desarrollo a nivel general.
	bIsDevClient = ( poLogonAdaptor->GetUserAuditClientType() == 0 ) ? M4_TRUE : M4_FALSE ;

	if( bIsDevClient == M4_TRUE )
	{
		iResult = poRecordSet->Register.Item[ "USR_TIME_AUDIT_DEV_CLIENT" ].Value.Get( vVariant ) ;

		if( iResult == M4_ERROR || vVariant.Type != M4CL_CPP_TYPE_NUMBER || int(vVariant.Data.DoubleData) == 0 )
		{
			return( M4_FALSE ) ;
		}
	}


	/* Bug 0174240
	Si el repositorio es monosocietario no se busca a nivel de sociedad y se basa en la marca general
	*/
	if( poLogonAdaptor->GetIsMultiOrganization() == 0 )
	{
		if( iStatus == 0 || iStatus >= ai_iTimeElapsed )
		{
			return( M4_FALSE ) ;
		}

		return( M4_TRUE ) ;
	}


	// Se busca en el canal sesión la sociedad y se comprueba si audita o no
	poAccess = poRecordSet->GetpAccess() ;
	
	if( poAccess == NULL )
	{
		return( M4_FALSE ) ;
	}

	bIsNotEof = poRecordSet->Current.Begin() ;

	if( bIsNotEof == M4_FALSE )
	{
		return( M4_FALSE ) ;
	}

	poNode = &( poAccess->Node[ "APP_ROLE_SESSION" ] ) ;

	if( poNode == NULL )
	{
		return( M4_FALSE ) ;
	}

	pcRole = poLogonAdaptor->GetRole() ;

	if( pcRole == NULL )
	{
		// En etapas tempranas se audita
		return( M4_TRUE ) ;
	}

	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 1 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 2 ].Type = M4CL_CPP_TYPE_NUMBER ;
	avArguments[ 2 ].Data.DoubleData = M4CL_EQUAL_FUNCTION ;

	avArguments[ 0 ].Data.PointerChar = "ID_APP_ROLE" ;
	avArguments[ 1 ].Data.PointerChar = pcRole ;

	iResult = poNode->RecordSet.Register.FindSec( avArguments, 3, &iRegister ) ;

	if( iResult == M4_ERROR || iRegister == -1 )
	{
		return( M4_FALSE ) ;
	}

	bIsNotEof = poNode->RecordSet.Current.MoveTo( iRegister ) ;

	if( bIsNotEof == M4_FALSE )
	{
		return( M4_FALSE ) ;
	}

	poNode = &( poAccess->Node[ "APP_ROLE_ORG" ] ) ;

	if( poNode == NULL )
	{
		return( M4_FALSE ) ;
	}

	if( aio_rpccOrganization == NULL || *aio_rpccOrganization == '\0' )
	{
		aio_rpccOrganization = poLogonAdaptor->GetOrganization() ;

		if( aio_rpccOrganization == NULL || *aio_rpccOrganization == '\0' )
		{
			// En etapas tempranas se audita
			return( M4_TRUE ) ;
		}
	}

	avArguments[ 0 ].Data.PointerChar = "ID_ORGANIZATION_USED" ;
	avArguments[ 1 ].Data.PointerChar = (m4pchar_t) aio_rpccOrganization ;

	iResult = poNode->RecordSet.Register.FindSec( avArguments, 3, &iRegister ) ;

	if( iResult == M4_ERROR || iRegister == -1 )
	{
		return( M4_FALSE ) ;
	}
	
	// Comprobar auditoria activa para cliente Windows a nivel de sociedad.
	iResult = poNode->RecordSet.Register[ iRegister ].Item[ "USR_TIME_AUDIT_RC_THRESHOLD" ].Value.Get( vVariant ) ;


	if( iResult == M4_ERROR || vVariant.Type != M4CL_CPP_TYPE_NUMBER || int(vVariant.Data.DoubleData) == 0 || int(vVariant.Data.DoubleData) >= ai_iTimeElapsed )
	{
		return( M4_FALSE ) ;
	}

	if( bIsDevClient == M4_TRUE )
	{
		// Comprobar auditoria activa para cliente de desarrollo a nivel de sociedad.
		iResult = poNode->RecordSet.Register[ iRegister ].Item[ "USR_TIME_AUDIT_DEV_CLIENT" ].Value.Get( vVariant ) ;

		if( iResult == M4_ERROR || vVariant.Type != M4CL_CPP_TYPE_NUMBER || int(vVariant.Data.DoubleData) == 0 )
		{
			return( M4_FALSE ) ;
		}
	}

	ao_rbIsValid = M4_TRUE ;
	return( M4_TRUE ) ;
}



m4return_t	ClUserActionsAudit::_GetNode( m4pcchar_t ai_pccNodeName, ClNode* &ao_rpoNode )
{

	m4return_t	iResult = M4_ERROR ;


	ao_rpoNode = NULL ;

	if( m_poAccess == NULL )
	{
		if( m_poChannel == NULL )
		{
			// Se construte el canal de auditoría
			iResult = m_poChannelManager->CreateChannel( m_poChannel ) ;
			CHECK_RETURN( iResult ) ;
			CHECK_POINTER( m_poChannel ) ;
			
			m_poChannel->Role_ID.Set( M4CL_SEC_SUPER_USER ) ;
			
			iResult = m_poChannel->BuildFromIdNoSec( "SSC_USER_ACTIONS_AUDITORY" ) ;
			CHECK_RETURN( iResult ) ;
			
			/* Bug 0176305
			Se deja el role a nulo para que se coja del canal sesión
			*/
			m_poChannel->Role_ID.SetFlags(M4_PROP_WRITE) ;
			m_poChannel->Role_ID.SetNull() ;
			m_poChannel->Role_ID.UnSetFlags(M4_PROP_WRITE) ;
			
			// Se saca del channel manager
			m_poChannelManager->Channel.Detach( m_poChannel );
		}
		
		// Se crea el acceso
		m_poAccess = m_poChannel->CreateAccess() ;
		CHECK_POINTER( m_poAccess ) ;
	}
	
	// Se apunta al nodo
	ao_rpoNode = &( m_poAccess->Node[ ai_pccNodeName ] ) ;
	CHECK_POINTER( ao_rpoNode ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClUserActionsAudit::_Execute( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength )
{

	m4return_t		iResult = M4_ERROR ;
	m4bool_t		bActive = M4_FALSE ;
	m4uint16_t		i = 0 ;
	ClNode			*poNode = NULL ;
	IStackInterface	*poStack = NULL ;
	m4VariantType	vVariant ;


	// Se obtiene el canal de auditoría
    iResult = _GetNode( ai_pccNode, poNode ) ;
	CHECK_RETURN( iResult ) ;
	CHECK_POINTER( poNode ) ;

	poStack = &( poNode->GetpAccess()->GetpExecutor()->Stack ) ;
	CHECK_POINTER( poStack ) ;

	// Se añaden los agumentos de entrada.
	if( ai_pvArguments != NULL && ai_iLength > 0 )
	{
		for( i = 0; i < ai_iLength; i++ )
		{
			iResult = poStack->Push( ai_pvArguments[ i ] ) ;
			CHECK_RETURN( iResult ) ;
		}
	}

	// Se desactiva la auditoría
	bActive = SwitchOff( M4_FALSE ) ;

	// Se ejecuta
	iResult = poNode->RecordSet.Item[ ai_pccItem ].Call( NULL, ai_iLength ) ;

	if( bActive == M4_TRUE )
	{
		// Se deja como estaba
		SwitchOn() ;
	}

	CHECK_RETURN( iResult ) ;

	iResult = poStack->Pop( vVariant ) ;

    if( iResult != M4_SUCCESS || vVariant.Type != M4CL_CPP_TYPE_NUMBER || vVariant.Data.DoubleData != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	return( M4_SUCCESS ) ;
}



m4return_t	ClUserActionsAudit::_PersistActions( void )
{

	m4return_t		iResult = M4_ERROR ;
	m4bool_t		bFound = M4_FALSE ;
	m4bool_t		bIsValid = M4_FALSE ;
	m4pcchar_t		pccOrganization = NULL ;
	ClNode			*poNode = NULL ;
	m4VariantType	vVariant ;
	m4char_t		szNode[] = "SSC_USER_ACTIONS_AUDITORY_API";

	list<ClUserAction*>::iterator	it ;
	
	// Añadimos el evento de logon, si procede.
	iResult = _AddLogonAction() ;

	if( m_lActions.size() == 0 )
	{
		return( M4_SUCCESS ) ;
	}

	// Se obtiene el canal de auditoría
    iResult = _GetNode( szNode, poNode ) ;
	CHECK_RETURN( iResult ) ;
	CHECK_POINTER( poNode ) ;

	// Se limpian los eventos en cualquier caso
	poNode->RecordSet.Register.DestroyAll() ;

	// Se añaden los registros
	it = m_lActions.begin() ;

	bFound = M4_FALSE ;

	while( it != m_lActions.end() )
	{
		pccOrganization = (*it)->GetOrganization() ;
		
		// Si no están validados se validan
		if( (*it)->GetIsValid() == M4_FALSE )
		{
			if( _HasAuditory( (*it)->GetEventID(), pccOrganization, bIsValid ) == M4_FALSE && bIsValid == M4_FALSE )
			{
				it++ ;
				continue ;
			}
		}

		bFound = M4_TRUE ;

		iResult = poNode->RecordSet.Register.Add() ;
		CHECK_RETURN( iResult ) ;

		vVariant.Type = M4CL_CPP_TYPE_STRING_VAR ;

		vVariant.Data.PointerChar = (m4pchar_t) pccOrganization ;
		if( vVariant.Data.PointerChar != '\0' )
		{
			iResult = poNode->RecordSet.Register.Item[ "ID_ORGANIZATION" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;
		}
		
		vVariant.Data.PointerChar = (m4pchar_t) (*it)->GetRole() ;
		if( vVariant.Data.PointerChar != '\0' )
		{
			iResult = poNode->RecordSet.Register.Item[ "APP_ROLE" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;
		}

		vVariant.Data.PointerChar = const_cast< m4pchar_t >( (*it)->GetEventName() ) ;
		if( vVariant.Data.PointerChar != '\0' )
		{
			iResult = poNode->RecordSet.Register.Item[ "ID_EVENT" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;
		}

		vVariant.Data.PointerChar = (m4pchar_t) (*it)->GetEventObject() ;
		if( vVariant.Data.PointerChar != '\0' )
		{
			iResult = poNode->RecordSet.Register.Item[ "ID_OBJECT" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;
		}

		vVariant.Data.PointerChar = (m4pchar_t) (*it)->GetEventObjectName() ;
		if( vVariant.Data.PointerChar != '\0' )
		{
			iResult = poNode->RecordSet.Register.Item[ "NM_OBJECT" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;
		}

		vVariant.Data.PointerChar = (m4pchar_t) (*it)->GetEventArgs() ;
		if( vVariant.Data.PointerChar != '\0' )
		{
			iResult = poNode->RecordSet.Register.Item[ "OBJECT_PARAMS" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;
		}

		vVariant.Type = M4CL_CPP_TYPE_DATE ;

		vVariant.Data.DoubleData = (*it)->GetDate() ;
		iResult = poNode->RecordSet.Register.Item[ "DT_EVENT" ].Value.Set( vVariant ) ;
		CHECK_RETURN( iResult ) ;

		it++ ;
	}

	// Se limpian los eventos en cualquier caso
	_ResetActions() ;

	// Se persiste si hay registros añadidos
	if( bFound == M4_TRUE )
	{
		iResult = _Execute( szNode, "PERSIST_TREE", NULL, 0 ) ;

		// Se limpian los eventos en cualquier caso
		poNode->RecordSet.Register.DestroyAll() ;
		CHECK_RETURN( iResult ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClUserActionsAudit::_PersistTimedActions( void )
{
	m4return_t			iResult = M4_ERROR ;
	m4bool_t			bFound = M4_FALSE ;
	m4bool_t			bIsValid = M4_FALSE ;
	m4pcchar_t			pccOrganization = NULL ;
	ClNode				*poNode = NULL ;
	m4VariantType		vVariant ;
	m4char_t			szNode[] = "SSC_USR_TIME_AUDITORY" ;
	ClLogonAdaptor		*poLogonAdaptor ;
	m4bool_t			bIsDevClient = M4_FALSE ;
	ClAccessRecordSet	*poRecordSet = NULL;
	ClAccess			*poAccess = NULL;
	ClNode				*poRootNode = NULL;
	

	list<ClUserTimedAction*>::iterator	it ;
	
	if( m_lTimedActions.size() == 0 )
	{
		return( M4_SUCCESS ) ;
	}

	// Recuperamos si estamos en el contexto de estación de desarrollo.
	poLogonAdaptor = m_poChannelManager->GetLogonAdaptor() ;

	if( poLogonAdaptor != NULL )
	{
		bIsDevClient = ( poLogonAdaptor->GetUserAuditClientType() == 0 ) ? M4_TRUE : M4_FALSE ;
	}


	// Se obtiene el canal de auditoría
    iResult = _GetNode( szNode, poNode ) ;
	CHECK_RETURN( iResult ) ;
	CHECK_POINTER( poNode ) ;

	// Se limpian los eventos en cualquier caso
	poNode->RecordSet.Register.DestroyAll() ;

	// Se añaden los registros
	it = m_lTimedActions.begin() ;

	bFound = M4_FALSE ;

	while( it != m_lTimedActions.end() )
	{
		pccOrganization = (*it)->GetOrganization() ;

		// Si no están validados se validan
		if( (*it)->GetIsValid() == M4_FALSE )
		{
			if( _HasTimedAuditory( (*it)->GetTimeElapsed(), pccOrganization, bIsValid ) == M4_FALSE && bIsValid == M4_FALSE )
			{
				it++ ;
				continue ;
			}
		}

		bFound = M4_TRUE ;

		iResult = poNode->RecordSet.Register.Add() ;
		CHECK_RETURN( iResult ) ;

		vVariant.Type = M4CL_CPP_TYPE_STRING_VAR ;

		vVariant.Data.PointerChar = (m4pchar_t) pccOrganization ;
		if( vVariant.Data.PointerChar != '\0' )
		{
			iResult = poNode->RecordSet.Register.Item[ "PLCO_ID_ORGANIZATION" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;
		}
		
		vVariant.Data.PointerChar = (m4pchar_t) (*it)->GetEventSenderContainer() ;
		if( vVariant.Data.PointerChar != '\0' )
		{
			iResult = poNode->RecordSet.Register.Item[ "EV_SENDER_CONTNR" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;
		}

		vVariant.Data.PointerChar = (m4pchar_t) (*it)->GetEventSender() ;
		if( vVariant.Data.PointerChar != '\0' )
		{
			iResult = poNode->RecordSet.Register.Item[ "EV_SENDER" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;
		}

		vVariant.Data.PointerChar = (m4pchar_t) (*it)->GetParentEventSenderContainer() ;
		if( vVariant.Data.PointerChar != '\0' )
		{
			iResult = poNode->RecordSet.Register.Item[ "P_EV_SENDER_CONTNR" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;
		}

		vVariant.Data.PointerChar = (m4pchar_t) (*it)->GetParentEventSender() ;
		if( vVariant.Data.PointerChar != '\0' )
		{
			iResult = poNode->RecordSet.Register.Item[ "P_EV_SENDER" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;
		}

		vVariant.Data.PointerChar = (m4pchar_t) (*it)->GetBuffer() ;
		if( vVariant.Data.PointerChar != '\0' )
		{
			iResult = poNode->RecordSet.Register.Item[ "INFO" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;
		}

		// Rellena con valores por defecto los campos nulos del entorno de desarrollo.
		if( bIsDevClient == M4_TRUE )
		{
			vVariant.Data.PointerChar = "" ;

			poRecordSet = poLogonAdaptor->GetSessionRecordSet();
			
			if( poRecordSet != NULL )
			{
				poAccess = poRecordSet->GetpAccess();
				
				if( poAccess != NULL )
				{
					if( poRecordSet->Current.Begin() == M4_TRUE )
					{
						poRootNode = &(poAccess->Node[SESSION_CHANNEL_NODE_ID]);

						if( poRootNode != NULL )
						{
							vVariant.Type = M4CL_CPP_TYPE_NULL ;
							iResult = poRootNode->RecordSet.Register.Item["USR_AUDIT_CLIENT_MACHINE"].Value.Get(vVariant);
							
							if (iResult != M4_SUCCESS || vVariant.Type != M4CL_CPP_TYPE_STRING_VAR || vVariant.Data.PointerChar == NULL)
							{
								vVariant.Type = M4CL_CPP_TYPE_STRING_VAR ;
								vVariant.Data.PointerChar = "" ;
							}
						}
					}
				}
			}

			
			iResult = poNode->RecordSet.Register.Item[ "SRV_NAME" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;

			vVariant.Data.PointerChar = "0" ;
			iResult = poNode->RecordSet.Register.Item[ "SRV_PORT" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;

			vVariant.Data.PointerChar = "0" ;
			iResult = poNode->RecordSet.Register.Item[ "SRV_ID_SESSION" ].Value.Set( vVariant ) ;
			CHECK_RETURN( iResult ) ;
		}

		vVariant.Type = M4CL_CPP_TYPE_DATE ;

		vVariant.Data.DoubleData = (*it)->GetEventTime() ;
		iResult = poNode->RecordSet.Register.Item[ "EVENT_DATE" ].Value.Set( vVariant ) ;
		CHECK_RETURN( iResult ) ;

		vVariant.Type = M4CL_CPP_TYPE_NUMBER ;

		vVariant.Data.DoubleData = (*it)->GetEventNum() ;
		iResult = poNode->RecordSet.Register.Item[ "EVENT_NUM" ].Value.Set( vVariant ) ;
		CHECK_RETURN( iResult ) ;

		vVariant.Data.DoubleData = (*it)->GetTimeElapsed() ;
		iResult = poNode->RecordSet.Register.Item[ "TIME_ELAPSED" ].Value.Set( vVariant ) ;
		CHECK_RETURN( iResult ) ;

		it++ ;
	}

	// Se limpian los eventos en cualquier caso
	_ResetTimedActions() ;

	// Se persiste si hay registros añadidos
	if( bFound == M4_TRUE )
	{
		iResult = _Execute( szNode, "PERSIST_TREE", NULL, 0 ) ;

		// Se limpian los eventos en cualquier caso
		poNode->RecordSet.Register.DestroyAll() ;
		CHECK_RETURN( iResult ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClUserActionsAudit::_PersistSystemInfo(m4pcchar_t ai_pccOrganization)
{
	m4return_t		iResult = M4_ERROR;
	m4VariantType	vVariant;
	vector<string>	vSystemInfoList;
	m4uint32_t		iSize = 0;
	m4char_t		szNode[] = "SSC_SYSTEM_INFO";
	ClLogonAdaptor	*poLogonAdaptor = NULL;
	m4pchar_t		pcUserId = NULL;
	m4char_t		acEncryptedUser[2 * M4_MAX_LEN_ONE_WAY + 1];
	m4int_t			iLength = -1;
	m4VariantType	avArguments[ 12 ] ;

	// Obtener hash(APP_USER).
	poLogonAdaptor = m_poChannelManager->GetLogonAdaptor() ;
	CHECK_POINTER( poLogonAdaptor ) ;

	pcUserId = poLogonAdaptor->GetUsername() ;
	
	iResult = EncryptOneWaySHA(pcUserId, strlen( pcUserId ) * sizeof(m4char_t), acEncryptedUser, 2 * M4_MAX_LEN_ONE_WAY, iLength);
	if (iResult != M4_SUCCESS)
	{
		return M4_ERROR;
	}
		
	// Obtener la información del sistema del item SYSTEM_INFO del canal sesión.
	// v1.0#user_agent#idioma_usuario#sistema_operativo#Framework .NET#procesador#memoria#disco_utilizado#disco_libre#resolución_pantalla#profundidad_color#
	iResult = _GetSystemInfo(vSystemInfoList);
	CHECK_RETURN(iResult);

	// Verificar que la lista con la informacíón del sistema no está vacía.
	iSize = vSystemInfoList.size();
	if (iSize == 0 || iSize < M4_SYSTEM_INFO_FIELDS_NUM)
	{
		return M4_ERROR;
	}
	
	// Encypted app user.
	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 0 ].Data.PointerChar = (m4pchar_t) acEncryptedUser ;
	
	// Establecer ID_ORGANIZATION.
	avArguments[ 1 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 1 ].Data.PointerChar = (m4pchar_t) ai_pccOrganization ;
	
	// Establecer cada campo de información del sistema cliente y guardarlo en el item correspondiente.
	// 0: Versión de la información serializada. No se persiste.
	// 1: User agent: relevante sólo para cliente ligero.
	// 2: Idioma de usuario.
	// 3: Sistema operativo.
	// 4: Framework .NET.
	// 5: Procesador.
	// 6: Memoria.
	// 7: Disco utilizado.
	// 8: Disco libre.
	// 9: Resolución de pantalla.
	// 10: Profundidad de color.

	// Establecer USER_AGENT.
	avArguments[ 2 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 2 ].Data.PointerChar = (m4pchar_t) vSystemInfoList[1].c_str() ;
	
	// Establecer LANG.
	avArguments[ 3 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 3 ].Data.PointerChar = (m4pchar_t) vSystemInfoList[2].c_str() ;
	
	// Establecer OPERATING_SYSTEM.
	avArguments[ 4 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 4 ].Data.PointerChar = (m4pchar_t) vSystemInfoList[3].c_str() ;
	
	// Establecer FRAMEWORK_NET.
	avArguments[ 5 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 5 ].Data.PointerChar = (m4pchar_t) vSystemInfoList[4].c_str() ;
	
	// Establecer PROCESSOR_TYPE.
	avArguments[ 6 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 6 ].Data.PointerChar = (m4pchar_t) vSystemInfoList[5].c_str() ;

	// Establecer MEMORY.
	if (vSystemInfoList[6].empty() == M4_TRUE)
	{
		vVariant.Type = M4CL_CPP_TYPE_NULL;
	}
	else
	{
		avArguments[ 7 ].Type = M4CL_CPP_TYPE_NUMBER ;
		avArguments[ 7 ].Data.DoubleData = atoi(vSystemInfoList[6].c_str()) ;
	}

	// Establecer USED_DISK_SPACE.
	if (vSystemInfoList[7].empty() == M4_TRUE)
	{
		vVariant.Type = M4CL_CPP_TYPE_NULL;
	}
	else
	{
		avArguments[ 8 ].Type = M4CL_CPP_TYPE_NUMBER ;
		avArguments[ 8 ].Data.DoubleData = atoi(vSystemInfoList[7].c_str()) ;
	}

	// Establecer FREE_DISK_SPACE.
	if (vSystemInfoList[8].empty() == M4_TRUE)
	{
		vVariant.Type = M4CL_CPP_TYPE_NULL;
	}
	else
	{
		avArguments[ 9 ].Type = M4CL_CPP_TYPE_NUMBER ;
		avArguments[ 9 ].Data.DoubleData = atoi(vSystemInfoList[8].c_str()) ;
	}

	// Establecer SCREEN_RESOLUTION.
	avArguments[ 10 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 10 ].Data.PointerChar = (m4pchar_t) vSystemInfoList[9].c_str() ;

	// Establecer COLOR_DEPTH.
	if (vSystemInfoList[10].empty() == M4_TRUE)
	{
		vVariant.Type = M4CL_CPP_TYPE_NULL;
	}
	else
	{
		avArguments[ 11 ].Type = M4CL_CPP_TYPE_NUMBER ;
		avArguments[ 11 ].Data.DoubleData = atoi(vSystemInfoList[10].c_str());
	}
	
	iResult = _Execute( szNode, "ADD_SYSTEM_INFO", avArguments, 12);
	CHECK_RETURN(iResult);
	
	return M4_SUCCESS;
}

m4return_t  ClUserActionsAudit::_GetSystemInfo(vector<string> & ao_vSystemInfo)
{
	ClLogonAdaptor		*poLogonAdaptor = NULL;
	ClAccessRecordSet	*poRecordSet = NULL;
	ClAccess			*poAccess = NULL;
	ClNode				*poNode = NULL;
	m4VariantType		vVariant;
	m4VariantType		vSystemInfo;
	m4return_t			iRet = M4_ERROR;
	string				sSeparator = "#";

	poLogonAdaptor = m_poChannelManager->GetLogonAdaptor();
	CHECK_POINTER(poLogonAdaptor);

	poRecordSet = poLogonAdaptor->GetSessionRecordSet();
	CHECK_POINTER(poRecordSet);

	poAccess = poRecordSet->GetpAccess();
	CHECK_POINTER(poAccess);
	
	if (poRecordSet->Current.Begin() == M4_FALSE)
	{
		return M4_ERROR;
	}
	
	poNode = &(poAccess->Node[SESSION_CHANNEL_NODE_ID]);
	if (poNode == NULL)
	{
		return M4_ERROR;
	}
	
	vSystemInfo.Type = M4CL_CPP_TYPE_NULL ;
	iRet = poNode->RecordSet.Register.Item["SYSTEM_INFO"].Value.Get(vSystemInfo);
	
	if (iRet != M4_SUCCESS || vSystemInfo.Type != M4CL_CPP_TYPE_STRING_VAR || vSystemInfo.Data.PointerChar == NULL)
	{
		return M4_ERROR;
	}

	_Tokenize(vSystemInfo.Data.PointerChar, sSeparator, ao_vSystemInfo);

	return M4_SUCCESS;
}

m4return_t ClUserActionsAudit::_Tokenize(const string & ai_sString, const string & ai_sDelimiter, vector<string> & ao_vStrings)
{
	char	* pToken = NULL;
	string	sString = "";
	string	sSubStringInit = "";
	string	sSubStringEnd = "";
	int		iPos = 0;
	int		iLen = 0;

	// Verificar que el string no está vacio.
	if (ai_sString.length() == 0)
	{
		return M4_SUCCESS;
	}

	// Inicializaciones.
	sString = ai_sString;
	iLen = sString.length();

	// Buscar el caracter delimitador en el string.
	iPos = sString.find(ai_sDelimiter);
	while (iPos != -1) 
	{
		sSubStringInit = sString.substr(0, iPos);
		sSubStringEnd = sString.substr(iPos + 1, iLen - iPos);
		
		ao_vStrings.push_back(sSubStringInit);
		
		sString.assign(sSubStringEnd);
		iPos = sString.find(ai_sDelimiter);
	}

	// Añadir lo que queda en caso de que no esté vacio.
	if (sString.length() != 0)
	{
		sSubStringInit = sString;
		ao_vStrings.push_back(sSubStringInit);
	}
	
	return M4_SUCCESS;
}

m4return_t	ClUserActionsAudit::_WriteString( ClGenericIODriver& IOD, m4pcchar_t ai_pccPointer )
{

	m4return_t	iResult = M4_ERROR ;


	if( ai_pccPointer != NULL && *ai_pccPointer != '\0' )
	{
		iResult = IOD.Write( m4uint32_t( strlen( ai_pccPointer ) ) ) ;
		CHECK_RETURN( iResult ) ;

		iResult = IOD.Write( ai_pccPointer ) ;
		CHECK_RETURN( iResult ) ;
	}
	else
	{
		iResult = IOD.Write( m4uint32_t( 0 ) ) ;
		CHECK_RETURN( iResult ) ;
	}

	return( M4_SUCCESS ) ;
}



m4return_t	ClUserActionsAudit::_ReadString( ClGenericIODriver& IOD, m4pchar_t &ao_rpcPointer, m4uint32_t &ao_rSize )
{

	m4return_t	iResult = M4_ERROR ;
	m4uint32_t	iLength = 0 ;


	iResult = IOD.Read( iLength ) ;
	CHECK_RETURN( iResult ) ;

	if( iLength > 0 )
	{
		if( iLength > ao_rSize )
		{
			if( ao_rpcPointer != NULL )
			{
				delete( ao_rpcPointer ) ;
			}

			ao_rSize = iLength ;
			ao_rpcPointer = new m4char_t[ ao_rSize + 1 ] ;
		}

		iResult = IOD.Read( ao_rpcPointer, ao_rSize ) ;
		CHECK_RETURN( iResult ) ;
	}
	else
	{
		*ao_rpcPointer = '\0' ;
	}

	return( M4_SUCCESS ) ;
}


m4bool_t	ClUserActionsAudit::_AddAction( ClUserAction* ai_poUserAction, m4bool_t ai_bFirst )
{

	m4bool_t	bIsValid = M4_FALSE ;
	m4pcchar_t	pccOrganization = NULL ;

	if( ai_poUserAction->GetIsValid() == M4_FALSE )
	{
		pccOrganization = ai_poUserAction->GetOrganization() ;

		if( _HasAuditory( ai_poUserAction->GetEventID(), pccOrganization, bIsValid ) == M4_FALSE )
		{
			delete( ai_poUserAction ) ;
			return M4_FALSE ;
		}

		if( pccOrganization != NULL && *pccOrganization != '\0' )
		{
			ai_poUserAction->SetOrganization( pccOrganization ) ;
		}
		ai_poUserAction->SetIsValid( bIsValid ) ;
	}

	if( ai_bFirst == M4_TRUE )
	{
		m_lActions.push_front( ai_poUserAction ) ;
	}
	else
	{
		m_lActions.push_back( ai_poUserAction ) ;
	}

	return M4_TRUE ;
}


m4return_t	ClUserActionsAudit::_AddLogonAction()
{
	m4return_t			iRet = M4_ERROR ;
	m4bool_t			bRet = M4_FALSE ;
	ClLogonAdaptor		*poLogonAdaptor = NULL ;
	m4date_t			dtLogon ;
	m4pchar_t			pcUserId = NULL ;
	ClUserAction		*poUserAction ;	
	ClAccessRecordSet	*poRecordSet = NULL ;
	ClAccess			*poAccess = NULL ;
	ClNode				*poNode = NULL ;
	m4VariantType		vVariant ;
	m4VariantType		vImpersonator ;
	m4char_t			acEvParams[ 512 ] ;
	m4pcchar_t			pccOrganization = NULL ;

	// bg 302284. 
	// El evento de logon se añade en server. 
	// En cliente no se debe comprobar la marca del canal porque siempre es != 0.
	if(( m_poChannelManager->GetDMContext()->ExecutingInClient() == M4_TRUE ) &&
	   ( m_poChannelManager->GetDMContext()->IsSM() == M4_FALSE ))
	{
		return M4_SUCCESS;
	}

	poLogonAdaptor = m_poChannelManager->GetLogonAdaptor() ;
	CHECK_POINTER( poLogonAdaptor ) ;

	dtLogon = poLogonAdaptor->GetUserActionsAuditSessionDate() ;


	// No es nula, por lo que se trata de la primera persistencia.
	if( dtLogon != m4date_t( 0 ) )
	{
		pcUserId = poLogonAdaptor->GetUsername() ;

		poRecordSet = poLogonAdaptor->GetSessionRecordSet() ;
		CHECK_POINTER( poRecordSet ) ;

		poAccess = poRecordSet->GetpAccess() ;
		CHECK_POINTER( poAccess ) ;

		if( poRecordSet->Current.Begin() == M4_FALSE )
		{
			return( M4_ERROR ) ;
		}

		poNode = &( poAccess->Node[ SESSION_CHANNEL_NODE_ID ] ) ;
		if( poNode == NULL )
		{
			return( M4_ERROR ) ;
		}

		vImpersonator.Type = M4CL_CPP_TYPE_NULL ;
		iRet = poNode->RecordSet.Register.Item[ "ID_DEBUG_USER" ].Value.Get( vImpersonator ) ;

		if( iRet == M4_SUCCESS && vImpersonator.Type == M4CL_CPP_TYPE_STRING_VAR && vImpersonator.Data.PointerChar != NULL && *vImpersonator.Data.PointerChar != '\0' )
		{
			vVariant.Type = M4CL_CPP_TYPE_NULL ;
			iRet = poNode->RecordSet.Register.Item[ "APP_USER_ALIAS" ].Value.Get( vVariant ) ;

			if( iRet == M4_SUCCESS && vVariant.Type == M4CL_CPP_TYPE_STRING_VAR && vVariant.Data.PointerChar != NULL && *vVariant.Data.PointerChar != '\0' )
			{
				sprintf( acEvParams, "Impersonator user = %s; Authentication user alias = %s", vImpersonator.Data.PointerChar, vVariant.Data.PointerChar ) ;
				poUserAction = new ClUserAction( NULL, NULL, ClUserAction::EV_I_LOGON, pcUserId, pcUserId, acEvParams, M4_FALSE ) ;
			}
			else
			{
				sprintf( acEvParams, "Impersonator user = %s", vImpersonator.Data.PointerChar ) ;
				poUserAction = new ClUserAction( NULL, NULL, ClUserAction::EV_I_LOGON, pcUserId, pcUserId, acEvParams, M4_FALSE ) ;
			}
		}
		else
		{
			// bg 171255
			vVariant.Type = M4CL_CPP_TYPE_NULL ;
			iRet = poNode->RecordSet.Register.Item[ "APP_USER_ALIAS" ].Value.Get( vVariant ) ;

			if( iRet == M4_SUCCESS && vVariant.Type == M4CL_CPP_TYPE_STRING_VAR && vVariant.Data.PointerChar != NULL && *vVariant.Data.PointerChar != '\0' )
			{
				sprintf( acEvParams, "Authentication user alias = %s", vVariant.Data.PointerChar ) ;
				poUserAction = new ClUserAction( NULL, NULL, ClUserAction::EV_LOGON, pcUserId, pcUserId, acEvParams, M4_FALSE ) ;
			}
			else
			{
				poUserAction = new ClUserAction( NULL, NULL, ClUserAction::EV_LOGON, pcUserId, pcUserId, NULL, M4_FALSE ) ;
			}
		}
		
		// Añadimos la acción de logon.
		poUserAction->SetDate( dtLogon ) ;
		bRet = _AddAction( poUserAction, M4_TRUE ) ;

		// Ponemos la fecha de logon a null para no añadir el evento más veces.
		vVariant.Type = M4CL_CPP_TYPE_DATE ;
		vVariant.Data.DoubleData = m4date_t( 0 ) ;
		iRet = poNode->RecordSet.Register.Item[ "USR_AUDIT_SESSION_DATE" ].Value.Set( vVariant ) ;
		if ( iRet != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		// Se graba la información del sistema.
		if (bRet == M4_TRUE)
		{
			pccOrganization = poUserAction->GetOrganization() ;
			iRet = _PersistSystemInfo( pccOrganization ) ;
		}
	}

	return iRet ;
}


void	ClUserActionsAudit::_ResetActions( void )
{
	list<ClUserAction*>::iterator	it ;

	// Si no hay datos no hay nada que hacer
	if( m_lActions.empty() == M4_FALSE )
	{
		it = m_lActions.begin() ;

		while( it != m_lActions.end() )
		{
			delete( *it ) ;
			it++ ;
		}

		m_lActions.clear() ;
	}
}


void	ClUserActionsAudit::_ResetTimedActions( void )
{
	list<ClUserTimedAction*>::iterator	itTimedActions ;

	// Eliminamos las acciones de tiempos
	if( m_lTimedActions.empty() == M4_FALSE )
	{
		itTimedActions = m_lTimedActions.begin() ;

		while( itTimedActions != m_lTimedActions.end() )
		{
			delete( *itTimedActions ) ;
			itTimedActions++ ;
		}

		m_lTimedActions.clear() ;
	}
}


