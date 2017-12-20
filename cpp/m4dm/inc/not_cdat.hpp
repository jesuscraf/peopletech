//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             not_cdat.hpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             24/06/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

//este archivo se le dedico a mi amigo Pipo, que me dio la brillante idea de
//poder notificar eventos a los notificadores

//#include "not_chan.hpp"

#include "notifier.hpp"
#include "not_cli.hpp"

#include "index.hpp"
#include "mmindex.hpp"

#ifndef __NOT_CDAT_HPP__
#define __NOT_CDAT_HPP__


class ClBlock_Channel_Proxy;
class ClNode_Data;
class ClBlockReference;


class ClNotifier_Channel_Data : public _AClNotifier, public _AClClient_Notifier
{
public:
//----------------------------
//interface _AClClient_Notifier:
    void DestroyRegister_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index);
    void DestroyRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index);

    void AddRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index);
    void InsertRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index);

    void DestroyAllRegisters_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br);
    void DestroyBlock_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br);
    void DestroyProxyBlock_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br);
    void ReleaseAll_Before (void);
    void DestroyChannelData_Before (void);
    void DestroyChannel_Before (ClChannel * ai_poCh);

    void Arrange_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, m4uint32_t *ai_pindex_qtc);

    void ChannelDataChanged_Before (ClChannel_Data *ai_pClChannel_Data, ClNotifier_Channel_Data *ai_pnewnotifier, ClChannel * ai_poCh);

	m4return_t GetSize(ClSize &ao_size);
//----------------------------
};

#endif //__NOT_CDAT_HPP__
