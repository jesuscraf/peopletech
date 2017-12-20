//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapitit.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapItIt
//
//==============================================================================

#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapitit.h"
#include "index.hpp"
#include "item.hpp"
#include "wrapitemdef.h"
#include "wrapslit.h"
#include "wrapcvm.h"
#include "wrapdef.h"
#include "gstring.h"
#include "newinterface.h"
#include "rerror.h"
#include "node.hpp"
#include "m4var.hpp"
#include "rm.hpp"
#include "regtot.hpp"
#include "totalize.hpp"
#include "wrapexecutor.h"
#include "m4unicode.hpp"
#include "node_knl.hpp"
#include "chan_knl.hpp"
#include "pl_data.hpp"

#ifdef _DEBUG
#	include <ctype.h>
#endif

void	M4ResetExceptions( void ) ;


/////////////////////////////////////////////////////////////////////////////
// CWrapItIt
STDMETHODIMP CWrapItIt::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = {	&IID_IWrapItIt,	&IID_IEnumVARIANT };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}


CWrapItIt::CWrapItIt() 
	: m_poItem(0), 
	  m_poSliceIterator(0),
	  m_IsClone(M4_FALSE),
      m_pItemDef(0)
{
}


CWrapItIt::~CWrapItIt()
{
	if (m_IsClone == M4_TRUE)
	{
		delete m_poItem;
		m_IsClone = M4_FALSE;
	}

	m_poItem = 0;
	m_poSliceIterator = 0;

    if (m_pItemDef)
	{
        m_pItemDef->Release();
        m_pItemDef = 0;
    }
}


void CWrapItIt::SetIsClone(m4bool_t ai_flag)
{
	m_IsClone = ai_flag;
}


m4bool_t CWrapItIt::GetIsClone(void)
{
	return m_IsClone;
}


void CWrapItIt::Init(ClItem* ai_poItem)
{
	m_poItem = ai_poItem;

	TRACE3("Initialising with item address %p and index %d", m_poItem, (long)m_poItem->GetIndex());
}


STDMETHODIMP CWrapItIt::get_IdItem(long* pVal)
{
	ENTER_METHOD(CWrapItIt::get_IdItem);

	*pVal = m_poItem->GetIndex();

	TRACE2("Returning item id %d", *pVal);
	LEAVE_METHOD(CWrapItIt::get_IdItem, S_OK);
}


STDMETHODIMP CWrapItIt::get_Name(BSTR* pVal)
{
	ENTER_METHOD(CWrapItIt::get_Name);

	gstring tmpGstring(m_poItem->ItemDef.Id());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItIt::get_Name, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CREATE_STRING, "Cannot create a new string");
		LEAVE_METHOD(CWrapItIt::get_Name, E_UNEXPECTED);
	}
}


STDMETHODIMP CWrapItIt::get_OldValue(VARIANT* pVal)
{
	m4VariantType var;

	ENTER_METHOD(CWrapItIt::get_OldValue);

	m_poItem->OldValue.Get(var);

	var.GetVARIANT(*pVal);

	//
	// For commentary on this see cvadte.cpp
	//
#ifdef _ZERO_DATE_IS_ERR

	if (pVal->vt == VT_DATE && !pVal->date)
	{
		FormatError(M4_CH_WO_INVALID_DATE, "Invalid date");
		LEAVE_METHOD(CWrapItIt::get_OldValue, E_UNEXPECTED);
	}
#endif

	LEAVE_METHOD(CWrapItIt::get_OldValue, S_OK);
}


HRESULT	_GetValue( ClItem *ai_poItem, VARIANT *ao_poVariant )
{

	m4uint32_t		iSlices = 0 ;
	m4VariantType	vValue ;

	
	// Se lee el valor
	iSlices = ai_poItem->Value.Count() ;

	if( iSlices == 0 )
	{
		ai_poItem->Value.Get( vValue ) ;
	}
	else
	{
		ai_poItem->Value.GetpSliceFooter().Get( vValue, -1 ) ;
	}

	if( ai_poItem->ItemDef.M4Type() != M4CL_M4_TYPE_BINARY_STRING )
	{
		vValue.GetVARIANT( *ao_poVariant );

#ifdef _ZERO_DATE_IS_ERR
		if( ao_poVariant->vt == VT_DATE && !ao_poVariant->date )
		{
			FormatError( M4_CH_WO_INVALID_DATE, "Invalid date" ) ;
			return( E_UNEXPECTED ) ;
		}
#endif
	}
	else
	{
		// Se trata de un ítem binario.
		ao_poVariant->vt = VT_BSTR;

		if( vValue.Data.PointerChar != NULL )
		{
			// No trailing '\0'
			size_t	wcLength = ai_poItem->Value.Size() - 1;  

			// UNICODE VB
			ao_poVariant->bstrVal = M4CppToVBBinary( vValue.Data.PointerChar, wcLength ) ;
		}
		else
		{
			// El puntero nunca debería de ser nulo.
			ao_poVariant->bstrVal = SysAllocString( L"" ) ;
		}
	}

	return( S_OK ) ;
}


STDMETHODIMP CWrapItIt::get_Value( VARIANT* ao_poVariant )
{
	ENTER_METHOD( CWrapItIt::get_Value ) ;

	HRESULT	hResult = _GetValue( m_poItem, ao_poVariant ) ;
	LEAVE_METHOD( CWrapItIt::get_Value, hResult ) ;
}


HRESULT	_SetValue( ClItem *ai_poItem, VARIANT& ai_roVariant )
{

	m4return_t iResult = M4_ERROR ;

	
	if( ai_roVariant.vt != VT_BSTR || ai_poItem->ItemDef.M4Type() != M4CL_M4_TYPE_BINARY_STRING )
	{
		m4VariantType vValue( ai_roVariant ) ;
		iResult = ai_poItem->Value.Set( vValue ) ;
	}
	else
	{
		// Se trata de un ítem binario.
		size_t	mbLength = SysStringLen( ai_roVariant.bstrVal ) ;

		// UNICODE VB
		m4pchar_t	pcStr = M4VBToCppBinary( ai_roVariant.bstrVal, mbLength ) ;

		// Cuando hacemos set del ítem hay que tener en cuenta el 0 del final.
		iResult = ai_poItem->Value.Set( pcStr, mbLength + 1 ) ;
		delete[] pcStr;
	}
	
	if( iResult == M4_ERROR )
	{
		return( E_UNEXPECTED ) ;
	}
	return( S_OK ) ;
}


STDMETHODIMP CWrapItIt::put_Value( VARIANT ai_oVariant )
{
	ENTER_METHOD( CWrapItIt::put_Value ) ;
	
	HRESULT	hResult = _SetValue( m_poItem, ai_oVariant ) ;
	LEAVE_METHOD( CWrapItIt::put_Value, hResult ) ;
}


STDMETHODIMP CWrapItIt::SetValue(/*[in]*/ VARIANT ai_oVariant, /*[out]*/ VARIANT* ao_poVariant, /*[out]*/ ULONGLONG* ao_plOldTime, /*[out]*/ ULONGLONG* ao_plNewTime, /*[out, retval]*/ short* ao_piReturn)
{
	ENTER_METHOD( CWrapItIt::SetValue ) ;

	HRESULT		hResult = S_OK ;
	m4uint64_t	lTime = 0 ;
	ClNode		*poNode = NULL ;


	*ao_plOldTime = 0 ;
	*ao_plNewTime = 0 ;
	*ao_piReturn = M4_ERROR ;

	poNode = m_poItem->GetpNodeAccess() ;

	if( poNode != NULL )
	{
		switch( m_poItem->ItemDef.Scope() )
		{
			case	M4CL_ITEM_SCOPE_NODE :
				lTime = poNode->RecordSet.GetNodeTimeStamp() ;
				break ;

			case	M4CL_ITEM_SCOPE_BLOCK :
				lTime = poNode->RecordSet.GetBlockTimeStamp() ;
				break ;

			case	M4CL_ITEM_SCOPE_REGISTER :
				lTime = poNode->RecordSet.GetRecordTimeStamp() ;
				break ;

			default:
				lTime = 0 ;
				break ;
		}

		*ao_plOldTime = lTime ;
	}

	hResult = _SetValue( m_poItem, ai_oVariant ) ;

	_GetValue( m_poItem, ao_poVariant ) ;

	if( poNode != NULL )
	{
		switch( m_poItem->ItemDef.Scope() )
		{
			case	M4CL_ITEM_SCOPE_NODE :
				lTime = poNode->RecordSet.GetNodeTimeStamp() ;
				break ;

			case	M4CL_ITEM_SCOPE_BLOCK :
				lTime = poNode->RecordSet.GetBlockTimeStamp() ;
				break ;

			case	M4CL_ITEM_SCOPE_REGISTER :
				lTime = poNode->RecordSet.GetRecordTimeStamp() ;
				break ;

			default:
				lTime = 0 ;
				break ;
		}

		*ao_plNewTime = lTime ;
	}

	if( hResult != S_OK )
	{
		LEAVE_METHOD( CWrapItIt::SetValue, S_OK ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD( CWrapItIt::SetValue, S_OK ) ;
}


STDMETHODIMP CWrapItIt::Call(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapItIt::Call);

	*ao_sRetVal = m_poItem->Call(0,-1,M4_TRUE); //0=default executor, -1=args in stack, M4_true=enable debug

	TRACE2("ItemIterator: Call returns %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapItIt::Call, S_OK);
}


STDMETHODIMP CWrapItIt::CallWithArgs(/*[in]*/ int ai_iNumArgs,short* ao_sRetVal)
{
	ENTER_METHOD(CWrapItIt::CallWithArgs);

	*ao_sRetVal = m_poItem->Call(0,ai_iNumArgs,M4_TRUE); //m4_true: enable debug

	TRACE2("ItemIterator: CallWithArgs returns %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapItIt::CallWithArgs, S_OK);
}


/**
 * CWrapItIt::get_ItemDef - Implements the IWrapItIt::ItemDef get 
 * property. Returns a reference to an IWrapItemDef object, that allows 
 * to ask questions about the underlying item.
 */
STDMETHODIMP CWrapItIt::get_ItemDef(IWrapItemDef** pVal)
{
	ENTER_METHOD(CWrapItIt::get_ItemDef);

	*pVal = NULL;

	// Optimization: only one CWrapItemDef per ItIt is necessary,
	// no need to create a new one each time. Changed CWrapItemDef
    // in order to avoid index caching.
	if (m_pItemDef == NULL) 
	{
		if (FAILED(M4_CREATE_INTERFACE(CWrapItemDef, pVal)))
		{
			LEAVE_METHOD(CWrapItIt::get_ItemDef, E_UNEXPECTED);
		}

		m_pItemDef = *pVal;
		((CWrapItemDef*)*pVal)->Init(m_poItem);
	}
	else
	{
		// Return the cached operation object
		*pVal = m_pItemDef;
	}

    static_cast<IUnknown*>(*pVal)->AddRef();
	
	LEAVE_METHOD(CWrapItIt::get_ItemDef, S_OK);
}



//
// Functions from WrapItems
//
m4return_t CWrapItIt::GetItemByName(BSTR ai_name,  IWrapItIt** ao_item)
{
	gstring tmpGstring(ai_name);

    //Edu:
	m4bool_t noteof = m_poItem->MoveTo(tmpGstring);

	if (noteof)
	{
		*ao_item = this;
		((IUnknown*)*ao_item)->AddRef();
		return M4_SUCCESS;
	}
	else // Is EOF
	{
		*ao_item = 0;
		FormatError(M4_CH_WO_NO_SUCH_ITEM, "Cannot move to item \"%s\"", m4pchar_t(tmpGstring));
		return M4_ERROR;
	}
}


m4return_t CWrapItIt::GetItemByPos(int ai_pos, IWrapItIt** ao_item)
{	
	m4bool_t noteof = m_poItem->MoveTo((ClItemIndex)ai_pos);

	if (noteof)
	{
		*ao_item = this;
		((IUnknown*)*ao_item)->AddRef();
		return M4_SUCCESS;
	}
	else // Is EOF
	{
		*ao_item = 0;
		return M4_ERROR;
	}
}


STDMETHODIMP CWrapItIt::get_Count(long * pVal)
{
	ENTER_METHOD(CWrapItIt::get_Count);

	*pVal = m_poItem->Count();

	TRACE2("The number of items is %d", *pVal);
	LEAVE_METHOD(CWrapItIt::get_Count, S_OK);
}


STDMETHODIMP CWrapItIt::Item(VARIANT ai_var, IWrapItIt** ao_item)
{
	// Pos si se han descuadrado las excepciones se van reseteando
	M4ResetExceptions() ;

	ENTER_METHOD(CWrapItIt::Item);

	m4return_t eof;

	switch (ai_var.vt)
	{
	case VT_BSTR:
		eof = GetItemByName(ai_var.bstrVal, ao_item);

		if (eof != M4_SUCCESS)
		{
			// Ya sale el mensaje por debajo
			LEAVE_METHOD(CWrapItIt::Item, E_UNEXPECTED);
		}
		break;

	case VT_UI1:
		eof = GetItemByPos(ai_var.bVal, ao_item);
		break;

	case VT_I2:
		eof = GetItemByPos(ai_var.iVal, ao_item);
		break;

	case VT_I4:
		eof = GetItemByPos(ai_var.lVal, ao_item);
		break;

	default:
		FormatError(M4_CH_WO_INVALID_GET_ITEM, "Cannot get item given variant type %d", ai_var.vt);
		LEAVE_METHOD(CWrapItIt::Item, E_UNEXPECTED);
		break;
	}

	if (eof != M4_SUCCESS)
	{
		FormatError(M4_CH_WO_NO_SUCH_ITEM, "Cannot move to item");
		LEAVE_METHOD(CWrapItIt::Item, E_UNEXPECTED);
	}

	LEAVE_METHOD(CWrapItIt::Item, S_OK);
}


STDMETHODIMP CWrapItIt::_NewEnum(IUnknown** pVal)
{
	ENTER_METHOD(CWrapItIt::_NewEnum);

	*pVal = (IEnumVARIANT*) this;
	((IUnknown*)*pVal)->AddRef();
	
	LEAVE_METHOD(CWrapItIt::_NewEnum, S_OK);
}


//
// From IEnum
//
STDMETHODIMP CWrapItIt::Next(ULONG celt, VARIANT* rgvar, ULONG* pceltFetched)
{
	ENTER_METHOD(CWrapItIt::Next);

	ULONG l2 = 0;

	if (pceltFetched != NULL)
	{
		*pceltFetched = 0;
	}

	m4uint32_t Count = m_poItem->Count();

	ClItemIndex oIndex = m_poItem->GetIndex() + 1;

	if (oIndex < Count)
    {
		m_poItem->MoveTo(oIndex);
		VariantInit(&rgvar[0]);

		rgvar[0].vt = VT_UNKNOWN;// | VT_BYREF;
		//&(rgvar[l2].punkVal)
		rgvar[0].punkVal = (IWrapItIt*) this;
		((IUnknown*)rgvar[0].punkVal)->AddRef();

		// Set count of elements retrieved.
		if (pceltFetched != NULL)
		{
			*pceltFetched = 1;
		}

		l2++;
	}

	LEAVE_METHOD(CWrapItIt::Next, (l2 < celt) ? S_FALSE : S_OK);
}


STDMETHODIMP CWrapItIt::Skip(ULONG celt)
{
	ENTER_METHOD(CWrapItIt::Skip);

	m4uint32_t Count = m_poItem->Count();

	ClItemIndex oIndex = m_poItem->GetIndex() + celt;

	if (oIndex > Count)
	{
		oIndex = Count - 1;
		LEAVE_METHOD(CWrapItIt::Skip, S_FALSE);
	}

	m_poItem->MoveTo(oIndex);
	LEAVE_METHOD(CWrapItIt::Skip, S_OK);
}


STDMETHODIMP CWrapItIt::Reset()
{
	ENTER_METHOD(CWrapItIt::Reset);

	m_poItem->MoveTo((ClItemIndex)0);

	LEAVE_METHOD(CWrapItIt::Reset, S_OK);
}


STDMETHODIMP CWrapItIt::Clone(IEnumVARIANT** ppenum)
{
	ENTER_METHOD(CWrapItIt::Clone);

	ClItem* poItem = new ClItem (m_poItem->GetpAccess ());

	if (poItem)
	{
#ifdef M4_NEWINTERFACE
		if (NewInterface(CLSID_WrapItIt, IID_IEnumVARIANT, (IUnknown**)ppenum) != M4_SUCCESS)
		{
			delete poItem;
			LEAVE_METHOD(CWrapItIt::Clone, E_UNEXPECTED);
		}
#else
		HRESULT hRes;
		if (FAILED(hRes = M4_CREATE_INTERFACE(CWrapItIt, ppenum))) 
		{
			delete poItem;
			LEAVE_METHOD(CWrapItIt::Clone, hRes);
		}
#endif
	
		// Use the assignment operator to do the copy
		*poItem = *m_poItem;
	
		((CWrapItIt*)(*ppenum))->Init(poItem);
		((CWrapItIt*)(*ppenum))->SetIsClone(M4_TRUE);

		LEAVE_METHOD(CWrapItIt::Clone, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANNOT_CLONE_ITEM, "Cannot create a clone of the item");
		LEAVE_METHOD(CWrapItIt::Clone, E_UNEXPECTED);
	}
}


STDMETHODIMP CWrapItIt::get_Copy(IWrapItIt** pVal)
{
	ENTER_METHOD(CWrapItIt::get_Copy);

	ClItem* poItem = new ClItem (m_poItem->GetpAccess ());

	if (poItem)
	{
		if (NewInterface(CLSID_WrapItIt, IID_IWrapItIt, (IUnknown**) pVal) != M4_SUCCESS)
		{
			delete poItem;
			LEAVE_METHOD(CWrapItIt::get_Copy, E_UNEXPECTED);
		}
	
		// use the overloaded assignment operator to do
		// the copying.
		*poItem = *m_poItem;

		((CWrapItIt*)(*pVal))->Init(poItem);
		((CWrapItIt*)(*pVal))->SetIsClone(M4_TRUE);

		LEAVE_METHOD(CWrapItIt::get_Copy, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANNOT_COPY_ITEM, "Cannot create a copy of the item");
		LEAVE_METHOD(CWrapItIt::Copy, E_UNEXPECTED);
	}
}


STDMETHODIMP CWrapItIt::get_SliceIterator(IWrapSlIt** pVal)
{
	ENTER_METHOD(CWrapItIt::get_SliceIterator);

	if (!m_poSliceIterator)
	{
		if (NewInterface(CLSID_WrapSlIt, IID_IWrapSlIt, (IUnknown**) &m_poSliceIterator) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapItIt::get_SliceIterator, E_UNEXPECTED);
		}

		((CWrapSlIt*)m_poSliceIterator)->Init(&m_poItem->Value);
	}

	*pVal = m_poSliceIterator;
	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapItIt::get_SliceIterator, S_OK);
}


STDMETHODIMP CWrapItIt::get_RecordSetFooterValue(int ai_operation,VARIANT* ao_poVariant)
{
	ENTER_METHOD(CWrapItIt::get_RecordSetFooterValue);

	m4VariantType var;

	//liberamos lo venga en el arg.
	if (FAILED(VariantClear(ao_poVariant) ))
	{
		FormatError(M4_CH_WO_CANT_COPY_VARIANT, "Cannot free variant");
		LEAVE_METHOD(CWrapItIt::get_RecordSetFooterValue, E_UNEXPECTED);
	}

	_ClRegister_Base * pReg = m_poItem->Value.GetpRegister();

	if (pReg)
	{
		ClRegTotalize oRegTotalize;
		oRegTotalize.Init(pReg);
		oRegTotalize.Get(var, ai_operation);
	}
	else
	{
		//Error: Devolvemos NULL
	}

	var.GetVARIANT(*ao_poVariant);

	//
	// For commentary on this see cvadte.cpp
	//
#ifdef _ZERO_DATE_IS_ERR
	if (ao_poVariant->vt == VT_DATE && !ao_poVariant->date)
	{
		FormatError(M4_CH_WO_INVALID_DATE, "Invalid date");
		LEAVE_METHOD(CWrapItIt::get_RecordSetFooterValue, E_UNEXPECTED);
	}
#endif

	LEAVE_METHOD(CWrapItIt::get_RecordSetFooterValue, S_OK);
}


STDMETHODIMP CWrapItIt::get_SliceFooterValue(int ai_operation, VARIANT* ao_poVariant)
{
	ENTER_METHOD(CWrapItIt::get_SliceFooterValue);

	m4VariantType var;

	//liberamos lo venga en el arg.
	if (FAILED(VariantClear(ao_poVariant) ))
	{
		FormatError(M4_CH_WO_CANT_COPY_VARIANT, "Cannot free variant");
		LEAVE_METHOD(CWrapItIt::get_SliceFooterValue, E_UNEXPECTED);
	}

	m_poItem->Value.GetpSliceFooter().Get(var, ai_operation);
	var.GetVARIANT(*ao_poVariant);

	//
	// For commentary on this see cvadte.cpp
	//
#ifdef _ZERO_DATE_IS_ERR
	if (ao_poVariant->vt == VT_DATE && !ao_poVariant->date)
	{
		FormatError(M4_CH_WO_INVALID_DATE, "Invalid date");
		LEAVE_METHOD(CWrapItIt::get_SliceFooterValue, E_UNEXPECTED);
	}
#endif

	LEAVE_METHOD(CWrapItIt::get_SliceFooterValue, S_OK);
}


void CWrapItIt::UnWrap (ClItem ** ao_poItem)
{
	*ao_poItem = m_poItem;
}


STDMETHODIMP CWrapItIt::CreateBlobFile(VARIANT_BOOL * pRet, BSTR DirName, BSTR Extension)
{
	ENTER_METHOD(CWrapItIt::CreateBlobFile);

	gstring tmpDirName(DirName);
	gstring tmpExtension(Extension);

	m4return_t res;

	res = m_poItem->Value.CreateTempFile((m4pchar_t)tmpDirName,(m4pchar_t)tmpExtension);
	*pRet = (res == M4_SUCCESS) ? VARIANT_TRUE : VARIANT_FALSE;

	LEAVE_METHOD(CWrapItIt::CreateBlobFile, S_OK);
}


STDMETHODIMP CWrapItIt::ChangedMask(long * pMask)
{
	ENTER_METHOD(CWrapItIt::ChangedMask);

	m4return_t res;
	m4uint32_t iMask = 0;

	res = m_poItem->Value.ChangedMask(iMask);

	if (res == M4_SUCCESS)
	{
		*pMask = iMask;
		LEAVE_METHOD(CWrapItIt::ChangedMask, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItIt::ChangedMask, S_FALSE);
	}
}


STDMETHODIMP CWrapItIt::SetDescr(BSTR Descr)
{
	ENTER_METHOD(CWrapItIt::SetDescr);

	gstring tmpDescr(Descr);
	m4return_t res;

	res = m_poItem->Value.SetDescr(tmpDescr);

	if (res == M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapItIt::SetDescr, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItIt::SetDescr, S_FALSE);
	}
}


STDMETHODIMP CWrapItIt::GetDescr(BSTR * Descr)
{
	ENTER_METHOD(CWrapItIt::GetDescr);

	m4pchar_t pcDescr = m_poItem->Value.GetDescr();
	gstring tmpDescr(pcDescr!=NULL?pcDescr:"");
	*Descr = tmpDescr;

	LEAVE_METHOD(CWrapItIt::GetDescr, S_OK);
}


STDMETHODIMP  CWrapItIt::get_Executor(IWrapExecutor **pVal)
{
	ENTER_METHOD(CWrapItIt::get_Executor);
	TRACE("Allocating new executor");
	
	if (NewInterface(CLSID_WrapExecutor,IID_IWrapExecutor,(IUnknown**) pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapRgIt::get_Executor,	E_UNEXPECTED);
	}
	
	// Inicializar  el ejecutor con la factoría para los breakpoints
	if( m_poItem != NULL )
	{
		ClAccess *poAccess = m_poItem->GetpAccess() ;

		if( poAccess != NULL )
		{
			ClChannel *poChannel = poAccess->GetpChannel() ;

			if( poChannel != NULL )
			{
				ClChannelManager *poChannelManager = poChannel->GetpChannelManager() ;

				if( poChannelManager != NULL )
				{
					((CWrapExecutor*)(*pVal))->SetFactory( poChannelManager->GetpFactory() ) ;
				}
			}
		}
	}

	m4return_t ret = ((CWrapExecutor*)(*pVal))->Init((ClExecutor*)m_poItem->GetpAccess()->GetpExecutor());

	if (ret!=M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapItIt::get_Executor,E_UNEXPECTED);
	}

	LEAVE_METHOD(CWrapItIt::get_Executor, S_OK);
}


/*
Lee los índices de los items y los devuelve en un safe array
Ademas devuelve el identificador del item
El acceso es primero por columna y luego por fila
*/
STDMETHODIMP CWrapItIt::ReadItemIndexes(/*[out]*/ SAFEARRAY** ao_avValues, /*[out, retval]*/ short* ao_piReturn)
{
	ENTER_METHOD(CWrapItIt::ReadItemIndexes);

	HRESULT			hResult = S_OK ;
	m4return_t		iResult = M4_SUCCESS ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iColumn = 0 ;
	m4uint32_t		iItems = 0 ;
	m4uint32_t		iColumns = 0 ;
	SAFEARRAYBOUND	rgsabound[ 2 ] ;
	VARIANT			*pvValues = NULL ;


	*ao_piReturn = M4_ERROR ;

	if( *ao_avValues != NULL )
	{
		SafeArrayDestroy( *ao_avValues ) ;
		*ao_avValues = NULL ;
	}

	iItems = m_poItem->Count() ;

	// Se calcula el número de columnas con valor
	iColumns = 0 ;

	for( i = 0 ; i < iItems ; i++ )
	{
		m_poItem->MoveTo( i ) ;

		if( m_poItem->ItemDef.Type() != M4CL_ITEM_TYPE_METHOD )
		{
			iColumns++ ;
		}
	}

	// Se crea el array de dos dimensiones, filas por 2 valores
	rgsabound[ 0 ].lLbound = 0 ;
	rgsabound[ 0 ].cElements = 2 ;
	rgsabound[ 1 ].lLbound = 0 ;
	rgsabound[ 1 ].cElements = iColumns ;

	*ao_avValues = SafeArrayCreate( VT_VARIANT, 2, rgsabound ) ;

	if( *ao_avValues == NULL )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot create SafeArray" ) ;
		LEAVE_METHOD( CWrapCVM::ReadItemIndexes, S_OK ) ;
	}

	hResult = SafeArrayAccessData( *ao_avValues, (void**) &pvValues ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot access SafeArray" ) ;
		SafeArrayDestroy( *ao_avValues ) ;
		*ao_avValues = NULL ;
		LEAVE_METHOD( CWrapCVM::ReadItemIndexes, S_OK ) ;
	}

	if(	iColumns > 0 )
	{
		iColumn = 0 ;

		for( i = 0 ; i < iItems ; i++ )
		{
			m_poItem->MoveTo( i ) ;

			if( m_poItem->ItemDef.Type() != M4CL_ITEM_TYPE_METHOD )
			{
				// Se escribe el nombre
				pvValues[ iColumn * 2 ].vt = VT_BSTR ;
				pvValues[ iColumn * 2 ].bstrVal = M4CppToVB( m_poItem->ItemDef.Id() ) ;

				// Se escribe el índice
				pvValues[ iColumn * 2 + 1 ].vt = VT_R8 ;
				pvValues[ iColumn * 2 + 1 ].dblVal = i ;

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
		LEAVE_METHOD( CWrapCVM::ReadItemIndexes, S_OK ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD(CWrapItIt::ReadItemIndexes, S_OK);
}


STDMETHODIMP CWrapItIt::CallTrackingChanges(/*[in]*/ int ai_iNumArgs, /*[out]*/ VARIANT_BOOL *ao_pbChanges, /*[out, retval]*/ short* ao_sRetVal)
{
	ENTER_METHOD(CWrapItIt::CallTrackingChanges);

	*ao_pbChanges = VARIANT_TRUE ;

	TRACE2("ItemIterator: CallTrackingChanges returns %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapItIt::CallTrackingChanges, S_OK);
}


