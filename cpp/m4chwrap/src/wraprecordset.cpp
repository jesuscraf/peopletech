//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wraprecordset.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapRecordSet
//
//==============================================================================

#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "wraprecordset.h"
#include "wrapoper.h"
#include "wraprecordset.h"
#include "wrapitit.h"
#include "wraprgit.h"
#include "newinterface.h"
#include "variantref.h"
#include "wrapdef.h"
#include "gstring.h"
#include "node.hpp"
#include "nodedef.hpp"
#include "rerror.h"


/////////////////////////////////////////////////////////////////////////////
// CWrapRecordSet
CWrapRecordSet::CWrapRecordSet()
	: m_poRecordSet(0)
	, m_poRegisterIterator(0)
	, m_poItemIterator(0)
	, m_poCurrentIterator(0)
{
}


CWrapRecordSet::~CWrapRecordSet()
{
	m_poRecordSet = 0;

	if (m_poRegisterIterator)
	{
		m_poRegisterIterator->Release();
		m_poRegisterIterator = 0;
	}
	if (m_poCurrentIterator)
	{
		m_poCurrentIterator->Release();
		m_poCurrentIterator = 0;
	}
	if (m_poItemIterator)
	{
		m_poItemIterator->Release();
		m_poItemIterator = 0;
	}
}


void CWrapRecordSet::Init(ClAccessRecordSet* ai_VarRef)
{
	m_poRecordSet = ai_VarRef;

	TRACE2("Wrapped record set has address %p",	m_poRecordSet);
}


// Blow this away...
STDMETHODIMP CWrapRecordSet::GetRegister(int ai_iIndex, IWrapRegister** ao_poRegister)
{
	ENTER_METHOD(CWrapRecordSet::GetRegister);

	m_poRecordSet->Register[ai_iIndex];

	if (NewInterface(CLSID_WrapRegister, IID_IWrapRegister,	(IUnknown**)ao_poRegister) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapRecordSet::GetRegister, E_UNEXPECTED);
	}

	ClVariantRef oVarRef((void*)(&m_poRecordSet->Register));
	(*ao_poRegister)->Init(oVarRef);

	LEAVE_METHOD(CWrapRecordSet::GetRegister, S_OK);
}


STDMETHODIMP CWrapRecordSet::ExistRegister(VARIANT_BOOL* ao_pfVal)
{
    DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapRecordSet::ExistRegister" << "M4ChWrap.dll" );
	LEAVE_METHOD(CWrapRecordSet::ExistRegister, E_UNEXPECTED);
}


STDMETHODIMP CWrapRecordSet::FindRegister(IWrapRegister** ao_poRegister)
{
    DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapRecordSet::FindRegister" << "M4ChWrap.dll" );
	LEAVE_METHOD(CWrapRecordSet::FindRegister, E_UNEXPECTED);
}


STDMETHODIMP CWrapRecordSet::GetRecordCount(int* ao_iCount)
{
	ENTER_METHOD(CWrapRecordSet::GetRecordCount);

	*ao_iCount = m_poRecordSet->Count();

	TRACE2("Returning a count of %d", *ao_iCount);
	LEAVE_METHOD(CWrapRecordSet::GetRecordCount, S_OK);
}


STDMETHODIMP CWrapRecordSet::NewRegister(IWrapRegister** ao_poRegister)
{
    DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapRecordSet::NewRegister" << "M4ChWrap.dll" );
	LEAVE_METHOD(CWrapRecordSet::NewRegister, E_UNEXPECTED);
}


STDMETHODIMP CWrapRecordSet::InsertRegister(IWrapRegister* ai_iRegister)
{
    DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapRecordSet::InsertRegister" << "M4ChWrap.dll" );
	LEAVE_METHOD(CWrapRecordSet::InsertRegister, E_UNEXPECTED);
}


// Finish blowing away...

STDMETHODIMP CWrapRecordSet::GetCurrentRegister(IWrapRegister** ao_poRegister)
{
	ENTER_METHOD(CWrapRecordSet::GetCurrentRegister);

	if (NewInterface(CLSID_WrapRegister, IID_IWrapRegister,	(IUnknown**)ao_poRegister) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapRecordSet::GetCurrentRegister, E_UNEXPECTED);
	}

	ClVariantRef oVarRef((void*)&m_poRecordSet->Current);
	(*ao_poRegister)->Init(oVarRef);

	LEAVE_METHOD(CWrapRecordSet::GetCurrentRegister, S_OK);
}


STDMETHODIMP CWrapRecordSet::SetCurrentRegister(IWrapRegister* ai_poRegister, short* ao_sRetVal)
{
	VARIANT var;

	ENTER_METHOD(CWrapRecordSet::SetCurrentRegister);

	ai_poRegister->Unwrap(&var);
	ClVariantRef oVarRef(var);
	ClRegister* pRegister = (ClRegister*)(void*)oVarRef;

	TRACE2("Unwrapped Register has address %p",	pRegister);

	m_poRecordSet->Current[pRegister->Index];
	*ao_sRetVal = M4_SUCCESS;

	LEAVE_METHOD(CWrapRecordSet::SetCurrentRegister, S_OK);
}


STDMETHODIMP CWrapRecordSet::MoveCurrent(BSTR ai_stBase, long ai_iOffset, VARIANT_BOOL* ao_pb)
{
	ENTER_METHOD(CWrapRecordSet::MoveCurrent);

	gstring tmpGstring(ai_stBase);

	if (tmpGstring)
	{
		TRACE2("Base is %s", m4pchar_t(tmpGstring));
		
		if (strcmp(_strupr(tmpGstring),	"BEGIN") == 0)
		{
			m_poRecordSet->Current[ai_iOffset];

			if (m_poRecordSet->Current.Index == M4DM_EOF_INDEX)
			{
				*ao_pb = VARIANT_TRUE;
			}
			else
			{
				*ao_pb = VARIANT_FALSE;
			}
		}
		else if (strcmp(_strupr(tmpGstring), "CURRENT") == 0)
		{
			m_poRecordSet->Current[m_poRecordSet->Current.Index + ai_iOffset];

			if (m_poRecordSet->Current.Index == M4DM_EOF_INDEX)
			{
				*ao_pb = VARIANT_TRUE;
			}
			else
			{
				*ao_pb = VARIANT_FALSE;
			}
		}
		else if (strcmp(_strupr(tmpGstring), "END") == 0)
		{
			m_poRecordSet->Current[m_poRecordSet->Count() - ai_iOffset - 1];

			if (m_poRecordSet->Current.Index == M4DM_EOF_INDEX)
			{
				*ao_pb = VARIANT_TRUE;
			}
			else
			{
				*ao_pb = VARIANT_FALSE;
			}
		}
		else // No such base, so return IsEof == TRUE
		{
			*ao_pb = VARIANT_TRUE;
		}

		LEAVE_METHOD(CWrapRecordSet::MoveCurrent, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapRecordSet::MoveCurrent, E_UNEXPECTED);
	}
}


STDMETHODIMP CWrapRecordSet::get_coItems( IWrapColItems** pVal)
{
	ENTER_METHOD(CWrapRecordSet::get_coItems);

	if(NewInterface(CLSID_WrapColItems,	IID_IWrapColItems, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapRecordSet::get_coItems, E_UNEXPECTED);
	}
		
	ClVariantRef oVarRef(&(m_poRecordSet->Item));
	(*pVal)->Init(oVarRef);

	LEAVE_METHOD(CWrapRecordSet::get_coItems, S_OK);
}


STDMETHODIMP CWrapRecordSet::get_Operation(IWrapOper** pVal)
{
	ENTER_METHOD(CWrapRecordSet::get_Operation);

	if (NewInterface(CLSID_WrapOper, IID_IWrapOper, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapRecordSet::get_Operation, E_UNEXPECTED);
	}

	((CWrapOper*)(*pVal))->Init(&m_poRecordSet->Operation);

	LEAVE_METHOD(CWrapRecordSet::get_Operation, S_OK);
}


STDMETHODIMP CWrapRecordSet::CheckPoint(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapRecordSet::CheckPoint);

	*ao_sRetVal = m_poRecordSet->CheckPoint();

	TRACE2("Returning %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapRecordSet::CheckPoint, S_OK);
}


STDMETHODIMP CWrapRecordSet::Undo(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapRecordSet::Undo);

	*ao_sRetVal = m_poRecordSet->Undo();

	TRACE2("Returning %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapRecordSet::Undo, S_OK);
}


STDMETHODIMP CWrapRecordSet::Load(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapRecordSet::Load);

	*ao_sRetVal = m_poRecordSet->Load_Block ();

	TRACE2("Returning %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapRecordSet::Load, S_OK);
}


STDMETHODIMP CWrapRecordSet::Persist(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapRecordSet::Persist);

	*ao_sRetVal = m_poRecordSet->Persist_Tree ();

	TRACE2("Returning %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapRecordSet::Persist, S_OK);
}


STDMETHODIMP CWrapRecordSet::get_RegisterIterator(IWrapRgIt** pVal)
{
	ENTER_METHOD(CWrapRecordSet::get_RegisterIterator);

	if (!m_poRegisterIterator)
	{
		TRACE("Register iterator being allocated");

		if (NewInterface(CLSID_WrapRgIt, IID_IWrapRgIt, (IUnknown**) &m_poRegisterIterator) != M4_SUCCESS)
		{		
			LEAVE_METHOD(CWrapRecordSet::get_RegisterIterator, E_UNEXPECTED);
		}

		((CWrapRgIt*)m_poRegisterIterator)->Init(&m_poRecordSet->Register);
	}

	TRACE2("RegisterIterator has address %p", m_poRegisterIterator);

	*pVal = m_poRegisterIterator;
	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapRecordSet::get_RegisterIterator, S_OK);
}


STDMETHODIMP CWrapRecordSet::get_CurrentIterator(IWrapRgIt** pVal)
{
	ENTER_METHOD(CWrapRecordSet::get_CurrentIterator);

	if (!m_poCurrentIterator)
	{
		TRACE("Current iterator being allocated");

		if (NewInterface(CLSID_WrapRgIt, IID_IWrapRgIt, (IUnknown**) &m_poCurrentIterator) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapRecordSet::get_CurrentIterator, E_UNEXPECTED);
		}

		((CWrapRgIt*)m_poCurrentIterator)->Init(&m_poRecordSet->Current);
	}

	TRACE2("CurrentIterator has address %p", m_poCurrentIterator);

	*pVal = m_poCurrentIterator;
	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapRecordSet::get_CurrentIterator, S_OK);
}


STDMETHODIMP CWrapRecordSet::get_ItemIterator(IWrapItIt** pVal)
{
	ENTER_METHOD(CWrapRecordSet::get_ItemIterator);

	if (!m_poItemIterator)
	{
		TRACE("Allocating new item iterator");

#ifdef M4_NEWINTERFACE
		if (NewInterface(CLSID_WrapItIt, IID_IWrapItIt,	(IUnknown**) &m_poItemIterator) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapRecordSet::get_ItemIterator, E_UNEXPECTED);
		}
#else
		HRESULT hRes;
		if (FAILED(hRes = M4_CREATE_INTERFACE(CWrapItIt, &m_poItemIterator))) 
		{
			LEAVE_METHOD(CWrapRecordSet::get_ItemIterator, hRes);
		}
#endif
		((CWrapItIt*)m_poItemIterator)->Init(&m_poRecordSet->Item);
	}

	TRACE2("The item iterator has address %p", m_poItemIterator);

	*pVal = m_poItemIterator;
	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapRecordSet::get_ItemIterator, S_OK);
}


STDMETHODIMP CWrapRecordSet::Destroy(short* ao_RetVal)
{
	ENTER_METHOD(CWrapRecordSet::Destroy);

	*ao_RetVal = m_poRecordSet->Destroy();

	TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CWrapRecordSet::Destroy, S_OK);
}


STDMETHODIMP CWrapRecordSet::SetCurrentSearchIndexByPos(short ai_iPos, short * ao_RetVal)
{
	ENTER_METHOD(CWrapRecordSet::SetCurrentSearchIndexByPos);
	
	*ao_RetVal = m_poRecordSet->SetCurrentSearchIndexByPos (ai_iPos);

	TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CWrapRecordSet::SetCurrentSearchIndexByPos, S_OK);
}


STDMETHODIMP CWrapRecordSet::SetCurrentSearchIndexById(long ai_IndexId, short * ao_RetVal)
{
	ENTER_METHOD(CWrapRecordSet::SetCurrentSearchIndexById);
	
	*ao_RetVal = m_poRecordSet->SetCurrentSearchIndexById (ai_IndexId);
		
	TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CWrapRecordSet::SetCurrentSearchIndexById, S_OK);
}


STDMETHODIMP CWrapRecordSet::Arrange(short * ao_RetVal)
{
	ENTER_METHOD(CWrapRecordSet::Arrange);
	
	*ao_RetVal = m_poRecordSet->Arrange();

	TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CWrapRecordSet::Arrange, S_OK);
}


STDMETHODIMP CWrapRecordSet::get_Copy(IWrapRecordSet** pVal)
{
	ENTER_METHOD(CWrapRecordSet::get_Copy);

	ClAccessRecordSet * poRecordSet = new ClAccessRecordSet(m_poRecordSet->GetpAccess());
	poRecordSet->Attach(m_poRecordSet->GetpNode_Data());
    poRecordSet->FollowSimple(* m_poRecordSet);
	
	if (NewInterface(CLSID_WrapRecordSet, IID_IWrapRecordSet, (IUnknown**) pVal) != M4_SUCCESS)
	{
		// Won't be needing this...
		LEAVE_METHOD(CWrapRecordSet::get_Copy, E_UNEXPECTED);
	}
		
	((CWrapRecordSet*)*pVal)->Init(poRecordSet);

	LEAVE_METHOD(CWrapRecordSet::get_Copy, S_OK);
}


STDMETHODIMP CWrapRecordSet::Follow(IWrapRecordSet** pVal)
{
	ENTER_METHOD(CWrapRecordSet::Follow);

	ClAccessRecordSet * poRecordSet;
    ((CWrapRecordSet*)*pVal)->Unwrap(&poRecordSet);
    m_poRecordSet->FollowSimple(* poRecordSet);
	
	LEAVE_METHOD(CWrapRecordSet::Follow, S_OK);
}


void CWrapRecordSet::Unwrap(ClAccessRecordSet** ao_VarRef)
{
	*ao_VarRef = m_poRecordSet;
}


STDMETHODIMP CWrapRecordSet::SendBlockMark(VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CWrapRecordSet::SendBlockMark);

    m4bool_t bBranchMode = (*(m4bool_t *)pVal) ? M4_TRUE : M4_FALSE;
    m_poRecordSet->SendBlockMark(bBranchMode);

	LEAVE_METHOD(CWrapRecordSet::SendBlockMark, S_OK);
}


/*
Lee los items de bloque y los devuelve en un safe array
*/
STDMETHODIMP CWrapRecordSet::ReadBlockItems(/*[out]*/ SAFEARRAY** ao_avValues, /*[out]*/ short* ao_piLastLoadStatus, /*[out]*/ long* ao_piLastLoadRows, /*[out]*/ long* ao_piLastLoadFetchs, /*[out, retval]*/ short* ao_piReturn)
{
	ENTER_METHOD(CWrapRecordSet::ReadBlockItems);

	HRESULT					hResult = S_OK ;
	m4bool_t				bIsNihil = M4_TRUE ;
	m4bool_t				bIsValid = M4_FALSE ;
	m4uint8_t				iLastLoadStatus = 0 ;
	m4uint32_t				iLastLoadRows = 0 ;
	m4uint32_t				iLastLoadFetchs = 0 ;
	m4uint32_t				i = 0 ;
	m4uint32_t				iColumn = 0 ;
	m4uint32_t				iItems = 0 ;
	m4uint32_t				iColumns = 0 ;
	ClBlock_Channel_Proxy	*poBlock = NULL ;
	SAFEARRAYBOUND			rgsabound[ 1 ] ;
	m4VariantType			vDefault ;
	VARIANT					*pvValues = NULL ;


	*ao_piLastLoadStatus = 0 ;
	*ao_piLastLoadRows = 0 ;
	*ao_piLastLoadFetchs = 0 ;
	*ao_piReturn = M4_ERROR ;

	if( *ao_avValues != NULL )
	{
		SafeArrayDestroy( *ao_avValues ) ;
		*ao_avValues = NULL ;
	}

	m_poRecordSet->GetLastLoadStatus( iLastLoadStatus, iLastLoadRows, iLastLoadFetchs ) ;

	*ao_piLastLoadStatus = iLastLoadStatus ;
	*ao_piLastLoadRows = iLastLoadRows ;
	*ao_piLastLoadFetchs = iLastLoadFetchs ;

	iItems = m_poRecordSet->Item.Count() ;

	// Se calcula el número de columnas con valor
	iColumns = 0 ;

	for( i = 0 ; i < iItems ; i++ )
	{
		m_poRecordSet->Item.MoveTo( i ) ;

		if( m_poRecordSet->Item.ItemDef.Type() != M4CL_ITEM_TYPE_METHOD )
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
		LEAVE_METHOD( CWrapCVM::ReadBlockItems, S_OK ) ;
	}

	hResult = SafeArrayAccessData( *ao_avValues, (void**) &pvValues ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot access SafeArray" ) ;
		SafeArrayDestroy( *ao_avValues ) ;
		*ao_avValues = NULL ;
		LEAVE_METHOD( CWrapCVM::ReadBlockItems, S_OK ) ;
	}

	if(	iColumns > 0 )
	{
		bIsNihil = M4_TRUE ;
		bIsValid = M4_FALSE ;
		poBlock = m_poRecordSet->GetCurrentBlock() ;

		if( poBlock != NULL )
		{
			bIsNihil = poBlock->IsNihil() ;
			bIsValid = poBlock->IsValid() ;
		}

		iColumn = 0 ;

		for( i = 0 ; i < iItems ; i++ )
		{
			m_poRecordSet->Item.MoveTo( i ) ;

			if( m_poRecordSet->Item.ItemDef.Type() != M4CL_ITEM_TYPE_METHOD )
			{
				// Se escribe el valor de cada celda
				if( bIsNihil == M4_FALSE )
				{
					/* Bug 0257678
					Si el bloque existe o el item está conectado se pide el valor, porque no se crea el bloque
					Si no existe y no está conectado se pone el valor por defecto, para que no se cree el bloque
					*/
					/* Bug 0265666 (detectado en el test de sincronización)
					Si el bloque no existe y el item realmente tiene ámbito de nodo, se debe tomar el valor correcto
					y no el de por defecto (no se creará el bloque)
					*/
					if( bIsValid == M4_TRUE || m_poRecordSet->Item.ItemDef.IsConnected() == M4_TRUE ||
						m_poRecordSet->Item.ItemDef.Scope() == M4CL_ITEM_SCOPE_NODE)
					{
						m_poItemIterator->get_Value( pvValues + iColumn ) ;
					}
					else
					{
						vDefault.SetNull() ;
						m_poRecordSet->Item.ItemDef.GetItemDefaultValue( vDefault ) ;
						vDefault.GetVARIANT( *(pvValues + iColumn) ) ;
					}
				}
				else
				{
					pvValues[ iColumn ].vt = VT_NULL ;
				}
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
		LEAVE_METHOD( CWrapCVM::ReadBlockItems, S_OK ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD(CWrapRecordSet::ReadBlockItems, S_OK);
}


STDMETHODIMP CWrapRecordSet::ReadChildrenTimeStamps(/*[out]*/ SAFEARRAY** ao_alTimes, /*[out, retval]*/ short* ao_piReturn)
{
	ENTER_METHOD(CWrapRecordSet::ReadChildrenTimeStamps);

	HRESULT				hResult = S_OK ;
	m4uint16_t			i = 0 ;
	m4uint16_t			iNodes = 0 ;
	m4uint32_t			iConnector = 0 ;
	m4uint32_t			iChildNode = 0 ;
	ClAccess			*poAccess = NULL ;
	ClNodeDef			*poNodeDef = NULL ;
	ClNode				*poNode = NULL ;
	SAFEARRAYBOUND		rgsabound[ 2 ] ;
	ULONGLONG			*plValues = NULL ;


	*ao_piReturn = M4_ERROR ;

	if( *ao_alTimes != NULL )
	{
		SafeArrayDestroy( *ao_alTimes ) ;
		*ao_alTimes = NULL ;
	}

	poNodeDef = m_poRecordSet->GetpNodeDef() ;
	poAccess = m_poRecordSet->GetpAccess() ;

	if( poAccess != NULL && poNodeDef != NULL )
	{
		iNodes = poNodeDef->GetNodeNumberOfRBConnectors() + poNodeDef->GetNodeNumberOfBBConnectors() ;

		rgsabound[ 0 ].lLbound = 0 ;
		rgsabound[ 0 ].cElements = iNodes ;
		rgsabound[ 1 ].lLbound = 0 ;
		rgsabound[ 1 ].cElements = 5 ;

		*ao_alTimes = SafeArrayCreate( VT_UI8, 2, rgsabound ) ;

		if( *ao_alTimes == NULL )
		{
			FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot create SafeArray" ) ;
			LEAVE_METHOD( CWrapAccess::ReadChildrenTimeStamps, S_OK ) ;
		}

		hResult = SafeArrayAccessData( *ao_alTimes, (void**) &plValues ) ;

		if( hResult != S_OK )
		{
			FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot access SafeArray" ) ;
			SafeArrayDestroy( *ao_alTimes ) ;
			*ao_alTimes = NULL ;
			LEAVE_METHOD( CWrapAccess::ReadChildrenTimeStamps, S_OK ) ;
		}

		memset( plValues, 0, iNodes * 5 * sizeof( ULONGLONG ) ) ;

	    for( i = 0 ; i < iNodes ; i++ )
		{
			iConnector = poNodeDef->GetNodeConnectorHandleByPosition( i ) ;
			iChildNode = poNodeDef->GetNodeConnectorSonNodeHandle( iConnector ) ;

			poNode = poAccess->Node.Get( ClHandle( iChildNode ) ) ;

			if( poNode != NULL )
			{
				plValues[ iNodes * 0 + i ] = poNode->RecordSet.GetNodeTimeStamp() ;
				plValues[ iNodes * 1 + i ] = poNode->RecordSet.GetBlockTimeStamp() ;
				plValues[ iNodes * 2 + i ] = poNode->RecordSet.GetRecordTimeStamp() ;
				plValues[ iNodes * 3 + i ] = poNode->RecordSet.GetCurrentTimeStamp() ;
				plValues[ iNodes * 4 + i ] = poNode->GetpNodeDef()->Index() ;
			}
		}

		hResult = SafeArrayUnaccessData( *ao_alTimes ) ;

		if( hResult != S_OK )
		{
			FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot unaccess SafeArray" ) ;
			SafeArrayDestroy( *ao_alTimes ) ;
			*ao_alTimes = NULL ;
			LEAVE_METHOD( CWrapCVM::ReadChildrenTimeStamps, S_OK ) ;
		}
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD(CWrapRecordSet::ReadChildrenTimeStamps, S_OK);
}



