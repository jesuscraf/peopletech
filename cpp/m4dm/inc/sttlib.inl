//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                sttlib.inl   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                15-05-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//
//
//==============================================================================
#include "channel.hpp"
#include "sttstate.hpp"
#include "access.hpp"
#include "iexecutor.hpp"


m4return_t ClVMStatistics::PropagateTimes (void)
{
	m4return_t ret = M4_ERROR;
	ClSttState * pParentState;

	ret = m_pSttStack->Top(pParentState);
	
	if (ret == M4_ERROR) //No tiene padre
		return M4_ERROR;

	pParentState->m_oExecData.m_dTotalTime += m_pActiveState->m_oExecData.m_dTotalTime;
	pParentState->m_oExecData.m_dDBTime += m_pActiveState->m_oExecData.m_dDBTime;
	pParentState->m_oExecData.m_dComTime += m_pActiveState->m_oExecData.m_dComTime;
	pParentState->m_oExecData.m_dSttTime += m_pActiveState->m_oExecData.m_dSttTime;

	pParentState->m_oExecData.m_dLDBTime += m_pActiveState->m_oExecData.m_dLDBTime;

	pParentState->m_oExecData.m_dOutputPDUSize += m_pActiveState->m_oExecData.m_dOutputPDUSize;
	pParentState->m_oExecData.m_dInputPDUSize += m_pActiveState->m_oExecData.m_dInputPDUSize;

	if (1/*ENABLED CHILD*/)
	{
		//El padre se queda con el estado hijo (solo con el hijo)
		pParentState->AddChildList(m_pActiveState);
	}

	return M4_SUCCESS;
}

m4return_t ClVMStatistics::sttStateEnd (m4uint32_t ai_hItem)
{
	m4return_t ret = M4_SUCCESS;

	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{		
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttStateEnd"<<EndLog;
		return M4_ERROR;
	}

	if ((m_pActiveState->m_hItem != ai_hItem) && (ai_hItem != 0))
	{
		//Comentado de momento. Si pasamos el MCR... Puede valer.
		g_oChLog<<BeginWarning(M4_CH_DM_STT_STATE_NOT_MATCHED)<<m_pActiveState->m_hItem << ai_hItem<<EndLog;
		return M4_ERROR;
	}


	m_oSttClock.Start();

	if ( (m_iInit & M4_VM_STATISTICS_ENABLED_TIME) || (m_iInit & M4_VM_STATISTICS_ENABLED_HISTORY) ) 
	{
		if (m_iInit & M4_VM_STATISTICS_ENABLED_TIME)
		{
			m4double_t dRealTotalTime = m_pActiveState->m_ClockGlobal.GetDifference();

			m_pActiveState->m_oExecData.m_dDBTime += m_pActiveState->m_oExecData.m_dSelfDBTime;
			m_pActiveState->m_oExecData.m_dComTime += m_pActiveState->m_oExecData.m_dSelfComTime;
			m_pActiveState->m_oExecData.m_dSttTime += m_pActiveState->m_oExecData.m_dSelfSttTime;

			m_pActiveState->m_oExecData.m_dLDBTime += m_pActiveState->m_oExecData.m_dSelfLDBTime;

			m_pActiveState->m_oExecData.m_dOutputPDUSize += m_pActiveState->m_oExecData.m_dSelfOutputPDUSize;
			m_pActiveState->m_oExecData.m_dInputPDUSize += m_pActiveState->m_oExecData.m_dSelfInputPDUSize;

			// Restamos el tiempo de Stt
			dRealTotalTime -= m_pActiveState->m_oExecData.m_dSttTime;

			m_pActiveState->m_oExecData.m_dSelfTotalTime = dRealTotalTime - m_pActiveState->m_oExecData.m_dTotalTime;
			m_pActiveState->m_oExecData.m_dTotalTime = dRealTotalTime;

			if (M4_TRUE == m_pActiveState->IsRealMapped())
			{
				//Si el item es mapped, entonces todo su tiempo de ejecución propio lo asignamos a "comunicaciones",
				//esto significa, que es el tiempo asociado a la comunicación (no solo trasporte)
				m_pActiveState->m_oExecData.m_dComTime -= m_pActiveState->m_oExecData.m_dSelfComTime;
				m_pActiveState->m_oExecData.m_dSelfComTime = m_pActiveState->m_oExecData.m_dSelfTotalTime - m_pActiveState->m_oExecData.m_dSelfDBTime;
				m_pActiveState->m_oExecData.m_dComTime += m_pActiveState->m_oExecData.m_dSelfComTime;
			}

			if (m_pActiveState->m_oExecData.m_dSelfSttOutputPDUSize || m_pActiveState->m_oExecData.m_dSelfSttInputPDUSize)
			{
				//sacamos la proporcion del tiempo de comm y restamos el tamaño del overhead de stt
				m4double_t dRealComm, dSttComTime;
				m4double_t dRealSize, dTotalSize;

				dTotalSize = m_pActiveState->m_oExecData.m_dSelfOutputPDUSize + m_pActiveState->m_oExecData.m_dSelfInputPDUSize;
				dRealSize = dTotalSize - m_pActiveState->m_oExecData.m_dSelfSttOutputPDUSize - m_pActiveState->m_oExecData.m_dSelfSttInputPDUSize;

				if (dRealSize > 0)
				{
					//Tiempo real
					dRealComm = (dRealSize * m_pActiveState->m_oExecData.m_dSelfComTime) / dTotalSize;
					dSttComTime = m_pActiveState->m_oExecData.m_dSelfComTime - dRealComm;

					//Restamos el t a comms y total
					m_pActiveState->m_oExecData.m_dSelfComTime -= dSttComTime;
					m_pActiveState->m_oExecData.m_dComTime -= dSttComTime;
					m_pActiveState->m_oExecData.m_dSelfTotalTime -= dSttComTime;
					m_pActiveState->m_oExecData.m_dTotalTime -= dSttComTime;

					//Añadimos el tiempo a Stt
					m_pActiveState->m_oExecData.m_dSelfSttTime += dSttComTime;
					m_pActiveState->m_oExecData.m_dSttTime += dSttComTime;

					//Restamos el tam de stt
					m_pActiveState->m_oExecData.m_dSelfOutputPDUSize -= m_pActiveState->m_oExecData.m_dSelfSttOutputPDUSize;
					m_pActiveState->m_oExecData.m_dOutputPDUSize -= m_pActiveState->m_oExecData.m_dSelfSttOutputPDUSize;
					m_pActiveState->m_oExecData.m_dSelfInputPDUSize -= m_pActiveState->m_oExecData.m_dSelfSttInputPDUSize;
					m_pActiveState->m_oExecData.m_dInputPDUSize -= m_pActiveState->m_oExecData.m_dSelfSttInputPDUSize;
				}
				else
				{
					//Error

				}
			}
		}

		PropagateTimes ();

		if (m_pSttStack->IsEmpty())
		{
			if( m_pcIntermediateFile != NULL )
			{
				// Si está activado el estado de mandar a fichero no se manda al canal
				ret = _SendToIntermediateFile() ;
			}
			else
			{
				ret = sttAddActiveStateAll();
			}

			if (ret == M4_ERROR)
				return M4_ERROR;
		}
		else
		{
			//Para que no se destruya el estado
			m_pActiveState = NULL;
		}
	}

	if( m_pActiveState != NULL )
	{
		delete m_pActiveState;	
		m_pActiveState = NULL;
	}

	//Volvamos al estado anterior.
	if (!m_pSttStack->IsEmpty())
	{
		m_pSttStack->Top(m_pActiveState);
		m_pSttStack->Pop();

		//El tiempo de la insercion del hijo en el canal se suma como Self del padre
		m_pActiveState->m_oExecData.m_dSelfSttTime += m_oSttClock.GetDifference();

#ifdef _WINDOWS
		//Sumamos al tiempo stt el delay del propio crono: 2 cronos / 2 (la mitad se queda ya dentro del propio crono)
		m_pActiveState->m_oExecData.m_dSelfSttTime += ClSttClock::m_dClockSelfTime;
#endif

	}

	return ret;
}


m4return_t ClVMStatistics::sttAddValue( ClItem& ai_roItem, m4VariantType &ai_vValue, priority_t ai_iPriority, m4bool_t ai_bAssigned, m4uint8_t ai_iSource, const m4VMState_t& ai_roState, m4pcchar_t ai_pccCallStack )
{
	// De momento no se almacena la pila de llamadas
	ai_pccCallStack = "";

	if (!(m_iInit & M4_VM_STATISTICS_ENABLED_VALUES))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddValue"<<EndLog;
		return M4_ERROR;
	}

	m4uint32_t		hItem = ai_roItem.ItemDef.GetHandle();
	ClAccess		*poAccess = ai_roItem.GetpAccess();
	ClCompiledMCR	*pCMCR = poAccess->GetpCMCR();
	m4uint32_t		hExecItem = ai_roState.m_hItem;
	

	if (m_pActiveState->m_hItem != hExecItem)
	{
		if (m_pActiveState->m_hItem != hItem)
		{
			//Si no es un concepto siempre deben cuadrar la pila de stt y la de ejecución.
			g_oChLog<<BeginWarning(M4_CH_DM_STT_STATE_NOT_MATCHED)<<m_pActiveState->m_hItem<< hExecItem << EndLog;
		}
	}

	ClSttValues * pSttValue = new ClSttValues();

	pSttValue->m_vDate.SetDate(ClActualDate((m4uint8_t)M4CL_TIME_LOCAL_TIME));
	pSttValue->m_vValue.Set(ai_vValue, M4_TRUE);

	if (ai_roItem.Value.Count() > 0)
	{
		pSttValue->m_dSliceStart = ai_roItem.Value.GetStartDate();
		pSttValue->m_dSliceEnd = ai_roItem.Value.GetEndDate();
	}
	else
	{
		pSttValue->m_dSliceStart = 0;
		pSttValue->m_dSliceEnd = 0;
	}

	pSttValue->m_iPriority = ai_iPriority;
	pSttValue->m_bAssigned = ai_bAssigned;
	pSttValue->m_iSource = ai_iSource;
	pSttValue->m_iM4Type = pCMCR->GetItemM4Type(hItem);

	ClSttState::GetItemIds(hItem, -1, pCMCR, pSttValue->m_vIdT3, pSttValue->m_vIdNode, pSttValue->m_vIdItem, pSttValue->m_vIdRule);
	pSttValue->m_dNorm = ai_roState.m_poFormula.GetStartDate();
	pSttValue->m_iLine = ai_roState.m_iSourceLine - 1;
	pSttValue->m_vCallStack.SetString((const m4pchar_t)ai_pccCallStack);
	pSttValue->m_iCSType = pCMCR->GetItemCsType(hItem);

	m_pActiveState->AddValuesList(pSttValue);

	return M4_SUCCESS;
}


m4return_t ClVMStatistics::sttAddRead( ClItem& ai_roItem, m4VariantType &ai_vValue, m4date_t ai_dStartDate, m4date_t ai_dEndDate, const m4VMState_t& ai_roState, m4pcchar_t ai_pccCallStack )
{
	// De momento no se almacena la pila de llamadas
	ai_pccCallStack = "";

	if (!(m_iInit & M4_VM_STATISTICS_ENABLED_READS))
		return M4_SUCCESS;


	/*
	if (ai_roItem.ItemDef.Type() != M4CL_ITEM_TYPE_CONCEPT) {
		// Solo lecturas a conceptos
		return M4_SUCCESS;
	}
	*/

	m4uint32_t	hExecItem = ai_roState.m_hItem;

	if (ai_roState.m_poAccess->GetpCMCR()->GetItemType(hExecItem) != M4CL_ITEM_TYPE_CONCEPT) {
		// Solo lecturas desde conceptos
		return M4_SUCCESS;
	}

	if (m_pActiveState == NULL)
	{
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddRead"<<EndLog;
		return M4_ERROR;
	}

	m4uint32_t		hItem = ai_roItem.ItemDef.GetHandle();
	ClAccess		*poAccess = ai_roItem.GetpAccess();
	ClCompiledMCR	*pCMCR = poAccess->GetpCMCR();
	

	if (m_pActiveState->m_hItem != hExecItem)
	{
		if (m_pActiveState->m_hItem != hItem)
		{
			//Si no es un concepto siempre deben cuadrar la pila de stt y la de ejecución.
			g_oChLog<<BeginWarning(M4_CH_DM_STT_STATE_NOT_MATCHED)<<m_pActiveState->m_hItem<< hExecItem << EndLog;
		}
	}

	ClSttReads * pSttRead = new ClSttReads();

	pSttRead->m_vDate.SetDate(ClActualDate((m4uint8_t)M4CL_TIME_LOCAL_TIME));
	pSttRead->m_vValue.Set(ai_vValue, M4_TRUE);

	pSttRead->m_dSliceStart = 0;
	pSttRead->m_dSliceEnd = 0;

	if (ai_roItem.Value.Count() > 0)
	{
		if (ai_dStartDate > 0 && ai_dEndDate > 0 )
		{
			pSttRead->m_dSliceStart = ai_dStartDate;
			pSttRead->m_dSliceEnd = ai_dEndDate;
		}
		else if (ai_dStartDate < 0 && ai_dEndDate < 0 )
		{
			pSttRead->m_dSliceStart = ai_roItem.Value.GetStartDate();
			pSttRead->m_dSliceEnd = ai_roItem.Value.GetEndDate();
		}
	}

	pSttRead->m_iM4Type = pCMCR->GetItemM4Type(hItem);

	ClSttState::GetItemIds(hItem, -1, pCMCR, pSttRead->m_vIdT3, pSttRead->m_vIdNode, pSttRead->m_vIdItem, pSttRead->m_vIdRule);
	pSttRead->m_dNorm = ai_roState.m_poFormula.GetStartDate();
	pSttRead->m_iLine = ai_roState.m_iSourceLine - 1;
	pSttRead->m_vCallStack.SetString((const m4pchar_t)ai_pccCallStack);
	pSttRead->m_iCSType = pCMCR->GetItemCsType(hItem);

	m_pActiveState->AddReadsList(pSttRead);

	return M4_SUCCESS;
}


m4uint16_t ClVMStatistics::sttGetInitState(void)
{
	return m_iInit;
}


void ClVMStatistics::sttSetInitState(m4uint16_t ai_iState)
{
	if (ai_iState == m_iInit)
		return;
		
	if (m_iInit == M4_VM_STATISTICS_NO_INIT)
		return;

	//Restricciones

	//Si DBTimes, se activan Times
	if (ai_iState & M4_VM_STATISTICS_ENABLED_DBTIMES_TIME)
		ai_iState |= M4_VM_STATISTICS_ENABLED_TIMES_TIME;

	//Si DBTimes, se activan Items
	if (ai_iState & M4_VM_STATISTICS_ENABLED_TIMES_TIME)
		ai_iState |= M4_VM_STATISTICS_ENABLED_ITEMS_TIME;

	m_iInit = ai_iState;

	/* Pruebas ??????????????????????????????????????????????????????????????????
	if( m_iInit & M4_VM_STATISTICS_ENABLED_VALUES )
	{
		m_iInit |= M4_VM_STATISTICS_ENABLED_READS;
	}*/
}


m4return_t ClVMStatistics::sttStateBegin (const m4VMState_t& ai_roState)
{
	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	//Salvamos el estado anterior.
	if (m_pActiveState!=NULL)
	{
#ifdef _WINDOWS
		//Sumamos al tiempo stt el delay del propio crono: 1 crono por estado para el padre
		m_pActiveState->m_oExecData.m_dSelfSttTime += ClSttClock::m_dClockSelfTime;
#endif
		m_pSttStack->Push(m_pActiveState);
	}

	m_pActiveState = new ClSttState(ai_roState);

	//Añadido para corregir un Bug al asignar m_uiChannelCSType
	if (M4_TRUE == IsClient())
	{
		m_pActiveState->m_uiChannelCSType = M4CL_CSTYPE_FRONT;
	}
	else
	{
		m_pActiveState->m_uiChannelCSType = M4CL_CSTYPE_BACK;
	}


#ifdef _WINDOWS
	//Sumamos al tiempo stt el delay del propio crono: 1 crono por estado para el hijo
	m_pActiveState->m_oExecData.m_dSelfSttTime += ClSttClock::m_dClockSelfTime;
#endif

	return M4_SUCCESS;
}


m4return_t ClVMStatistics::sttStateEndAll (void)
{
	m4return_t ret = M4_SUCCESS;

	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	while ( (ret != M4_ERROR) && (m_pActiveState != NULL) )
		ret = sttStateEnd(0);

	return ret;
}


m4return_t ClVMStatistics::sttStateBegin (eSpecialItems_t ai_eItem, m4uint8_t ai_uiCSType, m4uint8_t ai_uiChannelCSType)
{
	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	//Salvamos el estado anterior.
	if (m_pActiveState!=NULL)
	{
#ifdef _WINDOWS
		//Sumamos al tiempo stt el delay del propio crono: 1 crono por estado para el padre
		m_pActiveState->m_oExecData.m_dSelfSttTime += ClSttClock::m_dClockSelfTime;
#endif
		m_pSttStack->Push(m_pActiveState);
	}

	m_pActiveState = new ClSttState(ai_eItem, ai_uiCSType, ai_uiChannelCSType);

#ifdef _WINDOWS
	//Sumamos al tiempo stt el delay del propio crono: 1 crono por estado para el hijo
	m_pActiveState->m_oExecData.m_dSelfSttTime += ClSttClock::m_dClockSelfTime * 2;
#endif

	return M4_SUCCESS;
}


m4return_t ClVMStatistics::GetActiveItemIds (m4VariantType &ao_ActiveIdT3, m4VariantType &ao_ActiveIdNode, m4VariantType &ao_ActiveIdItem, m4VariantType &ao_ActiveIdRule)
{
	return m_pActiveState->GetItemIds (ao_ActiveIdT3, ao_ActiveIdNode, ao_ActiveIdItem, ao_ActiveIdRule);
}

m4return_t ClVMStatistics::GetParentItemIds (m4VariantType &ao_ParentIdT3, m4VariantType &ao_ParentIdNode, m4VariantType &ao_ParentIdItem, m4VariantType &ao_ParentIdRule)
{
	m4return_t ret = M4_ERROR;
	ClSttState * pParentState;

	ret = m_pSttStack->Top(pParentState);
	
	if (ret == M4_ERROR)
	{
		//No tiene padre
		ao_ParentIdT3.Type = ao_ParentIdNode.Type = ao_ParentIdItem.Type = ao_ParentIdRule.Type = M4CL_CPP_TYPE_STRING_VAR;
		
		ao_ParentIdT3.Data.PointerChar = "NONE";
		ao_ParentIdNode.Data.PointerChar = "NONE";
		ao_ParentIdItem.Data.PointerChar = "NONE";
		ao_ParentIdRule.Data.PointerChar = "NONE";
		return M4_SUCCESS;
	}

	return pParentState->GetItemIds(ao_ParentIdT3, ao_ParentIdNode, ao_ParentIdItem, ao_ParentIdRule);
}


// Inicia el crono, si ya estaba inicializado, suma el tiempo previamente
m4return_t ClVMStatistics::sttStartDBClock (void )
{
	m4return_t ret = M4_SUCCESS;

	if (!(m_iInit & M4_VM_STATISTICS_ENABLED_TIME))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttStartDBClock"<<EndLog;
		return M4_ERROR;
	}

	m_pActiveState->m_ClockLocal.Start();

#ifdef _WINDOWS
	//Sumamos al tiempo stt el delay del propio crono
	m_pActiveState->m_oExecData.m_dSelfSttTime += ClSttClock::m_dClockSelfTime;
#endif

	return M4_SUCCESS;
}

m4return_t ClVMStatistics::sttEndDBClock (m4int32_t ai_idLConn, eDBFunctionType_t ai_eFun, m4int16_t ai_eOp, m4uint8_t ai_IsInternal, m4pcchar_t ai_pccStatement)
{
	m_oSttClock.Start();
	m4double_t dTime;

	if (!(m_iInit & M4_VM_STATISTICS_ENABLED_TIME))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttEndDBClock"<<EndLog;
		return M4_ERROR;
	}

	dTime = m_pActiveState->m_ClockLocal.GetDifference();

	m_pActiveState->m_oExecData.m_dSelfDBTime += dTime;

	if ( m_iInit & M4_VM_STATISTICS_ENABLED_DB )
	{
		if (!m_pActiveState->m_pDBCahe)
			m_pActiveState->m_pDBCahe = new ClDBCache();

		m_pActiveState->m_pDBCahe->sttAddDBTime(ai_idLConn, ai_eFun, ai_eOp, ai_IsInternal, ai_pccStatement, dTime);
	}

	m_pActiveState->m_oExecData.m_dSelfSttTime += m_oSttClock.GetDifference();

#ifdef _WINDOWS
	//Sumamos al tiempo stt el delay del propio crono: 2 cronos / 2 (la mitad se queda ya dentro del propio crono)
	m_pActiveState->m_oExecData.m_dSelfSttTime += ClSttClock::m_dClockSelfTime;
#endif
	
	return M4_SUCCESS;
}

m4return_t ClVMStatistics::sttEndDBClock (m4int32_t ai_idLConn, eDBFunctionType_t ai_eFun)
{
	m_oSttClock.Start();
	m4double_t dTime;

	if (!(m_iInit & M4_VM_STATISTICS_ENABLED_TIME))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttEndDBClock"<<EndLog;
		return M4_ERROR;
	}

	dTime = m_pActiveState->m_ClockLocal.GetDifference();

	m_pActiveState->m_oExecData.m_dSelfDBTime += dTime;

	if ( m_iInit & M4_VM_STATISTICS_ENABLED_DB )
	{
		if (!m_pActiveState->m_pDBCahe)
			m_pActiveState->m_pDBCahe = new ClDBCache();

		m_pActiveState->m_pDBCahe->sttAddDBTime(ai_idLConn, ai_eFun, M4_STT_LDB_SELECT, 0, "", dTime);
	}

	m_pActiveState->m_oExecData.m_dSelfSttTime += m_oSttClock.GetDifference();
	
#ifdef _WINDOWS
	//Sumamos al tiempo stt el delay del propio crono: 2 cronos / 2 (la mitad se queda ya dentro del propio crono)
	m_pActiveState->m_oExecData.m_dSelfSttTime += ClSttClock::m_dClockSelfTime;
#endif

	return M4_SUCCESS;
}


m4return_t ClVMStatistics::sttAddComTime (m4double_t ai_dComTime)
{
	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{		
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddComTime"<<EndLog;
		return M4_ERROR;
	}
 
	m_pActiveState->m_oExecData.m_dSelfComTime += ai_dComTime;

	return M4_SUCCESS;
}


m4return_t ClVMStatistics::sttAddLDBTime (m4double_t ai_dLDBTime)
{
	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{		
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddLDBTime"<<EndLog;
		return M4_ERROR;
	}

	m_pActiveState->m_oExecData.m_dSelfLDBTime += ai_dLDBTime;

	//Prueba
	//m_pActiveState->m_dSelfComTime += ai_dLDBTime;

	return M4_SUCCESS;
}


m4return_t ClVMStatistics::sttAddOutputPDUSize (m4double_t ai_dOutputPDUSize, m4double_t ai_dSttOutputPDUSize)
{
	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{		
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddOutputPDUSize"<<EndLog;
		return M4_ERROR;
	}

	m_pActiveState->m_oExecData.m_dSelfOutputPDUSize += ai_dOutputPDUSize;

	m_pActiveState->m_oExecData.m_dSelfSttOutputPDUSize += ai_dSttOutputPDUSize;

	return M4_SUCCESS;
}

m4return_t ClVMStatistics::sttAddInputPDUSize (m4double_t ai_dInputPDUSize, m4double_t ai_dSttInputPDUSize)
{
	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{		
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddInputPDUSize"<<EndLog;
		return M4_ERROR;
	}

	m_pActiveState->m_oExecData.m_dSelfInputPDUSize += ai_dInputPDUSize;

	m_pActiveState->m_oExecData.m_dSelfSttInputPDUSize += ai_dSttInputPDUSize;

	return M4_SUCCESS;
}


m4return_t ClVMStatistics::sttAddGeneralData (m4pcchar_t ai_pcGeneralData)
{
	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{		
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddGeneralData"<<EndLog;
		return M4_ERROR;
	}

	if (m_pActiveState->m_oExecData.m_pcGeneralData)
	{
		delete [] (m_pActiveState->m_oExecData.m_pcGeneralData);
		m_pActiveState->m_oExecData.m_pcGeneralData = 0;
	}

	if (ai_pcGeneralData)
	{
		m_pActiveState->m_oExecData.m_pcGeneralData = new m4char_t[strlen(ai_pcGeneralData) + 1];
		strcpy(m_pActiveState->m_oExecData.m_pcGeneralData, ai_pcGeneralData);
	}

	return M4_SUCCESS;
}

m4return_t ClVMStatistics::sttAddService (m4pcchar_t ai_pcService)
{
	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{		
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddService"<<EndLog;
		return M4_ERROR;
	}

	if (m_pActiveState->m_oExecData.m_pcService)
	{
		delete [] (m_pActiveState->m_oExecData.m_pcService);
		m_pActiveState->m_oExecData.m_pcService = 0;
	}

	if (ai_pcService)
	{
		m_pActiveState->m_oExecData.m_pcService = new m4char_t[strlen(ai_pcService) + 1];
		strcpy(m_pActiveState->m_oExecData.m_pcService, ai_pcService);
	}

	m_pActiveState->AddFrontInterface();

	return M4_SUCCESS;
}

m4return_t ClVMStatistics::sttAddCacheHit (m4bool_t ai_bFoundInCache)
{
	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{		
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddCacheHit"<<EndLog;
		return M4_ERROR;
	}

	//OJO. Es para desarrollo
	//Asegurarnos de que no estaba ya asignado el campo.
	M4_ASSERT(m_pActiveState->m_oExecData.m_eCacheHit == M4_VM_STT_CACHE_NO_CACHEABLE);

	if (M4_TRUE == ai_bFoundInCache)
		m_pActiveState->m_oExecData.m_eCacheHit = M4_VM_STT_CACHE_FOUND;
	else 
		m_pActiveState->m_oExecData.m_eCacheHit = M4_VM_STT_CACHE_NOT_FOUND;

	return M4_SUCCESS;
}

//Funciones de ExecParams
m4return_t ClVMStatistics::sttAddExecParam (m4VariantType &ai_vParam)
{
	m4VariantType * pParam = NULL;

	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{		
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddExecParam"<<EndLog;
		return M4_ERROR;
	}

	pParam = new m4VariantType(ai_vParam);

	m_pActiveState->AddExecParamsList(pParam);

	return M4_SUCCESS;
	
}

m4return_t ClVMStatistics::sttAddExecParam (m4double_t ai_dParam)
{
	m4VariantType * pParam = NULL;

	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{		
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddExecParam"<<EndLog;
		return M4_ERROR;
	}

	pParam = new m4VariantType(ai_dParam);

	m_pActiveState->AddExecParamsList(pParam);

	return M4_SUCCESS;
}

m4return_t ClVMStatistics::sttAddExecParam (m4pchar_t ai_pcParam)
{
	m4VariantType * pParam = NULL;

	if (!(m_iInit & M4_VM_STATISTICS_ENABLED))
		return M4_SUCCESS;

	if (m_pActiveState == NULL)
	{		
		g_oChLog<<BeginWarning(M4_CH_DM_STT_NO_STATE_STARTED)<<"sttAddExecParam"<<EndLog;
		return M4_ERROR;
	}

	pParam = new m4VariantType(ai_pcParam, M4_TRUE);

	m_pActiveState->AddExecParamsList(pParam);

	return M4_SUCCESS;
}

ClSttStateTimer::ClSttStateTimer(ClVMStatistics * ai_poStatistics, m4uint16_t ai_iStatisticsLevel, eSpecialItems_t ai_eItem)
{
	m_poStatistics = ai_poStatistics;

	if (ai_iStatisticsLevel == M4_VM_STATISTICS_DISABLED)
		m_poStatistics = 0;

	if (m_poStatistics)
	{
		m4uint8_t iChannelCSType;
		m4uint8_t iCSType = M4CL_CSTYPE_MAPPED;
		
		if (M4_TRUE == m_poStatistics->IsClient())
		{
			iChannelCSType = M4CL_CSTYPE_FRONT;
		}
		else
		{
			iChannelCSType = M4CL_CSTYPE_BACK;
		}

		m_poStatistics->sttStateBegin(ai_eItem, iCSType, iChannelCSType);
	}

}

ClSttStateTimer::~ClSttStateTimer()
{
	if (m_poStatistics)
		m_poStatistics->sttStateEnd(0);
}

