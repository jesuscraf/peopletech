//## begin module%3742D023002E.cm preserve=no
//## end module%3742D023002E.cm

//## begin module%3742D023002E.cp preserve=no
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
//## end module%3742D023002E.cp

//## Module: m4condition%3742D023002E; Package body
//## Subsystem: M4Ipcs::src::win32%3742D0230024
//	d:\fuentesservidor\m4ipcs\version\src\win32
//## Source file: D:\FuentesServidor\m4ipcs\version\src\win32\m4condition.cpp

//## begin module%3742D023002E.additionalIncludes preserve=no
//## end module%3742D023002E.additionalIncludes

//## begin module%3742D023002E.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:              m4condition.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:6/28/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
// Definition:
//
//    This module defines...
//
//
//
//==============================================================================

#include "m4condition.hpp"

//## end module%3742D023002E.includes

// m4types
#include <m4types.hpp>
//## begin module%3742D023002E.declarations preserve=no
//## end module%3742D023002E.declarations

//## begin module%3742D023002E.additionalDeclarations preserve=yes
//## end module%3742D023002E.additionalDeclarations


// Class ClCondition 




ClCondition::ClCondition ()
  //## begin ClCondition::ClCondition%-1099979947.hasinit preserve=no
  //## end ClCondition::ClCondition%-1099979947.hasinit
  //## begin ClCondition::ClCondition%-1099979947.initialization preserve=yes
  //## end ClCondition::ClCondition%-1099979947.initialization
{
  //## begin ClCondition::ClCondition%-1099979947.body preserve=yes
	m_hHandleCondVar = NULL;
	m_create = M4_FALSE;
	m_terminate = M4_FALSE;
  //## end ClCondition::ClCondition%-1099979947.body
}


ClCondition::~ClCondition ()
{
  //## begin ClCondition::~ClCondition%858029057.body preserve=yes
	if (m_terminate == M4_FALSE)
	   CloseHandle(m_hHandleCondVar);

  //## end ClCondition::~ClCondition%858029057.body
}



//## Other Operations (implementation)
m4return_t ClCondition::Init ()
{
  //## begin ClCondition::Init%-196230685.body preserve=yes
  if ((m_hHandleCondVar = CreateEvent(NULL, M4_FALSE, M4_FALSE, NULL)) == NULL)
	  return M4_ERROR;

  m_create = M4_TRUE;

  return M4_SUCCESS;
  //## end ClCondition::Init%-196230685.body
}

m4int16_t ClCondition::Wait (m4int32_t millisecs)
{
  //## begin ClCondition::Wait%-733722643.body preserve=yes

	if (m_create == M4_TRUE)
	{
		if (millisecs == -1)
			millisecs = INFINITE;

		switch (WaitForSingleObject(m_hHandleCondVar,
									millisecs))
		{
			case WAIT_ABANDONED:
			case WAIT_FAILED:
				return M4_COND_ERROR;

			case WAIT_OBJECT_0:
				return M4_COND_SIGNALED;

			case WAIT_TIMEOUT:
				return M4_COND_TIMEOUT;
		}
	}
	return M4_COND_ERROR;
  //## end ClCondition::Wait%-733722643.body
}

m4return_t ClCondition::Signal (void )
{
  //## begin ClCondition::Signal%715533459.body preserve=yes
	if (m_create == M4_TRUE)
	{

	   if (SetEvent(m_hHandleCondVar) == 0)
		  return M4_ERROR;
	
	   return M4_SUCCESS;
	}

	return M4_ERROR;
  //## end ClCondition::Signal%715533459.body
}

m4return_t ClCondition::Close ()
{
  //## begin ClCondition::Close%-216214797.body preserve=yes
	if (m_create == M4_TRUE)
	{
		if (CloseHandle(m_hHandleCondVar) == 0)
			return M4_ERROR;

		m_terminate = M4_TRUE;

		return M4_SUCCESS;
	}

	 return M4_ERROR;
  //## end ClCondition::Close%-216214797.body
}

m4return_t ClCondition::Reset ()
{
  //## begin ClCondition::Reset%-686858407.body preserve=yes
  if (ResetEvent(m_hHandleCondVar) == 0)
	  return M4_ERROR;

  return M4_SUCCESS;
  //## end ClCondition::Reset%-686858407.body
}

// Additional Declarations
  //## begin ClCondition%3742D01F038F.declarations preserve=yes
  //## end ClCondition%3742D01F038F.declarations

//## begin module%3742D023002E.epilog preserve=yes
//## end module%3742D023002E.epilog
