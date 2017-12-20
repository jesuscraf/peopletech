//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           cldbgutil.cpp
// Project:        MVC
// Author:         Meta Software M.S. , S.A
// Date:             21 ene 99
// Language:        C++ 
// Operating System: WINDOWS, UNIX
// Design Document:  
//
// 
// Definition: 
//  
//    Utility tools box
//
//==============================================================================
#include "dmtypes.hpp"
#include "cldbgutil.hpp"
#include "cldmcontext.hpp"
#include "m4mdrt.hpp"
#include "node.hpp"
#include "access.hpp"
#include "channel.hpp"
#include "chlog.hpp"
#include "dm.hpp"
#include "iexecutor.hpp"
#include "chlog.hpp"
#include "m4objreg.hpp"		// M4ObjReg
#include "m4logsys.hpp"
#include "cldefine.hpp"
#include "m4objlogdef.hpp"	// M4ObjLog
#include "cllgadap.hpp"
#include "nodedef.hpp"
#include "m4unicode.hpp"
#include "cacheitfz.hpp"



string	_DumpTreePath		(ClAccess* ai_poAccess, m4uint32_t ai_hNode, m4bool_t ai_bRegister, m4bool_t ai_bItem);
string	_DumpItemInfo		(ClAccess* ai_poAccess, m4uint32_t ai_hNode, m4bool_t ai_bRegister);


	// Volcado basado en el Current del nodo actual
string		M4_DECL_M4DM	DumpTreePath					(ClAccess* ai_poAccess, m4uint32_t ai_hNode)
{
	return _DumpTreePath(ai_poAccess,ai_hNode,M4_FALSE,M4_FALSE);
}

	// Vuelca ademas el item donde esta posicionado (Utiliza el Current).
string		M4_DECL_M4DM	DumpTreePathItem				(ClAccess* ai_poAccess, m4uint32_t ai_hNode)
{
	return _DumpTreePath(ai_poAccess,ai_hNode,M4_FALSE,M4_TRUE);
}
	// Volcado basado en el Register del nodo actual
string		M4_DECL_M4DM	DumpTreePathRegister			(ClAccess* ai_poAccess, m4uint32_t ai_hNode)
{
	return _DumpTreePath(ai_poAccess,ai_hNode,M4_TRUE,M4_FALSE);
}
	// Vuelca el item donde esta posicionado (Utiliza el Register).
string		M4_DECL_M4DM	DumpTreePathRegisterItem		(ClAccess* ai_poAccess, m4uint32_t ai_hNode)
{
	return _DumpTreePath(ai_poAccess,ai_hNode,M4_TRUE,M4_TRUE);
}



//SetWS with String + LongString
m4bool_t	ClDbgUtil::SetWS (const m4char_t *ai_pcString, m4uint32_t ai_iLength)
{
	if (!ai_pcString)
		return M4_FALSE;
	m4bool_t bOk = M4_TRUE;
	if (ai_iLength>=m_iSizeWS)
		bOk=_ResizeWS(ai_iLength*2+1);
	if (bOk)
		_SetWS(ai_pcString, ai_iLength);
	return bOk;
}

//Append a String to end of WS
m4bool_t	ClDbgUtil::AppendWS	(const m4char_t *ai_pcString)	//String
{
	if (!ai_pcString)
		return M4_FALSE;
	m4uint32_t	iLength=strlen(ai_pcString);
	if ( IsUsedWS() ) {
		if (iLength >= m_iSizeWS-m_iLengthWS ) {	//No hay espacio
			m4char_t*	pcTemp = new m4char_t[m_iLengthWS+1];
			if (!pcTemp)
				return M4_FALSE;
			strcpy(pcTemp, m_pcWS);
			iLength += m_iLengthWS;
			_ResizeWS(iLength*2+1);
			strcpy(m_pcWS, pcTemp);
			strcat(m_pcWS, ai_pcString);
			delete [] pcTemp;
		}else{
			strcat(m_pcWS, ai_pcString);
		}
	}else{
		_ResizeWS(iLength*2+1);
		strcpy(m_pcWS, ai_pcString);
	}
	m_iLengthWS=strlen(m_pcWS);
	return M4_TRUE;
}

//Insert a String at the beginnig of WS
m4bool_t	ClDbgUtil::InsertWS	(const m4char_t *ai_pcString)	//String
{
	if (!ai_pcString)
		return M4_FALSE;
	m4uint32_t	iLength=strlen(ai_pcString);
	if ( iLength ) {
		iLength += m_iLengthWS;
		m4char_t*	pcTemp = new m4char_t[iLength+1];
		if (!pcTemp)
			return M4_FALSE;
		strcpy(pcTemp, ai_pcString);
		strcat(pcTemp, m_pcWS);
		_ResetWS();
		m_pcWS=pcTemp;
		m_iSizeWS=iLength+1;
		m_iLengthWS=iLength;
	}
	return M4_TRUE;
}
// Volcar Informacion de un Nodo.
// Entrada: ai_poAccess		= Access 
//			ai_poAccess		= Handle del nodo a volcar
//			ai_poAccess		= Indica si lo cogemos del Registero del Current.
string	_DumpItemInfo		(ClAccess* ai_poAccess, m4uint32_t ai_hNode, m4bool_t ai_bRegister)
{
	string stTemp("");		// #ItemId&&ItemName
	if (ai_poAccess==0){
		return stTemp;
	}

	ClCompiledMCR *pCMCR = ai_poAccess->GetpCMCR();

	ClNode *pNode = ai_poAccess->Node.Get((ClHandle)ai_hNode);
	if (pNode ==0){
		return stTemp;
	}
	
	ClItem *pItem = 0;
	if (ai_bRegister){
		pItem = &pNode->RecordSet.Register.Item;
	}
	else{
		pItem = &pNode->RecordSet.Current.Item;
	}

	if (pItem->GetIndex() != M4DM_EOF_INDEX) {
		m4int32_t hItem = pItem->ItemDef.GetHandle();
		stTemp.append("#");
		stTemp.append(pCMCR->iItemI(hItem));
		stTemp.append("&&");
		stTemp.append(pCMCR->nItemI(hItem));
	}
	
	return stTemp;
}

// Volcar Informacion de un Nodo.
// Entrada: ai_stTempId		= Cadena para montar Id
//			ai_stTempName	= Cadena para montar Name
//			ai_pReg			= Iterador de registro del nodo a volcar
// Salida:	return			Nothing

//Dump Info: //	"NodoId(RegId/CurrentIndex/Count)" && "NodoName(RegId/CurrentIndex/Count)"
void
_DumpNodeInfo(string &ai_stTempId,string &ai_stTempName,_ClRegister_Base *ai_pReg)
{

	ClCompiledMCR	*poCMCR=0;		// Puntero al MCR
	m4uint32_t		hNode = 0;		// Handle del Nodo
	m4uint32_t		uUniqueId=0;	// Identificador unico del registro.
	m4char_t		pcIndex[15];	// Indice de registro como char
	m4char_t		pcRegId[15];	// Indice de registro como char
	m4char_t		pcCount[15];	// Total de Registros como char

	poCMCR	= ai_pReg->GetpNodeDef()->GetpCMCR();	// MCR
	hNode = ai_pReg->GetpNodeDef()->GetHandle();	// MCR
	//Bloque: Indice del registro actual y Total de Registros del RecordSet
	sprintf( pcCount, "%d", ai_pReg->Count() );
	if (ai_pReg->Index == M4DM_EOF_INDEX) {
		strcpy( pcIndex, "EOF" );	//Es EOF
		strcpy( pcRegId, "EOF" );	//Es EOF
	}
	else{
		if (ai_pReg->GetRecordID(uUniqueId) == M4_SUCCESS) {
			sprintf( pcIndex, "%d", ai_pReg->Index);	// We have the Index.
			sprintf( pcRegId, "%d", uUniqueId);	// We have the ID.
		}
		else{
			strcpy( pcIndex, "EOF" );	// We don't have the Index.
			strcpy( pcRegId, "EOF" );	//Es EOF
		}
	}			

	//Montamos la cadena con Id
	ai_stTempId.assign(poCMCR->iNodeN(hNode));
	ai_stTempId.append("(");
	ai_stTempId.append(pcRegId);
	ai_stTempId.append("/");
	ai_stTempId.append(pcIndex);
	ai_stTempId.append("/");
	ai_stTempId.append(pcCount);
	ai_stTempId.append(")\\");

	//Montamos la cadena con Name
	ai_stTempName.assign(poCMCR->nNodeN(hNode));
	ai_stTempName.append("(");
	ai_stTempName.append(pcRegId);
	ai_stTempName.append("/");
	ai_stTempName.append(pcIndex);
	ai_stTempName.append("/");
	ai_stTempName.append(pcCount);
	ai_stTempName.append(")\\");
}


//#####################################################
//###			Funciones Globales					###
//#####################################################

// Volcar Arbol de Nodos y registros a partir de un Nodo.
// Recorre el arbol de padres y genera el Path.
// Entrada: ai_poAccess	= Access del Canal
//			ai_hNode	= Handle del Nodo en el que estamos posicionados.
// Salida:	return		= string con el Path.
//M4_DECL_M4DM
string GetTreePath(ClAccess* ai_poAccess, m4uint32_t ai_hNode, m4bool_t ai_bRegister)
{
	ClCompiledMCR	*poCMCR;		// Puntero al MCR
	ClNode			*poNode;		// Puntero al Nodo
	m4int32_t		iNumConFather;	// No. de conectores padre
	m4int8_t		iTipoConector;	// Tipo del Conector: RB, BB, Free, Auto
	m4uint32_t		hConectorFather;// Handle del Conector al nodo padre
	m4uint32_t		hNode;			// Handle del Nodo
	m4uint32_t		hNodeFather;	// Handle del Nodo Padre (no-free)
	m4uint8_t		iOldAutoLoad;	// Tipo de AutoLoad anterior
	m4uint16_t		i;
	m4bool_t		bFirst= M4_TRUE;// Indica si es el primer recordset que miramos. ( Para elegir entre Currrent o Register ).


	string			stBufferId("");		// String con Path de Ids.
	string			stBufferName("");	// String con Path de Names.
	string			stTempId("");		// String temporal para Id.
	string			stTempName("");		// String temporal para Name.

	//Obtener el MetaCanal
	if (!ai_poAccess || !ai_hNode) {
		return string("");
	}
	poCMCR	= ai_poAccess->GetpCMCR();	// MCR
	if (!poCMCR) {
		return string("");
	}

	//Inicializamos Proceso
	hNode = ai_hNode;	//Nodo inicial

	//Desactivar autoload
	iOldAutoLoad = ai_poAccess->GetAutoLoadMode();
	if (ai_poAccess->GetpChannel())
		ai_poAccess->GetpChannel()->SetAutoLoadMode(M4CL_AUTOLOAD_OFF);	//Desactivamos a nivel de canal
	else {
		return string("");
	}

	while (1) {
		//Get Info:
		poNode=ai_poAccess->Node.Get(ClHandle(hNode));	//Get Node pointer
		if (!poNode) {
			return string("");
		}
		//Si es un bloque nihil -> No se trata
		if (poNode->RecordSet.IsValid()) {				//RecordSet valido
			// Si hay que mirar el Register en lugar del Current lo miramos solo para el primer bloque.
			if (ai_bRegister&&bFirst){
				_DumpNodeInfo(stTempId,stTempName,&poNode->RecordSet.Register);
				bFirst=M4_FALSE;
			}
			else{
				_DumpNodeInfo(stTempId,stTempName,&poNode->RecordSet.Current);
			}
			//Insertamos por delante
			stBufferName.insert(0,stTempName);
			//Insertamos por delante
			stBufferId.insert(0,stTempId);
		}

		//### GetNodeFatherNoFree(poCMCR, hNode) ###
		//Obtener el no. de Conectores Padre
		iNumConFather = poCMCR->GetNodeNumberOfInvConnectors( hNode );
		hNodeFather = 0;
		//Buscamos el Conector Padre No-Free
		for (i=0; i<iNumConFather; ++i) {
			hConectorFather = poCMCR->GetNodeInvConnectorHandleByPosition( hNode, i );
			iTipoConector = poCMCR->GetNodeConnectorType( hConectorFather );
			if (iTipoConector == M4CL_NODE_CONNECTOR_TYPE_RB || iTipoConector == M4CL_NODE_CONNECTOR_TYPE_BB) {
				hNodeFather = poCMCR->GetNodeConnectorFatherNodeHandle( hConectorFather );
				break;
			}
		}

		if ( hNodeFather == 0 )
			break;

		hNode = hNodeFather;
	} //end-while

	//Restauramos la AutoLoad
	ai_poAccess->GetpChannel()->SetAutoLoadMode(iOldAutoLoad);	//Restauramos la autoload a nivel de canal

	
	// Cogemos cualquiera de la temporales (stTempId o stTempName).
	stTempId.assign(stBufferId);
	stTempId.append("&&");
	stTempId.append(stBufferName);
	return stTempId;
}

// Volcar Arbol de Nodos y registros a partir de un Nodo, formateado como Error Extendido No. 101 para la M4Log.
// Recorre el arbol de padres y genera el Path.
// Entrada: ai_poAccess	= Access del Canal
//			ai_hNode	= Handle del Nodo en el que estamos posicionados.
// Salida:	return		= string formateado: "||#*E101*#IdChannel#hAccessClient#hAccessServer#PathIds&&PathNames"
//M4_DECL_M4DM
string		_DumpTreePath		(ClAccess* ai_poAccess, m4uint32_t ai_hNode, m4bool_t ai_bRegister, m4bool_t ai_bItem)
{
	string stPath;
	string stTemp;

	//Validacion de entrada
	if (!ai_poAccess || !ai_hNode) {
		return string("");
	}
	if (!ai_poAccess->GetpCMCR() || !ai_poAccess->GetpChannel() || 
		!ai_poAccess->GetpChannel()->GetpChannelDef() || !ai_poAccess->GetpChannel()->GetpChannelDef()->Id() ||
		!ai_poAccess->GetEnvironment() ) {
		return string("");
	}
	
	//Obtenemos el Path
	stPath = GetTreePath(ai_poAccess, ai_hNode, ai_bRegister);
	
	//Formateamos al estilo que espera la Log:
	stTemp.assign(M4LOGSYS_EXT_ERROR_TREE_PATH);	//Cadena = "||#*E101*#IdChannel#hAccessClient#hAccessServer#PathIds&&PathNames"

	//Montamos la Salida final
	stTemp.append(ai_poAccess->GetpChannel()->GetpChannelDef()->Id());
	stTemp.append("#");
	
	m4char_t pcHandleAccess[17];	//Handle de Access, serializado, en decimal unsigned: %u
	if ( ai_poAccess->GetpChannel()->GetpChannelManager()->GetDMContext()->GetCSType() == M4CL_CSTYPE_FRONT ) {
		//Si estoy en client => Conozco solo 1 access: el del Client
		sprintf(pcHandleAccess, "%u", ai_poAccess);	//Access del Client
		stTemp.append(pcHandleAccess);
		stTemp.append("#");
		stTemp.append("0");							//Access del Server: No lo conocemos
		stTemp.append("#");
	} else {
		//Si estoy en server => Conozco los  2 access: el del Client y el del Server
		sprintf(pcHandleAccess, "%u", 0);			//Access del Cliente: 0 hasta que podamos pedirlo al Access actual
		stTemp.append(pcHandleAccess);
		stTemp.append("#");
		sprintf(pcHandleAccess, "%u", ai_poAccess);	//Access del Server
		stTemp.append(pcHandleAccess);
		stTemp.append("#");
	}
	
	stTemp.append(stPath);

	if (ai_bItem){
		stTemp.append(_DumpItemInfo(ai_poAccess, ai_hNode, ai_bRegister));
	}
		
	return stTemp;
}


// Funcion:	Buscar y sino Construir el Canal Indicado
// Entrada:	ai_pChannelManager	= Gestor de Canales
//			ai_pcChannelId		= Nombre de la Instancia del Canal a buscar
// Salida:	ao_pChannel			= Puntero al Canal
//			return = M4_ERROR (no encontrado, ni creado) | M4_SUCCESS (encontrado o creado)
m4return_t FindOrCreateChannel(ClChannelManager *ai_pChannelManager, m4pcchar_t ai_pcChannelId, ClChannel* &ao_pChannel)
{
	m4return_t ret=M4_SUCCESS;
	ClChannelDirectory *	pChanDir;
	ClChannel *				pChanAux;
	m4pcchar_t pcAuxChanId = 0;
	m4uint32_t iNumChan, i;
	m4bool_t	bEncontrado=M4_FALSE;

	if (!ai_pChannelManager)
		return M4_ERROR;

	//Buscamos el canal en el ChannelManager
	pChanDir = &( ai_pChannelManager->Channel );
	iNumChan = pChanDir->Count ();
	for (i=0; (i<iNumChan) && (!bEncontrado); i++) {
		pChanAux = (*pChanDir)[i];

		if (pChanAux->IsBuilt())
			pcAuxChanId = pChanAux->GetpChannelDef ()->Id ();
		else
			pcAuxChanId = 0;

		if (( pcAuxChanId ) && ( stricmp ( ai_pcChannelId, pcAuxChanId ) == 0 )) {
			// Encontrado (¿Que pasa si ese canal se lo cargan por ahí??? )
			ao_pChannel = pChanAux;
			bEncontrado = M4_TRUE;
		}
	}
	//Si no lo encontramos, creamos y levantamos el canal a CAPON!!!
	if (!bEncontrado) {
		//Creamos el Canal
		ret=ai_pChannelManager->CreateChannel(ao_pChannel);
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_DEBUGF(M4_CH_DM_BUILD_FROM_ID, ai_pcChannelId);
			ao_pChannel=NULL;
			return M4_ERROR;
		}
		//Levantamos el Canal
		ret=ao_pChannel->BuildFromId(ai_pcChannelId);
		if (ret!=M4_SUCCESS){
			DUMP_CHLOG_DEBUGF(M4_CH_DM_BUILD_FROM_ID, ai_pcChannelId);
			ao_pChannel->Destroy();
			ao_pChannel=NULL;
			return M4_ERROR;
		}
		//Deshabilitamos la AutoLoad a nivel de Canal
		ao_pChannel->WantsAutoLoad.Set (M4CL_AUTOLOAD_OFF);
	}
	return ret;
}




// Funcion:	Busca un error en el canal de errores, y formatea el error con los parametros pasados
// Entrada:	ai_uiCodErr			= Codigo de Error
//			ai_vaParam			= Vector de variants (Parametros del error) (no del tipo STRING_POOL)
//			ai_iNumArgs			= No. de elementos del vector
//			ai_pChannelManager	= Puntero al Gestor de Canales
//			ai_uiLang			= Lenguaje
// Salida:	ao_pcBuffer	= Puntero al buffer de salida (si es Null, devolvemos tamaño necesario en aio_uiBufferSize)
//			aio_uiBufferSize	= Tamaño del Buffer, o Tamaño necesario
//			return				= M4_ERROR | M4_SUCCESS |
//										M4_WARNING (si encuentra el error pero no hay suficiente espacio en el buffer que se le pasa)
//M4_DECL_M4DM
m4return_t ChannelErrorCodeByParams(m4uint32_t ai_uiCodErr, m4VariantType *ai_vaParam, m4int32_t ai_iNumArgs, m4uchar_t ai_byIdParamPos, ClChannelManager *ai_pChannelManager, m4char_t *ao_pcBuffer, m4uint32_t &aio_uiBufferSize, m4language_t ai_uiLang)
{
	m4return_t	ret=M4_SUCCESS;

	m4uint32_t uOutputBuffer;
	ClChannel	*	poChannel;		//Canal de Errores
	ClAccess	*	poAccess;		//Access del Canal de Errores
	ClNode		*	poNode;			//Nodo de Errores
	ClItem		*	poItem;			//Item de Errores
	m4VariantType	vErrorString;	//Plantilla de la Cadena de Error
	
	//#################################################
	//######## Buscamos en el Canal de Errores ########
	//#################################################
	
	//--llamamos al item <GetError>--

	ret=FindOrCreateChannel(ai_pChannelManager, M4_INSTANCE_M4_ERRORS, poChannel);
	if( ret != M4_SUCCESS ){
		DUMP_CHLOG_ERRORF(M4_CH_DM_NO_GET_ACCESS_L2, M4_INSTANCE_M4_ERRORS<<"ChannelErrorCodeByParams" );
		return M4_ERROR;
	}

	/* Bug 0231780
	Se pone en el lenguaje actual sólo en server
	*/
	if( ai_uiLang != M4CL_LANGUAGE_NONE && ai_pChannelManager->GetDMContext()->ExecutingInClient() == M4_FALSE )
	{
		poChannel->Data_Language.Set( m4double_t( ai_uiLang ) ) ;
	}

	//Creamos un Access
	poAccess = poChannel->CreateAccess();
	if (!poAccess) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_NO_GET_ACCESS_L2, M4_INSTANCE_M4_ERRORS<<"ChannelErrorCodeParams" );
		return M4_ERROR;
	}

	//Get Node
	poNode=poAccess->Node.Get(M4_NODE_SCHERRORS_NODE_CODES);
	if (!poNode || poAccess->Node.Index == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_NO_SUCH_NODE, M4_NODE_SCHERRORS_NODE_CODES <<M4_T3_M4_ERRORS<< "ChannelErrorCodeByParams" );
		poAccess->Destroy();
		return M4_ERROR;
	}   
	
	//Get Item
	poItem=&(poNode->RecordSet.Current.Item[M4_NODE_SCHERRORS_ITEM_GET_ERROR]);
	if (poItem->GetIndex() == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_ITEM_NOT_FOUND, M4_NODE_SCHERRORS_ITEM_GET_ERROR<<M4_NODE_SCHERRORS_NODE_CODES<<M4_T3_M4_ERRORS<<"ChannelErrorCodeByParams" );
		poAccess->Destroy();
		return M4_ERROR;
	}

	//comprobamos que el item tenga un solo argumento
	if (poItem->ItemDef.NumberOfArguments() != 1 || poItem->ItemDef.VariableArguments()==1){
		DUMP_CHLOG_ERRORF(M4_CH_DM_WRONG_ITEM_DEFINITION, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(), poItem->ItemDef.GetHandle())<<1<<poItem->ItemDef.NumberOfArguments() );
		poAccess->Destroy();
		return M4_ERROR;
	}

	//Obtenemos la Pila de Ejecucion y el Pool de Strings del Item
	IStackInterface *	pRuntimeStack	= &(poItem->GetpAccess()->GetpExecutor()->Stack);
	if (!pRuntimeStack) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_INTERNAL_ERROR, __FILE__ << __LINE__ << "ChannelErrorCodeByParams" );
		poAccess->Destroy();
		return M4_ERROR;
	}

	//Pasamos el Argumento (el codigo de error)
	m4VariantType vCodError(ai_uiCodErr);
	pRuntimeStack->Push(vCodError);
	
	//Ejecutamos el Item
	ret=poItem->Call();

	//comprobamos si la ejecucion fue bien
	if (ret!= M4_SUCCESS){
		DUMP_CHLOG_DEBUGF(M4_CH_DM_CANT_EXECUTE_ITEM, M4_NODE_SCHERRORS_ITEM_GET_ERROR<<M4_NODE_SCHERRORS_NODE_CODES<<M4_INSTANCE_M4_ERRORS );
		ret=M4_ERROR;
	}
	else{ //todo ha ido bien-> formateamos la cadena
		//sacamos la cadena	(es un VAR_STRING)
		ret=pRuntimeStack->Pop(vErrorString);
		
		M4_DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_DM_RTS_POP, DEBUGINFOLOG, "ChannelErrorCodeByParams.")	;

		//if (vErrorString.Type != M4CL_CPP_TYPE_STRING_POOL){
		if (vErrorString.Type != M4CL_CPP_TYPE_STRING_VAR || 
			(vErrorString.Type == M4CL_CPP_TYPE_STRING_VAR && vErrorString.Data.PointerChar == NULL) ){
			//DUMP_CHLOG_WARNINGF(M4_CH_VM_RETURN_TYPE_MISMATCH, M4_NODE_SCHERRORS_ITEM_GET_ERROR<<M4_NODE_SCHERRORS_NODE_CODES<<M4_INSTANCE_M4_ERRORS<<M4ClCppType(M4CL_CPP_TYPE_STRING_VAR)<<M4ClCppType(vErrorString.Type ) );
			aio_uiBufferSize=1;
			if (ao_pcBuffer)
				*ao_pcBuffer='\0';	//Devolvemos "" si es un valor Numerico o si es valor Null
		}
		else{
			//parseamos y sustituimos
			ret=FormatErrorStringByParams( vErrorString.Data.PointerChar, ai_vaParam, ai_iNumArgs, ai_byIdParamPos, ao_pcBuffer, aio_uiBufferSize, &uOutputBuffer, ai_uiLang);
			if (ret!=M4_ERROR) {
				aio_uiBufferSize=uOutputBuffer;
			} else {
				DUMP_CHLOG_ERROR(M4_CH_DM_FORMATTING_ERROR_STRING);
			}
		}
	}
	
	//Destruimos el Access utilizado
	poAccess->Destroy();

	return ret;
}



// Funcion:	Busca un error en el canal de errores, y formatea el error con los parametros pasados
// Entrada:	ai_uiCodErr			= Codigo de Error
//			ai_pszParams		= Cadena de parámetros
//			ai_pChannelManager	= Puntero al Gestor de Canales
//			ai_uiLang			= Lenguaje
// Salida:	ao_pcBuffer	= Puntero al buffer de salida (si es Null, devolvemos tamaño necesario en aio_uiBufferSize)
//			aio_uiBufferSize	= Tamaño del Buffer, o Tamaño necesario
//			return				= M4_ERROR | M4_SUCCESS |
//										M4_WARNING (si encuentra el error pero no hay suficiente espacio en el buffer que se le pasa)
//M4_DECL_M4DM
m4return_t	ChannelErrorCode(m4uint32_t ai_uiCodErr, m4pchar_t ai_pszParams, m4uchar_t ai_byIdParamPos, ClChannelManager *ai_pChannelManager, m4char_t *ao_pcBuffer, m4uint32_t &aio_uiBufferSize, m4language_t ai_uiLang)
{
	m4return_t	ret=M4_SUCCESS;

	m4uint32_t uOutputBuffer;
	ClChannel	*	poChannel;		//Canal de Errores
	ClAccess	*	poAccess;		//Access del Canal de Errores
	ClNode		*	poNode;			//Nodo de Errores
	ClItem		*	poItem;			//Item de Errores
	m4VariantType	vErrorString;	//Plantilla de la Cadena de Error
	
	//#################################################
	//######## Buscamos en el Canal de Errores ########
	//#################################################
	
	//--llamamos al item <GetError>--

	ret=FindOrCreateChannel(ai_pChannelManager, M4_INSTANCE_M4_ERRORS, poChannel);
	if( ret != M4_SUCCESS ){
		DUMP_CHLOG_ERRORF(M4_CH_DM_NO_GET_ACCESS_L2, M4_INSTANCE_M4_ERRORS<<"ChannelErrorCode" );
		return M4_ERROR;
	}

	/* Bug 0231780
	Se pone en el lenguaje actual sólo en server
	*/
	if( ai_uiLang != M4CL_LANGUAGE_NONE && ai_pChannelManager->GetDMContext()->ExecutingInClient() == M4_FALSE )
	{
		poChannel->Data_Language.Set( m4double_t( ai_uiLang ) ) ;
	}

	//Creamos un Access
	poAccess = poChannel->CreateAccess();
	if (!poAccess) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_NO_GET_ACCESS_L2, M4_INSTANCE_M4_ERRORS<<"ChannelErrorCodeParams" );
		return M4_ERROR;
	}

	//Get Node
	poNode=poAccess->Node.Get(M4_NODE_SCHERRORS_NODE_CODES);
	if (!poNode || poAccess->Node.Index == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_NO_SUCH_NODE, M4_NODE_SCHERRORS_NODE_CODES <<M4_T3_M4_ERRORS<< "ChannelErrorCode" );
		poAccess->Destroy();
		return M4_ERROR;
	}   
	
	//Get Item
	poItem=&(poNode->RecordSet.Current.Item[M4_NODE_SCHERRORS_ITEM_GET_ERROR]);
	if (poItem->GetIndex() == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_ITEM_NOT_FOUND, M4_NODE_SCHERRORS_ITEM_GET_ERROR<<M4_NODE_SCHERRORS_NODE_CODES<<M4_T3_M4_ERRORS<<"ChannelErrorCode" );
		poAccess->Destroy();
		return M4_ERROR;
	}

	//comprobamos que el item tenga un solo argumento
	if (poItem->ItemDef.NumberOfArguments() != 1 || poItem->ItemDef.VariableArguments()==1){
		DUMP_CHLOG_ERRORF(M4_CH_DM_WRONG_ITEM_DEFINITION, M4ObjNodeItemI(poItem->GetpAccess()->GetpCMCR(), poItem->ItemDef.GetHandle())<<1<<poItem->ItemDef.NumberOfArguments() );
		poAccess->Destroy();
		return M4_ERROR;
	}

	//Obtenemos la Pila de Ejecucion y el Pool de Strings del Item
	IStackInterface *	pRuntimeStack	= &(poItem->GetpAccess()->GetpExecutor()->Stack);
	if (!pRuntimeStack) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_INTERNAL_ERROR, __FILE__ << __LINE__ << "ChannelErrorCode" );
		poAccess->Destroy();
		return M4_ERROR;
	}

	//Pasamos el Argumento (el codigo de error)
	m4VariantType vCodError(ai_uiCodErr);
	pRuntimeStack->Push(vCodError);
	
	//Ejecutamos el Item
	ret=poItem->Call();

	//comprobamos si la ejecucion fue bien
	if (ret!= M4_SUCCESS){
		DUMP_CHLOG_DEBUGF(M4_CH_DM_CANT_EXECUTE_ITEM, M4_NODE_SCHERRORS_ITEM_GET_ERROR<<M4_NODE_SCHERRORS_NODE_CODES<<M4_INSTANCE_M4_ERRORS );
		ret=M4_ERROR;
	}
	else{ //todo ha ido bien-> formateamos la cadena
		//sacamos la cadena	(es un VAR_STRING)
		ret=pRuntimeStack->Pop(vErrorString);
		
		M4_DEBUG_CHECK_RES (ret==M4_SUCCESS, M4_CH_DM_RTS_POP, DEBUGINFOLOG, "ChannelErrorCode.")	;
		
		//if (vErrorString.Type != M4CL_CPP_TYPE_STRING_POOL){
		if (vErrorString.Type != M4CL_CPP_TYPE_STRING_VAR || 
			(vErrorString.Type == M4CL_CPP_TYPE_STRING_VAR && vErrorString.Data.PointerChar == NULL) ){
			//DUMP_CHLOG_WARNINGF(M4_CH_VM_RETURN_TYPE_MISMATCH, M4_NODE_SCHERRORS_ITEM_GET_ERROR<<M4_NODE_SCHERRORS_NODE_CODES<<M4_INSTANCE_M4_ERRORS<<M4ClCppType(M4CL_CPP_TYPE_STRING_VAR)<<M4ClCppType(vErrorString.Type ) );
			aio_uiBufferSize=1;
			if (ao_pcBuffer)
				*ao_pcBuffer='\0';	//Devolvemos "" si es un valor Numerico o si es valor Null
		}
		else{
			//parseamos y sustituimos
			ret=FormatErrorString( vErrorString.Data.PointerChar, ai_pszParams, ai_byIdParamPos, ao_pcBuffer, aio_uiBufferSize, &uOutputBuffer);
			if (ret!=M4_ERROR) {
				aio_uiBufferSize=uOutputBuffer;
			} else {
				DUMP_CHLOG_ERROR(M4_CH_DM_FORMATTING_ERROR_STRING);
			}
		}
	}
	
	//Destruimos el Access utilizado
	poAccess->Destroy();

	return ret;
}



// Funcion:	Busca un error en el canal de errores, y formatea el error con los parametros pasados
// Entrada:	ai_uiCodErr			= Codigo de Error
//			ai_vaParam			= Vector de variants (Parametros del error) (no del tipo STRING_POOL)
//			ai_iNumArgs			= No. de elementos del vector
//			ai_pChannelManager	= Puntero al Gestor de Canales
//			ai_uiLang			= Lenguaje
// Salida:	ao_pcBuffer	= Puntero al buffer de salida (si es Null, devolvemos tamaño necesario en aio_uiBufferSize)
//			aio_uiBufferSize	= Tamaño del Buffer, o Tamaño necesario
//			return				= M4_ERROR | M4_SUCCESS | 
//										M4_WARNING (si encuentra el error pero no hay suficiente espacio en el buffer que se le pasa)
//M4_DECL_M4DM
m4return_t M4GetErrorStringByParams(m4uint32_t ai_uiCodErr, m4VariantType *ai_vaParam, m4int32_t ai_iNumArgs, m4uchar_t ai_byIdParamPos, ClChannelManager *ai_pChannelManager, m4char_t *ao_pcBuffer, m4uint32_t &aio_uiBufferSize, m4language_t ai_uiLang)
{
	m4return_t ret;
	m4uint32_t uOutputBuffer;

	if (!ai_vaParam && ai_iNumArgs>0) {
		return M4_ERROR;
	}

	//Buscamos en la M4LogSys
	ret = FormatErrorCodeByParams(ai_uiCodErr, ai_vaParam, ai_iNumArgs, ai_byIdParamPos, ao_pcBuffer, aio_uiBufferSize, &uOutputBuffer, ai_uiLang);

	if (ret!=M4_ERROR) {
		aio_uiBufferSize=uOutputBuffer;
	} else {
		//Buscamos en el Canal de Errores, si tenemos ChannelManager
		if (ai_pChannelManager) 
			ret=ChannelErrorCodeByParams(ai_uiCodErr, ai_vaParam, ai_iNumArgs, ai_byIdParamPos, ai_pChannelManager, ao_pcBuffer, aio_uiBufferSize, ai_uiLang);
	}

	return ret;
}


// Funcion:	Busca un error en el canal de errores, y formatea el error con los parametros pasados
// Entrada:	ai_uiCodErr			= Codigo de Error
//			ai_pszParams		= Cadena con los parámetros
//			ai_pChannelManager	= Puntero al Gestor de Canales
//			ai_uiLang			= Lenguaje
// Salida:	ao_pcBuffer	= Puntero al buffer de salida (si es Null, devolvemos tamaño necesario en aio_uiBufferSize)
//			aio_uiBufferSize	= Tamaño del Buffer, o Tamaño necesario
//			return				= M4_ERROR | M4_SUCCESS | 
//										M4_WARNING (si encuentra el error pero no hay suficiente espacio en el buffer que se le pasa)
//M4_DECL_M4DM
m4return_t M4GetErrorString	(m4uint32_t ai_uiCodErr, m4pchar_t ai_pszParams, m4uchar_t ai_byIdParamPos, ClChannelManager *ai_pChannelManager, m4char_t *ao_pcBuffer, m4uint32_t &aio_uiBufferSize, m4language_t ai_uiLang)
{
	m4return_t ret;
	m4uint32_t uOutputBuffer;

	
	//Buscamos en la M4LogSys
	ret = FormatErrorCode(ai_uiCodErr, ai_pszParams, ai_byIdParamPos, ao_pcBuffer, aio_uiBufferSize, &uOutputBuffer, ai_uiLang);

	if (ret!=M4_ERROR) {
		aio_uiBufferSize=uOutputBuffer;
	} else {
		//Buscamos en el Canal de Errores, si tenemos ChannelManager
		if (ai_pChannelManager) 
			ret=ChannelErrorCode(ai_uiCodErr, ai_pszParams, ai_byIdParamPos, ai_pChannelManager, ao_pcBuffer, aio_uiBufferSize, ai_uiLang);
	}

	return ret;
}



//Funcion:	Obtener el Nombre de un fichero para Debug con la ruta completa,
//			utilizando el Environment
//Entrada:	ai_pszFileName	= Nombre del Fichero
//			ai_poEnv		= Environment, para poder obtener el Path. Si es NULL
//								se toma el directorio actual de ejecucion
//Salida:	return = string con el "Path+FileName"
string M4VMGetDebugFileName(m4pcchar_t ai_pszFileName, ClVMBaseEnv *ai_poEnv/*=0*/)
{
	string debugFile("");
	
	if (ai_poEnv) {
		m4pcchar_t pszDebugDir = ai_poEnv->GetDebugDirectory();
		if (pszDebugDir && pszDebugDir[0]!='\0') {
			debugFile.append(pszDebugDir);					//Directorio de debug
		}else {
			debugFile.append(".");							//Directorio actual
		}
	}else{
		debugFile.append(".");								//Directorio actual
	}
	
	debugFile.append (M4_BACK_SLASH);
	
	if (ai_pszFileName && ai_pszFileName[0]!='\0') {
		debugFile.append (ai_pszFileName);
	}
	//else{
	//	debugFile.append ("");	//Sin nombre
	//}
	return debugFile;
}


// Funcion:	Busca una conjuntos de errores en una sola transacción C/S de tal manera que se cachean en el cliente.
// Entrada:	ai_puErrorCodes	= Codigos de Error
//			ai_uNumErrors			= Número de errores
//M4_DECL_M4DM
void M4ErrorLookup(ClChannelManager *ai_pChannelManager, m4puint32_t ai_puErrorCodes, m4uint32_t ai_uNumErrors)
{
	m4return_t		iResult = M4_SUCCESS;
	m4uint32_t		i = 0;
	m4uint32_t		j = 0;
	m4uint32_t		iTotal = 0;
	m4bool_t		bRepeated = M4_FALSE;
	m4language_t	uiLang = LOGSYS_INVALID_M4UINT32;
	m4VariantType	vCodError;
	m4uint32_t		*piErrorCodes = NULL;
	ClChannel		*poChannel = NULL;		//Canal de Errores
	ClAccess		*poAccess = NULL;		//Access del Canal de Errores
	ClNode			*poNode = NULL;			//Nodo de Errores
	ClItem			*poItem= NULL;			//Item de Errores


	if (ai_pChannelManager==NULL)
	{
		return ;
	}

	if (ai_puErrorCodes==NULL || ai_uNumErrors==0) // Comprueba argumentos consistentes
	{
		return;
	}

	/*
	Calculamos primero la lista de mensajes a procesar
	Si no hay ninguno no hace falta seguir
	*/
	piErrorCodes = new m4uint32_t[ ai_uNumErrors ] ;

	if( piErrorCodes == NULL )
	{
		return ;
	}

	//Obtenemos el Lenguaje
	uiLang = ai_pChannelManager->GetLogonAdaptor()->GetSessionLanguage( "" );

	//Pasamos los Argumentos (los codigos de error)
	iTotal=0;
	for (i=0; i<ai_uNumErrors; i++) {
		// Sólo se mete los errores que no encuentre en el logmsg.ini
		if (LogsysGetCodeString(ai_puErrorCodes[i],NULL,0,NULL,uiLang)==M4_ERROR) {
			bRepeated=M4_FALSE;
			for (j=0; j<i; j++)
			{
				if (ai_puErrorCodes[i]==ai_puErrorCodes[j]) {
					bRepeated=M4_TRUE;
					break;
				}
			}

			if (bRepeated==M4_FALSE) {
				piErrorCodes[iTotal] = ai_puErrorCodes[i];
				iTotal++;
			}
		}
	}

	if( iTotal == 0 )
	{
		delete [] piErrorCodes ;
		return ;
	}


	//#################################################
	//######## Buscamos en el Canal de Errores ########
	//#################################################
	
	//--llamamos al item <GetErrors>--

	iResult=FindOrCreateChannel(ai_pChannelManager, M4_INSTANCE_M4_ERRORS, poChannel);
	if( iResult != M4_SUCCESS ){
		DUMP_CHLOG_ERRORF(M4_CH_DM_NO_GET_ACCESS_L2, M4_INSTANCE_M4_ERRORS<<"M4ErrorLookup" );
		delete [] piErrorCodes ;
		return;
	}

	//Creamos un Access
	poAccess = poChannel->CreateAccess();
	if (!poAccess) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_NO_GET_ACCESS_L2, M4_INSTANCE_M4_ERRORS<<"M4ErrorLookup" );
		delete [] piErrorCodes ;
		return;
	}

	//Obtenemos la Pila de Ejecucion y el Pool de Strings del Item
	IStackInterface *	pRuntimeStack = &( poAccess->GetpExecutor()->Stack );
	if (!pRuntimeStack) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_INTERNAL_ERROR, __FILE__ << __LINE__ << "M4ErrorLookup" );
		delete [] piErrorCodes ;
		poAccess->Destroy();
		return;
	}

	//Pasamos los Argumentos (los codigos de error)
	for (i=0; i<iTotal; i++) {
		vCodError=m4VariantType (m4int32_t(piErrorCodes[i]));
		pRuntimeStack->Push(vCodError);
	}

	delete [] piErrorCodes ;

	//Get Node
	poNode=poAccess->Node.Get(M4_NODE_SCHERRORS_NODE_CODES);
	if (!poNode || poAccess->Node.Index == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_NO_SUCH_NODE, M4_NODE_SCHERRORS_NODE_CODES <<M4_T3_M4_ERRORS<< "M4ErrorLookup" );
		poAccess->Destroy();
		return;
	}   
	
	//Get Item
	poItem=&(poNode->RecordSet.Current.Item[M4_NODE_SCHERRORS_ITEM_GET_ERRORS]);
	if (poItem->GetIndex() == M4DM_EOF_INDEX) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_ITEM_NOT_FOUND, M4_NODE_SCHERRORS_ITEM_GET_ERRORS<<M4_NODE_SCHERRORS_NODE_CODES<<M4_T3_M4_ERRORS<<"M4ErrorLookup" );
		poAccess->Destroy();
		return;
	}

	//comprobamos que el item tenga argumentos variables
	if (poItem->ItemDef.VariableArguments()!=1){
		DUMP_CHLOG_ERRORF(M4_CH_DM_WRONG_PARAM_NUM_INS_1, M4_NODE_SCHERRORS_ITEM_GET_ERRORS<<M4_NODE_SCHERRORS_NODE_CODES<<M4_T3_M4_ERRORS<<"M4ErrorLookup" );
		poAccess->Destroy();
		return;
	}

	//Ejecutamos el Item
	iResult=poItem->Call(0,iTotal);

	//comprobamos si la ejecucion fue bien
	if (iResult == M4_SUCCESS)
	{
		//todo ha ido bien-> formateamos la cadena
		//sacamos el valor de retorno y no lo usamos
		iResult=pRuntimeStack->Pop(vCodError);
		if (iResult!=M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF(M4_CH_DM_RTS_POP, "M4ErrorLookup.")	;
		}
	}
	else
	{ 
		DUMP_CHLOG_ERRORF(M4_CH_DM_CANT_EXECUTE_ITEM, M4_NODE_SCHERRORS_ITEM_GET_ERRORS<<M4_NODE_SCHERRORS_NODE_CODES<<M4_INSTANCE_M4_ERRORS );
	}
	
	//Destruimos el Access utilizado
	poAccess->Destroy();
}


