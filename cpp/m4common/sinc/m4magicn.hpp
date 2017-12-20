//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              common_channeñ
// File:                magicnum.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                31-07-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the magic number for the mind persistiable objects:
//			- CMCR --> Metachannels (serialized in clcmcrse.cpp)
//			- CSCR --> Security (serialized in clcmcrse.cpp)
//			- PRES --> Presentations (serialized in clpresen.cpp)
//			- DATA --> Channel Data (serialized in chandata_ser.cpp)
//			- CAC  --> Cache (serialized in clcache.cpp)
//			- VER  --> CacheVersion (serialized in cachedir.cpp)
//
//==============================================================================

#ifndef __MAGICNUM_HPP
#define __MAGICNUM_HPP

#define M4_CH_MAGIC_NUMBER_CMCR							0x6E4303A1
#define M4_CH_MAGIC_NUMBER_CSCR							0xD470DD47
#define M4_CH_MAGIC_NUMBER_PRES							0x90C16240
#define M4_CH_MAGIC_NUMBER_DATA							0x35382913
#define M4_CH_MAGIC_NUMBER_CAC							0xB3D5EC52
#define M4_CH_MAGIC_NUMBER_VER							0x30A2F489

#endif // __MAGICNUM_HPP
