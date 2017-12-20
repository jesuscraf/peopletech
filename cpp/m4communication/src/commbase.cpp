//## begin module%3B4971C90025.cm preserve=no
//## end module%3B4971C90025.cm

//## begin module%3B4971C90025.cp preserve=no
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
//## end module%3B4971C90025.cp

//## Module: commbase%3B4971C90025; Package body
//## Subsystem: M4Communication::M4COML::src%38D1FCE80326
//## Source file: e:\mybuild\m4communication\src\commbase.cpp

//## begin module%3B4971C90025.additionalIncludes preserve=no
//## end module%3B4971C90025.additionalIncludes

//## begin module%3B4971C90025.includes preserve=yes
//## end module%3B4971C90025.includes

// commbase
#include <commbase.hpp>
// m4types
#include <m4types.hpp>
// m4condition
#include <m4condition.hpp>
//## begin module%3B4971C90025.declarations preserve=no
//## end module%3B4971C90025.declarations

//## begin module%3B4971C90025.additionalDeclarations preserve=yes
//## end module%3B4971C90025.additionalDeclarations


// Class ClCommCondition 




ClCommCondition::ClCommCondition ()
  //## begin ClCommCondition::ClCommCondition%994669370.hasinit preserve=no
  //## end ClCommCondition::ClCommCondition%994669370.hasinit
  //## begin ClCommCondition::ClCommCondition%994669370.initialization preserve=yes
  //## end ClCommCondition::ClCommCondition%994669370.initialization
{
  //## begin ClCommCondition::ClCommCondition%994669370.body preserve=yes
	m_oCondition.Init(1,0);
  //## end ClCommCondition::ClCommCondition%994669370.body
}


ClCommCondition::~ClCommCondition ()
{
  //## begin ClCommCondition::~ClCommCondition%994669371.body preserve=yes
  //## end ClCommCondition::~ClCommCondition%994669371.body
}



//## Other Operations (implementation)
m4return_t ClCommCondition::Signal ()
{
  //## begin ClCommCondition::Signal%994669372.body preserve=yes
	CondSleep(35);
//	return m_oCondition.Signal();
	return m_oCondition.Unlock();
  //## end ClCommCondition::Signal%994669372.body
}

m4return_t ClCommCondition::Wait (m4int32_t ai_millisecs)
{
  //## begin ClCommCondition::Wait%994669373.body preserve=yes

//	return m_oCondition.Wait(ai_millisecs);
	return m_oCondition.Lock (ai_millisecs);
  //## end ClCommCondition::Wait%994669373.body
}

m4return_t ClCommCondition::Reset ()
{
  //## begin ClCommCondition::Reset%994669374.body preserve=yes
//	return m_oCondition.Reset ();
	return M4_ERROR ;
  //## end ClCommCondition::Reset%994669374.body
}

void ClCommCondition::CondSleep (m4int_t ai_milisecond)
{
  //## begin ClCommCondition::CondSleep%994669375.body preserve=yes
#ifdef _WINDOWS
		Sleep(ai_milisecond);
#else
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = ai_milisecond*1000;
		(void) select(2, NULL, NULL, NULL, &timeout);
#endif
  //## end ClCommCondition::CondSleep%994669375.body
}

// Additional Declarations
  //## begin ClCommCondition%3B4971F201A0.declarations preserve=yes
  //## end ClCommCondition%3B4971F201A0.declarations

//## begin module%3B4971C90025.epilog preserve=yes
//## end module%3B4971C90025.epilog
