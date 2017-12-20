// clsLineInfo.cpp: implementation of the clsLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "clslineinfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

clsLineInfo::clsLineInfo()
{
	m_bUndefined = true;
	m_lLineStatus = 0;
	m_iSelectionMark = 0;
	m_iBreakPointMark = 0;
}

clsLineInfo::~clsLineInfo()
{

}
