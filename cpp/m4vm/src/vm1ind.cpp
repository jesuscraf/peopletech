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
//    Virtual machine de nivel 1- Funciuones de indireccion
////
//==============================================================================


#include "vmachin1.h"

#include "vm1ind.hpp"

#include "vmattrib.hpp"
#include "tablatt.hpp"
#include "vmres.hpp"

#include "m4log.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "cldbgutil.hpp"
#include "cp_exe.hpp"
#include "rthierdes.hpp"
#include "m4objglb.hpp"
#include "nodedef.hpp"

#include <limits.h>



//---------------------------------------------------
//I N D I R E C C I O N E S 
//---------------------------------------------------
//-----------------------------
//obtienen item de forma indirecta
//-----------------------------

//función comun para el de prioridad y sin prioridad y lso get---
m4return_t ClVM1Indirections::GetItemIndirect ( m4VariantType  *ai_variant, m4uint8_t ai_iNumVar, m4bool_t bSlice,  ClItem *& ao_poItem, m4bool_t *aio_pbNewSlices/*=NULL*/){
	
/*Los argumentos vienen:

  Slice       0
  Registro    1
  Item        2
  Nodo        3
	*/
	
	
    m4return_t  ret=M4_SUCCESS;
    _ClRegister_Base    *poReg;
    
    const m4char_t    *pszNode, *pszItem;
    ClNode      *poNode;
    ClAccessRecordSet   *poRecordSet;
    ClValue     *poValue;
    ClItem      *poItem;
    m4uint32_t  hItem, hNode, hMyNode;
    m4uint32_t idxItem;
	
    ClCompiledMCR   *poCMCR=m_oState1.m_poAccess-> GetpCMCR() ;
    
	
	if (aio_pbNewSlices){
		*aio_pbNewSlices=M4_FALSE;
	}
    
    //tramo
    if ( bSlice ){
		
        if (IsNull (ai_variant[0])) { //comprobamos que no sea nulo
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "slice"<<"Set|GetValue" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(M4CL_CPP_TYPE_NULL)<< DumpCallStack() );
            return M4_ERROR;
        } 
		
        if (ai_variant[0].Type==M4CL_CPP_TYPE_STRING_POOL) { //si es cadena convertimos si se puede
            ret = m_oVariantMediator->ToDouble(ai_variant[0]);
			if (ret!=M4_SUCCESS){
				DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "slice"<<"Set|GetValue" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(ai_variant[0].Type)<< DumpCallStack() );
				return M4_ERROR;
			}
        }
		
    }
	
    
    //registro
    if ( IsNull ( ai_variant[1] ) ) { //comprobamos que no sea nulo
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "record number"<<"Set|GetValue" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(M4CL_CPP_TYPE_NULL)<< DumpCallStack() );
        return M4_ERROR;
    }
    if ( ai_variant[1].Type!=M4CL_CPP_TYPE_NUMBER ){ //si no es número intentamos convertir
        ret = m_oVariantMediator->ToDouble(ai_variant[1]);
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "record number"<<"Set|GetValue" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(ai_variant[1].Type)<< DumpCallStack() );
			return M4_ERROR;
		}

    }
    
	
    //nodo & item
    if  (ai_variant[2].Type !=M4CL_CPP_TYPE_STRING_POOL && ai_variant[2].Type !=M4CL_CPP_TYPE_NUMBER )  {
		
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "item"<<"Set|Get Value" << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<","<<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL) )<<M4ClCppType(ai_variant[2].Type)<<DumpCallStack() );

        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
    if  (ai_variant[3].Type !=M4CL_CPP_TYPE_STRING_POOL && ai_variant[3].Type !=M4CL_CPP_TYPE_NUMBER) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "node"<<"Set|Get Value" <<LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<","<<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL) )<<M4ClCppType(ai_variant[3].Type)<<DumpCallStack() );

        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }


    //--nodo
	if (ai_variant[3].Type==M4CL_CPP_TYPE_STRING_POOL){
        pszNode=m_oDynamicPool->GetString (ai_variant[3].Data.CharData);
        if ( pszNode == NULL ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "NodeId"<<"Set|Get Value"<<"<null>" << DumpCallStack() );
            RuntimePushDouble (M4_ERROR);
            return M4_ERROR;
        }

        //obtenemos el nodo
        hMyNode = poCMCR->GetItemNodeHandle             ( m_oState1.m_hItem ) ;
        M4CHECK_RES (hMyNode , M4_CH_VM_WRONG_THIS_NODE_HANDLE ,ERRORLOG, "Set|GetValue" << DumpCallStack() );
		
		if (pszNode[0]==0){ //nodo="" ->Usamos nuestro mismo nodo
			hNode=hMyNode;
			pszNode=poCMCR->GetNodeId (hNode);
		}
		else{
			hNode   = poCMCR->GetNodeAliasNodeHandleByAlias ( hMyNode, pszNode, m_oState1.m_poFormula.GetLevel() ) ;
		}

        if( hNode == 0 ){
            RuntimePushDouble( M4_ERROR ) ;
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, pszNode <<"Set|Get Value" << DumpCallStack() );
            return M4_ERROR;
        }
        
        poNode=&( m_oState1.m_poAccess->Node[ ClHandle( hNode ) ] ) ;
    }
    else{ //por indice
        poNode=&( m_oState1.m_poAccess->Node[ ClNodeIndex( (m4uint32_t)ai_variant[3].Data.DoubleData) ] ) ;
		if (!poNode) {
            RuntimePushDouble( M4_ERROR ) ;
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_INDEX, ((m4uint32_t)ai_variant[3].Data.DoubleData) <<"Set|Get Value" << DumpCallStack() );
            return M4_ERROR;
		}
        hNode=poNode->GetpNodeDef()->GetHandle()  ;
        pszNode=poNode->GetpNodeDef()->Id()  ;
    }
    
	
    //item
    if (ai_variant[2].Type==M4CL_CPP_TYPE_STRING_POOL){
        pszItem=m_oDynamicPool->GetString (ai_variant[2].Data.CharData);
        if ( pszItem == NULL ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "ItemId"<<"Set|Get Value "<<"<null>" << DumpCallStack() );
            RuntimePushDouble (M4_ERROR);
            return M4_ERROR;
        }
        //para poder obtener el scope del item
        hItem = poCMCR->GetNodeItemHandleById( hNode, pszItem );
        if ( hItem == NULL ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, pszItem<<M4ObjNodeN(poCMCR,hNode)<<"Set|Get Value " << DumpCallStack() );
            RuntimePushDouble ( M4_ERROR );
            return M4_ERROR;
        }
        idxItem=poCMCR->GetItemIndex (hItem);
    }
    else { //por indice
		//Verificamos index item valido
		if (ai_variant[2].Data.DoubleData<0.0 || ai_variant[2].Data.DoubleData>=poCMCR->GetNodeNumberOfItems(hNode) ) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_ITEM_INDEX, (m4int32_t)ai_variant[2].Data.DoubleData << pszNode << "Set|Get Value " << DumpCallStack() );
            RuntimePushDouble ( M4_ERROR );
            return M4_ERROR;
		}
        idxItem=(m4uint32_t)ai_variant[2].Data.DoubleData;
        hItem=poCMCR->GetNodeItemHandleByPosition (hNode, (m4uint16_t)idxItem);
        pszItem=poCMCR->GetNodeItemIdByPosition (hNode, (m4uint16_t)idxItem);
    }

#if _M4_DEBUG > 10         
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Set|Get Value (indirect) (" <<LogCat<< pszNode <<LogCat<< "[" <<LogCat<< (int)ai_variant[1].Data.DoubleData <<LogCat<< "]." <<LogCat<< pszItem<<LogCat<< "):" );
#endif
	
    if ( poNode==NULL ){
        RuntimePushDouble (M4_ERROR);
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, pszNode <<"Set|Get Value" << DumpCallStack() );
        return M4_ERROR;
    }
    
	
    //el bloque
    poRecordSet = &( poNode->RecordSet ); //casting automatico
    if (!poRecordSet->IsValid() && poCMCR->GetItemScope(hItem) != M4CL_ITEM_SCOPE_NODE ){
        RuntimePushDouble (M4_ERROR);
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK, "Set|Get Value"<<pszNode <<DumpTreePath(poRecordSet->GetpAccess(), hNode) << DumpCallStack() );
        return M4_ERROR;
    }
	
    //el record
    if ( (m4int32_t) ai_variant[1].Data.DoubleData < 0 ){
        poReg=&( poRecordSet->Current );
    }
    else{
        poReg=&( poRecordSet->Register[(m4int32_t)ai_variant[1].Data.DoubleData] );
    }
    
	
	
    //si el registro es EOF y el item es de registro->error
    if ( poReg->Index == M4DM_EOF_INDEX && poCMCR->GetItemScope(hItem) == M4CL_ITEM_SCOPE_REGISTER ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_NUMBER, M4ObjNodeI(poCMCR,hItem)<<"Set|Get Value"<<DumpTreePath(poRecordSet->GetpAccess(), hNode)  << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
    //el item
    poItem=&( poReg->Item[ClItemIndex(idxItem)] );
    if ( poItem->GetIndex() == M4DM_EOF_INDEX ) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, pszItem<<M4ObjNodeN(poCMCR,hNode)<<"Set|Get Value " << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
    //el valor 
    if ( bSlice ){ 
        switch ( ai_variant[0].Type ){
			
        case M4CL_CPP_TYPE_DATE:
            {
                poValue=& ( poItem->Value[ai_variant[0].Data.DoubleData]);
                break;
            }
        default:
            {
                poValue=& ( poItem->Value[ClSliceIndex( (m4int32_t)ai_variant[0].Data.DoubleData) ] );
                break;
            }
        }
    }
    else if ( m_poVM->GetRunSliceMode(m_oState1.m_poAccess)&& !bSlice ) { //tomamos el slice en el que estemod
		/* Bug 0076620
		Ahora el parámetro es de entrada y salida.
		Si se han creado tramos se pone a true
		*/
        if (aio_pbNewSlices){
			ret=m_poVM->CheckAndCreateItemSlice(m_oState1.m_dBeginDate, m_oState1.m_dEndDate, poItem);
            M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_ADDING_SLICE, ERRORLOG, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(), poItem->ItemDef.GetHandle())<<"Get|Set Value" << DumpCallStack() );
			*aio_pbNewSlices=M4_TRUE;
		}
		poValue=& ( poItem->Value[m_oState1.m_dBeginDate] );
		
		//comprobamos que todo el tramo actual esté dentro del periodo definido
		if (poValue->Index != M4DM_EOF_INDEX && m_oState1.m_dEndDate >poValue->GetLastEndDate()){
			poValue->MoveToEOF();
		}
    }
    else{
        poValue=&( poItem->Value );
    }
	
    //comprobamos que el value no sea NULL
    if ( poValue->Index == M4DM_EOF_INDEX ) {
        if ( bSlice ){
            m_oVariantMediator->ToPoolString ( ai_variant[0] );
            DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE, m_oDynamicPool->GetString(ai_variant[0].Data.CharData)<<M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(), poItem->ItemDef.GetHandle())<<"Get|Set Value" << DumpCallStack() );
            m_oDynamicPool->DeleteString( ai_variant[0].Data.CharData );
        }
        else{
            DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE, BeginLogCat<<DateLog(m_oState1.m_dBeginDate)<<" - "<<DateLog(m_oState1.m_dEndDate)<<EndLogCat<<M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(), poItem->ItemDef.GetHandle())<<"Get|Set Value" << DumpCallStack() );
        }
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
        //return M4_SUCCESS;
    }
	
    //leemos el valor y lo metemos en la pila
    ret=M4_SUCCESS;
    ao_poItem=poItem; //para retronar un puntero al item
	
	
	//    RuntimePushDouble (ret);
    return M4_SUCCESS;
	
	
}


//----------------------
//SetValue SIN prioridad
//----------------------
m4return_t ClVM1Indirections::SetValue			(void){

    m4VariantType aParam[4], vNum, Value;
    m4return_t  ret=M4_SUCCESS;
    m4bool_t    bSlice=M4_FALSE;
	m4bool_t	bNewSlices=M4_FALSE;
    m4uint32_t  i, iNumArgs;
    ClItem *poItem;
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "SetValue (indirect)");
#endif
	
    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetValue.")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
    //demasiados parametros 
    if ( iNumArgs> 5 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "SetValue"<<5<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 5 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetValue (indirect).")	;
        }
        iNumArgs=5; //ya hemos sacado los sobrantes
    }
    
	
    //Valor a escribir
    ret=RuntimePop(Value);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetValue (indirect).")	;
    
    //con tramo
    if ( iNumArgs > 4 ){
        ret=RuntimePop(aParam[0]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetValue (indirect)")	;
        
        bSlice=M4_TRUE;
    }
	
    
    //registro, item, nodo
    for (i=1; i < 4 ; ++i){
        ret=RuntimePop(aParam[i]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetValue (indirect).")	;
    }
	
	m4return_t retVal=M4_SUCCESS;
	
    //obtenemos el puntero al item
    ret=  GetItemIndirect ( aParam, 4, bSlice, poItem, &bNewSlices);
    if (ret != M4_SUCCESS){
		retVal=M4_ERROR;
	}
	else{
		if (poItem->ItemDef.Type()==M4CL_ITEM_TYPE_METHOD){
			DUMP_CHLOG_ERRORF(M4_CH_VM_TRY_WRITE_METHOD, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle() )<<"SetValue" << DumpCallStack() );
			retVal=M4_ERROR;
		}
		else{
			//metemos el valor 
			/* Bug 0076620
			Si se han creado tramos se asigna entre fechas
			*/
			if (bNewSlices==M4_TRUE)
			{
				ret=m_poVM->SetValue(*poItem, Value, M4CL_CM_DEFAULT_PRIOR, 0, m_oState1.m_dBeginDate, m_oState1.m_dEndDate, M4_TRUE);
			}
			else
			{
				// Chequeo de uso de un concepto tramado en modo no tramos
				m4uint32_t iSlices = poItem->Value.Count() ;

				if( iSlices > 0 )
				{
				    m4char_t	acDate[ M4_SECOND_END + 1 ] ;
					ClDateToString( m_oState1.m_dDate, acDate, M4_SECOND_END + 1 ) ;
					DUMP_CHLOG_WARNINGF( M4_CH_VM_BASE_SLICED_ITEM_USE, M4ObjNodeItemI( poItem->GetpAccess()->GetpCMCR(), poItem->ItemDef.GetHandle() ) << iSlices << poItem->Value.Index << m_oState1.m_iSourceLine << m_oState1.m_poAccess->GetpCMCR()->GetItemFormIdByPosition( m_oState1.m_hItem, m_oState1.m_iRule ) << M4ObjNodeItemI( m_oState1.m_poAccess->GetpCMCR(), m_oState1.m_hItem ) << acDate << DumpCallStack() ) ;
				}

				ret=m_poVM->__SetValue(*poItem, Value, M4CL_CM_DEFAULT_PRIOR, 0);
			}

			if (ret==M4_WARNING){
				DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle() )<<"SetValue"  << DumpCallStack() );
			}
			else if (ret!=M4_SUCCESS){
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle() )<<"SetValue" << DumpCallStack() );
				
			}
		}
	}
	
	//fredom
    DeleteVarString (Value);
    if ( iNumArgs > 4 ){
        DeleteVarString(aParam[0]);
    }
    //registro, item, nodo
    for (i=1; i < 4 ; ++i){
        DeleteVarString(aParam[i]);
    }
	
	
    RuntimePushDouble (ret);
    return retVal;
}



//----------------------
//SetValue con prioridad
//----------------------
m4return_t ClVM1Indirections::SetValuePriority			(void){
    m4VariantType aParam[4], vTemp, vNum, Value;
    m4return_t  ret=M4_SUCCESS;
    m4bool_t    bSlice=M4_FALSE;
	m4bool_t	bNewSlices=M4_FALSE;
    m4uint32_t  i, iNumArgs;
    ClItem *poItem;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "SetValuePriority (indirect)");
#endif
    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetValuePriority (indirect).")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
    
    //demasiados parametros 
    if ( iNumArgs> 6 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "SetValuePriority"<<6<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 6 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetValuePriority (indirect).")	;
        }
        iNumArgs=6; //ya hemos sacado los sobrantes
    }
    
	
    //Valor a escribir
    ret=RuntimePop(Value);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetValuePriority (indirect).")	;
    
    //con tramo
    if ( iNumArgs > 5 ){
        ret=RuntimePop(aParam[0]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetValuePriority (indirect).")	;
        bSlice=M4_TRUE;
    }
    
    //registro, item, nodo, 
    for (i=1; i < 4 ; ++i){
        ret=RuntimePop(aParam[i]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetValuePriority (indirect).")	;
    }
    
    //prioridad
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetValuePriority (indirect).")	;
    if (vTemp.Type!=M4CL_CPP_TYPE_NUMBER){
        ret = m_oVariantMediator->ToDouble(vTemp);
        if (ret != M4_SUCCESS){
			DUMP_CHLOG_WARNINGF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "priority"<<"SetValuePriority" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<< DumpCallStack() );

            vTemp.Data.DoubleData = M4CL_CM_DEFAULT_PRIOR;
        }
    }
	
	//valor de la prioridad-chequeo de si es o no correcta-Ojo!!!min->número más alto, max->número más bajo
	if (vTemp.Data.DoubleData >M4CL_RULE_MIN_PRIORITY || vTemp.Data.DoubleData  < M4CL_RULE_MAX_PRIORITY){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "priority"<<"SetValuePriority"<<(priority_t)vTemp.Data.DoubleData << DumpCallStack() );
		vTemp.Data.DoubleData = M4CL_CM_DEFAULT_PRIOR;
	}
	m4return_t retVal=M4_SUCCESS;
    //obtenemos el puntero al item
    ret=  GetItemIndirect ( aParam, 4, bSlice, poItem, &bNewSlices);
    if (ret!=M4_SUCCESS){
		retVal=M4_ERROR;
	}
	else{
		if (poItem->ItemDef.Type()==M4CL_ITEM_TYPE_METHOD){
			DUMP_CHLOG_ERRORF(M4_CH_VM_TRY_WRITE_METHOD, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle() )<<"SetValue" << DumpCallStack() );
			retVal=M4_ERROR;
		}
		else{
			
			//metemos el valor 
			/* Bug 0076620
			Si se han creado tramos se asigna entre fechas
			*/
			if (bNewSlices==M4_TRUE)
			{
				ret=m_poVM->SetValue(*poItem, Value, (priority_t)(vTemp.Data.DoubleData), 0, m_oState1.m_dBeginDate, m_oState1.m_dEndDate, M4_TRUE);
			}
			else
			{
				ret=m_poVM->__SetValue(*poItem, Value, (priority_t)(vTemp.Data.DoubleData), 0);
			}

			if (ret==M4_WARNING){
				DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle() )<<"SetValuePriority" << DumpCallStack() );
			}
			else if (ret!=M4_SUCCESS){
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle() )<<"SetValuePriority" << DumpCallStack() );
			}
		}
	}
	
    //freedom
    DeleteVarString (Value);
    if ( iNumArgs > 5 ){
        DeleteVarString(aParam[0]);
    }
    for (i=1; i < 4 ; ++i){
        DeleteVarString(aParam[i]);
    }
	
	
    RuntimePushDouble (ret);
    return retVal;
}


//----------------------
//GetValue 
//----------------------
m4return_t ClVM1Indirections::GetValue			(void){

    m4VariantType aParam[4],  vNum, Value;
    m4return_t  ret=M4_SUCCESS;
    m4bool_t    bSlice=M4_FALSE;
    m4uint32_t  i, iNumArgs;
	m4date_t	dStartDate = -1;
	m4date_t	dEndDate = -1;
    ClItem *poItem;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "GetValue (indirect)");
#endif
    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetValue (indirect). ")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
    //demasiados parametros 
    if ( iNumArgs> 4 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "GetValue"<<4<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 4 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetValue (indirect).")	;
        }
        iNumArgs=4; //ya hemos sacado los sobrantes
    }
    
    
    //con tramo
    if ( iNumArgs > 3 ){
        ret=RuntimePop(aParam[0]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetValue (indirect). ")	;
        
        bSlice=M4_TRUE;
    }
	
    
    //registro, item, nodo
    for (i=1; i < 4 ; ++i){
        ret=RuntimePop(aParam[i]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetValue (indirect).")	;
    }
	
	m4return_t retVal=M4_SUCCESS;
    //obtenemos el puntero al item
    ret=  GetItemIndirect ( aParam, 4, bSlice, poItem);
	if (ret != M4_SUCCESS){
		retVal=M4_ERROR;
	}
    else{
		if (poItem->ItemDef.Type()==M4CL_ITEM_TYPE_METHOD){
			DUMP_CHLOG_ERRORF(M4_CH_VM_TRY_READ_METHOD, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle() )<<"SetValue" << DumpCallStack() );
			retVal=M4_ERROR;
		}
		else{
			
			//leemos el valor y lo metemos en la pila
			if (m_poVM->GetRunSliceMode(m_oState1.m_poAccess) && !bSlice ){ //tomamos el slice en el que estemod
				ret=poItem->Value.Get (m_oState1.m_dBeginDate, m_oState1.m_dEndDate, Value);

				dStartDate = m_oState1.m_dBeginDate;
				dEndDate = m_oState1.m_dEndDate;
			}
			else{
				// Chequeo de uso de un concepto tramado en modo no tramos
				m4uint32_t iSlices = poItem->Value.Count() ;

				if( iSlices > 0 )
				{
				    m4char_t	acDate[ M4_SECOND_END + 1 ] ;
					ClDateToString( m_oState1.m_dDate, acDate, M4_SECOND_END + 1 ) ;
					DUMP_CHLOG_WARNINGF( M4_CH_VM_BASE_SLICED_ITEM_USE, M4ObjNodeItemI( poItem->GetpAccess()->GetpCMCR(), poItem->ItemDef.GetHandle() ) << iSlices << poItem->Value.Index << m_oState1.m_iSourceLine << m_oState1.m_poAccess->GetpCMCR()->GetItemFormIdByPosition( m_oState1.m_hItem, m_oState1.m_iRule ) << M4ObjNodeItemI( m_oState1.m_poAccess->GetpCMCR(), m_oState1.m_hItem ) << acDate << DumpCallStack() ) ;
				}

				ret=poItem->Value.Get (Value);
			}
			if (ret!=M4_SUCCESS){
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle())<<"GetValue" << DumpCallStack() );
			}
			else{
				if ( (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_READS) )
				{
					m4VMState_t oState;
					m_oStateStack->Top(oState);
					GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddRead( *poItem, Value, dStartDate, dEndDate, oState, DumpCallStack( &oState, M4_TRUE ).c_str() );
				}
			}

			if (Value.Type==M4CL_CPP_TYPE_STRING_VAR){
				m_oVariantMediator->ToPoolString (Value);
			}
		}
	}
	
	//freedom
    if ( iNumArgs > 3 ){
		DeleteVarString(aParam[0]);
	}
	for (i=1; i < 4 ; ++i){
		DeleteVarString(aParam[i]);
	}
	
	
    RuntimePush (Value);
    return retVal;
	
}


//-------------------------------------
//Llamada a método genérica
//-------------------------------------
m4return_t ClVM1Indirections::_Call(m4bool_t ai_bByNode){
    
	m4VariantType		vNode, vItem, vNumArgs, vFoo;
    m4return_t			ret=M4_SUCCESS;
    _ClRegister_Base    *poReg;
    const m4char_t		*pszNode, *pszItem;
    ClNode				*poNode;
    ClAccessRecordSet   *poAccessRecordSet ;
    ClAccessRecordSet   *poRecordSet;
    ClItem				*poItem;
    m4uint32_t			hItem,iNumArgsNeeded , hNode, hMyNode, idxItem;
	m4uint8_t			iType;
    
    
    
    ClCompiledMCR   *poCMCR=m_oState1.m_poAccess-> GetpCMCR() ;
	
    //Numero de argumentos 
	ret=RuntimePopDouble(vNumArgs);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Call")	;
    
    //descontamos los dos argumentos del call
    vNumArgs.Data.DoubleData-=2;
	
    //item
	ret=RuntimePop(vItem);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Call.")	;
    
    //nodo
	ret=RuntimePop(vNode);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Call.")	;
    
    if (vNode.Type !=M4CL_CPP_TYPE_STRING_POOL  && vNode.Type !=M4CL_CPP_TYPE_NUMBER ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "node"<<"Call" <<LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<","<<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL) )<<M4ClCppType(vNode.Type)<<DumpCallStack() );

        RuntimePushDouble (M4_ERROR);
		DeleteVarString (vItem); //para no dejar nada sin liberar
		DeleteVarString (vNode);
        return M4_ERROR;
    }
	if (vItem.Type!=M4CL_CPP_TYPE_STRING_POOL  && vItem.Type!=M4CL_CPP_TYPE_NUMBER) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "item"<<"Call" <<LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<","<<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL) )<<M4ClCppType(vItem.Type)<<DumpCallStack() );

        RuntimePushDouble (M4_ERROR);
		DeleteVarString (vItem); //para no dejar nada sin liberar
		DeleteVarString (vNode);
        return M4_ERROR;
    }
	//argumetnos correctos-->...
	
    hMyNode = poCMCR->GetItemNodeHandle             ( m_oState1.m_hItem ) ;
    M4CHECK_RES (hMyNode , M4_CH_VM_WRONG_THIS_NODE_HANDLE ,ERRORLOG, "Call" << DumpCallStack() );
    //nodo
    if (vNode.Type==M4CL_CPP_TYPE_STRING_POOL){
        pszNode=m_oDynamicPool->GetString (vNode.Data.CharData);
		//liberamos la cadena pos si hay error
		m_oDynamicPool->DeleteString (vNode.Data.CharData);
		
        if ( pszNode == NULL ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "NodeId"<<"Call"<<"<null>" << DumpCallStack() );
            RuntimePushDouble (M4_ERROR);
			DeleteVarString (vItem); //para no dejar nada sin liberar
            return M4_ERROR;
        }

		if (pszNode[0]==0){ //nodo="" ->Usamos nuestro mismo nodo
			hNode=hMyNode;
			pszNode=poCMCR->GetNodeId (hNode);
		}
		else{
			//obtenemos el nodo

			/* Bug 0123677
			Se necesita una función que busque sólo por id de nodo
			*/
			if( ai_bByNode == M4_TRUE )
			{
				hNode   = poCMCR->GetChannelNodeHandleById( pszNode ) ;
			}
			else
			{
				hNode   = poCMCR->GetNodeAliasNodeHandleByAlias ( hMyNode, pszNode, m_oState1.m_poFormula.GetLevel() ) ;
			}
		}

        if( hNode == 0 ){
            RuntimePushDouble( M4_ERROR ) ;
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, pszNode <<"Call" << DumpCallStack() );
			DeleteVarString (vItem); //para no dejar nada sin liberar
            return M4_ERROR;
        }
        
        poNode=&( m_oState1.m_poAccess->Node[ ClHandle( hNode ) ] ) ;
    }
    else{ //por indice
        poNode=&( m_oState1.m_poAccess->Node[ ClNodeIndex( (m4uint32_t)vNode.Data.DoubleData) ] ) ;
		if (!poNode) {
            RuntimePushDouble( M4_ERROR ) ;
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_INDEX, ((m4uint32_t)vNode.Data.DoubleData) <<"Call" << DumpCallStack() );
            return M4_ERROR;
		}
        hNode=poNode->GetpNodeDef()->GetHandle()  ;
        pszNode=poNode->GetpNodeDef()->Id()  ;
    }
    
	
	
    //item
    if (vItem.Type==M4CL_CPP_TYPE_STRING_POOL){
        pszItem=m_oDynamicPool->GetString (vItem.Data.CharData);
		
		m_oDynamicPool->DeleteString (vItem.Data.CharData); //liberamos la cadena por si hay error
        
		if ( pszItem == NULL ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "ItemId"<<"Call"<<"<null>" << DumpCallStack() );
            RuntimePushDouble (M4_ERROR);
            return M4_ERROR;
        }
        //para poder obtener el scope del item
        hItem = poCMCR->GetNodeItemHandleById( hNode, pszItem );
        if ( hItem == NULL ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, pszItem<<M4ObjNodeN(poCMCR,hNode)<<"Call" << DumpCallStack() );
            RuntimePushDouble ( M4_ERROR );
            return M4_ERROR;
        }
        idxItem=poCMCR->GetItemIndex (hItem);
    }
    else { //por indice
		//Verificamos index item valido
		if (vItem.Data.DoubleData<0.0 || vItem.Data.DoubleData>=poCMCR->GetNodeNumberOfItems(hNode) ) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_ITEM_INDEX, (m4int32_t)vItem.Data.DoubleData << pszNode << "Call" << DumpCallStack() );
            RuntimePushDouble ( M4_ERROR );
            return M4_ERROR;
		}
        idxItem=(m4uint32_t)vItem.Data.DoubleData;
        hItem=poCMCR->GetNodeItemHandleByPosition (hNode, (m4uint16_t)idxItem);
        pszItem=poCMCR->GetNodeItemIdByPosition (hNode, (m4uint16_t)idxItem);
    }

	//comprobamos que sea ejecutable
	if (poCMCR->GetItemType(hItem) != M4CL_ITEM_TYPE_METHOD && poCMCR->GetItemType(hItem) != M4CL_ITEM_TYPE_CONCEPT){
		DUMP_CHLOG_ERRORF(M4_CH_VM_TRY_EXECUTE_FIELD_PROP, M4ObjNodeItemI(poCMCR,hItem)<<"Call" << DumpCallStack() );
		return M4_ERROR;
	}

	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Call (indirect) (" <<LogCat<< pszNode <<LogCat<< "." <<LogCat<< pszItem <<LogCat<< ")." );
#endif
	
    //el bloque
    poAccessRecordSet = &(poNode->RecordSet); //casting automatico
    if (!poAccessRecordSet ->IsValid() && poCMCR->GetItemScope(hItem) != M4CL_ITEM_SCOPE_NODE){
        RuntimePushDouble (M4_ERROR);
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK, "Call"<<pszNode<<DumpTreePath(poAccessRecordSet->GetpAccess(), hNode)  << DumpCallStack() );
        return M4_ERROR;
    }
	
	//el record
    poReg=&(poAccessRecordSet ->Current);
    // despues de obtener el current usamos un recordset normal, no de access
    poRecordSet=&(poNode->RecordSet);   // directamente el rs del access !
    
	
    //si el registro es EOF y el item es de registro->error
    if (poReg->Index == M4DM_EOF_INDEX && poCMCR->GetItemScope(hItem) == M4CL_ITEM_SCOPE_REGISTER ) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_NUMBER, M4ObjNodeI(poCMCR,hItem)<<"Call"<<DumpTreePath(poAccessRecordSet->GetpAccess(), hNode)  << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
    //el item
    poItem=&(poReg->Item[ ClItemIndex (idxItem) ]);
    if (poItem->GetIndex() == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, pszItem<<M4ObjNodeN(poCMCR,hNode)<<"Call" << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
    //comprobamos que el número de argumentos sea correcto
    iNumArgsNeeded= poCMCR->GetItemNumberOfArguments(hItem);
    m4int8_t bVarArg=poCMCR->GetItemVariableArguments(hItem);
	
    if (bVarArg!=0 && vNumArgs.Data.DoubleData < iNumArgsNeeded){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_ITEM_PARAM_NUM, pszItem<<pszNode<<M4Obj(poCMCR)<<">=" <<LogCat<<iNumArgsNeeded <<(m4uint32_t)vNumArgs.Data.DoubleData<< DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
    else if (bVarArg==0 && vNumArgs.Data.DoubleData != iNumArgsNeeded){ //error ->nos faltan argumentos
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_ITEM_PARAM_NUM, pszItem<<pszNode<<M4Obj(poCMCR)<<iNumArgsNeeded <<(m4uint32_t)vNumArgs.Data.DoubleData<< DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
    if (poCMCR->GetItemNumberOfReferedArguments(hItem) !=0  ){ //error ->El item necesita arguemtnso por referencia-- No lo permitimos
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_REFERED_ARG_ALLOWED, M4ObjNodeItemI(poCMCR,hItem)<<"Call" << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
    
    //en lugar de meter resultado dejamos el que deja el 
    //metodo que estamos ejecutando
	
    m4VMState_t oNewState;
	
    oNewState.m_dDate=m_oState1.m_dDate;
    oNewState.m_dBeginDate=m_oState1.m_dBeginDate;
    oNewState.m_dEndDate=m_oState1.m_dEndDate;
    oNewState.m_poAccess=m_oState1.m_poAccess;
    oNewState.m_hItem=hItem;
    oNewState.m_iRule=0;
    oNewState.m_Flags=0;
    oNewState.m_poRecordSet=poRecordSet;
    oNewState.m_oRegisterIndex=poReg->Index ;
	//Bug:0064136
	// Cálculo correcto del m_iParentStatePos
	oNewState.m_iParentStatePos=m_oStateStack->GetPos();

	// Si el padre es uno de los items del sistema propagamos a mano la marca de no insite
	if( M4CL_HAS_STATE_NO_INSITE_FLAG_REC( m_oState1.m_Flags ) )
	{
		iType = poCMCR->GetItemInternalType( m_oState1.m_hItem ) ;

		if( iType == M4CL_INTERNAL_TYPE_SYSDB_ITEM )
		{
		    M4CL_SET_STATE_NO_INSITE_FLAG( oNewState.m_Flags ) ;
		}
	}
	
    ret= m_poVM->CreateExecute (oNewState, (m4uint16_t)vNumArgs.Data.DoubleData, vFoo, M4_FALSE) ;
	
	
    if (ret!=M4_SUCCESS){
        DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, pszItem<<pszNode<<""<<"Call" << DumpCallStack() );
    }
	
    return ret;
	
}


//-------------------------------------
//Llamada a método
//-------------------------------------
m4return_t ClVM1Indirections::Call(void)
{
	return( _Call( M4_FALSE ) ) ;
}


//-------------------------------------
//Llamada a método por nodo
//-------------------------------------
m4return_t ClVM1Indirections::CallNode(void)
{
	return( _Call( M4_TRUE ) ) ;
}


//-------------------------------------------------------------------
//INDIRECCIONES CON CANAL
//-------------------------------------------------------------------

m4return_t ClVM1Indirections::GetChannelItemIndirect ( m4VariantType  *ai_variant, m4uint8_t ai_iNumVar, m4bool_t bSlice,  ClItem *& ao_poItem,m4bool_t *aio_pbNewSlices/*=NULL*/,m4bool_t ai_bNeedJustInfo/*=M4_FALSE*/){
    
/*Los argumentos vienen:

  Slice       0
  Registro    1
  Item        2
  Nodo        3
  Canal       4
	*/
	
	
    m4return_t  ret=M4_SUCCESS;
    _ClRegister_Base    *poReg;
    
    m4pcchar_t   pszNode, pszItem, pszChannel, pszThisChannel , pccHostInstance;
    ClNode      *poNode;
    ClAccessRecordSet   *poRecordSet;
    ClValue     *poValue;
    ClItem      *poItem;
    m4uint32_t  hItem, hNode, hMyNode;
    m4uint32_t  idxItem;
    ClAccess    *poAccess;
    ClCompiledMCR   *poThisCMCR=m_oState1.m_poAccess-> GetpCMCR() ;
    m4bool_t    iSameChannel;
	

	if (aio_pbNewSlices){
		*aio_pbNewSlices=M4_FALSE;
	}

    pszThisChannel  = poThisCMCR->GetChannelId() ;
    hMyNode         = poThisCMCR->GetItemNodeHandle    ( m_oState1.m_hItem ) ;
	
    M4CHECK_RES (hMyNode , M4_CH_VM_WRONG_THIS_NODE_HANDLE ,ERRORLOG, "Get|Set ChannelValue" << DumpCallStack() );
    M4CHECK_RES (ai_variant[4].Type==M4CL_CPP_TYPE_STRING_POOL, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "M4ObjectName#Get|SetChannelValue" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(ai_variant[4].Type)<< DumpCallStack() );
    
    pszChannel=strupr(m_oDynamicPool->GetString (ai_variant[4].Data.CharData));
    if ( pszChannel == NULL ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "ChannelId"<<"Get|setChannelValue"<<"<null>" << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
    
    
    if( *pszChannel == '\0' || strcmpi( pszThisChannel, pszChannel ) == 0 )
    {
        iSameChannel = M4_TRUE ;
        poAccess = m_oState1.m_poAccess ;
    }
    else
    {
        iSameChannel = M4_FALSE ;
		
        pccHostInstance = poThisCMCR->GetNodeT3AliasInstanceIdByAlias( hMyNode, pszChannel ) ;
		
        if( pccHostInstance == NULL )
        {
    		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_INSTANCE, pszChannel<<poThisCMCR->iNodeN(hMyNode)<<LogCatAnd<<poThisCMCR->nNodeN(hMyNode)<<"Get|Set ChannelValue" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
            return( M4_ERROR ) ;
        }
		
        ret = m_poVM->GetL2Access( m_oState1.m_poAccess->GetpChannel(), pccHostInstance, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState1.m_Flags), &poAccess ) ;
		
        if( ret != M4_SUCCESS )
        {
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_GET_ACCESS_L2, pccHostInstance<<"Get|Set ChannelValue" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
            return( M4_ERROR ) ;
        }
    }
	
	
    ClCompiledMCR   *poCMCR=poAccess-> GetpCMCR() ;
	
	
    //tramo
    if ( bSlice ){
		
        if (IsNull (ai_variant[0])) { //comprobamos que no sea nulo
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "slice"<<"Set|GetChannelValue" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(ai_variant[0].Type)<<DumpCallStack() );
            return M4_ERROR;
        }
		
        if (ai_variant[0].Type==M4CL_CPP_TYPE_STRING_POOL) { //si es cadena convertimos si se puede
            ret = m_oVariantMediator->ToDouble(ai_variant[0]);
			if (ret!=M4_SUCCESS){
				DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "slice"<<"Set|GetChannelValue" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(ai_variant[0].Type)<< DumpCallStack() );
				return M4_ERROR;
			}
        }
    }
	
    
    //registro
    if ( IsNull ( ai_variant[1] ) ) { //comprobamos que no sea nulo
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "record"<<"Set|GetChannelValue" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(ai_variant[1].Type)<<DumpCallStack() );
        return M4_ERROR;
    }
	
    if ( ai_variant[1].Type!=M4CL_CPP_TYPE_NUMBER ){ //si no es número intentamos convertir
        ret = m_oVariantMediator->ToDouble(ai_variant[1]);
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "record"<<"Set|GetChannelValue" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(ai_variant[1].Type)<<DumpCallStack() );
			return M4_ERROR;
		}
    }
    
	
    //nodo & item
    if  (ai_variant[2].Type !=M4CL_CPP_TYPE_STRING_POOL && ai_variant[2].Type !=M4CL_CPP_TYPE_NUMBER) {
		
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "item"<<"Get|SetChannelValue" << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<","<<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL) )<<M4ClCppType(ai_variant[2].Type)<<DumpCallStack() );
	
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
    
	if ( ai_variant[3].Type !=M4CL_CPP_TYPE_STRING_POOL && ai_variant[3].Type !=M4CL_CPP_TYPE_NUMBER) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "node"<<"Get|SetChannelValue" << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<","<<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL) )<<M4ClCppType(ai_variant[3].Type)<<DumpCallStack() );
		
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
    //--nodo
	if (ai_variant[3].Type==M4CL_CPP_TYPE_STRING_POOL){
        pszNode=m_oDynamicPool->GetString (ai_variant[3].Data.CharData);
        if ( pszNode == NULL ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "NodeId"<<"Get|SetChannelValue"<<"<null>" << DumpCallStack() );
            RuntimePushDouble (M4_ERROR);
            return M4_ERROR;
        }
		
		
		//obtenemos el nodo
		if( iSameChannel == M4_TRUE ){
			if (pszNode[0]==0){ //nodo="" ->Usamos nuestro mismo nodo
				hNode=hMyNode;
				pszNode=poCMCR->GetNodeId (hNode);
			}
			else{
				
				hNode   = poCMCR->GetNodeAliasNodeHandleByAlias ( hMyNode, pszNode, m_oState1.m_poFormula.GetLevel() ) ;
			}
		}
		else{
			hNode   = poCMCR->GetChannelNodeHandleById ( pszNode ) ;
		}
		
		
        if( hNode == 0 ){
            RuntimePushDouble( M4_ERROR ) ;
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, pszNode <<"Set|Get ChannelValue" << DumpCallStack() );
            return M4_ERROR;
        }
        
        poNode=&( poAccess->Node[ ClHandle( hNode ) ] ) ;
    }
    else{ //por indice
        poNode=&( poAccess->Node[ ClNodeIndex( (m4uint32_t)ai_variant[3].Data.DoubleData) ] ) ;
		if (!poNode) {
            RuntimePushDouble( M4_ERROR ) ;
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_INDEX, ((m4uint32_t)ai_variant[3].Data.DoubleData) <<"Set|Get ChannelValue" << DumpCallStack() );
            return M4_ERROR;
		}
        hNode=poNode->GetpNodeDef()->GetHandle()  ;
        pszNode=poNode->GetpNodeDef()->Id()  ;
    }
    
	
    //item
    if (ai_variant[2].Type==M4CL_CPP_TYPE_STRING_POOL){
        pszItem=m_oDynamicPool->GetString (ai_variant[2].Data.CharData);
        if ( pszItem == NULL ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "ItemId"<<"Get|setChannelValue"<<"<null>" << DumpCallStack() );
            RuntimePushDouble (M4_ERROR);
            return M4_ERROR;
        }
        //para poder obtener el scope del item
        hItem = poCMCR->GetNodeItemHandleById( hNode, pszItem );
        if ( hItem == NULL ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, pszItem<<M4ObjNodeN(poCMCR,hNode)<<"Set|Get ChannelValue " << DumpCallStack() );
            RuntimePushDouble ( M4_ERROR );
            return M4_ERROR;
        }
        idxItem=poCMCR->GetItemIndex (hItem);
    }
    else { //por indice
		//Verificamos index item valido
		if (ai_variant[2].Data.DoubleData<0.0 || ai_variant[2].Data.DoubleData>=poCMCR->GetNodeNumberOfItems(hNode) ) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_ITEM_INDEX, (m4int32_t)ai_variant[2].Data.DoubleData << pszNode << "Set|Get ChannelValue " << DumpCallStack() );
            RuntimePushDouble ( M4_ERROR );
            return M4_ERROR;
		}
        idxItem=(m4uint32_t)ai_variant[2].Data.DoubleData;
        hItem=poCMCR->GetNodeItemHandleByPosition (hNode, (m4uint16_t)idxItem);
        pszItem=poCMCR->GetNodeItemIdByPosition (hNode, (m4uint16_t)idxItem);
    }
	
#if _M4_DEBUG > 10    
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Set|Get Value (indirect) (" <<LogCat<< pszNode <<LogCat<< "[" <<LogCat<< (int)ai_variant[1].Data.DoubleData <<LogCat<< "]." <<LogCat<< pszItem <<LogCat<< ")." );
#endif
	
    if ( poNode==NULL ){
        RuntimePushDouble (M4_ERROR);
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, pszNode <<"Set|Get ChannelValue " << DumpCallStack() );
        return M4_ERROR;
    }
    
	
    //el bloque
    poRecordSet = &( poNode->RecordSet ); //casting automatico
    if (!poRecordSet->IsValid() && poCMCR->GetItemScope(hItem) != M4CL_ITEM_SCOPE_NODE){
        RuntimePushDouble (M4_ERROR);
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK, "Set|Get ChannelValue"<<pszNode<<DumpTreePath(poRecordSet->GetpAccess(), hNode) << DumpCallStack() );
        return M4_ERROR;
    }
	
    //el record
    if ( (m4int32_t) ai_variant[1].Data.DoubleData < 0 ){
        poReg=&( poRecordSet->Current );
    }
    else{
        poReg=&( poRecordSet->Register[(m4int32_t)ai_variant[1].Data.DoubleData] );
    }
    
	
	
    //si el registro es EOF y el item es de registro->error
    if ( poReg->Index == M4DM_EOF_INDEX && poCMCR->GetItemScope(hItem) == M4CL_ITEM_SCOPE_REGISTER ) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_NUMBER, M4ObjNodeI(poCMCR,hItem)<<"Set|Get ChannelValue"<<DumpTreePath(poRecordSet->GetpAccess(), hNode)  << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
    //el item
    poItem=&( poReg->Item[ClItemIndex(idxItem)] );
    if ( poItem->GetIndex() == M4DM_EOF_INDEX ) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, pszItem<<M4ObjNodeN(poCMCR,hNode)<<"Set|Get ChannelValue " << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
    //el valor 
    if ( bSlice ){ 
        switch ( ai_variant[0].Type ){
			
        case M4CL_CPP_TYPE_DATE:
            {
                poValue=& ( poItem->Value[ai_variant[0].Data.DoubleData]);
                break;
            }
        default:
            {
                poValue=& ( poItem->Value[ClSliceIndex( (m4int32_t)ai_variant[0].Data.DoubleData) ] );
                break;
            }
        }
    }
    else if ( m_poVM->GetRunSliceMode(poAccess) && !bSlice ) { //tomamos el slice en el que estemod
		/* Bug 0076620
		Ahora el parámetro es de entrada y salida.
		Si se han creado tramos se pone a true
		*/
        if (aio_pbNewSlices){
			ret=m_poVM->CheckAndCreateItemSlice(m_oState1.m_dBeginDate, m_oState1.m_dEndDate, poItem);
            M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_ADDING_SLICE, ERRORLOG, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(), poItem->ItemDef.GetHandle())<<"Get|Set Value" << DumpCallStack() );
			*aio_pbNewSlices=M4_TRUE;
		}
		poValue=& ( poItem->Value[m_oState1.m_dBeginDate] );
		//comprobamos que todo el tramo actual esté dentro del periodo definido
		if (poValue->Index != M4DM_EOF_INDEX && m_oState1.m_dEndDate >poValue->GetLastEndDate()){
			poValue->MoveToEOF();
		}

    }
    else{
        poValue=&( poItem->Value );
    }
	
    //comprobamos que el valor no sea NULL
    if ( poValue->Index == M4DM_EOF_INDEX && !ai_bNeedJustInfo){
        if ( bSlice ){
            m_oVariantMediator->ToPoolString ( ai_variant[0] );
            DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE, m_oDynamicPool->GetString(ai_variant[0].Data.CharData)<<M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(), poItem->ItemDef.GetHandle())<<"Get|Set ChannelValue" << DumpCallStack() );
            m_oDynamicPool->DeleteString( ai_variant[0].Data.CharData );
        }
        else{
            DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE, BeginLogCat<<DateLog(m_oState1.m_dBeginDate)<<" - "<<DateLog(m_oState1.m_dEndDate)<<EndLogCat <<M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(), poItem->ItemDef.GetHandle())<<"Get|Set ChannelValue" << DumpCallStack() );
        }
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
        //return M4_SUCCESS;
    }
	
    ao_poItem=poItem; //para retronar un puntero al item
	
	
	//   RuntimePushDouble (M4_SUCCESS);
    return M4_SUCCESS;
	
	
	
}


//-------------------------------------
//Set channel value
//-------------------------------------

m4return_t ClVM1Indirections::SetChannelValue			(void){
	m4VariantType aParam[5],  vNum, Value;
    m4return_t  ret=M4_SUCCESS;
    m4bool_t    bSlice=M4_FALSE;
	m4bool_t	bNewSlices=M4_FALSE;
    m4uint32_t  i, iNumArgs;
    ClItem *poItem;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "SetChannelValue (indirect)" );
#endif
    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetChannelValue (indirect).")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
    //demasiados parametros 
    if ( iNumArgs> 6 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "SetChannelValue"<<6<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 6 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetChannelValue (indirect).")	;
        }
        iNumArgs=6; //ya hemos sacado los sobrantes
    }
    
	
    //Valor a escribir
    ret=RuntimePop(Value);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetChannelValue (indirect).")	;
    
    //con tramo
    if ( iNumArgs > 5 ){
        ret=RuntimePop(aParam[0]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetChannelValue (indirect).")	;
        
        bSlice=M4_TRUE;
    }
	
    
    //registro, item, nodo, canal
    for (i=1; i < 5 ; ++i){
        ret=RuntimePop(aParam[i]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetChannelValue (indirect).")	;
    }
	
	m4return_t retVal=M4_SUCCESS;
    //obtenemos el puntero al item
    ret=  GetChannelItemIndirect ( aParam, 5, bSlice, poItem, &bNewSlices);
    if (ret != M4_SUCCESS){
		retVal=M4_ERROR;
	}
	else{
		if (poItem->ItemDef.Type()==M4CL_ITEM_TYPE_METHOD){
			DUMP_CHLOG_ERRORF(M4_CH_VM_TRY_WRITE_METHOD, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle() )<<"SetValue" << DumpCallStack() );
			retVal=M4_ERROR;
		}
		else{
			
			//metemos el valor 
			/* Bug 0076620
			Si se han creado tramos se asigna entre fechas
			*/
			if (bNewSlices==M4_TRUE)
			{
				ret=m_poVM->SetValue(*poItem, Value, M4CL_CM_DEFAULT_PRIOR, 0, m_oState1.m_dBeginDate, m_oState1.m_dEndDate, M4_TRUE);
			}
			else
			{
				// Chequeo de uso de un concepto tramado en modo no tramos
				m4uint32_t iSlices = poItem->Value.Count() ;

				if( iSlices > 0 )
				{
				    m4char_t	acDate[ M4_SECOND_END + 1 ] ;
					ClDateToString( m_oState1.m_dDate, acDate, M4_SECOND_END + 1 ) ;
					DUMP_CHLOG_WARNINGF( M4_CH_VM_BASE_SLICED_ITEM_USE, M4ObjNodeItemI( poItem->GetpAccess()->GetpCMCR(), poItem->ItemDef.GetHandle() ) << iSlices << poItem->Value.Index << m_oState1.m_iSourceLine << m_oState1.m_poAccess->GetpCMCR()->GetItemFormIdByPosition( m_oState1.m_hItem, m_oState1.m_iRule ) << M4ObjNodeItemI( m_oState1.m_poAccess->GetpCMCR(), m_oState1.m_hItem ) << acDate << DumpCallStack() ) ;
				}

				ret=m_poVM->__SetValue(*poItem, Value, M4CL_CM_DEFAULT_PRIOR, 0);
			}

			if (ret==M4_WARNING){
				DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle() )<<"SetChannelValue" << DumpCallStack() );
			}
			else if (ret!=M4_SUCCESS){
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle() )<<"SetChannelValue" << DumpCallStack() );
			}
			
			if (Value.Type==M4CL_CPP_TYPE_STRING_POOL){
				m_oDynamicPool->DeleteString (Value.Data.CharData);
			}
			RuntimePushDouble (ret);
		}
	}
	
	//freedom
    if ( iNumArgs > 5 ){
        DeleteVarString(aParam[0]);
    }
    //registro, item, nodo, canal
    for (i=1; i < 5 ; ++i){
        DeleteVarString(aParam[i]);
    }
	
    return retVal;
}




m4return_t ClVM1Indirections::GetChannelValue			(void){
    
    m4VariantType aParam[5],  vNum, Value;
    m4return_t  ret=M4_SUCCESS;
    m4bool_t    bSlice=M4_FALSE;
    m4uint32_t  i, iNumArgs;
	m4date_t	dStartDate = -1;
	m4date_t	dEndDate = -1;
    ClItem *poItem;
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "GetChannelValue (indirect)" );
#endif
    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetChannelValue (indirect).")	;
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
	
    //demasiados parametros 
    if ( iNumArgs> 5 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "GetChannelValue"<<5<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 5 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetChannelValue (indirect).")	;
        }
        iNumArgs=5; //ya hemos sacado los sobrantes
    }
    
    
    //con tramo
    if ( iNumArgs > 4 ){
        ret=RuntimePop(aParam[0]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetChannelValue (indirect).")	;
        
        bSlice=M4_TRUE;
    }
	
    
    //registro, item, nodo, channel
    for (i=1; i < 5 ; ++i){
        ret=RuntimePop(aParam[i]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetChannelValue (indirect).")	;
    }
	
	m4return_t retVal=M4_SUCCESS;
    //obtenemos el puntero al item
    ret=  GetChannelItemIndirect ( aParam, 5, bSlice, poItem);
    if (ret!=M4_SUCCESS){
		retVal=M4_ERROR;
	}
	else{
		if (poItem->ItemDef.Type()==M4CL_ITEM_TYPE_METHOD){
			DUMP_CHLOG_ERRORF(M4_CH_VM_TRY_READ_METHOD, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle() )<<"SetValue" << DumpCallStack() );
			retVal=M4_ERROR;
		}
		else{
			
			//leemos el valor y lo metemos en la pila
			if (m_poVM->GetRunSliceMode(poItem->GetpAccess())&& !bSlice ){ //tomamos el slice en el que estemod
				ret=poItem->Value.Get (m_oState1.m_dBeginDate, m_oState1.m_dEndDate, Value);

				dStartDate = m_oState1.m_dBeginDate;
				dEndDate = m_oState1.m_dEndDate;
			}
			else{
				// Chequeo de uso de un concepto tramado en modo no tramos
				m4uint32_t iSlices = poItem->Value.Count() ;

				if( iSlices > 0 )
				{
				    m4char_t	acDate[ M4_SECOND_END + 1 ] ;
					ClDateToString( m_oState1.m_dDate, acDate, M4_SECOND_END + 1 ) ;
					DUMP_CHLOG_WARNINGF( M4_CH_VM_BASE_SLICED_ITEM_USE, M4ObjNodeItemI( poItem->GetpAccess()->GetpCMCR(), poItem->ItemDef.GetHandle() ) << iSlices << poItem->Value.Index << m_oState1.m_iSourceLine << m_oState1.m_poAccess->GetpCMCR()->GetItemFormIdByPosition( m_oState1.m_hItem, m_oState1.m_iRule ) << M4ObjNodeItemI( m_oState1.m_poAccess->GetpCMCR(), m_oState1.m_hItem ) << acDate << DumpCallStack() ) ;
				}

				ret=poItem->Value.Get (Value);
			}
			if (ret!=M4_SUCCESS){
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle()) <<"GetChannelValue" << DumpCallStack() );
				
			}
			else{
				if ( (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_READS) )
				{
					m4VMState_t oState;
					m_oStateStack->Top(oState);
					GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddRead( *poItem, Value, dStartDate, dEndDate, oState, DumpCallStack( &oState, M4_TRUE ).c_str() );
				}
			}

			if (Value.Type==M4CL_CPP_TYPE_STRING_VAR){
				m_oVariantMediator->ToPoolString (Value);
			}
			
			RuntimePush (Value);
		}
	}
	
	//fredom
    if ( iNumArgs > 4 ){
        DeleteVarString(aParam[0]);
    }
    for (i=1; i < 5 ; ++i){
		DeleteVarString(aParam[i]);
    }
	
    return retVal;
	
}



m4return_t ClVM1Indirections::_ChannelCall(m4uint32_t ai_iArguments, ClItem* &ao_rpItem)
{

	m4VariantType		vNode, vItem, vChannel;
    m4return_t			ret=M4_SUCCESS;
    _ClRegister_Base	*poReg;
	m4pcchar_t			pszNode, pszItem, pszChannel, pszThisChannel, pccHostInstance;
    ClNode				*poNode;
    ClAccessRecordSet	*poAccessRecordSet ;
    ClAccessRecordSet	*poRecordSet;
    ClItem				*poItem;
    m4uint32_t			hItem, iNumArgsNeeded, hNode, hMyNode, idxItem;
	ClAccess			*poAccess;
    ClCompiledMCR		*poThisCMCR=m_oState1.m_poAccess-> GetpCMCR() ;
    m4bool_t			iSameChannel;
	

	ao_rpItem = NULL;
    
    pszThisChannel  = poThisCMCR->GetChannelId() ;
    hMyNode         = poThisCMCR->GetItemNodeHandle    ( m_oState1.m_hItem ) ;
    M4CHECK_RES (hMyNode , M4_CH_VM_WRONG_THIS_NODE_HANDLE ,ERRORLOG, "ChannelCall" << DumpCallStack() );

    //item
	ret=RuntimePop(vItem);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ChannelCall.")	;
    
    //nodo
	ret=RuntimePop(vNode);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ChannelCall.")	;
    
    ret=RuntimePop(vChannel);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ChannelCall.")	;
    
    if (vNode.Type !=M4CL_CPP_TYPE_STRING_POOL  && vNode.Type !=M4CL_CPP_TYPE_NUMBER ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "NodeId"<<"ChannelCall" << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<","<<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL) )<<M4ClCppType(vNode.Type)<<DumpCallStack() );

        RuntimePushDouble (M4_ERROR);
		DeleteVarString (vItem);  //intentamos liberar todo
		DeleteVarString (vNode);
		DeleteVarString (vChannel);
        return M4_ERROR;
    }
	if (vItem.Type!=M4CL_CPP_TYPE_STRING_POOL  && vItem.Type!=M4CL_CPP_TYPE_NUMBER){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "itemId"<<"ChannelCall" << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<","<<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL) )<<M4ClCppType(vItem.Type)<<DumpCallStack() );
		
        RuntimePushDouble (M4_ERROR);
		DeleteVarString (vItem);  //intentamos liberar todo
		DeleteVarString (vNode);
		DeleteVarString (vChannel);
        return M4_ERROR;
    }
	
    if (vChannel.Type!=M4CL_CPP_TYPE_STRING_POOL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "ChannelId"<<"ChannelCall" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vChannel.Type)<<DumpCallStack() );
		DeleteVarString (vItem);  //intentamos liberar
		DeleteVarString (vNode);
		return M4_ERROR;
	}
	
	//los argumentos son válidos->...
    m4return_t retVal=M4_SUCCESS;
	
    pszChannel=strupr(m_oDynamicPool->GetString (vChannel.Data.CharData));
	m_oDynamicPool->DeleteString (vChannel.Data.CharData); //pos si hay errores, vamos liberando
	
    if ( pszChannel == 0 ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "ChannelId"<<"ChannelCall"<<"<null>" << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        retVal = M4_ERROR ;
    }
    
    else{
		if( *pszChannel == '\0' || strcmpi( pszThisChannel, pszChannel ) == 0 )
		{
			iSameChannel = M4_TRUE ;
			poAccess = m_oState1.m_poAccess ;
		}
		else
		{
			iSameChannel = M4_FALSE ;
			
			pccHostInstance = poThisCMCR->GetNodeT3AliasInstanceIdByAlias( hMyNode, pszChannel ) ;
			
			if( pccHostInstance == NULL )
			{
	    		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_INSTANCE, pszChannel<<poThisCMCR->iNodeN(hMyNode)<<LogCatAnd<<poThisCMCR->nNodeN(hMyNode)<<"ChannelCall" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
				retVal = M4_ERROR ;
			}
			else{
				
				ret = m_poVM->GetL2Access( m_oState1.m_poAccess->GetpChannel(), pccHostInstance, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState1.m_Flags), &poAccess ) ;
				
				if( ret != M4_SUCCESS )
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_NO_GET_ACCESS_L2, pccHostInstance<<"ChannelCall" << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					retVal = M4_ERROR ;
				}
			}
		}
		
	}
	
	if (retVal !=M4_SUCCESS){ //PS.:error en la obtención del canal->liberamos y salimos
		DeleteVarString (vItem);  
		DeleteVarString (vNode);
		return retVal;
	}
	
	
    ClCompiledMCR   *poCMCR=poAccess-> GetpCMCR() ;
    
	
    //nodo
    if (vNode.Type==M4CL_CPP_TYPE_STRING_POOL){
        pszNode=m_oDynamicPool->GetString (vNode.Data.CharData);
		m_oDynamicPool->DeleteString (vNode.Data.CharData); //liberamos por si hay error
		
        if ( pszNode == NULL ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "NodeId"<<"ChannelCall"<<"<null>" << DumpCallStack() );
            RuntimePushDouble (M4_ERROR);
            retVal=M4_ERROR;
        }
		else{       
			
			
			//obtenemos el nodo
			if( iSameChannel == M4_TRUE ){
				if (pszNode[0]==0){ //nodo="" ->Usamos nuestro mismo nodo
					hNode=hMyNode;
					pszNode=poCMCR->GetNodeId (hNode);
				}
				else{
					hNode   = poCMCR->GetNodeAliasNodeHandleByAlias ( hMyNode, pszNode, m_oState1.m_poFormula.GetLevel() ) ;
				}
			}
			else{ 
				hNode   = poCMCR->GetChannelNodeHandleById ( pszNode ) ;
			}
			
			if( hNode == 0 ){
				RuntimePushDouble( M4_ERROR ) ;
				DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, pszNode <<"ChannelCall" << DumpCallStack() );
				
				retVal=M4_ERROR;
			}
			else{
				poNode=&( poAccess->Node[ ClHandle( hNode ) ] ) ;
			}
		}
    }
    else{ //por indice
        poNode=&( poAccess->Node[ ClNodeIndex( (m4uint32_t)vNode.Data.DoubleData) ] ) ;
		if (!poNode) {
            RuntimePushDouble( M4_ERROR ) ;
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_INDEX, ((m4uint32_t)vNode.Data.DoubleData) <<"ChannelCall" << DumpCallStack() );
            return M4_ERROR;
		}
        hNode=poNode->GetpNodeDef()->GetHandle()  ;
        pszNode=poNode->GetpNodeDef()->Id()  ;
    }
    
	if (retVal !=M4_SUCCESS){ //PS.:error en la obtención del Nodo->liberamos y salimos
		DeleteVarString (vItem);  
		return retVal;
	}
	
	
	
    //item
    if (vItem.Type==M4CL_CPP_TYPE_STRING_POOL){
        pszItem=m_oDynamicPool->GetString (vItem.Data.CharData);
		
		m_oDynamicPool->DeleteString (vItem.Data.CharData); //liberamos por si hay error
		
        if ( pszItem == NULL ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "ItemId"<<"ChannelCall"<<"<null>" << DumpCallStack() );
            RuntimePushDouble (M4_ERROR);
            return M4_ERROR;
        }
        //para poder obtener el scope del item
        hItem = poCMCR->GetNodeItemHandleById( hNode, pszItem );
        if ( hItem == NULL ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, pszItem<<M4ObjNodeN(poCMCR,hNode)<<"ChannelCall" << DumpCallStack() );
            RuntimePushDouble ( M4_ERROR );
            return M4_ERROR;
        }
        idxItem=poCMCR->GetItemIndex (hItem);
    }
    else { //por indice
		//Verificamos index item valido
		if (vItem.Data.DoubleData<0.0 || vItem.Data.DoubleData>=poCMCR->GetNodeNumberOfItems(hNode) ) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_ITEM_INDEX, (m4int32_t)vItem.Data.DoubleData << pszNode << "ChannelCall" << DumpCallStack() );
            RuntimePushDouble ( M4_ERROR );
            return M4_ERROR;
		}
        idxItem=(m4uint32_t)vItem.Data.DoubleData;
        hItem=poCMCR->GetNodeItemHandleByPosition (hNode, (m4uint16_t)idxItem);
        pszItem=poCMCR->GetNodeItemIdByPosition (hNode, (m4uint16_t)idxItem);
    }
	
	//comprobamos que sea ejecutable
	if (poCMCR->GetItemType(hItem) != M4CL_ITEM_TYPE_METHOD && poCMCR->GetItemType(hItem) != M4CL_ITEM_TYPE_CONCEPT){
		DUMP_CHLOG_ERRORF(M4_CH_VM_TRY_EXECUTE_FIELD_PROP, M4ObjNodeItemI(poCMCR,hItem)<<"ChannelCall" << DumpCallStack() );
		return M4_ERROR;
	}

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: ChannelCall (indirect) (" <<LogCat<< pszNode <<LogCat<< "." <<LogCat<< pszItem  <<LogCat<< ")." );
#endif
	
    //el bloque
    poAccessRecordSet = &(poNode->RecordSet); //casting automatico
    if (!poAccessRecordSet ->IsValid() && poCMCR->GetItemScope(hItem) != M4CL_ITEM_SCOPE_NODE){
        RuntimePushDouble (M4_ERROR);
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK, "ChannelCall"<<pszNode<<DumpTreePath(poAccessRecordSet->GetpAccess(), hNode) << DumpCallStack() );
        return M4_ERROR;
    }
	
	//el record
    poReg=&(poAccessRecordSet ->Current);
    // despues de obtener el current usamos un recordset normal, no de access
    poRecordSet=&(poNode->RecordSet);   // directamente el del access !
	
    //si el registro es EOF y el item es de registro->error
    if (poReg->Index == M4DM_EOF_INDEX && poCMCR->GetItemScope(hItem) == M4CL_ITEM_SCOPE_REGISTER ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_NUMBER, M4ObjNodeI(poCMCR,hItem)<<"ChannelCall"<<DumpTreePath(poRecordSet->GetpAccess(), hNode)  << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
    //el item
    poItem=&(poReg->Item[ ClItemIndex (idxItem) ]);
    if (poItem->GetIndex() == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, pszItem<<M4ObjNodeN(poCMCR,hNode)<<"ChannelCall" << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
    //comprobamos que el número de argumentos sea correcto
    iNumArgsNeeded= poCMCR->GetItemNumberOfArguments(hItem);
    m4int8_t bVarArg=poCMCR->GetItemVariableArguments(hItem);
	
    if (bVarArg!=0 && ai_iArguments < iNumArgsNeeded){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_ITEM_PARAM_NUM, pszItem<<pszNode<<M4Obj(poCMCR)<< ">=" << LogCat << iNumArgsNeeded << ai_iArguments<< DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
    else if (bVarArg==0 && ai_iArguments != iNumArgsNeeded){ //error ->nos faltan argumentos
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_ITEM_PARAM_NUM, pszItem<<pszNode<<M4Obj(poCMCR)<<iNumArgsNeeded << ai_iArguments<< DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	
    if (poCMCR->GetItemNumberOfReferedArguments(hItem) !=0  ){ //error ->El item necesita arguemtnso por referencia-- No lo permitimos
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_REFERED_ARG_ALLOWED, M4ObjNodeItemI(poCMCR,hItem)<<"ChannelCall" << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }

	ao_rpItem = poItem;

	return M4_SUCCESS;
}


m4return_t ClVM1Indirections::ChannelCall(void)
{

    m4return_t			ret = M4_SUCCESS;
    m4uint32_t          hItem;
	m4VariantType		vNumArgs, vFoo;
    ClItem				*poItem;
    ClAccessRecordSet	*poRecordSet;
	ClAccess			*poAccess;
	
	
    //Numero de argumentos 
	ret=RuntimePopDouble(vNumArgs);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ChannelCall.");
    
    //descontamos los tres argumentos del call
    vNumArgs.Data.DoubleData-=3;


	ret = _ChannelCall( vNumArgs.Data.DoubleData, poItem ) ;
	
    if (ret!=M4_SUCCESS){
        return M4_ERROR;
    }

	hItem = poItem->ItemDef.GetHandle();
	poRecordSet = &(poItem->GetpNodeAccess()->RecordSet);
	poAccess = poRecordSet->GetpAccess();
	
    //en lugar de meter resultado dejamos el que deja el 
    //metodo que estamos ejecutando
	
    m4VMState_t oNewState;
	
    oNewState.m_dDate=m_oState1.m_dDate;
    oNewState.m_dBeginDate=m_oState1.m_dBeginDate;
    oNewState.m_dEndDate=m_oState1.m_dEndDate;
    oNewState.m_poAccess=poAccess;
    oNewState.m_hItem=hItem;
    oNewState.m_iRule=0;
    oNewState.m_Flags=0;
    oNewState.m_poRecordSet=poRecordSet;
    oNewState.m_oRegisterIndex=poRecordSet->Current.Index ;
	//Bug:0064136
	// Cálculo correcto del m_iParentStatePos
	oNewState.m_iParentStatePos=m_oStateStack->GetPos();  
    ret= m_poVM->CreateExecute (oNewState, (m4uint16_t)vNumArgs.Data.DoubleData, vFoo, M4_FALSE);
	
	
    if (ret!=M4_SUCCESS){
        DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poAccess->GetpCMCR(),hItem)<<"ChannelCall" << DumpCallStack() );
    }
    
    return ret;
}


#define	M4_ALLOWED_METHODS			8

static m4pchar_t	s_apcAllowedRunAsMethods[M4_ALLOWED_METHODS][3] =
{
	{ "M4RTR_INJECTOR", "M4RTR_INJECTION_LITERALS", "INJECT" },
	{ "SRTC_M4UNIT", "SRTC_M4UNIT", "LOAD_DATASET" },
	{ "SWF_ENGINE_RT", "SWF_BPO", "INSTANTIATEBPO" },
	{ "SWF_ENGINE_RT", "SWF_BPO", "INSTANCIATENEWSTATE" },
	{ "SWF_ENGINE_RT", "SWF_BPO", "PROCESSWFEVENT" },
	{ "SWF_ENGINE_RT", "SWF_BPO", "RECEIVEWFEVENT" },
	{ "SWF_ENGINE_RT", "SWF_BPO", "RECEIVEWFEVENT_ARG" },
	{ "SWF_ENGINE_RT", "SWF_INST_TASK", "GENERATEWORKITEMS" }
};


m4return_t ClVM1Indirections::ChannelCallAs(void)
{

    m4return_t		ret = M4_SUCCESS;
	m4bool_t		bFound;
	m4int32_t		iArguments;
	m4int32_t		i;
	m4int32_t		iLength;
	m4pchar_t		pcRole;
	m4pchar_t		pcRSM;
	m4pchar_t		pcNewRole;
	m4pchar_t		pcOldRole;
	m4pchar_t		pcOldRSM;
	m4pcchar_t		pccThisChannel;
	m4pcchar_t		pccThisNode;
	m4pcchar_t		pccThisItem;
	m4VariantType	vNumArgs;
	m4VariantType	vRole;
    ClItem			*poItem;
	ClChannel		*poChannel;
	ClCompiledMCR	*poThisCMCR;
	
	
    //Numero de argumentos 
	ret=RuntimePopDouble(vNumArgs);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ChannelCall.");
    
    //descontamos los cuatro argumentos del call
	iArguments = vNumArgs.Data.DoubleData - 4;

    //role
	ret=RuntimePop(vRole);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ChannelCall.")	;

    if (vRole.Type!=M4CL_CPP_TYPE_STRING_POOL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Role"<<"ChannelCall" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vRole.Type)<<DumpCallStack() );
		return M4_ERROR;
	}

    pcNewRole = m_oDynamicPool->GetString (vRole.Data.CharData);
	m_oDynamicPool->DeleteString (vRole.Data.CharData); //pos si hay errores, vamos liberando

	if( strcmp( pcNewRole, M4CL_SEC_SUPER_USER ) != 0 )
	{
		// De momento sólos e deja pasara a super usuario
        return M4_ERROR;
	}

	// De momento sólo se dejan ciertos métodos
	poThisCMCR = m_oState1.m_poAccess->GetpCMCR() ;
    pccThisChannel = poThisCMCR->GetChannelId() ;
	pccThisNode = poThisCMCR->GetNodeId( poThisCMCR->GetItemNodeHandle( m_oState1.m_hItem ) ) ;
	pccThisItem = poThisCMCR->GetItemId( m_oState1.m_hItem ) ;

	bFound = M4_FALSE ;

	for( i = 0 ; i < M4_ALLOWED_METHODS && bFound == M4_FALSE ; i++ )
	{
		if(
				strcmp( s_apcAllowedRunAsMethods[ i ][ 0 ], pccThisChannel ) == 0
			&&	strcmp( s_apcAllowedRunAsMethods[ i ][ 1 ], pccThisNode ) == 0
			&&	strcmp( s_apcAllowedRunAsMethods[ i ][ 2 ], pccThisItem ) == 0
			)
		{
			bFound = M4_TRUE;	
		}
	}

	if( bFound == M4_FALSE )
	{
		return( M4_ERROR ) ;
	}

	ret = _ChannelCall( iArguments, poItem ) ;
	
    if (ret!=M4_SUCCESS){
        return M4_ERROR;
    }

	poChannel = poItem->GetpAccess()->GetpChannel();

	if( poChannel == NULL )
	{
		return M4_ERROR;
	}

	/* Bug 0203430
	Se guarda el role y RSM viejos
	*/
	pcRole = NULL ;
	ret = poChannel->Role_ID.Get( pcRole ) ;

    if( ret != M4_SUCCESS || pcRole == NULL )
	{
        return M4_ERROR;
    }

	pcRSM = NULL ;
	ret = poChannel->RSM_ID.Get( pcRSM ) ;

    if( ret != M4_SUCCESS || pcRSM == NULL )
	{
        return M4_ERROR;
    }

	iLength = strlen( pcRole ) ;
	pcOldRole = new m4char_t[ iLength + 1 ] ;
	CHECK_CHLOG_ERRORF( pcOldRole == NULL, M4_ERROR, M4_CH_VM_OUT_OF_MEMORY, m4uint32_t( sizeof( m4char_t ) * ( iLength + 1 ) ) << __FILE__ << __LINE__ ) ;
	memcpy( pcOldRole, pcRole, iLength + 1 );

	iLength = strlen( pcRSM ) ;
	pcOldRSM = new m4char_t[ iLength + 1 ] ;
	CHECK_CHLOG_ERRORF( pcOldRSM == NULL, M4_ERROR, M4_CH_VM_OUT_OF_MEMORY, m4uint32_t( sizeof( m4char_t ) * ( iLength + 1 ) ) << __FILE__ << __LINE__ ) ;
	memcpy( pcOldRSM, pcRSM, iLength + 1 );

	// Se cambia el role
	poChannel->RSM_ID.SetFlags( M4_PROP_WRITE ) ;
	poChannel->Role_ID.SetFlags( M4_PROP_WRITE ) ;
	poChannel->Role_ID.Set( pcNewRole ) ;
	poChannel->RSM_ID.Set( pcNewRole ) ;
	poChannel->RSM_ID.UnSetFlags( M4_PROP_WRITE ) ;
	poChannel->Role_ID.UnSetFlags( M4_PROP_WRITE ) ;

	ret = poItem->Call( NULL, iArguments ) ;

	// Se restablece el role
	poChannel->RSM_ID.SetFlags( M4_PROP_WRITE ) ;
	poChannel->Role_ID.SetFlags( M4_PROP_WRITE ) ;
	poChannel->Role_ID.Set( pcOldRole ) ;
	poChannel->RSM_ID.Set( pcOldRSM ) ;
	poChannel->RSM_ID.UnSetFlags( M4_PROP_WRITE ) ;
	poChannel->Role_ID.UnSetFlags( M4_PROP_WRITE ) ;

	delete pcOldRole;
	delete pcOldRSM;

    if( ret != M4_SUCCESS )
	{
        return M4_ERROR;
    }
    
    return ret;
}



//---------------------------------------------
//Atributos de canal
//---------------------------------------------

//
//Leer attribute
m4return_t ClVM1Indirections::GetChannelAttribValue	(void){

    m4VariantType aParam[5],  vNum, Value, vNumArgToRead;
    m4return_t  ret=M4_SUCCESS;
    m4uint32_t  i;
    ClItem *poItem;
    m4return_t retVal=M4_SUCCESS;

#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "GetChannelAttribValue (indirect)");
#endif
  
	//Sacamos el número del argumento a leer
	ret=RuntimePop(vNumArgToRead);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetChannelAttribValue (indirect). ")	;
	if (IsNull( vNumArgToRead ) ){
		ret=M4_ERROR;
	}
	else{
		ret=m_oVariantMediator->ToDouble (vNumArgToRead);
	}
	if (ret != M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Attribute"<<"GetChannelAttribValue" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumArgToRead.Type)<< DumpCallStack() );

		return M4_ERROR;
	}

    //item, nodo, channel
    for (i=2; i < 5 ; ++i){
        ret=RuntimePop(aParam[i]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetChannelAttribValue (indirect).")	;
    }
	//el registro lo ponemos a cañon a -1
	aParam[1].Data.DoubleData=-1.0;
	aParam[1].Type=M4CL_CPP_TYPE_NUMBER;

	
    //obtenemos el puntero al item
    ret=  GetChannelItemIndirect ( aParam, 5, M4_FALSE, poItem,NULL,M4_TRUE/*justInfo*/);
    if (ret!=M4_SUCCESS){
		retVal=M4_ERROR;
	}
	else{
	/*	//al objeto de los argumentos argumento
		m4int16_t iFlags=0;
		//metemos slice si viene, en los flags y en la pila
		if (bSlice){
			M4CL_SET_TRAME_FLAG(iFlags);
			RuntimePush(aParam[0]);
		}
		//metemos el registro (si <0, es decir que no quieren el current) en flags y pila
		if (aParam[1].Data.DoubleData<0){
			M4CL_SET_ROW_FLAG(iFlags);
			RuntimePush(aParam[1]);
		}

		m_poVM->m_poAttribute->SetContext (poItem->GetpAccess(), iFlags,  poItem->ItemDef.NodeHandle(), poItem->ItemDef.GetHandle(), m_bRunSliceMode, m_oState1.m_dBeginDate, m_oState1.m_dEndDate);
	*/	
		m_poVM->m_poAttribute->SetContext ( poItem->GetpAccess(), 0,  poItem->ItemDef.NodeHandle(), poItem->ItemDef.GetHandle(), m_poVM->GetRunSliceMode(poItem->GetpAccess()), m_oState1.m_dBeginDate, m_oState1.m_dEndDate);
		
		//leemos el attributo (saca y mete en la pila lo que debe)
		m4return_t retVal=m_poVM->m_poAttribute->Get( (m4uint8_t)vNumArgToRead.Data.DoubleData );
		if (retVal==M4_ERROR){
            DUMP_CHLOG_ERRORF(M4_CH_VM_READING_ATT, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle())<<"ChannelAttribGetValue" << DumpCallStack() );
		}
	}
	
	//fredom
    for (i=2; i < 5 ; ++i){
		DeleteVarString(aParam[i]);
    }
	
    return retVal;
}


//
//Leer attribute de tramo
m4return_t ClVM1Indirections::GetChannelAttribSliceValue	(void){

    m4VariantType aParam[5],  vNum, Value, vNumArgToRead;
    m4return_t  ret=M4_SUCCESS;
    m4uint32_t  i;
    ClItem *poItem;
    m4return_t retVal=M4_SUCCESS;

#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "GetChannelAttribSliceValue (indirect)");
#endif
  
	//Sacamos el número del argumento a leer
	ret=RuntimePop(vNumArgToRead);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetChannelAttribSliceValue (indirect). ")	;
	if (IsNull( vNumArgToRead ) ){
		ret=M4_ERROR;
	}
	else{
		ret=m_oVariantMediator->ToDouble (vNumArgToRead);
	}
	if (ret != M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Attribute"<<"GetChannelAttribSliceValue" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumArgToRead.Type)<< DumpCallStack() );

		return M4_ERROR;
	}

    //tramo
    ret=RuntimePop(aParam[0]);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetChannelAttribSliceValue (indirect).")	;

    //item, nodo, channel
    for (i=2; i < 5 ; ++i){
        ret=RuntimePop(aParam[i]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetChannelAttribSliceValue (indirect).")	;
    }
	//el registro lo ponemos a cañon a -1
	aParam[1].Data.DoubleData=-1.0;
	aParam[1].Type=M4CL_CPP_TYPE_NUMBER;

	
    //obtenemos el puntero al item
    ret=  GetChannelItemIndirect ( aParam, 5, M4_TRUE, poItem,NULL,M4_TRUE/*justInfo*/);
    if (ret!=M4_SUCCESS){
		retVal=M4_ERROR;
	}
	else{
	/*	//al objeto de los argumentos argumento
		m4int16_t iFlags=0;
		//metemos slice si viene, en los flags y en la pila
		if (bSlice){
			M4CL_SET_TRAME_FLAG(iFlags);
			RuntimePush(aParam[0]);
		}
		//metemos el registro (si <0, es decir que no quieren el current) en flags y pila
		if (aParam[1].Data.DoubleData<0){
			M4CL_SET_ROW_FLAG(iFlags);
			RuntimePush(aParam[1]);
		}

		m_poVM->m_poAttribute->SetContext (poItem->GetpAccess(), iFlags,  poItem->ItemDef.NodeHandle(), poItem->ItemDef.GetHandle(), m_bRunSliceMode, m_oState1.m_dBeginDate, m_oState1.m_dEndDate);
	*/	
		m4int16_t iFlags=0;
		//metemos slice si viene, en los flags y en la pila
		M4CL_SET_TRAME_FLAG(iFlags);
		RuntimePush(aParam[0]);

		m_poVM->m_poAttribute->SetContext ( poItem->GetpAccess(), iFlags,  poItem->ItemDef.NodeHandle(), poItem->ItemDef.GetHandle(), m_poVM->GetRunSliceMode(poItem->GetpAccess()), m_oState1.m_dBeginDate, m_oState1.m_dEndDate);
		
		//leemos el attributo (saca y mete en la pila lo que debe)
		m4return_t retVal=m_poVM->m_poAttribute->Get( (m4uint8_t)vNumArgToRead.Data.DoubleData );
		if (retVal==M4_ERROR){
            DUMP_CHLOG_ERRORF(M4_CH_VM_READING_ATT, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle())<<"ChannelAttribGetValue" << DumpCallStack() );
		}
	}
	
	//fredom
    for (i=1; i < 5 ; ++i){
		DeleteVarString(aParam[i]);
    }
	
    return retVal;
}


//Execute Attribute
m4return_t ClVM1Indirections::ChannelAttribCall		(void){

	m4VariantType aParam[5],  vNum, Value, vNumArgToExe;
    m4return_t  ret=M4_SUCCESS;
	m4int8_t  i, iNumArgs, iNumArgsNeeded;
    ClItem *poItem;
	m4return_t retVal=M4_SUCCESS;
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "ChannelAttribCall (indirect)");
#endif
    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ChannelAttribCall (indirect).")	;
    iNumArgs= (m4int8_t)vNum.Data.DoubleData ;
	
	//Los argumentos tienen que ser >=4(por el prototipo)
    
	//Sacamos el número del argumento a ejecutar
	ret=RuntimePop(vNumArgToExe);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ChannelAttribCall (indirect).")	;
	if (IsNull(vNumArgToExe)){
		ret=M4_ERROR;
	}
	else{
		ret=m_oVariantMediator->ToDouble (vNumArgToExe);
	}
	if (ret != M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Attribute"<<"ChannelAttribCall" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumArgToExe.Type)<<DumpCallStack() );
		return M4_ERROR;
	}

    
    //item, nodo, channel
    for (i=2; i < 5 ; ++i){
        ret=RuntimePop(aParam[i]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ChannelAttribCall (indirect).")	;
    }
	//el registro lo ponemos a cañon a -1
	aParam[1].Data.DoubleData=-1.0;
	aParam[1].Type=M4CL_CPP_TYPE_NUMBER;

	

    //obtenemos el puntero al item
    ret=  GetChannelItemIndirect ( aParam, 5, M4_FALSE, poItem,NULL,M4_TRUE/*justInfo*/);
    if (ret!=M4_SUCCESS){
		retVal=M4_ERROR;
	}
	else{

		//
		//comprobación de argumentos
		//
		iNumArgsNeeded= ClAtributeFuncTable::GetInstance()->GetNumArgs( (m4uint8_t)vNumArgToExe.Data.DoubleData );
		
		iNumArgs-=4; //descontamos los propios de esta función
		if (iNumArgsNeeded < 0 ) iNumArgsNeeded=-iNumArgsNeeded; //si variable, los ponemos en positivo

		//si variables->comprobamos que tengamos los suficientes
		if (iNumArgsNeeded<0  &&  iNumArgs<iNumArgsNeeded){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_ATT_PARAM_NUM)<<m_oDynamicPool->GetString(aParam[2].Data.CharData)
															  <<m_oDynamicPool->GetString(aParam[3].Data.CharData)
															  <<m_oDynamicPool->GetString(aParam[4].Data.CharData)
															  <<(m4uint8_t)vNumArgToExe.Data.DoubleData
															  <<">="<<LogCat<< (m4uint8_t)iNumArgsNeeded
															  <<iNumArgs
															  <<DumpCallStack()<<EndLog;
			RuntimePushDouble (M4_ERROR);
			return M4_ERROR;
		}//si fijos ->comprobamos que sean los mismos
		else if (iNumArgsNeeded>=0 && iNumArgs != iNumArgsNeeded){ //error ->nos faltan argumentos
			g_oChLog<<BeginError(M4_CH_VM_WRONG_ATT_PARAM_NUM)<<m_oDynamicPool->GetString(aParam[2].Data.CharData)
															  <<m_oDynamicPool->GetString(aParam[3].Data.CharData)
															  <<m_oDynamicPool->GetString(aParam[4].Data.CharData)
															  <<(m4uint8_t)vNumArgToExe.Data.DoubleData
															  <<(m4uint8_t)iNumArgsNeeded 
															  <<iNumArgs
															  <<DumpCallStack()<<EndLog;
			RuntimePushDouble (M4_ERROR);
			return M4_ERROR;
		}
		
		if ( ClAtributeFuncTable::GetInstance()->HasReferedArgs( (m4uint8_t)vNumArgToExe.Data.DoubleData )  ){ //error ->El attributo necesita argumentos por referencia-- No lo permitimos
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_REFERED_ARG_ALLOWED, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle())<<"ChannelAttribCall" << DumpCallStack() );
			RuntimePushDouble (M4_ERROR);
			return M4_ERROR;
		}
		
		//si el item a ejecutar es de arg. variable metemos el número en la RTS
		if (iNumArgsNeeded < 0 ){ 
			RuntimePushDouble (iNumArgs);
		}
		

		//Todo listo->Ponemos contexto y a ejecutar

		m_poVM->m_poAttribute->SetContext (poItem->GetpAccess(), 0,  poItem->ItemDef.NodeHandle(), poItem->ItemDef.GetHandle(), m_poVM->GetRunSliceMode(poItem->GetpAccess()), m_oState1.m_dBeginDate, m_oState1.m_dEndDate);
		
		//Ejecutamos el attributo (saca y mete en la pila lo que debe)
		m4return_t retVal=m_poVM->m_poAttribute->Run( (m4uint8_t)vNumArgToExe.Data.DoubleData );
		if (retVal==M4_ERROR){
            DUMP_CHLOG_ERRORF(M4_CH_VM_READING_ATT, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle())<<"ChannelAttribCall" << DumpCallStack() );
		}
		

	}
	
    for (i=2; i < 5 ; ++i){
		DeleteVarString(aParam[i]);
    }
	
    return retVal;
}





m4return_t ClVM1Indirections::SetPriority (void){

	m4VariantType aParam[4], vTemp, vNum;
    m4return_t  ret=M4_SUCCESS;
    m4bool_t    bSlice=M4_FALSE;
	m4bool_t	bNewSlices=M4_FALSE;
    m4uint32_t  i, iNumArgs;
    ClItem *poItem;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "SetPriority");
#endif
    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetPriority.")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
    
    //demasiados parametros 
    if ( iNumArgs> 5 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "SetPriority"<<5<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 5 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetPriority.")	;
        }
        iNumArgs=5; //ya hemos sacado los sobrantes
    }
	
    
    //con tramo
    if ( iNumArgs > 4 ){
        ret=RuntimePop(aParam[0]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetPriority.")	;
        bSlice=M4_TRUE;
    }
	
    
    //registro, item, nodo, 
    for (i=1; i < 4 ; ++i){
        ret=RuntimePop(aParam[i]);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetPriority.")	;
    }
    
    //prioridad
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetPriority.")	;
    if (vTemp.Type!=M4CL_CPP_TYPE_NUMBER){
        if (IsNull( vTemp )){
			ret=M4_ERROR;
		}
		else{
			ret = m_oVariantMediator->ToDouble(vTemp);
		}
        if (ret != M4_SUCCESS){
			DUMP_CHLOG_WARNINGF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "priority"<<"SetPriority" <<  M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );

            vTemp.Data.DoubleData = M4CL_CM_DEFAULT_PRIOR;
        }
    }
	
	//valor de la prioridad-chequeo de si es o no correcta-Ojo!!!min->número más alto, max->número más bajo
	if (vTemp.Data.DoubleData >M4CL_RULE_MIN_PRIORITY || vTemp.Data.DoubleData  < M4CL_RULE_MAX_PRIORITY){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "priority"<<"SetValuePriority"<<(priority_t)vTemp.Data.DoubleData << DumpCallStack() );
		vTemp.Data.DoubleData = M4CL_CM_DEFAULT_PRIOR;
	}

	
	m4return_t retVal=M4_SUCCESS;
    //obtenemos el puntero al item
    ret=  GetItemIndirect ( aParam, 4, bSlice, poItem, &bNewSlices);
    if (ret!=M4_SUCCESS){
		retVal=M4_ERROR;
	}
	else{
		if (poItem->ItemDef.Type()==M4CL_ITEM_TYPE_METHOD){
			DUMP_CHLOG_ERRORF(M4_CH_VM_TRY_PRIOR_METHOD, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle() )<<"SetValue" << DumpCallStack() );
			retVal=M4_ERROR;
		}
		else{
			
			//ponemos la prioridad
			/* Bug 0076620
			Si se han creado tramos se asigna entre fechas
			*/
			if (bNewSlices==M4_TRUE)
			{
				ret=m_poVM->SetPriority(*poItem, (priority_t)(vTemp.Data.DoubleData), m_oState1.m_dBeginDate, m_oState1.m_dEndDate);
			}
			else
			{
				ret=poItem->Value.SetPriority( (priority_t)(vTemp.Data.DoubleData) );
			}

			if (ret==M4_ERROR){
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_CHANGING_PRIORITY, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(),poItem->ItemDef.GetHandle() )<<"SetPriority" << DumpCallStack() );
			}
		}
	}
	
    //freedom
    if ( iNumArgs > 4 ){
        DeleteVarString(aParam[0]);
    }
    for (i=1; i < 4 ; ++i){
        DeleteVarString(aParam[i]);
    }
	
	
    RuntimePushDouble (ret);
    return retVal;
}

m4return_t ClVM1Indirections::ExecuteBranch (void){

    m4return_t      ret = M4_SUCCESS;
	m4VariantType   vNumArg, vItemName, vScope; // De momento solo me va a venir un argumento
                                                //que es el nombre del item a ejecutar
    m4pchar_t       pItemName;
    m4uint8_t       iScope;
    m4int32_t      i;

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1," ExecuteBranch.");
#endif


	ret=RuntimePopNumber(vNumArg);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ParametersNumber" << "ExecuteBranch" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumArg.Type)<<DumpCallStack(&m_oState1) );

	m4int32_t iNumArg = m4int32_t(vNumArg.Data.DoubleData);

    for (i=0; i< iNumArg - 2 ;++i){ //sacamos los que sobran
		RuntimePopAndDelete();
	}

	ret=RuntimePopString(vItemName);
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ItemName" << "ExecuteBranch"<< M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vItemName.Type)<<DumpCallStack(&m_oState1) );
	ret=RuntimePopNumber(vScope);
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Scope" << "ExecuteBranch"<< M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vScope.Type)<<DumpCallStack(&m_oState1) );

    pItemName = m_oDynamicPool->GetString(vItemName.Data.CharData);
    iScope = (m4uint8_t) vScope.Data.DoubleData;

    ClChanProc_Execution oClCPE (pItemName, m_oState1.m_poAccess);
    ClChanRoutHierarchicalDescendent oRoute;

    oRoute.AddProcess(&oClCPE);
    switch (iScope)
    {
    case M4CL_ITEM_SCOPE_NODE:
        ret = oRoute.Begin(m_oState1.m_poRecordSet->GetpNode_Data(), 0);
        break;
    case M4CL_ITEM_SCOPE_BLOCK:
        ret = oRoute.Begin(m_oState1.m_poRecordSet->GetCurrentBlock(), 0);
        break;
    case M4CL_ITEM_SCOPE_REGISTER:
        ret = oRoute.Begin(m_oState1.m_poRecordSet->GetCurrentBlock(), m_oState1.m_poRecordSet->Current.Index, 0);
        break;
    default:
        break;
    }

	DeleteVarString (vItemName);
    RuntimePushDouble (ret);
    return M4_SUCCESS;
}
