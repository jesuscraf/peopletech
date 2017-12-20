// clsOBL.h: interface for the clsOBL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLSOBL_H__3EE7BFD8_CCC8_41A3_AA2D_4D3E04382BD7__INCLUDED_)
#define AFX_CLSOBL_H__3EE7BFD8_CCC8_41A3_AA2D_4D3E04382BD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "clspattern.h"

class clsOBL : public clsPattern  
{
public:
	clsOBL();
	virtual ~clsOBL();

	void Parse (clsTextInfo * oTextInfo, long lStartLine, CString &sWorkText, VARIANT_BOOL * bPostProcess, long * lEndLine);
	CString GetColorTable();
	VARIANT_BOOL GetColorInfo(short Id, long *Color, long *DefaultColor, short *iKind, CString & sDesc);
	VARIANT_BOOL SetColorInfo(short Id, long Color);

private:
	CString * m_cKeyWords;
	int m_iNumKeyWords;
};

#endif // !defined(AFX_CLSOBL_H__3EE7BFD8_CCC8_41A3_AA2D_4D3E04382BD7__INCLUDED_)
