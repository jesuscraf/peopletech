//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             acc_prop.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             14/08/97
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

//## begin module.includes preserve=yes
#include "colprop.hpp"
#include "property.h"
//## end module.includes preserve=yes

#ifndef __ACC_PROP__HPP__
#define __ACC_PROP__HPP__

class ClAccess;
class ClVMachine1;

/*
class ClRunSliceModeProperty : public ClProperty
{
public:	
	ClRunSliceModeProperty(ClVMachine1* ai_pVM);
	~ClRunSliceModeProperty();

	m4return_t Get(m4VariantType &ao_var);
	m4return_t Set(const m4VariantType &ai_SliceMode);

private:
	ClVMachine1* m_pVM;
};
*/

#endif //__ACC_PROP__HPP__
