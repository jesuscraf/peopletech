//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                Function.h
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:            20 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  EXTCONCI.DOC
//
//
// Definition:
//
//    This module defines... un atributo, con su nombre, id...número de argumentos..
////
//==============================================================================

#ifndef _CL_ATTRIBUTE_H
#define _CL_ATTRIBUTE_H

#include "m4vm_dll.hpp"

#include "m4types.hpp"
#include "m4define.hpp"

#include "functiondesc.hpp"


class M4_DECL_M4VM ClAttribute: public ClFunctionDesc {
public:
	ClAttribute(const m4char_t *ai_Name, 
				m4int8_t ai_NumArgs, 
				m4uint16_t ai_Id,
				m4char_t *ai_ParamDesc, 
				m4bool_t ai_bGenDep);

		
	ClAttribute(const ClAttribute &right);
	ClAttribute(){}; //vacio. No usar

	m4bool_t	GetGenDependencie	(void)	{ return m_bGeneDependencie;}
	m4return_t  SetGenDependencie	(m4bool_t ai_bDep){
		m_bGeneDependencie=ai_bDep;
		return M4_SUCCESS;
	}


protected:
	m4bool_t m_bGeneDependencie;

	//para poder meterlos en una estructura de STL con búsquedas
	friend m4bool_t operator < (const ClAttribute &left, const ClAttribute &right){
		return ( (strcmp(left.m_acName,right.m_acName)<0) ? M4_TRUE : M4_FALSE ) ;
	}
	friend m4bool_t operator==(const ClAttribute &left, const ClAttribute &right){
		return ( (strcmp(left.m_acName,right.m_acName)==0) ? M4_TRUE : M4_FALSE ) ;

	}


};

#endif

