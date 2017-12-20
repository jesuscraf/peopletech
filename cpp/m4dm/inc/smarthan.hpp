//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             SmartHandle.hpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             5/05/98
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    Smart Pointer for Handles
//
//==============================================================================

#ifndef __SMARTHAN_HPP__
#define __SMARTHAN_HPP__
#include "m4dm_dll.hpp"
#include "m4var.hpp"
#include "smartp.hpp"

class ClChannel;
class ClBSearchIndex;

class M4_DECL_M4DM ClSmartHandle : public _ClSmartPointer
{
public:

    ClSmartHandle(m4int32_t ai_iHandle, ClChannel * ai_pClChannel);
    ClSmartHandle(ClChannel * ai_pClChannel);

    ClSmartHandle * Attach(void);
    m4return_t Detach(void);

    m4return_t SetHandle(m4VariantType ai_vNewHandle);
    m4return_t GetHandle(m4VariantType & ao_vNewHandle);

	m4return_t GetSize(ClSize &ao_size);

    ClChannel * m_pClChannel;

protected:

    m4VariantType  m_vHandle;
    m4bool_t m_bOwner; //me dice si el Handle es mio o no, vale cuando me pasan el handle.
};

class M4_DECL_M4DM ClSmartSearchIndex : public _ClSmartPointer
{
public:

    ClSmartSearchIndex(ClBSearchIndex * ai_pBSearchIndex);
    ClSmartSearchIndex();

    ClSmartSearchIndex * Attach(void);
    m4return_t Detach(void);

    m4return_t SetIndex(ClBSearchIndex * ai_pBSearchIndex);
    ClBSearchIndex * GetIndex(void);

	m4return_t GetSize(ClSize &ao_size);

protected:

    ClBSearchIndex * m_pBSearchIndex;
    m4bool_t m_bOwner; //me dice si el searchindex es mio o no
                       //depende de donde provenga el inidice.
};

#endif // __SMARTHAN_HPP__
