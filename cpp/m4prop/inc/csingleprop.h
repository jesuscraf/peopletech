// CSingleProp.h : Declaration of the CCSingleProp

#ifndef __CSINGLEPROP_H_
#define __CSINGLEPROP_H_

#include "resource.h"       // main symbols
#include "cllstr.hpp"
#include "m4unicode.hpp"
extern int	M4IsUnicode( void ) ;

/////////////////////////////////////////////////////////////////////////////
// CCSingleProp
class ATL_NO_VTABLE CCSingleProp : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCSingleProp, &CLSID_CSingleProp>,
	public IDispatchImpl<ICSingleProp, &IID_ICSingleProp, &LIBID_M4PROPLib>
{
public:
	CCSingleProp()
	{
		//##. Begin Adri y Juanma.
		//	Optimización de ECI. Eliminamos todas las cadenas que almacenan nombre
		//	de propiedades y las sustituimos por un código
		m_iNameCode = -1;
		//End

		VariantInit(&m_vValue);
		//ATLTRACE("Construcción de CSingleProp\n");
	}
	~CCSingleProp()
	{
		//##. Begin Adri y Juanma.
		//	Optimización de ECI. Eliminamos todas las cadenas que almacenan nombre
		//	de propiedades y las sustituimos por un código
		m_iNameCode = -1;
		//End

		VariantClear(&m_vValue);
		//ATLTRACE("Destrucción de CSingleProp\n");
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

void writeString(unsigned char **strASN, char * strValue, long nstrLength)
{
	if( M4IsUnicode() == 1 && M4IsAscii( (char*)strValue, nstrLength ) == 0 )
	{
		(**strASN) = ASNUnicodeString;
	}
	else
	{
		(**strASN) = ASNGeneralString;
	}

	(*strASN)++;
		//incremento en el num de bytes el puntero strASN dentro de funcion
	writeLength(strASN, nstrLength);
	if (nstrLength != 0)
	{
		memcpy((*strASN), strValue, nstrLength);
		(*strASN) += nstrLength;
	}

}

void writeBool(unsigned char **strASN, VARIANT_BOOL bValue)
{

	(**strASN) = ASNBOOLEAN;
	(*strASN)++;
	(**strASN) = 1;
	(*strASN)++;
	if (bValue)
		(**strASN) = 255;
	else
		(**strASN) = 0;
	(*strASN)++;
}


void writeInteger(unsigned char **strASN, long m_value)
{
	
	(**strASN) = ASNINTEGER;
	(*strASN)++;	
		
	long aux;
	int n_bytes = 0;
	unsigned char total_bytes[5],b_V;
	int i = 0,cont;
	unsigned long tmpMask;

	if ((m_value <= 127) && (m_value >= -128))
		cont = 1;
	else
		if ((m_value <= 32767) && (m_value >= -32768))
			cont = 2;
		else
			if ((m_value <= 8388607) && (m_value >= -8388608))
				cont = 3;
			else
				cont = 4;
	aux = m_value;
	i = cont;
	while(i-- > 0)
	{
		tmpMask = 255 << (8 * i);
		b_V =(unsigned char) ((m_value & tmpMask) >> (8*i));
		total_bytes[i] = b_V;
	}		
	(**strASN) = cont;
	(*strASN)++;
	for (i=cont-1;i>=0;i--)
	{
		(**strASN) = total_bytes[i];
		(*strASN)++;
	}

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
		(**strASN) = n_total;
		(*strASN) ++;
		for(j=i-1;j>=0;j--)
		{
			(**strASN) = m_bytes[j];
			(*strASN)++;
		}
		
	}
	else
	{
		n_total = (unsigned char)m_length;
		(**strASN) = n_total;
		(*strASN)++;
	}
}




DECLARE_REGISTRY_RESOURCEID(IDR_CSINGLEPROP)

BEGIN_COM_MAP(CCSingleProp)
	COM_INTERFACE_ENTRY(ICSingleProp)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ICSingleProp
public:
	STDMETHOD(WriteASN)(/*[in, out]*/ unsigned char** charIn);
	STDMETHOD(WriteXML)(/*[in, out]*/ ClLongString &roXML);
	STDMETHOD(get_Length)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Value)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_Value)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);

	//##. Begin Adri y Juanma.
	//	Optimización de ECI. Eliminamos todas las cadenas que almacenan nombre
	//	de propiedades y las sustituimos por un código
		//BSTR m_bstrName;
	int m_iNameCode;
	//End

	VARIANT m_vValue;

};

#endif //__CSINGLEPROP_H_
