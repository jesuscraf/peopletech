//## begin module%37E6019501EB.cm preserve=no
//## end module%37E6019501EB.cm

//## begin module%37E6019501EB.cp preserve=no
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
//## end module%37E6019501EB.cp

//## Module: jscompoundjob%37E6019501EB; Package body
//## Subsystem: M4JSExeJob::m4jsjob::src%37E244D003E3
//## Source file: C:\Trabajo\Server\m4jsexejob\version\src\jscompoundjob.cpp

//## begin module%37E6019501EB.additionalIncludes preserve=no
//## end module%37E6019501EB.additionalIncludes

//## begin module%37E6019501EB.includes preserve=yes
//## end module%37E6019501EB.includes

// jscompoundjob
#include <jscompoundjob.hpp>
// jsjob
#include <jsjob.hpp>
//## begin module%37E6019501EB.declarations preserve=no
//## end module%37E6019501EB.declarations

//## begin module%37E6019501EB.additionalDeclarations preserve=yes
//## end module%37E6019501EB.additionalDeclarations


// Class ClJSCompoundJob 


ClJSCompoundJob::ClJSCompoundJob (const m4uint_t &ai_uiId, const m4uint_t &ai_uiIdExecution)
  //## begin ClJSCompoundJob::ClJSCompoundJob%937821612.hasinit preserve=no
  //## end ClJSCompoundJob::ClJSCompoundJob%937821612.hasinit
  //## begin ClJSCompoundJob::ClJSCompoundJob%937821612.initialization preserve=yes
  : ClJSJob()
  //## end ClJSCompoundJob::ClJSCompoundJob%937821612.initialization
{
  //## begin ClJSCompoundJob::ClJSCompoundJob%937821612.body preserve=yes
	m_uiId = 0;
	m_uiIdExecution = ai_uiIdExecution;
	m_uiIdSchedJob = ai_uiId;
	m_uiLocalId = 0;

	m_poExecutionStrategy = ClJSExecutionStrategy::GetStrategyType( ClJSExecutionStrategy::InitialCompoundStrategy );
  //## end ClJSCompoundJob::ClJSCompoundJob%937821612.body
}


ClJSCompoundJob::~ClJSCompoundJob ()
{
  //## begin ClJSCompoundJob::~ClJSCompoundJob%938600373.body preserve=yes
	TaskList::iterator iterador  ;/* Iterador para usar el mapa                 */
	ClJSJob * pJob               ;/* Puntero para los elementos del mapa        */
	m4int_t numTasks             ;/* Número de tareas en el trabajo             */
	register short i             ;/* Contador para bucles                       */
	
	numTasks = m_mTasks.size();
	if ( numTasks > 0)
	{
		iterador = m_mTasks.begin();
		for (i = 0; i < numTasks; i++)
		{
			pJob = (*iterador).second;
			(*iterador).second = NULL;
			iterador ++;
			delete pJob;
		}

		m_mTasks.clear();
	}
  //## end ClJSCompoundJob::~ClJSCompoundJob%938600373.body
}



//## Other Operations (implementation)
m4return_t ClJSCompoundJob::AddTask (ClJSJob *ai_poTask, const m4uint_t &ai_uiLocalId)
{
  //## begin ClJSCompoundJob::AddTask%933264300.body preserve=yes
	m4return_t res = M4_SUCCESS  ;/* Resultado del método                       */
	m4uint32_t idLocal           ;/* Copia del id local, por si hay que pedirlo */
	TaskList::iterator iterador  ;/* Iterador para usar el mapa                 */
	ClJSJob * pJobAnterior       ;/* Puntero para comprobar si ya se había      */
	                             ;/* otra tarea con el mismo Id                 */

	if (ai_poTask)
	{
		idLocal = ai_uiLocalId;
		if (idLocal == 0)
			ai_poTask->GetLocalId(idLocal);

		/* Comprobamos que no se hubiera metido otro con el mismo Id local     */
		iterador = m_mTasks.find( idLocal);
		if (iterador != m_mTasks.end() )
		{
			pJobAnterior = (*iterador).second;
			if (pJobAnterior != NULL)
				delete pJobAnterior;
		}

		ai_poTask->SetLocalId(idLocal);
		ai_poTask->SetParent( this);
		m_mTasks[ idLocal ] = ai_poTask;
	}
	else
		res = M4_ERROR;

   return res;
  //## end ClJSCompoundJob::AddTask%933264300.body
}

m4uint32_t ClJSCompoundJob::GetNumberOfTasks ()
{
  //## begin ClJSCompoundJob::GetNumberOfTasks%938517343.body preserve=yes
	return m_mTasks.size();
  //## end ClJSCompoundJob::GetNumberOfTasks%938517343.body
}

void ClJSCompoundJob::PrintOn (ostream &ai_ostream)
{
  //## begin ClJSCompoundJob::PrintOn%938424583.body preserve=yes
	register short i;
	m4int_t depthLevel = 1;
	m4uint32_t idJob = 0;
	m4uint32_t idExecution = 0;
	m4uint32_t idDefinition = 0;
	m4uint32_t idLocal; 
	M4ClTimeStamp executionDate;
	m4int32_t ano, mes, dia, hora, minutos, segundos;


	if ( ai_ostream.opfx() )
	{
		depthLevel = CalculateDepthLevel( depthLevel);
		GetExecutionId( idExecution);
		GetId( idDefinition);

		if (m_poParent != NULL) 
		{
			GetLocalId( idLocal);
			for (i = 0; i < (depthLevel * 3); i++) ai_ostream << " ";
			ai_ostream << idLocal << ".- " << idDefinition << endl;

		}
		else
		{
			ai_ostream << "Trabajo: " << idDefinition << " - Ejecucion: " << idExecution << endl;
			for (i = 0; i < (depthLevel * 3); i++) ai_ostream << " ";

			GetScheduledTime( executionDate);
			executionDate.get( ano, mes, dia, hora, minutos, segundos);
			ai_ostream << "Fecha de ejecucion: " << dia << "-" << mes << "-" << ano << " " << hora << ":" << minutos << ":" << segundos << endl;
		}
		
		ai_ostream.osfx();
		PrintAllParametersOn( ai_ostream);
		PrintAllTasksOn( ai_ostream);
	}
  //## end ClJSCompoundJob::PrintOn%938424583.body
}

void ClJSCompoundJob::PrintAllTasksOn (ostream &aio_ostream)
{
  //## begin ClJSCompoundJob::PrintAllTasksOn%938517344.body preserve=yes
	m4int32_t uiNumTasks;
	TaskList::iterator iterador  ;/* Iterador para usar el mapa                 */
	register short i;

	if ( aio_ostream.opfx() )
	{
		uiNumTasks = GetNumberOfTasks();

		if (uiNumTasks > 0)
		{
			iterador = m_mTasks.begin();

			for (i = 0; i < uiNumTasks; i++)
			{
				aio_ostream << (*(* iterador).second);
				iterador ++;
			}
		}

		aio_ostream.osfx();
	}
  //## end ClJSCompoundJob::PrintAllTasksOn%938517344.body
}

m4return_t ClJSCompoundJob::SetStatus (const eJobStatus &ai_eStatus)
{
  //## begin ClJSCompoundJob::SetStatus%938600375.body preserve=yes
	ClJSJob::SetStatus( ai_eStatus);

	if (ai_eStatus == ClJSJob::StatusExecuting )
		m_poExecutionStrategy = ClJSExecutionStrategy::GetStrategyType( ClJSExecutionStrategy::FinalCompoundStrategy);

	return M4_SUCCESS;
  //## end ClJSCompoundJob::SetStatus%938600375.body
}

ClJSJob * ClJSCompoundJob::GetNextExecutableTask ()
{
  //## begin ClJSCompoundJob::GetNextExecutableTask%938600380.body preserve=yes
	ClJSJob * pJobRes = NULL       ;/* Resultado del método                   */
	ClJSJob * pJobAuxiliar         ;/* Puntero aux. para comprobar terminadas */
	TaskList::iterator iterador    ;/* Iterador para usar el mapa             */
	m4uint_t numTerminadas = 0     ;/* Número de tareas terminadas            */
	eJobStatus statusAuxiliar      ;/* Estado de cada subtarea                */
	eInterruptRequestLevel IntLevel;/* Nivel de interrupción solicitado       */

	/* Lo primero es comprobar si el trabajo ya ha finalizado                 */
	if ( IsFinished() == M4_FALSE)
	{
		/* Luego miramos a ver si se ha pedido cualquier nivel de interrupción */
		GetInterruptionRequestLevel( IntLevel);
		if (IntLevel == ClJSJob::IRLNone)
		{
			/* Si el trabajo está en espera, lo primero que hay que ejecutar es */
			/* su propia inicialización                                         */  
			if (m_eStatus == ClJSJob::StatusWaiting)
				pJobRes = this;
			else
			{
				/* En otro caso, hay que recorrer la lista de tareas, a ver si   */
				/* si alguna es ejecutable                                       */
				if (!m_mTasks.empty())
				{
					iterador = m_mTasks.begin();
					while ( (iterador != m_mTasks.end()) && (pJobRes == NULL))
					{
						pJobAuxiliar = (* iterador).second;
						pJobRes = pJobAuxiliar->GetNextExecutableTask();
						pJobAuxiliar->GetStatus( statusAuxiliar);
						numTerminadas += ( statusAuxiliar == ClJSJob::StatusExecuted ||
							statusAuxiliar == ClJSJob::StatusExecutionError );
						iterador ++;
					}
				}
			}

			/* Si hemos llegado aquí sin encontrar nada, puede ser que todas    */
			/* las tareas estén terminadas, con lo que lo que hay que ejecutar  */
			/* es el proceso de finalización propio                             */
			if ((pJobRes == NULL) && (numTerminadas == m_mTasks.size()))
					pJobRes = this;
		}
		else
		{
			pJobRes = this;
		}
	}

	return pJobRes;
  //## end ClJSCompoundJob::GetNextExecutableTask%938600380.body
}

ClJSJob * ClJSCompoundJob::GetTaskWithLocalId (const m4uint32_t &ai_uiLocalId)
{
  //## begin ClJSCompoundJob::GetTaskWithLocalId%939135440.body preserve=yes
	ClJSJob * res = NULL        ;/* Resultado del método                      */
	TaskList::iterator iterador ;/* Iterador para usar el mapa                */

	if ( m_uiLocalId == ai_uiLocalId)
		res = this;
	else
	{
		iterador = m_mTasks.begin();
		while ( res == NULL && iterador != m_mTasks.end())
		{
			res = (*((* iterador).second)).GetTaskWithLocalId( ai_uiLocalId);
			iterador ++;
		}
	}

	return res;
  //## end ClJSCompoundJob::GetTaskWithLocalId%939135440.body
}

m4return_t ClJSCompoundJob::PropagateAllParamValues ()
{
  //## begin ClJSCompoundJob::PropagateAllParamValues%941619586.body preserve=yes
	m4int32_t uiNumTasks         ;/* Número de trabajos dentro de este        */
	TaskList::iterator iterador  ;/* Iterador para usar el mapa               */
	register short i             ;/* Contador para el bucle                   */

	/* Primero propagamos los parámetros propios                              */
	ClJSJob::PropagateAllParamValues();

	/* Y luego, los de cada una de las tareas que lo componen                 */
	uiNumTasks = GetNumberOfTasks();

	if (uiNumTasks > 0)
	{
		iterador = m_mTasks.begin();

		for (i = 0; i < uiNumTasks; i++)
		{
			((* iterador).second)->PropagateAllParamValues();
			iterador ++;
		}
	}

	return M4_SUCCESS;
  //## end ClJSCompoundJob::PropagateAllParamValues%941619586.body
}

// Additional Declarations
  //## begin ClJSCompoundJob%375BE75101E2.declarations preserve=yes
  //## end ClJSCompoundJob%375BE75101E2.declarations

//## begin module%37E6019501EB.epilog preserve=yes
//## end module%37E6019501EB.epilog
