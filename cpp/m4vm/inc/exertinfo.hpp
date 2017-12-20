//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             executor.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             12/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    class RT info declaration
//
//
//==============================================================================

#ifndef __EXECUTOR_RT_INFO_HPP__
#define __EXECUTOR_RT_INFO_HPP__

#include "m4vm_dll.hpp"

#include "m4types.hpp"
#include "m4stl.hpp"
#include "cltypes.hpp"
#include "m4var.hpp"
#include "medvarin.hpp"


class ClCompiledMCR;


class M4_DECL_M4VM ClExecutorRuntimeInfo{

public:
	ClExecutorRuntimeInfo();
	~ClExecutorRuntimeInfo();

	string						GetInfoAsString(void);
	
	//este objeto usa un metacanal del que no es dueño. Estamos seguros  de que cuando se rellenan los datos
	//el metacanal es válido, pero no en otro momento (el peligro es rellenarlo y leugo intentar pedir información
	//cuando el canal ya se ha destruido.. Esta función quita la referencia al metacanal
	void						Reset(void);

	m4uint32_t					GetItem(void);
	ClCompiledMCR *				GetpCMCR(void);
	void						SetItem(ClCompiledMCR *ai_poCMCR, m4uint32_t  ai_hItem);

	void						SetArgNum(m4uint32_t ai_iNumArg);
	m4return_t					SetArg   (const m4VariantType &ai_vArg, m4uint32_t ai_iPos);

	m4uint32_t					GetArgNum(void);
	m4return_t					GetArg   (m4VariantType &ai_vArg, m4uint32_t ai_iPos);


protected:
	m4uint32_t		m_hItem;
	m4VariantType	*m_avArgs;
	m4uint32_t		m_iNumArgs;
	ClCompiledMCR   *m_poCMCR;	
	ClVariantConversor m_oMediator;
};

#endif
