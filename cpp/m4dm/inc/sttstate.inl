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


void ClSttState::DestroyChildList()
{
	if (m_poChildList)
	{
		itClSttStateList it;

		for (it = m_poChildList->begin();it != m_poChildList->end();++it)
		{
			if( *it != NULL )
			{
				delete (*it);
			}
		}

		delete m_poChildList;
		m_poChildList=0;
	}
}

void ClSttState::AddChildList(ClSttState * ai_pChildState)
{
	if (!m_poChildList)
	{
		m_poChildList=new ClSttStateList();
	}

	m_poChildList->push_back(ai_pChildState);
}

void ClSttState::DestroyValuesList()
{
	if (m_poValuesList)
	{
		itClSttValuesList it;

		for (it = m_poValuesList->begin();it != m_poValuesList->end();++it)
		{
			if( *it != NULL )
			{
				delete (*it);
			}
		}

		delete m_poValuesList;
		m_poValuesList=0;
	}
}

void ClSttState::AddValuesList(ClSttValues * ai_pValue)
{
	if (!m_poValuesList)
	{
		m_poValuesList=new ClSttValuesList();
	}

	m_poValuesList->push_back(ai_pValue);
}

void ClSttState::DestroyReadsList()
{
	if (m_poReadsList)
	{
		itClSttReadsList it;

		for (it = m_poReadsList->begin();it != m_poReadsList->end();++it)
		{
			if( *it != NULL )
			{
				delete (*it);
			}
		}

		delete m_poReadsList;
		m_poReadsList=0;
	}
}

void ClSttState::AddReadsList(ClSttReads * ai_pRead)
{
	if (!m_poReadsList)
	{
		m_poReadsList=new ClSttReadsList();
	}

	m_poReadsList->push_back(ai_pRead);
}

void ClSttState::DestroyExecParamsList(void)
{
	if (m_poExecParamsList)
	{
		itm4VariantList it;

		for (it = m_poExecParamsList->begin();it != m_poExecParamsList->end();++it)
		{
			if( *it != NULL )
			{
				delete (*it);
			}
		}

		delete m_poExecParamsList;
		m_poExecParamsList=0;
	}
}

void ClSttState::AddExecParamsList(m4VariantType * ai_pParam)
{
	if (!m_poExecParamsList)
	{
		m_poExecParamsList=new m4VariantList();
	}

	m_poExecParamsList->push_back(ai_pParam);
}

void ClSttState::Reset()
{
	m_oExecData.Reset();
	m_pDBCahe = 0;
	m_uiCSType = 0;
	m_uiChannelCSType = 0;
	m_poChildList = 0;
	m_poValuesList = 0;
	m_poReadsList = 0;
	m_poExecParamsList = 0;
	m_vDate.SetDate(ClActualDate((m4uint8_t)M4CL_TIME_LOCAL_TIME));
	m_dNorm = 0;
};

ClDBTimes::~ClDBTimes()
{
	if (m_pcStatement)
		delete [] m_pcStatement;
}


ClDBCache::~ClDBCache()
{
	ClDBTimesList::iterator it;

	//Borra los objetos creados.
	for (it = m_DBTimesList.begin (); it != m_DBTimesList.end(); it++) 
	{
		if( *it != NULL )
		{
			delete (*it);
		}
	}

	m_pDBTimesActive = 0;
}


void ClSttState::Reset(m4uint32_t ai_hItem, eSpecialItems_t ai_eSpecialItem)
{		
	m_hItem = ai_hItem;
	m_eSpecialItem = ai_eSpecialItem;

	Reset();
};


ClSttState::ClSttState()
{
	Reset(0, M4_VM_STT_SPECIAL_ITEM_NULL);
};

m4return_t ClSttState::AddFrontInterface()
{
	m4char_t acAuxItem[300];

	if (m_uiCSType == M4CL_CSTYPE_REAL_MAPPED)
		return M4_SUCCESS;

	if (m_vIdItem.Type == M4CL_CPP_TYPE_STRING_VAR)
	{
		strcpy(acAuxItem, m_vIdItem.Data.PointerChar);
		strcat(acAuxItem,"_FRONT_INTERFACE");
		m_vIdItem.SetString(acAuxItem);
		m_uiCSType = M4CL_CSTYPE_REAL_MAPPED;
		return M4_SUCCESS;
	}
	else
	{
		return M4_ERROR;
	}
}

m4bool_t ClSttState::IsRealMapped()
{
	//Si es especial, se obtiene en el momento.
	//Si esta en cache no es realmapped (por definicion)
	return ( (m_uiCSType == M4CL_CSTYPE_REAL_MAPPED) ? M4_TRUE : M4_FALSE );
}


m4return_t ClSttState::GetItemIds (m4uint32_t ai_hItem, m4int32_t ai_iRule, ClCompiledMCR *ai_pCMCR, m4VariantType &ao_IdT3, m4VariantType &ao_IdNode, m4VariantType &ao_IdItem, m4VariantType &ao_IdRule)
{
	M4_ASSERT(ai_hItem);
	M4_ASSERT(ai_pCMCR);

	m4uint32_t hNode = ai_pCMCR->GetItemNodeHandle(ai_hItem);

	ao_IdT3.Type = ao_IdNode.Type = ao_IdItem.Type = ao_IdRule.Type = M4CL_CPP_TYPE_STRING_VAR;

    ao_IdT3.SetString((m4pchar_t) ai_pCMCR->GetChannelId());
	ao_IdNode.SetString((m4pchar_t) ai_pCMCR->GetNodeId(hNode));
	m4pchar_t pcItem = (m4pchar_t) ai_pCMCR->GetItemId(ai_hItem);

	ao_IdItem.SetString(pcItem);

	if (ai_iRule < 0)
	{
		ai_iRule = 0;
	}

	ao_IdRule.SetString((m4pchar_t) ai_pCMCR->GetItemFormIdByPosition(ai_hItem, (m4uint16_t)ai_iRule));
	
	return M4_SUCCESS;
}

m4return_t ClSttState::GetItemIds (eSpecialItems_t ai_eItem, m4uint8_t ai_uiCSType, m4uint8_t ai_uiChannelCSType, m4VariantType &ao_IdT3, m4VariantType &ao_IdNode, m4VariantType &ao_IdItem, m4VariantType &ao_IdRule)
{
	M4_ASSERT(ai_eItem != M4_VM_STT_SPECIAL_ITEM_NULL);

	ao_IdT3.Type = ao_IdNode.Type = ao_IdItem.Type = ao_IdRule.Type = M4CL_CPP_TYPE_STRING_VAR;

	m4pchar_t pcItem = 0;

	if (ai_eItem == M4_VM_STT_SPECIAL_ITEM_CREATE_MCR)
	{
		pcItem = "Create M4Object";
	}
	else if (ai_eItem == M4_VM_STT_SPECIAL_ITEM_CREATE_SCR)
	{
		pcItem = "Create Security";
	}
	else if (ai_eItem == M4_VM_STT_SPECIAL_ITEM_CREATE_PRESENTATION)
	{
		pcItem = "Create Presentation";
	}
	else if (ai_eItem == M4_VM_STT_SPECIAL_ITEM_PROXY_SPACE_CREATION)
	{
		pcItem = "Create Proxy Space";
	}
	else if (ai_eItem == M4_VM_STT_SPECIAL_ITEM_PROXY_SPACE_DESTRUCTION)
	{
		pcItem = "Destroy Proxy Space";
	}
	else if (ai_eItem == M4_VM_STT_SPECIAL_ITEM_METADATA_GET_UPDATE_LIST)
	{
		pcItem = "Refresh Cache List";
	}
	else if (ai_eItem == M4_VM_STT_SPECIAL_ITEM_CACHE_REMOVE_OBJECT)
	{
		pcItem = "Remove Cache Object";
	}
	else if (ai_eItem == M4_VM_STT_SPECIAL_ITEM_DESTROY_CHANNEL)
	{
		pcItem = "Destroy M4Object";
	}
	else if (ai_eItem == M4_VM_STT_SPECIAL_ITEM_CREATE_MAP)
	{
		pcItem = "Create Mapping";
	}
	else
	{
		pcItem = "Undefined";
	}

	ao_IdItem.Data.PointerChar = pcItem;

	ao_IdNode.Data.PointerChar = "System Info";
	ao_IdT3.Data.PointerChar = "System Info";
	ao_IdRule.Data.PointerChar = "";
	return M4_SUCCESS;
}


ClSttState::ClSttState(const m4VMState_t& ai_roState)
{

	m4uint32_t		hItem = ai_roState.m_hItem;
	ClAccess		*poAccess = ai_roState.m_poAccess;
	ClCompiledMCR	*pCMCR = poAccess->GetpCMCR();
			
	Reset(hItem, M4_VM_STT_SPECIAL_ITEM_NULL);

	m_uiCSType = pCMCR->GetItemCsType(hItem);

	m4double_t dValue = 0;
	poAccess->GetpChannel()->CsType.Get( dValue );
	m_uiChannelCSType = m4uint8_t( dValue );

	if (m_uiChannelCSType == M4CL_CSTYPE_DEBUG)
	{
		m_uiChannelCSType = M4CL_CSTYPE_FRONT;
	}
	else if (m_uiChannelCSType == M4CL_CSTYPE_BOTH)
	{
		m_uiChannelCSType = M4CL_CSTYPE_BACK;
	}

	GetItemIds (hItem, ai_roState.m_iRule, pCMCR, m_vIdT3, m_vIdNode, m_vIdItem, m_vIdRule);

	m_dNorm = ai_roState.m_poFormula.GetStartDate();
};

ClSttState::ClSttState(eSpecialItems_t ai_eSpecialItem, m4uint8_t ai_uiCSType, m4uint8_t ai_uiChannelCSType)
{
	Reset(0, ai_eSpecialItem);

	m_uiCSType = ai_uiCSType;
	m_uiChannelCSType = ai_uiChannelCSType;

	GetItemIds (ai_eSpecialItem, ai_uiCSType, ai_uiChannelCSType, m_vIdT3, m_vIdNode, m_vIdItem, m_vIdRule);
};

ClSttState::~ClSttState()
{
	if (m_pDBCahe)
		delete m_pDBCahe;

	DestroyChildList();

	DestroyValuesList();
	DestroyReadsList();
	DestroyExecParamsList();
}

m4return_t ClSttState::GetItemIds (m4VariantType &ao_IdT3, m4VariantType &ao_IdNode, m4VariantType &ao_IdItem, m4VariantType &ao_IdRule)
{
	ao_IdT3 = m_vIdT3;
	ao_IdNode = m_vIdNode;
	ao_IdItem = m_vIdItem;
	ao_IdRule = m_vIdRule;

	return M4_SUCCESS;
}


ClDBTimes::ClDBTimes(void)
{
	m_pcStatement = 0;
	m_idLConn = 0;
	m_eOp = 0;
	m_IsInternal = 0;
	
	m_dPrepareTime = m_dExecuteTime = m_dFetchTime = 0;
	m_iNumPrepare = m_iNumFetch = m_iNumExecute = 0;
}


ClDBTimes::ClDBTimes(m4int32_t ai_idLConn, m4int16_t ai_eOp, m4uint8_t ai_IsInternal, m4pcchar_t ai_pccStatement)
{
	M4_ASSERT(ai_pccStatement);

	m_pcStatement = new m4char_t[ strlen(ai_pccStatement)+1 ];
	strcpy(m_pcStatement, ai_pccStatement);

	m_idLConn = ai_idLConn;
	m_eOp = ai_eOp;
	m_IsInternal = ai_IsInternal;
	
	m_dPrepareTime = m_dExecuteTime = m_dFetchTime = 0;
	m_iNumPrepare = m_iNumFetch = m_iNumExecute = 0;
}


ClDBCache::ClDBCache()
{
	//m_itActual = m_DBTimesList.end ();
	m_pDBTimesActive = 0;
}

m4return_t ClDBCache::sttAddDBTime (m4int32_t ai_idLConn, eDBFunctionType_t ai_eFun, m4int16_t ai_eOp, m4uint8_t ai_IsInternal, m4pcchar_t ai_pccStatement, m4double_t ai_dTime)
{
	if (m_pDBTimesActive != 0)
	{
		//Por si es la op activa!!
		if (ai_idLConn != m_pDBTimesActive->m_idLConn)
		{
			ClDBTimesList::iterator it;
			
			m_pDBTimesActive = 0;
			for (it = m_DBTimesList.begin (); it != m_DBTimesList.end(); it++) 
			{
				if ( ai_idLConn == (*it)->m_idLConn )
				{
					m_pDBTimesActive = *it;
					break;
				}
			}
		}
	}

	if (m_pDBTimesActive == 0)
	{
		m_pDBTimesActive = new ClDBTimes(ai_idLConn, ai_eOp, ai_IsInternal, ai_pccStatement);

		m_DBTimesList.push_back ( m_pDBTimesActive );
	}

	if (ai_eFun == M4_VM_STT_DBFUNCTION_FETCH)
	{
		m_pDBTimesActive->m_dFetchTime += ai_dTime;

		m_pDBTimesActive->m_iNumFetch++;

		M4_ASSERT(m_pDBTimesActive->m_eOp == M4_STT_LDB_SELECT);
	}
	else if (ai_eFun == M4_VM_STT_DBFUNCTION_PREPARE)
	{
		m_pDBTimesActive->m_dPrepareTime += ai_dTime;

		if (m_pDBTimesActive->m_iNumPrepare == 0) 
			m_pDBTimesActive->m_iNumPrepare++;
	}
	else if (ai_eFun == M4_VM_STT_DBFUNCTION_EXECUTE)
	{
		m_pDBTimesActive->m_dExecuteTime += ai_dTime;

		m_pDBTimesActive->m_iNumExecute++;
	}
	else
		M4_ASSERT(0);

	return M4_SUCCESS;
}
