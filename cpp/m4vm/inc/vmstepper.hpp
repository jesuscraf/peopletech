//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           VM 
// File:             ClVmStepper.hpp
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
#ifndef __CLVMSTEPPER_HPP__
#define __CLVMSTEPPER_HPP__

#include "m4vm_dll.hpp"

#include "execontroller.hpp"
#include "breakpointlist.hpp"


class ClGenericIODriver;

enum eVMStepperMode {
	StepperModeNormal,
	StepperModeStepInto,
	StepperModeStepOver,
	StepperModeStepOut
};



/**
 * Defines the stepper, (por fin!!! uff!!!)
 */

class M4_DECL_M4VM ClVMStepper : public  ClVMExecutionController{

public:

	ClVMStepper ( void );
	~ClVMStepper ( void );

	ClBreakpoint*	CheckForBP( const m4VMState_t &ai_roState ) ;
	m4bool_t		CheckForStop( const m4VMState_t &ai_roState ) ;

	ClBreakpointList &GetBPs(void)
	{
		return m_oBPs;
	}
	
	void SetMode(eVMStepperMode ai_eMode)
	{
		m_eMode = ai_eMode;
	}

	eVMStepperMode GetMode(void) const
	{
		return m_eMode;
	}

	void SetActive(m4bool_t ai_bActive)
	{
		m_bActive = ai_bActive;
	}

	void SetReenterInactive(m4bool_t ai_bReenterInactive)
	{
		m_bReenterInactive = ai_bReenterInactive;
	}

	m4bool_t GetActive(void) const
	{
		return m_bActive;
	}

	m4bool_t GetReenterInactive(void) const
	{
		return m_bReenterInactive;
	}

	m4return_t  Serialize(ClGenericIODriver& IOD);
	m4return_t  DeSerialize (ClGenericIODriver& IOD);
protected:

	ClBreakpointList	m_oBPs;

	eVMStepperMode		m_eMode;
	m4bool_t			m_bActive;
	m4bool_t			m_bReenterInactive;
};

#endif //__CLVMSTEPPER_HPP__

