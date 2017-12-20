//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             ritbwalk.cpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             13/05/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//
//
//==============================================================================

#include "ritbwalk.hpp"
#include "rblck_it.hpp"

#include "node.hpp"
//#include "ch_node.hpp"
#include "node_knl.hpp"

#include "m4types.hpp"

void ClRegister_BlockWalker::Attach (ClNode *ai_pnc)
{
    Attach (ai_pnc->GetpNode_Data ());
}

void ClRegister_BlockWalker::Attach (ClNode_Data *ai_pnc)
{
//    _ClRegister_Base::Attach (ai_pnc);
    ClRegister::Attach (ai_pnc);
    //_ClRegister_Base::Attach (ai_pnotifier);

    //ya se ha inicializado el Filter, tiene o un Simple o en un PartialLoad
    M4_ASSERT (Filter.IsInitialized ());

    ClRegisterFilter_BlockWalker *pbw = new ClRegisterFilter_BlockWalker (ai_pnc->GetpBlockStore (), this);

    Filter.Add (pbw);
}
