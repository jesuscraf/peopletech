// CPropColDef.h : Declaration of the CCPropColDef

#ifndef __CPROPCOLDEF_H_
#define __CPROPCOLDEF_H_

#include "resource.h"       // main symbols


/////////////////////////////////////////////////////////////////////////////
// CCPropColDef
class ATL_NO_VTABLE CCPropColDef : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCPropColDef, &CLSID_CPropColDef>,
	//public CMyCollection<ICDefProp, IDispatchImpl<ICPropColDef, &IID_ICPropColDef, &LIBID_M4PROPLib> >
	//public CColTemplate<ICDefProp, IDispatchImpl<ICPropColDef, &IID_ICPropColDef, &LIBID_M4PROPLib> >
	public IDispatchImpl<ICPropColDef, &IID_ICPropColDef, &LIBID_M4PROPLib>
{
public:
	
	typedef CComEnum<CComIEnum<VARIANT>, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT> > _CComCollEnum;

	CCPropColDef()
	{
		m_nHashSize = HASH_SIZE;
		m_pNodeHead = NULL;
		m_pLastNode = NULL;
		m_nCount = 0;
		m_ppHashTable = NULL;
	}

	~CCPropColDef()
	{
		FreeAll();
	}

	
DECLARE_REGISTRY_RESOURCEID(IDR_CPROPCOLDEF)

BEGIN_COM_MAP(CCPropColDef)
	COM_INTERFACE_ENTRY(ICPropColDef)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


private:
	void AddPropToHash(DefProp * pDefProp);
	void RemovePropByPtr(DefProp * p);
	void RemovePropByPos(long nIndex);
	void RemovePropByKey(const char * key);
	long m_nHashSize;
	DefProp * m_pLastNode;
	DefProp * * m_ppHashTable;
	void InitHashTable();

public:
	HRESULT ResetTradIdentifiers();
	void Reset();
	STDMETHOD(Remove)(VARIANT index);
	STDMETHOD(get_Item)(VARIANT index, ICDefProp * * pRet);
	STDMETHOD(Add)(ICDefProp * pNew, VARIANT index);
	STDMETHOD(get_Count)(long *pVal);
	STDMETHOD(get__NewEnum)(LPUNKNOWN *ppVal);
	STDMETHOD(RemoveAll)();
	DefProp * GetDefProp(long nId);
	DefProp * GetDefProp(char * key);
	DefProp * m_pNodeHead;
	long m_nCount;
	HRESULT AddDefProp(DefProp * pDefProp, char * strKey);

protected:
	void FreeAll();
};

#endif //__CPROPCOLDEF_H_
