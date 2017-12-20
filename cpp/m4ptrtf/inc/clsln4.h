// clsLN4.h: interface for the clsLN4 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLSLN4_H__4EB88668_047E_44D2_A11C_B9A23A67D100__INCLUDED_)
#define AFX_CLSLN4_H__4EB88668_047E_44D2_A11C_B9A23A67D100__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "clspattern.h"

class clsLN4 : public clsPattern  
{
public:
	clsLN4();
	virtual ~clsLN4();

	void Parse(clsTextInfo * oTextInfo, long lStartLine, CString & sWorkText, VARIANT_BOOL * bPostProcess, long * lEndLine);
	CString GetColorTable();
	VARIANT_BOOL GetColorInfo(short Id, long *Color, long *DefaultColor, short *iKind, CString & sDesc);
	VARIANT_BOOL SetColorInfo(short Id, long Color);

private:
	int m_iNumFunctions;
	CString * m_cFunctions;
	int m_iNumOperators;
	CString * m_cOperators;
	int m_iNumConstants;
	CString * m_cConstants;
	int m_iNumKeyWords;
	CString * m_cKeyWords;
	int m_iNumColors;
	long * m_patColor;
};

#endif // !defined(AFX_CLSLN4_H__4EB88668_047E_44D2_A11C_B9A23A67D100__INCLUDED_)
