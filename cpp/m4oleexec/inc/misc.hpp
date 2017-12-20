//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           OLEExec.dll
// File:             misc.hpp
// Project:          OLE Object integration
// Author:           Meta Software M.S. , S.A
// Date:             06/07/97
// Language:         C++
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "cldefine.hpp"
#include "cltypes.hpp"
#include "cvdate.h"
#include "m4var.hpp"
#include <ocidl.h>
#include "atlbase.h"

static LCID M4_DEFAULT_LCID = MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT);
static IDispatch* s_pDsp;
static IUnknown* s_pUnk;

void InitDLL (void);
void EndDLL (void);

m4return_t M4VarToOLEVar(m4VariantType ai_m4var, VARIANT* ao_olevar);
m4return_t OLEVarToM4Var(VARIANT ai_olevar, m4VariantType* ao_olevar);
UINT GetFuncInterfaceLib(VARIANTARG vaArgLis[],UINT iMaxArg,DISPID dispid,CLSID LibId);
UINT GetFuncInterface(IDispatch* i_pDsp,VARIANTARG vaArgLis[],UINT iMaxArg,DISPID dispid,INVOKEKIND K);
m4return_t M4VarToOLEVar2(m4VariantType ai_m4var, VARIANT* ao_olevar,VARIANT* ao_oleinf);
void ReleaseParamBSTR(VARIANTARG vaArgLis[],m4uint32_t ai_iLongArg);
HRESULT CoCreateInstanceLic(const CLSID& clsid, IUnknown* pUnknownOuter, DWORD dwClsContext, const IID& iid, void **ppv, BSTR bstrLicenseKey);
m4return_t formatErrorMsg( long ai_lResult, m4pchar_t ao_pcErrorMessage, DWORD ai_size );
m4bool_t CheckLockingClipboardOperation();