#ifndef __DMRES_HPP
#define __DMRES_HPP

#include "m4res.hpp"

#define	M4LDB_CERROR_PROPERTY_NOT_FOUND             0x0001 + M4_CH_DM_ERROR_BASE // cambiar a MCR
//No se usa.
#define	M4_CH_MCR_NO_START_UP                       0x0003 + M4_CH_DM_ERROR_BASE // cambiar a dm
//No se puede conectar con la base de datos.\n \nDebe consultar con el administrador.
#define M4_CH_DM_CHNOTBUILT                         0x0004 + M4_CH_DM_ERROR_BASE
//Meta4Object no generado.\n \nNo se puede usar un Meta4Object que no se haya generado primero.\nDebe revisar los demás errores de la pila.
#define M4_CH_DM_SETCHECKPOINTMODE                  0x0005 + M4_CH_DM_ERROR_BASE
//Punto de control no válido.\n \nNo se puede definir el modo del punto de control "%0:s".\nDebe consultar con el administrador.\nEsto sucedió en "%1:s".
#define M4_CH_DM_DESTROYWACCESS                     0x0006 + M4_CH_DM_ERROR_BASE
//Destrucción de un Meta4Object con referencias.\n \Intento de destrucción "%0:s" al que aún se hace referencia "%1:s" veces.\nDebe revisar los demás errores de la pila.
#define M4_CH_DM_BUILDFROMFILE                      0x0007 + M4_CH_DM_ERROR_BASE
//Meta4Object no definido.\n \nError al definir un Meta4Object a partir del archivo: "%0:s".\nDebe revisar los demás errores de la pila.
#define M4_CH_DM_ENVIROMENTNULL                     0x0008 + M4_CH_DM_ERROR_BASE
//El entorno Meta4Object es nulo.\n \nDebe consultar al administrador.
#define M4_CH_DM_FACTORYNULL                        0x0009 + M4_CH_DM_ERROR_BASE
//La fábrica de entorno es nula.\n \nDebe consultar con el administrador.
#define M4_CH_DM_EXECUTINGITEM                      0x000B + M4_CH_DM_ERROR_BASE
//Elemento inexistente.\n \nIntento de ejecución de un elemento que no existe en "%0:s" de "%1:s".\nDebe revisar la definición del Meta4Object.
#define M4_CH_DM_BADITEMTYPE                        0x000D + M4_CH_DM_ERROR_BASE
//Elemento no válido.\n \nIntento de ejecución "%0:s" que no es un método ni un concepto.\nDebe revisar la definición del Meta4Object.\nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_MOVETOEOF                          0x000E + M4_CH_DM_ERROR_BASE
//Movimiento no válido.\n \nIntento de desplazamiento al elemento inexistente "%0:s".\nDebe revisar la definición del Meta4Object.\nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_ITEMOUTOFRANGE                     0x000F + M4_CH_DM_ERROR_BASE
//Intento de utilización un elemento que no existe.\n \nDebe revisar la definición del Meta4Object.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_REGISTERISEOF                      0x0010 + M4_CH_DM_ERROR_BASE
//Intento de realización de una operación en una fila no válida.\n \nDebe revisar las operaciones realizadas anteriormente realizadas en el Meta4Object.\nEsto sucedió en "%0:s" de %1:s".
#define M4_CH_DM_NIHILBLOCK                         0x0011 + M4_CH_DM_ERROR_BASE
//Intento de realización de una operación en un conjunto de filas no válido.\n \nDebe comprobar que el nodo padre está colocado correctamente o tiene filas.
#define M4_CH_DM_INVALIDSCOPE                       0x0015 + M4_CH_DM_ERROR_BASE
//Se encontró un alcance de elemento no válido.\n \nAlcance del elemento "%0:s".\nDebe revisar la definición del Meta4Object.
#define M4_CH_DM_INVALIDAUTOLOADMODE                0x0020 + M4_CH_DM_ERROR_BASE
//Intento de definición de un modo de carga automática no válido.\n \nModo "%0:s" en "%1:s".\nDebe revisar la definición del Meta4Object.
#define M4_CH_DM_AUTOLOADMANDATORY                  0x0021 + M4_CH_DM_ERROR_BASE
//Conflicto entre modos de carga automática.\n \nEl modo en Meta4Object es "%0:s" e intenta definir un modo incompatible "%1:s" para el nodo "%2:s".\nDebe revisar la definición del Meta4Object.\nEsto sucedió en "%3:s".
#define M4_CH_DM_CACHE_PUT_CHANNEL					0x0039 + M4_CH_DM_ERROR_BASE
//""%0:s" no se puede almacenar en la caché.\n \nDebe consultar al administrador.
#define M4_CH_DM_SLICEISEOF                         0x003A + M4_CH_DM_ERROR_BASE
//Intento de realización de una operación en un tramo no válido.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedió en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_DATEOUTOFRANGE                     0x003B + M4_CH_DM_ERROR_BASE
//Intento de desplazamiento a la fecha no válida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedió con la fecha "%0:s" en %0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_ITEMWITHOUTSLICES                  0x003D + M4_CH_DM_ERROR_BASE
//Intento de realización de una operación de tramo en un elemento sin tramos.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedió en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_MOVETOEOF_CNB                      0x0044 + M4_CH_DM_ERROR_BASE
//El elemento de ID "%0:s" no existe.\n \nInformación de contexto no disponible porque porque no se creó el Meta4Object.\nDebe revisar los demás errores de la pila.
#define M4_CH_DM_MOVETOEOF_S                        0x0045 + M4_CH_DM_ERROR_BASE
//El elemento de ID "%0:s" no existe.\n \nDebe revisar la definición del Meta4Object. \nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_MOVETOEOF_S_CNB                    0x0046 + M4_CH_DM_ERROR_BASE
//El elemento de ID "%0:s" no existe.\n \nInformación de contexto no disponible porque porque no se creó el Meta4Object.\nDebe revisar los demás errores de la pila.
#define M4_CH_DM_PARTIAL_LOAD_END                   0x0048 + M4_CH_DM_ERROR_BASE
//Intento de desplazamiento a la última fila de un nodo de carga parcial.\n \nDebe revisar la definición del Meta4Object.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_PARTIAL_LOAD_PL                    0x004A + M4_CH_DM_ERROR_BASE
//Error al cargar un conjunto de filas en carga parcial.\n \nDebe revisar los demás errores de la pila.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_PARTIAL_LOAD_NMRTL                 0x004C + M4_CH_DM_ERROR_BASE
//No hay más filas para carga parcial.\n \nDebe revisar la definición del Meta4Object.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_REGISTERISDELETED                  0x004D + M4_CH_DM_ERROR_BASE
//Intento de asignación de un valor a una fila suprimida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedió en la fila "%0:s" en "%1:s" de "%2:s" de "%3:s".
#define M4_CH_DM_AUTOLOAD_ERROR                     0x004F + M4_CH_DM_ERROR_BASE
//Error en carga automática.\n \nDebe revisar los demás errores de la pila.\nEsto sucedió en "%1:s" de "%2:s" con tipo de carga automática "%0:s".
#define M4_CH_DM_CHILDNODEDONTEXIST                 0x0051 + M4_CH_DM_ERROR_BASE
//Intento de utilización un ID de nodo hijo no válido para el nodo padre: "%0:s" de"%1:s".\n \nDebe revisar la definición del Meta4Object.
#define M4_CH_DM_CHILDBLOCK_ERROR_ID                0x0053 + M4_CH_DM_ERROR_BASE
//Intento de utilización un conjunto de filas hijo no válido del nodo hijo: "%0:s" para el nodo padre: "%1:s" de "%2:s".\n \nDebe revisar la definición del Meta4Object.
#define M4_CH_DM_ITEMISEOF_CNB                      0x0059 + M4_CH_DM_ERROR_BASE
//Se llamó a la operación "%0:s" y el elemento no es válido.\n \nMeta4Object no creado, información de contexto no disponible.\nDebe consultar con el administrador.
#define M4_CH_DM_ITEMCALLNOACCESS                   0x005A + M4_CH_DM_ERROR_BASE
//Meta4Object no válido.\n \nIntento de ejecución del elemento "%0:s" en "%1:s" de "%2:s".\nDebe consultar con el administrador.
#define M4_CH_DM_RECORDSETEXEBLOCKNIHIL             0x005C + M4_CH_DM_ERROR_BASE
//Intento de ejecución de "%0:s" en "%1:s" de "%2:s" sobre un conjunto de filas no válido.\n \nDebe comprobar si el nodo padre está colocado correctamente.
#define M4_CH_DM_RSEXEPOPERROR                      0x005D + M4_CH_DM_ERROR_BASE
//Error al ejecutar "%0:s" de "%1:s" de"%2:s".\n \nDebe revisar los demás errores de la pila.
#define M4_CH_DM_ACCESSLOAD                         0x005E + M4_CH_DM_ERROR_BASE
//Error al cargar "%0:s".\n \nDebe revisar los demás errores de la pila.
#define M4_CH_DM_ACCESSPERSIST                      0x005F + M4_CH_DM_ERROR_BASE
//Error al guardar "%0:s".\n \nDebe revisar los demás errores de la pila.
#define M4_CH_DM_DEBUGMONOBLOCK                     0x0060 + M4_CH_DM_ERROR_BASE
//Conjunto de filas borrado en el nodo monobloque "%0:s" de "%1:s".
#define M4_CH_DM_ITEMDONTEXIST2                     0x0062 + M4_CH_DM_ERROR_BASE
//Intento de ejecución de un "%0:s" que no existe.\n \nDebe revisar la definición del Meta4Object.\nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_EXTERNAL_ACCESS_DETECTED           0x0068 + M4_CH_DM_ERROR_BASE
//Se detectó una referencia externa al anexar/separar.\n \nDebe asegurarse de que no haya referencias externas antes de anexar/separar.
#define M4_CH_DM_INCONSISTENCY_REFERENCE            0x0069 + M4_CH_DM_ERROR_BASE
//Referencia no válida a un Gestor de Meta4Objects al agregar/separar en Meta4Object "%0:s".\n \nTodos los Meta4Object's generados tienen que pertenecer al mismo gestor.
#define M4_CH_DM_SET_VALUE_CONVERSION               0x006A + M4_CH_DM_ERROR_BASE
//Conversión de valores necesaria.\n \nDebe revisar la definición del Meta4Object.\nEsto sucedió al asignar un valor "%2:s" en "%3:s" de "%4:s". Tipo del valor a asignar :"%0:s". Tipo del elemento: "%1:s".
#define M4_CH_DM_OPERATION_VALUE_WITH_INDEX         0x006B + M4_CH_DM_ERROR_BASE
//Error durante una sesión de búsqueda al leer "%1:s" en la fila "%0:s".\n \nDebe revisar los demás errores de la pila.\nEsto sucedió en "%2:s" de "%3:s".
#define M4_CH_DM_CHANNELMANAGERNULL                 0x006C + M4_CH_DM_ERROR_BASE
//El Gestor de Meta4Objects es nulo.\n \nDebe consultar con el administrador.
#define M4_CH_DM_TYPE_MISMATCH                      0x006D + M4_CH_DM_ERROR_BASE
//No coinciden los tipos en el argumento "%0:s" para la función de búsqueda en "%2:s" de "%3:s". El tipo real es "%1:s", pero debe ser: "Número".\n \nDebe revisar la definición del Meta4Object
#define M4_CH_DM_WRONG_PARAM_NUM                    0x006E + M4_CH_DM_ERROR_BASE
//Número no válido de argumentos: "%0:s" en la función de búsqueda.\n \nDebe ser un múltiplo de 3 o de 3 + 1.\nDebe revisar la definición del Meta4Object.\nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_NO_SUCH_ITEM                       0x006F + M4_CH_DM_ERROR_BASE
//"%0:s" no existe en "%1:s" de "%2:s" mientras se utiliza la función de búsqueda.\n \nDebe revisar la definición del Meta4Object.
#define M4_CH_DM_SECURITY_ITEM_READ                 0x0070 + M4_CH_DM_ERROR_BASE
//Acceso de lectura denegado.\n \nAcceso de lectura a "%0:s" de "%1:s" denegado.\nDebe verificar su perfil.\nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_SECURITY_ITEM_WRITE                0x0071 + M4_CH_DM_ERROR_BASE
//Acceso de modificación denegado.\n \nAcceso de modificación en "%0:s" de "%1:s" denegado.\nDebe verificar su perfil.\nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_SECURITY_NODE_WRITE                0x0072 + M4_CH_DM_ERROR_BASE
//Acceso de escritura denegado.\n \nNo puede insertar filas porque no tiene los permisos necesarios.\nDebe verificar su perfil.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_SECURITY_NODE_DELETE               0x0073 + M4_CH_DM_ERROR_BASE
//Acceso de borrado denegado.\n \nNo puede borrar filas porque no tiene los permisos necesarios.\nDebe verificar su perfil.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_PARTIAL_LOAD_INVALID_OP            0x0074 + M4_CH_DM_ERROR_BASE
//Intento de añadir o borrar filas en carga parcial.\n \nDebe revisar la definición del Meta4Object.\nEsto sucedió en "%0:s" de"%1:s".
#define M4_CH_DM_PARTIAL_LOAD_NHIHIL                0x0076 + M4_CH_DM_ERROR_BASE
//Intento de utilización un conjunto de filas no válido en carga parcial.\n \nDebe revisar la definición del Meta4Object.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_FILTER_RAM_NIHIL                   0x0078 + M4_CH_DM_ERROR_BASE
//Intento de utilización un conjunto de filas no válido con filtros RAM.\n \nDebe comprobar si el nodo padre está colocado correctamente.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_OPERATION_VALUE_WITH_INDEX_EOF     0x007C + M4_CH_DM_ERROR_BASE
//Error durante una sesión de búsqueda en la columna "%0:s".\n \nNo se pudo acceder a la columna "%0:s" en la función "FindRegister".\nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_INVALIDAUTOLOADMODE_NODE           0x007D + M4_CH_DM_ERROR_BASE
//Intento de aplicar un modo de carga automática no válido "%0:s".\n \nDebe revisar la definición del Meta4Object\nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_ADDSLICES_DATESINVALID             0x0080 + M4_CH_DM_ERROR_BASE
//Error al agregar tramos porque la fecha de fin "%1:s" es menor que la fecha de inicio "%0:s".\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedió en "%2:s" de "%3:s" de "%4:s".
#define M4_CH_DM_SORT_FILTER_ITEM_TYPE_MISMATCH     0x0081 + M4_CH_DM_ERROR_BASE
//Error al crear filtro de Ordenación porque el elemento "%0:s" no es una cadena, número o fecha.\n \nDebe revisar la definición del Meta4Object. \nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_RAM_FILTER_FAILURE                 0x0082 + M4_CH_DM_ERROR_BASE
//Error operando con filtros RAM.\n \nDebe revisar los demás errores de la pila.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_RAM_FILTER_CREATION                0x0083 + M4_CH_DM_ERROR_BASE
//No se puede crear el filtro RAM.\n \nDebe revisar los demás errores de la pila.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_SORT_FILTER_NO_ITEM                0x0084 + M4_CH_DM_ERROR_BASE
//No se puede crear el filtro de ordenación porque no existe "%0:s".\n \nDebe revisar la definición del Meta4Object.\nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_SORT_FILTER_BAD_ORDER              0x0085 + M4_CH_DM_ERROR_BASE
//Orden no válido en el filtro de ordenación.\n \nLos valores posibles son: -1 = Ascendente y 1 = Descendente. El orden por defecto es el ascendente.\nEsto sucedió en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_ITEM_TYPE_MISMATCH2                0x0086 + M4_CH_DM_ERROR_BASE
//No coinciden los tipos en el argumento "%0:s" y la columna "%1:s" para la función de búsqueda.\n \nEl tipo real es "%2:s", pero debe ser: "Número" o "Cadena".\nEsto sucedió en "%3:s" de "%4:s".
#define M4_CH_DM_SEC_GET_IN_PARENT                  0x0087 + M4_CH_DM_ERROR_BASE
//Acceso de lectura denegado.\n \nNo tiene los permisos necesarios de lectura en "%0:s" para propagarse.\nDebe verificar su perfil.\nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_SECURITY_ITEM_WRITE_SETVALUE       0x008A + M4_CH_DM_ERROR_BASE
//Acceso de escritura denegado.\n \nNo tiene los permisos para modificar el campo "%0:s".\nDebe verificar su perfil.\nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_DATEOUTOFRANGE_SETDATE             0x008B + M4_CH_DM_ERROR_BASE
//Intento de definir una fecha de tramo no válida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedió con la fecha "%0:s" en "%1:s" de "%2:s" de "%3:s".
#define M4_CH_DM_ERASER_DESTROY		                0x008C + M4_CH_DM_ERROR_BASE
//Error al borrar el "%0:s" usando el destructor de objetos.\n \nDebe avisar al administrador del servidor de aplicaciones.
#define M4_CH_DM_ITEMOUTOFRANGE_GETVALUE            0x008D + M4_CH_DM_ERROR_BASE
//Intento de lectura de un elemento que no existe.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_ITEMOUTOFRANGE_SETVALUE            0x008E + M4_CH_DM_ERROR_BASE
//Intento de asignación de un elemento que no existe.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_SLICEISEOF_GETVALUE                0x0090 + M4_CH_DM_ERROR_BASE
//Intento de lectura de un elemento del valor de un tramo en posición no válida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedió en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_SLICEISEOF_SETVALUE                0x0091 + M4_CH_DM_ERROR_BASE
//Intento de asignación de un elemento del valor de un tramo en posición no válida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedió en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_VALIDATION_NIHILBLOCK              0x0092 + M4_CH_DM_ERROR_BASE
//Intento de realización de una operación en un conjunto de filas no válido.\n \nEsto sucedió en "%0:s" de "%1:s" de "%2:s".\n \nDebe comprobar que el nodo padre está colocado correctamente o tiene filas.
#define M4_CH_DM_VALIDATION_NIHILBLOCK_GETVALUE     0x0093 + M4_CH_DM_ERROR_BASE
//Intento de lectura de un elemento en un conjunto de filas no válido.\n \nEsto sucedió en "%0:s" de "%1:s" de "%2:s".\nDebe comprobar que el nodo padre está colocado correctamente o tiene filas.
#define M4_CH_DM_VALIDATION_NIHILBLOCK_SETVALUE     0x0094 + M4_CH_DM_ERROR_BASE
//Intento de asignación de un elemento en un conjunto de filas no válido.\n \nEsto sucedió en "%0:s" de "%1:s" de "%2:s".\nDebe comprobar que el nodo padre está colocado correctamente o tiene filas.
#define M4_CH_DM_VALIDATION_REGEOF                  0x0095 + M4_CH_DM_ERROR_BASE
//Intento de realización de una operación en una fila no válida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedió en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_VALIDATION_REGEOF_GETVALUE         0x0096 + M4_CH_DM_ERROR_BASE
//Intento de lectura de un elemento en una fila no válida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedió en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_VALIDATION_REGEOF_SETVALUE         0x0097 + M4_CH_DM_ERROR_BASE
//Intento de asignación de un elemento en una fila no válida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedió en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_NULL_RWT                           0x0098 + M4_CH_DM_ERROR_BASE
//Intento de realización de una operación en un estado no válido.\n \nDebe revisar los demás errores de la pila.\nEsto sucedió en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_NULL_RWT_GETVALUE                  0x0099 + M4_CH_DM_ERROR_BASE
//Intento de lectura en un estado no válido.\n \nDebe revisar los demás errores de la pila.\nEsto sucedió en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_NULL_RWT_SETVALUE                  0x009A + M4_CH_DM_ERROR_BASE
//Intento de asignación en un estado no válido.\n \nDebe revisar los demás errores de la pila.\nEsto sucedió en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_NIHILBLOCK_COUNT                   0x009B + M4_CH_DM_ERROR_BASE
//Intento de obtener el número de filas de un conjunto de filas no válido.\n \nDebe comprobar que el nodo padre está colocado correctamente o tiene filas.
#define M4_CH_DM_NIHILBLOCK_ADD                     0x009D + M4_CH_DM_ERROR_BASE
//Intento de añadir filas de un conjunto de filas no válido.\n \nDebe comprobar que el nodo padre está colocado correctamente o tiene filas.
#define M4_CH_DM_NIHILBLOCK_DELETE                  0x009E + M4_CH_DM_ERROR_BASE
//Intento de borrar filas de un conjunto de filas no válido.\n \nDebe comprobar que el nodo padre está colocado correctamente o tiene filas.
#define M4_CH_DM_FILTER_RAM_NIHIL2                  0x0101 + M4_CH_DM_ERROR_BASE
//Error al usar filtros en RAM sobre un conjunto de filas no válido.\n \nDebe comprobar que el nodo padre está colocado correctamente o tiene filas.\nEsto sucedió en "%0:s".
#define M4_CH_DM_FRIEND_OTHER_MANAGER               0x0102 + M4_CH_DM_ERROR_BASE
//Intento de establecer un M4Object compañero de un gestor distinto.\n \nEsto sucedió con el M4Object "%0:s".\n \nDebe establecer compañeros con M4Objects de un mismo gestor.
#define M4_CH_DM_CACHE_PUT_TEMPORAL_CHANNEL			0x0103 + M4_CH_DM_ERROR_BASE
//Intento de poner un Meta4Object temporal en la caché de datos.\n \nEl Meta4Object "%0:s" es temporal y no debe ser almacenado en caché.\nDebe marcar el Meta4Object como no almacenable en caché.
#define M4_CH_DM_CACHE_GET_TEMPORAL_CHANNEL			0x0104 + M4_CH_DM_ERROR_BASE
//Intento de obtener un Meta4Object temporal de la caché de datos.\n \nEl Meta4Object "%0:s" es temporal y no debe ser almacenado en caché.\nDebe marcar el Meta4Object como no almacenable en caché.
#define M4_CH_DM_CANNOT_OPEN_LOADFILE				0x0105 + M4_CH_DM_ERROR_BASE
//Error al cargar un Meta4Object de archivo.\n \nNo se pudo abrir el archivo "%0:s".\nDebe comprobar que el archivo existe y tiene permisos de lectura.
#define M4_CH_DM_CANNOT_OPEN_PERSISTFILE			0x0106 + M4_CH_DM_ERROR_BASE
//Error al guardar un Meta4Object a archivo.\n \nNo se pudo abrir el archivo "%0:s" para guardar el Meta4Object "%1:s".\nDebe comprobar la ruta existe y tiene permisos de escritura.
#define M4_CH_DM_CANNOT_LOAD_CHANNEL_FROM_FILE		0x0107 + M4_CH_DM_ERROR_BASE
//Error al cargar un Meta4Object de archivo.\n \nNo se pudo cargar el Meta4Object "%1:s" del archivo "%0:s". Posiblemente el archivo no es válido.\nDebe consultar con el administrador.
#define M4_CH_DM_CANNOT_PERSIST_CHANNEL_TO_FILE		0x0108 + M4_CH_DM_ERROR_BASE
//Error al guardar un Meta4Object a archivo.\n \nNo se pudo guardar el Meta4Object "%1:s" al archivo "%0:s".\nDebe consultar con el administrador.
#define M4_CH_DM_DIFFERENT_CHANNEL_LOADFROMFILE		0x0109 + M4_CH_DM_ERROR_BASE
//Error al cargar un Meta4Object de archivo.\n \nNo se pudo cargar de archivo el Meta4Object "%0:s" en el Meta4Object "%5:s". Son de distinto tipo.\nDebe consultar con el administrador.\nAtributos del Meta4Object "%0:s" del archivo: tipo cliente servidor "%1:s" - lenguaje "%2:s" - máscara de seguridad "%3:d" - versión "%4:d".\nAtributos del Meta4Object "%5:s" destino: tipo cliente servidor "%6:s" - lenguaje "%7:s" - máscara de seguridad "%8:d" - versión "%9:d".
#define M4_CH_DM_CANNOT_PERSIST_CHNOTBUILT			0x010A + M4_CH_DM_ERROR_BASE
//Error al guardar un Meta4Object a archivo.\n \nNo se pudo guardar el Meta4Object al archivo "%0:s" porque no está construido.\nDebe construir el Meta4Object antes de guardarlo a archivo.
#define M4_CH_DM_CACHE_PUT_PROXY_CHANNEL			0x010B + M4_CH_DM_ERROR_BASE
//Intento de poner un Meta4Object proxy en la caché de datos.\n \nEl Meta4Object "%0:s" es proxy y no debe ser almacenado en caché.\nDebe marcar el Meta4Object como no almacenable en caché.
#define M4_CH_DM_CACHE_GET_PROXY_CHANNEL			0x010D + M4_CH_DM_ERROR_BASE
//Intento de obtener un Meta4Object proxy de la caché de datos.\n \nEl Meta4Object "%0:s" es proxy y no debe ser almacenado en caché.\nDebe marcar el Meta4Object como no almacenable en caché.
#define M4_CH_DM_FULL_SERIALIZE_TEMPORAL_CHANNEL	0x010F + M4_CH_DM_ERROR_BASE
//Intento de serializar completamente un Meta4Object temporal.\n \nEl Meta4Object "%0:s" es temporal y no puede ser serializado completamente.\nDebe consultar con el administrador.
#define M4_CH_DM_FULL_SERIALIZE_PROXY_CHANNEL		0x0110 + M4_CH_DM_ERROR_BASE
//Intento de serializar completamente un Meta4Object proxy.\n \nEl Meta4Object "%0:s" es proxy y no puede ser serializado completamente.\nDebe consultar con el administrador.
#define M4_CH_DM_INCONSISTENCY_REFNULL              0x0112 + M4_CH_DM_ERROR_BASE
//Gestor de Meta4Objects nulo al agregar/separar en Meta4Object.\n \nTodos los Meta4Objects deben tener un Gestor válido asociado.
#define M4_CH_DM_NIHILBLOCK_MEMORY                  0x0113 + M4_CH_DM_ERROR_BASE
//Intento de realización de una operación en un conjunto de filas no válido.\n \nEsto se debe a que se ha excedido el número máximo de bloque en el nodo.\nDebe comprobar la definición del Meta4Object.
#define M4_CH_DM_NIHILBLOCK_COUNT_MEMORY            0x0114 + M4_CH_DM_ERROR_BASE
//Intento de obtener el número de filas en un conjunto de filas no válido.\n \nEsto se debe a que se ha excedido el número máximo de bloque en el nodo.\nDebe comprobar la definición del Meta4Object.
#define M4_CH_DM_NIHILBLOCK_ADD_MEMORY              0x0115 + M4_CH_DM_ERROR_BASE
//Intento de añadir filas en un conjunto de filas no válido.\n \nEsto se debe a que se ha excedido el número máximo de bloque en el nodo.\nDebe comprobar la definición del Meta4Object.
#define M4_CH_DM_NIHILBLOCK_DELETE_MEMORY           0x0116 + M4_CH_DM_ERROR_BASE
//Intento de borrar filas en un conjunto de filas no válido.\n \nEsto se debe a que se ha excedido el número máximo de bloque en el nodo.\nDebe comprobar la definición del Meta4Object.
#define M4_CH_DM_NUM_RECORDS                        0x0117 + M4_CH_DM_ERROR_BASE
//Alcanzado el limite maximo de filas por bloque.\n \nIntento de añadir filas en un bloque que alcanzó el número máximo de filas permitido.\nDebe comprobar la definición del Meta4Object.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_BAD_DATAL_MAGIC_NUMBER				0x0118 + M4_CH_DM_ERROR_BASE
//Formato incorrecto.\n \nEl formato de codificación del Meta4Object es incorrecto.\nDebe consultar con el administrador.
#define M4_CH_DM_FRIEND_WITHOUT_L2                  0x0119 + M4_CH_DM_ERROR_BASE
//Intento de establecer un M4Object compañero sin existir enlace de nivel 2 entre los M4Objects.\n \nDebe establecer primero un enlace de nivel 2 entre los M4Objects.\nEsto sucedió entre "%0:s" (padre de la relación) y "%1:s" (hijo de la relación).
#define M4_CH_DM_FRIEND_MYSELF						0x011A + M4_CH_DM_ERROR_BASE
//Un M4Object se ha hecho compañero de si mismo.\n \nEsto sucedió en el M4Object "%0:s".\n \nNo debe hacer un M4Object compañero de si mismo.
#define M4_CH_DM_FRIEND_DOUBLE_DEL					0x011B + M4_CH_DM_ERROR_BASE
//Se ha intentado borrar un M4Object compañero, que no lo era.\n \nPuede que esté intentando borrar el M4Object compañero "%0:s" más de una vez.\n \nNo debe borrar M4Objects compañeros más de una vez.
#define M4_CH_DM_BAD_ITEM_CONECTOR					0x011C + M4_CH_DM_ERROR_BASE
//Error en la conexión entre elementos.\n \nEl ámbito del elemento padre de la conexión debe ser mayor o igual que el ambito del elemento hijo de la conexión.\nDebe comprobar la definición del M4Object.\Esto sucedió entre el elemento padre de la conexión "%0:s", con ámbito "%1:n" y el elemento hijo de la conexión "%2:s", con ámbito "%3:n"."
#define M4_CH_DM_NO_MEMORY							0x011D + M4_CH_DM_ERROR_BASE
//Recursos insuficientes.\n \nNo hay memoria suficiente para obtener "%0:s" bytes.\nDebe cerrar alguna aplicación.\nEsto ha sucedido en la línea "%1:s" del archivo "%2:s".
#define M4_CH_DM_INTERNAL_ERROR						0x011E + M4_CH_DM_ERROR_BASE
//Error interno.\n \nDebe consultar con el administrador.\nEsto ha sucedido en la línea "%0:s" del fichero "%1:s".
#define M4_CH_DM_BAD_ORG_TYPE_TYPE					0x011F + M4_CH_DM_ERROR_BASE
//Tipo de sociedad incorrecto.\n \nEl tipo de sociedad debe ser numérico y es "%0:s". Se asumirá el valor de su definición "%1:s".\nDebe consultar con el administrador.
#define M4_CH_DM_BAD_ORG_TYPE						0x0120 + M4_CH_DM_ERROR_BASE
//Tipo de sociedad incorrecto.\n \nEl tipo de sociedad vale "%0:s" y debería estar comprendido entre "%1:s" y "%2:s". Se asumirá el valor de su definición "%3:s".\nDebe consultar con el administrador.
#define M4_CH_DM_BAD_BUILD_META_CHANNEL				0x0121 + M4_CH_DM_ERROR_BASE
//Meta4Object del sistema mal definido.\n \nNo se ha podido generar el Meta4Object del sistema.\nDebe consultar con el administrador.
#define M4_CH_DM_NULL_ROLE							0x0122 + M4_CH_DM_ERROR_BASE
//El perfil vacío es inválido.\n \nDebe consultar con el administrador."
#define M4_CH_DM_ATTR_TRUNCATED						0x0123 + M4_CH_DM_ERROR_BASE
//Atributo de Meta4Object truncado.\n \nEl atributo "%0:s" tienen un valor "%1:s" supera su máxima longitud "%2:s" y se usará "%3:s".\nDebe consultar con el administrador.
#define M4_CH_DM_STT_NO_STATE_STARTED				0x0124 + M4_CH_DM_ERROR_BASE
//Estadísticas no iniciadas.\n \nNo hay estado de estadísticas para la operación "%0:s".\nDebe consultar al administrador.
//Antiguo #define M4_CH_VM_STT_NO_STATE_STARTED					M4_CH_VM_ERROR_BASE + 0x0178
#define M4_CH_DM_ITEM_NOT_FOUND						0x0125 + M4_CH_DM_ERROR_BASE
//Elemento inexistente.\n \n"%0:s" de "%1:s" de "%2:s" no existe.\nDebe definir el elemento.\nEsto sucedió al ejecutar la función "%3:s".
//Antiguo #define M4_CH_VM_ITEM_NOT_FOUND							M4_CH_VM_ERROR_BASE + 0x0141
#define M4_CH_DM_NO_SUCH_NODE_STT					0x0126 + M4_CH_DM_ERROR_BASE
//Meta4Object no válido.\n \nEl Meta4Object de estadísticas "%1:s" no es válido porque no existe el nodo "%0:s".\nDebe consultar al administrador.
//Antiguo #define M4_CH_VM_NO_SUCH_NODE_STT						M4_CH_VM_ERROR_BASE + 0x0160
#define M4_CH_DM_NO_PERSISTIABLE_OBJECT_ID			0x0127 + M4_CH_DM_ERROR_BASE
// No existe ID en objeto persistente.\n \nDebe consultar con el administrador.
#define M4_CH_DM_WRONG_INSTANCE_HANDLE				0x0128 + M4_CH_DM_ERROR_BASE
//Instancia no encontrada.\n \nNo se encuentra la instancia "%0:s" del Meta4Object "%1:s".\nDebe definir la instancia.
#define M4_CH_DM_BUILD_FROM_ID						0x0129 + M4_CH_DM_ERROR_BASE
//Definición de Meta4Object incorrecto.\n \nError al crear la definición del Meta4Object "%0:s".
#define M4_CH_DM_ERROR_LOAD							0x012A + M4_CH_DM_ERROR_BASE /*solo debug*/
//No se puede cargar el Meta4Object.\n \nError al cargar el Meta4Object "%0:s".\nDebe revisar errores anteriores.
#define M4_CH_DM_REDEFINING_INSTANCE				0x012B + M4_CH_DM_ERROR_BASE
//Redefinición de instancia.\n \nRedefinición de la instancia del Meta4Object para código JIT. Cerrando el Meta4Object "%0:s".
#define M4_CH_DM_BAD_ITEM_INDEX						0x012C + M4_CH_DM_ERROR_BASE
//Elemento de índice incorrecto.\n \nEl elemento en la posición "%0:s" del índice "%1:s" no es válido.\nDebe cambiar el índice.\nEsto sucedió al ejecutar la función BSearch.
#define M4_CH_DM_BAD_OPERATION_ITEM_INDEX 			0x013C + M4_CH_DM_ERROR_BASE
//Operación no válida.\n \nOperación no válida para "%0:s" de "%1:s" de "%2:s" del índice "%3:s".\nDebe asegurarse de que sólo el último elemento del índice pueda tener una operación que no sea IGUAL.
#define M4_CH_DM_ITEM_TYPE_MISMATCH					0x013D + M4_CH_DM_ERROR_BASE
//Tipo no válido.\n \nEl tipo de "%0:s" de "%1:s" de "%2:s" no es válido.\nDebe ser "%4:s" en lugar de "%5:s".\nEsto sucedió al ejecutar la función "%3:s".
#define M4_CH_DM_STT_STATE_NOT_MATCHED				0x013E + M4_CH_DM_ERROR_BASE
//Estadísticas incorrectas.\n \nFalta el estado de ejecución de las estadísticas. "%0:s" no coincide con el estado final "%1:s".\nDebe consultar al administrador.
//Amtiguo #define M4_CH_VM_STT_STATE_NOT_MATCHED					M4_CH_VM_ERROR_BASE + 0x01d2
#define M4_CH_DM_ARGUMENT_TYPE_MISMATCH				0x013F + M4_CH_DM_ERROR_BASE
//Tipo de argumento incorrecto.\n \nEl tipo del argumento "%0:s" es "%1:s" y debería ser "%2:s".\nDebe cambiar el tipo del argumento.\nEsto ha sucedido al invocar la función "FindRegister".
#define M4_CH_DM_INVALID_COMPARE_FUNCTION			0x0140 + M4_CH_DM_ERROR_BASE
//Número de función no válido.\n \nLa función "%0:s" no es una función válida.\nDebe usar una función en el rango que va de "%1:s" a "%2:s".\nEsto ha sucedido al invocar la función "FindRegister".
#define M4_CH_DM_NO_GET_ACCESS_L2					0x0141 + M4_CH_DM_ERROR_BASE
//Instancia no encontrada.\n \nNo se encuentra la instancia "%0:s" en una llamada de nivel 2.\nDebe definir la instancia.\nEsto sucedió al ejecutar la función "%1:s".
#define M4_CH_DM_NO_SUCH_NODE						0x0142 + M4_CH_DM_ERROR_BASE
//Nodo inexistente.\n \n"%0:s" de "%1:s" no existe.\nDebe definir el nodo o eliminar las referencias a él.\nEsto sucedió en la función "%2:s".
#define M4_CH_DM_WRONG_ITEM_DEFINITION 				0x0143 + M4_CH_DM_ERROR_BASE
//Definición de elemento no válido.\n \nDefinición no válida para "%0:s" de "%1:s" de "%2:s".\nDebe tener "%3:s" argumentos en lugar de "%4:s".
#define M4_CH_DM_CANT_EXECUTE_ITEM					0x0144 + M4_CH_DM_ERROR_BASE
//No se puede ejecutar un elemento.\n \nNo se puede ejecutar "%0:s" de "%1:s" de "%2:s".\nDebe revisar sus permisos.
#define M4_CH_DM_RTS_POP							0x0145 + M4_CH_DM_ERROR_BASE
//Error interno de ejecución.\n \nError al ejecutar extracción de RTS. "%0:s".\nDebe consultar al administrador.
#define M4_CH_DM_FORMATTING_ERROR_STRING			0x0146 + M4_CH_DM_ERROR_BASE
//Error al dar formato al mensaje.\n \nNo se puede dar formato a la cadena de error. Se mostrará el mensaje sin dar formato.\nDebe revisar el formato de su mensaje de error.
#define M4_CH_DM_WRONG_PARAM_NUM_INS_1				0x0147 + M4_CH_DM_ERROR_BASE
//Número de argumentos no válido.\n \nLa función "%3:s" usa argumentos variables y la definición del elemento espera un número fijo.\nDebe cambiar la definición del elemento.\nEsto sucedió al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_NO_READ_PROP						0x0148 + M4_CH_DM_ERROR_BASE
//Acceso de lectura denegado.\n \nNo tiene permisos para leer un atributo de Meta4Object.\nDebe consultar con el administrador.
#define M4_CH_DM_NO_WRITE_PROP						0x0149 + M4_CH_DM_ERROR_BASE
//Acceso de modificación denegado.\n \nNo tiene permisos para modificar un atributo de Meta4Object.\nDebe consultar con el administrador.
#define M4_CH_DM_DELETE_PROP						0x014A + M4_CH_DM_ERROR_BASE
//Acceso de borrado denegado.\n \nNo tiene permisos para borrar el atributo de Meta4Object "%0:s".\nDebe consultar con el administrador.
#define M4_CH_DM_PROPAGATE_BLOCK_NIHIL_IN_SON		0x014B + M4_CH_DM_ERROR_BASE
//No se puede propagar el valor del elemento.\n \nNo se puede propagar el valor del elemento porque la relación en modo "libre" entre elementos de tipo "bloque" no queda unívocamente determinada en el nodo hijo.\nDebe revisar la estructura de conectores de su Meta4Object y comprobar que no tiene listas definidas en el nodo hijo.\nEsto sucedió al intentar propagar el valor "%0:s" entre el elemento padre de la relación "%1:s" de "%2:s" de "%3:s" y el elemento hijo "%4:s" de "%5:s" de "%6:s".
#define M4_CH_DM_PROPAGATE_BLOCK_NULL_IN_SONMETH	0x014C + M4_CH_DM_ERROR_BASE
//No se puede propagar el valor del elemento.\n \nNo se puede propagar el valor del elemento porque el elemento hijo es de tipo "método".\nDebe revisar la estructura de conectores de su Meta4Object.\nEsto sucedió al intentar propagar el valor "%0:s" entre el elemento padre de la relación "%1:s" de "%2:s" de "%3:s" y el elemento hijo "%4:s" de "%5:s" de "%6:s".
#define M4_CH_DM_SET_ITEM_NULL						0x014D + M4_CH_DM_ERROR_BASE
//Intento de escritura sobre un Item de tipo nulo.\n \nDebe revisar la definición del Meta4Object.\nEsto sucedió al asignar un valor al Item "%0:s" en "%1:s" de "%2:s".
#define M4_CH_DM_M4OBJ_AREADY_BUILT					0x014E + M4_CH_DM_ERROR_BASE
//Construcción repetida.\n \nNo se puede construir el Meta4Object porque ya está construido.\nDebe consultar con el administrador.
#define M4_CH_DM_CACHEABLE_DOC						0x014F + M4_CH_DM_ERROR_BASE
//Documento no válido.\n \nNo se puede utilizar un documento cacheable en una lista.\nDebe cambiar el modo cacheable del Meta4Object documento.\nEsto sucedió conectando el documento "%0:s" de "%1:s" a la lista "%2:s" de "%3:s".
#define M4_CH_DM_PARTIAL_LOAD_TOTALIZE	            0x0150 + M4_CH_DM_ERROR_BASE
//Intento de totalizar en carga parcial.\n \nNo se puede ejecutar la función de totalización "%0:s" en un nodo con carga parcial.\nDebe revisar la definición del Meta4Object.\nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_DM_STT_BAD_STT_M4OBJ					0x0151 + M4_CH_DM_ERROR_BASE
//Meta4Object de estadísticas incorrecto.\n \nEl Meta4Object "%0:s" no es un Meta4Object válido para las estadísticas.\nDebe consultar con el administrador.
#define M4_CH_DM_STT_BAD_INTERMEDIATE_VERSION		0x0152 + M4_CH_DM_ERROR_BASE
//Versión de estadísticas incorrecta.\n \nEl archivo "%0:s" tiene versión de estadísticas "%1:s" y la versión actual es "%2:s".\nDebe consultar con el administrador.
#define M4_CH_DM_STT_NO_READ_INTERMEDIATE	        0x0153 + M4_CH_DM_ERROR_BASE
//Error al leer estadísticas.\n \nNo se pueden leer las estadísticas del archivo "%0:s".\nDebe consultar con el administrador.
#define M4_CH_DM_STT_NO_WRITE_INTERMEDIATE	        0x0154 + M4_CH_DM_ERROR_BASE
//Error al escribir estadísticas.\n \nNo se pueden escribir las estadísticas al archivo "%0:s".\nDebe consultar con el administrador.
#define M4_CH_DM_STT_NO_COMPRESS_INTERMEDIATE       0x0155 + M4_CH_DM_ERROR_BASE
//Error comprimiendo archivo de estadísticas.\n \nNo se puede comprimir el archivo de estadísticas "%0:s" al archivo "%1:s".\nDebe consultar con el administrador.
#define M4_CH_DM_STT_NO_DECOMPRESS_INTERMEDIATE     0x0156 + M4_CH_DM_ERROR_BASE
//Error descomprimiendo archivo de estadísticas.\n \nNo se puede descomprimir el archivo de estadísticas "%0:s" al archivo "%1:s".\nDebe consultar con el administrador.
#define M4_CH_DM_NULL_ARGUMENT						0x0157 + M4_CH_DM_ERROR_BASE
// Argumento nulo a una función.\n \nEl argumento "%0:s" del método "%1:s" es nulo.\nDebe consultar con el administrador.\nEsto sucedió en la línea "%2:s" del archivo "%3:s".
#define M4_CH_DM_NO_OPEN_PL_FILE					0x0158 + M4_CH_DM_ERROR_BASE
//No se puede abrir archivo.\n \nNo se puede abrir el archivo "%0:s" en modo "%1:s" por la razón "%2:s".\nDebe consultar con el administrador.\nEsto sucedió en la carga parcial de "%3:s" de "%4:s".
#define M4_CH_DM_NO_WRITE_PL_RECORD					0x0159 + M4_CH_DM_ERROR_BASE
//No se puede guardar registro en archivo.\n \nNo se puede guardar el registro "%0:s" en el archivo "%1:s".\nDebe consultar con el administrador.\nEsto sucedió en la carga parcial de "%2:s" de "%3:s".
#define M4_CH_DM_NO_READ_PL_RECORD					0x015A + M4_CH_DM_ERROR_BASE
//No se puede recuperar registro de archivo.\n \nNo se puede recuperar el registro "%0:s" del archivo "%1:s".\nDebe consultar con el administrador.\nEsto sucedió en la carga parcial de "%2:s" de "%3:s".
#define M4_CH_DM_NO_ORDER_PL_INDEX					0x015B + M4_CH_DM_ERROR_BASE
//No se puede ordenar el bloque.\n \nNo se puede ordenar el bloque de carga parcial.\nDebe consultar con el administrador.\nEsto sucedió en la carga parcial de "%0:s" de "%1:s".
#define M4_CH_DM_NO_BRACKETS_IN_PL					0x015C + M4_CH_DM_ERROR_BASE
//Movimiento no permitido en carga parcial.\n \nNo se puede utilizar el operador corchete para moverse al registro "%0:s" en un nodo de carga parcial porque no está dentro de la ventana actual que va del registro "%1:s" al "%2:s".\nDebe consultar con el administrador.\nEsto sucedió en la carga parcial de "%3:s" de "%4:s".
#define M4_CH_DM_NO_ENCRYPT							0x015D + M4_CH_DM_ERROR_BASE
//Error al encriptar datos.\n \nSe ha producido un error al encriptar los datos en la caché de Meta4Objects de tipo delta.\nDebe consultar con el administrador.
#define M4_CH_DM_NO_DECRYPT							0x015E + M4_CH_DM_ERROR_BASE
//Error al desencriptar datos.\n \nSe ha producido un error al desencriptar los datos en la caché de Meta4Objects de tipo delta.\nDebe consultar con el administrador.
#define M4_CH_DM_REPEATED_INSTANCE					0x015F + M4_CH_DM_ERROR_BASE
//Instancia repetida.\n \nLa instancia de nivel 2 "%0:s" no puede ser añadida al Meta4Object "%1:s" porque ya existe.\nDebe utilizar otro identificador de instancia o eliminar la instancia existente.
#define M4_CH_DM_NO_SUCH_META4OBJ					0x0160 + M4_CH_DM_ERROR_BASE
//Meta4Object no encontrado.\n \nEl Meta4Object definido por el identificador de instancia "%0:s" no existe.\nCompruebe que el identificador de instancia se haya obtenido en el mismo entorno cliente/servidor en que se está utilizando.\nEsto sucedió al añadir la instancia de nivel 2 "%1:s" al Meta4Object "%2:s".
#define M4_CH_DM_BAD_CHANGE_ORG						0x0161 + M4_CH_DM_ERROR_BASE
//Sociedad no válida.\n \nNo se puede cambiar la sociedad "%0:s" a la sociedad "%1:s" porque no tiene los mismos mapeos al Meta4Object "%2:s".\nDebe elegir una sociedad que cumpla los mapeos.
#define M4_CH_DM_BAD_INIT_AS_MY_PARENT				0x0162 + M4_CH_DM_ERROR_BASE
//Elemento no válido.\n \nSe está intentando utilizar el elemento "%0:s" de "%1:s" de "%2:s" que no tiene valor asociado.\nPosiblemente se está intentando acceder a un elemento encarrilado que se ha borrado en la parte server.
#define M4_CH_DM_ROWS_QUOTA_EXCEEDED                0x0163 + M4_CH_DM_ERROR_BASE
//Se alcanzó el número máximo de filas por transacción.\n \nSe intentó añadir filas durante la transacción por encima del máximo permitido.\nDebe comprobar la definición del Meta4Object.\nEsto sucedió en "%0:s" de "%1:s".
#define M4_CH_DM_TOO_MANY_FILTERS	                0x0164 + M4_CH_DM_ERROR_BASE
//Se alcanzó el número máximo de filtros permitidos.\n \nSe intentó añadir más de "%0:s" filtros.\nDebe revisar la definición del Meta4Object\nEsto sucedió en "%1:s" de "%2:s".

#endif // __DMRES_HPP

