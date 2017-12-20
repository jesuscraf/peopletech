//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                mdupdate.cpp   
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
//   
//
//
//==============================================================================

#include "mdupdate.hpp"
#include "dm.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "regi_smp.hpp"
#include "iexecutor.hpp"
#include "m4cachedirres.hpp"
#include "cldates.hpp"
#include "m4objreg.hpp"
//#include "sttlib.hpp"
//#include "csres.hpp"
#include "blocksynchronization.hpp"


ClMDVersion::ClMDVersion(void)
{
}

ClMDVersion::ClMDVersion(m4VariantType & ai_vMDVersion, m4VariantType & ai_vMDType, m4VariantType & ai_vMDId)
{
	m_dMDVersion = ai_vMDVersion.Data.DoubleData;
	m_iMDType = (m4uint16_t) ai_vMDType.Data.DoubleData;

	if (ai_vMDId.Type == M4CL_CPP_TYPE_STRING_VAR)
	{
		m_vMDId.SetString(ai_vMDId.Data.PointerChar);
	}

}

ClMDVersion::~ClMDVersion(void)
{
}

m4return_t ClMDVersion::Serialize(ClGenericIODriver& IOD)
{
	m4return_t ret = M4_SUCCESS;

	ret = IOD.Write(m_dMDVersion);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IOD.Write(m_iMDType);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = m_vMDId.Serialize(IOD);

	if (ret == M4_ERROR)
		return M4_ERROR;

	return M4_SUCCESS;
}

m4return_t ClMDVersion::DeSerialize(ClGenericIODriver& IOD)
{
	m4return_t ret = M4_SUCCESS;

	ret = IOD.Read(m_dMDVersion);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IOD.Read(m_iMDType);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = m_vMDId.DeSerialize(IOD);

	if (ret == M4_ERROR)
		return M4_ERROR;

	return M4_SUCCESS;
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

ClMDUpdateDeltasList::ClMDUpdateDeltasList(void)
{
	m_dLastModificationDate = M4CL_MINUS_INFINITE_IN_JULIAN;
}

ClMDUpdateDeltasList::ClMDUpdateDeltasList(m4date_t ai_dLastModificationDate, m4VariantType &ai_vRepositoryKey)
{
	m_dLastModificationDate = ai_dLastModificationDate;
	m_vRepositoryKey = ai_vRepositoryKey;
}

ClMDUpdateDeltasList::~ClMDUpdateDeltasList(void)
{
	ClMDVersionList::iterator it;

	//Borra los objetos creados.
	for (it = m_oVersionList.begin (); it != m_oVersionList.end(); it++)
	{
		delete (*it);
	}
}

m4return_t ClMDUpdateDeltasList::Serialize(ClGenericIODriver& IOD)
{
	m4return_t ret = M4_SUCCESS;
	m4uint32_t iListSize;
	ClMDVersionList::iterator it;

	ret = IOD.Write(m_dLastModificationDate);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = m_vRepositoryKey.Serialize(IOD);

	if (ret == M4_ERROR)
		return M4_ERROR;

	iListSize = m_oVersionList.size();

	ret = IOD.Write(iListSize);

	if (ret == M4_ERROR)
		return M4_ERROR;

	//Borra los objetos creados.
	for (it = m_oVersionList.begin (); it != m_oVersionList.end(); it++)
	{
		ret = (*it)->Serialize(IOD);
		if (ret == M4_ERROR)
			return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClMDUpdateDeltasList::DeSerialize(ClGenericIODriver& IOD)
{
	m4return_t ret = M4_SUCCESS;
	m4uint32_t iListSize;
	ClMDVersionList::iterator it;
	ClMDVersion * poMDVersion;

	ret = IOD.Read(m_dLastModificationDate);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = m_vRepositoryKey.DeSerialize(IOD);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IOD.Read(iListSize);

	if (ret == M4_ERROR)
		return M4_ERROR;

	//Borra los objetos creados.
	for (iListSize; iListSize > 0; iListSize--)
	{
		poMDVersion = new ClMDVersion();
		ret = poMDVersion->DeSerialize(IOD);
		if (ret == M4_ERROR)
		{
			delete poMDVersion;
			return M4_ERROR;
		}
		m_oVersionList.push_back(poMDVersion);
	}

	return M4_SUCCESS;
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

ClMDUpdateList::ClMDUpdateList(m4date_t	ai_dLastModificationDate, m4uint32_t ai_iListMaxSize)
{
	m_dLastModificationDate = ai_dLastModificationDate;
	m_dFirstModificationDate = 0;
	m_poChList = 0;
	m_poAccListByVer = 0;
	m_poAccListById = 0;
	m_iListMaxSize = ai_iListMaxSize;
	m_vRepositoryKey.SetString("UNDEFINED");
	m_oMutex.Init();
}

ClMDUpdateList::~ClMDUpdateList(void)
{
	ClearList();
}

m4return_t ClMDUpdateList::ClearList(void)
{	
	// Destruye el canal y los access.

	ClMutBlock oBlock(m_oMutex);

	if (m_poAccListByVer)
	{
		m_poAccListByVer->Destroy();
		m_poAccListByVer = 0;
	}

	if (m_poAccListById)
	{
		m_poAccListById->Destroy();
		m_poAccListById = 0;
	}
	
	if (m_poChList)
	{
		m_poChList->Destroy();
		m_poChList = 0;
	}


	return M4_SUCCESS;
}


m4return_t ClMDUpdateList::RefreshList(ClChannelManager * ai_pChannelManager)
{
	// - Crea el canal de MD si no está creado
	// - Obtiene la RepositoryKey
	// - Hace la LoadByDate a partir de LastModificationDate
	// - Compacta los datos para que no haya duplicados
	// - Actualiza LastModificationDate y FirstModificationDate

	//Para evitar problemas en Multithread
	ClMutBlock oBlock(m_oMutex);
	
	m4return_t ret = M4_SUCCESS;
	m4bool_t bChExists = M4_TRUE;
	m4uint32_t iNumRecords = 0;

	m4VariantType vVersion, vRet;

	if (!m_poChList)
	{
		bChExists = M4_FALSE;

		ret = ai_pChannelManager->CreateChannel(m_poChList);

		if (M4_ERROR == ret)
		{
			g_oChLog<<BeginError(M4_CH_CACHEDIR_MD_REFRESH_LIST)<<EndLog;
			return ret;
		}
		
		m_poChList->Role_ID.Set(M4CL_SEC_SUPER_USER);
		ret = m_poChList->BuildFromIdNoSec(M4_CHANNEL_MD_VERSION);

		if (M4_ERROR == ret)
		{
			g_oChLog<<BeginError(M4_CH_CACHEDIR_MD_REFRESH_LIST)<<EndLog;
			m_poChList->Destroy();
			m_poChList = 0;
			return ret;
		}

		m_poAccListByVer = m_poChList->CreateAccess();	
	}

	ret = LoadRepositoryKey();

	if (M4_ERROR == ret)
	{
		g_oChLog<<BeginError(M4_CH_CACHEDIR_MD_REFRESH_LIST)<<EndLog;
		return M4_ERROR;
	}

	ret = LoadListByDate(iNumRecords);

	if (M4_ERROR == ret)
	{
		g_oChLog<<BeginError(M4_CH_CACHEDIR_MD_REFRESH_LIST)<<EndLog;
		return M4_ERROR;
	}

	if (iNumRecords)
	{
		ret = CompactList();

		if (M4_ERROR == ret)
		{
			g_oChLog<<BeginError(M4_CH_CACHEDIR_MD_REFRESH_LIST)<<EndLog;
			return M4_ERROR;
		}
	}

	if (M4_FALSE == bChExists)
	{
		//Añadimos un filtro de ordenación en el iterador Register del node ITEMS
		m4pchar_t apcNames[1] = { M4_ITEM_MD_VERSION_DT_MD_VERSION };
		m4uint8_t aiOrder[1] =	{ M4CL_INDEX_SEARCH_WAY_DESC };

		if ( M4_ERROR == m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Register.Filter.AddSort ( 1, apcNames, aiOrder) )
			return M4_ERROR;		
	}

	//Actualizamos m_dLastModificationDate y m_dFirstModificationDate
	if (iNumRecords)
	{
		m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Register.Begin();
		m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Register.Item[M4_ITEM_MD_VERSION_DT_MD_VERSION].Value.Get(vVersion);
		m_dLastModificationDate = vVersion.Data.DoubleData;

		m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Register.End();
		m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Register.Item[M4_ITEM_MD_VERSION_DT_MD_VERSION].Value.Get(vVersion);
		m_dFirstModificationDate = vVersion.Data.DoubleData;
	}


	return ret;
}

m4return_t ClMDUpdateList::RefreshLastModificationDate(ClChannelManager * ai_pChannelManager)
{
	// - Crea el canal de MD si no está creado
	// - Obtiene la RepositoryKey
	// - Hace la GetLastModificationDate
	// - Actualiza LastModificationDate

	//Para evitar problemas en Multithread
	ClMutBlock oBlock(m_oMutex);
	
	m4return_t ret = M4_SUCCESS;
	m4bool_t bChExists = M4_TRUE;

	if (!m_poChList)
	{
		bChExists = M4_FALSE;

		ret = ai_pChannelManager->CreateChannel(m_poChList);

		if (M4_ERROR == ret)
		{
			g_oChLog<<BeginError(M4_CH_CACHEDIR_MD_REFRESH_LIST)<<EndLog;
			return ret;
		}

		m_poChList->Role_ID.Set(M4CL_SEC_SUPER_USER);
		ret = m_poChList->BuildFromIdNoSec(M4_CHANNEL_MD_VERSION);

		if (M4_ERROR == ret)
		{
			g_oChLog<<BeginError(M4_CH_CACHEDIR_MD_REFRESH_LIST)<<EndLog;
			m_poChList->Destroy();
			m_poChList = 0;
			return ret;
		}

		m_poAccListByVer = m_poChList->CreateAccess();	
	}

	ret = LoadRepositoryKey();

	if (M4_ERROR == ret)
	{
		g_oChLog<<BeginError(M4_CH_CACHEDIR_MD_REFRESH_LIST)<<EndLog;
		return M4_ERROR;
	}

	ret = LoadLastModificationDate();
	
	if (M4_ERROR == ret)
	{
		g_oChLog<<BeginError(M4_CH_CACHEDIR_MD_REFRESH_LIST)<<EndLog;
		return M4_ERROR;
	}

	if (M4_FALSE == bChExists)
	{

		//Añadimos un filtro de ordenación en el iterador Register del node ITEMS
		m4pchar_t apcNames[1] = { M4_ITEM_MD_VERSION_DT_MD_VERSION };
		m4uint8_t aiOrder[1] =	{ M4CL_INDEX_SEARCH_WAY_DESC };
		if ( M4_ERROR == m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Register.Filter.AddSort ( 1, apcNames, aiOrder) )
			return M4_ERROR;		
	}

	return ret;
}

m4return_t ClMDUpdateList::CutList(void)
{
	// - Corta la lista en un tamaño determinado para que como mucho tenga m_iListMaxSize regiustros

	//Para evitar problemas en Multithread
	ClMutBlock oBlock(m_oMutex);
	
	m4return_t ret = M4_SUCCESS;
	m4uint32_t iCount;
	m4VariantType vVersion;

	if (!m_poChList)
		return M4_ERROR;

	//El canal debe estar ordenado DESC por VERSION
	iCount = m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Count();

	//Nos quedamos con los m_iListMaxSize ultimos.
	for (iCount; iCount > m_iListMaxSize; iCount--)
	{
		m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Register[iCount-1].Destroy();
	}

	if (iCount > 0)
	{
		m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Register[iCount-1].Item[M4_ITEM_MD_VERSION_DT_MD_VERSION].Value.Get(vVersion);
		m_dFirstModificationDate = vVersion.Data.DoubleData;
	}

	return ret;
}

m4return_t ClMDUpdateList::CompactList(void)
{
	// - Ordena el canal por IdMD y IdMDType
	// - Para claves repetidas (mismo IdMD y IdMDType) se queda con el registro de mayor MD_VERSION y borra el resto

	if (!m_poChList)
		return M4_ERROR;

	//Ordena por Id, IdType, Version Desc
	if (!m_poAccListById)
	{
		m_poAccListById = m_poChList->CreateAccess();

		//Añadimos un filtro de ordenación en el iterador Register del node ITEMS
		m4pchar_t apcNames[3] = { M4_ITEM_MD_VERSION_ID_MD,
								  M4_ITEM_MD_VERSION_ID_MD_TYPE,
								  M4_ITEM_MD_VERSION_DT_MD_VERSION };					
		m4uint8_t aiOrder[3] =	{ M4CL_INDEX_SEARCH_WAY_ASC,
								  M4CL_INDEX_SEARCH_WAY_ASC,
								  M4CL_INDEX_SEARCH_WAY_DESC };

		if ( M4_ERROR == m_poAccListById->Node[ M4_NODE_MD_VERSION ].RecordSet.Register.Filter.AddSort ( 3, apcNames, aiOrder) )
			return M4_ERROR;		
	}
	else
	{
		m_poAccListById->Node[ M4_NODE_MD_VERSION ].RecordSet.Register.Filter.UpdateFilter();	
	}

	m4uint32_t i, iCount;
	m4VariantType vIdAnt, vTypeAnt, vId, vType;

	iCount = m_poAccListById->Node[ M4_NODE_MD_VERSION ].RecordSet.Count();

	for (i=0; i<iCount; i++)
	{
		//Get de los 2 valores
		m_poAccListById->Node[ M4_NODE_MD_VERSION ].RecordSet.Register[i].Item[M4_ITEM_MD_VERSION_ID_MD].Value.Get(vId);
		m_poAccListById->Node[ M4_NODE_MD_VERSION ].RecordSet.Register[i].Item[M4_ITEM_MD_VERSION_ID_MD_TYPE].Value.Get(vType);

		if ( (vId == vIdAnt) && (vType == vTypeAnt) )
		{
			m_poAccListById->Node[ M4_NODE_MD_VERSION ].RecordSet.Register[i].Destroy();
			iCount--;i--;//Queda ordenado. i sera el que antes era i+1
			
			//Y nos quedamos solo con el de mayor version.
		}
		else
		{
			vIdAnt = vId;
			vTypeAnt = vType;
		}
	}

	//Ordena por version
	//m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Register.Filter.UpdateFilter();

	return M4_SUCCESS;
}

m4return_t ClMDUpdateList::CreateUpdateDeltasList( m4date_t ai_dLastModificationDate, m4VariantType &ai_vRepositoryKey, ClMDUpdateDeltasList *& ao_oMDUpdateDeltasList)
{
	// Crea una estructura de tipo ClMDUpdateDeltasList a partir de los parámetros y del contenido del propio objeto.
	// ClMDUpdateDeltasList contendrá una lista con los cambios de MD desde la fecha ai_dLastModificationDate así como
	// el nuevo LastModificationDate y RepositoryKey.
	// Si ai_dLastModificationDate es menor que m_dFirstModificationDate devuelve la lista vacia (esta fuera de rango)
	ClMutBlock oBlock(m_oMutex);
		
	m4VariantType vVersion, vId, vType;
	m4uint32_t iCount, i;
	ClMDVersion * poMDVersion;
	ao_oMDUpdateDeltasList = new ClMDUpdateDeltasList(m_dLastModificationDate, m_vRepositoryKey);

	if ( (ai_dLastModificationDate < m_dFirstModificationDate) || (m_vRepositoryKey != ai_vRepositoryKey) || (ai_dLastModificationDate == m_dLastModificationDate) || (m_poChList == 0) || (ai_dLastModificationDate == M4CL_PLUS_INFINITE_IN_JULIAN) )
	{
		//No creamos lista de cambios si LastModDate es demasiado pequeña o cambio el repositorio >= Cliente limpia todo
		//o la lista no tiene ningun cambio o LastModDate es la misma =>Cliente no hace nada.
	}
	else
	{
		iCount = m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Count();

		//Nos quedamos con los m_iListMaxSize ultimos.
		for (i = 0; i < iCount; i++)
		{
			m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Register[i].Item[M4_ITEM_MD_VERSION_DT_MD_VERSION].Value.Get(vVersion);
			m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Register[i].Item[M4_ITEM_MD_VERSION_ID_MD].Value.Get(vId);
			m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Register[i].Item[M4_ITEM_MD_VERSION_ID_MD_TYPE].Value.Get(vType);

			if (vVersion.Data.DoubleData <= ai_dLastModificationDate)
				break;

			poMDVersion = new ClMDVersion(vVersion, vType, vId);
			ao_oMDUpdateDeltasList->m_oVersionList.push_back(poMDVersion);
		}
	}


	return M4_SUCCESS;
}


m4return_t ClMDUpdateList::LoadListByDate(m4uint32_t & ao_iNumRecords)
{
	// - Carga el canal con los cambios de metadatos a partir de m_dLastModificationDate.
	m4return_t ret = M4_SUCCESS;
	m4VariantType vVersion, vRet;
	m4uint32_t iCountBefore, iCountAfter;
	m4uint32_t iStackCountBefore, iStackCountAfter;

	ao_iNumRecords = 0;

	if (!m_poChList)
	{
		return M4_ERROR;
	}

	//cargamos el canal a partir de la fecha que tenemos
	vVersion.Type = M4CL_CPP_TYPE_DATE;
	vVersion.Data.DoubleData = m_dLastModificationDate;
	m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Item[M4_ITEM_MD_VERSION_PARAM].Value.Set(vVersion);

	iCountBefore = m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Count();

	//Miro a ver si me dejo algun valor en la pila.
	iStackCountBefore = m_poAccListByVer->GetpExecutor()->Stack.Count();

	ret = m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Item[M4_METHOD_MD_VERSION_LOADBYDATE].Call();

	if (M4_ERROR == ret)
	{
		g_oChLog<<BeginError(M4_CH_CACHEDIR_MD_LOAD_UPDATE_LIST)<<EndLog;
		return M4_ERROR;
	}

	if (M4_ERROR == ret)
	{
		//SETCODEF
	}

	iStackCountAfter = m_poAccListByVer->GetpExecutor()->Stack.Count();

	while (iStackCountAfter > iStackCountBefore)
	{
		//Saco de la pila
		m_poAccListByVer->GetpExecutor()->Stack.Pop (vRet);
		iStackCountAfter--;
	}

	iCountAfter = m_poAccListByVer->Node[ M4_NODE_MD_VERSION ].RecordSet.Count();

	ao_iNumRecords = iCountAfter - iCountBefore;

	return ret;
}

m4return_t ClMDUpdateList::LoadRepositoryKey(void)
{
	// - Obtiene la nueva RepositoryKey que proviene de
	//		Version: APP_PARAMS-> MIND-VERSION-TEC-MIND.
	//		Correction: APP_PARAMS-> MIND-REVISION-TEC-MIND.
	//		ConnStrinng: Del environment.
	// RepositoryKey = ConnStrinng&Version&Correction

	m4return_t ret = M4_SUCCESS;
	m4VariantType vRepositoryKey, vRet;
	m4uint32_t iStackCountBefore, iStackCountAfter;

	if (!m_poChList)
	{
		return M4_ERROR;
	}

	//Miro a ver si me dejo algun valor en la pila.
	iStackCountBefore = m_poAccListByVer->GetpExecutor()->Stack.Count();

	ret = m_poAccListByVer->Node[ M4_NODE_MD_GENERAL_DATA ].RecordSet.Item[M4_METHOD_MD_GENERAL_GET_REPOSITORY_KEY].Call();

	if (M4_ERROR == ret)
	{
		g_oChLog<<BeginError(M4_CH_CACHEDIR_MD_GET_REPOSITORY_KEY)<<EndLog;
		return M4_ERROR;
	}

	iStackCountAfter = m_poAccListByVer->GetpExecutor()->Stack.Count();

	while (iStackCountAfter > iStackCountBefore)
	{
		//Saco de la pila
		m_poAccListByVer->GetpExecutor()->Stack.Pop (vRet);
		iStackCountAfter--;
	}

	ret = m_poAccListByVer->Node[ M4_NODE_MD_GENERAL_DATA ].RecordSet.Item[M4_ITEM_MD_GENERAL_REPOSITORY_KEY].Value.Get(vRepositoryKey);

	if ( (M4_ERROR == ret) || (vRepositoryKey.Type != M4CL_CPP_TYPE_STRING_VAR) )
	{
		vRepositoryKey.SetString("UNKNOWN");
	}

	//En vRepositoryKey tenemos Version&Correction
	
	//Obtenemos ConnString
	m4pcchar_t pcConcString = 0;
	m4pchar_t pcAux;
	m4pcchar_t	pcKeyFragment = 0;
	m4char_t	acKeyFragment[2 * M4_MAX_LEN_MD5 + 1];
	m4int_t		iLength = -1;

	pcConcString = m_poChList->GetpChannelManager()->GetEnvironment()->GetTempConnString();

	if (pcConcString == 0)
	{
		pcKeyFragment = "UNDEFINED";
	}
	else
	{
		// bg 0314737
		ret = EncryptOneWayMD5( pcConcString, strlen( pcConcString ) * sizeof( m4char_t ), acKeyFragment, 2 * M4_MAX_LEN_MD5, iLength );
		
		if( ret != M4_SUCCESS )
		{
			return M4_ERROR;
		}

		pcKeyFragment = acKeyFragment;
	}

	pcAux = new m4char_t[strlen(pcKeyFragment) + strlen(vRepositoryKey.Data.PointerChar) + 2];

	sprintf(pcAux, "%s&%s", pcKeyFragment, vRepositoryKey.Data.PointerChar);

	strupr(pcAux);

	m_vRepositoryKey.SetString(pcAux);

	delete [] pcAux;

	return ret;
}

m4return_t ClMDUpdateList::LoadLastModificationDate(void)
{
	// - Obtiene la nueva m_dLastModificationDate.

	m4return_t ret = M4_SUCCESS;
	m4VariantType vLastModificationDate, vRet;
	m4uint32_t iStackCountBefore, iStackCountAfter;

	if (!m_poChList)
	{
		return M4_ERROR;
	}

	//Miro a ver si me dejo algun valor en la pila.
	iStackCountBefore = m_poAccListByVer->GetpExecutor()->Stack.Count();
	
	ret = m_poAccListByVer->Node[ M4_NODE_MD_GENERAL_DATA ].RecordSet.Item[M4_METHOD_MD_GENERAL_GET_MAX_DT_MD_VERSION].Call();

	if (M4_ERROR == ret)
	{
		g_oChLog<<BeginError(M4_CH_CACHEDIR_MD_LOAD_UPDATE_LIST)<<EndLog;
		return M4_ERROR;
	}

	iStackCountAfter = m_poAccListByVer->GetpExecutor()->Stack.Count();
	
	while (iStackCountAfter > iStackCountBefore)
	{
		//Saco de la pila
		m_poAccListByVer->GetpExecutor()->Stack.Pop (vRet);
		iStackCountAfter--;
	}

	ret = m_poAccListByVer->Node[ M4_NODE_MD_GENERAL_DATA ].RecordSet.Item[M4_ITEM_MD_GENERAL_MAX_DT_MD_VERSION].Value.Get(vLastModificationDate);

	if ( (M4_ERROR == ret) || (vLastModificationDate.Type != M4CL_CPP_TYPE_DATE) )
	{
		g_oChLog<<BeginError(M4_CH_CACHEDIR_MD_LOAD_UPDATE_LIST)<<EndLog;
		return M4_ERROR;
	}
	else
	{
		m_dLastModificationDate = vLastModificationDate.Data.DoubleData;
	}

	return ret;
}
