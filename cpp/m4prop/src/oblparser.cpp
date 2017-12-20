// OBLParser.cpp : Implementation of COBLParser
#include "stdafx.h"
#include "m4stl.hpp"
#include "m4prop.h"
#include "oblparser.h"
#include "cpropcoldef.h"
#include "cchildcoldef.h"
#include "cpropbagdef.h"
#include "m4unicode.hpp"


/////////////////////////////////////////////////////////////////////////////
// COBLParser

STDMETHODIMP COBLParser::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IOBLParser,
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


void COBLParser::Free()
{
	free(m_pText);

	m_pText = NULL;
	m_pTextActual = NULL;
}

STDMETHODIMP COBLParser::PropBagDefFromOBL(Iveranle * pAnle, BSTR bstrText, ICPropBagDef * * pRetPropBag)
{
	if (pRetPropBag == NULL) return E_INVALIDARG;

	RELEASE((*pRetPropBag));

	// Liberamos lo anterior
	Free();
	
	if (pAnle)
	{
		RELEASE(m_pAnle);
		m_pAnle = pAnle;
		m_pAnle->AddRef();
	}

	m_pText = M4VBToCpp(bstrText);
	m_pTextActual = m_pText;
	m_pTextFinal = m_pText + strlen(m_pText);

	// Creamos la PBD que contendrá las que existan el el OBL
	CComObject<CCPropBagDef> * pPBTemp;
	HRESULT hr = CComObject<CCPropBagDef>::CreateInstance(&pPBTemp);
	if (hr == S_OK)
	{
		pPBTemp->QueryInterface(IID_ICPropBagDef, (void**)pRetPropBag);
		CCPropBagDef * pPBDef = static_cast<CCPropBagDef*>(*pRetPropBag);
		pPBDef->ChangeClassName("TextContents");
		pPBDef->ChangeAlias("TextContents");
		RecProcess(*pRetPropBag);
		Free();
		return S_OK;
	}
	return E_FAIL;
}

char * COBLParser::NextLine()
{

	*m_pBufferTemp = 0;
	if (m_pText && m_pTextActual && m_pBufferTemp)
	{
		do
		{
			*m_pBufferTemp = 0;
			// Si hemos llegado al final nos vamos
			if (m_pTextActual >= m_pTextFinal) break;

			long nInit = 0, nFin = 0, nPos = 0;
			while((*(m_pTextActual+nInit) == 0x20) || (*(m_pTextActual+nInit) == 0x9)) nInit++;
			nPos = nInit;
			// Tenemos la parte inicial
			while((*(m_pTextActual+nPos) != 0xD) && (*(m_pTextActual+nPos) != 0xA) && (*(m_pTextActual+nPos) != 0x0)) nPos++;

			// Buscamos la parte final
			while((*(m_pTextActual+nPos-nFin-1) == 0x20) || (*(m_pTextActual+nPos-nFin-1) == 0x9)) nFin++;

			while((nPos - nFin) >= m_nTempBufferSize)
			{
				ExpandTempBuffer();
			}

			strncpy(m_pBufferTemp, m_pTextActual + nInit, nPos - nFin);
			if ((nPos - nFin) > 0)
			{
				// Terminamos con un Cero
				*(m_pBufferTemp + nPos - nFin - nInit) = 0;
			}

			// El retorno de carro puede ser 0xD 0xA o sólo 0xD, la antigua línea no vale (adri) 
			// if ((*(m_pTextActual+nPos) == 0xD) && (*(m_pTextActual+nPos+1) == 0xA)) nPos+=2;
			while ((*(m_pTextActual + nPos) == 0xD) || (*(m_pTextActual + nPos) == 0xA)) nPos ++;

			m_pTextActual += nPos;

		} while(strlen(m_pBufferTemp) == 0);
	}
	return m_pBufferTemp;
}
 

ProccessReturnValue COBLParser::ProcessLine(char * pcBuffer)
{
	if (pcBuffer)
	{
		if (strnicmp("BEGIN ", pcBuffer, 6) == 0)
		{
			// Tenemos inicio de ClassName
			ProcessNewClass(pcBuffer);
			return RET_NEWCLASS;
		}
		else if (strnicmp("END", pcBuffer, 3) == 0)
		{
			//Bug 0106960
			//Comprobamos si hay algo más alla del END.
			if (strlen(pcBuffer) > 3) {
				//Comprobamos el caracter que hay despues del END.
				if (isspace(*(pcBuffer + 3 + 1)) != 0) {
					//Es un espacio.
					return RET_ENDCLASS;
				}
				else {
					//Es una propiedad que empieza por END.
					ProcessProperty(pcBuffer);
					return RET_PROPERTY;					
				}
			}
			else {
				// Final de Bloque
				return RET_ENDCLASS;
			}
		}
		else
		{
			// Buscamos una propiedad
			ProcessProperty(pcBuffer);
			return RET_PROPERTY;
		}
	}
	
	return RET_ERROR;
}

void COBLParser::ProcessNewClass(char * pcBuffer)
{
	// buscamos la estructura BEGIN ClassName Alias
	//                              ¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨  

	*m_pcClassName = 0;
	*m_pcAlias = 0;
	long nLength = strlen(pcBuffer)-6;
	char * pTemp = (char*) malloc(nLength+1);

	// Nos cargamos el "BEGIN " y trimamos los bordes
	memcpy(pTemp, pcBuffer+6, nLength);
	*(pTemp+nLength) = 0;

	CutLine(&pTemp);
	
	long nPos = 0;
	while ((*(pTemp+nPos) != 0x20) && (*(pcBuffer+nPos) != 0x9) && (*(pcBuffer+nPos) != 0)) nPos++;

	if (nPos)
	{
		while (nPos > m_nClassSize)
		{
			ExpandClassNameBuffer();
		}
		memcpy(m_pcClassName, pTemp, nPos);
		*(m_pcClassName + nPos) = 0;
	}

	char * pTemp2 = NULL;

	if ((nLength - nPos +1) > 0)
		 pTemp2 = (char*) malloc(nLength+1-nPos);
	else
		return;

	memcpy(pTemp2, pTemp+nPos, nLength-nPos);
	*(pTemp2+nLength-nPos) = 0;
	free(pTemp);

	CutLine(&pTemp2);

	while((long)strlen(pTemp2) > m_nAliasSize)
	{
		ExpandAliasBuffer();
	}
	memcpy(m_pcAlias, pTemp2, strlen(pTemp2));
	*(m_pcAlias + strlen(pTemp2)) = 0;
	free(pTemp2);
}

void COBLParser::ProcessProperty(char * pcBuffer)
{
	// Hay que buscar el "="
	long nPos = 0;
	while ((*(pcBuffer + nPos) != '=') && (*(pcBuffer + nPos) != 0x0)) nPos++;

	char * pTemp = (char*) malloc(nPos + 1);
	memcpy(pTemp, pcBuffer, nPos);
	*(pTemp+nPos) = 0;
	CutLine(&pTemp);

	while ((long)strlen(pTemp) > m_nPropNameSize)
		ExpandPropNameBuffer();

	strcpy(m_pcPropName, pTemp);

	if (nPos == (long)strlen(pcBuffer))
	{
		*m_pcPropValue = 0;
		return;
	}

	long nLength = strlen(pcBuffer) - nPos-1;
	pTemp = (char*) realloc(pTemp, nLength+1);
	*(pTemp+nLength) = 0;
	memcpy(pTemp, pcBuffer + nPos + 1, nLength);
	CutLine(&pTemp);

	while((long)strlen(pTemp) > m_nPropValueSize)
		ExpandPropValueBuffer();
	
	// Copiamos el contenido entre las comillas
	long nLength2 = strlen(pTemp);
	if (*pTemp == '"')
	{
		if (nLength2==1)
		{
			// Caso especial de una sola comilla
			*(m_pcPropValue) = 0;
		}
		else 
		{
			if (*(pTemp + nLength2 -1) == '"')
			{
				// Copiamos sin final de Comillas
				memcpy(m_pcPropValue, pTemp+1, nLength2-2);
				*(m_pcPropValue+nLength2-2) = 0;
			}
			else
			{
				memcpy(m_pcPropValue, pTemp+1, nLength2-1);
				*(m_pcPropValue+nLength2-1) = 0;
			}
		}
	}
	else
	{
		if (*(pTemp + nLength2 -1) == '"')
		{
			// Copiamos sin final de Comillas
			memcpy(m_pcPropValue, pTemp, nLength2-1);
			*(m_pcPropValue+nLength2-1) = 0;
		}
		else
			strcpy(m_pcPropValue, pTemp);   // No hay Comillas, ni al principio ni al final
	}
	
	free(pTemp);
}

void COBLParser::CutLine(char * * ppcBuffer)
{
	long nInit = 0, nFin = 0, nLength = strlen(*ppcBuffer);
	char * pBuff = *ppcBuffer;
	
	while((*(pBuff+nInit) == 0x20) || (*(pBuff+nInit) == 0x9)) nInit++;
	while((*(pBuff-nFin+nLength-1) == 0x20) || (*(pBuff-nFin+nLength-1) == 0x9)) nFin++;

	if (nInit || nFin)
	{
		char * pBuff2 = (char*) malloc(nLength - nInit - nFin +1);
		memcpy(pBuff2, pBuff+nInit, nLength - nInit - nFin);
		*(pBuff2 + nLength - nInit - nFin) = 0;
		free(pBuff);
		*ppcBuffer = pBuff2;
	}
}

void COBLParser::ExpandClassNameBuffer()
{
	m_nClassSize *=2;
	m_pcClassName = (char*)realloc((void*)m_pcClassName, m_nClassSize+1);
}

void COBLParser::ExpandAliasBuffer()
{
	m_nAliasSize *=2;
	m_pcAlias = (char*)realloc((void*)m_pcAlias, m_nAliasSize+1);
}

void COBLParser::ExpandPropNameBuffer()
{
	m_nPropNameSize *=2;
	m_pcPropName = (char*)realloc((void*)m_pcPropName, m_nPropNameSize+1);
}

void COBLParser::ExpandPropValueBuffer()
{
	m_nPropValueSize *=2;
	m_pcPropValue = (char*)realloc((void*)m_pcPropValue, m_nPropValueSize+1);
}

ICPropBagDef * COBLParser::CreateNewPropBagDef(char * pcClassName, char * pcAlias)
{
	USES_CONVERSION;
	
	ICPropBagDef * pPBD = NULL;
	if (m_pAnle)
	{
		BSTR bstrClassName = M4CppToVB(pcClassName);
		if (m_pAnle->get_PropBagDefFromClassname(bstrClassName, &pPBD) == S_OK)
		{
			CCPropBagDef * pCPBD = static_cast<CCPropBagDef*>(pPBD);
			pCPBD->ChangeAlias(pcAlias);
		}
		::SysFreeString(bstrClassName);
	}
	return pPBD;
}

void COBLParser::RecProcess(ICPropBagDef * pPBD)
{
	char * theBuffer = NextLine();

	long nLength = strlen(theBuffer);
	while (nLength > 0)
	{
		ProccessReturnValue nType = ProcessLine(theBuffer);	
		if (nType == RET_NEWCLASS)
		{
			// Primero comprobamos que no sea una propiedad Traducida
			if (strcmp("TraductProp", m_pcClassName) == 0)
			{
				// Habemus Propiedad Traducida
				ReadTraductProp(pPBD);
			}
			else
			{
				ICPropBagDef * pPBDChild = NULL;
				pPBDChild = CreateNewPropBagDef(m_pcClassName, m_pcAlias);
				// Primero lo añadimos, con lo que recibe sus propiedades heredadas
				// Y luego leemos los valores que tenga
				if (pPBDChild)
				{
					pPBD->AddChild(&pPBDChild,CComVariant());
					RecProcess(pPBDChild);
					RELEASE(pPBDChild);
				}
				else
					return;
			}
		}
		else if (nType == RET_PROPERTY)
		{
			InsertProperty(pPBD);
		}
		else if (nType == RET_ENDCLASS)
		{
			return;
		}
		theBuffer = NextLine();
		nLength = strlen(theBuffer);
	}

}

void COBLParser::InsertProperty(ICPropBagDef * pPBD)
{
	long nId = GetIdFromPropName(m_pcPropName);
	
	if (nId == 0x8000)
	{
		// Iditem
		InsertDataProperty(PROP_IDITEM, pPBD);
	}
	else if (nId == 0x8001)
	{
		// Idnode
		InsertDataProperty(PROP_IDNODE, pPBD);
	}
	else if (nId == 0x8002)
	{
		// Idchannel
		InsertDataProperty(PROP_IDCHANNEL, pPBD);
	}
	else if (nId == 0x8003)
	{
		// IdchannelAlias
		InsertDataProperty(PROP_IDCHANNELALIAS, pPBD);
	}
	else if (nId == 0x8004)
	{
		// IdchannelKey
		InsertDataProperty(PROP_IDCHANNELKEY, pPBD);
	}
	else if (nId == 0x8005)
	{
		// IdaccessAlias
		InsertDataProperty(PROP_IDACCESSALIAS, pPBD);
	}
	else if (nId == 0x8006)
	{
		// Idpresentation
		InsertDataProperty(PROP_IDPRESENTATION, pPBD);
	} 
	else if (nId == 0x8007)
	{
		// Grants
		InsertDataProperty(PROP_GRANTS, pPBD);
	}
	else if (nId == 0x8008)
	{
		// Startpos
		InsertDataProperty(PROP_STARTPOS, pPBD);
	}
	// Ahora las de tipo Font
	else if (strncmp("Font", m_pcPropName, 4) == 0)
		InsertFontProperty(pPBD);		
	else
		InsertGeneralProperty(pPBD);
}

void COBLParser::InsertDataProperty(DataType eType, ICPropBagDef * pPBD)
{

	USES_CONVERSION;
	
	IDataProperties * pData = NULL;

	CComVariant vValue;
	if (pPBD->Property(M4CComBSTR("DataProps"), &vValue) == S_OK)
	{
		if (vValue.vt == VT_DISPATCH)
		{
			if (((IDispatch*)vValue.pdispVal)->QueryInterface(IID_IDataProperties, (void**)&pData) == S_OK)
			{
				switch (eType)
				{
					case PROP_IDCHANNEL:
						pData->put_IdChannel(M4CComBSTR(m_pcPropValue));
						break;
					case PROP_IDNODE:
						pData->put_IdNode(M4CComBSTR(m_pcPropValue));
						break;
					case PROP_IDITEM:
						pData->put_IdItem(M4CComBSTR(m_pcPropValue));
						break;
					case PROP_IDCHANNELALIAS:
						pData->put_IdChannelAlias(M4CComBSTR(m_pcPropValue));
						break;
					case PROP_IDCHANNELKEY:
						pData->put_IdChannelKey(M4CComBSTR(m_pcPropValue));
						break;
					case PROP_IDACCESSALIAS:
						pData->put_IdAccessAlias(M4CComBSTR(m_pcPropValue));
						break;
					case PROP_IDPRESENTATION:
						pData->put_IdPresentation(atol(m_pcPropValue));
						break;
					case PROP_GRANTS:
						pData->put_Grants((short)atol(m_pcPropValue));
						break;
					case PROP_STARTPOS:
						pData->put_StartPos(atol(m_pcPropValue));
						break;
				}
				RELEASE(pData);
			}
		}
	}
	else
	{
		CCPropBagDef * pInnerPBD = static_cast<CCPropBagDef*>(pPBD);
		CCPropColDef * pInnerPC = static_cast<CCPropColDef*>(pInnerPBD->m_PropCol);
		
		DefProp * pDP = pInnerPC->GetDefProp(m_pcPropName);
		if (pDP)
		{
			if (pDP->Type == FE_STRING || pDP->Type == FE_STRING_NOTDEFINED || pDP->Type == FE_STRING)
			{
				free(pDP->Value.pstrValue);
				pDP->Value.pstrValue = (char*)malloc(strlen(m_pcPropValue)+1);
				strcpy(pDP->Value.pstrValue, m_pcPropValue);
			}
			else if (pDP->Type == FE_NUMBER)
			{
				pDP->Value.lValue = atol(m_pcPropValue);
			}
		}
	}

}

void COBLParser::InsertFontProperty(ICPropBagDef * pPBD)
{
	// Hay que eliminar los casos no contemplados
	if ((strcmp("Fontname", m_pcPropName) == 0) || (strcmp("Font", m_pcPropName) == 0))
	{
		// Sólo en estos casos abrimos el objeto
		IM4FontHolder * pM4FontDisp = NULL;
		CComVariant vValue;
		if (pPBD->Property(M4CComBSTR("Font"), &vValue) == S_OK)
		{
			if (vValue.vt == VT_DISPATCH)
			{
				if (((IDispatch*)vValue.pdispVal)->QueryInterface(IID_IM4FontHolder, (void**)&pM4FontDisp) == S_OK)
				{
					pM4FontDisp->put_Name(M4CComBSTR(m_pcPropValue));
					RELEASE(pM4FontDisp);
					return;
				}
			}
		}
	}
	else if (strcmp("Fontsize", m_pcPropName) == 0)
	{
		// Sólo en estos casos abrimos el objeto
		IM4FontHolder * pM4FontDisp = NULL;
		CComVariant vValue;
		if (pPBD->Property(M4CComBSTR("Font"), &vValue) == S_OK)
		{
			if (vValue.vt == VT_DISPATCH)
			{
				if (((IDispatch*)vValue.pdispVal)->QueryInterface(IID_IM4FontHolder, (void**)&pM4FontDisp) == S_OK)
				{
					USES_CONVERSION;

					BSTR sSize = M4CppToVB(m_pcPropValue);
					pM4FontDisp->put_FontSize(sSize);
					::SysFreeString(sSize);

					RELEASE(pM4FontDisp);
					return;
				}
			}
		}
	}
	else if (strcmp("Fontbold", m_pcPropName) == 0)
	{
		// Sólo en estos casos abrimos el objeto
		IM4FontHolder * pM4FontDisp = NULL;
		CComVariant vValue;
		if (pPBD->Property(M4CComBSTR("Font"), &vValue) == S_OK)
		{
			if (vValue.vt == VT_DISPATCH)
			{
				if (((IDispatch*)vValue.pdispVal)->QueryInterface(IID_IM4FontHolder, (void**)&pM4FontDisp) == S_OK)
				{
					if (stricmp("True", m_pcPropValue) == 0) {
						pM4FontDisp->put_FontBold(L"True");
					}
					else {
						pM4FontDisp->put_FontBold(L"False");
					}

					RELEASE(pM4FontDisp);
					return;
				}
			}
		}
	}
	else if (strcmp("Fontitalic", m_pcPropName) == 0)
	{
		// Sólo en estos casos abrimos el objeto
		IM4FontHolder * pM4FontDisp = NULL;
		CComVariant vValue;
		if (pPBD->Property(M4CComBSTR("Font"), &vValue) == S_OK)
		{
			if (vValue.vt == VT_DISPATCH)
			{
				if (((IDispatch*)vValue.pdispVal)->QueryInterface(IID_IM4FontHolder, (void**)&pM4FontDisp) == S_OK)
				{
					if (stricmp("True", m_pcPropValue) == 0) {
						pM4FontDisp->put_FontItalic(L"True");
					}
					else {
						pM4FontDisp->put_FontItalic(L"False");
					}

					RELEASE(pM4FontDisp);
					return;
				}
			}
		}
	}
	else if (strcmp("Fontunderline", m_pcPropName) == 0)
	{
		// Sólo en estos casos abrimos el objeto
		IM4FontHolder * pM4FontDisp = NULL;
		CComVariant vValue;
		if (pPBD->Property(M4CComBSTR("Font"), &vValue) == S_OK)
		{
			if (vValue.vt == VT_DISPATCH)
			{
				if (((IDispatch*)vValue.pdispVal)->QueryInterface(IID_IM4FontHolder, (void**)&pM4FontDisp) == S_OK)
				{
					if (stricmp("True", m_pcPropValue) == 0) {
						pM4FontDisp->put_FontUnderline(L"True");
					}
					else {
						pM4FontDisp->put_FontUnderline(L"False");
					}

					RELEASE(pM4FontDisp);
					return;
				}
			}
		}
	}
	else
		InsertGeneralProperty(pPBD);
}

void COBLParser::InsertGeneralProperty(ICPropBagDef * pPBD)
{
	// A pillarla a capón
	ICPropColDef * pPCD;

	HRESULT hr = pPBD->get_PropColDef(&pPCD);
	if (hr == S_OK)
	{
		ICDefProp * pDP;
		hr = pPCD->get_Item(M4CComVariant(m_pcPropName), &pDP);
		if (hr == S_OK)
		{
			M4PropertyType m4Type;
			pDP->get_Type(&m4Type);
			if ((m4Type == FE_STRING) || (m4Type == FE_ALIGNTYPE) || (m4Type == FE_IMAGE) || (m4Type == FE_LN4_CODEARGS) || (m4Type == FE_DATE) || (m4Type == FE_NOTDEFINED))
			{
				// Insertamos el valor de forma directa
				pDP->put_Value(M4CComVariant(m_pcPropValue));
			}
			else if (m4Type == FE_TRADUCTPROP)
			{
				// La metemos como si no fuese traducida
				// Tenemos que meterlo en su lugar correspondiente
				ITraductProp * pTraduct;
				CComVariant vValue;
				hr = pDP->get_Value(&vValue);
				if (hr == S_OK)
				{
					if (vValue.vt == VT_DISPATCH)
					{
						hr = ((IDispatch*)vValue.pdispVal)->QueryInterface(IID_ITraductProp, (void**)&pTraduct);
						if (hr == S_OK)
						{
							pTraduct->AllValues(M4CComBSTR(m_pcPropValue));
							RELEASE(pTraduct);
						}
					}
				}
			}
			else if ((m4Type == FE_NUMBER) || (m4Type == FE_COLOR) || (m4Type == FE_CONSTRUCTIF) || (m4Type == FE_GRANTSTYPE))
			{
				pDP->put_Value(CComVariant(atol(m_pcPropValue)));
			}
			else if (m4Type == FE_BOOLEAN)
			{
				if (stricmp("true", m_pcPropValue) == 0)
					pDP->put_Value(CComVariant((bool)-1)); // True
				else
					pDP->put_Value(CComVariant((bool)0)); // False
			}
			RELEASE(pDP);
		}
		else
		{
			// No existe, luego habrá que insertarla
			// Miramos a ver si es numérica
			long l = atol(m_pcPropValue);
			if ((l == 0) && (strcmp("0", m_pcPropValue) != 0))
			{
				// Es una cadena
				if (stricmp("true", m_pcPropValue) == 0)
				{
					// Booleano
					pPBD->AddProperty(M4CComBSTR(m_pcPropName), CComVariant((bool)-1), CComVariant(), FE_NOTDEFINED);
				}
				else if (stricmp("false", m_pcPropValue) == 0)
				{	
					// Booleano
					pPBD->AddProperty(M4CComBSTR(m_pcPropName), CComVariant((bool)0), CComVariant(), FE_NOTDEFINED);
				}
				else
				{
					// Es cadena
					pPBD->AddProperty(M4CComBSTR(m_pcPropName), M4CComVariant(m_pcPropValue), CComVariant(), FE_NOTDEFINED);
				}
			}
			else
			{
				// es un número
				pPBD->AddProperty(M4CComBSTR(m_pcPropName), CComVariant(l), CComVariant(), FE_NOTDEFINED);
			}
		}
		RELEASE(pPCD);
	}
}


void COBLParser::ReadTraductProp(ICPropBagDef * pPBD)
{
	ITraductProp * pTraduct = NULL;
	
	long nCount = 0;
	/* Bug 0257534
	Se pone un vector en vez de un array
	*/
	vector<char*> pContName, pContValue;

	pContName.reserve(128);
	pContValue.reserve(128);

	char * pTemp = NextLine();
	long nLength = strlen(pTemp);
	while (nLength > 0)
	{
		// Llenamos nuestros buffers
		ProccessReturnValue nType = ProcessLine(pTemp);
		if (nType == RET_NEWCLASS)
		{
			// No válido
			return;
		}
		else if (nType == RET_PROPERTY)
		{
			// Rellenamos nuestros Buffers
			pContName.push_back((char*) malloc(strlen(m_pcPropName)+1));
			strcpy(pContName[nCount], m_pcPropName);
			pContValue.push_back((char*) malloc(strlen(m_pcPropValue)+1));
			strcpy(pContValue[nCount], m_pcPropValue);
			nCount++;
		}
		else if (nType == RET_ENDCLASS)
		{
			break;
		}
		pTemp = NextLine();
		nLength = strlen(pTemp);
	}
	// Leemos el valor del nombre de la propiedad.
	long i;
	for (i = 0; i<nCount; i++)
	{
		if (strcmp("Name", pContName[i]) == 0)
		{
			CComVariant vValue;
			if (pPBD->Property(M4CComBSTR(pContValue[i]), &vValue) == S_OK)
			{
				if (vValue.vt == VT_DISPATCH)
				{	
					if(((IDispatch*)vValue.pdispVal)->QueryInterface(IID_ITraductProp, (void**)&pTraduct) == S_OK)
					{
						break;
					}
				}
			}
		}
	}

	if (pTraduct)
	{
		pTraduct->put_Identifier(M4CComBSTR(m_pcAlias));

		for (i =0; i< nCount; i++)
		{
			/* Bug 0202330
			El lang del idioma no tiene por qué ser de 3
			*/
			if (strncmp("Value_", pContName[i], 6) == 0)
			{
				pTraduct->put_ValueByLng(M4CComVariant(pContName[i] + 6), M4CComBSTR(pContValue[i]));
			}
		}
		RELEASE(pTraduct);
	}
	
	// Liberamos memoria
	for (i =0; i< nCount; i++)
	{
		free(pContName[i]);
		free(pContValue[i]);
	}
}

void COBLParser::ExpandTempBuffer()
{
	m_nTempBufferSize *=2;
	m_pBufferTemp = (char*)realloc((void*)m_pBufferTemp, m_nTempBufferSize+1);
}


