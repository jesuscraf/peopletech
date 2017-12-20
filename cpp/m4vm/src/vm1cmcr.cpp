//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:			Compiler
// File:			vm1cmcr.cpp
// Project:			Calculo
// Author:			Meta Software M.S. , S.A
// Date:            26 jan 98
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    Virtual Machine de nivel 1 (CMCR Functions (Obtener Metadatos)
//	  Ejecucion de Nivel 0
//==============================================================================

#include "vm1cmcr.hpp"
#include "vmres.hpp"
#include "access.hpp"
#include "nodedef.hpp"
#include "m4mdrt.hpp"
#include "m4objglb.hpp"
#include "vmachin1.h"
#include "replaceliterals.hpp"
#include "cllstr.hpp"


// Funcion LN4: GetChannelId()	- ""
//	- Entrada: Ninguna
//	- Salida:  Nombre del canal (char)
m4return_t ClVM1Mcr::GetChannelId (void)
{
    m4return_t  ret=M4_SUCCESS;	
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1:GetChannelId ");
#endif
	// Obtenemos el puntero al MetaCanal
	ClCompiledMCR   *poCMCR = m_oState1.m_poAccess-> GetpCMCR() ;
	M4_ASSERT(poCMCR);
	
	// Devolvemos el ID	del Canal	
	RuntimePushString ( poCMCR-> GetChannelId() );
	return ret;
}


m4return_t ClVM1Mcr::GetChannelNumberOfNodes (void)
{
	return _GetChannelData(M4_CH_VM_CHANNELDEF_NODE_NUMBER, "GetChannelNumberOfNodes", M4_TRUE);
}


// Funcion LN4: GetNodeNumberOfItems()	- "RR"
//	- Entrada: nodo	 = String=Nombre del Nodo (char) o Alias o Nombre de TI ... ó
//					   Double=Index
//			   scope = Filtro a aplicar M4_REGISTER+M4_BLOCK+ ... etc.
//	- Salida:  No.de items que cumplen el "filtro"
m4return_t ClVM1Mcr::GetNodeNumberOfItems (void)
{
    m4VariantType	vNodo, vScope ; 
	m4uint32_t		hNode, hMyNode;
	ClCompiledMCR *	poCMCR;
	ClNode *		poNode ;
	m4char_t *		pszNode ;
	m4uint32_t		iNumberOfItems;
    m4return_t  ret=M4_SUCCESS;	
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1:GetNodeNumberOfItems  ");
#endif
    //2o.arg: Scope o Filtro: M4_REGISTER, M4_BLOCK, M4_NODE, M4_CONCEPT, M4_PROPERTY, M4_METHOD, M4_FIELD
    ret=RuntimePop(vScope);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetNodeNumberOfItems.")	;
	
    //1o.arg: Nombre del Nodo
    ret=RuntimePop(vNodo);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetNodeNumberOfItems.")	;
	
    //tipo del Nodo
    if (IsNull (vNodo) ){ //Null behaviour: Valor nulo->Nulo
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<"GetNodeNumberOfItems" << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(M4CL_CPP_TYPE_NULL)<<DumpCallStack() );
		DeleteVarString (vScope);
		RuntimePushDouble( M4_ERROR );
        return M4_ERROR;
    }
    if (vNodo.Type==M4CL_CPP_TYPE_NUMBER){	// Es el Index
        ret=m_oVariantMediator->ToDouble (vNodo);
    } else if (vNodo.Type!=M4CL_CPP_TYPE_STRING_POOL){ // Es el Nombre, Alias o TI
        ret=m_oVariantMediator->ToPoolString (vNodo);
    }
    if (ret!=M4_SUCCESS){       //Si no es cadena 
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<"GetNodeNumberOfItems" <<LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(vNodo.Type)<< DumpCallStack() );

		DeleteVarString (vNodo);
		DeleteVarString (vScope);
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	
    //tipo del Filtro
	if ( !CheckNumberIfErrorPush (vScope, M4_ERROR, M4_TRUE) )   {	//Error
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Scope"<<"GetNodeNumberOfItems" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)  <<M4ClCppType(vScope.Type) <<DumpCallStack() );

		DeleteVarString (vNodo);		
		return M4_SUCCESS;
	}
	m4uint32_t uTypeAndScope = (m4uint32_t)vScope.Data.DoubleData;
	uTypeAndScope &= ( ~( M4CL_ITEM_TYPE_ALL_FLAG | M4CL_ITEM_SCOPE_ALL_FLAG ) );	//Filtramos la parte baja de los bits
	if (vScope.Data.DoubleData<=0.0 || uTypeAndScope != 0) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "Scope"<<"GetNodeNumberOfItems" << uTypeAndScope << DumpCallStack() );
		DeleteVarString (vNodo);
		RuntimePushDouble(M4_ERROR);
		return M4_SUCCESS;
	}


	// ********* Proceso ***********
	
	// Obtenemos el Puntero al Nodo
	if (vNodo.Type==M4CL_CPP_TYPE_NUMBER) {
		// Es el Index
		poNode = &( m_oState1.m_poAccess->Node[ ClNodeIndex( (m4uint32_t)vNodo.Data.DoubleData ) ] ) ;
		if ( poNode==NULL ){		
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_INDEX, (m4uint32_t)vNodo.Data.DoubleData <<"GetNodeNumberOfItems" << DumpCallStack() );

			DeleteVarString (vNodo);
			RuntimePushDouble (M4_ERROR);
			return M4_SUCCESS;
		}
	} else { 
		// Es el Nombre, Alias o TI		
		poCMCR=m_oState1.m_poAccess-> GetpCMCR() ;
		pszNode = m_oDynamicPool->GetString (vNodo.Data.CharData);
		hMyNode = poCMCR->GetItemNodeHandle( m_oState1.m_hItem ) ;
        DEBUG_CHECK_RES (hMyNode , M4_CH_VM_WRONG_THIS_NODE_HANDLE ,ERRORLOG, "GetNodeNumberOfItems" << DumpCallStack() );
		
		hNode   = poCMCR->GetNodeAliasNodeHandleByAlias( hMyNode, pszNode, m_oState1.m_poFormula.GetLevel() ) ;
		if( hNode == 0 ){			
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, pszNode <<"GetNodeNumberOfItems" << DumpCallStack() );
			DeleteVarString (vNodo);
			RuntimePushDouble( M4_ERROR ) ;
			return M4_SUCCESS;
		}
		poNode = &( m_oState1.m_poAccess->Node[ ClHandle( hNode ) ] ) ;
		if ( poNode==NULL ){		
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, pszNode <<"GetNodeNumberOfItems" << DumpCallStack() );
			DeleteVarString (vNodo);
			RuntimePushDouble (M4_ERROR);
			return M4_ERROR;
		}
	}

	// Obtenemos el Iterador de Item adecuado, segun el Scope pedido
    
    //EDU:
	//ClItem	rItItem;
    ClNoCallableItem rItItem;    

	//nos encargamos de conseguir los iteradores de los items
	m4uint8_t iScope = ((m4uint8_t)vScope.Data.DoubleData) & M4CL_ITEM_SCOPE_MASK_BITS ;
	m4uint8_t iType  = ((m4uint8_t)vScope.Data.DoubleData) & M4CL_ITEM_TYPE_MASK_BITS  ;

	iType >>= 3;	// Formato que espera el DM. Vease VMdefine.hpp e ItemInfo.hpp
		
	// Establecemos el Type y Scope en el iterador de Items
	//ao_rItItem = poNode->Item;
	rItItem  = (poNode->RecordSet).Register.Item;
	rItItem.SetMode (iScope, iType);

	// Contamos el No. de Items aplicando el filtro
	iNumberOfItems = rItItem.Count();	

	DeleteVarString (vNodo);
	
	// Devolvemos el No.de Items que verifican el filtro para dicho nodo
	RuntimePushDouble ( iNumberOfItems );
	return ret;
}


// Funcion LN4: GetNodeIndex()	- "R"
//	- Entrada: Nombre del Nodo o Alias o Ti (o Index- un poco tonto, no?)
//	- Salida:  Index del Nodo
m4return_t ClVM1Mcr::GetNodeIndex (void)
{	
    m4VariantType	vNodo ;
	ClNode *		poNode ;
	m4return_t  ret=M4_SUCCESS;	
	
#if _M4_DEBUG > 10
	DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1:GetNodeIndex  ");
#endif
    //1o.arg: Nombre del Nodo
    ret=RuntimePop(vNodo);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetNodeIndex.")	;
	
    //tipo del Nodo
    if (IsNull (vNodo) ){ //Null behaviour: Valor nulo->Nulo
        DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<"GetNodeIndex" << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(M4CL_CPP_TYPE_NULL)<<DumpCallStack() );
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
    if (vNodo.Type==M4CL_CPP_TYPE_NUMBER){
		// Es el Index
        ret=m_oVariantMediator->ToDouble (vNodo);
    } else if (vNodo.Type!=M4CL_CPP_TYPE_STRING_POOL){
		// Es el Nombre, Alias o TI
        ret=m_oVariantMediator->ToPoolString (vNodo);
    }
    if (ret!=M4_SUCCESS){       //Si no es cadena 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<"GetNodeIndex" <<LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(vNodo.Type)<< DumpCallStack() );
		DeleteVarString(vNodo);
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	
	// Obtenemos el Puntero al Nodo
	if (vNodo.Type==M4CL_CPP_TYPE_NUMBER) {
		// Es el Index
		poNode = &( m_oState1.m_poAccess->Node[ ClNodeIndex( (m4uint32_t)vNodo.Data.DoubleData ) ] ) ;		
		if ( poNode==NULL ){		
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_INDEX, (m4uint32_t)vNodo.Data.DoubleData <<"GetNodeIndex" << DumpCallStack() );

			DeleteVarString(vNodo);
			RuntimePushDouble (M4_ERROR);
			return M4_SUCCESS;
		}
	} else { 
		// Es el Nombre, Alias o TI		
		ClCompiledMCR *	poCMCR = m_oState1.m_poAccess-> GetpCMCR() ;
		M4_ASSERT(poCMCR);
		m4char_t *		pszNode= m_oDynamicPool->GetString (vNodo.Data.CharData);
		m4uint32_t		hMyNode= poCMCR->GetItemNodeHandle( m_oState1.m_hItem ) ;
        DEBUG_CHECK_RES (hMyNode , M4_CH_VM_WRONG_THIS_NODE_HANDLE ,ERRORLOG, "GetNodeIndex" << DumpCallStack() );
		m4uint32_t		hNode  = poCMCR->GetNodeAliasNodeHandleByAlias( hMyNode, pszNode, m_oState1.m_poFormula.GetLevel() ) ;
		if( hNode == 0 ) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, pszNode <<"GetNodeIndex" << DumpCallStack() );
			DeleteVarString(vNodo);
			RuntimePushDouble (M4_ERROR);
			return M4_SUCCESS;
		}
		poNode = &( m_oState1.m_poAccess->Node[ ClHandle( hNode ) ] ) ;
		if ( poNode==NULL ){		
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_NODE_NAME, pszNode <<"GetNodeIndex" << DumpCallStack() );
			DeleteVarString(vNodo);
			RuntimePushDouble (M4_ERROR);
			return M4_ERROR;
		}
	}
	
	DeleteVarString(vNodo);
	RuntimePushDouble ( poNode->GetpNodeDef()->Index() );
	
	return ret;
}

// Funcion LN4: GetThisNodeId()	- ""
//	- Entrada: ninguna
//	- Salida:  Id del Nodo (char)
m4return_t ClVM1Mcr::GetThisNodeId (void)
{
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1:GetThisNodeId  ");
#endif
	// Devolvemos el ID	del Nodo Actual
	RuntimePushString ( m_oState1.m_poRecordSet->GetpNodeDef()->Id() ) ;
	return M4_SUCCESS;
}


// Funcion LN4: GetNodeItemId()		-	3, "RRR"
//	- Entrada:	nodo	= String=Nombre del Nodo (char) o Alias o Nombre de TI ... ó
//						  Double=Index
//				pos		= Posicion del item en el filtro (0..NumberOfItems)
//				scope	= filtro
//	- Salida:	Id = Identificador del Item del Nodo
m4return_t ClVM1Mcr::GetNodeItemId (void)
{
    m4VariantType vNodo, vPos, vScope ;
    m4return_t  ret=M4_SUCCESS;	
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1:GetNodeItemId  ");
#endif
    //3o.arg: Scope o Filtro: M4_REGISTER, M4_BLOCK, M4_NODE, M4_CONCEPT, M4_PROPERTY, M4_METHOD, M4_FIELD
    ret=RuntimePop(vScope);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetNodeItemId.")	;
	
    //2o.arg: Posicion en la busqueda aplicando el filtro
    ret=RuntimePop(vPos);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetNodeItemId.")	;
	
    //1o.arg: Nombre del Nodo
    ret=RuntimePop(vNodo);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetNodeItemId.")	;
	
    //tipo del Nodo
    if (IsNull (vNodo) ){ //Null behaviour: Valor nulo->Nulo
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<"GetNodeItemId" <<LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(M4CL_CPP_TYPE_NULL)<< DumpCallStack() );
		DeleteVarString(vPos);
		DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
    if (vNodo.Type==M4CL_CPP_TYPE_NUMBER){
		// Es el Index
        ret=m_oVariantMediator->ToDouble (vNodo);
    } else if (vNodo.Type!=M4CL_CPP_TYPE_STRING_POOL){
		// Es el Nombre, Alias o TI
        ret=m_oVariantMediator->ToPoolString (vNodo);
    }
    if (ret!=M4_SUCCESS){       //Si no es cadena 
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<"GetNodeItemId" << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(vNodo.Type)<<DumpCallStack() );

		DeleteVarString(vNodo);
		DeleteVarString(vPos);
		DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	
    //Posicion
	if ( !CheckNumberIfErrorPush (vPos, M4_ERROR, M4_TRUE) )   {	//Error
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Position"<<"GetNodeItemId" <<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vPos.Type)<< DumpCallStack() );
		DeleteVarString(vNodo);
		DeleteVarString(vScope);
		return M4_SUCCESS;
	}
	
    //Filtro
	if ( !CheckNumberIfErrorPush (vScope, M4_ERROR, M4_TRUE) )   {	//Error
		g_oChLog<<BeginError(M4_CH_VM_PARAMETER_TYPE_MISMATCH)<<"scope"<<"GetNodeItemId"<<M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vScope.Type)<<EndLog;

		DeleteVarString(vNodo);
		return M4_SUCCESS;
	}
	m4uint32_t uTypeAndScope = (m4uint32_t)vScope.Data.DoubleData;
	uTypeAndScope &= ( ~( M4CL_ITEM_TYPE_ALL_FLAG | M4CL_ITEM_SCOPE_ALL_FLAG ) );	//Filtramos la parte baja de los bits
	if (vScope.Data.DoubleData<=0.0 || uTypeAndScope != 0) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "Scope"<<"GetNodeItemId" << uTypeAndScope << DumpCallStack() );
		DeleteVarString (vNodo);
		RuntimePushDouble(M4_ERROR);
		return M4_SUCCESS;
	}
	
	// ********* Proceso ***********
	
	//EDU:
    //ClItem	rItItem;
    ClNoCallableItem rItItem;
	
	if ( _FindNodeItemByPosScope( NULL, vNodo, (m4uint32_t)vPos.Data.DoubleData, (m4uint8_t)vScope.Data.DoubleData, rItItem, "GetNodeItemId", M4_TRUE ) == M4_TRUE ) {
		// Devolvemos el Id del Item del Nodo
		RuntimePushString( rItItem.ItemDef.Id() );
	} else {
		// Devolvemos el Id del Item del Nodo
		RuntimePushDouble( M4_ERROR );
		DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND_BY_SCOPE, "GetNodeItemId" <<(m4uint8_t)vScope.Data.DoubleData << (m4uint32_t)vPos.Data.DoubleData << DumpCallStack() );

        ret = M4_SUCCESS;
	}
	DeleteVarString(vNodo);
	return ret;
}

// Funcion LN4: GetNodeItemArgumentNumber()	- devuelve por referencia el No.de argumentos del item (fijos o variables)
//												y el No. de argumentos por referencia.
//				res = GetNodeItemArgumentNumber(&numberFix, &HasVarArg, &numberRef, nodo, IdItem)
//				res = GetNodeItemArgumentNumber(&numberFix, &HasVarArg, &numberRef, nodo, pos, scope)
//	- SalidaLN4 : numberFix = No.Argumentos fijos
//				  HasVarArg	= Indicador de si tiene argumentos variables (M4_TRUE | M4_FALSE)
//				  numberRef = No.Argumentos por Referencia (dentro del total en numberFix)
//	- EntradaLN4: nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				  IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//					El problema que existe con GetNodeItemPrecision es que si el item es "long", la precision es -1
//					y coincide con el valor de M4_ERROR, no pudiéndose distinguir la situación de error cuando el campo es "long"
//	- SalidaLN4:  return = M4_ERROR o M4_SUCCESS
m4return_t ClVM1Mcr::GetNodeItemArgumentNumber (void)
{
    m4VariantType vNodo, vPosID, vScope, vNumberFix ;
	m4int32_t	iNumArg;
    m4return_t  ret=M4_SUCCESS;	
	m4bool_t	bEncontrado;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1:GetNodeItemArgumentNumber ");
#endif

	m4pcchar_t pccNombreMetodo = "GetNodeItemArgumentNumber";
	//sacamos el numero de argumentos
	ret=RuntimePop(vNumberFix);
	DEBUG_CHECK_RES(ret==M4_SUCCESS,M4_CH_VM_RTS_POP, ERRORLOG, pccNombreMetodo)	;
	if (vNumberFix.Type!=M4CL_CPP_TYPE_NUMBER)
	{
		ret = m_oVariantMediator->ToDouble(vNumberFix);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "number_of_args" << "GetNodeItemArgumentNumber" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vNumberFix.Type)<<DumpCallStack() );
    }
	iNumArg=(m4int32_t)vNumberFix.Data.DoubleData;
	if (iNumArg<5){	//Al menos 5 argumentos
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, pccNombreMetodo<<"5-6" << DumpCallStack() );
		RuntimePopAndDelete();
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	// 5 o más argumentos
	if (iNumArg>=6) {
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, pccNombreMetodo<<6<<iNumArg << DumpCallStack() );
		//Sacamos todos los argumentos variables inutiles
		for ( m4int32_t iTempN=0; iTempN < iNumArg - 6; iTempN++ ) {
			ret=RuntimePopAndDelete();
			DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Error performing pop from RTS.")	;
		}
		//6o.arg: Scope o Filtro: M4_REGISTER, M4_BLOCK, M4_NODE, M4_CONCEPT, M4_PROPERTY, M4_METHOD, M4_FIELD
		ret=RuntimePop(vScope);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);
	}

    //5o.arg: Posicion en la busqueda aplicando el filtro
    ret=RuntimePop(vPosID);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);
	
    //4o.arg: Nombre del Nodo
    ret=RuntimePop(vNodo);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);

	//3o.arg: sacamos el parametro por referencia: no se usa el valor
	ret=RuntimePopAndDelete();
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);

	//2o.arg: sacamos el parametro por referencia: no se usa el valor
	ret=RuntimePopAndDelete();
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);

	//1o.arg: sacamos el parametro por referencia: no se usa el valor
	ret=RuntimePopAndDelete();
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);
	
    //tipo del Nodo
    if (IsNull (vNodo) ){ //Null behaviour: Valor nulo->Nulo
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<pccNombreMetodo <<LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(M4CL_CPP_TYPE_NULL)<< DumpCallStack() );
		DeleteVarString(vPosID);
		if (iNumArg>=6) DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
    if (vNodo.Type==M4CL_CPP_TYPE_NUMBER){
		// Es el Index
        ret=m_oVariantMediator->ToDouble (vNodo);
    } else if (vNodo.Type!=M4CL_CPP_TYPE_STRING_POOL){
		// Es el Nombre, Alias o TI
        ret=m_oVariantMediator->ToPoolString (vNodo);
    }
    if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<pccNombreMetodo << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(vNodo.Type)<<DumpCallStack() );
		DeleteVarString(vNodo);
		DeleteVarString(vPosID);
		if (iNumArg>=6) DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

	//Posicion o ID
	if ( iNumArg>=6 ){
		// Posicion del Item	(6 argumentos: Ref + Ref + Ref + Cadena + No. + No.)
		ret=m_oVariantMediator->ToDouble (vPosID);
	}else
	if (vPosID.Type!=M4CL_CPP_TYPE_STRING_POOL){
		// ID del Item			(5 argumentos: Ref + Ref + Ref + Cadena + Cadena)
		ret=m_oVariantMediator->ToPoolString (vPosID);
	}
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Position"<<pccNombreMetodo << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(vPosID.Type)<<DumpCallStack() );
		DeleteVarString(vNodo);
		DeleteVarString(vPosID);
		if (iNumArg>=6) DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
		return M4_ERROR;
	}
	
	//EDU:
	//ClItem rItItem;
	ClNoCallableItem rItItem;
	
	// ********* Proceso ***********
	if (iNumArg==5) {
		bEncontrado = _FindNodeItemById( NULL, vNodo, 
			m_oDynamicPool->GetString (vPosID.Data.CharData), 
			rItItem, pccNombreMetodo, M4_TRUE );
	} else {
		//Scope o Filtro
		if ( !CheckNumberIfErrorPush (vScope, M4_ERROR, M4_TRUE) )   {	//Error
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Scope"<<pccNombreMetodo << M4ClCppType(M4CL_CPP_TYPE_NUMBER)  <<M4ClCppType(vScope.Type) << DumpCallStack() );
			DeleteVarString(vNodo);
			DeleteVarString(vPosID);
			//RuntimePushDouble(M4_ERROR);	//return
			RuntimePushNull();		//ArgRef
			RuntimePushNull();		//HasVarArg
			RuntimePushNull();		//NumFix
			return M4_SUCCESS;
		}
		m4uint32_t uTypeAndScope = (m4uint32_t)vScope.Data.DoubleData;
		uTypeAndScope &= ( ~( M4CL_ITEM_TYPE_ALL_FLAG | M4CL_ITEM_SCOPE_ALL_FLAG ) );	//Filtramos la parte baja de los bits
		if (vScope.Data.DoubleData<=0.0 || uTypeAndScope != 0) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "Scope"<<pccNombreMetodo  << uTypeAndScope << DumpCallStack() );
			DeleteVarString (vNodo);
			RuntimePushDouble(M4_ERROR);	//return
			RuntimePushNull();		//ArgRef
			RuntimePushNull();		//HasVarArg
			RuntimePushNull();		//NumFix
			return M4_SUCCESS;
		}
		bEncontrado = _FindNodeItemByPosScope( NULL, vNodo, 
			(m4uint32_t)vPosID.Data.DoubleData, 
			(m4uint8_t)vScope.Data.DoubleData, 
			rItItem, pccNombreMetodo, M4_TRUE);
	}
	
	m4int32_t iNumArgFix = 0;
	m4int32_t iNumArgRef = 0;
	m4int32_t iHasVarArg = M4_FALSE;
	m4int32_t iRet  = M4_ERROR;
	ret = M4_SUCCESS;

	// Devolvemos el resultado
	if ( bEncontrado == M4_TRUE ) {
		iNumArgFix = rItItem.ItemDef.NumberOfArguments();			//Numero de argumentos fijos
		iHasVarArg = rItItem.ItemDef.VariableArguments();			//Argumentos variables (0/1)
		iNumArgRef = rItItem.ItemDef.NumberOfReferedArguments();	//Numero de argumentos variables
		iRet  = M4_SUCCESS;
	} else {
		if (iNumArg==5){ //por nombre
			if (vNodo.Type==M4CL_CPP_TYPE_NUMBER){
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, m_oDynamicPool->GetString(vPosID.Data.CharData)<<(m4uint32_t)vNodo.Data.DoubleData<<M4Obj(m_oState1.m_poAccess->GetpCMCR())<<pccNombreMetodo << DumpCallStack() );
			}
			else { 
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, m_oDynamicPool->GetString(vPosID.Data.CharData)<<m_oDynamicPool->GetString(vNodo.Data.CharData)<<M4Obj(m_oState1.m_poAccess->GetpCMCR())<<pccNombreMetodo << DumpCallStack() );
			}
		}
		else{ //por scope+posicion
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND_BY_SCOPE, pccNombreMetodo<<(m4uint8_t)vScope.Data.DoubleData << (m4uint32_t)vPosID.Data.DoubleData  << DumpCallStack() );
		}
	}
	
	DeleteVarString(vNodo);
	DeleteVarString(vPosID);

	RuntimePushDouble( iRet );			//return
	RuntimePushDouble( iNumArgRef );	//Arg.por referencia
	RuntimePushDouble( iHasVarArg );	//Arg.por referencia
	RuntimePushDouble( iNumArgFix );	//Arg.por referencia

	return ret;
}

// Funcion LN4: GetNodeItemArgument()	- devuelve por referencia el Id.Argumento, Tipo y si es por Referencia o no
//											para el argumento de la posicion indicada del item indicado.
//				res = GetNodeItemArgument(&IdArg, &TypeArg, &Prec, &IsRefArg, ArgPos, nodo, IdItem)
//				res = GetNodeItemArgument(&IdArg, &TypeArg, &Prec, &IsRefArg, ArgPos, nodo, pos, scope)
//	- SalidaLN4 : IdArg  = Id. del Argumento
//				  TypeArg = Tipo del Argumento: VARIANT_TYPE_STRING | VARIANT_TYPE_NUMBER | VARIANT_TYPE_DATE | VARIANT_TYPE_NULL
//				  Prec	 = Precision del Argumento (no usado normalmente, solo por front-end)
//				  IsRef  = Indicador de Argumento por referencia (M4_TRUE, M4_FALSE)
//	- EntradaLN4: ArgPos = Posicion del Argumento por el que preguntamos [0.. NumArg-1]
//				  nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				  IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//					El problema que existe con GetNodeItemPrecision es que si el item es "long", la precision es -1
//					y coincide con el valor de M4_ERROR, no pudiéndose distinguir la situación de error cuando el campo es "long"
//	- SalidaLN4:  return = M4_ERROR o M4_SUCCESS
m4return_t ClVM1Mcr::GetNodeItemArgument (void)
{
    m4VariantType vNodo, vPosID, vScope, vArgPos, vIdArg ;
	m4int32_t	iNumArg;
    m4return_t  ret=M4_SUCCESS;	
	m4bool_t	bEncontrado;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1:GetNodeItemArgument ");
#endif

	m4pcchar_t pccNombreMetodo = "GetNodeItemArgument";
	//sacamos el numero de argumentos
	ret=RuntimePop(vIdArg);
	DEBUG_CHECK_RES(ret==M4_SUCCESS,M4_CH_VM_RTS_POP, ERRORLOG, pccNombreMetodo)	;
	if (vIdArg.Type!=M4CL_CPP_TYPE_NUMBER)
	{
		ret = m_oVariantMediator->ToDouble(vIdArg);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "number_of_args" << "GetNodeItemArgument" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vIdArg.Type)<<DumpCallStack() );
    }
	iNumArg=(m4int32_t)vIdArg.Data.DoubleData;
	if (iNumArg<7){	//Al menos 7 argumentos
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, pccNombreMetodo<<"7-8" << DumpCallStack() );
		RuntimePopAndDelete();
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	// 7 o más argumentos
	if (iNumArg>=8) {
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, pccNombreMetodo<<8<<iNumArg << DumpCallStack() );
		//Sacamos todos los argumentos variables inutiles
		for ( m4int32_t iTempN=0; iTempN < iNumArg - 8; iTempN++ ) {
			ret=RuntimePopAndDelete();
			DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Error performing pop from RTS.")	;
		}
		//8o.arg: Scope o Filtro: M4_REGISTER, M4_BLOCK, M4_NODE, M4_CONCEPT, M4_PROPERTY, M4_METHOD, M4_FIELD
		ret=RuntimePop(vScope);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);
	}

    //7o.arg: Posicion en la busqueda aplicando el filtro
    ret=RuntimePop(vPosID);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);
	
    //6o.arg: Nombre del Nodo
    ret=RuntimePop(vNodo);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);

	//5o.arg: Posicion del Argumento
	ret=RuntimePop(vArgPos);
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);

	//4o.arg: sacamos el parametro por referencia: no se usa el valor
	ret=RuntimePopAndDelete();
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);

	//3o.arg: sacamos el parametro por referencia: no se usa el valor
	ret=RuntimePopAndDelete();
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);

	//2o.arg: sacamos el parametro por referencia: no se usa el valor
	ret=RuntimePopAndDelete();
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);

	//1o.arg: sacamos el parametro por referencia: no se usa el valor
	ret=RuntimePopAndDelete();
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);
	
    //tipo del Nodo
    if (IsNull (vNodo) ){ //Null behaviour: Valor nulo->Nulo
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<pccNombreMetodo <<LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(M4CL_CPP_TYPE_NULL)<< DumpCallStack() );
		DeleteVarString(vPosID);
		if (iNumArg>=8) DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
    if (vNodo.Type==M4CL_CPP_TYPE_NUMBER){
		// Es el Index
        ret=m_oVariantMediator->ToDouble (vNodo);
    } else if (vNodo.Type!=M4CL_CPP_TYPE_STRING_POOL){
		// Es el Nombre, Alias o TI
        ret=m_oVariantMediator->ToPoolString (vNodo);
    }
    if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<pccNombreMetodo << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(vNodo.Type)<<DumpCallStack() );
		DeleteVarString(vNodo);
		DeleteVarString(vPosID);
		if (iNumArg>=8) DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

	//Posicion o ID
	if ( iNumArg>=8 ){
		// Posicion del Item	(8 argumentos: Ref + Ref + Ref + Ref + ArgPos + Cadena + No. + No.)
		ret=m_oVariantMediator->ToDouble (vPosID);
	}else
	if (vPosID.Type!=M4CL_CPP_TYPE_STRING_POOL){
		// ID del Item			(7 argumentos: Ref + Ref + Ref + Ref + ArgPos + Cadena + Cadena)
		ret=m_oVariantMediator->ToPoolString (vPosID);
	}
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Position"<<pccNombreMetodo << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(vPosID.Type)<<DumpCallStack() );
		DeleteVarString(vNodo);
		DeleteVarString(vPosID);
		if (iNumArg>=8) DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
		return M4_ERROR;
	}

	//Arg.Position
	if ( !CheckNumberIfErrorPush (vArgPos, M4_ERROR, M4_TRUE) )   {	//Error
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "ArgPos"<<pccNombreMetodo << M4ClCppType(M4CL_CPP_TYPE_NUMBER)  <<M4ClCppType(vArgPos.Type) << DumpCallStack() );
		DeleteVarString(vNodo);
		DeleteVarString(vPosID);
		if (iNumArg>=8) DeleteVarString(vScope);
		return M4_ERROR;
	}
	if (vArgPos.Data.DoubleData<0.0 || vArgPos.Data.DoubleData>=USHRT_MAX) {	//Fuera de rango
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE, "ArgPos"<<pccNombreMetodo << vArgPos.Data.DoubleData << DumpCallStack() );
		DeleteVarString(vNodo);
		DeleteVarString(vPosID);
		if (iNumArg>=8) DeleteVarString(vScope);
		return M4_ERROR;
	}
	m4uint16_t uiArgPos = (m4uint16_t) vArgPos.Data.DoubleData;

	//EDU:
	//ClItem rItItem;
	ClNoCallableItem rItItem;
	
	// ********* Proceso ***********
	if (iNumArg==7) {
		bEncontrado = _FindNodeItemById( NULL, vNodo, 
			m_oDynamicPool->GetString (vPosID.Data.CharData), 
			rItItem, pccNombreMetodo, M4_TRUE );
	} else {
		//Scope o Filtro
		if ( !CheckNumberIfErrorPush (vScope, M4_ERROR, M4_TRUE) )   {	//Error
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Scope"<<pccNombreMetodo << M4ClCppType(M4CL_CPP_TYPE_NUMBER)  <<M4ClCppType(vScope.Type) << DumpCallStack() );
			DeleteVarString(vNodo);
			DeleteVarString(vPosID);
			//RuntimePushDouble(M4_ERROR);//return
			RuntimePushNull();		//IsRefArg
			RuntimePushNull();		//Precision
			RuntimePushNull();		//tipo M4
			RuntimePushNull();		//Id.arg
			return M4_SUCCESS;
		}
		m4uint32_t uTypeAndScope = (m4uint32_t)vScope.Data.DoubleData;
		uTypeAndScope &= ( ~( M4CL_ITEM_TYPE_ALL_FLAG | M4CL_ITEM_SCOPE_ALL_FLAG ) );	//Filtramos la parte baja de los bits
		if (vScope.Data.DoubleData<=0.0 || uTypeAndScope != 0) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "Scope"<<pccNombreMetodo  << uTypeAndScope << DumpCallStack() );
			DeleteVarString(vNodo);
			RuntimePushDouble(M4_ERROR);//return
			RuntimePushNull();		//IsRefArg
			RuntimePushNull();		//Precision
			RuntimePushNull();		//tipo M4
			RuntimePushNull();		//Id.arg
			return M4_SUCCESS;
		}
		bEncontrado = _FindNodeItemByPosScope( NULL, vNodo, 
			(m4uint32_t)vPosID.Data.DoubleData, 
			(m4uint8_t)vScope.Data.DoubleData, 
			rItItem, pccNombreMetodo, M4_TRUE);
	}
	m4pcchar_t	pcIdArg = "";
	m4uint8_t	iTypeArg = 0;
	m4uint16_t	iPrecision = 0;
	m4bool_t	bIsRefArg = M4_FALSE;
	m4int32_t	iRet  = M4_ERROR;
	ret = M4_SUCCESS;

	// Devolvemos el resultado
	if ( bEncontrado == M4_TRUE ) {
		//Validamos la posicion del argumento indicado
		m4uint16_t uiNumArg = rItItem.ItemDef.NumberOfArguments();
		if ( uiArgPos >= uiNumArg || rItItem.ItemDef.ArgumentIdByPosition(uiArgPos)==0 ) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "ArgPos"<<pccNombreMetodo << uiArgPos << DumpCallStack() );
			DeleteVarString(vNodo);
			DeleteVarString(vPosID);
			if (iNumArg>=8) DeleteVarString(vScope);
			RuntimePushDouble(iRet);//return
			RuntimePushNull();		//IsRefArg
			RuntimePushNull();		//Precision
			RuntimePushNull();		//tipo M4
			RuntimePushNull();		//Id.arg
			return M4_SUCCESS;
		}
		pcIdArg		= rItItem.ItemDef.ArgumentIdByPosition(uiArgPos);
		iTypeArg	= ConvertM4TypeToCppType(rItItem.ItemDef.ArgumentM4TypeByPosition(uiArgPos));
		iPrecision	= rItItem.ItemDef.ArgumentPrecisionByPosition(uiArgPos);
		bIsRefArg	= (rItItem.ItemDef.ArgumentTypeByPosition(uiArgPos)==M4CL_ITEM_ARGUMENT_TYPE_REFERENCE) ? M4_TRUE : M4_FALSE ;
		iRet  = M4_SUCCESS;
	} else {
		if (iNumArg==7){ //por nombre
			if (vNodo.Type==M4CL_CPP_TYPE_NUMBER){
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, m_oDynamicPool->GetString(vPosID.Data.CharData)<<(m4uint32_t)vNodo.Data.DoubleData<<M4Obj(m_oState1.m_poAccess->GetpCMCR())<<pccNombreMetodo << DumpCallStack() );
			}
			else { 
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, m_oDynamicPool->GetString(vPosID.Data.CharData)<<m_oDynamicPool->GetString(vNodo.Data.CharData)<<M4Obj(m_oState1.m_poAccess->GetpCMCR())<<pccNombreMetodo << DumpCallStack() );
			}
		}
		else{ //por scope+posicion
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND_BY_SCOPE, pccNombreMetodo<<(m4uint8_t)vScope.Data.DoubleData << (m4uint32_t)vPosID.Data.DoubleData  << DumpCallStack() );
		}
	}
	
	DeleteVarString(vNodo);
	DeleteVarString(vPosID);

	RuntimePushDouble( iRet );			//return
	RuntimePushDouble( bIsRefArg );		//Arg.por referencia
	RuntimePushDouble( iPrecision );	//Arg.por referencia
	RuntimePushDouble( iTypeArg );		//Arg.por referencia
	RuntimePushString( pcIdArg );		//Arg.por referencia

	return ret;
}

// Funcion LN4: GetNodeItemPrecisionEx()	- equivalente a GetNodeItemPrecision, salvo que el valor lo devuelve por referencia.
//				res = GetNodeItemPrecisionEx(&prec, nodo, IdItem)
//				res = GetNodeItemPrecisionEx(&prec, nodo, pos, scope)
//	- SalidaLN4 : prec   = Precision		prec   = Precision del Item
//	- EntradaLN4: nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				  IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//					El problema que existe con GetNodeItemPrecision es que si el item es "long", la precision es -1
//					y coincide con el valor de M4_ERROR, no pudiéndose distinguir la situación de error cuando el campo es "long"
//	- SalidaLN4:  return = M4_ERROR o M4_SUCCESS
m4return_t ClVM1Mcr::GetNodeItemPrecisionEx (void)
{
    m4VariantType vNodo, vPosID, vScope, vPrec ;
	m4int32_t	iNumArg;
    m4return_t  ret=M4_SUCCESS;	
	m4bool_t	bEncontrado;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1:GetNodeItemPrecisionEx  ");
#endif

	m4pcchar_t pccNombreMetodo = "GetNodeItemPrecisionEx";
	//sacamos el numero de argumentos
	ret=RuntimePop(vPrec);
	DEBUG_CHECK_RES(ret==M4_SUCCESS,M4_CH_VM_RTS_POP, ERRORLOG, pccNombreMetodo)	;
	if (vPrec.Type!=M4CL_CPP_TYPE_NUMBER)
	{
		ret = m_oVariantMediator->ToDouble(vPrec);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "number_of_args" << "GetNodeItemPrecisionEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vPrec.Type)<<DumpCallStack() );
    }
	iNumArg=(m4int32_t)vPrec.Data.DoubleData;
	if (iNumArg<3){	//Al menos 3 argumentos
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, pccNombreMetodo<<"3-4" << DumpCallStack() );
		RuntimePopAndDelete();
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	// 3 o más argumentos
	if (iNumArg>=4) {
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, pccNombreMetodo<<4<<iNumArg << DumpCallStack() );
		//Sacamos todos los argumentos variables inutiles
		for ( m4int32_t iTempN=0; iTempN < iNumArg - 4; iTempN++ ) {
			ret=RuntimePopAndDelete();
			DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Error performing pop from RTS.")	;
		}
		//4o.arg: Scope o Filtro: M4_REGISTER, M4_BLOCK, M4_NODE, M4_CONCEPT, M4_PROPERTY, M4_METHOD, M4_FIELD
		ret=RuntimePop(vScope);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);
	}

    //3o.arg: Posicion en la busqueda aplicando el filtro
    ret=RuntimePop(vPosID);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);
	
    //2o.arg: Nombre del Nodo
    ret=RuntimePop(vNodo);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);

	//1o.arg: sacamos el parametro por referencia: no se lee para nada
	ret=RuntimePopAndDelete();
	DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);
	
    //tipo del Nodo
    if (IsNull (vNodo) ){ //Null behaviour: Valor nulo->Nulo
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<pccNombreMetodo <<LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(M4CL_CPP_TYPE_NULL)<< DumpCallStack() );
		DeleteVarString(vPosID);
		if (iNumArg>=4) DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
    if (vNodo.Type==M4CL_CPP_TYPE_NUMBER){
		// Es el Index
        ret=m_oVariantMediator->ToDouble (vNodo);
    } else if (vNodo.Type!=M4CL_CPP_TYPE_STRING_POOL){
		// Es el Nombre, Alias o TI
        ret=m_oVariantMediator->ToPoolString (vNodo);
    }
    if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<pccNombreMetodo << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(vNodo.Type)<<DumpCallStack() );
		DeleteVarString(vNodo);
		DeleteVarString(vPosID);
		if (iNumArg>=4) DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

	//Posicion o ID
	if ( iNumArg>=4 ){
		// Posicion del Item	(4 argumentos: Cadena + No. + No. + Ref)
		ret=m_oVariantMediator->ToDouble (vPosID);
	}else
	if (vPosID.Type!=M4CL_CPP_TYPE_STRING_POOL){
		// ID del Item			(3 argumentos: Cadena + Cadena + Ref)
		ret=m_oVariantMediator->ToPoolString (vPosID);
	}
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Position"<<pccNombreMetodo << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(vPosID.Type)<<DumpCallStack() );
		DeleteVarString(vNodo);
		DeleteVarString(vPosID);
		if (iNumArg>=4) DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
		return M4_ERROR;
	}
	
	//EDU:
	//ClItem rItItem;
	ClNoCallableItem rItItem;
	
	// ********* Proceso ***********
	if (iNumArg==3) {
		bEncontrado = _FindNodeItemById( NULL, vNodo, 
			m_oDynamicPool->GetString (vPosID.Data.CharData), 
			rItItem, pccNombreMetodo, M4_TRUE );
	} else {
		//Scope o Filtro
		if ( !CheckNumberIfErrorPush (vScope, M4_ERROR, M4_TRUE) )   {	//Error
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Scope"<<pccNombreMetodo << M4ClCppType(M4CL_CPP_TYPE_NUMBER)  <<M4ClCppType(vScope.Type) << DumpCallStack() );
			DeleteVarString(vNodo);
			DeleteVarString(vPosID);
			//RuntimePushDouble(M4_ERROR);
			RuntimePushNull();		//Precision
			return M4_SUCCESS;
		}
		m4uint32_t uTypeAndScope = (m4uint32_t)vScope.Data.DoubleData;
		uTypeAndScope &= ( ~( M4CL_ITEM_TYPE_ALL_FLAG | M4CL_ITEM_SCOPE_ALL_FLAG ) );	//Filtramos la parte baja de los bits
		if (vScope.Data.DoubleData<=0.0 || uTypeAndScope != 0) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "Scope"<<pccNombreMetodo  << uTypeAndScope << DumpCallStack() );
			DeleteVarString (vNodo);
			RuntimePushDouble(M4_ERROR);
			RuntimePushNull();		//Precision
			return M4_SUCCESS;
		}
		bEncontrado = _FindNodeItemByPosScope( NULL, vNodo, 
			(m4uint32_t)vPosID.Data.DoubleData, 
			(m4uint8_t)vScope.Data.DoubleData, 
			rItItem, pccNombreMetodo, M4_TRUE);
	}
	
	m4int32_t iPrec = M4_ERROR;
	m4int32_t iRet  = M4_ERROR;
	ret = M4_SUCCESS;

	// Devolvemos el resultado
	if ( bEncontrado == M4_TRUE ) {
		iPrec = rItItem.ItemDef.Precision();	//Precision del Item
		iRet  = M4_SUCCESS;
	} else {
		if (iNumArg==3){ //por nombre
			if (vNodo.Type==M4CL_CPP_TYPE_NUMBER){
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, m_oDynamicPool->GetString(vPosID.Data.CharData)<<(m4uint32_t)vNodo.Data.DoubleData<<M4Obj(m_oState1.m_poAccess->GetpCMCR())<<pccNombreMetodo << DumpCallStack() );
			}
			else { 
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, m_oDynamicPool->GetString(vPosID.Data.CharData)<<m_oDynamicPool->GetString(vNodo.Data.CharData)<<M4Obj(m_oState1.m_poAccess->GetpCMCR())<<pccNombreMetodo << DumpCallStack() );
			}
		}
		else{ //por scope+posicion
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND_BY_SCOPE, pccNombreMetodo<<(m4uint8_t)vScope.Data.DoubleData << (m4uint32_t)vPosID.Data.DoubleData  << DumpCallStack() );
		}
	}
	
	DeleteVarString(vNodo);
	DeleteVarString(vPosID);

	RuntimePushDouble( iRet );	//return
	RuntimePushDouble( iPrec );	//Arg.por referencia

	return ret;
}

// Funcion LN4: GetNodeItemArgumentPos()	- Devuelve la Posicion del Argumento indicado (en el Nodo.Item especificado)
//				res = GetNodeItemArgumentPos(nodo, IdItem, ArgId)
//				res = GetNodeItemArgumentPos(nodo, pos, scope, ArgId)
//	- EntradaLN4: nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				  IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//				  ArgId	 = ID. Argumento	ArgId  = ID. Argumento
//	- SalidaLN4:  return = M4_ERROR o la posicion del Argumento >=0
m4return_t ClVM1Mcr::GetNodeItemArgumentPos(void)
{
    m4VariantType vNodo, vPosID, vScope, vArgID ;
	m4int32_t	iNumArg;
    m4return_t  ret=M4_SUCCESS;	
	m4bool_t	bEncontrado;
	
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1:GetNodeItemArgumentPos  ");
#endif

	m4pcchar_t pccNombreMetodo = "GetNodeItemArgumentPos";
	//sacamos el numero de argumentos
	ret=RuntimePop(vArgID);
	DEBUG_CHECK_RES(ret==M4_SUCCESS,M4_CH_VM_RTS_POP, ERRORLOG, pccNombreMetodo)	;
	if (vArgID.Type!=M4CL_CPP_TYPE_NUMBER)
	{
		ret = m_oVariantMediator->ToDouble(vArgID);
		M4CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_PARAMETER_TYPE_MISMATCH, ERRORLOG, "number_of_args" << "GetNodeItemPrecisionEx" << M4ClCppType(M4CL_CPP_TYPE_NUMBER)<<M4ClCppType(vArgID.Type)<<DumpCallStack() );
    }
	iNumArg=(m4int32_t)vArgID.Data.DoubleData;
	if (iNumArg<3){	//Al menos 3 argumentos
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_NUM, pccNombreMetodo<<"3-4" << DumpCallStack() );
		RuntimePopAndDelete();
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
	// 3 o más argumentos
	if (iNumArg>=4) {
		DUMP_CHLOG_WARNINGF(M4_CH_VM_TOO_PARAM, pccNombreMetodo<<4<<iNumArg << DumpCallStack() );
		//Sacamos todos los argumentos variables inutiles
		for ( m4int32_t iTempN=0; iTempN < iNumArg - 4; iTempN++ ) {
			ret=RuntimePopAndDelete();
			DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "Error performing pop from RTS.")	;
		}
		//4o.arg: sacamos el ID. del Argumento
		ret=RuntimePop(vArgID);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);

		//3o.arg: Scope o Filtro: M4_REGISTER, M4_BLOCK, M4_NODE, M4_CONCEPT, M4_PROPERTY, M4_METHOD, M4_FIELD
		ret=RuntimePop(vScope);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);
	} else {
		//3o.arg: sacamos el ID. del Argumento
		ret=RuntimePop(vArgID);
		DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);
	}

    //2o.arg: Posicion en la busqueda aplicando el filtro
    ret=RuntimePop(vPosID);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);
	
    //1o.arg: Nombre del Nodo
    ret=RuntimePop(vNodo);
    DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, pccNombreMetodo);

    //tipo del Nodo
    if (IsNull (vNodo) ){ //Null behaviour: Valor nulo->Nulo
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<pccNombreMetodo <<LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(M4CL_CPP_TYPE_NULL)<< DumpCallStack() );
		DeleteVarString(vArgID);
		DeleteVarString(vPosID);
		if (iNumArg>=4) DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }
    if (vNodo.Type==M4CL_CPP_TYPE_NUMBER){
		// Es el Index
        ret=m_oVariantMediator->ToDouble (vNodo);
    } else if (vNodo.Type!=M4CL_CPP_TYPE_STRING_POOL){
		// Es el Nombre, Alias o TI
        ret=m_oVariantMediator->ToPoolString (vNodo);
    }
    if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node"<<pccNombreMetodo << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(vNodo.Type)<<DumpCallStack() );
		DeleteVarString(vArgID);
		DeleteVarString(vNodo);
		DeleteVarString(vPosID);
		if (iNumArg>=4) DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
        return M4_ERROR;
    }

	//Posicion o ID
	if ( iNumArg>=4 ){
		// Posicion del Item	(4 argumentos: Cadena + No. + No. + Cadena)
		ret=m_oVariantMediator->ToDouble (vPosID);
	}else
	if (vPosID.Type!=M4CL_CPP_TYPE_STRING_POOL){
		// ID del Item			(3 argumentos: Cadena + Cadena + Cadena)
		ret=m_oVariantMediator->ToPoolString (vPosID);
	}
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Position"<<pccNombreMetodo << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(vPosID.Type)<<DumpCallStack() );
		DeleteVarString(vArgID);
		DeleteVarString(vNodo);
		DeleteVarString(vPosID);
		if (iNumArg>=4) DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
		return M4_ERROR;
	}
	
	//ID. Argumento
	if (!CheckStringIfErrorPush(vArgID, M4_ERROR, M4_TRUE) ) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Id.Argument"<<pccNombreMetodo << LOG_CAT_BLOCK(M4ClCppType(M4CL_CPP_TYPE_STRING_POOL)<<","<<M4ClCppType(M4CL_CPP_TYPE_NUMBER) ) <<M4ClCppType(vArgID.Type)<<DumpCallStack() );
		DeleteVarString(vArgID);
		DeleteVarString(vNodo);
		DeleteVarString(vPosID);
		if (iNumArg>=4) DeleteVarString(vScope);
		RuntimePushDouble(M4_ERROR);
		return M4_SUCCESS;
	}

	//EDU:
	//ClItem rItItem;
	ClNoCallableItem rItItem;
	
	// ********* Proceso ***********
	if (iNumArg==3) {
		bEncontrado = _FindNodeItemById( NULL, vNodo, 
			m_oDynamicPool->GetString (vPosID.Data.CharData), 
			rItItem, pccNombreMetodo, M4_TRUE );
	} else {
		//Scope o Filtro
		if ( !CheckNumberIfErrorPush (vScope, M4_ERROR, M4_TRUE) )   {	//Error
			DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Scope"<<pccNombreMetodo << M4ClCppType(M4CL_CPP_TYPE_NUMBER)  <<M4ClCppType(vScope.Type) << DumpCallStack() );
			DeleteVarString(vArgID);
			DeleteVarString(vNodo);
			DeleteVarString(vPosID);
			return M4_SUCCESS;
		}
		m4uint32_t uTypeAndScope = (m4uint32_t)vScope.Data.DoubleData;
		uTypeAndScope &= ( ~( M4CL_ITEM_TYPE_ALL_FLAG | M4CL_ITEM_SCOPE_ALL_FLAG ) );	//Filtramos la parte baja de los bits
		if (vScope.Data.DoubleData<=0.0 || uTypeAndScope != 0) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "Scope" << pccNombreMetodo  << uTypeAndScope << DumpCallStack() );
			DeleteVarString(vArgID);
			DeleteVarString (vNodo);
			RuntimePushDouble(M4_ERROR);
			return M4_SUCCESS;
		}
		bEncontrado = _FindNodeItemByPosScope( NULL, vNodo, 
			(m4uint32_t)vPosID.Data.DoubleData, 
			(m4uint8_t)vScope.Data.DoubleData, 
			rItItem, pccNombreMetodo, M4_TRUE);
	}
	
	m4int32_t iRet  = M4_ERROR;
	ret = M4_SUCCESS;

	// Devolvemos el resultado
	if ( bEncontrado == M4_TRUE ) {
		iRet = (rItItem.ItemDef.GetpCMCR())->GetItemArgumentPositionById( rItItem.ItemDef.GetHandle(), m_oDynamicPool->GetString(vArgID.Data.CharData) );
		iRet = (iRet<0) ? M4_ERROR : iRet;
	} else {
		if (iNumArg==3){ //por nombre
			if (vNodo.Type==M4CL_CPP_TYPE_NUMBER){
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, m_oDynamicPool->GetString(vPosID.Data.CharData)<<(m4uint32_t)vNodo.Data.DoubleData<<M4Obj(m_oState1.m_poAccess->GetpCMCR())<<pccNombreMetodo << DumpCallStack() );
			}
			else { 
				DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND, m_oDynamicPool->GetString(vPosID.Data.CharData)<<m_oDynamicPool->GetString(vNodo.Data.CharData)<<M4Obj(m_oState1.m_poAccess->GetpCMCR())<<pccNombreMetodo << DumpCallStack() );
			}
		}
		else{ //por scope+posicion
			DUMP_CHLOG_ERRORF(M4_CH_VM_ITEM_NOT_FOUND_BY_SCOPE, pccNombreMetodo<<(m4uint8_t)vScope.Data.DoubleData << (m4uint32_t)vPosID.Data.DoubleData  << DumpCallStack() );
		}
	}
	
	DeleteVarString(vArgID);
	DeleteVarString(vNodo);
	DeleteVarString(vPosID);

	RuntimePushDouble( iRet );	//return

	return ret;
}


/*
_GetChannelData()

Los posibles argumentos son
0- 
1- M4Obj (string)
*/
m4return_t ClVM1Mcr::_GetChannelData( m4int8_t ai_ChannelDefData, m4pcchar_t ai_pccMethodName, m4bool_t ai_bDumpError )
{

	m4return_t		iResult = M4_SUCCESS ;
	m4bool_t		bModified = M4_FALSE ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iNumArg = 0 ;
	m4uint32_t		iMaxNumArg = 0 ;
	m4uint32_t		iPosition = 0 ;
    m4pcchar_t		pcString = NULL ;
	m4pchar_t		pcOut = NULL ;
	m4pchar_t		pcOrganization = NULL;
	m4VariantType	vPosition ;
	m4VariantType	vM4Obj ;
	m4VariantType	vValue ;
	m4VariantType	vOrganization ;
	ClAccess		*poAccess = NULL ;
	ClChannel		*poChannel = NULL ;
	ClChannelDef	*poChannelDef = NULL ;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF( M4_CH_VM_TRACE_VM_L1, "Trace VM1: " << LogCat << ai_pccMethodName ) ;
#endif


    // Leemos el número de argumentos (pueden ser 0 ó 1)
	iResult = RuntimePopNumber( vValue ) ;
    DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;

    iNumArg = m4uint32_t( vValue.Data.DoubleData ) ;

	// Limpiamos los argumentos de más
	if( ai_ChannelDefData == M4_CH_VM_CHANNELDEF_L2_INSTANCE_HANDLE )
	{
		iMaxNumArg = 2 ;
	}
	else
	{
		iMaxNumArg = 1 ;
	}

	if( iNumArg > iMaxNumArg )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_TOO_PARAM, ai_pccMethodName << iMaxNumArg + 1 << iNumArg + 1 << DumpCallStack() ) ;

		for( i = iMaxNumArg ; i < iNumArg ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
			DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;
		}
		iNumArg = iMaxNumArg ;
	}

	// En caso de pedir la instancia de canal se saca la posición
	if( ai_ChannelDefData == M4_CH_VM_CHANNELDEF_L2_INSTANCE_HANDLE )
	{
		iResult = RuntimePop( vPosition ) ;
		DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;

		if( vPosition.Type != M4CL_CPP_TYPE_NUMBER )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Instance position" << ai_pccMethodName << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vPosition.Type ) << DumpCallStack() ) ;
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}

		iPosition = vPosition.Data.DoubleData ;
		iNumArg-- ;
	}

	// Se consigue el acceso al canal. De momento el acceso es al propio
	poAccess = m_oState1.m_poAccess ;

	if( iNumArg > 0 )
	{
		iResult = RuntimePop( vM4Obj ) ;
		DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;

		if( vM4Obj.Type != M4CL_CPP_TYPE_STRING_POOL )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "M4Obj ID" << ai_pccMethodName << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( vM4Obj.Type ) << DumpCallStack() ) ;
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}

		poAccess = _FindChannel( vM4Obj, ai_pccMethodName, m_oState1 ) ;
		DeleteVarString( vM4Obj ) ;

		if( poAccess == NULL )
		{
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_SUCCESS ) ;
		}
	}

	poChannel = poAccess->GetpChannel() ;
	poChannelDef = poChannel->GetpChannelDef() ;

	// Devolvemos el resultado
	iResult = M4_SUCCESS ;

	switch( ai_ChannelDefData )
	{
		case M4_CH_VM_CHANNELDEF_NODE_NUMBER:
			RuntimePushDouble( poChannelDef->NumberOfNodes() ) ;
			break ;

		case M4_CH_VM_CHANNELDEF_CREATION_TYPE:
			RuntimePushDouble( poChannelDef->CreationType() ) ;
			break ;

		case M4_CH_VM_CHANNELDEF_ID:
			RuntimePushString( poChannelDef->Id() ) ;
			break ;

		case M4_CH_VM_CHANNELDEF_NAME:

			pcString = poChannelDef->Name() ;

			if( poChannel->Organization.Get( vOrganization ) == M4_SUCCESS && vOrganization.Type == M4CL_CPP_TYPE_STRING_VAR )
			{
				pcOrganization = vOrganization.Data.PointerChar ;
			}

			iResult = M4ReplaceLiterals( poChannel->GetpChannelManager(), pcString, pcOut, pcOrganization, bModified ) ;

			if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcOut != NULL )
			{
				RuntimePushString( pcOut ) ;
				delete pcOut;
			}
			else
			{
				RuntimePushString( pcString ) ;
			}
			break ;

		case M4_CH_VM_CHANNELDEF_ENVIRONMENT_VARS:
			RuntimePushDouble( poChannelDef->EnvironmentVars() ) ;
			break ;

		case M4_CH_VM_CHANNELDEF_L2_INSTANCE_NUMBER:
			RuntimePushDouble( poChannel->Level2.m_oInstanceMap.Count() ) ;
			break ;

		case M4_CH_VM_CHANNELDEF_L2_INSTANCE_HANDLE:
			RuntimePushDouble( poChannel->Level2.m_oInstanceMap.GetChannelHandle( iPosition ) ) ;
			break ;

		default:
			DUMP_CHLOG_ERRORF( M4_CH_VM_WRONG_PARAM_VALUE, "Data Type" << ai_pccMethodName << m4int32_t( ai_ChannelDefData ) << DumpCallStack() ) ;
			RuntimePushDouble( M4_ERROR ) ;
			iResult = M4_ERROR ;
			break ;
	}

	return( iResult ) ;
}


/*
_GetNodeData()

Los posibles argumentos son
0- 
1- Position (number)
1- Nodo (string)
2- M4Obj (string), Position (number)
2- M4Obj (string), Nodo (string)
*/
m4return_t ClVM1Mcr::_GetNodeData( m4int8_t ai_NodeDefData, m4pcchar_t ai_pccMethodName, m4bool_t ai_bDumpError )
{

	m4return_t		iResult = M4_SUCCESS ;
	m4bool_t		bModified = M4_FALSE ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iNumArg = 0 ;
    m4pcchar_t		pcString = NULL ;
	m4pchar_t		pcOut = NULL ;
	m4pchar_t		pcOrganization = NULL;
	m4VariantType	vNode ;
	m4VariantType	vM4Obj ;
	m4VariantType	vValue ;
	m4VariantType	vOrganization ;
	ClAccess		*poAccess = NULL ;
	ClChannel		*poChannel = NULL ;
	ClNode			*poNode = NULL ;
	ClNodeDef		*poNodeDef = NULL ;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF( M4_CH_VM_TRACE_VM_L1, "Trace VM1: " << LogCat << ai_pccMethodName ) ;
#endif


    // Leemos el número de argumentos (pueden ser 0, 1 ó 2)
	iResult = RuntimePopNumber( vValue ) ;
    DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;

    iNumArg = m4uint32_t( vValue.Data.DoubleData ) ;

	// Limpiamos los argumentos de más
	if( iNumArg > 2 )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_TOO_PARAM, ai_pccMethodName << 3 << iNumArg + 1 << DumpCallStack() ) ;

		for( i = 2 ; i < iNumArg ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
			DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;
		}
		iNumArg = 2 ;
	}


	if( iNumArg > 0 )
	{
		iResult = RuntimePop( vNode ) ;
		DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;
	}
	else
	{
		vNode.Type = M4CL_CPP_TYPE_NULL ;
	}

	// Se consigue el acceso al canal. De momento el acceso es al propio
	poAccess = m_oState1.m_poAccess ;

	if( iNumArg > 1 )
	{
		iResult = RuntimePop( vM4Obj ) ;
		DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;

		if( vM4Obj.Type != M4CL_CPP_TYPE_STRING_POOL )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "M4Obj ID" << ai_pccMethodName << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( vM4Obj.Type ) << DumpCallStack() ) ;
			DeleteVarString( vNode ) ;
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}

		poAccess = _FindChannel( vM4Obj, ai_pccMethodName, m_oState1 ) ;
		DeleteVarString( vM4Obj ) ;

		if( poAccess == NULL )
		{
			DeleteVarString( vNode ) ;
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_SUCCESS ) ;
		}
	}


	poNode = _FindNode( poAccess, vNode, ai_pccMethodName, m_oState1, ai_bDumpError ) ;
	DeleteVarString( vNode ) ;

	if( poNode == NULL )
	{
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_SUCCESS ) ;
	}

	poNodeDef = poNode->GetpNodeDef() ;

	// Devolvemos el resultado
	iResult = M4_SUCCESS ;

	switch( ai_NodeDefData )
	{
		case M4_CH_VM_NODEDEF_READ_OBJECT_ID:
			RuntimePushString( poNodeDef->GetNodeReadObjectId() ) ;
			break ;

		case M4_CH_VM_NODEDEF_WRITE_OBJECT_ID:
			RuntimePushString( poNodeDef->GetNodeWriteObjectId() ) ;
			break ;

		case M4_CH_VM_NODEDEF_ID:
			RuntimePushString( poNodeDef->Id() ) ;
			break ;

		case M4_CH_VM_NODEDEF_NAME:

			pcString = poNodeDef->Name() ;
			poChannel = poAccess->GetpChannel() ;

			if( poChannel->Organization.Get( vOrganization ) == M4_SUCCESS && vOrganization.Type == M4CL_CPP_TYPE_STRING_VAR )
			{
				pcOrganization = vOrganization.Data.PointerChar ;
			}

			iResult = M4ReplaceLiterals( poChannel->GetpChannelManager(), pcString, pcOut, pcOrganization, bModified ) ;

			if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcOut != NULL )
			{
				RuntimePushString( pcOut ) ;
				delete pcOut;
			}
			else
			{
				RuntimePushString( pcString ) ;
			}
			break ;

		case M4_CH_VM_NODEDEF_INDEX:
			RuntimePushDouble( poNodeDef->Index() ) ;
			break ;

		case M4_CH_VM_NODEDEF_IS_ROOT:
			RuntimePushDouble( poNodeDef->IsRoot() ) ;
			break ;

		case M4_CH_VM_NODEDEF_IS_REAL_ROOT:
			RuntimePushDouble( poNodeDef->IsRealRoot() ) ;
			break ;

		case M4_CH_VM_NODEDEF_PARENT:
			RuntimePushString( poNodeDef->ParentId() ) ;
			break ;

		case M4_CH_VM_NODEDEF_SENTENCE:
			RuntimePushString( poNodeDef->ReadSentenceApiSql() ) ;
			break ;

		case M4_CH_VM_NODEDEF_MAX_NUM_ROWS_DB:
			RuntimePushDouble( poNodeDef->NumRowsDB() ) ;
			break ;

		case M4_CH_VM_NODEDEF_TYPE:
			RuntimePushDouble( poNodeDef->NodeType() ) ;
			break ;

		case M4_CH_VM_NODEDEF_AUTO_FILTER:
			RuntimePushDouble( poNodeDef->IdAutoFilter() ) ;
			break ;

		default:
			DUMP_CHLOG_ERRORF( M4_CH_VM_WRONG_PARAM_VALUE, "Data Type" << ai_pccMethodName << m4int32_t( ai_NodeDefData ) << DumpCallStack() ) ;
			RuntimePushDouble( M4_ERROR ) ;
			iResult = M4_ERROR ;
			break ;
	}

	return( iResult ) ;
}



/*
_GetItemData()

Los posibles argumentos son
2- Nodo (string, number), Item (string)
3- Nodo (string, number), Position (number), Scope (number)
3- M4Obj (string), Nodo (string, number), Item (string)
4- M4Obj (string), Nodo (string, number), Position (number), Scope (number)
*/
m4return_t ClVM1Mcr::_GetItemData( m4int8_t ai_ItemDefData, m4uint16_t ai_iRule, m4pcchar_t ai_pccMethodName, m4bool_t ai_bDumpError )
{

	m4return_t			iResult = M4_SUCCESS ;
	m4bool_t			bFound = M4_FALSE ;
	m4bool_t			bScope = M4_FALSE ;
	m4bool_t			bM4Obj = M4_FALSE ;
	m4bool_t			bModified = M4_FALSE ;
	m4uint8_t			iScope = m4uint8_t( -1 ) ;
	m4uint32_t			iPosition = m4uint32_t( -1 ) ;
	m4uint32_t			i = 0 ;
	m4uint32_t			iNumArg = 0 ;
	m4uint32_t			hMyNode = 0 ;
	size_t				iLength = 0 ;
	m4pcchar_t			pccValue = NULL ;
	m4pcchar_t			pszThisChannel = NULL ;
	m4pcchar_t			pccHostInstance = NULL ;
    m4pcchar_t			pcString = NULL ;
	m4pchar_t			pcOut = NULL ;
	m4pchar_t			pcOrganization = NULL;
	m4pcchar_t			pccCode = NULL ;
	m4char_t			acItem [ M4CL_MAX_LEN_ITEM + 1 ] ;
	m4char_t			acM4Obj[ M4CL_MAX_LEN_CHANNEL + 1 ] ;
	m4VariantType		vNode ;
	m4VariantType		vValue ;
	m4VariantType		vOrganization ;
    ClCompiledMCR		*poThisCMCR = NULL ;
	ClAccess			*poAccess = NULL ;
	ClChannel			*poChannel = NULL ;
	ClNoCallableItem	rItItem ;
	ClLongString		oString ;


#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF( M4_CH_VM_TRACE_VM_L1, "Trace VM1: " << LogCat << ai_pccMethodName ) ;
#endif


	*acM4Obj = '\0' ;
	*acItem = '\0' ;


    // Leemos el número de argumentos (pueden ser 2, 3 ó 4)
	iResult = RuntimePopNumber( vValue ) ;
    DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;

    iNumArg = m4uint32_t( vValue.Data.DoubleData ) ;

	if( iNumArg < 2 )
	{
		// Si hay menos de 2 argumentos se borran y se da error (teniendo en cuenta un argumento más)
		DUMP_CHLOG_ERRORF( M4_CH_VM_WRONG_PARAM_NUM, ai_pccMethodName << "3-4-5" << DumpCallStack() ) ;

		for( i = 0 ; i < iNumArg ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
		}
		RuntimePushDouble( M4_ERROR ) ;
        return( M4_ERROR ) ;
	}


	// Limpiamos los argumentos de más
	if( iNumArg > 4 )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_TOO_PARAM, ai_pccMethodName << 5 << iNumArg + 1 << DumpCallStack() ) ;

		for( i = 4 ; i < iNumArg ; i++ )
		{
			iResult = RuntimePopAndDelete() ;
			DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;
		}
		iNumArg = 4 ;
	}


	// El cuarto argumento
	if( iNumArg >= 4 )
	{
		// Si tiene 4 argumentos es con scope y con canal
		bScope = M4_TRUE ;
		bM4Obj = M4_TRUE ;

		iResult = RuntimePop( vValue ) ;
		DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;

		if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Scope" << ai_pccMethodName << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
			DeleteVarString( vValue ) ;
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}

		iScope = m4uint8_t( vValue.Data.DoubleData ) ;
	}


	// El tercer argumento
	if( iNumArg >= 3 )
	{
		iResult = RuntimePop( vValue ) ;
		DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;

		if( bScope == M4_TRUE )
		{
			// Si tiene 3 argumentos y tenía scope el siguiente es la posición
			if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
			{
				DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Position" << ai_pccMethodName << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
				DeleteVarString( vValue ) ;
				RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
			}

			iPosition = m4uint32_t( vValue.Data.DoubleData ) ;
		}
		else
		{
			if( vValue.Type == M4CL_CPP_TYPE_NUMBER )
			{
				// Si tiene 3 argumentos y no tenía scope y es un número es el scope sin canal
				bScope = M4_TRUE ;

				iScope = m4uint8_t( vValue.Data.DoubleData ) ;
			}
			else
			{
				// Si tiene 3 argumentos y no tenía scope y es una cadena es el item con canal
				bM4Obj = M4_TRUE ;

				if( vValue.Type != M4CL_CPP_TYPE_STRING_POOL )
				{
					DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Item ID" << ai_pccMethodName << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
					RuntimePushDouble( M4_ERROR ) ;
					return( M4_ERROR ) ;
				}

				pccValue = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
				iLength = strlen( pccValue ) ;
				if( iLength > M4CL_MAX_LEN_ITEM )
				{
					iLength = M4CL_MAX_LEN_ITEM ;
				}
				memcpy( acItem, pccValue, iLength ) ;
				acItem[ iLength ] = '\0' ;
				DeleteVarString( vValue ) ;
			}
		}
	}


	// El segundo argumento
	iResult = RuntimePop( vValue ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;

	if( bM4Obj == M4_TRUE )
	{
		// Si tiene canal el anterior es el nodo
		vNode = vValue ;
	}
	else
	{
		if( bScope == M4_TRUE )
		{
			// Si no tiene canal y si scope es de 3 y el siguiente es la posición
			if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
			{
				DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Position" << ai_pccMethodName << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
				DeleteVarString( vValue ) ;
				RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
			}

			iPosition = m4uint32_t( vValue.Data.DoubleData ) ;
		}
		else
		{
			// Si no tiene canal ni scope es de 2 y el siguiente es el item
			if( vValue.Type != M4CL_CPP_TYPE_STRING_POOL )
			{
				DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Item ID" << ai_pccMethodName << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
				RuntimePushDouble( M4_ERROR ) ;
				return( M4_ERROR ) ;
			}

			pccValue = m_oDynamicPool->GetString( vValue.Data.CharData ) ;
			iLength = strlen( pccValue ) ;
			if( iLength > M4CL_MAX_LEN_ITEM )
			{
				iLength = M4CL_MAX_LEN_ITEM ;
			}
			memcpy( acItem, pccValue, iLength ) ;
			acItem[ iLength ] = '\0' ;
			DeleteVarString( vValue ) ;
		}
	}


	// El primer argumento
	iResult = RuntimePop( vValue ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, ai_pccMethodName ) ;

	if( bM4Obj == M4_TRUE )
	{
		// Si tiene canal es el primer argumento
		if( vValue.Type != M4CL_CPP_TYPE_STRING_POOL )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "M4Obj ID" << ai_pccMethodName << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
			DeleteVarString( vNode ) ;
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
	}
	else
	{
		// Si no tiene canal el primero es el nodo
		vNode = vValue ;
	}


	// Se consigue el acceso al canal. De momento el acceso es al propio
	poAccess = m_oState1.m_poAccess ;

	if( bM4Obj == M4_TRUE )
	{
		poThisCMCR = poAccess->GetpCMCR() ;
	    pszThisChannel = poThisCMCR->GetChannelId() ;

		if( *acM4Obj != '\0' && strcmpi( pszThisChannel, acM4Obj ) != 0 )
		{
			hMyNode = poThisCMCR->GetItemNodeHandle( m_oState1.m_hItem ) ;

			pccHostInstance = poThisCMCR->GetNodeT3AliasInstanceIdByAlias( hMyNode, acM4Obj ) ;
			
			if( pccHostInstance == NULL )
			{
	    		DUMP_CHLOG_ERRORF( M4_CH_VM_NO_SUCH_INSTANCE, acM4Obj<< poThisCMCR->iNodeN( hMyNode) <<LogCatAnd << poThisCMCR->nNodeN( hMyNode ) << ai_pccMethodName << DumpCallStack() ) ;
				DeleteVarString( vNode ) ;
				RuntimePushDouble( M4_ERROR ) ;
				return( M4_SUCCESS ) ;
			}

			iResult = m_poVM->GetL2Access( poAccess->GetpChannel(), pccHostInstance, M4CL_HAS_STATE_EXE_JIT_FLAG( m_oState1.m_Flags ), &poAccess ) ;
			
			if( iResult != M4_SUCCESS )
			{
				DUMP_CHLOG_ERRORF( M4_CH_VM_NO_GET_ACCESS_L2, pccHostInstance << ai_pccMethodName << DumpCallStack() ) ;
				DeleteVarString( vNode ) ;
				RuntimePushDouble( M4_ERROR ) ;
				return( M4_SUCCESS ) ;
			}
		}
    }


	if( bScope == M4_TRUE )
	{
		if( iScope & ~( M4CL_ITEM_TYPE_ALL_FLAG | M4CL_ITEM_SCOPE_ALL_FLAG ) )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_WRONG_PARAM_VALUE, "Scope" << ai_pccMethodName  << iScope << DumpCallStack() ) ;
			DeleteVarString( vNode ) ;
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_SUCCESS ) ;
		}

		bFound = _FindNodeItemByPosScope( poAccess, vNode, iPosition, iScope, rItItem, ai_pccMethodName, M4_TRUE ) ;
	}
	else
	{
		bFound = _FindNodeItemById( poAccess, vNode, acItem, rItItem, ai_pccMethodName, M4_TRUE ) ;
	}


	// Devolvemos el resultado
	iResult = M4_SUCCESS ;

	if( ai_ItemDefData == M4_CH_VM_ITEMDEF_ITEM_NUMBER )
	{
		RuntimePushDouble( rItItem.Count() ) ;
	}
	else
	{
		if( bFound == M4_TRUE )
		{
			switch( ai_ItemDefData )
			{
				case M4_CH_VM_ITEMDEF_TYPE:
					RuntimePushDouble( rItItem.ItemDef.Type() ) ;
					break ;
					
				case M4_CH_VM_ITEMDEF_SCOPE:
					RuntimePushDouble( rItItem.ItemDef.Scope() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_THEORIC_SCOPE:
					RuntimePushDouble( rItItem.ItemDef.TheoricScope() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_M4TYPE:
					RuntimePushDouble( rItItem.ItemDef.M4Type() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_PRECISION:
					RuntimePushDouble( rItItem.ItemDef.Precision() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_INDEX:
					RuntimePushDouble( rItItem.ItemDef.Index() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_SCALE:
					RuntimePushDouble( rItItem.ItemDef.Scale() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_INTERNAL_TYPE:
					RuntimePushDouble( rItItem.ItemDef.InternalType() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_READ_OBJECT_ALIAS:
					RuntimePushString( rItItem.ItemDef.ReadObjectAlias() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_WRITE_OBJECT_ALIAS:
					RuntimePushString( rItItem.ItemDef.WriteObjectAlias() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_READ_FIELD_ID:
					RuntimePushString( rItItem.ItemDef.ReadFieldId() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_WRITE_FIELD_ID:
					RuntimePushString( rItItem.ItemDef.WriteFieldId() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_READ_OBJECT_ID:
					RuntimePushString( rItItem.ItemDef.ReadObjectId() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_WRITE_OBJECT_ID:
					RuntimePushString( rItItem.ItemDef.WriteObjectId() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_SLICE_TOTALIZE:
					RuntimePushDouble( rItItem.ItemDef.SliceTotalize() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_NAME:

					pcString = rItItem.ItemDef.Name() ;
					poChannel = poAccess->GetpChannel() ;

					if( poChannel->Organization.Get( vOrganization ) == M4_SUCCESS && vOrganization.Type == M4CL_CPP_TYPE_STRING_VAR )
					{
						pcOrganization = vOrganization.Data.PointerChar ;
					}

					iResult = M4ReplaceLiterals( poChannel->GetpChannelManager(), pcString, pcOut, pcOrganization, bModified ) ;

					if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcOut != NULL )
					{
						RuntimePushString( pcOut ) ;
						delete pcOut;
					}
					else
					{
						RuntimePushString( pcString ) ;
					}
					break ;

				case M4_CH_VM_ITEMDEF_ORDER:
					RuntimePushDouble( rItItem.ItemDef.Order() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_SYNONYM:
					RuntimePushDouble( rItItem.ItemDef.Synonym() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_ID:
					RuntimePushString( rItItem.ItemDef.Id() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_IS_PK:
					RuntimePushDouble( rItItem.ItemDef.IsPK() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_NOT_NULL:
					RuntimePushDouble( rItItem.ItemDef.NotNull() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_AUXILIAR_ITEM:
					RuntimePushString( rItItem.ItemDef.AuxiliarItem() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_DMD:
					RuntimePushString( rItItem.ItemDef.Dmd() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_DMD_COMPONENT:
					RuntimePushString( rItItem.ItemDef.DmdComponent() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_DMD_CROSSTAB:
					RuntimePushString( rItItem.ItemDef.DmdCrossTab() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_DMD_FIELD:
					RuntimePushString( rItItem.ItemDef.DmdField() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_RULE_NUMBER:
					RuntimePushDouble( rItItem.ItemDef.MaxCodes() ) ;
					break ;

				case M4_CH_VM_ITEMDEF_RULE_ID:
					RuntimePushString( rItItem.ItemDef.CodeRuleId( ai_iRule ) ) ;
					break ;

				case M4_CH_VM_ITEMDEF_RULE_START_DATE:
					RuntimePushDate( rItItem.ItemDef.CodeStartDate( ai_iRule ) ) ;
					break ;

				case M4_CH_VM_ITEMDEF_RULE_END_DATE:
					RuntimePushDate( rItItem.ItemDef.CodeEndDate( ai_iRule ) ) ;
					break ;

				case M4_CH_VM_ITEMDEF_RULE_AUTOGENERATED:
					RuntimePushDouble( rItItem.ItemDef.CodeAutoGenerated( ai_iRule ) ) ;
					break ;

				case M4_CH_VM_ITEMDEF_RULE_CODE:

					oString.Init( 2000, 500 ) ;

					if( rItItem.ItemDef.CodeSource( ai_iRule, oString ) == M4_SUCCESS )
					{
						pccCode = oString.GetString() ;
					}

					if( pccCode == NULL )
					{
						pccCode = "" ;
					}

					RuntimePushString( pccCode ) ;
					break ;

				default:
					DUMP_CHLOG_ERRORF( M4_CH_VM_WRONG_PARAM_VALUE, "Data Type" << ai_pccMethodName << m4int32_t( ai_ItemDefData ) << DumpCallStack() ) ;
					RuntimePushDouble( M4_ERROR ) ;
					iResult = M4_ERROR ;
					break ;
			}
		}
		else
		{
			RuntimePushDouble( M4_ERROR );

			if( ai_bDumpError == M4_TRUE )
			{
				if( bScope == M4_TRUE )
				{
					DUMP_CHLOG_ERRORF( M4_CH_VM_ITEM_NOT_FOUND_BY_SCOPE, ai_pccMethodName << iScope << iPosition << DumpCallStack() ) ;
				}
				else
				{
					if( vNode.Type == M4CL_CPP_TYPE_NUMBER )
					{
						DUMP_CHLOG_ERRORF( M4_CH_VM_ITEM_NOT_FOUND, acItem << m4uint32_t( vNode.Data.DoubleData ) << M4Obj( poAccess->GetpCMCR() ) << ai_pccMethodName << DumpCallStack() ) ;
					}
					else if( vNode.Type == M4CL_CPP_TYPE_STRING_POOL )
					{
						DUMP_CHLOG_ERRORF( M4_CH_VM_ITEM_NOT_FOUND, acItem << m_oDynamicPool->GetString( vNode.Data.CharData ) << M4Obj( poAccess->GetpCMCR() ) << ai_pccMethodName << DumpCallStack() ) ;
					}
					else
					{
						DUMP_CHLOG_ERRORF( M4_CH_VM_ITEM_NOT_FOUND, acItem << "" << M4Obj( poAccess->GetpCMCR() ) << ai_pccMethodName << DumpCallStack() ) ;
					}
				}
			}
		}
	}

	DeleteVarString( vNode ) ;
	return( iResult ) ;
}

// Funcion LN4: GetListInfo()	-5, "RRrrr"
// Combinación de Parámetros:
// L2, IdLista, parentNode(salida), listNode(salida), nCols(salida)
// L2, idLista, colIndex(salida), parentField(salida), childField(salida)
m4return_t ClVM1Mcr::GetListInfo(void)
{

	m4return_t		iResult = M4_SUCCESS;
	m4VariantType	vValueBuffer;
	m4int32_t		iNumArgs;
	m4pcchar_t		pccValue = NULL;
	size_t			iLength = 0;
	m4char_t		acM4Obj[M4CL_MAX_LEN_CHANNEL + 1];
	m4char_t		acListId[M4CL_MAX_LEN_CHANNEL + 1];
	ClAccess		*poAccess = NULL;
	ClAccess		*poList = NULL;
	ClCompiledMCR   *poListCMCR = NULL;

	iResult = RuntimePopDouble(vValueBuffer);
	if (iResult != M4_SUCCESS) {
		DUMP_CHLOG_ERRORF(M4_CH_VM_RTS_POP, "GetListHandlerById");
		RuntimePushDouble(M4_ERROR);
		return(M4_ERROR);
	}

	iNumArgs = (m4uint32_t)vValueBuffer.Data.DoubleData;

	if (iNumArgs > 5)
	{
		// Si hay más de 5 argumentos doy un error
		DUMP_CHLOG_ERRORF(M4_CH_VM_TOO_PARAM, "GetListHandlerById" << 5 << iNumArgs << DumpCallStack());

		for (int i = 0; i < iNumArgs; i++)
		{
			iResult = RuntimePopAndDelete();
		}
		RuntimePushDouble(M4_ERROR);
		return(M4_ERROR);
	}

	// recuperamos todos los parámetros
	m4VariantType vParams[5];
	for (int i = iNumArgs - 1; i >= 0; --i)
	{
		// salta parámetros extra
		if (i >= 5) {
			continue;
		}

		// actualiza array de parámetros
		iResult = RuntimePop(vParams[i]);
		if (iResult != M4_SUCCESS) {
			DUMP_CHLOG_ERRORF(M4_CH_VM_RTS_POP, "GetListHandlerById");
			RuntimePushDouble(M4_ERROR);
			return(M4_ERROR);
		}
	}

	// Recupera el id de la instancia
	if (vParams[0].Type != M4CL_CPP_TYPE_STRING_POOL)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "M4Obj ID" << "GetListInfo" << M4ClCppType(M4CL_CPP_TYPE_STRING) << M4ClCppType(vParams[0].Type) << DumpCallStack());
		DeleteVarString(vParams[1]);
		DeleteVarString(vParams[0]);
		RuntimePushDouble(M4_ERROR);
		return(M4_ERROR);
	}
	pccValue = m_oDynamicPool->GetString(vParams[0].Data.CharData);
	iLength = strlen(pccValue);
	if (iLength > M4CL_MAX_LEN_CHANNEL)
	{
		iLength = M4CL_MAX_LEN_CHANNEL;
	}
	memcpy(acM4Obj, pccValue, iLength);
	acM4Obj[iLength] = '\0';
	DeleteVarString(vParams[0]);

	// Recupera el id de la lista
	if (vParams[1].Type != M4CL_CPP_TYPE_STRING_POOL)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_PARAMETER_TYPE_MISMATCH, "List ID" << "GetListInfo" << M4ClCppType(M4CL_CPP_TYPE_STRING) << M4ClCppType(vParams[1].Type) << DumpCallStack());
		DeleteVarString(vParams[1]);
		RuntimePushDouble(M4_ERROR);
		return(M4_ERROR);
	}
	pccValue = m_oDynamicPool->GetString(vParams[1].Data.CharData);
	iLength = strlen(pccValue);
	if (iLength > M4CL_MAX_LEN_CHANNEL)
	{
		iLength = M4CL_MAX_LEN_CHANNEL;
	}
	memcpy(acListId, pccValue, iLength);
	acListId[iLength] = '\0';
	DeleteVarString(vParams[1]);

	// Recuperamos el acceso a la instancia local
	poAccess = m_oState1.m_poAccess;

	// Recuperamos el accesso a la instancia remota
	iResult = m_poVM->GetL2Access(poAccess->GetpChannel(), acM4Obj, M4CL_HAS_STATE_EXE_JIT_FLAG(m_oState1.m_Flags), &poList);

	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_NO_GET_ACCESS_L2, acM4Obj << "GetListInfo" << DumpCallStack());
		RuntimePushDouble(M4_ERROR);
		return(M4_SUCCESS);
	}

	// Buscamos la lista concreta
	poListCMCR = poList->GetpCMCR();
	m4uint16_t nNodes = poListCMCR->GetChannelNumberOfNodes();

	// Navegamos por todos los nodos
	m4uint32_t iListHandlerFound = 0;
	m4uint32_t iNodeHandlerFound = 0;
	for (int i = 0; iListHandlerFound == 0 && i < nNodes ; i++)
	{
		m4uint32_t iNodeHandler = poListCMCR->GetChannelNodeHandleByPosition(i);
		m4uint16_t iNLists = poListCMCR->GetNodeNumberOfPlugs(iNodeHandler);

		// navegamos or todas las listas
		for (int j = 0; iListHandlerFound == 0 && j < iNLists; j++)
		{
			m4uint32_t iListHandler = poListCMCR->GetNodePlugHandleByPosition(iNodeHandler, j);
			m4pcchar_t pccListId = poListCMCR->GetPlugFatherChanneId(iListHandler);

			// comparamos el nombre
			if (strcmp(pccListId, acListId) == 0)
			{
				iListHandlerFound = iListHandler;
				iNodeHandlerFound = iNodeHandler;
			}
		}
	}

	// Comprobamos que hemos encontrado la lista
	if (iListHandlerFound == 0)
	{
		DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "List Id" << "GetListInfo" << acListId << DumpCallStack());
		RuntimePushDouble(M4_ERROR);
		return M4_ERROR;
	}

	// El tipo de ejecución depende de si el tercer parámetro es numérico o no
	m4bool_t bGetColumn = (vParams[2].Type == M4CL_CPP_TYPE_NUMBER);

	// Comprobamos si se nos piden la info de de la lista o de las columnas
	m4uint16_t iNColumns = poListCMCR->GetPlugNumberOfItems(iListHandlerFound);
	if (bGetColumn == M4_FALSE)
	{
		// nos han pedido info de la lista
		m4pcchar_t pcFatherNodeId = poListCMCR->GetPlugFatherNodeId(iListHandlerFound);
		m4pcchar_t pcNodeId = poListCMCR->GetNodeId(iNodeHandlerFound);

		// escribe los valores de salida
		RuntimePushDouble(M4_SUCCESS);
		RuntimePushDouble(iNColumns);
		RuntimePushString(pcNodeId);
		RuntimePushString(pcFatherNodeId);
		return M4_SUCCESS;
	}
	else {
		// nos han pedido info de las columnas
		m4int16_t iColumn = (m4int16_t) vParams[2].Data.DoubleData;
		if (iColumn < 0 || iColumn >= iNColumns)
		{
			DUMP_CHLOG_ERRORF(M4_CH_VM_WRONG_PARAM_VALUE, "Column index" << "GetListInfo" << iColumn << DumpCallStack());
			RuntimePushDouble(M4_ERROR);
			return M4_ERROR;
		}

		// recupera los items de la lista y su padre
		m4uint32_t iListItemHandle = poListCMCR->GetPlugItemHandleByPosition(iListHandlerFound, iColumn);
		m4pcchar_t parentItemId = poListCMCR->GetPlugItemFatherItemId(iListItemHandle);
		m4uint32_t iChildItem = poListCMCR->GetPlugItemSonItemHandle(iListItemHandle);
		m4pcchar_t childItemId = poListCMCR->GetItemId(iChildItem);

		// escribe los valores de salida
		RuntimePushDouble(M4_SUCCESS);
		RuntimePushString(childItemId);
		RuntimePushString(parentItemId);
		RuntimePushDouble(iColumn);
		return M4_SUCCESS;
	}
}

// Funcion LN4: GetNodeItemType()
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:  Tipo del Item
m4return_t ClVM1Mcr::GetNodeItemType (void)
{	
	return _GetItemData(M4_CH_VM_ITEMDEF_TYPE, 0, "GetNodeItemType", M4_TRUE);
}


// Funcion LN4: GetNodeItemScope()
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:  Scope del Item
m4return_t ClVM1Mcr::GetNodeItemScope (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_SCOPE, 0, "GetNodeItemScope", M4_TRUE);
}


// Funcion LN4: GetNodeItemSliceTotalize()
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:  Modo de totalización del Item
m4return_t ClVM1Mcr::GetNodeItemSliceTotalize (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_SLICE_TOTALIZE, 0, "GetNodeItemSliceTotalize", M4_TRUE);
}


// Funcion LN4: GetNodeItemM4Type()
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:  M4Type del Item
m4return_t ClVM1Mcr::GetNodeItemM4Type (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_M4TYPE, 0, "GetNodeItemM4Type", M4_TRUE);
}


// Funcion LN4: GetNodeItemPrecision()
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:  Precision del Item
m4return_t ClVM1Mcr::GetNodeItemPrecision (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_PRECISION, 0, "GetNodeItemPrecision", M4_TRUE);
}


// Funcion LN4: GetNodeItemIndex()	-2, "RR"
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:  Index del Item del Nodo
m4return_t ClVM1Mcr::GetNodeItemIndex (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_INDEX, 0, "GetNodeItemIndex", M4_TRUE);
}


// Funcion LN4: ExistNodeItemIndex()	-2, "RR"
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:  Index del Item del Nodo
// Hace lo mismo que la función GetNodeItemIndex pero sin dar error
m4return_t ClVM1Mcr::ExistNodeItemIndex (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_INDEX, 0, "ExistNodeItemIndex", M4_FALSE);
}


// Funcion LN4: GetNodeItemScale()	-2, "RR"
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:  Escala del Item del Nodo
m4return_t ClVM1Mcr::GetNodeItemScale (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_SCALE, 0, "GetNodeItemScale", M4_TRUE);
}


// Funcion LN4: GetNodeItemInternalType()	-2, "RR"
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:  Tipo interno del Item
m4return_t ClVM1Mcr::GetNodeItemInternalType (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_INTERNAL_TYPE, 0, "GetNodeItemInternalType", M4_TRUE);
}


// Funcion LN4: GetNodeReadObjectId()	1, "R"
//	- Entrada:	nodo   = Nombre del Nodo
//	- Salida:  Id. del Read Object del Nodo
m4return_t ClVM1Mcr::GetNodeReadObjectId (void)
{
	RuntimePushDouble( 1 ) ;
	return _GetNodeData(M4_CH_VM_NODEDEF_READ_OBJECT_ID, "GetNodeReadObjectId", M4_TRUE);
}


// Funcion LN4: GetNodeWriteObjectId()	1, "R"
//	- Entrada:	nodo   = Nombre del Nodo
//	- Salida:  Id. del Write Object del Nodo
m4return_t ClVM1Mcr::GetNodeWriteObjectId (void)
{
	RuntimePushDouble( 1 ) ;
	return _GetNodeData(M4_CH_VM_NODEDEF_WRITE_OBJECT_ID, "GetNodeWriteObjectId", M4_TRUE);
}


// Funcion LN4: GetNodeItemReadObjectAlias()	-2, "RR"
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:  Alias del Read Object del Item del Nodo
m4return_t ClVM1Mcr::GetNodeItemReadObjectAlias (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_READ_OBJECT_ALIAS, 0, "GetNodeItemReadObjectAlias", M4_TRUE);
}


// Funcion LN4: GetNodeItemWriteObjectAlias()	-2, "RR"
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:  Alias del Write Object del Item del Nodo
m4return_t ClVM1Mcr::GetNodeItemWriteObjectAlias (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_WRITE_OBJECT_ALIAS, 0, "GetNodeItemWriteObjectAlias", M4_TRUE);
}


// Funcion LN4: GetNodeItemReadFieldId()	-2, "RR"
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:	Id. del Field Read del Item del Nodo
m4return_t ClVM1Mcr::GetNodeItemReadFieldId (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_READ_FIELD_ID, 0, "GetNodeItemReadFieldId", M4_TRUE);
}


// Funcion LN4: GetNodeItemWriteFieldId()	-2, "RR"
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:	Id. del Field Write del Item del Nodo
m4return_t ClVM1Mcr::GetNodeItemWriteFieldId (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_WRITE_FIELD_ID, 0, "GetNodeItemWriteFieldId", M4_TRUE);
}


// Funcion LN4: GetNodeItemReadObjectId()	-2, "RR"
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:  Id. del Read Object del Item del Nodo
m4return_t ClVM1Mcr::GetNodeItemReadObjectId (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_READ_OBJECT_ID, 0, "GetNodeItemReadObjectId", M4_TRUE);
}


// Funcion LN4: GetNodeItemWriteObjectId()	-2, "RR"
//	- Entrada:	nodo   = Nombre del Nodo	nodo   = Nombre del Nodo
//				IdItem = ID. del Item		pos    = Posicion del item en el filtro
//											scope  = filtro
//	- Salida:  Id. del Read Object del Item del Nodo
m4return_t ClVM1Mcr::GetNodeItemWriteObjectId (void)
{
	return _GetItemData(M4_CH_VM_ITEMDEF_WRITE_OBJECT_ID, 0, "GetNodeItemWriteObjectId", M4_TRUE);
}


// Funcion LN4: GetThisItemId()	- ""
//	- Entrada: ninguna
//	- Salida:  Id del Item (char)
m4return_t ClVM1Mcr::GetThisItemId (void)
{
#if _M4_DEBUG > 10
    DUMP_CHLOG_DEBUGF (M4_CH_VM_TRACE_VM_L1, "Trace VM1:GetThisItemId  ");
#endif

	// Devolvemos el ID	del Item Actual
	RuntimePushString ( m_oState1.m_poAccess->GetpCMCR()->GetItemId(m_oState1.m_hItem) ) ;
	return M4_SUCCESS;
}


// Funcion LN4: GetChannelData()	-1, "R"
m4return_t ClVM1Mcr::GetChannelData(void)
{

	m4return_t		iResult = M4_SUCCESS ;
	m4double_t		dArguments = 0 ;
    m4VariantType	vValue ;


    iResult = RuntimePopDouble( vValue ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetChannelData" ) ;

	dArguments = vValue.Data.DoubleData - 1 ;
	
    iResult = RuntimePop( vValue ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetChannelData" ) ;
	
	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Data Type" << "GetChannelData" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	RuntimePushDouble( dArguments ) ;

	iResult = _GetChannelData( m4int8_t( vValue.Data.DoubleData ), "GetChannelData", M4_TRUE ) ;

	return( iResult ) ;
}


// Funcion LN4: GetNodeData()	-1, "RR"
m4return_t ClVM1Mcr::GetNodeData (void)
{

	m4return_t		iResult = M4_SUCCESS ;
	m4bool_t		bDumpError = M4_TRUE ;
	m4int32_t		iArguments = 0 ;
    m4VariantType	vValue ;


    iResult = RuntimePopDouble( vValue ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetNodeData" ) ;

	iArguments = m4int32_t( vValue.Data.DoubleData ) ;
	
	/* Bug 0179175
	Se permite un argumento que indique si se quiere o no volcar errores
	*/
	if( iArguments == 4 )
	{
		iResult = RuntimePop( vValue ) ;
		DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetNodeData" ) ;

		if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Dump Error" << "GetNodeData" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
			RuntimePushDouble( M4_ERROR ) ;
			return( M4_ERROR ) ;
		}

		bDumpError = m4bool_t( vValue.Data.DoubleData ) ;
		iArguments = 3 ;
	}

    iResult = RuntimePop( vValue ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetNodeData" ) ;
	
	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Data Type" << "GetNodeData" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	RuntimePushDouble( iArguments - 1 ) ;

	iResult = _GetNodeData( m4int8_t( vValue.Data.DoubleData ), "GetNodeData", bDumpError ) ;

	return( iResult ) ;
}



// Funcion LN4: GetItemData()	-3, "RRR"
m4return_t ClVM1Mcr::GetItemData (void)
{

	m4return_t		iResult = M4_SUCCESS ;
	m4double_t		dArguments = 0 ;
    m4VariantType	vValue ;


    iResult = RuntimePopDouble( vValue ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetNodeItemData" ) ;

	dArguments = vValue.Data.DoubleData - 1 ;
	
    iResult = RuntimePop( vValue ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetNodeItemData" ) ;
	
	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Data Type" << "GetNodeItemData" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	RuntimePushDouble( dArguments ) ;

	iResult = _GetItemData( m4int8_t( vValue.Data.DoubleData ), 0, "GetNodeItemData", M4_TRUE ) ;

	return( iResult ) ;
}



// Funcion LN4: GetRuleData()	-4, "RRRR"
m4return_t ClVM1Mcr::GetRuleData (void)
{

	m4return_t		iResult = M4_SUCCESS ;
	m4double_t		dArguments = 0 ;
	m4VariantType	vValue ;
	m4VariantType	vRule ;


	iResult = RuntimePopDouble( vValue ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetRuleData" ) ;

	dArguments = vValue.Data.DoubleData - 2 ;

	iResult = RuntimePop( vValue ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetRuleData" ) ;

	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Data Type" << "GetRuleData" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << DumpCallStack() ) ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	iResult = RuntimePop( vRule ) ;
	DEBUG_CHECK_RES( iResult == M4_SUCCESS, M4_CH_VM_RTS_POP, DEBUGINFOLOG, "GetRuleData" ) ;

	if( vRule.Type != M4CL_CPP_TYPE_NUMBER )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Rule position" << "GetRuleData" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vRule.Type ) << DumpCallStack() ) ;
		RuntimePushDouble( M4_ERROR ) ;
		return( M4_ERROR ) ;
	}

	RuntimePushDouble( dArguments ) ;

	iResult = _GetItemData( m4int8_t( vValue.Data.DoubleData ),  m4uint16_t( vRule.Data.DoubleData ), "GetRuleData", M4_TRUE ) ;

	return( iResult ) ;
}



// Localiza el Item del Nodo por Pos y Filtro (Scope+Type)
// Entrada:	ai_poAccess = Acceso por si es de nivel 2
//			ai_vNodo	= String=Nombre del Nodo (char) o Alias o Nombre TI ...  	ó
//						  Index (double)
//			ai_iPos		= Posicion
//			ai_iScope	= Filtro a aplicar
// Salida:	ao_rItItem	= Item localizado
//			return		= M4_TRUE o M4_FALSE (localizao o no)
m4bool_t ClVM1Mcr::_FindNodeItemByPosScope( ClAccess *ai_poAccess, const m4VariantType& ai_vNodo, m4uint32_t ai_iPos, m4uint8_t ai_iScope, ClNoCallableItem& ao_rItItem, m4pcchar_t ai_pccMethodName, m4bool_t ai_bDumpError )
{

	m4uint8_t	iScope = 0 ;
	m4uint8_t	iType = 0 ;
	ClNode		*poNode = NULL ;

	
	poNode = _FindNode( ai_poAccess, ai_vNodo, ai_pccMethodName, m_oState1, ai_bDumpError ) ;

	if( poNode == NULL )
	{
		return( M4_FALSE ) ;
	}
	
	//nos encargamos de conseguir los iteradores de los items
	iScope = ai_iScope & M4CL_ITEM_SCOPE_MASK_BITS ;
	iType  = ai_iScope & M4CL_ITEM_TYPE_MASK_BITS ;
	
	iType >>= 3;	// Formato que espera el DM. Vease VMdefine.hpp e ItemInfo.hpp
	
	// Establecemos el Type y Scope en el iterador de Items
	ao_rItItem  = (poNode->RecordSet).Register.Item ;
	ao_rItItem.SetMode( iScope, iType ) ;

	// Nos posicionamos
	return ao_rItItem.MoveTo( ClItemIndex( ai_iPos ) ) ;
}


// Localiza el Item del Nodo
// Entrada:	ai_poAccess = Acceso por si es de nivel 2
//			ai_vNodo	= String=Nombre del Nodo (char) o Alias o Nombre TI ...  	ó
//						  Double=Index
//			ai_pcItemID	= Identificador del Item
// Salida:	ao_rItItem	= Item localizado
//			return		= M4_TRUE o M4_FALSE (localizao o no)
m4bool_t ClVM1Mcr::_FindNodeItemById( ClAccess *ai_poAccess, const m4VariantType& ai_vNodo, m4pcchar_t ai_pcItemID, ClNoCallableItem& ao_rItItem , m4pcchar_t ai_pccMethodName, m4bool_t ai_bDumpError )
{    

	m4bool_t	bResult = M4_TRUE ;
	ClNode		*poNode = NULL ;


	poNode = _FindNode( ai_poAccess, ai_vNodo, ai_pccMethodName, m_oState1, ai_bDumpError ) ;

	if( poNode == NULL )
	{
		return( M4_FALSE ) ;
	}

	// Obtenemos el Iterador de Item de Registro, que tiene todos los items
	ao_rItItem = (poNode->RecordSet).Register.Item ;			
	// Establecemos el Type y Scope en el iterador de Items para que busque en todos
	ao_rItItem.SetMode( M4DM_SCOPE_ALL, M4DM_TYPE_ALL ) ;


	/* ExistNodeItemIndex
	Para quen no se de un warning por debajo si no existe el item se hace un
	TryMove primero
	*/
	if( ao_rItItem.TryMove( ai_pcItemID, M4_FALSE ) == M4DM_EOF_INDEX )
	{
		ao_rItItem.MoveToEOF() ;
		bResult = M4_FALSE ;
	}
	else
	{
		// Nos posicionamos
		bResult = ao_rItItem.MoveTo( ai_pcItemID ) ;
	}

	return( bResult ) ;
}


