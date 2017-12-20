//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clexadap.hpp   
// Project:             Mind3x      
// Author:              Meta Software M.S. , S.A
// Date:                19/02/1997
// Language:            C++
//
// Definition:
//    This module declares the interface to the Execution Adaptor
//
//==============================================================================

// Esta definición es común para Client (DIC y SM)

#ifndef _CLEXADAP_HPP_
#define _CLEXADAP_HPP_

#include "m4types.hpp"
#include "cllgadap.hpp"
#include "bookmark.hpp"
#include "iexecadaptor.hpp"

class ClItem;
class ClClientAPI;
class M4DataStorage;
class ClChannel;
class ClCMObjectDirectory;
class ClCsTrace;
class ClVMBaseEnv;

/**
 * Adaptador para hacer una ejecución, de un método de un canal, en un servidor remoto.
 */
class ClExecAdaptor : public IExecAdaptor
{
public:
	/** Construye un nuevo ClExecAdaptor */
	ClExecAdaptor();
	
	/** Destructor virtual. Permito derivaciones. */
	virtual ~ClExecAdaptor();

	virtual m4return_t Init(ClCsTrace *ai_poCSTrace, ClAppClientAdap* ai_poClientAPI, ClVMBaseEnv *ai_poRegistry);
	
	/**
	 * Hace una llamada a un servidor remoto.
	 * 
	 * @param ai_hItem			Handle del ítem a ejecutar.
	 * @param ai_poRecordSet	Recordset en el que está el ítem.
	 * @return m4return_t
	 */
	virtual m4return_t RemoteCall(ClExecutor *ai_poExecutor, m4uint32_t ai_hItem, ClAccessRecordSet* const ai_poRecordSet);
	
	/** Resetea la conexión lógica del Environment. Función implementada sólo en ClExecAdaptorSM. */
	virtual void ResetLConn(void) {}

	/**
	 * Si estaba depurando la ejecución del método, se sincronizan los canales cliente con los del servidor.
	 * En caso de estar depurando un método OLTP se cierra el espacio proxy que se creo para la depuración.
	 *
	 * @return m4return_t.
	 */
	virtual m4return_t EndDebugMethod();

protected:
	ClAppClientAdap* m_poClientAPI;

	ClLogonAdaptor* m_poClLogonAdaptor;

	ClCsTrace* m_poCSTrace;
	
	ClVMBaseEnv* m_poRegistry;

	ClChannelManager* m_pChannelManager;

	/**
	 * Posibles valores para el modo de depuración:
	 * IS_NOT_DEBUG		Estamos ejecutando un método que no se puede depurar.
	 * IS_DEBUG_OLTP	Estamos ejecutando un método OLTP que se puede depurar.
	 * IS_DEBUG_PROXY	Estamos ejecutando un método Proxy que se puede depurar.
	 */
	enum eDebugMode_t { IS_NOT_DEBUG = 0, IS_DEBUG_OLTP, IS_DEBUG_OLTP_REENTER, IS_DEBUG_PROXY, IS_DEBUG_PROXY_REENTER };

	/**
	 * Indica si estamos ejecutando un método remoto en modo de depuracíón. Nos va a servir para
	 * detectar cuando se hace la última transacción y se termina la ejecución del método.
	 */
	eDebugMode_t m_eIsDebug;



	m4pcchar_t GetServiceId(ClChannel* ai_poChannel);
	
	void GetExecutionTransactionDescription(ClCMObjectDirectory& ai_objDir, m4pchar_t ao_strDescription);
	
	virtual m4return_t SyncRequest(M4DataStorage* ai_pOutDataStorage, M4DataStorage*& ao_pInDataStorage, ClChannel* ai_pChannel);
	
	ClLogonAdaptor* GetLogonAdaptor(void);

	/**
	 * Comprueba si ha habido error en la ejecucion remota. En caso afirmatvo toma las
	 * medidas pertinentes (borrar la cache, cambiar la version del registro, ...).
	 * Si estaba depurando el método remoto, se encarga de terminar el modo de depuración.
	 *
	 * @param ai_oDirectory	Directorio de objetos.
	 * @return m4return_t - Dice si ha habido error en la ejecucion remota.
	 */
    m4return_t ProcessError(ClCMObjectDirectory& ai_oDirectory);

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
	m4return_t PrepareRemoteCall(ClAccess** ai_ppAccess, m4uint32_t ai_iNumAccess, m4uint32_t ai_hItem, ClAccessRecordSet& ai_recordSet, ClGenericIODriver& ai_IOD, ClCMObjectDirectory& ai_objDir);

	/**
	 * Cuando se trata de un canal OLTP se llama a este método para fijar el modo de serialización del canal según su
	 * tipo de sincronización (SyncType).
	 * Este método sólo debe llamarse para canales OLTP.
	 *
	 * @param ai_pChannel	Canal al que vamos a fijar el modo de serialización según su tipo de sincronización.
	 * @param ai_iBlock		Para cuando el canal está encarrilado, indica el bloque activo.
	 * @param ai_iRegister	Para cuando el canal está encarrilado, indica el registro activo.
	 * @param ai_hItem		Handle del ítem de ejecución.
	 * @param ai_recordSet	RecordSet sobre el que se está ejecutnado.
	 * @return m4return_t.
	 */
	m4return_t SetOLTPChannelSerializationMode(ClChannel* ai_pChannel, m4uint32_t ai_iBlock, m4uint32_t ai_iRegister, m4uint32_t ai_hItem, ClAccessRecordSet& ai_recordSet);

	//m4return_t PrepareProxyRemoteCall( m4uint8_t ai_csExeType, m4char_t* pProxyID, ClGenericIODriver& ai_IOD, ClCMObjectDirectory& ai_objDir);

	/**
	 * Escribe en el IODriver que se pasa como parámetro la cabecera de una transacción proxy. Esta se compone del comando
	 * proxy a ejecutar (en ests caso siempre es la ejecución de un método) y el identificador del espacio proxy.
	 *
	 * @param pProxyID	Identificador del espacio proxy.
	 * @param ai_IOD	IODriver donde se va ha hacer la escritura.
	 * @return m4return_t.
	 */
	m4return_t WriteProxyHeader(m4pcchar_t pProxyID, ClGenericIODriver& ai_IOD);

	/**
	 * Añade al objDir que se pasa como parámetro los canales proxy que hay en el ChannelManager asociado al ClExecAdaptor.
	 *
	 * @param ai_objDir objDir donde se quieren añadir los canales proxy que se encuentren en el ChannelManager.
	 * @return m4return_t.
	 */
	m4return_t AddProxyChannels(ClCMObjectDirectory& ai_objDir);

	/**
	 * Añade al objDir que se pasa como parámetro los canales de depuración que hay en el ChannelManager asociado al ClExecAdaptor.
	 *
	 * @param ai_objDir objDir donde se quieren añadir los canales de depuración que se encuentren en el ChannelManager.
	 * @return m4return_t.
	 */
	m4return_t AddDebugChannels(ClCMObjectDirectory& ai_objDir);
};

#endif  // _CLEXADAP_HPP_
