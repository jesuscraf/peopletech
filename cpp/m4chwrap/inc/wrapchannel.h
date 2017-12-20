
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapchannel.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapChannel
//
//==============================================================================
#include "resource.h"       // main symbols

#ifndef __WRAPCHANNEL_H_
#define __WRAPCHANNEL_H_

class ClChannel;

/////////////////////////////////////////////////////////////////////////////
// CWrapChannel
class ATL_NO_VTABLE CWrapChannel : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapChannel, &CLSID_WrapChannel>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapChannel, &IID_IWrapChannel, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapChannel();
	~CWrapChannel();
	
	void Init(ClChannel* ai_VarRef, const bool ai_bOwner = false);
	void Unwrap(ClChannel** ao_VarRef);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPCHANNEL)

BEGIN_COM_MAP(CWrapChannel)
	COM_INTERFACE_ENTRY(IWrapChannel)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// IWrapChannel
// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

public:
	STDMETHOD(BuildXML)(/*[in]*/ long ai_lMask, /*[in, out]*/ DATE* aio_dtDate, /*[out]*/ SAFEARRAY** ao_psaXML, /*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(LoadFromSttFile)(/*[in]*/ BSTR ai_pstFileName, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(PersistToFileAdvanced)(/*[in]*/ BSTR ai_pstFileName, /*[in]*/ VARIANT_BOOL ai_bSerializeMCR, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(get_CsType)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_CsType)(/*[in]*/ short newVal);
	STDMETHOD(get_Handle)(/*[out, retval]*/ long *pVal);
	STDMETHOD(DeSerialize)(/*[in]*/ BSTR ai_stFileName, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(Serialize)(/*[in]*/ BSTR ai_stFileName, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(get_ChanDef)(/*[out, retval]*/ IWrapChanDef* *pVal);
	STDMETHOD(get_UnitDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_UnitDate)(/*[in]*/ DATE newVal);
	STDMETHOD(get_EndDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_EndDate)(/*[in]*/ DATE newVal);
	STDMETHOD(get_StartDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_StartDate)(/*[in]*/ DATE newVal);
	STDMETHOD(CheckPoint)(/*[out, retval]*/ short* ao_sRetVal);
    STDMETHOD(Commit)(/*[out, retval]*/ short* ao_sRetVal);
    STDMETHOD(Rollback)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(get_Operation)(/*[out, retval]*/ IWrapOper** pVal);
	STDMETHOD(BuildFromAccess)(/*[in]*/ IWrapAccess* ai_piAccess, /* [out, retval]*/ short* ao_sRetVal);
	STDMETHOD(BuildFromId)(/*[in]*/BSTR ai_stId, /* [out, retval]*/ short* ao_sRetVal);
	STDMETHOD(BuildAsMetachannel)(/* [out, retval]*/ short* ao_sRetVal);
	STDMETHOD(get_coProps)(/*[out, retval]*/ IWrapColProp* *pVal);
	STDMETHOD(CreateAccess)(/*[out, retval]*/ IWrapAccess** ao_poAccess);
	STDMETHOD(BuildFromfile)(/*[in]*/ BSTR ai_bstrPath, /* [out, retval]*/ short* ao_sRetVal);
	STDMETHOD(get_AutoLoad)(/*[out, retval]*/ BYTE* pVal);
	STDMETHOD(put_AutoLoad)(/*[in]*/ BYTE newVal);
	STDMETHOD(get_ClosingDate)(/*[out, retval]*/ DATE *pVal);
    STDMETHOD(DebugDump)(/*[in]*/ BSTR ai_stFilename);
//EDU:
	STDMETHOD(Load)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(UnLoad)(short * ao_sRetVal);
    STDMETHOD(Persist)(/*[out, retval]*/ short* ao_sRetVal);
    STDMETHOD(Free)(/*[out, retval]*/ short* ao_retval);

//DANI
	STDMETHOD(LoadFromFile)(/*[in]*/ BSTR ai_pstFileName, /*[out, retval]*/ short* ao_sRetVal);
    STDMETHOD(PersistToFile)(/*[in]*/ BSTR ai_pstFileName, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(PersistToTempFile)(/*[out]*/ BSTR * ao_pstFileName, /*[out, retval]*/ short* ao_sRetVal);

//GILES
	STDMETHOD(get_DebugMode)(/* [retval][out] */ VARIANT_BOOL *pVal);
	STDMETHOD(put_DebugMode)(/* [in] */ VARIANT_BOOL newVal);
	STDMETHOD(GetDebugBackAccess)(/* [retval][out] */ IWrapAccess** ao_poBackAccess);

	STDMETHOD(DebugAccessCount)(/* [retval][out] */ short* ao_sRetVal);
    STDMETHOD(DebugMemUsage)(/* [retval][out] */ short* ao_sRetVal);

	STDMETHOD(CheckModify)(/*[out, retval]*/ VARIANT_BOOL * ao_pbModified);

	STDMETHOD(RefreshFlags)(/* [retval][out] */ short* ao_sRetVal);

	STDMETHOD(AddPartner)(/*[in]*/ IWrapChannel* ai_piChannel, /* [out, retval]*/ short* ao_sRetVal);
    STDMETHOD(DelPartner)(/*[in]*/ IWrapChannel* ai_piChannel);
    STDMETHOD(DelAllPartners)(void);

	STDMETHOD(GetAccessBookmarkByHandle)(/*[in]*/ long ai_iPosition, /*[out, retval]*/ IWrapBookMark** ao_bookmark);

private:
	/** Puntero al canal que estamos recubriendo. */
	ClChannel* m_poChannel;

	/**
	 * Indica si el canal lo debo destruir yo, o no.
	 * Lo debo destruir cuando el canal se ha creado, y no debo destruirlo cuando el CM me devuelve uno que ya existe.
	 * Por defecto yo no soy el owner del canal. Para serlo me lo indican en el Init (sólo cuando he creado el canal).
	 * Corrige el bug 0049845.
	 */
	bool m_bOwner;
};

#endif //__WRAPCHANNEL_H_
