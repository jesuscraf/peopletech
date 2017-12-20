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

#ifndef _ClVMachine1_H
#define _ClVMachine1_H


#include "vmachine.h" //la clase base
#include "cldstack.hpp"


//implementadores
#include "vm1prop.hpp"
#include "vm1ldb.hpp"
#include "vm1ind.hpp"
#include "vm1compjit.hpp"
#include "vm1filtsort.hpp"
#include "vm1cmcr.hpp"
#include "vm1reg.hpp"
#include "vm1lib.hpp"
#include "vm1payroll.hpp"
#include "sttlib.hpp"
#include "sttwrap.hpp"
#include "vmcscontext.hpp"
#include "usractaudit.hpp" // User actions auditory.



//utilidades
class ClDbgUtil;
class ClItem;
class ClValue;
class VMItemAttribute;
class ClRecordSet;
class ClAccessRecordSet;
class ClVMStatistics;
class ClCompiledMCR;
class ClHandle;
class ClNoCallableItem;
class ClIndexList;
class ClVMStatisticsWrapper;
class ClVMCSContext;


typedef map<m4uint32_t, ClDateStack ,less<m4uint32_t> > MapDepStack;
typedef MapDepStack::iterator	itMapDepStack;


class ClVMachine1: public ClVMachine0{
public:
	ClVMachine1(tpFixedStack<m4VariantType> *ai_oRunTimeStack=0,
				ClStringPool *ai_oDynamicPool=0,
				tpFixedStack<m4VMState_t>	*ai_oStateStack=0,
				ClVMCSContext *ai_poVMCSContext=0);

	virtual ~ClVMachine1();
	void SetContext	(ClVMCSContext *ai_poContext);

    virtual m4uint32_t GetSize (void) const;

	//Ejecutar
	m4return_t Execute (void);

    //niveles superiores y serialize
    m4return_t GetExecStatus	(m4VMState_t *ao_oState, m4uint32_t ai_iuNumStates, m4VariantType *ao_Vars, m4uint32_t &ao_iNumArgs , m4bool_t &ao_bHasState, m4bool_t ai_bSerializeLocals, m4int32_t &ao_iRef,m4int32_t &ao_iTotalStates,m4int32_t &ao_iNumLocals);
    m4return_t RestoreExecution	(m4VMState_t *ai_oState, m4uint32_t ai_iuNumStates, m4VariantType *ai_Vars, m4uint32_t  ai_iNumArgs, m4bool_t ai_bSerializeLocals, m4int32_t ai_iRef,m4int32_t ai_iTotalStates,m4int32_t ai_iNumLocals);
private:
	m4return_t CalculateVarsToSerialize(m4uint32_t ai_iNumStates, m4uint32_t &ao_iNumVars, m4int32_t &ao_iNumLocals, m4int32_t &ao_iFrom, m4bool_t ai_bSerializeLocals); //función auxiliar para Get/Restore ExecStatus
public:
    
    //funciones propias del nivel 1
	m4return_t ExecuteItem			(void);
	m4return_t AssignItem			(void);
	m4return_t PushItem				(void);

    m4return_t ReadAttribute		(void);
	m4return_t ExecuteAttribute		(void);
    
	//para argumentos variables ( y no variables )
	m4return_t GetArgument			(void);
	m4return_t GetArgumentNumber	(void);

	//para lectuar de argumentos fijos en un item ln4 con arg. variables
	m4return_t AssignFixedArgumentInVarArgsItem	(void);
	m4return_t ReadFixedArgumentInVarArgsItem	(void);
 
    //ExecuteGroup
    m4return_t ExecuteGroup		(void);

    //ResetExecuteGroup
    m4return_t ResetExecuteGroup		(void);

	m4return_t SetSliceMode     (void);
    m4return_t GetSliceMode     (void);

	//----------------------------------------
	//Runtime info - stepper
	//----------------------------------------
	m4return_t GetSymbolTable	(ClItem *ai_poItem, m4char_t *ai_pszCode, m4char_t **ao_ppszSymbols, m4uint32_t ai_iSizeTable, m4uint32_t ai_iSizeSymbol);//ojo. asigna/libera el que llama

	//----------------------------------------
	//Llamada a métodos en remoto
	//----------------------------------------
	m4return_t  CreateRemoteCall (void);

	//----------------------------------------
	// Nivel 2.
	//----------------------------------------
	m4return_t GetL2Access ( ClChannel * ai_pChannel, m4pcchar_t ai_ChannelInstance, m4int16_t ai_bJIT, ClAccess** ao_pAccess );


	m4return_t ExecuteFunctionForLocalTest (m4uint16_t iNumFunction); //para que desde el local test puedan ejecutar funciones



	//slice mode	
    void     SetRunSliceMode (ClAccess *ai_poAccess, m4bool_t ai_SliceMode);
    m4bool_t GetRunSliceMode (ClAccess *ai_poAccess) ;



	virtual void RefreshState(void);
	virtual void RefreshReenter(m4return_t ai_rRet);
    m4return_t CheckAndCreateItemSlice (m4date_t ai_dStart, m4date_t ai_dEnd, ClItem *ai_poItem);

	/* Bug 0076620
	Hay que hacer una función que vaya asignando según las fechas de un conjunto de tramos
	Y otra que asigne prioridades
	*/
    m4return_t SetValue   ( ClItem &ai_roItem, m4VariantType &ai_vValue, priority_t ai_iPriority, m4uint8_t ai_iSource, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4bool_t ai_bCompact ) ;
    m4return_t SetPriority( ClItem &ai_roItem, priority_t ai_iPriority, m4date_t ai_dStartDate, m4date_t ai_dEndDate ) ;


private:

	m4VMState_t					m_oState1;
	ClDecodeInstruction			*m_oCurrentIns1;
	m4return_t					AllocVarSpaceIfNeeded(void);
    m4bool_t                    m_bMustKeepConceptValue;

	ClVVM						*m_oImplementors[M4CL_VM1_N_OBJECTS];


	ClVM1Propagator			m_oImplPropagator;		//propagacion
	ClVM1LDB				m_oImplLDB;				//ldb
	ClVM1Indirections		m_oImplIndirections;	//indirecciones
	ClVM1CompilerAndJIT		m_oImplCompilerAndJit;	//compilador y JIT
	ClVM1FilterSort			m_oImplFiltFind;		//filtros, ordenaciones y busquedas
	ClVM1Mcr				m_oImplMcr;				//funciones del metacanal
	ClVM1Register			m_oImplRegister;		//funciones de registro y recordset
	ClVM1BasicLib			m_oImplLib;				//funciones de librería
	ClVM1PayrollLib			m_oImplPayroll;			//funciones de nomina


    //finalización de la ejecución de un método
    m4return_t  _FreeAtEndOfMethod (void);
    //slices
    
    m4return_t	AddMethodToExecute (m4VMState_t &ai_oState ,m4VariantType ai_vSlice, m4int32_t ai_iParentStatePos, m4bool_t ai_bSpecSlice) ;
    m4return_t	AddSlicesStack		(ClAccess *ai_poAccess,m4uint32_t ai_handle);
	//m4return_t	AddSlicesStackDeep	(ClAccess *ai_poAccess,m4uint32_t ai_handle,ClDateStack &ai_oDateStackTotal);
	

    //null execution
    m4return_t  DeleteNullMethod (ClAccess *ai_poAccess,m4uint32_t ai_hItem, m4bool_t ai_bPurge ,m4bool_t ai_bPurgeRef, m4bool_t ai_bCompact ,m4date_t ai_StartDate, m4date_t ai_EndDate);

 
    
protected: 
    //crear estados de la ejecución de iun item
    
	m4return_t CreateExecute (m4VMState_t ai_oState,m4uint16_t ai_iNumArgs, m4VariantType ai_vSlice, m4bool_t ai_bSpecSlice=M4_FALSE);
	m4return_t DetSlicesOfItem   (ClAccess *ai_poAccess,m4uint32_t ai_handle, m4date_t ai_dStart,  m4date_t ai_dEnd,  m4uint16_t &aio_iNumSlices,  m4date_t* &ao_pdSlicesDates);
    m4return_t RecoverFromError (ClAccess *ai_poAccess);
    m4return_t __SetValue (ClItem &ai_item, m4VariantType &ai_var, priority_t ai_prior, m4uint8_t ai_iSource);

	//Para las estadisticas
	m4return_t GetParentItemState(m4VMState_t &ao_oState);

	//para refrescar el valor de la reentrada en continue+execute
	

	tpFixedStack<m4VariantType> *m_oResultStack; //Quitar en un futuro próximo
	tpFixedStack<m4int32_t>		*m_oReenterStack;
	m4int32_t					m_iReenter;

    ClVMItemAttribute   *m_poAttribute;
    ClDateStack			m_oDatesStack;
	MapDepStack			m_oDepStack;

	m4int32_t			m_iSerializeFrom;
    
	m4date_t            m_dUnit;


	void Reset		(void);
	//---------------------------------------
	//Declaracion de amistad- que mal rollo
	//---------------------------------------

	friend class ClVMItemAttribute;       //que disgusto
	friend class ClVM1LDB;			//para estadísticas 
	friend class ClVM1Indirections;	//para indirecciones

	//---------------------------------------
	//Para los de CS
	//---------------------------------------

	// User actions auditory.
	m4return_t	RegisterUserAuditMethodEvent( m4VMState_t ai_oState, m4uint32_t ai_iCallStackPos, m4pchar_t ai_pcOrganization ) ;

};





#endif
