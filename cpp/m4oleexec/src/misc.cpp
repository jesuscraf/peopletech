//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           OLEExec.dll
// File:             misc.cpp
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

#include "misc.hpp"
#include "m4buildversion.hpp"
#include "regedit.hpp"
#include "m4regdef.hpp"


void InitDLL (void)
{
}

void EndDLL (void)
{
}

m4return_t M4VarToOLEVar(m4VariantType ai_m4var, VARIANT* ao_olevar)
{
	::VariantInit(ao_olevar);
	switch (ai_m4var.Type)
	{
	case M4CL_CPP_TYPE_NUMBER:
		ao_olevar->vt = VT_R8;
		ao_olevar->dblVal = ai_m4var.Data.DoubleData;
		break;
	case M4CL_CPP_TYPE_DATE:
		ao_olevar->vt = VT_DATE;
		ao_olevar->date = M4dateToDate(ai_m4var.Data.DoubleData);
		break;

	case M4CL_CPP_TYPE_STRING_VAR:
		// jcr. Para que trate correctamente los euros
		ai_m4var.GetVARIANT( *ao_olevar ) ;
		break;
	}

	return M4_SUCCESS;
}

m4return_t OLEVarToM4Var(VARIANT ai_olevar, m4VariantType* ao_m4var)
{
	ao_m4var->SetNull();

	switch (ai_olevar.vt)
	{
	case VT_I2:
		ao_m4var->Type = M4CL_CPP_TYPE_NUMBER; 
		VarR8FromI2(ai_olevar.iVal,	&ao_m4var->Data.DoubleData);
		break;

	case VT_I4:
		ao_m4var->Type = M4CL_CPP_TYPE_NUMBER; 
		VarR8FromI4(ai_olevar.lVal,	&ao_m4var->Data.DoubleData);
		break;

	case VT_R4:
		ao_m4var->Type = M4CL_CPP_TYPE_NUMBER; 
		VarR8FromR4(ai_olevar.fltVal, &ao_m4var->Data.DoubleData);
		break;

	case VT_R8:
		ao_m4var->Type = M4CL_CPP_TYPE_NUMBER; 
		ao_m4var->Data.DoubleData = ai_olevar.dblVal;
		break;

	case VT_BOOL:
		ao_m4var->Type = M4CL_CPP_TYPE_NUMBER; 
		VarR8FromBool(ai_olevar.boolVal, &ao_m4var->Data.DoubleData);
		if(ao_m4var->Data.DoubleData==-1.0) 
			ao_m4var->Data.DoubleData = 1.00;
		break;

	case VT_UI1:
		ao_m4var->Type = M4CL_CPP_TYPE_NUMBER; 
		VarR8FromUI1(ai_olevar.bVal, &ao_m4var->Data.DoubleData);
		break;

	case VT_DATE:
		ao_m4var->Type = M4CL_CPP_TYPE_DATE;
		ao_m4var->Data.DoubleData = DateToM4date(ai_olevar.date);
		break;

	case VT_BSTR:
		// jcr. Para que trate correctamente los euros
		*ao_m4var = ai_olevar ;
		break;

	case VT_CY:
		ao_m4var->Type = M4CL_CPP_TYPE_NUMBER;
		VarR8FromCy(ai_olevar.cyVal, &ao_m4var->Data.DoubleData);
		break;

	case VT_EMPTY:
		// bg 116806		
		//ao_m4var->Type = M4CL_CPP_TYPE_NUMBER;
		ao_m4var->Type = M4CL_CPP_TYPE_NULL ;
		ao_m4var->Data.DoubleData = 0;		
		break;

	case VT_DISPATCH:
		ao_m4var->Type = M4CL_CPP_TYPE_NUMBER;
		ao_m4var->Data.DoubleData = (double) (long)ai_olevar.pdispVal;
		break;
	case VT_UNKNOWN:
		ao_m4var->Type = M4CL_CPP_TYPE_NUMBER;
		ao_m4var->Data.DoubleData = (double) (long)ai_olevar.punkVal;
		break;
/*   To be Tested JGM  */
	case VT_VARIANT:
		ao_m4var->Type = M4CL_CPP_TYPE_NUMBER;
		ao_m4var->Data.DoubleData = (double) (long)ai_olevar.pvarVal;
		break;
	default:
		break;
	}

	return M4_SUCCESS;
}

//Creates and Instance of an object which has bstrLicenseKey as string Key.
HRESULT CoCreateInstanceLic(const CLSID& clsid, IUnknown* pUnknownOuter, DWORD dwClsContext, const IID& iid, void **ppv, BSTR bstrLicenseKey)
{
	HRESULT hr;
	*ppv=NULL;
	LICINFO Lic;
	LICINFO* pLicInfo;


	pLicInfo=&Lic;	
	IClassFactory2* pIFactory=NULL;
	
	hr=CoGetClassObject(clsid,dwClsContext,NULL,IID_IClassFactory2,(void**)&pIFactory);
	if (SUCCEEDED(hr))
	{
	//			mgkgtgnnmnmninigthkgogggvmkhinjggnvm
		hr=pIFactory->CreateInstanceLic(pUnknownOuter,NULL, iid,bstrLicenseKey,ppv);	
		pIFactory->Release();
	}
	return hr;	

}
void ChangeNumberTo(m4VariantType ai_m4var, VARIANT* ao_olevar,VARIANT* ao_oleinf)
{
	UINT t;
	t=ao_oleinf->vt;
	switch(ao_oleinf->vt)
	{
	case VT_DISPATCH:
	case VT_PTR:
		ao_olevar->vt=VT_DISPATCH;
		ao_olevar->pdispVal=(IDispatch*)(long)ai_m4var.Data.DoubleData;
		break;
	case VT_UNKNOWN:
		ao_olevar->vt=VT_UNKNOWN;
		ao_olevar->punkVal=(IUnknown*)(long)ai_m4var.Data.DoubleData;
		break;
	case VT_VARIANT:
	//	ao_olevar->vt=VT_VARIANT;
	//	ao_olevar->pvarVal=(VARIANT*)&ai_m4var.Data.DoubleData;
	//	break;
	case VT_EMPTY: // No Info como antes
		ao_oleinf->vt=VT_R8;
		ao_oleinf->dblVal = ai_m4var.Data.DoubleData;
		VariantChangeType(ao_olevar,ao_oleinf,VARIANT_NOVALUEPROP,VT_R8);
		break;
	default:
		ao_oleinf->vt=VT_R8;
		ao_oleinf->dblVal = ai_m4var.Data.DoubleData;
		VariantChangeType(ao_olevar,ao_oleinf,VARIANT_NOVALUEPROP,t);
		break;
	}
}

m4return_t M4VarToOLEVar2(m4VariantType ai_m4var, VARIANT* ao_olevar,VARIANT* ao_oleinf)
{
	::VariantInit(ao_olevar);
	switch (ai_m4var.Type)
	{
	case M4CL_CPP_TYPE_NUMBER:
		ChangeNumberTo(ai_m4var,ao_olevar,ao_oleinf);
		break;
	case M4CL_CPP_TYPE_DATE:
		ao_olevar->vt = VT_DATE;
		ao_olevar->date = M4dateToDate(ai_m4var.Data.DoubleData);
		break;
	/*case M4CL_CPP_TYPE_DATE:
		ao_oleinf->date = M4dateToDate(ai_m4var.Data.DoubleData);
        VariantChangeType(ao_olevar,ao_oleinf,VARIANT_NOVALUEPROP,ao_oleinf->vt);
		break;*/

	case M4CL_CPP_TYPE_STRING_VAR:
		if(!strncmp(ai_m4var.Data.PointerChar, "{IDispatch}", 11))
		{
			ao_olevar->vt=VT_DISPATCH;
			ao_olevar->pdispVal=(IDispatch*)atol(&ai_m4var.Data.PointerChar[11]);;
		}
		else
		{
			// jcr. Para que trate correctamente los euros
			ai_m4var.GetVARIANT( *ao_olevar ) ;
		}
		break;
	}

	return M4_SUCCESS;
}






/* GetFuncInfo Devuelve la inforamción de los tipos de la función en el array 
   vaArgLis. En vaArgLis[0] se devuelve la información de retorno de la funcion.
   Parametros.
   [in]   ITypeInfo *i_pTypeinfo. Interfaz de iTypeInfo.
   [out] VARIANTARG vaArgLis []. Array donde se devuelve la informacion de los tipos.
                                 El array se crea y se destruye fuera de esta función
   [in] UINT iMaxArg. Tamaño del array vaArgLis.
   [in] DISPID dispid. Dispid de la función a devolver la informacion.
   [in] 
   Retorna
    1 si encontrado.
	0 si no encontrado
*/   
UINT GetFuncInfo(ITypeInfo *i_pTypInfo,VARIANTARG vaArgLis[],UINT iMaxArg,DISPID dispid,INVOKEKIND k)
{
	HRESULT         hr;
	TYPEATTR*		pTypeAttr;
	UINT			iCont;
	UINT			iFound;
	UINT			i;
	FUNCDESC*       pFuncDesc; 
		iCont = 0;
		pFuncDesc = NULL;
		iFound = 0;
		hr = i_pTypInfo->GetTypeAttr(&pTypeAttr);
		if (SUCCEEDED(hr))
		{
		while ((iCont < UINT(pTypeAttr->cFuncs)) && (iFound==0))
		{
			hr = i_pTypInfo->GetFuncDesc(iCont,&pFuncDesc);
			if (!hr && (dispid==pFuncDesc->memid) && (k==pFuncDesc->invkind))
			{
				iFound=1; // iCont == 0 es bueno
				::VariantInit(&vaArgLis[0]);
				vaArgLis[0].vt =pFuncDesc->elemdescFunc.tdesc.vt;// Retorno funcion.
				i=1;
				while (i<=iMaxArg)
				{
					if (i<=(UINT)pFuncDesc->cParams)
					{
						::VariantInit(&vaArgLis[i]);
						vaArgLis[i].vt =pFuncDesc->lprgelemdescParam[i-1].tdesc.vt; 
					}
					else
					{
						::VariantInit(&vaArgLis[i]);
						  vaArgLis[i].vt = VT_EMPTY;
					}
					i++;
				}
			}
			i_pTypInfo->ReleaseFuncDesc(pFuncDesc);			
			iCont++;			
		}
		i_pTypInfo ->ReleaseTypeAttr(pTypeAttr);
		}
		return (iFound);
}


/* 
   Funcion GetFuncInterfaceLib devuelve en vArgLis la información de los tipos
   de los parametros de la función extrayendo la información de la TypeLibrary.
   vArgLis[0] se devuelve el tipo que devuelve la función.
   Parametros.
   [out] VARIANTARG vaArgLis []. Array donde se devuelve la informacion de los tipos.
                                 El array se crea y se destruye fuera de esta función
   [in] UINT iMaxArg. Tamaño del array vaArgLis.
   [in] DISPID dispid. Dispid de la función a devolver la informacion.
   [in] 
   Retorna
    1 si encontrado.
	0 si no encontrado
								 
*/ 
/*
UINT GetFuncInterfaceLib(VARIANTARG vaArgLis[],UINT iMaxArg,DISPID dispid,CLSID LibId)
{
	HRESULT			hr;
	ITypeInfo*      i_pTypInfo; 
	ITypeLib*		pITypeLib;
	UINT			iFound;
	
		pITypeLib = NULL;
		i_pTypInfo = NULL;
		CLSID CL;
		CLSIDFromString(L"{1CF542E0-D988-11cf-B485-00805F98FACE}",&CL);
        hr=::LoadRegTypeLib(CL,1,0,0x00,&pITypeLib);
		UINT k;
		k=0;
		iFound = 0;
		while (k<pITypeLib->GetTypeInfoCount() && (iFound==0))
		{
			hr=pITypeLib->GetTypeInfo(k,&i_pTypInfo);
			if (SUCCEEDED(hr))
			{
				iFound = GetFuncInfo(i_pTypInfo,&vaArgLis[0],iMaxArg,dispid);			
				i_pTypInfo ->Release();
			}
			k++;
		}
		return(iFound);
	
}
*/
/* 
   Funcion GetFuncInterface devuelve en vArgLis la información de los tipos
   de los parametros de la función extrayendo la información directamente del
   interfaz IDispacth
   vArgLis[0] se devuelve el tipo que devuelve la función.
   Parametros.
   [in] IDispatch * i_pDsp, interfaz pointer IDipsacth
   [out] VARIANTARG vaArgLis []. Array donde se devuelve la informacion de los tipos.
                                 El array se crea y se destruye fuera de esta función
   [in] UINT iMaxArg. Tamaño del array vaArgLis.
   [in] DISPID dispid. Dispid de la función a devolver la informacion.
   Retorna
    1 si encontrado.
	0 si no encontrado
								 
*/ 

UINT GetFuncInterface(IDispatch* i_pDsp,VARIANTARG vaArgLis[],UINT iMaxArg,DISPID dispid,INVOKEKIND k)
{
	HRESULT			hr;
	ITypeInfo*      i_pTypInfo; 
	UINT			iTypInfo;
	UINT			iFound;
	iTypInfo = NULL;
	hr = i_pDsp->GetTypeInfoCount(&iTypInfo);
	iFound = 0;
	if (iTypInfo && SUCCEEDED(hr))
	{
		i_pTypInfo = NULL;
		hr = i_pDsp->GetTypeInfo(0,M4_DEFAULT_LCID,&i_pTypInfo);
		if (SUCCEEDED(hr))
		{
			iFound=GetFuncInfo(i_pTypInfo,&vaArgLis[0],iMaxArg,dispid,k);
			i_pTypInfo ->Release();
		}
	}
	return (iFound);
}



void ReleaseParamBSTR(VARIANTARG vaArgLis[],m4uint32_t ai_iLongArg)
{
	if (ai_iLongArg>0)
		for(m4uint32_t i = 0; i < ai_iLongArg; i++)
			if (vaArgLis[i].vt==VT_BSTR)
				::SysFreeString(vaArgLis[i].bstrVal);
}



m4return_t	formatErrorMsg( long ai_lResult, m4pchar_t ao_pcErrorMessage, DWORD ai_size )
{
	m4return_t	iResult = M4_SUCCESS ;
	DWORD		dwResult = 0 ;


	iResult = M4_SUCCESS ;


	dwResult = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				ai_lResult,
				0,
				ao_pcErrorMessage,
				ai_size,
				NULL 
			) ;

	if( dwResult == 0 )
	{
		iResult = M4_ERROR ;
	}

	return( iResult ) ;
}

// eh 265081
#define M4OLE_MAX_TEMP_BUFFER			256
#define M4OLE_REG_KEY_TOOLS_OLE			"TOOLS\\OLE"
#define M4OLE_REG_ATTR_LOCK_CB_OPS		"LockCBOperations"

// bg 263966
typedef char ( WINAPI * pFuncGRKD )( char*, char*, char*, unsigned long* );

m4bool_t CheckLockingClipboardOperation()
{
	m4uint32_t	iSize = 0;
	m4char_t	acPath[ M4OLE_MAX_TEMP_BUFFER + 1 ] = "" ;
	m4char_t	acLock[ M4OLE_MAX_TEMP_BUFFER + 1 ] = "" ;
	HMODULE		hMod;
	pFuncGRKD	pFunGetRegKeyData;
	char		iRet;

	if( getenv( "M4SERVER_PATH" ) == NULL )
	{
		return M4_TRUE;
	}

	M4ComposeRegistryPath( M4_NEW_REGEDIT_KEY_SERVER, M4OLE_REG_KEY_TOOLS_OLE, acPath, M4OLE_MAX_TEMP_BUFFER, iSize );
	memset( acLock, 0, M4OLE_MAX_TEMP_BUFFER + 1 );

	hMod = GetModuleHandle( TEXT( "m4inidll.dll"));

	if( hMod == NULL )
	{
		return M4_TRUE;
	}

	pFunGetRegKeyData = ( pFuncGRKD )GetProcAddress( hMod, "?GetRegKeyData@@YADPAD00PAK@Z" );
		
	if( pFunGetRegKeyData )
	{		
		iRet = pFunGetRegKeyData( acPath, M4OLE_REG_ATTR_LOCK_CB_OPS, acLock, NULL );

		if( iRet == FALSE )
		{
			// Backward compatibility.
			return M4_TRUE;
		}

		if( *acLock == '1' )
		{
			return M4_FALSE;
		}
	}


	return M4_TRUE;
}