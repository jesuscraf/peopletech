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

#ifndef __EXECUTIONPOINT_HPP__
#define __EXECUTIONPOINT_HPP__

#include "m4vm_dll.hpp"
#include "m4types.hpp"
#include "cldefine.hpp"
#include "string.h"


struct m4VMState_t;


/**
 * Defines a specific points at the execution of a item.
 */
class M4_DECL_M4VM ClExecutionPoint{

public:

	ClExecutionPoint( void ) ;
	ClExecutionPoint(	m4pcchar_t	ai_pccChannel,
						m4pcchar_t	ai_pccNode,
						m4pcchar_t	ai_pccItem,
						m4pcchar_t	ai_pccRule,
						m4pcchar_t	ai_pccRuleTI,
						m4date_t	ai_dStart,
						m4date_t	ai_dEnd,
						m4uint32_t	ai_iLine) ;
	
	virtual ~ClExecutionPoint( void ){};

	m4pcchar_t	GetChannelId(void) const
	{
		return m_acChannel;
	}
	m4pcchar_t	GetNodeId(void) const
	{
		return m_acNode;
	}
	m4pcchar_t GetItemId(void) const
	{
		return m_acItem;
	}
	m4pcchar_t	GetRuleId(void) const
	{
		return m_acRule;
	}
	m4pcchar_t	GetRuleTIId(void) const
	{
		return m_acRuleTI;
	}
	m4uint32_t	GetLine(void) const
	{
		return m_iLine;
	}
	m4date_t	GetStartDate(void) const
	{
		return m_dStart;
	}
	m4date_t	GetEndDate(void) const
	{
		return m_dEnd;
	}

	
	void SetChannelId(m4pcchar_t ai_pccChannel){

		strncpy(m_acChannel,ai_pccChannel,M4CL_MAX_T3_ID);
		m_acChannel[M4CL_MAX_T3_ID]=0;
	}
	
	void SetNodeId(m4pcchar_t ai_pccNode)
	{
		strncpy(m_acNode,ai_pccNode,M4CL_MAX_NODE_ID);
		m_acNode[M4CL_MAX_NODE_ID]=0;
	}
	
	void SetItemId(m4pcchar_t ai_pccItem)
	{
		strncpy(m_acItem,ai_pccItem,M4CL_MAX_ITEM_ID);
		m_acItem[M4CL_MAX_ITEM_ID]=0;
	}
	
	void SetRuleId(m4pcchar_t ai_pccRule)
	{
		strncpy(m_acRule,ai_pccRule,M4CL_MAX_RULE_ID);
		m_acRule[M4CL_MAX_RULE_ID]=0;
	}

	void SetRuleTIId(m4pcchar_t ai_pccRuleTI)
	{
		strncpy(m_acRuleTI,ai_pccRuleTI,M4CL_MAX_TI_ID);
		m_acRuleTI[M4CL_MAX_TI_ID]=0;
	}

	void SetLine(m4uint32_t ai_iLine)
	{
		m_iLine=ai_iLine;
	}
	
	void SetStartDate(m4date_t ai_dStart)
	{
		m_dStart=ai_dStart;
	}
	
	void SetEndDate(m4date_t ai_dEnd)
	{
		m_dEnd=ai_dEnd;
	}

	m4return_t Fill (const m4VMState_t &ai_roState);


protected:

	m4char_t	m_acChannel [ M4CL_MAX_T3_ID   + 1 ] ;
	m4char_t	m_acNode	[ M4CL_MAX_NODE_ID + 1 ] ;
	m4char_t	m_acItem	[ M4CL_MAX_ITEM_ID + 1 ] ;
	m4char_t	m_acRule	[ M4CL_MAX_RULE_ID + 1 ] ;
	m4char_t	m_acRuleTI	[ M4CL_MAX_TI_ID   + 1 ] ;

	m4uint32_t	m_iLine ;
	m4date_t	m_dStart ;
	m4date_t	m_dEnd ;

} ;

#endif //__EXECUTIONPOINT_HPP__

