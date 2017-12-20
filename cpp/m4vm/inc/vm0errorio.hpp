//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                ExeFurep.h
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
//    Declaración de la clase que implementa las funciones de error e I/O
////
//==============================================================================



#ifndef _ClVM0ERRORIO_H
#define _ClVM0ERRORIO_H

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"



class ClVM0ErrorIO : public ClVVM{

	const m4VMState_t &m_oState;
public:

	ClVM0ErrorIO (const m4VMState_t &ai_oState):m_oState(ai_oState){ 
	
	}

	//---------------------------------
    //Errores de usuario & I/O
    //---------------------------------
    m4return_t  InitFile        (void);
    m4return_t  ConcatFile      (void);
	m4return_t	OpenFile		(void);
	m4return_t	ReadFile		(void);
	m4return_t	CloseFile		(void);
	m4return_t	DestroyFile		(void);
	
    m4return_t  DumpUserInfo	(void);
    m4return_t  DumpUserError	(void);
    m4return_t  GetErrorCode	(void);
	m4return_t  PurgeErrorCode	(void);
	
	//nuevo modelo de gestion de errores
	m4return_t	GetLog			(void);
	m4return_t	SetLog			(void);
	m4return_t	GetLogSize		(void);
	m4return_t	PurgeLog		(void);
	
	m4return_t  MessageBox      (void);
    m4return_t  DialogWBox      (void);

	//file manager
	m4return_t  ClearFiles		(void);

private:


};

#endif

