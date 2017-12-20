
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapbookmark.h"
#include "wrapdef.h"
#include "wrapaccess.h"
#include "wrapchannel.h"
#include "channel.hpp"
#include "gstring.h"
#include "m4mdrt.hpp"
#include "wores.hpp"


/////////////////////////////////////////////////////////////////////////////
// CWrapBookMark


STDMETHODIMP CWrapBookMark::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapBookMark };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

void CWrapBookMark::GetFrom (ClAccess &ai_poAccess)
{
	m_BookMark.GetFrom(ai_poAccess);
}

void CWrapBookMark::GetFrom_PK (ClAccess &ai_poAccess)
{
	m_BookMark.GetFrom_PK (ai_poAccess);
}

STDMETHODIMP CWrapBookMark::GetFrom (IWrapAccess* ai_piAccess, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapBookMark::GetFrom);

	if( ai_piAccess == NULL )
	{
		DUMP_CHLOG_ERRORF( M4_CH_WO_NULL_ARGUMENT, "GetFrom" << "ai_piAccess" ) ;
		*ao_sRetVal = M4_ERROR ;
		LEAVE_METHOD(CWrapBookMark::GetFrom, S_OK);
	}

    ClAccess* pAccess;
	((CWrapAccess*)ai_piAccess)->Unwrap(&pAccess);

	TRACE2("Access pointer has address %p",	pAccess);

	m_BookMark.GetFrom (*pAccess);

	LEAVE_METHOD(CWrapBookMark::GetFrom, S_OK);
}

STDMETHODIMP CWrapBookMark::GetFrom_PK (IWrapAccess* ai_piAccess, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapBookMark::GetFrom_PK);

	if( ai_piAccess == NULL )
	{
		DUMP_CHLOG_ERRORF( M4_CH_WO_NULL_ARGUMENT, "GetFrom_PK" << "ai_piAccess" ) ;
		*ao_sRetVal = M4_ERROR ;
		LEAVE_METHOD(CWrapBookMark::GetFrom_PK, S_OK);
	}

    ClAccess* pAccess;
	((CWrapAccess*)ai_piAccess)->Unwrap(&pAccess);

	TRACE2("Access pointer has address %p",	pAccess);

	m_BookMark.GetFrom_PK (*pAccess);

	LEAVE_METHOD(CWrapBookMark::GetFrom_PK, S_OK);
}

STDMETHODIMP CWrapBookMark::ApplyTo(IWrapAccess* ai_piAccess, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapBookMark::ApplyTo);

	if( ai_piAccess == NULL )
	{
		DUMP_CHLOG_ERRORF( M4_CH_WO_NULL_ARGUMENT, "ApplyTo" << "ai_piAccess" ) ;
		*ao_sRetVal = M4_ERROR ;
		LEAVE_METHOD(CWrapBookMark::ApplyTo, S_OK);
	}

    ClAccess* pAccess;
	((CWrapAccess*)ai_piAccess)->Unwrap(&pAccess);

	TRACE2("Access pointer has address %p", pAccess);

	m_BookMark.ApplyTo (*pAccess);

	LEAVE_METHOD(CWrapBookMark::ApplyTo, S_OK);
}

STDMETHODIMP CWrapBookMark::ApplyToBranch(IWrapAccess* ai_piAccess, BSTR ai_sCutNode, BSTR * ai_sLast, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapBookMark::ApplyToBranch);

	if( ai_piAccess == NULL )
	{
		DUMP_CHLOG_ERRORF( M4_CH_WO_NULL_ARGUMENT, "ApplyToBranch" << "ai_piAccess" ) ;
		*ao_sRetVal = M4_ERROR ;
		LEAVE_METHOD(CWrapBookMark::ApplyToBranch, S_OK);
	}

    ClAccess* pAccess;
	((CWrapAccess*)ai_piAccess)->Unwrap(&pAccess);

	gstring tmpCutNode(ai_sCutNode);
	TRACE2("Access pointer has address %p", pAccess);

	m4pcchar_t pLastNode;

	* ao_sRetVal = m_BookMark.ApplyToBranch (*pAccess, tmpCutNode, pLastNode);

    if (pLastNode)
    {
        m4VariantType v;
        v.SetString((m4pchar_t)pLastNode);
	    gstring tmpGstring (v.Data.PointerChar);
    	*ai_sLast = (BSTR)tmpGstring;
    }

	LEAVE_METHOD(CWrapBookMark::ApplyToBranch, S_OK);
}

STDMETHODIMP CWrapBookMark::SerializeBranch(/*[in]*/IWrapChannel* ai_piChannel, /*[in]*/IWrapAccess* ai_piAccess, /*[in]*/BSTR ai_sNodeId, /*[out]*/BSTR *ao_sSerialized, /*[out, retval]*/short* ao_sRetVal)
{
	ENTER_METHOD(CWrapBookMark::SerializeBranch);

	if( ai_piChannel == NULL )
	{
		DUMP_CHLOG_ERRORF( M4_CH_WO_NULL_ARGUMENT, "SerializeBranch" << "ai_piChannel" ) ;
		*ao_sRetVal = M4_ERROR ;
		LEAVE_METHOD(CWrapBookMark::SerializeBranch, S_OK);
	}

    ClChannel* pChannel;
	((CWrapChannel*)ai_piChannel)->Unwrap(&pChannel);

	TRACE2("Channel pointer has address %p", pChannel);
	
    ClAccess* poAccess;
	if (ai_piAccess)
		((CWrapAccess*)ai_piAccess)->Unwrap(&poAccess);
	else
		poAccess = NULL;

	TRACE2("Access pointer has address %p",	poAccess);

	//Funcionalidad
	*ao_sRetVal = S_FALSE;
	gstring tmpGstring(ai_sNodeId);
	if (tmpGstring) {
		if ( pChannel && pChannel->GetpCMCR() ) {
			m4uint32_t hNode = pChannel->GetpCMCR()->GetChannelNodeHandleById( m4pchar_t(tmpGstring) );
			m4char_t	*pcBuffer=new m4char_t[10240];
			m4uint32_t  iLongBuffer=10240;
			m4return_t	ret=m_BookMark.SerializeBranch(pChannel, poAccess, hNode, pcBuffer, &iLongBuffer);
			
			//Si no hay suficiente buffer, reservamos mas y volvemos a llamar
			while (ret==M4_WARNING) {
				if (pcBuffer)
					delete [] pcBuffer;
				pcBuffer = new m4char_t[iLongBuffer];
				ret=m_BookMark.SerializeBranch(pChannel, poAccess, hNode, pcBuffer, &iLongBuffer);
			}

			if (ret==M4_SUCCESS) {
				gstring tmpBuffer(pcBuffer);
				*ao_sSerialized = (BSTR)tmpBuffer;
				if (ao_sSerialized) {
					*ao_sRetVal = S_OK;
				}
			}
			if (pcBuffer)
				delete [] pcBuffer;
		}
	}

	LEAVE_METHOD(CWrapBookMark::SerializeBranch, *ao_sRetVal);
}

STDMETHODIMP CWrapBookMark::DeserializeBranch(/*[in]*/IWrapChannel* ai_piChannel, /*[in]*/BSTR ai_sSerialized, /*[out, retval]*/short* ao_sRetVal)
{
	ENTER_METHOD(CWrapBookMark::DeserializeBranch);

	if( ai_piChannel == NULL )
	{
		DUMP_CHLOG_ERRORF( M4_CH_WO_NULL_ARGUMENT, "DeserializeBranch" << "ai_piChannel" ) ;
		*ao_sRetVal = M4_ERROR ;
		LEAVE_METHOD(CWrapBookMark::DeserializeBranch, S_OK);
	}

    ClChannel* pChannel;
	((CWrapChannel*)ai_piChannel)->Unwrap(&pChannel);

	TRACE2("Channel pointer has address %p", pChannel);

	//Funcionalidad
	gstring tmpGstring(ai_sSerialized);
	if ( (tmpGstring == M4_TRUE) && (m_BookMark.DeserializeBranch(pChannel, m4pchar_t(tmpGstring))==M4_SUCCESS) ) 
		*ao_sRetVal = S_OK;
	else
		*ao_sRetVal = S_FALSE;

	LEAVE_METHOD(CWrapBookMark::DeserializeBranch, *ao_sRetVal);
}
