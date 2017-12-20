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
//    This module defines... una función, con su nombre, id...número de argumentos..
////
//==============================================================================

#ifndef _CL_FUNCTION_H
#define _CL_FUNCTION_H

#include <stdio.h>
#include "vmdefine.hpp"
#include "vmtypes.hpp"

#include "vvm.hpp"



class ClFunction{
protected:
	

	m4return_t (ClVVM :: *m_pfExeFunc)(void);
	m4uint16_t m_iId;
	m4uint16_t m_iIdObj;
	

public:


	
	ClFunction(m4uint16_t ai_Id, m4uint16_t ai_IdObj, m4return_t(ClVVM::*ai_pExeFunc)(void) );
	ClFunction(const ClFunction &right);
	ClFunction(){}; //vacio. No usar
	
	//Funciones GET
	m4uint16_t GetId() const {return m_iId;};
	m4uint16_t GetIdObject() const {return m_iIdObj;};
	m4return_t(ClVVM::*GetExeFunc(void))(void)  {return m_pfExeFunc;};

	m4return_t SetId(m4uint16_t ai_Id)	{
		m_iId=ai_Id;
		return M4_SUCCESS;
	};

	m4return_t SetIdObject(m4uint16_t ai_IdObj)	{
		m_iIdObj=ai_IdObj;
		return M4_SUCCESS;
	};

	m4return_t SetExeFunc(m4return_t(ClVVM::*ai_pExeFunc)(void)) {
		m_pfExeFunc=ai_pExeFunc;
		return M4_SUCCESS;
	};

	//para poder meterlos en una estructura de STL con búsquedas
	friend m4bool_t operator==(const ClFunction &left, const ClFunction &right){
		return (left.m_iId==right.m_iId )?M4_TRUE:M4_FALSE ;
	}

	friend m4bool_t operator < (const ClFunction &left, const ClFunction &right){
		return ( left.m_iId<right.m_iId )?M4_TRUE:M4_FALSE ;
	}

};



#endif	// _CL_FUNCTION_H
