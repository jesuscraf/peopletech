//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              serial 
// File:                exechannel.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                02-10-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================

#ifndef _EXE_CHANNEL_HPP
#define _EXE_CHANNEL_HPP

#include "m4oeinit_dll.hpp"
#include "m4define.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
#include "clconfiguration.hpp"
#include "execdumpinfo.hpp"


class ClChannelManager;
class ClChannel;
class ClStatus;
class M4DataStorage;
class ClCMCRFactory;
class ClPresentationFactory;
class ClCacheDirectory;
class ClLConn_Provider;
class IObjDataCache;


M4_DECL_M4OEINIT ostream & operator<< (ostream &ai_ofs, ExecDumpInfo &ai_DumpInfo);

m4return_t M4_DECL_M4OEINIT ExecChannel(ClChannelManager* poChannelManager,
									M4DataStorage*        ai_poInputDataStorage,
									M4DataStorage*        ao_poOutputDataStorage,
									ClStatus*             ai_poClStatus,
									ExecDumpInfo*	      aio_poDumpInfo,
									ClConfiguration*      ai_poConfiguration,
									ClLConn_Provider*     ai_lConnProvider,
									IObjDataCache*		  ai_pIObjDataCache); 

m4return_t M4_DECL_M4OEINIT ExecMetaData(ClCMCRFactory			* ai_poCMCRFact, 
										 ClPresentationFactory	* ai_poPresFac, 
										 ClCacheDirectory		* ai_poCacheDirectory,
										 ClChannelManager		*ai_poChannelManager,
										 M4DataStorage			*ai_poInputDataStorage,
										 M4DataStorage			*ao_poOutputDataStorage,
										 void					*poExecParam,
										 ExecDumpInfo		    *aio_poDumpInfo);

#endif
