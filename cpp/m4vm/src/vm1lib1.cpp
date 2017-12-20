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
#include "vm1lib.hpp"

#include "vmres.hpp"
#include "vmachin1.h"
#include "execontx.hpp"
#include "executor.hpp"
#include "clextobj.hpp"
#include "m4log.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "clrgstrs.h"
#include "indexlist.hpp"
#include "node_knl.hpp"
#include "cldbgutil.hpp"
#include "m4objglb.hpp"
#include "clm4objservicecontext.hpp"
#include "vmauto_ptr_array.hpp"
#include "m4regimpl.hpp"
#include "m4lang.hpp"
#include "replaceliterals.hpp"



//-------------------------------------------------------------------------
//Flatten
//-------------------------------------------------------------------------

m4return_t ClVM1BasicLib::Flatten (void){

    m4return_t ret=M4_SUCCESS;
    m4VariantType			vNodeName, vNumArg;
    _ClRegister_Base		*poReadReg;
    m4uint16_t              iNumItems, iArrayPos;
    m4uint32_t              iNumSlices;
    m4uint32_t              hTempInicio, hTempFin, hTempItem;
    m4date_t                dStart, dEnd;
    m4VariantType           vTemp;
    ClSliceIndex            i;
    //ClSliceIndex            jSliceIndex;
    ClItemIndex             jIndex;
    ClAccessRecordSet       *poWriteRecordSet;
    m4VariantType           oTemp;
    ClChannel               *pChannel;
    ClItemIndex             iItemIndex;
    ClItem                  *pTempItem;
    m4date_t                dTimeUnit;
    ClAccessRecordSet       *poCurrentRecordSet;
    m4int32_t               f;
    m4uint32_t				hNodeWrite;
    m4uint32_t				hThisNode;
	ClItemIndex				idxStart, idxEnd;
	m4return_t				*pdone;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Flatten ");
#endif


    poCurrentRecordSet=m_oState1.m_poRecordSet;

    //comprobación previa para acelerar
    if (poCurrentRecordSet->Current.Index == M4DM_EOF_INDEX ) return M4_SUCCESS; //no hacemos nada si no hay regiztro activo
    ClCompiledMCR   *poCMCR=m_oState1.m_poAccess-> GetpCMCR() ;


    //EDU------------>>
    //pChannel=m_oState1.m_poRecordSet->GetpNode_Channel()->GetpChannel();
    pChannel=m_oState1.m_poAccess->GetpChannel();
    //EDU------------<<

    pChannel->UnitDate.Get(oTemp);
    dTimeUnit=oTemp.Data.DoubleData;



    //obtenemos el handle de nuestro nodo
    m4uint32_t hNodeRead=poCMCR->GetItemNodeHandle (m_oState1.m_hItem);
    M4CHECK_RES (hNodeRead!=0, M4_CH_VM_WRONG_THIS_NODE_HANDLE, ERRORLOG, "Flatten" << DumpCallStack() );

    //1º sacar nombre del nodo sobre el que escribiremos
/*    ret=RuntimePopString(vItemName);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_WRONG_ITEM_NAME, ERRORLOG, "wrong item name.")	;
    m4uint32_t  hItemCut=poCMCR->GetNodeItemHandleById (hNodeRead, m_oDynamicPool->GetString(vItemName.Data.CharData) );
    M4CHECK_RES (hItemCut!=NULL, M4_CH_VM_WRONG_ITEM_HANDLE, ERRORLOG, "wrong item handle.")	;
  */  
    //2º sacar nombre del item por el que cortaremos-- si es que tiene arg
    ret=RuntimePop(vNumArg);
    DEBUG_CHECK_RES (ret==M4_SUCCESS && vNumArg.Type==M4CL_CPP_TYPE_NUMBER, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "number_of_parameters" << "flatten" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumArg.Type)<< DumpCallStack() );

    
    hThisNode= m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle();
    
    if (vNumArg.Data.DoubleData >=1){

        if (vNumArg.Data.DoubleData >1){
			DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "Flatten"<<1<<m4int32_t(vNumArg.Data.DoubleData) << DumpCallStack() );
			
			//sacamos todos los argumentos de más que vengan
			for (f=0; f< m4int32_t(vNumArg.Data.DoubleData)-1; ++f){
				ret=RuntimePopAndDelete();
				DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Flatten.")	;
			}
		}

        //nombre del nodo destino
        ret=RuntimePopString(vNodeName);
        M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "NodeId" << "Flatten" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vNodeName.Type)<<DumpCallStack() );
    

        hNodeWrite=poCMCR->GetNodeAliasNodeHandleByAlias (hThisNode,  m_oDynamicPool->GetString(vNodeName.Data.CharData), m_oState1.m_poFormula.GetLevel() );
        if (hNodeWrite==0){
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, m_oDynamicPool->GetString(vNodeName.Data.CharData)<<"Flatten" << DumpCallStack() );
			return M4_ERROR;
		}

        
        
        //obtenemos el RS de escritura
        poWriteRecordSet=&(m_oState1.m_poAccess->Node[ClHandle(hNodeWrite)].RecordSet);
        
		if ( ! poWriteRecordSet->IsValid() ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK,"Flatten"<<m_oDynamicPool->GetString(vNodeName.Data.CharData)<<DumpTreePath(m_oState1.m_poAccess, hNodeWrite) << DumpCallStack() );
			return M4_ERROR;
		}

#if _M4_DEBUG > 10
        DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Flatten. Node = %s" <<LogCat<< m_oDynamicPool->GetString(vNodeName.Data.CharData) );
#endif

        //borramos la cadena
        m_oDynamicPool->DeleteString(vNodeName.Data.CharData);
    
    }
    else{ //sobre el mismo

        poWriteRecordSet=m_oState1.m_poRecordSet;
        hNodeWrite=hThisNode;
    }


    //registro sobre el que vamos a trabajar
    poReadReg=&(poCurrentRecordSet->Current);
    if (poReadReg->Index == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_RECORD_NUMBER, M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),hThisNode)<<"Flatten"<<DumpTreePath(poCurrentRecordSet->GetpAccess(), poCurrentRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }

    iNumItems=poCMCR->GetNodeNumberOfItems (hNodeRead);


    //tenemos que tener en cuenta todos los slices de todos los item.
    m_oDatesStack.Clear (m_oState1.m_dBeginDate, m_oState1.m_dEndDate);

    for (iItemIndex=0; iItemIndex< iNumItems; ++iItemIndex){
        pTempItem= &(poReadReg->Item [iItemIndex] );
        if (pTempItem->ItemDef.Type() == M4CL_ITEM_TYPE_METHOD ||pTempItem->ItemDef.Scope() != M4CL_ITEM_SCOPE_REGISTER) continue;
        
		pTempItem->Value.FillDateStack(&m_oDatesStack); 

    }


    m_oDatesStack.Reduce();
    m_oDatesStack.AddEndDate(dTimeUnit);
    iNumSlices=m_oDatesStack.GetLength();

   
    ClRegister itReg = poWriteRecordSet->Register;
    
    hTempFin=poCMCR->GetNodeEndDateItem(hNodeWrite);
    hTempInicio=poCMCR->GetNodeStartDateItem(hNodeWrite);
	
	//pasamos a indices
	if (hTempInicio!=0){
		idxStart=poCMCR->GetItemIndex(hTempInicio);
	}
	else{
		idxStart = M4DM_EOF_INDEX;
	}

	if (hTempFin!=0){
		idxEnd=poCMCR->GetItemIndex(hTempFin);
	}
	else{
		idxEnd = M4DM_EOF_INDEX;
	}

    //3º Si bloque origen y destino (el current del nodo especif) iguales
    if (poWriteRecordSet->GetCurrentBlock() == m_oState1.m_poRecordSet->GetCurrentBlock()){

        //3.1-a Creamos en destino tantos registros como slices tenga el item
        //por el que partimos en el record actual
        for (i=0; i <iNumSlices-1 ; ++i) {


            itReg.Add();
            ret=m_oDatesStack.GetElement ( (m4uint16_t)i, dStart);
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<<"Flatten. Error getting date from stack.")	;
            ret=m_oDatesStack.GetElement ( m4uint16_t(i+1), dEnd);
            dEnd-= dTimeUnit;
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Flatten. Error getting date from stack.")	;
            
            
            
            for (jIndex=0; jIndex< iNumItems ; ++jIndex){
             
                if (poReadReg->Item[jIndex].ItemDef.Type() == M4CL_ITEM_TYPE_METHOD ||poReadReg->Item[jIndex].ItemDef.Scope() != M4CL_ITEM_SCOPE_REGISTER) continue;
                
				pdone = 0;
                ret=poReadReg->Item[jIndex].Value.Get(dStart, dEnd, vTemp, pdone);
                M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, jIndex<<M4ObjNodeN(poCMCR,hNodeRead)<<"Flatten" << DumpCallStack() );

                ret=itReg.Item[jIndex].Value.Set(vTemp);
                M4CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, jIndex<<M4ObjNodeN(poCMCR,hNodeWrite)<<"Flatten" << DumpCallStack() );
            }
            //rellenamos fecha inicio
            if (idxStart != M4DM_EOF_INDEX) {
                vTemp.Data.DoubleData=dStart;   //---
                vTemp.Type= M4CL_CPP_TYPE_DATE; //---
                ret=itReg.Item[idxStart].Value.Set(vTemp);
            }
            DEBUG_CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, "StartDate#Flatten" << DumpCallStack() );
            
            //rellenamos fecha fin
            if (idxEnd != M4DM_EOF_INDEX) {
                vTemp.Data.DoubleData=dEnd; //---
                vTemp.Type= M4CL_CPP_TYPE_DATE;  //---
                ret=itReg.Item[idxEnd].Value.Set(vTemp);
            }
            DEBUG_CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, "EndDate#Flatten" << DumpCallStack() );
            
            
        }
        //3.1-b Borrar registro Flatten
        
        poReadReg->Destroy();
        
        
    }
    else{
        //si son distinos 
        
        //Leemos las columnas del destino (nombres)
        m4int32_t  *aiItemMap=new m4int32_t[iNumItems];    //para mapear los nombres de los item entre el de lectura y escritura
        m4pcchar_t  pszItemName;
        
		for (iArrayPos=0; iArrayPos<iNumItems; ++iArrayPos){
            pszItemName=poCMCR->GetNodeItemIdByPosition (hNodeRead, iArrayPos);
            hTempItem=poCMCR->GetNodeItemHandleById (hNodeWrite, pszItemName,M4_TRUE);
            if (hTempItem!=NULL){
                //comprobamos que sea de registro y que no sea metodo
                if (poCMCR->GetItemScope(hTempItem) != M4CL_ITEM_SCOPE_REGISTER ||
                    poCMCR->GetItemType (hTempItem) == M4CL_ITEM_TYPE_METHOD){
                    
                    aiItemMap[iArrayPos]=-1;
                }
                else{
                    aiItemMap[iArrayPos]=poCMCR->GetItemIndex (hTempItem);
                }
            }
            else{
                aiItemMap[iArrayPos]=-1;
            }
        }
        //cada columna debe existir en el origen. Sino ->error
        
        //añadims en destino tantos reg. como slices tenga el origen y
        //rellenamos cada columna con la correspondiente del origen
        for (i=0; i <iNumSlices-1 ; ++i) {
            itReg.Add();
            ret=m_oDatesStack.GetElement ( (m4uint16_t)i, dStart);
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Flatten. Error getting date from stack.")	;
            ret=m_oDatesStack.GetElement ( (m4uint16_t)i+1, dEnd);
            dEnd-= dTimeUnit;
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_INTERNAL_ERROR, ERRORLOG, __FILE__<<__LINE__<< "Flatten. Error getting date from stack.")	;

            
            for (jIndex=0; jIndex< iNumItems ; ++jIndex){
                if (aiItemMap[jIndex]>=0){  //si en el destino hay una correspondencia

					pdone = 0;
                    ret=poReadReg->Item[jIndex].Value.Get(dStart, dEnd, vTemp, pdone);
					if (ret==M4_ERROR){
						DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, jIndex<<poCMCR->iNodeN(hNodeRead)<<LogCatAnd<<poCMCR->nNodeN(hNodeRead)<<""  <<"Flatten" << DumpCallStack() );
						delete [] aiItemMap;
						return M4_ERROR;
					}

                    ret=itReg.Item[ ClItemIndex(aiItemMap[jIndex]) ].Value.Set(vTemp);
					if (ret==M4_ERROR){
						DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, aiItemMap[jIndex]<<poCMCR->iNodeN(hNodeWrite)<<LogCatAnd<<poCMCR->nNodeN(hNodeWrite)<<""<<"Flatten" << DumpCallStack() );
						delete [] aiItemMap;
						return M4_ERROR;
					}

                    
                }
            }
            //rellenamos fecha inicio
            if (idxStart != M4DM_EOF_INDEX) {
                vTemp.Data.DoubleData=dStart;   //---
                vTemp.Type= M4CL_CPP_TYPE_DATE; //---
                ret=itReg.Item[idxStart].Value.Set(vTemp);
            }
            DEBUG_CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, "StartDate#Flatten" << DumpCallStack() );
            
            //rellenamos fecha fin
            if (idxEnd != M4DM_EOF_INDEX){
                vTemp.Data.DoubleData=dEnd; //---
                vTemp.Type= M4CL_CPP_TYPE_DATE;  //---
                ret=itReg.Item[idxEnd].Value.Set(vTemp);
            }
            DEBUG_CHECK_RES (ret!=M4_ERROR, M4_CH_VM_ERROR_WRITING_ITEM, ERRORLOG, "EndDate#Flatten." << DumpCallStack() );
            
            
        }
        delete [] aiItemMap;
    }

    
	//metemos un valor de retorno en la pila
	RuntimePushDouble(ret);
    return ret;
}



//-------------------------------------------------
//Fechas del access
//-------------------------------------------------

m4return_t  ClVM1BasicLib:: AppStartDate      (void){
	m4VariantType       ovTemp;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: AppStartDate");
#endif

	m_oState1.m_poAccess->ExecutionStartDate.Get(ovTemp);
       
    RuntimePushDate (ovTemp.Data.DoubleData);
    return M4_SUCCESS;
}


m4return_t  ClVM1BasicLib:: AppEndDate     (void){
	m4VariantType       ovTemp;
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: AppEndDate");
#endif

	m_oState1.m_poAccess->ExecutionEndDate.Get(ovTemp);
        
    RuntimePushDate (ovTemp.Data.DoubleData);
    return M4_SUCCESS;
}


m4return_t  ClVM1BasicLib:: AppRunDate     (void){
	m4VariantType       ovTemp;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: AppRunDate");
#endif

	m_oState1.m_poAccess->ExecutionDate.Get(ovTemp);
        
    RuntimePushDate (ovTemp.Data.DoubleData);
    return M4_SUCCESS;
}




//-------------------------------------------------
// Llamadas a funciones de DLL, ....
//-------------------------------------------------
m4return_t  ClVM1BasicLib ::ExternalMethodCall(void){
    
    /*esta función es especial porque permite llamar a cosas externas
    El la pila tenemos:
        Tipo de objeto
        Nombre Metodo
        Nombre Objeto
        Argumentos del metodo a llamar
    el código para generar esta llamada a una cosa externa es Ln4 generado durante la instalación
    */

    m4return_t ret;
    m4VariantType vType, vMethod, vObject,vNumArgs,vTemp, vResult,*vArgs;
    m4int32_t  iNumArgs,i, iNumBasicArgs;
    m4char_t    *pszTemp;
    m4int32_t iStart=0; 
	m4uint32_t iSize;
	//--type--
    ret=RuntimePop (vType);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ExternalMethodCall.")	;
    //tipo del tipo de objeto
    if (vType.Type!=M4CL_CPP_TYPE_NUMBER){
        ret=m_oVariantMediator->ToDouble (vType);
    }
    if (ret!=M4_SUCCESS){ 
        //salimos con error. No sacamos el los argumentos que deben ir a la 
        //llamada porque ya los sacara el RecoverFromError
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "ObjectType"<<"ExternalObjectCall" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vType.Type)<<DumpCallStack() );

        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

    //--method--
    ret=RuntimePop (vMethod);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ExternalMethodCall.")	;
     //tipo del tipo de objeto
    if (vMethod.Type!=M4CL_CPP_TYPE_STRING_POOL){
        //salimos con error. No sacamos el los argumentos que deben ir a la 
        //llamada porque ya los sacara el RecoverFromError
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "MethodName"<<"ExternalObjectCall" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vMethod.Type)<< DumpCallStack() );

        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }


	//--object--
    ret=RuntimePop (vObject);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ExternalMethodCall.")	;
        //tipo del tipo de objeto
    if (vObject.Type!=M4CL_CPP_TYPE_STRING_POOL){
        //salimos con error. No sacamos el los argumentos que deben ir a la 
        //llamada porque ya los sacara el RecoverFromError
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "ObjectName"<<"ExternalObjectCall" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vObject.Type)<< DumpCallStack() );

        RuntimePushDouble(M4_ERROR);
		m_oDynamicPool->DeleteString(vMethod.Data.CharData);
        return M4_ERROR;
    }

	m4char_t *pszObject ;
	m4char_t *pszMethod ;
	pszObject = new m4char_t [m_oDynamicPool->GetStringLength(vObject.Data.CharData)+1];
	pszMethod = new m4char_t [m_oDynamicPool->GetStringLength(vMethod.Data.CharData)+1];

	strcpy(pszObject,m_oDynamicPool->GetString(vObject.Data.CharData) );  
	strcpy(pszMethod,m_oDynamicPool->GetString(vMethod.Data.CharData) );

	vmauto_ptr_array<m4char_t> poAutoPtrObject(pszObject);
	vmauto_ptr_array<m4char_t> poAutoPtrMethod(pszMethod);

	//borramos las cadenas, por si salimos con error, dejarlas free
	m_oDynamicPool->DeleteString(vMethod.Data.CharData);
	m_oDynamicPool->DeleteString(vObject.Data.CharData);

    /* Esta función es un caso especial, ya que se debe tratar como si fuese LN4
    Es decir, los argumentos del item, no los debemos sacar de la pila, sino simplemente leerlos como
    se hace en LN4.
    */

    //Leemos los argumentos que debamos  y los metemos en un array
    m4uint8_t iIsVariable=m_oState1.m_poAccess-> GetpCMCR() ->GetItemVariableArguments(m_oState1.m_hItem);

    iNumBasicArgs=m_oState1.m_poAccess-> GetpCMCR() ->GetItemNumberOfArguments(m_oState1.m_hItem);

    if (iIsVariable){
        ret=m_oRunTimeStack->GetElement (vNumArgs,-1);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ExternalMethodCall.")	;
            
        //tipo del número de args
        if (vNumArgs.Type!=M4CL_CPP_TYPE_NUMBER){
            ret=m_oVariantMediator->ToDouble (vNumArgs);
        }
        if (ret!=M4_SUCCESS){ 
            //salimos con error. No sacamos los argumentos que deben ir a la 
            //llamada porque ya los sacara el RecoverFromError
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "arg_number"<<"ExternalObjectCall" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumArgs.Type)<< DumpCallStack() );

            RuntimePushDouble(M4_ERROR);
            return M4_ERROR;
        }

        iNumArgs=(int)vNumArgs.Data.DoubleData;
        iStart=1;
    }
    else{ //mos los da el metacanal
        iNumArgs=iNumBasicArgs;
        iStart=0;
    }

	//conseguimos un puntero al objeto a través del manager
    M4ClExternObject* poExternal;
    poExternal=GetVMCSContext()->GetObjContext()->GetObject(pszObject, (m4uint32_t)vType.Data.DoubleData);
	
    if (poExternal==NULL){
        //g_oChLog<<BeginError(M4_CH_VM_CANT_LOAD_OBJECT)<<pszObject<<EndLog;
		//Quito este mensaje poruqe Jesus ya saca uno.
        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }


    //creamos un array para los arguemos
    vArgs=new m4VariantType[iNumArgs];
    m4char_t ** ppcTempStr=new char*[iNumArgs];
    //rellenamso el array
    for (i=0; i<iNumArgs; i++){

        ret=m_oRunTimeStack->GetElement (vTemp, -(iStart+i+1) );
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ExternalMethodCall.")	;

        
        //tratamos las cadenas
        if (vTemp.Type==M4CL_CPP_TYPE_STRING_POOL){

            //hacemos una copia pomiendo como string var
			if (i<iNumBasicArgs){ //solo hacemos comprobación sobre la precision cuando es de los fijos				
				iSize=max ((m4uint32_t) m_oDynamicPool->GetStringLength (vTemp.Data.CharData) , (m4uint32_t)m_oState1.m_poAccess->GetpCMCR()->GetItemArgumentPrecisionByPosition ( m_oState1.m_hItem , (m4uint16_t)i) );
			}
			else{
				iSize=m_oDynamicPool->GetStringLength (vTemp.Data.CharData) ;
			}
            pszTemp=new m4char_t [iSize+1];
            strcpy (pszTemp,m_oDynamicPool->GetString (vTemp.Data.CharData) );

            //hacemos un delete de la cadena del pool 
            //m_oDynamicPool->DeleteString (vTemp.Data.CharData);->>IMPORTANTE:No ahce falta borrarlo
			//PORQUE SE BORRARA CUANDO ACABE LA EJECUCION DEL METODO EN EL DELETE_AT_END_OF_EXE

            vTemp.Data.PointerChar=pszTemp;
            vTemp.Type=M4CL_CPP_TYPE_STRING_VAR;

            ppcTempStr[i]=pszTemp; //para luego poder liberar las cadenas
        }
        else{
            ppcTempStr[i]=0;
        }

        //lo metemos en el array
        vArgs[iNumArgs-i-1]=vTemp;
    }


        //contruimos la estructura de contexo

	ClAccess *poAccess = m_oState1.m_poAccess;
	ClChannelManager *poCM = NULL;
	if (poAccess != NULL) {
		poCM = poAccess->GetpChannel()->GetpChannelManager();
	}
	ClLogonAdaptor *poLA = NULL;
	if (poCM != NULL) {
		poLA = poCM->GetLogonAdaptor();
	}

    ClVMRunContext oContext;
    oContext.m_pVM=m_poVM;
    oContext.m_pAccess=poAccess;
    oContext.m_pRecordSet=m_oState1.m_poRecordSet;
    oContext.m_oRegIndex=m_oState1.m_oRegisterIndex;
    oContext.m_hItem=m_oState1.m_hItem;
	oContext.m_pLA=poLA;

    //llamamos a ejecutar el método
    vResult.Type=M4CL_CPP_TYPE_NULL;
    ret=poExternal->Execute (pszMethod, vArgs, iNumArgs, vResult, &oContext);

    m_poVM->RefreshState()  ;//restauramos el estado por si ha habido reentrada en la llamada a la dll

	
    if (ret!=M4_ERROR){
		//metemos el resultado
		//si el tipo es cadena..
		if (vResult.Type==M4CL_CPP_TYPE_STRING_VAR||vResult.Type==M4CL_CPP_TYPE_STRING_SYS){
			RuntimePushString (vResult.Data.PointerChar, vResult.IsUser);
			//la dll debe borrar sus cadenas  (con una segunda llamada....)
		}
		else{
			RuntimePush(vResult); //lo que nos retornen
		}
		
		//argumentos por referencia
		for (i=iNumBasicArgs -1; i>=0; --i){
			/* Bug 0103177
			El estado puede haber cambiado. Hay que usar el contexto
			*/
			if (oContext.m_pAccess-> GetpCMCR() ->GetItemArgumentTypeByPosition(oContext.m_hItem, (m4uint16_t) i)== M4CL_ITEM_ARGUMENT_TYPE_REFERENCE){
				//si es de tipo string_var lo pasamosa string_pool  
				if (vArgs[i].Type==M4CL_CPP_TYPE_STRING_VAR || vArgs[i].Type==M4CL_CPP_TYPE_STRING_SYS){
					RuntimePushString(vArgs[i].Data.PointerChar, vArgs[i].IsUser);
				}
				else{
					RuntimePush (vArgs[i]);
				}
			}
		}
	}
	//else{ El error ya sale en poExternalDll
		//g_oChLog<<BeginError(M4_CH_VM_CANT_FIND_METHOD)<<pszMethod<<pszObject<<EndLog;
        //no hemos podido llamar a la DLL
    //}
	
	//deletes varios
    delete [] vArgs;
	
    //borrarmos las cadenas
    for (i=0; i<iNumArgs; ++i){
        if (ppcTempStr[i] !=0) {
            delete [] ppcTempStr[i];
        }
    }
    delete [] ppcTempStr;
    
    return ret;
}


//-------------------------------------------
// UnflattenList
//
// Función de bloque que pasa de 2D a 3D los registros cuyo indice están en la lista
//-------------------------------------------

m4return_t ClVM1BasicLib::UnflattenList (ClIndexList & ai_IndexList)
{

    m4uint32_t		hNode, hStart, hEnd;
    m4uint32_t		iNumReg, iNumSlices, iNumItems;
    m4date_t		dTimeUnit ;
    m4VariantType	oTemp;
    ClChannel		*pChannel;
    m4return_t		ret=M4_SUCCESS;
    ClItem			*pTempItem;
	m4date_t		dMinusInf, dPlusInf;
	
	ClStringToDate(M4CL_MINUS_INFINITE_DATE,dMinusInf);
	ClStringToDate(M4CL_PLUS_INFINITE_DATE,dPlusInf);

    //EDU:
    //ClRegister		poNewRegister, poReadReg;
    ClRegister		poNewRegister (m_oState1.m_poAccess), poReadReg (m_oState1.m_poAccess);

    ClItemIndex		iStartIndex, iEndIndex;
    m4VariantType	vTempStart, vTempEnd, vData;
    m4uint32_t		i,k;
    m4bool_t		bret;

    
    ClCompiledMCR   *poCMCR=m_oState1.m_poAccess-> GetpCMCR() ;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Unflatten");
#endif
    
    //EDU--------->>
    //pChannel=m_oState1.m_poRecordSet->GetpNode_Channel()->GetpChannel();
    pChannel=m_oState1.m_poAccess->GetpChannel();
    //EDU---------<<


    pChannel->UnitDate.Get(oTemp);
    dTimeUnit=oTemp.Data.DoubleData;

    //Obtenemos el hanlde del nodoy los handles de los items 
    //de fecha_ini (DTE_START) y fecha_fin (DTE_END)
    hNode       = poCMCR->GetItemNodeHandle    (m_oState1.m_hItem);
    M4CHECK_RES (hNode!=0, M4_CH_VM_WRONG_THIS_NODE_HANDLE, ERRORLOG, "Unflatten" << DumpCallStack() );
    
    hEnd    =poCMCR->GetNodeEndDateItem   (hNode);
    if (hEnd==0){
        DUMP_CHLOG_ERRORF(M4_CH_VM_NO_END_DT_ITEM, M4ObjNodeN(poCMCR,hNode)<<"Unflatten" << DumpCallStack() );
        return M4_ERROR;
    }

    hStart =poCMCR->GetNodeStartDateItem (hNode);
    if (hStart==0){
        DUMP_CHLOG_ERRORF(M4_CH_VM_NO_START_DT_ITEM, M4ObjNodeN(poCMCR,hNode)<< "Unflatten" << DumpCallStack() );
        return M4_ERROR;
    }

    
    //Nos recorremos todos los registros y vamos metiendo en la pila de fechas
	iNumReg = ai_IndexList.Count ();

    if (iNumReg==0 ){ //si no hay registros salimos con éxito
        return M4_SUCCESS;
    }
	//excepcion: Si hay un solo registro y no nos ejecutamos con tramos->dejamos tal cual
	if (iNumReg < 2 && (!m_poVM->GetRunSliceMode(m_oState1.m_poAccess))  ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_UNFLATTEN_NOT_ENOUGH_REG, M4ObjNodeN(poCMCR,hNode) << DumpCallStack() );
		ClRegister oReg(m_oState1.m_poRecordSet->Register);
		oReg.Add();
		oReg<<m_oState1.m_poRecordSet->Register[ai_IndexList.Get(0)];
		m_oState1.m_poRecordSet->Register.Destroy();
		return M4_SUCCESS;
	}


    iStartIndex=m_oState1.m_poAccess->GetpCMCR()->GetItemIndex(hStart);
    iEndIndex  =m_oState1.m_poAccess->GetpCMCR()->GetItemIndex(hEnd);

	//calculamos las fechas mínimas y máximas, acotando por las de ejecución
	m4date_t dMinDate=m_oState1.m_dEndDate;
	m4date_t dMaxDate=m_oState1.m_dBeginDate ;

   
	//preparamos la pila
    //m_oDatesStack.Clear(m_oState1.m_dBeginDate , m_oState1.m_dEndDate); //Vaciamos la pila y ponemos las fechas en la que nos vamos a ejecutar
	m_oDatesStack.Clear(dMinusInf , dPlusInf); 
	
	//leemos las fechas de los registros 
	for (i=0; i<iNumReg; i++)
	{
        ret=m_oState1.m_poRecordSet->Register[ ai_IndexList.Get ( i ) ].Item[iStartIndex].Value.Get(vTempStart);
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "SourceNode.StartDate#UnflattenList" << DumpCallStack() );
        
        ret=m_oState1.m_poRecordSet->Register[ ai_IndexList.Get ( i ) ].Item[iEndIndex].Value.Get(vTempEnd  );
        DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, "SourceNode.EndDate#UnflattenList" << DumpCallStack() );

        //No comprobamos los tipos porque ya hemops mirado los tipos de los items
        if (!IsNull(vTempStart) ){
            m_oDatesStack.AddElement(vTempStart.Data.DoubleData);
			dMinDate=min(vTempStart.Data.DoubleData , dMinDate );
		}
        
        if (!IsNull(vTempEnd) ){
            m_oDatesStack.AddElement(vTempEnd.Data.DoubleData+dTimeUnit);
			dMaxDate=max(vTempEnd.Data.DoubleData, dMaxDate );
        }
    }

	if (dMinDate>dMaxDate) {
		dMinDate=dMaxDate;
	}

    m_oDatesStack.ClipReduce(dMinDate,dMaxDate);
    m_oDatesStack.AddEndDate(dTimeUnit);
    iNumSlices=m_oDatesStack.GetLength();

    iNumItems=m_oState1.m_poRecordSet->Register[0].Item.Count();

    //Añadimos el registro y le creamos slices
    ret=m_oState1.m_poRecordSet->Register.Add();
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_CANT_ADD_RECORD, ERRORLOG, M4ObjNodeN(poCMCR , m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle() )<<"Unflatten"<<DumpTreePath( m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
    
	//El ultimo es el nuevo.
    poNewRegister=m_oState1.m_poRecordSet->Register[ m_oState1.m_poRecordSet->Register.Count() - 1 ];

    //reservamos espacio para cada slice
        //i -> iterador de item
        //k -> iterador del registro
    for (i=0; i< iNumItems; i++)
	{
        pTempItem=&(poNewRegister.Item[ClItemIndex (i) ]);
        
        if (pTempItem->ItemDef.Type() == M4CL_ITEM_TYPE_METHOD ||pTempItem->ItemDef.Scope() != M4CL_ITEM_SCOPE_REGISTER) continue;

        //reservamos eespacio para los slices y los añadimos
        ret = pTempItem->Value.AllocSlices(&m_oDatesStack);
        
        //vamos iterando por cada registro y metemos valores en este item
        for (k=0; k<iNumReg; k++)
		{
            poReadReg= m_oState1.m_poRecordSet->Register[ ai_IndexList.Get ( k ) ];

            poReadReg.Item [iStartIndex ].Value.Get(vTempStart);
            poReadReg.Item [iEndIndex   ].Value.Get(vTempEnd  );

			if (vTempEnd.Type==M4CL_CPP_TYPE_NULL || vTempStart.Type==M4CL_CPP_TYPE_NULL || vTempEnd.Data.DoubleData   < m_oState1.m_dBeginDate || vTempStart.Data.DoubleData > m_oState1.m_dEndDate) continue;
			if (vTempStart.Data.DoubleData < m_oState1.m_dBeginDate) vTempStart.Data.DoubleData = m_oState1.m_dBeginDate;
			if (vTempEnd.Data.DoubleData   > m_oState1.m_dEndDate)   vTempEnd.Data.DoubleData = m_oState1.m_dEndDate;

            bret=pTempItem->Value.MoveTo (vTempStart.Data.DoubleData, vTempEnd.Data.DoubleData);
            if (!bret)
			{
				poNewRegister.Destroy();
                DUMP_CHLOG_ERRORF(M4_CH_VM_OVERLAPPED_DATES, k<<M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR() ,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle() )<<"Unflatten" << DumpCallStack() );
                return M4_ERROR;
            }

            //leemos del registro
            ret=poReadReg.Item[ClItemIndex(i)].Value.Get(vData);
            if (ret!=M4_SUCCESS)
			{
				poNewRegister.Destroy();
                g_oChLog<<BeginError(M4_CH_VM_ERROR_READING_ITEM)<<M4ObjNodeItemI(poReadReg.GetpAccess()->GetpCMCR(), poReadReg.Item.ItemDef.GetHandle() )
																<<"Unflatten"
																<<DumpCallStack()
																<<EndLog;
                return M4_ERROR;
            }

            //escribimos el registro
            ret=pTempItem->Value.Set (vData);
            if (ret!=M4_SUCCESS)
			{
				poNewRegister.Destroy();
                g_oChLog<<BeginError(M4_CH_VM_WRITING_SLICE)<<M4ObjNodeItemI(pTempItem->GetpAccess()->GetpCMCR() ,pTempItem->ItemDef.GetHandle() )
															<<DateLog (vTempStart.Data.DoubleData)
															<<DateLog (vTempEnd.Data.DoubleData)
															<<"Unflatten"
															<<DumpCallStack()
															<<EndLog;
                return M4_ERROR;
            }

            
        }

        //compactamos
        pTempItem->Value.Compact();

    }

    //nos cepillamos los registros originales
	//ai_IndexList debe ser una lista de orden creciente para poder eliminar los registros de atrás a adelante
	for (i = iNumReg; i != 0; --i)
	{
        m_oState1.m_poRecordSet->Register[ ai_IndexList.Get ( i - 1 ) ].Destroy();    
    }    

    //si tenemos un registro (que lo tenemos), movemos el current a el
    if (m_oState1.m_poRecordSet->Current.Count() >0 ){
        
        ClAccessRecordSet *poCurrentRecordSet=m_oState1.m_poRecordSet;
        poCurrentRecordSet->Current.MoveTo(0);

    }

    return M4_SUCCESS;

}

//-------------------------------------------
// Unflatten
// Función de bloque que pasa de 2D a 3D 
//-------------------------------------------
m4return_t ClVM1BasicLib::Unflatten (void)
{
    m4uint32_t iNumReg, i;
	m4uint32_t iNumValidReg;
	m4uint32_t iNewRegisters = 0;
	ClIndexList oIndexList;
	m4VariantType vArgument;
	m4int32_t iNumArgs, arg;
	m4return_t ret;

	ret = RuntimePop(vArgument);

	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Unflatten.")	;

	iNumArgs = (m4int32_t) vArgument.Data.DoubleData;

    
    
	iNumReg=m_oState1.m_poRecordSet->Register.Count();

    if (iNumReg==0 ){ //si no hay registros salimos con éxito
		//Sacamos los args de la pila
		for (arg=0; arg<iNumArgs; arg++)
		{
			RuntimePopAndDelete();
		}

        RuntimePushDouble (M4_SUCCESS); 
        return M4_SUCCESS;
    }

	oIndexList.Init ( iNumReg );
	
	if (iNumArgs == 0 )
	{
		for (i=0; i<iNumReg; i++)
		{
			oIndexList.Add ( i );
		}

		ret = UnflattenList ( oIndexList );
	}
	else
	{
		m4bool_t bEqual;
		m4pcchar_t pcKeyItemId;
		m4uint32_t hKeyItemHandle, iNodeHandle;
		ClItemIndex * piKeyItemIndex;
		m4VariantType * vItemKeyVal, vTemp;		
		ClCompiledMCR   *poCMCR = m_oState1.m_poAccess-> GetpCMCR();
		
		iNodeHandle = poCMCR->GetChannelNodeHandleById (m_oState1.m_poRecordSet->GetpNodeDef()->Id(), M4_TRUE);

		piKeyItemIndex = new ClItemIndex[iNumArgs];
		vItemKeyVal = new m4VariantType[iNumArgs];

		for (arg=0; arg<iNumArgs; arg++)
		{
			ret = RuntimePopString(vArgument);
			M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "KeyItem" << "Unflatten" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vArgument.Type)<<DumpCallStack() );

			pcKeyItemId = m_oDynamicPool->GetString(vArgument.Data.CharData);

			hKeyItemHandle = poCMCR->GetNodeItemHandleById (iNodeHandle, pcKeyItemId);

			//liberamos la cadena
			m_oDynamicPool->DeleteString(vArgument.Data.CharData);

	        if ( hKeyItemHandle == 0 ){				
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, pcKeyItemId<<M4ObjNodeN(poCMCR,iNodeHandle)<<"Unflatten" << DumpCallStack() );

				delete [] piKeyItemIndex;
				delete [] vItemKeyVal;
			    return M4_ERROR;
	        }

			//Obtenemos los indices
			piKeyItemIndex[arg] = poCMCR->GetItemIndex (hKeyItemHandle);

		}

		while ( iNumValidReg = m_oState1.m_poRecordSet->Register.Count() - iNewRegisters )
		{
			oIndexList.Reset ( );

			for (arg=0; arg<iNumArgs; arg++)
			{
				m_oState1.m_poRecordSet->Register[ 0 ].Item [ piKeyItemIndex[arg] ].Value.Get( vItemKeyVal[arg] );
			}

			oIndexList.Add ( 0 );

			for (i=1; i < iNumValidReg; i++)
			{
				bEqual = M4_TRUE;

				for (arg=0; arg<iNumArgs; arg++)
				{
					m_oState1.m_poRecordSet->Register[ i ].Item [ piKeyItemIndex[arg] ].Value.Get( vTemp );

					if ( !(vItemKeyVal[arg] == vTemp) )
					{
						bEqual = M4_FALSE;
						break;
					}
				}

				if (bEqual == M4_TRUE)
				{
					oIndexList.Add ( i );
				}

			}

			ret = UnflattenList ( oIndexList );

			if ( ret == M4_ERROR )
				break;

			iNewRegisters++;
		}

		delete [] piKeyItemIndex;
		delete [] vItemKeyVal;
	}

	if ( ret == M4_ERROR )
		return M4_ERROR;

    RuntimePushDouble (M4_SUCCESS); 

    return M4_SUCCESS;

}



m4return_t ClVM1BasicLib::JoinSlices ( void )
{
    m4uint32_t iNumReg, iNumSlices, iNumItems;
    m4date_t   dTimeUnit ;
    m4VariantType oTemp;
    ClChannel   *pChannel;    
    ClItem  *pTempItem, *pReadItem;
    
    //EDU:
    //ClRegister  poNewRegister, poReadReg;
    ClRegister  poNewRegister (m_oState1.m_poAccess), poReadReg (m_oState1.m_poAccess);

    //ClItemIndex iStartIndex;
    //ClItemIndex iEndIndex;
    m4VariantType vData, vSliceVal;
    m4uint32_t i,k;
	ClSliceIndex j;
    m4date_t dTempStartDate, dTempEndDate, dSliceStartDate, dSliceEndDate;
	m4date_t  dPlusInfiniteDate, dMinusInfiniteDate, dFirstStartDate, dLastEndDate;
	m4return_t  ret = M4_SUCCESS;

	ClStringToDate(M4CL_PLUS_INFINITE_DATE, dPlusInfiniteDate);
	ClStringToDate(M4CL_MINUS_INFINITE_DATE, dMinusInfiniteDate);

    
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1,"Trace VM1: Unflatten");
#endif


    //EDU----------->>
    //pChannel=m_oState1.m_poRecordSet->GetpNode_Channel()->GetpChannel();
    pChannel=m_oState1.m_poAccess->GetpChannel();
    //EDU-----------<<

    pChannel->UnitDate.Get(oTemp);
    dTimeUnit=oTemp.Data.DoubleData;
    
	iNumReg=m_oState1.m_poRecordSet->Register.Count();

    iNumItems=m_oState1.m_poRecordSet->Register[0].Item.Count();

    //Añadimos el registro
    ret = m_oState1.m_poRecordSet->Register.Add();
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_CANT_ADD_RECORD, ERRORLOG, M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle() )<<"Unflatten"<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())  << DumpCallStack() );
    
	//El ultimo es el nuevo.
    poNewRegister=m_oState1.m_poRecordSet->Register[ iNumReg ];

    //reservamos espacio para cada slice
        //i -> iterador de item
        //k -> iterador del registro
	    //k -> iterador del slice
    for (i=0; i< iNumItems; i++){

		// Obtenemos el Item destino
        pTempItem=&(poNewRegister.Item[ClItemIndex (i) ]);

		// nos situamos en el Item fuente del recordset
		m_oState1.m_poRecordSet->Register[ 0 ].Item[ClItemIndex (i) ];
        
        if (pTempItem->ItemDef.Type() == M4CL_ITEM_TYPE_METHOD ||pTempItem->ItemDef.Scope() != M4CL_ITEM_SCOPE_REGISTER) continue;


		//Calculamos la fecha minima y maxima de los todos los tramos
		dFirstStartDate=dPlusInfiniteDate;
		dLastEndDate=dMinusInfiniteDate;

        for (k=0; k<iNumReg; k++)
		{
            poReadReg= m_oState1.m_poRecordSet->Register[ k ];

			pReadItem = &(poReadReg.Item);

			dFirstStartDate=min(dFirstStartDate, pReadItem->Value.GetFirstStartDate());
			
			dLastEndDate=max(dLastEndDate, pReadItem->Value.GetLastEndDate());
        }

	    //--Resetear la pila de fechas
		//Vaciamos la pila y ponemos las fechas en la que nos vamos a ejecutar??No se	      
	    m_oDatesStack.Clear(dFirstStartDate , dLastEndDate); 


       //vamos iterando por cada registro y metemos valores en este item
        for (k=0; k<iNumReg; k++)
		{
            poReadReg= m_oState1.m_poRecordSet->Register[ k ];

			pReadItem = &(poReadReg.Item);

			iNumSlices=pReadItem->Value.Count();

			//Metemos en la pila la fecha de los tramos
			pReadItem->Value.FillDateStack(&m_oDatesStack); 
            
        }

		//Reducimos la pila
		m_oDatesStack.Reduce();
     
        //añadimos slices 
		iNumSlices = m_oDatesStack.GetLength();

		/* Bug 0078395
		Se añade la fecha fin
		*/
		m_oDatesStack.AddEndDate( dTimeUnit ) ;

        for (j=0; j< iNumSlices; j++)
		{
            ret = m_oDatesStack.GetElement( (m4uint16_t)j, dTempStartDate);

#ifdef _DEBUG
			if (ret != M4_SUCCESS){
				poNewRegister.Destroy();
				g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<< __FILE__<<__LINE__<< "Unflatten. Error getting date from stack."<<EndLog;
				return M4_ERROR;
			}
#endif

			/* Bug 0078395
			Se lee la fecha fin
			*/
            ret = m_oDatesStack.GetElement( (m4uint16_t)j+1, dTempEndDate);

#ifdef _DEBUG
			if (ret != M4_SUCCESS){
				poNewRegister.Destroy();
				g_oChLog<<BeginError(M4_CH_VM_INTERNAL_ERROR)<< __FILE__<<__LINE__<< "Unflatten. Error getting date from stack."<<EndLog;
				return M4_ERROR;
			}
#endif

			dTempEndDate -= dTimeUnit ;

			//DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_GETTING_DATE_FROM_STACK, ERRORLOG, "Unflatten. Error getting date from stack.")	;
            //--usamos addslice con solo la fecha de inicio porque las tenemos ordenadas
            //y consegutivas y es + rapido

			/* Bug 0078395
			Realmente si hace falta meter la fecha fin, porque la que se mete por debajo
			que es la de trabajo, puede no coincidir con la de los tramos en los que trabajamos
			*/
            pTempItem->Value.AddSlice (dTempStartDate, dTempEndDate);

		}
        //--metemos la fecha de fin--
        pTempItem->Value.SetEndDate (dLastEndDate);


		//Para cada slice asignamos el valor (suma de los registros)
        for (j=0; j< iNumSlices; j++)
		{
			 vSliceVal.Type = M4CL_CPP_TYPE_NULL;
			 vSliceVal.Data.DoubleData = 0;

			 dSliceStartDate = pTempItem->Value[j].GetStartDate ();
			 dSliceEndDate = pTempItem->Value[j].GetEndDate ();

			 for (k=0; k<iNumReg; k++)
			 {
			      poReadReg= m_oState1.m_poRecordSet->Register[ k ];

				  pReadItem = &(poReadReg.Item);			  	  

				  pReadItem->Value.Get(dSliceStartDate, dSliceEndDate, vData);


				  if ( vData.Type == M4CL_CPP_TYPE_NUMBER )
				  {
					  // Si es numerico hacemos la suma de los valores.
				  	  vSliceVal.Type = vData.Type;
					  vSliceVal.Data.DoubleData += vData.Data.DoubleData; 
				  }
				  else if ( vData.Type == M4CL_CPP_TYPE_NULL )
				  {
					  if ( vSliceVal.Type != M4CL_CPP_TYPE_NUMBER ) 
					  {
						/* Bug 0079588
						Si es nulo no hay que asignar, sólo dejar a nulo y limpiar el double
						*/
						vSliceVal.Type = M4CL_CPP_TYPE_NULL;
						vSliceVal.Data.DoubleData = 0;
					  }
				  }
				  else
				  {
					  // Si no es numero nos quedamos con el ultimo valor.
					  vSliceVal = vData;
				  }


			  }
			 
			pTempItem->Value[j].Set ( vSliceVal );
        }

 

        //compactamos
        pTempItem->Value.Compact();

    }


    //nos cepillamos los registros originales
	for (i = iNumReg; i != 0; --i){
        m_oState1.m_poRecordSet->Register[ i - 1 ].Destroy();    
    }    

	RuntimePushDouble (M4_SUCCESS); 

    return M4_SUCCESS;

}





//******************PARA JESUS*********************
// gracias pipo


m4return_t ClVM1BasicLib::LookUp(void)
{

    m4return_t          iResult ;
	
	m4uchar_t			c ;
	m4uint8_t			iAutoFilter ;
	m4uint8_t			iCacheable ;
	m4uint8_t			iCallApplyFilter ;
	m4uint8_t			iStart ;
	m4uint8_t			iVarARguments ;
	m4uint8_t			iBadArguments ;

	m4uint16_t			iLength ;
	m4uint16_t			iDateLength ;
	m4uint16_t			iPKFilterLength ;
	m4uint16_t			iSecFilterLength ;
	m4uint16_t			iElement ;
    m4uint16_t          j ;
	m4uint16_t			iArgumentNumber ;
	m4uint16_t			iPrimary ;
	m4uint16_t			iSecondary ;
	m4uint16_t			iMaxSecondary ;

	m4uint32_t			iHandle ;
	m4uint32_t			iNode ;
	m4uint32_t			iSecNode ;
	m4uint32_t			iItem ;
	m4uint32_t			iAppItem ;
	m4uint32_t			iSecItem ;
	m4uint32_t			iPkFilter;

    m4pcchar_t          pccApplyFilterItem ;
    m4pcchar_t          pccSecondaryNode ;
    m4pcchar_t			pccItemId ;
	m4pchar_t			pcSecondary ; 
    m4pchar_t			pcRAMFilter = NULL ;
    m4pchar_t			pcSQLFilter = NULL ;
    m4pchar_t			pcPKFilter = NULL ;
    m4pcchar_t			pccSecFilter = NULL ;
	
    m4char_t			acDateFilter[ ( M4CL_MAX_ITEM_ID + 25 ) * 4 + 15 ] ;	// " <= {1800-01-01 00:00:00}" * 4 + " AND " * 3

    m4VariantType       vValue ;

	m4VariantType		vStartDate ;
	m4VariantType		vEndDate ;
	m4VariantType		vCorStartDate ;
	m4VariantType		vCorEndDate ;

    m4VariantType		avSecondaryValues  [ M4CL_MAX_LOOKUP_ELEMENTS ] ;
    m4pchar_t			apcSecondaryColumns[ M4CL_MAX_LOOKUP_ELEMENTS ] ;
	m4bool_t			abSecondaryValues  [ M4CL_MAX_LOOKUP_ELEMENTS ] ;

    ClCompiledMCR       *poCMCR ;
    ClAccessRecordSet	*poDRecordSet ;
    ClAccessRecordSet	*poDSecRecordSet ;
    ClAccess            *poAccess ;
    ClNode              *poDSecNode ;
	ClItem				*poItem ;
	ClItem				*poSecItem ;


	// ==========================================================
	// inicializacion ===========================================
	// ==========================================================
	
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1,"Trace VM1: LookUp" ) ;
#endif
	
	// ==========================================================
	// argumentos de la pila ====================================
	// ==========================================================
	
	// Número de argumentos
	
    iResult = RuntimePopDouble( vValue ) ;
    DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Pop Error in LookUp. Argument Number" ) ;
	
    iArgumentNumber = m4int16_t( vValue.Data.DoubleData ) ;
	
    if( iArgumentNumber < 7 )
    {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "LookUp"<<"7-" << DumpCallStack() );
        return( M4_ERROR ) ;
    }


// sacamos las fechas

	iResult = m_oRunTimeStack->Top( vCorEndDate ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_RTS_POP, "Pop Error in LookUp. Cor End Date" ) ;
		return( M4_ERROR ) ;
	}
	m_oRunTimeStack->Pop() ;

	if( vCorEndDate.Type != M4CL_CPP_TYPE_DATE )
	{
		if( vCorEndDate.Type == M4CL_CPP_TYPE_STRING_POOL )
		{
			m_oDynamicPool->DeleteString( vCorEndDate.Data.CharData ) ;
		}
		vCorEndDate.Type = M4CL_CPP_TYPE_NULL ;
		vCorEndDate.Data.DoubleData = 0 ;
	}


	iResult = m_oRunTimeStack->Top( vCorStartDate ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_RTS_POP, "Pop Error in LookUp. Cor Star Date" ) ;
		return( M4_ERROR ) ;
	}
	m_oRunTimeStack->Pop() ;

	if( vCorStartDate.Type != M4CL_CPP_TYPE_DATE )
	{
		if( vCorStartDate.Type == M4CL_CPP_TYPE_STRING_POOL )
		{
			m_oDynamicPool->DeleteString( vCorStartDate.Data.CharData ) ;
		}
		vCorStartDate.Type = M4CL_CPP_TYPE_NULL ;
		vCorStartDate.Data.DoubleData = 0 ;
	}


	iResult = m_oRunTimeStack->Top( vEndDate ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_RTS_POP, "Pop Error in LookUp. End Date" ) ;
		return( M4_ERROR ) ;
	}
	m_oRunTimeStack->Pop() ;

	if( vEndDate.Type != M4CL_CPP_TYPE_DATE )
	{
		if( vEndDate.Type == M4CL_CPP_TYPE_STRING_POOL )
		{
			m_oDynamicPool->DeleteString( vEndDate.Data.CharData ) ;
		}
		vEndDate.Type = M4CL_CPP_TYPE_NULL ;
		vEndDate.Data.DoubleData = 0 ;
	}


	iResult = m_oRunTimeStack->Top( vStartDate ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_RTS_POP,"Pop Error in LookUp. Start Date" ) ;
		return( M4_ERROR ) ;
	}
	m_oRunTimeStack->Pop() ;

	if( vStartDate.Type != M4CL_CPP_TYPE_DATE )
	{
		if( vStartDate.Type == M4CL_CPP_TYPE_STRING_POOL )
		{
			m_oDynamicPool->DeleteString( vStartDate.Data.CharData ) ;
		}
		vStartDate.Type = M4CL_CPP_TYPE_NULL ;
		vStartDate.Data.DoubleData = 0 ;
	}


// sacamos el nodo opcional. dejamos que sea nulo

	iResult = RuntimePop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_RTS_POP, "Pop Error in LookUp. Secondary Node" ) ;
		return( M4_ERROR ) ;
	}

	if( vValue.Type == M4CL_CPP_TYPE_NULL )
	{
		pccSecondaryNode = "" ;
	}
	else if( vValue.Type == M4CL_CPP_TYPE_STRING_POOL )
	{
		pccSecondaryNode = m_oDynamicPool->GetString( vValue.Data.CharData ) ;

		if( pccSecondaryNode == NULL )
		{
			pccSecondaryNode = "" ;
		}

		m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
	}
	else
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Secondary Node" << "LookUp" << M4ClCppType( M4CL_CPP_TYPE_STRING_POOL ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
		return( M4_ERROR ) ;
	}


// sacamos el item a aplicar

    iResult = RuntimePopString( vValue ) ; 
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ApplyFilter Item" << "LookUp"  << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );

    pccApplyFilterItem = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
    M4CHECK_RES( pccApplyFilterItem!=NULL, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "ApplyFilter Item" << "LookUp" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(M4CL_CPP_TYPE_NULL)<< DumpCallStack() );

    m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;


// sacamos el número de secundarios

    iResult = RuntimePopNumber( vValue ) ;
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Secondary PK Number" << "LookUp"  << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vValue.Type)<< DumpCallStack() );
	
	iSecondary = m4uint16_t( vValue.Data.DoubleData ) ;

	iMaxSecondary = ( iArgumentNumber - 7 ) / 2 ;

	if( iMaxSecondary > M4CL_MAX_LOOKUP_ELEMENTS )
	{
		iMaxSecondary = M4CL_MAX_LOOKUP_ELEMENTS ;
	}

	if( iSecondary > iMaxSecondary )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_TOO_MANY_COLUMS, iSecondary<<iMaxSecondary<<"LookUp" << DumpCallStack() );
		RuntimePushDouble( 0 ) ;
		return( M4_ERROR ) ;
	}


// sacamos las parejas de secundarios

	for( j = 0 ; j < iSecondary ; j++ )
	{
		iElement = iSecondary - j - 1 ;

		iResult = m_oRunTimeStack->Top( vValue ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_RTS_POP, "Pop Error in LookUp. Secondary PK Value: <" << LogCat << iElement + 1 << LogCat << ">" ) ;
			return( M4_ERROR ) ;
		}

		if( vValue.Type == M4CL_CPP_TYPE_STRING_POOL )
		{
			pcSecondary = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
			m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
			vValue.Data.PointerChar = pcSecondary ;
		}

		avSecondaryValues[ iElement ].Type = vValue.Type ;
		avSecondaryValues[ iElement ].Data.DoubleData  = vValue.Data.DoubleData ;
		avSecondaryValues[ iElement ].Data.CharData    = vValue.Data.CharData ;
		avSecondaryValues[ iElement ].Data.PointerChar = vValue.Data.PointerChar ;

		m_oRunTimeStack->Pop() ;

		
		iResult = RuntimePopString( vValue ) ;

		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Secondary PK Column: <" << LogCat << iElement + 1 << LogCat << ">" << "LookUp"  << M4ClCppType(M4CL_CPP_TYPE_STRING)<<M4ClCppType(vValue.Type)<< DumpCallStack() );

		apcSecondaryColumns[ iElement ] = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
		m_oDynamicPool->DeleteString( vValue.Data.CharData ) ;
	}


// esto sólo para depurar la llamada al applyfilter
	iPrimary = iArgumentNumber - iSecondary * 2 - 7 ;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "\t\tLookUp. Item: <" <<LogCat<< pccApplyFilterItem <<LogCat<<"> - Node: <" <<LogCat<< pccSecondaryNode <<LogCat<<">") ;
#endif
	

// ==========================================================
// inicialización de variables ==============================
// ==========================================================

    poAccess = m_oState1.m_poAccess ;
	
    poCMCR = poAccess->GetpCMCR() ;
	
#ifdef _DEBUG
	if( poCMCR == NULL )
	{
		g_oChLog<<BeginError (M4_CH_VM_INTERNAL_ERROR)<<__FILE__<<__LINE__<<"Null mcr in LookUp."<<EndLog;
        RuntimePushDouble( 0 ) ;
		return( M4_ERROR ) ;
	}
#endif

	poDRecordSet = m_oState1.m_poRecordSet ;
    
	if( !poDRecordSet->IsValid() )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK,"LookUp"<<poDRecordSet->GetpNodeDef()->Id()<<DumpTreePath(poDRecordSet->GetpAccess(), poDRecordSet->GetpNodeDef()->GetHandle())  << DumpCallStack() );
        RuntimePushDouble( 0 ) ;
        return( M4_ERROR ) ;
	}

    iNode = poDRecordSet->GetpNodeDef()->GetHandle() ;



// borramos los filtros en RAM si hubiese =======================================================

	if( !poDRecordSet->Current.Filter[ M4_LOOKUP_RAM_FILTER ].IsTerminator() )
	{
		iResult = poDRecordSet->Current.Filter[ M4_LOOKUP_RAM_FILTER ].Delete() ;
	}

	if( !poDRecordSet->Register.Filter[ M4_LOOKUP_RAM_FILTER ].IsTerminator() )
	{
		iResult = poDRecordSet->Register.Filter[ M4_LOOKUP_RAM_FILTER ].Delete() ;
	}


	if( !poDRecordSet->Current.Filter[ M4_LOOKUP_SQL_FILTER ].IsTerminator() )
	{
		iResult = poDRecordSet->Current.Filter[ M4_LOOKUP_SQL_FILTER ].Delete() ;
	}

	if( !poDRecordSet->Register.Filter[ M4_LOOKUP_SQL_FILTER ].IsTerminator() )
	{
		iResult = poDRecordSet->Register.Filter[ M4_LOOKUP_SQL_FILTER ].Delete() ;
	}




// ==========================================================
// algoritmo ================================================
// ==========================================================

	iStart = 0 ;
	iCacheable = poCMCR->GetChannelIsCacheable() ;


// obtenemos el item del nodo secundario =====================

	if( *pccSecondaryNode != '\0')
	{
		iSecNode = poCMCR->GetChannelNodeHandleById( pccSecondaryNode ) ;

		if( iSecNode == 0 )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_NODE, pccSecondaryNode<< M4Obj(poCMCR) <<"Lookup" << DumpCallStack() );
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		iSecItem = poCMCR->GetNodeItemHandleById( iSecNode, M4_ITEM_APPLY_FILTER ) ;

		if( iSecItem == 0 )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, M4_ITEM_APPLY_FILTER<<M4ObjNodeN(poCMCR,iSecNode)<<"LookUp" << DumpCallStack() );

			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		iVarARguments = poCMCR->GetItemVariableArguments( iSecItem ) ;
		iArgumentNumber = poCMCR->GetItemNumberOfArguments( iSecItem ) ;

		iBadArguments = 0 ;

		if( iVarARguments == 1 )
		{
			if( iArgumentNumber > 1 )
			{
				iBadArguments = 1 ;
			}
		}
		else
		{
			if( iArgumentNumber != 1 )
			{
				iBadArguments = 1 ;
			}
		}

		if( iBadArguments == 1 )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, M4_ITEM_APPLY_FILTER<<M4ObjNodeN(poCMCR,iSecNode)<<"LookUp" << DumpCallStack() );
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		poDSecNode = &( poAccess->Node[ ClHandle( iSecNode ) ] ) ;
		
#ifdef _DEBUG
		if( poDSecNode == NULL )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_NODE, "SecundaryNode"<<M4Obj(poCMCR)<<"LookUp" << DumpCallStack() );
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}
#endif

		poDSecRecordSet = &( poDSecNode->RecordSet ) ;

		if( !poDSecRecordSet->IsValid() )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK,"LookUp"<<poDSecRecordSet->GetpNodeDef()->Id()<<DumpTreePath(poDSecRecordSet->GetpAccess(), poDSecRecordSet->GetpNodeDef()->GetHandle())  << DumpCallStack() );
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}
	}
	else
	{
		iSecItem = 0 ;
	}


// obtenemos el item applyfilter =====================================================

// si es el APPLY_LIST y es no cacheable no lo ejecutamos

	if( iCacheable == M4CL_CACHEABLE_FALSE && strcmpi( pccApplyFilterItem, "APPLY_LIST" ) == 0 )
	{
// Lo ponemos otra vez a que SI aplique
//		iCallApplyFilter = 0 ;
		iCallApplyFilter = 1 ;
	}
	else
	{
		iCallApplyFilter = 1 ;
	}

	if( iCallApplyFilter == 1 )
	{
		vValue.Type = M4CL_CPP_TYPE_NUMBER ;
		vValue.Data.DoubleData = iCacheable ;

		iResult = RuntimePush( vValue ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_PUSH,"LookUp. IsCacheable");
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

			
		iAppItem = poCMCR->GetNodeItemHandleById( iNode, pccApplyFilterItem ) ;

		if( iAppItem == 0 )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, pccApplyFilterItem<<M4ObjNodeN(poCMCR,iNode)<<"LookUp" << DumpCallStack() );
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		iVarARguments = poCMCR->GetItemVariableArguments( iAppItem ) ;
		iArgumentNumber = poCMCR->GetItemNumberOfArguments( iAppItem ) ;

		iBadArguments = 0 ;

		if( iVarARguments == 1 )
		{
			if( iArgumentNumber > ( iPrimary + 1 ) )
			{
				iBadArguments = 1 ;
			}
		}
		else
		{
			if( iArgumentNumber != ( iPrimary + 1 ) )
			{
				iBadArguments = 1 ;
			}
		}

		if( iBadArguments == 1 )
		{
			if( iVarARguments == 1 ){
				DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_ITEM_PARAM_NUM, pccApplyFilterItem << LogCat<< "(LookUp)"<<M4ObjNodeN(poCMCR,iNode) <<  ">="<< LogCat<< iArgumentNumber << iPrimary + 1<<DumpCallStack() );
			}
			else{
				DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_ITEM_PARAM_NUM, pccApplyFilterItem << LogCat<< "(LookUp)"<<M4ObjNodeN(poCMCR,iNode) << iArgumentNumber <<iPrimary + 1<< DumpCallStack() );

			}
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}
	}


// aplicamos las fechas ==============================================================


	iAutoFilter = poCMCR->GetNodeIdAutoFilter( iNode ) ;

	if( iCacheable == M4CL_CACHEABLE_TRUE )
	{
		*acDateFilter = '\0' ;

		if( ( iAutoFilter & M4CL_NODE_AUTO_FILTER_APPLICATION ) != 0 )
		{
			iHandle = poCMCR->GetNodeStartDateItem( iNode ) ;

			if( iHandle != 0 && vEndDate.Type == M4CL_CPP_TYPE_DATE )
			{
				pccItemId = poCMCR->GetItemId( iHandle ) ;

				if( iStart == 1 )
				{
					strcat( acDateFilter, " AND " ) ;
				}
				else
				{
					iStart = 1 ;
				}

				strcat( acDateFilter, pccItemId ) ;
				strcat( acDateFilter, " <= {" ) ;
				iLength = strlen( acDateFilter )  ;
				ClDateToString( vEndDate.Data.DoubleData, acDateFilter + iLength, M4_SECOND_END + 1 ) ;
				strcat( acDateFilter, "}" ) ;
			}

			iHandle = poCMCR->GetNodeEndDateItem( iNode ) ;

			if( iHandle != 0 && vStartDate.Type == M4CL_CPP_TYPE_DATE )
			{
				pccItemId = poCMCR->GetItemId( iHandle ) ;

				if( iStart == 1 )
				{
					strcat( acDateFilter, " AND " ) ;
				}
				else
				{
					iStart = 1 ;
				}

				strcat( acDateFilter, pccItemId ) ;
				strcat( acDateFilter, " >= {" ) ;
				iLength = strlen( acDateFilter )  ;
				ClDateToString( vStartDate.Data.DoubleData, acDateFilter + iLength, M4_SECOND_END + 1 ) ;
				strcat( acDateFilter, "}" ) ;
			}
		}

		if( ( iAutoFilter & M4CL_NODE_AUTO_FILTER_CORRECTION ) != 0 )
		{
			iHandle = poCMCR->GetNodeStartCorDateItem( iNode ) ;

			if( iHandle != 0 && vCorEndDate.Type == M4CL_CPP_TYPE_DATE )
			{
				pccItemId = poCMCR->GetItemId( iHandle ) ;

				if( iStart == 1 )
				{
					strcat( acDateFilter, " AND " ) ;
				}
				else
				{
					iStart = 1 ;
				}

				strcat( acDateFilter, pccItemId ) ;
				strcat( acDateFilter, " <= {" ) ;
				iLength = strlen( acDateFilter )  ;
				ClDateToString( vCorEndDate.Data.DoubleData, acDateFilter + iLength, M4_SECOND_END + 1 ) ;
				strcat( acDateFilter, "}" ) ;
			}

			iHandle = poCMCR->GetNodeEndCorDateItem( iNode ) ;

			if( iHandle != 0 && vCorStartDate.Type == M4CL_CPP_TYPE_DATE )
			{
				pccItemId = poCMCR->GetItemId( iHandle ) ;

				if( iStart == 1 )
				{
					strcat( acDateFilter, " AND " ) ;
				}
				else
				{
					iStart = 1 ;
				}

				strcat( acDateFilter, pccItemId ) ;
				strcat( acDateFilter, " >= {" ) ;
				iLength = strlen( acDateFilter )  ;
				ClDateToString( vCorStartDate.Data.DoubleData, acDateFilter + iLength, M4_SECOND_END + 1 ) ;
				strcat( acDateFilter, "}" ) ;
			}
		}
	}
	else
	{
		iHandle = poCMCR->GetNodeFilterStartDateItem( iNode ) ;

		if( iHandle != 0 )
		{
			iResult = poDRecordSet->Item[ ClHandle( iHandle ) ].Value.Set( vStartDate ) ;

			if( iResult != M4_SUCCESS )
			{
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, M4ObjNodeItemI(poCMCR,iHandle)<<"LookUp" << DumpCallStack() );
				RuntimePushDouble( 0 ) ;
				return( M4_ERROR ) ;
			}
		}

		iHandle = poCMCR->GetNodeFilterEndDateItem( iNode ) ;

		if( iHandle != 0 )
		{
			iResult = poDRecordSet->Item[ ClHandle( iHandle ) ].Value.Set( vEndDate ) ;

			if( iResult != M4_SUCCESS )
			{
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, M4ObjNodeItemI(poCMCR,iHandle)<<"LookUp" << DumpCallStack() );
				RuntimePushDouble( 0 ) ;
				return( M4_ERROR ) ;
			}
		}

		iHandle = poCMCR->GetNodeFilterStartCorDateItem( iNode ) ;

		if( iHandle != 0 )
		{
			iResult = poDRecordSet->Item[ ClHandle( iHandle ) ].Value.Set( vCorStartDate ) ;

			if( iResult != M4_SUCCESS )
			{
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, M4ObjNodeItemI(poCMCR,iHandle)<<"LookUp" << DumpCallStack() );
				RuntimePushDouble( 0 ) ;
				return( M4_ERROR ) ;
			}
		}

		iHandle = poCMCR->GetNodeFilterEndCorDateItem( iNode ) ;

		if( iHandle != 0 )
		{
			iResult = poDRecordSet->Item[ ClHandle( iHandle ) ].Value.Set( vCorEndDate ) ;

			if( iResult != M4_SUCCESS )
			{
				DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_WRITING_ITEM, M4ObjNodeItemI(poCMCR,iHandle)<<"LookUp" << DumpCallStack() );
				RuntimePushDouble( 0 ) ;
				return( M4_ERROR ) ;
			}
		}
	}


// calculamos el filtro en RAM ===========================================================

	//----------------------------
	//Comprobamos que no se cumpla que el canal sea mono-societario y el item sea el Id de la organización.
	m4uint32_t iItemTemp;
	m4uint8_t iInternalType;
	m4double_t dOrganizationType;

	poAccess->GetpChannel()->OrganizationType.Get(dOrganizationType);

	for( j = 0 ; j < iSecondary ; j++ ) {

		abSecondaryValues[ j ] = M4_TRUE; //valor por defecto

		iItemTemp = poCMCR->GetNodeItemHandleById( iNode, apcSecondaryColumns[ j ] ) ;

		if( iItemTemp == 0 )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, apcSecondaryColumns[ j ] <<M4ObjNodeN(poCMCR,iNode)<<"LookUp" << DumpCallStack() );
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}
		iInternalType = poCMCR->GetItemInternalType(iItemTemp);
		if((m4int32_t(dOrganizationType) == M4CL_ORGANIZATION_TYPE_MONO) && (iInternalType == M4CL_INTERNAL_TYPE_ID_ORGANIZATION)) {
			//No añadimos el item y su valor al filtro en RAM que construiremos
			abSecondaryValues[ j ] = M4_FALSE;
		}
	}
	//----------------------------

	iLength = 0 ;

	iStart = 0 ;
	for( j = 0 ; j < iSecondary ; j++ )
	{
		/* Bug 0080948
		Si la cadena viene vacía no hace falta añadir filtro. Van todos
		*/
		if( avSecondaryValues[ j ].Type != M4CL_CPP_TYPE_STRING_POOL || avSecondaryValues[ j ].Data.PointerChar == NULL || avSecondaryValues[ j ].Data.PointerChar[ 0 ] == '\0' )
		{
			continue ;
		}

		if(abSecondaryValues[ j ] == M4_FALSE) continue; //No se añade

		if( iStart == 1 )
		{
			iLength += 5 ;	// " AND "
		}
		else
		{
			iStart = 1 ;
		}

		iLength += strlen( apcSecondaryColumns[ j ] ) * 2 ;
		iLength += strlen( avSecondaryValues[ j ].Data.PointerChar ) * 2 ;
		iLength += 16 ;	// " >= \"" + "\" AND " + " < \"" + "\""
	}


	if( iLength > 0 )
	{
		iLength += 18 ;	// "if " + " then return 1' + '\0'

		pcRAMFilter = new m4char_t[ iLength ] ;

#ifdef _DEBUG
		if( pcRAMFilter == NULL )
		{
			g_oChLog<<BeginError(M4_CH_VM_OUT_OF_MEMORY)<<"tamaño pedido:"<<LogCat<<(m4uint32_t)sizeof( m4char_t ) * iLength <<__FILE__<<__LINE__<<EndLog;
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}
#endif

		*pcRAMFilter = '\0' ;

		strcat( pcRAMFilter, "if " ) ;

		iStart = 0 ;
		for( j = 0 ; j < iSecondary ; j++ )
		{
			if( avSecondaryValues[ j ].Type != M4CL_CPP_TYPE_STRING_POOL || avSecondaryValues[ j ].Data.PointerChar == NULL || avSecondaryValues[ j ].Data.PointerChar[ 0 ] == '\0' )
			{
				continue ;
			}

			if(abSecondaryValues[ j ] == M4_FALSE) continue; //No se añade

			if( iStart == 1 )
			{
				strcat( pcRAMFilter, " AND " ) ;
			}
			else
			{
				iStart = 1 ;
			}

			strcat( pcRAMFilter, apcSecondaryColumns[ j ] ) ;
			strcat( pcRAMFilter, " >= \"" ) ;
			strcat( pcRAMFilter, avSecondaryValues[ j ].Data.PointerChar ) ;
			strcat( pcRAMFilter, "\" AND " ) ;
			strcat( pcRAMFilter, apcSecondaryColumns[ j ] ) ;
			strcat( pcRAMFilter, " < \"" ) ;
			strcat( pcRAMFilter, avSecondaryValues[ j ].Data.PointerChar ) ;

			/* Bug 0080948
			El like se construye haciendo un menor que la palabra que se construye
			incrementando en 1 el último caracter.
			Si ya es el último caracter válido no se incrementa. Asumimos que
			eso no va a pasar nunca.
			El algoritmo bueno sería incrementar el anterior si se puede y sino
			seguir con los anteriores. Si no se puede con ninguno se quitaría esta
			condición, ya que ya cualquier palabra que empieze por esta es mayor.
			*/
			iLength = strlen( avSecondaryValues[ j ].Data.PointerChar ) ;
			c = avSecondaryValues[ j ].Data.PointerChar[ iLength - 1 ] ;

			if( c != 0xFF )
			{
				iLength = strlen( pcRAMFilter ) ;
				pcRAMFilter[ iLength - 1 ] = c + 1 ;
			}

			strcat( pcRAMFilter, "\"" ) ;
		}

		strcat( pcRAMFilter, " then return 1" ) ;
	}



// calculamos el filtro de Primary PKs ==========================================================


	if( iCallApplyFilter == 1 )
	{
		poItem = &( poDRecordSet->Item[ ClHandle( iAppItem ) ] ) ;

		iResult = poItem->Call( NULL, iPrimary + 1 ) ;

		if( iResult != M4_SUCCESS )
		{
			if( pcRAMFilter != NULL )
			{
				delete [] pcRAMFilter ;
				pcRAMFilter = NULL ;
			}
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		iResult = RuntimePop( vValue ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP,"LookUp");
			if( pcRAMFilter != NULL )
			{
				delete [] pcRAMFilter ;
				pcRAMFilter = NULL ;
			}
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		if( vValue.Type != M4CL_CPP_TYPE_STRING_POOL )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(), poItem->ItemDef.GetHandle())<<"LookUp" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );

			if( pcRAMFilter != NULL )
			{
				delete [] pcRAMFilter ;
				pcRAMFilter = NULL ;
			}
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}
		
		pcPKFilter = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
		iPkFilter = vValue.Data.CharData ;
	}
	else
	{
		for( j = 0 ; j < iPrimary ; j++ )
		{
			iResult = RuntimePopAndDelete() ;

			if( iResult != M4_SUCCESS )
			{
				DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP,"Pop Error in LookUp. Primary PK Value: <" << j << ">" );
				return( M4_ERROR ) ;
			}
		}

		pcPKFilter = NULL ;
		iPkFilter = 0 ;
	}


// calculamos el filtro secundario ==========================================================

	if( iSecItem != 0 )
	{
		vValue.Type = M4CL_CPP_TYPE_NUMBER ;
		vValue.Data.DoubleData = iCacheable ;

		iResult = RuntimePush( vValue ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_PUSH,"LookUp. IsCacheable" );
			if( pcRAMFilter != NULL )
			{
				delete [] pcRAMFilter ;
				pcRAMFilter = NULL ;
			}
			m_oDynamicPool->DeleteString( iPkFilter ) ;
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		poSecItem = &( poDSecRecordSet->Item[ ClHandle( iSecItem ) ] ) ;

		iResult = poSecItem->Call( NULL, 1 ) ;

		if( iResult != M4_SUCCESS )
		{
			if( pcRAMFilter != NULL )
			{
				delete [] pcRAMFilter ;
				pcRAMFilter = NULL ;
			}
			m_oDynamicPool->DeleteString( iPkFilter ) ;
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		iResult = RuntimePop( vValue ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP,"LookUp" ) ;
			if( pcRAMFilter != NULL )
			{
				delete [] pcRAMFilter ;
				pcRAMFilter = NULL ;
			}
			m_oDynamicPool->DeleteString( iPkFilter ) ;
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		if( vValue.Type != M4CL_CPP_TYPE_STRING_POOL )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_TYPE_MISMATCH, M4ObjNodeItemI(poSecItem->GetpAccess()->GetpCMCR(), poSecItem->ItemDef.GetHandle())<<"LookUp" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
			if( pcRAMFilter != NULL )
			{
				delete [] pcRAMFilter ;
				pcRAMFilter = NULL ;
			}
			m_oDynamicPool->DeleteString( iPkFilter ) ;
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		pccSecFilter = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
		m_oDynamicPool->DeleteString( vValue.Data.CharData ) ; //sabemos que no pasa nada
	}
	else
	{
		pccSecFilter = NULL ;
	}

// ya podemos borrar el otro
	if( iCallApplyFilter == 1 )
	{
		m_oDynamicPool->DeleteString( iPkFilter ) ;
	}


// si es cacheable componemos el filtro ==========================================================

	if( iCacheable == M4CL_CACHEABLE_TRUE )
	{
		iDateLength = strlen( acDateFilter ) ;

		if( pcPKFilter != NULL && *pcPKFilter != '\0' )
		{
			iPKFilterLength = strlen( pcPKFilter ) ;
		}
		else
		{
			iPKFilterLength = 0 ;
		}

		if( pccSecFilter != NULL && *pccSecFilter != '\0' )
		{
			iSecFilterLength = strlen( pccSecFilter ) ;
		}
		else
		{
			iSecFilterLength = 0 ;
		}

		iLength = iDateLength + iPKFilterLength + iSecFilterLength ;

		if( iLength > 0 )
		{
			iLength += 33 ;	// 3 + 14 + 1 + 5 * 3       "if " + " then return 1' + '\0' + " AND " * 3

			pcSQLFilter = new m4char_t[ iLength ] ;

#ifdef _DEBUG
			if( pcSQLFilter == NULL )
			{
				g_oChLog<<BeginError(M4_CH_VM_OUT_OF_MEMORY)<<"tamaño pedido:"<<LogCat<<(m4uint32_t)sizeof( m4char_t ) * iLength <<__FILE__<<__LINE__<<EndLog;
				if( pcRAMFilter != NULL )
				{
					delete [] pcRAMFilter ;
					pcRAMFilter = NULL ;
				}
				RuntimePushDouble( 0 ) ;
				return( M4_ERROR ) ;
			}
#endif

			iStart = 0 ;
			*pcSQLFilter = '\0' ;

			strcat( pcSQLFilter, "if " ) ;

			if( iDateLength != 0 )
			{
				strcat( pcSQLFilter, acDateFilter) ;
				iStart = 1 ;
			}

			if( iPKFilterLength != 0 )
			{
				if( iStart == 1 )
				{
					strcat( pcSQLFilter, " AND " ) ;
				}
				else
				{
					iStart = 1 ;
				}
				strcat( pcSQLFilter, pcPKFilter ) ;
			}

			if( iSecFilterLength != 0 )
			{
				if( iStart == 1 )
				{
					strcat( pcSQLFilter, " AND " ) ;
				}

				strcat( pcSQLFilter, pccSecFilter ) ;
			}

			strcat( pcSQLFilter, " then return 1" ) ;
		}
	}


// Invocamos la carga ========================================================================

	if( iCacheable == M4CL_CACHEABLE_TRUE )
	{
		iLength = m4uint16_t( poDRecordSet->Current.Count() ) ;

		if( iLength == 0 )
		{
			iResult = poAccess->Load() ;

			if( iResult != M4_SUCCESS )
			{
				if( pcRAMFilter != NULL )
				{
					delete [] pcRAMFilter ;
					pcRAMFilter = NULL ;
				}
				if( pcSQLFilter != NULL )
				{
					delete [] pcSQLFilter ;
					pcSQLFilter = NULL ;
				}
				RuntimePushDouble( 0 ) ;
				return( M4_ERROR ) ;
			}
		}
	}
	else
	{
		iItem = poCMCR->GetNodeItemHandleById( iNode, M4CL_ITEM_LOAD_PRG ) ;

		if( iItem == 0 )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, M4CL_ITEM_LOAD_PRG<<M4ObjNodeN(poCMCR,iNode)<<"LookUp" << DumpCallStack() );

			if( pcRAMFilter != NULL )
			{
				delete [] pcRAMFilter ;
				pcRAMFilter = NULL ;
			}
			if( pcSQLFilter != NULL )
			{
				delete [] pcSQLFilter ;
				pcSQLFilter = NULL ;
			}
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		poItem = &( poDRecordSet->Item[ ClHandle( iItem ) ] ) ;
		
	
		iResult = poItem->Call() ;
		
		if( iResult != M4_SUCCESS )
		{
			if( pcRAMFilter != NULL )
			{
				delete [] pcRAMFilter ;
				pcRAMFilter = NULL ;
			}
			if( pcSQLFilter != NULL )
			{
				delete [] pcSQLFilter ;
				pcSQLFilter = NULL ;
			}
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		iResult = RuntimePop( vValue ) ;

		if( iResult != M4_SUCCESS || vValue.Data.DoubleData == M4_ERROR )
		{
			DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP, "LookUp" ) ;
			if( pcRAMFilter != NULL )
			{
				delete [] pcRAMFilter ;
				pcRAMFilter = NULL ;
			}
			if( pcSQLFilter != NULL )
			{
				delete [] pcSQLFilter ;
				pcSQLFilter = NULL ;
			}
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}
	}


// aplicamos el filtro en RAM ===================================================================


	if( pcSQLFilter != NULL )
	{
		iResult = poDRecordSet->Current.Filter.AddNew( pcSQLFilter, ClFilterInterface::StaticFilter, 0, NULL, NULL, M4_LOOKUP_SQL_FILTER ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_DEBUGF(M4_CH_VM_ADDING_RAM_FILTER, pcSQLFilter<<"LookUp" << DumpCallStack() );
			delete [] pcSQLFilter ;

			if( pcRAMFilter != NULL )
			{
				delete [] pcRAMFilter ;
				pcRAMFilter = NULL ;
			}
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		//trabajamos tanto en current como en register

		iResult = poDRecordSet->Register.Filter.AddNew( pcSQLFilter, ClFilterInterface::StaticFilter, 0, NULL, NULL, M4_LOOKUP_SQL_FILTER ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_DEBUGF(M4_CH_VM_ADDING_RAM_FILTER, pcSQLFilter<<"LookUp" << DumpCallStack() );
			delete [] pcSQLFilter ;

			if( pcRAMFilter != NULL )
			{
				delete [] pcRAMFilter ;
				pcRAMFilter = NULL ;
			}
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		delete [] pcSQLFilter ;
	}


	if( pcRAMFilter != NULL )
	{
		iResult = poDRecordSet->Current.Filter.AddNew( pcRAMFilter, ClFilterInterface::StaticFilter, 0, NULL, NULL, M4_LOOKUP_RAM_FILTER ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_DEBUGF(M4_CH_VM_ADDING_RAM_FILTER, pcRAMFilter<<"LookUp" << DumpCallStack() );
			delete [] pcRAMFilter ;
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		//trabajamos tanto en current como en register

		iResult = poDRecordSet->Register.Filter.AddNew( pcRAMFilter, ClFilterInterface::StaticFilter, 0, NULL, NULL, M4_LOOKUP_RAM_FILTER ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_DEBUGF(M4_CH_VM_ADDING_RAM_FILTER, pcRAMFilter<<"LookUp" << DumpCallStack() );
			delete [] pcRAMFilter ;
			RuntimePushDouble( 0 ) ;
			return( M4_ERROR ) ;
		}

		delete [] pcRAMFilter ;
	}


// contamos los que hay ==============================================================================

    iLength = m4uint16_t( poDRecordSet->Current.Count() ) ;

	if( iLength > 0 )
	{
		poDRecordSet->Current.MoveTo( 0 ) ;
	}
 
	RuntimePushDouble( iLength ) ;
    return( M4_SUCCESS ) ;

}


//------------------------------------------
//ERRORES
//------------------------------------------

// Funcion:		Devuelve el mensaje de error formateado con los argumentos pasados.
//				Se pregunta a la M4LogSys, y si no está se pregunta al Canal de Errores
// LN4 function:	ErrorString = GetErrorString( module, submodule, code [, var_list_arguments] )
m4return_t ClVM1BasicLib::GetErrorString (void){

	m4return_t			ret=M4_SUCCESS;

	m4VariantType		vCode, vSubModule, vModule, vNum;
	m4bool_t			bFormated ;
	m4uint32_t			iNumArgs,i;
	//m4char_t			pszParsedString[M4CL_MAX_STRING_ERROR];
	//m4uint32_t		uiParsedLong=M4CL_MAX_STRING_ERROR;	//Long.	 de la Cadena de Error formateada
	m4char_t			*pszParsedString;
	m4uint32_t			uiParsedLong;	//Long.	 de la Cadena de Error formateada
	m4bool_t			bModified = M4_FALSE;
	m4pchar_t			pcOut = NULL;
	m4pchar_t			pcOrganization = NULL;
	m4VariantType		vOrganization;
	ClChannel			*poChannel = NULL;

	
	//sacamos el no. de argumentos
	ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetErrorString.")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;

	iNumArgs-=3;
	//sacamos los argumentos que nos quedan por sacar y los metemos en un array
	m4VariantType *vaParam= new m4VariantType	[iNumArgs];
	for (i=0; i<iNumArgs; ++i){
		RuntimePop( vaParam[iNumArgs-i-1] );
		//los convertimos  a string para llamar a M4LogSys
		if (vaParam[iNumArgs-i-1].Type == M4CL_CPP_TYPE_STRING_POOL)
			m_oVariantMediator->ToCharString(vaParam[iNumArgs-i-1]);
	}

	//sacamos codigo
    ret=RuntimePopNumber (vCode);
	M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "code" << "GetErrorString" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vCode.Type)<<DumpCallStack() );
	m4uint32_t iCode= (m4uint32_t) vCode;
	if (iCode>0xFFFF) {
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Code"<<"GetErrorString"<<iCode << DumpCallStack() );
		iCode=0xFFFF;
	}

	//sacamos submodulo
    ret=RuntimePopNumber (vSubModule);
	M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "submodule" << "GetErrorString" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vSubModule.Type)<< DumpCallStack() );
	m4uint32_t iSubModule= (m4uint32_t) vSubModule;
	if (iSubModule>0xFF) {
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Submodule"<<"GetErrorString"<<iSubModule << DumpCallStack() );
		iSubModule=0xFF;
	}
	
	//sacamos modulo
    ret=RuntimePopNumber (vModule);
	M4CHECK_RES(ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Module" << "GetErrorString" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vModule.Type)<<  DumpCallStack() );
	m4uint32_t iModule= (m4uint32_t) vModule;
	if (iModule>0xFF) {
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Module"<<"GetErrorString"<<iModule << DumpCallStack() );
		iModule=0xFF;
	}

	//creamos codigo de error
    m4uint32_t iCodErr = ( (iModule&0xFF) << 24) | ( (iSubModule&0xFF) <<16) | (iCode&0xFFFF);

	// jcr Vemos si viene como parámetros sin separar
	if(
			iNumArgs == 1
		&&	vaParam[ 0 ].Type == M4CL_CPP_TYPE_STRING_VAR
		&&	vaParam[ 0 ].Data.PointerChar != NULL
		&&	strlen( vaParam[ 0 ].Data.PointerChar ) > 5
		&&	( memcmp( vaParam[ 0 ].Data.PointerChar, "#*S1*#", 6 ) == 0 || memcmp( vaParam[ 0 ].Data.PointerChar, "#*s1*#", 6 ) == 0 )
	  )
	{
		bFormated = M4_TRUE ;
	}
	else
	{
		bFormated = M4_FALSE ;
	}

	poChannel = m_oState1.m_poAccess->GetpChannel();

	//Lenguaje en el que se construyó el canal
	m4double_t dLang;
	poChannel->Data_Language.Get (dLang);
	m4language_t uiLang = (m4language_t) dLang;

	/* Bug 0069089
	Hay que pasar un lenguaje válido
	*/
	M4ClLangInfo::Instance()->CheckAndSetValidLangId( uiLang ) ;

	//Llamamos a la M4LogSys, para calcular el buffer necesario
	if( bFormated == M4_TRUE )
	{
		ret=M4GetErrorString(iCodErr, vaParam[ 0 ].Data.PointerChar, 1, poChannel->GetpChannelManager(), NULL, uiParsedLong,uiLang);
	}
	else
	{
		ret=M4GetErrorStringByParams(iCodErr, vaParam, iNumArgs, 1, poChannel->GetpChannelManager(), NULL, uiParsedLong,uiLang);
	}

	if (ret!=M4_SUCCESS) {
		//Error No encontrado
		DUMP_CHLOG_ERROR(M4_CH_VM_FORMATTING_ERROR_STRING);
		ret=M4_ERROR;
	} else {
		pszParsedString=new m4char_t[uiParsedLong];
		if (!pszParsedString) {
			ret=M4_ERROR;
		}
		//Buscamos en la M4LogSys o en el Canal de Errores
		if (ret==M4_SUCCESS) {

			if( bFormated == M4_TRUE )
			{
				ret=M4GetErrorString(iCodErr, vaParam[ 0 ].Data.PointerChar, 1, poChannel->GetpChannelManager(), pszParsedString, uiParsedLong,uiLang);
			}
			else
			{
				ret=M4GetErrorStringByParams(iCodErr, vaParam, iNumArgs, 1, poChannel->GetpChannelManager(), pszParsedString, uiParsedLong,uiLang);
			}
		}
		if (ret!=M4_SUCCESS)
		{
			//Error No encontrado
			DUMP_CHLOG_ERROR(M4_CH_VM_FORMATTING_ERROR_STRING);
			ret=M4_ERROR;
		}
		else
		{
			//Error Encontrado => Devolvemos la cadena de Error

			/* Bug 0276910
			Se hace el replace literal
			*/
			if( poChannel->Organization.Get( vOrganization ) == M4_SUCCESS && vOrganization.Type == M4CL_CPP_TYPE_STRING_VAR )
			{
				pcOrganization = vOrganization.Data.PointerChar ;
			}

			ret = M4ReplaceLiterals( poChannel->GetpChannelManager(), pszParsedString, pcOut, pcOrganization, bModified ) ;

			if( ret == M4_SUCCESS && bModified == M4_TRUE && pcOut != NULL )
			{
				RuntimePushString( pcOut ) ;
				delete pcOut;
			}
			else
			{
				RuntimePushString( pszParsedString ) ;
			}
		}

		if (pszParsedString)
		{
			delete [] pszParsedString;
		}
	}
	
	delete [] vaParam;

	return ret;
}

/*
m4return_t ClVM1BasicLib::ParseErrorString (m4char_t *ai_pszErrorString, m4VariantType *ai_vParam, m4uint32_t ai_iNumParam, m4char_t *ao_pszOutString, m4uint32_t ai_iMaxOutputSize){

	m4uint32_t	iStart=0;
	m4char_t	*psTemp=ai_pszErrorString;
	m4uint32_t  iLenParam, iNumParam;
	char		cType;
	m4return_t	ret=M4_SUCCESS;
	if (psTemp==0) {
		return M4_ERROR;
	}
	while ( *psTemp && iStart<ai_iMaxOutputSize ){
		
		while (*psTemp!='%' && *psTemp && iStart<ai_iMaxOutputSize ){ //buscamos un *
			ao_pszOutString[iStart]=*psTemp;
			++psTemp;
			++iStart;
		}
		
		if (*psTemp && iStart<ai_iMaxOutputSize ){ //hemos encontrado un %
			++psTemp;
			//leemos el número de argumento a sustituir
			iNumParam=atoi(psTemp);
			//leemos el tipo
			while (*psTemp != ':' && *psTemp) ++psTemp;
			++psTemp;
			if (*psTemp){
				while (isspace(*psTemp)) ++psTemp;
				cType=*psTemp;
			}
			++psTemp;

// jcr. Empezamos en 0
//			if (iNumParam!=0){
//				--iNumParam;
//
			
			//miramos si es válido el número de arg
			if (iNumParam < ai_iNumParam){
				//convertimos al tipo
				switch (cType){
				case 's':
					break;
				case 'd':
					ret=m_oVariantMediator->ToDate(ai_vParam[iNumParam]);
					break;
				case 'n':
					ret=m_oVariantMediator->ToDouble(ai_vParam[iNumParam]);
					break;
				default: //no copiamos nada
					break;
				}
				//copiamos a la cadena destino
				if (ret==M4_SUCCESS){
					ret=m_oVariantMediator->ToCharString (ai_vParam[iNumParam]);
					if (ret== M4_SUCCESS) { //si va bien lo metemos, sino no metemos nada
						iLenParam=strlen (ai_vParam[iNumParam].Data.PointerChar );
						if (iStart+iLenParam < ai_iMaxOutputSize){
							strcpy (&ao_pszOutString[iStart], ai_vParam[iNumParam].Data.PointerChar );
							iStart+=iLenParam;
						}
					}
				}
				
			}
//			}
			
		}
	}
	ao_pszOutString[iStart]=0;
	return M4_SUCCESS;
	//añadimos el 0 al final de la cadena}
}
*/


//---Autoload's


m4return_t  ClVM1BasicLib::GetAutoloadMode     (void){
	m4VariantType       vTemp;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetAutoloadMode");
#endif
	m_oState1.m_poAccess->GetpChannel()->WantsAutoLoad.Get(vTemp);
	
    RuntimePush (vTemp);
    return M4_SUCCESS;
}

m4return_t  ClVM1BasicLib::SetAutoloadMode (void){
	m4VariantType       vTemp;

#if _M4_DEBUG > 10  
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SetAutoloadMode");
#endif

    m4return_t ret=RuntimePop(vTemp);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetAutoloadMode.")	;
	
    m_oState1.m_poAccess->GetpChannel()->WantsAutoLoad.Set(vTemp);
	
    RuntimePushDouble (M4_SUCCESS);
    return M4_SUCCESS;
}

//--------------------
//autoload de nodo
//--------------------
m4return_t  ClVM1BasicLib::GetNodeAutoloadMode     (void){
	m4VariantType       vTemp;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetNodeAutoloadMode");
#endif

    //EDU------>>
    //m_oState1.m_poRecordSet->GetpNode_Channel()->WantsAutoLoad.Get(vTemp);
    //ahora está en el access:
    ClNode *pn = m_oState1.m_poAccess->Node.Get (m_oState1.m_poRecordSet->GetpNode_ExtInfo ()->Index);
    M4_ASSERT (pn);
    pn->WantsAutoLoad.Get(vTemp);
    //EDU------<<
	
    RuntimePush (vTemp);
    return M4_SUCCESS;
}

m4return_t  ClVM1BasicLib::SetNodeAutoloadMode (void){
	m4VariantType       vTemp;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SetNodeAutoloadMode");
#endif
    m4return_t ret=RuntimePop(vTemp);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetNodeAutoloadMode.")	;
	
    //EDU------>>
    //m_oState1.m_poRecordSet->GetpNode_Channel()->WantsAutoLoad.Set(vTemp);
	//ahora está en el access:
    ClNode *pn = m_oState1.m_poAccess->Node.Get (m_oState1.m_poRecordSet->GetpNode_ExtInfo ()->Index);
    M4_ASSERT (pn);
    pn->WantsAutoLoad.Set(vTemp);
    //EDU------<<

	
    RuntimePushDouble (M4_SUCCESS);
    return M4_SUCCESS;
}

m4return_t ClVM1BasicLib::sttSetLabel    (void){
	m4VariantType vLabel;
	m4pchar_t pcLabel;

	m4return_t ret = M4_SUCCESS;

    ret = RuntimePop (vLabel);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error getting value from RTS.")	;

    if (vLabel.Type!=M4CL_CPP_TYPE_STRING_POOL){
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Label"<<"sttSetLabel" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vLabel.Type)<<  DumpCallStack() );
        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }


	pcLabel = m_oDynamicPool->GetString(vLabel.Data.CharData); 

	m_oState1.m_poAccess->GetpChannel()->SetStatisticsLabel(pcLabel);

	if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
		ret = GetVMCSContext()->GetSttWrap()->SetStatisticsLabel(pcLabel);
	else 
		ret = M4_ERROR;
	
	//en vm
	m_poVM->SetStatusLabel(pcLabel);
    m_oDynamicPool->DeleteString(vLabel.Data.CharData);
	RuntimePushDouble(ret);

    return M4_SUCCESS;
}

m4return_t ClVM1BasicLib::sttGetLevel    (void){
	m4double_t dLevel = GetVMCSContext()->GetSttWrap()->GetStatisticsLevel();
	RuntimePushDouble(dLevel);
	return M4_SUCCESS;
}

m4return_t ClVM1BasicLib::sttSetLevel    (void){
	m4VariantType vLevel;

	m4return_t ret = M4_SUCCESS;

    ret = RuntimePop (vLevel);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "vLevel.sttSetLevel.")	;

	// 1/10/98
	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "sttSetLevel"<<"" << DumpCallStack() );
	RuntimePushDouble(M4_ERROR);
	return M4_SUCCESS;

	if (vLevel.Type!=M4CL_CPP_TYPE_NUMBER){
        ret=m_oVariantMediator->ToDouble (vLevel);  
    }
    if (ret!=M4_SUCCESS){ 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "SttLevel"<<"sttSetLevel" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vLevel.Type)<<  DumpCallStack() );
        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

	if (M4_VM_STATISTICS_DISABLED == (m4uint8_t)vLevel.Data.DoubleData)
	{
		//No hacemos nada. No permitimos el paso a DISABLED desde otro estado por LN4.
		//Warning
		ret = M4_ERROR;
	}
	else
	{
		if (M4_VM_STATISTICS_DISABLED == m_oState1.m_poAccess->GetpChannel()->GetStatisticsLevel())
		{
			//No hacemos nada. No permitimos el paso de DISABLED a otro estado por LN4.
			//Warning
			ret = M4_ERROR;
		}
		else
		{
			//Cambiamos las Stt al canal
			m_oState1.m_poAccess->GetpChannel()->SetStatisticsLevel((m4uint16_t)vLevel.Data.DoubleData);
		}

		if (M4_VM_STATISTICS_DISABLED == GetVMCSContext()->GetSttWrap()->GetStatisticsLevel())
		{
			//No hacemos nada. No permitimos el paso de DISABLED a otro estado por LN4.
			//Warning
			ret = M4_ERROR;
		}
		else
		{
			//Cambiamos las Stt al ejecutor
			ret = GetVMCSContext()->GetSttWrap()->SetStatisticsLevel ((m4uint16_t)vLevel.Data.DoubleData, m_oState1.m_poAccess->GetpChannel()->GetpChannelManager());
		}
	}

	RuntimePushDouble(ret);

	return M4_SUCCESS;
}

m4return_t ClVM1BasicLib::sttPersistToFile    (void){
	m4VariantType vFileName;
	m4pchar_t pcFileName;

	m4return_t ret = M4_SUCCESS;

    ret = RuntimePop (vFileName);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error getting value from RTS.")	;

    if (vFileName.Type!=M4CL_CPP_TYPE_STRING_POOL){
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "FileName"<<"sttPersistToFile" <<M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vFileName.Type)<<  DumpCallStack() );
        RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

	pcFileName = m_oDynamicPool->GetString(vFileName.Data.CharData); 

	if (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		if (GetVMCSContext()->GetSttWrap()->GetStatistics())
		{
			ret = GetVMCSContext()->GetSttWrap()->GetStatistics()->sttPersistToFile(pcFileName);
		}
	}
	else
	{
		ret = M4_ERROR;
	}
	
    m_oDynamicPool->DeleteString(vFileName.Data.CharData);
	RuntimePushDouble(ret);

    return M4_SUCCESS;
}


//
//ReplaceLiterals
//
m4return_t  ClVM1BasicLib::ReplaceLiterals(void)
{

    m4return_t      iResult = M4_ERROR ;
	m4bool_t		bModified = M4_FALSE ;
	m4uint32_t		iOrganization = 0 ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iArgNumber = 0 ;
	m4pchar_t		pcOut = NULL ;
	m4pchar_t		pcString = NULL ;
	m4pchar_t		pcOrganization = NULL ;
	ClChannel		*poChannel = NULL ;
    m4VariantType   vArgNumber ;
    m4VariantType   vString ;
	m4VariantType	vOrganization ;
	
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: ReplaceLiterals");
#endif

	iResult = RuntimePopDouble( vArgNumber ) ;
    DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ReplaceLiterals." ) ;

    iArgNumber = (m4int32_t)vArgNumber.Data.DoubleData ;

    if( iArgNumber > 2 )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_TOO_PARAM, "ReplaceLiterals" << 2 << iArgNumber << DumpCallStack() ) ;

		// Sacamos los que sobran
		for( i = 0 ; i < iArgNumber - 2 ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
			DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ReplaceLiterals" ) ;
		}
		iArgNumber = 2 ;
    }

	poChannel = m_oState1.m_poAccess->GetpChannel() ;

	if( iArgNumber == 2 )
	{
	    iResult = RuntimePop( vOrganization ) ;
		DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ReplaceLiterals" ) ;

		if( vOrganization.Type == M4CL_CPP_TYPE_STRING_POOL )
		{
			iOrganization = vOrganization.Data.CharData ;
			pcOrganization = m_oDynamicPool->GetString( iOrganization ) ;
		}
	}


	iResult = RuntimeTop( vString ) ;
    DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ReplaceLiterals." ) ;

    // Si no es cadena o no se convierte, se deja lo que había
	if( vString.Type == M4CL_CPP_TYPE_STRING_POOL )
	{
		pcString = m_oDynamicPool->GetString( vString.Data.CharData ) ;

		if( pcString != NULL && *pcString != '\0' )
		{
			if( pcOrganization == NULL || *pcOrganization == '\0' )
			{
				// Si no viene organización se toma la del canal
				if( poChannel->Organization.Get( vOrganization ) == M4_SUCCESS &&  vOrganization.Type == M4CL_CPP_TYPE_STRING_VAR )
				{
					pcOrganization = vOrganization.Data.PointerChar ;
				}
			}

			iResult = M4ReplaceLiterals( poChannel->GetpChannelManager(), pcString, pcOut, pcOrganization, bModified ) ;

			if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcOut != NULL )
			{
				// Si se cambia se quita el valor viejo y se pone el nuevo
				RuntimePopAndDelete() ;
				RuntimePushString( pcOut, vString.IsUser ) ;
				delete pcOut;
			}
		}
	}

	if( iOrganization != 0 )
	{
		m_oDynamicPool->DeleteString( iOrganization ) ;
	}

    return M4_SUCCESS;
}


//
//CleanLiterals
//
m4return_t  ClVM1BasicLib::CleanLiterals(void)
{

    m4return_t      iResult = M4_ERROR ;
	m4uint32_t		iOrganization = 0 ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iArgNumber = 0 ;
	m4pchar_t		pcOrganization = NULL ;
	ClChannel		*poChannel = NULL ;
    m4VariantType   vArgNumber ;
	m4VariantType	vOrganization ;
	
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: CleanLiterals");
#endif

	iResult = RuntimePopDouble( vArgNumber ) ;
    DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "CleanLiterals." ) ;

    iArgNumber = (m4int32_t)vArgNumber.Data.DoubleData ;

    if( iArgNumber > 1 )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_TOO_PARAM, "CleanLiterals" << 1 << iArgNumber << DumpCallStack() ) ;

		// Sacamos los que sobran
		for( i = 0 ; i < iArgNumber - 1 ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
			DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "CleanLiterals" ) ;
		}
		iArgNumber = 1 ;
    }

	poChannel = m_oState1.m_poAccess->GetpChannel() ;

	if( iArgNumber == 1 )
	{
	    iResult = RuntimePop( vOrganization ) ;
		DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "CleanLiterals" ) ;

		if( vOrganization.Type == M4CL_CPP_TYPE_STRING_POOL )
		{
			iOrganization = vOrganization.Data.CharData ;
			pcOrganization = m_oDynamicPool->GetString( iOrganization ) ;
		}

		if( pcOrganization == NULL || *pcOrganization == '\0' )
		{
			// Si no viene organización se toma la del canal
			if( poChannel->Organization.Get( vOrganization ) == M4_SUCCESS && vOrganization.Type == M4CL_CPP_TYPE_STRING_VAR )
			{
				pcOrganization = vOrganization.Data.PointerChar ;
			}
		}

		iResult = M4CleanLiterals( poChannel->GetpChannelManager(), pcOrganization ) ;

		if( iOrganization != 0 )
		{
			m_oDynamicPool->DeleteString( iOrganization ) ;
		}
	}
	else
	{
		// Bug 0284962 Si no viene organización se borran todas
		iResult = M4CleanAllLiterals() ;
	}

	RuntimePushDouble( iResult ) ;
    return M4_SUCCESS;
}



//
// Import xml data
//
m4return_t	ClVM1BasicLib::LoadXML(void)
{
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF( M4_CH_VM_TRACE_VM_L1, "Trace VM1: LoadXML" ) ;
#endif

	return( _loadExportXml( M4_TRUE, "LoadXML" ) ) ;
}


//
// Export xml data
//
m4return_t	ClVM1BasicLib::ExportXML(void)
{
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF( M4_CH_VM_TRACE_VM_L1, "Trace VM1: ExportXML" ) ;
#endif

	return( _loadExportXml( M4_FALSE, "ExportXML" ) ) ;
}


m4return_t ClVM1BasicLib::_loadExportXml( m4bool_t ai_bLoad, m4pcchar_t ai_pccMethodName )
{

	m4return_t			iResult = M4_ERROR ;
	m4uint32_t			i = 0 ;
	m4uint32_t			iArgumentNumber = 0 ;
	m4uint32_t			hMyNode = 0 ;
	size_t				iLength = 0 ;
	m4pcchar_t			pccValue = NULL ;
	m4pcchar_t			pccThisChannel = NULL ;
	m4pcchar_t			pccHostInstance = NULL ;
	m4char_t			acM4Obj[ M4CL_MAX_LEN_CHANNEL + 1 ] ;
	m4VariantType		vValue ;
	m4VariantType		*pvArguments = NULL ;
	ClAccess			*poAccess = NULL ;
	ClCompiledMCR		*poThisCMCR = NULL ;
	M4ClExternObject	*poExternal = NULL ;
	ClVMRunContext		oContext ;


	iResult = RuntimePopDouble( vValue ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, ai_pccMethodName ) ;

	iArgumentNumber = (m4int32_t)vValue.Data.DoubleData - 1 ;

	// Arguemtos de la función, menos el primero
	pvArguments = new m4VariantType[ iArgumentNumber ] ;
	CHECK_CHLOG_ERRORF( pvArguments == NULL, M4_ERROR, M4_CH_VM_OUT_OF_MEMORY, m4uint32_t( sizeof( m4VariantType ) * ( iArgumentNumber ) ) << __FILE__ << __LINE__ ) ;

	for( i = 0 ; i < iArgumentNumber ; i++ )
	{
		iResult = RuntimePop( vValue ) ;
		DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, ai_pccMethodName ) ;

		if( vValue.Type == M4CL_CPP_TYPE_STRING_POOL )
		{
			pvArguments[ iArgumentNumber - i - 1 ].SetString( m_oDynamicPool->GetString( vValue.Data.CharData ) ) ;
			DeleteVarString( vValue ) ;
		}
		else
		{
			pvArguments[ iArgumentNumber - i - 1 ] = vValue ;
		}
	}

	// Primer argumento, el Meta4Object
	iResult = RuntimePop( vValue ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, ai_pccMethodName ) ;

	if( vValue.Type != M4CL_CPP_TYPE_STRING_POOL )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "M4Obj ID" << ai_pccMethodName << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
		delete [] pvArguments ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	pccValue = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
	iLength = strlen( pccValue ) ;
	if( iLength > M4CL_MAX_LEN_CHANNEL )
	{
		iLength = M4CL_MAX_LEN_CHANNEL ;
	}
	memcpy( acM4Obj, pccValue, iLength ) ;
	acM4Obj[ iLength ] = '\0' ;
	DeleteVarString( vValue ) ;


	// Se consigue el acceso al canal. De momento el acceso es al propio
	poAccess = m_oState1.m_poAccess ;

	poThisCMCR = poAccess->GetpCMCR() ;
	pccThisChannel = poThisCMCR->GetChannelId() ;

	if( *acM4Obj != '\0' && strcmpi( pccThisChannel, acM4Obj ) != 0 )
	{
		hMyNode = poThisCMCR->GetItemNodeHandle( m_oState1.m_hItem ) ;

		pccHostInstance = poThisCMCR->GetNodeT3AliasInstanceIdByAlias( hMyNode, acM4Obj ) ;
		
		if( pccHostInstance == NULL )
		{
	    	DUMP_CHLOG_ERRORF( M4_CH_VM_NO_SUCH_INSTANCE, acM4Obj<< poThisCMCR->iNodeN( hMyNode) << LogCatAnd << poThisCMCR->nNodeN( hMyNode ) << ai_pccMethodName << DumpCallStack() ) ;
			delete [] pvArguments ;
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_SUCCESS ) ;
		}

		iResult = m_poVM->GetL2Access( poAccess->GetpChannel(), pccHostInstance, M4CL_HAS_STATE_EXE_JIT_FLAG( m_oState1.m_Flags ), &poAccess ) ;
		
		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_NO_GET_ACCESS_L2, pccHostInstance << ai_pccMethodName << DumpCallStack() ) ;
			delete [] pvArguments ;
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_SUCCESS ) ;
		}
	}

	oContext.m_pVM = m_poVM ;
	oContext.m_pAccess = poAccess ;

	poExternal = GetVMCSContext()->GetObjContext()->GetObject( "m4dmsxml", M4CL_INTERN_OBJECT_DLL ) ;

	if( poExternal == NULL )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	if( ai_bLoad == M4_TRUE )
	{
		iResult = poExternal->Execute( "LoadXMLDataExtended", pvArguments, iArgumentNumber, vValue, &oContext ) ;
	}
	else
	{
	    iResult = poExternal->Execute( "ExportXMLDataExtended", pvArguments, iArgumentNumber, vValue, &oContext ) ;
	}

	delete [] pvArguments ;

	if( iResult != M4_SUCCESS )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	RuntimePush( vValue ) ;
    return( M4_SUCCESS ) ;
}
