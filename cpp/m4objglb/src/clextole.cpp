//## begin module%3912A6BD02F7.cm preserve=no
//## end module%3912A6BD02F7.cm

//## begin module%3912A6BD02F7.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%3912A6BD02F7.cp

//## Module: clextole%3912A6BD02F7; Package body
//## Subsystem: m4objglb::src%3912A6F00372
//## Source file: d:\new\m4objglb\src\clextole.cpp

//## begin module%3912A6BD02F7.additionalIncludes preserve=no
//## end module%3912A6BD02F7.additionalIncludes

//## begin module%3912A6BD02F7.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:             clextole.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             06/07/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Esta clase aún no está terminada falta definir el modo de invocación
// mediante el Execute.
//
//
//==============================================================================


#include "clextole.hpp"

/* COMENTADO PARA COMPILACIÓN EN UNIX PORQUE POR AHORA NO SE USA
M4ClExternOle::M4ClExternOle( void )
{
	m_pDsp = NULL;

}

m4return_t M4ClExternOle::Init( m4pcchar_t ai_pccOleName )
{
	HRESULT ret;
	GUID rclsid;

	ret = CoInitialize(NULL);

	if (ret != S_OK )
	{
		return M4_ERROR;
    }

	ret = CLSIDFromProgID(L"Word.Basic", &rclsid);
	
	if (ret != S_OK )
	{
		CoUninitialize();
		return M4_ERROR;
    }

	ret = CoCreateInstance(rclsid, NULL, CLSCTX_SERVER, IID_IDispatch, (void**)&m_pDsp);
	
	if (ret != S_OK )
	{
		CoUninitialize();
		return M4_ERROR;
    }

	return M4_SUCCESS;
}

M4ClExternOle::~M4ClExternOle( void )
{
	HRESULT ret;

	ret = m_pDsp -> Release();
	CoUninitialize();
}

m4return_t  M4ClExternOle::Execute ( m4pcchar_t ai_pccMethodName, m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	//Aún no sabemos como invocar a la función: de donde se sacan los tipos verdaderos
	//en el caso de ole quizas se podrían obtener dinámicamente.
	HRESULT ret;
	DISPID dispid;
	OLECHAR* szMember1 = L"ApMostrar";

	ret = m_pDsp -> GetIDsOfNames( IID_NULL, &szMember1, 1, 1033, &dispid);

	if (ret != S_OK)
	{
		return M4_ERROR;
	}
	
	//Ejemplo de paso de parámetros
	DISPPARAMS dpNoArgs = {NULL, NULL, 0, 0};
	VARIANTARG vArg;
	DISPPARAMS dpOneArg = {&vArg, NULL, 1, 0};
	BSTR pBstrFileName = SysAllocString(L"C:\\TEMP\\DB.DOC");
	vArg.vt = VT_BSTR;
	vArg.bstrVal = pBstrFileName;

	//Ejemplo de paso de invocación de función
	ret = m_pDsp -> Invoke(dispid,	IID_NULL, 1033, DISPATCH_METHOD, &dpNoArgs, NULL, NULL, NULL);

	if (ret != S_OK)
	{
		return M4_ERROR;
	}


	return M4_SUCCESS;
}
*/

//## end module%3912A6BD02F7.includes

//## begin module%3912A6BD02F7.declarations preserve=no
//## end module%3912A6BD02F7.declarations

//## begin module%3912A6BD02F7.additionalDeclarations preserve=yes
//## end module%3912A6BD02F7.additionalDeclarations


//## begin module%3912A6BD02F7.epilog preserve=yes
//## end module%3912A6BD02F7.epilog
