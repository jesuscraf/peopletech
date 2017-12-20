//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             mmarray.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             29/04/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    MMIndex Array for RAM Indexes
//
//
//==============================================================================

#ifndef __MMARR_HPP__
#define __MMARR_HPP__

#include "m4types.hpp"
#include "mmindex.hpp"


class _ClRegisterFilter;
class ClAccessProvider;
class ClBlock_Channel_Proxy;
class ClBlockReference;
class ClSize;


class ClMMArray
{
public:

    ClMMArray(void);
   ~ClMMArray(void);

    m4return_t  Get (m4uint32_t ai_iIndex, ClMMIndex &aio_MMIndex);
    m4return_t  Set (m4uint32_t ai_iIndex, ClMMIndex ai_MMIndex);

    m4return_t  Copy(ClMMArray * ai_poMMArray);

    m4return_t  Set (m4uint32_t * ai_pIndexArray, m4uint32_t ai_number, const ClBlockReference &ai_br, ClAccessProvider *ai_pap, _ClRegisterFilter * ai_pClSubFilter);

    m4return_t  Count (m4uint32_t &ao_iCount);

    m4return_t  Add (m4uint32_t ai_iIndex, ClMMIndex ai_MMIndex);

    m4return_t  Remove (m4uint32_t ai_iIndex, ClMMIndex &aio_MMIndex);
    m4return_t  RemoveAll (void);

    m4return_t  Notify_DeleteRegister (ClMMIndex &aio_MMIndex);
    m4return_t  Notify_AddRegister (ClMMIndex ai_MMIndex);

    m4return_t  Init (m4uint32_t ai_number, const ClBlockReference &ai_br, ClAccessProvider *ai_pap, _ClRegisterFilter * ai_pClSubFilter);

	m4return_t  GetSize(ClSize &ao_size);

    m4uint32_t  GetNewElements (void);
    void        ResetNewElements (void);

protected:

    m4uint32_t m_iCount, m_iArraySize, m_iNewElements;
    ClMMIndex * m_pClMMIndex;
};

#endif // __MMARR_HPP__
