//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            M4ClientAPI
//	 File:              http_errors.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980313
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
//## end module%34E313F800E7.cp

//## Module: clientapi%34E313F800E7; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: e:\source\M4AppClient\version\inc\http_errors.hpp


#ifndef http_errors_hpp
#define http_errors_hpp




#define M4_ERROR_HTTP_BASE  0x10

#define M4_ERROR_HTTP_PARSE_ERROR (M4_ERROR_HTTP_BASE+0x00)

#define M4_ERROR_HTTP_301 (M4_ERROR_HTTP_BASE+0x01)
#define M4_ERROR_HTTP_302 (M4_ERROR_HTTP_BASE+0x02)
#define M4_ERROR_HTTP_303 (M4_ERROR_HTTP_BASE+0x03)
#define M4_ERROR_HTTP_304 (M4_ERROR_HTTP_BASE+0x04)
#define M4_ERROR_HTTP_305 (M4_ERROR_HTTP_BASE+0x05)
#define M4_ERROR_HTTP_306 (M4_ERROR_HTTP_BASE+0x06)
#define M4_ERROR_HTTP_307 (M4_ERROR_HTTP_BASE+0x07)
#define M4_ERROR_HTTP_308 (M4_ERROR_HTTP_BASE+0x08)
#define M4_ERROR_HTTP_309 (M4_ERROR_HTTP_BASE+0x09)
#define M4_ERROR_HTTP_310 (M4_ERROR_HTTP_BASE+0x0a)
#define M4_ERROR_HTTP_311 (M4_ERROR_HTTP_BASE+0x0b)
#define M4_ERROR_HTTP_312 (M4_ERROR_HTTP_BASE+0x0c)
#define M4_ERROR_HTTP_BUFFER	(2*M4_ERROR_HTTP_BASE+0x00)
#define M4_ERROR_HTTP_NO_COOKIE (2*M4_ERROR_HTTP_BASE+0x01)
#define M4_ERROR_HTTP_NO_M4KEY	(2*M4_ERROR_HTTP_BASE+0x02)
#define M4_CANCELING_HTTP_REQUEST (3*M4_ERROR_HTTP_BASE+0x00)

#endif //http_errors_hpp

