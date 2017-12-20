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
//    Virtual machine de nivel 1 funciones de filtros
////
//==============================================================================

#ifndef _ClVM1FILTERSORT_H
#define _ClVM1FILTERSORT_H

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"

class ClVM1FilterSort : public ClVVM{
	
	const m4VMState_t &m_oState1;

public:

	ClVM1FilterSort (const m4VMState_t &ai_oState):	m_oState1(ai_oState)
	{
		
	}


	
	m4return_t AddFilter		(void);
	m4return_t RemoveFilter		(void);
	m4return_t CleanFilter		(void);
	m4return_t RefreshFilter	(void);
	m4return_t GetFilterArgument(void);
	m4return_t SetFilterArgument(void);
    //ordenaciones por filtros
	m4return_t AddSortFilter	(void);

	    //ordenación-búsquedas
    m4return_t Sort     (void);
    m4return_t BSearch  (void);

	//busquedas lineales
	m4return_t FindRegister		(void);

};

#endif

