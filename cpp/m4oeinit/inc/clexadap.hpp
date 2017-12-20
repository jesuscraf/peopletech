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

// Esta definici�n es com�n para Client (DIC y SM)

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
 * Adaptador para hacer una ejecuci�n, de un m�todo de un canal, en un servidor remoto.
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
	 * @param ai_hItem			Handle del �tem a ejecutar.
	 * @param ai_poRecordSet	Recordset en el que est� el �tem.
	 * @return m4return_t
	 */
	virtual m4return_t RemoteCall(ClExecutor *ai_poExecutor, m4uint32_t ai_hItem, ClAccessRecordSet* const ai_poRecordSet);
	
	/** Resetea la conexi�n l�gica del Environment. Funci�n implementada s�lo en ClExecAdaptorSM. */
	virtual void ResetLConn(void) {}

	/**
	 * Si estaba depurando la ejecuci�n del m�todo, se sincronizan los canales cliente con los del servidor.
	 * En caso de estar depurando un m�todo OLTP se cierra el espacio proxy que se creo para la depuraci�n.
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
	 * Posibles valores para el modo de depuraci�n:
	 * IS_NOT_DEBUG		Estamos ejecutando un m�todo que no se puede depurar.
	 * IS_DEBUG_OLTP	Estamos ejecutando un m�todo OLTP que se puede depurar.
	 * IS_DEBUG_PROXY	Estamos ejecutando un m�todo Proxy que se puede depurar.
	 */
	enum eDebugMode_t { IS_NOT_DEBUG = 0, IS_DEBUG_OLTP, IS_DEBUG_OLTP_REENTER, IS_DEBUG_PROXY, IS_DEBUG_PROXY_REENTER };

	/**
	 * Indica si estamos ejecutando un m�todo remoto en modo de depurac��n. Nos va a servir para
	 * detectar cuando se hace la �ltima transacci�n y se termina la ejecuci�n del m�todo.
	 */
	eDebugMode_t m_eIsDebug;



	m4pcchar_t GetServiceId(ClChannel* ai_poChannel);
	
	void GetExecutionTransactionDescription(ClCMObjectDirectory& ai_objDir, m4pchar_t ao_strDescription);
	
	virtual m4return_t SyncRequest(M4DataStorage* ai_pOutDataStorage, M4DataStorage*& ao_pInDataStorage, ClChannel* ai_pChannel);
	
	ClLogonAdaptor* GetLogonAdaptor(void);

	/**
	 * Comprueba si ha habido error en la ejecucion remota. En caso afirmatvo toma las
	 * medidas pertinentes (borrar la cache, cambiar la version del registro, ...).
	 * Si estaba depurando el m�todo remoto, se encarga de terminar el modo de depuraci�n.
	 *
	 * @param ai_oDirectory	Directorio de objetos.
	 * @return m4return_t - Dice si ha habido error en la ejecucion remota.
	 */
    m4return_t ProcessError(ClCMObjectDirectory& ai_oDirectory);

	/**
	 * Se encarga de a�adir al objDir los canales y accesos necesarios para la transacci�n. Estos ser�n
	 * los que ha indicado el ejecutor m�s los canales proxy o de depuraci�n.
	 * Tambi�n se encarga de comprobar si los canales est�n encarrilados y de activar la serializaci�n 
	 * avanzada en el caso de que el canal que ha disparado la ejecuci�n sea OLTP y lo requiera.
	 *
	 * @param ai_ppAccess		Array de punteros a los accesos que hay que a�adir al objDir.
	 * @param ai_iNumAccess		N�mero de accesos que hay en el array ai_ppAccess.
	 * @param ai_hItem			Handle del �tem de ejecuci�n.
	 * @param ai_recordSet		RecordSet sobre el que se est� ejecutnado.
	 * @param ai_IOD			IODriver donde queremos hacer la serializacion.
	 * @param ai_objDir			ObjDir donde queremos que se a�adan los objetos.
	 * @return m4return_t.
	 */
	m4return_t PrepareRemoteCall(ClAccess** ai_ppAccess, m4uint32_t ai_iNumAccess, m4uint32_t ai_hItem, ClAccessRecordSet& ai_recordSet, ClGenericIODriver& ai_IOD, ClCMObjectDirectory& ai_objDir);

	/**
	 * Cuando se trata de un canal OLTP se llama a este m�todo para fijar el modo de serializaci�n del canal seg�n su
	 * tipo de sincronizaci�n (SyncType).
	 * Este m�todo s�lo debe llamarse para canales OLTP.
	 *
	 * @param ai_pChannel	Canal al que vamos a fijar el modo de serializaci�n seg�n su tipo de sincronizaci�n.
	 * @param ai_iBlock		Para cuando el canal est� encarrilado, indica el bloque activo.
	 * @param ai_iRegister	Para cuando el canal est� encarrilado, indica el registro activo.
	 * @param ai_hItem		Handle del �tem de ejecuci�n.
	 * @param ai_recordSet	RecordSet sobre el que se est� ejecutnado.
	 * @return m4return_t.
	 */
	m4return_t SetOLTPChannelSerializationMode(ClChannel* ai_pChannel, m4uint32_t ai_iBlock, m4uint32_t ai_iRegister, m4uint32_t ai_hItem, ClAccessRecordSet& ai_recordSet);

	//m4return_t PrepareProxyRemoteCall( m4uint8_t ai_csExeType, m4char_t* pProxyID, ClGenericIODriver& ai_IOD, ClCMObjectDirectory& ai_objDir);

	/**
	 * Escribe en el IODriver que se pasa como par�metro la cabecera de una transacci�n proxy. Esta se compone del comando
	 * proxy a ejecutar (en ests caso siempre es la ejecuci�n de un m�todo) y el identificador del espacio proxy.
	 *
	 * @param pProxyID	Identificador del espacio proxy.
	 * @param ai_IOD	IODriver donde se va ha hacer la escritura.
	 * @return m4return_t.
	 */
	m4return_t WriteProxyHeader(m4pcchar_t pProxyID, ClGenericIODriver& ai_IOD);

	/**
	 * A�ade al objDir que se pasa como par�metro los canales proxy que hay en el ChannelManager asociado al ClExecAdaptor.
	 *
	 * @param ai_objDir objDir donde se quieren a�adir los canales proxy que se encuentren en el ChannelManager.
	 * @return m4return_t.
	 */
	m4return_t AddProxyChannels(ClCMObjectDirectory& ai_objDir);

	/**
	 * A�ade al objDir que se pasa como par�metro los canales de depuraci�n que hay en el ChannelManager asociado al ClExecAdaptor.
	 *
	 * @param ai_objDir objDir donde se quieren a�adir los canales de depuraci�n que se encuentren en el ChannelManager.
	 * @return m4return_t.
	 */
	m4return_t AddDebugChannels(ClCMObjectDirectory& ai_objDir);
};

#endif  // _CLEXADAP_HPP_
