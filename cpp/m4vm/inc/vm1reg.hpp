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
//    Virtual machine de nivel 1 funciones de registro
////
//==============================================================================

#ifndef _ClVM1REGISTER_H
#define _ClVM1REGISTER_H

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"

class ClVMachine1;

class ClVM1Register : public ClVVM{
	
	const m4VMState_t &m_oState1;
	ClVMachine1		*m_poVM;
public:

	ClVM1Register (const m4VMState_t &ai_oState):	m_oState1(ai_oState)
	{
		
	}

	void SetVM (ClVMachine1 *ai_poVM) {m_poVM = ai_poVM;}

//----------------------------------------
	//Manejo de registros
	//----------------------------------------
	m4return_t AddRegister		(void);
	m4return_t InsertRegister	(void);
	m4return_t DeleteRegister	(void);
    m4return_t DestroyRegister	(void);
	m4return_t Begin			(void);
	m4return_t End				(void);
	m4return_t MoveToEOF		(void);
	m4return_t MoveTo			(void);
    m4return_t Count			(void);
    m4return_t GetCurrent		(void);
    m4return_t IsEOF    		(void);
    m4return_t DeleteAllRegisters (void);
    m4return_t DestroyAllRegisters(void);
    m4return_t DestroyBlock		(void);
    m4return_t GetRegPosition   (void);
	m4return_t Next				(void);
	m4return_t Previous 		(void);

	//undo's , checkPoints
	m4return_t UndoRegister		(void);
	m4return_t UndoBlock		(void);
	m4return_t UndoNode			(void);
	m4return_t UndoChannel		(void);

	m4return_t CheckPointRegister	(void);
	m4return_t CheckPointBlock		(void);
	m4return_t CheckPointNode		(void);
	m4return_t CheckPointChannel	(void);

	m4return_t CopyRegister		(void);
	m4return_t CloneRegister    (void);
	m4return_t CopyBranch       (void);
	m4return_t PasteBranch      (void);

	m4return_t IsDeleted        (void);
    m4return_t IsNew            (void);
    m4return_t IsUpdated        (void);
    m4return_t IsNewAndDeleted  (void);
    m4return_t SetInserted      (void);
	m4return_t IsModifiedByDDBB (void);

    m4return_t IsBlockUpdated   (void);
    m4return_t IsNodeUpdated    (void);

    m4return_t GetNewNotPersist (void);
    m4return_t SetNewNotPersist (void);

	//----------------------------------------
    //Volcar Registro a vmreg .log---- pruebas
    //----------------------------------------
    m4return_t  DumpRegister(void);
    m4return_t  DumpChannel (void);
    m4return_t  DumpNode    (void);
	m4return_t  DumpStatistics(void);

	//------------------------------------
	//Serialización avanzada
	//------------------------------------
	m4return_t SendRegisterMark (void);
	m4return_t SendBlockMark    (void);
	m4return_t SendNodeMark     (void);


	//------------------------------------
	//Soporte de Timeout
	//------------------------------------
	m4return_t SetCSTimeOut        (void);
	m4return_t GetCSTimeOut        (void);
	m4return_t ConfigCSTimeOut     (void);
	m4return_t SetCSTimeoutFunction(void);


private:
	//para conseguir cosas de nivel 2

    m4return_t GetAccessAndNode      (m4pcchar_t ai_pszThisChannel, ClAccess *ai_poThisAccess, m4uint32_t ai_hThisNode, m4pcchar_t ai_pszChannel, m4pcchar_t ai_pszNode, ClNode *& ao_poNode, ClAccess *& ao_poAccess);
};

#endif

