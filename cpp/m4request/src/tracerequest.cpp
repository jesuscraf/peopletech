
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Protocol
//	 File:              tracerequest.cpp
//	 Project:           m4request
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980521
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

#include "tracerequest.hpp"

ClTraceRequest::ClTraceRequest(m4pcchar_t ai_serviceId) :
    ClRequest(M4_TRACE_REQ_TYPE, (m4pchar_t)ai_serviceId, NULL)
{
    m4uint32_t iPurgedElements = 0;

    // creates the statictics object.
	m_poStatistic = m_poRequestStatistic = new ClRequestStatistics(ClPrecisionTicker::GetTimeTick());
    if (NULL != m_poRequestStatistic)
    {
        m_poRequestStatistic->Sett(ClRequestStatistics::InService);
    }
        
    /*
    // activate cpu counter and purge.
    iPurgedElements = ClGarbageContainer::Purge();
    poRequestStatistic->StopCPUCount(M4Thread::GetCurrentIdThread());
    
    // dump.
    if (iPurgedElements <= 0)
    {
        // only dump if there was purged elements.
        delete poRequest;
    }
    else
    {
        // update statistics.
        poRequestStatistic->Sett(ClRequestStatistics::Deregistering);
        poRequestStatistic->SettFinish();

        // set request trace.
        char buffer[256];
        sprintf (buffer, "\tPurgedElements = %lu", iPurgedElements);
        ClMessageContainer* poMsgContainer = poRequest->GetMsgContainer();
        poMsgContainer->StoreTraceMessage(buffer);

        // dump statistics and delete object.
        poRequest->Destroy();
    }
    
    return iPurgedElements;
    */
}


ClTraceRequest::~ClTraceRequest()
{
}

void ClTraceRequest::StartCount()
{
    if (NULL == m_poRequestStatistic)
    {
        return;
    }

    m_poRequestStatistic->StartCPUCount((void*)M4Thread::GetCurrentIdThread());
}

void ClTraceRequest::StopCount()
{
    if (NULL == m_poRequestStatistic)
    {
        return;
    }

    m_poRequestStatistic->StopCPUCount(M4Thread::GetCurrentIdThread());
    m_poRequestStatistic->Sett(ClRequestStatistics::Deregistering);
    m_poRequestStatistic->SettFinish();
}

void ClTraceRequest::Trace()
{
    if (NULL != m_poRequestStatistic)
    {
        _Dump();
    }
}

void ClTraceRequest::SetMsg(m4pcchar_t ai_msg)
{
    // set request trace.
    m4string_t msg = "\t";
    msg.append(ai_msg);
    m_poMsgContainer->StoreTraceMessage(msg);
}


