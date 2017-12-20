// ASNPersistPBD.cpp: implementation of the CASNPersistPBD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdio.h"
#include "m4prop.h"
#include "dataproperties.h"
#include "asnpersistpbd.h"
#include "m4unicode.hpp"
extern int	M4IsUnicode( void ) ;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CASNPersistPBD::CASNPersistPBD()
{
	m_bPersistIdHelpTopic = -1;
	m_nASNLength = 0;
	m_nLangNumber = 0;
	m_pASNBuffer = NULL;
	m_pASNBufferActual = NULL;
	m_ppASNLangBuffer = NULL;
	m_ppASNLangBufferActual = NULL;
	m_pnASNLangLength = NULL;
	m_pLog = NULL;
	m_pTempBuffer = NULL;
	
	m_pstrIdChannel = (char*)malloc(2);
	m_pstrIdChannelAlias = (char*)malloc(2);
	m_pstrIdChannelKey = (char*)malloc(2);
	m_pstrIdAccessAlias = (char*)malloc(2);
	m_pstrIdItem = (char*)malloc(2);
	m_pstrIdNode = (char*)malloc(2);

	strcpy(m_pstrIdChannel, "");
	strcpy(m_pstrIdChannelAlias, "");
	strcpy(m_pstrIdChannelKey, "");
	strcpy(m_pstrIdAccessAlias, "");
	strcpy(m_pstrIdItem, "");
	strcpy(m_pstrIdNode, "");
	m_nGrants = 31;
	m_nIdPresentation = -1;
	m_nStartPos = -6;
	m_nRefCount = 0;
	m_pTraduct = NULL;
 }

CASNPersistPBD::~CASNPersistPBD()
{
	ClearASNBuffers(); 

	free(m_pstrIdChannel);
	free(m_pstrIdChannelAlias);
	free(m_pstrIdChannelKey);
	free(m_pstrIdAccessAlias);
	free(m_pstrIdItem);
	free(m_pstrIdNode);
	RELEASE(m_pTraduct);
	free(m_pTempBuffer);
}

void CASNPersistPBD::InitASNBuffers(ITraductProp *pTraduct)
{
	m_pTraduct = pTraduct;

	if (m_pLog)
	{
		if (m_pTraduct)
			m_pLog->Write(" Initializing ASN Buffers - OK");
		else
			m_pLog->Write(" Initializing ASN Buffers - Failed");
	}
	
	if (m_pTraduct)
	{
		m_pTraduct->AddRef();
	}
	else
		return;
	
	long nLang = 0;
	HRESULT hr = m_pTraduct->get_PropertyNumber((short*)&nLang);

	m_pASNBuffer = (BYTE*)malloc(PERSIST_ASN_CHUNK);
	m_pASNBufferActual = m_pASNBuffer + PERSIST_ASN_CHUNK;
	m_pASNBufferFinal = m_pASNBufferActual;
	m_nLangNumber = nLang;
	
	m_ppASNLangBuffer = (BYTE**)malloc(nLang*(sizeof(BYTE*)));
	m_ppASNLangBufferActual = (BYTE**)malloc(nLang*(sizeof(BYTE*)));
	m_ppASNLangBufferFinal = (BYTE**)malloc(nLang*(sizeof(BYTE*)));
	m_pnASNLangLength = (long*)malloc(nLang*(sizeof(long)));

	for (int i= 0; i<nLang; i++)
	{
		m_ppASNLangBuffer[i] = (BYTE*)malloc(PERSIST_ASN_LANG_CHUNK);
		m_ppASNLangBufferActual[i] = m_ppASNLangBuffer[i] + PERSIST_ASN_LANG_CHUNK;
		m_ppASNLangBufferFinal[i] = m_ppASNLangBufferActual[i];
		m_pnASNLangLength[i] = 0;
	}
}

void CASNPersistPBD::ClearASNBuffers()
{

	if (m_pASNBuffer)
	{
		if (m_pLog)
		{
			m_pLog->Write("Clear Main Buffer");
		}
		
		free(m_pASNBuffer);
		m_pASNBuffer = NULL;
	}
	if (m_ppASNLangBuffer)
	{
		
		for(int i= 0; i<m_nLangNumber; i++)
		{
			if (m_pLog)
			{
				sprintf(m_pTempBuffer," Clear Lang Buffer %i", i);
				m_pLog->Write(m_pTempBuffer);
			}
			
			if (m_ppASNLangBuffer[i])
			{
				if (m_pLog)
					m_pLog->Write("      OK");
				free(m_ppASNLangBuffer[i]);
				m_ppASNLangBuffer[i] = NULL;
			}
		}
		free(m_ppASNLangBuffer);
		m_ppASNLangBuffer = NULL;
		free(m_ppASNLangBufferActual);
		m_ppASNLangBufferActual = NULL;
		free(m_ppASNLangBufferFinal);
		m_ppASNLangBufferFinal = NULL;
		free(m_pnASNLangLength);
		m_pnASNLangLength = NULL;
	}
	m_nLangNumber = 0;
}

HRESULT CASNPersistPBD::ProccessPBD(ICPropBagDef * pPBD)
{
	// Tomamos al m_pASNBuffer como traza para saber si estamos inicializados
	if (m_pASNBuffer == NULL) return E_FAIL;
	m_nASNLength = 0;
	for (int i = 0; i<m_nLangNumber; i++)
	{
		m_pnASNLangLength[i] = 0;
	}
	
	// Nos hacemos con un puntero a un objeto de Propiedades de Datos válidos

	long tmpLength = RecProccess(pPBD, NULL);
	//if (tmpLength > 0)
	//{
		// Escribimos la Longitud
	PersistLength(tmpLength);

	if (m_pASNBufferActual == m_pASNBuffer)
	{
		ExpandBuffer();
	}
	*(--m_pASNBufferActual) = ASNSequence;
	m_nASNLength++;

	TerminateLangData();

	//}
	//else
	//	return E_FAIL;

	return S_OK;
}

long CASNPersistPBD::RecProccess(ICPropBagDef * pPBD, CDataProperties * pData)
{
	USES_CONVERSION;
	
	
	// El proceso será lanzamiento recursivo, obteniendo el puntero a
	// las Propiedades y a la ChildCol, Lanzar sobre ChildCol y luego sumar las Propiedades
	ICPropColDef * pColDef = NULL;
	ICChildColDef * pChildDef = NULL;
	long nActualLength = 0;
	long retValue = 0;
	
	HRESULT hr = pPBD->get_PropColDef(&pColDef);
	hr = pPBD->get_ChildColDef(&pChildDef);

	long nCount = 0;


	// Obtenemos nuestro DataProps y aparte de pasarlo
	// a las hijas, nos lo quedamos para persistir
	CDataProperties *pCDataProps = NULL;
	ICDefProp * pDefProp = NULL;
	
	VARIANT vData;
	::VariantInit(&vData);
	vData.vt = VT_BSTR;
	vData.bstrVal = ::SysAllocString(L"DataProps");
	hr = pColDef->get_Item( vData, &pDefProp);
	if ((hr == S_OK) && (pDefProp))
	{
		CMyVariant var1;
		pDefProp->get_Value(&var1.m_vData);
		if (var1.m_vData.vt == VT_DISPATCH)
		{
			pCDataProps = (CDataProperties*)var1.m_vData.pdispVal;
		}
		RELEASE(pDefProp);
	}
	::VariantClear(&vData);
	
	// Primero recorremos las PBD hijas
	pChildDef->get_Count(&nCount);
	if (nCount > 0)
	{
		
		// Las recorremos en sentido inverso
		for(long tmpCount = nCount; tmpCount > 0; tmpCount--)
		{
			ICPropBagDef * pInnerPBD = NULL;
			VARIANT v;
			::VariantInit(&v);
			v.vt = VT_I4;
			v.lVal = tmpCount;
			hr = pChildDef->get_Item(v, &pInnerPBD);
			::VariantClear(&v);
			if (pInnerPBD)
			{

				nActualLength = RecProccess(pInnerPBD, pCDataProps);
				// Ahora escribimos la longitud y el tipo
				retValue += PersistLength(nActualLength);
				
				if (m_pASNBufferActual == m_pASNBuffer)
				{
					ExpandBuffer();
				}				
				*(--m_pASNBufferActual) = ASNSequence;
				m_nASNLength++;
				retValue+=nActualLength+1;
			}
			RELEASE(pInnerPBD);
		}
	}
	
	// Luego las Propiedades
	pColDef->get_Count(&nCount);
	if (nCount > 0)
	{
		// Las recorremos en sentido inverso
		for(long tmpCount = nCount; tmpCount > 0; tmpCount--)
		{
			ICDefProp * pDefProp = NULL;
			VARIANT v;
			::VariantInit(&v);
			v.vt = VT_I4;
			v.lVal = tmpCount;
			hr = pColDef->get_Item(v, &pDefProp);
			::VariantClear(&v);
			if (pDefProp)
			{
				// A persistir la información
				nActualLength = PersistProperty(pDefProp);
				m_nASNLength+=nActualLength;
				retValue+=nActualLength;
			}
			RELEASE(pDefProp);
		}
	}

	// Vamos a persistir la parte de DataProperties (si es aplicable)
	retValue += PersistDataProperties(pCDataProps, pData);
	
	// Luego el ClassName y el Alias
	BSTR bstrClassname = NULL;
	BSTR bstrAlias = NULL;

	pPBD->get_ClassName(&bstrClassname);
	pPBD->get_Alias(&bstrAlias);

	if (::SysStringLen(bstrAlias) > 0)
	{
		char* pTemp = M4VBToCpp(bstrAlias);
		nActualLength = PersistString(pTemp);
		delete pTemp;
		retValue += nActualLength;
		m_nASNLength+=nActualLength;
	}
	else
	{
		nActualLength = PersistString("Alias");
		retValue += nActualLength;
		m_nASNLength+=nActualLength;
	}
	nActualLength = PersistString("Alias");
	retValue += nActualLength;
	m_nASNLength+=nActualLength;
	
	if (::SysStringLen(bstrClassname) > 0)
	{
		char* pTemp = M4VBToCpp(bstrClassname);
		nActualLength = PersistString(pTemp);
		delete pTemp;
		retValue += nActualLength;
		m_nASNLength+=nActualLength;
	}
	else
	{
		nActualLength = PersistString("Classname");	
		retValue += nActualLength;
		m_nASNLength+=nActualLength;
	}

	nActualLength = PersistString("Classname");
	retValue += nActualLength;
	m_nASNLength+=nActualLength;
	
	::SysFreeString(bstrClassname);
	::SysFreeString(bstrAlias);

	RELEASE(pChildDef);
	RELEASE(pColDef);

	return retValue;
}

long CASNPersistPBD::PersistProperty(ICDefProp * pProp)
{
	USES_CONVERSION;
	long nRet = 0;
	M4PropertyType	eType = FE_NOTDEFINED;
	CMyVariant var1, var2;
	IM4FontHolder * pFontHolderValue = NULL;
	IM4FontHolder * pFontHolderDef = NULL;
	ITraductProp * pTraduct = NULL;
	char* tmpName = NULL;
	char* pTemp = NULL;

	BSTR tmpBSTR = NULL;
	pProp->get_Name(&tmpBSTR);
	tmpName = M4VBToCpp(tmpBSTR);
	::SysFreeString(tmpBSTR);
	tmpBSTR = NULL;
	
	pProp->get_Value(&var1.m_vData);
	pProp->get_DefValue(&var2.m_vData);
	pProp->get_Type(&eType);
	if (var1 != var2) 
	{
		if (var1.m_vData.vt == VT_DISPATCH)
		{
			// Buscamos nuestros objetos
			switch(eType)
			{
				case FE_TRADUCTPROP:
					pTraduct = (ITraductProp*)var1.m_vData.pdispVal;
					nRet = PersistTraductProp(pTraduct);
					break;
				case FE_FONT:
					pFontHolderValue = (IM4FontHolder*)var1.m_vData.pdispVal;
					pFontHolderDef = (IM4FontHolder*)var2.m_vData.pdispVal;
					if (pFontHolderValue && pFontHolderDef)
					{
						nRet = PersistFont(pFontHolderValue, pFontHolderDef);
					}
					break;
				case FE_DATAPROP:
					// Ya la tenemos controlada en el Bucle Principal

					break;
				default:
					break;
			}
		}
		else
		{
			switch(eType)
			{
				case FE_NUMBER:
				case FE_CONSTRUCTIF:
				case FE_COLOR:
				case FE_NUMBER_NOTDEFINED:
					if (var1.m_vData.vt == VT_I4)
					{
						nRet = PersistNumber(var1.m_vData.lVal);
					}
					else if(var1.m_vData.vt == VT_I2)
					{
						nRet = PersistNumber((long)var1.m_vData.iVal);
					}
					else if(var1.m_vData.vt == VT_UI1)
					{
						nRet = PersistNumber((long)var1.m_vData.bVal);
					}
					break;
				case FE_STRING:
				case FE_ALIGNTYPE:
				case FE_IMAGE:
				case FE_LN4_CODEARGS:
				case FE_DATE:
				case FE_STRING_NOTDEFINED:
					if (!m_bPersistIdHelpTopic)
					{
						if (strcmp(tmpName, "Idhelptopic") == 0)
						{
							delete tmpName;
							return 0;
						}
					}
					if (var1.m_vData.vt = VT_BSTR)
					{
						pTemp = M4VBToCpp(var1.m_vData.bstrVal);
						nRet = PersistString(pTemp);
						delete pTemp;
					}
					break;
				case FE_BOOLEAN:
				case FE_BOOLEAN_NOTDEFINED:
					if (var1.m_vData.vt = VT_BOOL)
					{
						nRet = PersistBool(var1.m_vData.boolVal);
					}
					break;
				default:
					// Debiera ser un FE_NOTDEFINED
					if (var1.m_vData.vt == VT_I4)
					{
						nRet = PersistNumber((long)var1.m_vData.lVal);
					}
					else if (var1.m_vData.vt == VT_I2)
					{
						nRet = PersistNumber((long)var1.m_vData.iVal);
					}
					else if (var1.m_vData.vt == VT_UI1)
					{
						nRet = PersistNumber((long)var1.m_vData.bVal);
					}
					else if (var1.m_vData.vt == VT_BOOL)
					{
						nRet = PersistBool(var1.m_vData.boolVal);	
					}
					else if (var1.m_vData.vt == VT_BSTR)
					{
						pTemp = M4VBToCpp(var1.m_vData.bstrVal);
						nRet = PersistString(pTemp);
						delete pTemp;
					}
					break;
			}
			nRet += PersistString(tmpName);
		}
	}

	delete tmpName;
	return nRet;
}

long CASNPersistPBD::PersistNumber(long nNumber)
{
	// Calculamos el espacio necesario
	long cont = 0;
	
	if ((nNumber <= 127) && (nNumber >= -128))
		cont = 1;
	else if ((nNumber <= 32767) && (nNumber >= -32768))
		cont = 2;
	else if ((nNumber <= 8388607) && (nNumber >= -8388608))
		cont = 3;
	else
		cont = 4;

	cont+=2; // Uno para el Tag y otro para la longitud;

	// Expandimos el Tamaño del Buffer si es necesario
	if ((m_pASNBufferActual - cont) < m_pASNBuffer)
	{
		ExpandBuffer();
	}

	// Vamos colocando Bytes
	// aux = m_value;
	int i = cont-2;
	long tmpMask;
	while(i-- > 0)
	{
		tmpMask = 255 << (8 * i);
		*(m_pASNBufferActual - i -1) = (unsigned char) ((nNumber & tmpMask) >> (8*i));
	}
	*(m_pASNBufferActual - cont +1) = cont-2;
	*(m_pASNBufferActual - cont) = ASNINTEGER;
	m_pASNBufferActual -= cont;
	return cont;
	
}

long CASNPersistPBD::PersistBool(short bBool)
{
	// Sabemos a Priori que ocupa tres bytes
	if ((m_pASNBufferActual - 3) < m_pASNBuffer)
	{
		ExpandBuffer();
	}
	
	*(m_pASNBufferActual -3) = ASNBOOLEAN;
	*(m_pASNBufferActual -2) = 1;
	if (bBool)
		*(m_pASNBufferActual-1) = 255;
	else
		*(m_pASNBufferActual-1) = 0;

	
	m_pASNBufferActual -= 3;
	return 3;

}

long CASNPersistPBD::PersistString(char * strValue)
{

	// Calculamos longitud
	long nLength = 0;
	long nLengthBytes = 0;
	unsigned char lBytes[5]; 
	long nStrLength = strlen(strValue);
	int i;
	
	if (nStrLength <= 127)
	{
		nLength = nStrLength + 2;
		nLengthBytes = 1;
		lBytes[0] = (unsigned char)nStrLength;
	}
	else
	{
		// Hay que hacer cálculos
		long aux = nStrLength;
		i = 0;
		while (aux != 0)
		{
			lBytes[i++] = aux & (0xff);
			aux >>= 8;
		}
		lBytes[i] = 128 + i;
		nLengthBytes = i+1;
		nLength = nStrLength+nLengthBytes+1;
	}

	if ((m_pASNBufferActual - nLength) < m_pASNBuffer)
	{
		ExpandBuffer();
	}

	memcpy(m_pASNBufferActual-nStrLength, strValue, nStrLength);

	if( M4IsUnicode() == 1 && M4IsAscii( strValue, nStrLength ) == 0 )
	{
		*(m_pASNBufferActual-nLength) = ASNUnicodeString;
	}
	else
	{
		*(m_pASNBufferActual-nLength) = ASNGeneralString;
	}

	for (i = 0; i< nLengthBytes; i++)
	{
		*(m_pASNBufferActual-nLength+1+i) = lBytes[nLengthBytes - i-1];
	}
	m_pASNBufferActual-=nLength;

	return nLength;
}

void CASNPersistPBD::ExpandBuffer()
{
	// Expandimos únicmente el Buffer del Main
	long nActualDist = m_pASNBufferFinal-m_pASNBufferActual;
	long nActualLong = m_pASNBufferFinal-m_pASNBuffer;
	BYTE * ptmpBuffer = (BYTE*)malloc((nActualLong+PERSIST_ASN_CHUNK)*sizeof(BYTE));

	memcpy(ptmpBuffer+PERSIST_ASN_CHUNK, m_pASNBuffer, nActualLong);
	
	free(m_pASNBuffer);
	m_pASNBuffer = ptmpBuffer;
	m_pASNBufferFinal = m_pASNBuffer + nActualLong+PERSIST_ASN_CHUNK;
	m_pASNBufferActual = m_pASNBufferFinal-nActualDist;

}

BSTR CASNPersistPBD::GetMainString()
{
	BSTR tmpBSTR = NULL;
	if (m_pASNBufferActual)
	{
		if (m_pLog)
		{
			sprintf(m_pTempBuffer, " Getting Main Buffer Length: %i. RealLength: %i", m_nASNLength, (m_pASNBuffer-m_pASNBufferActual));
			m_pLog->Write(m_pTempBuffer);
		}

		// asignacion de la cadena ASN al BSTR de salida
		// UNICODE VB
		tmpBSTR = M4CppToVBBinary((const char*)m_pASNBufferActual, m_nASNLength);
	}
	return tmpBSTR;
}

BSTR CASNPersistPBD::GetLangString(long nLang)
{
	// Nos Basamos en las diferencias para obtener el valor de la longitud
	BSTR tmpBSTR = NULL;
	if ((nLang >= 0) && (nLang<m_nLangNumber))
	{

		long nLength = m_ppASNLangBufferFinal[nLang] - m_ppASNLangBufferActual[nLang];
		if (m_pLog)
		{
			sprintf(m_pTempBuffer, " Getting Lang Buffer #%i Length: %i", nLang, nLength);
			m_pLog->Write(m_pTempBuffer);
		}

		// UNICODE VB
		tmpBSTR = M4CppToVBBinary((const char*)m_ppASNLangBufferActual[nLang], nLength);
	}
	return tmpBSTR;
}

long CASNPersistPBD::PersistLength(long nLength)
{

	unsigned char lBytes[5];
	long nTotalLength = 0;

	if (nLength <= 127)
	{
		nTotalLength = 1;
		lBytes[0] = (unsigned char)nLength;
	}
	else
	{
		// Hay que hacer cálculos
		long aux = nLength;
		int i = 0;
		while (aux != 0)
		{
			lBytes[i++] = aux & (0xff);
			aux >>= 8;
		}
		lBytes[i] = 128 + i;
		nTotalLength = i+1;
	}

	long nSize = nTotalLength;

	if (m_pASNBufferActual-nTotalLength < m_pASNBuffer)
	{
		ExpandBuffer();
	}
	
	m_pASNBufferActual-=nSize;
	while(nTotalLength > 0)
	{
		*(m_pASNBufferActual+nTotalLength-1) = lBytes[nSize - nTotalLength];
		nTotalLength--;
	}
	//m_pASNBufferActual-=nSize;
	m_nASNLength += nSize;
	return nSize;

}

long CASNPersistPBD::PersistFont(IM4FontHolder * pM4FontV, IM4FontHolder * pM4FontDef)
{	
	long nRet = 0, nActual = 0;

	USES_CONVERSION;	

	BSTR sFontV = NULL;
	BSTR sFontDef = NULL;
	char* pTemp;

	//-------------------------------------
	//Name.
	pM4FontV->get_Name(&sFontV);
	pM4FontDef->get_Name(&sFontDef);
	if (wcscmp(sFontV, sFontDef) != 0)
	{
		pTemp = M4VBToCpp(sFontV);
		nRet += PersistString(pTemp);
		delete pTemp;
		nRet += PersistString("Fontname");
	}
	::SysFreeString(sFontV);
	::SysFreeString(sFontDef);
	sFontV = NULL;
	sFontDef = NULL;
	//-------------------------------------	

	//-------------------------------------			
	//Bold.
	pM4FontV->get_FontBold(&sFontV);			
	pM4FontDef->get_FontBold(&sFontDef);
	if (wcscmp(sFontV, sFontDef) != 0)
	{
		pTemp = M4VBToCpp(sFontV);
		nRet += PersistString(pTemp);
		delete pTemp;
		nRet += PersistString("Fontbold");
	}
	::SysFreeString(sFontV);
	::SysFreeString(sFontDef);
	sFontV = NULL;
	sFontDef = NULL;
	//-------------------------------------

	//-------------------------------------			
	//Italic.
	pM4FontV->get_FontItalic(&sFontV);			
	pM4FontDef->get_FontItalic(&sFontDef);
	if (wcscmp(sFontV, sFontDef) != 0)
	{
		pTemp = M4VBToCpp(sFontV);
		nRet += PersistString(pTemp);
		delete pTemp;
		nRet += PersistString("Fontitalic");
	}
	::SysFreeString(sFontV);
	::SysFreeString(sFontDef);
	sFontV = NULL;
	sFontDef = NULL;
	//-------------------------------------

	//-------------------------------------			
	//Underline.
	pM4FontV->get_FontUnderline(&sFontV);			
	pM4FontDef->get_FontUnderline(&sFontDef);
	if (wcscmp(sFontV, sFontDef) != 0)
	{
		pTemp = M4VBToCpp(sFontV);
		nRet += PersistString(pTemp);
		delete pTemp;
		nRet += PersistString("Fontunderline");
	}
	::SysFreeString(sFontV);
	::SysFreeString(sFontDef);
	sFontV = NULL;
	sFontDef = NULL;
	//-------------------------------------

	//-------------------------------------	
	//Size.
	pM4FontV->get_FontSize(&sFontV);			
	pM4FontDef->get_FontSize(&sFontDef);
	if (wcscmp(sFontV, sFontDef) != 0)
	{
		pTemp = M4VBToCpp(sFontV);
		nRet += PersistString(pTemp);
		delete pTemp;
		nRet += PersistString("Fontsize");
	}
	::SysFreeString(sFontV);
	::SysFreeString(sFontDef);
	sFontV = NULL;
	sFontDef = NULL;
	//-------------------------------------

	return nRet;
}

long CASNPersistPBD::PersistDataProperties(CDataProperties * pActualData, CDataProperties * pInheritData)
{
	USES_CONVERSION;
	
	long retValue = 0;
	long tmpLength = 0;
	char *pstrIdChannel = NULL;
	char *pstrIdChannelAlias = NULL;
	char *pstrIdChannelKey = NULL;
	char *pstrIdAccessAlias = NULL;
	char *pstrIdItem = NULL;
	char *pstrIdNode = NULL;
	short nGrants = 31;
	long nIdPresentation = -1;
	long nStartPos = -6;

	
	if ((pInheritData != NULL) && (pActualData != NULL))
	{

		// primero comprobamos si se está heredando o no
		if (pActualData->m_nmIdChannel == pInheritData->m_nmIdChannel)
		{
			// El canal se está heredando, no hacemos nada
			if (pActualData->m_nmIdNode != pInheritData->m_nmIdNode)
			{
				// Ha cambiado el Nodo
				pstrIdNode = GetcharFromId(pActualData->m_nmIdNode);
			}
			if (pActualData->m_nmIdItem != pInheritData->m_nmIdItem)
			{
				// Ha cambiado el Item
				pstrIdItem = GetcharFromId(pActualData->m_nmIdItem);
			}
		}
		else
		{
			pstrIdChannel = GetcharFromId(pActualData->m_nmIdChannel);
			pstrIdNode = GetcharFromId(pActualData->m_nmIdNode);
			pstrIdItem = GetcharFromId(pActualData->m_nmIdItem);
		}

		if (pActualData->m_nmIdChannelAlias != pInheritData->m_nmIdChannelAlias)
		{
			// Ha cambiado el ChannelAlias
			pstrIdChannelAlias = GetcharFromId(pActualData->m_nmIdChannelAlias);
		}
		if (pActualData->m_nmIdChannelKey != pInheritData->m_nmIdChannelKey)
		{
			// Ha cambiado el ChannelKey
			pstrIdChannelKey = GetcharFromId(pActualData->m_nmIdChannelKey);
		}
		if (pActualData->m_nmIdAccessAlias != pInheritData->m_nmIdAccessAlias)
		{
			// Ha cambiado el AccessAlias
			pstrIdAccessAlias = GetcharFromId(pActualData->m_nmIdAccessAlias);
		}
		if (pActualData->m_nGrants != pInheritData->m_nGrants)
		{
			// Ha cambiado el Grants
			nGrants = pActualData->m_nGrants;
		}
		if (pActualData->m_nIdPresentation != pInheritData->m_nIdPresentation)
		{
			// Ha cambiado el Grants
			nIdPresentation = pActualData->m_nIdPresentation;
		}
		if (pActualData->m_nStartPos != pInheritData->m_nStartPos)
		{
			// Ha cambiado el StartPos
			nStartPos = pActualData->m_nStartPos;
		}
	}

	// Si no estamos heredando, quizá es el primer elemento
	if ((pInheritData == NULL) && (pActualData))
	{
		pstrIdChannel = GetcharFromId(pActualData->m_nmIdChannel);
		pstrIdNode = GetcharFromId(pActualData->m_nmIdNode);
		pstrIdItem = GetcharFromId(pActualData->m_nmIdItem);
		pstrIdChannelAlias = GetcharFromId(pActualData->m_nmIdChannelAlias);
		pstrIdChannelKey = GetcharFromId(pActualData->m_nmIdChannelKey);
		pstrIdAccessAlias = GetcharFromId(pActualData->m_nmIdAccessAlias);
		nGrants = pActualData->m_nGrants;
		nIdPresentation = pActualData->m_nIdPresentation;
		nStartPos = pActualData->m_nStartPos;
	}
	
	// Hala, a persistir !
	if (nStartPos != m_nStartPos)
	{
		tmpLength = PersistNumber(nStartPos);
		tmpLength += PersistString("Startpos");
		m_nASNLength += tmpLength;
		retValue+=tmpLength;
	}
	if (nIdPresentation != m_nIdPresentation)
	{
		tmpLength = PersistNumber(nIdPresentation);
		tmpLength += PersistString("Idpresentation");
		m_nASNLength += tmpLength;
		retValue+=tmpLength;
	}

	if (nGrants != m_nGrants)
	{
		tmpLength = PersistNumber(nGrants);
		tmpLength += PersistString("Grants");
		m_nASNLength += tmpLength;
		retValue+=tmpLength;
	}

	if ((pstrIdAccessAlias) && (strcmp(pstrIdAccessAlias, m_pstrIdAccessAlias) !=0))
	{
		tmpLength = PersistString(pstrIdAccessAlias);
		tmpLength += PersistString("Idaccessalias");
		m_nASNLength += tmpLength;
		retValue+=tmpLength;
	}

	if ((pstrIdChannelAlias) && (strcmp(pstrIdChannelAlias, m_pstrIdChannelAlias) !=0))
	{
		tmpLength = PersistString(pstrIdChannelAlias);
		tmpLength += PersistString("Idchannelalias");
		m_nASNLength += tmpLength;
		retValue+=tmpLength;
	}

	if ((pstrIdChannelKey) && (strcmp(pstrIdChannelKey, m_pstrIdChannelKey) !=0))
	{
		tmpLength = PersistString(pstrIdChannelKey);
		tmpLength += PersistString("Idchannelkey");
		m_nASNLength += tmpLength;
		retValue+=tmpLength;
	}

	if ((pstrIdItem) && (strcmp(pstrIdItem, m_pstrIdItem) !=0))
	{
		tmpLength = PersistString(pstrIdItem);
		tmpLength += PersistString("Iditem");
		m_nASNLength += tmpLength;
		retValue+=tmpLength;
	}

	if ((pstrIdNode) && (strcmp(pstrIdNode, m_pstrIdNode) !=0))
	{
		tmpLength = PersistString(pstrIdNode);
		tmpLength += PersistString("Idnode");
		m_nASNLength += tmpLength;
		retValue+=tmpLength;
	}

	if ((pstrIdChannel) && (strcmp(pstrIdChannel, m_pstrIdChannel) !=0))
	{
		tmpLength = PersistString(pstrIdChannel);
		tmpLength += PersistString("Idchannel");
		m_nASNLength += tmpLength;
		retValue+=tmpLength;
	}

	return retValue;
}

long CASNPersistPBD::PersistTraductProp(ITraductProp * pTraduct)
{
	USES_CONVERSION;
	
	
	long nRet = 0;
	long nRetActual = 0;
	VARIANT_BOOL bResult = 0; // Partimos de False
	BSTR bstrResult = NULL;
	char* pTemp;

	if (pTraduct)
	{
		pTraduct->TestEqual(&bResult);
		if (bResult)
		{
			pTraduct->get_ValueByLng(CComVariant((long)1), &bstrResult);
			if (wcscmp(bstrResult, L"") != 0)
			{
				pTemp = M4VBToCpp(bstrResult);
				nRetActual = PersistString(pTemp);
				delete pTemp;

				// jcr Hay que liberar la memoria
				::SysFreeString(bstrResult);

				pTraduct->get_Property(&bstrResult);
				pTemp = M4VBToCpp(bstrResult);
				nRetActual += PersistString(pTemp);
				delete pTemp;
				nRet += nRetActual;
				//m_nASNLength += nRetActual;
			}
			// jcr Hay que liberar la memoria
			::SysFreeString(bstrResult);
		}
		else
		{
			// Hacemos el Split en multilenguaje
			// Los diferentes bloques van a los diferentes Buffers
			// Sólo va al Bloque principal el "NombrePropiedad" = "_Ref#"
			
			// Leemos el identificador en la propiedad
			BSTR bstrTemp = NULL;
			char * pcRef = NULL;

			//m_nRefCount += 1;
			//char refLabel[10];
			//sprintf(refLabel, "_Ref%d", m_nRefCount);

			pTraduct->get_Identifier(&bstrTemp);
			pcRef = M4VBToCpp(bstrTemp);
			SysFreeString(bstrTemp);
			pTraduct->get_Property(&bstrResult);
			nRetActual = PersistString(pcRef);

			pTemp = M4VBToCpp(bstrResult);
			nRetActual += PersistString(pTemp);
			delete pTemp;

			// jcr Hay que liberar la memoria
			::SysFreeString(bstrResult);

			nRet += nRetActual;
			//m_nASNLength += nRetActual;
			
			for (int i = 1; i <= m_nLangNumber; i++)
			{
				pTraduct->get_ValueByLng(CComVariant((long)i), &bstrResult);
				pTemp = M4VBToCpp(bstrResult);
				PersistLangString((long) i-1, pTemp);
				delete pTemp;

				// jcr Hay que liberar la memoria
				::SysFreeString(bstrResult);

				PersistLangString((long) i-1, pcRef);
			}
			delete pcRef;
		}
	}
	//::SysFreeString(bstrResult);
	return nRet;
}

void CASNPersistPBD::TerminateLangData()
{
	USES_CONVERSION;
	long nActualLength = 0;

	for (long i = 0; i < m_nLangNumber; i++)
	{
		BSTR bstrTmp = NULL;
		m_pTraduct->get_ID(i+1, &bstrTmp);
		char* pTemp = M4VBToCpp(bstrTmp);
		::SysFreeString(bstrTmp);		
		PersistLangString(i, pTemp);
		delete pTemp;
		
		PersistLangString(i, "Alias");
		PersistLangString(i, "RefSeq");
		PersistLangString(i, "Classname");
		
		// Creamos la PB que contiene al resto
		PersistLangLength(i,m_pnASNLangLength[i]);

		*(--m_ppASNLangBufferActual[i]) = ASNSequence;
		m_pnASNLangLength[i]++;
	}	
}

void CASNPersistPBD::PersistLangString(long nLang, char * strValue)
{
	// 0 Base
	
	// Calculamos longitud
	long nLength = 0;
	long nLengthBytes = 0;
	unsigned char lBytes[5];
	long nStrLength = strlen(strValue);
	int i;
	
	if (nStrLength <= 127)
	{
		nLength = nStrLength + 2;
		nLengthBytes = 1;
		lBytes[0] = (unsigned char)nStrLength;
	}
	else
	{
		// Hay que hacer cálculos
		long aux = nStrLength;
		i = 0;
		while (aux != 0)
		{
			lBytes[i++] = aux & (0xff);
			aux >>= 8;
		}
		lBytes[i] = 128 + i;
		nLengthBytes = i+1;
		nLength = nStrLength+nLengthBytes+1;
	}
	
	// Se nos envía la información 1 Base. La pasamos a 0 Base
	//nLang=1;
	if ((nLang >=0) && (nLang < m_nLangNumber))
	{
		if ((m_ppASNLangBufferActual[nLang] - nLength) < m_ppASNLangBuffer[nLang])
		{
			ExpandLangBuffer(nLang);
		}

		memcpy(m_ppASNLangBufferActual[nLang]-nStrLength, strValue, nStrLength);

		if( M4IsUnicode() == 1 && M4IsAscii( strValue, nStrLength ) == 0 )
		{
			*(m_ppASNLangBufferActual[nLang]-nLength) = ASNUnicodeString;
		}
		else
		{
			*(m_ppASNLangBufferActual[nLang]-nLength) = ASNGeneralString;
		}

		for (i = 0; i< nLengthBytes; i++)
		{
			*(m_ppASNLangBufferActual[nLang]-nLength+i+1) = lBytes[nLengthBytes-1-i];
		}

		m_ppASNLangBufferActual[nLang]-=nLength;
		m_pnASNLangLength[nLang]+=nLength;
	}
}

void CASNPersistPBD::PersistLangLength(long nLang, long nLength)
{

	// 0 base
	
	if ((nLang >= 0) && (nLang < m_nLangNumber))
	{
		unsigned char lBytes[5];
		long nTotalLength = 0;

		if (nLength <= 127)
		{
			nTotalLength = 1;
			lBytes[0] = (unsigned char)nLength;
		}
		else
		{
			// Hay que hacer cálculos
			long aux = nLength;
			int i = 0;
			while (aux != 0)
			{
				lBytes[i++] = aux & (0xff);
				aux >>= 8;
			}
			lBytes[i] = 128 + i;
			nTotalLength = i+1;
		}

		long nSize = nTotalLength;

		if (m_ppASNLangBufferActual[nLang]-nTotalLength < m_ppASNLangBuffer[nLang])
		{
			ExpandLangBuffer(nLang);
		}
		
		m_ppASNLangBufferActual[nLang]-=nSize;
		while(nTotalLength > 0)
		{
			*(m_ppASNLangBufferActual[nLang]+nTotalLength-1) = lBytes[nSize - nTotalLength];
			nTotalLength--;
		}
		m_pnASNLangLength[nLang] += nSize;
	}
}

void CASNPersistPBD::ExpandLangBuffer(long nLang)
{
	if ((nLang >= 0) && (nLang < m_nLangNumber))
	{
		long nActualDist = m_ppASNLangBufferFinal[nLang]-m_ppASNLangBufferActual[nLang];
		long nActualLong = m_ppASNLangBufferFinal[nLang]-m_ppASNLangBuffer[nLang];
		BYTE * ptmpBuffer = (BYTE*)malloc((nActualLong+PERSIST_ASN_LANG_CHUNK)*sizeof(BYTE));

		memcpy(ptmpBuffer+PERSIST_ASN_LANG_CHUNK, m_ppASNLangBuffer[nLang], nActualLong);
		
		free(m_ppASNLangBuffer[nLang]);
		m_ppASNLangBuffer[nLang] = ptmpBuffer;
		m_ppASNLangBufferFinal[nLang] = m_ppASNLangBuffer[nLang] + nActualLong+PERSIST_ASN_LANG_CHUNK;
		m_ppASNLangBufferActual[nLang] = m_ppASNLangBufferFinal[nLang]-nActualDist;
	}
}

void CASNPersistPBD::SetLogManager(CLogManager * pLog)
{
	m_pLog = pLog;
	if (m_pLog)
		m_pTempBuffer = (char*)malloc(100*sizeof(char));
}

void CASNPersistPBD::PersistIdHelpTopic(BOOL bPersist)
{
	if (bPersist)
		m_bPersistIdHelpTopic = -1;
	else
		m_bPersistIdHelpTopic = 0;
}
