//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                datacache.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the cache wrapper for data
//
//
//==============================================================================
#include "dmres.hpp"
#include "datacache.hpp"
#include "node_knl.hpp"
#include "clprsstb.hpp"
#include "chan_knl.hpp"
#include "cldates.hpp"
#include "chandata_ser.hpp"
#include "datacache.hpp"
#include "chan_knl.hpp"
#include "node_knl.hpp"
#include "clfileiod.hpp"
#include "m4mdfac.hpp"
#include "dm.hpp"
#include "channel.hpp"
#include "m4mdrt.hpp"


ClDataCache::ClDataCache(ClCache * ai_poCache, m4pcchar_t ai_pPath, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio, m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod) :
	ClDataCacheBase_t(ai_poCache, ai_pPath, "DataCache", ai_iMode, ai_iMaxMemory, ai_iNumObjects, ai_iRefreshRatio, ai_dMaxPeriod, ai_dMaxUnusedPeriod)
{
	static ClDataCacheableObjectFactory * pCoFactory = new ClDataCacheableObjectFactory();
	SetPersistor(pCoFactory);
	SetCOFactory(pCoFactory);
}

ClDataCache::~ClDataCache ()
{
}


m4return_t ClDataCache::GetObject (m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccRSM, m4uint8_t ai_iLanguage, m4uint8_t ai_iOrgType, m4pchar_t ai_pcOrg, m4VariantType * ai_pVarParamsId, m4uint32_t ai_iNumParams, m4VariantType &ai_roStartDate, m4VariantType &ai_roEndDate, m4VariantType &ai_roCorDate, ClChannel_Data* &ao_rpChannelData, m4bool_t &ao_bFound)
{
	m4return_t ret = M4_SUCCESS;
	m4pchar_t pcDataId;
	m4uint16_t iVerCounter; //No utilizamos su valor de vuelta

	pcDataId = ComposeStringKey(ai_iCsType, ai_pccChannelId, ai_pccRSM, ai_iLanguage, ai_iOrgType, ai_pcOrg, ai_pVarParamsId, ai_iNumParams);

	ret = _GetObject(pcDataId, ai_roStartDate.Data.DoubleData, ai_roEndDate.Data.DoubleData, ai_roCorDate.Data.DoubleData, iVerCounter, ao_rpChannelData);									 

	if (ret == M4_SUCCESS)
		ao_bFound = M4_TRUE;
	else
		ao_bFound = M4_FALSE;

	delete [] pcDataId;	

	return M4_SUCCESS;
}

m4return_t ClDataCache::PutObject (m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccRSM, m4uint8_t ai_iLanguage, m4uint8_t ai_iOrgType, m4pchar_t ai_pcOrg, m4VariantType * ai_pVarParamsId, m4uint32_t ai_iNumParams, ClChannel_Data* ai_pChannelData, m4date_t ai_dMaxPeriod)
{
	m4return_t ret = M4_SUCCESS;
	m4pchar_t pcDataId;

	pcDataId = ComposeStringKey(ai_iCsType, ai_pccChannelId, ai_pccRSM, ai_iLanguage, ai_iOrgType, ai_pcOrg, ai_pVarParamsId, ai_iNumParams);

	ret = _PutObject(pcDataId, 0, ai_pChannelData, ai_dMaxPeriod, 0);									 

	delete [] pcDataId;	
	
	return ret;
}

m4pchar_t ClDataCache::ComposeStringKey(m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccRSM, m4uint8_t ai_iLanguage, m4uint8_t ai_iOrgType, m4pchar_t ai_pcOrg, m4VariantType * ai_pVarParamsId, m4uint32_t ai_iNumParams)
{
	m4uint32_t i, iIdSize = 0;
	m4char_t acAuxNumParam[256];	

	//long strings +4 del lang +1 del orgtype +1 del cstype +5 de las @
	iIdSize = strlen(ai_pccChannelId) + strlen(ai_pccRSM) + strlen(ai_pcOrg) + 4 + 1 + 1 + 5 + 1;

	for (i = 0; i < ai_iNumParams; i++)
	{
		switch (ai_pVarParamsId[i].Type)
		{
			case M4CL_CPP_TYPE_NUMBER:
			case M4CL_CPP_TYPE_DATE:
				iIdSize += 256 + 2;
				break;

			case M4CL_CPP_TYPE_STRING_VAR:
				if (!ai_pVarParamsId[i].Data.PointerChar)
					ai_pVarParamsId[i].Data.PointerChar = "";

				iIdSize += strlen(ai_pVarParamsId[i].Data.PointerChar) + 2;
				break;

			case M4CL_CPP_TYPE_NULL:				
				ai_pVarParamsId[i].Type = M4CL_CPP_TYPE_STRING_VAR;
				ai_pVarParamsId[i].Data.PointerChar = "NULL";

				iIdSize += strlen(ai_pVarParamsId[i].Data.PointerChar) + 2;
				break;

			default:
				M4_ASSERT(0);
				break;
		}
	}

	m4pchar_t pcDataId = new m4char_t[iIdSize];

	sprintf(pcDataId, "%s@%s@%d@%1d@%s@%1d", ai_pccChannelId, ai_pccRSM, ai_iLanguage, ai_iOrgType, ai_pcOrg, ai_iCsType);

	for (i = 0; i < ai_iNumParams; i++)
	{
		switch (ai_pVarParamsId[i].Type)
		{
			case M4CL_CPP_TYPE_NUMBER:
			case M4CL_CPP_TYPE_DATE:								
				sprintf(acAuxNumParam,"%f",ai_pVarParamsId[i].Data.DoubleData);
				strcat(pcDataId, "(");
				strcat(pcDataId, acAuxNumParam);
				strcat(pcDataId, ")");
				break;

			case M4CL_CPP_TYPE_STRING_VAR:
				M4_ASSERT(ai_pVarParamsId[i].Data.PointerChar);
				strcat(pcDataId, "(");
				strcat(pcDataId, ai_pVarParamsId[i].Data.PointerChar);
				strcat(pcDataId, ")");
				break;

			default:
				M4_ASSERT(0);
				break;
		}
	}

	strupr(pcDataId);

	return pcDataId;
}

m4return_t ClDataCache::RemoveObjectById (m4pchar_t ai_pccChannelId,m4puint32_t ai_piRemovedCount)
{
	//Componemos la parte de la clave que lleva el nombre del canal.
	m4return_t ret;

	m4pchar_t pcDataId = new m4char_t[strlen(ai_pccChannelId) + 2];

	sprintf(pcDataId, "%s@", ai_pccChannelId);

	strupr(pcDataId);

    //Hacemos remove de todos los objetos que tengan en su clave la cadena.
	ret = m_poCache->RemoveAll(pcDataId, ai_piRemovedCount);

	delete [] pcDataId;	

	return ret;
}

m4return_t ClDataCache::RemoveObjectByIdAndVersion (m4pchar_t ai_pccChannelId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType)
{
	//para ClDataCache ai_bParentObject no sirve de nada.
	m4return_t ret = M4_SUCCESS;

	m4pchar_t pcDataId = new m4char_t[strlen(ai_pccChannelId) + 2];

	sprintf(pcDataId, "%s@", ai_pccChannelId);

	strupr(pcDataId);

	ClCOCondition oCondId, oCondVer;

	oCondId.m_iConditionMask = M4_CO_COND_CONTAINED_IN_KEY;
	oCondId.m_pValue = pcDataId;
	oCondId.m_iAnd = 1;
	oCondId.m_pNext = &oCondVer;

	// Si ai_bParentObject hay que quitar el ChannelData cuando su version de CMCR (T3) sea distinta.
	if ( ai_iRemoveType == ClBaseCacheInterface::MAIN )
	{
		oCondVer.m_iConditionMask = M4_CO_COND_OLDER_VERSION;
	}
	else
	{
		oCondVer.m_iConditionMask = M4_CO_COND_OLDER_PARENT_VERSION;
	}

	oCondVer.m_pValue = &ai_dVersion;
	oCondVer.m_iAnd = 1;
	oCondVer.m_pNext = 0;

	ret = m_poCache->RemoveByCondition(&oCondId);

	delete [] pcDataId;	

	return ret;

}





//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

m4return_t ClChannelDataContainer::Serialize(ClGenericIODriver& IOD)
{
	m4return_t ret;

	ret = ClDatedContainer <ClChannel_Data>::Serialize(IOD);

	if (M4_ERROR == ret)
		return M4_ERROR;

	ret = IOD.Write(m_dT3Version);

	if (ret == M4_ERROR)
		return M4_ERROR;

	return M4_SUCCESS;
}

m4return_t ClChannelDataContainer::DeSerialize(ClGenericIODriver& IOD)
{
	m4return_t ret;

	ret = ClDatedContainer <ClChannel_Data>::DeSerialize(IOD);

	if (M4_ERROR == ret)
		return M4_ERROR;

	ret = IOD.Read(m_dT3Version);

	if (ret == M4_ERROR)
		return M4_ERROR;

	return M4_SUCCESS;
}

m4return_t ClChannelDataContainer::SetValue ( ClChannel_Data * ai_pObject )
{
	ClDatedContainer <ClChannel_Data>::SetValue(ai_pObject);

	if (ai_pObject)
	{
		m_dT3Version = ((ClChannel_Data*)GetValue())->GetpExtInfo()->MCR.GetpCMCR()->GetChannelVersion();
	}

	return M4_SUCCESS;
}


m4bool_t ClChannelDataContainer::MatchCondition (void * ai_pCondition)
{
	m4bool_t bValue = M4_FALSE;
	ClCOCondition * pCondition = (ClCOCondition*)ai_pCondition;

	if (!pCondition)
		return M4_FALSE;

	if (!pCondition->m_pValue)
		return M4_FALSE;

	bValue = M4_FALSE;

	switch (pCondition->m_iConditionMask)
	{
		case M4_CO_COND_CONTAINED_IN_KEY:
			//Se cumple si ai_pcPortionKey se encuentra al principio de Getm_pszKey()!!
			if(strstr(m_poClCacheKey->Getm_pszKey(),(m4pchar_t)pCondition->m_pValue) == m_poClCacheKey->Getm_pszKey())		
			{
				bValue = M4_TRUE;
			}

			break;

		case M4_CO_COND_OLDER_VERSION:
			if ( m_dVersion < *(m4date_t*)(pCondition->m_pValue) )
			{
				bValue = M4_TRUE;
			}
			break;

		case M4_CO_COND_OLDER_PARENT_VERSION:
			if ( m_dT3Version < *(m4date_t*)(pCondition->m_pValue) )
			{
				bValue = M4_TRUE;
			}
			break;

		default:
			break;
	};

	if (pCondition->m_pNext)
	{
		if ( ((pCondition->m_iAnd) && (M4_TRUE == bValue)) || ((!pCondition->m_iAnd) && (M4_FALSE == bValue)) )
		{
			bValue = MatchCondition(pCondition->m_pNext);
		}
	}

	return bValue;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

ClDataCacheableObjectFactory::ClDataCacheableObjectFactory(void)
{
	m_poChannelManager = 0;
	m_oMutex.Init();
}

ClDataCacheableObjectFactory::~ClDataCacheableObjectFactory(void)
{
}

m4return_t ClDataCacheableObjectFactory::Persist  (ClPersistiableObject & ai_oPersistiableObj)
{	
	m4return_t ret;

	if (!ai_oPersistiableObj.GetObjId())
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_PERSISTIABLE_OBJECT_ID)<<EndLog;			
		return M4_ERROR;
	}

	eObjectType oObjectType = ai_oPersistiableObj.GetObjId()->GetType ();

	if (oObjectType == ChannelDataObjectType)
	{
		ClFileIODriver IOD;

		ret = InitIODFromObject(ai_oPersistiableObj, IOD, GEN_IO_MODE_WRITE);

		if (ret != M4_ERROR)
		{

			ClChannelDataSerializer oChannelDataSerializer;
			
			ret = oChannelDataSerializer.Serialize(IOD, (ClChannel_Data*)&ai_oPersistiableObj, M4_FALSE, M4CL_RECORD_SERIAL_ALL, M4CL_ITEM_SERIAL_ALL);

			IOD.End(M4_TRUE);
		}
	}
	else
	{
		ret = ClDiskPersistor::Persist(ai_oPersistiableObj);
	}

	return ret;

}

m4return_t ClDataCacheableObjectFactory::DePersist(ClPersistiableObject & ai_oPersistiableObj)
{
	m4return_t ret;

	if (!ai_oPersistiableObj.GetObjId())
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_PERSISTIABLE_OBJECT_ID)<<EndLog;			
		return M4_ERROR;
	}

	eObjectType oObjectType = ai_oPersistiableObj.GetObjId()->GetType ();

	if (oObjectType == ChannelDataObjectType)
	{
		ClFileIODriver IOD;

		ret = InitIODFromObject(ai_oPersistiableObj, IOD, GEN_IO_MODE_READ);

		if (ret != M4_ERROR)
		{
			if (m_poChannelManager)
			{
				ClChannelDataSerializer oChannelDataSerializer;
							
				ret = oChannelDataSerializer.DeSerialize(IOD, m_poChannelManager->GetpFactory(), NULL, NULL, (ClChannel_Data*)&ai_oPersistiableObj);

				IOD.End(M4_TRUE);
			}
			else
			{
				//No tenemos ChannelManager-> no podemos deserializar el objeto pq hace falta una CMCRFactory.
				ret = M4_ERROR;
			}
		}		
	}
	else
	{
		ret = ClDiskPersistor::DePersist(ai_oPersistiableObj);
	}

	return ret;
}


