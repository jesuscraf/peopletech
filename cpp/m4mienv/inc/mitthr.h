/*

Diseño
---------------------------------------------------------------------------------------------

Nomenclatura:

TL. Prefijo para alcance Thread local.
TLC .Prefijo para alcance Thread local for component (subdivision de un TL).
Insp. Inspector. Compuesto de un objeto traza y otro memprofiler.


- 1 Process. Un objeto global PManager por process.
	
	1 Process	 - n ThreadId  
				 - 1 PM (Process manager). Tracea la creacion/destruccion de Threads. 

- 1 ThreadId. Accesible PManager.ThreadId.

	1 ThreadId   - n TL_TaskId (se mantiene una lista en ThreadId de TL_TaskId-RootCompId-
						OwnerTL_TaskId. Owner = ID_NULL si es una RootTask).
				 - n CompId 

   Al tener cada thread su gestion, no hay que usar semaforos ni preocuparse por bloqueos.

- 1 TL_TaskId. Accesible PManager.ThreadId.TL_TaskId.

	1 TL_TaskId  - n TLC_TaskId.  

- 1 TLC_TaskId. Accesible PManager.ThreadId.CompId.TL_TaskId.

	1 TLC_TaskId - 1 TLC_TaskEnv (gestionado via callbacks por cada Comp)
				 - 1 TLC_TaskInsp propio. No se admiten links a otros objetos porque el caso 
						de multiples Comp padres seria muy costoso de tratar.

- 1 CompId. Accesible PManager.ThreadId.CompId. En parte sale de componer otras relaciones.

	1 CompId	 - n TL_TaskId.
				 - 1 TLC_ThreadEnv (gestionado via callbacks por cada Comp)

  -Cualquier funcion de una DLL que use clases de este componente debe crear previamente un 
	TL_TaskId donde ejecutar las acciones.
  -Desde cualquier Comp se pueden crear cuantas tareas se quieran en otro Comp (sea este un
	Comp hijo o no). El sistema le sigue la pista a quien posea la tarea. 
  -Como se ha indicado, una TL_TaskId abarca varias TLC_Task, compartiendo todas un mismo 
	identificador TL_TaskId. 
  -Cada TLC_Task puede necesitar de un arbol concreto de  TLC_Task hijas directas o indirectas, 
    variable y distinto entre distintas TL_Task. Puede formarse tanto un ARBOL como un GRAFO.
	Evidentemente, lo que no debe de darse son inclusiones recursivas. En el caso de los 
	grafos, nunca se crean mas de un TLC_ThreadEnv o TLC_TaskEnv. Ademas el TLC_TaskType debe
	ser el mismo en todas las inclusiones.
  -TLC_TaskType. Las variaciones de tipos de arboles de TLC_Task que precise una entrada en 
	una DLL, y el tipo de datos que se almacenen en la TLC_TaskEnv, determinan distintos 
	tipos de TLC_TaskType. 
  -Cualquier punto de entrada de una DLL crea/accede/destruye una TL_Task. Es la minima unidad
	de comunicacion entre DLLs.
  -Un Inspector contabiliza al momento los errores de su propio componente. Ademas, cualquier
	mensaje de error y warning tambien es enviado y contabilizado en el resto de componentes
	que participan en la TL_TaskId en cuestion (en unos contadores aparte para errores y 
	warnings producidos en componentes externos).
  -Reiterando: un componente SOLO tiene acceso a su propio inspector. Esto quiere decir que 
	solo puede usar grupos de traza y memoria creados por dicho componente. El objeto SysInfo
	tiene añadidos automaticamente un par de claves extra que si pueden usarse.
*/

#include "m4stl.hpp"
#include "m4ipcs.hpp"

class ClMIT_PM ;	
class ClMIT_TM ;	
class ClMIT_TCM ;
struct StMIT_CompPlugIn ;
class ClMIT_SysInfo ;

//StMIT_Inspector
//============================================================================================

//NOTA: hay unas pocas clases que intervienen en la creacion del Inspector. En estas clases
//	solo es valido que accedan al Inspector, pero no a nada mas. 
//Esto se comentara explicitamente en cada clase con el comentario de 
//	ACCESO LIMITADO A INSPECTOR
//Ademas, hay que tener cuidado con el orden en que se crean los tres objetos principales,
//	Este orden es:
//	1) ClMIT_MemProf (Resto de objetos pueden alocar cosas).	
//	2) ClMIT_SysInfo (Trace lee parametros del registry).
//	3) ClMIT_Trace   (Por fin el *&!* objeto de traza)


struct StMIT_InspEnv {	//Environment especial para Insp

	//--->Propietaria:ClMIT_File (para comparar ficheros)

	enum { ClMIT_File_COMPARE_SIZE=512*50 } ;	//512 es un tamaño usual de sector
	m4char_t ClMIT_File_pcBuffA[ClMIT_File_COMPARE_SIZE],  
		ClMIT_File_pcBuffB[ClMIT_File_COMPARE_SIZE];

	//--->Propietaria:ClMIT_Trace

	enum { ClMIT_Trace_SIZE_BUFFER=13000, ClMIT_Trace_SIZE_MOVE_AUX=2000 } ;
	m4char_t ClMIT_Trace_pcBuffAux[ClMIT_Trace_SIZE_BUFFER] ;
	m4char_t ClMIT_Trace_pcBuffAux2[ClMIT_Trace_SIZE_BUFFER] ;
	m4char_t ClMIT_Trace_pcBuffAux3[ClMIT_Trace_SIZE_BUFFER] ;
	m4char_t ClMIT_Trace_pcTraceMoveAux[ClMIT_Trace_SIZE_MOVE_AUX] ;	

} ;

struct StMIT_Inspector		//Mero contenedor, la creacion/asignacion no se gestiona.
{
	//SysInfo seria mas correcto que estuviera a nivel de PM, pero para una clase tan pequeña
	//	la complicacion de los semaforos y del codigo no merece la pena.
	ClMIT_SysInfo *m_poSysInfo ;	
	ClMIT_Trace   *m_poTrace ;
	ClMIT_MemProf *m_poMemProf ;
	StMIT_InspEnv *m_poEnv ;
	MIT_Id_t       m_TaskId ;	//TaskId propietaria (NULL_ID para el PInsp) 
	MIT_Id_t	   m_CompId ;	//CompId propietario (NULL_ID para el PInsp)	
} ;

//Process manager
//============================================================================================

class M4_DECL_M4MIENV ClMIT_PM	
{

	//  SOLO DEBE HABER UN OBJETO GLOBAL ESTATICO DE ESTE TIPO EN UN PROCESO.

	// ESTA ES LA UNICA CLASE QUE DEBE USAR SEMAFOROS. 
	
	friend class ClMIT_TM;

MIT_PRIVATE:

	//----------------------------------->Lista de Threads Manager (1 por Thread)

	//Lista del TLS, ordenada por el m4iden_t correspondiente.

	typedef map < m4iden_t, ClMIT_TM *, less<m4iden_t> > ThreadList_t ;
	ThreadList_t m_oTList ;	

	//----------------------------------->Inspector del PM

	StMIT_InspEnv m_oInspEnv ;
	StMIT_Inspector m_stInsp ;

	//----------------------------------->TaskCount

	// Sirve para asignar los TaskId de forma secuencial, independientemente del thread en que
	//	se este (y de rebote, los nombre de ficheros de traza cumpliran lo mismo).

	m4uint32_t m_uiTasksRequested ;

	//Maximo numero de threads que han estado al mismo tiempo en RAM. Para estadistica

	m4uint32_t m_uiMaxThreads ;	

MIT_PUBLIC:

	// NOTAS SOBRE SECCIONES CRITICAS:

	// A) SpawnTask() y KillTask() son tareas excluyentes entre si. Solo un thread esta a 
	//	la vez haciendo alguna de estas dos tareas. Son las unicas tareas que no son 
	//	'solo lectura', es decir, pueden alterar el objeto.
	// B) GetInsp(). Puede ejecutarse concurrentemente con el grupo A. Ademas, dentro de la 
	//	ejecucion del SpawnTask() o KillTask() del thread que esta en A, puede llamarse a 
	//	GetInsp(), pudiendo este devolver el Insp de la Task o el PInsp.
	//	Al ejecutarse concurrentemente con A, hay que proteger el acceso a listas, etc, que
	//	pueda estar actualizando A.
	// C) GetTM(). Exactamente igual al grupo B), solo que NO es valido ejecutar este metodo
	//	dentro del Thread que tiene el semaforo de A, aunque si en el resto.

	
	//----------------------------------->Inicializaciones

	// Esta clase debe ser implementada como un objeto global estatico. Por ello, el constructor
	//	y destructor se invocan al levantarse y bajarse la DLL, con lo cual ya no hay threads
	//	en ejecucion. Es, pues, una ejecucion MonoThread.
	// El constructor, entre otras cosas, crea el PInsp.
	// El destructor generara un error fatal si detecta que quedan TM sin cerrar. En este
	//	caso, saca en la traza primero los threads pendientes de borrar.

	ClMIT_PM() ;	
	~ClMIT_PM() ;	

	
	//----------------------------------->Acceso regulado con semaforos. Uso general

	// INTENTAR MINIMIZAR ACCESOS A GetInsp, ETC: USAN SEMAFOROS; PUEDEN RALENTIZAR

	// Deben de tener exito, o error immediato.
	// Hacer un SpawnTask no significa cambiarse a esa tarea. Si eso es lo que se quiere,
	//	hay que llamar al metodo oportuno explicitamente.
	// Asi pues, el prototipo de funcion interfaz para crear tareas es:
	//
	//	MIT_Id_t SpawnSomething(...) {
	//		MIT_Id_t NewTask, PrevT ;
	//		NewTask=SPAWN_TASK(PlugIn) ;
	//		ClMIT_TM *TM = GET_TM() ; 	
	//
	//		TM->InTask(NewTask,&PrevT) ;
	//		{
	//			//Inicializaciones, etc. Insp, ThreadEnv y TaskEnv ya disponibles. 
	//			//Pueden llamarse a otras funciones interfaz de otras Task.
	//		} ;
	//		TM->OutTask(PrevT) ;
	//
	//		return NewTask ;
	//  } 

	MIT_Id_t SpawnTask( StMIT_CompPlugIn *ai_pstPlugIn) ;

	// Deben de tener exito, o error immediato.
	// Si detecta que la tarea borrada es la tarea actual, pondra la tarea actual a NULL.

	// Asi pues, el prototipo de funcion interfaz para destruir tareas es:
	//
	//	void KillSomething(MIT_Id_t ai_TaskId,...) {
	//		MIT_Id_t PrevT ;
	//		ClMIT_TM *TM = GET_TM() ; 	
	//
	//		TM->InTask(ai_TaskId,&PrevT) ;
	//		{
	//			//Tareas de limpieza. Insp, ThreadEnv y TaskEnv disponibles. 
	//			//Pueden llamarse a otras funciones interfaz de otras Task.
	//		} ;
	//		TM->OutTask(PrevT) ;
	//		
	//		KILL_TASK(ai_TaskId) ;
	//  } 

	void KillTask( MIT_Id_t ai_TaskId ) ;

	// Deben de tener exito, o error immediato.
	// El ai_CompId DEBE de ser OBLIGATORIAMENTE el CompId desde donde es llamado, ya que un
	//	Comp solo debe tener acceso a su propio Insp. Es una buena idea en cada componente
	//	crear una macro para GetInsp que siempre llame con el CompId en cuestion.
	// Como ya se comento, en los objetos con acceso limitado a inspector, NO ES VALIDO
	//	LLAMAR A GetTM. De hecho, se produce un error immediato en esta situacion.

	StMIT_Inspector * GetInsp(MIT_Id_t ai_CompId) ;
	ClMIT_TM		* GetTM() ;

MIT_PRIVATE:

	//----------------------------------->Metodos privados

	// Fix sirve para que GetInsp de incondicionalmente un PInsp, mientras que Undo vuelve
	//	a fijar el comportamiento normal.
	// Se usan dentro del contructor, el destructor, y de Spawn y Kill. Como es posible ejecutar
	//	GetInsp y GetTM concurrentemente con Spawn y Kill, hay que usar una estructura auxiliar
	//	para identificar el Thread que pide el Fix (solo es uno a la vez, por ser Spawn y Kill
	//	secciones criticas entre si). Tambien hay un mutex para la integridad de las operaciones 
	//	Fix, Undo y Get.
	// OJO, manejan semaforos: no pueden anidarse llamadas Fix ni Undo, o caeremos en un DeadLock.

	//Fix Insp for this thread, si se passa NULL pone el PInsp.
	void PInspForTT_Fix(StMIT_Inspector *ai_pstInsp=NULL) ;	
	void PInspForTT_Undo() ;

	// Se encarga de inicializar el inspector. NO usa metodos PInspForTT, esto ya debe de 
	//	estar puesto a los valores que se quieran.
	// Llama a los Init en el orden adecuado, y forma los nombres para los ficheros de
	//	traza e init respectivamente. Inserta las RK obligatorias.
	// Si ai_TaskId es =  MIT_NULL_ID, se esta creando el PInsp (CompId se pondra tambien a
	//	MIT_NULL_ID).

	void InitInspector( 
		StMIT_Inspector *ai_poInsp, MIT_Id_t ai_TaskId, MIT_Id_t ai_CompId, StMIT_CompPlugIn *ai_poPlugIn ) ; 

	//----------------------------------->Miembros auxiliares

	ClMutex  m_oSpawnKillMutex ;	//Seccion critica grupo A
	ClMutex  m_oUpdatingTList ;

	struct StGetSynchro {
		ClMutex m_oDoingFixUndo ;			
		m4bool_t m_bFixActivated ; 
		m4iden_t m_oFixTId ;
		StMIT_Inspector *m_pstFixInsp ;
	} m_stGetSynchro ;

} ;

//Thread manager
//============================================================================================

class M4_DECL_M4MIENV ClMIT_TM	
{
	friend class ClMIT_PM;

MIT_PRIVATE:

	StMIT_Inspector *m_poInsp ;	//Simple link

	//----------------------------------->Lista de Tasks en el Thread actual

	struct StTaskD {
		MIT_Id_t m_TaskId ;		//TaskId compartido entra las TLC correspondientes.
		MIT_Id_t m_RootCompId ;	//IdComp del TLC raiz de la tarea
		MIT_Id_t m_OwnerTaskId ;	//Task que creo esta Task, ID_NULL si en una RootTask
	} ;

	//Lista ordenada por TaskId.
	typedef map < MIT_Id_t, StTaskD, less<MIT_Id_t> > TL_TaskList_t ;
	TL_TaskList_t m_oTaskList ;

	//Lista hermana, para saber los Task hijos de una Task. Se actualiza paralelamente a la 
	//	lista anterior. Es multimap porque puede haber varias tareas hijas.
	//La clave es la tarea padre (OwnerTaskId en la otra lista), y el dato son las task hijas.

	typedef multimap < MIT_Id_t, MIT_Id_t, less<MIT_Id_t> > TL_SonTaskList_t ;
	TL_SonTaskList_t m_oSonTaskList ;

	//----------------------------------->Lista de Components en este Thread

	//Lista ordenada por CompId.
	typedef map < MIT_Id_t, ClMIT_TCM *, less<MIT_Id_t> > TL_CompList_t ;
	TL_CompList_t m_oCompList ;

	//----------------------------------->TL_TaskId activo en este momento

	//Cualquier llamada que cambie de Task -siempre se dira a que TL_Task pasar- cambia este
	//	valor. El llamador preserva el valor anterior, y lo restaura tras la llamada,
	//	para poder asi soportar subtasks dentro de una task.

	MIT_Id_t m_TL_ActiveTaskId ;

	//----------------------------------->InspEnv, compartido entre todos Insp de este Thread. 

	StMIT_InspEnv m_oInspEnv ;

MIT_PUBLIC:

	//----------------------------------->Inicializaciones

	ClMIT_TM() ;	
	virtual ~ClMIT_TM() { ClMIT_TM::End() ;  }

MIT_PRIVATE:

	void Init() ;	//Limpia el objeto
	virtual void End() ;	//Error immediato si queda algun item en m_oTaskList o m_oCompList.

	//----------------------------------->Creacion de Tasks

	// SpawnTask crea una nueva tarea. 
	// NOTA: Este proceso es llamado con PInspForTT_Fix, por lo que el trace, etc, generado se
	//		asigna al PInsp.
	//  Los pasos que sigue son:
	//	-Inserta nueva entrada en m_oTaskList, error immediato si no puede.
	//		El TaskId se le pasa, el RootCompId se saca del PlugIn, y el OwnerTaskId del 
	//		ActiveTaskId.
	//  -Se llama a SpawnSubTask con el CompId raiz.
	//		-SpawnSubTask. 
	//			-Se accede al TCM correspondiente. Si no existe, se crea uno (lo que provocara
	//				la creacion del ThreadEnv via PlugIn).
	//			-Si existe  el TCM+TaskId correspondiente, hay un grafo de llamadas. Se testea
	//				que el PlugIn es el mismo, error immediato si no lo es, fin si lo es.
	//			-Se llama a si misma recursivamente, con los datos de las Comp de las que tira
	//				(lo sabe por el PlugIn). 
	//			-No existe el TCM+TaskId, se crea. La creacion del TaskEnv e Insp se posterga.
	//	-Ahora falta que se llame a CreateTaskEnv.

	void SpawnTask(MIT_Id_t ai_TaskId, StMIT_CompPlugIn *ai_pstPlugIn) ;

	// Auxiliar recursiva de SpawnTask
	
	void SpawnSubTask(MIT_Id_t ai_TaskId, StMIT_CompPlugIn *ai_pstPlugIn) ;

	// Complementario a SpawnTask, crea los TaskEnv. El orden de creacion es como en la 
	//	derivacion de clases; primero se construyen los Comp hijos para que estos esten
	//	accesibles y construidos en el padre.
	// NOTA:DEBE ser llamado como antes, con el PInspForTT_Fix, pero a la hora de crear el 
	//	TaskEnv se hace un PInspForTT_Undo y un cambio del ActiveTask al TaskId, para que
	//	los news, trazas, etc, del TaskEnv se adjudiquen al Insp de la Task en cuestion.
	//	Este cambio es temporal, y se vuelve a restaurar el ActiveTask previo y el estado 
	//	PInspForTT_Fix.
	//	Tiene en cuenta la posibilidad de grafos, para no crear algo varias veces y cortar 
	//	el flujo de ejecucion.
	
	void CreateTaskEnv(MIT_Id_t ai_TaskId) ;

	// Auxiliar recursiva de CreateTaskEnv

	void CreateSubTaskEnv( MIT_Id_t ai_TaskId, MIT_Id_t ai_CompId ) ;

	//----------------------------------->Destruccion de Tasks

	// KillTask destruye una tarea existente. 
	// NOTA: Este proceso es llamdo con PInspForTT_Fix, por lo que el trace, etc, generado se
	//		asigna al PInsp.
	//  Los pasos que sigue son:
	//	-Se debe de haber llamado a DestroyTaskEnv.
	//	-Localiza la Task en la TaskList, para averiguar el arbol de Comp. Si no existe,
	//		error immediato.
	//	-Testea que no haya subtareas pendientes ejecutandose. Si las hay, error immediato.
	//  -Se llama a KillSubTask con el CompId raiz.
	//		-KillSubTask. 
	//			-Se accede al TCM correspondiente, que debe de existir, o error immediato.
	//			-Si NO existe  el TCM+TaskId correspondiente, hay un grafo de llamadas, y ya
	//				se destruyo el TCM+TaskId. Se sale,
	//			-Se llama a si misma recursivamente, con los datos de las Comp de las que tira
	//				(lo sabe por el PlugIn). 
	//			-El TCM+TaskId existe, se destruye. 
	//			-Si en el TCM ya no quedan Task, se destruye el TCM (lo que provocara
	//				la destruccion del ThreadEnv via PlugIn).

	void KillTask( MIT_Id_t ai_TaskId ) ;

	// Auxiliar recursiva de KillTask

	void KillSubTask(MIT_Id_t ai_TaskId, MIT_Id_t ai_CompId) ;

	// Complementario a KillTask, destruye los TaskEnv. El orden de destruccion es como en la 
	//	derivacion de clases; primero se destruye el Comp raiz, y luego los hijos.
	// NOTA:DEBE ser llamado como antes, con el PInspForTT_Fix, pero a la hora de destruir el 
	//	TaskEnv se hace un PInspForTT_Undo y un cambio del ActiveTask al TaskId, para que
	//	los deletes, trazas, etc, del TaskEnv se adjudiquen al Insp de la Task en cuestion.
	//	Este cambio es temporal, y se vuelve a restaurar el ActiveTask previo y el estado 
	//	PInspForTT_Fix.
	//	Tiene en cuenta la posibilidad de grafos, para no borrar algo varias veces y cortar 
	//	el flujo de ejecucion.

	void DestroyTaskEnv(MIT_Id_t ai_TaskId) ;

	// Auxiliar recursiva de DestroyTaskEnv

	void DestroySubTaskEnv( MIT_Id_t ai_TaskId, MIT_Id_t ai_CompId ) ;

	MIT_Id_t GetActiveTaskId() { return m_TL_ActiveTaskId ; }
	void     SetActiveTaskId(MIT_Id_t ai_Id) { m_TL_ActiveTaskId=ai_Id ; }


MIT_PUBLIC:

	//----------------------------------->Swap entre Tasks 
	
	// Cuando se va a ejecutar lago, hay que activar la Task en cuestion. Como se pueden 
	//	hacer llamadas anidadas, al salir de un area hay que restaurar el valor que habia
	//	originalmente.
	// Asi pues, el prototipo tipico de metodo interfaz de una DLL es:
	//
	//	void DoSomething(MIT_Id_t ai_TaskId, ...) {
	//		MIT_Id_t PrevT ;
	//		ClMIT_TM *TM= GET_TM() ;	
	//
	//		TM->InTask(ai_TaskId,&PrevT) ;
	//		{
	//			//Pueden llamarse a otras funciones interfaz de otras Task.
	//		} ;
	//		TM->OutTask(PrevT) ;
	//  } 
	

	void     InTask(MIT_Id_t ai_TaskId, MIT_Id_t *ao_PreviousTask) ;
	void     OutTask(MIT_Id_t ai_PreviousTask) ;

	// Para notificar errores y warnings a otros componentes PADRE. 
	// Por si son errores producidos en tiempo de Init o End de trace, este metodo verifica
	//	que el objeto este construido antes de mandarle la notificacion, y si no puede 
	//	localizar Comp, etc, simplemente no hace nada.

	void  NotifyProblem(
		ClMIT_Trace *ai_poCaller, ClMIT_Msg::MSG_TYPE ai_Type, 
		m4uint32_t ai_uiLocalId, m4char_t *ai_pcMsg) ;

MIT_PRIVATE:

	// Auxiliar recursiva de NotifyProblem

	void  SubNotifyProblem(
		MIT_Id_t ai_TaskId, MIT_Id_t ai_CompId,
		ClMIT_Trace *ai_poCaller, ClMIT_Msg::MSG_TYPE ai_Type, 
		m4uint32_t ai_uiLocalId, m4char_t *ai_pcMsg) ;
	
	// Auxiliar recursiva, pone el flag Crossed de los TCM de una rama 
	// No se usa en tareas Spawn ni Kill, ya que estas pueden disparar algun EC_Notify, que 
	//	tambien lo usa.

	void  SetCrossed(MIT_Id_t ai_TaskId, MIT_Id_t ai_CompId,m4bool_t ai_bSw=M4_FALSE) ;

MIT_PUBLIC:

	//----------------------------------->Gets
	
	m4uint32_t GetTLCount() { return m_oTaskList.size() ; }

	//Retorna el inspector correspondiente al ai_CompId pasado y a la actual TaskId activa
	//Error immediato si no encuentra.

	StMIT_Inspector * GetInsp(MIT_Id_t ai_CompId) ;

	//Retornan los Env asociados (pueden ser NULL).
	//Hacer el casting apropiado.
	//Error immediato si no encuentra.

	void *GetThreadEnv(MIT_Id_t ai_CompId) ;
	void *GetTaskEnv(MIT_Id_t ai_CompId) ;
} ;

//Thread Component manager
//============================================================================================

class ClMIT_TCM	
{
	friend class ClMIT_TM;
MIT_PRIVATE:

	StMIT_Inspector *m_poInsp ;	//Simple link

	//----------------------------------->Datos globales por TLC

	void *m_poThreadEnv ;	//Mantenido via callbacks. Puede ser NULL si no hay.

	//----------------------------------->Lista de Task en las que intervie el Comp

	struct StTaskD {
		MIT_Id_t m_TaskId ;			//TLC_TaskId, igual al TaskId que pertenece.
		StMIT_Inspector m_stInsp ;	//TLC_TaskInsp propio. m_poEnv NULL solo en Spawn 
		void *m_poTaskEnv ;			//Mantenido via callbacks. Puede ser NULL si no hay.
		StMIT_CompPlugIn *m_pstPlugIn ;	//Mantiene dialogo para soportar TLC_TaskType.
		m4bool_t m_bSwCrossed ;		//Flag auxiliar para testeo de grafos
	} ;

	//Lista ordenada por TaskId.
	typedef map < MIT_Id_t, StTaskD, less<MIT_Id_t> > TLC_TaskList_t ;
	TLC_TaskList_t m_oTaskList ;


MIT_PUBLIC:

	//----------------------------------->Inicializaciones

	ClMIT_TCM() ;	
	virtual ~ClMIT_TCM() { ClMIT_TCM::End(); }	

MIT_PRIVATE:

	void Init(StMIT_CompPlugIn *ai_pstPlugIn) ;	//Crea el ThreadEnv
	void End(StMIT_CompPlugIn *ai_pstPlugIn) ;	//Destruye el ThreadEnv
	virtual void End() ;	//No hace nada con el ThreadEnv

	// Error fatal si no tiene exito

	void NewTLC_Task( StTaskD * ai_poData ) ;	
	void DeleteTLC_Task( MIT_Id_t ai_TaskId  ) ;	


	//----------------------------------->Gets

	StMIT_Inspector * GetInsp(MIT_Id_t ai_TaskId) ;
	void            * GetTaskEnv(MIT_Id_t ai_TaskId) ;

	// Devuelve el StTaskD del la lista de TLC_Task, o NULL si no la encuentra.

	StTaskD * GetTLC_Task(MIT_Id_t ai_TaskId) ;

	m4uint32_t GetTLC_Count() ;

	void *GetThreadEnv() { return m_poThreadEnv; }
} ;

//PlugIn de los Componentes
//============================================================================================

//IMPORTANTE
//	-El PlugIn debe ser un objeto ESTATICO.
//	-Los objetos PlugIn deben de ser objetos CONST estaticos de los Comp que los precisan, 
//		para que esten siempre disponibles. 
//	-En un Comp hay tantos objetos PlugIn como TLC_TaskType se quieren implementar.
//  -Si no se indica lo contrario, las funciones que forman parte del PlugIn pueden son 
//		llamadas fuera del ambito de control de este componente, por lo cual NO pueden usarse
//		ningun objeto ni ninguna facilidad de las que da este componente.


struct M4_DECL_M4MIENV StMIT_CompPlugIn 
{
	//------------->Identificador de componente

	MIT_Id_t m_CompId ;
	m4char_t m_pcCompId[40] ;	//Usado para formar nombres de fichero unico (y otros).
									//Normalmente 3 caracteres.

	//------------->Componentes asociados

	//Da los PlugIns correspondientes a los componentes de los que tira este componente. 
	//-Estos se acceden via indice, de 0 a n, como si fuese un array. 
	//-Cuando el indice se sale de rango, devolver un NULL. 
	//-Puede no haber ningun componente asociado.
	//-NO puede haber varios PlugIn de un mismo componente (no se testea).
	//-Solo hace falta poner los PlugIn de los Comp de los que se tira directamente. 
	//	Los indirectos tambien se incluiran. Si se incluyen no pasa nada, es superfluo.

	typedef StMIT_CompPlugIn *(*GetSonPlugIn_t)(m4int16_t ai_Index) ;

	GetSonPlugIn_t m_pfGetSonPlugIn ;	


	//------------->Creacion de Insp propio

	// Los objetos en cuestion ya estan alocados, solo falta inicializarlos.
	//  -Al objeto SysInfo tambien hay que añadirle todas las Registry Keys que el componente
	//		necesita. 
	//		Hay que añadirsla key adicional MIT_RK_TRACE_FILE, que debe contener el path,
	//			nombre y extension de fichero donde se deja la traza. 
	//			Este nombre es usado para componer el nombre completo de traza.
	//			Si no se mete esta clave, por defecto se introducira la de este componente,
	//			ubicada en:
	//		Tras la callback, se añade la key adicional MIT_RK_LOG_INI_FILE, ubicada en
	//			   * NOTA: Lo mismo para la clave "LogIniFile".
	//
	//  -Es mejor inicializar el objeto MemProf con el Init que precisa del array de nombres,
	//		pues los CheckPoints saldran mucho mas claros.
	//  -Al objeto trace se le pasan los nombres de fichero a usar en el Init. 
	//		Los nombres no deben de cambiarse. La unica eleccion es no usarlos y utilizar en 
	//		su lugar NULL para eliminar un tipo de salida.
	//		Tambien es licito ajustar parametros de trace, tales como niveles de debug en 
	//		grupos, etc.
	
	typedef void (*InitSysInfo_t)(	ClMIT_SysInfo *ai_poSysInfo) ;
	typedef void (*InitMemProf_t)(	ClMIT_MemProf *ai_poMemProf) ;
	typedef void (*InitTrace_t)(	ClMIT_Trace   *ai_poTrace, 
		const m4char_t *ai_pcLocalTraceFile, const m4char_t *ai_pcM4INIFile) ;

	InitSysInfo_t m_pfInitSysInfo ;		//NULL Init estandar
	InitMemProf_t m_pfInitMemProf ;		//NULL Init estandar
	InitTrace_t   m_pfInitTrace ;		//NULL Init estandar

	//------------->Creacion de Env

	// El ThreadEnv DEBE ESTAR COMPUESTO EXCLUSIVAMENTE DE  TIPOS SIMPLES. No puede estar
	//	compuesto de ninguna clase implementada en este componente. NO pueden usarse 
	//	M4_NEWS ni nada parecido. Si hace falta news, hay que usarlos a pelo y acordarse de
	//	los correspondientes deletes en DeleteThreadEnv.
	// Es el sitio ideal para crear variables tipo buffer auxiliar de proceso.

	typedef void * (*NewThreadEnv_t)() ;				//Da ThreadEnv alocado.  
	typedef void   (*DeleteThreadEnv_t)(void *ai_poEnv) ;	

	NewThreadEnv_t    m_pfNewThreadEnv ;	//NULL si no hay ThreadEnv
	DeleteThreadEnv_t m_pfDeleteThreadEnv ;	//NULL si no hay ThreadEnv 

	// El TaskEnv no tiene restriccion ninguna. Puede estar compuesto de clases y de lo que
	//	se quiera, pues el Inspector esta completamente formado y disponible.
	//	Sin embargo, el TaskEnv no lo esta. Dentro de este metodo ningun objeto, directa o 
	//	indirectamente, puede llamar a GetTM() (y por ello tampoco al GetTaskEnv() ni al 
	//	GetThreadEnv()). Lo que si puede hacerse sin problema es acceder a estos valores
	//	tras la llamada a SpawnTask, tras el InTask (podemos alocar objetos en el 
	//	TaskEnv, etc). 
	// Lo mas normal es que simplemente tenga un estructura contenedora, y que sea dentro 
	//	del flujo normal de tratamiento de Task del Comp en cuestion donde se inicializen los
	//	datos pertinentes.
	// Primero se crean los TaskEnv de los Comp hijos, y luego el del Comp llamador.
	// Primero se destruye el TaskEnv del Comp llamador y luego los de Comp hijos.


	typedef void * (*NewTaskEnv_t)() ;
	typedef void   (*DeleteTaskEnv_t)(void *ai_poEnv) ;

	NewTaskEnv_t	m_pfNewTaskEnv ;		//NULL si no hay TaskEnv
	DeleteTaskEnv_t m_pfDeleteTaskEnv ;		//NULL si no hay TaskEnv

	//------------->Constructor estatico


	StMIT_CompPlugIn(
		MIT_Id_t ai_CompId, m4char_t * ai_pcCompId,
		GetSonPlugIn_t ai_pfGetSonPlugIn,	

		InitSysInfo_t ai_pfInitSysInfo=NULL,	//Si no se pasan, Inits estandar de este Comp
		InitMemProf_t ai_pfInitMemProf=NULL,
		InitTrace_t   ai_pfInitTrace=NULL,
		

		NewTaskEnv_t	ai_pfNewTaskEnv=NULL, 
		DeleteTaskEnv_t ai_pfDeleteTaskEnv=NULL,

		NewThreadEnv_t    ai_pfNewThreadEnv=NULL,
		DeleteThreadEnv_t ai_pfDeleteThreadEnv=NULL

	) ;

} ;

//Macros
//============================================================================================
/*
  El uso de estas macros es opcional, pero recomendable, ya que facilitan la gestion de 
tratamiento de Task.

----->Tareas Spawn

MIT_Id_t Spawn(...) 
{
	MIT_Id_t NewTask ;

	SPAWN_IN(NewTask,g_CompA_pstPlugIn) ;

	//Tareas inicializacion. Puede usarse cualquier Env.

	SPAWN_OUT()

	return NewTask ;
}
*/


//-------------------------------------------------------------------------
// CUIDADO: Estos defines se comentan al principio para poder verlos
//          Pero deben escribirse en una sola lines para compilar en UNIX
//--------------------------------------------------------------------------

/*#define SPAWN_IN(NewTask,PlugIn)		\
	{									\
	MIT_Id_t PrevT ;					\
	(NewTask)=SPAWN_TASK(&(PlugIn)) ;	\
	ClMIT_TM *TM = GET_TM() ; 			\
										\
	TM->InTask((NewTask),&PrevT) ;		\
	{									\
		StMIT_Inspector *Insp=TM_GET_INSP(TM) ;*/
#define SPAWN_IN(NewTask,PlugIn)		{	MIT_Id_t PrevT ; (NewTask)=SPAWN_TASK(&(PlugIn)) ;	ClMIT_TM *TM = GET_TM() ;  TM->InTask((NewTask),&PrevT) ;	{	StMIT_Inspector *Insp=TM_GET_INSP(TM) ;
		
/*#define SPAWN_OUT()					\
	} ;								\
	TM->OutTask(PrevT) ;			\
	} ;								\
	//	Aqui lo mas normal es hacer un return NewTask ;
*/
#define SPAWN_OUT()		} ; TM->OutTask(PrevT) ; } ;								\

/*
----->Tareas Kill

void Kill(MIT_Id_t ai_TaskId,....) 
{
	KILL_IN(ai_TaskId) ;
	
	//Tareas destruccion. Puede usarse cualquier Env.

	KILL_OUT(ai_TaskId) ;
}

*/

//-------------------------------------------------------------------------
// CUIDADO: Estos defines se comentan al principio para poder verlos
//          Pero deben escribirse en una sola lines para compilar en UNIX
//--------------------------------------------------------------------------

/*#define KILL_IN(TaskId)				\
	{								\
	MIT_Id_t PrevT ;				\
	ClMIT_TM *TM = GET_TM() ; 		\
									\
	TM->InTask((TaskId),&PrevT) ;	\
	{								\
		StMIT_Inspector *Insp=TM_GET_INSP(TM) ;*/

#define KILL_IN(TaskId)	 {	MIT_Id_t PrevT ;	ClMIT_TM *TM = GET_TM() ;  TM->InTask((TaskId),&PrevT) ;	{ 	StMIT_Inspector *Insp=TM_GET_INSP(TM) ;

/*#define KILL_OUT(TaskId)	\
	} ;					\
	TM->OutTask(PrevT) ;	\
							\
	KILL_TASK((TaskId)) ;	\
	} ;*/

#define KILL_OUT(TaskId)	} ; 	TM->OutTask(PrevT) ;	KILL_TASK((TaskId)) ;	} ;


/*
----->Tareas Swap task

void DoTask(MIT_Id_t ai_TaskId,....) 
{
	SWAP_IN(ai_TaskId) ;

	//Tareas inherentes de ai_TaskId. Puede usarse cualquier Env.

	SWAP_OUT() ;
}
*/

/*#define SWAP_IN(TaskId)				\
	{								\
	MIT_Id_t PrevT ;				\
	ClMIT_TM *TM= GET_TM() ;		\
									\
	TM->InTask((TaskId),&PrevT) ;	\
	{								\
		StMIT_Inspector *Insp=TM_GET_INSP(TM) ;*/

#define SWAP_IN(TaskId)	   {	MIT_Id_t PrevT ;     ClMIT_TM *TM= GET_TM() ;	TM->InTask((TaskId),&PrevT) ;	{	StMIT_Inspector *Insp=TM_GET_INSP(TM) ;


/*#define SWAP_OUT()			\
	} ;						\
	TM->OutTask(PrevT) ;	\
	} ;*/

#define SWAP_OUT()	        } ; 	TM->OutTask(PrevT) ;  } ;

