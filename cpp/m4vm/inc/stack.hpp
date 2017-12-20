//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             access.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/97
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

#ifndef __STACK__HPP__
#define __STACK__HPP__

#include "m4vm_dll.hpp"

#include "m4types.hpp"
#include "m4var.hpp"


template <class T> class tpFixedStack ;
class ClVMachine2;


struct StStackStatus
{
    m4uint32_t ipos;
};

class M4_DECL_M4VM IStackInterface
{
public:

    virtual m4return_t Push (const m4VariantType &ai_var)=0;
    virtual m4return_t Pop (m4VariantType &ao_var)=0;

    virtual m4uint32_t Count (void)=0;
    virtual void Reset (void)=0;


private:
};

class M4_DECL_M4VM ClStackInterface: public IStackInterface
{
public:
    ClStackInterface ();
    virtual ~ClStackInterface ();

    m4return_t Push (const m4VariantType &ai_var);
    m4return_t Pop (m4VariantType &ao_var);

    m4uint32_t Count (void);
    void Reset (void);

    void Init (ClVMachine2 *);

private:
    ClVMachine2 *m_pVM;

    tpFixedStack<m4VariantType> *_GetStack (void);
};

#endif // __STACK__HPP__
