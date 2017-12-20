//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              serial 
// File:                exem4xdm.hpp
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
//    This module is the executor of XML in the server
//
//
//==============================================================================

m4return_t ExecM4XML(ClCMInitParameters			&aio_oCMInitParameters,
					 SAXParser*					ai_poParser,
					 M4DataStorage*				ai_poInputDataStorage,
					 M4DataStorage*				ao_poOutputDataStorage,
					 ExecDumpInfo *				aio_poDumpInfo,
					 ClM4XMLInfo*				ai_poInfo);

