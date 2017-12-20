//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           VM 
// File:             breakpoint.hpp
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
//    Execution breakpoint
//
//
//==============================================================================
#ifndef __BREAKPOINT_HPP__
#define __BREAKPOINT_HPP__

#include "m4string.hpp"
#include "m4vm_dll.hpp"
#include "executionpoint.hpp"

class ClGenericIODriver;

/**
 * Defines a breakpoints 
 */
class M4_DECL_M4VM ClBreakpoint : public ClExecutionPoint{

public:
	ClBreakpoint(){
		m_iValue=0;
		m_bActive=M4_FALSE;
	}
	
	ClBreakpoint(
		m4pcchar_t	ai_pccChannel,
		m4pcchar_t	ai_pccNode,
		m4pcchar_t	ai_pccItem,
		m4pcchar_t	ai_pccRule, 
		m4pcchar_t	ai_pccRuleTI,
		m4date_t	ai_dStart,
		m4date_t	ai_dEnd,
		m4uint32_t	ai_iLine,
		m4int32_t	ai_iValue,
		m4bool_t	ai_bActive)
		
		:ClExecutionPoint(ai_pccChannel,ai_pccNode,ai_pccItem,ai_pccRule,ai_pccRuleTI,ai_dStart,ai_dEnd,ai_iLine ){

		m_iValue=ai_iValue;
		m_bActive=ai_bActive;
	}

	~ClBreakpoint(){};

	void SetValue(m4int32_t ai_iValue)
	{
		m_iValue=ai_iValue;
	}
	void SetActive(m4bool_t ai_bActive)
	{
		m_bActive=ai_bActive;
	}

	m4int32_t GetValue(void)const
	{
		return m_iValue;
	}
	m4bool_t  GetActive(void)const
	{
		return m_bActive;
	}

	//---------------
	//Operadores
	//---------------
	m4bool_t operator < (const ClBreakpoint &rhs) const {
		//ordenación: 
		//1.- Número de linea
		//2.- Fechas
		//3.- item
		//4.- Node
		//5.- Rule
		//6.- M4Obj
		
		if (m_iLine != rhs.m_iLine) return (m_iLine < rhs.m_iLine)? M4_TRUE : M4_FALSE;

		/* Bug 0157611
		La búsqueda debe ser case insensitive
		*/
		m4int32_t i = stricmp (m_acItem, rhs.m_acItem);
		if (i !=0 ) return M4_BOOL(i < 0);
		i = stricmp (m_acNode, rhs.m_acNode);
		if (i !=0 ) return M4_BOOL(i < 0);
		i = strcmpi (m_acRule, rhs.m_acRule);
		if (i !=0 ) return M4_BOOL(i < 0);
		i = strcmpi (m_acRuleTI, rhs.m_acRuleTI);
		if (i !=0 ) return M4_BOOL(i < 0);
		i = strcmpi (m_acChannel, rhs.m_acChannel);
		if (i !=0 ) return M4_BOOL(i < 0);

		//Bug:65644   Begin  -------------------------------------------------------------
		// Al dar a continuar en el Stepper, no paraba en breakpoints puestos en reglas tramadas, en concreto
		//  a partir del segundo tramo
		if(m_dStart <= m_dEnd && rhs.m_dStart <= rhs.m_dEnd && m_dStart <= rhs.m_dStart && m_dEnd >= rhs.m_dEnd)
			return M4_FALSE; //En este caso los consideramos iguales
		if(m_dStart != rhs.m_dStart)
			return M4_BOOL(m_dStart < rhs.m_dStart);
		if(m_dEnd != rhs.m_dEnd)
			return M4_BOOL(m_dEnd < rhs.m_dEnd);

		return M4_FALSE; //Son identicos
		//Bug:65644   End    -------------------------------------------------------------
	}

	m4return_t Serialize	(ClGenericIODriver& IOD) const;
	m4return_t DeSerialize	(ClGenericIODriver& IOD);

protected:

	m4int32_t	m_iValue;
	m4bool_t	m_bActive;


};

#endif //__BREAKPOINT_HPP__

