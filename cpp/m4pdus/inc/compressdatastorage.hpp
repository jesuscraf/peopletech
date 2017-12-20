//	==============================================================================
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
//	==============================================================================

#ifndef _COMPRESSDATASTORAGE
#define _COMPRESSDATASTORAGE 1
#include <m4types.hpp>
#include <datastorages.hpp>
#include <m4trace.hpp>
#include "m4stl.hpp"
#include "m4pdus_dll.hpp"

#define M4_ZIP_EXTENSION "_m4zip"
#define  M4_COMPRESS_FILE_BUFFER_SIZE 16384

class ClCompress;

m4return_t M4_DECL_M4PDUS M4DSCompress ( M4DataStorage & ao_CompressDataStorage  , 
										  M4DataStorage & ai_DeCompressDataStorage ,
										  m4pchar_t ai_pcCompressType,
										  m4uint32_t ai_uiCompressionType,
										  m4uint32_t ai_uiCompressionLevel);

m4return_t M4_DECL_M4PDUS M4DSDecompress ( M4DataStorage & ai_CompressDataStorage  , 
											M4DataStorage & ao_DeCompressDataStorage,
											m4pchar_t ai_pcCompressType,
											m4uint32_t ai_uiCompressionType) ;

m4return_t M4_DECL_M4PDUS M4Compress ( M4DataStorage & ao_CompressDataStorage  , 
										M4DataStorage & ai_DeCompressDataStorage ,
										ClCompress  * ai_poCompress,
										m4uint32_t ai_uiCompressionType);

m4return_t M4_DECL_M4PDUS M4Decompress ( M4DataStorage & ai_CompressDataStorage  , 
										  M4DataStorage & ao_DeCompressDataStorage,
										  ClCompress  * ai_poCompress ,
										  m4uint32_t ai_uiCompressionType) ;


#endif // _COMPRESSDATASTORAGE

