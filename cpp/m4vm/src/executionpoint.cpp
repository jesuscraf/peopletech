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

#include "executionpoint.hpp"
#include "m4mdrt.hpp"
#include "access.hpp"
#include "vmtypes.hpp"


ClExecutionPoint::ClExecutionPoint( void )
{
	m_acChannel[0]=0;
	m_acNode[0]=0;
	m_acItem[0]=0;
	m_acRule[0]=0;
	m_acRuleTI[0]=0;

	m_iLine=0;
	m_dStart=0;
	m_dEnd=0;
}

ClExecutionPoint::ClExecutionPoint(
					m4pcchar_t	ai_pccChannel,
					m4pcchar_t	ai_pccNode,
					m4pcchar_t	ai_pccItem,
					m4pcchar_t	ai_pccRule,
					m4pcchar_t	ai_pccRuleTI,
					m4date_t	ai_dStart,
					m4date_t	ai_dEnd,
					m4uint32_t	ai_iLine)
{

	SetChannelId( ai_pccChannel ) ;
	SetNodeId( ai_pccNode ) ;
	SetItemId( ai_pccItem ) ;
	SetRuleId( ai_pccRule ) ;
	SetRuleTIId( ai_pccRuleTI ) ;
	SetLine( ai_iLine ) ;
	SetStartDate( ai_dStart ) ;
	SetEndDate( ai_dEnd ) ;
}


m4return_t ClExecutionPoint::Fill (const m4VMState_t &ai_roState)
{

	m4uint32_t		hNode = 0 ;
	ClCompiledMCR	*poCMCR = NULL ;


	if( ai_roState.m_poFormula.IsNull() == M4_TRUE )
	{
		return( M4_ERROR ) ;
	}

	poCMCR = ai_roState.m_poAccess->GetpCMCR() ;
	hNode = poCMCR->GetItemNodeHandle( ai_roState.m_hItem ) ;

	SetChannelId( poCMCR->GetChannelId() ) ;
	SetNodeId	( poCMCR->GetNodeId( hNode ) ) ;
	SetItemId	( poCMCR->GetItemId( ai_roState.m_hItem ) ) ;
	SetRuleId	( poCMCR->GetItemFormIdByPosition( ai_roState.m_hItem, ai_roState.m_iRule ) ) ;
	SetRuleTIId	( poCMCR->GetPointer( ai_roState.m_poFormula.GetRuleTI() ) ) ;
	SetLine		( ai_roState.m_iSourceLine ) ;
	SetStartDate( ai_roState.m_poFormula.GetStartDate() ) ;
	SetEndDate	( ai_roState.m_poFormula.GetEndDate() ) ;

	return( M4_SUCCESS ) ;
}

