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
//    Virtual machine de nivel 1 funciones de algoritmos raros
////
//==============================================================================

#ifndef _ClVM1LIB_H
#define _ClVM1LIB_H

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"

#include "cldstack.hpp"

class ClIndexList;
class ClVMachine1;

class ClVM1BasicLib : public ClVVM{
	
	const m4VMState_t &m_oState1;
	ClVMachine1	*m_poVM;


public:

	ClVM1BasicLib (const m4VMState_t &ai_oState):	m_oState1(ai_oState)
	{
		m_oDatesStack.Init (0.0, 0.0, M4CL_INITIAL_STACK_SLICES_NUM);

	}
	void SetVM (ClVMachine1 *ai_poVM) {m_poVM = ai_poVM;}

	    //flatten
    m4return_t Flatten			(void);
    m4return_t Unflatten		(void);
	m4return_t JoinSlices		(void);

	//----------------------------------------
	//Lookup
	//----------------------------------------
	m4return_t LookUp				(void);

	//----------------------------------------
	//Errores
	//----------------------------------------
	m4return_t GetErrorString	(void);
    m4return_t AppRunDate       (void);
    m4return_t AppEndDate       (void);
    m4return_t AppStartDate     (void);


	m4return_t GetAutoloadMode  (void);
	m4return_t SetAutoloadMode  (void);
	m4return_t GetNodeAutoloadMode  (void);
	m4return_t SetNodeAutoloadMode  (void);

    //Llamada a métodos en dll, ocx, activex..
    m4return_t  ExternalMethodCall (void);

	//----------------------------------------
	//Statistics
	//----------------------------------------
	m4return_t sttSetLabel    (void);
	m4return_t sttGetLevel (void);
	m4return_t sttSetLevel (void);
	m4return_t sttPersistToFile (void);

    m4return_t ReplaceLiterals(void);
    m4return_t CleanLiterals(void);

	m4return_t LoadXML(void);
	m4return_t ExportXML(void);

private:

	m4return_t _loadExportXml(m4bool_t ai_bMode, m4pcchar_t ai_pccMethodName);

    // para unflatten
	m4return_t UnflattenList (ClIndexList & ai_IndexList);
	ClDateStack			m_oDatesStack;



};
#endif
