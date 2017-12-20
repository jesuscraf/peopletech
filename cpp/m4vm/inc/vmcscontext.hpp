//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             executor.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             12/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    contexto del ejecutor
//
//
//==============================================================================
#ifndef CL_VM_CS_CONTEXT_HPP
#define CL_VM_CS_CONTEXT_HPP

#include "m4vm_dll.hpp"
#include "m4types.hpp"
#include "m4clock.hpp"


class ClVMStatisticsWrapper;
class ClLConn_Provider;
class ClAccess;
class IExecAdaptor;
class ClM4ObjServiceContext;
class ClAccessRecordSet;
struct m4VMState_t;
class ClExecutor;


class IM4MessageBoxProvider
{
public:
	virtual m4int_t	OnMessageBox( m4pcchar_t ai_pccTitle, m4pcchar_t ai_pccMessage, m4uint32_t ai_iType ) = 0 ;
};


class M4_DECL_M4VM ClVMCSContext{

public:
	ClVMCSContext(ClM4ObjServiceContext *ai_poObjContext ,ClVMStatisticsWrapper *ai_poSttWrap, ClLConn_Provider *ai_poConnProvider, IExecAdaptor* ai_poExecAdap, m4bool_t ai_bIsSM);
	virtual ~ClVMCSContext();

	virtual ClVMStatisticsWrapper *GetSttWrap(void){
		return m_poSttWrap;
	}
	
	virtual ClLConn_Provider	*GetConnProvider(void){
		return m_poConnProvider;
	}
	virtual IExecAdaptor*		GetExecAdaptor(void){
		return m_poExecAdap;
	}
	virtual ClM4ObjServiceContext *GetObjContext  (void){
		return m_poObjContext;
	}

	virtual	m4return_t	PreBDLMethod(ClAccess *ai_poAccess, m4uint32_t ai_hItem, const char* ai_pszFunctionName){
		return M4_FALSE; 
	}
	
	virtual m4return_t	GetServerValue (char *ai_pszPath, char *ai_pszValue, char *ao_acValue, m4uint32_t ai_iBufferLength){
		return M4_ERROR;
	}

	virtual	m4int_t		Messagebox(const char * ai_pszListArg,const char * ai_pszTitle, m4uint32_t ai_iType, m4uint32_t ai_iError, m4int16_t ai_iErrorType){
		return -1;
	}

	virtual m4return_t	RemoteCall(ClExecutor *ai_poExecutor, m4uint32_t ai_hItem, ClAccessRecordSet *ai_poRecordSet,m4bool_t ai_bEnableDebug){
		return M4_ERROR;
	};
	
	virtual m4bool_t	IsClient(void){
			return M4_FALSE;
	}
	m4bool_t			IsSM(void){
			return m_bIsSM;
	}

	virtual void NotifyVMExecution(m4bool_t ai_bIsStart){
	}

	static void RegisterMessageBoxProvider(IM4MessageBoxProvider *ai_poMessageBoxProvider);
	static void UnregisterMessageBoxProvider(IM4MessageBoxProvider *ai_poMessageBoxProvider);

protected:
	ClVMStatisticsWrapper *m_poSttWrap;
	ClLConn_Provider	  *m_poConnProvider;
	IExecAdaptor		  *m_poExecAdap;
	ClM4ObjServiceContext *m_poObjContext;
	m4bool_t			   m_bIsSM;

	static set<IM4MessageBoxProvider*, less<IM4MessageBoxProvider*> >	sm_oMessageBoxProviders;

	m4int_t	_MessageBox( m4pcchar_t ai_pccTitle, m4pcchar_t ai_pccMessage, m4uint32_t ai_iType );
};


class M4_DECL_M4VM ClVMCSContextClient: public ClVMCSContext{

public:
	ClVMCSContextClient(ClM4ObjServiceContext *ai_poObjContext ,ClVMStatisticsWrapper *ai_poSttWrap, ClLConn_Provider *ai_poConnProvider, IExecAdaptor* ai_poExecAdap, m4bool_t ai_bIsSM) 
		: ClVMCSContext(ai_poObjContext, ai_poSttWrap, ai_poConnProvider, ai_poExecAdap, ai_bIsSM){
		m_iTimeStamp = -1;
	}

	m4return_t	PreBDLMethod	(ClAccess *ai_poAccess, m4uint32_t ai_hItem, const char* ai_pszFunctionName);
	m4return_t	GetServerValue	(char *ai_pszPath, char *ai_pszValue, char *ao_acValue, m4uint32_t ai_iBufferLength);
	m4int_t		Messagebox		(const char * ai_pszListArg,const char * ai_pszTitle, m4uint32_t ai_iType, m4uint32_t ai_iError, m4int16_t ai_iErrorType);
	m4return_t	RemoteCall		(ClExecutor *ai_poExecutor, m4uint32_t ai_hItem, ClAccessRecordSet	*ai_poRecordSet,m4bool_t ai_bEnableDebug);
	m4bool_t	IsClient(void)  { return M4_TRUE;}
	void		NotifyVMExecution(m4bool_t ai_bIsStart);

	m4millisec_t	m_iTimeStamp;
};




class M4_DECL_M4VM ClVMCSContextServer: public ClVMCSContext{

public:
	ClVMCSContextServer(ClM4ObjServiceContext *ai_poObjContext ,ClVMStatisticsWrapper *ai_poSttWrap, ClLConn_Provider *ai_poConnProvider, m4bool_t ai_bIsSM) 
		: ClVMCSContext(ai_poObjContext, ai_poSttWrap, ai_poConnProvider, 0, ai_bIsSM){
	}

	m4return_t	GetServerValue	(char *ai_pszPath, char *ai_pszValue, char *ao_acValue, m4uint32_t ai_iBufferLength);
	m4int_t		Messagebox		(const char * ai_pszListArg,const char * ai_pszTitle, m4uint32_t ai_iType, m4uint32_t ai_iError, m4int16_t ai_iErrorType);
	m4return_t	RemoteCall		(ClExecutor *ai_poExecutor, m4uint32_t ai_hItem, ClAccessRecordSet *ai_poRecordSet,m4bool_t ai_bEnableDebug);
	m4bool_t	IsClient(void)  { return M4_FALSE;}
};



#endif
