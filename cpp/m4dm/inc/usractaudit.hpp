//==============================================================================
//
// (c) Copyright  1991-2009 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4dm.dll
// File:             usractaudit.hpp
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
//    Definición de las clases de auditoria de acciones de usuario.
//
//==============================================================================

#ifndef _USRACTAUDIT_H
#define _USRACTAUDIT_H


#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
#include "m4string.hpp"
#include "m4variant.hpp"
#include "m4log.hpp"


class ClChannelManager ;
class ClChannel ;
class ClAccess ;
class ClNode ;


// ******************************************************************************
// **
// **	Class ClUserAction.
// **
// **	Evento o acción individual de usuario.
// **
// ******************************************************************************

class	M4_DECL_M4DM	ClUserAction
{
public:

	typedef enum { EV_NOEV = -1, EV_LOGON, EV_LOGOFF, EV_I_LOGON, EV_LOGON_FAILED_USERNAME, EV_LOGON_FAILED_PASSWORD, EV_M4OBJECT, EV_METHOD, EV_TASK, EV_PRESENTATION, EV_WEB_PAGE, EV_JOB, EV_LOG, EV_WEB_MENU } eAuditEventID ;

	ClUserAction() ;
	ClUserAction( m4pcchar_t ai_pccOrganization, m4pcchar_t ai_pccRole, eAuditEventID ai_eEventID, m4pcchar_t ai_pccObject, m4pcchar_t ai_pccObjectName, m4pcchar_t ai_pccArgs, m4bool_t ai_bIsValid ) ;
	~ClUserAction() ;

	void	SetOrganization( m4pcchar_t ai_pccOrganization ) ;
	void	SetRole( m4pcchar_t ai_pccRole ) ;
	void	SetEventID( eAuditEventID ai_eEventID ) ;
	void	SetEventObject( m4pcchar_t ai_pccObject ) ;
	void	SetEventObjectName( m4pcchar_t ai_pccObjectName ) ;
	void	AppendEventObject( m4pcchar_t ai_pccObject ) ;
	void	AppendEventObjectName( m4pcchar_t ai_pccObjectName ) ;
	void	SetEventArgs( m4pcchar_t ai_pccArgs ) ;
	void	AppendEventArgs( m4pcchar_t	ai_pccArgs ) ;
	void	SetDate( m4date_t ai_dDate ) ;
	void	SetIsValid( m4bool_t ai_bIsValid ) ;

	m4pcchar_t			GetOrganization( void ) const ;
	m4pcchar_t			GetRole( void ) const ;
	eAuditEventID		GetEventID( void ) const ;
	m4pcchar_t			GetEventObject( void ) const ;
	m4pcchar_t			GetEventObjectName( void ) const ;
	m4pcchar_t			GetEventArgs( void ) const ;
	m4date_t			GetDate( void ) const ;
	m4bool_t			GetIsValid( void ) const ;
	m4pcchar_t			GetEventName( void ) const ;

protected:

	string			m_pcOrganization ;
	string			m_pcRole ;
	eAuditEventID	m_eEventID ;
	string			m_pcEventObject ;
	string			m_pcEventObjectName ;
	string			m_pcEventArgs ;
	m4date_t		m_dDate ;
	m4bool_t		m_bIsValid ;

	static m4pcchar_t	s_apccAuditEventNames[] ;
} ;


// ******************************************************************************
// **
// **	Class ClUserTimedAction.
// **
// **	Acción de usuario cronometrada.
// **
// ******************************************************************************

class	M4_DECL_M4DM	ClUserTimedAction
{
public:

	ClUserTimedAction() ;
	ClUserTimedAction( long ai_iEventNum, m4date_t ai_dEventTime, m4pcchar_t ai_pccOrganization, m4pcchar_t ai_pccEventSenderContainer, m4pcchar_t ai_pccEventSender, m4pcchar_t ai_pccParentEventSenderContainer, m4pcchar_t ai_pccParentEventSender, long ai_iTimeElapsed, m4pcchar_t ai_pccBuffer, m4bool_t ai_bIsValid );
	~ClUserTimedAction() ;

	void	SetEventNum( long ai_iEventNum ) ;
	void	SetEventTime( m4date_t ai_dEventTime ) ;
	void	SetOrganization( m4pcchar_t ai_pccOrganization ) ;
	void	SetEventSenderContainer( m4pcchar_t ai_pccEventSenderContainer ) ;
	void	SetEventSender( m4pcchar_t ai_pccEventSender ) ;
	void	SetParentEventSenderContainer( m4pcchar_t ai_pccParentEventSenderContainer ) ;
	void	SetParentEventSender( m4pcchar_t ai_pccParentEventSender ) ;
	void	SetTimeElapsed( long ai_iTimeElapsed ) ;
	void	SetBuffer( m4pcchar_t ai_pccBuffer ) ;
	void	SetIsValid( m4bool_t ai_bIsValid ) ;

	long				GetEventNum( void ) const ;
	m4date_t			GetEventTime( void ) const ;
	m4pcchar_t			GetOrganization( void ) const ;
	m4pcchar_t			GetEventSenderContainer( void ) const ;
	m4pcchar_t			GetEventSender( void ) const ;
	m4pcchar_t			GetParentEventSenderContainer( void ) const ;
	m4pcchar_t			GetParentEventSender( void ) const ;
	long				GetTimeElapsed( void ) const ;
	m4pcchar_t			GetBuffer( void ) const ;
	m4bool_t			GetIsValid( void ) const ;

protected:

	long			m_iEventNum ;
	m4date_t		m_dtEventTime ;
	string			m_pcOrganization ;
	string			m_pcEventSenderContainer ;
	string			m_pcEventSender ;
	string			m_pcParentEventSenderContainer ;
	string			m_pcParentEventSender ;
	long			m_iTimeElapsed;
	string			m_pcBuffer ;
	m4bool_t		m_bIsValid ;
} ;


// ******************************************************************************
// **
// **	Class ClUserActionsAudit.
// **
// **	API auditoria de acciones de usuario.
// **
// ******************************************************************************

class	M4_DECL_M4DM	ClUserActionsAudit : public ILogListener
{
public:

	// -----------------------------------------------------------------------------
	// -- Constructor.
	// -- 
	// -- ai_pChannelMngr: In. Channel Manager al que pertenece.
	// -----------------------------------------------------------------------------
	ClUserActionsAudit( ClChannelManager* ai_poChannelManager ) ;

	// -----------------------------------------------------------------------------
	// -- Destructor.
	// -----------------------------------------------------------------------------
	~ClUserActionsAudit( void ) ;

	// -----------------------------------------------------------------------------
	// -- Limpia el objeto completamente.
	// -----------------------------------------------------------------------------
	void	End( void ) ;

	// -----------------------------------------------------------------------------
	// -- Destruye los eventos almacenados.
	// -----------------------------------------------------------------------------
	void	Reset( void ) ;

	// -----------------------------------------------------------------------------
	// -- Activa internamente la auditoria.
	// -----------------------------------------------------------------------------
	m4bool_t	SwitchOn( void ) ;

	// -----------------------------------------------------------------------------
	// -- Desactiva internamente la auditoria y persiste si se pide.
	// -----------------------------------------------------------------------------
	m4bool_t	SwitchOff( m4bool_t ai_bPersist ) ;

	// -----------------------------------------------------------------------------
	// -- Comprueba si una sociedad tiene la auditoría activada para un tipo de evento.
	// -----------------------------------------------------------------------------
	m4bool_t	HasAuditory( ClUserAction::eAuditEventID ai_eEventID, m4pcchar_t ai_pccOrganization ) ;

	// -----------------------------------------------------------------------------
	// -- Añade una acción a partir de sus valores.
	// -----------------------------------------------------------------------------
	void	AddAction( m4pcchar_t ai_pccOrganization, m4pcchar_t ai_pccRole, ClUserAction::eAuditEventID ai_eEventID, m4pcchar_t ai_pccObject, m4pcchar_t ai_pccObjectName, m4pcchar_t ai_pccArgs, m4bool_t ai_bIsValid ) ;

	// -----------------------------------------------------------------------------
	// -- Añade una acción.
	// -----------------------------------------------------------------------------
	void	AddAction( ClUserAction* ai_pUserAction ) ;

	// -----------------------------------------------------------------------------
	// -- Añade una acción cronometrada a partir de sus valores.
	// -----------------------------------------------------------------------------
	void	AddTimedAction( long ai_iEventNum, m4date_t aio_dtEventTime, m4pcchar_t ai_pccOrganization, m4pcchar_t ai_pccEventSenderContainer, m4pcchar_t ai_pccEventSender, m4pcchar_t ai_pccParentEventSenderContainer, m4pcchar_t ai_pccParentEventSender, long ai_iTimeElapsed, m4pcchar_t ai_pccBuffer, m4bool_t ai_bIsValid ) ;

	// -----------------------------------------------------------------------------
	// -- Guarda los eventos en base datos si el estado es el adecuado.
	// -----------------------------------------------------------------------------
	m4return_t	Persist( m4bool_t ai_bPersistAlways ) ;

	// -----------------------------------------------------------------------------
	// -- Persistencia evento de error en logon de un usuario.
	// --
	// -- ai_pcUser:		In. Usuario que se conecta.
	// -- ai_pcClient:		In. Puesto cliente.
	// -----------------------------------------------------------------------------
	m4return_t	PersistLogonError( m4pchar_t ai_pcUser, m4pchar_t ai_pcClient, m4pchar_t ai_pcClientType ) ;

	// -----------------------------------------------------------------------------
	// -- Interfaz ILogListener.
	// --
	// -- ai_pccEventType:			In. Tipo de evento de log (_error_, _warning_, _info_).
	// -- ai_iEventId:				In. Identificador del evento de log.
	// -- ai_pccEventType:			In. Parámetros del evento de log.
	// -----------------------------------------------------------------------------
	m4return_t	LogEvent( m4pcchar_t ai_pccEventType, m4uint32_t ai_iEventId, m4pcchar_t ai_pccEventMessage ) ;

	// -----------------------------------------------------------------------------
	// -- Serializa el canal de auditoria de acciones de usuario.
	// --
	// -- IOD:	In. IO driver.
	// -----------------------------------------------------------------------------
	m4return_t	Serialize( ClGenericIODriver& IOD ) ;

	// -----------------------------------------------------------------------------
	// -- Deserializa el canal de auditoria de acciones de usuario.
	// --
	// -- IOD:	In. IO driver.
	// -----------------------------------------------------------------------------
	m4return_t	Deserialize( ClGenericIODriver& IOD ) ;

protected:

	m4bool_t					m_bActive ;
	ClChannelManager*			m_poChannelManager ;
	ClChannel*					m_poChannel ;
	ClAccess*					m_poAccess ;
	list<ClUserAction*>			m_lActions ;
	list<ClUserTimedAction*>	m_lTimedActions ;

protected:

	m4bool_t	_HasAuditory( ClUserAction::eAuditEventID ai_eEventID, m4pcchar_t &aio_rpccOrganization, m4bool_t &ao_rbIsValid ) ;
	m4bool_t	_HasTimedAuditory( long ai_iTimeElapsed, m4pcchar_t &aio_rpccOrganization, m4bool_t &ao_rbIsValid ) ;
	m4return_t	_GetNode( m4pcchar_t ai_pccNodeName, ClNode* &ao_rpoNode ) ;
	m4return_t	_Execute( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArgs, m4uint16_t ai_iLength ) ;
	m4return_t	_PersistActions( void ) ;
	m4return_t	_PersistTimedActions( void ) ;
	m4return_t	_PersistSystemInfo( m4pcchar_t ai_pccOrganization ) ;
	m4return_t  _GetSystemInfo(vector<string> & ao_vSystemInfo);
	m4return_t  _Tokenize(const string & ai_sString, const string & ai_sDelimiter, vector<string> & ao_vStrings);
	m4return_t	_WriteString( ClGenericIODriver& IOD, m4pcchar_t ai_pccPointer  ) ;
	m4return_t	_ReadString( ClGenericIODriver& IOD, m4pchar_t &ao_rpcPointer, m4uint32_t &ao_rSize ) ;
	m4bool_t	_AddAction( ClUserAction* ai_pUserAction, m4bool_t ai_bFirst ) ;
	m4return_t	_AddLogonAction() ;
	void		_ResetActions( void ) ;
	void		_ResetTimedActions( void ) ;
} ;

#endif



