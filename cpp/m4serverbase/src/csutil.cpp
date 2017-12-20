//## begin module%341FFCEA0143.cm preserve=no
//## end module%341FFCEA0143.cm

//## begin module%341FFCEA0143.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:			Utilities
//	 File:				CSUtil.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				971020
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%341FFCEA0143.cp

//## Module: CSUtil%341FFCEA0143; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: d:\eduardoma\m4serverbase\src\csutil.cpp

//## begin module%341FFCEA0143.additionalIncludes preserve=no
//## end module%341FFCEA0143.additionalIncludes

//## begin module%341FFCEA0143.includes preserve=yes
//## end module%341FFCEA0143.includes

// m4types
#include <m4types.hpp>
// m4thread
#include <m4thread.hpp>
// CSUtil
#include <csutil.hpp>
// Statistics
#include <statistics.hpp>


//## begin module%341FFCEA0143.declarations preserve=no
//## end module%341FFCEA0143.declarations

//## begin module%341FFCEA0143.additionalDeclarations preserve=yes
//## end module%341FFCEA0143.additionalDeclarations


// Class ClActiveClass 




ClActiveClass::ClActiveClass (ClStatistic *ai_pStats)
  //## begin ClActiveClass::ClActiveClass%875808403.hasinit preserve=no
  //## end ClActiveClass::ClActiveClass%875808403.hasinit
  //## begin ClActiveClass::ClActiveClass%875808403.initialization preserve=yes
  : m_pStats(ai_pStats), m_pVisitor(NULL)
  //## end ClActiveClass::ClActiveClass%875808403.initialization
{
  //## begin ClActiveClass::ClActiveClass%875808403.body preserve=yes
	sprintf(m_pszName, "UNKNOWN THREAD: Base Address %p", this);
  //## end ClActiveClass::ClActiveClass%875808403.body
}


ClActiveClass::~ClActiveClass ()
{
  //## begin ClActiveClass::~ClActiveClass%875808404.body preserve=yes
  //## end ClActiveClass::~ClActiveClass%875808404.body
}



//## Other Operations (implementation)
ClStatistic * ClActiveClass::GetStats ()
{
  //## begin ClActiveClass::GetStats%879346830.body preserve=yes
	return m_pStats;
  //## end ClActiveClass::GetStats%879346830.body
}

ClStatistic * ClActiveClass::SetStats (ClStatistic *ai_pStats)
{
  //## begin ClActiveClass::SetStats%893927989.body preserve=yes
	ClStatistic *auxPointer;

	auxPointer = m_pStats;
	m_pStats = ai_pStats;

	return auxPointer;
  //## end ClActiveClass::SetStats%893927989.body
}

m4int16_t ClActiveClass::Start (m4pchar_t ai_pszName)
{
  //## begin ClActiveClass::Start%901194407.body preserve=yes
	if (NULL != ai_pszName)
	{
		strncpy(m_pszName, ai_pszName, 63);
		m_pszName[63]='\0';
	}

	return M4Thread::Start((m4pchar_t)m_pszName);
  //## end ClActiveClass::Start%901194407.body
}

// Additional Declarations
  //## begin ClActiveClass%341FFAA60059.declarations preserve=yes
  //## end ClActiveClass%341FFAA60059.declarations

// Class ClActiveClassVisitor 



ClActiveClassVisitor::ClActiveClassVisitor ()
  //## begin ClActiveClassVisitor::ClActiveClassVisitor%884712762.hasinit preserve=no
      : m_nLastProcessedItems(0)
  //## end ClActiveClassVisitor::ClActiveClassVisitor%884712762.hasinit
  //## begin ClActiveClassVisitor::ClActiveClassVisitor%884712762.initialization preserve=yes
  , ClVisitor()
  //## end ClActiveClassVisitor::ClActiveClassVisitor%884712762.initialization
{
  //## begin ClActiveClassVisitor::ClActiveClassVisitor%884712762.body preserve=yes
  //## end ClActiveClassVisitor::ClActiveClassVisitor%884712762.body
}



//## Other Operations (implementation)
void ClActiveClassVisitor::VisitActiveClass (ClActiveClass &ai_oActiveClass)
{
  //## begin ClActiveClassVisitor::VisitActiveClass%884712763.body preserve=yes
	//	StartTime
	//	UserCPUTime
	//	SystemCPUTime
	//	WaitTime
	//	ReadyTime
	//	IOOperations
	//	PageFaults
	//	RTProcess
	//	RunPRiority
	ClStatNumericalType	newValue, swapValue;
	ClStatAverageType	avgValue;

	ClActiveClassStats	*pStats = (ClActiveClassStats	*)ai_oActiveClass.GetStats();

	swapValue = m_nLastProcessedItems;
	newValue = pStats->GetnProcessedItems();
	m_nLastProcessedItems = newValue;
	newValue -= swapValue;

	avgValue = (ClStatAverageType)newValue;
	avgValue /= 5;

	pStats->AddAvgProcessedItems(avgValue);
  //## end ClActiveClassVisitor::VisitActiveClass%884712763.body
}

// Additional Declarations
  //## begin ClActiveClassVisitor%34BBA64D0212.declarations preserve=yes
  //## end ClActiveClassVisitor%34BBA64D0212.declarations

// Class ClActiveClassStats 



ClActiveClassStats::ClActiveClassStats ()
  //## begin ClActiveClassStats::ClActiveClassStats%884336013.hasinit preserve=no
      : m_nProcessedItems(0)
  //## end ClActiveClassStats::ClActiveClassStats%884336013.hasinit
  //## begin ClActiveClassStats::ClActiveClassStats%884336013.initialization preserve=yes
	, 	m_avgProcessedItems(12, 180)
  //## end ClActiveClassStats::ClActiveClassStats%884336013.initialization
{
  //## begin ClActiveClassStats::ClActiveClassStats%884336013.body preserve=yes
  //## end ClActiveClassStats::ClActiveClassStats%884336013.body
}


ClActiveClassStats::~ClActiveClassStats ()
{
  //## begin ClActiveClassStats::~ClActiveClassStats%884336014.body preserve=yes
  //## end ClActiveClassStats::~ClActiveClassStats%884336014.body
}



//## Other Operations (implementation)
ClStatAverageType ClActiveClassStats::GetAvgProcessedItems (m4uint16_t ai_uiPeriod)
{
  //## begin ClActiveClassStats::GetAvgProcessedItems%884336011.body preserve=yes
	ClStatAverageType	res;

	switch(ai_uiPeriod)
	{
	case 0:		//	60 secs. => 12
		res = m_avgProcessedItems.GetAvg(12);
		break;
	case 1:		//	300 secs. => 60
		res = m_avgProcessedItems.GetAvg(60);
		break;
	case 2:		//	900 secs. => 180
		res = m_avgProcessedItems.GetAvg();
		break;
	default:
		res = (ClStatAverageType)-1;
	}

	return res;
  //## end ClActiveClassStats::GetAvgProcessedItems%884336011.body
}

void ClActiveClassStats::AddAvgProcessedItems (ClStatAverageType &ai_avgValue)
{
  //## begin ClActiveClassStats::AddAvgProcessedItems%884336012.body preserve=yes
	ClStatAverageType	avgValue;

	avgValue = (ClStatAverageType)ai_avgValue;
	m_avgProcessedItems.Update(avgValue);
  //## end ClActiveClassStats::AddAvgProcessedItems%884336012.body
}

m4return_t ClActiveClassStats::Reset ()
{
  //## begin ClActiveClassStats::Reset%879346831.body preserve=yes
	m_nProcessedItems = 0;
	m_avgProcessedItems.Reset();

	return M4_SUCCESS;
  //## end ClActiveClassStats::Reset%879346831.body
}

m4return_t ClActiveClassStats::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData)
{
  //## begin ClActiveClassStats::GetStat%886502776.body preserve=yes
	eActiveClassStatsID		statIndex;
	m4return_t				res = M4_TRUE;

	statIndex = (eActiveClassStatsID)ai_hStatId.GetLowID();

	if (0 < ao_poData.GetSize())
		return M4_ERROR;

	ao_poData.SetSize(DEF_STRING_SIZE);
	m4pchar_t	buffer = ao_poData.GetBuffer();
//
/*	enum {
				PROC_ITEMS_ID,
				TOTAL_NUM
			} */

	switch(statIndex)
	{
	case PROC_ITEMS_ID:
		sprintf(buffer, "%010d", m_nProcessedItems);
		break;
	case AVG_PROC_ITEMS_ID:
		sprintf(buffer, "%010.2f", m_avgProcessedItems.GetAvg(0));
		break;
	default:
		sprintf(buffer, "%010d", 0);
		res = M4_FALSE;
	}

	return res;
  //## end ClActiveClassStats::GetStat%886502776.body
}

m4return_t ClActiveClassStats::ResetStat (ClStatHandle &ai_hStatId)
{
  //## begin ClActiveClassStats::ResetStat%886502778.body preserve=yes
	eActiveClassStatsID		statIndex;
	m4return_t				res = M4_TRUE;

	statIndex = (eActiveClassStatsID)ai_hStatId.GetLowID();

//
/*	enum {
				PROC_ITEMS_ID,
				AVG_PROC_ITEMS_ID,
				TOTAL_NUM
			} */

	switch(statIndex)
	{
	case PROC_ITEMS_ID:
		m_nProcessedItems = 0;
		break;
	case AVG_PROC_ITEMS_ID:
		m_avgProcessedItems.Reset();
		break;
	default:
		res = M4_FALSE;
	}

	return res;
  //## end ClActiveClassStats::ResetStat%886502778.body
}

// Additional Declarations
  //## begin ClActiveClassStats%346221120006.declarations preserve=yes
  //## end ClActiveClassStats%346221120006.declarations

//## begin module%341FFCEA0143.epilog preserve=yes
//## end module%341FFCEA0143.epilog
