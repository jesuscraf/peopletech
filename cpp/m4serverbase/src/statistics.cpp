//## begin module%36960B770298.cm preserve=no
//## end module%36960B770298.cm

//## begin module%36960B770298.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%36960B770298.cp

//## Module: Statistics%36960B770298; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: F:\integration\m4serverbase\version\src\statistics.cpp

//## begin module%36960B770298.additionalIncludes preserve=no
//## end module%36960B770298.additionalIncludes

//## begin module%36960B770298.includes preserve=yes
//## end module%36960B770298.includes

// StatUtil
#include <statutil.hpp>
// m4types
#include <m4types.hpp>
// m4date
#include <m4date.hpp>
// Statistics
#include <statistics.hpp>
// DataStorages
#include <datastorages.hpp>
//## begin module%36960B770298.declarations preserve=no
//## end module%36960B770298.declarations

//## begin module%36960B770298.additionalDeclarations preserve=yes
//## end module%36960B770298.additionalDeclarations


// Class ClVisitor 

ClVisitor::ClVisitor ()
  //## begin ClVisitor::ClVisitor%877616166.hasinit preserve=no
  //## end ClVisitor::ClVisitor%877616166.hasinit
  //## begin ClVisitor::ClVisitor%877616166.initialization preserve=yes
  //## end ClVisitor::ClVisitor%877616166.initialization
{
  //## begin ClVisitor::ClVisitor%877616166.body preserve=yes
  //## end ClVisitor::ClVisitor%877616166.body
}


ClVisitor::~ClVisitor ()
{
  //## begin ClVisitor::~ClVisitor%877616167.body preserve=yes
  //## end ClVisitor::~ClVisitor%877616167.body
}


// Class ClStatistic 





ClStatistic::ClStatistic ()
  //## begin ClStatistic::ClStatistic%879346819.hasinit preserve=no
  //## end ClStatistic::ClStatistic%879346819.hasinit
  //## begin ClStatistic::ClStatistic%879346819.initialization preserve=yes
  :	m_uiTypeID(0), m_uiNumberOfStats(TOTAL_NUM)
  //## end ClStatistic::ClStatistic%879346819.initialization
{
  //## begin ClStatistic::ClStatistic%879346819.body preserve=yes
  //## end ClStatistic::ClStatistic%879346819.body
}


ClStatistic::~ClStatistic ()
{
  //## begin ClStatistic::~ClStatistic%915791252.body preserve=yes
  //## end ClStatistic::~ClStatistic%915791252.body
}



//## Other Operations (implementation)
m4return_t ClStatistic::_CheckStatID (ClStatHandle &ai_hStatId, m4uint16_t ai_uiUniqueID)
{
  //## begin ClStatistic::_CheckStatID%886596297.body preserve=yes
	m4uint16_t		lowID, highID;

	highID = ai_hStatId.GetHighID();
	lowID = ai_hStatId.GetLowID();

	if (ai_uiUniqueID != highID)
		return M4_ERROR;

	if (m_uiNumberOfStats <= lowID)
		return M4_ERROR;

	return M4_SUCCESS;
  //## end ClStatistic::_CheckStatID%886596297.body
}

//## begin module%36960B770298.epilog preserve=yes
//## end module%36960B770298.epilog
