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
#include "function.hpp"

//*******************
//Constructores
//*******************

ClFunction::ClFunction(m4uint16_t ai_Id, m4uint16_t ai_IdObj, m4return_t(ClVVM::*ai_pExeFunc)(void)){
	m_iId=ai_Id;
	m_pfExeFunc=ai_pExeFunc;
	m_iIdObj=ai_IdObj;
}


ClFunction::ClFunction(const ClFunction &right){
	m_iId=right.m_iId;
	m_pfExeFunc=right.m_pfExeFunc;
	m_iIdObj=right.m_iIdObj;
}


