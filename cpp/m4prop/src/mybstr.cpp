// MyBSTR.cpp: implementation of the CMyBSTR class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "mybstr.h"
#include "m4unicode.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyBSTR::CMyBSTR()
{
	m_BSTRData = ::SysAllocString(L"");
}

CMyBSTR::~CMyBSTR()
{
	::SysFreeString(m_BSTRData);
}

void CMyBSTR::Attach(BSTR newValue)
{
	::SysFreeString(m_BSTRData);
	m_BSTRData = ::SysAllocString(newValue);
}

BOOL CMyBSTR::operator!=(const CMyBSTR& str)
{
	return Compare(str) != 0;
}

BOOL CMyBSTR::operator==(const CMyBSTR& str)
{
	return Compare(str) == 0;
}

BOOL CMyBSTR::operator>(const CMyBSTR& str)
{
	return Compare(str) > 0;
}

BOOL CMyBSTR::operator<(const CMyBSTR& str)
{
	return Compare(str) < 0;
}

int CMyBSTR::Compare(const CMyBSTR& str)
{
	if (m_BSTRData == str.m_BSTRData) {
		return 0;
	}

	if (m_BSTRData == NULL) {
		return -1;
	}

	if (str.m_BSTRData == NULL) {
		return 1;
	}

	const unsigned int l1 = ::SysStringLen(m_BSTRData);
	const unsigned int l2 = ::SysStringLen(str.m_BSTRData);

	unsigned int len = l1;
	if (len > l2) {
		len = l2;
	}

	BSTR bstr1 = m_BSTRData;
	BSTR bstr2 = str.m_BSTRData;

	while (len-- > 0) {
		if (*bstr1++ != *bstr2++) {
			return bstr1[-1] - bstr2[-1];
		}
	}

	return (l1 < l2) ? -1 : (l1 == l2) ? 0 : 1;
}

CMyBSTR& CMyBSTR::operator=(const BSTR str)
{
	::SysFreeString(m_BSTRData);
	m_BSTRData = ::SysAllocString(str);

	return *this;
}

CMyBSTR& CMyBSTR::operator=(const CMyBSTR &str)
{
	::SysFreeString(m_BSTRData);
	m_BSTRData = ::SysAllocString(str.m_BSTRData);
	return *this;
}


CMyBSTR& CMyBSTR::operator=(const char * str)
{
	::SysFreeString(m_BSTRData);
	m_BSTRData = M4CppToVB(str);

	return *this;
}