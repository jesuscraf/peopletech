#include "vmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         clc
// File:           VMAttrib.cpp
// Project:        clc
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    Implementación de la clase VMAttribute que contiene las funciones de acceso a las propiedades
////
//==============================================================================

#include "vmattrib.hpp"

#include "access.hpp"
#include "item.hpp"
#include "val.hpp"
#include "channel.hpp"
#include "cpdefine.hpp" 
#include "vmachin1.h"
#include "totalize.hpp"
#include "regtot.hpp"
#include "m4objglb.hpp"
#include "cldbgutil.hpp"
#include "file_manager.hpp"
#include "replaceliterals.hpp"


//-------------------------------------------------------------------
//Inicialización  (uff)
//-------------------------------------------------------------------
void ClVMItemAttribute::_Init(void){
	
    m_apfRun[M4CL_ATT_MTHD_SYS_ADD_NEW_SLICE]	= &ClVMItemAttribute::SysAddNewSlice;
    m_apfRun[M4CL_ATT_MTHD_SYS_GET_ITEM_NAME]	= &ClVMItemAttribute::SysGetItemName;
    m_apfRun[M4CL_ATT_MTHD_SYS_CREATE_SLICE]	= &ClVMItemAttribute::SysCreateSlice;
    m_apfRun[M4CL_ATT_MTHD_SYS_SPLIT_SLICE]		= &ClVMItemAttribute::SysSplitSlice;
	m_apfRun[M4CL_ATT_MTHD_SYS_TOTALIZE_ITEMS]	= &ClVMItemAttribute::SysTotalizeItems;
	m_apfRun[M4CL_ATT_MTHD_SYS_TOTALIZE_SLICES]	= &ClVMItemAttribute::SysTotalizeSlices;
	m_apfRun[M4CL_ATT_MTHD_SYS_CREATE_BLOB_FILE]	= &ClVMItemAttribute::SysCreateBlobFile;
	m_apfRun[M4CL_ATT_MTHD_SYS_SET_BLOB_DESCRIP]	= &ClVMItemAttribute::SysSetBlobDescription;

	
    m_apfGet[M4CL_ATT_SYS_SLICE_NUMBER] = &ClVMItemAttribute::GetSysSliceNumber;
    m_apfGet[M4CL_ATT_SYS_START_DATE]   = &ClVMItemAttribute::GetSysStartDate;
    m_apfGet[M4CL_ATT_SYS_END_DATE]     = &ClVMItemAttribute::GetSysEndDate;
    m_apfGet[M4CL_ATT_SYS_FIRST_SLICE]  = &ClVMItemAttribute::GetSysFirstSlice;
    m_apfGet[M4CL_ATT_SYS_LAST_SLICE]   = &ClVMItemAttribute::GetSysLastSlice;
    m_apfGet[M4CL_ATT_SYS_OLD_VALUE]	= &ClVMItemAttribute::GetSysOldValue;
	m_apfGet[M4CL_ATT_SYS_BLOB_DESCRIP] = &ClVMItemAttribute::GetSysBlobDescription;
    m_apfGet[M4CL_ATT_SYS_BLOB_MASK]	= &ClVMItemAttribute::GetSysBlobMask;
    m_apfGet[M4CL_ATT_SYS_AUX_ITEM_ID]	= &ClVMItemAttribute::GetSysAuxItemId;

	
	
}
//-------------------------------------------------------------------
// Lectura de atributos de items
//-------------------------------------------------------------------


//--SlicesNumber
m4return_t ClVMItemAttribute::GetSysSliceNumber  (void){
	
    ClItem  *poItem;
    m4return_t ret;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetSysSliceNumber");
#endif
    ret=GetItem (poItem, M4_TRUE);
	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}
	//    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_WRONG_ITEM_READ, ERRORLOG, "Wrong item to read number of slices.")	;
    ret=RuntimePushDouble(poItem->Value.Count());

    return ret;
}

//---SliceStartDate
m4return_t ClVMItemAttribute::GetSysStartDate       (void){
	
    ClValue  *poValue;
	ClItem* poItem;
    m4return_t ret;
    m4bool_t    bRow, bSlice;
    //m4VariantType   ovTemp;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetSysStartDate");
#endif
    ret=GetItemValue (poValue, bRow, bSlice, poItem);
	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}

	//M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_WRONG_SLICE, ERRORLOG, "Wrong slice to read start date.")	;
	
    if (bSlice){
        ret=RuntimePushDate(poValue->GetStartDate());
    }
    else {
        //m_poAccess->ExecutionStartDate.Get(ovTemp);
        //ret=RuntimePushDate(ovTemp.Data.DoubleData );
		ret=RuntimePushDate(m_dBeginDate);
    }
	
    return ret;
}

//---SliceEndDate
m4return_t ClVMItemAttribute::GetSysEndDate         (void){
	
    ClValue  *poValue;
	ClItem* poItem;
    m4return_t ret;
    m4bool_t    bRow, bSlice;
    //m4VariantType   ovTemp;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetSysEndDate");
#endif
    ret=GetItemValue (poValue, bRow, bSlice, poItem);
	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}
//    M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_WRONG_SLICE, ERRORLOG, "Wrong slice to read end date (attribute).")	;
    if (bSlice){
        ret=RuntimePushDate(poValue->GetEndDate());
    }
    else{
      //  m_poAccess->ExecutionEndDate.Get(ovTemp);
      //  ret=RuntimePushDate(ovTemp.Data.DoubleData  );
		ret=RuntimePushDate(m_dEndDate);
    }
	
    return ret;
}

//---First Slice
m4return_t ClVMItemAttribute::GetSysFirstSlice   (void){
	
    m4return_t ret;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetSysFirstSlice");
#endif
    ret=RuntimePushDouble(0);
    return ret;
    
}

//---EndSlice
m4return_t ClVMItemAttribute::GetSysLastSlice    (void){
	
    ClItem  *poItem;
    m4return_t ret;
	m4uint32_t iCount;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetSysLastSlice");
#endif
    ret=GetItem (poItem, M4_TRUE);
	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}
//    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_WRONG_ITEM_READ, ERRORLOG, "Wrong item to read last slice.")	;
	
	iCount=poItem->Value.Count();
	if (iCount!=0){
		ret=RuntimePushDouble(iCount-1);
	}
	else{
		ret=RuntimePushDouble(0.0);
	}

    return ret;
}


//---OldValue
m4return_t ClVMItemAttribute::GetSysOldValue    (void){
	
    ClOldValue  *poValue;
    m4return_t ret;
    m4bool_t    bRow, bSlice;
    m4VariantType   vTemp;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetSysOldValue");
#endif
    ret=GetItemOldValue (poValue, bRow, bSlice);
	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}
//    M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_WRONG_ITEM_READ, ERRORLOG, "Wrong item and/or slice to read old value (attribute).")	;
    
    poValue->Get(vTemp);
    //M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "Error reading item old value." << DumpCallStack() )	;
    if (vTemp.Type==M4CL_CPP_TYPE_STRING_VAR){
		ret=RuntimePushString(vTemp.Data.PointerChar, vTemp.IsUser);
	}
	else{
		ret=RuntimePush(vTemp);
	}

    return ret;
}

//---------------------
//GetSysBlobMask
m4return_t ClVMItemAttribute::GetSysBlobDescription (void){
    ClItem  *poItem;
    m4return_t ret;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetSysBlobDescription");
#endif
    ret=GetItem (poItem, M4_TRUE);
	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	m4pchar_t pszDesc=poItem->Value.GetDescr();

	if (pszDesc){
		RuntimePushString(pszDesc);
	}
	else{
		RuntimePushString("");
	}
	

    return M4_SUCCESS;

}

m4return_t ClVMItemAttribute::GetSysBlobMask	     (void){
   ClItem  *poItem;
    m4return_t ret;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetSysBlobMask");
#endif
    ret=GetItem (poItem, M4_TRUE);
	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	m4uint32_t uiMask=0;

	poItem->Value.ChangedMask(uiMask);
	RuntimePushDouble(uiMask);


    return M4_SUCCESS;

}

//--------------------------------------------------------------------
//AuxiliarItemId
//--------------------------------------------------------------------
m4return_t ClVMItemAttribute::GetSysAuxItemId(void){
	    ClItem  *poItem;
    m4return_t ret;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetSysAuxItemId");
#endif
    ret=GetItem (poItem, M4_TRUE);
	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	m4pcchar_t pszAuxId=poItem->ItemDef.AuxiliarItem();

	if (pszAuxId){
		RuntimePushString(pszAuxId);
	}
	else{
		RuntimePushString("");
	}
	

    return M4_SUCCESS;

}

//--------------------------------------------------------------------
//Atributos ejecutables
//--------------------------------------------------------------------


//---AddNewSlice
m4return_t ClVMItemAttribute::SysAddNewSlice     (void){
	
    ClItem *oItem;
    m4VariantType vDate1,vDate2;
    m4return_t ret;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SysAddNewSlice");
#endif
    //fecha final (param2)
    ret=RuntimePop(vDate2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SysAddNewSlice.")	;
    ret=m_oVariantMediator->ToDate(vDate2);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "date2" << "SysAddNewSlice" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDate2.Type)<<DumpCallStack() );    
    //fecha inicial (param1)
    ret=RuntimePop(vDate1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SysAddNewSlice.")	;
    ret=m_oVariantMediator->ToDate(vDate1);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "date1" << "SysAddNewSlice" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDate1.Type)<<DumpCallStack() );
	
	
    ret=GetItem (oItem, M4_TRUE);
	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}
	//comprobamos que FI<=FF
	if (vDate1.Data.DoubleData> vDate2.Data.DoubleData){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_DATES_ADDING_SLICE, M4ObjNodeItemI(oItem->GetpAccess()->GetpCMCR(),oItem->ItemDef.GetHandle() )<<DateLog(vDate1.Data.DoubleData)<<DateLog(vDate2.Data.DoubleData)<<"SysAddNewSlice" << DumpCallStack() );
		return M4_ERROR;
	}
	//comprobación de +/- inf
	if (vDate1.Data.DoubleData<M4CL_MINUS_INFINITE_IN_JULIAN){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_DATE_OVERFLOW, M4CL_MINUS_INFINITE_DATE<<"SysAddNewSlice");
		vDate1.Data.DoubleData=M4CL_MINUS_INFINITE_IN_JULIAN;
	}
	if (vDate2.Data.DoubleData>M4CL_PLUS_INFINITE_IN_JULIAN){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_DATE_OVERFLOW, M4CL_PLUS_INFINITE_DATE<<"SysAddNewSlice");
		vDate2.Data.DoubleData=M4CL_PLUS_INFINITE_IN_JULIAN;
	}

//    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_WRONG_ITEM_READ, ERRORLOG, "Error getting item to add a new slice.")	;
    ret=oItem->Value.AddSlice (vDate1.Data.DoubleData, vDate2.Data.DoubleData);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_ADDING_SLICE, ERRORLOG, M4ObjNodeItemI(oItem->GetpAccess()->GetpCMCR(),oItem->ItemDef.GetHandle() )<<BeginLogCat<<DateLog(vDate1.Data.DoubleData)<<"-"<<DateLog(vDate2.Data.DoubleData)<<EndLogCat<<"SysAddNewSlice"  << DumpCallStack() );
	
    //resultado
    ret=RuntimePushDouble (ret);
	
    return M4_SUCCESS;
}

//---GetitemName
m4return_t ClVMItemAttribute::SysGetItemName        (void){
	
    m4return_t		iResult = M4_ERROR;
	m4bool_t		bModified = M4_FALSE;
    m4pcchar_t		pcString = NULL;
	m4pchar_t		pcOut = NULL;
	m4pchar_t		pcOrganization = NULL;
	ClChannel		*poChannel = NULL;
	m4VariantType	vOrganization;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SysGetItemName");
#endif

    //borramos el argumento por referencia que se nos pasa
    iResult = RuntimePopAndDelete();
    DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SysGetItemName" );
    
    RuntimePushDouble( iResult );

	poChannel = m_poAccess->GetpChannel();
	pcString = poChannel->GetpCMCR()->GetItemName( m_hItem );

	if( poChannel->Organization.Get( vOrganization ) == M4_SUCCESS && vOrganization.Type == M4CL_CPP_TYPE_STRING_VAR )
	{
		pcOrganization = vOrganization.Data.PointerChar;
	}

	iResult = M4ReplaceLiterals( poChannel->GetpChannelManager(), pcString, pcOut, pcOrganization, bModified );

	if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcOut != NULL )
	{
		RuntimePushString( pcOut );
		delete pcOut;
	}
	else
	{
		RuntimePushString( pcString );
	}
    
    return M4_SUCCESS;
}

//---CreateSlice
m4return_t  ClVMItemAttribute::SysCreateSlice       (void){
	
    ClItem *oItem;
    m4VariantType vDate1,vDate2,vValue;
    m4return_t ret;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SysCreateSlice");
#endif
    //sacamos el valor
    ret=RuntimePop(vValue);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SysCreateSlice.")	;
	
    //fecha final (param2)
    ret=RuntimePop(vDate2);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SysCreateSlice.")	;
    ret=m_oVariantMediator->ToDate(vDate2);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "date2" << "SysCreateSlice" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDate2.Type)<<DumpCallStack() );
    
    //fecha inicial (param1)
    ret=RuntimePop(vDate1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SysCreateSlice.")	;
    ret=m_oVariantMediator->ToDate(vDate1);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "date1" << "SysCreateSlice" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDate1.Type)<<DumpCallStack() );
	
	
    ret=GetItem (oItem, M4_TRUE);
	if (ret!=M4_SUCCESS){
		DeleteVarString (vValue);
		return M4_ERROR;
	}
//    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_WRONG_ITEM_READ, ERRORLOG, "Error getting item to create a new slice.")	;

	//comprobamos que FI<=FF
	if (vDate1.Data.DoubleData> vDate2.Data.DoubleData){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_DATES_ADDING_SLICE, M4ObjNodeItemI(oItem->GetpAccess()->GetpCMCR(),oItem->ItemDef.GetHandle() )<<DateLog(vDate1.Data.DoubleData)<<DateLog(vDate2.Data.DoubleData)<<"SysCreateSlice" << DumpCallStack() );
		DeleteVarString (vValue);
		return M4_ERROR;
	}
	//comprobación +/- inf
	if (vDate1.Data.DoubleData<M4CL_MINUS_INFINITE_IN_JULIAN){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_DATE_OVERFLOW, M4CL_MINUS_INFINITE_DATE<<"SysCreateSlice");
		vDate1.Data.DoubleData=M4CL_MINUS_INFINITE_IN_JULIAN;
	}
	if (vDate2.Data.DoubleData>M4CL_PLUS_INFINITE_IN_JULIAN){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_DATE_OVERFLOW, M4CL_PLUS_INFINITE_DATE<<"SysCreateSlice");
		vDate2.Data.DoubleData=M4CL_PLUS_INFINITE_IN_JULIAN;
	}

    //creamos el slice
    ret=oItem->Value.AddSlice (vDate1.Data.DoubleData, vDate2.Data.DoubleData);
    if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_ADDING_SLICE,  M4ObjNodeItemI(oItem->GetpAccess()->GetpCMCR(),oItem->ItemDef.GetHandle() )<<BeginLogCat<<DateLog(vDate1.Data.DoubleData)<<"-"<<DateLog(vDate2.Data.DoubleData)<<EndLogCat<<"SysCreateSlice"  << DumpCallStack() );
		DeleteVarString (vValue);
		return M4_ERROR;
	}
	
    //metemos el valor
//    ret=oItem->Value.Set (vValue);
    if (vValue.Type==M4CL_CPP_TYPE_STRING_POOL){
        m4VariantType copy;
        copy.IsUser=vValue.IsUser;
        copy.Type=M4CL_CPP_TYPE_STRING_VAR;
        copy.Data.PointerChar=m_oDynamicPool->GetString (vValue.Data.CharData);
		ret = oItem->Value.Set (copy);

		if( ret != M4_ERROR && (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_VALUES) )
		{
			m4VMState_t oState;
			m_oStateStack->Top(oState);
			GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddValue( *oItem, copy, oItem->Value.GetPriority(), M4_TRUE, 3, oState, DumpCallStack( &oState, M4_TRUE ).c_str() ) ;
		}
    }
    else{
		ret = oItem->Value.Set (vValue);

		if( ret != M4_ERROR && (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_VALUES) )
		{
			m4VMState_t oState;
			m_oStateStack->Top(oState);
			GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddValue( *oItem, vValue, oItem->Value.GetPriority(), M4_TRUE, 3, oState, DumpCallStack( &oState, M4_TRUE ).c_str() ) ;
		}
	}

    if (ret==M4_ERROR){
		DUMP_CHLOG_ERRORF ( M4_CH_VM_WRITING_SLICE_ATT, M4ObjNodeItemI(oItem->GetpAccess()->GetpCMCR(),oItem->ItemDef.GetHandle()) <<DateLog (vDate1.Data.DoubleData)<<DateLog(vDate2.Data.DoubleData) <<"SysCreateSlice" << DumpCallStack() );
		DeleteVarString (vValue);
		return M4_ERROR;
	}
	
    //resultado
    ret=RuntimePushDouble (ret);
	DeleteVarString (vValue);

    return M4_SUCCESS;
}

//---SplitSlice
m4return_t ClVMItemAttribute::SysSplitSlice        (void){
    ClItem *oItem;
    m4VariantType vDate1;
    m4return_t ret;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SysSplitSlice");
#endif
    
    //fecha inicial (param1)
    ret=RuntimePop(vDate1);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SysSplitSlice.")	;
    ret=m_oVariantMediator->ToDate(vDate1);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "date1" << "SysAddNewSlice" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDate1.Type)<<DumpCallStack() );
	
	
    ret=GetItem (oItem, M4_TRUE);
	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}
//    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_WRONG_ITEM_READ, ERRORLOG, "Error getting item to split a new slice.")	;
    oItem->Value[vDate1.Data.DoubleData];
	ret=oItem->Value.SplitSlice (vDate1.Data.DoubleData);
    if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_SPLITING_SLICE, M4ObjNodeItemI(oItem->GetpAccess()->GetpCMCR(),oItem->ItemDef.GetHandle() )<<DateLog(vDate1.Data.DoubleData)<<"SysSplitSlice" << DumpCallStack() );
    }
	 
    //resultado
    ret=RuntimePushDouble (ret);
	
    return M4_SUCCESS;
	
}

//---Totalize item
m4return_t ClVMItemAttribute::SysTotalizeItems        (void)
{
	m4return_t ret=M4_SUCCESS;
	m4VariantType vTypeTot, vValueTot;
	ClItem *poItem;
	
	ret=RuntimePop(vTypeTot);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SysTotalizeItems.")	;
    
	/* Bug 0259171
	El argumento puede no ser número
	*/
	if (vTypeTot.Type!=M4CL_CPP_TYPE_NUMBER){
        ret=m_oVariantMediator->ToDouble (vTypeTot);
    }

	ret=GetItem (poItem, M4_FALSE);
	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}
//    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_WRONG_ITEM_READ, ERRORLOG, "Error getting item to totalize.")	;
	
	ret=poItem->GetpNodeAccess()->RecordSet.GetFooter()[ ClItemIndex(poItem->ItemDef.Index()) ].Get(vValueTot, (m4uint8_t)vTypeTot.Data.DoubleData);
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_TOT_ITEM, ERRORLOG, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR() , poItem->ItemDef.GetHandle() )  << DumpCallStack() );
	
	if ( (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_READS) )
	{
		m4VMState_t oState;
		m_oStateStack->Top(oState);
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddRead( *poItem, vValueTot, 0.0, 0.0, oState, DumpCallStack( &oState, M4_TRUE ).c_str() );
	}

	if (vValueTot.Type==M4CL_CPP_TYPE_STRING_VAR){
		RuntimePushString (vValueTot.Data.PointerChar, vValueTot.IsUser);
	}
	else{
		RuntimePush (vValueTot);
	}
	return M4_SUCCESS;
}

//---Totalize Slice
m4return_t ClVMItemAttribute::SysTotalizeSlices        (void)
{
	
	m4return_t ret=M4_SUCCESS;
	ClValue *oValue;
	ClItem* poItem;
	m4bool_t bRow,bSlice;
	m4VariantType vTypeTot, vValueTot;
	
	ret=RuntimePop(vTypeTot);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SysTotalizeSlices.")	;

	/* Bug 0259171
	El argumento puede no ser número
	*/
	if (vTypeTot.Type!=M4CL_CPP_TYPE_NUMBER){
        ret=m_oVariantMediator->ToDouble (vTypeTot);
    }
	
	ret=GetItemValue (oValue,bRow,bSlice, poItem);
	if (ret!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	/* Bug 0146789
	El casting debe ser a entero con signo porque sino en AIX lo convierte a 0
	*/
	ret=oValue->GetpSliceFooter().Get(vValueTot, (m4int8_t)vTypeTot.Data.DoubleData);
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_TOT_ITEM, ERRORLOG, M4ObjNodeItemI(oValue->GetpRegister()->GetpAccess()->GetpCMCR() , oValue->_GetpItem_Interface()->ItemDef.GetHandle() )  << DumpCallStack() );

	if ( (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_READS) )
	{
		m4VMState_t oState;
		m_oStateStack->Top(oState);
		GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddRead( *poItem, vValueTot, 0.0, 0.0, oState, DumpCallStack( &oState, M4_TRUE ).c_str() );
	}
	
	if (vValueTot.Type==M4CL_CPP_TYPE_STRING_VAR){
		RuntimePushString (vValueTot.Data.PointerChar, vValueTot.IsUser);
	}
	else{
		RuntimePush (vValueTot);
	}
	return M4_SUCCESS;
}

m4return_t ClVMItemAttribute::SysCreateBlobFile(void){

    ClItem *oItem;
    m4VariantType vExt,vDir;
    m4return_t ret;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SysCreateBlobFile");
#endif
    //Extension(param2)
    ret=RuntimePop(vExt);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SysCreateBlobFile")	;
    ret=m_oVariantMediator->ToPoolString(vExt);
  	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Extension" << "SysCreateBlobFile" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vExt.Type)<<DumpCallStack() );
    
    //Directorio (param1)
    ret=RuntimePop(vDir);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SysCreateBlobFile")	;
    ret=m_oVariantMediator->ToPoolString(vDir);
	if (ret!=M4_SUCCESS){
		DeleteVarString (vExt);
		DUMP_CHLOG_ERRORF (M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Directory"<<"SysCreateBlobFile" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vDir.Type)<<DumpCallStack() );
		return M4_ERROR;
	}
	
    ret=GetItem (oItem, M4_TRUE);
	if (ret!=M4_SUCCESS){
		DeleteVarString (vDir);
		DeleteVarString (vExt);
		return M4_ERROR;
	}
	
	const m4char_t *pcDir=m_oDynamicPool->GetString(vDir.Data.CharData);
	if (!pcDir || pcDir[0]=='\0'){ //ES cadena vacía
		pcDir=ClFileManager::Instance()->GetDefaultDirectory();
	}
	ret=oItem->Value.CreateTempFile ((char*)pcDir, m_oDynamicPool->GetString(vExt.Data.CharData) );
	if (ret!=M4_SUCCESS){
		DeleteVarString (vExt);
		DUMP_CHLOG_ERRORF (M4_CH_VM_CANT_CREATE_BLOB_FILE,  M4ObjNodeItemI(oItem->GetpAccess()->GetpCMCR() , oItem->ItemDef.GetHandle() ) << DumpCallStack() );
		return M4_ERROR;
	}
	
	//resultado
    RuntimePushDouble (ret);
	
	DeleteVarString (vDir);
	DeleteVarString (vExt);

    return M4_SUCCESS;
}

m4return_t ClVMItemAttribute::SysSetBlobDescription	(void){
    ClItem *oItem;
    m4VariantType vDesc,vDir;
    m4return_t ret;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SysSetBlobDescription");
#endif
    //Extension(param1)
    ret=RuntimePop(vDesc);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SysSetBlobDescription")	;
    ret=m_oVariantMediator->ToPoolString(vDesc);
  	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Description" << "SysSetBlobDescription" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vDesc.Type)<<DumpCallStack() );
    
    
    ret=GetItem (oItem, M4_TRUE);
	if (ret!=M4_SUCCESS){
		DeleteVarString (vDesc);
		return M4_ERROR;
	}
	
	
	ret=oItem->Value.SetDescr(m_oDynamicPool->GetString(vDesc.Data.CharData) );
	if (ret!=M4_SUCCESS){
		DeleteVarString (vDesc);
		DUMP_CHLOG_ERRORF (M4_CH_VM_CANT_SET_BLOB_DESC,  M4ObjNodeItemI(oItem->GetpAccess()->GetpCMCR() , oItem->ItemDef.GetHandle() ) <<m_oDynamicPool->GetString(vDesc.Data.CharData)<< DumpCallStack() );
		return M4_ERROR;
	}

	
	//resultado
    RuntimePushDouble (ret);
	
	DeleteVarString (vDesc);
    return M4_SUCCESS;

}



//---------------------------------------------------------------------
//Funciones auxiliares
//---------------------------------------------------------------------

m4return_t ClVMItemAttribute::GetItem(ClItem* &ao_poItem, m4bool_t ai_bPositioned){
	
    m4uint32_t	iRow;
	m4VariantType vTemp;
	m4return_t	ret;
	
	//miramos si viene con tramo
	if ( M4CL_HAS_TRAME_FLAG (m_iFlags ) ){
		
		ret=RuntimePop(vTemp);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Attribute::GetItem.")	;
        //lo sacamos y pasamos, porque no nos importa		
	}
	
    //miramos si viene con row
    _ClRegister_Base *pr;
	
    if ( M4CL_HAS_ROW_FLAG (m_iFlags) ){
		ret=RuntimePopNumber (vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Attribute::GetItem, getting record number for attribue of item")	;
		
		iRow=int (vTemp.Data.DoubleData);
        
        if (!m_poAccess->Node[ClHandle(m_hNodo)].RecordSet.IsValid() && m_poAccess->GetpCMCR()->GetItemScope(m_hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_READING_ATTRIBUTE)<<M4ObjNodeItemI(m_poAccess->GetpCMCR(),m_hItem)<<DumpTreePath(m_poAccess, m_hNodo)<<DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//no movemos el nodo porque ya esta movido
		pr=&(m_poAccess->Node->RecordSet.Register[iRow]);		
	}
    else{
        if (!m_poAccess->Node[ClHandle(m_hNodo)].RecordSet.IsValid() && m_poAccess->GetpCMCR()->GetItemScope(m_hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_READING_ATTRIBUTE)<<M4ObjNodeItemI(m_poAccess->GetpCMCR(),m_hItem)<<DumpTreePath(m_poAccess, m_hNodo)<<DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//no movemos el nodo porque ya esta movido
        pr=&(m_poAccess->Node->RecordSet.Current);
    }
	
    //comprobamso que podamos leer, si es que debemos estar posiscionados
	if (ai_bPositioned ){
		if (pr->Index == M4DM_EOF_INDEX && m_poAccess->GetpCMCR()->GetItemScope(m_hItem) == M4CL_ITEM_SCOPE_REGISTER ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_READING_ATTRIBUTE, M4ObjNodeItemI(m_poAccess->GetpCMCR(),m_hItem) << DumpCallStack() );
			return M4_ERROR;
		}
    }

	ao_poItem=&( pr->Item[ClHandle(m_hItem)] );
	
    if (ao_poItem->GetIndex() == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_ITEM_READING_ATTRIBUTE, m_poAccess->GetpCMCR()->iItemI(m_hItem)<<LogCatAnd<<m_poAccess->GetpCMCR()->nItemI(m_hItem) << DumpCallStack() );
        return M4_ERROR;
    }
    
    return M4_SUCCESS;
}



m4return_t ClVMItemAttribute::GetItemValue(ClValue* &ao_poValue, m4bool_t &ao_bRow, m4bool_t &ao_bSlice, ClItem* &ao_rpoItem){
    
    m4uint32_t	iRow;
	m4double_t	dTrame;
	m4uint8_t   iSliceFlag;
	ClSliceIndex	SliceIndex;
	m4VariantType vTemp;
	m4return_t	ret=M4_SUCCESS;;
    m4bool_t    bRet;
	
	
	const m4uint8_t NoSliceWithSlices=0;
    const m4uint8_t NoSliceWithoutSlices=1;
    const m4uint8_t SliceByDate=2;
    const m4uint8_t SliceByOrder=3;
    
	
	ao_rpoItem = NULL;

    //obtenemos el valor 
    if (m_bRunSliceMode){
        iSliceFlag=NoSliceWithSlices;
		ao_bSlice=M4_TRUE;
    }
    else{
        iSliceFlag=NoSliceWithoutSlices;
		ao_bSlice=M4_FALSE;
    }
    
	//miramos si viene con tramo
	if ( M4CL_HAS_TRAME_FLAG (m_iFlags ) ){
		
		ret=RuntimePop(vTemp);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Attribute::GetItemValue.")	;
        
        ao_bSlice=M4_TRUE;
		
        if (vTemp.Type==M4CL_CPP_TYPE_NUMBER){ //por número de tramo
			SliceIndex=m4uint32_t(vTemp.Data.DoubleData);
            
            iSliceFlag=SliceByOrder;
			
		}
		else if (vTemp.Type==M4CL_CPP_TYPE_DATE){ //por fecha
			dTrame=vTemp.Data.DoubleData;
			iSliceFlag=SliceByDate;
		}
		else{
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE_FORMAT_ATT, M4ObjNodeItemI(m_poAccess->GetpCMCR(), m_hItem) << DumpCallStack() );
			return M4_ERROR;
		}
	}
    //else{
        //ao_bSlice=M4_FALSE;
    //}
	
    //miramos si viene con row
    _ClRegister_Base *pr;
	
    if ( M4CL_HAS_ROW_FLAG (m_iFlags ) ){
		ret=RuntimePopNumber (vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Attribute::GetItemValue, getting record number.")	;
		
		iRow=int (vTemp.Data.DoubleData);
        
        if (!m_poAccess->Node[ClHandle(m_hNodo)].RecordSet.IsValid() && m_poAccess->GetpCMCR()->GetItemScope(m_hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_READING_ATTRIBUTE)<<M4ObjNodeItemI(m_poAccess->GetpCMCR(),m_hItem)<<DumpTreePath(m_poAccess, m_hNodo)<<DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//no movemos el nodo porque ya esta movido
		pr=&(m_poAccess->Node->RecordSet.Register[iRow]);		
		
        ao_bRow=M4_TRUE;
	}
    else{
        
        if (!m_poAccess->Node[ClHandle(m_hNodo)].RecordSet.IsValid() && m_poAccess->GetpCMCR()->GetItemScope(m_hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_READING_ATTRIBUTE)<<M4ObjNodeItemI(m_poAccess->GetpCMCR(),m_hItem)<<DumpTreePath(m_poAccess, m_hNodo)<<DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//no movemos el nodo porque ya esta movido
        pr=&(m_poAccess->Node->RecordSet.Current);
        ao_bRow=M4_FALSE;
    }
	
	
    //comprobamso que podamos leer
    if (pr->Index == M4DM_EOF_INDEX && m_poAccess->GetpCMCR()->GetItemScope(m_hItem) == M4CL_ITEM_SCOPE_REGISTER ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_READING_ATTRIBUTE, M4ObjNodeItemI(m_poAccess->GetpCMCR(),m_hItem)<< DumpCallStack() );
        return M4_ERROR;
    }

	ao_rpoItem = &(pr->Item[ClHandle(m_hItem)]);
    
    switch(iSliceFlag){
    case NoSliceWithoutSlices:
        {
            ao_poValue=&(ao_rpoItem->Value);
            break;
        }
    case NoSliceWithSlices:
        {
            ao_poValue=&(ao_rpoItem->Value);
            bRet=ao_poValue->MoveTo(m_dBeginDate, m_dEndDate);
            if (!bRet)
                ret=M4_WARNING;
            
            break;
        }
        
    case SliceByDate:
        {
            ao_poValue = &(ao_rpoItem->Value[dTrame]);
            break;
        }
    case SliceByOrder:
        {
            ao_poValue=&(ao_rpoItem->Value[SliceIndex] );
            break;
        }
    }
    
    if (ao_poValue->Index == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE_READING_ATTRIBUTE, m_poAccess->GetpCMCR()->iItemI(m_hItem)<<LogCatAnd<<m_poAccess->GetpCMCR()->nItemI(m_hItem) << DumpCallStack() );
        return M4_ERROR;
    }
	return ret;
}



m4return_t ClVMItemAttribute::GetItemOldValue(ClOldValue* &ao_poValue, m4bool_t &ao_bRow, m4bool_t &ao_bSlice ){
    
    m4uint32_t	iRow;
	m4double_t	dTrame;
	m4uint8_t   iSliceFlag;
	ClSliceIndex	SliceIndex;
	m4VariantType vTemp;
	m4return_t	ret=M4_SUCCESS;
    m4bool_t    bRet;
	
	
	const m4uint8_t NoSliceWithSlices=0;
    const m4uint8_t NoSliceWithoutSlices=1;
    const m4uint8_t SliceByDate=2;
    const m4uint8_t SliceByOrder=3;
    
	
	
	
    //obtenemos el valor 
	
    if (m_bRunSliceMode){
        iSliceFlag=NoSliceWithSlices;
		ao_bSlice=M4_TRUE;
    }
    else{
        iSliceFlag=NoSliceWithoutSlices;
		ao_bSlice=M4_FALSE;
    }
	
	//miramos si viene con tramo
	if ( M4CL_HAS_TRAME_FLAG (m_iFlags ) ){
		
		ret=RuntimePop(vTemp);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Attribute::GetItemOldValue.")	;
        
        ao_bSlice=M4_TRUE;
		
        if (vTemp.Type==M4CL_CPP_TYPE_NUMBER){ //por número de tramo
			SliceIndex=m4uint32_t(vTemp.Data.DoubleData);
            
            iSliceFlag=SliceByOrder;
			
		}
		else if (vTemp.Type==M4CL_CPP_TYPE_DATE){ //por fecha
			dTrame=vTemp.Data.DoubleData;
			iSliceFlag=SliceByDate;
		}
		else{
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE_FORMAT_ATT, M4ObjNodeItemI(m_poAccess->GetpCMCR(), m_hItem) << DumpCallStack() );
			return M4_ERROR;
		}
	}
    //else{
      //  ao_bSlice=M4_FALSE;
    //}
	
    //miramos si viene con row
    _ClRegister_Base *pr;
	
    if ( M4CL_HAS_ROW_FLAG (m_iFlags ) ){
		ret=RuntimePopNumber (vTemp);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Attribute::GetItemOldValue, getting record number.")	;
		
		iRow=int (vTemp.Data.DoubleData);
        
        if (!m_poAccess->Node[ClHandle(m_hNodo)].RecordSet.IsValid() && m_poAccess->GetpCMCR()->GetItemScope(m_hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_READING_ATTRIBUTE)<<M4ObjNodeItemI(m_poAccess->GetpCMCR(),m_hItem)<<DumpTreePath(m_poAccess, m_hNodo)<<DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//no movemos el nodo porque ya esta movido
		pr=&(m_poAccess->Node->RecordSet.Register[iRow]);		
		
        ao_bRow=M4_TRUE;
	}
    else{
		
        if (!m_poAccess->Node[ClHandle(m_hNodo)].RecordSet.IsValid() && m_poAccess->GetpCMCR()->GetItemScope(m_hItem) != M4CL_ITEM_SCOPE_NODE){
			g_oChLog<<BeginError(M4_CH_VM_WRONG_BLOCK_READING_ATTRIBUTE)<<M4ObjNodeItemI(m_poAccess->GetpCMCR(),m_hItem)<<DumpTreePath(m_poAccess, m_hNodo)<<DumpCallStack()<<EndLog;

            return M4_ERROR;
        }
		//no movemos el nodo porque ya esta movido
        pr=&(m_poAccess->Node->RecordSet.Current);
        ao_bRow=M4_FALSE;
    }
	
	
    //comprobamso que podamos leer
    if (pr->Index == M4DM_EOF_INDEX && m_poAccess->GetpCMCR()->GetItemScope(m_hItem) == M4CL_ITEM_SCOPE_REGISTER ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_READING_ATTRIBUTE, M4ObjNodeItemI(m_poAccess->GetpCMCR(),m_hItem) << DumpCallStack() );
        return M4_ERROR;
    }
    
    
    
    switch(iSliceFlag){
    case NoSliceWithoutSlices:
        {
            ao_poValue=&(pr->Item[ClHandle(m_hItem)].OldValue);
            break;
        }
    case NoSliceWithSlices:
        {
            
            ao_poValue=&(pr->Item[ClHandle(m_hItem)].OldValue);
            bRet=ao_poValue->MoveTo(m_dBeginDate, m_dEndDate);
            if (!bRet)
                ret=M4_WARNING;
            break;
        }
        
    case SliceByDate:
        {
            ao_poValue = &(pr->Item[ClHandle(m_hItem)].OldValue[dTrame]);
            break;
        }
    case SliceByOrder:
        {
            ao_poValue=&(pr->Item[ClHandle(m_hItem)].OldValue[SliceIndex] );
            break;
        }
    }
    
    
    if (ao_poValue->Index == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SLICE_READING_ATTRIBUTE, m_poAccess->GetpCMCR()->iItemI(m_hItem)<<LogCatAnd<<m_poAccess->GetpCMCR()->nItemI(m_hItem) << DumpCallStack() );
        return M4_ERROR;
    }
	return ret;
}
