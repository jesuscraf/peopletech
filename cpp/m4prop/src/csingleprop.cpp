// CSingleProp.cpp : Implementation of CCSingleProp
#include "stdafx.h"
#include "m4prop.h"
#include "csingleprop.h"
#include "m4unicode.hpp"

//##. Begin Adri y Juanma.
//	Optimización de ECI. Eliminamos todas las cadenas que almacenan nombre
//	de propiedades y las sustituimos por un código
#include "namepropmap.h"
extern CNamePropMap g_NamePropMap;
// End


/////////////////////////////////////////////////////////////////////////////
// CCSingleProp

STDMETHODIMP CCSingleProp::get_Name(BSTR * pVal)
{
	

	//##. Begin Adri y Juanma.
	//	Optimización de ECI. Eliminamos todas las cadenas que almacenan nombre
	//	de propiedades y las sustituimos por un código

	// devolvemos una copia
	*pVal = ::SysAllocString(g_NamePropMap.GetBSTRFromCode (m_iNameCode));
	return S_OK;
	//End
}

STDMETHODIMP CCSingleProp::put_Name(BSTR newVal)
{
	//##. Begin Adri y Juanma.
	//	Optimización de ECI. Eliminamos todas las cadenas que almacenan nombre
	//	de propiedades y las sustituimos por un código

	m_iNameCode = g_NamePropMap.GetCodeFromBSTR (newVal);
	return S_OK;
	//End
}

STDMETHODIMP CCSingleProp::get_Value(VARIANT * pVal)
{
	return VariantCopy(pVal, &m_vValue);
}

STDMETHODIMP CCSingleProp::put_Value(VARIANT newVal)
{
	return VariantCopy(&m_vValue, &newVal);
}

STDMETHODIMP CCSingleProp::get_Length(long * pVal)
{
	
	long nLength = 0;
	long ntmpValue;
	long nCount;

	//##. Begin Adri y Juanma.
	//	Optimización de ECI. Eliminamos todas las cadenas que almacenan nombre
	//	de propiedades y las sustituimos por un código
	char* pTemp = M4VBToCpp(g_NamePropMap.GetBSTRFromCode(m_iNameCode));
	long ntmpLength = strlen(pTemp);
	delete pTemp;
	//End

	// sumamos Longitud y Tag
	nLength += ntmpLength + 1;
	// Sumamos Longitud de la longitud
	nLength += Longlength(ntmpLength);

	// Luego el valor del Variant
	switch(m_vValue.vt)
	{
		case VT_I4:
			ntmpValue = m_vValue.lVal;
			// Sumamos tag, longitud, ocupa el valor
			
			if ((ntmpValue <= 127) && (ntmpValue >= -128))
				nCount = 1;
			else if ((ntmpValue <= 32767) && (ntmpValue >= -32768))
				nCount = 2;
			else
				if ((ntmpValue <= 8388607) && (ntmpValue >= -8388608))
					nCount = 3;
				else
					nCount = 4;
			nLength += nCount + 2;

			break;
		case VT_I2:
			ntmpValue = m_vValue.iVal;
			// Sumamos tag, longitud, ocupa el valor
			
			if ((ntmpValue <= 127) && (ntmpValue >= -128))
				nCount = 1;
			else if ((ntmpValue <= 32767) && (ntmpValue >= -32768))
				nCount = 2;
			else
				if ((ntmpValue <= 8388607) && (ntmpValue >= -8388608))
					nCount = 3;
				else
					nCount = 4;
			nLength += nCount + 2;

			break;

		case VT_BSTR:
			pTemp = M4VBToCpp(m_vValue.bstrVal);
			ntmpLength = strlen(pTemp);
			delete pTemp;
			nLength += ntmpLength +1;
			nLength += Longlength(ntmpLength);
			break;

		case VT_BOOL:
			nLength += 3;
			break;

	}
	
	*pVal = nLength;


	return S_OK;
}

STDMETHODIMP CCSingleProp::WriteASN(unsigned char** charIn)
{
	USES_CONVERSION;
		
	long ntmpValue = 0;
	
	// Escribimos el nombre de la propiedad
	//##. Begin Adri y Juanma.
	//	Optimización de ECI. Eliminamos todas las cadenas que almacenan nombre
	//	de propiedades y las sustituimos por un código
	char* tmpName = M4VBToCpp(g_NamePropMap.GetBSTRFromCode(m_iNameCode));
	writeString(charIn, tmpName, strlen(tmpName));
	delete tmpName;
	//End
	
	// Luego el valor del Variant
	switch(m_vValue.vt)
	{
		case VT_BSTR:
			tmpName = M4VBToCpp(m_vValue.bstrVal);
			writeString(charIn, tmpName, strlen(tmpName));
			delete tmpName;
			break;
	
		case VT_I4:
			ntmpValue = m_vValue.lVal;
			writeInteger(charIn, ntmpValue);
			break;

		case VT_I2:
			ntmpValue = m_vValue.iVal;
			writeInteger(charIn, ntmpValue);
			break;

		case VT_BOOL:
 			writeBool(charIn, m_vValue.bVal);
			break;
	}
	
	return S_OK;
}

STDMETHODIMP CCSingleProp::WriteXML(ClLongString &roXML)
{
	USES_CONVERSION;
		
	int iLength = -1;
	char acBuffer[128];
	
	// Escribimos el nombre de la propiedad
	//##. Begin Adri y Juanma.
	//	Optimización de ECI. Eliminamos todas las cadenas que almacenan nombre
	//	de propiedades y las sustituimos por un código
	iLength = -1;
	char* tmpName = M4VBToCpp(g_NamePropMap.GetBSTRFromCode(m_iNameCode));
	char* pcAttribute = M4CppToUtf8(tmpName, iLength);
	delete tmpName;
	roXML.Concat(" ");
	roXML.Concat(pcAttribute);
	roXML.Concat("='");
	delete pcAttribute;
	//End
	
	// Luego el valor del Variant
	char* pcValue = NULL;

	switch(m_vValue.vt)
	{
		case VT_BSTR:
			iLength = -1;
			tmpName = M4VBToCpp(m_vValue.bstrVal);
			pcValue = M4CppToUtf8(tmpName, iLength);
			delete tmpName;
			roXML.Concat(pcValue, iLength, M4MDRT_CODE_MODE_XML);
			delete pcValue;
			break;
	
		case VT_I4:
			sprintf(acBuffer, "%d", m_vValue.lVal);
			roXML.Concat(acBuffer);
			break;

		case VT_I2:
			sprintf(acBuffer, "%d", m_vValue.iVal);
			roXML.Concat(acBuffer);
			break;

		case VT_BOOL:

			if(m_vValue.bVal)
			{
				roXML.Concat("TRUE");
			}
			else
			{
				roXML.Concat("FALSE");
			}
			break;
	}
	
	roXML.Concat("'");
	return S_OK;
}
