//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           serial
// File:            clexadap.cpp
// Project:          Mind3x
// Author:           Meta Software M.S. , S.A
// Date:            30/06/1998
// Language:         C++
//
// Definition:
//   This module defines the code for the execution adaptor
//
//==============================================================================
// Solo CLIENT y DIC


#include "csres.hpp"
#include "dsres.hpp"



#include "clexadap.hpp"
#include "genproxy.hpp"

#include "cldatstoiod.hpp"
#include "cllgadap.hpp"
#include "access.hpp"
#include "channel.hpp"
#include "executor.hpp"
#include "dm.hpp"
#include "px_block.hpp"
#include "chan_knl.hpp"

#include "objdir.hpp"
#include "cs_result.hpp"
#include "execntxt.hpp"
#include "chlog.hpp"
#include "sttwrap.hpp"		// M4DM
#include "sttlib.hpp"		// M4DM
#include "m4lgadapdef.hpp"
#include "m4cstrace.hpp"
#include "clappclientadap.hpp"
#include "cldmcontext.hpp"	// M4DM
#include "m4mdrt.hpp"
#include "m4objreg.hpp"
#include "vmstepper.hpp"
#include "cachedir.hpp"
#include "licensemanagerinterface.hpp"
#include "m4objglb.hpp"


/**
 * Comprueba si un canal está encarrilado. En este caso obtiene el bloque y el registro
 * (del root multibloque) que hay que enviar.
 * Si todavía no existe bloque, lo crea y propaga los items.
 *
 * @param ai_objDir    A este objdir se le informa de cual es el bloque y el registro a serializar.
 * @param ai_pChannel    Canal que se comprueba si está encarrilado.
 * @param ai_pAccess    Acceso de ejecución. Está posicionado sobre el bloque y el registro que renemos que serializar.
 * @param ao_iBlock    Nos sirve para devolver el bloque sobre el que esta posicionado el acceso ai_pAccess.
 * @param ao_iRegister    Nos sirve para devolver el registro sobre el que esta posicionado el acceso ai_pAccess.
 * @return m4return_t.
 */
static m4return_t CheckChannelLaned(ClCMObjectDirectory& ai_objDir,
                                    ClChannel*           ai_pChannel,
                                    ClAccess*            ai_pAccess,
                                    m4uint32_t&          ao_iBlock,
                                    m4uint32_t&          ao_iRegister)
{
	m4pcchar_t pLaneSonNodeId = ai_pChannel->GetpCMCR()->GetLaneSonNodeId();

	if (!pLaneSonNodeId) {
		ao_iBlock = 0;
		ao_iRegister = 0;
		return M4_SUCCESS;
	}

	// El canal esta encarrilado (root multibloque), tengo que indicar que bloque quiero mandar 
	// Ademas si el bloque no existe tengo que crearlo para que se copien las PKs
	ClBlock_Channel_Proxy* pBlockProxy = ai_pAccess->Node[pLaneSonNodeId].RecordSet.GetCurrentBlock();

	if (pBlockProxy->IsNihil() == M4_TRUE) {
		// Si estoy en un bloque Nihil, no tengo que serializarlo
		ao_iBlock = -1;
		ao_iRegister = 0;
	}
	else {
		// Si el bloque no existe, lo creo. Con esto conseguimos la propagacion de items
		if (pBlockProxy->IsValid() == M4_FALSE)
			if (!pBlockProxy->_CreateBlock()) return M4_ERROR;

		ao_iBlock = pBlockProxy->ID.BlockId;
		ao_iRegister = pBlockProxy->ID.RegisterId;
	}

	return ai_objDir.SetRootBlockToSerialize(ai_pChannel, ao_iBlock, ao_iRegister);
}


ClExecAdaptor::ClExecAdaptor()
   : m_poClientAPI(0)
   , m_poClLogonAdaptor(0)
   , m_poCSTrace(0)
   , m_pChannelManager(0)
   , m_eIsDebug(IS_NOT_DEBUG)
{
}

ClExecAdaptor::~ClExecAdaptor()
{
   m_poClientAPI = 0,
   m_poClLogonAdaptor = 0;
   m_poCSTrace = 0;
}

m4return_t ClExecAdaptor::Init(ClCsTrace *ai_poCSTrace, ClAppClientAdap* ai_poClientAPI, ClVMBaseEnv* ai_poRegistry)
{
	if ( !ai_poCSTrace || !ai_poClientAPI || !ai_poRegistry )
	{
		return M4_ERROR;
	}

	m_poCSTrace		= ai_poCSTrace;
	m_poClientAPI	= ai_poClientAPI;
	m_poRegistry	= ai_poRegistry;
	return M4_SUCCESS;
}

/**
 * Se encarga de añadir al objDir los canales y accesos necesarios para la transacción. Estos serán
 * los que ha indicado el ejecutor más los canales proxy o de depuración.
 * También se encarga de comprobar si los canales están encarrilados y de activar la serialización 
 * avanzada en el caso de que el canal que ha disparado la ejecución sea OLTP y lo requiera.
 *
 * @param ai_ppAccess		Array de punteros a los accesos que hay que añadir al objDir.
 * @param ai_iNumAccess		Número de accesos que hay en el array ai_ppAccess.
 * @param ai_hItem			Handle del ítem de ejecución.
 * @param ai_recordSet		RecordSet sobre el que se está ejecutnado.
 * @param ai_IOD			IODriver donde queremos hacer la serializacion.
 * @param ai_objDir			ObjDir donde queremos que se añadan los objetos.
 * @return m4return_t.
 */
m4return_t ClExecAdaptor::PrepareRemoteCall(ClAccess** ai_ppAccess, m4uint32_t ai_iNumAccess, m4uint32_t ai_hItem, ClAccessRecordSet& ai_recordSet, ClGenericIODriver& ai_IOD, ClCMObjectDirectory& ai_objDir)
{
	M4_ASSERT(ai_ppAccess);
	M4_ASSERT(ai_iNumAccess);

	ClAccess* pAccess = 0;
	ClChannel* pChannel = 0;
	m4uint32_t iRemoteId = 0;
	m4int16_t j = -1;
	m4uint32_t iBlock = 0;
	m4uint32_t iRegister = 0;
	m4return_t iRet = M4_ERROR;

	// Incluir los canales en el objDir.
	// Me recorro el vector de atrás a delante para quedarme posicionado sobre
	// el primer canal, que es el canal sobre el que se está ejecutando.

	for (m4int32_t i = ai_iNumAccess - 1 ; i >= 0; i--) {
		pAccess = ai_ppAccess[i];
		pChannel = pAccess->GetpChannel();
		iRemoteId = pChannel->GetRemoteId();

		j = ai_objDir.IncludeChannel(pChannel, iRemoteId);
		if (j == -1) {
			return M4_ERROR;
		}

		j = ai_objDir.IncludeAccessAndCreateOID(pAccess, 0);
		if (j == -1) {
			return M4_ERROR;
		}
		
		// Bloque y registro a serializar
		iRet = CheckChannelLaned(ai_objDir, pChannel, pAccess, iBlock, iRegister);
		if (iRet != M4_SUCCESS) {
			return iRet;
		}
	}

	m4bool_t bIsChannelDebugEnabled = pChannel->IsDebugEnabled();
	m4uint8_t iCSExeType = pChannel->GetpChannelDef()->CsExeType();

	if (bIsChannelDebugEnabled == M4_FALSE) {
		// Compruebo si estoy en una reentrada.
		// Esta comprobación hay que hacerla ya que las ejecuciones reentrantes (como la autoload) no se pueden depurar.
		// Se si estoy en una reentrada porque tengo activo un modo de depuración (OLTP o Proxy).
		// En caso de estar en una reentrada entro en un modo especial que me lo indica.
		if (m_eIsDebug == IS_DEBUG_OLTP || m_eIsDebug == IS_DEBUG_PROXY) {
			m_eIsDebug = ClExecAdaptor::eDebugMode_t(m_eIsDebug + 1);
		}

		// Si es un canal OLTP tengo que fijar su modo de serialización.
		if (iCSExeType == M4CL_CSEXETYPE_OLTP)
		{
			iRet = SetOLTPChannelSerializationMode(pChannel, iBlock, iRegister, ai_hItem, ai_recordSet);
		}
		else if( iCSExeType == M4CL_CSEXETYPE_AUTO )
		{
			// Si es auto no hay que hacer nada y se queda con modo deltas
		}
		else
		{
			// Si se trata de un canal Proxy tengo que añadir el resto de canales proxy al objDir.
			IProxyManager* pProxyManager = m_pChannelManager->GetProxyManager();
			ClProxySpace*  pProxySpace   = pProxyManager->GetSpace("Proxy");

			iRet = WriteProxyHeader(pProxySpace->GetUniqueID(), ai_IOD);
			if (iRet != M4_SUCCESS) return iRet;

			iRet = AddProxyChannels(ai_objDir);
		}
	}
	else
	{
		// Como estoy depurando tengo que determinar que espacio proxy voy a utilizar el de los canalse Proxy o el
		// de los canales OLTP. Si ya tenía un espacio asignado para la depuración sigo utilizando el mismo. Con esto
		// evito que si en una depuración OLTP en el servidor se ha levantado un canal Proxy no se intente ir contra
		// el espacio proxy para continuar la depuración.
		if (m_eIsDebug == IS_NOT_DEBUG)
		{
			if (iCSExeType == M4CL_CSEXETYPE_OLTP)
			{
				// Como es la primera transacción de la depuración tengo que comprobar el modo de serialización
				// del canal OLTP que ha provocado la ejecución.
				iRet = SetOLTPChannelSerializationMode(pChannel, iBlock, iRegister, ai_hItem, ai_recordSet);
				if (iRet != M4_SUCCESS) return iRet;

				m_eIsDebug = IS_DEBUG_OLTP;
			}
			else if( iCSExeType == M4CL_CSEXETYPE_AUTO )
			{
				// Si es AUTO es como una depuración de OLTP pero sin modo de serialización
				m_eIsDebug = IS_DEBUG_OLTP;
			}
			else
			{
				m_eIsDebug = IS_DEBUG_PROXY;
			}
		}
		else
		{
			// Hay que comprobar que el modo de depuración está activo. Si no lo está (por ejemplo
			// por una ejecución desde el inmediate window) lo tratamos como una reentrada.
			ClExecutor* pExecutor = (ClExecutor*)pAccess->GetpExecutor();
			m4bool_t bStepperActive = pExecutor->GetpStepper()->GetActive();

			if (bStepperActive == M4_FALSE)
			{
				if (m_eIsDebug == IS_DEBUG_OLTP || m_eIsDebug == IS_DEBUG_PROXY)
				{
					m_eIsDebug = ClExecAdaptor::eDebugMode_t(m_eIsDebug + 1);
				}
			}
		}

		IProxyManager* pProxyManager = m_pChannelManager->GetProxyManager();
		ClProxySpace* pProxySpace = 0;

		if (m_eIsDebug == IS_DEBUG_OLTP || m_eIsDebug == IS_DEBUG_OLTP_REENTER)
		{
			pProxySpace = pProxyManager->GetSpace("OLTPD", M4_FALSE);	// M4_FALSE porque el espacio ya tiene que existir (no queremos crearlo).
		}
		else
		{
			M4_ASSERT(m_eIsDebug == IS_DEBUG_PROXY || m_eIsDebug == IS_DEBUG_PROXY_REENTER);
			pProxySpace = pProxyManager->GetSpace("Proxy", M4_FALSE);	// M4_FALSE porque el espacio ya tiene que existir (no queremos crearlo).
		}

		iRet = WriteProxyHeader(pProxySpace->GetUniqueID(), ai_IOD);
		if (iRet != M4_SUCCESS) {
			return iRet;
		}
		
		iRet = AddDebugChannels(ai_objDir);
	}

	return iRet;
}

/**
 * Cuando se trata de un canal OLTP se llama a este método para fijar el modo de serialización del canal según su
 * tipo de sincronización (SyncType).
 * Este método sólo debe llamarse para canales OLTP o AUTO
 *
 * @param ai_pChannel	Canal al que vamos a fijar el modo de serialización según su tipo de sincronización.
 * @param ai_iBlock		Para cuando el canal está encarrilado, indica el bloque activo.
 * @param ai_iRegister	Para cuando el canal está encarrilado, indica el registro activo.
 * @param ai_hItem		Handle del ítem de ejecución.
 * @param ai_recordSet	RecordSet sobre el que se está ejecutnado.
 * @return m4return_t.
 */
m4return_t ClExecAdaptor::SetOLTPChannelSerializationMode(ClChannel* ai_pChannel, m4uint32_t ai_iBlock, m4uint32_t ai_iRegister, m4uint32_t ai_hItem, ClAccessRecordSet& ai_recordSet)
{
	ClChannel_Data*        pChannelData = ai_pChannel->GetpChannel_Data();
	ClBlock_Channel_Proxy* pBlockProxy  = 0;
	m4uint8_t iSyncType = pChannelData->GetpChannelDef()->GetpCMCR()->GetItemSyncType(ai_hItem);
	
	// Tenemos que comprobar el syncType para comprobar si hay que utilizar serializacion avanzada.
	// En todos los casos se respeta la marcacion realizada por el usuario.
	switch (iSyncType)
	{
	case M4_SYNC_NONE:
		// No tengo que hacer nada.
		// Serializacion de marcas donde no hay marcado ningun root ni ningun free.
		ai_pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_ADVANCED);
		break;
		
	case M4_SYNC_DOCUMENT:
		// Es una serializacion como la conocemos hasta el momento.
		ai_pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_ALL);
		break;
		
	case M4_SYNC_DOCUMENT_PERSIST:
		// Se envia solo lo que hay que persistir.
		ai_pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_ADVANCED);
		pChannelData->MarkForPersistSerial(ai_iBlock, ai_iRegister);
		break;
		
	case M4_SYNC_BLOCK:
		// Marcamos el bloque activo.
		ai_pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_ADVANCED);
		pBlockProxy = ai_recordSet.Current.GetCurrentBlock();
		pBlockProxy->MarkBlockAndParentRecordsForSerialize(M4_FALSE);
		break;
		
	case M4_SYNC_BLOCK_BRANCH:
		// Marcamos el bloque activo, y activamos el modo branch a partir de este.
		ai_pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_ADVANCED);
		pBlockProxy = ai_recordSet.Current.GetCurrentBlock();
		pBlockProxy->MarkBlockAndParentRecordsForSerialize(M4_TRUE);
		break;
		
	case M4_SYNC_REGISTER:
		{
			// Marcamos el registro activo.
			ai_pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_ADVANCED);
			ClRegister_Current& oRegCurrent = ai_recordSet.Current;
			pBlockProxy = oRegCurrent.GetCurrentBlock();
			pBlockProxy->MarkRecordAndParentRecordsForSerialize(oRegCurrent.Index, M4_FALSE);
			break;
		}
		
	case M4_SYNC_REGISTER_BRANCH:
		{
			// Marcamos el registro activo, y activamos el modo branch a partir de este.
			ai_pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_ADVANCED);
			ClRegister_Current& oRegCurrent = ai_recordSet.Current;
			pBlockProxy = oRegCurrent.GetCurrentBlock();
			pBlockProxy->MarkRecordAndParentRecordsForSerialize(oRegCurrent.Index, M4_TRUE);
			break;
		}
		
	default:
		M4_MSG_ASSERT(0, "Invalid syncType");
		return M4_ERROR;
	}
	
	// En cualquier caso mandamos sólo los ítems mapped.
	ai_pChannel->SetItemSerializationMode(M4CL_ITEM_SERIAL_MAPPED);

	return M4_SUCCESS;
}

/**
 * Escribe en el IODriver que se pasa como parámetro la cabecera de una transacción proxy. Esta se compone del comando
 * proxy a ejecutar (en ests caso siempre es la ejecución de un método) y el identificador del espacio proxy.
 *
 * @param pProxyID	Identificador del espacio proxy.
 * @param ai_IOD	IODriver donde se va ha hacer la escritura.
 * @return m4return_t.
 */
m4return_t ClExecAdaptor::WriteProxyHeader(m4pcchar_t pProxyID, ClGenericIODriver& ai_IOD)
{
	M4_ASSERT(pProxyID);

	// Writes the proxy command.
	m4int16_t ieCommand = (m4int16_t)ClProxyManager::PROXY_EXECUTE_COMMAND;
	m4return_t iRet = ai_IOD.Write(ieCommand);
	if (iRet != M4_SUCCESS) return iRet;
	
	// Writes the proxy space name.
	iRet = ai_IOD.Write(pProxyID);
	
	return iRet;
}

/**
 * Añade al objDir que se pasa como parámetro los canales proxy que hay en el ChannelManager asociado al ClExecAdaptor.
 *
 * @param ai_objDir objDir donde se quieren añadir los canales proxy que se encuentren en el ChannelManager.
 * @return m4return_t.
 */
m4return_t ClExecAdaptor::AddProxyChannels(ClCMObjectDirectory& ai_objDir)
{
	ClChannelDirectory& channelDirectory = m_pChannelManager->Channel;
	m4uint32_t iNChannels = channelDirectory.Count();
	ClChannel* pChannel = 0;
	m4uint8_t iCSExeType = (m4uint8_t)-1 ;

	for (m4uint32_t iCount = 0; iCount < iNChannels; iCount++)
	{
		pChannel = channelDirectory[iCount];

		/* Bug 0113035
		No se puede preguntar por el tipo sin saber si está construido o no
		*/
		if (!pChannel->IsBuilt()) {
			continue;
		}

		// Sólo busco los canales proxy.
		iCSExeType = pChannel->GetpChannelDef()->CsExeType() ;

		if (iCSExeType != M4CL_CSEXETYPE_PROXY) {
			continue;
		}

		// Tampoco me interesan los canales proxy de depuración.
		ClChannel::eDebugMode_t eDebugMode = pChannel->GetDebugMode();

		if (eDebugMode != ClChannel::IS_NOT_DEBUG) {
			continue;
		}

		m4double_t dCsType = 0;
		pChannel->CsType.Get(dCsType);

		if (dCsType == M4CL_CSTYPE_DEBUG) {
			continue;
		}

		// Finalmente añado el canal proxy al objDir.
		m4uint32_t iRemoteId = pChannel->GetRemoteId();

		if (ai_objDir.IncludeChannel(pChannel, iRemoteId) == -1) {
			M4_ASSERT(0);
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}

/**
 * Añade al objDir que se pasa como parámetro los canales de depuración que hay en el ChannelManager asociado al ClExecAdaptor.
 *
 * @param ai_objDir objDir donde se quieren añadir los canales de depuración que se encuentren en el ChannelManager.
 * @return m4return_t.
 */
m4return_t ClExecAdaptor::AddDebugChannels(ClCMObjectDirectory& ai_objDir)
{
	ClChannelDirectory& channelDirectory = m_pChannelManager->Channel;
	m4uint32_t iNChannels = channelDirectory.Count();
	m4uint16_t iObjectsInObjDir = ai_objDir.GetCount();
	m4int16_t i = 0;
	m4uint32_t iRemoteId = 0;
	ClChannel::eDebugMode_t eDebugMode = ClChannel::IS_NOT_DEBUG;
	ClChannel* pChannel = 0;
	m4return_t iRet = M4_SUCCESS;

	for (m4uint32_t iCount = 0; iCount < iNChannels; iCount++) {
		pChannel = channelDirectory[iCount];

		// Sólo quiero añadir los canales que se estan depurando.
		eDebugMode = pChannel->GetDebugMode();

		if (eDebugMode == ClChannel::IS_NOT_DEBUG || 
				eDebugMode == ClChannel::IS_DEBUG_FIRST_TIME ||
				eDebugMode == ClChannel::IS_DEBUG_VIEW)
		{
			continue;
		}

		// Finalmente añado el canal de depuración al objDir.
		iRemoteId = pChannel->GetRemoteId();

		// EFV
		// We just have to send channels with remote id
		// Session channels in emulation mode don't have such setting
		// Fix bug 0061405
		if (iRemoteId != 0)
		{
			i = ai_objDir.IncludeChannel(pChannel, iRemoteId);
			if (i == -1) {
				iRet = M4_ERROR;
				break;
			}

			/* Bug 0069768
			Como se permite que existan varios accesos de depuración para un canal
			desde la corrección del bug 0063324, hay que serializarlos todos,
			aunque el canal ya esté incluido. De lo contrario a la vuelta se crea
			uno nuevo en cliente.
			*/
			iRet = ai_objDir.IncludeAllAccessAndCreateOID(*pChannel, M4_FALSE);

			if (iRet != M4_SUCCESS) {
				break;
			}
		}
	}
	
	return iRet;
}

/**
 * Hace una llamada a un servidor remoto.
 * 
 * @param ai_hItem			Handle del ítem a ejecutar.
 * @param ai_poRecordSet	Recordset en el que está el ítem.
 * @return m4return_t
 */
m4return_t ClExecAdaptor::RemoteCall(ClExecutor* ai_poExecutor, m4uint32_t ai_hItem, ClAccessRecordSet* const ai_poRecordSet)
{
	if (ai_poExecutor == 0 || ai_hItem == 0 || ai_poRecordSet == 0) {
		return M4_ERROR;
	}

	m4bool_t					bCheckNotNull = M4_FALSE;
	ClChannelProcess_NullCheck*	pChanProc = NULL;
	m4uint32_t					iOutputSize = 0;
	m4int16_t					iIndex = -1;
	ClAccessBookMark            accessBookMark;
	ClHandle                    itemHandleWithError = 0;
	ClHandle                    nodeHandleWithError = 0;

	
	//miramos cuantos access/channel vamos a tener que llevarnos
	m4uint32_t iNumAccess = ai_poExecutor->GetNumAccessToSerialize();
	ClAccess   **ppoAccess = new ClAccess* [iNumAccess];
	
	//pillamos los access que nos vamos a tener que llevar
	m4return_t iRet = ai_poExecutor->GetAccessToSerialize(ppoAccess, iNumAccess);
	if (iRet != M4_SUCCESS) {
		delete [] ppoAccess;
		return iRet;
	}

	ClChannel* pChannel   = ppoAccess[0]->GetpChannel();
	m4uint8_t  iCSExeType = pChannel->GetpChannelDef()->CsExeType();
	m_pChannelManager     = pChannel->GetpChannelManager();

	// Creates a new object directory.
	ClCMObjectDirectory oDirectory (m_pChannelManager);

	// Create the OLTPExecutionContext.
	ClOLTPExecutionContext oOLTPExeCntxt(*m_pChannelManager, ppoAccess[0]);

	// We need to set the logon adaptor by getting it from the
	// manager of the current channel (we dont rely on it being a singleton)
	m_poClLogonAdaptor = m_pChannelManager->GetLogonAdaptor();
	
	M4DataStorage* poOutputDataStorage = M4DataStorage::GetNewDataStorage();
	M4DataStorage* poInputDataStorage  = M4DataStorage::GetNewDataStorage();
	
	poOutputDataStorage->Clear();
	
	ClDataStoreIODriver oDSIOD;
	
	iRet = oDSIOD.Init(GEN_IO_MODE_WRITE, poOutputDataStorage, GetLogonAdaptor()->GetServerArchitecture());
	if (iRet != M4_SUCCESS) {
		goto RETURN_ERROR;
	}
	
	iRet = PrepareRemoteCall(ppoAccess, iNumAccess, ai_hItem, *ai_poRecordSet, oDSIOD, oDirectory);
	if (iRet != M4_SUCCESS) {
		goto RETURN_ERROR;
	}

	// Añadimos el contexto de ejecución.
	iRet = (oDirectory.IncludeOLTPExecutionContextAndCreateOID(oOLTPExeCntxt) == -1) ? M4_ERROR : M4_SUCCESS;
	if (iRet != M4_SUCCESS) {
		goto RETURN_ERROR;
	}
	
	// Preparo los objetos para chequear el canal mientras se serializa.
	bCheckNotNull = ((m_pChannelManager->IsDCEnabled() == M4_TRUE) &&
					 (pChannel->GetpCMCR()->GetItemCheckNotNull(ai_hItem) & PROCCESS_MASK_CHECK_NULL) != 0) ? M4_TRUE : M4_FALSE;
	
	if (bCheckNotNull == M4_TRUE) {
		pChanProc = new ClChannelProcess_NullCheck;
		
		pChanProc->Init(pChannel, &accessBookMark);
		pChanProc->set_m_pChannelProcessNext(NULL);
		m_pChannelManager->SetDCData();
		
		// Busco el canal sobre el que hay que hacer el chequeo de nulos
		iIndex = oDirectory.GetObjectIndex(pChannel);
		M4_ASSERT(iIndex != -1);
		
		iRet = oDirectory.SetChannelProcess(iIndex, pChanProc);
		if (iRet != M4_SUCCESS) {
			goto RETURN_ERROR;
		}
	}
	
	iRet = oDirectory.Serialize(oDSIOD);

	if (iRet != M4_SUCCESS) {

#ifdef _DEBUG
		DUMP_CHLOG_ERROR(M4_CS_SERIALIZE_IN_CLIENT);
#else
		DUMP_CHLOG_DEBUG(M4_CS_SERIALIZE_IN_CLIENT);
#endif

		goto RETURN_ERROR;
	}
	
	iOutputSize = oDSIOD.GetActualOffset();
	
	oDSIOD.End();
	
	
	// C/S Statistics
	if (m_pChannelManager->GetSttWrapper()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED) {
		// Por ahora, en el futuro tenemos que pedir al nivel de transporte para
		// obtener el numero real de bytes enviados/recibisos. Caso extremo de error
		// cuando tenemos apuntadores a BLOBs ! ! ! CRG
		
		m_pChannelManager->GetSttWrapper()->GetStatistics()->sttAddOutputPDUSize(oDSIOD.GetActualOffset()/*, dSttOutputPDUSize*/);
		
		m4pcchar_t serviceId = GetServiceId(pChannel);
		m_pChannelManager->GetSttWrapper()->GetStatistics()->sttAddService((m4pchar_t)serviceId);
		
		m4char_t psDescription[8*1024];
		GetExecutionTransactionDescription(oDirectory, psDescription);
		
		m_pChannelManager->GetSttWrapper()->GetStatistics()->sttAddGeneralData(psDescription);
	}

	// Si no ha habido error en procesado de canal
	m_pChannelManager->SetDCData(); 
	
	m4pchar_t pzRoleId;
	pChannel->Role_ID.Get(pzRoleId);
	
	m_poCSTrace->PreMethodExecutionTrace(ai_poExecutor, &oDirectory, pzRoleId, M4_FALSE); // From Cache

	iRet = SyncRequest(poOutputDataStorage, poInputDataStorage, pChannel);
	if (iRet != M4_SUCCESS) {
		goto RETURN_ERROR;
	}

	iRet = oDSIOD.Init(GEN_IO_MODE_READ, poInputDataStorage, GetLogonAdaptor()->GetServerArchitecture());

	if (iRet != M4_SUCCESS)
	{
#ifdef _DEBUG
		DUMP_CHLOG_ERROR(M4_CS_DESERIALIZE_IN_CLIENT);
#else
		DUMP_CHLOG_DEBUG(M4_CS_DESERIALIZE_IN_CLIENT);
#endif
		goto RETURN_ERROR;
	}
	
	iRet = oDirectory.DeSerialize(oDSIOD, NULL, M4_TRUE);

	if (iRet != M4_SUCCESS)
	{
#ifdef _DEBUG
		DUMP_CHLOG_ERROR(M4_CS_DESERIALIZE_IN_CLIENT);
#else
		DUMP_CHLOG_DEBUG(M4_CS_DESERIALIZE_IN_CLIENT);
#endif
		goto RETURN_ERROR;
	}


	m_poCSTrace->PostMethodExecutionTrace(iOutputSize,
										  oDSIOD.GetActualOffset(),
										  M4ClCsExeType( iCSExeType )); 

	//oDSIOD.End();
	
	// C/S Statistics
	if (m_pChannelManager->GetSttWrapper()->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED) {
		m_pChannelManager->GetSttWrapper()->GetStatistics()->sttAddInputPDUSize(oDSIOD.GetActualOffset()/*, dSttInputPDUSize*/);
	}
	
RETURN_ERROR:	

	/* Bug 0067552
	Cuando el error de versión da en la deserialización de cliente
	también hay que hacer el control de errores
	*/
	// Una vez terminada la deserializacion, hacemos el tratamiento de errores
	m4return_t iRet2 = ProcessError(oDirectory);

	/* Bug 0105483
	Si hay un error hay que conservarlo, no puede ser que la process lo limpie
	*/
	if( iRet2 != M4_SUCCESS )
	{
		iRet = iRet2 ;
	}


	// Comprobamos si se ha encontrado un item no nulo.
	if( pChanProc != NULL )
	{
		if( pChanProc->ErrorConditionReached() == M4_TRUE )
		{
			itemHandleWithError = pChanProc->GetItemHandle();
			nodeHandleWithError = pChanProc->GetNodeHandle();
			m_pChannelManager->SetDCData( pChanProc->GetChannel(), pChanProc->GetAccessBookMark(), &nodeHandleWithError, &itemHandleWithError);
			
			DUMP_CHLOG_ERRORF(M4_CH_DS_PROCESS_CHECK_NULLS, M4ObjNodeItemI(pChanProc->GetMetaChannel(), itemHandleWithError));

			/* Bug 0103145
			Si da error de chequeo debe subirse para arriba
			*/
			iRet = M4_ERROR ;
		}

		delete( pChanProc ) ;
	}


	if (iRet == M4_ERROR) {
		// Si estamos en OLTP y se uso algun tipo de marca tengo que
		// destruir los deltas que he creado para enviar los registros validos
		// Para AUTOS y PROXY no hay que hacerlo 
		ClChannel_Data* poChannel_Data = pChannel->GetpChannel_Data();
		m4uint8_t iSyncType = poChannel_Data->GetpChannelDef()->GetpCMCR()->GetItemSyncType(ai_hItem);

		if ( iCSExeType == M4CL_CSEXETYPE_OLTP && iSyncType != M4_SYNC_DOCUMENT) {
			poChannel_Data->SetKeepStatus(M4_FALSE, 1);
		}
	}
		
	oDSIOD.End();

	// Releases memory.
	if (poInputDataStorage) {
		M4DataStorage::DeleteDataStorage(poInputDataStorage); 
	}
	if (poOutputDataStorage) {
		M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
	}
	delete [] ppoAccess;
	
	return iRet;
}

/**
 * Comprueba si ha habido error en la ejecucion remota. En caso afirmatvo toma las
 * medidas pertinentes (borrar la cache, cambiar la version del registro, ...).
 * Si estaba depurando el método remoto, se encarga de terminar el modo de depuración.
 *
 * @param ai_oDirectory	Directorio de objetos.
 * @return m4return_t - Dice si ha habido error en la ejecucion remota.
 */
m4return_t ClExecAdaptor::ProcessError(ClCMObjectDirectory& ai_oDirectory)
{
    m4return_t iRet = M4_SUCCESS;

    // Buscamos si se ha producido algun error en el server

    m4int16_t iIndex = ai_oDirectory.GetFirstObjectIndex(ClientServerResultObjectType);
    if (iIndex > -1) {
        ClClientServerResult* pCSResult = (ClClientServerResult*)ai_oDirectory.GetObjectAddress(iIndex);

		if( pCSResult != NULL )
		{
			m4uint16_t i = 0;
			m4pcchar_t pccErrMessage = 0;
			m4uint16_t iIndex = 0;
			m4VariantType oVar;
			oVar.Type = M4CL_CPP_TYPE_STRING_VAR;
			oVar.Data.PointerChar = 0;
			ClRecordVersionProperty oRecVerProp;
			m4uint32_t iRecordId = 0;
			m4date_t dNewRecVer = 0;
			ClChannel* pChannel = 0;

			switch (pCSResult->GetResponseCode())
			{
			case M4_CSR_ERROR_CHANNEL_VERSION:
				// Hay que invalidar la cache
				while (pccErrMessage = pCSResult->GetMess(i++)) {
					iIndex = ai_oDirectory.GetFirstObjectIndex(ChannelObjectType, pccErrMessage);
					M4_ASSERT(iIndex != -1);
                
					/* Bug 0067552
					Cuando el error de versión da en la deserialización de cliente
					no se tiene el canal construido y hay que tomar el id de canal
					del mensaje de error
					*/
					DUMP_CHLOG_ERRORF(M4_CS_OUTDATED_MCR_VERSION, pccErrMessage);
                
					/* jcr Bug 0068735
					Hay que borrar todas las caches asociadas con este metadato,
					no sólo los metadatos cliente.
					*/
					m_pChannelManager->GetCacheDirectory()->RemoveObjectById( M4_CACHE_ALL_CACHE_TYPE, (m4pchar_t) pccErrMessage, M4_FALSE ) ;
				}
				break;
            
			case M4_CSR_ERROR_CONCURRENCY:
				// Se ha producido un error de concurrencia,
				// hay que cambiarle la version al registro
				while (pccErrMessage = pCSResult->GetMess(i++)) {
					iIndex = atoi(pccErrMessage);
                
					pccErrMessage = pCSResult->GetMess(i++);
					M4_ASSERT(pccErrMessage);
					oVar.Data.PointerChar = (m4char_t*)pccErrMessage;
					oRecVerProp.Set(oVar);
					oRecVerProp.Get(iRecordId, dNewRecVer);

					pChannel = (ClChannel*)ai_oDirectory.GetObjectAddress(iIndex);
					pChannel->GetpChannel_Data()->Node.FindRecordAndSetVersion(iRecordId, dNewRecVer);
				}
				break;
            
			case M4_CSR_ERROR_NO_CHANNEL:
			case M4_CSR_ERROR_NO_EXECONTEXT:
			case M4_CSR_ERROR_EXECUTE:
			case M4_CSR_ERROR_DESERIALIZE:
				// El error se metio en la log en el lado server
				break;

			case M4_CSR_EXEC_INTERRUPT:
				return M4CL_EXECUTION_INTERRUPTION;
			}
        
			iRet = M4_ERROR;
		}
    }
	
	if (EndDebugMethod() != M4_SUCCESS) {
		iRet = M4_ERROR;
	}

    return iRet;
}

m4return_t ClExecAdaptor::EndDebugMethod()
{
	m4return_t iRet = M4_SUCCESS;

	switch (m_eIsDebug) {
	case IS_NOT_DEBUG:
		break;
		
	case IS_DEBUG_OLTP:
		{
			IProxyManager* pProxyManager = m_pChannelManager->GetProxyManager();
			ClProxySpace* pProxySpace = pProxyManager->GetSpace("OLTPD", M4_FALSE);

			iRet = pProxySpace->EndDebugMethod();
			iRet = pProxyManager->ReleaseSpace(pProxySpace);
			
			m_eIsDebug = IS_NOT_DEBUG;
			break;
		}
		
	case IS_DEBUG_PROXY:
		{
			IProxyManager* pProxyManager = m_pChannelManager->GetProxyManager();
			ClProxySpace* pProxySpace = pProxyManager->GetSpace("Proxy", M4_FALSE);

			iRet = pProxySpace->EndDebugMethod();
			
			m_eIsDebug = IS_NOT_DEBUG;
			break;
		}
		
	case IS_DEBUG_OLTP_REENTER:
	case IS_DEBUG_PROXY_REENTER:
		m_eIsDebug = ClExecAdaptor::eDebugMode_t(m_eIsDebug - 1);	// He terminado de ejecutar la reentrada. Vuelvo al estado de depuración correspondiente.
		break;

	default:
		M4_ASSERT(0);
		iRet = M4_ERROR;
	}

	return iRet;
}

ClLogonAdaptor* ClExecAdaptor::GetLogonAdaptor(void)
{
   return m_poClLogonAdaptor;
}

m4pcchar_t ClExecAdaptor::GetServiceId(ClChannel* ai_poChannel)
{
	if (m_poClLogonAdaptor->GetCurrentState() != M4CL_LOG_ONLINE &&
		strcmpi(ai_poChannel->GetpChannel_Data()->GetpChannelDef()->Id(), M4_SESSION_CHANNEL_ID) == 0) 
	{
		// si no estamos logados y somos el canal sesión va por el especial
		return M4CL_SESSION_SERVICE;
	}

	if (ai_poChannel->IsDebugEnabled()){
		return M4CL_PROXY_SERVICE;
	}

	return ai_poChannel->GetpCMCR()->GetChannelServiceId();
}


m4return_t ClExecAdaptor::SyncRequest(M4DataStorage* ai_poOutputDataStorage,
                                      M4DataStorage*& ao_poInputDataStorage,
                                      ClChannel* ai_poChannel)
{
	m4return_t RetVal = M4_ERROR;

	if (m_poClientAPI) {
		m4handle_t hRequestId = 0;
		m4pchar_t serviceId = (m4pchar_t) GetServiceId(ai_poChannel);

		RetVal = m_poClientAPI->SyncRequest(
			serviceId, 0, 0, ai_poOutputDataStorage, 
			ao_poInputDataStorage, hRequestId, 
			m_poRegistry->GetNextCSTimeOut()
			);
		m_poRegistry->ResetNextCSTimeOut();
	}
	
	return RetVal;
}

void ClExecAdaptor::GetExecutionTransactionDescription(ClCMObjectDirectory& ai_oObjectdir,
                                                       m4pchar_t            ao_psDescriptionString)
{
    // Get the information from the context to set a description
    
    ao_psDescriptionString[0] = 0; // Null description...
    
    m4uint16_t iNumChannels = ai_oObjectdir.GetCountByType(ChannelObjectType);
    
    if (iNumChannels > 1)
    {
        ClObjectId* poObjectId;

        m4int16_t iIndex;
        m4int16_t iCount;
        
        for (iIndex = ai_oObjectdir.GetFirstObjectIndex(ChannelObjectType), iCount = 0;
             iIndex != -1;
             iIndex = ai_oObjectdir.GetNextObjectIndex(), iCount++)
        {
            if (ai_oObjectdir.GetObjectId(iIndex, poObjectId) == M4_SUCCESS)
            {
                if (poObjectId != NULL)
                {
                    strcat(ao_psDescriptionString, poObjectId->GetTypedId());
                    
                    if (iCount < iNumChannels-1)
                        strcat(ao_psDescriptionString, ",");
                }
            }
        }
    }
}
