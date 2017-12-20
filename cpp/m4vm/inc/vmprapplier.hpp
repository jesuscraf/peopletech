    //==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:               VMachin1.h
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition: 
//
//    Payroll functions
////
//==============================================================================


#ifndef _ClVMPRAPPLIER_H
#define _ClVMPRAPPLIER_H


#include "cldefine.hpp"
#include "prnodef.hpp"
#include "prdefine.hpp"
#include "m4var.hpp"





class ClVMPayRollApplier{
public:
	
	ClVMPayRollApplier	(void) : m_ValuesInfo(0),m_DmdComponentsInfo(0),m_EmployeeInfo(0),m_dTimeUnit(0) {}

	void	SetContext(EmployeeInfoPR *ai_EmployeeInfo, ValuesInfoPR *ai_ValuesInfo, NodeInfoPR *ai_PeriodsInfo, DmdComponentsInfo *ai_DmdComponentsInfo , m4date_t ai_dTimeUnit)
	{
		m_ValuesInfo=ai_ValuesInfo;
		m_PeriodsInfo=ai_PeriodsInfo;
		m_DmdComponentsInfo=ai_DmdComponentsInfo;
		m_EmployeeInfo=ai_EmployeeInfo;
		m_dTimeUnit=ai_dTimeUnit;
	}


	virtual m4return_t _AddRegisterToEmployeeWithSlices (m4int32_t iRegValueFind,  m4date_t ai_dStart, m4date_t ai_dEnd, NodeInfoPR *ai_poNode, ClRegisterIndex ai_idxCR )=0;
    virtual m4return_t _AddRegisterToEmployeeNoSlices   (m4int32_t iRegValueFind, m4date_t ai_dStart, m4date_t ai_dEnd )=0;

protected:
	ValuesInfoPR      *m_ValuesInfo;
    NodeInfoPR        *m_PeriodsInfo;
    DmdComponentsInfo *m_DmdComponentsInfo;
    EmployeeInfoPR    *m_EmployeeInfo;
	m4date_t		  m_dTimeUnit;

	virtual m4return_t _CopyFromValToEmpSlice (m4uint8_t ai_iSliceBehaviour, m4date_t ai_dStartToApply, m4date_t ai_dEndToApply, NodeInfoPR *ai_poNode, ClRegisterIndex ai_idxCR);
    virtual m4return_t _CopyFromValToEmp      (m4uint8_t ai_iSliceBehaviour,m4date_t ai_dStartToApply, m4date_t ai_dEndToApply);
    virtual m4bool_t   DmdMustApply (m4VariantType &ai_DmdComponent, m4uint8_t &ao_iSliceBehaviour);

};


class ClVMPayRollApplierNormal:public  ClVMPayRollApplier{
public:
  
	m4return_t _AddRegisterToEmployeeWithSlices (m4int32_t iRegValueFind,  m4date_t ai_dStart, m4date_t ai_dEnd, NodeInfoPR *ai_poNode, ClRegisterIndex ai_idxCR );
    m4return_t _AddRegisterToEmployeeNoSlices   (m4int32_t iRegValueFind, m4date_t ai_dStart, m4date_t ai_dEnd );

};

class ClVMPayRollApplierApplyAlways:public  ClVMPayRollApplier{
public:
  
	m4return_t _AddRegisterToEmployeeWithSlices (m4int32_t iRegValueFind,  m4date_t ai_dStart, m4date_t ai_dEnd, NodeInfoPR *ai_poNode, ClRegisterIndex ai_idxCR );
    m4return_t _AddRegisterToEmployeeNoSlices   (m4int32_t iRegValueFind, m4date_t ai_dStart, m4date_t ai_dEnd );

};

#endif
