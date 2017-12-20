// clsPattern.h: interface for the clsPattern class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLSPATTERN_H__1212C52D_FA8B_4153_8FBB_A03F2B861DA3__INCLUDED_)
#define AFX_CLSPATTERN_H__1212C52D_FA8B_4153_8FBB_A03F2B861DA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "clstextinfo.h"
#include "m4ptrtf32.h" // Toma el enumerado definido en la idl

class clsPattern  
{
public:
	clsPattern();
	virtual ~clsPattern();

	ParseType GetType();
	long get_LCID();
	void put_LCID(long newVal);
	virtual void Parse(clsTextInfo * oTextInfo, long lStartLine, CString & sWorkText, VARIANT_BOOL * bPostProcess, long * lEndLine);
	virtual CString GetColorTable();
	virtual VARIANT_BOOL GetColorInfo(short Id, long *Color, long *DefaultColor, short *iKind, CString & Description);
	virtual VARIANT_BOOL SetColorInfo(short Id, long Color);


protected:
	ParseType m_eType;
	long m_LCID;
	long m_lBaseResources;
};

#endif // !defined(AFX_CLSPATTERN_H__1212C52D_FA8B_4153_8FBB_A03F2B861DA3__INCLUDED_)
