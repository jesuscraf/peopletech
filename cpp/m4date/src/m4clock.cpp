//## begin module%37F46B1401EB.cm preserve=no
//## end module%37F46B1401EB.cm

//## begin module%37F46B1401EB.cp preserve=no
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
//## end module%37F46B1401EB.cp

//## Module: m4clock%37F46B1401EB; Subprogram body
//## Subsystem: M4Date::src%3779DEFC01B7
//	d:\compon\m4date\version\src
//## Source file: d:\eduardoma\m4date\version\src\m4clock.cpp

//## begin module%37F46B1401EB.additionalIncludes preserve=no
//## end module%37F46B1401EB.additionalIncludes

//## begin module%37F46B1401EB.includes preserve=yes
#ifdef _WINDOWS
	#include <m4win.hpp>
#endif
#include <m4date.hpp>
//## end module%37F46B1401EB.includes

// m4clock
#include <m4clock.hpp>
// m4types
#include <m4types.hpp>
//## begin module%37F46B1401EB.declarations preserve=no
//## end module%37F46B1401EB.declarations

//## begin module%37F46B1401EB.additionalDeclarations preserve=yes
//## end module%37F46B1401EB.additionalDeclarations

/* Bug 0175418
Se crea una instancia para que se inicialicen las variables globales.
*/
static ClPrecisionTicker	oDummyTickerInitializer;

// Class Utility ClPrecisionTicker 

//## begin ClPrecisionTicker::s_bHighPerformance%37F478F503C1.role preserve=yes  public: static m4bool_t { -> 1VHAN}
#ifdef _WINDOWS
	m4bool_t ClPrecisionTicker::s_bHighPerformance;
#endif
//## end ClPrecisionTicker::s_bHighPerformance%37F478F503C1.role

//## begin ClPrecisionTicker::s_iPrecision%37F487ED01CD.role preserve=yes  public: m4millisec_t { -> 1VHAN}
#ifdef _WINDOWS
	m4millisec_t ClPrecisionTicker::s_iPrecision;
#endif
//## end ClPrecisionTicker::s_iPrecision%37F487ED01CD.role

//## begin ClPrecisionTicker::s_isInit%37F48B1500CB.role preserve=yes  private: m4bool_t { -> 1VHAN}
m4bool_t ClPrecisionTicker::s_isInit = M4_FALSE;
//## end ClPrecisionTicker::s_isInit%37F48B1500CB.role

ClPrecisionTicker::ClPrecisionTicker ()
  //## begin ClPrecisionTicker::ClPrecisionTicker%938765156.hasinit preserve=no
  //## end ClPrecisionTicker::ClPrecisionTicker%938765156.hasinit
  //## begin ClPrecisionTicker::ClPrecisionTicker%938765156.initialization preserve=yes
  //## end ClPrecisionTicker::ClPrecisionTicker%938765156.initialization
{
  //## begin ClPrecisionTicker::ClPrecisionTicker%938765156.body preserve=yes
	if (M4_FALSE == s_isInit)
	{
#ifdef _WINDOWS
		LARGE_INTEGER iPF;
		s_bHighPerformance = QueryPerformanceFrequency(&iPF);

		if (M4_FALSE != s_bHighPerformance)
		{
			s_iPrecision = (m4millisec_t)iPF.QuadPart;
		}
		else
		{
			s_iPrecision = (m4millisec_t)(DWORD)-1;	// GetTickCount value
		}
#endif
	}

	s_isInit = M4_TRUE;
  //## end ClPrecisionTicker::ClPrecisionTicker%938765156.body
}



//## Other Operations (implementation)
m4millisec_t ClPrecisionTicker::GetTimeTick ()
{
  //## begin ClPrecisionTicker::GetTimeTick%938765152.body preserve=yes
	m4millisec_t	 dMillisecs;
	m4millisec_t iMilliSeconds;

#ifdef _WINDOWS
   if (M4_FALSE != s_bHighPerformance)
   {
		LARGE_INTEGER iCounter;
		BOOL b;
		b = QueryPerformanceCounter(&iCounter);
		dMillisecs = (m4millisec_t)iCounter.QuadPart;
		dMillisecs *= 1000;
		dMillisecs /= s_iPrecision;
   }
   else
   {
	   dMillisecs = (m4millisec_t)GetTickCount();
   }
#else
   struct timeval sTimeStamp;
   gettimeofday( &sTimeStamp, NULL ) ;
   //Bug 0085060. Operaciones enteras.
   dMillisecs = (m4millisec_t)sTimeStamp.tv_sec * (m4millisec_t) 1000 ;
   dMillisecs += (m4millisec_t)sTimeStamp.tv_usec / (m4millisec_t) 1000 ;
#endif

	iMilliSeconds = (m4millisec_t)dMillisecs;

//Cogemos solamante los últimos 32 bits, porque al hacer el casting se pierde información
	iMilliSeconds &= 0xffffffff ;

   return iMilliSeconds;
  //## end ClPrecisionTicker::GetTimeTick%938765152.body
}

m4millisec_t ClPrecisionTicker::QueryClockDelay ()
{
  //## begin ClPrecisionTicker::QueryClockDelay%938765155.body preserve=yes
	m4millisec_t iStart, iEnd, iClockDelay;

	m4sleep(0);
	iStart = GetTimeTick();
	for (m4int_t i=0; i<2000;i++)
		GetTimeTick();
	iEnd = GetTimeTick();
	iClockDelay = (iEnd - iStart) / 2000;

	return iClockDelay;
  //## end ClPrecisionTicker::QueryClockDelay%938765155.body
}

// Additional Declarations
  //## begin ClPrecisionTicker%37F4878A03A1.declarations preserve=yes
  //## end ClPrecisionTicker%37F4878A03A1.declarations

//## begin module%37F46B1401EB.epilog preserve=yes
//## end module%37F46B1401EB.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClM4Clock::ClM4Clock%938765143.body preserve=no
	m_iClockDelay = -1;		// Not initialized (lazy initialization)
	Start();
//## end ClM4Clock::ClM4Clock%938765143.body

//## begin ClM4Clock::Start%938765144.body preserve=no
   m_iStartMilliseconds = ClPrecisionTicker::GetTimeTick();
//## end ClM4Clock::Start%938765144.body

//## begin ClM4Clock::Reset%938765145.body preserve=no
	m_iStartMilliseconds = 0 ;
	m_iClockDelay = ClPrecisionTicker::QueryClockDelay();
//## end ClM4Clock::Reset%938765145.body

//## begin ClM4Clock::GetDifference%938765146.body preserve=no
	if (0 == m_iStartMilliseconds)
		return 0;

	m4millisec_t	iNow = ClPrecisionTicker::GetTimeTick();
	
	return ClPrecisionTicker::GetAbsDifference(m_iStartMilliseconds, iNow);
//## end ClM4Clock::GetDifference%938765146.body

//## begin ClM4Clock::operator-%938765147.body preserve=no
	return ClPrecisionTicker::GetAbsDifference(m_iStartMilliseconds, ai_iTime.m_iStartMilliseconds);
//## end ClM4Clock::operator-%938765147.body

//## begin ClM4Clock::GetClockDelay%938779152.body preserve=no
	if (m_iClockDelay < 0)
	{
		m_iClockDelay = ClPrecisionTicker::QueryClockDelay();
	}

	return m_iClockDelay;
//## end ClM4Clock::GetClockDelay%938779152.body

#endif
