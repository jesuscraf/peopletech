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
//    class executor, a vmachine wrapper
//
//
//==============================================================================
#ifndef __EXECUTOR_HPP__
#define __EXECUTOR_HPP__

#include "m4vm_dll.hpp"

#include "iexecutor.hpp"
#include "stack.hpp"


class ClCompiledMCR;
class ClGenericIODriver;
class ClItem;
class ClAccess;
class ClDecodeInstrucion;
class ClExecutorRuntimeInfo ;
class ClAccessRecordSet;
class ClChannelManager;
struct m4VMState_t;
class ClDecodeInstruction; 
class ClVMachine2;
class ClVMStepper;
class ClRunningFormInfo;
// cancel in server
class ClStatus;
class ClItem;
class ClBreakpoint;
class ClCMCRFactory;


class M4_DECL_M4VM ClExecutor: public IExecutor
{
public:
    ClExecutor ();
	virtual ~ClExecutor ();

	m4return_t Init (ClVMCSContext *ai_poContext1, ClVMCSContext *ai_poContext2=0) ;   

    
	// Functions for serialization 
    m4return_t Serialize				(ClGenericIODriver& IOD);
    m4return_t DeSerialize				(ClGenericIODriver& IOD, ClChannelManager * ai_pChannelManager);

	m4uint32_t GetNumAccessToSerialize	(void);
	m4return_t GetAccessToSerialize		(ClAccess **ai_ppAcesss, m4uint32_t ai_iNumAccess);

	m4return_t GetExecutionDate			(m4VariantType &ao_var);
	m4return_t GetStartDate				(m4VariantType &ao_var);
	m4return_t GetEndDate				(m4VariantType &ao_var);

	//Para la ejecución
	m4bool_t	IsRunning				(void);
	m4bool_t	IsRunning				(m4VMState_t &ao_State);
	m4return_t	Call					(ClItem *ao_poItem,m4int32_t ai_iArgs=-1, m4bool_t ai_bEnableDebug=M4_FALSE, m4bool_t ai_bNoInsiteRec=M4_FALSE); 
	m4return_t	Abort					(void);
	m4return_t	Continue				(void);
	m4return_t	StepInto				(void);
	m4return_t	StepOver				(void);
	m4return_t	StepOut					(void);
	m4return_t	RunToCursor				(ClBreakpoint &ai_oBP, ClCMCRFactory *ai_poFactory);
	m4return_t	Execute					(void);

	// Para los breakpoints
	m4int32_t	AddBreakPoint			(      ClBreakpoint &ai_oBP, ClCMCRFactory *ai_poFactory);
	m4int32_t	DeleteBreakPoint		(      ClBreakpoint &ai_oBP, ClCMCRFactory *ai_poFactory);
	m4int32_t	ToggleBreakPoint		(      ClBreakpoint &ai_oBP, ClCMCRFactory *ai_poFactory);
	m4int32_t	NextValidBreakPoint		(const ClBreakpoint &ai_oBP, ClCMCRFactory *ai_poFactory);
	
	//para el stepper-controladores de ejecución
	ClVMStepper *GetpStepper			(void);

	//replica del interfaz de vmachine en lo relacionado a stepper (desde aquí solo se llama a vmachine, tal cual)
	m4return_t GetVariableValue			(m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ao_vValue);
	m4return_t SetVariableValue			(m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ai_vValue);
	m4uint32_t GetNumberOfVariables		(m4uint32_t ai_iCallStackPos);

	m4return_t GetArgumentValue			(m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ao_vValue);
	m4return_t SetArgumentValue			(m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ai_vValue);
	m4uint32_t GetNumberOfArguments		(m4uint32_t ai_iCallStackPos);

	m4uint32_t GetCallStackSize			(void);
	m4return_t GetCallStackInfo			(m4uint32_t ai_iCallStackPos, ClRunningFormInfo &ao_oInfo);

	m4return_t GetSymbolTable			(ClItem *ai_poItem, m4char_t *ai_pszCode, m4char_t **ao_ppszSymbols, m4uint32_t ai_iSizeTable, m4uint32_t ai_iSizeSymbol); //ojo. asigna/libera el que llama


	//level de execution
	m4uint8_t		GetLevel			(void);
	//args. del item ejecutandose
	m4bool_t		IsVariableArgs		(void);
	m4uint8_t		GetNumberOfArguments(void);
	m4VariantType	GetArgument			(m4uint8_t ai_iPos);
	m4return_t		SetArgument			(m4VariantType ai_vValue ,m4uint8_t ai_iPos);

	//info del item
	m4pcchar_t		GetItemId			(void);
	m4pcchar_t		GetNodeId			(void);
	m4pcchar_t		GetChannelId		(void);

	//Sobre lo que estamos ejecutando
	m4uint16_t		GetOperation		(void);
	m4uint16_t		GetRuleNumber		(void);
	m4bool_t		HasSlice			(void);
	m4bool_t		HasRegister			(void);

	m4return_t		RequestAbort		(m4bool_t ai_bRequest=M4_TRUE); 
	m4return_t		SetStatusLabel		(m4pcchar_t ai_pszLabel);
	m4return_t		GetStatusLabel		(m4pchar_t ao_pszLabel, m4uint32_t ai_iSize);
	m4uint32_t		GetSize				(void) const;
	
	void			Reset				(void);
	void			ResetStack			(void);
	void			ResetDebugging		(void);

	string			DumpCallStack		(void);//Volcar pila de llamadas de la VM
	m4return_t		GetRTInfo			(ClExecutorRuntimeInfo *ai_poRTInfo);
	void			SetpCancelStatus	(ClStatus *ai_poCancelStatus = NULL);
	m4bool_t		GetCancelStatus		(void);

	ClVMStatisticsWrapper *GetSttWrap	(void);

	/**
	 * Petición para abortar una ejecución.
	 * Es distinto del RequestToAbort porque tiene en cuenta si tenemos asignado un ClStatus
	 * (por ejemplo en el caso de estar ejecutando en el servidor). En tal caso la petición para
	 * abortar se hace a través de este objeto.
	 *
	 * @param ai_bRequestToAbort    M4_TRUE  --> se pide que se aborte la ejecución.
	 *								M4_FALSE --> se pide que la ejecución continue con normalidad.
	 */
	void RequestToAbortWithClStatus(const m4bool_t ai_bRequestToAbort);

	/**
	 * Nos devuelve si ha sido abortada una ejecución.
	 */
	m4bool_t GetRequestToAbortWithClStatus(void) const;	

   	//----------------------------------------
	// Exclusión de funciones
	//----------------------------------------
	m4return_t	SetExcludeFunction		( m4pcchar_t ai_pccFunction ) ;
	m4return_t	UnsetExcludeFunction	( m4pcchar_t ai_pccFunction ) ;
	m4return_t	ClearExcludeFunctions	( void ) ;


private:

	ClDecodeInstruction*	GetInstruction	(m4VMState_t &ai_State);
	m4uint32_t				GetItemHandleByIds (ClAccess *ai_poAccess, ClDecodeInstruction* ai_poIns);

	
	m4bool_t				m_bInit;
    ClVMachine2             *m_poVMachine;
	ClStackInterface        m_Stack;

	ClVMCSContext			*m_poContext1;
	ClVMCSContext			*m_poContext2;

	// Para saber si hemos hecho un callback
	m4uint32_t				m_iInsideCallBack;

	m4return_t	_Continue(void);

};


#endif // __EXECUTOR_HPP__

