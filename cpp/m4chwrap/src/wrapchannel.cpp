//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapchannel.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapChannel
//
//==============================================================================

#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapchannel.h"
#include "wrapchandef.h"
#include "wrapcolprop.h"
#include "wrapaccess.h"
#include "wrapoper.h"
#include "wrapchandef.h"
#include "m4types.hpp"
#include "newinterface.h"
#include "channel.hpp"
#include "chan_knl.hpp"
#include "cvdate.h"
#include "wrapdef.h"
#include "cldates.hpp"
#include "gstring.h"
#include "access.hpp"
#include "rerror.h"
#include "clfileiod.hpp"  // for serialization
#include "genproxy.hpp"
#include "sttchn.hpp"
#include "m4unicode.hpp"
#include "cllstr.hpp"
#include "m4mdfac.hpp"
#include "wrapbookmark.h"
#include "replaceliterals.hpp"
#include "dump_vst.hpp"
#include "m4stl.hpp"


/////////////////////////////////////////////////////////////////////////////
// CWrapChannel

STDMETHODIMP CWrapChannel::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapChannel };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

CWrapChannel::CWrapChannel() 
	: m_poChannel(0)
	, m_bOwner(false)
{
}

CWrapChannel::~CWrapChannel()
{
	// If a channel has one or more accesses we dont really want to destroy it,
	// but if it has none, we want to call the destructor 
	if (m_poChannel) {
		if (m_bOwner == true) {
			m_poChannel->Destroy();
		}
		m_poChannel = 0;
	}	
}

void CWrapChannel::Unwrap(ClChannel** ao_VarRef)
{
	*ao_VarRef = m_poChannel;
}

void CWrapChannel::Init(ClChannel* ai_VarRef, const bool ai_bOwner /*= false*/)
{
	m_poChannel = ai_VarRef;

	/* Corrige el bug 0051757.
	 * Si le canal es de depuración dejamos que VB tenga el control sobre él.
	 */
	const ClChannel::eDebugMode_t eDebugMode = m_poChannel->GetDebugMode();
	if (eDebugMode == ClChannel::IS_DEBUG) {
		m_bOwner = true;
	} else {
		m_bOwner = ai_bOwner;
	}

	TRACE2("Assigning address of channel %p", m_poChannel);
}


STDMETHODIMP CWrapChannel::BuildFromfile(BSTR ai_bstrPath, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::BuildFromfile);

	gstring tmpGstring(ai_bstrPath);

	if (tmpGstring)
	{
		TRACE2("Trying to open file %s", m4pchar_t(tmpGstring));

		*ao_sRetVal = m_poChannel->BuildFromFile(tmpGstring);

		LEAVE_METHOD(CWrapChannel::BuildFromfile, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
		LEAVE_METHOD(CWrapChannel::BuildFromfile, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChannel::CreateAccess(IWrapAccess** ao_poAccess)
{
	ENTER_METHOD(CWrapChannel::CreateAccess);

	ClAccess* poAccess = m_poChannel->CreateAccess();

	if (poAccess)
	{
		if (NewInterface(CLSID_WrapAccess, IID_IWrapAccess,	(IUnknown**)ao_poAccess) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapChannel::CreateAccess, E_UNEXPECTED);
		}
		else
		{
			((CWrapAccess*)(*ao_poAccess))->Init(poAccess);
			LEAVE_METHOD(CWrapChannel::CreateAccess, S_OK);
		}
	}
	else
	{
		// Cannot create access
		FormatError(M4_CH_WO_CANT_CREATE_ACCESS, "Unable to create new access object");
		LEAVE_METHOD(CWrapChannel::CreateAccess, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChannel::get_coProps(IWrapColProp** pVal)
{
	ENTER_METHOD(CWrapChannel::CreateAccess);

	if (NewInterface(CLSID_WrapColProp,	IID_IWrapColProp, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapChannel::CreateAccess, E_UNEXPECTED);
	}
	else
	{
		((CWrapColProp*)(*pVal))->Init(&m_poChannel->Property);
		LEAVE_METHOD(CWrapChannel::CreateAccess, S_OK);
	}
}

STDMETHODIMP CWrapChannel::BuildAsMetachannel(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::BuildAsMetachannel);

	*ao_sRetVal = m_poChannel->BuildAsMetachannel();

	LEAVE_METHOD(CWrapChannel::BuildAsMetachannel, S_OK);
}

STDMETHODIMP CWrapChannel::BuildFromId(BSTR ai_stId, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::BuildFromId);

	gstring tmpGstring(ai_stId);

	if (tmpGstring)
	{
		ATLTRACE("Trying to build channel from id <%s>\n", m4pchar_t(tmpGstring));

		*ao_sRetVal = m_poChannel->BuildFromId(tmpGstring);

   		ATLTRACE("Building channel from id <%s> Retval =<%d>\n", m4pchar_t(tmpGstring), *ao_sRetVal);
		LEAVE_METHOD(CWrapChannel::BuildFromId, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
		LEAVE_METHOD(CWrapChannel::BuildFromId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChannel::BuildFromAccess(IWrapAccess* ai_piAccess, short* ao_sRetVal)
{
	ClAccess* pAccess;

	ENTER_METHOD(CWrapChannel::BuildFromAccess);

	((CWrapAccess*)ai_piAccess)->Unwrap(&pAccess);

	TRACE2("Access pointer has address %p", pAccess);

	*ao_sRetVal = m_poChannel->BuildFromAccess(pAccess);

	LEAVE_METHOD(CWrapChannel::BuildFromAccess, S_OK);
}

STDMETHODIMP CWrapChannel::get_Operation(IWrapOper** pVal)
{
	ENTER_METHOD(CWrapChannel::get_Operation);

	if (NewInterface(CLSID_WrapOper, IID_IWrapOper, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapChannel::get_Operation, E_UNEXPECTED);
	}
	else
	{
		((CWrapOper*)(*pVal))->Init(&m_poChannel->GetpChannel_Data ()->Operation);
		LEAVE_METHOD(CWrapChannel::get_Operation, S_OK);
	}
}

STDMETHODIMP CWrapChannel::get_StartDate(DATE* pVal)
{
	ENTER_METHOD(CWrapChannel::get_StartDate);

	m4VariantType tmpVar;

	m_poChannel->StartDate.Get(tmpVar);

	*pVal = M4dateToDate(tmpVar);

	TRACE3("Returning m4date %g to date %g", tmpVar, *pVal);
	LEAVE_METHOD(CWrapChannel::get_StartDate, S_OK);
}

STDMETHODIMP CWrapChannel::put_StartDate(DATE newVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapChannel::put_StartDate);

	tmpVar.Type = M4CL_CPP_TYPE_DATE;
	tmpVar.Data.DoubleData = DateToM4date(newVal);

	TRACE3("Converting date %g to m4date %g", newVal, tmpVar.Data.DoubleData);

	m_poChannel->StartDate.Set(tmpVar);

	LEAVE_METHOD(CWrapChannel::put_StartDate, S_OK);
}

STDMETHODIMP CWrapChannel::get_EndDate(DATE* pVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapChannel::get_EndDate);

	m_poChannel->EndDate.Get(tmpVar);

	*pVal = M4dateToDate(tmpVar);

	TRACE3("Returning m4date %g to date %g", tmpVar, *pVal);
	LEAVE_METHOD(CWrapChannel::get_EndDate, S_OK);
}

STDMETHODIMP CWrapChannel::put_EndDate(DATE newVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapChannel::put_EndDate);

	tmpVar.Type = M4CL_CPP_TYPE_DATE;
	tmpVar.Data.DoubleData = DateToM4date(newVal);

	TRACE3("Converting date %g to m4date %g", newVal, tmpVar.Data.DoubleData);

	m_poChannel->EndDate.Set(tmpVar);

	LEAVE_METHOD(CWrapChannel::put_EndDate, S_OK);
}

STDMETHODIMP CWrapChannel::get_UnitDate(DATE* pVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapChannel::get_UnitDate);

	m_poChannel->UnitDate.Get(tmpVar);

	*pVal = M4dateToDate(tmpVar);

	TRACE3("Returning m4date %g to date %g", tmpVar, *pVal);
	LEAVE_METHOD(CWrapChannel::get_UnitDate, S_OK);
}

STDMETHODIMP CWrapChannel::put_UnitDate(DATE newVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapChannel::put_UnitDate);

	tmpVar.Type = M4CL_CPP_TYPE_DATE;
	tmpVar.Data.DoubleData = DateToM4date(newVal);

	TRACE3("Converting date %g to m4date %g", newVal, tmpVar.Data.DoubleData);

	m_poChannel->UnitDate.Set(tmpVar);

	LEAVE_METHOD(CWrapChannel::put_UnitDate, S_OK);
}

STDMETHODIMP CWrapChannel::get_ChanDef(IWrapChanDef** pVal)
{
	ENTER_METHOD(CWrapChannel::get_ChanDef);

	if (NewInterface(CLSID_WrapChanDef, IID_IWrapChanDef, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapChannel::get_ChanDef, E_UNEXPECTED);
	}

	CWrapChanDef* pObj = (CWrapChanDef*)(*pVal);
		
	pObj->Init(m_poChannel->GetpChannelDef ());

	LEAVE_METHOD(CWrapChanDef::get_ChanDef, S_OK);
}


//EDU:
STDMETHODIMP CWrapChannel::get_AutoLoad(BYTE* ao_pVal)
{
	ENTER_METHOD(CWrapChannel::get_AutoLoad);

	m_poChannel->WantsAutoLoad.Get((m4uint8_t &)*ao_pVal);

    /*
	if (tmpVar)
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}
    */

	LEAVE_METHOD(CWrapChannel::get_AutoLoad, S_OK);
}

STDMETHODIMP CWrapChannel::put_AutoLoad(BYTE ai_newVal)
{
	m4uint8_t tmpVar;

	ENTER_METHOD(CWrapChannel::put_AutoLoad);

	//tmpVar.Type = M4CL_CPP_TYPE_NUMBER;
	//tmpVar.Data.DoubleData = (ai_newVal == VARIANT_TRUE);

    //intento minimizar problemas del cambio de tipo de autoload de Boolean
    //a Byte
    switch (ai_newVal) {
        case M4CL_AUTOLOAD_OFF:
        case M4CL_AUTOLOAD_BLOCK:
        case M4CL_AUTOLOAD_PRG:
        case M4CL_AUTOLOAD_NODESAYS:
            tmpVar = ai_newVal;
            break;

        default:
            //esto es que me viene un -1, que es un True de VB
            //deduzco quieren activar la autoload 'clásica':
            tmpVar = M4CL_AUTOLOAD_BLOCK;
            break;
    }

    TRACE2("Setting AutoLoad mode to %i", tmpVar);

	m_poChannel->WantsAutoLoad.Set(tmpVar);

	LEAVE_METHOD(CWrapChannel::put_AutoLoad, S_OK);
}

STDMETHODIMP CWrapChannel::get_ClosingDate(DATE* pVal)
{
	m4date_t tmpDate;

	ENTER_METHOD(CWrapChannel::get_ClosingDate);

	tmpDate = ClActualDate() ;
	*pVal = M4dateToDate(tmpDate);

	TRACE3("Returning m4date %g to date %g", tmpDate, *pVal);
	TRACE("Warning: The function CWrapChannel::get_ClosingDate is not supported.");
	LEAVE_METHOD(CWrapChannel::get_ClosingDate, S_OK);
}

STDMETHODIMP CWrapChannel::Serialize(BSTR ai_stFilename, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::Serialize);

	ClFileIODriver IOD;

	gstring tmpGstring(ai_stFilename);

	if (tmpGstring)
	{
		TRACE2("Trying to open file %s", m4pchar_t(tmpGstring));

		*ao_sRetVal = IOD.Init(GEN_IO_MODE_WRITE, tmpGstring);

		if (*ao_sRetVal == M4_SUCCESS)
		{
			*ao_sRetVal = m_poChannel->Serialize(IOD,NULL, 0);

			if (*ao_sRetVal != M4_SUCCESS)
			{
				FormatError(M4_CH_WO_CANT_SERIALIZE_CHANNEL, "Cannot serialize the channel");
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
		LEAVE_METHOD(CWrapChannel::Serialize, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChannel::Serialize, E_UNEXPECTED);
	}	
}

STDMETHODIMP CWrapChannel::DeSerialize(BSTR ai_stFilename, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::DeSerialize);

	ClFileIODriver IOD;

	gstring tmpGstring(ai_stFilename);

	if (tmpGstring)
	{
		TRACE2("Trying to open file %s", m4pchar_t(tmpGstring));

		*ao_sRetVal = IOD.Init(GEN_IO_MODE_READ, tmpGstring);

		if (*ao_sRetVal == M4_SUCCESS)
		{
			*ao_sRetVal = m_poChannel->DeSerialize(IOD);

			if (*ao_sRetVal != M4_SUCCESS)
			{
				FormatError(M4_CH_WO_CANT_DESERIALIZE_CHANNEL, "Cannot deserialize the channel");
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
		LEAVE_METHOD(CWrapChannel::DeSerialize, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChannel::DeSerialize, E_UNEXPECTED);
	}	
}

STDMETHODIMP CWrapChannel::get_Handle(long* pVal)
{
	ENTER_METHOD(CWrapChannel::get_Handle);

	*pVal = m_poChannel->GetHandle();
		
	TRACE2("Returning the value %d", *pVal);
	LEAVE_METHOD(CWrapChannel::get_Handle, S_OK);
}

//--------------------------------------->>
//EDU, 20/02/98 
// debug dump facility:

STDMETHODIMP CWrapChannel::DebugDump(BSTR ai_stFilename)
{
    ENTER_METHOD(CWrapChannel::DebugDump);

    gstring tmpGstring(ai_stFilename);

    ofstream osf;
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;

	M4Open( osf, tmpGstring, M4UniWrite, eEncoding );

    if (osf.is_open ()) {
        ClSimpleDump_Visitor pp (osf);

        pp.Visit_Channel (*m_poChannel);
    }

    osf.close ();

    LEAVE_METHOD(CWrapChannel::DebugDump, S_OK);
}

//---------------------------------------<<

STDMETHODIMP CWrapChannel::get_CsType(short * pVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapChannel::get_CsType);

	m_poChannel->CsType.Get(tmpVar);

	*pVal = short( tmpVar.Data.DoubleData ) ;

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChannel::get_CsType, S_OK);
}

STDMETHODIMP CWrapChannel::put_CsType(short newVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapChannel::put_CsType);

	tmpVar.Type = M4CL_CPP_TYPE_NUMBER;
	tmpVar.Data.DoubleData = newVal;

	m_poChannel->CsType.Set(tmpVar);

	LEAVE_METHOD(CWrapChannel::put_CsType, S_OK);
}

STDMETHODIMP CWrapChannel::Load(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::Load);

	*ao_sRetVal = m_poChannel->Load( NULL, M4_TRUE );

	LEAVE_METHOD(CWrapChannel::Load, S_OK);
}

STDMETHODIMP CWrapChannel::UnLoad(short * ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::UnLoad);

	*ao_sRetVal = m_poChannel->UnLoad();

	LEAVE_METHOD(CWrapChannel::UnLoad, S_OK);
}

STDMETHODIMP CWrapChannel::Persist(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::Persist);

	*ao_sRetVal = m_poChannel->Persist( NULL, M4_TRUE );

	LEAVE_METHOD(CWrapChannel::Persist, S_OK);
}

STDMETHODIMP CWrapChannel::LoadFromFile(BSTR ai_pstFileName, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::LoadFromFile);

	gstring tmpGstring(ai_pstFileName);

	if (tmpGstring)
	{
		// Apaño para cargar ficheros de staísticas
		m4pcchar_t	pccFileName = tmpGstring ;
		size_t	iLength = strlen( pccFileName ) ;

		if( iLength > 4
			&& strcmpi( pccFileName + iLength - 4, ".stt" ) == 0
			&& strcmpi( m_poChannel->GetpChannelDef()->Id(), M4_CHANNEL_STATISTICS ) == 0 )
		{
			*ao_sRetVal = m_poChannel->LoadFromSttFile(tmpGstring);
		}
		else
		{
			*ao_sRetVal = m_poChannel->LoadFromFile(tmpGstring);
		}

		LEAVE_METHOD(CWrapChannel::LoadFromFile, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
		LEAVE_METHOD(CWrapChannel::LoadFromFile, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChannel::PersistToFile(BSTR ai_pstFileName, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::PersistToFile);

	gstring tmpGstring(ai_pstFileName);

	if (tmpGstring)
	{
		*ao_sRetVal = m_poChannel->PersistToFile(tmpGstring, M4_FALSE);

		LEAVE_METHOD(CWrapChannel::PersistToFile, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
		LEAVE_METHOD(CWrapChannel::PersistToFile, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChannel::PersistToTempFile(BSTR * ao_pstFileName, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::PersistToTempFile);

	m4char_t aFileName[256];

	*ao_sRetVal = m_poChannel->PersistToTempFile(aFileName, 256);

	if (M4_SUCCESS == *ao_sRetVal)
	{
		gstring tmpGstring(aFileName);
	
		if (tmpGstring)
		{
			*ao_pstFileName = (BSTR)tmpGstring;

			LEAVE_METHOD(CWrapChannel::PersistToTempFile, S_OK);
		}
		else
		{
			FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
			LEAVE_METHOD(CWrapChannel::PersistToTempFile, E_UNEXPECTED);
		}
	}

	LEAVE_METHOD(CWrapChannel::PersistToTempFile, S_OK);
}

STDMETHODIMP CWrapChannel::Free(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::Free);

	*ao_sRetVal = m_poChannel->ReleaseAll();

	LEAVE_METHOD(CWrapChannel::Free, S_OK);
}

STDMETHODIMP CWrapChannel::get_DebugMode(VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CWrapChannel::get_DebugMode);

	if (m_poChannel->IsDebugEnabled())
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}
	LEAVE_METHOD(CWrapChannel::get_DebugMode,S_OK);
}

STDMETHODIMP CWrapChannel::put_DebugMode(VARIANT_BOOL newVal)
{
	ENTER_METHOD(CWrapChannel::put_DebugMode);

	m4return_t iRet = M4_SUCCESS;
	
	/* TODO: Alex. Hay que revisarlo y probarlo.
	if (newVal==VARIANT_TRUE)
	{
		iRet = m_poChannel->SetDebugEnabled();
	}
	else
	{
		m_poChannel->SetDebugMode(ClChannel::IS_NOT_DEBUG);
	}
	*/
	
	if (iRet == M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapChannel::put_DebugMode, S_OK);
	}
	else
	{
		RaiseError("Back channel views still open");
		LEAVE_METHOD(CWrapChannel::put_DebugMode, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChannel::GetDebugBackAccess(IWrapAccess** ao_poBackAccess)
{
	ENTER_METHOD(CWrapChannel::GetDebugBackAccess);

	if (!m_poChannel)
	{
		RaiseError("Invalid channel - no back view available");
		LEAVE_METHOD(CWrapChannel::GetDebugBackAccess, E_UNEXPECTED);
	}

	m4VariantType vCsType;
	m_poChannel->CsType.Get(vCsType);
	if (vCsType.Data.DoubleData == M4CL_CSTYPE_BACK)
	{
		RaiseError("This is a back channel - no back view available");
		LEAVE_METHOD(CWrapChannel::GetDebugBackAccess, E_UNEXPECTED);
	}

	ClChannel *poBackChannel = 0;
	//Bug:0064091
	//Hemos añadido el siguiente if para controlar el caso en que el inspector pida la parte
	// back de un canal que se está depurando con el stepper
	if(m_poChannel->GetDebugMode() != ClChannel::IS_DEBUG) {

		m4uint32_t iRemoteId = m_poChannel->GetRemoteId();
		if (!iRemoteId)
		{
			RaiseError("Channel not connected - no back view available");
			LEAVE_METHOD(CWrapChannel::GetDebugBackAccess, E_UNEXPECTED);
		}

		m4pcchar_t pcLocalId = m_poChannel->GetpChannelDef()->Id();

		ClChannelManager *poCM = m_poChannel->GetpChannelManager();
		ClClientProxyManager *poProxyManager = (ClClientProxyManager*)poCM->GetProxyManager();
		ClProxySpace *poProxySpace = poProxyManager->GetSpace("Proxy");
		poBackChannel = poProxySpace->GetProxyDebugView(pcLocalId, iRemoteId);
	}
	else {
		poBackChannel = m_poChannel;
	}

	if (poBackChannel == NULL)
	{
		RaiseError("Channel not connected - no back view available");
		LEAVE_METHOD(CWrapChannel::GetDebugBackAccess, E_UNEXPECTED);
	}

	ClAccess* poBackAccess = NULL;
	m4return_t iResult = poBackChannel->CreateAccess(poBackAccess);
	if (iResult != M4_SUCCESS)
	{
		RaiseError("Back access not available");
		LEAVE_METHOD(CWrapChannel::GetDebugBackAccess, E_UNEXPECTED);
	}

	if (NewInterface(CLSID_WrapAccess, IID_IWrapAccess, (IUnknown**)ao_poBackAccess) != M4_SUCCESS)
	{
		RaiseError("Error obtaining Access");
		LEAVE_METHOD(CWrapChannel::GetDebugBackAccess, E_UNEXPECTED);
	}

	((CWrapAccess*)(*ao_poBackAccess))->Init(poBackAccess, M4_TRUE);

	LEAVE_METHOD(CWrapChannel::GetDebugBackAccess, S_OK);
}

STDMETHODIMP CWrapChannel::Commit (short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::Commit);

	*ao_sRetVal = m_poChannel->GetpChannel_Data ()->CheckPoint ();

	LEAVE_METHOD(CWrapChannel::Commit, S_OK);
}

STDMETHODIMP CWrapChannel::Rollback (short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::Rollback);

	*ao_sRetVal = m_poChannel->GetpChannel_Data ()->Undo ();

	LEAVE_METHOD(CWrapChannel::Rollback, S_OK);
}

STDMETHODIMP CWrapChannel::CheckPoint(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::CheckPoint);

	*ao_sRetVal = m_poChannel->GetpChannel_Data ()->CheckPoint ();

	LEAVE_METHOD(CWrapChannel::CheckPoint, S_OK);
}

STDMETHODIMP CWrapChannel::DebugMemUsage(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::DebugMemUsage);
#if 0
    m4pchar_t buffer = new m4char_t [256];

#ifdef _VERIF_MEMORY
    StTotalMemInfo TMI;
    s_MemoryInfo (m_poChannel, TMI);

    sprintf (buffer, 
"\
Nodes count: \t%i\n\
Blocks count: \t%i\n\
Regs count: \t%i\n\
Data memory: \t%i\n\
Used memory: \t%i\n\
BStore size: \t%i\
", TMI.nNodes, TMI.nBlocks, TMI.nRegisters, TMI.DataMemory, TMI.UsedMemory, TMI.BlockStoreSize);

    ofstream ofs;
	// UNICODE FILE
	M4Open( osf, "c:\\temp\\mem_xxx.txt", M4Append );
    ofs << TMI.nNodes << '\t';
    ofs << TMI.nBlocks << '\t';
    ofs << TMI.nRegisters << '\t';
    ofs << TMI.DataMemory << '\t';
    ofs << TMI.UsedMemory << '\t';
    ofs << TMI.BlockStoreSize << "\t\n";
    ofs.close ();

#else
    sprintf (buffer, "Memory info not available");
#endif

    MessageBox (NULL, buffer, "Channel Memory Usage", MB_OK);

    delete []buffer;
#endif
	*ao_sRetVal = M4_SUCCESS;

	LEAVE_METHOD(CWrapChannel::DebugMemUsage, S_OK);
}

STDMETHODIMP CWrapChannel::DebugAccessCount(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::DebugAccessCount);

    m4pchar_t buffer = new m4char_t [256];

    sprintf (buffer, "Access count: %i", m_poChannel->Access.Count ());
	
#ifdef _DEBUG
    MessageBox (NULL, buffer, "WrapChannel", MB_OK);
#endif

    delete []buffer;

	*ao_sRetVal = M4_SUCCESS;

	LEAVE_METHOD(CWrapChannel::DebugAccessCount, S_OK);
}

STDMETHODIMP CWrapChannel::CheckModify(VARIANT_BOOL * ao_pbModified)
{
	ENTER_METHOD(CWrapChannel::CheckModify);

	m4return_t	iResult ;
	m4bool_t	bModified ;


	iResult = m_poChannel->CheckModify( bModified ) ;

	if( iResult != M4_SUCCESS )
	{
		*ao_pbModified = VARIANT_FALSE ;
	}
	else if( bModified == M4_TRUE ) 
	{
		*ao_pbModified = VARIANT_TRUE ;
	}
	else
	{
		*ao_pbModified = VARIANT_FALSE ;
	}

	LEAVE_METHOD(CWrapChannel::CheckModify, S_OK);
}

STDMETHODIMP CWrapChannel::RefreshFlags(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::RefreshFlags);

    m_poChannel->GetpChannel_Data()->RefreshFlags();
	
	*ao_sRetVal = M4_SUCCESS;

	LEAVE_METHOD(CWrapChannel::RefreshFlags, S_OK);
}

STDMETHODIMP CWrapChannel::AddPartner(IWrapChannel* ai_piChannel, short* ao_sRetVal)
{
    ENTER_METHOD(CWrapChannel::AddPartner);

	ClChannel* pChannel;

	((CWrapChannel*)ai_piChannel)->Unwrap(&pChannel);

	*ao_sRetVal = m_poChannel->m_oGroupInfo.AddPartner(pChannel);

	LEAVE_METHOD(CWrapChannel::AddPartner, S_OK);
}

STDMETHODIMP CWrapChannel::DelPartner(IWrapChannel* ai_piChannel)
{
    ENTER_METHOD(CWrapChannel::DelPartner);

	ClChannel* pChannel;

	((CWrapChannel*)ai_piChannel)->Unwrap(&pChannel);

	m_poChannel->m_oGroupInfo.DelPartner(pChannel);

	LEAVE_METHOD(CWrapChannel::DelPartner, S_OK);
}

STDMETHODIMP CWrapChannel::DelAllPartners(void)
{
    ENTER_METHOD(CWrapChannel::DelAllPartners);

	m_poChannel->m_oGroupInfo.DelAllPartners();

	LEAVE_METHOD(CWrapChannel::DelAllPartners, S_OK);
}


STDMETHODIMP CWrapChannel::GetAccessBookmarkByHandle(/*[in]*/ long ai_iPosition, /*[out, retval]*/ IWrapBookMark** ao_bookmark)
{
	ENTER_METHOD(CWrapChannel::GetAccessBookmarkByHandle);

	ClAccess* pAccess=0;
	m4return_t ret;
	m4bool_t   bFound=M4_FALSE;
	m4uint32_t i;
	
	for (i=0; i<m_poChannel->Access.Count();++i){
		pAccess=m_poChannel->Access[i];
		if (pAccess->GetHandle() == ai_iPosition){
			bFound=M4_TRUE;
			break;
		}
	}

	if (bFound){

#ifdef M4_NEWINTERFACE
		ret = NewInterface(CLSID_WrapBookMark, IID_IWrapBookMark,(IUnknown**)ao_bookmark);
#else
		HRESULT hRes = M4_CREATE_INTERFACE(CWrapBookMark,ao_bookmark);
		ret = SUCCEEDED(hRes)? M4_SUCCESS : M4_ERROR;
#endif		
		if (ret != M4_SUCCESS){
			LEAVE_METHOD(CWrapChannel::GetAccessBookmarkByHandle, E_UNEXPECTED);
		}

		((CWrapBookMark*)(*ao_bookmark))->GetFrom(*pAccess);
	}
	else{
		FormatError(M4_CH_WO_CANT_CREATE_ACCESS,"Could not get access");
		LEAVE_METHOD(CWrapChannel::GetAccessBookmarkByHandle, E_UNEXPECTED);
	}

	LEAVE_METHOD(CWrapChannel::GetAccessBookmarkByHandle, S_OK);
}

STDMETHODIMP CWrapChannel::PersistToFileAdvanced(BSTR ai_pstFileName, VARIANT_BOOL ai_bSerializeMCR, short *ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::PersistToFileAdvanced);

	gstring tmpGstring(ai_pstFileName);

	if (tmpGstring)
	{
	    m4bool_t bSerializeMCR = (ai_bSerializeMCR == VARIANT_TRUE) ? M4_TRUE : M4_FALSE;
		*ao_sRetVal = m_poChannel->PersistToFile(tmpGstring, bSerializeMCR);

		LEAVE_METHOD(CWrapChannel::PersistToFileAdvanced, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
		LEAVE_METHOD(CWrapChannel::PersistToFileAdvanced, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChannel::LoadFromSttFile(BSTR ai_pstFileName, short *ao_sRetVal)
{
	ENTER_METHOD(CWrapChannel::LoadFromSttFile);

	gstring tmpGstring(ai_pstFileName);

	if (tmpGstring)
	{
		*ao_sRetVal = m_poChannel->LoadFromSttFile(tmpGstring);

		LEAVE_METHOD(CWrapChannel::LoadFromSttFile, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CONVERT_STRING, "Unable to convert string");
		LEAVE_METHOD(CWrapChannel::LoadFromSttFile, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChannel::BuildXML(long ai_lMask, DATE* aio_dtDate, SAFEARRAY** ao_psaXML, short *ao_iRetVal)
{
	ENTER_METHOD( CWrapChannel::BuildXML ) ;

	HRESULT				hResult = S_OK ;
	m4return_t			iResult = M4_ERROR ;
	m4uint32_t			iMask = 0 ;
	m4uint32_t			iLength = 0 ;
	m4date_t			dOldVersion = 0 ;
	m4date_t			dNewVersion = 0 ;
	m4date_t			dLiteralsVersion = 0 ;
	m4pchar_t			pcOrganization = NULL ;
	m4pcchar_t			pcXML = NULL ;
	ClLongString		oXML ;
	ClChannelManager	*poCM = NULL ;
	ClCMCRFactory		*poFactory = NULL ;
	ClCompiledMCR		*poCMCR = NULL ;
	void				*pvData = NULL ;

	
	*ao_iRetVal = M4_ERROR ;

	if( *ao_psaXML != NULL )
	{
		SafeArrayDestroy( *ao_psaXML ) ;
		*ao_psaXML = NULL ;
	}

	poCMCR = m_poChannel->GetpCMCR() ;
	m_poChannel->Organization.Get( pcOrganization ) ;
	poCM = m_poChannel->GetpChannelManager() ;

	dOldVersion = DateToM4date( *aio_dtDate ) ;
	dNewVersion = poCMCR->GetChannelVersion() ;

	iResult = M4LiteralsVersion( poCM, pcOrganization, dLiteralsVersion ) ;

	if( iResult == M4_SUCCESS )
	{
		if( dNewVersion < dLiteralsVersion )
		{
			dNewVersion = dLiteralsVersion ;
		}
	}

	if( fabs( dOldVersion - dNewVersion ) <= M4_LN4_DATE_PRECISION )
	{
		// Si ya se tiene la fecha correcta no se manda otra vez
		*ao_iRetVal = M4_SUCCESS ;
		LEAVE_METHOD( CWrapChannel::BuildXML, S_OK ) ;
	}

	*aio_dtDate = M4dateToDate( dNewVersion ) ;

	iMask = m4uint32_t( ai_lMask ) ;
	oXML.Init( 200000, 100000 ) ;

	poFactory = poCM->GetpFactory() ;

	iResult = poFactory->MDFac_BuildXML( poCMCR, pcOrganization, iMask, oXML, M4_TRUE ) ;

	if( iResult != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::BuildXML, S_OK ) ;
	}

	pcXML = oXML.GetString() ;
	iLength = oXML.GetLength() ;

	if( pcXML == NULL || iLength == 0 )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "XML is empty" ) ;
		LEAVE_METHOD( CWrapCVM::BuildXML, S_OK ) ;
	}

	*ao_psaXML = SafeArrayCreateVector( VT_UI1, 0, iLength ) ;

	if( *ao_psaXML == NULL )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot create SafeArray" ) ;
		LEAVE_METHOD( CWrapCVM::BuildXML, S_OK ) ;
	}

	hResult = SafeArrayAccessData( *ao_psaXML, &pvData ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot access SafeArray" ) ;
		SafeArrayDestroy( *ao_psaXML ) ;
		*ao_psaXML = NULL ;
		LEAVE_METHOD( CWrapCVM::BuildXML, S_OK ) ;
	}

	memcpy( pvData, pcXML, iLength ) ;

	hResult = SafeArrayUnaccessData( *ao_psaXML ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot unaccess SafeArray" ) ;
		SafeArrayDestroy( *ao_psaXML ) ;
		*ao_psaXML = NULL ;
		LEAVE_METHOD( CWrapCVM::BuildXML, S_OK ) ;
	}

	*ao_iRetVal = M4_SUCCESS ;

	LEAVE_METHOD( CWrapChannel::BuildXML, S_OK ) ;
}

