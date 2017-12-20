

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                vMachin2.cpp
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
//    Virtual machine de nivel 2
////
//==============================================================================

#include "vmachin2.h"
#include "vmattrib.hpp"
#include "tablfunc.h"
#include "vmres.hpp"
#include "chan_dir.hpp"
#include "m4mdrt.hpp"
#include "chlog.hpp"
#include "cldbgutil.hpp"
#include "m4objglb.hpp"
#include "vmauto_ptr_array.hpp"
#include "cldmcontext.hpp"
#include "nodedef.hpp"
#include "m4lang.hpp"



ClVMachine2::ClVMachine2(tpFixedStack<m4VariantType> *ai_oRunTimeStack,
						ClStringPool *ai_oDynamicPool,
						tpFixedStack<m4VMState_t>	*ai_oStateStack,
						ClVMCSContext *ai_poVMCSContext ):ClVMachine1 (ai_oRunTimeStack,ai_oDynamicPool, ai_oStateStack, ai_poVMCSContext){

	
	//iniciamos los objetos implementadores
	m_oImplementors[M4CL_VM_OBJECT_THIS]=this;

	#ifdef _M4_TRACE_POOL
		m_oDynamicPool->SetVM(this);
	#endif

}

ClVMachine2::~ClVMachine2(){

}

//EDU:
m4uint32_t ClVMachine2::GetSize (void) const
{
    m4uint32_t length = 0;

    length += sizeof (ClVMachine2);
    length += ClVMachine1::GetSize ();
    length -= sizeof (ClVMachine1);

    return length;
}



//**************************
//*Funciones básicas
//**************************



m4return_t ClVMachine2::ExecuteItem(void){

	//ejecuta el metodo que indican los campos de la instrucción
	// y pone en la pila el resultado
	
	m4int32_t			i;
	m4return_t			ret=M4_SUCCESS;
	m4VariantType		vTempVariant, vSlice(0.0);
	m4VMState_t			oState;
	m4uint16_t			iNumArg , iNumRealArg, iVarArgs;
	m4pcvoid_t			hFormula;
	m4uint16_t			iInstruFlags;;
	m4uint16_t			iNodeIndex;
	ClNodeIndex			oNodeIndex;
	m4uint32_t			hNodo,hItem;
	m4bool_t            bSpecSlice;
	m4uint16_t			iNumRegla;
	ClAccess * poAccess;
	
	ret = GetContext ( &poAccess, hNodo, hItem );
	if (ret==M4_ERROR){
		return M4_ERROR;
	}
    
    ClCompiledMCR   *poCMCR = poAccess-> GetpCMCR() ;

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L2, "Trace VM2: Execute Item" <<LogCat<< poCMCR->iNodeI(hItem) <<LogCat<< "." <<LogCat<<poCMCR->iItemI(hItem) );
#endif

	hFormula =	NULL;
	iNumArg	 =  0;
	iNumRealArg = 0;
   
    iNodeIndex	=poCMCR->GetNodeIndex (hNodo);
    
    //metemos el access, el bloque actual y el registro actual
    oState.m_poAccess = poAccess;
    oNodeIndex=iNodeIndex;

    oState.m_poRecordSet=&(poAccess->Node[oNodeIndex].RecordSet);   //directamente del access
    oState.m_Flags=0;
    
    iInstruFlags=m_oCurrentIns2->GetFlags();

	//!!!! De momento el numero de regla lo cableamos hasta que lo podamos recuperar del metacanal por Nombre
	iNumRegla = 0; //oCurrentIns.GetNumRegla()
    
    iNumRealArg=poCMCR->GetItemNumberOfArguments ( hItem);
    iVarArgs = poCMCR->GetItemVariableArguments ( hItem);
    if ( iVarArgs != 0 ) {
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

    if (M4CL_HAS_TRAME_FLAG(iInstruFlags)  || M4CL_HAS_ROW_FLAG( iInstruFlags )	){ //hay que salvar los argumentos
        
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
    if ( M4CL_HAS_ROW_FLAG ( iInstruFlags ) ) {
        ret=RuntimePop(vTempVariant); //leemos el reg.
        if (vTempVariant.Type != M4CL_CPP_TYPE_NUMBER){
            ret=m_oVariantMediator->ToDouble (vTempVariant);
            M4CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_WRONG_RECORD_NUMBER_FORMAT, ERRORLOG, M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem)<<DumpTreePath(poAccess, hNodo) << DumpCallStack() );
        }
        oState.m_oRegisterIndex=int (vTempVariant.Data.DoubleData);
    }
    else{
        //el registro sobre el que actuar será current o NULL
        if (oState.m_poRecordSet->IsValid ()){
            ClAccessRecordSet *pars = oState.m_poRecordSet;
            oState.m_oRegisterIndex = pars->Current.Index;
        }
        else{
            oState.m_oRegisterIndex = M4DM_EOF_INDEX;
        }
    }

    
    //si habiamos sacado los registros los metemos de nuevo
    //dejamos los argumentos en la pila
    //si habiamos sacado los registros los metemos de nuevo
    //dejamos los argumentos en la pila
    if (bRestoreArgs ==M4_TRUE){
        for (i=0; i < iNumArg ; i++){
            ret=m_oResultStack->Top(vTempVariant);
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG,  __FILE__<<__LINE__<<"Error adding arguments to RTS reading from tempStack, in ExecuteItem-2.")	;
            m_oResultStack->Pop();					
            //metemos en la pila de ejecución lo que habiamos sacado
            ret=RuntimePush(vTempVariant);
        }    
    }
      


	oState.m_dBeginDate=m_oState2.m_dBeginDate;
    oState.m_dEndDate=m_oState2.m_dEndDate;
    oState.m_dDate=m_oState2.m_dDate;
    oState.m_hItem=hItem;
    oState.m_iRule=iNumRegla;

    if ( iVarArgs != 0 ) --iNumArg;

	//Nuevo. Estadisticas.
	oState.m_iParentStatePos=m_oStateStack->GetPos();

	ret= CreateExecute (oState, iNumArg, vSlice, bSpecSlice)	;


	return ret;

}


m4return_t ClVMachine2::AssignItem(void){

	//asigna al item que indican los campos de la instrucción
	//el tope de la pila
	//y elimina el tope de la pila
	
   	m4uint32_t	iRow;
	m4double_t	dTrame;
	m4uint32_t	hNodo, hItem;	
	m4uint8_t   iSliceFlag;
	ClSliceIndex	SliceIndex;
	m4VariantType vTemp, vValue;    
	ClAccess * poAccess;
	ClItem     *poItem;
	m4return_t	ret;

	const m4uint8_t NoSliceWithSlices=0;
    const m4uint8_t NoSliceWithoutSlices=1;
    const m4uint8_t SliceByDate=2;
    const m4uint8_t SliceByOrder=3;
	
	ret = GetContext ( &poAccess, hNodo, hItem );
	
	if (ret==M4_ERROR){
		return M4_ERROR;
	}
//	M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_GETTING_CONTEXT, ERRORLOG, "Error getting context in Assign Item.");

	ClCompiledMCR   *poCMCR=poAccess-> GetpCMCR() ;

#if _M4_DEBUG > 10  
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L2, "Trace VM2: Assign Item"<<LogCat<< poCMCR->iNodeI(hItem) <<LogCat<< "." <<LogCat<<poCMCR->iItemI(hItem) );
#endif
	
	
	ret=RuntimePop(vValue);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "AssignItem")	;


    //obtenemos el valor al que asignar y asignamos

    if (GetRunSliceMode(poAccess)){
        iSliceFlag=NoSliceWithSlices;
    }
    else{
        iSliceFlag=NoSliceWithoutSlices;
    }

	//miramos si viene con tramo
	if ( M4CL_HAS_TRAME_FLAG (m_oCurrentIns2->GetFlags() ) )
	{
		ret=RuntimePop(vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "AssignItem.")	;
       
		if (vTemp.Type==M4CL_CPP_TYPE_NUMBER){ //por número de tramo
			SliceIndex=m4uint32_t(vTemp.Data.DoubleData);
            iSliceFlag=SliceByOrder;
		}
		else if (vTemp.Type==M4CL_CPP_TYPE_DATE){ //por fecha
			dTrame=vTemp.Data.DoubleData;
			iSliceFlag=SliceByDate;
		}
		else{
            DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE_FORMAT_WRITE, M4ObjNodeItemI(poCMCR ,hItem )<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR() ,m_oState2.m_hItem ) << DumpCallStack() );
			return M4_ERROR;
		}
        
        //como ha modificado "a mano" los tramos, no nos asignaremos
        //M4CL_SET_STATE_PURGE_FLAG (m_oState1.m_Flags);
		//????
        M4CL_RESET_STATE_CONCEPT_FLAG (m_oState2.m_Flags);
	}
	
    //miramos si viene con row
    _ClRegister_Base *pr;
	
    if ( M4CL_HAS_ROW_FLAG (m_oCurrentIns2->GetFlags() ) ){
		ret=RuntimePop (vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP , ERRORLOG, "AssignItem, to get record number.")	;

        if (vTemp.Type != M4CL_CPP_TYPE_NUMBER){    // si no es un numero lo intentamos converit
            ret=m_oVariantMediator->ToDouble (vTemp);
            M4CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_WRONG_RECORD_NUMBER_FORMAT, ERRORLOG, M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem)<<DumpTreePath(poAccess, hNodo) << DumpCallStack() );
        }
        
		iRow=int (vTemp.Data.DoubleData);

        if (!poAccess->Node[ClHandle(hNodo)].RecordSet.IsValid() && poCMCR->GetItemScope(hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_ASSIGNING)<<M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem)<<DumpTreePath(poAccess, hNodo)<<DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//el nodo ya esta posicionado
        pr=&(poAccess->Node->RecordSet.Register[iRow]);
	}
    else{
        if (!poAccess->Node[ClHandle(hNodo)].RecordSet.IsValid() && poCMCR->GetItemScope(hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_ASSIGNING)<<M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem)<<DumpTreePath(poAccess, hNodo)<<DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//el nodo ya esta posicionado
        pr=&(poAccess->Node->RecordSet.Current);
    }


    //comprobamso que podamos asignar
    if (pr->Index == M4DM_EOF_INDEX && poCMCR ->GetItemScope(hItem) == M4CL_ITEM_SCOPE_REGISTER ){		
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_NUMBER_WRITE,M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem)<<DumpTreePath(poAccess, poAccess->Node->GetpNodeDef()->GetHandle())  << DumpCallStack() );
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
				ClDateToString( m_oState2.m_dDate, acDate, M4_SECOND_END + 1 ) ;
				DUMP_CHLOG_WARNINGF( M4_CH_VM_BASE_SLICED_ITEM_USE, M4ObjNodeItemI( poCMCR, hItem ) << iSlices << poItem->Value.Index << m_oState2.m_iSourceLine << m_oState2.m_poAccess->GetpCMCR()->GetItemFormIdByPosition( m_oState2.m_hItem, m_oState2.m_iRule ) << M4ObjNodeItemI( m_oState2.m_poAccess->GetpCMCR(), m_oState2.m_hItem ) << acDate << DumpCallStack() ) ;
			}

			ret=__SetValue(*poItem, vValue, M4CL_CM_DEFAULT_PRIOR, 0) ;
            break;
        }
    case NoSliceWithSlices:
        {
			poItem=&(pr->Item[iItem]);

            ret=CheckAndCreateItemSlice (m_oState2.m_dBeginDate, m_oState2.m_dEndDate ,poItem);
            if (ret==M4_SUCCESS){
				/* Bug 0076620
				Se asignan los subtramos si los hubiera
				*/
                ret=SetValue(*poItem, vValue, M4CL_CM_DEFAULT_PRIOR, 0, m_oState2.m_dBeginDate, m_oState2.m_dEndDate, M4_TRUE);    //no movemos el item porque ya esta puesto    
            }
			
            M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_SET_CONCEPT_SLICE_VALUE, ERRORLOG, M4ObjNodeItemI(poCMCR,hItem)<<DateLog(m_oState2.m_dBeginDate)<<DateLog(m_oState2.m_dEndDate)<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem)  << DumpCallStack() );
			break;
        }
        
    case SliceByDate:
        {
			pr->Item[iItem].Value[dTrame];
			ret=__SetValue(pr->Item, vValue, M4CL_CM_DEFAULT_PRIOR, 0);
            break;
        }
    case SliceByOrder:
        {
			pr->Item[iItem].Value[SliceIndex];
			ret=__SetValue(pr->Item, vValue, M4CL_CM_DEFAULT_PRIOR, 0);            
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
		DUMP_CHLOG_WARNINGF(M4_CH_VM_ASSIGN_TRUNCATED_EXE_ITEM, M4ObjNodeItemI(pr->GetpAccess()->GetpCMCR(),pr->Item.ItemDef.GetHandle())<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem) << DumpCallStack() );
        ret=M4_SUCCESS;
    }
	
	return M4_SUCCESS;
}


m4return_t ClVMachine2::PushItem(void){

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
	ClAccess * poAccess;

	m4return_t	ret;
	
	const m4uint8_t NoSliceWithSlices=0;
    const m4uint8_t NoSliceWithoutSlices=1;
    const m4uint8_t SliceByDate=2;
    const m4uint8_t SliceByOrder=3;

    m4char_t	acStartDate[ M4_SECOND_END + 1 ] ;
    m4char_t	acEndDate  [ M4_SECOND_END + 1 ] ;

	
	ret = GetContext ( &poAccess, hNodo, hItem );
	if (ret==M4_ERROR){
		return M4_ERROR;
	}
//	M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_GETTING_CONTEXT, ERRORLOG, "Error getting context in Push Item.");
	ClCompiledMCR   *poCMCR=poAccess-> GetpCMCR() ;

#if _M4_DEBUG > 10  
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L2, "Trace VM2: Push Item"<<LogCat<< poCMCR->iNodeI(hItem) <<LogCat<< "." <<LogCat<<poCMCR->iItemI(hItem) );
#endif


    if (GetRunSliceMode(poAccess)){
        iSliceFlag=NoSliceWithSlices;
    }
    else{
        iSliceFlag=NoSliceWithoutSlices;
    }
	
	//miramos si viene con tramo
	if ( M4CL_HAS_TRAME_FLAG (m_oCurrentIns2->GetFlags() ) ){
		
		ret=RuntimePop(vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "PushItem.")	;
        
		if (vTemp.Type==M4CL_CPP_TYPE_NUMBER){ //por número de tramo
			SliceIndex=m4uint32_t(vTemp.Data.DoubleData);
            iSliceFlag=SliceByOrder;
		}
		else if (vTemp.Type==M4CL_CPP_TYPE_DATE){ //por fecha
			dTrame=vTemp.Data.DoubleData;
			iSliceFlag=SliceByDate;
		}
		else{
            DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE_FORMAT_READ, M4ObjNodeItemI(poCMCR ,hItem )<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR() ,m_oState2.m_hItem ) << DumpCallStack() );
			return M4_ERROR;
		}
	}
	
    //miramos si viene con row
    _ClRegister_Base *pr;
	
    if ( M4CL_HAS_ROW_FLAG (m_oCurrentIns2->GetFlags() ) ){
		ret=RuntimePop (vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP , ERRORLOG, "PushItem, to get record number.")	;

        if (vTemp.Type != M4CL_CPP_TYPE_NUMBER){    // si no es un numero lo intentamos converit
            ret=m_oVariantMediator->ToDouble (vTemp);
            M4CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_WRONG_RECORD_NUMBER_FORMAT, ERRORLOG, M4ObjNodeI(poCMCR,hItem)<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem)<<DumpTreePath(poAccess, hNodo) << DumpCallStack() );
        }
		
		iRow=int (vTemp.Data.DoubleData);

        if (!poAccess->Node[ClHandle(hNodo)].RecordSet.IsValid()&& poCMCR->GetItemScope(hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_READING)  <<M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem)<<DumpTreePath(poAccess, hNodo)<<DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//el nodo ya esta posicionado
        pr=&(poAccess->Node->RecordSet.Register[iRow]);
	}
    else{

        if (!poAccess->Node[ClHandle(hNodo)].RecordSet.IsValid() && poCMCR->GetItemScope(hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_READING)  <<M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem)<<DumpTreePath(poAccess, hNodo)<<DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//el nodo ya esta posicionado
        pr=&(poAccess->Node->RecordSet.Current);
    }
	
    //comprobamos que podamos leer
    if (pr->Index == M4DM_EOF_INDEX && poCMCR->GetItemScope(hItem) == M4CL_ITEM_SCOPE_REGISTER ){		
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_NUMBER_READ,M4ObjNodeItemI(poCMCR,hItem)<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem)<<DumpTreePath(poAccess, poAccess->Node->GetpNodeDef()->GetHandle())  << DumpCallStack() );
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
				ClDateToString( m_oState2.m_dDate, acDate, M4_SECOND_END + 1 ) ;
				DUMP_CHLOG_WARNINGF( M4_CH_VM_BASE_SLICED_ITEM_USE, M4ObjNodeItemI( poCMCR, hItem ) << iSlices << poItem->Value.Index << m_oState2.m_iSourceLine << m_oState2.m_poAccess->GetpCMCR()->GetItemFormIdByPosition( m_oState2.m_hItem, m_oState2.m_iRule ) << M4ObjNodeItemI( m_oState2.m_poAccess->GetpCMCR(), m_oState2.m_hItem ) << acDate << DumpCallStack() ) ;
			}

			ret=poItem->Value.Get(vTemp);

			/* Bug 0084877 Hay que poner más información en el mensaje */
			if (ret==M4_ERROR){
				DUMP_CHLOG_ERRORF(M4_CH_VM_CANT_READ_ITEM, M4ObjNodeItemI(poCMCR,hItem)<<pr->Index<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem) << DumpCallStack() );
				return M4_ERROR;
			}
            break;
        }
    case NoSliceWithSlices:
        {
			ret=poItem->Value.Get(m_oState2.m_dBeginDate, m_oState2.m_dEndDate, vTemp);
			/* Bug 0084877 Hay que poner más información en el mensaje */
			if (ret==M4_ERROR){
				ClDateToString( m_oState2.m_dBeginDate, acStartDate, M4_SECOND_END + 1 ) ;
				ClDateToString( m_oState2.m_dEndDate,   acEndDate,   M4_SECOND_END + 1 ) ;
				DUMP_CHLOG_ERRORF(M4_CH_VM_CANT_READ_SLICE_BY_INTERVAL, acStartDate<<acEndDate<<M4ObjNodeItemI(poCMCR,hItem)<<pr->Index<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem) << DumpCallStack() );
				return M4_ERROR;
			}

			dStartDate = m_oState2.m_dBeginDate;
			dEndDate = m_oState2.m_dEndDate;
            break;
        }
        
    case SliceByDate:
        {
			ret=poItem->Value[dTrame].Get(vTemp);
			/* Bug 0084877 Hay que poner más información en el mensaje */
			if (ret==M4_ERROR){
				ClDateToString( dTrame, acStartDate, M4_SECOND_END + 1 ) ;
				DUMP_CHLOG_ERRORF(M4_CH_VM_CANT_READ_SLICE_BY_DATE, acStartDate<<M4ObjNodeItemI(poCMCR,hItem)<<pr->Index<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem) << DumpCallStack() );
				return M4_ERROR;
			}
            break;
        }
    case SliceByOrder:
        {
			ret=poItem->Value[SliceIndex].Get(vTemp);
			/* Bug 0084877 Hay que poner más información en el mensaje */
			if (ret==M4_ERROR){
				DUMP_CHLOG_ERRORF(M4_CH_VM_CANT_READ_SLICE_BY_INDEX, SliceIndex<<M4ObjNodeItemI(poCMCR,hItem)<<pr->Index<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem) << DumpCallStack() );
				return M4_ERROR;
			}
            break;
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
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Error performing push to stack in PushItem-2.")	;
		
	return M4_SUCCESS;
}



m4return_t ClVMachine2::ReadAttribute(void){

	m4return_t res;
	ClAccess * poAccess;
	m4uint32_t hItem ,hNodo;

	res = GetContext ( &poAccess, hNodo, hItem );

	if (res==M4_ERROR){
		return M4_ERROR;
	}

	m_poAttribute->SetContext (poAccess, m_oCurrentIns2->GetFlags(), hNodo, hItem, GetRunSliceMode(poAccess), m_oState2.m_dBeginDate, m_oState2.m_dEndDate);


	res=m_poAttribute->Get(m_oCurrentIns2->GetNumAtrib());
    if (res==M4_ERROR){ 
        DUMP_CHLOG_ERRORF(M4_CH_VM_READING_ATT_EXE_ITEM, M4ObjNodeItemI(poAccess->GetpCMCR(),hItem)<<M4ObjNodeItemI(m_oState2.m_poAccess->GetpCMCR(),m_oState2.m_hItem) << DumpCallStack() );
    }
    return res;
	
}

m4return_t ClVMachine2::ExecuteAttribute(void){
	
	m4return_t res;
	ClAccess * poAccess;
	m4uint32_t hItem, hNodo;

	res = GetContext ( &poAccess, hNodo, hItem);

	if (res==M4_ERROR){
		return M4_ERROR;
	}

	m_poAttribute->SetContext (poAccess, m_oCurrentIns2->GetFlags(), hNodo, hItem, GetRunSliceMode(poAccess), m_oState2.m_dBeginDate, m_oState2.m_dEndDate);

	
	res=m_poAttribute->Run(m_oCurrentIns2->GetNumAtrib());
   // if (res==M4_ERROR){
   //    DUMP_CHLOG_ERRORF(M4_CH_VM_EXECUTING_ATT, M4ObjNodeItemI(poAccess->GetpCMCR(),hItem) << DumpCallStack() );
   // }
    return res;
	
		
}



//****ejecutar función de CANAL****
//Nivel 2


m4return_t ClVMachine2::ExecuteChannelFunction(void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L2, "Execute channel function ... non-operative");
#endif
	m_oRunTimeStack->Pop(); //de momento todos los métods tienen 2 atributos,  sacamos uno de la pila y dejamos el otro como resultado
	return M4_SUCCESS;

}




m4return_t ClVMachine2::Execute(void){

	m4return_t ret,ret2 = M4_SUCCESS;
	ret=M4_SUCCESS;


	m4uint16_t iAuxSttInit = M4_VM_STATISTICS_DISABLED;
	m4VariantType vAuxSttLabel;
	m4pchar_t pcAuxSttLabel = 0;
	ClChannelManager * poChannelManager = 0;

	if (m_oStateStack->GetPos() == 0){
		// Asignamos el objeto estadisticas cuando es el primer Execute (no anidado).
		m4VMState_t oState;
		ret = m_oStateStack->Top(oState);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR,ERRORLOG,__FILE__<<__LINE__<< "Error Reading state.")	;

		poChannelManager = oState.m_poAccess->GetpChannel()->GetpChannelManager();

		iAuxSttInit = GetVMCSContext()->GetSttWrap()->GetStatisticsLevel();
		pcAuxSttLabel = GetVMCSContext()->GetSttWrap()->GetStatisticsLabel();
		if (pcAuxSttLabel)
		{
			vAuxSttLabel.SetString(pcAuxSttLabel);
			pcAuxSttLabel = vAuxSttLabel.Data.PointerChar;
		}

		GetVMCSContext()->GetSttWrap()->SetStatisticsLevel(oState.m_poAccess->GetpChannel()->GetStatisticsLevel(), poChannelManager);
		GetVMCSContext()->GetSttWrap()->SetStatisticsLabel(oState.m_poAccess->GetpChannel()->GetStatisticsLabel());
	}


	//mandamos ejecutar al nivel 1 y tomamos decisiones mientras halla algo que ejecutar
	while (m_iReenter< m_oStateStack->GetPos() && ret==M4_SUCCESS){		
		
		ret2=(ClVMachine1::Execute() );

        DUMP_RTS();
		
		switch( ret2 ){
			
		case 2:	
			{
			    //Hay que ejecutar algo de nivel 2
			
				ret = m_oStateStack->Top(m_oState2);

				DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR,ERRORLOG,__FILE__<<__LINE__<< "Error Reading state.")	;
				
				//Tomar current Inst de ls MV
				m_oCurrentIns2 = (m_oState2.m_poFormula)[m_oState2.m_iCurrent]; 

				//Quitamos el estado
				m_oStateStack->Pop();
								
				//guardamos el estado antes de ejecutar la instrucción
				//de nivel 2, ya que esa instrucción puede meter un
				//nuevo estado en la pila (execute item) y debe quedar por encima de todo.
				//El estado que guardamos ya va con la instrucción incrementada
				m_oState2.m_iCurrent++; //siguiente instrucción			
				m_oStateStack->Push(m_oState2);
				
				//obtenemos el número de función	
				m4uint16_t	ui16NumFunc = m_oCurrentIns2->GetExeFunc();

	#ifdef _DEBUG
				// Mark for check execution
				m4int8_t	iNumArgRef;
				m4int32_t	iPosBeforeExec;
				m4bool_t	bCheckExec;
				
				bCheckExec = MarkBeginExec(ui16NumFunc, iNumArgRef, iPosBeforeExec);			
	#endif
				
				//executamos la función
				ret = ExecuteFunction( ui16NumFunc, m_oImplementors, m_oState2 ) ;

	#ifdef _DEBUG
				// Check execution
				if ( CheckEndExec(ui16NumFunc, iNumArgRef, iPosBeforeExec, ret, bCheckExec) != M4_SUCCESS ) {					
				    DUMP_CHLOG_WARNINGF(M4_CH_VM_TRACE_STACK_VM, 2<<ui16NumFunc );
				}
	#endif

				ret2=ret;

                //si hay un error intentamos recuperarlo
                if (ret!=M4_SUCCESS){
                    
                    //intentamos recuperarnos- Si retorna error es que no podemos recuperarnos 
                    ret=RecoverFromError(m_oState2.m_poAccess);
#ifdef _DEBUG
                    if (ret==M4_SUCCESS){
                        //SETCODE( M4_CH_VM_ERROR_RECOVERED, DEBUGINFOLOG, "Runtime error recovered at level 2")	;
						DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_RECOVERED,"2");
                    }
#endif
					if (ret==M4_SUCCESS) ret2=M4_SUCCESS; //si nos hemos recuperado, para poder seguir
                }
                

				break;
			}


		default: //es de un nivel superior o hemos acabado o error
				 
			{
				if (M4_TRUE == m_oStateStack->IsEmpty())
				{
					// Quitamos el objeto de stt del ejecutor cuando termina la ejecución
					GetVMCSContext()->GetSttWrap()->SetStatisticsLevel(iAuxSttInit, poChannelManager);
					GetVMCSContext()->GetSttWrap()->SetStatisticsLabel(pcAuxSttLabel);
				}

				return ret2;
			}
		} //del switch
		

	} //del while

	if (M4_TRUE == m_oStateStack->IsEmpty())
	{
		// Quitamos el objeto de stt del ejecutor cuando termina la ejecución
		GetVMCSContext()->GetSttWrap()->SetStatisticsLevel(iAuxSttInit, poChannelManager);
		GetVMCSContext()->GetSttWrap()->SetStatisticsLabel(pcAuxSttLabel);
	}

	return ret2;
}


m4return_t ClVMachine2::Execute (m4VMState_t ai_oState, m4uint16_t ai_iNumArgs)
{

	m4return_t		ret;
    m4VariantType	vTemp(0.0);
    m4VariantType	vDate;
	m4int32_t		iOldReenter = m_iReenter;
	m4int32_t		iOldBeginTransaction = m_BeginTransaction;
	m4bool_t		bStepper;
	m4bool_t		bReenterInactive;
	

	m_iReenter = m_oStateStack->GetPos();
	
	// En reentrada se desactiva el stepper
	bStepper = m_oStepper.GetActive();

	if( bStepper == M4_TRUE )
	{
		bReenterInactive = m_oStepper.GetReenterInactive();

		if( m_iReenter != -1 )
		{
			m_oStepper.SetActive( M4_FALSE );
			m_oStepper.SetReenterInactive( M4_TRUE );
		}
	}
	
	ai_oState.m_poAccess->GetpChannel()->UnitDate.Get( vDate );
    m_dUnit = vDate.Data.DoubleData;
    
	//Nuevo. Estadisticas.
	ai_oState.m_iParentStatePos=m_oStateStack->GetPos();
    
    ret = CreateExecute( ai_oState, ai_iNumArgs, vTemp );

	if( ret != M4_SUCCESS )
	{
		m_iReenter = iOldReenter; //restauramos el punto de reentrada
		return M4_ERROR;
	}

	m_oReenterStack->Push( m_iReenter );

     //ejecutar y comprobar resultados
    ret = Execute();

	RefreshReenter( ret );
	
	// Restauramos el modo de stepper
	if( bStepper == M4_TRUE )
	{
		m_oStepper.SetActive( M4_TRUE );
		m_oStepper.SetReenterInactive( bReenterInactive );
	}

	/* Bug 0192122
	En reentrada se restaura el valor de la transacción de máquina virtual
	*/
	m_BeginTransaction = iOldBeginTransaction;

	return ret;
}


//-------------------------------------------------------------------
//OpenChannel
//-------------------------------------------------------------------
m4return_t ClVMachine2::OpenChannel (void)
{
	m4VariantType vOpenMode, vScope, vInstance, vChannelId;
	m4pchar_t pcInstance, pcChannelId;
	ClChannel * pChannel;
	m4uint8_t iScope, iOpenMode;
	m4return_t ret = M4_SUCCESS;

    //Recuperamos de la pila los parametros
    ret = RuntimePop (vOpenMode);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "OpenChannel.")	;
       
    ret = RuntimePop (vScope);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "OpenChannel.")	;

    ret = RuntimePopString (vInstance);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Instance" << "OpenChannel" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vInstance.Type)<< DumpCallStack() );

    ret = RuntimePopString (vChannelId);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ChannelId" << "OpenChannel" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vChannelId.Type)<<DumpCallStack() );


	if (vOpenMode.Type!=M4CL_CPP_TYPE_NUMBER){
        ret=m_oVariantMediator->ToDouble (vOpenMode);
    }
    if (ret!=M4_SUCCESS){ 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "OpenMode"<<"OpenChannel" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vOpenMode.Type)<<DumpCallStack() );

        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	iOpenMode = (m4uint8_t) vOpenMode.Data.DoubleData;
	//chequeamos que esté en rango
	if (iOpenMode > M4CL_T3ALIAS_OPEN_MODE_MAX|| iOpenMode < M4CL_T3ALIAS_OPEN_MODE_MIN ){
		//asumimos valor por def y warning
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "OpenMode"<<"OpenChannel"<<iOpenMode << DumpCallStack() );
		iOpenMode = M4CL_T3ALIAS_OPEN_MODE_NO_LOAD;
	}


	if (vScope.Type!=M4CL_CPP_TYPE_NUMBER){
        ret=m_oVariantMediator->ToDouble (vScope);
    }
    if (ret!=M4_SUCCESS){ 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Scope"<<"OpenChannel" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vScope.Type)<<DumpCallStack() );

        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	iScope = (m4uint8_t) vScope.Data.DoubleData;
	//chequeamos que esté en rango
	if (iScope > M4CL_T3ALIAS_OPEN_SCOPE_MAX|| iScope < M4CL_T3ALIAS_OPEN_SCOPE_MIN ){
		//asumimos valor por def y warning
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Scope"<<"OpenChannel"<<iScope << DumpCallStack() );
		iScope = M4CL_T3ALIAS_OPEN_SCOPE_NOT_SHARED;
	}

	
	pcInstance  = strupr(m_oDynamicPool->GetString(vInstance.Data.CharData)); 
	pcChannelId = strupr(m_oDynamicPool->GetString(vChannelId.Data.CharData)); 
	pChannel = m_oState2.m_poAccess->GetpChannel();


	ret= _OpenChannel(pChannel, pcInstance, pcChannelId, iScope, iOpenMode, M4CL_CSTYPE_L2_SITE, NULL, M4CL_ORGANIZATION_L2_TYPE_FATHER);


	m_oDynamicPool->DeleteString (vInstance.Data.CharData);
	m_oDynamicPool->DeleteString (vChannelId.Data.CharData);

    RuntimePushDouble(ret);

    return ret;
}

//-------------------------------------------------------------------
//OpenChannelEx
//-------------------------------------------------------------------
m4return_t ClVMachine2::OpenChannelEx (void)
{
	m4VariantType vOpenMode, vScope, vInstance, vChannelId, vOrg, vTemp, vNum;
	m4pchar_t pcInstance, pcChannelId, pcOrg;
	ClChannel * pChannel;
	m4uint8_t iScope, iOpenMode, iCSAccessType, iOrgType ;
	m4return_t ret = M4_SUCCESS;
	m4int32_t	iNumArgs,i;

    //Recuperamos de la pila los parametros
	
	//número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "OpenChannelEx.")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
    //demasiados parametros 
    if ( iNumArgs> 7 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "OpenChannelEx"<<7<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 7 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "OpenChannelEx")	;
        }
        iNumArgs=7; //ya hemos sacado los sobrantes
    }

	//cs access type
	if (iNumArgs==7){ //indican Access type
		//sacamos argumento
		ret = RuntimePop (vTemp);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "OpenChannelEx.")	;
		
		//chequeamos tipo 
		if (vTemp.Type!=M4CL_CPP_TYPE_NUMBER){
			ret=m_oVariantMediator->ToDouble (vTemp);
		}
		if (ret!=M4_SUCCESS){ 
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "CS Access Type"<<"OpenChannelEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );
			RuntimePushDouble(M4_ERROR);
			return M4_ERROR;
		}

		iCSAccessType = (m4uint8_t) vTemp.Data.DoubleData;
		
		//chequeamos que esté en rango
		if (iCSAccessType > M4CL_CSTYPE_L2_MAX_NUMBER|| iCSAccessType < M4CL_CSTYPE_L2_MIN_NUMBER ){
			//asumimos valor por def y warning
			DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "CS Access Type"<<"OpenChannelEx"<<iCSAccessType << DumpCallStack() );
			iCSAccessType = M4CL_CSTYPE_L2_SITE;
		}

		//para evaluar el siguiente arg
		--iNumArgs;
	}
	else{//valor por defecto M4CL_CSTYPE_L2_SITE
			iCSAccessType = M4CL_CSTYPE_L2_SITE;
	}

	//tipo de organización
	if (iNumArgs==6){ //indican Tipo de organización,
		//sacamos argumento
		ret = RuntimePop (vTemp);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "OpenChannelEx.")	;
		
		//chequeamos tipo 
		if (vTemp.Type!=M4CL_CPP_TYPE_NUMBER){
			ret=m_oVariantMediator->ToDouble (vTemp);
		}
		if (ret!=M4_SUCCESS){ 
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "OrganizationType"<<"OpenChannelEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );
			RuntimePushDouble(M4_ERROR);
			return M4_ERROR;
		}

		iOrgType = (m4uint8_t) vTemp.Data.DoubleData;
		
		//chequeamos que esté en rango
		if (iOrgType > M4CL_ORGANIZATION_L2_MAX|| iOrgType < M4CL_ORGANIZATION_L2_MIN ){
			//asumimos valor por def y warning
			DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "OrganizationType"<<"OpenChannelEx"<<iOrgType << DumpCallStack() );
			iOrgType = M4CL_ORGANIZATION_L2_TYPE_FATHER;
		}
	
		--iNumArgs;
	}
	else{//valor por defecto M4CL_ORGANIZATION_L2_TYPE_FATHER
		iOrgType = M4CL_ORGANIZATION_L2_TYPE_FATHER;
	}


	//organización
	if (iNumArgs==5){ //indican Organización
		ret = RuntimePop (vOrg);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "OpenChannelEx.")	;
		
		//chequeamos tipo 
		if (vOrg.Type!=M4CL_CPP_TYPE_STRING_POOL){
			ret=m_oVariantMediator->ToPoolString(vOrg);
		}
		if (ret!=M4_SUCCESS){ 
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Organization"<<"OpenChannelEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vOrg.Type)<<DumpCallStack() );
			RuntimePushDouble(M4_ERROR);
			return M4_ERROR;
		}
		--iNumArgs;
	}

	//los argumentos fijos!

    ret = RuntimePop (vOpenMode);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "OpenChannelEx.")	;
       
    ret = RuntimePop (vScope);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "OpenChannelEx.")	;

    ret = RuntimePopString (vInstance);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Instance" << "OpenChannelEx" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vInstance.Type)<< DumpCallStack() );

    ret = RuntimePopString (vChannelId);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ChannelId" << "OpenChannelEx" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vChannelId.Type)<<DumpCallStack() );


	//chequeos de OpenMode
	if (vOpenMode.Type!=M4CL_CPP_TYPE_NUMBER){
        ret=m_oVariantMediator->ToDouble (vOpenMode);
    }
    if (ret!=M4_SUCCESS){ 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "OpenMode"<<"OpenChannelEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vOpenMode.Type)<<DumpCallStack() );

        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	iOpenMode = (m4uint8_t) vOpenMode.Data.DoubleData;
	//chequeamos que esté en rango
	if (iOpenMode > M4CL_T3ALIAS_OPEN_MODE_MAX|| iOpenMode < M4CL_T3ALIAS_OPEN_MODE_MIN ){
		//asumimos valor por def y warning
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "OpenMode"<<"OpenChannelEx"<<iOpenMode << DumpCallStack() );
		iOpenMode = M4CL_T3ALIAS_OPEN_MODE_NO_LOAD;
	}


	//chequeos de Scope
	if (vScope.Type!=M4CL_CPP_TYPE_NUMBER){
        ret=m_oVariantMediator->ToDouble (vScope);
    }
    if (ret!=M4_SUCCESS){ 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Scope"<<"OpenChannelEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vScope.Type)<<DumpCallStack() );

        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	iScope = (m4uint8_t) vScope.Data.DoubleData;
	//chequeamos que esté en rango
	if (iScope > M4CL_T3ALIAS_OPEN_SCOPE_MAX|| iScope < M4CL_T3ALIAS_OPEN_SCOPE_MIN ){
		//asumimos valor por def y warning
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Scope"<<"OpenChannelEx"<<iScope << DumpCallStack() );
		iScope = M4CL_T3ALIAS_OPEN_SCOPE_NOT_SHARED;
	}

	/* Bug 243351
	Hay que hacer una copia de las cadenas del pool
	*/
	m_oVariantMediator->ToCharString( vInstance );
	m_oVariantMediator->ToCharString( vChannelId );
	m_oVariantMediator->ToCharString( vOrg );

	pcInstance = vInstance.Data.PointerChar;
	pcChannelId = vChannelId.Data.PointerChar;
	pcOrg = vOrg.Data.PointerChar;

	pChannel = m_oState2.m_poAccess->GetpChannel();

	ret = _OpenChannel(pChannel, pcInstance, pcChannelId, iScope, iOpenMode, iCSAccessType, pcOrg, iOrgType);

    RuntimePushDouble(ret);
    return ret;
}


m4return_t ClVMachine2::CloseChannel (void)
{
	m4VariantType vInstance;
	m4pchar_t pcInstance;
	//m4VMState_t oState;
	m4return_t ret = M4_SUCCESS;

    //Recuperamos de la pila los parametros
    ret = RuntimePop (vInstance);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "CloseChannel.")	;

    if (vInstance.Type!=M4CL_CPP_TYPE_STRING_POOL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "ChannelInstance"<<"CloseChannel" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vInstance.Type)<< DumpCallStack() );

        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	pcInstance = m_oDynamicPool->GetString(vInstance.Data.CharData); 

	// Buscamos el alias en el mapa del ejecutor 
	m_oState2.m_poAccess->GetpChannel()->Level2.EraseL2Instance(pcInstance, M4_TRUE);

    RuntimePushDouble(ret);
	
	m_oDynamicPool->DeleteString (vInstance.Data.CharData);

    return ret;
}



m4return_t ClVMachine2::InsertChannel (void)
{

	m4VariantType vInstance, vChannelHandle;
	m4uint32_t iHandle;
	m4pchar_t pcInstance;
	m4return_t ret = M4_SUCCESS;


    ret = RuntimePopString (vInstance);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Instance" << "InsertChannel" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vInstance.Type)<< DumpCallStack() );

    ret = RuntimePopNumber (vChannelHandle);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "M4ObjHandle" << "InsertChannel" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vChannelHandle.Type)<<DumpCallStack() );

	pcInstance  = strupr(m_oDynamicPool->GetString(vInstance.Data.CharData));
	iHandle = m4uint32_t( vChannelHandle.Data.DoubleData );


	ret = m_oState2.m_poAccess->GetpChannel()->Level2.InsertL2Channel(pcInstance, iHandle);


	m_oDynamicPool->DeleteString (vInstance.Data.CharData);

    RuntimePushDouble(ret);

    return ret;
}


//-----------------------------------
//GetNumAccessToSerialize
//-----------------------------------
m4uint32_t ClVMachine2::GetNumAccessToSerialize(void){
	if (m_iSerializeFrom == -1 || m_iReenter!=-1){
		return 1; //solo el estado actual
	}
    //return min(m_oStateStack->GetPos()+1-m_iSerializeFrom, m_oStateStack->GetPos()+1 -(m_iReenter +1) ); //el -1 final es para que la referencia entre el m_iReenter y el SerializeFrom sea la misma
	return m_oStateStack->GetPos()+1-m_iSerializeFrom;
}

//-----------------------------------
//GetAccessToSerialize
//-----------------------------------
m4return_t ClVMachine2::GetAccessToSerialize (ClAccess **ai_ppAcesss, m4uint32_t ai_iNumAccess){

	m4uint32_t i;
	m4VMState_t oState;
	m4return_t ret;

	for (i=0; i<ai_iNumAccess; ++i){
		ret=m_oStateStack->Top(oState,i);
		if (ret!=M4_SUCCESS) return ret;
		ai_ppAcesss[i]=oState.m_poAccess;
	}
	return M4_SUCCESS;
}


//
// Function: ClVMachine::Serialize
//
// Arguments:
//    ClGenericIODriver& IOD - Where we serialize to
//
// Returns: m4return_t
//
// Description:
//    To serialize the virtual machine we need to get the
//    current state from the stack of states and serialize that, 
//    and then the arguments to the current method and serialize 
//    that
//
// Author:  Stuart Murdoch (mailto:stuartm@meta4.es)
// Created: 13 January 1998
//
m4return_t ClVMachine2::Serialize(ClGenericIODriver& IOD)
{
	m4VMState_t *poState;
	m4uint32_t	iNumArgs,iNumStates,i;
	m4bool_t	bTakeLocals, anyState;
	m4return_t	ret=M4_SUCCESS;
	m4int32_t	iRef, iTotalStates, iNumLocals;

	iNumLocals=0;

	ret=m_oStepper.Serialize(IOD);	
	if (ret!=M4_SUCCESS) return ret;


	// -- Exclusión de funciones --
	if( m_poExcludeFunctions == NULL )
	{
		if( IOD.Write( m4uint32_t( 0 ) ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}
	else
	{
		if( IOD.Write( m4uint32_t( m_poExcludeFunctions->size() ) ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		itExcludeFunctionsSet_t iterator = m_poExcludeFunctions->begin() ;

		while( iterator != m_poExcludeFunctions->end() )
		{
			if( IOD.Write( (*iterator) ) != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
			iterator++ ;
		}
	}
	// -- Fin exclusión de funciones --
	

	//Si m_iSerializeFrom == -1 =>Serializamos solo el TOP (tambien si hay reentrada)
	//Si != -1 =>serializamos los top-m_iSerializeFrom estados desde el tope

	if (m_iSerializeFrom == -1 || m_iReenter!=-1){ //
		iNumStates=1; //solo el estado actual
		bTakeLocals=M4_FALSE;
	}
	else{
		iNumStates=m_oStateStack->GetPos()+1-m_iSerializeFrom;
		//iNumStates=m_oStateStack->GetPos()+1-m_iSerializeFrom, m_oStateStack->GetPos()+1 -(m_iReenter +1) ); //el -1 final es para que la referencia entre el m_iReenter y el SerializeFrom sea la misma
		bTakeLocals=M4_TRUE;
	}
	
	//en el caso de que estemos en una reentrada, nos debemos llevar un solo
	//estado y sin variables locales, sin embargo como el 

	//reseteamos el serializeFrom, a menos que sea reentrada, que no lo tocamos.
	if (m_iReenter==-1){
		m_iSerializeFrom = -1;
	}

	poState=new m4VMState_t [iNumStates];

	vmauto_ptr_array<m4VMState_t> poAutoPtr(poState); //metemos el array de estados en un auto_ptr_array para que se borren al salir

	ret = GetExecStatus(poState, iNumStates, 0, iNumArgs, anyState, bTakeLocals, iRef,iTotalStates,iNumLocals);

	if (ret=M4_SUCCESS){
		return M4_ERROR;
	}

	if (!anyState){
		iNumStates=0;
		bTakeLocals=M4_FALSE;
	}
		
	//el numero de estados	
	if (IOD.Write(iNumStates)!=M4_SUCCESS){
		return M4_ERROR;
	}

	/* Bug 0117879
	Las variables no enviadas son todas menos las enviadas
	El cálculo es las que hay en la pila menos las que se van a quitar
	Se quitan el número de variables (iNumArgs) si se llevan variables locales,
	porque sino	si se serializan las variables, pero sin quitarlas de la pila
	*/
	m4int32_t	iNotSentVariables = m_oRunTimeStack->GetPos() + 1;

	if( bTakeLocals == M4_TRUE )
	{
		iNotSentVariables -= iNumArgs;
	}

	//cada uno de los estados
	for (i=0; i<iNumStates;++i){
		ret = poState[i].Serialize(IOD, bTakeLocals, iNotSentVariables);
		if (ret != M4_SUCCESS){
			return ret;
		}
	}

	//Numero total de estados para hallar el offset
	if (IOD.Write(iTotalStates )!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	//la referencia
	if (IOD.Write(iRef )!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	//la si van locales o no
	if (IOD.Write((m4bool_t)bTakeLocals)!=M4_SUCCESS){
		return M4_ERROR;
	}

	//el número de argumentos
	if (IOD.Write(iNumArgs) !=M4_SUCCESS){
		return M4_ERROR;
	}
	
	//el número de variants locales
	if (IOD.Write(iNumLocals) !=M4_SUCCESS){
		return M4_ERROR;
	}

			
	//los variants en sí
	if (iNumStates || iNumArgs)
	{
		m4VariantType* poVars = new m4VariantType[iNumArgs+1]; //Allways alloc 1 or more

		ret = GetExecStatus(poState, iNumStates, poVars, iNumArgs, anyState, bTakeLocals, iRef, iTotalStates,iNumLocals);
						
		if (ret != M4_ERROR)	{
			ret = M4_SUCCESS; 
						
			for (i = 0; i < iNumArgs && ret == M4_SUCCESS; i++)
			{
				if (poVars[i].Type == M4CL_CPP_TYPE_STRING_POOL)
				{
					poVars[i].Data.PointerChar=m_oDynamicPool->GetString (poVars[i].Data.CharData);
					poVars[i].Type = M4CL_CPP_TYPE_STRING_VAR;
				}
				ret = poVars[i].Serialize(IOD);
			}			
		}
		//delete vars
		delete [] poVars;
	}

	if (ret != M4_ERROR) 
	{
		GetVMCSContext()->GetSttWrap()->Serialize(IOD);
	}
		
	if (!iNumStates)
	{
		Reset();//reentrada
	}

	return ret;
}


//
// Function: ClVMachine2::DeSerialize
//
// Arguments:
//    ClGenericIODriver& IOD - Where we serialize to
//
// Returns: m4return_t
//
// Description:
//    To deserialize the virtual machine we need to deserialize the
//    current state, we need to deserialize the number of arguments
//    and we need to deserialize each of the arguments, and then
//    restore the execution state of the virtual machine
//
// Author:  Stuart Murdoch (mailto:stuartm@meta4.es)
// Created: 13 January 1998
//
m4return_t ClVMachine2::DeSerialize (ClGenericIODriver& IOD, ClChannelManager * ai_pChannelManager)
{
	m4VMState_t *poState;
	m4uint32_t iNumArgs,iNumStates,i;
	m4int32_t iRef;
	m4int32_t iTotalStates;
	m4int32_t iNumLocals;
	m4bool_t bTakeLocals;

	
	m4return_t ret = m_oStepper.DeSerialize(IOD);	
	if (ret != M4_SUCCESS) return ret;

	// -- Exclusión de funciones --
	if( m_poExcludeFunctions != NULL )
	{
		m_poExcludeFunctions->clear() ;
	}

	m4uint32_t	iExcludes = 0 ;
	m4uint16_t	iFunction = 0 ;

	if( IOD.Read( iExcludes ) != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( iExcludes == 0 )
	{
		if( m_poExcludeFunctions != NULL )
		{
			delete( m_poExcludeFunctions ) ;
			m_poExcludeFunctions = NULL ;
		}
	}
	else
	{
		if( m_poExcludeFunctions == NULL )
		{
			m_poExcludeFunctions = new ExcludeFunctionsSet_t ;
			CHECK_CHLOG_ERRORF( m_poExcludeFunctions == NULL, M4_ERROR, M4_CH_VM_OUT_OF_MEMORY, m4uint32_t( sizeof( ExcludeFunctionsSet_t ) ) << __FILE__ << __LINE__ ) ;
		}

		for( i = 0 ; i < iExcludes ; i++ )
		{
			if( IOD.Read( iFunction ) != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			m_poExcludeFunctions->insert( iFunction ) ;
		}

	}
	// -- Fin exclusión de funciones --



	//Segun el número de estados que nos vengan:
	// 0->En el origen (que va a ser el server en este caso, fijo), la ejecución acabó->ponemos m_iSerializeFrom=-1
	// Si viene != 0 ->actualizamos al m_iSerializeFrom = Tope de la pila

	//numero de estados
	ret = IOD.Read(iNumStates);
	if (ret != M4_SUCCESS) return ret;
	
	poState = new m4VMState_t [iNumStates];

	vmauto_ptr_array<m4VMState_t> poAutoPtr(poState); //metemos el array de estados en un auto_ptr_array para que se borren al salir

	//ponemos el SerializeFrom donde deba estar
	if (iNumStates) {
		m_iSerializeFrom=m_oStateStack->GetPos()+1;	
	}
	else if (m_iReenter==-1) {
		m_iSerializeFrom=-1;
	}
	
	/* Bug 0117879
	Las variables en la pila se calculan directamente
	*/
	m4int32_t	iVariablesInStack = m_oRunTimeStack->GetPos() + 1;

	//cada uno de los estados
	for (i = 0; i < iNumStates; ++i) {
		ret = poState[i].DeSerialize(IOD, iVariablesInStack);
		if (ret != M4_SUCCESS) return ret;
	}

	//Numero total de estados para hallar el offset
	ret = IOD.Read(iTotalStates);
	if (ret != M4_SUCCESS) return ret;

	//la referencia
	ret = IOD.Read(iRef);
	if (ret != M4_SUCCESS) return ret;

	//Si van locales o no
	ret = IOD.Read(bTakeLocals);
	if (ret != M4_SUCCESS) return ret;

	//el numero de variants
	ret = IOD.Read(iNumArgs);
	if (ret != M4_SUCCESS) return ret;

	//el numero de variants locales
	ret = IOD.Read(iNumLocals);
	if (ret != M4_SUCCESS) return ret;


	m4VariantType *poVars = new m4VariantType[iNumArgs];
	
	for (i = 0; i < iNumArgs && ret == M4_SUCCESS; i++)	{
		ret = poVars[i].DeSerialize(IOD);
	}
	
	if (ret == M4_SUCCESS)
	{
		//Se podría meter todo en RestoreExecution, pero haría falta un nuevo parametro
		if (iNumStates)
		{
			ret = RestoreExecution(poState, iNumStates, poVars, iNumArgs, bTakeLocals, iRef,iTotalStates,iNumLocals);
		}
		else
		{
			//Ponemos la variables en la pila tal cual.
			for (m4int32_t i = iNumArgs-1; i >= 0; i--)
			{
				if (poVars[i].Type == M4CL_CPP_TYPE_STRING_VAR)
				{
					ret = RuntimePushString (poVars[i].Data.PointerChar, poVars[i].IsUser);
				}
				else
				{
					ret=RuntimePush (poVars[i]);
				}
			}

			/* Bug 0068832
			m4VMState_t oState;
			Se comenta este código. No tiene sentido que se actualice la referencia
			hasta que se elimine el estado de ejecución de la pila. Eso se hace en el
			método _FreeAtEndOfMethod

			//actualizamos la referencia
			if (m_oStateStack->Top(oState)==M4_SUCCESS && iNumArgs==1){
				//Bug:61605
				// Estamos en el caso de que se ha terminado la ejecución de un método mapped en el server
				//  y vamos a deserializar el resultado en el cliente.
				// Si el resultado es error, no podemos cambiar la referencia, puesto que posteriormente se
				//  hará un RecoverFromError
				m4VariantType vTemp;
				m_oRunTimeStack->Top(vTemp);
				if( (vTemp.Type == M4CL_CPP_TYPE_NUMBER) && (vTemp.Data.DoubleData != -1) ) {
					m_oRunTimeStack->SetReference(oState.m_iOldStackReference);
				}
			}
			*/
		}
	}

	//delete vars
	delete [] poVars;
	
	if (ret != M4_ERROR)
	{
		GetVMCSContext()->GetSttWrap()->DeSerialize(IOD, ai_pChannelManager);
	}
	
	return ret;
}


//-----------------------------------
//Load de canal
//-----------------------------------

m4return_t ClVMachine2::LoadChannel (void){

	ClAccess * poAccess;
	
	m4return_t ret;

	ret=GetAccessContext(&poAccess );
	if (ret==M4_ERROR){
		return M4_ERROR;
	}
	
	ret=poAccess->GetpChannel()->Load();
	RuntimePushDouble(ret);
	if (ret!= M4_SUCCESS){
		DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_LOAD, M4Obj(poAccess->GetpChannel()->GetpCMCR()) << DumpCallStack() );
	}
	return M4_SUCCESS;
}

//-----------------------------------
//Persist de canal
//-----------------------------------
m4return_t ClVMachine2::PersistChannel (void){


	ClAccess * poAccess;
	
	m4return_t ret;
	
	ret=GetAccessContext(&poAccess );

	if (ret==M4_ERROR){
		return M4_ERROR;
	}

	ret=poAccess->GetpChannel()->Persist();
	RuntimePushDouble(ret);
	if (ret!= M4_SUCCESS){
		DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_PERSIST_CHANNEL, M4Obj(poAccess->GetpChannel()->GetpCMCR()) << DumpCallStack() );
	}
	return M4_SUCCESS;


}

//-----------------------------------
//Persist de canal
//-----------------------------------
m4return_t ClVMachine2::ReleaseChannel (void){


	ClAccess * poAccess;
	
	m4return_t ret;
	
	ret=GetAccessContext(&poAccess );

	if (ret==M4_ERROR){
		return M4_ERROR;
	}

	ret=poAccess->GetpChannel()->ReleaseAll();
	RuntimePushDouble(ret);
	if (ret!= M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_RELEASE_CHANNEL, M4Obj(poAccess->GetpChannel()->GetpCMCR())  << DumpCallStack() );
	}
	return M4_SUCCESS;


}
//-----------------------------------
//Unload de canal
//-----------------------------------
m4return_t ClVMachine2::UnloadChannel (void){


	ClAccess * poAccess;
	
	m4return_t ret;
	
	ret=GetAccessContext(&poAccess );

	if (ret==M4_ERROR){
		return M4_ERROR;
	}

	ret=poAccess->UnLoad();
	RuntimePushDouble(ret);
	if (ret!= M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_UNLOAD_CHANNEL, M4Obj(poAccess->GetpChannel()->GetpCMCR()) << DumpCallStack() );
	}
	return M4_SUCCESS;


}

//-----------------------------------
//Propiedad Data Cacheable del Canal
//-----------------------------------
// Funcion LN4: Channel.IsDataCacheable()	- ""
//	- Entrada: Ninguna
//	- Salida:  M4_TRUE = Is Cacheable || M4_FALSE = No Is Cacheable
m4return_t ClVMachine2::IsDataCacheable (void)
{

	ClAccess * poAccess;
	m4return_t ret;
	
	ret=GetAccessContext(&poAccess );

	if (ret==M4_ERROR){
		return M4_ERROR;
	}

	ClCompiledMCR   *poCMCR = poAccess->GetpCMCR();
	M4_ASSERT(poCMCR);

	// Devolvemos la Propiadad del Canal
	RuntimePushDouble( (poCMCR->GetChannelIsCacheable() == M4CL_CACHEABLE_TRUE) ? 1.0 : 0.0 );
	return M4_SUCCESS;
}




//-----------------------------------
//Función de AddFriend
//-----------------------------------
// Funcion LN4: Channel!AddFriend()	- ""
//	- Entrada: Ninguna
//	- Salida:  M4_SUCCESS|M4_ERROR
m4return_t ClVMachine2::AddAsFriend (void)
{

	ClAccess * poAccess;
	m4return_t ret;
	
	ret=GetAccessContext(&poAccess );

	if (ret==M4_ERROR){
		return M4_ERROR;
	}
	ret=m_oState2.m_poAccess->GetpChannel()->m_oGroupInfo.AddPartner(poAccess->GetpChannel() );
	
	// Devolvemos el resultado
	RuntimePushDouble( ret );
	return M4_SUCCESS;
}


//-----------------------------------
//Función de RemoveAsFriend
//-----------------------------------
// Funcion LN4: Channel!RemoveAsFriend()	- ""
//	- Entrada: Ninguna
//	- Salida:  M4_SUCCESS|M4_ERROR
m4return_t ClVMachine2::RemoveAsFriend (void)
{

	ClAccess * poAccess;
	m4return_t ret;
	
	ret=GetAccessContext(&poAccess );

	if (ret==M4_ERROR){
		return M4_ERROR;
	}
	m_oState2.m_poAccess->GetpChannel()->m_oGroupInfo.DelPartner(poAccess->GetpChannel() );
	
	// Devolvemos el resultado
	RuntimePushDouble( M4_SUCCESS );
	return M4_SUCCESS;
}

//-----------------------------------
//Función de GetProperty
//-----------------------------------
// Funcion LN4: Channel!GetProperty(Property_name)	- ""
//	- Entrada: Identificador de la propiedad
//	- Salida:  Value of the property
m4return_t ClVMachine2::GetProperty (void)
{

	ClAccess * poAccess;
	m4return_t ret;
	m4pchar_t pcName = NULL;
	m4VariantType vPropName,vValue;

	ret=GetAccessContext(&poAccess );

	if (ret==M4_ERROR){
		return M4_ERROR;
	}
	
	ret = RuntimePopString (vPropName);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "PropertyName" << "GetProperty" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vPropName.Type)<< DumpCallStack() );

	//leemos la prop
	pcName = m_oDynamicPool->GetString( vPropName.Data.CharData );

	if( poAccess->Property.GetProperty( pcName ) != NULL )
	{
		// Bug 0298232. Se permite asignar las propiedades de acceso
		ret = poAccess->Property.GetPropValue( pcName, vValue ) ;
	}
	else
	{
		ret = poAccess->GetpChannel()->Property.GetPropValue( pcName, vValue ) ;
	}
	
	if (ret!=M4_SUCCESS){
//		DUMP_CHLOG_WARNINGF( M4_CH_VM_NO_SUCH_COL_PROP, m_oDynamicPool->GetString(vPropName.Data.CharData) ) ;
		vValue.SetNull();
	}

	//liberamos la variable
	m_oDynamicPool->DeleteString(vPropName.Data.CharData);

	// Devolvemos el resultado
	if (vValue.Type==M4CL_CPP_TYPE_STRING_VAR){
		RuntimePushString (vValue.Data.PointerChar, vValue.IsUser);
	}
	else{
		RuntimePush(vValue);
	}
	return M4_SUCCESS;
}


//-----------------------------------
//Función de SetProperty
//-----------------------------------
// Funcion LN4: Channel!SetProperty(Property_name, value, [CS_TYPE])	- ""
//	- Entrada: Identificador de la propiedad, Valor, Tipo CS si es una propiedad nueva
//	- Salida:  M4_SUCCESS|M4_ERROR
m4return_t ClVMachine2::SetProperty (void)
{

	ClAccess * poAccess;
	m4return_t ret;
	m4VariantType vPropName,vValue,vNum;
	m4uint32_t iNumArgs,i;
	m4bool_t bHas3rdArg=M4_FALSE, bMapped=M4_FALSE;
	m4language_t lLanguage = M4_LANG_NONE_ID;
	m4pchar_t pcName = NULL;

	ret=GetAccessContext(&poAccess );
	
	if (ret==M4_ERROR){
		return M4_ERROR;
	}

    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetProperty.")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
    //demasiados parametros ->Borramos los que sobran, más mensaje de aviso
    if ( iNumArgs> 2 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "SetProperty"<<"2"<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 2 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetProperty")	;
        }
        iNumArgs=2; //ya hemos sacado los sobrantes
    }
    

	//valor a meter
	ret = RuntimePop (vValue);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetProperty" );

	//nombre de la propiedad
	ret = RuntimePopString (vPropName);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "PropertyName" << "SetProperty" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vPropName.Type)<< DumpCallStack() );

	//Asignamos la prop
	if (vValue.Type==M4CL_CPP_TYPE_STRING_POOL){
		//cambiamos el string_pool a string_char
		m_oVariantMediator->ToCharString(vValue);
	}
	
	/* Bug 0259208
	Se chequea la valided de la propiedad de lenguaje
	*/
	ret = M4_SUCCESS;
	pcName = m_oDynamicPool->GetString( vPropName.Data.CharData );

	if( strcmpi( pcName, M4CMPROP_CHANNEL_DATA_LANGUAGE ) == 0 )
	{
		m_oVariantMediator->ToDouble( vValue );
		lLanguage = m4language_t( vValue.Data.DoubleData );
		
		if( M4ClLangInfo::Instance()->CheckAndSetValidLangId( lLanguage ) == M4_FALSE )
		{
			ret = M4_ERROR ;
		}
	}

	if( ret == M4_SUCCESS )
	{
		if( poAccess->Property.GetProperty( pcName ) != NULL )
		{
			// Bug 0298232. Se permite asignar las propiedades de acceso
			ret = poAccess->Property.SetPropValue( pcName, vValue ) ;
		}
		else
		{
			ret = poAccess->GetpChannel()->Property.SetOrNewProperty( pcName, vValue, M4_PROP_LN4_STATE );
		}
	}
	
	//liberamos la variable
	m_oDynamicPool->DeleteString(vPropName.Data.CharData);

	//resultado a la pila
	RuntimePushDouble(ret);
	// Devolvemos el resultado
	return M4_SUCCESS;
}


//-----------------------------------
//Función de GetHandle
//-----------------------------------
// Funcion LN4: Channel!GetHandle()	- ""
//	- Entrada: Ninguna
//	- Salida:  Handle del canal
m4return_t ClVMachine2::GetHandle (void)
{

	ClAccess * poAccess;
	m4return_t ret;

	ret=GetAccessContext(&poAccess );

	if (ret==M4_ERROR){
		return M4_ERROR;
	}

	// Devolvemos el handle de canal
	RuntimePushDouble( poAccess->GetpChannel()->GetHandle() );
	return M4_SUCCESS;
}



//-----------------------------------
//Función de LoadChannelFromFile
//-----------------------------------
// Funcion LN4: Channel!LoadChannelFromFile(file_name, ... )	- ""
//	- Entrada: nombre del fichero
//	- Salida:  Código de error
m4return_t ClVMachine2::LoadChannelFromFile (void)
{

	ClAccess * poAccess;
	ClChannel * poChannel;
	m4return_t ret;
	m4VariantType vFileName,vNum;
	m4uint32_t iNumArgs,i;

    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "LoadFromFile.")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
    //demasiados parametros ->Borramos los que sobran, más mensaje de aviso
    if ( iNumArgs> 1 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "LoadFromFile"<<"1-1"<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 1 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "LoadFromFile")	;
        }
        iNumArgs=1; //ya hemos sacado los sobrantes
    }
    

	//nombre del fichero
	ret = RuntimePopString (vFileName);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "FileName" << "LoadFromFile" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vFileName.Type)<< DumpCallStack() );

	ret=GetAccessContext(&poAccess );

	if (ret==M4_ERROR){
		m_oDynamicPool->DeleteString(vFileName.Data.CharData);
		return M4_ERROR;
	}
	
	poChannel=poAccess->GetpChannel();

	if (poChannel==m_oState2.m_poAccess->GetpChannel()) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_AUTO_M4OBJ_FUNCTION, "LoadFromFile" << M4Obj(poChannel->GetpCMCR()) << DumpCallStack() );
		m_oDynamicPool->DeleteString(vFileName.Data.CharData);
		return M4_ERROR;
	}
	else{
		ret=poChannel->LoadFromFile(m_oDynamicPool->GetString(vFileName.Data.CharData));
	}

	//liberamos la variable
	m_oDynamicPool->DeleteString(vFileName.Data.CharData);

	//resultado a la pila
	RuntimePushDouble(ret);
	// Devolvemos el resultado
	return M4_SUCCESS;
}


//-----------------------------------
//Función de PersistChannelToFile
//-----------------------------------
// Funcion LN4: Channel!PersistChannelToFile([file_name], ... )	- ""
//	- Entrada: nombre del fichero (opcional)
//	- Salida:  Nulo si hay error y el nombre del fichero sino
m4return_t ClVMachine2::PersistChannelToFile (void)
{

	ClAccess * poAccess;
	m4bool_t bIsUser = M4_FALSE;
	m4return_t ret;
	m4VariantType vFileName,vNum;
	m4uint16_t iLength;
	m4uint32_t iNumArgs,i;
	m4pcchar_t pccString;
	m4char_t acFileName[ M4CH_MAX_FILE+1 ];

    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "PersistToFile.")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
    //demasiados parametros ->Borramos los que sobran, más mensaje de aviso
    if ( iNumArgs> 1 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "PersistToFile"<<"0-1"<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 1 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "PersistToFile")	;
        }
        iNumArgs=1; //ya hemos sacado los sobrantes
    }
    

	//nombre del fichero si tiene
	if (iNumArgs==1){
		ret = RuntimePopString (vFileName);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "FileName" << "PersistToFile" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vFileName.Type)<< DumpCallStack() );

		pccString=m_oDynamicPool->GetString(vFileName.Data.CharData);
		iLength=strlen(pccString);
		if (iLength>M4CH_MAX_FILE){
			iLength=M4CH_MAX_FILE;
		}
		memcpy(acFileName, pccString, sizeof(m4char_t)*iLength);
		*(acFileName+iLength)='\0';

		//liberamos la variable
		m_oDynamicPool->DeleteString(vFileName.Data.CharData);
		bIsUser = vFileName.IsUser;
	}

	ret=GetAccessContext(&poAccess );

	if (ret==M4_ERROR){
		return M4_ERROR;
	}
	
	if (iNumArgs==1){
		ret=poAccess->GetpChannel()->PersistToFile(acFileName, M4_FALSE);
	}
	else{
		ret=poAccess->GetpChannel()->PersistToTempFile(acFileName,M4CH_MAX_FILE);
	}

	//resultado a la pila
	if (ret==M4_SUCCESS){
		RuntimePushString(acFileName, bIsUser);
	}
	else{
		RuntimePushNull();
	}

	// Devolvemos el resultado
	return M4_SUCCESS;
}



//-----------------------------------
//Función de CheckModify
//-----------------------------------
// Funcion LN4: Channel!CheckModify(... )	- ""
//	- Entrada: de momento nada
//	- Salida:  M4_TRUE si hay que cambios, M4_FALSE sino
m4return_t ClVMachine2::CheckModify (void)
{

	ClAccess * poAccess;
	m4return_t ret;
	m4VariantType vNum;
	m4bool_t bModified;
	m4uint32_t iNumArgs,i;
	m4double_t dResult;

    //número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "CheckModify.")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;
    //demasiados parametros ->Borramos los que sobran, más mensaje de aviso
    if ( iNumArgs> 0 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "CheckModify"<<"0"<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs  ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "CheckModify")	;
        }
        iNumArgs=0; //ya hemos sacado los sobrantes
    }
    

	ret=GetAccessContext(&poAccess );

	if (ret==M4_ERROR){
		return M4_ERROR;
	}
	
	ret=poAccess->GetpChannel()->CheckModify(bModified);

	//resultado a la pila
	if (ret==M4_SUCCESS){
		if (bModified==M4_TRUE){
			dResult=M4_TRUE;
		}
		else{
			dResult=M4_FALSE;
		}
	}
	else{
		dResult=M4_FALSE;
	}

	RuntimePushDouble(dResult);

	// Devolvemos el resultado
	return M4_SUCCESS;
}



//-------------------------------------------------------------
//Internas
//-------------------------------------------------------------

m4return_t ClVMachine2::GetContext ( ClAccess ** ao_pAccess, m4uint32_t& ao_hNode, m4uint32_t& ao_hItem   )
{
	ClCompiledMCR   *poCMCR;
	m4pcchar_t pcChannelInstance, pcNode, pcItem;
	//m4uint32_t hNode;
	m4return_t res = M4_SUCCESS;

	poCMCR = m_oState2.m_poAccess->GetpCMCR();

	//Obtener Chanel Inst!Node.Item
	//JIT
	if (!(M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState2.m_Flags)))
	{
		pcChannelInstance =  poCMCR->GetPointer( m_oCurrentIns2->GetNameChannel() ) ;

		pcNode = poCMCR->GetPointer( m_oCurrentIns2->GetNumNodo() ) ;

		pcItem = poCMCR->GetPointer( m_oCurrentIns2->GetNumItem() ) ;
	}
	else 
	{
		pcChannelInstance=&(m_oState2.m_poFormula.GetFormulaHandle()[m_oCurrentIns2->GetNameChannel()]);
		pcNode=&(m_oState2.m_poFormula.GetFormulaHandle()[m_oCurrentIns2->GetNumNodo()]);
		pcItem=&(m_oState2.m_poFormula.GetFormulaHandle()[m_oCurrentIns2->GetNumItem()]);
	}
	////
	//----Access del nuevo canal   
//	res = _GetL2Access ( m_oState2.m_poAccess->GetpChannel(), pcChannelInstance, ao_pAccess );
	res = GetL2Access ( m_oState2.m_poAccess->GetpChannel(), pcChannelInstance, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState2.m_Flags), ao_pAccess );
	if ( res == M4_ERROR )
		return M4_ERROR;

	//----Statistics en L2
	/*
	if (m_iSttInit)
	{
		//Activamos las statistics
		(*ao_pAccess)->SetSttStatus(m_iSttInit);

		//Ponemos la SttLabel del access local (maquina donde ejecutamos)
		(*ao_pAccess)->SetSttLabel( GetSttLabel() );
	}*/

	//----NumItem (Handle)
	poCMCR = (*ao_pAccess)->GetpCMCR();
	
	ao_hNode = poCMCR->GetChannelNodeHandleById( pcNode ) ;

	if( ao_hNode == NULL )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_L2_NODE, pcNode <<(*ao_pAccess)->GetpChannel()->GetpChannelDef ()->Id ()<< DumpCallStack() );
		return( M4_ERROR ) ;
	}

	ao_hItem = poCMCR->GetNodeItemHandleById( ao_hNode, pcItem ) ;
	
	if( ao_hItem == 0 )
	{
        DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_L2_ITEM, pcItem<<pcNode << (*ao_pAccess)->GetpChannel()->GetpChannelDef()->Id()<<DumpCallStack() );
		return( M4_ERROR ) ;
	}	

	return M4_SUCCESS;	
}


m4return_t ClVMachine2::GetAccessContext ( ClAccess ** ao_pAccess){
	
	
	m4pcchar_t pcChannelInstance;


	//Obtener Chanel Inst!Node.Item
	//JIT
	if (!(M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState2.m_Flags))){
		pcChannelInstance =  m_oState2.m_poAccess->GetpCMCR()->GetPointer( m_oCurrentIns2->GetNameChannel() ) ;
	}
	else {
		pcChannelInstance=&(m_oState2.m_poFormula.GetFormulaHandle()[m_oCurrentIns2->GetNameChannel()]);
	}

	return GetL2Access ( m_oState2.m_poAccess->GetpChannel(), pcChannelInstance, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState2.m_Flags), ao_pAccess );
}


m4return_t ClVMachine2::ExecuteFunctionForLocalTest (m4uint16_t iNumFunction){ //para que desde el local test puedan ejecutar funciones

	//Executamos la función
	return ExecuteFunction( iNumFunction, m_oImplementors, m_oState2 ) ;
}

void ClVMachine2::RefreshState(void){
		//recuperamos el estado
		ClVMachine1::RefreshState();
		m_oStateStack->Top(m_oState2);

}
void ClVMachine2::RefreshReenter(m4return_t ai_rRet){

	if (ai_rRet>2 || ai_rRet == M4CL_EXECUTION_INTERRUPTION) { //no podemos ejecutarlo o es una interrupción
		
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

//-------------------------------------------------------------------
// -- Añade el registro de auditoria de usuario correspondiente a la
// -- ejecución de método.
//-------------------------------------------------------------------
m4return_t	ClVMachine2::RegisterUserAuditEvents()
{
	m4return_t			iRet ;
	ClChannelManager*	poChannelManager = NULL ;
	m4pchar_t			pcOrganization ;


	if( m_oStateStack->GetPos() == 0 )
	{
		m4VMState_t oState ;
		iRet = m_oStateStack->Top( oState ) ;
		DEBUG_CHECK_RES( iRet == M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR,ERRORLOG,__FILE__<<__LINE__<< "Error Reading state.")	;

		poChannelManager = oState.m_poAccess->GetpChannel()->GetpChannelManager();

		if( poChannelManager->GetDMContext()->ExecutingInClient() == M4_FALSE )
		{
			oState.m_poAccess->GetpChannel()->Organization.Get( pcOrganization );

			if( poChannelManager->UserActionsAudit.HasAuditory( ClUserAction::EV_METHOD, pcOrganization ) == M4_TRUE )
			{
				RegisterUserAuditMethodEvent( oState, 0, pcOrganization ) ;
			}
		}
	}

	return M4_SUCCESS ;
}

//-------------------------------------------------------------------
//Funcion de LodChannel (sobreescribe una definición de instancia)
//Genérica, a la que llaman las otras dos
//-------------------------------------------------------------------
m4return_t  ClVMachine2::_OpenChannel	(ClChannel *ai_poChannel, const m4pchar_t ai_pszInstance, 
										const m4pchar_t ai_pszChannelId, m4uint8_t ai_iScope,
										m4uint8_t ai_iOpenMode, m4uint8_t ai_iCSAccessType, 
										const m4pchar_t ai_pcOrg, m4uint8_t ai_iOrgType){

	m4return_t ret;
	m4pchar_t pcRole;
	ClAccess * pAccess;


	// Buscamos el alias en el mapa del ejecutor 
	// A lo que vamos:
	// Si la instancia ya está levantada con anterioridad no se puede sobreescribir
	// y te quedas con lo que tenías
	// Si la instancia no estaba levantada, se levanta (y se crea el canal)
	// con los nuevos parámetros


    if ( ai_poChannel->Level2.m_oInstanceMap.Find ( ai_pszInstance, &pAccess ) == M4_ERROR ) {
		ai_poChannel->Role_ID.Get( pcRole );
		ret = ai_poChannel->Level2.InsertL2Instance ( ai_pszInstance, ai_pszChannelId, ai_iScope, ai_iOpenMode, ai_iCSAccessType, &pAccess, pcRole, ai_pcOrg, ai_iOrgType, M4_TRUE );
	}
	else{
		//La instancia ya esta en el mapa.
		ret = M4_SUCCESS;
	}

	return ret;
}


m4return_t ClVMachine2::EraseAllL2Instances (void)
{
	m4return_t ret = M4_SUCCESS;
	ClChannel * pChannel;

	pChannel = m_oState2.m_poAccess->GetpChannel();

	ret = pChannel->Level2.EraseAllL2Instances(M4_TRUE, M4_TRUE, NULL);

    RuntimePushDouble(ret);

    return ret;
}


//------------------------------------
//Bookmarks
//------------------------------------
m4return_t ClVMachine2::GetBookMark(void)
{

    m4return_t      iResult = M4_ERROR ;
	m4uint8_t		iType = -1 ;
	m4uint32_t		iBookMark = -1 ;
    m4VariantType   vType ;
	ClAccess		*poAccess = NULL ;

#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF( M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetBookMark" );
#endif


	iResult = RuntimePopDouble( vType ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Type" << "GetBookMark" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vType.Type ) << DumpCallStack() ) ;
    iType = (m4int8_t)vType.Data.DoubleData ;

	iResult = GetAccessContext( &poAccess ) ;

	if( iResult != M4_SUCCESS )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return M4_ERROR;
	}

	if( poAccess == m_oState2.m_poAccess->GetpChannel()->Level2.GetSelfAccess() )
	{
		// Si es el propio canal se toma el acceso original y no el self
		poAccess = m_oState2.m_poAccess ;
	}
	
	iBookMark = poAccess->GetpChannel()->NewBookMark( iType, poAccess ) ;

	if( iBookMark == -1 )
	{
   		DUMP_CHLOG_ERRORF( M4_CH_VM_NO_GET_BOOKMARK, poAccess->GetpCMCR()->iM4Obj() << LogCatAnd << poAccess->GetpCMCR()->nM4Obj() << DumpCallStack() );
		RuntimePushDouble( M4_ERROR ) ;
		return M4_ERROR;
	}

	RuntimePushDouble( iBookMark ) ;
	return M4_SUCCESS;
}


m4return_t ClVMachine2::ApplyBookMark(void)
{

    m4return_t      iResult = M4_ERROR ;
	m4uint32_t		iBookMark = -1 ;
    m4VariantType   vBookmark ;
	ClAccess		*poAccess = NULL ;

#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF( M4_CH_VM_TRACE_VM_L1, "Trace VM1: ApplyBookMark" );
#endif


	iResult = RuntimePopDouble( vBookmark ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Bookmark" << "ApplyBookMark" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vBookmark.Type ) << DumpCallStack() ) ;
    iBookMark = (m4int32_t)vBookmark.Data.DoubleData ;

	iResult = GetAccessContext( &poAccess ) ;

	if( iResult != M4_SUCCESS )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return M4_ERROR;
	}

	if( poAccess == m_oState2.m_poAccess->GetpChannel()->Level2.GetSelfAccess() )
	{
		// Si es el propio canal se toma el acceso original y no el self
		poAccess = m_oState2.m_poAccess ;
	}
	
	iResult = poAccess->GetpChannel()->ApplyBookMark( iBookMark, poAccess ) ;

	if( iResult != M4_SUCCESS )
	{
   		DUMP_CHLOG_ERRORF( M4_CH_VM_NO_APPLY_BOOKMARK, iBookMark << poAccess->GetpCMCR()->iM4Obj() << LogCatAnd << poAccess->GetpCMCR()->nM4Obj() << DumpCallStack() );
		RuntimePushDouble( M4_ERROR ) ;
		return M4_ERROR;
	}

	RuntimePushDouble( M4_SUCCESS ) ;
	return M4_SUCCESS;
}


m4return_t ClVMachine2::ReleaseBookMark(void)
{

    m4return_t      iResult = M4_ERROR ;
	m4uint32_t		iBookMark = -1 ;
    m4VariantType   vBookmark ;
	ClAccess		*poAccess = NULL ;

#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF( M4_CH_VM_TRACE_VM_L1, "Trace VM1: ReleaseBookMark" );
#endif


	iResult = RuntimePopDouble( vBookmark ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Bookmark" << "ReleaseBookMark" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vBookmark.Type ) << DumpCallStack() ) ;
    iBookMark = (m4int32_t)vBookmark.Data.DoubleData ;

	iResult = GetAccessContext( &poAccess ) ;

	if( iResult != M4_SUCCESS )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return M4_ERROR;
	}

	iResult = poAccess->GetpChannel()->ReleaseBookMark( iBookMark ) ;

	if( iResult != M4_SUCCESS )
	{
   		DUMP_CHLOG_ERRORF( M4_CH_VM_NO_RELEASE_BOOKMARK, iBookMark << poAccess->GetpCMCR()->iM4Obj() << LogCatAnd << poAccess->GetpCMCR()->nM4Obj() << DumpCallStack() );
	}

	RuntimePushDouble( M4_SUCCESS ) ;
	return M4_SUCCESS;
}




