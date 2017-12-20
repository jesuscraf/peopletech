/////////////////////////////////////////////////////////////////////////////
// CColTemplate

#ifndef __CCOLTEMPLATE_H__
#define __CCOLTEMPLATE_H__

#define TEMPLATE_BLOCK_SIZE 20
#define TEMPLATE_HASH_SIZE 17

template<class TYPE, class BASE> 
class CColTemplate : public BASE
{
	///                     tipo expuesto           este tipo   Este IID        LibID
	//	public CColTemplate<[Ixxxxx], IDispatchImpl<Iyyyyyyyyy, &IID_Iyyyyyy, &LIBID_zzzzzzz> >
	//
	typedef CComEnum<CComIEnum<VARIANT>, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT> > _CComCollEnum;

public:
	CColTemplate()
	{
		m_nHashSize = TEMPLATE_HASH_SIZE;
		m_pNodeHead = NULL;
		m_pLastNode = NULL;
		m_nCount = 0;
		m_ppHashTable = NULL;
	}
	virtual ~CColTemplate()
	{
		FreeAll();
	}

private:
	long m_nHashSize;
	long m_nCount;
	DefProp * m_pNodeHead;
	DefProp * m_pLastNode;
	DefProp * * m_ppHashTable;

public:

STDMETHOD(Remove)(VARIANT index)
{
	DefProp * p = NULL;
	
	if (index.vt == VT_BSTR)
	{
		RemoveNodeByKey(index.bstrVal);
	}
	else if (index.vt == VT_UI2)
	{
		RemoveNodeByPos((long)index.bVal);
	}
	else if (index.vt == VT_I2)
	{
		RemoveNodeByPos((long)index.iVal);
	}
	else if (index.vt == VT_I4)
	{
		RemoveNodeByPos((long)index.lVal);
	}
	else
		return E_INVALIDARG;

	return S_OK;

}
	
	
STDMETHOD(get_Item)(VARIANT index, TYPE **pvar)
{
	DefProp * p = NULL;
	
	if (index.vt == VT_BSTR)
	{
		p = GetNodeByKey(index.bstrVal);
	}
	else if (index.vt == VT_UI2)
	{
		p = GetNodeByPos((long)index.bVal);
	}
	else if (index.vt == VT_I2)
	{
		p = GetNodeByPos((long)index.iVal);
	}
	else if (index.vt == VT_I4)
	{
		p = GetNodeByPos((long)index.lVal);
	}
	RELEASE((*pvar));
	if (p)
	{
		// Aquí hay que crear el Proxy
		CComObject<CCDefProp> * pDef = NULL;
		if (CComObject<CCDefProp>::CreateInstance(&pDef) == S_OK)
		{
			pDef->SetData(p);
			pDef->QueryInterface(pvar);
		}
	}
	else
		return E_INVALIDARG;

	return S_OK;

}

STDMETHOD(get_Count)(long *pVal)
{
	*pVal = m_nCount;
	return S_OK;
};

STDMETHODIMP get__NewEnum(LPUNKNOWN *ppVal)
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

	// Set the VARIANTs that will hold the TYPE Interface pointers.
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
					 reinterpret_cast<TYPE*>(this), AtlFlagCopy);
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

STDMETHODIMP AddData(DefProp * pData, VARIANT index)
{
	// Primero lo buscamos, si existe, marchando una de error.
	DefProp * pNode = NULL;
	
	if (pData)
	{
		if (index.vt == VT_BSTR)
		{
			pNode = GetNodeByKey(index.bstrVal);
			if (pNode)
			{
				return E_FAIL;
			}

			// Lo añadimos a la tabla de hash;
			pData->Key = ::SysAllocString(index.bstrVal);
			AddNodeToHash(pData); 
			m_nCount++;
			if (m_nCount == 1)
			{
				m_pNodeHead = pData;
				m_pLastNode = pData;
			}
			else
			{
				if (m_pLastNode) m_pLastNode->pNextSeq = pData;
				m_pLastNode = pData;
			}
			//FreeNode(pNode);
			return S_OK;
		}
		// Se añade a lo bestia
		m_nCount++;
		if (m_nCount == 1)
		{
			m_pNodeHead = pData;
			m_pLastNode = pData;
		}
		else
		{
			if (m_pLastNode) m_pLastNode->pNextSeq = pData;
			m_pLastNode = pData;
		}
		return S_OK;
	}
	return E_INVALIDARG;

}


/*STDMETHODIMP Add(TYPE *pIType, VARIANT index)
{
	// Primero lo buscamos, si existe, marchando una de error.
	CNode * pNode = NULL;
	
	if (pIType)
	{
		if (index.vt == VT_BSTR)
		{
			pNode = GetNodeByKey(index.bstrVal);
			if (pNode)
			{
				return E_FAIL;
			}
			// Ya lo podemos entrar
			pNode = GetNewNode();
			pNode->pData = pIType;
			pNode->pData->AddRef();
			pNode->Key = ::SysAllocString(index.bstrVal);
			// Lo añadimos a la tabla de hash;
			AddNodeToHash(pNode); 
			m_nCount++;
			if (m_nCount == 1)
			{
				m_pNodeHead = pNode;
				m_pLastNode = pNode;
			}
			else
			{
				if (m_pLastNode) m_pLastNode->pNextSeq = pNode;
				m_pLastNode = pNode;
			}
			//FreeNode(pNode);
			return S_OK;
		}
		// Se añade a lo bestia
		pNode = GetNewNode();
		pNode->pData = pIType;
		pNode->pData->AddRef();
		m_nCount++;
		if (m_nCount == 1)
		{
			m_pNodeHead = pNode;
			m_pLastNode = pNode;
		}
		else
		{
			if (m_pLastNode) m_pLastNode->pNextSeq = pNode;
			m_pLastNode = pNode;
		}
		return S_OK;
	}
	return E_INVALIDARG;
}*/


STDMETHOD(RemoveAll)()
{
	FreeAll();
	return S_OK;
}


void FreeAll()
{
	// Hay que deshacerse de todo.
	
	// Ahora la emprendemos con los enganchados de forma sequencial
	while(m_pNodeHead)
	{
		DefProp * p = m_pNodeHead;
		m_pNodeHead = m_pNodeHead->pNextSeq;
		FreeBlock(p);
	}

	// Liberamos espacio Hash
	free(m_ppHashTable);
	m_ppHashTable = NULL;
	m_pLastNode = NULL;
	m_nCount = 0;
	m_pNodeHead = NULL;
}

private:


void InitHashTable()
{
	free(m_ppHashTable);
	m_ppHashTable = (DefProp**)malloc(m_nHashSize*sizeof(DefProp*));
	memset(m_ppHashTable, 0, m_nHashSize*sizeof(DefProp*));
}

void AddNodeToHash(DefProp * p)
{
	//Calculamos Hash
	int nHash = HashKey(p->Key) % m_nHashSize;

	int i = 0;
	if (m_ppHashTable == NULL) InitHashTable();
	if (m_ppHashTable[nHash])
	{
		DefProp * pT = m_ppHashTable[nHash];
		while(pT->pNextHash)
		{
			pT = pT->pNextHash;
		}
		pT->pNextHash = p;
	}
	else
	{
		m_ppHashTable[nHash] = p;
		p->pNextHash = NULL;
	}
}

DefProp * GetNodeByKey(BSTR Key)
{
	DefProp * p = NULL;

	if (m_ppHashTable)
	{
		//Calculamos Hash
		int nHash = HashKey(Key) % m_nHashSize;
		p = m_ppHashTable[nHash];
		while(p && wcscmp(p->Key, Key) !=0)
		{
			p = p->pNextHash;
		}
	}
	return p;
}

DefProp * GetNodeByPos(long nPos)
{
	DefProp * p = NULL;
	if (nPos > 0 && nPos <= m_nCount)
	{
		p = m_pNodeHead;
		for (int i = 1; i< nPos; i++)
		{
			p = p->pNextSeq;
		}
	}

	return p;
}

int HashKey(BSTR Key)
{
	BSTR innerKey = Key;
	int nValue = 0;
	while(*innerKey)
	{
		nValue += *innerKey;
		innerKey++;
	}
	return nValue;
}

void RemoveNodeByPtr(DefProp * pRemove)
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

void RemoveNodeByPos(long nIndex)
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
				RemoveNodeByKey(p->Key);
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
					RemoveNodeByKey(p->Key);
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

void RemoveNodeByKey(BSTR Key)
{
	DefProp * p = NULL;
	DefProp * pBefore = NULL;

	if (m_ppHashTable)
	{
		//Calculamos Hash
		int nHash = HashKey(Key) % m_nHashSize;
		p = m_ppHashTable[nHash];
		pBefore = p;
		if (p && wcscmp(p->Key, Key) == 0)
		{
			// Es el primero
			m_ppHashTable[nHash] = p->pNextHash;
			RemoveNodeByPtr(p);
			FreeBlock(p);
			m_nCount--;
			return;
		}
		while(p && wcscmp(p->Key, Key) !=0)
		{
			pBefore = p;
			p = p->pNextHash;
		}
		if (p)
		{
			pBefore->pNextHash = p->pNextHash;
			RemoveNodeByPtr(p);
			FreeBlock(p);
			m_nCount--;
		}
	}

}


};

#endif