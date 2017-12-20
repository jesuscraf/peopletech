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
//    Declaración de la clase que implementa las funciones de cadenas
////
//==============================================================================



#ifndef _ClVM0STR_H
#define _ClVM0STR_H

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"

class ClRegExp;

class ClVM0String : public ClVVM{

	const m4VMState_t &m_oState;
public:

	ClVM0String (const m4VMState_t &ai_oState):m_oState(ai_oState){ 
		m_poRegex=0;
	}
	virtual ~ClVM0String();

	//------------------------------
    //String Functions
    //------------------------------
    m4return_t  Mid             (void);
    m4return_t  StrIn           (void);
    m4return_t  Length          (void);
    m4return_t  Trim            (void);
    m4return_t  ConvertCase     (void);
    m4return_t  Replace         (void);
    m4return_t  IndexOf         (void);
    m4return_t  LastIndexOf     (void);
	m4return_t  Format          (void);
	m4return_t  Ascii		    (void);
    m4return_t  Chr		        (void);
	m4return_t	MatchRegex		(void);

private:
	ClRegExp	*m_poRegex;
};

#endif

