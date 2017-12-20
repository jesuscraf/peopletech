//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              serial 
// File:                exeproxy.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                03-08-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module implements the proxy executor on server
//
//
//==============================================================================
 
#ifndef _EXE_PROXY_HPP
#define _EXE_PROXY_HPP

class ClChannel;
class ClStatus;
class ExecDumpInfo;
class ClConfiguration;
class M4DataStorage;
class ClProxyManager;

#include "m4oeinit_dll.hpp"


m4return_t M4_DECL_M4OEINIT ExecForProxy(M4DataStorage* const   ai_poInputDataStorage,
						M4DataStorage* const   ao_poOutputDataStorage,
						ClProxyManager* const  ai_poProxyManager, 
						ClChannel* const       ai_poSessionChannel,  
						m4uint32_t             ai_iSessionId,
						m4pcchar_t             ai_pcUserName, 
						ClStatus* const        ai_poClStatus,
						ExecDumpInfo* const    ai_poDumpInfo,
		                ClConfiguration* const ai_poConfiguration);

#endif
