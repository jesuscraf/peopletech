//## begin module%34C860E40388.cm preserve=no
//## end module%34C860E40388.cm

//## begin module%34C860E40388.cp preserve=no
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
//## end module%34C860E40388.cp

//## Module: USSession%34C860E40388; Package body
//## Subsystem: M4UserInterface::src%3795DCBF0109
//## Source file: F:\integration\m4userinterface\src\ussession.cpp

//## begin module%34C860E40388.additionalIncludes preserve=no
//## end module%34C860E40388.additionalIncludes

//## begin module%34C860E40388.includes preserve=yes
#include <m4trace.hpp>
#include <subsystemids.hpp>
//	Rose 98 Body include
#include <channel.hpp>
#include <timeinterval.hpp>
//## end module%34C860E40388.includes

// Statistics
#include <statistics.hpp>
// USInterface
#include <usinterface.hpp>
// USSession
#include <ussession.hpp>


//## begin module%34C860E40388.declarations preserve=no
//## end module%34C860E40388.declarations

//## begin module%34C860E40388.additionalDeclarations preserve=yes
//## end module%34C860E40388.additionalDeclarations


// Class ClUSSessionVisitor 

ClUSSessionVisitor::ClUSSessionVisitor ()
  //## begin ClUSSessionVisitor::ClUSSessionVisitor%893349365.hasinit preserve=no
  //## end ClUSSessionVisitor::ClUSSessionVisitor%893349365.hasinit
  //## begin ClUSSessionVisitor::ClUSSessionVisitor%893349365.initialization preserve=yes
  //## end ClUSSessionVisitor::ClUSSessionVisitor%893349365.initialization
{
  //## begin ClUSSessionVisitor::ClUSSessionVisitor%893349365.body preserve=yes
  //## end ClUSSessionVisitor::ClUSSessionVisitor%893349365.body
}



//## Other Operations (implementation)
void ClUSSessionVisitor::VisitSession (ClUSSessionInterface &ai_oSession)
{
  //## begin ClUSSessionVisitor::VisitSession%885550067.body preserve=yes
  //## end ClUSSessionVisitor::VisitSession%885550067.body
}

// Class ClUSSessionStats 






ClUSSessionStats::ClUSSessionStats ()
  //## begin ClUSSessionStats::ClUSSessionStats%890086955.hasinit preserve=no
      : m_nPendingReqs(0), m_nProcessedReqs(0), m_nSubrogatedSessions(0), m_tLogonTime(0)
  //## end ClUSSessionStats::ClUSSessionStats%890086955.hasinit
  //## begin ClUSSessionStats::ClUSSessionStats%890086955.initialization preserve=yes
  //## end ClUSSessionStats::ClUSSessionStats%890086955.initialization
{
  //## begin ClUSSessionStats::ClUSSessionStats%890086955.body preserve=yes
	M4ClCrono			TimeMarker;
	m_uiNumberOfStats = TOTAL_NUM ;
	m_tLogonTime = TimeMarker.Start() ;
  //## end ClUSSessionStats::ClUSSessionStats%890086955.body
}


ClUSSessionStats::~ClUSSessionStats ()
{
  //## begin ClUSSessionStats::~ClUSSessionStats%890086956.body preserve=yes
  //## end ClUSSessionStats::~ClUSSessionStats%890086956.body
}



//## Other Operations (implementation)
m4return_t ClUSSessionStats::Reset ()
{
  //## begin ClUSSessionStats::Reset%887707931.body preserve=yes
	return M4_ERROR;
  //## end ClUSSessionStats::Reset%887707931.body
}

m4return_t ClUSSessionStats::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData)
{
  //## begin ClUSSessionStats::GetStat%887707932.body preserve=yes
	m4uint16_t				lowIndex;
	eSessionStats		statIndex;
	m4return_t				res = M4_TRUE;
	M4ClCrono			TimeMarker;
	M4ClString			szLogonTime ;
	ClTimeInterval		* poLogonTime ;
	m4uint32_t			iLenBuff = DEF_SESSION_SIZE ;

	if (M4_ERROR == _CheckStatID(ai_hStatId, M4_SESSION_SUBSYSTEM))
		return M4_ERROR;

	lowIndex = ai_hStatId.GetLowID();

	statIndex = (eSessionStats)lowIndex;

	if (0 < ao_poData.GetSize())
		return M4_ERROR;

	// fix bug 0152431
	// checks for LOGON_TIME type
	if (LOGON_TIME == statIndex)
	{
		 poLogonTime = new ClTimeInterval ( (TimeMarker.Start() - m_tLogonTime)/1000 ) ;

		 if ( M4_ERROR == poLogonTime -> Get ( szLogonTime ) )
			 return M4_ERROR ;

		 iLenBuff = szLogonTime.size() + 1 ;

	}

	ao_poData.SetSize( iLenBuff );
	m4pchar_t	buffer = ao_poData.GetBuffer();


	switch(statIndex)
	{
	case LOGON_TIME:

		sprintf ( buffer , szLogonTime ) ;

		break ;

	case NUM_PROCESED_REQ:
		sprintf ( buffer , "%10d" , m_nProcessedReqs ) ;

		break ;

	case PENDING_REQ:
		sprintf ( buffer , "%10d" , m_nPendingReqs ) ;

		break ;

	case NUM_SUBROGATED_SESSIONS:
		sprintf ( buffer , "%10d" , m_nSubrogatedSessions ) ;

		break ;

	default:
		sprintf(buffer, "%010d", 0);
		res = M4_FALSE;
	}

	return res;
  //## end ClUSSessionStats::GetStat%887707932.body
}

m4return_t ClUSSessionStats::ResetStat (ClStatHandle &ai_hStatId)
{
  //## begin ClUSSessionStats::ResetStat%887707933.body preserve=yes
	return M4_ERROR;
  //## end ClUSSessionStats::ResetStat%887707933.body
}

// Additional Declarations
  //## begin ClUSSessionStats%34E9A8C60175.declarations preserve=yes
  //## end ClUSSessionStats%34E9A8C60175.declarations

// Class ClUSSession 

//## begin ClUSSession::m_lTimeout%3AB8E1760320.attr preserve=no  private: static m4double_t {VA} 0
m4double_t ClUSSession::m_lTimeout = 0;
//## end ClUSSession::m_lTimeout%3AB8E1760320.attr

ClUSSession::ClUSSession (const m4uint32_t ai_uiId, const m4string_t &ai_strSessID, ClConnBase *ai_poConnection, ClUSUserInterface *ai_poUser, ClStatistic *ai_poStats, ClUSSessionTicketInterface *ai_poUSTicket, ClVMBaseEnv* ai_poEnvironment, const m4uint32_t ai_uiCompType, const m4uint32_t ai_uiCompLevel, m4bool_t ai_bAuditSession)
  //## begin ClUSSession::ClUSSession%893349373.hasinit preserve=no
  //## end ClUSSession::ClUSSession%893349373.hasinit
  //## begin ClUSSession::ClUSSession%893349373.initialization preserve=yes
  : ClUSSessionInterface (ai_uiId, ai_strSessID, ai_poConnection, ai_poUser, ai_poStats, ai_poUSTicket, ai_poEnvironment, ai_uiCompType, ai_uiCompLevel, ai_bAuditSession)
  //## end ClUSSession::ClUSSession%893349373.initialization
{
  //## begin ClUSSession::ClUSSession%893349373.body preserve=yes
	SetConnProtocol ( M4_CONNECTION_ORIENTED_PROTOCOL ) ;
  //## end ClUSSession::ClUSSession%893349373.body
}



//## Other Operations (implementation)
void ClUSSession::AddRequest ()
{
  //## begin ClUSSession::AddRequest%926668808.body preserve=yes
	if (NULL != m_poStatistic)
		((ClUSSessionStats *)m_poStatistic)->AddnPendingReqs(1);

  //## end ClUSSession::AddRequest%926668808.body
}

void ClUSSession::RemoveRequest ()
{
  //## begin ClUSSession::RemoveRequest%926668809.body preserve=yes
	if (NULL != m_poStatistic)
	{
		((ClUSSessionStats *)m_poStatistic)->AddnPendingReqs(-1);
		((ClUSSessionStats *)m_poStatistic)->AddnProcessedReqs(1);
	}

	if (  ( m_oStatus.GetValue () == SessCanceling ) || ( m_oStatus.GetValue () == SessCanceled ) )
	{
		if ( m_poStatistic )
		{
			if ( ((ClUSSessionStats *)m_poStatistic)->GetnPendingReqs() == 0)
				m_oStatus.SetValue ( SessClosed	) ;
		}
	}
  //## end ClUSSession::RemoveRequest%926668809.body
}

m4return_t ClUSSession::Recycle ()
{
  //## begin ClUSSession::Recycle%941721555.body preserve=yes
	if ( ! CanRecycle () )
		return M4_ERROR ;

	if (m_pUser )
	{
		--( m_pUser -> m_oNumSessions ) ;
	}

	return M4_SUCCESS;
  //## end ClUSSession::Recycle%941721555.body
}

m4bool_t ClUSSession::CanRecycle ()
{
  //## begin ClUSSession::CanRecycle%941721556.body preserve=yes
	return (( m_oStatus.GetValue () == SessClosed)?M4_TRUE:M4_FALSE);
  //## end ClUSSession::CanRecycle%941721556.body
}

m4return_t ClUSSession::Destroy ()
{
  //## begin ClUSSession::Destroy%941721557.body preserve=yes
	if ( ! CanDestroy() )
		return M4_ERROR ;

	if (m_pUser )
	{
		--( m_pUser -> m_oNumSessions ) ;
	}


	delete this ;
	return M4_SUCCESS ;
  //## end ClUSSession::Destroy%941721557.body
}

m4bool_t ClUSSession::CanDestroy ()
{
  //## begin ClUSSession::CanDestroy%941721558.body preserve=yes
	m4uint32_t iReq ;
	ClUSSessionStats * poStatistic ;

	if ( poStatistic = (ClUSSessionStats *) GetStats () )
	{
		iReq = poStatistic -> GetnPendingReqs () ;
	}

	if ( ( m_oStatus.GetValue () == SessCanceling) && ( 0 == iReq ) )
	{
		m_oStatus.SetValue (SessClosed );
	}

	return (( m_oStatus.GetValue () == SessClosed)?M4_TRUE:M4_FALSE);
  //## end ClUSSession::CanDestroy%941721558.body
}

m4return_t ClUSSession::CheckTimeout ()
{
  //## begin ClUSSession::CheckTimeout%985177783.body preserve=yes
	ClUSSessionStats * poStats ;
	M4ClTimeStamp oCrono;
	m4double_t dTime, dElapseTime;
	m4int32_t imin;


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

		if( m_lTimeout )
		{
			oCrono.now () ;

			oCrono.get ( dTime ) ;

			dElapseTime = dTime - dTimeLastTransaction  ; //En días
			dElapseTime *= 24 ; //Se pasa a horas
			dElapseTime *= 60 ; //Se pasa a minutos
			imin = dElapseTime ;


			if ( imin >= m_lTimeout ) 
			{
				return SESSION_TIMED_OUT ;
			}
		}
	}

	return ACTIVE_SESSION ;
  //## end ClUSSession::CheckTimeout%985177783.body
}

m4return_t ClUSSession::SetTimeout (m4double_t ai_lTimeout)
{
  //## begin ClUSSession::SetTimeout%985177784.body preserve=yes
	m_lTimeout = ai_lTimeout ;

	return M4_SUCCESS ;
  //## end ClUSSession::SetTimeout%985177784.body
}

m4double_t ClUSSession::GetTimeout ()
{
	return m_lTimeout ;
}

// Additional Declarations
  //## begin ClUSSession%353DE74101AD.declarations preserve=yes
  //## end ClUSSession%353DE74101AD.declarations

// Class ClUSSessionTicket 

ClUSSessionTicket::ClUSSessionTicket (m4char_t *ai_poTicketCode)
  //## begin ClUSSessionTicket::ClUSSessionTicket%926583146.hasinit preserve=no
  //## end ClUSSessionTicket::ClUSSessionTicket%926583146.hasinit
  //## begin ClUSSessionTicket::ClUSSessionTicket%926583146.initialization preserve=yes
  :ClUSSessionTicketInterface(ai_poTicketCode), m_pRowsQuotaCtrl(NULL)
  //## end ClUSSessionTicket::ClUSSessionTicket%926583146.initialization
{
  //## begin ClUSSessionTicket::ClUSSessionTicket%926583146.body preserve=yes
	m_oMutex.Init();
  //## end ClUSSessionTicket::ClUSSessionTicket%926583146.body
}


ClUSSessionTicket::~ClUSSessionTicket ()
{
  //## begin ClUSSessionTicket::~ClUSSessionTicket%926583148.body preserve=yes
	if (NULL != m_poTicketCode)
	{
		ClChannel	*poChannel = (ClChannel *)m_poTicketCode;
		poChannel->Destroy();
	}

	if (NULL != m_pRowsQuotaCtrl)
	{
		delete m_pRowsQuotaCtrl;
	}
  //## end ClUSSessionTicket::~ClUSSessionTicket%926583148.body
}



//## Other Operations (implementation)
m4pchar_t ClUSSessionTicket::GetTicketCode ()
{
  //## begin ClUSSessionTicket::GetTicketCode%926583149.body preserve=yes
	return m_poTicketCode;
  //## end ClUSSessionTicket::GetTicketCode%926583149.body
}

void* ClUSSessionTicket::GetRowsQuotaCtrl()
{
	return m_pRowsQuotaCtrl;
}

void ClUSSessionTicket::SetRowsQuotaCtrl(void* ai_pConCtroller)
{
	m_pRowsQuotaCtrl = ai_pConCtroller;
}

void ClUSSessionTicket::Lock()
{
	m_oMutex.Lock();
}

void ClUSSessionTicket::Unlock()
{
	m_oMutex.Unlock();
}

// Additional Declarations
  //## begin ClUSSessionTicket%373B18A90399.declarations preserve=yes
  //## end ClUSSessionTicket%373B18A90399.declarations

//## begin module%34C860E40388.epilog preserve=yes
//## end module%34C860E40388.epilog
