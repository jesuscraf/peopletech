//## begin module%38AD2ABD0378.cm preserve=no
//## end module%38AD2ABD0378.cm

//## begin module%38AD2ABD0378.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%38AD2ABD0378.cp

//## Module: clientbasicdef%38AD2ABD0378; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: e:\source\m4appclient\inc\clientbasicdef.hpp

#ifndef clientbasicdef_h
#define clientbasicdef_h 1

//## begin module%38AD2ABD0378.additionalIncludes preserve=no
//## end module%38AD2ABD0378.additionalIncludes

//## begin module%38AD2ABD0378.includes preserve=yes
//## end module%38AD2ABD0378.includes

// m4types
#include <m4types.hpp>
//## begin module%38AD2ABD0378.declarations preserve=no
//## end module%38AD2ABD0378.declarations

//## begin module%38AD2ABD0378.additionalDeclarations preserve=yes
#include <m4srvres.hpp>
#include <m4trace.hpp>

//## end module%38AD2ABD0378.additionalDeclarations


//## begin ClTimeoutFunction%3659A483009A.preface preserve=yes
//## end ClTimeoutFunction%3659A483009A.preface

//## Class: ClTimeoutFunction%3659A483009A
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38B110D40379;m4int32_t { -> }

typedef m4int32_t(*ClTimeoutFunction)(m4int32_t);

//## begin ClTimeoutFunction%3659A483009A.postscript preserve=yes
#ifdef _DEBUG
	#define CRONOSTART		M4ClCrono ElDichoSoCronoMetro;  ElDichoSoCronoMetro.Reset();  ElDichoSoCronoMetro.Start();
	#define CRONOSTOP(x)	SETCODEF(M4_DEBUG_APICLIENT_TIME,DEBUGINFOLOG,"TIME:%s ha tardado %lf.",x,ElDichoSoCronoMetro.Stop());
	#define CRONORESET		ElDichoSoCronoMetro.Reset(); ElDichoSoCronoMetro.Start();
	#define CRONOPAUSE(x)   SETCODEF(M4_DEBUG_APICLIENT_TIME,DEBUGINFOLOG,"TIME:%s tiempo intermedio %lf.",x,ElDichoSoCronoMetro.Stop());  ElDichoSoCronoMetro.Start();					
#else
	#define CRONOSTART		;
	#define CRONOPAUSE(x)	;
	#define CRONOSTOP(x)	;
	#define CRONORESET		;
#endif

#define TIMEOUT_ADMIN_SERV  60000
#define M4_MAX_CHUNK_SIZE 1000000
//## end ClTimeoutFunction%3659A483009A.postscript

//## begin ClServerEventFunction%3769F5DF02EC.preface preserve=yes
//## end ClServerEventFunction%3769F5DF02EC.preface

//## Class: ClServerEventFunction%3769F5DF02EC
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef void(*ClServerEventFunction)(m4uint32_t,void *);

//## begin ClServerEventFunction%3769F5DF02EC.postscript preserve=yes
//## end ClServerEventFunction%3769F5DF02EC.postscript

//## begin module%38AD2ABD0378.epilog preserve=yes

#define M4_NULL_COMMS_TYPE					0
#define M4_CLASSIC_COMMS_TYPE			1
#define M4_RAW_COMMS_TYPE					2
#define M4_ADVANCED_COMMS_TYPE		3

//## end module%38AD2ABD0378.epilog


#endif
