/////////////////////////////////////////////////////////////////////////////
// Colección Personalizada con ordenación por petición de datos

#ifndef __CGOCOLLECTION_H__
#define __CGOCOLLECTION_H__

#include "m4define.hpp"

#define BY_LONG 1
#define BY_BSTR	2
#define HASH_SIZE 17


inline UINT HashKey(char * Key)
{
	UINT nHash = 0;
	while (*Key)
		nHash = (nHash<<5) + nHash + *Key++;
	return nHash;
}


//##ModelId=396DF30E0195
template<class TYPE>
struct CMinNode
{
	//##ModelId=396DF30E0219
	CMinNode*	pNext;
	//##ModelId=396DF30E0205
	CMinNode*	pNextHash;
	//##ModelId=396DF30E01EF
	CMinNode*	pPrev;
	//##ModelId=396DF30E01DB
	TYPE*		pData;
	//##ModelId=396DF30E01D3
	BSTR		bstrKey;
	//##ModelId=396DF30E01BF
	BSTR		bstrOrder;
	//##ModelId=396DF30E01AB
	long		nLong;
};


//##ModelId=396DF30E02D6
template<class TYPE, class BASE> 
class CGoCollection : public BASE
{

	public:
// Construction
	//##ModelId=396DF30E0363
	CGoCollection() 
    {	
        m_nCount = 0;
        m_pNodeHead = m_pNodeTail = NULL;
		m_nOrderType = 0;
		m_ppHashTable = (CMinNode<TYPE>**)malloc(HASH_SIZE*sizeof(CMinNode<TYPE>*));
		memset(m_ppHashTable, 0, HASH_SIZE*sizeof(CMinNode<TYPE>*));
    };

	//##ModelId=396DF30E0358
    virtual ~CGoCollection()
    {
	    RemoveAll();
		free(m_ppHashTable);
    };

	// count of elements
	STDMETHOD(Add)(TYPE *pIType, VARIANT index);
	STDMETHOD(get_Count)(long *pVal);
	STDMETHOD(get_Item)(VARIANT index, TYPE **pvar);
	STDMETHOD(get__NewEnum)(LPUNKNOWN *pVal);
	STDMETHOD(Remove)(VARIANT index);
	STDMETHOD(OrderBy)(BSTR bstrOrderKey);

	STDMETHOD(GetPropertyValue)(BSTR bstrName, VARIANT * pVar, IDispatch * pDis);
	/*


		[id(1), helpstring("Add")] HRESULT Add([in]TYPE *pI, [in,optional]VARIANT index);
		[propget, id(2), helpstring("Count")] HRESULT Count([out, retval] long *pVal);
		[propget, id(3), helpstring("Item")] HRESULT Item([in]VARIANT index, [out, retval]TYPE **pvar);
		[propget, id(DISPID_NEWENUM), restricted, helpstring("_NewEnum")] HRESULT _NewEnum([out, retval] LPUNKNOWN *pVal);
		[id(4), helpstring("Remove")] HRESULT Remove([in]VARIANT index);
		[id(5), helpstring("method Order")] HRESULT OrderBy([in] BSTR bstrOrderKey);


	*/

    //typedef CComEnum<CComIEnum<TYPE*>, &IID_IEnumVARIANT, TYPE*, _CopyInterface<TYPE> > _CComCollEnum;
    typedef CComEnum<CComIEnum<VARIANT>, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT> > _CComCollEnum;

// Implementation

	//##ModelId=396DF30E0350
	CMinNode<TYPE> *		m_pNodeHead;
	//##ModelId=396DF30E0328
	CMinNode<TYPE> *		m_pNodeTail;
	//##ModelId=396DF30E0312
	int				m_nCount;
	//##ModelId=396DF30E0308
	long			m_nOrderType;

	// remove all elements
	bool RemoveAll();

private:
	//##ModelId=396DF30E0301
	CMinNode<TYPE> * *	m_ppHashTable;
    
		
	// array type iteration
    TYPE* Item(long lIndex);
    TYPE* Item(BSTR bstrKey);
    CMinNode<TYPE>* GetNode(long lIndex);
    CMinNode<TYPE>* GetNode(BSTR bstrKey);

    long AddTail(TYPE *pIType, BSTR bstrKey);
    CMinNode<TYPE>* NewMinNode(CMinNode<TYPE>*, CMinNode<TYPE>*);
	void FreeMinNode(CMinNode<TYPE>*);
	void OrderByBstr();
	void OrderByLong();
	void OrderQuick(CMinNode<TYPE>*, CMinNode<TYPE>*);
	long CompareNodes(CMinNode<TYPE>*, CMinNode<TYPE>*);
	void SwapNodes(CMinNode<TYPE>*, CMinNode<TYPE>*);
	void RecalculateHashTable();
	void InsertNodeInHash(CMinNode<TYPE>* pNewNode);
};

template<class TYPE, class BASE>
STDMETHODIMP CGoCollection<TYPE,BASE>::Add(TYPE *pIType, VARIANT index)
{

	TYPE *pTestItem = NULL;
	// Primero buscamos si existe este elemento
	HRESULT hr = get_Item(index, &pTestItem);
	if (hr == S_OK)
	{
		// Se ha encontrado, luego procedemos a irnos por patas
		// Eliminamos la referencia que hemos obtenido
		pTestItem->Release();
		hr = E_FAIL;
	}
    else if(pIType!=NULL)
    {
        hr = S_OK;
		if(index.vt == VT_BSTR)
        {
            
			if(AddTail(pIType, index.bstrVal)==NULL)
            {
                hr = E_FAIL;
            }
        }
        else if(index.vt == VT_EMPTY || index.vt == VT_ERROR)
        {
            if(AddTail(pIType, NULL)==NULL)
            {
                hr = E_FAIL;
            }
        }
        else
        {
            hr = E_INVALIDARG;
        }
    }
    else
    {
        hr = E_INVALIDARG;
    }
    return hr;
}

template<class TYPE, class BASE>
STDMETHODIMP CGoCollection<TYPE,BASE>::get_Count(long *pVal)
{ 
    HRESULT hr = S_OK;
    if (pVal != NULL)
		*pVal = m_nCount;
    else
        hr = E_INVALIDARG;

	return hr; 
}

template<class TYPE, class BASE>
STDMETHODIMP CGoCollection<TYPE,BASE>::get_Item(VARIANT index, TYPE **pvar)
{
    HRESULT hr = S_OK;
    if (pvar != NULL)
    {
        if(index.vt == VT_I4)
        {
            TYPE* pType=NULL;
            pType = Item(index.lVal);
            if(pType == NULL)
            {
                hr = E_INVALIDARG;
            }
            else
            {
                *pvar = pType;
            }
        }
        else if(index.vt == VT_I2)
        {
            TYPE* pType=NULL;
            pType = Item((long)index.iVal);
            if(pType == NULL)
            {
                hr = E_INVALIDARG;
            }
            else
            {
                *pvar = pType;
            }
        }
        else if(index.vt == VT_BSTR)
        {
            TYPE* pType=NULL;
            pType = Item(index.bstrVal);
            if(pType == NULL)
            {
                hr = E_INVALIDARG;
            }
            else
            {
                *pvar = pType;
            }
        }
        else
        {
            hr = E_INVALIDARG;
        }
    }
    else
    {
        hr = E_INVALIDARG;
    }

	return hr; 
}

template<class TYPE, class BASE>
STDMETHODIMP CGoCollection<TYPE,BASE>::get__NewEnum(LPUNKNOWN *ppVal)
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
    int i = 0;
    while ( i < m_nCount )
    {
		TYPE* pIP = NULL;
		pVar[i].vt = VT_DISPATCH;
		pIP = Item((long)i+1);
		if(pIP != NULL)
		{
			hr = pIP->QueryInterface(IID_IUnknown, (void**)&pVar[i].pdispVal);
			pIP->Release();
			pIP = NULL;
		}
		i++;
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
}

template<class TYPE, class BASE>
STDMETHODIMP CGoCollection<TYPE,BASE>::Remove(VARIANT index)
{
    HRESULT hr = S_OK;
    CMinNode<TYPE>* pOldNode = NULL;

    if(index.vt == VT_I4)
    {
        pOldNode = GetNode(index.lVal);
    }
    else if(index.vt == VT_I2)
    {
        pOldNode = GetNode((long)index.iVal);
    }
    else if(index.vt == VT_BSTR)
    {
        pOldNode = GetNode(index.bstrVal);
    }
    else
    {
        hr = E_INVALIDARG;
    }

    if(pOldNode!=NULL)
    {
	    // if this is the head remove pOldNode from list
	    if (pOldNode == m_pNodeHead)
	    {
		    m_pNodeHead = pOldNode->pNext;
	    }
	    else
	    {
		    if(pOldNode->pPrev != NULL)
            {
                // Adjust the Next pointer of the previous Node.
    		    pOldNode->pPrev->pNext = pOldNode->pNext;
            }
	    }

        // if this is the tail remove it from the list.
        if (pOldNode == m_pNodeTail)
	    {
		    m_pNodeTail = pOldNode->pPrev;
	    }
	    else
	    {
		    if(pOldNode->pNext != NULL)
            {
                // Adjust the Prev pointer of the next Node.
		        pOldNode->pNext->pPrev = pOldNode->pPrev;
            }
	    }
    
        // Now free the dangling node.
        FreeMinNode(pOldNode);
    }
    else
    {
        hr = E_INVALIDARG;
    }

	return hr; 
}

template<class TYPE, class BASE>
TYPE* CGoCollection<TYPE,BASE>::Item(long lIndex)
{ 
    TYPE* pType=NULL;
    CMinNode<TYPE>* pNode = GetNode(lIndex);

    if(pNode != NULL)
    {
        pType = pNode->pData;
        if(pType!=NULL)
            pType->AddRef();
    }

	return pType; 
}

template<class TYPE, class BASE>
TYPE* CGoCollection<TYPE,BASE>::Item(BSTR bstrKey)
{ 
    TYPE* pType=NULL;
    CMinNode<TYPE>* pNode = GetNode(bstrKey);

    if(pNode != NULL)
    {
        pType = pNode->pData;
        if(pType!=NULL)
            pType->AddRef();
    }

	return pType; 
}

template<class TYPE, class BASE>
CMinNode<TYPE>* CGoCollection<TYPE,BASE>::GetNode(long lIndex)
{ 
    CMinNode<TYPE>* pNode=NULL;
    long lCounter=1;

    pNode = m_pNodeHead;
	while (pNode != NULL && lCounter < lIndex)
    {
		pNode = pNode->pNext;
        lCounter++;
    }

	return pNode; 
}

template<class TYPE, class BASE>
CMinNode<TYPE>* CGoCollection<TYPE,BASE>::GetNode(BSTR bstrKey)
{ 
	USES_CONVERSION;
    
	CMinNode<TYPE> *p=NULL;

	if (bstrKey)
	{
		char * cTemp = OLE2A(bstrKey);
		strlwr(cTemp);
	    // Calculamos Hash
		int nHash = HashKey(cTemp) % HASH_SIZE;
		p = m_ppHashTable[nHash];
		while (p && wcsicmp(p->bstrKey, bstrKey))
		{
			p = p->pNextHash;
		}
	}
	
	return p;
}

template<class TYPE, class BASE>
long CGoCollection<TYPE,BASE>::AddTail(TYPE *pIType, BSTR bstrKey)
{
	USES_CONVERSION;
	
	CMinNode<TYPE>* pNewNode = NewMinNode(m_pNodeTail, NULL);
	pNewNode->pData = pIType;
    
	if (bstrKey)
	{
		pNewNode->bstrKey = ::SysAllocString(bstrKey);
		wcslwr(pNewNode->bstrKey);
		InsertNodeInHash(pNewNode);
	}

    pIType->AddRef();

	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = pNewNode;
	else
		m_pNodeHead = pNewNode;

    m_pNodeTail = pNewNode;

    return (long) pNewNode;
}

template<class TYPE, class BASE>
bool CGoCollection<TYPE,BASE>::RemoveAll()
{
	// destroy elements
    bool fRet = true;
    VARIANT var;
    var.vt = VT_I4;
    var.lVal = 1;

    while (m_nCount != 0 && fRet)
    {
        if(Remove(var)!=S_OK)
            fRet = false;
    }

    return fRet;
}

template<class TYPE, class BASE>
CMinNode<TYPE>* CGoCollection<TYPE,BASE>::NewMinNode(
                                        CMinNode<TYPE>* pPrev,
                                        CMinNode<TYPE>* pNext)
{
    CMinNode<TYPE>* pNode = new CMinNode<TYPE>;
	pNode->pPrev = pPrev;
	pNode->pNext = pNext;
	pNode->pNextHash = NULL;
    pNode->pData = NULL;
    pNode->bstrKey = NULL;
	pNode->bstrOrder = NULL;

    m_nCount++;

    return pNode;
}

template<class TYPE, class BASE>
void CGoCollection<TYPE,BASE>::FreeMinNode(CMinNode<TYPE>* pNode)
{
    
	USES_CONVERSION;
	
	// Release del Objeto
    pNode->pData->Release();
	
	// Miramos si lo tenemos en Tabla Hash Para eliminarlo
	if (pNode->bstrKey)
	{
		// Lo Localizamos
		char * cTemp = OLE2A(pNode->bstrKey);
		int nHash = HashKey(cTemp) % HASH_SIZE;
		CMinNode<TYPE> * pBefore = NULL;
		CMinNode<TYPE> * p = m_ppHashTable[nHash];
		while (p && wcsicmp(p->bstrKey, pNode->bstrKey))
		{
			pBefore = p;
			p = p->pNextHash;
		}
		if (p)
		{
			// Lo hemos encontrado
			if (pBefore)
				pBefore->pNextHash = p->pNextHash;
			else
			{
				// Es el Primero
				m_ppHashTable[nHash] = m_ppHashTable[nHash]->pNextHash;
			}
		}
	}
		
	::SysFreeString(pNode->bstrKey);
	::SysFreeString(pNode->bstrOrder);

    // Now delete the memory used by the CMinNode.
    delete pNode;

	m_nCount--;
}



template<class TYPE, class BASE>
STDMETHODIMP CGoCollection<TYPE,BASE>::OrderBy(BSTR bstrOrderKey)
{

	long		orderType = 0;  //  1 -> Long.   2 -> BSTR
	VARIANT		VarVal;
	CMinNode<TYPE>*	pNode;

	// Tenemos que tener los suficientes elementos para ordenar	
	if (m_nCount < 2) return S_OK;
	
	// Leer los elementos y rellenar la información en CMinNode
	// El primero es el que va a marcar el tipo
	
	::VariantInit(&VarVal);

	if (GetPropertyValue(bstrOrderKey, &VarVal, (IDispatch *) m_pNodeHead->pData) == S_OK)
	{
		if (VarVal.vt == VT_BSTR)
		{
			orderType = BY_BSTR;
			::SysFreeString(m_pNodeHead->bstrOrder);
			m_pNodeHead->bstrOrder = ::SysAllocString(VarVal.bstrVal);

		}
		else if(VarVal.vt == VT_I4)     // Long
		{
			orderType = BY_LONG;
			m_pNodeHead->nLong = VarVal.lVal;
		}
		else if(VarVal.vt == VT_I2)      // short
		{
			orderType = BY_LONG;
			m_pNodeHead->nLong = VarVal.iVal;
		}
		else if(VarVal.vt == VT_UI1)      //  Unsigned Int
		{
			orderType = BY_LONG;
			m_pNodeHead->nLong = VarVal.bVal;
		}
		else if(VarVal.vt == VT_BOOL)      //  Boolean
		{
			orderType = BY_LONG;
			m_pNodeHead->nLong = VarVal.boolVal;
			
		}
		else
		{
			::VariantClear(&VarVal);
			return E_FAIL;
		}

	}
	else
	{
		::VariantClear(&VarVal);
		return E_FAIL;
	}
	
	// Nos vamos al Siguiente
	pNode = m_pNodeHead->pNext;
	::VariantClear(&VarVal);
	while(pNode != NULL)
	{
		HRESULT hr;	
		if (hr = GetPropertyValue(bstrOrderKey, &VarVal, (IDispatch *) pNode->pData) == S_OK)
		{
			if (orderType == BY_BSTR) 
			{
				if (VarVal.vt == VT_BSTR)
				{
					::SysFreeString(pNode->bstrOrder);
					pNode->bstrOrder = ::SysAllocString(VarVal.bstrVal);
				}
				else
				{
					// Algo ha fallado (se ha cambiado de tipo)
					::VariantClear(&VarVal);
					return E_FAIL;
				}
			}
			else if (orderType == BY_LONG)
			{
				if(VarVal.vt == VT_I4)     // Long
				{
					pNode->nLong = VarVal.lVal;
				}
				else if(VarVal.vt == VT_I2)      // short
				{
					pNode->nLong = VarVal.iVal;
				}
				else if(VarVal.vt == VT_UI1)      //  Unsigned Int
				{
					pNode->nLong = VarVal.bVal;
				}
				else if(VarVal.vt == VT_BOOL)      //  Unsigned Int
				{
					pNode->nLong = VarVal.boolVal;
				}
				else
				{
					::VariantClear(&VarVal);
					return E_FAIL;
				}
			}
			else
			{
				::VariantClear(&VarVal);
				return E_FAIL;
			}
		}
		else
		{
			// Algo malo ha ocurrido
			::VariantClear(&VarVal);
			return hr;
		}
		::VariantClear(&VarVal);
		pNode = pNode->pNext;
	}

	// Ordenar en función de si es BSTR o Long
	if (orderType == BY_BSTR)
	{
		OrderByBstr();
	}
	else if (orderType == BY_LONG)
	{
		OrderByLong();
	}
	else
		return E_FAIL;
	
	// Nos vamos
	return S_OK;
}

template<class TYPE, class BASE>
STDMETHODIMP CGoCollection<TYPE,BASE>::GetPropertyValue(BSTR bstrName, VARIANT * pVar, IDispatch * pDis)
{
	HRESULT hr;
	DISPID dispid;

	hr = pDis->GetIDsOfNames(IID_NULL,&bstrName,1, LOCALE_SYSTEM_DEFAULT,&dispid);

	if (hr == S_OK)
	{
		DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};

		// Suponemos que el Variant Ya Está Inicializado Correctamente
		hr = pDis->Invoke(dispid,IID_NULL,LOCALE_SYSTEM_DEFAULT,DISPATCH_PROPERTYGET,
									&dispparamsNoArgs, pVar, NULL, NULL);
		return hr;
	}
	return E_FAIL;

}

template<class TYPE, class BASE>
void CGoCollection<TYPE,BASE>::OrderByBstr()
{
	m_nOrderType = 2;    ///  1 -> Long, 2 -> BSTR;
	if (m_nCount > 1) OrderQuick(m_pNodeHead, m_pNodeTail);
	// Al haber cambiado posiciones de los elementos, volvemos a calcular la Hash
	RecalculateHashTable();
}

template<class TYPE, class BASE>
void CGoCollection<TYPE,BASE>::OrderByLong()
{
	m_nOrderType = 1;    ///  1 -> Long, 2 -> BSTR;
	if (m_nCount > 1) OrderQuick(m_pNodeHead, m_pNodeTail);
	// Al haber cambiado posiciones de los elementos, volvemos a calcular la Hash
	RecalculateHashTable();
}


template<class TYPE, class BASE>
void CGoCollection<TYPE,BASE>::OrderQuick(CMinNode<TYPE>* first, CMinNode<TYPE>* last)
{
	
	CMinNode<TYPE>* Pivot;
	CMinNode<TYPE>* aNodelo = first;
	CMinNode<TYPE>* aNodehi = last;
	
	if (first->pNext == last)
	{
		// Tenemos únicamente 2 elementos
		if (CompareNodes(first, last) > 0) SwapNodes(first, last);
		return;
	}


	// Buscamos optimización en la búsqueda del Pivot (cuando está ordenada)
	while ((aNodelo != last) && ( CompareNodes(aNodelo, aNodelo->pNext) <= 0))
	{
		aNodelo = aNodelo->pNext;
	}
	if (aNodelo == last) return; // está ordenada
	
	Pivot = aNodelo;
	
	while(aNodelo != aNodehi) 
	{

		// De Arriba a Abajo
		while ((aNodehi != Pivot) && (CompareNodes(Pivot, aNodehi) <= 0))
		{
			aNodehi = aNodehi->pPrev;
		}

        if (Pivot != aNodehi)
		{
			SwapNodes(aNodehi, Pivot);
			Pivot = aNodehi;
		}

		// De Abajo a Arriba
		while ((aNodelo != Pivot) && ( CompareNodes(aNodelo, Pivot) <= 0))
		{
			aNodelo = aNodelo->pNext;
		}

		if (aNodelo != Pivot)
		{
			SwapNodes(aNodelo, Pivot);
			Pivot = aNodelo;
		}

	}

	if (first != aNodelo) OrderQuick(first, aNodelo->pPrev);
	if (aNodehi != last) OrderQuick(aNodehi->pNext, last);

}

template<class TYPE, class BASE>
long CGoCollection<TYPE,BASE>::CompareNodes(CMinNode<TYPE>* pBefore, CMinNode<TYPE>* pAfter)
{
	
	//  retorno
	//  1 si Before Mayor After
	//  0 si Before = After
	// -1 si Before Menor After
		
	// 1 -> Long, 2 -> BSTR
	if (m_nOrderType == 2)
	{
		return wcsicmp(pBefore->bstrOrder, pAfter->bstrOrder);
	}
	else
	{
		if (pBefore->nLong > pAfter->nLong) return 1;
		if (pBefore->nLong < pAfter->nLong) return -1;
	}	
	return 0;
}


template<class TYPE, class BASE>
void CGoCollection<TYPE,BASE>::SwapNodes(CMinNode<TYPE>* pBefore, CMinNode<TYPE>* pAfter)
{
	CMinNode<TYPE> Buffer;
	Buffer.bstrOrder = NULL;
	Buffer.bstrKey = NULL;
	
	//m_nSwapCount++;
	Buffer.pData = pBefore->pData;
	pBefore->pData = pAfter->pData;
	pAfter->pData = Buffer.pData;

	Buffer.nLong = pBefore->nLong;
	pBefore->nLong = pAfter->nLong;
	pAfter->nLong = Buffer.nLong;

	Buffer.bstrOrder = pBefore->bstrOrder;
	pBefore->bstrOrder = pAfter->bstrOrder;
	pAfter->bstrOrder = Buffer.bstrOrder;

	Buffer.bstrKey = pBefore->bstrKey;
	pBefore->bstrKey = pAfter->bstrKey;
	pAfter->bstrKey = Buffer.bstrKey;

}

template<class TYPE, class BASE>
void CGoCollection<TYPE,BASE>::RecalculateHashTable()
{
	// Limpiamos los datos de Hash y volvemos a añadir los elementos
	CMinNode<TYPE> * pNode = m_pNodeHead;

	// Limpiamos información en los nodos
	while(pNode)
	{
		pNode->pNextHash = NULL;
		pNode = pNode->pNext;
	}
	// Limpiamos información de la tabla
	memset(m_ppHashTable, 0, HASH_SIZE * sizeof(CMinNode<TYPE>*));

	// Los volvemos a calcular
	pNode = m_pNodeHead;
	
	while(pNode)
	{
		InsertNodeInHash(pNode);
		pNode = pNode->pNext;
	}
}

template<class TYPE, class BASE>
void CGoCollection<TYPE,BASE>::InsertNodeInHash(CMinNode<TYPE>* pNewNode)
{
	USES_CONVERSION;

	if(pNewNode->bstrKey != NULL)
	{
		// Lo engarzamos en las tablas Hash
		char * cTemp = OLE2A(pNewNode->bstrKey);
		int nHash = HashKey(cTemp) % HASH_SIZE;
		CMinNode<TYPE> * p = m_ppHashTable[nHash];
		if (p)
		{
			while(p->pNextHash)
			{
				p = p->pNextHash;
			}
			p->pNextHash = pNewNode;
			pNewNode->pNextHash = NULL;
		}
		else
			m_ppHashTable[nHash] = pNewNode;
	}

}

#endif

