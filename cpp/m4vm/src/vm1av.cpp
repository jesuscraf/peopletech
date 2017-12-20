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
//    Virtual machine de nivel 1- Funciones de aplica valor 
////
//==============================================================================

#include "vm1payroll.hpp"

#include "vmres.hpp"
#include "vm1pr.hpp"
#include "sttlib.hpp"
#include "cltotacum.hpp"
#include "vmachin1.h"

#include "chlog.hpp"
#include "access.hpp"
#include "handles.hpp"
#include "channel.hpp"
#include "m4objglb.hpp"
#include "cldbgutil.hpp"
#include "nodedef.hpp"


//--------------------------
//destructor
//--------------------------
ClVM1PayrollLib::~ClVM1PayrollLib( void ){

	
	if(m_poPayRoll!=NULL){
		delete m_poPayRoll;
		m_poPayRoll=NULL;
	}
	
}
//-----------------------------------------------------
//C R E A T E   O R G A N I G R A M 
//-----------------------------------------------------
m4return_t ClVM1PayrollLib::prCreateOrganigram( void )
{
    m4return_t ret;
	
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1,"Trace VM1: CreateOrganigram ");
#endif
	
    if(m_poPayRoll==NULL)
	{
        m_poPayRoll=new ClVMPayRoll;
    }
    
//    m_poPayRoll->SetVM(this);
    m_poPayRoll->SetDatesStack(&m_oDatesStack);
    m_poPayRoll->SetAccess(m_oState1.m_poAccess);
    m_poPayRoll->SetWorkingDates(m_oState1.m_dBeginDate, m_oState1.m_dEndDate, m_oState1.m_dDate);
	
    ret = m_poPayRoll->_CreateOrganigram( m_oState1.m_poRecordSet );
	
    RuntimePushDouble(ret); //metemos el resultado (exito o error)
    
    return M4_SUCCESS;  //retornamos éxito
}

//----------------------------------------------------------------
// A P L I C A   V A L O R -- level 2
//----------------------------------------------------------------
// pedazo de aplicavalor 2


m4return_t  ClVM1PayrollLib :: prTransferApplyValue( void )
{
	
    m4return_t          iResult ;
	
    m4bool_t            bDone ;
    m4uint8_t           iDump ;
    //m4uint8_t           iSliceModeOn ;
    m4uint8_t           iSameChannel ;
    
    m4uint16_t          k ;
    m4uint16_t          iLength ;
    m4int32_t           iSlices ,js; 
	m4int16_t			iArgumentNumber ;
	
    m4uint32_t          i, m ;
    m4uint32_t          iStart ;
    
    m4double_t          dDivider ;
    m4double_t          dCutRate ;
	
    m4date_t            dStart ;
    m4date_t            dEnd ;
    m4date_t            dSliceDate ;
    m4date_t            dTimeUnit ;
    m4date_t            dExchangeDateArg ;
    m4date_t            dExchangeDate ;

    m4pcchar_t          pccValue ;
	m4pchar_t			pccExchangeType ;
	
    m4char_t            acTempString[ M4CL_MCR_MAX_TEMP_STRING + 1 ] ;
    m4char_t            acLastDmdComponent[ M4CL_MAX_DMD_COMPONENT_ID + 1 ] ;
	m4char_t			acHostDmdComponent[ M4CL_MAX_DMD_COMPONENT_ID + 1 ] ;
    m4char_t            acHostChannelArg[ M4CL_MAX_T3_ID + 1 ] ;
    m4char_t            acHostNodeArg[ M4CL_MAX_NODE_ID + 1 ] ;
    m4char_t            acTargetDmdArg[ M4CL_MAX_DMD_ID + 1 ] ;
    m4char_t            acDestCurrencyArg[ M4CL_MAX_CURRENCY_ID + 1 ] ;
    m4char_t            acExchangeTypeArg[ M4CL_MAX_EXCHANGE_TYPE_ID + 1 ] ;
	
    m4VariantType       vValue, vAux ;
    m4VariantType       vSrcCurrency ;
	
	ClAccess			*poCurrencyAccess ;
	m4bool_t            bExchangeFound ;
	
	// ==========================================================
	// variables del host =======================================
	// ==========================================================
	
    m4uint8_t           iHostType ;
    m4uint8_t           iHostM4Type ;
    m4uint8_t           iHostRealM4Type ;
    m4uint8_t           iHostPriority ;
    m4int16_t           iHostAuxItemIndex ;
    m4uint16_t          iHostItems ;
    m4uint16_t          iHostComponentIndex ;
    m4uint16_t          iHostStartDateIndex ;
    m4uint16_t          iHostEndDateIndex ;
    m4uint16_t          iHostPriorityIndex ;
    m4uint16_t          iHostSrcItemIndex ;
    m4uint32_t          iHostRegisters ;
    m4uint32_t          iHostHandle ;
    m4uint32_t          iHostComponentHandle ;
    m4uint32_t          iHostStartDateHandle ;
    m4uint32_t          iHostEndDateHandle ;
    m4uint32_t          iHostPriorityHandle ;
    m4uint32_t          iHostItemHandle ;
    m4uint32_t          iHostAuxItemHandle ;
    m4uint32_t          iHostSrcItemHandle ;
	m4uint32_t          iHostStartDateFilterItemHandle ;
    m4uint32_t          iHostEndDateFilterItemHandle ;
    m4pcchar_t          pccHostDmdComponent ;
    m4pcchar_t          pccHostItemId ;
    m4pcchar_t          pccHostInstance ;
	
    ClCompiledMCR       *poHostCMCR ;
    ClNode              *poDHostNode ;
    ClRecordSet         *poDHostRecordSet;
    ClAccess            *poHostAccess ;
	
	m4date_t			dPhase1End;
	m4date_t			dPhase1Start;
	m4date_t			dPhase1Run;
	m4bool_t			bPhase1Slice;
	// ==========================================================
	// variables del target =====================================
	// ==========================================================
	
    m4uint8_t           iTargetSliceBhrv ;
    m4uint16_t          iTargetItems ;
    m4uint32_t          hTargetNode ;
    m4uint32_t          iTargetItemHandle ;
    m4pcchar_t          pccTargetChannel ;
	
	ClItemIndex			iTargetItemIndex ;
    ClCompiledMCR       *poTargetCMCR ;
    ClRegister_Current  *poTargetCurrentRegister ;
    ClChannel           *poDTargetChannel ;
    ClAccess            *poTargetAccess ;
	ClPriorSlice		oPriorSlice ;

	priority_t			iPriority ;
	m4bool_t			bAssigned ;
	
	// ==========================================================
	// inicializacion ===========================================
	// ==========================================================

    
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1,"Trace VM1: ApplyValue (2)");
#endif
	
	// ==========================================================
	// argumentos de la pila ====================================
	// ==========================================================
	
	// Número de argumentos
	
    iResult = RuntimePopDouble( vValue ) ;
    DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "ApplyValue (2)" ) ;
	
    iArgumentNumber = m4int16_t( vValue.Data.DoubleData ) ;
	
    if( iArgumentNumber < 2 || iArgumentNumber > 6 )
    {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "ApplyValue (2)"<<"2-6" << DumpCallStack() );
        return( M4_ERROR ) ;
    }
	
	//Exchange Type (argumento 6)
	
	*acExchangeTypeArg = '\0' ; 

    if( iArgumentNumber > 5 )
    {
        iResult = RuntimePopString( vValue ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Exchange_type" << "ApplyValue (2)" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
		
		pccValue = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
		
		iLength = strlen( pccValue ) ;
		
		if( iLength > M4CL_MAX_EXCHANGE_TYPE_ID )
		{
			iLength = M4CL_MAX_EXCHANGE_TYPE_ID ;
		}
		
		memcpy( acExchangeTypeArg, pccValue, iLength) ;
		*( acExchangeTypeArg + iLength ) = '\0';
		m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
    }

	
	//Exchange Date (argumento 5)
	
    if( iArgumentNumber > 4 )
    {
        iResult = RuntimePopDate( vValue ) ;
        M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ExchangeDate" << "ApplyValue (2)" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
		
		dExchangeDateArg = vValue.Data.DoubleData ;
    }
    else
    {
		dExchangeDateArg = 0 ;
    }
	
	
	//Exchange Currency (argumento 4)
	
	*acDestCurrencyArg = '\0' ;
	
    if( iArgumentNumber > 3 )
    {
        iResult = RuntimePopString( vValue ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Exchange currency" << "ApplyValue (2)" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
	
		pccValue = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
		
		iLength = strlen( pccValue ) ;
		
		if( iLength > M4CL_MAX_CURRENCY_ID )
		{
			iLength = M4CL_MAX_CURRENCY_ID ;
		}
		
		memcpy( acDestCurrencyArg, pccValue, iLength  ) ;
		*( acDestCurrencyArg + iLength ) = '\0';
		m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
    }
	
	//TargetDMD (argumento 3)
	
	*acTargetDmdArg = '\0' ;
	
	if( iArgumentNumber > 2 )
    {
		iResult = RuntimePopString( vValue ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "TargetDMD" << "ApplyValue (2)" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
		
		pccValue = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
		
		iLength = strlen( pccValue ) ;
		
		if( iLength > M4CL_MAX_DMD_ID )
		{
			iLength = M4CL_MAX_DMD_ID ;
		}
		
		memcpy( acTargetDmdArg, pccValue, iLength ) ;
		*( acTargetDmdArg + iLength ) = '\0';
		m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;

		//--new pasamos a mayusculas
		strupr(acTargetDmdArg);
		//-- fin new
	}
	
	//HostId (Argumento 2)
	
	*acHostNodeArg = '\0' ;
	
    iResult = RuntimePopString( vValue ) ;
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "HostId" << "ApplyValue (2)" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
	
	pccValue = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
	
	iLength = strlen( pccValue ) ;
	
	if( iLength > M4CL_MAX_NODE_ID )
	{
		iLength = M4CL_MAX_NODE_ID ;
	}
	
	memcpy( acHostNodeArg, pccValue, iLength ) ;
	*( acHostNodeArg + iLength ) = '\0';
	m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
	
	
	//ChannelId (argumento 1)
	
	*acHostChannelArg = '\0' ;
	
    iResult = RuntimePopString( vValue );
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ChannelId" << "ApplyValue (2)" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
	
	pccValue = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
	
	iLength = strlen( pccValue ) ;
	
	if( iLength > M4CL_MAX_T3_ID )
	{
		iLength = M4CL_MAX_T3_ID ;
	}
	
	memcpy( acHostChannelArg, pccValue, iLength  ) ;
	*( acHostChannelArg + iLength ) = '\0';
	m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1, BeginLogCat<<"Trace VM1: InitApplyValue. Channel:<"<<acHostChannelArg<<"> Host Id:<"<<acHostNodeArg<<"> TargetDMD: <"<<acTargetDmdArg<<"> Currency: <"<<acDestCurrencyArg << ">" << EndLogCat);
#endif
	
	// ==========================================================
	// asignación de variables del target =======================
	// ==========================================================
	
    poTargetAccess = m_oState1.m_poAccess ;
	
    poTargetCMCR = poTargetAccess->GetpCMCR() ;
	
#ifdef _DEBUG
	if( poTargetCMCR == NULL )
	{
		g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"Null mcr in ApplyValue (2). Channel: TargetCMCR"<<EndLog;
        RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}
#endif
	
    pccTargetChannel = poTargetCMCR->GetChannelId() ;
	
    poTargetCurrentRegister = &( m_oState1.m_poRecordSet->Current ) ;
    
    hTargetNode = m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle() ;
	
    iTargetItems = poTargetCMCR->GetNodeNumberOfItems( hTargetNode ) ;
	
	
	// ==========================================================
	// asignación de variables del host =========================
	// ==========================================================
	
    if( *acHostChannelArg == '\0' || strcmpi( acHostChannelArg, pccTargetChannel ) == 0 )
    {
        iSameChannel = 1 ;
        poHostAccess = m_oState1.m_poAccess ;
    }
    else
    {
        iSameChannel = 0 ;
		
        pccHostInstance = poTargetCMCR->GetNodeT3AliasInstanceIdByAlias( hTargetNode, acHostChannelArg ) ;
		
        if( pccHostInstance == NULL )
        {
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_INSTANCE, acHostChannelArg<<poTargetCMCR->iNodeN(hTargetNode)<<LogCatAnd<<poTargetCMCR->nNodeN(hTargetNode)<<"ApplyValue (2)" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
            return( M4_ERROR ) ;
        }
		
        iResult = m_poVM->GetL2Access( poTargetAccess->GetpChannel(), pccHostInstance, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState1.m_Flags), &poHostAccess ) ;		
        if( iResult != M4_SUCCESS )
        {
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_GET_ACCESS_L2, acHostChannelArg<<"ApplyValue (2)" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
            return( M4_ERROR ) ;
        }
    }
	
    poHostCMCR = poHostAccess->GetpCMCR() ;
	
#ifdef _DEBUG
	if( poHostCMCR == NULL )
	{
		g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"Null mcr in ApplyValue (2). Channel: HostCMCR"<<EndLog;
        RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}
#endif
	
    if( iSameChannel == 1 )
    {
        iHostHandle = poHostCMCR->GetNodeAliasNodeHandleByAlias( hTargetNode, acHostNodeArg, m_oState1.m_poFormula.GetLevel() ) ;
    }
    else
    {
        iHostHandle = poHostCMCR->GetChannelNodeHandleById( acHostNodeArg ) ;
    }
	
	if( iHostHandle == 0 )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, acHostNodeArg <<"ApplyValue (2)" << DumpCallStack() );
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}
	
    iHostComponentHandle = poHostCMCR->GetNodeDmdComponentItem( iHostHandle ) ;
	
	if( iHostComponentHandle == 0 )
	{
		g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"Dmd component"
														<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
														<<"ApplyValue (2)"
														<< DumpCallStack() << EndLog;
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}
	
    iHostStartDateHandle = poHostCMCR->GetNodeStartDateItem( iHostHandle ) ;
	
	if( iHostStartDateHandle == 0 )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_START_DT_ITEM, M4ObjNodeN(poHostCMCR,iHostHandle)<<"ApplyValue (2)" << DumpCallStack() );
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}
	
    iHostEndDateHandle = poHostCMCR->GetNodeEndDateItem( iHostHandle ) ;
	
	if( iHostEndDateHandle == 0 )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_END_DT_ITEM, M4ObjNodeN(poHostCMCR,iHostHandle)<<"ApplyValue (2)" << DumpCallStack() );
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}

	iHostStartDateFilterItemHandle = poHostCMCR->GetNodeFilterStartDateItem( iHostHandle ) ;
	
	if( iHostStartDateFilterItemHandle == 0 )
	{
		g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"Filter Start Date"
														<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
														<<"ApplyValue (2)"<< DumpCallStack() << EndLog;
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}

	iHostEndDateFilterItemHandle = poHostCMCR->GetNodeFilterEndDateItem( iHostHandle ) ;
	
	if( iHostStartDateFilterItemHandle == 0 )
	{
         RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}

    iHostPriorityHandle = poHostCMCR->GetNodePriorityItem( iHostHandle ) ;
	
    iHostItems = poHostCMCR->GetNodeNumberOfItems( iHostHandle ) ;
	
    iHostComponentIndex = poHostCMCR->GetItemIndex( iHostComponentHandle ) ;
    iHostStartDateIndex = poHostCMCR->GetItemIndex( iHostStartDateHandle ) ;
    iHostEndDateIndex   = poHostCMCR->GetItemIndex( iHostEndDateHandle ) ;
	
	if( iHostPriorityHandle != 0 )
	{
        iHostPriorityIndex = poHostCMCR->GetItemIndex( iHostPriorityHandle ) ;
	}
	
	
    poDHostNode = &( poHostAccess->Node[ ClHandle( iHostHandle ) ] ) ;
	
#ifdef _DEBUG
	if( poDHostNode == NULL )
	{
		g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"Null pointer in ApplyValue(2) for node with handle:"<<LogCat<<iHostHandle<<EndLog;
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}
#endif
	
    poDHostRecordSet = &( poDHostNode->RecordSet ) ;
    
	if( !poDHostRecordSet->IsValid() )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK, "ApplyValue (2)"<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)<<DumpTreePath(poDHostRecordSet->GetpAccess(), iHostHandle) << DumpCallStack() );
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}
	
    iHostRegisters = poDHostRecordSet->Register.Count() ;
	
	if(iHostRegisters == 0){ //si no ha registros->directamente salimos existosamente
		RuntimePushDouble( M4_SUCCESS ) ;
		return( M4_SUCCESS ) ;
	}
    
	// multimoneda
	
	poCurrencyAccess = NULL ;
	
	
	// ==========================================================
	// algoritmo ================================================
	// ==========================================================
	
	//leemos del target recordset las fecha_ini, fech_fin, run_date y slice_mode
	iResult=poDHostRecordSet->Register[0].Item[ClHandle(iHostStartDateFilterItemHandle)].Value.Get(vValue); //tiene al menos un reg. y no posicionamos
    M4CHECK_RES(iResult==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, M4ObjNodeItemI(poHostCMCR,iHostStartDateFilterItemHandle)<<"ApplyValue (2)" << DumpCallStack()  )	;
    dPhase1Start=vValue.Data.DoubleData;

	iResult=poDHostRecordSet->Register.Item[ClHandle(iHostEndDateFilterItemHandle)].Value.Get(vValue); 
    M4CHECK_RES(iResult==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, M4ObjNodeItemI(poHostCMCR,iHostEndDateFilterItemHandle)<<"ApplyValue (2)" << DumpCallStack()  )	;
	dPhase1End=vValue.Data.DoubleData;

	iResult=poDHostRecordSet->Register.Item[M4CL_VM_PR_SLICE_MODE_ITEM].Value.Get(vValue); 
    M4CHECK_RES(iResult==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, M4CL_VM_PR_SLICE_MODE_ITEM<<M4ObjNodeN(poHostCMCR,iHostHandle)<<"ApplyValue (2)" << DumpCallStack()  )	;
    bPhase1Slice=vValue.Data.DoubleData==0 ? M4_FALSE : M4_TRUE;
	
	iResult=poDHostRecordSet->Register.Item[M4CL_VM_PR_RUN_DATE_ITEM].Value.Get(vValue); 
    M4CHECK_RES(iResult==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, M4CL_VM_PR_RUN_DATE_ITEM<<M4ObjNodeN(poHostCMCR,iHostHandle)<<"ApplyValue (2)" << DumpCallStack()  )	;
    dPhase1Run=vValue.Data.DoubleData;

	/*
    iResult = poTargetAccess->RunSliceMode.Get( vValue ) ;
	
#ifdef _DEBUG
    if( iResult != M4_SUCCESS )
    {
        SET CODE( M4_CH_VM_NO_SLICE_MODE, ERRORLOG, "No Slice Mode in ApplyValue (2)" ) ;
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
    }
#endif
	
    if( vValue.Type == M4CL_CPP_TYPE_NULL )
    {
        iSliceModeOn = 0 ;
    }
    else
    {
        iSliceModeOn = m4uint8_t( vValue.Data.DoubleData ) ;
    }
	*/
    if( bPhase1Slice == 0)
    {
		m4bool_t  bRunSlice = m_poVM->GetRunSliceMode(poTargetAccess);
		// ==========================================================
		// modo de no tramos ========================================
		// ==========================================================
		
        for( i = 0 ; i < iHostRegisters ; i++ )
        {
            iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( iHostStartDateIndex ) ].Value.Get( vValue ) ;
			
            if( iResult != M4_SUCCESS )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poHostCMCR,iHostStartDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
                return( M4_ERROR ) ;
            }
			
            if( IsNull( vValue ) )
            { 
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL, M4ObjNodeItemI(poHostCMCR,iHostStartDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
                return( M4_ERROR ) ;
            }
			
            if( vValue.Type != M4CL_CPP_TYPE_DATE )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE, M4ObjNodeItemI(poHostCMCR,iHostStartDateHandle)<<"StartDate"<< "ApplyValue (2)"  << DumpCallStack() );
                RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
            }
			
            dStart = vValue.Data.DoubleData ;
			
            iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( iHostEndDateIndex ) ].Value.Get( vValue ) ;
			
            if( iResult != M4_SUCCESS )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poHostCMCR,iHostEndDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
                RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
            }
			
            if( IsNull( vValue ) )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL, M4ObjNodeItemI(poHostCMCR,iHostEndDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
                RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
            }
			
            if( vValue.Type != M4CL_CPP_TYPE_DATE )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE, M4ObjNodeItemI(poHostCMCR,iHostEndDateHandle)<<"EndDate"  << "ApplyValue (2)"  << DumpCallStack() );
                RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
            }
			
			
            dEnd = vValue.Data.DoubleData ;
			
            if( dStart <= dPhase1Run && dEnd >= dPhase1Run)
            {
                iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( iHostComponentIndex ) ].Value.Get( vValue ) ;
				
                if( iResult != M4_SUCCESS )
                {
					DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poHostCMCR,iHostComponentHandle)<<"ApplyValue (2)" << DumpCallStack() );
                    RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
                }
				
                if( IsNull( vValue ) )
                {
					DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL, M4ObjNodeItemI(poHostCMCR,iHostComponentHandle)<<"ApplyValue (2)" << DumpCallStack() );
                    RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
                }
				
                if( vValue.Type == M4CL_CPP_TYPE_STRING_VAR )
                {
                    pccHostDmdComponent = vValue.Data.PointerChar ;
					
					//--new
					//copiamos pccHostDmdComponent a acHostDmdComponent y pasamos a mayusculas
					iLength = strlen( pccHostDmdComponent ) ;
					
					if( iLength > M4CL_MAX_DMD_COMPONENT_ID )
					{
						iLength = M4CL_MAX_DMD_COMPONENT_ID ;
					}
					
					memcpy( acHostDmdComponent, pccHostDmdComponent, sizeof( m4char_t) * iLength ) ;
					*( acHostDmdComponent + iLength ) = '\0' ;
					strupr(acHostDmdComponent);
					//--end new

                }
                else if( vValue.Type == M4CL_CPP_TYPE_NUMBER )
                {
                    sprintf( acTempString, "%.0f", vValue.Data.DoubleData ) ;
                    pccHostDmdComponent = acTempString ;
                }
                else
                {
					DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE, M4ObjNodeItemI(poHostCMCR,iHostComponentHandle)<<"DmdComponent" << "ApplyValue (2)"  << DumpCallStack() );
                    RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
                }
				

                if( iHostPriorityHandle != 0 )
                {
                    iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( iHostPriorityIndex ) ].Value.Get( vValue ) ;
					
                    if( iResult != M4_SUCCESS )
                    {
						DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poHostCMCR,iHostPriorityHandle)<<"ApplyValue (2)" << DumpCallStack() );
                        RuntimePushDouble( M4_ERROR ) ;
						return( M4_ERROR ) ;
                    }
					
                    if( IsNull( vValue ) )
                    {
						DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL, M4ObjNodeItemI(poHostCMCR,iHostPriorityHandle)<<"ApplyValue (2)" << DumpCallStack() );
                        RuntimePushDouble( M4_ERROR ) ;
						return( M4_ERROR ) ;
                    }
					
                    if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
                    {
						DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE, M4ObjNodeItemI(poHostCMCR,iHostPriorityHandle)<<"Priority"<< "ApplyValue (2)"  << DumpCallStack() );
                        RuntimePushDouble( M4_ERROR ) ;
						return( M4_ERROR ) ;
                    }
					
                    iHostPriority = m4uint8_t( vValue.Data.DoubleData ) ;
                }
				
				
                for( k = 0 ; k < iHostItems ; k++ )
                {
                    iHostItemHandle = poHostCMCR->GetNodeItemHandleByPosition( iHostHandle, k ) ;
					
#ifdef _DEBUG
					if( iHostItemHandle == 0 )
					{
						g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"HostItem["<<LogCat<<k<<LogCat<<"]"
																		<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
																		<<"ApplyValue (2)"<< DumpCallStack() << EndLog;
                        RuntimePushDouble( M4_ERROR ) ;
						return( M4_ERROR ) ;
					}
#endif
					
                    iHostType = poHostCMCR->GetItemType( iHostItemHandle ) ;
                    
					
                    if( iHostType == M4CL_ITEM_TYPE_METHOD || k == iHostComponentIndex )
                    {
                        continue ;
                    }
					
					// es un item host valido
					
                    pccHostItemId = poHostCMCR->GetItemId( iHostItemHandle ) ;
					
#ifdef _DEBUG
					if( pccHostItemId == NULL )
					{
                        RuntimePushDouble( M4_ERROR ) ;
					}
#endif

					CHECK_CHLOG_DEBUGF( pccHostItemId == NULL, M4_ERROR, M4_CH_VM_INTERNAL_ERROR, __FILE__<<__LINE__ << k ) ;

					
					iTargetItemHandle = poTargetCMCR->GetNodeDmdHandleByIds( hTargetNode, acTargetDmdArg, acHostDmdComponent , pccHostItemId, M4_TRUE ) ;

                    if( iTargetItemHandle == 0 )
                    {
                        continue ;
                    }
					
					// es un item al que hay que generar valor
					
					iTargetItemIndex = poTargetCMCR->GetItemIndex( iTargetItemHandle ) ;
					
					iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( k ) ].Value.Get( vValue ) ;
					
                    if( iResult != M4_SUCCESS )
                    {
						DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, k << M4ObjNodeN(poHostCMCR,iHostHandle)<<"ApplyValue (2)" << DumpCallStack() );
                        RuntimePushDouble( M4_ERROR ) ;
						return( M4_ERROR ) ;
                    }
					
					
					// Conversión de tipos variant ======================================================================

					/* Bug 0102160
					La conversión de tipos siempre hay que hacerla aunque no haya cambio de moneda
					*/

					iHostM4Type = poHostCMCR->GetItemM4Type( iHostItemHandle ) ;
					
					if( iHostM4Type == M4CL_M4_TYPE_VARIANT || iHostM4Type == M4CL_M4_TYPE_NUM_VARIANT ) 
					{
						iHostAuxItemHandle = poHostCMCR->GetItemAuxItem( iHostItemHandle ) ;
						
						if( iHostAuxItemHandle == 0 )
						{
							g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"Variant Aux Item"
																			<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
																			<<"ApplyValue (2)"<< DumpCallStack() << EndLog;
							RuntimePushDouble( M4_ERROR ) ;
							return( M4_ERROR ) ;
						}
						
						iHostSrcItemHandle = iHostAuxItemHandle ;
						iHostSrcItemIndex = poHostCMCR->GetItemIndex( iHostSrcItemHandle ) ;
						
						
						iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( iHostSrcItemIndex ) ].Value.Get( vAux ) ;
						
						if( iResult != M4_SUCCESS )
						{
							DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostSrcItemHandle)<<"Variant Aux Item"<<"ApplyValue (2)" << DumpCallStack() );
							RuntimePushDouble( M4_ERROR ) ;
							return( M4_ERROR ) ;
						}
						
						if( IsNull( vAux ) )
						{
							DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostSrcItemHandle)<<"Variant Aux Item"<<"ApplyValue (2)" <<  DumpCallStack() );
							RuntimePushDouble( M4_ERROR ) ;
							return( M4_ERROR ) ;
						}
						
						if( vAux.Type != M4CL_CPP_TYPE_NUMBER )
						{
							DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostSrcItemHandle)<<"Variant Aux Item"<< "ApplyValue (2)"<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) << M4ClCppType(vAux.Type) << DumpCallStack() );
							RuntimePushDouble( M4_ERROR ) ;
							return( M4_ERROR ) ;
						}
						
						iHostRealM4Type = m4uint8_t( vAux.Data.DoubleData ) ;
						
						if( iHostM4Type == M4CL_M4_TYPE_VARIANT && vValue.Type == M4CL_CPP_TYPE_STRING_VAR )
						{
							switch( iHostRealM4Type )
							{
								case M4CL_M4_TYPE_NUMBER :
								case M4CL_M4_TYPE_CURRENCY :
									
									vValue.Type = M4CL_CPP_TYPE_NUMBER ;
									vValue.Data.DoubleData = atof( vValue.Data.PointerChar ) ;
									break ;
									
								case M4CL_M4_TYPE_DATE :
								case M4CL_M4_TYPE_TIMESTAMP :
								case M4CL_M4_TYPE_HOUR :
									
									vValue.Type = M4CL_CPP_TYPE_DATE ;
									vValue.Data.DoubleData = ClStringToDate( vValue.Data.PointerChar ) ;
									break ;
									
								default :
									break ;
							}
						}
					}
					else
					{
						iHostRealM4Type = iHostM4Type ;
						iHostSrcItemHandle = iHostItemHandle ;
					}
						
						
					// Multimoneda ======================================================================================
					
					if( *acDestCurrencyArg != '\0' )
					{
						if( iHostRealM4Type == M4CL_M4_TYPE_CURRENCY && vValue.Type == M4CL_CPP_TYPE_NUMBER )
						{
							// moneda origen
							iHostAuxItemHandle = poHostCMCR->GetItemAuxItem( iHostSrcItemHandle ) ;
							
							if( iHostAuxItemHandle == 0 )
							{
								g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"Currency Aux Item"
																				<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
																				<<"ApplyValue (2)"<< DumpCallStack() << EndLog;
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							iHostAuxItemIndex = poHostCMCR->GetItemIndex( iHostAuxItemHandle ) ;
							
							
							iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( iHostAuxItemIndex ) ].Value.Get( vSrcCurrency ) ;
							
							if( iResult != M4_SUCCESS )
							{
								DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Currency Aux Item"<<"ApplyValue (2)" << DumpCallStack() );
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							if( IsNull( vSrcCurrency ) )
							{
								DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Currency Aux Item"<<"ApplyValue (2)" << DumpCallStack() );
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							if( vSrcCurrency.Type != M4CL_CPP_TYPE_STRING_VAR )
							{
								DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Currency Aux Item"<< "ApplyValue (2)"  << M4ClCppType(M4CL_CPP_TYPE_STRING_VAR) << M4ClCppType(vSrcCurrency.Type) <<DumpCallStack() );
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							// fecha de cambio
							iHostAuxItemHandle = poHostCMCR->GetItemAuxItem( iHostAuxItemHandle ) ;
							
							if( iHostAuxItemHandle != 0 )
							{
								iHostAuxItemIndex = poHostCMCR->GetItemIndex( iHostAuxItemHandle ) ;
								
								
								iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( iHostAuxItemIndex ) ].Value.Get( vAux ) ;
								
								if( iResult != M4_SUCCESS )
								{
									DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Date Aux Item"<<"ApplyValue (2)" << DumpCallStack() );
									RuntimePushDouble( M4_ERROR ) ;
									return( M4_ERROR ) ;
								}
								
								if( IsNull( vAux ) )
								{
									if( dExchangeDateArg != 0 )
									{
										dExchangeDate = dExchangeDateArg ;
									}
									else
									{
										DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Date Aux Item"<<"ApplyValue (2)" << DumpCallStack() );
										RuntimePushDouble( M4_ERROR ) ;
										return( M4_ERROR ) ;
									}
								}
								else 
								{
									if( vAux.Type != M4CL_CPP_TYPE_DATE )
									{
										DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Date Aux Item"<<"ApplyValue (2)"  <<M4ClCppType(M4CL_CPP_TYPE_DATE) << M4ClCppType(vAux.Type) << DumpCallStack() );
										RuntimePushDouble( M4_ERROR ) ;
										return( M4_ERROR ) ;
									}
									
									dExchangeDate = vAux.Data.DoubleData ;
								}
							}
							else if( dExchangeDateArg != 0 )
							{
								dExchangeDate = dExchangeDateArg ;
							}
							else
							{
								g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"Exchange Date"
																				<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
																				<<"ApplyValue (2)"<< DumpCallStack() << EndLog;
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}

							// tipo de cambio
							if( iHostAuxItemHandle != 0 ) //si la fecha de cambio la leimos del item auxiliar, intentamos leer el tipo, sino vamos por los argumentos
							{
								iHostAuxItemHandle = poHostCMCR->GetItemAuxItem( iHostAuxItemHandle ) ;
							}
	
							if( iHostAuxItemHandle != 0 )
							{
								iHostAuxItemIndex = poHostCMCR->GetItemIndex( iHostAuxItemHandle ) ;
								
								
								iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( iHostAuxItemIndex ) ].Value.Get( vAux ) ;
								
								if( iResult != M4_SUCCESS )
								{
									DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Exchange Type"<<"ApplyValue (2)" << DumpCallStack() );
									RuntimePushDouble( M4_ERROR ) ;
									return( M4_ERROR ) ;
								}
								
								if( IsNull( vAux ) )
								{
									if( *acExchangeTypeArg != '\0' )
									{
										pccExchangeType = acExchangeTypeArg ;
									}
									else
									{
										DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Exchange Type"<<"ApplyValue (2)" << DumpCallStack() );
										RuntimePushDouble( M4_ERROR ) ;
										return( M4_ERROR ) ;
									}
								}
								else 
								{
									if( vAux.Type != M4CL_CPP_TYPE_STRING_VAR )
									{
										DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Exchange Type"<< "ApplyValue (2)"  << M4ClCppType(M4CL_CPP_TYPE_STRING_VAR) << M4ClCppType(vAux.Type) <<DumpCallStack() );
										RuntimePushDouble( M4_ERROR ) ;
										return( M4_ERROR ) ;
									}
									
									pccExchangeType = vAux.Data.PointerChar ;
								}
							}
							else if( *acExchangeTypeArg != '\0' )
							{
								pccExchangeType = acExchangeTypeArg ;
							}
							else
							{
								g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"Exchange Type"
																				<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
																				<<"ApplyValue (2)"<< DumpCallStack() << EndLog;
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}

							
							dCutRate = 1 ;
							
							iResult = _curGetExchangeRate( vSrcCurrency.Data.PointerChar, acDestCurrencyArg, pccExchangeType, dExchangeDate, dCutRate, bExchangeFound, NULL, poCurrencyAccess ) ;
							
							if( iResult != M4_SUCCESS  )
							{
								DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_EXCHANGE, vSrcCurrency.Data.PointerChar<<acDestCurrencyArg<<dExchangeDate<<"ApplyValue (2)" << DumpCallStack() );
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							vValue.Data.DoubleData = vValue.Data.DoubleData * dCutRate ;
						}
					}
					
					
					// Asignación =======================================================================================
					
                    if( iHostPriorityHandle != 0 )
                    {	//con prioridad

						if(bRunSlice){
							//caso de que ejecuten el AV sin tramos pero la nómina se ejecute con tramos
							iResult=m_poVM->CheckAndCreateItemSlice(m_oState1.m_dBeginDate, m_oState1.m_dEndDate , &(poTargetCurrentRegister->Item[ iTargetItemIndex ]) );
							if(iResult==M4_SUCCESS){
								/* Bug 0076620
								Se asignan los subtramos si los hubiera
								*/
								iResult = m_poVM->SetValue( poTargetCurrentRegister->Item, vValue, iHostPriority, 2, m_oState1.m_dBeginDate, m_oState1.m_dEndDate, M4_TRUE );
							}
						}
						else {
							iResult = poTargetCurrentRegister->Item[ iTargetItemIndex ].Value.Set( vValue, iHostPriority, bDone ) ;

							iPriority = iHostPriority;
							bAssigned = bDone ;
						}
                    }
                    else //sin prioridad
                    {	
						if(bRunSlice){
						//caso de que ejecuten el AV sin tramos pero la nómina se ejecute con tramos
							iResult=m_poVM->CheckAndCreateItemSlice(m_oState1.m_dBeginDate, m_oState1.m_dEndDate , &(poTargetCurrentRegister->Item[ iTargetItemIndex ]) );
							if(iResult==M4_SUCCESS){
								/* Bug 0076620
								Se asignan los subtramos si los hubiera
								*/
								iResult = m_poVM->SetValue( poTargetCurrentRegister->Item, vValue, -1, 2, m_oState1.m_dBeginDate, m_oState1.m_dEndDate, M4_TRUE );
							}
						}
						else {
							iResult = poTargetCurrentRegister->Item[ iTargetItemIndex ].Value.Set( vValue ) ;

							iPriority = poTargetCurrentRegister->Item.Value.GetPriority() ;
							bAssigned = M4_TRUE ;
						}
                    }
					

                    if( iResult != M4_SUCCESS )
                    {
						DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, M4ObjNodeItemI(poTargetCMCR,iTargetItemHandle)<<"ApplyValue (2)" << DumpCallStack() );
                        RuntimePushDouble( M4_ERROR ) ;
						return( M4_ERROR ) ;
                    }
					else
					{
						if( bRunSlice == M4_FALSE && GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_VALUES)
						{
							GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddValue( poTargetCurrentRegister->Item, vValue, iPriority, bAssigned, 2, m_oState1, DumpCallStack( &m_oState1, M4_TRUE ).c_str() ) ;
						}
					}
                }
            }
        }
		
		
        RuntimePushDouble( M4_SUCCESS ) ;
        return( M4_SUCCESS ) ;
    }
	
	
	// ==========================================================
	// modo de tramos ===========================================
	// ==========================================================
	
    poDTargetChannel = poTargetAccess->GetpChannel() ;
	
#ifdef _DEBUG
    if( poDTargetChannel == NULL )
    {
		g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"Null mcr in Pivot. Channel: TargetCMCR"<<EndLog;
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
    }
#endif
	
    iResult = poDTargetChannel->UnitDate.Get( vValue ) ;
	
#ifdef _DEBUG
    if( iResult != M4_SUCCESS )
    {
		g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"No Get Date Unit in ApplyValue (2)"<<EndLog;
        RuntimePushDouble(iResult);
        return( iResult ) ;
    }
#endif
	
    dTimeUnit = vValue.Data.DoubleData ;
	
    m_oItemSlicesStack.Clear( dPhase1Start, dPhase1End, dTimeUnit ) ;
	m_oNodeSlicesStack.Clear( dPhase1Start, dPhase1End, dTimeUnit ) ;
	
	
    *acLastDmdComponent = '\0' ;
    iStart = 0 ;
	
    for( i = 0 ; i <= iHostRegisters ; i++ )
    {
        if( i == iHostRegisters )
        {
            if( *acLastDmdComponent == '\0' )
            {
                iDump = 0 ;
            }
            else
            {
                iDump = 1 ;
            }
        }
        else
        {
            iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( iHostComponentIndex ) ].Value.Get( vValue ) ;
			
            if( iResult != M4_SUCCESS )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poHostCMCR,iHostComponentHandle)<<"ApplyValue (2)" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
                return( M4_ERROR ) ;
            }
			
            if( IsNull( vValue ) )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL, M4ObjNodeItemI(poHostCMCR,iHostComponentHandle)<<"ApplyValue (2)" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
                return( M4_ERROR ) ;
            }
			
            if( vValue.Type == M4CL_CPP_TYPE_STRING_VAR )
            {
                pccHostDmdComponent = vValue.Data.PointerChar ;
            }
            else if( vValue.Type == M4CL_CPP_TYPE_NUMBER )
            {
                sprintf( acTempString, "%.0f", vValue.Data.DoubleData ) ;
                pccHostDmdComponent = acTempString ;
            }
            else
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE, M4ObjNodeItemI(poHostCMCR,iHostComponentHandle)<<"Dmd Component" << "ApplyValue (2)"  << DumpCallStack() );
                RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
            }
			
			
            if( strcmpi( pccHostDmdComponent, acLastDmdComponent ) != 0 )
            {
                if( *acLastDmdComponent == '\0' )
                {
                    iDump = 0 ;
					
                    iLength = strlen( pccHostDmdComponent ) ;
					
                    if( iLength > M4CL_MAX_DMD_COMPONENT_ID )
                    {
                        iLength = M4CL_MAX_DMD_COMPONENT_ID ;
                    }
					
                    memcpy( acLastDmdComponent, pccHostDmdComponent, sizeof( m4char_t) * iLength ) ;
                    *( acLastDmdComponent + iLength ) = '\0' ;
				
					//pasamos a mayusculas
					strupr(acLastDmdComponent);
                }
                else
                {
                    iDump = 1 ;
                }
            }
            else
            {
                iDump = 0 ;
            }
        }
		
		// hay que volcar
		
        if( iDump == 1 )
        {
            m_oNodeSlicesStack.Sort() ; //ya tenemos todos los tramos que hay que generar->Ordenamos
			
			

			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_VM_INTERNAL_ERROR, __FILE__<<__LINE__ <<"ApplyValue (2)") ;

			
            for( k = 0 ; k < iHostItems ; k++ )
            {
                iHostItemHandle = poHostCMCR->GetNodeItemHandleByPosition( iHostHandle, k ) ;
				
#ifdef _DEBUG
				if( iHostItemHandle == 0 )
				{
					g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"HostItem["<<LogCat<<k<<LogCat<<"]"
																	<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
																	<<"ApplyValue (2)"<< DumpCallStack() << EndLog;
                    RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
#endif
				
                iHostType = poHostCMCR->GetItemType( iHostItemHandle ) ;
         
				 
                if( iHostType == M4CL_ITEM_TYPE_METHOD || k == iHostComponentIndex )
                {
                    continue ;
                }
				
				// es un item host valido
				
                pccHostItemId = poHostCMCR->GetItemId( iHostItemHandle ) ;
				
#ifdef _DEBUG
				if( pccHostItemId == NULL )
				{
                    RuntimePushDouble( M4_ERROR ) ;
				}
#endif

				CHECK_CHLOG_DEBUGF( pccHostItemId == NULL, M4_ERROR, M4_CH_VM_INTERNAL_ERROR, __FILE__<<__LINE__ << k ) ;

				
				//iTargetItemHandle = poTargetCMCR->GetNodeDmdHandleByIds( hTargetNode, acTargetDmdArg, acLastDmdComponent, pccHostItemId ) ;
				//--new
				iTargetItemHandle = poTargetCMCR->GetNodeDmdHandleByIds( hTargetNode, acTargetDmdArg, acLastDmdComponent, pccHostItemId , M4_TRUE ) ;
				//fin new
                if( iTargetItemHandle == 0 )
                {
                    continue ;
                }
				
				// es un item al que hay que generar tramos
				
                iTargetSliceBhrv = poTargetCMCR->GetItemSliceBhrv( iTargetItemHandle ) ;
				iTargetItemIndex = poTargetCMCR->GetItemIndex( iTargetItemHandle ) ;
				
			     //añadimos slices a mano
				poTargetCurrentRegister->Item[ iTargetItemIndex ]; //nos movemos al item
					
				//copiamos de la pila de nodo a la de item
				m_oItemSlicesStack=m_oNodeSlicesStack;
				
				if(iHostPriorityHandle != 0){
				
					//reducimos con los tramos del item
					iSlices=poTargetCurrentRegister->Item.Value.Count();
					for(js=0;js<iSlices;++js){
						oPriorSlice.m_dStart=poTargetCurrentRegister->Item.Value[ClSliceIndex(js)].GetStartDate();
						oPriorSlice.m_dEnd=poTargetCurrentRegister->Item.Value.GetEndDate();
						oPriorSlice.m_iPrior=poTargetCurrentRegister->Item.Value.GetPriority();
						m_oItemSlicesStack.Reduce(oPriorSlice);
					}
					//metemos los tramos resultantes
					iSlices=m_oItemSlicesStack.GetLength();
					for(js=0;js<iSlices;++js){
						m_oItemSlicesStack.GetSlice(js, oPriorSlice);
						poTargetCurrentRegister->Item.Value.AddSlice(oPriorSlice.m_dStart, oPriorSlice.m_dEnd);
						poTargetCurrentRegister->Item.Value[oPriorSlice.m_dStart].SetPriority(oPriorSlice.m_iPrior);
					}
				}
				else{
					iSlices=m_oItemSlicesStack.GetLength();
					for(js=0;js<iSlices;++js){
						m_oItemSlicesStack.GetSlice(js, oPriorSlice);
						poTargetCurrentRegister->Item.Value.AddSlice(oPriorSlice.m_dStart, oPriorSlice.m_dEnd);
					}
				
				}
				
				// enchufamos los valores
				
                iHostM4Type = poHostCMCR->GetItemM4Type( iHostItemHandle ) ;
				
                if( iHostM4Type == M4CL_M4_TYPE_VARIANT || iHostM4Type == M4CL_M4_TYPE_CURRENCY || iHostM4Type == M4CL_M4_TYPE_NUM_VARIANT)
                {
                    iHostAuxItemHandle = poHostCMCR->GetItemAuxItem( iHostItemHandle ) ;
					
                    if( iHostAuxItemHandle == 0 )
                    {
						g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"Aux Item"
																		<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
																		<<"ApplyValue (2)"<< DumpCallStack() << EndLog;
                        RuntimePushDouble( M4_ERROR ) ;
                        return( M4_ERROR ) ;
                    }
					
                    iHostAuxItemIndex = poHostCMCR->GetItemIndex( iHostAuxItemHandle ) ;
                }
				

                m=iStart; //registro por el que empezamos a buscar (los que corresponden a este dmd_comp)
						  //m irá como mucho hasta i

				//leemos los valores de FI y FF para el primer registro del grupo
				//antes de bucle por todos los tramos de la pila
				iResult = poDHostRecordSet->Register[ m ].Item[ ClItemIndex( iHostStartDateIndex ) ].Value.Get( vValue ) ;
				
				if( iResult != M4_SUCCESS ){
					DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poHostCMCR,iHostStartDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
					RuntimePushDouble(M4_ERROR);
					return( M4_ERROR ) ;
				}
				
				if( IsNull( vValue ) ){
					g_oChLog<<BeginError(M4_CH_VM_ITEM_IS_NULL)<< M4ObjNodeItemI(poHostCMCR,iHostStartDateHandle)<<"ApplyValue (2)"<<EndLog;
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				if( vValue.Type != M4CL_CPP_TYPE_DATE ){
					DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE, M4ObjNodeItemI(poHostCMCR,iHostStartDateHandle)<<"Start Date" << "ApplyValue (2)"  << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				dStart = vValue.Data.DoubleData ;
				
				iResult = poDHostRecordSet->Register.Item[ ClItemIndex( iHostEndDateIndex ) ].Value.Get( vValue ) ;
				
				if( iResult != M4_SUCCESS ){
					DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poHostCMCR,iHostEndDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				if( IsNull( vValue ) ){
					DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL, M4ObjNodeItemI(poHostCMCR,iHostEndDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				if( vValue.Type != M4CL_CPP_TYPE_DATE ){
					DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE, M4ObjNodeItemI(poHostCMCR,iHostEndDateHandle)<<"End Date"<< "ApplyValue (2)"  << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				dEnd = vValue.Data.DoubleData ;

				//
				//Nos vamos recorriendo todos los tramos que tengamos en la pila, y vamos buscando el 
				//registro en el que está para leer el valor y poder enchufarlo (previo prorrateo y
				//cambio de moneda si procede)

				for( js=0; js < iSlices; ++js ){

					//leemos el tramo que tenemos que asignar
					m_oItemSlicesStack.GetSlice(js, oPriorSlice);
                    
					//si el registro que tenemos no corresponde, nos vamos al siguiente 
					//***************************OJO !!! tienen que estar ordenados
					while( (dStart > oPriorSlice.m_dEnd || dEnd < oPriorSlice.m_dStart) && m < i ){
						
						
						if(m!=poDHostRecordSet->Register.Index ){ //si el registro ya estaba leido no lo leemos a menos que nos vayamos al siguiente al final del while con el ++m
							iResult = poDHostRecordSet->Register[ m ].Item[ ClItemIndex( iHostStartDateIndex ) ].Value.Get( vValue ) ;
							
							if( iResult != M4_SUCCESS )
							{
								DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poHostCMCR,iHostStartDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
								RuntimePushDouble(M4_ERROR);
								return( M4_ERROR ) ;
							}
							
							if( IsNull( vValue ) )
							{
								g_oChLog<<BeginError(M4_CH_VM_ITEM_IS_NULL)<< M4ObjNodeItemI(poHostCMCR,iHostStartDateHandle)<<"ApplyValue (2)"<<EndLog;
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							if( vValue.Type != M4CL_CPP_TYPE_DATE )
							{
								DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE, M4ObjNodeItemI(poHostCMCR,iHostStartDateHandle)<<"Start Date" << "ApplyValue (2)"  << DumpCallStack() );
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							dStart = vValue.Data.DoubleData ;
							
							iResult = poDHostRecordSet->Register[ m ].Item[ ClItemIndex( iHostEndDateIndex ) ].Value.Get( vValue ) ;
							
							if( iResult != M4_SUCCESS )
							{
								DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poHostCMCR,iHostEndDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							if( IsNull( vValue ) )
							{
								DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL, M4ObjNodeItemI(poHostCMCR,iHostEndDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							if( vValue.Type != M4CL_CPP_TYPE_DATE )
							{
								DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE, M4ObjNodeItemI(poHostCMCR,iHostEndDateHandle)<<"End Date"<< "ApplyValue (2)"  << DumpCallStack() );
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							dEnd = vValue.Data.DoubleData ;
						}
						if(dStart > oPriorSlice.m_dEnd || dEnd < oPriorSlice.m_dStart){ //si no estamos en el registro adecuado ->al siguiente
							++m;
						}
					}


                    if( dStart <= oPriorSlice.m_dEnd && dEnd >= oPriorSlice.m_dStart){
                    
                        dDivider = dEnd - dStart + 1 ;
                        
						
                        if( dStart >= oPriorSlice.m_dStart)
                        {
                            dSliceDate = dStart ;
							
                            if( dEnd <= oPriorSlice.m_dEnd )
                            {
                                dCutRate = 1 ;
                            }
                            else
                            {
                                dCutRate = ( oPriorSlice.m_dEnd - dStart + 1 ) / dDivider ;
                            }
                        }
                        else
                        {
                            dSliceDate = oPriorSlice.m_dStart;
							
                            if( dEnd <= oPriorSlice.m_dEnd )
                            {
                                dCutRate = ( dEnd - oPriorSlice.m_dStart+ 1 ) / dDivider ;
                            }
                            else
                            {
                                dCutRate = ( oPriorSlice.m_dEnd - oPriorSlice.m_dStart+ 1 ) / dDivider ;
                            }
                        }
						
						
                        iResult = poDHostRecordSet->Register[ m ].Item[ ClItemIndex( k ) ].Value.Get( vValue ) ;
						
                        if( iResult != M4_SUCCESS )
                        {
							DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, k<<M4ObjNodeN(poHostCMCR,iHostHandle)<<"ApplyValue (2)" << DumpCallStack() );
							RuntimePushDouble( M4_ERROR ) ;
                            return( M4_ERROR ) ;
                        }
						
						
						// Variant ==========================================================================================
						
						iHostM4Type = poHostCMCR->GetItemM4Type( iHostItemHandle ) ;
						
						if( iHostM4Type == M4CL_M4_TYPE_VARIANT || iHostM4Type == M4CL_M4_TYPE_NUM_VARIANT )
						{
							iHostAuxItemHandle = poHostCMCR->GetItemAuxItem( iHostItemHandle ) ;
							
							if( iHostAuxItemHandle == 0 )
							{
								g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"Variant Aux Item"
																				<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
																				<<"ApplyValue (2)"<< DumpCallStack() << EndLog;
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							iHostSrcItemHandle = iHostAuxItemHandle ;
							iHostSrcItemIndex = poHostCMCR->GetItemIndex( iHostSrcItemHandle ) ;
							
							
							iResult = poDHostRecordSet->Register[ m ].Item[ ClItemIndex( iHostSrcItemIndex ) ].Value.Get( vAux ) ;
							
							if( iResult != M4_SUCCESS )
							{
								DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostSrcItemHandle)<<"Variant Aux Item"<<"ApplyValue (2)" << DumpCallStack() );
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							if( IsNull( vAux ) )
							{
								DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostSrcItemHandle)<<"Variant Aux Item"<<"ApplyValue (2)" << DumpCallStack() );
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							if( vAux.Type != M4CL_CPP_TYPE_NUMBER )
							{
								DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostSrcItemHandle)<<"Variant Aux Item"<<"ApplyValue (2)"  <<M4ClCppType(M4CL_CPP_TYPE_NUMBER) << M4ClCppType(vAux.Type) << DumpCallStack() );
								RuntimePushDouble( M4_ERROR ) ;
								return( M4_ERROR ) ;
							}
							
							iHostRealM4Type = m4uint8_t( vAux.Data.DoubleData ) ;
							
							if( iHostM4Type == M4CL_M4_TYPE_VARIANT && vValue.Type==M4CL_CPP_TYPE_STRING_VAR)
							{
								switch( iHostRealM4Type )
								{
									case M4CL_M4_TYPE_NUMBER :
									case M4CL_M4_TYPE_CURRENCY :
										
										vValue.Type = M4CL_CPP_TYPE_NUMBER ;
										vValue.Data.DoubleData = atof( vValue.Data.PointerChar ) ;
										break ;
										
									/* Bug 0102160
									La conversión de tipos también para fechas
									*/
									case M4CL_M4_TYPE_DATE :
									case M4CL_M4_TYPE_TIMESTAMP :
									case M4CL_M4_TYPE_HOUR :
										
										vValue.Type = M4CL_CPP_TYPE_DATE ;
										vValue.Data.DoubleData = ClStringToDate( vValue.Data.PointerChar ) ;
										break ;
										
									default :
										break ;
								}
							}
						}
						else
						{
							iHostRealM4Type = iHostM4Type ;
							iHostSrcItemHandle = iHostItemHandle ;
						}
						

						// Prorrateo ========================================================================================
						
						if( vValue.Type == M4CL_CPP_TYPE_NUMBER )
                        {
                            switch( iTargetSliceBhrv )
                            {
								case M4CL_ITEM_SLICE_BHRV_FINAL_VALUE :
								case M4CL_ITEM_SLICE_BHRV_UNIT :
									
									vValue.Data.DoubleData = vValue.Data.DoubleData * dCutRate ;
									break ;
									
								default :
									break ;
									
                            }
						}

						
						// Multimoneda ======================================================================================
						
						if( *acDestCurrencyArg != '\0' )
						{
							if( iHostRealM4Type == M4CL_M4_TYPE_CURRENCY && vValue.Type == M4CL_CPP_TYPE_NUMBER )
							{
								// moneda origen
								iHostAuxItemHandle = poHostCMCR->GetItemAuxItem( iHostSrcItemHandle ) ;
								
								if( iHostAuxItemHandle == 0 )
								{
									g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"Currency Aux Item"
																					<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
																					<<"ApplyValue (2)"<< DumpCallStack() << EndLog;
									RuntimePushDouble( M4_ERROR ) ;
									return( M4_ERROR ) ;
								}
								
								iHostAuxItemIndex = poHostCMCR->GetItemIndex( iHostAuxItemHandle ) ;
								
								
								iResult = poDHostRecordSet->Register[ m ].Item[ ClItemIndex( iHostAuxItemIndex ) ].Value.Get( vSrcCurrency ) ;
								
								if( iResult != M4_SUCCESS )
								{
									DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<< "Currency Aux Item"<<"ApplyValue (2)" << DumpCallStack() );
									RuntimePushDouble( M4_ERROR ) ;
									return( M4_ERROR ) ;
								}
								
								if( IsNull( vSrcCurrency ) )
								{
									DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Currency Aux Item"<<"ApplyValue (2)" << DumpCallStack() );
									RuntimePushDouble( M4_ERROR ) ;
									return( M4_ERROR ) ;
								}
								
								if( vSrcCurrency.Type != M4CL_CPP_TYPE_STRING_VAR )
								{
									DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_ITEM_AUX,M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Currency Aux Item"<<"ApplyValue (2)"  <<M4ClCppType(M4CL_CPP_TYPE_STRING_VAR) << M4ClCppType(vSrcCurrency.Type) << DumpCallStack() );
									RuntimePushDouble( M4_ERROR ) ;
									return( M4_ERROR ) ;
								}
								
								// fecha de cambio
								iHostAuxItemHandle = poHostCMCR->GetItemAuxItem( iHostAuxItemHandle ) ;
								
								if( iHostAuxItemHandle != 0 )
								{
									iHostAuxItemIndex = poHostCMCR->GetItemIndex( iHostAuxItemHandle ) ;
									
									
									iResult = poDHostRecordSet->Register[ m ].Item[ ClItemIndex( iHostAuxItemIndex ) ].Value.Get( vAux ) ;
									
									if( iResult != M4_SUCCESS )
									{
										DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Exchange Date"<<"ApplyValue (2)" << DumpCallStack() );
										RuntimePushDouble( M4_ERROR ) ;
										return( M4_ERROR ) ;
									}
									
									if( IsNull( vAux ) )
									{
										if( dExchangeDateArg != 0 )
										{
											dExchangeDate = dExchangeDateArg ;
										}
										else
										{
											DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Exchange Date"<<"ApplyValue (2)" << DumpCallStack() );
											RuntimePushDouble( M4_ERROR ) ;
											return( M4_ERROR ) ;
										}
									}
									else 
									{
										if( vAux.Type != M4CL_CPP_TYPE_DATE )
										{
											DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_ITEM_AUX,M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Exchange Date"<< "ApplyValue (2)"  << M4ClCppType(M4CL_CPP_TYPE_DATE) << M4ClCppType(vAux.Type) <<DumpCallStack() );
											RuntimePushDouble( M4_ERROR ) ;
											return( M4_ERROR ) ;
										}
										
										dExchangeDate = vAux.Data.DoubleData ;
									}
								}
								else if( dExchangeDateArg != 0 )
								{
									dExchangeDate = dExchangeDateArg ;
								}
								else
								{
									g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"Exchange Date"
																					<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
																					<<"ApplyValue (2)"<< DumpCallStack() << EndLog;
									RuntimePushDouble( M4_ERROR ) ;
									return( M4_ERROR ) ;
								}

								// tipo de cambio
								if( iHostAuxItemHandle != 0 ) //si la fecha de cambio la leimos del item auxiliar, intentamos leer el tipo, sino vamos por los argumentos
								{
									iHostAuxItemHandle = poHostCMCR->GetItemAuxItem( iHostAuxItemHandle ) ;
								}

								
								if( iHostAuxItemHandle != 0 )
								{
									iHostAuxItemIndex = poHostCMCR->GetItemIndex( iHostAuxItemHandle ) ;
									
									
									iResult = poDHostRecordSet->Register[ m ].Item[ ClItemIndex( iHostAuxItemIndex ) ].Value.Get( vAux ) ;
									
									if( iResult != M4_SUCCESS )
									{
										DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Exchange Type"<<"ApplyValue (2)" << DumpCallStack() );
										RuntimePushDouble( M4_ERROR ) ;
										return( M4_ERROR ) ;
									}
									
									if( IsNull( vAux ) )
									{
										if( *acExchangeTypeArg != '\0' )
										{
											pccExchangeType = acExchangeTypeArg ;
										}
										else
										{
											DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Exchange Type"<<"ApplyValue (2)" << DumpCallStack() );
											RuntimePushDouble( M4_ERROR ) ;
											return( M4_ERROR ) ;
										}
									}
									else 
									{
										if( vAux.Type != M4CL_CPP_TYPE_STRING_VAR )
										{
											DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_ITEM_AUX, M4ObjNodeItemI(poHostCMCR,iHostAuxItemHandle)<<"Exchange Type"<< "ApplyValue (2)"  <<M4ClCppType(M4CL_CPP_TYPE_STRING_VAR) << M4ClCppType(vAux.Type) << DumpCallStack() );
											RuntimePushDouble( M4_ERROR ) ;
											return( M4_ERROR ) ;
										}
										
										pccExchangeType = vAux.Data.PointerChar ;
									}
								}
								else if( *acExchangeTypeArg != '\0' )
								{
									pccExchangeType = acExchangeTypeArg ;
								}
								else
								{
									g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"Exchange Type"
																					<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
																					<<"ApplyValue (2)"<< DumpCallStack() << EndLog;
									RuntimePushDouble( M4_ERROR ) ;
									return( M4_ERROR ) ;
								}


								dCutRate = 1 ;
								
								iResult = _curGetExchangeRate( vSrcCurrency.Data.PointerChar, acDestCurrencyArg, pccExchangeType, dExchangeDate, dCutRate, bExchangeFound, NULL, poCurrencyAccess ) ;
								
								if( iResult != M4_SUCCESS  )
								{
									DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_EXCHANGE, vSrcCurrency.Data.PointerChar<<acDestCurrencyArg<<dExchangeDate<<"ApplyValue (2)" << DumpCallStack() );
									RuntimePushDouble( M4_ERROR ) ;
									return( M4_ERROR ) ;
								}
								
								vValue.Data.DoubleData = vValue.Data.DoubleData * dCutRate ;
							}
						}
						
						
						// Asignación =======================================================================================
						
                        if( iHostPriorityHandle != 0 )
                        {
                            iResult = poTargetCurrentRegister->Item[ iTargetItemIndex ].Value[ dSliceDate ].Set( vValue, oPriorSlice.m_iPrior, bDone ) ;

							iPriority = oPriorSlice.m_iPrior ;
							bAssigned = bDone ;
                        }
                        else
                        {
                            iResult = poTargetCurrentRegister->Item[ iTargetItemIndex ].Value[ dSliceDate ].Set( vValue ) ;

							iPriority = poTargetCurrentRegister->Item.Value.GetPriority() ;
							bAssigned = M4_TRUE ;
                        }
						
                        if( iResult != M4_SUCCESS )
                        {
							DUMP_CHLOG_ERRORF(M4_CH_VM_WRITING_SLICE, M4ObjNodeItemI(poTargetCMCR,iTargetItemHandle)<<DateLog(dSliceDate)<<DateLog(dSliceDate)<<"ApplyValue (2)" << DumpCallStack() );
							RuntimePushDouble( M4_ERROR ) ;
                            return( M4_ERROR ) ;
                        }
						else
						{
							if( GetVMCSContext()->GetSttWrap()->GetStatisticsLevel()  & M4_VM_STATISTICS_ENABLED_VALUES)
							{
								GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddValue( poTargetCurrentRegister->Item, vValue, iPriority, bAssigned, 2, m_oState1, DumpCallStack( &m_oState1, M4_TRUE ).c_str() );
							}
						}
                    }
                }
				
				// compactamos los tramos
				
                poTargetCurrentRegister->Item[ iTargetItemIndex ].Value.Compact() ;
				
                if( iResult != M4_SUCCESS )
                {
					DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, M4ObjNodeItemI(poTargetCMCR,iTargetItemHandle)<<"ApplyValue (2)" << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
                    return( M4_ERROR ) ;
                }
            }
			
			
			// despues de volcar
			
			m_oNodeSlicesStack.Clear( dPhase1Start, dPhase1End, dTimeUnit ) ;
			
            iLength = strlen( pccHostDmdComponent ) ;
			
            if( iLength > M4CL_MAX_DMD_COMPONENT_ID )
            {
                iLength = M4CL_MAX_DMD_COMPONENT_ID ;
            }
			
            memcpy( acLastDmdComponent, pccHostDmdComponent, sizeof( m4char_t) * iLength ) ;
            *( acLastDmdComponent + iLength ) = '\0' ;

			//pasamos a mayusculas
			strupr(acLastDmdComponent);
			
            iStart = i ;
        }
		
        if( i != iHostRegisters )
        {
            iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( iHostStartDateIndex ) ].Value.Get( vValue ) ;
			
            if( iResult != M4_SUCCESS )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poHostCMCR,iHostStartDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
                return( M4_ERROR ) ;
            }
            
            if( IsNull( vValue ) )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL, M4ObjNodeItemI(poHostCMCR,iHostStartDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
                return( M4_ERROR ) ;
            }
			
            if( vValue.Type != M4CL_CPP_TYPE_DATE )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE, M4ObjNodeItemI(poHostCMCR,iHostStartDateHandle)<<"Start Date"  << "ApplyValue (2)"  << DumpCallStack() );
                RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
            }
			
			
            dStart = vValue.Data.DoubleData ;
			
            iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( iHostEndDateIndex ) ].Value.Get( vValue ) ;
			
            if( iResult != M4_SUCCESS )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poHostCMCR,iHostEndDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
                return( M4_ERROR ) ;
            }
			
            if( IsNull( vValue ) )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL, M4ObjNodeItemI(poHostCMCR,iHostEndDateHandle)<<"ApplyValue (2)" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
                return( M4_ERROR ) ;
            }
			
            if( vValue.Type != M4CL_CPP_TYPE_DATE )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE, M4ObjNodeItemI(poHostCMCR,iHostEndDateHandle)<<"End Date"<< "ApplyValue (2)"  << DumpCallStack() );
                RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
            }
			
			
            dEnd = vValue.Data.DoubleData ;
			
			if(iHostPriorityHandle != 0){ // si tiene prioridad la leemos

				iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( iHostPriorityIndex ) ].Value.Get( vValue ) ;
				
				if( iResult != M4_SUCCESS )
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poHostCMCR,iHostPriorityHandle)<<"ApplyValue (2)" << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				if( IsNull( vValue ) )
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL, M4ObjNodeItemI(poHostCMCR,iHostPriorityHandle)<<"ApplyValue (2)" << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE, M4ObjNodeItemI(poHostCMCR,iHostPriorityHandle)<<"Priority"<< "ApplyValue (2)"  << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				iHostPriority = m4uint8_t( vValue.Data.DoubleData ) ;
			}
			else{
				iHostPriority = M4CL_CM_DEFAULT_PRIOR;

			}
			
			iResult=m_oNodeSlicesStack.AddSlice( ClPriorSlice(dStart, dEnd, iHostPriority) ) ;

			
#ifdef _DEBUG
			if( iResult != M4_SUCCESS )
			{
				RuntimePushDouble( M4_ERROR ) ;
			}
#endif

			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_VM_INTERNAL_ERROR,  __FILE__ <<__LINE__<<"applyValue (2)") ;

			
        }
    }
	
    RuntimePushDouble( M4_SUCCESS ) ;
    return( M4_SUCCESS ) ;
}



//-----------------------------------------------------
//F U N C I O N E S  D E  P I V O T A C I O N
//-----------------------------------------------------

// Van aquí porque son casi como el aplicavalor 2

m4return_t  ClVM1PayrollLib :: pvPivot( void )
{
	
    m4return_t          iResult ;
	
    m4uint8_t           iSameChannel ;
    
	m4int16_t			iArgumentNumber ;
    m4uint16_t          j, k ;
    m4uint16_t          p, q ;
    m4uint16_t          iPkElementNumber ;
	
	m4int32_t			iRegister ;
    m4uint32_t          i ;
	m4uint32_t			iItemHandle ;
	m4uint16_t			iItemIndex ;
	
    m4pcchar_t          pccItemId ;
    m4pchar_t           pcHostChannelIdArg ;
    m4pcchar_t          pccHostIdArg ;
    m4pcchar_t          pccTargetDmdArg ;
	
    m4char_t            acTempString[ M4CL_MCR_MAX_TEMP_STRING + 1 ] ;
	
    m4VariantType       vValue, vAuxValue ;
    
	
	// ==========================================================
	// variables del host =======================================
	// ==========================================================
	
    m4uint8_t           iHostType ;
    m4uint8_t           iHostScope ;
    m4uint16_t          iHostItems ;
    m4uint16_t          iHostComponentIndex ;
    m4uint32_t          iHostRegisters ;
    m4uint32_t          iHostHandle ;
    m4uint32_t          iHostComponentHandle ;
    m4uint32_t          iHostItemHandle ;
    m4uint32_t          iHostIndexId ;
    m4uint32_t          iHostIndexHandle ;
    m4pcchar_t          pccHostDmdComponent ;
    m4pcchar_t          pccHostItemId ;
    m4pcchar_t          pccHostInstance ;
	
    ClItemIndex         aiHostPKIndexes[ M4CL_MAX_PIVOT_PK_ELEMENTS ] ;
    m4VariantType       avHostPkValues[ M4CL_MAX_PIVOT_PK_ELEMENTS ] ;
	
    ClCompiledMCR       *poHostCMCR ;
    ClNode              *poDHostNode ;
    ClRecordSet         *poDHostRecordSet ;
    ClAccess            *poHostAccess ;
	
	
	// ==========================================================
	// variables del target =====================================
	// ==========================================================
	
    m4uint8_t           iTargetNewRegister ;
    m4uint16_t          iTargetItems ;
    m4uint32_t          hTargetNode ;
    m4uint32_t          iTargetItemHandle ;
    m4pcchar_t          pccTargetDmd ;
    m4pcchar_t          pccTargetDmdComponent ;
    m4pcchar_t          pccTargetDmdField ;
    m4pcchar_t          pccTargetChannel ;
	
	ClItemIndex			iTargetItemIndex ;
    ClItemIndex         aiTargetPKIndexes[ M4CL_MAX_PIVOT_PK_ELEMENTS ] ;
    m4VariantType		aiTargetPKTrios[ M4CL_MAX_PIVOT_PK_ELEMENTS * 3 ] ;  //para el find
	
    ClCompiledMCR       *poTargetCMCR ;
    ClAccess            *poTargetAccess ;
    ClAccessRecordSet   *poTargetRecordSet ;
	
	
	// ==========================================================
	// inicializacion ===========================================
	// ==========================================================
	
    //set RS
    
	
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1,"Trace VM1: Pivot");
#endif
	
	// ==========================================================
	// argumentos de la pila ====================================
	// ==========================================================
	
	// Número de argumentos
	
    iResult = RuntimePopDouble( vValue ) ;
    DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Pivot" ) ;
	
    iArgumentNumber = m4int16_t( vValue.Data.DoubleData ) ;
	
    if( iArgumentNumber != 3 && iArgumentNumber != 4 )
    {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "Pivot"<<"3-4" << DumpCallStack() );
        return( M4_ERROR ) ;
    }
	
	//HostIndexId (argumento 4 - opcional)
    if( iArgumentNumber == 4 )
    {
        iResult = RuntimePopDouble( vValue ) ;
        DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "HostIndexId" << "Pivot" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
		
        iHostIndexId = m4uint32_t( vValue.Data.DoubleData ) ;
    }
    else
    {
        iHostIndexId = 0 ;
    }
	
	//TargetDMD (argumento 3)
	
    iResult = RuntimePopString( vValue ) ;
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "TargetDMD" << "Pivot" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
	
    pccTargetDmdArg = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
	M4CHECK_RES( pccTargetDmdArg!=NULL, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "TargetDMD" << "Pivot" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(M4CL_CPP_TYPE_NULL)<<DumpCallStack() );

    m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
	
	
	//HostId (Argumento 2)
	
    iResult = RuntimePopString( vValue ) ;
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "HostId" << "Pivot" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
	
    pccHostIdArg = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
    M4CHECK_RES( pccHostIdArg!=NULL, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "HostId" << "Pivot" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(M4CL_CPP_TYPE_NULL)<<DumpCallStack() );

    m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
	
	
	//ChannelId (argumento 1)
	
    iResult = RuntimePopString( vValue );
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ChannelId" << "Pivot" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
	
    pcHostChannelIdArg = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
    M4CHECK_RES( pcHostChannelIdArg!=NULL, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ChannelId" << "Pivot" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(M4CL_CPP_TYPE_NULL)<<DumpCallStack() );

    m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
	
	
    /*Este control de errores lo pongo en cada uno de los parametros antes.
	if( pccTargetDmdArg == NULL || pccHostIdArg == NULL || pcHostChannelIdArg == NULL )
    {
        SET CODE( M4_CH_VM_BAD_POINTER, ERRORLOG, "Null pointer in Pivot" ) ;
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
    }*/
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1, BeginLogCat<<"Trace VM1: Pivot. Channel:<"<<pcHostChannelIdArg<<"> Host Id:<"<<pccHostIdArg<<"> TargetDMD: <"<<pccTargetDmdArg<<">" << EndLogCat);
#endif
	
	// ==========================================================
	// asignación de variables del target =======================
	// ==========================================================
	
    poTargetAccess = m_oState1.m_poAccess ;
	
    poTargetCMCR = poTargetAccess->GetpCMCR() ;
	
#ifdef _DEBUG
	if( poTargetCMCR == NULL )
	{
		g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"Null mcr in Pivot. Channel TargetCMCR"<<EndLog;
        RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}
#endif
	
    pccTargetChannel = poTargetCMCR->GetChannelId() ;
	
    hTargetNode = m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle() ;
	
    poTargetRecordSet = m_oState1.m_poRecordSet ;
	
    iTargetItems = poTargetCMCR->GetNodeNumberOfDmds( hTargetNode ) ;
	
	
	// ==========================================================
	// asignación de variables del host =========================
	// ==========================================================
	
    if( *pcHostChannelIdArg == '\0' || strcmpi( pcHostChannelIdArg, pccTargetChannel ) == 0 )
    {
        iSameChannel = 1 ;
        poHostAccess = m_oState1.m_poAccess ;
    }
    else
    {
        iSameChannel = 0 ;
		
        pccHostInstance = poTargetCMCR->GetNodeT3AliasInstanceIdByAlias( hTargetNode, pcHostChannelIdArg ) ;
		
        if( pccHostInstance == NULL )
        {
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_INSTANCE, pcHostChannelIdArg<<poTargetCMCR->iNodeN(hTargetNode)<<LogCatAnd<<poTargetCMCR->nNodeN(hTargetNode)<<"Pivot" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
            return( M4_ERROR ) ;
        }
		

		iResult = m_poVM->GetL2Access( poTargetAccess->GetpChannel(), pccHostInstance, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState1.m_Flags), &poHostAccess ) ;		
        if( iResult != M4_SUCCESS )
        {
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_GET_ACCESS_L2, pccHostInstance<<"Pivot" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
            return( M4_ERROR ) ;
        }
    }
	
    poHostCMCR = poHostAccess->GetpCMCR() ;
	
#ifdef _DEBUG
	if( poHostCMCR == NULL )
	{
		g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"Null mcr in Pivot. Channel: HostCMCR"<<EndLog;
        RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}
#endif
	
    if( iSameChannel == 1 )
    {
        iHostHandle = poHostCMCR->GetNodeAliasNodeHandleByAlias( hTargetNode, pccHostIdArg, m_oState1.m_poFormula.GetLevel() ) ;
    }
    else
    {
        iHostHandle = poHostCMCR->GetChannelNodeHandleById( pccHostIdArg ) ;
    }
	
	if( iHostHandle == 0 )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, pccHostIdArg <<"Pivot" << DumpCallStack() );
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}
	
    iHostComponentHandle = poHostCMCR->GetNodeDmdComponentItem( iHostHandle ) ;
	
	if( iHostComponentHandle == 0 )
	{
		g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"Dmd Component"
														<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
														<<"Pivot"<< DumpCallStack() << EndLog;
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}
	
	
    iHostItems = poHostCMCR->GetNodeNumberOfItems( iHostHandle ) ;
    iHostComponentIndex = poHostCMCR->GetItemIndex( iHostComponentHandle ) ;
	
	
    poDHostNode = &( poHostAccess->Node[ ClHandle( iHostHandle ) ] ) ;
	
#ifdef _DEBUG
	if( poDHostNode == NULL )
	{
		g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"Pivot. Null pointer for node with handle:"<<LogCat<<iHostHandle<<EndLog;
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}
#endif
	
    poDHostRecordSet = &( poDHostNode->RecordSet ) ;
    
	if( !poDHostRecordSet->IsValid() )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK, "Pivot"<<poDHostRecordSet->GetpNodeDef()->Id()<<LogCatAnd<<poDHostRecordSet->GetpNodeDef()->Name()<<DumpTreePath(poDHostRecordSet->GetpAccess(), iHostHandle) << DumpCallStack() );
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}
	
    iHostRegisters = poDHostRecordSet->Register.Count() ;
	
    
	// ==========================================================
	// algoritmo ================================================
	// ==========================================================
	
	// ordenacion
	
    if( iHostIndexId == 0 )
    {
        iHostIndexHandle = poHostCMCR->GetNodeDefaultIndex( iHostHandle ) ;
		
        if( iHostIndexHandle == 0 )
        {
            DUMP_CHLOG_ERRORF(M4_CH_VM_NO_DEFAULT_INDEX, M4ObjNodeN(poHostCMCR,iHostHandle)<<"Pivot" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
            return( M4_ERROR ) ;
        }
		
		iHostIndexId = poHostCMCR->GetIndexId( iHostIndexHandle ) ;
    }
	else
	{
		iHostIndexHandle = poHostCMCR->GetNodeIndexHandleById( iHostHandle, iHostIndexId ) ;
		
		if( iHostIndexHandle == 0 )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONGINDEX, iHostIndexId<<M4ObjNodeN(poHostCMCR,iHostHandle ) <<"Pivot" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}
	}
	
	
    iResult = poDHostRecordSet->SetCurrentSearchIndexById( iHostIndexId ) ;
	
    if( iResult != M4_SUCCESS )
    {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONGINDEX, (m4uint32_t) iHostIndexId <<M4ObjNodeN(poHostCMCR,iHostHandle ) <<"Pivot" << DumpCallStack() );
        RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
    }
	
    iPkElementNumber = poHostCMCR->GetIndexNumberOfItems( iHostIndexHandle ) ;
	
    if( iPkElementNumber > M4CL_MAX_PIVOT_PK_ELEMENTS )
    {
        DUMP_CHLOG_ERRORF(M4_CH_VM_TOO_MANY_PKS, iPkElementNumber<<M4CL_MAX_PIVOT_PK_ELEMENTS<<"Pivot" << DumpCallStack() );
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
    }
	
    for( p = 0, q = 0 ; p < iPkElementNumber ; p++, q += 3 )
    {
        iItemHandle = poHostCMCR->GetIndexItemHandleByPosition( iHostIndexHandle, p ) ;
        iItemIndex = poHostCMCR->GetItemIndex( iItemHandle ) ;
		
        aiHostPKIndexes[ p ] = iItemIndex ;
		
        avHostPkValues[ p ].Type = M4CL_CPP_TYPE_NULL ;
        avHostPkValues[ p ].Data.DoubleData = 0 ;
        avHostPkValues[ p ].Data.PointerChar = NULL ;
		
        pccItemId = poHostCMCR->GetItemId( iItemHandle ) ;
        iItemHandle = poTargetCMCR->GetNodeItemHandleById( hTargetNode, pccItemId ) ;
		
        if( iItemHandle == 0 )
        {
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, pccItemId<<M4ObjNodeN(poTargetCMCR,hTargetNode)<<"Pivot" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
            return( M4_ERROR ) ;
        }
		
        iItemIndex = poTargetCMCR->GetItemIndex( iItemHandle ) ;
		
        aiTargetPKIndexes[ p ] = iItemIndex ;
		
		aiTargetPKTrios[ q ].Type = M4CL_CPP_TYPE_NUMBER ;
		aiTargetPKTrios[ q ].Data.DoubleData = iItemIndex ;
		
		aiTargetPKTrios[ q + 2 ].Type = M4CL_CPP_TYPE_NUMBER ;
		aiTargetPKTrios[ q + 2 ].Data.DoubleData = M4CL_EQUAL_FUNCTION ;
    }
	
	
    for( i = 0 ; i < iHostRegisters ; i++ )
    {
		if( i == 0 )
		{
			iTargetNewRegister = 1 ;
		}
		else
		{
			iTargetNewRegister = 0 ;
		}
		
        for( p = 0 ; p < iPkElementNumber ; p++ )
        {
            iResult = poDHostRecordSet->Register[ i ].Item[ aiHostPKIndexes[ p ] ].Value.Get( vValue ) ;
			
            if( iResult != M4_SUCCESS )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, (m4uint32_t) aiHostPKIndexes[p]<<M4ObjNodeN(poHostCMCR,iHostHandle)<<"Pivot" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
                return( M4_ERROR ) ;
            }
			
            if( M4VariantIsEqual( &vValue, &avHostPkValues[ p ] ) == 0 )
            {
				avHostPkValues[ p ].Type = vValue.Type ;
				avHostPkValues[ p ].Data.DoubleData = vValue.Data.DoubleData ;
				avHostPkValues[ p ].Data.PointerChar = vValue.Data.PointerChar ;
				
                iTargetNewRegister = 1 ;
            }
        }
		
        if( iTargetNewRegister == 1 )
        {
			// hay que intentar encontrarlo
			
			for( p = 0, q = 0 ; p < iPkElementNumber ; p++, q += 3 )
			{
				aiTargetPKTrios[ q + 1 ].Type = avHostPkValues[ p ].Type ;
				aiTargetPKTrios[ q + 1 ].Data.DoubleData = avHostPkValues[ p ].Data.DoubleData ;
				aiTargetPKTrios[ q + 1 ].Data.PointerChar = avHostPkValues[ p ].Data.PointerChar ;
			}
			
            iResult = poTargetRecordSet->Register.FindSec( aiTargetPKTrios, q, &iRegister ) ;
			
            if( iResult != M4_SUCCESS )
            {
                DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_IN_FIND, M4ObjNodeN(poTargetRecordSet->GetpAccess()->GetpCMCR(),poTargetRecordSet->GetpNodeDef()->GetHandle())<<"Pivot" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
                return( M4_ERROR ) ;
            }
			
            if( iRegister != - 1 )
            {
				// hacer el MoveTo
				poTargetRecordSet->Current.MoveTo( ClRegisterIndex( iRegister ) ) ;
            }
            else
            {
				iResult = poTargetRecordSet->Current.Add() ;
				
				if( iResult != M4_SUCCESS )
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_CANT_ADD_RECORD, M4ObjNodeN(poTargetCMCR,hTargetNode)<<"Pivot"<< DumpTreePath( poTargetRecordSet->GetpAccess(), hTargetNode )  << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				// copiamos pk
				
				for( p = 0 ; p < iPkElementNumber ; p++ )
				{
					iResult = poTargetRecordSet->Current.Item[ aiTargetPKIndexes[ p ] ].Value.Set( avHostPkValues[ p ] ) ;
					
					if( iResult != M4_SUCCESS )
					{
						DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, M4ObjNodeItemI(poTargetCMCR,poTargetRecordSet->Current.Item.ItemDef.GetHandle())<<"Pivot" << DumpCallStack() );
						RuntimePushDouble( M4_ERROR ) ;
						return( M4_ERROR ) ;
					}
				}
			}
        }
		
		// Pivotamos
		iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( iHostComponentIndex ) ].Value.Get( vValue ) ;
		
		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poHostCMCR,iHostComponentHandle)<<"Pivot" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}
		
		if( IsNull( vValue ) )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_IS_NULL, M4ObjNodeItemI(poHostCMCR,iHostComponentHandle)<<"Pivot" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}
		
		if( vValue.Type == M4CL_CPP_TYPE_STRING_VAR )
		{
			pccHostDmdComponent = vValue.Data.PointerChar ;
		}
		else if( vValue.Type == M4CL_CPP_TYPE_NUMBER )
		{
			sprintf( acTempString, "%.0f", vValue.Data.DoubleData ) ;
			pccHostDmdComponent = acTempString ;
		}
		else
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE, M4ObjNodeItemI(poHostCMCR,iHostComponentHandle)<<"DmdComponent" << "Pivot"  << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}
		
		
		for( j = 0 ; j < iTargetItems ; j++ )
		{
			pccTargetDmd = poTargetCMCR->GetNodeDmdDmdIdByPosition( hTargetNode, j ) ;
			
#ifdef _DEBUG
			if( pccTargetDmd == NULL )
			{
				g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"Pivot.  Dmd. Target Item Dmd:"<<LogCat<<j<<EndLog;
				RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
			}
#endif
			
			if( *pccTargetDmdArg != 0 && strcmpi( pccTargetDmd, pccTargetDmdArg ) != 0 )
			{
				continue ;
			}
			
			pccTargetDmdComponent = poTargetCMCR->GetNodeDmdDmdComponentIdByPosition( hTargetNode, j ) ;
			
#ifdef _DEBUG
			if( pccTargetDmdComponent == NULL )
			{
				g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"Pivot.  Dmd Component. Target Item Dmd:"<<LogCat<<j<<EndLog;
				RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
			}
#endif
			
			if( strcmpi( pccTargetDmdComponent, pccHostDmdComponent ) != 0 )
			{
				continue ;
			}
			
			// es un componente que hace matching
			
			pccTargetDmdField = poTargetCMCR->GetNodeDmdDmdFieldIdByPosition( hTargetNode, j ) ;
			
#ifdef _DEBUG
			if( pccTargetDmdField == NULL )
			{
				g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"Pivot.  Dmd Field. Target Item Dmd:"<<LogCat<<j<<EndLog;
				RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
			}
#endif
			
			iTargetItemHandle = poTargetCMCR->GetNodeDmdHandleByPosition( hTargetNode, j ) ;
			iTargetItemIndex = poTargetCMCR->GetItemIndex( iTargetItemHandle ) ;
			
			for( k = 0 ; k < iHostItems ; k++ )
			{
				iHostItemHandle = poHostCMCR->GetNodeItemHandleByPosition( iHostHandle, k ) ;
				
#ifdef _DEBUG
				if( iHostItemHandle == 0 )
				{
					g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"HostItem["<<LogCat<<k<<LogCat<<"]"
																	<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
																	<<"Pivot"<< DumpCallStack() << EndLog;
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
#endif
				
				iHostType = poHostCMCR->GetItemType( iHostItemHandle ) ;
                iHostScope = poHostCMCR->GetItemScope( iHostItemHandle ) ;
				
                if( iHostType == M4CL_ITEM_TYPE_METHOD || iHostScope != M4CL_ITEM_SCOPE_REGISTER || k == iHostComponentIndex )
				{
					continue ;
				}
				
				// es un item host valido
				
				pccHostItemId = poHostCMCR->GetItemId( iHostItemHandle ) ;
				
#ifdef _DEBUG
				if( pccHostItemId == NULL )
				{
                    RuntimePushDouble( M4_ERROR ) ;
				}
#endif

				CHECK_CHLOG_DEBUGF( pccHostItemId == NULL, M4_ERROR, M4_CH_VM_INTERNAL_ERROR, __FILE__<<__LINE__ << k ) ;

				
				if( strcmpi( pccTargetDmdField, pccHostItemId ) != 0 )
				{
					continue ;
				}
				
				// es un item al que hay que generar valor
				
				iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( k ) ].Value.Get( vValue ) ;
				
				if( iResult != M4_SUCCESS )
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, k<< M4ObjNodeN(poHostCMCR,iHostHandle)<<"Pivot" << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				iResult = poTargetRecordSet->Current.Item[ iTargetItemIndex ].Value.Get( vAuxValue ) ;
				
				if( iResult != M4_SUCCESS )
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, iTargetItemIndex<<M4ObjNodeN(poTargetCMCR,hTargetNode)<<"Pivot" << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				if( M4VariantIsEqual( &vValue, &vAuxValue ) == 0 )
				{
					iResult = poTargetRecordSet->Current.Item[ iTargetItemIndex ].Value.Set( vValue ) ;
					
					if( iResult != M4_SUCCESS )
					{
						DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, M4ObjNodeItemI(poTargetCMCR,poTargetRecordSet->Current.Item.ItemDef.GetHandle())<<"Pivot" << DumpCallStack() );
						RuntimePushDouble( M4_ERROR ) ;
						return( M4_ERROR ) ;
					}
					else
					{
						if( GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_VALUES)
						{
							ClItem	&item = poTargetRecordSet->Current.Item;
							GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddValue( item, vValue, item.Value.GetPriority(), M4_TRUE, 0, m_oState1, DumpCallStack( &m_oState1, M4_TRUE ).c_str() ) ;
						}
					}
				}
			}
		}
	}
	
	RuntimePushDouble(M4_SUCCESS);
    return( M4_SUCCESS ) ;
}




m4return_t ClVM1PayrollLib :: pvUnPivot( void )
{
	::MessageBox( NULL, "This Function is not implemented yet. pvUnPivot", "Fatal Error", MB_OK ) ;
    return( M4_SUCCESS ) ;
}


m4return_t ClVM1PayrollLib :: pvCrossTab( void )
{
	
    m4return_t          iResult ;
	
    m4uint8_t           iSameChannel ;
    
	m4uint16_t			iArgumentNumber ;
	m4uint16_t			iPoped ;
	m4uint16_t			iElement ;
    m4uint16_t          j, k ;
    m4uint16_t          p, q ;
    m4uint16_t          iPkElementNumber ;
	m4uint32_t			iItemHandle ;
	m4uint16_t			iItemIndex ;
	
	m4int32_t			iRegister ;
    m4uint32_t          i ;
    m4uint32_t          iLength ;
    m4uint32_t          iActualLength ;
	
    m4pcchar_t          pccItemId ;
    m4pchar_t           pcHostChannelIdArg ;
    m4pcchar_t          pccHostIdArg ;
	
    m4char_t            acTempString[ M4CL_MCR_MAX_TEMP_STRING + 1 ] ;
    m4char_t            acColumnString[ M4CL_MAX_CROSSTAB_COLUMN_ID + 1 ] ;
	
    m4VariantType       vValue, vAuxValue ;
    
	ClItemIndex			aiColumnIndexes[ M4CL_MAX_CROSSTAB_COLUMNS ] ;
    m4pcchar_t			apcColumns     [ M4CL_MAX_CROSSTAB_COLUMNS ] ;
	
	
	// ==========================================================
	// variables del host =======================================
	// ==========================================================
	
    m4uint8_t           iHostType ;
    m4uint16_t          iHostItems ;
    m4uint32_t          iHostRegisters ;
    m4uint32_t          iHostHandle ;
    m4uint32_t          iHostItemHandle ;
    m4uint32_t          iHostIndexId ;
    m4uint32_t          iHostIndexHandle ;
    m4pcchar_t          pccHostColumn ;
    m4pcchar_t          pccHostDmdComponent ;
    m4pcchar_t          pccHostItemId ;
    m4pcchar_t          pccHostInstance ;
	
    ClItemIndex         aiHostPKIndexes[ M4CL_MAX_PIVOT_PK_ELEMENTS ] ;
    m4VariantType       avHostPkValues[ M4CL_MAX_PIVOT_PK_ELEMENTS ] ;
	
    ClCompiledMCR       *poHostCMCR ;
    ClNode              *poDHostNode ;
    ClRecordSet         *poDHostRecordSet ;
    ClAccess            *poHostAccess ;
	
	
	// ==========================================================
	// variables del target =====================================
	// ==========================================================
	
    m4uint8_t           iTargetType ;
    m4uint8_t           iTargetNewRegister ;
    m4uint16_t          iTargetItems ;
    m4uint32_t          hTargetNode ;
    m4uint32_t          iTargetItemHandle ;
    m4pcchar_t          pccTargetDmdComponent ;
    m4pcchar_t          pccTargetDmdField ;
    m4pcchar_t          pccTargetChannel ;
	
    ClItemIndex         aiTargetPKIndexes[ M4CL_MAX_PIVOT_PK_ELEMENTS ] ;
    m4VariantType		aiTargetPKTrios[ M4CL_MAX_PIVOT_PK_ELEMENTS * 3 ] ;  //para el find
	
    ClCompiledMCR       *poTargetCMCR ;
    ClAccess            *poTargetAccess ;
    ClAccessRecordSet   *poTargetRecordSet ;
	
	
	// ==========================================================
	// inicializacion ===========================================
	// ==========================================================
	
    //set RS
    
	
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1,"Trace VM1: CrossTab");
#endif
	
	// ==========================================================
	// argumentos de la pila ====================================
	// ==========================================================
	
	// Número de argumentos
	
    iResult = RuntimePopDouble( vValue ) ;
    DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "CrossTab" ) ;
	
    iArgumentNumber = m4int16_t( vValue.Data.DoubleData ) ;
	
    if( iArgumentNumber < 3 )
    {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "CrossTab"<<"3-" << DumpCallStack() );
        return( M4_ERROR ) ;
    }
	
	iArgumentNumber -= 2 ;
	
	
	// intentamos sacar el indice
	
	iHostIndexId = 0 ;
	
	m_oRunTimeStack->Top( vValue ) ;
	m_oRunTimeStack->Pop() ;
	
	if( vValue.Type == M4CL_CPP_TYPE_NUMBER )
	{
		iArgumentNumber-- ;
		iPoped = 0 ;
		
		if( iArgumentNumber > M4CL_MAX_CROSSTAB_COLUMNS )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_TOO_MANY_COLUMS, iArgumentNumber<<M4CL_MAX_CROSSTAB_COLUMNS<<"CrossTab" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}
		
		iHostIndexId = m4uint32_t( vValue.Data.DoubleData ) ;
	}
	else if( vValue.Type == M4CL_CPP_TYPE_STRING_POOL )
	{
		iPoped = 1 ;
		
		if( iArgumentNumber > M4CL_MAX_CROSSTAB_COLUMNS )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_TOO_MANY_COLUMS, iArgumentNumber<<M4CL_MAX_CROSSTAB_COLUMNS<<"CrossTab" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}
		
		iElement = iArgumentNumber - 1 ;
		
		iResult = m_oVariantMediator->ToPoolString( vValue ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "index" << "CrossTab" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
		
		apcColumns[ iElement ] = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
		m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
	}
	else
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "HostIndexId"<<"CrossTab" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}
	
	
	// sacamos las columnas
	
	for( j = 0 ; j < iArgumentNumber - iPoped ; j++ )
	{
		iElement = iArgumentNumber - iPoped - j - 1 ;
		
		iResult = RuntimePopString( vValue ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "columns" << "CrossTab" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
		
		apcColumns[ iElement ] = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
		m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
	}
	
	
	//HostId (Argumento 2)
	
    iResult = RuntimePopString( vValue ) ;
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "HostId" << "CrossTab" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
	
    pccHostIdArg = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
    M4CHECK_RES( pccHostIdArg!=NULL, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "HostId" << "CrossTab" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(M4CL_CPP_TYPE_NULL)<<DumpCallStack() );

    m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
	
	
	//ChannelId (argumento 1)
	
    iResult = RuntimePopString( vValue );
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ChannelId" << "CrossTab" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
	
    pcHostChannelIdArg = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
    M4CHECK_RES( pcHostChannelIdArg != NULL , M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ChannelId" << "CrossTab" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(M4CL_CPP_TYPE_NULL)<<DumpCallStack() );

    m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
	
	
    /*Este control de errores lo pongo en cada uno de los parametros (antes)
	if( pccHostIdArg == NULL || pcHostChannelIdArg == NULL )
    {
        SET CODE( M4_CH_VM_BAD_POINTER, ERRORLOG, "Null pointer in CrossTab" ) ;
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
    }
    */
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1, BeginLogCat<<"Trace VM1: CrossTab. Channel:<"<<pcHostChannelIdArg<<"> Host Id:<"<<pccHostIdArg<<">" << EndLogCat);
#endif
	
	// ==========================================================
	// asignación de variables del target =======================
	// ==========================================================
	
    poTargetAccess = m_oState1.m_poAccess ;
	
    poTargetCMCR = poTargetAccess->GetpCMCR() ;
	
#ifdef _DEBUG
	if( poTargetCMCR == NULL )
	{
		g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"Null mcr in CrossTab. Channel: TargetCMCR"<<EndLog;
        RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}
#endif
	
    pccTargetChannel = poTargetCMCR->GetChannelId() ;
	
    hTargetNode = m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle() ;
	
    poTargetRecordSet = m_oState1.m_poRecordSet ;
	
    iTargetItems = poTargetCMCR->GetNodeNumberOfItems( hTargetNode ) ;
	
	pccHostDmdComponent = acColumnString ;
	
	
	// ==========================================================
	// asignación de variables del host =========================
	// ==========================================================
	
    if( *pcHostChannelIdArg == '\0' || strcmpi( pcHostChannelIdArg, pccTargetChannel ) == 0 )
    {
        iSameChannel = 1 ;
        poHostAccess = m_oState1.m_poAccess ;
    }
    else
    {
        iSameChannel = 0 ;
		
        pccHostInstance = poTargetCMCR->GetNodeT3AliasInstanceIdByAlias( hTargetNode, pcHostChannelIdArg ) ;
		
        if( pccHostInstance == NULL )
        {
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_INSTANCE, pcHostChannelIdArg<<poTargetCMCR->iNodeN(hTargetNode)<<LogCatAnd<<poTargetCMCR->nNodeN(hTargetNode)<<"CrossTab" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
            return( M4_ERROR ) ;
        }
		

        iResult = m_poVM->GetL2Access( poTargetAccess->GetpChannel(), pccHostInstance, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState1.m_Flags), &poHostAccess ) ;
		
        if( iResult != M4_SUCCESS )
        {
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_GET_ACCESS_L2, pccHostInstance<<"CrossTab" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
            return( M4_ERROR ) ;
        }
    }
	
    poHostCMCR = poHostAccess->GetpCMCR() ;
	
#ifdef _DEBUG
	if( poHostCMCR == NULL )
	{
		g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"Null mcr in CrossTab. Channel: HostCMCR"<<EndLog;
        RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}
#endif
	
    if( iSameChannel == 1 )
    {
        iHostHandle = poHostCMCR->GetNodeAliasNodeHandleByAlias( hTargetNode, pccHostIdArg, m_oState1.m_poFormula.GetLevel() ) ;
    }
    else
    {
        iHostHandle = poHostCMCR->GetChannelNodeHandleById( pccHostIdArg ) ;
    }
	
	if( iHostHandle == 0 )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, pccHostIdArg <<"CrossTab" << DumpCallStack() );
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}
	
	
	for( j = 0 ; j < iArgumentNumber ; j++ )
	{
		iItemHandle = poHostCMCR->GetNodeItemHandleById( iHostHandle, apcColumns[ j ] ) ;
		
		if( iItemHandle == 0 )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, apcColumns[j]<<M4ObjNodeN(poHostCMCR,iHostHandle)<<"CrossTab" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}
		
		aiColumnIndexes[ j ] = poHostCMCR->GetItemIndex( iItemHandle ) ;
	}
	
	
    iHostItems = poHostCMCR->GetNodeNumberOfItems( iHostHandle ) ;
	
	
    poDHostNode = &( poHostAccess->Node[ ClHandle( iHostHandle ) ] ) ;
	
#ifdef _DEBUG
	if( poDHostNode == NULL )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_INTERNAL_ERROR, __FILE__<<__LINE__<<"CrossTab. Null pointer for node with handle:"<<LogCat<<iHostHandle );
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}
#endif
	
    poDHostRecordSet = &( poDHostNode->RecordSet ) ;
    
	if( !poDHostRecordSet->IsValid() )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK, "CrossTab"<<poDHostRecordSet->GetpNodeDef()->Id()<<LogCatAnd<<poDHostRecordSet->GetpNodeDef()->Name()<<DumpTreePath(poDHostRecordSet->GetpAccess(), iHostHandle) << DumpCallStack() );
        RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}
	
    iHostRegisters = poDHostRecordSet->Register.Count() ;
	
	
    
	// ==========================================================
	// algoritmo ================================================
	// ==========================================================
	
	// ordenacion
	
	
    if( iHostIndexId == 0 )
    {
		iHostIndexHandle = poHostCMCR->GetNodeDefaultIndex( iHostHandle ) ;
		
		if( iHostIndexHandle == 0 )
		{
            DUMP_CHLOG_ERRORF(M4_CH_VM_NO_DEFAULT_INDEX, M4ObjNodeN(poHostCMCR,iHostHandle)<<"CrossTab" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}
		
		iHostIndexId = poHostCMCR->GetIndexId( iHostIndexHandle ) ;
	}
	else
	{
		iHostIndexHandle = poHostCMCR->GetNodeIndexHandleById( iHostHandle, iHostIndexId ) ;
		
		if( iHostIndexHandle == 0 )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONGINDEX, iHostIndexId<<M4ObjNodeN(poHostCMCR,iHostHandle ) <<"CrossTab" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}
	}
	
	
    iResult = poDHostRecordSet->SetCurrentSearchIndexById( iHostIndexId ) ;
	
    if( iResult != M4_SUCCESS )
    {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONGINDEX, iHostIndexId<<M4ObjNodeN(poHostCMCR,iHostHandle ) <<"CrossTab" << DumpCallStack() );
        RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
    }
	
    iPkElementNumber = poHostCMCR->GetIndexNumberOfItems( iHostIndexHandle ) ;
	
    if( iPkElementNumber > M4CL_MAX_PIVOT_PK_ELEMENTS )
    {
		DUMP_CHLOG_ERRORF(M4_CH_VM_TOO_MANY_PKS, iPkElementNumber<<M4CL_MAX_PIVOT_PK_ELEMENTS<<"CrossTab" << DumpCallStack() );
        RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
    }
	
    for( p = 0, q = 0 ; p < iPkElementNumber ; p++, q += 3 )
    {
        iItemHandle = poHostCMCR->GetIndexItemHandleByPosition( iHostIndexHandle, p ) ;
        iItemIndex = poHostCMCR->GetItemIndex( iItemHandle ) ;
		
        aiHostPKIndexes[ p ] = iItemIndex ;
		
        avHostPkValues[ p ].Type = M4CL_CPP_TYPE_NULL ;
        avHostPkValues[ p ].Data.DoubleData = 0 ;
        avHostPkValues[ p ].Data.PointerChar = NULL ;
		
        pccItemId = poHostCMCR->GetItemId( iItemHandle ) ;
        iItemHandle = poTargetCMCR->GetNodeItemHandleById( hTargetNode, pccItemId ) ;
		
        if( iItemHandle == 0 )
        {
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, pccItemId<<M4ObjNodeN(poTargetCMCR,hTargetNode)<<"CrossTab" << DumpCallStack() );
			RuntimePushDouble( M4_ERROR ) ;
            return( M4_ERROR ) ;
        }
		
        iItemIndex = poTargetCMCR->GetItemIndex( iItemHandle ) ;
		
        aiTargetPKIndexes[ p ] = iItemIndex ;
		
		aiTargetPKTrios[ q ].Type = M4CL_CPP_TYPE_NUMBER ;
		aiTargetPKTrios[ q ].Data.DoubleData = iItemIndex ;
		
		aiTargetPKTrios[ q + 2 ].Type = M4CL_CPP_TYPE_NUMBER ;
		aiTargetPKTrios[ q + 2 ].Data.DoubleData = M4CL_EQUAL_FUNCTION ;
    }
	
	
    for( i = 0 ; i < iHostRegisters ; i++ )
    {
		if( i == 0 )
		{
			iTargetNewRegister = 1 ;
		}
		else
		{
			iTargetNewRegister = 0 ;
		}
		
        for( p = 0 ; p < iPkElementNumber ; p++ )
        {
            iResult = poDHostRecordSet->Register[ i ].Item[ aiHostPKIndexes[ p ] ].Value.Get( vValue ) ;
			
            if( iResult != M4_SUCCESS )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, (m4uint32_t)aiHostPKIndexes[ p ]<<M4ObjNodeN(poHostCMCR,iHostHandle)<<"CrossTab" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
                return( M4_ERROR ) ;
            }
			
            if( M4VariantIsEqual( &vValue, &avHostPkValues[ p ] ) == 0 )
            {
				avHostPkValues[ p ].Type = vValue.Type ;
				avHostPkValues[ p ].Data.DoubleData = vValue.Data.DoubleData ;
				avHostPkValues[ p ].Data.PointerChar = vValue.Data.PointerChar ;
				
                iTargetNewRegister = 1 ;
            }
        }
		
        if( iTargetNewRegister == 1 )
        {
			// hay que intentar encontrarlo
			
			for( p = 0, q = 0 ; p < iPkElementNumber ; p++, q += 3 )
			{
				aiTargetPKTrios[ q + 1 ].Type = avHostPkValues[ p ].Type ;
				aiTargetPKTrios[ q + 1 ].Data.DoubleData = avHostPkValues[ p ].Data.DoubleData ;
				aiTargetPKTrios[ q + 1 ].Data.PointerChar = avHostPkValues[ p ].Data.PointerChar ;
			}
			
            iResult = poTargetRecordSet->Register.FindSec( aiTargetPKTrios, q, &iRegister ) ;
			
            if( iResult != M4_SUCCESS )
            {
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_IN_FIND, M4ObjNodeN(poTargetRecordSet->GetpAccess()->GetpCMCR(),poTargetRecordSet->GetpNodeDef()->GetHandle())<<"CrossTab" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
                return( M4_ERROR ) ;
            }
			
            if( iRegister != - 1 )
            {
				// hacer el MoveTo
				poTargetRecordSet->Current.MoveTo( ClRegisterIndex( iRegister ) ) ;
            }
            else
            {
				iResult = poTargetRecordSet->Current.Add() ;
				
				if( iResult != M4_SUCCESS )
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_CANT_ADD_RECORD, M4ObjNodeN(poTargetCMCR,hTargetNode)<<"CrossTab"<< DumpTreePath(poTargetRecordSet->GetpAccess(),hTargetNode)  << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				// copiamos pk
				
				for( p = 0 ; p < iPkElementNumber ; p++ )
				{
					iResult = poTargetRecordSet->Current.Item[ aiTargetPKIndexes[ p ] ].Value.Set( avHostPkValues[ p ] ) ;
					
					if( iResult != M4_SUCCESS )
					{
						DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, M4ObjNodeItemI(poTargetCMCR,poTargetRecordSet->Current.Item.ItemDef.GetHandle())<<"CrossTab" << DumpCallStack() );
						RuntimePushDouble( M4_ERROR ) ;
						return( M4_ERROR ) ;
					}
				}
			}
        }
		
		// CrossTabamos
		
		iActualLength = 0 ;
		
		for( j = 0 ; j < iArgumentNumber ; j++ )
		{
			iResult = poDHostRecordSet->Register[ i ].Item[ aiColumnIndexes[ j ] ].Value.Get( vValue ) ;
			
			if( iResult != M4_SUCCESS )
			{
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, apcColumns[ j ]<<M4ObjNodeN(poHostCMCR,iHostHandle)<<"CrossTab" << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
			}
			
			if( vValue.Type == M4CL_CPP_TYPE_NULL )
			{
				pccHostColumn = "@@" ;
			}
			else if( vValue.Type == M4CL_CPP_TYPE_STRING_VAR )
			{
				pccHostColumn = vValue.Data.PointerChar ;
			}
			else if( vValue.Type == M4CL_CPP_TYPE_NUMBER )
			{
				sprintf( acTempString, "%.0f", vValue.Data.DoubleData ) ;
				pccHostColumn = acTempString ;
			}
			else
			{
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH, apcColumns[ j ]<<M4ObjNodeN(poHostCMCR,iHostHandle)<< "CrossTab" << LOG_CAT_BLOCK( M4ClCppType(M4CL_CPP_TYPE_NULL)<<", "<<M4ClCppType(M4CL_CPP_TYPE_STRING_VAR)<<", "<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) )<<M4ClCppType(vValue.Type)<<DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
			}
			
			if( j > 0 )
			{
				acColumnString[ iActualLength++ ] = '#' ;
			}
			
			iLength = strlen( pccHostColumn ) ;
			
			if( iActualLength + iLength > M4CL_MAX_CROSSTAB_COLUMN_ID )
			{
				acColumnString[ iActualLength ] = '\0' ;
				DUMP_CHLOG_ERRORF(M4_CH_VM_DATA_TRUNCATED, acColumnString << LogCat << pccHostColumn << M4CL_MAX_CROSSTAB_COLUMN_ID << acColumnString << DumpCallStack() );
				RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
			}
			
			memcpy( acColumnString + iActualLength, pccHostColumn, iLength + 1 ) ;
			iActualLength += iLength ;
		}
		
		
		for( j = 0 ; j < iTargetItems ; j++ )
		{
			iTargetItemHandle = poTargetCMCR->GetNodeItemHandleByPosition( hTargetNode, j ) ;
			
#ifdef _DEBUG
			if( iTargetItemHandle == 0 )
			{
				g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"TargetItem["<<LogCat<<j<<LogCat<<"]"
																<<poTargetCMCR->iNodeN(hTargetNode)<<LogCatAnd<<poTargetCMCR->nNodeN(hTargetNode)
																<<"CrossTab"<< DumpCallStack() << EndLog;

				RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
			}
#endif
			
			iTargetType = poTargetCMCR->GetItemType( iTargetItemHandle ) ;
			
			if( iTargetType == M4CL_ITEM_TYPE_METHOD )
			{
				continue ;
			}
			
			// es un item con celda en el target
			
			
			pccTargetDmdComponent = poTargetCMCR->GetItemDmdCrosstab( iTargetItemHandle ) ;
			
#ifdef _DEBUG
			if( pccTargetDmdComponent == NULL )
			{
				g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"CrossTab.  Dmd Component. Target Item:"<<LogCat<<j<<EndLog;
				RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
			}
#endif
			
			if( strcmpi( pccTargetDmdComponent, pccHostDmdComponent ) != 0 )
			{
				continue ;
			}
			
			// es un componente que hace matching
			
			pccTargetDmdField = poTargetCMCR->GetItemDmdField( iTargetItemHandle ) ;
			
#ifdef _DEBUG
			if( pccTargetDmdField == NULL )
			{
				g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"CrossTab.  Dmd Field. Target Item: :"<<LogCat<<j<<EndLog;
				RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
			}
#endif
			
			for( k = 0 ; k < iHostItems ; k++ )
			{
				iHostItemHandle = poHostCMCR->GetNodeItemHandleByPosition( iHostHandle, k ) ;
				
#ifdef _DEBUG
				if( iHostItemHandle == 0 )
				{
					g_oChLog<<BeginError(M4_CH_VM_UNDEFINED_ITEM)	<<"HostItem["<<LogCat<<k<<LogCat<<"]"
																	<<poHostCMCR->iNodeN(iHostHandle)<<LogCatAnd<<poHostCMCR->nNodeN(iHostHandle)
																	<<"CrossTab"<< DumpCallStack() << EndLog;
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
#endif
				
				iHostType = poHostCMCR->GetItemType( iHostItemHandle ) ;
				
				if( iHostType == M4CL_ITEM_TYPE_METHOD )
				{
					continue ;
				}
				
				// es un item host valido
				
				pccHostItemId = poHostCMCR->GetItemId( iHostItemHandle ) ;
				
#ifdef _DEBUG
				if( pccHostItemId == NULL )
				{
                    RuntimePushDouble( M4_ERROR ) ;
				}
#endif

				CHECK_CHLOG_DEBUGF( pccHostItemId == NULL, M4_ERROR, M4_CH_VM_INTERNAL_ERROR, __FILE__<<__LINE__ << k ) ;

				
				if( strcmpi( pccTargetDmdField, pccHostItemId ) != 0 )
				{
					continue ;
				}
				
				// es un item al que hay que generar valor
				
				iResult = poDHostRecordSet->Register[ i ].Item[ ClItemIndex( k ) ].Value.Get( vValue ) ;
				
				if( iResult != M4_SUCCESS )
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, k<<M4ObjNodeN(poHostCMCR,iHostHandle)<<"CrossTab" << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				iResult = poTargetRecordSet->Current.Item[ ClItemIndex( j ) ].Value.Get( vAuxValue ) ;
				
				if( iResult != M4_SUCCESS )
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, j<<M4ObjNodeN(poTargetCMCR,hTargetNode)<<"CrossTab" << DumpCallStack() );
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}
				
				if( M4VariantIsEqual( &vValue, &vAuxValue ) == 0 )
				{
					iResult = poTargetRecordSet->Current.Item[ ClItemIndex( j ) ].Value.Set( vValue ) ;
					
					if( iResult != M4_SUCCESS )
					{
						DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, M4ObjNodeItemI(poTargetCMCR,poTargetRecordSet->Current.Item.ItemDef.GetHandle())<<"CrossTab" << DumpCallStack() );
						RuntimePushDouble( M4_ERROR ) ;
						return( M4_ERROR ) ;
					}
					else
					{
						if( GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_VALUES)
						{
							ClItem	&item = poTargetRecordSet->Current.Item;
							GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddValue( item, vValue, item.Value.GetPriority(), M4_TRUE, 0, m_oState1, DumpCallStack( &m_oState1, M4_TRUE ).c_str() ) ;
						}
					}
				}
			}
		}
	}
	
    RuntimePushDouble(M4_SUCCESS);
    return( M4_SUCCESS ) ;
	
}


//-----------------------------------------------------
//F U N C I O N E S  D E  I N I C I A L I Z A C I O N
//-----------------------------------------------------

m4return_t ClVM1PayrollLib::prInitApplyValue(void){
    
    m4VariantType  vGroup, vDMD;
    m4return_t  ret;
	
    
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1,"Trace VM1: InitApplyValue ");
#endif
    //inicializaciones previas
    if(m_poPayRoll==NULL){
		
        m_poPayRoll=new ClVMPayRoll;
    }
    
//    m_poPayRoll->SetVM(this);
    m_poPayRoll->SetDatesStack(&m_oDatesStack);
    m_poPayRoll->SetAccess(m_oState1.m_poAccess);
    m_poPayRoll->SetWorkingDates(m_oState1.m_dBeginDate, m_oState1.m_dEndDate, m_oState1.m_dDate);
	
    //leemos argumentos
	ret=RuntimePop(vGroup);
	DEBUG_CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "InitApplyValue.")	;
    
    if(vGroup.Type!=M4CL_CPP_TYPE_STRING_POOL){
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "group"<<"prInitApplyValue" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vGroup.Type)<<DumpCallStack() );
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	
	ret=RuntimePop(vDMD);
	DEBUG_CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "InitApplyValue.")	;
    
    if(vDMD.Type!=M4CL_CPP_TYPE_STRING_POOL){
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "id_dmd"<<"prInitApplyValue" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vDMD.Type)<<DumpCallStack() );
        RuntimePushDouble(M4_ERROR);
		m_oDynamicPool->DeleteString(vGroup.Data.CharData);
        return M4_ERROR;
    }
	
    ret=m_poPayRoll->InitApplyValue(m_oState1.m_poRecordSet, m_oDynamicPool->GetString(vDMD.Data.CharData) ,m_oDynamicPool->GetString(vGroup.Data.CharData) );
	
    //borramos las cadenas 
    m_oDynamicPool->DeleteString(vGroup.Data.CharData);
	m_oDynamicPool->DeleteString(vDMD.Data.CharData);
	
    //metemos resultado
    RuntimePushDouble(ret);
    return M4_SUCCESS;
}


m4return_t ClVM1PayrollLib::prSetDates(void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1,"Trace VM1: prSetDates");
#endif
    m4VariantType vStart, vEnd,vRun, vNumArg;
    m4return_t  ret;
	
    
	
	
    //inicializaciones previas
    if(m_poPayRoll==NULL){
		
        m_poPayRoll=new ClVMPayRoll;
    }
    
//    m_poPayRoll->SetVM(this);
    m_poPayRoll->SetDatesStack(&m_oDatesStack);
    m_poPayRoll->SetAccess(m_oState1.m_poAccess);
    //m_poPayRoll->SetWorkingDates(m_oState1.m_dBeginDate, m_oState1.m_dEndDate, m_oState1.m_dDate);
	
    //leemos argumentos
	
    //miramos cuantos argumentos tiene
    ret=RuntimePop(vNumArg);
	DEBUG_CHECK_RES(ret==M4_SUCCESS && vNumArg.Type==M4CL_CPP_TYPE_NUMBER, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "number_of_arguments#setdates" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumArg.Type)<<DumpCallStack() );
	
    if(vNumArg.Data.DoubleData==3) {
        ret=RuntimePop(vRun);
        DEBUG_CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "prSetDates")	;
    }
    else if(vNumArg.Data.DoubleData!=2) { //numero incorrecto de parametros
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "prSetDates"<<"2-3" << DumpCallStack() );

        return M4_ERROR;
    }
    else{
        vRun.Type=M4CL_CPP_TYPE_DATE;
        vRun.Data.DoubleData=0.0;
    }
	
	
    ret=RuntimePopDate(vEnd);
	M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "end_date" << "prSetDates" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vEnd.Type)<<DumpCallStack() );
	ret=RuntimePopDate(vStart);
	M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "start_date" << "prSetDates" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vStart.Type)<<DumpCallStack() );
	
    if(vRun.Type!=M4CL_CPP_TYPE_DATE){
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "run_date"<<"prSetDates" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vRun.Type)<<DumpCallStack() );

        RuntimePushDouble(M4_ERROR);
		DeleteVarString(vRun);
        return M4_ERROR;
    }
	
	
    ret = m_poPayRoll->SetEmployee(m_oState1.m_poRecordSet);
	
    M4CHECK_DEBUGINFO(ret==M4_SUCCESS, M4_CH_VM_AV_PREP_NODE, ERRORLOG, "SetDates" <<DumpCallStack() )	;
	
    ret=m_poPayRoll->SetDates(vStart.Data.DoubleData, vEnd.Data.DoubleData, vRun.Data.DoubleData);
	
    
    //metemos resultado
    RuntimePushDouble(ret);
    return M4_SUCCESS;
}


m4return_t ClVM1PayrollLib::prSetPeriods(void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1,"Trace VM1: prSetPeriods ");
#endif
    m4VariantType vStart, vEnd, vRun, vSlice;
    m4return_t  ret;
	
    
	
	
    //inicializaciones previas
    if(m_poPayRoll==NULL){
		
        m_poPayRoll=new ClVMPayRoll;
    }
    
//    m_poPayRoll->SetVM(this);
    m_poPayRoll->SetDatesStack(&m_oDatesStack);
    m_poPayRoll->SetAccess(m_oState1.m_poAccess);
    m_poPayRoll->SetWorkingDates(m_oState1.m_dBeginDate, m_oState1.m_dEndDate, m_oState1.m_dDate);
	
    //leemos argumentos
    ret=RuntimePop(vSlice);
	DEBUG_CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "prSetPeriods. ")	;
	ret=RuntimePopDate(vRun);
	M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "run_date" << "prSetPeriods" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vRun.Type)<<DumpCallStack() );
	ret=RuntimePopDate(vEnd);
	M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "start_date" << "prSetPeriods" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vEnd.Type)<<DumpCallStack() );
	ret=RuntimePopDate(vStart);
	M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "end_date" << "prSetPeriods" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vStart.Type)<<DumpCallStack() );
	
	
    m_oVariantMediator->ToDouble(vSlice);
    if(vSlice.Type!=M4CL_CPP_TYPE_NUMBER){
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "slice_mode"<<"prSetPeriods" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vSlice.Type)<<DumpCallStack() );

        RuntimePushDouble(M4_ERROR);
		DeleteVarString(vSlice) ;
        return M4_ERROR;
    }
	
    ret = m_poPayRoll->SetEmployee(m_oState1.m_poRecordSet );
    M4CHECK_DEBUGINFO(ret==M4_SUCCESS, M4_CH_VM_AV_PREP_NODE, ERRORLOG, "SetPeriods" <<DumpCallStack() )	;
	
    ret=m_poPayRoll->SetPeriods(vStart.Data.DoubleData, vEnd.Data.DoubleData, vRun.Data.DoubleData, ( (vSlice.Data.DoubleData!=0) ? M4_TRUE : M4_FALSE ) );
	
    //metemos resultado
    RuntimePushDouble(ret);
    return M4_SUCCESS;
	
	
	
	
}


//-----------------------------------------------------
//A P P L Y   T A B L E
//-----------------------------------------------------
m4return_t ClVM1PayrollLib::prApplyTable(void){
	
    m4return_t ret;
    ClAccessRecordSet *poHistoric,*poOrg, *poValues  ;
    m4VariantType vValues, vOrg, vHistoric, vTemp;
    m4uint32_t iNumPK, iNumArg,i, iNumPKOriginal;
    m4VariantType *avPK=0;
    ClItemIndex *aiHistoric=0;
    ClItemIndex *aiValues  =0;
	m4bool_t	bApplyAlways;
    
	
    if(m_poPayRoll==NULL){
		
        m_poPayRoll=new ClVMPayRoll;
    }
    
//    m_poPayRoll->SetVM(this);
    m_poPayRoll->SetDatesStack(&m_oDatesStack);
    m_poPayRoll->SetAccess(m_oState1.m_poAccess);
    m_poPayRoll->SetWorkingDates(m_oState1.m_dBeginDate, m_oState1.m_dEndDate, m_oState1.m_dDate);
	
    //---------------------leemos los parámetros
	
    //Numero de argumentos
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ApplyTable.")	;
    iNumArg=int(vTemp.Data.DoubleData);
    if(iNumArg<4){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "prApplyTable"<<"4-" << DumpCallStack() );

        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	
	//Si el numero de argumentos-4 es impar ->nos indican el argumento opcional bApplyAlways
	//si es par no lo especifican
	if((iNumArg-4)%2 ){ //leemos el applyAlways
		ret=RuntimePop(vTemp);
		DEBUG_CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ApplyTable.")	;
		ret=m_oVariantMediator->ToDouble(vTemp);
		if(ret!=M4_SUCCESS){ 
	        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "apply_always"<<"ApplyTable" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );

			DeleteVarString(vTemp) ;
			return M4_ERROR;
		}
		bApplyAlways= ( m4int32_t(vTemp.Data.DoubleData)==0 ? M4_FALSE : M4_TRUE);
	}
	else{ //por defecto es que No ApplyAlways
		bApplyAlways=M4_FALSE;
	}

    //leemos el número de pk's que nos dicen que van a venir
    ret=RuntimePop(vTemp);
    DEBUG_CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ApplyTable.")	;
    m_oVariantMediator->ToDouble(vTemp);
    if(vTemp.Type!=M4CL_CPP_TYPE_NUMBER){ 
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "pk's number"<<"ApplyTable" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );

		DeleteVarString(vTemp) ;
        return M4_ERROR;
    }
    iNumPK=int(vTemp.Data.DoubleData);
	
	
    //comprobaciçon para ver si el número de pk's es cierto)
    if((iNumArg-4)/2 < iNumPK){
        DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PK_PAIR_NUMBER, (iNumArg-4)/2<<iNumPK << DumpCallStack() );
        return M4_ERROR;
    }
	
    //sacamos las pk's y las metemso es un array
    avPK=new m4VariantType [iNumPK*2];
    for(i=0; i< iNumPK*2; ++i){
        ret=RuntimePop(avPK[i]);
        DEBUG_CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ApplyTable")	;
        
		if(avPK[i].Type!=M4CL_CPP_TYPE_STRING_POOL){ //no es cadena->error
	        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "pk"<<"ApplyTable" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(avPK[i].Type)<<DumpCallStack() );

			//fredom
			for(m4uint32_t j=0; j<i ; ++j){
				m_oDynamicPool->DeleteString(avPK[j].Data.CharData);
			}
			if(avPK!=0)
				delete[] avPK;
            return M4_ERROR;
        }
    }
	
	//--Los parametros
	//el nodo organigrama
	m4return_t ret2=M4_SUCCESS;
    
	ret=RuntimePop(vOrg);
    DEBUG_CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ApplyTable")	;
    
    if(vOrg.Type!=M4CL_CPP_TYPE_STRING_POOL){ //Null behaviour: error
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "org_chart"<<"ApplyTable" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vOrg.Type)<<DumpCallStack() );
        ret2 = M4_ERROR;
    }
    //parámetro- Nombre del nodo de unidades
    ret=RuntimePop(vHistoric);
    DEBUG_CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ApplyTable.")	;
    
    if(vHistoric.Type!=M4CL_CPP_TYPE_STRING_POOL){ //Null behaviour: error
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "historic"<<"ApplyTable" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vHistoric.Type)<<DumpCallStack() );
		ret2 = M4_ERROR;
    }
	
    //parámetro- Nombre del nodo de valores
    ret=RuntimePop(vValues);
    DEBUG_CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ApplyTable")	;
    
    if(vValues.Type!=M4CL_CPP_TYPE_STRING_POOL){ //Null behaviour: error
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "values"<<"ApplyTable" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValues.Type)<<DumpCallStack() );
        ret2 = M4_ERROR;
    }
	
	//si alguno de los argumentos no ha ido bien SALIMOS
	if(ret2 == M4_ERROR){
		DeleteVarString(vValues);
		DeleteVarString(vHistoric);
		DeleteVarString(vOrg);
		//delete strings
        for(i=0; i< iNumPK*2; ++i){
            m_oDynamicPool->DeleteString(avPK[i].Data.CharData);
        }
        if(avPK!=0)
			delete[] avPK;
		
		return M4_ERROR;
	}
	
	
	
    //Obtenemos Recordsets
    ClHandle hNodeValues;    
    ClHandle hNodeOrg;
    ClHandle hNodeHistoric;
    m4uint32_t hThisNode=m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle();
	
	
    //el de históricos->
    if(strcmp(m_oDynamicPool->GetString(vHistoric.Data.CharData), "") !=0 ){
        hNodeHistoric=m_oState1.m_poAccess->GetpCMCR()->GetNodeAliasNodeHandleByAlias(hThisNode, m_oDynamicPool->GetString(vHistoric.Data.CharData), m_oState1.m_poFormula.GetLevel() );
        if( hNodeHistoric==0 || &(m_oState1.m_poAccess->Node[hNodeHistoric] ) ==0){
            DUMP_CHLOG_ERRORF(M4_CH_VM_AV_WRONG_HIST_NODE, m_oDynamicPool->GetString(vHistoric.Data.CharData) <<DumpCallStack() );
			poHistoric=0;
        }
        else {
            poHistoric= &(m_oState1.m_poAccess->Node->RecordSet);
        }
        
    }
    else{
        poHistoric=0;
    }
	
    //el de Organigrama
    if(strcmp(m_oDynamicPool->GetString(vOrg.Data.CharData), "") !=0 ){
        hNodeOrg=m_oState1.m_poAccess->GetpCMCR()->GetNodeAliasNodeHandleByAlias(hThisNode, m_oDynamicPool->GetString(vOrg.Data.CharData), m_oState1.m_poFormula.GetLevel() );
        if( hNodeOrg==0 || &(m_oState1.m_poAccess->Node[hNodeOrg]) ==0 ){
            DUMP_CHLOG_ERRORF(M4_CH_VM_AV_WRONG_ORGANIGRAM_NODE, m_oDynamicPool->GetString(vOrg.Data.CharData) <<DumpCallStack() );
            poOrg=0;
        }
        else{
            poOrg= &(m_oState1.m_poAccess->Node->RecordSet);
        }
    }
    else{
        poOrg=0;
    }
	
	
	m4return_t retVal ;
	
	// Bg 0132608
	iNumPKOriginal = iNumPK ;

    //el de valores
    hNodeValues=m_oState1.m_poAccess->GetpCMCR()->GetNodeAliasNodeHandleByAlias(hThisNode, m_oDynamicPool->GetString(vValues.Data.CharData), m_oState1.m_poFormula.GetLevel() );
    if( hNodeValues==0 || &(m_oState1.m_poAccess->Node[hNodeValues]) == 0){
        DUMP_CHLOG_ERRORF(M4_CH_VM_AV_WRONG_VALUES_NODE, m_oDynamicPool->GetString(vValues.Data.CharData) <<DumpCallStack() );
        retVal = M4_ERROR; //ERROR GRAVE-->Salimos sin aplicar
    }
    else{ //tenemos node de valores->aplicamos
        poValues= &(m_oState1.m_poAccess->Node->RecordSet);
		
		
		//el de empleados-> en el que estmos
		
		
		//si no hay histórico, aunque nos pasen pk's las ignoramos
		if(poHistoric==0) iNumPK=0;
		
		//creamos lo array de índices
		aiHistoric=new ClItemIndex [iNumPK];
		aiValues  =new ClItemIndex [iNumPK];
		
		for(i=0; i< iNumPK; ++i){

            m4uint32_t hitem_edu;
            hitem_edu = poValues->GetpNodeDef()->GetNodeItemHandle(m_oDynamicPool->GetString(avPK[i*2].Data.CharData));
            if(hitem_edu==0){
				aiValues[i] = M4DM_EOF_INDEX;
			}
			else{
				aiValues[i]=poValues->GetpNodeDef()->GetItemIndex(hitem_edu);
			}

			if(aiValues[i] == M4DM_EOF_INDEX) { //el item no existe
				DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PK_MAPPING, m_oDynamicPool->GetString(avPK[(i*2)].Data.CharData)<<M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),hNodeValues )<<DumpCallStack() );
				ret= M4_ERROR;
				break;
			}


            hitem_edu = poHistoric->GetpNodeDef()->GetNodeItemHandle(m_oDynamicPool->GetString(avPK[(i*2)+1].Data.CharData));
			if(hitem_edu==0){
				aiHistoric[i] = M4DM_EOF_INDEX;
			}
			else{
				aiHistoric[i]=poHistoric->GetpNodeDef()->GetItemIndex(hitem_edu);
			}
			if(aiHistoric[i] == M4DM_EOF_INDEX) { //el item no existe
				DUMP_CHLOG_ERRORF(M4_CH_VM_AV_PK_MAPPING, m_oDynamicPool->GetString(avPK[(i*2)+1].Data.CharData)<<M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),hNodeHistoric )<<DumpCallStack() );
				ret= M4_ERROR;
				break;
			}
		}
		
		if(ret==M4_SUCCESS){
			ret = m_poPayRoll->SetEmployee(m_oState1.m_poRecordSet );
			M4CHECK_DEBUGINFO(ret==M4_SUCCESS, M4_CH_VM_AV_PREP_NODE, ERRORLOG, "ApplyTable" <<DumpCallStack() )	;
			
			ret = m_poPayRoll->ApplyTable(poValues,poHistoric, poOrg, aiHistoric,aiValues,iNumPK ,bApplyAlways);
		}
		
		RuntimePushDouble(ret); //metemos el resultado (exito o error)
		retVal=M4_SUCCESS;
	}
	
	
    //delete strings
    for(i=0; i< iNumPKOriginal*2; ++i){
        m_oDynamicPool->DeleteString(avPK[i].Data.CharData);
    }
    m_oDynamicPool->DeleteString(vOrg.Data.CharData);
    m_oDynamicPool->DeleteString(vValues.Data.CharData);
    m_oDynamicPool->DeleteString(vHistoric.Data.CharData);
	
    //delete structures used
    if(avPK!=0)
        delete[] avPK;
    if(aiHistoric!=0)
        delete[] aiHistoric;
    if(aiValues!=0)
        delete[] aiValues;
	
    return retVal;  //retornamos éxito o error
	
    
}






m4return_t ClVM1PayrollLib::_FindEqualItems( ClHandle ai_hNodo1, ClCompiledMCR *ai_poCMCR1 , VectorItemIndex &ai_vIndex1, ClHandle ai_hNodo2, ClCompiledMCR *ai_poCMCR2 , VectorItemIndex &ai_vIndex2 ,m4int32_t &ao_iNumItems,m4uint8_t ai_ItemType )

{
	ClHandle	TempHandle1,TempHandle2;
	m4pcchar_t	pcTempItem1;
	
	
	ao_iNumItems = 0;

	if( ai_hNodo1 == 0 || ai_hNodo2 == 0 )
	{
		return M4_ERROR;
	}
	
	m4uint16_t iNumItems = ai_poCMCR1->GetNodeNumberOfItems(ai_hNodo1);

	for( m4uint16_t i = 0; i < iNumItems ; i++ )
	{
		TempHandle1 = ai_poCMCR1->GetNodeItemHandleByPosition(ai_hNodo1, i);

		if( ai_ItemType == ai_poCMCR1->GetItemScope(TempHandle1) && ai_poCMCR1->GetItemType(TempHandle1) != M4CL_ITEM_TYPE_METHOD )
		{
			pcTempItem1 = ai_poCMCR1->GetItemId(TempHandle1);
			
			TempHandle2 = ai_poCMCR2->GetNodeItemHandleById(ai_hNodo2,pcTempItem1);

			if(TempHandle2 != 0)
			{
				ai_vIndex1.insert(ai_vIndex1.end(),ai_poCMCR1->GetItemIndex(TempHandle1));

				ai_vIndex2.insert(ai_vIndex2.end(),ai_poCMCR2->GetItemIndex(TempHandle2));

				ao_iNumItems++;

			}

		}
	}
	return M4_SUCCESS;

}


//------------------------------------------------------------------------
//YTDSearch (la clásica, sin argumentos variables)
//------------------------------------------------------------------------
m4return_t ClVM1PayrollLib::YTDSearch(void){
	StFunctionCmp_t	astfcmpAND[7];
	m4VariantType  avArgs[10];
	ClAccess *poSourceAccess;
	m4uint32_t  hSourceNode,i;
	m4return_t ret;

	ret=GetFixedYTDArguments(poSourceAccess, hSourceNode, avArgs,"YTDSearch");
	if(ret!=M4_SUCCESS){
		//borramos las posibles cadenas del array, en caso deque llamemos al algoritmo se borran alli
		for(i = 0;i < 10;i++){
			DeleteVarString(avArgs[i]);
		}
		return M4_ERROR;
	}
	return YTDSearchAlgorithm(avArgs, poSourceAccess, hSourceNode, astfcmpAND, NULL, M4_CL_VM_YTD_ONE_RECORD , M4_FALSE,"YTDSearch");
}


//------------------------------------------------------------------------
//YTDSearchFiltered (Con argumentos adicionales para montar filtraje)
//------------------------------------------------------------------------

m4return_t ClVM1PayrollLib::YTDSearchFiltered(void){
	
	StFunctionCmp_t	astfcmpAND[7];
	m4VariantType  avArgs[10],vNumArg, avAditional[60],vSlice;
	ClAccess *poSourceAccess;
	m4uint32_t  hSourceNode, hItem;
	m4return_t ret,iResult;
	m4uint32_t iAditionalComp;
	m4uint16_t idxItem;
	m4uint8_t	iKeepSlices=M4_CL_VM_YTD_ONE_RECORD;

	iResult=M4_SUCCESS;
	
	
	//sacamos el número de argumentos
	ret=RuntimePopNumber(vNumArg);
    M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ParametersNumber" << "YTDSearchFiltered" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumArg.Type)<<DumpCallStack(&m_oState1) );

	m4int32_t iNumArg = m4int32_t(vNumArg.Data.DoubleData);
	m4int32_t i;
	
	//comprobamos que sean correctos (>=12, %3=0 o %3=1), y menos de 20 condiciones
	/* Bug 0094225
	Con el argumento de si se quieren slices o no deben ser 73 argumentos
	*/
	if(iNumArg>73){
		DUMP_CHLOG_ERRORF(M4_CH_VM_TOO_MUCH_PARAM_NUM, "YTDSearchFiltered" << DumpCallStack() );
		return( M4_ERROR ) ;
	}
	
	
	if(iNumArg<12 || (iNumArg%3!=0 && iNumArg%3!=1) ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "YTDSearchFiltered"<<"12-" << DumpCallStack() );
		return( M4_ERROR ) ;
	}

	//comprobamos si nos indican por parámetros si quieren o no tramos
	if(iNumArg%3==1){
		//sacamos el argumento
		ret=RuntimePopNumber(vSlice);
		M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "KeepValuesPolicy" << "YTDSearchFiltered" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vSlice.Type)<<DumpCallStack(&m_oState1) );
		
		//vemos si verdadero o false
		iKeepSlices=(m4uint8_t)vSlice.Data.DoubleData;
		
		if(iKeepSlices>M4_CL_VM_YTD_KEEP_NUMBER){ //si argumento inválido, les cantamos un error
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "KeepValuesPolicy"<<"YTDSearchFiltered"<<vSlice.Data.DoubleData<< DumpCallStack() );
			return M4_ERROR;
		}

		--iNumArg; //para que los cálculos sobre el número de condiciones... sea correcto
	}
	else{ //por defecto, false
		iKeepSlices=M4_CL_VM_YTD_ONE_RECORD;
	}

	//sacamos lo argumentos variables comprobando tipòs
	for(i=0; i<iNumArg-12;++i){
		//sacamos función
		ret=RuntimePopNumber(avAditional[i]);
		if(ret!=M4_SUCCESS){
			DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH , "CompareFunction" << "YTDSearchFiltered" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(avAditional[i].Type)<<DumpCallStack() );
			iResult=M4_ERROR;
		}

		if(avAditional[i].Data.DoubleData<1.0 || avAditional[i].Data.DoubleData>M4CL_MAX_VARIANT_COMPARE_FUNCTION){
			DUMP_CHLOG_ERRORF( M4_CH_VM_INVALID_COMPARE_FUNCTION, (m4uint32_t)avAditional[i].Data.DoubleData << 1 << M4CL_MAX_VARIANT_COMPARE_FUNCTION ) ;
			iResult= M4_ERROR;
		}
		++i;

		//sacamos Valor
		ret=RuntimePop(avAditional[i]);
		DEBUG_CHECK_RES(ret==M4_SUCCESS , M4_CH_VM_RTS_POP , ERRORLOG, "YTDSearchFiltered" << DumpCallStack() );
		++i;
		
		//sacamos id_item
		ret=RuntimePop(avAditional[i]);
		DEBUG_CHECK_RES(ret==M4_SUCCESS , M4_CH_VM_RTS_POP , ERRORLOG, "YTDSearchFiltered" << DumpCallStack() );
		if(avAditional[i].Type != M4CL_CPP_TYPE_NUMBER && avAditional[i].Type != M4CL_CPP_TYPE_STRING_POOL ){ //viene por indice o nombree, vale
			DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH , "CompareItemID" << "YTDSearchFiltered" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(avAditional[i].Type)<<DumpCallStack() );
			iResult=M4_ERROR;
		}
		//no incrementamos, se hace en el bucle
	}

	if(iResult==M4_SUCCESS){
		
		//Obtenemos puntero al metacanal+hNode
		ret=GetFixedYTDArguments(poSourceAccess, hSourceNode, avArgs,"YTDSearchFiltered");
		
		if(ret==M4_SUCCESS){
			
			ClCompiledMCR *poCMCR=poSourceAccess->GetpCMCR();

			//comparaciones adicionales = (iNumArg-12)/3
			iAditionalComp=(iNumArg-12)/3;
			

			//rellenamos las comparacions adicionales
			for(i=0; i<iNumArg-12;i+=3){
				//rellenamos función,valor e item

				if(avAditional[i+2].Type==M4CL_CPP_TYPE_NUMBER){
					//comprobamos que esté dentro de límites
					idxItem=(m4uint16_t)avAditional[i+2].Data.DoubleData;
					if(idxItem < 0 || idxItem >= poCMCR->GetNodeNumberOfItems(hSourceNode) ){
						DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_ITEM_INDEX, idxItem << poSourceAccess->GetpCMCR()->GetNodeId(hSourceNode) << "YTDSearchFiltered" << DumpCallStack() );
						iResult=M4_ERROR;
						break;
					}

				}
				else{ //tiene que ser cadena
					//obtenemos el índex por id
					hItem = poCMCR->GetNodeItemHandleById( hSourceNode, m_oDynamicPool->GetString(avAditional[i+2].Data.CharData) );
					if( hItem == 0 ){
						DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, m_oDynamicPool->GetString(avAditional[i+2].Data.CharData) << M4ObjNodeN(poCMCR,hSourceNode) <<"YTDSearchFiltered" << DumpCallStack() );
						iResult=M4_ERROR;
						break;
					}
					m_oDynamicPool->DeleteString(avAditional[i+2].Data.CharData);
					idxItem=poCMCR->GetItemIndex(hItem);
					avAditional[i+2].SetDouble(idxItem);
				}
				//si el valor es cadena (de pool) convertimso a ver_string
				if(avAditional[i+1].Type==M4CL_CPP_TYPE_STRING_POOL){
					ret=m_oVariantMediator->ToCharString(avAditional[i+1]);
				}


			}
		}
		else{
			iResult=M4_ERROR;
		}
	}
			
	if(iResult==M4_SUCCESS){		
		//llamamos al algoritmo
		ret= YTDSearchAlgorithm(avArgs, poSourceAccess, hSourceNode, astfcmpAND, avAditional, iAditionalComp, iKeepSlices, "YTDSearchFiltered");

	}
	else{
		//borramos las posibles cadenas del array, en caso deque llamemos al algoritmo se borran alli
		for(i = 0;i < 10;i++){
			DeleteVarString(avArgs[i]);
		}
		ret=M4_ERROR;
	}

	//borramos los argumentos adicionales, ya no se necesitan
	for(i=0; i<iNumArg-12;++i){
		DeleteVarString(avAditional[i]);
	}
	

	//el resultado en la pila lo mete el algoritmo
	return ret;
}


	

//------------------------------------------------------------------------
//GetFixedYTDArguments
//------------------------------------------------------------------------
//Saca los 12 argumentos fijos del YTD y resuleve el puntero al canal y el hNodo

m4return_t	ClVM1PayrollLib::GetFixedYTDArguments(ClAccess *&ao_poAccess, m4uint32_t &ao_hNode, m4VariantType *ao_vFixedArgs, const m4char_t *ai_pccMethod){

	const m4int32_t iNumArgs = 10;
	m4return_t ret;
	m4VariantType vNodeName, vChannelName;

	for( m4int32_t i = 0;i < iNumArgs;i++){
		ret=RuntimePop( ao_vFixedArgs[i] );
	    DEBUG_CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethod)	;
	}

	//sacamos nombre de nodo
	ret=RuntimePopString(vNodeName);
	if(ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node Name"<<ai_pccMethod << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vNodeName.Type)<<DumpCallStack() );
		return M4_ERROR;
	}

	//sacamos nombre de canal
	ret=RuntimePopString(vChannelName);
	if(ret!=M4_SUCCESS){
		m_oDynamicPool->DeleteString(vNodeName.Data.CharData);
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "M4Object Name"<<ai_pccMethod << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vNodeName.Type)<<DumpCallStack() );
		return M4_ERROR;
	}

	//obtenemos el handel de nodo y pAccess
	ClCompiledMCR	*poCMCR;

	////// (DE MOMENTO ESTA EN EL MISMO CANAL)	////
	if( strcmp(m_oDynamicPool->GetString(vChannelName.Data.CharData), "" ) != 0)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_CHANNEL_NAME_NOT_ALLOWED, ai_pccMethod << DumpCallStack() );
		m_oDynamicPool->DeleteString(vNodeName.Data.CharData);
		m_oDynamicPool->DeleteString(vChannelName.Data.CharData);
		return M4_ERROR;
	}	
	
	poCMCR = m_oState1.m_poAccess->GetpCMCR();
	
	// Si no existe el nodo salimos 
	ao_hNode = poCMCR->GetChannelNodeHandleById(m_oDynamicPool->GetString(vNodeName.Data.CharData));

	
	if(ao_hNode == 0 )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, m_oDynamicPool->GetString(vNodeName.Data.CharData)<<ai_pccMethod << DumpCallStack() );
		m_oDynamicPool->DeleteString(vNodeName.Data.CharData);
		m_oDynamicPool->DeleteString(vChannelName.Data.CharData);
		return M4_ERROR;
	}

	m_oDynamicPool->DeleteString(vNodeName.Data.CharData);
	m_oDynamicPool->DeleteString(vChannelName.Data.CharData);

	ao_poAccess=m_oState1.m_poAccess;
	return M4_SUCCESS;
}

//------------------------------------------------------------------------
//YTDSearchAlgorithm
//------------------------------------------------------------------------

/*
Algoritmo de YTD: Esta función pasa registros de la ti de acumulado (ti grande)
a otrs ti's (ti's pequeñas) segun uciertas condiciones, como fechas de pago, impuatación...
Esta función es la genéricam siendo llamada desde dos variantes, una con argumentos fijos (la clásica)
y otra que puede tener además argumentos varaibles.
En la parte debida a los argumentos fijos, puede haber hasta 7 condiciones, y en la parte variable los que
sean.
Esta función recibe un array de variant para poder poner unos fastfilter con los argumentos variables
Para los argumentos fijos, se sacan de la pila de ejecución los 12 argumentos obligatorios
de ambas funciones y se rellenan con las condiciones que correspondan
*/

m4return_t ClVM1PayrollLib::YTDSearchAlgorithm(m4VariantType *ao_vFixedArgs, ClAccess *ai_poAccess, m4uint32_t ai_hNode, StFunctionCmp_t *ai_astfcmpAND, m4VariantType *ai_avFFConditions, m4uint32_t ai_iNumCond, m4uint8_t ai_iLeaveSlices, const m4char_t *ai_pccMethod)
{
	m4return_t ret=M4_SUCCESS;
	m4VariantType	vNull;
	m4uint32_t i,iStartFunc=0;
	m4uint8_t iTotType ;
	
	ClRgAc  * pRgAc = NULL;
	ClRgAc_Base	 * pRgAc_Base = NULL;
	ClAcumTITotalizer_Base * pTIAcTot_Base = NULL;
	ClAcumTITotalizer * pTIAcTot = NULL;

	m4bool_t bTotLast = M4_FALSE;	

	vNull.Type=M4CL_CPP_TYPE_NULL;

	
	m4VariantType &vTot_Op = ao_vFixedArgs[0];
	m4VariantType &vSel_Pays = ao_vFixedArgs[1];
	m4VariantType &vRev_Beh= ao_vFixedArgs[2];
	m4VariantType &vSearchType = ao_vFixedArgs[3];
	m4VariantType &vPayType = ao_vFixedArgs[4];
	m4VariantType &vPayFreq = ao_vFixedArgs[5];
	m4VariantType &vDatePay  = ao_vFixedArgs[6];
	m4VariantType &vDateImp = ao_vFixedArgs[7];
	m4VariantType &vDateEnd = ao_vFixedArgs[8];
	m4VariantType &vDateStart = ao_vFixedArgs[9];


	//////  COMPRUEBO TODOS LOS ARGUMENTOS

	/* Bug 0122180
	Libero cadenas si acaso
	*/
	if(vTot_Op.Type!=M4CL_CPP_TYPE_NUMBER)
	{
		ret=m_oVariantMediator->ToDouble(vTot_Op);
		M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "Totalize_Type"<<ai_pccMethod << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTot_Op.Type)<<DumpCallStack() );
	}

	//Seleccón de pagas (1:Varias, 2:Anterior, 3:Actual) (NUMBER)
	if(vSel_Pays.Type!=M4CL_CPP_TYPE_NUMBER)
	{
		ret=m_oVariantMediator->ToDouble(vSel_Pays);
		M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "Sel_Pays"<<ai_pccMethod << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vSel_Pays.Type)<<DumpCallStack() );
	}

	//Tratamiento en revisiones:(1:Suma,2:First,3:Last) (NUMBER)
	if(vRev_Beh.Type!=M4CL_CPP_TYPE_NUMBER)
	{
		ret=m_oVariantMediator->ToDouble(vRev_Beh);
		M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "Rev. Behav."<<ai_pccMethod<< M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vRev_Beh.Type)<<DumpCallStack() );
	}

	//Search_Type: (1:Imputado,2:Pagado) (NUMBER)
	if(vSearchType.Type!=M4CL_CPP_TYPE_NUMBER)
	{
		ret=m_oVariantMediator->ToDouble(vSearchType);
		M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "Search Type"<<ai_pccMethod<< M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vSearchType.Type)<<DumpCallStack() );
	}

	//Pasamos el PayType y el PayFreq a CharString, para que se pueda utilizar desde fuera.
	if(vPayType.Type!=M4CL_CPP_TYPE_STRING_POOL)
	{
		ret=m_oVariantMediator->ToPoolString(vPayType);
		M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "Pay Type"<<ai_pccMethod<< M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vPayType.Type)<<DumpCallStack() );
	}

// Puede ser que el pool de cadenas se realoque, me tendo que quedar con una copia.
	m4pchar_t pccT1,pccT2 = m_oDynamicPool->GetString( vPayType.Data.CharData );
	pccT1 = new m4char_t [ strlen(pccT2) + 1 ];
	strcpy(pccT1, pccT2);

	m_oDynamicPool->DeleteString(vPayType.Data.CharData);
	
	vPayType.Data.PointerChar = pccT1;
	vPayType.Type = M4CL_CPP_TYPE_STRING_VAR;


	if(vPayFreq.Type!=M4CL_CPP_TYPE_STRING_POOL)
	{
		ret=m_oVariantMediator->ToPoolString(vPayFreq);
		M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "Pay Freq."<<ai_pccMethod<< M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vPayFreq.Type)<<DumpCallStack() );
	}

// Puede ser que el pool de cadenas se realoque, me tendo que quedar con una copia.
	pccT2 = m_oDynamicPool->GetString( vPayFreq.Data.CharData );
	pccT1 = new m4char_t [ strlen(pccT2) + 1 ];
	strcpy(pccT1, pccT2);

	m_oDynamicPool->DeleteString(vPayFreq.Data.CharData);
	
	vPayFreq.Data.PointerChar = pccT1;
	vPayFreq.Type = M4CL_CPP_TYPE_STRING_VAR;	

	
	/* Bug 0122180
	Libero cadenas si acaso
	*/
	if(vDatePay.Type!=M4CL_CPP_TYPE_DATE)
	{
		ret=m_oVariantMediator->ToDate(vDatePay);
		M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "Pay Date."<<ai_pccMethod<< M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDatePay.Type)<<DumpCallStack() );
	}

	if(vDateImp.Type!=M4CL_CPP_TYPE_DATE)
	{
		ret=m_oVariantMediator->ToDate(vDateImp);
		M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "Imputation Date."<<ai_pccMethod<< M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDateImp.Type)<<DumpCallStack() );
	}

	if(vDateEnd.Type!=M4CL_CPP_TYPE_DATE)
	{
		ret=m_oVariantMediator->ToDate(vDateEnd);
		M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "End Date."<<ai_pccMethod<< M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDateEnd.Type)<<DumpCallStack() );
	}

	if(vDateStart.Type!=M4CL_CPP_TYPE_DATE)
	{
		ret=m_oVariantMediator->ToDate(vDateStart);
		M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "Start Date."<<ai_pccMethod<< M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDateStart.Type)<<DumpCallStack() );
	}

	
	ClCompiledMCR	*poCMCR, *poThisCMCR;
	ClHandle		hNodoActual;
	ClItemIndex		iFirst,iSecond;
	ClHandle		hFirst,hSecond;
	m4VariantType	*vFirst=NULL,*vSecond=NULL;
	m4uint32_t		uiSearchIndex;

	//Items fecha inicio y fecha fin del nodo de acumulado, usados para el cálculo
	// de la media ponderada
	ClHandle		hStartDateItem ;
	ClHandle		hEndDateItem ;
	ClItemIndex		iStartDateItemIndex = -1;
	ClItemIndex		iEndDateItemIndex = -1;
	poCMCR=ai_poAccess->GetpCMCR();
	poThisCMCR=m_oState1.m_poAccess->GetpCMCR();

	ClRegister  * poRegTIAcum;
	ClRegister_Current  * poRegTITarget;

	ClRecordSet  oRecordSet(m_oState1.m_poAccess);
	ClNode		 * poNode;

	poNode = &(ai_poAccess->Node[ClHandle(ai_hNode)] ); 

	oRecordSet.SetCurrentBlock(poNode->RecordSet.GetCurrentBlock());
	poRegTIAcum = &oRecordSet.Register;

	
	poRegTITarget = &m_oState1.m_poRecordSet->Current;

	if( poNode->RecordSet.Register.Count() == 0 )
	{
		DUMP_CHLOG_WARNINGF(M4_CH_VM_NO_RECORDS, M4ObjNodeN(poCMCR,ai_hNode)<<ai_pccMethod<< DumpCallStack() );
		// Añado un registro, para que coja los default values.
		poRegTITarget->Add();
		
		delete [] vPayType.Data.PointerChar;
		delete [] vPayFreq.Data.PointerChar;

		RuntimePushDouble(M4_ERROR);
		return M4_SUCCESS;
	}

	hNodoActual = m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle();

	///	Buscamos los item comunes a ambas TIs
	
	VectorItemIndex	IndexSource,IndexTarget;
	m4int32_t iNumItems=0;

	_FindEqualItems(hNodoActual, poThisCMCR, IndexTarget,ai_hNode, poCMCR, IndexSource, iNumItems );

	if(iNumItems <= 0 )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_ITEM_MATCHES, ai_pccMethod << DumpCallStack() );
		delete [] vPayType.Data.PointerChar;
		delete [] vPayFreq.Data.PointerChar;

		RuntimePushDouble(M4_ERROR);
		return M4_ERROR;
	}

	VIndAndTotType vIndexAndTotType;

	for( m4int32_t yy = 0; yy < iNumItems ; ++yy )
	{
		vIndexAndTotType.insert( vIndexAndTotType.end(), stIndexAndTotType_t( IndexSource[yy], poThisCMCR->GetItemSliceTotalize( poThisCMCR->GetNodeItemHandleByPosition( hNodoActual, (m4uint16_t)IndexTarget[yy] ) ) ) );
	}

	

	///////////////CONDICIONES (FILTRO DE LOS REGISTROS)/////////////

	/////////////////////////////////////////////////////////////////
	///////////////////SEGUN LA ORIENTACION//////////////////////////
				// (Se decide el tipo de ordenación) //
	ClHandle	hPT,hPF;

	switch((m4char_t)vSearchType.Data.DoubleData)
	{
	case M4_CL_VM_PAYROLL_SEARCH_TYPE_IMPUTATION:
	///		iFirst = F.IMPUT	;	iSecond = F.PAGO		///
		hFirst=poCMCR->GetNodeImputeDateItem(ai_hNode);
		vFirst=&vDateImp;
		hSecond=poCMCR->GetNodePayDateItem(ai_hNode);
		vSecond=&vDatePay;
		uiSearchIndex = 1;
		hPT= poCMCR->GetNodeImputePayTypeItem(ai_hNode);
		hPF = poCMCR->GetNodeImputePayFrequencyItem(ai_hNode);
		break;
		
	case M4_CL_VM_PAYROLL_SEARCH_TYPE_PAY:
	///		iFirst = F.PAGO	;	iSecond = F.IMPUT		///
		hFirst = poCMCR->GetNodePayDateItem(ai_hNode);
		vFirst = &vDatePay;
		hSecond = poCMCR->GetNodeImputeDateItem(ai_hNode);
		vSecond = &vDateImp;
		uiSearchIndex = 2;
		hPT= poCMCR->GetNodePayTypeItem(ai_hNode);
		hPF = poCMCR->GetNodePayFrequencyItem(ai_hNode);

		break;

	default:
	
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "SearchType"<<ai_pccMethod<<vSearchType.Data.DoubleData << DumpCallStack() );

		delete [] vPayType.Data.PointerChar;
		delete [] vPayFreq.Data.PointerChar;
	
		RuntimePushDouble(M4_ERROR);
		return M4_ERROR;
		break;
	}


	if( hFirst ==0 || hSecond == 0 || hPT == 0 || hPF == 0)	
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_UNDEFINED_ITEM, "ImputationDate|PayDate|PayType|PayFreq"<<NodeN(poCMCR,ai_hNode)<<ai_pccMethod << DumpCallStack() );
		delete [] vPayType.Data.PointerChar;
		delete [] vPayFreq.Data.PointerChar;

		RuntimePushDouble(M4_ERROR);
		return M4_ERROR;
	}

	iFirst = poCMCR->GetItemIndex(hFirst);
	iSecond = poCMCR->GetItemIndex(hSecond);

	hStartDateItem = poCMCR->GetNodeStartDateItem(ai_hNode) ;
	hEndDateItem = poCMCR->GetNodeEndDateItem(ai_hNode) ;

	if (hStartDateItem != 0 && hEndDateItem != 0 )
	{
		iStartDateItemIndex = poCMCR->GetItemIndex(hStartDateItem) ;
		iEndDateItemIndex = poCMCR->GetItemIndex(hEndDateItem) ;
	}

	/////////////////DEBE ESTAR ORDENADO POR iFirst desc,iSecond desc///


	/* Bug 0092287
	Hay que chequear que el índice exista y sea correcto y sino dar un warning
	*/

	m4uint32_t iIndexHandle = poCMCR->GetNodeIndexHandleById( ai_hNode, uiSearchIndex ) ;

	if( iIndexHandle != 0 )
	{
		m4uint16_t iItemNumber = poCMCR->GetIndexNumberOfItems( iIndexHandle ) ;

		if( iItemNumber != 2 )
		{
			iIndexHandle = 0 ;
		}
		else
		{
			m4uint32_t iFirstItemHandle = poCMCR->GetIndexItemHandleByPosition( iIndexHandle, 0 ) ;
			m4uint32_t iSecondItemHandle = poCMCR->GetIndexItemHandleByPosition( iIndexHandle, 1 ) ;
			m4uint8_t iFirstItemWay = poCMCR->GetIndexWayByPosition( iIndexHandle, 0 ) ;
			m4uint8_t iSecondItemWay = poCMCR->GetIndexWayByPosition( iIndexHandle, 1 ) ;

			if( iFirstItemHandle != hFirst || iSecondItemHandle != hSecond || iFirstItemWay != M4CL_INDEX_WAY_DESCENDING || iSecondItemWay != M4CL_INDEX_WAY_DESCENDING )
			{
				iIndexHandle = 0 ;
			}
		}
	}


	if( iIndexHandle != 0 )
	{
		oRecordSet.SetCurrentSearchIndexById(uiSearchIndex);
		oRecordSet.Arrange();
	}
	else
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_BAD_YTDSEARCH_INDEX, uiSearchIndex << M4ObjNodeN( poCMCR, ai_hNode ) ) ;
	}

	////////////////////////////////////////////////////////////////////
	///////////////SEGUN LA SELECCION DE PAGAS///////////////////////

	switch((m4char_t)vSel_Pays.Data.DoubleData)
	{
	case M4_CL_VM_PAYROLL_SELECT_PAY_MANY_PAYS:

	//FEC_IMPUTACION>=DATE_START
	//FEC_IMPUTACION<=DATE_END
	//FEC_IMPUTACION<=DATE_IMP 

		ai_astfcmpAND[iStartFunc].Fill(iFirst,vDateStart,M4VariantIsBiggerOrEqual);
		iStartFunc++;
		ai_astfcmpAND[iStartFunc].Fill(iFirst,vDateEnd,M4VariantIsSmallerOrEqual);
		iStartFunc++;
//		Se quita esta condicion, para que la Query pueda traer pagas a futuro.
//		En nomina se filtra y no se traen nunca pagas futuras, con lo que no hay problema.
/*		ai_astfcmpAND[iStartFunc].Fill(iFirst,*vFirst,M4VariantIsSmaller);
		iStartFunc++;
*/		
		break;	

	case M4_CL_VM_PAYROLL_SELECT_PAY_ACTUAL:
		
	//FEC_IMPUTACION = F.Imp
	//FEC_PAGO < F.pago
		ai_astfcmpAND[iStartFunc].Fill(iFirst,(*vFirst),M4VariantIsEqual);
		iStartFunc++;
		ai_astfcmpAND[iStartFunc].Fill(iSecond,(*vSecond),M4VariantIsSmaller);
		iStartFunc++;

	//Totalizador

		pRgAc_Base = new ClRgAc_Base( poRegTIAcum, ai_astfcmpAND,  &vIndexAndTotType,iNumItems, M4CL_ITEM_TOTALIZE_FIRST, iFirst, iSecond, iStartDateItemIndex, iEndDateItemIndex, poRegTIAcum->Index);
		pTIAcTot_Base = new ClAcumTITotalizer_Base;
		pTIAcTot_Base->Init(pRgAc_Base);
		break;

	case M4_CL_VM_PAYROLL_SELECT_PAY_ACTUAL_FOR_PATRO_I: //(Actual for PATRO)

		ai_astfcmpAND[iStartFunc].Fill(iFirst,(*vFirst),M4VariantIsEqual);
		iStartFunc++;
		ai_astfcmpAND[iStartFunc].Fill(iSecond,(*vSecond),M4VariantIsSmallerOrEqual);
		iStartFunc++;

	//Totalizador

		pRgAc_Base = new ClRgAc_Base( poRegTIAcum, ai_astfcmpAND,  &vIndexAndTotType,iNumItems, M4CL_ITEM_TOTALIZE_FIRST, iFirst, iSecond, iStartDateItemIndex, iEndDateItemIndex, poRegTIAcum->Index);
		pTIAcTot_Base = new ClAcumTITotalizer_Base;
		pTIAcTot_Base->Init(pRgAc_Base);
		break;

	case M4_CL_VM_PAYROLL_SELECT_PAY_ACTUAL_FOR_PATRO_II: //(Actual for PATRO II)

		ai_astfcmpAND[iStartFunc].Fill(iFirst,(*vFirst),M4VariantIsEqual);
		iStartFunc++;

	//Totalizador

		pRgAc_Base = new ClRgAc_Base( poRegTIAcum, ai_astfcmpAND,  &vIndexAndTotType,iNumItems, M4CL_ITEM_TOTALIZE_FIRST, iFirst, iSecond, iStartDateItemIndex, iEndDateItemIndex,  poRegTIAcum->Index);
		pTIAcTot_Base = new ClAcumTITotalizer_Base;
		pTIAcTot_Base->Init(pRgAc_Base);
		break;

	case M4_CL_VM_PAYROLL_SELECT_PAY_BEFORE:

	//FEC_IMPUTACION < F.Imp
		ai_astfcmpAND[iStartFunc].Fill(iFirst,(*vFirst),M4VariantIsSmaller);
		iStartFunc++;
	//Totalizador
		pRgAc_Base = new ClRgAc_Base( poRegTIAcum, ai_astfcmpAND, &vIndexAndTotType,iNumItems, M4CL_ITEM_TOTALIZE_FIRST, iFirst, iSecond, iStartDateItemIndex, iEndDateItemIndex, poRegTIAcum->Index);
		pTIAcTot_Base = new ClAcumTITotalizer_Base;
		pTIAcTot_Base->Init(pRgAc_Base);

		break;

	default:
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "SELECT_PAY"<<ai_pccMethod<<vSel_Pays.Data.DoubleData << DumpCallStack() );
		delete [] vPayType.Data.PointerChar;
		delete [] vPayFreq.Data.PointerChar;

		if( pRgAc ) delete pRgAc;
		if( pTIAcTot ) delete pTIAcTot;
		if( pRgAc_Base ) delete pRgAc_Base;
		if( pTIAcTot_Base ) delete pTIAcTot_Base;

		RuntimePushDouble(M4_ERROR);
		return M4_ERROR;
	}
	////////////////////////////////////////////////////////////////////
	

	
	////////////////////////////////////////////////////////////////////
	///////////////SEGUN EL TRATAMIENTO EN REVISIONES///////////////////

	switch((m4char_t)vRev_Beh.Data.DoubleData)
	{
	case M4_CL_VM_PAYROLL_REVISION_BEH_FIRST:

		// FEC_IMPUTACION = F.Pago
		ai_astfcmpAND[iStartFunc].Fill(iFirst,vNull,M4VariantIsEqual,iSecond);
		iStartFunc++;
		break;

	case M4_CL_VM_PAYROLL_REVISION_BEH_SUM:	
		//TOTALIZACION
		if(pRgAc_Base == NULL)
		{
			pRgAc_Base = new ClRgAc_Base( poRegTIAcum, ai_astfcmpAND,&vIndexAndTotType,iNumItems, M4CL_ITEM_TOTALIZE_SUM, iFirst, iSecond, iStartDateItemIndex, iEndDateItemIndex, poRegTIAcum->Index);
			pTIAcTot_Base = new ClAcumTITotalizer_Base;
			pTIAcTot_Base->Init(pRgAc_Base);
		}
		else
		{
			pRgAc_Base->UpdateTotType(M4CL_ITEM_TOTALIZE_SUM);
		}

		break;
		
	case M4_CL_VM_PAYROLL_REVISION_BEH_LAST:
		/* Bug 0088824, 0091933
		El bug 0088824 al final era falso. Aquí no hay que cambiar el totalizador,
		porque todo el proceso se basa en que los datos se han ordenado por fecha
		de imputación o fecha de pago, pero de manera DESCENDENTE, de forma que
		los datos últimos están los primeros.
		Para que esto funcione correctamente la TI de lectura de acumulado
		debe tener obligatoriamente definidos los índices 1 (por fecha de imputación
		fecha de pago) y 2 (por fecha de pago fecha de imputación) y descendentes
		ambas columnas.
		El caso de totalizador first funciona porque se añade un filtro donde
		la fecha de imputación sea igual a la fecha de pago, de forma que sólo son
		visibles los primeros registros.
		*/
		break;

	default:
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "RevisionBehaviour"<<ai_pccMethod<<vRev_Beh.Data.DoubleData << DumpCallStack() );
		delete [] vPayType.Data.PointerChar;
		delete [] vPayFreq.Data.PointerChar;

		if( pRgAc ) delete pRgAc;
		if( pTIAcTot ) delete pTIAcTot;
		if( pRgAc_Base ) delete pRgAc_Base;
		if( pTIAcTot_Base ) delete pTIAcTot_Base;
		
		RuntimePushDouble(M4_ERROR);
		return M4_ERROR;
		break;
	}

	//////////////FALTA EL PAY FREQUENCY (CONDICIONES OR) Y EL PAY TYPE///////////
	
	ClItemIndex iPayFreq,iPayType;
// El handle del Pay Type y el Pay Frequency, lo obtenemos arriba.
//	hPT= poCMCR->GetNodePayTypeItem(ai_hNode);
//	hPF = poCMCR->GetNodePayFrequencyItem(ai_hNode);


	iPayType = poCMCR->GetItemIndex(hPT);
	iPayFreq = poCMCR->GetItemIndex(hPF);
	
	ai_astfcmpAND[iStartFunc].Fill(iPayFreq,vPayFreq,M4SearchVarStringInVarString);
	iStartFunc++;
	ai_astfcmpAND[iStartFunc].Fill(iPayType,vPayType,M4SearchVarStringInVarString);
	iStartFunc++;

	/////////////////////////////////////////////////////////////



	//////////////////////////TOTALIZACION////////////////////////

	if(pTIAcTot_Base == NULL)
	{
		switch((m4char_t)vRev_Beh.Data.DoubleData)
		{
		case M4_CL_VM_PAYROLL_REVISION_BEH_FIRST:

			///utilizando el totalize de dani//		
			pRgAc_Base = new ClRgAc_Base( poRegTIAcum, ai_astfcmpAND, &vIndexAndTotType,iNumItems,(m4char_t)vTot_Op.Data.DoubleData, iFirst, iSecond, iStartDateItemIndex, iEndDateItemIndex, poRegTIAcum->Index);
			pTIAcTot_Base = new ClAcumTITotalizer_Base;
			pTIAcTot_Base->Init(pRgAc_Base);

			//pfTotal1 = afTotalize[(m4char_t)vTot_Op.Data.DoubleData];
			break;

		case M4_CL_VM_PAYROLL_REVISION_BEH_LAST:

			pRgAc_Base = new ClRgAc_Base( poRegTIAcum, ai_astfcmpAND,&vIndexAndTotType,iNumItems,M4CL_ITEM_TOTALIZE_FIRST, iFirst, iSecond, iStartDateItemIndex, iEndDateItemIndex, poRegTIAcum->Index);
			pTIAcTot_Base = new ClAcumTITotalizer_Base;
			pTIAcTot_Base->Init(pRgAc_Base);
			// Desgraciadamente si el tipo de totalización es Last hay que distinguir.
			if((m4char_t)vTot_Op.Data.DoubleData == M4CL_ITEM_TOTALIZE_LAST)
			{
				bTotLast = M4_TRUE;
			}
			else 
			{
				pRgAc = new ClRgAc(pTIAcTot_Base, pRgAc_Base,(m4uint8_t)vTot_Op.Data.DoubleData);
				pRgAc_Base->SetMustStop();
				pTIAcTot = new ClAcumTITotalizer;
				pTIAcTot->Init(pRgAc);
			}

			//pfTotal1 = afTotalize[(m4char_t)vTot_Op.Data.DoubleData];
			break;

		default:

			//Hay que mirar, porque no debería llegarme ninguno con Defualt;
			break;
		}
	}
	else if(vSel_Pays.Data.DoubleData == M4_CL_VM_PAYROLL_SELECT_PAY_MANY_PAYS &&
		vRev_Beh.Data.DoubleData != M4_CL_VM_PAYROLL_REVISION_BEH_FIRST )
	{	
		if((m4char_t)vTot_Op.Data.DoubleData != M4CL_ITEM_TOTALIZE_LAST)
		{
			switch((m4char_t) vRev_Beh.Data.DoubleData)
			{
//			case M4_CL_VM_PAYROLL_REVISION_BEH_FIRST:
			
			case M4_CL_VM_PAYROLL_REVISION_BEH_LAST:
			case M4_CL_VM_PAYROLL_REVISION_BEH_SUM:
			
				///utilizando el totalize de dani//		
				pRgAc = new ClRgAc(pTIAcTot_Base, pRgAc_Base ,(m4uint8_t)vTot_Op.Data.DoubleData);
				pRgAc_Base->SetMustStop();
				pTIAcTot = new ClAcumTITotalizer;
				pTIAcTot->Init(pRgAc);
//				pfTotal2 = afTotalize[(m4char_t)vTot_Op.Data.DoubleData];

				break;
			
			default:
				break;
			}
		}
		// Desgraciadamente si el tipo de totalización es Last hay que distinguir.		 
		 else
		 {
			 bTotLast = M4_TRUE;
		 }
	}
	else if(vSel_Pays.Data.DoubleData == M4_CL_VM_PAYROLL_SELECT_PAY_BEFORE&&
			vRev_Beh.Data.DoubleData == M4_CL_VM_PAYROLL_REVISION_BEH_SUM )
	{
		// Hay que distinguir.
		// Vamos a hacer el First del Sum.
		pRgAc_Base->UpdateTotType(M4CL_ITEM_TOTALIZE_SUM);
		pRgAc = new ClRgAc(pTIAcTot_Base, pRgAc_Base ,(m4uint8_t)M4CL_ITEM_TOTALIZE_FIRST);
		pRgAc_Base->SetMustStop();
		pTIAcTot = new ClAcumTITotalizer;
		pTIAcTot->Init(pRgAc);
	}
	/////////////////////////////////////////////////////////////////
	
	
	// Para finalizar las comparaciones.
	ai_astfcmpAND[iStartFunc].Fill(0,vNull,NULL);	

	for(i=0; i<ai_iNumCond*3;i+=3)
	{
		/* Bug 0071076
		El filtro debe ser estático o sino la performance se deteriora
		*/
		poRegTIAcum->Filter.AddFastFilter(m4uint32_t(ai_avFFConditions[i+2].Data.DoubleData) , ai_avFFConditions[i+1], (m4uint32_t)ai_avFFConditions[i].Data.DoubleData ,ClFilterInterface::StaticFilter);
	}

	poRegTIAcum->Begin();

	if(bTotLast == M4_TRUE)
	{
		pRgAc_Base->SetMustStop();
		pRgAc_Base->End();
	}

	ClVectorVariants vvVector(iNumItems);

	if(pTIAcTot != NULL)
	{
		iTotType = pRgAc->GetTotType() ;
		
		/* Bug 0082332 Las cadenas no se suman y nos quedamos con la primera */
		pTIAcTot->GetNoConcat(vvVector);
	}
	else
	{
		if(pTIAcTot_Base != NULL )
		{
			iTotType = pRgAc_Base->GetTotType() ;

			/* Bug 0082332 Las cadenas no se suman y nos quedamos con la primera */
			ret = pTIAcTot_Base->GetNoConcat(vvVector);
		}
		else
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "TotalizeType"<<ai_pccMethod<<vTot_Op.Data.DoubleData << DumpCallStack() );
			delete [] vPayType.Data.PointerChar;
			delete [] vPayFreq.Data.PointerChar;
			
			if( pRgAc ) delete pRgAc;
			if( pTIAcTot ) delete pTIAcTot;
			if( pRgAc_Base ) delete pRgAc_Base;
			if( pTIAcTot_Base ) delete pTIAcTot_Base;

			//quitamos los fastfilter, si hubiera
			for(i=0; i<ai_iNumCond;++i)
			{
				poRegTIAcum->Filter.DeleteLast();
			}
			
			RuntimePushDouble(M4_ERROR);
			return M4_ERROR;
		}
	}
	

	m4int32_t k ;
	m4VariantType vTemp;
	
	//copia de valores
	if( ret == M4_SUCCESS )
	{
		if(ai_iLeaveSlices==M4_CL_VM_YTD_KEEP_SLICES){ //squieren que dejemos tramos
			//lo quieren con tramos, metemos valores entre fechas
			
			poRegTITarget->Begin();
			//si no hay un registro añadimos
			if(!poRegTITarget->Count())
			{
				ret = poRegTITarget->Add();
			}

			//metemos valores			
			/* Bug 0077048
			Si es un count no hace falta que haya registros
			*/
			if(ret == M4_SUCCESS && ( pRgAc_Base->FoundRegs() == M4_TRUE || iTotType == M4CL_ITEM_TOTALIZE_COUNT ) )
			{
				for(k=0; k < iNumItems; k++ )
				{
					//metemos el tramo
					ret = poRegTITarget->Item[ IndexTarget[k] ].Value.AddSlice(m_oState1.m_dBeginDate, m_oState1.m_dEndDate);

					if( ret != M4_ERROR)
					{
						//nos movemos al tramo y asignamos //no hace falta mover el item, ya está puesto en el addslice
						poRegTITarget->Item.Value.MoveTo(m_oState1.m_dBeginDate);
						poRegTITarget->Item.Value.Set(vvVector [k]);
						poRegTITarget->Item.Value.Compact();
					}
				}
			}
		}
		else
		{ //no dejamos tramos!!! si hubiere + de 1 registro->se los totalizamos o no según el argumentos
			//añadimos un registro y copiamos los valores
			ret = poRegTITarget->Add();
			/* Bug 0077048
			Si es un count no hace falta que haya registros
			*/
			if(ret == M4_SUCCESS && ( pRgAc_Base->FoundRegs() == M4_TRUE || iTotType == M4CL_ITEM_TOTALIZE_COUNT ) )
			{
				for( k = 0; k < iNumItems; k++ )
				{
					poRegTITarget->Item[ IndexTarget[k] ].Value.Set(vvVector [k]);
				}
			}

			if(poRegTITarget->Count()>1 && ai_iLeaveSlices==M4_CL_VM_YTD_ONE_RECORD)
			{
				//si tenemos más de un registro, habra que totalizar (ya que no quieren tramos)
				//totalizamos
				poRegTITarget->Begin(); //en el primer registro quedarán los totales

				for(k = 0; k < iNumItems; k++ )
				{
					/* Bug 0082332 Las cadenas no se suman y nos quedamos con la primera */
					ret = m_oState1.m_poRecordSet->GetFooter()[ IndexTarget[k] ].GetNoConcat(vTemp);
					if(ret==M4_SUCCESS)
					{
						poRegTITarget->Item[ IndexTarget[k] ].Value.Set(vTemp);
					}
					//si da error la asignación pasamos, pero si es la totalización la que da error, no asignamos
				}
				
				//nos cepillamos todos los registros menos el primero, que es donde están los valores totalizados
				poRegTITarget->MoveTo(1); //hay más de 1
				while(poRegTITarget->Index != M4DM_EOF_INDEX)
				{
					poRegTITarget->Destroy();
				}
			}
		}
	}

	//quitamos los fastfilter, si hubiera
	for(i=0; i<ai_iNumCond;++i)
	{
		poRegTIAcum->Filter.DeleteLast();
	}

	//nos situamos en el nodo destino
	poRegTITarget->Begin();
	

	delete [] vPayType.Data.PointerChar;
	delete [] vPayFreq.Data.PointerChar;

	if( pRgAc ) delete pRgAc;
	if( pTIAcTot ) delete pTIAcTot;
	if( pRgAc_Base ) delete pRgAc_Base;
	if( pTIAcTot_Base ) delete pTIAcTot_Base;
	
	RuntimePushDouble(M4_SUCCESS);
	return ret;
}


m4uint32_t ClVM1PayrollLib::GetSize(void) const{
	m4uint32_t length =0;
	length += m_oDatesStack.GetSize() *sizeof(m4date_t);
	if(m_poPayRoll)
		length += sizeof(ClVMPayRoll);
	length += sizeof(ClVM1PayrollLib);
	return length ;
}
