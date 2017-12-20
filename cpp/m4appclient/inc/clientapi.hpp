//## begin module%34E313F800E7.cm preserve=no
//## end module%34E313F800E7.cm

//## begin module%34E313F800E7.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            M4ClientAPI
//	 File:              clientapi.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980313
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%34E313F800E7.cp

//## Module: clientapi%34E313F800E7; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: z:\m4appclient\inc\clientapi.hpp

#ifndef clientapi_h
#define clientapi_h 1

//## begin module%34E313F800E7.additionalIncludes preserve=no
//## end module%34E313F800E7.additionalIncludes

//## begin module%34E313F800E7.includes preserve=yes
#include "m4appclient_dll.hpp"
//#include <communication_dll.hpp>
#define M4_DECL_COMMUNICATION 
#include <m4props.hpp>
#include <m4win.hpp>

// compresion types.
#ifndef M4_COMPRESSION_TYPE_NULL

#   define M4_COMPRESSION_TYPE_NULL  1
#   define M4_COMPRESSION_TYPE_CLASSIC  2
#   define M4_COMPRESSION_TYPE_ZDELTA  3

// #	define M4_COMPRESSION_LEVEL_LOW 1
// #	define M4_COMPRESSION_LEVEL_MEDIUM 2
// #	define M4_COMPRESSION_LEVEL_HIGH 3
#   define M4_COMPRESSION_CLASSIC_DEFAULT_LEVEL  3

#endif //M4_COMPRESSION_TYPE_NULL
//## end module%34E313F800E7.includes

// reqdescriptor
#include <reqdescriptor.hpp>
// clientbasicdef
#include <clientbasicdef.hpp>
// m4types
#include <m4types.hpp>
// appmonitor
#include "appmonitor.hpp"

class ClCCMonitor;
class ClCCConnection;
class ClCCService;
class ClCCConnectionHTTP;
class ClTagList;
class ClLogSystem;
class M4DataStorage;
class ClCommunication;
class ClCommSocketPoolBasicTSAP;

//## begin module%34E313F800E7.declarations preserve=no
//## end module%34E313F800E7.declarations

//## begin module%34E313F800E7.additionalDeclarations preserve=yes
//## end module%34E313F800E7.additionalDeclarations


//## begin ClClientAPI%35050F9600A6.preface preserve=yes
//## end ClClientAPI%35050F9600A6.preface

//## Class: ClClientAPI%35050F9600A6
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3534BFBB0019;m4date_t { -> }
//## Uses: <unnamed>%3769FCED01EF;ClServerEventFunction { -> }
//## Uses: <unnamed>%376A10DE0308;ClCCDistributor { -> }
//## Uses: <unnamed>%3797645900EC;ClCCSession { -> }
//## Uses: <unnamed>%3879FF2501D6;ClTagList { -> F}
//## Uses: <unnamed>%387DA5BD03AD;M4ClCrono { -> }
//## Uses: <unnamed>%387DAB1C0273;M4DataStorage { -> F}
//## Uses: <unnamed>%387DAB4B036B;ClCCConnection { -> F}
//## Uses: <unnamed>%38AD44A8004E;ClCCService { -> F}
//## Uses: <unnamed>%3ADAFEFF0200;ClRequestDescriptor { -> }
//## Uses: <unnamed>%3ADE93EF026E;ClLogSystem { -> F}
//## Uses: <unnamed>%429473C00294;ClCCConnectionHTTP { -> F}

class M4_DECL_M4APPCLIENT ClClientAPI 
{
  //## begin ClClientAPI%35050F9600A6.initialDeclarations preserve=yes
  //## end ClClientAPI%35050F9600A6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClClientAPI%889525732; C++
      //	Constructor por defecto de la clase API de cliente de Aplicaciones.
      ClClientAPI (m4pchar_t ai_pcHostname = NULL, m4int_t ai_iCommsType = M4_ADVANCED_COMMS_TYPE, m4int32_t ai_iControlPort = 6666, m4int32_t ai_nCommWorkers = 1);

    //## Destructor (specified)
      //## Operation: ~ClClientAPI%889525733
      //	Destructor de la clase API de cliente de Aplicaciones.
      //	Esta función se llama justo antes de dejar el CSC, se encarga de realizar la
      //	función inversa a la función CSC_Init, terminando con la ejecución de los
      //	hilos de forma ordenada (liberando la memoria ocupada, etc..). Para que esta
      //	función se ejecute correctamente antes de llamarla se deberían haber cerrado
      //	todas las conexiones activas llamando a la función CSC_Disconnect. En caso
      //	de tener recursos como conexiones abiertas o peticiones pendiente,
      //	dependiendo del flag
      //	· M4_FINISH_MODEL
      //	· M4_SAFE_1_FINISH = 0.- Cerramos borrando todas las conexiones, dejando en
      //	la estadísticas toda la información necesaria para averiguar todo lo que se
      //	está quedando sin borrarse.
      //	· M4_SAFE_0_FINISH = 1.- Cerramos todo pero sin tener volcar nada al  Log.
      //	· M4_KILL_FINISH = 2.- No cerramos nada y devolvemos un error.
      //	Cuando se llame a esta función y se tenga alguna petición pendiente de ser
      //	devuelta o alguna conexión o operación abierta se retornará M4_WARNIG.
      //	Excepto cuando estemos  en M4_KILL_FINISH que devolveremos un M4_SUCCESS ,
      //	debido a que a lo mejor queremos mantener estados de peticiones abiertas en
      //	el servidor.
      ~ClClientAPI ();


    //## Other Operations (specified)
      //## Operation: EnableRole%889525735; C++
      //	NEW!!
      //	This functions allows a user to activate a particular Role in the server so
      //	that new requests can be sent using a new active Role. Once a Role is
      //	enabled it can be "played" until it is explicitly Disabled.
      m4return_t EnableRole (m4handle_t ai_hSessionId, m4pchar_t ai_pRoleId = NULL, m4bool_t ai_bDefault = M4_FALSE);

      //## Operation: OpenService%889525736; C++
      //	Esta función se encarga de abrir  un servicio autenticando al usuario contra
      //	el servicio que desea utilizar. Nos permite comprobar los permisos y crear
      //	la estructura adecuada en el cliente para llevar el control de las
      //	peticiones.
      //	Recibe como parámetros el identificador de la conexión ConId con el SSC, el
      //	nombre del servicio ServiceName  con su tamaño SizeServiceName, y la
      //	información para la apertura del servicio Param . El tiempo máximo de espera
      //	TimeOut se define en la inicialización del API.
      //	En caso de éxito, esta función devuelve al cliente un handle del servicio (ao
      //	_HServID) para ser utilizado en el resto de funciones de envío/recepción del
      //	API. Este handle será unívoco para todo el servidor. Lo podremos construir a
      //	partir del handle interno de la operación y el handle de la sesión que está
      //	utilizando.
      //	Como parámetros tenemos el Identificador  de la conexión contra la que
      //	posteriormente lanzaremos la petición del servicio. En caso de no ser valido
      //	devolveríamos directamente un error.
      //	Cuando vamos a comprobar el tema de los permisos tenemos tres situaciones:
      //	· Tener permiso:  Se genera la estructura para almacenar todos los datos
      //	intermedios que necesitemos para esa operación.
      //	· No tenerlo:  No creamos nada y devolvemos un error.
      //	· No encontrar el permiso para ese servicio en el cliente: Tenemos dos
      //	posibilidades
      //	a) Crear el objeto operación, esperar a la primera petición y entonces
      //	comprobar los permisos de este servicio en caso de no tenerlo devolvemos
      //	error.
      //	b) Hacer una llamada al servidor de comprobación de permisos. En principio
      //	optamos por la primera de las ramas. Hasta ver futuros resultados.
      //	FFS.- En principio no sé si debemos tener en cuenta que la operación sea con
      //	estado o sin estado en este punto de abrir el servicio.
      //	FFS.- La decisión de que mecanismo de comprobación de permisos vamos a
      //	utilizar queda pendiente del diseño de la seguridad y de ver el formato de
      //	los servicios,  permisos, usuarios etc.
      //	Cuando creamos el objeto operación, le colocamos dentro de la sesión.
      //	Inicializamos todos sus parámetros a sus valores iniciales y ponemos la
      //	lista de  peticiones pendientes a cero.
      //	En la cadena de parámetro podemos introducir una serie de flag que pueden
      //	ser interesantes.
      //	· TIME_FOR_OPEN_SERVICE : Tiempo máximo para la apertura del servicio
      //	· SSC_LOG : Volcar las trazas del servidor de comunicaciones. Por defecto no
      //	devolvemos estas trazas.
      //	· SERVICE_LOG : Devolver las trazas generada por el servicio por defecto no
      //	devuelve trazas.
      //	· STATISTIC_SSC: Devolver las estadísticas del servidor de comunicaciones.
      //	Por defecto es que no.
      //	· STATISTIC_SERVICE : Devolver las estadísticas del servicio. El valor por
      //	defecto es: ?
      m4return_t OpenService (m4handle_t ai_hSessionId, m4pchar_t ai_szServiceId, m4handle_t &ao_hContext);

      //## Operation: OpenService%898675801; C++
      //	Esta función se encarga de abrir  un servicio autenticando al usuario contra
      //	el servicio que desea utilizar. Nos permite comprobar los permisos y crear
      //	la estructura adecuada en el cliente para llevar el control de las
      //	peticiones.
      //	Recibe como parámetros el identificador de la conexión ConId con el SSC, el
      //	nombre del servicio ServiceName  con su tamaño SizeServiceName, y la
      //	información para la apertura del servicio Param . El tiempo máximo de espera
      //	TimeOut se define en la inicialización del API.
      //	En caso de éxito, esta función devuelve al cliente un handle del servicio (ao
      //	_HServID) para ser utilizado en el resto de funciones de envío/recepción del
      //	API. Este handle será unívoco para todo el servidor. Lo podremos construir a
      //	partir del handle interno de la operación y el handle de la sesión que está
      //	utilizando.
      //	Como parámetros tenemos el Identificador  de la conexión contra la que
      //	posteriormente lanzaremos la petición del servicio. En caso de no ser valido
      //	devolveríamos directamente un error.
      //	Cuando vamos a comprobar el tema de los permisos tenemos tres situaciones:
      //	· Tener permiso:  Se genera la estructura para almacenar todos los datos
      //	intermedios que necesitemos para esa operación.
      //	· No tenerlo:  No creamos nada y devolvemos un error.
      //	· No encontrar el permiso para ese servicio en el cliente: Tenemos dos
      //	posibilidades
      //	a) Crear el objeto operación, esperar a la primera petición y entonces
      //	comprobar los permisos de este servicio en caso de no tenerlo devolvemos
      //	error.
      //	b) Hacer una llamada al servidor de comprobación de permisos. En principio
      //	optamos por la primera de las ramas. Hasta ver futuros resultados.
      //	FFS.- En principio no sé si debemos tener en cuenta que la operación sea con
      //	estado o sin estado en este punto de abrir el servicio.
      //	FFS.- La decisión de que mecanismo de comprobación de permisos vamos a
      //	utilizar queda pendiente del diseño de la seguridad y de ver el formato de
      //	los servicios,  permisos, usuarios etc.
      //	Cuando creamos el objeto operación, le colocamos dentro de la sesión.
      //	Inicializamos todos sus parámetros a sus valores iniciales y ponemos la
      //	lista de  peticiones pendientes a cero.
      //	En la cadena de parámetro podemos introducir una serie de flag que pueden
      //	ser interesantes.
      //	· TIME_FOR_OPEN_SERVICE : Tiempo máximo para la apertura del servicio
      //	· SSC_LOG : Volcar las trazas del servidor de comunicaciones. Por defecto no
      //	devolvemos estas trazas.
      //	· SERVICE_LOG : Devolver las trazas generada por el servicio por defecto no
      //	devuelve trazas.
      //	· STATISTIC_SSC: Devolver las estadísticas del servidor de comunicaciones.
      //	Por defecto es que no.
      //	· STATISTIC_SERVICE : Devolver las estadísticas del servicio. El valor por
      //	defecto es: ?
      m4return_t OpenService (m4handle_t ai_hSessionId, m4uint32_t ai_ulServiceId, m4handle_t &ao_hContext);

      //## Operation: SyncRequest%889525737; C++
      //	Esta función se encarga de realizar el envío síncrono de peticiones al SSC y
      //	recoger las respuestas que se produzcan, igual que en el caso asíncrono,
      //	salvo que en este caso se queda bloqueado hasta que el servidor devuelve la
      //	respuesta.
      //	Recibe como parámetros el identificador del manejador del servicio HservID,
      //	los flags de control de peticiones con estado OperSTATEFlags que indican si
      //	la petición es:
      //	- la primera de una petición con estado (FIRST_WITH_STATE),
      //	- una intermedia con estado (NEXT_WITH_STATE),
      //	- la última petición con estado (LAST_WITH_STATE),
      //	- ó es sin estado (WITHOUT_STATE).
      //	Para los valores NEXT_WITH_STATE y LAST_WITH_STATE se debe pasar además como
      //	parámetro el RID devuelto en la petición con estado anterior (FIRST_WITH_
      //	STATE ó NEXT_WITH_STATE) en el propio campo RID (es un parámetro de
      //	entrada/salida).
      //	Recibe también los flags de envío en Flags, la carga estimada en Load, los
      //	datos a enviar en claro en DataIn junto con su tamaño SizeDataIn, y los
      //	datos a enviar encriptados en DataCrypIn junto con su tamaño SizeDataCrypIn.
      //	Recibe como parámetro también el tiempo máximo de espera para completar la
      //	operación TimeOut.
      //	Devuelve en caso de éxito la respuesta con los datos en claro en DataOut con
      //	su tamaño, los datos encriptados en DataCrypOut con su tamaño, las trazas en
      //	DataTraceOut con su tamaño y el RID asociado .
      //	FFS .- Queda por ver que se hace con los resultados que vienen después del
      //	TimeOut. Que hacemos con la respuesta cuando llegue, la eliminamos y la
      //	destruimos o esperamos que  la recojan con el receive.
      //	FFS : Pendiente de definir los flags de las llamadas.
      //	El parámetro ai_uiRequestType indica el tipo de PDU que hay que construir y
      //	enviar al servidor (de request, de administración...). Por defecto se
      //	construye una PDU de request..
      m4return_t SyncRequest (m4handle_t ai_hSessionId, m4handle_t ai_hServiceContext, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, M4DataStorage *&ao_pOutDataStore, m4handle_t &ao_hRequestId, m4uint32_t ai_uiTimeout = 00, m4bool_t ai_bLogInfo = M4_FALSE, ClLogSystem *ai_poLog = NULL, ClTimeoutFunction ai_pTimeoutFunction = NULL, ClRequestDescriptor *ao_poReqId = NULL);

      //## Operation: AsyncRequest%889525738; C++
      //	Esta función se encarga de realizar el envío asíncrono de peticiones,
      //	rellenando los campos que forman parte de la cabecera de la PDU. El cliente
      //	construye una petición y la envía al servidor, sin tener en cuenta ni como
      //	ni cuando viene la respuesta. Esta función nos va a devolver un
      //	identificador de la petición enviada, el RequestID (ao_hRID), para poder
      //	recuperar la respuesta posteriormente realizando recepciones de forma
      //	selectiva, basándose en el RID de la petición para la que se desea obtener
      //	su respuesta.
      //	Los datos de la petición van ordenados y clasificados entre cifrados y no
      //	cifrados.
      //	Cuando entramos en la función lo primero que deberíamos hacer es comprobar
      //	que el  Servicio y/u Operación  es correcto.
      //	Los handles que se devuelve siempre son distintos. Con ellos es fácil
      //	resolver a quien está referenciando. Debería ser de alguna composición más o
      //	menos fija con información del exterior .
      //	Una vez conseguido la Operación  y la conexión podríamos conseguir la
      //	conexión que va tirando. Pedimos una PDU de petición, la rellenamos con los
      //	datos del servicio , datos cifrados , datos no cifrados y poco más.
      //	Creamos la petición le pasamos la PDU y la conexión, la (estructura/clase)
      //	que identifica. Se introduce en la cola de envío y se manda al Sender. A
      //	partir de una referencia de la conexión conseguimos el protocolo,  debido a
      //	que cuando se haga la extracción de los datos de entrada tendremos que
      //	descifrar en caso que haya algo que cifrar.
      //	Una vez que llega la petición al Sender recoge el TSAP de la petición y
      //	empieza a volcar en la Red todos los datos, es en este momento donde mete la
      //	marca de tiempo que recoge del objeto conexión (GetServerTime).  En algún
      //	momento el servidor responderá  a la petición hecha.
      //	Los flags de control de peticiones con estado OperSTATEFlags  indican si la
      //	petición es:
      //	- la primera de una petición con estado (FIRST_WITH_STATE),
      //	- una intermedia con estado (NEXT_WITH_STATE),
      //	- la última petición con estado (LAST_WITH_STATE),
      //	- ó es sin estado (WITHOUT_STATE).
      //	Para los valores NEXT_WITH_STATE y LAST_WITH_STATE se debe pasar además como
      //	parámetro el RID devuelto en la petición con estado anterior  (FIRST_WITH_
      //	STATE ó NEXT_WITH_STATE) en el propio campo RID (es un parámetro de
      //	entrada/salida).
      //	Recibe también los flags de envío en Flags , la carga estimada en Load, los
      //	datos a enviar en claro en DataIn junto con su tamaño SizeDataIn y los datos
      //	a enviar encriptados en DataCrypIn junto con su tamaño SizeDataCrypIn.
      //
      //	El parámetro ai_uiRequestType indica el tipo de PDU que hay que construir y
      //	enviar al servidor (de request, de administración...). Por defecto se
      //	construye una PDU de request..
      m4return_t AsyncRequest (m4handle_t ai_hSessionId, m4handle_t ai_hServiceContext, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, m4handle_t &ao_hRequestId, m4bool_t ai_bLogInfo = M4_FALSE);

      //## Operation: ReceiveRequest%889525739; C++
      //	Esta función se encarga de la recepción de las respuestas del SSC.
      //	Recibe como parámetro el identificador del servicio ServId, el tiempo máximo
      //	de espera para completar la operación TimeOut, el identificador
      //	correspondiente a la petición de la que se desea recibir la respuesta RID y
      //	las opciones de recepción solicitadas en ReceiveFlags:
      //	1. Para recibir cualquier respuesta de las peticiones sin estado pendientes
      //	se le pasaría GET_ANY_WITHOUT_STATE y devolvería el RID correspondiente a la
      //	petición que se responde.
      //	2. Para recibir una respuesta a una petición concreta el valor sería  GET_
      //	THIS y el parámetro RID identificaría la petición, permitiendo recepciones
      //	selectivas.
      //	Devuelve en caso de éxito la respuesta con los datos en claro en DataOut con
      //	su tamaño, los datos encriptados en DataCrypOut con su tamaño, las trazas en
      //	DataTraceOut con su tamaño. Si se eligió la opción de recibir cualquiera de
      //	las respuestas pendientes devuelve además el identificador asociado en RID .
      //	Como parámetro de salida aparte de tener una clase de almacenamiento
      //	devuelve el  handle de la petición  satisfecha (ao_hRID) ya que puede ser
      //	interesante porque podemos querer leer cualquier petición que quede
      //	pendiente de recoger, con lo cual con handle sabemos cual es el que hemos
      //	cogido .
      m4return_t ReceiveRequest (m4handle_t ai_hSessionId, m4handle_t ai_hRequestId, M4DataStorage *&ao_pOutDataStore, m4int32_t ai_lTimeOut, m4int16_t ai_lReceiveFlag, ClLogSystem *ai_poLog = NULL);

      //## Operation: CancelRequest%889525740; C++
      //	Esta función está orientada a poder eliminar una petición el servidor si
      //	todavía no está ejecutada del todo. Cuando se hace una llamada a esta
      //	petición se le pasa un handle de petición. Este me indica cual es la
      //	petición que tengo que cancelar.
      //	Mando al servidor la cancelación y me puede devolver dos posibles resultados.
      //	Si me devuelve éxito quiere decir que se ha cancelado. En este caso
      //	desbloqueo los posibles  bloqueos existentes.
      //	Si me devuelve fracaso, quiere decir que no se ha podido cancelar porque
      //	probablemente esté ya en curso la petición.
      //
      //	FFS.- Este tema está muy verde, a falta de definir un poco más la
      //	arquitectura cliente servidor.
      m4return_t CancelRequest (m4handle_t ai_hSessionId, m4handle_t ai_hServiceId, m4handle_t ai_hRequestId, m4int32_t ai_uiTimeout = TIMEOUT_ADMIN_SERV, m4bool_t ai_bSeverity = M4_FALSE);

      //## Operation: CloseService%889525741; C++
      //	Esta función se encarga de cerrar un servicio previamente creado con CSC_Open
      //	Service. A partir de su cierre ya no se podrán enviar más peticiones a
      //	través del manejador del servicio.
      //	Recibe como parámetro el identificador del manejador del servicio ai_hServID
      //	y las opciones de cierre del servicio en ai_strParam.
      //	En este caso las peticiones pendientes de respuestas podrían ser descartadas
      //	.
      //	En el caso de estar con estados además de eliminar los datos temporales
      //	relacionados con la operación, si se da el caso, debemos enviar al SSC una
      //	petición con la marca de última petición. Con lo cual el servidor se entera
      //	que tiene que eliminar todos los datos que tenga pendiente.
      //	· M4_CLOSE_SERVICE_TYPE : Tipo de desconexión
      //	· M4_DROPPED : No me puedo volver a reconectar y mantener el mismo estado
      //	que tenía.
      //	· M4_CLOSED : Cerramos pero permitimos que se pueda reabrir la conexión. No
      //	se borra la conexión cuando se llega a cero.
      //	· M4_DISCONNECT_TIMEOUT : Tiempo máximo para que una vez que se haya
      //	realizado una desconexión con M4_CLOSED y el contador de referencia haya
      //	llegado a 0 mantenga la conexión abierta. Pasado ese tiempo en cualquier
      //	momento se puede desconectar la conexión y eliminar todo el estado que se
      //	tenía en esta sesión.
      //	Cuando queremos cerrar el servicio hay que contrastar con las opciones del
      //	disconnect.
      m4return_t CloseService (m4handle_t ai_hSessionId, m4handle_t ai_hContext);

      //## Operation: DisableRole%889525742; C++
      //	NEW!!
      //	This functions disbles an active Role in the server for that session.
      m4return_t DisableRole (m4handle_t ai_hSessionId, m4pchar_t ai_pRoleId);

      //## Operation: OpenSession%889525745; C++
      //	Esta función permite realizar el proceso de conexión del CSC con el SSC.
      //	Sus parámetros de entrada son la dirección remota oTAddress y la
      //	identificación del usuario UserId/Password que utiliza esta conexión.
      //	También puede recibir otros parámetros adicionales en Param. El tiempo
      //	máximo de espera para completar la conexión TimeOut se definiría en la
      //	inicialización del API.
      //	Devuelve en caso de éxito un identificador de la conexión ya autenticada Con
      //	Id para ser utilizado en posteriores llamadas a las funciones del API.
      //	Si el mismo usuario (nombre/password) llama varias veces (por ej. en
      //	distintos threads) a esta función para conectarse con el mismo servidor se
      //	creará, en la primera llamada, una nueva conexión y en las restantes
      //	llamadas NO se crearán nuevas conexiones sino que se compartirá de forma
      //	automática la primera conexión que se creó. (Se devolverá un identificador
      //	(ai_hConId) correspondiente a dicha conexión y se incrementará internamente
      //	el número de usuarios asociados a la misma para saber cuando dicha conexión
      //	ha de ser desconectada).
      //	FFS.- Falta concretar los parámetros y flags para esta función. ( Flag para
      //	permitir la conexión o no conexión, nivel de seguridad, etc. )
      //	FFS.- Falta especificar la actualización de cache, en el que se está
      //	avanzando.
      m4return_t OpenSession (M4ClString &ai_strAddress, m4uint32_t ai_uiPort, M4ClString &ai_strUsername, M4ClString &ai_strPasswd, m4bool_t ai_bDefaultRole, m4handle_t &ao_hSessionId, ClTagList *ao_oArgs = NULL, m4bool_t ai_bUseSSL = M4_FALSE, M4ClString ai_strCACert = "");

      //## Operation: ChangePassword%892638101; C++
      m4return_t ChangePassword (m4handle_t ai_hSessionId, M4ClString &ai_oOldPasswd, M4ClString &ai_oNewPasswd);

      //## Operation: CloseSession%889525746; C++
      //	Esta función se encarga de cerrar la conexión con el SSC que indica el
      //	primer parámetro ai_hConId (la conexión ha sido abierta previamente con CSC_
      //	Connect).
      //	Tenemos dos posibilidades: que la sesión se destruya directamente o que
      //	tenga un contador de referencia (debido a la posibilidad de utilizar la
      //	misma conexión para varios usuarios). En este caso, la conexión sólo será
      //	destruída realmente cuando el último usuario asociado con la conexión llame
      //	a la función CSC_Disconnect, en el resto de llamadas solamente se
      //	decrementará el número de usuarios asociados.
      //	Cuando se va a destruir la sesión, se debe desconectar el TSAP en caso de no
      //	estar desconectado ya previamente. Es posible que en la desconexión
      //	necesitemos mandar algún tipo de confirmación o seguir algún tipo de
      //	protocolo . Y se debe liberar todos los objetos relacionados y en propiedad
      //	de la sesión.
      //	En caso de que hagamos una  desconexión y se tiene operaciones o
      //	peticiones,  devolvería un M4_WARNING  y volcaría a traza toda la
      //	información de lo pendiente. En caso de tener las estadísticas activadas
      //	almacenaríamos todas las estadísticas relacionadas con la conexión.
      //	En la desconexión también tenemos un tipo de desconexión momentánea, es
      //	decir, se cierra ahora pero se va a querer abrir en un cierto tiempo. Este
      //	tipo de conexiones se utiliza en el Web
      //	Dependiendo del parámetro que tengamos al desconectarnos.
      //	M4_CLOSED_LEVEL : Que tipo de cierre tenemos
      //	· M4_DROPPED = 0:  Se borra la conexión cuando se llegue a cero. Esta es la
      //	opción por defecto.
      //	· M4_CLOSE  = 1: Se cierra la conexión y se deja  pendiente de borrar un
      //	cierto tiempo el indicado en el TimeOut.
      //	M4_DISCONNECT_TIMEOUT : Tiempo máximo que se mantiene la conexión abierta
      //	estando el contador de referencia a cero. Esta opción sólo tiene sentido
      //	cuando se quiere cerrar la conexión pero no se quiere eliminar la conexión
      //	en el servidor. Con este parámetro determinamos cuanto tiempo debe estar
      //	abierta la conexión en el servidor .
      m4return_t CloseSession (m4handle_t ai_hSessionId);

      //## Operation: RemoveRequest%899719991
      m4return_t RemoveRequest (m4handle_t ai_hSessionId, m4handle_t ai_hServiceId, m4handle_t ai_hRequestId);

      //## Operation: SetTimeoutFunction%911845632; C++
      m4return_t SetTimeoutFunction (ClTimeoutFunction ai_pTimeoutFunction);

      //## Operation: GetTimeoutFunction %911845633; C++
      m4return_t GetTimeoutFunction (ClTimeoutFunction &ao_pTimeoutFunction);

      //## Operation: Ping%927272837; C++
      m4return_t Ping (M4ClString &ai_strAddress, m4uint32_t ai_uiPort, ClTagList *ao_oArgs = NULL, m4bool_t ai_bUseSSL = M4_FALSE, M4ClString ai_strCACert = "");

      //## Operation: SetServerEventFunction%929691988; C++
      m4return_t SetServerEventFunction (ClServerEventFunction ai_ServerEventFunction);

      //## Operation: OpenSession%942079100; C++
      //	Esta función permite realizar el proceso de conexión del CSC con el SSC.
      //	Sus parámetros de entrada son la dirección remota oTAddress y la
      //	identificación del usuario UserId/Password que utiliza esta conexión.
      //	También puede recibir otros parámetros adicionales en Param. El tiempo
      //	máximo de espera para completar la conexión TimeOut se definiría en la
      //	inicialización del API.
      //	Devuelve en caso de éxito un identificador de la conexión ya autenticada Con
      //	Id para ser utilizado en posteriores llamadas a las funciones del API.
      //	Si el mismo usuario (nombre/password) llama varias veces (por ej. en
      //	distintos threads) a esta función para conectarse con el mismo servidor se
      //	creará, en la primera llamada, una nueva conexión y en las restantes
      //	llamadas NO se crearán nuevas conexiones sino que se compartirá de forma
      //	automática la primera conexión que se creó. (Se devolverá un identificador
      //	(ai_hConId) correspondiente a dicha conexión y se incrementará internamente
      //	el número de usuarios asociados a la misma para saber cuando dicha conexión
      //	ha de ser desconectada).
      //	FFS.- Falta concretar los parámetros y flags para esta función. ( Flag para
      //	permitir la conexión o no conexión, nivel de seguridad, etc. )
      //	FFS.- Falta especificar la actualización de cache, en el que se está
      //	avanzando.
      m4return_t OpenSession (M4ClString &ai_strAddress, m4uint32_t ai_uiPort, M4ClString &ai_strCredential, m4handle_t &ao_hSessionId, ClTagList *ao_oArgs = NULL, m4bool_t ai_bUseSSL = M4_FALSE, M4ClString ai_strCACert = "");

      //## Operation: Init%947519631; C++
      //	Esta función se encarga de realizar las tareas de inicialización del CSC.
      //	Permitirá generar todos los objetos globales con sus correspondientes
      //	constantes y a establecer una serie de opciones por defecto. Se le pasa como
      //	argumento un objeto que contiene una lista de pares Atributo = Valor con los
      //	parámetros de inicialización . El conjunto de variables y constantes irá
      //	creciendo a lo largo del tiempo pero en una primera versión aparecen estas:
      //	· ID_SITE : Esta será la variable que identifica el puesto que tenemos.
      //	· PRIVATE_KEY_SITE : Clave privada del puesto.
      //	· GLOBAL_TIMEOUT_FOR_CONNECT : Por defecto es 60 segundos. Si queremos
      //	cambiar el valor por defecto usaremos esta variable.
      //	· GLOBAL_TIMEOUT_FOR_OPEN_SERVICE : Por defecto 60 segundos. Si queremos
      //	cambiar el valor por defecto usaremos esta variable.
      //	· GLOBAL_LOG_SYSTEM : Decidimos si queremos tener activo el sistema de log o
      //	no.  Por defecto se dice que no va a haber sistema de log.
      //	· GLOBAL_STATISTIC: Si se mantienen las estadísticas o no.
      //	Solo se puede llamar a esta función una vez, si se llama más veces debería
      //	devolver errores.
      m4return_t Init (m4uint32_t ai_uiControlTimeout = TIMEOUT_ADMIN_SERV);

      //## Operation: GetProps%947519633
      //	Obtiene los args para un mapa de tags para una sesion dada devolviendo M4_
      //	SUCESS.
      //
      //	En el caso en que no pueda encontrar alguna de las
      //	tags devolvera M4_WARNING y pondra el correspondiente Data.PointerData a
      //	NULL.
      m4return_t GetProps (m4handle_t ai_hSessionId, ClTagList *ao_cTagMap);

      //## Operation: SetProps%947519634
      //	Establece los args de las tags  contenidas en el mapa para una sesion dada,
      //	devolviendo M4_SUCESS.
      //
      //	En el caso en que no pueda establecer alguna devuelve M4_WARNING y pone a
      //	NULL el Data.PointerData correspondiente.
      m4return_t SetProps (m4handle_t ai_hSessionId, ClTagList *ao_cTagMap);

      //## Operation: SetControlTimeout%947519635; C++
      m4return_t SetControlTimeout (m4uint32_t ai_uiControlTimeout);

      //## Operation: GetControlTimeout%947519636; C++
      m4return_t GetControlTimeout (m4uint32_t &ao_uiControlTimeout);

      //## Operation: GetError%947519637; C++
      //	En caso de error esta función devuelve una información más pormenorizada
      //	sobre la causa del mismo . Se podría hacer que la propia función de error
      //	generará la memoria que ocupa la cadena de error (devolviendo un char**).
      void GetError (m4handle_t ai_hSessionId, m4return_t ai_uiError, M4ClString &ao_strDescription);

      //## Operation: GetComms%962005377
      static ClCommunication * GetComms ();

      //## Operation: SetCommsType%972486058; C++
      m4return_t SetCommsType (m4int_t ai_iCommsType);

      //## Operation: OpenSession%1023207538; C++
      //	Esta función permite realizar el proceso de conexión del CSC con el SSC.
      //	Sus parámetros de entrada son la dirección remota oTAddress y la
      //	identificación del usuario UserId/Password que utiliza esta conexión.
      //	También puede recibir otros parámetros adicionales en Param. El tiempo
      //	máximo de espera para completar la conexión TimeOut se definiría en la
      //	inicialización del API.
      //	Devuelve en caso de éxito un identificador de la conexión ya autenticada Con
      //	Id para ser utilizado en posteriores llamadas a las funciones del API.
      //	Si el mismo usuario (nombre/password) llama varias veces (por ej. en
      //	distintos threads) a esta función para conectarse con el mismo servidor se
      //	creará, en la primera llamada, una nueva conexión y en las restantes
      //	llamadas NO se crearán nuevas conexiones sino que se compartirá de forma
      //	automática la primera conexión que se creó. (Se devolverá un identificador
      //	(ai_hConId) correspondiente a dicha conexión y se incrementará internamente
      //	el número de usuarios asociados a la misma para saber cuando dicha conexión
      //	ha de ser desconectada).
      //	FFS.- Falta concretar los parámetros y flags para esta función. ( Flag para
      //	permitir la conexión o no conexión, nivel de seguridad, etc. )
      //	FFS.- Falta especificar la actualización de cache, en el que se está
      //	avanzando.
      m4return_t OpenSession (M4ClString &ai_strAddress, m4uint32_t ai_uiPort, M4ClString &ai_strUsername, M4ClString &ai_strPasswd, m4bool_t ai_bDefaultRole, m4handle_t &ao_hSessionId, m4pchar_t &ao_strM4X509Cert, m4uint32_t &aoi_iM4X509CertBufLeN, ClTagList *ao_oArgs, m4bool_t  ai_bUseSSL, M4ClString ai_strCACert);

      //## Operation: OpenSession%1023207539; C++
      //	Esta función permite realizar el proceso de conexión del CSC con el SSC.
      //	Sus parámetros de entrada son la dirección remota oTAddress y la
      //	identificación del usuario UserId/Password que utiliza esta conexión.
      //	También puede recibir otros parámetros adicionales en Param. El tiempo
      //	máximo de espera para completar la conexión TimeOut se definiría en la
      //	inicialización del API.
      //	Devuelve en caso de éxito un identificador de la conexión ya autenticada Con
      //	Id para ser utilizado en posteriores llamadas a las funciones del API.
      //	Si el mismo usuario (nombre/password) llama varias veces (por ej. en
      //	distintos threads) a esta función para conectarse con el mismo servidor se
      //	creará, en la primera llamada, una nueva conexión y en las restantes
      //	llamadas NO se crearán nuevas conexiones sino que se compartirá de forma
      //	automática la primera conexión que se creó. (Se devolverá un identificador
      //	(ai_hConId) correspondiente a dicha conexión y se incrementará internamente
      //	el número de usuarios asociados a la misma para saber cuando dicha conexión
      //	ha de ser desconectada).
      //	FFS.- Falta concretar los parámetros y flags para esta función. ( Flag para
      //	permitir la conexión o no conexión, nivel de seguridad, etc. )
      //	FFS.- Falta especificar la actualización de cache, en el que se está
      //	avanzando.
      m4return_t OpenSession (M4ClString &ai_strAddress, m4uint32_t ai_uiPort, m4pchar_t ai_strX509Cert, m4uint32_t ai_iX509CertLen, m4handle_t &ao_hSessionId, m4pchar_t &ao_strOutSessionContext, m4uint32_t ai_iOutSessContextBufLen, m4pchar_t ai_strSessionContext, ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, M4ClString ai_strCACert);

      //## Operation: GetSessContext%1023207540
      m4return_t GetSessContext (m4handle_t ai_hSessionId, m4pchar_t &  ao_strX509Cert, m4uint32_t &  aoi_iX509CertBufLen, m4pchar_t &  ao_strSessionContext, m4uint32_t &  aoi_iSessContextBufLen);

      //## Operation: _StartSession%1105443234
      m4return_t _StartSession (m4uchar_t *ai_szCertificate, m4uint32_t ai_CertificateLen, m4uchar_t *ai_szSessContext, m4bool_t ai_bDefaultRole, m4handle_t &ao_hSessionId, ClCCConnection *ai_poConnection, ClTagList *ao_oArgs);

      //## Operation: ConnectRequest%1105443233
      m4return_t ConnectRequest (M4ClString &aio_strHostName, m4uint32_t &aio_uiPort, ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, const M4ClString &ai_strCACert);

	  m4return_t SetCommsLanguage( m4language_t ai_iLang ) ;

	  void		 NotifyVMExecution(m4handle_t ai_hSessionId, m4bool_t ai_bIsStart);
    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%387DBC2800FC
      //## Role: ClClientAPI::m_pClientMonitor%387DBC28032D
      //## begin ClClientAPI::m_pClientMonitor%387DBC28032D.role preserve=no  public: ClCCMonitor { -> RFHAN}
      ClCCMonitor *m_pClientMonitor;
      //## end ClClientAPI::m_pClientMonitor%387DBC28032D.role

      //## Association: M4AppClient::<unnamed>%3A06D05A0391
      //## Role: ClClientAPI::m_poTSAPPool%3A06D05B0270
      //## begin ClClientAPI::m_poTSAPPool%3A06D05B0270.role preserve=no  public: static ClCommSocketPoolBasicTSAP { -> RFHAN}
      static ClCommSocketPoolBasicTSAP *m_poTSAPPool;
      //## end ClClientAPI::m_poTSAPPool%3A06D05B0270.role

    // Additional Public Declarations
      //## begin ClClientAPI%35050F9600A6.public preserve=yes
      //## end ClClientAPI%35050F9600A6.public

  protected:

    //## Other Operations (specified)
      //## Operation: _StartComunication%947519638; C++
      m4return_t _StartComunication (M4ClString &ai_strAddress, m4uint32_t ai_uiPort, ClCCConnection * &ao_poConnection, ClTagList *ao_oArgs = NULL, m4bool_t ai_bUseSSL = M4_FALSE, M4ClString ai_strCACert = "");

      //## Operation: _StartSession%947519639; C++
      m4return_t _StartSession (M4ClString &ai_strUsername, m4pchar_t ai_strPasswd, m4uint32_t ai_iPasswdLen, m4bool_t ai_bDefaultRole, m4handle_t &ao_hSessionId, ClCCConnection *  ai_poConnection, ClTagList *ao_oArgs = NULL);

      //## Operation: _PrepareRequest%948190724; C++
      m4return_t _PrepareRequest (m4handle_t ai_hSessionId, m4handle_t ai_hServiceContext, m4handle_t ai_hRequestID, ClCCService * &ao_pService, m4bool_t &ai_bLogInfo, ClLogSystem * &ai_poLog);

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%34E858D3028F
      //## Role: ClClientAPI::m_pszHostName%34E858D3031B
      //## begin ClClientAPI::m_pszHostName%34E858D3031B.role preserve=no  protected: static m4pchar_t {1 -> 1VHAN}
      static m4pchar_t m_pszHostName;
      //## end ClClientAPI::m_pszHostName%34E858D3031B.role

      //## Association: M4AppClient::<unnamed>%392E625B00ED
      //## Role: ClClientAPI::m_pEventFunction%392E625B0364
      //## begin ClClientAPI::m_pEventFunction%392E625B0364.role preserve=no  protected: ClServerEventFunction { -> VHAN}
      ClServerEventFunction m_pEventFunction;
      //## end ClClientAPI::m_pEventFunction%392E625B0364.role

      //## Association: M4AppClient::<unnamed>%395708690011
      //## Role: ClClientAPI::m_poComms%3957086A018F
      //## begin ClClientAPI::m_poComms%3957086A018F.role preserve=no  protected: static ClCommunication { -> RFHAN}
      static ClCommunication *m_poComms;
      //## end ClClientAPI::m_poComms%3957086A018F.role
#ifdef _WINDOWS
	  ClAppMonitor * m_pAppMonitor;
#endif
    // Additional Protected Declarations
      //## begin ClClientAPI%35050F9600A6.protected preserve=yes
      //## end ClClientAPI%35050F9600A6.protected

  private:
    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%387A004E0182
      //## Role: ClClientAPI::m_uiControlTimeout%387A004E0377
      //## begin ClClientAPI::m_uiControlTimeout%387A004E0377.role preserve=no  private: m4uint32_t { -> UHAN}
      m4uint32_t m_uiControlTimeout;
      //## end ClClientAPI::m_uiControlTimeout%387A004E0377.role

    // Additional Private Declarations
      //## begin ClClientAPI%35050F9600A6.private preserve=yes
      //## end ClClientAPI%35050F9600A6.private

  private: //## implementation
    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%3659A5FF035D
      //## Role: ClClientAPI::m_pTimeoutFunction%3659A60001E2
      //## begin ClClientAPI::m_pTimeoutFunction%3659A60001E2.role preserve=no  implementation: ClTimeoutFunction {1 -> 1UHAN}
      ClTimeoutFunction m_pTimeoutFunction;
      //## end ClClientAPI::m_pTimeoutFunction%3659A60001E2.role

    // Additional Implementation Declarations
      //## begin ClClientAPI%35050F9600A6.implementation preserve=yes
      //## end ClClientAPI%35050F9600A6.implementation

};

//## begin ClClientAPI%35050F9600A6.postscript preserve=yes
//## end ClClientAPI%35050F9600A6.postscript

// Class ClClientAPI 

//## begin module%34E313F800E7.epilog preserve=yes
//## end module%34E313F800E7.epilog


#endif
