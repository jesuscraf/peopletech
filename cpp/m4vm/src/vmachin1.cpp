//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                vMachin1.cpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:             21 may 97
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


#include "vmres.hpp"

#include "vmachin1.h"
#include "tablfunc.h"
#include "vmattrib.hpp"
#include "vm1pr.hpp"
#include "sttlib.hpp"
#include "instru.h"


#include "m4mdrt.hpp" 
#include "m4log.hpp"
#include "m4ldb.hpp"
#include "access.hpp"
#include "channel.hpp"
#include "cldbgutil.hpp"
#include "m4objglb.hpp"
#include "m4objreg.hpp"

#include "cltnform.hpp"
#include "nodedef.hpp"

// User actions auditory.
#include "usractaudit.hpp"


ClVMachine1::ClVMachine1 (	tpFixedStack<m4VariantType> *ai_oRunTimeStack,
							ClStringPool				*ai_oDynamicPool,
							tpFixedStack<m4VMState_t>	*ai_oStateStack,
							ClVMCSContext				*ai_poVMCSContext)

						:ClVMachine0 (ai_oRunTimeStack,ai_oDynamicPool, ai_oStateStack,ai_poVMCSContext),
						m_oImplPropagator	(m_oState1),
						m_oImplLDB			(m_oState1),
						m_oImplIndirections (m_oState1),
						m_oImplCompilerAndJit (m_oState1),
						m_oImplFiltFind		(m_oState1),
						m_oImplMcr			(m_oState1),
						m_oImplRegister		(m_oState1),
						m_oImplLib			(m_oState1),
						m_oImplPayroll		(m_oState1){


    

	//Como la pila de fechas es redimensionable usamos un tamaño inicial pequeño.
	m_oDatesStack.Init (0.0, 0.0, M4CL_INITIAL_STACK_SLICES_NUM);
    m_oResultStack=new tpFixedStack<m4VariantType>(M4CL_MAX_PARAM*2+2); //argumetnos por ref y resultado
    m_oReenterStack=new tpFixedStack<m4int32_t>(M4CL_MAX_REENTER);
	
    m_dUnit=1;
    m_iReenter=-1;
	m_iSerializeFrom=-1;

	m_poAttribute = new ClVMItemAttribute ();

	//iniciamos el objeto de los atributos
	m_poAttribute->SetRuntimeStack	(m_oRunTimeStack);
	m_poAttribute->SetStateStack	(m_oStateStack);
	m_poAttribute->SetPool			(m_oDynamicPool);
	m_poAttribute->SetMediator		(m_oVariantMediator);

	//iniciamos los objetos implementadores
	m_oImplementors[M4CL_VM_OBJECT_THIS]	=	this;
	m_oImplementors[M4CL_VM1_PROPAGATOR]	=	&m_oImplPropagator;
	m_oImplementors[M4CL_VM1_LDB]			=	&m_oImplLDB;
	m_oImplementors[M4CL_VM1_INDIRECTION]	=	&m_oImplIndirections;
	m_oImplementors[M4CL_VM1_COMPILER_JIT]	=	&m_oImplCompilerAndJit;
	m_oImplementors[M4CL_VM1_SORT_FILTER]	=	&m_oImplFiltFind;
	m_oImplementors[M4CL_VM1_CMCR]			=	&m_oImplMcr;
	m_oImplementors[M4CL_VM1_REGISTER]		=	&m_oImplRegister;
	m_oImplementors[M4CL_VM1_LIB]			=	&m_oImplLib;			
	m_oImplementors[M4CL_VM1_PAYROLL]		=	&m_oImplPayroll;			


	//iniciamos los objetos
		//el de propagacion
	m_oImplPropagator.SetRuntimeStack	(m_oRunTimeStack);
	m_oImplPropagator.SetStateStack		(m_oStateStack);
	m_oImplPropagator.SetPool			(m_oDynamicPool);
	m_oImplPropagator.SetMediator		(m_oVariantMediator);
	m_oImplPropagator.SetVM				(this);

		//el de LDB
	m_oImplLDB.SetRuntimeStack	(m_oRunTimeStack);
	m_oImplLDB.SetStateStack	(m_oStateStack);
	m_oImplLDB.SetPool			(m_oDynamicPool);
	m_oImplLDB.SetMediator		(m_oVariantMediator);
	m_oImplLDB.SetVM			(this);

		//el de indirecciones
	m_oImplIndirections.SetRuntimeStack	(m_oRunTimeStack);
	m_oImplIndirections.SetStateStack	(m_oStateStack);
	m_oImplIndirections.SetPool			(m_oDynamicPool);
	m_oImplIndirections.SetMediator		(m_oVariantMediator);
	m_oImplIndirections.SetVM			(this);

			//compiler and jit
	m_oImplCompilerAndJit.SetRuntimeStack	(m_oRunTimeStack);
	m_oImplCompilerAndJit.SetStateStack		(m_oStateStack);
	m_oImplCompilerAndJit.SetPool			(m_oDynamicPool);
	m_oImplCompilerAndJit.SetMediator		(m_oVariantMediator);
	m_oImplCompilerAndJit.SetVM				(this);

				//busquedas ordenaciones y filtros
	m_oImplFiltFind.SetRuntimeStack	(m_oRunTimeStack);
	m_oImplFiltFind.SetStateStack	(m_oStateStack);
	m_oImplFiltFind.SetPool			(m_oDynamicPool);
	m_oImplFiltFind.SetMediator		(m_oVariantMediator);

				//funciones del metacanal
	m_oImplMcr.SetRuntimeStack	(m_oRunTimeStack);
	m_oImplMcr.SetStateStack	(m_oStateStack);
	m_oImplMcr.SetPool			(m_oDynamicPool);
	m_oImplMcr.SetMediator		(m_oVariantMediator);
	m_oImplMcr.SetVM			(this);

			//compiler and jit
	m_oImplRegister.SetRuntimeStack	(m_oRunTimeStack);
	m_oImplRegister.SetStateStack	(m_oStateStack);
	m_oImplRegister.SetPool			(m_oDynamicPool);
	m_oImplRegister.SetMediator		(m_oVariantMediator);
	m_oImplRegister.SetVM			(this);

			//el de librería
	m_oImplLib.SetRuntimeStack		(m_oRunTimeStack);
	m_oImplLib.SetStateStack		(m_oStateStack);
	m_oImplLib.SetPool				(m_oDynamicPool);
	m_oImplLib.SetMediator			(m_oVariantMediator);
	m_oImplLib.SetVM				(this);

				//nomina
	m_oImplPayroll.SetRuntimeStack		(m_oRunTimeStack);
	m_oImplPayroll.SetStateStack		(m_oStateStack);
	m_oImplPayroll.SetPool				(m_oDynamicPool);
	m_oImplPayroll.SetMediator			(m_oVariantMediator);
	m_oImplPayroll.SetVM				(this);

	SetContext(ai_poVMCSContext);

	Reset();
}

//
//SetContext
//
void ClVMachine1::SetContext	(ClVMCSContext *ai_poContext){
	m_poContext=ai_poContext;
	//iniciamos los implementors
	m_oImplPayroll.SetVMCSContext		(ai_poContext);
	m_oImplLib.SetVMCSContext		(ai_poContext);
	m_oImplRegister.SetVMCSContext	(ai_poContext);
	m_oImplMcr.SetVMCSContext	(ai_poContext);
	m_oImplFiltFind.SetVMCSContext	(ai_poContext);
	m_oImplCompilerAndJit.SetVMCSContext	(ai_poContext);
	m_oImplIndirections.SetVMCSContext	(ai_poContext);
	m_oImplLDB.SetVMCSContext	(ai_poContext);
	m_oImplPropagator.SetVMCSContext	(ai_poContext);
	m_poAttribute->SetVMCSContext	(ai_poContext);
		
	ClVMachine0::SetContext (ai_poContext);
}


ClVMachine1::~ClVMachine1(){
    if (m_oResultStack!=NULL){
		delete m_oResultStack;
        m_oResultStack=NULL;
    }
    
    if (m_poAttribute){
        delete m_poAttribute;
        m_poAttribute=NULL;
    }
    
    if (m_oReenterStack){
        delete 	m_oReenterStack;
		m_oReenterStack=NULL;
    }


}

//EDU:
m4uint32_t ClVMachine1::GetSize (void) const
{
    m4uint32_t length = 0;
	
    length += sizeof (ClVMachine1);
    length += m_oResultStack->GetSize ();
    length += sizeof (ClVMItemAttribute);
    length += ClVMachine0::GetSize ();
    length -= sizeof (ClVMachine0);
	
	length += m_oReenterStack->GetSize();
    length += m_oDatesStack.GetSize() * sizeof (m4date_t);
	length += m_oImplCompilerAndJit.GetSize();
	length += m_oImplPayroll.GetSize();
	length -= sizeof (m_oImplPayroll);
	length -= sizeof (m_oImplCompilerAndJit);
	
	return length;
}


void ClVMachine1::Reset		(void){
	m_iReenter=-1;
	m_iSerializeFrom=-1;
	m_BeginTransaction = M4CL_NO_VM_TRANSACTION_DEFINED;

	while (!m_oStateStack->IsEmpty()   ) m_oStateStack->Pop();
	while (!m_oRunTimeStack->IsEmpty() ) RuntimePopAndDelete(); 
	while (!m_oReenterStack->IsEmpty() ) m_oReenterStack->Pop();
	m_oReenterStack->Push  (m_iReenter);
	m_oRunTimeStack->SetReference(0);
	m_oStateStack->SetReference(0);
	ResetStepper();

	// ok, lets clear the string pool also
	m_oDynamicPool->Reset();
}

//---------------------------------------------------------------------
//Exectute real de nivel 1
//---------------------------------------------------------------------

m4return_t ClVMachine1::Execute(void){
	
	m4uint16_t ui16NumFunc;

	m4return_t ret,ret2 = M4_SUCCESS;
    m4return_t alret;    
	m4VMState_t oState;
	eVMStepperMode eModeStep = m_oStepper.GetMode();
	
	//	
	//Continuar en remoto
	//
	//si lo que hay en la pila no es ejecutable->debemos forzar un remoteCall
	ret = m_oStateStack->Top(m_oState1);
	if (ret==M4_SUCCESS && M4CL_HAS_STATE_NOT_EXE_FLAG(m_oState1.m_Flags) ){

		//Forzamos el remote call
		ret= GetVMCSContext()->RemoteCall( (ClExecutor*)m_oState1.m_poAccess->GetpExecutor(), m_oState1.m_hItem, m_oState1.m_poRecordSet, m_oStepper.GetActive() );
		
		switch (ret){

		case M4_ERROR:
		case M4CL_USER_ERROR_ABORT_TRANSACTION:
		case M4CL_USER_ABORT_ALL_REQUEST:	 
			//ha habido un error en el otro lado o han abortado, intentamos recuperarnos
			if (m_oStateStack->Top(oState)!=M4_SUCCESS) return ret; //puede que hayan abortado todo
			//intentamos recuperarnos- Si retorna error es que no podemos recuperarnos 
			ret=RecoverFromError(oState.m_poAccess);
			ret2=ret; //en el caso de que no consigamos recuperarnos retornará ret2, lo asignamos a M4_ERROR.
			break;
		case M4CL_EXECUTION_INTERRUPTION:
			return M4CL_EXECUTION_INTERRUPTION; //si han interrumpido en el otro lado, que salga interrumpiendo, 
		default:
			//ha dado exito, dejamos que siga ejecutando
			break;
		}
	}


	//mandamos ejecutar al nivel 0 y tomamos decisiones mientras halla algo que ejecutar

	while (m_iReenter< m_oStateStack->GetPos() && ret==M4_SUCCESS){		
		
		alret=AllocVarSpaceIfNeeded();
        if (alret==M4_WARNING) continue; 

		if (alret!=M4_SUCCESS){ //error en la propagación->salimos con recoverfromerror
								//Tambien puede ser interrupción
			ret2=alret;
		}
		else{
			ret2=(ClVMachine0::Execute() ); //ejecutamos L0
		}

		
        DUMP_RTS();
		
		switch( ret2 ){
			
		case M4_SUCCESS: 
			{	//Liberar al acabar de ejecutar una formula
				m_oStateStack->Top(m_oState1); //Para las estadisticas
				
                ret = _FreeAtEndOfMethod();
                ret2=ret;
				break;
			}
			
			
		case 1:	 //Hay que ejecutar algo de nivel 1
			{	 //es de nuestro nivel
				
				ret=m_oStateStack->Top(m_oState1);
				DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Error Reading state.")	;
				m_oStateStack->Pop();
				
				//la instrucción actal a un miembro para que sea más rápido
				m_oCurrentIns1 = (m_oState1.m_poFormula)[m_oState1.m_iCurrent]; 
				
				//guardamos el estado antes de ejecutar la instrucción
				//de nivel 1, ya que esa instrucción puede meter un
				//nuevo estado en la pila (execute item) y debe quedar por encima de todo.
				//El estado que guardamos ya va con la instrucción incrementada
				m_oState1.m_iCurrent++; //siguiente instrucción
				m_oStateStack->Push(m_oState1);
				
				ui16NumFunc = m_oCurrentIns1->GetExeFunc();

	#ifdef _DEBUG
				// Mark for check execution
				m4int8_t	iNumArgRef;
				m4int32_t	iPosBeforeExec;
				m4bool_t	bCheckExec;
				
				//obtenemos el número de función	
				bCheckExec = MarkBeginExec(ui16NumFunc, iNumArgRef, iPosBeforeExec);			
	#endif

				//Executamos
				ret = ExecuteFunction( ui16NumFunc, m_oImplementors, m_oState1 ) ;


	#ifdef _DEBUG
				// Check execution
				if ( CheckEndExec(ui16NumFunc, iNumArgRef, iPosBeforeExec, ret, bCheckExec) != M4_SUCCESS ) {
						DUMP_CHLOG_WARNINGF(M4_CH_VM_TRACE_STACK_VM, 1<<ui16NumFunc );
				}
	#endif
				
				ret2=ret;
				if (ret2==M4CL_EXECUTION_INTERRUPTION) return ret2; //si hay interrupción en remoto salimos.
                //si hay un error intentamos recuperarlo
                if (ret!=M4_SUCCESS){
					
                    //intentamos recuperarnos- Si retorna error es que no podemos recuperarnos 
                    ret=RecoverFromError(m_oState1.m_poAccess);
#ifdef _DEBUG
                    if (ret==M4_SUCCESS){
                        //SETCODE(M4_CH_VM_ERROR_RECOVERED, DEBUGINFOLOG, "Runtime error recovered at level 1")	;
						DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_RECOVERED,"1");
                    }
#endif
					if (ret==M4_SUCCESS) ret2=M4_SUCCESS; //si nos hemos recuperado, para poder seguir
				}
                
				
				break;
			}
			
			
		case M4_ERROR:
		case M4CL_USER_ERROR_ABORT_TRANSACTION:
		case M4CL_USER_ABORT_ALL_REQUEST:	 //han abortado y debemos salir

		
			//-------------------------------
			//Hubo un error.
			//-------------------------------
			{


				
                ret=m_oStateStack->Top(oState);
				if (ret!=M4_SUCCESS) return ret2; //puede que hayan abortado todo
				
                //intentamos recuperarnos- Si retorna error es que no podemos recuperarnos 
				ret=RecoverFromError(oState.m_poAccess);

#ifdef _DEBUG
                if (ret==M4_SUCCESS){
					DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_RECOVERED,"1");
                }
#endif
				if (ret == M4_SUCCESS) ret2=ret; //si conseguimos recuperarnos->ponemos ret2 bien
				
				break;
				
			}

		default:							 //es de un nivel superior o bien es M4CL_EXECUTION_INTERRUPTION
			{
				
				return ret2;
			}
		} //del switch
		
		
	} //del while
	
	
	return ret2;
} //de la función


//-------------------------------------------------------------------------
//NIVELES SUPERIORES Y SERIALIZE
//-------------------------------------------------------------------------
m4return_t ClVMachine1::GetExecStatus (m4VMState_t *ao_oState, m4uint32_t ai_iNumStates, m4VariantType *ao_Vars, m4uint32_t &ao_iNumArgs , m4bool_t &ao_bHasState, m4bool_t ai_bSerializeLocals, m4int32_t &ao_iRef,m4int32_t &ao_iTotalStates, m4int32_t &ao_iNumLocals){
	
    m4return_t ret=M4_SUCCESS;
    m4uint32_t i;
	m4VariantType vNumArgs;
	m4int32_t iFrom=0;

	ao_iRef=m_oRunTimeStack->GetReference();
	ao_iTotalStates=m_oStateStack->GetPos()+1;

    if (ao_Vars==0){ //retornamos el número de variants que pasaremos
		
		if (ai_iNumStates && ai_iNumStates<= m4uint32_t(m_oStateStack->GetPos()+1) ){ //si hay suficientes estados, y quieren llevar al menos 1

			//leemos los estados y los metemos en el array
			for (i=0; i<ai_iNumStates;++i){
				ret=m_oStateStack->Top(ao_oState[i],i);
				if (ret!=M4_SUCCESS) return ret;
			}

			/* Bug 0105256
			Si no se saca el estado es uno nuevo y hay que actualizar su referencia
			*/
			if( ai_bSerializeLocals == M4_FALSE )
			{
				ao_oState[ 0 ].m_iOldStackReference = m_oRunTimeStack->GetReference() ;
			}

			//decimos cuantos variants van a tener que llevarse
			ret=CalculateVarsToSerialize(ai_iNumStates, ao_iNumArgs, ao_iNumLocals, iFrom ,ai_bSerializeLocals);
			if (ret!=M4_SUCCESS) return ret;

			ao_bHasState=M4_TRUE;
		}
		else{
			ao_bHasState=M4_FALSE;
			//Todos los de la pila
			ao_iNumArgs = (m4uint16_t) (m_oRunTimeStack->GetPos()>=0 ? m_oRunTimeStack->GetPos()+1:0) ;

		}
    }
	else{

		//si hay que sacar los variants 
		
		if (ai_iNumStates && ai_iNumStates<= m4uint32_t(m_oStateStack->GetPos()+1) ){ //si hay suficientes estados, y quieren llevar al menos 1

			//leemos los estados y los metemos en el array
			for (i=0; i<ai_iNumStates;++i){
				ret=m_oStateStack->Top(ao_oState[i],i);
				if (ret!=M4_SUCCESS) return ret;
			}

			//decimos cuantos variants van a tener que llevarse
			ret=CalculateVarsToSerialize(ai_iNumStates, ao_iNumArgs,ao_iNumLocals,iFrom, ai_bSerializeLocals);
			if (ret!=M4_SUCCESS) return ret;

			ao_bHasState=M4_TRUE;

			
			
			//sacamos los variants del sitio adecuas 
			m_oRunTimeStack->SetReference(0);
			for (i=0 ;i < ao_iNumArgs ; ++i){
				if (m_oRunTimeStack->GetElement(ao_Vars[i], iFrom - i) == M4_ERROR){
					m_oRunTimeStack->SetReference(ao_iRef);
					return M4_ERROR;
				}
			}
			m_oRunTimeStack->SetReference(ao_iRef);

			if (ai_bSerializeLocals){ //si se serializan  las var locales->pop
				for (i=0; i<ai_iNumStates;++i){
					m_oStateStack->Pop();
				}
				for (i=0 ;i < ao_iNumArgs ; ++i){
					m_oRunTimeStack->Pop();
				}
				//actualizamos la referencia de la pila
				m_oRunTimeStack->SetReference(ao_oState[ai_iNumStates-1].m_iOldStackReference);

			}
		}
		else{
			//Todos los de la pila
			ao_iNumArgs = (m4uint16_t) (m_oRunTimeStack->GetPos()>=0 ? m_oRunTimeStack->GetPos()+1:0) ;
			ao_bHasState=M4_FALSE;
			
			for (i=0 ;i < ao_iNumArgs ; ++i){
				if (RuntimePop (ao_Vars[i]) == M4_ERROR){
					return M4_ERROR;
				}
			}
		}
	}

	return M4_SUCCESS;
}


m4return_t ClVMachine1::RestoreExecution  (m4VMState_t *ai_oState, m4uint32_t ai_iNumStates, m4VariantType *ai_Vars, m4uint32_t  ai_iNumArgs, m4bool_t ai_bSerializeLocals, m4int32_t ai_iRef,m4int32_t ai_iTotalStates, m4int32_t ai_iNumLocals){
    
    m4uint32_t i;
    m4return_t ret;
	
    //metemos los argumentos
	for (i=0; i<ai_iNumArgs; ++i){
		
     	if (ai_Vars[ai_iNumArgs-i-1].Type==M4CL_CPP_TYPE_STRING_VAR){
			ret=RuntimePushString (ai_Vars[ai_iNumArgs-i-1].Data.PointerChar, ai_Vars[ai_iNumArgs-i-1].IsUser);
		}
		else{
			ret=RuntimePush (ai_Vars[ai_iNumArgs-i-1]);
		}
	   DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Error performing push to stack in RestoreExecution.")	;
    }

	//si necesitamso transaccion la ponemos
	if (!ai_bSerializeLocals && ai_iNumStates){
		m4uint8_t iTransMode=ai_oState[0].m_poAccess->GetpCMCR()->GetItemTransactionMode(ai_oState[0].m_hItem);
		if (iTransMode == M4CL_ITEM_TRAN_MODE_NEED_TRANSACTION ||
			iTransMode == M4CL_ITEM_TRAN_MODE_NEW_TRANSACTION  ) {
			
			BeginVMTrans();
		}
	}

	/* Bug 0087002
	Sólo hay que tener en cuenta las variables locales si de verdad se han serializado.
	Se podría hacer mejor que se serializase que el número de locales es 0.
	Por si acaso lo corregimos sólo aquí.
	*/
	/* Bug 0105256
	Ya no hace falta ajustar los offsets
	Se dejan como están y a la vuelta se quedarán bien

  	m4int32_t iOffsetOldStackRef=0;

	if( ai_bSerializeLocals == M4_TRUE )
	{
		iOffsetOldStackRef=m_oRunTimeStack->GetPos()+1-ai_iNumLocals - ai_iRef; //old reference
	}
	else
	{
		iOffsetOldStackRef=m_oRunTimeStack->GetPos()+1 - ai_iRef; //old reference
	}
	*/

	m4int32_t	iOffsetParent=m_oStateStack->GetPos()+1+ai_iNumStates - ai_iTotalStates;

	for (i=0;i<ai_iNumStates;++i)
	{
		/* Bug 0105256
		Ya no hace falta ajustar los offsets
		Se dejan como están y a la vuelta se quedarán bien

		ai_oState[ai_iNumStates-i-1].m_iOldStackReference+=iOffsetOldStackRef;
		*/

		/* Bugs 0079839, 0080925
		No se puede poner a 0 la referencia, porque sino a la vuelta
		al calcular el offset daría un valor falso
		Lo suyo es que se quede negativo y al volver ya se sumará y volverá a
		quedar con el valor bueno.

		if (ai_oState[ai_iNumStates-i-1].m_iOldStackReference<0){
			ai_oState[ai_iNumStates-i-1].m_iOldStackReference=0;
		}
		*/

		ai_oState[ai_iNumStates-i-1].m_iParentStatePos+=iOffsetParent;
		
		if (!ai_bSerializeLocals){
			//quitamso el flag de purgado, porque sino se hace en cliente y en remoto
			M4CL_RESET_STATE_PURGE_FLAG(ai_oState[ai_iNumStates-i-1].m_Flags);
		}

		// Ajuste manual para que el padre del primer elemento sea justo el anterior
		if( i == 0 )
		{
			ai_oState[ai_iNumStates-i-1].m_iParentStatePos = m_oStateStack->GetPos() ;
		}

		ret=m_oStateStack->Push(ai_oState[ai_iNumStates-i-1]);
		if (ret!=M4_SUCCESS) return ret;
	}


	/* Bug 0105256
	Se ajusta la referencia, pero de forma más sencilla
	Es el tope de la pila menos las variables locales si las hay
    m_oRunTimeStack->SetReference(ai_iRef+ iOffsetOldStackRef); //ponemos de referencia el tope. Esto solo se hace si hay estado (por ahora es el caso)
	*/
	m4int32_t	iRef = m_oRunTimeStack->GetPos() + 1 ;

	if( ai_bSerializeLocals == M4_TRUE )
	{
		iRef -= ai_iNumLocals ;
	}

    m_oRunTimeStack->SetReference( iRef ) ;

	return ret;
}

m4return_t	ClVMachine1::CalculateVarsToSerialize(m4uint32_t ai_iNumStates, m4uint32_t &ao_iNumVars, m4int32_t &ao_iNumLocals, m4int32_t &ao_iFrom, m4bool_t ai_bSerializeLocals){

	m4int32_t		iRef;
	m4VMState_t		oState;
	m4VMState_t		oPreviousState;
	m4VariantType	vNumArgs;
	m4int32_t		iNumArgs;
	m4return_t		ret;

	iRef=m_oRunTimeStack->GetReference();
	ao_iFrom=(ai_bSerializeLocals ? m_oRunTimeStack->GetPos() :iRef-1); //si no se serializan las locales, de la referencia para abajo, sino del topo para abajo
	
	//si no es un solo estado, para ver donde está la referencia de la pila de variants
	//tenemos que mirar en el estado justo encima de él.
	if (ai_iNumStates>1){
		ret=m_oStateStack->Top(oPreviousState,ai_iNumStates-2);
		if (ret!=M4_SUCCESS) return M4_ERROR;

		//Bug:62709 Begin  -------------------------------
		//Nos saltamos todos los estados MoveTo que se han introducido para la propagación
		// por conectores. Nos movemos hacia arriba en la pila de estados, hasta que encontramos
		// el estado real.
		int i = 3;
		while ( (oPreviousState.m_hItem == -1) && (ai_iNumStates-i >= 0) ) {
			ret=m_oStateStack->Top(oPreviousState,ai_iNumStates-i);
			if (ret!=M4_SUCCESS)
				return M4_ERROR;
			i++;
		}
		if (ai_iNumStates-i < 0)
			return M4_ERROR;
		//Bug:62709 End    -------------------------------

		iRef=oPreviousState.m_iOldStackReference ;
	}
	
	//localizamos el estado en cuestion 
	ret=m_oStateStack->Top(oState,ai_iNumStates-1);		
	if (ret!=M4_SUCCESS) return M4_ERROR;


	/* Bug 0085967
	Apaño todavía mucho más cutre que el anterior, pero a ver como se arregla
	esto bien.
	Si resulta que el anteúltimo estado no es de posicionamiento de propagación
	y el último si que lo es para restaurar la posición son estados que se han
	invertido, y la OldStackReference que hay que tomar es la del de
	restauración de la propagación.
	El cambio sólo se hace cuando la OldStackReference del de restauración es
	mayor que la del normal, para evitar que se nos cuele un estado de propagación
	de no restauración (de posicionamiento) de un estado que esté en la
	posición -1, ya que esos no se habrían invertido.
	*/
	if( oState.m_hItem == -1 )
	{
		if( oState.m_iOldStackReference > iRef )
		{
			iRef = oState.m_iOldStackReference ;

			/* Bug 0127420
			Cuando se ha metido el estado de propagación, también hay que tener en cuenta
			que los argumentos hay que sacarlos del estado anterior, el de verdad
			Se da el cambiazo al estado.
			Ojo, si esto da problemas, basta con comentar esta linea y dejarlo como antes
			*/
			oState = oPreviousState ;
		}
	}
	
	//ahora vemos cuantos argumentos tiene y ya podemos calcular el núemro de variant
	//total que tenemos que transportar.

    ClCompiledMCR   *poCMCR=oState.m_poAccess->GetpCMCR() ;
	
	if (oState.m_hItem!=-1){
		m4uint32_t iVariable= poCMCR->GetItemVariableArguments (oState.m_hItem);
		
		if (iVariable != 0){
			//quitamos ref
			m4int32_t iOldRef=m_oRunTimeStack->GetReference();
			m_oRunTimeStack->SetReference(0);
			
			//leemos
			m_oRunTimeStack->GetElement(vNumArgs, iRef-1);
			
			//restauramos ref
			m_oRunTimeStack->SetReference(iOldRef);
			
			iNumArgs=m4uint32_t(vNumArgs.Data.DoubleData)+1; //el +a es el numero de argumentos qur tambien nos lo tenemos que llevar
		}
		else{
			iNumArgs=poCMCR->GetItemNumberOfArguments (oState.m_hItem);
		}
	}
	else{
		iNumArgs=0;//los estados de propagación tienen 0 args
	}

	ao_iNumVars=(m_oRunTimeStack->GetPos()+1) - iRef + iNumArgs;
	
	ao_iNumLocals=(m_oRunTimeStack->GetPos()+1) - m_oRunTimeStack->GetReference();

	if (!ai_bSerializeLocals){
		ao_iNumVars-= ao_iNumLocals;
	}
	return M4_SUCCESS;
}

//**************************
//*Funciones básicas
//**************************

m4return_t ClVMachine1::ExecuteItem(void){
    
    //ejecuta el metodo que indican los campos de la instrucción
    // y pone en la pila el resultado
    
    m4int32_t			i;
    m4return_t			ret=M4_SUCCESS;
    m4VariantType		vTempVariant, vSlice(0.0);
    m4VMState_t			oState;
    m4uint16_t			iNumArg , iNumRealArg, iVarArgs;
    m4uint16_t			iInstruFlags;
    m4uint32_t			hNodo;
    m4bool_t            bSpecSlice;
	
    
    
    ClCompiledMCR   *poCMCR=m_oState1.m_poAccess->GetpCMCR() ;
    
    iNumArg	 =  0;
    iNumRealArg = 0;
    
    oState.m_hItem=m_oCurrentIns1->GetNumItem() ;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Execute Item:" <<LogCat<< poCMCR->GetItemId(oState.m_hItem) );
#endif
    
    hNodo = m_oCurrentIns1->GetNumNodo() ;
    
#ifdef _DEBUG
    if (hNodo == 0 ) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_NODE_HANDLE, "ExecuteItem:"<<LogCat<< poCMCR->iItemI(oState.m_hItem) <<""<<"" << DumpCallStack() );
        return M4_ERROR;
    }
#endif
    
    //metemos en el estado el handle a la formula
    oState.m_iRule=m_oCurrentIns1->GetNumRegla();
    //metemos el access, el bloque actual y el registro actual
    oState.m_poAccess = m_oState1.m_poAccess;
    oState.m_poRecordSet=&(oState.m_poAccess->Node[ClHandle(hNodo)].RecordSet);
    oState.m_Flags=0;
    
    
    iInstruFlags=m_oCurrentIns1->GetFlags();

	/*
	Código heurístico para detectar cambios en argumentos por referencia
	m4uint16_t	iFunction = 0;
    m4uint16_t	iRefered = 0;
    m4uint16_t	ii = 0;
	m4uint32_t	iHandle = 0;
	m4pcchar_t	pccChannel = NULL;
	m4pcchar_t	pccNode = NULL;
	m4pcchar_t	pccItem = NULL;

    iRefered = poCMCR->GetItemNumberOfReferedArguments( oState.m_hItem );

	if( iRefered > 0 )
	{
		for( ii = 0; ii < iRefered ; ii++ )
		{
			iFunction = (m_oState1.m_poFormula)[m_oState1.m_iCurrent + ii]->GetExeFunc();

			if( iFunction != M4CL_VM_FT_ASSIGNVAR && iFunction != M4CL_VM_FT_ASSIGNITEM && iFunction != M4CL_VM_FT_ASSIGNFIXEDARGUMENTINVARARGSITEM)
			{
				iHandle = oState.m_hItem;
				pccChannel = poCMCR->GetChannelId();
				pccNode = poCMCR->GetNodeId( hNodo );
				pccItem = poCMCR->GetItemId( iHandle );
			}
		}
	}
	*/

    //leemos el número de argumentos. Solo necesitamos el número de argumentos para saber donde está 
    //el tramo y/o row si es que lo hay
    iNumRealArg=poCMCR->GetItemNumberOfArguments ( oState.m_hItem);
    iVarArgs = poCMCR->GetItemVariableArguments ( oState.m_hItem);

    if ( iVarArgs != 0 )
	{
		/*
		Código heurístico para detectar cambios en argumentos variables
		iFunction = (m_oState1.m_poFormula)[m_oState1.m_iCurrent - 2]->GetExeFunc();

		if( iFunction != M4CL_VM_FT_PUSHNUM)
		{
			iHandle = oState.m_hItem;
			pccChannel = poCMCR->GetChannelId();
			pccNode = poCMCR->GetNodeId( hNodo );
			pccItem = poCMCR->GetItemId( iHandle );
		}
		*/

        RuntimePop(vTempVariant);
        iNumArg	 = int(	vTempVariant.Data.DoubleData );
        //consideramos que tenemos un argumento más porque al tener un número variable de argumentos hay un elemento 
        //en la pila que indica ese número y tambien hay que sacarlo/dejarlo  en la pila, como un argumento mas.
        iNumArg++;
    }
    else{
        iNumArg = iNumRealArg;
    }
	
    m4bool_t bRestoreArgs=M4_FALSE;
    //sacar los arg. si se necesitan
    
	if (M4CL_HAS_TRAME_FLAG(iInstruFlags)  || M4CL_HAS_ROW_FLAG( iInstruFlags )	){ //hay que salvar los argumentos
        //No hacemos comprobaciones sobre el número de argumentos  !!
		
        //sacamos los arg. y los metemos en la pila temporal
        for (i=0; i < iNumArg ; i++){
            ret=RuntimePop(vTempVariant);
            //metemos en la pila de resultados las varaible por ref. y el resultado
            m_oResultStack->Push(vTempVariant);
        }
        bRestoreArgs=M4_TRUE;
    }
    
    
    //Si tiene tramo
    if (M4CL_HAS_TRAME_FLAG( iInstruFlags) ){ //tiene el tramo
        ret=RuntimePop(vSlice); //leemos el tramo
        bSpecSlice=M4_TRUE;
    }
    else{ //viene sin tramo
        bSpecSlice=M4_FALSE;    
    }
    
    
    //Si tiene registro
    if ( M4CL_HAS_ROW_FLAG (iInstruFlags) ) {
        
        ret=RuntimePop(vTempVariant); //leemos el reg.
        
        if (vTempVariant.Type != M4CL_CPP_TYPE_NUMBER){
            ret=m_oVariantMediator->ToDouble (vTempVariant);
            M4CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_WRONG_RECORD_NUMBER_FORMAT, ERRORLOG, M4ObjNodeItemI(poCMCR,oState.m_hItem)<<M4ObjNodeItemI (poCMCR,m_oState1.m_hItem)<<DumpTreePath(oState.m_poAccess, hNodo) << DumpCallStack() );
        }
        oState.m_oRegisterIndex=int (vTempVariant.Data.DoubleData);
    }
    else{
        //el registro sobre el que actuar será current o NULL
        if (oState.m_poRecordSet->IsValid ()){
            oState.m_oRegisterIndex = oState.m_poRecordSet->Current.Index;
        }
        else{
            oState.m_oRegisterIndex = M4DM_EOF_INDEX;
        }
    }
    
    
    //si habiamos sacado los registros los metemos de nuevo
    //dejamos los argumentos en la pila
    if (bRestoreArgs ==M4_TRUE){
        for (i=0; i < iNumArg ; i++){
            ret=m_oResultStack->Top(vTempVariant);
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Error adding arguments to RTS, reading from tempStack in ExecuteItem.")	;
            m_oResultStack->Pop();					
            //metemos en la pila de ejecución lo que habiamos sacado
            ret=RuntimePush(vTempVariant);
        }    
    }
    
    
    oState.m_dBeginDate=m_oState1.m_dBeginDate;
    oState.m_dEndDate=m_oState1.m_dEndDate;
    oState.m_dDate=m_oState1.m_dDate;
    
	
    if ( iVarArgs != 0 ) --iNumArg;
	
	//Nuevo. Estadisticas.
	oState.m_iParentStatePos=m_oStateStack->GetPos();
	
    ret= CreateExecute (oState, iNumArg, vSlice, bSpecSlice)	;
	return ret;
}

//-------------------------------------------------------------
//Asignar item
//-------------------------------------------------------------

m4return_t ClVMachine1::AssignItem(void){
	
	//asigna al item que indican los campos de la instrucción
	//el tope de la pila
	//y elimina el tope de la pila
	
   	m4uint32_t	iRow;
	m4double_t	dTrame;
	m4uint32_t	hNodo, hItem;	
	m4uint8_t   iSliceFlag;
	ClSliceIndex	SliceIndex;
	m4VariantType vTemp, vValue;
	m4return_t	ret;
    ClItem     *poItem;
	

	const m4uint8_t NoSliceWithSlices=0;
    const m4uint8_t NoSliceWithoutSlices=1;
    const m4uint8_t SliceByDate=2;
    const m4uint8_t SliceByOrder=3;
	
    
	ClCompiledMCR   *poCMCR=m_oState1.m_poAccess-> GetpCMCR() ;
	
	ret=RuntimePop(vValue);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "assignItem")	;
	
	
    //obtenemos el valor al que asignar y asignamos
    if (GetRunSliceMode(m_oState1.m_poAccess)){
        iSliceFlag=NoSliceWithSlices;
    }
    else{
        iSliceFlag=NoSliceWithoutSlices;
    }
	
	hItem= m_oCurrentIns1->GetNumItem() ;
	hNodo= m_oCurrentIns1->GetNumNodo() ;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Assign Item:"<<LogCat<< poCMCR->GetItemId(hItem) );
#endif

	//miramos si viene con tramo
	if ( M4CL_HAS_TRAME_FLAG (m_oCurrentIns1->GetFlags() ) ){
		
		ret=RuntimePop(vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop from stack.")	;
		
        
		if (vTemp.Type==M4CL_CPP_TYPE_NUMBER){ //por número de tramo
			SliceIndex=m4uint32_t(vTemp.Data.DoubleData);
            iSliceFlag=SliceByOrder;
		}
		else if (vTemp.Type==M4CL_CPP_TYPE_DATE){ //por fecha
			dTrame=vTemp.Data.DoubleData;
			iSliceFlag=SliceByDate;
		}
		else{
            DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE_FORMAT_WRITE, M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI (poCMCR,m_oState1.m_hItem) << DumpCallStack() );
			return M4_ERROR;
		}
        
        //como ha modificado "a mano" los tramos, no nos asignaremos
        //M4CL_SET_STATE_PURGE_FLAG (m_oState1.m_Flags);
        M4CL_RESET_STATE_CONCEPT_FLAG (m_oState1.m_Flags);
		
	}
	
    //miramos si viene con row
    _ClRegister_Base *pr;
	
    if ( M4CL_HAS_ROW_FLAG (m_oCurrentIns1->GetFlags() ) ){
		ret=RuntimePop (vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP , ERRORLOG, "Error performing pop from RTS to get record number.")	;
		
        if (vTemp.Type != M4CL_CPP_TYPE_NUMBER){    // si no es un numero lo intentamos converit
            ret=m_oVariantMediator->ToDouble (vTemp);
            M4CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_WRONG_RECORD_NUMBER_FORMAT, ERRORLOG, M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI (poCMCR,m_oState1.m_hItem)<<DumpTreePath(m_oState1.m_poAccess, hNodo) << DumpCallStack() );
        }
        
		iRow=int (vTemp.Data.DoubleData);
		
        if (!m_oState1.m_poAccess->Node[ClHandle(hNodo)].RecordSet.IsValid() && poCMCR->GetItemScope(hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_ASSIGNING)<<M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(poCMCR,m_oState1.m_hItem)<<DumpTreePath(m_oState1.m_poAccess, hNodo)<< DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//el nodo ya esta posicionado
        pr=&(m_oState1.m_poAccess->Node[ClHandle(hNodo)].RecordSet.Register[iRow]);
		
	}
    else{
        if (!m_oState1.m_poAccess->Node[ClHandle(hNodo)].RecordSet.IsValid() && poCMCR->GetItemScope(hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_ASSIGNING)<<M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(poCMCR,m_oState1.m_hItem)<<DumpTreePath(m_oState1.m_poAccess, hNodo)<< DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//el nodo ya esta posicionado
        pr=&(m_oState1.m_poAccess->Node[ClHandle(hNodo)].RecordSet.Current);
		
    }
	
	
    //comprobamso que podamos asignar  
    if (pr->Index == M4DM_EOF_INDEX && poCMCR->GetItemScope(hItem) == M4CL_ITEM_SCOPE_REGISTER ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_NUMBER_WRITE, M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(poCMCR,m_oState1.m_hItem)<<DumpTreePath(m_oState1.m_poAccess, hNodo) << DumpCallStack() );
        return M4_ERROR;
    }
	
	//con todos los datos obtenemos el item
    
	ClItemIndex iItem = poCMCR->GetItemIndex (hItem);

    switch(iSliceFlag){
    case NoSliceWithoutSlices:
        {
			poItem=&(pr->Item[iItem]);
			
			// Chequeo de uso de un concepto tramado en modo no tramos
			m4uint32_t iSlices = poItem->Value.Count() ;

			if( iSlices > 0 )
			{
				m4char_t	acDate[ M4_SECOND_END + 1 ] ;
				ClDateToString( m_oState1.m_dDate, acDate, M4_SECOND_END + 1 ) ;
				DUMP_CHLOG_WARNINGF( M4_CH_VM_BASE_SLICED_ITEM_USE, M4ObjNodeItemI( poItem->GetpAccess()->GetpCMCR(), hItem ) << iSlices << poItem->Value.Index << m_oState1.m_iSourceLine << m_oState1.m_poAccess->GetpCMCR()->GetItemFormIdByPosition( m_oState1.m_hItem, m_oState1.m_iRule ) << M4ObjNodeItemI( m_oState1.m_poAccess->GetpCMCR(), m_oState1.m_hItem ) << acDate << DumpCallStack() ) ;
			}

			ret=__SetValue(*poItem, vValue, M4CL_CM_DEFAULT_PRIOR, 0) ;
            break;
        }
    case NoSliceWithSlices:
        {
            
			poItem=&(pr->Item[iItem]);
			
            ret=CheckAndCreateItemSlice (m_oState1.m_dBeginDate, m_oState1.m_dEndDate ,poItem);
            if (ret==M4_SUCCESS){
				/* Bug 0076620
				Se asignan los subtramos si los hubiera
				*/
                ret=SetValue(*poItem, vValue, M4CL_CM_DEFAULT_PRIOR, 0, m_oState1.m_dBeginDate, m_oState1.m_dEndDate, M4_TRUE);    //no movemos el item porque ya esta puesto    
            }

            M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_SET_CONCEPT_SLICE_VALUE, ERRORLOG, M4ObjNodeItemI(poCMCR,hItem)<<DateLog(m_oState1.m_dBeginDate)<<DateLog(m_oState1.m_dEndDate)<<M4ObjNodeItemI(poCMCR,m_oState1.m_hItem)  << DumpCallStack() );
            break;
        }
        
    case SliceByDate:
        {
			pr->Item[iItem].Value[dTrame];
			ret=__SetValue(pr->Item,vValue, M4CL_CM_DEFAULT_PRIOR, 0); //no movemos el item porque ya esta puesto
            break;
        }
    case SliceByOrder:
        {
			pr->Item[iItem].Value[SliceIndex];
            ret=__SetValue(pr->Item, (vValue), M4CL_CM_DEFAULT_PRIOR, 0); //no movemos el item porque ya esta puesto
            break;
        }
    }
    
    if (vValue.Type==M4CL_CPP_TYPE_STRING_POOL){
        //la borramos
        m_oDynamicPool->DeleteString(vValue.Data.CharData);
    }
	
    if (ret==M4_ERROR){
        g_oChLog<<BeginError(M4_CH_VM_WRONG_ITEM_ASSIGN)<<M4ObjNodeItemI(pr->GetpAccess()->GetpCMCR(),pr->Item.ItemDef.GetHandle())
														<<DumpCallStack()
														<<EndLog;
        return M4_ERROR;
    }
    else if (ret==M4_WARNING){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED_EXE_ITEM, M4ObjNodeItemI(pr->GetpAccess()->GetpCMCR(),pr->Item.ItemDef.GetHandle())<<M4ObjNodeItemI(poCMCR,m_oState1.m_hItem) << DumpCallStack() );
        ret=M4_SUCCESS;
    }
	
	return M4_SUCCESS;
}



//-------------------------------------------------------------
//leer item
//-------------------------------------------------------------

m4return_t ClVMachine1::PushItem(void){
	
	//mete en la pila el item
	//que indican los campos de la instrucción
	
	m4uint32_t	iRow;
	m4double_t	dTrame;
	m4uint32_t	hNodo, hItem;	
	m4uint8_t   iSliceFlag;
	m4date_t	dStartDate = -1;
	m4date_t	dEndDate = -1;
	ClSliceIndex	SliceIndex;
	m4VariantType vTemp;
	m4return_t	ret;
	
	const m4uint8_t NoSliceWithSlices=0;
    const m4uint8_t NoSliceWithoutSlices=1;
    const m4uint8_t SliceByDate=2;
    const m4uint8_t SliceByOrder=3;

    m4char_t	acStartDate[ M4_SECOND_END + 1 ] ;
    m4char_t	acEndDate  [ M4_SECOND_END + 1 ] ;

    
    
    ClCompiledMCR   *poCMCR=m_oState1.m_poAccess-> GetpCMCR() ;
	
    if (GetRunSliceMode(m_oState1.m_poAccess)){
        iSliceFlag=NoSliceWithSlices;
    }
    else{
        iSliceFlag=NoSliceWithoutSlices;
    }
	
	hItem= m_oCurrentIns1->GetNumItem() ;
    hNodo= m_oCurrentIns1->GetNumNodo() ;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Push Item:"<<LogCat<< poCMCR->GetItemId(hItem) );
#endif
	
	//miramos si viene con tramo
	if ( M4CL_HAS_TRAME_FLAG (m_oCurrentIns1->GetFlags() ) )
	{
		ret=RuntimePop(vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop from stack.")	;
        
		if (vTemp.Type==M4CL_CPP_TYPE_NUMBER){ //por número de tramo
			SliceIndex=m4uint32_t(vTemp.Data.DoubleData);
            iSliceFlag=SliceByOrder;
		}
		else if (vTemp.Type==M4CL_CPP_TYPE_DATE){ //por fecha
			dTrame=vTemp.Data.DoubleData;
			iSliceFlag=SliceByDate;
		}
		else{
            DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE_FORMAT_READ, M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI (poCMCR,m_oState1.m_hItem) << DumpCallStack() );
			return M4_ERROR;
		}
	}
	
    //miramos si viene con row
    _ClRegister_Base *pr;
	
    if ( M4CL_HAS_ROW_FLAG (m_oCurrentIns1->GetFlags() ) ){
		ret=RuntimePop (vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP , ERRORLOG, "Error performing pop from RTS to get record number.")	;
		
        if (vTemp.Type != M4CL_CPP_TYPE_NUMBER){    // si no es un numero lo intentamos converit
            ret=m_oVariantMediator->ToDouble (vTemp);
            M4CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_WRONG_RECORD_NUMBER_FORMAT, ERRORLOG, M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI (poCMCR,m_oState1.m_hItem)<<DumpTreePath(m_oState1.m_poAccess, hNodo) << DumpCallStack() );
        }
		
		iRow=int (vTemp.Data.DoubleData);
		
        if (!m_oState1.m_poAccess->Node[ClHandle(hNodo)].RecordSet.IsValid() && poCMCR->GetItemScope(hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_READING)  <<M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(poCMCR,m_oState1.m_hItem)<<DumpTreePath(m_oState1.m_poAccess, hNodo)<< DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//el nodo ya esta posicionado
        pr=&(m_oState1.m_poAccess->Node->RecordSet.Register[iRow]);
		
	}
    else{
        if (!m_oState1.m_poAccess->Node[ClHandle(hNodo)].RecordSet.IsValid() && poCMCR->GetItemScope(hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_READING)  <<M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(poCMCR,m_oState1.m_hItem)<<DumpTreePath(m_oState1.m_poAccess, hNodo)<< DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//el nodo ya esta posicionado
        pr=&(m_oState1.m_poAccess->Node->RecordSet.Current);
    }
	
	
    //comprobamso que podamos leer 
    if (pr->Index == M4DM_EOF_INDEX && poCMCR->GetItemScope(hItem) == M4CL_ITEM_SCOPE_REGISTER ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_NUMBER_READ,M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(poCMCR,m_oState1.m_hItem)<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poAccess->Node->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        return M4_ERROR;
    }
	
	//con todos los datos obtenemos el item
	ClItemIndex iItem = poCMCR->GetItemIndex (hItem);
	ClItem	*poItem=&(pr->Item[iItem]);

    switch(iSliceFlag){
    case NoSliceWithoutSlices:
        {
			// Chequeo de uso de un concepto tramado en modo no tramos
			m4uint32_t iSlices = poItem->Value.Count() ;

			if( iSlices > 0 )
			{
				m4char_t	acDate[ M4_SECOND_END + 1 ] ;
				ClDateToString( m_oState1.m_dDate, acDate, M4_SECOND_END + 1 ) ;
				DUMP_CHLOG_WARNINGF( M4_CH_VM_BASE_SLICED_ITEM_USE, M4ObjNodeItemI( poItem->GetpAccess()->GetpCMCR(), hItem ) << iSlices << poItem->Value.Index << m_oState1.m_iSourceLine << m_oState1.m_poAccess->GetpCMCR()->GetItemFormIdByPosition( m_oState1.m_hItem, m_oState1.m_iRule ) << M4ObjNodeItemI( m_oState1.m_poAccess->GetpCMCR(), m_oState1.m_hItem ) << acDate << DumpCallStack() ) ;
			}

			ret=poItem->Value.Get(vTemp);
			/* Bug 0084877 Hay que poner más información en el mensaje */
			if (ret==M4_ERROR){
				DUMP_CHLOG_ERRORF(M4_CH_VM_CANT_READ_ITEM, M4ObjNodeItemI(poCMCR,hItem)<<pr->Index<<M4ObjNodeItemI(poCMCR,m_oState1.m_hItem) << DumpCallStack() );
				return M4_ERROR;
			}
            break;
        }

    case NoSliceWithSlices:
        {
			ret=poItem->Value.Get(m_oState1.m_dBeginDate, m_oState1.m_dEndDate, vTemp);
			/* Bug 0084877 Hay que poner más información en el mensaje */
			if (ret==M4_ERROR){
				ClDateToString( m_oState1.m_dBeginDate, acStartDate, M4_SECOND_END + 1 ) ;
				ClDateToString( m_oState1.m_dEndDate,   acEndDate,   M4_SECOND_END + 1 ) ;
				DUMP_CHLOG_ERRORF(M4_CH_VM_CANT_READ_SLICE_BY_INTERVAL, acStartDate<<acEndDate<<M4ObjNodeItemI(poCMCR,hItem)<<pr->Index<<M4ObjNodeItemI(poCMCR,m_oState1.m_hItem) << DumpCallStack() );
				return M4_ERROR;
			}

			dStartDate = m_oState1.m_dBeginDate;
			dEndDate = m_oState1.m_dEndDate;
            break;
        }
        
    case SliceByDate:
        {
			ret=poItem->Value[dTrame].Get(vTemp);
			/* Bug 0084877 Hay que poner más información en el mensaje */
			if (ret==M4_ERROR){
				ClDateToString( dTrame, acStartDate, M4_SECOND_END + 1 ) ;
				DUMP_CHLOG_ERRORF(M4_CH_VM_CANT_READ_SLICE_BY_DATE, acStartDate<<M4ObjNodeItemI(poCMCR,hItem)<<pr->Index<<M4ObjNodeItemI(poCMCR,m_oState1.m_hItem) << DumpCallStack() );
				return M4_ERROR;
			}
            break;
        }

    case SliceByOrder:
        {
			ret=poItem->Value[SliceIndex].Get(vTemp);
			/* Bug 0084877 Hay que poner más información en el mensaje */
			if (ret==M4_ERROR){
				DUMP_CHLOG_ERRORF(M4_CH_VM_CANT_READ_SLICE_BY_INDEX, SliceIndex<<M4ObjNodeItemI(poCMCR,hItem)<<pr->Index<<M4ObjNodeItemI(poCMCR,m_oState1.m_hItem) << DumpCallStack() );
				return M4_ERROR;
			}
            break;
        }
    }

	if ( (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_READS) )
	{
		m4VMState_t oState;
		m_oStateStack->Top(oState);
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddRead( *poItem, vTemp, dStartDate, dEndDate, oState, DumpCallStack( &oState, M4_TRUE ).c_str() );
	}

	if (vTemp.Type==M4CL_CPP_TYPE_STRING_VAR){
		ret=RuntimePushString(vTemp.Data.PointerChar, vTemp.IsUser);
	}
	else{
		ret=RuntimePush(vTemp);
	}

    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Error performing push to stack in PushItem.")	;
	return M4_SUCCESS;
}


//-----------------------------------------------------------
//Atributos
//-----------------------------------------------------------

m4return_t ClVMachine1::ReadAttribute(void){
	
	m_poAttribute->SetContext ( m_oState1.m_poAccess, m_oCurrentIns1->GetFlags(),  m_oCurrentIns1->GetNumNodo(), m_oCurrentIns1->GetNumItem(), GetRunSliceMode(m_oState1.m_poAccess), m_oState1.m_dBeginDate, m_oState1.m_dEndDate);
	
    m4return_t ret=m_poAttribute->Get( m_oCurrentIns1->GetNumAtrib() );
    if (ret==M4_ERROR){
        DUMP_CHLOG_ERRORF(M4_CH_VM_READING_ATT_EXE_ITEM, M4ObjNodeItemI(m_oState1.m_poAccess->GetpCMCR(),m_oCurrentIns1->GetNumItem())<<M4ObjNodeItemI(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_hItem) << DumpCallStack() );
    }
    return ret;
	
	
}


m4return_t ClVMachine1::ExecuteAttribute(void){
	
	m_poAttribute->SetContext (m_oState1.m_poAccess, m_oCurrentIns1->GetFlags(),  m_oCurrentIns1->GetNumNodo(), m_oCurrentIns1->GetNumItem(), GetRunSliceMode(m_oState1.m_poAccess), m_oState1.m_dBeginDate, m_oState1.m_dEndDate);
	
	m4return_t ret=m_poAttribute->Run(m_oCurrentIns1->GetNumAtrib());
  //  if (ret==M4_ERROR){
  //      DUMP_CHLOG_ERRORF(M4_CH_VM_EXECUTING_ATT, M4ObjNodeItemI(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_hItem) << DumpCallStack() );
  //  }
    return ret;
}

//_----------------------------------------------------
//Para argumentos fijos en items ln4 con arg. variables
//_----------------------------------------------------
m4return_t ClVMachine1::AssignFixedArgumentInVarArgsItem		 (void){
	
	m4return_t ret;
	m4VariantType vTempVariant,vTemp2,vNumArgs;
	m4int32_t iNumArg,iNumPassed,iNumFixed;

    ClCompiledMCR   *poCMCR=m_oState1.m_poAccess-> GetpCMCR() ;
	//asigna a un agumento fijo en un item ln4 de argumentos variable
	//m_iNumVar indica la posicion original, el valor del tope de la pila
	// y elimina ese valor de la pila
	
	
	//obtenemos el No. de la Variable a la que vamos a realizar la Asignacion
	iNumArg = m_oCurrentIns1->GetNumVar();
		
	M4_ASSERT (poCMCR->GetItemVariableArguments ( m_oState1.m_hItem) );

	m_oRunTimeStack->GetElement(vNumArgs, -1);
	iNumPassed=(m4int32_t)vNumArgs.Data.DoubleData;
	iNumFixed = poCMCR->GetItemNumberOfArguments (m_oState1.m_hItem);

	iNumArg+=(iNumFixed-iNumPassed);
	//leemos lo que había en la variable a la que vamos a cambiar el contenido
	m_oRunTimeStack -> GetElement ( vTempVariant , iNumArg );
	
	//SI en la variable habia una cadena tendremos que liberarla
	if (vTempVariant.Type==M4CL_CPP_TYPE_STRING_POOL){//hay que liberar
		m_oDynamicPool->DeleteString(vTempVariant.Data.CharData);
	}
	
	//leemos y sacamos el tope de la pila, que es lo que hay que Asignar
	RuntimePop(vTempVariant);
	
	ret=m_oRunTimeStack->SetElement (vTempVariant, iNumArg );
	return ret;
}

m4return_t ClVMachine1::ReadFixedArgumentInVarArgsItem		 (void){
	
	m4return_t ret;
	m4VariantType vTempVariant,vNumArgs;
	m4uint32_t handle;
	m4int32_t iNumArg,iNumPassed,iNumFixed;

    ClCompiledMCR   *poCMCR=m_oState1.m_poAccess-> GetpCMCR() ;
	//leer un agumento fijo en un item ln4 de argumentos variable
	//mete en la pila de ejecución el contenido de la variable
	//que indique el  m_iNumVar
	
	
	//obtenemos el No. del argumentoa la que vamos a realizar la Asignacion
	iNumArg = m_oCurrentIns1->GetNumVar();
		
	M4_ASSERT (poCMCR->GetItemVariableArguments ( m_oState1.m_hItem) );

	m_oRunTimeStack->GetElement(vNumArgs, -1);
	iNumPassed=(m4int32_t)vNumArgs.Data.DoubleData;
	iNumFixed = poCMCR->GetItemNumberOfArguments (m_oState1.m_hItem);

	iNumArg+=(iNumFixed-iNumPassed);

	ret=m_oRunTimeStack->GetElement(vTempVariant, iNumArg);
	
	//si la varaible es de tipo string habra que meter una copia de la cadena
	if (vTempVariant.Type==M4CL_CPP_TYPE_STRING_POOL){
		m_oDynamicPool->AddString( m_oDynamicPool->GetString(vTempVariant.Data.CharData), handle);
		vTempVariant.Data.CharData=handle;
	}
	
	RuntimePush(vTempVariant);
    
    return ret;
}

//------------------------------------------------
//A R G U M E N T O S variables ( y no variables )
//------------------------------------------------
m4return_t ClVMachine1::GetArgument		 (void){
	
	m4VariantType vArgToRead, vNumArgs, vTemp;
	m4return_t ret;
	m4uint32_t iNumArgs,iArgToRead;
	
	
    ClCompiledMCR   *poCMCR=m_oState1.m_poAccess-> GetpCMCR() ;
	
	//leemos el argumento que quieren leer
	ret=RuntimePopNumber (vArgToRead);
	if (ret!= M4_SUCCESS){
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Position"<<"GetArgument" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vArgToRead.Type)<< DumpCallStack() );
		DeleteVarString (vArgToRead);
		return M4_SUCCESS;
	}
	iArgToRead= (m4uint32_t)vArgToRead.Data.DoubleData;
	
	//comprobamos que exista
	iNumArgs=GetRunningItemArgumentNumber(m_oState1, m_oRunTimeStack->GetReference());
	
	/* Bug 0079624
	Se ponen unos mensajes más claros
	*/
	if( iNumArgs == 0 )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_NO_PARAM_METHOD, iArgToRead << M4ObjNodeItemI( m_oState1.m_poAccess->GetpCMCR(), m_oState1.m_hItem ) << DumpCallStack() ) ;
		RuntimePushNull() ;
		return( M4_SUCCESS ) ;
	}

	if( iArgToRead > iNumArgs || iArgToRead <= 0 )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_BAD_METHOD_PARAM, iArgToRead << iNumArgs << M4ObjNodeItemI( m_oState1.m_poAccess->GetpCMCR(), m_oState1.m_hItem ) << DumpCallStack() ) ;
		RuntimePushNull() ;
		return( M4_SUCCESS ) ;
	}
	
	ret= GetRunningItemArgumentByPosition(m_oState1, m_oRunTimeStack->GetReference(),iArgToRead,vTemp);
	
	if (ret==M4_SUCCESS){
		//si el arg. es de tipo string habra que meter una copia de la cadena
		if (vTemp.Type==M4CL_CPP_TYPE_STRING_VAR){
			RuntimePushString(vTemp.Data.PointerChar, vTemp.IsUser);
		}
		else{
			RuntimePush(vTemp);
		}
	}
	return ret;
}

m4return_t ClVMachine1::GetArgumentNumber (void){

	RuntimePushDouble(GetRunningItemArgumentNumber(m_oState1, m_oRunTimeStack->GetReference()) );
	
	return M4_SUCCESS;
	
}




//-------------------------------
//AllocVarSpace
//-------------------------------
m4return_t ClVMachine1::AllocVarSpaceIfNeeded(void){
	
	m4VMState_t oState;
	m4VariantType vEmpty;
	m4uint32_t iNumVars;
	
	
	//Reservamos espacio para las varaibles en el caso de que
	//sea la instrucción 0, que es cuando se debe reservar
	m_oStateStack->Top(oState);

    //Sistema de posicionamiento de propagaciones
    if (oState.m_hItem==-1){
		//Explicación:
		//Durante la propagación es necesario ir posicionando los registros de los bloques padre e hijo. 
		//Para ello se meten estados 'ficticios' con handle -1 que indica a que registro de que 
		//recordset debemos movernos. Hay dos tipos de posicionamientos(segun flag), uno con moveto tal cual (posición lógica) 
		//y otros con MMindex, dejando el índice lógico en la misma posición que el MM, que puede incluso no ser visible!!! ,
		//pero así no se queda en eof, y el próximo posicionamiento actualizará bien el índice
		//El hecho de movernos con MM es para poder 'saltarnos los filtros', ya que la prop. no debe tenerlos en cuenta.
		//En VM1prop es donde se meten estos estados ficticios.

		if (M4CL_HAS_STATE_MM_POSITION_FLAG(oState.m_Flags) ){
			if (oState.m_oRegisterIndex<oState.m_poRecordSet->Count() ){
				oState.m_poRecordSet->Current.MoveToMM(oState.m_oRegisterIndex);
				oState.m_poRecordSet->Current.Index=oState.m_oRegisterIndex;
			}
			else{
				//Nos han borrado algun registro desde que empezó la propagación
				//ES peligrosísimo hacerlo, y  un error de lógica SEGURO, pero
				//como la gente es así de burra, pues nada, doy un error y me quedo tan contento
				DUMP_CHLOG_ERRORF(M4_VM_INCONSISTENT_PROPAGATION_REGISTER, DumpCallStack() );
				return M4_ERROR;
			}
		}
		else{

			// Bug 0304172 Sólo se posiciona si el bloque sigue cargado
			ClBlock_Channel_Proxy* poBlock = oState.m_poRecordSet->GetCurrentBlock() ;

			if( poBlock != NULL && poBlock->IsLoaded() == M4_TRUE )
			{
				oState.m_poRecordSet->Current.MoveTo(oState.m_oRegisterIndex);
			}

			/* Bug 0094704
			Si se ha forzado a que se haga un posicionamiento en los hijos
			se llama al refresh
			*/
			if( M4CL_HAS_STATE_REFRESH_FLAG( oState.m_Flags ) )
			{
				oState.m_poRecordSet->Current._Refresh() ;
			}
		}
		m_oStateStack->Pop(); //sacamos ese y segimos con el siguiente
        return M4_WARNING;
    }

	if (oState.m_iCurrent==0 && oState.m_poFormula.IsNotNull() && !M4CL_HAS_STATE_INIT_FLAG(oState.m_Flags) ){//es la primera instrucción y la formulan no es nula->Reservar variables...
		//Arrancamos el reloj para el item.
		if ( (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED) && (!M4CL_HAS_STATE_HIDE_FLAG(oState.m_Flags)) && (!M4CL_HAS_STATE_INIT_FLAG(oState.m_Flags) )) //ver nota
		{
			GetVMCSContext()->GetSttWrap()->GetStatistics()->sttStateBegin(oState);

			if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_PARAMS)
			{
				m4uint32_t i,iNumArg;
				m4VariantType vTemp;

				iNumArg = GetRunningItemArgumentNumber(oState, GetRuntimeStack()->GetReference());

				for (i=1; i<=iNumArg; ++i)
				{
					if ( M4_SUCCESS != GetRunningItemArgumentByPosition(oState, GetRuntimeStack()->GetReference(), i , vTemp) )
						break;

					GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddExecParam(vTemp);
				}
			}
		}
	
#ifdef _DEBUG
		m4int32_t iReferences = -1;
		m4int8_t iType = oState.m_poFormula.GetType();

		m4uint16_t	iNumArg = 0;
		m4uint16_t	iVarArgs = oState.m_poAccess->GetpCMCR()->GetItemVariableArguments(oState.m_hItem);

		if ( iVarArgs != 0 )
		{
			m4VariantType vTemp2;

			m_oRunTimeStack->Top(vTemp2);
			iNumArg = (m4int32_t)vTemp2.Data.DoubleData +1;
		}
		else
		{
			iNumArg = oState.m_poAccess->GetpCMCR()->GetItemNumberOfArguments(oState.m_hItem);
		}

		if( iType == M4CL_LENGUAGE_CPP )
		{
			iReferences = m_oRunTimeStack->GetPos() + 1 - iNumArg ;
		}
#endif

		iNumVars=oState.m_poFormula.GetNumVars();
		for (m4uint32_t i=0;i < iNumVars; i++){
			RuntimePush(vEmpty);
		}

#ifdef _DEBUG
		if( iType == M4CL_LENGUAGE_LN4 )
		{
			iReferences = m_oRunTimeStack->GetPos() + 1 ;
		}

		m_oStateStack->Pop();
		oState.m_iDebugVarReferences = iReferences ;
		m_oStateStack->Push(oState);
#endif

	}

	return M4_SUCCESS;
}


//----------------------------------------------------------------------
//F I N A L I Z A C I O N   D E    L A   E J E C U C I O N
//----------------------------------------------------------------------


m4return_t  ClVMachine1 :: _FreeAtEndOfMethod (void){
	
    m4return_t ret;
	m4VariantType vTempVariant;
	m4int32_t i;
	m4VMState_t oState;
	m4uint16_t iNumArg, iNumArgRef, iVarArgs ;
	m4int32_t iNumVar, iNumPosToDelete, iNumPosToPop;
	ClItem *poItem;
    m4VariantType   vTemp2;
    m4int8_t iPrior, iMustAssign;
	
	ret=M4_SUCCESS;
	
	
    //leemos el estado
    ret=m_oStateStack->Top(oState);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Empty execution stack at level 1.")	;

	m_oStateStack->Pop();
    
	if (m_BeginTransaction >= m_oStateStack->GetPos()){
		//acaba el método que inicio la transación->finaliza la transación
		EndVMTrans();
	}

    ClCompiledMCR   *poCMCR=oState.m_poAccess-> GetpCMCR() ;

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Item " <<LogCat<< poCMCR->GetItemId(oState.m_hItem) <<LogCat<< "finished its execution" );
#endif

	
    if (oState.m_poFormula.IsNull()){ //si no habia nada para ejecutar
        DeleteNullMethod(oState.m_poAccess, oState.m_hItem, 
							(M4CL_HAS_STATE_PURGE_FLAG (oState.m_Flags) ? M4_TRUE : M4_FALSE ),
							(M4CL_HAS_STATE_PURGE_REF_FLAG (oState.m_Flags) ? M4_TRUE : M4_FALSE ), 
							(M4CL_HAS_STATE_COMPACT_FLAG (oState.m_Flags) ? M4_TRUE : M4_FALSE ), 
							oState.m_dBeginDate, 
							oState.m_dEndDate );
		DUMP_CHLOG_WARNINGF(M4_CH_VM_DEL_NULL_METH, M4ObjNodeItemI(poCMCR,oState.m_hItem) << DumpCallStack() );
        //restauramos la referencia de la pila de ejecución
        m_oRunTimeStack->SetReference(oState.m_iOldStackReference); 
		
        return M4_SUCCESS;
    }
	
	//tenemos que liberar
    
    iNumVar		=	oState.m_poFormula.GetNumVars();

	
	if (!M4CL_HAS_STATE_EXE_JIT_FLAG(oState.m_Flags))
	{
	    iNumArg		=	poCMCR->GetItemNumberOfArguments(oState.m_hItem);
	    iNumArgRef	=	poCMCR->GetItemNumberOfReferedArguments(oState.m_hItem);
		iVarArgs	=	poCMCR->GetItemVariableArguments ( oState.m_hItem);

	}
	else 
	{
		// Lo marcamos con argumentos variables, para poder indicar el numero.
		iNumArgRef = 0;
		iNumArg = 0;
		iVarArgs = 1;
	}

#ifdef _DEBUG
	if( oState.m_iDebugVarReferences != -1 )
	{
		if( oState.m_iDebugVarReferences > m_oRunTimeStack->GetPos() - iNumArgRef )
		{
			int x = 0 ;
		}
		else if( oState.m_iDebugVarReferences < m_oRunTimeStack->GetPos() - iNumArgRef )
		{
			int y = 0 ;
		}
	}
#endif

    //para las prioridades
    iPrior		=	oState.m_poFormula.GetPriority();        
    iMustAssign =	oState.m_poFormula.GetMustAssign();
	
    if ( M4CL_HAS_STATE_PURGE_FLAG (oState.m_Flags) && !M4CL_HAS_STATE_CONCEPT_FLAG (oState.m_Flags)){
		if (M4CL_HAS_STATE_PURGE_REF_FLAG (oState.m_Flags) ){ //nos cargamos resultado y args. por referencia
			iNumPosToDelete=iNumArgRef+1;
			iNumPosToPop=0;
		}
		else{	//solo nos cargamos el resultado
			iNumPosToPop=iNumArgRef; //para cargarnos el resultado
			iNumPosToDelete=1;
		}
    }
    else{
        iNumPosToPop=iNumArgRef+1; //salvamos el res
        iNumPosToDelete=0;
    }
    
	
    //leemos del tope de la pila->El resultado + las variables por referencia
    for (i=0;i<iNumPosToPop;i++){
        ret=RuntimePop(vTempVariant);
		
        //metemos en la pila de resultados las varaible por ref. y el resultado
        m_oResultStack->Push(vTempVariant);
    }
    
	
	
    //eliminamos de la pila todo lo que esté por encima de
    //la referencia y tantas posiciones por debajo como argumentos teníamos
    //y ponemos lo que había en el tope en la pila una vez quitado todo lo que hay que quitar
    
    //a) Sacamos las varaibles
    iNumPosToDelete+=iNumVar;
    
    for (i=0;i<iNumPosToDelete;i++){
        ret=RuntimePopAndDelete();
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop from RTS freeing variables at the end of execution.")	;
    }
    
    //b) Sacamos los argumentos
	
    switch (oState.m_poFormula.GetType() ){
        
    case M4CL_LENGUAGE_LN4:
        {
            if ( iVarArgs != 0 ) { //argumentos variables
                ret=RuntimePopDouble(vTemp2);
                if (ret==M4_SUCCESS){
                    iNumPosToDelete=(m4int32_t)vTemp2.Data.DoubleData;
                }
                else{
                    iNumPosToDelete=0;
                }
            }
            else{
                iNumPosToDelete=iNumArg;
            }
            break;
        }
    case M4CL_LENGUAGE_CPP:
        {
            iNumPosToDelete=0;
            break;
        }
    }
    
	
    for (i=0;i<iNumPosToDelete;i++){
        ret=RuntimePopAndDelete();
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop from RTS freeing variables at the end of execution.")	;
    }
	
	//_change_
	//borramos la formula, (ya no la usaremos)
	//solo no borramos cuando es jit  y preparada
	if ( !M4CL_HAS_STATE_PREP_JIT_FLAG(oState.m_Flags) && M4CL_HAS_STATE_EXE_JIT_FLAG(oState.m_Flags)){ //si es jit, además borramos el código instalado en sí
		delete [] (m4char_t*)(oState.m_poFormula.GetFormulaHandle());
	}
	

    if (M4CL_HAS_STATE_CONCEPT_FLAG (oState.m_Flags) ){
        
        ret=m_oResultStack->Top(vTempVariant);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop from temp stack.")	;
        
		ClItemIndex iItem;

        // si el registro está a EOF y el item es de registro no podemos asignarnos
        if (oState.m_oRegisterIndex == M4DM_EOF_INDEX && poCMCR->GetItemScope(oState.m_hItem) == M4CL_ITEM_SCOPE_REGISTER ){
            //no podemos asignarnos
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_NUMBER_WRITE,M4ObjNodeItemI(poCMCR,oState.m_hItem)<<M4ObjNodeItemI(poCMCR,oState.m_hItem)<<DumpTreePath(oState.m_poAccess, oState.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        }
        else if (iMustAssign){ //Nos asignamos
			
			iItem = poCMCR->GetItemIndex (oState.m_hItem);

            if (GetRunSliceMode(oState.m_poAccess)){
				poItem=&(oState.m_poRecordSet->Register[oState.m_oRegisterIndex].Item[iItem]);
				
                ret=CheckAndCreateItemSlice (m_oState1.m_dBeginDate, m_oState1.m_dEndDate ,poItem);

                if (ret==M4_SUCCESS){
					/* Bug 0076620
					Se asignan los subtramos si los hubiera
					*/
					/* Bug 0095793
					La macro M4CL_HAS_STATE_COMPACT_FLAG no devuelve booleano y hay que convertirlo
					*/
                    ret=SetValue(*poItem, vTempVariant, iPrior, 0, m_oState1.m_dBeginDate, m_oState1.m_dEndDate, ( M4CL_HAS_STATE_COMPACT_FLAG( oState.m_Flags ) ? M4_TRUE : M4_FALSE ) );    //no movemos el item porque ya esta puesto    
                }
			
				/* Bug 0069365
				Aunque falle hay que seguir para limpiar la pila
				*/
				if (ret==M4_ERROR) {
					DUMP_CHLOG_ERRORF(M4_CH_VM_SET_CONCEPT_SLICE_VALUE, M4ObjNodeItemI(poCMCR,oState.m_hItem)<<DateLog(m_oState1.m_dBeginDate)<<DateLog(m_oState1.m_dEndDate)<<M4ObjNodeItemI(poCMCR,oState.m_hItem) << DumpCallStack() );
					ret=M4_SUCCESS;
				}
				
                if (ret==M4_WARNING){
					DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED_EXE_ITEM, M4ObjNodeItemI(poCMCR,oState.m_hItem)<<M4ObjNodeItemI(poCMCR,oState.m_hItem) << DumpCallStack() );
					ret=M4_SUCCESS;
				}
                
				/* Bug 0085200
				Ya se compacta en el SetValue si hace falta

                //Si acabamos de ejecutar un item por tramos-> compactamos si debemos
                if (M4CL_HAS_STATE_COMPACT_FLAG (oState.m_Flags )){ 

                    poItem->Value.Compact();
                    //Compactamos los items que asignamos
                    ret=CompactAssignedItems(oState.m_poAccess,oState.m_hItem);

					// Bug 0069365 Aunque falle hay que seguir para limpiar la pila
					if (ret==M4_ERROR) {
						DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_COMPACTING_ASSIGNED, M4ObjNodeItemI(poCMCR ,oState.m_hItem) << DumpCallStack() );
						ret=M4_SUCCESS;
					}
                }
				*/
            }
            else{
				ret=__SetValue(oState.m_poRecordSet->Register[oState.m_oRegisterIndex].Item[iItem], vTempVariant, iPrior, 0);

				/* Bug 0069365
				Aunque falle hay que seguir para limpiar la pila
				*/
				if (ret==M4_ERROR) {
					DUMP_CHLOG_ERRORF(M4_CH_VM_SET_CONCEPT_VALUE, M4ObjNodeItemI(poCMCR ,oState.m_hItem ) << DumpCallStack() );
					ret=M4_SUCCESS;
				}
				
                if (ret==M4_WARNING){
					DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED_EXE_ITEM, M4ObjNodeItemI(poCMCR,oState.m_hItem)<<M4ObjNodeItemI(poCMCR,oState.m_hItem) << DumpCallStack() );
					ret=M4_SUCCESS;
				}
            }
        }

		if (M4CL_HAS_STATE_PURGE_FLAG  (oState.m_Flags) ){
            m_oResultStack->Pop();
            iNumPosToPop--; //el resultado ya lo hemos sacado
			DeleteVarString (vTempVariant); //por si es cadena
			
			//miramos que hacer con los argumentos por referencia
			if (M4CL_HAS_STATE_PURGE_REF_FLAG  (oState.m_Flags) ){
				for (i=0;i<iNumPosToPop;i++){
					m_oResultStack->Top(vTempVariant);
					m_oResultStack->Pop();	
					DeleteVarString (vTempVariant); //por si es cadena
				}
				iNumPosToPop=0;//ya no hay que meter nada
			}
        }
    }//de si es concepto
    
    //de la pila de resultados a la de ejecución
    for (i=0;i<iNumPosToPop;i++){
        ret=m_oResultStack->Top(vTempVariant);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop from RTS.")	;
        m_oResultStack->Pop();					
        //metemos en la pila de ejecución las var. por referencia y el resultado
        ret=RuntimePush(vTempVariant);
    }
    
    //restauramos la referencia de la pila de ejecución
    m_oRunTimeStack->SetReference(oState.m_iOldStackReference); 
	
	//escribimos el tiempo empleado por el item
	if ( (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED) && (!M4CL_HAS_STATE_HIDE_FLAG(oState.m_Flags)) )
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttStateEnd(oState.m_hItem);
	
    return ret;
}


//--------------------------------------------------------------------------
//Crear los estados de la ejecución de un item
//--------------------------------------------------------------------------
//*****************************
//*E J E C U T A R
//*****************************
m4return_t ClVMachine1::CreateExecute (m4VMState_t ai_oState,m4uint16_t ai_iNumArgs, m4VariantType ai_vSlice, m4bool_t ai_bSpecSlice){    
/*Execute que se llama desde el canal.
Recibe como param. en "estado" en el que está toda la
información sobre lo que debemos ejecutar.
A partir de esta información ejecutamos teniendo en 
cuenta tramos....
    */
    

    m4return_t			ret;
    m4uint16_t			i,j;
    m4bool_t            bRunSliceMode;
	m4int32_t			iParentStatePos ;
    
	m4char_t date1[24], date2[24];

    
	/* Bug 0069037
	Todos los métodos son hijos del mismo padre
	*/
	iParentStatePos = m_oStateStack->GetPos() ;
    
    
    //miramos a ver si el número de argumentos es variable
    m4uint8_t bVariable=ai_oState.m_poAccess->GetpCMCR()->GetItemVariableArguments (ai_oState.m_hItem);
    if (bVariable != 0){
        //metemos el número de argumentos
        RuntimePushDouble (ai_iNumArgs);
        //ahora consideramos que tenemos un arg. más para que lo salve...
        ai_iNumArgs++;
    }
    
    
    //por tramos solo los conceptos
    bRunSliceMode=GetRunSliceMode(ai_oState.m_poAccess);
    
    //Si es ejecución por tramos y no especifican tramo
    if ( bRunSliceMode && !ai_bSpecSlice ){
        
		if (ClVMDebugEnv::getInstance()->getDebugLevel() >= 3)
		{
			ClCompiledMCR	*pMCR = ai_oState.m_poAccess->GetpCMCR() ;
			VMTracer::GetInstance(ai_oState.m_poAccess->GetEnvironment())
				<< beginthread
				<< "Generating sliced execution states for item <"
				<< pMCR->iM4Obj() << "!" << pMCR->iNodeI( ai_oState.m_hItem ) << "." << pMCR->iItemI( ai_oState.m_hItem )
				<< ">" << endthread ;
		}

		m4date_t			*padSlices = NULL;
		m4uint16_t			iNumSlices = 0;
		m4VariantType		*param=0;
		

        //Nos aseguramos de que no tewnga argumentos por referencia.!!! ->Limitación temporal
        //YA SE PERMITE!!! M4CHECK_RES (ai_oState.m_poAccess-> GetpCMCR()->GetItemNumberOfReferedArguments (ai_oState.m_hItem) ==0, M4_CH_VM_NO_REFERED_ARG_ALLOWED_IN_SLICE, ERRORLOG, M4ObjNodeItemI(ai_oState.m_poAccess->GetpCMCR(),ai_oState.m_hItem) )	;
        
        //1ºgeneramos los tramos para el item que vamos a ejecutar
		ret=DetSlicesOfItem(ai_oState.m_poAccess, ai_oState.m_hItem, ai_oState.m_dBeginDate, ai_oState.m_dEndDate, iNumSlices, padSlices);
        if (ret!=M4_SUCCESS) {
            DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE_CREATION, M4ObjNodeItemI(ai_oState.m_poAccess->GetpCMCR(),ai_oState.m_hItem) << DumpCallStack() );
            return M4_ERROR;
        }
        
        
        //3ºPara cada tramo del item generamos un estado y lo metemos a ejecutar (en orden inverso)
        //metiendo los argumentos si hay más de un tramo...)
        //3.1 Argumentos
        if (iNumSlices > 1 && ai_iNumArgs){	//copiamos los argumentos porque habrá que meterlos más de una vez
			
			param=new m4VariantType [ai_iNumArgs];

			for (i=0; i < ai_iNumArgs; ++i){
                ret = m_oRunTimeStack->Top(param[ai_iNumArgs-i-1], i);
                DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop from stack.")	;
            }
        }
        
        //Creamos el estado correspondiente al último a ejecutar (el primero que debemos meter en la cola)
        
        M4CL_SET_STATE_COMPACT_FLAG (ai_oState.m_Flags);    //el ultimo trammo debe compactar
        ai_oState.m_dBeginDate = padSlices[0];
        
        ret=AddMethodToExecute (ai_oState, ai_vSlice, iParentStatePos, ai_bSpecSlice);
		
		
        if (ret!=M4_SUCCESS){
			delete [] padSlices;
			delete [] param;
            return M4_ERROR;
        }
		
        
		if (ClVMDebugEnv::getInstance()->getDebugLevel() >= 3)
		{
			ClDateToString (ai_oState.m_dBeginDate, date1, 23);
			ClDateToString (ai_oState.m_dEndDate  , date2, 23);
			ClCompiledMCR	*pMCR = ai_oState.m_poAccess->GetpCMCR() ;
			VMTracer::GetInstance(ai_oState.m_poAccess->GetEnvironment())
				<< beginthread
				<< "Creating slice {" << date1 << "} {" << date2
				<< "} for execution of item <"
				<< pMCR->iM4Obj() << "!" << pMCR->iNodeI( ai_oState.m_hItem ) << "." << pMCR->iItemI( ai_oState.m_hItem )
				<< ">" << endthread ;
		}
        
        
        //los args los metemos n-1 vez (ya estan metidos una vez)
        
        ai_oState.m_Flags=0;
        M4CL_SET_STATE_PURGE_FLAG     (ai_oState.m_Flags);  //todos menos el ultimo slice deben purgar su resultado
        M4CL_SET_STATE_PURGE_REF_FLAG (ai_oState.m_Flags);  //todos menos el ultimo slice deben purgar sus argumentos por referencia
        
		for (i=0; i < iNumSlices-1 ; ++i){ //por cada slice (execpto el primero) metemos una copia de los argumentos
            
            for (j=0; j < ai_iNumArgs; ++j){
                if (param[j].Type ==M4CL_CPP_TYPE_STRING_POOL){ //Si es cadena la copiamos otra vez
                    m_oDynamicPool->AddString(m_oDynamicPool->GetString(param[j].Data.CharData), param[j].Data.CharData);
                }
                ret=RuntimePush( param [j] );
                DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_PUSH, ERRORLOG, "Error performing push to stack.")	;
            }	
            
            ai_oState.m_dBeginDate = padSlices[i+1];
            ai_oState.m_dEndDate = padSlices[i]-m_dUnit;
            
            ret=AddMethodToExecute (ai_oState,  ai_vSlice, iParentStatePos, ai_bSpecSlice);
            if (ret!=M4_SUCCESS){
				delete [] padSlices;
				delete [] param;
                return M4_ERROR;
            }
            
            
			if (ClVMDebugEnv::getInstance()->getDebugLevel() >= 3)
			{
				ClDateToString (ai_oState.m_dBeginDate, date1, 23);
				ClDateToString (ai_oState.m_dEndDate  , date2, 23);
				ClCompiledMCR	*pMCR = ai_oState.m_poAccess->GetpCMCR() ;
				VMTracer::GetInstance(ai_oState.m_poAccess->GetEnvironment())
					<< beginthread
					<< "Creating slice {" << date1 << "} {" << date2
					<< "} for execution of item <"
					<< pMCR->iM4Obj() << "!" << pMCR->iNodeI( ai_oState.m_hItem ) << "." << pMCR->iItemI( ai_oState.m_hItem )
					<< ">" << endthread ;
			}

        } //for
			
		delete [] padSlices;
		delete [] param;
    }//del si tramos y no especifican tramos
    
    else{//Si es ejecución sin tramos o especifican un tramos

#if _M4_DEBUG > 10 
        DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Generating execution state of item" <<LogCat<< ai_oState.m_poAccess->GetpCMCR()->GetItemId(ai_oState.m_hItem) );
#endif
        //1º meter el estado para el item a ejecutar (los argumentos ya están en la pila)
        ret=AddMethodToExecute (ai_oState, ai_vSlice, iParentStatePos, ai_bSpecSlice);
        
        if (ret!=M4_SUCCESS){
            return M4_ERROR;
        }
		
        
    }

	return ret;
}
//--------------------------------------------------------------------------
//Execute group
//--------------------------------------------------------------------------
m4return_t ClVMachine1::ExecuteGroup(void){
	

	m4uint32_t  hGroup, hNodo, hItem;
    m4uint16_t  iNumItems;
    m4return_t  ret=M4_SUCCESS;
    m4VariantType   vTemp;
    m4int16_t   i,j;
	m4uint32_t	aiNumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_MAX_TYPES];
	
	ClCompiledMCR   *poCMCR=m_oState1.m_poAccess-> GetpCMCR() ;
    //al crear los estados de ejecución hay que indicar 
    //que el ultimo, en caso de que tenga tramos tambien hay que purgarlos .
    //handle del grupo
    hNodo = poCMCR->GetItemNodeHandle(m_oState1.m_hItem); //si lo llaman como función

#ifdef _DEBUG
	if (hNodo==0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_NODE_HANDLE, "ExecuteGroup:"<<LogCat<< poCMCR->iItemI(m_oState1.m_hItem) <<""<<"" << DumpCallStack() );
		return M4_ERROR;
	}
#endif
    
    //sacamos el nombre del grupo a ejecutar
   	ret=RuntimePopString (vTemp);
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "GroupId" << "ExecuteGroup" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "\t\t\tExecute group" <<LogCat<< m_oDynamicPool->GetString(vTemp.Data.CharData) );
#endif


	ClInstruction * poIns;


    //obtenemos el handle del grupo
	hGroup=poCMCR->GetNodeGroupHandleById(hNodo , m_oDynamicPool->GetString(vTemp.Data.CharData));             
    
	if (hGroup!=NULL){

        iNumItems=poCMCR->GetGroupNumberOfItems(hGroup);
	
		//reservamos el array de instrucciones
		poIns=new ClInstruction[(iNumItems*2)+2];

        //para cada item del grupo lo mandamos ejecutar (en oreden inverso)
        for (i=0; i<iNumItems;++i){
            
            hItem=poCMCR->GetGroupItemHandleByPosition(hGroup,i);

            if (m_oState1.m_hItem==hItem){
		        DUMP_CHLOG_ERRORF(M4_CH_VM_RECURSIVE_EXE_GROUP, m_oDynamicPool->GetString(vTemp.Data.CharData)<< M4ObjNodeItemI (poCMCR, m_oState1.m_hItem) );
                delete [] poIns;
				m_oDynamicPool->DeleteString(vTemp.Data.CharData);
				return M4_ERROR;
            }

				j=i*2;
			//rellenamos instruccion de ejecutar item
				poIns[j].m_Flags=0;
				M4CL_SET_LEVEL_FLAG(poIns[j].m_Flags,1);
				M4CL_SET_ITEM_FLAG(poIns[j].m_Flags);

				poIns[j].m_iTipoIns = M4CL_MCR_CODE_INSTRUCTION_TYPE_ITEM;
				poIns[j].m_iNumFunc=M4CL_VM_FT_EXECUTEITEM;
				poIns[j].m_iNumItem_Var.m_iNumItem=hItem;
				//poIns[i].m_iNumItem_Var.m_iNumVar=hItem;
				poIns[j].m_iNumNodo=hNodo;
				
				poIns[j].m_iNumRegla=0;
				poIns[j].m_uVariant.DoubleData=0;
				poIns[j].m_bIsChar=M4_FALSE;
				poIns[j].m_iNumAtrib=0;
		        poIns[j].m_poNode=0;
				poIns[j].m_poItem=0;
		        poIns[j].m_pcNameChannel=0;

			//siguiente instruccion				
				++j;
			//rellenamos instruccion de purgar 1 la pila

				poIns[j].m_Flags=0;
				M4CL_SET_LEVEL_FLAG(poIns[j].m_Flags,0);
				poIns[j].m_iTipoIns = M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL;
				poIns[j].m_iNumFunc=g_aiFuncToFlag[M4CL_VM_EMB_INS_PURGE];
				poIns[j].m_iNumItem_Var.m_iNumItem=1; //purgar un elemento de la pila
				//poIns[i].m_iNumItem_Var.m_iNumVar=hItem;
				poIns[j].m_iNumNodo=0;
				
				poIns[j].m_iNumRegla=0;
				poIns[j].m_uVariant.DoubleData=0;
				poIns[j].m_bIsChar=M4_FALSE;
				poIns[j].m_iNumAtrib=0;
		        poIns[j].m_poNode=0;
				poIns[j].m_poItem=0;
		        poIns[j].m_pcNameChannel=0;
        }

		++j;
		//Rellenamos las dos últimas instrucciones ->Resultado + return
			//meter resultado
		poIns[j].m_Flags=0;
		M4CL_SET_LEVEL_FLAG(poIns[j].m_Flags,0);
		poIns[j].m_iTipoIns = M4CL_MCR_CODE_INSTRUCTION_TYPE_VARIANT;
		poIns[j].m_iNumFunc=M4CL_VM_FT_PUSHNUM;
		poIns[j].m_uVariant.DoubleData=0.0; //valor a retornar
		poIns[j].m_bVariantAsInt=M4_FALSE;
		poIns[j].m_bIsChar=M4_FALSE;

		poIns[j].m_iNumItem_Var.m_iNumItem=0; 
		poIns[j].m_iNumNodo=0;
		poIns[j].m_iNumRegla=0;
		poIns[j].m_bIsChar=M4_FALSE;
		poIns[j].m_iNumAtrib=0;
		poIns[j].m_poNode=0;
		poIns[j].m_poItem=0;
		poIns[j].m_pcNameChannel=0;
 
		++j;
			//salir
		poIns[j].m_Flags=0;
		M4CL_SET_LEVEL_FLAG(poIns[j].m_Flags,0);
		poIns[j].m_iTipoIns = M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL;
		poIns[j].m_iNumFunc=g_aiFuncToFlag[M4CL_VM_EMB_INS_RETURN];
		poIns[j].m_uVariant.DoubleData=0.0; //valor a retornar
		poIns[j].m_bVariantAsInt=M4_FALSE;
		poIns[j].m_bIsChar=M4_FALSE;

		poIns[j].m_iNumItem_Var.m_iNumItem=0; 
		poIns[j].m_iNumNodo=0;
		poIns[j].m_iNumRegla=0;
		poIns[j].m_bIsChar=M4_FALSE;
		poIns[j].m_iNumAtrib=0;
		poIns[j].m_poNode=0;
		poIns[j].m_poItem=0;
		poIns[j].m_pcNameChannel=0;

		//rellenamos el número de instrucciones de cada tipo
		aiNumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_TYPE_SIMPLE ]=0;
		aiNumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_TYPE_VARIANT]=1;
		aiNumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL]=iNumItems+1;
		aiNumInstructionsType[M4CL_MCR_CODE_INSTRUCTION_TYPE_ITEM   ]=iNumItems;
	
		//llamamos a instalar + ejecutar
		ret=m_oImplCompilerAndJit.InstallRTGC	(poIns, (iNumItems*2)+2,  aiNumInstructionsType, M4CL_MCR_CODE_INSTRUCTION_MAX_TYPES);
	
		//borramos el array de instrucciones
		delete [] poIns;

    }
    else{
        //Si no existe el grupo, retornamos un warning en la pila, pero dejamos seguir ejecutando
        DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_GROUP, m_oDynamicPool->GetString(vTemp.Data.CharData)<<  DumpCallStack() );
		RuntimePushDouble(M4_WARNING);
    }
	//liberamso la cadena
	m_oDynamicPool->DeleteString(vTemp.Data.CharData);
		
	return ret;

}

//--------------------------------------------------------------------------
//Reset Execute Group
//--------------------------------------------------------------------------
m4return_t ClVMachine1::ResetExecuteGroup(void){
	

	m4uint32_t  hGroup, hNodo, hItem;
    m4uint16_t  iNumItems;
    m4return_t  ret=M4_SUCCESS;
    m4VariantType   vTemp;
    m4int16_t   i;
	
	ClCompiledMCR   *poCMCR=m_oState1.m_poAccess-> GetpCMCR() ;
    hNodo = poCMCR->GetItemNodeHandle(m_oState1.m_hItem); //si lo llaman como función

#ifdef _DEBUG
	if (hNodo==0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_NODE_HANDLE, "ResetExecuteGroup:"<<LogCat<< poCMCR->iItemI(m_oState1.m_hItem) <<""<<"" << DumpCallStack() );
		return M4_ERROR;
	}
#endif
    
    //sacamos el nombre del grupo a ejecutar
   	ret=RuntimePopString (vTemp);
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "GroupId" << "ResetExecuteGroup" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "\t\t\t Reset Execute group" <<LogCat<< m_oDynamicPool->GetString(vTemp.Data.CharData) );
#endif

    //obtenemos el handle del grupo
	hGroup=poCMCR->GetNodeGroupHandleById(hNodo , m_oDynamicPool->GetString(vTemp.Data.CharData));             
    
	if (hGroup!=NULL){

        iNumItems=poCMCR->GetGroupNumberOfItems(hGroup);
	
		//Lo usamos para resetear la prioridad de los conceptos del grupo
		ClItem oItemWalker(0);
		oItemWalker = m_oState1.m_poAccess->Node[ClHandle(hNodo)].RecordSet.Register.Item;

        //para cada item del grupo lo mandamos ejecutar (en orden inverso)
        for (i=0; i<iNumItems;++i){
            hItem=poCMCR->GetGroupItemHandleByPosition(hGroup,i);
			oItemWalker[ClHandle(hItem)].Value.SetPriority(); //Por defecto es 3, es la mínima
        }
    }
    else{
        //Si no existe el grupo, retornamos un warning en la pila, pero dejamos seguir ejecutando
        DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_GROUP, m_oDynamicPool->GetString(vTemp.Data.CharData)<<  DumpCallStack() );
		RuntimePushDouble(M4_WARNING);
    }
	//liberamso la cadena
	m_oDynamicPool->DeleteString(vTemp.Data.CharData);
	RuntimePushDouble(M4_SUCCESS); //Devolvemos 'exito
		
	return ret;

}


//------------------------------------------------------------
//Crear lista de slices a crear
////En la pila de fechas retorna los tramos a crear,
//------------------------------------------------------------
m4return_t	ClVMachine1::AddSlicesStack (ClAccess *ai_poAccess,m4uint32_t ai_handle){
	
	
    m4uint32_t	iNumConditionants,  iNumNormas;
    m4uint32_t	HandleConditionant ;
    m4uint32_t  hNodo;
    ClItem		*oItem;
    ClSliceIndex i;
    //ClSliceIndex j;
    ClCompiledMCR   *poCMCR=ai_poAccess-> GetpCMCR() ;
	ClItemIndex  IndexConditionant,IndexInput;
	ClNode *poNode;
	m4uint8_t	iScope,iType;
	
    
	switch (poCMCR->GetItemSliceSplit(ai_handle) )
    {
        
    case M4CL_ITEM_SLICE_SPLIT_LINEAR:
        {
            iNumConditionants = poCMCR->GetItemNumberOfConditionants(ai_handle);
            for (i=0;i <iNumConditionants ; ++i){
                HandleConditionant = poCMCR->GetItemConditionantHandleByPosition (ai_handle, (m4uint16_t)i );
				if (poCMCR->GetItemGenerateSlices (HandleConditionant) ){//si es de los que no genera tramos (incidencias) pasamos.
				
					hNodo=poCMCR->GetItemNodeHandle( HandleConditionant );
#ifdef _DEBUG
					if (!hNodo){
						DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_NODE_HANDLE, "AddSlicesStack (dependencies-no_slices):"<<LogCat<< poCMCR->iItemI(ai_handle) <<""<<"" << DumpCallStack() );
						return M4_ERROR;
					}
#endif
					
					poNode=&(ai_poAccess->Node[ClHandle(hNodo)]);
					
					iScope=poCMCR->GetItemScope(HandleConditionant);
					if ((iScope!=M4CL_ITEM_SCOPE_REGISTER   ||  poNode->RecordSet.Current.Index != M4DM_EOF_INDEX ) &&
						(iScope==M4CL_ITEM_SCOPE_NODE       ||  poNode->RecordSet.IsValid()				 )){
						
						IndexConditionant=poCMCR->GetItemIndex(HandleConditionant);
						oItem=&(poNode->RecordSet.Current.Item[IndexConditionant]) ;
						oItem->Value.FillDateStack(&m_oDatesStack); 
					}
				}//incidencias
            }//for
            
			
            break;
        }
        
    case M4CL_ITEM_SLICE_SPLIT_NON_LINEAR:
        {
            HandleConditionant = poCMCR->GetItemSliceItem (ai_handle);
			if (HandleConditionant && poCMCR->GetItemGenerateSlices (HandleConditionant) ){//si tiene condicionante y es de los que genera tramos
				hNodo=poCMCR->GetItemNodeHandle( HandleConditionant );
#ifdef _DEBUG
				if (!hNodo){
					DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_NODE_HANDLE, "AddSlicesStack (dependencies-no_slices):"<<LogCat<< poCMCR->iItemI(ai_handle) <<""<<"" << DumpCallStack() );
					return M4_ERROR;
				}
#endif
				
				poNode=&(ai_poAccess->Node[ClHandle(hNodo)]);
				
				
				iScope=poCMCR->GetItemScope(HandleConditionant);
				if ((iScope!=M4CL_ITEM_SCOPE_REGISTER   ||  poNode->RecordSet.Current.Index != M4DM_EOF_INDEX ) &&
					(iScope==M4CL_ITEM_SCOPE_NODE       ||  poNode->RecordSet.IsValid()				)){
					
					IndexConditionant=poCMCR->GetItemIndex(HandleConditionant);
					oItem=&(poNode->RecordSet.Current.Item[IndexConditionant]) ;
					oItem->Value.FillDateStack(&m_oDatesStack); 	
				}
            }
            break;
        }
        
    case M4CL_ITEM_SLICE_SPLIT_NO_SLICES:
        {
            break;
        }
        
    }//switch

	iType=poCMCR->GetItemType(ai_handle);

	if (iType !=M4CL_ITEM_TYPE_METHOD){
		
		hNodo=poCMCR->GetItemNodeHandle(ai_handle);
#ifdef _DEBUG
		if (!hNodo){
			DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_NODE_HANDLE, "AddSlicesStack (dependencies-no_slices):"<<LogCat<< poCMCR->iItemI(ai_handle) <<""<<"" << DumpCallStack() );
			return M4_ERROR;
		}
#endif
		
		poNode=&(ai_poAccess->Node[ClHandle(hNodo)]);
		
		iScope=poCMCR->GetItemScope(ai_handle);
		if( (iScope!=M4CL_ITEM_SCOPE_REGISTER   ||  poNode->RecordSet.Current.Index != M4DM_EOF_INDEX ) &&
			(iScope==M4CL_ITEM_SCOPE_NODE       ||  poNode->RecordSet.IsValid()				)){

			//si tuviera tramos el concepto ya creados, tambien los tenemos en cuenta
			IndexInput=poCMCR->GetItemIndex(ai_handle);
			oItem=&(poNode->RecordSet.Current.Item[IndexInput]) ;
			oItem->Value.FillDateStack(&m_oDatesStack); 			
		}

		//metemos las que tenga según su formula (formulas definidas)
		iNumNormas=poCMCR->GetItemNumberOfSlices (ai_handle);
		for (i=0;i < iNumNormas; ++i){
			m_oDatesStack.AddElement ( poCMCR->GetItemSliceStartDate (ai_handle , (m4uint16_t) i) );
		}
	}

	return M4_SUCCESS;
}




//---------------------------------------------------------
//Creación de tramos. 
//---------------------------------------------------------
 m4return_t	ClVMachine1::DetSlicesOfItem    (ClAccess *ai_poAccess,
                                             m4uint32_t ai_handle,
											 m4date_t ai_dStart,
											 m4date_t ai_dEnd,
											 m4uint16_t &aio_iNumSlices,
 											 m4date_t* &ao_pdSlicesDates){

	 ClCompiledMCR   *poCMCR=ai_poAccess-> GetpCMCR() ;
	 m4uint16_t	iLength, iNumExecuted, iNumNormas;
	 m4uint32_t  HandleExecuted;
	 m4return_t ret;
	 ClSliceIndex i,j;
	 
	 m_oDatesStack.Clear (ai_dStart, ai_dEnd);
	 ret=AddSlicesStack (ai_poAccess, ai_handle);
	 

	 DEBUG_CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Error getting slices dates.")	;
	 
	 
	 //Metemos los ejecutados
	 iNumExecuted = poCMCR->GetItemNumberOfExecuted(ai_handle);
	 for (i=0;i <iNumExecuted ; ++i){
		 HandleExecuted = poCMCR->GetItemExecutedHandleByPosition (ai_handle, (m4uint16_t)i );
		 //ret=AddSlicesStack     (ai_poAccess, HandleExecuted);
		 //DEBUG_CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Error getting slices dates.")	;
		 iNumNormas=poCMCR->GetItemNumberOfSlices (HandleExecuted);
		 for (j=0;j < iNumNormas; ++j){
			 m_oDatesStack.AddElement ( poCMCR->GetItemSliceStartDate (HandleExecuted , (m4uint16_t) j) );
		 }
		 

	 }
	 m_oDatesStack.Reduce();
	 //metemos las fechas en el array
	 iLength= m_oDatesStack.GetLength() ;
	 
	 ao_pdSlicesDates = new m4date_t[iLength];
	 
	 for (i=0;i < iLength; ++i){
		 //al array
		 ret = m_oDatesStack.GetElement((m4uint16_t) i , ao_pdSlicesDates[iLength-i-1] );
		 DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Error reading from slices stack in DetSlicesOfItem.")	;
	 }
	 
	 aio_iNumSlices=iLength;
	 
	 return ret;
 }
 
 //---------------------------------------------
 //Transacciones
 //---------------------------------------------
 m4return_t  ClVMachine1::RecoverFromError(ClAccess *ai_poAccess)
 {
	 m4uint32_t			iStatePos;
	 m4uint16_t			iNumRefArgs;
	 m4int32_t			iNumPosToDelete;
	 m4bool_t			bPushResult;
	 ClLConn_Interface	*oConn;
	 m4return_t			FinalRet=M4_SUCCESS;
	 
	 iStatePos=m_oStateStack->GetPos() ;

#if _M4_DEBUG > 10    
	 DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Recover from error. Begin Transaction = " <<LogCat<< m_BeginTransaction);
#endif
	 
	 m4VMState_t oLastState, oCurrentState, oStateTemp;
	 m4return_t ret;
	 m4int32_t  i;
	 m4int32_t  iBeginPointToUse=max (m_BeginTransaction, m_iReenter); 
				//usamos esto, porque como es reentrante si hay un error debemos borrar como máximo hasta
				//donde empezo la ejecución (que no tiene porque se el estado 0,es decir -1) sino el reenter
										


	 /* Bug 103177
	 Ponemos las transacciones al principio para no tenerlas en 2 sitios
	 */
	 //obtenemos conexion, sin crear ninguna si no estaba
	 oConn=m_oImplLDB.GetConnectEnv( ai_poAccess,M4_FALSE );
	 
	 // si ha y conexión de BBDD abierta deberiamos cerrarla
	 if (oConn!=0){
		 m4bool_t bFree = M4_FALSE;
		 do {
			 oConn->EndTransaction(NULL, M4_FALSE, bFree);
		 } while (!bFree);
	 }
	 

	 m4int32_t  ActualPos=m_oStateStack->GetPos();
	 if (ActualPos <= iBeginPointToUse){ //si no hemos cerrado la transacción después del
		 //nivel en que se abrió la cerramos a pedal
		 m_BeginTransaction= M4CL_NO_VM_TRANSACTION_DEFINED;
		 iBeginPointToUse=m_iReenter; 
	 }
	 
	 if (iBeginPointToUse==m_iReenter) { //estamos borrando hasta la reentrada
		FinalRet=M4_ERROR; //tenemos que indicar que no nos hemos recuperado
	 }
	 
	 if (iBeginPointToUse==M4CL_NO_VM_TRANSACTION_DEFINED || iBeginPointToUse== -1 ){ //no hay definida transaccion || definida antes del primer metodo-> borramos todo			                
		 
		 m_BeginTransaction= M4CL_NO_VM_TRANSACTION_DEFINED;	 
		 
		 iNumPosToDelete=m_oRunTimeStack->GetPos()+1;
		 for (i=0; i<iNumPosToDelete;i++){
			 RuntimePopAndDelete();
		 }
		 iNumPosToDelete=m_oStateStack->GetPos()+1;
		 for (i=0; i<iNumPosToDelete;i++){
			 ret=m_oStateStack->Top(oStateTemp);
			 DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<<"Error performing rollback.")	;
			 if ( oStateTemp.m_poFormula.IsNotNull() ) {
		 		if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
					GetVMCSContext()->GetSttWrap()->GetStatistics()->sttStateEnd(oStateTemp.m_hItem);

				 //solo no borramos cuando es jit  y preparada
				 if ( !M4CL_HAS_STATE_PREP_JIT_FLAG(oStateTemp.m_Flags) && M4CL_HAS_STATE_EXE_JIT_FLAG(oStateTemp.m_Flags)){ //si es jit, además borramos el código instalado en sí
					 delete [] (m4char_t*)(oStateTemp.m_poFormula.GetFormulaHandle());
				 }
			 }
			 
			 ret=m_oStateStack->Pop();
			 DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Error performing rollback.")	;
		 }

		 //como resultado metemos error
		 RuntimePushDouble (M4_ERROR);
		 m_oRunTimeStack->SetReference(0); //ponemos a cero la referencia de la pila
		 return M4_ERROR;
	 }



	 /* Bug 0089081
	 Primero de todo se purga la pila de estados.
	 En esta pasada nos quedamos con el último estado que no sea un moveTo
	 de propagación (item=-1)
	 Esta claro que al menos tiene que haber uno, que es en el que estamos
	 */
	 iNumPosToDelete = m_oStateStack->GetPos() - iBeginPointToUse ;

	 /* Bug 103177
	 Si no hay nada que borrar nos salimos
	 */
	 if( iNumPosToDelete <= 0 )
	 {
		 return( FinalRet ) ;
	 }

	 m4int32_t iOldReference=m_oRunTimeStack->GetReference();

	 for (i=0; i < iNumPosToDelete; i++){
		 ret=m_oStateStack->Top(oStateTemp);
		 DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Error performing rollback.")	;
		 if (oStateTemp.m_poFormula.IsNotNull () ) {
	 		if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
				GetVMCSContext()->GetSttWrap()->GetStatistics()->sttStateEnd(oStateTemp.m_hItem);

			 //solo no borramos cuando es jit  y preparada
			 if ( !M4CL_HAS_STATE_PREP_JIT_FLAG(oStateTemp.m_Flags) && M4CL_HAS_STATE_EXE_JIT_FLAG(oStateTemp.m_Flags)){ //si es jit, además borramos el código instalado en sí
				 delete [] (m4char_t*)(oStateTemp.m_poFormula.GetFormulaHandle());
			 }

		 }
		 ret=m_oStateStack->Pop();
		 DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Error performing rollback.")	;

		 if( oStateTemp.m_hItem != -1 )
		 {
			 oLastState = oStateTemp ;

			 iOldReference=m_oRunTimeStack->GetReference();
			 m_oRunTimeStack->SetReference(oStateTemp.m_iOldStackReference); //reactulizamos la referencia de la pila
		 }
	 }
	

	 //Los JIT no pueden tener arg por ref., y lo ponemos a capon porque si le
	 //preguntamos al mcr nos va a decir si el item padre (no el JIT) tiene o no args por referencia, y puede decir que sí cuando es mentira
	 if (M4CL_HAS_STATE_EXE_JIT_FLAG(oLastState.m_Flags) )
		 iNumRefArgs=0;
	 else{
		 iNumRefArgs=oLastState.m_poAccess->GetpCMCR()->GetItemNumberOfReferedArguments(oLastState.m_hItem);
	 }
	 
	 
	 ret=m_oStateStack->GetElement(oCurrentState, iBeginPointToUse);
	 M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Error getting rollback state.")	;
	 
	 //Bug:61605
	 // Tenemos que añadir M4CL_HAS_STATE_INIT_FLAG porque en el caso de depuración, tendremos ya
	 // construido el espacio de variables locales a pesar de que la instrucción current sea 0.
	 // Este flag solo se activa en el server en el caso de que estemos en depuración.
	 // En el cliente y en la parte back del canal, en el método tenemos las variables locales por la deserialización
	 if (oCurrentState.m_iCurrent>0  || M4CL_HAS_STATE_INIT_FLAG(oCurrentState.m_Flags)){ //habra que salvar las variables, que ya estarán creadas
		 //determinamos el número de variables para guardarlas        
		 
		 m4uint32_t iVariable;
		 m4uint32_t iNumArgs;
		 m4VariantType vNumArgs;

		 if (M4CL_HAS_STATE_EXE_JIT_FLAG(oLastState.m_Flags) ){ //si es JIT x guevos es de arg_var. No le preguntamos al mcr porque no diria si el padre es de arg. var o no, no el, que sí lo es
			 iVariable=1;
		 }
		 else{ //si es un item normal, le preguntamos al mcr
			 iVariable= oLastState.m_poAccess->GetpCMCR()->GetItemVariableArguments ( oLastState.m_hItem);
		 }

		 if (iVariable != 0){
			 //Bug:61605  Begin   -------------------------------------
			 // En el caso de tener un método con argumentos variables, se estaba utilizando una referencia incorrecta
			 m4int32_t iTempReference=m_oRunTimeStack->GetReference();
			 m_oRunTimeStack->SetReference(iOldReference);
			 m_oRunTimeStack->GetElement(vNumArgs, -1);
			 m_oRunTimeStack->SetReference(iTempReference);
			 iNumArgs=(m4uint32_t)vNumArgs.Data.DoubleData + 1;
			 //Bug:61605  End     -------------------------------------
		 }
		 else{
			 iNumArgs = oLastState.m_poAccess->GetpCMCR()->GetItemNumberOfArguments (oLastState.m_hItem);
		 }
			 
		 iNumPosToDelete=m_oRunTimeStack->GetPos()+1-iOldReference+iNumArgs;
		 bPushResult=M4_TRUE;
	 }
	 else{ //aun no se han creado variables
		 
		 iNumPosToDelete=m_oRunTimeStack->GetPos()  - oLastState.m_iOldStackReference +1 ;
		 bPushResult=M4_FALSE;
	 }

	 //sacamos de la RTS lo que sea necesario
	 for (i=0; i < iNumPosToDelete; i++){
		 RuntimePopAndDelete();
	 }
	 
	 
	 //metemos en la pila de ejecución tantos valores como argumentos por
	 //referencia tenga que retornar y el resultado
	 if (bPushResult){
		 for (i=0 ; i < iNumRefArgs +1 ; i++){
			 ret=RuntimePushDouble(M4_ERROR);
			 DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_PUSH, ERRORLOG, "Error performing push to stack.")	;
			 
		 }
	 }

	 return FinalRet; //si hemos cepillado hasta la reentrada->error, sino es que hemos sido capaces de recuperarnos
}



m4return_t ClVMachine1::__SetValue (ClItem &ai_item, m4VariantType &ai_var, priority_t ai_prior, m4uint8_t ai_iSource)
{
	m4return_t	ret;
    m4bool_t bret;
    //My very, very private SetValue function-----
    
    if (ai_var.Type==M4CL_CPP_TYPE_STRING_POOL){

        m4VariantType copy;
		copy.IsUser=ai_var.IsUser;
        copy.Type=M4CL_CPP_TYPE_STRING_VAR;
        copy.Data.PointerChar=m_oDynamicPool->GetString (ai_var.Data.CharData);

		ret = ai_item.Value.Set (copy, ai_prior, bret);

		if ( (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_VALUES) && (ret != M4_ERROR) )
		{
			GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddValue( ai_item, copy, ai_prior, bret, ai_iSource, m_oState1, DumpCallStack( &m_oState1, M4_TRUE ).c_str() );
		}

        return ret;
    }
    else{
		ret = ai_item.Value.Set (ai_var, ai_prior, bret);

		if ( (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_VALUES) && (ret != M4_ERROR) )
		{
			GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddValue( ai_item, ai_var, ai_prior, bret, ai_iSource, m_oState1, DumpCallStack( &m_oState1, M4_TRUE ).c_str() );
		}

        return ret;
    }
}


m4return_t  ClVMachine1::DeleteNullMethod (ClAccess *ai_poAccess,m4uint32_t ai_hItem, m4bool_t ai_bPurge, m4bool_t ai_bPurgeRef, m4bool_t ai_bCompact, m4date_t ai_StartDate, m4date_t ai_EndDate)
{
    m4uint16_t i;
    m4return_t  ret;
    m4VariantType vTempVariant;
    //sacamos los parametros quye tenga
    m4uint16_t  iNumArgs;
	m4uint32_t hNodo;
	ClItem *poItem;
	m4VariantType vDefault;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Delete null method. Item= " <<LogCat<< ai_poAccess->GetpCMCR()->GetItemId(ai_hItem) );
#endif  

    m4uint8_t  iVarArgs = ai_poAccess->GetpCMCR()->GetItemVariableArguments ( ai_hItem);
	
    //determinamos el número de arguemtos
    if ( iVarArgs != 0 ) {
        RuntimePop(vTempVariant);
        iNumArgs	 = int(	vTempVariant.Data.DoubleData );
    }
    else{
        iNumArgs	=ai_poAccess->GetpCMCR()->GetItemNumberOfArguments (ai_hItem);
    }
	
    //sacamos argumentos
    for (i=0; i< iNumArgs; i++){
        ret=RuntimePopAndDelete();
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__ <<__LINE__<<"Error deleting null rule. Can't retrieve arguments")	;
    }
	
    //metemos los argumentos por referencia que necesite
    m4uint16_t  iNumArgsRef	=ai_poAccess->GetpCMCR()->GetItemNumberOfReferedArguments (ai_hItem);
    
	//si no hay que purgar metemos un valor como resultado
	//si es un concepto, aunque haya que purgar entramos, para crear el tramso entre las fechas que estamos ejecutandonos
	//aunque el tramos quede al valor por defecto
    if (!ai_bPurge || (GetRunSliceMode(ai_poAccess) && ai_poAccess->GetpCMCR()->GetItemType(ai_hItem) == M4CL_ITEM_TYPE_CONCEPT ) ){ 
		
		ClCompiledMCR   *poCMCR=ai_poAccess->GetpCMCR();
		ClItemIndex iItem = poCMCR->GetItemIndex (ai_hItem);

        if (poCMCR->GetItemType(ai_hItem) == M4CL_ITEM_TYPE_CONCEPT ){
			//retornamos el valor que tenga el concepto
			hNodo=poCMCR->GetItemNodeHandle (ai_hItem);
			ClNode *poNode=ai_poAccess->Node.Get(ClHandle(hNodo));
			if (!poNode->RecordSet.IsValid() && poCMCR->GetItemScope(ai_hItem) != M4CL_ITEM_SCOPE_NODE){
				DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK_READING_IN_NULL_EXEC, M4ObjNodeItemI(poCMCR,ai_hItem)<<DumpTreePath(m_oState1.m_poAccess, hNodo) << DumpCallStack() );
				return M4_ERROR;
			}
			//el nodo ya no lo movemos porque está movido
			if (poNode->RecordSet.Current.Index == M4DM_EOF_INDEX && poCMCR->GetItemScope(ai_hItem) == M4CL_ITEM_SCOPE_REGISTER ){
				DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_NUMBER_WRITE,M4ObjNodeItemI(poCMCR,ai_hItem)<<M4ObjNodeItemI(poCMCR,ai_hItem)<<DumpTreePath(ai_poAccess, poNode->RecordSet.GetpNodeDef()->GetHandle())  << DumpCallStack() );
				return M4_ERROR;
			}
			
			poItem=&(poNode->RecordSet.Current.Item[iItem]) ;
			
			if ( GetRunSliceMode(ai_poAccess) ){ //segun el mode slice on/off

				ret=CheckAndCreateItemSlice (ai_StartDate, ai_EndDate ,poItem); //creamos o nos movemos al tramo
				M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_SET_CONCEPT_SLICE_VALUE, ERRORLOG, M4ObjNodeItemI(poCMCR,ai_hItem)<<DateLog(ai_StartDate)<<DateLog(ai_EndDate)<<M4ObjNodeItemI(poCMCR,ai_hItem)  << DumpCallStack() );

				ret=poItem->Value.Get(ai_StartDate, ai_EndDate, vDefault);

				/* Bug 0077475
				Si hay que compactar se compacta.
				Como no tiene fórmula, no se compactan los asignados.
				*/
                if (ai_bCompact){ 
                    poItem->Value.Compact();
                }
			}
			else{
				ret=poItem->Value.Get(vDefault);

			}
		}
		else{ //retornamos el valor por defeto
			ai_poAccess->GetpCMCR()->GetItemDefaultValue (ai_hItem, vDefault);
		}


		if (!ai_bPurge){ //por si solo ha entrado por aqui apra crear el tramo de un concepto
			if (vDefault.Type==M4CL_CPP_TYPE_STRING_VAR){
				RuntimePushString (vDefault.Data.PointerChar, vDefault.IsUser);
			}
			else{
				RuntimePush (vDefault);
			}
		}
    }

	if (!ai_bPurgeRef){
		for (i=0; i< iNumArgsRef; i++){
			ret=RuntimePushDouble(M4_ERROR); //los argumentos por referencia-> valores de error
			DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<<"Error deleting null rule. Can't push reference arguments")	;
		}
    }

    return M4_SUCCESS;
}



//---------------------------------------------------
//CHECK & CREATE ITEM SLICE 
//Comprobación de que un tramo existe, y sino crearlo
//y si el item es de tipo no_lineal dando warning
//---------------------------------------------------
m4return_t ClVMachine1::CheckAndCreateItemSlice (m4date_t ai_dStart, m4date_t ai_dEnd, ClItem *ai_poItem){
	
	m4char_t date1[24], date2[24];
	
    //buscamos a ver si el tramo existe exactamente
    if (ai_poItem->Value.CheckSlice(ai_dStart,ai_dEnd)){

		ai_poItem->Value.MoveTo (ai_dStart); //nos movemos porque el checkslice no nos posiciona
			//si existe salimos correctamente com información de depurado de posicioanmiento correcto

			if (ClVMDebugEnv::getInstance()->getDebugLevel() >= 3)
			{
				ClDateToString (ai_dStart, date1, 23);
				ClDateToString (ai_dEnd  , date2, 23);
				m4uint32_t		iItemHandle = ai_poItem->ItemDef.GetHandle() ;
				ClCompiledMCR	*pMCRItem = ai_poItem->ItemDef.GetpCMCR() ;
				ClCompiledMCR	*pMCRParent = m_oState1.m_poAccess->GetpCMCR() ;
				VMTracer::GetInstance(ai_poItem->GetpAccess()->GetEnvironment())
					<< beginthread
					<< "Moving to slice {" << date1 << "} {" << date2
					<< "} to assign value of item <"
					<< pMCRItem->iM4Obj() << "!" << pMCRItem->iNodeI( iItemHandle ) << "." << pMCRItem->iItemI( iItemHandle )
					<< "> from item <"
					<< pMCRParent->iM4Obj() << "!" << pMCRParent->iNodeI( m_oState1.m_hItem ) << "." << pMCRParent->iItemI( m_oState1.m_hItem ) << "(" << m_oState1.m_iSourceLine << ")"
					<< ">" << endthread ;
			}

            return M4_SUCCESS;
    }

	
    //si no existe comprobamos el tipo del item
    
	//lineal    ->Crear + info
	//no Lineal ->Crear + info + warning
	//no slice  -> ????
#ifdef _DEBUG
    if (ai_poItem->ItemDef.SliceSplit() != M4CL_ITEM_SLICE_SPLIT_LINEAR) {
		DUMP_CHLOG_DEBUGF(M4_CH_VM_SLICES_NON_LINEAR, M4ObjNodeItemI(ai_poItem->GetpAccess()->GetpCMCR(),ai_poItem->ItemDef.GetHandle()) );
		
    }
#endif
    //creamos tramo


	/* Bug 0076620
	No se puede crear el tramo alegremente, porque si intersecta con otros
	machaca su valor y les pone prioridad y valor por defecto.
	Lo que hay que hacer es que vaya creando subtramos que no intersecten,
	porque en esos si que se conserva la prioridad y se prorratea el valor.
	*/
	m4uint16_t		i ;
	m4uint16_t		iLength ;
	m4date_t		dStartDate ;
	m4date_t		dEndDate ;
	m4VariantType	vUnitDate ;


	// Se limitan los tramos al periodo que nos viene
	m_oDatesStack.Clear( ai_dStart, ai_dEnd ) ;

	// Se añaden los que tiene el item
	ai_poItem->Value.FillDateStack( &m_oDatesStack ) ;

	// Compactamos las fechas
	m_oDatesStack.Reduce() ;

	iLength = m_oDatesStack.GetLength() ;

	// Se añade el final
	ai_poItem->GetpAccess()->GetpChannel()->UnitDate.Get( vUnitDate ) ;
	m_oDatesStack.AddEndDate( vUnitDate.Data.DoubleData ) ;

	// Se itera por los subtramos
	for( i = 0 ; i < iLength ; i++ )
	{
		m_oDatesStack.GetElement( i, dStartDate ) ;
		m_oDatesStack.GetElement( i + 1, dEndDate ) ;
		dEndDate -= vUnitDate.Data.DoubleData ;

		m4return_t ret = ai_poItem->Value.AddSlice (dStartDate, dEndDate);
		if ( ret == M4_ERROR){
			DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_ADDING_SLICE_TO_ASSIGN, M4ObjNodeItemI(ai_poItem->GetpAccess()->GetpCMCR(),ai_poItem->ItemDef.GetHandle() )<<BeginLogCat<<DateLog(dStartDate)<<"-"<<DateLog(dEndDate)<<EndLogCat << DumpCallStack() );
			return M4_ERROR;
		}
	}
			
			
	ai_poItem->Value.MoveTo(ai_dStart);
    //info

	if (ClVMDebugEnv::getInstance()->getDebugLevel() >= 3)
	{
		ClDateToString (ai_dStart, date1, 23);
		ClDateToString (ai_dEnd  , date2, 23);
		m4uint32_t		iItemHandle = ai_poItem->ItemDef.GetHandle() ;
		ClCompiledMCR	*pMCRItem = ai_poItem->ItemDef.GetpCMCR() ;
		ClCompiledMCR	*pMCRParent = m_oState1.m_poAccess->GetpCMCR() ;
		VMTracer::GetInstance(ai_poItem->GetpAccess()->GetEnvironment())
			<< beginthread
			<< "Creating slice {" << date1 << "} {" << date2
			<< "} to assign value of item <"
			<< pMCRItem->iM4Obj() << "!" << pMCRItem->iNodeI( iItemHandle ) << "." << pMCRItem->iItemI( iItemHandle )
			<< "> from item <"
			<< pMCRParent->iM4Obj() << "!" << pMCRParent->iNodeI( m_oState1.m_hItem ) << "." << pMCRParent->iItemI( m_oState1.m_hItem ) << "(" << m_oState1.m_iSourceLine << ")"
			<< ">" << endthread ;
	}

	return M4_SUCCESS;
	
}


/* Bug 0076620
Esta función va asignando un valor en un conjunto de subtramos dados por dos
fechas, si la prioridad es buena.
Se supone que los tramos están muy bien creados.
*/
m4return_t	ClVMachine1::SetValue( ClItem &ai_roItem, m4VariantType &ai_vValue, priority_t ai_iPriority, m4uint8_t ai_iSource, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4bool_t ai_bCompact )
{

	m4return_t		iResult ;
	m4bool_t		bIsNotEof ;
	priority_t		iPriority ;
	ClSliceIndex	iIndex ;
	m4date_t		dStartDate ;
	m4date_t		dEndDate ;
	m4date_t		adDates[ 4 ] ;
	m4VariantType	vValue ;


	// Nos guardamos el índice viejo
	iIndex = ai_roItem.Value.Index ;

	// Recorremos los tramos asignando
	bIsNotEof = ai_roItem.Value.MoveTo( ai_dStartDate ) ;

	while( bIsNotEof == M4_TRUE )
	{
		dStartDate = ai_roItem.Value.GetStartDate() ;
		dEndDate = ai_roItem.Value.GetEndDate() ;

		if( dStartDate < ai_dStartDate || dEndDate > ai_dEndDate )
		{
			// Si no estamos dentro de las fechas nos salimos.
			// Que si, que lo de la fecha inicio no hacía falta
			break ;
		}

		iPriority = ai_roItem.Value.GetPriority() ;

		if( ai_iPriority != -1 && ai_iPriority > iPriority )
		{
			// Si se pasa prioridad y la prioridad es menor no se asigna
			/* Bug 0092141
			No se puede hacer un break, hay que continuar
			*/
			bIsNotEof = ai_roItem.Value.Next() ;
			continue;
		}


		// Pequeña optimización. Si las fechas son iguales no se prorratea, claro
		if( ai_dStartDate == dStartDate && ai_dEndDate == dEndDate )
		{
			vValue = ai_vValue ;
		}
		else
		{
			// Se prorratea el valor
			adDates[ 0 ] = ai_dStartDate ;
			adDates[ 1 ] = ai_dEndDate ;
			adDates[ 2 ] = dStartDate ;
			adDates[ 3 ] = dEndDate ;

			ai_roItem.Value._GetWithDates( adDates, ai_vValue, vValue, M4_TRUE ) ;
		}


		// Se asigna
		iResult = __SetValue( ai_roItem, vValue, ai_iPriority, ai_iSource ) ;

		if( iResult != M4_SUCCESS )
		{
			ai_roItem.Value.MoveTo( iIndex ) ;
			return( M4_ERROR ) ;
		}

		/* Bug 0100954
		Se saca la traza de asignación de tramo
		*/
		if( ClVMDebugEnv::getInstance()->getDebugLevel() >= 3 )
		{
			m4char_t	date1[ 24 ] ;
			m4char_t	date2[ 24 ] ;
			m4char_t	acBuffer[ 128 + 1 ] ;

			m4pcchar_t		pccValue = NULL ;
			m4uint32_t		iItemHandle = ai_roItem.ItemDef.GetHandle() ;
			ClCompiledMCR	*pMCRItem = ai_roItem.ItemDef.GetpCMCR() ;
			ClCompiledMCR	*pMCRParent = m_oState1.m_poAccess->GetpCMCR() ;

			ClDateToString( dStartDate, date1, 23 ) ;
			ClDateToString( dEndDate  , date2, 23 ) ;

			/* Bug 0166212
			Se hace la conversión sin utilizar el m_oVariantMediator para no liberar cosas del pool
			*/
			switch( vValue.Type )
			{
				case M4CL_CPP_TYPE_STRING_VAR:
				case M4CL_CPP_TYPE_STRING_SYS:
					pccValue = vValue.Data.PointerChar ;
					break;

				case M4CL_CPP_TYPE_STRING_POOL:
					pccValue = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
					break;

				case M4CL_CPP_TYPE_DATE:
					pccValue = acBuffer ;
					ClDateToString( vValue.Data.DoubleData, acBuffer, 23 ) ;
					break;

				case M4CL_CPP_TYPE_NUMBER:
					pccValue = acBuffer ;
					sprintf( acBuffer, "%.8lf", vValue.Data.DoubleData ) ;
					break;

				default: 
					pccValue = "" ;
					break;
			}

			VMTracer::GetInstance( ai_roItem.GetpAccess()->GetEnvironment() )
				<< beginthread
				<< "Assigning <" << pccValue << "> to slice {" << date1 << "} {" << date2
				<< "} of item <"
				<< pMCRItem->iM4Obj() << "!" << pMCRItem->iNodeI( iItemHandle ) << "." << pMCRItem->iItemI( iItemHandle )
				<< "> from item <"
				<< pMCRParent->iM4Obj() << "!" << pMCRParent->iNodeI( m_oState1.m_hItem ) << "." << pMCRParent->iItemI( m_oState1.m_hItem ) << "(" << m_oState1.m_iSourceLine << ")"
				<< ">" << endthread ;
		}


		bIsNotEof = ai_roItem.Value.Next() ;
	}

	// Recuperamos la posición para dejar todo igual
	ai_roItem.Value.MoveTo( iIndex ) ;


	/* Bug 0085200
	Hay que compactar al final de cada asignación
	*/
	if( ai_bCompact == M4_TRUE )
	{
		ai_roItem.Value.Compact() ;
	}

	return( M4_SUCCESS ) ;
}


/* Bug 0076620
Esta función va asignando una prioridad en un conjunto de subtramos dados por dos fechas.
Se supone que los tramos están muy bien creados.
*/
m4return_t ClVMachine1::SetPriority(  ClItem &ai_roItem, priority_t ai_iPriority, m4date_t ai_dStartDate, m4date_t ai_dEndDate )
{

	m4return_t		iResult ;
	m4bool_t		bIsNotEof ;
	ClSliceIndex	iIndex ;
	m4date_t		dStartDate ;
	m4date_t		dEndDate ;


	// Nos guardamos el índice viejo
	iIndex = ai_roItem.Value.Index ;

	// Recorremos los tramos asignando
	bIsNotEof = ai_roItem.Value.MoveTo( ai_dStartDate ) ;

	while( bIsNotEof == M4_TRUE )
	{
		dStartDate = ai_roItem.Value.GetStartDate() ;
		dEndDate = ai_roItem.Value.GetEndDate() ;

		if( dStartDate < ai_dStartDate || dEndDate > ai_dEndDate )
		{
			// Si no estamos dentro de las fechas nos salimos
			// Que si, que lo de la fecha inicio no hacía falta
			ai_roItem.Value.MoveTo( iIndex ) ;
			return( M4_SUCCESS ) ;
		}

		iResult	= ai_roItem.Value.SetPriority( ai_iPriority ) ;

		if( iResult != M4_SUCCESS )
		{
			ai_roItem.Value.MoveTo( iIndex ) ;
			return( M4_ERROR ) ;
		}

		bIsNotEof = ai_roItem.Value.Next() ;
	}

	// Recuperamos la posición para dejar todo igual
	ai_roItem.Value.MoveTo( iIndex ) ;

	return( M4_SUCCESS ) ;
}


m4return_t ClVMachine1::GetL2Access ( ClChannel * ai_pChannel, m4pcchar_t ai_ChannelInstance, m4int16_t ai_bJIT,ClAccess** ao_pAccess )
{
	return ai_pChannel->Level2.GetL2Access (ai_ChannelInstance, ao_pAccess, ai_bJIT ? M4_TRUE : M4_FALSE  );
}

m4return_t ClVMachine1::GetParentItemState(m4VMState_t &ao_oState)
{
	if (m_oState1.m_iParentStatePos == -1)
		return M4_ERROR;
	
	m_oStateStack->GetElement(ao_oState, m_oState1.m_iParentStatePos);
	
	return M4_SUCCESS;
}

// User actions auditory.
m4return_t ClVMachine1::RegisterUserAuditMethodEvent( m4VMState_t ai_oState, m4uint32_t ai_iCallStackPos, m4pchar_t ai_pcOrganization )
{
	ClCompiledMCR*	pCMCR ;
	m4uint32_t		hNode ;
	ClUserAction	*poAction = NULL ;
	m4uint32_t		lNumTotalArgs ;
	m4uint16_t		iNumFixedArgs ;
	m4VariantType	vValue ;
	m4pcchar_t		pccId ;
	m4pcchar_t		pccName ;
	m4pcchar_t		pccArgName ;
	m4pchar_t		pcRole ;

	pCMCR = ai_oState.m_poAccess->GetpCMCR() ;

	if( pCMCR->GetChannelType() > 100 )
	{
		// Los canales del sistema no se deben auditar
		return( M4_SUCCESS ) ;
	}

	poAction = new ClUserAction() ;
	poAction->SetOrganization( ai_pcOrganization ) ;

	ai_oState.m_poAccess->GetpChannel()->Role_ID.Get( pcRole );
	poAction->SetRole ( pcRole ) ;

	poAction->SetEventID( ClUserAction::EV_METHOD ) ;

	hNode = pCMCR->GetItemNodeHandle( ai_oState.m_hItem ) ;

	pccId = pCMCR->GetChannelId() ;
	pccName = pCMCR->GetChannelName() ;
	
	if( pccName == NULL || *pccName == '\0' )
	{
		pccName = pccId;
	}

	poAction->SetEventObject( pccId ) ;
	poAction->SetEventObjectName( pccName ) ;

	poAction->AppendEventObject( "!" ) ;
	poAction->AppendEventObjectName( "!" ) ;

	pccId = pCMCR->GetNodeId( hNode ) ;
	pccName = pCMCR->GetNodeName( hNode ) ;
	
	if( pccName == NULL || *pccName == '\0' )
	{
		pccName = pccId;
	}

	poAction->AppendEventObject( pccId ) ;
	poAction->AppendEventObjectName( pccName ) ;
	
	poAction->AppendEventObject( "." ) ;
	poAction->AppendEventObjectName( "." ) ;

	pccId = pCMCR->GetItemId( ai_oState.m_hItem ) ;
	pccName = pCMCR->GetItemName( ai_oState.m_hItem ) ;
	
	if( pccName == NULL || *pccName == '\0' )
	{
		pccName = pccId;
	}

	poAction->AppendEventObject( pccId ) ;
	poAction->AppendEventObjectName( pccName ) ;

	lNumTotalArgs = GetNumberOfArguments( ai_iCallStackPos ) ;
	iNumFixedArgs = pCMCR->GetItemNumberOfArguments( ai_oState.m_hItem ) ;

	poAction->SetEventArgs( "" ) ;

	for( m4uint8_t iIdx = 0; iIdx < lNumTotalArgs; iIdx++ )
	{
		GetArgumentValue( ai_iCallStackPos, iIdx, vValue ) ;
		
		m_oVariantMediator->ToCharString( vValue ) ;

		if( vValue.Type == M4CL_CPP_TYPE_STRING_VAR )
		{
			if( iIdx > 0 )
			{
				poAction->AppendEventArgs( " | " ) ;
			}

			if( iIdx < iNumFixedArgs )
			{
				pccArgName = pCMCR->GetItemArgumentIdByPosition ( ai_oState.m_hItem, iIdx ) ;
				poAction->AppendEventArgs( pccArgName ) ;
			}
			else
			{
				poAction->AppendEventArgs( "VariableArgument" ) ;
			}
			poAction->AppendEventArgs( " = " ) ;
			poAction->AppendEventArgs( vValue.Data.PointerChar ) ;
		}
	}

	ai_oState.m_poAccess->GetpChannel()->GetpChannelManager()->UserActionsAudit.AddAction( poAction ) ;

	return M4_SUCCESS ;
}


m4return_t ClVMachine1::CreateRemoteCall(void)
{

	m4return_t			ret;
	m4int32_t			iOldPosition;
	m4int32_t			iNewPosition;
	m4VMState_t			oTempState;
	m4pchar_t			pcOrganization ;
	ClChannelManager*	poChannelManager ;
	m4int32_t			lNumStates ;
	m4VMState_t			oState ;
	
	m_oState1.m_poAccess->GetpChannel()->Organization.Get( pcOrganization );
	poChannelManager = m_oState1.m_poAccess->GetpChannel()->GetpChannelManager() ;

	if( poChannelManager != NULL && poChannelManager->UserActionsAudit.HasAuditory( ClUserAction::EV_METHOD, pcOrganization ) == M4_TRUE )
	{
		lNumStates = m_oStateStack->GetPos() ;

		// -- Hay métodos previos al mapped en la pila.
		if( lNumStates != 0 )
		{
			// -- Antes de nada, registramos el primer método de la pila.
			m_oStateStack->GetElement( oState, 0 ) ;

			if( oState.m_hItem != -1 )
			{
				RegisterUserAuditMethodEvent( oState, lNumStates, pcOrganization ) ;
			}
			else
			{
				// -- Conectores. Buscamos el método origen.
				for( m4int32_t lIndex = 1; lIndex <= lNumStates; lIndex++ )
				{
					m_oStateStack->GetElement( oState, lIndex ) ;
					
					if( oState.m_hItem != -1 && oState.m_hItem != m_oState1.m_hItem )
					{
						RegisterUserAuditMethodEvent( oState, lNumStates - lIndex, pcOrganization ) ;
						break ;
					}
				}
			}
		}
	}


	// Marcar el estado como que ya se han ejecutado los insite y cambiar el que hay en la pila

	m_oStateStack->Top( oTempState ) ;
	M4CL_SET_STATE_NO_INSITE_FLAG( oTempState.m_Flags ) ;
	m_oStateStack->Pop() ;
	m_oStateStack->Push( oTempState ) ;
	iOldPosition = m_oStateStack->GetPos() ;

	ret=GetVMCSContext()->RemoteCall( (ClExecutor*)m_oState1.m_poAccess->GetpExecutor(), m_oState1.m_hItem, m_oState1.m_poRecordSet, m_oStepper.GetActive() );

	iNewPosition = m_oStateStack->GetPos() ;
	if( iNewPosition == iOldPosition )
	{
		// Se vuelve a poner el estado como estaba
		m_oStateStack->Top( oTempState ) ;
		M4CL_RESET_STATE_NO_INSITE_FLAG( oTempState.m_Flags ) ;
		m_oStateStack->Pop() ;
		m_oStateStack->Push( oTempState ) ;
	}

	return ret;
}
 

//---------------------------------------------------------------
//Añadir método a la pila de ejecución
//---------------------------------------------------------------

m4return_t ClVMachine1::AddMethodToExecute (m4VMState_t &ai_oState, m4VariantType ai_vSlice, m4int32_t ai_iParentStatePos, m4bool_t ai_bSpecSlice)
{
	m4pcvoid_t	hFormula;
	m4return_t	ret = M4_SUCCESS;
	m4VariantType	vCsType;
	m4uint16_t	iSlicePosition;
	ClCompiledMCR* const poCMCR = ai_oState.m_poAccess->GetpCMCR() ;


	// Si estamos en cliente y se trata de un canal front, del cual estamos depurando su parte back, no dejamos ejecutar.
	// jesus para la vista de debug
	const ClChannel::eDebugMode_t eChannelDebugMode = ai_oState.m_poAccess->GetpChannel()->GetDebugMode();
	const m4bool_t bIsClient = m_poContext->IsClient();
	if (bIsClient == M4_TRUE && eChannelDebugMode == ClChannel::IS_DEBUG_FIRST_TIME) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_EXECUTE_PERM_DEBUG, M4ObjNodeItemI(poCMCR,ai_oState.m_hItem) << DumpCallStack());
		return M4_ERROR;
	}
	
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Create execution of method " <<LogCat<< poCMCR->iItemI(ai_oState.m_hItem) <<LogCat<< ",RunDate = " <<LogCat<< DateLog(ai_oState.m_dDate) <<LogCat<< "; StartDate=" <<LogCat<< DateLog (ai_oState.m_dBeginDate) <<LogCat<< "; EndDate = " <<LogCat<< DateLog (ai_oState.m_dEndDate) );
#endif
	//guardamos en el estado la referencia vieja de la pila 
	//ai_oState.m_iOldStackReference=m_oRunTimeStack->GetReference(); 
	ai_oState.m_iOldStackReference = min(m_oRunTimeStack->GetReference(), m_oRunTimeStack->GetPos()+1);  //el min es porque: por si por reentrada se han quitado args.
	//de la pila cuando nos llaman desde dentro de VM, puede ser desde un método que ha quitado args. y por tanto la referencia y el tope de la 
	//pila no sean consistentes (por ejemplo desde LookUp) . Mirando el mínimo se fuerza la consistencia.
	
	
	if (poCMCR->GetItemType(ai_oState.m_hItem) == M4CL_ITEM_TYPE_CONCEPT ){
		M4CL_SET_STATE_CONCEPT_FLAG (ai_oState.m_Flags); //ponemos flag de concepto
	}
	
	
	//miramos si tiene persmiso
	if (poCMCR->GetItemSecExecute(ai_oState.m_hItem) != 1){//->Si no tiene permisos->error o warning 
		
		m4pcchar_t	pccItem ;
		m4uint32_t	iError ;
		
		pccItem = poCMCR->GetItemId( ai_oState.m_hItem ) ;
		iError = M4GetDBOperation( pccItem ) ;
		
		if ( poCMCR->GetItemSecContinue(ai_oState.m_hItem) == 0 ){ //no continua la ejecución->Cortamos cascada
			
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_EXECUTE_PERM, M4ObjNodeItemI(poCMCR,ai_oState.m_hItem) << DumpCallStack() );
			
			if( iError != 0 ){
				DUMP_CHLOG_ERRORF( M4_CH_VM_NO_LOAD_PERM + iError - 1, DumpCallStack() );
			}
			
			return( M4_ERROR ) ;
		}
		else{  //dejamos que continue, pero con ejecución nula
			
			hFormula=0; //ejecución vacia
			
			//sacamos warnings, si es que no los hay ya
			if( M4ChLogIsErrorIn( M4_CH_VM_NO_EXECUTE_PERM ) == 0 ){
				DUMP_CHLOG_WARNINGF(M4_CH_VM_NO_EXECUTE_PERM, M4ObjNodeItemI(poCMCR,ai_oState.m_hItem) << DumpCallStack() );
			}
			
			if( iError != 0 && M4ChLogIsErrorIn( M4_CH_VM_NO_LOAD_PERM + iError - 1) == 0 ){
				DUMP_CHLOG_WARNINGF( M4_CH_VM_NO_LOAD_PERM + iError - 1, DumpCallStack() );
			}
		}
	}
	else{ //si tiene permisos->vamos a por el handle
		
		//si tiene permiso->intentamos objtener el handle de la formula
		if (ai_bSpecSlice){ //la formula nos la indican con variant
			
			//si es de tipo fecha-> obtenemos hFormula por fecha
			if (ai_vSlice.Type == M4CL_CPP_TYPE_DATE){
				
				hFormula=poCMCR->GetItemCodeByDate (ai_oState.m_hItem, ai_oState.m_iRule, ai_vSlice.Data.DoubleData, iSlicePosition);
				//metemos las fechas en el estado
				ai_oState.m_dBeginDate = poCMCR->GetItemSliceStartDate	(ai_oState.m_hItem, iSlicePosition);
				ai_oState.m_dEndDate   = poCMCR->GetItemSliceEndDate	(ai_oState.m_hItem, iSlicePosition);
				
			}
			else if (ai_vSlice.Type == M4CL_CPP_TYPE_NUMBER ){ //nos pasan la posicion del tramo
				
				iSlicePosition = m4uint16_t(ai_vSlice.Data.DoubleData);
				
				hFormula=poCMCR->GetItemCodeByPosition (ai_oState.m_hItem, ai_oState.m_iRule, iSlicePosition );
				//metemos las fechas en el estado
				ai_oState.m_dBeginDate = poCMCR->GetItemSliceStartDate	(ai_oState.m_hItem, iSlicePosition);
				ai_oState.m_dEndDate   = poCMCR->GetItemSliceEndDate	(ai_oState.m_hItem, iSlicePosition);
				
			}
			else{	//en otro caso -> null
				DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE_FORMAT_EXECUTE, M4ObjNodeItemI(poCMCR,ai_oState.m_hItem) << DumpCallStack() );
				hFormula=0;
				
			}
			
		}
		else { //no nos especifican el tramo por variable, sino con las fechas del estado
			
			//segun sea concepto o no y estemos por tramos o no tomamos una fecha u otra
			if (poCMCR->GetItemType(ai_oState.m_hItem) == M4CL_ITEM_TYPE_CONCEPT && GetRunSliceMode(ai_oState.m_poAccess) ){  //concepto
				hFormula=poCMCR->GetItemCodeByDate (ai_oState.m_hItem, ai_oState.m_iRule, ai_oState.m_dBeginDate , iSlicePosition);
			}
			else{//metodo
				
				hFormula=poCMCR->GetItemCodeByDate (ai_oState.m_hItem, ai_oState.m_iRule, ai_oState.m_dDate, iSlicePosition);
			}
		}
	} //fin del else de permisos de ejecución
	
#if _M4_DEBUG > 10
	if (hFormula==0){
		DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Method " <<LogCat<<  poCMCR->iItemI(ai_oState.m_hItem) <<LogCat<<" has no instructions. Creating null execution");
	}
#endif
	
	//la creamso aquí y se eliminará al acabar la ejecución
	ai_oState.m_poFormula.Init( (m4pcchar_t)hFormula); //si inicia con el handle o con zero
	
	
	
	//establecemos la referencia en la pos. actual de la pila
	m_oRunTimeStack->SetReference(m_oRunTimeStack->GetPos()+1); //ponemos de referencia el tope
	
	//empezamos la ejecución en la instrucción 0, 
	ai_oState.m_iCurrent=0;
	ai_oState.m_iParentStatePos=ai_iParentStatePos;	// bug 0069037

	ai_oState.m_iSourceLine=1;	//No. Linea fuente
	ai_oState.m_iStepperType=StepperTypeNormal;
	
	//si necesitamso transaccion la ponemos
	m4uint8_t iTransMode=poCMCR->GetItemTransactionMode(ai_oState.m_hItem);
	if (iTransMode == M4CL_ITEM_TRAN_MODE_NEED_TRANSACTION ||
		iTransMode == M4CL_ITEM_TRAN_MODE_NEW_TRANSACTION  ) {
		
		BeginVMTrans();
	}
	
	
	//guardamos el estado en la pila de estados
	ret=m_oStateStack->Push(ai_oState);
	
	return ret;
}


m4return_t ClVMachine1::ExecuteFunctionForLocalTest (m4uint16_t iNumFunction){ //para que desde el local test puedan ejecutar funciones

	//Executamos la función
	return ExecuteFunction( iNumFunction, m_oImplementors, m_oState1 ) ;
}


void     ClVMachine1::SetRunSliceMode (ClAccess *ai_poAccess , m4bool_t ai_SliceMode){
	m4VariantType vValue (ai_SliceMode);

	m4return_t ret= ai_poAccess->RunSliceMode.Set( vValue ) ;
}

m4bool_t ClVMachine1::GetRunSliceMode (ClAccess *ai_poAccess) {

	m4VariantType vValue;
	ai_poAccess->RunSliceMode.Get( vValue ) ;
	
    if( vValue.Type == M4CL_CPP_TYPE_NULL )
    {
        return M4_FALSE;
    }
    else
    {
        return (vValue.Data.DoubleData != 0.0 ? M4_TRUE:M4_FALSE) ;
    }

}

//-------------------
//SliceMode
//-------------------
m4return_t  ClVMachine1:: GetSliceMode     (void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetSliceMode");
#endif
	
    RuntimePushDouble (GetRunSliceMode(m_oState1.m_poAccess));
    return M4_SUCCESS;
}



m4return_t  ClVMachine1:: SetSliceMode (void){
	m4VariantType       vTemp;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SetSliceMode");
#endif

    m4return_t ret=RuntimePop(vTemp);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error performing pop from stack.")	;

	m4bool_t	bSliceMode = vTemp.Data.DoubleData == 0.0 ? M4_FALSE : M4_TRUE;
    SetRunSliceMode (m_oState1.m_poAccess, bSliceMode);

	if (ClVMDebugEnv::getInstance()->getDebugLevel() >= 3)
	{
		ClCompiledMCR	*pMCRParent = m_oState1.m_poAccess->GetpCMCR() ;
		VMTracer::GetInstance(m_oState1.m_poAccess->GetEnvironment())
			<< beginthread
			<< "Setting slice mode " << bSliceMode
			<< " from item <"
			<< pMCRParent->iM4Obj() << "!" << pMCRParent->iNodeI( m_oState1.m_hItem ) << "." << pMCRParent->iItemI( m_oState1.m_hItem ) << "(" << m_oState1.m_iSourceLine << ")"
			<< ">" << endthread ;
	}
        
    RuntimePushDouble (M4_SUCCESS);
    return M4_SUCCESS;
}

void ClVMachine1::RefreshState(void){
		//recuperamos el estado
		ClVMachine0::RefreshState();
		m_oStateStack->Top(m_oState1);

}
void ClVMachine1::RefreshReenter(m4return_t ai_rRet){

	if (ai_rRet>1 || ai_rRet == M4CL_EXECUTION_INTERRUPTION) { //no podemos ejecutarlo o es una interrupción
		
#ifdef _DEBUG
		DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LEVEL_JUMPS, "Level greater than 1");
#endif
	}
	else{    
		m_oReenterStack->Pop  ();
		m_oReenterStack->Top  (m_iReenter);
		//recuperamos el estado
		RefreshState();
		//nos aseguramos de que no queden transacciones abiertas si salimos del todo
		if (m_oStateStack->IsEmpty() ) {
			EndVMTrans();
		}
	}
}

//----------------------------------------
//Runtime info - stepper
//----------------------------------------

m4return_t ClVMachine1::GetSymbolTable	(ClItem *ai_poItem, m4char_t *ai_pszCode, m4char_t **ao_ppszSymbols, m4uint32_t ai_iSizeTable, m4uint32_t ai_iSizeSymbol){ //ojo. asigna/libera el que llama

	m4return_t ret;
	m4uint16_t iNumArgs=ai_poItem->ItemDef.NumberOfArguments();
	m4char_t **ppszArgs=new m4char_t *[iNumArgs];
	m4uint16_t i;
	m4pcchar_t pszArg;

	for (i=0;i<iNumArgs;++i){
		pszArg=ai_poItem->ItemDef.ArgumentIdByPosition(i);
		ppszArgs[i]=new m4char_t [strlen(pszArg)+1];
		strcpy(ppszArgs[i],pszArg);
	}

	// corrige bug 0061238
	ret=m_oImplCompilerAndJit.GetSymbolTable(
					ai_poItem,			//contexto del item
					ai_pszCode,			//code
					ppszArgs,iNumArgs,  //args
					ao_ppszSymbols, ai_iSizeTable, ai_iSizeSymbol); //espacio para dejar los símbolos

	for (i=0;i<iNumArgs;++i){
		delete []ppszArgs[i];
	}
	delete[]ppszArgs;

	return ret;

}
