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
//    Virtual machine de nivel 1- Funciones relativas a la compilacion y JIT
////
//==============================================================================

#include "vm1compjit.hpp"

#include "vmachin1.h"
#include "vmres.hpp"
#include "finder.hpp"
#include "trad.h"
#include "intln4.hpp"
#include "tablfunc.h"
#include "tablafuncdesc.hpp"
#include "tablatt.hpp"


#include "m4log.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "cltnform.hpp"
#include "m4mdrt.hpp"
#include "clstringpool.hpp"
#include "spoolstr.h"
#include "cldbgutil.hpp"
#include "clcodejit.hpp"
#include "m4objglb.hpp"

#include "cllgadap.hpp"
#include "m4lang.hpp"
#include "nodedef.hpp"


#define M4_VM_STATIC_POOL_SIZE_FOR_JIT						0x2



ClVM1CompilerAndJIT::ClVM1CompilerAndJIT (const m4VMState_t &ai_oState):	m_oState1(ai_oState)
{
	m_poTrad=0;
	m_IsFreeTrad=M4_TRUE;
	m_poInterpreter=0;
	//la tabla de funciones (ahora es Comun a todas las instancias)
	m_poFunctionTable = ClFuncTable::GetInstance();
	
	m_oUpperPool	= 0;
	m_oNoUpperPool	= 0;

	m_poFinderJIT	= 0;
	m_poFinderTICH	= 0;
	
}

ClVM1CompilerAndJIT::~ClVM1CompilerAndJIT ()
{
	
	if (m_poTrad!=NULL){
		delete m_poTrad;
		m_poTrad=NULL;
	}
	if (m_poInterpreter!=NULL){
		delete m_poInterpreter;
		m_poInterpreter=NULL;
	}

	if (m_oUpperPool!=NULL){
		delete m_oUpperPool;
        m_oUpperPool=NULL;
    }
	
	if (m_oNoUpperPool!=NULL){
		delete m_oNoUpperPool;
        m_oNoUpperPool=NULL;
    }

	if (m_poFinderJIT)
	{
		delete m_poFinderJIT;
		m_poFinderJIT=NULL;
	}
	if (m_poFinderTICH)
	{
		delete m_poFinderTICH;
		m_poFinderTICH=NULL;
	}

}

//----------------------------------------------------------------------
// Funciones para C O M P I L A R
//----------------------------------------------------------------------

m4return_t ClVM1CompilerAndJIT::clcCompile		(void){
	
        
    m4VariantType vTempCode;
	m4return_t	ret;
    ClAccessRecordSet     *poCurrentRecordSet;
	m4uint32_t hRules=0,hSourceCode=0,hTempCode=0;

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1," clcCompile.");
#endif

	ClTrad *pTrad=0;
    ClCompiledMCR *poCMCR=0;

	poCMCR = m_oState1.m_poAccess->GetpCMCR();

	if (InitTrad (M4CL_FINDER_TICH,pTrad)!=M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) );
        RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}


	M4_ASSERT (pTrad);

	//Comprobamos que existe el nodo de Rules. No hace falta que estemos posicionados en el !!!.
	hRules = poCMCR->GetChannelNodeHandleById(M4_NODE_SCHTIS_RULES);
	if (hRules == 0)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, M4_NODE_SCHTIS_RULES<<"clcCompile" << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
	}

    poCurrentRecordSet=&(m_oState1.m_poAccess->Node[(ClHandle)hRules].RecordSet);

    if (!poCurrentRecordSet->IsValid() ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK,"CompilerFunction: clcCompile"<<poCMCR->iNodeN(m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())<<LogCatAnd<<poCMCR->nNodeN(m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())  << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }

    if (poCurrentRecordSet->Current.Index == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_RECORD_TO_COMPILE,M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }

	if ( pTrad->SetAccess   (m_oState1.m_poAccess, m_oState1.m_hItem, m_oState1.m_poFormula.GetLevel() ) != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
	}


	pTrad->SetRegIndex (poCurrentRecordSet->Current.Index);
    pTrad->SetRecordSet(poCurrentRecordSet);
    
	ret=pTrad->SetArguments();
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_SETTING_ARGS, DumpCallStack() );
		FreeTrad(pTrad);
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}

        	
	hSourceCode = poCMCR->GetNodeItemHandleById(hRules,M4_ITEM_SCHTIS_RULE_CODES_SOURCE_CODE);
	hTempCode = poCMCR->GetNodeItemHandleById(hRules,M4_ITEM_SCHTIS_RULE_CODES_TEMP_CODE);
	
	if (hTempCode&&hSourceCode)
	{
		poCurrentRecordSet->Register.Item[(ClHandle)hSourceCode].Value.Get(vTempCode);
		/* Bug 0084826 No se debe modificar el estado */
		poCurrentRecordSet->Register.Item[(ClHandle)hTempCode].Value.SetNoChangeStatus(vTempCode);
	}
	else
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
	}
	
	ret=pTrad->SetCurrentRuleInfo(M4_TRUE);
    if (ret!=M4_SUCCESS){
		DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_COMPILING, DumpCallStack() );
    }

	////// Cuando compilen que no se haga transformación.
	ret=pTrad->CompileRule(M4_GET_POLISH);
	
	//reseteamos la información sobre la current rule
	pTrad->SetCurrentRuleInfo(M4_FALSE,"","","","");
	
	FreeTrad(pTrad);

    if (ret!=M4_SUCCESS){
		DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_COMPILING, DumpCallStack() );
    }
	//metemos el resultado
	RuntimePushDouble( ret);
	return M4_SUCCESS;
}


m4return_t ClVM1CompilerAndJIT::clcGetPolish	(void){

	m4return_t	ret;

    ClAccessRecordSet     *poCurrentRecordSet;
	m4VariantType vTempCode;
	ClCompiledMCR *poCMCR = 0;
	m4uint32_t hRules=0,hSourceCode=0,hTempCode=0;

	poCMCR=m_oState1.m_poAccess->GetpCMCR();
    
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1," clcGetPolish.");
#endif

	ClTrad *pTrad=0;
	if (InitTrad (M4CL_FINDER_TICH,pTrad)!=M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) );
        RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}


	M4_ASSERT (pTrad);

	//Comprobamos que existe el nodo de Rules. No hace falta que estemos posicionados en el !!!.
	hRules = poCMCR->GetChannelNodeHandleById(M4_NODE_SCHTIS_RULES);
	if (hRules == 0)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, M4_NODE_SCHTIS_RULES<<"clcGetPolish" << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }

    poCurrentRecordSet=&(m_oState1.m_poAccess->Node[(ClHandle)hRules].RecordSet);

    if (!poCurrentRecordSet->IsValid() ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK,"CompilerFunction: clcGetPolish"<<"Node:"<<LogCat<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())  << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }

    if (poCurrentRecordSet->Current.Index == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_RECORD_TO_COMPILE,M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }
	if ( pTrad->SetAccess   (m_oState1.m_poAccess, m_oState1.m_hItem, m_oState1.m_poFormula.GetLevel()) != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK,"CompilerFunction: clcGetPolish"<<"Node:"<<LogCat<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
	}

	pTrad->SetRegIndex (poCurrentRecordSet->Current.Index);
    pTrad->SetRecordSet(poCurrentRecordSet);

	ret=pTrad->SetArguments();
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_SETTING_ARGS, DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
		return M4_ERROR;
	}


	hSourceCode = poCMCR->GetNodeItemHandleById(hRules,M4_ITEM_SCHTIS_RULE_CODES_SOURCE_CODE);
	hTempCode = poCMCR->GetNodeItemHandleById(hRules,M4_ITEM_SCHTIS_RULE_CODES_TEMP_CODE);
	
	if (hTempCode&&hSourceCode)
	{
		poCurrentRecordSet->Register.Item[(ClHandle)hSourceCode].Value.Get(vTempCode);
		/* Bug 0084826 No se debe modificar el estado */
		poCurrentRecordSet->Register.Item[(ClHandle)hTempCode].Value.SetNoChangeStatus(vTempCode);
	}
	else
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
	}
	//////
	ret=pTrad->SetCurrentRuleInfo(M4_TRUE);
    if (ret!=M4_SUCCESS){
		DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_COMPILING, DumpCallStack() );
    }

    ret=pTrad->CompileRule ( M4_GET_POLISH );

	//reseteamos la información sobre la current rule
	pTrad->SetCurrentRuleInfo(M4_FALSE,"","","","");

	FreeTrad(pTrad);
    
    if (ret!=M4_SUCCESS){
		DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_COMPILING, DumpCallStack() );
    }
	
    //metemos el resultado
	RuntimePushDouble( ret);

	return ret;

}
m4return_t ClVM1CompilerAndJIT::clcCheckSyntax	(void){
	return (_clcCheckSyntax());
}

m4return_t ClVM1CompilerAndJIT::clcSourceToId	(void){
	return (_clcCheckSyntax(M4_SOURCE_TO_ID));
}
m4return_t ClVM1CompilerAndJIT::clcOldItemToIDItem	(void){
	return (_clcCheckSyntax(M4_OLD_ITEM_TO_ID_ITEM));
}
m4return_t ClVM1CompilerAndJIT::clcSourceToNumber	(void){
	return (_clcCheckSyntax(M4_SOURCE_TO_NUMBER));
}
m4return_t ClVM1CompilerAndJIT::clcSourceToString	(void){
	return (_clcCheckSyntax(M4_SOURCE_TO_STRING));
}
m4return_t ClVM1CompilerAndJIT::clcRestoreSourceAfterLoad	(void){
	return (RestoreSource(M4_RESTORE_AFTER_LOAD));
}
m4return_t ClVM1CompilerAndJIT::clcRestoreAfterCompile	(void){
/*    RuntimePushDouble (M4_ERROR);
	DUMP_CHLOG_ERRORF(M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE, "clcRestoreAfterCompile"<<"none" << DumpCallStack(&m_oState1) );
	return M4_ERROR;
*/
	// Mapeada de momento.
	return clcOldTIToIDTI();
}
m4return_t ClVM1CompilerAndJIT::clcOldTIToIDTI	(void){
	return _clcCheckSyntax(M4_OLD_TI_TO_ID_TI);
}

m4return_t ClVM1CompilerAndJIT::ResolveAlias	(void){
	return _clcCheckSyntax(M4_TI_BASE_TO_INHERITED);
}
m4return_t ClVM1CompilerAndJIT::RestoreSource	(m4bool_t ai_bTypeOfRestore)
{
	m4bool_t bSetAccess = M4_FALSE;
    m4return_t ret= M4_SUCCESS;
    ClRegisterIndex i(0);
    m4uint32_t  iNumReg;
    m4bool_t bIsPersisted;

	m4uint32_t hRules=0,hInternalDep=0,hExternalDep=0,hChannelDep=0;
	m4uint32_t hTempCode=0, hHasSynonym=0, hIsCorrectRule=0,hCompileLanguage=0,hSourceCode=0;	
	m4pcchar_t pWriteField = 0;
	m4bool_t bDontCompile = M4_FALSE ;
	ClCompiledMCR * poCMCR=0;


	poCMCR = m_oState1.m_poAccess->GetpCMCR();
        
    m4VariantType vType,vSourceCode;

    
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1," RestoreSource.");
#endif

    //Comprobamos que el bloque no sea nulo
    if (!m_oState1.m_poRecordSet->IsValid() ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK,"CompilerFunction: clcRestoreSource"<<"Node:"<<LogCat<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }
	ClTrad *pTrad=0;

   	if (InitTrad (M4CL_FINDER_TICH,pTrad)!=M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()));
        RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}


	M4_ASSERT (pTrad);

  
    iNumReg=m_oState1.m_poRecordSet->Register.Count();
    
    //comprobamos que el nodo en el que estamos sea el de RULES
	if (strcmp(m_oState1.m_poRecordSet->GetpNodeDef()->Id(),M4_NODE_SCHTIS_RULES))
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, M4_NODE_SCHTIS_RULES<<"RestoreSource" << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }

	hRules = m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle();

	//Ordenar el Access, para que quede con los Current bien posicionados.
	ClAccessRecordSet *poCurrentRecordSet;


	poCurrentRecordSet=&(m_oState1.m_poAccess->Node[(ClHandle)hRules].RecordSet);

	// ¿Me sigue haciendo falta el Follow?.
	poCurrentRecordSet->Follow(*(m_oState1.m_poRecordSet))	;


    ClAccess *paccess_xxx=0;

	hTempCode = poCMCR -> GetNodeItemHandleById( hRules,M4_ITEM_SCHTIS_RULE_CODES_TEMP_CODE, M4_TRUE  ) ;
	if ( hTempCode) pWriteField = poCMCR -> GetItemWriteFieldId( hTempCode ) ;
	hHasSynonym = poCMCR -> GetNodeItemHandleById( hRules,M4_ITEM_SCHTIS_RULES_HAS_SYNONYM, M4_TRUE  ) ;
	hIsCorrectRule = poCMCR -> GetNodeItemHandleById( hRules,M4_ITEM_SCHTIS_RULES_IS_CORRECT_RULE, M4_TRUE  ) ;
	hCompileLanguage =  poCMCR -> GetNodeItemHandleById( hRules,M4CL_VM_COMPILE_ITEM_LANGUAGE, M4_TRUE  ) ;
	hSourceCode = poCMCR -> GetNodeItemHandleById( hRules,M4_ITEM_SCHTIS_RULE_CODES_SOURCE_CODE, M4_TRUE  ) ;

	if (hHasSynonym==0 || hIsCorrectRule==0 || hTempCode==0 || hCompileLanguage==0 || hSourceCode==0)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_UNDEFINED_ITEM, "IsCorrectRule|HasSynonym|TempCode|SourceCode|CodeType"<<NodeN(poCMCR,hRules)<<"RestoreSource" << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
	}

	hInternalDep=poCMCR->GetChannelNodeHandleById(M4_NODE_SCHTIS_INTERNAL_DEP);
	hExternalDep=poCMCR->GetChannelNodeHandleById(M4_NODE_SCHTIS_EXTERNAL_DEP);
    hChannelDep=poCMCR->GetChannelNodeHandleById(M4_NODE_SCHTIS_CHANNEL_DEP);
    
    for (i=0; i< iNumReg; i++){

        bDontCompile = M4_FALSE ;
		//comprobamos si el registro esta borrado. si lo esta seguimos con los demas.
        if (m_oState1.m_poRecordSet->Register[i].Operation.IsDeleted ()) {
            //dejamos seguir
            continue;
        }

        //comprobamos si es de tipo LN4  (si no es ->continue)
        m_oState1.m_poRecordSet->Register[i].Item[(ClHandle)hCompileLanguage].Value.Get(vType);
        
        if (IsNull (vType) ){ //si no tienen puesto el tipo de la regla
            //sacamos un warning
			DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_VALUE, "RestoreSource"<<M4CL_VM_COMPILE_ITEM_LANGUAGE<<"<null>" );
            //dejamos seguir
            continue;
        }

        if (vType.Data.DoubleData !=M4CL_LENGUAGE_LN4) continue;
		//comprobamos si fue compilado o no.
		if ( ai_bTypeOfRestore == M4_RESTORE_AFTER_COMPILE )
		{
			//comprobamos si ha sido modificado y no persistido
			m_oState1.m_poRecordSet->Register[i].FlagPersist.Get (bIsPersisted);
			if (!bIsPersisted) {
				//dejamos seguir
				continue;
			}

			/// existe TEMP_CODE, luego pWriteField != NULL
			if ( strcmp ( pWriteField , "" ) != 0 )
			{
				// existe TEMP_CODE y tiene objeto de lectura.
				// no hacemos nada porque no hemos tocado el SOURCE_CODE.
				continue;
			}
			else 
			{
				// si no existe tenemos que copiar el código.
				bDontCompile = M4_TRUE;
			}
        }
		else 
		{
			// Comrobamos que en el canal hay nodo de dependencias, si las hay las cargamos.
			ClNode *pnode_xxx;
            ClBlock_Channel_Proxy *pb_xxx;

			if (hInternalDep)
			{
				if (paccess_xxx==0)
					m_oState1.m_poAccess->GetpChannel ()->CreateAccess (paccess_xxx);
				pnode_xxx = paccess_xxx->Node.Get ((ClHandle)hInternalDep);
				pb_xxx = m_oState1.m_poRecordSet->Register[i].GetChildBlock((ClHandle)hInternalDep);
				pnode_xxx->RecordSet.Follow (pb_xxx);
				pnode_xxx->RecordSet.Load_Block ();
			}


	
			if (hExternalDep)
			{
				if (paccess_xxx==0)
					m_oState1.m_poAccess->GetpChannel ()->CreateAccess (paccess_xxx);
				pnode_xxx = paccess_xxx->Node.Get ((ClHandle)hExternalDep);
				pb_xxx = m_oState1.m_poRecordSet->Register[i].GetChildBlock((ClHandle)hExternalDep);
				pnode_xxx->RecordSet.Follow (pb_xxx);
				pnode_xxx->RecordSet.Load_Block ();
			}

			if (hChannelDep)
			{
				if (paccess_xxx==0)
					m_oState1.m_poAccess->GetpChannel ()->CreateAccess (paccess_xxx);
				pnode_xxx = paccess_xxx->Node.Get ((ClHandle)hChannelDep);
				pb_xxx = m_oState1.m_poRecordSet->Register[i].GetChildBlock((ClHandle)hChannelDep);
				pnode_xxx->RecordSet.Follow (pb_xxx);
				pnode_xxx->RecordSet.Load_Block ();
			}


			/// existe TEMP_CODE, luego pWriteField != NULL
			// Condiciones para que se copie y no se compile ;
			// !pWriteField : 
			m_oState1.m_poRecordSet->Register[i].Item[(ClHandle)hIsCorrectRule].Value.Get(vType);
			
			if ( vType.Type == M4CL_CPP_TYPE_NUMBER && vType.Data.DoubleData == M4_FALSE )
			{
				///// en caso de que la regla no sea correcta, no se va a compilar y debemos copiar el fuente.
				bDontCompile = M4_TRUE ;
			}
			else 
			{
				m_oState1.m_poRecordSet->Register[i].Item[(ClHandle)hHasSynonym].Value.Get(vType);
				if ( vType.Type == M4CL_CPP_TYPE_NUMBER && vType.Data.DoubleData == M4_FORMULA_WITHOUT_SYNONYM )
				{
					// No tiene sinónimos, lo copiamos.
					bDontCompile = M4_TRUE ;
				}

			}

			// Si no tiene sinonimos y no hay WF para Temp_Code, tenemos en Source_Code el codigo correcto.
			// Si la regla no es correcta y no hay WF para Temp_Code, tenemos en Source_Code el codigo correcto.
			// En caso de que tengamos WF lo copiamos.
			if (strcmp (pWriteField,"") == 0 && bDontCompile )
				continue;

		}// else/restore_after_compile

		if ( M4_BOOL (bDontCompile) )
		{
			m_oState1.m_poRecordSet->Register[i].Item[(ClHandle)hTempCode].Value.Get(vSourceCode);

			/* Bug 0084826 No se debe modificar el estado */
			m_oState1.m_poRecordSet->Register[i].Item[(ClHandle)hSourceCode].Value.SetNoChangeStatus(vSourceCode);
			continue ;
		}
		if ( bSetAccess == M4_FALSE )
		{
			if ( pTrad->SetAccess   (m_oState1.m_poAccess, m_oState1.m_hItem, m_oState1.m_poFormula.GetLevel()) != M4_SUCCESS )
			{
				DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) );
				RuntimePushDouble (M4_ERROR);
				if (paccess_xxx!=0)
				    paccess_xxx->Destroy ();
				FreeTrad(pTrad);
				return M4_ERROR;
			}
			bSetAccess = M4_TRUE;
		}

		pTrad->SetRegIndex (i);
		pTrad->SetRecordSet(m_oState1.m_poRecordSet);

	     //llamamos a compilar
		ret=pTrad->SetArguments();
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_SETTING_ARGS, DumpCallStack() );
			RuntimePushDouble (M4_ERROR);
			FreeTrad(pTrad);
			return M4_ERROR;
		}

		//ponemos la información sobre la regla actual
		ret=pTrad->SetCurrentRuleInfo(M4_TRUE);
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_COMPILING, DumpCallStack() );
		}

		ret=pTrad->CompileRule(M4_RESTORE_SOURCE);

		//reseteamos la información sobre la current rule
		pTrad->SetCurrentRuleInfo(M4_FALSE,"","","","");

	    if (ret!=M4_SUCCESS){
			// Copiamos el codigo de TEMP_CODE a SOURCE_CODE por que si no lo perdemos.
			if (strcmp(pWriteField,""))
			{
				m_oState1.m_poRecordSet->Register[i].Item[(ClHandle)hTempCode].Value.Get(vSourceCode);

				/* Bug 0084826 No se debe modificar el estado */
				m_oState1.m_poRecordSet->Register[i].Item[(ClHandle)hSourceCode].Value.SetNoChangeStatus(vSourceCode);
			}
			DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_COMPILING, DumpCallStack() );
			continue; //De momento para las reglas antiguas, y para que "compile" las demás.
		}
    }

	if (paccess_xxx!=0){
	    paccess_xxx->Destroy ();
	}

	FreeTrad(pTrad);

	/*
	Bug 0084826 Ya no hace falta el checkpoint porque no se toca el estado
    m_oState1.m_poRecordSet->CheckPoint();
	*/

    RuntimePushDouble (ret);
    return ret;

}





m4return_t ClVM1CompilerAndJIT::_clcCheckSyntax	(Compile_Type ai_cSource){
	
	m4return_t	ret;
    ClAccessRecordSet		*poCurrentRecordSet;
	m4VariantType			vTempCode;
	m4uint32_t				hRules=0;
 
	ClCompiledMCR *poCMCR = 0;

	poCMCR=m_oState1.m_poAccess->GetpCMCR();
    
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1," clcCheckSyntax.");
#endif

	ClTrad *pTrad=0;
	if (InitTrad (M4CL_FINDER_TICH,pTrad)!=M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) );
        RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}


	M4_ASSERT (pTrad);

	hRules = poCMCR->GetChannelNodeHandleById(M4_NODE_SCHTIS_RULES);
	if (hRules == 0)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, M4_NODE_SCHTIS_RULES<<"clcCompile"  << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }

    poCurrentRecordSet=&(m_oState1.m_poAccess->Node[(ClHandle)hRules ].RecordSet);

    if (!poCurrentRecordSet->IsValid() ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK,"CompilerFunction: clcCheckSyntax"<<"Node:"<<LogCat<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }

    if (poCurrentRecordSet->Current.Index == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_RECORD_TO_COMPILE,M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }
	if ( pTrad->SetAccess   (m_oState1.m_poAccess, m_oState1.m_hItem, m_oState1.m_poFormula.GetLevel()) != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
	}

	pTrad->SetRegIndex (poCurrentRecordSet->Current.Index);
    pTrad->SetRecordSet(poCurrentRecordSet);

	//establecemos arg.
	ret=pTrad->SetArguments();
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_SETTING_ARGS, DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
		return M4_ERROR;
	}


	poCurrentRecordSet->Register.Item[M4_ITEM_SCHTIS_RULE_CODES_SOURCE_CODE].Value.Get(vTempCode);

	/* Bug 0084826 No se debe modificar el estado */
	poCurrentRecordSet->Register.Item[M4_ITEM_SCHTIS_RULE_CODES_TEMP_CODE].Value.SetNoChangeStatus(vTempCode);

	//ponemos la información sobre la regla actual
	ret=pTrad->SetCurrentRuleInfo(M4_TRUE);
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_COMPILING, DumpCallStack() );
	}

	//////
	//compilamos
	ret=pTrad->CompileRule (ai_cSource );

	//reseteamos la información sobre la current rule
	pTrad->SetCurrentRuleInfo(M4_FALSE,"","","","");

	FreeTrad(pTrad);
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_COMPILING, DumpCallStack() );
    }
	
	//metemos el resultado
	RuntimePushDouble( ret );
	return M4_SUCCESS; //si la sintaxis es o no correcta va en el res. que metemos en la pila
                       //el método ha funcioando bien
}



//--------------------------------------------------------
//Prueba local
//--------------------------------------------------------
m4return_t ClVM1CompilerAndJIT::_LocalTest (m4char_t *ai_pszSourceCode, m4char_t **ai_pszContext,ClTrad * ai_pTrad){

    m4return_t ret;

    if (m_poInterpreter==NULL){
        m_poInterpreter=new IntLN4;
    }

    m_poInterpreter->SetRTS (m_oRunTimeStack);
    m_poInterpreter->SetStrPool (m_oDynamicPool);
    m_poInterpreter->SetVM(m_poVM);
    m_poInterpreter->SetFuncTable(m_poFunctionTable);
    m_poInterpreter->SetTrad(ai_pTrad);

    ret=m_poInterpreter->LocalTest(ai_pszSourceCode,ai_pszContext);
    return ret;
}


m4return_t ClVM1CompilerAndJIT::LocalTest (void){
    

   	m4return_t	ret,LocalRet;
    ClAccessRecordSet     *poCurrentRecordSet;
    m4VariantType vParam, vSource;
    m4char_t    *pszSource, *pszParam;
	ClCompiledMCR *poCMCR = 0;
	m4uint32_t				hRules=0;

	poCMCR=m_oState1.m_poAccess->GetpCMCR();


	/* Bug 0088929
	Sólo se deja ejecutar la función loacaltest en el design channel
	*/

	/* Bug 0121141
	También se debe poder ejecutar en el design channel proxy
	*/
	m4pcchar_t	pccChannelId = poCMCR->GetChannelId() ;

	if( strcmp( pccChannelId, M4_CHANNEL_SCHT3S_DESIGN_CHANNEL ) != 0 && strcmp( pccChannelId, M4_CHANNEL_SCHT3S_DESIGN_CHANNEL_PROXY ) != 0 )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_BAD_LOACALTEST_M4OBJ, m_oState1.m_iSourceLine << poCMCR->GetItemFormIdByPosition( m_oState1.m_hItem, m_oState1.m_iRule ) << M4ObjNodeItemI( poCMCR, m_oState1.m_hItem ) << DumpCallStack() ) ;
        RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}


#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1," LocalTest.");
#endif

	ClTrad *pTrad=0;
	if (InitTrad (M4CL_FINDER_TICH,pTrad)!=M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) );
        RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}


	M4_ASSERT (pTrad);
	
	hRules = poCMCR->GetChannelNodeHandleById(M4_NODE_SCHTIS_RULES);
	if (hRules == 0)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, M4_NODE_SCHTIS_RULES<<"clcCompile"  << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }

    poCurrentRecordSet=&(m_oState1.m_poAccess->Node[(ClHandle)hRules].RecordSet);

    if (!poCurrentRecordSet->IsValid() ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK,"CompilerFunction: LocalTest"<<"Node:"<<LogCat<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }

    if (poCurrentRecordSet->Current.Index == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_RECORD_TO_COMPILE,M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }

	if ( pTrad->SetAccess   (m_oState1.m_poAccess, m_oState1.m_hItem, m_oState1.m_poFormula.GetLevel()) != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
	}
    pTrad->SetRegIndex (poCurrentRecordSet->Current.Index);
    pTrad->SetRecordSet(poCurrentRecordSet);

    //leemos la polaca y argumentos
    ret=poCurrentRecordSet->Current.Item[M4_ITEM_SCHTIS_RULE_CODES_POLISH_CODE].Value.Get(vSource);
    //M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, M4ObjNodeItemI(poCurrentRecordSet->GetpAccess()->GetpCMCR(),poCurrentRecordSet->Current.Item.ItemDef.GetHandle())<<"LocalTest" << DumpCallStack() );
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poCurrentRecordSet->GetpAccess()->GetpCMCR(),poCurrentRecordSet->Current.Item.ItemDef.GetHandle())<<"LocalTest" << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
		return M4_ERROR;
	}

    ret=poCurrentRecordSet->Current.Item[M4_ITEM_SCHTIS_RULE_CODES_TEST_PARAMETERS].Value.Get(vParam);
//    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_ERROR_READING_ITEM, ERRORLOG, M4ObjNodeItemI(poCurrentRecordSet->GetpAccess()->GetpCMCR(),poCurrentRecordSet->Current.Item.ItemDef.GetHandle())<<"LocalTest" << DumpCallStack() );
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poCurrentRecordSet->GetpAccess()->GetpCMCR(),poCurrentRecordSet->Current.Item.ItemDef.GetHandle())<<"LocalTest" << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
		return M4_ERROR;
	}


    
    if (IsNull(vSource) || vSource.Type!=M4CL_CPP_TYPE_STRING_VAR){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_VALUE, "LocalTest"<<M4_ITEM_SCHTIS_RULE_CODES_POLISH_CODE<<"<null>" );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }
    pszSource=vSource.Data.PointerChar;

    if (IsNull(vParam) ){
        pszParam="";
    }
    else if (vParam.Type!=M4CL_CPP_TYPE_STRING_VAR){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_VALUE, "LocalTest"<<M4_ITEM_SCHTIS_RULE_CODES_TEST_PARAMETERS<<"<null>" );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }
    else{
        pszParam=vParam.Data.PointerChar;
    }


    LocalRet=_LocalTest (pszSource, &pszParam,pTrad);
    if (LocalRet!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_IN_LOCAL_TEST, DumpCallStack() );
    }


    //ponemos los arguementos
	vParam.Type=M4CL_CPP_TYPE_STRING_VAR;
    vParam.Data.PointerChar=pszParam;
    
	/* Bug 0084826 No se debe modificar el estado */
    ret=poCurrentRecordSet->Current.Item[M4_ITEM_SCHTIS_RULE_CODES_TEST_PARAMETERS].Value.SetNoChangeStatus(vParam);
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_READING_ITEM, M4ObjNodeItemI(poCurrentRecordSet->GetpAccess()->GetpCMCR(),poCurrentRecordSet->Current.Item.ItemDef.GetHandle())<<"LocalTest" << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
		return M4_ERROR;
	}

	//metemos el resultado
	RuntimePushDouble( LocalRet);
	FreeTrad(pTrad);
	return LocalRet; // si no compila bien dejamos de ejecutar retornando error

}



//-------------------------------------------------------------------------
// C O M P I L E   B L O C K
//-------------------------------------------------------------------------
m4return_t  ClVM1CompilerAndJIT::CompileBlock (void){
    
	m4bool_t bSetAccess = M4_FALSE;
    m4return_t ret= M4_SUCCESS;

    ClRegisterIndex			i(0);
    m4uint32_t				iNumReg;
	m4uint32_t hRules=0,hInternalDep=0,hExternalDep=0,hChannelDep=0;
	m4uint32_t hTempCode=0, hHasSynonym=0, hIsCorrectRule=0,hCompileLanguage=0,hSourceCode=0,hPolishCode=0;	
    
    //EDU:
    //ClRecordSet				oRS;
    ClRecordSet				oRS (m_oState1.m_poAccess);
	ClBlock_Channel_Proxy	*poBlock;				

	ClCompiledMCR *poCMCR;
    m4VariantType vType,vTempCode;

    
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1," CompileBlock.");
#endif

   	poCMCR = m_oState1.m_poAccess->GetpCMCR();


    //Comprobamos que el bloque no sea nulo
    if ( !m_oState1.m_poRecordSet->IsValid() ){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK,"CompilerFunction: CompileBlock"<<"Node:"<<LogCat<<m_oState1.m_poRecordSet->GetpNodeDef()->Id()<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
        return M4_ERROR;
    }

	ClTrad * pTrad =0 ;
	if (InitTrad (M4CL_FINDER_TICH,pTrad)!=M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) );
        RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}


	M4_ASSERT (pTrad);
  
    iNumReg=m_oState1.m_poRecordSet->Register.Count();
    
    //comprobamos que el nodo en el que estamos tenga el item "M4CL_VM_COMPILE_ITEM_LANGUAGE"     
	hRules = poCMCR->GetChannelNodeHandleById(M4_NODE_SCHTIS_RULES);
	if (hRules == 0)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, M4_NODE_SCHTIS_RULES<<"CompileBlock"  << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
    }

	hTempCode = poCMCR->GetNodeItemHandleById(hRules,M4_ITEM_SCHTIS_RULE_CODES_TEMP_CODE);
	hHasSynonym = poCMCR -> GetNodeItemHandleById( hRules,M4_ITEM_SCHTIS_RULES_HAS_SYNONYM, M4_TRUE  ) ;
	hIsCorrectRule = poCMCR -> GetNodeItemHandleById( hRules,M4_ITEM_SCHTIS_RULES_IS_CORRECT_RULE, M4_TRUE  ) ;
	hCompileLanguage =  poCMCR -> GetNodeItemHandleById( hRules,M4CL_VM_COMPILE_ITEM_LANGUAGE, M4_TRUE  ) ;
	hSourceCode = poCMCR -> GetNodeItemHandleById( hRules,M4_ITEM_SCHTIS_RULE_CODES_SOURCE_CODE, M4_TRUE  ) ;
	hPolishCode = poCMCR -> GetNodeItemHandleById( hRules,M4_ITEM_SCHTIS_RULE_CODES_POLISH_CODE, M4_TRUE  ) ;

	if (hHasSynonym==0 || hIsCorrectRule==0 || hTempCode==0 || hCompileLanguage==0 || hSourceCode==0 || hPolishCode == 0)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_UNDEFINED_ITEM, "IsCorrectRule|HasSynonym|TempCode|SourceCode|PolishCode|CodeType"<<NodeN(poCMCR,hRules)<<"CompileBlock" << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
	}
	hInternalDep=poCMCR->GetChannelNodeHandleById(M4_NODE_SCHTIS_INTERNAL_DEP);
	hExternalDep=poCMCR->GetChannelNodeHandleById(M4_NODE_SCHTIS_EXTERNAL_DEP);
    hChannelDep=poCMCR->GetChannelNodeHandleById(M4_NODE_SCHTIS_CHANNEL_DEP);

    for (i=0; i< iNumReg; i++){

        //comprobamos si ha sido modificado 
        if ((!m_oState1.m_poRecordSet->Register[i].Operation.IsModified ()) ||
            (m_oState1.m_poRecordSet->Register[i].Operation.IsDeleted ()    ) ) {
            //dejamos seguir
            continue;
        }
        //comprobamos si es de tipo LN4  (si no es ->continue)
        m_oState1.m_poRecordSet->Register[i].Item[(ClHandle)hCompileLanguage].Value.Get(vType);
        
        if (IsNull (vType) ){ //si no tienen puesto el tipo de la regla
            //sacamos un warning
			DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_VALUE, "CompileBlock"<<M4CL_VM_COMPILE_ITEM_LANGUAGE<<"<null>" );
            //dejamos seguir
            continue;
        }

        if (vType.Data.DoubleData !=M4CL_LENGUAGE_LN4) continue;
		m_oState1.m_poRecordSet->Register[i].Item[(ClHandle)hSourceCode].Value.Get(vTempCode);

		/* Bug 0084826 No se debe modificar el estado */
		m_oState1.m_poRecordSet->Register[i].Item[(ClHandle)hTempCode].Value.SetNoChangeStatus(vTempCode);

		//comprobamos si se quiere compilar o no .

	    m_oState1.m_poRecordSet->Register[i].Item[(ClHandle)hIsCorrectRule].Value.Get(vType);
		//si no se quiere compilar vamos a la siguiente.

		if (IsNull(vType)||vType.Data.DoubleData == M4CL_VM_DONT_COMPILE)
		{

			m_oState1.m_poRecordSet->Register[i].Item[(ClHandle)hPolishCode].Value.SetNULL();
		
			// Si tenemos nodos de dependencias las borramos.
			if (hInternalDep)
			{
				poBlock = m_oState1.m_poRecordSet->Register[i].GetChildBlock((ClHandle)hInternalDep);
				oRS.SetCurrentBlock(poBlock);
				oRS.Register.DeleteAll();
			}
			
			if (hExternalDep)
			{
				poBlock = m_oState1.m_poRecordSet->Register[i].GetChildBlock((ClHandle)hExternalDep);
				oRS.SetCurrentBlock(poBlock);
				oRS.Register.DeleteAll();
			}

			if (hChannelDep)
			{
				poBlock = m_oState1.m_poRecordSet->Register[i].GetChildBlock((ClHandle)hChannelDep);
				oRS.SetCurrentBlock(poBlock);
				oRS.Register.DeleteAll();
			}
			continue;
		}

		if ( bSetAccess == M4_FALSE )
		{
			if ( pTrad->SetAccess   (m_oState1.m_poAccess, m_oState1.m_hItem, m_oState1.m_poFormula.GetLevel()) != M4_SUCCESS )
			{
				DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) );
				RuntimePushDouble (M4_ERROR);
				FreeTrad(pTrad);
				return M4_ERROR;
			}
			bSetAccess = M4_TRUE;

		}

        pTrad->SetRegIndex (i);
        pTrad->SetRecordSet(m_oState1.m_poRecordSet);

         //llamamos a compilar
        ret=pTrad->SetArguments();
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_ERROR_SETTING_ARGS, DumpCallStack() );
			RuntimePushDouble (M4_ERROR);
			FreeTrad(pTrad);
			return M4_ERROR;
		}

		//ponemos la información sobre la regla actual
		ret=pTrad->SetCurrentRuleInfo(M4_TRUE);
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_COMPILING, DumpCallStack() );
		}

        ret=pTrad->CompileRule(M4_SOURCE_SAVE);

		//reseteamos la información sobre la current rule
		pTrad->SetCurrentRuleInfo(M4_FALSE,"","","","");

        if (ret!=M4_SUCCESS){
			DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_COMPILING, DumpCallStack() );
            break;
        }

    }

	FreeTrad(pTrad);
    

    RuntimePushDouble (ret);
    return ret;
}


//--------------------------------------
//J u s t  I n  T i m e
//--------------------------------------
m4return_t ClVM1CompilerAndJIT::clcExecuteJIT (void)
{
	m4return_t ret=M4_SUCCESS;
	m4VariantType vValue,*pvValues;
	m4int32_t	iHandle,iNumArgs;

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1," clcExecuteJIT.");
#endif


	ret=RuntimePop(vValue);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Execute JIT.(number of arguments).")	;
    
	if (vValue.Type!=M4CL_CPP_TYPE_NUMBER)
	{
		ret = m_oVariantMediator->ToDouble(vValue);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Number_of_args" << "ExecuteJIT" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
    }
	

	iNumArgs=(m4int32_t)vValue.Data.DoubleData-1;

	if (iNumArgs<0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_BAD_NUMBER_OF_ARGUMENTS, "clcExecuteJIT" << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}

	pvValues= new m4VariantType[iNumArgs];

	if (iNumArgs)
	{
		for (m4int32_t i=0;i<iNumArgs;i++)
		{
		//Extraigo el valor de cada argumento.
			ret=RuntimePop(vValue);
			if (ret != M4_SUCCESS )
			{
//				M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Execute JIT.Error performing pop from stack(argument value).")	;
				DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP, "Argument value in clcExecuteJIT" );
				FreePoolStrings ( pvValues , i-1 );
				delete [] pvValues;
				RuntimePushDouble ( M4_ERROR );
				return M4_ERROR;
			}
			pvValues[i]=vValue;
		}
	}
	//Extraigo el handle
	ret=RuntimePop(vValue);
//	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Execute JIT.Error performing pop from stack (handle).")	;
	if (ret != M4_SUCCESS )
	{
		DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP, "Handle in clcExecuteJIT" );
		FreePoolStrings ( pvValues ,iNumArgs);
		delete [] pvValues;
		RuntimePushDouble ( M4_ERROR );
		return M4_ERROR;
	}
    if (vValue.Type!=M4CL_CPP_TYPE_NUMBER)
	{
		ret = m_oVariantMediator->ToDouble(vValue);
		if (ret != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Handle"<<"ExecuteJIT"  << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
			FreePoolStrings ( pvValues , iNumArgs );
			delete [] pvValues;
			RuntimePushDouble ( M4_ERROR );
			return M4_ERROR;
		}
  
    }


	iHandle=(m4int32_t)vValue.Data.DoubleData;

	m4pchar_t pCodeJIT=0;
	
	ret = m_oState1.m_poAccess -> GetpChannel() -> GetpCodeJIT() -> GetCodebyHandle ( (m4int32_t )vValue.Data.DoubleData, &pCodeJIT );

	if ( ret == M4_SUCCESS )
	{
		M4_ASSERT (pCodeJIT);
		ret = ExecuteJIT ( pCodeJIT , pvValues , iNumArgs , M4_TRUE );
	}


	if (ret != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_JIT_HANDLE_NOT_FOUND, DumpCallStack() );
		FreePoolStrings ( pvValues , iNumArgs );
		delete []pvValues;
		RuntimePushDouble (M4_ERROR );
		return M4_ERROR ;
	}

	delete []pvValues;
	//Debe liberar el metodo que toma los argumentos
	//FreePoolStrings ( pvValues , iNumArgs );

	return M4_SUCCESS;
}

m4return_t ClVM1CompilerAndJIT::clcPrepareJIT (void)
{
	m4return_t ret=M4_SUCCESS;
	m4VariantType vValue,vArgRef;
	m4pchar_t	pszLN4code,pszTemp,pszCodeSerial;
	m4int32_t	iNumArgs;
	m4uint32_t	iLen;
	
	m4VariantType *pvArguments;

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1," clcPrepareJIT.");
#endif


	
	//Extraigo el numero de argumentos.
	ret=RuntimePop(vValue);
	DEBUG_CHECK_RES(ret==M4_SUCCESS,M4_CH_VM_RTS_POP, ERRORLOG, "Preparing JIT.")	;
	if (vValue.Type!=M4CL_CPP_TYPE_NUMBER)
	{
		ret = m_oVariantMediator->ToDouble(vValue);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "number_of_args" << "PrepareJIT" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
    }
	// ******** Correccion *****

	iNumArgs=(m4int32_t)vValue.Data.DoubleData-2;//me vienen el código, el handle y los argumentos.

	if (iNumArgs<0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_BAD_NUMBER_OF_ARGUMENTS, "clcPrepareJIT" << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
	
	pvArguments=new	m4VariantType[iNumArgs];

	if (iNumArgs) 
	{

		//Extraigo los argumentos
		for (m4int32_t i=0;i<iNumArgs;i++)
		{
			//Extraigo el nombre del argumento.
			ret=RuntimePop(vValue);
			if (ret != M4_SUCCESS )
			{
				DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP, "Argument name in clcPrepareJIT" );
				FreePoolStrings ( pvArguments , i-1 );
				delete [] pvArguments;
				RuntimePushDouble ( M4_ERROR );
				return M4_ERROR;
			}
			pvArguments[i]=vValue;
		}
	}

	//Extraigo el handle(referencia)
	ret=RuntimePop(vArgRef);
	if (ret != M4_SUCCESS )
	{
		DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP, "Handle (reference) in clcPrepareJIT" );
		FreePoolStrings ( pvArguments , iNumArgs );
		delete [] pvArguments;
		RuntimePushDouble ( M4_ERROR );
		return M4_ERROR;
	}

	//Extraigo el código:
	ret=RuntimePop(vValue);
	if (ret != M4_SUCCESS )
	{
		DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP, "LN4 code in clcPrepareJIT" );
		FreePoolStrings ( pvArguments , iNumArgs );
		delete [] pvArguments;
		RuntimePushDouble ( M4_ERROR );
		return M4_ERROR;
	}

	if (vValue.Type!=M4CL_CPP_TYPE_STRING_POOL)
	ret=m_oVariantMediator->ToPoolString(vValue);
	if (ret != M4_SUCCESS )
	{
		DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP, "LN4 code in clcPrepareJIT" );
		FreePoolStrings ( pvArguments , iNumArgs );
		delete [] pvArguments;
		RuntimePushDouble ( M4_ERROR );
		return M4_ERROR;
	}

	pszTemp=m_oDynamicPool->GetString(vValue.Data.CharData);
	iLen = m_oDynamicPool->GetStringLength(vValue.Data.CharData)+1;
	pszLN4code=new m4char_t[iLen];
	strcpy(pszLN4code,pszTemp);

	m4pchar_t pszPolaca;
	ret=CompileJIT(pszLN4code,pvArguments, iNumArgs,pszPolaca);

	FreePoolStrings ( pvArguments , iNumArgs );
	delete []pvArguments;
	delete []pszLN4code;

	if (ret != M4_SUCCESS )
	{
		m_oDynamicPool -> DeleteString (vValue.Data.CharData);
		// Me temos handle -1 error en la pila y retornamos exito.
		RuntimePushDouble ( M4_ERROR );
		RuntimePushDouble (-1.0);
		return M4_SUCCESS;
	}
	
	ret=InstallJIT(pszPolaca,pszCodeSerial);
	if (ret != M4_SUCCESS )
	{
		m_oDynamicPool -> DeleteString (vValue.Data.CharData);
		RuntimePushDouble ( M4_ERROR );
		return M4_ERROR;
	}

	
	///  No borro el pszCodeSerial, porque lo que guardo es el puntero. En el destructor, lo borro.
	m4int32_t HandleJIT;
	ret = m_oState1.m_poAccess -> GetpChannel() -> GetpCodeJIT() -> InsertCode ( pszCodeSerial ,HandleJIT );	

	
	
	
	m_oDynamicPool -> DeleteString (vValue.Data.CharData);
	RuntimePushDouble(ret);

	/// Si ha retornado éxito, le asignamos tipo NUMBER al Variant.
	/// Si no borramos la cadena de código serializada, y no hacemos el push del handle.
	if (ret == M4_SUCCESS )
	{
		vArgRef.Data.DoubleData = (m4double_t) HandleJIT;
		vArgRef.Type=M4CL_CPP_TYPE_NUMBER;
		RuntimePush(vArgRef);

	}
	else 
	{
		vArgRef.Type=M4CL_CPP_TYPE_NULL;
		delete []pszCodeSerial;
	}

	return ret;
}

m4return_t ClVM1CompilerAndJIT::CompileJIT	(m4pchar_t ai_pszLN4Code,m4VariantType *ai_avArgsName,m4int32_t ai_iNumRealArgs,m4pchar_t &ao_pszPolish)
{
	m4return_t	ret=M4_SUCCESS;
	m4pchar_t	*ppszArguments = 0;
	m4pchar_t	pszTemp = 0;
	m4pbool_t	pbReference = 0;
	m4int32_t	iLen;
	
	ClCompiledMCR *poCMCR = 0;

	poCMCR=m_oState1.m_poAccess->GetpCMCR();

	ClTrad * pTrad;
	if (InitTrad (M4CL_FINDER_JIT,pTrad)!=M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) );
        RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}

	M4_ASSERT (pTrad);
	if ( pTrad->SetAccess   (m_oState1.m_poAccess, m_oState1.m_hItem, m_oState1.m_poFormula.GetLevel()) != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK,"CompilerFunction: clcCompileJIT"<<poCMCR->iNodeN(m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())<<LogCatAnd<<poCMCR->nNodeN(m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle())<<DumpTreePath(m_oState1.m_poAccess, m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle()) << DumpCallStack() );
        RuntimePushDouble (M4_ERROR);
		FreeTrad(pTrad);
        return M4_ERROR;
	}
    pTrad->SetRecordSet(m_oState1.m_poRecordSet);
	pTrad->SetRegIndex(m_oState1.m_oRegisterIndex);

	if (ai_iNumRealArgs)
	{
		pbReference=new m4bool_t[ai_iNumRealArgs];
		ppszArguments=new m4pchar_t[ai_iNumRealArgs];

		for (m4int32_t j=0;j<ai_iNumRealArgs;j++)	
		{
			//NINGUN ARGUMENTO POR REFERENCIA
			pbReference[j]=M4_FALSE;
		
			if (ai_avArgsName[j].Type!=M4CL_CPP_TYPE_STRING_POOL)
			{
				ret=m_oVariantMediator->ToPoolString(ai_avArgsName[j]);
				if (ret!=M4_SUCCESS)
				{
					DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, LOG_CAT_BLOCK( "Argument("<<j<<")" ) <<"CompileJIT"  << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(ai_avArgsName[j].Type)<<DumpCallStack() );

					for ( m4int32_t i = 0 ; i < j -1 ; i-- )
						delete ppszArguments [i];

					delete [] ppszArguments;
					delete [] pbReference;
					FreeTrad(pTrad);
					return M4_ERROR;
				}
			}	
		
			pszTemp=m_oDynamicPool->GetString(ai_avArgsName[j].Data.CharData);
			//iLen = strlen (pszTemp)+1;
			iLen = m_oDynamicPool->GetStringLength(ai_avArgsName[j].Data.CharData)+1;
			ppszArguments[j]=new m4char_t[iLen];
			strcpy(ppszArguments[j],pszTemp);
		}
	
	
	
	}
	pTrad->SetArguments(ppszArguments,pbReference,(m4int8_t) ai_iNumRealArgs,M4_TRUE);

	if (ai_iNumRealArgs) {
		for (m4int32_t k = 0; k < ai_iNumRealArgs; k++) delete ppszArguments[k];
		delete []ppszArguments;
		delete []pbReference;
	}

	//ponemos la información sobre la regla actual
	m4uint32_t hNode=poCMCR->GetItemNodeHandle(m_oState1.m_hItem);
	ret=pTrad->SetCurrentRuleInfo(M4_FALSE, poCMCR->GetChannelId(), poCMCR->GetNodeId(hNode), poCMCR->GetItemId(m_oState1.m_hItem), poCMCR->GetItemFormIdByPosition(m_oState1.m_hItem, m_oState1.m_iRule) );
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_DEBUGF(M4_CH_VM_ERROR_COMPILING, DumpCallStack() );
	}

	pTrad->SetSourceMode(M4_NO_SOURCE);
	ret=pTrad->Compile(ai_pszLN4Code);
	ao_pszPolish = pTrad->GetPolishCode();

	//reseteamos la información sobre la current rule
	pTrad->SetCurrentRuleInfo(M4_FALSE,"","","","");

	FreeTrad(pTrad);

	return ret;
}

m4return_t ClVM1CompilerAndJIT::InstallJIT	(m4pchar_t ai_pszPolish,m4pchar_t &ao_CodeSerial)

{
	m4return_t ret=M4_SUCCESS;
	m4int32_t iNumInstLN4=0,iNumInstFormula=0;
	m4uint32_t iSize=0,iUpperSize,iNoUpperSize;

	if (m_oUpperPool==NULL)
		m_oUpperPool=new ClStaticPoolStr(M4_VM_STATIC_POOL_SIZE_FOR_JIT,M4CL_UPPERCASE);

	if (m_oNoUpperPool==NULL)
		m_oNoUpperPool=new ClStaticPoolStr(M4_VM_STATIC_POOL_SIZE_FOR_JIT);


	ClTempNitemFormula  oTempNItemFormula;

	oTempNItemFormula.SetUpperPool( m_oUpperPool ) ;
	oTempNItemFormula.SetNoUpperPool( m_oNoUpperPool ) ;
	
	// pasamos la tabla de funciones y la de atributos.
	oTempNItemFormula.InitforJIT(
		ClFuncTableDesc::GetInstance(),
		ClAtributeFuncTable::GetInstance(),
		m_oState1.m_hItem,
		m_oState1.m_poFormula.GetLevel(),
		m_oState1.m_poRecordSet,
		m_oState1.m_poAccess->GetpCMCR(),
		M4CL_LENGUAGE_LN4,
		M4CL_RULE_MIN_PRIORITY,
		m_oState1.m_dBeginDate,
		m_oState1.m_dEndDate);

	oTempNItemFormula.SetMustReturn(0); // ¿NO HAY CONSTANTE?		
	ret=oTempNItemFormula.CountInstructionsLN4forJIT(ai_pszPolish,iNumInstLN4);
	//ret=oTempNItemFormula.CountInstructionsLN4forJIT(pTrad->GetPolishCode(),iNumInstLN4);
	
	if (ret!=M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR(M4_CH_VM_INSTALLING_JIT);
		return M4_ERROR;
	}
	ret=oTempNItemFormula.InstallLN4();
	if (ret!=M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR(M4_CH_VM_INSTALLING_JIT);
		return M4_ERROR;
	}
	
	oTempNItemFormula.GetSize(iSize,NULL);

	iUpperSize=m_oUpperPool->GetLength();
	iNoUpperSize=m_oNoUpperPool->GetLength();

	ao_CodeSerial=new m4char_t[iSize+iUpperSize+iNoUpperSize+1];		//Se destruye al acabar la ejecución, si procede.

	if (ao_CodeSerial!=NULL)	
		ret=oTempNItemFormula.Compile(ao_CodeSerial, iSize,iUpperSize);
	else return M4_ERROR;

	if (ret!=M4_SUCCESS)
	{
		delete []ao_CodeSerial;
		DUMP_CHLOG_ERROR(M4_CH_VM_INSTALLING_JIT);
		ao_CodeSerial=NULL;
		return M4_ERROR;
	}

	m_oUpperPool->Copy(ao_CodeSerial,iSize);
	m_oNoUpperPool->Copy(ao_CodeSerial,iSize+iUpperSize);

	m_oUpperPool->Reset();
	m_oNoUpperPool->Reset();

	
	return ret;
}



m4return_t  ClVM1CompilerAndJIT::clcExecuteLN4JIT (void){

	m4return_t ret=M4_SUCCESS;
	m4VariantType vValue;
	m4pchar_t	pszTemp;
	m4int32_t	iNumArgs,iNumRealArgs;
	m4double_t dCsType = M4CL_CSTYPE_BOTH;
	
	
	m4VariantType *pvValues,*pvArguments;

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1," clcExecuteLN4JIT.");
#endif

	/* Bug 0150731
	Si el canal es de debug el ExecuteLN4JIT se debe ejecutar en servidor
	*/
	m_oState1.m_poAccess->GetpChannel()->CsType.Get( dCsType ) ;

	if( m4uint8_t( dCsType ) == M4CL_CSTYPE_DEBUG )
	{
		ret = m_poVM->CreateRemoteCall() ;
		return( ret ) ;
	}

	//Extraigo el numero de argumentos.
	ret=RuntimePopDouble(vValue);
	M4CHECK_RES(ret==M4_SUCCESS,M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "NumberOfArguments" << "ExecutingLN4JIT" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vValue.Type)<<DumpCallStack() );

	iNumArgs  = (m4int32_t)vValue.Data.DoubleData-1;//me vienen el código y los argumentos.

	if (iNumArgs<0){
		DUMP_CHLOG_ERRORF(M4_CH_VM_BAD_NUMBER_OF_ARGUMENTS, "clcExecuteLN4JIT" << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}

	iNumRealArgs = (iNumArgs/2);

	if (iNumArgs%2)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_BAD_NUMBER_OF_ARGUMENTS, "clcExecuteLN4JIT" << DumpCallStack() );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}

	pvArguments=new	m4VariantType[iNumRealArgs];
	pvValues=new	m4VariantType[iNumRealArgs];

	if (iNumRealArgs)
	{	
		//Extraigo los argumentos
		for (m4int32_t i=0;i<iNumRealArgs;i++)
		{
		//Extraigo el valor del argumento.
			ret=RuntimePop(vValue);
			if (ret!=M4_SUCCESS)
			{
				DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP, "Argument value in clcExecuteLN4JIT" );
				FreePoolStrings ( pvValues , i-1 );
				FreePoolStrings ( pvArguments, i-1 );
				delete []pvValues;
				delete []pvArguments;
				RuntimePushDouble (M4_ERROR);
				return M4_ERROR;
			}
			pvValues[i]=vValue;

			//Extraigo el nombre del argumento.
			ret=RuntimePop(vValue);

		    if (ret!=M4_SUCCESS)
			{
				DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP, "Argument name in clcExecuteLN4JIT" );
				FreePoolStrings ( pvValues , i );
				FreePoolStrings ( pvArguments, i-1 );
				delete []pvValues;
				delete []pvArguments;
				RuntimePushDouble (M4_ERROR);
				return M4_ERROR;
			}
			pvArguments[i]=vValue;
		}
	}

		//Extraigo el código:
	ret=RuntimePop(vValue);
	if (ret!=M4_SUCCESS)
	{
		DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP, "LN4 code in clcExecuteLN4JIT" );
		FreePoolStrings ( pvValues , iNumRealArgs);
		FreePoolStrings ( pvArguments, iNumRealArgs);
		delete []pvValues;
		delete []pvArguments;
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
	if (vValue.Type!=M4CL_CPP_TYPE_STRING_POOL)
		ret=m_oVariantMediator->ToPoolString(vValue);
	if (ret!=M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "LN4 code"<<"ExecuteLN4JIT"  << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
		FreePoolStrings ( pvValues , iNumRealArgs);
		FreePoolStrings ( pvArguments, iNumRealArgs);
		delete []pvValues;
		delete []pvArguments;
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;

	}


	pszTemp=m_oDynamicPool->GetString(vValue.Data.CharData);
	m4pchar_t pszPolacaJIT=0;
	
	ret=CompileJIT(pszTemp,pvArguments, iNumRealArgs,pszPolacaJIT);

	FreePoolStrings ( pvArguments, iNumRealArgs);
	m_oDynamicPool -> DeleteString (vValue.Data.CharData );
	delete [] pvArguments;
	
	if (ret!=M4_SUCCESS)
	{
		FreePoolStrings ( pvValues , iNumRealArgs);
		//delete [] pszPolacaJIT;
		delete []pvValues;
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
 
	m4pchar_t pcInstalled=0;
	ret=InstallJIT(pszPolacaJIT,pcInstalled);


	if (ret!=M4_SUCCESS||pcInstalled==NULL)
	{
		FreePoolStrings ( pvValues , iNumRealArgs);
		delete []pvValues;
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
//	El metodo no devuelve nada, devuelve lo que se ejcute
//	RuntimePushDouble(M4_SUCCESS);

	ret=ExecuteJIT(pcInstalled, pvValues, iNumRealArgs, M4_FALSE);	

// No libero, porque los he pasado como argumentos, y pueden ser cadenas.
// Los libera el metodo que los coja.
// 

	if (ret!=M4_SUCCESS)
	{
	//	DUMP_CHLOG_ERRORF(M4_CH_VM_EXECUTING_JIT, "clcExecuteLN4JIT" << DumpCallStack() ); Ya sale mensaje por abajo
		FreePoolStrings ( pvValues , iNumRealArgs);
		delete []pvValues;
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}

	delete []pvValues;

	return M4_SUCCESS;
}

m4return_t ClVM1CompilerAndJIT::ExecuteJIT (m4pchar_t ai_CodeSerial,m4VariantType *ai_avArgValues,m4int32_t ai_iNumArgs,m4bool_t bPrepared)
{
	m4return_t ret=M4_SUCCESS;

	m4VMState_t	oState;


	
	oState.m_hItem=m_oState1.m_hItem;//¿COJO EL MISMO?

	oState.m_poAccess=m_oState1.m_poAccess;
	oState.m_oRegisterIndex=m_oState1.m_oRegisterIndex;
	oState.m_poRecordSet=m_oState1.m_poRecordSet;
	oState.m_dDate=m_oState1.m_dDate;
	oState.m_dBeginDate=m_oState1.m_dBeginDate;
	oState.m_dEndDate=m_oState1.m_dEndDate;

	oState.m_iRule=0;
	oState.m_Flags=0;
	M4CL_SET_STATE_HIDE_FLAG(oState.m_Flags); //para que cuando se muestre en el call stack no salga

	//oState.m_iSourceLine=m_oState1.m_iSourceLine;
	oState.m_iSourceLine=1;
	oState.m_iStepperType=StepperTypeNormal;

	
	oState.m_poFormula.Init(ai_CodeSerial);

    
	//Meter los argumentos.


	for (m4int32_t i=0;i<ai_iNumArgs;i++)
	{
		ret=RuntimePush(ai_avArgValues[i]);
	}
	
	// Metemos el número de argumentos, porque lo vamos a tratar como si fuera de
	// argumentos variables. Si no, a la hora de liberar, no podemos saber los
	// argumentos que le hemos pasado.
	RuntimePushDouble (ai_iNumArgs);

	oState.m_iOldStackReference=m_oRunTimeStack->GetReference();

    m_oRunTimeStack->SetReference(m_oRunTimeStack->GetPos()+1);

	oState.m_iCurrent=0;

	M4CL_SET_STATE_EXE_JIT_FLAG(oState.m_Flags);

	if (bPrepared==M4_TRUE){
		M4CL_SET_STATE_PREP_JIT_FLAG(oState.m_Flags);
	}

	oState.m_iParentStatePos=m_oStateStack->GetPos(); //metemos la posicion del item padre.
    
	ret=m_oStateStack->Push(oState);
	
	return ret;
}

m4return_t	ClVM1CompilerAndJIT::clcReleaseJIT(void)
{
	m4return_t	ret=M4_SUCCESS;
	m4VariantType vValue;
//	m4int32_t iHandle;
	m4bool_t bFound=M4_FALSE;

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1," clcReleaseJIT.");
#endif


	ret=RuntimePop(vValue);
	if (ret!=M4_SUCCESS)
	{
		DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP, "Handle in clcReleaseJIT" );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
	
	if (vValue.Type!=M4CL_CPP_TYPE_NUMBER)
	{
		ret=m_oVariantMediator->ToDouble(vValue);
		if (ret!=M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Handle"<<"ReleaseJIT"  << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vValue.Type)<<DumpCallStack() );
			RuntimePushDouble (M4_ERROR);
			return M4_ERROR;
		}
	}

	ret = m_oState1.m_poAccess -> GetpChannel() -> GetpCodeJIT() -> RemoveCodebyHandle ( (m4int32_t)vValue.Data.DoubleData );

	if ( ret != M4_SUCCESS )
	/////// Si no lo hemos podido borrar, damos warning, pero no cortamos la ejecución.
		DUMP_CHLOG_WARNINGF(M4_CH_VM_JIT_HANDLE_NOT_FOUND, DumpCallStack() );

	RuntimePushDouble(ret);
	return M4_SUCCESS;
}


//----------------------------------------------------------------
//PAra definir en JIT una instancia
//----------------------------------------------------------------
m4return_t ClVM1CompilerAndJIT::DefineInstance(void)
{
	m4VariantType vTemp, vChannel,vInstance;
	ClChannel * pChannel;

	m4return_t ret;
	m4bool_t bIns;
	StInstChannJIT_t StNewInstance;
	m4pchar_t pccInstance;

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1," DefineInstance.");
#endif


	// Shared
	ret = RuntimePop (vTemp);
	DEBUG_CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error extracting Shared Policy")	;

	if ( vTemp.Type != M4CL_CPP_TYPE_NUMBER ){
		ret = m_oVariantMediator->ToDouble (vTemp);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Shared Policy" << "defineInstance" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );
	}
	StNewInstance.m_iShared = (m4uint8_t )vTemp.Data.DoubleData;
	//chequeamos que esté en rango
	if (StNewInstance.m_iShared  > M4CL_T3ALIAS_OPEN_SCOPE_MAX|| StNewInstance.m_iShared  < M4CL_T3ALIAS_OPEN_SCOPE_MIN ){
		//asumimos valor por def y warning
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Shared Policy"<<"defineInstance"<<StNewInstance.m_iShared  << DumpCallStack() );
		StNewInstance.m_iShared  = M4CL_T3ALIAS_OPEN_SCOPE_NOT_SHARED;
	}

	//Load
	ret = RuntimePop (vTemp);
	DEBUG_CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error extracting Load Policy")	;

	if ( vTemp.Type != M4CL_CPP_TYPE_NUMBER ){
		ret = m_oVariantMediator->ToDouble (vTemp);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Load Policy" << "defineInstance" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );
	}
	StNewInstance.m_iLoad = ( m4uint8_t )vTemp.Data.DoubleData;
	//chequeamos que esté en rango
	if (StNewInstance.m_iLoad > M4CL_T3ALIAS_OPEN_MODE_MAX|| StNewInstance.m_iLoad < M4CL_T3ALIAS_OPEN_MODE_MIN ){
		//asumimos valor por def y warning
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Load Policy"<<"defineInstance"<<StNewInstance.m_iLoad << DumpCallStack() );
		StNewInstance.m_iLoad = M4CL_T3ALIAS_OPEN_MODE_NO_LOAD;
	}

	//Channel Name
	ret = RuntimePop (vChannel);
	DEBUG_CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error extracting Channel name")	;
	
	if (vChannel.Type != M4CL_CPP_TYPE_STRING_POOL ){
		ret = m_oVariantMediator->ToPoolString (vChannel);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Channel Name" << "defineInstance" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vChannel.Type)<<DumpCallStack() );
	}
	strncpy ( StNewInstance.m_acChannel, strupr (m_oDynamicPool->GetString (vChannel.Data.CharData)), M4CL_MAX_LEN_CHANNEL);
	StNewInstance.m_acChannel[ M4CL_MAX_LEN_CHANNEL ]=0; //nulterminamos pa porsi

	if (strlen (m_oDynamicPool->GetString (vChannel.Data.CharData) ) >M4CL_MAX_LEN_CHANNEL){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_ID_TOO_LONG, m_oDynamicPool->GetString (vChannel.Data.CharData)<<M4CL_MAX_LEN_CHANNEL<<"DefineInstance"<<DumpCallStack() );
	}

	//Instance
	ret = RuntimePop (vInstance);

#ifdef _DEBUG
	if ( ret != M4_SUCCESS )
	{
		m_oDynamicPool -> DeleteString (vChannel.Data.CharData );
		DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP, "Error extracting instance name" );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
#endif

	if ( vInstance.Type != M4CL_CPP_TYPE_STRING_POOL )
	{

		ret = m_oVariantMediator->ToPoolString (vInstance );
		if ( ret != M4_SUCCESS )
		{
			m_oDynamicPool -> DeleteString (vChannel.Data.CharData );
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Instance Name"<<"DefineInstance"  << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vInstance.Type)<<DumpCallStack() );
			RuntimePushDouble (M4_ERROR);
			return M4_ERROR;
		}
	}

	pccInstance = strupr (m_oDynamicPool->GetString (vInstance.Data.CharData));

	pChannel = m_oState1.m_poAccess->GetpChannel();
	

	//valores por defecto
	StNewInstance.m_iCsAccess= M4CL_CSTYPE_L2_SITE;
	StNewInstance.m_iOrgType=M4CL_ORGANIZATION_L2_TYPE_FATHER;
	StNewInstance.m_acOrg[0]='\0';
	StNewInstance.m_acRole[0] = NULL ;
	//el lenguage de datos por defecto de la instancia que definimos, es igual al del
	// canal desde donde se define.
	pChannel->Data_Language.Get(vTemp);
	StNewInstance.m_iDataLanguage = (m4language_t) vTemp.Data.DoubleData;

	/* Bug 0069089
	Hay que pasar un lenguaje válido
	*/
	M4ClLangInfo::Instance()->CheckAndSetValidLangId( StNewInstance.m_iDataLanguage ) ;
	
	bIns=_DefineInstance(pChannel, pccInstance, StNewInstance);

	if (bIns != M4_TRUE ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_DEFINE_INSTANCE, pccInstance<<StNewInstance.m_acChannel << DumpCallStack() );
			ret=M4_ERROR;
	}

	m_oDynamicPool -> DeleteString (vChannel.Data.CharData );
	m_oDynamicPool -> DeleteString (vInstance.Data.CharData );
	
	RuntimePushDouble (ret);
	return ret;
}


//---------------------------------------------------------------------
//PAra definir en JIT una instancia, pudiento especificar multisociedad
//---------------------------------------------------------------------
m4return_t ClVM1CompilerAndJIT::DefineInstanceEx(void)
{
	m4VariantType vTemp, vChannel,vInstance, vOrg, vRole, vNum;
	ClChannel * pChannel;
	m4return_t ret;
	m4bool_t bIns;
	StInstChannJIT_t StNewInstance;
	m4pchar_t pccInstance;
	m4int32_t	iNumArgs,i;

#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF(M4_CH_VM_TRACE_VM_L1," DefineInstanceEx.");
#endif

	pChannel = m_oState1.m_poAccess->GetpChannel();
	

    //Recuperamos de la pila los parametros
	
	//número de parámetros
    ret=RuntimePopDouble(vNum);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DefineInstanceEx.")	;
	
    iNumArgs= (m4int32_t)vNum.Data.DoubleData ;

    //demasiados parametros 
    if ( iNumArgs > 9 )
	{
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "DefineInstanceEx"<<9<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 9 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DefineInstanceEx")	;
        }
        iNumArgs = 9; //ya hemos sacado los sobrantes
    }

	//Role de apliación
	if ( iNumArgs == 9 )
	{ 
		//sacamos argumento
		ret = RuntimePop (vRole);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DefineInstanceEx.")	;
		
		//chequeamos tipo 
		if (vRole.Type != M4CL_CPP_TYPE_STRING_POOL){
			ret = m_oVariantMediator->ToPoolString(vRole);
		}
		if (ret != M4_SUCCESS){ 
			DUMP_CHLOG_ERRORF ( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Role ID"<<"DefineInstanceEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vRole.Type)<<DumpCallStack() );
			RuntimePushDouble ( M4_ERROR );
			return M4_ERROR;
		}

		strncpy( StNewInstance.m_acRole, strupr (m_oDynamicPool->GetString(vRole.Data.CharData)), M4CL_MAX_ROLE_ID ); 
		StNewInstance.m_acRole[M4CL_MAX_ROLE_ID] = 0; //lo nulterminamos

		if (strlen (m_oDynamicPool->GetString (vRole.Data.CharData) ) > M4CL_MAX_ROLE_ID){
			DUMP_CHLOG_WARNINGF(M4_CH_VM_ID_TOO_LONG, m_oDynamicPool->GetString (vRole.Data.CharData)<<M4CL_MAX_ROLE_ID<<"DefineInstanceEx"<<DumpCallStack() );
		}

		//para evaluar el siguiente arg
		--iNumArgs;
	}
	else
	{
		//Valor por defecto
		StNewInstance.m_acRole[0] = NULL ;
	}

	//--lenguaje de datos del canal--
	if (iNumArgs==8){ //indican Access type
		//sacamos argumento
		ret = RuntimePop (vTemp);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DefineInstanceEx.")	;
		
		//chequeamos tipo 
		if (vTemp.Type!=M4CL_CPP_TYPE_NUMBER){
			ret=m_oVariantMediator->ToDouble (vTemp);
		}
		if (ret!=M4_SUCCESS){ 
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Data Language"<<"DefineInstanceEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );
			RuntimePushDouble(M4_ERROR);
			return M4_ERROR;
		}

		m4language_t iDataLanguage;
		ClChannelManager * poChannelManager = 0;

		iDataLanguage = (m4language_t) vTemp.Data.DoubleData;
		poChannelManager = m_oState1.m_poAccess->GetpChannel()->GetpChannelManager();

		if(iDataLanguage == M4CL_LANG_LOGON) {
			iDataLanguage = poChannelManager->GetLogonAdaptor()->GetSessionLanguage("",M4CL_LANGUAGE_NONE);
		}

		if(iDataLanguage == M4CL_LANG_DEVELOPMENT) {
			iDataLanguage = poChannelManager->GetLogonAdaptor()->GetSessionDevLanguage("",M4CL_LANGUAGE_NONE);
		}

		StNewInstance.m_iDataLanguage= iDataLanguage;
		
		//para evaluar el siguiente arg
		--iNumArgs;
	}
	else{//valor por defecto
		//el lenguage de datos por defecto de la instancia que definimos, es igual al del
		// canal desde donde se define.
		pChannel->Data_Language.Get(vTemp);
		StNewInstance.m_iDataLanguage = (m4language_t) vTemp.Data.DoubleData;

		/* Bug 0069089
		Hay que pasar un lenguaje válido
		*/
		M4ClLangInfo::Instance()->CheckAndSetValidLangId( StNewInstance.m_iDataLanguage ) ;
	}

	//--cs access type--
	if (iNumArgs==7){ //indican Access type
		//sacamos argumento
		ret = RuntimePop (vTemp);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DefineInstanceEx.")	;
		
		//chequeamos tipo 
		if (vTemp.Type!=M4CL_CPP_TYPE_NUMBER){
			ret=m_oVariantMediator->ToDouble (vTemp);
		}
		if (ret!=M4_SUCCESS){ 
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "CS Access Type"<<"DefineInstanceEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );
			RuntimePushDouble(M4_ERROR);
			return M4_ERROR;
		}

		StNewInstance.m_iCsAccess=(m4uint8_t) vTemp.Data.DoubleData;
		
		//chequeamos que esté en rango
		if (StNewInstance.m_iCsAccess > M4CL_CSTYPE_L2_MAX_NUMBER|| StNewInstance.m_iCsAccess < M4CL_CSTYPE_L2_MIN_NUMBER ){
			//asumimos valor por def y warning
			DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "CS Access Type"<<"DefineInstanceEx"<<StNewInstance.m_iCsAccess << DumpCallStack() );
			StNewInstance.m_iCsAccess=M4CL_CSTYPE_L2_SITE;
		}

		//para evaluar el siguiente arg
		--iNumArgs;
	}
	else{//valor por defecto M4CL_CSTYPE_L2_SITE
			StNewInstance.m_iCsAccess=M4CL_CSTYPE_L2_SITE;

	}

	//--tipo de organización--
	if (iNumArgs==6){ //indican Tipo de organización,
		//sacamos argumento
		ret = RuntimePop (vTemp);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DefineInstanceEx.")	;
		
		//chequeamos tipo 
		if (vTemp.Type!=M4CL_CPP_TYPE_NUMBER){
			ret=m_oVariantMediator->ToDouble (vTemp);
		}
		if (ret!=M4_SUCCESS){ 
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "OrganizationType"<<"DefineInstanceEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );
			RuntimePushDouble(M4_ERROR);
			return M4_ERROR;
		}

		StNewInstance.m_iOrgType = (m4uint8_t) vTemp.Data.DoubleData;
		
		//chequeamos que esté en rango
		if (StNewInstance.m_iOrgType > M4CL_ORGANIZATION_L2_MAX|| StNewInstance.m_iOrgType < M4CL_ORGANIZATION_L2_MIN ){
			//asumimos valor por def y warning
			DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "OrganizationType"<<"DefineInstanceEx"<<StNewInstance.m_iOrgType<< DumpCallStack() );
			StNewInstance.m_iOrgType = M4CL_ORGANIZATION_L2_TYPE_FATHER;
		}
	
		--iNumArgs;
	}
	else{//valor por defecto M4CL_ORGANIZATION_L2_TYPE_FATHER
		StNewInstance.m_iOrgType = M4CL_ORGANIZATION_L2_TYPE_FATHER;
	}


	//--organización--
	if (iNumArgs==5){ //indican Organización
		ret = RuntimePop (vOrg);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DefineInstanceEx.")	;
		
		//chequeamos tipo 
		if (vOrg.Type!=M4CL_CPP_TYPE_STRING_POOL){
			ret=m_oVariantMediator->ToPoolString(vOrg);
		}
		if (ret!=M4_SUCCESS){ 
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Organization"<<"DefineInstanceEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vOrg.Type)<<DumpCallStack() );
			RuntimePushDouble(M4_ERROR);
			return M4_ERROR;
		}
		strncpy(StNewInstance.m_acOrg, strupr (m_oDynamicPool->GetString(vOrg.Data.CharData)), M4CL_MAX_ORG_ID ); 
		StNewInstance.m_acOrg[M4CL_MAX_ORG_ID]=0; //lo nulterminamos
		if (strlen (m_oDynamicPool->GetString (vOrg.Data.CharData) ) >M4CL_MAX_ORG_ID){
			DUMP_CHLOG_WARNINGF(M4_CH_VM_ID_TOO_LONG, m_oDynamicPool->GetString (vOrg.Data.CharData)<<M4CL_MAX_ORG_ID<<"DefineInstanceEx"<<DumpCallStack() );
		}

		--iNumArgs;
	}
	else{//valor por defecto NULL
		StNewInstance.m_acOrg[0]=0;
	}

	//los argumentos fijos!


	// --Shared--
	ret = RuntimePop (vTemp);
	DEBUG_CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error extracting Shared Policy")	;

	if ( vTemp.Type != M4CL_CPP_TYPE_NUMBER ){
		ret = m_oVariantMediator->ToDouble (vTemp);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Shared Policy" << "defineInstanceEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );
	}
	
	StNewInstance.m_iShared = (m4uint8_t )vTemp.Data.DoubleData;
	
	//chequeamos que esté en rango
	if (StNewInstance.m_iShared  > M4CL_T3ALIAS_OPEN_SCOPE_MAX|| StNewInstance.m_iShared  < M4CL_T3ALIAS_OPEN_SCOPE_MIN ){
		//asumimos valor por def y warning
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Shared Policy"<<"defineInstanceEx"<<StNewInstance.m_iShared  << DumpCallStack() );
		StNewInstance.m_iShared  = M4CL_T3ALIAS_OPEN_SCOPE_NOT_SHARED;
	}


	//--Load--
	ret = RuntimePop (vTemp);
	DEBUG_CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error extracting Load Policy")	;

	if ( vTemp.Type != M4CL_CPP_TYPE_NUMBER ){
		ret = m_oVariantMediator->ToDouble (vTemp);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Load Policy" << "defineInstanceEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );
	}
	
	StNewInstance.m_iLoad = ( m4uint8_t )vTemp.Data.DoubleData;
	
	//chequeamos que esté en rango
	if (StNewInstance.m_iLoad > M4CL_T3ALIAS_OPEN_MODE_MAX|| StNewInstance.m_iLoad < M4CL_T3ALIAS_OPEN_MODE_MIN ){
		//asumimos valor por def y warning
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Load Policy"<<"defineInstanceEx"<<StNewInstance.m_iLoad << DumpCallStack() );
		StNewInstance.m_iLoad = M4CL_T3ALIAS_OPEN_MODE_NO_LOAD;
	}


	//--Channel Name--
	ret = RuntimePop (vChannel);
	DEBUG_CHECK_RES (ret == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "Error extracting Channel name")	;
	
	if (vChannel.Type != M4CL_CPP_TYPE_STRING_POOL ){
		ret = m_oVariantMediator->ToPoolString (vChannel);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Channel Name" << "defineInstanceEx" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vChannel.Type)<<DumpCallStack() );
	}
	strncpy ( StNewInstance.m_acChannel, strupr (m_oDynamicPool->GetString (vChannel.Data.CharData)), M4CL_MAX_LEN_CHANNEL);
	StNewInstance.m_acChannel[ M4CL_MAX_LEN_CHANNEL ]=0; //nulterminamos pa porsi

	if (strlen (m_oDynamicPool->GetString (vChannel.Data.CharData) ) >M4CL_MAX_LEN_CHANNEL){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_ID_TOO_LONG, m_oDynamicPool->GetString (vChannel.Data.CharData)<<M4CL_MAX_LEN_CHANNEL<<"DefineInstanceEx"<<DumpCallStack() );
	}

	//--Instance--
	ret = RuntimePop (vInstance);

#ifdef _DEBUG
	if ( ret != M4_SUCCESS ){
		m_oDynamicPool -> DeleteString (vChannel.Data.CharData );
		DUMP_CHLOG_DEBUGF(M4_CH_VM_RTS_POP, "Error extracting instance name" );
		RuntimePushDouble (M4_ERROR);
		return M4_ERROR;
	}
#endif

	if ( vInstance.Type != M4CL_CPP_TYPE_STRING_POOL ){

		ret = m_oVariantMediator->ToPoolString (vInstance );
		if ( ret != M4_SUCCESS ){
			m_oDynamicPool -> DeleteString (vChannel.Data.CharData );
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Instance Name"<<"DefineInstance"  << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vInstance.Type)<<DumpCallStack() );
			RuntimePushDouble (M4_ERROR);
			return M4_ERROR;
		}
	}

	pccInstance = strupr (m_oDynamicPool->GetString (vInstance.Data.CharData));

	bIns=_DefineInstance(pChannel, pccInstance, StNewInstance);

	if (bIns != M4_TRUE ){
			DUMP_CHLOG_ERRORF(M4_CH_VM_DEFINE_INSTANCE, pccInstance<<StNewInstance.m_acChannel << DumpCallStack() );
			ret=M4_ERROR;
	}

	m_oDynamicPool -> DeleteString (vChannel.Data.CharData );
	m_oDynamicPool -> DeleteString (vInstance.Data.CharData );
	DeleteVarString (vOrg);
	DeleteVarString (vRole);

	RuntimePushDouble (ret);
	return ret;
}


//-------------------
//GetSize
//-------------------
m4uint32_t ClVM1CompilerAndJIT::GetSize (void) const{
	m4uint32_t length =0;
	if (m_poTrad)
        length += m_poTrad->GetSize ();
	if (m_poInterpreter)
        length += m_poInterpreter->GetSize ();
	length +=sizeof (ClVM1CompilerAndJIT);
	return length ;
}


m4return_t ClVM1CompilerAndJIT::InitTrad(m4uint8_t ai_Type,ClTrad *&ao_pTrad)
{

	ClTrad * pTrad=0;

	if (m_IsFreeTrad == M4_TRUE)
	{
		if (!m_poTrad)
			m_poTrad = new ClTrad ();
		pTrad = m_poTrad;
	}
	else // Creamos uno nuevo. Ya hay alguine compilando.
	{
		ao_pTrad = new ClTrad ();
		pTrad = ao_pTrad;
	}
	ao_pTrad = pTrad;
	pTrad -> UpdateFinder (GetFinder(ai_Type,pTrad));
	m_IsFreeTrad=M4_FALSE;
	return M4_SUCCESS;

}

ClFinder * ClVM1CompilerAndJIT::GetFinder(m4uint8_t ai_Type,ClTrad *ai_pTrad)
{
	ClFinder *pFinder=0;
	if (m_IsFreeTrad == M4_TRUE)
	{
		if (ai_Type == M4CL_FINDER_TICH)
		{
			if (!m_poFinderTICH)
				m_poFinderTICH = new ClFinder_TICH(ai_pTrad);
				
			return m_poFinderTICH;
		}
		else 
		{
			// Mientras que no haya más tipos.
			M4_ASSERT (ai_Type == M4CL_FINDER_JIT);

			if (!m_poFinderJIT)
				m_poFinderJIT = new ClFinder_JIT(ai_pTrad);

			return m_poFinderJIT;

		}
	}
	else 
	{
		if (ai_Type == M4CL_FINDER_TICH)
			pFinder = new ClFinder_TICH(ai_pTrad);
		else 
			pFinder = new ClFinder_JIT(ai_pTrad);
				
		return pFinder;
	}

}

void ClVM1CompilerAndJIT::FreeTrad(ClTrad *ai_pTrad)
{
	ai_pTrad->Reset();
	if (ai_pTrad != m_poTrad)
	{
		M4_ASSERT(ai_pTrad->GetpFinder());
		delete ai_pTrad->GetpFinder();
		delete ai_pTrad;
	}
	else 
		m_IsFreeTrad = M4_TRUE;
}


m4return_t ClVM1CompilerAndJIT::InstallRTGC	(ClInstruction *ai_pInstructions, m4uint32_t ai_iNumInstructions,  m4uint32_t *ai_piInsTypes, m4uint32_t ai_iInsTypesNum){

	m4return_t ret=M4_SUCCESS;
	m4int32_t iNumInstLN4=0,iNumInstFormula=0;
	m4uint32_t iSize=0,iUpperSize,iNoUpperSize;
	m4pchar_t pszCodeSerial;

	if (m_oUpperPool==NULL)
		m_oUpperPool=new ClStaticPoolStr(M4_VM_STATIC_POOL_SIZE_FOR_JIT,M4CL_UPPERCASE);

	if (m_oNoUpperPool==NULL)
		m_oNoUpperPool=new ClStaticPoolStr(M4_VM_STATIC_POOL_SIZE_FOR_JIT);


	ClTempNitemFormula  oTempNItemFormula;

	oTempNItemFormula.SetUpperPool( m_oUpperPool ) ;
	oTempNItemFormula.SetNoUpperPool( m_oNoUpperPool ) ;
	
	oTempNItemFormula.SetInstructionsRTGC(ai_pInstructions,ai_iNumInstructions,ai_piInsTypes,ai_iInsTypesNum); 
	oTempNItemFormula.SetInfoRTGC(0,M4CL_LENGUAGE_LN4,M4CL_LENGUAGE_LN4, 0,M4CL_RULE_MIN_PRIORITY); 

	oTempNItemFormula.GetSize(iSize,NULL);

	iUpperSize=m_oUpperPool->GetLength();
	iNoUpperSize=m_oNoUpperPool->GetLength();

	pszCodeSerial=new m4char_t[iSize+iUpperSize+iNoUpperSize+1];		//Se destruye al acabar la ejecución, si procede.

	if (pszCodeSerial!=NULL){	
		ret=oTempNItemFormula.Compile(pszCodeSerial, iSize,iUpperSize);
		oTempNItemFormula.ResetInstructionsRTGC(); //para que no se quede con na referencia al puntero de instrucciones y el destructor borre el array
	}
	else{
		return M4_ERROR;
	}


	if (ret!=M4_SUCCESS)
	{
		delete []pszCodeSerial;
		return M4_ERROR;
	}

	m_oUpperPool->Copy(pszCodeSerial,iSize);
	m_oNoUpperPool->Copy(pszCodeSerial,iSize+iUpperSize);

	m_oUpperPool->Reset();
	m_oNoUpperPool->Reset();
	
	//LLLLLLLLLLLLLAMAMSO A EJECUTAR DESDE AQUI!!!!!
	ret=ExecuteJIT(pszCodeSerial,0,0,M4_FALSE);


	return ret;
}



m4bool_t		ClVM1CompilerAndJIT::_DefineInstance (ClChannel *ai_poChannel, const m4char_t *ai_pszInstance, const StInstChannJIT_t &ai_stNewInstance){
	
	m4bool_t bIns;
	
	// Si la instancia ya está definida (o definida y levantada) se borra
	// y se pone la nueva definición
	// Si no estaba definida se mete la nueva y ya está

	bIns = M4_BOOL( ai_poChannel->Level2.m_oChannelInstJIT.insert ( MapChannInstJIT::value_type ( ai_pszInstance , ai_stNewInstance)).second ) ;
	if ( bIns != M4_TRUE ){
	
		// Para el caso en que haya que cerrar la instancia si ya estuviera levantada.

		ai_poChannel->Level2.m_oChannelInstJIT.erase ( ai_poChannel->Level2.m_oChannelInstJIT.find (ai_pszInstance));
		bIns = M4_BOOL( ai_poChannel->Level2.m_oChannelInstJIT.insert ( MapChannInstJIT::value_type ( ai_pszInstance , ai_stNewInstance)).second );

	}
	return bIns;

}


m4return_t ClVM1CompilerAndJIT::GetSymbolTable(ClItem *ai_poItem,  m4char_t *ai_pszLN4Code, m4char_t **ai_ppszArguments, m4uint32_t ai_iNumArgs,m4char_t **ai_ppszSymbols, m4uint32_t ai_iNumSymbols, m4uint32_t ai_iSymbolSize){
	
	m4return_t	ret=M4_SUCCESS;
	
	// corrige bug 0061238
	ClAccess *poAccess = ai_poItem->GetpAccess();
	ClCompiledMCR *poCMCR = poAccess->GetpCMCR();
	ClAccessRecordSet *poRS = &ai_poItem->GetpNodeAccess()->RecordSet;
	m4uint32_t hItem = ai_poItem->ItemDef.GetHandle();

	//Bug: 63226  Begin -------------------------
	//No tiene sentido pedir la tabla de símbolos y no dar un contenedor para el resultado
	if(ai_iNumSymbols <= 0)
		return M4_ERROR;
	//Bug: 63226  End ---------------------------

	ClTrad *pTrad = 0;
	if (InitTrad (M4CL_FINDER_JIT,pTrad)!=M4_SUCCESS) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_PREPARING_TO_COMPILE, M4ObjNodeN(poCMCR,poRS->GetpNodeDef()->GetHandle() ) );
		return M4_ERROR;
	}

	// Bug 0069036 Ojo, que aquí se pone el nivel de herencia más bajo y no de la regla
	// por que no está disponible hasta que VB lo pase
	if ( pTrad->SetAccess (poAccess, hItem, 0 ) != M4_SUCCESS )	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_BLOCK,"CompilerFunction: clcCompileJIT"<<poCMCR->iNodeN(poRS->GetpNodeDef()->GetHandle())<<LogCatAnd<<poCMCR->nNodeN(poRS->GetpNodeDef()->GetHandle())<<DumpTreePath(poAccess, poRS->GetpNodeDef()->GetHandle()) << DumpCallStack() );
		FreeTrad(pTrad);
        return M4_ERROR;
	}

    pTrad->SetRecordSet(poRS);

	m4bool_t *pbReference=new m4bool_t[ai_iNumArgs];
	
	memset(pbReference,0, sizeof(m4bool_t)*ai_iNumArgs);

	pTrad->SetArguments(ai_ppszArguments,pbReference,(m4int8_t) ai_iNumArgs,M4_TRUE);

	delete[]pbReference;

	//En los mensajes de error del restore source no sacaremos información sobre el item que se está compialndo, ya que solo tenenmos el ´códigoFuente
	pTrad->SetCurrentRuleInfo(M4_FALSE,"","","","");
	
	pTrad->SetSourceMode(M4_GET_SYMBOLS); 
	
	pTrad->SetSymbolsTableSpace(ai_ppszSymbols,ai_iNumSymbols,ai_iSymbolSize); //le decimos al trad donde poner los simbolos
	
	//OJO: NO ponemos la información sobre el item que vamos a compilar porque no se ejecuta a través de VM sino llamada directa 
	//desde el ejecutor, así que no hay nada que se esté ejecutando

	ret=pTrad->Compile(ai_pszLN4Code);
	
	//reseteamos la información sobre la current rule
	pTrad->SetCurrentRuleInfo(M4_FALSE,"","","","");

	pTrad->SetSymbolsTableSpace(0,0,0); //quitamos como destino de los simbolos ese buffer
	
	FreeTrad(pTrad);

	return ret;
}