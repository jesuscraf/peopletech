
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Protocol
//	 File:              tracerequest.hpp
//	 Project:           m4request
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980521
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	 Esta clase se utiliza para definir unas request que únicamente tienen como
//   objetivo volcar una línea de traza en el fichero benchmark.txt.
//
//   Lo utilizamos para volcar trazas de garbageColector, synchronize, etc.
//
//
//	==============================================================================

#ifndef tracerequest_h
#define tracerequest_h 1

#include <m4request_dll.hpp>
#include <request.hpp>


class M4_DECL_M4REQUEST ClTraceRequest : public ClRequest
{
    public:
        ClTraceRequest(m4pcchar_t ai_serviceId);
        ~ClTraceRequest();

        void StartCount();
        void StopCount();
        void Trace();
        void SetMsg(m4pcchar_t ai_msg);

    protected:
        ClRequestStatistics* m_poRequestStatistic;

};

#endif
