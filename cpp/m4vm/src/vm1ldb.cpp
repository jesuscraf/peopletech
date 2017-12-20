

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
//    Virtual machine de nivel 1- Funciones de la LDB
////
//==============================================================================

#include "vm1ldb.hpp"
#include "m4stl.hpp"

#include "vmres.hpp"
#include "sttlib.hpp"
#include "vmachin1.h"

#include "m4ldb.hpp"
#include "m4log.hpp"
#include "access.hpp"
#include "channel.hpp"
#include "pl_data.hpp"
#include "m4mdrt.hpp"
#include "cldbgutil.hpp"
#include "m4objglb.hpp"
#include "regi_smp.hpp"
#include "nodedef.hpp"
#include "m4hsparser.hpp"	



//----------------------------------------------------------
//Write
//----------------------------------------------------------

m4return_t ClVM1LDB::Write (void){

	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "Write"<<"insert_blk, update_blk" << DumpCallStack() );
	return M4_ERROR;


}





//----------------------------------------------------------
//Delete
//----------------------------------------------------------


m4return_t ClVM1LDB::Delete (void){
	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "Delete"<<"delete_blk" << DumpCallStack() );
	return M4_ERROR;


}




m4return_t  ClVM1LDB::Persist (void){

	// 1/10/98
	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "Persist"<<"Persist_tree" << DumpCallStack() );
	return M4_ERROR;
//	return _Persist("Persist", M4CL_ITEM_WRITE, M4CL_ITEM_DELETE);
	
}


m4return_t  ClVM1LDB::PersistAll (void){
	
	// 1/10/98
	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "PersistAll"<<"Persist_tree" << DumpCallStack() );

	return M4_ERROR;
	//return _Persist("PersistAll", M4CL_ITEM_WRITE_ALL, M4CL_ITEM_DELETE_ALL);
	
	
	
}


//---------------
//Persist de nodo
//---------------

m4return_t  ClVM1LDB::PersistNode (void){
	
	// 1/10/98
	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "Persist_node"<<"Persist_tree" << DumpCallStack() );

	return M4_ERROR;
	//return _PersistNode("PersistNode", M4CL_ITEM_WRITE, M4CL_ITEM_DELETE);
}



m4return_t  ClVM1LDB::PersistNodeAll (void){
	
	// 1/10/98
	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "PersistNodeAll"<<"Persist_tree" << DumpCallStack() );

	return M4_ERROR;
	//return _PersistNode("PersistNodeAll", M4CL_ITEM_WRITE_ALL, M4CL_ITEM_DELETE_ALL);
	
}


//---------------------------------------------------------------
//LDB transactions
//---------------------------------------------------------------

m4return_t ClVM1LDB::BeginTransaction (void){
	
	m4return_t ret;
	ClLConn_Interface  *oConn;

#if _M4_DEBUG > 10    
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: BeginTransaction");
#endif
	
	ret = GetVMCSContext()->PreBDLMethod(m_oState1.m_poAccess, m_oState1.m_hItem, "BeginTransaction");

	/* Bug 0080526
	Si no se puede conseguir la conexión ni se intenta y se da un sólo mensaje de error
	*/
	if( ret == M4_ERROR )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return M4_ERROR;
	}

	//obtenemos conexion
	oConn=GetConnectEnv( m_oState1.m_poAccess );
	
	//si no ha conexion libre
	if (oConn==0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_FREE_CONNECTION, "BeginTransaction"<<M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
	
	ClM4Clock oClock;
	oClock.Start();

	ret=oConn->BeginTransaction (m_oState1.m_poRecordSet);
	//el resultado

	if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddLDBTime(oClock.GetDifference());
	}

	RuntimePushDouble(ret);
	//la conexion se liberará al acabar la transaccion
	m4int32_t iOldtr=m_poVM->m_BeginTransaction;
	//Marcamos el inicio de la transacción (si no había ninguna iniciada)
	m_poVM->BeginVMTrans();
	if (iOldtr!=m_poVM->m_BeginTransaction){ //el inicio de transacción ha sido efectivo

		/* Bug 0089081
		Para calcular donde hay que recuperar la ejecución no basta con tomar
		2 posiciones arriba en la pila, porque puede haber estados de propagación
		entre medias, tanto los normales como los moveTo.
		Para hacer el cálculo hay que usar los items padre y subir 2 en la pila
		de estados.
		*/
		m4int32_t	iParentStatePos = m_oState1.m_iParentStatePos ;

		if( iParentStatePos != -1 )
		{
			m4VMState_t	oState ;

			ret = m_oStateStack->GetElement( oState, iParentStatePos ) ;
			DEBUG_CHECK_RES( ret == M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Error getting parent state in begintransaction." )	;

			iParentStatePos = oState.m_iParentStatePos ;
		}

		m_poVM->m_BeginTransaction=max(iParentStatePos, m_poVM->m_iReenter);
	}
	//explicación de esta linea
	//Cuando se inicia una transacción de VM desde la funcion de nivel 0 hay dos estdos menos
	//en la pila que cuando se inicia desde la funciónd e la ldb, que al ser un item del sistema 
	//tenemos por un lado en la pila el item y por otro el estado del que llama al BT, que en nivel 0 no esta 
	//porque el bucle de ejecución de L0 no tiene metido el estado que estamo ejecutando, para no andar
	//metiendolo y sacnadolo por intrucción (en L1 es necesario)
	//Así, para que la transacción deVM iniciado en BT sea la misma que se se llamase a la función de VM
	//hacemos que no considere esos dos estados, pero teniendo en cuanta que no se pase del punto de reentrada
	
	return (ret==M4_ERROR ? M4_ERROR : M4_SUCCESS);
	
	
}

m4return_t ClVM1LDB::EndTransaction (void){

	m4return_t ret;
	m4VariantType vTemp;
	m4bool_t bFree = M4_FALSE;
	ClLConn_Interface   *oConn;

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: EndTransaction");
#endif

	ret = GetVMCSContext()->PreBDLMethod(m_oState1.m_poAccess, m_oState1.m_hItem, "EndTransaction");

	/* Bug 0080526
	Si no se puede conseguir la conexión ni se intenta y se da un sólo mensaje de error
	*/
	if( ret == M4_ERROR )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return M4_ERROR;
	}

	//obtenemos conexion, sin crea ninguna si no estaba
	oConn=GetConnectEnv( m_oState1.m_poAccess,M4_FALSE );

	//debe haber conexion abierta-> sino es un error
	if (oConn==0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_CONNECT, "EndTransaction" << DumpCallStack() );
		RuntimePushDouble(M4_ERROR);
		return M4_ERROR;
	}
	 
	ret=RuntimePop (vTemp);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "EndTansaction.")	;

	ret=m_oVariantMediator->ToDouble(vTemp);
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "commit"<<"EndTransaction" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );
		DeleteVarString (vTemp);
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
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Commit"<<"EndTransaction"<<(m4uint32_t)vTemp.Data.DoubleData << DumpCallStack() );
		vTemp.Data.DoubleData=M4CL_VM_ET_ROLLBACK;
	}

	ClM4Clock oClock;
	oClock.Start();
	
	ret=oConn->EndTransaction (m_oState1.m_poRecordSet, (m4int32_t) vTemp.Data.DoubleData, bFree);

	if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddLDBTime(oClock.GetDifference());
	}

	//el resultado
	RuntimePushDouble(ret);

	if( vTemp.Data.DoubleData == M4CL_VM_ET_ROLLBACK )
	{
		//mensajito de error
		if( GET_ERROR_QUEUE_SIZE() == 0 )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_ROLLBACK_TRANS, DumpCallStack() );
		}
	}

	if( bFree == M4_TRUE )
	{
		if( vTemp.Data.DoubleData == M4CL_VM_ET_COMMIT )
		{
			if( ret != M4_ERROR )
			{
				// El commit se ha podido hacer acabamos la transacción
				m_poVM->EndVMTrans();
			}
		}
		else if( vTemp.Data.DoubleData == M4CL_VM_ET_ROLLBACK )
		{
			//hacen un rollback-> Nosotros tambien dejamos de ejecutar
			/* Bug 0103177
			Volvemos el cambio atrás por incompatibilidad (ver 0105696)
			*/
			/* Bug 0089081
			Volvemos el cambio atrás por incompatibilidad
			Hay que devolver éxito si se ha recupera bien para que continue
			return ret;
			*/

			ret = m_poVM->RecoverFromError(m_oState1.m_poAccess);

			/* Bug 0115803
			Si hay que recuperarse se devuelve el resultado de la recuperación
			Y sino como antes error
			*/
			if( bRecover == M4_TRUE )
			{
				return( ret ) ;
			}

			return( M4_ERROR ) ;
		}
	}
	
	return (ret==M4_ERROR ? M4_ERROR : M4_SUCCESS);
	
}

//---------------------------------------------------------
//Conexiones
//---------------------------------------------------------
ClLConn_Interface   *ClVM1LDB::GetConnectEnv (ClAccess *ai_pAccess, m4bool_t ai_bNewConn)
{
	return GetVMCSContext()->GetConnProvider()->GetConnection(ai_pAccess->GetpChannel (),ai_bNewConn);
}


m4return_t ClVM1LDB::GetConnect(void){

	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "GetConnect"<<"" << DumpCallStack() );
	
	return M4_ERROR;


}

m4return_t ClVM1LDB::FreeConnect(void){
	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "FreeConnect"<<"" << DumpCallStack() );
	return M4_ERROR;

  }

//-------------------------------------
//API SQL
//-------------------------------------

m4return_t ClVM1LDB::ExecuteSQL       (void){
	
    m4return_t ret=M4_SUCCESS;
    m4VariantType vTemp, *vTemp2 =0;
	m4uint32_t numParam;
    ClLConn_Interface   *oConn;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: ExecuteSQL");
#endif

#ifdef _DEBUG
    if (m_oState1.m_poRecordSet->GetCurrentBlock()->IsNihil() ){ //comprobamos que no sea nihil
		DUMP_CHLOG_DEBUGF(M4_CH_VM_WRONG_BLOCK_FOR_LDB,"LdbFunction: ExecuteSQL"<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())  << DumpCallStack() );
	}
#endif
	
    RuntimePop (vTemp);
    //Vemos cuantos argumentos trae y creamos un array para los parametros
	numParam = (m4uint32_t) vTemp.Data.DoubleData;
	
	if (numParam > 0) {
		vTemp2=new m4VariantType [ numParam ];
	}
	
	//sacamos los parametros en orden inverso
    m4uint32_t ind ;
	for (m4uint32_t i=0; i< numParam; ++i){
		ind = numParam-i-1;
        RuntimePop(vTemp2[ind]);
        if (vTemp2[ind].Type==M4CL_CPP_TYPE_STRING_POOL){
			m_oVariantMediator->ToCharString(vTemp2[ind]);
        }
    }
	
	//obtenemos conexion
	oConn=GetConnectEnv( m_oState1.m_poAccess );
	
    //si no ha conexion libre
    if (oConn==0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_FREE_CONNECTION, "ExecuteSQL"<<M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        RuntimePushDouble (M4_ERROR); //resultado
		if (vTemp2){
			delete [] vTemp2;
		}
		return M4_ERROR;
    }
		
	/* Bug 0061709, 0065627
	Se desactiva el modo de carga del canal.
	Ahora el bloque se marca como cargado en bdl
	*/
	m4uint8_t iMode = m_oState1.m_poAccess->GetAutoLoadMode() ;
	m_oState1.m_poAccess->SetAutoLoadMode( M4CL_AUTOLOAD_OFF ) ;


	ClM4Clock oClock;
	oClock.Start();
	
	ret= oConn->ExecuteSQL (m_oState1.m_poRecordSet, GetVMCSContext()->GetSttWrap()->GetStatistics(), m_oState1.m_hItem, M4_FALSE, vTemp2, numParam);

	if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddLDBTime(oClock.GetDifference());
	}

	/* Bug 0061709, 0065627
	Se recupera el modo de carga del canal.
	*/
	m_oState1.m_poAccess->SetAutoLoadMode( iMode ) ;


	//resultado
	RuntimePushDouble (ret); //res=0
	
	
	if (vTemp2){
		delete [] vTemp2;
	}
	
    return M4_SUCCESS;
}


m4return_t ClVM1LDB::ExecuteDirectSQL (void){
	
    m4return_t ret=M4_SUCCESS;
    m4VariantType vTemp, vIdConn, vMask, *vTemp2 = NULL;
	m4uint32_t numParam;
    ClLConn_Interface   *oConn;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: ExecuteDirectSQL");
#endif

#ifdef _DEBUG
	if (m_oState1.m_poRecordSet->GetCurrentBlock()->IsNihil() ){ //comprobamos que no sea nihil
		DUMP_CHLOG_DEBUGF(M4_CH_VM_WRONG_BLOCK_FOR_LDB,"LdbFunction: ExecuteDirectSQL"<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())  << DumpCallStack() );
	}
#endif
	
    RuntimePop (vTemp);

	//nos aseguramos de tener suficientes arg.
	if (vTemp.Data.DoubleData<2){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "ExecuteDirectSQL"<<"1-" << DumpCallStack() );
		return M4_ERROR;
	}

    //Vemos cuantos argumentos trae y creamos un array para los parametros
	numParam = (m4uint32_t) vTemp.Data.DoubleData-2;
	
	if (numParam > 0) {
		vTemp2=new m4VariantType [ numParam ];
	}
	
	//sacamos los parametros en orden inverso
    m4uint32_t ind ;
	for (m4uint32_t i=0; i< numParam; ++i){
		ind = numParam-i-1;
        RuntimePop(vTemp2[ind]);
        if (vTemp2[ind].Type==M4CL_CPP_TYPE_STRING_POOL){
			m_oVariantMediator->ToCharString(vTemp2[ind]);
        }
    }
	
	//el siguiente es la mascara de validación.
    ret=RuntimePopDouble(vMask);
	if (ret != M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Mask"<<"ExecuteDirectSQL" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vMask.Type)<<DumpCallStack() );
		if (vTemp2){
			delete [] vTemp2;
		}
		return M4_ERROR;
	}

	//el siguiente es el identificador de la conexión.
	ret=RuntimePopDouble(vIdConn);
	if (ret != M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "idConn"<<"ExecuteDirectSQL" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vIdConn.Type)<<DumpCallStack() );
		if (vTemp2){
			delete [] vTemp2;
		}
		return M4_ERROR;
	}

	//obtenemos la conexion
	oConn=GetConnectEnv( m_oState1.m_poAccess );
				
	//si no ha conexion libre
	if (oConn==0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_FREE_CONNECTION, "ExecuteDirectSQL"<<M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		if (vTemp2){
			delete [] vTemp2;
		}
		return M4_ERROR;
	}

	/* Bug 0061709, 0065627
	Se desactiva el modo de carga del canal.
	Ahora el bloque se marca como cargado en bdl
	*/
	m4uint8_t iMode = m_oState1.m_poAccess->GetAutoLoadMode() ;
	m_oState1.m_poAccess->SetAutoLoadMode( M4CL_AUTOLOAD_OFF ) ;

	
	ClM4Clock oClock;
	oClock.Start();

	ret= oConn->ExecuteSQL (m_oState1.m_poRecordSet, GetVMCSContext()->GetSttWrap()->GetStatistics(), m_oState1.m_hItem, M4_FALSE, vTemp2, numParam, (m4int32_t)vIdConn.Data.DoubleData, (m4int32_t)vMask.Data.DoubleData);
	
	if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddLDBTime(oClock.GetDifference());
	}

	/* Bug 0061709, 0065627
	Se recupera el modo de carga del canal.
	*/
	m_oState1.m_poAccess->SetAutoLoadMode( iMode ) ;


	//resultado
	RuntimePushDouble (ret); //res=0
	
	if (vTemp2){
		delete [] vTemp2;
	}
	
    return M4_SUCCESS;
	
}


m4return_t ClVM1LDB::ExecuteRealSQL (void){
	
    m4return_t ret=M4_SUCCESS;
    m4VariantType vTemp, vIdConn, *vTemp2 = NULL;
	m4uint32_t numParam;
    ClLConn_Interface   *oConn;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: ExecuteRealSQL");
#endif

#ifdef _DEBUG
    if (m_oState1.m_poRecordSet->GetCurrentBlock()->IsNihil() ){ //comprobamos que no sea nihil
		DUMP_CHLOG_DEBUGF(M4_CH_VM_WRONG_BLOCK_FOR_LDB,"LdbFunction: ExecuteRealSQL"<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())  << DumpCallStack() );
	}
#endif
	
    RuntimePop (vTemp);

	//nos aseguramos de tener suficientes arg.
	if (vTemp.Data.DoubleData<1){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "ExecuteRealSQL"<<"1-" << DumpCallStack() );
		return M4_ERROR;
	}

    //Vemos cuantos argumentos trae y creamos un array para los parametros
	numParam = (m4uint32_t) vTemp.Data.DoubleData-1;
	if (numParam > 0) {
		vTemp2=new m4VariantType [ numParam ];
	}
	
	
	//sacamos los parametros en orden inverso
    m4uint32_t ind ;
	for (m4uint32_t i=0; i< numParam; ++i){
		ind = numParam-i-1;
        RuntimePop(vTemp2[ind]);
        if (vTemp2[ind].Type==M4CL_CPP_TYPE_STRING_POOL){
			m_oVariantMediator->ToCharString(vTemp2[ind]);
        }
    }
	
	//el siguiente es el identificador de la conexión.
    ret=RuntimePopDouble(vIdConn);
    if (ret != M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "idConn"<<"ExecuteRealSQL" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vIdConn.Type)<<DumpCallStack() );
		if (vTemp2){
			delete [] vTemp2;
		}
		return M4_ERROR;
	}

	//obtenemos conexion
	oConn=GetConnectEnv( m_oState1.m_poAccess );
	
	//si no ha conexion libre
	if (oConn==NULL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_FREE_CONNECTION, "ExecuteRealSQL"<<M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		if (vTemp2){
			delete [] vTemp2;
		}
		return M4_ERROR;
	}

	/* Bug 0061709, 0065627
	Se desactiva el modo de carga del canal.
	Ahora el bloque se marca como cargado en bdl
	*/
	m4uint8_t iMode = m_oState1.m_poAccess->GetAutoLoadMode() ;
	m_oState1.m_poAccess->SetAutoLoadMode( M4CL_AUTOLOAD_OFF ) ;


	ClM4Clock oClock;
	oClock.Start();
		
	ret= oConn->ExecuteSQL (m_oState1.m_poRecordSet, GetVMCSContext()->GetSttWrap()->GetStatistics(), m_oState1.m_hItem, M4_TRUE, vTemp2, numParam, (m4int32_t)vIdConn.Data.DoubleData);
	
	if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddLDBTime(oClock.GetDifference());
	}

	/* Bug 0061709, 0065627
	Se recupera el modo de carga del canal.
	*/
	m_oState1.m_poAccess->SetAutoLoadMode( iMode ) ;


	RuntimePushDouble (ret); //res=0

	//fredom
	if (vTemp2){
		delete [] vTemp2;
	}
	
    return M4_SUCCESS;
	
}

//-------------------------------
//DDBB Transactions
//-------------------------------

m4return_t ClVM1LDB::BeginDBTransaction (void){
    m4return_t ret;
    ClLConn_Interface   *oConn;


#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: BeginDBTransaction");
#endif

	ret = GetVMCSContext()->PreBDLMethod(m_oState1.m_poAccess, m_oState1.m_hItem, "BeginDBTransaction");

	/* Bug 0080526
	Si no se puede conseguir la conexión ni se intenta y se da un sólo mensaje de error
	*/
	if( ret == M4_ERROR )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return M4_ERROR;
	}

	//obtenemos conexion
	oConn=GetConnectEnv( m_oState1.m_poAccess );
	
	//si no ha conexion libre
	if (oConn==0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_FREE_CONNECTION, "BeginDBTransaction"<<M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
	
	ClM4Clock oClock;
	oClock.Start();
	
	ret=oConn->BeginTransaction (m_oState1.m_poRecordSet);

	if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddLDBTime(oClock.GetDifference());
	}

	//el resultado
	RuntimePushDouble(ret);
	
	return (ret==M4_ERROR ? M4_ERROR : M4_SUCCESS);
	
}



m4return_t ClVM1LDB::EndDBTransaction (void){
    
	m4return_t ret;
	m4VariantType vTemp;
	m4bool_t bFree = M4_FALSE;
    ClLConn_Interface   *oConn;

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: EndDBTransaction");
#endif

	ret = GetVMCSContext()->PreBDLMethod(m_oState1.m_poAccess, m_oState1.m_hItem, "EndDBTransaction");

	/* Bug 0080526
	Si no se puede conseguir la conexión ni se intenta y se da un sólo mensaje de error
	*/
	if( ret == M4_ERROR )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return M4_ERROR;
	}

	//obtenemos conexion, sin crea ninguna si no estaba
	oConn=GetConnectEnv( m_oState1.m_poAccess,M4_FALSE );

	//debe haber conexion abierta-> sino es un error
	if (oConn==0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_CONNECT, "EndDBTransaction" << DumpCallStack() );
		RuntimePushDouble(M4_ERROR);
		return M4_ERROR;
	}
	
	ret=RuntimePop (vTemp);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "EndDBTransaction.")	;
	
	ret=m_oVariantMediator->ToDouble(vTemp);
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "commit"<<"endDBTransaction" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );

		DeleteVarString (vTemp);
		return M4_ERROR;
	}
	
	//chequeo de rangos
	/* Bug 0147956
	Se vuelca un error y el valor asumido es rollback
	*/
	if (vTemp.Data.DoubleData > M4CL_VM_ET_MAX_VALUE || vTemp.Data.DoubleData < M4CL_VM_ET_MIN_VALUE){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Commit"<<"EndDbTransaction"<<(m4uint32_t)vTemp.Data.DoubleData << DumpCallStack() );
		vTemp.Data.DoubleData=M4CL_VM_ET_ROLLBACK;
	}

	ClM4Clock oClock;
	oClock.Start();
	
	ret=oConn->EndTransaction (m_oState1.m_poRecordSet, (m4int32_t) vTemp.Data.DoubleData, bFree);

	if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddLDBTime(oClock.GetDifference());
	}

	//el resultado
	RuntimePushDouble(ret);
	
	if (vTemp.Data.DoubleData==M4CL_VM_ET_ROLLBACK){ //mensajito de error
		if (GET_ERROR_QUEUE_SIZE() == 0){
			DUMP_CHLOG_ERRORF(M4_CH_VM_ROLLBACK_DB_TRANS, DumpCallStack() );
		}
	}

	return (ret==M4_ERROR ? M4CL_USER_ERROR_ABORT_TRANSACTION : M4_SUCCESS);
	
}


m4return_t ClVM1LDB::DBTransactionNumber (void)
{
	m4int32_t			iNumTrans = 0;
    ClLConn_Interface	*poConn = NULL;


#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: DBTransactionNumber");
#endif

	//obtenemos conexion, sin crear ninguna si no estaba
	poConn = GetConnectEnv( m_oState1.m_poAccess, M4_FALSE );

	if( poConn != NULL )
	{
		iNumTrans = poConn->GetNumTrans();
	}

	RuntimePushDouble( iNumTrans );
	return M4_SUCCESS;
}




//------------------------------------------------
// NEW PERSISTENCE MODEL
//------------------------------------------------

//------
//Load
//------

m4return_t ClVM1LDB::SysLoad (void){
	m4return_t ret;
	ClLConn_Interface   *oConn;
    
    
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: SysLoad");
#endif

	if (m_oState1.m_poRecordSet->GetCurrentBlock()->IsNihil() ){ //comprobamos que no sea nihil
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK_FOR_LDB,"LdbFunction: SysLoad"<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())  << DumpCallStack() );
		return M4_ERROR;
	}


	//obtenemos conexion
	oConn=GetConnectEnv( m_oState1.m_poAccess );
    
    
	//si no ha conexion libre
	if (oConn==0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_FREE_CONNECTION, "SysLoad"<<M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
    
	/* Bug 0061709, 0065627
	Se desactiva el modo de carga del canal.
	Ahora el bloque se marca como cargado en bdl
	*/
	m4uint8_t iMode = m_oState1.m_poAccess->GetAutoLoadMode() ;
	m_oState1.m_poAccess->SetAutoLoadMode( M4CL_AUTOLOAD_OFF ) ;


	ClM4Clock oClock;
	oClock.Start();
	
	ret=oConn->SysLoad(m_oState1.m_poRecordSet, GetVMCSContext()->GetSttWrap()->GetStatistics());
    
	if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddLDBTime(oClock.GetDifference());
	}
	
	/* Bug 0061709, 0065627
	Se recupera el modo de carga del canal.
	*/
	m_oState1.m_poAccess->SetAutoLoadMode( iMode ) ;


	//el resultado
	RuntimePushDouble(ret);
    
	return (ret==M4_ERROR ? M4_ERROR : M4_SUCCESS);
}


m4return_t ClVM1LDB::Load (void){
	m4return_t ret;
	ClLConn_Interface   *oConn;
    
    
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: Load");
#endif

	ret = GetVMCSContext()->PreBDLMethod(m_oState1.m_poAccess, m_oState1.m_hItem, "Load");

	/* Bug 0080526
	Si no se puede conseguir la conexión ni se intenta y se da un sólo mensaje de error
	*/
	if( ret == M4_ERROR )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return M4_ERROR;
	}

	if (m_oState1.m_poRecordSet->GetCurrentBlock()->IsNihil() ){ //comprobamos que no sea nihil
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK_FOR_LDB,"LdbFunction: Load"<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())  << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}

	//obtenemos conexion
	oConn=GetConnectEnv( m_oState1.m_poAccess );
    
	//si no ha conexion libre
	if (oConn==0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_FREE_CONNECTION, "Load_Blk"<<M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
    
	ClM4Clock oClock;
	m4uint8_t iMode ;

	m4uint32_t NumRows = m_oState1.m_poRecordSet->GetpNodeDef()->NumRows();

	if (NumRows)
    {
        ClBlockReference br = m_oState1.m_poRecordSet->GetCurrentBlock();
		ClPartialLoadData *poPartial = br->GetpPL_Data();
        m4int32_t ai_iNumberToWrite = poPartial->GetNumberToWrite();
        m4int32_t ai_iFirstToRead = poPartial->GetFirstToRead();
        m4int32_t ai_iFirstToWrite = poPartial->GetFirstToWrite();
        //indice machacable por SetLotOfItems:
        poPartial->SetCurrentRegister(ai_iFirstToWrite);
		
		/* Bug 0070883
		Hay que poner chequear que si está cargado no se venga de una load.
		*/
        if (poPartial->GetFirstWindowLoaded()==M4_TRUE && poPartial->GetLoadFromPageFault()==M4_FALSE) {

			/* Bug 0083829
			Hay que dar un mensaje de error en este caso, aunque no se corta la ejecución
			*/
			DUMP_CHLOG_ERRORF( M4_CH_VM_REPEATED_LOAD_IN_PARTIAL, M4ObjNodeItemI( m_oState1.m_poAccess->GetpCMCR(), m_oState1.m_hItem ) << DumpCallStack() ) ;

			RuntimePushDouble(M4_SUCCESS);
			return M4_SUCCESS;
		}

		/* Bug 0061709, 0065627
		Se desactiva el modo de carga del canal.
		Ahora el bloque se marca como cargado en bdl
		*/
		iMode = m_oState1.m_poAccess->GetAutoLoadMode() ;
		m_oState1.m_poAccess->SetAutoLoadMode( M4CL_AUTOLOAD_OFF ) ;


		oClock.Start();
	
		ret = oConn->Load (m_oState1.m_poRecordSet, GetVMCSContext()->GetSttWrap()->GetStatistics(), 0, NULL, 0, -1, &ai_iNumberToWrite, ai_iFirstToRead);

		if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
		{
			GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddLDBTime(oClock.GetDifference());
		}

		/* Bug 0061709, 0065627
		Se recupera el modo de carga del canal.
		*/
		m_oState1.m_poAccess->SetAutoLoadMode( iMode ) ;

		
// Marco el bloque como cargado. Por si hacen load, no executar nada.
        poPartial->SetFirstWindowLoaded (M4_TRUE);

        poPartial->SetReadRegister(ai_iNumberToWrite);

		/* Bug 0098542
		Si no se han cargado registros hay que poner que estamos al final
		*/
		if( ai_iNumberToWrite == 0 )
		{
			poPartial->SetEndOfRegisters( M4_TRUE ) ;
		}


        m4uint32_t FirstVisible = poPartial->GetNextFV();
        m4uint32_t LastVisible = poPartial->GetNextLV();

        poPartial->SetFirstVisible(FirstVisible);
        poPartial->SetLastVisible(LastVisible);

        m4uint32_t prep = poPartial->GetPrepareIndexPL();
        m4uint32_t iLV;
        m4uint32_t iFV;

        if (prep == 1)
		{
            m_oState1.m_poRecordSet->Register.Filter->PrepareIndexPL(M4_FALSE, br);
			/* Bug 0070559
			Hay que ajustar el LastVisible al tamaño de página
			*/
            iFV = poPartial->GetFirstVisible();
            poPartial->SetLastVisible(iFV + NumRows);
		}
        else if (prep == 2)
        {
            m_oState1.m_poRecordSet->Register.Filter->PrepareIndexPL(M4_TRUE, br);
            iLV = poPartial->GetLastVisible();
            poPartial->SetFirstVisible(iLV - NumRows);
        }
    }
    else
    {
		/* Bug 0061709, 0065627
		Se desactiva el modo de carga del canal.
		Ahora el bloque se marca como cargado en bdl
		*/
		iMode = m_oState1.m_poAccess->GetAutoLoadMode() ;
		m_oState1.m_poAccess->SetAutoLoadMode( M4CL_AUTOLOAD_OFF ) ;

		
		oClock.Start();

		ret=oConn->Load(m_oState1.m_poRecordSet, GetVMCSContext()->GetSttWrap()->GetStatistics());

		if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
		{
			GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddLDBTime(oClock.GetDifference());
		}

		/* Bug 0061709, 0065627
		Se recupera el modo de carga del canal.
		*/
		m_oState1.m_poAccess->SetAutoLoadMode( iMode ) ;

		
        //Actualizamos el filtro, por ahora, luego habra que actualizar todos los iteradores
        m_oState1.m_poRecordSet->Current.Filter.UpdateFilter();
        m_oState1.m_poRecordSet->Register.Filter.UpdateFilter();

		//si hay registros nos posicionamos en el primero
		if ( m_oState1.m_poRecordSet->Current.Count() ) 
			m_oState1.m_poRecordSet->Current.Begin();
    }
	
	//el resultado
	RuntimePushDouble(ret);
    
	
	return (ret==M4_ERROR ? M4_ERROR : M4_SUCCESS);
}


m4return_t ClVM1LDB::Unload (void){

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: UnLoad");
#endif
	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "Unload"<<"Meta4Object Function Unload" << DumpCallStack() );
	RuntimePushDouble(M4_ERROR);
	return M4_SUCCESS;
}


//
//load_prg
//
m4return_t ClVM1LDB::Load_prg         (void) {

#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: Load_prg");
#endif

	m4uint32_t hItem, hNode;

	//obtenemos el handle del nodo en el que estamos
	hNode=m_oState1.m_poAccess-> GetpCMCR() ->GetItemNodeHandle (m_oState1.m_hItem);

#ifdef _DEBUG
	if (hNode == 0) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_NODE_HANDLE, M4ObjNodeItemI(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_hItem) << DumpCallStack() );

		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
#endif
	
	
	//obtenemos los handles
	hItem    =   m_oState1.m_poAccess-> GetpCMCR() ->GetNodeLoadBlkItem (hNode);
	if (hItem == 0) {
		g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<M4CL_ITEM_LOAD_BLK
														<<m_oState1.m_poAccess->GetpCMCR()->iNodeN(hNode)<<LogCatAnd<<m_oState1.m_poAccess->GetpCMCR()->nNodeN(hNode)
														<<"LOAD_PRG"<< DumpCallStack() << EndLog;
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
    return CallPersistenceItem_blk (hItem);
}


//-----------------------------------
//Extended load
//-----------------------------------
m4return_t ClVM1LDB::ExtendedLoad (void){

	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "ExtendedLoad"<<"load_prg" << DumpCallStack() );
	return M4_ERROR;

}



//-------
//Delete
//-------
m4return_t ClVM1LDB::Delete_blk       (void) {
	
   	m4return_t ret;
	ClLConn_Interface   *oConn;
	m4bool_t bStopExecution = M4_FALSE;
	
    
#if _M4_DEBUG > 10  
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: DeleteBlk");
#endif

	ret = GetVMCSContext()->PreBDLMethod(m_oState1.m_poAccess, m_oState1.m_hItem, "DeleteBlk");

	/* Bug 0080526
	Si no se puede conseguir la conexión ni se intenta y se da un sólo mensaje de error
	*/
	if( ret == M4_ERROR )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return M4_ERROR;
	}

#ifdef _DEBUG
	if (m_oState1.m_poRecordSet->GetCurrentBlock()->IsNihil() ){ //comprobamos que no sea nihil
		DUMP_CHLOG_DEBUGF(M4_CH_VM_WRONG_BLOCK_FOR_LDB,"LdbFunction: Delete_blk"<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())  << DumpCallStack() );
	}
#endif

	//obtenemos conexion
	oConn=GetConnectEnv( m_oState1.m_poAccess );
	
	//si no ha conexion libre
	if (oConn==0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_FREE_CONNECTION, "Delete_Blk"<<M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}

	ClM4Clock oClock;
	oClock.Start();
	
	ret=oConn->DeleteBlk(m_oState1.m_poRecordSet, GetVMCSContext()->GetSttWrap()->GetStatistics(),bStopExecution);
	
	if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddLDBTime(oClock.GetDifference());
	}

	//el resultado
	RuntimePushDouble(ret);
	// Dependiendo de lo que venga en bStopExecution, tengo que parar o no la ejecución.
	// Con esto se evita que se haga Rollback de lo que ya hubiera persistido.
	if (ret == M4_ERROR){
		return (bStopExecution? M4_ERROR : M4_SUCCESS);
	}
	else{
		return M4_SUCCESS;
	}
	
}

//
//Delete_prg
//
m4return_t ClVM1LDB::Delete_prg       (void) {

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: Delete_prg");
#endif
	
	m4uint32_t hItem, hNode;
	//obtenemos el handle del nodo en el que estamos
	hNode=m_oState1.m_poAccess->GetpCMCR()->GetItemNodeHandle (m_oState1.m_hItem);

#ifdef _DEBUG
	if (hNode == 0) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_NODE_HANDLE, M4ObjNodeItemI(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_hItem) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
#endif
	
	
	//obtenemos los handles
	hItem    =   m_oState1.m_poAccess-> GetpCMCR() ->GetNodeDeleteBlkItem (hNode);
	if (hItem == 0) {
		g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<M4CL_ITEM_DELETE_BLK
														<<m_oState1.m_poAccess->GetpCMCR()->iNodeN(hNode)<<LogCatAnd<<m_oState1.m_poAccess->GetpCMCR()->nNodeN(hNode)
														<<"DELETE_PRG"<< DumpCallStack() << EndLog;
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
    return CallPersistenceItem_blk (hItem);
	
}

//-------
//Update
//-------
m4return_t ClVM1LDB::Update_blk       (void) {
    
    m4return_t ret;
	ClLConn_Interface   *oConn;
	m4bool_t bStopExecution = M4_FALSE;
	
    
#if _M4_DEBUG > 10 
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: UpdateBlk");
#endif

	ret = GetVMCSContext()->PreBDLMethod(m_oState1.m_poAccess, m_oState1.m_hItem, "Update_blk");

	/* Bug 0080526
	Si no se puede conseguir la conexión ni se intenta y se da un sólo mensaje de error
	*/
	if( ret == M4_ERROR )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return M4_ERROR;
	}

#ifdef _DEBUG
	if (m_oState1.m_poRecordSet->GetCurrentBlock()->IsNihil() ){ //comprobamos que no sea nihil
		DUMP_CHLOG_DEBUGF(M4_CH_VM_WRONG_BLOCK_FOR_LDB,"LdbFunction: Update_blk"<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())  << DumpCallStack() );
	}
#endif

	//obtenemos conexion
	oConn=GetConnectEnv( m_oState1.m_poAccess );
	
	//si no ha conexion libre
	if (oConn==NULL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_FREE_CONNECTION, "Update_Blk"<<M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
	
	ClM4Clock oClock;
	oClock.Start();
	
	ret=oConn->UpdateBlk(m_oState1.m_poRecordSet, GetVMCSContext()->GetSttWrap()->GetStatistics(),bStopExecution);

	if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddLDBTime(oClock.GetDifference());
	}

	//el resultado
	RuntimePushDouble(ret);
	// Dependiendo de lo que venga en bStopExecution, tengo que parar o no la ejecución.
	// Con esto se evita que se haga Rollback de lo que ya hubiera persistido.
	if (ret == M4_ERROR){
		return (bStopExecution? M4_ERROR : M4_SUCCESS);
	}
	else{
		return M4_SUCCESS;
	}
}


//
//Update_prg
//
m4return_t ClVM1LDB::Update_prg       (void) {
	
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: Update_prg");
#endif

	m4uint32_t hItem, hNode;
	//obtenemos el handle del nodo en el que estamos
	hNode=m_oState1.m_poAccess-> GetpCMCR() ->GetItemNodeHandle (m_oState1.m_hItem);

#ifdef _DEBUG
	if (hNode == 0) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_NODE_HANDLE, M4ObjNodeItemI(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_hItem) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
#endif
	
	
	//obtenemos los handles
	hItem    =   m_oState1.m_poAccess-> GetpCMCR() ->GetNodeUpdateBlkItem (hNode);
	if (hItem == 0) {
		g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<M4CL_ITEM_UPDATE_BLK
														<<m_oState1.m_poAccess->GetpCMCR()->iNodeN(hNode)<<LogCatAnd<<m_oState1.m_poAccess->GetpCMCR()->nNodeN(hNode)
														<<"UPDATE_PRG"<< DumpCallStack() << EndLog;
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
    return CallPersistenceItem_blk (hItem);
}

//-------
//Insert
//-------
m4return_t ClVM1LDB::Insert_blk       (void) {
    m4return_t ret;
	ClLConn_Interface   *oConn;
	m4bool_t bStopExecution = M4_FALSE;
	
    
#if _M4_DEBUG > 10 
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: InsertBlk");
#endif

	ret = GetVMCSContext()->PreBDLMethod(m_oState1.m_poAccess, m_oState1.m_hItem, "Insert_blk");

	/* Bug 0080526
	Si no se puede conseguir la conexión ni se intenta y se da un sólo mensaje de error
	*/
	if( ret == M4_ERROR )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return M4_ERROR;
	}

#ifdef _DEBUG
	if (m_oState1.m_poRecordSet->GetCurrentBlock()->IsNihil() ){ //comprobamos que no sea nihil
		DUMP_CHLOG_DEBUGF(M4_CH_VM_WRONG_BLOCK_FOR_LDB,"LdbFunction: Insert_blk"<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())  << DumpCallStack() );
	}
#endif

	//obtenemos conexion
	oConn=GetConnectEnv( m_oState1.m_poAccess );
	
	//si no ha conexion libre
	if (oConn==NULL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_FREE_CONNECTION, "Insert_Blk"<<M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
	
	ClM4Clock oClock;
	oClock.Start();
	
	ret=oConn->InsertBlk(m_oState1.m_poRecordSet, GetVMCSContext()->GetSttWrap()->GetStatistics(),bStopExecution);

	if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddLDBTime(oClock.GetDifference());
	}

	//el resultado
	RuntimePushDouble(ret);
	// Dependiendo de lo que venga en bStopExecution, tengo que parar o no la ejecución.
	// Con esto se evita que se haga Rollback de lo que ya hubiera persistido.
	if (ret == M4_ERROR){
		return (bStopExecution? M4_ERROR : M4_SUCCESS);
	}
	else{
		return M4_SUCCESS;
	}
}


//
//Insert_prg
//

m4return_t ClVM1LDB::Insert_prg       (void) {

#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: Insert_prg");
#endif
	
	m4uint32_t hItem, hNode;
	//obtenemos el handle del nodo en el que estamos
	hNode=m_oState1.m_poAccess-> GetpCMCR() ->GetItemNodeHandle (m_oState1.m_hItem);

#ifdef _DEBUG
	if (hNode == 0) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_NODE_HANDLE, M4ObjNodeItemI(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_hItem) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
#endif
	
	
	//obtenemos los handles
	hItem    =   m_oState1.m_poAccess-> GetpCMCR() ->GetNodeInsertBlkItem (hNode);
	if (hItem == 0) {
		g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<M4CL_ITEM_INSERT_BLK
														<<m_oState1.m_poAccess->GetpCMCR()->iNodeN(hNode)<<LogCatAnd<<m_oState1.m_poAccess->GetpCMCR()->nNodeN(hNode)
														<<"INSERT_PRG"<< DumpCallStack() << EndLog;
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
    return CallPersistenceItem_blk (hItem);
}

//------------
//Persist Tree
//------------
m4return_t ClVM1LDB::Persist_tree     (void){
	m4uint32_t  hNode;
	m4uint32_t  hEnd, hBegin, hDelete, hUpdate, hInsert;
	m4return_t  ret=M4_SUCCESS;
	m4uint16_t  Flags;
	m4int32_t	iParentStatePos ;
	
	
	
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: Persist_tree");
#endif

	/* Bug 0080526
	Aquí no debe salir porque sino puede salir 2 veces
	ret = GetVMCSContext()->PreBDLMethod(m_oState1.m_poAccess, m_oState1.m_hItem, "Persist_tree");
	*/

	//obtenemos el handle del nodo en el que estamos
	hNode=m_oState1.m_poAccess-> GetpCMCR() ->GetItemNodeHandle (m_oState1.m_hItem);
	if (hNode==NULL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_NODE_HANDLE, M4ObjNodeItemI(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_hItem) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return ret;
	}
	
	
	//obtenemos los handles
	hEnd    =   m_oState1.m_poAccess-> GetpCMCR() ->GetNodeItemHandleById (hNode, M4CL_ITEM_END_TRANSACTION);
	if (hEnd==NULL){
		g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<M4CL_ITEM_END_TRANSACTION
														<<m_oState1.m_poAccess->GetpCMCR()->iNodeN(hNode)<<LogCatAnd<<m_oState1.m_poAccess->GetpCMCR()->nNodeN(hNode)
														<<"PERSIST_TREE"<< DumpCallStack() << EndLog;
		return M4_ERROR;
	}
	
	hBegin  =   m_oState1.m_poAccess-> GetpCMCR() ->GetNodeItemHandleById (hNode, M4CL_ITEM_BEGIN_TRANSACTION);
	if (hBegin==NULL){
		g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<M4CL_ITEM_BEGIN_TRANSACTION
														<<m_oState1.m_poAccess->GetpCMCR()->iNodeN(hNode)<<LogCatAnd<<m_oState1.m_poAccess->GetpCMCR()->nNodeN(hNode)
														<<"PERSIST_TREE"<< DumpCallStack() << EndLog;
		return M4_ERROR;
	}
	
	hUpdate =   m_oState1.m_poAccess-> GetpCMCR() ->GetNodeItemHandleById (hNode, M4CL_ITEM_UPDATE_PRG);
	if (hUpdate==NULL){
		g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<M4CL_ITEM_UPDATE_PRG
														<<m_oState1.m_poAccess->GetpCMCR()->iNodeN(hNode)<<LogCatAnd<<m_oState1.m_poAccess->GetpCMCR()->nNodeN(hNode)
														<<"PERSIST_TREE"<< DumpCallStack() << EndLog;
		return M4_ERROR;
	}
	
	hInsert  =   m_oState1.m_poAccess-> GetpCMCR() ->GetNodeItemHandleById (hNode, M4CL_ITEM_INSERT_PRG);
	if (hInsert==NULL){
		g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<M4CL_ITEM_INSERT_PRG
														<<m_oState1.m_poAccess->GetpCMCR()->iNodeN(hNode)<<LogCatAnd<<m_oState1.m_poAccess->GetpCMCR()->nNodeN(hNode)
														<<"PERSIST_TREE"<< DumpCallStack() << EndLog;
		return M4_ERROR;
	}
	
	hDelete =   m_oState1.m_poAccess-> GetpCMCR() ->GetNodeItemHandleById (hNode, M4CL_ITEM_DELETE_PRG);
	if (hDelete==NULL){
		g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<M4CL_ITEM_DELETE_PRG
														<<m_oState1.m_poAccess->GetpCMCR()->iNodeN(hNode)<<LogCatAnd<<m_oState1.m_poAccess->GetpCMCR()->nNodeN(hNode)
														<<"PERSIST_TREE"<< DumpCallStack() << EndLog;
		return M4_ERROR;
	}
	
	
	
	//metemos los metodos para que se ejecuten
	//como es para el bloque en el que estamos mandamos ttodos los datos de
	//m_oState1, excepto el hItem a ejecutar
	
	Flags= 0;
	M4CL_SET_STATE_PURGE_FLAG (Flags);
	
	//el resultado
	RuntimePushDouble(M4_SUCCESS);
	
    m4VariantType vTemp;
	vTemp.Type = M4CL_CPP_TYPE_NULL;
	
	m4VMState_t oNewState;
    oNewState=m_oState1;

	/* Bug 0069037
	Todos los métodos son hijos del mismo padre
	*/

	iParentStatePos = m_poVM->GetStateStack()->GetPos() ;
	
	//1.-metemos para ejecutar "EndTransaction"
	
    RuntimePushDouble (M4_TRUE); //parametro del item EndTransaction
	
    oNewState.m_hItem=hEnd;
    oNewState.m_iRule=0;
    oNewState.m_Flags=Flags;
	
    ret=m_poVM->AddMethodToExecute (oNewState, vTemp, iParentStatePos, M4_FALSE);
	if (ret!=M4_SUCCESS){
		RuntimePushDouble (ret);
		return ret;
	}
	
    //2.-metemos para ejecutar "Insert"
    oNewState.m_hItem=hInsert;
   // oNewState.m_iRule=0;
   // oNewState.m_Flags=Flags;
	
    ret=m_poVM->AddMethodToExecute (oNewState, vTemp, iParentStatePos, M4_FALSE);
	if (ret!=M4_SUCCESS){
		RuntimePushDouble (ret);
		return ret;
	}
	
	//3.-metemos para ejecutar "Update"
    oNewState.m_hItem=hUpdate;
    //oNewState.m_iRule=0;
    //oNewState.m_Flags=Flags;
	
    ret=m_poVM->AddMethodToExecute (oNewState, vTemp, iParentStatePos, M4_FALSE);
	if (ret!=M4_SUCCESS){
		RuntimePushDouble (ret);
		return ret;
	}
	
	
	//4.-metemos para ejecutar "Delete"
    oNewState.m_hItem=hDelete;
    //oNewState.m_iRule=0;
    //oNewState.m_Flags=Flags;
	
    ret=m_poVM->AddMethodToExecute (oNewState, vTemp, iParentStatePos, M4_FALSE);
	if (ret!=M4_SUCCESS){
		RuntimePushDouble (ret);
		return ret;
	}
	
	//5.-metemos para ejecutar "BeginTransaction"
    oNewState.m_hItem=hBegin;
    //oNewState.m_iRule=0;
    //oNewState.m_Flags=Flags;
	
    ret=m_poVM->AddMethodToExecute (oNewState, vTemp, iParentStatePos, M4_FALSE);
	if (ret!=M4_SUCCESS){
		RuntimePushDouble (ret);
		return ret;
	}
	
	
	return ret;
}



m4return_t ClVM1LDB::MergeSentences(void)
{

	m4return_t ret = M4_SUCCESS;
	m4pchar_t pcChildSentence;
	m4pchar_t pcParentSentence;
	m4VariantType vChildSentence;
	m4VariantType vParentSentence;
	M4HsParser oParser;


#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: MergeSentences");
#endif


    ret = RuntimePopString (vParentSentence);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ParentSentence" << "MergeSentences" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vParentSentence.Type)<<DumpCallStack() );

	pcParentSentence = m_oDynamicPool->GetString(vParentSentence.Data.CharData); 
	istrstream oParent (pcParentSentence);
	m_oDynamicPool->DeleteString (vParentSentence.Data.CharData);

    ret = RuntimePopString (vChildSentence);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ChildSentence" << "MergeSentences" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vChildSentence.Type)<< DumpCallStack() );

	pcChildSentence = m_oDynamicPool->GetString(vChildSentence.Data.CharData); 
	istrstream oChild (pcChildSentence);
	m_oDynamicPool->DeleteString (vChildSentence.Data.CharData);

	ostrstream oResult;
	ret = oParser.SentenceMerge (oChild, oParent, oResult);

	if( ret != M4_SUCCESS )
	{
	    RuntimePushString("");
		return M4_ERROR ;
	}

    RuntimePushString(oResult.str(), vParentSentence.IsUser || vChildSentence.IsUser);
    return M4_SUCCESS;
}




//--------
//auxiliar
//--------
m4return_t ClVM1LDB::CallPersistenceItem_blk (m4uint32_t ai_hItem) {
	
	
	m4return_t  ret=M4_SUCCESS;
	m4int32_t	iParentStatePos ;
	
	
	
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_LBD, "Trace VM-LDB: Persistence item:" <<LogCat<< m_oState1.m_poAccess->GetpCMCR()->GetItemId(ai_hItem) <<LogCat<< " )" );
#endif


	/* Bug 0080526
	Aquí no debe salir porque sino puede salir 2 veces
	ret = GetVMCSContext()->PreBDLMethod(m_oState1.m_poAccess, m_oState1.m_hItem, m_oState1.m_poAccess->GetpCMCR()->GetItemId(m_oState1.m_hItem) );
	*/

	//metemos el metodo para que se ejecute
	//como es para el bloque en el que estamos mandamos todos los datos de
	//m_oState1, excepto el hItem a ejecutar
	
    m4VariantType vTemp;
	vTemp.Type = M4CL_CPP_TYPE_NULL;
	
	m4VMState_t oNewState;
    oNewState=m_oState1;
	

    oNewState.m_hItem=ai_hItem;
    oNewState.m_iRule=0;
    oNewState.m_Flags=0;
	
	iParentStatePos = m_poVM->GetStateStack()->GetPos() ;
    ret=m_poVM->AddMethodToExecute (oNewState, vTemp, iParentStatePos, M4_FALSE);
	
    if (ret!=M4_SUCCESS){
//		DUMP_CHLOG_ERRORF(M4_CH_VM_CANT_EXECUTE_ITEM, M4ObjNodeItemI(m_oState1.m_poAccess->GetpCMCR(),ai_hItem)  << DumpCallStack() );
		return ret;
	}
	
    //no metemos resultado en la pila, porque dejamos el que ponga el método al que llamamos
	return ret;
}


m4return_t ClVM1LDB::SetLoadRange(void)
{

	m4return_t		iResult = M4_SUCCESS ;
	m4uint32_t		iRangeSize = 0 ;
	m4uint32_t		iRangeStart = 0 ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iNumArg = 0 ;
	m4VariantType	vNode ;
	m4VariantType	vM4Obj ;
	m4VariantType	vValue ;
	ClAccess		*poAccess = NULL ;
	ClNode			*poNode = NULL ;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF( M4_CH_VM_TRACE_VM_L1, "Trace VM1: " << LogCat << "SetLoadRange" ) ;
#endif


    // Leemos el número de argumentos (pueden ser 2, 3 ó 4)
	iResult = RuntimePopNumber( vValue ) ;
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "SetLoadRange" ) ;

    iNumArg = m4uint32_t( vValue.Data.DoubleData ) ;

	if( iNumArg < 2 )
	{
		// Si hay menos de 2 argumentos se borran y se da error
		DUMP_CHLOG_ERRORF( M4_CH_VM_WRONG_PARAM_NUM, "SetLoadRange" << "2-3-4" << DumpCallStack() ) ;

		for( i = 0 ; i < iNumArg ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
		}
		RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}

	// Limpiamos los argumentos de más
	if( iNumArg > 4 )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_TOO_PARAM, "SetLoadRange" << 4 << iNumArg + 1 << DumpCallStack() ) ;

		for( i = 4 ; i < iNumArg ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
			M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "SetLoadRange" ) ;
		}
		iNumArg = 4 ;
	}

	// Range start
	iResult = RuntimePop( vValue ) ;
	M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "SetLoadRange" ) ;

	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "RangeStart" << "SetLoadRange" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
		DeleteVarString( vValue ) ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	iRangeStart = m4uint32_t( vValue.Data.DoubleData ) ;

	// Range size
	iResult = RuntimePop( vValue ) ;
	M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "SetLoadRange" ) ;

	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "RangeSize" << "SetLoadRange" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
		DeleteVarString( vValue ) ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	iRangeSize = m4uint32_t( vValue.Data.DoubleData ) ;

	// Nodo
	if( iNumArg >= 3 )
	{
		iResult = RuntimePop( vNode ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "SetLoadRange" ) ;
	}
	else
	{
		vNode.Type = M4CL_CPP_TYPE_NULL ;
	}

	// Meta4Object
	poAccess = m_oState1.m_poAccess ;

	if( iNumArg >= 4 )
	{
		iResult = RuntimePop( vM4Obj ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "SetLoadRange" ) ;

		poAccess = _FindChannel( vM4Obj, "SetLoadRange", m_oState1 ) ;
		DeleteVarString( vM4Obj ) ;

		if( poAccess == NULL )
		{
			DeleteVarString( vNode ) ;
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}
	}

	poNode = _FindNode( poAccess, vNode, "SetLoadRange", m_oState1, M4_TRUE ) ;
	DeleteVarString( vNode ) ;

	if( poNode == NULL )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	iResult = poNode->RecordSet.GetCurrentBlock()->SetRangeSize( iRangeSize ) ;

	if( iResult != M4_SUCCESS )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	iResult = poNode->RecordSet.GetCurrentBlock()->SetRangeStart( iRangeStart ) ;

	if( iResult != M4_SUCCESS )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	RuntimePushDouble( M4_SUCCESS ) ;
	return( M4_SUCCESS ) ;
}


m4return_t ClVM1LDB::SetLoadTimeOut(void)
{

	m4return_t		iResult = M4_SUCCESS ;
	m4uint32_t		iTimeOut = 0 ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iNumArg = 0 ;
	m4VariantType	vNode ;
	m4VariantType	vM4Obj ;
	m4VariantType	vValue ;
	ClAccess		*poAccess = NULL ;
	ClNode			*poNode = NULL ;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF( M4_CH_VM_TRACE_VM_L1, "Trace VM1: " << LogCat << "SetLoadTimeOut" ) ;
#endif


    // Leemos el número de argumentos (pueden ser 1, 2 ó 3)
	iResult = RuntimePopNumber( vValue ) ;
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "SetLoadTimeOut" ) ;

    iNumArg = m4uint32_t( vValue.Data.DoubleData ) ;

	if( iNumArg < 1 )
	{
		// Si hay menos de 1 argumento se borran y se da error
		DUMP_CHLOG_ERRORF( M4_CH_VM_WRONG_PARAM_NUM, "SetLoadTimeOut" << "1-2-3" << DumpCallStack() ) ;

		for( i = 0 ; i < iNumArg ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
		}
		RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}

	// Limpiamos los argumentos de más
	if( iNumArg > 3 )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_TOO_PARAM, "SetLoadTimeOut" << 3 << iNumArg + 1 << DumpCallStack() ) ;

		for( i = 3 ; i < iNumArg ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
			M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "SetLoadTimeOut" ) ;
		}
		iNumArg = 3 ;
	}

	// TimeOut
	iResult = RuntimePop( vValue ) ;
	M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "SetLoadTimeOut" ) ;

	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "TimeOut" << "SetLoadTimeOut" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
		DeleteVarString( vValue ) ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	iTimeOut = m4uint32_t( vValue.Data.DoubleData ) ;

	// Nodo
	if( iNumArg >= 2 )
	{
		iResult = RuntimePop( vNode ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "SetLoadTimeOut" ) ;
	}
	else
	{
		vNode.Type = M4CL_CPP_TYPE_NULL ;
	}

	// Meta4Object
	poAccess = m_oState1.m_poAccess ;

	if( iNumArg >= 3 )
	{
		iResult = RuntimePop( vM4Obj ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "SetLoadTimeOut" ) ;

		poAccess = _FindChannel( vM4Obj, "SetLoadTimeOut", m_oState1 ) ;
		DeleteVarString( vM4Obj ) ;

		if( poAccess == NULL )
		{
			DeleteVarString( vNode ) ;
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}
	}

	poNode = _FindNode( poAccess, vNode, "SetLoadTimeOut", m_oState1, M4_TRUE ) ;
	DeleteVarString( vNode ) ;

	if( poNode == NULL )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	iResult = poNode->RecordSet.GetCurrentBlock()->SetTimeOut( iTimeOut ) ;

	if( iResult != M4_SUCCESS )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	RuntimePushDouble( M4_SUCCESS ) ;
	return( M4_SUCCESS ) ;
}
