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
//    Virtual machine de nivel 1- Funciones relativas a la propagacion
////
//==============================================================================

#include "vm1prop.hpp"


#include "vmres.hpp"
#include "vmachin1.h"

#include "m4log.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "m4mdrt.hpp"
#include "pl_data.hpp"
#include "cldbgutil.hpp"
#include "m4objglb.hpp"
#include "nodedef.hpp"

//---------------------------------------------
//P R O P A G A C I O N
//---------------------------------------------

//*********De registro a bloque
m4return_t ClVM1Propagator::_PropagateRBR_ ( m4bool_t ai_bBlock, m4bool_t ai_bRegister){
    
    m4VMState_t				oCurrentState = m_oState1;
	m4VariantType			vTemp, vNode, vItem;
    m4return_t				ret;
    m4int32_t				iNumArgs;
    const m4char_t			*pszNode, *pszItem;
    ClRecordSet				oRecordSet (oCurrentState.m_poAccess);
    ClAccessRecordSet		*poRecordSet_xxx;
    //ClNodeIndex				oNodeIndex;
    ClRegisterIndex			oRegIndex;
    m4uint32_t				hNodo,hItem;
    m4uint8_t				iSpin;
	m4uint8_t				iCsType;
    m4VariantType			param[M4CL_MAX_PARAM];
    m4int32_t				i;
    m4uint32_t				hThisNode;
    ClRegisterIndex			iThisIndex;
    m4int32_t				n;
    ClAccessRecordSet		*pars;
    ClBlock_Channel_Proxy	*poBlock;    
	    
    
    //número de argumentos
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateRBR_.")	;
    iNumArgs=(m4int32_t)vTemp.Data.DoubleData;
    
    //SPIN
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateRBR_.")	;
    iSpin=(m4uint8_t)vTemp.Data.DoubleData;
    
    //TYPE
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateRBR_.")	;
    iCsType=(m4uint8_t)vTemp.Data.DoubleData;
    
    //nombre de nodo
    ret=RuntimePopString(vNode);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateRBR_.")	;
    pszNode=m_oDynamicPool->GetString(vNode.Data.CharData);
    
    
    //nombre de item
    ret=RuntimePopString(vItem);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateRBR_.")	;
    pszItem=m_oDynamicPool->GetString(vItem.Data.CharData);

    hNodo=oCurrentState.m_poAccess-> GetpCMCR()  -> GetChannelNodeHandleById (pszNode);
	hItem=oCurrentState.m_poAccess-> GetpCMCR()  -> GetNodeItemHandleById (hNodo, pszItem,M4_TRUE);

	m_oDynamicPool->DeleteString (vNode.Data.CharData);
	m_oDynamicPool->DeleteString (vItem.Data.CharData);

    //sacamos los argumentos 
	iNumArgs -= 4 ;
    for (i=0;i<iNumArgs; i++){
        RuntimePop(param[i]);
    }
    
    
    RuntimePushDouble(M4_SUCCESS); //el resultado

	// Se chequea que si es un conector insite no se ejecute si ya se ha ejecutado
	if( M4CL_HAS_STATE_NO_INSITE_FLAG( oCurrentState.m_Flags ) && iCsType == M4CL_CSTYPE_MAPPED )
	{
		return( M4_SUCCESS ) ;
	}

    
    //------- Para poder resituar el current
    hThisNode=oCurrentState.m_poRecordSet->GetpNodeDef ()->GetHandle ();
    DEBUG_CHECK_RES (hThisNode, M4_CH_VM_WRONG_THIS_NODE_HANDLE, ERRORLOG, "_PropagateRBR_" << DumpCallStack() );
    
	pars=&(oCurrentState.m_poAccess->Node.Get (ClHandle (hThisNode))->RecordSet);
    iThisIndex=pars->Current.Index;
    //metemos un newstate para retornar a nuestro sitio---OJO..cosita delicada
    m4VMState_t stTemp;
    m_oStateStack->Top (stTemp); //sacamos el actual
    m_oStateStack->Pop (); //
    
    _GeneratePropMoveTo (oCurrentState.m_poRecordSet, iThisIndex, M4_FALSE, M4_FALSE);
    
    m_oStateStack->Push (stTemp); //metemos el nuestro
    //-----fin de  resituación
    

    if (ai_bBlock && iThisIndex == M4DM_EOF_INDEX) { //si vamos a leer el bloque comprobamos que podamos
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_RECORDS_TO_PROPAGATE, M4ObjNodeN(oCurrentState.m_poAccess->GetpCMCR(),hThisNode)<<DumpTreePath(oCurrentState.m_poAccess, hThisNode) << DumpCallStack() );
        return M4_ERROR;
    }
    
    
    //bloque sobre el que tiene que ejecutarse
    if (ai_bBlock){
        poBlock = pars->Current.GetChildBlock ((ClHandle)hNodo);
    }    
    else{ //tomamos lo que podemos---nodo hijo a trves de la colección RB con bloque nihil 
        poBlock=&g_oNihilBlock;
    }
	
    oRecordSet.SetCurrentBlock(poBlock);
    poRecordSet_xxx = &(oCurrentState.m_poAccess->Node.Get ((ClHandle)hNodo)->RecordSet);
	
	//Nuevo. Estadisticas.
	m4int32_t iParentStatePos=m_oStateStack->GetPos();
	
    //registro ->segun parámetro
    if (ai_bRegister && ai_bBlock){
        
        m4uint32_t iNumReg=oRecordSet.Register.Count();
        //---trigger
        m4uint8_t context, newcontext;
        ClAccessRecordSet *pars;
        if (GetContext(oCurrentState.m_hItem, hItem, oCurrentState.m_poAccess->GetpCMCR() ,context) != M4_SUCCESS) {
            DUMP_CHLOG_ERRORF(M4_CH_VM_NO_TRIGGER_CONTEXT, M4ObjNodeItemI(oCurrentState.m_poAccess->GetpCMCR(),oCurrentState.m_hItem)<<M4ObjNodeItemI(oCurrentState.m_poAccess->GetpCMCR(),hItem) << DumpCallStack() );
            return M4_ERROR;
        }
        
        switch (context){
        case M4CL_VM_TRIGGER_ALL:			
			for (n=iNumReg-1; n>=0; --n){
                ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs,  poRecordSet_xxx,  n,iParentStatePos, param,oCurrentState);
				if (ret != M4_SUCCESS) return ret;
                _GeneratePropMoveTo (poRecordSet_xxx, n, M4_TRUE, M4_FALSE);
            }
            
            break;
        case M4CL_VM_TRIGGER_CURRENT: //para el current 
            pars = poRecordSet_xxx;
            ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs,  poRecordSet_xxx,  pars->Current.Index,iParentStatePos, param,oCurrentState);
			if (ret != M4_SUCCESS) return ret;
            break;
        default: //todos los demás casos los tratamos de una tacada
			for (n=iNumReg-1; n>=0; --n){
                newcontext=0;
                newcontext=oRecordSet.Register[n].Operation.IsNew();
                newcontext|= (m4uint8_t)oRecordSet.Register.Operation.IsUpdatedByDDBB()<<1;
                newcontext|= (m4uint8_t)oRecordSet.Register.Operation.IsDeleted()<<2;
				//excepciones
				if (oRecordSet.Register.Operation.IsDeleted() && oRecordSet.Register.Operation.IsNew() ){
					//ambas->no hacer nada
					newcontext=0;
				}
				if (oRecordSet.Register.Operation.IsDeleted() && oRecordSet.Register.Operation.IsUpdatedByDDBB() ){
					//ambas->consideramos solo borrado
					newcontext= M4CL_VM_TRIGGER_DELETED;
				}

                if (newcontext & context){
                    ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs,  poRecordSet_xxx,  n,iParentStatePos, param,oCurrentState);
					if (ret != M4_SUCCESS) return ret;
                    _GeneratePropMoveTo (poRecordSet_xxx, n, M4_TRUE, M4_FALSE);
                    
                }
            }
            break;
            
        }
        
    }
    else{
        oRegIndex = M4DM_EOF_INDEX;
        ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs,  poRecordSet_xxx, oRegIndex,iParentStatePos, param,oCurrentState);
		if (ret != M4_SUCCESS) return ret;
       
    }
    
    
    //---- ESte movimiento que se hacía no servía para nada (nos moviamos donde ya estabamos)
    //_GeneratePropMoveTo (oCurrentState.m_poRecordSet, iThisIndex, M4_FALSE, M4_FALSE);
    //----
    
    return ret;
    
}

m4return_t ClVM1Propagator::PropagateRBRR(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateRBRR");
#endif
    return _PropagateRBR_(M4_TRUE,M4_TRUE);	
}


m4return_t ClVM1Propagator::PropagateRBRB(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateRBRB");
#endif
    return _PropagateRBR_(M4_TRUE,M4_FALSE);	
}


m4return_t ClVM1Propagator::PropagateRBRN(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateRBRN");
#endif
    return _PropagateRBR_(M4_FALSE,M4_FALSE);	
    
}

//--------------------------------------------------

m4return_t ClVM1Propagator::_PropagateRBB_ ( m4bool_t ai_bBlock, m4bool_t ai_bRegister){
    
    m4VMState_t				oCurrentState = m_oState1;
    m4int32_t				num_reg;
    m4VariantType			vTemp, vNode, vItem;
    m4return_t				ret;
    m4int32_t				iNumArgs;
    const m4char_t			*pszNode, *pszItem;
    ClRegister				oRegister (oCurrentState.m_poAccess);
    ClAccessRecordSet		*poRecordSet_xxx;
    //ClNodeIndex				oNodeIndex;
    ClRegisterIndex			oRegIndex;
    m4uint32_t				hNodo,hItem;
    m4VariantType			param[M4CL_MAX_PARAM];
    m4uint8_t				iSpin;
	m4uint8_t				iCsType;
    m4int32_t				i, IniFor, EndFor;
    m4uint32_t				hThisNode;
    ClRegisterIndex			iThisIndex;
    m4int32_t				n;	
    ClBlock_Channel_Proxy	*poBlock;    
    m4bool_t				bUseMM;
    m4bool_t				bPartialLoad;
	
    
    //número de argumentos
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateRBB_")	;
    iNumArgs=(m4int32_t)vTemp.Data.DoubleData;
    
    //SPIN
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateRBB_.")	;
    iSpin=(m4uint8_t)vTemp.Data.DoubleData;

    //TYPE
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateRBR_.")	;
    iCsType=(m4uint8_t)vTemp.Data.DoubleData;
    
    //nombre de nodo
    ret=RuntimePopString(vNode);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateRBB_.")	;
    pszNode=m_oDynamicPool->GetString(vNode.Data.CharData);
    
    
    //nombre de item
    ret=RuntimePopString(vItem);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateRBB_.")	;
    pszItem=m_oDynamicPool->GetString(vItem.Data.CharData);

    hNodo=oCurrentState.m_poAccess-> GetpCMCR()  -> GetChannelNodeHandleById (pszNode);
	hItem=oCurrentState.m_poAccess-> GetpCMCR()  -> GetNodeItemHandleById (hNodo, pszItem,M4_TRUE);
    
	m_oDynamicPool->DeleteString (vNode.Data.CharData);
	m_oDynamicPool->DeleteString (vItem.Data.CharData);

    //sacamos los argumentos para meter tantas copias como llamadas
    //tengamos que hacer
	iNumArgs -= 4 ;
    for (i=0;i<iNumArgs; i++){
        RuntimePop(param[i]);
    }
    
    RuntimePushDouble(M4_SUCCESS); //el resultado

	// Se chequea que si es un conector insite no se ejecute si ya se ha ejecutado
	if( M4CL_HAS_STATE_NO_INSITE_FLAG( oCurrentState.m_Flags ) && iCsType == M4CL_CSTYPE_MAPPED )
	{
		return( M4_SUCCESS ) ;
	}


    //------- Para poder resituar el current
    hThisNode=oCurrentState.m_poRecordSet->GetpNodeDef ()->GetHandle ();
    DEBUG_CHECK_RES (hThisNode, M4_CH_VM_WRONG_THIS_NODE_HANDLE, ERRORLOG, "_PropagateRBB_" << DumpCallStack() );
	
    iThisIndex=oCurrentState.m_poAccess->Node.Get (ClHandle (hThisNode))->RecordSet.Current.Index;
    
    //metemos un newstate para retornar a nuestro sitio---OJO..cosita delicada
    m4VMState_t stTemp;
    m_oStateStack->Top (stTemp); //sacamos el actual
    m_oStateStack->Pop (); //
    _GeneratePropMoveTo (oCurrentState.m_poRecordSet, iThisIndex, M4_FALSE, M4_FALSE);
    m_oStateStack->Push (stTemp); //metemos el nuestro
    //-----fin de  resituación
    

    //bloque sobre el que tiene que ejecutarse
    
    m4int32_t First, Last;

    if (oCurrentState.m_poRecordSet->GetpNodeDef()->NumRows())
    {
        First = oCurrentState.m_poRecordSet->GetCurrentBlock()->GetpPL_Data()->GetFirstToRead();
        Last = First + oCurrentState.m_poRecordSet->GetCurrentBlock()->GetpPL_Data()->GetReadRegister();
		bUseMM = M4_FALSE; //Si hay carga parcial no puede tener filtros->El índice normal vale, no se puede usar el mmindex
		bPartialLoad = M4_TRUE ;
    }
    else
    {
        num_reg = oCurrentState.m_poRecordSet->Count(); //como vamos a usar indices físicos, el count se lo pedimos al rs, que da los físicos, no al register que da los lógicos
        First = 0;
        Last = num_reg;
		bUseMM = M4_TRUE;
		bPartialLoad = M4_FALSE ;
    }

    
    if (iSpin==M4CL_ITEM_CONNECTOR_DOWNWARDS){
        IniFor = -Last + 1;
        EndFor = -First + 1;
        
    }
    else{ //downwards
        IniFor= First;
        EndFor= Last;
    }
    
   
	//Nuevo. Estadisticas.
	m4int32_t iParentStatePos=m_oStateStack->GetPos();

    for (i=IniFor; i < EndFor; i++){
        
        
        //bloque sobre el que tiene que ejecutarse
        if (ai_bBlock){
			if (bUseMM){
				oCurrentState.m_poRecordSet->Register.MoveToMM (abs((m4int_t)i)); //bloque hijo, pero moviendonos según registro físico
				poBlock = oCurrentState.m_poRecordSet->Register.GetChildBlock ((ClHandle)hNodo);
			}
			else{//según índice lógico
				poBlock = oCurrentState.m_poRecordSet->Register[abs((m4int_t)i)].GetChildBlock ((ClHandle)hNodo);
			}

        }
        else{
            poBlock=&g_oNihilBlock;
        }
        oRegister.SetCurrentBlock(poBlock);
        poRecordSet_xxx = &(oCurrentState.m_poAccess->Node.Get ((ClHandle)hNodo)->RecordSet);
        //registro ->segun argumento
		
        if (ai_bRegister && ai_bBlock){
            m4uint32_t iNumReg=oRegister.Count(); //Como es un objeto local, no tiene filtros, y contará los registros sin tener en cuenta el filtro
            //---trigger
            m4uint8_t context, newcontext;
            ClAccessRecordSet *pars;
            if (GetContext(oCurrentState.m_hItem, hItem, oCurrentState.m_poAccess->GetpCMCR(), context) != M4_SUCCESS) {
	            DUMP_CHLOG_ERRORF(M4_CH_VM_NO_TRIGGER_CONTEXT, M4ObjNodeItemI(oCurrentState.m_poAccess->GetpCMCR(),oCurrentState.m_hItem)<<M4ObjNodeItemI(oCurrentState.m_poAccess->GetpCMCR(),hItem) << DumpCallStack() );
                return M4_ERROR;
            }
            
            switch (context){
            case M4CL_VM_TRIGGER_ALL:
				for (n=iNumReg-1; n>=0; --n){
                    ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs,  poRecordSet_xxx,  n, iParentStatePos, param,oCurrentState);
					if (ret != M4_SUCCESS) return ret;
                    _GeneratePropMoveTo (poRecordSet_xxx, n, M4_TRUE, M4_FALSE);
                }
                
                break;
            case M4CL_VM_TRIGGER_CURRENT: //para el current 
                pars = poRecordSet_xxx;
                ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs,  poRecordSet_xxx,  pars->Current.Index,iParentStatePos, param,oCurrentState);
				if (ret != M4_SUCCESS) return ret;
                break;
            default: //todos los demás casos los tratamos de una tacada
				for (n=iNumReg-1; n>=0; --n){
                    newcontext=0;
                    newcontext=oRegister[n].Operation.IsNew();
                    newcontext|= (m4uint8_t)oRegister.Operation.IsUpdatedByDDBB()<<1;
                    newcontext|= (m4uint8_t)oRegister.Operation.IsDeleted()<<2;
					//excepciones
					if (oRegister.Operation.IsDeleted() && oRegister.Operation.IsNew() ){
						//ambas->no hacer nada
						newcontext=0;
					}
					if (oRegister.Operation.IsDeleted() && oRegister.Operation.IsUpdatedByDDBB() ){
						//ambas->consideramos solo borrado
						newcontext= M4CL_VM_TRIGGER_DELETED;
					}

                    if (newcontext & context){
                        ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs,  poRecordSet_xxx,  n,iParentStatePos, param,oCurrentState);
						if (ret != M4_SUCCESS) return ret;
                        _GeneratePropMoveTo (poRecordSet_xxx, n, M4_TRUE, M4_FALSE);
                    }
                }
                break;
                
            }
            
        }
        
        else{
            oRegIndex = M4DM_EOF_INDEX;
            ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs, poRecordSet_xxx,  oRegIndex,iParentStatePos, param ,oCurrentState);
			if (ret != M4_SUCCESS) return ret;
        }
        
        
        //----
        //metemos el de movernos al registro cuyo hijo vamos a tratar
		/* Bug 0094704
		Para el registro actual de carga parcial hay que forzar el posicionado de los hijos
		*/
        _GeneratePropMoveTo (oCurrentState.m_poRecordSet, abs((m4int_t)i), bUseMM, ( i == m4int32_t( iThisIndex ) ) & bPartialLoad) ;
        //----
        
    }
	
	
    return ret;
    
}

m4return_t ClVM1Propagator::PropagateRBBR(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateRBBR");
#endif
    return _PropagateRBB_ (M4_TRUE, M4_TRUE);
}


m4return_t ClVM1Propagator::PropagateRBBB(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateRBBB");
#endif
    return _PropagateRBB_ ( M4_TRUE, M4_FALSE);
    
}


m4return_t ClVM1Propagator::PropagateRBBN(void){
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateRBBN");
#endif
    return PropagateRBRN (); 
    
}



//-----------------


m4return_t ClVM1Propagator::PropagateRBNR(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateRBNR");
#endif
	
	//La propagación con padre nodo solo sirve para nodos monobloques
	if (m_oState1.m_poRecordSet->GetpNodeDef ()->Capacity() !=M4CL_NODE_CAPACITY_TYPE_MONOBLOCK  ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_INVALID_PROPAGATION_CALL, M4ObjNodeN (m_oState1.m_poAccess->GetpCMCR(), m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle() ) << DumpCallStack() );
		return M4_ERROR;
	}
	//como solo tiene un bloque->vamos por la de bloque
    return _PropagateRBB_ ( M4_TRUE, M4_TRUE);
    
}


m4return_t ClVM1Propagator::PropagateRBNB(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateRBNB");
#endif
 	//La propagación con padre nodo solo sirve para nodos monobloques
	if (m_oState1.m_poRecordSet->GetpNodeDef ()->Capacity() !=M4CL_NODE_CAPACITY_TYPE_MONOBLOCK  ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_INVALID_PROPAGATION_CALL, M4ObjNodeN (m_oState1.m_poAccess->GetpCMCR(), m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle() ) << DumpCallStack() );
		return M4_ERROR;
	}
	//como solo tiene un bloque->vamos por la de bloque
    return _PropagateRBB_ ( M4_TRUE, M4_FALSE);
    
}

m4return_t ClVM1Propagator::PropagateRBNN(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateRBNN");
#endif
    return PropagateRBRN ();
    
    
}



//---------------------------------------------------------
//*********De Bloque a bloque
//---------------------------------------------------------

m4return_t ClVM1Propagator::_PropagateBBB_ ( m4bool_t ai_bBlock, m4bool_t ai_bRegister){
    
    m4VMState_t				oCurrentState = m_oState1;
    m4VariantType			vTemp, vNode, vItem;
    m4return_t				ret;
    m4int32_t				iNumArgs;
    const m4char_t			*pszNode, *pszItem;
    ClRecordSet				oRecordSet (oCurrentState.m_poAccess);
    ClAccessRecordSet		*poRecordSet_xxx;
    //ClNodeIndex				oNodeIndex;
    ClRegisterIndex			oRegIndex;
    m4uint32_t				hNodo,hItem;
    m4uint8_t				iSpin;
	m4uint8_t				iCsType;
    m4VariantType			param[M4CL_MAX_PARAM];
    m4int32_t				i;
    m4uint32_t				hThisNode;
    ClRegisterIndex			iThisIndex;
    m4int32_t				n;
	ClBlock_Channel_Proxy	*poBlock;
    
    
    //número de argumentos
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateBBB_.")	;
    iNumArgs=(m4int32_t)vTemp.Data.DoubleData;
    
    //SPIN
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateBBB_.")	;
    iSpin=(m4uint8_t)vTemp.Data.DoubleData;
    
    //TYPE
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateRBR_.")	;
    iCsType=(m4uint8_t)vTemp.Data.DoubleData;
    
    //nombre de nodo
    ret=RuntimePopString(vNode);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateBBB_.")	;
    pszNode=m_oDynamicPool->GetString(vNode.Data.CharData);
    
    
    //nombre de item
    ret=RuntimePopString(vItem);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateBBB_.")	;
    pszItem=m_oDynamicPool->GetString(vItem.Data.CharData);

    hNodo=oCurrentState.m_poAccess-> GetpCMCR()  -> GetChannelNodeHandleById (pszNode);
	hItem=oCurrentState.m_poAccess-> GetpCMCR()  -> GetNodeItemHandleById (hNodo, pszItem,M4_TRUE);
    
	m_oDynamicPool->DeleteString (vNode.Data.CharData);
	m_oDynamicPool->DeleteString (vItem.Data.CharData);

    //sacamos los argumentos 
	iNumArgs -= 4 ;
    for (i=0;i<iNumArgs; i++){
        RuntimePop(param[i]);
    }
    
    
    RuntimePushDouble(M4_SUCCESS); //el resultado

	// Se chequea que si es un conector insite no se ejecute si ya se ha ejecutado
	if( M4CL_HAS_STATE_NO_INSITE_FLAG( oCurrentState.m_Flags ) && iCsType == M4CL_CSTYPE_MAPPED )
	{
		return( M4_SUCCESS ) ;
	}
    
    
    //------- Para poder resituar el current
    hThisNode=oCurrentState.m_poRecordSet->GetpNodeDef ()->GetHandle ();
    DEBUG_CHECK_RES (hThisNode, M4_CH_VM_WRONG_THIS_NODE_HANDLE, ERRORLOG, "_PropagateBBB_" << DumpCallStack() );
    iThisIndex=oCurrentState.m_poAccess->Node.Get (ClHandle (hThisNode))->RecordSet.Current.Index;
    //metemos un newstate para retornar a nuestro sitio---OJO..cosita delicada
    m4VMState_t stTemp;
    m_oStateStack->Top (stTemp); //sacamos el actual
    m_oStateStack->Pop (); //
    
    _GeneratePropMoveTo (oCurrentState.m_poRecordSet, iThisIndex, M4_FALSE, M4_FALSE);
    
    m_oStateStack->Push (stTemp); //metemos el nuestro
    //-----fin de  resituación
    
    
    //bloque sobre el que tiene que ejecutarse
    if (oCurrentState.m_poRecordSet->IsValid() ){
        if (ai_bBlock){
            poBlock = oCurrentState.m_poRecordSet->GetChildBlock ((ClHandle)hNodo);
        }
        else{ //usamos la colección BB
            poBlock=&g_oNihilBlock;
        }
        oRecordSet.SetCurrentBlock(poBlock);
        poRecordSet_xxx = &(oCurrentState.m_poAccess->Node.Get ((ClHandle)hNodo)->RecordSet);
        
    }
    else{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_BLOCKS_TO_PROPAGATE, M4ObjNodeN(oCurrentState.m_poAccess->GetpCMCR(), oCurrentState.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        return M4_ERROR;
    }
    
#ifdef _DEBUG
	if (poBlock==NULL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK,"PropagateBBB"<<pszNode<<DumpTreePath(oCurrentState.m_poAccess, hNodo) << DumpCallStack() );
		return M4_ERROR;
	}
#endif

	//Nuevo. Estadisticas.
	m4int32_t iParentStatePos=m_oStateStack->GetPos();
	
    //registro ->segun arg
    if (ai_bRegister && ai_bBlock){
        
        m4uint32_t iNumReg=oRecordSet.Register.Count();
        //---trigger
        m4uint8_t context, newcontext;
        ClAccessRecordSet *pars;
        if (GetContext(oCurrentState.m_hItem, hItem, oCurrentState.m_poAccess->GetpCMCR(), context) != M4_SUCCESS) {
            DUMP_CHLOG_ERRORF(M4_CH_VM_NO_TRIGGER_CONTEXT, M4ObjNodeItemI(oCurrentState.m_poAccess->GetpCMCR(),oCurrentState.m_hItem)<<M4ObjNodeItemI(oCurrentState.m_poAccess->GetpCMCR(),hItem) << DumpCallStack() );
            return M4_ERROR;
        }
        
        switch (context){
        case M4CL_VM_TRIGGER_ALL:
			for (n=iNumReg-1; n>=0; --n){
                ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs,  poRecordSet_xxx,  n,iParentStatePos, param,oCurrentState);
				if (ret != M4_SUCCESS) return ret;
                _GeneratePropMoveTo ( poRecordSet_xxx, n, M4_TRUE, M4_FALSE);
            }
            
            break;
        case M4CL_VM_TRIGGER_CURRENT: //para el current 
            pars = poRecordSet_xxx;
            ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs,  poRecordSet_xxx,  pars->Current.Index,iParentStatePos, param,oCurrentState);
			if (ret != M4_SUCCESS) return ret;
            break;
        default: //todos los demás casos los tratamos de una tacada
			for (n=iNumReg-1; n>=0; --n){
                newcontext=0;
                newcontext=oRecordSet.Register[n].Operation.IsNew();
                newcontext|= (m4uint8_t)oRecordSet.Register.Operation.IsUpdatedByDDBB()<<1;
                newcontext|= (m4uint8_t)oRecordSet.Register.Operation.IsDeleted()<<2;
				//excepciones
				if (oRecordSet.Register.Operation.IsDeleted() && oRecordSet.Register.Operation.IsNew() ){
					//ambas->no hacer nada
					newcontext=0;
				}
				if (oRecordSet.Register.Operation.IsDeleted() && oRecordSet.Register.Operation.IsUpdatedByDDBB() ){
					//ambas->consideramos solo borrado
					newcontext= M4CL_VM_TRIGGER_DELETED;
				}

                if (newcontext & context){
                    ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs, poRecordSet_xxx,  n,iParentStatePos, param,oCurrentState);
					if (ret != M4_SUCCESS) return ret;
                    _GeneratePropMoveTo (poRecordSet_xxx, n, M4_TRUE, M4_FALSE);
                }
            }
            break;
            
        }
        
        
    }
    else{
        oRegIndex = M4DM_EOF_INDEX;
        ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs, poRecordSet_xxx,  oRegIndex,iParentStatePos , param,oCurrentState);
		if (ret != M4_SUCCESS) return ret;
    }
    
	
    return ret;
    
    
}


m4return_t ClVM1Propagator::PropagateBBBR(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateBBBR");
#endif
    return _PropagateBBB_ (M4_TRUE, M4_TRUE);
}


m4return_t ClVM1Propagator::PropagateBBBB(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateBBBB");
#endif
    return _PropagateBBB_ ( M4_TRUE, M4_FALSE);
}


m4return_t ClVM1Propagator::PropagateBBBN(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateBBBN");
#endif
    return PropagateBBBB ();
}


//-----------------------------------------------------------
//-----------------------------------------------------------
m4return_t ClVM1Propagator::PropagateBBRR(void){
    //igual que BR
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: Propagate BBRR");
#endif
    return PropagateBBBR();
}


m4return_t ClVM1Propagator::PropagateBBRB(void){
    //igual que BB
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: Propagate BBRB");
#endif
    return PropagateBBBB();
}


m4return_t ClVM1Propagator::PropagateBBRN(void){
    //igual que BN
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: Propagate BBRN");
#endif
    return PropagateBBBN();
}



m4return_t ClVM1Propagator::PropagateBBNR(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateBBNR");
#endif

	//La propagación con padre nodo solo sirve para nodos monobloques
	if (m_oState1.m_poRecordSet->GetpNodeDef ()->Capacity() !=M4CL_NODE_CAPACITY_TYPE_MONOBLOCK  ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_INVALID_PROPAGATION_CALL, M4ObjNodeN (m_oState1.m_poAccess->GetpCMCR(), m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle() ) << DumpCallStack() );
		return M4_ERROR;
	}
	//como solo tiene un bloque->vamos por la de bloque

    return _PropagateBBB_( M4_TRUE, M4_TRUE);
}


m4return_t ClVM1Propagator::PropagateBBNB(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateBBNB");
#endif

	//La propagación con padre nodo solo sirve para nodos monobloques
	if (m_oState1.m_poRecordSet->GetpNodeDef ()->Capacity() !=M4CL_NODE_CAPACITY_TYPE_MONOBLOCK  ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_INVALID_PROPAGATION_CALL, M4ObjNodeN (m_oState1.m_poAccess->GetpCMCR(), m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle() ) << DumpCallStack() );
		return M4_ERROR;
	}
	//como solo tiene un bloque->vamos por la de bloque
	return _PropagateBBB_(M4_TRUE, M4_FALSE);
}


m4return_t ClVM1Propagator::PropagateBBNN(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateBBNN");
#endif
    return PropagateBBBB( );
}

//----------------------------
//*********De ASOCIACIÓN LIBRE
//----------------------------
m4return_t ClVM1Propagator::_PropagateFree_ ( m4bool_t ai_bBlock, m4bool_t ai_bRegister){
    
    m4VMState_t				oCurrentState = m_oState1;
   	m4VariantType			vTemp, vNode, vItem;
    m4return_t				ret;
    m4int32_t				iNumArgs;
    const m4char_t			*pszNode, *pszItem;
    ClRecordSet				oRecordSet (oCurrentState.m_poAccess);
    ClAccessRecordSet		*poRecordSet_xxx;

    //ClNodeIndex				oNodeIndex;
    ClRegisterIndex			oRegIndex;
    m4uint32_t				hNodo,hItem;
    m4uint8_t				iSpin;
	m4uint8_t				iCsType;
    m4VariantType			param[M4CL_MAX_PARAM];
    m4int32_t				i;
    m4uint32_t				hThisNode;
    ClRegisterIndex			iThisIndex;
    m4int32_t				n;
	ClBlock_Channel_Proxy	*poBlock;
	m4bool_t				bUseMM;
    m4bool_t				bPartialLoad;
    
    
    //número de argumentos
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateFree_.")	;
    iNumArgs=(m4int32_t)vTemp.Data.DoubleData;
    
    //SPIN
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateFree_.")	;
    iSpin=(m4uint8_t)vTemp.Data.DoubleData;
    
    //TYPE
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateRBR_.")	;
    iCsType=(m4uint8_t)vTemp.Data.DoubleData;
    
    //nombre de nodo
    ret=RuntimePopString(vNode);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateFree_.")	;
    pszNode=m_oDynamicPool->GetString(vNode.Data.CharData);
    
    
    //nombre de item
    ret=RuntimePopString(vItem);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "_PropagateFree_.")	;
    pszItem=m_oDynamicPool->GetString(vItem.Data.CharData);

    hNodo=oCurrentState.m_poAccess-> GetpCMCR()  -> GetChannelNodeHandleById (pszNode);
	hItem=oCurrentState.m_poAccess-> GetpCMCR()  -> GetNodeItemHandleById (hNodo, pszItem,M4_TRUE);

	m_oDynamicPool->DeleteString (vNode.Data.CharData);
	m_oDynamicPool->DeleteString (vItem.Data.CharData);

    //sacamos los argumentos 
	iNumArgs -= 4 ;
    for (i=0;i<iNumArgs; i++){
        RuntimePop(param[i]);
    }
    
    
    RuntimePushDouble(M4_SUCCESS); //el resultado

	// Se chequea que si es un conector insite no se ejecute si ya se ha ejecutado
	if( M4CL_HAS_STATE_NO_INSITE_FLAG( oCurrentState.m_Flags ) && iCsType == M4CL_CSTYPE_MAPPED )
	{
		return( M4_SUCCESS ) ;
	}

    
    //------- Para poder resituar el current
    hThisNode=oCurrentState.m_poRecordSet->GetpNodeDef ()->GetHandle ();
    DEBUG_CHECK_RES (hThisNode, M4_CH_VM_WRONG_THIS_NODE_HANDLE, ERRORLOG, "_PropagateFree_" << DumpCallStack() );
    iThisIndex=oCurrentState.m_poAccess->Node.Get (ClHandle (hThisNode))->RecordSet.Current.Index;
    //metemos un newstate para retornar a nuestro sitio---OJO..cosita delicada
    m4VMState_t stTemp;
    m_oStateStack->Top (stTemp); //sacamos el actual
    m_oStateStack->Pop (); //
    
    _GeneratePropMoveTo (oCurrentState.m_poRecordSet, iThisIndex, M4_FALSE, M4_FALSE);
    m_oStateStack->Push (stTemp); //metemos el nuestro
    //-----fin de  resituación
    
    
    //bloque sobre el que tiene que ejecutarse

    //puente para que si la propag. es de un nodo a el mismo no tome los current
    //sino los que tenemos
    if (hNodo==hThisNode) { //estamos sobre nosotros mismos
        poBlock     =oCurrentState.m_poRecordSet->GetCurrentBlock(); //usamos el mismo bloque
        oRecordSet.SetCurrentBlock (poBlock);

        poRecordSet_xxx=oCurrentState.m_poRecordSet; //mismo RS
    }    
    else{
        if (ai_bBlock){
            poRecordSet_xxx = &(oCurrentState.m_poAccess->Node.Get (ClHandle(hNodo))->RecordSet);
            poBlock = poRecordSet_xxx->GetCurrentBlock ();
            oRecordSet.SetCurrentBlock (poBlock);
        }
        else{ //NUNCA SE LLAMARA POR AQUI--_
            poBlock=NULL;
            poRecordSet_xxx=NULL;
        }
    }
	
	//Nuevo. Estadisticas.
	m4int32_t iParentStatePos=m_oStateStack->GetPos();
	
    
    //registro ->segun corresponda
    if (ai_bRegister && ai_bBlock){
        
		m4int32_t First, Last;

		if (oCurrentState.m_poRecordSet->GetpNodeDef()->NumRows()){

			First = poRecordSet_xxx->GetCurrentBlock()->GetpPL_Data()->GetFirstToRead();
			Last = First + poRecordSet_xxx->GetCurrentBlock()->GetpPL_Data()->GetReadRegister();
			bUseMM = M4_FALSE; //Si hay carga parcial no puede tener filtros->El índice normal vale, no se puede usar el mmindex
			bPartialLoad = M4_TRUE ;
		}
		else{
    
			m4uint32_t iNumReg=oRecordSet.Count();//como vamos a usar indices físicos, el count se lo pedimos al rs, que da los físicos, no al register que da los lógicos
			First = 0;
			Last = iNumReg;
			bUseMM = M4_TRUE;
			bPartialLoad = M4_FALSE ;
		}

        //---trigger
        m4uint8_t context, newcontext;
        ClAccessRecordSet *pars;
        if (GetContext(oCurrentState.m_hItem, hItem, oCurrentState.m_poAccess->GetpCMCR(), context) != M4_SUCCESS) {
            DUMP_CHLOG_ERRORF(M4_CH_VM_NO_TRIGGER_CONTEXT, M4ObjNodeItemI(oCurrentState.m_poAccess->GetpCMCR(),oCurrentState.m_hItem)<<M4ObjNodeItemI(oCurrentState.m_poAccess->GetpCMCR(),hItem) << DumpCallStack() );
            return M4_ERROR;
        }
        
        switch (context){
        case M4CL_VM_TRIGGER_ALL:
			for (n=Last-1; n>=First; --n){
                ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs,  poRecordSet_xxx, n,iParentStatePos, param,oCurrentState);
				if (ret != M4_SUCCESS) return ret;

				/* Bug 0094704
				Para el registro actual de carga parcial hay que forzar el posicionado de los hijos
				*/
                _GeneratePropMoveTo (poRecordSet_xxx, n, bUseMM, ( n == m4int32_t( iThisIndex ) ) & bPartialLoad);
            }
            
            break;
        case M4CL_VM_TRIGGER_CURRENT: //para el current 
            pars = poRecordSet_xxx;
            ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs,  poRecordSet_xxx,  pars->Current.Index,iParentStatePos, param,oCurrentState);
			if (ret != M4_SUCCESS) return ret;
            break;
        default: //todos los demás casos los tratamos de una tacada
			for (n=Last-1; n>=First; --n){
                newcontext=0;
                newcontext=oRecordSet.Register[n].Operation.IsNew();
                newcontext|= (m4uint8_t)oRecordSet.Register.Operation.IsUpdatedByDDBB()<<1;
                newcontext|= (m4uint8_t)oRecordSet.Register.Operation.IsDeleted()<<2;
				//excepciones
				if (oRecordSet.Register.Operation.IsDeleted() && oRecordSet.Register.Operation.IsNew() ){
					//ambas->no hacer nada
					newcontext=0;
				}
				if (oRecordSet.Register.Operation.IsDeleted() && oRecordSet.Register.Operation.IsUpdatedByDDBB() ){
					//ambas->consideramos solo borrado
					newcontext= M4CL_VM_TRIGGER_DELETED;
				}

                if (newcontext & context){
                    ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs, poRecordSet_xxx,  n,iParentStatePos, param,oCurrentState);
					if (ret != M4_SUCCESS) return ret;

					/* Bug 0094704
					Para el registro actual de carga parcial hay que forzar el posicionado de los hijos
					*/
                    _GeneratePropMoveTo (poRecordSet_xxx, n, bUseMM, ( n == m4int32_t( iThisIndex ) ) & bPartialLoad);
                }
            }
            break;
            
        }
        
        
    }
    else{
        oRegIndex = M4DM_EOF_INDEX;
        ret=AddPropagateMethodToExecute(hNodo, hItem, iNumArgs, poRecordSet_xxx, oRegIndex,iParentStatePos , param,oCurrentState);
		if (ret != M4_SUCCESS) return ret;
    }
    
    //}
    
    
    
    
    return ret;
    
}

m4return_t ClVM1Propagator::PropagateFreeRR(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateFreeRR");
#endif
    return _PropagateFree_ ( M4_TRUE, M4_TRUE);
}


m4return_t ClVM1Propagator::PropagateFreeRB(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateFreeRB");
#endif
    return _PropagateFree_ (M4_TRUE, M4_FALSE);
    
}


m4return_t ClVM1Propagator::PropagateFreeRN(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: PropagateFreeRN");
#endif
    return _PropagateFree_ (M4_TRUE, M4_FALSE); //tambien podria ser return _PropagateFree_ ("RN", M4_FALSE, M4_FALSE);
    
}



m4return_t ClVM1Propagator::PropagateFreeBR(void){
    
    //todas la xR son iguales
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: Propagate FreeBR");
#endif
    return PropagateFreeRR();
}


m4return_t ClVM1Propagator::PropagateFreeBB(void){
    
    //todas la xB son iguales
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: Propagate FreeBB");
#endif
    return PropagateFreeRB();
}


m4return_t ClVM1Propagator::PropagateFreeBN(void){
    
    //todas la xN son iguales
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: Propagate FreeBN");
#endif
    return PropagateFreeRN();
}

m4return_t ClVM1Propagator::PropagateFreeNR(void){
    
    //todas la xR son iguales
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: Propagate FreeNR");
#endif
    return PropagateFreeRR();
}

m4return_t ClVM1Propagator::PropagateFreeNB(void){
    
    //todas la xB son iguales
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: Propagate FreeNB");
#endif
    return PropagateFreeRB();
    
}

m4return_t ClVM1Propagator::PropagateFreeNN(void){
    
    //todas la xN son iguales
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: Propagate FreeNN");
#endif
    return PropagateFreeRN();
}




//-------------------------------------------------------------------------
//Funciones auxiliares
//-------------------------------------------------------------------------
m4return_t ClVM1Propagator::GetContext (m4uint32_t ai_hParent, m4uint32_t ai_hChild,  ClCompiledMCR   *ai_poCMCR, m4uint8_t &ao_context){
    
    m4uint32_t iNumConnected=ai_poCMCR->GetItemNumberOfConnectedItems(ai_hParent);
    m4uint32_t i, hConnector, hChild;
    m4return_t ret=M4_ERROR;
    
    for (i=0; i<iNumConnected; ++i){
        
        hConnector=ai_poCMCR->GetItemConnectorHandleByPosition(ai_hParent, (m4uint16_t)i);
        hChild=ai_poCMCR->GetItemConnectorSonItemHandle (hConnector);
        if (hChild==ai_hChild){
            ret=M4_SUCCESS;
            ao_context=ai_poCMCR->GetItemConnectorContext (hConnector);
            break;
        }
        
    }
    return ret;
}


//---------------------------------------------------------------
//Añadir método de propagación a la pila de ejecución
//---------------------------------------------------------------

m4return_t ClVM1Propagator::AddPropagateMethodToExecute(m4uint32_t ai_hNodo, 
													  m4uint32_t ai_hItem, 
													  m4uint16_t ai_iNumArgs,
													  ClAccessRecordSet *ai_RecordSet,
													  ClRegisterIndex ai_RegIndex,
													  m4int32_t ai_iParentStatePos, 
													  m4VariantType *ai_param,
													  m4VMState_t &ai_oParentState){
	
	m4VMState_t			oState;
	m4return_t			ret=M4_SUCCESS;
	m4uint16_t			iRuleOrder, iSlicePosition;
    m4uint8_t           iVarArgs ;
	m4pcvoid_t			hFormula;
	m4uint32_t jp;	

#if _M4_DEBUG > 10
     DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_PROPAGATION, "Trace Propagation: Create execution of propagated item:" <<LogCat<< ai_hNodo <<LogCat<< "." <<LogCat<< ai_pszItem <<LogCat<< ". Arguments=" <<LogCat<< ai_iNumArgs );
#endif
	//metemos loas argumentos
	for (jp = ai_iNumArgs ; jp>0; jp--){ 
		if (ai_param[jp-1].Type ==M4CL_CPP_TYPE_STRING_POOL){ //Si es cadena la copiamos otra vez
			m_oDynamicPool->AddString( m_oDynamicPool->GetString(ai_param[jp-1].Data.CharData), ai_param[jp-1].Data.CharData);
		}
		RuntimePush(ai_param[jp-1]);
	}
	
	
    ClCompiledMCR   *poCMCR=ai_RecordSet->GetpAccess()-> GetpCMCR() ;
    
	iRuleOrder=0; //metaregla

	
	//si es de argumentos variables, en la pila de RT
	//metemos el númerode argumentos
    iVarArgs = poCMCR -> GetItemVariableArguments(ai_hItem) ;
	if (iVarArgs != 0 ){ // numero variable de args
		RuntimePushDouble(ai_iNumArgs);
	}
	
	//**Metemos el estado
	
	//guardamos en el estado la referencia vieja de la pila 
	oState.m_iOldStackReference=m_oRunTimeStack->GetReference(); 
	
	//metemos en el estado el handle a la formula
	oState.m_hItem=ai_hItem;
	oState.m_iRule=iRuleOrder;
	oState.m_dDate=ai_oParentState.m_dDate; //la fecha en la que estamso (la instrucción que provoca esta llamada)
    oState.m_dBeginDate=ai_oParentState.m_dBeginDate;
    oState.m_dEndDate=ai_oParentState.m_dEndDate;
	oState.m_Flags=0;
	oState.m_iSourceLine=1;	//No.de Linea fuente
	oState.m_iStepperType=StepperTypeNormal;
	
	//Nuevo. Estadisticas
	oState.m_iParentStatePos=ai_iParentStatePos;
	
	M4CL_SET_STATE_PURGE_FLAG (oState.m_Flags);
    if (poCMCR->GetItemType(ai_hItem) ==M4CL_ITEM_TYPE_CONCEPT ){ 
		M4CL_SET_STATE_CONCEPT_FLAG (oState.m_Flags);
	}
    

	//ver si tenemos permiso de ejecución
	if (poCMCR->GetItemSecExecute(ai_hItem) != 1){//No tiene persimos .Ejecución nula o corte de cascada

		m4pcchar_t	pccItem ;
		m4uint32_t	iError ;

		pccItem = poCMCR->GetItemId( ai_hItem ) ;
		iError = M4GetDBOperation( pccItem ) ;

		if ( poCMCR->GetItemSecContinue(ai_hItem) == 0 ) { //no continua la ejecución->Cortamos cascada
			
			//paramos la ejecución con error

			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_EXECUTE_PERM_CONN, M4ObjNodeItemI(poCMCR,ai_hItem)<< NodeItemI(poCMCR,ai_oParentState.m_hItem)   << DumpCallStack() );
			
			if( iError != 0 ){
				DUMP_CHLOG_ERRORF( M4_CH_VM_NO_LOAD_PERM + iError - 1, DumpCallStack() ) ;
			}
			
			return( M4_ERROR ) ;
		}
		else{ //dejamos continuar con formula nula y warning
			
			hFormula=0; //ejecución vacia
			
			//sacamos warnings, si es que no los hay ya
			if( M4ChLogIsErrorIn( M4_CH_VM_NO_EXECUTE_PERM_CONN ) == 0 ){
				DUMP_CHLOG_WARNINGF(M4_CH_VM_NO_EXECUTE_PERM_CONN, M4ObjNodeItemI(poCMCR,ai_hItem)<< NodeItemI(poCMCR,ai_oParentState.m_hItem)   << DumpCallStack() );
			}
			
			if( iError != 0 && M4ChLogIsErrorIn( M4_CH_VM_NO_LOAD_PERM + iError - 1 ) == 0 ){
				DUMP_CHLOG_WARNINGF( M4_CH_VM_NO_LOAD_PERM + iError - 1, DumpCallStack() ) ;
			}
		}
	}
	else{ //si tiene permisos
		//si tiene permisos->Intentamos obtener la formula
		//puntero a la primera instrucción
		if (M4CL_HAS_STATE_CONCEPT_FLAG (oState.m_Flags) && m_poVM->GetRunSliceMode(ai_RecordSet->GetpAccess())){
			hFormula=poCMCR->GetItemCodeByDate (ai_hItem, iRuleOrder, ai_oParentState.m_dBeginDate, iSlicePosition);
		}
		else{
			hFormula=poCMCR->GetItemCodeByDate (ai_hItem, iRuleOrder, ai_oParentState.m_dDate, iSlicePosition);
		}
    }

	//la creamos aquí con el handle de la formula o con 0 para que sea nula
	oState.m_poFormula.Init( (m4pcchar_t)hFormula);
	
    
	//metemos el access, el bloque actual y el registro actual
	oState.m_poAccess=ai_RecordSet->GetpAccess();
    oState.m_poRecordSet=ai_RecordSet;
	oState.m_oRegisterIndex=ai_RegIndex;
	
	//establecemos la referencia en la pos. actual de la pila
	m_oRunTimeStack->SetReference(m_oRunTimeStack->GetPos()+1); //ponemos de referencia el tope
	
	//empezamos la ejecución en la instrucción 0, 
	oState.m_iCurrent=0;
	
    //si necesitamso transaccion la ponemos
    m4uint8_t iTransMode=poCMCR->GetItemTransactionMode(ai_hItem);
    if (iTransMode==M4CL_ITEM_TRAN_MODE_NEED_TRANSACTION ||
        iTransMode==M4CL_ITEM_TRAN_MODE_NEW_TRANSACTION ){
        
//        m_poVM->BeginVMTrans(M4_TRUE);
		m_poVM->BeginVMTrans();
    }
	
    
	//guardamos el estado en la pila de estados
	ret=m_oStateStack->Push(oState);
	return ret;
	
}


m4return_t ClVM1Propagator::_GeneratePropMoveTo (ClAccessRecordSet  *ai_poRecordSet,ClRegisterIndex ai_Reg, m4bool_t ai_bUseMM, m4bool_t ai_bRefresh){
	
	
	m4VMState_t temp;
	const char *p=0; //el compilador de Sun Microsystems(c) es una mierda y me obliga a poner esto
					//para pasar un nulo a una función en release. El mensaje de error es para descojonarse.
	temp.m_hItem=-1;       
	//temp.m_poFormula=0;
	temp.m_poAccess=ai_poRecordSet->GetpAccess();
	temp.m_poRecordSet=ai_poRecordSet;
	temp.m_oRegisterIndex=ai_Reg;
	//guardamos en el estado la referencia vieja de la pila 
	temp.m_iOldStackReference=m_oRunTimeStack->GetReference(); 
	temp.m_dDate=0;
	temp.m_dBeginDate=0;
	temp.m_dEndDate=0;
	temp.m_iCurrent=0;
	temp.m_Flags=0;
	
	M4CL_SET_STATE_HIDE_FLAG(temp.m_Flags); //para que cuando se muestre en el call stack no salga
	
	if (ai_bUseMM){ //indicamos que debe considerar el index como absoluto (MM) no como índice lógico, que puede verse 
					//afectado por los filtros (p.e.)
		M4CL_SET_STATE_MM_POSITION_FLAG(temp.m_Flags);
	}

	/* Bug 0094704
	Se indica si se fuerza a que se haga un posicionamiento en los hijos
	después del move to
	*/
	if( ai_bRefresh )
	{
		M4CL_SET_STATE_REFRESH_FLAG( temp.m_Flags ) ;
	}

	temp.m_iParentStatePos=m_oStateStack->GetPos();
	temp.m_iSourceLine=0;
	temp.m_iStepperType=StepperTypeNormal;
	temp.m_poFormula.Init ( p );
	temp.m_iRule=0;
	return m_oStateStack->Push(temp); //metemos el nuevo que hace el MoveTo
	
}

