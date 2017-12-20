//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                ExeFurep.h
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
//    Declaración de la clase ExeFuncRepository que contiene el "ejecutable" 8metodo" de las funciones de la semantica...
////
//==============================================================================



#ifndef _ClVMachine0_H
#define _ClVMachine0_H

#include "m4stl.hpp"
#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"
#include "fixstackvm.hpp" //es un template
#include "medvarin.hpp"
#include "vmlabel.hpp"


//implementors
#include "vm0dx.hpp"
#include "vm0str.hpp"
#include "vm0errorio.hpp"
#include "vm0general.hpp"

//stepper
#include "vmstepper.hpp"

//------------------------------------------------------
//   Unas macros para hacer una pila de depuración
//------------------------------------------------------

class ClDecodeFormula;
class ClDecodeInstruction;
class ClAccess;
class ClStringPool;
class ClStaticPoolStr;
class ClFuncTable;
class ClStatus;
class ClVMCSContext;
class ClRunningFormInfo;


// Exclusión de funciones
typedef	set<m4uint16_t, less<m4uint16_t> >	ExcludeFunctionsSet_t ;
typedef ExcludeFunctionsSet_t::iterator		itExcludeFunctionsSet_t ;



class ClVMachine0:public ClVVM{
	
public: 

    ClVMachine0(tpFixedStack<m4VariantType> *ai_oRunTimeStack=0,
				ClStringPool *ai_oDynamicPool=0,
				tpFixedStack<m4VMState_t>	*ai_oStateStack=0,
				ClVMCSContext *ai_poVMCSContext=0);

	virtual void Init(tpFixedStack<m4VariantType> *ai_oRunTimeStack,
				ClStringPool *ai_oDynamicPool,
				tpFixedStack<m4VMState_t>	*ai_oStateStack,
				ClVMCSContext *ai_poVMCSContext);

	virtual	void SetContext	(ClVMCSContext *ai_poContext);
	
	virtual ~ClVMachine0();


	//--------
	//ejecutar
	//--------
	m4return_t Execute(void);

	//--------------------------
	//Funciones para el run-time
	//--------------------------

	m4return_t ReadComment(void);
	m4return_t ReadVar(void);
	m4return_t PushNum(void);
	m4return_t PushDate(void);
	m4return_t PushString(void);
	m4return_t PurgeStack(void);
	m4return_t AssignVar(void);
	m4return_t ConditionalJump(void);
	m4return_t InconditionalJump(void);
	m4return_t Return(void);
	

	//aritmeticas
	m4return_t LogicalAnd(void);
	m4return_t LogicalOr(void);
	m4return_t LogicalNot(void);
	m4return_t LogicalEqual(void);
	m4return_t LogicalNotEqual(void);
	m4return_t LogicalGreater(void);
	m4return_t LogicalGreaterEqual(void);
	m4return_t LogicalLess(void);
	m4return_t LogicalLessEqual(void);
	
	//Operaciones aritmeticas básicas
	m4return_t AritmeticalAdd(void);
	m4return_t AritmeticalSub(void);
	m4return_t AritmeticalMul(void);
	m4return_t AritmeticalDiv(void);
	m4return_t AritmeticalChangeSign(void);
    m4return_t Increment(void);
	
	//librería de LN4
	
	m4return_t DumpPool			(void);
    m4return_t  NullFunction    (void){
        RuntimePushDouble(M4_SUCCESS);
        return M4_SUCCESS;
    }
    m4return_t IsVariantNull    (void);
    m4return_t NullValue        (void);

    
    m4return_t SetStartDate (void);
    m4return_t SetEndDate (void);
    m4return_t SetRunDate (void);

    m4return_t GetStartDate (void);
    m4return_t GetEndDate (void);
    m4return_t GetRunDate (void);

	//Fechas en las que se está ejecutando el estado, indep. de las fechas del access
    m4return_t GetRunningStartDate (void);
    m4return_t GetRunningEndDate (void);
    m4return_t GetRunningRunDate (void);

	virtual void RefreshState(void);

    //EDU:
    virtual m4uint32_t GetSize (void) const;
    
	m4bool_t	GetCancelStatus	(void);
	void		SetpCancelStatus(ClStatus* ai_poCancelStatus = NULL);

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
	m4bool_t GetRequestToAbortWithClStatus(void);

	m4uint32_t m_iInstructionsCounter;

	//-----------------------
	//Argumentos
	//-----------------------
	//para leer los argumentos y el número de argumentos, son llamadas por las de ln4 y por el ClExecutor
	m4uint32_t GetRunningItemArgumentNumber			(const m4VMState_t &ai_oState, m4int32_t ai_iRef);
	m4return_t GetRunningItemArgumentByPosition		(const m4VMState_t &ai_oState, m4int32_t ai_iRef, m4uint32_t ai_iPosition, m4VariantType &ao_vArg);

   	//----------------------------------------
	//Runtime info - stepper
	//----------------------------------------
	m4return_t GetVariableValue (m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ao_vValue);
	m4return_t SetVariableValue (m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ai_vValue);
	m4uint32_t GetNumberOfVariables (m4uint32_t ai_iCallStackPos);

	m4return_t GetArgumentValue (m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ao_vValue);
	m4return_t SetArgumentValue (m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ai_vValue);
	m4uint32_t GetNumberOfArguments (m4uint32_t ai_iCallStackPos);

	m4uint32_t GetCallStackSize (void);
	m4return_t GetCallStackInfo (m4uint32_t ai_iCallStackPos, ClRunningFormInfo &ao_oInfo);

   	//----------------------------------------
	// Exclusión de funciones
	// Se tiene un set con la lista de funciones a excluir
	// Se añade una función que ejecuta comprobando el set
	//----------------------------------------
	m4return_t	SetExcludeFunction		( m4pcchar_t ai_pccFunction ) ;
	m4return_t	UnsetExcludeFunction	( m4pcchar_t ai_pccFunction ) ;
	m4return_t	ClearExcludeFunctions	( void ) ;
	m4return_t	ExecuteFunction			( m4uint16_t ai_iFunction, ClVVM **ai_ppoImplementors, m4VMState_t& ai_roState ) ;


private:	
	m4VMState_t m_oState;
	ClDecodeInstruction *m_oCurrentIns;
	
	m4bool_t		m_bGoOn; //Para saber cuando acabamos

	/* Bug 0163438
	Para indicar si ha habido renetrada de nivel 0 y por tanto ha cambiado el estado
	*/
	m4bool_t		m_bStateChanged;

	m4bool_t		m_bRequestAbortion; //por si alguien nos quiere matar
	m4return_t		AbortAll(void);
	
	//--------------
	//impelemtadores
	//--------------
	ClVVM			*m_oImplementors[M4CL_VM0_N_OBJECTS];

	ClVM0dx			m_oImplDx; //para las funciones de compatibilidad con 2x
	ClVM0String		m_oImplString; //para las funciones de cadenas
	ClVM0ErrorIO	m_oImplErrorIO; //para las funciones de error e IO
	ClVM0General	m_oImplGeneral; //funcioens de librería generales

	ClStatus		*m_poCancelStatus;


	m4return_t _GetVariableValue (m4int32_t ai_iRef, m4int32_t ai_ivar, m4VariantType &ao_vValue);
	m4return_t _SetVariableValue (m4int32_t ai_iRef, m4int32_t ai_ivar, m4VariantType &ai_vValue);

protected:

		
	//--------------------------
	// Status Labels
	//--------------------------
	ClVMStatusLabel m_oStatusLabel;
	ClFuncTable					*m_poFunctionTable;
	
   	//----------------------------------------
	// Exclusión de funciones
   	//----------------------------------------
	ExcludeFunctionsSet_t		*m_poExcludeFunctions ;

	m4bool_t					m_bDynPoolOwner;
	m4bool_t					m_bStateStackOwner;
	m4bool_t					m_bRuntimeStackOwner;

    m4int32_t                  m_BeginTransaction;

	m4return_t	LocateStateAndRef(m4uint32_t ai_iCallStackPos, m4VMState_t &ao_oState, m4int32_t &ao_iRef);

public:
    //funciones de la librería para las transacciones de VM
    m4return_t BeginVMTransaction(void);
    m4return_t EndVMTransaction(void);


    //Transacciones de VM 
	void BeginVMTrans(){
			if (m_BeginTransaction==M4CL_NO_VM_TRANSACTION_DEFINED ){
            m_BeginTransaction=m_oStateStack->GetPos();
		}
		

    }
    void EndVMTrans(void){
        m_BeginTransaction=M4CL_NO_VM_TRANSACTION_DEFINED; //antes ponía -1
    }
    
	//funciones especiales para el manejos desde el cliente administrador

	m4return_t RequestAbort(m4bool_t ai_bRequest=M4_TRUE) {
		m_bRequestAbortion=ai_bRequest;
		return M4_SUCCESS;
	};
	m4return_t SetStatusLabel(m4pcchar_t ai_pszLabel){ 
		return m_oStatusLabel.SetLabel(ai_pszLabel);
	};
	m4return_t GetStatusLabel(m4pchar_t ao_pszLabel, m4uint32_t ai_iSize){
		return m_oStatusLabel.GetLabel(ao_pszLabel, ai_iSize);
	};

	m4return_t ExecuteFunctionForLocalTest (m4uint16_t iNumFunction); //para que desde el local test puedan ejecutar funciones

	ClVMStepper *GetpStepper(void)
	{
		return &m_oStepper;
	}

	void	SetStepperStates( void ) ;
	void	ResetStepper( void ) ;

protected:
	
	friend class ClVMItemAttribute;       //vamos...que me vais a matar, es que todo son deque disgustos
	
	ClVMStepper  m_oStepper;			  //Step by step, hold to...

	//--------------------------------------------------------------------------
	//functions for Check Execution
	//--------------------------------------------------------------------------

#ifdef _DEBUG
	m4bool_t	MarkBeginExec(m4uint16_t	ai_ui16NumFunc, m4int8_t &ao_iNumArgRef, m4int32_t &ao_iPosBeforeExec );
	m4return_t	CheckEndExec (m4uint16_t	ai_ui16NumFunc, m4int8_t ai_iNumArgRef, m4int32_t ai_iPosBeforeExec, m4return_t ai_ret, m4bool_t ai_bCheckExec);
#endif

#if _M4_DEBUG > 2
	#define DUMP_RTS()	m_oRunTimeStack->Dump()	
#else
    #define DUMP_RTS()		
#endif

#ifdef _M4_TRACE_POOL
	friend class ClStringPool;
#endif

};


#endif
