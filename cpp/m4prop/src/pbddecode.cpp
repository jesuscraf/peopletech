// PBDDecode.cpp: implementation of the CPBDDecode class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "stdafx.h"
#include "m4prop.h"
#include "cchildcoldef.h"
#include "cpropcoldef.h"
#include "cpropbagdef.h"
#include "persistobject.h"
#include "traductprop.h"
#include "convertpropbag.h"

#ifdef PROP_STAT
#include "csingleprop.h"
#include "cpropcol.h"
#endif

#include "pbddecode.h"
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

CPBDDecode::CPBDDecode()
{
	m_nLangNumber = 0;
	m_ppTransPB = NULL;
	m_pConvert = NULL;
	m_pAnle = NULL;
#ifdef PROP_STAT
	m_pPropCol = NULL;
#endif

	if( M4IsUnicode() == 0 )
	{
		m_iStringBaseType = ASNGeneralString;
	}
	else
	{
		m_iStringBaseType = ASNUnicodeString;
	}
}

CPBDDecode::~CPBDDecode()
{
	FreeData();
	RELEASE(m_pAnle);
#ifdef PROP_STAT
	RELEASE(m_pPropCol);
#endif
}

HRESULT CPBDDecode::InitFromPersistObject(IPersistObject * pPO)
{
	HRESULT hr = E_INVALIDARG;
	USES_CONVERSION;

	if (m_pConvert && pPO)
	{
		// Primero obtenemos los elementos de traducción
		ITraductProp * pTraduct = NULL;
		hr = pPO->get_PersistTraductProp(&pTraduct);
		if (hr == S_OK)
		{
			short nLang=0;
			
			if (m_nLangNumber)
			{
				FreeData();
			}
			
			pTraduct->get_PropertyNumber(&nLang);
			if (nLang)
			{
				m_nLangNumber = nLang;
				m_ppTransPB = (ICPropBag **)malloc(m_nLangNumber * sizeof(ICPropBag*));
				
				for (long i = 0; i < m_nLangNumber; i++)
				{
					CTraductProp * pCTraductProp = static_cast<CTraductProp*>(pTraduct);
					long nDataLength = ::SysStringLen(pCTraductProp->m_pBSTRData[i]);

					// UNICODE VB
					char* tmpBuffer = M4VBToCppBinary( pCTraductProp->m_pBSTRData[i], nDataLength ) ;

					m_ppTransPB[i] = ConstructPBFromASN1( (unsigned char*) tmpBuffer, NULL, NULL);

					delete[] tmpBuffer;
				}
			}
			RELEASE(pTraduct);
		}
	}
	return hr;
}

void CPBDDecode::SetConvert(CConvertPropBag * pConvert)
{
	// Le asignamos el puntero a su padre
	m_pConvert = pConvert;
}

void CPBDDecode::FreeData()
{
	ICPropBag * pPBTemp;
	if (m_nLangNumber && m_ppTransPB)
	{
		for (long i = 0; i< m_nLangNumber; i++)
		{
			pPBTemp = m_ppTransPB[i];
			if (pPBTemp != NULL)
			{
				RELEASE(pPBTemp);
			}
		}
		delete [] m_ppTransPB;
		m_ppTransPB = NULL;
		m_nLangNumber = 0;
	}
}

void CPBDDecode::SetAnle(Iveranle * pAnle)
{
	RELEASE(m_pAnle);
	m_pAnle = pAnle;
	if (m_pAnle)
	{
		m_pAnle->AddRef();
	}
}

ICPropBagDef * CPBDDecode::DecodeMainString(IPersistObject * pPO)
{
	USES_CONVERSION;

#ifdef PROP_STAT
	RELEASE(m_pPropCol);
	CComObject<CCPropCol> * pPr;
	if (CComObject<CCPropCol>::CreateInstance(&pPr) == S_OK)
	{
		pPr->QueryInterface(IID_ICPropCol, (void**)&m_pPropCol);
	}
#endif


	if (pPO && m_pAnle && m_ppTransPB)
	{
		CPersistObject * pPersistObject = static_cast<CPersistObject *>(pPO);
		long nDataLength = ::SysStringLen(pPersistObject->m_strMain);

		// UNICODE VB
		BYTE* tmpBuffer = (BYTE*) M4VBToCppBinary( pPersistObject->m_strMain, nDataLength ) ;
		BYTE* tmpInit = tmpBuffer;

		// Tenemos las condiciones.  m_pAnle, m_ppTransPB, tmpBuffer
		BYTE bType;
		long nLength = ReadType(&tmpBuffer, &bType);
		if (bType == ASNSequence)
		{
			// Vamos a por la primera PropBagDefTipada
			ReadLength(&tmpBuffer, &nLength);
			ICPropBagDef * pPBD = NULL;
			long nLength2 = GetPBDefFromASN(&tmpBuffer, &pPBD);
			if (pPBD)
			{
				nLength2 += ReadASNRec(&tmpBuffer, nLength - nLength2, pPBD);
				if (nLength2 == nLength)
				{
					delete[] tmpInit;
					return pPBD;
				}
				RELEASE(pPBD);
			}
		}

		delete[] tmpBuffer;
	}
	return NULL;
}

long CPBDDecode::ReadType(BYTE * * bBuffer, BYTE * bType)
{
	*bType = *(*bBuffer);
	(*bBuffer)++;
	return 1;
}


long CPBDDecode::GetPBDefFromASN(BYTE * * bBuffer, ICPropBagDef * * ppPBResult)
{
	// Creamos una CPropBagDef a partir del código ASN
	// Estamos posicionados para leer ClassName + Alias
	USES_CONVERSION;
	BYTE bType;

	long nLength = ReadType(bBuffer, &bType);

	if (bType == ASNGeneralString || bType == ASNUnicodeString)
	{
		long nTempLength;
		char * tmpBuffer = NULL;
		nLength += ReadLength(bBuffer, &nTempLength);
		nLength += ReadString(bBuffer, nTempLength, &tmpBuffer, bType);
		if (strcmp(tmpBuffer, "Classname") == 0)
		{
			free(tmpBuffer);
			tmpBuffer = NULL;
			nLength += ReadType(bBuffer,&bType);
			if (bType == ASNGeneralString || bType == ASNUnicodeString)
			{
				nLength += ReadLength(bBuffer, &nTempLength);
				nLength += ReadString(bBuffer, nTempLength, &tmpBuffer, bType);
				BSTR bstrTemp = M4CppToVB(tmpBuffer);
				free(tmpBuffer);
				tmpBuffer = NULL;

				HRESULT hr = m_pAnle->get_PropBagDefFromClassname(bstrTemp, ppPBResult);
				::SysFreeString(bstrTemp);
				bstrTemp = NULL;

				if (hr == S_OK)
				{
					// Ya tenemos la PropBagDef, vamos a por su Alias	
					nLength += ReadType(bBuffer, &bType);
					if (bType == ASNGeneralString || bType == ASNUnicodeString)
					{
						nLength += ReadLength(bBuffer, &nTempLength);
						nLength += ReadString(bBuffer, nTempLength, &tmpBuffer, bType);
						if (strcmp(tmpBuffer, "Alias") == 0)
						{
							free(tmpBuffer);
							tmpBuffer = NULL;
							nLength += ReadType(bBuffer, &bType);
							if (bType == ASNGeneralString || bType == ASNUnicodeString)
							{
								nLength += ReadLength(bBuffer, &nTempLength);
								nLength += ReadString(bBuffer, nTempLength, &tmpBuffer, bType);
								CCPropBagDef* pPBD = static_cast<CCPropBagDef*> (*ppPBResult);
								if (pPBD)
								{
									pPBD->ChangeAlias(tmpBuffer);
								}
								free(tmpBuffer);
								tmpBuffer = NULL;
							}
						}
					}
				}
			}
		}
	}
	return nLength;
}

long CPBDDecode::ReadLength(BYTE** bBuffer, long* nLength)
{
	BYTE bValue;
	
	*nLength = 0;
	bValue = *(*bBuffer);
	(*bBuffer)++;
	if(bValue>128)
	{
		BYTE bMask;
		bValue -= 128;
		long nRetValue = bValue +1;
		for (int i = bValue; i > 0; i--)
		{
			bMask = *(*bBuffer);
			(*bBuffer)++;
			*nLength += (bMask<<8*(i-1));
		}
		return nRetValue;

	}
	*nLength = bValue;	
	return 1;
}

long CPBDDecode::ReadString(BYTE** bBuffer, long nTempLength, char** tmpBuffer, char type)
{
	*tmpBuffer = (char*)malloc((nTempLength+1) * sizeof(char));
	memcpy(*tmpBuffer,(*bBuffer),nTempLength);
	(*bBuffer) += nTempLength;
	*(*tmpBuffer + nTempLength) = 0;

	if( type != m_iStringBaseType && M4IsAscii( *tmpBuffer, nTempLength ) == 0 )
	{
		char* pcValue = *tmpBuffer;
		int iLength = nTempLength;

		if( type == ASNUnicodeString )
		{
			// Si es Utf8 se pasa a ANSI
			*tmpBuffer = M4Utf8ToCpp( pcValue, iLength );
		}
		else
		{
			// Si es ANSI se pasa a Utf8
			*tmpBuffer = M4ANSIToCpp( pcValue, iLength );
		}
		delete pcValue;
	}

	return nTempLength;
}

long CPBDDecode::ReadASNRec(BYTE** bBuffer, long nBytesToRead, ICPropBagDef* pParent)
{
	// Empezamos a leer los datos
	// Miramos si es una nueva PropBagDef

	if (nBytesToRead == 0)
	{
		return 0;
	}
	
	BYTE bType;
	long nLength = ReadType(bBuffer, &bType);

	do
	{
		if (bType == ASNSequence)
		{
			// Vamos a añadir una PropBagDef Hija
			long nPBLength;
			nLength += ReadLength(bBuffer, &nPBLength);
			ICPropBagDef * pPBD = NULL;
			long nHead = GetPBDefFromASN(bBuffer, &pPBD);
			// Una vez que hemos leído la cabecera podemos empezar a contar
			nLength += nHead;
			if (pPBD)
			{
				// Añadimos la PBDef y entramos de forma recursiva
				pParent->AddChild(&pPBD, CComVariant());
				// Volvemos a entrar de forma recursiva
				nLength += ReadASNRec(bBuffer, nPBLength - nHead, pPBD);
				RELEASE(pPBD);
			}
		}
		else if (bType == ASNGeneralString || bType == ASNUnicodeString)
		{
			// Estamos leyendo una propiedad
			nLength += ReadProperty(bBuffer, pParent, bType);
		}
		else
		{
			// No suena muy bien
			return 0;
		}
		
		if (nLength < nBytesToRead)
		{
			nLength += ReadType(bBuffer, &bType);
		}
	}while (nLength < nBytesToRead);

	return nLength;
}

long CPBDDecode::ReadProperty(BYTE** bBuffer, ICPropBagDef* pParent, char type)
{
	// Estamos posicionados en la longitud del nombre de la propiedad
	USES_CONVERSION;
	long nTmpLength = 0;
	char * tmpName;
	BYTE bType;
	long nLength = ReadLength(bBuffer, &nTmpLength);

	// Leemos el Nombre
	nLength += ReadString(bBuffer, nTmpLength, &tmpName, type);
	
	// Hacemos la conversión hacia el mapa de propiedades
	long nId = GetIdFromPropName(tmpName);

	// Metemos un HOOK para conocer la distribución de propiedades
#ifdef PROP_STAT

	ICSingleProp * pSP = NULL;

	if (m_pPropCol->get_Item(M4CComVariant(tmpName), &pSP) == S_OK)
	{
		long nValue = 0;
		CComVariant vValue;
		pSP->get_Value(&vValue);
		pSP->put_Value(CComVariant(vValue.lVal+1));
		RELEASE(pSP);
	}
	else
	{
		CComObject<CCSingleProp> * pCSP = NULL;
		CComObject<CCSingleProp>::CreateInstance(&pCSP);
		pCSP->QueryInterface(&pSP);
		pSP->put_Name(M4CComBSTR(tmpName));
		pSP->put_Value(CComVariant((long)1));
		m_pPropCol->Add(pSP, M4CComVariant(tmpName));
		RELEASE(pSP);
	}


#endif

	nLength += ReadType(bBuffer, &bType);
	// Vamos a por el valor
	if (bType == ASNGeneralString || bType == ASNUnicodeString)
	{
		char * tmpValue = NULL;

		// Tenemos propiedad de tipo String
		nLength += ReadLength(bBuffer, &nTmpLength);
		nLength += ReadString(bBuffer, nTmpLength, &tmpValue, bType);
		// Hay que comprobar en función de la propiedad si entra en
		// TraductProp, en DataProps o en FontProps
		if (nId & 0x8000)
		{
			// Es una propiedad de datos
			InsertPropInDataProps(pParent, nId, tmpValue);
		}
		else if (nId & 0x800)
		{
			// Propiedad de tipo Font
			if (!InsertFontPropInFont(pParent, nId, tmpValue))
			{
				//Si no se ha podido meter lo metemos a fuego como numérico!.
				InsertFontPropInFont(pParent, nId, atol(tmpValue));
			}
		}
		else
		{
			InsertStringProperty2PropBagDef(pParent, tmpName, tmpValue);
		}
		free(tmpValue);
	}
	else if (bType == ASNBOOLEAN)
	{
		// A leer un dato Boolean
		VARIANT_BOOL bData = 0;
		nLength += ReadBoolean(bBuffer, &bData);
		if (nId & 0x800)
		{
			// Propiedad de tipo Font
			InsertFontPropInFont(pParent, nId, bData);
		}
		else
		{
			InsertBooleanProperty2PropBagDef(pParent, tmpName, bData);
		}
	}
	else if (bType == ASNINTEGER)
	{
		// A leer un dato numérico
		long nData = 0;
		nLength += ReadNumber(bBuffer, &nData);
		if (nId & 0x8000)
		{
			InsertPropInDataProps(pParent, nId, nData);
		}
		else if (nId & 0x800)
		{
			if (!InsertFontPropInFont(pParent, nId, nData))
			{
				if (nId == 0x802 || nId == 0x803 || nId == 0x804)
				{
					// Es booleano (y yo me pregunto ¿QUÉ COÑO HACE UNA PROPIEDAD BOOLEANA PERSISTIDA COMO ENTERO?)
					if (nData)
					{
						InsertFontPropInFont(pParent, nId, (VARIANT_BOOL) -1);
					}
					else
					{
						InsertFontPropInFont(pParent, nId, (VARIANT_BOOL) 0);
					}
				}
			}
		}
		else
		{
			InsertNumberProperty2PropBagDef(pParent, tmpName, nData);
		}
	}
	else
	{
		// La jodimos
		return 0;
	}

	free(tmpName);
	return nLength;
}

void CPBDDecode::InsertStringProperty2PropBagDef(ICPropBagDef* pParent, char* tmpName, char* tmpValue)
{
	long	nL = 0;
	size_t	iLength = 0 ;
	char	*pcBool = NULL ;
	char	acBuffer[ 128 + 1 ] ;

	
	CCPropBagDef * pPBD = static_cast<CCPropBagDef*> (pParent);
	DefProp * pDefProp = pPBD->GetDefProp(tmpName);
		
	if (pDefProp)
	{
		if (pDefProp->Type == FE_STRING || pDefProp->Type == FE_DATE || pDefProp->Type == FE_IMAGE || pDefProp->Type == FE_ALIGNTYPE || pDefProp->Type == FE_LN4_CODEARGS)
		{
			nL = strlen(tmpValue);
			free(pDefProp->Value.pstrValue);
			pDefProp->Value.pstrValue = (char*) malloc(nL+1);
			strcpy(pDefProp->Value.pstrValue, tmpValue);
		}
		else if (pDefProp->Type == FE_TRADUCTPROP)
		{
			ITraductProp * pTrad = static_cast<ITraductProp*> (pDefProp->Value.pdispValue);
			if (pTrad)
			{
				InsertValueOfTraductProp(tmpValue, pTrad);
			}
		}
		else if (pDefProp->Type == FE_BOOLEAN || pDefProp->Type == FE_BOOLEAN_NOTDEFINED)
		{
			// Leemos el valor
			if (stricmp(tmpValue, "true") == 0)
			{
				pDefProp->Value.boolValue = -1;
			}
			else if (strcmp(tmpValue, "0") == 0)
			{
				pDefProp->Value.boolValue = 0;
			}
			/* Bug 107861
			Si viene un ##DEFAULT_VALUE## en una propiedad booleana hay que respetarlo
			Se pone el tipo de propiedad a cadena y se copia el valor
			Se convierte su valor por defecto booleano a cadena
			*/
			else if (strcmp(tmpValue, "##DEFAULT_VALUE##") == 0)
			{
				pDefProp->Type = FE_STRING ;
				iLength = strlen( tmpValue ) + 1 ;
				pDefProp->Value.pstrValue = (char*) malloc( iLength ) ;
				memcpy( pDefProp->Value.pstrValue, tmpValue, iLength ) ;

				if( pDefProp->DefValue.boolValue )
				{
					pcBool = "-1" ;
				}
				else
				{
					pcBool = "0" ;
				}

				iLength = strlen( pcBool ) + 1 ;
				pDefProp->DefValue.pstrValue = (char*) malloc( iLength ) ;
				memcpy( pDefProp->DefValue.pstrValue, pcBool, iLength ) ;
			}
			else if (atol(tmpValue) != 0)
			{
				// Si estamos aquí, tempValue es numérico distinto de cero (True)
				pDefProp->Value.boolValue = -1;
			}
			else
			{
				pDefProp->Value.boolValue = 0;
			}
		}
		else
		{
			/* Bug 107861
			Si viene un ##DEFAULT_VALUE## en una propiedad numérica hay que respetarlo
			Se pone el tipo de propiedad a cadena y se copia el valor
			Se convierte su valor por defecto numérico a cadena
			*/
			pDefProp->Type = FE_STRING ;
			iLength = strlen( tmpValue ) + 1 ;
			pDefProp->Value.pstrValue = (char*) malloc( iLength ) ;
			memcpy( pDefProp->Value.pstrValue, tmpValue, iLength ) ;

			sprintf( acBuffer, "%ld", pDefProp->DefValue.lValue ) ;
			iLength = strlen( acBuffer ) + 1 ;
			pDefProp->DefValue.pstrValue = (char*) malloc( iLength ) ;
			memcpy( pDefProp->DefValue.pstrValue, acBuffer, iLength ) ;
		}
	}
	else
	{
		pParent->AddProperty(M4CComBSTR(tmpName), M4CComVariant(tmpValue), CComVariant(), FE_NOTDEFINED);
	}
}

void CPBDDecode::FillNullTraductProp(char* pcValue, ITraductProp* pTraduct)
{
	int				iPos = 0;
	bool			bNullPropBag = false;
	short			iCount = 0;
	ICPropBag *		pPropBag = NULL;
	
	pTraduct->get_PropertyNumber(&iCount);
	for (iPos = 0; iPos < iCount; iPos++)
	{
		// Verificar si hay algún propbag que es nulo.
		pPropBag = m_ppTransPB[iPos];
		if (pPropBag == NULL)
		{
			bNullPropBag = true;
			break;
		}
	}
	
	// Si se tiene al menos un propbag nulo, se tiene que rellenar su valor.
	pPropBag = NULL;
	if (bNullPropBag == true)
	{
		// Quedarse con un propbag no nulo.
		for (iPos = 0; iPos < iCount; iPos++)
		{
			pPropBag = m_ppTransPB[iPos];
			if (pPropBag != NULL)
			{
				break;
			}
		}
		
		// Establecer el valor de la traduct prop para aquellos propbag nulos.
		if (pPropBag != NULL)
		{
			ICPropCol *	pPropCol = NULL;
			if (pPropBag->get_PropCol(&pPropCol) == S_OK)
			{
				ICSingleProp * pSingleProp = NULL;
				if (pPropCol->get_Item(M4CComVariant(pcValue), &pSingleProp) != S_OK)
				{
					BSTR bstrIdentifier = M4CppToVB(pcValue);
					pTraduct->AllValues(bstrIdentifier);
					::SysFreeString(bstrIdentifier);
				}
				
				RELEASE(pSingleProp);
			}
			
			RELEASE(pPropCol);
		}
	}
	
	return;
}


void CPBDDecode::InsertValueOfTraductProp(char* pcValue, ITraductProp* pTraduct)
{
	USES_CONVERSION;
	// Buscamos si se encuentra en la bolsa de propiedades de las propertyBags de multilenguaje
/*	if (strncmp("_", pcValue, 1) == 0)
	{*/
	// Hay que traducirla

	// Si nos han llamado sin idioma salimos con el valor
	if( m_ppTransPB == NULL )
	{
		BSTR bstrIdentifier = M4CppToVB(pcValue);
		// Bug 0132444.
		pTraduct->AllValues( bstrIdentifier ) ;
		::SysFreeString(bstrIdentifier);
		return;
	}

	// Bug 0144403. Rellenar las traductprop que vienen a nulas.
	FillNullTraductProp(pcValue, pTraduct);

	short nCount;
	pTraduct->get_PropertyNumber(&nCount);
	for (long i = 0; i < nCount; i++)
	{
		// El orden debería ser el correcto, pero por si acaso hacemos una comprobación y búsqueda
		for (long j = i; j < (i+nCount); j++)
		{
			BSTR bstrID1 = NULL, bstrID2 = NULL;
			BSTR bstrIdentifier = M4CppToVB(pcValue);
			
			pTraduct->get_ID((j%nCount)+1, &bstrID1);
			ICPropBag * pPropBag = m_ppTransPB[i];
			if (pPropBag != NULL)
			{
				pPropBag->get_Alias(&bstrID2);
				
				if (wcscmp(bstrID1, bstrID2) == 0)
				{
					// Tenemos la equivalencia
					ICSingleProp * pSP;
					ICPropCol * pPC;
					if (pPropBag->get_PropCol(&pPC) == S_OK)
					{
						CComVariant vValue;
						if (pPC->get_Item(M4CComVariant(pcValue), &pSP) == S_OK)
						{
							pSP->get_Value(&vValue);
							
							/* Bug 0102283
							Si venía algo que no era cadena entraba igualmente
							*/
							if (vValue.vt == VT_BSTR)
							{
								pTraduct->put_ValueByLng(CComVariant((j%nCount)+1), vValue.bstrVal);
								pTraduct->put_Identifier(bstrIdentifier);
							}
							RELEASE(pSP);
						}
						else
						{
							// Metemos el valor genérico
							// Bug 0144226. Se pierden las propiedades traducibles del tipo 
							// Text = "##CHNNL[XXX]" cuando se genera el OBL desde el Pack Manager.
							pTraduct->put_ValueByLng(CComVariant((j%nCount)+1), bstrIdentifier);
						}
						RELEASE(pPC);
					}
					::SysFreeString(bstrID1);
					::SysFreeString(bstrID2);
					// jcr Hay que liberar la memoria
					::SysFreeString(bstrIdentifier);
					break;
				}
			}
			::SysFreeString(bstrID1);
			::SysFreeString(bstrID2);
			::SysFreeString(bstrIdentifier);
		}
	}
	/*}
	else
	{
		// Se mete en todos los Values
		BSTR bstrValue = M4CppToVB(pcValue);
		pTraduct->AllValues(bstrValue);
		::SysFreeString(bstrValue);
	}*/
}

void CPBDDecode::InsertNumberProperty2PropBagDef(ICPropBagDef* pPB, char* pcName, long nData)
{
	
	CCPropBagDef * pPBD = static_cast<CCPropBagDef*> (pPB);
	DefProp * pDefProp = pPBD->GetDefProp(pcName);
		
	if (pDefProp)
	{
		if (pDefProp->Type == FE_NUMBER || pDefProp->Type == FE_COLOR || pDefProp->Type == FE_CONSTRUCTIF || pDefProp->Type == FE_GRANTSTYPE)
		{
			pDefProp->Value.lValue = nData;
		}
		else if (pDefProp->Type == FE_BOOLEAN || pDefProp->Type == FE_BOOLEAN_NOTDEFINED)
		{
			if (nData)
			{
				pDefProp->Value.boolValue = -1;
			}
			else
			{
				pDefProp->Value.boolValue = 0;
			}
		}
	}
	else
	{
		pPB->AddProperty(M4CComBSTR(pcName), CComVariant(nData), CComVariant(), FE_NOTDEFINED);
	}
	
}

long CPBDDecode::ReadNumber(BYTE** bBuffer, long* nData)
{
	long nMask = 0xFFFFFFFF;
	bool bNegative = false;
	BYTE bValue;
	long nDataLength;
	*nData = 0;
	
	long nLength = ReadLength(bBuffer, &nDataLength);
	nLength += nDataLength;

	for (long i = nDataLength ; i > 0; i--)
	{
		nMask <<= 8;
		bValue = *(*bBuffer);
		(*bBuffer)++;
		if ((i == nDataLength) && (bValue & 0x80))
		{
			bNegative = true;
		}
		*nData += ((long)bValue<<8*(i-1));
	}

	if (bNegative)
	{
		*nData |= nMask;
	}

	return nLength;
}

long CPBDDecode::ReadBoolean(BYTE** bBuffer, VARIANT_BOOL* bValue)
{
	long nBoolLength;
	BYTE tmpValue;

	// Leemos longitud
	long nLength = ReadLength(bBuffer, &nBoolLength);
	tmpValue = *(*bBuffer);
	(*bBuffer)++;
	if (tmpValue == 0)
	{
		*bValue = 0;
	}
	else
	{
		*bValue = -1;
	}

	return nBoolLength+1;
}

void CPBDDecode::InsertBooleanProperty2PropBagDef(ICPropBagDef* pPB, char* pcName, VARIANT_BOOL bData)
{
	CCPropBagDef * pPBD = static_cast<CCPropBagDef*> (pPB);
	DefProp * pDefProp = pPBD->GetDefProp(pcName);
		
	if (pDefProp)
	{
		if (pDefProp->Type == FE_BOOLEAN)
		{
			if (bData)
			{
				pDefProp->Value.boolValue = -1;
			}
			else
			{
				pDefProp->Value.boolValue = 0;
			}
		}
		else if (pDefProp->Type == FE_NUMBER)
		{
			pDefProp->Value.lValue = (long)bData;
		}
		else if (pDefProp->Type == FE_STRING)
		{
			char * tmpValue = (char*)malloc(10);
			
			if (bData == 0)
			{
				strcpy(tmpValue, "False");
			}
			else
			{
				strcpy(tmpValue, "True");
			}

			long nL = strlen(tmpValue);
			free(pDefProp->Value.pstrValue);
			pDefProp->Value.pstrValue = (char*) malloc(nL+1);
			strcpy(pDefProp->Value.pstrValue, tmpValue);
			free(tmpValue);
		}
	}
	else
	{
		if (bData)
		{
			pPB->AddProperty(M4CComBSTR(pcName), CComVariant((bool)-1), CComVariant(), FE_NOTDEFINED);
		}
		else
		{
			pPB->AddProperty(M4CComBSTR(pcName), CComVariant((bool)0), CComVariant(), FE_NOTDEFINED);
		}
	}
}

bool CPBDDecode::InsertPropInDataProps(ICPropBagDef* pParent, long nId, char* pcValue)
{
	USES_CONVERSION;
	
	// Obtenemos la propiedad
	CCPropBagDef * pPBD = static_cast<CCPropBagDef*> (pParent);
	IDataProperties * pDataProp = NULL;
	CDataProperties * pDataDirect = NULL;
	DefProp * pDefProp = pPBD->GetDefProp("DataProps");
	
	if (pDefProp)
	{
		if (pDefProp->Type == FE_DATAPROP)
		{
			pDataProp = (IDataProperties*) pDefProp->Value.pdispValue;
			pDataDirect = static_cast<CDataProperties*> (pDataProp);
		}
		
		if (pDataProp)
		{
			switch (nId)
			{
				case 0x8000:
					pDataDirect->m_nmIdItem = GetIdFromChar(pcValue);
					// Iditem
					break;
				case 0x8001:
					pDataDirect->m_nmIdNode = GetIdFromChar(pcValue);
					// Idnode
					break;
				case 0x8002:
					pDataDirect->m_nmIdChannel = GetIdFromChar(pcValue);
					// Idchannel
					break;
				case 0x8003:
					pDataDirect->m_nmIdChannelAlias = GetIdFromChar(pcValue);
					// Idchannelalias
					break;
				case 0x8004:
					pDataDirect->m_nmIdChannelKey = GetIdFromChar(pcValue);
					// Idchannelkey
					break;
				case 0x8005:
					pDataDirect->m_nmIdAccessAlias = GetIdFromChar(pcValue);
					// Idaccessalias
					break;
				case 0x8006:
					// Idpresentation
					break;
				case 0x8007:
					// Grants
					break;
				case 0x8008:
					// Startpos
					break;
				default:
					return FALSE;
					break;
			}
		}
	}
	else
	{
		DefProp * pDP = NULL;
		switch (nId)
		{
			case 0x8000:
				pDP = pPBD->GetDefProp("Iditem");
				if (pDP)
				{
					free(pDP->Value.pstrValue);
					if (pcValue)
					{
						pDP->Value.pstrValue = (char*)malloc(strlen(pcValue)+1);
						strcpy(pDP->Value.pstrValue, pcValue);
					}
					else
					{
						pDP->Value.pstrValue = NULL;
					}
				}
				// Iditem
				break;
			case 0x8001:
				pDP = pPBD->GetDefProp("Idnode");
				if (pDP)
				{
					free(pDP->Value.pstrValue);
					if (pcValue)
					{
						pDP->Value.pstrValue = (char*)malloc(strlen(pcValue)+1);
						strcpy(pDP->Value.pstrValue, pcValue);
					}
					else
					{
						pDP->Value.pstrValue = NULL;
					}
				}
				// Idnode
				break;
			case 0x8002:
				// Idchannel
				break;
			case 0x8003:
				// Idchannelalias
				break;
			case 0x8004:
				// Idchannelkey
				break;
			case 0x8005:
				// Idaccessalias
				break;
			case 0x8006:
				// Idpresentation
				break;
			case 0x8007:
				// Grants
				break;
			case 0x8008:
				// Startpos
				break;
			default:
				return FALSE;
				break;
		}
	}
	return TRUE;
}

bool CPBDDecode::InsertFontPropInFont(ICPropBagDef* pParent, long nId, char* pcValue)
{
	USES_CONVERSION;

	// Obtenemos la propiedad
	CCPropBagDef * pPBD = static_cast<CCPropBagDef*> (pParent);
	DefProp * pDefProp = pPBD->GetDefProp("Font");
	IM4FontHolder * pM4FontDisp = NULL;
		
	if(	pDefProp != NULL
		&&	pDefProp->Type == FE_FONT
		&&	pDefProp->Value.pdispValue != NULL
		&&  pcValue != NULL)
	{
		//Ahora todas son cadenas.
		pM4FontDisp = (IM4FontHolder*) pDefProp->Value.pdispValue;

		if (nId == 0x800) {
			pM4FontDisp->put_Name(M4CComBSTR(pcValue));
			return TRUE;
		}
		if (nId == 0x801) {
			pM4FontDisp->put_FontSize(M4CComBSTR(pcValue));
			return TRUE;
		}
		if (nId == 0x802) {
			pM4FontDisp->put_FontBold(M4CComBSTR(pcValue));
			return TRUE;
		}
		if (nId == 0x803) {
			pM4FontDisp->put_FontItalic(M4CComBSTR(pcValue));
			return TRUE;
		}
		if (nId == 0x804) {
			pM4FontDisp->put_FontUnderline(M4CComBSTR(pcValue));
			return TRUE;
		}
	}

	return FALSE; // No hay propiedad Font en este objeto
}

bool CPBDDecode::InsertFontPropInFont(ICPropBagDef* pParent, long nId, VARIANT_BOOL bData)
{
	USES_CONVERSION;
	
	// Obtenemos la propiedad
	CCPropBagDef * pPBD = static_cast<CCPropBagDef*> (pParent);
	DefProp * pDefProp = pPBD->GetDefProp("Font");
	IM4FontHolder * pM4FontDisp = NULL;
	
	if (pDefProp)
	{
		if (pDefProp->Type == FE_FONT)
		{
			pM4FontDisp = (IM4FontHolder*) pDefProp->Value.pdispValue;
		}
		
		switch (nId)
		{
			case 0x802:
				if (bData == -1) {
					pM4FontDisp->put_FontBold(L"True");
				}
				else {
					pM4FontDisp->put_FontBold(L"False");
				}					
				break;
			case 0x803:
				if (bData == -1) {
					pM4FontDisp->put_FontItalic(L"True");
				}
				else {
					pM4FontDisp->put_FontItalic(L"False");
				}					

				break;
			case 0x804:
				if (bData == -1) {
					pM4FontDisp->put_FontUnderline(L"True");
				}
				else {
					pM4FontDisp->put_FontUnderline(L"False");
				}					
				break;
			default:
				return FALSE;
				break;
 		}
		return TRUE;
	}
	return FALSE;
}

bool CPBDDecode::InsertFontPropInFont(ICPropBagDef* pParent, long nId, long nData)
{
	USES_CONVERSION;

	// Obtenemos la propiedad
	CCPropBagDef * pPBD = static_cast<CCPropBagDef*> (pParent);
	DefProp * pDefProp = pPBD->GetDefProp("Font");
	IM4FontHolder * pM4FontDisp = NULL;
	
	if (pDefProp)
	{
		if (pDefProp->Type == FE_FONT)
		{
			pM4FontDisp = (IM4FontHolder*) pDefProp->Value.pdispValue;
		}
		switch (nId)
		{
			case 0x801:
				{
					//Bug 0121974
					char pcValue[128];
					sprintf(pcValue, "%ld", nData);					
					pM4FontDisp->put_FontSize(M4CComBSTR(pcValue));
					break;
				}
			default:
				return FALSE;
				break;
		}
		return TRUE;
	}
	return FALSE;
}

bool CPBDDecode::InsertPropInDataProps(ICPropBagDef* pParent, long nId, long nData)
{
	USES_CONVERSION;
	
	// Obtenemos la propiedad
	CCPropBagDef * pPBD = static_cast<CCPropBagDef*> (pParent);
	DefProp * pDefProp = pPBD->GetDefProp("DataProps");
	IDataProperties * pDataProp = NULL;
	
	if (pDefProp->Type == FE_DATAPROP)
	{
		pDataProp = (IDataProperties*) pDefProp->Value.pdispValue;
	}
	
	if (pDataProp)
	{
		switch (nId)
		{
			case 0x8000:
				//pDataProp->put_IdItem(M4CComBSTR(pcValue));
				// Iditem
				break;
			case 0x8001:
				//pDataProp->put_IdNode(M4CComBSTR(pcValue));
				// Idnode
				break;
			case 0x8002:
				//pDataProp->put_IdChannel(M4CComBSTR(pcValue));
				// Idchannel
				break;
			case 0x8003:
				//pDataProp->put_IdChannelAlias(M4CComBSTR(pcValue));
				// Idchannelalias
				break;
			case 0x8004:
				//pDataProp->put_IdChannelKey(M4CComBSTR(pcValue));
				// Idchannelkey
				break;
			case 0x8005:
				//pDataProp->put_IdAccessAlias(M4CComBSTR(pcValue));
				// Idaccessalias
				break;
			case 0x8006:
				pDataProp->put_IdPresentation(nData);
				// Idpresentation
				break;
			case 0x8007:
				pDataProp->put_Grants((short)nData);
				// Grants
				break;
			case 0x8008:
				pDataProp->put_StartPos(nData);
				// Startpos
				break;
			default:
				return FALSE;
				break;
		}
	}
	return TRUE;
}

ICPropCol * CPBDDecode::GetPropStat()
{
#ifdef PROP_STAT
	return m_pPropCol;
#endif
	return NULL;

}

ICPropBagDef * CPBDDecode::InitFromArray(SAFEARRAY *pArray)
{
	// Nos llega un array, donde el primer elemento son los datos principales y
	// desde el 1-n son los de multilenguaje.
	HRESULT hr = S_OK ;
	BOOL bNeedToRelease = false;
	
	if (pArray)
	{
		BYTE * tmpMain = NULL;		

		VARIANT * pVa = (VARIANT*)pArray->pvData;
		long iDimensions = pArray->rgsabound[0].cElements;

		if (iDimensions > 1)
		{
			m_nLangNumber = iDimensions - 1;
			m_ppTransPB = (ICPropBag **)malloc(m_nLangNumber * sizeof(ICPropBag*));
			memset(m_ppTransPB, 0, sizeof(ICPropBag*)*m_nLangNumber);

			for (long i = 1; i <= m_nLangNumber; i++)
			{
				if (pVa[i].vt == (VT_ARRAY | VT_UI1))  // Típico cuando llega de un lenguaje decente
				{
					SAFEARRAY * psa = pVa[i].parray;
					m_ppTransPB[i-1] = ConstructPBFromASN1((BYTE*)psa->pvData, NULL, NULL);
				}
				else if (pVa[i].vt == (VT_BSTR)) // Típico de algún Scripting
				{
					long nDataLength = ::SysStringLen(pVa[i].bstrVal);

					if (nDataLength != 0)
					{
						// UNICODE VB
						char* tmpBuffer = M4VBToCppBinary( pVa[i].bstrVal, nDataLength ) ;

						m_ppTransPB[i-1] = ConstructPBFromASN1( (unsigned char*) tmpBuffer, NULL, NULL);

						delete[] tmpBuffer;
					}
				}
			}
		}

		// Aunque no haya idiomas si puede haber main
		if( iDimensions > 0 )
		{
			if (pVa[0].vt == (VT_ARRAY | VT_UI1) && pVa[0].parray->pvData != NULL)
			{
				tmpMain = (BYTE*)pVa[0].parray->pvData;
			}
			else if (pVa[0].vt == (VT_BSTR))
			{
				long nDataLength = ::SysStringLen(pVa[0].bstrVal);

				// UNICODE VB
				tmpMain = (BYTE*) M4VBToCppBinary( pVa[0].bstrVal, nDataLength ) ;

				bNeedToRelease = true;
			}
		}

		// A por el Bloque principal
		if (tmpMain && (tmpMain[0] == 48))
		{
			BYTE * pData = tmpMain;
			// Tenemos las condiciones.  m_pAnle, m_ppTransPB, tmpBuffer
			BYTE bType;
			long nLength = ReadType(&tmpMain, &bType);
			if (bType == ASNSequence)
			{
				// Vamos a por la primera PropBagDefTipada
				ReadLength(&tmpMain, &nLength);
				ICPropBagDef * pPBD = NULL;
				long nLength2 = GetPBDefFromASN(&tmpMain, &pPBD);
				if (pPBD)
				{
					nLength2 += ReadASNRec(&tmpMain, nLength - nLength2, pPBD);
					if (nLength2 == nLength)
					{
						if (bNeedToRelease)
						{
							free(pData);
						}

						/*
						Si se llama sin idiomas hay que intentar resolver las traducciones
						No comprobamos error
						*/
						if( iDimensions == 1 )
						{
							hr = pPBD->SolveTranslation() ;
						}

						return pPBD;
					}
					RELEASE(pPBD);
				}
			}
			if (bNeedToRelease)
			{
				free(pData);
			}
		}
	}
	
	return NULL;
}
