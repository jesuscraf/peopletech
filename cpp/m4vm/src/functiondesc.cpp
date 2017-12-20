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
//    Implementación de la clase función
////
//==============================================================================
#include "functiondesc.hpp"

//*******************
//Constructores
//*******************

ClFunctionDesc::ClFunctionDesc(const m4char_t *ai_Name, m4int8_t ai_NumArgs, m4uint16_t ai_Id, m4char_t *ai_ParamDesc,  m4int8_t ai_iType, m4int8_t ai_flags, m4bool_t ai_bIsDatabase){
	
	strncpy(m_acName,ai_Name,M4CL_MAX_NOMBRE_FUNC);
	m_acName[M4CL_MAX_NOMBRE_FUNC]=0;	//por si acaso

	strncpy(m_acParamDesc,ai_ParamDesc,M4CL_MAX_PARAM);
	m_acParamDesc[M4CL_MAX_PARAM]=0;	//por si acaso
	
	m_iNumArgs=ai_NumArgs;
	m_iId=ai_Id;
	m_iType=ai_iType;
	m_fFlags=ai_flags;
	m_bIsDatabase=ai_bIsDatabase;
}


ClFunctionDesc::ClFunctionDesc(const ClFunctionDesc &right){
	strncpy(m_acName,right.m_acName,M4CL_MAX_NOMBRE_FUNC);
	m_acName[M4CL_MAX_NOMBRE_FUNC]=0;	//por si acaso

	strncpy(m_acParamDesc,right.m_acParamDesc,M4CL_MAX_PARAM);
	m_acParamDesc[M4CL_MAX_PARAM]=0;	//por si acaso
	
	m_iNumArgs=right.m_iNumArgs;
	m_iId=right.m_iId;
	m_iType=right.m_iType;
	m_fFlags=right.m_fFlags;
	m_bIsDatabase=right.m_bIsDatabase;
}


//*******************
//Funciones SET
//*******************

m4return_t ClFunctionDesc::SetName(const m4char_t *ai_Name){
	strncpy(m_acName,ai_Name,M4CL_MAX_NOMBRE_FUNC);
	m_acName[M4CL_MAX_NOMBRE_FUNC]=0;	//por si acaso
	return M4_SUCCESS;		
}

m4return_t ClFunctionDesc::SetParamDescription (const m4char_t *ai_ParamDesc){
	strncpy(m_acParamDesc,ai_ParamDesc,M4CL_MAX_PARAM);
	m_acParamDesc[M4CL_MAX_PARAM]=0;	//por si acaso
	return M4_SUCCESS;
}

m4int8_t	ClFunctionDesc::GetNumReferedArgs (void){
	m4int8_t	iNumRefArgs=0;
	m4pchar_t	pcTemp = m_acParamDesc;
	for ( ; (pcTemp = strchr(pcTemp,'r')) != NULL ; ++iNumRefArgs, ++pcTemp);
	return iNumRefArgs;
}
