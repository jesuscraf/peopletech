//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                Function.cpp
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
//    Implementación de la clase attributo
////
//==============================================================================
#include "attrib.hpp"
#include "vmdefine.hpp"

//*******************
//Constructores
//*******************

ClAttribute::ClAttribute(const ClAttribute &right){
	
	strncpy(m_acName,right.m_acName,M4CL_MAX_NOMBRE_FUNC);
	m_acName[M4CL_MAX_NOMBRE_FUNC]=0;	//por si acaso
	
	strncpy(m_acParamDesc,right.m_acParamDesc,M4CL_MAX_PARAM);
	m_acParamDesc[M4CL_MAX_PARAM]=0;	//por si acaso
	
	m_iNumArgs=right.m_iNumArgs;
	m_iId=right.m_iId;
	m_iType=right.m_iType;
	m_bGeneDependencie=right.m_bGeneDependencie;
}


ClAttribute::ClAttribute(const m4char_t *ai_Name, 
						 m4int8_t ai_NumArgs, 
						 m4uint16_t ai_Id,
						 m4char_t *ai_ParamDesc, 
						 m4bool_t ai_bGenDep): ClFunctionDesc (ai_Name, ai_NumArgs, ai_Id, ai_ParamDesc, M4CL_ATRIBUTE_FUNC, 0, M4_FALSE) {
	
	m_bGeneDependencie=ai_bGenDep;
}
