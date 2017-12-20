// MyBSTR.h: interface for the CMyBSTR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBSTR_H__D0E49064_2358_11D2_B669_D4ED314D400F__INCLUDED_)
#define AFX_MYBSTR_H__D0E49064_2358_11D2_B669_D4ED314D400F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMyBSTR  
{
public:
	void Attach(BSTR newValue);
	BSTR m_BSTRData;
	CMyBSTR();
	virtual ~CMyBSTR();

	CMyBSTR& operator=(const CMyBSTR& str);
	CMyBSTR& operator=(const BSTR str);
	CMyBSTR& operator=(const char * str);
	BOOL operator>(const CMyBSTR& str);
	BOOL operator<(const CMyBSTR& str);
	BOOL operator==(const CMyBSTR& str);
	BOOL operator!=(const CMyBSTR& str);

private:
	int CMyBSTR::Compare(const CMyBSTR& str);

};

#endif // !defined(AFX_MYBSTR_H__D0E49064_2358_11D2_B669_D4ED314D400F__INCLUDED_)
