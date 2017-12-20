//## begin module%3588F16B03CD.cm preserve=no
//## end module%3588F16B03CD.cm

//## begin module%3588F16B03CD.cp preserve=no
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
//## end module%3588F16B03CD.cp

//## Module: StatsJSEXECUTE%3588F16B03CD; Package body
//## Subsystem: M4JSExeJob::src%37F8BA6602AA
//## Source file: e:\m4server\m4jsexejob\src\ClStatsJSEXECUTE.cpp

//## begin module%3588F16B03CD.additionalIncludes preserve=no
//## end module%3588F16B03CD.additionalIncludes

//## begin module%3588F16B03CD.includes preserve=yes
#include <subsystemids.hpp>
#include <m4log.hpp>
#include <timeinterval.hpp>
//## end module%3588F16B03CD.includes

// LauncherInterface
#include <launcher.hpp>
// syncro
#include <syncro.hpp>
// StatsJSEXECUTE
#include <clstatsjsexecute.hpp>
//## begin module%3588F16B03CD.declarations preserve=no
//## end module%3588F16B03CD.declarations

//## begin module%3588F16B03CD.additionalDeclarations preserve=yes
#define MAX_SIZE_STRING  100
//## end module%3588F16B03CD.additionalDeclarations


// Class ClStatsJSEXECUTE 

















ClStatsJSEXECUTE::ClStatsJSEXECUTE ()
  //## begin ClStatsJSEXECUTE::ClStatsJSEXECUTE%898160025.hasinit preserve=no
  //## end ClStatsJSEXECUTE::ClStatsJSEXECUTE%898160025.hasinit
  //## begin ClStatsJSEXECUTE::ClStatsJSEXECUTE%898160025.initialization preserve=yes
  :ClLauncherStats()
  //## end ClStatsJSEXECUTE::ClStatsJSEXECUTE%898160025.initialization
{
  //## begin ClStatsJSEXECUTE::ClStatsJSEXECUTE%898160025.body preserve=yes

	m_uiNumberOfStats = TOTAL_NUM ;
	m_numjobsSuccess=0;
	m_numjobsError=0;
	m_numjobgroupsComplete=0;
	m_dDateEndExecution=0;
	m_dDateStartExecution=0;
	m_uiIdJob=0;
	m_uiIdBox=0;
	m_pcUserJob=NULL;
	m_uiJobsInterrupted=0;
	m_dTimeInExecution=0;
	m_dTimeDelay=0;
	m_uiDelayCurrentJob=0;
	m_uiLastTimeJob=0;
	m_uiIdBoxExec=0;
	m_poMutex.Init();
	
	

  //## end ClStatsJSEXECUTE::ClStatsJSEXECUTE%898160025.body
}


ClStatsJSEXECUTE::~ClStatsJSEXECUTE ()
{
  //## begin ClStatsJSEXECUTE::~ClStatsJSEXECUTE%898160026.body preserve=yes

	m_numjobsSuccess=0;
	m_numjobsError=0;
	m_numjobgroupsComplete=0;
	m_dDateEndExecution=0;
	m_dDateStartExecution=0;
	m_uiIdJob=0;
	m_uiIdBox=0;
	if (m_pcUserJob) {
		delete[] m_pcUserJob;
		m_pcUserJob=NULL;
	}
	m_uiJobsInterrupted=0;
	m_dTimeInExecution=0;
	m_dTimeDelay=0;
	m_uiDelayCurrentJob=0;
	m_uiIdBoxExec=0;
	m_uiLastTimeJob=0;
	

  //## end ClStatsJSEXECUTE::~ClStatsJSEXECUTE%898160026.body
}



//## Other Operations (implementation)
m4uint32_t ClStatsJSEXECUTE::GetNumberJobsSuccess ()
{
  //## begin ClStatsJSEXECUTE::GetNumberJobsSuccess%898160028.body preserve=yes

	m4uint32_t aux;

	//Tenemos que bloquear el mutex para que no 
	//puedan modificar valores mientras obtenemos los valores de las
	//estadísticas
	m_poMutex.Lock();

	aux=m_numjobsSuccess;

	//Desbloqueamos el mutex
	m_poMutex.Unlock();

	return aux;

  //## end ClStatsJSEXECUTE::GetNumberJobsSuccess%898160028.body
}

m4uint32_t ClStatsJSEXECUTE::GetNumberJobsError ()
{
  //## begin ClStatsJSEXECUTE::GetNumberJobsError%898160029.body preserve=yes

	m4uint32_t aux;

	//Tenemos que bloquear el mutex para que no 
	//puedan modificar valores mientras obtenemos los valores de las
	//estadísticas
	m_poMutex.Lock();

	aux=m_numjobsError;

	//Desbloqueamos el mutex
	m_poMutex.Unlock();

	return aux;

  //## end ClStatsJSEXECUTE::GetNumberJobsError%898160029.body
}

m4return_t ClStatsJSEXECUTE::SetNumberJobsSuccess (m4uint32_t ai_numjobsSuccess)
{
  //## begin ClStatsJSEXECUTE::SetNumberJobsSuccess%898160036.body preserve=yes

	//Tenemos que bloquear los mutex para que no puedan
	//obtener valores mientras los modificamos
	m_poMutex.Lock();

	m_numjobsSuccess=ai_numjobsSuccess;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::SetNumberJobsSuccess%898160036.body
}

m4return_t ClStatsJSEXECUTE::SetNumberJobsError (m4uint32_t ai_numjobsError)
{
  //## begin ClStatsJSEXECUTE::SetNumberJobsError%898160037.body preserve=yes

	m_poMutex.Lock();

	m_numjobsError=ai_numjobsError;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::SetNumberJobsError%898160037.body
}

m4return_t ClStatsJSEXECUTE::SetNumberJobGroupsComplete (m4uint32_t ai_numjobgroupsComplete)
{
  //## begin ClStatsJSEXECUTE::SetNumberJobGroupsComplete%898160039.body preserve=yes

	m_poMutex.Lock();

	m_numjobgroupsComplete=ai_numjobgroupsComplete;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::SetNumberJobGroupsComplete%898160039.body
}

m4return_t ClStatsJSEXECUTE::Reset ()
{
  //## begin ClStatsJSEXECUTE::Reset%898160043.body preserve=yes

	m_poMutex.Lock();

	m_numjobsSuccess=0;
	m_numjobsError=0;
	m_numjobgroupsComplete=0;
	m_dDateEndExecution=0;
	m_dDateStartExecution=0;
	m_uiIdJob=0;
	m_uiIdBox=0;
	if (m_pcUserJob) {
		delete[] m_pcUserJob;
		m_pcUserJob=NULL;
	}
	m_uiJobsInterrupted=0;
	m_dTimeInExecution=0;
	m_dTimeDelay=0;
	m_uiDelayCurrentJob=0;
	m_uiLastTimeJob=0;
	

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::Reset%898160043.body
}

m4return_t  ClStatsJSEXECUTE::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData)
{
  //## begin ClStatsJSEXECUTE::GetStat%898513641.body preserve=yes

	m4return_t res=M4_SUCCESS;

	//Chequeamos si el handle del subsistema 
	//que nos pasan coincide con el handle del subsistema
	//que recibe las estadísticas
	if (M4_ERROR == _CheckStatID(ai_hStatId,M4_EXECUTOR_JOB_EXECUTE_SUBSYSTEM))
		return M4_ERROR;

	//Obtenemos el identificador de la estadística 
	//que queremos
	m4uint32_t lowIndex=ai_hStatId.GetLowID();

	if (ClLauncherStats::TOTAL_NUM>lowIndex)
		return ClLauncherStats::GetStat(ai_hStatId,ao_poData);

	if (ao_poData.GetSize()>0)
		return M4_ERROR;

	ao_poData.SetSize(MAX_SIZE_STRING);

	m4pchar_t buffer=ao_poData.GetBuffer();

	eJSExecuteStats statsIndex=(eJSExecuteStats) lowIndex;

	//Copiamos en le buffer del Data Unit el valor 
	//que corresponda a la estadística que queremos

	M4ClTimeStamp DateConvert;
	M4ClString StringDateExecutor;
	ClTimeInterval interval(0);

	M4ClTimeStamp dateStart;
	M4ClString strDateStart;
	M4ClTimeStamp dateEnd;
	M4ClString strDateEnd;

	m_poMutex.Lock();

	switch (statsIndex)
	{
		case NUM_JOBS_TOTAL:
			sprintf(buffer, "%d",m_numjobsError+m_numjobsSuccess+m_uiJobsInterrupted);
			break;
		case NUM_JOBS_ERROR:
			sprintf(buffer ,"%d", m_numjobsError);
			break;
		case NUM_JOBS_SUCCESS:
			sprintf(buffer ,"%d", m_numjobsSuccess);	
			break;
		case NUM_JOB_GROUPS_COMPLETE:
			sprintf(buffer, "%d" ,m_numjobgroupsComplete);
			break;
		case NUM_BOX_INTERRUPTED:
			sprintf(buffer, "%d" ,m_uiJobsInterrupted);
		break;
		case M4_JS_ID_EXECUTION:
			if (m_uiIdBox)
				sprintf(buffer,"Group<%d>  Job<%d>  Exec<%d>",m_uiIdBox,m_uiIdJob,m_uiIdBoxExec);
			else
				strcpy(buffer,"-");
			break;
		case M4_USER_JOB:
			if (m_pcUserJob != NULL)
				sprintf(buffer,"%s",m_pcUserJob);
			else
				strcpy(buffer,"-");
			break;
		case M4_LAST_TIME_JOB:
			if (m_uiLastTimeJob) {
				interval.Set(m_uiLastTimeJob);
				interval.Get(StringDateExecutor);
				strcpy(buffer, StringDateExecutor.c_str());
			}
			else
				strcpy(buffer, "-");
			break;	
		case M4_TOTAL_DELAY_IN_JOBS:
			sprintf(buffer,"%d",m_dTimeDelay);
			break;
		case M4_TOTAL_TIME_IN_EXECUTION:
			sprintf(buffer,"%d",m_dTimeInExecution);
			break;
		case M4_DATE_START_EXECUTION:
			if (!m_dDateStartExecution)
			{
				strcpy(buffer, "-");
			}
			else
			{
				dateStart.set(m_dDateStartExecution);
				dateStart.get(strDateStart);
				sprintf(buffer, "%s", strDateStart.c_str());
			}
			break;
		case M4_DATE_END_EXECUTION:
			if (!m_dDateEndExecution)
			{
				strcpy(buffer, "-");
			}
			else
			{
				dateEnd.set(m_dDateEndExecution);
				dateEnd.get(strDateEnd);
				sprintf(buffer ,"%s", strDateEnd.c_str());
			}
			break;
		default:
			sprintf(buffer, "%010d", 0);
			res = M4_FALSE;
	}

	m_poMutex.Unlock();

	return res;
			

  //## end ClStatsJSEXECUTE::GetStat%898513641.body
}

m4return_t ClStatsJSEXECUTE::ResetStat (ClStatHandle &ai_hStatId)
{
  //## begin ClStatsJSEXECUTE::ResetStat%898513642.body preserve=yes

		m4return_t res=M4_SUCCESS;

	if (M4_ERROR == _CheckStatID(ai_hStatId,M4_EXECUTOR_JOB_EXECUTE_SUBSYSTEM))
		return M4_ERROR;

	//Obtenemos el identificador de la estadística 
	//que queremos resetear
	m4uint32_t lowIndex=ai_hStatId.GetLowID();

	if (ClLauncherStats::TOTAL_NUM>lowIndex)
		return ClLauncherStats::ResetStat(ai_hStatId);

	eJSExecuteStats statsIndex=(eJSExecuteStats) lowIndex;

	
	m_poMutex.Lock();

	switch (statsIndex)
	{
		case NUM_JOBS_ERROR:
			m_numjobsError=0;
			break;
		case NUM_JOBS_SUCCESS:
			m_numjobsSuccess=0;	
			break;
		case NUM_JOB_GROUPS_COMPLETE:
			m_numjobgroupsComplete=0;
			break;
		case NUM_BOX_INTERRUPTED:
			m_uiJobsInterrupted=0;
			break;
		case M4_JS_ID_EXECUTION:
			m_uiIdBox=0;
			m_uiIdJob=0;
			m_uiIdBoxExec=0;
			break;
		case M4_USER_JOB:
			if (m_pcUserJob) {
				delete[] m_pcUserJob;
				m_pcUserJob=NULL;
			}
			break;
		case  M4_TOTAL_DELAY_IN_JOBS:
			m_dTimeDelay=0;
			break;
		case M4_TOTAL_TIME_IN_EXECUTION:
			m_dTimeInExecution=0;
			break;
		case M4_LAST_TIME_JOB:
			m_uiLastTimeJob=0;
			break;
		case M4_DATE_START_EXECUTION:
			m_dDateStartExecution = 0;
			break;
		case M4_DATE_END_EXECUTION:
			m_dDateEndExecution = 0;
			break;
		default:
			res=M4_ERROR;
	}

	m_poMutex.Unlock();

	return res;
	


  //## end ClStatsJSEXECUTE::ResetStat%898513642.body
}

m4uint32_t ClStatsJSEXECUTE::GetIdJob ()
{
  //## begin ClStatsJSEXECUTE::GetIdJob%936603572.body preserve=yes


		m4uint32_t aux;

	m_poMutex.Lock();

	aux=m_uiIdJob;

	m_poMutex.Unlock();

	return aux;

  //## end ClStatsJSEXECUTE::GetIdJob%936603572.body
}

m4return_t ClStatsJSEXECUTE::SetIdJob (m4uint32_t ai_uiIdJob)
{
  //## begin ClStatsJSEXECUTE::SetIdJob%936603573.body preserve=yes

	m_poMutex.Lock();

	m_uiIdJob=ai_uiIdJob;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::SetIdJob%936603573.body
}

m4uint32_t ClStatsJSEXECUTE::GetIdBox ()
{
  //## begin ClStatsJSEXECUTE::GetIdBox%936603574.body preserve=yes

		m4uint32_t aux;

	m_poMutex.Lock();

	aux=m_uiIdBox;

	m_poMutex.Unlock();

	return aux;



  //## end ClStatsJSEXECUTE::GetIdBox%936603574.body
}

m4return_t ClStatsJSEXECUTE::SetIdBox (m4uint32_t ai_uiIdBox)
{
  //## begin ClStatsJSEXECUTE::SetIdBox%936603575.body preserve=yes

	m_poMutex.Lock();

	m_uiIdBox=ai_uiIdBox;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::SetIdBox%936603575.body
}

m4return_t ClStatsJSEXECUTE::SetUser (m4pchar_t ai_pcUser)
{
  //## begin ClStatsJSEXECUTE::SetUser%936603576.body preserve=yes

	if (ai_pcUser == NULL)
		return M4_ERROR;

	m4return_t ret = M4_SUCCESS;

	m4uint32_t size=strlen(ai_pcUser);

	m_poMutex.Lock();

	//Nos aseguramos de que el buffer
	//está a nulo en caso contrario
	//borramos la memoria 

	if (m_pcUserJob !=NULL)
		delete[] m_pcUserJob;

	m_pcUserJob=new m4char_t [size+1];

	if (m_pcUserJob != NULL)
		strcpy(m_pcUserJob,ai_pcUser);
	else
		ret = M4_ERROR;

	m_poMutex.Unlock();

	return ret;

  //## end ClStatsJSEXECUTE::SetUser%936603576.body
}

m4return_t ClStatsJSEXECUTE::GetUser (M4ClString &a_sUser)
{
  //## begin ClStatsJSEXECUTE::GetUser%936603577.body preserve=yes

	m4return_t ret = M4_SUCCESS;
	m_poMutex.Lock();

	if (m_pcUserJob != NULL)
		a_sUser=m_pcUserJob;
	else
		ret = M4_ERROR;

	m_poMutex.Unlock();

	return ret;

  //## end ClStatsJSEXECUTE::GetUser%936603577.body
}

m4uint32_t ClStatsJSEXECUTE::GetJobsInterrupted ()
{
  //## begin ClStatsJSEXECUTE::GetJobsInterrupted%936603578.body preserve=yes

	m4uint32_t aux;

	m_poMutex.Lock();

	aux=m_uiJobsInterrupted;

	m_poMutex.Unlock();

	return aux;

  //## end ClStatsJSEXECUTE::GetJobsInterrupted%936603578.body
}

m4return_t ClStatsJSEXECUTE::SetJobsInterrupted (m4uint32_t ai_uiJobsInterrupted)
{
  //## begin ClStatsJSEXECUTE::SetJobsInterrupted%936603579.body preserve=yes

	m_poMutex.Lock();

	m_uiJobsInterrupted=ai_uiJobsInterrupted;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::SetJobsInterrupted%936603579.body
}

m4double_t ClStatsJSEXECUTE::GetTimeInExecution ()
{
  //## begin ClStatsJSEXECUTE::GetTimeInExecution%936603580.body preserve=yes

	m4uint32_t aux;

	m_poMutex.Lock();

	aux=m_dTimeInExecution;

	m_poMutex.Unlock();

	return aux;

  //## end ClStatsJSEXECUTE::GetTimeInExecution%936603580.body
}

m4double_t ClStatsJSEXECUTE::GetTimeDelay ()
{
  //## begin ClStatsJSEXECUTE::GetTimeDelay%936603582.body preserve=yes

	m4uint32_t aux;

	m_poMutex.Lock();

	aux=m_dTimeDelay;

	m_poMutex.Unlock();

	return aux;

  //## end ClStatsJSEXECUTE::GetTimeDelay%936603582.body
}

m4return_t ClStatsJSEXECUTE::SetTimeDelay (m4double_t ai_dDateScheduled)
{
  //## begin ClStatsJSEXECUTE::SetTimeDelay%936603583.body preserve=yes

	m_poMutex.Lock();

	m4double_t SecondsDateStart=m_dDateStartExecution*24*60*60;

	m4double_t SecondsDateSched=ai_dDateScheduled*24*60*60;

	m4uint32_t TimeDelay=(m4uint32_t)(SecondsDateStart-SecondsDateSched);

	m_uiDelayCurrentJob=TimeDelay;

	m_poMutex.Unlock();

	return M4_SUCCESS;



  //## end ClStatsJSEXECUTE::SetTimeDelay%936603583.body
}

m4return_t ClStatsJSEXECUTE::DesactiveExecution ()
{
  //## begin ClStatsJSEXECUTE::DesactiveExecution%936621011.body preserve=yes

	m_poMutex.Lock();

	m_uiIdJob=0;

	m_uiIdBox=0;

	if (m_pcUserJob) {
		delete[] m_pcUserJob;
		m_pcUserJob=NULL;
	}

	m_uiIdBoxExec=0;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::DesactiveExecution%936621011.body
}

m4return_t ClStatsJSEXECUTE::SetLastTimeJob ()
{
  //## begin ClStatsJSEXECUTE::SetLastTimeJob%936621012.body preserve=yes

	m_poMutex.Lock();

	m4double_t SecondDateEnd= m_dDateEndExecution *24*60*60;

	m4double_t SecondDateStart=m_dDateStartExecution*24*60*60;

	m4uint32_t LastTimeJob=(m4uint32_t)(SecondDateEnd-SecondDateStart);

	m_uiLastTimeJob=LastTimeJob;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::SetLastTimeJob%936621012.body
}

m4uint32_t ClStatsJSEXECUTE::GetLastTimeJob ()
{
  //## begin ClStatsJSEXECUTE::GetLastTimeJob%936621013.body preserve=yes

	m4uint32_t aux;

	m_poMutex.Lock();

	aux=m_uiLastTimeJob;

	m_poMutex.Unlock();

	return aux;

  //## end ClStatsJSEXECUTE::GetLastTimeJob%936621013.body
}

m4uint32_t ClStatsJSEXECUTE::GetNumberJobGroupsComplete ()
{
  //## begin ClStatsJSEXECUTE::GetNumberJobGroupsComplete%898160031.body preserve=yes

	m4uint32_t aux;

	//Tenemos que bloquear el mutex para que no 
	//puedan modificar valores mientras  obtenemos los valores de las
	//estadísticas
	m_poMutex.Lock();

	aux=m_numjobgroupsComplete;

	//Desbloqueamos el mutex
	m_poMutex.Unlock();

	return aux;

  //## end ClStatsJSEXECUTE::GetNumberJobGroupsComplete%898160031.body
}

m4return_t ClStatsJSEXECUTE::AddJobsError (m4uint32_t ai_uiNumJobsError)
{
  //## begin ClStatsJSEXECUTE::AddJobsError%936621014.body preserve=yes

	m_poMutex.Lock();

	m_numjobsError+=ai_uiNumJobsError;

	m_dTimeDelay+=m_uiDelayCurrentJob;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::AddJobsError%936621014.body
}

m4return_t ClStatsJSEXECUTE::AddJobsSuccess (m4uint32_t ai_uiNumJobsSuccess)
{
  //## begin ClStatsJSEXECUTE::AddJobsSuccess%936621015.body preserve=yes

	m_poMutex.Lock();

	m_dTimeDelay+=m_uiDelayCurrentJob;

	m_numjobsSuccess+=ai_uiNumJobsSuccess;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::AddJobsSuccess%936621015.body
}

m4return_t ClStatsJSEXECUTE::AddJobsGroupCompleted (m4uint32_t ai_uiJobGroupsCompleted)
{
  //## begin ClStatsJSEXECUTE::AddJobsGroupCompleted%936621016.body preserve=yes

	m_poMutex.Lock();

	m_numjobgroupsComplete+=ai_uiJobGroupsCompleted;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::AddJobsGroupCompleted%936621016.body
}

m4return_t ClStatsJSEXECUTE::DesactiveLastExecution ()
{
  //## begin ClStatsJSEXECUTE::DesactiveLastExecution%936691798.body preserve=yes

	m_poMutex.Lock();

	m_dDateStartExecution=0;

	m_dDateEndExecution=0;

	m_uiLastTimeJob=0;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::DesactiveLastExecution%936691798.body
}

m4return_t ClStatsJSEXECUTE::ActiveIdentifiersJob (m4uint32_t ai_uiIdBox, m4uint32_t ai_uiIdJob, m4uint32_t ai_uiIdBoxExec)
{
  //## begin ClStatsJSEXECUTE::ActiveIdentifiersJob%936691799.body preserve=yes

	m_poMutex.Lock();

	m_uiIdBox=ai_uiIdBox;

	m_uiIdJob=ai_uiIdJob;

	m_uiIdBoxExec=ai_uiIdBoxExec;

	m_poMutex.Unlock();

	return M4_SUCCESS;


  //## end ClStatsJSEXECUTE::ActiveIdentifiersJob%936691799.body
}

m4return_t ClStatsJSEXECUTE::AddBoxInterrupted (m4uint32_t ai_uiBoxInterrupted)
{
  //## begin ClStatsJSEXECUTE::AddBoxInterrupted%936691800.body preserve=yes

	m_poMutex.Lock();

	m_uiJobsInterrupted+=ai_uiBoxInterrupted;

	m_poMutex.Unlock();

	return M4_SUCCESS;


  //## end ClStatsJSEXECUTE::AddBoxInterrupted%936691800.body
}

m4return_t ClStatsJSEXECUTE::SetTimeInExecution ()
{
  //## begin ClStatsJSEXECUTE::SetTimeInExecution%936691801.body preserve=yes

	m_poMutex.Lock();

	m4double_t SecondDateEnd=m_dDateEndExecution*24*60*60;

	m4double_t SecondDateStart=m_dDateStartExecution *24*60*60;

	m4uint32_t TimeInExecution=(m4uint32_t)(SecondDateEnd-SecondDateStart);

	m_dTimeInExecution+=TimeInExecution;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::SetTimeInExecution%936691801.body
}

m4return_t ClStatsJSEXECUTE::DateToHourSecondMinutes (m4uint32_t ai_uiTime, m4uint32_t &ao_uiHour, m4uint32_t &ao_uiMinutes, m4uint32_t &ao_uiSeconds)
{
  //## begin ClStatsJSEXECUTE::DateToHourSecondMinutes%936691802.body preserve=yes

	m4uint32_t minutes;

	minutes=ai_uiTime/60;

	ao_uiSeconds=ai_uiTime%60;

	ao_uiHour=minutes/60;

	ao_uiMinutes=minutes%60;

	return M4_SUCCESS;



  //## end ClStatsJSEXECUTE::DateToHourSecondMinutes%936691802.body
}

m4uint32_t ClStatsJSEXECUTE::GetIdBoxExec ()
{
  //## begin ClStatsJSEXECUTE::GetIdBoxExec%938599737.body preserve=yes

	m4uint32_t aux;

	m_poMutex.Lock();

	aux=m_uiIdBoxExec;

	m_poMutex.Unlock();

	return aux;

  //## end ClStatsJSEXECUTE::GetIdBoxExec%938599737.body
}

m4return_t ClStatsJSEXECUTE::SetIdBoxExec (m4uint32_t ai_uiIdBoxExec)
{
  //## begin ClStatsJSEXECUTE::SetIdBoxExec%938599738.body preserve=yes

	m_poMutex.Lock();

	m_uiIdBoxExec=ai_uiIdBoxExec;

	m_poMutex.Unlock();

	return M4_SUCCESS;

  //## end ClStatsJSEXECUTE::SetIdBoxExec%938599738.body
}

m4return_t ClStatsJSEXECUTE::SetDateEndExecution (m4double_t ai_dDateEndExecution)
{
  //## begin ClStatsJSEXECUTE::SetDateEndExecution%942339040.body preserve=yes
	m_poMutex.Lock();

	m_dDateEndExecution=ai_dDateEndExecution;

	m_poMutex.Unlock();

	return M4_SUCCESS;
  //## end ClStatsJSEXECUTE::SetDateEndExecution%942339040.body
}

m4return_t ClStatsJSEXECUTE::SetDateStartExecution (m4double_t ai_dDateStartExecution)
{
  //## begin ClStatsJSEXECUTE::SetDateStartExecution%942339041.body preserve=yes
	m_poMutex.Lock();

	m_dDateStartExecution=ai_dDateStartExecution;

	m_poMutex.Unlock();

	return M4_SUCCESS;
  //## end ClStatsJSEXECUTE::SetDateStartExecution%942339041.body
}

// Additional Declarations
  //## begin ClStatsJSEXECUTE%3588EEFC010B.declarations preserve=yes
  //## end ClStatsJSEXECUTE%3588EEFC010B.declarations

//## begin module%3588F16B03CD.epilog preserve=yes
//## end module%3588F16B03CD.epilog
