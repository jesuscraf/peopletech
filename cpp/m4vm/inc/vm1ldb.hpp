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
//    Virtual machine de nivel 1 funciones de LDB
////
//==============================================================================

#ifndef _ClVM1LDB_H
#define _ClVM1LDB_H

class ClAccess;
class ClLConn_Interface;
class ClVMachine1;

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"


class ClVM1LDB : public ClVVM{
	
	const m4VMState_t &m_oState1;
	ClVMachine1		*m_poVM;
public:

	ClVM1LDB (const m4VMState_t &ai_oState):	m_oState1(ai_oState)
	{
		
	}


	void SetVM (ClVMachine1 *ai_poVM) {m_poVM = ai_poVM;}
	 //-----------------------
    //ldb
    //-----------------------

    m4return_t SysLoad          (void);
	m4return_t Load             (void);
	m4return_t ExtendedLoad     (void);

	m4return_t Write            (void);
    
	m4return_t Delete           (void);

    m4return_t Persist           (void);
    m4return_t PersistAll        (void);
    m4return_t PersistNode       (void);
    m4return_t PersistNodeAll    (void);


	m4return_t BeginTransaction (void);
	m4return_t EndTransaction   (void);

	ClLConn_Interface   *GetConnectEnv    (ClAccess *ai_pAccess, m4bool_t ai_bNewConn=M4_TRUE);
	
    m4return_t GetConnect      (void);
    m4return_t FreeConnect     (void);

    m4return_t Unload           (void);
    
	m4return_t ExecuteSQL       (void);
    m4return_t ExecuteDirectSQL (void);
    m4return_t ExecuteRealSQL   (void);

    m4return_t BeginDBTransaction (void);
    m4return_t EndDBTransaction (void);
    m4return_t DBTransactionNumber (void);

    

    //new persistence model
    m4return_t Load_prg                 (void) ;

    m4return_t Delete_blk               (void) ;
    m4return_t Delete_prg               (void) ;
    
    m4return_t Update_blk               (void) ;
    m4return_t Update_prg               (void) ;
    
    m4return_t Insert_blk               (void) ;
    m4return_t Insert_prg               (void) ;

    m4return_t Persist_tree             (void);

	m4return_t MergeSentences			(void);

    m4return_t SetLoadRange		        (void) ;
    m4return_t SetLoadTimeOut	        (void) ;


private:
	//para persist
	m4return_t CallPersistenceItem_blk (m4uint32_t ai_hItem);
//    m4return_t _Persist (const m4char_t *ai_pszMethodName, const m4char_t *ai_pszWriteMethod, const m4char_t *ai_pszDeleteMethod);
//    m4return_t _PersistNode (const m4char_t *ai_pszMethodName, const m4char_t *ai_pszWriteMethod, const m4char_t *ai_pszDeleteMethod);

	
};

#endif
