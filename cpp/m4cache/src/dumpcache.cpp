//## begin module.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             dumpcache.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             10/10/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Memory DumpCache
//
//
//==============================================================================

#include "dumpcache.hpp"
#include "m4unicode.hpp"

//## end module.includes preserve=yes
ClDumpCacheVisitor::ClDumpCacheVisitor(m4pchar_t ai_pcFileName)
{
//## begin ClDumpCacheVisitor::ClDumpCacheVisitor%572710952.body preserve=yes
	m_ofs = new ofstream();
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;

	M4Open( *(ofstream*)m_ofs, ai_pcFileName, M4UniWrite, eEncoding ) ;

    m_bDumpDates = M4_TRUE;
	m_bDumpDelQueue = M4_TRUE;
	m_bDumpStatistics = M4_TRUE;
	m_bDumpSizes = M4_TRUE;
	m_bDumpDirs = M4_TRUE;

	m_bLocalFile = M4_TRUE;
//## end ClDumpCacheVisitor::ClDumpCacheVisitor%572710952.body
}

ClDumpCacheVisitor::ClDumpCacheVisitor(ostream * ai_ofs)
{
//## begin ClDumpCacheVisitor::ClDumpCacheVisitor%-1497634471.body preserve=yes
	m_ofs = ai_ofs;

    m_bDumpDates = M4_TRUE;
	m_bDumpDelQueue = M4_TRUE;
	m_bDumpStatistics = M4_TRUE;
	m_bDumpSizes = M4_TRUE;
	m_bDumpDirs = M4_TRUE;

	m_bLocalFile = M4_FALSE;
//## end ClDumpCacheVisitor::ClDumpCacheVisitor%-1497634471.body
}

ClDumpCacheVisitor::~ClDumpCacheVisitor()
{
//## begin ClDumpCacheVisitor::~ClDumpCacheVisitor%1951092174.body preserve=yes
	if (m_bLocalFile == M4_TRUE)
	{
		((ofstream*)m_ofs)->close();
		delete m_ofs;
	}
//## end ClDumpCacheVisitor::~ClDumpCacheVisitor%1951092174.body
}


ClDumpCacheVisitor& ClDumpCacheVisitor::operator << (const m4double_t ai_d)
{
//## begin ClDumpCacheVisitor::operator <<%1730848809.body preserve=yes
	*m_ofs<<ai_d;
	return *this;
//## end ClDumpCacheVisitor::operator <<%1730848809.body
}

ClDumpCacheVisitor& ClDumpCacheVisitor::operator << (const m4uint32_t ai_ui)
{
//## begin ClDumpCacheVisitor::operator <<%-563878121.body preserve=yes
	*m_ofs<<ai_ui;
	return *this;
//## end ClDumpCacheVisitor::operator <<%-563878121.body
}

ClDumpCacheVisitor& ClDumpCacheVisitor::operator << (const m4int32_t ai_i)
{
//## begin ClDumpCacheVisitor::operator <<%1301076335.body preserve=yes
	*m_ofs<<ai_i;
	return *this;
//## end ClDumpCacheVisitor::operator <<%1301076335.body
}

ClDumpCacheVisitor& ClDumpCacheVisitor::operator << (const m4int_t ai_i)
{
//## begin ClDumpCacheVisitor::operator <<%-2006379168.body preserve=yes
	*m_ofs<<ai_i;
	return *this;
//## end ClDumpCacheVisitor::operator <<%-2006379168.body
}

ClDumpCacheVisitor& ClDumpCacheVisitor::operator << (const m4pcchar_t ai_s)
{
//## begin ClDumpCacheVisitor::operator <<%1269028911.body preserve=yes
	*m_ofs<<ai_s;
	return *this;
//## end ClDumpCacheVisitor::operator <<%1269028911.body
}

ClDumpCacheVisitor& ClDumpCacheVisitor::operator << (const m4char_t ai_c)
{
//## begin ClDumpCacheVisitor::operator <<%1653989012.body preserve=yes
	*m_ofs<<ai_c;
	return *this;
//## end ClDumpCacheVisitor::operator <<%1653989012.body
}

ClDumpCacheVisitor& ClDumpCacheVisitor::operator << (const m4bool_t ai_b)
{
//## begin ClDumpCacheVisitor::operator <<%-2029523331.body preserve=yes
	*m_ofs<<ai_b;
	return *this;
//## end ClDumpCacheVisitor::operator <<%-2029523331.body
}

ClDumpCacheVisitor& ClDumpCacheVisitor::operator << (const m4float_t ai_f)
{
//## begin ClDumpCacheVisitor::operator <<%-445808185.body preserve=yes
	*m_ofs<<ai_f;
	return *this;
//## end ClDumpCacheVisitor::operator <<%-445808185.body
}
//## begin module.epilog preserve=yes

ClDumpCacheVisitor& EndDumpCacheLn (ClDumpCacheVisitor& aio_Dump)
{
	*(aio_Dump.m_ofs)<<endl;
	return aio_Dump;
}
//## end module.epilog
