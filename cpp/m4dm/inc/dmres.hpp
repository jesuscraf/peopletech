#ifndef __DMRES_HPP
#define __DMRES_HPP

#include "m4res.hpp"

#define	M4LDB_CERROR_PROPERTY_NOT_FOUND             0x0001 + M4_CH_DM_ERROR_BASE // cambiar a MCR
//No se usa.
#define	M4_CH_MCR_NO_START_UP                       0x0003 + M4_CH_DM_ERROR_BASE // cambiar a dm
//No se puede conectar con la base de datos.\n \nDebe consultar con el administrador.
#define M4_CH_DM_CHNOTBUILT                         0x0004 + M4_CH_DM_ERROR_BASE
//Meta4Object no generado.\n \nNo se puede usar un Meta4Object que no se haya generado primero.\nDebe revisar los dem�s errores de la pila.
#define M4_CH_DM_SETCHECKPOINTMODE                  0x0005 + M4_CH_DM_ERROR_BASE
//Punto de control no v�lido.\n \nNo se puede definir el modo del punto de control "%0:s".\nDebe consultar con el administrador.\nEsto sucedi� en "%1:s".
#define M4_CH_DM_DESTROYWACCESS                     0x0006 + M4_CH_DM_ERROR_BASE
//Destrucci�n de un Meta4Object con referencias.\n \Intento de destrucci�n "%0:s" al que a�n se hace referencia "%1:s" veces.\nDebe revisar los dem�s errores de la pila.
#define M4_CH_DM_BUILDFROMFILE                      0x0007 + M4_CH_DM_ERROR_BASE
//Meta4Object no definido.\n \nError al definir un Meta4Object a partir del archivo: "%0:s".\nDebe revisar los dem�s errores de la pila.
#define M4_CH_DM_ENVIROMENTNULL                     0x0008 + M4_CH_DM_ERROR_BASE
//El entorno Meta4Object es nulo.\n \nDebe consultar al administrador.
#define M4_CH_DM_FACTORYNULL                        0x0009 + M4_CH_DM_ERROR_BASE
//La f�brica de entorno es nula.\n \nDebe consultar con el administrador.
#define M4_CH_DM_EXECUTINGITEM                      0x000B + M4_CH_DM_ERROR_BASE
//Elemento inexistente.\n \nIntento de ejecuci�n de un elemento que no existe en "%0:s" de "%1:s".\nDebe revisar la definici�n del Meta4Object.
#define M4_CH_DM_BADITEMTYPE                        0x000D + M4_CH_DM_ERROR_BASE
//Elemento no v�lido.\n \nIntento de ejecuci�n "%0:s" que no es un m�todo ni un concepto.\nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_MOVETOEOF                          0x000E + M4_CH_DM_ERROR_BASE
//Movimiento no v�lido.\n \nIntento de desplazamiento al elemento inexistente "%0:s".\nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_ITEMOUTOFRANGE                     0x000F + M4_CH_DM_ERROR_BASE
//Intento de utilizaci�n un elemento que no existe.\n \nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_REGISTERISEOF                      0x0010 + M4_CH_DM_ERROR_BASE
//Intento de realizaci�n de una operaci�n en una fila no v�lida.\n \nDebe revisar las operaciones realizadas anteriormente realizadas en el Meta4Object.\nEsto sucedi� en "%0:s" de %1:s".
#define M4_CH_DM_NIHILBLOCK                         0x0011 + M4_CH_DM_ERROR_BASE
//Intento de realizaci�n de una operaci�n en un conjunto de filas no v�lido.\n \nDebe comprobar que el nodo padre est� colocado correctamente o tiene filas.
#define M4_CH_DM_INVALIDSCOPE                       0x0015 + M4_CH_DM_ERROR_BASE
//Se encontr� un alcance de elemento no v�lido.\n \nAlcance del elemento "%0:s".\nDebe revisar la definici�n del Meta4Object.
#define M4_CH_DM_INVALIDAUTOLOADMODE                0x0020 + M4_CH_DM_ERROR_BASE
//Intento de definici�n de un modo de carga autom�tica no v�lido.\n \nModo "%0:s" en "%1:s".\nDebe revisar la definici�n del Meta4Object.
#define M4_CH_DM_AUTOLOADMANDATORY                  0x0021 + M4_CH_DM_ERROR_BASE
//Conflicto entre modos de carga autom�tica.\n \nEl modo en Meta4Object es "%0:s" e intenta definir un modo incompatible "%1:s" para el nodo "%2:s".\nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� en "%3:s".
#define M4_CH_DM_CACHE_PUT_CHANNEL					0x0039 + M4_CH_DM_ERROR_BASE
//""%0:s" no se puede almacenar en la cach�.\n \nDebe consultar al administrador.
#define M4_CH_DM_SLICEISEOF                         0x003A + M4_CH_DM_ERROR_BASE
//Intento de realizaci�n de una operaci�n en un tramo no v�lido.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_DATEOUTOFRANGE                     0x003B + M4_CH_DM_ERROR_BASE
//Intento de desplazamiento a la fecha no v�lida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedi� con la fecha "%0:s" en %0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_ITEMWITHOUTSLICES                  0x003D + M4_CH_DM_ERROR_BASE
//Intento de realizaci�n de una operaci�n de tramo en un elemento sin tramos.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_MOVETOEOF_CNB                      0x0044 + M4_CH_DM_ERROR_BASE
//El elemento de ID "%0:s" no existe.\n \nInformaci�n de contexto no disponible porque porque no se cre� el Meta4Object.\nDebe revisar los dem�s errores de la pila.
#define M4_CH_DM_MOVETOEOF_S                        0x0045 + M4_CH_DM_ERROR_BASE
//El elemento de ID "%0:s" no existe.\n \nDebe revisar la definici�n del Meta4Object. \nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_MOVETOEOF_S_CNB                    0x0046 + M4_CH_DM_ERROR_BASE
//El elemento de ID "%0:s" no existe.\n \nInformaci�n de contexto no disponible porque porque no se cre� el Meta4Object.\nDebe revisar los dem�s errores de la pila.
#define M4_CH_DM_PARTIAL_LOAD_END                   0x0048 + M4_CH_DM_ERROR_BASE
//Intento de desplazamiento a la �ltima fila de un nodo de carga parcial.\n \nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_PARTIAL_LOAD_PL                    0x004A + M4_CH_DM_ERROR_BASE
//Error al cargar un conjunto de filas en carga parcial.\n \nDebe revisar los dem�s errores de la pila.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_PARTIAL_LOAD_NMRTL                 0x004C + M4_CH_DM_ERROR_BASE
//No hay m�s filas para carga parcial.\n \nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_REGISTERISDELETED                  0x004D + M4_CH_DM_ERROR_BASE
//Intento de asignaci�n de un valor a una fila suprimida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedi� en la fila "%0:s" en "%1:s" de "%2:s" de "%3:s".
#define M4_CH_DM_AUTOLOAD_ERROR                     0x004F + M4_CH_DM_ERROR_BASE
//Error en carga autom�tica.\n \nDebe revisar los dem�s errores de la pila.\nEsto sucedi� en "%1:s" de "%2:s" con tipo de carga autom�tica "%0:s".
#define M4_CH_DM_CHILDNODEDONTEXIST                 0x0051 + M4_CH_DM_ERROR_BASE
//Intento de utilizaci�n un ID de nodo hijo no v�lido para el nodo padre: "%0:s" de"%1:s".\n \nDebe revisar la definici�n del Meta4Object.
#define M4_CH_DM_CHILDBLOCK_ERROR_ID                0x0053 + M4_CH_DM_ERROR_BASE
//Intento de utilizaci�n un conjunto de filas hijo no v�lido del nodo hijo: "%0:s" para el nodo padre: "%1:s" de "%2:s".\n \nDebe revisar la definici�n del Meta4Object.
#define M4_CH_DM_ITEMISEOF_CNB                      0x0059 + M4_CH_DM_ERROR_BASE
//Se llam� a la operaci�n "%0:s" y el elemento no es v�lido.\n \nMeta4Object no creado, informaci�n de contexto no disponible.\nDebe consultar con el administrador.
#define M4_CH_DM_ITEMCALLNOACCESS                   0x005A + M4_CH_DM_ERROR_BASE
//Meta4Object no v�lido.\n \nIntento de ejecuci�n del elemento "%0:s" en "%1:s" de "%2:s".\nDebe consultar con el administrador.
#define M4_CH_DM_RECORDSETEXEBLOCKNIHIL             0x005C + M4_CH_DM_ERROR_BASE
//Intento de ejecuci�n de "%0:s" en "%1:s" de "%2:s" sobre un conjunto de filas no v�lido.\n \nDebe comprobar si el nodo padre est� colocado correctamente.
#define M4_CH_DM_RSEXEPOPERROR                      0x005D + M4_CH_DM_ERROR_BASE
//Error al ejecutar "%0:s" de "%1:s" de"%2:s".\n \nDebe revisar los dem�s errores de la pila.
#define M4_CH_DM_ACCESSLOAD                         0x005E + M4_CH_DM_ERROR_BASE
//Error al cargar "%0:s".\n \nDebe revisar los dem�s errores de la pila.
#define M4_CH_DM_ACCESSPERSIST                      0x005F + M4_CH_DM_ERROR_BASE
//Error al guardar "%0:s".\n \nDebe revisar los dem�s errores de la pila.
#define M4_CH_DM_DEBUGMONOBLOCK                     0x0060 + M4_CH_DM_ERROR_BASE
//Conjunto de filas borrado en el nodo monobloque "%0:s" de "%1:s".
#define M4_CH_DM_ITEMDONTEXIST2                     0x0062 + M4_CH_DM_ERROR_BASE
//Intento de ejecuci�n de un "%0:s" que no existe.\n \nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_EXTERNAL_ACCESS_DETECTED           0x0068 + M4_CH_DM_ERROR_BASE
//Se detect� una referencia externa al anexar/separar.\n \nDebe asegurarse de que no haya referencias externas antes de anexar/separar.
#define M4_CH_DM_INCONSISTENCY_REFERENCE            0x0069 + M4_CH_DM_ERROR_BASE
//Referencia no v�lida a un Gestor de Meta4Objects al agregar/separar en Meta4Object "%0:s".\n \nTodos los Meta4Object's generados tienen que pertenecer al mismo gestor.
#define M4_CH_DM_SET_VALUE_CONVERSION               0x006A + M4_CH_DM_ERROR_BASE
//Conversi�n de valores necesaria.\n \nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� al asignar un valor "%2:s" en "%3:s" de "%4:s". Tipo del valor a asignar :"%0:s". Tipo del elemento: "%1:s".
#define M4_CH_DM_OPERATION_VALUE_WITH_INDEX         0x006B + M4_CH_DM_ERROR_BASE
//Error durante una sesi�n de b�squeda al leer "%1:s" en la fila "%0:s".\n \nDebe revisar los dem�s errores de la pila.\nEsto sucedi� en "%2:s" de "%3:s".
#define M4_CH_DM_CHANNELMANAGERNULL                 0x006C + M4_CH_DM_ERROR_BASE
//El Gestor de Meta4Objects es nulo.\n \nDebe consultar con el administrador.
#define M4_CH_DM_TYPE_MISMATCH                      0x006D + M4_CH_DM_ERROR_BASE
//No coinciden los tipos en el argumento "%0:s" para la funci�n de b�squeda en "%2:s" de "%3:s". El tipo real es "%1:s", pero debe ser: "N�mero".\n \nDebe revisar la definici�n del Meta4Object
#define M4_CH_DM_WRONG_PARAM_NUM                    0x006E + M4_CH_DM_ERROR_BASE
//N�mero no v�lido de argumentos: "%0:s" en la funci�n de b�squeda.\n \nDebe ser un m�ltiplo de 3 o de 3 + 1.\nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_NO_SUCH_ITEM                       0x006F + M4_CH_DM_ERROR_BASE
//"%0:s" no existe en "%1:s" de "%2:s" mientras se utiliza la funci�n de b�squeda.\n \nDebe revisar la definici�n del Meta4Object.
#define M4_CH_DM_SECURITY_ITEM_READ                 0x0070 + M4_CH_DM_ERROR_BASE
//Acceso de lectura denegado.\n \nAcceso de lectura a "%0:s" de "%1:s" denegado.\nDebe verificar su perfil.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_SECURITY_ITEM_WRITE                0x0071 + M4_CH_DM_ERROR_BASE
//Acceso de modificaci�n denegado.\n \nAcceso de modificaci�n en "%0:s" de "%1:s" denegado.\nDebe verificar su perfil.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_SECURITY_NODE_WRITE                0x0072 + M4_CH_DM_ERROR_BASE
//Acceso de escritura denegado.\n \nNo puede insertar filas porque no tiene los permisos necesarios.\nDebe verificar su perfil.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_SECURITY_NODE_DELETE               0x0073 + M4_CH_DM_ERROR_BASE
//Acceso de borrado denegado.\n \nNo puede borrar filas porque no tiene los permisos necesarios.\nDebe verificar su perfil.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_PARTIAL_LOAD_INVALID_OP            0x0074 + M4_CH_DM_ERROR_BASE
//Intento de a�adir o borrar filas en carga parcial.\n \nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� en "%0:s" de"%1:s".
#define M4_CH_DM_PARTIAL_LOAD_NHIHIL                0x0076 + M4_CH_DM_ERROR_BASE
//Intento de utilizaci�n un conjunto de filas no v�lido en carga parcial.\n \nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_FILTER_RAM_NIHIL                   0x0078 + M4_CH_DM_ERROR_BASE
//Intento de utilizaci�n un conjunto de filas no v�lido con filtros RAM.\n \nDebe comprobar si el nodo padre est� colocado correctamente.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_OPERATION_VALUE_WITH_INDEX_EOF     0x007C + M4_CH_DM_ERROR_BASE
//Error durante una sesi�n de b�squeda en la columna "%0:s".\n \nNo se pudo acceder a la columna "%0:s" en la funci�n "FindRegister".\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_INVALIDAUTOLOADMODE_NODE           0x007D + M4_CH_DM_ERROR_BASE
//Intento de aplicar un modo de carga autom�tica no v�lido "%0:s".\n \nDebe revisar la definici�n del Meta4Object\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_ADDSLICES_DATESINVALID             0x0080 + M4_CH_DM_ERROR_BASE
//Error al agregar tramos porque la fecha de fin "%1:s" es menor que la fecha de inicio "%0:s".\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedi� en "%2:s" de "%3:s" de "%4:s".
#define M4_CH_DM_SORT_FILTER_ITEM_TYPE_MISMATCH     0x0081 + M4_CH_DM_ERROR_BASE
//Error al crear filtro de Ordenaci�n porque el elemento "%0:s" no es una cadena, n�mero o fecha.\n \nDebe revisar la definici�n del Meta4Object. \nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_RAM_FILTER_FAILURE                 0x0082 + M4_CH_DM_ERROR_BASE
//Error operando con filtros RAM.\n \nDebe revisar los dem�s errores de la pila.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_RAM_FILTER_CREATION                0x0083 + M4_CH_DM_ERROR_BASE
//No se puede crear el filtro RAM.\n \nDebe revisar los dem�s errores de la pila.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_SORT_FILTER_NO_ITEM                0x0084 + M4_CH_DM_ERROR_BASE
//No se puede crear el filtro de ordenaci�n porque no existe "%0:s".\n \nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_SORT_FILTER_BAD_ORDER              0x0085 + M4_CH_DM_ERROR_BASE
//Orden no v�lido en el filtro de ordenaci�n.\n \nLos valores posibles son: -1 = Ascendente y 1 = Descendente. El orden por defecto es el ascendente.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_ITEM_TYPE_MISMATCH2                0x0086 + M4_CH_DM_ERROR_BASE
//No coinciden los tipos en el argumento "%0:s" y la columna "%1:s" para la funci�n de b�squeda.\n \nEl tipo real es "%2:s", pero debe ser: "N�mero" o "Cadena".\nEsto sucedi� en "%3:s" de "%4:s".
#define M4_CH_DM_SEC_GET_IN_PARENT                  0x0087 + M4_CH_DM_ERROR_BASE
//Acceso de lectura denegado.\n \nNo tiene los permisos necesarios de lectura en "%0:s" para propagarse.\nDebe verificar su perfil.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_SECURITY_ITEM_WRITE_SETVALUE       0x008A + M4_CH_DM_ERROR_BASE
//Acceso de escritura denegado.\n \nNo tiene los permisos para modificar el campo "%0:s".\nDebe verificar su perfil.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_DATEOUTOFRANGE_SETDATE             0x008B + M4_CH_DM_ERROR_BASE
//Intento de definir una fecha de tramo no v�lida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedi� con la fecha "%0:s" en "%1:s" de "%2:s" de "%3:s".
#define M4_CH_DM_ERASER_DESTROY		                0x008C + M4_CH_DM_ERROR_BASE
//Error al borrar el "%0:s" usando el destructor de objetos.\n \nDebe avisar al administrador del servidor de aplicaciones.
#define M4_CH_DM_ITEMOUTOFRANGE_GETVALUE            0x008D + M4_CH_DM_ERROR_BASE
//Intento de lectura de un elemento que no existe.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_ITEMOUTOFRANGE_SETVALUE            0x008E + M4_CH_DM_ERROR_BASE
//Intento de asignaci�n de un elemento que no existe.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_SLICEISEOF_GETVALUE                0x0090 + M4_CH_DM_ERROR_BASE
//Intento de lectura de un elemento del valor de un tramo en posici�n no v�lida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_SLICEISEOF_SETVALUE                0x0091 + M4_CH_DM_ERROR_BASE
//Intento de asignaci�n de un elemento del valor de un tramo en posici�n no v�lida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_VALIDATION_NIHILBLOCK              0x0092 + M4_CH_DM_ERROR_BASE
//Intento de realizaci�n de una operaci�n en un conjunto de filas no v�lido.\n \nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".\n \nDebe comprobar que el nodo padre est� colocado correctamente o tiene filas.
#define M4_CH_DM_VALIDATION_NIHILBLOCK_GETVALUE     0x0093 + M4_CH_DM_ERROR_BASE
//Intento de lectura de un elemento en un conjunto de filas no v�lido.\n \nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".\nDebe comprobar que el nodo padre est� colocado correctamente o tiene filas.
#define M4_CH_DM_VALIDATION_NIHILBLOCK_SETVALUE     0x0094 + M4_CH_DM_ERROR_BASE
//Intento de asignaci�n de un elemento en un conjunto de filas no v�lido.\n \nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".\nDebe comprobar que el nodo padre est� colocado correctamente o tiene filas.
#define M4_CH_DM_VALIDATION_REGEOF                  0x0095 + M4_CH_DM_ERROR_BASE
//Intento de realizaci�n de una operaci�n en una fila no v�lida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_VALIDATION_REGEOF_GETVALUE         0x0096 + M4_CH_DM_ERROR_BASE
//Intento de lectura de un elemento en una fila no v�lida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_VALIDATION_REGEOF_SETVALUE         0x0097 + M4_CH_DM_ERROR_BASE
//Intento de asignaci�n de un elemento en una fila no v�lida.\n \nDebe revisar las operaciones realizadas anteriormente en el Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_NULL_RWT                           0x0098 + M4_CH_DM_ERROR_BASE
//Intento de realizaci�n de una operaci�n en un estado no v�lido.\n \nDebe revisar los dem�s errores de la pila.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_NULL_RWT_GETVALUE                  0x0099 + M4_CH_DM_ERROR_BASE
//Intento de lectura en un estado no v�lido.\n \nDebe revisar los dem�s errores de la pila.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_NULL_RWT_SETVALUE                  0x009A + M4_CH_DM_ERROR_BASE
//Intento de asignaci�n en un estado no v�lido.\n \nDebe revisar los dem�s errores de la pila.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_NIHILBLOCK_COUNT                   0x009B + M4_CH_DM_ERROR_BASE
//Intento de obtener el n�mero de filas de un conjunto de filas no v�lido.\n \nDebe comprobar que el nodo padre est� colocado correctamente o tiene filas.
#define M4_CH_DM_NIHILBLOCK_ADD                     0x009D + M4_CH_DM_ERROR_BASE
//Intento de a�adir filas de un conjunto de filas no v�lido.\n \nDebe comprobar que el nodo padre est� colocado correctamente o tiene filas.
#define M4_CH_DM_NIHILBLOCK_DELETE                  0x009E + M4_CH_DM_ERROR_BASE
//Intento de borrar filas de un conjunto de filas no v�lido.\n \nDebe comprobar que el nodo padre est� colocado correctamente o tiene filas.
#define M4_CH_DM_FILTER_RAM_NIHIL2                  0x0101 + M4_CH_DM_ERROR_BASE
//Error al usar filtros en RAM sobre un conjunto de filas no v�lido.\n \nDebe comprobar que el nodo padre est� colocado correctamente o tiene filas.\nEsto sucedi� en "%0:s".
#define M4_CH_DM_FRIEND_OTHER_MANAGER               0x0102 + M4_CH_DM_ERROR_BASE
//Intento de establecer un M4Object compa�ero de un gestor distinto.\n \nEsto sucedi� con el M4Object "%0:s".\n \nDebe establecer compa�eros con M4Objects de un mismo gestor.
#define M4_CH_DM_CACHE_PUT_TEMPORAL_CHANNEL			0x0103 + M4_CH_DM_ERROR_BASE
//Intento de poner un Meta4Object temporal en la cach� de datos.\n \nEl Meta4Object "%0:s" es temporal y no debe ser almacenado en cach�.\nDebe marcar el Meta4Object como no almacenable en cach�.
#define M4_CH_DM_CACHE_GET_TEMPORAL_CHANNEL			0x0104 + M4_CH_DM_ERROR_BASE
//Intento de obtener un Meta4Object temporal de la cach� de datos.\n \nEl Meta4Object "%0:s" es temporal y no debe ser almacenado en cach�.\nDebe marcar el Meta4Object como no almacenable en cach�.
#define M4_CH_DM_CANNOT_OPEN_LOADFILE				0x0105 + M4_CH_DM_ERROR_BASE
//Error al cargar un Meta4Object de archivo.\n \nNo se pudo abrir el archivo "%0:s".\nDebe comprobar que el archivo existe y tiene permisos de lectura.
#define M4_CH_DM_CANNOT_OPEN_PERSISTFILE			0x0106 + M4_CH_DM_ERROR_BASE
//Error al guardar un Meta4Object a archivo.\n \nNo se pudo abrir el archivo "%0:s" para guardar el Meta4Object "%1:s".\nDebe comprobar la ruta existe y tiene permisos de escritura.
#define M4_CH_DM_CANNOT_LOAD_CHANNEL_FROM_FILE		0x0107 + M4_CH_DM_ERROR_BASE
//Error al cargar un Meta4Object de archivo.\n \nNo se pudo cargar el Meta4Object "%1:s" del archivo "%0:s". Posiblemente el archivo no es v�lido.\nDebe consultar con el administrador.
#define M4_CH_DM_CANNOT_PERSIST_CHANNEL_TO_FILE		0x0108 + M4_CH_DM_ERROR_BASE
//Error al guardar un Meta4Object a archivo.\n \nNo se pudo guardar el Meta4Object "%1:s" al archivo "%0:s".\nDebe consultar con el administrador.
#define M4_CH_DM_DIFFERENT_CHANNEL_LOADFROMFILE		0x0109 + M4_CH_DM_ERROR_BASE
//Error al cargar un Meta4Object de archivo.\n \nNo se pudo cargar de archivo el Meta4Object "%0:s" en el Meta4Object "%5:s". Son de distinto tipo.\nDebe consultar con el administrador.\nAtributos del Meta4Object "%0:s" del archivo: tipo cliente servidor "%1:s" - lenguaje "%2:s" - m�scara de seguridad "%3:d" - versi�n "%4:d".\nAtributos del Meta4Object "%5:s" destino: tipo cliente servidor "%6:s" - lenguaje "%7:s" - m�scara de seguridad "%8:d" - versi�n "%9:d".
#define M4_CH_DM_CANNOT_PERSIST_CHNOTBUILT			0x010A + M4_CH_DM_ERROR_BASE
//Error al guardar un Meta4Object a archivo.\n \nNo se pudo guardar el Meta4Object al archivo "%0:s" porque no est� construido.\nDebe construir el Meta4Object antes de guardarlo a archivo.
#define M4_CH_DM_CACHE_PUT_PROXY_CHANNEL			0x010B + M4_CH_DM_ERROR_BASE
//Intento de poner un Meta4Object proxy en la cach� de datos.\n \nEl Meta4Object "%0:s" es proxy y no debe ser almacenado en cach�.\nDebe marcar el Meta4Object como no almacenable en cach�.
#define M4_CH_DM_CACHE_GET_PROXY_CHANNEL			0x010D + M4_CH_DM_ERROR_BASE
//Intento de obtener un Meta4Object proxy de la cach� de datos.\n \nEl Meta4Object "%0:s" es proxy y no debe ser almacenado en cach�.\nDebe marcar el Meta4Object como no almacenable en cach�.
#define M4_CH_DM_FULL_SERIALIZE_TEMPORAL_CHANNEL	0x010F + M4_CH_DM_ERROR_BASE
//Intento de serializar completamente un Meta4Object temporal.\n \nEl Meta4Object "%0:s" es temporal y no puede ser serializado completamente.\nDebe consultar con el administrador.
#define M4_CH_DM_FULL_SERIALIZE_PROXY_CHANNEL		0x0110 + M4_CH_DM_ERROR_BASE
//Intento de serializar completamente un Meta4Object proxy.\n \nEl Meta4Object "%0:s" es proxy y no puede ser serializado completamente.\nDebe consultar con el administrador.
#define M4_CH_DM_INCONSISTENCY_REFNULL              0x0112 + M4_CH_DM_ERROR_BASE
//Gestor de Meta4Objects nulo al agregar/separar en Meta4Object.\n \nTodos los Meta4Objects deben tener un Gestor v�lido asociado.
#define M4_CH_DM_NIHILBLOCK_MEMORY                  0x0113 + M4_CH_DM_ERROR_BASE
//Intento de realizaci�n de una operaci�n en un conjunto de filas no v�lido.\n \nEsto se debe a que se ha excedido el n�mero m�ximo de bloque en el nodo.\nDebe comprobar la definici�n del Meta4Object.
#define M4_CH_DM_NIHILBLOCK_COUNT_MEMORY            0x0114 + M4_CH_DM_ERROR_BASE
//Intento de obtener el n�mero de filas en un conjunto de filas no v�lido.\n \nEsto se debe a que se ha excedido el n�mero m�ximo de bloque en el nodo.\nDebe comprobar la definici�n del Meta4Object.
#define M4_CH_DM_NIHILBLOCK_ADD_MEMORY              0x0115 + M4_CH_DM_ERROR_BASE
//Intento de a�adir filas en un conjunto de filas no v�lido.\n \nEsto se debe a que se ha excedido el n�mero m�ximo de bloque en el nodo.\nDebe comprobar la definici�n del Meta4Object.
#define M4_CH_DM_NIHILBLOCK_DELETE_MEMORY           0x0116 + M4_CH_DM_ERROR_BASE
//Intento de borrar filas en un conjunto de filas no v�lido.\n \nEsto se debe a que se ha excedido el n�mero m�ximo de bloque en el nodo.\nDebe comprobar la definici�n del Meta4Object.
#define M4_CH_DM_NUM_RECORDS                        0x0117 + M4_CH_DM_ERROR_BASE
//Alcanzado el limite maximo de filas por bloque.\n \nIntento de a�adir filas en un bloque que alcanz� el n�mero m�ximo de filas permitido.\nDebe comprobar la definici�n del Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_BAD_DATAL_MAGIC_NUMBER				0x0118 + M4_CH_DM_ERROR_BASE
//Formato incorrecto.\n \nEl formato de codificaci�n del Meta4Object es incorrecto.\nDebe consultar con el administrador.
#define M4_CH_DM_FRIEND_WITHOUT_L2                  0x0119 + M4_CH_DM_ERROR_BASE
//Intento de establecer un M4Object compa�ero sin existir enlace de nivel 2 entre los M4Objects.\n \nDebe establecer primero un enlace de nivel 2 entre los M4Objects.\nEsto sucedi� entre "%0:s" (padre de la relaci�n) y "%1:s" (hijo de la relaci�n).
#define M4_CH_DM_FRIEND_MYSELF						0x011A + M4_CH_DM_ERROR_BASE
//Un M4Object se ha hecho compa�ero de si mismo.\n \nEsto sucedi� en el M4Object "%0:s".\n \nNo debe hacer un M4Object compa�ero de si mismo.
#define M4_CH_DM_FRIEND_DOUBLE_DEL					0x011B + M4_CH_DM_ERROR_BASE
//Se ha intentado borrar un M4Object compa�ero, que no lo era.\n \nPuede que est� intentando borrar el M4Object compa�ero "%0:s" m�s de una vez.\n \nNo debe borrar M4Objects compa�eros m�s de una vez.
#define M4_CH_DM_BAD_ITEM_CONECTOR					0x011C + M4_CH_DM_ERROR_BASE
//Error en la conexi�n entre elementos.\n \nEl �mbito del elemento padre de la conexi�n debe ser mayor o igual que el ambito del elemento hijo de la conexi�n.\nDebe comprobar la definici�n del M4Object.\Esto sucedi� entre el elemento padre de la conexi�n "%0:s", con �mbito "%1:n" y el elemento hijo de la conexi�n "%2:s", con �mbito "%3:n"."
#define M4_CH_DM_NO_MEMORY							0x011D + M4_CH_DM_ERROR_BASE
//Recursos insuficientes.\n \nNo hay memoria suficiente para obtener "%0:s" bytes.\nDebe cerrar alguna aplicaci�n.\nEsto ha sucedido en la l�nea "%1:s" del archivo "%2:s".
#define M4_CH_DM_INTERNAL_ERROR						0x011E + M4_CH_DM_ERROR_BASE
//Error interno.\n \nDebe consultar con el administrador.\nEsto ha sucedido en la l�nea "%0:s" del fichero "%1:s".
#define M4_CH_DM_BAD_ORG_TYPE_TYPE					0x011F + M4_CH_DM_ERROR_BASE
//Tipo de sociedad incorrecto.\n \nEl tipo de sociedad debe ser num�rico y es "%0:s". Se asumir� el valor de su definici�n "%1:s".\nDebe consultar con el administrador.
#define M4_CH_DM_BAD_ORG_TYPE						0x0120 + M4_CH_DM_ERROR_BASE
//Tipo de sociedad incorrecto.\n \nEl tipo de sociedad vale "%0:s" y deber�a estar comprendido entre "%1:s" y "%2:s". Se asumir� el valor de su definici�n "%3:s".\nDebe consultar con el administrador.
#define M4_CH_DM_BAD_BUILD_META_CHANNEL				0x0121 + M4_CH_DM_ERROR_BASE
//Meta4Object del sistema mal definido.\n \nNo se ha podido generar el Meta4Object del sistema.\nDebe consultar con el administrador.
#define M4_CH_DM_NULL_ROLE							0x0122 + M4_CH_DM_ERROR_BASE
//El perfil vac�o es inv�lido.\n \nDebe consultar con el administrador."
#define M4_CH_DM_ATTR_TRUNCATED						0x0123 + M4_CH_DM_ERROR_BASE
//Atributo de Meta4Object truncado.\n \nEl atributo "%0:s" tienen un valor "%1:s" supera su m�xima longitud "%2:s" y se usar� "%3:s".\nDebe consultar con el administrador.
#define M4_CH_DM_STT_NO_STATE_STARTED				0x0124 + M4_CH_DM_ERROR_BASE
//Estad�sticas no iniciadas.\n \nNo hay estado de estad�sticas para la operaci�n "%0:s".\nDebe consultar al administrador.
//Antiguo #define M4_CH_VM_STT_NO_STATE_STARTED					M4_CH_VM_ERROR_BASE + 0x0178
#define M4_CH_DM_ITEM_NOT_FOUND						0x0125 + M4_CH_DM_ERROR_BASE
//Elemento inexistente.\n \n"%0:s" de "%1:s" de "%2:s" no existe.\nDebe definir el elemento.\nEsto sucedi� al ejecutar la funci�n "%3:s".
//Antiguo #define M4_CH_VM_ITEM_NOT_FOUND							M4_CH_VM_ERROR_BASE + 0x0141
#define M4_CH_DM_NO_SUCH_NODE_STT					0x0126 + M4_CH_DM_ERROR_BASE
//Meta4Object no v�lido.\n \nEl Meta4Object de estad�sticas "%1:s" no es v�lido porque no existe el nodo "%0:s".\nDebe consultar al administrador.
//Antiguo #define M4_CH_VM_NO_SUCH_NODE_STT						M4_CH_VM_ERROR_BASE + 0x0160
#define M4_CH_DM_NO_PERSISTIABLE_OBJECT_ID			0x0127 + M4_CH_DM_ERROR_BASE
// No existe ID en objeto persistente.\n \nDebe consultar con el administrador.
#define M4_CH_DM_WRONG_INSTANCE_HANDLE				0x0128 + M4_CH_DM_ERROR_BASE
//Instancia no encontrada.\n \nNo se encuentra la instancia "%0:s" del Meta4Object "%1:s".\nDebe definir la instancia.
#define M4_CH_DM_BUILD_FROM_ID						0x0129 + M4_CH_DM_ERROR_BASE
//Definici�n de Meta4Object incorrecto.\n \nError al crear la definici�n del Meta4Object "%0:s".
#define M4_CH_DM_ERROR_LOAD							0x012A + M4_CH_DM_ERROR_BASE /*solo debug*/
//No se puede cargar el Meta4Object.\n \nError al cargar el Meta4Object "%0:s".\nDebe revisar errores anteriores.
#define M4_CH_DM_REDEFINING_INSTANCE				0x012B + M4_CH_DM_ERROR_BASE
//Redefinici�n de instancia.\n \nRedefinici�n de la instancia del Meta4Object para c�digo JIT. Cerrando el Meta4Object "%0:s".
#define M4_CH_DM_BAD_ITEM_INDEX						0x012C + M4_CH_DM_ERROR_BASE
//Elemento de �ndice incorrecto.\n \nEl elemento en la posici�n "%0:s" del �ndice "%1:s" no es v�lido.\nDebe cambiar el �ndice.\nEsto sucedi� al ejecutar la funci�n BSearch.
#define M4_CH_DM_BAD_OPERATION_ITEM_INDEX 			0x013C + M4_CH_DM_ERROR_BASE
//Operaci�n no v�lida.\n \nOperaci�n no v�lida para "%0:s" de "%1:s" de "%2:s" del �ndice "%3:s".\nDebe asegurarse de que s�lo el �ltimo elemento del �ndice pueda tener una operaci�n que no sea IGUAL.
#define M4_CH_DM_ITEM_TYPE_MISMATCH					0x013D + M4_CH_DM_ERROR_BASE
//Tipo no v�lido.\n \nEl tipo de "%0:s" de "%1:s" de "%2:s" no es v�lido.\nDebe ser "%4:s" en lugar de "%5:s".\nEsto sucedi� al ejecutar la funci�n "%3:s".
#define M4_CH_DM_STT_STATE_NOT_MATCHED				0x013E + M4_CH_DM_ERROR_BASE
//Estad�sticas incorrectas.\n \nFalta el estado de ejecuci�n de las estad�sticas. "%0:s" no coincide con el estado final "%1:s".\nDebe consultar al administrador.
//Amtiguo #define M4_CH_VM_STT_STATE_NOT_MATCHED					M4_CH_VM_ERROR_BASE + 0x01d2
#define M4_CH_DM_ARGUMENT_TYPE_MISMATCH				0x013F + M4_CH_DM_ERROR_BASE
//Tipo de argumento incorrecto.\n \nEl tipo del argumento "%0:s" es "%1:s" y deber�a ser "%2:s".\nDebe cambiar el tipo del argumento.\nEsto ha sucedido al invocar la funci�n "FindRegister".
#define M4_CH_DM_INVALID_COMPARE_FUNCTION			0x0140 + M4_CH_DM_ERROR_BASE
//N�mero de funci�n no v�lido.\n \nLa funci�n "%0:s" no es una funci�n v�lida.\nDebe usar una funci�n en el rango que va de "%1:s" a "%2:s".\nEsto ha sucedido al invocar la funci�n "FindRegister".
#define M4_CH_DM_NO_GET_ACCESS_L2					0x0141 + M4_CH_DM_ERROR_BASE
//Instancia no encontrada.\n \nNo se encuentra la instancia "%0:s" en una llamada de nivel 2.\nDebe definir la instancia.\nEsto sucedi� al ejecutar la funci�n "%1:s".
#define M4_CH_DM_NO_SUCH_NODE						0x0142 + M4_CH_DM_ERROR_BASE
//Nodo inexistente.\n \n"%0:s" de "%1:s" no existe.\nDebe definir el nodo o eliminar las referencias a �l.\nEsto sucedi� en la funci�n "%2:s".
#define M4_CH_DM_WRONG_ITEM_DEFINITION 				0x0143 + M4_CH_DM_ERROR_BASE
//Definici�n de elemento no v�lido.\n \nDefinici�n no v�lida para "%0:s" de "%1:s" de "%2:s".\nDebe tener "%3:s" argumentos en lugar de "%4:s".
#define M4_CH_DM_CANT_EXECUTE_ITEM					0x0144 + M4_CH_DM_ERROR_BASE
//No se puede ejecutar un elemento.\n \nNo se puede ejecutar "%0:s" de "%1:s" de "%2:s".\nDebe revisar sus permisos.
#define M4_CH_DM_RTS_POP							0x0145 + M4_CH_DM_ERROR_BASE
//Error interno de ejecuci�n.\n \nError al ejecutar extracci�n de RTS. "%0:s".\nDebe consultar al administrador.
#define M4_CH_DM_FORMATTING_ERROR_STRING			0x0146 + M4_CH_DM_ERROR_BASE
//Error al dar formato al mensaje.\n \nNo se puede dar formato a la cadena de error. Se mostrar� el mensaje sin dar formato.\nDebe revisar el formato de su mensaje de error.
#define M4_CH_DM_WRONG_PARAM_NUM_INS_1				0x0147 + M4_CH_DM_ERROR_BASE
//N�mero de argumentos no v�lido.\n \nLa funci�n "%3:s" usa argumentos variables y la definici�n del elemento espera un n�mero fijo.\nDebe cambiar la definici�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_DM_NO_READ_PROP						0x0148 + M4_CH_DM_ERROR_BASE
//Acceso de lectura denegado.\n \nNo tiene permisos para leer un atributo de Meta4Object.\nDebe consultar con el administrador.
#define M4_CH_DM_NO_WRITE_PROP						0x0149 + M4_CH_DM_ERROR_BASE
//Acceso de modificaci�n denegado.\n \nNo tiene permisos para modificar un atributo de Meta4Object.\nDebe consultar con el administrador.
#define M4_CH_DM_DELETE_PROP						0x014A + M4_CH_DM_ERROR_BASE
//Acceso de borrado denegado.\n \nNo tiene permisos para borrar el atributo de Meta4Object "%0:s".\nDebe consultar con el administrador.
#define M4_CH_DM_PROPAGATE_BLOCK_NIHIL_IN_SON		0x014B + M4_CH_DM_ERROR_BASE
//No se puede propagar el valor del elemento.\n \nNo se puede propagar el valor del elemento porque la relaci�n en modo "libre" entre elementos de tipo "bloque" no queda un�vocamente determinada en el nodo hijo.\nDebe revisar la estructura de conectores de su Meta4Object y comprobar que no tiene listas definidas en el nodo hijo.\nEsto sucedi� al intentar propagar el valor "%0:s" entre el elemento padre de la relaci�n "%1:s" de "%2:s" de "%3:s" y el elemento hijo "%4:s" de "%5:s" de "%6:s".
#define M4_CH_DM_PROPAGATE_BLOCK_NULL_IN_SONMETH	0x014C + M4_CH_DM_ERROR_BASE
//No se puede propagar el valor del elemento.\n \nNo se puede propagar el valor del elemento porque el elemento hijo es de tipo "m�todo".\nDebe revisar la estructura de conectores de su Meta4Object.\nEsto sucedi� al intentar propagar el valor "%0:s" entre el elemento padre de la relaci�n "%1:s" de "%2:s" de "%3:s" y el elemento hijo "%4:s" de "%5:s" de "%6:s".
#define M4_CH_DM_SET_ITEM_NULL						0x014D + M4_CH_DM_ERROR_BASE
//Intento de escritura sobre un Item de tipo nulo.\n \nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� al asignar un valor al Item "%0:s" en "%1:s" de "%2:s".
#define M4_CH_DM_M4OBJ_AREADY_BUILT					0x014E + M4_CH_DM_ERROR_BASE
//Construcci�n repetida.\n \nNo se puede construir el Meta4Object porque ya est� construido.\nDebe consultar con el administrador.
#define M4_CH_DM_CACHEABLE_DOC						0x014F + M4_CH_DM_ERROR_BASE
//Documento no v�lido.\n \nNo se puede utilizar un documento cacheable en una lista.\nDebe cambiar el modo cacheable del Meta4Object documento.\nEsto sucedi� conectando el documento "%0:s" de "%1:s" a la lista "%2:s" de "%3:s".
#define M4_CH_DM_PARTIAL_LOAD_TOTALIZE	            0x0150 + M4_CH_DM_ERROR_BASE
//Intento de totalizar en carga parcial.\n \nNo se puede ejecutar la funci�n de totalizaci�n "%0:s" en un nodo con carga parcial.\nDebe revisar la definici�n del Meta4Object.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_DM_STT_BAD_STT_M4OBJ					0x0151 + M4_CH_DM_ERROR_BASE
//Meta4Object de estad�sticas incorrecto.\n \nEl Meta4Object "%0:s" no es un Meta4Object v�lido para las estad�sticas.\nDebe consultar con el administrador.
#define M4_CH_DM_STT_BAD_INTERMEDIATE_VERSION		0x0152 + M4_CH_DM_ERROR_BASE
//Versi�n de estad�sticas incorrecta.\n \nEl archivo "%0:s" tiene versi�n de estad�sticas "%1:s" y la versi�n actual es "%2:s".\nDebe consultar con el administrador.
#define M4_CH_DM_STT_NO_READ_INTERMEDIATE	        0x0153 + M4_CH_DM_ERROR_BASE
//Error al leer estad�sticas.\n \nNo se pueden leer las estad�sticas del archivo "%0:s".\nDebe consultar con el administrador.
#define M4_CH_DM_STT_NO_WRITE_INTERMEDIATE	        0x0154 + M4_CH_DM_ERROR_BASE
//Error al escribir estad�sticas.\n \nNo se pueden escribir las estad�sticas al archivo "%0:s".\nDebe consultar con el administrador.
#define M4_CH_DM_STT_NO_COMPRESS_INTERMEDIATE       0x0155 + M4_CH_DM_ERROR_BASE
//Error comprimiendo archivo de estad�sticas.\n \nNo se puede comprimir el archivo de estad�sticas "%0:s" al archivo "%1:s".\nDebe consultar con el administrador.
#define M4_CH_DM_STT_NO_DECOMPRESS_INTERMEDIATE     0x0156 + M4_CH_DM_ERROR_BASE
//Error descomprimiendo archivo de estad�sticas.\n \nNo se puede descomprimir el archivo de estad�sticas "%0:s" al archivo "%1:s".\nDebe consultar con el administrador.
#define M4_CH_DM_NULL_ARGUMENT						0x0157 + M4_CH_DM_ERROR_BASE
// Argumento nulo a una funci�n.\n \nEl argumento "%0:s" del m�todo "%1:s" es nulo.\nDebe consultar con el administrador.\nEsto sucedi� en la l�nea "%2:s" del archivo "%3:s".
#define M4_CH_DM_NO_OPEN_PL_FILE					0x0158 + M4_CH_DM_ERROR_BASE
//No se puede abrir archivo.\n \nNo se puede abrir el archivo "%0:s" en modo "%1:s" por la raz�n "%2:s".\nDebe consultar con el administrador.\nEsto sucedi� en la carga parcial de "%3:s" de "%4:s".
#define M4_CH_DM_NO_WRITE_PL_RECORD					0x0159 + M4_CH_DM_ERROR_BASE
//No se puede guardar registro en archivo.\n \nNo se puede guardar el registro "%0:s" en el archivo "%1:s".\nDebe consultar con el administrador.\nEsto sucedi� en la carga parcial de "%2:s" de "%3:s".
#define M4_CH_DM_NO_READ_PL_RECORD					0x015A + M4_CH_DM_ERROR_BASE
//No se puede recuperar registro de archivo.\n \nNo se puede recuperar el registro "%0:s" del archivo "%1:s".\nDebe consultar con el administrador.\nEsto sucedi� en la carga parcial de "%2:s" de "%3:s".
#define M4_CH_DM_NO_ORDER_PL_INDEX					0x015B + M4_CH_DM_ERROR_BASE
//No se puede ordenar el bloque.\n \nNo se puede ordenar el bloque de carga parcial.\nDebe consultar con el administrador.\nEsto sucedi� en la carga parcial de "%0:s" de "%1:s".
#define M4_CH_DM_NO_BRACKETS_IN_PL					0x015C + M4_CH_DM_ERROR_BASE
//Movimiento no permitido en carga parcial.\n \nNo se puede utilizar el operador corchete para moverse al registro "%0:s" en un nodo de carga parcial porque no est� dentro de la ventana actual que va del registro "%1:s" al "%2:s".\nDebe consultar con el administrador.\nEsto sucedi� en la carga parcial de "%3:s" de "%4:s".
#define M4_CH_DM_NO_ENCRYPT							0x015D + M4_CH_DM_ERROR_BASE
//Error al encriptar datos.\n \nSe ha producido un error al encriptar los datos en la cach� de Meta4Objects de tipo delta.\nDebe consultar con el administrador.
#define M4_CH_DM_NO_DECRYPT							0x015E + M4_CH_DM_ERROR_BASE
//Error al desencriptar datos.\n \nSe ha producido un error al desencriptar los datos en la cach� de Meta4Objects de tipo delta.\nDebe consultar con el administrador.
#define M4_CH_DM_REPEATED_INSTANCE					0x015F + M4_CH_DM_ERROR_BASE
//Instancia repetida.\n \nLa instancia de nivel 2 "%0:s" no puede ser a�adida al Meta4Object "%1:s" porque ya existe.\nDebe utilizar otro identificador de instancia o eliminar la instancia existente.
#define M4_CH_DM_NO_SUCH_META4OBJ					0x0160 + M4_CH_DM_ERROR_BASE
//Meta4Object no encontrado.\n \nEl Meta4Object definido por el identificador de instancia "%0:s" no existe.\nCompruebe que el identificador de instancia se haya obtenido en el mismo entorno cliente/servidor en que se est� utilizando.\nEsto sucedi� al a�adir la instancia de nivel 2 "%1:s" al Meta4Object "%2:s".
#define M4_CH_DM_BAD_CHANGE_ORG						0x0161 + M4_CH_DM_ERROR_BASE
//Sociedad no v�lida.\n \nNo se puede cambiar la sociedad "%0:s" a la sociedad "%1:s" porque no tiene los mismos mapeos al Meta4Object "%2:s".\nDebe elegir una sociedad que cumpla los mapeos.
#define M4_CH_DM_BAD_INIT_AS_MY_PARENT				0x0162 + M4_CH_DM_ERROR_BASE
//Elemento no v�lido.\n \nSe est� intentando utilizar el elemento "%0:s" de "%1:s" de "%2:s" que no tiene valor asociado.\nPosiblemente se est� intentando acceder a un elemento encarrilado que se ha borrado en la parte server.
#define M4_CH_DM_ROWS_QUOTA_EXCEEDED                0x0163 + M4_CH_DM_ERROR_BASE
//Se alcanz� el n�mero m�ximo de filas por transacci�n.\n \nSe intent� a�adir filas durante la transacci�n por encima del m�ximo permitido.\nDebe comprobar la definici�n del Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_CH_DM_TOO_MANY_FILTERS	                0x0164 + M4_CH_DM_ERROR_BASE
//Se alcanz� el n�mero m�ximo de filtros permitidos.\n \nSe intent� a�adir m�s de "%0:s" filtros.\nDebe revisar la definici�n del Meta4Object\nEsto sucedi� en "%1:s" de "%2:s".

#endif // __DMRES_HPP

