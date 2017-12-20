// MyVariant.h: interface for the CMyVariant class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYVARIANT_H__44B7BAA8_26B9_11D2_969F_00C04FC2444B__INCLUDED_)
#define AFX_MYVARIANT_H__44B7BAA8_26B9_11D2_969F_00C04FC2444B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMyVariant  
{
public:
	void Clear();
	VARIANT m_vData;
	CMyVariant();
	virtual ~CMyVariant();

	CMyVariant& operator=( VARIANT Src);
	bool operator==(const CMyVariant & pSrc);
	bool operator!=(const CMyVariant & pSrc);
	bool EqualVARIANT (VARIANT var1,VARIANT var2);

};

#endif // !defined(AFX_MYVARIANT_H__44B7BAA8_26B9_11D2_969F_00C04FC2444B__INCLUDED_)
