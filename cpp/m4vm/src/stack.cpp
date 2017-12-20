//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             stack.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             17/07/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    Stack outside-interface
//
//
//==============================================================================

#include "stack.hpp"

#include "vmachin2.h"


ClStackInterface::ClStackInterface ()
{
    m_pVM = 0;
}

ClStackInterface::~ClStackInterface ()
{
}

void ClStackInterface::Init (ClVMachine2 *ai_pvm)
{
    m_pVM = ai_pvm;
}

m4return_t ClStackInterface::Push (const m4VariantType &ai_var)
{
    m4return_t res;

    m4VariantType var;
	
	//Copiamos el variant, pero como referencia. No nos interesa ser owners (es temporal)
	var.Type = ai_var.Type;
	var.Data = ai_var.Data;
	var.IsUser = ai_var.IsUser;

    if (var.Type == M4CL_CPP_TYPE_STRING_VAR || var.Type == M4CL_CPP_TYPE_STRING_SYS) {        
        m_pVM->GetMediator ()->ToPoolString (var);
    }

    res = _GetStack ()->Push (var);

    return res;
}

m4return_t ClStackInterface::Pop (m4VariantType &ao_var)
{
    m4return_t res;

    res = _GetStack ()->Top (ao_var);

    if (res == M4_SUCCESS){
        res = _GetStack ()->Pop ();

        if (res == M4_SUCCESS){
            if (ao_var.Type == M4CL_CPP_TYPE_STRING_POOL) {
                m_pVM->GetMediator ()->ToCharString (ao_var);
            }
        }
    }

    return res;
}

m4uint32_t ClStackInterface::Count (void)
{
    return _GetStack ()->GetPos () + 1;
}

void ClStackInterface::Reset (void)
{
    m4return_t res;
	
	if (m_pVM->GetStateStack()->IsEmpty ()){
		do {    
			res = _GetStack ()->Pop ();
		} while (res == M4_SUCCESS);
		_GetStack ()->SetReference(0);

		// ok, lets clear the string pool also
		m_pVM->GetPool()->Reset();
	}
}


tpFixedStack<m4VariantType> *ClStackInterface::_GetStack (void)
{
    M4_ASSERT (m_pVM);

    return m_pVM->GetRuntimeStack ();
}

