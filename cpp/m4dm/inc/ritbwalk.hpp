//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             ritbwalk.hpp
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

#ifndef __RITBWALK_HPP__
#define __RITBWALK_HPP__
#include "m4dm_dll.hpp"
#include "register.hpp"

class M4_DECL_M4DM ClRegister_BlockWalker : public ClRegister
{
public:
    ClRegister_BlockWalker (ClAccess *ai_paccess) : ClRegister (ai_paccess) {}
    ClRegister_BlockWalker (const ClRegister &ai_reg) : ClRegister (ai_reg) {}

    void Attach (ClAccess *ai_paccess)
    {
        //para que no quede oculta hay que definirla también
        ClRegister::Attach (ai_paccess);
    }    
    void Attach (ClNode_Data *ai_pnc);

    void Attach (ClNode *ai_pnc);
};

#endif //__RITBWALK_HPP__
