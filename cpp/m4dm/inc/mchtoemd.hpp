#ifndef _MCHTOEMD_HPP_
#define _MCHTOEMD_HPP_

//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                mchtoemd.hpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                28-07-1997
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
// 
//
//
//==============================================================================

#ifndef _M4CHCOMMON_HPP_
#include "m4chcommon.hpp"
#endif

class ClCompiledMCR;
class EMDStaticData;
class ClExtendedMetaData;

class ClMetaChannelToExtendedMetaData
{
public:
    m4return_t ConvertMetaChannelToExtendedMetaDataByScope(const ClCompiledMCR& ai_oMetaChannel,
                                                           const m4uint32_t     ai_iNodeHandle,
                                                           const m4uint8_t      ai_iScope,
                                                           EMDStaticData*&      ao_pStaticMetaData);
    
private:
    void AssignNumberOfItemsNotMethodByScope(const ClCompiledMCR& ai_oMetaChannel,
                                             const m4uint32_t     ai_iNodeHandle,
                                             const m4uint8_t      ai_iScope,
                                             EMDStaticData* const ai_pStaticMetaData);
};

#endif
