//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           VM 
// File:             ExecutionPoint.hpp
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
//    Execution point
//
//
//==============================================================================
#ifndef __RUNNINGFORMINFO_HPP__
#define __RUNNINGFORMINFO_HPP__

#include "m4vm_dll.hpp"

#include "executionpoint.hpp"

#include "m4types.hpp"
#include "m4var.hpp"
#include "vmmcrdef.hpp"



/**
 * Defines a specific points at the execution of a item.
 * while running
 */
class M4_DECL_M4VM ClRunningFormInfo : public ClExecutionPoint{

public:

	ClRunningFormInfo();
	~ClRunningFormInfo();

	void		SetNumberofArguments	(m4uint32_t ai_iNumArgs);
	m4return_t	SetArgument				(const m4VariantType &ai_vArg,m4uint32_t ai_iPos);
	void		SetIsVariable			(m4bool_t	ai_bIsVariable);
	void		SetDates				(m4date_t ai_dStart, m4date_t ai_dEnd, m4date_t ai_dRun);
	void		SetChannelHandle		(m4int32_t ai_hHandle);
	void		SetAccessHandle			(m4int32_t ai_hHandle);
	void		SetIsClientSide			(m4bool_t  ai_bIsClient);

	void			GetDates				(m4date_t &ao_dStart, m4date_t &ao_dEnd, m4date_t &ao_dRun);
	m4uint32_t		GetNumberofArguments	(void);
	m4return_t		GetArgument				(m4uint32_t ai_iPos, m4VariantType &ao_vValue);
	m4return_t		GetArguments			(m4VariantType	*ao_pvArgs,m4uint32_t ai_iSize);
	m4bool_t		GetIsVariable			(void);
	m4int32_t		GetChannelHandle		(void);
	m4int32_t		GetAccessHandle			(void);
	m4bool_t		GetIsClientSide			(void);
 

	ClRunningFormInfo	&operator=(const ClRunningFormInfo	&rhs);
protected:

	
	m4date_t	m_dRunningStart;
	m4date_t	m_dRunningEnd;
	m4date_t	m_dRunningCut;

	m4bool_t    m_bVariableArgs; 
	
	m4uint32_t	m_iNumberofArguments;
	
	m4int32_t	m_hChannel;
	m4int32_t	m_hAccess;

	m4bool_t	m_bIsClient;

	m4VariantType *m_avArgs;

};

#endif //__RUNNINGFORMINFO_HPP__ 

