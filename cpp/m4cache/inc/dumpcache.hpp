//## begin module.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             dumpcache.hpp
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

#include "m4types.hpp"
#include "m4stl.hpp"
#include "m4define.hpp"

#include "m4cache_dll.hpp"

//## end module.includes preserve=yes
#ifndef __DUMPCACHE_HPP__
#define __DUMPCACHE_HPP__

#if defined(_AIX) && defined(_XLC_R)
    class ClDumpCacheVisitor;
    ClDumpCacheVisitor& EndDumpCacheLn (ClDumpCacheVisitor& aio_Dump);
#endif

class M4_DECL_M4CACHE ClDumpCacheVisitor
{
public:

	ClDumpCacheVisitor(m4pchar_t ai_pcFileName);
	ClDumpCacheVisitor(ostream * ai_ofs);
	virtual ~ClDumpCacheVisitor();

	// Sobrecarga para tipos básicos - volcado
	ClDumpCacheVisitor &operator << (const m4double_t ai_d);
	ClDumpCacheVisitor &operator << (const m4uint32_t ai_ui);
	ClDumpCacheVisitor &operator << (const m4int32_t ai_i);
	ClDumpCacheVisitor &operator << (const m4int_t ai_i);
	ClDumpCacheVisitor &operator << (const m4pcchar_t ai_s);
	ClDumpCacheVisitor &operator << (const m4char_t ai_c);
	ClDumpCacheVisitor &operator << (const m4bool_t ai_b);
	ClDumpCacheVisitor &operator << (const m4float_t ai_f);

	ClDumpCacheVisitor& operator << (ClDumpCacheVisitor& (*_f)( ClDumpCacheVisitor& )) 
	{
//## begin ClDumpCacheVisitor::operator <<%1542319346.body preserve=yes
		return (*_f) (*this); 
//## end ClDumpCacheVisitor::operator <<%1542319346.body
	}
//## begin ClDumpCacheVisitor.initialDeclarations preserve=yes
public:

	friend M4_DECL_M4CACHE ClDumpCacheVisitor& EndDumpCacheLn (ClDumpCacheVisitor& aio_Dump);	

//## end ClDumpCacheVisitor.initialDeclarations
	m4bool_t DumpDates()
	{
//## begin ClDumpCacheVisitor::DumpDates%-978772967.body preserve=yes
		return m_bDumpDates;
	
//## end ClDumpCacheVisitor::DumpDates%-978772967.body
	}

	m4bool_t DumpDelQueue()
	{
//## begin ClDumpCacheVisitor::DumpDelQueue%793401871.body preserve=yes
		return m_bDumpDelQueue;
	
//## end ClDumpCacheVisitor::DumpDelQueue%793401871.body
	}

	m4bool_t DumpStatistics()
	{
//## begin ClDumpCacheVisitor::DumpStatistics%1928477096.body preserve=yes
		return m_bDumpStatistics;
	
//## end ClDumpCacheVisitor::DumpStatistics%1928477096.body
	}

	m4bool_t DumpSizes()
	{
//## begin ClDumpCacheVisitor::DumpSizes%1074595913.body preserve=yes
		return m_bDumpSizes;
	
//## end ClDumpCacheVisitor::DumpSizes%1074595913.body
	}

	m4bool_t DumpDirs()
	{
//## begin ClDumpCacheVisitor::DumpDirs%-1845757643.body preserve=yes
		return m_bDumpDirs;
	
//## end ClDumpCacheVisitor::DumpDirs%-1845757643.body
	}

	void SetDumpDates(m4bool_t ai_bDumpDates)
	{
//## begin ClDumpCacheVisitor::SetDumpDates%-1206273885.body preserve=yes
		m_bDumpDates = ai_bDumpDates;
	
//## end ClDumpCacheVisitor::SetDumpDates%-1206273885.body
	}

	void SetDumpDelQueue(m4bool_t ai_bDumpDelQueue)
	{
//## begin ClDumpCacheVisitor::SetDumpDelQueue%-2081521162.body preserve=yes
		m_bDumpDelQueue = ai_bDumpDelQueue;
	
//## end ClDumpCacheVisitor::SetDumpDelQueue%-2081521162.body
	}

	void SetDumpStatistics(m4bool_t ai_bDumpStatistics)
	{
//## begin ClDumpCacheVisitor::SetDumpStatistics%-613711058.body preserve=yes
		m_bDumpStatistics = ai_bDumpStatistics;
	
//## end ClDumpCacheVisitor::SetDumpStatistics%-613711058.body
	}

	void SetDumpSizes(m4bool_t ai_bDumpSizes)
	{
//## begin ClDumpCacheVisitor::SetDumpSizes%-62293340.body preserve=yes
		m_bDumpSizes = ai_bDumpSizes;
	
//## end ClDumpCacheVisitor::SetDumpSizes%-62293340.body
	}

	void SetDumpDirs(m4bool_t ai_bDumpDirs)
	{
//## begin ClDumpCacheVisitor::SetDumpDirs%801930597.body preserve=yes
		m_bDumpDirs = ai_bDumpDirs;
	
//## end ClDumpCacheVisitor::SetDumpDirs%801930597.body
	}

protected:
    m4bool_t m_bDumpDates;
	m4bool_t m_bDumpDelQueue;
	m4bool_t m_bDumpStatistics;
	m4bool_t m_bDumpSizes;
	m4bool_t m_bDumpDirs;

    ostream * m_ofs;
	m4bool_t   m_bLocalFile;
};


#endif 
