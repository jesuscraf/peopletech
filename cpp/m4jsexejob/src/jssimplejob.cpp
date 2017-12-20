//## begin module%37E601810264.cm preserve=no
//## end module%37E601810264.cm

//## begin module%37E601810264.cp preserve=no
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
//## end module%37E601810264.cp

//## Module: jssimplejob%37E601810264; Package body
//## Subsystem: M4JSExeJob::m4jsjob::src%37E244D003E3
//## Source file: C:\Trabajo\Server\m4jsexejob\version\src\jssimplejob.cpp

//## begin module%37E601810264.additionalIncludes preserve=no
//## end module%37E601810264.additionalIncludes

//## begin module%37E601810264.includes preserve=yes
//## end module%37E601810264.includes

// jssimplejob
#include <jssimplejob.hpp>
// jsjob
#include <jsjob.hpp>

//## begin module%37E601810264.declarations preserve=no
//## end module%37E601810264.declarations

//## begin module%37E601810264.additionalDeclarations preserve=yes
//## end module%37E601810264.additionalDeclarations


// Class ClJSSimpleJob 



ClJSSimpleJob::ClJSSimpleJob (const m4uint_t &ai_uiId)
  //## begin ClJSSimpleJob::ClJSSimpleJob%937821613.hasinit preserve=no
  //## end ClJSSimpleJob::ClJSSimpleJob%937821613.hasinit
  //## begin ClJSSimpleJob::ClJSSimpleJob%937821613.initialization preserve=yes
  : ClJSJob()
  //## end ClJSSimpleJob::ClJSSimpleJob%937821613.initialization
{
  //## begin ClJSSimpleJob::ClJSSimpleJob%937821613.body preserve=yes
	m_uiId = ai_uiId;
	m_poExecutionStrategy = ClJSExecutionStrategy::GetStrategyType( ClJSExecutionStrategy::LN4SimpleStrategy);
  //## end ClJSSimpleJob::ClJSSimpleJob%937821613.body
}



//## Other Operations (implementation)
void ClJSSimpleJob::PrintOn (ostream &ai_ostream)
{
  //## begin ClJSSimpleJob::PrintOn%938424582.body preserve=yes
	register short i;
	m4int_t depthLevel = 1;
	m4uint32_t idDefinition = 0;
	m4uint32_t idLocal = 0;

	if ( ai_ostream.opfx() )
	{
		depthLevel = CalculateDepthLevel( depthLevel);
		GetId( idDefinition);
		GetLocalId( idLocal);

		for (i = 0; i < (depthLevel * 3); i++) ai_ostream << " ";

		ai_ostream << idLocal << ".- " << idDefinition << endl;
		
		ai_ostream.osfx();
		PrintAllParametersOn( ai_ostream);
	}

  //## end ClJSSimpleJob::PrintOn%938424582.body
}

ClJSJob * ClJSSimpleJob::GetNextExecutableTask ()
{
  //## begin ClJSSimpleJob::GetNextExecutableTask%938600381.body preserve=yes
	ClJSJob * pJobRes = NULL;

	if (m_eStatus == ClJSJob::StatusWaiting) 
		pJobRes = this;

	return pJobRes;
  //## end ClJSSimpleJob::GetNextExecutableTask%938600381.body
}

ClJSJob * ClJSSimpleJob::GetTaskWithLocalId (const m4uint32_t &ai_uiLocalId)
{
  //## begin ClJSSimpleJob::GetTaskWithLocalId%939135441.body preserve=yes
	ClJSJob * res = NULL;

	if ( m_uiLocalId == ai_uiLocalId)
		res = this;

	return res;
  //## end ClJSSimpleJob::GetTaskWithLocalId%939135441.body
}

// Additional Declarations
  //## begin ClJSSimpleJob%375BE74303CD.declarations preserve=yes
  //## end ClJSSimpleJob%375BE74303CD.declarations

//## begin module%37E601810264.epilog preserve=yes
//## end module%37E601810264.epilog
