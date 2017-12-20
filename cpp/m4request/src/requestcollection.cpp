//## begin module%3950C4550012.cm preserve=no
//## end module%3950C4550012.cm

//## begin module%3950C4550012.cp preserve=no
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
//## end module%3950C4550012.cp

//## Module: RequestCollection%3950C4550012; Subprogram body
//## Subsystem: M4Request::src%379439BF01B7
//## Source file: F:\integration\m4request\src\requestcollection.cpp

//## begin module%3950C4550012.additionalIncludes preserve=no
//## end module%3950C4550012.additionalIncludes

//## begin module%3950C4550012.includes preserve=yes
#include <blocksynchronization.hpp>
//Rose98 Blody Include
#include <request.hpp>
//## end module%3950C4550012.includes

// RequestCollection
#include <requestcollection.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// garbagecontainer
#include <garbagecontainer.hpp>

class IGarbageable;

//## begin module%3950C4550012.declarations preserve=no
//## end module%3950C4550012.declarations

//## begin module%3950C4550012.additionalDeclarations preserve=yes
#include "tracerequest.hpp"
//## end module%3950C4550012.additionalDeclarations


// Class ClRequestCollector 



ClRequestCollector::ClRequestCollector (const m4string_t &ai_strFileName, m4int_t ai_iCollectorSize)
  //## begin ClRequestCollector::ClRequestCollector%961594594.hasinit preserve=no
  //## end ClRequestCollector::ClRequestCollector%961594594.hasinit
  //## begin ClRequestCollector::ClRequestCollector%961594594.initialization preserve=yes
  : ClGarbageContainer(ai_iCollectorSize)
  //## end ClRequestCollector::ClRequestCollector%961594594.initialization
{
  //## begin ClRequestCollector::ClRequestCollector%961594594.body preserve=yes
  //## end ClRequestCollector::ClRequestCollector%961594594.body
}


ClRequestCollector::~ClRequestCollector ()
{
  //## begin ClRequestCollector::~ClRequestCollector%961594595.body preserve=yes
//	Purge();
  //## end ClRequestCollector::~ClRequestCollector%961594595.body
}



//## Other Operations (implementation)
ClRequest * ClRequestCollector::CreateRequest (m4uint32_t ai_uiReqType, m4pchar_t ai_szServiceId, m4uint64_t ai_uiStartRecordTime, m4uint32_t ai_uiReqId, const m4string_t &ai_strDumpFile) const
{
  //## begin ClRequestCollector::CreateRequest%962640090.body preserve=yes
	ClRequestStatistics * poRequestStats = new ClRequestStatistics ( ai_uiStartRecordTime ) ;
	return new ClRequest(ai_uiReqType, ai_szServiceId, poRequestStats , ai_uiReqId, ai_strDumpFile);
  //## end ClRequestCollector::CreateRequest%962640090.body
}

// Additional Declarations
  //## begin ClRequestCollector%3950C36E03A0.declarations preserve=yes
m4uint32_t ClRequestCollector::Purge()
{
    ClTraceRequest tRequest("RequestCollector");
    m4uint32_t iPurgedElements = 0;

    tRequest.StartCount();
    iPurgedElements = ClGarbageContainer::Purge();
    tRequest.StopCount();

    if (iPurgedElements > 0)
    {
        // only dump if there was purged elements.
        char msg[256];
        sprintf (msg, "\tPurgedElements = %lu", iPurgedElements);
        tRequest.SetMsg(msg);
        tRequest.Trace();
    }

    return iPurgedElements;
}
  //## end ClRequestCollector%3950C36E03A0.declarations

//## begin module%3950C4550012.epilog preserve=yes
//## end module%3950C4550012.epilog
