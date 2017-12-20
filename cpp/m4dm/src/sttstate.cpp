//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           sttstate.cpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:             21 may 97
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    
////
//==============================================================================

#include "sttstate.hpp"
#include "sttstate.inl"


m4return_t ClSttExecData::Serialize(ClGenericIODriver& IOD)
{
	//Tiempos
	if ( M4_ERROR == IOD.Write(m_dTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dComTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSttTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSelfTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSelfComTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSelfDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSelfSttTime) )
		return M4_ERROR;

	//De momento no exportados al canal
	if ( M4_ERROR == IOD.Write(m_dLDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSelfLDBTime) )
		return M4_ERROR;

	//tamaños PDUs
	/*
	if ( M4_ERROR == IOD.Write(m_dOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dInputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSelfOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSelfInputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSelfSttOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSelfSttInputPDUSize) )
		return M4_ERROR;*/

	//Cache
	if ( M4_ERROR == IOD.Write((m4uint8_t)m_eCacheHit) )
		return M4_ERROR;	

	//CS
	if (m_pcGeneralData != NULL)
	{
		if ( M4_ERROR == IOD.Write((void*)m_pcGeneralData, strlen(m_pcGeneralData)+1) )
			return M4_ERROR;
	}
	else
	{
		if ( M4_ERROR == IOD.Write("", 1) )
			return M4_ERROR;
	}

	/*
	if (m_pcService != NULL)
	{
		if ( M4_ERROR == IOD.Write((void*)m_pcService, strlen(m_pcService)+1) )
			return M4_ERROR;
	}
	else
	{
		if ( M4_ERROR == IOD.Write("", 1) )
			return M4_ERROR;
	}*/

	return M4_SUCCESS;
}


m4return_t ClSttExecData::DeSerialize(ClGenericIODriver& IOD)
{
	//Tiempos
	if ( M4_ERROR == IOD.Read(m_dTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dComTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSttTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSelfTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSelfComTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSelfDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSelfSttTime) )
		return M4_ERROR;

	//De momento no exportados al canal
	if ( M4_ERROR == IOD.Read(m_dLDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSelfLDBTime) )
		return M4_ERROR;

	/*
	//tamaños PDUs
	if ( M4_ERROR == IOD.Read(m_dOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dInputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSelfOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSelfInputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSelfSttOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSelfSttInputPDUSize) )
		return M4_ERROR;
	*/

	//Cache
	m4uint8_t iAux;
	if ( M4_ERROR == IOD.Read(iAux) )
		return M4_ERROR;

	switch (iAux)
	{
		case 0:
			m_eCacheHit = M4_VM_STT_CACHE_NO_CACHEABLE;
			break;
		case 1:
			m_eCacheHit = M4_VM_STT_CACHE_FOUND;
			break;
		default:
			m_eCacheHit = M4_VM_STT_CACHE_NOT_FOUND;
			break;
	}

	//CS
	m4pchar_t pcGeneralData = 0, pcService = 0;
	m4uint32_t iSize;

	if ( M4_ERROR ==IOD.ReadSize(iSize) )
		return M4_ERROR;

	if (!iSize)
		return M4_ERROR;

	pcGeneralData = new m4char_t[iSize];
	
	if ( M4_ERROR == IOD.ReadBuffer(pcGeneralData) )
		return M4_ERROR;

	if (pcGeneralData[0] == '\0')
	{
		delete [] pcGeneralData;
	}
	else
	{
		if (m_pcGeneralData != NULL)
			delete [] m_pcGeneralData;

		m_pcGeneralData = pcGeneralData;
	}

	/*
	if ( M4_ERROR ==IOD.ReadSize(iSize) )
		return M4_ERROR;

	if (!iSize)
		return M4_ERROR;

	pcService = new m4char_t[iSize];
	
	if ( M4_ERROR == IOD.ReadBuffer(pcService) )
		return M4_ERROR;

	if (m_pcService != NULL)
		delete [] m_pcService;

	m_pcService = pcService;*/

	return M4_SUCCESS;
}


m4return_t ClDBTimes::Serialize(ClGenericIODriver& IOD)
{
	if ( M4_ERROR == IOD.Write(m_idLConn) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dPrepareTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dExecuteTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dFetchTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_iNumPrepare) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_iNumExecute) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_iNumFetch) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_eOp) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_IsInternal) )
		return M4_ERROR;

	if (m_pcStatement != NULL)
	{
		if ( M4_ERROR == IOD.Write((void*)m_pcStatement, strlen(m_pcStatement)+1) )
			return M4_ERROR;
	}
	else
	{
		if ( M4_ERROR == IOD.Write("", 1) )
			return M4_ERROR;
	}

	return M4_SUCCESS;
}


m4return_t ClDBTimes::DeSerialize(ClGenericIODriver& IOD)
{
	if ( M4_ERROR == IOD.Read(m_idLConn) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dPrepareTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dExecuteTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dFetchTime) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_iNumPrepare) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_iNumExecute) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_iNumFetch) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_eOp) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_IsInternal) )
		return M4_ERROR;

	m4pchar_t pcStatement = 0;
	m4uint32_t iSize;

	if ( M4_ERROR ==IOD.ReadSize(iSize) )
		return M4_ERROR;

	if (!iSize)
		return M4_ERROR;

	pcStatement = new m4char_t[iSize];
	
	if ( M4_ERROR == IOD.ReadBuffer(pcStatement) )
		return M4_ERROR;

	if (m_pcStatement != NULL)
		delete [] m_pcStatement;

	m_pcStatement = pcStatement;

	return M4_SUCCESS;
}


m4return_t ClDBCache::Serialize(ClGenericIODriver& IOD)
{
	ClDBTimes * pDBTimes = 0;
	ClDBTimesList::iterator it;
	
	m4uint32_t iSize = m_DBTimesList.size();

	if ( M4_ERROR == IOD.Write(iSize) )
		return M4_ERROR;

	for (it = m_DBTimesList.begin (); it != m_DBTimesList.end(); it++) 
	{
		pDBTimes = *it;
		if (M4_ERROR == pDBTimes->Serialize(IOD))
			return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClDBCache::DeSerialize(ClGenericIODriver& IOD)
{
	ClDBTimes * pDBTimes = 0;
	ClDBTimesList::iterator it;
	
	m_pDBTimesActive = 0;

	m4uint32_t i, iSize = 0;

	if ( M4_ERROR == IOD.Read(iSize) )
		return M4_ERROR;

	for (i=0; i<iSize; i++)
	{
		pDBTimes = new ClDBTimes();

		if (M4_ERROR == pDBTimes->DeSerialize(IOD))
			return M4_ERROR;

		m_DBTimesList.push_back ( pDBTimes );
	}

	return M4_SUCCESS;
}

m4return_t ClSttState::Serialize(ClGenericIODriver& IOD)
{
	//Serializamos el identificador del Item
	m4uint32_t iSpecialItem = (m4uint32_t)m_eSpecialItem;

	if ( M4_ERROR == IOD.Write(iSpecialItem) )
		return M4_ERROR;
	
	if ( M4_ERROR == m_vDate.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == m_vIdT3.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == m_vIdNode.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == m_vIdItem.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == m_vIdRule.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dNorm) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_uiCSType) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_uiChannelCSType) )
		return M4_ERROR;

	if ( M4_ERROR == m_oExecData.Serialize(IOD) )
		return M4_ERROR;

	if (m_pDBCahe != NULL)
	{
		IOD.Write((m4uint8_t)1);
		m_pDBCahe->Serialize(IOD);

		// Para optimizar la liberación de memoria
		delete( m_pDBCahe ) ;
		m_pDBCahe = NULL ;
	}
	else
	{
		IOD.Write((m4uint8_t)0);
	}

	//ChildList
	if (m_poChildList != NULL)
	{
		IOD.Write((m4uint8_t)1);
		
		itClSttStateList it;
		ClSttState * pAuxState;
		m4uint32_t iSize = m_poChildList->size();

		if ( M4_ERROR == IOD.Write(iSize) )
			return M4_ERROR;

		for (it = m_poChildList->begin (); it != m_poChildList->end(); it++) 
		{
			pAuxState = *it;
			if (M4_ERROR == pAuxState->Serialize(IOD))
				return M4_ERROR;

			// Para optimizar la liberación de memoria
			delete( pAuxState ) ;
			*it = NULL ;
		}

		delete( m_poChildList ) ;
		m_poChildList = NULL ;

	}
	else
	{
		IOD.Write((m4uint8_t)0);
	}

	//ValuesList
	if (m_poValuesList != NULL)
	{
		IOD.Write((m4uint8_t)1);
		
		itClSttValuesList it;
		ClSttValues * pValue;
		m4uint32_t iSize = m_poValuesList->size();

		if ( M4_ERROR == IOD.Write(iSize) )
			return M4_ERROR;

		for (it = m_poValuesList->begin (); it != m_poValuesList->end(); it++) 
		{
			pValue = *it;
			if (M4_ERROR == pValue->Serialize(IOD))
				return M4_ERROR;

			// Para optimizar la liberación de memoria
			delete( pValue ) ;
			*it = NULL ;
		}

		delete( m_poValuesList ) ;
		m_poValuesList = NULL ;
	}
	else
	{
		IOD.Write((m4uint8_t)0);
	}

	//ReadsList
	if (m_poReadsList != NULL)
	{
		IOD.Write((m4uint8_t)1);
		
		itClSttReadsList it;
		ClSttReads * pRead;
		m4uint32_t iSize = m_poReadsList->size();

		if ( M4_ERROR == IOD.Write(iSize) )
			return M4_ERROR;

		for (it = m_poReadsList->begin (); it != m_poReadsList->end(); it++) 
		{
			pRead = *it;
			if (M4_ERROR == pRead->Serialize(IOD))
				return M4_ERROR;

			// Para optimizar la liberación de memoria
			delete( pRead ) ;
			*it = NULL ;
		}

		delete( m_poReadsList ) ;
		m_poReadsList = NULL ;
	}
	else
	{
		IOD.Write((m4uint8_t)0);
	}

	//Params
	if (m_poExecParamsList != NULL)
	{
		IOD.Write((m4uint8_t)1);
		
		itm4VariantList it;
		m4VariantType * pParam;
		m4uint32_t iSize = m_poExecParamsList->size();

		if ( M4_ERROR == IOD.Write(iSize) )
			return M4_ERROR;

		for (it = m_poExecParamsList->begin (); it != m_poExecParamsList->end(); it++) 
		{
			pParam = *it;
			if (M4_ERROR == pParam->Serialize(IOD))
				return M4_ERROR;

			// Para optimizar la liberación de memoria
			delete( pParam ) ;
			*it = NULL ;
		}

		delete( m_poExecParamsList ) ;
		m_poExecParamsList = NULL ;
	}
	else
	{
		IOD.Write((m4uint8_t)0);
	}

	
	return M4_SUCCESS;
}


m4return_t ClSttState::DeSerialize(ClGenericIODriver& IOD)
{
	//Serializamos el identificador del Item
	//DeSerializamos el identificador del Item
	m4uint32_t iSpecialItem;

	if ( M4_ERROR == IOD.Read(iSpecialItem) )
		return M4_ERROR;

	m_eSpecialItem = (eSpecialItems_t)iSpecialItem;
	
	if ( M4_ERROR == m_vDate.DeSerialize(IOD) )
		return M4_ERROR;

	m_vIdT3.FreeData();
	if ( M4_ERROR == m_vIdT3.DeSerialize(IOD) )
		return M4_ERROR;

	m_vIdNode.FreeData();
	if ( M4_ERROR == m_vIdNode.DeSerialize(IOD) )
		return M4_ERROR;

	m_vIdItem.FreeData();
	if ( M4_ERROR == m_vIdItem.DeSerialize(IOD) )
		return M4_ERROR;

	m_vIdRule.FreeData();
	if ( M4_ERROR == m_vIdRule.DeSerialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dNorm) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_uiCSType) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_uiChannelCSType) )
		return M4_ERROR;

	if ( M4_ERROR == m_oExecData.DeSerialize(IOD) )
		return M4_ERROR;

	//BDcache
	m4uint8_t iRead;

	if ( M4_ERROR == IOD.Read(iRead) )
		return M4_ERROR;

	if (m_pDBCahe != NULL)
	{
		delete m_pDBCahe;
		m_pDBCahe = NULL ;
	}

	if (iRead == 1)
	{
		m_pDBCahe = new ClDBCache();
		m_pDBCahe->DeSerialize(IOD);
	}

	//ChildList
	if ( M4_ERROR == IOD.Read(iRead) )
		return M4_ERROR;

	if (m_poChildList != NULL)
	{
		DestroyChildList();
	}

	if (iRead == 1)
	{
		ClSttState * pAuxState;
		m4uint32_t i, iSize;

		if ( M4_ERROR == IOD.Read(iSize) )
			return M4_ERROR;

		for (i=0; i< iSize; i++) 
		{
			pAuxState = new ClSttState();

			if (M4_ERROR == pAuxState->DeSerialize(IOD))
				return M4_ERROR;

			AddChildList(pAuxState);
		}
	}

	//ValuesList
	if ( M4_ERROR == IOD.Read(iRead) )
		return M4_ERROR;

	if (m_poValuesList != NULL)
	{
		DestroyValuesList();
	}

	if (iRead == 1)
	{
		ClSttValues * pValue;
		m4uint32_t i, iSize;

		if ( M4_ERROR == IOD.Read(iSize) )
			return M4_ERROR;

		for (i=0; i< iSize; i++) 
		{
			pValue = new ClSttValues();

			if (M4_ERROR == pValue->DeSerialize(IOD))
				return M4_ERROR;

			AddValuesList(pValue);
		}
	}

	//ReadsList
	if ( M4_ERROR == IOD.Read(iRead) )
		return M4_ERROR;

	if (m_poReadsList != NULL)
	{
		DestroyReadsList();
	}

	if (iRead == 1)
	{
		ClSttReads * pRead;
		m4uint32_t i, iSize;

		if ( M4_ERROR == IOD.Read(iSize) )
			return M4_ERROR;

		for (i=0; i< iSize; i++) 
		{
			pRead = new ClSttReads();

			if (M4_ERROR == pRead->DeSerialize(IOD))
				return M4_ERROR;

			AddReadsList(pRead);
		}
	}

	//Params
	if ( M4_ERROR == IOD.Read(iRead) )
		return M4_ERROR;

	if (m_poExecParamsList != NULL)
	{
		DestroyExecParamsList();
	}

	if (iRead == 1)
	{
		m4VariantType * pParam;
		m4uint32_t i, iSize;

		if ( M4_ERROR == IOD.Read(iSize) )
			return M4_ERROR;

		for (i=0; i< iSize; i++) 
		{
			pParam = new m4VariantType();

			if (M4_ERROR == pParam->DeSerialize(IOD))
				return M4_ERROR;

			AddExecParamsList(pParam);
		}
	}

	return M4_SUCCESS;
}

m4return_t ClSttValues::Serialize(ClGenericIODriver& IOD)
{
	if ( M4_ERROR == m_vDate.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == m_vValue.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSliceStart) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSliceEnd) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_iPriority) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_bAssigned) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_iSource) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_iM4Type) )
		return M4_ERROR;

	if ( M4_ERROR == m_vIdT3.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == m_vIdNode.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == m_vIdItem.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == m_vIdRule.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dNorm) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_iLine) )
		return M4_ERROR;

	if ( M4_ERROR == m_vCallStack.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_iCSType) )
		return M4_ERROR;

	return M4_SUCCESS;
}

m4return_t ClSttValues::DeSerialize(ClGenericIODriver& IOD)
{
	m_vDate.FreeData();
	if ( M4_ERROR == m_vDate.DeSerialize(IOD) )
		return M4_ERROR;

	m_vValue.FreeData();
	if ( M4_ERROR == m_vValue.DeSerialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSliceStart) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSliceEnd) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_iPriority) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_bAssigned) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_iSource) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_iM4Type) )
		return M4_ERROR;

	m_vIdT3.FreeData();
	if ( M4_ERROR == m_vIdT3.DeSerialize(IOD) )
		return M4_ERROR;

	m_vIdNode.FreeData();
	if ( M4_ERROR == m_vIdNode.DeSerialize(IOD) )
		return M4_ERROR;

	m_vIdItem.FreeData();
	if ( M4_ERROR == m_vIdItem.DeSerialize(IOD) )
		return M4_ERROR;

	m_vIdRule.FreeData();
	if ( M4_ERROR == m_vIdRule.DeSerialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dNorm) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_iLine) )
		return M4_ERROR;

	m_vCallStack.FreeData();
	if ( M4_ERROR == m_vCallStack.DeSerialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_iCSType) )
		return M4_ERROR;

	return M4_SUCCESS;
}


m4return_t ClSttReads::Serialize(ClGenericIODriver& IOD)
{
	if ( M4_ERROR == m_vDate.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == m_vValue.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSliceStart) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dSliceEnd) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_iM4Type) )
		return M4_ERROR;

	if ( M4_ERROR == m_vIdT3.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == m_vIdNode.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == m_vIdItem.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == m_vIdRule.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_dNorm) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_iLine) )
		return M4_ERROR;

	if ( M4_ERROR == m_vCallStack.Serialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Write(m_iCSType) )
		return M4_ERROR;

	return M4_SUCCESS;
}

m4return_t ClSttReads::DeSerialize(ClGenericIODriver& IOD)
{
	m_vDate.FreeData();
	if ( M4_ERROR == m_vDate.DeSerialize(IOD) )
		return M4_ERROR;

	m_vValue.FreeData();
	if ( M4_ERROR == m_vValue.DeSerialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSliceStart) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dSliceEnd) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_iM4Type) )
		return M4_ERROR;

	m_vIdT3.FreeData();
	if ( M4_ERROR == m_vIdT3.DeSerialize(IOD) )
		return M4_ERROR;

	m_vIdNode.FreeData();
	if ( M4_ERROR == m_vIdNode.DeSerialize(IOD) )
		return M4_ERROR;

	m_vIdItem.FreeData();
	if ( M4_ERROR == m_vIdItem.DeSerialize(IOD) )
		return M4_ERROR;

	m_vIdRule.FreeData();
	if ( M4_ERROR == m_vIdRule.DeSerialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_dNorm) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_iLine) )
		return M4_ERROR;

	m_vCallStack.FreeData();
	if ( M4_ERROR == m_vCallStack.DeSerialize(IOD) )
		return M4_ERROR;

	if ( M4_ERROR == IOD.Read(m_iCSType) )
		return M4_ERROR;

	return M4_SUCCESS;
}


