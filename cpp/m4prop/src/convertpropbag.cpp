// ConvertPropBag.cpp : Implementation of CConvertPropBag
#include "stdafx.h"
#include "stdio.h"
#include "m4stl.hpp"
#include "traducttools.h"
#include "m4prop.h"
#include "cpropcoldef.h"
#include "cchildcoldef.h"
#include "dataproperties.h"
#include "asnpersistpbd.h"
#include "convertpropbag.h"
#include "cpropbag.h"
#include "cpropbagdef.h"
#include "pbddecode.h"
#include "xmlpersist.h"
#include "m4unicode.hpp"
#include "traductprop.h"
#include "m4unicode.hpp"
#include "replaceliterals.hpp"
extern int	M4IsUnicode( void ) ;


// Defines para ocultar la posición
#define	M4PROP_HIDE_LEFTH				0x01
#define	M4PROP_HIDE_TOP					0x02
#define	M4PROP_HIDE_WIDTH				0x04
#define	M4PROP_HIDE_HEIGHT				0x08


// Retorno de carro
#define		M4PROP_CR					"\r\n"
#define		M4PROP_CR_SIZE				2

//-------------------------------------------------------------------------------------------------
//Atributos invisibles cuando se edita el OBL.
#define		M4PROP_HIDDEN_ATTRIBUTE_INHERIT_STATUS									"Inheritstatus"
//-------------------------------------------------------------------------------------------------


ICPropBag * GetNewPropBag()
{
	CComObject<CCPropBag>  * pNew;
	ICPropBag * pb = NULL;

	HRESULT hr = CComObject<CCPropBag>::CreateInstance(&pNew);
	if (SUCCEEDED(hr))
	{
		pNew->QueryInterface(IID_ICPropBag, (void**)&pb);
		return pb;
	}
	return NULL;
}

ICPropBagDef * GetNewPropBagDef()
{
	CComObject<CCPropBagDef>  * pNew;
	ICPropBagDef * pbDef = NULL;
	
	HRESULT hr = CComObject<CCPropBagDef>::CreateInstance(&pNew);
	if (SUCCEEDED(hr))
	{
		pNew->QueryInterface(IID_ICPropBagDef, (void**)&pbDef);
		return pbDef;
	}
	return NULL;
}



unsigned char readByte(unsigned char **strASN)
{
	unsigned char m_byte;

	m_byte = *(*strASN);
	(*strASN)++;
	return m_byte;
}


bool readBool(unsigned char **strASN)
{
	unsigned char m_byte;

	m_byte = *(*strASN);
	(*strASN)++;
	if (m_byte == 0)
		return false;
	else
		return true;
}

char * readString(unsigned char **strASN, long &lengthString, m4uchar_t type)
{
	// se hace la reserva del espacio de la cadena
	char *pcString = new char[lengthString+1];

	memcpy(pcString, (*strASN),lengthString);
	(*strASN) += lengthString;
	pcString[lengthString] = '\0';

	m4uchar_t  iStringBaseType = ASNGeneralString;

	if( M4IsUnicode() == 1 )
	{
		iStringBaseType = ASNUnicodeString;
	}

	if( type != iStringBaseType && M4IsAscii( pcString, lengthString ) == 0 )
	{
		m4pchar_t pcValue = pcString;
		int iLength = lengthString;

		if( type == ASNUnicodeString )
		{
			// Si es Utf8 se pasa a ANSI
			pcString = M4Utf8ToCpp( pcValue, iLength );
		}
		else
		{
			// Si es ANSI se pasa a Utf8
			pcString = M4ANSIToCpp( pcValue, iLength );
		}
		delete pcValue;
		lengthString = iLength;
	}

	return pcString;
}

long readLong(unsigned char **strASN, long lengthLong)
{
	long mask = 0xFFFFFFFF;
	bool negative = false;
	unsigned char b_Value;
	long m_long = 0, i;
	
				
	for (i=lengthLong;i>0;i--)
	{
		mask <<= 8;
		b_Value = *(*strASN);
		(*strASN)++;
		if ((i == lengthLong) && (b_Value & 0x80))
			negative = true;				
		m_long+=((long)b_Value<<8*(i-1));
	}

	if (negative)
		m_long |= mask;
	return m_long;
}


long readLength(unsigned char **strASN, long *lValue)
{
	unsigned char b_LN,byte;
	long m_length = 0;
	int i;
	
	*lValue = 0;
	b_LN = *(*strASN);
	(*strASN)++;
	if(b_LN>128)
	{
		b_LN -= 128;
		long ret = b_LN +1;
		for (i=b_LN;i>0;i--)
		{
			byte = *(*strASN);
			(*strASN)++;
			*lValue+=(byte<<8*(i-1));
		}
		return ret;

	}
	*lValue = b_LN;	
	return 1;

}


void writeLength(unsigned char **strASN,long m_length)
{
	long aux;
	unsigned char m_bytes[5];
	int n_total;
	int i = 0,j = 0;

	if (m_length >127)
	{
		aux = m_length;
		while (aux != 0)
		{
			m_bytes[i++] = aux & (0xff);
			aux >>= 8;
		}

		n_total = 128 +i;

		/// i representa los bytes que ocupa el numero
		*(*strASN) = n_total;
		(*strASN) ++;
		for(j=i-1;j>=0;j--)
		{
			*(*strASN) = m_bytes[j];
			(*strASN)++;
		}
	}
	else
	{
		n_total = (unsigned char)m_length;
		*(*strASN) = n_total;
		(*strASN)++;
	}
}

long Longlength(long nLength)
{

	if (nLength >127)
	{
		unsigned long aux = nLength;
		long i = 1;
		while (aux != 0)
		{
			i++;
			aux >>= 8;
		}
		return i;
		
	}
	return 1;
}



long code2PropChild(ICPropBag * pb, unsigned char **strASN, long nMylong, const char* ai_pccOrganization, ClChannelManager *ai_poChannelManager)
{
	unsigned char b_T,b_TN;
	long m_lengthString=0,m_lengthPropChild = 0, m_lengthLong= 0, m_lengthPropBag = 0;
	char * m_string = NULL;
	char * m_stringValue = NULL;
	bool m_bool;
	long m_long;
	long lRet;
	BSTR PropName = NULL;
	VARIANT vTmp;
	
	::VariantInit(&vTmp);

	do
	{

		b_TN = readByte(strASN);
		m_lengthPropChild++;
		
		if(b_TN == ASNGeneralString || b_TN == ASNUnicodeString )
		{
			// Leemos el nombre de la propiedad
			m_lengthPropChild += readLength(strASN, &m_lengthString);	
			m_lengthPropChild += m_lengthString;
			m_string = readString(strASN, m_lengthString, b_TN);

			b_T = readByte(strASN);
			m_lengthPropChild++;
	
			PropName = M4CppToVB(m_string);
			
			// Leemos el valor de la propiedad
			switch (b_T)
			{
				case ASNGeneralString:
				case ASNUnicodeString:
				{
					// Leemos su longitud
					m_lengthPropChild += readLength(strASN, &m_lengthString);
					m_lengthPropChild += m_lengthString;
					// Leemos contenido

					m_stringValue = readString(strASN, m_lengthString, b_T);

					/* Bug 0166236
					Lo traducible son propiedades de tipo Text, Tooltip y Collabel
					*/
					if( ai_poChannelManager != NULL &&
						( strcmpi( m_string, "Text" ) == 0 ||
						  strcmpi( m_string, "Tooltip" ) == 0 ||
						  strcmpi( m_string, "Collabel" ) == 0 )
						)
					{
						// Si es texto y se puede se remplazan literales
						m4return_t	iResult = M4_ERROR ;
						m4bool_t	bModified = M4_FALSE ;
						m4pchar_t	pcOut = NULL ;

						iResult = M4ReplaceLiterals( ai_poChannelManager, m_stringValue, pcOut, ai_pccOrganization, bModified ) ;

						if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcOut != NULL )
						{
							delete m_stringValue ;
							m_stringValue = pcOut ;
						}
					}

					// Lo asignamos al Variant
					vTmp.vt = VT_BSTR;
					vTmp.bstrVal = M4CppToVB(m_stringValue);
					
					// Y "pa dentro"
					pb->AddProperty(PropName, vTmp);
					VariantClear(&vTmp);
					delete m_stringValue;
					m_stringValue = NULL;
					break;
				}
			
				case ASNBOOLEAN:
				{	
					readByte(strASN);
					m_bool = readBool(strASN);
					m_lengthPropChild += 2;
					vTmp.vt = VT_BOOL;
					if (m_bool == 0)
					{
						vTmp.boolVal = m_bool;
					}
					else
					{
						vTmp.boolVal = -1;
					}
					pb->AddProperty(PropName, vTmp);
					VariantClear(&vTmp);
					break;
				}
				case ASNINTEGER:
				{
					m_lengthPropChild += readLength(strASN, &m_lengthLong);	
					m_lengthPropChild += m_lengthLong;
					m_long = readLong(strASN, m_lengthLong);	
					
					if (m_lengthLong > 2)
					{
						vTmp.vt = VT_I4;
						vTmp.lVal = m_long;
					}
					else
					{
						vTmp.vt = VT_I2;
						vTmp.iVal = (short)m_long;
					}
					pb->AddProperty(PropName, vTmp);
					VariantClear(&vTmp);
					break;
				}
				
			}
 			delete m_string;
			::SysFreeString(PropName);
			PropName = NULL;
			// Hasta aquí hemos leído una propiedad

		}
		else if (b_TN == ASNSequence)
		{
			// Generamos un nuevo objeto
			ICPropBag * pbChild = GetNewPropBag();
			if (pbChild != NULL)
			{
				
				m_lengthPropChild += readLength(strASN, &m_lengthPropBag);
				// Si la PropBag está completamente vacía se nos jode la historia
				if (m_lengthPropChild < nMylong)
				{
					lRet = code2PropChild(pbChild, strASN, m_lengthPropBag, ai_pccOrganization, ai_poChannelManager);
					if (lRet >= 0)
					{
						m_lengthPropChild += lRet;
					}
					else
					{
						RELEASE(pb);
						return -1;
					}
					pb->AddChild(&pbChild);
					RELEASE(pbChild);

				}
				else
				{
					// Nos Vamos que ya es hora
					RELEASE(pbChild);
					return m_lengthPropChild;
				}
			}
			else
			{
				pbChild = NULL;

			}
		}
		else
		{
			return -1;
			pb = NULL;
		}

	} while (m_lengthPropChild < nMylong);
	return m_lengthPropChild;
}

long code2PropChildDef(ICPropBagDef * pbDef, unsigned char **strASN, long nMylong)
{
	unsigned char b_T,b_TN;
	long m_lengthString=0,m_lengthPropChild = 0, m_lengthLong= 0, m_lengthPropBag = 0;
	char * m_string;
	char * m_stringValue;
	bool m_bool;
	long m_long;
	long lRet;
	CComVariant vTmp;
	
	do
	{

		b_TN = readByte(strASN);
		m_lengthPropChild++;
		
		if(b_TN == ASNGeneralString || b_TN == ASNUnicodeString)
		{
			// Leemos el nombre de la propiedad
			m_lengthPropChild += readLength(strASN, &m_lengthString);	
			m_lengthPropChild += m_lengthString;
			m_string = readString(strASN, m_lengthString, b_TN);

			b_T = readByte(strASN);
			m_lengthPropChild++;
	
			BSTR bstrPropName = M4CppToVB(m_string);
			// Leemos el valor de la propiedad
			switch (b_T)
			{
				case ASNGeneralString:
				case ASNUnicodeString:
				{
					// Leemos su longitud
					m_lengthPropChild += readLength(strASN, &m_lengthString);	
					m_lengthPropChild += m_lengthString;
					// Leemos contenido
					m_stringValue = readString(strASN, m_lengthString, b_T);

					// Lo asignamos al Variant
					vTmp = M4CppToVB(m_stringValue);
					
					// Y "pa dentro"
					pbDef->AddProperty(bstrPropName, vTmp, vTmp,FE_STRING);

					delete m_stringValue;
					break;
				}
			
				case ASNBOOLEAN:
				{	
					readByte(strASN);
					m_bool = readBool(strASN);
					m_lengthPropChild += 2;
					if (m_bool == 0)
					{
						vTmp = (VARIANT_BOOL) m_bool;
					}
					else
					{
						vTmp = (VARIANT_BOOL)  -1;
					}
					pbDef->AddProperty(bstrPropName, vTmp, vTmp, FE_BOOLEAN);
					break;
				}
				case ASNINTEGER:
				{
					m_lengthPropChild += readLength(strASN, &m_lengthLong);	
					m_lengthPropChild += m_lengthLong;
					m_long = readLong(strASN, m_lengthLong);	
					
					if (m_lengthLong > 2)
					{
						vTmp = m_long;
					}
					else
					{
						vTmp = (short) m_long;
					}
					pbDef->AddProperty(bstrPropName, vTmp, vTmp, FE_NUMBER);
					break;
				}
				
			}
			delete m_string;
			// Hasta aquí hemos leído una propiedad
			RELEASE_BSTR(bstrPropName);
		}
		else if (b_TN == ASNSequence)
		{
			// Generamos un nuevo objeto
			ICPropBagDef * pbChild = GetNewPropBagDef();
			if (pbChild != NULL)
			{
				m_lengthPropChild += readLength(strASN, &m_lengthPropBag);
				// Si la PropBag está completamente vacía se nos jode la historia
				if (m_lengthPropChild < nMylong)
				{
					lRet = code2PropChildDef(pbChild, strASN, m_lengthPropBag);
					if (lRet >= 0)
					{
						m_lengthPropChild += lRet;
					}
					else
					{
						RELEASE(pbDef);
						return -1;
					}
					pbDef->AddChild(&pbChild, CComVariant());
					RELEASE(pbChild);
				}
				else
				{
					// Nos Vamos que ya es hora
					RELEASE(pbChild);
					return m_lengthPropChild;
				}
			}
			else
				pbChild = NULL;
		}
		else
		{
			return -1;
			pbDef = NULL;
		}

	} while (m_lengthPropChild < nMylong);

	return m_lengthPropChild;
				
}



ICPropBag* ConstructPBFromASN1(unsigned char * strASN, const char* ai_pccOrganization, ClChannelManager *ai_poChannelManager)
{
	
	unsigned char b_T;
	long m_lengthPropBag,m_lengthPropChild = 0,i;
	bool acabar = false;
	ICPropBag * pb;
	
	if (strASN && (strASN[0] == 48))
	{
		b_T = readByte(&strASN);
		if (b_T==ASNSequence)
		{
			CComObject<CCPropBag> * pPB;
			HRESULT hr = CComObject<CCPropBag>::CreateInstance(&pPB);
			if (hr == S_OK)
			{
				hr = pPB->QueryInterface(IID_ICPropBag, (void**)&pb);
			}
			if (hr == S_OK)
			{
				readLength(&strASN, &m_lengthPropBag);
				i = 0; 
				while ((i<m_lengthPropBag)&&(!acabar))
				{
					m_lengthPropChild = code2PropChild(pb, &strASN, m_lengthPropBag, ai_pccOrganization, ai_poChannelManager);
					if(m_lengthPropChild>=0)
					{
						i += m_lengthPropChild;
					}
					else
					{
						// Llegamos con código de error, Release y nos vamos
						acabar = true;
						pb = NULL;
					}
				}
				return pb;
			}
		} 
	}
	
	return NULL;

}





/////////////////////////////////////////////////////////////////////////////
// CConvertPropBag

STDMETHODIMP CConvertPropBag::PB2FileOBL(ICPropBag * pPB,BSTR nomFile)
{
	USES_CONVERSION;

	if (pPB == NULL) return E_FAIL;

	char *myNomb, *tmpOBL;
	BSTR strOBL =  ::SysAllocString(L"");

	myNomb = M4VBToCpp(nomFile);
	eUniFileEncodingType_t eEncoding = M4UniNative;
	FILE *pfFile = M4Fopen( myNomb, M4UniWriteBinary, eEncoding ) ;
	delete myNomb;

	if( pfFile == NULL )
	{
		return S_FALSE;
	}
	
	PB2StringOBL(pPB, &strOBL);

	tmpOBL = M4VBToCpp( strOBL );
	SysFreeString(strOBL);

	fwrite( tmpOBL, strlen( tmpOBL ), sizeof( unsigned char ), pfFile );
	fclose( pfFile );
	delete tmpOBL;

	return S_OK;
}

STDMETHODIMP CConvertPropBag::PBDef2FileOBL(ICPropBagDef * pPBDef,BSTR nomFile)
{
	USES_CONVERSION;

	HRESULT hr = E_INVALIDARG;
	if (pPBDef && nomFile)
	{
		char *myNomb = NULL, *tmpOBL = NULL;

		myNomb = M4VBToCpp(nomFile);
		eUniFileEncodingType_t eEncoding = M4UniNative;
		FILE *pfFile = M4Fopen( myNomb, M4UniWriteBinary, eEncoding ) ;
		delete myNomb;

		if( pfFile == NULL )
		{
			return S_FALSE;
		}
		
		contMem = 0;
		maxMem = MAX_MEM;
		initPoint = NULL;

		tmpOBL = (char *)malloc(MAX_MEM);
		initPoint = tmpOBL;
		WritePBDefInStringOBL(pPBDef, &tmpOBL, NULL, 0, false, NULL);
		*tmpOBL ='\0';

		fwrite( tmpOBL, strlen( tmpOBL ), sizeof( unsigned char ), pfFile );
		fclose( pfFile );
		free(initPoint);
		hr = S_OK;
	}
	return hr;

}

void CConvertPropBag::WriteTabInString(int num, char **myOBL)
{
	CheckLengthMemory(myOBL,num);
	memset((*myOBL), 32, num);
	(*myOBL)+=num;
}

void CConvertPropBag::CheckLengthMemory(char **strOBL, long incr)
{

	if (contMem + incr >= maxMem)
	{
		NewBlockOfMemory();
		(*strOBL) = initPoint+contMem;
	}

	contMem = contMem + incr;
}

void CConvertPropBag::NewBlockOfMemory()
{
	char *newOBL = (char *)realloc(initPoint, 2 * maxMem);
	maxMem = 2 * maxMem;
	initPoint = newOBL;

}

STDMETHODIMP CConvertPropBag::get_NumEspTab(int * num)
{
	*num = m_nNumTab;
	return S_OK;
}

STDMETHODIMP CConvertPropBag::put_NumEspTab(int num)
{
	m_nNumTab = num;
	return S_OK;
}


void CConvertPropBag::WriteLevelProperty(char *pstrPropName, char*pstrPropValue, char **ppstrOBL, long nLevel)
{

	for (int i=1;i<=nLevel;i++)
	{
		WriteTabInString(m_nNumTab, ppstrOBL);
	}

	long nLength = strlen(pstrPropName);
	CheckLengthMemory(ppstrOBL, nLength);
	memcpy(*ppstrOBL, pstrPropName, nLength);
	*ppstrOBL += nLength;

	CheckLengthMemory(ppstrOBL, 3);
	strcpy(*ppstrOBL, " = ");
	*ppstrOBL += 3;
	
	nLength = strlen(pstrPropValue);
	CheckLengthMemory(ppstrOBL, nLength);
	memcpy(*ppstrOBL, pstrPropValue, nLength);
	*ppstrOBL += nLength;
	
	CheckLengthMemory(ppstrOBL,M4PROP_CR_SIZE);
	memcpy(*ppstrOBL, M4PROP_CR, M4PROP_CR_SIZE);
	*ppstrOBL += M4PROP_CR_SIZE;

}


void CConvertPropBag::WriteLevelInString(DefProp * pDefProp,char ** myOBL,long level)
{

	// A por la propiedad
	const char * pName = GetNameFromPropId(pDefProp->nIdProperty);
	
	for (int i=1;i<=level;i++)
		WriteTabInString(m_nNumTab, myOBL);

	long nLength = strlen(pName);
	CheckLengthMemory(myOBL,nLength);
	memcpy((*myOBL),pName,nLength);
	(*myOBL) += nLength;
	CheckLengthMemory(myOBL,3);
	memcpy((*myOBL), " = ", 3);
	(*myOBL) += 3;

	// Ya tenemos el nombre de la propiedad, vamos a por su valor
	
	char * tmpStr = (char*)malloc(20); // No creo que tengamos numéricos tan grandes

	switch (pDefProp->Type)
	{
		// Tipos Numéricos
		case FE_NUMBER:
		case FE_COLOR:
		case FE_CONSTRUCTIF:
		case FE_GRANTSTYPE:
		case FE_NUMBER_NOTDEFINED:
			_itoa(pDefProp->Value.lValue,tmpStr,10);
			nLength = strlen(tmpStr);
			CheckLengthMemory(myOBL,nLength);
			memcpy((*myOBL),tmpStr,nLength);
			(*myOBL) += nLength;
			break;
		// Tipos String
		case FE_STRING:
		case FE_DATE:
		case FE_IMAGE:
		case FE_ALIGNTYPE:
		case FE_LN4_CODEARGS:
		case FE_STRING_NOTDEFINED:
			if (pDefProp->Value.pstrValue)
			{
				/* Bug 0095995
				Hay que copiar la longitud de la cadena sin comillas
				*/
				nLength = strlen(pDefProp->Value.pstrValue);
				CheckLengthMemory(myOBL, nLength+2);
				*(*myOBL) = 34;  // Marchando una de Comillas
				(*myOBL) += 1;
				memcpy((*myOBL), pDefProp->Value.pstrValue,nLength);
				(*myOBL) += nLength;
				*(*myOBL) = 34;  // Marchando una de Comillas
				*myOBL +=1;
			}
			else
			{
				CheckLengthMemory(myOBL, 2);
				*(*myOBL) = 34;  // Marchando una de Comillas
				(*myOBL) += 1;
				*(*myOBL) = 34;  // Marchando una de Comillas
				*myOBL +=1;
			}
			break;
		// Tipos ya resueltos
		case FE_TRADUCTPROP:
		case FE_FONT:
		case FE_DATAPROP:
			break;
		case FE_BOOLEAN:
		case FE_BOOLEAN_NOTDEFINED:
			if (pDefProp->Value.boolValue)
			{
				// "True"
				CheckLengthMemory(myOBL, 4); 
				memcpy((*myOBL), "True",4);
				(*myOBL) += 4;
			}
			else
			{
				// "False"
				CheckLengthMemory(myOBL, 5);
				memcpy((*myOBL), "False",5);
				(*myOBL) += 5;
			}
			break;
		default:
			break;
	}
	free(tmpStr);

	CheckLengthMemory(myOBL,M4PROP_CR_SIZE);
	memcpy(*myOBL, M4PROP_CR, M4PROP_CR_SIZE);
	*myOBL += M4PROP_CR_SIZE;
}


void CConvertPropBag::WritePBInStringOBL(ICPropBag * pPB,char **myOBL,long level)
{
	
	USES_CONVERSION;


	BSTR BSTRclassname = ::SysAllocString(L"");
	BSTR BSTRalias = ::SysAllocString(L"");
	char *begin="BEGIN  ";
	char *espacio="";
	long lenClassname = 0, lenAlias = 0,i;
	char *tab = "\t", *myProp;
	VARIANT val, numProp, numChild;
	long nProp = 0, nPropTotal = 0,nChild = 0, nChildTotal = 0;
	ICPropCol *pCol;
	ICChildCol *pChild;
	ICPropBag *pPBChild;
	ICSingleProp *pSP;
	HRESULT hr1, hr2, hr3;
	
	
	VariantInit(&val);
	VariantInit(&numProp);
	numProp.vt = VT_I4;
	VariantInit(&numChild);
	numChild.vt = VT_I4;

	pPB->get_ClassName(&BSTRclassname);
	pPB->get_Alias(&BSTRalias);
	char *classname = M4VBToCpp(BSTRclassname);
	char *alias = M4VBToCpp(BSTRalias);
	SysFreeString(BSTRclassname);
	SysFreeString(BSTRalias);
	lenClassname=strlen(classname);
	lenAlias=strlen(alias);
	
	for (i = 1;i <= level;i++)
	{
		WriteTabInString(m_nNumTab, myOBL);
	}
	
	CheckLengthMemory(myOBL,6);
	memcpy((*myOBL), "BEGIN ", 6);
	(*myOBL) += 6;
	CheckLengthMemory(myOBL,lenClassname);
	memcpy((*myOBL), classname, lenClassname);
	delete classname;
	(*myOBL) += lenClassname;
	CheckLengthMemory(myOBL,1);
	memcpy((*myOBL)," ", 1);
	(*myOBL) += 1;
	CheckLengthMemory(myOBL,lenAlias);
	memcpy((*myOBL),alias, lenAlias);
	delete alias;
	(*myOBL) += lenAlias;

	CheckLengthMemory(myOBL,M4PROP_CR_SIZE);
	memcpy(*myOBL, M4PROP_CR, M4PROP_CR_SIZE);
	*myOBL += M4PROP_CR_SIZE;

	
	hr1 = pPB->get_PropCol(&pCol);
	if (hr1 == S_OK)
	{
		pCol->get_Count(&nPropTotal);
		for (nProp = 1;nProp <= nPropTotal;nProp++)
		{
			numProp.lVal = nProp;
			hr2 = pCol->get_Item(numProp,&pSP);
			if (hr2 == S_OK)
			{
				BSTR bstrName = ::SysAllocString(L"");
				hr2 = pSP->get_Name(&bstrName);
				if (hr2 == S_OK)
				{
					myProp = M4VBToCpp(bstrName);
					::SysFreeString(bstrName);
					VariantClear(&val);
					hr2 = pSP->get_Value(&val);
					if (hr2 == S_OK)
					{
						WriteLevelInString(myProp, val, myOBL, level+1);
					}
					delete myProp;
				}
				RELEASE(pSP);
			}
		}
		RELEASE(pCol);
	}

	VariantClear(&val);
	VariantClear(&numProp);
	
	hr3 = pPB->get_ChildCol(&pChild);
	pChild->get_Count(&nChildTotal);
	for (nChild = 1;nChild <= nChildTotal;nChild++)
	{
		numChild.lVal = nChild;
		HRESULT hr = pChild->get_Item(numChild,&pPBChild);
		WritePBInStringOBL(pPBChild,myOBL, level+1);
		if (SUCCEEDED(hr))
			RELEASE(pPBChild);
	}
	if (SUCCEEDED(hr3))
		RELEASE(pChild);

	VariantClear(&numChild);
	WriteLevelInString("END", val, myOBL, level);
	
}

void CConvertPropBag::WritePBDefInStringOBL(ICPropBagDef * pPBDef,char **myOBL, CDataProperties *pData, long level, bool ai_bHidePosition, const char* ai_pccParent)
{
	USES_CONVERSION;

	CCPropBagDef * pOrigen = static_cast<CCPropBagDef*> (pPBDef);
	CCPropColDef * pPropColDef = pOrigen->m_PropCol;
	CDataProperties * pInnerData = NULL;
	int i = 0;

	// Metemos los "Tabs" apropiados
	for (i = 1;i <= level;i++)
	{
		WriteTabInString(m_nNumTab, myOBL);
	}
	
	// Primer Cálculo de espacio
	long nClassNameLength = strlen(pOrigen->m_strClassName);
	long nAliasLength = strlen(pOrigen->m_strAlias);
	CheckLengthMemory(myOBL, 7 + nClassNameLength + nAliasLength + M4PROP_CR_SIZE);

	// Escribimos la cabecera
	memcpy((*myOBL), "BEGIN ", 6);
	(*myOBL) += 6;
	memcpy((*myOBL), pOrigen->m_strClassName, nClassNameLength);
	(*myOBL) += nClassNameLength;
	memcpy((*myOBL)," ", 1);
	(*myOBL) += 1;
	memcpy((*myOBL),pOrigen->m_strAlias, nAliasLength);
	(*myOBL) += nAliasLength;

	memcpy(*myOBL, M4PROP_CR, M4PROP_CR_SIZE);
	*myOBL += M4PROP_CR_SIZE;

	DefProp * pDefProp = NULL;

	/* Bug 0142830
	Para el caso de que no se quiera mostrar la posición según el alineamiento
	se busca si existe la propiedad alineamiento y se calcula lo que se tienen que ocultar
	*/
	int iHideMode = 0 ;
	bool bHideObject = false ;
	const char* pccName = NULL ;
	const char* pccValue = NULL ;

	if( ai_bHidePosition == true )
	{
		if( ai_pccParent != NULL )
		{
			// Si el padre es Tab, Changer, Form, Crosstable, Includecall, Splittblock, Panel, Scrollpanel, Mimic, Extern, Table
			// hay que ocultar los campos
			bHideObject = false ;

			if(
					strcmpi( ai_pccParent, "Tab" ) == 0
				||	strcmpi( ai_pccParent, "Changer" ) == 0
				||	strcmpi( ai_pccParent, "Form" ) == 0
				||	strcmpi( ai_pccParent, "Crosstable" ) == 0
				||	strcmpi( ai_pccParent, "Includecall" ) == 0
				||	strcmpi( ai_pccParent, "Splittblock" ) == 0
				||	strcmpi( ai_pccParent, "Panel" ) == 0
				||	strcmpi( ai_pccParent, "Scrollpanel" ) == 0
				||	strcmpi( ai_pccParent, "Mimic" ) == 0
				||	strcmpi( ai_pccParent, "Extern" ) == 0
				||	strcmpi( ai_pccParent, "Table" ) == 0
				)
			{
				bHideObject = true ;

				// Pero si el objeto es Splittvertical o Splitthorizontal no
				pccName = pOrigen->m_strClassName ;

				if( pccName != NULL )
				{
					if(
							strcmpi( pccName, "Splittvertical" ) == 0
						||	strcmpi( pccName, "Splitthorizontal" ) == 0
						)
					{
						bHideObject = false ;
					}
				}
			}
		}
	}

	if( bHideObject == true )
	{
		pDefProp = pPropColDef->m_pNodeHead ;

		while( pDefProp != NULL )
		{
			pccName = GetNameFromPropId( pDefProp->nIdProperty ) ;

			if( strcmpi( pccName, "Align" ) != 0 || pDefProp->Type != FE_ALIGNTYPE || pDefProp->Value.pstrValue == NULL )
			{
				pDefProp = pDefProp->pNextSeq ;
				continue ;
			}

			pccValue = pDefProp->Value.pstrValue ;

			if( strcmpi( pccValue, "Top" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_LEFTH | M4PROP_HIDE_TOP | M4PROP_HIDE_WIDTH ;
			}
			else if( strcmpi( pccValue, "Bottom" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_LEFTH | M4PROP_HIDE_TOP | M4PROP_HIDE_WIDTH ;
			}
			else if( strcmpi( pccValue, "Left" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_LEFTH | M4PROP_HIDE_TOP | M4PROP_HIDE_HEIGHT ;
			}
			else if( strcmpi( pccValue, "Right" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_LEFTH | M4PROP_HIDE_TOP | M4PROP_HIDE_HEIGHT ;
			}
			else if( strcmpi( pccValue, "Bottomfixed" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_LEFTH | M4PROP_HIDE_WIDTH | M4PROP_HIDE_HEIGHT ;
			}
			else if( strcmpi( pccValue, "Rightfixed" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_TOP | M4PROP_HIDE_WIDTH | M4PROP_HIDE_HEIGHT ;
			}
			else if( strcmpi( pccValue, "All" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_LEFTH | M4PROP_HIDE_TOP | M4PROP_HIDE_WIDTH | M4PROP_HIDE_HEIGHT ;
			}
			else if( strcmpi( pccValue, "Absright" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_LEFTH | M4PROP_HIDE_TOP | M4PROP_HIDE_HEIGHT ;
			}
			else if( strcmpi( pccValue, "Toponly" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_TOP ;
			}
			else if( strcmpi( pccValue, "Bottomonly" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_TOP ;
			}
			else if( strcmpi( pccValue, "Leftonly" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_LEFTH ;
			}
			else if( strcmpi( pccValue, "Rightonly" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_LEFTH ;
			}
			else if( strcmpi( pccValue, "Topfixed" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_LEFTH | M4PROP_HIDE_WIDTH ;
			}
			else if( strcmpi( pccValue, "TopScrollBar" ) == 0 )
			{
				iHideMode = M4PROP_HIDE_LEFTH | M4PROP_HIDE_TOP ;
			}

			break ;
		}
	}

	pDefProp = pPropColDef->m_pNodeHead;

	while(pDefProp)
	{
		//----------------------------------------------------
		//Si es una propiedad oculta no hacemos nada.
		if (_IsHiddenProperty( pDefProp ) == true) {
			pDefProp = pDefProp->pNextSeq;
			continue;
		}
		//----------------------------------------------------

		if (pDefProp->Type == FE_DATAPROP)
		{
			pInnerData = static_cast<CDataProperties*> (pDefProp->Value.pdispValue);
			WriteDataProp(pInnerData, pData, myOBL, level);
		}
		else if (pDefProp->Type == FE_FONT)
		{
			WriteFont(pDefProp, myOBL, level);
		}
		else if (pDefProp->Type == FE_TRADUCTPROP)
		{
			WriteTraductProp(static_cast<ITraductProp*> (pDefProp->Value.pdispValue), myOBL, level);
		}
		else
		{
			if (!(DefPropEqual(pDefProp)))
			{
				bool bWrite = true ;

				if( iHideMode != 0 )
				{
					// Si hay algo que no mostrar se ignora
					pccName = GetNameFromPropId( pDefProp->nIdProperty ) ;

					if( ( iHideMode & M4PROP_HIDE_LEFTH ) != 0 && strcmpi( pccName, "Left" ) == 0 )
					{
						bWrite = false ;
					}
					else if( ( iHideMode & M4PROP_HIDE_TOP ) != 0 && strcmpi( pccName, "Top" ) == 0 )
					{
						bWrite = false ;
					}
					else if( ( iHideMode & M4PROP_HIDE_WIDTH ) != 0 && strcmpi( pccName, "Width" ) == 0 )
					{
						bWrite = false ;
					}
					else if( ( iHideMode & M4PROP_HIDE_HEIGHT ) != 0 && strcmpi( pccName, "Height" ) == 0 )
					{
						bWrite = false ;
					}
				}

				if( bWrite == true )
				{
					WriteLevelInString(pDefProp, myOBL,level+1);
				}
			}
		}
		pDefProp = pDefProp->pNextSeq;
	}
	
	// Iteramos a través de los Críos
	ICChildColDef * pChildDef = NULL;
	if (pPBDef->get_ChildColDef(&pChildDef) == S_OK)
	{
		long nChildTotal = 0;
		pChildDef->get_Count(&nChildTotal);
		for (long nChild = 1;nChild <= nChildTotal;nChild++)
		{
			ICPropBagDef * pPBDefChild = NULL;
			if (pChildDef->get_Item(CComVariant(nChild),&pPBDefChild) == S_OK)
			{
				WritePBDefInStringOBL(pPBDefChild,myOBL, pInnerData, level+1, ai_bHidePosition, pOrigen->m_strClassName);
				RELEASE(pPBDefChild);
			}
		}
		RELEASE(pChildDef);
	}

	for (i=1;i<=level;i++)
	{
		WriteTabInString(m_nNumTab, myOBL);
	}
	
	CheckLengthMemory(myOBL,3 + M4PROP_CR_SIZE );
	memcpy((*myOBL), "END", 3);
	(*myOBL) += 3;

	memcpy(*myOBL, M4PROP_CR, M4PROP_CR_SIZE);
	*myOBL += M4PROP_CR_SIZE;
}

bool CConvertPropBag::_IsHiddenProperty( DefProp * ai_pDefProp )
{
	//Comprobamos si es una propiedad oculta que no queremos que se vea en el OBL.
	if ((ai_pDefProp == NULL) || (ai_pDefProp->Key == NULL)) {
		return false;
	}

	//-----------------------------------------------------------
	//Comprobamos si es la propiedad de herencia.
	if (strcmp(ai_pDefProp->Key, M4PROP_HIDDEN_ATTRIBUTE_INHERIT_STATUS) == 0) {
		return true;
	}
	//-----------------------------------------------------------

	return false;
}

STDMETHODIMP CConvertPropBag::PB2StringOBL(ICPropBag * pPB, BSTR *strOBL)
{
	
	USES_CONVERSION;

	char *tmpOBL;

	maxMem = MAX_MEM;
	contMem = 0;
	initPoint = NULL;
	tmpOBL = (char *)malloc(MAX_MEM);
	initPoint = tmpOBL;
	WritePBInStringOBL(pPB,&tmpOBL,0);
	*tmpOBL ='\0';

	SysFreeString(*strOBL);
	*strOBL = M4CppToVB(initPoint);
	delete[] initPoint;
	return S_OK;
}


HRESULT CConvertPropBag::_PBDef2StringOBL(ICPropBagDef * pPBDef, BSTR *strOBL, bool ai_bHidePosition)
{
	
	USES_CONVERSION;

	char *tmpOBL = NULL;

	contMem = 0;
	maxMem = MAX_MEM;
	initPoint = NULL;

	tmpOBL = (char *)malloc(MAX_MEM);
	initPoint = tmpOBL;
	WritePBDefInStringOBL(pPBDef,&tmpOBL,NULL, 0, ai_bHidePosition, NULL);
	*tmpOBL ='\0';

	SysFreeString(*strOBL);
	*strOBL = M4CppToVB(initPoint);
	delete[] initPoint;
	return S_OK;
}


STDMETHODIMP CConvertPropBag::PBDef2StringOBL(ICPropBagDef * pPBDef, BSTR *strOBL)
{
	return( _PBDef2StringOBL( pPBDef, strOBL, false ) ) ;
}

STDMETHODIMP CConvertPropBag::PBDef2StringOBLHide(ICPropBagDef *pPBDef, BSTR *strOBL)
{
	return( _PBDef2StringOBL( pPBDef, strOBL, true ) ) ;
}

STDMETHODIMP CConvertPropBag::Array2PB(SAFEARRAY ** aArray, ICPropBag * * pPB)
{
	// TODO: Add your implementation code here
	//char * thePointer;
	char * tmpBuffer;
	//long lRet;
	
	// Versión Pesimista
	HRESULT hr = E_INVALIDARG; 
	*pPB = NULL;
	hr = SafeArrayAccessData(*aArray, (void**)&tmpBuffer);
	if (SUCCEEDED(hr))
	{
		*pPB = ConstructPBFromASN1((unsigned char*)tmpBuffer, NULL, NULL);

		SafeArrayUnaccessData(*aArray);
		if (pPB != NULL)
		{
			return S_OK;
		}
	}
	return hr;
}

STDMETHODIMP CConvertPropBag::Array2PBDef(SAFEARRAY ** aArray, ICPropBagDef * * pPBDef)
{
	char * tmpBuffer;
	HRESULT hr = E_INVALIDARG;

	if (pPBDef)
	{
		RELEASE((*pPBDef));
		hr = SafeArrayAccessData(*aArray, (void**)&tmpBuffer);
		if (SUCCEEDED(hr))
		{
			*pPBDef = ConstructPBDefFromASN1((unsigned char*)tmpBuffer);
			SafeArrayUnaccessData(*aArray);
			if (pPBDef != NULL)
			{
				return S_OK;
			}
		}
	}
	return hr;
}
ICPropBagDef * CConvertPropBag::ConstructPBDefFromASN1(unsigned char * strASN)
{
	
	unsigned char ucType;
	
	long m_lengthPropBag,m_lengthPropChild = 0,i;
	
	bool acabar = false;
	
	ICPropBagDef * pbDef;
	
	ucType = readByte(&strASN);
	if (ucType == ASNSequence)
	{
		CComObject<CCPropBagDef> * pPBD;
		HRESULT hr = CComObject<CCPropBagDef>::CreateInstance(&pPBD);
		if (hr == S_OK)
		{
			hr = pPBD->QueryInterface(IID_ICPropBagDef, (void**)&pbDef);
		}
		if (hr == S_OK)
		{
			readLength(&strASN, &m_lengthPropBag);
			i = 0; 
			while ((i<m_lengthPropBag)&&(!acabar))
			{
				m_lengthPropChild = code2PropChildDef(pbDef, &strASN, m_lengthPropBag);
				if(m_lengthPropChild>=0)
				{
					i += m_lengthPropChild;
				}
				else
				{
					// Llegamos con código de error, Release y nos vamos
					acabar = true;
					pbDef = NULL;
				}
			}

			return pbDef;
		}

	} 
	
	return NULL;

}



STDMETHODIMP CConvertPropBag::GetPBLength(ICPropBag * pPB, long ** pVal)
{
	long *Value = *pVal;
	pPB->Invalidatelength();
	
	return S_OK;
}

STDMETHODIMP CConvertPropBag::PB2ASN(ICPropBag * pb, BSTR * bstrOut)
{

	// Si la Pb es nothing nos vamos a tomar viento.

	if (pb == NULL)
	{
		::SysFreeString(*bstrOut);
		 * bstrOut = ::SysAllocString(L"");
		return S_OK;
	}

	
	long tmpLength = 0;

	pb->Invalidatelength();
	pb->get_Length(&tmpLength);
	long tmpLongLength = Longlength(tmpLength);

	// tmpLength (lo que ocupa la PB
	// tmpLongLength (lo que ocupa la longitud)
	// 1 del Tag, 1 para un '/0' final
	long outputLength = (tmpLength + tmpLongLength + 2);

	if( M4IsUnicode() == 1 )
	{
		// En unicode se dobla la longitud para no tener problemas con el utf8
		outputLength *= 2;
	}

	char* tmpChar = new char[outputLength];
	unsigned char* tmpChain = (unsigned char*)tmpChar;

	// lanzamos Recursión
	*tmpChain = ASNSequence;
	tmpChain++;
	writeLength(&tmpChain, tmpLength);
	CCPropBag * cpb = (CCPropBag *) pb;
	cpb->WriteASN(&tmpChain);

	::SysFreeString(*bstrOut);
	
	// UNICODE VB
	*bstrOut = M4CppToVBBinary( tmpChar, (char*)tmpChain - tmpChar ) ;

	delete[] tmpChar;

	return S_OK;
}


STDMETHODIMP CConvertPropBag::Testeo(BSTR strName)
{
	return S_OK;

}

STDMETHODIMP CConvertPropBag::PrePersistProcess(ICPropBag * * pb)
{
	// TODO: Add your implementation code here
	CCPropBag * cpb = (CCPropBag *) *pb;

	cpb->PrePersistProccess();

	return S_OK;
}



STDMETHODIMP CConvertPropBag::PBFromStr(BSTR strIn, ICPropBag * * pb)
{
	// TODO: Add your implementation code here
 
/*	CParser MyParser;

	MyParser.Init(strIn);
	LPSTR nLine = NULL;
	do
	{
		nLine = MyParser.GetNextLine();
		ATLTRACE("%s\n",nLine);
	}while(nLine != NULL);

	delete [] nLine;*/
//	test += 1;	
	return S_OK;
}

STDMETHODIMP CConvertPropBag::PersistPBDProccess(ICPropBagDef * pPersistPBD, IPersistObject * pPersistObject)
{

	// Nos pasan un PersistObject ya inicializado
	// Lo aseguramos antes que nada
	pPersistObject->AddRef(); 
	pPersistPBD->AddRef();

	// Obtenemos cuantos bloques de traducción tenemos que construir
	ITraductProp * pTraduct;
	HRESULT hr = pPersistObject->get_PersistTraductProp(&pTraduct);
	if ((hr == S_OK) && (pTraduct))
	{
		short nLangNumber = 0;
		hr = pTraduct->get_PropertyNumber(&nLangNumber);
		
		CASNPersistPBD PersistObject;

		PersistObject.PersistIdHelpTopic(m_bPersistIdHelpTopic);
		
		// Hacemos que se procese
		PersistObject.InitASNBuffers(pTraduct);
		PersistObject.ProccessPBD(pPersistPBD);

		// Recogemos los resultados
		BSTR tmpBSTR = PersistObject.GetMainString();

		if (tmpBSTR)
		{
			pPersistObject->put_PersistMain(tmpBSTR);
			::SysFreeString(tmpBSTR);
		}
		else
			hr = E_FAIL;

		// Ahora recogemos los valores de lenguaje
		VARIANT v;
		::VariantInit(&v);
		for (int i = 0; i<nLangNumber; i++)
		{
			tmpBSTR = PersistObject.GetLangString(i);
			if (tmpBSTR)
			{
				v.vt = VT_I4;
				v.lVal = (long)i+1;
				pTraduct->put_ValueByLng(v, tmpBSTR);
				::SysFreeString(tmpBSTR);
			}
			else
			{
				hr = E_FAIL;
			}
			::VariantClear(&v);
		}
		// Y nos vamos ...
		RELEASE(pTraduct);
	}

	RELEASE(pPersistPBD);
	RELEASE(pPersistObject);
	return S_OK;
}



void CConvertPropBag::WriteDataProp(CDataProperties * pActualData, CDataProperties * pInheritData, char ** ppstrOBL, int nLevel)
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
	long nStartPos = -6;
	long nIdPresentation = -1;

	nLevel+=1;
	
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
	
	if ((pstrIdChannel) && (strcmp(pstrIdChannel, m_pstrIdChannel) !=0))
	{
		char *pChann = (char*)malloc(strlen(pstrIdChannel)+3);
		sprintf(pChann, "\"%s\"", pstrIdChannel);
		WriteLevelProperty("Idchannel", pChann, ppstrOBL, nLevel);
		free(pChann);
	}

	if ((pstrIdNode) && (strcmp(pstrIdNode, m_pstrIdNode) !=0))
	{
		char *pTmp = (char*)malloc(strlen(pstrIdNode)+3);
		sprintf(pTmp, "\"%s\"", pstrIdNode);
		WriteLevelProperty("Idnode", pTmp, ppstrOBL, nLevel);
		free(pTmp);
	}

	if ((pstrIdItem) && (strcmp(pstrIdItem, m_pstrIdItem) !=0))
	{
		char *pTmp = (char*)malloc(strlen(pstrIdItem)+3);
		sprintf(pTmp, "\"%s\"", pstrIdItem);
		WriteLevelProperty("Iditem", pTmp, ppstrOBL, nLevel);
		free(pTmp);
	}

	if ((pstrIdChannelKey) && (strcmp(pstrIdChannelKey, m_pstrIdChannelKey) !=0))
	{
		char *pTmp = (char*)malloc(strlen(pstrIdChannelKey)+3);
		sprintf(pTmp, "\"%s\"", pstrIdChannelKey);
		WriteLevelProperty("Idchannelkey", pTmp, ppstrOBL, nLevel);
		free(pTmp);
	}

	if ((pstrIdChannelAlias) && (strcmp(pstrIdChannelAlias, m_pstrIdChannelAlias) !=0))
	{
		char *pTmp = (char*)malloc(strlen(pstrIdChannelAlias)+3);
		sprintf(pTmp, "\"%s\"", pstrIdChannelAlias);
		WriteLevelProperty("Idchannelalias", pTmp, ppstrOBL, nLevel);
		free(pTmp);
	}

	if ((pstrIdAccessAlias) && (strcmp(pstrIdAccessAlias, m_pstrIdAccessAlias) !=0))
	{
		char *pTmp = (char*)malloc(strlen(pstrIdAccessAlias)+3);
		sprintf(pTmp, "\"%s\"", pstrIdAccessAlias);
		WriteLevelProperty("Idaccessalias", pTmp, ppstrOBL, nLevel);
		free(pTmp);
	}

	if (nGrants != m_nGrants)
	{
		char aData[10];
		sprintf(aData, "%d", nGrants);
		WriteLevelProperty("Grants", aData, ppstrOBL, nLevel);
	}
	
	if (nIdPresentation != m_nIdPresentation)
	{
		char aData[10];
		sprintf(aData, "%d", nIdPresentation);
		WriteLevelProperty("Idpresentation", aData, ppstrOBL, nLevel);
	}

	if (nStartPos != m_nStartPos)
	{
		char aData[10];
		sprintf(aData, "%d", nStartPos);
		WriteLevelProperty("Startpos", aData, ppstrOBL, nLevel);
	}

}

void CConvertPropBag::WriteFont(DefProp * pDefProp, char ** ppstrOBL, int nLevel)
{
	
	USES_CONVERSION;
	nLevel++;
	
	IM4FontHolder *pM4FontV = static_cast<IM4FontHolder*> (pDefProp->Value.pdispValue);
	IM4FontHolder *pM4FontDef = static_cast<IM4FontHolder*> (pDefProp->DefValue.pdispValue);

	if ((pM4FontV == NULL) || (pM4FontDef == NULL))
	{
		return;
	}
	
	BSTR sFontV = NULL;
	BSTR sFontDef = NULL;

	//-----------------------------------
	//Name.
	pM4FontV->get_Name(&sFontV);
	pM4FontDef->get_Name(&sFontDef);
	if (wcscmp(sFontV, sFontDef) != 0)
	{
		char *pFont = M4VBToCpp(sFontV);
		char *pTemp = (char*) malloc(strlen(pFont) + 3);
		sprintf(pTemp, "\"%s\"", pFont);
		delete pFont;
		WriteLevelProperty("Fontname", pTemp, ppstrOBL, nLevel);
		delete pTemp;
		pTemp = NULL;
	}
	::SysFreeString(sFontV);
	::SysFreeString(sFontDef);
	sFontV = NULL;
	sFontDef = NULL;
	//-----------------------------------

	//-----------------------------------
	//Bold.
	pM4FontV->get_FontBold(&sFontV);
	pM4FontDef->get_FontBold(&sFontDef);
	if (wcscmp(sFontV, sFontDef) != 0) {
		char * pTemp = NULL;
		if (wcscmp(sFontV, L"True") == 0) {
			pTemp = new char[strlen("True") + 3];
			sprintf(pTemp, "\"%s\"", "True");
		}
		else {
			pTemp = new char[strlen("False") + 3];
			sprintf(pTemp, "\"%s\"", "False");					
		}				
		WriteLevelProperty("Fontbold", pTemp, ppstrOBL, nLevel);
		delete [] pTemp;
	}
	::SysFreeString(sFontV);
	::SysFreeString(sFontDef);
	sFontV = NULL;
	sFontDef = NULL;
	//-----------------------------------

	//-----------------------------------
	//Italic.
	pM4FontV->get_FontItalic(&sFontV);
	pM4FontDef->get_FontItalic(&sFontDef);
	if (wcscmp(sFontV, sFontDef) != 0) {
		char * pTemp = NULL;
		if (wcscmp(sFontV, L"True") == 0) {
			pTemp = new char[strlen("True") + 3];
			sprintf(pTemp, "\"%s\"", "True");
		}
		else {
			pTemp = new char[strlen("False") + 3];
			sprintf(pTemp, "\"%s\"", "False");					
		}				
		WriteLevelProperty("Fontitalic", pTemp, ppstrOBL, nLevel);
		delete [] pTemp;
	}
	::SysFreeString(sFontV);
	::SysFreeString(sFontDef);
	sFontV = NULL;
	sFontDef = NULL;
	//-----------------------------------

	//-----------------------------------
	//Underline.
	pM4FontV->get_FontUnderline(&sFontV);
	pM4FontDef->get_FontUnderline(&sFontDef);
	if (wcscmp(sFontV, sFontDef) != 0) {
		char * pTemp = NULL;
		if (wcscmp(sFontV, L"True") == 0) {
			pTemp = new char[strlen("True") + 3];
			sprintf(pTemp, "\"%s\"", "True");
		}
		else {
			pTemp = new char[strlen("False") + 3];
			sprintf(pTemp, "\"%s\"", "False");					
		}				
		WriteLevelProperty("Fontunderline", pTemp, ppstrOBL, nLevel);
		delete [] pTemp;
	}
	::SysFreeString(sFontV);
	::SysFreeString(sFontDef);
	sFontV = NULL;
	sFontDef = NULL;
	//-----------------------------------

	//-----------------------------------
	//Size.
	pM4FontV->get_FontSize(&sFontV);
	pM4FontDef->get_FontSize(&sFontDef);
	if (wcscmp(sFontV, sFontDef) != 0)
	{
		char *pFont = M4VBToCpp(sFontV);
		char *pTemp = (char*) malloc(strlen(pFont) + 3);
		sprintf(pTemp, "\"%s\"", pFont);
		delete pFont;
		WriteLevelProperty("Fontsize", pTemp, ppstrOBL, nLevel);
		delete pTemp;
		pTemp = NULL;
	}
	::SysFreeString(sFontV);
	::SysFreeString(sFontDef);
	sFontV = NULL;
	sFontDef = NULL;
	//-----------------------------------
}

void CConvertPropBag::WriteTraductProp(ITraductProp * pTraduct, char * * ppstrOBL, int nLevel)
{
	USES_CONVERSION;

	VARIANT_BOOL bResult = 0; // Partimos de False
	BSTR bstrName = NULL;
	BSTR bstrValue = NULL;
	char* pTemp;
	int i = 0;

	nLevel ++;
	if (pTraduct)
	{
		pTraduct->TestEqual(&bResult);
		if (bResult)
		{
			pTraduct->get_Property(&bstrName);
			pTraduct->get_ValueByLng(CComVariant((long)1), &bstrValue);
			if (wcscmp(bstrValue, L"") != 0)
			{
				pTemp = M4VBToCpp(bstrValue);
				sprintf(m_charBuffer, "\"%s\"", pTemp);
				delete pTemp;

				pTemp = M4VBToCpp(bstrName);
				WriteLevelProperty(pTemp, m_charBuffer, ppstrOBL, nLevel);
				delete pTemp;
			}
		}
		else
		{
			// Hacemos el Split en multilenguaje
			// Los diferentes bloques van a los diferentes Buffers
			// Sólo va al Bloque principal el "NombrePropiedad" = "_Ref#"

			// Se acabaron los _Ref
			BSTR bstrTemp = NULL;
			char * pcRef = NULL;

/*			m_nRefCount += 1;
			char refLabel[10];
			sprintf(refLabel, "\"_Ref%d\"", m_nRefCount);*/
			
			pTraduct->get_Identifier(&bstrTemp);
			pcRef = M4VBToCpp(bstrTemp);
			SysFreeString(bstrTemp);
			// Creamos el Buffer real
			char * pcRef2 = (char*)malloc(strlen(pcRef) + 3);
			sprintf(pcRef2, "\"%s\"", pcRef);
			pTraduct->get_Property(&bstrName);

			pTemp = M4VBToCpp(bstrName);
			WriteLevelProperty(pTemp, pcRef2, ppstrOBL, nLevel);
			delete pTemp;

			for (i=1;i<=nLevel;i++)
			{
				WriteTabInString(m_nNumTab, ppstrOBL);
			}
			sprintf(m_charBuffer, "BEGIN TraductProp %s", pcRef);
			delete pcRef;
			CheckLengthMemory(ppstrOBL, strlen(m_charBuffer)+M4PROP_CR_SIZE);
			memcpy(*ppstrOBL, m_charBuffer, strlen(m_charBuffer));
			*ppstrOBL+=strlen(m_charBuffer);

			memcpy(*ppstrOBL, M4PROP_CR, M4PROP_CR_SIZE);
			*ppstrOBL += M4PROP_CR_SIZE;

			// Escribimos el nombre de la propiedad
			pTemp = M4VBToCpp(bstrName);
			RELEASE_BSTR(bstrName);
			sprintf(m_charBuffer, "\"%s\"", pTemp);
			delete pTemp;
			WriteLevelProperty("Name", m_charBuffer, ppstrOBL, nLevel+1);
			// Y ahora los valores por idiomas
			short nLang = 0;
			pTraduct->get_PropertyNumber(&nLang);

			for (i = 1; i <= nLang; i++)
			{
				char strName[15];
				bool bNullValue = true;

				pTraduct->get_ID((long)i, &bstrName);
				pTemp = M4VBToCpp(bstrName);
				RELEASE_BSTR(bstrName);
				sprintf(strName, "Value_%s", pTemp);
				delete pTemp;

				((CTraductProp *)pTraduct)->get_ValueByLng(CComVariant((long)i), &bstrName, bNullValue);
				if (bNullValue == false)
				{
					pTemp = M4VBToCpp(bstrName);
					sprintf(m_charBuffer, "\"%s\"", pTemp);
					delete pTemp;
					WriteLevelProperty(strName, m_charBuffer, ppstrOBL, nLevel+1);
				}
				RELEASE_BSTR(bstrName);
			}
			// Marchando una de END
			for (i=1;i<=nLevel;i++)
			{
				WriteTabInString(m_nNumTab, ppstrOBL);
			}
			CheckLengthMemory(ppstrOBL, 3 + M4PROP_CR_SIZE);
			strcpy(*ppstrOBL, "END");
			*ppstrOBL+=3;

			memcpy(*ppstrOBL, M4PROP_CR, M4PROP_CR_SIZE);
			*ppstrOBL += M4PROP_CR_SIZE;

			free(pcRef2);
		}
	}
	RELEASE_BSTR(bstrName);
	RELEASE_BSTR(bstrValue);
}

STDMETHODIMP CConvertPropBag::get_UsePersistLog(VARIANT_BOOL * pVal)
{
	if (pVal)
	{
		*pVal = m_bPersistLog;
	}

	return S_OK;
}

STDMETHODIMP CConvertPropBag::put_UsePersistLog(VARIANT_BOOL newVal)
{
	m_bPersistLog = newVal;

	return S_OK;
}

STDMETHODIMP CConvertPropBag::get_LogData(BSTR * pVal)
{
	USES_CONVERSION;

	if (pVal)
	{
		if (*pVal)
		{
			::SysFreeString(*pVal);
		}
		char * pBuffer = m_Log.GetBuffer();
		if (pBuffer)
		{
			*pVal = M4CppToVB(pBuffer);
		}
		else
		{
			*pVal = ::SysAllocString(L"No Log Data");
		}
	}
	return S_OK;
}

STDMETHODIMP CConvertPropBag::AccessViolation()
{
	CConvertPropBag * pProp = NULL;
	pProp->AccessViolation();
	return S_OK;
}

STDMETHODIMP CConvertPropBag::ConstructPresentation(IPersistObject * pPO, Iveranle * pAnle, ICPropBagDef * * pPBDef)
{
	HRESULT hr = E_INVALIDARG;
	
	if (pPO && pAnle && pPBDef)
	{

		CPBDDecode oDecode;
		
		oDecode.SetConvert(this);
		oDecode.SetAnle(pAnle);
		hr = oDecode.InitFromPersistObject(pPO);
		if (hr == S_OK)
		{
			RELEASE((*pPBDef));
			(*pPBDef) = oDecode.DecodeMainString(pPO);
#ifdef PROP_STAT
			RELEASE(m_pPropStat);
			m_pPropStat = oDecode.GetPropStat();
			if (m_pPropStat)
				m_pPropStat->AddRef();
#endif

		}
	}
	return hr;

}

ICPropBagDef * CConvertPropBag::ConstructPBDefFromASN(unsigned char * theBuffer)
{

	if (theBuffer == NULL || m_pAnle == NULL) return NULL;


	// Aquí empieza el origen de la creación, la belleza de la persistencia de objetos
	// en meros datos y la vuelta a la vida de los mismos en forma de PropertyBagsDef's
	//
	//												JFM 1998

	BYTE ucType;
	long lengthPropBag;
	ICPropBagDef * pPBDef = NULL;
	
	ucType = readByte(&theBuffer);
	if (ucType == ASNSequence)
	{
		readLength(&theBuffer, &lengthPropBag);
		// Siempre vamos a buscar la secuencia ClassName / Alias
		long nTempLength = GetPBDefFromASN(&theBuffer, &pPBDef);
		return pPBDef;

	}
	return  NULL;

}

long CConvertPropBag::GetPBDefFromASN(BYTE * * theBuffer, ICPropBagDef * * ppPBDef)
{

	USES_CONVERSION;
	
	// Debemos estar correctamente posicionados (ClassName/Alias)
	if (theBuffer && ppPBDef && m_pAnle)
	{
		// Leemos ClassName
		long nLength = 0;
		BYTE bType = readByte(theBuffer);
		char * pstrClassName = NULL, * pstrAlias = NULL;
		long nTemp = 1;
		if (bType == ASNGeneralString || bType == ASNUnicodeString)
		{
			nTemp += readLength(theBuffer, &nLength);
			if (nLength)
			{
				pstrClassName = readString(theBuffer, nLength, bType);
				// Esto Debería haber sido la ClassName
				if (strcmp(pstrClassName, "Classname") == 0)
				{
					nTemp += nLength;
					delete [] pstrClassName;
					// Vamos a por el valor de verdad
					bType = readByte(theBuffer);
					nTemp ++;
					if (bType != ASNGeneralString && bType != ASNUnicodeString)
					{
						return 0;
					}
					nTemp += readLength(theBuffer, &nLength);
					nTemp += nLength;
					pstrClassName = readString(theBuffer, nLength, bType);
				}
				else
					return 0;
			}
			else
				return 0;

			// Vamos a por el Alias
			bType = readByte(theBuffer);
			nTemp ++;
			if (bType != ASNGeneralString && bType != ASNUnicodeString)
			{
				delete [] pstrClassName;
				return 0;
			}
			
			nTemp += readLength(theBuffer, &nLength);
			if (nLength)
			{
				nTemp += nLength;
				pstrAlias = readString(theBuffer, nLength, bType);
				// Miramos a ver si es el Alias
				if (strcmp(pstrAlias, "Alias") == 0)
				{
					delete [] pstrAlias;
					bType = readByte(theBuffer);
					nTemp++;
					if (bType != ASNGeneralString && bType != ASNUnicodeString)
					{
						delete [] pstrClassName;
						return 0;
					}
					nTemp += readLength(theBuffer, &nLength);
					nTemp += nLength;
					pstrAlias = readString(theBuffer, nLength, bType);
				}
				else
				{
					delete [] pstrClassName;
					return 0;
				}
			}
			else
			{
				delete [] pstrClassName;
				return 0;
			}
		}

		BSTR bstrTemp = M4CppToVB(pstrClassName);
		HRESULT hr = m_pAnle->get_PropBagDefFromClassname(bstrTemp, ppPBDef);
		::SysFreeString(bstrTemp);
		bstrTemp = NULL;
		if (hr == S_OK)
		{
			CCPropBagDef * pPBD = static_cast<CCPropBagDef*> (*ppPBDef);
			pPBD->ChangeAlias(pstrAlias);
		}
		
		delete [] pstrClassName;
		delete [] pstrAlias;
		return nTemp;
	}
	return 0;
}

STDMETHODIMP CConvertPropBag::PBFromASNString(BSTR bstrIn, ICPropBag **ppPBRet)
{
	
	// Lo pasamos al Buffer adecuado y luego procesamos

	if ((bstrIn == NULL) || (ppPBRet == NULL)) return E_INVALIDARG;

	RELEASE((*ppPBRet));

	long nLong = ::SysStringLen(bstrIn);
	if (nLong <= 0) return E_INVALIDARG;

	// UNICODE VB
	char*	tmpBuff = M4VBToCppBinary( bstrIn, nLong) ;
	
	*ppPBRet = ConstructPBFromASN1((unsigned char*)tmpBuff, NULL, NULL);

	delete[] tmpBuff;

	return S_OK;
}

STDMETHODIMP CConvertPropBag::PBDef2PB(ICPropBagDef *pPBDSource, VARIANT_BOOL bTranslate, VARIANT vIndex, ITranslate * pTranslate, ICPropBag **pPBRet)
{
	
	HRESULT hr = E_INVALIDARG;
	if ((pPBDSource) && (pPBRet))
	{
		RELEASE((*pPBRet));
		if (bTranslate)
			m_bTranslatePB = true;
		else
			return E_NOTIMPL;

		
		RELEASE(m_pTranslate);
		m_pTranslate = pTranslate;
		if (m_pTranslate) m_pTranslate->AddRef();
		m_vIndex = vIndex;
		// Creamos la correspondiente a la parte básica y luego la pasamos
		CComObject<CCPropBag> * pPB;
		if (CComObject<CCPropBag>::CreateInstance(&pPB) == S_OK)
		{
			CDataProperties * tmpData = NULL;
			pPB->AddRef();
			if (ProcessPBD2PB(pPBDSource, pPB, tmpData) == S_OK)
			{
				hr = S_OK;
				*pPBRet = pPB;
			}
			else
				RELEASE(pPB);
		}
	}

	return hr;
}

HRESULT CConvertPropBag::ProcessPBD2PB(ICPropBagDef *pPBDSource, ICPropBag *pPBDest, CDataProperties * pInhData)
{
	// Primero copiamos ClassName y Alias
	BSTR bstrTemp = NULL;
	pPBDSource->get_ClassName(&bstrTemp);
	pPBDest->put_ClassName(bstrTemp);
	pPBDSource->get_Alias(&bstrTemp);
	pPBDest->put_Alias(bstrTemp);
	::SysFreeString(bstrTemp);
	CDataProperties * pData = NULL;
	char * pTemp = NULL;
	

	// Luego las propiedades (La parte interesante).
	ICPropColDef * pPropCol = NULL;
	long nCount = 0, i;

	if (pPBDSource->get_PropColDef(&pPropCol) == S_OK)
	{
		// Primero vamos a por las propiedades de datos
		ICDefProp * pProp = NULL;
		pPropCol->get_Item(M4CComVariant("DataProps"), &pProp);
		if (pProp)
		{
			CComVariant vValue;
			pProp->get_Value(&vValue);
			pData = static_cast<CDataProperties*>(vValue.pdispVal);
			if (pInhData)
			{
				if (pInhData->m_nmIdChannel != pData->m_nmIdChannel && (pData->m_nmIdChannel != -1))
				{
					pTemp = GetcharFromId(pData->m_nmIdChannel);
					if (strcmp(pTemp, "") != 0)
					{
						pPBDest->AddProperty(M4CComBSTR("Idchannel"), M4CComVariant(pTemp));
					}
				}
				if (pInhData->m_nmIdChannelKey != pData->m_nmIdChannelKey && (pData->m_nmIdChannelKey != -1))
				{
					pTemp = GetcharFromId(pData->m_nmIdChannelKey);
					if (strcmp(pTemp, "") != 0)
					{
						pPBDest->AddProperty(M4CComBSTR("Idchannelkey"), M4CComVariant(pTemp));
					}
				}
				if (pInhData->m_nmIdChannelAlias != pData->m_nmIdChannelAlias && (pData->m_nmIdChannelAlias != -1))
				{
					pTemp = GetcharFromId(pData->m_nmIdChannelAlias);
					if (strcmp(pTemp, "") != 0)
					{
						pPBDest->AddProperty(M4CComBSTR("Idchannelalias"), M4CComVariant(pTemp));
					}
				}
				if (pInhData->m_nmIdAccessAlias != pData->m_nmIdAccessAlias && (pData->m_nmIdAccessAlias != -1))
				{
					pTemp = GetcharFromId(pData->m_nmIdAccessAlias);
					if (strcmp(pTemp, "") != 0)
					{
						pPBDest->AddProperty(M4CComBSTR("Idaccessalias"), M4CComVariant(pTemp));
					}
				}
				if (pInhData->m_nmIdNode != pData->m_nmIdNode && (pData->m_nmIdNode != -1))
				{
					pTemp = GetcharFromId(pData->m_nmIdNode);
					if (strcmp(pTemp, "") != 0)
					{
						pPBDest->AddProperty(M4CComBSTR("Idnode"), M4CComVariant(pTemp));
					}
				}
				if (pInhData->m_nmIdItem != pData->m_nmIdItem && (pData->m_nmIdItem != -1))
				{
					pTemp = GetcharFromId(pData->m_nmIdItem);
					if (strcmp(pTemp, "") != 0)
					{
						pPBDest->AddProperty(M4CComBSTR("Iditem"), M4CComVariant(pTemp));
					}
				}
				if ((pData->m_nIdPresentation != pInhData->m_nIdPresentation) && (pData->m_nIdPresentation != -1))
				{
					pPBDest->AddProperty(M4CComBSTR("Idpresentation"), CComVariant(pData->m_nIdPresentation));
				}
				if ((pData->m_nGrants != pInhData->m_nGrants) && (pData->m_nGrants != 31))
				{
					pPBDest->AddProperty(M4CComBSTR("Grants"), CComVariant(pData->m_nGrants));
				}
				if ((pData->m_nStartPos != pInhData->m_nStartPos) && (pData->m_nStartPos != -6))
				{
					pPBDest->AddProperty(M4CComBSTR("Startpos"), CComVariant(pData->m_nStartPos));
				}
			}
			else
			{
				if (pData)
				{
					pTemp = GetcharFromId(pData->m_nmIdChannel);
					if (pTemp && (strcmp(pTemp, "") != 0))
					{
						pPBDest->AddProperty(M4CComBSTR("Idchannel"), M4CComVariant(pTemp));
					}
					pTemp = GetcharFromId(pData->m_nmIdAccessAlias);
					if (pTemp && (strcmp(pTemp, "") != 0))
					{
						pPBDest->AddProperty(M4CComBSTR("Idaccessalias"), M4CComVariant(pTemp));
					}
					pTemp = GetcharFromId(pData->m_nmIdChannelKey);
					if (pTemp && (strcmp(pTemp, "") != 0))
					{
						pPBDest->AddProperty(M4CComBSTR("Idchannelkey"), M4CComVariant(pTemp));
					}
					pTemp = GetcharFromId(pData->m_nmIdChannelAlias);
					if (pTemp && (strcmp(pTemp, "") != 0))
					{
						pPBDest->AddProperty(M4CComBSTR("Idchannelalias"), M4CComVariant(pTemp));
					}
					pTemp = GetcharFromId(pData->m_nmIdNode);
					if (pTemp && (strcmp(pTemp, "") != 0))
					{
						pPBDest->AddProperty(M4CComBSTR("Idnode"), M4CComVariant(pTemp));
					}
					pTemp = GetcharFromId(pData->m_nmIdItem);
					if (pTemp && (strcmp(pTemp, "") != 0))
					{
						pPBDest->AddProperty(M4CComBSTR("Iditem"), M4CComVariant(pTemp));
					}
					if (pData->m_nIdPresentation != -1)
					{
						pPBDest->AddProperty(M4CComBSTR("Idpresentation"), CComVariant(pData->m_nIdPresentation));
					}
					if (pData->m_nGrants != 31)
					{
						pPBDest->AddProperty(M4CComBSTR("Grants"), CComVariant(pData->m_nGrants));
					}
					if (pData->m_nStartPos != -6)
					{
						pPBDest->AddProperty(M4CComBSTR("Startpos"), CComVariant(pData->m_nStartPos));
					}
				}
			}
			RELEASE(pProp);
		}
		
		pPropCol->get_Count(&nCount);
		for (i = 1; i <= nCount; i++)
		{
			pPropCol->get_Item(CComVariant(i), &pProp);
			AddProperty2PB(pProp, pPBDest, pData);
			RELEASE(pProp);
		}
		RELEASE(pPropCol);
	}

	// La Recursividad
	ICChildColDef * pChild = NULL;
	
	if (pPBDSource->get_ChildColDef(&pChild) == S_OK)
	{
		pChild->get_Count(&nCount);
		for (i = 1; i <= nCount; i++)
		{
			ICPropBagDef * pPBDChild = NULL;
			if (pChild->get_Item(CComVariant(i), &pPBDChild) == S_OK)
			{
				CComObject<CCPropBag> * pPB;
				if (CComObject<CCPropBag>::CreateInstance(&pPB) == S_OK)
				{
					ICPropBag * pIPB;
					pPB->QueryInterface(IID_ICPropBag, (void**)&pIPB);
					if (ProcessPBD2PB(pPBDChild, pPB, pData) == S_OK)
					{
						pPBDest->AddChild(&pIPB);						
					}
					// Si está enlazada no se destruye, en otro caso (POOR PETER ... )					
					RELEASE(pPB);										
				}
				RELEASE(pPBDChild);
			}
		}
		RELEASE(pChild);
	}

	return S_OK;
}

void CConvertPropBag::AddProperty2PB(ICDefProp *pProp, ICPropBag *pPBDest, CDataProperties * pData)
{
	CComVariant vVar1, vVar2;
	M4PropertyType propType;
	BSTR bstrTemp = NULL;

	pProp->get_Type(&propType);
	
	if (propType == FE_TRADUCTPROP)
	{
		// enganchamos el valor
		ITraductProp * pTraduct = NULL;
		pProp->get_Value(&vVar1);
		if (vVar1.vt == VT_DISPATCH)
		{
			if (vVar1.pdispVal->QueryInterface(IID_ITraductProp, (void**)&pTraduct) == S_OK)
			{
				pTraduct->get_ValueByLng(m_vIndex, &bstrTemp);
				if (wcscmp(L"", bstrTemp) != 0)
				{
					// Añadimos el valor
					BSTR bstrTemp2 = NULL;;
					pProp->get_Name(&bstrTemp2);
					// Primero Miramos si hay que filtrar por el objeto de traducción
					if (m_pTranslate)
					{
						// Hacemos el Intento de transformación
						if (wcsncmp(L"##", bstrTemp, 2) == 0)
						{
							BSTR bstrIdChannel = GetBSTRFromId(pData->m_nmIdChannel);
							BSTR bstrIdNode = GetBSTRFromId(pData->m_nmIdNode);
							m_pTranslate->TranslateString(bstrTemp, bstrIdChannel, bstrIdNode, &bstrTemp);
							::SysFreeString(bstrIdChannel);
							::SysFreeString(bstrIdNode);
						}
					}
					pPBDest->AddProperty(bstrTemp2, CComVariant(bstrTemp));
				}
				::SysFreeString(bstrTemp);
				bstrTemp = NULL;
				RELEASE(pTraduct);
			}
		}
	}
	else if (propType == FE_FONT)
	{
		IM4FontHolder * pM4Font1 = NULL, * pM4Font2 = NULL;
		pProp->get_Value(&vVar1);
		pProp->get_DefValue(&vVar2);
		if (vVar1.vt == VT_DISPATCH)
		{
			vVar1.pdispVal->QueryInterface(IID_IM4FontHolder, (void**) &pM4Font1);
		}
		if (vVar2.vt == VT_DISPATCH)
		{
			vVar2.pdispVal->QueryInterface(IID_IM4FontHolder, (void**) &pM4Font2);
		}

		BSTR sFont1 = NULL;
		BSTR sFont2 = NULL;

		//-----------------------------------
		//Name.
		pM4Font1->get_Name(&sFont1);
		pM4Font2->get_Name(&sFont2);
		if (wcscmp(sFont1, sFont2) != 0) {
			pPBDest->AddProperty(M4CComBSTR("Fontname"), CComVariant(sFont1));
		}
		::SysFreeString(sFont1);
		::SysFreeString(sFont2);
		sFont1 = NULL;
		sFont2 = NULL;
		//-----------------------------------
		
		//-----------------------------------
		//Bold.
		pM4Font1->get_FontBold(&sFont1);
		pM4Font2->get_FontBold(&sFont2);
		if (wcscmp(sFont1, sFont2) != 0) {
			if (wcscmp(sFont1, L"True") == 0) {
				pPBDest->AddProperty(M4CComBSTR("Fontbold"), M4CComVariant("True"));
			}
			else {
				pPBDest->AddProperty(M4CComBSTR("Fontbold"), M4CComVariant("False"));
			}
		}
		::SysFreeString(sFont1);
		::SysFreeString(sFont2);
		sFont1 = NULL;
		sFont2 = NULL;
		//-----------------------------------

		//-----------------------------------
		//Italic.
		pM4Font1->get_FontItalic(&sFont1);
		pM4Font2->get_FontItalic(&sFont2);
		if (wcscmp(sFont1, sFont2) != 0) {
			if (wcscmp(sFont1, L"True") == 0) {
				pPBDest->AddProperty(M4CComBSTR("Fontitalic"), M4CComVariant("True"));
			}
			else {
				pPBDest->AddProperty(M4CComBSTR("Fontitalic"), M4CComVariant("False"));
			}
		}
		::SysFreeString(sFont1);
		::SysFreeString(sFont2);
		sFont1 = NULL;
		sFont2 = NULL;
		//-----------------------------------

		//-----------------------------------
		//Underline.
		pM4Font1->get_FontUnderline(&sFont1);
		pM4Font2->get_FontUnderline(&sFont2);
		if (wcscmp(sFont1, sFont2) != 0) {
			if (wcscmp(sFont1, L"True") == 0) {
				pPBDest->AddProperty(M4CComBSTR("Fontunderline"), M4CComVariant("True"));
			}
			else {
				pPBDest->AddProperty(M4CComBSTR("Fontunderline"), M4CComVariant("False"));
			}
		}
		::SysFreeString(sFont1);
		::SysFreeString(sFont2);
		sFont1 = NULL;
		sFont2 = NULL;
		//-----------------------------------

		//-----------------------------------
		//Size.
		pM4Font1->get_FontSize(&sFont1);
		pM4Font2->get_FontSize(&sFont2);
		if (wcscmp(sFont1, sFont2) != 0) {				
			pPBDest->AddProperty(M4CComBSTR("Fontsize"), CComVariant(sFont1));
		}
		::SysFreeString(sFont1);
		::SysFreeString(sFont2);
		sFont1 = NULL;
		sFont2 = NULL;
		//-----------------------------------

		RELEASE(pM4Font1);
		RELEASE(pM4Font2)
	}
	else if (propType == FE_DATAPROP)
	{
		
	}
	else if (propType == FE_STRING)
	{
		pProp->get_Value(&vVar1);
		pProp->get_DefValue(&vVar2);
		if (vVar1 != vVar2)
		{
			BSTR bstrTemp = NULL;
			pProp->get_Name(&bstrTemp);
			if (m_pTranslate)
			{
				// Hacemos el Intento de transformación
				if ((wcsncmp(L"##", bstrTemp, 2) != 0) && pData)
				{
					BSTR bstrIdChannel = GetBSTRFromId(pData->m_nmIdChannel);
					BSTR bstrIdNode = GetBSTRFromId(pData->m_nmIdNode);
					m_pTranslate->TranslateString(vVar1.bstrVal, bstrIdChannel, bstrIdNode, &(vVar1.bstrVal));
					::SysFreeString(bstrIdChannel);
					::SysFreeString(bstrIdNode);
				}
			}
			pPBDest->AddProperty(bstrTemp, vVar1);
			::SysFreeString(bstrTemp);
		}
	}
	else  // Todos los tipos no basados en Objetos
	{
		pProp->get_Value(&vVar1);
		pProp->get_DefValue(&vVar2);
		if (vVar1 != vVar2)
		{
			BSTR bstrTemp = NULL;
			pProp->get_Name(&bstrTemp);
			pPBDest->AddProperty(bstrTemp, vVar1);
			::SysFreeString(bstrTemp);
		}
	}
	
}

void CConvertPropBag::WriteLevelInString(char *prop,VARIANT value,char ** myOBL,long level)
{
	// Mantenida por compatibilidad hacia atrás con las PB
	
	USES_CONVERSION;

	long i, lenValue;;
	char *igual = " = ";
	char *comilla = "\"";
	//char *CR = "\n";
	char *myValue, *tmpStr = (char *)malloc(100);
	
	for (i=1;i<=level;i++)
		WriteTabInString(m_nNumTab, myOBL);

	long lenProp = strlen(prop);
	CheckLengthMemory(myOBL,lenProp);
	memcpy((*myOBL),prop,lenProp);
	(*myOBL) += lenProp;

	if (strcmp(prop, "END") != 0)
	{
		switch (value.vt){
		case VT_BSTR :{
			CheckLengthMemory(myOBL,3);
			memcpy(*myOBL,igual,3);
			(*myOBL) += 3;
			CheckLengthMemory(myOBL,1);
			memcpy(*myOBL,comilla, 1);
			(*myOBL) ++;
			myValue= M4VBToCpp(value.bstrVal);
			lenValue = strlen(myValue);
			CheckLengthMemory(myOBL,lenValue);
			memcpy((*myOBL),myValue,lenValue);
			delete myValue;
			(*myOBL) += lenValue;
			CheckLengthMemory(myOBL,1);
			memcpy(*myOBL,comilla, 1);
			(*myOBL) ++;
			break;
		}
		case VT_I2:
			CheckLengthMemory(myOBL,3);
			memcpy(*myOBL,igual,3);
			(*myOBL) += 3;
			tmpStr = _itoa(value.iVal, (char *)tmpStr,10);
			lenValue = strlen(tmpStr);
			CheckLengthMemory(myOBL,lenValue);
			memcpy((*myOBL),tmpStr,lenValue);
			(*myOBL) += lenValue;
			break;
		case VT_I4:
			CheckLengthMemory(myOBL,3);
			memcpy(*myOBL,igual,3);
			(*myOBL) += 3;
			tmpStr = _ltoa(value.lVal, (char *)tmpStr,10);
			lenValue = strlen(tmpStr);
			CheckLengthMemory(myOBL,lenValue);
			memcpy((*myOBL),tmpStr,lenValue);
			(*myOBL) += lenValue;
			break;
		case VT_BOOL:
			CheckLengthMemory(myOBL,3);
			memcpy((*myOBL),igual,3);
			(*myOBL) += 3;
			if (value.bVal == 0)
				strcpy(tmpStr,"False");
			else
				strcpy(tmpStr,"True");
			lenValue = strlen(tmpStr);
			CheckLengthMemory(myOBL,lenValue);
			memcpy((*myOBL),tmpStr,lenValue);
			(*myOBL) += lenValue;
			break;
		}
	}
	CheckLengthMemory(myOBL,M4PROP_CR_SIZE);

	memcpy(*myOBL, M4PROP_CR, M4PROP_CR_SIZE);
	*myOBL += M4PROP_CR_SIZE;

	free(tmpStr);
}


STDMETHODIMP CConvertPropBag::get_PropStat(ICPropCol **pVal)
{
#ifdef PROP_STAT
	*pVal = m_pPropStat;
	if (m_pPropStat)
		m_pPropStat->AddRef();
#endif

	return S_OK;
}

STDMETHODIMP CConvertPropBag::ClonePBDefRec(ICPropBagDef *pPBDSource, Iveranle *pAnle, ICPropBagDef **pPBDDest)
{
	if (!pPBDSource || !pAnle || !pPBDDest) return E_INVALIDARG;

	*pPBDDest = NULL;

	RELEASE(m_pAnle);
	m_pAnle = pAnle;
	m_pAnle->AddRef();

	return InnerClonePBDefRec(pPBDSource, pPBDDest);

}

HRESULT CConvertPropBag::InnerClonePBDefRec(ICPropBagDef *pSource, ICPropBagDef **pDest)
{
	// Partimos de que el Anle está correctamente inicializado;
	// Se crea la PB nueva
	// Se copian las propiedades
	// Se Itera Recursivamente

	CCPropBagDef * pCPSource = static_cast<CCPropBagDef*>(pSource);

	*pDest = NULL;
	BSTR bstrClassName = NULL;
	pSource->get_ClassName(&bstrClassName);

	if (m_pAnle->get_PropBagDefFromClassname(bstrClassName, pDest) == S_OK)
	{
		// Tenemos el Source en pSource y el destino en *pDest
		ICPropBagDef * pSourceChild = NULL;
		long nCount;
		
		CopyPBD2PBDProp(*pDest, pSource);

		pCPSource->m_ChildCol->get_Count(&nCount);
		for (long n = 1; n <= nCount; n++)
		{
			ICPropBagDef * pSourcePB = NULL;
			ICPropBagDef * pDestPB = NULL;

			pCPSource->m_ChildCol->get_Item(CComVariant(n), &pSourcePB);
			if (InnerClonePBDefRec(pSourcePB, &pDestPB) == S_OK)
			{
				(*pDest)->AddChild(&pDestPB, CComVariant());
				RELEASE(pDestPB);
			}
			RELEASE(pSourcePB);			
		}
	}
	else
	{
		return	E_INVALIDARG;
	}
	SysFreeString(bstrClassName);
	return S_OK;
}

/*void CConvertPropBag::CopyPBD2PBDProp(CCPropBagDef *pCPDest, CCPropBagDef *pCPSource)
{




}*/

STDMETHODIMP CConvertPropBag::CopyPBDef2PBDefProp(ICPropBagDef *pPBDDest, ICPropBagDef *pPBDSource)
{
	// TODO: Add your implementation code here
	if (pPBDDest && pPBDSource) 
	{
		CopyPBD2PBDProp(pPBDDest, pPBDSource);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP CConvertPropBag::get_PersistIdHelpTopic(VARIANT_BOOL *pVal)
{
	if (pVal == NULL) return E_POINTER;
	
	if (m_bPersistIdHelpTopic)
		*pVal = -1;
	else
		*pVal = 0;

	return S_OK;
}

STDMETHODIMP CConvertPropBag::put_PersistIdHelpTopic(VARIANT_BOOL newVal)
{
	if (newVal)
		m_bPersistIdHelpTopic = -1;
	else
		m_bPersistIdHelpTopic = 0;

	return S_OK;
}

STDMETHODIMP CConvertPropBag::ConstructPresentationFromArray(VARIANT vArray, Iveranle *pAnle, ICPropBagDef **pPBDef)
{
	if (!pAnle || !pPBDef || (vArray.vt != (VT_ARRAY | VT_VARIANT)))
		return E_INVALIDARG;

	//long nResID = 0;
	
	CPBDDecode oDecode;
		
	oDecode.SetConvert(this);
	oDecode.SetAnle(pAnle);

	/*if (pAnle)
	{
		pAnle->get_MetalengResourceID(&nResID);
	}*/

	if (vArray.vt == (VT_ARRAY | VT_VARIANT))
	{
		* pPBDef = oDecode.InitFromArray(vArray.parray);
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP CConvertPropBag::PBDef2XMLFile(ICPropBagDef *pPbDef, BSTR sFileName)
{
	USES_CONVERSION;
	
	CXMLPersist pObject;

	if (pPbDef && sFileName)
	{
		char *pTemp = M4VBToCpp(sFileName);
		return pObject.SavePBDef2File(pPbDef, pTemp);
		delete pTemp;
	}
	return E_FAIL;
	
}

STDMETHODIMP CConvertPropBag::PBDef2XMLString(ICPropBagDef *pPBDef, BSTR *bsRet)
{
	
	CXMLPersist pObject;

	if (pPBDef && bsRet)
	{

		return pObject.GetPBDef2String(pPBDef, bsRet);
	}
	return E_FAIL;

}

STDMETHODIMP CConvertPropBag::ConstructPresentationFromArrayVar(VARIANT vArray, Iveranle *pAnle, VARIANT *pVal)
{
	if (!pAnle || !pVal || (vArray.vt != (VT_ARRAY | VT_VARIANT)))
		return E_INVALIDARG;

	CPBDDecode oDecode;
		
	oDecode.SetConvert(this);
	oDecode.SetAnle(pAnle);
	if (vArray.vt == (VT_ARRAY | VT_VARIANT))
	{
		ICPropBagDef * pPBDef = NULL;
		pPBDef = oDecode.InitFromArray(vArray.parray);
		VariantInit(pVal);
		pVal->vt = VT_DISPATCH;
		pPBDef->QueryInterface(IID_IDispatch, (void**)&(pVal->pdispVal));
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP CConvertPropBag::ResetTradIdentifiers(ICPropBagDef * pPBD)
{
	// Recorrido recursivo de PBDefs reseteando identificadores de propiedades traducidas
	CTraductTools ctt;
	
	return ctt.ResetTradIdentifiers(pPBD);

}


STDMETHODIMP CConvertPropBag::PB2XML(ICPropBag *poPropBag, SAFEARRAY **abXML)
{

	long			iLength = 0;
	HRESULT			hr = S_OK;
	void*			poTmpBuffer = NULL;
	ClLongString	oXML ;


	poPropBag->get_Length( &iLength );
	oXML.Init( iLength, iLength / 10 );

	oXML.Concat( M4Utf8Header() );

	((CCPropBag *)poPropBag)->WriteXML( oXML );

	iLength = oXML.GetLength();

	*abXML = ::SafeArrayCreateVector( VT_UI1, 0, iLength );

	if( *abXML == NULL )
	{
		return S_FALSE;
	}

	hr = SafeArrayAccessData( *abXML, (void**)&poTmpBuffer );

	if( SUCCEEDED(hr) == FALSE )
	{
		::SafeArrayDestroy( *abXML ) ;
		return S_FALSE;
	}

	memcpy( poTmpBuffer, oXML.GetString(), iLength ) ;
	
	hr = SafeArrayUnaccessData( *abXML ) ;

	if( SUCCEEDED(hr) == FALSE )
	{
		::SafeArrayDestroy( *abXML ) ;
		return S_FALSE;
	}

	return S_OK;
}
