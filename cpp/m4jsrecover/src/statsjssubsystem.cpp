//## begin module%382BF54800C2.cm preserve=no
//## end module%382BF54800C2.cm

//## begin module%382BF54800C2.cp preserve=no
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
//## end module%382BF54800C2.cp

//## Module: statsjssubsystem%382BF54800C2; Package body
//## Subsystem: M4JSRecover::src%37F8B6A102CB
//## Source file: D:\FuentesServidor\m4jsrecover\src\statsjssubsystem.cpp

//## begin module%382BF54800C2.additionalIncludes preserve=no
//## end module%382BF54800C2.additionalIncludes

//## begin module%382BF54800C2.includes preserve=yes
#include <subsystemids.hpp>
//## end module%382BF54800C2.includes

// Statistics
#include <statistics.hpp>
// syncro
#include <syncro.hpp>
// statsjssubsystem
#include <statsjssubsystem.hpp>
//## begin module%382BF54800C2.declarations preserve=no
//## end module%382BF54800C2.declarations

//## begin module%382BF54800C2.additionalDeclarations preserve=yes
#define MAX_JS_BUFFER	255
//## end module%382BF54800C2.additionalDeclarations


// Class ClStatsJSSubsystem 




ClStatsJSSubsystem::ClStatsJSSubsystem ()
  //## begin ClStatsJSSubsystem::ClStatsJSSubsystem%938709938.hasinit preserve=no
      : m_uiNJobsExpired(0), m_uiNResExpired(0)
  //## end ClStatsJSSubsystem::ClStatsJSSubsystem%938709938.hasinit
  //## begin ClStatsJSSubsystem::ClStatsJSSubsystem%938709938.initialization preserve=yes
  //## end ClStatsJSSubsystem::ClStatsJSSubsystem%938709938.initialization
{
  //## begin ClStatsJSSubsystem::ClStatsJSSubsystem%938709938.body preserve=yes
	m_uiNumberOfStats = ClStatsJSSubsystem::TOTAL_NUM ;
	m_oMutex.Init();
  //## end ClStatsJSSubsystem::ClStatsJSSubsystem%938709938.body
}


ClStatsJSSubsystem::~ClStatsJSSubsystem ()
{
  //## begin ClStatsJSSubsystem::~ClStatsJSSubsystem%938709939.body preserve=yes
  //## end ClStatsJSSubsystem::~ClStatsJSSubsystem%938709939.body
}



//## Other Operations (implementation)
m4return_t ClStatsJSSubsystem::Reset ()
{
  //## begin ClStatsJSSubsystem::Reset%938709940.body preserve=yes
	return M4_SUCCESS;
  //## end ClStatsJSSubsystem::Reset%938709940.body
}

m4return_t ClStatsJSSubsystem::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData)
{
  //## begin ClStatsJSSubsystem::GetStat%938709941.body preserve=yes

	//Chequeamos si el handle del subsistema 
	//que nos pasan coincide con el handle del subsistema
	//que recibe las estadísticas
	if (M4_ERROR == _CheckStatID(ai_hStatId,M4_JOB_SCHEDULER_SUBSYSTEM))
		return M4_ERROR;

	//Obtenemos el identificador de la estadística 
	//que queremos
	m4uint32_t lowIndex=ai_hStatId.GetLowID();

	if (ao_poData.GetSize()>0)
		return M4_ERROR;

	ao_poData.SetSize(MAX_JS_BUFFER);

	m4pchar_t buffer=ao_poData.GetBuffer();


	eStatsJSSubsystem statsIndex=(eStatsJSSubsystem) lowIndex;

	//Copiamos en le buffer del Data Unit el valor 
	//que corresponda a la estadística que queremos
	m4return_t ret = M4_SUCCESS;
	m_oMutex.Lock();

	switch (statsIndex)
	{
		case JS_NJOBS_EXPIRED:
			sprintf(buffer, "%d", m_uiNJobsExpired);
			break;
		case JS_NRESOURCES_EXPIRED:
			sprintf(buffer, "%d", m_uiNResExpired);			
			break;
		default:
			ret = M4_ERROR;
			break;
	};

	m_oMutex.Unlock();

	return ret;
  //## end ClStatsJSSubsystem::GetStat%938709941.body
}

m4return_t ClStatsJSSubsystem::ResetStat (ClStatHandle &ai_hStatId)
{
  //## begin ClStatsJSSubsystem::ResetStat%938709942.body preserve=yes
	//Chequeamos si el handle del subsistema 
	//que nos pasan coincide con el handle del subsistema
	//que recibe las estadísticas
	if (M4_ERROR == _CheckStatID(ai_hStatId,M4_JOB_SCHEDULER_SUBSYSTEM))
		return M4_ERROR;

	//Obtenemos el identificador de la estadística 
	//que queremos
	m4uint32_t lowIndex=ai_hStatId.GetLowID();

	m4return_t ret = M4_SUCCESS;
	m_oMutex.Lock();

	switch (lowIndex)
	{
		case JS_NJOBS_EXPIRED:
			m_uiNJobsExpired=0;
			break;
		case JS_NRESOURCES_EXPIRED:
			m_uiNResExpired=0;			
			break;
		default:
			ret = M4_ERROR;
			break;
	};

	m_oMutex.Unlock();

	return ret;
  //## end ClStatsJSSubsystem::ResetStat%938709942.body
}

m4return_t ClStatsJSSubsystem::AddJobsExpired (m4uint32_t ai_uiJobs)
{
  //## begin ClStatsJSSubsystem::AddJobsExpired%942405296.body preserve=yes
	m_oMutex.Lock();
	m_uiNJobsExpired+=ai_uiJobs;
	m_oMutex.Unlock();
	return M4_SUCCESS;
  //## end ClStatsJSSubsystem::AddJobsExpired%942405296.body
}

m4return_t ClStatsJSSubsystem::AddResExpired (m4uint32_t ai_uiJobs)
{
  //## begin ClStatsJSSubsystem::AddResExpired%942405297.body preserve=yes
	m_oMutex.Lock();
	m_uiNResExpired+=ai_uiJobs;
	m_oMutex.Unlock();
	return M4_SUCCESS;
  //## end ClStatsJSSubsystem::AddResExpired%942405297.body
}

// Additional Declarations
  //## begin ClStatsJSSubsystem%37F391A80160.declarations preserve=yes
  //## end ClStatsJSSubsystem%37F391A80160.declarations

//## begin module%382BF54800C2.epilog preserve=yes
//## end module%382BF54800C2.epilog
