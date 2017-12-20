//## begin module%37F1057E00B1.cm preserve=no
//## end module%37F1057E00B1.cm

//## begin module%37F1057E00B1.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%37F1057E00B1.cp

//## Module: staterecord%37F1057E00B1; Subprogram specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\staterecord.hpp

#ifndef staterecord_h
#define staterecord_h 1

//## begin module%37F1057E00B1.additionalIncludes preserve=no
//## end module%37F1057E00B1.additionalIncludes

//## begin module%37F1057E00B1.includes preserve=yes
#include <m4serverbase_dll.hpp>
#include "m4stl.hpp"
//## end module%37F1057E00B1.includes

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
//## begin module%37F1057E00B1.declarations preserve=no
//## end module%37F1057E00B1.declarations

//## begin module%37F1057E00B1.additionalDeclarations preserve=yes
//## end module%37F1057E00B1.additionalDeclarations


//## Class: ClAbsTimePeriod%37F0FDA00119
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClAbsTimePeriod 
{
  public:
    //## Constructors (specified)
      //## Operation: ClAbsTimePeriod%938686094
      ClAbsTimePeriod ();


    //## Other Operations (specified)
      //## Operation: GetAbsStartTime%938598221; C++
      m4millisec_t GetAbsStartTime () const;

      //## Operation: GetAbsStopTime%938598222; C++
      m4millisec_t GetAbsStopTime () const;

      //## Operation: GetElapsedTime%938598223; C++
      m4uint_t GetElapsedTime () const;

      //## Operation: SetAbsStartTime%938598224; C++
      void SetAbsStartTime (m4millisec_t ai_dStartTime);

      //## Operation: SetElapsedTime%938598225; C++
      void SetElapsedTime (m4uint_t ai_uiElapsedTime);

      //## Operation: GetInterval%948372618; C++
      static m4uint_t GetInterval (m4millisec_t ai_dStartTime, m4millisec_t ai_dStopTime);

  protected:
    // Data Members for Associations

      //## Association: M4ServerBase::Statistics::<unnamed>%37F0FE63017D
      //## Role: ClAbsTimePeriod::m_dStartTime%37F0FE6302E7
      //## begin ClAbsTimePeriod::m_dStartTime%37F0FE6302E7.role preserve=no  protected: m4millisec_t { -> 1VHAN}
      m4millisec_t m_dStartTime;
      //## end ClAbsTimePeriod::m_dStartTime%37F0FE6302E7.role

      //## Association: M4ServerBase::Statistics::<unnamed>%37F0FEB90253
      //## Role: ClAbsTimePeriod::m_uiElapsedTime%37F0FEBA0007
      //## begin ClAbsTimePeriod::m_uiElapsedTime%37F0FEBA0007.role preserve=no  protected: m4uint_t { -> 1VHAN}
      m4uint_t m_uiElapsedTime;
      //## end ClAbsTimePeriod::m_uiElapsedTime%37F0FEBA0007.role

  private:
  private: //## implementation
};

//## Class: TPeriodList%37F10190008C; Instantiated Class
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37F101A500FA;ClAbsTimePeriod { -> }

typedef list< ClAbsTimePeriod * > TPeriodList;

//## Class: ClStateSamples%37F1DB7A03D2
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37F4B07B00EE;ClPrecisionTicker { -> }

class ClStateSamples 
{
  public:
    //## Constructors (specified)
      //## Operation: ClStateSamples%938598210; C++
      ClStateSamples ();

      //## Operation: ClStateSamples%938598211; C++
      ClStateSamples (m4millisec_t ai_dStartTime);

    //## Destructor (specified)
      //## Operation: ~ClStateSamples%938598212; C++
      ~ClStateSamples ();


    //## Other Operations (specified)
      //## Operation: GetInfo%938598204; C++
      m4return_t GetInfo (m4size_t &ao_iChanges, m4uint_t &ao_uiElapsedTime, m4uint_t ai_uiPeriod);

      //## Operation: GetAbsInfo%939112992; C++
      m4return_t GetAbsInfo (m4size_t &ao_iChanges, m4uint_t &ao_uiElapsedTime);

      //## Operation: UpdateInfo%938598205; C++
      void UpdateInfo (m4uint_t ai_uiWindowSize);

      //## Operation: StartRecord%938598206; C++
      m4return_t StartRecord (m4uint64_t ai_iStartTime = 0);

      //## Operation: StopRecord%938598207; C++
      m4return_t StopRecord (m4uint64_t ai_iStopTime = 0);

      //## Operation: Clear%938598209; C++
      void Clear ();

      //## Operation: InsertSample%940581813; C++
      m4return_t InsertSample (m4millisec_t ai_dStartTime, m4uint_t ai_uiElapsedTime);

      //## Operation: GetLastStopTime%974128928
      m4return_t GetLastStopTime (m4uint64_t &ao_iStopTime);

  protected:

    //## Other Operations (specified)
      //## Operation: GetTimeUpTo%939112994; C++
      m4return_t GetTimeUpTo (m4uint_t &ao_uiElapsedTime) const;

      //## Operation: IsActive%939112993
      m4bool_t IsActive () const;

      //## Operation: SetActive%939112995; C++
      void SetActive (m4bool_t ai_bActive);

    // Data Members for Associations

      //## Association: M4ServerBase::Statistics::<unnamed>%37F1DC270100
      //## Role: ClStateSamples::m_oTimePeriodContainer%37F1DC270254
      //## begin ClStateSamples::m_oTimePeriodContainer%37F1DC270254.role preserve=no  protected: TPeriodList { -> 1VHAN}
      TPeriodList m_oTimePeriodContainer;
      //## end ClStateSamples::m_oTimePeriodContainer%37F1DC270254.role

      //## Association: M4ServerBase::Statistics::<unnamed>%37F9B936011B
      //## Role: ClStateSamples::m_iTotalElapsedTime%37F9B93A0329
      //## begin ClStateSamples::m_iTotalElapsedTime%37F9B93A0329.role preserve=no  protected: m4uint_t { -> 1VHAN}
      m4uint_t m_iTotalElapsedTime;
      //## end ClStateSamples::m_iTotalElapsedTime%37F9B93A0329.role

      //## Association: M4ServerBase::Statistics::<unnamed>%37F9B9F2031A
      //## Role: ClStateSamples::m_iTotalSamples%37F9B9F300F5
      //## begin ClStateSamples::m_iTotalSamples%37F9B9F300F5.role preserve=no  protected: m4size_t { -> 1VHAN}
      m4size_t m_iTotalSamples;
      //## end ClStateSamples::m_iTotalSamples%37F9B9F300F5.role

      //## Association: M4ServerBase::Statistics::<unnamed>%37F9C26D0217
      //## Role: ClStateSamples::m_bIsActive%37F9C26E007E
      //## begin ClStateSamples::m_bIsActive%37F9C26E007E.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_bIsActive;
      //## end ClStateSamples::m_bIsActive%37F9C26E007E.role

  private:
  private: //## implementation
};

//## Class: TStateInfoMap%37F101F70030; Instantiated Class
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37F1020901D1;ClStateSamples { -> }
//## Uses: <unnamed>%37F10B2D0370;M4ClString { -> }

typedef map< M4ClString,ClStateSamples *,less< M4ClString > > TStateInfoMap;

//## begin ClStateRecord%37F1023C0288.preface preserve=yes
//## end ClStateRecord%37F1023C0288.preface

//## Class: ClStateRecord%37F1023C0288
//	This class provide a public interface for recording of elapsed time and
//	number of transitions for a number of user-defined states.
//## Category: M4ServerBase::Statistics%341FE6E202CA
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37F209380394;ClBlockWriters { -> }
//## Uses: <unnamed>%37F20A6E0390;ClBlockReaders { -> }
//## Uses: <unnamed>%37F9B9AA0168;m4size_t { -> }
//## Uses: <unnamed>%38870D4800FB;ClPrecisionTicker { -> }

class M4_DECL_M4SERVERBASE ClStateRecord 
{
  //## begin ClStateRecord%37F1023C0288.initialDeclarations preserve=yes
  //## end ClStateRecord%37F1023C0288.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClStateRecord%938542582; C++
      //	Parameterized constructor. Accepts a window size in milliseconds as the
      //	maximun recording time. It also acts as a default constructor with a
      //	predefined window size (M4DefWindowSize).
      ClStateRecord (m4uint_t ai_uiWindowSize = M4DefWindowSize, m4uint64_t ai_uiStartRecordTime = 0);

      //## Operation: ClStateRecord%938598202; C++
      //	Parameterized constructor. Accepts a starting state and a window size in
      //	milliseconds as the maximun recording time (M4DefWindowSize).
      ClStateRecord (const M4ClString &ai_pszStartState, m4uint_t ai_uiWindowSize = M4DefWindowSize, m4uint64_t ai_uiStartRecordTime = 0);

    //## Destructor (specified)
      //## Operation: ~ClStateRecord%938542583
      //	Destructor.
      ~ClStateRecord ();


    //## Other Operations (specified)
      //## Operation: GetWindowSize%938542584
      //	Returns the maximum window size in milliseconds.
      m4uint_t GetWindowSize ();

      //## Operation: GetElapsedTime%939209123
      //	Returns theabsolute recording time in milliseconds.
      m4uint_t GetElapsedTime ();

      //## Operation: GetCurrentState%938542585; C++
      //	Gets the current state in a string as an output parameter. Returns M4_
      //	SUCCESS upon successfull execution, M4_ERROR otherwise. If no starting state
      //	was ever set, it returns a constant value (M4NoneStateId).
      m4return_t GetCurrentState (M4ClString &ao_strCurrentState);

      //## Operation: ChangeState%938542586; C++
      //	Changes the current state to a new one. Accepts a new state id as an input
      //	paramter. Returns M4_ERROR if both states share the same id or some other
      //	technological error occurred.
      m4return_t ChangeState (const M4ClString &ai_strStateId, m4uint64_t ai_uiTime = 0);

      //## Operation: GetStateInfo%938542587; C++
      //	Gets the number of transitions and elapsed time for a state in a time
      //	window. Accepts a state Id and [optionally] a window size as input
      //	parameters. The number of transitions and elapsed time are output
      //	paramters.  Returns M4_SUCCESS upon successfull execution, M4_ERROR if the
      //	state information could not be retrieved.
      m4return_t GetStateInfo (const M4ClString &ai_strStateId, m4size_t &ao_iChanges, m4uint_t &ao_uiElapsedTime, m4uint_t ai_uiPeriod = M4AllWindowSize);

      //## Operation: GetAbsStateInfo%939112991; C++
      //	Gets the number of transitions and elapsed time for a state since the
      //	recording started. Accepts a state Id as input parameter. The number of
      //	transitions and elapsed time are output paramters.  Returns M4_SUCCESS upon
      //	successfull execution, M4_ERROR if the state information could not be
      //	retrieved.
      m4return_t GetAbsStateInfo (const M4ClString &ai_strStateId, m4size_t &ao_iChanges, m4uint_t &ao_uiElapsedTime);

      //## Operation: Clear%938598208; C++
      //	Releases previous data and resets all states recording.
      void Clear ();

      //## Operation: ResetState%940526051; C++
      //	Changes the current state to M4NoneState. Returns M4_ERROR if  some other
      //	technological error occurred.
      m4return_t ResetState ();

      //## Operation: RecordChange%940581812; C++
      m4return_t RecordChange (const M4ClString &ai_strStateId, m4millisec_t ai_dStartTime, m4uint_t ai_uiElapsedTime);

      //## Operation: GetStartTime%948372619
      m4uint64_t GetStartTime () const;

      //## Operation: GetTimePassToState%974128927
      m4return_t GetTimePassToState (M4ClString &ai_strState, m4uint_t &ao_iTimeTo);

    // Data Members for Class Attributes

      //## Attribute: M4NoneStateId%37F10B690005
      //	String id for unselected state.
      //## begin ClStateRecord::M4NoneStateId%37F10B690005.attr preserve=no  public: static m4pchar_t {VAC} "M4NoneState"
      static const m4pchar_t  M4NoneStateId;
      //## end ClStateRecord::M4NoneStateId%37F10B690005.attr

      //## Attribute: M4AllWindowSize%38870DFC00C8
      //## begin ClStateRecord::M4AllWindowSize%38870DFC00C8.attr preserve=no  public: static m4uint_t {VAC} 0
      static const m4uint_t  M4AllWindowSize;
      //## end ClStateRecord::M4AllWindowSize%38870DFC00C8.attr

      //## Attribute: M4DefWindowSize%38870E2802E8
      //## begin ClStateRecord::M4DefWindowSize%38870E2802E8.attr preserve=no  public: static m4uint_t {VAC} 600000 
      static const m4uint_t  M4DefWindowSize;
      //## end ClStateRecord::M4DefWindowSize%38870E2802E8.attr

    // Additional Public Declarations
      //## begin ClStateRecord%37F1023C0288.public preserve=yes
      //## end ClStateRecord%37F1023C0288.public

  protected:

    //## Other Operations (specified)
      //## Operation: _GetCurrentRelTime%948372620
      static m4uint_t _GetCurrentRelTime ();

      //## Operation: _GetSamples%938598214; C++
      //	Retrieves the info record for a certain state.
      m4return_t _GetSamples (const M4ClString &ai_strStateId, ClStateSamples *&ao_poSamples);

      //## Operation: _UpdateRecord%938598203
      //	Ensures the recording sticks to the defeined window size.
      void _UpdateRecord ();

    // Data Members for Associations

      //## Association: M4ServerBase::Statistics::<unnamed>%37F1025A03A4
      //## Role: ClStateRecord::m_oStateInfoContainer%37F1025B01BB
      //	This class member contains a table of states whose entries hold the record
      //	information.
      //## begin ClStateRecord::m_oStateInfoContainer%37F1025B01BB.role preserve=no  protected: TStateInfoMap { -> 1VHAN}
      TStateInfoMap m_oStateInfoContainer;
      //## end ClStateRecord::m_oStateInfoContainer%37F1025B01BB.role

      //## Association: M4ServerBase::Statistics::<unnamed>%37F1081700C1
      //## Role: ClStateRecord::m_uiWindowSize%37F1081702D4
      //	This class member holds the maximum recording time in milliseconds for the
      //	object. M4DefWindowSize is the default.
      //## begin ClStateRecord::m_uiWindowSize%37F1081702D4.role preserve=no  protected: m4uint_t { -> 1VHAN}
      m4uint_t m_uiWindowSize;
      //## end ClStateRecord::m_uiWindowSize%37F1081702D4.role

      //## Association: M4ServerBase::Statistics::<unnamed>%37F10B3E02B6
      //## Role: ClStateRecord::m_strCurrentState%37F10B3F0088
      //	This class member contains the current active state id. M4NoneStateId is the
      //	default.
      //## begin ClStateRecord::m_strCurrentState%37F10B3F0088.role preserve=no  protected: M4ClString { -> 1VHAN}
      M4ClString m_strCurrentState;
      //## end ClStateRecord::m_strCurrentState%37F10B3F0088.role

      //## Association: M4ServerBase::Statistics::<unnamed>%37F2048F0352
      //## Role: ClStateRecord::m_oLocker%37F2049000A1
      //	This class member provides the MT synchronization in terms of the readers
      //	and writers strategy.
      //## begin ClStateRecord::m_oLocker%37F2049000A1.role preserve=no  protected: ClWriterReaders { -> 1VHAN}
      ClWriterReaders m_oLocker;
      //## end ClStateRecord::m_oLocker%37F2049000A1.role

      //## Association: M4ServerBase::Statistics::<unnamed>%37FB21E8016B
      //## Role: ClStateRecord::m_uiStartRecordTime%37FB21E90299
      //## begin ClStateRecord::m_uiStartRecordTime%37FB21E90299.role preserve=no  protected: m4uint_t { -> 1VHAN}
      m4millisec_t m_uiStartRecordTime;
      //## end ClStateRecord::m_uiStartRecordTime%37FB21E90299.role

      //## Association: M4ServerBase::Statistics::<unnamed>%3887257601BA
      //## Role: ClStateRecord::s_dReferenceTime%388725770053
      //## begin ClStateRecord::s_dReferenceTime%388725770053.role preserve=no  protected: static m4millisec_t { -> 1VHAN}
      static m4millisec_t s_dReferenceTime;
      //## end ClStateRecord::s_dReferenceTime%388725770053.role

    // Additional Protected Declarations
      //## begin ClStateRecord%37F1023C0288.protected preserve=yes
      //## end ClStateRecord%37F1023C0288.protected

  private:
    // Additional Private Declarations
      //## begin ClStateRecord%37F1023C0288.private preserve=yes
      //## end ClStateRecord%37F1023C0288.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClStateRecord%37F1023C0288.implementation preserve=yes
      //## end ClStateRecord%37F1023C0288.implementation

};

//## begin ClStateRecord%37F1023C0288.postscript preserve=yes
//## end ClStateRecord%37F1023C0288.postscript

// Class ClAbsTimePeriod 


//## Other Operations (inline)
inline m4millisec_t ClAbsTimePeriod::GetAbsStartTime () const
{
  //## begin ClAbsTimePeriod::GetAbsStartTime%938598221.body preserve=yes
	return m_dStartTime;
  //## end ClAbsTimePeriod::GetAbsStartTime%938598221.body
}

inline m4millisec_t ClAbsTimePeriod::GetAbsStopTime () const
{
  //## begin ClAbsTimePeriod::GetAbsStopTime%938598222.body preserve=yes
	if (0 > m_uiElapsedTime)		return 0;

	return (m_dStartTime + m_uiElapsedTime);
  //## end ClAbsTimePeriod::GetAbsStopTime%938598222.body
}

inline m4uint_t ClAbsTimePeriod::GetElapsedTime () const
{
  //## begin ClAbsTimePeriod::GetElapsedTime%938598223.body preserve=yes
	if (0 > m_uiElapsedTime)		return 0;

	return m_uiElapsedTime;
  //## end ClAbsTimePeriod::GetElapsedTime%938598223.body
}

inline void ClAbsTimePeriod::SetAbsStartTime (m4millisec_t ai_dStartTime)
{
  //## begin ClAbsTimePeriod::SetAbsStartTime%938598224.body preserve=yes
	m_dStartTime = ai_dStartTime;
  //## end ClAbsTimePeriod::SetAbsStartTime%938598224.body
}

inline void ClAbsTimePeriod::SetElapsedTime (m4uint_t ai_uiElapsedTime)
{
  //## begin ClAbsTimePeriod::SetElapsedTime%938598225.body preserve=yes
	m_uiElapsedTime = ai_uiElapsedTime;
  //## end ClAbsTimePeriod::SetElapsedTime%938598225.body
}

inline m4uint_t ClAbsTimePeriod::GetInterval (m4millisec_t ai_dStartTime, m4millisec_t ai_dStopTime)
{
  //## begin ClAbsTimePeriod::GetInterval%948372618.body preserve=yes
	if (ai_dStopTime < ai_dStartTime)	return (m4uint_t)-1;

	return (m4uint_t)(ai_dStopTime - ai_dStartTime);
  //## end ClAbsTimePeriod::GetInterval%948372618.body
}

// Class ClStateSamples 


//## Other Operations (inline)
inline m4return_t ClStateSamples::GetTimeUpTo (m4uint_t &ao_uiElapsedTime) const
{
  //## begin ClStateSamples::GetTimeUpTo%939112994.body preserve=yes
	m4millisec_t		iStartTime, iCurrentTime;

	iCurrentTime = ClPrecisionTicker::GetTimeTick();
	if (m_oTimePeriodContainer.empty())
		return M4_ERROR;

	ClAbsTimePeriod		*pStateInfo = m_oTimePeriodContainer.front();
	if (M4_TRUE != IsActive())
	{
		ao_uiElapsedTime = pStateInfo->GetElapsedTime();
	}
	else
	{
		iStartTime = pStateInfo->GetAbsStartTime();
		if (iCurrentTime < iStartTime)
			return M4_ERROR;

		ao_uiElapsedTime = ClAbsTimePeriod::GetInterval(iStartTime, iCurrentTime);
	}

	return M4_SUCCESS;
  //## end ClStateSamples::GetTimeUpTo%939112994.body
}

inline m4bool_t ClStateSamples::IsActive () const
{
  //## begin ClStateSamples::IsActive%939112993.body preserve=yes
	return m_bIsActive;
  //## end ClStateSamples::IsActive%939112993.body
}

inline void ClStateSamples::SetActive (m4bool_t ai_bActive)
{
  //## begin ClStateSamples::SetActive%939112995.body preserve=yes
	m_bIsActive = ai_bActive;
  //## end ClStateSamples::SetActive%939112995.body
}

// Class ClStateRecord 


//## Other Operations (inline)
inline m4uint64_t ClStateRecord::GetStartTime () const
{
  //## begin ClStateRecord::GetStartTime%948372619.body preserve=yes
	return m_uiStartRecordTime;
  //## end ClStateRecord::GetStartTime%948372619.body
}

inline m4uint_t ClStateRecord::_GetCurrentRelTime ()
{
  //## begin ClStateRecord::_GetCurrentRelTime%948372620.body preserve=yes
	return ClAbsTimePeriod::GetInterval(s_dReferenceTime, ClPrecisionTicker::GetTimeTick());
  //## end ClStateRecord::_GetCurrentRelTime%948372620.body
}

//## begin module%37F1057E00B1.epilog preserve=yes
//## end module%37F1057E00B1.epilog


#endif
