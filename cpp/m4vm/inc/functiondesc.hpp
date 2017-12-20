//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                FunctionDesc.h
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


#ifndef _CL_FUNCTIONDESC_H
#define _CL_FUNCTIONDESC_H

#include "m4vm_dll.hpp"

#include "m4types.hpp"
#include "cldefine.hpp"

#include <stdio.h>
#include <string.h>




class M4_DECL_M4VM ClFunctionDesc{
protected:
	
	m4char_t m_acName[M4CL_MAX_NOMBRE_FUNC+1];
	m4char_t m_acParamDesc[M4CL_MAX_PARAM+1];	//un caracter por parametro obligatorio
												// R->Valor
												// r->referencia

	m4uint16_t m_iId;
	m4int8_t m_iNumArgs;	// >0 Numero Fijo de argumentos
							// 0 Sin argumentos
							// n<0  al menos n argumentos aunque puede tener más
							// n<-M4CL_MAX_PARAM Cualquier número variable de argumentos
	
	m4int8_t m_iType;
	m4int8_t m_fFlags;

	m4bool_t m_bIsDatabase;
	

public:


	
	ClFunctionDesc(const m4char_t *ai_Name, m4int8_t ai_NumArgs, m4uint16_t ai_Id, m4char_t *ai_ParamDesc,  m4int8_t ai_iType, m4int8_t ai_flags, m4bool_t ai_bIsDatabase );
	ClFunctionDesc(const ClFunctionDesc &right);
	ClFunctionDesc(){}; //vacio. No usar
	
	//Funciones GET
	const m4char_t *GetName(void) const
	{
		return m_acName;
	}

	m4int8_t GetNumArgs(void) const
	{
		return m_iNumArgs;
	}

	m4uint16_t GetId() const
	{
		return m_iId;
	}

	const m4char_t *GetParamDescription (void) const
	{
		return m_acParamDesc;
	}

	m4int8_t GetType (void)
	{
		return m_iType;
	}

	m4int8_t GetFlags(void)
	{
		return m_fFlags;
	}

	m4bool_t GetIsDatabase( void ) const
	{
		return( m_bIsDatabase ) ;
	}


	//Funciones SET
	m4return_t SetName(const m4char_t *ai_Name);
	
	m4return_t SetType (m4int8_t ai_iType){
		m_iType=ai_iType;
		return M4_SUCCESS;
	}
	
	m4return_t SetNumArgs(m4int8_t ai_NumArgs)	{
		m_iNumArgs=ai_NumArgs;
		return M4_SUCCESS;
	}
	
	m4return_t SetId(m4uint16_t ai_Id)	{
		m_iId=ai_Id;
		return M4_SUCCESS;
	}

	m4return_t SetParamDescription (const m4char_t *ai_ParamDesc);


	m4bool_t	HasReferedArgs		(void){
		return (strchr (m_acParamDesc,'r')!=NULL ? M4_TRUE: M4_FALSE);
	}

	m4return_t SetFlags(m4int8_t ai_flags)	{
		m_fFlags=ai_flags;
		return M4_SUCCESS;
	}

	m4return_t SetIsDatabase( m4bool_t ai_bIsDatabase )
	{
		m_bIsDatabase = ai_bIsDatabase ;
		return( M4_SUCCESS ) ;
	}

	m4int8_t	GetNumReferedArgs (void) ;

	//para poder meterlos en una estructura de STL con búsquedas
	friend m4bool_t operator==(const ClFunctionDesc &left, const ClFunctionDesc &right){
		return ( M4_BOOL(strcmp(left.m_acName,right.m_acName)==0) ) ;
	}

	friend m4bool_t operator < (const ClFunctionDesc &left, const ClFunctionDesc &right){
		return ( M4_BOOL(strcmp(left.m_acName,right.m_acName)<0) ) ;
	}

};



#endif
