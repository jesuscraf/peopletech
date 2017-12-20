//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             clfufast.hpp
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
//    class FilterUtilityVariableArguments
//
//==============================================================================

#ifndef __CLFUFAST_HPP__
#define __CLFUFAST_HPP__

#include "clfuvar.hpp"
#include "m4types.hpp"
#include "mmindex.hpp"
#include "index.hpp"
#include "m4var.hpp"
#include "clargume.hpp"

class ClAccess;
class ClBlock_Channel_Proxy;

class ClBlockReference;

class ClFU_Fast : public _ClFilterUtility
{
public:
    //ClFU_Fast(ClNode_Channel, ClItemIndex ai_Index, m4VariantType ai_var);
    ClFU_Fast(ClChannel *ai_pchannel, ClNodeIndex ai_inodeindex, ClItemIndex ai_Index, m4VariantType ai_var, m4uint32_t ai_iOperation=M4CL_CASE_EQUAL_FUNCTION);

    ClFU_Fast(_ClFilterUtility * ai_pClFU);
    //ClFU_Fast(ClNode_Channel *ai_pNode
    UtilityType Type(void) { return _ClFilterUtility::Fast; };

    m4return_t Prepare (m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs) { return M4_ERROR; };
    //m4return_t Init (ClNode_Channel *ai_pNodeChannel);
    m4return_t Init (ClChannel *ai_pChannel);

    m4bool_t IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap);

	m4return_t GetSize(ClSize &ao_size);

protected:

    ClItemIndex Index;
    m4VariantType m_Var;
//    ClAccess * m_pAccess;
    //ClNode_Channel *m_pNodeChannel;
    ClNodeIndex m_iNodeIndex;
    m4int32_t m_iCompFunc;
};

#endif //__CLFUVAR_HPP__
