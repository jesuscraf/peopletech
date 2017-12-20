//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             reg_load.hpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             27/04/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Autoload 'filter' declaration
//
//
//==============================================================================


#ifndef __REG_LOAD_HPP__
#define __REG_LOAD_HPP__
#include "m4dm_dll.hpp"
#include "regi_smp.hpp"

class ClAccessProvider;
class ClBlockReference;

class M4_DECL_M4DM ClRegisterFilter_AutoLoad : public ClRegisterFilter_Simple
{
public:
    ClRegisterFilter_AutoLoad ();
    ~ClRegisterFilter_AutoLoad ();

    //m4return_t Next   (ClRegisterIndex &aio_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br);
    //m4return_t Prev   (ClRegisterIndex &aio_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br);
    m4return_t Begin  (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t End    (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t MoveTo (ClRegisterIndex ai_newindex, ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    //m4return_t MoveToEOF (ClRegisterIndex &aio_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br);
    m4return_t Count  (m4uint32_t &ao_icount, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

    //m4return_t UpdateIndex  (ClRegisterIndex &aio_Index, ClMMIndex ai_MMIndex, const ClBlockReference &ai_br);    
    //m4bool_t IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br);

    Type_e Type (void) const { return _ClRegisterFilter::AutoLoad; }

private:
    m4return_t preprocess (const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
};

#endif //__REG_LOAD_HPP__
