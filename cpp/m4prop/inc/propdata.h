// PropData.h: interface for the CPropData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPDATA_H__4E079D03_8BB0_11D3_BD46_00C04F71652D__INCLUDED_)
#define AFX_PROPDATA_H__4E079D03_8BB0_11D3_BD46_00C04F71652D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPropData  
{
public:
	CPropData();
	virtual ~CPropData();

	BSTR				m_bstrName;
	CComVariant			m_vValue;
	CComVariant			m_vDefValue;
	M4PropertyType		m_sType;
};

#endif // !defined(AFX_PROPDATA_H__4E079D03_8BB0_11D3_BD46_00C04F71652D__INCLUDED_)
