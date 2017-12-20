//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           VM 
// File:             execontroller.hpp
// Project:          m4vm
// Author:           Meta Software M.S. , S.A
// Date:             12/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    The stepper
//
//
//==============================================================================


#ifndef __CLEXECUTIONCONTROLLER_HPP__
#define __CLEXECUTIONCONTROLLER_HPP__


#include "m4vm_dll.hpp"
#include "m4types.hpp"

class ClBreakpoint;



struct m4VMState_t;

/**
 * Defines the execution controller interface
 */

class M4_DECL_M4VM ClVMExecutionController{

public:
	ClVMExecutionController ( void ){};
	virtual ~ClVMExecutionController ( void ){};

	virtual ClBreakpoint*	CheckForBP( const m4VMState_t &ai_roState ) = 0; 
	virtual m4bool_t		CheckForStop( const m4VMState_t &ai_roState ) = 0; 
};

#endif //__CLEXECUTIONCONTROLLER_HPP__

