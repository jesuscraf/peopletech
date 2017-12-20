#ifndef __CSRES_HPP
#define __CSRES_HPP

#include "m4res.hpp"


#define M4_CH_CS_NO_BUILD_PRESENTATION							0x0003 + M4_CH_CS_ERROR_BASE
//Error al construir Presentaci�n.\n \nNo se pudo construir la Presentaci�n "%0:s".\nDebe consultar con el Administrador.\nPropiedades del la presentaci�n "%0:s": perfil "%1:s".
#define M4_CH_CS_BAD_MD_TYPE									0x000A + M4_CH_CS_ERROR_BASE
//Error de petici�n de definici�n de datos.\n \nEl tipo de definici�n de datos "%0:s" es err�neo para la petici�n al servicio.\nDebe consultar con el Administrador.
#define M4_CH_CS_MD_SERVICE_NO_RESPONSE							0x000B + M4_CH_CS_ERROR_BASE
//Error de petici�n de definici�n de datos.\n \nNo se pudo leer la respuesta del servidor del servicio de definici�n de datos.\nDebe consultar con el Administrador.
#define M4_CH_CS_MD_SERVICE_ERROR_DESERIALIZING					0x000C + M4_CH_CS_ERROR_BASE
//Error de petici�n de definici�n de datos.\n \nError deserializando el objeto enviado por el servidor del servicio de definici�n de datos.\nDebe consultar con el Administrador.
#define M4_CH_CS_MD_SERVICE_SERVER_ERROR						0x000D + M4_CH_CS_ERROR_BASE
//Error de petici�n de definici�n de datos.\n \nEl servidor del servicio de definici�n de datos devolvi� error.\nDebe consultar con el Administrador.

//Logon Adaptor
#define M4_CH_CS_NO_SESSION_CHANNEL								0x0010 + M4_CH_CS_ERROR_BASE
// No se puede construir el Meta4Object de la sesi�n\n \nDebe consultar con el administrador.
#define M4_CH_CS_NOT_VALID_ROL_REGISTER							0x0011 + M4_CH_CS_ERROR_BASE
// Perfil no v�lido.\n \nNo existe el perfil n�mero "%0:s".\nDebe elegir un perfil v�lido.
#define M4_CH_CS_INSERT_USER_DATA								0x0012 + M4_CH_CS_ERROR_BASE
// Error insertando datos del usuario "%0:s".\n \nDebe consultar con el administrador.
#define M4_CH_CS_GET_HOSTNAME									0x0013 + M4_CH_CS_ERROR_BASE
// Error del sistema.\n \nNo se puede conseguir el nombre de la m�quina.\nDebe consultar con el administrador.
#define M4_CH_CS_INIT_TCP_IP									0x0014 + M4_CH_CS_ERROR_BASE
// Error del sistema.\n \nNo se puede inicializar el servicio TCP/IP.\nDebe consultar con el administrador.
#define M4_CH_CS_SET_MASK										0x0016 + M4_CH_CS_ERROR_BASE
// No se puede establecer la seguridad del Meta4Object de la sesi�n\n \nDebe consultar con el administrador.
#define M4_CH_MCR_INVALID_ORG_T3								0x0017 + M4_CH_CS_ERROR_BASE
// Permisos insuficentes para construir M4Object.\n \nEl usuario no tiene permisos para construir el M4Object en el contexto de una sociedad. Esto ocurri� para el M4Object %0:s y la sociedad %1:s\nRevise su contexto de seguridad.
#define M4_CH_CS_NO_LOGOFF										0x0018 + M4_CH_CS_ERROR_BASE
// Error cerrando la sesi�n.\n \nSe ha producido un error tecnol�gico al cerrar la sesi�n de la aplicaci�n.\nDebe consultar con el administrador.
#define M4_CH_CS_NO_CHANGE_PASSWORD								0x0019 + M4_CH_CS_ERROR_BASE
// Error cambiando la clave.\n \nSe ha producido un error tecnol�gico al cambiar la clave.\nDebe consultar con el administrador.
#define M4_CH_MCR_INVALID_ROLE_ORG								0x001A + M4_CH_CS_ERROR_BASE
// Perfil y organizaci�n inv�lidos.\n \nLa pareja perfil "%0:s" y organizaci�n "%1:s" no es v�lida.\nDebe consultar con el administrador.
#define M4_CH_MCR_NULL_RSM										0x001B + M4_CH_CS_ERROR_BASE
// El perfil de m�scara vac�o es inv�lido.\n \nDebe consultar con el administrador.
#define M4_CH_MCR_ATTR_TRUNCATED								0x001C + M4_CH_CS_ERROR_BASE
// Atributo de Meta4Object truncado.\n \nEl atributo "%0:s" tienen un valor "%1:s" supera su m�xima longitud "%2:s" y se usar� "%3:s".\nDebe consultar con el administrador.
#define M4_CH_CS_NO_SET_ROLE           							0x001D + M4_CH_CS_ERROR_BASE
// Error asignando el perfil.\n \nSe ha producido un error tecnol�gico al establecer el perfil n�mero "%0:s.\nDebe consultar con el administrador.
#define M4_CH_CS_NO_LOGON           							0x001E + M4_CH_CS_ERROR_BASE
// Error abriendo la sesi�n.\n \nSe ha producido un error tecnol�gico al abrir la sesi�n de la aplicaci�n para el usuario "%0:s".\nDebe consultar con el administrador.
#define M4_CH_CS_NO_WEB_SERVER_NAME								0x001F + M4_CH_CS_ERROR_BASE
// La conexi�n via HTTP no es posible, el usuario debe especificar el nombre del servidor WEB (gateway).
#define M4_CH_CS_NO_NON_SECURE_APP_SERVER_PORT					0x0020 + M4_CH_CS_ERROR_BASE
// La conexi�n via HTTP no es posible, el usuario debe especificar el puerto no seguro del servidor de aplicaciones.


// Debug Info
#define M4_CH_CS_LOGON_NOTIFY									0x0021 + M4_CH_CS_ERROR_BASE
// Traza de apertura de sesi�n.\n \nEl usuario "%0:s" intenta abrir una sesi�n con el servidor "%1:s" por el puerto "%2:s" con el lenguaje "%3:s".
#define M4_CH_CS_LOGOFF_NOTIFY									0x0022 + M4_CH_CS_ERROR_BASE
// Traza de cierre de sesi�n.


// Serialize and deserialize >>>>>>>>>>>>>>>>>>>>>>>>>

#define M4_CS_SERIALIZE_IN_CLIENT                               0x0023 + M4_CH_CS_ERROR_BASE
//Error serializando en cliente.\n \nError enviando datos desde el cliente al servidor.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_DESERIALIZE_IN_CLIENT                             0x0024 + M4_CH_CS_ERROR_BASE
//Error deserializando en cliente.\n \nError recibiendo datos en el cliente desde el servidor.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_SERIALIZE_IN_SERVER                               0x0025 + M4_CH_CS_ERROR_BASE
//Error serializando en server.\n \nError enviando datos desde el servidor al cliente.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_DESERIALIZE_IN_SERVER                             0x0026 + M4_CH_CS_ERROR_BASE
//Error deserializando en server.\n \nError recibiendo datos en el servidor desde el cliente.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_SERIALIZE_METACHANNEL                             0x0027 + M4_CH_CS_ERROR_BASE
//Error en la serializaci�n\n \nError de la estructura del Meta4Object. Estructura Meta4Object "%0:s".\nError enviando datos entre el cliente y el servidor.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_DESERIALIZE_METACHANNEL                           0x0028 + M4_CH_CS_ERROR_BASE
//Error en la deserializaci�n\n \nError de la estructura del Meta4Object. Estructura Meta4Object "%0:s".\nError enviando datos entre el cliente y el servidor.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_SERIALIZE_ACCESS                                  0x0029 + M4_CH_CS_ERROR_BASE
//Error en la serializaci�n del acceso\n \n. Acceso del Meta4Object "%0:s".\nError enviando datos entre el cliente y el servidor.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_DESERIALIZE_ACCESS                                0x002A + M4_CH_CS_ERROR_BASE
//Error en la deserializaci�n\n \nSe produjo en el acceso "%0:s". Acceso del Meta4Object "%0:s".\nError enviando datos entre el cliente y el servidor.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_SERIALIZE_OLTPEXECUTIONCONTEXT                    0x002B + M4_CH_CS_ERROR_BASE
//Error en la serializaci�n del contexto de ejecuci�n.\n \nError enviando datos entre el cliente y el servidor.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_DESERIALIZE_OLTPEXECUTIONCONTEXT                  0x002C + M4_CH_CS_ERROR_BASE
//Error en la deserializaci�n del contexto de ejecuci�n.\n \nError enviando datos entre el cliente y el servidor.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_SERIALIZE_CLIENTSERVERRESULT                      0x002D + M4_CH_CS_ERROR_BASE
//Error en la serializaci�n\n \n Error en el resultado de la ejecuci�n cliente/servidor.\nError enviando datos entre el cliente y el servidor.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_DESERIALIZE_CLIENTSERVERRESULT                    0x002E + M4_CH_CS_ERROR_BASE
//Error en la deserializaci�n\n \nError en el resultado de la ejecuci�n cliente/servidor.\nError enviando datos entre el cliente y el servidor.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_SERIALIZE_OBJECTIDENTIFIER                        0x002F + M4_CH_CS_ERROR_BASE
//Error en la serializaci�n\n \nError en el identificador de Meta4Object.\nError enviando datos entre el cliente y el servidor.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_DESERIALIZE_OBJECTIDENTIFIER                      0x0030 + M4_CH_CS_ERROR_BASE
//Error en la deserializaci�n\n \nError en el identificador de Meta4Object.\nError enviando datos entre el cliente y el servidor.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_OUTDATED_MCR_VERSION                              0x0031 + M4_CH_CS_ERROR_BASE
//La versi�n de un objeto ha cambiado.\n \nDebido a una actualizaci�n del sistema, la versi�n de "%0:s" ha cambiado.\nPor favor, cierre la presentaci�n y vuelva a ejecutarla.
#define M4_CS_NO_CHANNEL                                        0x0032 + M4_CH_CS_ERROR_BASE
//No hay Meta4Object asociado a la transacci�n.\n \nNo se ha encontrado el Meta4Object asociado.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_NO_EXECONTEXT                                     0x0033 + M4_CH_CS_ERROR_BASE
//No hay contexto de ejecuci�n asociado a la transacci�n.\n \nNo se ha encontrado el contexto.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_ERROR_EXECUTE                                     0x0034 + M4_CH_CS_ERROR_BASE
//Error interno ejecutanto m�todo remoto.\n \nNo se ha posido ejecutar el m�todo remoto.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_ERROR_SERIALIZE_ITEMBLOBFILE                      0x0035 + M4_CH_CS_ERROR_BASE
//Error en la serializaci�n de un item de tipo BLOB.\n \nError enviando datos entre el cliente y el servidor.\nDebe comprobar la versi�n de su instalaci�n.
#define M4_CS_ERROR_DESERIALIZE_ITEMBLOBFILE                    0x0036 + M4_CH_CS_ERROR_BASE
//Error en la deserializaci�n de un item de tipo BLOB.\n \nError enviando datos entre el cliente y el servidor.\nDebe comprobar la versi�n de su instalaci�n.

// Serialize and deserialize <<<<<<<<<<<<<<<<<<<<<<<<<

#define M4_CS_ROLL_BACK_INVOQUED_IN_SERVER_SIDE                 0x0037 + M4_CH_CS_ERROR_BASE
//Se ha hecho un "Rollback" en el servidor.\n \nSe ha ejecutado un "RollBack" en el servidor una vez detectado el final de una transacci�n en curso. Se ha ejecutado posiblemente una operaci�n OLTP sin cerrar la transacci�n.\nRevisar la l�gica del c�digo del m�todo remoto.

// Serialize and deserialize >>>>>>>>>>>>>>>>>>>>>>>>>

#define M4_CS_ERROR_SERIALIZE_RECORDSET_IN_M4VMSTATE            0x0038 + M4_CH_CS_ERROR_BASE
//Error en la serializaci�n del estado de ejecuci�n.\n \nNo se ha serializado el conjunto de filas asociado a la ejecuci�n.\nDebe comprobar la configuraci�n cliente servidor.\nEsto ha sucedido en "%0:s" de "%1:s".

// Serialize and deserialize <<<<<<<<<<<<<<<<<<<<<<<<<

#define M4_CH_CS_NO_MD_SERVER_CACHE_REMOVE_BY_ID				0x0039 + M4_CH_CS_ERROR_BASE
//Error al borrar definici�n de datos en el servidor.\n \nNo se pudo borrar definici�n de datos "%0:s" de tipo "%1:s" en el servidor.\nDebe consultar con el Administrador.
#define M4_CH_CS_NO_MD_GET_UPDATE_LIST							0x003A + M4_CH_CS_ERROR_BASE
//Error al obtener la lista de cambios de definici�n de datos.\n \nDebe consultar con el Administrador.

/* En m4cachedirres.hpp
#define M4_CH_CS_NO_MD_CACHE_REFREH								0x003B + M4_CH_CS_ERROR_BASE
//Error al refrescar las caches de definici�n de datos.\n \nDebe consultar con el Administrador.
#define M4_CH_CS_MD_REFRESH_LIST								0x003C + M4_CH_CS_ERROR_BASE
//Error al refrescar la lista de cambios de definici�n de datos.\n \nDebe consultar con el Administrador.
#define M4_CH_CS_MD_LOAD_UPDATE_LIST							0x003D + M4_CH_CS_ERROR_BASE
//Error al cargar la lista de cambios de definici�n de datos.\n \nDebe consultar con el Administrador.
#define M4_CH_CS_MD_GET_REPOSITORY_KEY							0x003E + M4_CH_CS_ERROR_BASE
//Error al obtener la clave de repositorio.\n \nDebe consultar con el Administrador.
#define M4_CH_CS_MD_LAST_METADATA_DATE_BIGGER					0x003F + M4_CH_CS_ERROR_BASE
//La fecha de �ltima actualizaci�n de datos de definici�n es incorrecta.\n \nLa fecha de �ltima actualizaci�n de datos de definici�n es posterior a la del servidor.\nDebe consultar con el Administrador.
*/
#define M4_CS_ERROR_DSITEMBLOBFILE_FILE_MODIFIED				0x0040 + M4_CH_CS_ERROR_BASE
//Error al intentar actualizar el contenido del fichero.\n \nDebe comprobar que la fila, donde se hace referencia al fichero, no se borr� antes de salvar el fichero.\nEsto sucedi� al modificar el fichero "%0:s".

#define M4_CS_MCR_NO_CREATE_M4OBJ								0x0041 + M4_CH_CS_ERROR_BASE
// Meta4Object mal definido.\n \nNo se puede construir el Meta4Object "%0:s" de tipo cliente-servidor "%1:s".\nDebe consultar otros mensajes de error.
//Antiguo #define M4_CH_MCR_NO_CREATE_M4OBJ							0x0040 + M4_CH_MCR_ERROR_BASE
#define M4_CS_MCR_NO_CREATE_SECURITY							0x0042 + M4_CH_CS_ERROR_BASE
// Seguridad mal definida.\n \nNo se puede construir la seguridad de "%0:s" de tipo cliente-servidor "%2:s" para el perfil "%1:s".\nDebe consultar otros mensajes de error.
//Antiguo #define M4_CH_MCR_NO_CREATE_SECURITY						0x0041 + M4_CH_MCR_ERROR_BASE

#define M4_CH_MCR_GET_CONNECT_IN_DIC							0x0043 + M4_CH_CS_ERROR_BASE
//Solicitud de conexi�n no v�lida.\n \nSe est� solicitando una conexi�n en un cliente distribuido.

#define M4_CS_MCR_NO_CREATE_XML									0x0044 + M4_CH_CS_ERROR_BASE
// Error al generar XML.\n \nNo se puede generar la definici�n en XML del Meta4Object "%0:s".\nDebe consultar otros mensajes de error.

#define M4_CS_SERIALIZE_DELETED_L2								0x0045 + M4_CH_CS_ERROR_BASE
//Error al intentar serializar un Meta4Object de vuelta al cliente.\n \nEl Meta4Object se borr� en el servidor. Esto pudo suceder por definir una instancia en cliente y borrarla en el servidor.\nEsto sucedi� al intentar serializar "%0:s".\nDebe comprobar que crea y destruye las instancias en el mismo contexto (cliente o servidor).

#define M4_CS_NO_RESTORE_REMOTE_M4OBJ							0x0046 + M4_CH_CS_ERROR_BASE
//No existen datos en el servidor.\n \nNo se han podido recuperar los datos del Meta4Object "%0:s" de tipo "delta" en el servidor.\nDebe consultar con el administrador.

// Execution Error
#define M4_CH_CS_INVALID_SESSION_NODE							0x0050 + M4_CH_CS_ERROR_BASE
// Nodo inv�lido.\n \nEl identificador de nodo %0:s no es v�lido. Eso ocurri� en el M4Object sesi�n %1:s.\nDebe consultar con el administrador.
#define M4_CH_CS_INVALID_SESSION_ITEM							0x0051 + M4_CH_CS_ERROR_BASE
// Item inv�lido.\n \nEl identificador de item %0:s no es v�lido. Eso ocurri� en el nodo %1:s del M4Object sesi�n %2:s.\nDebe consultar con el administrador.
#define M4_CH_CS_WRONG_LOGON_TYPE								0x0052 + M4_CH_CS_ERROR_BASE
// Tipo de logon incorrecto. El tipo de logon especificado no es un valor correcto. Por defecto, se asume tipo de logon Meta4.

#define M4_CS_NO_OPEN_FILE										0x0053 + M4_CH_CS_ERROR_BASE
// No se puede abrir archivo. No se puede abrir el archivo "%0:s" en modo escritura por la raz�n "%1:s".\nDebe consultar con el administrador.
#define M4_CS_MCR_NO_CREATE_MAP									0x0054 + M4_CH_CS_ERROR_BASE
// Mapeo mal definido.\n \nNo se puede construir el mapeo "%0:s".\nDebe consultar otros mensajes de error.
#define M4_CS_SERIALIZE_PROXY_ERROR								0x0055 + M4_CH_CS_ERROR_BASE
// Error en la serializaci�n\n \nDetectada desincronizaci�n en la cach� delta del Meta4Object proxy "%0:s".\nDebe consultar con el administrador.
#define M4_CS_CANNOT_SET_COMM_LANG								0x0056 + M4_CH_CS_ERROR_BASE
// Error estableciendo idioma\n \nNo se ha podido establecer el idioma con identificador "%0:s" en el m�dulo de comunicaciones.\nDebe consultar con el administrador.
#endif // __CSRES_HPP
