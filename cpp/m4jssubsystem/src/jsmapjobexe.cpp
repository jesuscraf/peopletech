//## begin module%37FA29A402EE.cm preserve=no
//## end module%37FA29A402EE.cm

//## begin module%37FA29A402EE.cp preserve=no
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
//## end module%37FA29A402EE.cp

//## Module: jsmapjobexe%37FA29A402EE; Package body
//## Subsystem: M4JSSubsystem::src%37BD2F0A02AE
//## Source file: D:\FuentesServidor\m4jssubsystem\version\src\jsmapjobexe.cpp

//## begin module%37FA29A402EE.additionalIncludes preserve=no
//## end module%37FA29A402EE.additionalIncludes

//## begin module%37FA29A402EE.includes preserve=yes
#include <executive.hpp> //forward en hpp
#include <exeforjobschecexecute.hpp>//body only
#include <basiclog.hpp>
#include <m4jsres.hpp>
//## end module%37FA29A402EE.includes

// jsmapjobexe
#include <jsmapjobexe.hpp>
// jsjob
#include <jsjob.hpp>


//## begin module%37FA29A402EE.declarations preserve=no
//## end module%37FA29A402EE.declarations

//## begin module%37FA29A402EE.additionalDeclarations preserve=yes
#include <m4trace.hpp>
//## end module%37FA29A402EE.additionalDeclarations


// Class ClJSMapJobExe 






ClJSMapJobExe::ClJSMapJobExe ()
  //## begin ClJSMapJobExe::ClJSMapJobExe%939122286.hasinit preserve=no
      : m_uiNumJobsExecuting(0), m_uiNumExecutors(0)
  //## end ClJSMapJobExe::ClJSMapJobExe%939122286.hasinit
  //## begin ClJSMapJobExe::ClJSMapJobExe%939122286.initialization preserve=yes
  //## end ClJSMapJobExe::ClJSMapJobExe%939122286.initialization
{
  //## begin ClJSMapJobExe::ClJSMapJobExe%939122286.body preserve=yes
  //## end ClJSMapJobExe::ClJSMapJobExe%939122286.body
}



//## Other Operations (implementation)
m4return_t ClJSMapJobExe::UpdateExecutor (ClExecutive *ai_pExecutive, ClJSJob *ai_pJSJob)
{
  //## begin ClJSMapJobExe::UpdateExecutor%939122280.body preserve=yes

	/*Esta funcion devuelve error cuando no se encuentra el Executive
	El puntero al Job puede ser NULL y su significado es que el Ejecutor
	a finalizado la ejecución del trabajo.
	Si el puntero tiene un valor, significa que ha iniciado la ejecución de ese trabajo*/

	m4return_t ret = M4_ERROR;

	ClJSJob *RootJob;
	structJSJobInfo *infoTemp;
	jsmapJob_t::iterator theJobIterator;

	jsmapJobExecutor_t::iterator theIterator;
	theIterator = m_oMapJobExe.find(ai_pExecutive);

    if(theIterator != m_oMapJobExe.end()) {
		(*theIterator).second = ai_pJSJob;
		ret = M4_SUCCESS;
		if (ai_pJSJob != NULL) {
			RootJob = ai_pJSJob->GetRoot();
			theJobIterator = m_oMapJobs.find(RootJob);
			if (theJobIterator != m_oMapJobs.end()) {
				infoTemp = (*theJobIterator).second;
				infoTemp->m_uiStatus = structJSJobInfo::JOB_EXECUTING;
			}
		}
		else
			m_uiNumJobsExecuting--;
	}

	return ret;
  //## end ClJSMapJobExe::UpdateExecutor%939122280.body
}

m4return_t ClJSMapJobExe::AddExecutor (ClExecutive *ai_pExecutive)
{
  //## begin ClJSMapJobExe::AddExecutor%939122281.body preserve=yes
	if (ai_pExecutive == NULL)
		return M4_ERROR;

	pair<jsmapJobExecutor_t::iterator, bool >		mapRes;	
	jsmapJobExecutor_t::value_type		mapEntry(ai_pExecutive , (ClJSJob *)NULL);
	mapRes = m_oMapJobExe.insert(mapEntry);
	if (FALSE == mapRes.second) {
		BL_ERRORF(M4_JSADM_MAP , "AddExecutor");
		return M4_ERROR;	
	}
	else {
		m_uiNumExecutors++;
		return M4_SUCCESS; 
	}
  //## end ClJSMapJobExe::AddExecutor%939122281.body
}

m4return_t ClJSMapJobExe::DeleteExecutor (ClExecutive *ai_pExecutive)
{
  //## begin ClJSMapJobExe::DeleteExecutor%939122282.body preserve=yes

	//Esta funcion devuelve error cuando no se encuentra el Executive

	m4return_t ret = M4_ERROR;
	ClJSJob *jobTemp, *RootJob;
	structJSJobInfo *infoTemp;
	jsmapJob_t::iterator theJobIterator;
	
	jsmapJobExecutor_t::iterator theIterator;
	theIterator = m_oMapJobExe.find(ai_pExecutive);

    if(theIterator != m_oMapJobExe.end()) {
		//Antes de borrar comprobamos que no esta ejecutando ningun job
		//ejemplo si se ha producido un GP.
		if ((*theIterator).second != NULL) {
			jobTemp = (*theIterator).second;
			RootJob = jobTemp->GetRoot();
			//Busco el trabajo en la lista de trabajos, para cambiarle el estatus
			theJobIterator = m_oMapJobs.find(RootJob);
			if (theJobIterator != m_oMapJobs.end()) {
				RootJob->SetInterruptionRequestLevel(ClJSJob::IRL_INSS);
				infoTemp = (*theJobIterator).second;
				infoTemp->m_uiStatus = structJSJobInfo::JOB_CANCELED;
				m_uiNumJobsExecuting--;
				m_uiNumExecutors--;
				/*Esto solo puede ocurrir en caso de GP, luego en este caso
				decrementamos el nº de ejecutores.
				Ya que si en este momento se ha solicitado una petición de borrado
				de ejecutor y se produce esto, durante un tiempo puede ocurrir una 
				inconsitencia, de que aparezca n ejecutores libres en lugar de n-1*/
			}
		}
		m_oMapJobExe.erase(theIterator);
		ret = M4_SUCCESS;
	}
	if (ret == M4_ERROR) 
		BL_ERRORF(M4_JSADM_MAP , "DeleteExecutor");
	else {
		/*unca el size debe ser menor a m_uiNumExecutors
		Esto puede ocurrir en caso de un GP de un ejecutor, 
		luego en este punto arreglamos la situación.*/
		if (m_uiNumExecutors > m_oMapJobExe.size())
			m_uiNumExecutors = m_oMapJobExe.size();
	}
	return ret;
  //## end ClJSMapJobExe::DeleteExecutor%939122282.body
}

m4uint32_t ClJSMapJobExe::GetFreeExecutors ()
{
  //## begin ClJSMapJobExe::GetFreeExecutors%939122287.body preserve=yes
	return (m_uiNumExecutors - m_uiNumJobsExecuting);
  //## end ClJSMapJobExe::GetFreeExecutors%939122287.body
}

m4uint32_t ClJSMapJobExe::GetUsedExecutors ()
{
  //## begin ClJSMapJobExe::GetUsedExecutors%939213537.body preserve=yes
	return m_uiNumJobsExecuting;
  //## end ClJSMapJobExe::GetUsedExecutors%939213537.body
}

m4uint32_t ClJSMapJobExe::GetNumExecutors ()
{
  //## begin ClJSMapJobExe::GetNumExecutors%941108272.body preserve=yes
	return m_uiNumExecutors;
  //## end ClJSMapJobExe::GetNumExecutors%941108272.body
}

m4return_t ClJSMapJobExe::AddJob (ClJSJob *ai_pJob)
{
  //## begin ClJSMapJobExe::AddJob%939805990.body preserve=yes
	if (ai_pJob == NULL)
		return M4_ERROR;
	structJSJobInfo *infoTemp = new structJSJobInfo();
	pair<jsmapJob_t::iterator, bool >		mapRes;	
	jsmapJob_t::value_type		mapEntry(ai_pJob , infoTemp);
	mapRes = m_oMapJobs.insert(mapEntry);
	if (FALSE == mapRes.second) {
		BL_ERRORF(M4_JSADM_MAP , "AddJob");
		return M4_ERROR;	
	}
	else {
		return M4_SUCCESS; 
	}
	return M4_SUCCESS;
  //## end ClJSMapJobExe::AddJob%939805990.body
}

m4return_t ClJSMapJobExe::DeleteJob (ClJSJob *ai_pJob)
{
  //## begin ClJSMapJobExe::DeleteJob%939805991.body preserve=yes
	m4return_t ret = M4_ERROR;
	structJSJobInfo *infoTemp;
	jsmapJob_t::iterator theIterator;
	theIterator = m_oMapJobs.find(ai_pJob);
	ClJSJob *theJob;
    if(theIterator != m_oMapJobs.end()) {
		//Obtengo el puntero de la informacion para borrarlo
		infoTemp = (*theIterator).second;
		if (infoTemp) {
			if (infoTemp->m_uiStatus == structJSJobInfo::JOB_SEND) {
				/*Esto solo pasa en el caso de mandar un UNMARK del job,
				por lo que no debemos borrarlo fisicamente, ya que de ello 
				se encarga el recuperador/cancelador*/
				m_uiNumJobsExecuting--; 
			}
			// Bug 0286430: Borrar solo en el caso de no estar en ejecución.
			// En este caso se manda el job como parámetro de la request M4JS_REQ_UNMARK_JOB
			// y se borra allí.
			else if (infoTemp->m_uiStatus != structJSJobInfo::JOB_EXECUTING)
			{
				//Ya que se ha ejecutado se borra fisicamente.
				theJob = (*theIterator).first;
				if (theJob)
					delete theJob;
			}
			delete infoTemp;
		}
		m_oMapJobs.erase(theIterator);
		ret = M4_SUCCESS;
	}
	if (ret == M4_ERROR) 
		BL_ERRORF(M4_JSADM_MAP , "DeleteJob");
	return ret;
  //## end ClJSMapJobExe::DeleteJob%939805991.body
}

m4return_t ClJSMapJobExe::GetPendingJob (ClJSJob *&ao_pJob)
{
  //## begin ClJSMapJobExe::GetPendingJob%939805992.body preserve=yes
	m4return_t ret;
	ret = GetCanceledJob(ao_pJob);
	if ( ret == M4_WARNING)
		//Nos devuelve warning porque no hay ninguno de tipo de cancelación.
		ret = GetWaitingJob(ao_pJob);
	return ret;
  //## end ClJSMapJobExe::GetPendingJob%939805992.body
}

m4return_t ClJSMapJobExe::GetCanceledJob (ClJSJob *&ao_pJob)
{
  //## begin ClJSMapJobExe::GetCanceledJob%939912564.body preserve=yes
	if (GetFreeExecutors()==0)
		return M4_ERROR;
	return GetJob(structJSJobInfo::JOB_CANCELED, ao_pJob);
  //## end ClJSMapJobExe::GetCanceledJob%939912564.body
}

m4return_t ClJSMapJobExe::GetWaitingJob (ClJSJob *&ao_pJob)
{
  //## begin ClJSMapJobExe::GetWaitingJob%939912566.body preserve=yes
	if (GetFreeExecutors()==0)
		return M4_ERROR;
	return GetJob(structJSJobInfo::JOB_WAITING, ao_pJob);
  //## end ClJSMapJobExe::GetWaitingJob%939912566.body
}

m4return_t ClJSMapJobExe::GetJob (structJSJobInfo::eJobStatus ai_uiJobStatus, ClJSJob *&ao_pJob)
{
  //## begin ClJSMapJobExe::GetJob%939912565.body preserve=yes
	m4return_t ret = M4_WARNING;
	ao_pJob = NULL;
	structJSJobInfo *infoTemp;
	jsmapJob_t::iterator theIterator;
	theIterator = m_oMapJobs.begin();
    while (theIterator != m_oMapJobs.end() && ret == M4_WARNING) {
		//Obtengo el puntero de la informacion para saber si esta pendiente
		infoTemp = (*theIterator).second;
		if (infoTemp->m_uiStatus == ai_uiJobStatus) {
			infoTemp->m_uiStatus = structJSJobInfo::JOB_SEND;
			ao_pJob = (*theIterator).first;
			m_uiNumJobsExecuting++;
			ret = M4_SUCCESS;
		}
		else
			theIterator++;
	}
	return ret;
  //## end ClJSMapJobExe::GetJob%939912565.body
}

m4return_t ClJSMapJobExe::InterruptJob (const ClJSJob::eInterruptRequestLevel &ai_uiInterruptType, ClJSJob *ai_pJob)
{
  //## begin ClJSMapJobExe::InterruptJob%939912567.body preserve=yes
	m4return_t ret = M4_ERROR;
	ClJSJob *jobTemp, *RootJob;
	m4uint32_t uiIdTemp;
	ClExecutive *executorTemp;
	ClAdminTagList tagInTemp, tagOutTemp; //para la cancelación de jobs
						
	jsmapJobExecutor_t::iterator theIterator;
	theIterator = m_oMapJobExe.begin();

	//Buscamos por todos los ejecutores por si hay varias tareas
	//del mismo job ejecutándose.
    while (theIterator != m_oMapJobExe.end()) {
		if ((*theIterator).second != NULL) {
			//Esta ejecutando algo
			jobTemp = (*theIterator).second;
			RootJob = jobTemp->GetRoot();
			if (*RootJob == *ai_pJob) {
				RootJob->SetInterruptionRequestLevel(ai_uiInterruptType);
				if (ai_uiInterruptType==ClJSJob::IRL_INSU || ai_uiInterruptType==ClJSJob::IRL_INSS) {
					//Interrupcion no controlada, intentamos poner el flag en MVC
					executorTemp = (*theIterator).first;
					executorTemp->ExecuteCommand(ClExeForJobSchecEXECUTE::CANCEL, tagInTemp, tagOutTemp);
					BL_BEGIN(WARNINGLOG, M4_JSADM_INTJOB ); 
					ai_pJob->GetId(uiIdTemp);
					BL_OBJ << ai_uiInterruptType << uiIdTemp;
					BL_END;
					ret = M4_SUCCESS;
				}
			}

		}
		theIterator++;
	}
	return ret;
  //## end ClJSMapJobExe::InterruptJob%939912567.body
}

ClJSJob * ClJSMapJobExe::GetNextTask (ClJSJob *ai_pJob)
{
  //## begin ClJSMapJobExe::GetNextTask%940322788.body preserve=yes
	ClJSJob *NextTask = NULL;
	if (ai_pJob != NULL) {
		//Solicitamos al Job el siguiente task que hay que ejecutar
		NextTask = ai_pJob->GetNextExecutableTask();

		if (NextTask != NULL) {
			m_uiNumJobsExecuting++;
		}
	}
	return NextTask;
  //## end ClJSMapJobExe::GetNextTask%940322788.body
}

m4return_t ClJSMapJobExe::InterruptAllJobs (const ClJSJob::eInterruptRequestLevel &ai_uiInterruptType)
{
  //## begin ClJSMapJobExe::InterruptAllJobs%940521658.body preserve=yes
	m4return_t ret = M4_ERROR;
	ClJSJob *jobTemp, *RootJob;
	m4uint32_t uiIdTemp;
	ClExecutive *executorTemp;
	ClAdminTagList tagInTemp, tagOutTemp; //para la cancelación de jobs
						
	jsmapJobExecutor_t::iterator theIterator;
	theIterator = m_oMapJobExe.begin();

    while (theIterator != m_oMapJobExe.end()) {
		if ((*theIterator).second != NULL) {
			//Esta ejecutando algo
			jobTemp = (*theIterator).second;
			RootJob = jobTemp->GetRoot();
			RootJob->SetInterruptionRequestLevel(ai_uiInterruptType);
			if (ai_uiInterruptType==ClJSJob::IRL_INSU || ai_uiInterruptType==ClJSJob::IRL_INSS) {
				//Interrupcion no controlada, intentamos poner el flag en MVC
				executorTemp = (*theIterator).first;
				executorTemp->ExecuteCommand(ClExeForJobSchecEXECUTE::CANCEL, tagInTemp, tagOutTemp);
				BL_BEGIN(WARNINGLOG, M4_JSADM_INTJOB ); 
				jobTemp->GetId(uiIdTemp);
				BL_OBJ << ai_uiInterruptType << uiIdTemp;
				BL_END;
				ret = M4_SUCCESS;
			}
		}
		theIterator++;
	}
	return ret;
  //## end ClJSMapJobExe::InterruptAllJobs%940521658.body
}

m4return_t ClJSMapJobExe::ReqDelExecutor ()
{
  //## begin ClJSMapJobExe::ReqDelExecutor%940853233.body preserve=yes
	m4return_t ret = M4_ERROR;
	if ( (GetFreeExecutors() > 0) && (m_uiNumExecutors > 0) ) {
		m_uiNumExecutors--;
		ret = M4_SUCCESS;
	}
	return ret;
  //## end ClJSMapJobExe::ReqDelExecutor%940853233.body
}

m4return_t ClJSMapJobExe::DeleteWaitingJobs ()
{
  //## begin ClJSMapJobExe::DeleteWaitingJobs%941043668.body preserve=yes
	m4return_t ret = M4_ERROR;
	structJSJobInfo *infoTemp;
	jsmapJob_t::iterator theIterator = m_oMapJobs.begin();
    while (theIterator != m_oMapJobs.end()) {
		//Obtengo el puntero de la informacion para ver estado y borrarlo
		infoTemp = (*theIterator).second;
		if (infoTemp) {
			if (infoTemp->m_uiStatus == structJSJobInfo::JOB_WAITING) {
				delete infoTemp;
				m_oMapJobs.erase(theIterator);
				theIterator = m_oMapJobs.begin();
				ret = M4_SUCCESS;
			}
			else
				theIterator++;
		}
		else
			theIterator++;
		
	}
	if (ret == M4_ERROR) 
		BL_WARNINGF(M4_JSADM_MAP , "DeleteWaitingJobs");
	return ret;
  //## end ClJSMapJobExe::DeleteWaitingJobs%941043668.body
}

// Additional Declarations
  //## begin ClJSMapJobExe%37FA1AA601AF.declarations preserve=yes
  //## end ClJSMapJobExe%37FA1AA601AF.declarations

// Class structJSJobInfo 




structJSJobInfo::structJSJobInfo ()
  //## begin structJSJobInfo::structJSJobInfo%939898164.hasinit preserve=no
      : m_uiPriority(0), m_uiActiveTask(0), m_uiStatus(JOB_WAITING)
  //## end structJSJobInfo::structJSJobInfo%939898164.hasinit
  //## begin structJSJobInfo::structJSJobInfo%939898164.initialization preserve=yes
  //## end structJSJobInfo::structJSJobInfo%939898164.initialization
{
  //## begin structJSJobInfo::structJSJobInfo%939898164.body preserve=yes
  //## end structJSJobInfo::structJSJobInfo%939898164.body
}


// Additional Declarations
  //## begin structJSJobInfo%37FC716100EA.declarations preserve=yes
  //## end structJSJobInfo%37FC716100EA.declarations

//## begin module%37FA29A402EE.epilog preserve=yes
//## end module%37FA29A402EE.epilog
