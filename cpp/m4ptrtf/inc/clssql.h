// clsSQL.h: interface for the clsSQL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLSSQL_H__90371977_8C3D_43E9_9707_C7820F310BD7__INCLUDED_)
#define AFX_CLSSQL_H__90371977_8C3D_43E9_9707_C7820F310BD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "clspattern.h"

class clsSQL : public clsPattern  
{
public:
	clsSQL();
	virtual ~clsSQL();

	void Parse (clsTextInfo * oTextInfo, long lStartLine, CString &sWorkText, VARIANT_BOOL * bPostProcess, long * lEndLine);
	CString GetColorTable();
	VARIANT_BOOL GetColorInfo(short Id, long *Color, long *DefaultColor, short *iKind, CString & sDesc);
	VARIANT_BOOL SetColorInfo(short Id, long Color);

private:
	CString * m_cFunctions;
	int m_iNumFunctions;
	CString * m_cKeyWords;
	int m_iNumKeyWords;
};

#endif // !defined(AFX_CLSSQL_H__90371977_8C3D_43E9_9707_C7820F310BD7__INCLUDED_)
