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
//    Virtual machine de nivel 1 funciones de indirecciones
////
//==============================================================================

#ifndef _ClVM1INDIRECTIONS_H
#define _ClVM1INDIRECTIONS_H

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"
#include "vmachin1.h"

 
class ClItem;
//class ClVMachine1;

class ClVM1Indirections : public ClVVM{
	
	const m4VMState_t &m_oState1;
	ClVMachine1		*m_poVM;
public:

	ClVM1Indirections (const m4VMState_t &ai_oState):	m_oState1(ai_oState)
	{
		
	}


	void SetVM (ClVMachine1 *ai_poVM) {m_poVM = ai_poVM;}

	    //----------------------------------------
    //indirecciones
    //----------------------------------------
    m4return_t SetChannelValue				(void);
    m4return_t GetChannelValue				(void);
    m4return_t ChannelCall					(void);
    m4return_t ChannelCallAs				(void);
	m4return_t GetChannelAttribValue		(void);
	m4return_t GetChannelAttribSliceValue	(void);
    m4return_t ChannelAttribCall			(void);
    m4return_t SetValue						(void);
    m4return_t SetValuePriority				(void);
	m4return_t SetPriority					(void);
    m4return_t GetValue						(void);
    m4return_t Call 						(void);
    m4return_t CallNode						(void);
    m4return_t ExecuteBranch				(void);

private:

    m4return_t _ChannelCall(m4uint32_t ai_iArguments, ClItem* &ao_rpItem);
    m4return_t _Call(m4bool_t ai_bByNode);

//Para indirecciones
    m4return_t GetItemIndirect        ( m4VariantType  *ai_variant, m4uint8_t ai_iNumVar, m4bool_t bSlice, ClItem *&ao_poItem, m4bool_t *aio_pbNewSlices=NULL); 
    m4return_t GetChannelItemIndirect ( m4VariantType  *ai_variant, m4uint8_t ai_iNumVar, m4bool_t bSlice, ClItem *&ao_poItem, m4bool_t *aio_pbNewSlices=NULL, m4bool_t ai_bNeedJustInfo=M4_FALSE);
		//el parametro ai_bCreateSliceToAssig es para crear un tramo en el caso
		//de que queramos asignar un valor, estemos en ejecución con tramos
		//y no esten especificando un slice a capon

};

#endif

