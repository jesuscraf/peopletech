//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             reg_filt.hpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             04/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Simple filter declaration
//
//
//==============================================================================

/*#ifndef ClRegisterIndex
#include "index.hpp"
#endif

#ifndef ClMMIndex
#include "mmindex.hpp"
#endif
*/

#include "index.hpp"
#include "mmindex.hpp"

#ifndef __REG_FILT_HPP__
#define __REG_FILT_HPP__
#include "m4dm_dll.hpp"
#include "regi_smp.hpp"
#include "mmindex.hpp"

#include "m4var.hpp"

class ClBlock_Channel_Proxy;
class _ClFilterUtility;
class ClNode_Kernel;
class ClChannel;
class ClBlockReference;

class M4_DECL_M4DM ClRegisterDynFilter : public _ClRegisterFilter_Generic
{
public:
    ClRegisterDynFilter (ClChannel *ai_pchannel, ClNodeIndex ai_inodeindex, m4pchar_t ai_pName = 0);
    ClRegisterDynFilter (ClChannel *ai_pchannel, ClNodeIndex ai_inodeindex, m4int32_t ai_pHandleForm, m4pchar_t ai_pName = 0);
	ClRegisterDynFilter (_ClFilterUtility * ai_pClFU, m4pchar_t ai_pName = 0);
    ClRegisterDynFilter (m4uint8_t ai_OpType, m4pchar_t ai_pName = 0);
    ClRegisterDynFilter (ClChannel *ai_pchannel, ClNodeIndex ai_nodeindex, ClItemIndex ai_Index, m4VariantType ai_var, m4pchar_t ai_pName = 0);
    ClRegisterDynFilter (ClChannel *ai_pchannel, ClNodeIndex ai_nodeindex, ClItemIndex ai_Index, m4VariantType ai_var, m4uint32_t ai_iOperation, m4pchar_t ai_pName = 0);

    m4return_t InstallFormula (m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs);

    m4return_t Next   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Prev   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Begin  (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t End    (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t MoveTo (ClRegisterIndex ai_newindex, ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t MoveToEOF (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t Count  (m4uint32_t &ao_icount, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

    m4return_t UpdateIndex  (ClRegisterIndex &aio_Index, ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

    void Notify_DestroyRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    void Notify_InsertRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    void Notify_DestroyBlock_After (const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    void Notify_ReleaseAll_Before (ClAccessProvider *ai_pap);

    m4bool_t IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

	m4return_t UpdateFilter(const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
    m4return_t UpdateOneIndex(ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap) { return M4_SUCCESS; }

    _ClRegisterFilter * Copy (ClFilterInterface * ai_pClFI, m4bool_t ai_bTotal = M4_FALSE);

    _ClFilterUtility * operator -> (void) const
    {
        return m_pClFilterUtility;
    }

    Type_e Type (void) const { return _ClRegisterFilter::DynamicFilter; }

	m4return_t GetSize(ClSize &ao_size);

private:

    m4return_t FindLastVisible (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);
	m4return_t FindFirstVisible (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);	

    m4VariantType m_vHandleFormula, m_vFormula;    
};

#endif // __REG_FILT_HPP__
