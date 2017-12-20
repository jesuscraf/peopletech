// XMLPersist.h: interface for the CXMLPersist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLPERSIST_H__4BC14EA3_E4A6_11D3_BDB3_00C04F71652D__INCLUDED_)
#define AFX_XMLPERSIST_H__4BC14EA3_E4A6_11D3_BDB3_00C04F71652D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXMLPersist  
{
public:
	HRESULT GetPBDef2String(ICPropBagDef * pPBdef, BSTR * bsOut);
	HRESULT SavePBDef2File(ICPropBagDef * pPBDef, char * sFileName);
	CXMLPersist();
	virtual ~CXMLPersist();

protected:
	void PersistDataProp(MSXML::IXMLDOMNode * pNode, CDataProperties * pChildData, CDataProperties * pData);
	void PersistDefPropInXML(DefProp * pDefProp, MSXML::IXMLDOMNode * pNode, CDataProperties * pData);
	void AddPropsToNode(MSXML::IXMLDOMNode * pNode, ICPropBagDef * pPBDef, CDataProperties * pData);
	HRESULT ProcessRec(ICPropBagDef * pPBDef,MSXML::IXMLDOMNode * pRoot, CDataProperties * pData);
	HRESULT GetNewChild(MSXML::IXMLDOMNode ** ppNode);
	HRESULT SetAttribute(MSXML::IXMLDOMNode * pNode, BSTR AttName, VARIANT vValue);
	MSXML::IXMLDOMDocument * m_pDocument;
};

#endif // !defined(AFX_XMLPERSIST_H__4BC14EA3_E4A6_11D3_BDB3_00C04F71652D__INCLUDED_)
