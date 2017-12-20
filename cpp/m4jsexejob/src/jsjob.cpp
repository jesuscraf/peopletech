//## begin module%37E244EF024D.cm preserve=no
//## end module%37E244EF024D.cm

//## begin module%37E244EF024D.cp preserve=no
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
//## end module%37E244EF024D.cp

//## Module: jsjob%37E244EF024D; Package body
//## Subsystem: M4JSExeJob::m4jsjob::src%37E244D003E3
//## Source file: C:\Trabajo\Server\m4jsexejob\src\jsjob.cpp

//## begin module%37E244EF024D.additionalIncludes preserve=no
//## end module%37E244EF024D.additionalIncludes

//## begin module%37E244EF024D.includes preserve=yes
#include <basiclog.hpp>
#include <m4jsres.hpp>
//## end module%37E244EF024D.includes

// m4date
#include <m4date.hpp>
// jsexecutionstrategy
#include <jsexecutionstrategy.hpp>
// jssessionsettings
#include <jssessionsettings.hpp>
// m4jsjob_dll
#include <jsparameter.hpp>
// jsjob
#include <jsjob.hpp>
//## begin module%37E244EF024D.declarations preserve=no
//## end module%37E244EF024D.declarations

//## begin module%37E244EF024D.additionalDeclarations preserve=yes
//## end module%37E244EF024D.additionalDeclarations


// Class ClJSJob 

















ClJSJob::ClJSJob ()
  //## begin ClJSJob::ClJSJob%938164381.hasinit preserve=no
  //## end ClJSJob::ClJSJob%938164381.hasinit
  //## begin ClJSJob::ClJSJob%938164381.initialization preserve=yes
  //## end ClJSJob::ClJSJob%938164381.initialization
{
  //## begin ClJSJob::ClJSJob%938164381.body preserve=yes
	m_uiLocalId = 0;
	m_eStatus = StatusWaiting;
	m_poParent = NULL;
	m_eInterruptionRequestLevel = IRLNone;
	m_bParameterIteratorIsValid = M4_FALSE;

  //## end ClJSJob::ClJSJob%938164381.body
}


ClJSJob::~ClJSJob ()
{
  //## begin ClJSJob::~ClJSJob%938600374.body preserve=yes
	ParameterList::iterator iterador ;/* Iterador para usar el mapa            */
	ClJSParameter * pParametro       ;/* Puntero para los elementos del mapa   */
	m4int_t numParametros            ;/* Número de parámetros en el trabajo    */
	
	numParametros = m_mParameters.size();
	if ( numParametros > 0)
	{
		iterador = m_mParameters.begin();

		while ( iterador != m_mParameters.end() )
		{
			pParametro = (*iterador).second;
			iterador ++;
			delete pParametro;
		}

		m_mParameters.clear();
	}
  //## end ClJSJob::~ClJSJob%938600374.body
}



//## Other Operations (implementation)
m4return_t ClJSJob::SetParent (ClJSJob *ai_poParent)
{
  //## begin ClJSJob::SetParent%938073480.body preserve=yes
	m_poParent = ai_poParent;
	return M4_SUCCESS;
  //## end ClJSJob::SetParent%938073480.body
}

ClJSJob * ClJSJob::GetParent ()
{
  //## begin ClJSJob::GetParent%938073481.body preserve=yes
	return m_poParent;
  //## end ClJSJob::GetParent%938073481.body
}

m4return_t ClJSJob::GetId (m4uint32_t &ao_uiId)
{
  //## begin ClJSJob::GetId%938164382.body preserve=yes
	ao_uiId = m_uiId;
	return M4_SUCCESS;
  //## end ClJSJob::GetId%938164382.body
}

m4return_t ClJSJob::SetId (const m4uint32_t &ai_uiId)
{
  //## begin ClJSJob::SetId%938164386.body preserve=yes
	m_uiId = ai_uiId;
	return M4_SUCCESS;
  //## end ClJSJob::SetId%938164386.body
}

m4return_t ClJSJob::SetExecutionId (const m4uint32_t &ai_uiIdExecution)
{
  //## begin ClJSJob::SetExecutionId%938164389.body preserve=yes
	m4return_t res ; /* Resultado del método: M4_SUCCESS si se asigna la fecha */
	                 /* a un trabajo principal, M4_ERROR si se intenta con una */ 
	                 /* tarea                                                  */

	if (m_poParent == NULL)
	{
		m_uiIdExecution = ai_uiIdExecution;
		res = M4_SUCCESS;
	}
	else
		res = M4_ERROR;

	return res;
  //## end ClJSJob::SetExecutionId%938164389.body
}

m4return_t ClJSJob::GetExecutionId (m4uint32_t &ao_uiIdExecution)
{
  //## begin ClJSJob::GetExecutionId%938164385.body preserve=yes
	if (m_poParent == NULL)
		ao_uiIdExecution = m_uiIdExecution;
	else
		m_poParent->GetExecutionId( ao_uiIdExecution);

	return M4_SUCCESS;
  //## end ClJSJob::GetExecutionId%938164385.body
}

m4return_t ClJSJob::SetLocalId (const m4uint32_t &ai_uiLocalId)
{
  //## begin ClJSJob::SetLocalId%938164388.body preserve=yes
	m_uiLocalId = ai_uiLocalId;
	return M4_SUCCESS;
  //## end ClJSJob::SetLocalId%938164388.body
}

m4return_t ClJSJob::GetLocalId (m4uint32_t &ao_uiLocalId)
{
  //## begin ClJSJob::GetLocalId%938164384.body preserve=yes
	ao_uiLocalId = m_uiLocalId;
	return M4_SUCCESS;
  //## end ClJSJob::GetLocalId%938164384.body
}

m4return_t ClJSJob::SetSchedJobId (const m4uint32_t &ai_uiIdSchedJob)
{
  //## begin ClJSJob::SetSchedJobId%938164387.body preserve=yes
	m4return_t res ; /* Resultado del método: M4_SUCCESS si se asigna la fecha */
	                 /* a un trabajo principal, M4_ERROR si se intenta con una */ 
	                 /* tarea                                                  */

	if (m_poParent == NULL)
	{
		m_uiIdSchedJob = ai_uiIdSchedJob;
		res = M4_SUCCESS;
	}
	else
		res = M4_ERROR;

	return res;
  //## end ClJSJob::SetSchedJobId%938164387.body
}

m4return_t ClJSJob::GetSchedJobId (m4uint32_t &ao_uiIdSchedJob)
{
  //## begin ClJSJob::GetSchedJobId%938164383.body preserve=yes
	if (m_poParent == NULL)
		ao_uiIdSchedJob = m_uiIdSchedJob;
	else
		m_poParent->GetSchedJobId( ao_uiIdSchedJob);

	return M4_SUCCESS;
  //## end ClJSJob::GetSchedJobId%938164383.body
}

m4return_t ClJSJob::SetScheduledTime (const M4ClTimeStamp &ai_dtScheduledTime)
{
  //## begin ClJSJob::SetScheduledTime%938073482.body preserve=yes
	m4return_t res ; /* Resultado del método: M4_SUCCESS si se asigna la fecha */
	                 /* a un trabajo principal, M4_ERROR si se intenta con una */ 
	                 /* tarea                                                  */

	if (m_poParent == NULL)
	{
		m_oScheduledTime = ai_dtScheduledTime;
		res = M4_SUCCESS;
	}
	else
		res = M4_ERROR;

	return res; 
  //## end ClJSJob::SetScheduledTime%938073482.body
}

m4return_t ClJSJob::GetScheduledTime (M4ClTimeStamp &ao_dtScheduledTime)
{
  //## begin ClJSJob::GetScheduledTime%938073483.body preserve=yes
	if (m_poParent == NULL)
	{
		/* Estoy en el trabajo padre, debo copiar mi fecha en la otra         */
		ao_dtScheduledTime = m_oScheduledTime;
	}
	else
	{
		m_poParent->GetScheduledTime( ao_dtScheduledTime);
	}

	return M4_SUCCESS;
  //## end ClJSJob::GetScheduledTime%938073483.body
}

m4int_t ClJSJob::CalculateDepthLevel (m4int_t &aio_uiDepthLevel)
{
  //## begin ClJSJob::CalculateDepthLevel%938424584.body preserve=yes
	if (m_poParent != NULL)
	{
		aio_uiDepthLevel += 1;
		m_poParent->CalculateDepthLevel ( aio_uiDepthLevel);
	}

	return aio_uiDepthLevel;
  //## end ClJSJob::CalculateDepthLevel%938424584.body
}

m4uint32_t ClJSJob::GetNumberOfTasks ()
{
  //## begin ClJSJob::GetNumberOfTasks%938517342.body preserve=yes
	return 0;
  //## end ClJSJob::GetNumberOfTasks%938517342.body
}

ostream& operator << (ostream &aio_ostream, ClJSJob &ai_oJob)
{
  //## begin ClJSJob::operator<<%938517345.body preserve=yes
	ai_oJob.PrintOn( aio_ostream);
	return aio_ostream;
  //## end ClJSJob::operator<<%938517345.body
}

m4return_t ClJSJob::SetStatus (const eJobStatus &ai_eStatus)
{
  //## begin ClJSJob::SetStatus%938600376.body preserve=yes
	m_eStatus = ai_eStatus;

	return M4_SUCCESS;
  //## end ClJSJob::SetStatus%938600376.body
}

m4return_t ClJSJob::GetStatus (eJobStatus &ao_eStatus)
{
  //## begin ClJSJob::GetStatus%938600377.body preserve=yes
	ao_eStatus = m_eStatus;

	return M4_SUCCESS;
  //## end ClJSJob::GetStatus%938600377.body
}

m4return_t ClJSJob::SetInterruptionRequestLevel (const eInterruptRequestLevel &ai_eIRLevel)
{
  //## begin ClJSJob::SetInterruptionRequestLevel%938600378.body preserve=yes
	if (m_poParent == NULL)
	{
		if ( m_eInterruptionRequestLevel == IRLNone )
			m_eInterruptionRequestLevel = ai_eIRLevel;
	}
	else
		m_poParent->SetInterruptionRequestLevel( ai_eIRLevel);

	return M4_SUCCESS;
  //## end ClJSJob::SetInterruptionRequestLevel%938600378.body
}

m4return_t ClJSJob::GetInterruptionRequestLevel (eInterruptRequestLevel &ao_eIRLevel)
{
  //## begin ClJSJob::GetInterruptionRequestLevel%938600379.body preserve=yes
	ao_eIRLevel = m_eInterruptionRequestLevel;

	return M4_SUCCESS;
  //## end ClJSJob::GetInterruptionRequestLevel%938600379.body
}

ClJSParameter * ClJSJob::GetParamWithName (const M4ClString &ai_stName)
{
  //## begin ClJSJob::GetParamWithName%939135438.body preserve=yes
	ClJSParameter * res = NULL       ;/* Resultado del método                 */
	ParameterList::iterator iterador ;/* Iterador para recorrer el mapa       */

	iterador = m_mParameters.find( ai_stName);

	if (iterador == m_mParameters.end())
		res = NULL;
	else
		res = (* iterador).second;

	return res;
  //## end ClJSJob::GetParamWithName%939135438.body
}

m4return_t ClJSJob::ExecuteOn ()
{
  //## begin ClJSJob::ExecuteOn%938766907.body preserve=yes
	m4return_t res = M4_ERROR   ;/* Resultado del método                      */
	if (m_poExecutionStrategy != NULL)
	{
		m_poExecutionStrategy->Execute( this);

		res = M4_SUCCESS;
	}

	return res;
  //## end ClJSJob::ExecuteOn%938766907.body
}

m4return_t ClJSJob::AddParameter (ClJSParameter *ai_pParameter)
{
  //## begin ClJSJob::AddParameter%938766922.body preserve=yes
	m4return_t res = M4_SUCCESS      ;/* Resultado del método                  */
	m4return_t parcial               ;/* Resultado parcial                     */
	M4ClString pStClave              ;/* Clave identif. del parámetro (nombre) */
	ClJSParameter * pParamAnterior   ;/* Puntero para comprobar si ya estaba   */
	ParameterList::iterator iterador ;/* Iterador para recorrer el mapa        */  
	
	if( ai_pParameter )
	{
		parcial = ai_pParameter->GetName( pStClave);
		if (parcial == M4_SUCCESS)
		{
			iterador = m_mParameters.find( pStClave);
			if (iterador != m_mParameters.end() )
			{
				pParamAnterior = (* iterador).second;
				(* iterador).second = NULL;
				delete pParamAnterior;
				m_mParameters.erase( iterador, iterador);
			}

			m_mParameters[ pStClave ] = ai_pParameter;
			m_bParameterIteratorIsValid = M4_FALSE;
		}
		else
			res = M4_ERROR;
	}
	else
		res = M4_ERROR;
	
	return res;
  //## end ClJSJob::AddParameter%938766922.body
}

m4return_t ClJSJob::AddParameterConnection (const m4uint32_t &ai_uiLocalId1, const M4ClString &ai_stParamName1, const m4uint32_t &ai_uiLocalId2, const M4ClString &ai_stParamName2)
{
  //## begin ClJSJob::AddParameterConnection%939135436.body preserve=yes
	ClJSJob * pTask1 = NULL;
	ClJSJob * pTask2 = NULL;
	ClJSParameter * pParam1 = NULL;
	ClJSParameter * pParam2 = NULL;
	ClJSParameterConnector * pConector;
	m4return_t res = M4_ERROR;

	pTask1 = GetTaskWithLocalId( ai_uiLocalId1);
	pTask2 = GetTaskWithLocalId( ai_uiLocalId2);

	if ( pTask1 != NULL && pTask2 != NULL )
	{
		pParam1 = pTask1->GetParamWithName( ai_stParamName1);
		pParam2 = pTask2->GetParamWithName( ai_stParamName2);

		if (pParam1 != NULL && pParam2 != NULL)
		{
			pConector = new ClJSParameterConnector( pParam1, pParam2);
			if (pConector != NULL)
			{
				pParam1->AddConnector( pConector);
				res = M4_SUCCESS;
			}
		}
	}

	return res;
  //## end ClJSJob::AddParameterConnection%939135436.body
}

void ClJSJob::PrintAllParametersOn (ostream &aio_ostream)
{
  //## begin ClJSJob::PrintAllParametersOn%939045077.body preserve=yes
	m4int32_t uiNumParametros         ;/* Número de parámetros en el trabajo   */
	m4int_t depthLevel = 0               ;/* Nivel de profundidad del trabajo     */ 
	ParameterList::iterator iterador  ;/* Iterador para usar el mapa           */
	register short i, j;

	if ( aio_ostream.opfx() )
	{
		uiNumParametros = m_mParameters.size();
		depthLevel = CalculateDepthLevel( depthLevel);

		if (uiNumParametros > 0)
		{
			iterador = m_mParameters.begin();

			for (i = 0; i < uiNumParametros; i++)
			{
				for (j = 0; j < ((depthLevel + 2) * 3); j++) aio_ostream << " ";

				aio_ostream << *((* iterador).second);
				iterador ++;
			}
		}

		aio_ostream.osfx();
	}
  //## end ClJSJob::PrintAllParametersOn%939045077.body
}

m4bool_t ClJSJob::IsRoot ()
{
  //## begin ClJSJob::IsRoot%939812332.body preserve=yes
	m4bool_t ret = M4_FALSE;
	if ( m_poParent == NULL)
		ret = M4_TRUE;
	return ret;
  //## end ClJSJob::IsRoot%939812332.body
}

ClJSJob * ClJSJob::GetRoot ()
{
  //## begin ClJSJob::GetRoot%939812334.body preserve=yes
	ClJSJob * res = NULL;

	if ( m_poParent == NULL)
		res = this;
	else
		res = m_poParent->GetRoot();

	return res;
  //## end ClJSJob::GetRoot%939812334.body
}

m4bool_t ClJSJob::IsFinished ()
{
  //## begin ClJSJob::IsFinished%939812333.body preserve=yes
	m4bool_t ret = M4_FALSE;
	if ( m_eStatus != ClJSJob::StatusWaiting &&
		 m_eStatus != ClJSJob::StatusExecuting)
		 ret = M4_TRUE;
	return ret;
  //## end ClJSJob::IsFinished%939812333.body
}

m4bool_t ClJSJob::operator == (const ClJSJob &ai_oJob)
{
  //## begin ClJSJob::operator ==%939980388.body preserve=yes
	m4bool_t ret = M4_FALSE;
	if (m_uiId == ai_oJob.m_uiId && m_uiIdSchedJob == ai_oJob.m_uiIdSchedJob
		&& m_uiIdExecution == ai_oJob.m_uiIdExecution && m_uiLocalId == ai_oJob.m_uiLocalId
		&& m_oScheduledTime == ai_oJob.m_oScheduledTime)
			ret = M4_TRUE;
	return ret;
  //## end ClJSJob::operator ==%939980388.body
}

m4return_t ClJSJob::AddTask (ClJSJob *ai_poTask, const m4uint_t &ai_uiLocalId)
{
  //## begin ClJSJob::AddTask%940243354.body preserve=yes
	return M4_ERROR;
  //## end ClJSJob::AddTask%940243354.body
}

ClJSParameter * ClJSJob::GetFirstInputParameter ()
{
  //## begin ClJSJob::GetFirstInputParameter%941561489.body preserve=yes
	ClJSParameter * pParameter ;/* Resultado del método                        */
  
	m_ParameterIterator = m_mParameters.begin();
	m_bParameterIteratorIsValid = M4_TRUE;
  
	pParameter = FindNextParameter( ClJSParameter::FlowInput);
  
	return pParameter;
  //## end ClJSJob::GetFirstInputParameter%941561489.body
}

ClJSParameter * ClJSJob::GetNextInputParameter ()
{
  //## begin ClJSJob::GetNextInputParameter%941561490.body preserve=yes
	ClJSParameter * pParameter = NULL ;/* Resultado del método                 */

	if ( m_bParameterIteratorIsValid == M4_TRUE)
		pParameter = FindNextParameter( ClJSParameter::FlowInput);
	else
	{
		BL_BEGIN( ERRORLOG, M4_JS_JOB_INVALID_ITERATOR);
		BL_OBJ << "ParameterIterator" << "ClJSJob::GetNextInputParameter";
		BL_END;
	}	
  
	return pParameter;
  //## end ClJSJob::GetNextInputParameter%941561490.body
}

ClJSParameter * ClJSJob::GetFirstOutputParameter ()
{
  //## begin ClJSJob::GetFirstOutputParameter%941561491.body preserve=yes
	ClJSParameter * pParameter ;/* Resultado del método                        */
  
	m_ParameterIterator = m_mParameters.begin();
	m_bParameterIteratorIsValid = M4_TRUE;
  
	pParameter = FindNextParameter( ClJSParameter::FlowOutput);
  
	return pParameter;
  //## end ClJSJob::GetFirstOutputParameter%941561491.body
}

ClJSParameter * ClJSJob::GetNextOutputParameter ()
{
  //## begin ClJSJob::GetNextOutputParameter%941561492.body preserve=yes
	ClJSParameter * pParameter = NULL ;/* Resultado del método                 */

	if ( m_bParameterIteratorIsValid == M4_TRUE)
		pParameter = FindNextParameter( ClJSParameter::FlowOutput);
	else
	{
		BL_BEGIN( ERRORLOG, M4_JS_JOB_INVALID_ITERATOR);
		BL_OBJ << "ParameterIterator" << "ClJSJob::GetNextOutputParameter";
		BL_END;
	}	
  
	return pParameter;
  //## end ClJSJob::GetNextOutputParameter%941561492.body
}

ClJSParameter * ClJSJob::FindNextParameter (ClJSParameter::eParameterFlowType ai_eType)
{
  //## begin ClJSJob::FindNextParameter%941619585.body preserve=yes
   ClJSParameter * pParameter = NULL            ;/* Resultado del método      */
   ClJSParameter::eParameterFlowType eTipoParm  ;/* Tipo de cada parámetro    */
   m4bool_t bSalir = M4_FALSE                   ;/* Control del bucle         */

	if ( m_bParameterIteratorIsValid == M4_TRUE)
	{
		while (bSalir == FALSE && m_ParameterIterator != m_mParameters.end() )
		{
			(( * m_ParameterIterator).second)->GetFlowType( eTipoParm);
			if ( eTipoParm == ai_eType)
			{
				pParameter = (* m_ParameterIterator).second;
			   bSalir = M4_TRUE;
			}
			m_ParameterIterator ++;
		}
	}	
  
	return pParameter;
  //## end ClJSJob::FindNextParameter%941619585.body
}

m4return_t ClJSJob::PropagateAllParamValues ()
{
  //## begin ClJSJob::PropagateAllParamValues%941619584.body preserve=yes
	m4int32_t uiNumParametros         ;/* Número de parámetros en el trabajo   */
	ParameterList::iterator iterador  ;/* Iterador para usar el mapa           */
	register short i;

	uiNumParametros = m_mParameters.size();

	if (uiNumParametros > 0)
	{
		iterador = m_mParameters.begin();

		for (i = 0; i < uiNumParametros; i++)
		{
			((* iterador).second)->PropagateValue();
			iterador ++;
		}
	}

	return M4_SUCCESS;
  //## end ClJSJob::PropagateAllParamValues%941619584.body
}

m4return_t ClJSJob::SetParamValue (const M4ClString &ai_stParamName, const M4ClString &ai_stParamValue)
{
  //## begin ClJSJob::SetParamValue%941737616.body preserve=yes
 	m4return_t res = M4_SUCCESS   ;/* Resultado del método                    */
 	ClJSParameter * pParameter    ;/* Puntero al parámetro solicitado         */
 	
 	/* Localizamos el parámetro                                               */
 	pParameter = GetParamWithName( ai_stParamName);
 	/* Si está ...                                                            */
 	if (pParameter != NULL)
 	{
		/* Le asignamos el valor                                               */
 		res = pParameter->SetValue( ai_stParamValue);
	}
	/* Y si no ...                                                            */
	else
	{
		/* Damos un error (hasta rima y todo)                                  */
		res = M4_ERROR;
		BL_BEGIN( ERRORLOG, M4_JS_JOB_PARAMETER_NOT_FOUND);
		BL_OBJ << ai_stParamName << m_uiId << "ClJSJob::SetParamValue";
		BL_END;
	}

	return res;
  //## end ClJSJob::SetParamValue%941737616.body
}

// Additional Declarations
  //## begin ClJSJob%375BE73302C5.declarations preserve=yes
  //## end ClJSJob%375BE73302C5.declarations

//## begin module%37E244EF024D.epilog preserve=yes
//## end module%37E244EF024D.epilog
