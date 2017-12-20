// M4Base64.cpp : Implementation of CM4Base64
#include "stdafx.h"
#include "m4apie.h"
#include "_base64.h"
#include "m4base64.h"

/////////////////////////////////////////////////////////////////////////////
// CM4Base64


//##ModelId=396B329A0245
STDMETHODIMP CM4Base64::Code(BSTR strIn, BSTR *strOut)
{
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	unsigned char *salida;
	long longitud;

	longitud = ::SysStringLen(strIn);

	salida = new BYTE[longitud];

	WideCharToMultiByte(CP_ACP , NULL, strIn, longitud, (char *)salida, longitud, NULL, NULL);

	m_b64.Code(salida, strOut, longitud);
	
	delete[] salida;

	return S_OK;
}

//##ModelId=396B329A023A
STDMETHODIMP CM4Base64::Decode(BSTR strIn, BSTR *strOut)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

		
	if (m_b64.Decode(strIn, strOut) == 1) return S_OK;
	return E_INVALIDARG;
}
