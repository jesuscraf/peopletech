
#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "iwrapcolnodedef.h"
#include "wrapdef.h"
#include "wrapnodedef.h"
#include "newinterface.h"
#include "nod_data.hpp"
#include "node_knl.hpp"

/////////////////////////////////////////////////////////////////////////////
// WrapColNodeDef Interface

STDMETHODIMP CIWrapColNodeDef::get_Count(long * ao_iCount)
{
	ENTER_METHOD(CIWrapColNodeDef::get_Count);
    
	*ao_iCount = m_pnodedir->Count();

	TRACE2("Returning a count of %d", *ao_iCount);

	LEAVE_METHOD(CIWrapColNodeDef::get_Count, S_OK);
}

STDMETHODIMP CIWrapColNodeDef::Item(int ai_iIndex, IWrapNodeDef** ao_poNodeDef)
{
	ENTER_METHOD(CIWrapColNodeDef::Item);

    ClNode_Data *pnode = m_pnodedir->Get ((ClNodeIndex)(m4uint32_t)ai_iIndex);

#ifdef NO_REG_ERROR
	if (pnode)
	{
#endif
        //creamos una nueva instancia de WrapNodeDef
	    if (NewInterface(CLSID_WrapNodeDef, IID_IWrapNodeDef, (IUnknown**)ao_poNodeDef) != M4_SUCCESS)
	    {
		    LEAVE_METHOD(CIWrapColNodeDef::Item, E_UNEXPECTED);	
	    }

	    CWrapNodeDef* pObj = (CWrapNodeDef*)(*ao_poNodeDef);
		    
	    pObj->Init(pnode->GetpNodeDef ());

        m_index = (m4uint32_t)ai_iIndex;

#ifdef NO_REG_ERROR
	}
	else
	{
		FormatError(M4_CH_WO_NO_SUCH_NODE, "Node #%i dont exist", ai_iIndex);
		LEAVE_METHOD(CIWrapColNodeDef::Item, E_UNEXPECTED);
	}
#endif

	LEAVE_METHOD(CIWrapColNodeDef::Item, S_OK);
}

STDMETHODIMP CIWrapColNodeDef::_NewEnum(IUnknown** pVal)
{
	ENTER_METHOD(CIWrapColNodeDef::_NewEnum);

	*pVal = (IEnumVARIANT*) this;

	((IUnknown*)*pVal)->AddRef();
	
	LEAVE_METHOD(CIWrapColNodeDef::_NewEnum, S_OK);
}

STDMETHODIMP CIWrapColNodeDef::Clone(IEnumVARIANT** ppenum)
{
	ENTER_METHOD(CIWrapColNodeDef::Clone);

	if (NewInterface(CLSID_WrapChanDef, IID_IEnumVARIANT, (IUnknown**)ppenum) != M4_SUCCESS)
	{
		LEAVE_METHOD(CIWrapColNodeDef::Clone, E_UNEXPECTED);
	}

	((CIWrapColNodeDef*)*ppenum)->m_pnodedir = m_pnodedir;
	((CIWrapColNodeDef*)*ppenum)->m_index = m_index;

	LEAVE_METHOD(CIWrapColNodeDef::Clone, S_OK);
}

STDMETHODIMP CIWrapColNodeDef::Reset()
{
	ENTER_METHOD(CIWrapColNodeDef::Reset);

	m_index = 0;

	LEAVE_METHOD(CIWrapColNodeDef::Reset, S_OK);
}

STDMETHODIMP CIWrapColNodeDef::Skip(ULONG celt)
{
	ENTER_METHOD(CIWrapColNodeDef::Skip);

	m4uint32_t Count = m_pnodedir->Count();

	m4uint32_t oIndex = m_index + celt;
		
	if (oIndex >= Count) {

		oIndex = Count - 1;

		LEAVE_METHOD(CIWrapColNodeDef::Skip, S_FALSE);
	}

	LEAVE_METHOD(CIWrapColNodeDef::Skip, S_OK);
}

STDMETHODIMP CIWrapColNodeDef::Next(ULONG celt, VARIANT* rgvar, ULONG* pceltFetched)
{
	ENTER_METHOD(CIWrapColNodeDef::Next);

	ULONG l2 = 0;

	if (pceltFetched != NULL) {
		*pceltFetched = 0;
	}
	
	// Set the index to be the next item

	m4uint32_t Count = m_pnodedir->Count();

	m_index = m_index + 1;
		
	if (m_index < Count) {
		VariantInit(&rgvar[0]);

		rgvar[0].vt = VT_UNKNOWN;
		rgvar[0].punkVal = (IWrapColNodeDef*) this;
		((IUnknown*)rgvar[0].punkVal)->AddRef();

		// Set count of elements retrieved.
		if (pceltFetched != NULL) {
			*pceltFetched = 1;
		}

		l2++;

		TRACE3("Index %d Next %d", m_index, m_index + 1);
	}

    LEAVE_METHOD(CIWrapColNodeDef::Next, (l2 < celt) ? S_FALSE : S_OK);
}

void CIWrapColNodeDef::Init (ClNodeDirectory_Data *ai_pndc)
{
    m_pnodedir = ai_pndc;
    m_index = 0;
}
