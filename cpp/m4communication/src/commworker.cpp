//## begin module%38DB639D0141.cm preserve=no
//## end module%38DB639D0141.cm

//## begin module%38DB639D0141.cp preserve=no
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
//## end module%38DB639D0141.cp

//## Module: CommWorker%38DB639D0141; Package body
//## Subsystem: M4Communication::M4COML::src%38D1FCE80326
//## Source file: e:\mybuild\m4communication\src\commworker.cpp

//## begin module%38DB639D0141.additionalIncludes preserve=no
#include <m4trace.hpp>
//## end module%38DB639D0141.additionalIncludes

//## begin module%38DB639D0141.includes preserve=yes
#include <m4date.hpp>
//## end module%38DB639D0141.includes

// m4thread
#include <m4thread.hpp>
// m4types
#include <m4types.hpp>
// CommWorker
#include <commworker.hpp>
// CommAction
#include <commaction.hpp>
// m4crono
#include <m4crono.hpp>
// m4clock
#include <m4clock.hpp>
//## begin module%38DB639D0141.declarations preserve=no
//## end module%38DB639D0141.declarations

//## begin module%38DB639D0141.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID "COMM"
#include <m4srvtrace.h>



ClMutex ClCommWorker::m_oActiveWorkerMutex(M4_TRUE);
m4int_t ClCommWorker::m_iNActivesWorker=0;

//## end module%38DB639D0141.additionalDeclarations


// Class ClCommWorker 















ClCommWorker::ClCommWorker (ClActionQueue &ai_oActionQ)
  //## begin ClCommWorker::ClCommWorker%954259092.hasinit preserve=no
      : m_bIsReady(M4_FALSE), m_poActionQueue(NULL), m_bShuttingDown(M4_FALSE), m_msLastActionExecutionTime(0), m_msExecutionTime(0), m_msWaitingTime(0), m_uiExecutedActions(0), m_msExternalExecutingTime(0)
  //## end ClCommWorker::ClCommWorker%954259092.hasinit
  //## begin ClCommWorker::ClCommWorker%954259092.initialization preserve=yes
  //## end ClCommWorker::ClCommWorker%954259092.initialization
{
  //## begin ClCommWorker::ClCommWorker%954259092.body preserve=yes
	m_poActionQueue=&ai_oActionQ;
  //## end ClCommWorker::ClCommWorker%954259092.body
}



//## Other Operations (implementation)
void ClCommWorker::Run (void )
{
  //## begin ClCommWorker::Run%961088169.body preserve=yes
	SetIdThread(GetCurrentIdThread());
	
	if(!m_poActionQueue)
		return;
	m_bIsReady=M4_TRUE;

	AddedWorker();

	M4_SRV_GENERAL("Worker %0:s running",m_iWorkerID);
	ClCommAction *theAction;
	m4return_t iRet;
	m_oExternalClock.SetCounter(m_msExternalExecutingTime);
	m_oExternalClock.SetClock(&m_oExtExecutingCrono);

	while (!IsTerminate())
	{
 		m_oWaitingCrono.Start();
		m_poActionQueue->Pop(theAction);
		m_msWaitingTime+=m_oWaitingCrono.Stop();
		
		if(theAction && !IsTerminate())
		{
			theAction->SetExtExecutionCrono(&m_oExternalClock);
			m_oExecutingCrono.Start();
			iRet=theAction->Execute();
			m_msExecutionTime+=m_oExecutingCrono.Stop();
			m_uiExecutedActions++;
			m4DebugExecute(if(M4_ERROR==iRet)    cerr<<" ERROR!!"<<endl);
			delete theAction;
			if(iRet==M4_COMM_SHUTDOWN_WORKER)
				break;
		}
		else
			break;

	}
	M4_SRV_GENERAL_N("Worker Exiting");
	DelWorker();
  //## end ClCommWorker::Run%961088169.body
}

m4return_t ClCommWorker::Stop ()
{
  //## begin ClCommWorker::Stop%961088170.body preserve=yes

	Terminate();
	return M4_SUCCESS;
  //## end ClCommWorker::Stop%961088170.body
}

m4uint_t ClCommWorker::GetLastActionExecutionTime ()
{
  //## begin ClCommWorker::GetLastActionExecutionTime%964423553.body preserve=yes
	return 0; //m_dLastActionExecutionTime;
  //## end ClCommWorker::GetLastActionExecutionTime%964423553.body
}

m4uint_t ClCommWorker::GetExecutionTime ()
{
  //## begin ClCommWorker::GetExecutionTime%964423554.body preserve=yes
	return (m4uint_t)m_msExecutionTime;
  //## end ClCommWorker::GetExecutionTime%964423554.body
}

m4uint_t ClCommWorker::GetWaitingTime ()
{
  //## begin ClCommWorker::GetWaitingTime%964423555.body preserve=yes
	return (m4uint_t)(m_msWaitingTime+m_oWaitingCrono.GetEllapsedTime());
  //## end ClCommWorker::GetWaitingTime%964423555.body
}

m4uint_t ClCommWorker::GetExecutedActions ()
{
  //## begin ClCommWorker::GetExecutedActions%964423556.body preserve=yes
	return m_uiExecutedActions;
  //## end ClCommWorker::GetExecutedActions%964423556.body
}

m4uint_t ClCommWorker::GetExternalExecutingTime ()
{
  //## begin ClCommWorker::GetExternalExecutingTime%965977801.body preserve=yes
	return (m4uint_t)m_msExternalExecutingTime;
  //## end ClCommWorker::GetExternalExecutingTime%965977801.body
}

m4bool_t ClCommWorker::IsReady ()
{
  //## begin ClCommWorker::IsReady%967794007.body preserve=yes
	return m_bIsReady;
  //## end ClCommWorker::IsReady%967794007.body
}

m4return_t ClCommWorker::SetID (m4int_t ai_iWorkerID)
{
  //## begin ClCommWorker::SetID%967794008.body preserve=yes
	m_iWorkerID=ai_iWorkerID;
	return M4_SUCCESS;;
  //## end ClCommWorker::SetID%967794008.body
}

m4int_t ClCommWorker::GetID ()
{
  //## begin ClCommWorker::GetID%967794009.body preserve=yes
	return m_iWorkerID;
  //## end ClCommWorker::GetID%967794009.body
}

// Additional Declarations
  //## begin ClCommWorker%38DA016D0136.declarations preserve=yes
void ClCommWorker::AddedWorker()
{
	m_oActiveWorkerMutex.Lock();
	m_iNActivesWorker++;
	m_oActiveWorkerMutex.Unlock();
}

void ClCommWorker::DelWorker()
{
	m_oActiveWorkerMutex.Lock();
	m_iNActivesWorker--;
//	cerr << "Somos "<<m_iNActivesWorker<<"  \r"<<flush;
	m_oActiveWorkerMutex.Unlock();
}

m4int_t ClCommWorker::GetWorkerNumber()
{
	return m_iNActivesWorker;
}
  //## end ClCommWorker%38DA016D0136.declarations

//## begin module%38DB639D0141.epilog preserve=yes
//## end module%38DB639D0141.epilog
