// CDefProp.h : Declaration of the CCDefProp

#ifndef __CDEFPROP_H_
#define __CDEFPROP_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCDefProp
class ATL_NO_VTABLE CCDefProp : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCDefProp, &CLSID_CDefProp>,
	public IDispatchImpl<ICDefProp, &IID_ICDefProp, &LIBID_M4PROPLib>
{
public:
 	CCDefProp()
	{
		m_pData = NULL;
	}

	~CCDefProp()
	{
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
DECLARE_REGISTRY_RESOURCEID(IDR_CDEFPROP)

BEGIN_COM_MAP(CCDefProp)
	COM_INTERFACE_ENTRY(ICDefProp)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


protected:


/*	BSTR				m_bstrName;
	VARIANT				m_vValue;
	VARIANT				m_vDefValue;
	M4PropertyType		m_sType;*/

// ICDefProp
public:
	DefProp *			m_pData;
	void SetData(DefProp * pData);
	STDMETHOD(get_Type)(/*[out, retval]*/ M4PropertyType *pVal);
	STDMETHOD(put_Type)(/*[in]*/ M4PropertyType newVal);
	STDMETHOD(get_DefValue)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_DefValue)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Value)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_Value)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);
	STDMETHOD(WriteASN)(/*[in, out]*/ unsigned char ** charIn);
//	STDMETHOD(get_Length)(/*[out, retval]*/ long *pVal);

};

#endif //__CDEFPROP_H_
