// CPropColDef.cpp : Implementation of CCPropColDef
#include "stdafx.h"
#include "m4prop.h"
#include "propdata.h"
#include "cpropcoldef.h"
#include "cdefprop.h"
#include "dataproperties.h"
#include "m4unicode.hpp"


/////////////////////////////////////////////////////////////////////////////
// CCPropColDef
/*inline UINT HashKey(char * Key)
{
	UINT nHash = 0;
	while (*Key)
		nHash = (nHash<<5) + nHash + *Key++;
	return nHash;
}*/

DefProp * CCPropColDef::GetDefProp(char * key)
{
	DefProp * p = NULL;

	if (m_ppHashTable)
	{
		//Calculamos Hash
		char * cTemp = (char*)key;
		int nHash = HashKey(cTemp) % m_nHashSize;
		p = m_ppHashTable[nHash];
		while (p && strcmp(p->Key, key))
		{
			p = p->pNextHash;
		}
	}
	
	return p;
}

DefProp * CCPropColDef::GetDefProp(long nId)
{
	if (nId > 0 && nId <= m_nCount)
	{
		DefProp * p = m_pNodeHead;
		long i = 1;
		
		while(p && i < nId)
		{
			p = p->pNextSeq;
			i++;
		}
		return p;
	}
	
	return NULL;
	
}

STDMETHODIMP CCPropColDef::RemoveAll()
{
	FreeAll();
	return S_OK;
}

STDMETHODIMP CCPropColDef::Remove(VARIANT index)
{
	USES_CONVERSION;
	
	if (index.vt == VT_BSTR)
	{
		char* pTemp = M4VBToCpp(index.bstrVal);
		RemovePropByKey(pTemp);
		delete pTemp;
	}
	else if (index.vt == VT_UI2)
	{
		RemovePropByPos((long)index.bVal);
	}
	else if (index.vt == VT_I2)
	{
		RemovePropByPos((long)index.iVal);
	}
	else if (index.vt == VT_I4)
	{
		RemovePropByPos((long)index.lVal);
	}
	else
	{
		return E_INVALIDARG;
	}

	return S_OK;
}

STDMETHODIMP CCPropColDef::get_Item(VARIANT index, ICDefProp **pvar)
{

	if (!pvar) return E_INVALIDARG;

	// Es un puntero OUT
	*pvar = NULL;
	//if (*pvar) RELEASE((*pvar));
		
	USES_CONVERSION;

	DefProp * propItem = NULL;

	// Obtenemos el elemento y construimos su objeto Proxy
	if (index.vt == VT_BSTR)
	{
		char* pTemp = M4VBToCpp(index.bstrVal);
		propItem = GetDefProp(pTemp);
		delete pTemp;
	}
	else if (index.vt == VT_UI2)
	{
		propItem = GetDefProp((long)index.bVal);
	}
	else if (index.vt == VT_I2)
	{
		propItem = GetDefProp((long)index.iVal);
	}
	else if (index.vt == VT_I4)
	{
		propItem = GetDefProp((long)index.lVal);
	}

	if (!propItem)
	{
		return E_INVALIDARG;
	}

	CComObject<CCDefProp> * pProp =NULL;
	HRESULT hr = CComObject<CCDefProp>::CreateInstance(&pProp);

	if (hr == S_OK)
	{
		pProp->SetData(propItem);
		hr = pProp->QueryInterface(pvar);
	}

	return hr;
}

STDMETHODIMP CCPropColDef::get_Count(long *pVal)
{
	*pVal = m_nCount;
	return S_OK;
}

STDMETHODIMP CCPropColDef::Add(ICDefProp * pNew, VARIANT index)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCPropColDef::get__NewEnum(LPUNKNOWN *ppVal)
{
    HRESULT hr = S_OK;

    if (ppVal == NULL)
	    return E_POINTER;
    *ppVal = NULL;

    _CComCollEnum *pEnum = NULL;
    ATLTRY(pEnum = new CComObject<_CComCollEnum>)
    if (pEnum == NULL)
	    return E_OUTOFMEMORY;

    // allocate an initialize a vector of VARIANT objects
	unsigned int uiSize = sizeof(VARIANT) * m_nCount;
    VARIANT* pVar = (VARIANT*) new VARIANT[m_nCount];
	memset(pVar, '\0', uiSize);

	// Construimos los objetos Proxies
    DefProp * p = m_pNodeHead;
	CComObject<CCDefProp> * pIP = NULL;
	int i = 0;
	while(p)
	{
		// Creamos el objeto Proxy
		hr = CComObject<CCDefProp>::CreateInstance(&pIP);		
		pIP->SetData(p);
		pVar[i].vt = VT_DISPATCH;
		pIP->QueryInterface(IID_IUnknown, (void**)&pVar[i].pdispVal);
		pIP = NULL;
		i++;
		p = p->pNextSeq;
	}

    // copy the array of VARIANTs
	hr = pEnum->Init(&pVar[0],
					 &pVar[m_nCount],
					 reinterpret_cast<IUnknown*>(this), AtlFlagCopy);
    if (FAILED(hr))
    {
	    delete pEnum;
    }
	else
	{
		hr = pEnum->QueryInterface(IID_IUnknown, (void**)ppVal);
		if (FAILED(hr))
			delete pEnum;
	}

	// Iterate through the VARIANT pointer array clearing each VARIANT
    i = 0;
    while ( i < m_nCount )
    {
		VariantClear(&pVar[i]);
		i++;
    }

	// Now free the memory for the array of VARIANT pointers.
	if(pVar)
	{
		delete pVar;
		pVar = NULL;
	}

	return hr; 
};


void CCPropColDef::InitHashTable()
{
	free(m_ppHashTable);
	m_ppHashTable = (DefProp**)malloc(m_nHashSize*sizeof(DefProp*));
	memset(m_ppHashTable, 0, m_nHashSize*sizeof(DefProp*));
}

void CCPropColDef::FreeAll()
{
	DefProp * p = NULL;
	while(m_pNodeHead)
	{
		p = m_pNodeHead;
		m_pNodeHead = m_pNodeHead->pNextSeq;
		FreeBlock(p);
	}

	free(m_ppHashTable);
	m_ppHashTable = NULL;
	m_pLastNode = NULL;
	m_nCount = 0;
	m_pNodeHead = NULL;
}

void CCPropColDef::RemovePropByKey(const char *key)
{

	DefProp * p = NULL;
	DefProp * pBefore = NULL;

	if (m_ppHashTable)
	{
		//Calculamos Hash
		char * cTemp = (char*)key;
		int nHash = HashKey(cTemp) % m_nHashSize;
		p = m_ppHashTable[nHash];
		pBefore = p;
		if (p && strcmp(p->Key, key) == 0)
		{
			// Es el primero
			m_ppHashTable[nHash] = p->pNextHash;
			RemovePropByPtr(p);
			FreeBlock(p);
			m_nCount--;
			return;
		}
		while(p && strcmp(p->Key, key) !=0)
		{
			pBefore = p;
			p = p->pNextHash;
		}
		if (p)
		{
			pBefore->pNextHash = p->pNextHash;
			RemovePropByPtr(p);
			FreeBlock(p);
			m_nCount--;
		}
	}
}

void CCPropColDef::RemovePropByPos(long nIndex)
{
	DefProp * pBefore = NULL;
	if (nIndex > 0 && nIndex <= m_nCount)
	{
		DefProp * p = m_pNodeHead;
		if (nIndex == 1)
		{
			m_pNodeHead = m_pNodeHead->pNextSeq;
			m_pLastNode = NULL;
			if (p->Key)
			{
				RemovePropByKey(p->Key);
			}
			else
			{
				FreeBlock(p);
				m_nCount--;
				return;
			}
		}
		else
		{
			for (int i = 1; i<nIndex; i++)
			{
				pBefore = p;
				p = p->pNextSeq;
			}
			if (p)
			{
				//m_nCount--;
				if (p == m_pLastNode)
					m_pLastNode = pBefore;
				pBefore->pNextSeq = p->pNextSeq;
				if (p->Key)
				{
					RemovePropByKey(p->Key);
				}
				else
				{
					m_nCount--;
					FreeBlock(p);
				}
			}
		}
	}

}

void CCPropColDef::RemovePropByPtr(DefProp *pRemove)
{
	DefProp * p = m_pNodeHead;
	
	if (pRemove == p)
	{
		m_pNodeHead = p->pNextSeq;
		if (m_nCount == 1)
		{
			m_pLastNode = NULL;
		}
		//m_nCount--;
		return;
	}

	DefProp * pBefore = p;
	p = p->pNextSeq;
	
	while(p && p != pRemove)
	{
		pBefore = p;
		p = p->pNextSeq;
	}

	if (p)
	{
		if (m_pLastNode == p)
		{
			m_pLastNode = pBefore;
		}
		pBefore->pNextSeq = p->pNextSeq;
		//m_nCount--;
	}
}

HRESULT CCPropColDef::AddDefProp(DefProp *pDefProp, char * strKey)
{
	DefProp * pNode = NULL;
	
	if (pDefProp)
	{
		if (strKey)
		{
			//long nId = GetIdFromPropName(strKey);
			pNode = GetDefProp(strKey);
			if (pNode)
			{
				return E_FAIL;
			}
			long nLength = strlen(strKey);
			pDefProp->Key = (char*) malloc(nLength+1);
			strcpy(pDefProp->Key, strKey);
		}
		AddPropToHash(pDefProp); 
		m_nCount++;
		if (m_nCount == 1)
		{
			m_pNodeHead = pDefProp;
			m_pLastNode = pDefProp;
		}
		else
		{
			if (m_pLastNode) m_pLastNode->pNextSeq = pDefProp;
			m_pLastNode = pDefProp;
		}
		return S_OK;
	}
	return E_INVALIDARG;
}

void CCPropColDef::AddPropToHash(DefProp *pDefProp)
{
	//Calculamos Hash
	int nHash = HashKey(pDefProp->Key) % m_nHashSize;

	int i = 0;
	if (m_ppHashTable == NULL) InitHashTable();
	if (m_ppHashTable[nHash])
	{
		DefProp * pT = m_ppHashTable[nHash];
		while(pT->pNextHash)
		{
			pT = pT->pNextHash;
		}
		pT->pNextHash = pDefProp;
	}
	else
	{
		m_ppHashTable[nHash] = pDefProp;
		pDefProp->pNextHash = NULL;
	}
}

void CCPropColDef::Reset()
{
	// Se resetean las propiedades a sus valores por defecto
	DefProp * p = m_pNodeHead;
	IM4FontHolder * pFont = NULL;
	CComObject<CDataProperties> * pCData = NULL;
	IDataProperties * pData = NULL;
	ITraductProp * pTraduct = NULL;
	DefProp * pRemove = NULL;
	
	while(p)
	{
		switch(p->Type)
		{

			case FE_FONT:
				RELEASE((p->Value.pdispValue));
				pFont = static_cast<IM4FontHolder*>(p->DefValue.pdispValue);
				pFont->Clone((IM4FontHolder**)&p->Value.pdispValue);
				break;
			case FE_DATAPROP:
				// Creamos uno vacío
				RELEASE((p->Value.pdispValue));
				CComObject<CDataProperties>::CreateInstance(&pCData);
				pCData->QueryInterface(IID_IDataProperties, (void**)&p->Value.pdispValue);
				break;
			case FE_TRADUCTPROP:
				pTraduct = static_cast<ITraductProp*>(p->Value.pdispValue);
				pTraduct->AllValues(NULL);
				break;
			case FE_NUMBER:
			case FE_COLOR:
			case FE_CONSTRUCTIF:
			case FE_GRANTSTYPE:
				p->Value.lValue = p->DefValue.lValue;
				break;
			case FE_STRING:
			case FE_DATE:
			case FE_IMAGE:
			case FE_ALIGNTYPE:
			case FE_LN4_CODEARGS:
				free(p->Value.pstrValue);
				p->Value.pstrValue = NULL;
				if (p->DefValue.pstrValue)
				{
					p->Value.pstrValue = (char*)malloc(strlen(p->DefValue.pstrValue)+1);
					strcpy(p->Value.pstrValue, p->DefValue.pstrValue);
				}
				break;
			case FE_BOOLEAN:
				p->Value.boolValue = p->DefValue.boolValue;
				break;
			default:
				// No es de tipo conocido, hay que eliminarla
				pRemove = p;
				p = NULL;
				break;
		}			
		if (p)
			p = p->pNextSeq;
	}

	while (pRemove)
	{
		DefProp * pR2 = pRemove;
		pRemove = pRemove->pNextSeq;
		RemovePropByKey(pR2->Key);
	}
}


HRESULT CCPropColDef::ResetTradIdentifiers()
{
	ITraductProp * pTraduct = NULL;
	DefProp * p = m_pNodeHead;

	while(p)
	{
		switch(p->Type)
		{
			case FE_TRADUCTPROP:
				pTraduct = static_cast<ITraductProp*>(p->Value.pdispValue);
				pTraduct->ResetIdentifier();
				break;
			default:
				// No hacemos nada
				break;
		}			
		if (p)
		p = p->pNextSeq;
	}

	return S_OK;
}
