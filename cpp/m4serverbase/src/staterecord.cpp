//## begin module%37F105A802E2.cm preserve=no
//## end module%37F105A802E2.cm

//## begin module%37F105A802E2.cp preserve=no
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
//## end module%37F105A802E2.cp

//## Module: staterecord%37F105A802E2; Subprogram body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: F:\integration\m4serverbase\src\staterecord.cpp

//## begin module%37F105A802E2.additionalIncludes preserve=no
//## end module%37F105A802E2.additionalIncludes

//## begin module%37F105A802E2.includes preserve=yes
#include <m4date.hpp>
//## end module%37F105A802E2.includes

// m4string
#include <m4string.hpp>
// m4types
#include <m4types.hpp>
// synchutil
#include <synchutil.hpp>
// syncronization
#include <m4syncronization.hpp>
// m4clock
#include <m4clock.hpp>
// staterecord
#include <staterecord.hpp>
//## begin module%37F105A802E2.declarations preserve=no
//## end module%37F105A802E2.declarations

//## begin module%37F105A802E2.additionalDeclarations preserve=yes
//## end module%37F105A802E2.additionalDeclarations


// Class ClAbsTimePeriod 



ClAbsTimePeriod::ClAbsTimePeriod ()
  //## begin ClAbsTimePeriod::ClAbsTimePeriod%938686094.hasinit preserve=no
      : m_uiElapsedTime((m4uint_t)-1)
  //## end ClAbsTimePeriod::ClAbsTimePeriod%938686094.hasinit
  //## begin ClAbsTimePeriod::ClAbsTimePeriod%938686094.initialization preserve=yes
  	, m_dStartTime(0)
  //## end ClAbsTimePeriod::ClAbsTimePeriod%938686094.initialization
{
  //## begin ClAbsTimePeriod::ClAbsTimePeriod%938686094.body preserve=yes
  //## end ClAbsTimePeriod::ClAbsTimePeriod%938686094.body
}


// Class ClStateRecord 

//## begin ClStateRecord::M4NoneStateId%37F10B690005.attr preserve=no  public: static m4pchar_t {VAC} "M4NoneState"
const m4pchar_t  ClStateRecord::M4NoneStateId = "M4NoneState";
//## end ClStateRecord::M4NoneStateId%37F10B690005.attr

//## begin ClStateRecord::M4AllWindowSize%38870DFC00C8.attr preserve=no  public: static m4uint_t {VAC} 0
const m4uint_t  ClStateRecord::M4AllWindowSize = 0;
//## end ClStateRecord::M4AllWindowSize%38870DFC00C8.attr

//## begin ClStateRecord::M4DefWindowSize%38870E2802E8.attr preserve=no  public: static m4uint_t {VAC} 600000 
const m4uint_t  ClStateRecord::M4DefWindowSize = 172800000 ;
//## end ClStateRecord::M4DefWindowSize%38870E2802E8.attr






//## begin ClStateRecord::s_dReferenceTime%388725770053.role preserve=yes  protected: static m4millisec_t { -> 1VHAN}
m4millisec_t ClStateRecord::s_dReferenceTime = ClPrecisionTicker::GetTimeTick();
//## end ClStateRecord::s_dReferenceTime%388725770053.role



ClStateRecord::ClStateRecord (m4uint_t ai_uiWindowSize, m4uint64_t ai_uiStartRecordTime)
  //## begin ClStateRecord::ClStateRecord%938542582.hasinit preserve=no
  //## end ClStateRecord::ClStateRecord%938542582.hasinit
  //## begin ClStateRecord::ClStateRecord%938542582.initialization preserve=yes
  //## end ClStateRecord::ClStateRecord%938542582.initialization
{
  //## begin ClStateRecord::ClStateRecord%938542582.body preserve=yes
	m_uiStartRecordTime= 0 ;
	if (M4DefWindowSize < ai_uiWindowSize)
		m_uiWindowSize = M4DefWindowSize;
	else
		m_uiWindowSize = ai_uiWindowSize;

	m_strCurrentState = M4NoneStateId;

	if ( ai_uiStartRecordTime ) 
		m_uiStartRecordTime = ai_uiStartRecordTime ;
//		 	m_uiStartRecordTime = ClAbsTimePeriod::GetInterval(s_dReferenceTime, ai_uiStartRecordTime );

  //## end ClStateRecord::ClStateRecord%938542582.body
}

ClStateRecord::ClStateRecord (const M4ClString &ai_pszStartState, m4uint_t ai_uiWindowSize, m4uint64_t ai_uiStartRecordTime)
  //## begin ClStateRecord::ClStateRecord%938598202.hasinit preserve=no
  //## end ClStateRecord::ClStateRecord%938598202.hasinit
  //## begin ClStateRecord::ClStateRecord%938598202.initialization preserve=yes
  //## end ClStateRecord::ClStateRecord%938598202.initialization
{
  //## begin ClStateRecord::ClStateRecord%938598202.body preserve=yes
	m_uiStartRecordTime= 0 ;
	if (M4DefWindowSize < ai_uiWindowSize)
		m_uiWindowSize = M4DefWindowSize;
	else
		m_uiWindowSize = ai_uiWindowSize;

	m_strCurrentState = ai_pszStartState;
	if (M4NoneStateId != ai_pszStartState)
	{
		ChangeState(m_strCurrentState);
		if ( ! m_uiStartRecordTime )
			m_uiStartRecordTime = _GetCurrentRelTime();
	}
	if ( ai_uiStartRecordTime ) 
		 	m_uiStartRecordTime = ClAbsTimePeriod::GetInterval(s_dReferenceTime, ai_uiStartRecordTime );
  //## end ClStateRecord::ClStateRecord%938598202.body
}


ClStateRecord::~ClStateRecord ()
{
  //## begin ClStateRecord::~ClStateRecord%938542583.body preserve=yes
	Clear();
  //## end ClStateRecord::~ClStateRecord%938542583.body
}



//## Other Operations (implementation)
m4uint_t ClStateRecord::GetWindowSize ()
{
  //## begin ClStateRecord::GetWindowSize%938542584.body preserve=yes
	return m_uiWindowSize;
  //## end ClStateRecord::GetWindowSize%938542584.body
}

m4uint_t ClStateRecord::GetElapsedTime ()
{
  //## begin ClStateRecord::GetElapsedTime%939209123.body preserve=yes
	if (m_oStateInfoContainer.empty())
		return 0;

	m4uint_t	uiCurrentTime, uiElapsedTime, uiVoidTime = 0;
	m4size_t	iChanges;

	uiCurrentTime = _GetCurrentRelTime();
	uiElapsedTime = (uiCurrentTime - m_uiStartRecordTime);
	if (M4_SUCCESS == GetStateInfo(M4NoneStateId, iChanges, uiVoidTime))
	{
		uiElapsedTime -= uiVoidTime;
	}

	return uiElapsedTime;
  //## end ClStateRecord::GetElapsedTime%939209123.body
}

m4return_t ClStateRecord::GetCurrentState (M4ClString &ao_strCurrentState)
{
  //## begin ClStateRecord::GetCurrentState%938542585.body preserve=yes
	ClBlockReaders oLockWrapper(&m_oLocker);

	if (M4ClString(M4NoneStateId) == m_strCurrentState)
		return M4_ERROR;

	ao_strCurrentState = m_strCurrentState;

	return M4_SUCCESS;
  //## end ClStateRecord::GetCurrentState%938542585.body
}

m4return_t ClStateRecord::ChangeState (const M4ClString &ai_strStateId, m4uint64_t ai_uiTime)
{
  //## begin ClStateRecord::ChangeState%938542586.body preserve=yes
	ClBlockWriters oLockWrapper(&m_oLocker);

	if (ai_strStateId == m_strCurrentState)
		return M4_ERROR;

	ClStateSamples	*pNewState, *pCurrentSamples;
	if (m_oStateInfoContainer.empty())
	{
		if ( ! m_uiStartRecordTime )
			m_uiStartRecordTime = _GetCurrentRelTime();
	}
	else
	{
		if (M4_SUCCESS != _GetSamples(m_strCurrentState, pCurrentSamples))
			return M4_ERROR;
		pCurrentSamples->StopRecord(ai_uiTime);
		pCurrentSamples->UpdateInfo(m_uiWindowSize);
	}

	if (M4_SUCCESS != _GetSamples(ai_strStateId, pNewState))
	{
		pNewState = new ClStateSamples();
		TStateInfoMap::value_type	aValue(ai_strStateId, pNewState);
		m_oStateInfoContainer.insert(aValue);
	}

	if (M4_SUCCESS != pNewState->StartRecord(ai_uiTime))
		return M4_ERROR;

	m_strCurrentState = ai_strStateId;

	return M4_SUCCESS;
  //## end ClStateRecord::ChangeState%938542586.body
}

m4return_t ClStateRecord::GetStateInfo (const M4ClString &ai_strStateId, m4size_t &ao_iChanges, m4uint_t &ao_uiElapsedTime, m4uint_t ai_uiPeriod)
{
  //## begin ClStateRecord::GetStateInfo%938542587.body preserve=yes
	if (m_uiWindowSize < ai_uiPeriod)
		return M4_ERROR;

	ClBlockReaders oLockWrapper(&m_oLocker);

	ClStateSamples	*pNewState;
	if (M4_SUCCESS != _GetSamples(ai_strStateId, pNewState))
		return M4_ERROR;

	m4uint_t	uiAbsTime;
	if (M4AllWindowSize == ai_uiPeriod)
	{
		uiAbsTime = m_uiWindowSize;
	}
	else
	{
		uiAbsTime = ai_uiPeriod;
	}

	if (M4_SUCCESS != pNewState->GetInfo(ao_iChanges, ao_uiElapsedTime, uiAbsTime))
		return M4_ERROR;
	
	return M4_SUCCESS;
  //## end ClStateRecord::GetStateInfo%938542587.body
}

m4return_t ClStateRecord::GetAbsStateInfo (const M4ClString &ai_strStateId, m4size_t &ao_iChanges, m4uint_t &ao_uiElapsedTime)
{
  //## begin ClStateRecord::GetAbsStateInfo%939112991.body preserve=yes
	ClBlockReaders oLockWrapper(&m_oLocker);

	ClStateSamples	*pNewState;
	if (M4_SUCCESS != _GetSamples(ai_strStateId, pNewState))
		return M4_ERROR;

	if (M4_SUCCESS != pNewState->GetAbsInfo(ao_iChanges, ao_uiElapsedTime))
		return M4_ERROR;
	
	return M4_SUCCESS;
  //## end ClStateRecord::GetAbsStateInfo%939112991.body
}

void ClStateRecord::Clear ()
{
  //## begin ClStateRecord::Clear%938598208.body preserve=yes
	ClBlockWriters oLockWrapper(&m_oLocker);

	TStateInfoMap::iterator		it;
	ClStateSamples				*ptrSamples;

	for (it = m_oStateInfoContainer.begin(); it != m_oStateInfoContainer.end(); it = m_oStateInfoContainer.begin())
	{
		ptrSamples = (*it).second;
		ptrSamples->Clear();

		m_oStateInfoContainer.erase(it);
		delete ptrSamples;
	}

	m_oStateInfoContainer.clear();

	m_strCurrentState = M4NoneStateId;

	m_uiStartRecordTime = 0;

  //## end ClStateRecord::Clear%938598208.body
}

m4return_t ClStateRecord::ResetState ()
{
  //## begin ClStateRecord::ResetState%940526051.body preserve=yes
	if (M4ClString(M4NoneStateId) == m_strCurrentState)
		return M4_SUCCESS;

	return ChangeState(M4NoneStateId);
  //## end ClStateRecord::ResetState%940526051.body
}

m4return_t ClStateRecord::RecordChange (const M4ClString &ai_strStateId, m4millisec_t ai_dStartTime, m4uint_t ai_uiElapsedTime)
{
  //## begin ClStateRecord::RecordChange%940581812.body preserve=yes
	ClBlockWriters oLockWrapper(&m_oLocker);

	if (m_oStateInfoContainer.empty())
	{
		m_uiStartRecordTime = _GetCurrentRelTime();
	}

	ClStateSamples	*pNewState;
	if (M4_SUCCESS != _GetSamples(ai_strStateId, pNewState))
	{
		pNewState = new ClStateSamples();
		TStateInfoMap::value_type	aValue(ai_strStateId, pNewState);
		m_oStateInfoContainer.insert(aValue);
	}

	if (M4_SUCCESS != pNewState->InsertSample(ai_dStartTime, ai_uiElapsedTime))
		return M4_ERROR;

	pNewState->UpdateInfo(m_uiWindowSize);

	return M4_SUCCESS;
  //## end ClStateRecord::RecordChange%940581812.body
}

m4return_t ClStateRecord::_GetSamples (const M4ClString &ai_strStateId, ClStateSamples *&ao_poSamples)
{
  //## begin ClStateRecord::_GetSamples%938598214.body preserve=yes
	TStateInfoMap::iterator			mapIter;

	if (m_oStateInfoContainer.empty())
		return M4_ERROR;

	if ( m_oStateInfoContainer.end() == (mapIter = m_oStateInfoContainer.find(ai_strStateId)))
		return M4_ERROR;

	ao_poSamples = (*mapIter).second;

	return M4_SUCCESS;
  //## end ClStateRecord::_GetSamples%938598214.body
}

void ClStateRecord::_UpdateRecord ()
{
  //## begin ClStateRecord::_UpdateRecord%938598203.body preserve=yes
	TStateInfoMap::iterator		it;
	ClStateSamples				*ptrSamples;

	if (m_oStateInfoContainer.empty())
		return;

	for (it = m_oStateInfoContainer.begin(); it != m_oStateInfoContainer.end(); it = m_oStateInfoContainer.begin())
	{
		ptrSamples = (*it).second;
		ptrSamples->UpdateInfo(m_uiWindowSize);
	}
  //## end ClStateRecord::_UpdateRecord%938598203.body
}

m4return_t ClStateRecord::GetTimePassToState (M4ClString &ai_strState, m4uint_t &ao_iTimeTo)
{
  //## begin ClStateRecord::GetTimePassToState%974128927.body preserve=yes
	ClStateSamples	*pCurrentSamples;
	m4uint64_t iStopTime ;

	if (m_oStateInfoContainer.empty())
	{
		return M4_ERROR ;
	}

	if (M4_SUCCESS != _GetSamples(ai_strState, pCurrentSamples))
		return M4_ERROR;

	if ( M4_ERROR == pCurrentSamples->GetLastStopTime(iStopTime) )
	{
		return M4_ERROR ;
	}

	ao_iTimeTo = ClAbsTimePeriod::GetInterval(m_uiStartRecordTime , iStopTime );
/*
	ao_iTimeTo = ClAbsTimePeriod::GetInterval(s_dReferenceTime, iStopTime );

	ao_iTimeTo = ClAbsTimePeriod::GetInterval( m_uiStartRecordTime , ao_iTimeTo );
*/
	return M4_SUCCESS ;
  //## end ClStateRecord::GetTimePassToState%974128927.body
}

// Additional Declarations
  //## begin ClStateRecord%37F1023C0288.declarations preserve=yes
  //## end ClStateRecord%37F1023C0288.declarations

// Class ClStateSamples 





ClStateSamples::ClStateSamples ()
  //## begin ClStateSamples::ClStateSamples%938598210.hasinit preserve=no
  //## end ClStateSamples::ClStateSamples%938598210.hasinit
  //## begin ClStateSamples::ClStateSamples%938598210.initialization preserve=yes
  :	m_iTotalElapsedTime(0), m_iTotalSamples(0), m_bIsActive(M4_FALSE)
  //## end ClStateSamples::ClStateSamples%938598210.initialization
{
  //## begin ClStateSamples::ClStateSamples%938598210.body preserve=yes
  //## end ClStateSamples::ClStateSamples%938598210.body
}

ClStateSamples::ClStateSamples (m4millisec_t ai_dStartTime)
  //## begin ClStateSamples::ClStateSamples%938598211.hasinit preserve=no
  //## end ClStateSamples::ClStateSamples%938598211.hasinit
  //## begin ClStateSamples::ClStateSamples%938598211.initialization preserve=yes
  :	m_iTotalElapsedTime(0), m_iTotalSamples(0), m_bIsActive(M4_FALSE)
  //## end ClStateSamples::ClStateSamples%938598211.initialization
{
  //## begin ClStateSamples::ClStateSamples%938598211.body preserve=yes
	StartRecord();
  //## end ClStateSamples::ClStateSamples%938598211.body
}


ClStateSamples::~ClStateSamples ()
{
  //## begin ClStateSamples::~ClStateSamples%938598212.body preserve=yes
	Clear();
  //## end ClStateSamples::~ClStateSamples%938598212.body
}



//## Other Operations (implementation)
m4return_t ClStateSamples::GetInfo (m4size_t &ao_iChanges, m4uint_t &ao_uiElapsedTime, m4uint_t ai_uiPeriod)
{
  //## begin ClStateSamples::GetInfo%938598204.body preserve=yes
	TPeriodList::iterator		it;
	ClAbsTimePeriod				*pStateInfo;
	m4millisec_t				iStartTime, iStopTime, iCurrentTime, iFromTime;
	m4bool_t					bDone = M4_FALSE;

	iCurrentTime = ClPrecisionTicker::GetTimeTick();
	iFromTime = iCurrentTime - ai_uiPeriod;
	if (m_oTimePeriodContainer.empty())
		return M4_ERROR;

	ao_iChanges = 0;
	ao_uiElapsedTime = 0;
	it = m_oTimePeriodContainer.begin();
	if (M4_TRUE == IsActive())
	{
		ao_iChanges = 1;
		if (M4_SUCCESS != GetTimeUpTo(ao_uiElapsedTime))
			return M4_ERROR;
		if (ao_uiElapsedTime > ai_uiPeriod)
		{
			ao_uiElapsedTime = ai_uiPeriod;
			return M4_SUCCESS;
		}

		it++;
	}
	while (m_oTimePeriodContainer.end() != it && !bDone)
	{
		pStateInfo = *it;
		iStartTime = pStateInfo->GetAbsStartTime();
		iStopTime = pStateInfo->GetAbsStopTime();
		if (iStartTime > iFromTime)
		{
			ao_uiElapsedTime += pStateInfo->GetElapsedTime();
		}
		else
		{
			if (iStopTime > iFromTime)
			{
				ao_uiElapsedTime += ClAbsTimePeriod::GetInterval(iFromTime, iStopTime);
			}
			else
			{
				bDone = M4_TRUE;
				continue;
			}
		}

		ao_iChanges++;
		it++;
	}

	return M4_SUCCESS;
  //## end ClStateSamples::GetInfo%938598204.body
}

m4return_t ClStateSamples::GetAbsInfo (m4size_t &ao_iChanges, m4uint_t &ao_uiElapsedTime)
{
  //## begin ClStateSamples::GetAbsInfo%939112992.body preserve=yes
	m4uint_t		uiElapsedTime;

	ao_iChanges = m_iTotalSamples;
	ao_uiElapsedTime = m_iTotalElapsedTime;
	if (M4_TRUE == IsActive())
	{
		if (M4_SUCCESS != GetTimeUpTo(uiElapsedTime))
			return M4_ERROR;

		ao_uiElapsedTime += uiElapsedTime;
	}

	return M4_SUCCESS;
  //## end ClStateSamples::GetAbsInfo%939112992.body
}

void ClStateSamples::UpdateInfo (m4uint_t ai_uiWindowSize)
{
  //## begin ClStateSamples::UpdateInfo%938598205.body preserve=yes
	ClAbsTimePeriod			*pStateInfo;
	m4millisec_t			iStartTime, iCurrentTime;

	iCurrentTime = ClPrecisionTicker::GetTimeTick();
	m4bool_t				bDone = M4_FALSE;

	while (!m_oTimePeriodContainer.empty() && !bDone)
	{
		pStateInfo = m_oTimePeriodContainer.back();
		iStartTime = pStateInfo->GetAbsStartTime();
		if (iStartTime < (iCurrentTime - ai_uiWindowSize))
		{
			m_oTimePeriodContainer.pop_back();
			delete pStateInfo;
		}
		else
		{
			bDone = M4_TRUE;
		}
	}
  //## end ClStateSamples::UpdateInfo%938598205.body
}

m4return_t ClStateSamples::StartRecord (m4uint64_t ai_iStartTime)
{
  //## begin ClStateSamples::StartRecord%938598206.body preserve=yes
	ClAbsTimePeriod		*pStateInfo;
	m4millisec_t		iCurrentTime ;
	
	if ( ai_iStartTime )
	{
		iCurrentTime = ai_iStartTime ;
	}
	else
	{
		iCurrentTime = ClPrecisionTicker::GetTimeTick();
	}

	if (M4_TRUE == IsActive())
		return M4_ERROR;

	pStateInfo = new ClAbsTimePeriod;
	pStateInfo->SetAbsStartTime(iCurrentTime);

	m_oTimePeriodContainer.push_front(pStateInfo);
	m_iTotalSamples++;
	SetActive(M4_TRUE);

	return M4_SUCCESS;
  //## end ClStateSamples::StartRecord%938598206.body
}

m4return_t ClStateSamples::StopRecord (m4uint64_t ai_iStopTime)
{
  //## begin ClStateSamples::StopRecord%938598207.body preserve=yes
	if (m_oTimePeriodContainer.empty())
		return M4_ERROR;

	if (M4_TRUE != IsActive())
		return M4_ERROR;

	ClAbsTimePeriod		*pStateInfo = m_oTimePeriodContainer.front();
	m4millisec_t		iStartTime;
	m4millisec_t		iCurrentTime ;
	
	if ( ai_iStopTime )
	{
		iCurrentTime = ai_iStopTime ;
	}
	else
	{
		iCurrentTime = ClPrecisionTicker::GetTimeTick();
	}

	iStartTime = pStateInfo->GetAbsStartTime();
	if (iCurrentTime < iStartTime)
		return M4_ERROR;

	m4uint_t	iDiff = ClAbsTimePeriod::GetInterval(iStartTime, iCurrentTime);

	pStateInfo->SetElapsedTime(iDiff);
	m_iTotalElapsedTime += iDiff;
	SetActive(M4_FALSE);

	return M4_SUCCESS;
  //## end ClStateSamples::StopRecord%938598207.body
}

void ClStateSamples::Clear ()
{
  //## begin ClStateSamples::Clear%938598209.body preserve=yes
	ClAbsTimePeriod				*stateInfo;

	while (!m_oTimePeriodContainer.empty())
	{
		stateInfo = (ClAbsTimePeriod *)m_oTimePeriodContainer.back();
		delete stateInfo;
		m_oTimePeriodContainer.pop_back();
	}

	m_iTotalElapsedTime = 0;
	m_iTotalSamples = 0;
	SetActive(M4_FALSE);
  //## end ClStateSamples::Clear%938598209.body
}

m4return_t ClStateSamples::InsertSample (m4millisec_t ai_dStartTime, m4uint_t ai_uiElapsedTime)
{
  //## begin ClStateSamples::InsertSample%940581813.body preserve=yes
	ClAbsTimePeriod			*pNewStateInfo, *pOldStateInfo;
	m4millisec_t			iStartTime, iCurrentTime;
	m4bool_t				bDone = M4_FALSE;

	iCurrentTime = ClPrecisionTicker::GetTimeTick();
	if (iCurrentTime < (ai_dStartTime + ai_uiElapsedTime))
		return M4_ERROR;

	TPeriodList::iterator		it;
	it = m_oTimePeriodContainer.begin();

	pNewStateInfo = new ClAbsTimePeriod;
	pNewStateInfo->SetAbsStartTime(ai_dStartTime);
	pNewStateInfo->SetElapsedTime(ai_uiElapsedTime);
	while (m_oTimePeriodContainer.end() != it && !bDone)
	{
		pOldStateInfo = *it;
		iStartTime = pOldStateInfo->GetAbsStartTime();
		if (iStartTime <= ai_dStartTime)
		{
			m_oTimePeriodContainer.insert(it, pNewStateInfo);
			bDone = M4_TRUE;
		}
		it++;
	}

	if (M4_FALSE == bDone)
		m_oTimePeriodContainer.insert(it, pNewStateInfo);

	m_iTotalElapsedTime += ai_uiElapsedTime;		// WARNING! Elapsed Time may overlap
	m_iTotalSamples++;
	return M4_SUCCESS;
  //## end ClStateSamples::InsertSample%940581813.body
}

m4return_t ClStateSamples::GetLastStopTime (m4uint64_t &ao_iStopTime)
{
  //## begin ClStateSamples::GetLastStopTime%974128928.body preserve=yes
	TPeriodList::iterator		it;
	ClAbsTimePeriod				*pStateInfo;
	m4millisec_t				iStopTime;
	m4uint_t					iperiods;


	ao_iStopTime = 0 ;
	iperiods = m_oTimePeriodContainer.size() ;

	if ( ! iperiods )
		return M4_ERROR;

	if ( (M4_TRUE == IsActive()) && ( iperiods == 1 ) )
	{
		return M4_ERROR ;
	}

	it = m_oTimePeriodContainer.begin();

	while (m_oTimePeriodContainer.end() != it )
	{
		pStateInfo = *it;
		iStopTime = pStateInfo->GetAbsStopTime();
		if (iStopTime > ao_iStopTime)
		{
			ao_iStopTime = iStopTime ;
		}

		it++;
	}

	return M4_SUCCESS;
  //## end ClStateSamples::GetLastStopTime%974128928.body
}

//## begin module%37F105A802E2.epilog preserve=yes
//## end module%37F105A802E2.epilog
