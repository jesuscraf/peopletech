//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             channel.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Manuel Lazcano (28/05/98) - Tipo CURRENCY
//
// Definition:
//
//    API con la capa de canales
//
//
//==============================================================================
#ifndef __APICHANNEL__H__
#define __APICHANNEL__H__

//=========================================================================

#include "m4stl.hpp"
#include "m4ipcs.hpp"

#include "clstr.h"
#include "xengval.h"

//#include "midates.hpp"
//#include "m4string.hpp"
#include "m4clflag.h"
#include "formattp.h"

#include "cltypes.hpp"
//Para el OrderBy
#include "handles.hpp"
#include "cldefine.hpp"
class ClNode ;
class ClChannelManager;
class ClM4ObjService ;

class ClMIT_Chn ;

#include "execontx.hpp"
#include "channel.hpp"
#include "colitemdefit.hpp"


#undef TYPE		//Pendiente de que lo quiten en server (salvajada descomunal)



/*NOTAS SOBRE LA ARQUITECTURA
	
	1 LDB -> n MVC  
	1 MVC -> 1 conexion con una LDB 
		  -> 1 Env (de ClVMClientEnv o ClVMServerEnv segun version).
		  -> n canales.
		  -> 1 "LogOn" simultaneo.
	1 "LogOn a MVC" -> 1 canal sesion 
	1 canal -> 1 MVC -> 1 LDB
			-> n accesos
	1 acceso -> 1 canal


	-Capa LDB. Es multithread, independiente de la MVC, hay una  instancia por proceso/server.
		NO debemos de inicializarla ni hacer nada con ella, pues la capa MVC lo gestiona.
	-MVC y su Env. NO es multithread, hay una instancia por thread/subsistema server. 
		En el Init del Env se engancha a la LDB y la levanta si hace falta. Lo mismo vale para
		el ShutDown.

	Versiones de MVC:

	-SM. Single machine. La capa canal tiene tanto la funcionalidad client como la server.
		El LogOn() levanta el canal sesion.
	-DIC. Cliente distribuido. Solo parte client. Sigue existiendo el LogOn(), y tambien 
		levanta el canal sesion. La sincronizacion client-server es realizada de forma
		transparente.
	-Server. Solo la parte server. NO existe el LogOn(), pues el susbsitema de usuarios de 
		server se encarga de esta tarea y de la gestion del canal sesion. 
		Esta parte es la que esta un poco oscura.


*/

/*NOTAS SOBRE EL WRAPPER
		
	Si un MVC, canal, acceso, etc, es pasada (NO creada) a este modulo, el modulo USA estos
objetos, pero NO los destruye. En el otro extremo, cualquier MVC, canal, acceso, etc, que
cree este modulo, sera destruido por este modulo. 
	En la capa canal, el valor de un item se guarda en un miembro ClChnValue, que es 
REUTILIZADO en virtud de la sobrecarga [] de un miembro ClItem. Osea, un ... 
	Item[ItemName].Value...
reutiliza el Value. Esto quiere decir que en el,momento en que el mismo registro se 
selecciona otro item (o incluso el mismo), se pierde toda informacion de estado del Value,
tal como indices de Slices y otros. Esta es la razon de tener que reposicionarnos siempre
y de tener que guardar el Index.

*/



/*NOTAS SOBRE CANCELACIONES

 Todos los metodos de capa CANAL susceptibles de producir problemas en un estado de cancelado
actualizan y preguntan por este estado: sobre todo los metodos en cuestion son los de nivel 
canal y registro, mientras que no producen problemas accesos a items.
 Si un metodo gestiona el estado de cancelado, NO se hace nada en la capa canal, y se 
devuelven errores o ponen valores dummy. No lanzan mensajes, pues el mensaje de error de 
cancelacion ya se envio cuando se puso el estado de cancelado, o si lateralmente mandan 
alguno, da igual, pues no saldran.
*/

/*=========================================================================ClMIT_ItemExternProp

	Muchos items pueden compartir un único objeto de este tipo, por eso en ClMIT_ChnItem solo 
hay punteros a este tipo de objeto, y no objetos.
	La relación siempre es de uso: los items nunca borran ni crean objetos de este tipo,
y el objeto ClMIT_ItemExternProp debe vivir mientras lo hagan los ClField asociados.
	La gran ventaja es que aqui agrupamos comportamientos comunes a muchos objetos: si por
ejemplo hay 50 items en los que queremos, de golpe, variar el rango de slices, con 
actualizar esto una vez en ClMIT_ItemExternProp vale, y no hace falta tener que recorrer todos 
items (y tener que mantener por ello una lista).
	El link no se pone en el Init, si no posteriormente. Puede ser reinicializado a NULL, y
también reinicializado tantas veces como se quiera. El proceso Init no debe depender para
nada de ningun miembro de esta clase. Más bien está pensada para poner propiedades que
puedan modificar a comportamientos ya implementados en mienbros de ClMIT_ChnItem.

*/

class ClMIT_ItemExternProp 
{
MIT_PRIVATE:

	// Utilizado por GUIDE_LOAD

	m4date_t	 m_StartDate, m_EndDate ;

MIT_PUBLIC:

	void SetStartDate ( m4date_t ai_StartDate ) { m_StartDate=ai_StartDate; }
	void SetEndDate (   m4date_t ai_EndDate ) { m_EndDate=ai_EndDate; }

	m4date_t GetStartDate() { return m_StartDate ; }
	m4date_t GetEndDate()   { return m_EndDate ; }
} ;

//============================================================================ClMIT_MVC
/*
	Gestiona el entorno MVC. Se puede inicializa de dos maneras:
	-Modo Link. Se enlaza con una MVC ya existente. Mera relacion de uso.
	-Modo build. Se construye una MVC, debe hacerse el LogOn(). Se hace ShutDown(). Este modo
		no esta implementado para compilaciones Server.
*/


class ClMIT_MVC
{
MIT_PRIVATE:

	StMIT_Inspector *m_poInsp ;	//Simple link
    StMIT_APIChn_TaskEnv *m_poTaskEnv ;	//Simple link

	m4bool_t m_bSwExternVM ;
	ClM4ObjService   * m_poService ;	//Si ! m_bSwExternVM, creado, sino, NULL.
	ClChannelManager * m_poManager;		//Link. Si m_bSwExternVM, pasado directamente, si no,
										//	sacado de m_poService.
	ClMIT_SessionChn *m_poSessChn ;		//Objeto. Solo alocado si LogOn activo.

	static ClMutex m_oMutexCreatedCounter ;
	static m4uint32_t m_uiCreatedMVCCount ;	//Contador de objetos MVC creados (not extern) con 
											//	éxito

	ClAccess* ItemV_poDeclareExtFileChn;     //Simple link

MIT_PUBLIC:

	ClMIT_MVC();
	virtual ~ClMIT_MVC() { ClMIT_MVC::End(); }
	virtual size_t GetSizeof() { return sizeof(ClMIT_MVC) ; }

	// End(), en modo propio, baja la MVC. Si se hizo un Init en modo Link, no se hace ningun
	//	LogOff explicito ni nada parecido.
	// Tambien actualiza UpdateCancel.

	virtual void End();

	// ----------------------------- Inicialización modo Link-------------------------------	

	// Enlazamos con el ChannelManager, Env y canal sesion activos.El LogOn debe de estar
	//	hecho, enlaza con el canal sesion. Es posible hacer posteriores LogOn.
	// Da TRUE si todo va bien.
	// En estado CANCEL se devuelve M4_FALSE. Esto se testea al final del proceso, ya
	//	que estos metodos son los que inicializan el Environment donde esta el flag de
	//	cancelado MVC.

	m4bool_t Init( ClChannelManager *ai_poManager, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;

	// ----------------------------- Inicialización modo Build-------------------------------	

	//Construimos todo, primero el Env y luego la MVC.
	// Da TRUE si todo va bien.
	// No implementado para versiones Server, da FALSE.
	// En estado CANCEL se devuelve M4_FALSE. Esto se testea al final del proceso, ya
	//	que estos metodos son los que inicializan el Environment donde esta el flag de
	//	cancelado MVC.
	//Queda pendiente el LogOn().

	m4bool_t Init( ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;

	// ----------------------------- LogOn ---------------------------------------------------
	//
	// Necesario en modo Build, solo necesario en modo Link si la MVC pasada no estaba en LogOn
	//
	//En un modo build, antes de usar los canales debe hacerse LogOn tras hacer un Init.
	//ai_pcServer, ai_uiPort y ai_bSSLType solo tienen repercusion en DIC.
	//Enlaza con el canal sesion. Si ya se hizo un LogOn, da TRUE, y en realidad seguimos
	//	enlazados con el mismo usuario que la primera vez.

	m4bool_t LogOn(m4char_t *ai_pcUser, m4char_t *ai_pcPassword, m4int16_t ai_Language,
		m4char_t *ai_pcServer=NULL, m4uint32_t ai_uiPort=0, m4bool_t ai_bSSLType=M4_FALSE ) ;

	void LogOff() ;

	// ----------------------------- Consultas ----------------------------------------------	

	// Solo para usar. Puede ser NULL si no se ha inicializado. 

	ClMIT_SessionChn * GetSessChn() { return m_poSessChn ; }

	m4bool_t IsExternal() { return m_bSwExternVM ; }

	// ATENCION: Esta funcion solo puede ser llamada si se ha hecho un LogOn, si se nos pasa una
	//  maquina virtual ya levantada no se debe llamar.
	m4return_t GetLanguage(m4language_t *ai_pLanguage);

	// ----------------------------- UpdateCancel -------------------------------------------	

	// Actualiza el estado de cancelacion del Insp conforme al estado de peticion de 
	//cancelacion del Env. 
	// Solo hace algo si el Env esta accesible (no NULL), y si el Insp no ha recibido aun ninguna 
	//	cancelacion.
	// Siempre devuelve el estado del flag, accesible tambien desde el Insp.

	m4bool_t UpdateCancel() ;

	// Como el anterior, solo que el environment al que pregunta el flag es el correspondiente 
	//	al ai_poManager pasado.

	m4bool_t UpdateCancel(ClChannelManager * ai_poManager) ; 

	ClChannelManager * GetChannelManager() { return m_poManager; }
	ClMIT_SessionChn *GetSessionChn() { return m_poSessChn ; }

	m4uint32_t GetCreatedMVCCount() ;


	// ----------------------------- DeclareExternalFile--------------------------------------	
	
	//Inicializa el canal "CH_JS_EXE_JIT", para la declaracion de interes de los ficheros de salida.
	// En estado CANCEL, no hace nada y devuelve M4_FALSE

	m4bool_t StartUpDeclareExtFile() ;

	//Ejecuta el método para la declaracion de ficheros.
	// le pasamos el tipo de declaracion y la ruta del fichero a declarar
	m4bool_t DeclareExternalFile(DECLARE_EXTERNAL_FILE ai_DeclareExtFile, m4char_t* ai_pcFilePath);

	//Destruye el acceso al canal "CH_JS_EXE_JIT". Se debe hacer despues del CloseAll(), en el End
	// del DvcFile, para que no afecte a la declaracion en la ruptura a varios grupos.
	m4bool_t DestroyDeclareExtFileAccess();

	
	// ----------------------------- ClVMRunContext ------------------------------------------	

	static ClChannelManager *GetChannelManagerFromRunContext(ClVMRunContext * ai_poRunContext) ;

	static ClAccess *GetAccessFromRunContext(ClVMRunContext * ai_poRunContext) ;
} ;


//============================================================================ClMIT_ChnNode
/*
	Esta clase sumininistra todo lo necesario para manejar nodos, y para moverse por los
registros de estos.
	No crear directamente clases de este tipo; usar en su lugar ClMIT_Chn.GetNode().
*/


class ClMIT_ChnNode
{
	//El constructor enlaza con el TaskEnv:no pueden crearse objetos de este tipo en la
	//	callback.

	friend class ClMIT_Chn ;
	friend class ClMIT_ChnItem ;

MIT_PUBLIC:

	enum { RECORD_EOF=-1 } ;
	typedef m4uint32_t Record_t ;	//Van de 0 a n, RECORD_EOF es marca de EOF.

MIT_PRIVATE:

	StMIT_Inspector *m_poInsp ;	//Simple link
    StMIT_APIChn_TaskEnv *m_poTaskEnv ;	//Simple link
	ClMIT_IdDesc m_oIdDesc ;

	ClMIT_Chn * m_poChn ;				//Link a canal, ni crea ni borra.
	ClNode	*pNode;					//Relacion de uso

	ClHandle m_oHandleOrderBy;      //Handle del Item OrderBy (type 62) del Nodo. Si no hay puede ser nulo

	enum { 

		//Por defecto a TRUE, hace que cualquier operacion Next, Previous, GoFirst, 
		//	GoLast y GetCount ignore los registros marcados como borrados.

		IGNORE_DELETED =ClMIT_Flag::BIT0	
	} ;
	ClMIT_Flag m_oFlag ;

MIT_PUBLIC:


	// ----------------------------- Inicialización---------------------------------------	

	ClMIT_ChnNode();
	virtual ~ClMIT_ChnNode() { ClMIT_ChnNode::End(); }
	virtual size_t GetSizeof() { return sizeof(ClMIT_ChnNode) ; }

MIT_PRIVATE:

	// True si ejecucion tiene exito

	m4bool_t Init( ClMIT_Chn * ai_poChn, const m4char_t * ai_pcNameNode, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR   ) ;

MIT_PUBLIC:

	virtual void End();

	// ----------------------------- Moverse ---------------------------------------------	

	// Cuando nos movemos en un nodo, los nodos hijos de la jerarquia se actualizan tambien.
	// Los numeros de registro van de 0 a n. 
	// Si ai_Record es mayor que el ultimo record valido, o ai_Record es RECORD_EOF,
	//	siempre se queda en EOF.
	// En estado CANCEL, siempre se mueve a EOF

	void MoveTo( Record_t ai_Record) ;

	// Si no hay registros, todos se quedan en EOF. En estado CANCEL, siempre se mueven a EOF.

	m4bool_t Next()	;
	void Previous() ;	//En EOF() no va al ultimo. 

	void GoFirst( ) ;
	void GoLast( ) ;	//Para que vaya con carga parcial, chapuza: MUY, MUY, MUY LENTO.

	// -----------------------------Informacion carga parcial----------------------------------	

	m4bool_t IsPartialLoad() { return M4_BOOL(GetPartialLoadRows()>0); }
	m4uint32_t GetPartialLoadRows() ;
	m4uint32_t GetPartialLoadKeepRows() ;

	//Recarga los datos otra vez (si habia datos antes, los descarga).

	void ReLoad() ;

	// --------------------------Insercion y Borrado de registros--------------------------------
	
	// Inserta un nuevo registro VACIO y nos deja posicionados en el registro current insertado 
	void Insert();

	// Borra un registro actual (Pone la marca de borrado , no borra fisicamente). Nos deja posicionados
	//en el registro borrado o en el Next, dependiendo del flag de IGNORE_DELETED. Si IsIgnoreDeleted(),
	//nos movemos al Next valido.
	void Delete();

	//Borra todos los registros del RecordSet. Una vez borrados se posiicona en EOF.
	m4bool_t DeleteAll();

	// -----------------------------Capacidades especiales--------------------------------	

	void SetIgnoreDeleted(m4bool_t ai_bSw) { m_oFlag.Switch(IGNORE_DELETED,ai_bSw); }
	m4bool_t IsIgnoreDeleted() { return m_oFlag.True(IGNORE_DELETED); }

	// ----------------------------- Gets ------------------------------------------------	

	//Si nodo padre en EOF, da siempre EOF. 
	//En estado CANCEL, siempre devuelve TRUE ( en EOF() )
	m4bool_t IsEOF() ;		

	//Con IsIgnoreDeleted es más lento, debe recorrerse el nodo. 
	//Si es preciso disparar el autoload para saber el count, se dispara.
	//CUIDADO: MUY lento ademas con carga parcial.
	//En estado CANCEL, siempre devuelve 0
	Record_t GetRealCount() ;	

	//Devuelve el contador de la capa de canal, tal cual.
	//Si es preciso disparar el autoload para saber el count, se dispara.
	//En carga parical NO da el total de la select, sino lo que hay en memoria en el momento.
	//En estado CANCEL, siempre devuelve 0
	Record_t GetCount() ;	

	//Da el numero de registro actual, RECORD_EOF si se esta en EOF. 
	//En estado CANCEL, siempre devuelve RECORD_EOF
	Record_t GetNumRecord() ;	

	const ClMIT_IdDesc & GetIdDesc() { return m_oIdDesc ; }	//Solo para uso inmediato

	//En estado CANCEL, no hace nada y devuelve M4_TRUE
	m4bool_t IsRecordDeleted() ;

	// Comprueba si el registro actual ha sido modificado. Verifica los siguientes casos:
	// MODIFIED, NEW, DELETED, MODIFIED_AND_DELETED.
	m4bool_t IsRecordModified() ;

	//Devuelve el Id del la TI asociada al Nodo
	m4pcchar_t GetTiId() ;

	//Delvuelve el canal real del que proviene el nodo (por el tema de herencias)
	m4pcchar_t GetM4ObjId();

	m4bool_t UpdateCancel() ;

	//=========== TRATAMIENTO DEL ORDER BY ==================

	//Devuelve el handle del Item del OrderBy
	ClHandle GetOrderByHandle() { return m_oHandleOrderBy; }

	//Obtiene el valor del Item ORDER BY
	m4bool_t GetOrderByItem(ClMIT_Str *ai_poStrOrderBy);

	//Actualiza el valor del Item ORDER BY
	m4return_t SetOrderByItem(m4char_t* ai_pcStrOrderBy);

	//------------------------------------------------------------------------------------

};

//============================================================================ClMIT_Chn

class ClMIT_Chn	 
{
	//El constructor enlaza con el TaskEnv:no pueden crearse objetos de este tipo en 
	//	la callback.

	friend class ClMIT_ChnNode ;
	friend class ClMIT_ChnItem ;

MIT_PUBLIC:

	enum { 
		LINKED=ClMIT_Flag::BIT0, 
		EXTERN_CHANNEL=ClMIT_Flag::BIT1,
		PRIVATE_ACCESS=ClMIT_Flag::BIT2
	} ;
	enum AUTOLOAD_MODE { 
		AUTOLOAD_OFF=M4CL_AUTOLOAD_OFF, AUTOLOAD_BLOCK=M4CL_AUTOLOAD_BLOCK,
		AUTOLOAD_PRG=M4CL_AUTOLOAD_PRG,	AUTOLOAD_NODESAYS=M4CL_AUTOLOAD_NODESAYS 
	} ;
    

	enum ORGANIZATION_TYPE {
		ORGANIZATION_NONE=M4CL_ORGANIZATION_TYPE_NONE,
		ORGANIZATION_MONO=M4CL_ORGANIZATION_TYPE_MONO,
		ORGANIZATION_MULTI=M4CL_ORGANIZATION_TYPE_MULTI
	} ;

MIT_PROTECTED:

	StMIT_Inspector *m_poInsp ;	//Simple link
    StMIT_APIChn_TaskEnv *m_poTaskEnv ;	//Simple link
								
MIT_PRIVATE:

	ClMIT_IdDesc m_oIdDesc ;

	ClChannelManager * m_poManager;
    ClChannel * m_poChannel;
	ClAccess  * m_poAccess;

	typedef map< ClMIT_Str, ClMIT_ChnNode*, less<ClMIT_Str> > NodeList_t;
	NodeList_t m_oNodeList;
	ClMIT_Flag m_oFlag ;


MIT_PUBLIC:

	// ----------------------------- Inicialización---------------------------------------	


	// Incializa el canal moneda; si no se hace, no podran hacer cambios. Cuando el canal que
	//	lo levanta se destruye, el puntero ya no es valido.
	//Este metodo y el siguiente podrian haberse hecho mejor, pero como aun no esta la cosa
	//	clara, lo dejamos asi por ahora.
	//En estado CANCEL, no hace nada y devuelve M4_FALSE

	m4bool_t StartUpCurrency() ;

	// Cambia moneda. Si el acceso a la moneda mo se inicializo, lo intentara inicializar.
	// ao_pcCur y ao_dCurValue son tanto parametros de entrada como de salida. Solo si el 
	//	metodo tiene exito, cambian el tipo al nuevo tipo, y cambian el valor.
	//En estado CANCEL, no hace nada y devuelve M4_FALSE

	static m4bool_t ExchangeCurrency( ClMIT_Str * ao_poCur, const m4char_t* ai_pcDestCur,
		m4date_t ai_ExchDate, const m4char_t* ai_pcExchType, m4double_t *ao_dCurValue  );

	ClMIT_Chn() ;
	virtual ~ClMIT_Chn() {  ClMIT_Chn::End(); }
	virtual size_t GetSizeof() { return sizeof(ClMIT_Chn) ; }
	
	// El canal pertenece a este modulo: su Channel y Access sera creado y destruido por 
	//	esta clase, y el Manager sera el del MVC del entorno.
	// Levanta un canal, pero no hace el load.
	// Da true si tiene exito.
	// En estado CANCEL, no hace nada y devuelve M4_FALSE

	m4bool_t InitOwn( const m4char_t * ai_pcNameChn, m4int16_t ai_iMetadataLang,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR,
		ClAccess *ai_poMetaChnAccess=NULL, 
		m4bool_t ai_bBuildAsMetaChannel=M4_FALSE) ;	 //Tras ai_ErrorEffect porque es de raro uso
									//Si se pasan los dos, se hace caso a ai_poMetaChnAccess	

	// El canal NO pertenece a este modulo, el Channel, Access y Manager sera el indicado. 
	//	Ni el Channel ni el Manager son destruidos, y el canal quedara cargado con los datos 
	//	del ultimo Load ejecutado.
	// Se enlaza con un canal externo, que ya esta levantado y creado.
	// En estado CANCEL, no hace nada y devuelve M4_FALSE

	m4bool_t InitCommonAccess( ClAccess *ai_poAccess, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;

	// Igual que el anterior, solo que se crea un acceso privado (que se destruira al destruir
	//	este objeto), con lo cual los recorridos por los nodos son privados a este canal.

	m4bool_t InitPrivateAccess( ClChannel *ai_poChn, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;
	m4bool_t InitPrivateAccess( ClAccess *ai_poAccess, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;

	// Entre una secuencia Init y una Load, pueden crearse Items de tipo PROPERTY, rellenarse 
	//	sus valores y hacer un Save. Posteriormente, el Load usara estos items en la SELECT 
	//	para levantar el canal.
	// El Load puede ejecutarse varias veces. Si no es la primera vez que se llama a Load, 
	//	debe de hacerse un Unload para que solo haya un 'grupo de datos Load' a la vez en 
	//	memoria, o en caso contrario habra mas grupos y el consumo de memoria se disparara.
	//  Si un Load detecta que el canal es AutoLoad, NO hace nada. 
	// Tras el Unload, pueden cargarse los items property, pero nunca antes, porque el 
	//	Unload los destruye. De hecho, este es el motivo para que la funcionalidad de 
	//	Unload no puede ser absorbida por Load.
	//Load, En estado CANCEL, no hace nada y devuelve M4_FALSE


	m4bool_t Load() ;
	m4bool_t Unload() ;

	// Cierra el canal. Destruye todos los nodos enganchados al canal.
	// Ver Inits para saber el comportamiento en cada caso.
	virtual void End();

	// Hace un CheckPoint del canal. 
	// En estado CANCEL, no hace nada y devuelve M4_FALSE

	void CheckPoint() ;
	
	// La clase mantiene una lista de nodos creados sobre el canal. Cuando hacemos un GetNode
	//	,mira si el nodo ya se creo: si es asi nos devuelve el puntero a el y ya esta. Si no
	//	es asi, crea otro elemento en la lista y nos da su direccion.
	// Esta direccion se entiende que es para uso del nodo y nada más: no hay que borrar los
	//	nodos, porque esto ya lo hace esta clase.
	// Devuelve NULL si hay error.
	// En estado CANCEL, no hace nada y devuelve NULL

	ClMIT_ChnNode *GetNode( const m4char_t *ai_pcNameNode, 
			ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;

	// -----------------------------Capacidades especiales--------------------------------	

	// Por defecto es FALSE. Si se quiere activarlo, hacerlo ANTES de una operacion Load, o
	//	esta cargara todo previamente, con lo que no tendra sentido el AutoLoaf.

	m4bool_t IsAutoLoad() ;
	void SetAutoLoad(AUTOLOAD_MODE ai_AutoLoadMode) ;
	AUTOLOAD_MODE GetAutoLoad() ;

	static m4char_t * AutoloadModeToTraceStr(AUTOLOAD_MODE ai_Mode) ;
	m4char_t * AutoloadModeToTraceStr() { return AutoloadModeToTraceStr(GetAutoLoad()); }

	// ----------------------------- Gets ------------------------------------------------	

	const ClMIT_IdDesc & GetIdDesc() { return m_oIdDesc ; }	//Solo para uso inmediato

	// Dice si el objeto tiene algun canal conectado.

	m4bool_t IsLinked()  { return m_oFlag.True(LINKED); }

	// Dice si el canal es externo: esto es, creado y levantado por otro modulo.
	
	m4bool_t IsExtern()  { return m_oFlag.True(EXTERN_CHANNEL); }

	m4bool_t IsPrivateAccess()  { return m_oFlag.True(PRIVATE_ACCESS); }


	m4bool_t UpdateCancel() { return m_poTaskEnv->m_poMVC->UpdateCancel(m_poManager) ; } 

	ORGANIZATION_TYPE GetOrganizationType() ;

	//Solo si el tipo es ORGANIZATION_MONO, puede devolver un nombre de sociedad.
	//En los otros  casos, simplemente devolvera la cadena "".
	
	m4char_t * GetOrganization( ) ;


	// --------------------Publicacion---------------------------

	//Retorna el GroupId de publicacion, 0 si hay error (ya lanza un mensaje error).
	//A llamar SOLO UNA VEZ por cada nuevo grupo de publicacion.
	//Cuando se llama debe garantizarse que el canal de datos este en el registro 
	//	correspiente al break en cuestion, ya que estte metodo puede copiar datos de ahi.

	m4uint32_t NewPublishGroup( const m4char_t *ai_ReportId, const m4char_t *ai_RootPath ) ;

	//Notifica que un file a formar parte de un grupo de publicacion.
	//El file debe estar presente hasta el EndPublish. Hasta ese momento, puede alterarse el
	//	contenido de ficho file; no se hace nada con él hasta el EndPublish
	//No hace falta que el canal de datos este posicionado, basta tener el groupId.
	//Retorna true si todo va bien.

	m4bool_t NotifyFileToPublish( 
		m4uint32_t ai_PublishGroupId, const m4char_t *ai_FilePath, m4bool_t ai_bIsStartFile ) ;

	//Notifica que ya se puede publicar el grupo. Efectua los uploads precisos, el fichero debe
	//	estar completamente actualizado. Tras este metodo, si se desea, se puede borrar.
	//No hace falta que el canal de datos este posicionado, basta tener el groupId.
	//Da true si todo fue bien.

	m4bool_t EndPublishGroup(m4uint32_t ai_PublishGroupId) ;

	// --------------------Llamadas con informacion capa canal---------------------------
	// Estos metodos dan datos dependientes de capa canal. Cuanto menos se usen, mejor

	// -------->Otros

	ClChannelManager * GetChannelManager() { return m_poManager; }
    ClChannel * GetChannel() { return m_poChannel; }
	ClAccess  * GetAccess() { return m_poAccess; }

	//------------------------------------------------------------------------------------

};

//============================================================================ClMIT_ChnItem

/*
	Contiene toda la informacion para identificar un item concreto de un nodo concreto de 
un canal concreto.
	Adicionalmente tiene la posibilidad de contener un valor de item, util porque desde
esta clase podemos leer y actualizar items.
	Un item de canal puede ser referenciado por uno o más de estos objetos.
*/

class ClMIT_ChnItem	
{
	//El constructor enlaza con el TaskEnv:no pueden crearse objetos de este tipo en 
	//	la callback.
MIT_PUBLIC:

	// TYPE, SCOPE, y los TOTALIZE coinciden con los valores usados en la cm.lib 
	//	(y por ello en el modelo de datos).

	enum TYPE { 
		METHOD=M4CL_ITEM_TYPE_METHOD, CONCEPT=M4CL_ITEM_TYPE_CONCEPT, 
		FIELD= M4CL_ITEM_TYPE_FIELD,  PROPERTY=M4CL_ITEM_TYPE_PROPERTY
	} ;
	enum SCOPE { 
		BLOCK=M4CL_ITEM_SCOPE_BLOCK, REGISTER=M4CL_ITEM_SCOPE_REGISTER, 
		NODE=M4CL_ITEM_SCOPE_NODE
	} ;

	//Importante: cualquier LOAD_MODE debe devolver un valor del mismo tipo que el item.
	//Por eso un Count no es un LoadMode. (Un Count sobre un item tipo cadena no devuelve
	//	una cadena). 

	enum LOAD_MODE { 

		TOTALIZE_SUM	=M4CL_ITEM_TOTALIZE_SUM,
		TOTALIZE_AVG	=M4CL_ITEM_TOTALIZE_AVG,
		TOTALIZE_MAX	=M4CL_ITEM_TOTALIZE_MAX,
		TOTALIZE_MIN	=M4CL_ITEM_TOTALIZE_MIN,
		TOTALIZE_FIRST	=M4CL_ITEM_TOTALIZE_FIRST,
		TOTALIZE_LAST	=M4CL_ITEM_TOTALIZE_LAST,
		TOTALIZE_DESIGN	=-1,

		TOTALIZE_SLICE_OFF=5000,
		TOTALIZE_SLICE_SUM		=TOTALIZE_SLICE_OFF+M4CL_ITEM_TOTALIZE_SUM,
		TOTALIZE_SLICE_AVG		=TOTALIZE_SLICE_OFF+M4CL_ITEM_TOTALIZE_AVG,
		TOTALIZE_SLICE_MAX		=TOTALIZE_SLICE_OFF+M4CL_ITEM_TOTALIZE_MAX,
		TOTALIZE_SLICE_MIN		=TOTALIZE_SLICE_OFF+M4CL_ITEM_TOTALIZE_MIN,
		TOTALIZE_SLICE_FIRST	=TOTALIZE_SLICE_OFF+M4CL_ITEM_TOTALIZE_FIRST,
		TOTALIZE_SLICE_LAST		=TOTALIZE_SLICE_OFF+M4CL_ITEM_TOTALIZE_LAST,
		TOTALIZE_SLICE_DESIGN			=-2,

		GUIDE_LOAD=6000, SLICE_LOAD, RANGE_SLICE_LOAD, ITEM_LOAD, 
		NULL_LOAD
	} ;

MIT_PRIVATE:

	StMIT_Inspector *m_poInsp ;	//Simple link
    StMIT_APIChn_TaskEnv *m_poTaskEnv ;	//Simple link

	ClMIT_IdDesc m_oIdDesc ;
	ClMIT_ChnNode  * m_poNode;	//Link a nodo, ni se aloca ni borra nada.
	ClMIT_Str        m_oItemName ;
	TYPE       	 m_Type ;  
	SCOPE		 m_Scope ;
	LOAD_MODE    m_LoadMode ;
	m4date_t	 m_StartDate, m_EndDate ;

	ClMIT_ItemExternProp * m_poExternProp ;	//simple link, puede no tenerlo, NULL.

	m4return_t ConvertStringToDouble (m4pcchar_t ai_pccItemValue, m4double_t & ao_dDoubleValue);
	m4return_t ConvertStringToDate(m4pcchar_t ai_pccDateValue, m4uint8_t ai_uiItemM4Type, m4double_t & ao_dDateValue);
	m4return_t GetCurrencyData(m4char_t const * ai_pItemName, m4char_t *& aio_pCurrency, m4date_t & aio_dExchDate,  m4char_t *& aio_pExchangeType);
	m4return_t SetCurrencyData(m4char_t const * ai_pItemName, m4char_t * ai_pCurrency, m4date_t ai_dExchDate, m4char_t * ai_pExchangeType);

MIT_PUBLIC:

	//Se puede actualizar y consultar a voluntad, pero leer o actualizar este valor a pelo
	//	no tiene ninguna repercusion en el canal.
	//Un Init o End del Item pone el flag Updated del value asociado a FALSE. Cualquier operacion
	//	sobre el Value que afecte al valor lo pondra a TRUE. Un save del item tambien pone
	//	el flag Updated a false.

	ClMIT_ChnValue m_oValue ;

MIT_PUBLIC:
	// ----------------------------- Inicialización---------------------------------------	

	ClMIT_ChnItem();
	virtual ~ClMIT_ChnItem() { ClMIT_ChnItem::End(); }
	virtual size_t GetSizeof() { return sizeof(ClMIT_ChnItem) ; }

	// Al inicializar, se ve de que tipo es el item del canal, y se pone Value a  este 
	//	tipo. Durante la vida de un item de un canal, este puede cambiar de valor, pero 
	//	no de tipo.

	enum INIT_RETURN { ITEM_OK, ITEM_NOT_FOUND, SCOPE_ERROR, ITEM_TYPE_ERROR, 
		VALUE_TYPE_ERROR, ITEM_EXTEND_TYPE_ERROR } ;

	INIT_RETURN Init(ClMIT_ChnNode  * m_poNode, const m4char_t * m_pcNameItem, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR );

	// Este es un grupo de metodos Init para tipos extendidos. Un tipo extendido es uno que
	//	no tiene homologo directo en el canal, y que se gestiona usando y convirtiendo de 
	//	tipos que si tiene el canal.

	INIT_RETURN InitBoolean(ClMIT_ChnNode  * m_poNode, const m4char_t * m_pcNameItem,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR );

	// Constructor copia. Funciona correctamente con items no linkados.
	
	void Init(ClMIT_ChnItem  * ai_poSource ) ;

	// Grupo de InitLoads. Equivalen al Init+Load.
	// Devuelven lo  mismo que load (si es PROPAGATE_ERROR y hay problemas, siempre false).
	// En caso de usar el modo PROPAGATE_ERROR, debe pasarse una variable para almacenar
	//	el valor de retorno.

	m4bool_t InitLoad(ClMIT_ChnNode  * m_poNode, const m4char_t * m_pcNameItem, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR, 
		INIT_RETURN  *ao_Return=NULL);
	m4bool_t InitBooleanLoad(ClMIT_ChnNode  * m_poNode, const m4char_t * m_pcNameItem, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR, 
		INIT_RETURN  *ao_Return=NULL);


	virtual void End() ;

	// Resetea link

	void ResetExternProp( ClMIT_ItemExternProp * ai_poExternProp ) 
		{ m_poExternProp = ai_poExternProp ; }

	// ----------------------------- Actualizaciones--------------------------------------	

	// Load lee el item del canal y almacena el valor en Value. Save graba el valor
	//		Value en el item del canal.  
	// El valor que se trae depende del LoadMode del item.
	// Da FALSE si no hay ningun dato, aunque en cualquier caso siempre el buffer contiene
	//	un valor valido.
	// Para métodos no cargamos el valor, sino que salimos directamente. El valor se debió
	// cargar en el Execute()
	// En estado CANCEL, no hace nada, devuelve M4_FALSE y pone el valor a NULL.

	m4bool_t Load();
	
	void Save();

	//Por defecto siempre se esta en modo ITEM_LOAD
	//  -ITEM_LOAD. Se carga el valor del item.
	//	-SLICE_LOAD. Se carga el valor del slice actual.
	//	-RANGE_SLICE_LOAD. Se carga el valor resultante de los calculos de totalizacion 
	//		ponderados sobre un rango de slices, especificado este por StartDate y EndDate.
	//	-GUIDE_LOAD. Como SLICE_LOAD, solo que el StartDate y EndDate se cojen de 
	//		ExternProps. Si no hay link, se coje el rango de fechas completo.
	//  -TOTALIZE_xxxx. Se carga el valor de totalizacion especificado, o bien el que se
	//		indico en el diseño del cana, si se selecciona TOTALIZE_DESIGN
	//  -TOTALIZE_SLICE_xxxx. Exactamente igual al anterior, pero opera sobre slices. 
	// En estado CANCEL, no hace nada.

	void SetLoadMode( LOAD_MODE ai_LoadMode, m4date_t ai_StartDate=0, m4date_t ai_EndDate=0 ) ;

	LOAD_MODE GetLoadMode() { return m_LoadMode ; }
	m4date_t GetStartDateLoadMode() { return m_StartDate ; }
	m4date_t GetEndDateLoadMode() { return m_EndDate ; }

	m4uint32_t GetTotalizeCount() ;

	// ----------------------------- Slices-----------------------------------------------	
	
	enum OTHERS { SLICE_EOF=-1 } ;

	typedef m4uint32_t SliceIndex_t ;	//Va de 0 a n, SLICE_EOF es la marca de EOF.
	SliceIndex_t m_Slice ;

	//--------------->Rangos del grupo de Slices

	//0 si no tiene slices. Los indices de los slices van de 0 a GetNumSlices()-1

	SliceIndex_t GetNumSlices() ;	

	//Respecto del primer y ultimo slice. 

	m4date_t   GetFirstStartDate() ;	
	m4date_t   GetLastEndDate() ;

	//--------------->Posicionamientos de Slices

	//Cualquier posicionamiento en un slice invalido hara que 
	//	IsSliceEOF de TRUE

	m4bool_t IsSliceEOF() ;
	void  GoFirstSlice() ;	
	void  GoLastSlice() ;
	void  GoNextSlice() ;
	void  GoPrevSlice() ;
	void  MoveToSlice( SliceIndex_t ai_Index ) ;	//Si ai_Index==SLICE_EOF, dejar en EOF
	void  MoveToSlice( m4date_t ai_Date ) ;
	void  MoveToSlice( m4date_t ai_StartDate, m4date_t ai_EndDate ) ;

	//------------->Datos del Slice actual.

	SliceIndex_t GetSliceIndex() ;  //Indice de slice, o SLICE_EOF si esta en EOF
	m4date_t   GetStartDate(SliceIndex_t ai_Index) ;	
	m4date_t   GetEndDate(SliceIndex_t ai_Index) ;
	m4date_t   GetStartDate()  { return GetStartDate(m_Slice) ; }	
	m4date_t   GetEndDate()    { return GetEndDate(  m_Slice) ; }

	// ----------------------------- Acciones sobre items --------------------------------	

	// Hay que retocarlo. Actualmente no admite parametros de entrada ni de salida. 
	// Se hace un pop tras la llamada para chequear el codigo de error.
	// Error local si ejecucion falla.
	// Solo pueden ejecutarse metodos y conceptos.
	// David:
	// Si es método, el resultado de vuelta se obtiene de la pila y se almacena 
	// en su m_oValue. 
	// Tanto si es método como concepto hay que vaciar la pila. En el caso de con-
	// cepto no hay que asignar el resultado a m_oValue porque se hace automática-
	// mente. 	
	// En estado CANCEL, no hace nada.

	void Execute() ; 

	// Si el switch es TRUE y la busqueda no tiene exito, se produce un error
	// NOTA:En estado de CANCEL, ningun find  encontrara nada, pues usan los metodos de 
	//	ClMIT_ChnNode, y estos son inoperantes en estado de cancelado.

	m4bool_t FindDouble(m4double_t ai_dValue, m4bool_t ai_bFromFirst=M4_TRUE, m4bool_t ai_bSwOblig=M4_FALSE ) ;

	m4bool_t FindDate(  m4date_t ai_dDate, m4bool_t ai_bFromFirst=M4_TRUE, m4bool_t ai_bSwOblig=M4_FALSE ) ;

	m4bool_t FindTimeStamp(  m4date_t ai_dDate, m4bool_t ai_bFromFirst=M4_TRUE, m4bool_t ai_bSwOblig=M4_FALSE ) ;

	// Al comparar se obvian blancos finales
	// FindIString, a diferencia de FindString, no es case-sensitive.

	m4bool_t FindString(const m4char_t  *ai_pcValue,  m4bool_t ai_bFromFirst=M4_TRUE, m4bool_t ai_bSwOblig=M4_FALSE ) ;
	m4bool_t FindIString(const m4char_t  *ai_pcValue,  m4bool_t ai_bFromFirst=M4_TRUE, m4bool_t ai_bSwOblig=M4_FALSE ) ;

	m4bool_t FindNextDouble(m4double_t ai_dValue) 
		{ return FindDouble(ai_dValue,M4_FALSE,M4_FALSE) ; }
	m4bool_t FindNextDate(m4date_t ai_dValue) 
		{ return FindDate(ai_dValue,M4_FALSE,M4_FALSE) ; }
	m4bool_t FindNextTimeStamp(m4date_t ai_dValue) 
		{ return FindTimeStamp(ai_dValue,M4_FALSE,M4_FALSE) ; }
	m4bool_t FindNextString(m4pchar_t  ai_pcValue )
		{ return FindString(ai_pcValue,M4_FALSE,M4_FALSE) ; }

	
	m4bool_t LocateDouble(m4double_t ai_dValue)  
		{ return FindDouble(ai_dValue, M4_TRUE, M4_TRUE) ; }
	m4bool_t LocateDate(m4date_t ai_dValue)  
		{ return FindDate(ai_dValue, M4_TRUE, M4_TRUE) ; }
	m4bool_t LocateTimeStamp(m4date_t ai_dValue)  
		{ return FindTimeStamp(ai_dValue, M4_TRUE, M4_TRUE) ; }
	m4bool_t LocateString(m4pchar_t  ai_pcValue) 
		{ return FindString(ai_pcValue, M4_TRUE, M4_TRUE) ; }

	m4bool_t LocateNextDouble(m4double_t ai_dValue)  
		{ return FindDouble(ai_dValue, M4_FALSE, M4_TRUE) ; }
	m4bool_t LocateNextDate(m4date_t ai_dValue)  
		{ return FindDate(ai_dValue, M4_FALSE, M4_TRUE) ; }
	m4bool_t LocateNextTimeStamp(m4date_t ai_dValue)  
		{ return FindTimeStamp(ai_dValue, M4_FALSE, M4_TRUE) ; }
	m4bool_t LocateNextString(m4pchar_t  ai_pcValue) 
		{ return FindString(ai_pcValue, M4_FALSE, M4_TRUE) ; }

	// ----------------------------- Gets ------------------------------------------------	

	TYPE GetType() { return m_Type ; }
	SCOPE GetScope() { return m_Scope ; }
	m4bool_t IsLinked() { return M4_BOOL( m_poNode!=NULL ); }
	ClMIT_ChnNode * GetNode() { return m_poNode; }
	const ClMIT_IdDesc & GetIdDesc() { return m_oIdDesc ; }	//Solo para uso inmediato
	const ClMIT_IdDesc & GetNodeIdDesc()  { return m_poNode->GetIdDesc() ; } 
	const ClMIT_IdDesc & GetChannelIdDesc() {return m_poNode->m_poChn->GetIdDesc() ; }

	m4bool_t UpdateCancel() { return m_poNode->UpdateCancel() ; } 

	// Dan cadenas de uso inmediato, asociadas al modo Load. Usado en Debug.

	static m4char_t * GetStringLoadMode( LOAD_MODE ai_LoadMode ) ;	
	m4char_t * GetStringLoadMode() { return GetStringLoadMode(m_LoadMode) ; }	

	//Devuelve el Id del la TI asociada al Item
	m4pcchar_t GetTiId() ;
	
	//----------------------------------------------------------------------------------

};

#endif


