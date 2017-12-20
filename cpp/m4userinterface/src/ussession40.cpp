//## begin module%3993DA0800F2.cm preserve=no
//## end module%3993DA0800F2.cm

//## begin module%3993DA0800F2.cp preserve=no
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
//## end module%3993DA0800F2.cp

//## Module: ussession40%3993DA0800F2; Package body
//## Subsystem: M4UserInterface::src%3795DCBF0109
//## Source file: F:\integration\m4userinterface\src\ussession40.cpp

//## begin module%3993DA0800F2.additionalIncludes preserve=no
//## end module%3993DA0800F2.additionalIncludes

//## begin module%3993DA0800F2.includes preserve=yes
//## end module%3993DA0800F2.includes

// ussession40
#include <ussession40.hpp>
// USInterface
#include <usinterface.hpp>
// USSession
#include <ussession.hpp>
//## begin module%3993DA0800F2.declarations preserve=no
//## end module%3993DA0800F2.declarations

//## begin module%3993DA0800F2.additionalDeclarations preserve=yes
//## end module%3993DA0800F2.additionalDeclarations


// Class ClUSSession40 

ClUSSession40::ClUSSession40 (const m4uint32_t ai_uiId, const m4string_t &ai_strSessID, ClUSUserInterface *ai_poUser, ClStatistic *ai_poStats, ClUSSessionTicketInterface *ai_poUSTicket, ClVMBaseEnv* ai_poEnvironment, const m4uint32_t ai_uiCompType, const m4uint32_t ai_uiCompLevel, m4bool_t ai_bAuditSession)
  //## begin ClUSSession40::ClUSSession40%965977091.hasinit preserve=no
  //## end ClUSSession40::ClUSSession40%965977091.hasinit
  //## begin ClUSSession40::ClUSSession40%965977091.initialization preserve=yes
  : ClUSSessionInterface (ai_uiId, ai_strSessID, NULL /*conexion*/, ai_poUser, ai_poStats, ai_poUSTicket, ai_poEnvironment, ai_uiCompType, ai_uiCompLevel, ai_bAuditSession)
  //## end ClUSSession40::ClUSSession40%965977091.initialization
{
  //## begin ClUSSession40::ClUSSession40%965977091.body preserve=yes
	SetConnProtocol ( M4_NON_CONNECTION_ORIENTED_PROTOCOL ) ;

  //## end ClUSSession40::ClUSSession40%965977091.body
}


ClUSSession40::~ClUSSession40 ()
{
  //## begin ClUSSession40::~ClUSSession40%965977092.body preserve=yes
  //## end ClUSSession40::~ClUSSession40%965977092.body
}



//## Other Operations (implementation)
void ClUSSession40::AddRequest ()
{
  //## begin ClUSSession40::AddRequest%965977093.body preserve=yes
	if (NULL != m_poStatistic)
		((ClUSSessionStats *)m_poStatistic)->AddnPendingReqs(1);
  //## end ClUSSession40::AddRequest%965977093.body
}

void ClUSSession40::RemoveRequest ()
{
  //## begin ClUSSession40::RemoveRequest%965977094.body preserve=yes
	if (NULL != m_poStatistic)
	{
		((ClUSSessionStats *)m_poStatistic)->AddnPendingReqs(-1);
		((ClUSSessionStats *)m_poStatistic)->AddnProcessedReqs(1);
	}

	if (  ( GetStatus () == SessCanceling ) || ( GetStatus () == SessCanceled ) )
	{
		if ( m_poStatistic )
		{
			if ( ((ClUSSessionStats *)m_poStatistic)->GetnPendingReqs() == 0)
				SetStatus ( SessClosed	) ;
		}
	}
  //## end ClUSSession40::RemoveRequest%965977094.body
}

m4return_t ClUSSession40::Recycle ()
{
  //## begin ClUSSession40::Recycle%965977095.body preserve=yes
	if ( ! CanRecycle () )
		return M4_ERROR ;

/*	if (m_poUser )
	{
		--( m_poUser -> m_oNumSessions ) ;
	}
*/
	return M4_SUCCESS;
  //## end ClUSSession40::Recycle%965977095.body
}

m4bool_t ClUSSession40::CanRecycle ()
{
  //## begin ClUSSession40::CanRecycle%965977096.body preserve=yes
	m4uint32_t iReq ;
	ClUSSessionStats * poStatistic ;

	if ( poStatistic = (ClUSSessionStats *) GetStats () )
	{
		iReq = poStatistic -> GetnPendingReqs () ;
	}

	if ( ( GetStatus () == SessCanceling) && ( 0 == iReq ) )
	{
		SetStatus (SessClosed );
	}

	return (( GetStatus () == SessClosed)?M4_TRUE:M4_FALSE);
  //## end ClUSSession40::CanRecycle%965977096.body
}

m4return_t ClUSSession40::Destroy ()
{
  //## begin ClUSSession40::Destroy%965977097.body preserve=yes
	if ( ! CanDestroy() )
		return M4_ERROR ;

/*	if (m_poUser )
	{
		--( m_poUser -> m_oNumSessions ) ;
	}
*/

	delete this ;
	return M4_SUCCESS ;
  //## end ClUSSession40::Destroy%965977097.body
}

m4bool_t ClUSSession40::CanDestroy ()
{
  //## begin ClUSSession40::CanDestroy%965977098.body preserve=yes
	m4uint32_t iReq ;
	ClUSSessionStats * poStatistic ;

	if ( poStatistic = (ClUSSessionStats *) GetStats () )
	{
		iReq = poStatistic -> GetnPendingReqs () ;
	}

	if ( ( GetStatus () == SessCanceling) && ( 0 == iReq ) )
	{
		SetStatus (SessClosed );
	}

	return (( GetStatus () == SessClosed)?M4_TRUE:M4_FALSE);
  //## end ClUSSession40::CanDestroy%965977098.body
}

// Additional Declarations
  //## begin ClUSSession40%3993D1C20010.declarations preserve=yes
  //## end ClUSSession40%3993D1C20010.declarations

//## begin module%3993DA0800F2.epilog preserve=yes
//## end module%3993DA0800F2.epilog
