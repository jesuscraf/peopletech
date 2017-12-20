//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              serial 
// File:                exeproxy.cpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                03-08-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module implements the proxy executor on server
//
//
//==============================================================================

// VM
#include "m4types.hpp"
#include "m4define.hpp"
#include "level2.hpp"
#include "dm.hpp"
#include "item.hpp"
#include "cldatstoiod.hpp"
#include "dump_vst.hpp"
#include "m4thread.hpp"
#include "m4trace.hpp"
#include "m4mdrt.hpp"

// for statistics
class ClRequestExeStats;

// proxy's
#include "exeproxy.hpp"
#include "m4srvres.hpp"
#include "exechannel.hpp"
#include "genproxy.hpp"
#include "exvariables.hpp"

// for execution
#include "iexecutor.hpp"
#include "executor.hpp"
#include "objdir.hpp"
#include "cs_result.hpp"
#include "execntxt.hpp"
#include "chan_knl.hpp"

#include "clconfiguration.hpp"
#include "chaninfo.hpp"
#include "exertinfo.hpp"

#include "channel.hpp"
#include "access.hpp"

#include "clm4objservice.hpp"
#include "conn_provider.hpp"
#include "lconn_interface.hpp"
#include "csres.hpp"
#include "cldmcontext.hpp"

#include "licensemanagerinterface.hpp"

typedef set< ClChannel*, less< ClChannel* > >	SetChannelRefs_t;
typedef SetChannelRefs_t::iterator				ItSetChannelRefs_t;


static void ProxyWriteError(ClDataStoreIODriver *ao_poOutputIODriver, m4char_t ai_iSerialCode, m4pcchar_t ai_pcMessage)
{
	ClCMObjectDirectory oDirectory(NULL);
	ClClientServerResult* pCSResult = oDirectory.CreateClientServerResult(M4_TRUE);
	pCSResult->SetResponseCode(ai_iSerialCode);
	pCSResult->AddMess("-1");
	
	if (oDirectory.Serialize(*ao_poOutputIODriver) != M4_SUCCESS)
		return;
}

static m4return_t WriteChannelInfo( ClChannel* ai_poClChannel, ClDataStoreIODriver* ao_poOutputIODriver, SetChannelRefs_t& ai_setChannelRefs )
{
	pair< ItSetChannelRefs_t, bool >	p;
	m4return_t							iResult = M4_SUCCESS;
	m4int32_t							iNL2Serialize, i = 0;
	ItSetChannelRefs_t					itChRefs;
	SetChannelRefs_t					setChsInSubtree;
	

	// bg 267481
	// Avoid circular references.
	p = ai_setChannelRefs.insert( ai_poClChannel );
	if( p.second == false )
	{
		return M4_SUCCESS;
	}

	// just ignores dynamic channels
	if (ai_poClChannel->GetpBackMCR() != NULL)
		return M4_SUCCESS;
	
	
	// Not preprocessing subtree, so do write to the driver.
	if( ao_poOutputIODriver != NULL )
	{
		// writes the Id
		m4char_t pcChannelId[PROXY_MAX_VIEW_ID];
		strcpy(pcChannelId,ai_poClChannel->GetpChannel_Data()->GetpChannelDef()->Id());
		iResult = ao_poOutputIODriver->Write(pcChannelId);
		if (iResult != M4_SUCCESS)
			return iResult;

		// writes the unique id
		m4uint32_t iChannelHandle = ai_poClChannel->GetHandle();
		iResult = ao_poOutputIODriver->Write(iChannelHandle);
		if (iResult != M4_SUCCESS)
			return iResult;
	}

	
	m4int32_t iNL2 = ai_poClChannel->Level2.m_oInstanceMap.Count();


	// Not preprocessing subtree, so do write to the driver.
	if( ao_poOutputIODriver != NULL )
	{
		// Finds out the final number of L2 that are going to be serialized, because some 
		// of them could be ignored later while processing the tree. 
		iNL2Serialize = iNL2;

		for( i = (iNL2 - 1); i >= 0 ; i-- ) 
		{
			itChRefs = ai_setChannelRefs.find( ai_poClChannel->Level2.m_oInstanceMap.GetAccess( i )->GetpChannel() );

			// L2 channel was already visited.
			if( itChRefs != ai_setChannelRefs.end() )
			{
				--iNL2Serialize;
			}
			else
			{
				// Searchs the L2 in the foregoing L2 subtrees.
				for( m4int32_t j = 0; j < i; j++ )
				{
					// Resets subtree container (inserting channels already visited above this level).
					setChsInSubtree = ai_setChannelRefs;

					// Populates the subtree container.
					iResult = WriteChannelInfo( ai_poClChannel->Level2.m_oInstanceMap.GetAccess( j )->GetpChannel(), NULL, setChsInSubtree );
					if( iResult != M4_SUCCESS )
					{
						return iResult;
					}

					itChRefs = setChsInSubtree.find( ai_poClChannel->Level2.m_oInstanceMap.GetAccess( i )->GetpChannel() );

					// This L2 exists in foregoing L2 subtrees.
					if( itChRefs != setChsInSubtree.end() )
					{
						--iNL2Serialize;
						break;
					}
				}
			}
		}

		// writes the number of Level 2
		iResult = ao_poOutputIODriver->Write( iNL2Serialize );
		if (iResult != M4_SUCCESS)
			return iResult;
	}

	// writes the information for each child
	for ( i = 0; i < iNL2; i++ ) {
		iResult = WriteChannelInfo( ai_poClChannel->Level2.m_oInstanceMap.GetAccess(i)->GetpChannel(),ao_poOutputIODriver, ai_setChannelRefs );
		if (iResult != M4_SUCCESS)
			break;
	}

	// returns
	return iResult;
}

/**
 * Como la ejecución ha terminado por un breakpoint, esta función se encarga de preparar
 * los objetos que tienen que retornar al cliente.
 *
 * @param ai_objDir		Object directory que utilizamos para retornar los objetos al cliente.
 * @param ai_pExecutor	Ejecutor que nos dice que objetos son los que tienen que volver al cliente.
 * @return m4return_t.
 */
static m4return_t PrepareExecutionInterruptionReturn(ClCMObjectDirectory& ai_objDir, ClExecutor* const ai_pExecutor)
{
	m4return_t iResponse = M4_CSR_ERROR_EXECUTE;
	m4return_t iExecuteResult = -1;
	ClChannel* pChannel = 0;
	ClAccess* pAccess = 0;
	m4int16_t j = -1;
	m4uint16_t i = 0;
	m4return_t iRet = M4_ERROR;

	
	// Miramos cuantos access/channel vamos a tener que llevarnos.
	m4uint32_t iNumAccess = ai_pExecutor->GetNumAccessToSerialize();
	ClAccess   **ppoAccess = new ClAccess* [iNumAccess];


	ClOLTPExecutionContext*  pOLTPExeCntxt = ai_objDir.CheckFirstDebugTransaction();
	if (!pOLTPExeCntxt) goto RETURN_ERROR;

	//Indicamos que no se serialize el puntero al Access del ejecutor, porque no se va a utilizar en
	// el cliente, y porque si no lo hiciesemos podría saltar el control que obliga a que el puntero al
	// Access exista en el Access del ObjectDirectory. Esta último sucede cuando se marca para borrar el
	// Access en el espacio de depuración cliente.
	pOLTPExeCntxt->SetbSendAccessPointer(M4_FALSE);


	// Pillamos los access que nos vamos a tener que llevar
	iRet = ai_pExecutor->GetAccessToSerialize(ppoAccess, iNumAccess);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

	for (; i < iNumAccess; i++)
	{
		pAccess = ppoAccess[i];
		pChannel = pAccess->GetpChannel();

		// Cambiamos el modo de depuración para que se serialize el canal entero (todos los registros e ítems) al viajar al cliente.
		// Además, como el canal ya existe en el servidor, le digo que me devuelva deltas.
		// Si hay algún canal que no está marcado como de depuración también hay que enviarlo entero al cliente. Estos
		// canales se han creado en el servidor y se han accedido por nivel2 (no venían desde el cliente).
		const ClChannel::eDebugMode_t eDebugMode = pChannel->GetDebugMode();

		if (eDebugMode == ClChannel::IS_DEBUG_FIRST_TIME || eDebugMode == ClChannel::IS_NOT_DEBUG)
		{
			pChannel->SetDebugMode(ClChannel::IS_DEBUG_SECOND_TIME);
			pChannel->GetpChannel_Data()->SetKeepStatusOnRemote(M4_TRUE);

			/* Bug 0150731
			Se manda el metacanal antes del canal de depuración
			*/
			ClCompiledMCR *poCMCR = pChannel->GetpCMCR() ;

			if( poCMCR != NULL )
			{
				if( ai_objDir.IncludeMetaChannel( poCMCR, pChannel ) == -1 )
				{
					goto RETURN_ERROR;
				}
			}
		}

		j = ai_objDir.IncludeChannel(pChannel, pChannel->GetHandle());
		if (j == -1) goto RETURN_ERROR;

		j = ai_objDir.IncludeAccessAndCreateOID(pAccess, pAccess->GetHandle());
		if (j == -1) goto RETURN_ERROR;
	}

	// También hay que meter el ejecutor (ya dijimos antes que lo meteríamos al final del todo).
	j = ai_objDir.IncludeOLTPExecutionContextAndCreateOID(*pOLTPExeCntxt);
	if (j == -1) {
		delete pOLTPExeCntxt;
		goto RETURN_ERROR;
	}
	ai_objDir.SetKeepAlive(j, M4_FALSE);

	iResponse = M4_CSR_EXEC_INTERRUPT;
	iExecuteResult = M4CL_EXECUTION_INTERRUPTION;

RETURN_ERROR:
	// Añadimos un objetito en el objDir para indicar al cliente que se terminó la ejecución por un breakpoint.
	ClClientServerResult* pCSResult = ai_objDir.CreateClientServerResult(M4_TRUE);
	pCSResult->SetResponseCode(iResponse);
	m4char_t sResult[50];
	sprintf(sResult, "%d", iExecuteResult);
	pCSResult->AddMess(sResult);

	delete [] ppoAccess;

	return iRet;
}

static m4return_t ExecProxy(ClChannelManager* const    ai_poChannelManager,
							ClDataStoreIODriver* const ai_poInputIODriver,
							ClDataStoreIODriver* const ao_poOutputIODriver,
							ClProxySpace* const        ai_poProxySpace,
							ClStatus* const            ai_poClStatus,
							ExecDumpInfo* const        ai_poDumpInfo,
							ClConfiguration* const     ai_poConfiguration,
							IObjDataCache*             ai_pIObjDataCache)
{
	if (ai_poDumpInfo != NULL) {
		ai_poDumpInfo->SetCurrentStage("ExecProxy: Executing the remote command");
	}
	
	DUMP_CHLOG_DEBUGF ( M4_INF_PROXY_GENERIC_0 , "Proxy command starting");

#ifdef _DEBUG
	cerr << "PROXY EXECUTOR" << endl;
#else
	m4TraceLevel(2, cerr << "PROXY+" << endl);
#endif
	
	m4return_t				iExecResult = M4_ERROR;
	m4uint32_t				i = 0;
	m4uint32_t				iAccesses = 0;
	IExecutor*				poExecutor = 0;
	ClClientServerResult*	pCSResult = 0;
	ClOLTPExecutionContext*	poOLTPExecutionContext = 0;
	ClChannel*				poChannel = 0;
	ClAccess**				ppoAccess = NULL ;
	m4int16_t				OLTPExecutionContextIndex = -1;

	// updates the request id (for execution only).
	// this value keeps on so the proxy space is deleted if the end user cancels the 
	//    request
	if (ai_poProxySpace != NULL && ai_poClStatus != NULL)
		ai_poProxySpace->SetRequestId(ai_poClStatus->GetIdObject());
		
	//
	// Create an object directory ... (begin the game)
	//
	ClCMObjectDirectory oDirectory(ai_poChannelManager);
	
	if (ai_poDumpInfo != NULL) {
		ai_poDumpInfo->SetCurrentStage("ExecProxy: Deserializes the directory");
		ai_poDumpInfo->m_pObjectDirectory = &oDirectory;
	}
	
	// Deserializo el Directory
	m4return_t iRet = oDirectory.DeSerialize(*ai_poInputIODriver, ai_pIObjDataCache, M4_TRUE);
	if (iRet != M4_SUCCESS)
	{
#ifdef _DEBUG
		cerr << "Error deserializing OIDs or objects !" << endl;
#endif
		pCSResult = oDirectory.CreateClientServerResult();
		pCSResult->SetResponseCode(M4_CSR_ERROR_DESERIALIZE);
		goto RETURN_ERROR;
	}
	
	if (ai_poDumpInfo != NULL) {
		ai_poDumpInfo->SetCurrentStage("ExecProxy: Preparing to execute");
	}
	
	// Localizamos el ClOLTPExecutionContext que sabe que Access hay que ejecutar
	OLTPExecutionContextIndex = oDirectory.GetFirstObjectIndex(OLTPExecutionContextType);
	if (OLTPExecutionContextIndex == -1)
	{
#ifdef _DEBUG
		cerr << "Error no hay ningún OLTPExecutionContext " << endl;
#endif
		pCSResult = oDirectory.CreateClientServerResult();
		pCSResult->SetResponseCode(M4_CSR_ERROR_NO_EXECONTEXT);
		goto RETURN_ERROR;
	}
	
	poOLTPExecutionContext = (ClOLTPExecutionContext*)(oDirectory.GetObjectAddress(OLTPExecutionContextIndex));
	poExecutor = poOLTPExecutionContext->GetExecutor();
	poChannel = poOLTPExecutionContext->GetAccess()->GetpChannel();
	
#ifdef _DEBUG
	cerr << "Ejecutando metodo del canal " << poChannel->GetpChannel_Data()->GetpChannelDef()->Id() << endl;
	cerr << "Conectado al canal " << poChannel->GetHandle() << endl;
#endif
	//
	// Hacemos la ejecución del método
	//
	
	// PROXY pre status setup
	
	ai_poChannelManager->GetpExecutor()->SetpCancelStatus(ai_poClStatus);
	
	if (ai_poDumpInfo != NULL) {
		ai_poDumpInfo->SetCurrentStage("ExecProxy: Executing...");
		*ai_poDumpInfo << "Executing method of channel " << poChannel->GetpChannel_Data()->GetpChannelDef()->Id() << endl;
		*ai_poDumpInfo << "Stack State: " << *poExecutor << endl;
	}
	
	if (ai_poConfiguration != NULL) {
		// Set the properties...
		
		// 1st: method executing...
		ClExecutorRuntimeInfo oInfo;
		
		ai_poChannelManager->GetpExecutor()->GetRTInfo(&oInfo);
		string info=oInfo.GetInfoAsString(); 
		oInfo.Reset();
		
		ai_poConfiguration->SetStringValue("", PROP_EXECUTION_DESCRIPTION, (m4pchar_t)info.c_str());
		
		// 2nd: initial channel list...
		ClChannelsInfo oAuxChannelsInfo(oDirectory);
		
		ai_poConfiguration->SetStringValue("", PROP_OBJECT_LIST, (m4pchar_t)oAuxChannelsInfo.ConvertToString());
	}
	
	/* Bug 0164116
	Hay que crear una referencia artificial a cada canal antes de la ejecución y borrarla después.
	Esto es para que no se borren estos canal si hay borrado de niveles 2.
	*/
	iAccesses = 0;
	ppoAccess = new ClAccess*[ oDirectory.GetCountByType( ChannelObjectType ) ];

	for( i = oDirectory.GetFirstObjectIndex( ChannelObjectType ); i != -1; i = oDirectory.GetNextObjectIndex() )
	{
		ClChannel	*poDirectoryChannel = (ClChannel*)oDirectory.GetObjectAddress( i );

		if( poDirectoryChannel != NULL )
		{
			ppoAccess[ iAccesses++ ] = poDirectoryChannel->CreateAccess();
		}
	}

	iExecResult = poExecutor->Execute();

	for( i = 0; i < iAccesses; i++ )
	{
		// Se liberan los accesos creados
		ppoAccess[ i ]->Destroy();
	}

	delete [] ppoAccess;

	if (ai_poConfiguration != NULL) {
		ai_poConfiguration->SetStringValue("", PROP_EXECUTION_DESCRIPTION, "-");
		ai_poConfiguration->SetStringValue("", PROP_OBJECT_LIST, "-");
	}

	if (ai_poDumpInfo != NULL) {
		ai_poDumpInfo->SetCurrentStage("ExecProxy: Processing after execution");
	}
	
	// PROXY pos status setup
	ai_poChannelManager->GetpExecutor()->SetpCancelStatus(NULL);

	/* Bug 0069602
	Solo se serializan las relaciones de nivel 2 si la ejecución ha acabado bien
	*/
	oDirectory.SetbProhibitL2RelationSerialize(M4_TRUE);

	/* Bug 0069695
	Hay que chequear que los canales y accesos del objdir existan
	*/
	oDirectory.CheckObjectExistence() ;

	switch (iExecResult) {
	case M4_SUCCESS:
		oDirectory.SetbProhibitL2RelationSerialize(M4_FALSE);
		break;

	case M4CL_EXECUTION_INTERRUPTION:
#ifdef _DEBUG
		cerr << "El exec retorna el error " << m4int32_t(iRet) << endl;
#endif
		iRet = PrepareExecutionInterruptionReturn(oDirectory, (ClExecutor*)poExecutor);
		break;

	case M4CL_USER_ABORT_ALL_REQUEST:
		if (ai_poClStatus) {
			m4char_t buffer[128];
			sprintf(buffer, "In proxy execution. Cancelation processed in request id %d.", ai_poClStatus->GetIdObject() ) ;
			DUMP_CHLOG_DEBUGF ( M4_CANCELLATION_DETECTED , buffer);
		}

	default:
#ifdef _DEBUG
		cerr << "El exec retorna el error " << m4int32_t(iRet) << endl;
#endif
		pCSResult = oDirectory.CreateClientServerResult(M4_TRUE);
		pCSResult->SetResponseCode(M4_CSR_ERROR_EXECUTE);
		m4char_t sResult[50];
		sprintf(sResult, "%d", iExecResult);
		pCSResult->AddMess(sResult);
	}

RETURN_ERROR:
	if (ai_poDumpInfo != NULL) {
		ai_poDumpInfo->SetCurrentStage("ExecProxy: Serializing the directory");
		ai_poDumpInfo->m_pObjectDirectory = NULL;
	}
	
	iRet = oDirectory.Serialize(*ao_poOutputIODriver);
	
	if (ai_poDumpInfo != NULL) {
		ai_poDumpInfo->SetCurrentStage("ExecProxy: Finishes the execution.");
	}
	
	switch (iExecResult) {
	case M4_SUCCESS:
	case M4CL_USER_ABORT_ALL_REQUEST:
		if (poExecutor) {
			poExecutor->Reset();
		}
		break;
	}

#ifdef _DEBUG
	cerr << endl << "End of PROXY remote execution" << endl;
#else
	m4TraceLevel(2, cerr << "PROXY-");
#endif
	
	return iRet;
}

// defines the processor function
typedef m4return_t (ExecFuncProcType)(ClDataStoreIODriver* const ai_poInputIODriver,
									  ClDataStoreIODriver* const ao_poOutputIODriver,
									  ClProxyManager* const      ai_poProxyManager,
									  ClChannel* const           ai_poSessionChannel,
									  m4uint32_t                 ai_iSessionId,
									  m4pcchar_t                 ai_pcUserName,
									  ClStatus* const            ai_poClStatus,
									  ExecDumpInfo* const        ai_poDumpInfo,
									  ClConfiguration* const     ai_poConfiguration);

/**
 * Acciones comunes a la mayoría de los comandos de proxy.
 * Estas acciones hay que hacerlas antes de nada, y son:
 *		Gets the proxy space name.
 *		Gets the proxy.
 *		Set the session channel.
 *		Checks the use status (the space must be already created).
 *		Gets the channel manager.
 *
 * @param ai_pszParent			Cadena que identifica la función que me ha llamado.
 * @param ai_pszProxyId[]		Cadena donde guardo el id del espacio proxy.
 * @param ao_pChannelManager	A traves de este parámetro devuelvo el puntero al ChannelManager.
 * @param ai_pInputIODriver		IODriver de lectura.
 * @param ai_pOutputIODriver	IODriver de escritura.
 * @param ai_pProxyManager		Gestor de espacios proxy.
 * @param ai_pSessionChannel	Canal sesión.
 * @param ai_iSessionId			Id de sesión.
 * @param ai_pszUserName		Nombre del usuario.
 * @param ai_pDumpInfo			Objeto para volcar información durante la ejecución.
 * @return El espacio proxy con el que vamos a trabajar a continuación.
 */
static ClProxySpace* const ProxyCmdPre(const m4char_t* const      ai_pszParent,
									   m4char_t                   ai_pszProxyId[],
									   ClChannelManager*&         ao_pChannelManager,
									   ClDataStoreIODriver* const ai_pInputIODriver,
									   ClDataStoreIODriver* const ai_pOutputIODriver,
									   ClProxyManager* const      ai_pProxyManager,
									   ClChannel* const           ai_pSessionChannel,
									   m4uint32_t                 ai_iSessionId,
									   const m4char_t* const      ai_pszUserName,
									   ExecDumpInfo* const        ai_pDumpInfo,
									   IObjDataCache**			  ao_ppIObjDataCache)
{
	if (ai_pDumpInfo) {
		ai_pDumpInfo->SetCurrentStage(/*ai_pszParent ": */"Gets the Space");
		ai_pDumpInfo->m_pChannelManager = NULL;
		ai_pDumpInfo->m_pProxySpace = NULL;
	}

	// Gets the proxy space name.
	m4return_t iRet = ai_pInputIODriver->Read(ai_pszProxyId);
	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_IOD_READ);
		goto RETURN_ERROR;
	}

	// Gets the proxy.
	ClProxySpace* pProxySpace;
	pProxySpace = ai_pProxyManager->GetSpace(ai_pszProxyId,M4_FALSE);
	if (!pProxySpace) {
		DUMP_CHLOG_ERRORF(M4_ERR_PROXY_BUILD_PXSPACE, ai_pszProxyId);
		goto RETURN_ERROR;
	}

	if (ai_pDumpInfo) {
		ai_pDumpInfo->SetCurrentStage(/*ai_pszParent ": */"Sets the Session Channel");
		ai_pDumpInfo->m_pChannelManager = pProxySpace->GetChannelManager();
		ai_pDumpInfo->m_pProxySpace = pProxySpace;
	}

	// Set the session channel.
	iRet = pProxySpace->SetSessionChannel(ai_pSessionChannel, ai_iSessionId, ai_pszUserName);
	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_SESSION_CHANNEL);
		goto RETURN_ERROR;
	}
	
	// Checks the use status (the space must be already created).
	if (pProxySpace->GetUseStatus() != M4_TRUE) {
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_WRONG_STATE);
		goto RETURN_ERROR;
	}

	// Gets the channel manager.
	ao_pChannelManager = (ClChannelManager*)pProxySpace->GetChannelManager();

	if( ao_ppIObjDataCache != NULL )
	{
		// La cache de autos sólo se puede obtener cuando ya se tiene el channelmanager
		m4char_t	acTempBuffer[ 100 ] ;

		sprintf( acTempBuffer, "%d", ai_iSessionId ) ;
		*ao_ppIObjDataCache = ao_pChannelManager->GetObjDataCache( acTempBuffer ) ;
	}

	// User actions auditory
	ao_pChannelManager->UserActionsAudit.SwitchOn() ;

	return pProxySpace;

RETURN_ERROR:
	if (strcmp(ai_pszParent, "ProxyCmdExecute") == 0) {
		ProxyWriteError(ai_pOutputIODriver, M4_CSR_ERROR_EXECUTE, "-1");
	}
	ai_pOutputIODriver->Write((m4return_t)M4_ERROR);
	return 0;
}

/**
 * Acciones comunes a la mayoría de los comandos de proxy.
 * Estas acciones hay que hacerlas al final del todo, y son:
 *		Release the space.
 *
 * @param ai_pszParent			Cadena que identifica la función que me ha llamado.
 * @param ai_pszProxyId[]		Cadena donde guardo el id del espacio proxy.
 * @param ai_pOutputIODriver	IODriver de escritura.
 * @param ai_pProxyManager		Gestor de espacios proxy.
 * @param ai_pProxySpace		Espacio proxy con el que queremos trabajar.
 * @param ai_pDumpInfo			Objeto para volcar información durante la ejecución.
 * @return m4return_t.
 */
static m4return_t ProxyCmdPost(const m4char_t* const      ai_pszParent,
							   m4char_t                   ai_pszProxyId[],
							   ClDataStoreIODriver* const ai_pOutputIODriver,
							   ClProxyManager* const      ai_pProxyManager,
							   ClProxySpace* const        ai_pProxySpace,
							   ExecDumpInfo* const        ai_pDumpInfo)
{
	// Release the space.
	if (ai_pDumpInfo) {
		ai_pDumpInfo->SetCurrentStage(/*ai_pszParent ": */"Releasing Space");
		ai_pDumpInfo->m_pChannelManager = NULL;
		ai_pDumpInfo->m_pProxySpace = NULL;
	}

	ClChannelManager * pChannelManager = ai_pProxySpace->GetChannelManager();

	// User actions auditory
	pChannelManager->UserActionsAudit.SwitchOff( M4_TRUE ) ;

	m4return_t iRet = ai_pProxyManager->ReleaseSpace(ai_pProxySpace);
	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_ERRORF(M4_ERR_PROXY_BUILD_PXSPACE, ai_pszProxyId);
		if (ai_pOutputIODriver) {
			ai_pOutputIODriver->Write(iRet);
		}
	}
	
	return iRet;
}


static m4return_t ProxyCmdGetSpace(ClDataStoreIODriver* const ai_poInputIODriver,
								   ClDataStoreIODriver* const ao_poOutputIODriver,
								   ClProxyManager* const      ai_poProxyManager,
								   ClChannel* const           ai_poSessionChannel,
								   m4uint32_t                 ai_iSessionId,
								   m4pcchar_t                 ai_pcUserName,
								   ClStatus* const            ai_poClStatus,
								   ExecDumpInfo* const        ai_poDumpInfo,
								   ClConfiguration* const     ai_poConfiguration)
{
	if (ai_poDumpInfo != NULL)
	{
		ai_poDumpInfo->SetCurrentStage("ProxyCmdGetSpace: Executing Get Space");
		ai_poDumpInfo->m_pChannelManager = NULL;
		ai_poDumpInfo->m_pProxySpace = NULL;
	}

	// gets the proxy space name
	m4return_t iResult = M4_ERROR;
	m4char_t pcProxyID [PROXY_MAX_ID] = "";
	if (ai_poInputIODriver->Read(pcProxyID) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_PROXY_IOD_READ );
		ao_poOutputIODriver->Write(iResult);
		return M4_ERROR;
	}
	
	// generates the new proxy
	ClProxySpace *poClProxySpace = ai_poProxyManager->GetSpace(pcProxyID, M4_TRUE);
	if (poClProxySpace == NULL)
	{
		DUMP_CHLOG_ERRORF (M4_ERR_PROXY_BUILD_PXSPACE, pcProxyID);
		ao_poOutputIODriver->Write(iResult);
		return M4_ERROR;
	}

	if (ai_poDumpInfo != NULL)
	{
		ai_poDumpInfo->SetCurrentStage("ProxyCmdGetSpace: Getting session channel");
		ai_poDumpInfo->m_pChannelManager = poClProxySpace->GetChannelManager();
		ai_poDumpInfo->m_pProxySpace = poClProxySpace;
	}

	// sets the session channel
	iResult = poClProxySpace->SetSessionChannel(ai_poSessionChannel, ai_iSessionId, ai_pcUserName);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_PROXY_SESSION_CHANNEL );
		ao_poOutputIODriver->Write(iResult);
		return M4_ERROR;
	}

	ClChannelManager * pChannelManager = poClProxySpace->GetChannelManager();
	

	// sets the unique key
	strcpy(pcProxyID,poClProxySpace->GetUniqueID());
	
	// sets the result status
	iResult = M4_SUCCESS;
	if (ao_poOutputIODriver->Write(iResult) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_IOD_WRITE ) ;
		return M4_ERROR;
	}
	
	// writes the unique ID
	if (ao_poOutputIODriver->Write(pcProxyID) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_IOD_WRITE) ;
		return M4_ERROR;
	}

	iResult = ProxyCmdPost("ProxyCmdGetSpace", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
	return iResult;
}


static m4return_t ProxyCmdGetView(ClDataStoreIODriver* const ai_poInputIODriver,
								  ClDataStoreIODriver* const ao_poOutputIODriver,
								  ClProxyManager* const      ai_poProxyManager,
								  ClChannel* const           ai_poSessionChannel,
								  m4uint32_t                 ai_iSessionId,
								  m4pcchar_t                 ai_pcUserName,
								  ClStatus* const            ai_poClStatus,
								  ExecDumpInfo* const        ai_poDumpInfo,
								  ClConfiguration* const     ai_poConfiguration)
{
	m4char_t pcProxyID [PROXY_MAX_ID] = "";
	ClChannelManager* poClChannelManager = 0;
	IObjDataCache* pIObjDataCache = NULL ;

	ClProxySpace* poClProxySpace = ProxyCmdPre("ProxyCmdGetView", pcProxyID, poClChannelManager, ai_poInputIODriver, ao_poOutputIODriver, ai_poProxyManager, ai_poSessionChannel, ai_iSessionId, ai_pcUserName, ai_poDumpInfo, &pIObjDataCache);
	if (!poClProxySpace) return M4_ERROR;

	// local variables
	m4char_t pcViewID [PROXY_MAX_VIEW_ID] = "";
	m4uint32_t lViewID = 0;
	m4return_t iResult = M4_ERROR;
	
	if (ai_poDumpInfo != NULL)
	{
		ai_poDumpInfo->SetCurrentStage("ProxyCmdGetView: Gets the Remote View");
	}

	// gets the view name
	if (ai_poInputIODriver->Read(pcViewID) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_PROXY_IOD_READ );
		ao_poOutputIODriver->Write(iResult);
		/* Bug 0114801
		Si se produce un error hay que liberar
		*/
		ProxyCmdPost("ProxyCmdGetView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_ERROR;
	}
	
	// initialies output status
	iResult = M4_SUCCESS;

	// writes the first level serialization state
	iResult = ao_poOutputIODriver->Write(iResult);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_IOD_WRITE ) ;
		ProxyCmdPost("ProxyCmdGetView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_ERROR;
	}
	
	// creates the directory
	ClCMObjectDirectory* poDirectory = new ClCMObjectDirectory(poClChannelManager);
	if (poDirectory == NULL)
	{
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_OUT_OF_MEMORY );
		ProxyCmdPost("ProxyCmdGetView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_ERROR;
	}

	if (ai_poDumpInfo != NULL)
	{
		ai_poDumpInfo->SetCurrentStage("ProxyCmdGetView: Deserializes the directory");
		ai_poDumpInfo->m_pObjectDirectory = poDirectory;
	}

	// deserializes the directory
	m4bool_t bReturn = M4_FALSE;
	iResult = poDirectory->DeSerialize(*ai_poInputIODriver, pIObjDataCache, M4_TRUE);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_OBJDIR_DESER );
		bReturn = M4_TRUE;
	}

	// gets the number of objects in directory
	m4uint16_t iObjCount = poDirectory->GetCount();
	M4_ASSERT(iObjCount == 1 || iObjCount == 2);

	if (ai_poDumpInfo != NULL)
	{
		ai_poDumpInfo->SetCurrentStage("ProxyCmdGetView: Process directory info");
	}

	// keeps all objects in the directory alive (channels and metachannels)
	for (m4uint16_t i = 0; i < iObjCount; i++)
	{
		// don't serialize the channel back
		poDirectory->SetDoSerialize(i, M4_FALSE);
		
		// don´t delete the channel at the end
		poDirectory->SetKeepAlive(i, M4_TRUE);
	}

	if (ai_poDumpInfo != NULL)
	{
		ai_poDumpInfo->SetCurrentStage("ProxyCmdGetView: Serializes the directory");
	}

	// serializes the directory
	iResult = poDirectory->Serialize(*ao_poOutputIODriver);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_OBJDIR_SER);
		ProxyCmdPost("ProxyCmdGetView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_ERROR;
	}
		
	// returns if error occurs
	if (bReturn == M4_TRUE)
	{
		ProxyCmdPost("ProxyCmdGetView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_SUCCESS;
	}
	
	// returns the status OK
	iResult = M4_SUCCESS;
	if (ao_poOutputIODriver->Write(iResult) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_PROXY_IOD_WRITE);
		ProxyCmdPost("ProxyCmdGetView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_ERROR;
	}

	// gets the channel index from the directory
	m4int16_t iChannelIndex = poDirectory->GetFirstObjectIndex(ChannelObjectType);
	M4_ASSERT(iChannelIndex != -1);
	if (iChannelIndex == -1)
	{
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_OBJDIR_SER);
		ProxyCmdPost("ProxyCmdGetView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_ERROR;
	}

	// gets the channel from the directory
	ClChannel* poClChannel = (ClChannel*)poDirectory->GetObjectAddress(iChannelIndex);
	M4_ASSERT(poClChannel != NULL);

	if (ai_poDumpInfo != NULL)
	{
		ai_poDumpInfo->SetCurrentStage("ProxyCmdGetView: Deletes the directory");
		ai_poDumpInfo->m_pObjectDirectory = NULL;
	}

	// delets the object directory
	delete poDirectory;
	
	// writes the unique ID
	lViewID = poClChannel->GetHandle();
	if (ao_poOutputIODriver->Write(lViewID) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR( M4_ERR_PROXY_IOD_WRITE );
		ProxyCmdPost("ProxyCmdGetView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_ERROR;
	}

	iResult = ProxyCmdPost("ProxyCmdGetView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
	return iResult;
}


static m4return_t ProxyCmdReleaseSpace(ClDataStoreIODriver* const ai_poInputIODriver,
									   ClDataStoreIODriver* const ao_poOutputIODriver,
									   ClProxyManager* const      ai_poProxyManager,
									   ClChannel* const           ai_poSessionChannel,
									   m4uint32_t                 ai_iSessionId,
									   m4pcchar_t                 ai_pcUserName,
									   ClStatus* const            ai_poClStatus,
									   ExecDumpInfo* const        ai_poDumpInfo,
									   ClConfiguration* const     ai_poConfiguration)
{
	if (ai_poDumpInfo != NULL)
	{
		ai_poDumpInfo->SetCurrentStage("ProxyCmdReleaseSpace: Processing Release space command");
	}

	// local variables 
	m4return_t iResult = M4_ERROR;
	m4char_t pcProxyID [PROXY_MAX_ID] = "";

	// gets the proxy space name
	if (ai_poInputIODriver->Read(pcProxyID) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_PROXY_IOD_READ ) ;
		ao_poOutputIODriver->Write(iResult);
		return M4_ERROR;
	}
	
	if (ai_poDumpInfo != NULL)
	{
		ai_poDumpInfo->SetCurrentStage("ProxyCmdReleaseSpace: Processing Release space command on proxy manager");
	}


	/* Bug 0074293
	Si hay transacciones abiertas hay que cerrarlas y dar un error
	*/
	m4bool_t			bRollBack = M4_FALSE ;
	m4bool_t			bFree = M4_FALSE ;
	ClProxySpace		*poProxySpace = NULL ;
	ClM4ObjService		*poObjService = NULL ;
	ClLConn_Provider	*poConnProvider = NULL ;
	ClLConn_Interface	*pLConn = NULL ;

	poProxySpace = ai_poProxyManager->GetSpace( pcProxyID, M4_FALSE ) ;

	if( poProxySpace != NULL )
	{
		poObjService = poProxySpace->GetObjService() ;

		if( poObjService != NULL )
		{
			poConnProvider = poObjService->GetpConnProvider() ;

			if( poConnProvider != NULL )
			{
				pLConn = poConnProvider->GetConnection( 0, M4_FALSE ) ;

				if( pLConn != NULL )
				{
					if( pLConn->EndTransaction( NULL, M4_FALSE, bFree ) != M4_SUCCESS )
					{
						DUMP_CHLOG_ERROR( M4_CS_ROLL_BACK_INVOQUED_IN_SERVER_SIDE ) ;
						bRollBack = M4_TRUE ;
					}
				}
			}
		}
	}

	// fix bug 0085584
	if (poProxySpace == NULL) {
		// the error should be reported at lower level
		// the proxy is already deleted, return success
		return M4_SUCCESS;
	}

	// removes
	// disables execution
	poProxySpace->SetIsRunning(M4_FALSE);
	iResult = ai_poProxyManager->ReleaseActiveSpace(pcProxyID);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_PROXY_BUILD_PXSPACE , pcProxyID);
		ao_poOutputIODriver->Write(iResult);
		return M4_ERROR;
	}

	if (ai_poDumpInfo != NULL)
	{
		ai_poDumpInfo->SetCurrentStage("ProxyCmdReleaseSpace: Returning");
	}

	if( bRollBack == M4_TRUE )
	{
		iResult = M4_ERROR ;
	}

	// returns
	ao_poOutputIODriver->Write(iResult);
	return M4_SUCCESS;
}


static m4return_t ProxyCmdExecute(ClDataStoreIODriver* const ai_poInputIODriver,
								  ClDataStoreIODriver* const ao_poOutputIODriver,
								  ClProxyManager* const      ai_poProxyManager,
								  ClChannel* const           ai_poSessionChannel,
								  m4uint32_t                 ai_iSessionId,
								  m4pcchar_t                 ai_pcUserName,
								  ClStatus* const            ai_poClStatus,
								  ExecDumpInfo* const        ai_poDumpInfo,
								  ClConfiguration* const     ai_poConfiguration)
{
	m4char_t pcProxyID [PROXY_MAX_ID] = "";
	ClChannelManager* poClChannelManager = 0;
	IObjDataCache* pIObjDataCache = NULL ;

	if (ai_poDumpInfo) {
		ai_poDumpInfo->SetCurrentStage("ProxyCmdExecute: Processing the execute command");
	}

	ClProxySpace* poClProxySpace = ProxyCmdPre("ProxyCmdExecute", pcProxyID, poClChannelManager, ai_poInputIODriver, ao_poOutputIODriver, ai_poProxyManager, ai_poSessionChannel, ai_iSessionId, ai_pcUserName, ai_poDumpInfo, &pIObjDataCache);
	if (!poClProxySpace) return M4_ERROR;

	if (ai_poDumpInfo) {
		ai_poDumpInfo->SetCurrentStage("ProxyCmdExecute: Executes the remote command");
	}

	// executes the method
	m4return_t iRet = ExecProxy(poClChannelManager, ai_poInputIODriver, ao_poOutputIODriver, poClProxySpace, ai_poClStatus, ai_poDumpInfo, ai_poConfiguration, pIObjDataCache);
	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_EXEC_COMMAND);
		/* Bug 0114801
		Si se produce un error hay que liberar
		*/
		ProxyCmdPost("ProxyCmdExecute", pcProxyID, 0, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return iRet;
	}

	iRet = ProxyCmdPost("ProxyCmdExecute", pcProxyID, 0, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
	return iRet;
}


static m4return_t ProxyGetList(ClDataStoreIODriver* const ai_poInputIODriver,
							   ClDataStoreIODriver* const ao_poOutputIODriver,
							   ClProxyManager* const      ai_poProxyManager,
							   ClChannel* const           ai_poSessionChannel,
							   m4uint32_t                 ai_iSessionId,
							   m4pcchar_t                 ai_pcUserName,
							   ClStatus* const            ai_poClStatus,
							   ExecDumpInfo* const        ai_poDumpInfo,
							   ClConfiguration* const     ai_poConfiguration)
{
	if (ai_poDumpInfo) {
		ai_poDumpInfo->SetCurrentStage("ProxyGetList: Process get list command");
	}

	// local variables
	m4return_t iRet = M4_ERROR;
	m4int32_t iCount ;
	m4char_t pcProxyID [PROXY_MAX_ID] = "";
	ClChannelManager* poClChannelManager = 0;

	ClProxySpace* poClProxySpace = ProxyCmdPre("ProxyGetList", pcProxyID, poClChannelManager, ai_poInputIODriver, ao_poOutputIODriver, ai_poProxyManager, ai_poSessionChannel, ai_iSessionId, ai_pcUserName, ai_poDumpInfo, NULL);
	if (!poClProxySpace) return M4_ERROR;

	// gets the number of channels
	ClChannelDirectory *poClChannelDirectory = &poClChannelManager->Channel;
	m4int32_t iNChannels = poClChannelDirectory->Count(),iNRootChannels=iNChannels;

	if (ai_poDumpInfo) {
		ai_poDumpInfo->SetCurrentStage("ProxyGetList: Generates the channel info");
	}

	// maybe there is no channels there
	if (!iNRootChannels) {
		ao_poOutputIODriver->Write(iRet);
		ao_poOutputIODriver->Write(iNRootChannels);
		/* Bug 0114801
		Si se produce un error hay que liberar
		*/
		ProxyCmdPost("ProxyGetList", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_SUCCESS;
	}

	// detects the root channels
	ClChannel **poRootChannels = new ClChannel * [iNChannels];
	if (!poRootChannels) {
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_OUT_OF_MEMORY );
		iRet = M4_ERROR;
		ao_poOutputIODriver->Write(iRet);
		ProxyCmdPost("ProxyGetList", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_ERROR;
	}

	// initializes the root channels vectors
	for ( iCount = 0; iCount < iNChannels; iCount++) {
		ClChannel *poChannel = (*poClChannelDirectory)[iCount];
		poRootChannels[iCount] = (poChannel->IsBuilt() == M4_FALSE) ? NULL : poChannel;
	}

	// deletes referenced channels
	for (iCount = 0; iCount < iNChannels; iCount++) {
		// gets the number of L2 channels
		ClChannel *poClChannel = poRootChannels[iCount];
		if (!poClChannel)
			continue;

		// if I am a dynamic channel, I delete myself
		if (poClChannel->GetpBackMCR())
		{
			poRootChannels[iCount] = 0;
			/* Bug 0100048
			Hay que decrementar el número de objetos que se serializan
			*/
			iNRootChannels--;
		}

		// gets the number or L2
		ClLevel2 *poClLevel2 = &poClChannel->Level2;
		m4int32_t iL2 = poClLevel2->m_oInstanceMap.Count();

		// for each L2 channel deletes the pointer in out root channels list
		for (m4int32_t iCount2 = 0; iCount2 < iL2; iCount2++) {
			ClChannel *poChannel2 = poClLevel2->m_oInstanceMap.GetAccess(iCount2)->GetpChannel();
			for (m4int32_t iCount3 = 0; iCount3 < iNChannels; iCount3++) {
				if (poChannel2 == poRootChannels[iCount3]) {
					poRootChannels[iCount3] = 0;
					iNRootChannels--;
					break;
				}
			}
		}
	}

	// Serializes the output
	iRet = ao_poOutputIODriver->Write((m4return_t)M4_SUCCESS);
	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_IOD_WRITE);
		ProxyCmdPost("ProxyGetList", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_ERROR;
	}

	// writes the number of root nodes
	iRet = ao_poOutputIODriver->Write(iNRootChannels);
	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_IOD_WRITE);
		ProxyCmdPost("ProxyGetList", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_ERROR;
	}


	SetChannelRefs_t setChnReferences;

	// writes the data for the nodes
	for (iCount = 0; iCount < iNChannels; iCount++) {
		if (!poRootChannels[iCount])
			continue;

		setChnReferences.clear();

		// writes the root channel info
		iRet = WriteChannelInfo( poRootChannels[iCount], ao_poOutputIODriver, setChnReferences );
		if (iRet != M4_SUCCESS)
			break;
	}

	delete poRootChannels;

	iRet = ProxyCmdPost("ProxyGetList", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
	return iRet;
}


static m4return_t ProxyGetDebugView(ClDataStoreIODriver* const ai_poInputIODriver,
									ClDataStoreIODriver* const ao_poOutputIODriver,
									ClProxyManager* const      ai_poProxyManager,
									ClChannel* const           ai_poSessionChannel,
									m4uint32_t                 ai_iSessionId,
									m4pcchar_t                 ai_pcUserName,
									ClStatus* const            ai_poClStatus,
									ExecDumpInfo* const        ai_poDumpInfo,
									ClConfiguration* const     ai_poConfiguration)
{
	if (ai_poDumpInfo) {
		ai_poDumpInfo->SetCurrentStage("ProxyGetDebugView: Processing get debug view command");
	}

	m4char_t pcProxyID [PROXY_MAX_ID] = "";
	ClChannelManager* poClChannelManager = 0;
	IObjDataCache* pIObjDataCache = NULL ;

	ClProxySpace* poClProxySpace = ProxyCmdPre("ProxyGetDebugView", pcProxyID, poClChannelManager, ai_poInputIODriver, ao_poOutputIODriver, ai_poProxyManager, ai_poSessionChannel, ai_iSessionId, ai_pcUserName, ai_poDumpInfo, &pIObjDataCache);
	if (!poClProxySpace) return M4_ERROR;

	// reads the channel id
	m4uint32_t iRemoteId = 0;
	m4return_t iRet = ai_poInputIODriver->Read(iRemoteId);
	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_IOD_READ);
		ao_poOutputIODriver->Write(iRet);
		/* Bug 0114801
		Si se produce un error hay que liberar
		*/
		ProxyCmdPost("ProxyGetDebugView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return iRet;
	}

	if (ai_poDumpInfo) {
		ai_poDumpInfo->SetCurrentStage("ProxyGetDebugView: Scans the channel manager");
	}

	// Checks for the channel.
	// Comprobamos si el canal realmente está en el channelManager.
	ClChannelDirectory& channelDirectory = poClChannelManager->Channel;
	m4int32_t iNChannels = channelDirectory.Count();
	ClChannel* pChannel = 0;
	ClChannel* pDebugView = 0;
	for (m4int32_t iCount = 0; iCount < iNChannels; iCount++) {
		pChannel = channelDirectory[iCount];
		if (pChannel->GetHandle() == iRemoteId) {
			pDebugView = pChannel;
			break;
		}
	}
	if (!pDebugView) {
		DUMP_CHLOG_ERRORF(M4_ERR_PROXY_DEBUG_VIEW, iRemoteId);
		ao_poOutputIODriver->Write((m4bool_t)M4_ERROR);
		ProxyCmdPost("ProxyGetDebugView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_ERROR;
	}

	// Bug 0313006 Se guardan los modos de serialización para restauralos
	m4uint8_t iRecSerMod = pDebugView->GetRecordSerializationMode();
	m4uint8_t iIteSerMod = pDebugView->GetItemSerializationMode();

	// Puede ser que ese canal se estuviera depurando. Así que para no fastidiarle
	// el modo de depuración me lo guardo y se lo vuelvo a asignar al terminar.
	const ClChannel::eDebugMode_t eDebugMode = pDebugView->GetDebugMode();

	// serializes the return code
	iRet = ao_poOutputIODriver->Write(iRet);
	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_IOD_WRITE);
		ProxyCmdPost("ProxyGetDebugView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return iRet;
	}
	
	// processes the object directory
	ClCMObjectDirectory objDir(poClChannelManager);

	// No se mandan niveles 2 y se conserva el tipo
	objDir.SetbProhibitL2RelationSerialize(M4_TRUE);
	objDir.SetKeepChannelCsTypeOnRemote(M4_TRUE);

	/* Bug 0153732
	Se manda el metacanal y el canal para que no creen en cliente
	*/
	m4int16_t i = objDir.IncludeMetaChannel(pDebugView->GetpCMCR(), pDebugView);
	if (i == -1) {
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_IOD_WRITE);
		ProxyCmdPost("ProxyGetDebugView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return iRet;
	}

	// Al deserializar el objDir el canal de depuración se ha enganchado al canal del servidor. Ahora sólo
	// tenemos que cambiar su modo de serialización para que cuando se serialice hacia el cliente se mande el
	// canal completo (todos los registos y todos los ítems).
	pDebugView->SetRecordSerializationMode(M4CL_RECORD_SERIAL_ALL);
	pDebugView->SetItemSerializationMode(M4CL_ITEM_SERIAL_ALL);

	/* Bug 0153732
	Se pone como que no es de debug para que serialice todo
	*/
	pDebugView->SetDebugMode(ClChannel::IS_NOT_DEBUG);

	i = objDir.IncludeChannel(pDebugView, 0);

	if (i == -1) {
		pDebugView->SetRecordSerializationMode(iRecSerMod);
		pDebugView->SetItemSerializationMode(iIteSerMod);
		pDebugView->SetDebugMode(eDebugMode);
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_IOD_WRITE);
		ProxyCmdPost("ProxyGetDebugView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return iRet;
	}
	
	if (ai_poDumpInfo) {
		ai_poDumpInfo->SetCurrentStage("ProxyGetDebugView: Serializes the directory");
		ai_poDumpInfo->m_pObjectDirectory = &objDir;
	}

	// serializes the data back
	iRet = objDir.Serialize(*ao_poOutputIODriver);

	// Bug 0313006 Se restauran los modos de serialización
	pDebugView->SetRecordSerializationMode(iRecSerMod);
	pDebugView->SetItemSerializationMode(iIteSerMod);

	// Puede ser que ese canal se estuviera depurando. Así que para no fastidiarle
	// el modo de depuración me lo guardo y se lo vuelvo a asignar al terminar.
	pDebugView->SetDebugMode(eDebugMode);

	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_OBJDIR_SER);
		ProxyCmdPost("ProxyGetDebugView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return iRet;
	}

	iRet = ProxyCmdPost("ProxyGetDebugView", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
	return iRet;
}


static m4return_t ProxyCmdEndDebugMethod(ClDataStoreIODriver* const ai_pInputIODriver,
										 ClDataStoreIODriver* const ai_pOutputIODriver,
										 ClProxyManager* const      ai_pProxyManager,
										 ClChannel* const           ai_pSessionChannel,
										 m4uint32_t                 ai_iSessionId,
										 m4pcchar_t                 ai_pszUserName,
										 ClStatus* const            ai_pClStatus,
										 ExecDumpInfo* const        ai_pDumpInfo,
										 ClConfiguration* const     ai_pConfiguration)
{
	m4return_t iRet = M4_ERROR;
	m4char_t pszProxyID [PROXY_MAX_ID] = "";
	ClChannelManager* pChannelManager = 0;
	IObjDataCache* pIObjDataCache = NULL ;

	ClProxySpace* pProxySpace = ProxyCmdPre("ProxyCmdEndDebugMethod", pszProxyID, pChannelManager, ai_pInputIODriver, ai_pOutputIODriver, ai_pProxyManager, ai_pSessionChannel, ai_iSessionId, ai_pszUserName, ai_pDumpInfo, &pIObjDataCache);
	if (!pProxySpace) return M4_ERROR;

	ClCMObjectDirectory oDirectory(pChannelManager);
	
	if (ai_pDumpInfo) {
		ai_pDumpInfo->SetCurrentStage("ProxyCmdEndDebugMethod: Deserializes the directory");
		ai_pDumpInfo->m_pObjectDirectory = &oDirectory;
	}
	
	iRet = oDirectory.DeSerialize(*ai_pInputIODriver, pIObjDataCache, M4_TRUE);
	ai_pOutputIODriver->Write(iRet);
	if (iRet != M4_SUCCESS)
	{
		/* Bug 0114801
		Si se produce un error hay que liberar
		*/
		ProxyCmdPost("ProxyCmdEndDebugMethod", pszProxyID, ai_pOutputIODriver, ai_pProxyManager, pProxySpace, ai_pDumpInfo);
		return iRet;
	}

	if (ai_pDumpInfo) {
		ai_pDumpInfo->SetCurrentStage("ProxyCmdEndDebugMethod: Serializing the directory");
		ai_pDumpInfo->m_pObjectDirectory = 0;
	}
	
	//En la última sincronización del server al cliente permitimos el transporte de relaciones de nivel 2
	oDirectory.SetbProhibitL2RelationSerialize(M4_FALSE);

	iRet = oDirectory.Serialize(*ai_pOutputIODriver);
	if (iRet != M4_SUCCESS)
	{
		ProxyCmdPost("ProxyCmdEndDebugMethod", pszProxyID, ai_pOutputIODriver, ai_pProxyManager, pProxySpace, ai_pDumpInfo);
		return iRet;
	}

	// Después de serializar tengo que recorrerme el Channel Manager para buscar canales que todavía estén marcados como de depuración.
	// Esto puede pasar con canales que se han creado por nivel2 y que han viajado al cliente en algún momento. Estos canales no van ha
	// estar en la lista del objDir ya que el cliente no pide su sincronización.
	// A los canales que todavía están marcados como de depuración hay que desactivarles este modo para que si, en un una depuración
	// posterior, se vuelven a utilizar, viajen enteros al cliente (no queremos deltas porque en el cliente se han borrado).
	// Esto sólo puede pasar en depuración de proxy, ya que en depuración OLTP cuando se termina de depurar el método se destruye el
	// espacio, y con el todos los canales.
	ClChannelDirectory& channelDirectory = pChannelManager->Channel;
	const m4uint32_t iNChannels = channelDirectory.Count();
	ClChannel* pChannel = 0;

	for (m4uint32_t iCount = 0; iCount < iNChannels; iCount++) {
		pChannel = channelDirectory[iCount];
		pChannel->SetDebugMode(ClChannel::IS_NOT_DEBUG);
	}

	iRet = ProxyCmdPost("ProxyCmdEndDebugMethod", pszProxyID, ai_pOutputIODriver, ai_pProxyManager, pProxySpace, ai_pDumpInfo);
	return iRet;
}


static m4return_t ProxyEraseAllL2Instances(ClDataStoreIODriver* const ai_poInputIODriver,
										   ClDataStoreIODriver* const ao_poOutputIODriver,
										   ClProxyManager* const      ai_poProxyManager,
										   ClChannel* const           ai_poSessionChannel,
										   m4uint32_t                 ai_iSessionId,
										   m4pcchar_t                 ai_pcUserName,
										   ClStatus* const            ai_poClStatus,
										   ExecDumpInfo* const        ai_poDumpInfo,
										   ClConfiguration* const     ai_poConfiguration)
{
	if (ai_poDumpInfo) {
		ai_poDumpInfo->SetCurrentStage("ProxyEraseAllL2Instances: Processing release all level2 instances command");
	}

	m4char_t pcProxyID [PROXY_MAX_ID] = "";
	ClChannelManager* poClChannelManager = 0;

	ClProxySpace* poClProxySpace = ProxyCmdPre("ProxyEraseAllL2Instances", pcProxyID, poClChannelManager, ai_poInputIODriver, ao_poOutputIODriver, ai_poProxyManager, ai_poSessionChannel, ai_iSessionId, ai_pcUserName, ai_poDumpInfo, NULL);
	if (!poClProxySpace) return M4_ERROR;

	// reads the channel id
	m4uint32_t iRemoteId = 0;
	m4return_t iRet = ai_poInputIODriver->Read(iRemoteId);
	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_IOD_READ);
		ao_poOutputIODriver->Write(iRet);
		/* Bug 0114801
		Si se produce un error hay que liberar
		*/
		ProxyCmdPost("ProxyEraseAllL2Instances", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return iRet;
	}

	if (ai_poDumpInfo) {
		ai_poDumpInfo->SetCurrentStage("ProxyEraseAllL2Instances: Scans the channel manager");
	}

	// Checks for the channel.
	// Comprobamos si el canal realmente está en el channelManager.
	ClChannelDirectory& channelDirectory = poClChannelManager->Channel;
	m4int32_t iNChannels = channelDirectory.Count();
	ClChannel* pChannel = 0;
	ClChannel* pEraseChannel = 0;
	for (m4int32_t iCount = 0; iCount < iNChannels; iCount++) {
		pChannel = channelDirectory[iCount];
		if (pChannel->GetHandle() == iRemoteId) {
			pEraseChannel = pChannel;
			break;
		}
	}
	if (!pEraseChannel) {
		DUMP_CHLOG_ERRORF(M4_ERR_PROXY_NO_SUCH_M4OBJ, iRemoteId);
		ao_poOutputIODriver->Write((m4bool_t)M4_ERROR);
		ProxyCmdPost("ProxyEraseAllL2Instances", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
		return M4_ERROR;
	}

	if (ai_poDumpInfo) {
		ai_poDumpInfo->SetCurrentStage("ProxyEraseAllL2Instances: Release instances");
	}

	iRet = pEraseChannel->Level2.EraseAllL2Instances(M4_TRUE, M4_FALSE, NULL);

	ao_poOutputIODriver->Write(iRet);

	iRet = ProxyCmdPost("ProxyEraseAllL2Instances", pcProxyID, ao_poOutputIODriver, ai_poProxyManager, poClProxySpace, ai_poDumpInfo);
	return iRet;
}


// main routine
m4return_t ExecForProxy(M4DataStorage* const   ai_poInputDataStorage,
						M4DataStorage* const   ao_poOutputDataStorage,
						ClProxyManager* const  ai_poProxyManager, 
						ClChannel* const       ai_poSessionChannel,
						m4uint32_t             ai_iSessionId,
						m4pcchar_t             ai_pcUserName,
						ClStatus* const        ai_poClStatus,
						ExecDumpInfo* const    ai_poDumpInfo,
						ClConfiguration* const ai_poConfiguration)
{
	// command vector
	static ExecFuncProcType *pVExecCmd[]=
	{
		NULL,
		ProxyCmdGetView,
		ProxyCmdReleaseSpace,
		NULL,
		NULL,
		NULL,
		NULL,
		ProxyGetList,
		ProxyGetDebugView,
		ProxyCmdEndDebugMethod,
		ProxyEraseAllL2Instances
	};

	// Ahora siempre me tienen que pasar el canal sesión (también en SM).
	M4_ASSERT (ai_poSessionChannel);
	if (!ai_poSessionChannel) return M4_ERROR;
	
	// resets the DumpInfo
	if (ai_poDumpInfo != NULL)
		ai_poDumpInfo->SetCurrentStage("ExecForProxy: Preparing to execute. Decoding command");

	DUMP_CHLOG_DEBUGF ( M4_INF_PROXY_GENERIC_0 , "Proxy executor starting");
#ifdef _DEBUG
    cerr << "Executor for Proxy starting...." << endl;
	cerr.flush();
#endif

	ClDataStoreIODriver IODIn;
	m4return_t iResult = IODIn.Init(GEN_IO_MODE_READ, ai_poInputDataStorage);
	if (iResult != M4_SUCCESS) return iResult;

	ClDataStoreIODriver IODOut;
	iResult = IODOut.Init(GEN_IO_MODE_WRITE, ao_poOutputDataStorage);
	if (iResult != M4_SUCCESS) return iResult;

	// gets the command;
	m4int16_t ieCommand;
	iResult = IODIn.Read(ieCommand);
	if (iResult != M4_SUCCESS) {
		IODOut.Write(iResult);
		return iResult;
	}
	const ClProxyManager::ExecTypeCommand eCommand = (ClProxyManager::ExecTypeCommand)ieCommand;

	// selects the command
	switch (eCommand) {
	case ClProxyManager::PROXY_GET_SPACE:
		iResult = ProxyCmdGetSpace(&IODIn, &IODOut, ai_poProxyManager, ai_poSessionChannel, ai_iSessionId, ai_pcUserName, ai_poClStatus, ai_poDumpInfo, ai_poConfiguration);
		break;
	case ClProxyManager::PROXY_GET_VIEW:
	case ClProxyManager::PROXY_RELEASE_SPACE:
	case ClProxyManager::PROXY_GET_LIST:
	case ClProxyManager::PROXY_GET_DEBUG_VIEW:
	case ClProxyManager::PROXY_END_DEBUG_METHOD:
	case ClProxyManager::PROXY_ERASE_ALL_L2_INSTANCES:
		iResult = pVExecCmd[ieCommand](&IODIn, &IODOut, ai_poProxyManager, ai_poSessionChannel, ai_iSessionId, ai_pcUserName, ai_poClStatus, ai_poDumpInfo, ai_poConfiguration);
		break;
	case ClProxyManager::PROXY_EXECUTE_COMMAND:
		iResult = ProxyCmdExecute(&IODIn, &IODOut, ai_poProxyManager, ai_poSessionChannel, ai_iSessionId, ai_pcUserName, ai_poClStatus, ai_poDumpInfo, ai_poConfiguration);
		break;
	// unindetified field
	default:
		iResult = IODOut.Write((m4return_t)M4_ERROR);
		if (iResult != M4_SUCCESS) return iResult;
	}

	// resets the DumpInfo
	if (ai_poDumpInfo != NULL)
		ai_poDumpInfo->SetCurrentStage("ExecForProxy: End of Processing");

	return iResult;
}
