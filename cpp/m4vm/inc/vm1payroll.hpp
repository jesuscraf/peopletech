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
//    Virtual machine de nivel 1 funciones de algoritmos rtelacionados con nomina.
//		Se implementa en av y curlib (aplicavalor, pivotados y librería de monedas)
////
//==============================================================================

#ifndef _ClVM1PAYROLLLIB_H
#define _ClVM1PAYROLLLIB_H 

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"
#include "cldstack.hpp"
#include "handles.hpp"

#include "m4stl.hpp"
#include "colprop.hpp"
#include "priorstack.hpp"
#include "dmtypes.hpp"

class ClVMPayRoll;
class ClVMachine1;
class ClCompiledMCR;
class ClRegister;
class ClAccess;


class ClVM1PayrollLib : public ClVVM{
	
	m4VMState_t &m_oState1;
	ClVMachine1	*m_poVM;



public:

	ClVM1PayrollLib (m4VMState_t &ai_oState):	m_oState1(ai_oState)
	{
		m_oDatesStack.Init (0.0, 0.0, M4CL_INITIAL_STACK_SLICES_NUM);
		m_poPayRoll=0;
	}

	void SetVM (ClVMachine1 *ai_poVM) {m_poVM = ai_poVM;}

	~ClVM1PayrollLib ();

	//-----------------------------
    //Aplica valor
    //-----------------------------
    m4return_t prTransferApplyValue  (void);
    m4return_t prCreateOrganigram    (void);
    m4return_t prApplyTable          (void);
    m4return_t prInitApplyValue      (void);
    m4return_t prSetDates            (void);
    m4return_t prSetPeriods          (void);

    //----------------------------------------
	//TIs de Acumulado
	//----------------------------------------
	m4return_t	YTDSearch			(void);
	m4return_t	YTDSearchFiltered   (void);


    //----------------------------------------
    //Currency
    //----------------------------------------
    m4return_t curExchangeRecord		(void);
	m4return_t curExchangeRecordRounded	(void);
    m4return_t curGetExchange		    (void);
	m4return_t curGetExchangeRounded    (void);
	m4return_t curGetDecimals			(void);

	m4return_t curGetCurrencyExchange	(void);
	m4return_t curGetCurrencyExchangeRounded (void);

	    //----------------------------------------
    //Pivotado 
    //----------------------------------------
    m4return_t pvPivot      (void);
    m4return_t pvUnPivot    (void);
    m4return_t pvCrossTab   (void);

	virtual m4uint32_t GetSize (void) const;
private:

	typedef set<m4pchar_t, m4pchar_t_less> ClCurrencyList;
	typedef ClCurrencyList::iterator itClCurrencyList;
	
	//  Currency
	m4return_t _curGetExchangeRate (m4pchar_t ai_pcSrcCurType, m4pchar_t ai_pcDstCurType, m4pchar_t ai_pcExType, m4date_t ai_dDate, m4double_t & ao_dRate, m4bool_t &ao_bExchangeFound, m4VariantType * ao_pvDecimals, ClAccess * & aio_poExchangeAccess, ClCurrencyList * aio_pCurList = 0);
	m4return_t _curExchangeRecord (ClRegister * ai_poReg, m4pchar_t ai_pcDstCur, m4pchar_t ai_pcExType, m4date_t ai_dDate, m4int8_t iFlag, m4bool_t ai_bRound);
	m4return_t _curGetDecimals (m4pchar_t ai_pcSrcCurType, m4VariantType & ao_vDecimals, ClAccess * & aio_poExchangeAccess);
	//acumulados
	m4return_t	_FindEqualItems ( ClHandle ai_hNodo1, ClCompiledMCR *ai_poCMCR1 , VectorItemIndex &ai_vIndex1, ClHandle ai_hNodo2, ClCompiledMCR *ai_poCMCR2 , VectorItemIndex &ai_vIndex2 ,m4int32_t &ao_iNumItem,m4uint8_t ai_ItemType = M4CL_ITEM_SCOPE_REGISTER);
	
	m4return_t	YTDSearchAlgorithm (m4VariantType *ao_vFixedArgs, ClAccess *ai_poAccess, m4uint32_t ai_hNode, StFunctionCmp_t *ai_astfcmpAND, m4VariantType *ai_avFFConditions, m4uint32_t ai_iNumCond, m4uint8_t ai_iLeaveSlices, const m4char_t *ai_pccMethod);
	m4return_t	GetFixedYTDArguments (ClAccess *&ao_poAccess, m4uint32_t &ao_hNode, m4VariantType *ao_vFixedArgs, const m4char_t *ai_pccMethod);

	ClVMPayRoll         *m_poPayRoll;
	ClDateStack			m_oDatesStack;
	ClPriorSliceStack	m_oItemSlicesStack, m_oNodeSlicesStack;


};
#endif





