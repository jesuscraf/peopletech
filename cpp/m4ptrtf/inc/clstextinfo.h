// clsTextInfo.h: interface for the clsTextInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLSTEXTINFO_H__CED308C4_8DEF_40B1_8D1E_A900B3F55378__INCLUDED_)
#define AFX_CLSTEXTINFO_H__CED308C4_8DEF_40B1_8D1E_A900B3F55378__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "clslineinfo.h"

class clsTextInfo  
{
public:
	clsTextInfo();
	virtual ~clsTextInfo();

	void Clear();

	void AdjustStatus(long lStartLine, long lOldNumLines, long lNewNumLines);

	long get_ActiveLine()
	{
		return m_lActiveLine;
	}
	void put_ActiveLine(long lValue)
	{
		m_lActiveLine = lValue;
	}

	clsLineInfo * LineInfo(long lLine);
	long NextSelectionLine(long Line);
	long NextBreakPointLine(long Line);


protected:
	void TestLine(long lLine);

	long m_lActiveLine;
	long m_lNumLinesInfo;
	long m_lNumLinesAllocated;
	clsLineInfo ** m_poLinesInfo;

};

#endif // !defined(AFX_CLSTEXTINFO_H__CED308C4_8DEF_40B1_8D1E_A900B3F55378__INCLUDED_)
