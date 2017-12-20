// DataProperties.h : Declaration of the CDataProperties

#ifndef __DATAPROPERTIES_H_
#define __DATAPROPERTIES_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDataProperties
class ATL_NO_VTABLE CDataProperties : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDataProperties, &CLSID_DataProperties>,
	public IDispatchImpl<IDataProperties, &IID_IDataProperties, &LIBID_M4PROPLib>
{
public:
	STDMETHOD(Clone)(/*[out, retval]*/ IDataProperties ** ppDataProps);
	STDMETHOD(get_Grants)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_Grants)(/*[in]*/ short newVal);
	STDMETHOD(get_IdPresentation)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_IdPresentation)(/*[in]*/ long newVal);
	STDMETHOD(get_IdAccessAlias)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IdAccessAlias)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_IdItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IdItem)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_IdNode)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IdNode)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_IdChannelKey)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IdChannelKey)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_IdChannelAlias)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IdChannelAlias)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_IdChannel)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IdChannel)(/*[in]*/ BSTR newVal);
	CDataProperties()
	{
		// Inicializamos con los valores por defecto
		m_nmIdChannel = -1;
		m_nmIdNode = -1;
		m_nmIdItem = -1;
		m_nmIdAccessAlias = -1;
		m_nmIdChannelKey = -1;
		m_nmIdChannelAlias = -1;
		m_nIdPresentation = -1;
		m_nGrants = 31; 
		m_nStartPos = -6;
		//ATLTRACE("Construccion DataProperties\n");

	}
	~CDataProperties()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DATAPROPERTIES)

BEGIN_COM_MAP(CDataProperties)
	COM_INTERFACE_ENTRY(IDataProperties)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IDataProperties
public:
	STDMETHOD(Reset)();
	STDMETHOD(get_StartPos)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_StartPos)(/*[in]*/ long newVal);

	
	long	m_nmIdChannel;
	long	m_nmIdNode;
	long	m_nmIdItem;
	long	m_nmIdAccessAlias;
	long	m_nmIdChannelKey;
	long	m_nmIdChannelAlias;
	long	m_nIdPresentation;
	long	m_nStartPos;
	short	m_nGrants;

};

#endif //__DATAPROPERTIES_H_
