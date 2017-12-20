//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler 
// File:           vvm.hpp
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
//   Clase base de todas las que implementan funciones de vm. Implementación
////
//==============================================================================
#include "vvm.hpp"
#include "m4mdrt.hpp"
#include "access.hpp"
#include "nodedef.hpp"
#include "m4objglb.hpp"
#include "m4unicode.hpp"


// Vuelca la Pila de Llamadas con "Identificadores&&Nombres" en el Buffer miembro del Objeto.
// Entrada: Ninguna
// Salida:  return = string formateado, con la pila de llamadas
string ClVVM::DumpCallStack(const m4VMState_t *poState, m4bool_t ai_bIdsOnly)
{
	// Recorrer la Pila de estados
	// Volcar la informacion: Canal!Nodo.Item..IDRegla
	m4int32_t		iPos;				// Posicion Absoluta en la Pila
	m4VMState_t		oState;				// Estado actual
	ClCompiledMCR	*poCMCR;			// Puntero al MCR
	m4uint32_t		hItem;				// Handle del Item

	m4char_t		pcTemp[128];		// Temporal buffer
	string			stBufferId("");		// String con Ids.
	string			stBufferName("");	// String con Names.
	string			stTemp("");			// String temporal
	
	if (!m_oStateStack)
		return string("");

	//Si nos marcan un estado inicial (que no esta en la pila), lo metemos temporalmente, 
	//y despues lo sacamos de la pila. Este es el caso de la ejecuciones de nivel 0.
	if (poState)
		m_oStateStack->Push(*poState);

	//Inicializamos Proceso
	iPos = m_oStateStack->GetPos();
	
#if _M4_DEBUG > 10
    ofstream fFile;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
	M4Open( fFile, "C:\\temp\\CallStack.txt", M4UniWrite, eEncoding );	//Abrimos para escritura

	fFile << "***********************" << endl;
	fFile << "****** CallStack ******" << endl;
	fFile << "***********************" << endl <<endl;
	fFile << "*** Pos Inicio: " << iPos << endl;
#endif

	//Recorremos la Pila de Llamadas por estado
	while (1) {

		//Obtenemos estado con item valido (no propagado)
		for (m4int32_t iPosRelat = m_oStateStack->GetPos() - iPos; ;++iPosRelat ) {
			if ( m_oStateStack->Top(oState,iPosRelat) != M4_SUCCESS ) {
				if (poState) m_oStateStack->Pop();	//Sacamos el "push temporal"
				return string("");
			}
			if (oState.m_hItem != (unsigned)-1)	{	// Buscar un item valido
				iPos = m_oStateStack->GetPos() - iPosRelat;
				break;
			}
		}

#if _M4_DEBUG > 10
		fFile << "* Pos: " << iPos;
#endif
		//Obtenemos Info del estado
		if (!oState.m_poAccess) {
			if (poState) m_oStateStack->Pop();	//Sacamos el "push temporal"
			return string("");
		}
		poCMCR	= oState.m_poAccess->GetpCMCR();	// MCR
		hItem	= oState.m_hItem;					// Handle del Item
		if (!poCMCR) {
			if (poState) m_oStateStack->Pop();	//Sacamos el "push temporal"
			return string("");
		}

		//Si el estado es 'hide'... no contamos esta llamada en la pila.
		if ( !M4CL_HAS_STATE_HIDE_FLAG(oState.m_Flags) ) {
			//Montamos la cadena con Id:	"%s!%s.%s..%s[JIT](%d)\r\n"
			stTemp.assign(poCMCR->iM4Obj());
			stTemp.append("!");
			stTemp.append(poCMCR->iNodeI(hItem));
			stTemp.append(".");
			stTemp.append(poCMCR->iItemI(hItem));
			stTemp.append("..");
			if (oState.m_iRule < 0) {
				stTemp.append("METARULE");
			}
			else {
				stTemp.append(poCMCR->GetItemFormIdByPosition(hItem, oState.m_iRule));
			}

			if ( ai_bIdsOnly == M4_TRUE ) {
				stTemp.append("[");
				ClDateToString(oState.m_poFormula.GetStartDate(), pcTemp, M4_SECOND_END + 1);
				stTemp.append(pcTemp);
				stTemp.append("]");
			}

			//Si es JIT añadimos comentario
			if ( M4CL_HAS_STATE_EXE_JIT_FLAG(oState.m_Flags) ) {
				stTemp.append("[JIT]");
			}
			stTemp.append("(");
			sprintf(pcTemp, "%d", oState.m_iSourceLine-1);
			stTemp.append(pcTemp);
			stTemp.append(")");
			//Insertamos al final
			stBufferId.append(stTemp);

#if _M4_DEBUG > 10
			fFile << " Path: " << stTemp.c_str() << endl;
#endif

			if ( ai_bIdsOnly == M4_FALSE ) {
				//Montamos la cadena con Names:	"%s!%s.%s..%s(%d)\r\n"
				stTemp.assign(poCMCR->nM4Obj());
				stTemp.append("!");
				stTemp.append(poCMCR->nNodeI(hItem));
				stTemp.append(".");
				stTemp.append(poCMCR->nItemI(hItem));
				stTemp.append("..");
				if (oState.m_iRule < 0) {
					stTemp.append("METARULE");
				}
				else {
					stTemp.append(poCMCR->GetItemFormIdByPosition(hItem, oState.m_iRule));
				}
				//Si es JIT añadimos comentario
				if ( M4CL_HAS_STATE_EXE_JIT_FLAG(oState.m_Flags) ) {
					stTemp.append("[JIT]");
				}
				stTemp.append("(");
				sprintf(pcTemp, "%d", oState.m_iSourceLine-1);
				stTemp.append(pcTemp);
				stTemp.append(")");
				//Insertamos al final
				stBufferName.append(stTemp);
			}

		} //endif - M4CL_HAS_STATE_HIDE_FLAG

		// Get Parent Position
		iPos = oState.m_iParentStatePos;
		if (iPos == -1)
			break;	//End of process

		if ( !M4CL_HAS_STATE_HIDE_FLAG(oState.m_Flags) ) {
			stBufferId.append("\r\n");

			if ( ai_bIdsOnly == M4_FALSE ) {
				stBufferName.append("\r\n");
			}
		}
	} //end-while

#if _M4_DEBUG > 10
	fFile.close();
#endif
	
	//Montamos la Salida final
 	stTemp.assign(stBufferId);

	if ( ai_bIdsOnly == M4_FALSE ) {
		stTemp.append("&&");
		stTemp.append(stBufferName);

		//Formateamos al estilo que espera la Log:
		stTemp.insert(0, M4LOGSYS_EXT_ERROR_CALL_STACK);	//Cadena = "||#*E102*#CallStackIds&&CallStackNames"
	}

	if (poState) m_oStateStack->Pop();	//Sacamos el "push temporal"
	return stTemp;
}


// Localiza un canal por indirección
ClAccess*	ClVVM::_FindChannel( const m4VariantType& ai_vM4obj, m4pcchar_t ai_pccMethodName, const m4VMState_t &roState )
{

	ClAccess		*poResult = NULL ;
	m4return_t		iResult = M4_SUCCESS ;
	m4uint32_t		hMyNode = 0 ;
	size_t			iLength = 0 ;
	m4pcchar_t		pccValue = NULL ;
	m4pcchar_t		pccThisChannel = NULL ;
	m4pcchar_t		pccHostInstance = NULL ;
	m4char_t		acM4Obj[ M4CL_MAX_LEN_CHANNEL + 1 ] ;
    ClCompiledMCR	*poThisCMCR = NULL ;


	if( ai_vM4obj.Type != M4CL_CPP_TYPE_STRING_POOL )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "M4Obj ID" << ai_pccMethodName << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( ai_vM4obj.Type ) << DumpCallStack() ) ;
		return( NULL ) ;
	}

	pccValue = m_oDynamicPool->GetString( ai_vM4obj.Data.CharData ) ;
	iLength = strlen( pccValue ) ;
	if( iLength > M4CL_MAX_LEN_CHANNEL )
	{
		iLength = M4CL_MAX_LEN_CHANNEL ;
	}
	memcpy( acM4Obj, pccValue, iLength ) ;
	acM4Obj[ iLength ] = '\0' ;

	poResult = roState.m_poAccess ;
	poThisCMCR = poResult->GetpCMCR() ;
	pccThisChannel = poThisCMCR->GetChannelId() ;

	if( *acM4Obj != '\0' && strcmpi( pccThisChannel, acM4Obj ) != 0 )
	{
		hMyNode = poThisCMCR->GetItemNodeHandle( roState.m_hItem ) ;

		pccHostInstance = poThisCMCR->GetNodeT3AliasInstanceIdByAlias( hMyNode, acM4Obj ) ;
		
		if( pccHostInstance == NULL )
		{
	    	DUMP_CHLOG_ERRORF( M4_CH_VM_NO_SUCH_INSTANCE, acM4Obj << poThisCMCR->iNodeN( hMyNode ) << LogCatAnd << poThisCMCR->nNodeN( hMyNode ) << ai_pccMethodName << DumpCallStack() ) ;
			return( NULL ) ;
		}

		iResult = poResult->GetpChannel()->Level2.GetL2Access( pccHostInstance, &poResult, M4CL_HAS_STATE_EXE_JIT_FLAG( roState.m_Flags ) ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_NO_GET_ACCESS_L2, pccHostInstance << ai_pccMethodName << DumpCallStack() ) ;
			return( NULL ) ;
		}
	}

	return( poResult ) ;
}


// Localiza un nodo por posición o por id
ClNode*		ClVVM::_FindNode( ClAccess *ai_poAccess, const m4VariantType& ai_vNodo, m4pcchar_t ai_pccMethodName, const m4VMState_t &roState, m4bool_t ai_bDumpError )
{

	ClNode			*poResult = NULL ;
	m4uint32_t		hNode = 0 ;
	m4uint32_t		hMyNode = 0 ;
	m4pcchar_t		pccNode = NULL ;
	ClCompiledMCR	*poCMCR = NULL ;


	if( ai_poAccess == NULL )
	{
		// Si no nos pasan un acceso tomamos el del propio canal
		ai_poAccess = roState.m_poAccess ;
	}

	if( ai_vNodo.Type == M4CL_CPP_TYPE_NUMBER )
	{
		// Es el Index
		poResult = &( ai_poAccess->Node[ ClNodeIndex( m4uint32_t( ai_vNodo.Data.DoubleData ) ) ] ) ;

		if( poResult == NULL )
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_WRONG_NODE_INDEX, m4uint32_t( ai_vNodo.Data.DoubleData ) << ai_pccMethodName << DumpCallStack() ) ;
			return( NULL ) ;
		}
	}
	else
	{
		if( ai_vNodo.Type == M4CL_CPP_TYPE_STRING_POOL )
		{
			pccNode = m_oDynamicPool->GetString( ai_vNodo.Data.CharData ) ;
		}
		else if( ai_vNodo.Type == M4CL_CPP_TYPE_NULL && ai_poAccess == roState.m_poAccess )
		{
			pccNode = roState.m_poRecordSet->GetpNodeDef()->Id() ;
		}
		else
		{
			DUMP_CHLOG_ERRORF( M4_CH_VM_PARAMETER_TYPE_MISMATCH, "Node" << ai_pccMethodName << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( ai_vNodo.Type ) << DumpCallStack() ) ;
			return( NULL ) ;
		}

		// Es el Nombre, Alias o TI
		poCMCR = ai_poAccess->GetpCMCR() ;

		if( ai_poAccess == roState.m_poAccess )
		{
			// Si es del propio canal se obtiene el nodo por alias
			hMyNode = poCMCR->GetItemNodeHandle( roState.m_hItem ) ;

			if( hMyNode == 0)
			{
				DUMP_CHLOG_ERRORF( M4_CH_VM_WRONG_THIS_NODE_HANDLE, ai_pccMethodName << DumpCallStack() ) ;
				return( NULL ) ;
			}

			hNode = poCMCR->GetNodeAliasNodeHandleByAlias( hMyNode, pccNode, roState.m_poFormula.GetLevel() ) ;
		}
		else
		{
			// Si es de otro canal se obtiene por nodo directamente
			hNode = poCMCR->GetChannelNodeHandleById( pccNode ) ;
		}

		if( hNode == 0 )
		{
			if( ai_bDumpError == M4_TRUE )
			{
				DUMP_CHLOG_ERRORF( M4_CH_VM_WRONG_NODE_NAME, pccNode << ai_pccMethodName  << DumpCallStack() );
			}
			return( NULL ) ;
		}

		poResult = &( ai_poAccess->Node[ ClHandle( hNode ) ] ) ;
	}

	return( poResult ) ;
}


ChLog&	DumpLinef( ChLog& aio_chlog, const m4VMState_t* ai_poState )
{

	ClCompiledMCR	*poCMCR = NULL ;


	if( aio_chlog.IsDumpActive() == M4_FALSE )
	{
		return( aio_chlog ) ;
	}

	if( ai_poState != NULL && ai_poState->m_poAccess != NULL )
	{
		poCMCR = ai_poState->m_poAccess->GetpCMCR() ;

		if( poCMCR != NULL )
		{
			aio_chlog << ai_poState->m_iSourceLine - 1 ;
			aio_chlog << LOG_CAT_AND( poCMCR->iItemI( ai_poState->m_hItem ), poCMCR->nItemI( ai_poState->m_hItem ) ) ;
			aio_chlog << LOG_CAT_AND( poCMCR->iNodeI( ai_poState->m_hItem ), poCMCR->nNodeI( ai_poState->m_hItem ) ) ;
			aio_chlog << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) ;
		}
	}

	return( aio_chlog ) ;
}


ChLogManip_line_dump_t	DumpLine( const m4VMState_t* ai_poState )
{
	return( ChLogManip_line_dump_t( DumpLinef, ai_poState )  );
}


