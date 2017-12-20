//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                m4vmstate_t.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                15-05-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module set the machine's state
//
//==============================================================================

#ifndef __M4VMSTATE_T_HPP__
#define __M4VMSTATE_T_HPP__

#include "m4mdrt.hpp"
#include "index.hpp"
#include "clgeniod.hpp" // To serialize

class ClAccess;
class ClAccessRecordSet;


enum	stStateStepperType
{
	StepperTypeNormal,
	StepperTypeActual,
	StepperTypeParent
} ;


//estado de la máquina 
struct m4VMState_t
{
	m4uint32_t			m_hItem;			//Handle del Item
	m4uint16_t			m_iRule;			//Id. de la regla
	m4uint16_t			m_Flags;			//Flags de estado
	m4date_t			m_dDate;
	m4uint32_t			m_iCurrent;			//Instruccion actual en ejecucion
	m4int32_t			m_iOldStackReference;
	m4date_t			m_dBeginDate;
	m4date_t			m_dEndDate;
	ClDecodeFormula		m_poFormula;		//Formula asociada
    
    ClAccess			*m_poAccess;
    ClAccessRecordSet	*m_poRecordSet;
    ClRegisterIndex		m_oRegisterIndex;

	m4int32_t			m_iParentStatePos;	//Posicion absoluta del estado padre
	m4int32_t			m_iSourceLine;		//Source Line
	stStateStepperType	m_iStepperType;		// Tipo de estado para el stepper

#ifdef _DEBUG
	m4int32_t			m_iDebugVarReferences;

	m4VMState_t( void )
	{
		m_iDebugVarReferences = -1;
	}
#endif

	// Serialization functions >>>>>>>>>>>>>>>>>>>>>>>>>>>
	m4return_t Serialize(ClGenericIODriver& IOD, m4bool_t ai_bLocals, m4int32_t ai_iNotSentVariables);

	m4return_t DeSerialize(ClGenericIODriver& IOD, m4int32_t ai_iVariablesInStack);
   // Serialization functions <<<<<<<<<<<<<<<<<<<<<<<<<<<
};

ostream & operator <<(ostream &out, m4VMState_t ai_data);

#endif
