

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                TablFunc.cpp
// Project:        Cálculo
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:   
//
// 
// Definition: 
//
//    This module implementa la clase tabla de funciones
////
//==============================================================================
#include "tablfunc.h"
#include "vmachin1.h"
#include "vmachin2.h"
#include "vmobjdef.hpp"
#include "vmres.hpp"
//los objetos implementadores

#include "vm0dx.hpp"
#include "vm0str.hpp"
#include "vm0errorio.hpp"
#include "vm0general.hpp"

#include "vm1prop.hpp"
#include "vm1ldb.hpp"
#include "vm1ind.hpp"
#include "vm1compjit.hpp"
#include "vm1filtsort.hpp"
#include "vm1cmcr.hpp"
#include "vm1reg.hpp"
#include "vm1lib.hpp"

#include "syncro.hpp"
#include "chlog.hpp"

#include "functiondesc.hpp"
#include "tablafuncdesc.hpp"
//*****************
//Constructor
//*****************

ClFuncTable::ClFuncTable(){

		
}

//*****************
//Instancia unica!!!!!
//*****************
ClFuncTable* ClFuncTable::m_poInstance = NULL;

// Mutex para la Creacion de la Instancia unica de la Tabla de Funciones (Seccion Critica)
ClMutex oMutexFuncTable (M4_TRUE);

//*****************
//GetInstance	- Obtener la instancia unica!!!!
//crea (si es necesario) y devuelve la referencia a la instancia unica de la clase
//*****************
ClFuncTable* ClFuncTable::GetInstance(void) {
	//Comprobamos que la instancia no existe (debe ser unica)
	if (m_poInstance == NULL) {
		//Si no existia, se entra en la Seccion Critica, con doble-comprobacion
		oMutexFuncTable.Lock();
		if (m_poInstance == NULL) {
			//Si no existia, se crea
			if ( (m_poInstance=new ClFuncTable) != NULL)
				m_poInstance->InitLn4Semantic();
			else	//Error de no hay memoria
				m_poInstance = NULL;
		}
		oMutexFuncTable.Unlock();
	}
	return m_poInstance;
}



//*******************
//AddFunction
//*******************
m4return_t ClFuncTable::AddFunction(const m4char_t *ai_Name, m4int8_t ai_NumArgs, m4uint16_t ai_Id, m4uint16_t ai_IdObj, m4char_t *ai_ParamDesc, m4return_t(ClVVM::*ai_pExeFunc)(void), m4int8_t ai_iType, m4int8_t ai_iFlags, m4bool_t ai_bIsDatabase){
	
	m4char_t acName[M4CL_MAX_NOMBRE_FUNC];
	
	strcpy(acName,ai_Name);
	//pasamos a mayusculas
	strupr(acName);
	
	ClFunction		oFuncTemp(ai_Id ,ai_IdObj, ai_pExeFunc);
	
	oTable.push_back(oFuncTemp);
	ClFuncTableDesc::GetInstance()->AddFunction (acName, ai_NumArgs, ai_Id, ai_ParamDesc, ai_iType, ai_iFlags, ai_bIsDatabase);
	return M4_SUCCESS;
}

//EDU:
m4uint32_t ClFuncTable::GetSize (void) const
{
    m4uint32_t length = 0;

    length += sizeof (ClFuncTable);
    length += oTable.size () * sizeof (ClFunction);

    return length;
}

//**********************
//INIT
//**********************

void ClFuncTable::InitLn4Semantic()
{
	AddFunction("LogicalAnd"		,2	,0, M4CL_VM_OBJECT_THIS	,"RR"	,(pmfVM0)&ClVMachine0::LogicalAnd,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	AddFunction("LogicalOr"			,2	,1, M4CL_VM_OBJECT_THIS	,"RR"	,(pmfVM0)&ClVMachine0::LogicalOr,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	AddFunction("LogicalNot"		,1	,M4CL_VM_FT_LOGICALNOT,				M4CL_VM_OBJECT_THIS	,"R"	,(pmfVM0)&ClVMachine0::LogicalNot,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	AddFunction("LogicalEqual"		,2	,3, M4CL_VM_OBJECT_THIS	,"RR"	,(pmfVM0)&ClVMachine0::LogicalEqual,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	AddFunction("LogicalNotEqual"	,2	,4, M4CL_VM_OBJECT_THIS	,"RR"	,(pmfVM0)&ClVMachine0::LogicalNotEqual,	M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	AddFunction("LogicalGreater"	,2	,5, M4CL_VM_OBJECT_THIS	,"RR"	,(pmfVM0)&ClVMachine0::LogicalGreater,	M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	AddFunction("LogicalGreaterEqual",2	,M4CL_VM_FT_LOGICALGREATEREQUAL,	M4CL_VM_OBJECT_THIS	,"RR"	,(pmfVM0)&ClVMachine0::LogicalGreaterEqual, M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	AddFunction("LogicalLess"		,2	,7, M4CL_VM_OBJECT_THIS	,"RR"	,(pmfVM0)&ClVMachine0::LogicalLess,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	AddFunction("LogicalLessEqual"	,2	,8, M4CL_VM_OBJECT_THIS	,"RR"	,(pmfVM0)&ClVMachine0::LogicalLessEqual, M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	
	AddFunction("AritmeticalAdd"	,2	,9,  M4CL_VM_OBJECT_THIS	,"RR"	,(pmfVM0)&ClVMachine0::AritmeticalAdd,	M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	AddFunction("AritmeticalSub"	,2	,10, M4CL_VM_OBJECT_THIS	,"RR"	,(pmfVM0)&ClVMachine0::AritmeticalSub,	M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	AddFunction("AritmeticalMul"	,2	,11, M4CL_VM_OBJECT_THIS	,"RR"	,(pmfVM0)&ClVMachine0::AritmeticalMul,	M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	AddFunction("AritmeticalDiv"	,2	,12, M4CL_VM_OBJECT_THIS	,"RR"	,(pmfVM0)&ClVMachine0::AritmeticalDiv,	M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	AddFunction("AritmeticalChangeSign",1,13,M4CL_VM_OBJECT_THIS	,"R"	,(pmfVM0)&ClVMachine0::AritmeticalChangeSign, M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("ReadVar"			,0	,M4CL_VM_FT_READVAR,			M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine0::ReadVar,			M4CL_INTERNAL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PushNum"			,0	,M4CL_VM_FT_PUSHNUM,			M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine0::PushNum,			M4CL_INTERNAL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PushString"		,0	,M4CL_VM_FT_PUSHSTRING,			M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine0::PushString,		M4CL_INTERNAL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PushDate"			,0	,M4CL_VM_FT_PUSHDATE,			M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine0::PushDate,			M4CL_INTERNAL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PurgeStack"		,0	,M4CL_VM_FT_PURGESTACK,			M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine0::PurgeStack,		M4CL_INTERNAL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("AssignVar"			,0	,M4CL_VM_FT_ASSIGNVAR,			M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine0::AssignVar,		M4CL_INTERNAL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("ConditionalJump"	,0	,M4CL_VM_FT_CONDITIONALJUMP,	M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine0::ConditionalJump,	M4CL_INTERNAL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("InconditionalJump"	,0	,M4CL_VM_FT_INCONDITIONALJUMP,	M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine0::InconditionalJump, M4CL_INTERNAL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("Return"			,0	,M4CL_VM_FT_RETURN,				M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine0::Return,			M4CL_INTERNAL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("ExecuteItem"		,0	,M4CL_VM_FT_EXECUTEITEM,		M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine1::ExecuteItem, M4CL_INTERNAL_FUNC, 0, M4_FALSE);
	AddFunction("AssignItem"		,0	,M4CL_VM_FT_ASSIGNITEM,			M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine1::AssignItem, M4CL_INTERNAL_FUNC, 0, M4_FALSE);
	AddFunction("PushItem"			,0	,M4CL_VM_FT_PUSHITEM,			M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine1::PushItem, M4CL_INTERNAL_FUNC, 0, M4_FALSE);
	AddFunction("ReadAttribute"		,0	,M4CL_VM_FT_READATTRIBUTE,		M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine1::ReadAttribute, M4CL_INTERNAL_FUNC, 0, M4_FALSE);
	AddFunction("ExecuteAttribute"	,0	,M4CL_VM_FT_EXECUTEATTRIBUTE,	M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine1::ExecuteAttribute, M4CL_INTERNAL_FUNC, 0, M4_FALSE);

	AddFunction("ExecuteItem2"		,0	,M4CL_VM_FT_EXECUTEITEM2,		M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine2::ExecuteItem, M4CL_INTERNAL_FUNC, 0, M4_FALSE);
	AddFunction("AssignItem2"		,0	,M4CL_VM_FT_ASSIGNITEM2,		M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine2::AssignItem,	M4CL_INTERNAL_FUNC, 0, M4_FALSE);
	AddFunction("PushItem2"			,0	,M4CL_VM_FT_PUSHITEM2,			M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine2::PushItem,	M4CL_INTERNAL_FUNC, 0, M4_FALSE);
	AddFunction("ReadAttribute2"	,0	,M4CL_VM_FT_READATTRIBUTE2,		M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine2::ReadAttribute, M4CL_INTERNAL_FUNC, 0, M4_FALSE);
	AddFunction("ExecuteAttribute2"	,0	,M4CL_VM_FT_EXECUTEATTRIBUTE2,	M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine2::ExecuteAttribute, M4CL_INTERNAL_FUNC, 0, M4_FALSE);
	AddFunction("ExecuteChannelFunction2",0,M4CL_VM_FT_EXECUTECHANNELFUNCTION2, M4CL_VM_OBJECT_THIS,""	,(pmfVM0)&ClVMachine2::ExecuteChannelFunction, M4CL_INTERNAL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("MessageBox"		,-2	,34, M4CL_VM0_ERROR_IO		,"RR"	,(pmfVM0)&ClVM0ErrorIO::MessageBox,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("DumpPool"			,1	,35	, M4CL_VM_OBJECT_THIS,"R"	,(pmfVM0)&ClVMachine0::DumpPool,			M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("Load"				,0	,36	, M4CL_VM_OBJECT_THIS,""	,(pmfVM0)&ClVMachine2::LoadChannel,			M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);
	AddFunction("Persist"			,0	,37	, M4CL_VM_OBJECT_THIS,""	,(pmfVM0)&ClVMachine2::PersistChannel,		M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);
	AddFunction("Release"			,0	,38	, M4CL_VM_OBJECT_THIS,""	,(pmfVM0)&ClVMachine2::ReleaseChannel,		M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("PropagateRBRR"		,-2	,39	, M4CL_VM1_PROPAGATOR,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateRBRR, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateRBRB"		,-2	,40	, M4CL_VM1_PROPAGATOR,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateRBRB, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateRBRN"		,-2	,41, M4CL_VM1_PROPAGATOR,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateRBRN, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("PropagateRBBR"		,-2	,42	, M4CL_VM1_PROPAGATOR,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateRBBR, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateRBBB"		,-2	,43	, M4CL_VM1_PROPAGATOR,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateRBBB, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateRBBN"		,-2	,44	, M4CL_VM1_PROPAGATOR,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateRBBN, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("PropagateRBNR"		,-2	,45	, M4CL_VM1_PROPAGATOR,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateRBNR, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateRBNB"		,-2	,46	, M4CL_VM1_PROPAGATOR,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateRBNB, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateRBNN"		,-2	,47	, M4CL_VM1_PROPAGATOR,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateRBNN, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("PropagateBBRR"		,-2	,48, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateBBRR, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateBBRB"		,-2	,49, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateBBRB, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateBBRN"		,-2	,50, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateBBRN, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("PropagateBBBR"		,-2	,51, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateBBBR, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateBBBB"		,-2	,52, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateBBBB, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateBBBN"		,-2	,53, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateBBBN, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("PropagateBBNR"		,-2	,54, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateBBNR, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateBBNB"		,-2	,55, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateBBNB, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateBBNN"		,-2	,56, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateBBNN, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("PropagateFreeRR"		,-2	,57, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateFreeRR, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateFreeRB"		,-2	,58, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateFreeRB, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateFreeRN"		,-2	,59, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateFreeRN, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("PropagateFreeBR"		,-2	,60, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateFreeBR, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateFreeBB"		,-2	,61, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateFreeBB, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateFreeBN"		,-2	,62, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateFreeBN, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("PropagateFreeNR"		,-2	,63, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateFreeNR, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateFreeNB"		,-2	,64, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateFreeNB, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PropagateFreeNN"		,-2	,65, M4CL_VM1_PROPAGATOR	,"RR"	,(pmfVM0)&ClVM1Propagator::PropagateFreeNN, M4CL_PROPAGATION_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	
	AddFunction("clcCompile"			,0	,66, M4CL_VM1_COMPILER_JIT	,""		,(pmfVM0)&ClVM1CompilerAndJIT::clcCompile,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("clcGetPolish"			,0	,67, M4CL_VM1_COMPILER_JIT	,""		,(pmfVM0)&ClVM1CompilerAndJIT::clcGetPolish,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("clcCheckSyntax"		,0	,68, M4CL_VM1_COMPILER_JIT	,""     ,(pmfVM0)&ClVM1CompilerAndJIT::clcCheckSyntax,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("AddRegister"			,0	,69, M4CL_VM1_REGISTER	,""		,(pmfVM0)&ClVM1Register::AddRegister,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("InsertRegister"		,0	,70, M4CL_VM1_REGISTER	,""		,(pmfVM0)&ClVM1Register::InsertRegister,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("DeleteRegister"		,0	,71, M4CL_VM1_REGISTER	,""		,(pmfVM0)&ClVM1Register::DeleteRegister,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("Begin"					,0	,72, M4CL_VM1_REGISTER	,""		,(pmfVM0)&ClVM1Register::Begin,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("End"					,0	,73, M4CL_VM1_REGISTER	,""		,(pmfVM0)&ClVM1Register::End,					M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("MoveToEOF"				,0	,74, M4CL_VM1_REGISTER	,""		,(pmfVM0)&ClVM1Register::MoveToEOF,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("MoveTo"				,1	,75, M4CL_VM1_REGISTER	,"R"	,(pmfVM0)&ClVM1Register::MoveTo,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("Count"	    			,0	,76	, M4CL_VM1_REGISTER,""		,(pmfVM0)&ClVM1Register::Count,			    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("SetValue"	   			,-4	,77	, M4CL_VM1_INDIRECTION,"RRRR"	,(pmfVM0)&ClVM1Indirections::SetValue,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("GetValue"	   			,-3	,78 , M4CL_VM1_INDIRECTION	,"RRR"	,(pmfVM0)&ClVM1Indirections::GetValue,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("Call"	    			,-2	,79	, M4CL_VM1_INDIRECTION,"RR"		,(pmfVM0)&ClVM1Indirections::Call,			    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("sysLoad"				,0	,80, M4CL_VM1_LDB	,""		,(pmfVM0)&ClVM1LDB::SysLoad,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); //la ponemos de prop. para que sea de nivel 1
	AddFunction("Load"					,0	,81, M4CL_VM1_LDB	,""		,(pmfVM0)&ClVM1LDB::Load,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); 

    AddFunction("Unload"				,0	,82, M4CL_VM1_LDB	,""		,(pmfVM0)&ClVM1LDB::Unload,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); 

    AddFunction("Write"					,0	,83, M4CL_VM1_LDB	,""		,(pmfVM0)&ClVM1LDB::Write,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); 
    AddFunction("Delete"				,0	,84, M4CL_VM1_LDB	,""		,(pmfVM0)&ClVM1LDB::Delete,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); 

    AddFunction("Persist"				,0	,85, M4CL_VM1_LDB	,""		,(pmfVM0)&ClVM1LDB::Persist,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); 
    AddFunction("PersistAll"			,0	,86, M4CL_VM1_LDB	,""		,(pmfVM0)&ClVM1LDB::PersistAll,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); 
    AddFunction("PersistNode"			,0	,87, M4CL_VM1_LDB	,""		,(pmfVM0)&ClVM1LDB::PersistNode,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); 
    AddFunction("PersistNodeAll"		,0	,88, M4CL_VM1_LDB	,""		,(pmfVM0)&ClVM1LDB::PersistNodeAll,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); 
	
    AddFunction("BeginTransaction"		,0	,89, M4CL_VM1_LDB	,""		,(pmfVM0)&ClVM1LDB::BeginTransaction,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); 
	AddFunction("EndTransaction"		,1	,90, M4CL_VM1_LDB	,"R"	,(pmfVM0)&ClVM1LDB::EndTransaction,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); 
    AddFunction("GetConnect"    		,0	,91, M4CL_VM1_LDB	,""	    ,(pmfVM0)&ClVM1LDB::GetConnect,  		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); 
    AddFunction("FreeConnect"	    	,0	,92, M4CL_VM1_LDB	,""	    ,(pmfVM0)&ClVM1LDB::FreeConnect,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); 

    AddFunction("ExecuteGroup"		    ,1	,93, M4CL_VM_OBJECT_THIS	,"R"	,(pmfVM0)&ClVMachine1::ExecuteGroup,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 
    AddFunction("Flatten"   		    ,-(M4CL_MAX_PARAM+1),94, M4CL_VM1_LIB	,""	,(pmfVM0)&ClVM1BasicLib::Flatten,     	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 

    AddFunction("TransferApplyValue"    ,-2	,95, M4CL_VM1_PAYROLL	,"RR"	,(pmfVM0)&ClVM1PayrollLib::prTransferApplyValue,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 

    AddFunction("DumpRegister"         	,0	,96, M4CL_VM1_REGISTER    ,""	,(pmfVM0)&ClVM1Register::DumpRegister,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

   	AddFunction("ExtendedLoad"			,0	,97	, M4CL_VM1_LDB,""		,(pmfVM0)&ClVM1LDB::ExtendedLoad,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE); 

    AddFunction("BeginVMTransaction"    ,0	,98, M4CL_VM_OBJECT_THIS	,""		,(pmfVM0)&ClVMachine0::BeginVMTransaction,  M4CL_SEMANTIC_FUNC, 0, M4_FALSE); 
    AddFunction("EndVMTransaction"      ,1	,99, M4CL_VM_OBJECT_THIS	,"R"	,(pmfVM0)&ClVMachine0::EndVMTransaction,  M4CL_SEMANTIC_FUNC, 0, M4_FALSE); 

    AddFunction("NULL"	            	,0	,100, M4CL_VM_OBJECT_THIS,""		,(pmfVM0)&ClVMachine0::NullFunction,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Round"	                ,2	,101, M4CL_VM0_GENERAL	,"RR"		,(pmfVM0)&ClVM0General::Round,       		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Mid"	                ,3	,102, M4CL_VM0_STRING_MANIP	,"RRR"		,(pmfVM0)&ClVM0String::Mid,       	    	M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("CommonTime"	        ,4	,103, M4CL_VM0_GENERAL	,"RRRR"		,(pmfVM0)&ClVM0General::CommonTime,  		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Format"	            ,2	,104, M4CL_VM0_STRING_MANIP	,"RR"		,(pmfVM0)&ClVM0String::Format,	        	M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DayOfTheWeek"	        ,1	,105, M4CL_VM0_GENERAL	,"R"		,(pmfVM0)&ClVM0General::DayOfTheWeek,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Months"	            ,2	,106, M4CL_VM0_GENERAL	,"RR"		,(pmfVM0)&ClVM0General::Months,		        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Years"	            	,2	,107, M4CL_VM0_GENERAL	,"RR"		,(pmfVM0)&ClVM0General::Years,		        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Max"	            	,-2	,108, M4CL_VM0_GENERAL	,"RR"		,(pmfVM0)&ClVM0General::Max,		            M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Min"	            	,-2	,109, M4CL_VM0_GENERAL	,"RR"		,(pmfVM0)&ClVM0General::Min,		            M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Abs"	            	,1	,110, M4CL_VM0_GENERAL	,"R"		,(pmfVM0)&ClVM0General::Abs,		            M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Date30"	            ,1	,111, M4CL_VM0_GENERAL	,"R"		,(pmfVM0)&ClVM0General::Date30,		        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("AddMonths"	            ,2	,112, M4CL_VM0_GENERAL	,"RR"		,(pmfVM0)&ClVM0General::AddMonths,		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("AddYears"	            ,2	,113, M4CL_VM0_GENERAL    ,"RR"		,(pmfVM0)&ClVM0General::AddYears,		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Fraction"	            ,1	,114, M4CL_VM0_GENERAL	,"R"		,(pmfVM0)&ClVM0General::Fraction,		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Bound"	            	,3	,115, M4CL_VM0_GENERAL	,"RRR"		,(pmfVM0)&ClVM0General::Bound,		        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DaysOfYear"           	,1	,116, M4CL_VM0_GENERAL	,"R"		,(pmfVM0)&ClVM0General::DaysOfYear,	        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DaysOfMonth"	        ,1	,117, M4CL_VM0_GENERAL	,"R"		,(pmfVM0)&ClVM0General::DaysOfMonth,		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Power"	            	,2	,118, M4CL_VM0_GENERAL	,"RR"		,(pmfVM0)&ClVM0General::Power,		        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Today"	            	,0	,119, M4CL_VM0_GENERAL	,""			,(pmfVM0)&ClVM0General::Today,		        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Percentage"	        ,2	,120, M4CL_VM0_GENERAL    ,"RR"		,(pmfVM0)&ClVM0General::Percentage,		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("ToInt"     	        ,1	,121, M4CL_VM0_GENERAL    ,"R"		,(pmfVM0)&ClVM0General::ToInt,   		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("ToDouble"     	        ,1	,122, M4CL_VM0_GENERAL   ,"R"		,(pmfVM0)&ClVM0General::ToDouble,		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("ToString"     	        ,-1	,123, M4CL_VM0_GENERAL    ,"R"		,(pmfVM0)&ClVM0General::ToString,		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("ToDate"     	        ,1	,124, M4CL_VM0_GENERAL    ,"R"		,(pmfVM0)&ClVM0General::ToDate,  		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Truncate"     	        ,2	,125, M4CL_VM0_GENERAL    ,"RR"		,(pmfVM0)&ClVM0General::Truncate,  		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DateToNumbers"         ,4	,126, M4CL_VM0_GENERAL    ,"Rrrr"		,(pmfVM0)&ClVM0General::DateToNumbers,  		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DateToAllNumbers"      ,7	,127, M4CL_VM0_GENERAL    ,"Rrrrrrr"	,(pmfVM0)&ClVM0General::DateToAllNumbers,  		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("NumbersToDate"        	,-1	,128, M4CL_VM0_GENERAL    ,"R"		,(pmfVM0)&ClVM0General::NumbersToDate,  		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("InitFile"          	,-2	,129, M4CL_VM0_ERROR_IO      ,"RR"		,(pmfVM0)&ClVM0ErrorIO::InitFile,  		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("ConcatFile"           	,-2	,130, M4CL_VM0_ERROR_IO    ,"RR"		,(pmfVM0)&ClVM0ErrorIO::ConcatFile,  		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("CompileBlock"           ,0	,131, M4CL_VM1_COMPILER_JIT    ,""	    ,(pmfVM0)&ClVM1CompilerAndJIT::CompileBlock,  		    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    
    AddFunction("DumpChannel"           	,0	,132, M4CL_VM1_REGISTER    ,""	,(pmfVM0)&ClVM1Register::DumpChannel,  		    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("DumpNode"           	    ,0	,133, M4CL_VM1_REGISTER    ,""	,(pmfVM0)&ClVM1Register::DumpNode,  		    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("GetCurrent"	    		,0	,134, M4CL_VM1_REGISTER	,""		,	(pmfVM0)&ClVM1Register::GetCurrent,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("IsNull"    	    		,1	,135, M4CL_VM_OBJECT_THIS	,"R",	(pmfVM0)&ClVMachine0::IsVariantNull,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("IsEOF"     	    		,0	,136, M4CL_VM1_REGISTER	,""		,	(pmfVM0)&ClVM1Register::IsEOF,			    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("FindRegister" 	    		,-3	,137, M4CL_VM1_SORT_FILTER	,"RRR"	,(pmfVM0)&ClVM1FilterSort::FindRegister,    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("StrIn"    	    	    	,2	,138, M4CL_VM0_STRING_MANIP	,"RR"	,(pmfVM0)&ClVM0String::StrIn,			    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("NullValue"    	    		,0	,139, M4CL_VM_OBJECT_THIS, ""		,(pmfVM0)&ClVMachine0::NullValue,		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("ExternalMethodCall" 		,3	,140, M4CL_VM1_LIB	,"RRR"			,(pmfVM0)&ClVM1BasicLib::ExternalMethodCall,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("Unflatten"   		        ,-(M4CL_MAX_PARAM+1),141, M4CL_VM1_LIB	,   ""	,(pmfVM0)&ClVM1BasicLib::Unflatten,     		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 
    
    AddFunction("GetStartDate"   		,0	,142, M4CL_VM_OBJECT_THIS	,   ""	,(pmfVM0)&ClVMachine0::GetStartDate,     		M4CL_SEMANTIC_FUNC, 0, M4_FALSE); 
    AddFunction("GetEndDate"   		    ,0	,143, M4CL_VM_OBJECT_THIS	,   ""	,(pmfVM0)&ClVMachine0::GetEndDate,     		M4CL_SEMANTIC_FUNC, 0, M4_FALSE); 
    AddFunction("GetRunDate"   		    ,0	,144, M4CL_VM_OBJECT_THIS	,   ""	,(pmfVM0)&ClVMachine0::GetRunDate,     		M4CL_SEMANTIC_FUNC, 0, M4_FALSE); 

    AddFunction("SetStartDate"   		,1	,145, M4CL_VM_OBJECT_THIS	,   "R"	,(pmfVM0)&ClVMachine0::SetStartDate,     	M4CL_SEMANTIC_FUNC, 0, M4_FALSE); 
    AddFunction("SetEndDate"   		    ,1	,146, M4CL_VM_OBJECT_THIS	,   "R"	,(pmfVM0)&ClVMachine0::SetEndDate,     		M4CL_SEMANTIC_FUNC, 0, M4_FALSE); 
    AddFunction("SetRunDate"   		    ,1	,147, M4CL_VM_OBJECT_THIS	,   "R"	,(pmfVM0)&ClVMachine0::SetRunDate,     		M4CL_SEMANTIC_FUNC, 0, M4_FALSE); 

    AddFunction("CreateOrgChart"       ,0	,148, M4CL_VM1_PAYROLL	,   ""	        ,(pmfVM0)&ClVM1PayrollLib::prCreateOrganigram,     	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 
    AddFunction("LocalTest"            ,0	,149, M4CL_VM1_COMPILER_JIT	,   ""	        ,(pmfVM0)&ClVM1CompilerAndJIT::LocalTest,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 
    AddFunction("ApplyTable"           ,-3	,150, M4CL_VM1_PAYROLL	,   "RRR"	,(pmfVM0)&ClVM1PayrollLib::prApplyTable,     	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 

	AddFunction("JoinSlices"             ,0	,151, M4CL_VM1_LIB	,   ""	,(pmfVM0)&ClVM1BasicLib::JoinSlices,     		    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 
	
    AddFunction("GetRegPosition_deleted"         ,0	,152, M4CL_VM1_REGISTER	,   ""	,(pmfVM0)&ClVM1Register::GetRegPosition,    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 
	AddFunction("DeleteAllRegisters"     ,0	,153, M4CL_VM1_REGISTER	,   ""	,(pmfVM0)&ClVM1Register::DeleteAllRegisters,     	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 

    AddFunction("GoNext"                   ,0	,154, M4CL_VM1_REGISTER	,   ""	,(pmfVM0)&ClVM1Register::Next,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 
    AddFunction("GoPrevious"             ,0	,155, M4CL_VM1_REGISTER	,   ""	,(pmfVM0)&ClVM1Register::Previous,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 

    AddFunction("DialogBox" 	         ,-3	,156, M4CL_VM0_ERROR_IO		,"RRR"	,(pmfVM0)&ClVM0ErrorIO::DialogWBox,			M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("prSetDates"              ,-2	,157, M4CL_VM1_PAYROLL	,"RR"	,(pmfVM0)&ClVM1PayrollLib::prSetDates,     		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 
    AddFunction("prSetPeriods"            ,4	,158, M4CL_VM1_PAYROLL	,"RRRR"	,(pmfVM0)&ClVM1PayrollLib::prSetPeriods,     	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 
    AddFunction("prInitApplyValue"        ,2	,159, M4CL_VM1_PAYROLL	,"RR"	,(pmfVM0)&ClVM1PayrollLib::prInitApplyValue,    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 

    AddFunction("DestroyRegister"       ,0,160, M4CL_VM1_REGISTER	,   ""	,(pmfVM0)&ClVM1Register::DestroyRegister,     	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 
	AddFunction("DestroyAllRegisters"   ,0,161, M4CL_VM1_REGISTER	,   ""	,(pmfVM0)&ClVM1Register::DestroyAllRegisters,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 
	AddFunction("DestroyBlock"          ,0,162, M4CL_VM1_REGISTER	,   ""	,(pmfVM0)&ClVM1Register::DestroyBlock,     		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 
	AddFunction("clcSourceToId"			,0,163, M4CL_VM1_COMPILER_JIT		,""		,(pmfVM0)&ClVM1CompilerAndJIT::clcSourceToId,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("clcSourceToNumber"		,0,164, M4CL_VM1_COMPILER_JIT		,""		,(pmfVM0)&ClVM1CompilerAndJIT::clcSourceToNumber,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("clcSourceToString"		,0,165, M4CL_VM1_COMPILER_JIT		,""		,(pmfVM0)&ClVM1CompilerAndJIT::clcSourceToString,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("clcRestoreSource"		,0,166, M4CL_VM1_COMPILER_JIT		,""		,(pmfVM0)&ClVM1CompilerAndJIT::clcRestoreSourceAfterLoad,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("IsDeleted"			    ,0,167, M4CL_VM1_REGISTER		,""		,(pmfVM0)&ClVM1Register::IsDeleted,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("IsNew"		        	,0,168, M4CL_VM1_REGISTER		,""		,(pmfVM0)&ClVM1Register::IsNew,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("IsUpdated"			    ,0,169, M4CL_VM1_REGISTER		,""		,(pmfVM0)&ClVM1Register::IsUpdated,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("AppRunDate"			,0,170, M4CL_VM1_LIB		,""		,(pmfVM0)&ClVM1BasicLib::AppRunDate,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("AppEndDate"			,0,171, M4CL_VM1_LIB		,""		,(pmfVM0)&ClVM1BasicLib::AppEndDate,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("AppStartDate"			,0,172, M4CL_VM1_LIB		,""		,(pmfVM0)&ClVM1BasicLib::AppStartDate,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    
    AddFunction("curExchangeRecord"		,-2,173	, M4CL_VM1_PAYROLL	,"RR"		,(pmfVM0)&ClVM1PayrollLib::curExchangeRecord,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("curGetExchange"	    ,5,174, M4CL_VM1_PAYROLL		,"RRRRR"		,(pmfVM0)&ClVM1PayrollLib::curGetExchange,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("ExecuteSQL"	    	,-(M4CL_MAX_PARAM+1),175, M4CL_VM1_LDB		,""		,(pmfVM0)&ClVM1LDB::ExecuteSQL,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED|M4CL_FLAG_FN_NO_RETURN, M4_TRUE);
    AddFunction("ExecuteDirectSQL"	    ,-2                 ,176, M4CL_VM1_LDB		,"RR"	,(pmfVM0)&ClVM1LDB::ExecuteDirectSQL,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED|M4CL_FLAG_FN_NO_RETURN, M4_TRUE);

    AddFunction("Sort"	    	        ,-(M4CL_MAX_PARAM+1),177, M4CL_VM1_SORT_FILTER	,""	,(pmfVM0)&ClVM1FilterSort::Sort,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("BSearch"	            ,-1     ,178, M4CL_VM1_SORT_FILTER	,"R"	,(pmfVM0)&ClVM1FilterSort::BSearch,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    
    AddFunction("Length" 	                ,1	,179, M4CL_VM0_STRING_MANIP	,"R"	,(pmfVM0)&ClVM0String::Length,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Trim" 	                    ,2	,180, M4CL_VM0_STRING_MANIP	,"RR"	,(pmfVM0)&ClVM0String::Trim,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("ConvertCase" 	            ,2	,181, M4CL_VM0_STRING_MANIP	,"RR"	,(pmfVM0)&ClVM0String::ConvertCase,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Replace" 	                ,3	,182, M4CL_VM0_STRING_MANIP	,"RRR"	,(pmfVM0)&ClVM0String::Replace,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("IndexOf" 	                ,3	,183, M4CL_VM0_STRING_MANIP	,"RRR"	,(pmfVM0)&ClVM0String::IndexOf,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("LastIndexOf" 	            ,-2	,184, M4CL_VM0_STRING_MANIP	,"RR"	,(pmfVM0)&ClVM0String::LastIndexOf,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("DxYear" 	                ,1	,185, M4CL_VM0_COMPATIBILITY_DX	,"R"	,(pmfVM0)&ClVM0dx::DxYear,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxCompare" 	            ,2	,186, M4CL_VM0_COMPATIBILITY_DX	,"RR"	,(pmfVM0)&ClVM0dx::DxCompare,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxDay" 	                ,1	,187, M4CL_VM0_COMPATIBILITY_DX	,"R"	,(pmfVM0)&ClVM0dx::DxDay,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxDaysOfMonth" 	        ,2	,188, M4CL_VM0_COMPATIBILITY_DX	,"RR"	,(pmfVM0)&ClVM0dx::DxDaysOfMonth,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxDate" 	                ,3	,189, M4CL_VM0_COMPATIBILITY_DX	,"RRR"	,(pmfVM0)&ClVM0dx::DxDate,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxDate30" 	                ,3	,190, M4CL_VM0_COMPATIBILITY_DX	,"RRR"	,(pmfVM0)&ClVM0dx::DxDate30,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxDateIso" 	            ,1	,191, M4CL_VM0_COMPATIBILITY_DX	,"R"	,(pmfVM0)&ClVM0dx::DxDateIso,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxDateIsoD" 	            ,1	,192, M4CL_VM0_COMPATIBILITY_DX	,"R"	,(pmfVM0)&ClVM0dx::DxDateIsoD,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxDateIsoTS" 	            ,1	,193, M4CL_VM0_COMPATIBILITY_DX	,"R"	,(pmfVM0)&ClVM0dx::DxDateIsoTS,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxInc" 	                ,1	,194, M4CL_VM0_COMPATIBILITY_DX	,"R"	,(pmfVM0)&ClVM0dx::DxInc,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxMonth" 	                ,1	,195, M4CL_VM0_COMPATIBILITY_DX	,"R"	,(pmfVM0)&ClVM0dx::DxMonth,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxRoundCent" 	            ,1	,196, M4CL_VM0_COMPATIBILITY_DX	,"R"	,(pmfVM0)&ClVM0dx::DxRoundCent,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxRound" 	                ,1	,197, M4CL_VM0_COMPATIBILITY_DX	,"R"	,(pmfVM0)&ClVM0dx::DxRound,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxRound50" 	            ,1	,198, M4CL_VM0_COMPATIBILITY_DX	,"R"	,(pmfVM0)&ClVM0dx::DxRound50,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxTruncaCent" 	            ,1	,199, M4CL_VM0_COMPATIBILITY_DX	,"R"	,(pmfVM0)&ClVM0dx::DxTruncaCent,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("clcRestoreAfterCompile"    ,0	,200, M4CL_VM1_COMPILER_JIT	,""		,(pmfVM0)&ClVM1CompilerAndJIT::clcRestoreAfterCompile,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("Chr"					    ,1	,201, M4CL_VM0_STRING_MANIP	,"R"	,(pmfVM0)&ClVM0String::Chr,				        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Ascii"					    ,1	,202, M4CL_VM0_STRING_MANIP	,"R"	,(pmfVM0)&ClVM0String::Ascii,			        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("ExecuteRealSQL"			,-1 ,203, M4CL_VM1_LDB	,"R"	,(pmfVM0)&ClVM1LDB::ExecuteRealSQL,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED|M4CL_FLAG_FN_NO_RETURN, M4_TRUE);
    AddFunction("BeginDBTransaction"		,0 ,204 , M4CL_VM1_LDB    ,""	,(pmfVM0)&ClVM1LDB::BeginDBTransaction,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);
    AddFunction("EndDBTransaction"			,1 ,205	, M4CL_VM1_LDB    ,"R"	,(pmfVM0)&ClVM1LDB::EndDBTransaction,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);

    AddFunction("GetSliceMode"		        ,0 ,206, M4CL_VM_OBJECT_THIS	    ,""	,(pmfVM0)&ClVMachine1::GetSliceMode,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("SetSliceMode"			    ,1 ,207, M4CL_VM_OBJECT_THIS	    ,"R",(pmfVM0)&ClVMachine1::SetSliceMode,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("SetInserted"       		,0 ,208	, M4CL_VM1_REGISTER    ,""	,(pmfVM0)&ClVM1Register::SetInserted,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("clcExecuteJIT" 	        ,-1	,209, M4CL_VM1_COMPILER_JIT	,"R"	,(pmfVM0)&ClVM1CompilerAndJIT::clcExecuteJIT,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("clcExecuteLN4JIT" 	        ,-1	,210, M4CL_VM1_COMPILER_JIT	,"R"	,(pmfVM0)&ClVM1CompilerAndJIT::clcExecuteLN4JIT,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("clcPrepareJIT" 	        ,-2	,211, M4CL_VM1_COMPILER_JIT	,"Rr"	,(pmfVM0)&ClVM1CompilerAndJIT::clcPrepareJIT,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("clcReleaseJIT"				,1	,212, M4CL_VM1_COMPILER_JIT	,"R"	,(pmfVM0)&ClVM1CompilerAndJIT::clcReleaseJIT,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("IsUpdatedDB"			    ,0  ,213, M4CL_VM1_REGISTER	,""			,(pmfVM0)&ClVM1Register::IsModifiedByDDBB,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("SetValuePriority"   		,-5	,214, M4CL_VM1_INDIRECTION	,"RRRRR"	,(pmfVM0)&ClVM1Indirections::SetValuePriority,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("GetChannelId"   			,0	,215, M4CL_VM1_CMCR	,""	,		(pmfVM0)&ClVM1Mcr::GetChannelId,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetThisNodeId"				,0	,216, M4CL_VM1_CMCR	,""	,		(pmfVM0)&ClVM1Mcr::GetThisNodeId,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeNumberOfItems" 		,2	,217, M4CL_VM1_CMCR	,"RR",		(pmfVM0)&ClVM1Mcr::GetNodeNumberOfItems,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeIndex"				,1	,218, M4CL_VM1_CMCR	,"R",		(pmfVM0)&ClVM1Mcr::GetNodeIndex,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemId" 			,3	,219, M4CL_VM1_CMCR	,"RRR",		(pmfVM0)&ClVM1Mcr::GetNodeItemId,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemType" 			,-2	,220, M4CL_VM1_CMCR	,"RR",		(pmfVM0)&ClVM1Mcr::GetNodeItemType,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemScope" 			,-2	,221, M4CL_VM1_CMCR	,"RR",		(pmfVM0)&ClVM1Mcr::GetNodeItemScope,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemM4Type"			,-2	,222, M4CL_VM1_CMCR	,"RR",		(pmfVM0)&ClVM1Mcr::GetNodeItemM4Type,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemPrecision"		,-2	,223, M4CL_VM1_CMCR	,"RR",		(pmfVM0)&ClVM1Mcr::GetNodeItemPrecision,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemIndex"			,-2	,224, M4CL_VM1_CMCR	,"RR",		(pmfVM0)&ClVM1Mcr::GetNodeItemIndex,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("OpenChannel"				,4	,225, M4CL_VM_OBJECT_THIS	,"RRRR"	,(pmfVM0)&ClVMachine2::OpenChannel,			        M4CL_SEMANTIC_FUNC2, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("CloseChannel"				,1	,226, M4CL_VM_OBJECT_THIS	,"R"	,(pmfVM0)&ClVMachine2::CloseChannel,			    M4CL_SEMANTIC_FUNC2, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

 	AddFunction("Pivot"				        ,-3	,227, M4CL_VM1_PAYROLL	,"RRR"	,(pmfVM0)&ClVM1PayrollLib::pvPivot,			    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	
    AddFunction("GetChannelValue"	        ,-4	,228, M4CL_VM1_INDIRECTION	,"RRRR"		,(pmfVM0)&ClVM1Indirections::GetChannelValue,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("SetChannelValue"	        ,-5	,229, M4CL_VM1_INDIRECTION	,"RRRRR"	,(pmfVM0)&ClVM1Indirections::SetChannelValue,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("ChannelCall"		        ,-3	,230, M4CL_VM1_INDIRECTION	,"RRR"		,(pmfVM0)&ClVM1Indirections::ChannelCall, 		    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("DumpUserInfo"		        ,-1	,231, M4CL_VM0_ERROR_IO		,"R"	,(pmfVM0)&ClVM0ErrorIO::DumpUserInfo, 		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DumpUserError"		        ,-1	,232, M4CL_VM0_ERROR_IO		,"R"	,(pmfVM0)&ClVM0ErrorIO::DumpUserError, 		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("sttSetLabel"		        ,1	,233, M4CL_VM1_LIB	,"R"	,(pmfVM0)&ClVM1BasicLib::sttSetLabel, 		    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("Div"		               ,2	,234, M4CL_VM0_GENERAL	,"RR"	,(pmfVM0)&ClVM0General::Div, 		        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Increment"	               ,1	,M4CL_VM_FT_INCREMENT, M4CL_VM_OBJECT_THIS	,"R"	,(pmfVM0)&ClVMachine0::Increment,	        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("CrossTab"	               ,-3	,236, M4CL_VM1_PAYROLL	,"RRR"	,(pmfVM0)&ClVM1PayrollLib::pvCrossTab,	        M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("YTDSearch"					,12	,237, M4CL_VM1_PAYROLL	,"RRRRRRRRRRRR",		(pmfVM0)&ClVM1PayrollLib::YTDSearch,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	

	AddFunction("Load_blk"					,0	,238, M4CL_VM1_LDB	,"",	(pmfVM0)&ClVM1LDB::Load,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);	
	AddFunction("Load_prg"					,0	,239, M4CL_VM1_LDB	,"",	(pmfVM0)&ClVM1LDB::Load_prg,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);	
	AddFunction("Delete_blk"				,0	,240, M4CL_VM1_LDB	,"",	(pmfVM0)&ClVM1LDB::Delete_blk,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);	
	AddFunction("Delete_prg"				,0	,241, M4CL_VM1_LDB	,"",	(pmfVM0)&ClVM1LDB::Delete_prg,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);	
	AddFunction("Update_blk"				,0	,242, M4CL_VM1_LDB	,"",	(pmfVM0)&ClVM1LDB::Update_blk,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);	
	AddFunction("Update_prg"				,0	,243, M4CL_VM1_LDB	,"",	(pmfVM0)&ClVM1LDB::Update_prg,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);	
	AddFunction("Insert_blk"				,0	,244, M4CL_VM1_LDB	,"",	(pmfVM0)&ClVM1LDB::Insert_blk,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);	
	AddFunction("Insert_prg"				,0	,245, M4CL_VM1_LDB	,"",	(pmfVM0)&ClVM1LDB::Insert_prg,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);	
	AddFunction("Persist_tree"				,0	,246, M4CL_VM1_LDB	,"",	(pmfVM0)&ClVM1LDB::Persist_tree,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);	

	AddFunction("GetAutoloadMode"			,0	,247, M4CL_VM1_LIB	,"",	(pmfVM0)&ClVM1BasicLib::GetAutoloadMode,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	
	AddFunction("SetAutoloadMode"			,1	,248, M4CL_VM1_LIB	,"R",	(pmfVM0)&ClVM1BasicLib::SetAutoloadMode,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	

	AddFunction("DefineInstance"			,4	,249, M4CL_VM1_COMPILER_JIT	,"RRRR",(pmfVM0)&ClVM1CompilerAndJIT::DefineInstance,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	
	
	AddFunction("curGetDecimals"		    ,1	,250, M4CL_VM1_PAYROLL	,"R"	,(pmfVM0)&ClVM1PayrollLib::curGetDecimals, 		    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("CopyRegister"				,-2	,251, M4CL_VM1_REGISTER	,"RR"	,(pmfVM0)&ClVM1Register::CopyRegister,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	
	AddFunction("CloneRegister"				,0	,252, M4CL_VM1_REGISTER	,""		,(pmfVM0)&ClVM1Register::CloneRegister,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	
    AddFunction("ReadFile"          		,3	,253, M4CL_VM0_ERROR_IO   ,"RRr"	,(pmfVM0)&ClVM0ErrorIO::ReadFile,  		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("GetArgument"		        ,1	,254, M4CL_VM_OBJECT_THIS	,"R"	,(pmfVM0)&ClVMachine1::GetArgument, 		    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("GetArgumentNumber"		    ,0	,255, M4CL_VM_OBJECT_THIS	,"" 	,(pmfVM0)&ClVMachine1::GetArgumentNumber,	    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("TodayNow"          		,0	,256 , M4CL_VM0_GENERAL   ,""		,(pmfVM0)&ClVM0General::TodayNow,  		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("AddFilter"			    	,-1	,257, M4CL_VM1_SORT_FILTER	,"R"	,(pmfVM0)&ClVM1FilterSort::AddFilter,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	
	AddFunction("RemoveFilter"				,0	,258, M4CL_VM1_SORT_FILTER	,""		,(pmfVM0)&ClVM1FilterSort::RemoveFilter,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	
	AddFunction("CleanFilter"				,0	,259, M4CL_VM1_SORT_FILTER	,""		,(pmfVM0)&ClVM1FilterSort::CleanFilter,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	

	AddFunction("ChannelAttribGetValue"		,4	,260, M4CL_VM1_INDIRECTION	,"RRRR"		,(pmfVM0)&ClVM1Indirections::GetChannelAttribValue,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	
	AddFunction("ChannelAttribCall"			,-4	,261, M4CL_VM1_INDIRECTION	,"RRRR"	,    (pmfVM0)&ClVM1Indirections::ChannelAttribCall,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	

	AddFunction("RefreshFilter"				,0	,262, M4CL_VM1_SORT_FILTER	,""		,(pmfVM0)&ClVM1FilterSort::RefreshFilter,					M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	

	AddFunction("GetErrorCode"				,-1	,263, M4CL_VM0_ERROR_IO		,"R"	,(pmfVM0)&ClVM0ErrorIO::GetErrorCode,			M4CL_SEMANTIC_FUNC, 0, M4_FALSE);	
	AddFunction("PurgeErrorCode"			,1	,264, M4CL_VM0_ERROR_IO		,"R"	,(pmfVM0)&ClVM0ErrorIO::PurgeErrorCode,			M4CL_SEMANTIC_FUNC, 0, M4_FALSE);	
    AddFunction("OpenFile"          		,2	,265, M4CL_VM0_ERROR_IO    ,"Rr"	,(pmfVM0)&ClVM0ErrorIO::OpenFile,  				M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("CloseFile"          		,1	,266, M4CL_VM0_ERROR_IO    ,"R"		,(pmfVM0)&ClVM0ErrorIO::CloseFile,  		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("GetFilterArgument"   		,2	,267, M4CL_VM1_SORT_FILTER    ,"RR"	,(pmfVM0)&ClVM1FilterSort::GetFilterArgument,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("SetFilterArgument"    		,-3	,268 , M4CL_VM1_SORT_FILTER   ,"RRR",(pmfVM0)&ClVM1FilterSort::SetFilterArgument,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("GetNodeAutoloadMode"		,0	,269, M4CL_VM1_LIB	,"",	(pmfVM0)&ClVM1BasicLib::GetNodeAutoloadMode,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	
	AddFunction("SetNodeAutoloadMode"		,1	,270, M4CL_VM1_LIB	,"R",	(pmfVM0)&ClVM1BasicLib::SetNodeAutoloadMode,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	

	AddFunction("RemoteCall"		  ,0	,271, M4CL_VM_OBJECT_THIS	,"",	(pmfVM0)&ClVMachine1::CreateRemoteCall,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("GetLog"	    	  ,6	,272, M4CL_VM0_ERROR_IO	,"Rrrrrr",	(pmfVM0)&ClVM0ErrorIO::GetLog,			M4CL_SEMANTIC_FUNC, 0, M4_FALSE);	
	AddFunction("SetLog"	    	  ,-4	,273, M4CL_VM0_ERROR_IO	,"RRRR",	(pmfVM0)&ClVM0ErrorIO::SetLog,			M4CL_SEMANTIC_FUNC, 0, M4_FALSE);	
	AddFunction("PurgeLog"	    	  ,1	,274, M4CL_VM0_ERROR_IO	,"R",		(pmfVM0)&ClVM0ErrorIO::PurgeLog,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);	
	AddFunction("GetLogSize"	      ,0	,275, M4CL_VM0_ERROR_IO	,"",		(pmfVM0)&ClVM0ErrorIO::GetLogSize,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);	
	AddFunction("GetErrorString"      ,-3	,276, M4CL_VM1_LIB		,"RRR",		(pmfVM0)&ClVM1BasicLib::GetErrorString,	M4CL_SEMANTIC_FUNC1, 0, M4_FALSE);	

	AddFunction("LookUp"		      ,-7	,277, M4CL_VM1_LIB	,"RRRRRRRR",(pmfVM0)&ClVM1BasicLib::LookUp,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	

	AddFunction("GetNodeItemScale"			,-2,278, M4CL_VM1_CMCR	,"RR",	(pmfVM0)&ClVM1Mcr::GetNodeItemScale,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemInternalType"	,-2,279, M4CL_VM1_CMCR	,"RR",	(pmfVM0)&ClVM1Mcr::GetNodeItemInternalType,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemReadObjectAlias",-2,280, M4CL_VM1_CMCR	,"RR",	(pmfVM0)&ClVM1Mcr::GetNodeItemReadObjectAlias,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemWriteObjectAlias",-2,281, M4CL_VM1_CMCR,"RR",	(pmfVM0)&ClVM1Mcr::GetNodeItemWriteObjectAlias,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemReadFieldId"	,-2,282, M4CL_VM1_CMCR	,"RR",	(pmfVM0)&ClVM1Mcr::GetNodeItemReadFieldId,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemWriteFieldId"	,-2,283, M4CL_VM1_CMCR	,"RR",	(pmfVM0)&ClVM1Mcr::GetNodeItemWriteFieldId,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemReadObjectId"	,-2,284, M4CL_VM1_CMCR	,"RR",	(pmfVM0)&ClVM1Mcr::GetNodeItemReadObjectId,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemWriteObjectId"	,-2,285, M4CL_VM1_CMCR	,"RR",	(pmfVM0)&ClVM1Mcr::GetNodeItemWriteObjectId,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("AssignFixedArgumentInVarArgsItem"	,0,M4CL_VM_FT_ASSIGNFIXEDARGUMENTINVARARGSITEM, M4CL_VM_OBJECT_THIS	,"",	(pmfVM0)&ClVMachine1::AssignFixedArgumentInVarArgsItem,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("ReadFixedArgumentInVarArgsItem"	,0,M4CL_VM_FT_READFIXEDARGUMENTINVARARGSITEM, M4CL_VM_OBJECT_THIS	,"",	(pmfVM0)&ClVMachine1::ReadFixedArgumentInVarArgsItem,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("AddSortFilter"							,-1	,288, M4CL_VM1_SORT_FILTER,"R",	(pmfVM0)&ClVM1FilterSort::AddSortFilter,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("Unload"			,0	,289	, M4CL_VM_OBJECT_THIS,""	,(pmfVM0)&ClVMachine2::UnloadChannel				,M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("UndoRegister"		,0	,290	, M4CL_VM1_REGISTER,""	,(pmfVM0)&ClVM1Register::UndoRegister			,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("UndoBlock"			,0	,291	, M4CL_VM1_REGISTER,""	,(pmfVM0)&ClVM1Register::UndoBlock				,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("UndoNode"			,0	,292	, M4CL_VM1_REGISTER,""	,(pmfVM0)&ClVM1Register::UndoNode				,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("UndoChannel"		,0	,293	, M4CL_VM1_REGISTER,""	,(pmfVM0)&ClVM1Register::UndoChannel			,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("CheckPointRegister"	,0	,294	, M4CL_VM1_REGISTER,""	,(pmfVM0)&ClVM1Register::CheckPointRegister		,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("CheckPointBlock"		,0	,295	, M4CL_VM1_REGISTER,""	,(pmfVM0)&ClVM1Register::CheckPointBlock		,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("CheckPointNode"		,0	,296	, M4CL_VM1_REGISTER,""	,(pmfVM0)&ClVM1Register::CheckPointNode			,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("CheckPointChannel"		,0	,297	, M4CL_VM1_REGISTER,""	,(pmfVM0)&ClVM1Register::CheckPointChannel		,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("SetPriority"		,-4	,298	, M4CL_VM1_INDIRECTION,"RRRR"	,(pmfVM0)&ClVM1Indirections::SetPriority		,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("curGetExchangeCurrency"		,5	,299	, M4CL_VM1_PAYROLL,"RRRRR"	,(pmfVM0)&ClVM1PayrollLib::curGetCurrencyExchange	,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("SendRegisterMark"		,-(M4CL_MAX_PARAM+1)	,300	, M4CL_VM1_REGISTER,""	,(pmfVM0)&ClVM1Register::SendRegisterMark	,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("SendBlockMark"			,-(M4CL_MAX_PARAM+1)	,301	, M4CL_VM1_REGISTER,""	,(pmfVM0)&ClVM1Register::SendBlockMark		,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("SetCSTimeOut"			,2	,302	, M4CL_VM1_REGISTER,"RR"	,(pmfVM0)&ClVM1Register::SetCSTimeOut			,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetCSTimeOut"			,1	,303	, M4CL_VM1_REGISTER,"R"		,(pmfVM0)&ClVM1Register::GetCSTimeOut			,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("ConfigCSTimeOut"		,-1	,304	, M4CL_VM1_REGISTER,"R"		,(pmfVM0)&ClVM1Register::ConfigCSTimeOut		,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("SetCSTimeoutFunction"	,2	,305	, M4CL_VM1_REGISTER,"RR"	,(pmfVM0)&ClVM1Register::SetCSTimeoutFunction	,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("Convert"          		,1	,306	, M4CL_VM0_GENERAL,"R"		,(pmfVM0)&ClVM0General::Convert,  		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("ReadComment"			,0	,M4CL_VM_FT_READCOMMENT	, M4CL_VM_OBJECT_THIS,""	,(pmfVM0)&ClVMachine0::ReadComment,		M4CL_INTERNAL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("IsDataCacheable"		,0	,308, M4CL_VM_OBJECT_THIS,""	,(pmfVM0)&ClVMachine2::IsDataCacheable,		M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    
    AddFunction("GetServerValue"		,2	,309	, M4CL_VM0_GENERAL,"RR"		,(pmfVM0)&ClVM0General::GetServerValue,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("BitwiseOr"				,2	,310	, M4CL_VM0_GENERAL,"RR"		,(pmfVM0)&ClVM0General::BitwiseOr,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("BitwiseNot"			,1	,311	, M4CL_VM0_GENERAL,"R"		,(pmfVM0)&ClVM0General::BitwiseNot,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("BitwiseAnd"			,2	,312	, M4CL_VM0_GENERAL,"RR"		,(pmfVM0)&ClVM0General::BitwiseAnd,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("BitwiseXor"			,2	,313	, M4CL_VM0_GENERAL,"RR"		,(pmfVM0)&ClVM0General::BitwiseXor,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    
    AddFunction("ClearFiles"			,0	,314	, M4CL_VM0_ERROR_IO,""		,(pmfVM0)&ClVM0ErrorIO::ClearFiles,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("sttGetLevel"		    ,0	,315, M4CL_VM1_LIB	,""	,(pmfVM0)&ClVM1BasicLib::sttGetLevel, 		    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("sttSetLevel"		    ,1	,316, M4CL_VM1_LIB	,"R"	,(pmfVM0)&ClVM1BasicLib::sttSetLevel, 		    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("GetVariantType"		,1	,317, M4CL_VM0_GENERAL,"R"		,(pmfVM0)&ClVM0General::GetVariantType,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("DumpStatistics"        ,-2	,318, M4CL_VM1_REGISTER,"RR"	,(pmfVM0)&ClVM1Register::DumpStatistics,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("GetRegistryValue"		,2	,319	, M4CL_VM0_GENERAL,"RR"		,(pmfVM0)&ClVM0General::GetRegistryValue,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("clcOldTIToIDTI"		,0	,320	, M4CL_VM1_COMPILER_JIT	,""		,(pmfVM0)&ClVM1CompilerAndJIT::clcOldTIToIDTI,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("TodayGMT"	            	,0	,321	, M4CL_VM0_GENERAL	,""			,(pmfVM0)&ClVM0General::TodayGMT,		        M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("TodayNowGMT"         		,0	,322	, M4CL_VM0_GENERAL   ,""		,(pmfVM0)&ClVM0General::TodayNowGMT,  		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("curExchangeRecordRounded"		,-2,323	, M4CL_VM1_PAYROLL	,"RR"		,(pmfVM0)&ClVM1PayrollLib::curExchangeRecordRounded,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("curGetExchangeRounded"	    ,5,324, M4CL_VM1_PAYROLL		,"RRRRR"		,(pmfVM0)&ClVM1PayrollLib::curGetExchangeRounded,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("curGetCurrencyExchangeRounded"		,5	,325	, M4CL_VM1_PAYROLL,"RRRRR"	,(pmfVM0)&ClVM1PayrollLib::curGetCurrencyExchangeRounded	,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("ExecuteBranch"             ,-2	,326, M4CL_VM1_INDIRECTION, "RR", 	     (pmfVM0)&ClVM1Indirections::ExecuteBranch,   M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("sttPersistToFile"		    ,1	,327, M4CL_VM1_LIB	,"R"	,(pmfVM0)&ClVM1BasicLib::sttPersistToFile, 		    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	
	
	AddFunction("GetNodeItemPrecisionEx"	,-3	,328, M4CL_VM1_CMCR	,"rRR", (pmfVM0)&ClVM1Mcr::GetNodeItemPrecisionEx,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemArgumentPos"	,-3	,329, M4CL_VM1_CMCR	,"RRR", (pmfVM0)&ClVM1Mcr::GetNodeItemArgumentPos,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("GetNodeItemArgumentNumber"	,-5	,330, M4CL_VM1_CMCR	,"rrrRR", (pmfVM0)&ClVM1Mcr::GetNodeItemArgumentNumber,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeItemArgument"		,-7	,331, M4CL_VM1_CMCR	,"rrrrRRR", (pmfVM0)&ClVM1Mcr::GetNodeItemArgument,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("GetUniqueID"				,0	,332, M4CL_VM0_GENERAL,""		,(pmfVM0)&ClVM0General::GetUniqueID,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("ConvertOneWay"        		,1	,333	, M4CL_VM0_GENERAL,"R"		,(pmfVM0)&ClVM0General::ConvertOneWay,  		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("AddAsPartner"					,0	,334	, M4CL_VM_OBJECT_THIS,""	,(pmfVM0)&ClVMachine2::AddAsFriend,			M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("RemoveAsPartner"				,0	,335	, M4CL_VM_OBJECT_THIS,""	,(pmfVM0)&ClVMachine2::RemoveAsFriend,		M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	
	AddFunction("SetProperty"					,-2	,336	, M4CL_VM_OBJECT_THIS,"RR"	,(pmfVM0)&ClVMachine2::SetProperty,			M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetProperty"					,1	,337	, M4CL_VM_OBJECT_THIS,"R"	,(pmfVM0)&ClVMachine2::GetProperty, 		M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("SendNodeMark"		,-(M4CL_MAX_PARAM+1)	,338	, M4CL_VM1_REGISTER,""	,(pmfVM0)&ClVM1Register::SendNodeMark	,M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("YTDSearchFiltered"				,-12 ,339, M4CL_VM1_PAYROLL	,"RRRRRRRRRRRR",		(pmfVM0)&ClVM1PayrollLib::YTDSearchFiltered,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	

	AddFunction("Rnd"							,1 ,340, M4CL_VM0_GENERAL	,"R",			(pmfVM0)&ClVM0General::Rnd,			M4CL_SEMANTIC_FUNC, 0, M4_FALSE);	

    AddFunction("GetRunningStartDate"   		,0	,341, M4CL_VM_OBJECT_THIS	,   ""	,(pmfVM0)&ClVMachine0::GetRunningStartDate,     		M4CL_SEMANTIC_FUNC, 0, M4_FALSE); 
    AddFunction("GetRunningEndDate"   		    ,0	,342, M4CL_VM_OBJECT_THIS	,   ""	,(pmfVM0)&ClVMachine0::GetRunningEndDate,     		M4CL_SEMANTIC_FUNC, 0, M4_FALSE); 
    AddFunction("GetRunningRunDate"   		    ,0	,343, M4CL_VM_OBJECT_THIS	,   ""	,(pmfVM0)&ClVMachine0::GetRunningRunDate,     		M4CL_SEMANTIC_FUNC, 0, M4_FALSE); 

    AddFunction("MatchRegExp"		   		    ,-2	,344, M4CL_VM0_STRING_MANIP	,"RR"		,(pmfVM0)&ClVM0String::MatchRegex,       	    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("LoadFromFile"					,-1	,345, M4CL_VM_OBJECT_THIS,"R"	,(pmfVM0)&ClVMachine2::LoadChannelFromFile,			M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("PersistToFile",-(M4CL_MAX_PARAM+1) ,346, M4CL_VM_OBJECT_THIS,""	,(pmfVM0)&ClVMachine2::PersistChannelToFile,		M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("DestroyFile"          			,1	,347, M4CL_VM0_ERROR_IO    ,"R"	,(pmfVM0)&ClVM0ErrorIO::DestroyFile,  				M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
	AddFunction("CheckModify",  -(M4CL_MAX_PARAM+1) ,348, M4CL_VM_OBJECT_THIS,""	,(pmfVM0)&ClVMachine2::CheckModify,					M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("CommonTimeStamp"				,4	,349, M4CL_VM0_GENERAL	,"RRRR"		,(pmfVM0)&ClVM0General::CommonTimestamp,  		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DateAdd"						,-3	,350, M4CL_VM0_GENERAL	,"RRR"		,(pmfVM0)&ClVM0General::DateAdd,  		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DateDiff"						,-3	,351, M4CL_VM0_GENERAL	,"RRR"		,(pmfVM0)&ClVM0General::DateDiff,  		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("GetNewNotPersist"	            ,0  ,352, M4CL_VM1_REGISTER	,""	 ,(pmfVM0)&ClVM1Register::GetNewNotPersist,			M4CL_SEMANTIC_FUNC1, 0, M4_FALSE);
    AddFunction("SetNewNotPersist" 		        ,1  ,353, M4CL_VM1_REGISTER ,"R" ,(pmfVM0)&ClVM1Register::SetNewNotPersist,     	M4CL_SEMANTIC_FUNC1, 0, M4_FALSE); 

	AddFunction("OpenChannelEx"					,-4	,354, M4CL_VM_OBJECT_THIS	,"RRRR"	,(pmfVM0)&ClVMachine2::OpenChannelEx,        M4CL_SEMANTIC_FUNC2, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("DefineInstanceEx"				,-4	,355, M4CL_VM1_COMPILER_JIT	,"RRRR",(pmfVM0)&ClVM1CompilerAndJIT::DefineInstanceEx,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	

    AddFunction("KeyConvert"            		,1	,356, M4CL_VM0_GENERAL,"R"		,(pmfVM0)&ClVM0General::KeyConvert,  		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("EraseAllL2Instances"			,0	,357, M4CL_VM_OBJECT_THIS	,"",(pmfVM0)&ClVMachine2::EraseAllL2Instances,			M4CL_SEMANTIC_FUNC2, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	
	AddFunction("ResolveAlias"					,0	,358, M4CL_VM1_COMPILER_JIT	,"",(pmfVM0)&ClVM1CompilerAndJIT::ResolveAlias,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	

    AddFunction("DataConvert"          		,1	,359	, M4CL_VM0_GENERAL,"R"		,(pmfVM0)&ClVM0General::DataConvert,  		    M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("Log"          				,-1	,360	, M4CL_VM0_GENERAL,"R"		,(pmfVM0)&ClVM0General::Log,  					M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("GetNodeReadObjectId"	,1,361, M4CL_VM1_CMCR	,"R",	(pmfVM0)&ClVM1Mcr::GetNodeReadObjectId,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("GetNodeWriteObjectId"	,1,362, M4CL_VM1_CMCR	,"R",	(pmfVM0)&ClVM1Mcr::GetNodeWriteObjectId,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("ResetExecuteGroup"		    ,1	,363, M4CL_VM_OBJECT_THIS	,"R"	,(pmfVM0)&ClVMachine1::ResetExecuteGroup,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE); 

	AddFunction("GetNodeItemSliceTotalize" 		,-2	,364, M4CL_VM1_CMCR	,"RR",		(pmfVM0)&ClVM1Mcr::GetNodeItemSliceTotalize,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("ExistNodeItemIndex"			,-2	,365, M4CL_VM1_CMCR	,"RR",		(pmfVM0)&ClVM1Mcr::ExistNodeItemIndex,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("GetNodeItemData"		,-3,366, M4CL_VM1_CMCR	,"RRR",	(pmfVM0)&ClVM1Mcr::GetItemData,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("GetHandle"				,0	,367, M4CL_VM_OBJECT_THIS,""	,(pmfVM0)&ClVMachine2::GetHandle,			M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("InsertChannel"			,2	,368, M4CL_VM_OBJECT_THIS	,"RR"	,(pmfVM0)&ClVMachine2::InsertChannel,	M4CL_SEMANTIC_FUNC2, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("IsBlockUpdated"		,1	,369, M4CL_VM1_REGISTER		,"R"	,(pmfVM0)&ClVM1Register::IsBlockUpdated,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("IsNodeUpdated"			,1	,370, M4CL_VM1_REGISTER		,"R"	,(pmfVM0)&ClVM1Register::IsNodeUpdated,		M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("IsNewAndDeleted"	    ,0	,371, M4CL_VM1_REGISTER		,""		,(pmfVM0)&ClVM1Register::IsNewAndDeleted,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("CallNode"	    		,-2	,372, M4CL_VM1_INDIRECTION	,"RR"	,(pmfVM0)&ClVM1Indirections::CallNode,	    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("GetChannelNumberOfNodes"	,-(M4CL_MAX_PARAM+1),373, M4CL_VM1_CMCR	,""		,(pmfVM0)&ClVM1Mcr::GetChannelNumberOfNodes,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("GetNodeData"	    		,-1					,374, M4CL_VM1_CMCR	,"R"	,(pmfVM0)&ClVM1Mcr::GetNodeData,				M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("Date30Ex"	            ,-1	,375, M4CL_VM0_GENERAL	,"R"			,(pmfVM0)&ClVM0General::Date30Ex,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("DxDate30Ex"            ,-3	,376, M4CL_VM0_COMPATIBILITY_DX	,"RRR"	,(pmfVM0)&ClVM0dx::DxDate30Ex,			M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("MergeSentences"         ,2	,377, M4CL_VM1_LDB	,"RR"	,(pmfVM0)&ClVM1LDB::MergeSentences,		M4CL_SEMANTIC_FUNC1, 0, M4_FALSE);

    AddFunction("TickCount"	            ,0	,378, M4CL_VM0_GENERAL	,""		,(pmfVM0)&ClVM0General::TickCount,		M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("Sleep"		            ,1	,379, M4CL_VM0_GENERAL	,"R"	,(pmfVM0)&ClVM0General::vmSleep,			M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("ReplaceLiterals"  		,-1						,380, M4CL_VM1_LIB,	"R"			,(pmfVM0)&ClVM1BasicLib::ReplaceLiterals, 	    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("CleanLiterals"  		,-(M4CL_MAX_PARAM+1)	,381, M4CL_VM1_LIB,	""			,(pmfVM0)&ClVM1BasicLib::CleanLiterals, 	    M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("GetBookMark"			,1	,382	, M4CL_VM_OBJECT_THIS, "R"	,(pmfVM0)&ClVMachine2::GetBookMark,		M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("ApplyBookMark"			,1	,383	, M4CL_VM_OBJECT_THIS, "R"	,(pmfVM0)&ClVMachine2::ApplyBookMark,	M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	AddFunction("ReleaseBookMark"		,1	,384	, M4CL_VM_OBJECT_THIS, "R"	,(pmfVM0)&ClVMachine2::ReleaseBookMark,	M4CL_CHANNEL_FUNC, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("ConvertPassword"      	,2	,385	, M4CL_VM0_GENERAL,"RR"		,(pmfVM0)&ClVM0General::ConvertPassword,      M4CL_SEMANTIC_FUNC, 0, M4_FALSE);
    AddFunction("ComparePassword"      	,5	,386	, M4CL_VM0_GENERAL,"RRRRr"	,(pmfVM0)&ClVM0General::ComparePassword,      M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

    AddFunction("ChannelCallAs"		    ,-4	,387	, M4CL_VM1_INDIRECTION	,"RRRR"		,(pmfVM0)&ClVM1Indirections::ChannelCallAs, M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
	
    AddFunction("GetChannelData"	 	,-1	,388	, M4CL_VM1_CMCR	,"R"	,(pmfVM0)&ClVM1Mcr::GetChannelData,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("DBTransactionNumber"	,0  ,389	, M4CL_VM1_LDB   ,""	,(pmfVM0)&ClVM1LDB::DBTransactionNumber,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_TRUE);

    AddFunction("LoadXML"				,-8 ,390	, M4CL_VM1_LIB   ,"RRRRRRRR"	,(pmfVM0)&ClVM1BasicLib::LoadXML,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("ExportXML"				,-9 ,391	, M4CL_VM1_LIB   ,"RRRRRRRRR"	,(pmfVM0)&ClVM1BasicLib::ExportXML,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("SetLoadRange"			,-2 ,392	, M4CL_VM1_LDB   ,"RR"	,(pmfVM0)&ClVM1LDB::SetLoadRange,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("CopyBranch"			,-1 ,393	, M4CL_VM1_REGISTER   ,"r"		,(pmfVM0)&ClVM1Register::CopyBranch,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
    AddFunction("PasteBranch"			,-4 ,394	, M4CL_VM1_REGISTER   ,"RRRR"	,(pmfVM0)&ClVM1Register::PasteBranch,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("SetLoadTimeOut"		,-1 ,395	, M4CL_VM1_LDB   ,"R"	,(pmfVM0)&ClVM1LDB::SetLoadTimeOut,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("ChannelAttribGetSliceValue"	,5	,396, M4CL_VM1_INDIRECTION	,"RRRRR"	,(pmfVM0)&ClVM1Indirections::GetChannelAttribSliceValue,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);	

	AddFunction("clcOldItemToIDItem"	,0	,397	, M4CL_VM1_COMPILER_JIT	,""	,(pmfVM0)&ClVM1CompilerAndJIT::clcOldItemToIDItem,	M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

    AddFunction("Compare2FAPassword"	,6	,398	, M4CL_VM0_GENERAL,"RRRRrr"	,(pmfVM0)&ClVM0General::Compare2FAPassword, M4CL_SEMANTIC_FUNC, 0, M4_FALSE);

	AddFunction("GetThisItemId"			,0	,399	, M4CL_VM1_CMCR	,""	, (pmfVM0)&ClVM1Mcr::GetThisItemId, M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("GetListInfo", -5, 400, M4CL_VM1_CMCR, "RRrrr", (pmfVM0)&ClVM1Mcr::GetListInfo, M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);

	AddFunction("GetRuleData"		,-4,401, M4CL_VM1_CMCR	,"RRRR",	(pmfVM0)&ClVM1Mcr::GetRuleData,			M4CL_SEMANTIC_FUNC1, M4CL_FLAG_FN_LOCAL_TEST_DISABLED, M4_FALSE);
}



//*****************************
//Acceso Datos de las funciones
//*****************************
m4return_t ClFuncTable::GetIdChannel(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc){
	
	return ClFuncTableDesc::GetInstance()->GetIdChannel (ai_name, ao_Id, ao_NumArgs, ao_ParamDesc);
}


m4return_t ClFuncTable::GetIdSemantic(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc){
	return ClFuncTableDesc::GetInstance()->GetIdSemantic(ai_name, ao_Id, ao_NumArgs, ao_ParamDesc);
}


m4return_t ClFuncTable::GetId(const m4char_t *ai_name, m4uint16_t &ao_Id, m4int8_t &ao_NumArgs, m4char_t *ao_ParamDesc,  m4int8_t &ao_iType) {

	return ClFuncTableDesc::GetInstance()->GetId(ai_name, ao_Id, ao_NumArgs, ao_ParamDesc, ao_iType);
}

m4return_t ClFuncTable::GetId(const m4char_t *ai_name, m4uint16_t &ao_Id) {

	return ClFuncTableDesc::GetInstance()->GetId(ai_name, ao_Id);
}

m4return_t ClFuncTable::GetChannelId(const m4char_t *ai_name, m4uint16_t &ao_Id) {

	return ClFuncTableDesc::GetInstance()->GetChannelId(ai_name, ao_Id);
}

ClFunctionDesc&  ClFuncTable::GetObjectFunction(m4uint16_t ai_id) {
	return ClFuncTableDesc::GetInstance()->GetObjectFunction(ai_id);
}

m4bool_t	 ClFuncTable::HasReferedArgs (m4uint16_t ai_id){
	return ClFuncTableDesc::GetInstance()->HasReferedArgs(ai_id);
}

m4uint16_t	 ClFuncTable::GetNumReferedArgs (m4uint16_t ai_id) {
	return ClFuncTableDesc::GetInstance()->GetNumReferedArgs(ai_id);
}
m4int8_t  ClFuncTable::GetType (m4uint16_t ai_id){
	return ClFuncTableDesc::GetInstance()->GetType(ai_id);
};

//**********Obtener la función ejecutable
m4return_t ClFuncTable::GetExeFunc(m4uint16_t ai_Id, m4return_t(ClVVM::*&ao_pfFunc)(void), m4int8_t &ao_NumArg ) {
	itVectorFunciones it;
	
	it=oTable.begin()+ai_Id;
	if (it>=oTable.end()) {
		return M4_ERROR;
	}
	if ( (*it).GetId() !=ai_Id){

#ifdef _DEBUG
		DUMP_CHLOG_DEBUG(M4_CH_VM_UNSORTED_FUNCTION_TABLE);
#endif
		for (it=oTable.begin(); it<oTable.end(); it++){
			if ((*it).GetId()==ai_Id) {
				ao_pfFunc=(*it).GetExeFunc();
				ao_NumArg=GetObjectFunction(ai_Id).GetNumArgs();
				return M4_SUCCESS;
			}
		}
		return M4_ERROR;

	}
	ao_pfFunc=(*it).GetExeFunc();
	ao_NumArg=GetObjectFunction(ai_Id).GetNumArgs();
	return M4_SUCCESS;
}


