#ifndef _IEXEADAPTOR_HPP_
#define _IEXEADAPTOR_HPP_


#include "m4types.hpp"

class ClCsTrace;
class ClClientAPI;
class ClM4ObjServiceContext;
class ClAppClientAdap;
class ClExecutor;
/**
 * Esta clase define la interfaz para hacer la ejecución remota de un método de un canal.
 * No importa que el canal sea OLTP (no se guarda estado en el servidor) o
 * Proxy (se guarda el estado del canal en el servidor, para futuras ejecuciones remotas).
 */
class IExecAdaptor
{
public:
	/** Destructor virtual. Permito derivaciones. */
	virtual ~IExecAdaptor() {}

	virtual m4return_t Init(ClCsTrace *ai_poCSTrace, ClAppClientAdap* ai_poClientAPI, ClVMBaseEnv* ai_poRegistry) = 0;

	/**
	 * Hace una llamada a un servidor remoto.
	 * 
	 * @param ai_poExecutor     Caller
	 * @param ai_hItem			Handle del ítem a ejecutar.
	 * @param ai_poRecordSet	Recordset en el que está el ítem.
	 * @return m4return_t
	 */
	virtual m4return_t RemoteCall(ClExecutor *ai_poExecutor, m4uint32_t ai_hItem, ClAccessRecordSet* const ai_poRecordSet) = 0;

	/**
	 * Si estaba depurando la ejecución del método, se sincronizan los canales cliente con los del servidor.
	 * En caso de estar depurando un método OLTP se cierra el espacio proxy que se creo para la depuración.
	 *
	 * @return m4return_t.
	 */
	virtual m4return_t EndDebugMethod() = 0;

	/** Resetea la conexión lógica del Environment. Función implementada sólo en ClExecAdaptorSM. */
	virtual void ResetLConn(void) = 0;

protected:
	/** Construye un nuevo IExecAdaptor. Esto es sólo una interfaz. No permitimos instancias */
	IExecAdaptor() {}
};


#endif  // #ifndef _IEXEADAPTOR_HPP_

