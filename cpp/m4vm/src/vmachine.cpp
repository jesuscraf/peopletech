
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                ExeFurep.cpp
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
//    Implementación de la clase ExeFuncRepository que contiene el "ejecutable" 8metodo" de las funciones de la semantica...
////
//==============================================================================

#include <limits.h>

#include "vmachine.h"

#include "tablfunc.h"
#include "vmres.hpp"
#include "vmcscontext.hpp"

#include "m4mdrt.hpp"
#include "handles.hpp"
#include "index.hpp"
#include "fechas.h"
#include "clstringpool.hpp"
#include "cldates.hpp"
#include "medvarin.hpp"
#include "m4objglb.hpp"
#include "exvariables.hpp"
#include "runningforminfo.hpp"
#include "tablafuncdesc.hpp"
#include "m4unicode.hpp"
#include "nodedef.hpp"




#define M4CL_VM_FREQ_POOLING_ABORT_RQST 50


#ifdef	_M4_TRACE_POOL
extern void PoolTrace( m4pcchar_t ai_pccLabel, m4pcchar_t ai_pccString1, m4pcchar_t ai_pccString2, m4pcchar_t ai_pccString3, m4int32_t ai_iNumber1, m4int32_t ai_iNumber2 ) ;
#endif




ClVMachine0::ClVMachine0(tpFixedStack<m4VariantType> *ai_oRunTimeStack,
						ClStringPool				 *ai_oDynamicPool,
						tpFixedStack<m4VMState_t>	 *ai_oStateStack,
						ClVMCSContext				 *ai_poVMCSContext)
				//iniciamos los objetos implementadores
				:m_oImplDx		(m_oState),
				 m_oImplString	(m_oState),
				 m_oImplErrorIO (m_oState),
				 m_oImplGeneral (m_oState)
{
	

	Init(ai_oRunTimeStack , ai_oDynamicPool , ai_oStateStack, ai_poVMCSContext);
}

ClVMachine0::~ClVMachine0()
{
    if (m_oDynamicPool!=NULL && m_bDynPoolOwner){
		delete m_oDynamicPool;
        m_oDynamicPool=NULL;
    }

    if (m_oRunTimeStack!=NULL && m_bRuntimeStackOwner){
		delete m_oRunTimeStack;
        m_oRunTimeStack=NULL;
    }
	
    if (m_oStateStack!=NULL && m_bStateStackOwner){
		delete m_oStateStack;
        m_oStateStack=NULL;
    }
	if (m_oVariantMediator!=NULL){
		delete m_oVariantMediator;
		m_oVariantMediator=NULL;
	}

	if( m_poExcludeFunctions != NULL )
	{
		m_poExcludeFunctions->clear() ;
		delete( m_poExcludeFunctions ) ;
		m_poExcludeFunctions = NULL ;
	}
}


void ClVMachine0::Init(	tpFixedStack<m4VariantType> *ai_oRunTimeStack,
						ClStringPool *ai_oDynamicPool,
						tpFixedStack<m4VMState_t>	*ai_oStateStack,
						ClVMCSContext *ai_poVMCSContext)
{
	/* Bug 0163438
	Se inicializa el estado a cero
	*/
	memset( &m_oState, '\0', sizeof( m4VMState_t ) ) ;

	//el pool
	if (ai_oDynamicPool==0){
		m_oDynamicPool	= new ClStringPool(M4CL_DYNAMIC_POOL_SIZE);
		m_bDynPoolOwner = M4_TRUE;
	}
	else{
		m_oDynamicPool	= ai_oDynamicPool;
		m_bDynPoolOwner = M4_FALSE;
	}

	//la pila de valores
	if (ai_oRunTimeStack==0){
		m_oRunTimeStack	= new tpFixedStack<m4VariantType>(M4CL_RUNTIME_STACK_SIZE , M4CL_RUNTIME_MAX_STACK_SIZE);
		m_bRuntimeStackOwner=M4_TRUE;
	}
	else{
		m_oRunTimeStack=ai_oRunTimeStack;
		m_bRuntimeStackOwner=M4_FALSE;
	}

	//la pila de estados
	if (ai_oStateStack==0){
		m_oStateStack	= new tpFixedStack<m4VMState_t> (M4CL_STATE_STACK_SIZE, M4CL_STATE_STACK_MAX_SIZE);
		m_bStateStackOwner= M4_TRUE;
	}
	else{
		m_oStateStack=ai_oStateStack;
		m_bStateStackOwner=M4_FALSE;
	}


	
	SetContext(ai_poVMCSContext);

	//la tabla de funciones (ahora es Comun a todas las instancias)
	m_poFunctionTable = ClFuncTable::GetInstance();
	
	m_bGoOn = M4_TRUE; 
	m_bStateChanged = M4_FALSE;
    
    m_BeginTransaction = M4CL_NO_VM_TRANSACTION_DEFINED;
	
	m_oVariantMediator=new ClVariantConversor;
	m_oVariantMediator->SetPool(m_oDynamicPool);
	
	m_bRequestAbortion=M4_FALSE;
	m_poCancelStatus = NULL;
	m_iInstructionsCounter=0;

	m_poExcludeFunctions = NULL ;

	//-------------------------------------
	//iniciamos los objetos implementadores
	//-------------------------------------
	m_oImplementors[M4CL_VM_OBJECT_THIS]		=	this; //este no necesita set access...
	m_oImplementors[M4CL_VM0_COMPATIBILITY_DX]	=	&m_oImplDx; //el de las funciones de compatibilidad con 2,x
	m_oImplementors[M4CL_VM0_STRING_MANIP]		=	&m_oImplString; //para las func. de cadenas
	m_oImplementors[M4CL_VM0_ERROR_IO]			=	&m_oImplErrorIO; //para funciones de error e IO
	m_oImplementors[M4CL_VM0_GENERAL]			=	&m_oImplGeneral; //para funciones generales de librería
	 
	//iniciamos los objetos 
		
		//This no hace falta iniciarlo!!!
		
	
	//el de compatibilidad con 2.x
	m_oImplDx.SetRuntimeStack	(m_oRunTimeStack);
	m_oImplDx.SetStateStack		(m_oStateStack);
	m_oImplDx.SetPool			(m_oDynamicPool);
	m_oImplDx.SetMediator		(m_oVariantMediator);
	
	//el de cadenas
	m_oImplString.SetRuntimeStack	(m_oRunTimeStack);
	m_oImplString.SetStateStack		(m_oStateStack);
	m_oImplString.SetPool			(m_oDynamicPool);
	m_oImplString.SetMediator		(m_oVariantMediator);

	//error, IO
	m_oImplErrorIO.SetRuntimeStack	(m_oRunTimeStack);
	m_oImplErrorIO.SetStateStack	(m_oStateStack);
	m_oImplErrorIO.SetPool			(m_oDynamicPool);
	m_oImplErrorIO.SetMediator		(m_oVariantMediator);
	
	//General
	m_oImplGeneral.SetRuntimeStack	(m_oRunTimeStack);
	m_oImplGeneral.SetStateStack	(m_oStateStack);
	m_oImplGeneral.SetPool			(m_oDynamicPool);
	m_oImplGeneral.SetMediator		(m_oVariantMediator);

	// FIX Bug 0178286: srand( (unsigned)time( NULL ) );


}

//
//SetVMCSContext	
//
void ClVMachine0::SetContext	(ClVMCSContext *ai_poContext){

	m_poContext = ai_poContext;
	//iniciamos todos los implementors
	m_oImplGeneral.SetVMCSContext	(ai_poContext);
	m_oImplErrorIO.SetVMCSContext	(ai_poContext);
	m_oImplString.SetVMCSContext	(ai_poContext);
	m_oImplDx.SetVMCSContext	(ai_poContext);

}

//
//GetSize
//
m4uint32_t ClVMachine0::GetSize (void) const
{
    m4uint32_t length = 0;
	
    length += sizeof (ClVMachine0);
    length += m_oDynamicPool->GetSize ();
    length += m_oRunTimeStack->GetSize ();
    length += m_oStateStack->GetSize ();
	
    return length;
}

//*****************************
//*E J E C U T A R
//*****************************



m4return_t ClVMachine0::Execute(void){
	
	m4return_t ret;	
	m4uint16_t ui16NumFunc;

	//leemos el estado 
	ret=m_oStateStack->Top(m_oState);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, "Empty execution stack at level 0."<<__FILE__<<__LINE__)

	m_oStateStack->Pop();
    
	if (m_oState.m_poFormula.IsNull() )
	{
        m_oStateStack->Push(m_oState);
		/* Bug 0163438
		Se marca que el estado ha cambiado
		*/
		m_bStateChanged = M4_TRUE;
		return M4_SUCCESS;
    }
	
	//BUCLE PRINCIPAL DE EJECUCION de Nivel 0
	m_bGoOn = M4_TRUE;
	while  (m_bGoOn && (ret==M4_SUCCESS) )
	{
		// -- Chequeo para cancelación
			//El requestAbort no hace una cancelación inmediata, aunque podría hacerlo poniendo el 
			//m_bRequestAbortion dentro del primer if, en lugar del externo
		++m_iInstructionsCounter;

		if (m_iInstructionsCounter==M4CL_VM_FREQ_POOLING_ABORT_RQST)
		{
			m_iInstructionsCounter=0;
			
			if (m_bRequestAbortion || GetCancelStatus() )
			{
				AbortAll();
				ret = M4CL_USER_ABORT_ALL_REQUEST;
				break;
			}
		}
		// -- Fin chequeo para cancelación

		//la instrucción actual a un miembro para que sea más rápido
		m_oCurrentIns = (m_oState.m_poFormula)[m_oState.m_iCurrent];
		
		if ( M4CL_IS_LEVEL0_FLAG ( m_oCurrentIns->GetFlags() ) )
		{
			//si es de nuestro nivel la ejecutamos
			ui16NumFunc = m_oCurrentIns->GetExeFunc();		

#ifdef _DEBUG
			// Mark for check execution
			m4int8_t	iNumArgRef=0;
			m4int32_t	iPosBeforeExec;
			m4bool_t	bCheckExec;
			
			//obtenemos el número de función	
			bCheckExec = MarkBeginExec(ui16NumFunc, iNumArgRef, iPosBeforeExec);			
#endif
		
			/* Bug 0163438
			Se guarda el estado anterior por si hay reentrada
			*/
			m4VMState_t	oState ;
			memcpy( &oState, &m_oState, sizeof( m4VMState_t ) ) ;
			m_bStateChanged = M4_FALSE ;

			//Executamos la función
			ret = ExecuteFunction( ui16NumFunc, m_oImplementors, m_oState ) ;

			/* Bug 0163438
			Se restaura el estado anterior si ha habido reentrada
			*/
			if( m_bStateChanged == M4_TRUE )
			{
				m_bGoOn = M4_TRUE;
				memcpy( &m_oState, &oState, sizeof( m4VMState_t ) ) ;
			}

#ifdef _DEBUG
			// Check execution
			if ( CheckEndExec(ui16NumFunc, iNumArgRef, iPosBeforeExec, ret, bCheckExec) != M4_SUCCESS ) {
				//DUMP_CHLOG_WARNINGF(M4_CH_VM_TRACE_STACK_VM, 0<<ui16NumFunc );
			}
#endif
			m_oState.m_iCurrent++; //siguiente instrucción
		}
		else
		{
			//si la instrucción no es de nuestro nivel retornamos guardando el estado
			ret=M4CL_GET_LEVEL_FLAG( m_oCurrentIns->GetFlags() ) ;
			break;
		}
	}
	
	m_oStateStack->Push(m_oState);

	/* Bug 0163438
	Se marca que el estado ha cambiado
	*/
	m_bStateChanged = M4_TRUE;
	return ret;
}


//---------------------------------------
//Matamos todo->el gran genocidio
//---------------------------------------
m4return_t ClVMachine0::AbortAll(void){

	/*//1.-  Nos cepillamos todos los estados
	while (!m_oStateStack->IsEmpty() ) m_oStateStack->Pop();

	//2.- Nos cepillamso todos lo de la pila de variants
	while (!m_oRunTimeStack->IsEmpty() ) RuntimePopAndDelete(); //para que libere las cadenas
*/
	//3.- La transaccion la dejamos al principio
	EndVMTrans();

	//4.- El reques Abort a false
	m_bRequestAbortion=M4_FALSE;

	//5.- Retornamos M4CL_USER_ABORT_ALL_REQUEST en la pila
	RuntimePushDouble (M4CL_USER_ABORT_ALL_REQUEST);
	
	return M4_SUCCESS;
}

//***********************************************************************************
//***********************************************************************************

//**************************
//*Funciones básicas
//**************************

m4return_t ClVMachine0::ReadComment(void){
	
	//mete en la pila de ejecución el numero de linea
	//que indique el  m_iNumVar
	m_oState.m_iSourceLine = m_oCurrentIns->GetNumVar();
	
	if (m_oStepper.GetActive()){ //estamos en ejecución paso a paso
		
		if (m_oStepper.CheckForStop(m_oState) == M4_TRUE ){ //debemos parar 

			//Bug:67798   Begin  --------------------------------------------------
			//El stepper nunca se debe detener en una linea de comentario.
			// Esto sucedía cuando la polaca tiene varias instrucciones C seguidas.
			ClDecodeInstruction * poInstruction;
			for(;;) {
				poInstruction = (m_oState.m_poFormula)[m_oState.m_iCurrent + 1];
				if(poInstruction->GetExeFunc() == M4CL_VM_FT_READCOMMENT) {
					m_oState.m_iSourceLine = poInstruction->GetNumVar();
					m_oState.m_iCurrent++;
				}
				else
					break;
			}
			//Bug:67798   End    --------------------------------------------------

			return M4CL_EXECUTION_INTERRUPTION;
		}
	}
	else if( m_oStepper.GetReenterInactive() == M4_TRUE )
	{
		if( m_oStepper.CheckForBP( m_oState ) != NULL )
		{
			/* Bug 0066974 y 0051864
			Si hay reentrada no se para y se vuelca un mensaje de error
			*/
			ClCompiledMCR	*poCMCR = m_oState.m_poAccess->GetpCMCR() ;
			DUMP_CHLOG_ERRORF( M4_VM_BP_IN_REENTER, m_oState.m_iSourceLine << poCMCR->GetItemFormIdByPosition( m_oState.m_hItem, m_oState.m_iRule ) << M4ObjNodeItemI( poCMCR, m_oState.m_hItem ) ) ;
		}
	}

    return M4_SUCCESS;
}

m4return_t ClVMachine0::ReadVar(void){
	
	//mete en la pila de ejecución el contenido de la variable
	//que indique el  m_iNumVar
	
	m4return_t ret;
	m4VariantType vTempVariant;
	m4uint32_t handle;
	
	ret=m_oRunTimeStack->GetElement(vTempVariant, m_oCurrentIns->GetNumVar());
	
	//si la varaible es de tipo string habra que meter una copia de la cadena
	if (vTempVariant.Type==M4CL_CPP_TYPE_STRING_POOL){
		m_oDynamicPool->AddString( m_oDynamicPool->GetString(vTempVariant.Data.CharData), handle);
		vTempVariant.Data.CharData=handle;
	}
	
	RuntimePush(vTempVariant);
    
	
    return ret;
}


m4return_t ClVMachine0::PushNum(void){
	
	//mete en la pila de ejecución el double
	//que indique el  miembro DataDouble de la unión 
	//VariantData indicando que es un numero (poniendo 
	//el tipo del elemento VariantType de la pila
	// a numerico
	m4return_t ret;
	
	ret=RuntimePushDouble( m_oCurrentIns->GetVariantDouble());
    
	return ret;
}


m4return_t ClVMachine0::PushString(void){
	
	//mete en la pila de ejecución el int
	//que indique el  miembro CharDatae de la unión 
	//VariantData indicando que es una cadena (poniendo 
	//el tipo del elemento VariantType de la pila
	// a string
	
	m4return_t ret;
	m4uint32_t hTemp;
	m4VariantType vTempVariant;
	
	
	//JIT
	if (!(M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState.m_Flags)))
	{
		ret=m_oDynamicPool->AddString(m_oState.m_poAccess->GetpCMCR()->GetPointer( m_oCurrentIns->GetVariantInt() ) , hTemp);	
	}
	else 
	{
		ret=m_oDynamicPool->AddString(&(m_oState.m_poFormula.GetFormulaHandle()[m_oCurrentIns->GetVariantInt()]),hTemp);
	}
	
	if (ret==M4_SUCCESS){
		vTempVariant.Data.CharData=hTemp;
		vTempVariant.Type=M4CL_CPP_TYPE_STRING_POOL;
		ret=RuntimePush(vTempVariant);        
	}
	
	return ret;
}



m4return_t ClVMachine0::PushDate(void){
	
	//mete en la pila de ejecución el double
	//que indique el  miembro DataDouble de la unión 
	//VariantData indicando que es una fecha (poniendo 
	//el tipo del elemento VariantType de la pila
	// a date
	
	m4return_t ret;
	
	ret=RuntimePushDate(m_oCurrentIns->GetVariantDouble());
    
	return ret;
}




m4return_t ClVMachine0::PurgeStack(void){
	
	//purga de la pila tantos elementos como
	//indique el  m_iNumItem
	
	m4return_t ret;
	m4int32_t iNumPurges;
	m4int32_t i;
	
	iNumPurges=m_oCurrentIns->GetNumItem();
	
	for (i=0;i<iNumPurges;i++){
		ret=RuntimePopAndDelete();
		if (ret!=M4_SUCCESS) return ret;	
	}
	return M4_SUCCESS;
}



m4return_t ClVMachine0::AssignVar(void){
	//asigna a la variable que
	//indique m_iNumVar, el valor del tope de la pila
	// y elimina ese valor de la pila
	
	m4return_t ret;
	m4VariantType vTempVariant,vTemp2;
	m4int32_t iNumVar;
	
	//obtenemos el No. de la Variable a la que vamos a realizar la Asignacion
	iNumVar = m_oCurrentIns->GetNumVar();
	
	//leemos lo que había en la variable a la que vamos a cambiar el contenido
	m_oRunTimeStack -> GetElement ( vTempVariant , iNumVar );

	//leemos y sacamos el tope de la pila, que es lo que hay que Asignar
	RuntimePop(vTemp2);
	
	//SI en la variable habia una cadena tendremos que liberarla
	if (vTempVariant.Type == M4CL_CPP_TYPE_STRING_POOL)
	{
		//hay que liberar
		if (vTemp2.Type == M4CL_CPP_TYPE_STRING_POOL)
		{
			m_oDynamicPool->SetString( vTemp2.Data.CharData, vTempVariant.Data.CharData );
			vTemp2.Data.CharData = vTempVariant.Data.CharData;
		}
		else
		{
			//si el nuevo valor no es cadena liberamos la vieja y asignamos lo que sea
			m_oDynamicPool->DeleteString(vTempVariant.Data.CharData);
		}
	}
	
	ret=m_oRunTimeStack->SetElement (vTemp2, iNumVar );
	return ret;
}



m4return_t ClVMachine0::ConditionalJump(void){
	
	//salta 
	//si el valor del tope de la pila es FALSE
	//a la intrucción 
	//cuyo número está en m_iNumVar
	//se elimina el valor del tope de la pila (resultado de la cond)
	
	//Ponemos como instruccion actual la anterior a la que
	//se debe ejecutar porque en el bucle de ejecución se
	//incrementa siempre en uno
	
	m4VariantType vTempVariant;
	
    RuntimePop (vTempVariant);   //resultado de la condición
	if (vTempVariant.Data.DoubleData==0.0){ //miramos si es cero, para saltar
		m_oState.m_iCurrent+= m_oCurrentIns->GetNumVar()-1;
	}
	return M4_SUCCESS;
}


m4return_t ClVMachine0::InconditionalJump(void){
	
	//salta incondicionalmente a la intrucción
	//cuyo número está en m_iNumItem
	
	//Ponemos como instruccion actual la anterior a la que
	//se debe ejecutar porque en el bucle de ejecución se
	//incrementa siempre en uno
	m_oState.m_iCurrent+= m_oCurrentIns->GetNumVar()-1;
	return M4_SUCCESS;
}


m4return_t ClVMachine0::Return(void){
	
	//acaba la función, limpiando lo que quedeba
	//y deja como resultado el tope de la pila
	//para el que le llamo  
	
	m_bGoOn=M4_FALSE; //para no seguir ejecutando
	return M4_SUCCESS;
}




//***************************************************
//Funciones de la librería---de la tabla de funciones
//***************************************************

m4return_t ClVMachine0::IsVariantNull    (void){
	
    m4VariantType vTempVariant;
	
    RuntimePop (vTempVariant);
	
    if (IsNull (vTempVariant) ){
        RuntimePushDouble (TRUE);
    }
    else{
		DeleteVarString(vTempVariant);		
        RuntimePushDouble (FALSE);
    }
    return M4_SUCCESS;
    
}

m4return_t ClVMachine0::NullValue (void){
	
    //para hacer asignaciones a null...
    RuntimePushNull();
    return M4_SUCCESS;
}

m4return_t ClVMachine0::LogicalAnd(void){
	
	m4VariantType vTemp;
	m4VariantType vTempVariant;
	
	RuntimePop(vTempVariant);
	RuntimePop(vTemp);
    
    if (IsNull (vTempVariant) || IsNull (vTemp) ){ //Null behaviour: algun nulo->falso
        RuntimePushDouble (0.0);
        return M4_SUCCESS;
    }
	
	if (vTempVariant.Data.DoubleData!=0.0 && vTemp.Data.DoubleData!=0.0) {
		RuntimePushDouble(1.0);        
	}
	else {
		RuntimePushDouble(0.0);        
	}
	
	return M4_SUCCESS;
}

m4return_t ClVMachine0::LogicalOr(void){
	
	m4VariantType vTemp;
	m4VariantType vTempVariant;
	
	RuntimePop(vTempVariant);
	RuntimePop(vTemp);
	
    if (IsNull (vTempVariant) || IsNull (vTemp) ){ //Null behaviour: algun nulo->falso
        RuntimePushDouble (0.0);
        return M4_SUCCESS;
    }
	
	if (vTempVariant.Data.DoubleData!=0.0 || vTemp.Data.DoubleData!=0.0) {
		RuntimePushDouble(1.0);
	}
	else {
		RuntimePushDouble(0.0);
	}
    
	return M4_SUCCESS;
}


m4return_t ClVMachine0::LogicalNot(void){
	
	m4VariantType vTempVariant;
	
	RuntimePop(vTempVariant);
	
    if (IsNull (vTempVariant) ){ //Null behaviour: algun nulo->falso
        RuntimePushDouble (0.0);
        return M4_SUCCESS;
    }
	
	if (vTempVariant.Data.DoubleData!=0.0) {
		RuntimePushDouble(0.0);
	}
	else {
		RuntimePushDouble(1.0);
	}
	
	return M4_SUCCESS;
}


m4return_t ClVMachine0::LogicalEqual(void){
	
	m4return_t ret;
	m4VariantType vTemp,vTemp2;
	m4VariantType vTempVariant;
	
	ret=M4_SUCCESS;
	
	RuntimePop(vTempVariant);
	RuntimePop(vTemp);
	
    if (IsNull (vTempVariant) && IsNull (vTemp) ){ //Null behaviour: Dos nulo->true
        RuntimePushDouble (1.0);
        return M4_SUCCESS;
    }
	
	if (vTempVariant.Type!=vTemp.Type){ //los tipos no coinciden
		vTemp2.Data.DoubleData=0.0;
		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
		
	}
	else{
		switch (vTemp.Type) {
		case M4CL_CPP_TYPE_STRING_POOL: {
			vTemp2.Data.DoubleData=!(strcmp(m_oDynamicPool->GetString(vTempVariant.Data.CharData), m_oDynamicPool->GetString(vTemp.Data.CharData) ) );
			//liberamos las cadenas
			m_oDynamicPool->DeleteString(vTempVariant.Data.CharData);
			m_oDynamicPool->DeleteString(vTemp.Data.CharData);
			break;
										}
			
		case M4CL_CPP_TYPE_NUMBER: {
			//vTemp2.Data.DoubleData=(vTemp.Data.DoubleData==vTempVariant.Data.DoubleData);
			vTemp2.Data.DoubleData = ( fabs(vTemp.Data.DoubleData-vTempVariant.Data.DoubleData) <= M4_LN4_MATH_PRECISION) ? 1.0 : 0.0 ;
			break;
								   }
			
		case M4CL_CPP_TYPE_DATE:   {
			//vTemp2.Data.DoubleData=(vTemp.Data.DoubleData==vTempVariant.Data.DoubleData);
			vTemp2.Data.DoubleData = ( fabs(vTemp.Data.DoubleData-vTempVariant.Data.DoubleData) <= M4_LN4_DATE_PRECISION) ? 1.0 : 0.0 ;
			break;
								   }
		}
	}
	
	vTemp2.Type=M4CL_CPP_TYPE_NUMBER;
	RuntimePush(vTemp2);
	return ret;
}


m4return_t ClVMachine0::LogicalNotEqual(void){
	
	m4return_t ret;
	m4VariantType vTemp,vTemp2;
	m4VariantType vTempVariant;
	
	ret=M4_SUCCESS;
	
	RuntimePop(vTempVariant);
	RuntimePop(vTemp);
	
    if (IsNull (vTempVariant) && IsNull (vTemp) ){ //Null behaviour: Dos nulo->False
        RuntimePushDouble (0.0);
        return M4_SUCCESS;
    }
	
	if (vTempVariant.Type!=vTemp.Type){ //los tipos no coinciden
		vTemp2.Data.DoubleData=1.0;
		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
		
	}
	else{
		switch (vTemp.Type) {
		case M4CL_CPP_TYPE_STRING_POOL: {
			vTemp2.Data.DoubleData=( (strcmp(m_oDynamicPool->GetString(vTempVariant.Data.CharData), m_oDynamicPool->GetString(vTemp.Data.CharData) ) )==0 ? 0.0:1.0);			
			//liberamos las cadenas
			m_oDynamicPool->DeleteString(vTempVariant.Data.CharData);
			m_oDynamicPool->DeleteString(vTemp.Data.CharData);
			break;
										}
			
		case M4CL_CPP_TYPE_NUMBER: {
			//vTemp2.Data.DoubleData=(vTemp.Data.DoubleData!=vTempVariant.Data.DoubleData);
			vTemp2.Data.DoubleData = ( fabs(vTemp.Data.DoubleData-vTempVariant.Data.DoubleData) > M4_LN4_MATH_PRECISION) ? 1.0 : 0.0 ;
			break;
								   }
			
		case M4CL_CPP_TYPE_DATE:   {
			//vTemp2.Data.DoubleData=(vTemp.Data.DoubleData!=vTempVariant.Data.DoubleData);
			vTemp2.Data.DoubleData = ( fabs(vTemp.Data.DoubleData-vTempVariant.Data.DoubleData) > M4_LN4_DATE_PRECISION) ? 1.0 : 0.0 ;
			break;
								   }
		}
	}
	
	vTemp2.Type=M4CL_CPP_TYPE_NUMBER;
	RuntimePush(vTemp2);
	return ret;
}



m4return_t ClVMachine0::LogicalGreater(void){
	
	m4return_t ret;
	m4VariantType vTemp,vTemp2;
	m4VariantType vTempVariant;
	
	ret=M4_SUCCESS;
	
	RuntimePop(vTempVariant);
	RuntimePop(vTemp);
	//Null behaviour: Algun nulo->False o los tipos no coinciden
    if (IsNull (vTempVariant) || IsNull (vTemp) || vTempVariant.Type!=vTemp.Type){
		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
        RuntimePushDouble (0.0);
        return M4_SUCCESS;
    }
	
	switch (vTemp.Type) {
	case M4CL_CPP_TYPE_STRING_POOL: {
		vTemp2.Data.DoubleData=( (strcmp(m_oDynamicPool->GetString(vTemp.Data.CharData), m_oDynamicPool->GetString(vTempVariant.Data.CharData) ) )>0 ? 1.0:0.0);
		//liberamos las cadenas
		m_oDynamicPool->DeleteString(vTempVariant.Data.CharData);
		m_oDynamicPool->DeleteString(vTemp.Data.CharData);
		break;
									}
		
	case M4CL_CPP_TYPE_NUMBER: {
		//vTemp2.Data.DoubleData=(vTemp.Data.DoubleData > vTempVariant.Data.DoubleData);
		if ( fabs(vTemp.Data.DoubleData-vTempVariant.Data.DoubleData) <= M4_LN4_MATH_PRECISION ) {
			vTemp2.Data.DoubleData=0.0;	// Son iguales
		} else {
			vTemp2.Data.DoubleData=(vTemp.Data.DoubleData>vTempVariant.Data.DoubleData);
		}
		break;
							   }
		
	case M4CL_CPP_TYPE_DATE:   {
		//vTemp2.Data.DoubleData=(vTemp.Data.DoubleData > vTempVariant.Data.DoubleData);
		if ( fabs(vTemp.Data.DoubleData-vTempVariant.Data.DoubleData) <= M4_LN4_DATE_PRECISION ) {
			vTemp2.Data.DoubleData=0.0;	// Son iguales
		} else {
			vTemp2.Data.DoubleData=(vTemp.Data.DoubleData>vTempVariant.Data.DoubleData);
		}
		break;
							   }
	}
	
	vTemp2.Type=M4CL_CPP_TYPE_NUMBER;
	RuntimePush(vTemp2);
	return ret;
}


m4return_t ClVMachine0::LogicalGreaterEqual(void){
	
	m4return_t ret;
	m4VariantType vTemp,vTemp2;
	m4VariantType vTempVariant;
	
	ret=M4_SUCCESS;
	
	RuntimePop(vTempVariant);
	RuntimePop(vTemp);
	//Null behaviour: Algun nulo->False o los tipos no coinciden
    if (IsNull (vTempVariant) || IsNull (vTemp) || vTempVariant.Type!=vTemp.Type){
		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
        RuntimePushDouble (0.0);
        return M4_SUCCESS;
    }
	
	switch (vTemp.Type) {
	case M4CL_CPP_TYPE_STRING_POOL: {
		vTemp2.Data.DoubleData=( (strcmp(m_oDynamicPool->GetString(vTemp.Data.CharData), m_oDynamicPool->GetString(vTempVariant.Data.CharData) ) )>=0 ? 1.0:0.0);
		//liberamos las cadenas
		m_oDynamicPool->DeleteString(vTempVariant.Data.CharData);
		m_oDynamicPool->DeleteString(vTemp.Data.CharData);
		break;
									}
		
	case M4CL_CPP_TYPE_NUMBER: {
		//vTemp2.Data.DoubleData=(vTemp.Data.DoubleData >= vTempVariant.Data.DoubleData);
		if (vTemp.Data.DoubleData > vTempVariant.Data.DoubleData)	//Caso > que
			vTemp2.Data.DoubleData=1.0;
		else {														//Caso = que
			//M4_LN4_MATH_PRECISION = 10e-15;
			vTemp2.Data.DoubleData = ( fabs(vTemp.Data.DoubleData-vTempVariant.Data.DoubleData) <= M4_LN4_MATH_PRECISION) ? 1.0 : 0.0 ;
		}
		break;
							   }
		
	case M4CL_CPP_TYPE_DATE:   {
		//vTemp2.Data.DoubleData=(vTemp.Data.DoubleData >= vTempVariant.Data.DoubleData);
		if (vTemp.Data.DoubleData > vTempVariant.Data.DoubleData)	//Caso > que
			vTemp2.Data.DoubleData=1.0;
		else {														//Caso = que
			//M4_LN4_DATE_PRECISION = 10e-9;
			vTemp2.Data.DoubleData = ( fabs(vTemp.Data.DoubleData-vTempVariant.Data.DoubleData) <= M4_LN4_DATE_PRECISION) ? 1.0 : 0.0 ;
		}
		break;
							   }
	}
	
	vTemp2.Type=M4CL_CPP_TYPE_NUMBER;
	RuntimePush(vTemp2);
	return ret;
}


m4return_t ClVMachine0::LogicalLess(void){
	
	m4return_t ret;
	m4VariantType vTemp,vTemp2;
	m4VariantType vTempVariant;
	
	ret=M4_SUCCESS;
	
	RuntimePop(vTempVariant);
	RuntimePop(vTemp);
	
	//Null behaviour: Algun nulo->False o los tipos no coinciden
    if (IsNull (vTempVariant) || IsNull (vTemp) || vTempVariant.Type!=vTemp.Type){
		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
        RuntimePushDouble (0.0);
        return M4_SUCCESS;
    }
	
	switch (vTemp.Type) {
	case M4CL_CPP_TYPE_STRING_POOL: {
		vTemp2.Data.DoubleData=(strcmp(m_oDynamicPool->GetString(vTemp.Data.CharData), m_oDynamicPool->GetString(vTempVariant.Data.CharData) )<0 ? 1.0:0.0);
		//liberamos las cadenas
		m_oDynamicPool->DeleteString(vTempVariant.Data.CharData);
		m_oDynamicPool->DeleteString(vTemp.Data.CharData);
		break;
									}
		
	case M4CL_CPP_TYPE_NUMBER: {
		//vTemp2.Data.DoubleData=(vTemp.Data.DoubleData > vTempVariant.Data.DoubleData);
		if ( fabs(vTemp.Data.DoubleData-vTempVariant.Data.DoubleData) <= M4_LN4_MATH_PRECISION ) {
			vTemp2.Data.DoubleData=0.0;	// Son iguales
		} else {
			vTemp2.Data.DoubleData=(vTemp.Data.DoubleData<vTempVariant.Data.DoubleData);
		}
		break;
							   }
		
	case M4CL_CPP_TYPE_DATE:   {
		//vTemp2.Data.DoubleData=(vTemp.Data.DoubleData > vTempVariant.Data.DoubleData);
		if ( fabs(vTemp.Data.DoubleData-vTempVariant.Data.DoubleData) <= M4_LN4_DATE_PRECISION ) {
			vTemp2.Data.DoubleData=0.0;	// Son iguales
		} else {
			vTemp2.Data.DoubleData=(vTemp.Data.DoubleData<vTempVariant.Data.DoubleData);
		}
		break;
							   }
	}
	
	vTemp2.Type=M4CL_CPP_TYPE_NUMBER;
	RuntimePush(vTemp2);
	return ret;
}


m4return_t ClVMachine0::LogicalLessEqual(void){
	
	m4return_t ret;
	m4VariantType vTemp,vTemp2;
	m4VariantType vTempVariant;
	
	ret=M4_SUCCESS;
	
	RuntimePop(vTempVariant);
	RuntimePop(vTemp);
	
	//Null behaviour: Algun nulo->False o los tipos no coinciden
    if (IsNull (vTempVariant) || IsNull (vTemp) || vTempVariant.Type!=vTemp.Type){
		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
        RuntimePushDouble (0.0);
        return M4_SUCCESS;
    }
	
	switch (vTemp.Type) {
	case M4CL_CPP_TYPE_STRING_POOL: {
		vTemp2.Data.DoubleData= (strcmp(m_oDynamicPool->GetString(vTemp.Data.CharData), m_oDynamicPool->GetString(vTempVariant.Data.CharData) )<= 0 ? 1.0:0.0);
		//liberamos las cadenas
		m_oDynamicPool->DeleteString(vTempVariant.Data.CharData);
		m_oDynamicPool->DeleteString(vTemp.Data.CharData);
		break;
									}
		
	case M4CL_CPP_TYPE_NUMBER: 
		{
			//vTemp2.Data.DoubleData=(vTemp.Data.DoubleData <= vTempVariant.Data.DoubleData);			
			if (vTemp.Data.DoubleData < vTempVariant.Data.DoubleData)	//Caso < que
				vTemp2.Data.DoubleData=1.0;
			else {														//Caso = que
				//M4_LN4_MATH_PRECISION = 10e-15;
				vTemp2.Data.DoubleData = ( fabs(vTemp.Data.DoubleData-vTempVariant.Data.DoubleData) <= M4_LN4_MATH_PRECISION) ? 1.0 : 0.0 ;
			}
			break;
		}
		
	case M4CL_CPP_TYPE_DATE:   {
		//vTemp2.Data.DoubleData=(vTemp.Data.DoubleData <= vTempVariant.Data.DoubleData);			
		if (vTemp.Data.DoubleData < vTempVariant.Data.DoubleData)	//Caso < que
			vTemp2.Data.DoubleData=1.0;
		else {														//Caso = que
			//M4_LN4_DATE_PRECISION = 10e-9;
			vTemp2.Data.DoubleData = ( fabs(vTemp.Data.DoubleData-vTempVariant.Data.DoubleData) <= M4_LN4_DATE_PRECISION) ? 1.0 : 0.0 ;
		}
		break;
							   }
	}
	
	vTemp2.Type=M4CL_CPP_TYPE_NUMBER;
	RuntimePush(vTemp2);
	return ret;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//O P E R A C I O N E S    A R I T M E T I C A S
//---------------------------------------------------------------------------
//--------------------------------------------------------------------------




m4return_t ClVMachine0::AritmeticalAdd(void){
	
	m4VariantType vTemp,vTemp2;
	m4VariantType vTempVariant;
	m4return_t ret;	
	
	RuntimePop(vTempVariant);	//dch
	RuntimePop(vTemp);			//izq
	
    if (IsNull (vTempVariant) || IsNull (vTemp) ){ //Null behaviour: Algun nulo->Nulo
		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
        RuntimePushNull ();
        return M4_SUCCESS;
    }
	
	if (vTempVariant.Type!=vTemp.Type && ( (vTemp.Type|vTempVariant.Type) != (M4CL_CPP_TYPE_NUMBER|M4CL_CPP_TYPE_DATE) ) ){ //los tipos no coinciden
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPES_NOT_VALID, "+" << M4ClCppType(vTemp.Type)<<M4ClCppType(vTempVariant.Type)<<M4ObjNodeItemI(m_oState.m_poAccess->GetpCMCR(), m_oState.m_hItem)<<DumpCallStack(&m_oState) );

		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
		return M4_ERROR;
	}
	
	if (vTemp.Type==M4CL_CPP_TYPE_STRING_POOL){//concatena cadenas
		//concatenamos las cadenas

		// Borrando primero, para recortar el Pool
		ret=m_oDynamicPool->AddStringConcat(vTemp.Data.CharData , vTempVariant.Data.CharData, vTemp2.Data.CharData);
		vTemp2.IsUser = vTemp.IsUser || vTempVariant.IsUser;
		vTemp2.Type=M4CL_CPP_TYPE_STRING_POOL;

		if (ret!=M4_SUCCESS) {
			return ret;
		}
	}
	else{ //double o date
		vTemp2.Data.DoubleData=vTemp.Data.DoubleData + vTempVariant.Data.DoubleData;
        if (vTemp.Type==M4CL_CPP_TYPE_DATE || vTempVariant.Type==M4CL_CPP_TYPE_DATE ){
            vTemp2.Type=M4CL_CPP_TYPE_DATE;
        }
        else{
            vTemp2.Type=vTemp.Type;
        }
	}
	
	RuntimePush(vTemp2);
	return M4_SUCCESS;
}


m4return_t ClVMachine0::AritmeticalSub(void){
	
	m4VariantType vTemp,vTemp2;
	m4VariantType vTempVariant;
	m4char_t *pszTemp2,*pszTemp;
	m4return_t ret;
	m4uint32_t handle;
	
	RuntimePop(vTempVariant);
	RuntimePop(vTemp);
	
    if (IsNull (vTempVariant) || IsNull (vTemp) ){ //Null behaviour: Algun nulo->Nulo
		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
        RuntimePushNull ();
        return M4_SUCCESS;
    }
	
	if (vTempVariant.Type!=vTemp.Type && ( (vTemp.Type|vTempVariant.Type) != (M4CL_CPP_TYPE_NUMBER|M4CL_CPP_TYPE_DATE) ) ){ //los tipos no coinciden
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPES_NOT_VALID, "-" << M4ClCppType(vTemp.Type)<<M4ClCppType(vTempVariant.Type)<<M4ObjNodeItemI(m_oState.m_poAccess->GetpCMCR(), m_oState.m_hItem)<<DumpCallStack(&m_oState) );

		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
		return M4_ERROR;
	}
	
	if (vTemp.Type==M4CL_CPP_TYPE_STRING_POOL){//retornamos solo la última cadena
		//Retornamos solo la última cadena
		pszTemp2=m_oDynamicPool->GetString(vTempVariant.Data.CharData);
		pszTemp=new char[m_oDynamicPool->GetStringLength(vTempVariant.Data.CharData)+1];
		strcpy(pszTemp,pszTemp2);
		m_oDynamicPool->DeleteString(vTempVariant.Data.CharData);
		m_oDynamicPool->DeleteString(vTemp.Data.CharData);
		//añadimos la copia de la nueva cadena al pool
		ret=m_oDynamicPool->AddString(pszTemp , handle);
		vTemp2.Data.CharData=handle;
		if (ret!=M4_SUCCESS) {
			delete [] pszTemp;
			return M4_ERROR;
		}
		vTemp2.IsUser = vTempVariant.IsUser;
		vTemp2.Type=M4CL_CPP_TYPE_STRING_POOL;
		//borramos las cadenas que ya no se usan
		delete [] pszTemp;
	}
	else{  //double o date
		vTemp2.Data.DoubleData= vTemp.Data.DoubleData - vTempVariant.Data.DoubleData;
        if (vTemp.Type==M4CL_CPP_TYPE_DATE && vTempVariant.Type==M4CL_CPP_TYPE_NUMBER){
			vTemp2.Type=M4CL_CPP_TYPE_DATE;
		}
		else{
			vTemp2.Type=M4CL_CPP_TYPE_NUMBER;
		}
		//vTemp2.Type=vTemp.Type;
       
		//	      -  -----------------
		//            DATE     NUMBER
		//
		//   DATE     Number   Date
		// NUMBER	  Number   Number
		//
		//

	}
	
	RuntimePush(vTemp2);
	return M4_SUCCESS;
}



m4return_t ClVMachine0::AritmeticalMul(void){
	
	m4VariantType vTemp,vTemp2;
	m4VariantType vTempVariant;
	m4char_t *pszTemp2,*pszTemp;
	m4return_t ret;
	m4uint32_t handle;
	m4uint32_t i;
	
	RuntimePop(vTempVariant);
	RuntimePop(vTemp);
	
    if (IsNull (vTempVariant) || IsNull (vTemp) ){ //Null behaviour: Algun nulo->Nulo
		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
        RuntimePushNull ();
        return M4_SUCCESS;
    }
	
	if (vTemp.Type== M4CL_CPP_TYPE_STRING_POOL && vTempVariant.Type== M4CL_CPP_TYPE_NUMBER){ 
		//Retornamos solo la última cadena
		pszTemp2=m_oDynamicPool->GetString(vTemp.Data.CharData);
		m4uint32_t  iNumTimes = m4uint32_t(vTempVariant.Data.DoubleData > 0 ? vTempVariant.Data.DoubleData : -vTempVariant.Data.DoubleData);
		int tam=m_oDynamicPool->GetStringLength(vTemp.Data.CharData) * iNumTimes+1;
		pszTemp=new char[tam];
		pszTemp[0]='\0';
		for (i=0;i<iNumTimes;i++){
			strcat(pszTemp,pszTemp2);
		}
		
		//borramos del pool, para recortar el pool
		m_oDynamicPool->DeleteString(vTemp.Data.CharData);
		//añadimos la copia de la nueva cadena al pool		
		ret=m_oDynamicPool->AddString(pszTemp, handle);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ADD_TO_POOL, ERRORLOG, DumpCallStack(&m_oState))
			
		vTemp2.Data.CharData=handle;
		vTemp2.IsUser=vTemp.IsUser;
		vTemp2.Type=M4CL_CPP_TYPE_STRING_POOL;
		//borramos lo que ya no sirve
		delete [] pszTemp;		
	}
	else if (vTempVariant.Type== M4CL_CPP_TYPE_NUMBER && vTemp.Type== M4CL_CPP_TYPE_NUMBER   ){ //double o date
		vTemp2.Data.DoubleData= vTemp.Data.DoubleData * vTempVariant.Data.DoubleData;
		vTemp2.Type=vTemp.Type;
	}
	else { //los tipos no valen
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPES_NOT_VALID, "*" << M4ClCppType(vTemp.Type)<<M4ClCppType(vTempVariant.Type)<<M4ObjNodeItemI(m_oState.m_poAccess->GetpCMCR(), m_oState.m_hItem)<<DumpCallStack(&m_oState) );

		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
		return M4_ERROR;
	}
	RuntimePush(vTemp2);
	return M4_SUCCESS;
}


m4return_t ClVMachine0::AritmeticalDiv(void){
	
	m4VariantType vTemp;
	m4VariantType vTempVariant;
	
	RuntimePop(vTempVariant);
	RuntimePop(vTemp);
	
    if (IsNull (vTempVariant) || IsNull (vTemp) ){ //Null behaviour: Algun nulo->Nulo
        RuntimePushNull ();
		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
        return M4_SUCCESS;
    }
	
	
	/* Bug 0079617
	Debe ser un || y no &&.
	*/
	if (vTempVariant.Type!=M4CL_CPP_TYPE_NUMBER || vTemp.Type!=M4CL_CPP_TYPE_NUMBER){ //los tipos no coinciden
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPES_NOT_VALID, "/" << M4ClCppType(vTemp.Type)<<M4ClCppType(vTempVariant.Type)<<M4ObjNodeItemI(m_oState.m_poAccess->GetpCMCR(), m_oState.m_hItem)<<DumpCallStack(&m_oState) );
		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		DeleteVarString (vTemp);
		return M4_ERROR;
	}
	
	//double 
	if (vTempVariant.Data.DoubleData==0.0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_DIV_BY_ZERO, DumpLine(&m_oState) << DumpCallStack(&m_oState) );
		RuntimePushDouble (0.0);
	}
	else{
        RuntimePushDouble (vTemp.Data.DoubleData / vTempVariant.Data.DoubleData);
	}
	
	return M4_SUCCESS;
}


m4return_t ClVMachine0::AritmeticalChangeSign(void){
	
	m4VariantType vTempVariant;
	
	RuntimePop(vTempVariant);
	
    if (IsNull (vTempVariant)  ){ //Null behaviour: Algun nulo->Nulo
        RuntimePushNull ();
        return M4_SUCCESS;
    }
	
	if (vTempVariant.Type!=M4CL_CPP_TYPE_NUMBER) { //el tipo no coincide
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "operand"<<"-" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTempVariant.Type)<< DumpCallStack(&m_oState) );

		//liberamos la posible cadena
		DeleteVarString (vTempVariant);
		return M4_ERROR;
	}
	
	// es double 
	vTempVariant.Data.DoubleData=-vTempVariant.Data.DoubleData ;
	
	RuntimePush(vTempVariant);
	return M4_SUCCESS;
}


m4return_t ClVMachine0::Increment(void){
/*incrementa una variable:
number -> +1
fecha  -> +1
cadena -> si es de una sola letra-> siguiente ascii, si no error
nulo   -> nulo
    */
	
	m4VariantType vTemp;
	
	RuntimePop(vTemp); 
	
    switch (vTemp.Type) {
        
    case M4CL_CPP_TYPE_NULL:
        {
            RuntimePushNull();
            break;
        }
    case M4CL_CPP_TYPE_NUMBER:
    case M4CL_CPP_TYPE_DATE:
        {   
            ++vTemp.Data.DoubleData;
            RuntimePush(vTemp);
            break;
        }
		
    case M4CL_CPP_TYPE_STRING_POOL:
        {
            m4char_t *pszTemp=m_oDynamicPool->GetString (vTemp.Data.CharData);
            if (m_oDynamicPool->GetStringLength (vTemp.Data.CharData) == 1 ){
				m4char_t pcBuffer[2];
				strcpy(pcBuffer,pszTemp);
                pcBuffer[0]++;
                m_oDynamicPool->DeleteString (vTemp.Data.CharData);
                RuntimePushString (pcBuffer, vTemp.IsUser);
            }
            else if(m_oDynamicPool->GetStringLength (vTemp.Data.CharData) > 1 ){
                m_oDynamicPool->DeleteString (vTemp.Data.CharData);
				DUMP_CHLOG_ERRORF(M4_CH_VM_STRING_TOO_LONG, "Increment"<<1 << DumpCallStack(&m_oState) );
                return M4_ERROR;
            }
			else{ //<0
				m_oDynamicPool->DeleteString (vTemp.Data.CharData);
				DUMP_CHLOG_ERRORF(M4_CH_VM_STRING_TOO_SHORT, "Increment"<<1 << DumpCallStack(&m_oState) );
                return M4_ERROR;

			}
            break;
        }
    }
	
	return M4_SUCCESS;
	
}
//*******************************************************
//Funciones de la LIBRERIA LN4
//*******************************************************


//funciones de la librería para las transacciones de VM
m4return_t ClVMachine0 :: BeginVMTransaction(void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: BeginVMTransaction");
#endif    
	m4int32_t iOld;
	iOld=m_BeginTransaction;
    BeginVMTrans();
	if (iOld != m_BeginTransaction){
		// TEST ++m_BeginTransaction; //cuando se ejecuta esta funcion ln4, en la pila de metodos no esta
						//el que estamos ejecutando, por lo que el sitio donde empieza la trnasacción hay que incrementarlo para que cuente el nuestro
	}
    RuntimePushDouble(M4_SUCCESS);
    return M4_SUCCESS;
}

m4return_t ClVMachine0 :: EndVMTransaction(void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: EndVMTransaction");
#endif
    m4VariantType vTemp;
    m4return_t ret;
    //sacamos el argumento de la pila--commit
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "EndVMTransaction.")	;
    if (vTemp.Type!=M4CL_CPP_TYPE_NUMBER){
		//liberamos la posible cadena
		DeleteVarString (vTemp);
        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
    
	/* Bug 0115803
	Si viene el valor M4CL_VM_ET_ROLLBACK_RESUME es como si fuere un rollback
	pero recuperando
	*/
	m4bool_t	bRecover = M4_FALSE ;

	if( vTemp.Data.DoubleData == M4CL_VM_ET_ROLLBACK_RESUME )
	{
		vTemp.Data.DoubleData = M4CL_VM_ET_ROLLBACK ;
		bRecover = M4_TRUE ;
	}
	
	//chequeo de rangos	
	/* Bug 0147956
	Se vuelca un error y el valor asumido es rollback
	*/
	if (vTemp.Data.DoubleData > M4CL_VM_ET_MAX_VALUE || vTemp.Data.DoubleData < M4CL_VM_ET_MIN_VALUE){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Commit"<<"EndVMTransaction"<<(m4uint32_t)vTemp.Data.DoubleData << DumpCallStack() );
		vTemp.Data.DoubleData=M4CL_VM_ET_ROLLBACK;
	}


	if( bRecover == M4_FALSE )
	{
		EndVMTrans();
	}

    RuntimePushDouble(vTemp.Data.DoubleData!=M4CL_VM_ET_ROLLBACK ? M4_SUCCESS: M4_ERROR);
	
	if( vTemp.Data.DoubleData == M4CL_VM_ET_ROLLBACK )
	{
		//mensajito de error
		if( GET_ERROR_QUEUE_SIZE() == 0 )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_ROLLBACK_TRANS, DumpCallStack() );
		}
	}

    return (vTemp.Data.DoubleData!=M4CL_VM_ET_ROLLBACK ? M4_SUCCESS: M4_ERROR);
	//usamos -1 en lugarde -2. No se muy bien porque se ha antojado esto, porque los de VB no le hacian caso
	//y desde ln4 no se leia este valor (aunque alguien con poderes para-normales parece que si lo ha leido)
    //return (vTemp.Data.DoubleData!=0.0 ? M4_SUCCESS: M4CL_USER_ERROR_ABORT_TRANSACTION);
}


ostream & operator <<(ostream &out, m4VariantType ai_data){
	
	switch (ai_data.Type ){
	case M4CL_CPP_TYPE_NUMBER:
		out<<"Double:"<<ai_data.Data.DoubleData;
		break;
	case M4CL_CPP_TYPE_STRING_POOL:
		out<<"String:"<<ai_data.Data.CharData;
		break;
	case M4CL_CPP_TYPE_DATE:
		out<<"Fecha:"<<ai_data.Data.DoubleData;
		break;
    case M4CL_CPP_TYPE_STRING_VAR:
        out<<"String:"<<ai_data.Data.PointerChar;
        break;
		
	}
	
	return out;
}

//-----------------------------------------------
//---------SetDates
//-----------------------------------------------
m4return_t ClVMachine0::SetStartDate (void){
    m4VariantType   vTemp;
    m4return_t      ret;
    m4VariantType   Fecha;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: SetStartDate");
#endif
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetStartDate.")	;
	
	if ( CheckDateIfErrorPush (vTemp, M4_ERROR) )   {
		//ponemos como fecha del access y de nuestro estado la que nos pasan
		m_oState.m_dBeginDate=vTemp.Data.DoubleData;
		Fecha.Type=vTemp.Type;
		Fecha.Data=vTemp.Data;
		m_oState.m_poAccess->ExecutionStartDate.Set (Fecha);
		RuntimePushDouble(M4_SUCCESS);
	}       
    return M4_SUCCESS;
}


m4return_t ClVMachine0::SetEndDate (void){
    m4VariantType   vTemp;
    m4return_t      ret;
    m4VariantType   Fecha;
	
#if _M4_DEBUG > 10  
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: SetEndDate");
#endif
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetEndDate.")	;
	
	if ( CheckDateIfErrorPush (vTemp, M4_ERROR) )   {    
		//ponemos como fecha del access y de nuestro estado la que nos pasan
		m_oState.m_dEndDate=vTemp.Data.DoubleData;
		Fecha.Type=vTemp.Type;
		Fecha.Data=vTemp.Data;
		m_oState.m_poAccess->ExecutionEndDate.Set (Fecha);
		RuntimePushDouble(M4_SUCCESS);
    }    
    return M4_SUCCESS;
	
}

m4return_t ClVMachine0::SetRunDate (void){
    m4VariantType   vTemp;
    m4return_t      ret;
    m4VariantType   Fecha;
    
#if _M4_DEBUG > 10   
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: SetRunDate");
#endif
    ret=RuntimePop (vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetRunDate.")	;
	
	if ( CheckDateIfErrorPush (vTemp, M4_ERROR) )   {
		//ponemos como fecha del access y de nuestro estado la que nos pasan
		m_oState.m_dDate=vTemp.Data.DoubleData;  //en el estado
		Fecha.Type=vTemp.Type;
		Fecha.Data=vTemp.Data;
		m_oState.m_poAccess->ExecutionDate.Set (Fecha); //en el access
        RuntimePushDouble(M4_SUCCESS);
    }
    return M4_SUCCESS;
}

//----------------------------------------------------
// Leer fechas
//----------------------------------------------------
m4return_t ClVMachine0::GetStartDate (void){
	
    m4VariantType   Fecha;
	
#if _M4_DEBUG > 10   
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: GetStartDate");
#endif
    m_oState.m_poAccess->ExecutionStartDate.Get (Fecha);
	
    RuntimePushDate (Fecha.Data.DoubleData);
    return M4_SUCCESS;
    
}

m4return_t ClVMachine0::GetEndDate (void){
	
    m4VariantType   Fecha;
	
#if _M4_DEBUG > 10  
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: GetEndDate");
#endif
    m_oState.m_poAccess->ExecutionEndDate.Get (Fecha);
	
    RuntimePushDate (Fecha.Data.DoubleData);
    return M4_SUCCESS;
}

m4return_t ClVMachine0::GetRunDate (void){
	
    m4VariantType   Fecha;

#if _M4_DEBUG > 10    
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: GetRunDate");
#endif
    m_oState.m_poAccess->ExecutionDate.Get (Fecha);
	
    RuntimePushDate (Fecha.Data.DoubleData);
    return M4_SUCCESS;
}



//----------------------------------------------------
// Leer fechas en las que se está ejecutando el estado, sin mirar las del access
//----------------------------------------------------
m4return_t ClVMachine0::GetRunningStartDate (void){
	
#if _M4_DEBUG > 10   
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: GetRunningStartDate");
#endif
    return RuntimePushDate (m_oState.m_dBeginDate);
}

m4return_t ClVMachine0::GetRunningEndDate (void){
	
#if _M4_DEBUG > 10  
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: GetRunningEndDate");
#endif
    return RuntimePushDate (m_oState.m_dEndDate);
}

m4return_t ClVMachine0::GetRunningRunDate (void){
	
#if _M4_DEBUG > 10    
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L0, "Trace VM0: GetRunningRunDate");
#endif
    return RuntimePushDate (m_oState.m_dDate);
}





//--------------
//Volcar el pool
//--------------
m4return_t ClVMachine0::DumpPool (void)
{
	
	m4return_t ret = M4_SUCCESS;
	m4VariantType vFileName;
	
	
	ret = RuntimePopString (vFileName);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "FileName" << "DumpPool" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vFileName.Type)<<DumpCallStack(&m_oState) );
	
	
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	FILE *ffile = M4Fopen( m_oDynamicPool->GetString(vFileName.Data.CharData), M4UniWriteBinary, eEncoding ) ;
	
	m_oDynamicPool->DeleteString (vFileName.Data.CharData);
	
	if (ffile != NULL)
	{
		m_oDynamicPool->Dump(ffile);
		fclose (ffile);
	}
	else 
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_FILE, m_oDynamicPool->GetString (vFileName.Data.CharData ) << DumpCallStack(&m_oState) );
		
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
	
	RuntimePushDouble ( M4_SUCCESS );
	return M4_SUCCESS;
} 


//--------------------------------------------------------------------------
//functions for Check Execution
//--------------------------------------------------------------------------

#ifdef _DEBUG
// Mark for check the execution of the function
// In : ai_ui16NumFunc	= No.function
// Out:	ao_iNumArgRef	= No.Referenced arguments
//		ao_iPosBeforeExec = Stack Position before execution
//		return = Check Execution Y/N
m4bool_t ClVMachine0::MarkBeginExec(m4uint16_t	ai_ui16NumFunc, m4int8_t &ao_iNumArgRef, m4int32_t &ao_iPosBeforeExec) {

	// Preparacion:
	m4bool_t	bCheckExec = M4_FALSE;			// Check Execution Yes/No
	m4int8_t	iNumArg;						// No. Value	 Arguments in the function
	//m4int8_t	iNumArgRef;						// No. Reference Arguments in the function
	m4return_t	ret = M4_SUCCESS;
	m4VariantType vNumArg;
	
	ClFunctionDesc& oFunction = m_poFunctionTable->GetObjectFunction( ai_ui16NumFunc );
	ao_iPosBeforeExec = -1;

	// Internal functions M4CL_INTERNAL_FUNC will not be check
	if ( oFunction.GetType() != M4CL_INTERNAL_FUNC ) {

		// Check all functions except internal functions
		bCheckExec = M4_TRUE;

		// Arguments = No.Arg(maybe variable) + No.ArgRef + Resultado(1)
		iNumArg		 = oFunction.GetNumArgs();			// No.Total Arguments = Valor + Referencia
		ao_iNumArgRef= oFunction.GetNumReferedArgs();	// No.Reference Arguments

		// If number of arguments is variable for this function, we read it from the stack
		if (iNumArg <0 ) {
			// Read number of value arguments BEFORE EXECUTION
			ret=RuntimeTop(vNumArg);
			M4_ASSERT(ret==M4_SUCCESS);
			iNumArg = int(vNumArg.Data.DoubleData) + 1;	// No.Total Real Arguments = No.Arg.+ itself
		
		}
		
		// Position in the Stack: Base = Actual Position - No.Total Arguments
		// Have to be the same Position + No.Reference Arguments + Result(1) after execution
		//	ao_iPosAfterExec == m_oRunTimeStack->GetPos() - iNumArg + ao_iNumArgRef + 1;
		ao_iPosBeforeExec = m_oRunTimeStack->GetPos() - iNumArg;

	}

	return bCheckExec;
}

#endif

#ifdef _DEBUG
// Check the execution of the function
// In :	ai_ui16NumFunc	= No.function
//		ai_iNumArgRef	= No.Referenced arguments
//		ai_iPosBeforeExec = Stack Position before execution
//		ai_ret			= Result execution of the function
//		ai_bCheckExec	= Check Execution Y/N
// Out:	return	= Check status: Error or Success
m4return_t ClVMachine0::CheckEndExec(m4uint16_t	ai_ui16NumFunc, m4int8_t ai_iNumArgRef, m4int32_t ai_iPosBeforeExec, m4return_t ai_ret, m4bool_t ai_bCheckExec) {

	m4int32_t	iPosAfterExec;	// Stack Position After Execution	
	m4int8_t	iReturnValue = 1;

	// Check only if execution is Ok and check is possible
	if ( (ai_ret == M4_SUCCESS) && ai_bCheckExec ) {
		
		// "Wired functions" will be check special (see TablFunc.cpp) because they doesn't return a value
		// - Load_prg, 0, 239, ""

		switch( ai_ui16NumFunc )
		{
			case 239:	// load_prg
			case 241:	// delete_prg
			case 243:	// update_prg
			case 245:	// insert_prg

				iReturnValue = 0 ;
				break ;

			case 246:	// persist_tree

				iReturnValue = 2 ;
				break ;

			case 209:	// clcExecuteJIT
			case 210:	// clcExecuteLN4JIT
			case 90:	//EndTransaction (por si ha habido rollback)
			case 149:	//LocalTest
			case 140:	//ExternalMethodCall
			case 79:	//Call
			case 372:	//CallNode
			case 271:	//RemoteCall
			case 230:   //channel call
			case M4CL_VM_FT_ASSIGNFIXEDARGUMENTINVARARGSITEM:	//AssignFixedArgumentInVarArgsItem
			case M4CL_VM_FT_READFIXEDARGUMENTINVARARGSITEM:		//ReadFixedArgumentInVarArgsItem
                return M4_SUCCESS;
		}

		// Position in the Stack AFTER EXECUTION:	Base + No.Reference Arg. + Result(1/0)
		iPosAfterExec = m_oRunTimeStack->GetPos();

		// Check if the position in the stack is OK
		if ( iPosAfterExec != (ai_iPosBeforeExec + ai_iNumArgRef + iReturnValue)  ) {
			ClFunctionDesc& oFunction = m_poFunctionTable->GetObjectFunction(ai_ui16NumFunc);			
			DUMP_CHLOG_WARNINGF(M4_CH_VM_TRACE_STACK_VM_GENERIC, iPosAfterExec - (ai_iPosBeforeExec + ai_iNumArgRef + 1)<<ai_ui16NumFunc<<oFunction.GetName() );
			return M4_ERROR;
		}
		
	}
	return M4_SUCCESS;

}

#endif

m4return_t ClVMachine0::ExecuteFunctionForLocalTest (m4uint16_t iNumFunction){ //para que desde el local test puedan ejecutar funciones

	return ExecuteFunction( iNumFunction, m_oImplementors, m_oState ) ;
}



void	ClVMachine0::SetStepperStates( void )
{

	m4int32_t		i = 0 ;
	m4int32_t		iLength = 0 ;
	m4VMState_t		*poState = NULL ;


	if( m_oStepper.GetActive() == M4_TRUE )
	{
		iLength = m_oStateStack->GetPos() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poState = m_oStateStack->GetPointer( i ) ;

			if( poState != NULL )
			{
				poState->m_iStepperType = StepperTypeParent ;
			}
		}

		if( iLength >= 0 )
		{
			poState = m_oStateStack->GetPointer( iLength ) ;

			if( poState != NULL )
			{
				poState->m_iStepperType = StepperTypeActual ;
			}
		}
	}
}


void	ClVMachine0::ResetStepper( void )
{

	m4uint32_t		i = 0 ;
	m4uint32_t		iLength = 0 ;
	m4VMState_t		*poState = NULL ;


	if( m_oStepper.GetActive() == M4_TRUE )
	{
		iLength = m_oStateStack->GetPos() + 1 ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poState = m_oStateStack->GetPointer( i ) ;

			if( poState != NULL )
			{
				poState->m_iStepperType = StepperTypeNormal ;
			}
		}

		m_oStepper.SetMode( StepperModeNormal ) ;
	}
}



void ClVMachine0::RefreshState(void){
		//recuperamos el estado
		m_oStateStack->Top(m_oState);

}


void ClVMachine0::SetpCancelStatus(ClStatus *ai_poCancelStatus)
{
	m_poCancelStatus = ai_poCancelStatus;
}

m4bool_t ClVMachine0::GetCancelStatus(void)
{
	/* Bug 0175560
	Se chequea el estado de manera recursiva
	*/
	ClStatus*	poStatus = m_poCancelStatus ;

	while( poStatus != NULL )
	{
		if( poStatus->GetIntrValue() != ClStatus::STATUS_INTR_NORMAL )
		{
			return( M4_TRUE ) ;
		}
		poStatus = poStatus->GetNextStatus() ;
	}

	return M4_FALSE;
}

/**
 * Petición para abortar una ejecución.
 * Es distinto del RequestToAbort porque tiene en cuenta si tenemos asignado un ClStatus
 * (por ejemplo en el caso de estar ejecutando en el servidor). En tal caso la petición para
 * abortar se hace a través de este objeto.
 *
 * @param ai_bRequestToAbort    M4_TRUE  --> se pide que se aborte la ejecución.
 *								M4_FALSE --> se pide que la ejecución continue con normalidad.
 */
void ClVMachine0::RequestToAbortWithClStatus(const m4bool_t ai_bRequestToAbort) {
	if (!m_poCancelStatus) {
		RequestAbort(ai_bRequestToAbort);
	}
	else {
		if (ai_bRequestToAbort == M4_TRUE) {
			m_poCancelStatus->SetIntrValue(ClStatus::STATUS_INTR_QUIT);
		}
		else {
			m_poCancelStatus->SetIntrValue(ClStatus::STATUS_INTR_NORMAL);
		}
	}
}

/**
 * Nos devuelve si ha sido abortada una ejecución.
 */ 
m4bool_t ClVMachine0::GetRequestToAbortWithClStatus(void) {
	if (m_poCancelStatus == NULL)
		return m_bRequestAbortion;

	return (m_poCancelStatus->GetIntrValue() != ClStatus::STATUS_INTR_NORMAL) ? M4_TRUE : M4_FALSE;
}

//----------------------------
//Argumentos
//----------------------------
//----------------------------------------------------------------------------------
//Funciones privadas pare obtener el número de argumetnos de un item
//y los argumetnos en sí
//Para obtener los argumentos usan la referencia de la pila que se le pase, habitualmente
//sera m_oRunTimeStack->GetReference();
//-----------------------------------------------------------------------------------
m4uint32_t ClVMachine0::GetRunningItemArgumentNumber (const m4VMState_t &ai_oState, m4int32_t ai_iRef){
						
	m4VariantType vNumArgs;
    ClCompiledMCR   *poCMCR=ai_oState.m_poAccess-> GetpCMCR() ;
	m4uint32_t iNumArgs;

	m4uint32_t iVariable= poCMCR->GetItemVariableArguments ( ai_oState.m_hItem);
	
	if (iVariable != 0){
		//quitamos ref
		m4int32_t iOldRef=m_oRunTimeStack->GetReference();
		m_oRunTimeStack->SetReference(0);

		//leemos
		m_oRunTimeStack->GetElement(vNumArgs, ai_iRef-1);

		//restauramos ref
		m_oRunTimeStack->SetReference(iOldRef);

		iNumArgs=m4uint32_t(vNumArgs.Data.DoubleData);
	}
	else{
		iNumArgs=poCMCR->GetItemNumberOfArguments (ai_oState.m_hItem);
	}
	return iNumArgs;
	
}

m4return_t ClVMachine0::GetRunningItemArgumentByPosition		 (const m4VMState_t &ai_oState, m4int32_t ai_iRef, m4uint32_t ai_iPosition, m4VariantType &ao_vArg){
	
	m4return_t ret;
	m4uint32_t iNumArgs;
	m4VariantType vTemp;

    ClCompiledMCR   *poCMCR=ai_oState.m_poAccess-> GetpCMCR() ;

	
	//comprobamos que exista
	m4uint32_t iVariable= poCMCR->GetItemVariableArguments ( ai_oState.m_hItem);
	iNumArgs= GetRunningItemArgumentNumber(ai_oState,ai_iRef);

	if (ai_iPosition > iNumArgs || ai_iPosition <=0){
		return M4_ERROR;
	}
	if (iVariable != 0)	--ai_iPosition; //para que lea del sitio correcto, ya que el -1 es el número de argumentos
		
	//retornamos el argumento

	//quitamos ref
	m4int32_t iOldRef=m_oRunTimeStack->GetReference();
	m_oRunTimeStack->SetReference(0);
	
	//leemos
	ret=m_oRunTimeStack->GetElement(vTemp, ai_iRef-m4int32_t(iNumArgs-ai_iPosition+1)  );

	//restauramos ref
	m_oRunTimeStack->SetReference(iOldRef);

	if (vTemp.Type==M4CL_CPP_TYPE_STRING_POOL){
		ao_vArg.SetString(m_oDynamicPool->GetString(vTemp.Data.CharData), vTemp.IsUser );
	}
	else{
		ao_vArg=vTemp;
	}
	return ret;
}


//----------------------------------------
//Runtime info - stepper
//----------------------------------------

m4uint32_t ClVMachine0::GetNumberOfVariables (m4uint32_t ai_iCallStackPos){
	m4VMState_t		oState;
	m4int32_t		iRTStackRef;
	m4return_t		ret;

	ret=LocateStateAndRef(ai_iCallStackPos, oState, iRTStackRef);
	if (ret!=M4_SUCCESS || oState.m_poFormula.IsNull()) {
		return 0;
	}
	return oState.m_poFormula.GetNumVars();
}

//las variables las numeramos de 0 a n-1, aunque en la polaca las guademos de 1 a n
m4return_t ClVMachine0::GetVariableValue (m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ao_vValue){

	m4VMState_t		oState;
	m4int32_t		iRTStackRef;
	m4return_t		ret;
	m4int32_t		iNumVars;

	ret=LocateStateAndRef(ai_iCallStackPos, oState, iRTStackRef);
	if (ret!=M4_SUCCESS || oState.m_poFormula.IsNull()) {
		return M4_ERROR;
	}

	iNumVars = oState.m_poFormula.GetNumVars();
	if (ai_ivar<0 || ai_ivar>=iNumVars) {
		return M4_ERROR;
	}

	ret = _GetVariableValue (iRTStackRef, ai_ivar, ao_vValue);

	return ret;
}

m4return_t ClVMachine0::SetVariableValue (m4uint32_t ai_iCallStackPos, m4int32_t ai_ivar, m4VariantType &ai_vValue){
	m4VMState_t		oState;
	m4int32_t		iRTStackRef;
	m4return_t		ret;
	m4int32_t		iNumVars;

	ret=LocateStateAndRef(ai_iCallStackPos, oState, iRTStackRef);
	if (ret!=M4_SUCCESS || oState.m_poFormula.IsNull()) {
		return M4_ERROR;
	}

	iNumVars = oState.m_poFormula.GetNumVars();
	if (ai_ivar<0 || ai_ivar>=iNumVars) {
		return M4_ERROR;
	}

	ret = _SetVariableValue (iRTStackRef, ai_ivar, ai_vValue);

	return ret;
}


m4uint32_t ClVMachine0::GetNumberOfArguments (m4uint32_t ai_iCallStackPos){
	m4VMState_t		oState;
	m4int32_t		iRTStackRef;
	m4return_t		ret;

	ret=LocateStateAndRef(ai_iCallStackPos, oState, iRTStackRef);
	if (ret!=M4_SUCCESS || oState.m_poFormula.IsNull()) {
		return 0;
	}
	return GetRunningItemArgumentNumber(oState, iRTStackRef);
}

m4return_t ClVMachine0::GetArgumentValue (m4uint32_t ai_iCallStackPos, m4int32_t ai_iArg, m4VariantType &ao_vValue){

	m4VMState_t		oState;
	m4int32_t		iRTStackRef;
	m4return_t		ret;
	m4int32_t		iNumArgs;
	m4int32_t		iPosition;
	m4uint32_t		iVariable;
	ClCompiledMCR   *poCMCR;

	ret=LocateStateAndRef(ai_iCallStackPos, oState, iRTStackRef);
	if (ret!=M4_SUCCESS || oState.m_poFormula.IsNull()) {
		return M4_ERROR;
	}

	iNumArgs = GetRunningItemArgumentNumber(oState, iRTStackRef);
	if (ai_iArg<0 || ai_iArg>=iNumArgs) {
		return M4_ERROR;
	}

    poCMCR=oState.m_poAccess->GetpCMCR() ;
	iVariable=poCMCR->GetItemVariableArguments(oState.m_hItem);

	iPosition=ai_iArg-iNumArgs;

	if (iVariable==1){
		iPosition--;
	}

	ret = _GetVariableValue (iRTStackRef, iPosition, ao_vValue);

	return ret;
}

m4return_t ClVMachine0::SetArgumentValue (m4uint32_t ai_iCallStackPos, m4int32_t ai_iArg, m4VariantType &ai_vValue){
	m4VMState_t		oState;
	m4int32_t		iRTStackRef;
	m4return_t		ret;
	m4int32_t		iNumArgs;
	m4int32_t		iPosition;
	m4uint32_t		iVariable;
	ClCompiledMCR   *poCMCR;

	ret=LocateStateAndRef(ai_iCallStackPos, oState, iRTStackRef);
	if (ret!=M4_SUCCESS || oState.m_poFormula.IsNull()) {
		return M4_ERROR;
	}

	iNumArgs = GetRunningItemArgumentNumber(oState, iRTStackRef);
	if (ai_iArg<0 || ai_iArg>=iNumArgs) {
		return M4_ERROR;
	}

    poCMCR=oState.m_poAccess->GetpCMCR() ;
	iVariable=poCMCR->GetItemVariableArguments(oState.m_hItem);

	iPosition=ai_iArg-iNumArgs;

	if (iVariable==1){
		iPosition--;
	}

	ret = _SetVariableValue (iRTStackRef, iPosition, ai_vValue);

	return ret;
}


m4uint32_t ClVMachine0::GetCallStackSize (void){
	
	m4int32_t		iPos; //posición en la pila del estado que estamos tratando
	m4uint32_t		iSize=0;
	m4VMState_t		oState;
	m4int32_t		iRef;
	iRef = m_oStateStack->GetReference();
	m_oStateStack->SetReference(0); //para poder usar el GetElement sin problemas
	iPos = m_oStateStack->GetPos();

	while (iPos!=-1){
		m_oStateStack->GetElement(oState,iPos);
		if (oState.m_hItem == -1){
			--iPos;
		}
		else{
			iPos=oState.m_iParentStatePos;
			if (!M4CL_HAS_STATE_HIDE_FLAG(oState.m_Flags) ) {
				iSize++;;
			}
		}
		
	}

	m_oStateStack->SetReference(iRef); 
	return iSize;
}

m4return_t ClVMachine0::GetCallStackInfo (m4uint32_t ai_iCallStackPos, ClRunningFormInfo &ao_oInfo){

	m4VMState_t		oState;
	m4int32_t		iRTStackRef;
	
	m4return_t ret = LocateStateAndRef(ai_iCallStackPos, oState, iRTStackRef);
	if (ret!=M4_SUCCESS) {
		return ret;
	}
	//la hemos encontrado.Rellenamos la informacíón!!!!

	ClCompiledMCR* poCMCR=oState.m_poAccess->GetpCMCR() ;
			
	ret=ao_oInfo.Fill(oState); //lo comun de executionpoint
	if (ret!=M4_SUCCESS) {
		return ret;
	}

	//las fechas
	ao_oInfo.SetDates (oState.m_dBeginDate , oState.m_dEndDate , oState.m_dDate );
	//handles
	ao_oInfo.SetChannelHandle (oState.m_poAccess->GetpChannel()->GetHandle());
	ao_oInfo.SetAccessHandle (oState.m_poAccess->GetHandle());
	//CS side
	if (M4CL_HAS_STATE_NOT_EXE_FLAG(oState.m_Flags)){
		ao_oInfo.SetIsClientSide (M4_FALSE);
	}
	else{
		ao_oInfo.SetIsClientSide (M4_TRUE);
	}
	//número de argumentos, si es de argumentos variables y los argumentos en sí
	ao_oInfo.SetIsVariable (poCMCR->GetItemVariableArguments(oState.m_hItem)?M4_TRUE:M4_FALSE );
	m4uint32_t iNumArgs = 0;
	/* Corrige el bug 0047847.
	 * Si se trata de un método c++ y ya se ha ejecutado (lo sabemos por la instrucción actual), se dice que el número
	 * de argumentos es 0, puesto que estos ya no están disponibles en la pila (se sacaron al ejecutar el método).
	 */
	if (oState.m_poFormula.GetType() == M4CL_LENGUAGE_LN4 || oState.m_iCurrent < 2) {
		iNumArgs = GetRunningItemArgumentNumber(oState, iRTStackRef);
	}
	ao_oInfo.SetNumberofArguments(iNumArgs);

	m4VariantType var;	
	for (m4uint32_t i = 0; i < iNumArgs; ++i){
		ret=GetRunningItemArgumentByPosition (oState,iRTStackRef,i+1, var);
		if (ret==M4_SUCCESS){
			ao_oInfo.SetArgument (var,i);
		}
		else{
			return ret;
		}
	}
	return M4_SUCCESS;
}



m4return_t	ClVMachine0::LocateStateAndRef(m4uint32_t ai_iCallStackPos, m4VMState_t &ao_oState, m4int32_t &ao_iRef){

	m4int32_t		iPos; //posición en la pila del estado que estamos tratando
	m4uint32_t		iCurrentPos=0;
	m4int32_t		iRef;
	
	iRef = m_oStateStack->GetReference();
	m_oStateStack->SetReference(0); //para poder usar el GetElement sin problemas

	ao_iRef=m_oRunTimeStack->GetReference();
	iPos = m_oStateStack->GetPos();

	//localizamos el estado en cuestion
	while (iPos!=-1){
		m_oStateStack->GetElement(ao_oState,iPos);
		if (ao_oState.m_hItem == -1){
			--iPos;
		}
		else{
			if (!M4CL_HAS_STATE_HIDE_FLAG(ao_oState.m_Flags) ) {
				if (iCurrentPos!=ai_iCallStackPos){
					iCurrentPos++;
				}
				else{
					break; //salimos, ya lo hemos encontrado
				}
			}
			iPos=ao_oState.m_iParentStatePos;
		}
		/* Bug 0081440
		La referencia anterior hay que actualizarla siempre, aunque sea
		un estado que no se muestra
		*/
		ao_iRef=ao_oState.m_iOldStackReference ;
	}

	m_oStateStack->SetReference(iRef); 

	return (iPos == -1 ? M4_ERROR :M4_SUCCESS);
}


/*
 * Devuelve el valor de una variable de la pila.
 * Vale tanto para argumentos como para variables locales.
 * No comprueba la valided de la posición.
 */
m4return_t ClVMachine0::_GetVariableValue (m4int32_t ai_iRef, m4int32_t ai_ivar, m4VariantType &ao_vValue){
	m4int32_t		iOldRef;
	m4return_t		ret;
	m4VariantType	vVar;

	//guardamos la referencia antigua de la pila
	iOldRef=m_oRunTimeStack->GetReference();
	//ponemos la nueva
	m_oRunTimeStack->SetReference(ai_iRef);

	//ya podemos leer de la pila
	ret=m_oRunTimeStack->GetElement(vVar, ai_ivar); 
	
	if (vVar.Type==M4CL_CPP_TYPE_STRING_POOL){
		ao_vValue.SetString(m_oDynamicPool->GetString(vVar.Data.CharData), vVar.IsUser );
	}
	else{
		ao_vValue=vVar;
	}

	//restauramos la referencia
	m_oRunTimeStack->SetReference(iOldRef);

	return ret;
}

/*
 * Cambia el valor de una variable de la pila.
 * Vale tanto para argumentos como para variables locales.
 * No comprueba la valided de la posición.
 */
m4return_t ClVMachine0::_SetVariableValue (m4int32_t ai_iRef, m4int32_t ai_ivar, m4VariantType &ai_vValue){
	m4int32_t		iOldRef;
	m4return_t		ret;
	m4VariantType	vVar;

	//guardamos la referencia antigua de la pila
	iOldRef=m_oRunTimeStack->GetReference();
	//ponemos la nueva
	m_oRunTimeStack->SetReference(ai_iRef);

	//ya podemos leer de la pila
	ret=m_oRunTimeStack->GetElement(vVar, ai_ivar); 
	
	//si la variable tenía una cadena ->la borramos
	if (vVar.Type==M4CL_CPP_TYPE_STRING_POOL){
		m_oDynamicPool->DeleteString(vVar.Data.CharData);
	}

	//si el nuevo valor es cadena lo metemos en el pool y asignamos
	if (ai_vValue.Type==M4CL_CPP_TYPE_STRING_VAR){
		m_oDynamicPool->AddString(ai_vValue.Data.PointerChar,vVar.Data.CharData);
		vVar.IsUser=ai_vValue.IsUser;
		vVar.Type=M4CL_CPP_TYPE_STRING_POOL;
		ret=m_oRunTimeStack->SetElement(vVar, ai_ivar);
	}
	else{//si no, asignamos tal cual
		ret=m_oRunTimeStack->SetElement(ai_vValue, ai_ivar);
	}

	//restauramos la referencia
	m_oRunTimeStack->SetReference(iOldRef);

	return ret;
}




/*
Añade una función al grupo de funciones a excluir
Si la función no existe da un error
Si ya existe no se hace nada
Si no está creado el grupo se crea
*/
m4return_t	ClVMachine0::SetExcludeFunction( m4pcchar_t ai_pccFunction )
{

	m4return_t	iResult ;
	m4uint16_t	iFunction ;


	// Si la función es nula se da un error
	CHECK_CHLOG_ERRORF( ai_pccFunction == NULL, M4_ERROR, M4_CH_VM_NO_SUCH_FUNCTION, "NULL" ) ;

	iResult = m_poFunctionTable->GetId( ai_pccFunction, iFunction ) ;

	// Si la función no existe se da un error
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_VM_NO_SUCH_FUNCTION, ai_pccFunction ) ;

	if( m_poExcludeFunctions == NULL )
	{
		m_poExcludeFunctions = new ExcludeFunctionsSet_t ;
		CHECK_CHLOG_ERRORF( m_poExcludeFunctions == NULL, M4_ERROR, M4_CH_VM_OUT_OF_MEMORY, m4uint32_t( sizeof( ExcludeFunctionsSet_t ) ) << __FILE__ << __LINE__ ) ;
	}

	m_poExcludeFunctions->insert( iFunction ) ;


	return( M4_SUCCESS ) ;
}



/*
Elimina una función del grupo de funciones a excluir
Si la función no existe da un error
Si ya no quedan funciones se borra el grupo
*/
m4return_t	ClVMachine0::UnsetExcludeFunction( m4pcchar_t ai_pccFunction )
{

	m4return_t	iResult ;
	m4uint16_t	iFunction ;


	// Si la función es nula se da un error
	CHECK_CHLOG_ERRORF( ai_pccFunction == NULL, M4_ERROR, M4_CH_VM_NO_SUCH_FUNCTION, "NULL" ) ;

	iResult = m_poFunctionTable->GetId( ai_pccFunction, iFunction ) ;

	// Si la función no existe se da un error
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_VM_NO_SUCH_FUNCTION, ai_pccFunction ) ;

	if( m_poExcludeFunctions != NULL )
	{
		m_poExcludeFunctions->erase( iFunction ) ;

		if( m_poExcludeFunctions->empty() )
		{
			delete( m_poExcludeFunctions ) ;
			m_poExcludeFunctions = NULL ;
		}
	}


	return( M4_SUCCESS ) ;
}



/*
Elimina todas las funciones del grupo de funciones a excluir
Se borra el grupo
*/
m4return_t	ClVMachine0::ClearExcludeFunctions( void )
{
	if( m_poExcludeFunctions != NULL )
	{
		m_poExcludeFunctions->clear() ;
		delete( m_poExcludeFunctions ) ;
		m_poExcludeFunctions = NULL ;
	}

	return( M4_SUCCESS ) ;
}



/*
Comprueba si se puede o no ejecutar una función, según la lista de
funciones a excluir
Si no se puede ejecutar da un error
Si se puede ejecutar la ejecuta
*/
m4return_t	ClVMachine0::ExecuteFunction( m4uint16_t ai_iFunction, ClVVM **ai_ppoImplementors, m4VMState_t& ai_roState )
{

	m4return_t	iResult ;
	m4uint16_t	iObject ;
	pmfVM0		pfFunction ;


	if( m_poExcludeFunctions != NULL )
	{
		if( m_poExcludeFunctions->find( ai_iFunction ) != m_poExcludeFunctions->end() )
		{
			ClCompiledMCR	*poCMCR = ai_roState.m_poAccess->GetpCMCR() ;
			DUMP_CHLOG_ERRORF( M4_CH_VM_EXCLUDED_FUNCTION, ClFuncTableDesc::GetInstance()->GetName( ai_iFunction ) << poCMCR->GetItemFormIdByPosition( ai_roState.m_hItem, ai_roState.m_iRule ) << M4ObjNodeItemI( poCMCR, ai_roState.m_hItem ) << DumpCallStack( &m_oState ) ) ;
			return( M4_ERROR ) ;
		}
	}

#ifdef	_M4_TRACE_POOL
	m4pchar_t	pcM4Obj = NULL ;
	m4pcchar_t	pccNode = NULL ;
	m4pcchar_t	pccItem = NULL ;

	ClAccess*	poAccess = ai_roState.m_poAccess ;

	if( poAccess != NULL )
	{
		ClChannel*	poChannel = poAccess->GetpChannel() ;

		if( poChannel != NULL )
		{
			poChannel->MetaIdentifier.Get( pcM4Obj ) ;
		}
	}

	ClNodeDef*	poNodeDef = ai_roState.m_poRecordSet->GetpNodeDef() ;

	if( poNodeDef != NULL )
	{
		pccNode = poNodeDef->Id() ;
	}

	ClCompiledMCR*	poCMCR = poNodeDef->GetpCMCR() ;

	if( poCMCR != NULL )
	{
		pccItem = poCMCR->GetItemId( ai_roState.m_hItem ) ;
	}

	PoolTrace( "Function", pcM4Obj, pccNode, pccItem, ai_roState.m_iSourceLine - 1, ai_iFunction ) ;
#endif

	iObject = m_poFunctionTable->GetIdObject( ai_iFunction ) ;
	pfFunction = m_poFunctionTable->GetExeFunc( ai_iFunction ) ;

	iResult = ( ai_ppoImplementors[ iObject ]->*( pfFunction ) )() ;

	return( iResult ) ;
}

