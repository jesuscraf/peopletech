//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapaccess.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapAccess
//
//==============================================================================

#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapaccess.h"
#include "wrapcolprop.h"
#include "wraprecordset.h"
#include "m4types.hpp"
#include "wrapcvm.h"
#include "newinterface.h"
#include "cvdate.h"
#include "wrapdef.h"
#include "gstring.h"
#include "wrapnode.h"
#include "wrapchannel.h"
#include "wrapimap.h"
#include "rerror.h"
#include "channel.hpp"
#include "access.hpp"
#include "clfileiod.hpp"  // for serialization
#include "wrapexecutor.h"
#include "executor.hpp"
#include "vmstepper.hpp"
#include "sttchn.hpp"
#include "m4unicode.hpp"
#include "m4loadll.hpp"
#include "clextman.hpp"
#include "chan_knl.hpp"
#include "cllgadap.hpp"
#include "dump_vst.hpp"
#include "m4stl.hpp"


extern M4ClLoaDll*	GetEngine( void ) ;


/////////////////////////////////////////////////////////////////////////////
// CWrapAccess

STDMETHODIMP CWrapAccess::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapAccess, &IID_IWrapColNodes, &IID_IWrapStack };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

CWrapAccess::CWrapAccess()
	: m_poAccess(0), m_poExecutor(0)
{
}

CWrapAccess::~CWrapAccess()
{
	//DBR. Nuevo para quitar un memory leak del m_poExecutor. Hay que quitar nuestra ref.
	if (m_poExecutor)
	{
        m_poExecutor->Release();
        m_poExecutor = 0;
    }

	if (m_poAccess != NULL && m_bIsOwner == M4_TRUE)
	{
		m_poAccess->Destroy();
		m_poAccess = 0;
	}
}

void CWrapAccess::Init(ClAccess* ai_VarRef,	bool ai_bIsOwner)
{
	m_poAccess = ai_VarRef;
	m_bIsOwner = ai_bIsOwner;
}

void CWrapAccess::Unwrap(ClAccess** ao_VarRef)
{
	*ao_VarRef = m_poAccess;
}

STDMETHODIMP CWrapAccess::GetRecordSet(BSTR ai_bstrNodeName, IWrapRecordSet** ao_poRecordSet)
{
	ENTER_METHOD(CWrapAccess::GetRecordSet);

	gstring tmpGstring(ai_bstrNodeName);

	if (tmpGstring)
	{
		TRACE2("Trying to get block %s", m4pchar_t(tmpGstring));

		ClAccessRecordSet* poRecordSet = 0;
		ClNode* pNode = m_poAccess->Node.Get(tmpGstring);
		if (pNode) {
			poRecordSet = &(pNode->RecordSet);
		}

		TRACE2("RecordSet pointer %p", poRecordSet);
	
		if (poRecordSet)
		{
			if (NewInterface(CLSID_WrapRecordSet, IID_IWrapRecordSet, (IUnknown**)ao_poRecordSet) != M4_SUCCESS)
			{
				LEAVE_METHOD(CWrapAccess::GetRecordSet, E_UNEXPECTED);
			}
			else
			{
				((CWrapRecordSet*)(*ao_poRecordSet))->Init(poRecordSet);
				LEAVE_METHOD(CWrapAccess::GetRecordSet, S_OK);
			}
		}
	}

	FormatError(M4_CH_WO_CANT_INITIALISE_BLOCK,	"Unable to get node to initialise record set");
	LEAVE_METHOD(CWrapAccess::GetRecordSet, E_UNEXPECTED);		
}

// Another interface...

STDMETHODIMP CWrapAccess::GetNodeById(int ai_iId, IWrapNode** ao_poNode)
{ 
	ENTER_METHOD(CWrapAccess::GetNodeById);

	TRACE2("Getting node from access with id %d", ai_iId);

	ClNode* poNode = &(m_poAccess->Node[(ClNodeIndex)ai_iId]);

	TRACE2("Node has address %p", m_poAccess);

	if (poNode)
	{
		if(NewInterface(CLSID_WrapNode,	IID_IWrapNode, (IUnknown**)ao_poNode) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapAccess::GetNodeById, E_UNEXPECTED);
		}
		else
		{
			((CWrapNode*)(*ao_poNode))->Init(poNode);
			LEAVE_METHOD(CWrapAccess::GetNodeById, S_OK);
		}
	}
	else
	{
		FormatError(M4_CH_WO_CANT_INITIALISE_BLOCK,	"Unable to get node to initialise record set");
		LEAVE_METHOD(CWrapAccess::GetNodeById, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapAccess::GetNodeByName(BSTR ai_bstrName, IWrapNode** ao_poNode)
{
	ENTER_METHOD(CWrapAccess::GetNodeByName);

	gstring tmpGstring(ai_bstrName);

	if (tmpGstring)
	{
		ClNode* poNode = &(m_poAccess->Node[tmpGstring]);

		TRACE3("Node with id %s has address %p", m4pchar_t(tmpGstring), poNode);

		if (poNode)
		{
			if (NewInterface(CLSID_WrapNode, IID_IWrapNode, (IUnknown**)ao_poNode) != M4_SUCCESS)
			{
				LEAVE_METHOD(CWrapAccess::GetNodeByName, E_UNEXPECTED);
			}
			else
			{
				((CWrapNode*)(*ao_poNode))->Init(poNode);
				LEAVE_METHOD(CWrapAccess::GetNodeByName, S_OK);
			}
		}
	}

	FormatError(M4_CH_WO_CANT_INITIALISE_BLOCK,	"Unable to get node to initialise record set");
	LEAVE_METHOD(CWrapAccess::GetNodeByName,	E_UNEXPECTED);
}

STDMETHODIMP CWrapAccess::get_CoNodes(IWrapColNodes** pVal)
{
	ENTER_METHOD(CWrapAccess::get_CoNodes);

	// Just cast this and return the pointer
	// Add one to the reference counts...
	// of the base class
	*pVal = (IWrapColNodes*) this;

	TRACE("Casting Access to a collection of nodes");

	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapAccess::GetNodeByName, S_OK);
}

STDMETHODIMP CWrapAccess::get_Stack(IWrapStack** pVal)
{
	ENTER_METHOD(CWrapAccess::get_Stack);

	// Just cast this and return a pointer to
	// the stack
	// Add one to the reference counts...
	// of the base class...
	*pVal = (IWrapStack*) this;

	TRACE("Casting Access to a collection of nodes");

	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapAccess::get_Stack, S_OK);
}

STDMETHODIMP CWrapAccess::Pop(VARIANT* ao_varref, short* ao_RetVal)
{
	//cambiado el tratt. del VARIANT. Pipo
	m4VariantType var;

	ENTER_METHOD(CWrapAccess::Pop);
	
	*ao_RetVal = m_poAccess->GetpExecutor()->Stack.Pop(var);

	var.GetVARIANT(*ao_varref);

	if (*ao_RetVal == M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapAccess::Pop, S_OK);
	}

	LEAVE_METHOD(CWrapAccess::Pop, E_UNEXPECTED);
}

STDMETHODIMP CWrapAccess::Push(VARIANT ai_varref, short* ao_RetVal)
{
	ENTER_METHOD(CWrapAccess::Push);
	
	m4VariantType pvar(ai_varref);

	*ao_RetVal = m_poAccess->GetpExecutor()->Stack.Push(pvar);

	LEAVE_METHOD(CWrapAccess::Push, S_OK);
}

STDMETHODIMP CWrapAccess::Reset()
{
	ENTER_METHOD(CWrapAccess::Reset);

	IExecutor* pExecutor = m_poAccess->GetpExecutor();

	pExecutor->ResetStack();

	LEAVE_METHOD(CWrapAccess::Reset, S_OK);
}

STDMETHODIMP CWrapAccess::get_Count(long* ao_iCount)
{
	ENTER_METHOD(CWrapAccess::get_Count);

	*ao_iCount = m_poAccess->Node.Count();

	TRACE2("The count iS %d", *ao_iCount);

	LEAVE_METHOD(CWrapAccess::get_Count, S_OK);
}

STDMETHODIMP CWrapAccess::get_coProps(IWrapColProp** pVal)
{
	ENTER_METHOD(CWrapAccess::get_coProps);

	if (NewInterface(CLSID_WrapColProp,	IID_IWrapColProp, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapAccess::get_coProps, E_UNEXPECTED);
	}
	else
	{
		((CWrapColProp*)(*pVal))->Init(&m_poAccess->Property);
		LEAVE_METHOD(CWrapAccess::get_coProps, S_OK);
	}

	LEAVE_METHOD(CWrapAccess::get_coProps, E_UNEXPECTED);
}


STDMETHODIMP CWrapAccess::CheckPoint(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapAccess::CheckPoint);

	// Bug 0113644 Deprecated. No longer supported.
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapAccess::CheckPoint" << "M4ChWrap.dll" );

	LEAVE_METHOD(CWrapAccess::CheckPoint, S_OK);
}


STDMETHODIMP CWrapAccess::get_WorkDate(DATE* pVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapAccess::get_WorkDate);

	m_poAccess->ExecutionDate.Get(tmpVar);
	
	*pVal = M4dateToDate(tmpVar);

	TRACE3("Converting m4date %g to date %g", tmpVar, *pVal);

	LEAVE_METHOD(CWrapAccess::get_WorkDate, S_OK);
}

STDMETHODIMP CWrapAccess::put_WorkDate(DATE newVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapAccess::put_WorkDate);

	tmpVar.Type = M4CL_CPP_TYPE_DATE;
	tmpVar.Data.DoubleData = DateToM4date(newVal);

	m_poAccess->ExecutionDate.Set(tmpVar);

	TRACE3("Converting date %g to date m4 %g", newVal, tmpVar);

	LEAVE_METHOD(CWrapAccess::put_WorkDate, S_OK);
}

STDMETHODIMP CWrapAccess::get_RunSliceMode(VARIANT_BOOL* pVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapAccess::get_RunSliceMode);

	m_poAccess->RunSliceMode.Get(tmpVar);

	if ((m4double_t)tmpVar)
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapAccess::get_RunSliceMode, S_OK);
}

STDMETHODIMP CWrapAccess::put_RunSliceMode(VARIANT_BOOL newVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapAccess::put_RunSliceMode);

	tmpVar.Type = M4CL_CPP_TYPE_NUMBER;
	tmpVar.Data.DoubleData = (newVal == VARIANT_TRUE);

	m_poAccess->RunSliceMode.Set(tmpVar);

	LEAVE_METHOD(CWrapAccess::put_RunSliceMode, S_OK);
}

STDMETHODIMP CWrapAccess::Load(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapAccess::Load);

   	*ao_sRetVal = m_poAccess->Load( M4_TRUE );

    ATLTRACE("Access::Load: IdChannel = <%s> RetVal =<%d>\n", m_poAccess->GetpChannel()->GetpChannelDef()->Id(), *ao_sRetVal);

	LEAVE_METHOD(CWrapAccess::Load, S_OK);
}

STDMETHODIMP CWrapAccess::UnLoad(short * ao_sRetVal)
{
	ENTER_METHOD(CWrapAccess::UnLoad);

   	*ao_sRetVal = m_poAccess->UnLoad();

	LEAVE_METHOD(CWrapAccess::UnLoad, S_OK);
}

STDMETHODIMP CWrapAccess::Persist(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapAccess::Persist);

	*ao_sRetVal = m_poAccess->Persist( M4_TRUE );

	LEAVE_METHOD(CWrapAccess::Persist, S_OK);
}

STDMETHODIMP CWrapAccess::LoadFromFile(BSTR ai_pstFileName, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapAccess::LoadFromFile);

	gstring tmpGstring(ai_pstFileName);

	if (tmpGstring)
	{
		// Apaño para cargar ficheros de staísticas
		m4pcchar_t	pccFileName = tmpGstring ;
		size_t	iLength = strlen( pccFileName ) ;

		if( iLength > 4
			&& strcmpi( pccFileName + iLength - 4, ".stt" ) == 0
			&& strcmpi( m_poAccess->GetpChannel()->GetpChannelDef()->Id(), M4_CHANNEL_STATISTICS ) == 0 )
		{
			*ao_sRetVal = m_poAccess->LoadFromSttFile(tmpGstring);
		}
		else
		{
			*ao_sRetVal = m_poAccess->LoadFromFile(tmpGstring);
		}

		LEAVE_METHOD(CWrapAccess::LoadFromFile, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
		LEAVE_METHOD(CWrapAccess::LoadFromFile,	E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapAccess::PersistToFile(BSTR ai_pstFileName, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapAccess::PersistToFile);

	gstring tmpGstring(ai_pstFileName);

	if (tmpGstring)
	{
		*ao_sRetVal = m_poAccess->PersistToFile(tmpGstring, M4_FALSE);
		LEAVE_METHOD(CWrapAccess::PersistToFile, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
		LEAVE_METHOD(CWrapAccess::PersistToFile,	E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapAccess::PersistToTempFile(BSTR * ao_pstFileName, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapAccess::PersistToTempFile);

	m4char_t aFileName[256];

	*ao_sRetVal = m_poAccess->PersistToTempFile(aFileName, 256);

	if (M4_SUCCESS == *ao_sRetVal)
	{
		gstring tmpGstring(aFileName);
	
		if (tmpGstring)
		{
			*ao_pstFileName = (BSTR)tmpGstring;

			LEAVE_METHOD(CWrapAccess::PersistToTempFile, S_OK);
		}
		else
		{
			FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
			LEAVE_METHOD(CWrapAccess::PersistToTempFile,	E_UNEXPECTED);
		}
	}

	LEAVE_METHOD(CWrapAccess::PersistToTempFile, S_OK);
}


STDMETHODIMP CWrapAccess::Free(short* ao_retval)
{
	ENTER_METHOD(CWrapAccess::Free);

	*ao_retval = m_poAccess->GetpChannel()->UnLoad(M4_FALSE);
		
	TRACE2("Returning the value %d", *ao_retval);
	
	LEAVE_METHOD(CWrapAccess::Free, S_OK);
}

//EDU----------------------->>>
STDMETHODIMP CWrapAccess::get_ExecutionStartDate(DATE* pVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapAccess::get_ExecutionStartDate);

	m_poAccess->ExecutionStartDate.Get(tmpVar);
	
	*pVal = M4dateToDate(tmpVar);

	TRACE3("Converting m4date %g to date %g", tmpVar, *pVal);

	LEAVE_METHOD(CWrapAccess::get_ExecutionStartDate, S_OK);
}

STDMETHODIMP CWrapAccess::put_ExecutionStartDate(DATE newVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapAccess::put_ExecutionStartDate);

	tmpVar.Type = M4CL_CPP_TYPE_DATE;
	tmpVar.Data.DoubleData = DateToM4date(newVal);

	m_poAccess->ExecutionStartDate.Set(tmpVar);

	TRACE3("Converting date %g to date m4 %g", newVal, tmpVar);

	LEAVE_METHOD(CWrapAccess::put_ExecutionStartDate, S_OK);
}

STDMETHODIMP CWrapAccess::get_ExecutionEndDate(DATE* pVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapAccess::get_ExecutionEndDate);

	m_poAccess->ExecutionEndDate.Get(tmpVar);
	
	*pVal = M4dateToDate(tmpVar);

	TRACE3("Converting m4date %g to date %g", tmpVar, *pVal);

	LEAVE_METHOD(CWrapAccess::get_ExecutionEndDate, S_OK);
}

STDMETHODIMP CWrapAccess::put_ExecutionEndDate(DATE newVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapAccess::put_ExecutionEndDate);

	tmpVar.Type = M4CL_CPP_TYPE_DATE;
	tmpVar.Data.DoubleData = DateToM4date(newVal);

	m_poAccess->ExecutionEndDate.Set(tmpVar);

	TRACE3("Converting date %g to date m4 %g", newVal, tmpVar);

	LEAVE_METHOD(CWrapAccess::put_ExecutionEndDate, S_OK);
}

STDMETHODIMP CWrapAccess::Release(short* ao_retval)
{
	ENTER_METHOD(CWrapAccess::Release);

	*ao_retval = m_poAccess->GetpChannel()->ReleaseAll();
		
	TRACE2("Returning the value %d", *ao_retval);
	
	LEAVE_METHOD(CWrapAccess::Release, S_OK);
}

//EDU-----------------------<<<


//DANI
STDMETHODIMP CWrapAccess::get_Handle(long* ao_iHandle)
{
	ENTER_METHOD(CWrapAccess::get_Handle);

	*ao_iHandle = m_poAccess->GetHandle();
		
	TRACE2("Returning the value %d", *ao_iHandle);
	
	LEAVE_METHOD(CWrapAccess::get_Handle, S_OK);
}

STDMETHODIMP CWrapAccess::get_GetChannel(IWrapChannel** pVal)
{
	ENTER_METHOD(CWrapAccess::get_GetChannel);

	if (NewInterface(CLSID_WrapChannel,	IID_IWrapChannel, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapAccess::get_GetChannel, E_UNEXPECTED);
	}
	else
	{
		((CWrapChannel*)(*pVal))->Init(m_poAccess->GetpChannel());
		LEAVE_METHOD(CWrapAccess::get_GetChannel, S_OK);
	}

	LEAVE_METHOD(CWrapAccess::get_GetChannel, E_UNEXPECTED);
}

STDMETHODIMP CWrapAccess::get_InstanceMap(IWrapIMap** pVal)
{
	ENTER_METHOD(CWrapAccess::get_InstanceMap);

	if (NewInterface(CLSID_WrapIMap, IID_IWrapIMap,	(IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapAccess::get_InstanceMap, E_UNEXPECTED);
	}
	else
	{
		((CWrapIMap*)(*pVal))->Init(&m_poAccess->GetpChannel()->Level2);
		LEAVE_METHOD(CWrapAccess::get_InstanceMap, S_OK);
	}

	LEAVE_METHOD(CWrapAccess::get_InstanceMap, E_UNEXPECTED);
}

//DANI

STDMETHODIMP CWrapAccess::get_SttStatus(short * pVal)
{
	ENTER_METHOD(CWrapAccess::get_SttStatus);

	*pVal = m_poAccess->GetpChannel ()->GetStatisticsLevel();

	LEAVE_METHOD(CWrapAccess::get_SttStatus,	S_OK);
}

STDMETHODIMP CWrapAccess::put_SttStatus(short newVal)
{
	ENTER_METHOD(CWrapAccess::put_SttStatus);

	m_poAccess->GetpChannel ()->SetStatisticsLevel((m4uint16_t)newVal);

	LEAVE_METHOD(CWrapAccess::put_SttStatus, S_OK);
}

STDMETHODIMP CWrapAccess::Serialize(BSTR ai_stFilename, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapAccess::Serialize);

	ClFileIODriver IOD;

	gstring tmpGstring(ai_stFilename);

	if (tmpGstring)
	{
		TRACE2("Trying to open file %s", m4pchar_t(tmpGstring));

		*ao_sRetVal = IOD.Init(GEN_IO_MODE_WRITE, tmpGstring);

		if (*ao_sRetVal == M4_SUCCESS)
		{
			*ao_sRetVal = m_poAccess->Serialize(IOD);

			if (*ao_sRetVal != M4_SUCCESS)
			{
				FormatError(M4_CH_WO_CANT_SERIALIZE_ACCESS, "Cannot serialize the access");
			}

			IOD.End(M4_TRUE);
		}
		else
		{
			FormatError(M4_CH_WO_CANT_OPEN_FILE, "Unable to open file %s", m4pchar_t(tmpGstring));
		}
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
	}

	if (*ao_sRetVal == M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapAccess::Serialize, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapAccess::Serialize, E_UNEXPECTED);
	}	
}

STDMETHODIMP CWrapAccess::DeSerialize(BSTR ai_stFilename, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapAccess::DeSerialize);

	ClFileIODriver IOD;

	gstring tmpGstring(ai_stFilename);

	if (tmpGstring)
	{
		TRACE2("Trying to open file %s", m4pchar_t(tmpGstring));

		*ao_sRetVal = IOD.Init(GEN_IO_MODE_READ, tmpGstring);

		if (*ao_sRetVal == M4_SUCCESS)
		{
			*ao_sRetVal = m_poAccess->DeSerialize(IOD);

			if (*ao_sRetVal != M4_SUCCESS)
			{
				FormatError(M4_CH_WO_CANT_DESERIALIZE_ACCESS, "Cannot deserialize the access");
			}

			IOD.End(M4_TRUE);
		}
		else
		{
			FormatError(M4_CH_WO_CANT_OPEN_FILE, "Unable to open file %s", m4pchar_t(tmpGstring));
		}
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
	}

	if (*ao_sRetVal == M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapAccess::DeSerialize, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapAccess::DeSerialize, E_UNEXPECTED);
	}	
}

STDMETHODIMP CWrapAccess::GetNodeByHandle(long ai_iHandle, IWrapNode** ao_poNode)
{
	ENTER_METHOD(CWrapAccess::GetNodeByHandle);

	TRACE2("Getting node from access with handle %d", ai_iHandle);

	ClNode* poNode = &(m_poAccess->Node[(ClHandle)ai_iHandle]);

	TRACE2("Node has address %p", m_poAccess);

	if (poNode)
	{
		if(NewInterface(CLSID_WrapNode,	IID_IWrapNode, (IUnknown**)ao_poNode) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapAccess::GetNodeByHandle, E_UNEXPECTED);
		}
		else
		{
			((CWrapNode*)(*ao_poNode))->Init(poNode);
		
			LEAVE_METHOD(CWrapAccess::GetNodeByHandle,S_OK);
		}
	}
	else
	{
		FormatError(M4_CH_WO_CANT_INITIALISE_BLOCK, "Unable to get node to initialise record set");
		LEAVE_METHOD(CWrapAccess::GetNodeByHandle, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapAccess::get_SttLabel(BSTR * ao_pstLabel)
{
	ENTER_METHOD(CWrapAccess::get_SttLabel);

	m4pchar_t pclabel = m_poAccess->GetpChannel ()->GetStatisticsLabel();
	if (pclabel == 0) pclabel = "";

	gstring tmpGstring(pclabel);

	if (tmpGstring)
	{
		*ao_pstLabel = (BSTR)tmpGstring;
	
		if (ao_pstLabel)
		{
			LEAVE_METHOD(CWrapAccess::get_SttLabel, S_OK);
		}
	}

	LEAVE_METHOD(CWrapAccess::get_SttLabel, E_UNEXPECTED);

}

STDMETHODIMP CWrapAccess::put_SttLabel(BSTR ai_pstName)
{
	ENTER_METHOD(CWrapAccess::put_SttLabel);

	gstring tmpGstring(ai_pstName);

	if (tmpGstring)
	{
		m_poAccess->GetpChannel ()->SetStatisticsLabel(tmpGstring);
	}
	else
	{
		m_poAccess->GetpChannel ()->SetStatisticsLabel("");
	}

	LEAVE_METHOD(CWrapAccess::put_SttLabel, S_OK);
}


STDMETHODIMP CWrapAccess::get_Executor(IWrapExecutor **pVal)
{
	ENTER_METHOD(CWrapAccess::get_Executor);

	if (!m_poExecutor)
	{
		TRACE("Allocating new executor");

		if (NewInterface(CLSID_WrapExecutor, IID_IWrapExecutor, (IUnknown**) &m_poExecutor) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapRgIt::get_Executor, E_UNEXPECTED);
		}

		// Inicializar  el ejecutor con la factoría para los breakpoints
		if( m_poAccess != NULL )
		{
			ClChannel *poChannel = m_poAccess->GetpChannel() ;

			if( poChannel != NULL )
			{
				ClChannelManager *poChannelManager = poChannel->GetpChannelManager() ;

				if( poChannelManager != NULL )
				{
					((CWrapExecutor*)m_poExecutor)->SetFactory( poChannelManager->GetpFactory() ) ;
				}
			}
		}

		m4return_t ret = ((CWrapExecutor*)m_poExecutor)->Init((ClExecutor*)m_poAccess->GetpExecutor());

		if (ret!=M4_SUCCESS){
				LEAVE_METHOD(CWrapAccess::get_Executor,E_UNEXPECTED);
		}
	}

	TRACE2("The executor has address %p", m_poExecutor);

	*pVal = m_poExecutor;

	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapAccess::get_Executor, S_OK);
}

//--------------------------------------->>
//EDU, 07/05/98 
// debug dump facility:

STDMETHODIMP CWrapAccess::DebugDump(BSTR ai_stFilename)
{
    ENTER_METHOD(CWrapAccess::DebugDump);

    gstring tmpGstring(ai_stFilename);

    ofstream osf;
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;

	M4Open( osf, tmpGstring, M4UniWrite, eEncoding );

    if (osf.is_open ()) {
        ClSimpleDump_Visitor pp (osf);

        pp.Visit_Access (*m_poAccess);
    }

    osf.close ();

    LEAVE_METHOD(CWrapAccess::DebugDump, S_OK);
}

STDMETHODIMP CWrapAccess::CanUnloadNow(VARIANT_BOOL *pVal)
{
    ENTER_METHOD(CWrapAccess::CanUnloadNow);

	*pVal = VARIANT_TRUE;

	LEAVE_METHOD(CWrapAccess::CanUnloadNow, S_OK);

}

//---------------------------------------<<

//ClAccessEventSink

STDMETHODIMP CWrapAccess::GetCurrentNode(IWrapNode **ao_poNode)
{
	// TODO: Add your implementation code here
	ENTER_METHOD(CWrapAccess::GetCurrentNode);

	TRACE("Getting current node from access");


	// Operador * sobrecargado en m_poAccess->Node.
	// Devuelve *m_pCurrentNode;
	ClNode* poNode = &(*(m_poAccess->Node));

	TRACE2("Node has address %p", m_poAccess);

	if (poNode)
	{
		if(NewInterface(CLSID_WrapNode,	IID_IWrapNode, (IUnknown**)ao_poNode) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapAccess::GetCurrentNode, E_UNEXPECTED);
		}
		else
		{
			((CWrapNode*)(*ao_poNode))->Init(poNode);
		
			LEAVE_METHOD(CWrapAccess::GetCurrentNode, S_OK);
		}
	}
	else
	{
		FormatError(M4_CH_WO_CANT_INITIALISE_BLOCK,	"Unable to get node to initialise record set");
		LEAVE_METHOD(CWrapAccess::GetCurrentNode, E_UNEXPECTED);
	}
}


STDMETHODIMP CWrapAccess::PersistToFileAdvanced(BSTR ai_pstFileName, VARIANT_BOOL ai_bSerializeMCR, short *ao_sRetVal)
{
	ENTER_METHOD(CWrapAccess::PersistToFileAdvanced);

	gstring tmpGstring(ai_pstFileName);

	if (tmpGstring)
	{
	    m4bool_t bSerializeMCR = (ai_bSerializeMCR == VARIANT_TRUE) ? M4_TRUE : M4_FALSE;
		*ao_sRetVal = m_poAccess->PersistToFile(tmpGstring, bSerializeMCR);

		LEAVE_METHOD(CWrapAccess::PersistToFile, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
		LEAVE_METHOD(CWrapAccess::PersistToFileAdvanced,	E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapAccess::LoadFromSttFile(BSTR ai_pstFileName, short *ao_sRetVal)
{
	ENTER_METHOD(CWrapAccess::LoadFromSttFile);

	gstring tmpGstring(ai_pstFileName);

	if (tmpGstring)
	{
		*ao_sRetVal = m_poAccess->LoadFromSttFile(tmpGstring);
		LEAVE_METHOD(CWrapAccess::LoadFromSttFile, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
		LEAVE_METHOD(CWrapAccess::LoadFromSttFile, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapAccess::get_GetChannelAsOwner(IWrapChannel **pVal)
{
	ENTER_METHOD(CWrapAccess::get_GetChannelAsOwner);

	if (NewInterface(CLSID_WrapChannel,	IID_IWrapChannel, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapAccess::get_GetChannelAsOwner, E_UNEXPECTED);
	}
	else
	{
		((CWrapChannel*)(*pVal))->Init(m_poAccess->GetpChannel(), true);
		LEAVE_METHOD(CWrapAccess::get_GetChannelAsOwner, S_OK);
	}

	LEAVE_METHOD(CWrapAccess::get_GetChannelAsOwner, E_UNEXPECTED);
}

STDMETHODIMP CWrapAccess::ReadProperties(SAFEARRAY** ao_avValues, short* ao_piReturn)
{
	ENTER_METHOD(CWrapAccess::ReadProperties);

	HRESULT			hResult = S_OK ;
	m4return_t		iResult = M4_SUCCESS ;
	size_t			i = 0 ;
	size_t			iProperty = 0 ;
	size_t			iProperties = 0 ;
	size_t			iChannelProperties = 0 ;
	size_t			iAccessProperties = 0 ;
	m4pchar_t		pcName = NULL ;
	m4VariantType	vValue ;
	SAFEARRAYBOUND	rgsabound[ 2 ] ;
	VARIANT			*pvValues = NULL ;


	*ao_piReturn = M4_ERROR ;

	if( *ao_avValues != NULL )
	{
		SafeArrayDestroy( *ao_avValues ) ;
		*ao_avValues = NULL ;
	}

	ClColProp& oChannelProperties = m_poAccess->GetpChannel()->Property ;
	ClColProp& oAccessProperties = m_poAccess->Property ;

	iChannelProperties = oChannelProperties.Count() ;
	iAccessProperties = oAccessProperties.Count() ;

	// Primero se cuentan las propiedades
	iProperties = 0 ;

	for( i = 0 ; i < iChannelProperties ; i++ )
	{
		pcName = oChannelProperties.GetPropName( i ) ;

		if( strcmp( pcName, "START_DATE" ) == 0 || strcmp( pcName, "END_DATE" ) == 0 || strcmp( pcName, "EXECUTION_START_DATE" ) == 0 || strcmp( pcName, "EXECUTION_END_DATE" ) == 0 || strcmp( pcName, "EXECUTION_DATE" ) == 0 )
		{
            // Estas propiedades tienen conflicto con las del acceso
            continue;
        }

		iProperties++;
	}

	iProperties += iAccessProperties ;

	// Se crea el array de dos dimensiones, nombre y valor, para cada propiedad
	rgsabound[ 0 ].lLbound = 0 ;
	rgsabound[ 0 ].cElements = iProperties ;
	rgsabound[ 1 ].lLbound = 0 ;
	rgsabound[ 1 ].cElements = 2 ;

	*ao_avValues = SafeArrayCreate( VT_VARIANT, 2, rgsabound ) ;

	if( *ao_avValues == NULL )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot create SafeArray" ) ;
		LEAVE_METHOD( CWrapAccess::ReadProperties, S_OK ) ;
	}

	hResult = SafeArrayAccessData( *ao_avValues, (void**) &pvValues ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot access SafeArray" ) ;
		SafeArrayDestroy( *ao_avValues ) ;
		*ao_avValues = NULL ;
		LEAVE_METHOD( CWrapAccess::ReadProperties, S_OK ) ;
	}

	// Se asignan los valores
	iProperty = 0 ;

	for( i = 0 ; i < iChannelProperties ; i++ )
	{
		pcName = oChannelProperties.GetPropName( i ) ;

		if( strcmp( pcName, "START_DATE" ) == 0 || strcmp( pcName, "END_DATE" ) == 0 || strcmp( pcName, "EXECUTION_START_DATE" ) == 0 || strcmp( pcName, "EXECUTION_END_DATE" ) == 0 || strcmp( pcName, "EXECUTION_DATE" ) == 0 )
		{
            // Estas propiedades tienen conflicto con las del acceso
            continue;
        }

		vValue.SetString( pcName ) ;
		vValue.GetVARIANT( *(pvValues + iProperty) ) ;

		iResult = oChannelProperties.GetPropValue( i, vValue ) ;

		if( iResult != M4_SUCCESS )
		{
			FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot GetPropValue" ) ;
			SafeArrayDestroy( *ao_avValues ) ;
			*ao_avValues = NULL ;
			LEAVE_METHOD( CWrapAccess::ReadProperties, S_OK ) ;
		}

		vValue.GetVARIANT( *(pvValues + iProperties + iProperty) ) ;
		iProperty++ ;
	}

	for( i = 0 ; i < iAccessProperties ; i++ )
	{
		pcName = oAccessProperties.GetPropName( i ) ;

		vValue.SetString( pcName ) ;
		vValue.GetVARIANT( *(pvValues + iProperty) ) ;

		iResult = oAccessProperties.GetPropValue( i, vValue ) ;

		if( iResult != M4_SUCCESS )
		{
			FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot GetPropValue" ) ;
			SafeArrayDestroy( *ao_avValues ) ;
			*ao_avValues = NULL ;
			LEAVE_METHOD( CWrapAccess::ReadProperties, S_OK ) ;
		}

		vValue.GetVARIANT( *(pvValues + iProperties + iProperty) ) ;
		iProperty++ ;
	}

	hResult = SafeArrayUnaccessData( *ao_avValues ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot unaccess SafeArray" ) ;
		SafeArrayDestroy( *ao_avValues ) ;
		*ao_avValues = NULL ;
		LEAVE_METHOD( WrapAccess::ReadProperties, S_OK ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD(CWrapAccess::ReadProperties, S_OK);
}


STDMETHODIMP CWrapAccess::ExecuteReport(BSTR ai_sCommandString, short ai_iManager, short ai_iDesigner, short ai_iData, VARIANT ai_vDesigner, BSTR *ao_psFile, short *ao_piReturn)
{
	ENTER_METHOD(CWrapAccess::ExecuteReport);

	m4bool_t		bCancel = M4_FALSE ;
	m4return_t		iResult = M4_ERROR ;
	ClAccess		*poDesignAccess = NULL ;
	ClChannel		*poChannel = NULL ;
	m4char_t		acOutputFile[ MAX_PATH + 1 ] ;
	M4ClLoaDll		*poEngine = NULL ;
	m4int16_t		(*pfEngine)( const m4char_t*, m4int16_t, m4int16_t, m4int16_t, ClAccess*, ClAccess*, ClAccess*, m4bool_t*, m4char_t*, size_t, m4char_t*, size_t, m4bool_t ) ;
	m4typefunreturn	poFunction = NULL ;

	gstring	tmpCommandString( ai_sCommandString ) ;
	m4VariantType vDesigner( ai_vDesigner ) ;


	*ao_piReturn = M4_ERROR ;

	poEngine = GetEngine() ;

	if( poEngine == NULL )
	{
		LEAVE_METHOD( CWrapCVM::ExecuteReport, S_OK ) ;
	}

	if( poEngine->GetFunc( "ExecuteReport2", poFunction ) != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::ExecuteReport, S_OK ) ;
	}

	pfEngine = ( m4int16_t(*)( const m4char_t*, m4int16_t, m4int16_t, m4int16_t, ClAccess*, ClAccess*, ClAccess*, m4bool_t*, m4char_t*, size_t, m4char_t*, size_t, m4bool_t ) ) poFunction ;

	poChannel = m_poAccess->GetpChannel() ;

	if( vDesigner.Type == M4CL_CPP_TYPE_NUMBER )
	{
		// Si es número es el handle físico del acceso
		poDesignAccess = poChannel->GetpChannelManager()->GetAccessByHandle( vDesigner.Data.DoubleData ) ;
	}
	else if( vDesigner.Type == M4CL_CPP_TYPE_STRING_VAR )
	{
		// Si es cadena es el alias de nivel 2
		iResult = poChannel->Level2.m_oInstanceMap.Find( vDesigner.Data.PointerChar, &poDesignAccess ) ;

		if( iResult != M4_SUCCESS )
		{
			poDesignAccess = NULL ;
		}
	}

	iResult = (*pfEngine)( tmpCommandString, ai_iManager, ai_iDesigner, ai_iData, m_poAccess, poDesignAccess, m_poAccess, &bCancel, NULL, 0, acOutputFile, MAX_PATH, M4_TRUE );

	gstring tmpOutputFile( acOutputFile ) ;
	*ao_psFile = (BSTR)tmpOutputFile ;

	if( bCancel == M4_TRUE )
	{
		*ao_piReturn = M4CL_USER_ABORT_ALL_REQUEST ;
	}
	else
	{
		*ao_piReturn = iResult ;
	}

	LEAVE_METHOD(CWrapAccess::ExecuteReport, S_OK);
}


STDMETHODIMP CWrapAccess::ReadAllTimeStamps(/*[out]*/ ULONGLONG* ao_plSession, /*[out]*/ ULONGLONG* ao_plProperties, /*[out]*/ SAFEARRAY** ao_alTimes, /*[out, retval]*/ short* ao_piReturn)
{
	ENTER_METHOD(CWrapAccess::ReadAllTimeStamps);

	HRESULT				hResult = S_OK ;
	m4uint16_t			i = 0 ;
	m4uint16_t			iNodes = 0 ;
	m4uint64_t			lTime = 0 ;
	m4uint64_t			lAuxTime = 0 ;
	ClChannel			*poChannel = NULL ;
	ClChannel			*poSessionChannel = NULL ;
	ClChannelManager	*poChannelManager = NULL ;
	ClLogonAdaptor		*poLogonAdaptor = NULL ;
	ClAccessRecordSet	*poRecordSet = NULL ;
	ClNode				*poNode = NULL ;
	SAFEARRAYBOUND		rgsabound[ 2 ] ;
	ULONGLONG			*plValues = NULL ;


	*ao_piReturn = M4_ERROR ;
	*ao_plSession =  0;
	*ao_plProperties =  0;

	if( *ao_alTimes != NULL )
	{
		SafeArrayDestroy( *ao_alTimes ) ;
		*ao_alTimes = NULL ;
	}

	poChannel = m_poAccess->GetpChannel() ;

	if( poChannel != NULL )
	{
		// Propiedades de sesión
		poChannelManager = poChannel->GetpChannelManager() ;

		if( poChannelManager != NULL )
		{
			poSessionChannel = poChannelManager->GetSessionChannel() ;

			if( poSessionChannel != NULL )
			{
				poLogonAdaptor = poChannelManager->GetLogonAdaptor() ;

				if( poLogonAdaptor != NULL )
				{
					poRecordSet = poLogonAdaptor->GetSessionRecordSet() ;

					if( poRecordSet != NULL )
					{
						*ao_plSession = poRecordSet->GetNodeTimeStamp() ;
					}
				}
			}
		}


		// Propiedades de canal
		lTime = poChannel->Property.GetTimeStamp() ;
		lAuxTime = m_poAccess->Property.GetTimeStamp() ;

		if( lAuxTime > lTime )
		{
			lTime = lAuxTime ;
		}

		*ao_plProperties = lTime ;


		// Nodos
		iNodes = m_poAccess->Node.Count() ;

		rgsabound[ 0 ].lLbound = 0 ;
		rgsabound[ 0 ].cElements = iNodes ;
		rgsabound[ 1 ].lLbound = 0 ;
		rgsabound[ 1 ].cElements = 4 ;

		*ao_alTimes = SafeArrayCreate( VT_UI8, 2, rgsabound ) ;

		if( *ao_alTimes == NULL )
		{
			FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot create SafeArray" ) ;
			LEAVE_METHOD( CWrapAccess::ReadAllTimeStamps, S_OK ) ;
		}

		hResult = SafeArrayAccessData( *ao_alTimes, (void**) &plValues ) ;

		if( hResult != S_OK )
		{
			FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot access SafeArray" ) ;
			SafeArrayDestroy( *ao_alTimes ) ;
			*ao_alTimes = NULL ;
			LEAVE_METHOD( CWrapAccess::ReadAllTimeStamps, S_OK ) ;
		}

		memset( plValues, 0, iNodes * 4 * sizeof( ULONGLONG ) ) ;

	    for( i = 0 ; i < iNodes ; i++ )
		{
			poNode = m_poAccess->Node.Get( ClNodeIndex( i ) ) ;

			if( poNode != NULL )
			{
				plValues[ iNodes * 0 + i ] = poNode->RecordSet.GetNodeTimeStamp() ;
				plValues[ iNodes * 1 + i ] = poNode->RecordSet.GetBlockTimeStamp() ;
				plValues[ iNodes * 2 + i ] = poNode->RecordSet.GetRecordTimeStamp() ;
				plValues[ iNodes * 3 + i ] = poNode->RecordSet.GetCurrentTimeStamp() ;
			}
		}

		hResult = SafeArrayUnaccessData( *ao_alTimes ) ;

		if( hResult != S_OK )
		{
			FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot unaccess SafeArray" ) ;
			SafeArrayDestroy( *ao_alTimes ) ;
			*ao_alTimes = NULL ;
			LEAVE_METHOD( CWrapCVM::ReadAllTimeStamps, S_OK ) ;
		}
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD(CWrapAccess::ReadAllTimeStamps, S_OK);
}


STDMETHODIMP CWrapAccess::get_LastTimeStamp(ULONGLONG *ao_plTime)
{
	ENTER_METHOD(CWrapAccess::get_LastTimeStamp);

	*ao_plTime = ClChangeListener::GetLastTimeStamp() ;

	LEAVE_METHOD(CWrapAccess::get_LastTimeStamp, S_OK);
}


STDMETHODIMP CWrapAccess::ReadTimeStamps(/*[out]*/ UINT* ao_plSession, /*[out]*/ UINT* ao_plProperties, /*[out]*/ SAFEARRAY** ao_alTimes, /*[out, retval]*/ short* ao_piReturn)
{
	ENTER_METHOD(CWrapAccess::ReadTimeStamps);

	HRESULT				hResult = S_OK ;
	m4uint16_t			i = 0 ;
	m4uint16_t			iNodes = 0 ;
	m4uint64_t			iTime = 0 ;
	m4uint64_t			iAuxTime = 0 ;
	ClChannel			*poChannel = NULL ;
	ClChannel			*poSessionChannel = NULL ;
	ClChannelManager	*poChannelManager = NULL ;
	ClLogonAdaptor		*poLogonAdaptor = NULL ;
	ClAccessRecordSet	*poRecordSet = NULL ;
	ClNode				*poNode = NULL ;
	SAFEARRAYBOUND		rgsabound[ 2 ] ;
	UINT				*piValues = NULL ;


	*ao_piReturn = M4_ERROR ;
	*ao_plSession =  0;
	*ao_plProperties =  0;

	if( *ao_alTimes != NULL )
	{
		SafeArrayDestroy( *ao_alTimes ) ;
		*ao_alTimes = NULL ;
	}

	poChannel = m_poAccess->GetpChannel() ;

	if( poChannel != NULL )
	{
		// Propiedades de sesión
		poChannelManager = poChannel->GetpChannelManager() ;

		if( poChannelManager != NULL )
		{
			poSessionChannel = poChannelManager->GetSessionChannel() ;

			if( poSessionChannel != NULL )
			{
				poLogonAdaptor = poChannelManager->GetLogonAdaptor() ;

				if( poLogonAdaptor != NULL )
				{
					poRecordSet = poLogonAdaptor->GetSessionRecordSet() ;

					if( poRecordSet != NULL )
					{
						iTime = poRecordSet->GetNodeTimeStamp() ;
						*ao_plSession = UINT( iTime ) ;
					}
				}
			}
		}


		// Propiedades de canal
		iTime = poChannel->Property.GetTimeStamp() ;
		iAuxTime = m_poAccess->Property.GetTimeStamp() ;

		if( iAuxTime > iTime )
		{
			iTime = iAuxTime ;
		}

		*ao_plProperties = UINT( iTime ) ;


		// Nodos
		iNodes = m_poAccess->Node.Count() ;

		rgsabound[ 0 ].lLbound = 0 ;
		rgsabound[ 0 ].cElements = iNodes ;
		rgsabound[ 1 ].lLbound = 0 ;
		rgsabound[ 1 ].cElements = 4 ;

		*ao_alTimes = SafeArrayCreate( VT_UI4, 2, rgsabound ) ;

		if( *ao_alTimes == NULL )
		{
			FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot create SafeArray" ) ;
			LEAVE_METHOD( CWrapAccess::ReadTimeStamps, S_OK ) ;
		}

		hResult = SafeArrayAccessData( *ao_alTimes, (void**) &piValues ) ;

		if( hResult != S_OK )
		{
			FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot access SafeArray" ) ;
			SafeArrayDestroy( *ao_alTimes ) ;
			*ao_alTimes = NULL ;
			LEAVE_METHOD( CWrapAccess::ReadTimeStamps, S_OK ) ;
		}

		memset( piValues, 0, iNodes * 4 * sizeof( UINT ) ) ;

	    for( i = 0 ; i < iNodes ; i++ )
		{
			poNode = m_poAccess->Node.Get( ClNodeIndex( i ) ) ;

			if( poNode != NULL )
			{
				iTime = poNode->RecordSet.GetNodeTimeStamp() ;
				piValues[ iNodes * 0 + i ] = UINT( iTime ) ;

				iTime = poNode->RecordSet.GetBlockTimeStamp() ;
				piValues[ iNodes * 1 + i ] = UINT( iTime ) ;
	
				iTime = poNode->RecordSet.GetRecordTimeStamp() ;
				piValues[ iNodes * 2 + i ] = UINT( iTime ) ;

				iTime = poNode->RecordSet.GetCurrentTimeStamp() ;
				piValues[ iNodes * 3 + i ] = UINT( iTime ) ;
			}
		}

		hResult = SafeArrayUnaccessData( *ao_alTimes ) ;

		if( hResult != S_OK )
		{
			FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot unaccess SafeArray" ) ;
			SafeArrayDestroy( *ao_alTimes ) ;
			*ao_alTimes = NULL ;
			LEAVE_METHOD( CWrapCVM::ReadTimeStamps, S_OK ) ;
		}
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD(CWrapAccess::ReadTimeStamps, S_OK);
}


