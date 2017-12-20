//## begin module%37443E740248.cm preserve=no
//## end module%37443E740248.cm

//## begin module%37443E740248.cp preserve=no
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
//## end module%37443E740248.cp

//## Module: syncrowait%37443E740248; Package body
//## Subsystem: M4Ipcs::src%3742D0220389
//	d:\fuentesservidor\m4ipcs\version\src
//## Source file: D:\FuentesServidor\m4ipcs\version\src\syncrowait.cpp

//## begin module%37443E740248.additionalIncludes preserve=no
//## end module%37443E740248.additionalIncludes

//## begin module%37443E740248.includes preserve=yes
//## end module%37443E740248.includes

// boolcondition
#include <boolcondition.hpp>
// syncro
#include <syncro.hpp>
// syncrowait
#include <syncrowait.hpp>
//## begin module%37443E740248.declarations preserve=no
//## end module%37443E740248.declarations

//## begin module%37443E740248.additionalDeclarations preserve=yes
//## end module%37443E740248.additionalDeclarations


// Class ClSyncroWait 






ClSyncroWait::ClSyncroWait ()
  //## begin ClSyncroWait::ClSyncroWait%927221338.hasinit preserve=no
      : m_bIsInitialized(M4_FALSE), m_iActualState(1), m_iMaxState(1), m_poBoolCond(NULL), m_poMutex(NULL)
  //## end ClSyncroWait::ClSyncroWait%927221338.hasinit
  //## begin ClSyncroWait::ClSyncroWait%927221338.initialization preserve=yes
  //## end ClSyncroWait::ClSyncroWait%927221338.initialization
{
  //## begin ClSyncroWait::ClSyncroWait%927221338.body preserve=yes
  //## end ClSyncroWait::ClSyncroWait%927221338.body
}


ClSyncroWait::~ClSyncroWait ()
{
  //## begin ClSyncroWait::~ClSyncroWait%927221339.body preserve=yes
	if (m_poMutex != NULL)
		delete m_poMutex;
	if (m_poBoolCond != NULL)
		delete m_poBoolCond;
  //## end ClSyncroWait::~ClSyncroWait%927221339.body
}



//## Other Operations (implementation)
m4return_t ClSyncroWait::Init (m4int32_t ai_iMaxState, m4int32_t ai_iInitialState)
{
  //## begin ClSyncroWait::Init%927221340.body preserve=yes
	if (m_bIsInitialized)
		return M4_WARNING;

	if ( ai_iMaxState < 1 || ai_iInitialState < 0 ||
		ai_iInitialState > ai_iMaxState)
		return M4_ERROR;

	m_iActualState = ai_iInitialState;
	m_iMaxState = ai_iMaxState;

	m_poMutex = new ClMutex(M4_TRUE);
	if (m_poMutex == NULL)
		return M4_ERROR;

	m_poBoolCond = new ClBooleanCondition(m_poMutex);
	if (m_poBoolCond == NULL || m_poBoolCond->Init() != M4_SUCCESS)
		return M4_ERROR;

	m_bIsInitialized = M4_TRUE;

	return M4_SUCCESS;

  //## end ClSyncroWait::Init%927221340.body
}

m4return_t ClSyncroWait::Lock (void )
{
  //## begin ClSyncroWait::Lock%927221335.body preserve=yes
	return Lock(0);
  //## end ClSyncroWait::Lock%927221335.body
}

m4return_t ClSyncroWait::Lock (m4uint32_t ai_iTimeOut)
{
  //## begin ClSyncroWait::Lock%927221337.body preserve=yes
	if (!m_bIsInitialized)
		return M4_ERROR;

	m4return_t ret;

	m_poBoolCond->Lock();

	while (m_iActualState == 0) {
		ret = m_poBoolCond->Wait(ai_iTimeOut);
		if ( ret != M4_SUCCESS)
		{
			m_poBoolCond->Unlock();
			return ret;
		}
	}

	m_iActualState--;
	m_poBoolCond->Unlock();
	return M4_SUCCESS;
  //## end ClSyncroWait::Lock%927221337.body
}

m4return_t ClSyncroWait::Unlock (void )
{
  //## begin ClSyncroWait::Unlock%927221336.body preserve=yes
	if (!m_bIsInitialized)
		return M4_ERROR;

	m_poBoolCond->Lock();

	if (m_iActualState < m_iMaxState)
		m_iActualState++;

	m_poBoolCond->Notify();
	m_poBoolCond->Unlock();
	return M4_SUCCESS;
  //## end ClSyncroWait::Unlock%927221336.body
}

// Additional Declarations
  //## begin ClSyncroWait%37443D1A01A1.declarations preserve=yes
  //## end ClSyncroWait%37443D1A01A1.declarations

//## begin module%37443E740248.epilog preserve=yes
//## end module%37443E740248.epilog
