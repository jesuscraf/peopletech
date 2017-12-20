
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
//    Virtual machine de nivel 1- Funciones del canal de moneda dentro de las de nomina
////
//==============================================================================

#include "vm1payroll.hpp"

#include "vmachin1.h"
#include "vmres.hpp"

#include "m4log.hpp"
#include "index.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "m4objglb.hpp"
#include "m4mdrt.hpp"
#include "nodedef.hpp"


m4return_t ClVM1PayrollLib::curExchangeRecord    (void){
	m4VariantType vDstCur, vDate, vFlag, vArgument, vExType;
	m4pchar_t pcDstCur, pcExType;
	ClAccessRecordSet  *poRecordSet;
	ClRegisterIndex	oRegisterIndex;
	m4uint32_t iNumArgs;

	m4return_t ret = M4_SUCCESS;


	ret = RuntimePop(vArgument);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "number of args.curExchangeRecord.")	;

	iNumArgs = (m4int32_t) vArgument.Data.DoubleData;

	if ( iNumArgs == 4)
	{
		ret = RuntimePop (vExType);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vExType.curExchangeRecord.")	;

		ret = RuntimePop (vDate);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vDate.curExchangeRecord.")	;
	}	
	else if ( iNumArgs == 3)
	{
		ret = RuntimePop (vDate);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vDate.curExchangeRecord.")	;

		vExType.Type=M4CL_CPP_TYPE_NULL;
		vExType.Data.PointerChar = 0;

	}	
	else if (iNumArgs == 2)
	{
		vDate.Type=M4CL_CPP_TYPE_DATE;
		vDate.Data.DoubleData = 0;

		vExType.Type=M4CL_CPP_TYPE_NULL;
		vExType.Data.PointerChar = 0;
	}
	else
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "curExchangeRecord"<<"2-4" << DumpCallStack() );
		return M4_ERROR;
	}

    ret = RuntimePop (vFlag);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vFlag.curExchangeRecord.")	;

    ret = RuntimePop (vDstCur);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vDstCur.curExchangeRecord.")	;

	if (vFlag.Type!=M4CL_CPP_TYPE_NUMBER){
        ret=m_oVariantMediator->ToDouble (vFlag);  
    }
    if (ret!=M4_SUCCESS){ 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Flag"<<"curExchangeRecord" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vFlag.Type)<<DumpCallStack() );
        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
    
	if (vDate.Type!=M4CL_CPP_TYPE_DATE){
        ret=m_oVariantMediator->ToDate (vDate);
    }
    if (ret!=M4_SUCCESS){ 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Date"<<"curExchangeRecord" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDate.Type)<<DumpCallStack() );
        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

	if ((vExType.Type!=M4CL_CPP_TYPE_STRING_POOL) && (vExType.Type!=M4CL_CPP_TYPE_NULL)){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "ExType"<<"curExchangeRecord" << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<", "<< M4ClCppType(M4CL_CPP_TYPE_NULL) )<<M4ClCppType(vExType.Type)<<DumpCallStack() );
        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
        
    if (vDstCur.Type!=M4CL_CPP_TYPE_STRING_POOL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Dest_currency"<<"curExchangeRecord" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vDstCur.Type)<< DumpCallStack() );
        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

    

	if (vExType.Type==M4CL_CPP_TYPE_STRING_POOL)
		pcExType = m_oDynamicPool->GetString(vExType.Data.CharData);
	else if (vExType.Type==M4CL_CPP_TYPE_NULL)
		pcExType = 0;

	pcDstCur = m_oDynamicPool->GetString(vDstCur.Data.CharData); 

	poRecordSet = m_oState1.m_poRecordSet;
	
	oRegisterIndex = m_oState1.m_poRecordSet->Current.Index;

	//si no llega fecha pasar un 0.
	ret = _curExchangeRecord (&poRecordSet->Register[oRegisterIndex], pcDstCur, pcExType, vDate.Data.DoubleData, (m4int8_t)vFlag.Data.DoubleData, M4_FALSE);

	if (ret == M4_ERROR)
	{
		if (iNumArgs == 4)
		{
			m4char_t psStringDate[80];
	        ClDateToString(vDate.Data.DoubleData,psStringDate,sizeof(psStringDate));
		}
		else if (iNumArgs == 3)
		{
			m4char_t psStringDate[80];
	        ClDateToString(vDate.Data.DoubleData,psStringDate,sizeof(psStringDate));
		}
		else
		{
		}
	}

	//borramos las cadenas
	if (vExType.Type != M4CL_CPP_TYPE_NULL)
		m_oDynamicPool->DeleteString(vExType.Data.CharData);
	m_oDynamicPool->DeleteString(vDstCur.Data.CharData);

    RuntimePushDouble(ret);

    return ret;
}


m4return_t ClVM1PayrollLib::curExchangeRecordRounded    (void){
	m4VariantType vDstCur, vDate, vFlag, vArgument, vExType;
	m4pchar_t pcDstCur, pcExType;
	ClAccessRecordSet  *poRecordSet;
	ClRegisterIndex	oRegisterIndex;
	m4uint32_t iNumArgs;

	m4return_t ret = M4_SUCCESS;


	ret = RuntimePop(vArgument);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "number of args.curExchangeRecord.")	;

	iNumArgs = (m4int32_t) vArgument.Data.DoubleData;

	if ( iNumArgs == 4)
	{
		ret = RuntimePop (vExType);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vExType.curExchangeRecord.")	;

		ret = RuntimePop (vDate);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vDate.curExchangeRecord.")	;
	}	
	else if ( iNumArgs == 3)
	{
		ret = RuntimePop (vDate);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vDate.curExchangeRecord.")	;

		vExType.Type=M4CL_CPP_TYPE_NULL;
		vExType.Data.PointerChar = 0;

	}	
	else if (iNumArgs == 2)
	{
		vDate.Type=M4CL_CPP_TYPE_DATE;
		vDate.Data.DoubleData = 0;

		vExType.Type=M4CL_CPP_TYPE_NULL;
		vExType.Data.PointerChar = 0;
	}
	else
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "curExchangeRecord"<<"2-4" << DumpCallStack() );
		return M4_ERROR;
	}

    ret = RuntimePop (vFlag);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vFlag.curExchangeRecord.")	;

    ret = RuntimePop (vDstCur);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vDstCur.curExchangeRecord.")	;

	if (vFlag.Type!=M4CL_CPP_TYPE_NUMBER){
        ret=m_oVariantMediator->ToDouble (vFlag);  
    }
    if (ret!=M4_SUCCESS){ 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Flag"<<"curExchangeRecord" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vFlag.Type)<<DumpCallStack() );
        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
    
	if (vDate.Type!=M4CL_CPP_TYPE_DATE){
        ret=m_oVariantMediator->ToDate (vDate);
    }
    if (ret!=M4_SUCCESS){ 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Date"<<"curExchangeRecord" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDate.Type)<<DumpCallStack() );
        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

	if ((vExType.Type!=M4CL_CPP_TYPE_STRING_POOL) && (vExType.Type!=M4CL_CPP_TYPE_NULL)){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "ExType"<<"curExchangeRecord" << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<", "<< M4ClCppType(M4CL_CPP_TYPE_NULL) )<<M4ClCppType(vExType.Type)<<DumpCallStack() );
        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
        
    if (vDstCur.Type!=M4CL_CPP_TYPE_STRING_POOL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Dest_currency"<<"curExchangeRecord" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vDstCur.Type)<< DumpCallStack() );
        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

    

	if (vExType.Type==M4CL_CPP_TYPE_STRING_POOL)
		pcExType = m_oDynamicPool->GetString(vExType.Data.CharData);
	else if (vExType.Type==M4CL_CPP_TYPE_NULL)
		pcExType = 0;

	pcDstCur = m_oDynamicPool->GetString(vDstCur.Data.CharData); 

	poRecordSet = m_oState1.m_poRecordSet;
	
	oRegisterIndex = m_oState1.m_poRecordSet->Current.Index;

	//si no llega fecha pasar un 0.
	ret = _curExchangeRecord (&poRecordSet->Register[oRegisterIndex], pcDstCur, pcExType, vDate.Data.DoubleData, (m4int8_t)vFlag.Data.DoubleData, M4_TRUE);

	if (ret == M4_ERROR)
	{
		if (iNumArgs == 4)
		{
			m4char_t psStringDate[80];
	        ClDateToString(vDate.Data.DoubleData,psStringDate,sizeof(psStringDate));
		}
		else if (iNumArgs == 3)
		{
			m4char_t psStringDate[80];
	        ClDateToString(vDate.Data.DoubleData,psStringDate,sizeof(psStringDate));
		}
		else
		{
		}
	}

	//borramos las cadenas
	if (vExType.Type != M4CL_CPP_TYPE_NULL)
		m_oDynamicPool->DeleteString(vExType.Data.CharData);
	m_oDynamicPool->DeleteString(vDstCur.Data.CharData);

    RuntimePushDouble(ret);

    return ret;
}



m4return_t ClVM1PayrollLib::curGetExchange       (void)
{	
	//Por ahora devuelve el vValue convertido
	m4VariantType vSrcCur, vDstCur, vExType, vDate, vValue;
	m4pchar_t pcSrcCur, pcDstCur, pcExType;
	m4double_t dRate = 1;
	m4bool_t bExchangeFound;

	m4return_t ret = M4_SUCCESS;

    ret = RuntimePop (vValue);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vValue.curGetExchange.")	;
       
    ret = RuntimePop (vExType);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vExType.curGetExchange.")	;

    ret = RuntimePop (vDate);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vDate.curGetExchange.")	;

    ret = RuntimePop (vDstCur);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vDstCur.curGetExchange.")	;

    ret = RuntimePop (vSrcCur);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vSrcCur.curGetExchange.")	;

    
	if (vValue.Type!=M4CL_CPP_TYPE_NUMBER){
        ret=m_oVariantMediator->ToDouble (vValue);
    }
    if (ret!=M4_SUCCESS){ 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Money_value"<<"curGetExchange" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vValue.Type)<< DumpCallStack() );
		RuntimePush (vValue); //res=valor inicial
        //RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
    
	if (vDate.Type!=M4CL_CPP_TYPE_DATE){
        ret=m_oVariantMediator->ToDate (vDate);
    }
    if (ret!=M4_SUCCESS){ 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Date"<<"curGetExchange" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDate.Type)<<DumpCallStack() );
		RuntimePush (vValue); //res=valor inicial
        //RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

	if (vExType.Type!=M4CL_CPP_TYPE_STRING_POOL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "ExType"<<"curGetExchange" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vExType.Type)<<DumpCallStack() );
		RuntimePush (vValue); //res=valor inicial
        //RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
        
    if (vDstCur.Type!=M4CL_CPP_TYPE_STRING_POOL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Dest_currency"<<"curGetExchange" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vDstCur.Type)<<DumpCallStack() );
		RuntimePush (vValue); //res=valor inicial
        //RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

    if (vSrcCur.Type!=M4CL_CPP_TYPE_STRING_POOL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Source_Currency"<<"curGetExchange" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vSrcCur.Type)<<DumpCallStack() );
		RuntimePush (vValue); //res=valor inicial
        //RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

	pcExType = m_oDynamicPool->GetString(vExType.Data.CharData); 
   	pcSrcCur = m_oDynamicPool->GetString(vSrcCur.Data.CharData); 
	pcDstCur = m_oDynamicPool->GetString(vDstCur.Data.CharData); 

	ret = _curGetExchangeRate(pcSrcCur, pcDstCur, pcExType, vDate.Data.DoubleData, dRate, bExchangeFound, NULL, m_oState1.m_poAccess);

	if (ret!=M4_ERROR)
	{
		vValue.Data.DoubleData *= dRate;  //Cambiamos vValue
	}
	else
	{
		m4char_t psStringDate[80];
        ClDateToString(vDate.Data.DoubleData,psStringDate,sizeof(psStringDate));
	}

	//borramos las cadenas
	m_oDynamicPool->DeleteString(vExType.Data.CharData);
	m_oDynamicPool->DeleteString(vSrcCur.Data.CharData);
	m_oDynamicPool->DeleteString(vDstCur.Data.CharData);
	
    RuntimePush (vValue); 

    return ret;

}


m4return_t ClVM1PayrollLib::curGetExchangeRounded       (void)
{	
	//Por ahora devuelve el vValue convertido
	m4VariantType vSrcCur, vDstCur, vExType, vDate, vValue, vDecimals;
	m4pchar_t pcSrcCur, pcDstCur, pcExType;
	m4double_t dRate = 1;
	m4bool_t bExchangeFound;
	
	m4return_t ret = M4_SUCCESS;

    ret = RuntimePop (vValue);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vValue.curGetExchange.")	;
       
    ret = RuntimePop (vExType);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vExType.curGetExchange.")	;

    ret = RuntimePop (vDate);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vDate.curGetExchange.")	;

    ret = RuntimePop (vDstCur);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vDstCur.curGetExchange.")	;

    ret = RuntimePop (vSrcCur);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vSrcCur.curGetExchange.")	;

    
	if (vValue.Type!=M4CL_CPP_TYPE_NUMBER){
        ret=m_oVariantMediator->ToDouble (vValue);
    }
    if (ret!=M4_SUCCESS){ 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Money_value"<<"curGetExchange" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vValue.Type)<< DumpCallStack() );
		RuntimePush (vValue); //res=valor inicial
        //RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
    
	if (vDate.Type!=M4CL_CPP_TYPE_DATE){
        ret=m_oVariantMediator->ToDate (vDate);
    }
    if (ret!=M4_SUCCESS){ 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Date"<<"curGetExchange" << M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDate.Type)<<DumpCallStack() );
		RuntimePush (vValue); //res=valor inicial
        //RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

	if (vExType.Type!=M4CL_CPP_TYPE_STRING_POOL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "ExType"<<"curGetExchange" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vExType.Type)<<DumpCallStack() );
		RuntimePush (vValue); //res=valor inicial
        //RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
        
    if (vDstCur.Type!=M4CL_CPP_TYPE_STRING_POOL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Dest_currency"<<"curGetExchange" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vDstCur.Type)<<DumpCallStack() );
		RuntimePush (vValue); //res=valor inicial
        //RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

    if (vSrcCur.Type!=M4CL_CPP_TYPE_STRING_POOL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Source_Currency"<<"curGetExchange" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vSrcCur.Type)<<DumpCallStack() );
		RuntimePush (vValue); //res=valor inicial
        //RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

	pcExType = m_oDynamicPool->GetString(vExType.Data.CharData); 
   	pcSrcCur = m_oDynamicPool->GetString(vSrcCur.Data.CharData); 
	pcDstCur = m_oDynamicPool->GetString(vDstCur.Data.CharData); 

	vDecimals.Data.DoubleData = -1;
	ret = _curGetExchangeRate(pcSrcCur, pcDstCur, pcExType, vDate.Data.DoubleData, dRate, bExchangeFound, &vDecimals, m_oState1.m_poAccess);

	if (ret!=M4_ERROR)
	{
		//Cambiamos vValue
		vValue.Data.DoubleData *= dRate;  

		//Y redondeamos
		if ((M4_TRUE == bExchangeFound) && (vDecimals.Data.DoubleData != -1))
			vValue.Data.DoubleData = M4VMRound(vValue.Data.DoubleData, (int) vDecimals.Data.DoubleData); 
	}
	else
	{
		m4char_t psStringDate[80];
        ClDateToString(vDate.Data.DoubleData,psStringDate,sizeof(psStringDate));
	}

	//borramos las cadenas
	m_oDynamicPool->DeleteString(vExType.Data.CharData);
	m_oDynamicPool->DeleteString(vSrcCur.Data.CharData);
	m_oDynamicPool->DeleteString(vDstCur.Data.CharData);
	
    RuntimePush (vValue); 

    return ret;

}

m4return_t ClVM1PayrollLib::curGetDecimals       (void)
{	
	//Por ahora devuelve el vValue convertido
	m4VariantType vSrcCur, vDecimals;
	m4pchar_t pcSrcCur;

	m4return_t ret = M4_SUCCESS;

	vDecimals.Type = M4CL_CPP_TYPE_NULL;
    
	ret = RuntimePop (vSrcCur);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vSrcCur.curGetDecimals.")	;

       if (vSrcCur.Type!=M4CL_CPP_TYPE_STRING_POOL){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Source_currency"<<"curGetDecimals" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vSrcCur.Type)<<DumpCallStack() );
		RuntimePush (vDecimals); //res=NULO
        return M4_ERROR;
    }
    
   	pcSrcCur = m_oDynamicPool->GetString(vSrcCur.Data.CharData); 

	ret = _curGetDecimals (pcSrcCur, vDecimals, m_oState1.m_poAccess);

	if (ret==M4_ERROR)
	{
	}

	//borramos las cadenas
	m_oDynamicPool->DeleteString(vSrcCur.Data.CharData);
	
    RuntimePush (vDecimals); 

    return ret;
}


class ClCurListHandler{
private:
	typedef set<m4pchar_t, m4pchar_t_less> ClCurrencyList;
	typedef ClCurrencyList::iterator itClCurrencyList;

	m4pchar_t m_pcSrcDstPair;

	ClCurrencyList * m_pCurList;
	m4bool_t		m_bListowner;

public:
	ClCurListHandler(void)
	{
		m_pcSrcDstPair = 0;
		m_pCurList = 0;
		m_bListowner = M4_FALSE;
	}

	m4return_t CheckAndAdd(m4pchar_t ai_pcSrcCurType, m4pchar_t ai_pcDstCurType, ClCurrencyList * & aio_pCurList)
	{
		m4return_t ret = M4_SUCCESS;

		m_pcSrcDstPair = new m4char_t[strlen(ai_pcSrcCurType) + strlen(ai_pcDstCurType) + 2];
		sprintf(m_pcSrcDstPair, "%s-%s", ai_pcSrcCurType, ai_pcDstCurType);

		if (aio_pCurList)
		{
			m_bListowner = M4_FALSE;

			itClCurrencyList it;
			it = aio_pCurList->find ( m_pcSrcDstPair );
			if (it == aio_pCurList->end ())
			{
				aio_pCurList->insert ( m_pcSrcDstPair );
				m_pCurList = aio_pCurList;
			}
			else
			{
				DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_EXCHANGE_RECURSIVE_CYCLE, ai_pcSrcCurType<<ai_pcDstCurType);
				m_pCurList = 0;
				ret = M4_ERROR;
			}
		}
		else
		{
			m_bListowner = M4_TRUE;

			aio_pCurList = new ClCurrencyList();
			aio_pCurList->insert ( m_pcSrcDstPair );
			
			m_pCurList = aio_pCurList;
		}

		return ret;
	};

	~ClCurListHandler()
	{
		if (m_pCurList)
		{
			itClCurrencyList it;

			it = m_pCurList->find ( m_pcSrcDstPair );

			if ( it != m_pCurList->end () )
			{
				m_pCurList->erase (it);
			}

			if (M4_TRUE == m_bListowner)
				delete m_pCurList;
		}

		if (m_pcSrcDstPair)
			delete [] m_pcSrcDstPair;
	};
};
	
extern m4return_t __BSearch( ClAccessRecordSet & ai_RecSet, m4pchar_t ai_pcVal, m4int32_t ai_Index);
/* Si el ultimo parametro es NULL, se llama a GetAccess para intentar conseguir por nivel 2 el acceso
al canal Exchange. Si no, suponemos que el access es de dicho canal, y lo utilizamos directamente.
*/
m4return_t ClVM1PayrollLib::_curGetExchangeRate (m4pchar_t ai_pcSrcCurType, m4pchar_t ai_pcDstCurType, m4pchar_t ai_pcExType, m4date_t ai_dDate, m4double_t & ao_dRate, m4bool_t &ao_bExchangeFound, m4VariantType * ao_pvDecimals, ClAccess * & aio_poExchangeAccess, ClCurrencyList * aio_pCurList){

	ClItemIndex     iItemIndex;
	m4uint32_t      iNodeHandle, iItemHandle;
	m4VariantType	vRate, vSrcCur;

	ClNode * poNodeCurrency;
	
	ClCompiledMCR   *poCMCR;

	m4return_t res = M4_SUCCESS;

	ClCurListHandler oCurListHandler;

	ao_bExchangeFound = M4_TRUE;

	// Si la moneda es la misma, decolvemos exito.
	/*if ( strcmpi( ai_pcSrcCurType, ai_pcDstCurType ) == 0 )
	{ 
		ao_dRate = 1;
		return M4_SUCCESS;
	}*/

	// Comprobamos si el cambio que se desea ha entrado en recursividad infinita.
	// El parametro aio_pCurList sirve para mantener una lista con los cambios Src-Dst
	// a través de todas las llamadas recursivas cuando se usan monedas intermedias o ROOT.
	// La primera vez que se hace una llama recursiva, la lista se crea y despues se destruye.
	if ( M4_ERROR == oCurListHandler.CheckAndAdd(ai_pcSrcCurType, ai_pcDstCurType, aio_pCurList) )
	{
		return M4_ERROR;
	}

	if (!aio_poExchangeAccess)
	{
		//Obtenemos el canal de EXCHANGE_RATES mediante Nivel2
		res = m_poVM->GetL2Access( m_oState1.m_poAccess->GetpChannel(), M4_INSTANCE_EXCHANGE_RATES, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState1.m_Flags), &aio_poExchangeAccess );

	    if (res == M4_ERROR)
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_GET_ACCESS_L2, M4_INSTANCE_EXCHANGE_RATES<<"curGetExchangeRate" << DumpCallStack() );
			return M4_ERROR;
		}
	}

	ClRecordSet oRecordSetInterm (aio_poExchangeAccess);
    ClRecordSet  oRecordSetRates (aio_poExchangeAccess);
    //ClRecordSet  oRecordSetCurrency (aio_poExchangeAccess);

	poCMCR = aio_poExchangeAccess->GetpCMCR() ;

	//Ya tenemos el access al canal ECHANGE_RATES

  	//Obtenemos el recordset de CURRENCY.

	iNodeHandle = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHEXRATES_CURRENCY, M4_TRUE ) ;
	
	if( iNodeHandle == 0 )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_NODE, M4_NODE_SCHEXRATES_CURRENCY<<M4Obj(poCMCR)<<"GetExchangeRates" << DumpCallStack() );
		return M4_ERROR;
	}

	poNodeCurrency = &(aio_poExchangeAccess->Node[ (ClHandle)iNodeHandle ]) ; 
  
	res = __BSearch(poNodeCurrency->RecordSet, ai_pcSrcCurType, 1);

	if ( (res==M4_ERROR) || (poNodeCurrency->RecordSet.Register.Index == M4DM_EOF_INDEX) )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_SRC_CUR_NOT_FOUND, ai_pcSrcCurType << DumpCallStack() );
		ao_dRate = 1;
		ao_bExchangeFound = M4_FALSE;
		return M4_SUCCESS;
	}

	if ( strcmpi( ai_pcSrcCurType, ai_pcDstCurType ) == 0 )
	{ 
		if (ao_pvDecimals)
		{
			//Devolvemos los decimales del cambio
			poNodeCurrency->RecordSet.Register.Item[M4_ITEM_SCHEXRATES_CURRENCY_DECIMALS].Value.Get(*ao_pvDecimals);
		}

		ao_dRate = 1;
		return M4_SUCCESS;
	}

	/*!!ROOT_CURRENCY!!
	  Si SRC tiene ROOT cambio a través de root. Recursivo y con return;
		  Ex(ROOT_SRC, SRC, rate1)
		  rate1 = 1/rate1
		  Si ROOT_SRC == DST 
		     return rate1
		  Else 
		     Ex(ROOT_SRC, DST, rate2)
		     return rate1*rate2
	  Else Buscamos DST
	      Si tiene ROOT (y ROOT_DST != SRC) cambio a través de su ROOT con return
		     Ex(ROOT_DST, DST, rate1)
			 Ex(SRC, ROOT_DST, rate2)
			 return rate1*rate2
		  Else
		     Else nos posicionamos donde estábamos y seguimos

	  ¿¿facil no?? 
	  ***Recordar que:
	  - No pueden haber monedas ROOT con ROOT?? o si pueden?? NO, dan errores de recursividad.
					- por el mismo motivo, los ROOT no puede usar AUX_CURR a alguno de sus hijos.
	  - Siempre hay cambio de ROOT a todos sus hijos.
	  - Si una moneda tiene ROOT, no puede ser destino de otro cambio que su ROOT ni fuente de cambio alguno.

	*/

//--------------------------------------
//NUEVO
//--------------------------------------
	m4uint32_t iRegSrc = poNodeCurrency->RecordSet.Register.Index;
	m4VariantType vRootSrc, vRootDst;
	m4bool_t bRootFound;

	res = poNodeCurrency->RecordSet.Register.Item[M4_ITEM_SCHEXRATES_CURRENCY_ID_ROOT_CURRENCY].Value.Get(vRootSrc);

	bRootFound = M4_FALSE;
	if ( (M4_SUCCESS == res) && (!IsNull(vRootSrc)) && (vRootSrc.Type == M4CL_CPP_TYPE_STRING_VAR) )
	{
		//Validamos la fecha del ROOT
		m4VariantType vRootDate;
		res = poNodeCurrency->RecordSet.Register.Item[M4_ITEM_SCHEXRATES_CURRENCY_DT_START_ROOT_CURRENCY].Value.Get(vRootDate);

		if ( (M4_SUCCESS == res) && (!IsNull(vRootDate)) && (vRootDate.Type == M4CL_CPP_TYPE_DATE) )
		{
			if (vRootDate.Data.DoubleData <= ai_dDate)
			{
				//Solo si hay fecha de inico de ROOT y esta es menor que la de cambio hay ROOT
				bRootFound = M4_TRUE;
			}
		}
	}

	if ( M4_TRUE == bRootFound )
	{
		//SRC tiene ROOT cambio a través de root. Recursivo y con return;
		m4double_t dRate1, dRate2;
		
		if (strcmpi(vRootSrc.Data.PointerChar, ai_pcSrcCurType) == 0)
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_SELF_ROOT, ai_pcDstCurType << DumpCallStack() );					
			return M4_ERROR;
		}

		res = _curGetExchangeRate (vRootSrc.Data.PointerChar, ai_pcSrcCurType, ai_pcExType, ai_dDate, dRate1, ao_bExchangeFound, NULL, aio_poExchangeAccess, aio_pCurList);

		if (res == M4_ERROR) 
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_SCR_ROOT_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType<<vRootSrc.Data.PointerChar << DumpCallStack() );
			return M4_ERROR;
		}

		if (ao_bExchangeFound == M4_FALSE)
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_SCR_ROOT_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType<<vRootSrc.Data.PointerChar << DumpCallStack() );
			ao_dRate = 1;
			return M4_SUCCESS;
		}
		
		dRate1 = 1/dRate1;

		/*if (strcmpi(vRootSrc.Data.PointerChar, ai_pcDstCurType) == 0)
		{
			ao_dRate = dRate1;
			return M4_SUCCESS;
		}*/

		res = _curGetExchangeRate (vRootSrc.Data.PointerChar, ai_pcDstCurType, ai_pcExType, ai_dDate, dRate2, ao_bExchangeFound, ao_pvDecimals, aio_poExchangeAccess, aio_pCurList);

		if (res == M4_ERROR)
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_SCR_ROOT_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType<<vRootSrc.Data.PointerChar << DumpCallStack() );
			return M4_ERROR;
		}

		if (ao_bExchangeFound == M4_FALSE)
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_SCR_ROOT_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType<<vRootSrc.Data.PointerChar << DumpCallStack() );
			ao_dRate = 1;
			return M4_SUCCESS;
		}

		ao_dRate = dRate1 * dRate2;

		return M4_SUCCESS;
	}
	else
	{
		//Buscamos nuestro ai_pcDstCurType
		res = __BSearch(poNodeCurrency->RecordSet, ai_pcDstCurType, 1);

		if ( (res==M4_ERROR) || (poNodeCurrency->RecordSet.Register.Index == M4DM_EOF_INDEX) )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_SRC_CUR_NOT_FOUND, ai_pcDstCurType << DumpCallStack() );
			ao_dRate = 1;
			ao_bExchangeFound = M4_FALSE;
			return M4_SUCCESS;
		}

		if (ao_pvDecimals)
		{
			//Devolvemos los decimales del cambio
			poNodeCurrency->RecordSet.Register.Item[M4_ITEM_SCHEXRATES_CURRENCY_DECIMALS].Value.Get(*ao_pvDecimals);
		}

		res = poNodeCurrency->RecordSet.Register.Item[M4_ITEM_SCHEXRATES_CURRENCY_ID_ROOT_CURRENCY].Value.Get(vRootDst);

		bRootFound = M4_FALSE;
		if ( (M4_SUCCESS == res) && (!IsNull(vRootDst)) && (vRootDst.Type == M4CL_CPP_TYPE_STRING_VAR) )
		{
			//Validamos la fecha del ROOT
			m4VariantType vRootDate;
			res = poNodeCurrency->RecordSet.Register.Item[M4_ITEM_SCHEXRATES_CURRENCY_DT_START_ROOT_CURRENCY].Value.Get(vRootDate);

			if ( (M4_SUCCESS == res) && (!IsNull(vRootDate)) && (vRootDate.Type == M4CL_CPP_TYPE_DATE) )
			{
				if (vRootDate.Data.DoubleData <= ai_dDate)
				{
					//Solo si hay fecha de inico de ROOT y esta es menor que la de cambio hay ROOT
					bRootFound = M4_TRUE;
				}
			}
		}

		if ( M4_TRUE == bRootFound )
		{
			if ( strcmpi(vRootDst.Data.PointerChar, ai_pcSrcCurType) != 0)
			{
				//Tiene ROOT_DST (y ROOT_DST != SRC) cambio a través de su ROOT con return
				m4double_t dRate1, dRate2;

				if (strcmpi(vRootDst.Data.PointerChar, ai_pcDstCurType) == 0)
				{
					//No puedes ser ROOT de ti mismo
					DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_SELF_ROOT, ai_pcDstCurType << DumpCallStack() );					
					return M4_ERROR;
				}

				res = _curGetExchangeRate (vRootDst.Data.PointerChar, ai_pcDstCurType, ai_pcExType, ai_dDate, dRate1, ao_bExchangeFound, NULL, aio_poExchangeAccess, aio_pCurList);

				if (res == M4_ERROR) 
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_DST_ROOT_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType<<vRootDst.Data.PointerChar << DumpCallStack() );
					return M4_ERROR;
				}

				if (ao_bExchangeFound == M4_FALSE)
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_DST_ROOT_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType<<vRootDst.Data.PointerChar << DumpCallStack() );
					ao_dRate = 1;
					return M4_SUCCESS;
				}
				
				res = _curGetExchangeRate (ai_pcSrcCurType, vRootDst.Data.PointerChar, ai_pcExType, ai_dDate, dRate2, ao_bExchangeFound, NULL, aio_poExchangeAccess, aio_pCurList);

				if (res == M4_ERROR) 
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_DST_ROOT_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType<<vRootDst.Data.PointerChar << DumpCallStack() );
					return M4_ERROR;
				}

				if (ao_bExchangeFound == M4_FALSE)
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_DST_ROOT_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType<<vRootDst.Data.PointerChar << DumpCallStack() );
					ao_dRate = 1;
					return M4_SUCCESS;
				}

				ao_dRate = dRate1 * dRate2;

				return M4_SUCCESS;
			}
			else
			{
				//Continuamos como siempre
				poNodeCurrency->RecordSet.Register[iRegSrc];
			}
		}
		else
		{
			//Continuamos como siempre
			poNodeCurrency->RecordSet.Register[iRegSrc];
		}
	}

//--------------------------------------
//NUEVO
//--------------------------------------



	//Nos posicionamos en el bloque hijo (en RATES) del iRegister

	iNodeHandle = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHEXRATES_RATES, M4_TRUE ) ;


	if( iNodeHandle == NULL )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_NODE, M4_NODE_SCHEXRATES_RATES<<M4Obj(poCMCR)<<"GetExchangeRates" << DumpCallStack() );
		return( M4_ERROR ) ;
	}

    oRecordSetRates.SetCurrentBlock (poNodeCurrency->RecordSet.Register.GetChildBlock( ClHandle(iNodeHandle) ));

	m4VariantType	pstArgRates[3];

	pstArgRates[0].Type = M4CL_CPP_TYPE_STRING_VAR;
	pstArgRates[0].Data.PointerChar = ai_pcDstCurType;

	pstArgRates[1].Type = M4CL_CPP_TYPE_STRING_VAR;
	pstArgRates[1].Data.PointerChar = ai_pcExType;

	pstArgRates[2].Type = M4CL_CPP_TYPE_DATE;
	pstArgRates[2].Data.DoubleData = ai_dDate;

	
	res = oRecordSetRates.SetCurrentSearchIndexById(1); 

	if (res == M4_ERROR) 
	{
		return M4_ERROR;
	}

	oRecordSetRates.Register.Begin();
	
	oRecordSetRates.Register.Find (pstArgRates);

	/*if (res==M4_ERROR)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType << DumpCallStack() );
		ao_dRate = 1;
		return M4_SUCCESS;
	}*/

	if (oRecordSetRates.Register.Index != M4DM_EOF_INDEX)
	{
		//Comprobamos que la fecha de fin sea correcta
		res = oRecordSetRates.Register.Item[M4_ITEM_SCHEXRATES_RATES_DT_END].Value.Get(pstArgRates[2]);

		if (M4_ERROR == res)
		{
			return( M4_ERROR ) ;
		}

		if (pstArgRates[2].Data.DoubleData < ai_dDate)
		{
			//La fecha que buscamos no está en ningún rango de DT_START - DT_END. Cambio no encontrado.
			oRecordSetRates.Register.Index = -1;
		}
	}

	if (oRecordSetRates.Register.Index == M4DM_EOF_INDEX)
	{
		//No encontramos el cambio deseado. Buscamos un itermediario.
		m4VariantType	pstArgIntern[9];				

		iNodeHandle = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHEXRATES_AUX_CURR_EX, M4_TRUE ) ;
		
		
		if( iNodeHandle == NULL )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_NODE, M4_NODE_SCHEXRATES_AUX_CURR_EX<<M4Obj(poCMCR)<<"GetExchangeRates" << DumpCallStack() );
			return( M4_ERROR ) ;
		}
		

		oRecordSetInterm.SetCurrentBlock (poNodeCurrency->RecordSet.Register.GetChildBlock( ClHandle(iNodeHandle) ));

		//Obtenemos el Index del ID_REFER_CUR
		iItemHandle = poCMCR->GetNodeItemHandleById( iNodeHandle, M4_ITEM_SCHEXRATES_AUX_CURR_EX_ID_CURRENCY_DES, M4_TRUE ) ;

		if( iItemHandle == 0 )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_ITEM, M4_ITEM_SCHEXRATES_AUX_CURR_EX_ID_CURRENCY_DES <<M4_NODE_SCHEXRATES_AUX_CURR_EX<<poCMCR->GetChannelId()<< DumpCallStack() );
			return( M4_ERROR ) ;
		}

		iItemIndex = poCMCR->GetItemIndex( iItemHandle ) ;

		pstArgIntern[0].Type = M4CL_CPP_TYPE_NUMBER;
		pstArgIntern[0].Data.DoubleData = iItemIndex;

		pstArgIntern[1].Type = M4CL_CPP_TYPE_STRING_VAR;
		pstArgIntern[1].Data.PointerChar = ai_pcDstCurType;

		pstArgIntern[2].Type = M4CL_CPP_TYPE_NUMBER;
		pstArgIntern[2].Data.DoubleData = M4CL_EQUAL_FUNCTION;

		//Obtenemos el Index del DT_START
		iItemHandle = poCMCR->GetNodeItemHandleById( iNodeHandle, M4_ITEM_SCHEXRATES_AUX_CURR_EX_DT_START, M4_TRUE ) ;

		if( iItemHandle == 0 )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_ITEM, M4_ITEM_SCHEXRATES_AUX_CURR_EX_DT_START <<M4_NODE_SCHEXRATES_AUX_CURR_EX<<poCMCR->GetChannelId()<< DumpCallStack() );
			return( M4_ERROR ) ;
		}

		iItemIndex = poCMCR->GetItemIndex( iItemHandle ) ;

		pstArgIntern[3].Type = M4CL_CPP_TYPE_NUMBER;
		pstArgIntern[3].Data.DoubleData = iItemIndex;

		pstArgIntern[4].Type = M4CL_CPP_TYPE_DATE;
		pstArgIntern[4].Data.DoubleData = ai_dDate;

		pstArgIntern[5].Type = M4CL_CPP_TYPE_NUMBER;
		pstArgIntern[5].Data.DoubleData = M4CL_SMALLER_OR_EQUAL_FUNCTION;
		
		//Obtenemos el Index del DT_END
		iItemHandle = poCMCR->GetNodeItemHandleById( iNodeHandle, M4_ITEM_SCHEXRATES_AUX_CURR_EX_DT_END, M4_TRUE ) ;

		if( iItemHandle == 0 )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_ITEM, M4_ITEM_SCHEXRATES_AUX_CURR_EX_DT_END <<M4_NODE_SCHEXRATES_AUX_CURR_EX<<poCMCR->GetChannelId()<< DumpCallStack() );
			return( M4_ERROR ) ;
		}

		iItemIndex = poCMCR->GetItemIndex( iItemHandle ) ;

		pstArgIntern[6].Type = M4CL_CPP_TYPE_NUMBER;
		pstArgIntern[6].Data.DoubleData = iItemIndex;

		pstArgIntern[7].Type = M4CL_CPP_TYPE_DATE;
		pstArgIntern[7].Data.DoubleData = ai_dDate;

		pstArgIntern[8].Type = M4CL_CPP_TYPE_NUMBER;
		pstArgIntern[8].Data.DoubleData = M4CL_BIGGER_OR_EQUAL_FUNCTION;


		//Buscamos ai_pcDstCurType.
   		res=oRecordSetInterm.Register.FindSec (pstArgIntern, 9);

		if ( (res==M4_ERROR) || (oRecordSetInterm.Register.Index == M4DM_EOF_INDEX) )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType << DumpCallStack() );
			ao_dRate = 1;
			ao_bExchangeFound = M4_FALSE;
			return M4_SUCCESS;
		}


		m4VariantType vInterm;
		m4double_t dRate1, dRate2;

		//Ya tenemos el valor intermedio.
		res = oRecordSetInterm.Register.Item[M4_ITEM_SCHEXRATES_AUX_CURR_EX_ID_CURREN_INTERM].Value.Get(vInterm);

		if (res == M4_ERROR) 
		{
			//Error
			return M4_ERROR;
		}
		
		if (vInterm.Type != M4CL_CPP_TYPE_STRING_VAR)
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_ITEM_TYPE_MISMATCH, M4_ITEM_SCHEXRATES_AUX_CURR_EX_ID_CURREN_INTERM<<M4_NODE_SCHEXRATES_AUX_CURR_EX<<poCMCR->GetChannelId() <<"GetExchangeRate" << M4ClCppType(M4CL_CPP_TYPE_STRING_VAR)<<M4ClCppType(vInterm.Type)<< DumpCallStack() );
			return M4_ERROR;
		}

		//Accedemos a traves de la moneda intermedia. 
		res = _curGetExchangeRate (ai_pcSrcCurType, vInterm.Data.PointerChar, ai_pcExType, ai_dDate, dRate1, ao_bExchangeFound, NULL, aio_poExchangeAccess, aio_pCurList);

		if (res == M4_ERROR) 
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_INTERM_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType<<vInterm.Data.PointerChar << DumpCallStack() );
			return M4_ERROR;
		}

		if (ao_bExchangeFound == M4_FALSE)
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_INTERM_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType<<vInterm.Data.PointerChar << DumpCallStack() );
			ao_dRate = 1;
			return M4_SUCCESS;
		}

		res = _curGetExchangeRate (vInterm.Data.PointerChar, ai_pcDstCurType, ai_pcExType, ai_dDate, dRate2, ao_bExchangeFound, NULL, aio_poExchangeAccess, aio_pCurList);

		if (res == M4_ERROR) 
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_INTERM_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType<<vInterm.Data.PointerChar << DumpCallStack() );
			return M4_ERROR;
		}

		if (ao_bExchangeFound == M4_FALSE)
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_NO_INTERM_EXCHANGE_RATE, ai_pcSrcCurType<<ai_pcDstCurType<<vInterm.Data.PointerChar << DumpCallStack() );
			ao_dRate = 1;
			return M4_SUCCESS;
		}

		//Uua!! Lo hemos conseguido.
		ao_dRate = dRate1 * dRate2;

		return M4_SUCCESS;
		
	}	
	
	//Ya tenemos el registro en rates que queremos.
	res = oRecordSetRates.Register.Item[M4_ITEM_SCHEXRATES_RATES_RATE].Value.Get(vRate);

	if (res==M4_ERROR)
	{
		return M4_ERROR;
	}


	ao_dRate = vRate.Data.DoubleData;

	return M4_SUCCESS;
}


/* Si el ultimo parametro es NULL, se llama a GetAccess para intentar conseguir por nivel 2 el acceso
al canal Exchange. Si no, suponemos que el access es de dicho canal, y lo utilizamos directamente.
*/
m4return_t ClVM1PayrollLib::_curGetDecimals (m4pchar_t ai_pcSrcCurType, m4VariantType & ao_vDecimals, ClAccess * & aio_poExchangeAccess)
{
	//ClAccess * poExchangeAccess;
	//ClItemIndex     iItemIndex;
	m4uint32_t      iNodeHandle;
	m4VariantType	vSrcCur;

	ClNode * poNodeCurrency;

	ClCompiledMCR   *poCMCR;

	m4return_t res = M4_SUCCESS;

	if (!aio_poExchangeAccess)
	{
		//Obtenemos el canal de EXCHANGE_RATES mediante Nivel2
		res = m_poVM->GetL2Access( m_oState1.m_poAccess->GetpChannel(), M4_INSTANCE_EXCHANGE_RATES, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState1.m_Flags), &aio_poExchangeAccess );

	    if (res == M4_ERROR)
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_GET_ACCESS_L2, M4_INSTANCE_EXCHANGE_RATES<<"curGetDecimals" << DumpCallStack() );
			return M4_ERROR;
		}
	}

	poCMCR = aio_poExchangeAccess->GetpCMCR() ;

	//Ya tenemos el access al canal ECHANGE_RATES

  	//Obtenemos el recordset de CURRENCY.

	iNodeHandle = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHEXRATES_CURRENCY, M4_TRUE ) ;

	if( iNodeHandle == 0 )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_NODE, M4_NODE_SCHEXRATES_CURRENCY <<M4Obj(poCMCR)<< DumpCallStack() );
		return M4_ERROR;
	}

	poNodeCurrency = &(aio_poExchangeAccess->Node[ (ClHandle)iNodeHandle ]) ; 

	res = __BSearch(poNodeCurrency->RecordSet, ai_pcSrcCurType, 1);

	if (res==M4_ERROR)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_SRC_CUR_NOT_FOUND, ai_pcSrcCurType << DumpCallStack() );
		return M4_ERROR;
	}

	if (poNodeCurrency->RecordSet.Register.Index == M4DM_EOF_INDEX)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_CUR_SRC_CUR_NOT_FOUND, ai_pcSrcCurType << DumpCallStack() );
		ao_vDecimals.Type = M4CL_CPP_TYPE_NUMBER;
		ao_vDecimals.Data.DoubleData = 0;
		return M4_SUCCESS;
	}

	poNodeCurrency->RecordSet.Register.Item[M4_ITEM_SCHEXRATES_CURRENCY_DECIMALS].Value.Get(ao_vDecimals);

	return M4_SUCCESS;
}


typedef set<ClHandle, less<ClHandle> > ClHandleList;
typedef ClHandleList::iterator itClHandleList;

m4return_t ClVM1PayrollLib::_curExchangeRecord (ClRegister * ai_poReg, m4pchar_t ai_pcDstCur, m4pchar_t ai_pcExType, m4date_t ai_dDate, m4int8_t iFlag, m4bool_t ai_bRound)
{
	ClAccess *poExchangeAccess = NULL;
	m4pchar_t pcSrcCur;
	m4uint16_t	iNumItems, i;
	ClHandleList oAuxItemSrcHandleList;
	itClHandleList it;
	ClHandle hItem = 0, 
			 hAuxItemSrcCur = 0, 
			 hAuxItemDate = 0,
			 hAuxItemExType = 0;
	m4uint8_t iM4Type;
	m4VariantType	vValue, vDate, vDestCur, vSrcCur, vExType;
	m4double_t dRate;
	m4bool_t bExchangeFound;
	//ClRegisterIndex oRegIndex;

	m4return_t ret;

	ClCompiledMCR   *poCMCR = m_oState1.m_poAccess-> GetpCMCR() ;

	//Obtener hNode
	m4int32_t hNode = ai_poReg->GetpNodeDef ()->GetHandle();
	
	//Obtener NumItems	
	iNumItems = poCMCR->GetNodeNumberOfItems (hNode);	


	//Si Flag Crear registro Nuevo
	if (iFlag)
	{
		ClRegister oRegRead = *ai_poReg;

		ai_poReg->Add();

		ai_poReg->CopyDataFrom(oRegRead);
	}
	
	// checks for EOF condition
	if (ai_poReg->Index == M4DM_EOF_INDEX)
	{
		DUMP_CHLOG_WARNINGF(M4_VM_EXCHANGE_INVALID_REGISTER, "ExchangeRecord" << DumpCallStack() );
		return M4_SUCCESS;
	}
	
	for (i=0; i<iNumItems; i++)
	{
		hItem = poCMCR->GetNodeItemHandleByPosition (hNode, i);

		/* Bug 0146049
		Si el valor es nulo no hay que intentar cambiarlo
		*/
		ai_poReg->Item[hItem].Value.Get(vValue);

		if( IsNull( vValue ) )
		{
			continue ;
		}
		
		iM4Type = poCMCR->GetItemM4Type (hItem);
		
		if (iM4Type == M4CL_M4_TYPE_CURRENCY)
		{
			// Aux Item para el ScrCur del item
			hAuxItemSrcCur = poCMCR->GetItemAuxItem (hItem);
		}
		else if ( (iM4Type == M4CL_M4_TYPE_VARIANT) || (iM4Type == M4CL_M4_TYPE_NUM_VARIANT) )
		{
			ClHandle hAuxItemVarType = 0;
			m4VariantType vTypeVar;
			m4uint8_t iRealM4Type;

			hAuxItemVarType = poCMCR->GetItemAuxItem (hItem);

			if (hAuxItemVarType == 0)
			{
				DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_AUX_ITEM, M4ObjNodeItemI(poCMCR,hItem)<<"Variant Type" << DumpCallStack() );
				return M4_ERROR;
			}

			ret = ai_poReg->Item[hAuxItemVarType].Value.Get(vTypeVar);

			if( ret != M4_SUCCESS )
			{
				return M4_ERROR;
			}

			if( IsNull( vTypeVar ) )
			{
				DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_AUX_ITEM,M4ObjNodeItemI(poCMCR,hAuxItemVarType)<<poCMCR->GetItemId(hItem)<<"Variant Type" << DumpCallStack() );
				return M4_ERROR;
			}

			if( vTypeVar.Type != M4CL_CPP_TYPE_NUMBER )
			{
				DUMP_CHLOG_ERRORF(M4_CH_VM_AUX_ITEM_TYPE_MISMATCH, M4ObjNodeItemI(poCMCR,hAuxItemVarType)<<poCMCR->GetItemId(hItem)<<"Variant Type"<<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTypeVar.Type)<<DumpCallStack() );
				return M4_ERROR;
			}

			iRealM4Type = m4uint8_t( vTypeVar.Data.DoubleData ) ;

			if( iRealM4Type == M4CL_M4_TYPE_CURRENCY )
			{
				hAuxItemSrcCur = poCMCR->GetItemAuxItem (hAuxItemVarType);
			}
			else
			{
				// No muy ortodoxpo pero cómodo.
				continue;
			}

		}
		else
		{
			// No muy ortodoxpo pero cómodo.
			continue;
		}
			
		if (hAuxItemSrcCur == 0)
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_AUX_ITEM, M4ObjNodeItemI(poCMCR,hItem)<<"Source Id Currency" << DumpCallStack() );
			return M4_ERROR;
		}

		// Continuamos con nuestro tipo moneda
		ret = ai_poReg->Item[hAuxItemSrcCur].Value.Get(vSrcCur);

		if( ret != M4_SUCCESS )
		{
			return M4_ERROR;
		}

		if( IsNull( vSrcCur ) )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_AUX_ITEM, M4ObjNodeItemI(poCMCR,hAuxItemSrcCur)<<poCMCR->GetItemId(hItem)<<"Source Id Currency" << DumpCallStack() );
			return M4_ERROR;
		}

		if( vSrcCur.Type != M4CL_CPP_TYPE_STRING_VAR )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_AUX_ITEM_TYPE_MISMATCH, M4ObjNodeItemI(poCMCR,hAuxItemSrcCur)<<poCMCR->GetItemId(hItem)<<"Source Id Currency" <<M4ClCppType(M4CL_CPP_TYPE_STRING_VAR)<<M4ClCppType(vSrcCur.Type)<<DumpCallStack() );
			return M4_ERROR;
		}

		pcSrcCur = vSrcCur.Data.PointerChar;

		//Obtenemos la fecha del aux item del aux item.
		hAuxItemDate = poCMCR->GetItemAuxItem (hAuxItemSrcCur);

		if (hAuxItemDate == 0)
		{
			if (ai_dDate == 0)
			{
				//No se paso la fecha como parametro. Error
				DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_AUX_ITEM, M4ObjNodeItemI(poCMCR,hAuxItemSrcCur)<<"Exchange Date" << DumpCallStack() );
				return M4_ERROR;
			}

			if (ai_pcExType == 0)
			{
				//No se paso el ExType como parametro. Error
				DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_AUX_ITEM, M4ObjNodeItemI(poCMCR,hAuxItemSrcCur)<<"Exchange Type" << DumpCallStack() );
				return M4_ERROR;
			}
		}
		else
		{
			/*
			Bug 0095411
			Mejora para hacer caso a los argumentos
			*/
			if( ai_dDate == 0 )
			{
				ret = ai_poReg->Item[hAuxItemDate].Value.Get(vDate);

				if( ret != M4_SUCCESS )
				{
					return M4_ERROR;
				}

				if( IsNull( vDate ) )
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_AUX_ITEM, M4ObjNodeItemI(poCMCR,hAuxItemDate)<<poCMCR->GetItemId(hAuxItemSrcCur)<<"Exchange Date" << DumpCallStack() );
					return M4_ERROR;
				}

				if( vDate.Type != M4CL_CPP_TYPE_DATE )
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_AUX_ITEM_TYPE_MISMATCH, M4ObjNodeItemI(poCMCR,hAuxItemDate)<<poCMCR->GetItemId(hAuxItemSrcCur)<<"Exchange Date"<<M4ClCppType(M4CL_CPP_TYPE_DATE)<<M4ClCppType(vDate.Type)<<DumpCallStack() );
					return M4_ERROR;
				}

				ai_dDate = vDate.Data.DoubleData;
			}

			/*
			Bug 0095411
			El tipo de cambio sólo se inspecciona si existe el item de la moneda
			*/
			//Obtenemos la fecha del aux item del aux item.
			hAuxItemExType = poCMCR->GetItemAuxItem (hAuxItemDate);

			if (hAuxItemExType == 0)
			{
				if (ai_pcExType == 0)
				{
					//No se paso el ExType. Error
					DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_AUX_ITEM, M4ObjNodeItemI(poCMCR,hAuxItemDate)<<"Exchange Type" << DumpCallStack() );
					return M4_ERROR;
				}
			}
			else
			{
				/*
				Bug 0095411
				Mejora para hacer caso a los argumentos
				*/
				if( ai_pcExType == NULL )
				{
					ret = ai_poReg->Item[hAuxItemExType].Value.Get(vExType);

					if( ret != M4_SUCCESS )
					{
						return M4_ERROR;
					}

					if( IsNull( vExType ) )
					{
						DUMP_CHLOG_ERRORF(M4_CH_VM_NULL_AUX_ITEM, M4ObjNodeItemI(poCMCR,hAuxItemExType)<<poCMCR->GetItemId(hAuxItemDate)<<"Exchange Type" << DumpCallStack() );
						return M4_ERROR;
					}

					if( vExType.Type != M4CL_CPP_TYPE_STRING_VAR )
					{
						DUMP_CHLOG_ERRORF(M4_CH_VM_AUX_ITEM_TYPE_MISMATCH, M4ObjNodeItemI(poCMCR,hAuxItemExType)<<poCMCR->GetItemId(hAuxItemDate)<<"Exchange Type"<<M4ClCppType(M4CL_CPP_TYPE_STRING_VAR)<<M4ClCppType(vExType.Type)<<DumpCallStack() );
						return M4_ERROR;
					}

					ai_pcExType = vExType.Data.PointerChar;
				}
			}
		}

		m4VariantType vDecimals;
		m4VariantType * pvDecimals = NULL;

		if (M4_TRUE == ai_bRound)
		{
			vDecimals.Data.DoubleData = -1;
			pvDecimals = &vDecimals;
		}

		ret = _curGetExchangeRate(pcSrcCur, ai_pcDstCur, ai_pcExType, ai_dDate, dRate, bExchangeFound, pvDecimals, poExchangeAccess);

		if (ret == M4_ERROR)
		{
			return M4_ERROR;
		}

		/* Bug 0099229
		Esto habrá que volver a descomentarlo, que parece que se quedó olvidado

		Para manteber compatibilidad haci atrás, de momento, aunque el cambio no haya sido encontrado, 
		el tipo de moneda del item Si se cambia.
		Este codigo se deberia descomentar el la 3.2 (3.110)
		*/
		if (bExchangeFound == M4_FALSE)
		{
			continue;
		}

		//Añadimos el handle del item tipo moneda a la lista para cambiar su valor luego.

		it = oAuxItemSrcHandleList.find ( hAuxItemSrcCur );
		if (it == oAuxItemSrcHandleList.end ())
		{
			oAuxItemSrcHandleList.insert ( hAuxItemSrcCur );
		}

		if (vValue.Type == M4CL_CPP_TYPE_STRING_VAR)
		{
			// Lo convertimos (es un variant)
			vValue.Type = M4CL_CPP_TYPE_NUMBER ;
			vValue.Data.DoubleData = atof( vValue.Data.PointerChar ) ;
		}

		//Cambiamos vValue
		vValue.Data.DoubleData *= dRate;

		if (M4_TRUE == ai_bRound)
		{
			//Redondeamos
			if ((M4_TRUE == bExchangeFound) && (vDecimals.Data.DoubleData != -1))
				vValue.Data.DoubleData = M4VMRound(vValue.Data.DoubleData, (int)vDecimals.Data.DoubleData);
		}
		
		//Asignamos el nuevo valor.
		ai_poReg->Item[hItem].Value.Set(vValue);
			
	}


	//Cambiamos el tipo de moneda de los items.
	vDestCur.Type = M4CL_CPP_TYPE_STRING_VAR;
	vDestCur.Data.PointerChar = ai_pcDstCur;

	for (it = oAuxItemSrcHandleList.begin();it != oAuxItemSrcHandleList.end();++it)
	{
		hAuxItemSrcCur = (*it);
		ai_poReg->Item[hAuxItemSrcCur].Value.Set(vDestCur);
	}

	return M4_SUCCESS;

}


//--------------------------------------------------------------
//LLAMAR AL ITEM DE EXCHANGE, con los 5 args. que vienen
//--------------------------------------------------------------
m4return_t ClVM1PayrollLib::curGetCurrencyExchange (void){

	m4return_t ret;
	ClAccess *poAccess;
	

	ret = m_poVM->GetL2Access( m_oState1.m_poAccess->GetpChannel(), M4_INSTANCE_EXCHANGE_RATES, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState1.m_Flags), &poAccess ) ;
	
	if( ret != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_GET_ACCESS_L2, M4_INSTANCE_EXCHANGE_RATES<<"curGetCurrencyExchange" << DumpCallStack() );
		return( M4_ERROR ) ;
	}



	
	//--llamamos al item <curGetExchange>--

	ClNode *poNode;
	ClItem *poItem;

	poNode=poAccess->Node.Get(M4_NODE_SCHEXRATES_CURRENCY);

	if (poAccess->Node.Index == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_NODE, M4_NODE_SCHEXRATES_CURRENCY<<M4Obj(m_oState1.m_poAccess->GetpCMCR())<<"GetCurrencyExchange" << DumpCallStack() );
		return M4_ERROR;
	}   
	poItem=&(poNode->RecordSet.Current.Item[M4_NODE_SCHEXRATES_ITEM_GET_EXCHANGE]);
	
	if (poItem->GetIndex() == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_ITEM, M4_NODE_SCHEXRATES_ITEM_GET_EXCHANGE << M4_NODE_SCHEXRATES_CURRENCY<<m_oState1.m_poAccess->GetpCMCR()->GetChannelId()<<DumpCallStack() );
		return M4_ERROR;
	}
	//comprobamos que tenga un solo argumento
	if (poItem->ItemDef.NumberOfArguments() != 5 || poItem->ItemDef.VariableArguments()==1){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "curGetCurrencyExchange"<<"5" << DumpCallStack() );
		return M4_ERROR;
	}

	

	//ret=poItem->CallForDM (); //ya se cambiara por el güeno
	ret=poItem->Call (m_oState1.m_poAccess->GetpExecutor() ); //ya es el güeno
					//le pasamos el ejecutor para forzar a que se ejcute sobre el mismo 
					//ejecutor en el que estamso (en cuya pila están los argumentos)
	//No metemos argumentos, porque son los que nos pasan
	
	//retornamos sin meter resultado
	return ret;
}

m4return_t ClVM1PayrollLib::curGetCurrencyExchangeRounded (void){

	m4return_t ret;
	ClAccess *poAccess;
	

	ret = m_poVM->GetL2Access( m_oState1.m_poAccess->GetpChannel(), M4_INSTANCE_EXCHANGE_RATES, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState1.m_Flags), &poAccess ) ;
	
	if( ret != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_GET_ACCESS_L2, M4_INSTANCE_EXCHANGE_RATES<<"curGetCurrencyExchange" << DumpCallStack() );
		return( M4_ERROR ) ;
	}



	
	//--llamamos al item <curGetExchange>--

	ClNode *poNode;
	ClItem *poItem;

	poNode=poAccess->Node.Get(M4_NODE_SCHEXRATES_CURRENCY);

	if (poAccess->Node.Index == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_NODE, M4_NODE_SCHEXRATES_CURRENCY<<M4Obj(m_oState1.m_poAccess->GetpCMCR())<<"GetCurrencyExchange" << DumpCallStack() );
		return M4_ERROR;
	}   
	poItem=&(poNode->RecordSet.Current.Item[M4_NODE_SCHEXRATES_ITEM_GET_EXCHANGE_ROUNDED]);
	
	if (poItem->GetIndex() == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_ITEM, M4_NODE_SCHEXRATES_ITEM_GET_EXCHANGE << M4_NODE_SCHEXRATES_CURRENCY<<m_oState1.m_poAccess->GetpCMCR()->GetChannelId()<<DumpCallStack() );
		return M4_ERROR;
	}
	//comprobamos que tenga un solo argumento
	if (poItem->ItemDef.NumberOfArguments() != 5 || poItem->ItemDef.VariableArguments()==1){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "curGetCurrencyExchange"<<"5" << DumpCallStack() );
		return M4_ERROR;
	}

	

	//ret=poItem->CallForDM (); //ya se cambiara por el güeno
	ret=poItem->Call (m_oState1.m_poAccess->GetpExecutor() ); //ya es el güeno
					//le pasamos el ejecutor para forzar a que se ejcute sobre el mismo 
					//ejecutor en el que estamso (en cuya pila están los argumentos)
	//No metemos argumentos, porque son los que nos pasan
	
	//retornamos sin meter resultado
	return ret;
}

