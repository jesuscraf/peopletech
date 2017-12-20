#ifndef __NOT_CLI_HPP__
#define __NOT_CLI_HPP__

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             not_cli.hpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             25/06/98
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

#include "mmindex.hpp"
#include "sizer.hpp"


class ClBlockReference;
class ClChannel;
class ClChannel_Data;
class ClNode_Data;
class ClNotifier_Channel_Data;


class _AClClient_Notifier
{
public:
//----------------------------
//interface _AClClient_Notifier obligatorio:
//por defecto no hace nada:
    virtual void DestroyRegister_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index) {}
    virtual void DestroyRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index) {}

    virtual void AddRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index) {}
    virtual void InsertRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index) {}

    virtual void DestroyAllRegisters_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br) {}
    virtual void DestroyBlock_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br) {}
    virtual void DestroyProxyBlock_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br) {}
    virtual void ReleaseAll_Before (void) {}
    virtual void DestroyChannelData_Before (void) {}
    virtual void DestroyChannel_Before (ClChannel * ai_poCh) {}
    virtual void Arrange_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, m4uint32_t *ai_pindex_qtc) {}

    virtual void ChannelDataChanged_Before (ClChannel_Data *ai_pClChannel_Data,
        ClNotifier_Channel_Data *ai_pnewnotifier, ClChannel * ai_poCh) {}

// Aunque no tiene nada que ver con los notificador, es necesario que este aqui para
// que compile en ClChannel, Recordset, register. Ya que solo se usa con notificadores.
    virtual ClChannel * GetpChannel (void)
    {
        return 0;
    }

	virtual m4return_t GetSize(ClSize &ao_size)
	{
		ao_size+=sizeof(_AClClient_Notifier);
		return M4_SUCCESS;
	}

	//para avisar a la gente por cortesia profesional:
    //virtual void Detached_Before (ClNotifier_Channel_Data *ai_poldnotifier) {}
    //virtual void Attached_After (ClNotifier_Channel_Data *ai_pnewnotifier) {}
//----------------------------
};

#endif //__NOT_CLI_HPP__
