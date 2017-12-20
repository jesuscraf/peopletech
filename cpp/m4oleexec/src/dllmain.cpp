//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           OLEExec.dll
// File:             dllmain.cpp
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
#include "m4stl.hpp"
#include "access.hpp"
#include "dllmain.hpp"
#include "misc.hpp"
#include "execontx.hpp"
#include "errors.hpp"
#include "oledef.hpp"
#include "m4mdrt.hpp"
#include "m4unicode.hpp"
#include <blocksynchronization.hpp>
#include "cldmcontext.hpp"
#include "cllgadap.hpp"

// bg 179007
ClMutex g_oMutex( M4_TRUE );

// bg 257032
ClSemaphore g_oSem( 1, 1, "Global\\m4cbsynch");

m4bool_t CheckExecutionPermission( ClVMRunContext* ai_poContext, m4pchar_t ai_pcProgId );

BOOL CALLBACK DllMain(HANDLE hInst, DWORD ul_reason_being_called, LPVOID lpReserved) 
{
	switch (ul_reason_being_called)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebugString("OleExec.dll, DllMain\n");
		InitDLL();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		EndDLL();

		CoUninitialize();

		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}

	return TRUE;
	UNREFERENCED_PARAMETER(lpReserved);
}


void PutError(ClVMRunContext* pvmContext,HRESULT hr)
{
	m4VariantType pRet; // Error Managment
	pRet.Type=M4CL_CPP_TYPE_NUMBER;
	pRet.Data.DoubleData=(double)hr;
	(pvmContext->m_pRecordSet)->Current.Item["Error"].Value.Set(pRet);
}

m4return_t ChangeParamWithTypeInfo(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg,VARIANTARG* vaArgList,IDispatch*	i_pDsp,DISPID dispid,INVOKEKIND k)
{
	//JGM PARA OBTENER INFO DE LA TYPELIB 
	UINT iInf;
	VARIANTARG*		vaArgListInf = NULL;
	vaArgListInf = new VARIANTARG[ai_iLongArg+1];
	iInf = 0;
	
	
	iInf=GetFuncInterface(i_pDsp,vaArgListInf,ai_iLongArg,dispid,k);

	/*if (!iInf) 
	{
		//CLSID CL;
		//iInf=GetFuncInterfaceLib(vaArgListInf,ai_iLongArg,dispid,CL);
    }*/
	if (iInf)
	{
		// Convert arguments from M4 variants to OLE variants
		for (m4uint32_t i = 0; i < ai_iLongArg; i++)
		{
			M4VarToOLEVar2(ai_pvArg[i], &vaArgList[(ai_iLongArg-1)-i],&vaArgListInf[i + 1]);
		}
		delete [] vaArgListInf;
		return M4_SUCCESS;
	}
	else 
		return M4_ERROR;
}

m4return_t CheckParamString(m4VariantType *ai_pvArg)
{
	if (ai_pvArg->Type == M4CL_CPP_TYPE_STRING_VAR) 
		return M4_SUCCESS;
	else 
		return M4_ERROR;
}


m4return_t OLE_Execute(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	DISPID			dispid;
	VARIANTARG*		vaArgList = NULL;
	VARIANT			vReturn;
	VARIANT*		pvReturn;
	bool			bReturn; 
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;
	UINT			iArgErr;
	EXCEPINFO		excepinfo;
	m4VariantType	pvDisp;
	

	TRACESUB(OleExec.dll:OLE_Execute);
	
	// First we retrieve the pointer to the IDispatch pointer we
	// have stored in the channel in the "InterfacePointer" item
	// We use the ai_pvContext to access the channel

	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	m4pcchar_t pccMethod = ((pvmContext->m_pAccess->GetpChannel())->GetpCMCR())->GetItemId(pvmContext->m_hItem);
	long	lDispId = (long)((pvmContext->m_pAccess->GetpChannel())->GetpCMCR())->GetItemId(pvmContext->m_hItem);
	bReturn = ((pvmContext->m_pRecordSet)->Current.Item[ClHandle(pvmContext->m_hItem)].ItemDef.M4Type() != 0);
	VariantInit(&vReturn);
	pvReturn = bReturn ? &vReturn : NULL;
	CComBSTR bstrMethod(pccMethod);
	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
	i_pDsp = (IDispatch*)(long)pvDsp.Data.DoubleData;

	// Check server clipboard access in server side.
	// Eg. EXCEL!RANGES.COPY
	if( !strcmpi( pccMethod, "COPY" ) || !strcmpi( pccMethod, "PASTE" ) )
	{
		m4bool_t bRet = CheckLockingClipboardOperation();
		
		if( bRet == M4_FALSE )
		{
			SETCODEF( M4_OBJ_OLE_CB_OPR_LOCKED, ERRORLOG,"#*s1*#%s#", pccMethod );
			TRACERETVAL( OleExec.dll:OLE_Execute, M4_SUCCESS );
		}
	}
	
	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}

	// Create OLE argument list
	if (ai_iLongArg > 0)
		vaArgList = new VARIANTARG[ai_iLongArg];
	DISPPARAMS dpParams = {vaArgList, NULL, ai_iLongArg, 0};
   
	

	// Get DISPID to do the invocation
	hr = i_pDsp->GetIDsOfNames(IID_NULL, &bstrMethod, 1, M4_DEFAULT_LCID, &dispid);

	if (FAILED(hr)) 
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG, "OleExec - OLE method \"%s\" name not recognized", pccMethod);
		delete [] vaArgList;
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
    //
	
	//m4return_t ret=ChangeParamWithTypeInfo(ai_pvArg,ai_iLongArg,vaArgList,i_pDsp,dispid,INVOKE_FUNC);
	// Convert arguments from M4 variants to OLE variants
	//if(ret==M4_ERROR)
		for (m4uint32_t i = 0; i < ai_iLongArg; i++)
		{
			M4VarToOLEVar(ai_pvArg[i], &vaArgList[(ai_iLongArg-1)-i]);
		}
	 

	memset(&excepinfo, 0, sizeof excepinfo);

	iArgErr = (UINT)-1;

	char strErr[1000];
	strErr[0] = NULL;

	// Invocation
	hr = i_pDsp->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, DISPATCH_METHOD, &dpParams, pvReturn, &excepinfo, &iArgErr);

	if (FAILED(hr)) 
	{
		if(hr == DISP_E_EXCEPTION) 
		{
			if(excepinfo.pfnDeferredFillIn != NULL)
				(*(excepinfo.pfnDeferredFillIn)) (&excepinfo);

			USES_CONVERSION;
			if(excepinfo.bstrDescription)
				wsprintf(strErr,"Error:  %s.",OLE2T(excepinfo.bstrDescription));
		}

		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, pccMethod,strErr);
	//	TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR); printf
	}

	if (bReturn)
		OLEVarToM4Var(vReturn, &ai_pvReturn);

	ReleaseParamBSTR(&vReturn,1);
	ReleaseParamBSTR(vaArgList,ai_iLongArg);
	delete [] vaArgList;
    // Error Mangment JGM
			PutError(pvmContext,hr);
    // End Error 

	TRACERETVAL(OleExec.dll:OLE_Execute, M4_SUCCESS);
}


//7/10/98 JBF
//Same as OLE_Execute but the first argument is the function naem to execute
m4return_t OLE_EXECUTE_NAME(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	DISPID			dispid;
	VARIANTARG*		vaArgList = NULL;
	VARIANT			vReturn;
	VARIANT*		pvReturn;
	bool			bReturn; 
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;
	UINT			iArgErr;
	EXCEPINFO		excepinfo;

	TRACESUB(OleExec.dll:OLE_Execute);

	// First we retrieve the pointer to the IDispatch pointer we
	// have stored in the channel in the "InterfacePointer" item
	// We use the ai_pvContext to access the channel
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
//	m4pcchar_t pccMethod = ((pvmContext->m_pAccess->GetpChannel())->GetpCMCR())->GetItemId(pvmContext->m_hItem);
	bReturn = ((pvmContext->m_pRecordSet)->Current.Item[ClHandle(pvmContext->m_hItem)].ItemDef.M4Type() != 0);
	pvReturn = bReturn ? &vReturn : NULL;

	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
	i_pDsp = (IDispatch*)(long)pvDsp.Data.DoubleData;
	
	
	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}

	if ((ai_iLongArg<1)||(CheckParamString(&ai_pvArg[0])==M4_ERROR))
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - Parameter Name is not a String");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	CComBSTR bstrMethod(ai_pvArg[0].Data.PointerChar);
	// Create OLE argument list
	if (ai_iLongArg > 1)
		vaArgList = new VARIANTARG[ai_iLongArg-1];
	DISPPARAMS dpParams = {vaArgList, NULL, ai_iLongArg-1, 0};
/*
	// Convert arguments from M4 variants to OLE variants
	for (m4uint32_t i = 1; i < ai_iLongArg; i++)
	{
		M4VarToOLEVar(ai_pvArg[i], &vaArgList[(ai_iLongArg-1)-i]);
	}
*/
	// Get DISPID to do the invocation
	hr = i_pDsp->GetIDsOfNames(IID_NULL, &bstrMethod, 1, M4_DEFAULT_LCID, &dispid);

	if (FAILED(hr)) 
	{
		ReleaseParamBSTR(vaArgList,ai_iLongArg-1);
		delete [] vaArgList;
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG, "OleExec - OLE method \"%s\" name not recognized", ai_pvArg[0].Data.PointerChar);
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}

		//m4return_t ret=ChangeParamWithTypeInfo(&ai_pvArg[1],ai_iLongArg-1,vaArgList,i_pDsp,dispid,INVOKE_FUNC);
	    // Convert arguments from M4 variants to OLE variants
		//if(ret==M4_ERROR)
		for (m4uint32_t i = 1; i < ai_iLongArg; i++)
		{
			M4VarToOLEVar(ai_pvArg[i], &vaArgList[(ai_iLongArg-1)-i]);
		}
	

	memset(&excepinfo, 0, sizeof excepinfo);

	char strErr[1000];
	strErr[0] = NULL;

	iArgErr = (UINT)-1;

	// Invocation
	hr = i_pDsp->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, DISPATCH_METHOD, &dpParams, pvReturn, &excepinfo, &iArgErr);

	if (FAILED(hr)) 
	{
		if(hr == DISP_E_EXCEPTION)
		{
			if(excepinfo.pfnDeferredFillIn != NULL)
				(*(excepinfo.pfnDeferredFillIn)) (&excepinfo);

			USES_CONVERSION;
			if(excepinfo.bstrDescription)
				wsprintf(strErr,"Error:  %s.",OLE2T(excepinfo.bstrDescription));
		}

		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, ai_pvArg[0].Data.PointerChar, strErr);
		//TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}

	if (bReturn)
		OLEVarToM4Var(vReturn, &ai_pvReturn);
	ReleaseParamBSTR(vaArgList,ai_iLongArg-1);
	ReleaseParamBSTR(&vReturn,1);
	delete [] vaArgList;
// Error Mangment JGM

	PutError(pvmContext,hr);
	TRACERETVAL(OleExec.dll:OLE_Execute, M4_SUCCESS);
}


m4return_t OLE_EXECUTE_ID(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	DISPID			dispid;
	VARIANTARG*		vaArgList = NULL;
	VARIANT			vReturn;
	VARIANT*		pvReturn;
	bool			bReturn; 
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;
	UINT			iArgErr;
	EXCEPINFO		excepinfo;

	TRACESUB(OleExec.dll:OLE_Execute);

	// First we retrieve the pointer to the IDispatch pointer we
	// have stored in the channel in the "InterfacePointer" item
	// We use the ai_pvContext to access the channel
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
//	m4pcchar_t pccMethod = ((pvmContext->m_pAccess->GetpChannel())->GetpCMCR())->GetItemId(pvmContext->m_hItem);
	long tipo = (pvmContext->m_pRecordSet)->Current.Item[ClHandle(pvmContext->m_hItem)].ItemDef.M4Type();
	bReturn = ((pvmContext->m_pRecordSet)->Current.Item[ClHandle(pvmContext->m_hItem)].ItemDef.M4Type() != 0);

	VariantInit(&vReturn);

	pvReturn = bReturn ? &vReturn : NULL;
	//pvReturn = NULL;

	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
	i_pDsp = (IDispatch*)(long)pvDsp.Data.DoubleData;
	
	
	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}

	if (ai_iLongArg<1 )
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - Parameter Id is not a Number");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	double idMethod = ai_pvArg[0].Data.DoubleData;

	// Create OLE argument list
	if (ai_iLongArg > 1)
		vaArgList = new VARIANTARG[ai_iLongArg-1];
	DISPPARAMS dpParams = {vaArgList, NULL, ai_iLongArg-1, 0};
/*
	// Convert arguments from M4 variants to OLE variants
	for (m4uint32_t i = 1; i < ai_iLongArg; i++)
	{
		M4VarToOLEVar(ai_pvArg[i], &vaArgList[(ai_iLongArg-1)-i]);
	}
*/
	// Get DISPID to do the invocation
	//hr = i_pDsp->GetIDsOfNames(IID_NULL, &bstrMethod, 1, M4_DEFAULT_LCID, &dispid);
	dispid = idMethod;


    if (FAILED(hr)) 
	{
		ReleaseParamBSTR(vaArgList,ai_iLongArg-1);
		delete [] vaArgList;
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG, "OleExec - OLE method \"%d\" id not recognized", ai_pvArg[0].Data.DoubleData);
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}


	//m4return_t ret=ChangeParamWithTypeInfo(&ai_pvArg[1],ai_iLongArg-1,vaArgList,i_pDsp,dispid,INVOKE_FUNC);
	// Convert arguments from M4 variants to OLE variants
	//if(ret==M4_ERROR)
	for (m4uint32_t i = 1; i < ai_iLongArg; i++)
	{
		M4VarToOLEVar(ai_pvArg[i], &vaArgList[(ai_iLongArg-1)-i]);
	}
	

	memset(&excepinfo, 0, sizeof excepinfo);

	char strErr[1000];
	strErr[0] = NULL;

	iArgErr = (UINT)-1;

	// Invocation
	hr = i_pDsp->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, DISPATCH_METHOD, &dpParams, pvReturn, &excepinfo, &iArgErr);

	if (FAILED(hr)) 
	{
		if(hr == DISP_E_EXCEPTION)
		{
			if(excepinfo.pfnDeferredFillIn != NULL)
				(*(excepinfo.pfnDeferredFillIn)) (&excepinfo);

			USES_CONVERSION;
			if(excepinfo.bstrDescription)
				wsprintf(strErr,"Error:  %s.",OLE2T(excepinfo.bstrDescription));
		}

		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, ai_pvArg[0].Data.DoubleData, strErr);
		//TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}

	if (pvReturn != NULL)
		//OLEVarToM4Var(vReturn, &ai_pvReturn);
		OLEVarToM4Var(*pvReturn, &ai_pvReturn);
		
	ReleaseParamBSTR(vaArgList,ai_iLongArg-1);
	ReleaseParamBSTR(&vReturn,1);
	delete [] vaArgList;
// Error Mangment JGM

	PutError(pvmContext,hr);
	TRACERETVAL(OleExec.dll:OLE_Execute, M4_SUCCESS);
}





m4return_t OLE_Property(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	DISPID			dispid;
	VARIANTARG*		vaArgList = NULL;
	VARIANT			vRetVal;
	VARIANT*			pvRetVal;
	WORD				wFlags;
	DISPPARAMS		dpParams;
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;
	EXCEPINFO		excepinfo;

	TRACESUB(OleExec.dll:OLE_Property);
	
	// First we retrieve the pointer to the IDispatch pointer we
	// have stored in the channel in the "InterfacePointer" item
	// We use the ai_pvContext to access the channel
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	m4pcchar_t pccProperty = ((pvmContext->m_pAccess->GetpChannel())->GetpCMCR())->GetItemId(pvmContext->m_hItem);
	CComBSTR bstrProperty(pccProperty);
	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
	i_pDsp = (IDispatch*)(long)pvDsp.Data.DoubleData;

	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}

	// Get DISPID for invocation
	hr = i_pDsp->GetIDsOfNames(IID_NULL, &bstrProperty, 1, M4_DEFAULT_LCID, &dispid);

	if (FAILED(hr)) 
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG, "OleExec - OLE property \"%s\" name not recognized", pccProperty);
		TRACERETVAL(OleExec.dll:OLE_Property, M4_ERROR);
	}

	// If we receive an argument then we do a set otherwise we do a get
	if (ai_iLongArg == 1)
	{
		VARIANT vNewValue;
		DISPID dspNamedArg = DISPID_PROPERTYPUT;
		//m4return_t ret=ChangeParamWithTypeInfo(ai_pvArg,1,&vNewValue,i_pDsp,dispid,INVOKE_PROPERTYPUT);
	// Convert arguments from M4 variants to OLE variants
		//if(ret==M4_ERROR)
			M4VarToOLEVar(ai_pvArg[0], &vNewValue);
		
	

		//M4VarToOLEVar(ai_pvArg[0], &vNewValue);
		dpParams.rgvarg = &vNewValue;
		dpParams.rgdispidNamedArgs = &dspNamedArg;
		dpParams.cArgs = 1;
		dpParams.cNamedArgs = 1;
		wFlags = DISPATCH_PROPERTYPUT;
		pvRetVal = NULL;
	}
	else  //  ai_iLongArg == 0
	{
		dpParams.rgvarg = NULL;
		dpParams.rgdispidNamedArgs = NULL;
		dpParams.cArgs = 0;
		dpParams.cNamedArgs = 0;
		wFlags = DISPATCH_PROPERTYGET;
		pvRetVal = &vRetVal;
	}

	memset(&excepinfo, 0, sizeof excepinfo);

	char strErr[1000];
	strErr[0] = NULL;

	// Invocation
	hr = i_pDsp->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, wFlags, &dpParams, pvRetVal, &excepinfo, NULL);

	if (FAILED(hr)) 
	{
		if(hr == DISP_E_EXCEPTION)
		{
			if(excepinfo.pfnDeferredFillIn != NULL)
				(*(excepinfo.pfnDeferredFillIn)) (&excepinfo);

			USES_CONVERSION;
			if(excepinfo.bstrDescription)
				wsprintf(strErr,"Error:  %s.",OLE2T(excepinfo.bstrDescription));
		}
		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, pccProperty, strErr);
		//TRACERETVAL(OleExec.dll:OLE_Property, M4_ERROR);
	}

	// If we are doing a get then fill in return value
	if (ai_iLongArg == 0)
	{
		// bg 243861
		if( vRetVal.vt == VT_DATE && vRetVal.date <= 60 )
		{
			vRetVal.date++ ;
		}

		OLEVarToM4Var(vRetVal, &ai_pvReturn);
	}
	

	ReleaseParamBSTR(&vRetVal,1);
	// Error Mangment JGM
	PutError(pvmContext,hr);
		
   
	return M4_SUCCESS;
}

//7/10/98 JBF
//New function analogous to OLE_PROPERTY, but which gets the name for the Property from 
//the first arg instead of from the item name
m4return_t OLE_PROPERTY_NAME(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	DISPID			dispid;
	VARIANTARG*		vaArgList = NULL;
	VARIANT			vRetVal;
	VARIANT*			pvRetVal;
	WORD				wFlags;
	DISPPARAMS		dpParams;
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;
	VARIANT vNewValue;

	TRACESUB(OleExec.dll:OLE_Property);

	// First we retrieve the pointer to the IDispatch pointer we
	// have stored in the channel in the "InterfacePointer" item
	// We use the ai_pvContext to access the channel
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	//m4pcchar_t pccProperty = ((pvmContext->m_pAccess->GetpChannel())->GetpCMCR())->GetItemId(pvmContext->m_hItem);
	
	//Get the Property from 1 argument
	
	if ((ai_iLongArg<1)||(CheckParamString(&ai_pvArg[0])==M4_ERROR))
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - Parameter Name is not a String");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	CComBSTR bstrProperty(ai_pvArg[0].Data.PointerChar);
	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
	i_pDsp = (IDispatch*)(long)pvDsp.Data.DoubleData;


	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}

	// Get DISPID for invocation
	hr = i_pDsp->GetIDsOfNames(IID_NULL, &bstrProperty, 1, M4_DEFAULT_LCID, &dispid);

	if (FAILED(hr)) 
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG, "OleExec - OLE property \"%s\" name not recognized", ai_pvArg[0].Data.PointerChar);
		TRACERETVAL(OleExec.dll:OLE_Property, M4_ERROR);
	}

	// If we receive two argument then we do a set otherwise we do a get
	if (ai_iLongArg == 2)  
	{
		DISPID dspNamedArg = DISPID_PROPERTYPUT;
		VARIANT vNewValue;
	//	m4return_t ret=ChangeParamWithTypeInfo(&ai_pvArg[1],1,&vNewValue,i_pDsp,dispid,INVOKE_PROPERTYPUT);
	// Convert arguments from M4 variants to OLE variants
	//	if(ret==M4_ERROR)
			M4VarToOLEVar(ai_pvArg[1], &vNewValue);
	//	M4VarToOLEVar(ai_pvArg[1], &vNewValue);
		dpParams.rgvarg = &vNewValue;
		dpParams.rgdispidNamedArgs = &dspNamedArg;
		dpParams.cArgs = 1;
		dpParams.cNamedArgs = 1;
		wFlags = DISPATCH_PROPERTYPUT;
		pvRetVal = NULL;
	}
	else  //  ai_iLongArg == 0
	{
		dpParams.rgvarg = NULL;
		dpParams.rgdispidNamedArgs = NULL;
		dpParams.cArgs = 0;
		dpParams.cNamedArgs = 0;
		wFlags = DISPATCH_PROPERTYGET;
		pvRetVal = &vRetVal;
	}

	// Invocation
	hr = i_pDsp->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, wFlags, &dpParams, pvRetVal, NULL, NULL);

	if (FAILED(hr)) 
	{
		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, ai_pvArg[0].Data.PointerChar,"");
		//TRACERETVAL(OleExec.dll:OLE_Property, M4_ERROR);
	}

	// If we are doing a get then fill in return value
	if (ai_iLongArg == 1)
		OLEVarToM4Var(vRetVal, &ai_pvReturn);
	if (ai_iLongArg==2)
		ReleaseParamBSTR(&vNewValue,1);
	ReleaseParamBSTR(&vRetVal,1);
     // Error Mangment JGM
		PutError(pvmContext,hr);
	return M4_SUCCESS;
}

m4return_t OLE_PROPERTY_ID(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	DISPID			dispid;
	VARIANTARG*		vaArgList = NULL;
	VARIANT			vRetVal;
	VARIANT*			pvRetVal;
	WORD				wFlags;
	DISPPARAMS		dpParams;
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;
	VARIANT vNewValue;

	TRACESUB(OleExec.dll:OLE_Property);

	// First we retrieve the pointer to the IDispatch pointer we
	// have stored in the channel in the "InterfacePointer" item
	// We use the ai_pvContext to access the channel
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	//m4pcchar_t pccProperty = ((pvmContext->m_pAccess->GetpChannel())->GetpCMCR())->GetItemId(pvmContext->m_hItem);
	
	//Get the Property from 1 argument
	
	if (ai_iLongArg<1)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - Parameter Name is not a String");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}

	//CComBSTR bstrProperty(ai_pvArg[0].Data.PointerChar);
	double idMethod = ai_pvArg[0].Data.DoubleData;

	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
	i_pDsp = (IDispatch*)(long)pvDsp.Data.DoubleData;


	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}

	// Get DISPID for invocation
	//hr = i_pDsp->GetIDsOfNames(IID_NULL, &bstrProperty, 1, M4_DEFAULT_LCID, &dispid);
	dispid = idMethod;

	if (FAILED(hr)) 
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG, "OleExec - OLE property \"%s\" name not recognized", ai_pvArg[0].Data.PointerChar);
		TRACERETVAL(OleExec.dll:OLE_Property, M4_ERROR);
	}

	// If we receive two argument then we do a set otherwise we do a get
	if (ai_iLongArg == 2)  
	{
		DISPID dspNamedArg = DISPID_PROPERTYPUT;
		VARIANT vNewValue;
	//	m4return_t ret=ChangeParamWithTypeInfo(&ai_pvArg[1],1,&vNewValue,i_pDsp,dispid,INVOKE_PROPERTYPUT);
	// Convert arguments from M4 variants to OLE variants
	//	if(ret==M4_ERROR)
			M4VarToOLEVar(ai_pvArg[1], &vNewValue);
	//	M4VarToOLEVar(ai_pvArg[1], &vNewValue);
		dpParams.rgvarg = &vNewValue;
		dpParams.rgdispidNamedArgs = &dspNamedArg;
		dpParams.cArgs = 1;
		dpParams.cNamedArgs = 1;
		wFlags = DISPATCH_PROPERTYPUT;
		pvRetVal = NULL;
	}
	else  //  ai_iLongArg == 0
	{
		dpParams.rgvarg = NULL;
		dpParams.rgdispidNamedArgs = NULL;
		dpParams.cArgs = 0;
		dpParams.cNamedArgs = 0;
		wFlags = DISPATCH_PROPERTYGET;
		pvRetVal = &vRetVal;
	}

	// Invocation
	hr = i_pDsp->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, wFlags, &dpParams, pvRetVal, NULL, NULL);

	if (FAILED(hr)) 
	{
		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, ai_pvArg[0].Data.DoubleData,"");
		//TRACERETVAL(OleExec.dll:OLE_Property, M4_ERROR);
	}

	// If we are doing a get then fill in return value
	if (ai_iLongArg == 1)
		OLEVarToM4Var(vRetVal, &ai_pvReturn);
	if (ai_iLongArg==2)
		ReleaseParamBSTR(&vNewValue,1);
	ReleaseParamBSTR(&vRetVal,1);
     // Error Mangment JGM
		PutError(pvmContext,hr);
	return M4_SUCCESS;
}

	


// 
// 	OLE_SetProp included only for backwards compatibility
// 	it has been superceded by OLE_Property
// 
m4return_t OLE_SetProp(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	MessageBox(NULL, "The function OLE_SetProp is no longer supported - please use OLE_Property instead",
		NULL, MB_OK | MB_ICONHAND); 
	return M4_SUCCESS;
}

// 
// 	OLE_GetProp included only for backwards compatibility
// 	it has been superceded by OLE_Property
// 
m4return_t OLE_GetProp(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	MessageBox(NULL, "The function OLE_GetProp is no longer supported - please use OLE_Property instead",
		NULL, MB_OK | MB_ICONHAND); 
	return M4_SUCCESS;
}


m4return_t Initialise(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	GUID				rclsid;
	IUnknown*		i_pUnk;
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;

	TRACESUB(OleExec.dll:Initialise);

	//Bug fixed: 0166076 27/04/2010
    AllowSetForegroundWindow(ASFW_ANY);
    

	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;

	// Check argument
	if ((ai_iLongArg != 1) || (ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR))
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INIT_ERROR, ERRORLOG, "OleExec - Error in Initialize (invalid arguments)");
		TRACERETVAL(OleExec.dll:Initialise, M4_ERROR);
	}

	m4bool_t bRet = CheckExecutionPermission( pvmContext, ai_pvArg[0].Data.PointerChar );
	if( bRet == M4_FALSE )
	{
		PutError( pvmContext, M4_ERROR );
		SETCODEF( M4_OBJ_SRV_COM_APP_LOCKED, ERRORLOG,"#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
		TRACERETVAL( OleExec.dll:ExecuteCom, M4_SUCCESS );
	}

	CComBSTR bstrClassName(ai_pvArg[0].Data.PointerChar);
	
	// Starts COM
	hr = CoInitialize(NULL);

	// Get Global ID of object
	hr = CLSIDFromProgID(bstrClassName, &rclsid);

	if (FAILED(hr))
	{
		// Se llama a esta función al descargar la DLL
		//CoUninitialize();
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_NO_CLSID, ERRORLOG, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar);
		TRACERETVAL(OleExec.dll:Initialise, M4_ERROR);
    }

	// Create object
	hr = CoCreateInstance(rclsid, NULL, CLSCTX_SERVER, IID_IUnknown, (void**)&i_pUnk);

	if (FAILED(hr))
	{
		//CoUninitialize();
		PutError(pvmContext,hr);
		SETCODE(M4_OBJ_OLE_CREATE_FAILED, ERRORLOG, "OleExec - could not create object");
		TRACERETVAL(OleExec.dll:Initialise, M4_ERROR);
   }

	// Query to see if object supports Automation
	hr = i_pUnk->QueryInterface(IID_IDispatch, (void**)&i_pDsp);

	if (hr == E_NOINTERFACE )
	{
		//CoUninitialize();
		PutError(pvmContext,hr);
		SETCODE(M4_OBJ_OLE_NO_AUTOMATION, ERRORLOG, "OleExec - object does not support automation");
		TRACERETVAL(OleExec.dll:Initialise, M4_ERROR);
   }

	// Store object pointer in channel to be retrieved by subsequent calls
	pvDsp.Type = M4CL_CPP_TYPE_NUMBER;
	pvDsp.Data.DoubleData = (double) (long) i_pDsp;
	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Set(pvDsp);
	hr = i_pUnk->Release();
	PutError(pvmContext,M4_SUCCESS);
	TRACERETVAL(OleExec.dll:Initialise, M4_SUCCESS);
}


m4return_t INITIALISE_LIC(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	GUID			rclsid;
	IUnknown*		i_pUnk;
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;

	TRACESUB(OleExec.dll:INITIALISE_LIC);

	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;

	// Check argument
	if ((ai_iLongArg != 2) || (ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR)||(ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR))
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INIT_ERROR, ERRORLOG, "OleExec - Error in Initialize (invalid arguments)");
		TRACERETVAL(OleExec.dll:Initialise, M4_ERROR);
	}
	
	CComBSTR bstrClassName(ai_pvArg[0].Data.PointerChar);
	//Second argument contains the license key for the object to be created.
	CComBSTR bstrLicenseKey(ai_pvArg[1].Data.PointerChar);

	m4bool_t bRet = CheckExecutionPermission( pvmContext, ai_pvArg[0].Data.PointerChar );
	if( bRet == M4_FALSE )
	{
		PutError( pvmContext, M4_ERROR );
		SETCODEF( M4_OBJ_SRV_COM_APP_LOCKED, ERRORLOG,"#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
		TRACERETVAL( OleExec.dll:ExecuteCom, M4_SUCCESS );
	}

	// Starts COM
	hr = CoInitialize(NULL);

	// Get Global ID of object
	hr = CLSIDFromProgID(bstrClassName, &rclsid);

	if (FAILED(hr))
	{
		// Se llama a esta función al descargar la DLL
		//CoUninitialize();
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_NO_CLSID, ERRORLOG, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar);
		TRACERETVAL(OleExec.dll:Initialise, M4_ERROR);
    }

	// Create object
	hr = CoCreateInstanceLic(rclsid, NULL, CLSCTX_SERVER, IID_IUnknown, (void**)&i_pUnk,bstrLicenseKey);

	if (FAILED(hr))
	{
		//CoUninitialize();
		PutError(pvmContext,hr);
		SETCODE(M4_OBJ_OLE_CREATE_FAILED, ERRORLOG, "OleExec - could not create object");
		TRACERETVAL(OleExec.dll:Initialise, M4_ERROR);
   }

	// Query to see if object supports Automation
	hr = i_pUnk->QueryInterface(IID_IDispatch, (void**)&i_pDsp);

	if (hr == E_NOINTERFACE )
	{
		//CoUninitialize();
		PutError(pvmContext,hr);
		SETCODE(M4_OBJ_OLE_NO_AUTOMATION, ERRORLOG, "OleExec - object does not support automation");
		TRACERETVAL(OleExec.dll:Initialise, M4_ERROR);
   }

	// Store object pointer in channel to be retrieved by subsequent calls
	pvDsp.Type = M4CL_CPP_TYPE_NUMBER;
	pvDsp.Data.DoubleData = (double) (long) i_pDsp;
	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Set(pvDsp);
	hr = i_pUnk->Release();
	PutError(pvmContext,M4_SUCCESS);
	TRACERETVAL(OleExec.dll:Initialise, M4_SUCCESS);
}

m4return_t Release (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;

	TRACESUB(OleExec.dll:Release);

	// Get interface pointer
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
	i_pDsp = (IDispatch*)(long)pvDsp.Data.DoubleData;

	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	

	// Do release


	hr = i_pDsp->Release();

	if(hr == 0)
	{
		i_pDsp = NULL;

		// Store object pointer in channel to be retrieved by subsequent calls
		pvDsp.Type = M4CL_CPP_TYPE_NUMBER;
		pvDsp.Data.DoubleData = NULL;
		(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Set(pvDsp);
	}

	// Se llama a esta función al descargar la DLL
	//CoUninitialize();
	PutError(pvmContext,hr);
	TRACERETVAL(OleExec.dll:Release, M4_SUCCESS);
}

//7/9/98 ReleaseCol -> Releases the IPointer, but not Uninitialices the COM Library as it 
//is never initialized before.
m4return_t ReleaseCol (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;

	TRACESUB(OleExec.dll:Release);

	// Get interface pointer
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
	i_pDsp = (IDispatch*)(long)pvDsp.Data.DoubleData;

	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	

	// Do release


	hr = i_pDsp->Release();

	if(hr == 0)
	{
		i_pDsp = NULL;

		// Store object pointer in channel to be retrieved by subsequent calls
		pvDsp.Type = M4CL_CPP_TYPE_NUMBER;
		pvDsp.Data.DoubleData = NULL;
		(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Set(pvDsp);
	}
	PutError(pvmContext,hr);
	TRACERETVAL(OleExec.dll:Release, M4_SUCCESS);
}

m4return_t GetObjectFromCollection(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	IDispatch*		i_pDspParent;
	IDispatch*		i_pDspCollection;
	IDispatch*		i_pDspChild;
	m4VariantType	pvDsp;
	DISPID			dispid;
	EXCEPINFO		sEInfo;
	UINT				iNoArg;
	VARIANTARG*		vaArgList = NULL;
	VARIANT			vCollection, vChild;

	TRACESUB(OleExec.dll:GetObjectFromCollection);

	//  Get interface pointer from the channel - this will be in the item
	//  "ParentInterfacePointer" since there will be a connector between it and
	//  the "InterfacePointer" item in the parent node.
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item["ParentInterfacePointer"].Value.Get(pvDsp);
	i_pDspParent = (IDispatch*)(long)pvDsp.Data.DoubleData;
	DISPPARAMS dpParams = {NULL, NULL, 0, 0};


	// Check to see that our object still exists
	if (CheckServerAlive(i_pDspParent) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	if ((ai_iLongArg<2)||(CheckParamString(&ai_pvArg[0])==M4_ERROR) || (CheckParamString(&ai_pvArg[1])==M4_ERROR))
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - Parameter Name is not a String");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	
	CComBSTR bstrCollectionName(ai_pvArg[0].Data.PointerChar);
	CComBSTR bstrAccessFunc(ai_pvArg[1].Data.PointerChar);

	// The first argument we receive is the name of a collection inside the parent object
	// First we get the collection object
	hr = i_pDspParent->GetIDsOfNames( IID_NULL, &bstrCollectionName, 1, M4_DEFAULT_LCID, &dispid);
	if (FAILED(hr))
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_COLLECTION, ERRORLOG, "#*s1*#%s#",ai_pvArg[0].Data.PointerChar);
		TRACERETVAL(OleExec.dll:GetObjectFromCollection, M4_ERROR);
	}

	hr = i_pDspParent->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, DISPATCH_PROPERTYGET, &dpParams,
		&vCollection, NULL, NULL);
	if (FAILED(hr))
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, ai_pvArg[0].Data.PointerChar,"");
		//TRACERETVAL(OleExec.dll:GetObjectFromCollection, M4_ERROR);
		pvDsp.Type = M4CL_CPP_TYPE_NUMBER;
		pvDsp.Data.DoubleData = (double) (long)NULL;
		(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Set(pvDsp);
		TRACERETVAL(OleExec.dll:GetObjectFromCollection, M4_SUCCESS);
	}
	
	i_pDspCollection = vCollection.pdispVal;

	// The second argument is the name of a function inside the collection 
	// Now we call this function
	if (ai_iLongArg>2)
		vaArgList = new VARIANTARG[ai_iLongArg-2];
	
	dpParams.rgvarg = vaArgList;
	dpParams.cArgs = ai_iLongArg-2;

    VARIANTARG vOpt;
    vOpt.vt = VT_ERROR;
    vOpt.scode = DISP_E_PARAMNOTFOUND;

	for (m4uint32_t i=2; i < ai_iLongArg; i++)
	{
		if (ai_pvArg[i].GetType() != M4CL_CPP_TYPE_NULL)
			M4VarToOLEVar(ai_pvArg[i], &vaArgList[(ai_iLongArg-1)-i]);
		else			
			vaArgList[(ai_iLongArg-1)-i] = vOpt;
	}

	hr = i_pDspCollection->GetIDsOfNames( IID_NULL, &bstrAccessFunc, 1, M4_DEFAULT_LCID, &dispid);
	if (FAILED(hr))
	{
		if (ai_iLongArg>2)
			ReleaseParamBSTR(vaArgList,ai_iLongArg-2);
		delete [] vaArgList;	
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG,"#*s1*#%s#",ai_pvArg[1].Data.PointerChar);
		TRACERETVAL(OleExec.dll:GetObjectFromCollection, M4_ERROR);
	}

	hr = i_pDspCollection->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID,	
		DISPATCH_METHOD|DISPATCH_PROPERTYGET, &dpParams, &vChild, &sEInfo, &iNoArg);
	if (FAILED(hr))
	{
		if (ai_iLongArg>2)
			ReleaseParamBSTR(vaArgList,ai_iLongArg-2);
		delete [] vaArgList;
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, ai_pvArg[1].Data.PointerChar,"");
		//TRACERETVAL(OleExec.dll:GetObjectFromCollection, M4_ERROR);
		pvDsp.Type = M4CL_CPP_TYPE_NUMBER;
		pvDsp.Data.DoubleData = (double) (long)NULL;
		(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Set(pvDsp);
		
		//bg 0123133, 0123134
		hr = i_pDspCollection->Release();

		TRACERETVAL(OleExec.dll:GetObjectFromCollection, M4_SUCCESS);
	}
    else
    {
	hr = i_pDspCollection->Release();
	i_pDspChild = vChild.pdispVal;

	pvDsp.Type = M4CL_CPP_TYPE_NUMBER;
	pvDsp.Data.DoubleData = (double) (long) i_pDspChild;

	// Store the new interface pointer back in the channel
	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Set(pvDsp);
	PutError(pvmContext,M4_SUCCESS);
	if (ai_iLongArg>2)
			ReleaseParamBSTR(vaArgList,ai_iLongArg-2);
		delete [] vaArgList;
	TRACERETVAL(OleExec.dll:GetObjectFromCollection, M4_SUCCESS);
	}
}

m4return_t GetObjectFromProperty(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	IDispatch*		i_pDspParent;
	IDispatch*		i_pDspChild;
	m4VariantType	pvDsp;
	DISPID			dispid;
	VARIANT			vChild;

	TRACESUB(OleExec.dll:GetObjectFromProperty);
	
	//  Get interface pointer from the channel - this will be in the item
	//  "ParentInterfacePointer" since there will be a connector between it and
	//  the "InterfacePointer" item in the parent node.
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item["ParentInterfacePointer"].Value.Get(pvDsp);
	i_pDspParent = (IDispatch*)(long)pvDsp.Data.DoubleData;

	// Check to see that our object still exists
	if (CheckServerAlive(i_pDspParent) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}

	if ((ai_iLongArg<1)||(CheckParamString(&ai_pvArg[0])==M4_ERROR)) 
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - Parameter Name is not a String");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	
	CComBSTR bstrPropertyName(ai_pvArg[0].Data.PointerChar);
	DISPPARAMS dpParams = {NULL, NULL, 0, 0};

	// Get DISPID for invocation
	hr = i_pDspParent->GetIDsOfNames( IID_NULL, &bstrPropertyName, 1, M4_DEFAULT_LCID, &dispid);
	if (FAILED(hr)) 
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG, "#*s1*#%s#",ai_pvArg[0].Data.PointerChar);
		TRACERETVAL(OleExec.dll:GetObjectFromProperty, M4_ERROR);
	}

	// Invocation
	hr = i_pDspParent->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, DISPATCH_PROPERTYGET, &dpParams, &vChild,
		NULL, NULL);
	if (FAILED(hr)) 
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, ai_pvArg[0].Data.PointerChar,"");
		//TRACERETVAL(OleExec.dll:GetObjectFromProperty, M4_ERROR);
		pvDsp.Type = M4CL_CPP_TYPE_NUMBER;
		pvDsp.Data.DoubleData = (double) (long) NULL;
		TRACERETVAL(OleExec.dll:GetObjectFromProperty, M4_SUCCESS);
	}
    else 
	{
	i_pDspChild = vChild.pdispVal;
	//hr = i_pDspParent->Release();
	pvDsp.Type = M4CL_CPP_TYPE_NUMBER;
	pvDsp.Data.DoubleData = (double) (long) i_pDspChild;

	// Store the new interface pointer back in the channel
	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Set(pvDsp);
	PutError(pvmContext,M4_SUCCESS);
	TRACERETVAL(OleExec.dll:GetObjectFromProperty, M4_SUCCESS);
	}
}

m4return_t GetObjectFromMethod (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	
	HRESULT			hr = ERROR_SUCCESS;
	IDispatch*		i_pDspParent;
	IDispatch*		i_pDspChild;
	m4VariantType	pvDsp;
	DISPID			dispid;
	VARIANTARG*		vaArgList = NULL;
	VARIANT			vChild;

	TRACESUB(OleExec.dll:GetObjectFromMethod);

	//  Get interface pointer from the channel - this will be in the item
	//  "ParentInterfacePointer" since there will be a connector between it and
	//  the "InterfacePointer" item in the parent node.
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item["ParentInterfacePointer"].Value.Get(pvDsp);
	i_pDspParent = (IDispatch*)(long)pvDsp.Data.DoubleData;


	// Check to see that our object still exists
	if (CheckServerAlive(i_pDspParent) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	if ((ai_iLongArg<1)||(CheckParamString(&ai_pvArg[0])==M4_ERROR)) 
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - Parameter Name is not a String");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	CComBSTR bstrMethod(ai_pvArg[0].Data.PointerChar);

	if (ai_iLongArg>1)
		vaArgList = new VARIANTARG[ai_iLongArg-1];
	DISPPARAMS dpParams = {vaArgList, NULL, ai_iLongArg-1, 0};

	for (m4uint32_t i=1; i < ai_iLongArg; i++)
	{
		M4VarToOLEVar(ai_pvArg[i], &vaArgList[(ai_iLongArg-1)-i]);
	}
	// Get DISPID for invocation
	hr = i_pDspParent->GetIDsOfNames( IID_NULL, &bstrMethod, 1, M4_DEFAULT_LCID, &dispid);
	if (FAILED(hr)) 
	{
		if (ai_iLongArg>1)
			ReleaseParamBSTR(vaArgList,ai_iLongArg-1);
		delete [] vaArgList;
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG,"#*s1*#%s#",ai_pvArg[0].Data.PointerChar);
		TRACERETVAL(OleExec.dll:GetObjectFromMethod, M4_ERROR);
	}

	// Invocation
	hr = i_pDspParent->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, DISPATCH_METHOD|DISPATCH_PROPERTYGET,
		&dpParams, &vChild, NULL, NULL);
	if (FAILED(hr)) 
	{
		if (ai_iLongArg>1)
			ReleaseParamBSTR(vaArgList,ai_iLongArg-1);
		delete [] vaArgList;
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, ai_pvArg[0].Data.PointerChar,"");
		//TRACERETVAL(OleExec.dll:GetObjectFromMethod, M4_ERROR);
		pvDsp.Type = M4CL_CPP_TYPE_NUMBER;
		pvDsp.Data.DoubleData = (double) (long) NULL;

	// Store the new interface pointer back in the channel
		(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Set(pvDsp);
		TRACERETVAL(OleExec.dll:GetObjectFromMethod, M4_SUCCESS);
	}
    else 
    {
	i_pDspChild = vChild.pdispVal;
	//hr = i_pDspParent->Release();
	pvDsp.Type = M4CL_CPP_TYPE_NUMBER;
	pvDsp.Data.DoubleData = (double) (long) i_pDspChild;

	// Store the new interface pointer back in the channel
	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Set(pvDsp);
	if (ai_iLongArg>1)
			ReleaseParamBSTR(vaArgList,ai_iLongArg-1);
	delete [] vaArgList;
	PutError(pvmContext,M4_SUCCESS);
	TRACERETVAL(OleExec.dll:GetObjectFromMethod, M4_SUCCESS);
	}
}

m4return_t CheckServerAlive(IDispatch* ai_iDsp)
{
	UINT	uiIgnore;
	HRESULT	hr = ERROR_SUCCESS;

	if(ai_iDsp == 0)
		return M4_ERROR;
	try
	{
	hr = ai_iDsp->GetTypeInfoCount(&uiIgnore);
	return ((hr == 0x800706BA) ? M4_ERROR : M4_SUCCESS);
	}
	catch (...)
	{
		return M4_ERROR;
	}
}
m4return_t GetObjectInstance(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	GUID				rclsid;
	IUnknown*		i_pUnk;
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;

	TRACESUB(OleExec.dll:GetObjectInstance);

	// Check argument
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	if ((ai_iLongArg != 1) || (ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR))
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INIT_ERROR, ERRORLOG, "OleExec - Error in GetObjectInstance (invalid arguments)");
		TRACERETVAL(OleExec.dll:GetObjectInstance, M4_ERROR);
	}

	m4bool_t bRet = CheckExecutionPermission( pvmContext, ai_pvArg[0].Data.PointerChar );
	if( bRet == M4_FALSE )
	{
		PutError( pvmContext, M4_ERROR );
		SETCODEF( M4_OBJ_SRV_COM_APP_LOCKED, ERRORLOG,"#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
		TRACERETVAL( OleExec.dll:ExecuteCom, M4_SUCCESS );
	}

	CComBSTR bstrClassName(ai_pvArg[0].Data.PointerChar);
	
	// Starts COM
	hr = CoInitialize(NULL);

	// Get Global ID of object
	hr = CLSIDFromProgID(bstrClassName, &rclsid);

	if (FAILED(hr))
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_NO_CLSID, ERRORLOG, "#*s1*#%s#",ai_pvArg[0].Data.PointerChar);
		TRACERETVAL(OleExec.dll:GetObjectInstance, M4_SUCCESS);
    }

	// Create object
	hr = GetActiveObject(rclsid, NULL, (IUnknown**)&i_pUnk);
	if (FAILED(hr))
	{
		PutError(pvmContext,hr);
		SETCODE(M4_OBJ_OLE_CREATE_FAILED, ERRORLOG, "OleExec - could not create object");
		TRACERETVAL(OleExec.dll:GetObjectInstance, M4_SUCCESS);
   }

	// Query to see if object supports Automation
	hr = i_pUnk->QueryInterface(IID_IDispatch, (void**)&i_pDsp);

	if (hr == E_NOINTERFACE )
	{
		PutError(pvmContext,hr);
		SETCODE(M4_OBJ_OLE_NO_AUTOMATION, ERRORLOG, "OleExec - object does not support automation");
		TRACERETVAL(OleExec.dll:GetObjectInstance, M4_SUCCESS);
   }

	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER; 
	ai_pvReturn.Data.DoubleData = (double) (long)i_pDsp;


	hr = i_pUnk->Release();
	PutError(pvmContext,M4_SUCCESS);
	TRACERETVAL(OleExec.dll:GetObjectInstance, M4_SUCCESS);
}

m4return_t InitialiceCom(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	GUID				rclsid;
	IUnknown*		i_pUnk;
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;

	TRACESUB(OleExec.dll:Initialise);

	// bg 179007
	ClMutBlock oBlock(&g_oMutex);

	// bg 234784. Avoid return uninitialized value.
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = 0;	

	// Check argument
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	if ((ai_iLongArg != 1) || (ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR))
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INIT_ERROR, ERRORLOG, "OleExec - Error in Initialise (invalid arguments)");
		TRACERETVAL(OleExec.dll:Initialise, M4_ERROR);
	}

	m4bool_t bRet = CheckExecutionPermission( pvmContext, ai_pvArg[0].Data.PointerChar );
	if( bRet == M4_FALSE )
	{
		PutError( pvmContext, M4_ERROR );
		SETCODEF( M4_OBJ_SRV_COM_APP_LOCKED, ERRORLOG,"#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
		TRACERETVAL( OleExec.dll:ExecuteCom, M4_SUCCESS );
	}

	CComBSTR bstrClassName(ai_pvArg[0].Data.PointerChar);
	
	// Starts COM
	hr = CoInitialize(NULL);

	// Get Global ID of object
	hr = CLSIDFromProgID(bstrClassName, &rclsid);

	if (FAILED(hr))
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_NO_CLSID, ERRORLOG, "#*s1*#%s#",ai_pvArg[0].Data.PointerChar);
		TRACERETVAL(OleExec.dll:Initialise, M4_SUCCESS);
    }

	// Create object
	hr = CoCreateInstance(rclsid, NULL, CLSCTX_SERVER, IID_IUnknown, (void**)&i_pUnk);

	if (FAILED(hr))
	{
		PutError(pvmContext,hr);
		SETCODE(M4_OBJ_OLE_CREATE_FAILED, ERRORLOG, "OleExec - could not create object");
		TRACERETVAL(OleExec.dll:Initialise, M4_SUCCESS);
   }

	// Query to see if object supports Automation
	hr = i_pUnk->QueryInterface(IID_IDispatch, (void**)&i_pDsp);

	if (hr == E_NOINTERFACE )
	{
		PutError(pvmContext,hr);
		SETCODE(M4_OBJ_OLE_NO_AUTOMATION, ERRORLOG, "OleExec - object does not support automation");
		TRACERETVAL(OleExec.dll:Initialise, M4_SUCCESS);
   }

	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER; 
	ai_pvReturn.Data.DoubleData = (double) (long)i_pDsp;


	hr = i_pUnk->Release();
	PutError(pvmContext,M4_SUCCESS);
	TRACERETVAL(OleExec.dll:Initialise, M4_SUCCESS);
}

m4return_t ExecuteCom(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	DISPID			dispid;
	VARIANTARG*		vaArgList = NULL;
	VARIANT			vReturn;
	VARIANT*		pvReturn;
	bool			bReturn; 
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;
	UINT			iArgErr;
	EXCEPINFO		excepinfo;

	TRACESUB(OleExec.dll:OLE_Execute);

	// bg 179007
	ClMutBlock oBlock(&g_oMutex);

	// bg 234784. Avoid return uninitialized value.
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = 0;	

	// First we retrieve the pointer to the IDispatch pointer we
	// have stored in the channel in the "InterfacePointer" item
	// We use the ai_pvContext to access the channel
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	//m4pcchar_t pccMethod = ((pvmContext->m_pAccess->GetpChannel())->GetpCMCR())->GetItemId(pvmContext->m_hItem);
	//bReturn = ((pvmContext->m_pRecordSet)->Current.Item[ClHandle(pvmContext->m_hItem)].ItemDef.M4Type() != 0);
	bReturn = 1;
	::VariantInit(&vReturn);
	vReturn.vt = VT_R8;
	vReturn.dblVal = (double)0;
	pvReturn = bReturn ? &vReturn : NULL;
	if ((ai_iLongArg<2)||(CheckParamString(&ai_pvArg[1])==M4_ERROR)) 
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - Parameter Name is not a String");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	CComBSTR bstrMethod(ai_pvArg[1].Data.PointerChar);

	// Check server clipboard access
	// Eg. COM!COM.EXECUTE
	if( !strcmpi( ai_pvArg[1].Data.PointerChar, "COPY" ) || !strcmpi( ai_pvArg[1].Data.PointerChar, "PASTE" ) )
	{
		m4bool_t bRet = CheckLockingClipboardOperation();
		
		if( bRet == M4_FALSE )
		{
			SETCODEF( M4_OBJ_OLE_CB_OPR_LOCKED, ERRORLOG,"#*s1*#%s#", ai_pvArg[1].Data.PointerChar );
			TRACERETVAL( OleExec.dll:ExecuteCom, M4_SUCCESS );
		}
	}

	//(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);

	
	try
	{
	i_pDsp = (IDispatch*)(long)ai_pvArg[0].Data.DoubleData;
	}
	catch( ...)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_SUCCESS);
	}
	
	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_SUCCESS);
	}

	// Create OLE argument list
	if (ai_iLongArg > 2)
		vaArgList = new VARIANTARG[ai_iLongArg - 2];

	DISPPARAMS dpParams = {vaArgList, NULL, ai_iLongArg - 2, 0};
	memset(&excepinfo, 0, sizeof excepinfo);

	// initialize return value
	//pvReturn = NULL;
	//AfxVariantInit(&vReturn);
	//if (vtRet != VT_EMPTY)
	//	pvReturn = &vReturn;
	iArgErr = (UINT)-1;
	

	// Get DISPID to do the invocation
	hr = i_pDsp->GetIDsOfNames(IID_NULL, &bstrMethod, 1, M4_DEFAULT_LCID, &dispid);

	if (FAILED(hr)) 
	{
		delete [] vaArgList;
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG, "#*s1*#%s#", ai_pvArg[1].Data.PointerChar);
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_SUCCESS);
	}

	m4return_t ret=ChangeParamWithTypeInfo(&ai_pvArg[2],ai_iLongArg-2,vaArgList,i_pDsp,dispid,INVOKE_FUNC);
	// Convert arguments from M4 variants to OLE variants
	if(ret==M4_ERROR)
	for (m4uint32_t i = 0; i < ai_iLongArg -2; i++)
	{
		M4VarToOLEVar(ai_pvArg[i + 2], &vaArgList[(ai_iLongArg-3)-i]);
	}

	// Invocation
	hr = i_pDsp->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, DISPATCH_METHOD, &dpParams, pvReturn, &excepinfo, &iArgErr);

	char strErr[1000];
	strErr[0] = NULL;

	if (FAILED(hr)) 
	{
		if(hr == DISP_E_EXCEPTION)
		{
			if(excepinfo.pfnDeferredFillIn != NULL)
				(*(excepinfo.pfnDeferredFillIn)) (&excepinfo);

			USES_CONVERSION;
			wsprintf(strErr,"Error:  %s.",OLE2T(excepinfo.bstrDescription));
		}

		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, ai_pvArg[1].Data.PointerChar, strErr);
		//TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}

	if (bReturn)
		OLEVarToM4Var(vReturn, &ai_pvReturn);
	if (ai_iLongArg>2)
			ReleaseParamBSTR(vaArgList,ai_iLongArg-2);
	ReleaseParamBSTR(&vReturn,1);
	delete [] vaArgList;
	// Error Mangment JGM
	PutError(pvmContext,hr);
	TRACERETVAL(OleExec.dll:OLE_Execute, M4_SUCCESS);
}

m4return_t PropertyCom(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	DISPID			dispid;
	VARIANTARG*		vaArgList = NULL;
	VARIANT			vRetVal;
	VARIANT*			pvRetVal;
	WORD				wFlags;
	DISPPARAMS		dpParams;
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;

	TRACESUB(OleExec.dll:OLE_Property);

	// bg 179007
	ClMutBlock oBlock(&g_oMutex);

	// bg 234784. Avoid return uninitialized value.
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = 0;	

	// First we retrieve the pointer to the IDispatch pointer we
	// have stored in the channel in the "InterfacePointer" item
	// We use the ai_pvContext to access the channel
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	//m4pcchar_t pccProperty = ((pvmContext->m_pAccess->GetpChannel())->GetpCMCR())->GetItemId(pvmContext->m_hItem);
	if ((ai_iLongArg<2)||(CheckParamString(&ai_pvArg[1])==M4_ERROR)) 
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - Parameter Name is not a String");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	CComBSTR bstrProperty(ai_pvArg[1].Data.PointerChar);
	//(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
	
	try
	{
		i_pDsp = (IDispatch*)(long)ai_pvArg[0].Data.DoubleData;
	}
	catch( ...)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_SUCCESS);
	}
	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}

	// Get DISPID for invocation
	hr = i_pDsp->GetIDsOfNames(IID_NULL, &bstrProperty, 1, M4_DEFAULT_LCID, &dispid);

	if (FAILED(hr)) 
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG, "#*s1*#%s#", ai_pvArg[1].Data.PointerChar);
		TRACERETVAL(OleExec.dll:OLE_Property, M4_ERROR);
	}

	// If we receive an argument then we do a set otherwise we do a get
	if (ai_iLongArg == 3)
	{
		VARIANT vNewValue;
		DISPID dspNamedArg = DISPID_PROPERTYPUT;
		M4VarToOLEVar(ai_pvArg[2], &vNewValue);
		dpParams.rgvarg = &vNewValue;
		dpParams.rgdispidNamedArgs = &dspNamedArg;
		dpParams.cArgs = 1;
		dpParams.cNamedArgs = 1;
		wFlags = DISPATCH_PROPERTYPUT;
		pvRetVal = NULL;
	}
	else  //  ai_iLongArg == 0
	{
		dpParams.rgvarg = NULL;
		dpParams.rgdispidNamedArgs = NULL;
		dpParams.cArgs = 0;
		dpParams.cNamedArgs = 0;
		wFlags = DISPATCH_PROPERTYGET;
		pvRetVal = &vRetVal;
	}

	// Invocation
	hr = i_pDsp->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, wFlags, &dpParams, pvRetVal, NULL, NULL);

	if (FAILED(hr)) 
	{
		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, ai_pvArg[1].Data.PointerChar,"");
		//TRACERETVAL(OleExec.dll:OLE_Property, M4_ERROR);
	}

	// If we are doing a get then fill in return value
	if (ai_iLongArg == 2)
		OLEVarToM4Var(vRetVal, &ai_pvReturn);

	delete [] vaArgList;
    // Error Mangment JGM
		PutError(pvmContext,hr);
	return M4_SUCCESS;
}

m4return_t  ReleaseCom (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	IDispatch*		i_pDsp; 
	m4VariantType	pvDsp;

	TRACESUB(OleExec.dll:Release);
	
	// bg 179007
	ClMutBlock oBlock(&g_oMutex);

	// bg 234784. Avoid return uninitialized value.
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = 0;	

	// Get interface pointer
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	//(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
    
	try
	{
	i_pDsp = (IDispatch*)(long)ai_pvArg[0].Data.DoubleData;
	}
	catch( ...)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_SUCCESS);
	}
	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_SUCCESS);
	}
		

	// Do release

	hr = i_pDsp->Release();
	if(hr == 0)
	{
		i_pDsp = NULL;

		// Store object pointer in channel to be retrieved by subsequent calls
		ai_pvArg[0].Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvArg[0].Data.DoubleData = NULL;
	}
	//CoUninitialize();
	PutError(pvmContext,M4_SUCCESS);
	TRACERETVAL(OleExec.dll:Release, M4_SUCCESS);
}

m4return_t GetPropertyCom(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	DISPID			dispid;
	VARIANTARG*		vaArgList = NULL;
	VARIANT			vRetVal;
	WORD				wFlags;
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;
	EXCEPINFO		excepinfo;
	UINT			iArgErr;

	TRACESUB(OleExec.dll:OLE_GetPropertyCom);

	// bg 179007
	ClMutBlock oBlock(&g_oMutex);

	// bg 234784. Avoid return uninitialized value.
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = 0;	

	// First we retrieve the pointer to the IDispatch pointer we
	// have stored in the channel in the "InterfacePointer" item
	// We use the ai_pvContext to access the channel
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	//m4pcchar_t pccProperty = ((pvmContext->m_pAccess->GetpChannel())->GetpCMCR())->GetItemId(pvmContext->m_hItem);
	if ((ai_iLongArg<2)||(CheckParamString(&ai_pvArg[1])==M4_ERROR)) 
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - Parameter Name is not a String");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	CComBSTR bstrProperty(ai_pvArg[1].Data.PointerChar);
	//(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
	//i_pDsp = (IDispatch*)(long)ai_pvArg[0].Data.DoubleData;
	try
	{
	i_pDsp = (IDispatch*)(long)ai_pvArg[0].Data.DoubleData;
	}
	catch( ...)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_SUCCESS);
	}
	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_SUCCESS);
	}

	// Get DISPID for invocation
	hr = i_pDsp->GetIDsOfNames(IID_NULL, &bstrProperty, 1, M4_DEFAULT_LCID, &dispid);

	if (FAILED(hr)) 
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG,"#*s1*#%s#", ai_pvArg[1].Data.PointerChar);
		TRACERETVAL(OleExec.dll:OLE_GetProperty, M4_SUCCESS);
	}

	// If we receive an argument then we do a set otherwise we do a get
	if (ai_iLongArg > 2)
		vaArgList = new VARIANTARG[ai_iLongArg - 2];

	DISPPARAMS dpParams = {vaArgList, NULL, ai_iLongArg - 2, 0};

	wFlags = DISPATCH_PROPERTYGET;
	::VariantInit(&vRetVal);
	m4return_t ret=ChangeParamWithTypeInfo(&ai_pvArg[2],ai_iLongArg-2,vaArgList,i_pDsp,dispid,INVOKE_PROPERTYGET);
	if(ret==M4_ERROR)
	for (m4uint32_t i = 0; i < ai_iLongArg -2; i++)
	{
		M4VarToOLEVar(ai_pvArg[i + 2], &vaArgList[(ai_iLongArg-3)-i]);
	}


	memset(&excepinfo, 0, sizeof excepinfo);
	iArgErr = (UINT)-1;

	// Invocation
	hr = i_pDsp->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, wFlags, &dpParams, &vRetVal, &excepinfo, &iArgErr);

	char strErr[1000];
	strErr[0] = NULL;

	if (FAILED(hr)) 
	{
		if(hr == DISP_E_EXCEPTION)
		{
			if(excepinfo.pfnDeferredFillIn != NULL)
				(*(excepinfo.pfnDeferredFillIn)) (&excepinfo);

			USES_CONVERSION;
			wsprintf(strErr,"Error:  %s.",OLE2T(excepinfo.bstrDescription));
		}

		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, ai_pvArg[1].Data.PointerChar,strErr);
		//TRACERETVAL(OleExec.dll:OLE_Property, M4_ERROR);
	}

	// If we are doing a get then fill in return value
	//if (ai_iLongArg == 2)
	OLEVarToM4Var(vRetVal, &ai_pvReturn);
	PutError(pvmContext,hr);
	ReleaseParamBSTR(&vRetVal,1);
	ReleaseParamBSTR(vaArgList,ai_iLongArg - 2);
	delete [] vaArgList;
	return M4_SUCCESS;
}

m4return_t PutPropertyCom(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	DISPID			dispid;
	VARIANTARG*		vaArgList = NULL;
	VARIANT*			pvRetVal;
	WORD				wFlags;
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;
	EXCEPINFO		excepinfo;
	UINT			iArgErr;

	TRACESUB(OleExec.dll:OLE_PutPropertyCom);

	// bg 179007
	ClMutBlock oBlock(&g_oMutex);

	// bg 234784. Avoid return uninitialized value.
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = 0;	

	// First we retrieve the pointer to the IDispatch pointer we
	// have stored in the channel in the "InterfacePointer" item
	// We use the ai_pvContext to access the channel
	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	//m4pcchar_t pccProperty = ((pvmContext->m_pAccess->GetpChannel())->GetpCMCR())->GetItemId(pvmContext->m_hItem);
	if ((ai_iLongArg<2)||(CheckParamString(&ai_pvArg[1])==M4_ERROR)) 
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - Parameter Name is not a String");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_ERROR);
	}
	CComBSTR bstrProperty(ai_pvArg[1].Data.PointerChar);
	//(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
	//i_pDsp = (IDispatch*)(long)ai_pvArg[0].Data.DoubleData;
    try
	{
		i_pDsp = (IDispatch*)(long)ai_pvArg[0].Data.DoubleData;
	}
	catch( ...)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_Execute, M4_SUCCESS);
	} 
	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_PutProtperty, M4_SUCCESS);
	}

	// Get DISPID for invocation
	hr = i_pDsp->GetIDsOfNames(IID_NULL, &bstrProperty, 1, M4_DEFAULT_LCID, &dispid);

	if (FAILED(hr)) 
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG,"#*s1*#%s#", ai_pvArg[1].Data.PointerChar);
		TRACERETVAL(OleExec.dll:OLE_PutProperty, M4_SUCCESS);
	}

	// If we receive an argument then we do a set otherwise we do a get
	vaArgList = new VARIANTARG[ai_iLongArg - 2];
	DISPPARAMS dpParams = {vaArgList, NULL, ai_iLongArg - 2, 1};

	wFlags = DISPATCH_PROPERTYPUT;
	DISPID dspNamedArg = DISPID_PROPERTYPUT;
	dpParams.rgvarg = &vaArgList[0];
	dpParams.rgdispidNamedArgs = &dspNamedArg;
	pvRetVal = NULL;
	m4return_t ret=ChangeParamWithTypeInfo(&ai_pvArg[2],ai_iLongArg-2,vaArgList,i_pDsp,dispid,INVOKE_PROPERTYPUT);
	if(ret==M4_ERROR)
	for (m4uint32_t i = 0; i < ai_iLongArg -2; i++)
	{
		M4VarToOLEVar(ai_pvArg[i + 2], &vaArgList[(ai_iLongArg-3)-i]);
	}


	memset(&excepinfo, 0, sizeof excepinfo);
	iArgErr = (UINT)-1;

	// Invocation
	hr = i_pDsp->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, wFlags, &dpParams, pvRetVal, &excepinfo, &iArgErr);

	char strErr[1000];
	strErr[0] = NULL;

	if (FAILED(hr)) 
	{
		if(hr == DISP_E_EXCEPTION)
		{
			if(excepinfo.pfnDeferredFillIn != NULL)
				(*(excepinfo.pfnDeferredFillIn)) (&excepinfo);

			USES_CONVERSION;
			wsprintf(strErr,"Error:  %s.",OLE2T(excepinfo.bstrDescription));
		}

		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, ai_pvArg[1].Data.PointerChar,strErr);
//		TRACERETVAL(OleExec.dll:OLE_Property, M4_ERROR);
	}

	PutError(pvmContext,hr);
	ReleaseParamBSTR(vaArgList,ai_iLongArg-2);
	delete [] vaArgList;
	return M4_SUCCESS;
}



m4return_t PasteSpecialText(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	DISPID			dispid;
	VARIANT			vReturn;
	VARIANT*		pvReturn;
	bool			bReturn; 
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;
	UINT			iArgErr;
	EXCEPINFO		excepinfo;
	BOOL			bRet = false;
	HANDLE			hMemSaved = 0;
	m4int_t			imbDataLen;
	WCHAR*			pwcData;
	HANDLE			hMem;
	HANDLE			pMem;
	HANDLE			pMem2;
	HANDLE			hRet;
	m4pcchar_t		pccMethod = "PasteSpecial";
	CComBSTR		bstrMethod(pccMethod);
	char			strErr[1000];

	// bg 257032
	ClSemBlock oCBSemBlock( &g_oSem );

	TRACESUB(OleExec.dll:PasteSpecialText);

	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;

	// -- Check arguments.
	if ( (ai_iLongArg < 1) || (CheckParamString( &ai_pvArg[0] ) == M4_ERROR) )
	{
		PutError( pvmContext, M4_ERROR );
		SETCODE( M4_OBJ_OLE_INVALID_ARGUMENT, ERRORLOG, "PasteSpecialText" );
		TRACERETVAL( OleExec.dll:PasteSpecialText, M4_ERROR );
	}


	// -- Setting the data into the clipboard 
	bRet = OpenClipboard( GetDesktopWindow() );

	if( bRet != 0 )
	{
		// -- Get current text clipboard
		hMem = GetClipboardData( CF_UNICODETEXT );

		// -- Save a copy of the current clipboard
		if( hMem != 0 )
		{
			hMemSaved = GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, GlobalSize(hMem) );
			
			pMem = GlobalLock( hMem );
			pMem2 = GlobalLock( hMemSaved );

			memcpy( pMem2, pMem, GlobalSize(hMem) );

			GlobalUnlock( hMem );
			GlobalUnlock( hMemSaved );
		}

		// -- Empty clipboard
		bRet = EmptyClipboard();

		if( bRet != 0 )
		{
			imbDataLen = strlen( ai_pvArg[0].Data.PointerChar );
			
			pwcData = (WCHAR*) M4CppToUtf16( ai_pvArg[0].Data.PointerChar, imbDataLen );

			hMem = GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, (imbDataLen+1) * sizeof(WCHAR) );
			pMem = GlobalLock( hMem );

			memcpy( pMem, pwcData, imbDataLen * sizeof(WCHAR) );

			GlobalUnlock( hMem );

			// -- Set argument text into clipboard as UTF16 string
			hRet = SetClipboardData( CF_UNICODETEXT, hMem );

			bRet = CloseClipboard();

			delete[] pwcData;
			GlobalFree(hMem);

			if( hRet == 0 )
			{
				formatErrorMsg( GetLastError(), strErr, sizeof(strErr) );
				PutError( pvmContext,M4_ERROR );
				SETCODEF( M4_OBJ_OLE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", "SetClipboardData", strErr );
				
				restoreClipboard( hMemSaved );

				TRACERETVAL( OleExec.dll:PasteSpecialText, M4_ERROR );
			}

			if( bRet == 0 )
			{
				formatErrorMsg( GetLastError(), strErr, sizeof(strErr) );
				PutError( pvmContext,M4_ERROR );
				SETCODEF( M4_OBJ_OLE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", "CloseClipboard", strErr );

				restoreClipboard( hMemSaved );

				TRACERETVAL( OleExec.dll:PasteSpecialText, M4_ERROR );
			}
		}
		else
		{
			formatErrorMsg( GetLastError(), strErr, sizeof(strErr) );
			PutError( pvmContext,M4_ERROR );
			SETCODEF( M4_OBJ_OLE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", "EmptyClipboard", strErr );
			CloseClipboard();

			restoreClipboard( hMemSaved );

			TRACERETVAL( OleExec.dll:PasteSpecialText, M4_ERROR );
		}
	}
	else
	{
		formatErrorMsg( GetLastError(), strErr, sizeof(strErr) );
		PutError( pvmContext,M4_ERROR );
		SETCODEF( M4_OBJ_OLE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", "OpenClipboard", strErr );
		TRACERETVAL( OleExec.dll:PasteSpecialText, M4_ERROR );
	}


	// -- Check if calling method must return a value.
	bReturn = ((pvmContext->m_pRecordSet)->Current.Item[ClHandle(pvmContext->m_hItem)].ItemDef.M4Type() != 0);
	VariantInit(&vReturn);
	pvReturn = bReturn ? &vReturn : NULL;

	// -- Get current interface pointer.
	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
	i_pDsp = (IDispatch*)(long)pvDsp.Data.DoubleData;

	// -- Check if our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:PasteSpecialText, M4_ERROR);
	}

	// -- Get DISPID to do the invocation
	hr = i_pDsp->GetIDsOfNames(IID_NULL, &bstrMethod, 1, M4_DEFAULT_LCID, &dispid);

	if (FAILED(hr)) 
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG, "OleExec - OLE method \"%s\" name not recognized", pccMethod);
		TRACERETVAL(OleExec.dll:PasteSpecialText, M4_ERROR);
	}

	// bg 155058
	DISPPARAMS dpParams = {NULL, NULL, 0, 0};
	

	memset(&excepinfo, 0, sizeof excepinfo);
	iArgErr = (UINT)-1;
	strErr[0] = NULL;


	// -- Invocation
	hr = i_pDsp->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, DISPATCH_METHOD, &dpParams, pvReturn, &excepinfo, &iArgErr);

	if (FAILED(hr)) 
	{
		if(hr == DISP_E_EXCEPTION) 
		{
			if(excepinfo.pfnDeferredFillIn != NULL)
				(*(excepinfo.pfnDeferredFillIn)) (&excepinfo);

			USES_CONVERSION;
			if(excepinfo.bstrDescription)
				wsprintf(strErr,"Error:  %s.",OLE2T(excepinfo.bstrDescription));
		}

		SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, pccMethod, strErr);
	}
	
	// -- Restore previous text clipboard
	restoreClipboard( hMemSaved );

	if (bReturn)
		OLEVarToM4Var(vReturn, &ai_pvReturn);

	ReleaseParamBSTR(&vReturn,1);
	PutError(pvmContext,hr);

	TRACERETVAL(OleExec.dll:PasteSpecialText, M4_SUCCESS);
}

m4bool_t restoreClipboard( HANDLE ai_hMem )
{
	BOOL	bRet = true;
	HANDLE	hRet;
	char	acErrorMessage[1000];


	if( ai_hMem != 0 )
	{
		bRet = OpenClipboard( GetDesktopWindow() );

		if( bRet != 0 )
		{
			hRet = SetClipboardData( CF_UNICODETEXT, ai_hMem );

			if( hRet == 0 )
			{
				formatErrorMsg( GetLastError(), acErrorMessage, sizeof(acErrorMessage) );
				SETCODEF( M4_OBJ_OLE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", "SetClipboardData", acErrorMessage );
			}
			
			bRet = CloseClipboard();

			if( bRet == 0 )
			{
				formatErrorMsg( GetLastError(), acErrorMessage, sizeof(acErrorMessage) );
				SETCODEF( M4_OBJ_OLE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", "CloseClipboard", acErrorMessage );
			}
		}
		else
		{
			formatErrorMsg( GetLastError(), acErrorMessage, sizeof(acErrorMessage) );
			SETCODEF( M4_OBJ_OLE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", "OpenClipboard", acErrorMessage );
		}

		GlobalFree(ai_hMem);
	}


	return bRet;
}


// -- Dump Excel range to channel instance.

#define MAX_XLS_COLUMNS 16384
#define XLS_COLUMN_WIDTH 4

// --  Fills an array where can get the Excel column name given its index.
static m4return_t FillXlsColumnNames( m4pchar_t ao_pcXlsColNames, m4int_t  ai_numCols )
{
	m4pchar_t	pcColumn = NULL ;
	int			iRange1c, iRange2c ;

	memset( ao_pcXlsColNames, 0, ai_numCols * XLS_COLUMN_WIDTH ) ;

	for( int iIdx = 1; iIdx <= ai_numCols; iIdx++ )
	{
		pcColumn = ( m4pchar_t ) ao_pcXlsColNames + ( (iIdx-1) * XLS_COLUMN_WIDTH ) ;

		// -- Column is in the A to Z range
		if( iIdx <= 26 )
		{
			*pcColumn = 64 + iIdx ; 
		}
		else if( iIdx <= 702 )
		{
			// -- Column is in the AA to ZZ range.
			iRange1c   = ( int )( iIdx / 26.0 ) ;
			*pcColumn++	= ( !(iIdx % 26) ) ? 63 + iRange1c : 64 + iRange1c ;
			*pcColumn	= ( !(iIdx % 26) ) ? 90 : 64 + ( iIdx % 26 ) ;
		}
		else
		{
			// -- Column is in the AAA to XFD range.
			iRange1c   = ( int )( (iIdx - 702) / 676.0 ) ; // 702 is A to ZZ and 676 is AAA to AZZ
			iRange2c   = ( int )( ( (iIdx - 702) % 676 ) / 26.0 ) ; 

			*pcColumn++	= ( !( (iIdx - 702) % 676 ) ) ? 64 + iRange1c : 65 + iRange1c ;
			*pcColumn++	= ( !( (iIdx - 702) % 676 )  ) ? 90 : ( !( ( (iIdx - 702) % 676 ) % 26 ) ) ? 64 + iRange2c : 65 + iRange2c ;
			*pcColumn	= ( !(iIdx % 26) ) ? 90 : 64 + ( iIdx % 26 ) ;
		}
	}

	return M4_SUCCESS ;
}

// Fills an array where can be checked if a column in a range is really of a string type.
static m4return_t GetXlsRangeColumnTypes( m4pvoid_t ai_pvContext, m4int_t ai_iStartCol, m4int_t ai_iStartRow, m4int_t ai_iRangeWidth, VARTYPE * ao_piXlsColTypes )
{
	HRESULT			hr = ERROR_SUCCESS ;
	m4VariantType	pvDsp ;
	IDispatch*		i_pDspParent ;
	DISPID			dispidCells ;
	VARIANTARG*		vaArgList = NULL ;
	VARIANT			vChild ;
	IDispatch*		i_pDspChild ;
	DISPID			dispidNFormat ;
	DISPPARAMS		dpNFormatParams ;
	VARIANT			vRetVal ;
	VARIANT*		pvRetVal ;
	WORD			wFlags ;
	EXCEPINFO		excepinfo ;
	char			strErr[ 1000 ] ;
	m4int_t			i = 0;
	
	// Initialize output data.
	for( i = 0; i < ai_iStartCol + ai_iRangeWidth - 1; i++ )
	{
		ao_piXlsColTypes[ i ] = VT_EMPTY ;
	}

	ClVMRunContext*	pvmContext = (ClVMRunContext*) ai_pvContext ;

	// Parent pointer is the worksheets node.
	(pvmContext->m_pRecordSet)->Current.Item["ParentInterfacePointer"].Value.Get( pvDsp ) ;
	i_pDspParent = (IDispatch*)(long) pvDsp.Data.DoubleData ;

	// Check to see that our object still exists.
	if( CheckServerAlive( i_pDspParent ) == M4_ERROR )
	{
		SETCODE( M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid" ) ;
		return M4_ERROR ;
	}

	
	// Get the cell objects of the first row in the range.
	CComBSTR bstrMethod( "CELLS" ) ;

	// Get DISPID for invocation
	hr = i_pDspParent->GetIDsOfNames( IID_NULL, &bstrMethod, 1, M4_DEFAULT_LCID, &dispidCells ) ;
	if( FAILED( hr ) ) 
	{
		SETCODEF( M4_OBJ_OLE_INVALID_NAME, ERRORLOG,"#*s1*#%s#", "CELLS" ) ;
		return M4_ERROR ;
	}

	
	vaArgList = new VARIANTARG[ 2 ] ;
	DISPPARAMS dpCellsParams = { vaArgList, NULL, 2, 0 } ;

	// Common arguments in all cells in the first row.
	vaArgList[ 0 ].vt = VT_R8 ;
	vaArgList[ 1 ].vt = VT_R8 ;
	vaArgList[ 1 ].dblVal = ai_iStartRow ;

	for( i = 0; i < ai_iRangeWidth; i++ )
	{
		vaArgList[ 0 ].dblVal = ai_iStartCol + i ;

		// Invocation
		hr = i_pDspParent->Invoke( dispidCells, IID_NULL, M4_DEFAULT_LCID, DISPATCH_METHOD|DISPATCH_PROPERTYGET, &dpCellsParams, &vChild, NULL, NULL ) ;

		if( FAILED( hr ) ) 
		{
			ReleaseParamBSTR( vaArgList, 2 ) ;
			delete [] vaArgList ;
			SETCODEF( M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, "Cells", "" ) ;
			return M4_ERROR ;
		}
		else 
		{
			// Pointer to current cell.
			i_pDspChild = vChild.pdispVal ;

			// Get the current cell type.
			CComBSTR bstrProperty( "NumberFormat" ) ;

			// Get DISPID for invocation
			hr = i_pDspChild->GetIDsOfNames( IID_NULL, &bstrProperty, 1, M4_DEFAULT_LCID, &dispidNFormat ) ;

			if( FAILED( hr ) ) 
			{
				ReleaseParamBSTR( vaArgList, 2 ) ;
				delete [] vaArgList ;
				i_pDspChild->Release() ;
				SETCODEF( M4_OBJ_OLE_INVALID_NAME, ERRORLOG, "#*s1*#%s#", "NumberFormat" ) ;
				return M4_ERROR ;
			}

			dpNFormatParams.rgvarg = NULL ;
			dpNFormatParams.rgdispidNamedArgs = NULL ;
			dpNFormatParams.cArgs = 0 ;
			dpNFormatParams.cNamedArgs = 0 ;
			wFlags = DISPATCH_PROPERTYGET ;
			pvRetVal = &vRetVal ;

			memset( &excepinfo, 0, sizeof excepinfo ) ;

			// Invocation
			hr = i_pDspChild->Invoke( dispidNFormat, IID_NULL, M4_DEFAULT_LCID, wFlags, &dpNFormatParams, pvRetVal, &excepinfo, NULL ) ;

			i_pDspChild->Release() ;

			if(FAILED( hr ) ) 
			{
				ReleaseParamBSTR( vaArgList, 2 ) ;
				delete [] vaArgList ;
				ReleaseParamBSTR( &vRetVal, 1 ) ;

				strErr[0] = NULL;

				if( hr == DISP_E_EXCEPTION )
				{
					if( excepinfo.pfnDeferredFillIn != NULL )
						(*(excepinfo.pfnDeferredFillIn)) (&excepinfo) ;

					USES_CONVERSION ;
					if( excepinfo.bstrDescription )
						wsprintf( strErr, "Error:  %s.", OLE2T(excepinfo.bstrDescription) ) ;
				}

				SETCODEF( M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, "NumberFormat", strErr ) ;
				return M4_ERROR ;
			}

			// Set which cells are really of a string type.
			if( vRetVal.vt == VT_BSTR && *vRetVal.pbVal == '@' )
			{
				ao_piXlsColTypes[ ai_iStartCol + i - 1 ] = vRetVal.vt ;
			}

			ReleaseParamBSTR( &vRetVal, 1 ) ;
		}
	}

	ReleaseParamBSTR( vaArgList, 2 ) ;
	delete [] vaArgList ;

	return M4_SUCCESS ;
}

// Trim a string left and right.
static m4return_t TrimValue( m4pchar_t& ao_pcValue )
{
	m4bool_t bTrimmed = M4_FALSE;

	if( ao_pcValue == NULL )
	{
		return M4_ERROR;
	}

	// Trims right.
	int	iIdx = strlen( ao_pcValue ) - 1 ;
	while( ao_pcValue[ iIdx ] == 32 )
	{
		ao_pcValue[ iIdx-- ] = '\0';
		bTrimmed = M4_TRUE;
	}

	// Trims left.
	m4pchar_t pcData = ao_pcValue;
	while( *pcData == 32 )
	{
		pcData++;
		bTrimmed = M4_TRUE;
	}

	// Sets the trimmed value.
	if( bTrimmed == M4_TRUE )
	{
		m4pchar_t pcItemData = new m4char_t[ strlen( pcData ) + 1 ];
		strcpy( pcItemData, pcData );

		delete ao_pcValue;
		ao_pcValue = pcItemData;
	}

	return M4_SUCCESS;
}

class ClObjTerminator
{
	public:
		ClObjTerminator( ClAccess*	ai_poAccess )
		{
			m_poAccess = ai_poAccess;
		}

		~ClObjTerminator()
		{
			if( m_poAccess != NULL )
			{
				m_poAccess->Destroy();
			}
		}

	private:
		ClAccess*	m_poAccess;
};

m4return_t DumpRangeToNode(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS ;
	DISPID			dispid ;
	VARIANT			vRetVal ;
	VARIANT*		pvRetVal ;
	WORD			wFlags ;
	DISPPARAMS		dpParams ;
	IDispatch*		i_pDsp ;
	m4VariantType	pvDsp ;
	EXCEPINFO		excepinfo ;
	m4pchar_t		pcXlsColumnNames ;
	long			lXlsNumCols, lXlsNumRows ;
	ClChannel*		pChannel = NULL ;
	ClAccess*		poAccess = NULL ;
	ClNode*			poNode = NULL ;
	m4VariantType	vItemValue ;
	VARIANT			vCellValue ;
	m4bool_t		bBreak = M4_FALSE ;
	m4int_t			iRangeStartCol = 0 ;
	VARTYPE*		pvXlsColumnTypes ;
	m4char_t		acDoubleValue[ 64 ] ;
	m4return_t		iRet ;
	

	TRACESUB( OleExec.dll:DumpRangeToNode );

	ClVMRunContext*  pvmContext = ( ClVMRunContext* )ai_pvContext ;

	// -- Check arguments.
	if( ai_iLongArg != 4 || ai_pvArg[0].Type != M4CL_CPP_TYPE_NUMBER || CheckParamString( &ai_pvArg[1] ) == M4_ERROR || ai_pvArg[2].Type != M4CL_CPP_TYPE_NUMBER || ai_pvArg[3].Type != M4CL_CPP_TYPE_NUMBER )
	{
		PutError( pvmContext, M4_ERROR ) ;
		SETCODE( M4_OBJ_OLE_INVALID_ARGUMENT, ERRORLOG, "DumpRangeToNode" ) ;
		TRACERETVAL( OleExec.dll:DumpRangeToNode, M4_ERROR ) ;
	}

	m4pcchar_t pccProperty = "Value" ;
	CComBSTR bstrProperty( pccProperty ) ;
	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get( pvDsp ) ;
	i_pDsp = ( IDispatch* )( long )pvDsp.Data.DoubleData ;

	// -- Check to see that our object still exists
	if( CheckServerAlive( i_pDsp ) == M4_ERROR )
	{
		PutError( pvmContext, M4_ERROR ) ;
		SETCODE( M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid" ) ;
		TRACERETVAL( OleExec.dll:DumpRangeToNode, M4_ERROR ) ;
	}

	// -- Get DISPID for invocation
	hr = i_pDsp->GetIDsOfNames( IID_NULL, &bstrProperty, 1, M4_DEFAULT_LCID, &dispid ) ;

	if( FAILED( hr ) ) 
	{
		PutError( pvmContext, hr ) ;
		SETCODEF( M4_OBJ_OLE_INVALID_NAME, ERRORLOG, "OleExec - OLE property \"%s\" name not recognized", pccProperty ) ;
		TRACERETVAL( OleExec.dll:DumpRangeToNode, M4_ERROR ) ;
	}

	// -- Get operation. This method will retrieve an array with all the cells of the range.
	dpParams.rgvarg				= NULL ;
	dpParams.rgdispidNamedArgs	= NULL ;
	dpParams.cArgs				= 0 ;
	dpParams.cNamedArgs			= 0 ;
	wFlags		= DISPATCH_PROPERTYGET ;
	pvRetVal	= &vRetVal ;

	memset( &excepinfo, 0, sizeof excepinfo ) ;
	char strErr[1000];
	strErr[0] = NULL;

	// -- Invocation.
	hr = i_pDsp->Invoke( dispid, IID_NULL, M4_DEFAULT_LCID, wFlags, &dpParams, pvRetVal, &excepinfo, NULL ) ;

	if( FAILED( hr ) ) 
	{
		if( hr == DISP_E_EXCEPTION )
		{
			if(excepinfo.pfnDeferredFillIn != NULL)
				(*(excepinfo.pfnDeferredFillIn)) (&excepinfo);

			USES_CONVERSION;
			if(excepinfo.bstrDescription)
				wsprintf(strErr,"Error:  %s.",OLE2T(excepinfo.bstrDescription));
		}
		PutError( pvmContext, hr ) ;
		SETCODEF( M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, pccProperty, strErr ) ;
		TRACERETVAL( OleExec.dll:DumpRangeToNode, M4_ERROR ) ;
	}


	// -- Checks results format.
	if( vRetVal.vt != ( VT_ARRAY | VT_VARIANT ) )
	{
		PutError( pvmContext, M4_ERROR ) ;
		SETCODEF( M4_OBJ_OLE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", "DumpRangeToNode", "Invalid results format" ) ;
		TRACERETVAL( OleExec.dll:DumpRangeToNode, M4_ERROR ) ;
	}


	// -- Get the target channel instance.
	pChannel = pvmContext->m_pAccess->GetpChannel()->GetpChannelManager()->GetChannelByHandle( ai_pvArg[0].Data.DoubleData ) ;

	if( pChannel == NULL )
	{
		PutError( pvmContext, M4_ERROR ) ;
		SETCODEF( M4_OBJ_OLE_M4O_NOT_FOUND, ERRORLOG, "#*s1*#%s#%d#", "DumpRangeToNode", ( int )ai_pvArg[0].Data.DoubleData ) ;
		TRACERETVAL( OleExec.dll:DumpRangeToNode, M4_ERROR ) ;
	}

	poAccess = pChannel->CreateAccess() ;

	if( poAccess == NULL )
	{
		PutError( pvmContext, M4_ERROR ) ;
		SETCODEF( M4_OBJ_OLE_M4O_NOT_FOUND, ERRORLOG, "#*s1*#%s#", "DumpRangeToNode", ( int )ai_pvArg[0].Data.DoubleData ) ;
		TRACERETVAL(OleExec.dll:DumpRangeToNode, M4_ERROR ) ;
	}

	// bg 256800
	ClObjTerminator oTerminator( poAccess );

	poNode = &( poAccess->Node[ ai_pvArg[1].Data.PointerChar ] ) ;

	if( poNode == NULL )
	{
		PutError( pvmContext, M4_ERROR ) ;
		SETCODEF( M4_OBJ_OLE_NODE_NOT_FOUND, ERRORLOG, "#*s1*#%s#%s#%s#", "DumpRangeToNode", ai_pvArg[1].Data.PointerChar, pChannel->GetpChannelDef()->Id() ) ;
		TRACERETVAL( OleExec.dll:DumpRangeToNode, M4_ERROR ) ;
	}


	// -- Get the Excel range bounds.
    lXlsNumCols = ( vRetVal.parray )->rgsabound[ 0 ].cElements ;
	lXlsNumRows = ( vRetVal.parray )->rgsabound[ 1 ].cElements ;

	// -- Range starting column.
	iRangeStartCol = ai_pvArg[2].Data.DoubleData ;
	
	pcXlsColumnNames = new m4char_t[ (iRangeStartCol + lXlsNumCols - 1) * XLS_COLUMN_WIDTH ];

	// -- Calculate all Excel column names.
	FillXlsColumnNames( pcXlsColumnNames, iRangeStartCol + lXlsNumCols - 1 ) ;


	pvXlsColumnTypes = new VARTYPE[ (iRangeStartCol + lXlsNumCols - 1) ] ;

	// -- Get the range column types.
	iRet = GetXlsRangeColumnTypes( ai_pvContext, iRangeStartCol, ai_pvArg[3].Data.DoubleData, (m4int_t)lXlsNumCols, pvXlsColumnTypes ) ;

	if( iRet == M4_ERROR )
	{
		SafeArrayDestroy( vRetVal.parray ) ;
		PutError( pvmContext, M4_ERROR ) ;
		SETCODEF( M4_OBJ_OLE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", "DumpRangeToNode", "Cannot get range column types" ) ;
		delete[] pcXlsColumnNames ;
		delete[] pvXlsColumnTypes ;
		TRACERETVAL( OleExec.dll:DumpRangeToNode, M4_ERROR ) ;
	}

	// -- Main rows iteration.
	for( long lRowIdx = 0; lRowIdx < lXlsNumRows; lRowIdx++ )
	{
		if( poNode->RecordSet.Register.Add() != M4_SUCCESS)
		{
			SafeArrayDestroy( vRetVal.parray ) ;
			PutError( pvmContext, M4_ERROR ) ;
			SETCODEF( M4_OBJ_OLE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", "DumpRangeToNode", "Cannot add new register" ) ;
			delete[] pcXlsColumnNames ;
			delete[] pvXlsColumnTypes ;
			TRACERETVAL( OleExec.dll:DumpRangeToNode, M4_ERROR ) ;
		}

		for( long lColIdx = 0; lColIdx < lXlsNumCols; lColIdx++ )
		{
			long lIndex[] = { lRowIdx + 1, lColIdx + 1} ;

			SafeArrayGetElement( vRetVal.parray, lIndex, &vCellValue ) ;

			// bg 173345
			if( vCellValue.vt == VT_DATE && vCellValue.date <= 60 )
			{
				vCellValue.date++ ;
			}

			OLEVarToM4Var( vCellValue, &vItemValue ) ;

			// -- Check for end condition.
			if( lColIdx == 0 && ( vItemValue.Type == M4CL_CPP_TYPE_NULL || ( vItemValue.Type == M4CL_CPP_TYPE_STRING_VAR && ( vItemValue.Data.PointerChar == NULL || ( vItemValue.Data.PointerChar != NULL && *(vItemValue.Data.PointerChar) == NULL ) ) ) ) )
			{
				VariantClear( &vCellValue ) ;
				bBreak = M4_TRUE ;

				// Removes the last one.
				if( poNode->RecordSet.Register.Destroy() != M4_SUCCESS)
				{
					PutError( pvmContext, M4_ERROR ) ;
					SETCODEF( M4_OBJ_OLE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", "DumpRangeToNode", "Cannot remove last register" ) ;
					delete[] pcXlsColumnNames ;
					delete[] pvXlsColumnTypes ;
					TRACERETVAL( OleExec.dll:DumpRangeToNode, M4_ERROR ) ;
				}

				break ;
			}

			// bg 177343
			// Checks if the cell value must be a string.
			if( vCellValue.vt == VT_R8 && pvXlsColumnTypes[lColIdx + iRangeStartCol - 1] == VT_BSTR )
			{
				memset( acDoubleValue, 0, 64 ) ;
				sprintf( acDoubleValue, "%g", m4double_t( vItemValue ) ) ;
				vItemValue.Type =  M4CL_CPP_TYPE_STRING_VAR ;
				vItemValue.Data.PointerChar = acDoubleValue ;
			}

			// bg 250177
			// Trim data if it is of string type.
			if( vItemValue.Type == M4CL_CPP_TYPE_STRING_VAR && vItemValue.Data.PointerChar != NULL && *(vItemValue.Data.PointerChar) != NULL )
			{
				TrimValue( vItemValue.Data.PointerChar );
			}

			VariantClear( &vCellValue ) ;

			// -- Set the item value.
			if( poNode->RecordSet.Register.Item[ pcXlsColumnNames + ( ( lColIdx + iRangeStartCol - 1 ) * XLS_COLUMN_WIDTH ) ].Value.Set( vItemValue ) != M4_SUCCESS )
			{
				SafeArrayDestroy( vRetVal.parray ) ;
				PutError( pvmContext, M4_ERROR ) ;
				SETCODEF( M4_OBJ_OLE_ERR_SETTING_ITEM, ERRORLOG, "#*s1*#%s#%s#%s#", "DumpRangeToNode", pcXlsColumnNames + ( ( lColIdx + iRangeStartCol - 1 ) * XLS_COLUMN_WIDTH ), ai_pvArg[1].Data.PointerChar ) ;
				delete[] pcXlsColumnNames ;
				delete[] pvXlsColumnTypes ;
				TRACERETVAL( OleExec.dll:DumpRangeToNode, M4_ERROR ) ;
			}
		}

		if( bBreak == M4_TRUE )
		{
			break ;
		}
	}

	// -- Free the array.
	SafeArrayDestroy( vRetVal.parray ) ;

	delete[] pcXlsColumnNames ;
	delete[] pvXlsColumnTypes ;
	
	PutError( pvmContext, M4_SUCCESS ) ;
	TRACERETVAL( OleExec.dll:DumpRangeToNode, M4_SUCCESS ) ;
}

m4return_t OLE_ExecRetrying(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	HRESULT			hr = ERROR_SUCCESS;
	DISPID			dispid;
	VARIANTARG*		vaArgList = NULL;
	VARIANT			vReturn;
	VARIANT*		pvReturn;
	bool			bReturn; 
	IDispatch*		i_pDsp;
	m4VariantType	pvDsp;
	UINT			iArgErr;
	EXCEPINFO		excepinfo;
	m4VariantType	pvDisp;
	

	TRACESUB(OleExec.dll:OLE_ExecRetrying);

	ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;

	if( ai_iLongArg < 3 || CheckParamString(&ai_pvArg[0])==M4_ERROR || ai_pvArg[1].Type != M4CL_CPP_TYPE_NUMBER || ai_pvArg[2].Type != M4CL_CPP_TYPE_NUMBER )
	{
		PutError( pvmContext, M4_ERROR ) ;
		SETCODE( M4_OBJ_OLE_INVALID_ARGUMENT, ERRORLOG, "OLE_ExecRetrying" ) ;
		TRACERETVAL( OleExec.dll:OLE_ExecRetrying, M4_ERROR ) ;
	}

	m4pcchar_t pccMethod = ai_pvArg[0].Data.PointerChar;
	bReturn = ((pvmContext->m_pRecordSet)->Current.Item[ClHandle(pvmContext->m_hItem)].ItemDef.M4Type() != 0);
	VariantInit(&vReturn);
	pvReturn = bReturn ? &vReturn : NULL;
	CComBSTR bstrMethod(pccMethod);


	// First we retrieve the pointer to the IDispatch pointer we
	// have stored in the channel in the "InterfacePointer" item
	// We use the ai_pvContext to access the channel

	(pvmContext->m_pRecordSet)->Current.Item["InterfacePointer"].Value.Get(pvDsp);
	i_pDsp = (IDispatch*)(long)pvDsp.Data.DoubleData;
	
	
	// Check to see that our object still exists
	if (CheckServerAlive(i_pDsp) == M4_ERROR)
	{
		PutError(pvmContext,M4_ERROR);
		SETCODE(M4_OBJ_OLE_INVALID_POINTER, ERRORLOG, "OleExec - OLE object no longer valid");
		TRACERETVAL(OleExec.dll:OLE_ExecRetrying, M4_ERROR);
	}

	// Create OLE argument list
	if (ai_iLongArg > 3)
		vaArgList = new VARIANTARG[ai_iLongArg - 3];
	DISPPARAMS dpParams = {vaArgList, NULL, ai_iLongArg - 3, 0};
   
	

	// Get DISPID to do the invocation
	hr = i_pDsp->GetIDsOfNames(IID_NULL, &bstrMethod, 1, M4_DEFAULT_LCID, &dispid);

	if (FAILED(hr)) 
	{
		PutError(pvmContext,hr);
		SETCODEF(M4_OBJ_OLE_INVALID_NAME, ERRORLOG, "OleExec - OLE method \"%s\" name not recognized", pccMethod);
		delete [] vaArgList;
		TRACERETVAL(OleExec.dll:OLE_ExecRetrying, M4_ERROR);
	}

	// Convert arguments from M4 variants to OLE variants
	for (m4uint32_t i = 3; i < ai_iLongArg; i++)
	{
		M4VarToOLEVar(ai_pvArg[i], &vaArgList[(ai_iLongArg-1)-i]);
	}
	 

	char strErr[1000];
	char strRetErr[1000];

	long lNumRetries = 0;
	long lMaxRetries = ( ai_pvArg[1].Data.DoubleData > 0 ) ? ai_pvArg[1].Data.DoubleData : 1;
	long lRetryDelay = ai_pvArg[2].Data.DoubleData;

	do
	{
		lNumRetries += 1;
		memset(&excepinfo, 0, sizeof excepinfo);
		iArgErr = (UINT)-1;
		strErr[0] = NULL;
		memset(strRetErr, 0, 1000);

		// Invocation
		hr = i_pDsp->Invoke(dispid, IID_NULL, M4_DEFAULT_LCID, DISPATCH_METHOD, &dpParams, pvReturn, &excepinfo, &iArgErr);

		if (FAILED(hr)) 
		{
			if(hr == DISP_E_EXCEPTION) 
			{
				if(excepinfo.pfnDeferredFillIn != NULL)
					(*(excepinfo.pfnDeferredFillIn)) (&excepinfo);

				USES_CONVERSION;
				if(excepinfo.bstrDescription)
					wsprintf(strErr,"Error:  %s.",OLE2T(excepinfo.bstrDescription));
			}

			if( lMaxRetries > 1 )
			{
				sprintf(strRetErr, "%s [%ld/%ld]", strErr, lNumRetries, lMaxRetries );
				if( lNumRetries == lMaxRetries )
				{
					SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, pccMethod, strRetErr);
				}
				else
				{
					SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, WARNINGLOG,"#*s1*#%ld#%s#%s#", hr, pccMethod, strRetErr);
				}
			}
			else
			{
				SETCODEF(M4_OBJ_OLE_INVOKE_FAILED, ERRORLOG,"#*s1*#%ld#%s#%s#", hr, pccMethod, strErr);
			}

			if( lNumRetries < lMaxRetries && lRetryDelay > 0 )
			{
				Sleep( lRetryDelay );
			}
		}
	}
	while( FAILED(hr) && lNumRetries < lMaxRetries );

	if (bReturn)
		OLEVarToM4Var(vReturn, &ai_pvReturn);

	ReleaseParamBSTR(&vReturn,1);
	ReleaseParamBSTR(vaArgList, ai_iLongArg - 3); // bg 257782
	delete [] vaArgList;
    
	PutError(pvmContext,hr);

	TRACERETVAL(OleExec.dll:OLE_ExecRetrying, M4_SUCCESS);
}

// Trims a string left and right, shifting the contents.
// It does not delete the original string pointer.
static m4return_t TrimValueShifting( m4pchar_t ao_pcValue )
{
	size_t iLen;

	if( ao_pcValue == NULL )
	{
		return M4_ERROR;
	}

	// Trims right.
	int	iIdx = strlen( ao_pcValue ) - 1 ;
	while( ao_pcValue[ iIdx ] == 32 )
	{
		ao_pcValue[ iIdx-- ] = '\0';
	}

	// Trims left.
	while( *ao_pcValue == 32 )
	{
		iLen = strlen( ao_pcValue );

		for( int i = 0; i <= ( iLen - 2 ); i++ )
		{
			ao_pcValue[ i ] = ao_pcValue[ i + 1 ];
		}

		ao_pcValue[ iLen - 1 ] = 0;
	}


	return M4_SUCCESS;
}

typedef set< m4pchar_t, m4pchar_t_less > AppProgIdSet_t;
typedef AppProgIdSet_t::iterator AppProgIdSetIt_t;

static m4bool_t CheckExecutionPermission( ClVMRunContext* ai_poContext, m4pchar_t ai_pcProgId )
{
	m4bool_t			bGranted = M4_TRUE;
	ClChannelManager*	poChManager;
	m4pchar_t			pcLockedListCfg;
	m4pchar_t			pcLockedList;
	m4pchar_t			pcItem;
	size_t				iSize;
	AppProgIdSet_t		setAppProgIds;
	m4pchar_t			pcAppId;
	m4bool_t			bDel = M4_FALSE;


	poChManager = ai_poContext->m_pAccess->GetpChannel()->GetpChannelManager();

	// Client side execution is always allowed.
	if( poChManager->GetDMContext()->ExecutingInClient() == true )
	{
		return M4_TRUE;
	}


	// Gets the configured COM applications to lock in server side.
	ClLogonAdaptor* poLogonAdaptor = poChManager->GetLogonAdaptor();

	if( poLogonAdaptor == NULL )
	{
		return M4_FALSE;
	}

	pcLockedListCfg = poLogonAdaptor->GetSrvCOMAppsLocked();


	// There isn't configured any application to lock.
	if( pcLockedListCfg == NULL || *pcLockedListCfg == 0 )
	{
		return M4_TRUE;
	}


	// Makes a copy of the list to tokenize it, storing the item trimmed and in lower case.
	iSize = strlen( pcLockedListCfg ) + 1;
	pcLockedList = new m4char_t[ iSize ];
	strcpy( pcLockedList, pcLockedListCfg );

	pcItem = strtok( pcLockedList, "," );

	while( pcItem != NULL ) 
	{
		TrimValueShifting( pcItem );
		M4StrLwr( pcItem );

		// Detected wildchar so all applications must be locked.
		if( strcmpi( pcItem, "*" ) == 0 )
		{
			delete pcLockedList;
			return M4_FALSE;
		}
	
		if( *pcItem != 0 )
		{
			setAppProgIds.insert( pcItem );
		}

		pcItem = strtok( NULL, "," );
	}


	// Determines the application Id from the progId requested.
	// In order to be version independent, ignores the last characters.
	if( _strnicmp( ai_pcProgId, "word.", 5 ) == 0 )
	{
		pcAppId = "msword";
	}
	else if( _strnicmp( ai_pcProgId, "excel.", 6 ) == 0 )
	{
		pcAppId = "msexcel";
	}
	else if( _strnicmp( ai_pcProgId, "msmapi.", 7 ) == 0 )
	{
		pcAppId = "msmapi";
	}
	else if( _strnicmp( ai_pcProgId, "notes.", 6 ) == 0 )
	{
		pcAppId = "notes";
	}
	else if( _strnicmp( ai_pcProgId, "msproject.", 10 ) == 0 )
	{
		pcAppId = "msproject";
	}
	else if( _strnicmp( ai_pcProgId, "msscriptcontrol.", 16 ) == 0 )
	{
		pcAppId = "msscript";
	}
	else
	{
		// It isn't a base progId, saves it as progId.
		// Makes a copy to manipulate it.		
		iSize = strlen( ai_pcProgId ) + 1;
		pcAppId = new m4char_t[ iSize ];
		strcpy( pcAppId, ai_pcProgId );

		TrimValueShifting( pcAppId );
		M4StrLwr( pcAppId );

		bDel = M4_TRUE;
	}

	// Checks if the application execution is forbidden.
	AppProgIdSetIt_t oIt = setAppProgIds.find( pcAppId );

	if( oIt != setAppProgIds.end() )
	{
		bGranted = M4_FALSE;
	}

	// Clean.
	delete pcLockedList;

	if( bDel == M4_TRUE )
	{
		delete pcAppId;
	}
	

	return bGranted;
}
