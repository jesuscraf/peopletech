

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
//    Virtual machine de nivel 1- Funciones relativas a filtros y ordenaciones/busquedas
////
//==============================================================================
#include "vm1filtsort.hpp"

#include "vmres.hpp"

#include "m4log.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "regi_smp.hpp"
#include "clbsearch.hpp"
#include "dm_def.hpp"
#include "m4objglb.hpp"
#include "nodedef.hpp"

//--------------------------------------------------
//Filtros
//--------------------------------------------------


//
//AddFilter
//
m4return_t ClVM1FilterSort::AddFilter		(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: AddFilter ");
#endif
	m4VariantType vNumArg,vForm,vType;
	m4int32_t i;
	m4int16_t j,iNumFilter=0;
	m4return_t ret;
	ClFilterInterface::FilterType IsStatic;
	m4VariantType avValues[ M4CL_VM_MAX_FILTER_ARGS ];
	m4VariantType avTempValues[ M4CL_VM_MAX_FILTER_ARGS ];
	m4VariantType avNames[ M4CL_VM_MAX_FILTER_ARGS ];
	m4char_t *appNames[ M4CL_VM_MAX_FILTER_ARGS ];
	m4int32_t iNumPar=0;

	//leemos el número de argumentos
	ret=RuntimePop(vNumArg);
    DEBUG_CHECK_RES (ret==M4_SUCCESS && vNumArg.Type==M4CL_CPP_TYPE_NUMBER, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "NumberOfParameters" << "AddFilter." << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumArg.Type)<<DumpCallStack() );
	m4int32_t iNumArg = m4int32_t(vNumArg.Data.DoubleData);

	if (iNumArg  < 1) { //error->Faltan argumentos
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "AddFilter"<<"1-" << DumpCallStack() );

		return M4_ERROR;
	}
	
	
	if (iNumArg>=2){ //sacamos el tipo de filtro y miramos los pares que vienen
		
		//los argumentos del filtro
		if ( (iNumArg -2)%2 ) { //ignoramos un argumento. No es par
			RuntimePopAndDelete();
		}
		
		iNumPar=(iNumArg -2)/2;
		
		if (iNumPar > M4CL_VM_MAX_FILTER_ARGS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_TOO_MUCH_PARAM_NUM, "AddFilter" << DumpCallStack() );
			return M4_ERROR;
		}
		
		for (i=0; i< iNumPar ;++i){ 
			//sacamos valor
			ret=RuntimePop(avValues[iNumPar-i-1]);
			DEBUG_CHECK_RES (ret==M4_SUCCESS , M4_CH_VM_RTS_POP , ERRORLOG, "Error getting parameters value in AddFilter.")	;
			
			//sacamos nombre
			ret=RuntimePopString(avNames[iNumPar-i-1]);
			M4CHECK_RES (ret==M4_SUCCESS , M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "ParameterName" << "AddFilter" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(avNames[iNumPar-i-1].Type)<<DumpCallStack() );
		}
		
		ret=RuntimePopNumber(vType);
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "FilterType"<<"AddFilter" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vType.Type)<<DumpCallStack() );
			return M4_ERROR;
		}
        IsStatic= (vType.Data.DoubleData==0.0 ? ClFilterInterface::DynamicFilter : ClFilterInterface::StaticFilter);
	}
	else{ //por defecto es estático
		IsStatic=ClFilterInterface::StaticFilter;
	}

	//sacamos el que tiene el código
	ret = RuntimePop (vForm);
	DEBUG_CHECK_RES (ret==M4_SUCCESS , M4_CH_VM_RTS_POP, ERRORLOG, "Error getting parameter (code) in AddFilter.")	;
	
	if (vForm.Type == M4CL_CPP_TYPE_STRING_POOL){ //con codigo ln4

		/* Bug 0117615
		Hay que recalcular los argumentos porque puede que el pool se redimensione
		*/
		for( i = 0 ; i < iNumPar ; i++ )
		{ 
			if( avValues[ i ].Type == M4CL_CPP_TYPE_STRING_POOL )
			{
				avTempValues[ i ].Type = M4CL_CPP_TYPE_STRING_VAR ;
				avTempValues[ i ].Data.PointerChar = m_oDynamicPool->GetString( avValues[ i ].Data.CharData ) ;
			}
			else
			{
				avTempValues[ i ] = avValues[ i ] ;
			}
			
			appNames[ i ] = m_oDynamicPool->GetString( avNames[ i ].Data.CharData ) ;
		}

		//ponemos el filtro
		m4char_t *pszCode;
		pszCode=m_oDynamicPool->GetString(vForm.Data.CharData);
		ret = m_oState1.m_poRecordSet->Current.Filter.AddNew(pszCode,IsStatic,iNumPar,appNames,avTempValues);

		if( ret == M4_SUCCESS )
		{
			//trabajamos tanto en current como en register

			/* Bug 0117615
			Hay que recalcular los argumentos porque puede que el pool se redimensione
			*/
			for( i = 0 ; i < iNumPar ; i++ )
			{ 
				if( avValues[ i ].Type == M4CL_CPP_TYPE_STRING_POOL )
				{
					avTempValues[ i ].Type = M4CL_CPP_TYPE_STRING_VAR ;
					avTempValues[ i ].Data.PointerChar = m_oDynamicPool->GetString( avValues[ i ].Data.CharData ) ;
				}
				else
				{
					avTempValues[ i ] = avValues[ i ] ;
				}
				
				appNames[ i ] = m_oDynamicPool->GetString( avNames[ i ].Data.CharData ) ;
			}

			/* Bug 0077036
			Hay que volver a pedir el puntero porque se puede haber redimensionado el pool
			*/
			pszCode=m_oDynamicPool->GetString(vForm.Data.CharData);
			ret = m_oState1.m_poRecordSet->Register.Filter.AddNew(pszCode,IsStatic,iNumPar,appNames,avTempValues);
		}
	}
	else if (vForm.Type == M4CL_CPP_TYPE_NUMBER){ //por operacion
	
		m4uint8_t op;
		switch ((m4int32_t)vForm.Data.DoubleData){
		
		case M4CL_VM_OP_DELETED:
			op=M4DM_OP_TYPE_DELETE;
			break;

		case M4CL_VM_OP_MODIFIED:
			op=M4DM_OP_TYPE_MODIFY;
			break;

		case M4CL_VM_OP_NEW:
			op=M4DM_OP_TYPE_NEW;
			break;

		case M4CL_VM_OP_UPDATED:
			op=M4DM_OP_TYPE_UPDATE;
			break;

		case -M4CL_VM_OP_DELETED:
			op=M4DM_OP_TYPE_NOTDELETE;
			break;

		case -M4CL_VM_OP_MODIFIED:
			op=M4DM_OP_TYPE_NOTMODIFY;
			break;

		case -M4CL_VM_OP_NEW:
			op=M4DM_OP_TYPE_NOTNEW;
			break;

		case -M4CL_VM_OP_UPDATED:
			op=M4DM_OP_TYPE_NOTUPDATE;
			break;

		default:
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "Code/Operation" << "AddFilter" << (m4int32_t)vForm.Data.DoubleData << DumpCallStack() );
			return M4_ERROR;
		}

		ret = m_oState1.m_poRecordSet->Current.Filter.AddNew(op,IsStatic);

		if( ret == M4_SUCCESS )
		{
			//trabajamos tanto en current como en register
			ret = m_oState1.m_poRecordSet->Register.Filter.AddNew(op,IsStatic);
		}
	}
	else { //no es valido
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Code/Operation" << "AddFilter" << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_NUMBER) << ", " << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)) << M4ClCppType(vForm.Type)<<DumpCallStack() );
		return M4_ERROR;
	}

	//el retorno
	if (ret != M4_SUCCESS){
		RuntimePushDouble (ret);
	}
	else{
		iNumFilter=m_oState1.m_poRecordSet->Current.Filter.Count()-1;
		RuntimePushDouble (iNumFilter);
	}
	
	// Liberamos los argumentos
	for (j=0; j<iNumPar; ++j){
		DeleteVarString (avNames[j]);
		DeleteVarString (avValues[j]);
	}

	DeleteVarString (vForm);

	//salimos
	return ret;
}


//
//RemoveFilter
//
m4return_t ClVM1FilterSort::RemoveFilter	(void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: RemoveFilter ");
#endif
	
	//quitamos el último filtro
	RuntimePushDouble (m_oState1.m_poRecordSet->Current.Filter.DeleteLast() );
	//trabajamos tanto en current como en register
	m_oState1.m_poRecordSet->Register.Filter.DeleteLast() ;
	return M4_SUCCESS;
}

//
//CleanFilter
//
m4return_t ClVM1FilterSort::CleanFilter		(void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: CleanFilter ");
#endif
	
	//quitamos TODOS los filtro
	RuntimePushDouble (m_oState1.m_poRecordSet->Current.Filter.DeleteAll() );
	//trabajamos tanto en current como en register
	m_oState1.m_poRecordSet->Register.Filter.DeleteAll() ;
	return M4_SUCCESS;
	
}

//
//RefreshFilter
//
m4return_t ClVM1FilterSort::RefreshFilter		(void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: RefreshFilter ");
#endif
	
	//refrescamos
	RuntimePushDouble (m_oState1.m_poRecordSet->Current.Filter.UpdateFilter() );
	//trabajamos tanto en current como en register
	m_oState1.m_poRecordSet->Register.Filter.UpdateFilter() ;
	return M4_SUCCESS;
	
}


//
//GetFilterArgument
//
m4return_t ClVM1FilterSort::GetFilterArgument		(void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetFilterArgument ");
#endif

	
	m4VariantType vFilter, vValue, vName;
	m4int16_t iNumFilter=0;
	m4return_t ret;

	
	//sacamos nombre
	ret=RuntimePop(vName);
	DEBUG_CHECK_RES (ret==M4_SUCCESS , M4_CH_VM_RTS_POP , ERRORLOG, "Error getting parameters name in GetFilterArgument.")	;
	
	
	//sacamos el id del filtro
	ret=RuntimePopNumber(vFilter);//->De momento solo por numero
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "FilterId"<<"GetFilterArgument" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vFilter.Type)<<DumpCallStack() );
		return M4_ERROR;
	}
    
	iNumFilter =(m4int16_t) vFilter.Data.DoubleData; 
	
	switch (vName.Type)
	{
		case M4CL_CPP_TYPE_NUMBER://por pos
			m_oState1.m_poRecordSet->Current.Filter[iNumFilter]->Arguments()[(m4int16_t)vName.Data.DoubleData].Get(vValue);
			break;

		case M4CL_CPP_TYPE_STRING_POOL: //por nombre
			m_oState1.m_poRecordSet->Current.Filter[iNumFilter]->Arguments()[m_oDynamicPool->GetString(vName.Data.CharData)].Get(vValue);
			break;
	}
	
	//vamos liberando
	DeleteVarString(vName);
	
	if (vValue.Type==M4CL_CPP_TYPE_STRING_VAR){
		ret=RuntimePushString(vValue.Data.PointerChar, vValue.IsUser);
	}
	else{
		ret=RuntimePush(vValue);
	}
	
	//salimos
	return M4_SUCCESS;
}


//
//SetFilterArgument
//
m4return_t ClVM1FilterSort::SetFilterArgument		(void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SetFilterArgument ");
#endif

	m4VariantType vNumArg,vFilter, vValue, vName;
	m4int32_t j;
	m4int16_t iNumFilter=0;
	m4int32_t i;
	m4return_t ret, retVal=M4_SUCCESS;
	m4VariantType avValues[ M4CL_VM_MAX_FILTER_ARGS ];
	m4VariantType avNames [ M4CL_VM_MAX_FILTER_ARGS ];

	//leemos el número de argumentos
	ret=RuntimePop(vNumArg);
    DEBUG_CHECK_RES (ret==M4_SUCCESS && vNumArg.Type==M4CL_CPP_TYPE_NUMBER, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "NumberOfParameters" << "SetFilterArgument" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumArg.Type)<<DumpCallStack() );
	
	m4int32_t iNumArg = m4int32_t(vNumArg.Data.DoubleData);

	if (iNumArg  < 3) { //error->Faltan argumentos
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "SetFilterArgument"<<"3-" << DumpCallStack() );
		return M4_ERROR;
	}


	//los argumentos del filtro
	if ( (iNumArg -1)%1 ) { //ignoramos un argumento. No es par
		RuntimePopAndDelete();
	}
	
	m4int32_t iNumPar=(iNumArg -1)/2;

	if (iNumPar > M4CL_VM_MAX_FILTER_ARGS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_TOO_MUCH_PARAM_NUM, "SetFilterArgument" << DumpCallStack() );

		return M4_ERROR;
	}

	for (i=0; i< iNumPar ;++i){ 
		//sacamos valor
		ret=RuntimePop(vValue);
		DEBUG_CHECK_RES (ret==M4_SUCCESS , M4_CH_VM_RTS_POP , ERRORLOG, "Error getting parameters value in SetFilterArgument.")	;
		
		//sacamos nombre
		ret=RuntimePop(vName);
		DEBUG_CHECK_RES (ret==M4_SUCCESS , M4_CH_VM_RTS_POP , ERRORLOG, "Error getting parameters name in SetFilterArgument.")	;

		//los metemos al final del array
		avNames [i]=vName;
		avValues[i]=vValue;
	}


	//sacamos el id del filtro
	ret=RuntimePopNumber(vFilter);//->De momento solo por numero
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "FilterId"<<"SetFilterArgument" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vFilter.Type)<<DumpCallStack() );
		return M4_ERROR;
	}
    
	iNumFilter =(m4int16_t) vFilter.Data.DoubleData; 

	//ponemso valores a los argumentos
	for (j=0; j<iNumPar; ++j){
		switch (avNames[j].Type){
			
		case M4CL_CPP_TYPE_NUMBER://por pos
			{
				m_oState1.m_poRecordSet->Current.Filter[iNumFilter]->Arguments()[(m4int16_t)avNames[j].Data.DoubleData].Set(avValues[j]);
				//trabajamos tanto en current como en register
				m_oState1.m_poRecordSet->Register.Filter[iNumFilter]->Arguments()[(m4int16_t)avNames[j].Data.DoubleData].Set(avValues[j]);
				break;
			}
		case M4CL_CPP_TYPE_STRING_POOL: //por nombre
			{
				m_oState1.m_poRecordSet->Current.Filter[iNumFilter]->Arguments()[m_oDynamicPool->GetString(avNames[j].Data.CharData)].Set(avValues[j]);
				//trabajamos tanto en current como en register
				m_oState1.m_poRecordSet->Register.Filter[iNumFilter]->Arguments()[m_oDynamicPool->GetString(avNames[j].Data.CharData)].Set(avValues[j]);
				break;
			}
		default:
			{
				retVal=M4_WARNING;
			}
		}
		
		//vamos liberando
		DeleteVarString(avNames[j]);
		DeleteVarString (avValues[j]);
	}
	RuntimePushDouble (retVal);

	//salimos
	return M4_SUCCESS;
}


//
//AddSortFilter
//
m4return_t ClVM1FilterSort::AddSortFilter		(void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: AddSortFilter ");
#endif

	m4return_t		ret;
	m4VariantType	vOrder, vName, vNumArg, vIndex;
	m4uint8_t		ai8Values	[ M4CL_VM_MAX_FILTER_ARGS ];
	m4uint32_t		ai32Names		[ M4CL_VM_MAX_FILTER_ARGS ];
	m4char_t		*appNames	[ M4CL_VM_MAX_FILTER_ARGS ];
	m4int16_t		i, iNumPar=0;	
	
	//leemos el número de argumentos
	ret=RuntimePop(vNumArg);
    DEBUG_CHECK_RES (ret==M4_SUCCESS && vNumArg.Type==M4CL_CPP_TYPE_NUMBER, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "NumberOfParameters" << "AddSortFilter" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumArg.Type)<<DumpCallStack() );
	
	m4int16_t iNumArg = m4int16_t(vNumArg.Data.DoubleData);

#ifdef _DEBUG
	//mínimo número de args
	if (iNumArg  < 1) { //error->Faltan argumentos
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "AddSortFilter"<<"1-" << DumpCallStack() );
		return M4_ERROR;
	}
#endif
	
	if (iNumArg==1) { //quieren ordenar según un index
		ret=RuntimePop(vIndex);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP , ERRORLOG, "Error getting index in AddSortFilter.")	;
		
		ret=m_oVariantMediator->ToDouble(vIndex);
    
	    if (ret == M4_ERROR) {        
		    m_oVariantMediator->ToPoolString (vIndex);
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONGTYPEININDEX, m_oDynamicPool->GetString(vIndex.Data.CharData) << M4ClCppType (M4CL_CPP_TYPE_NUMBER) <<M4ClCppType(vIndex.Type)<<DumpCallStack() );
	        m_oDynamicPool->DeleteString (vIndex.Data.CharData);
	    }
		else { //ponemos filtro de ordenación
			ret = m_oState1.m_poRecordSet->Current.Filter.AddSort( (m4uint32_t)vIndex.Data.DoubleData);

			if (ret == M4_ERROR) {
				DUMP_CHLOG_ERRORF(M4_CH_VM_WRONGINDEX, (m4uint32_t)vIndex.Data.DoubleData<< M4ObjNodeN(m_oState1.m_poRecordSet->GetpAccess()->GetpCMCR(),m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle() ) <<"AddSortFilter" << DumpCallStack() );
				return M4_ERROR;
	        }
			
			//ponemos tambien en register
			ret = m_oState1.m_poRecordSet->Register.Filter.AddSort( (m4uint32_t)vIndex.Data.DoubleData);
		}
	}
	else{ //lista de nombres-orden
		
		//los argumentos del filtro
		if (iNumArg %2) { //ignoramos un argumento. No es par
			RuntimePopAndDelete();
			--iNumArg;
		}
		
		iNumPar=(iNumArg)/2;
		
		if (iNumPar > M4CL_VM_MAX_FILTER_ARGS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_TOO_MUCH_PARAM_NUM, "AddSortFilter" << DumpCallStack() );

			return M4_ERROR;
		}
		
		for (i=0; i< iNumPar ;++i){ 
			//sacamos valor
			ret=RuntimePop(vOrder);
			DEBUG_CHECK_RES (ret==M4_SUCCESS , M4_CH_VM_RTS_POP , ERRORLOG, "Error getting parameters value in AddFilter.")	;
			
			//sacamos nombre
			ret=RuntimePopString(vName);
			M4CHECK_RES (ret==M4_SUCCESS , M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "ParameterName"<<"AddSortFilter" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vName.Type)<<DumpCallStack() );
			
			//los metemos al final del array
			ai32Names[iNumPar-i-1]=vName.Data.CharData;
			ai8Values[iNumPar-i-1]= (m4uint8_t)vOrder.Data.DoubleData;
			
		}

		/* Bug 0117615
		Hay que recalcular los argumentos porque puede que el pool se redimensione
		*/
		for( i = 0 ; i < iNumPar ; i++ )
		{ 
			appNames[ i ] = m_oDynamicPool->GetString( ai32Names[ i ] ) ;
		}
		
		//llamamos al sort
		ret = m_oState1.m_poRecordSet->Current.Filter.AddSort( iNumPar, appNames, ai8Values);

		if (ret == M4_ERROR) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_SORT_FILTER, DumpCallStack() );
			return M4_ERROR;
		}

		/* Bug 0117615
		Hay que recalcular los argumentos porque puede que el pool se redimensione
		*/
		for( i = 0 ; i < iNumPar ; i++ )
		{ 
			appNames[ i ] = m_oDynamicPool->GetString( ai32Names[ i ] ) ;
		}

		ret = m_oState1.m_poRecordSet->Register.Filter.AddSort( iNumPar, appNames, ai8Values);		
	}
	
	//el retorno
	RuntimePushDouble (ret);

	//ponemso valores a los argumentos y liberamos
	if (iNumPar){
		for (i=0; i<iNumPar; ++i){
			m_oDynamicPool->DeleteString(ai32Names[i]);
		}
	}

	//salimos
	return ret;
}



//--------------------
//ordenación-búsquedas
//--------------------
m4return_t ClVM1FilterSort::Sort     (void)
{
  m4return_t ret;
	m4uint32_t iIndexId, iIndexHandle;
    m4VariantType vIndex, vNum;
	m4uint32_t hNode = m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle();

	RuntimePop (vNum);

	if (vNum.Data.DoubleData == 0)
	{
		
		
		iIndexHandle = m_oState1.m_poAccess->GetpCMCR()->GetNodeDefaultIndex( hNode );
		
		if (iIndexHandle == 0){
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_DEFAULT_INDEX, M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),hNode)<<"Sort" << DumpCallStack() );
			return M4_ERROR;
		}
		
		iIndexId=m_oState1.m_poAccess->GetpCMCR()->GetIndexId (iIndexHandle);
		ret = m_oState1.m_poRecordSet->SetCurrentSearchIndexById (iIndexId);
		if (ret == M4_ERROR) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_DEFAULT_INDEX, iIndexId<<"Sort" << DumpCallStack() );
		}
		m_oState1.m_poRecordSet->Arrange();
	}
	else
	{
	    RuntimePop (vIndex);
		ret=m_oVariantMediator->ToDouble(vIndex);
    
	    if (ret == M4_ERROR) {        
		    m_oVariantMediator->ToPoolString (vIndex);
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONGTYPEININDEX, m_oDynamicPool->GetString(vIndex.Data.CharData)<<M4ClCppType (M4CL_CPP_TYPE_NUMBER) <<M4ClCppType(vIndex.Type)<< DumpCallStack() );
	        m_oDynamicPool->DeleteString (vIndex.Data.CharData);
	    }
	    else {
	

			iIndexId = (m4uint32_t)vIndex.Data.DoubleData;
		    ret = m_oState1.m_poRecordSet->SetCurrentSearchIndexById (iIndexId);
	        if (ret == M4_ERROR) {
				DUMP_CHLOG_ERRORF(M4_CH_VM_WRONGINDEX, (m4uint16_t)vIndex.Data.DoubleData<<M4ObjNodeN(m_oState1.m_poRecordSet->GetpAccess()->GetpCMCR(),hNode ) <<"Sort" << DumpCallStack() );
	        }
			m_oState1.m_poRecordSet->Arrange();
	    }
	}

    RuntimePushDouble (ret);

    return ret;
}

m4return_t ClVM1FilterSort::BSearch (void)
{
    m4return_t ret;
    m4VariantType vNum, vIndex;
    m4VariantType *pvTemp;
	m4uint32_t *piChar;

	m4uint32_t hnode=m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle ();
    ClCompiledMCR   *poCMCR=m_oState1.m_poAccess-> GetpCMCR() ;
    

    RuntimePop (vNum);
    m4uint32_t i, n;
    n = ((m4uint32_t)vNum.Data.DoubleData) - 1;

    pvTemp = new m4VariantType[n];
	piChar = new m4uint32_t [n];

    //Vemos cuantos argumentos trae y los sacamos
    for (i=0; i<n; ++i){
        RuntimePop(pvTemp[n-i-1]);
		if (pvTemp[n-i-1].Type == M4CL_CPP_TYPE_STRING_POOL){

			piChar[n-i-1]=pvTemp[n-i-1].Data.CharData;	
			pvTemp[n-i-1].Data.PointerChar=m_oDynamicPool->GetString(pvTemp[n-i-1].Data.CharData);
			pvTemp[n-i-1].Type = M4CL_CPP_TYPE_STRING_VAR;
		}
		else{
			piChar[n-i-1]=0;
		}

    }

    RuntimePop (vIndex);
	ret=m_oVariantMediator->ToDouble(vIndex);
    
	
    if (ret == M4_ERROR) {        
        m_oVariantMediator->ToPoolString (vIndex);
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONGTYPEININDEX, m_oDynamicPool->GetString(vIndex.Data.CharData)<<M4ClCppType (M4CL_CPP_TYPE_NUMBER) <<M4ClCppType(vIndex.Type)<< DumpCallStack() );
        m_oDynamicPool->DeleteString (vIndex.Data.CharData);
        RuntimePushDouble (-1);
    }
    else {

		m4uint32_t hIndex = poCMCR->GetNodeIndexHandleById (hnode, (m4uint16_t)vIndex.Data.DoubleData);
		if (!hIndex){ //si el handle es 0 es uqe el index no existe
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONGINDEX, (m4uint16_t)vIndex.Data.DoubleData<<M4ObjNodeN(poCMCR,hnode ) <<"BSearch" << DumpCallStack() );
			RuntimePushDouble (-1);
			ret=M4_WARNING;
		}
		if (ret==M4_SUCCESS){
			m4uint32_t iNumItemIndex = poCMCR->GetIndexNumberOfItems (hIndex);
			
			if (iNumItemIndex != n){
				DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "bSearch"<<iNumItemIndex+1 << DumpCallStack() );

				ret=M4_ERROR;

			}
		}
		if (ret==M4_SUCCESS){
			ClRecordSet *prs = m_oState1.m_poRecordSet;    
			
			ret = prs->SetCurrentSearchIndexById ((m4uint32_t)vIndex.Data.DoubleData);
			if (ret == M4_ERROR) {
				DUMP_CHLOG_ERRORF(M4_CH_VM_WRONGINDEX, (m4uint16_t)vIndex.Data.DoubleData<<M4ObjNodeN(poCMCR,hnode ) <<"BSearch" << DumpCallStack() );
				RuntimePushDouble (-1);
			}
			else {
				prs->Register.Begin();
				prs->Register.Find (pvTemp);
				if (prs->Register.Index == M4DM_EOF_INDEX) {
					RuntimePushDouble(-1);
				}
				else{
					RuntimePushDouble (prs->Register.Index);
				}
			}
		}
    }

	//fredom
	for (i=0; i<n; ++i){
		if (piChar[i]!= 0){
			m_oDynamicPool->DeleteString(piChar[i]);
		}
	}
    delete [] pvTemp;
	delete [] piChar;
    return ret==M4_ERROR ?  M4_ERROR :M4_SUCCESS;
}

//-----------------------------------
//Find Register
//-----------------------------------

m4return_t  ClVM1FilterSort::FindRegister( void )
{
	
    m4return_t              iResult;
    m4uint8_t               iMiss ;
    m4uint16_t              i;
    m4uint16_t              iArguments ;
    m4uint32_t              iString ;
    m4int32_t               iRegister ;
	m4uint32_t				aiArguments [ 61 ] ;
    m4VariantType           astArguments[ 61 ] ;
    m4VariantType           vValue ;
	
    

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1,"Trace VM1: FindRegister");
#endif
	
    iResult = M4_SUCCESS ;
    iRegister = -1 ;
    
	
    iResult = RuntimePop( vValue ) ;
	
#ifdef _DEBUG
	if( iResult != M4_SUCCESS )
	{
		g_oChLog<<BeginError(M4_CH_VM_RTS_POP)<<"FindRegister"<<EndLog;
		return( M4_ERROR ) ;
	}
#endif
	
    if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
    {
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Function"<<"FindRegister" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
		return( M4_ERROR ) ;
    }
	
    iArguments = (m4uint16_t) vValue.Data.DoubleData ;


// de momento 61 son ya suficientes = 3 * 20 + 1
	if( iArguments > 61 )
	{
        for( i = 0 ; i < iArguments ; i++ )
        {
            RuntimePop( vValue ) ;
        }
		RuntimePushDouble( iRegister ) ;
		DUMP_CHLOG_ERRORF(M4_CH_VM_TOO_MUCH_PARAM_NUM, "FindRegister" << DumpCallStack() );
		return( M4_ERROR ) ;
	}
	
	
    iMiss = 0 ;
	
    for( i = iArguments ; i > 0 ; i-- )
    {
        iResult = RuntimePop( astArguments[ i - 1 ] ) ;
		
#ifdef _DEBUG
		if( iResult != M4_SUCCESS )
		{
			g_oChLog<<BeginError(M4_CH_VM_RTS_POP)<<"FindRegister"<<EndLog;
            iMiss += 1 ;
		}
#endif
		
        if( astArguments[ i - 1 ].Type == M4CL_CPP_TYPE_STRING_POOL )
        {
            iString = astArguments[ i - 1 ].Data.CharData ;
            astArguments[ i - 1 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
            astArguments[ i - 1 ].Data.PointerChar = m_oDynamicPool->GetString( iString ) ;
			aiArguments[ i - 1 ] = iString ;
        }
		else
		{
			aiArguments[ i - 1 ] = 0 ;
		}
    }
	

    if( iMiss == 0 )
    {
        iResult = m_oState1.m_poRecordSet->Register.FindSec( astArguments, iArguments, &iRegister ) ;
    }
	
	
    for( i = 0 ; i < iArguments ; i++ )
    {
		iString = aiArguments[ i ] ;
		if( iString != 0 && astArguments[ i ].Type == M4CL_CPP_TYPE_STRING_VAR )
		{
			m_oDynamicPool->DeleteString( iString ) ;
		}
    }

    RuntimePushDouble( iRegister ) ;
	
	
    return( M4_SUCCESS ) ;
}
