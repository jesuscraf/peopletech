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
//    Virtual machine de nivel 1 funciones de compiler y JIT
////
//==============================================================================

#ifndef _ClVM1COMP_JIT_H
#define _ClVM1COMP_JIT_H

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"
#include "cpdefine.hpp"



class ClTrad;
class IntLN4;
class ClVMachine1;
class ClFuncTable;
class ClStaticPoolStr;
class ClFinder;
class ClFinder_TICH;
class ClFinder_JIT;
class ClInstruction;
class ClAccess;
class ClAccessRecordSet;

class ClVM1CompilerAndJIT : public ClVVM{
	
	const m4VMState_t &m_oState1;
	ClVMachine1		*m_poVM;
public:

	ClVM1CompilerAndJIT (const m4VMState_t &ai_oState);
	~ClVM1CompilerAndJIT ();
	void SetVM (ClVMachine1 *ai_poVM) {m_poVM = ai_poVM;}

	//----------------------------------------
	//Compiler
	//----------------------------------------
    m4return_t clcCompile		(void);
	m4return_t clcGetPolish		(void);
	m4return_t clcCheckSyntax	(void);
    m4return_t LocalTest        (void);
	m4return_t clcSourceToId	(void);
	m4return_t clcSourceToNumber(void);
	m4return_t clcSourceToString(void);
	m4return_t clcOldItemToIDItem(void);
	m4return_t clcRestoreSourceAfterLoad	(void);
	m4return_t clcRestoreAfterCompile	(void);
	m4return_t clcOldTIToIDTI	(void);
	m4return_t ResolveAlias		(void);	// Para reglas heredadas.
	
	m4return_t  CompileBlock (void);

	//para compilar JIT
	m4return_t clcExecuteLN4JIT (void);
	m4return_t clcExecuteJIT (void);
	m4return_t clcPrepareJIT (void);
	m4return_t clcReleaseJIT (void);
	m4return_t DefineInstance(void);
	m4return_t DefineInstanceEx(void);
	m4return_t InstallRTGC	(ClInstruction *ai_pInstructions, m4uint32_t ai_iNumInstructions,  m4uint32_t *ai_piInsTypes, m4uint32_t ai_iInsTypesNum);

	m4return_t GetSymbolTable(ClItem *ai_poItem, m4char_t *ai_pszLN4Code, m4char_t **ai_ppszArguments, m4uint32_t ai_iNumArgs,m4char_t **ai_ppszSymbols, m4uint32_t ai_iNumSymbols, m4uint32_t ai_iSymbolSize);

    virtual m4uint32_t GetSize (void) const;
private:
	m4return_t ExecuteJIT	(m4pchar_t ai_CodeSerial,m4VariantType *ai_avArgsValue,m4int32_t iNumRealArgs,m4bool_t bPrepare);
	m4return_t CompileJIT	(m4pchar_t ai_pszLN4Code,m4VariantType *ai_avArgsName,m4int32_t iNumRealArgs,m4pchar_t &ao_pszPolish);
	m4return_t InstallJIT	(m4pchar_t ai_pszPolish,m4pchar_t &ao_CodeSerial);
	m4return_t RestoreSource(m4bool_t ai_bTypeofRestore=M4_RESTORE_AFTER_LOAD);

	m4return_t	InitTrad(m4uint8_t ai_Type,ClTrad *&ai_pTrad);
	void		FreeTrad(ClTrad *ai_pTrad);
	ClFinder	*GetFinder(m4uint8_t ai_Type,ClTrad *ai_pTrad);


	//compile
	m4return_t	_clcCheckSyntax(Compile_Type ai_cSource=M4_NO_SOURCE);
   //---------------------------------
    //    Prueba local
    //---------------------------------
    m4return_t _LocalTest (m4char_t *ai_pszSourceCode, m4char_t **ai_pszContext,ClTrad * ai_pTrad);
 
	//---------------------------------
	//Instancias L2 en JIT
	//---------------------------------
	m4bool_t		_DefineInstance (ClChannel *ai_poChannel, const m4char_t *ai_pszInstance, const StInstChannJIT_t &ai_stNewInstance);


   ClTrad			*m_poTrad;
   m4bool_t			m_IsFreeTrad;

   IntLN4           *m_poInterpreter;
   ClFuncTable		*m_poFunctionTable;

   ClStaticPoolStr				*m_oUpperPool;
   ClStaticPoolStr				*m_oNoUpperPool;

	//---------------------------------
	//    Finders para compilacion
	//---------------------------------
   
   ClFinder_TICH	*m_poFinderTICH;
   ClFinder_JIT		*m_poFinderJIT;


};

#endif


