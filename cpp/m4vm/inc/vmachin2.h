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



#ifndef _ClVMachine2_H
#define _ClVMachine2_H

#include "vmachin1.h" //la clase base
#include "clgeniod.hpp" // To Serialize
#include "m4mdrt.hpp" // To Serialize
#include "m4vm_dll.hpp"

class ClCompiledMCR;
class ClExecutor;

class M4_DECL_M4VM ClVMachine2: public ClVMachine1{
public:
	ClVMachine2(tpFixedStack<m4VariantType> *ai_oRunTimeStack=0,
				ClStringPool *ai_oDynamicPool=0,
				tpFixedStack<m4VMState_t>	*ai_oStateStack=0,
				ClVMCSContext				*ai_poVMCSContext=0);

	virtual ~ClVMachine2();

    virtual m4uint32_t GetSize (void) const;
	
	m4return_t Execute (void);

	//Bug:0063126
	// El número de argumentos estaba definido como m4int8_t, con lo cual no se podía pasar de 128 argumentos
	m4return_t Execute (m4VMState_t ai_oState,m4uint16_t ai_iNumArgs);

	//funciones propias del nivel 2
	m4return_t ExecuteItem(void);
	m4return_t AssignItem(void);
	m4return_t PushItem(void);
	m4return_t ReadAttribute(void);
	m4return_t ExecuteAttribute(void);
	
	//funcion de ejecutar funciones de canal ...Nivel 2
	m4return_t ExecuteChannelFunction(void);

	m4return_t OpenChannel (void);
	m4return_t OpenChannelEx (void);
	m4return_t CloseChannel (void);
	m4return_t InsertChannel (void);

	m4return_t EraseAllL2Instances (void);

	//------------------------------------
	//Bookmarks
	//------------------------------------
	m4return_t GetBookMark      (void);
	m4return_t ApplyBookMark    (void);
	m4return_t ReleaseBookMark  (void);

	//funcioens de canal
	m4return_t LoadChannel(void);
	m4return_t PersistChannel(void);
	m4return_t ReleaseChannel(void);
 	m4return_t UnloadChannel(void);
	m4return_t IsDataCacheable (void);
	m4return_t AddAsFriend (void);
	m4return_t RemoveAsFriend (void);
	m4return_t GetProperty (void);
	m4return_t SetProperty (void);
	m4return_t GetHandle (void);

	m4return_t LoadChannelFromFile (void);
	m4return_t PersistChannelToFile (void);

	m4return_t CheckModify (void);

	// Functions for serialization >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD, ClChannelManager * ai_pChannelManager);
	m4uint32_t GetNumAccessToSerialize(void);
	m4return_t GetAccessToSerialize (ClAccess **ai_ppAcesss, m4uint32_t ai_iNumAccess);

   // Functions for serialization <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	m4return_t ExecuteFunctionForLocalTest (m4uint16_t iNumFunction); //para que desde el local test puedan ejecutar funciones
	
	virtual void RefreshState(void);
	virtual void RefreshReenter(m4return_t ai_rRet);

	// User Actions Auditory.
	m4return_t	RegisterUserAuditEvents() ;
private:
	m4VMState_t					m_oState2;
	ClDecodeInstruction		   *m_oCurrentIns2;
	ClVVM	*m_oImplementors[M4CL_VM2_N_OBJECTS];

	m4return_t	GetContext		( ClAccess ** ao_pAccess, m4uint32_t& ao_hNodo,  m4uint32_t& ao_hItem );
	m4return_t	GetAccessContext( ClAccess ** ao_pAccess);
	m4return_t  _OpenChannel	(ClChannel *ai_poChannel, const m4pchar_t ai_pszInstance, 
								 const m4pchar_t ai_pszChannelId, m4uint8_t ai_iScope,
								 m4uint8_t ai_iOpenMode, m4uint8_t ai_iCSAccessType, 
								 const m4pchar_t ai_pcOrg, m4uint8_t ai_iOrgType);
 

	friend class ClExecutor;

};
#endif

