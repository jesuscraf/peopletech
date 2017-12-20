//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             rblck_it.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             12/05/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Super Register Iterator, iterates through all blocks presents in a Node
//
//
//==============================================================================

#ifndef __RBLCK_IT_HPP__
#define __RBLCK_IT_HPP__
#include "m4dm_dll.hpp"
#include "regi_smp.hpp"

class _ClRegisterFilter_Terminator;
class _AClComponentStore;
class _AClComponentStore_ForwardIt;
class _ClRegister_Base;
class ClAccessProvider;

class ClBlockReference;


class M4_DECL_M4DM ClRegisterFilter_BlockWalker : public _ClRegisterFilter_Generic
{
public:
    ClRegisterFilter_BlockWalker (
        _AClComponentStore *ai_pComponentStore,
        _ClRegister_Base *ai_pClRegister);
   ~ClRegisterFilter_BlockWalker ();

    m4return_t Next   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Prev   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Begin  (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t End    (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t MoveTo (ClRegisterIndex ai_newindex, ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t MoveToEOF (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Count  (m4uint32_t &ao_icount, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

    m4return_t UpdateIndex  (ClRegisterIndex &aio_Index, ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
    {
        aio_Index = ai_MMIndex;
        return M4_SUCCESS;
    }

    m4bool_t IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
    {
        //devuelve siempre cierto
        return M4_TRUE;
    }

    void Notify_DestroyRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) { return; }
    void Notify_InsertRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) { return; }
    void Notify_DestroyBlock_After (const ClBlockReference &ai_br, ClAccessProvider *ai_pap) { return; }
    void Notify_ReleaseAll_Before (ClAccessProvider *ai_pap) { return; }

    _ClRegisterFilter * Copy (ClFilterInterface * ai_pClFI);

	m4return_t UpdateFilter(const ClBlockReference &ai_br, ClAccessProvider *ai_pap) { return M4_SUCCESS; }
    m4return_t UpdateOneIndex(ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap){ return M4_SUCCESS; }

    m4return_t InstallFormula (m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs){ return M4_SUCCESS; }

    _ClFilterUtility * operator -> (void) const
    {
        return m_pClFilterUtility;
    }

    Type_e Type (void) const { return  _ClRegisterFilter::BlockWalker; }

	m4return_t GetSize(ClSize &ao_size);

private:
    _AClComponentStore *m_pComponentStore;
    _AClComponentStore_ForwardIt *m_pComponentStore_ForwardIt;
    _ClRegister_Base *m_pClRegister;

    ClMMIndex _GetNextValid (ClAccessProvider *ai_pap);
    ClMMIndex _GetPrevValid (ClAccessProvider *ai_pap);
};


#endif //__RBLCK_IT_HPP__
