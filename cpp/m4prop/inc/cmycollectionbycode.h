/////////////////////////////////////////////////////////////////////////////
// CComCollection<TYPE>

#ifndef __CCOMCOLLECTIONBYCODE_H__
#define __CCOMCOLLECTIONBYCODE_H__

#include "namepropmap.h"

extern CNamePropMap g_NamePropMap;


template<class TYPE, class BASE> 
class CMyCollectionByCode : public BASE
{
public:
	struct CMinNode
	{
		CMinNode* pNext;
		CMinNode* pPrev;
		TYPE* pData;
		//##
        //BSTR bstrKey;
		int iKey;
	};

	public:
	// Construction
	CMyCollectionByCode() 
    {	
        m_nCount = 0;
        m_pNodeHead = m_pNodeTail = NULL;
    };

    virtual ~CMyCollectionByCode()
    {
	    InnerRemoveAll();
    };

	STDMETHOD(Add)(TYPE *pIType, VARIANT index);
	STDMETHOD(get_Count)(long *pVal);
	STDMETHOD(get_Item)(VARIANT index, TYPE **pvar);
	STDMETHOD(get__NewEnum)(LPUNKNOWN *pVal);
	STDMETHOD(Remove)(VARIANT index);
	STDMETHOD(get_Key)(VARIANT vIndex, BSTR * bstrVal);
	STDMETHOD(put_Key)(VARIANT vIndex, BSTR bstrnewVal);
	STDMETHOD(RemoveAll)();

    typedef CComEnum<CComIEnum<VARIANT>, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT> > _CComCollEnum;

	// Implementation
	CMinNode* m_pNodeHead;
	CMinNode* m_pNodeTail;
	int m_nCount;

	
	// remove all elements
	bool InnerRemoveAll();

private:

    // array type iteration
    TYPE* Item(long lIndex);

    TYPE* Item(BSTR bstrKey);
	
    CMinNode* GetNode(long lIndex);

    CMinNode* GetNode(BSTR bstrKey);
	CMinNode* GetNode(VARIANT vIndex);

    long AddTail(TYPE *pIType, BSTR bstrKey);

    CMinNode* NewMinNode(CMinNode*, CMinNode*);
	void FreeMinNode(CMinNode*);
	//virtual void Order() = 0;

};

template<class TYPE, class BASE>
STDMETHODIMP CMyCollectionByCode<TYPE,BASE>::Add(TYPE *pIType, VARIANT index)
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
            if(AddTail(pIType, (BSTR) NULL)==NULL)
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
STDMETHODIMP CMyCollectionByCode<TYPE,BASE>::get_Count(long *pVal)
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
STDMETHODIMP CMyCollectionByCode<TYPE,BASE>::RemoveAll()
{
    HRESULT hr = E_FAIL;
	
	if (InnerRemoveAll() == true)
	{
		hr = S_OK;
	}
	return hr;
}


template<class TYPE, class BASE>
STDMETHODIMP CMyCollectionByCode<TYPE,BASE>::get_Item(VARIANT index, TYPE **pvar)
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
                *pvar = NULL;
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
                *pvar = NULL;
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
                *pvar = NULL;
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
STDMETHODIMP CMyCollectionByCode<TYPE,BASE>::get__NewEnum(LPUNKNOWN *ppVal)
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
STDMETHODIMP CMyCollectionByCode<TYPE,BASE>::Remove(VARIANT index)
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
TYPE* CMyCollectionByCode<TYPE,BASE>::Item(long lIndex)
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
TYPE* CMyCollectionByCode<TYPE,BASE>::Item(BSTR bstrKey)
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
typename CMyCollectionByCode <TYPE,BASE>::CMinNode* CMyCollectionByCode<TYPE,BASE>::GetNode(long lIndex)
{ 
	//## Esto da el I-esimo elemento, acceso por indice
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

//## Modificada para buscar por doble
template<class TYPE, class BASE>
typename CMyCollectionByCode<TYPE,BASE>::CMinNode* CMyCollectionByCode<TYPE,BASE>::GetNode(BSTR bstrKey)
{ 
    CMinNode *pNode=NULL;

    pNode = m_pNodeHead;
	int iKey = g_NamePropMap.GetCodeFromBSTR(bstrKey);
	while (pNode != NULL && (pNode->iKey != iKey))
    {
		pNode = pNode->pNext;
    }

	return pNode; 
}

//## Modificada para insertar doble no lo que me pasan
template<class TYPE, class BASE>
long CMyCollectionByCode<TYPE,BASE>::AddTail(TYPE *pIType, BSTR bstrKey)
{
	CMinNode* pNewNode = NewMinNode(m_pNodeTail, NULL);
	
	pNewNode->pData = pIType;
    if(bstrKey != NULL)
        pNewNode->iKey = g_NamePropMap.GetCodeFromBSTR(bstrKey);;

    pIType->AddRef();

	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = pNewNode;
	else
		m_pNodeHead = pNewNode;

    m_pNodeTail = pNewNode;

    return (long) pNewNode;
}

template<class TYPE, class BASE>
bool CMyCollectionByCode<TYPE,BASE>::InnerRemoveAll()
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
typename CMyCollectionByCode<TYPE,BASE>::CMinNode* CMyCollectionByCode<TYPE,BASE>::NewMinNode(
	typename CMyCollectionByCode<TYPE,BASE>::CMinNode* pPrev,
	typename CMyCollectionByCode<TYPE,BASE>::CMinNode* pNext)
{
    CMyCollectionByCode<TYPE,BASE>::CMinNode* pNode = new CMyCollectionByCode<TYPE,BASE>::CMinNode;
	pNode->pPrev = pPrev;
	pNode->pNext = pNext;
    pNode->pData = NULL;
    //pNode->bstrKey = NULL;
	pNode->iKey = -1;

    m_nCount++;

    return pNode;
}

template<class TYPE, class BASE>
void CMyCollectionByCode<TYPE,BASE>::FreeMinNode(typename CMyCollectionByCode<TYPE,BASE>::CMinNode* pNode)
{
    // First, release the object
    RELEASE(pNode->pData);

	//## no se borra
    // Now delete the memory used by the CMinNode.
    //::SysFreeString(pNode->bstrKey);
	delete pNode;

	m_nCount--;
}

template<class TYPE, class BASE>
STDMETHODIMP CMyCollectionByCode<TYPE,BASE>::get_Key(VARIANT vIndex, BSTR * bstrVal)
{
	CMinNode *pNode = GetNode(vIndex);
	if (pNode != NULL)
	{
		//##
		*bstrVal = ::SysAllocString(g_NamePropMap.GetBSTRFromCode(pNode->iKey));
		//*bstrVal = ::SysAllocString(pNode->bstrKey);
		return S_OK;
	}
	return E_FAIL;
}

template<class TYPE, class BASE>
STDMETHODIMP CMyCollectionByCode<TYPE,BASE>::put_Key(VARIANT vIndex, BSTR bstrnewVal)
{
	CMinNode *pNode = GetNode(vIndex);
	if (pNode != NULL)
	{
		//##
		//::SysFreeString(pNode->bstrKey);
		//pNode->bstrKey = ::SysAllocString(bstrnewVal);
		pNode->iKey = g_NamePropMap.GetCodeFromBSTR(bstrnewVal);
		return S_OK;
	}
	return E_FAIL;
}


template<class TYPE, class BASE>
typename CMyCollectionByCode<TYPE,BASE>::CMinNode* CMyCollectionByCode<TYPE,BASE>::GetNode(VARIANT vIndex)
{

    CMinNode *pNew = NULL;
	if(vIndex.vt == VT_I4)
    {
		pNew = GetNode(vIndex.lVal);
    }
    else if(vIndex.vt == VT_I2)
    {
		pNew = GetNode(vIndex.iVal);
    }
    else if(vIndex.vt == VT_BSTR)
    {
		pNew = GetNode(vIndex.bstrVal);
    }
	return pNew;
}


#endif

