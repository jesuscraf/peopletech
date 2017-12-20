//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapnode.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapNode
//
//==============================================================================

#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "dm_types.hpp"
#include "wrapnode.h"
#include "wraprecordset.h"
#include "wrapoper.h"
#include "wrapitit.h"
#include "handles.hpp"
#include "m4mdrt.hpp"
#include "variantref.h"
#include "newinterface.h"
#include "wrapdef.h"
#include "gstring.h"
#include "wrapnodedef.h"
#include "rerror.h"
#include "node_knl.hpp"
#include "node.hpp"


/////////////////////////////////////////////////////////////////////////////
// CWrapNode

STDMETHODIMP CWrapNode::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = {	&IID_IWrapNode };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}


CWrapNode::CWrapNode()
	: m_poNode(0)
	, m_poItemIterator(0)
	, m_piWrapColItems(0)
{
}


CWrapNode::~CWrapNode()
{
	m_poNode = 0;

	if (m_poItemIterator)
	{
		m_poItemIterator->Release();
		m_poItemIterator = 0;
	}

	if (m_piWrapColItems)
	{
		m_piWrapColItems->Release();
		m_piWrapColItems = 0;
	}
}


m4return_t CWrapNode::Init(ClNode* ai_varRef)
{
	m4return_t status = M4_SUCCESS;
	m_poNode = ai_varRef;
	
	TRACE2("Wrapped node has address %p", m_poNode);

//Should get rid of this...
//and put in an item iterator
	
	if ((status = NewInterface(CLSID_WrapColItems, IID_IWrapColItems, (IUnknown**)&m_piWrapColItems)) == M4_SUCCESS)
	{
		ClVariantRef oVarRef((void*)&(m_poNode->Item));
		m_piWrapColItems->Init(oVarRef);
	}

	return status;
}


void CWrapNode::Unwrap(ClNode** ao_VarRef)
{
	*ao_VarRef = m_poNode;
}


STDMETHODIMP CWrapNode::GetItemCount(int* ao_count)
{
	ENTER_METHOD(CWrapNode::GetItemCount);

	*ao_count = m_poNode->GetpNodeDef()->NumberOfNodeItems();

	TRACE2("Returning %d", *ao_count);
	LEAVE_METHOD(CWrapNode::GetItemCount, S_OK);
}


STDMETHODIMP CWrapNode::get_Name(BSTR* pVal)
{
	ENTER_METHOD(CWrapNode::get_Name);

	gstring tmpGstring(m_poNode->GetpNodeDef()->Id());

	if (tmpGstring)
	{
		TRACE2("The name of the node is %s", m4pchar_t(tmpGstring));

		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapNode::get_Name, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CREATE_STRING, "Cannot create a new string");
		LEAVE_METHOD(CWrapNode::get_Name, E_UNEXPECTED);
	}
}


STDMETHODIMP CWrapNode::get_coItems(IWrapColItems** pVal)
{
	ENTER_METHOD(CWrapNode::get_coItems);

	*pVal = m_piWrapColItems;
	((IUnknown*)(*pVal))->AddRef();

	TRACE2("Returning value %d", *pVal);
	LEAVE_METHOD(CWrapNode::get_coItems, S_OK);
}


STDMETHODIMP CWrapNode::GetCurrentRecordSet(IWrapRecordSet** ao_RecordSet)
{
	ENTER_METHOD(CWrapNode::GetCurrentRecordSet);

    ClAccessRecordSet* oVarRef = &m_poNode->RecordSet;
	
	if (oVarRef)
	{
		if (NewInterface(CLSID_WrapRecordSet, IID_IWrapRecordSet, (IUnknown**)ao_RecordSet) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapItem::GetCurrentRecordSet, E_UNEXPECTED);
		}
		((CWrapRecordSet*)(*ao_RecordSet))->Init(oVarRef);
	}
	else
	{
		FormatError(M4_CH_WO_UNABLE_TO_GET_NODE, "Unable to get node");
		LEAVE_METHOD(CWrapItem::GetCurrentRecordSet, E_UNEXPECTED);
	}

	LEAVE_METHOD(CWrapItem::GetCurrentRecordSet, S_OK);
}


STDMETHODIMP CWrapNode::get_Operation(IWrapOper** pVal)
{
	ENTER_METHOD(CWrapNode::get_Operation);

	if (NewInterface(CLSID_WrapOper, IID_IWrapOper, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapNode::get_Operation, E_UNEXPECTED);
	}

	((CWrapOper*)(*pVal))->Init(&m_poNode->GetpNode_Data ()->Operation);

	LEAVE_METHOD(CWrapNode::get_Operation, S_OK);
}


STDMETHODIMP CWrapNode::Load(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapNode::Load);

//	*ao_sRetVal = m_poNode->Load();

	TRACE2("Returning %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapNode::Load, S_OK);
}


STDMETHODIMP CWrapNode::Persist(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapNode::Persist);

//	*ao_sRetVal = m_poNode->Persist();

	TRACE2("Returning %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapNode::Persist, S_OK);
}


STDMETHODIMP CWrapNode::get_NodeDef(IWrapNodeDef** pVal)
{
	ENTER_METHOD(CWrapNode::get_NodeDef);

	if (NewInterface(CLSID_WrapNodeDef, IID_IWrapNodeDef, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapNode::get_NodeDef, E_UNEXPECTED);	
	}

	CWrapNodeDef* pObj = (CWrapNodeDef*)(*pVal);
	pObj->Init(m_poNode->GetpNodeDef());

	LEAVE_METHOD(CWrapNode::get_NodeDef, S_OK);
}


STDMETHODIMP CWrapNode::get_ParentNodeId(BSTR* pVal)
{
	ENTER_METHOD(CWrapNode::get_ParentNodeId);

	ClNode_Connector* pncon = m_poNode->GetpExtInfo ()->Parent.GetpNotFree();

	if (pncon)
	{
		gstring tmpGstring(pncon->GetAddress ()->GetpNodeDef()->Id());

		if (tmpGstring)
		{
			*pVal = (BSTR)tmpGstring;
	
			LEAVE_METHOD(CWrapNode::get_ParentNodeId, S_OK);
		}
		else
		{
			// Can't create a string
			LEAVE_METHOD(CWrapNode::get_ParentNodeId, E_UNEXPECTED);
		}
	}
	else
	{
		// The node does not existm return an empty
		// string.
		gstring tmpGstring("");
		*pVal = (BSTR)tmpGstring;
	}

	LEAVE_METHOD(CWrapNode::get_ParentNodeId, S_OK);
}


STDMETHODIMP CWrapNode::get_ConnectorType(BYTE* pVal)
{
	ENTER_METHOD(CWrapNode::get_ConnectorType);

	ClNode_Connector* pncon = m_poNode->GetpExtInfo ()->Parent.GetpNotFree();

    if (pncon)
	{
	    *pVal = pncon->GetType ();
    }
    else
	{
        *pVal = 0;
    }

	LEAVE_METHOD(CWrapNode::get_ConnectorType, S_OK);
}


STDMETHODIMP CWrapNode::get_ItemIterator(IWrapItIt** pVal)
{
	ENTER_METHOD(CWrapNode::get_ItemIterator);

	if (!m_poItemIterator)
	{
		TRACE("Allocating new item iterator");

#ifdef M4_NEWINTERFACE
		if (NewInterface(CLSID_WrapItIt, IID_IWrapItIt, (IUnknown**) &m_poItemIterator) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapNode::get_ItemIterator, E_UNEXPECTED);
		}
#else
		HRESULT hRes;
		if (FAILED(hRes = M4_CREATE_INTERFACE(CWrapItIt , &m_poItemIterator))) 
		{
			LEAVE_METHOD(CWrapNode::get_ItemIterator, hRes);
		}
#endif

		((CWrapItIt*)m_poItemIterator)->Init(&m_poNode->Item);
	}

	TRACE2("The item iterator has address %p", m_poItemIterator);

	*pVal = m_poItemIterator;
	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapNode::get_ItemIterator, S_OK);
}


STDMETHODIMP CWrapNode::get_ParentNode(IWrapNode** pVal)
{
	ENTER_METHOD(CWrapNode::get_ParentNode);

	ClNode_Connector* pncon = m_poNode->GetpExtInfo ()->Parent.GetpNotFree ();

	if (pncon)
	{
        ClNodeIndex ni = pncon->GetAddress ()->GetpExtInfo ()->Index;
        ClNode *pnparent = m_poNode->GetpAccess ()->Node.Get (ni);

		if (NewInterface(CLSID_WrapNode, IID_IWrapNode, (IUnknown**)pVal) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapNode::get_ParentNode, E_UNEXPECTED);
		}

		((CWrapNode*)*pVal)->Init(pnparent);
	}
	else 
	{
		// No parent Node
		*pVal = 0;
	}

	LEAVE_METHOD(CWrapNode::get_ParentNode, S_OK);
}


STDMETHODIMP CWrapNode::get_AutoLoad(BYTE* ao_pVal)
{
	ENTER_METHOD(CWrapNode::get_AutoLoad);

    m4uint8_t tmp;
	m_poNode->WantsAutoLoad.Get(tmp);
    *ao_pVal = tmp;

	LEAVE_METHOD(CWrapChannel::get_AutoLoad, S_OK);
}


STDMETHODIMP CWrapNode::put_AutoLoad(BYTE ai_newVal)
{
	ENTER_METHOD(CWrapNode::put_AutoLoad);
    TRACE2("Setting AutoLoad mode to %i", ai_newVal);

	m_poNode->WantsAutoLoad.Set(ai_newVal);

	LEAVE_METHOD(CWrapNode::put_AutoLoad, S_OK);
}


STDMETHODIMP CWrapNode::UnLoad(VARIANT_BOOL* pVal, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapNode::UnLoad);

    m4bool_t bResetNodeItems = (*(m4bool_t *)pVal) ? M4_TRUE : M4_FALSE;

    if (m_poNode->GetpNode_Data()->GetpExtInfo ()->ChildNodeRB.Count ())
	{
        *ao_sRetVal = M4_ERROR;
	}
    else
	{
        *ao_sRetVal = m_poNode->GetpNode_Data()->ReleaseAll(bResetNodeItems);
	}

	TRACE2("Returning %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapNode::UnLoad, S_OK);
}


STDMETHODIMP CWrapNode::SendNodeMark(VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CWrapNode::SendNodeMark);

    m4bool_t bBranchMode = (*(m4bool_t *)pVal) ? M4_TRUE : M4_FALSE;
    m_poNode->SendNodeMark(bBranchMode);

	LEAVE_METHOD(CWrapNode::SendNodeMark, S_OK);
}


/*
Lee los items de nodo y los devuelve en un safe array
*/
STDMETHODIMP CWrapNode::ReadNodeItems(/*[out]*/ SAFEARRAY** ao_avValues, /*[out, retval]*/ short* ao_piReturn)
{
	ENTER_METHOD(CWrapNode::ReadNodeItems);

	HRESULT			hResult = S_OK ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iColumn = 0 ;
	m4uint32_t		iItems = 0 ;
	m4uint32_t		iColumns = 0 ;
	SAFEARRAYBOUND	rgsabound[ 1 ] ;
	VARIANT			*pvValues = NULL ;


	*ao_piReturn = M4_ERROR ;

	if( *ao_avValues != NULL )
	{
		SafeArrayDestroy( *ao_avValues ) ;
		*ao_avValues = NULL ;
	}

	iItems = m_poNode->Item.Count() ;

	// Se calcula el número de columnas con valor
	iColumns = 0 ;

	for( i = 0 ; i < iItems ; i++ )
	{
		m_poNode->Item.MoveTo( i ) ;

		if( m_poNode->Item.ItemDef.Type() != M4CL_ITEM_TYPE_METHOD )
		{
			iColumns++ ;
		}
	}

	// Se crea el array de 1 dimensión
	rgsabound[ 0 ].lLbound = 0 ;
	rgsabound[ 0 ].cElements = iColumns ;

	*ao_avValues = SafeArrayCreate( VT_VARIANT, 1, rgsabound ) ;

	if( *ao_avValues == NULL )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot create SafeArray" ) ;
		LEAVE_METHOD( CWrapCVM::ReadNodeItems, S_OK ) ;
	}

	hResult = SafeArrayAccessData( *ao_avValues, (void**) &pvValues ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot access SafeArray" ) ;
		SafeArrayDestroy( *ao_avValues ) ;
		*ao_avValues = NULL ;
		LEAVE_METHOD( CWrapCVM::ReadNodeItems, S_OK ) ;
	}

	if(	iColumns > 0 )
	{
		iColumn = 0 ;

		for( i = 0 ; i < iItems ; i++ )
		{
			m_poNode->Item.MoveTo( i ) ;

			if( m_poNode->Item.ItemDef.Type() != M4CL_ITEM_TYPE_METHOD )
			{
				// Se escribe el valor de cada celda
				m_poItemIterator->get_Value( pvValues + iColumn ) ;
				iColumn++ ;
			}
		}
	}

	hResult = SafeArrayUnaccessData( *ao_avValues ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot unaccess SafeArray" ) ;
		SafeArrayDestroy( *ao_avValues ) ;
		*ao_avValues = NULL ;
		LEAVE_METHOD( CWrapCVM::ReadNodeItems, S_OK ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD(CWrapNode::ReadNodeItems, S_OK);
}
