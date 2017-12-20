//## begin module%3422406B012F.cm preserve=no
//## end module%3422406B012F.cm

//## begin module%3422406B012F.cp preserve=no
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
//## end module%3422406B012F.cp

//## Module: USUser%3422406B012F; Package body
//## Subsystem: M4UserInterface::src%3795DCBF0109
//## Source file: D:\eduardoma\m4userinterface\src\ususer.cpp

//## begin module%3422406B012F.additionalIncludes preserve=no
//## end module%3422406B012F.additionalIncludes

//## begin module%3422406B012F.includes preserve=yes
//## end module%3422406B012F.includes

// Statistics
#include <statistics.hpp>
// USInterface
#include <usinterface.hpp>
// USUser
#include <ususer.hpp>
//## begin module%3422406B012F.declarations preserve=no
//## end module%3422406B012F.declarations

//## begin module%3422406B012F.additionalDeclarations preserve=yes
//## end module%3422406B012F.additionalDeclarations


// Class ClUSUserVisitor 

ClUSUserVisitor::ClUSUserVisitor ()
  //## begin ClUSUserVisitor::ClUSUserVisitor%893330502.hasinit preserve=no
  //## end ClUSUserVisitor::ClUSUserVisitor%893330502.hasinit
  //## begin ClUSUserVisitor::ClUSUserVisitor%893330502.initialization preserve=yes
  //## end ClUSUserVisitor::ClUSUserVisitor%893330502.initialization
{
  //## begin ClUSUserVisitor::ClUSUserVisitor%893330502.body preserve=yes
  //## end ClUSUserVisitor::ClUSUserVisitor%893330502.body
}



//## Other Operations (implementation)
void ClUSUserVisitor::VisitUser (ClUSUserInterface &ai_oUserInfo)
{
  //## begin ClUSUserVisitor::VisitUser%878029377.body preserve=yes
  //## end ClUSUserVisitor::VisitUser%878029377.body
}

// Class ClUSUserStats 


ClUSUserStats::ClUSUserStats ()
  //## begin ClUSUserStats::ClUSUserStats%887707934.hasinit preserve=no
      : m_nTotalSessions(0)
  //## end ClUSUserStats::ClUSUserStats%887707934.hasinit
  //## begin ClUSUserStats::ClUSUserStats%887707934.initialization preserve=yes
  //## end ClUSUserStats::ClUSUserStats%887707934.initialization
{
  //## begin ClUSUserStats::ClUSUserStats%887707934.body preserve=yes
	m_uiNumberOfStats = TOTAL_NUM ;
  //## end ClUSUserStats::ClUSUserStats%887707934.body
}


ClUSUserStats::~ClUSUserStats ()
{
  //## begin ClUSUserStats::~ClUSUserStats%887707935.body preserve=yes
  //## end ClUSUserStats::~ClUSUserStats%887707935.body
}



//## Other Operations (implementation)
m4return_t ClUSUserStats::Reset ()
{
  //## begin ClUSUserStats::Reset%887707940.body preserve=yes
	return M4_ERROR;
  //## end ClUSUserStats::Reset%887707940.body
}

m4return_t ClUSUserStats::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData)
{
  //## begin ClUSUserStats::GetStat%887707941.body preserve=yes
	m4uint16_t				lowIndex;
	eUserStats				statIndex;
	m4return_t				res = M4_TRUE;

	
	if (M4_ERROR == _CheckStatID(ai_hStatId, M4_USER_SUBSYSTEM))
		return M4_ERROR;

	lowIndex = ai_hStatId.GetLowID();

	statIndex = (eUserStats)lowIndex;

	ao_poData.SetSize(DEF_STRING_SIZE);
	m4pchar_t	buffer = ao_poData.GetBuffer();

// 
/*		enum {
				TOTAL_NUM
			} */

	switch(statIndex)
	{
	case ACTIVE_SESSIONS:
		sprintf ( buffer , "%ld" , m_nTotalSessions ) ;

		break ;

	default:
		sprintf(buffer, "%010d", 0);
		res = M4_FALSE;
	}

	return res;
  //## end ClUSUserStats::GetStat%887707941.body
}

m4return_t ClUSUserStats::ResetStat (ClStatHandle &ai_hStatId)
{
  //## begin ClUSUserStats::ResetStat%887707942.body preserve=yes
	return M4_ERROR;
  //## end ClUSUserStats::ResetStat%887707942.body
}

// Additional Declarations
  //## begin ClUSUserStats%34E9A7FB028C.declarations preserve=yes
  //## end ClUSUserStats%34E9A7FB028C.declarations

// Class ClUSUser 

ClUSUser::ClUSUser (const m4string_t &ai_strUserID, const m4string_t &ai_strName, ClStatistic *ai_poStatistic, const m4string_t ai_strUserAlias, const m4string_t ai_strUserImpersonator)
  //## begin ClUSUser::ClUSUser%893349374.hasinit preserve=no
  //## end ClUSUser::ClUSUser%893349374.hasinit
  //## begin ClUSUser::ClUSUser%893349374.initialization preserve=yes
  :	ClUSUserInterface (ai_strUserID, ai_strName, ai_poStatistic, ai_strUserAlias, ai_strUserImpersonator)
  //## end ClUSUser::ClUSUser%893349374.initialization
{
  //## begin ClUSUser::ClUSUser%893349374.body preserve=yes
  //## end ClUSUser::ClUSUser%893349374.body
}



//## Other Operations (implementation)
m4return_t ClUSUser::Recycle ()
{
  //## begin ClUSUser::Recycle%941721551.body preserve=yes

	if ( ! CanRecycle () )
		return M4_ERROR ;

	m_strName.erase();
	m_strUserID.erase();

	if ( m_poStatistic )
	{
		delete m_poStatistic ;
		m_poStatistic = NULL ;
	}
	return M4_SUCCESS ;
  //## end ClUSUser::Recycle%941721551.body
}

m4bool_t ClUSUser::CanRecycle ()
{
  //## begin ClUSUser::CanRecycle%941721552.body preserve=yes
	return  (( m_oNumSessions.GetValue() == 0 )?M4_TRUE:M4_FALSE ) ;
  //## end ClUSUser::CanRecycle%941721552.body
}

m4return_t ClUSUser::Destroy ()
{
  //## begin ClUSUser::Destroy%941721553.body preserve=yes
	if (! CanDestroy() )
		return M4_ERROR ;

	delete this ;
	return M4_SUCCESS ;
  //## end ClUSUser::Destroy%941721553.body
}

m4bool_t ClUSUser::CanDestroy ()
{
  //## begin ClUSUser::CanDestroy%941721554.body preserve=yes
		return  (( m_oNumSessions.GetValue() == 0 )?M4_TRUE:M4_FALSE ) ;
  //## end ClUSUser::CanDestroy%941721554.body
}

// Additional Declarations
  //## begin ClUSUser%353DE72200D2.declarations preserve=yes
  //## end ClUSUser%353DE72200D2.declarations

//## begin module%3422406B012F.epilog preserve=yes
//## end module%3422406B012F.epilog
