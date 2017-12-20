

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
//    Virtual machine de nivel 1- Funciuones de registro y recordset
////
//==============================================================================

#include <errno.h>
#include <string.h>

#include "vm1reg.hpp"
#include "vmres.hpp"
#include "vmachin1.h"
#include "m4log.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "m4mdrt.hpp"
#include "node_knl.hpp"
#include "chan_knl.hpp"
#include "dump_vst.hpp"
#include "meminfo.hpp"
#include "m4win2ux.hpp"
#include "m4objglb.hpp"
#include "clm4objservicecontext.hpp"
#include "m4objreg.hpp"
#include "rec_tran.hpp"
#include "m4filemanager.hpp"
#include "cldbgutil.hpp"
#include "m4unicode.hpp"


//Volcar la estadísticas a partir de un nivel dado.
//Nota: Algoritmo recursivo -> Estaría bien pasarlo a iterativo
//		La recursion finaliza cuando el nivel es 0

//Entrada:	aio_ofreg			= Stream de Salida de la informacion.
//			ai_poAccess			= Access del Canal sobre el que se obtendrán las estadìsticas.
//			ai_iLevel			= Nivel de Profundidad al que se obtendrán las estadísticas (>=0)
//Salida:	ao_oChannelMemInfo	= Estructura con el resumen de la informacion del Canal obtenida.
m4return_t DumpChannelStatistics(ostream& aio_ofreg, ClAccess* ai_poAccess, m4uint32_t ai_iLevel, stChannelMemInfo_t& ao_oChannelMemInfo)
{
	m4return_t ret;
	m4uint32_t i;

	//Validación de argumentos de entrada
	if (!ai_poAccess)
		return M4_ERROR;

	ClChannel		*pChannel		= ai_poAccess->GetpChannel();	//Canal en que estamos
	if (!pChannel)
		return M4_ERROR;

	ClCompiledMCR   *poCMCR			= ai_poAccess->GetpCMCR();		//Metacanal del Canal en que estamos
	if (!poCMCR)
		return M4_ERROR;

	m4pcchar_t		 pszIdChannel	= poCMCR->GetChannelId();	//Id. del Canal en que estamos
	if (!pszIdChannel)
		return M4_ERROR;
	

	//Informacion de memoria ocupada por el Canal
	ClChannelMemInfo	oMemInfo;			//Objeto para medir la memoria
	ao_oChannelMemInfo.Reset();				//Inicializo objeto de estadisticas
	ret=oMemInfo.GetMemInfo(ai_poAccess, ao_oChannelMemInfo);
	if (ret!=M4_SUCCESS)
		return M4_ERROR;

	//Dump cabecera de Listado + info
	aio_ofreg << endl<<"#######----- Begin " << pszIdChannel   << " -----######" << endl<<endl;
	aio_ofreg << "M4Object Memory Info: "<< pszIdChannel << endl;
	aio_ofreg << ao_oChannelMemInfo;
	aio_ofreg << "------------------------------------------------" << endl;

	//obtenemos e imprimimos información de todos los nodos del canal
	stNodeMemInfo_t oNodeInfo;
	ClNode			*oCurrentNode;

	for (i=0; i<ai_poAccess->Node.Count();++i){
		oCurrentNode=ai_poAccess->Node.Get(ClNodeIndex(i));
		ret=oMemInfo.GetMemInfo(oCurrentNode, oNodeInfo);
		if (ret!=M4_SUCCESS)
			return M4_ERROR;
		aio_ofreg << "Node Memory Info: "<< oCurrentNode->GetpNodeDef()->Id() << endl;
		aio_ofreg<<oNodeInfo;
		aio_ofreg << "------------------------------------------------" << endl;

	}

	//PRINT info niveles inferiores + RESUMEN de info niveles inferiores
	if (ai_iLevel>0) {

		aio_ofreg << "##---------- Begin Level2: " << pszIdChannel << " ---------##" << endl;

		--ai_iLevel;	//Decrementamos un Nivel

		//Informacion de memoria ocupada por Level 2
		//Nota: Ahora profundizamos recursivamente en el arbol (de forma limitada), y ojo porque se podrian
		//		dar ciclos.
		stChannelMemInfo_t oChannelMemInfoAcuL2;//Estructura con el acumulado de la informacion del Nivel 2
		stChannelMemInfo_t oChannelMemInfoL2;	//Estructura con la informacion de un access de Nivel 2

		m4uint32_t iNAccessErroneos=0;	//No. de Access Erroneos
		m4uint32_t iNAccess = pChannel->Level2.m_oInstanceMap.Count();	//No. de Access para este Canal
		ClAccess *poAccessL2;

		//Recorrido por todos los Access de Nivel 2
		for (m4uint32_t j=0; j<iNAccess; ++j) {
			poAccessL2 = pChannel->Level2.m_oInstanceMap.GetAccess(j);
			ret=DumpChannelStatistics(aio_ofreg, poAccessL2, ai_iLevel, oChannelMemInfoL2);
			if (ret==M4_SUCCESS){
				oChannelMemInfoAcuL2 += oChannelMemInfoL2;	//Acumulo la informacion de mi nivel 2
			}else{
				++iNAccessErroneos;							//Cuento los Access que me han fallado al darme la Info
			}
		}
		aio_ofreg << "##---------- End   Level2: " << pszIdChannel << " ---------##" << endl;

		//Vuelco al finalizar "Info Resumen" de Nivel 2
		aio_ofreg << "M4Object Memory Info Level2: "<< pszIdChannel << endl;
		aio_ofreg << "No. Access: "		<< iNAccess	<< endl;
		aio_ofreg << oChannelMemInfoAcuL2;
		aio_ofreg << "------------------------------------------------" << endl;
		if (iNAccessErroneos>0) {
			aio_ofreg << "No. Access sin Info: "	<< iNAccessErroneos	 << endl;
		}

	}	//endif (ai_iLevel>0)

	aio_ofreg << endl<<"#######----- End   " << pszIdChannel << " -----######" << endl<<endl;

	return M4_SUCCESS;
}


//-------------------------------------------------
//Estado de los registros
//-------------------------------------------------
m4return_t  ClVM1Register::IsDeleted        (void){
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: IsDeleted");
#endif

    m4bool_t bret=m_oState1.m_poRecordSet->Current.Operation.IsDeleted();
	RuntimePushDouble( bret);
	return M4_SUCCESS;
}


m4return_t  ClVM1Register::IsNew       (void){
	
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: IsNew");
#endif
	
    m4bool_t bret=m_oState1.m_poRecordSet->Current.Operation.IsNew();
	RuntimePushDouble( bret);
	return M4_SUCCESS;
}


m4return_t  ClVM1Register::IsUpdated        (void){
	
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: IsUpdated");
#endif

    m4bool_t bret=m_oState1.m_poRecordSet->Current.Operation.IsUpdated();
	RuntimePushDouble( bret);
	return M4_SUCCESS;
}


m4return_t  ClVM1Register::IsNewAndDeleted        (void){
	
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: IsNewAndDeleted");
#endif

    m4bool_t bret=m_oState1.m_poRecordSet->Current.Operation.IsNewAndDeleted();
	RuntimePushDouble( bret);
	return M4_SUCCESS;
}


m4return_t  ClVM1Register::IsModifiedByDDBB        (void){
    
#if _M4_DEBUG > 10   
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: IsModifiedByDDBB");
#endif

    m4bool_t bret=m_oState1.m_poRecordSet->Current.Operation.IsUpdatedByDDBB();
	RuntimePushDouble( bret);
	return M4_SUCCESS;
}


m4return_t  ClVM1Register::IsBlockUpdated        (void){
	
	m4return_t ret;
	m4VariantType vTemp;
	m4bool_t bret;

#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: IsBlockUpdated");
#endif

	ret=RuntimePop(vTemp);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "IsBlockUpdated")	;
	
	ret=m_oVariantMediator->ToDouble(vTemp);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "Recursive" << "IsBlockUpdated" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );

	if( vTemp.Data.DoubleData == M4_TRUE )
	{
	    bret=m_oState1.m_poRecordSet->Operation.IsUpdated() ;
	}
	else
	{
	    bret=m_oState1.m_poRecordSet->GetCurrentBlock()->B_IsUpdated() ;
	}
	RuntimePushDouble( bret);
	return M4_SUCCESS;
}


m4return_t  ClVM1Register::IsNodeUpdated        (void){
	
	m4return_t ret;
	m4VariantType vTemp;
	m4bool_t bret;

#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: IsNodeUpdated");
#endif

	ret=RuntimePop(vTemp);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "IsNodeUpdated")	;
	
	ret=m_oVariantMediator->ToDouble(vTemp);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "Recursive" << "IsNodeUpdated" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );

	if( vTemp.Data.DoubleData == M4_TRUE )
	{
	    bret=m_oState1.m_poRecordSet->GetpNode_Data()->Operation.IsUpdated() ;
	}
	else
	{
	    bret=m_oState1.m_poRecordSet->GetpNode_Data()->GetpRWT()->IsRecordModified(0) ;
	}
	RuntimePushDouble( bret);
	return M4_SUCCESS;
}


m4return_t  ClVM1Register::SetInserted        (void){
    
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SetInserted");
#endif

    m4return_t ret=m_oState1.m_poRecordSet->Current.Operation.SetUpdated();
	RuntimePushDouble( ret);
	return M4_SUCCESS;
}


m4return_t  ClVM1Register::GetNewNotPersist (void){
    
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetNewNotPersist");
#endif

    m4bool_t bret=m_oState1.m_poRecordSet->Current.GetNewNotPersist();
	RuntimePushDouble( bret);
	return M4_SUCCESS;
}


m4return_t  ClVM1Register::SetNewNotPersist (void){
	
	m4return_t ret;
	m4VariantType vTemp;
	m4bool_t bAux;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SetNewNotPersist");
#endif

	ret=RuntimePop(vTemp);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SetNewNotPersist.")	;
	
	ret=m_oVariantMediator->ToDouble(vTemp);
    bAux = (vTemp.Data.DoubleData) ? M4_TRUE: M4_FALSE;
    ret = m_oState1.m_poRecordSet->Current.SetNewNotPersist (bAux);
    
    RuntimePushDouble(ret);
	return M4_SUCCESS;
}

//-------------------------------------------------------
// Las de  R E G I S T R O S 
//-------------------------------------------------------

m4return_t ClVM1Register::AddRegister(void){
	
	m4return_t ret;
	
    
#if _M4_DEBUG > 10   
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: AddRegister");
#endif

    ret = m_oState1.m_poRecordSet->Current.Add();
	RuntimePushDouble( ret);
	return ret;
}


m4return_t ClVM1Register::InsertRegister(void){
	
	m4return_t ret;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: InsertRegister");
#endif

	ret = m_oState1.m_poRecordSet->Current.Insert();
	RuntimePushDouble( ret);
	return ret;
}


m4return_t ClVM1Register::DeleteRegister(void){
    
#if _M4_DEBUG > 10   
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: DeleteRegister");
#endif

	m4return_t ret=M4_WARNING;
	if (m_oState1.m_poRecordSet->Current.Index != M4DM_EOF_INDEX) {
		ret = m_oState1.m_poRecordSet->Current.Delete();
	}
	
	RuntimePushDouble( ret);
	return M4_SUCCESS;
}


m4return_t ClVM1Register::DestroyRegister(void){
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: DeleteRegister");
#endif

	m4return_t ret=M4_WARNING;
	if (m_oState1.m_poRecordSet->Current.Index != M4DM_EOF_INDEX) {
		ret = m_oState1.m_poRecordSet->Current.Destroy();
	}
	
	RuntimePushDouble( ret);
	return M4_SUCCESS;
}


m4return_t ClVM1Register::Begin(void){
	
	m4return_t ret=M4_SUCCESS;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Begin");
#endif

    RuntimePushDouble ( m_oState1.m_poRecordSet->Current.Begin() );
	return M4_SUCCESS;
}


m4return_t ClVM1Register::End(void){
	
	m4return_t ret=M4_SUCCESS;
	
#if _M4_DEBUG > 10   
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: End");
#endif

	RuntimePushDouble ( m_oState1.m_poRecordSet->Current.End() );
	return M4_SUCCESS;
}


m4return_t ClVM1Register::Next(void){
	
	m4return_t ret=M4_SUCCESS;
	
#if _M4_DEBUG > 10   
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Next");
#endif

    RuntimePushDouble ( m_oState1.m_poRecordSet->Current.Next() );
	return M4_SUCCESS;
}


m4return_t ClVM1Register::Previous(void){
	
	m4return_t ret=M4_SUCCESS;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Previous");
#endif
	
    RuntimePushDouble ( m_oState1.m_poRecordSet->Current.Prev() );
	return M4_SUCCESS;
}


m4return_t ClVM1Register::MoveToEOF(void){
	
	m4return_t ret=M4_SUCCESS;
	
#if _M4_DEBUG > 10  
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: MoveToEOF");
#endif
	
    m_oState1.m_poRecordSet->Current.MoveToEOF();
	
	RuntimePushDouble(ret);
    return M4_SUCCESS;
}

m4return_t ClVM1Register::MoveTo(void){
	
	m4return_t ret;
	m4VariantType vTemp;
	m4bool_t bRet;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: MoveTo");
#endif

	ret=RuntimePop(vTemp);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "MoveTo.")	;
	
	ret=m_oVariantMediator->ToDouble(vTemp);
    M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH , ERRORLOG, "Position" << "MoveTo" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTemp.Type)<<DumpCallStack() );
	
    bRet = m_oState1.m_poRecordSet->Current.MoveTo( ClRegisterIndex ( (m4int32_t)vTemp.Data.DoubleData) ) ;
	//moveto devuelve isnoteof
    
    RuntimePushDouble(bRet==M4_TRUE ? M4_SUCCESS : M4_ERROR);
	return M4_SUCCESS;
}


m4return_t ClVM1Register::Count			(void){
    
#if _M4_DEBUG > 10  
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: Count");
#endif
	
    RuntimePushDouble (m_oState1.m_poRecordSet->Current.Count() );
    return M4_SUCCESS;
}


m4return_t ClVM1Register::GetCurrent	(void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetCurrent");
#endif

	if (m_oState1.m_poRecordSet->Current.Index == M4DM_EOF_INDEX) {
		RuntimePushDouble (-1);
	}
	else{
		RuntimePushDouble (m_oState1.m_poRecordSet->Current.Index);
	}
	
    return M4_SUCCESS;
}


m4return_t ClVM1Register::IsEOF (void){
	
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: IsEOF");
#endif
	
    RuntimePushDouble (m_oState1.m_poRecordSet->Current.Index == M4DM_EOF_INDEX)? TRUE : FALSE;
    return M4_SUCCESS;
}


m4return_t ClVM1Register::DeleteAllRegisters(void){
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: DeleteAllRegisters");
#endif
	
    RuntimePushDouble (m_oState1.m_poRecordSet->Register.DeleteAll() );
    return M4_SUCCESS;
}


m4return_t ClVM1Register::DestroyAllRegisters(void){
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: DestroyAllRegisters");
#endif
    
    RuntimePushDouble (m_oState1.m_poRecordSet->Register.DestroyAll() );
    return M4_SUCCESS;
}


m4return_t ClVM1Register::DestroyBlock(void){
    
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: DestroyBlock");
#endif
	
    RuntimePushDouble (m_oState1.m_poRecordSet->Destroy() );
    return M4_SUCCESS;
}


m4return_t ClVM1Register::GetRegPosition   (void){
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetRegPosition");
#endif

    if (m_oState1.m_oRegisterIndex == M4DM_EOF_INDEX) {
        RuntimePushDouble (-1);
    }
    else{
        RuntimePushDouble (m_oState1.m_oRegisterIndex);
    }
    
    return M4_SUCCESS;
}


//-----------------------------------
//Copiado de registros
//-----------------------------------
m4return_t  ClVM1Register::CopyRegister (void){
	
    //Parametros:  channel, nodo , indice de registro del que copiamos
	
    m4pcchar_t  pszThisChannel;
    m4uint32_t  hThisNode;
	m4char_t	*pszChannel, *pszNode;
	
    ClAccess    *poSrcAccess;
    ClNode      *poSrcNode;
	ClAccessRecordSet *poSrcRS;
	_ClRegister_Base  *poDstReg, *poSrcReg;
	m4VariantType		vNode, vChannel, vRegIdx, vNumArg, vSrcFilt, vDestFilt;
	m4uint32_t iNumArg, iRegIdx, i;
    m4return_t ret=M4_SUCCESS;
	m4return_t retVal=M4_SUCCESS;
	m4uint8_t  iSrcFilt=0, iDestFilt=0;

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: CopyRegister");
#endif

	//número de argumentos
	ret= RuntimePopNumber (vNumArg);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "CopyRegister.")	;
	iNumArg=(m4int32_t) vNumArg.Data.DoubleData;

	if (iNumArg<2){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "CopyRegister"<<"2-5" << DumpCallStack() );
		return M4_ERROR;
	}


	//sacamos los argumentos sobrantes
	if (iNumArg>5){
		for (i=0 ; i< iNumArg-5; ++i){
			RuntimePopAndDelete();
		}
	}
	//filtro del destino
	if (iNumArg > 4){
		ret = RuntimePop (vDestFilt);
		DEBUG_CHECK_RES (ret==M4_SUCCESS , M4_CH_VM_RTS_POP, ERRORLOG, "Error getting parameter (Dest. filter) in CopyRegister.")	;
		ret=m_oVariantMediator->ToDouble (vDestFilt);
		if (ret != M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Dest_filter"<<"CopyRegister" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vDestFilt.Type)<< DumpCallStack() );

			return M4_ERROR;
		}
		iDestFilt=(m4uint8_t)vDestFilt.Data.DoubleData;
	}

	//filtro del origen (nodo que se especifica con parámetros)
	if (iNumArg > 3){
		ret = RuntimePop (vSrcFilt);
		DEBUG_CHECK_RES (ret==M4_SUCCESS , M4_CH_VM_RTS_POP, ERRORLOG, "Error getting parameter (Src. filter) in CopyRegister.")	;
		ret=m_oVariantMediator->ToDouble (vSrcFilt);
		if (ret != M4_SUCCESS){
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Src_filter"<<"CopyRegister" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vSrcFilt.Type)<< DumpCallStack() );

			return M4_ERROR;
		}
		iSrcFilt=(m4uint8_t)vSrcFilt.Data.DoubleData;
	}

	//número de registro
	if (iNumArg > 2){ //viene con número de registro
		ret=RuntimePop(vRegIdx);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "CopyRegister.RegisterIndex.")	;
		if (vRegIdx.Type!=M4CL_CPP_TYPE_NUMBER){
			ret=m_oVariantMediator->ToDouble (vRegIdx);
			
			if (ret != M4_SUCCESS ){
				DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "index"<<"CopyRegister" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vRegIdx.Type)<< DumpCallStack() );
				return M4_ERROR;
			}
		}
		/* Bug 0116022
		El casting debe ser a entero con signo porque sino en AIX lo convierte a 0
		*/
		iRegIdx=(m4int32_t)vRegIdx.Data.DoubleData;
	}
	else{
		iRegIdx=-1;
	}
	
	ret=RuntimePopString(vNode);
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "Node" << "CopyRegister" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vNode.Type)<<DumpCallStack() );
	
	ret=RuntimePopString(vChannel);
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "M4Object"<<"CopyRegister" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vChannel.Type)<<DumpCallStack() );

		m_oDynamicPool->DeleteString (vNode.Data.CharData);
		return M4_ERROR;
	}
	
	
	pszChannel= m_oDynamicPool->GetString (vChannel.Data.CharData);
	pszNode   = m_oDynamicPool->GetString (vNode.Data.CharData);
	
	hThisNode = m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle();
	
	if (pszChannel == 0 || strcmp(pszChannel,"")==0){
		
		poSrcAccess=m_oState1.m_poAccess;
		if (strcmpi(pszNode, m_oState1.m_poAccess->GetpCMCR()->GetNodeId(hThisNode) )==0 ){
			poSrcRS=m_oState1.m_poRecordSet;	
		}
		else{
			m4uint32_t hNode   = m_oState1.m_poAccess->GetpCMCR()->GetNodeAliasNodeHandleByAlias ( hThisNode, pszNode, m_oState1.m_poFormula.GetLevel() ) ;
			if( hNode == 0 ){
				RuntimePushDouble( M4_ERROR ) ;
				DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, pszNode <<"CopyRegister" << DumpCallStack() );
				ret = M4_ERROR;
			}
			poSrcRS	   =&(m_oState1.m_poAccess->Node[ClHandle(hNode)].RecordSet);//m_oState1.m_poRecordSet;
		}
	}
	else{
		pszThisChannel = m_oState1.m_poAccess->GetpCMCR()->GetChannelId();
		
		ret = GetAccessAndNode      (pszThisChannel, m_oState1.m_poAccess, hThisNode, pszChannel, pszNode, poSrcNode, poSrcAccess);
		if (ret!=M4_SUCCESS){
			//El error ya sale en GetAccessAndNode
			//	DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_INSTANCE, pszChannel<<ai_poThisAccess->GetpCMCR()->iNodeN(hThisNode)<<LogCatAnd<<ai_poThisAccess->GetpCMCR()->nNodeN(hThisNode)<<"CopyRegister" << DumpCallStack() );
			ret = M4_ERROR;
		}
		poSrcRS= &(poSrcNode->RecordSet);
	}
	
	//libero las cadenas
	m_oDynamicPool->DeleteString (vChannel.Data.CharData);
	m_oDynamicPool->DeleteString  (vNode.Data.CharData);
	
	if (ret == M4_SUCCESS){ //tenemos nodo y canal -> comprobamos los indices de registro
		
		poDstReg = &( m_oState1.m_poRecordSet->Current );
		
		if (iRegIdx==-1){
			iRegIdx=poSrcRS->Current.Index;
		}
		poSrcReg = &( poSrcRS->Register[ iRegIdx ] );
/*		
		if (poSrcReg->Index.IsEOF() ){
			//SETCODE ( M4_CH_VM_COPING_REGISTER ,WARNINGLOG, "Warning CopyRegister. Source record is EOF. Cannot copy record. ");
			DUMP_CHLOG_WARNINGF(M4_CH_VM_COPING_REGISTER, "Warning CopyRegister. Source record is EOF. Cannot copy record." << DumpCallStack() );
			RuntimePushDouble (M4_SUCCESS);
			return M4_SUCCESS;
		}
		
		if (poDstReg->Index.IsEOF() ){
			//SETCODE ( M4_CH_VM_COPING_REGISTER ,WARNINGLOG, "Warning CopyRegister. Destination record is EOF. Cannot copy record. ");		
			DUMP_CHLOG_WARNINGF(M4_CH_VM_COPING_REGISTER, "Warning CopyRegister. Destination record is EOF. Cannot copy record." << DumpCallStack() );
			RuntimePushDouble (M4_SUCCESS);
			return M4_SUCCESS;
		}*/
	}
	else{
		return ret;
	}
	

	//nos encargamos de conseguir los iteradores de los items
    ClItem *SrcIt=&(poSrcReg->Item);
	ClItem *DestIt=&(poDstReg->Item);
	m4bool_t	bItCreated=M4_FALSE;
	ClItemIndex	idxSrcOldPos, idxDestOldPos;

	//Guardamos las posiciones de los index
	idxSrcOldPos=SrcIt->GetIndex();
	idxDestOldPos=DestIt->GetIndex();

	if (DestIt==SrcIt){ //si los iteradores son los mismos->estamos copiando sobre el mismo bloque
		SrcIt=new ClItem(poSrcReg->Item);
		bItCreated=M4_TRUE;
	}

	m4uint8_t iSrcScope, iSrcType, iDestScope, iDestType;

	iSrcScope=iSrcFilt & M4CL_ITEM_SCOPE_MASK_BITS;
	iSrcType=(iSrcFilt & M4CL_ITEM_TYPE_MASK_BITS) >>3;
	iSrcType &= (M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT); //desactivamos el flag de metodos, por si acaso

	iDestScope=iDestFilt & M4CL_ITEM_SCOPE_MASK_BITS;
	iDestType =(iDestFilt & M4CL_ITEM_TYPE_MASK_BITS) >>3;
	iDestType &=(M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT); //desactivamos el flag de metodos, por si acaso

	//si alguno es 0 usamos los de por defecto
	if (iSrcScope==0){ 
		iSrcScope = M4DM_SCOPE_REGISTER;
	}
	if (iSrcType  == 0){
		iSrcType  = M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT;
	}

	if (iDestScope==0){
		iDestScope = M4DM_SCOPE_REGISTER;
	}
	if (iDestType==0){
		iDestType  = M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT;
	}
	

	SrcIt->SetMode (iSrcScope, iSrcType);
	DestIt->SetMode (iDestScope, iDestType);

	//copiado en sí
    m4uint32_t iNumItem= DestIt->Count();
    m4VariantType vTemp;
	ClItemIndex idxSrc;

    for (i=0 ; i< iNumItem; ++i){
		(*DestIt)[ClItemIndex(i)];
        
		idxSrc=SrcIt->TryMove ( DestIt->ItemDef.Id() , M4_TRUE ); //no posicionamos el item porque ya lo hemos hecho
																//El true porque viene en mayusculas
		if (idxSrc != M4DM_EOF_INDEX) {
			
			(*SrcIt)[idxSrc]; //Nos movemos (por index) sabiendo que va a existir
			
			ret=DestIt->Value.CopyItem ( &(SrcIt->Value) ); //copaimso todo (incluido slices)
			if (ret == M4_ERROR){
//				g_oChLog<<BeginError(M4_CH_VM_COPING_ITEM)<<M4ObjNodeItemI(DestIt.GetpAccess()->GetpCMCR(), DestIt.ItemDef.GetHandle() )<<EndLog;
				retVal=M4_ERROR;
			}
			else {
				if ( (GetVMCSContext()->GetSttWrap()->GetStatisticsLevel() & M4_VM_STATISTICS_ENABLED_VALUES) )
				{
					m4VariantType	vValue ;
					m4uint32_t		iSlices = DestIt->Value.Count() ;

					if( iSlices == 0 )
					{
						ret = DestIt->Value.Get( vValue ) ;

						if( ret != M4_ERROR )
						{
							GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddValue( *DestIt, vValue, DestIt->Value.GetPriority(), M4_TRUE, 1, m_oState1, DumpCallStack( &m_oState1, M4_TRUE ).c_str() );
						}
					}
					else
					{
						for( m4uint32_t i = 0 ; i < iSlices ; i++ )
						{
							ret = DestIt->Value[i].Get( vValue ) ;

							if( ret != M4_ERROR )
							{
								GetVMCSContext()->GetSttWrap()->GetStatistics()->sttAddValue( *DestIt, vValue, DestIt->Value[i].GetPriority(), M4_TRUE, 1, m_oState1, DumpCallStack( &m_oState1, M4_TRUE ).c_str() );
							}
						}
					}
				}
			}
		}
    }


	//restauramos los filtros y borramos si es necesario
	DestIt->SetMode (M4DM_SCOPE_ALL, M4DM_TYPE_ALL);

	if (bItCreated){
		delete SrcIt;
	}
	else{
		SrcIt->SetMode (M4DM_SCOPE_ALL, M4DM_TYPE_ALL);
	}

	//restauramos las posiciones de los index
	SrcIt[idxSrcOldPos];
	DestIt[idxDestOldPos];

	RuntimePushDouble( retVal);
	
	
    return M4_SUCCESS;
}


m4return_t ClVM1Register::CloneRegister (void){
	
	//añade un registro, copia a el el current y se posiciona en el
    m4return_t ret;
    
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: CloneRegister");
#endif
	if (m_oState1.m_poRecordSet->Current.Index == M4DM_EOF_INDEX) {
		DUMP_CHLOG_WARNINGF(M4_CH_VM_SOURCE_RECORD_IS_EOF, M4ObjNodeN(m_oState1.m_poAccess->GetpCMCR(),m_oState1.m_poRecordSet->GetpNodeDef()->GetHandle() ) << DumpCallStack() );
		RuntimePushDouble( M4_WARNING );
		return M4_SUCCESS;
	}
	
    ret = m_oState1.m_poRecordSet->Register.Add();
    
    m_oState1.m_poRecordSet->Register << m_oState1.m_poRecordSet->Current;
	m_oState1.m_poRecordSet->Current.End();
	
    //metemos el resultado
	RuntimePushDouble( ret);
	return ret;
}


m4return_t	ClVM1Register::CopyBranch( void )
{

	m4return_t			iResult = M4_SUCCESS ;
	m4uint32_t			iRecord = 0 ;
	m4uint32_t			i = 0 ;
	m4uint32_t			iNumArg = 0 ;
	m4uint32_t			hNode = 0 ;
	m4VariantType		vNode ;
	m4VariantType		vM4Obj ;
	m4VariantType		vValue ;
	ClAccess			*poAccess = NULL ;
	ClNode				*poNode = NULL ;
	_ClRegister_Base	*poRecord = NULL ;
    ClFileName			oFileName ;


#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF( M4_CH_VM_TRACE_VM_L1, "Trace VM1: CopyBranch" ) ;
#endif

    // Leemos el número de argumentos (pueden ser 1, 2, 3 ó 4)
	iResult = RuntimePopNumber( vValue ) ;
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "CopyBranch" ) ;

    iNumArg = m4uint32_t( vValue.Data.DoubleData ) ;

	if( iNumArg < 1 )
	{
		// Si hay menos de 1 argumento se borran y se da error
		DUMP_CHLOG_ERRORF( M4_CH_VM_WRONG_PARAM_NUM, "CopyBranch" << "1-2-3-4" << DumpCallStack() ) ;

		for( i = 0 ; i < iNumArg ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
		}
		RuntimePushDouble( M4_ERROR ) ;
		RuntimePushNull() ;
        return( M4_ERROR ) ;
	}

	// Limpiamos los argumentos de más
	if( iNumArg > 4 )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_TOO_PARAM, "CopyBranch" << 4 << iNumArg + 1 << DumpCallStack() ) ;

		for( i = 4 ; i < iNumArg ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
			M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "CopyBranch" ) ;
		}
		iNumArg = 4 ;
	}

	// Meta4Object
	poAccess = m_oState1.m_poAccess ;

	if( iNumArg >= 4 )
	{
		iResult = RuntimePop( vM4Obj ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "CopyBranch" ) ;

		poAccess = _FindChannel( vM4Obj, "CopyBranch", m_oState1 ) ;
		DeleteVarString( vM4Obj ) ;

		if( poAccess == NULL )
		{
			DeleteVarString( vNode ) ;
			RuntimePushDouble( M4_ERROR ) ;
			RuntimePushNull() ;
			return( M4_ERROR ) ;
		}
	}

	// Nodo
	if( iNumArg >= 3 )
	{
		iResult = RuntimePop( vNode ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "CopyBranch" ) ;
	}
	else
	{
		vNode.Type = M4CL_CPP_TYPE_NULL ;
	}

	poNode = _FindNode( poAccess, vNode, "CopyBranch", m_oState1, M4_TRUE ) ;
	DeleteVarString( vNode ) ;

	if( poNode == NULL )
	{
		RuntimePushDouble( M4_ERROR ) ;
		RuntimePushNull() ;
		return( M4_ERROR ) ;
	}

	// Registro
	if( iNumArg >= 2 )
	{
		iResult = RuntimePop( vValue ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "CopyBranch" ) ;

		if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Record" << "CopyBranch" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
			DeleteVarString( vValue ) ;
			RuntimePushDouble( M4_ERROR ) ;
			RuntimePushNull() ;
			return( M4_ERROR ) ;
		}

		iRecord = (m4int32_t) vValue.Data.DoubleData ;
	}
	else
	{
		iRecord = -1 ;
	}

	if( iRecord == -1 )
	{
		iRecord = poNode->RecordSet.Current.Index;
	}

	poRecord = &( poNode->RecordSet.Register[ iRecord ] );

	if( poRecord == NULL || poRecord->Index == M4DM_EOF_INDEX )
	{
		hNode = poNode->GetpNodeDef()->GetHandle() ;
		DUMP_CHLOG_ERRORF( M4_CH_VM_WRONG_RECORD_NUMBER, M4ObjNodeN( poAccess->GetpCMCR(), hNode ) << "CopyBranch" << DumpTreePath( poAccess, hNode ) << DumpCallStack() ) ;
		RuntimePushDouble( M4_ERROR ) ;
		RuntimePushNull() ;
		return( M4_ERROR ) ;
	}

	// Filemane de salida
	iResult = RuntimePop( vValue ) ;
	M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "CopyBranch" ) ;
	DeleteVarString( vValue ) ;

	iResult = poRecord->CopyRecordWithSerialize( oFileName ) ;

	if( iResult != M4_SUCCESS )
	{
		RuntimePushDouble( M4_ERROR ) ;
		RuntimePushNull() ;
		return( M4_ERROR ) ;
	}

	RuntimePushDouble( M4_SUCCESS ) ;
	RuntimePushString( oFileName ) ;
	return M4_SUCCESS;
}


m4return_t	ClVM1Register::PasteBranch( void )
{

	m4return_t			iResult = M4_SUCCESS ;
	m4bool_t			bNewRecord = M4_FALSE ;
	m4bool_t			bKeepMarks = M4_FALSE ;
	m4bool_t			bDeleteFile = M4_FALSE ;
	m4uint32_t			iRecord = 0 ;
	m4uint32_t			i = 0 ;
	m4uint32_t			iNumArg = 0 ;
	m4uint32_t			hNode = 0 ;
	m4VariantType		vNode ;
	m4VariantType		vM4Obj ;
	m4VariantType		vValue ;
	ClAccess			*poAccess = NULL ;
	ClNode				*poNode = NULL ;
	_ClRegister_Base	*poRecord = NULL ;


#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF( M4_CH_VM_TRACE_VM_L1, "Trace VM1: PasteBranch" ) ;
#endif

    // Leemos el número de argumentos (pueden ser 4, 5, 6 ó 7)
	iResult = RuntimePopNumber( vValue ) ;
    M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "PasteBranch" ) ;

    iNumArg = m4uint32_t( vValue.Data.DoubleData ) ;

	if( iNumArg < 4 )
	{
		// Si hay menos de 1 argumento se borran y se da error
		DUMP_CHLOG_ERRORF( M4_CH_VM_WRONG_PARAM_NUM, "PasteBranch" << "4-5-6-7" << DumpCallStack() ) ;

		for( i = 0 ; i < iNumArg ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
		}
		RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}

	// Limpiamos los argumentos de más
	if( iNumArg > 7 )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_TOO_PARAM, "PasteBranch" << 7 << iNumArg + 1 << DumpCallStack() ) ;

		for( i = 7 ; i < iNumArg ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
			M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "PasteBranch" ) ;
		}
		iNumArg = 7 ;
	}

	// Meta4Object
	poAccess = m_oState1.m_poAccess ;

	if( iNumArg >= 7 )
	{
		iResult = RuntimePop( vM4Obj ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "PasteBranch" ) ;

		poAccess = _FindChannel( vM4Obj, "PasteBranch", m_oState1 ) ;
		DeleteVarString( vM4Obj ) ;

		if( poAccess == NULL )
		{
			DeleteVarString( vNode ) ;
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}
	}

	// Nodo
	if( iNumArg >= 6 )
	{
		iResult = RuntimePop( vNode ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "PasteBranch" ) ;
	}
	else
	{
		vNode.Type = M4CL_CPP_TYPE_NULL ;
	}

	poNode = _FindNode( poAccess, vNode, "PasteBranch", m_oState1, M4_TRUE ) ;
	DeleteVarString( vNode ) ;

	if( poNode == NULL )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	// Registro
	if( iNumArg >= 5 )
	{
		iResult = RuntimePop( vValue ) ;
		M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "PasteBranch" ) ;

		if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Record" << "PasteBranch" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
			DeleteVarString( vValue ) ;
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}

		iRecord = (m4int32_t) vValue.Data.DoubleData ;
	}
	else
	{
		iRecord = -1 ;
	}

	if( iRecord == -1 )
	{
		iRecord = poNode->RecordSet.Current.Index;
	}

	poRecord = &( poNode->RecordSet.Current[ iRecord ] );

	// Delete file
	iResult = RuntimePop( vValue ) ;
	M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "PasteBranch" ) ;

	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "DeleteFile" << "PasteBranch" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
		DeleteVarString( vValue ) ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	if( vValue.Data.DoubleData == 1.0 )
	{
		bDeleteFile = M4_TRUE ;
	}
	else
	{
		bDeleteFile = M4_FALSE ;
	}

	// Keep marks
	iResult = RuntimePop( vValue ) ;
	M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "PasteBranch" ) ;

	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "KeepMarks" << "PasteBranch" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
		DeleteVarString( vValue ) ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	if( vValue.Data.DoubleData == 1.0 )
	{
		bKeepMarks = M4_TRUE ;
	}
	else
	{
		bKeepMarks = M4_FALSE ;
	}

	// New record
	iResult = RuntimePop( vValue ) ;
	M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "PasteBranch" ) ;

	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "NewRecord" << "PasteBranch" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
		DeleteVarString( vValue ) ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	if( vValue.Data.DoubleData == 1.0 )
	{
		bNewRecord = M4_TRUE ;
	}
	else
	{
		bNewRecord = M4_FALSE ;
	}

	// Filemane
	iResult = RuntimePop( vValue ) ;
	M4CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, ERRORLOG, "PasteBranch" ) ;

	if( vValue.Type != M4CL_CPP_TYPE_STRING_POOL )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "FileName" << "PasteBranch" << M4ClCppType( M4CL_CPP_TYPE_STRING_POOL ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

    ClFileName	oFileName( m_oDynamicPool->GetString( vValue.Data.CharData ) ) ;
	DeleteVarString( vValue ) ;

	iResult = poRecord->PasteRecordWithDeSerialize( bNewRecord, bKeepMarks, oFileName, bDeleteFile, M4_FALSE, NULL, 0, M4_FALSE ) ;

	if( iResult != M4_SUCCESS )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	RuntimePushDouble( M4_SUCCESS ) ;
	return M4_SUCCESS;
}




//
//undo's 
//
m4return_t ClVM1Register::UndoRegister		(void){

#if _M4_DEBUG > 10  
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: UndoRegister");
#endif

	RuntimePushDouble ( m_oState1.m_poRecordSet->Current.Undo() );
	return M4_SUCCESS;
}


m4return_t ClVM1Register::UndoBlock			(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: UndoBlock");
#endif

	RuntimePushDouble ( m_oState1.m_poRecordSet->Undo() );
	return M4_SUCCESS;
}


m4return_t ClVM1Register::UndoNode			(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: UndoNode");
#endif

	RuntimePushDouble ( m_oState1.m_poRecordSet->GetpNode_Data()->Undo() );
	return M4_SUCCESS;
}


m4return_t ClVM1Register::UndoChannel			(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: UndoChannel");
#endif

	RuntimePushDouble ( m_oState1.m_poAccess->GetpChannel()->GetpChannel_Data()->Undo() );
	return M4_SUCCESS;
}


//
//CheckPoint
//
m4return_t ClVM1Register::CheckPointRegister	(void){

#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: CheckPointRegister");
#endif

	RuntimePushDouble ( m_oState1.m_poRecordSet->Current.CheckPoint() );
	return M4_SUCCESS;

}

m4return_t ClVM1Register::CheckPointBlock		(void){

#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: CheckPointBlock");
#endif

	RuntimePushDouble (m_oState1.m_poRecordSet->CheckPoint());
	return M4_SUCCESS;

}

m4return_t ClVM1Register::CheckPointNode		(void){
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: CheckPointNode");
#endif

	RuntimePushDouble ( m_oState1.m_poRecordSet->GetpNode_Data()->CheckPoint() );
	return M4_SUCCESS;

}

m4return_t ClVM1Register::CheckPointChannel	(void){
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: CheckPointChannel");
#endif

	RuntimePushDouble (m_oState1.m_poAccess->GetpChannel()->GetpChannel_Data()->CheckPoint());
	return M4_SUCCESS;

}


//---------------------------------------------
//-D U M P 
//---------------------------------------------
m4return_t  ClVM1Register::DumpRegister(void){
    
	m4return_t ret;
    ofstream ofreg;
    ClAccessRecordSet     *poCurrentRecordSet;
    
	m4char_t	acFile[ M4CH_MAX_FILE + 1 ] ;
	strcpy (acFile,m_oState1.m_poAccess->GetEnvironment()->GetDebugDirectory());
	strcat (acFile,M4_BACK_SLASH);
	strcat (acFile,"vmreg.log");

    poCurrentRecordSet=m_oState1.m_poRecordSet;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
	M4Open( ofreg, acFile, M4UniAppend, eEncoding );

    if (m_oState1.m_poRecordSet->IsValid() && poCurrentRecordSet->Current.Index != M4DM_EOF_INDEX) {
		ClRegister *pRegister = & (m_oState1.m_poRecordSet->Register[ poCurrentRecordSet->Current.Index ]);

        ofreg << "---------- Volcado de registro ----------"<<endl;
        ofreg << *pRegister <<endl;
        ofreg << "---------- Fin de Volcado reg. ----------"<<endl;

		//Informacion de memoria ocupada por el Nodo
		ClChannelMemInfo			oMemInfo;		//Objeto para medir la memoria
		stRgMemInfo_t		oRegMemInfo;	//Estructura con el resumen de la informacion
		ret=oMemInfo.GetMemInfo(pRegister, oRegMemInfo);
		if (ret==M4_SUCCESS) {
			ofreg << "Register Memory Info:" << endl;
			ofreg << "---------------------" << endl;
			ofreg << oRegMemInfo		 << endl;
		}else{
			ofreg << "Error in DumpRegister Statistics" << endl;
		}
    }
	else{
        ofreg << "---------- registro en EOF ----------"<<endl;
	}

	ofreg.close();
    RuntimePushDouble (M4_SUCCESS);
    return M4_SUCCESS;
}


m4return_t  ClVM1Register::DumpNode(void){
    
	m4return_t ret;
    ofstream ofreg;

    m4char_t	acFile[ M4CH_MAX_FILE + 1 ] ;
	strcpy (acFile,m_oState1.m_poAccess->GetEnvironment()->GetDebugDirectory());
	strcat (acFile,M4_BACK_SLASH);
	strcat (acFile,"vmNode.log");

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
	M4Open( ofreg, acFile, M4UniAppend, eEncoding );

    if (m_oState1.m_poRecordSet->IsValid()  ){
        ofreg << "---------- Volcado   de   Nodo ----------"<<endl;        
        ofreg << *m_oState1.m_poRecordSet->GetpNode_Data()  <<endl;
        ofreg << "---------- Fin de Volcado Nodo ----------"<<endl;

		//Informacion de memoria ocupada por el Nodo
		ClChannelMemInfo			oMemInfo;		//Objeto para medir la memoria
		stNodeMemInfo_t		oNodeMemInfo;	//Estructura con el resumen de la informacion
		ClNode *pNode = m_oState1.m_poAccess->Node.Get( m_oState1.m_poRecordSet->GetpNode_Data()->GetpNodeDef()->GetHandle() );

		ret=oMemInfo.GetMemInfo(pNode, oNodeMemInfo);
		if (ret==M4_SUCCESS) {
			ofreg << "Node Memory Info:" << endl;
			ofreg << "-----------------" << endl;
			ofreg << oNodeMemInfo		 << endl;
		}else{
			ofreg << "Error in DumpNode Statistics" << endl;
		}
    }
	else{
        ofreg << "---------- Nodo sin bloque activo----------"<<endl;
	}
	
	ofreg.close();
    RuntimePushDouble (M4_SUCCESS);
    return M4_SUCCESS;
}

m4return_t  ClVM1Register::DumpChannel(void){
 
	m4return_t ret;
	ofstream ofreg;
	ClSimpleDump_Visitor dv (ofreg);
	
    m4char_t	acFile[ M4CH_MAX_FILE + 1 ] ;
	strcpy (acFile,m_oState1.m_poAccess->GetEnvironment()->GetDebugDirectory());
	strcat (acFile,M4_BACK_SLASH);
	strcat (acFile,"vmChannel.log");

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
	M4Open( ofreg, acFile, M4UniAppend, eEncoding );

    if (m_oState1.m_poAccess !=NULL){
        ofreg << "---------- Volcado de Access   ----------"<<endl;
        dv.Visit_Channel (*(m_oState1.m_poAccess->GetpChannel() ) );
        ofreg << endl<< "---------- Fin de Volcado Acc. ----------"<<endl;

		//Informacion de memoria ocupada por el Canal
		ClChannelMemInfo		   oMemInfo;		//Objeto para medir la memoria
		stChannelMemInfo_t oChannelMemInfo;	//Estructura con el resumen de la informacion
		ret=oMemInfo.GetMemInfo(m_oState1.m_poAccess, oChannelMemInfo);
		if (ret==M4_SUCCESS) {
			ofreg << "M4Object Memory Info:" << endl;
			ofreg << "---------------------" << endl;
			ofreg << oChannelMemInfo		 << endl;
		}else{
			ofreg << "Error in DumpChannel Statistics" << endl;
		}
    }
	else{
        ofreg << "---------- Access nulo. ----------"<<endl;
	}
	
	ofreg.close();

    RuntimePushDouble (M4_SUCCESS);
    return M4_SUCCESS;
}

//LN4 Function:		DumpStatistics(filename, Id(Instancia o Alias) [, iLevel2(>=0) ] )
m4return_t  ClVM1Register::DumpStatistics(void){
    
	m4return_t ret=M4_SUCCESS;
	
	m4VariantType vFileName;
	m4VariantType vId;
	m4VariantType vType;
	
	m4int32_t i;
	m4uint32_t uiLevel;

    //sacamos el No. de Argumentos
    ret=RuntimePop(vType);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DumpStatistics.");

	m4int32_t iNumArg = (m4int32_t)vType.Data.DoubleData;
    if (iNumArg >= 3 ){
		//eliminamos los posibles argumentos extras
        for (i=0; i < iNumArg-3; ++i){
            ret=RuntimePopAndDelete ();
			DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DumpStatistics.");
		}
		//sacamos el Nivel de Estadisticas
		ret=RuntimePop (vType);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "DumpStatistics.");
		ret=m_oVariantMediator->ToDouble(vType);
		if (ret!=M4_SUCCESS || vType.Data.DoubleData<0.0 || vType.Data.DoubleData>=UINT_MAX) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "Level" << "DumpStatistics" << vType.Data.DoubleData << DumpCallStack() );
			return M4_ERROR;
		}
    }
    else{
		//Tipo por defecto:	0 - Level 2 Desactivado. Solo las estadisticas del canal.
        vType.Data.DoubleData=0.0;
    }
	uiLevel = (m4uint32_t)vType.Data.DoubleData;
	
    //sacamos el Id (Identificador de Instancia o Alias de Instancia)
    ret=RuntimePopString(vId);
    if (ret!=M4_SUCCESS) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Id M4Object"<< "DumpStatistics" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vId.Type)<<DumpCallStack() );
		return M4_ERROR;
	}

    //sacamos el Nombre de Fichero
    ret=RuntimePopString(vFileName);
    if (ret!=M4_SUCCESS) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Filename"<< "DumpStatistics" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vFileName.Type)<<DumpCallStack() );
		DeleteVarString (vId);
		return M4_ERROR;
	}

	//Abrimos el fichero de salida
	ofstream ofreg;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
	M4Open( ofreg, m_oDynamicPool->GetString(vFileName.Data.CharData), M4UniAppend, eEncoding );

	if ( !ofreg.rdbuf()->is_open() ){
		//Fichero no se puede crear, salimos sin estadisticas y sin error
		DUMP_CHLOG_WARNINGF(M4_CH_VM_ERROR_IN_FILE, "DumpStatistics" << m_oDynamicPool->GetString(vFileName.Data.CharData) << strerror( errno ) << DumpCallStack() );
		DeleteVarString (vId);
		DeleteVarString (vFileName);
		RuntimePushDouble (M4_ERROR);
		return M4_SUCCESS;
	}

	//*** PROCESO ***
	m4pcchar_t pszId = m_oDynamicPool->GetString(vId.Data.CharData);

	//Si estamos en el mismo canal, utilizo el mismo Access, sino busco el "alias" o "instancia"
	//y obtengo un access a la instancia asociada.
	m4bool_t		bSameChannel;
	ClAccess		*poAccess		= NULL;	//Access al Canal actual o de Level 2 (segun el caso)
	ClCompiledMCR   *poThisCMCR		= NULL;	//Metacanal del Canal en que nos ejecutamos
	m4pcchar_t		 pszThisChannel	= NULL;	//Id. del Canal en que nos ejecutamos

	//Verifico el access donde ejecuto
	if (m_oState1.m_poAccess!=NULL) {
		poThisCMCR	   = m_oState1.m_poAccess->GetpCMCR();	//Metacanal del Canal en que nos ejecutamos
		pszThisChannel = poThisCMCR->GetChannelId() ;		//Id. del Canal en que nos ejecutamos
	}

	if ( m_oState1.m_poAccess==NULL || *pszId=='\0' || strcmpi(pszThisChannel, pszId)==0 ) {
		//Piden el mismo canal, o access erroneo
		bSameChannel=M4_TRUE;
		poAccess = m_oState1.m_poAccess;
		pszId = pszThisChannel;
	} else {
		//Piden otro canal => Mirar si es una instancia de Level 2
		bSameChannel=M4_FALSE;
		
		m4uint32_t hNode = m_oState1.m_poRecordSet->GetpNode_Data()->GetpNodeDef()->GetHandle();
		m4pcchar_t pszInstance = poThisCMCR->GetNodeT3AliasInstanceIdByAlias( hNode, pszId ) ;
		if (!pszInstance) {
			//No encontrado => Lo podria intentar levantar???
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_INSTANCE, pszId<<poThisCMCR->iNodeN(hNode)<<LogCatAnd<<poThisCMCR->nNodeN(hNode)<<"DumpStatistics" << DumpCallStack() );
			poAccess = NULL;
			ret = M4_ERROR;
		}else{
			//Encontrada la Instancia
			ret = m_poVM->GetL2Access( m_oState1.m_poAccess->GetpChannel(), pszInstance, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState1.m_Flags), &poAccess );
			if( ret != M4_SUCCESS ) {
				//Que extraño ????
				DUMP_CHLOG_ERRORF(M4_CH_VM_NO_GET_ACCESS_L2, pszInstance<<"DumpStatistics" << DumpCallStack() );
				poAccess = NULL;
				ret = M4_ERROR;
			}
		}
	}

	ofreg << "##########################################" << endl;
	ofreg << "############ Begin Statistics ############" << endl;
	ofreg << "##########################################" << endl;

	//Tenemos el Access a la Instancia adecuada => a volcar la Informacion
    if (poAccess !=NULL){

		stChannelMemInfo_t oChannelMemInfo;		//Estructura con el resumen de la informacion del Canal
		ret=DumpChannelStatistics(ofreg, poAccess, uiLevel, oChannelMemInfo);
		if (ret!=M4_SUCCESS) {
			//Error muy extraño
			ofreg << "******** Error in DumpStatistics ********" << endl;
		}
		ofreg << *( poAccess->GetpChannel()->GetpChannelManager() ) ;
		
    }else{
		//No tenemos access
        ofreg << "---------- Access null. ----------"<<endl;
	}

	ofreg << "##########################################" << endl;
	ofreg << "############ End   Statistics ############" << endl;
	ofreg << "##########################################" << endl<<endl;

    ofreg.close();

	DeleteVarString (vId);
	DeleteVarString (vFileName);

    RuntimePushDouble (ret);
    return M4_SUCCESS;
}

//---------
//Auxiliar
//---------

//para obtener el access y el nodo
m4return_t ClVM1Register::GetAccessAndNode      (m4pcchar_t ai_pszThisChannel, ClAccess *ai_poThisAccess, m4uint32_t ai_hThisNode, m4pcchar_t ai_pszChannel, m4pcchar_t ai_pszNode, ClNode *& ao_poNode, ClAccess *& ao_poAccess){
	
	
    m4pcchar_t  pccHostInstance; 
    m4uint32_t hNode;
    m4bool_t iSameChannel;
    m4return_t ret=M4_SUCCESS;
    
    if ( ai_pszChannel == 0 ){
		//el copyregister es porque es desde donde se llama, que es lo que el user tiene que saber
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "ChannelId"<<"CopyRegister"<<"<null>" << DumpCallStack() );
        return M4_ERROR;
    }
    
    
    if( *ai_pszChannel == '\0' || strcmpi( ai_pszThisChannel, ai_pszChannel ) == 0 )
    {
        iSameChannel = M4_TRUE ;
        ao_poAccess = ai_poThisAccess ;
    }
    else
    {
        iSameChannel = M4_FALSE ;
        
        pccHostInstance = ai_poThisAccess->GetpCMCR()->GetNodeT3AliasInstanceIdByAlias( ai_hThisNode, ai_pszChannel ) ;
        
        if( pccHostInstance == NULL )
        {
			//el copyregister es porque es desde donde se llama, que es lo que el user tiene que saber
    		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_SUCH_INSTANCE, ai_pszChannel<<ai_poThisAccess->GetpCMCR()->iNodeN(ai_hThisNode)<<LogCatAnd<<ai_poThisAccess->GetpCMCR()->nNodeN(ai_hThisNode)<<"CopyRegister" << DumpCallStack() );
            return( M4_ERROR ) ;
        }
        
        ret = m_poVM->GetL2Access( ai_poThisAccess->GetpChannel(), pccHostInstance, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState1.m_Flags), &ao_poAccess ) ;
        
        if( ret != M4_SUCCESS )
        {
			//el copyregister es porque es desde donde se llama, que es lo que el user tiene que saber
			DUMP_CHLOG_ERRORF(M4_CH_VM_NO_GET_ACCESS_L2, pccHostInstance<<"CopyRegister" << DumpCallStack() );
            return( M4_ERROR ) ;
        }
    }
    
    
    ClCompiledMCR   *poCMCR=ao_poAccess-> GetpCMCR() ;
    
    
    //nodo
    if ( ai_pszNode == NULL ){
		//el copyregister es porque es desde donde se llama, que es lo que el user tiene que saber
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "NodeId"<<"CopyRegister"<<"<null>" << DumpCallStack() );
        return M4_ERROR;
    }
    
    //obtenemos el nodo
    if( iSameChannel == M4_TRUE )
    {
        hNode   = poCMCR->GetNodeAliasNodeHandleByAlias ( ai_hThisNode, ai_pszNode, m_oState1.m_poFormula.GetLevel() ) ;
    }
    else
    { 
        hNode   = poCMCR->GetChannelNodeHandleById ( ai_pszNode ) ;
        
    }
    
    if( hNode == 0 ){
		//el copyregister es porque es desde donde se llama, que es lo que el user tiene que saber
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, ai_pszNode <<"CopyRegister" << DumpCallStack() );
        return M4_ERROR;
    }
    
    ao_poNode=&( ao_poAccess->Node[ ClHandle( hNode ) ] ) ;
    
    return M4_SUCCESS;
    
	
}


//------------------------------------
//Serialización avanzada
//------------------------------------
m4return_t ClVM1Register::SendRegisterMark (void){
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SendRegisterMark");
#endif

	m4VariantType vNumArg,vBranchMode;
	m4int32_t	  iNumArgs,i;
	m4return_t	  ret;

	//número de argumentos
	ret= RuntimePopNumber (vNumArg);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SendRegisterMark.")	;
	iNumArgs=(m4int32_t) vNumArg.Data.DoubleData;

    //demasiados parametros ->Borramos los que sobran, más mensaje de aviso
    if ( iNumArgs> 1 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "SendRegisterMark"<<1<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 1 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SendRegisterMark")	;
        }
        iNumArgs=1; //ya hemos sacado los sobrantes
    }


	if (iNumArgs==1){
		//sacamos el BranchMode
		ret = RuntimePopNumber (vBranchMode);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "BranchMode" << "SendRegisterMark" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vBranchMode.Type)<< DumpCallStack() );
	}
	else{
		vBranchMode.Type=M4CL_CPP_TYPE_NUMBER;
		vBranchMode.Data.DoubleData=M4_FALSE;
	}

	//llamaos a la función
	m_oState1.m_poRecordSet->GetCurrentBlock()->MarkRecordAndParentRecordsForSerialize(m_oState1.m_poRecordSet->Current.Index, vBranchMode.Data.DoubleData!=0.0?M4_TRUE:M4_FALSE );

	//metemos el resuldato
	RuntimePushDouble (M4_SUCCESS);
	return M4_SUCCESS;

}


m4return_t ClVM1Register::SendBlockMark    (void){
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SendBlockMark");
#endif

	m4VariantType vNumArg,vBranchMode;
	m4int32_t	  iNumArgs,i;
	m4return_t	  ret;

	//número de argumentos
	ret= RuntimePopNumber (vNumArg);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SendBlockMark.")	;
	iNumArgs=(m4int32_t) vNumArg.Data.DoubleData;

    //demasiados parametros ->Borramos los que sobran, más mensaje de aviso
    if ( iNumArgs> 1 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "SendBlockMark"<<1<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 1 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SendBlockMark")	;
        }
        iNumArgs=1; //ya hemos sacado los sobrantes
    }
	
	
	if (iNumArgs==1){
		//sacamos el BranchMode
		ret = RuntimePopNumber (vBranchMode);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "BranchMode" << "SendBlockMark" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vBranchMode.Type)<< DumpCallStack() );
	}
	else{
		vBranchMode.Type=M4CL_CPP_TYPE_NUMBER;
		vBranchMode.Data.DoubleData=M4_FALSE;
	}

	//llamaos a la función
	m_oState1.m_poRecordSet->GetCurrentBlock()->MarkBlockAndParentRecordsForSerialize(vBranchMode.Data.DoubleData!=0.0?M4_TRUE:M4_FALSE);

	//metemos el resuldato

	RuntimePushDouble (M4_SUCCESS);
	return M4_SUCCESS;

}

m4return_t ClVM1Register::SendNodeMark    (void){
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SendNodeMark");
#endif


	m4VariantType vNumArg,vBranchMode;
	m4int32_t	  iNumArgs,i;
	m4return_t	  ret;

	//número de argumentos
	ret= RuntimePopNumber (vNumArg);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SendNodeMark.")	;
	iNumArgs=(m4int32_t) vNumArg.Data.DoubleData;

    //demasiados parametros ->Borramos los que sobran, más mensaje de aviso
    if ( iNumArgs> 1 ){
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, "SendNodeMark"<<1<<iNumArgs << DumpCallStack() );
        //sacamos los que sobran
        for (i = 0; i < iNumArgs - 1 ; ++i){
            ret=RuntimePopAndDelete();
            DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "SendNodeMark")	;
        }
        iNumArgs=1; //ya hemos sacado los sobrantes
    }

	if (iNumArgs==1){
		//sacamos el BranchMode
		ret = RuntimePopNumber (vBranchMode);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "BranchMode" << "SendNodeMark" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vBranchMode.Type)<< DumpCallStack() );
	}
	else{
		vBranchMode.Type=M4CL_CPP_TYPE_NUMBER;
		vBranchMode.Data.DoubleData=M4_FALSE;
	}
	
	//llamaos a la función
	m_oState1.m_poRecordSet->GetpNode_Data()->MarkNodeAndParentRecordsForSerialize(vBranchMode.Data.DoubleData!=0.0?M4_TRUE:M4_FALSE );

	//metemos el resuldato

	RuntimePushDouble (M4_SUCCESS);
	return M4_SUCCESS;}


//------------------------------------
//TimeOut's 
//------------------------------------
m4return_t ClVM1Register::SetCSTimeOut    (void){

#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SetCSTimeOut");
#endif

	m4VariantType vTime,vType;
	m4return_t	  ret;


	// sacamos los argumentos
	ret=RuntimePopDouble(vType);
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, DEBUGINFOLOG, "Type" << "SetCSTimeOut." << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vType.Type)<<DumpCallStack() );

	ret=RuntimePopDouble(vTime);
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, DEBUGINFOLOG, "Time" << "SetCSTimeOut." << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vTime.Type)<<DumpCallStack() );

	m4uint32_t iTimeOut = ((m4uint32_t)vTime.Data.DoubleData)*1000;

	// invocamos la función set
	ClVMBaseEnv *poEnv = m_oState1.m_poAccess->GetEnvironment();

	if (GetVMCSContext()->IsClient() ){

		switch((m4int32_t) vType.Data.DoubleData)
		{
		case 0: // default CS timeout
			poEnv->SetDefaultCSTimeOut(iTimeOut);
			break;
			
		case 1: // next CS timeout
			poEnv->SetNextCSTimeOut(iTimeOut); 
			break;
			
		case 2: // generic timeout
			poEnv->SetGenCSTimeOut(iTimeOut);
			break;
		}
	}
	else{

		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_FUNCT_CONTEXT, "SetCSTimeOut");
	}


	RuntimePushDouble (M4_SUCCESS);

	return M4_SUCCESS;

}

m4return_t ClVM1Register::GetCSTimeOut    (void){
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: GetCSTimeOut");
#endif

	m4VariantType vTime,vType;
	m4return_t	  ret;


	// sacamos los argumentos
	ret=RuntimePopDouble(vType);
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, DEBUGINFOLOG, "Type" << "GetCSTimeOut." << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vType.Type)<<DumpCallStack() );

	// invocamos la función get
	m4uint32_t iTimeOut=0;

	ClVMBaseEnv *poEnv = m_oState1.m_poAccess->GetEnvironment();

	if (GetVMCSContext()->IsClient()){

		switch((m4int32_t) vType.Data.DoubleData)
		{
		case 0: // default CS timeout
			iTimeOut = poEnv->GetDefaultCSTimeOut();
			break;
			
		case 1: // next CS timeout
			iTimeOut = poEnv->GetNextCSTimeOut();
			break;
			
		case 2: // generic timeout
			iTimeOut = poEnv->GetGenCSTimeOut();
			break;
		}
	}
	else{

		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_FUNCT_CONTEXT, "GetCSTimeOut");
	}

	// el resultado
	RuntimePushDouble (iTimeOut/1000);
	return M4_SUCCESS;
}

m4return_t ClVM1Register::ConfigCSTimeOut(void)
{
#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: ConfigCSTimeOut");
#endif

	m4VariantType vNumArg,vFlag;
	m4int32_t	  iNumArg,i;
	m4return_t	  ret;

	//número de argumentos
	ret= RuntimePopNumber (vNumArg);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "ConfigCSTimeOut.")	;
	iNumArg=(m4int32_t) vNumArg.Data.DoubleData;

	if (iNumArg >2){
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, "ConfigCSTimeOut"<<"1-2" << DumpCallStack() );
		return M4_ERROR;
	}

	//sacamos los argumentos sobrantes
	for (i=0 ; i< iNumArg-2; ++i){
		RuntimePopAndDelete();
	}

	m4int16_t iType=0;

	// sacamos los argumentos
	if (iNumArg>=2){
		m4VariantType vType;
		ret=RuntimePopDouble(vType);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, DEBUGINFOLOG, "Type" << "ConfigCSTimeOut" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vType.Type)<<DumpCallStack() );
		iType= (vType.Data.DoubleData ? 1 : 0);
	}

	ret=RuntimePopDouble(vFlag);
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, DEBUGINFOLOG, "Flag" << "ConfigCSTimeOut" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vFlag.Type)<< DumpCallStack() );
	m4bool_t bFlag = (vFlag.Data.DoubleData ? M4_TRUE : M4_FALSE);

	ClVMBaseEnv *poEnv =m_oState1.m_poAccess->GetEnvironment();
	// invocamos las funciones de configuración
	if (GetVMCSContext()->IsClient() ){

		switch(iType)
		{
		case 0: // reset timeout
			if (bFlag == M4_TRUE)
				poEnv->ResetNextCSTimeOut();
			break;
			
		case 1: // next CS timeout
			GetVMCSContext()->GetObjContext()->EnableTimeOut(bFlag);
			break;
		}
	}
	else{
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_FUNCT_CONTEXT, "ConfigCSTimeOut");
	}

	RuntimePushDouble (M4_SUCCESS);

	return M4_SUCCESS;

}

m4return_t ClVM1Register::SetCSTimeoutFunction(void)
{

#if _M4_DEBUG > 10 
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1: SetCSTimeoutFunction");
#endif


	m4return_t	  ret;
	m4VariantType vName,vFunc;
	//Sacamos argumentos
	ret=RuntimePopString(vFunc);
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, DEBUGINFOLOG, "DllFunction" << "SetCSTimeoutFunction" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vFunc.Type)<<DumpCallStack() );

	ret=RuntimePopString(vName);
	M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, DEBUGINFOLOG, "DllName" << "SetCSTimeoutFunction" << M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<M4ClCppType(vName.Type)<<DumpCallStack() );

	// invocamos las funciones de configuración
	
	if (GetVMCSContext()->IsClient()){
		ret = GetVMCSContext()->GetObjContext()->SetCSTimeoutFunction(m_oDynamicPool->GetString (vName.Data.CharData), m_oDynamicPool->GetString (vFunc.Data.CharData) );
	}
	else{
		DUMP_CHLOG_WARNINGF(M4_CH_VM_WRONG_FUNCT_CONTEXT, "SetCSTimeoutFunction");
	}

	m_oDynamicPool->DeleteString (vName.Data.CharData);
	m_oDynamicPool->DeleteString (vFunc.Data.CharData);

	RuntimePushDouble (ret);

	return M4_SUCCESS;
}


