//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             channelwrapper.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     ChannelWrapper.cpp : Implementation of DLL Exports.
// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f ChannelWrapperps.mk in the project directory.
//
//==============================================================================


#define M4_IMPL_CHWRAP 1
//Quizas se deberia poner en los Settings del DSP

#include "apichannelwrapper.h"
#include "stdafx.h"
#include "resource.h"
#include "initguid.h"
#include "m4chwrap.h"
#include "m4chwrap_i.c"
#include "wrapnode.h"
#include "wrapcolitems.h"
#include "wrapitem.h"
#include "wrapregister.h"
#include "wrapchannel.h"
#include "wrapaccess.h"
#include "wraprecordset.h"
#include "wrapslice.h"
#include "wrapcolprop.h"
#include "wrapcvm.h"
#include "wrapoper.h"
#include "wrapitemdef.h"
#include "wrapitems.h"
#include "enumitem.h"
#include "wrapnodedef.h"
#include "wrapitit.h"
#include "wraprgit.h"
#include "wrapchandef.h"
#include "wrapslit.h"
#include "wrapimap.h"
#include "wrapexecutor.h"
#include "wraprgit_blockwalker.h"
#include "wrapvmsign.h"
#include "wrapproxylistit.h"
#include "wrapbookmark.h"
#include "wrapstchannelmeminfo.h"
#include "wrapstnodememinfo.h"
#include "wrapstrsmeminfo.h"
#include "wrapstrgmeminfo.h"
#include "wrapchannelmeminfo.h"
#include "wrapbreakpoint.h"
#include "wraprunninginfo.h"
#include "m4unicode.hpp"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
// Test to see if we can reduce the size of the ole dll
// but we need to change the defn of NewInterface for
// this to work (SAM 28/1/1998)
#define BIGOBJ

#ifdef BIGOBJ
	OBJECT_ENTRY(CLSID_WrapNode, CWrapNode)
	OBJECT_ENTRY(CLSID_WrapColItems, CWrapColItems)
	OBJECT_ENTRY(CLSID_WrapItem, CWrapItem)
	OBJECT_ENTRY(CLSID_WrapRegister, CWrapRegister)
//	OBJECT_ENTRY(CLSID_WrapColNodes, CWrapColNodes)
	OBJECT_ENTRY(CLSID_WrapChannel, CWrapChannel)
	OBJECT_ENTRY(CLSID_WrapAccess, CWrapAccess)
	OBJECT_ENTRY(CLSID_WrapRecordSet, CWrapRecordSet)
//	OBJECT_ENTRY(CLSID_WrapVariant, CWrapVariant)
//	OBJECT_ENTRY(CLSID_WrapStack, CWrapStack)
//	OBJECT_ENTRY(CLSID_WrapColSlices, CWrapColSlices)
	OBJECT_ENTRY(CLSID_WrapSlice, CWrapSlice)
	OBJECT_ENTRY(CLSID_WrapColProp, CWrapColProp)
#endif
	OBJECT_ENTRY(CLSID_WrapCVM, CWrapCVM)
#ifdef BIGOBJ
	OBJECT_ENTRY(CLSID_WrapOper, CWrapOper)
	OBJECT_ENTRY(CLSID_WrapItemDef, CWrapItemDef)
	OBJECT_ENTRY(CLSID_WrapItems, CWrapItems)
	OBJECT_ENTRY(CLSID_enumitem, Cenumitem)
	OBJECT_ENTRY(CLSID_WrapNodeDef, CWrapNodeDef)
	OBJECT_ENTRY(CLSID_WrapItIt, CWrapItIt)
	OBJECT_ENTRY(CLSID_WrapRgIt, CWrapRgIt)
	OBJECT_ENTRY(CLSID_WrapChanDef, CWrapChanDef)
	OBJECT_ENTRY(CLSID_WrapSlIt, CWrapSlIt)
	OBJECT_ENTRY(CLSID_WrapIMap, CWrapIMap)
#endif
	OBJECT_ENTRY(CLSID_WrapExecutor, CWrapExecutor)
	OBJECT_ENTRY(CLSID_WrapBookMark, CWrapBookMark)
    OBJECT_ENTRY(CLSID_WrapRgIt_BlockWalker, CWrapRgIt_BlockWalker)
	OBJECT_ENTRY(CLSID_WrapVMSign, CWrapVMSign)
    OBJECT_ENTRY(CLSID_WrapProxyListIt, CWrapProxyListIt)
	OBJECT_ENTRY(CLSID_WrapChannelMemInfo, CWrapChannelMemInfo)
	OBJECT_ENTRY(CLSID_WrapStChannelMemInfo, CWrapStChannelMemInfo)
	OBJECT_ENTRY(CLSID_WrapStNodeMemInfo, CWrapStNodeMemInfo)
	OBJECT_ENTRY(CLSID_WrapStRSMemInfo, CWrapStRSMemInfo)
	OBJECT_ENTRY(CLSID_WrapStRgMemInfo, CWrapStRgMemInfo)
	OBJECT_ENTRY(CLSID_WrapBreakpoint, CWrapBreakpoint)
	OBJECT_ENTRY(CLSID_WrapRunningFormInfo, CWrapRunningFormInfo)

END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point


extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		_Module.Init(ObjectMap, hInstance);
		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
		_Module.Term();
	return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	_Module.UnregisterServer();
	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// M4GetErrorString - Get Error String through ChDll (using Error-Channel) or M4LogSys (using m4logmsg.ini)

//Chlib/version/inc
#include "cldbgutil.hpp"
#include "wrapchannelmeminfo.h"
#include "wrapstchannelmeminfo.h"
#include "wrapstnodememinfo.h"
#include "wrapstrsmeminfo.h"
#include "wrapstrgmeminfo.h"
#include "cllgadap.hpp"

ClChannelManager * getChannelManager(void * piCVM)
{
	ClChannelManager *poChannelManager=NULL;
	if (piCVM) {
		// Bug 155069. Accedemos via COM para obtener el puntero a CWrapCVM
		//  (Para ello se crea la propiedad Read-Only PtrFullInterface que actua como
		//   wrapper de la clase y así poder realizar el casting a CWrapCVM*
		//   e invocar al método C++ GetpChannelManager).
		// Nota: piCVM aparece con un desplazamiento de 4 bytes respecto a la clase.
		//   Si la propiedad wrapper devuelve IWrapCVM* se realiza también un desplazamiento
		//   de 4 bytes en el puntero obtenido.
		//   El comienzo de la estructura de interfaces está  constituida por el puntero a la VTBL
		//   de la clase y posteriormente el puntero a la interface IWrapCVM (hereda de IDispatch).
		//   Por ello el desplazamiento de 4 bytes
		CComDispatchDriver disp;
		if (S_OK == ((IDispatch*)piCVM)->QueryInterface(&disp))
		{
			VARIANT retValue;
			if (S_OK ==  disp.GetPropertyByName(OLESTR("PtrFullInterface"), &retValue))
			{
				poChannelManager = ((CWrapCVM*) (retValue.lVal))->GetpChannelManager();
			}
		}
	}

	return poChannelManager;
}

m4int16_t _M4ChannelWrapperGetErrorStringByParams(m4uint32_t ai_uiCodErr, SAFEARRAY **ai_pParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer, m4language_t ai_uiLang)
{
	m4return_t		ret=M4_SUCCESS;	//Retorno
	m4VariantType *	pvParams=NULL;	//Vector de Variants
	m4uint32_t		uNumParams;		//No. de Variants
	VARIANT *		pvariantTemp;	//Puntero de VARIANT temporal de VB

	m4uint32_t uiBufferSize=0;		//Tamaño del String del Error.

	//Get number of variants
	if (*ai_pParams) {
		uNumParams = (**ai_pParams).rgsabound[0].cElements;
	
		//VARIANT to m4VariantType

		//Get array of variants	
		if ((pvParams = new m4VariantType[uNumParams]) == NULL)
			return M4_ERROR;
		//Lock array memory
		if (SafeArrayAccessData(*ai_pParams, (void HUGEP* FAR*)&pvariantTemp)) {
			delete [] pvParams;
			return M4_ERROR;
		}
		//Copiamos el vector de VARIANTs al vector de m4Variants
		for (m4uint32_t i=0; i<uNumParams;i++) {
			pvParams[i] = pvariantTemp[i];
		}
		//Unlock array memory
		SafeArrayUnaccessData(*ai_pParams);
	} else {
		uNumParams = 0;
	}

	//Get ChannelManager
	ClChannelManager *poChannelManager=getChannelManager(ai_piCVM);

	//Pedimos Longitud del Error a la ChDll
	ret=M4GetErrorStringByParams(ai_uiCodErr, pvParams, uNumParams, ai_byIdParamPos, poChannelManager, NULL, uiBufferSize, ai_uiLang);
	if (ret!=M4_SUCCESS || uiBufferSize<=0) {
		if (pvParams) delete [] pvParams;
		return M4_ERROR;
	}

	// UNICODE VB
	m4pchar_t pcBuffer = NULL ;

	pcBuffer = new m4char_t[ uiBufferSize ] ;

	if( pcBuffer == NULL )
	{
		if (pvParams) delete [] pvParams;
		return M4_ERROR;	// Not enough memory
	}

	//Pedimos la Cadena de Error a la ChDll
	ret=M4GetErrorStringByParams(ai_uiCodErr, pvParams, uNumParams, ai_byIdParamPos, poChannelManager, pcBuffer, uiBufferSize, ai_uiLang);

	if (pvParams) delete [] pvParams;

	if (ret!=M4_SUCCESS) {
		delete [] pcBuffer ;
		ret=M4_ERROR;
	}
	
	// Bug 154828. Se realiza la conversión M4CppToVB en lugar de M4CppToANSI para soportar parámetros Unicode
	*ao_pcBuffer = M4CppToVB( pcBuffer );
	delete [] pcBuffer ;

	if (*ao_pcBuffer == NULL) {
		return M4_ERROR;	// Not enough memory
	}

	return ret;
}



m4int16_t _M4ChannelWrapperGetErrorString(m4uint32_t ai_uiCodErr, m4pchar_t ai_pszParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer, m4language_t ai_uiLang)
{
	m4return_t		ret=M4_SUCCESS;	//Retorno
	m4VariantType *	pvParams=NULL;	//Vector de Variants

	m4uint32_t uiBufferSize=0;		//Tamaño del String del Error.

	//Get ChannelManager
	ClChannelManager *poChannelManager=getChannelManager(ai_piCVM);

	//Pedimos Longitud del Error a la ChDll
	ret=M4GetErrorString	(ai_uiCodErr, ai_pszParams, ai_byIdParamPos, poChannelManager, NULL, uiBufferSize, ai_uiLang);
	if (ret!=M4_SUCCESS || uiBufferSize<=0) {
		return M4_ERROR;
	}

	// UNICODE FILE
	m4pchar_t pcBuffer = NULL ;

	pcBuffer = new m4char_t[ uiBufferSize ] ;

	if( pcBuffer == NULL )
	{
		return M4_ERROR;	// Not enough memory
	}

	//Pedimos la Cadena de Error a la ChDll
	ret=M4GetErrorString(ai_uiCodErr, ai_pszParams, ai_byIdParamPos, poChannelManager, pcBuffer, uiBufferSize, ai_uiLang);
	if (ret!=M4_SUCCESS) {
		delete [] pcBuffer ;
		ret=M4_ERROR;
	}

	// Bug 154828. Se realiza la conversión M4CppToVB en lugar de M4CppToANSI para soportar parámetros Unicode
	*ao_pcBuffer = M4CppToVB( pcBuffer );

	if (*ao_pcBuffer == NULL) {
		return M4_ERROR;	// Not enough memory
	}
	
	return ret;
}

m4int16_t M4_DECL_CHWRAP M4ChannelWrapperGetErrorStringByParams(m4int32_t ai_uiCodErr, SAFEARRAY **ai_pParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer)
{
	m4language_t uiLang;

	//Get ChannelManager
	ClChannelManager *poChannelManager=getChannelManager(ai_piCVM);

	if (!poChannelManager) return M4_ERROR;

	//Lenguaje de la sesión
	uiLang = poChannelManager->GetLogonAdaptor()->GetSessionLanguage("",M4CL_LANGUAGE_NONE);
	return _M4ChannelWrapperGetErrorStringByParams(ai_uiCodErr, ai_pParams, ai_byIdParamPos, ai_piCVM, ao_pcBuffer, uiLang);
}

m4int16_t M4_DECL_CHWRAP M4ChannelWrapperGetErrorString(m4int32_t ai_uiCodErr, m4pchar_t ai_pszParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer)
{
	m4language_t uiLang;

	//Get ChannelManager
	ClChannelManager *poChannelManager=getChannelManager(ai_piCVM);

	if (!poChannelManager) return M4_ERROR;

	//Lenguaje de la sesión
	uiLang = poChannelManager->GetLogonAdaptor()->GetSessionLanguage("",M4CL_LANGUAGE_NONE);
	return _M4ChannelWrapperGetErrorString(ai_uiCodErr, ai_pszParams, ai_byIdParamPos, ai_piCVM, ao_pcBuffer, uiLang);
}

m4int16_t M4_DECL_CHWRAP M4ChannelWrapperGetErrorStringByParamsByLang(m4int32_t ai_uiCodErr, SAFEARRAY **ai_pParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer, short ai_uiLang)
{
	return _M4ChannelWrapperGetErrorStringByParams(ai_uiCodErr, ai_pParams, ai_byIdParamPos, ai_piCVM, ao_pcBuffer, m4language_t(ai_uiLang));
}

m4int16_t M4_DECL_CHWRAP M4ChannelWrapperGetErrorStringByLang(m4int32_t ai_uiCodErr, m4pchar_t ai_pszParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer, short ai_uiLang)
{
	return _M4ChannelWrapperGetErrorString(ai_uiCodErr, ai_pszParams, ai_byIdParamPos, ai_piCVM, ao_pcBuffer, m4language_t(ai_uiLang));
}


m4int16_t M4_DECL_CHWRAP M4ChannelWrapperGetErrorStringByParamsByLangType(m4int32_t ai_uiCodErr, SAFEARRAY **ai_pParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer, short ai_uiLangType)
{
	m4language_t uiLang;

	//Get ChannelManager
	ClChannelManager *poChannelManager=getChannelManager(ai_piCVM);

	if (!poChannelManager) return M4_ERROR;

	//Lenguaje de la sesión o de desarrollo
	if (ai_uiLangType==M4CL_LANGUAGE_TYPE_LOGON) {
		uiLang = poChannelManager->GetLogonAdaptor()->GetSessionLanguage("",M4CL_LANGUAGE_NONE);
	}
	else {
		uiLang = poChannelManager->GetLogonAdaptor()->GetSessionDevLanguage("",M4CL_LANGUAGE_NONE);
	}
	return _M4ChannelWrapperGetErrorStringByParams(ai_uiCodErr, ai_pParams, ai_byIdParamPos, ai_piCVM, ao_pcBuffer, uiLang);
}

m4int16_t M4_DECL_CHWRAP M4ChannelWrapperGetErrorStringByLangType(m4int32_t ai_uiCodErr, m4pchar_t ai_pszParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer, short ai_uiLangType)
{
	m4language_t uiLang;

	//Get ChannelManager
	ClChannelManager *poChannelManager=getChannelManager(ai_piCVM);

	if (!poChannelManager) return M4_ERROR;

	//Lenguaje de la sesión o de desarrollo
	if (ai_uiLangType==M4CL_LANGUAGE_TYPE_LOGON) {
		uiLang = poChannelManager->GetLogonAdaptor()->GetSessionLanguage("",M4CL_LANGUAGE_NONE);
	}
	else {
		uiLang = poChannelManager->GetLogonAdaptor()->GetSessionDevLanguage("",M4CL_LANGUAGE_NONE);
	}
	return _M4ChannelWrapperGetErrorString(ai_uiCodErr, ai_pszParams, ai_byIdParamPos, ai_piCVM, ao_pcBuffer, uiLang);
}



void M4_DECL_CHWRAP M4ChannelWrapperErrorLookup( void* ai_piCVM, SAFEARRAY **ai_ppCodErrs )
{
	m4puint32_t pErrorArray;
	m4uint32_t uNumErrors;

	if (*ai_ppCodErrs == NULL) // Array uninitialized - do nothing
		return;

	//Get ChannelManager
	ClChannelManager *poChannelManager=getChannelManager(ai_piCVM);

	// Get number of params
	uNumErrors = (**ai_ppCodErrs).rgsabound[0].cElements;

	if (SafeArrayAccessData(*ai_ppCodErrs, (void HUGEP* FAR*)&pErrorArray)) 
		return;

	M4ErrorLookup(poChannelManager, pErrorArray, uNumErrors);	
	SafeArrayUnaccessData(*ai_ppCodErrs);
}




// For backward compatibility
m4int16_t M4_DECL_CHWRAP M4ChannelWrapperGetErrorString(
	m4int32_t ai_uiCodErr,
	SAFEARRAY **ai_pParams,	
	IWrapCVM* ai_piCVM,		
	BSTR *ao_pcBuffer)
{
	return M4ChannelWrapperGetErrorStringByParams(ai_uiCodErr, ai_pParams, 1, ai_piCVM, ao_pcBuffer);
}

