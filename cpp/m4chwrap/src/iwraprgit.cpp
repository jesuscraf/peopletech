//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             iwraprgit.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CIWrapRgIt
//
//==============================================================================

#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "iwraprgit.h"
#include "wraprgit.h"
#include "index.hpp"
#include "aregister.hpp"
#include "wrapoper.h"
#include "wrapdef.h"
#include "wrapcvm.h"
#include "newinterface.h"
#include "wrapitit.h"
#include "rerror.h"
#include "recorset.hpp"
#include "access.hpp"
#include "executor.hpp"
#include "node_knl.hpp"
#include "bookmark.hpp"
#include "m4mdrt.hpp"
#include "file_misc.hpp"
#include "pl_data.hpp"


/////////////////////////////////////////////////////////////////////////////
// CIWrapRgIt


CIWrapRgIt::CIWrapRgIt()
		: m_poRegister(0)
		, m_poItemIterator(0)
		, m_IsClone(M4_FALSE)
		, m_pOperation(0)
{
}


/**
 *  CIWrapRgIt::~CIWrapRgIt - Frees allocated resources and
 *  releases its references on dependent objects.
 */
CIWrapRgIt::~CIWrapRgIt()
{
	if (m_IsClone == M4_TRUE)
	{
		delete m_poRegister;
		m_IsClone = M4_FALSE;
	}

	m_poRegister = 0;

	if (m_poItemIterator)
	{
		m_poItemIterator->Release();
		m_poItemIterator = 0;
	}

	if (m_pOperation)
	{
		m_pOperation->Release();
		m_pOperation = 0;
	}
}


void CIWrapRgIt::SetIsClone(m4bool_t ai_flag)
{
	m_IsClone = ai_flag;
}


m4bool_t CIWrapRgIt::GetIsClone(void)
{
	return m_IsClone;
}


void  CIWrapRgIt::Init(_ClRegister_Base* ai_poRegister)
{
	m_poRegister = ai_poRegister;

    CIWrapRgItFilter::Init (&m_poRegister->Filter);

	TRACE3("Initialising with register address %p and index %d", m_poRegister, m_poRegister->Index);
}


STDMETHODIMP CIWrapRgIt::CheckPoint(short* ao_sRetVal)
{
	ENTER_METHOD(CIWrapRgIt::CheckPoint);

	*ao_sRetVal = m_poRegister->CheckPoint();

	LEAVE_METHOD(CIWrapRgIt::CheckPoint, S_OK);
}


STDMETHODIMP CIWrapRgIt::Undo(short* ao_sRetVal)
{
	ENTER_METHOD(CIWrapRgIt::Undo);

	*ao_sRetVal = m_poRegister->Undo();

	LEAVE_METHOD(CIWrapRgIt::Undo, S_OK);
}


STDMETHODIMP CIWrapRgIt::Remove(short* ao_sRetVal)
{
	ENTER_METHOD(CIWrapRgIt::Remove);

	m4return_t	iResult ;

// First search for item with delete register method

// jcr para comprobar que existe al DELETEREGISTER
	ClNodeDef		*poNodeDef = m_poRegister->Item.GetpNodeAccess()->GetpNodeDef() ;
	ClCompiledMCR	*poCMCR = poNodeDef->GetpCMCR() ;
	m4uint32_t		iNodeHandle = poNodeDef->GetHandle() ;
	ClHandle		iItemHandle = poCMCR->GetNodeItemHandleById( iNodeHandle, "DELETEREGISTER" ) ;


	if( iItemHandle != 0 )
	{
		// Store the current index so we can
		// restore later
		ClItemIndex oIndex = m_poRegister->Index;

		if( m_poRegister->Item.MoveTo(iItemHandle) == M4_TRUE )
		{
			m4VariantType TmpVar ;
			
            ClAccess * poAccess = m_poRegister->GetpAccess();
            ClNode * poNode = poAccess->Node.Get((ClHandle)iNodeHandle);

            ClAccessBookMark oBookMark;
            oBookMark.GetFrom (* poAccess);

            ClRegisterIndex RegIndex = m_poRegister->Index;
            poNode->RecordSet.Current.MoveTo(RegIndex);

			// Is not EOF therefore call the method
			iResult = m_poRegister->Item.Call(0,-1,M4_TRUE);

            oBookMark.ApplyTo (* poAccess);

            if( iResult != M4_SUCCESS )
			{
				*ao_sRetVal = M4_ERROR ;
			}
			else
			{
				// After the call of a method we always need to
				// pop the stack...follow the pointers, and throw
				// away the result

				iResult = m_poRegister->Item.GetpAccess()->GetpExecutor()->Stack.Pop(TmpVar);

				if( iResult != M4_SUCCESS )
				{
					*ao_sRetVal = M4_ERROR ;
				}
				else
				{
					*ao_sRetVal = short( TmpVar.Data.DoubleData ) ;
				}
			}
		}
		else
		{
			*ao_sRetVal = m_poRegister->Delete();
		}

		// restore the item
		m_poRegister->Item[oIndex];

	}
	else
	{
		*ao_sRetVal = m_poRegister->Delete();
	}

	LEAVE_METHOD(CIWrapRgIt::Remove, S_OK);
}


STDMETHODIMP CIWrapRgIt::get_IsEof(VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CIWrapRgIt::get_IsEof);

	if (m_poRegister->Index == M4DM_EOF_INDEX)
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CIWrapRgIt::get_IsEof, S_OK);
}


/**
 * CIWrapRgIt::get_Operation - Implements the IWrapRgIt::Operation get 
 * property. Returns a reference to an IWrapOper object, that allows 
 * to ask questions about the underlying register.
 */
STDMETHODIMP CIWrapRgIt::get_Operation(IWrapOper** pVal)
{
	ENTER_METHOD(CIWrapRgIt::get_Operation);
	
	*pVal = NULL;

	// Optimization: only one CWrapOper per RgIt is necessary,
	// no need to create a new one each time.
	if (m_pOperation == NULL) 
	{
		if (FAILED(M4_CREATE_INTERFACE(CWrapOper, pVal)))
		{
			LEAVE_METHOD(CIWrapRgIt::get_Operation, E_UNEXPECTED);
		}
		m_pOperation = *pVal;
		((CWrapOper*)*pVal)->Init(&m_poRegister->Operation);
	}
	else
	{
		// Return the cached operation object
		*pVal = m_pOperation;
	}

	static_cast<IUnknown*>(*pVal)->AddRef();

	LEAVE_METHOD(CIWrapRgIt::get_Operation, S_OK);
}


STDMETHODIMP CIWrapRgIt::get_Index(long* pVal)
{
	ENTER_METHOD(CIWrapRgIt::get_Index);

	*pVal = m_poRegister->Index;

	TRACE2("Returning index %d", *pVal);
	LEAVE_METHOD(CIWrapRgIt::get_Index, S_OK);
}


STDMETHODIMP CIWrapRgIt::Free(short* ao_RetVal)
{
	ENTER_METHOD(CIWrapRgIt::Free);

	// Bug 0113644 Deprecated. No longer supported.
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CIWrapRgIt::Free" << "M4ChWrap.dll" );
	LEAVE_METHOD(CIWrapRgIt::Free, S_OK);
}


STDMETHODIMP CIWrapRgIt::get_ItemIterator(IWrapItIt** pVal)
{
	ENTER_METHOD(CIWrapRgIt::get_ItemIterator);

	if (!m_poItemIterator)
	{
		TRACE("Allocating new item iterator");

#ifdef M4_NEWINTERFACE
		if (NewInterface(CLSID_WrapItIt, IID_IWrapItIt, (IUnknown**) &m_poItemIterator) != M4_SUCCESS)
		{
			LEAVE_METHOD(CIWrapRgIt::get_ItemIterator, E_UNEXPECTED);
		}
#else
		HRESULT hRes;
		if (FAILED(hRes = M4_CREATE_INTERFACE(CWrapItIt, &m_poItemIterator))) 
		{
			LEAVE_METHOD(CIWrapRgIt::get_ItemIterator, hRes);
		}
#endif
		((CWrapItIt*)m_poItemIterator)->Init(&m_poRegister->Item);
	}

	TRACE2("The item iterator has address %p", m_poItemIterator);

	*pVal = m_poItemIterator;
	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CIWrapRgIt::get_ItemIterator, S_OK);
}


STDMETHODIMP CIWrapRgIt::Item(int ai_iIndex, IWrapRgIt** ao_poRegister)
{
	ENTER_METHOD(CIWrapRgIt::Item);

	m_poRegister->MoveTo((ClRegisterIndex)ai_iIndex);

#ifdef NO_REG_ERROR
	if (m_poRegister->Index != M4DM_EOF_INDEX)
	{
#endif
		*ao_poRegister = this;
		((IUnknown*)*ao_poRegister)->AddRef();

#ifdef NO_REG_ERROR
	}
	else
	{
		FormatError(M4_CH_WO_NO_SUCH_ITEM, "Cannot move to register %d", ai_iIndex);
		LEAVE_METHOD(CIWrapRgIt::Item, E_UNEXPECTED);
	}
#endif

	LEAVE_METHOD(CIWrapRgIt::Item, S_OK);
}


STDMETHODIMP CIWrapRgIt::get_Count(long* ao_iCount)
{
	ENTER_METHOD(CIWrapRgIt::get_Count);

	*ao_iCount = m_poRegister->Count();

	TRACE2("Returning a count of %d", *ao_iCount);
	LEAVE_METHOD(CIWrapRgIt::get_Count, S_OK);
}


STDMETHODIMP CIWrapRgIt::get_New(IWrapRgIt** ao_poRegister)
{
	ENTER_METHOD(CIWrapRgIt::get_New);

	m4return_t	iResult ;

// First search for item with add register method

// jcr para comprobar que existe al ADDREGISTER
	ClNodeDef		*poNodeDef = m_poRegister->Item.GetpNodeAccess()->GetpNodeDef() ;
	ClCompiledMCR	*poCMCR = poNodeDef->GetpCMCR() ;
	m4uint32_t		iNodeHandle = poNodeDef->GetHandle() ;
	ClHandle		iItemHandle = poCMCR->GetNodeItemHandleById( iNodeHandle, "ADDREGISTER" ) ;


	if( iItemHandle != 0 )
	{
		// Store the current index so we can
		// restore later
		ClItemIndex oIndex = m_poRegister->Index;

		if( m_poRegister->Item.MoveTo(iItemHandle) == M4_TRUE )
		{
			m4VariantType TmpVar ;
			
			// Is not EOF therefore call the method
			iResult = m_poRegister->Item.Call(0,-1,M4_TRUE);

			if( iResult == M4_SUCCESS )
			{
				//restore de register
				m_poRegister->MoveTo((ClRegisterIndex)((m_poRegister->Count())-1));
			}

			// After the call of a method we always need to
			// pop the stack...follow the pointers, and throw
			// away the result

			m_poRegister->Item.GetpAccess()->GetpExecutor()->Stack.Pop(TmpVar);
		}
		else
		{
			// Call the Add function of register as no method exists
			m_poRegister->Add();
		}

		// restore the item
		m_poRegister->Item[oIndex];
	}
	else
	{
		// Call the Add function of register as no method exists
		m_poRegister->Add();
	}


	// Return this object, but with the register pointing
	// to the newly added register

	*ao_poRegister = this;
	((IUnknown*)*ao_poRegister)->AddRef();

	LEAVE_METHOD(CIWrapRgIt::get_New, S_OK);
}


//EDU:
STDMETHODIMP CIWrapRgIt::get_Insert (IWrapRgIt** ao_poRegister)
{
	ENTER_METHOD(CIWrapRgIt::get_Insert);

    //de momento sin BOLOX

    m_poRegister->Insert();

	// Return this object, but with the register pointing
	// to the newly added register

	*ao_poRegister = this;
	((IUnknown*)*ao_poRegister)->AddRef();

	LEAVE_METHOD(CIWrapRgIt::get_Insert, S_OK);
}


STDMETHODIMP CIWrapRgIt::get_Exist(VARIANT_BOOL* ao_pfVal)
{
	ENTER_METHOD(CIWrapRgIt::get_Exist);

	// Bug 0113644 Deprecated. No longer supported.
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CIWrapRgIt::get_Exist" << "M4ChWrap.dll" );
	LEAVE_METHOD(CIWrapRgIt::get_Exist, S_OK);
}


STDMETHODIMP CIWrapRgIt::_NewEnum(IUnknown** pVal)
{
	ENTER_METHOD(CIWrapRgIt::_NewEnum);

	*pVal = (IEnumVARIANT*) this;
	((IUnknown*)*pVal)->AddRef();
	
	LEAVE_METHOD(CIWrapRgIt::_NewEnum, S_OK);
}


//
// From IEnum
//
STDMETHODIMP CIWrapRgIt::Begin(IWrapRgIt** ao_poRegister)
{
	ENTER_METHOD(CIWrapRgIt::Begin);

	m4bool_t noteof = m_poRegister->Begin();

#ifdef NO_REG_ERROR
	if (noteof)
	{
#endif
		*ao_poRegister = this;
		((IUnknown*)*ao_poRegister)->AddRef();

#ifdef NO_REG_ERROR
	}
	else
	{
		FormatError(M4_CH_WO_NO_SUCH_ITEM, "Cannot move to register %d", ai_iIndex);
		LEAVE_METHOD(CIWrapRgIt::Begin, E_UNEXPECTED);
	}
#endif
	LEAVE_METHOD(CIWrapRgIt::Begin, S_OK);
}


STDMETHODIMP CIWrapRgIt::Next(IWrapRgIt** ao_poRegister)
{
	ENTER_METHOD(CIWrapRgIt::Next);

	m4bool_t noteof = m_poRegister->Next();

#ifdef NO_REG_ERROR
	if (noteof)
	{
#endif
		*ao_poRegister = this;
		((IUnknown*)*ao_poRegister)->AddRef();

#ifdef NO_REG_ERROR
	}
	else
	{
		FormatError(M4_CH_WO_NO_SUCH_ITEM, "Cannot move to register %d", ai_iIndex);
		LEAVE_METHOD(CIWrapRgIt::Next, E_UNEXPECTED);
	}
#endif
	LEAVE_METHOD(CIWrapRgIt::Next, S_OK);
}


STDMETHODIMP CIWrapRgIt::End(IWrapRgIt** ao_poRegister)
{
	ENTER_METHOD(CIWrapRgIt::End);

	m4bool_t noteof = m_poRegister->End();

#ifdef NO_REG_ERROR
	if (noteof)
	{
#endif
		*ao_poRegister = this;
		((IUnknown*)*ao_poRegister)->AddRef();

#ifdef NO_REG_ERROR
	}
	else
	{
		FormatError(M4_CH_WO_NO_SUCH_ITEM, "Cannot move to register %d", ai_iIndex);
		LEAVE_METHOD(CIWrapRgIt::End, E_UNEXPECTED);
	}
#endif
	LEAVE_METHOD(CIWrapRgIt::End, S_OK);
}


STDMETHODIMP CIWrapRgIt::Prev(IWrapRgIt** ao_poRegister)
{
	ENTER_METHOD(CIWrapRgIt::Prev);

	m4bool_t noteof = m_poRegister->Prev();

#ifdef NO_REG_ERROR
	if (noteof)
	{
#endif
		*ao_poRegister = this;
		((IUnknown*)*ao_poRegister)->AddRef();

#ifdef NO_REG_ERROR
	}
	else
	{
		FormatError(M4_CH_WO_NO_SUCH_ITEM, "Cannot move to register %d", ai_iIndex);
		LEAVE_METHOD(CIWrapRgIt::Prev, E_UNEXPECTED);
	}
#endif
	LEAVE_METHOD(CIWrapRgIt::Prev, S_OK);
}


STDMETHODIMP CIWrapRgIt::Next(ULONG celt, VARIANT* rgvar, ULONG* pceltFetched)
{
	ENTER_METHOD(CIWrapRgIt::Next);

	ULONG l2 = 0;

	if (pceltFetched != NULL)
	{
		*pceltFetched = 0;
	}
	
	// Set the index to be the next item
	ClRegisterIndex oIndex = m_poRegister->Index;
		
	if (m_poRegister->Next()) // esta operacion retorna IsNotEOF()
    {
		VariantInit(&rgvar[0]);

		rgvar[0].vt = VT_UNKNOWN;
		rgvar[0].punkVal = (IWrapRgIt*) this;
		((IUnknown*)rgvar[0].punkVal)->AddRef();

		// Set count of elements retrieved.
		if (pceltFetched != NULL)
		{
			*pceltFetched = 1;
		}

		l2++;

		TRACE3("Index %d Next %d", oIndex, oIndex + 1);
	}

	LEAVE_METHOD(CIWrapRgIt::Next, (l2 < celt) ? S_FALSE : S_OK);
}


STDMETHODIMP CIWrapRgIt::Skip(ULONG celt)
{
	ENTER_METHOD(CIWrapRgIt::Skip);

	m4uint32_t Count = m_poRegister->Count();

	ClRegisterIndex oIndex = m_poRegister->Index + celt;
		
	if (oIndex >= Count)
	{
		oIndex = Count - 1;
		m_poRegister->MoveTo(oIndex);

		LEAVE_METHOD(CIWrapRgIt::Skip, S_FALSE);
	}

	m_poRegister->MoveTo(oIndex);

	LEAVE_METHOD(CIWrapRgIt::Skip, S_OK);
}


STDMETHODIMP CIWrapRgIt::Reset()
{
	ENTER_METHOD(CIWrapRgIt::Reset);

	m_poRegister->MoveTo(0);

	LEAVE_METHOD(CIWrapRgIt::Reset, S_OK);
}


STDMETHODIMP CIWrapRgIt::Clone(IEnumVARIANT** ppenum)
{
	ENTER_METHOD(CIWrapRgIt::Clone);

	_ClRegister_Base* poRegister = m_poRegister->Clone();
	
	if (poRegister)
	{

#ifdef M4_NEWINTERFACE
		if (NewInterface(CLSID_WrapRgIt, IID_IEnumVARIANT, (IUnknown**)ppenum) != M4_SUCCESS)
		{
			// Won't be needing this...
			delete poRegister;

			LEAVE_METHOD(CIWrapRgIt::Clone, E_UNEXPECTED);
		}
#else
		HRESULT hRes;
		if (FAILED(hRes = M4_CREATE_INTERFACE(CWrapRgIt, ppenum))) 
		{
			// Won't be needing this...
			delete poRegister;

			LEAVE_METHOD(CIWrapRgIt::Clone, hRes);
		}
#endif
		((CIWrapRgIt*)(*ppenum))->Init(poRegister);
		((CIWrapRgIt*)(*ppenum))->SetIsClone(M4_TRUE);

		LEAVE_METHOD(CIWrapRgIt::Clone, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANNOT_CLONE_REGISTER, "Cannot create a clone of the register");
		LEAVE_METHOD(CIWrapRgIt::Clone, E_UNEXPECTED);
	}
}


STDMETHODIMP CIWrapRgIt::get_Copy(IWrapRgIt** pVal)
{
	ENTER_METHOD(CIWrapRgIt::get_Copy);

	_ClRegister_Base* poRegister = m_poRegister->Clone();
	
	if (poRegister)
	{
		if (NewInterface(CLSID_WrapRgIt, IID_IWrapRgIt, (IUnknown**) pVal) != M4_SUCCESS)
		{
			// Won't be needing this...
			LEAVE_METHOD(CIWrapRgIt::get_Copy, E_UNEXPECTED);
		}
		
		((CIWrapRgIt*)*pVal)->Init(poRegister);
		((CIWrapRgIt*)*pVal)->SetIsClone(M4_TRUE);

		LEAVE_METHOD(CIWrapRgIt::get_Copy, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANNOT_COPY_REGISTER, "Cannot create a copy of the register");

		LEAVE_METHOD(CIWrapRgIt::get_Copy, E_UNEXPECTED);
	}
}


STDMETHODIMP CIWrapRgIt::get_Copy_Total(IWrapRgIt** pVal)
{
	ENTER_METHOD(CIWrapRgIt::get_Copy_Total);

	_ClRegister_Base* poRegister = m_poRegister->Clone(M4_TRUE);
	
	if (poRegister)
	{
		if (NewInterface(CLSID_WrapRgIt, IID_IWrapRgIt,	(IUnknown**) pVal) != M4_SUCCESS)
		{
			// Won't be needing this...
			LEAVE_METHOD(CIWrapRgIt::get_Copy_Total, E_UNEXPECTED);
		}
		
		((CIWrapRgIt*)*pVal)->Init(poRegister);
		((CIWrapRgIt*)*pVal)->SetIsClone(M4_TRUE);

		LEAVE_METHOD(CIWrapRgIt::get_Copy_Total, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANNOT_COPY_REGISTER, "Cannot create a copy of the register");
		LEAVE_METHOD(CIWrapRgIt::get_Copy_Total, E_UNEXPECTED);
	}
}


STDMETHODIMP CIWrapRgIt::DeleteAll(short* ao_RetVal)
{
	ENTER_METHOD(CIWrapRgIt::DeleteAll);

	*ao_RetVal = m_poRegister->DeleteAll();

	TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CIWrapRgIt::DeleteAll, S_OK);
}


STDMETHODIMP CIWrapRgIt::DestroyAll(short * ao_RetVal)
{
	ENTER_METHOD(CIWrapRgIt::DestroyAll);

	*ao_RetVal = m_poRegister->DestroyAll();

	TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CIWrapRgIt::DestroyAll, S_OK);
}


STDMETHODIMP CIWrapRgIt::Destroy(short * ao_RetVal)
{
	ENTER_METHOD(CIWrapRgIt::Destroy);

	*ao_RetVal = m_poRegister->Destroy();

	TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CIWrapRgIt::Destroy, S_OK);
}


STDMETHODIMP CIWrapRgIt::BSearch(SAFEARRAY ** ai_saArguments,  VARIANT_BOOL* ao_pbFound, VARIANT_BOOL* ao_pbVal)
{
	HRESULT hresult;
	VARIANT VVar;
	//ClTmpVar ** ppTempVar;
	m4VariantType * pvArguments;
	m4bool_t bOK = M4_TRUE;
	
	ENTER_METHOD(CIWrapRgIt::BSearch);
	
	if ((*ai_saArguments)->cDims != 1)
	{
		//Poner error
		bOK = M4_FALSE;
	}
	else
	{
		m4uint32_t i,j;
		m4uint32_t iSize = (*ai_saArguments)->rgsabound[0].cElements;
		m4uint32_t iLBound = (*ai_saArguments)->rgsabound[0].lLbound;
		m4uint32_t iUBound = iLBound + iSize;

		//pequeña chapuza con los variants en tanto se unifican en una sola clase.
		//little  chapuzation with the variants.
		pvArguments = new m4VariantType [iSize];
		//ppTempVar = new ClTmpVar* [iSize];

		for (j = 0, i = iLBound; i < iUBound; i++, j++)
		{
			hresult = SafeArrayGetElement(*ai_saArguments,(m4int32_t *) &i, &VVar);

			if (FAILED(hresult))
			{
				//Poner error
				bOK = M4_FALSE;
				break;
			}

			//Convert de VARIANT
/*			ppTempVar[j] = new ClTmpVar(VVar);

			pvArguments[j].Type = ppTempVar[j]->Type;
			pvArguments[j].Data = ppTempVar[j]->Data;*/
			pvArguments[j] = VVar;

		}

		if (bOK) 
		{
			bOK = m_poRegister->Find (pvArguments);
		}

		//delete de intermediate arrays
		delete 	[] pvArguments; 
/*
		for (--j; j >= 0; j--)
		{
			delete ppTempVar[j];
		}

		delete [] ppTempVar;		*/

	}

	if (bOK)
	{
		*ao_pbVal = VARIANT_TRUE;
		*ao_pbFound = VARIANT_TRUE;
	}
	else
	{
		*ao_pbVal = VARIANT_FALSE;
		*ao_pbFound = VARIANT_FALSE;
	}

	LEAVE_METHOD(CIWrapRgIt::BSearch, S_OK);
}


STDMETHODIMP CIWrapRgIt::MoveTo(/*[in]*/int ai_iIndex, /*[out, retval]*/ short* ao_piReturn)
{
    ENTER_METHOD( CIWrapRgIt::MoveTo ) ;

	*ao_piReturn = M4_ERROR ;

    m_poRegister->MoveTo( (ClRegisterIndex)ai_iIndex ) ;

	*ao_piReturn = M4_SUCCESS ;

    LEAVE_METHOD( CIWrapRgIt::MoveTo, S_OK ) ;
}


STDMETHODIMP CIWrapRgIt::MoveToEOF(/*[out, retval]*/ short* ao_piReturn)
{
    ENTER_METHOD( CIWrapRgIt::MoveToEOF ) ;

	*ao_piReturn = M4_ERROR ;

    m_poRegister->MoveToEOF () ;

	*ao_piReturn = M4_SUCCESS ;

    LEAVE_METHOD( CIWrapRgIt::MoveToEOF, S_OK ) ;
}


STDMETHODIMP CIWrapRgIt::MoveToFirst(/*[out, retval]*/ short* ao_piReturn)
{
    ENTER_METHOD( CIWrapRgIt::MoveToFirst ) ;

    m4uint32_t			iFirst = 0 ;
	m4uint32_t			iNumRows = 0 ;
	ClPartialLoadData	*poPartial = NULL ;


	*ao_piReturn = M4_ERROR ;

	if( m_poRegister->GetCurrentBlock()->IsNihil() == M4_FALSE )
	{
		iNumRows = m_poRegister->GetpNode_Data()->GetpNodeDef()->NumRows() ;

		if( iNumRows != 0 )
		{
			// Hay carga parcial
			poPartial = m_poRegister->GetCurrentBlock()->GetpPL_Data();

			if( poPartial != NULL )
			{
				iFirst = poPartial->GetFirstVisible() ;
			}
		}
	}

    m_poRegister->MoveTo( (ClRegisterIndex)iFirst ) ;

	*ao_piReturn = M4_SUCCESS ;

    LEAVE_METHOD( CIWrapRgIt::MoveToFirst, S_OK ) ;
}


///////////////////////////////////////////////////////////////////////////////
//
// FILTER INTERFACE
//

STDMETHODIMP CIWrapRgIt::get_Filter(IWrapRgItFilter** pVal)
{
	ENTER_METHOD(CIWrapRgIt::get_Filter);

	// Just cast this and return a pointer to
	// the RgItFilter
	// Add one to the reference counts...
	// of the base class...
	*pVal = (IWrapRgItFilter*) this;

	TRACE("Casting RgIt to RgItFilter");

	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CIWrapRgIt::get_Filter, S_OK);
}


//---------------------------
//find 
//---------------------------
STDMETHODIMP CIWrapRgIt::Find(SAFEARRAY ** ai_saArguments,   long* ao_pbVal)
{
	HRESULT hresult;
	VARIANT VVar;
	m4VariantType * pvArguments;
	m4bool_t bOK=M4_TRUE;

	m4return_t ret=M4_SUCCESS;

	ENTER_METHOD(CIWrapRgIt::Find);
	
	if ((*ai_saArguments)->cDims != 1)
	{
		//Poner error
		*ao_pbVal=-1;
	}
	else
	{
		m4uint32_t i,j;
		m4uint32_t iSize = (*ai_saArguments)->rgsabound[0].cElements;
		m4uint32_t iLBound = (*ai_saArguments)->rgsabound[0].lLbound;
		m4uint32_t iUBound = iLBound + iSize;
		
		M4_ASSERT (iSize <65536);

		pvArguments = new m4VariantType [iSize];

		for (j = 0, i = iLBound; i < iUBound; i++, j++)
		{
			hresult = SafeArrayGetElement(*ai_saArguments,(m4int32_t *) &i, &VVar);

			if (FAILED(hresult))
			{
				//Poner error
				bOK = M4_FALSE;
				break;
			}

			pvArguments[j] = VVar;
		}

		if (bOK)
		{
			ret = m_poRegister->FindSec (pvArguments , (m4uint16_t)iSize, ao_pbVal);

			if (ret!=M4_SUCCESS)
			{
				*ao_pbVal=-1;
			}
		}
		//delete de intermediate arrays
		delete 	[] pvArguments; 
	}
	
	LEAVE_METHOD(CIWrapRgIt::BSearch, S_OK);
}


STDMETHODIMP CIWrapRgIt::ReviewNewRegisters(/*[out, retval]*/ long * ao_iIndex)
{
	ENTER_METHOD(CIWrapRgIt::ReviewNewRegisters);

    m4return_t res = m_poRegister->ReviewNewRegisters(* (m4uint32_t *) ao_iIndex);

	TRACE2("Returning %d", * ao_iIndex);

	LEAVE_METHOD(CIWrapRgIt::ReviewNewRegisters, S_OK);
}


STDMETHODIMP CIWrapRgIt::CopyRecord(/*[out]*/ BSTR * ai_pFileName,
                                   /*[out, retval]*/ short* ao_RetVal)
{
	ENTER_METHOD(CIWrapRgIt::CopyRecord);

    ClFileName oFileName;

    *ao_RetVal = m_poRegister->CopyRecordWithSerialize(oFileName);

    if (oFileName.IsNull())
    {
	    gstring tmpFileName("");
	    *ai_pFileName = tmpFileName;
    }
    else
    {
	    gstring tmpFileName(oFileName);
	    *ai_pFileName = tmpFileName;
    }

	LEAVE_METHOD(CIWrapRgIt::CopyRecord, S_OK);
}


STDMETHODIMP CIWrapRgIt::PasteRecord(VARIANT_BOOL* ai_pbNewRecord,
                                     VARIANT_BOOL* ai_pbKeepMarks,
                                     BSTR *        ai_pFileName,
                                     VARIANT_BOOL* ai_pbDeleteFile,
                                     VARIANT_BOOL* ai_pbDestroyRecords,
                             /*[in]*/VARIANT       ai_pExclusionNodeName,
                                     VARIANT_BOOL* ai_pbKeepAccesses,
                   /*[out, retval]*/ short *       ao_RetVal)
{
	ENTER_METHOD(CIWrapRgIt::PasteRecord);

    ClFileName oFileName;
    gstring tmpGstring(* ai_pFileName);
    oFileName.SetFileName ((m4pchar_t)tmpGstring);

    SAFEARRAY *NodeArray = 0;

    //comprobamos que el variant sea un array:
    switch (ai_pExclusionNodeName.vt)
	{
        case VT_BYREF|VT_ARRAY|VT_BSTR:
            NodeArray = *ai_pExclusionNodeName.pparray;
            break;

        case VT_ARRAY|VT_BSTR:
            NodeArray = ai_pExclusionNodeName.parray;
            break;
        default:
            TRACE("No array with arguments names detected");
    }

    const m4char_t * *pnodes;
    gstring *pxxx;
    m4uint32_t iSize;

    *ao_RetVal = M4_SUCCESS;

    //no pasa nada, se mantiene el orden de evaluación:
    if (NodeArray != 0)
	{
        BSTR VVar;
		m4uint32_t i,j;
        iSize = NodeArray->rgsabound[0].cElements;
		m4uint32_t iLBound = NodeArray->rgsabound[0].lLbound;
		m4uint32_t iUBound = iLBound + iSize;        

        pxxx = new gstring [iSize];
        pnodes = new m4pcchar_t [iSize];            

		for (j = 0, i = iLBound; i < iUBound; i++, j++)
		{
			HRESULT hresult = SafeArrayGetElement(NodeArray, (m4int32_t *) &i, &VVar);

			if (FAILED(hresult))
			{
				//Poner error
				*ao_RetVal = M4_ERROR;
				break;
			}

            pxxx[j] = VVar;
            pnodes[j] = pxxx[j];
		}
    }
    else
	{
        pnodes = 0;
        pxxx = 0;
        iSize = 0;
    }

    m4bool_t bNewRecord      = (*(m4bool_t *)ai_pbNewRecord)      ? M4_TRUE : M4_FALSE;
    m4bool_t bKeepMarks      = (*(m4bool_t *)ai_pbKeepMarks)      ? M4_TRUE : M4_FALSE;
    m4bool_t bDeleteFile     = (*(m4bool_t *)ai_pbDeleteFile)     ? M4_TRUE : M4_FALSE;
    m4bool_t bDestroyRecords = (*(m4bool_t *)ai_pbDestroyRecords) ? M4_TRUE : M4_FALSE;
    m4bool_t bKeepAccesses   = (*(m4bool_t *)ai_pbKeepAccesses)   ? M4_TRUE : M4_FALSE;

    *ao_RetVal = m_poRegister->PasteRecordWithDeSerialize(bNewRecord, bKeepMarks, oFileName, bDeleteFile, bDestroyRecords, pnodes, (m4uint16_t)iSize, bKeepAccesses);

	LEAVE_METHOD(CIWrapRgIt::PasteRecord, S_OK);
}


STDMETHODIMP CIWrapRgIt::get_IsLoaded(VARIANT_BOOL* ai_pbIsLoaded)
{
	ENTER_METHOD(CIWrapRgIt::get_IsLoaded);

	m4bool_t bisloaded = m_poRegister->BlockIsLoaded();

    if (bisloaded)
	{
        *ai_pbIsLoaded = VARIANT_TRUE;
	}
    else
	{
		*ai_pbIsLoaded = VARIANT_FALSE;
	}

	LEAVE_METHOD(CIWrapRgIt::get_IsLoaded, S_OK);
}


STDMETHODIMP CIWrapRgIt::get_NewNotPersist(/*[out]*/ VARIANT_BOOL* pbNewNotPersist)
{
	ENTER_METHOD(CIWrapRgIt::get_NewNotPersist);

	m4bool_t bNewNotPersist = m_poRegister->GetNewNotPersist();

    if (bNewNotPersist)
	{
        *pbNewNotPersist = VARIANT_TRUE;
	}
    else
	{
		*pbNewNotPersist = VARIANT_FALSE;
	}

	LEAVE_METHOD(CIWrapRgIt::get_NewNotPersist, S_OK);
}


STDMETHODIMP CIWrapRgIt::put_NewNotPersist(/*[in]*/ VARIANT_BOOL * pbNewNotPersist)
{
	ENTER_METHOD(CIWrapRgIt::put_NewNotPersist);

    m4bool_t bNNP = (* (m4bool_t *)pbNewNotPersist) ? M4_TRUE : M4_FALSE;
    m_poRegister->SetNewNotPersist(bNNP);

	LEAVE_METHOD(CIWrapRgIt::put_NewNotPersist, S_OK);
}


STDMETHODIMP CIWrapRgIt::get_RecordID(/*[out, retval]*/ long* ao_plRecordId)
{
	ENTER_METHOD(CIWrapRgIt::get_RecordID);

	m4return_t	iResult = M4_SUCCESS ;
	m4uint32_t	iUniqueId = 0 ;

	*ao_plRecordId = -1 ;

	iResult = m_poRegister->GetRecordID( iUniqueId ) ;

	if( iResult == M4_SUCCESS )
	{
		*ao_plRecordId = iUniqueId ;
	}

	LEAVE_METHOD(CIWrapRgIt::get_RecordID, S_OK);
}


STDMETHODIMP CIWrapRgIt::IndexOf(/*[in]*/ long ai_lRecordId, /*[out, retval]*/ long* ao_piIndex)
{
	ENTER_METHOD(CIWrapRgIt::IndexOf);

	m4return_t			iResult = M4_SUCCESS ;
	ClRegisterIndex		iIndex = 0 ;

	*ao_piIndex = -1 ;

	iResult = m_poRegister->GetIndexOf( ai_lRecordId, iIndex ) ;

	if( iResult == M4_SUCCESS )
	{
		*ao_piIndex = iIndex ;
	}

	LEAVE_METHOD(CIWrapRgIt::IndexOf, S_OK);
}


/*
Lee los registros de un bloque y los devuelve en un safe array
Ademas devuelve el identificador del registro y su estado
Los elementos que no son de registro solo se escriben una vez en el primer registro
El acceso es primero por columna y luego por fila
*/
STDMETHODIMP CIWrapRgIt::ReadRegisters(/*[out]*/ SAFEARRAY** ao_avValues, /*[out]*/ long* ao_iFirst, /*[out, retval]*/ short* ao_piReturn)
{
	ENTER_METHOD(CIWrapRgIt::ReadRegisters);

	HRESULT				hResult = S_OK ;
	m4return_t			iResult = M4_SUCCESS ;
	m4uint8_t			iLastAutoload = M4CL_AUTOLOAD_OFF ;
	m4uint32_t			iUniqueId = 0 ;
	m4uint32_t			i = 0 ;
	m4uint32_t			j = 0 ;
	m4uint32_t			iColumn = 0 ;
	m4uint32_t			iNumRows = 0 ;
    m4uint32_t			iFirst = 0 ;
	m4uint32_t			iRows = 0 ;
	m4uint32_t			iItems = 0 ;
	m4uint32_t			iColumns = 0 ;
	ClAccess			*poAccess = NULL ;
	ClPartialLoadData	*poPartial = NULL ;
	SAFEARRAYBOUND		rgsabound[ 2 ] ;
	VARIANT				*pvValues = NULL ;


	*ao_piReturn = M4_ERROR ;
	*ao_iFirst = 0 ;

	if( *ao_avValues != NULL )
	{
		SafeArrayDestroy( *ao_avValues ) ;
		*ao_avValues = NULL ;
	}

	if( m_poRegister->GetCurrentBlock()->IsNihil() == M4_FALSE )
	{
		// Se desactiva el modo autoload para hacer la consulta
		poAccess = m_poRegister->GetpAccess() ;
		iLastAutoload = poAccess->GetAutoLoadMode() ;

		if( iLastAutoload != M4CL_AUTOLOAD_OFF )
		{
			poAccess->SetAutoLoadMode( M4CL_AUTOLOAD_OFF ) ;
		}

		iNumRows = m_poRegister->GetpNode_Data()->GetpNodeDef()->NumRows() ;

		if( iNumRows != 0 )
		{
			// Hay carga parcial
			poPartial = m_poRegister->GetCurrentBlock()->GetpPL_Data();

			if( poPartial != NULL )
			{
				iFirst = poPartial->GetFirstVisible() ;
				iRows = poPartial->GetLastVisible() - iFirst ;
			}
			else
			{
				iFirst = 0 ;
				iRows = 0 ;
			}
		}
		else
		{
			iFirst = 0 ;
			iRows = m_poRegister->Count() ;
		}

		*ao_iFirst = iFirst ;

		if( iLastAutoload != M4CL_AUTOLOAD_OFF )
		{
			poAccess->SetAutoLoadMode( iLastAutoload ) ;
		}
	}
	else
	{
		iFirst = 0 ;
		iRows = 0 ;
	}

	iItems = m_poRegister->Item.Count() ;

	// Se calcula el número de columnas con valor
	iColumns = 0 ;

	for( j = 0 ; j < iItems ; j++ )
	{
		m_poRegister->Item.MoveTo( j ) ;

		if( m_poRegister->Item.ItemDef.Type() != M4CL_ITEM_TYPE_METHOD )
		{
			iColumns++ ;
		}
	}

	// Se crea el array de dos dimensiones, filas por columnas (más 2 para el identificador y la operación)
	rgsabound[ 0 ].lLbound = 0 ;
	rgsabound[ 0 ].cElements = iRows ;
	rgsabound[ 1 ].lLbound = 0 ;
	rgsabound[ 1 ].cElements = iColumns + 2 ;

	*ao_avValues = SafeArrayCreate( VT_VARIANT, 2, rgsabound ) ;

	if( *ao_avValues == NULL )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot create SafeArray" ) ;
		LEAVE_METHOD( CWrapCVM::ReadRegisters, S_OK ) ;
	}

	hResult = SafeArrayAccessData( *ao_avValues, (void**) &pvValues ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot access SafeArray" ) ;
		SafeArrayDestroy( *ao_avValues ) ;
		*ao_avValues = NULL ;
		LEAVE_METHOD( CWrapCVM::ReadRegisters, S_OK ) ;
	}

    for( i = 0 ; i < iRows ; i++ )
    {
		m_poRegister->MoveTo( ClRegisterIndex( iFirst + i ) ) ;

		// Se escribe el identificador del registro
		iResult = m_poRegister->GetRecordID( iUniqueId ) ;

		if( iResult != M4_SUCCESS )
		{
			FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot GetRecordID" ) ;
			SafeArrayDestroy( *ao_avValues ) ;
			*ao_avValues = NULL ;
			LEAVE_METHOD( CWrapCVM::ReadRegisters, S_OK ) ;
		}

		pvValues[ i ].vt = VT_R8 ;
		pvValues[ i ].dblVal = iUniqueId ;

		// Se escribe la operación del registro
		pvValues[ iRows + i ].vt = VT_R8 ;

		if( m_poRegister->Operation.IsUpdated() == M4_TRUE )
		{
			pvValues[ iRows + i ].dblVal = 1 ;
		}
		else if( m_poRegister->Operation.IsNew() == M4_TRUE )
		{
			pvValues[ iRows + i ].dblVal = 2 ;
		}
		else if( m_poRegister->Operation.IsDeleted() == M4_TRUE )
		{
			pvValues[ iRows + i ].dblVal = -1 ;
		}
		else
		{
			pvValues[ iRows + i ].dblVal = 0 ;
		}

		if(	iColumns > 0 )
		{
			iColumn = 0 ;

			for( j = 0 ; j < iItems ; j++ )
			{
				m_poRegister->Item.MoveTo( j ) ;

				if( m_poRegister->Item.ItemDef.Type() != M4CL_ITEM_TYPE_METHOD )
				{
					// Se escribe el valor de cada celda. Los que no son de registro sólo se escriben en el primer registro
					if( i == 0 || m_poRegister->Item.ItemDef.Scope() == M4CL_ITEM_SCOPE_REGISTER )
					{
						m_poItemIterator->get_Value( pvValues + ( iColumn + 2 ) * iRows + i ) ;
					}
					iColumn++ ;
				}
			}
		}
	}

	hResult = SafeArrayUnaccessData( *ao_avValues ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot unaccess SafeArray" ) ;
		SafeArrayDestroy( *ao_avValues ) ;
		*ao_avValues = NULL ;
		LEAVE_METHOD( CWrapCVM::ReadRegisters, S_OK ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD(CIWrapRgIt::ReadRegisters, S_OK);
}


