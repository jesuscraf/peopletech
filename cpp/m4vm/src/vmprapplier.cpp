
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
//    funciones de nómina
////
//==============================================================================

#include "vmprapplier.hpp"

#include "executor.hpp"
#include "vmres.hpp"
#include "vmdefine.hpp"
#include "vmtypes.hpp"

#include "m4log.hpp" 
#include "channel.hpp"
#include "access.hpp"
#include "cldefine.hpp"
#include "m4objglb.hpp"
#include "nodedef.hpp"




//---------------------------------------
//Comunes
//---------------------------------------

m4return_t ClVMPayRollApplier::_CopyFromValToEmpSlice (m4uint8_t ai_iSliceBehaviour, m4date_t ai_dStartToApply, m4date_t ai_dEndToApply, NodeInfoPR *ai_poNode, ClRegisterIndex ai_idxCR){

	m4uint32_t iNumItem= m_EmployeeInfo->RS->Register.Item.Count();
    m4uint32_t i;
    m4return_t ret=M4_SUCCESS;
    m4VariantType vTemp,vStart,vEnd,vTempType;;
    m4date_t dStart, dEnd, dPeriodStart, dPeriodEnd, dCutRate, dPeriodCutRate, dDivider, dPeriodDivider;
    m4uint16_t iM4Type, idxAuxItem;
    //m4uint32_t hAuxItem;
    m4pcchar_t pccValue;
    m4VariantType vChange;
	ClItemIndex  idxSrc;

    ClCompiledMCR *poCMCR = m_EmployeeInfo->RS->GetpNodeDef()->_GetpCMCR() ;

    //leemos las fechas de inicio y fin del valor
    ret=m_ValuesInfo->RS->Register.Item[m_ValuesInfo->idxStart].Value.Get (vStart); 
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Values.start_date#_CopyFromValToEmpSlice" << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    dStart=vStart.Data.DoubleData;
    
    ret=m_ValuesInfo->RS->Register.Item[m_ValuesInfo->idxEnd].Value.Get (vEnd); 
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Values.end_date#_CopyFromValToEmpSlice" << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    dEnd=vEnd.Data.DoubleData;

    // si las fechas del valor no cortan con las que tenemos que aplicar->pasamos
    // y borramos el registro que habiamos creado
    if ( (dEnd > ai_dEndToApply   && dStart > ai_dEndToApply  ) ||
        (dEnd < ai_dStartToApply && dStart < ai_dStartToApply) ) {
     
        m_EmployeeInfo->RS->Register.Destroy();
        return M4_SUCCESS; 

    }

	dPeriodCutRate = 1;

	// Si es de tipo PERIOD_FINAL_VALUE se pasa a FINAL_VALUE y se ajustan las fechas y el valor
	if( ai_iSliceBehaviour == M4CL_ITEM_SLICE_BHRV_PERIOD_FINAL_VALUE )
	{
		ai_iSliceBehaviour = M4CL_ITEM_SLICE_BHRV_FINAL_VALUE ;

        ret=m_PeriodsInfo->RS->Current.Item[m_PeriodsInfo->idxStart].Value.Get(vTemp);
	    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Values.period_start_date#_CopyFromValToEmpSlice" << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        dPeriodStart = vTemp.Data.DoubleData;
        
        ret=m_PeriodsInfo->RS->Current.Item[m_PeriodsInfo->idxEnd].Value.Get(vTemp);
	    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Values.period_end_date#_CopyFromValToEmpSlice" << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        dPeriodEnd = vTemp.Data.DoubleData;

        dPeriodDivider = dPeriodEnd - dPeriodStart + m_dTimeUnit ;

		if( dPeriodStart > dStart )
		{
			dStart = dPeriodStart ;
		}

		if( dPeriodEnd < dEnd )
		{
			dEnd = dPeriodEnd ;
		}

		dPeriodCutRate = ( dEnd - dStart + m_dTimeUnit ) / dPeriodDivider ;
	}

    for (i=0 ; i< iNumItem; ++i){
        
        if (i!=m_EmployeeInfo->idxStart && i!=m_EmployeeInfo->idxEnd &&   //los item de FI-FF no los copiamos
            m_EmployeeInfo->RS->Register.Item[ClItemIndex(i)].ItemDef.Type() != M4CL_ITEM_TYPE_METHOD &&
            m_EmployeeInfo->RS->Register.Item.ItemDef.Scope() == M4CL_ITEM_SCOPE_REGISTER){ //no posicionamos el item porque ya lo hemos hecho
            
            idxSrc=m_ValuesInfo->RS->Register.Item.TryMove ( m_EmployeeInfo->RS->Register.Item.ItemDef.Id(),M4_TRUE ); //no posicionamos el item porque ya lo hemos hecho
																														//EL M4_TRUE es porque viene en mayusculas
            if (idxSrc != M4DM_EOF_INDEX) {
                
                m_ValuesInfo->RS->Register.Item.MoveTo(idxSrc); //sabemos que existe.
				 
                //si podemos prorratear lo hacemos
                if( dStart <= ai_dEndToApply && dEnd >= ai_dStartToApply ){

                    //leemos..
                    ret=m_ValuesInfo->RS->Register.Item.Value.Get (vTemp); //no posicionamos el item porque ya lo hemos hecho
                    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Values.Item_to_copy#_CopyFromValToEmpSlice" << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    
                    iM4Type = m_ValuesInfo->RS->Register.Item.ItemDef.M4Type() ;

                    if( iM4Type == M4CL_M4_TYPE_VARIANT && vTemp.Type==M4CL_CPP_TYPE_STRING_VAR)  {
                        idxAuxItem=m_ValuesInfo->RS->Register.Item.ItemDef.AuxiliarItemIndex();

                        M4CHECK_RES (idxAuxItem!=-1, M4_CH_VM_UNDEFINED_ITEM, ERRORLOG, "Variant Aux Item"
																						<<m_ValuesInfo->RS->GetpNodeDef()->Id()<<LogCatAnd<<m_ValuesInfo->RS->GetpNodeDef()->Name()
																						<<"ApplyValue (1)"  << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

                        pccValue = vTemp.Data.PointerChar ;
                        
                        ret=m_ValuesInfo->RS->Register.Item[ClItemIndex(idxAuxItem)].Value.Get (vTempType);
                        if (ret!=M4_SUCCESS){
							DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(m_ValuesInfo->RS->GetpAccess()->GetpCMCR(), m_ValuesInfo->RS->Register.Item.ItemDef.GetHandle())<<"ApplyValue (1)" << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
							return M4_ERROR;
                        }
						
                        M4CHECK_RES (vTempType.Type != M4CL_CPP_TYPE_NULL ,M4_CH_VM_ITEM_IS_NULL_ITEM_AUX, ERRORLOG, M4ObjNodeItemI(m_ValuesInfo->RS->GetpAccess()->GetpCMCR(), m_ValuesInfo->RS->Register.Item.ItemDef.GetHandle())<<"(Variant Aux. Item)" <<"ApplyValue (1)" << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
																											
                        
                        if( vTempType.Type != M4CL_CPP_TYPE_NUMBER )
                        {
							DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_ITEM_AUX, M4ObjNodeItemI(m_ValuesInfo->RS->GetpAccess()->GetpCMCR(), m_ValuesInfo->RS->Register.Item.ItemDef.GetHandle())<<"Variant Aux. Item"<<"ApplyValue (1)" << M4ClCppType(M4CL_CPP_TYPE_NUMBER) << M4ClCppType(vTempType.Type) << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                            return( M4_ERROR ) ;
                        }
                        
                        iM4Type = m4uint8_t( vTempType.Data.DoubleData ) ;
                        
                        switch( iM4Type )
                        {
                        case M4CL_M4_TYPE_NUMBER :
                        case M4CL_M4_TYPE_CURRENCY :
                            
                            vTemp.Type = M4CL_CPP_TYPE_NUMBER ;
                            vTemp.Data.DoubleData = atof( pccValue ) ;
                            break ;
                            
                        default :
                            break ;
                        }
                    }

                    //según el tipo habrá que prorratear o no---
					m4int32_t iInternalType;
					iInternalType = m_EmployeeInfo->RS->Register.Item.ItemDef.InternalType();
                    if( (vTemp.Type == M4CL_CPP_TYPE_NUMBER) && (iInternalType!=M4CL_INTERNAL_TYPE_VARIANT_TYPE) && (iInternalType!=M4CL_INTERNAL_TYPE_PRIORITY) ){
                        switch( ai_iSliceBehaviour )
                        {
                        case M4CL_ITEM_SLICE_BHRV_FINAL_VALUE :
                        case M4CL_ITEM_SLICE_BHRV_UNIT :
                            
                            
                            dDivider = dEnd - dStart + m_dTimeUnit ;
                            
                            
                            if( dStart >= ai_dStartToApply ){
                                
                                if( dEnd <= ai_dEndToApply ){
                                    dCutRate = 1 ;
                                }
                                else{
                                    dCutRate = ( ai_dEndToApply - dStart + m_dTimeUnit) / dDivider ;
                                }
                            }
                            else{
                                
                                if( dEnd <= ai_dEndToApply ){
                                    dCutRate = ( dEnd - ai_dStartToApply + m_dTimeUnit) / dDivider ;
                                }
                                else{
                                    dCutRate = ( ai_dEndToApply - ai_dStartToApply + m_dTimeUnit ) / dDivider ;
                                }
                            }
                            
                            
                            vTemp.Data.DoubleData = vTemp.Data.DoubleData * dCutRate * dPeriodCutRate;
                            break ;
                            
                        default :
                            break ;
                            
                        }
                    }
                    
                    //establecemos el valor
                    ret=m_EmployeeInfo->RS->Register.Item.Value.Set (vTemp); //no posicionamos el item porque ya lo hemos hecho
                    M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo->RS->Register.Item.ItemDef.GetHandle() ) <<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                    if (ret==M4_WARNING){
						DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR(),m_EmployeeInfo->RS->Register.Item.ItemDef.GetHandle() )<<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                        ret=M4_SUCCESS;
                    }
                }
                
            }
        }
    }
    
    //ponemos los items de fecha_ini-fecha fin
    vTemp.Type=M4CL_CPP_TYPE_DATE;
    vTemp.Data.DoubleData=max (ai_dStartToApply, dStart);
    ret=m_EmployeeInfo->RS->Register.Item[m_EmployeeInfo->idxStart].Value.Set (vTemp); 
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo->hStart ) <<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    
    vTemp.Data.DoubleData=min (ai_dEndToApply, dEnd);
    ret=m_EmployeeInfo->RS->Register.Item[m_EmployeeInfo->idxEnd].Value.Set (vTemp); 
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo->hEnd ) <<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );


    //change_reason
    if ( m_EmployeeInfo->idxChangeReason != M4DM_EOF_INDEX ) {
        
        if (ai_poNode->RS!=m_ValuesInfo->RS && ai_poNode->idxChangeReason != M4DM_EOF_INDEX ){ //tenemos que copiar el change reason
            
            ret=ai_poNode->RS->Register[ai_idxCR].Item[ai_poNode->idxChangeReason].Value.Get (vChange);
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "SourceNode.ChangeReason#_CopyFromValToEmpSlice" << ai_poNode->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            
            //ponemso el change_reason que nos digan si no es nulo, o si aun siendo nulo el de valores
            //no debemos ponerlo, porque la fecha de validez del valor son anteriores a lo que tenemos que aplicar
            //y por tanto no hay cambio y debemos ponerlo a nulo (oel de historico si tiene)
            if (vChange.Type !=M4CL_CPP_TYPE_NULL || dStart < ai_dStartToApply ){ 
                ret=m_EmployeeInfo->RS->Register.Item[m_EmployeeInfo->idxChangeReason].Value.Set (vChange); 
                M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo->RS->Register.Item.ItemDef.GetHandle() ) <<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                if (ret==M4_WARNING){
					DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR(),m_EmployeeInfo->RS->Register.Item.ItemDef.GetHandle() )<<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                    ret=M4_SUCCESS;
                }
            }
        }
        else{ //si hemos metido el de values si las fechas de value es anterior a la que tenemos que aplicar ponemos el change reason a null
            if (dStart < ai_dStartToApply){ //a null
                vChange.Type=M4CL_CPP_TYPE_NULL;
                ret=m_EmployeeInfo->RS->Register.Item[m_EmployeeInfo->idxChangeReason].Value.Set (vChange); 
                M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo->RS->Register.Item.ItemDef.GetHandle() ) <<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                if (ret==M4_WARNING){
					DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR(),m_EmployeeInfo->RS->Register.Item.ItemDef.GetHandle() )<<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                    ret=M4_SUCCESS;
                }
                
            }
        }
        
    } //del IsNotEOF()
			//copiamso la tabla del que viene
	if (m_EmployeeInfo->idxSrcTable != M4DM_EOF_INDEX) {
		vTemp.Type=M4CL_CPP_TYPE_STRING_VAR;
		vTemp.Data.PointerChar=(m4char_t *)m_ValuesInfo->RS->GetpNodeDef()->Id(); //casting a cañon para que cuele-> En el Variant debería haber un pConstChar
		ret=m_EmployeeInfo->RS->Register.Item[m_EmployeeInfo->idxSrcTable].Value.Set (vTemp);
		DEBUG_CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo->RS->Register.Item.ItemDef.GetHandle() ) <<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
	}

    //si llega hasta aqui salimos con exito
	return M4_SUCCESS;

}


m4return_t ClVMPayRollApplier::_CopyFromValToEmp      (m4uint8_t ai_iSliceBehaviour,m4date_t ai_dStartToApply, m4date_t ai_dEndToApply){

    m4uint32_t iNumItem= m_EmployeeInfo->RS->Register.Item.Count();
    m4uint32_t i;
    m4return_t ret=M4_SUCCESS;
    m4VariantType vTemp;
	ClItemIndex idxSrc;

    for (i=0 ; i< iNumItem; ++i){

        if (i!=m_EmployeeInfo->idxStart && i!=m_EmployeeInfo->idxEnd &&   //los item de FI-FF no los copiamos
            m_EmployeeInfo->RS->Register.Item[ClItemIndex(i)].ItemDef.Type() != M4CL_ITEM_TYPE_METHOD &&
            m_EmployeeInfo->RS->Register.Item.ItemDef.Scope() ==M4CL_ITEM_SCOPE_REGISTER){ //no posicionamos el item porque ya lo hemos hecho
            
            idxSrc=m_ValuesInfo->RS->Register.Item.TryMove ( m_EmployeeInfo->RS->Register.Item.ItemDef.Id() , M4_TRUE); //no posicionamos el item porque ya lo hemos hecho
																														//El true es porque sabemos que viene en may.
			if (idxSrc != M4DM_EOF_INDEX) {
				m_ValuesInfo->RS->Register.Item.MoveTo(idxSrc); //sabemos que existe

                ret=m_ValuesInfo->RS->Register.Item.Value.Get (vTemp); //no posicionamos el item porque ya lo hemos hecho
                DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Values.Item_to_copy#_CopyFromValToEmp" << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );

                ret=m_EmployeeInfo->RS->Register.Item.Value.Set (vTemp); //no posicionamos el itm porque ya lo hemos hecho
                M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo->RS->Register.Item.ItemDef.GetHandle() ) <<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                if (ret==M4_WARNING){
					DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR(),m_EmployeeInfo->RS->Register.Item.ItemDef.GetHandle() )<<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                    ret=M4_SUCCESS;
                }
                
            }
        }

    }
    //ponemos los items de fecha_ini-fecha fin
    vTemp.Type=M4CL_CPP_TYPE_DATE;
    vTemp.Data.DoubleData=ai_dStartToApply;
    ret=m_EmployeeInfo->RS->Register.Item[m_EmployeeInfo->idxStart].Value.Set (vTemp); 
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo->hStart) <<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    
    vTemp.Data.DoubleData=ai_dEndToApply;
    ret=m_EmployeeInfo->RS->Register.Item[m_EmployeeInfo->idxEnd].Value.Set (vTemp); 
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo->hEnd) <<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    

    //change reason->Sin tramos es siempre NULL
    if ( m_EmployeeInfo->idxChangeReason != M4DM_EOF_INDEX ) {
        vTemp.Type=M4CL_CPP_TYPE_NULL;
        m_EmployeeInfo->RS->Register.Item[m_EmployeeInfo->idxChangeReason].Value.Set (vTemp);
        DEBUG_CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "EmployeeNode.ChangeReason#_CopyFromValToEmpSlice" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    }
		//copiamso la tabla del que viene
	if (m_EmployeeInfo->idxSrcTable != M4DM_EOF_INDEX) {
		vTemp.Type=M4CL_CPP_TYPE_STRING_VAR;
		vTemp.Data.PointerChar=(m4char_t *)m_ValuesInfo->RS->GetpNodeDef()->Id();
		ret=m_EmployeeInfo->RS->Register.Item[m_EmployeeInfo->idxSrcTable].Value.Set (vTemp);
		DEBUG_CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, M4ObjNodeItemI(m_EmployeeInfo->RS->GetpAccess()->GetpCMCR() , m_EmployeeInfo->RS->Register.Item.ItemDef.GetHandle() ) <<"ApplyValue (1)" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
	
	}

    //si llega hasta aqui salimos con exito
	return M4_SUCCESS;

}


m4bool_t   ClVMPayRollApplier::DmdMustApply (m4VariantType &ai_DmdComponent, m4uint8_t &ao_iSliceBehaviour){

    m4VariantType   vTemp;
    m4bool_t        bret;
    m4return_t      ret;
    m4int32_t		iRegFind;
	m4uint32_t		iIndexHandle, iIndexId;
	
	ClCompiledMCR   *poCMCR=m_DmdComponentsInfo->RS->GetpAccess()->GetpCMCR();
	
	iIndexHandle = poCMCR->GetNodeDefaultIndex(  m_DmdComponentsInfo->RS->GetpNodeDef()->GetHandle() );
		
	if (iIndexHandle == 0){ //No hay indice definido->busqueda secuencial
		m4VariantType   avParam[3];

		avParam[0].Data.DoubleData=m_DmdComponentsInfo->idxDmdComponent;
		avParam[0].Type=avParam[2].Type=M4CL_CPP_TYPE_NUMBER;
		
		avParam[1]=ai_DmdComponent;
		avParam[2].Data.DoubleData=M4CL_EQUAL_FUNCTION;
		
		ret=m_DmdComponentsInfo->RS->Register.FindSec(avParam, 3 , &iRegFind);
	}
	else{ //->Busqueda binaria
		m4uint32_t iNumIndexItems=poCMCR->GetIndexNumberOfItems (iIndexHandle) ;

		if ( iNumIndexItems != 1) {
			 //mensaje de wrong index
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_DEF_INDEX_ITEM_NUMBER, M4ObjNodeN(poCMCR, m_DmdComponentsInfo->RS->GetpNodeDef()->GetHandle() )<<1<<iNumIndexItems<< m_DmdComponentsInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
			//retornar error
			ao_iSliceBehaviour=0;
			return M4_FALSE;
		 
		}

		iIndexId=poCMCR->GetIndexId (iIndexHandle);
		ret = m_DmdComponentsInfo->RS->SetCurrentSearchIndexById (iIndexId);
		if (ret==M4_SUCCESS){
			//busqueda binaria
			m_DmdComponentsInfo->RS->Register.Begin();
			m_DmdComponentsInfo->RS->Register.Find (&ai_DmdComponent);
			
			iRegFind=m_DmdComponentsInfo->RS->Register.Index;
		}
	}

    if (ret!=M4_SUCCESS){
        DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_IN_FIND, M4ObjNodeN(poCMCR, m_DmdComponentsInfo->RS->GetpNodeDef()->GetHandle() )<<"ApplyValue (1)" << m_DmdComponentsInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        ao_iSliceBehaviour=0;
        return M4_FALSE;
    }
    if (iRegFind==-1){

        ao_iSliceBehaviour=0;
        bret=M4_FALSE;
    }
    else{
        ret=m_DmdComponentsInfo->RS->Register[iRegFind].Item[m_DmdComponentsInfo->idxSliceBehaviour].Value.Get (vTemp);
        if (ret!=M4_SUCCESS){
	        DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_IN_FIND, M4ObjNodeN(poCMCR, m_DmdComponentsInfo->RS->GetpNodeDef()->GetHandle() )<<"ApplyValue (1)" << m_DmdComponentsInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
            ao_iSliceBehaviour=0;
            return M4_FALSE;
        }
        
        ao_iSliceBehaviour=(m4uint8_t)vTemp.Data.DoubleData;
        bret=M4_TRUE;
    }
    
    return bret;
}


//---------------------------------------------------------
//Applier Normal
//---------------------------------------------------------

m4return_t ClVMPayRollApplierNormal::_AddRegisterToEmployeeWithSlices (m4int32_t iRegValueFind,  m4date_t ai_dStart, m4date_t ai_dEnd, NodeInfoPR *ai_poNode, ClRegisterIndex ai_idxCR ){

    //añadimos el registro iRegValue a empleados si no está ya o 
    //no hay otro al que machaque. Si hace falta dividirlo en 2 -> se hace

    //  |------------------|  Existente en empelado  -->NO AÑADIR
    //     |-----|            Value

    
    //     |-----|            Existente en empleado  
    //  |0-1-----2----------3  Value -->Dividir en 2: de 0-1  y de 2-3

    //ai_poCRRS contiene el RS del que tenemos que aplicar el change reason
    //ai_CRindex tiene el regsitro de ai_poCRRS que tenemos que aplicar o -1 si no hay que aplicar

    
    m4bool_t bAdd=M4_TRUE;
    m4VariantType vValuesDmdComponent,vStart, vEnd, vDmdComponent;
    m4return_t ret=M4_SUCCESS;
    m4int32_t j;
    m4date_t  dEndAdd, dStartAdd;
    m4int32_t iEqual;
    m4bool_t    bEOF;
    m4uint8_t   iSliceBehaviour;
    
    m4date_t dRemainStart, dRemainEnd;
    
    ClItemIndex idxOld=m_EmployeeInfo->RS->Register.Item.GetIndex();
    
    
    
    
    dRemainStart=ai_dStart;
    dRemainEnd=ai_dEnd;
    
    ret=m_ValuesInfo->RS->Register[iRegValueFind].Item[m_ValuesInfo->idxDmdComponent].Value.Get (vValuesDmdComponent);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Values.dmd_component#_AddRegisterToEmployeeSlices" << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    
    //Para comprobar si debemos o no aplicar
    if (!DmdMustApply (vValuesDmdComponent, iSliceBehaviour)  ) return M4_SUCCESS;
    
    //nos movemos al  primer registro que coincida dmd_component
    j=0;
    iEqual=0;
    //nos movemos al registro 0 y miramos si es EOF
    bEOF= (!m_EmployeeInfo->RS->Register.MoveTo(ClRegisterIndex(j))) ? M4_TRUE : M4_FALSE;
    while (iEqual==0 && bEOF== M4_FALSE){
        
        ret=m_EmployeeInfo->RS->Register.Item[m_EmployeeInfo->idxDmdComponent].Value.Get (vDmdComponent);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "EmployeeNode.id_dmd_component#_AddRegisterToEmployeeSlices" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        
        //comparamso unidades
        iEqual= (vDmdComponent==vValuesDmdComponent);
        
        if (iEqual==0){
            ++j;
            bEOF= (!m_EmployeeInfo->RS->Register.MoveTo(ClRegisterIndex(j)) ) ? M4_TRUE : M4_FALSE;
        }
        
    }
    
    bAdd=M4_TRUE;
    bEOF= (!m_EmployeeInfo->RS->Register.MoveTo(ClRegisterIndex(j))) ? M4_TRUE : M4_FALSE ;
	
	m4bool_t bBreak=M4_FALSE;
    
	while (dRemainEnd-dRemainStart >=0 && bEOF==M4_FALSE && bBreak==M4_FALSE){
        
        ret=m_EmployeeInfo->RS->Register.Item[m_EmployeeInfo->idxStart].Value.Get (vStart);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "EmployeeNode.StartDate#_AddRegisterToEmployeeSlices" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        
        ret=m_EmployeeInfo->RS->Register.Item[m_EmployeeInfo->idxEnd].Value.Get (vEnd);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "EmployeeNode.EndDate#_AddRegisterToEmployeeSlices" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        
        
        if (dRemainStart >= vStart.Data.DoubleData && dRemainEnd <=vEnd.Data.DoubleData){
            //el regsitro cubre todo lo lo que queda por añadir
            dRemainStart=dRemainEnd=0;
            bAdd=M4_FALSE;
			bBreak=M4_TRUE;
        }
        else{
            
            if (dRemainStart < vStart.Data.DoubleData ){
                dEndAdd=vStart.Data.DoubleData-m_dTimeUnit;
                dStartAdd=dRemainStart;
                
                dRemainStart=vStart.Data.DoubleData;
                
                //insertamos el registro con fechas dStartAdd, dEndAdd
                
                m_EmployeeInfo->RS->Register.Insert(); //añade y posiciona el it. reg. en el nuevo
                //copiamos , poniendo las fechas correctas
                ret=_CopyFromValToEmpSlice (iSliceBehaviour, dStartAdd, dEndAdd, ai_poNode, ai_idxCR);
//                M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_COPING_REGISTER, ERRORLOG, "_AddRegisterToEmployeeSlices. Error copying register from values to employee." << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
				if (ret!= M4_SUCCESS){
					return M4_ERROR;
				}
            }
            
            if (dRemainStart >= vStart.Data.DoubleData ){
                dRemainStart=max(vEnd.Data.DoubleData+m_dTimeUnit,dRemainStart);
            }
            
            
            bEOF= (!m_EmployeeInfo->RS->Register.MoveTo(ClRegisterIndex(++j)) ) ? M4_TRUE : M4_FALSE;
            
            if (!bEOF){
                //miramos si seguimos en nuestro dmd_comp
                ret=m_EmployeeInfo->RS->Register.Item[m_EmployeeInfo->idxDmdComponent].Value.Get (vDmdComponent);
                DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "EmployeeNode.IdDmdComponent#_AddRegisterToEmployeeSlices" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
                
                //comparamso unidades
                //bEOF |= (vDmdComponent!=vValuesDmdComponent);
				bEOF = (vDmdComponent!=vValuesDmdComponent) ? M4_TRUE : M4_FALSE ;
            }
        }
    }

    if (dRemainEnd==0 && dRemainStart==0){
		bAdd=M4_FALSE;
	}
    if (dRemainEnd-dRemainStart <0 ){ //si ya no queda nada, ni añadimos
		bAdd=M4_FALSE;
	}

    if (bAdd){
        //añadimso registro con fechas dRemainStart, dRemainEnd
        if (m_EmployeeInfo->RS->Register.Index == M4DM_EOF_INDEX)
            m_EmployeeInfo->RS->Register.Add(); 
        else
            m_EmployeeInfo->RS->Register.Insert(); //añade y posiciona el it. reg. en el nuevo
        
        //copiamos , poniendo las fechas correctas
        ret=_CopyFromValToEmpSlice (iSliceBehaviour,dRemainStart,dRemainEnd, ai_poNode, ai_idxCR);
		if (ret!= M4_SUCCESS){
			return M4_ERROR;
		}
		//        M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_COPING_REGISTER, ERRORLOG, "_AddRegisterToEmployeeSlices. Error copying register from values to employee." << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
        
    }

        
    m_EmployeeInfo->RS->Register.Item[idxOld];
    
	//Si llega aqui salimos con éxito
    //return ret;
	return M4_SUCCESS;

}


m4return_t ClVMPayRollApplierNormal::_AddRegisterToEmployeeNoSlices   (m4int32_t iRegValueFind, m4date_t ai_dStart, m4date_t ai_dEnd ){

	//añadimos el registro iRegValue a empleados si no está ya 
    m4VariantType vValuesDmdComponent,vDmdComponent;
    m4return_t ret=M4_SUCCESS;
    m4uint8_t   iSliceBehaviour;
    m4int32_t i;
    
    ClItemIndex idxOld=m_EmployeeInfo->RS->Register.Item.GetIndex();
	
    
    
    
    ret=m_ValuesInfo->RS->Register[iRegValueFind].Item[m_ValuesInfo->idxDmdComponent].Value.Get (vValuesDmdComponent);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Values.IdDmdComponent#_AddRegisterToEmployee" << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    
    //Para comprobar si debemos o no aplicar----
    if (!DmdMustApply (vValuesDmdComponent,iSliceBehaviour)  ) return M4_WARNING;
	
	m4int32_t iCount=m_EmployeeInfo->RS->Register.Count();
	for (i=0; i< iCount; ++i){
		
		ret=m_EmployeeInfo->RS->Register[i].Item[m_EmployeeInfo->idxDmdComponent].Value.Get (vDmdComponent);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Employee.IdDmdComponent#_AddRegisterToEmployee" << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
		
		//comparamso unidades
		if(vDmdComponent==vValuesDmdComponent){
			return M4_SUCCESS; //ya está metido , no lo metemos
		}
	}
	
	
    m_EmployeeInfo->RS->Register.Add();    //añade y posiciona el it. reg. en el nuevo
    
    //copiamos , poniendo las fechas correctas
    ret=_CopyFromValToEmp (iSliceBehaviour,ai_dStart, ai_dEnd);
	if (ret!= M4_SUCCESS){
		return M4_ERROR;
	}
	//    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_COPING_REGISTER, ERRORLOG, "_AddRegisterToEmployee. Error copying register from values to employee." << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
	
	m_EmployeeInfo->RS->Register.Item[idxOld];
	
	//si llega aqui, salimos con éxito
    //return ret;
	return M4_SUCCESS;


}




//---------------------------------------------------------
//Applier ApplyAlways
//---------------------------------------------------------

m4return_t ClVMPayRollApplierApplyAlways::_AddRegisterToEmployeeWithSlices (m4int32_t iRegValueFind,  m4date_t ai_dStart, m4date_t ai_dEnd, NodeInfoPR *ai_poNode, ClRegisterIndex ai_idxCR ){
    //añadimos el registro iRegValue a empleados siempre
    
    m4bool_t bAdd=M4_TRUE;
    m4VariantType vValuesDmdComponent,vDmdComponent;
    m4return_t ret=M4_SUCCESS;
    m4uint8_t   iSliceBehaviour;
    m4date_t dRemainStart, dRemainEnd;
    
    ClItemIndex idxOld=m_EmployeeInfo->RS->Register.Item.GetIndex();
    
    dRemainStart=ai_dStart;
    dRemainEnd=ai_dEnd;
    
    ret=m_ValuesInfo->RS->Register[iRegValueFind].Item[m_ValuesInfo->idxDmdComponent].Value.Get (vValuesDmdComponent);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Values.IdDmdComponent#_AddRegisterToEmployeeSlices" << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    
    //Para comprobar si debemos o no aplicar
    if (!DmdMustApply (vValuesDmdComponent, iSliceBehaviour)  ) return M4_SUCCESS;
	
	
	m_EmployeeInfo->RS->Register.Add(); 
	//copiamos , poniendo las fechas correctas
	ret=_CopyFromValToEmpSlice (iSliceBehaviour,dRemainStart,dRemainEnd, ai_poNode, ai_idxCR);
	if (ret!= M4_SUCCESS){
		return M4_ERROR;
	}
	//	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_COPING_REGISTER, ERRORLOG, "_AddRegisterToEmployeeSlices. Error copying register from values to employee." << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
				
				
	m_EmployeeInfo->RS->Register.Item[idxOld];
    //si llega aqui, salimos con éxito
    //return ret;
	return M4_SUCCESS;
}

m4return_t ClVMPayRollApplierApplyAlways::_AddRegisterToEmployeeNoSlices   (m4int32_t iRegValueFind, m4date_t ai_dStart, m4date_t ai_dEnd ){

		//añadimos el registro iRegValue a empleados SIEMPRE
    m4VariantType vValuesDmdComponent,vDmdComponent;
    m4return_t ret=M4_SUCCESS;
    m4uint8_t   iSliceBehaviour;
    
    ClItemIndex idxOld=m_EmployeeInfo->RS->Register.Item.GetIndex();
	
    ret=m_ValuesInfo->RS->Register[iRegValueFind].Item[m_ValuesInfo->idxDmdComponent].Value.Get (vValuesDmdComponent);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Values.IdDmdComponent#_AddRegisterToEmployee" << m_ValuesInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    
    //Para comprobar si debemos o no aplicar----
    if (!DmdMustApply (vValuesDmdComponent,iSliceBehaviour)  ) return M4_WARNING;
	
    m_EmployeeInfo->RS->Register.Add();    //añade y posiciona el it. reg. en el nuevo
    
    //copiamos , poniendo las fechas correctas
    ret=_CopyFromValToEmp (iSliceBehaviour,ai_dStart, ai_dEnd);
	if (ret!= M4_SUCCESS){
		return M4_ERROR;
	}
	//    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_COPING_REGISTER, ERRORLOG, "_AddRegisterToEmployee. Error copying register from values to employee." << m_EmployeeInfo->RS->GetpAccess()->GetpExecutor()->DumpCallStack() );
    
    m_EmployeeInfo->RS->Register.Item[idxOld];
	
	//Si llega aqui salimos con éxito
    //return ret;
	return M4_SUCCESS;

}
