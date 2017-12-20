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
//    This module defines thefunctions of the class of the factory of
//    compiled metachannels
//
//
//==============================================================================

#include "prescache.hpp"
#include "clprsstb.hpp"
#include "clpresen.hpp"
#include "cldates.hpp"
#include "clcofac.hpp"
#include "attributes.hpp"

ClPresentationCache::ClPresentationCache(ClCache * ai_poCache, m4pcchar_t ai_pPath, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio, m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod) :
	ClPresentationCacheBase_t (ai_poCache, ai_pPath, "PresCache", ai_iMode, ai_iMaxMemory, ai_iNumObjects, ai_iRefreshRatio, ai_dMaxPeriod, ai_dMaxUnusedPeriod)
{
	static ClPresentationCacheableObjectFactory * pCoFactory = new ClPresentationCacheableObjectFactory();
	SetPersistor(pCoFactory);
	SetCOFactory(pCoFactory);
}

ClPresentationCache::~ClPresentationCache ()
{
}


m4return_t ClPresentationCache::GetObject (const ClPresentationAttributes & ai_ClPresentationAttributes, ClPresentation* &ao_rpPresentation)
{
	m4return_t ret = M4_SUCCESS;
	m4pchar_t pcPresentationId;
	m4uint16_t iVerCounter; //No utilizamos su valor de vuelta

	pcPresentationId = ComposeStringKey(ai_ClPresentationAttributes);

	ret = _GetObject(pcPresentationId, M4CL_MINUS_INFINITE_IN_JULIAN, M4CL_PLUS_INFINITE_IN_JULIAN, ClActualDate(), iVerCounter, ao_rpPresentation);									 

	delete [] pcPresentationId;	

	return ret;
}

m4return_t ClPresentationCache::PutObject (const ClPresentationAttributes & ai_ClPresentationAttributes, ClPresentation* ai_pPresentation, m4date_t ai_dMaxPeriod)
{
	m4return_t ret = M4_SUCCESS;
	m4pchar_t pcPresentationId;

	pcPresentationId = ComposeStringKey(ai_ClPresentationAttributes);

	ret = _PutObject(pcPresentationId, 0, ai_pPresentation, ai_dMaxPeriod, 0);									 

	delete [] pcPresentationId;	

	return ret;
}

m4pchar_t ClPresentationCache::ComposeStringKey(const ClPresentationAttributes & ai_ClPresentationAttributes)
{
	m4pchar_t pcPresentationId = new m4char_t[strlen(ai_ClPresentationAttributes.m_pIdPresentation) + strlen(ai_ClPresentationAttributes.m_pRole) + 2 + 4 + 1];	// 4 del lenguaje

	sprintf(pcPresentationId, "%s@%s@%d",ai_ClPresentationAttributes.m_pIdPresentation, ai_ClPresentationAttributes.m_pRole, ai_ClPresentationAttributes.m_iLanguage);

	strupr(pcPresentationId);

	return pcPresentationId;
}


m4return_t ClPresentationCache::RemoveObjectById (m4pchar_t ai_pcIdPresentation, m4puint32_t ai_piRemovedCount)
{
	//Componemos la parte de la clave que lleva el id de la presentacion.
	m4return_t ret = M4_SUCCESS;

	m4pchar_t pcPresentationId = new m4char_t[strlen(ai_pcIdPresentation) + 2];

	sprintf(pcPresentationId, "%s@",ai_pcIdPresentation);

	strupr(pcPresentationId);

    //Hacemos remove de todos los objetos que tengan en su clave la cadena.
	ret = m_poCache->RemoveAll(pcPresentationId, ai_piRemovedCount);

	delete [] pcPresentationId;	

	return ret;
}

struct stIdAndVersion
{
	m4pchar_t m_pcId;
	m4date_t  m_dVersion;
};

m4return_t ClPresentationCache::RemoveObjectByIdAndVersion (m4pchar_t ai_pcIdPresentation, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType)
{
	m4return_t ret = M4_SUCCESS;

	if (ai_iRemoveType == ClBaseCacheInterface::MAIN)
	{
		m4pchar_t pcPresentationId = new m4char_t[strlen(ai_pcIdPresentation) + 2];

		sprintf(pcPresentationId, "%s@",ai_pcIdPresentation);

		strupr(pcPresentationId);

		ClCOCondition oCondId, oCondVer;

		oCondId.m_iConditionMask = M4_CO_COND_CONTAINED_IN_KEY;
		oCondId.m_pValue = pcPresentationId;
		oCondId.m_iAnd = 1;
		oCondId.m_pNext = &oCondVer;

		// Si ai_bParentObject hay que quitar la CSCR cuando su version de CMCR (T3) sea distinta.
		oCondVer.m_iConditionMask = M4_CO_COND_OLDER_VERSION;
		oCondVer.m_pValue = &ai_dVersion;
		oCondVer.m_iAnd = 1;
		oCondVer.m_pNext = 0;

		ret = m_poCache->RemoveByCondition(&oCondId);

		delete [] pcPresentationId;	
	}

	else
	{
		// Si ai_bParentObject hay que quitar la Pres cuando su version de CMCR (T3) sea distinta.
		ClCOCondition oCondVer;
	    stIdAndVersion oIdAndVer;
		
		oIdAndVer.m_pcId = ai_pcIdPresentation;
		oIdAndVer.m_dVersion = ai_dVersion;

		if (ai_iRemoveType == ClBaseCacheInterface::PARENT) {
			oCondVer.m_iConditionMask = M4_CO_COND_OLDER_PARENT_VERSION;
		}
		else {
			oCondVer.m_iConditionMask = M4_CO_COND_OLDER_DEPENDENCY_VERSION;
		}
		oCondVer.m_pValue = &oIdAndVer;
		oCondVer.m_iAnd = 1;
		oCondVer.m_pNext = 0;

		ret = m_poCache->RemoveByCondition(&oCondVer);
	}

	return ret;
}

m4return_t ClPresentationContainer::Serialize(ClGenericIODriver& IOD)
{
	m4return_t ret;
	m4uint32_t i;

	ret = ClDatedContainer <ClPresentation>::Serialize(IOD);

	if (M4_ERROR == ret)
		return M4_ERROR;

	ret = IOD.Write(m_iNumT3);

	if (ret == M4_ERROR)
		return M4_ERROR;

	for (i = 0; i < m_iNumT3; i++)
	{
		ret = IOD.Write(m_ppcIdT3[i]);

		if (ret == M4_ERROR)
			return M4_ERROR;

		ret = IOD.Write(m_pdVersionT3[i]);

		if (ret == M4_ERROR)
			return M4_ERROR;
	}

	//--------------------------------------------------------------
	ret = IOD.Write(m_iSizeIncludes);
	if (ret == M4_ERROR) {
		return M4_ERROR;
	}

	for (i=0; i<m_iSizeIncludes; i++) {
		ret = IOD.Write(m_ppcIdInclude[i]);
		if (ret == M4_ERROR) {
			return M4_ERROR;
		}

		//Metemos la fecha de ultima actualizacion (contando sus includes).
		ret = IOD.Write(m_pdVersionInclude[i]);
		if (ret == M4_ERROR) {
			return M4_ERROR;
		}
	}	
	//--------------------------------------------------------------

	return M4_SUCCESS;
}

m4return_t ClPresentationContainer::DeSerialize(ClGenericIODriver& IOD)
{
	m4return_t ret;
	m4uint32_t i;

	ret = ClDatedContainer <ClPresentation>::DeSerialize(IOD);

	if (M4_ERROR == ret)
		return M4_ERROR;

	m4uint32_t iSize = 0;
	m4pchar_t pcT3Id;
	m4double_t dT3Version;

	ret = IOD.Read(m_iNumT3);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (m_iNumT3)
	{
		m_ppcIdT3 = new m4pchar_t[m_iNumT3];
		m_pdVersionT3 = new m4double_t[m_iNumT3];
	}
	
	for (i = 0; i < m_iNumT3; i++)
	{
		ret = IOD.ReadSize(iSize);

		if (ret == M4_ERROR)
			return M4_ERROR;

		pcT3Id = new m4char_t[iSize+1];

		ret = IOD.ReadBuffer(pcT3Id);

		if (ret == M4_ERROR)
		{
			delete [] pcT3Id;
			return M4_ERROR;
		}

		pcT3Id[iSize] = '\0';

		ret = IOD.Read(dT3Version);

		if (ret == M4_ERROR)
			return M4_ERROR;

		m_ppcIdT3[i] = pcT3Id;
		m_pdVersionT3[i] = dT3Version;
	}


	//----------------------------------------------------------------
	m4pchar_t pcIdInclude = NULL;
	m4double_t dIncludeVersion;

	ret = IOD.Read(m_iSizeIncludes);
	if (ret == M4_ERROR) {
		return M4_ERROR;
	}

	if (m_iSizeIncludes != 0) {
		m_ppcIdInclude = new m4pchar_t[m_iSizeIncludes];
		m_pdVersionInclude = new m4double_t[m_iSizeIncludes];
	}
	
	for (i = 0; i < m_iSizeIncludes; i++)
	{
		ret = IOD.ReadSize(iSize);

		if (ret == M4_ERROR)
			return M4_ERROR;

		pcIdInclude = new m4char_t[iSize + 1];

		ret = IOD.ReadBuffer(pcIdInclude);

		if (ret == M4_ERROR)
		{
			delete [] pcIdInclude;
			return M4_ERROR;
		}
		pcIdInclude[iSize] = '\0';

		m_ppcIdInclude[i] = pcIdInclude;

		ret = IOD.Read(dIncludeVersion);
		if (ret == M4_ERROR) {
			return M4_ERROR;
		}

		m_pdVersionInclude[i] = dIncludeVersion;
	}
	//----------------------------------------------------------------

	return M4_SUCCESS;
}

m4return_t ClPresentationContainer::SetValue ( ClPresentation * ai_pObject )
{
	ClDatedContainer <ClPresentation>::SetValue(ai_pObject);

	if (ai_pObject)
	{
		m4uint32_t i;

		for (i = 0; i < m_iNumT3; i++)
		{
			delete [] m_ppcIdT3[i];
		}

		if (m_ppcIdT3)
		{
			delete [] m_ppcIdT3;
			m_ppcIdT3 = 0;
		}
	
		if (m_pdVersionT3)
		{
			delete [] m_pdVersionT3;
			m_pdVersionT3 = 0;
		}

		m4pchar_t pcAux;
		m_iNumT3 = ai_pObject->GetNumT3();

		if (m_iNumT3)
		{
			m_ppcIdT3 = new m4pchar_t[m_iNumT3];
			m_pdVersionT3 = new m4double_t[m_iNumT3];
		}

		for (i = 0; i < m_iNumT3; i++)
		{
			pcAux = ai_pObject->GetIdT3ByIndex(i);
			m_ppcIdT3[i] = new m4char_t[strlen(pcAux) + 1];			
			strcpy(m_ppcIdT3[i], pcAux);
			m_pdVersionT3[i] = ai_pObject->GetVersionT3ByIndex(i);
		}

		//-----------------------------------------------------------
		//Limpiamos antes.
		if (m_ppcIdInclude != NULL) {
			for (i = 0; i < m_iSizeIncludes; i++)
			{
				delete [] m_ppcIdInclude[i];
			}
			delete [] m_ppcIdInclude;
			m_ppcIdInclude = NULL;
		}
		if (m_pdVersionInclude != NULL) {
			delete [] m_pdVersionInclude;
			m_pdVersionInclude = NULL;
		}

		m_iSizeIncludes = ai_pObject->GetNumInclude();

		if (m_iSizeIncludes != 0) {
			m_ppcIdInclude = new m4pchar_t[m_iSizeIncludes];
			m_pdVersionInclude = new m4double_t[m_iSizeIncludes];
		}

		for (i = 0; i < m_iSizeIncludes; i++)
		{
			string sAuxIdInclude = ai_pObject->GetIdIncludeByPos(i);
			if (sAuxIdInclude.size() > 0) {
				m_ppcIdInclude[i] = new m4char_t[sAuxIdInclude.size() + 1];			
				strcpy(m_ppcIdInclude[i], sAuxIdInclude.c_str());
				m_pdVersionInclude[i] = ai_pObject->GetVersionIncludeByPos(i);
			}
		}
		//-----------------------------------------------------------
	}

	return M4_SUCCESS;
}

m4bool_t ClPresentationContainer::MatchCondition (void * ai_pCondition)
{
	m4bool_t bValue = M4_FALSE;
	ClCOCondition * pCondition = (ClCOCondition*)ai_pCondition;

	if (!pCondition)
		return M4_FALSE;

	if (!pCondition->m_pValue)
		return M4_FALSE;

	bValue = M4_FALSE;

	M4_ASSERT(m_dVersion);

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
			{
				m4uint32_t i = 0;
				m4date_t dVersion = 0;
				m4pchar_t pcIdT3 = 0;
				m4date_t dVerCond = ((stIdAndVersion*)(pCondition->m_pValue))->m_dVersion;
				m4pchar_t pcIdCond = ((stIdAndVersion*)(pCondition->m_pValue))->m_pcId;

				// Revisamos todos los metacanales de la presentacion.
				for (i = 0; i < m_iNumT3; i++)
				{
					pcIdT3 = m_ppcIdT3[i];

					if (strcmpi(pcIdT3, pcIdCond) == 0)
					{
						// La presentacion tiene el mismo Metacanal
						dVersion = m_pdVersionT3[i];

						if (dVersion < dVerCond)
						{
							//La ver del metacanal de la presentacion es mas antigüa.
							bValue = M4_TRUE;
							break;
						}
					}
				}
			}
			break;

		case M4_CO_COND_OLDER_DEPENDENCY_VERSION:
			{
				m4uint32_t i = 0;
				m4date_t dVersion = 0;
				m4pchar_t pcIdInclude = 0;
				m4date_t dVerCond = ((stIdAndVersion*)(pCondition->m_pValue))->m_dVersion;
				m4pchar_t pcIdCond = ((stIdAndVersion*)(pCondition->m_pValue))->m_pcId;

				//Buscamos los includes de una include.
				for (i = 0; i < m_iSizeIncludes; i++)
				{
					pcIdInclude = m_ppcIdInclude[i];

					if (strcmpi(pcIdInclude, pcIdCond) == 0) 
					{
						dVersion = m_pdVersionInclude[i];
						if (dVersion < dVerCond)
						{
							//La version del include es mas antigua que la version del objeto que se ha detectado que
							//ha cambiado.
							bValue = M4_TRUE;
							break;
						}
					}
				}
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

