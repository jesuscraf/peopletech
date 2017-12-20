//## begin module%37F862AC0227.cm preserve=no
//## end module%37F862AC0227.cm

//## begin module%37F862AC0227.cp preserve=no
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
//## end module%37F862AC0227.cp

//## Module: m4crono%37F862AC0227; Subprogram body
//## Subsystem: M4Date::src%3779DEFC01B7
//	d:\compon\m4date\version\src
//## Source file: d:\eduardoma\m4date\version\src\m4crono.cpp

//## begin module%37F862AC0227.additionalIncludes preserve=no
//## end module%37F862AC0227.additionalIncludes

//## begin module%37F862AC0227.includes preserve=yes
//## end module%37F862AC0227.includes

// m4clock
#include <m4clock.hpp>
// m4crono
#include <m4crono.hpp>
//## begin module%37F862AC0227.declarations preserve=no
//## end module%37F862AC0227.declarations

//## begin module%37F862AC0227.additionalDeclarations preserve=yes
//## end module%37F862AC0227.additionalDeclarations


// Class ClCrono 




ClCrono::ClCrono ()
  //## begin ClCrono::ClCrono%938765143.hasinit preserve=no
      : m_iStartMilliseconds(0)
  //## end ClCrono::ClCrono%938765143.hasinit
  //## begin ClCrono::ClCrono%938765143.initialization preserve=yes
  //## end ClCrono::ClCrono%938765143.initialization
{
  //## begin ClCrono::ClCrono%938765143.body preserve=yes
	m_iClockDelay = -1;		// Not initialized (lazy initialization)
  //## end ClCrono::ClCrono%938765143.body
}



//## Other Operations (implementation)
m4millisec_t ClCrono::GetDifference () const
{
  //## begin ClCrono::GetDifference%938765146.body preserve=yes
	if (0 == m_iStartMilliseconds)
		return 0;

	m4millisec_t	iNow = ClPrecisionTicker::GetTimeTick();
	
	return ClPrecisionTicker::GetAbsDifference(m_iStartMilliseconds, iNow);
  //## end ClCrono::GetDifference%938765146.body
}

m4millisec_t ClCrono::GetClockDelay ()
{
  //## begin ClCrono::GetClockDelay%938779152.body preserve=yes
	if (m_iClockDelay < 0)
	{
		m_iClockDelay = ClPrecisionTicker::QueryClockDelay();
	}

	return m_iClockDelay;
  //## end ClCrono::GetClockDelay%938779152.body
}

// Additional Declarations
  //## begin ClCrono%37F4699A0378.declarations preserve=yes
  //## end ClCrono%37F4699A0378.declarations

//## begin module%37F862AC0227.epilog preserve=yes
//## end module%37F862AC0227.epilog
