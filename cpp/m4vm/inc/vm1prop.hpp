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
//    Virtual machine de nivel 1
////
//==============================================================================

#ifndef _ClVM1PROPAGATOR_H
#define _ClVM1PROPAGATOR_H

class ClAccess;
class ClAccessRecordSet;
class ClVMachine1;

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"
#include "index.hpp"

class ClVM1Propagator : public ClVVM{
	
	const m4VMState_t &m_oState1;
	ClVMachine1	*m_poVM;
public:

	ClVM1Propagator (const m4VMState_t &ai_oState):	m_oState1(ai_oState)
	{
		
	}

	void SetVM (ClVMachine1 *ai_poVM) {m_poVM = ai_poVM;}
	
	
	//funciones de la propagación

		//de registro a bloque
	m4return_t PropagateRBRR(void);
	m4return_t PropagateRBRB(void);
	m4return_t PropagateRBRN(void);

	m4return_t PropagateRBBR(void);
	m4return_t PropagateRBBB(void);
	m4return_t PropagateRBBN(void);

	m4return_t PropagateRBNR(void);
	m4return_t PropagateRBNB(void);
	m4return_t PropagateRBNN(void);


		//de bloque a bloque

	m4return_t PropagateBBRR(void);
	m4return_t PropagateBBRB(void);
	m4return_t PropagateBBRN(void);


	m4return_t PropagateBBBR(void);
	m4return_t PropagateBBBB(void);
	m4return_t PropagateBBBN(void);


	m4return_t PropagateBBNR(void);
	m4return_t PropagateBBNB(void);
	m4return_t PropagateBBNN(void);

		
		//ASOCIACIÓN LIBRE
	m4return_t PropagateFreeRR(void);
	m4return_t PropagateFreeRB(void);
	m4return_t PropagateFreeRN(void);

	m4return_t PropagateFreeBR(void);
	m4return_t PropagateFreeBB(void);
	m4return_t PropagateFreeBN(void);

	m4return_t PropagateFreeNR(void);
	m4return_t PropagateFreeNB(void);
	m4return_t PropagateFreeNN(void);

private:
	m4return_t AddPropagateMethodToExecute (m4uint32_t ai_hNodo, m4uint32_t ai_hItem,  
											m4uint16_t ai_iNumArgs,
								 			ClAccessRecordSet *ai_poRecordSet, ClRegisterIndex ai_RegIndex,
											m4int32_t ai_iParentStatePos, m4VariantType *ai_param,
											m4VMState_t &ai_oParentState);

    m4return_t _PropagateRBR_	(m4bool_t ai_bBlock, m4bool_t ai_bRegister);
    m4return_t _PropagateRBB_	(m4bool_t ai_bBlock, m4bool_t ai_bRegister);
    m4return_t _PropagateBBB_	(m4bool_t ai_bBlock, m4bool_t ai_bRegister);
    m4return_t _PropagateFree_	(m4bool_t ai_bBlock, m4bool_t ai_bRegister);
 
	m4return_t _GeneratePropMoveTo (ClAccessRecordSet *ai_poRecordSet,ClRegisterIndex ai_Reg, m4bool_t ai_bUseMM, m4bool_t ai_bRefresh);

	m4return_t GetContext  (m4uint32_t ai_hParent, m4uint32_t ai_hChild, ClCompiledMCR   *ai_poCMCR, m4uint8_t &ao_context);
};

#endif

