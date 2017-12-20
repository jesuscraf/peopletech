// clsPattern.cpp: implementation of the clsPattern class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "clspattern.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

clsPattern::clsPattern()
{
	m_eType = M4PT_DISABLED;
}

clsPattern::~clsPattern()
{

}

CString clsPattern::GetColorTable()
{
	return ("");
}

void clsPattern::Parse(clsTextInfo * oTextInfo, long lStartLine, CString & sWorkText, VARIANT_BOOL * bPostProcess, long * lEndLine)
{
}

ParseType clsPattern::GetType()
{
	return m_eType;
}

VARIANT_BOOL clsPattern::GetColorInfo(short Id, long *Color, long *DefaultColor, short *iKind, CString & Description)
{
	return FALSE;
}

VARIANT_BOOL clsPattern::SetColorInfo(short Id, long Color)
{
	return FALSE;
}

long clsPattern::get_LCID()
{
	return m_LCID;
}

void clsPattern::put_LCID(long newVal)
{
	m_LCID = newVal;
	switch (newVal)
	{
		case 0x409:
			m_lBaseResources = 1000;    // English
			break;
		case 0xC0A:
			m_lBaseResources = 2000;	// Spanish
			break;
		case 0x816:
			m_lBaseResources = 3000;	// Portuguese
			break;
		case 0x40C:
			m_lBaseResources = 4000;	// French
			break;
		case 0x410:
			m_lBaseResources = 5000;	// Italian
			break;
		case 0x416:
			m_lBaseResources = 6000;	// Brazilian
			break;
		default:
			m_lBaseResources = 1000;
			m_LCID = 0x409;			// Default (English)
	}
}
