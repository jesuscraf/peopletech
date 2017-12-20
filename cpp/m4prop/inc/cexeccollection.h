/////////////////////////////////////////////////////////////////////////////
// Colección Personalizada con ordenación por petición de datos

#ifndef __CExecCollection_H__
#define __CExecCollection_H__

#include "mybstr.h"

template<class TYPE, class BASE> 
class CExecCollection : public BASE
{
public:
	struct CMinNode
	{
		CMinNode*	pNext;
		CMinNode*	pPrev;
		TYPE*		pData;
	    CMyBSTR		bstrKey;
		long		nExecOrder;
	};

	public:
// Construction
	CExecCollection() 
    {	
        m_nCount = 0;
        m_pNodeHead = m_pNodeTail = NULL;
    };

    virtual ~CExecCollection()
    {
	    InnerRemoveAll();
		//ATLTRACE("RemoveAll del CExecCollection\n");
    };

	// count of elements
	
	STDMETHOD(Add)(TYPE *pIType, VARIANT index);
	STDMETHOD(get_Count)(long *pVal);
	STDMETHOD(get_Item)(VARIANT index, TYPE **pvar);
	STDMETHOD(get__NewEnum)(LPUNKNOWN *pVal);
	STDMETHOD(Remove)(VARIANT index);
	STDMETHOD(get_Exec)(VARIANT index, long *pVal);
	STDMETHOD(put_Exec)(VARIANT index, long pVal);
	STDMETHOD(RemoveAll)();

	//STDMETHOD(GetPropertyValue)(BSTR bstrName, VARIANT * pVar, IDispatch * pDis);
	/*
		[id(1), helpstring("Add")] HRESULT Add([in]TYPE *pI, [in,optional]VARIANT index);
		[propget, id(2), helpstring("Count")] HRESULT Count([out, retval] long *pVal);
		[propget, id(3), helpstring("Item")] HRESULT Item([in]VARIANT index, [out, retval]TYPE **pvar);
		[propget, id(DISPID_NEWENUM), restricted, helpstring("_NewEnum")] HRESULT _NewEnum([out, retval] LPUNKNOWN *pVal);
		[id(4), helpstring("Remove")] HRESULT Remove([in]VARIANT index);
		[propget, id(5), helpstring("Property ExecOrder")] HRESULT Exec([in]VARIANT index, [out, retval]long *pVal);
		[propput, id(5), helpstring("property ExecOrder")] HRESULT Exec([in]VARIANT index, [in] long pVal);
	*/

    //typedef CComEnum<CComIEnum<TYPE*>, &IID_IEnumVARIANT, TYPE*, _CopyInterface<TYPE> > _CComCollEnum;
    typedef CComEnum<CComIEnum<VARIANT>, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT> > _CComCollEnum;

// Implementation

	CMinNode 	*m_pNodeHead;
	CMinNode	*m_pNodeTail;
	int			m_nCount;
	long		m_nOrderType;

	

	// remove all elements
	bool InnerRemoveAll();

private:

    // array type iteration
    TYPE* Item(long lIndex);
    TYPE* Item(BSTR bstrKey);
    CMinNode* GetNode(long lIndex);
    CMinNode* GetNode(BSTR bstrKey);
	long GetMyNode(BSTR bstrKey);
	long GetMyNode(long lIndex);

    long AddTail(TYPE *pIType, BSTR bstrKey);
    CMinNode* NewMinNode(CMinNode*, CMinNode*);
	void FreeMinNode(CMinNode*);
	//void OrderByBstr();
	//void OrderByLong();
	//void OrderQuick(CMinNode*, CMinNode*);
	//long CompareNodes(CMinNode*, CMinNode*);
	//void SwapNodes(CMinNode*, CMinNode*);
	
	//virtual void Order() = 0;

};

template<class TYPE, class BASE>
STDMETHODIMP CExecCollection<TYPE,BASE>::Add(TYPE *pIType, VARIANT index)
{
    
	TYPE *pTestItem = NULL;
	// Primero buscamos si existe este elemento
	HRESULT hr = get_Item(index, &pTestItem);
	if (hr == S_OK)
	{
		// Se ha encontrado, luego procedemos a irnos por patas
		// Eliminamos la referencia que hemos obtenido
		RELEASE(pTestItem);
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
STDMETHODIMP CExecCollection<TYPE,BASE>::get_Count(long *pVal)
{ 
    HRESULT hr = S_OK;
    if (pVal != NULL)
    {
		*pVal = m_nCount;
    }
    else
    {
        hr = E_INVALIDARG;
    }

	return hr; 
}

template<class TYPE, class BASE>
STDMETHODIMP CExecCollection<TYPE,BASE>::get_Item(VARIANT index, TYPE **pvar)
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
STDMETHODIMP CExecCollection<TYPE,BASE>::get__NewEnum(LPUNKNOWN *ppVal)
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
			//pIP = Item((long)i+1);
			hr = pIP->QueryInterface(IID_IUnknown, (void**)&pVar[i].pdispVal);
			RELEASE(pIP);
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
STDMETHODIMP CExecCollection<TYPE,BASE>::Remove(VARIANT index)
{
    HRESULT hr = S_OK;
    CMinNode* pOldNode = NULL;

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
TYPE* CExecCollection<TYPE,BASE>::Item(long lIndex)
{ 
    TYPE* pType=NULL;
    CMinNode* pNode = GetNode(lIndex);

    if(pNode != NULL)
    {
        pType = pNode->pData;
        if(pType!=NULL)
            pType->AddRef();
    }

	return pType; 
}

template<class TYPE, class BASE>
TYPE* CExecCollection<TYPE,BASE>::Item(BSTR bstrKey)
{ 
    TYPE* pType=NULL;
    CMinNode* pNode = GetNode(bstrKey);

    if(pNode != NULL)
    {
        pType = pNode->pData;
        if(pType!=NULL)
            pType->AddRef();
    }

	return pType; 
}

template<class TYPE, class BASE>
typename CExecCollection<TYPE,BASE>::CMinNode* CExecCollection<TYPE,BASE>::GetNode(long lIndex)
{ 
    CMinNode* pNode=NULL;
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
typename CExecCollection<TYPE,BASE>::CMinNode* CExecCollection<TYPE,BASE>::GetNode(BSTR bstrKey)
{ 
    CMinNode *pNode=NULL;
    CMyBSTR bstrTemp;
	bstrTemp.Attach(bstrKey);

    pNode = m_pNodeHead;
	while (pNode != NULL && (bstrTemp != pNode->bstrKey) )
    {
		pNode = pNode->pNext;
    }

	return pNode; 
}


template<class TYPE, class BASE>
long CExecCollection<TYPE,BASE>::GetMyNode(long lIndex)
{ 
    CMinNode* pNode=NULL;
    long lCounter=1;

    pNode = m_pNodeHead;
	while (pNode != NULL && lCounter < lIndex)
    {
		pNode = pNode->pNext;
        lCounter++;
    }

	if (pNode == NULL) return 0;
	return lCounter; 
}

template<class TYPE, class BASE>
long CExecCollection<TYPE,BASE>::GetMyNode(BSTR bstrKey)
{ 
    CMinNode *pNode=NULL;
    CMyBSTR bstrTemp;
	bstrTemp.Attach(bstrKey);
	long lCounter = 1;

    pNode = m_pNodeHead;
	while (pNode != NULL && (bstrTemp != pNode->bstrKey) )
    {
		pNode = pNode->pNext;
		lCounter++;
    }

	if (pNode == NULL) return 0;
	return lCounter; 
}



template<class TYPE, class BASE>
long CExecCollection<TYPE,BASE>::AddTail(TYPE *pIType, BSTR bstrKey)
{
	CMinNode* pNewNode = NewMinNode(m_pNodeTail, NULL);
	pNewNode->pData = pIType;
    if(bstrKey != NULL)
        pNewNode->bstrKey = bstrKey;

    pIType->AddRef();

	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = pNewNode;
	else
		m_pNodeHead = pNewNode;

    m_pNodeTail = pNewNode;

    return (long) pNewNode;
}

template<class TYPE, class BASE>
STDMETHODIMP CExecCollection<TYPE,BASE>::RemoveAll()
{
    HRESULT hr = E_FAIL;
	
	if (InnerRemoveAll() == true)
	{
		hr = S_OK;
	}
	return hr;
}

template<class TYPE, class BASE>
bool CExecCollection<TYPE,BASE>::InnerRemoveAll()
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
typename CExecCollection<TYPE,BASE>::CMinNode* CExecCollection<TYPE,BASE>::NewMinNode(
	typename CExecCollection<TYPE,BASE>::CMinNode* pPrev,
	typename CExecCollection<TYPE,BASE>::CMinNode* pNext)
{
    CExecCollection<TYPE,BASE>::CMinNode* pNode = new CExecCollection<TYPE,BASE>::CMinNode;
	pNode->pPrev = pPrev;
	pNode->pNext = pNext;
    pNode->pData = NULL;
    //pNode->bstrKey = "";

    m_nCount++;

    return pNode;
}

template<class TYPE, class BASE>
void CExecCollection<TYPE,BASE>::FreeMinNode(typename CExecCollection<TYPE,BASE>::CMinNode* pNode)
{
    // First, release the object
    RELEASE(pNode->pData);

    // Now delete the memory used by the CMinNode.
    pNode->pNext = NULL;
	pNode->pPrev = NULL;
	delete pNode;

	m_nCount--;
}


/*
template<class TYPE, class BASE>
STDMETHODIMP CExecCollection<TYPE,BASE>::OrderBy(BSTR bstrOrderKey)
{

	long	orderType = 0;  //  1 -> Long.   2 -> BSTR
	VARIANT	VarVal;
	CMinNode* pNode;

	// Tenemos que tener los suficientes elementos para ordenar	
	if (m_nCount < 2) return S_OK;
	
	// Leer los elementos y rellenar la información en CMinNode
	// El primero es el que va a marcar el tipo
	
	::VariantInit(&VarVal);

	if (GetPropertyValue(bstrOrderKey, &VarVal, (IDispatch *) m_pNodeHead->pData) == S_OK)
	{
		if (VarVal.vt == VT_BSTR)
		{
			orderType = 2;
			m_pNodeHead->bstrOrder = VarVal.bstrVal;

		}
		else if(VarVal.vt == VT_I4)     // Long
		{
			orderType = 1;
			m_pNodeHead->nLong = VarVal.lVal;
		}
		else if(VarVal.vt == VT_I2)      // short
		{
			orderType = 1;
			m_pNodeHead->nLong = VarVal.iVal;
		}
		else if(VarVal.vt == VT_UI1)      //  Unsigned Int
		{
			orderType = 1;
			m_pNodeHead->nLong = VarVal.bVal;
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
			if (orderType == 2) 
			{
				if (VarVal.vt == VT_BSTR)
				{
					pNode->bstrOrder = VarVal.bstrVal;
				}
				else
				{
					// Algo ha fallado (se ha cambiado de tipo)
					::VariantClear(&VarVal);
					return E_FAIL;
				}
			}
			else if (orderType == 1)
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
	if (orderType == 2)
	{
		OrderByBstr();
	}
	else if (orderType == 1)
	{
		OrderByLong();
	}
	else
		return E_FAIL;
	
	// Nos vamos
	return S_OK;
}

template<class TYPE, class BASE>
STDMETHODIMP CExecCollection<TYPE,BASE>::GetPropertyValue(BSTR bstrName, VARIANT * pVar, IDispatch * pDis)
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
void CExecCollection<TYPE,BASE>::OrderByBstr()
{
	m_nOrderType = 2;    ///  1 -> Long, 2 -> BSTR;
	if (m_nCount > 1) OrderQuick(m_pNodeHead, m_pNodeTail);
}

template<class TYPE, class BASE>
void CExecCollection<TYPE,BASE>::OrderByLong()
{
	m_nOrderType = 1;    ///  1 -> Long, 2 -> BSTR;
	if (m_nCount > 1) OrderQuick(m_pNodeHead, m_pNodeTail);
}


template<class TYPE, class BASE>
void CExecCollection<TYPE,BASE>::OrderQuick(CExecCollection<TYPE,BASE>::CMinNode* first, CExecCollection<TYPE,BASE>::CMinNode* last)
{
	
	CExecCollection<TYPE,BASE>::CMinNode* Pivot;
	CExecCollection<TYPE,BASE>::CMinNode* aNodelo = first;
	CExecCollection<TYPE,BASE>::CMinNode* aNodehi = last;
	
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
long CExecCollection<TYPE,BASE>::CompareNodes(CExecCollection<TYPE,BASE>::CMinNode* pBefore, CExecCollection<TYPE,BASE>::CMinNode* pAfter)
{
	
	//  retorno
	//  1 si Before Mayor After
	//  0 si Before = After
	// -1 si Before Menor After
		
	// 1 -> Long, 2 -> BSTR
	if (m_nOrderType == 2)
	{
		if (pBefore->bstrOrder > pAfter->bstrOrder) return 1;
		if (pBefore->bstrOrder < pAfter->bstrOrder) return -1;
	}
	else
	{
		if (pBefore->nLong > pAfter->nLong) return 1;
		if (pBefore->nLong < pAfter->nLong) return -1;
	}	
	return 0;
}


template<class TYPE, class BASE>
void CExecCollection<TYPE,BASE>::SwapNodes(CExecCollection<TYPE,BASE>::CMinNode* pBefore, CExecCollection<TYPE,BASE>::CMinNode* pAfter)
{
	CMinNode Buffer;
	
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

*/
template<class TYPE, class BASE>
STDMETHODIMP CExecCollection<TYPE,BASE>::get_Exec(VARIANT index, long *pVal)
{
	
	long	lValue;
    HRESULT hr = S_OK;
    
	if(index.vt == VT_I4)
    {
		lValue = GetMyNode(index.lVal);
		if (lValue == 0) return E_INVALIDARG;
		*pVal = lValue;
    }
    else if(index.vt == VT_I2)
    {
		lValue = GetMyNode((long)index.iVal);
		if (lValue == 0) return E_INVALIDARG;		
		*pVal = lValue;
    }
    else if(index.vt == VT_BSTR)
    {
		lValue = GetMyNode(index.bstrVal);
		if (lValue == 0) return E_INVALIDARG;		
		*pVal = lValue;
    }
	else
	{
		return E_INVALIDARG;
	}
	return hr; 

}

template<class TYPE, class BASE>
STDMETHODIMP CExecCollection<TYPE,BASE>::put_Exec(VARIANT index, long pVal)
{
	
	long		lPos;
	CMinNode	*pNode;
    HRESULT hr = S_OK;
    
	if (pVal < 1) pVal = 1;
	if (m_nCount > 1)
	{
		if (pVal > m_nCount)
		{
			pVal = m_nCount;
		}

		if(index.vt == VT_I4)
		{
			lPos = GetMyNode(index.lVal);
			if (lPos == 0) return E_INVALIDARG;
		}
		else if(index.vt == VT_I2)
		{
			lPos = GetMyNode((long)index.iVal);
			if (lPos == 0) return E_INVALIDARG;		
		}
		else if(index.vt == VT_BSTR)
		{
			lPos = GetMyNode(index.bstrVal);
			if (lPos == 0) return E_INVALIDARG;		
		}
		else
		{
			return E_INVALIDARG;
		}

		// Si hemos llegado aquí tenemos en lPos el índice del nodo a mover de sitio
		if (lPos == pVal) return S_OK;

		
		if (m_nCount == 2)
		{
			// Si estamos aquí tenemos dos elementos y hay que darles la vuelta.
			// Simplificamos
			pNode = m_pNodeHead;
			// Pasamos el Head al Tail
			pNode->pPrev = m_pNodeTail;
			pNode->pNext = NULL;

			// Pasamos el Tail al Head
			m_pNodeTail->pNext = pNode;
			m_pNodeTail->pPrev = NULL;

			m_pNodeHead = m_pNodeTail;
			m_pNodeTail = pNode;
			return S_OK;
		}
		else
		{	
			pNode = GetNode(lPos);
			// Miramos si eramos la cola
			if (pNode == m_pNodeTail)
			{
				// Como nos vamos a ir pasamos el Tail al anterior
				m_pNodeTail = m_pNodeTail->pPrev;
				m_pNodeTail->pNext = NULL;
				// Extraído está
			}
			else if(pNode == m_pNodeHead)
			{
				m_pNodeHead = m_pNodeHead->pNext;
				m_pNodeHead->pPrev = NULL;
			}
			else
			{
				// Al Siguiente le colocamos el anterior
				pNode->pNext->pPrev = pNode->pPrev;
				// Al Anterior le colocamos el siguiente
				pNode->pPrev->pNext = pNode->pNext;
			}

			// Ahora toca insertarlo en el lugar adecuado (antes de)
			//CMinNode *pAux;
			BOOL bMoved = FALSE;
			if (pVal >= m_nCount)
			{
				pVal = m_nCount - 1;
				bMoved = TRUE;
			}
			CMinNode *pNodeAux = GetNode(pVal);
			if (pVal == 1)
			{
				// Lo pasamos al Head
				m_pNodeHead->pPrev = pNode;
				pNode->pNext = m_pNodeHead;
				m_pNodeHead = pNode;
				m_pNodeHead->pPrev = NULL;
			}
			else if (pVal == m_nCount - 1)
			{
				if (bMoved)
				{
					// Lo pasamos al Tail
					m_pNodeTail->pNext = pNode;
					pNode->pPrev = m_pNodeTail;
					m_pNodeTail = pNode;
					m_pNodeTail->pNext = NULL;
				}
				else
				{
					// Lo pasamos al anterior del Tail
					/*m_pNodeTail->pNext = pNode;
					pNode->pPrev = m_pNodeTail;
					m_pNodeTail = pNode;
					m_pNodeTail->pNext = NULL;*/

					pNodeAux->pPrev->pNext = pNode;
					pNode->pPrev = pNodeAux->pPrev;
					pNode->pNext = pNodeAux;
					pNodeAux->pPrev = pNode;
				}
			}
	/*		else if (pNodeAux == m_pNodeHead)
			{
				// Lo insertamos en la cabeza
				m_pNodeHead->pPrev = pNode;
				pNode->pNext = m_pNodeHead;
				m_pNodeHead = pNode;
				m_pNodeHead->pPrev = NULL;
			}
			else if (pNodeAux == m_pNodeTail)
			{
				m_pNodeTail->pNext = pNode;
				pNode->pPrev = m_pNodeTail;
				m_pNodeTail = pNode;
				m_pNodeTail->pNext = NULL;
			}*/
			else
			{
				// Lo Insertamos
				pNodeAux->pPrev->pNext = pNode;
				pNode->pPrev = pNodeAux->pPrev;
				pNode->pNext = pNodeAux;
				pNodeAux->pPrev = pNode;
			}
		}
	}

		
/*		if (pVal == m_nCount)
		{
			// directamente va al final
			pNode->pPrev = m_pNodeTail;
			pNode->pNext = NULL;
			m_pNodeTail->pNext = pNode;
			m_pNodeTail = pNode;
		}
		else if (pVal == 1)
		{
			// directamente al principio
			pNode->pNext = m_pNodeHead;
			pNode->pPrev = NULL;
			m_pNodeHead->pPrev = pNode;
			m_pNodeHead = pNode;
		}
		else if (pNodeAux == m_pNodeTail)
		{
			//Tratamiento especial
			//pNodeAux ya está como pNodeTail
			CMinNode *pAux = pNodeAux->pPrev;
			pAux->pNext = pNode;
			pNode->pPrev = pAux;
			pNodeAux->pPrev = pNode;
			pNode->pNext = pNodeAux; 
		}
		else if (pNodeAux == m_pNodeHead->pNext)
		{
			CMinNode *pAux = pNodeAux->pPrev;
			pAux->pNext = pNode;
			pNode->pPrev = pAux;
			pNodeAux->pPrev = pNode;
			pNode->pNext = pNodeAux; 
		}
		else if (pNodeAux == m_pNodeTail->pPrev)
		{
			//Tratamiento especial
			//pNodeAux ya está como pNodeTail
			CMinNode *pAux = pNodeAux->pPrev;
			pAux->pNext = pNode;
			pNode->pPrev = pAux;
			pNodeAux->pPrev = pNode;
			pNode->pNext = pNodeAux; 
		}
		else
		{
			// Al anterior, le decimos que el siguiente somos nosotros
			// Miramos antes si existe el anterior (podemos haber pasado a la primera posición)
			CMinNode *pAux = pNodeAux->pNext;
			// Le indicamos al punto de inserción su enganche
			pAux->pPrev = pNode;
			pNodeAux->pNext = pNode;
			
			// Al Insertado le indicamos el punto adecuado.
			pNode->pPrev = pNodeAux;
			pNode->pNext = pAux;

			


*/
			


/*
			if (pNodeAux->pPrev)
			{
				pNodeAux->pPrev->pNext = pNode;
			}
			// Nuestro previo es el anterior al lugar de inserción
			pNode->pPrev = pNodeAux->pPrev;
			pNodeAux->pPrev = pNode;
			pNodeAux->pNext = pNode->pNext;
			pNode->pNext = pNodeAux;
			*/
	return hr; 

}




#endif

