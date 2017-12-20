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
//    Declaración de la clase que implementa las funciones de compatibilidad con 2x
////
//==============================================================================



#ifndef _ClVM0DX_H
#define _ClVM0DX_H

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"

class ClVM0dx : public ClVVM{

	const m4VMState_t &m_oState;
public:

	ClVM0dx (const m4VMState_t &ai_oState):m_oState(ai_oState){ 
		
	}
	//----------------------------------
    //De 2.x a 3.x
    //----------------------------------
    m4return_t DxYear           (void);
    m4return_t DxCompare        (void);
    m4return_t DxDay            (void);
    m4return_t DxDaysOfMonth    (void);
    m4return_t DxDate           (void);
    m4return_t DxDate30         (void);
    m4return_t DxDate30Ex       (void);
    m4return_t DxDateIso        (void);
    m4return_t DxDateIsoD       (void);
    m4return_t DxDateIsoTS      (void);
    m4return_t DxInc            (void);
    m4return_t DxMonth          (void);
    m4return_t DxRoundCent      (void);
    m4return_t DxRound          (void);
    m4return_t DxRound50        (void);
    m4return_t DxTruncaCent     (void);


};

#endif

