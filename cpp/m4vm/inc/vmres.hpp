#ifndef __VMRES_HPP
#define __VMRES_HPP

#include "m4res.hpp"

//----------------------------
//Instalaci�n
//----------------------------

#define M4_CH_VM_ADD_TO_POOL_INS						M4_CH_VM_ERROR_BASE + 0x0000
//Recursos agotados.\n \nNo se pueden obtener recursos suficientes para instalar una constante alfanum�rica.\nDebe consultar con el administrador.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_BAD_POINTER_INS						M4_CH_VM_ERROR_BASE + 0x0001 //solo debug
//Error interno de instalaci�n.\n \nEl apuntador "%3:s" no es correcto.\nDebe consultar con el administrador.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_CANT_CONNECT_ITEMS						M4_CH_VM_ERROR_BASE + 0x0002
//Conector err�neo.\n \nNo se puede conectar este elemento porque el elemento padre tiene un n�mero de argumentos variable.\nDebe desconectar estos dos elementos.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s". 
#define M4_CH_VM_FUNCTION_NOT_FOUND						M4_CH_VM_ERROR_BASE + 0x0003
//Funci�n de tipo CPP inexistente.\n \nEl elemento est� asignado a la funci�n "%3:s" que no existe.\nDebe cambiar el m�todo al que mapea el elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_NO_ILLEGAL_INSTRUCTION_SERIALIZE 		M4_CH_VM_ERROR_BASE + 0x0004
//Tipo ilegal de instrucci�n.\n \nEl tipo de instrucci�n "%3:s" no es correcto.\nDebe revisar la integridad de sus datos o volver a compilar el elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_NO_ITEM_FORMULA_PREGETSIZE             M4_CH_VM_ERROR_BASE + 0x0005
//C�digo compilado incorrecto.\n \nEl c�digo polaco "%3:s" no es correcto.\nDebe volver a compilar el elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_NO_SUCH_INSTANCE_INS					M4_CH_VM_ERROR_BASE + 0x0006
//Instancia no v�lida.\n \nLa instancia de Meta4Object "%3:s" no es v�lida.\nDebe cambiar el nombre de instancia o definirla.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_NO_SUCH_ITEM_IN_VIEW					M4_CH_VM_ERROR_BASE + 0x0007
//Elemento inexistente.\n \nEl elemento "%3:s" de "%4:s" no existe en este tipo de cliente/servidor. Debe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_NO_SUCH_NODE_IN_VIEW					M4_CH_VM_ERROR_BASE + 0x0008
//Nodo inexistente.\n \nNo existe el nodo "%3:s" en este tipo de cliente/servidor.\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_T3_ALIAS_ID_INS						M4_CH_VM_ERROR_BASE + 0x0009
//Alias inexistente.\n \nEl ID de alias del Meta4Object "%3:s" no es correcto.\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_TOO_MUCH_LABELS_INS           			M4_CH_VM_ERROR_BASE + 0x000A
//C�digo demasiado complejo.\n \nNo se puede instalar el c�digo del elemento por ser demasiado complejo.\nDebe simplificar el c�digo o partir en varios elementos.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_TRACE_INSTALL_UNUSED  					M4_CH_VM_ERROR_BASE + 0x000B //no se usa**
	/*//Traza de instalaci�n.\n \nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".*/
#define M4_CH_VM_UNDEFINED_ITEM_INS_UNUSED				M4_CH_VM_ERROR_BASE + 0x000C //no se usa**
	/*//Elemento no definido.\n \nElemento no definido "%3:s".\nDebe definirlo.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".*/
#define M4_CH_VM_WRONG_ARG_MAPPING  					M4_CH_VM_ERROR_BASE + 0x000D
//Mapeo incorrecto de argumentos.\n \nNo existe argumento en la posici�n "%3:s" del elemento padre.\nDebe cambiar el mapeo de argumentos.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_ARG_TYPE_MAPPING					M4_CH_VM_ERROR_BASE + 0x000E
//Tipo de mapeo de argumentos incorrecto.\n \nEl tipo de mapeo (a la salida o por referencia) no es correcto en conexiones previas. S�lo se permite mapeo de argumentos de tipo entrada. Debe cambiar los conectores.\n\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_CHANNEL_FUNCTION					M4_CH_VM_ERROR_BASE + 0x000F
//Funci�n de Meta4Object inexistente.\n \nNo existe la funci�n "%3:s" de este Meta4Object.\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_CHANNEL_INS						M4_CH_VM_ERROR_BASE + 0x0010
//Meta4Object inexistente.\n \nNo existe el Meta4Object "%3:s".\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_FUNCTION							M4_CH_VM_ERROR_BASE + 0x0011
//Funci�n inexistente.\n \nLa funci�n "%3:s" no existe.\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_ITEM_HANDLE_INS					M4_CH_VM_ERROR_BASE + 0x0012
//Elemento inexistente.\n \nNo existe el elemento "%3:s" de "%4:s".\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_NODE_HANDLE_INS_UNUSED			M4_CH_VM_ERROR_BASE + 0x0013 //NO SE USA
  /*//Nodo inexistente.\n \nNo existe el nodo "%3:s".\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".*/
#define M4_CH_VM_WRONG_NODE_INS							M4_CH_VM_ERROR_BASE + 0x0014
//Nodo inexistente.\n \nNo existe el nodo "%3:s".\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_OBJECT_FORMAT					M4_CH_VM_ERROR_BASE + 0x0015
//Formato incorrecto de objeto.\n \nAl instalar "%3:s" se detect� que el formato de llamadas es incorrecto.\nDebe volver a compilar el elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_PARAM_INPUT_INS					M4_CH_VM_ERROR_BASE + 0x0016
//Tipo incorrecto de argumento.\n \nArgumento "%3:s" para la regla llama a la funci�n CPP "%4:s" debe ser de entrada.\nDebe cambiar el tipo del argumento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_PARAM_NUM_INS_1					M4_CH_VM_ERROR_BASE + 0x0017
//N�mero de argumentos no v�lido.\n \nLa funci�n "%3:s" usa argumentos variables y la definici�n del elemento espera un n�mero fijo.\nDebe cambiar la definici�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_RULE_TYPE						M4_CH_VM_ERROR_BASE + 0x0018
//Tipo de regla no valido.\n \nEl tipo de regla "%3:s" no es correcto.\nDebe cambiar el tipo de la regla.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_PARAM_NUM_INS_2					M4_CH_VM_ERROR_BASE + 0x0019
//N�mero de argumentos no v�lido.\n \nLa definici�n del elemento indica un n�mero de argumento variable; sin embargo, la funci�n "%3:s". espera un n�mero fijo.\nDebe cambiar la definici�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_PARAM_NUM_INS_3					M4_CH_VM_ERROR_BASE + 0x001A
//N�mero de argumentos no v�lido.\n \nLa definici�n del elemento indica un n�mero de argumento menor al de la funci�n CPP "%3:s" (con un n�mero variable de argumentos). Son necesarios al menos "%4:s".\nDebe cambiar la definici�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_PARAM_NUM_INS_4					M4_CH_VM_ERROR_BASE + 0x001B
//N�mero de argumentos no v�lido.\n \nLa definici�n del elemento indica un n�mero de argumento distinto al de la funci�n "%3:s". Necesita "%4:s".\nDebe cambiar la definici�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_NOT_MAPPEABLE_FUNCTION					M4_CH_VM_ERROR_BASE + 0x001c
//Funci�n incorrecta.\n \nLa funci�n "%3:s" no se puede mapear a un elemento.\nDebe cambiar la definici�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_PARAM_REF_INS					M4_CH_VM_ERROR_BASE + 0x001d
//Argumento incorrecto.\n \nArgumento "%3:s" para la regla que llama la funci�n CPP "%4:s" debe ser de tipo entrada.\nDebe cambiar la definici�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_NODE_POINTER_ITEM_INS			M4_CH_VM_ERROR_BASE + 0x001e
//Error interno de instalaci�n.\n \nEl apuntador del nodo que se est� usando para buscar "%3:s" de "%4:s" no es correcto.\nDebe consultar al administrador de su sistema.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_NODE_POINTER_NODE_INS_UNUSED		M4_CH_VM_ERROR_BASE + 0x001f //no se usa
  /*//Nodo no definido.\n \Nodo no definido "%3:s".\nDebe definirlo.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".*/
#define M4_CH_VM_WRONG_ITEM_POINTER_RULE_INS			M4_CH_VM_ERROR_BASE + 0x0020
//Error interno de instalaci�n.\n \nEl apuntador de elemento para buscar la regla "%3:s" no es correcto.\nDebe consultar con el administrador.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_T3_NODE_INSTANCE					M4_CH_VM_ERROR_BASE + 0x0021
//Instancia no definida.\n \nNo existe la instancia del alias de Meta4Object "%3:s".\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_NODE_POINTER_CONN				M4_CH_VM_ERROR_BASE + 0x0022
//Nodo inexistente.\n \nNo existe el nodo conectado.\nDebe cambiar el conector.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".



//----------
//Ejecucion
//----------
#define M4_CH_VM_ADD_TO_POOL							M4_CH_VM_ERROR_BASE + 0x0100
//Memoria insuficiente.\n \nNo hay suficiente memoria libre.\nDebe cerrar una o varias aplicaciones.
#define M4_CH_VM_ADDING_RAM_FILTER    					M4_CH_VM_ERROR_BASE + 0x0101
//Error en filtro RAM.\n \nError al agregar el filtro RAM "%0:s".\nDebe revisar el filtro.\nEsto sucedi� al ejecutar la funci�n "%1:s".
#define M4_CH_VM_AUX_ITEM_TYPE_MISMATCH	                M4_CH_VM_ERROR_BASE + 0x0102
//Tipo incorrecto.\n \nTipo incorrecto en "%0:s" de "%1:s" de "%2:s" que es el elemento auxiliar de "%3:s", con el sentido de "%4:s".\nDebe ser "%5:s" en lugar de "%6:s".
#define M4_CH_VM_AV_APPLY_TABLE_UNUSED					M4_CH_VM_ERROR_BASE + 0x0103 /*LIBRE-Ya no se usa*/
  /*Error applying table*/
#define M4_CH_VM_AV_PK_MAPPING  						M4_CH_VM_ERROR_BASE + 0x0104
//Mapeo incorrecto de claves primarias.\n \nError en el mapeo de claves primarias en una llamada al Aplicavalor. No existe "%0:s" de "%1:s" de "%2:s".\nDebe cambiar el mapeo de claves primarias.
#define M4_CH_VM_AV_PK_PAIR_NUMBER 						M4_CH_VM_ERROR_BASE + 0x0105
//N�mero incorrecto de pares de claves primarias.\n \nEl n�mero de pares de claves primarias en el valor aplicado es incorrecto. Los pares pasados son "%0:s".\nDeben ser "%1:s".
#define M4_CH_VM_AV_PREP_NODE  							M4_CH_VM_ERROR_BASE + 0x0106//solo debug
//Nodo de Aplicavalor incorrecto.\n \nError al preparar el nodo en Aplicavalor.\nEsto sucedi� al ejecutar la funci�n "%0:s".
#define M4_CH_VM_AV_PREP_NODE_ITEM 						M4_CH_VM_ERROR_BASE + 0x0107
//Elemento de nodo de Aplicavalor inexistente.\n \nError al leer el nodo "%0:s" de "%1:s" porque no existe el elemento "%2:s".\nDebe incluir el elemento.
#define M4_CH_VM_AV_WRONG_HIST_NODE						M4_CH_VM_ERROR_BASE + 0x0108
//Nodo de hist�rico inexistente.\n \nNo existe el nodo de hist�rico "%0:s". S�lo se utilizar� la tabla de valores.\nDebe incluir el nodo de hist�rico.
#define M4_CH_VM_AV_WRONG_ORGANIGRAM_NODE				M4_CH_VM_ERROR_BASE + 0x0109
//Nodo de organigrama inexistente.\n \nNo existe el nodo de organigrama "%0:s". Aplicando sin nodo de organigrama.\nDebe incluir el nodo de organigrama.
#define M4_CH_VM_AV_WRONG_VALUES_NODE					M4_CH_VM_ERROR_BASE + 0x010A
//Nodo de valores inexistente.\n \nNo se puede realizar el Aplicavalor porque no existe el nodo de valores "%0:s".\nDebe incluir el nodo.
#define M4_CH_VM_BAD_CURRENT_ITEM_TYPE_COMPILING        M4_CH_VM_ERROR_BASE + 0x010B //OJO, ES DE FINDER
//Error de compilaci�n.\n \nNo se puede compilar "%0:s" porque no es ni m�todo ni concepto.
#define M4_CH_VM_BAD_ITEM_INDEX							M4_CH_VM_ERROR_BASE + 0x010C
//Elemento de �ndice incorrecto.\n \nEl elemento en la posici�n "%0:s" del �ndice "%1:s" no es v�lido.\nDebe cambiar el �ndice.\nEsto sucedi� al ejecutar la funci�n BSearch.
#define M4_CH_VM_BAD_NUMBER_OF_ARGUMENTS				M4_CH_VM_ERROR_BASE + 0x010D
//N�mero de argumentos no v�lido.\n \nN�mero de argumentos no v�lido en la funci�n "%0:s".\nDebe cambiar la llamada a la funci�n.
#define M4_CH_VM_CANT_ADD_RECORD						M4_CH_VM_ERROR_BASE + 0x010E
//No se puede a�adir registro.\n \nDebe revisar sus permisos para a�adir registros.\nEsto sucedi� en "%0:s" de "%1:s" al ejecutar la funci�n "%2:s".
#define M4_CH_VM_CANT_EXECUTE_ITEM						M4_CH_VM_ERROR_BASE + 0x010F
//No se puede ejecutar un elemento.\n \nNo se puede ejecutar "%0:s" de "%1:s" de "%2:s".\nDebe revisar sus permisos.
#define M4_CH_VM_CANT_FIND_METHOD						M4_CH_VM_ERROR_BASE + 0x0110
//M�todo externo no encontrado.\n \nNo se puede encontrar el m�todo "%0:s" en librer�a externa "%1:s".\nDebe asegurarse de que el m�todo exista y sea accesible.
#define M4_CH_VM_CANT_LOAD_OBJECT						M4_CH_VM_ERROR_BASE + 0x0111
//Objeto externo no encontrado.\n \nNo se puede cargar objeto externo "%0:s".\nDebe asegurarse de que exista y sea accesible.
#define M4_CH_VM_NOT_USED_1								M4_CH_VM_ERROR_BASE + 0x0112
// No se usa.
#define M4_CH_VM_CANT_RESIZE_STACK						M4_CH_VM_ERROR_BASE + 0x0113
//Ejecuci�n demasiado compleja.\n \nNo se puede seguir ejecutando debido a la excesiva complejidad del c�digo, a demasiadas ejecuciones propagadas o a una recursividad infinita.\nDebe reducir la complejidad de su c�digo.
#define M4_CH_VM_CHANNEL_ALIAS_NOT_FOUND_COMPILING	    M4_CH_VM_ERROR_BASE + 0x0114
//Alias no encontrado.\n \nNo se encuentra el alias del Meta4Object "%0:s".\nDebe definir el alias.
#define M4_CH_VM_CHANNEL_NAME_NOT_ALLOWED				M4_CH_VM_ERROR_BASE + 0x0115
//Par�metro no permitido.\n \nEl par�metro Meta4Object no est� permitido en la funci�n "%0:s". Esta funci�n s�lo trabaja sobre un �nico Meta4Object.\nDebe quitar el argumento Meta4Object.
#define M4_CH_VM_CHANNEL_NOT_FOUND_COMPILING_UNUSED		M4_CH_VM_ERROR_BASE + 0x0116 //No se usa
  /*Can't find Meta4Object "%0:s" compiling.*/
#define M4_CH_VM_COMPILING_JIT							M4_CH_VM_ERROR_BASE + 0x0117
//Error al compilar c�digo JIT.\n \nDebe revisar su c�digo JIT.
#define M4_CH_VM_COPING_ITEM_UNUSED						M4_CH_VM_ERROR_BASE + 0x0118//No se usa
  /*Error al copiar el elemento "%0:s" en el nodo "%1:s" del Meta4Object "%2:s"*/
#define M4_CH_VM_COPING_REGISTER_UNUSED					M4_CH_VM_ERROR_BASE + 0x0119/*LIBRE Ya no se usa*/
 /*Error coping row.*/
#define M4_CH_VM_CUR_EXCHANGE							M4_CH_VM_ERROR_BASE + 0x011A
//No se puede hacer el cambio de moneda.\n \nNo se puede hacer el cambio de la moneda de origen "%0:s" a la moneda de destino "%1:s" porque no se encontraron los valores de cambio.\nDebe consultar los valores de cambio.
#define M4_CH_VM_CUR_EXCHANGE_RECURSIVE_CYCLE			M4_CH_VM_ERROR_BASE + 0x011B
//No se puede hacer el cambio de moneda.\n \nNo se puede hacer el cambio de la moneda de origen "%0:s" a la moneda de destino "%1:s" a trav�s de la moneda intermedia "%2:s" porque hay un ciclo en los valores de cambio.\nDebe consultar los valores de cambio.
#define M4_CH_VM_CUR_NO_EXCHANGE_RATE					M4_CH_VM_ERROR_BASE + 0x011C
//No existe conversi�n de moneda.\n \nNo se puede hacer el cambio de la moneda de origen "%0:s" a la moneda de destino "%1:s".\nDebe consultar los valores de cambio.
#define M4_CH_VM_CUR_NO_INTERM_EXCHANGE_RATE			M4_CH_VM_ERROR_BASE + 0x011D
//No existe conversi�n de moneda.\n \nNo se puede hacer el cambio de la moneda de origen "%0:s" a la moneda de destino "%1:s" a trav�s de la moneda intermedia "%2:s".\nDebe consultar los valores de cambio.
#define M4_CH_VM_CUR_SRC_CUR_NOT_FOUND					M4_CH_VM_ERROR_BASE + 0x011E
//Moneda no definida.\n \nDebe consultar los valores de cambio.
#define M4_CH_VM_DEFINE_INSTANCE						M4_CH_VM_ERROR_BASE + 0x011F
//No se puede agregar la instancia.\n \nNo se puede agregar la instancia "%0:s" del Meta4Object "%1:s".\nDebe revisar si ya est� definida.
#define M4_CH_VM_DEL_NULL_METH							M4_CH_VM_ERROR_BASE + 0x0120
//Ejecuci�n nula.\n \nSe realiz� una ejecuci�n nula para el "%0:s" de "%1:s" de "%2:s".\nDebe definir una regla para el elemento en la fecha de ejecuci�n.
#define M4_CH_VM_DIALOG_BOX								M4_CH_VM_ERROR_BASE + 0x0121
//Cuadro de dialogo en servidor.\n \nSe ha encontrado un cuadro de dialogo en el servidor con el mensaje "%0:s", t�tulo "%1:s". Asumiendo que el bot�n OK ha sido pulsado.\nDebe quitar los cuadros de dialogo en los m�todos que se ejecutan en el servidor.
#define M4_CH_VM_DIV_BY_ZERO							M4_CH_VM_ERROR_BASE + 0x0122
//Divisi�n entre cero.\n \nError matem�tico. Devuelve 0.\n\nEsto sucedi� en la l�nea "%0:s" del c�digo de "%1:s" de "%2:s" de "%3:s".
#define M4_CH_VM_ERROR_ADDING_SLICE						M4_CH_VM_ERROR_BASE + 0x0123
//No se puede a�adir tramo.\n \nNo se puede agregar un tramo para "%0:s" de "%1:s" de "%2:s". Tramo "%3:s". Debe revisar sus permisos y las fechas entre las que quiere a�adir el tramo.\nEsto sucedi� al ejecutar la funci�n: "%4:s".
#define M4_CH_VM_ERROR_ADDING_SLICE_TO_ASSIGN			M4_CH_VM_ERROR_BASE + 0x0124
//No se puede a�adir tramo.\n \nNo se puede crear un tramo en "%0:s" de "%1:s" de "%2:s", el tramo "%3:s" para asignar un valor.\nDebe revisar sus permisos y las fechas entre las que quiere a�adir el tramo.
#define M4_CH_VM_ERROR_CHANGING_PRIORITY				M4_CH_VM_ERROR_BASE + 0x0125
//Error cambiando la prioridad.\n \nNo se puede cambiar la prioridad de "%0:s" de "%1:s" de "%2:s".\nDebe revisar sus permisos y su situaci�n.\nEsto sucedi� al ejecutar la funci�n "%3:s".
#define M4_CH_VM_ERROR_COMPACTING_ITEM					M4_CH_VM_ERROR_BASE + 0x0126
//Error al compactar.\n \nNo se puede compactar el elemento "%0:s".\nDebe revisar su situaci�n.\nEsto sucedi� al ejecutar la funci�n: "%1:s".
#define M4_CH_VM_ERROR_COMPILING            			M4_CH_VM_ERROR_BASE + 0x0127
//Error de compilaci�n.\n \nDebe consultar errores anteriores.
#define M4_CH_VM_ERROR_EXT_LOAD             			M4_CH_VM_ERROR_BASE + 0x0128
//Error al realizar carga extendida.\n \nDebe consultar errores anteriores.
#define M4_CH_VM_ERROR_IN_LOCAL_TEST					M4_CH_VM_ERROR_BASE + 0x0129
//Error en prueba local.\n \nDebe asegurarse de que el c�digo no contenga ninguna funci�n no ejecutable en prueba local.
#define M4_CH_VM_ERROR_LOAD								M4_CH_VM_ERROR_BASE + 0x012A/*solo debug*/
//No se puede cargar el Meta4Object.\n \nError al cargar el Meta4Object "%0:s".\nDebe revisar errores anteriores.
#define M4_CH_VM_ERROR_PERSIST_CHANNEL					M4_CH_VM_ERROR_BASE + 0x012B/*solo debug*/
//No se puede guardar el Meta4Object.\n \nError al guardar el Meta4Object "%0:s".\nDebe revisar errores anteriores.
#define M4_CH_VM_ERROR_READING_ITEM						M4_CH_VM_ERROR_BASE + 0x012C
//No se puede leer elemento.\n \nError al leer "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de estar situado.\nEsto sucedi� al ejecutar la funci�n "%3:s".
#define M4_CH_VM_ERROR_RELEASE_CHANNEL					M4_CH_VM_ERROR_BASE + 0x012D
//Error al liberar el Meta4Object.\n \nError al liberar el Meta4Object "%0:s".\nDebe consultar con el administrador.
#define M4_CH_VM_ERROR_SET_VALUE_COMPILING            	M4_CH_VM_ERROR_BASE + 0x012E
//Error de compilaci�n.\n \nNo se puede asignar el valor "%0:s" en el elemento "%1:s".\nDebe revisar sus permisos sobre el Meta4Object de compilaci�n.
#define M4_CH_VM_ERROR_SETTING_ARGS						M4_CH_VM_ERROR_BASE + 0x012F
//Argumentos incorrectos.\n \nError al definir argumentos para compilar este elemento.\nDebe revisar que ning�n argumento est� borrado.
#define M4_CH_VM_ERROR_SPLITING_SLICE					M4_CH_VM_ERROR_BASE + 0x0130
//Error partiendo tramo.\n \nError al dividir un tramo de "%0:s" de "%1:s" de "%2:s". No existe un tramo para la fecha "%3:s".\nDebe asegurarse que de exista un tramo en esa fecha.\nEsto sucedi� al ejecutar un atributo de tipo m�todo.
#define M4_CH_VM_ERROR_SPLITTING_SLICE_UNUSED			M4_CH_VM_ERROR_BASE + 0x0131 //no se usa
  /*Error splitting slice for elemento "%0:s", slice "%1:s". Function: "%2:s".*/
#define M4_CH_VM_ERROR_UNLOAD_CHANNEL					M4_CH_VM_ERROR_BASE + 0x0132
//Error al descargar Meta4Object.\n \nError al descargar el Meta4Object "%0:s".\nDebe consultar con el administrador de su sistema.
#define M4_CH_VM_ERROR_WRITING_END_DT_ITEM_UNUSED		M4_CH_VM_ERROR_BASE + 0x0133/*LIBRE-YA NO SE USA*/
	/*//Error writing end date elemento.*/
#define M4_CH_VM_ERROR_WRITING_ITEM						M4_CH_VM_ERROR_BASE + 0x0134
//Error al asignar elemento.\n \nError al asignar "%0:s" de "%1:s" de "%2:s".\nDebe revisar sus permisos y asegurarse de estar bien situado.\nEsto sucedi� al ejecutar la funci�n "%3:s".
#define M4_CH_VM_ERROR_WRITING_START_DT_ITEM_UNUSED		M4_CH_VM_ERROR_BASE + 0x0135/*LIBRE-Ya no se usa*/
	/*//Error writing start date elemento.*/
#define M4_CH_VM_EXECUTING_ATT_UNUSED					M4_CH_VM_ERROR_BASE + 0x0136//no se usa
   /*//Ejecuci�n de atributo incorrecta.\n \nError al ejecutar un atributo de tipo m�todo.\nDebe revisar errores anteriores.\nEsto sucedi� al ejecutar "%0:s" de "%1:s" de "%2:s".*/
#define M4_CH_VM_EXECUTING_JIT_UNUSED					M4_CH_VM_ERROR_BASE + 0x0137//ya no se usa
  /*//No se puede crear ejecuci�n JIT.\n \nError al crear una ejecuci�n JIT en la funci�n "%0:s"*/
#define M4_CH_VM_FORMATTING_ERROR_STRING				M4_CH_VM_ERROR_BASE + 0x0138
//Error al dar formato al mensaje.\n \nNo se puede dar formato a la cadena de error. Se mostrar� el mensaje sin dar formato.\nDebe revisar el formato de su mensaje de error.
#define M4_CH_VM_FUNCTION_NO_LONGER_AVAILABLE			M4_CH_VM_ERROR_BASE + 0x0139
//Funci�n no disponible.\n \nLa funci�n "%0:s" no est� ya disponible.\nDebe usar "%1:s" en su lugar.
#define M4_CH_VM_GETTING_DATE_FROM_STACK_UNUSED			M4_CH_VM_ERROR_BASE + 0x013A/*LIBRE-Ya no se usa*/
  /*//Error al leer fecha de la pila.*/
#define M4_CH_VM_INSTALLING_JIT							M4_CH_VM_ERROR_BASE + 0x013B
//No se puede preparar c�digo JIT.\n \nDebe revisar el c�digo a preparar.
#define M4_CH_VM_INTERNAL_ERROR							M4_CH_VM_ERROR_BASE + 0x013C
//Error interno\n \nArchivo "%0:s" l�nea:"%1:s" "%2:s".\nDebe consultar con el administrador.
#define M4_CH_VM_INVALID_ACCESS_COMPILING	            M4_CH_VM_ERROR_BASE + 0x013D
//Error interno de compilaci�n.\n \nEstado de Meta4Object no v�lido.\nDebe consultar con el administrador.\nEsto sucedi� al compilar.
#define M4_CH_VM_INVALID_CALL_UNUSED					M4_CH_VM_ERROR_BASE + 0x013E/*LIBRE. Ya no se usa*/
  /*//Invalid call.*/
#define M4_CH_VM_INVALID_TYPE_UNUSED					M4_CH_VM_ERROR_BASE + 0x013F/*LIBRE-Ya no se usa*/
  /*//Invalid Type for the elemento "%0:s".*/
#define M4_CH_VM_ITEM_IS_NULL							M4_CH_VM_ERROR_BASE + 0x0140
//Elemento nulo no es v�lido.\n \n"%0:s" de "%1:s" de "%2:s" es nulo.\nDebe asignar un valor al elemento.\nEsto sucedi� al ejecutar la funci�n "%3:s".
#define M4_CH_VM_ITEM_NOT_FOUND							M4_CH_VM_ERROR_BASE + 0x0141
//Elemento inexistente.\n \n"%0:s" de "%1:s" de "%2:s" no existe.\nDebe definir el elemento.\nEsto sucedi� al ejecutar la funci�n "%3:s".
#define M4_CH_VM_ITEM_NOT_FOUND_BY_SCOPE				M4_CH_VM_ERROR_BASE + 0x0142
//Elemento inexistente.\n \nEl elemento con �mbito "%1:s" y en la posici�n "%2:s" no existe.\nDebe revisar su c�digo.\nEsto sucedi� al ejecutar la funci�n "%0:s".
#define M4_CH_VM_ITEM_CH_NOT_FOUND_COMPILING			M4_CH_VM_ERROR_BASE + 0x0143
//Elemento inexistente.\n \n"%0:s" de "%1:s" de "%2:s" no existe.\nDebe asegurarse de la existencia de este elemento en el Meta4Object de compilaci�n.
#define M4_CH_VM_ITEM_TYPE_MISMATCH						M4_CH_VM_ERROR_BASE + 0x0144
//Tipo no v�lido.\n \nEl tipo de "%0:s" de "%1:s" de "%2:s" no es v�lido.\nDebe ser "%4:s" en lugar de "%5:s".\nEsto sucedi� al ejecutar la funci�n "%3:s".
#define M4_CH_VM_ITEM_TYPE_MISMATCH_COMP				M4_CH_VM_ERROR_BASE + 0x0145
//Tipo no v�lido.\n \nEl tipo de "%0:s" de "%1:s" de "%2:s" no es v�lido.\nDebe revisar la integridad de su Meta4Object de compilaci�n.
#define M4_CH_VM_JIT_HANDLE_NOT_FOUND					M4_CH_VM_ERROR_BASE + 0x0146
//Identificador de c�digo JIT no encontrado.\n \nDebe preparar el c�digo antes de ejecutarlo.
#define M4_CH_VM_MESSAGE_BOX 							M4_CH_VM_ERROR_BASE + 0x0147
//Cuadro de di�logo en servidor.\n \nSe ha encontrado un cuadro de di�logo en el servidor con el mensaje "%0:s", t�tulo "%1:s".\nDebe quitar los cuadros de dialogo de los m�todos que se ejecutan en servidor.
#define M4_CH_VM_NO_BLOCKS_TO_PROPAGATE					M4_CH_VM_ERROR_BASE + 0x0148
//No se encuentra el bloque.\n \nNo hay bloques en el nodo "%0:s" de "%1:s" para realizar una ejecuci�n propagada.\nDebe situarse antes de ejecutar un elemento de �mbito de bloque.
#define M4_CH_VM_NO_CONNECT								M4_CH_VM_ERROR_BASE + 0x0149
//No hay transacci�n de base de datos.\n \nNo se puede cerrar la transacci�n de base de datos porque no est� abierta.\nDebe asegurarse de tener una transacci�n abierta.\nEsto sucedi� al intentar ejecutar "%0:s".
#define M4_CH_VM_NO_DEFAULT_INDEX   					M4_CH_VM_ERROR_BASE + 0x014A
//No existe un �ndice por defecto.\n \nNo hay ning�n �ndice por defecto disponible para "%0:s" de"%1:s".\nDebe especificar un �ndice.\nEsto sucedi� en "%2:s".
#define M4_CH_VM_NO_END_DT_ITEM							M4_CH_VM_ERROR_BASE + 0x014B
//Elemento de fecha fin requerido.\n \n"%0:s" de "%1:s" no tiene ning�n elemento con tipo interno de fecha final.\nDebe definir un elemento con tipo interno de fecha fin.\nEsto sucedi� al ejecutar "%2:s".
#define M4_CH_VM_NO_EXECUTE_PERM						M4_CH_VM_ERROR_BASE + 0x014C
//No tiene los permisos necesarios.\n \nNo puede ejecutar "%0:s" porque no tiene permisos de ejecuci�n sobre �l.\nDebe verificar su perfil.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_VM_NO_EXECUTE_PERM_CONN					M4_CH_VM_ERROR_BASE + 0x014D
//No tiene los permisos necesarios.\n \nNo puede ejecutar "%0:s" porque no tiene permisos de ejecuci�n sobre �l.\nDebe verificar su perfil.\nEsto sucedi� en "%1:s" de "%2:s" conectado al elemento padre "%3:s" de "%4:s".
#define M4_CH_VM_NO_EXECUTE_PERM_DEBUG					M4_CH_VM_ERROR_BASE + 0x014E
//Ejecuci�n no disponible.\n \nNo se puede ejecutar en cliente mientras se depura en servidor.
#define M4_CH_VM_NO_FREE_CONNECTION						M4_CH_VM_ERROR_BASE + 0x014F
//No hay ninguna conexi�n libre.\n \nNo se ha podido obtener una conexi�n con la base de datos.\nDebe revisar la configuraci�n del n�mero m�ximo de conexiones o consultar al administrador.\nEsto sucedi� intentando ejecutar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_NO_GET_ACCESS_L2						M4_CH_VM_ERROR_BASE + 0x0150
//Instancia no encontrada.\n \nNo se encuentra la instancia "%0:s" en una llamada de nivel 2.\nDebe definir la instancia.\nEsto sucedi� al ejecutar la funci�n "%1:s".
#define M4_CH_VM_NO_ITEM_NAME_COMPILING            		M4_CH_VM_ERROR_BASE + 0x0151
//ID de elemento no encontrado.\n \nEl ID del elemento a compilar no es v�lido, por lo que no se puede compilar la regla actual.\nDebe poner un ID correcto.
#define M4_CH_VM_NO_RECORD_TO_COMPILE					M4_CH_VM_ERROR_BASE + 0x0152
//No hay fila actual.\n \nNo hay ninguna fila actual para compilar en "%0:s" de "%1:s".\nDebe situarse antes de compilar.
#define M4_CH_VM_NO_RECORDS_TO_PROPAGATE				M4_CH_VM_ERROR_BASE + 0x0153
//No hay registros.\n \nNo hay ninguna fila para propagar ejecuci�n en "%0:s" de "%1:s". Probablemente: el elemento padre es de �mbito de registro, el hijo de bloque y el nodo padre est� en una posici�n no v�lida.\nDebe posicionar el nodo del elemento padre.
#define M4_CH_VM_NO_REFERED_ARG_ALLOWED					M4_CH_VM_ERROR_BASE + 0x0154
//Argumentos por referencia no permitidos.\n \nNo se permiten argumentos de referencia en llamadas indirectas a "%0:s" de "%1:s" "%2:s".\nDebe llamarlo de forma directa.\nEsto sucedi� al ejecutar la funci�n "%3:s".
#define M4_CH_VM_NO_START_DT_ITEM						M4_CH_VM_ERROR_BASE + 0x0155
//Elemento de fecha inicio requerido.\n \n"%0:s" de "%1:s" no tiene ning�n elemento con tipo interno de fecha de inicio.\nDebe definir un elemento con tipo interno de fecha de inicio.\nEsto sucedi� al ejecutar "%2:s".
#define M4_CH_VM_NO_SUCH_AUX_ITEM						M4_CH_VM_ERROR_BASE + 0x0156
//Elemento auxiliar requerido.\n \n"%0:s" de "%1:s" de "%2:s" no tiene ning�n elemento auxiliar con la funci�n de "%3:s".\nDebe definir un elemento con ese significado.
#define M4_CH_VM_NO_SUCH_ERROR_IN_STACK					M4_CH_VM_ERROR_BASE + 0x0157
//Mensaje de error no encontrado.\n \n
#define M4_CH_VM_NO_SUCH_FILE 							M4_CH_VM_ERROR_BASE + 0x0158
//Archivo no encontrado.\n \nEl archivo "%0:s" no existe.\nDebe asegurarse de la existencia del archivo y sus permisos sobre �l.
#define M4_CH_VM_NO_SUCH_HANDLE_FILE					M4_CH_VM_ERROR_BASE + 0x0159
//Identificador de archivo inexistente.\n \nNo existe el indicador de archivo "%0:s".\nDebe asegurarse de la existencia de un archivo asociado a ese identificador.\nEsto sucedi� al ejecutar la funci�n "%1:s".
#define M4_CH_VM_NO_SUCH_INSTANCE						M4_CH_VM_ERROR_BASE + 0x015A
//Instancia inexistente.\n \nNo se encuentra la instancia "%0:s" definida para "%1:s".\nDebe definir la instancia.\nEsto sucedi� al ejecutar la funci�n "%2:s".
#define M4_CH_VM_NO_SUCH_ITEM							M4_CH_VM_ERROR_BASE + 0x015B
//Elemento inexistente.\n \n"%0:s" de "%1:s" de "%2:s" no existe.\nDebe definir el elemento o eliminar las referencias al elemento.
#define M4_CH_VM_NO_SUCH_L2_ITEM						M4_CH_VM_ERROR_BASE + 0x015C
//Elemento inexistente.\n \n"%0:s" de "%1:s" de "%2:s", al que se hace referencia a trav�s de una llamada de nivel 2, no existe.\nDebe definir el elemento o eliminar las referencias al elemento.
#define M4_CH_VM_NO_SUCH_L2_NODE						M4_CH_VM_ERROR_BASE + 0x015D
//Nodo inexistente.\n \n"%0:s" de "%1:s", al que se hace referencia a trav�s de una llamada de nivel 2, no existe.\nDebe eliminar las referencias a ese nodo.
#define M4_CH_VM_NO_SUCH_NODE							M4_CH_VM_ERROR_BASE + 0x015E
//Nodo inexistente.\n \n"%0:s" de "%1:s" no existe.\nDebe definir el nodo o eliminar las referencias a �l.\nEsto sucedi� en la funci�n "%2:s".
#define M4_CH_VM_NO_SUCH_NODE_COMP						M4_CH_VM_ERROR_BASE + 0x015F
//Meta4Object no v�lido.\n \nEl Meta4Object de compilaci�n "%1:s" no es v�lido porque no existe el nodo "%0:s".\nDebe consultar al administrador.
#define M4_CH_VM_NO_SUCH_NODE_STT						M4_CH_VM_ERROR_BASE + 0x0160
//Meta4Object no v�lido.\n \nEl Meta4Object de estad�sticas "%1:s" no es v�lido porque no existe el nodo "%0:s".\nDebe consultar al administrador.
#define M4_CH_VM_NO_TRIGGER_CONTEXT  					M4_CH_VM_ERROR_BASE + 0x0161
//Conector incorrecto.\n \nNo se puede obtener contexto conector.\nDebe revisar el conector.\nEsto sucedi� en el conector entre el elemento padre "%0:s" de "%1:s" de "%2:s" y el elemento hijo "%3:s" de "%4:s" "%5:s".
#define M4_CH_VM_NODE_NOT_FOUND_UNUSED					M4_CH_VM_ERROR_BASE + 0x0162/*LIBRE-No se usa*/
  /*//Can't find Node.*/
#define M4_CH_VM_NODE_NOT_FOUND_COMPILING				M4_CH_VM_ERROR_BASE + 0x0163
//Nodo no encontrado.\n \nNo se encuentra "%0:s" de "%1:s".\nDebe definir el nodo o eliminar las referencias a �l.\nEsto sucedi� al compilar.
#define M4_CH_VM_NULL_ACCESS							M4_CH_VM_ERROR_BASE + 0x0164 //solo debug
//Acceso nulo.\n \nAcceso nulo en "%0:s".\nDebe consultar al administrador.
#define M4_CH_VM_NULL_AUX_ITEM							M4_CH_VM_ERROR_BASE + 0x0165
//Elemento auxiliar nulo.\n \n"%0:s" de "%1:s" de "%2:s" que es el elemento auxiliar de "%3:s", con la funci�n "%4:s", no tiene valor.\nDebe asignar un valor al elemento.
#define M4_CH_VM_NULL_ITEM_UNUSED						M4_CH_VM_ERROR_BASE + 0x0166/*LIBRE-No se usa*/
  /*//elemento "%0:s"of node "%1:s" in Meta4Object "%2:s" is NULL.*/
#define M4_CH_VM_NULL_ITEM_VAL_COMP						M4_CH_VM_ERROR_BASE + 0x0167
//Valor nulo no v�lido.\n \n"%0:s" no puede ser nulo en "%1:s" de "%2:s" de "%3:s".\nDebe asignarle un valor.\nEsto sucedi� durante la compilaci�n.
#define M4_CH_VM_NULL_ITEM_VAL_COMP_ALIAS				M4_CH_VM_ERROR_BASE + 0x0168
//Valor nulo no v�lido.\n \n"%0:s" no puede ser nulo, con par�metros "%1:s".\nDebe asignarle un valor.\nEsto sucedi� durante la b�squeda de alias.
#define M4_CH_VM_NULL_ITEM_VAL_COMP_JIT					M4_CH_VM_ERROR_BASE + 0x0169
//Valor nulo no v�lido.\n \n"%0:s" no puede ser nulo en "%1:s" de "%2:s" de "%3:s".\nDebe asignarle un valor.\nEsto sucedi� durante la compilaci�n de c�digo JIT.
#define M4_CH_VM_NULL_NODE_HANDLE						M4_CH_VM_ERROR_BASE + 0x016A//solo debug
//Identificador de nodo nulo.\n \nSe ha encontrado un identificador de nodo nulo.\nDebe consultar al administrador.\nEsto sucedi� al ejecutar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_OUT_OF_MEMORY							M4_CH_VM_ERROR_BASE + 0x016B//solo debug
//Memoria insuficiente.\n \nNo hay suficiente memoria libre.\nDebe cerrar una o varias aplicaciones.\n"%0:s", archivo:"%1:s", l�nea:"%2:s".
#define M4_CH_VM_PARAMETER_TYPE_MISMATCH	            M4_CH_VM_ERROR_BASE + 0x016C
//Tipo de par�metro no v�lido.\n \nEl par�metro "%0:s" es de un tipo no v�lido.\nDebe ser "%2:s" en lugar de "%3:s".\nEsto sucedi� en la funci�n "%1:s".
#define M4_CH_VM_PREPARING_TO_COMPILE					M4_CH_VM_ERROR_BASE + 0x016D
//Meta4Object no v�lido.\n \nEl nodo "%0:s" no es v�lido en el Meta4Object de compilaci�n.\nDebe consultar al administrador.
#define M4_CH_VM_READING_ATT							M4_CH_VM_ERROR_BASE + 0x016E
//Error al leer atributo.\n \nError al leer un atributo de "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de estar bien situado.\nEsto sucedi� durante la ejecuci�n de la funci�n "%3:s".
#define M4_CH_VM_READING_SLICE_UNUSED					M4_CH_VM_ERROR_BASE + 0x016F /*LIBRE-No se usa*/
  /*//Error reading slice of elemento "%0:s" from "%1:s" to "%2:s" in function "%3:s".*/
#define M4_CH_VM_REDEFINING_INSTANCE					M4_CH_VM_ERROR_BASE + 0x0170
//Redefinici�n de instancia.\n \nRedefinici�n de la instancia del Meta4Object para c�digo JIT. Cerrando el Meta4Object "%0:s".
#define M4_CH_VM_RETURN_TYPE_MISMATCH					M4_CH_VM_ERROR_BASE + 0x0171
//Tipo de retorno no v�lido.\n \n"%0:s" de "%1:s" de "%2:s" tiene un tipo de devoluci�n no v�lido.\nDebe ser "%3:s". en lugar de "%4:s".
#define M4_CH_VM_RTS_POP								M4_CH_VM_ERROR_BASE + 0x0172
//Error interno de ejecuci�n.\n \nError al ejecutar extracci�n de RTS. "%0:s".\nDebe consultar al administrador.
#define M4_CH_VM_RTS_PUSH								M4_CH_VM_ERROR_BASE + 0x0173
//Error interno de ejecuci�n.\n \nError al ejecutar inclusi�n en RTS. "%0:s".\nDebe consultar al administrador.
#define M4_CH_VM_SET_CONCEPT_SLICE_VALUE    			M4_CH_VM_ERROR_BASE + 0x0174
//Asignaci�n de tramo incorrecta.\n \nError al asignar un valor en un tramo de "%0:s" de "%1:s" de "%2:s". Tramo de "%3:f" a "%4:f".\nDebe asegurarse de estar situado correctamente.\nEsto sucedi� durante la ejecuci�n de "%5:s" de "%6:s" de "%7:s".
#define M4_CH_VM_SET_CONCEPT_VALUE						M4_CH_VM_ERROR_BASE + 0x0175 
//Asignaci�n de concepto incorrecta.\n \nError al establecer el valor de "%0:s" de "%1:s" de "%2:s" con el valor de retorno de la ejecuci�n.\nDebe asegurarse de estar correctamente situado.
#define M4_CH_VM_STRING_TOO_LONG						M4_CH_VM_ERROR_BASE + 0x0176
//Cadena demasiado larga.\n \nDebe reducir la cadena hasta "%1:s" caracteres como m�ximo.\nEsto sucedi� en la funci�n "%0:s".
#define M4_CH_VM_STT_INIT								M4_CH_VM_ERROR_BASE + 0x0177
//Error al iniciar estad�sticas.\n \nNo se puede inicializar estad�sticas sobre el Meta4Object "%0:s".\nDebe consultar al administrador.
#define M4_CH_VM_STT_NO_STATE_STARTED					M4_CH_VM_ERROR_BASE + 0x0178
//Estad�sticas no iniciadas.\n \nNo hay estado de estad�sticas para la operaci�n "%0:s".\nDebe consultar al administrador.
#define M4_CH_VM_SYNTAX_ERROR       					M4_CH_VM_ERROR_BASE + 0x0179
//Error de sintaxis.\n \nDebe corregir su c�digo.\nEsto sucedi� en la l�nea "%0:s" del c�digo de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_TI_NOT_FOUND_COMPILING					M4_CH_VM_ERROR_BASE + 0x017A
//Estructura de nodo inexistente.\n \nNo se encuentra la estructura de nodo "%0:s" en compilaci�n\nDebe eliminar las referencias a la estructura de nodo o definirla.
#define M4_CH_VM_TOO_MUCH_PARAM_NUM           			M4_CH_VM_ERROR_BASE + 0x017B
//Demasiados argumentos.\n \nDemasiados argumentos en la funci�n "%0:s".\nDebe llamar a la funci�n con el n�mero correcto de argumentos.
#define M4_CH_VM_TOO_PARAM             					M4_CH_VM_ERROR_BASE + 0x017C
//Demasiados argumentos.\n \nDemasiados argumentos en la funci�n "%0:s".\nDebe pasar como m�ximo "%1:s" en lugar de "%2:s".
#define M4_CH_VM_TOT_ITEM  								M4_CH_VM_ERROR_BASE + 0x017D
//Error al leer totalizador.\n \nError al leer el totalizador de "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de estar correctamente situado y revisar su perfil.
#define M4_CH_VM_TRACE_STACK_VM							M4_CH_VM_ERROR_BASE + 0x017E
//Traza de pila.\n \nVM"%0:s": argumentos err�neos al ejecutar la funci�n: "%1:s".\nDebe consultar al administrador.
#define M4_CH_VM_TRACE_STACK_VM_GENERIC					M4_CH_VM_ERROR_BASE + 0x017F
//Traza de pila.\n \nVM: "%0:s" argumentos err�neos al ejecutar la funci�n:"%1:s" - "%2:s".\nDebe consultar al administrador.
#define M4_CH_VM_TRACE_VM_L0          					M4_CH_VM_ERROR_BASE + 0x0180
//Traza ejecuci�n.\n \nL0 "%0:s".
#define M4_CH_VM_TRACE_VM_L1          					M4_CH_VM_ERROR_BASE + 0x0181
//Traza ejecuci�n\n \nL1 "%0:s".
#define M4_CH_VM_TRACE_VM_L2          					M4_CH_VM_ERROR_BASE + 0x0182
//Traza ejecuci�n\n \nL2 "%0:s".
#define M4_CH_VM_TRACE_VM_LBD          					M4_CH_VM_ERROR_BASE + 0x0183
//Traza ejecuci�n.\n \nLDB funci�n "%0:s".
#define M4_CH_VM_TRACE_VM_LEVEL_JUMPS					M4_CH_VM_ERROR_BASE + 0x0184
//Traza de ejecuci�n.\n \nNiveles superiores. Saltos entre niveles.
#define M4_CH_VM_TRACE_VM_PROPAGATION          			M4_CH_VM_ERROR_BASE + 0x0185
//Traza de ejecuci�n.\n \nPropagaci�n "%0:s".
#define M4_CH_VM_SLICES_NON_LINEAR         				M4_CH_VM_ERROR_BASE + 0x0186
//Comportamiento no lineal en tramos.\n \nSe est� creando un tramo en "%0:s" de "%1:s" de "%2:s", pero la forma de partir los tramos es no lineal.\nDebe asegurarse de que realmente no deba ser lineal.
#define M4_CH_VM_UNDEFINED_ITEM							M4_CH_VM_ERROR_BASE + 0x0187
//Elemento no definido.\n \n"%0:s" de "%1:s" no est� definido.\nDebe definir el elemento.\nEsto sucedi� al ejecutar la funci�n "%2:s".
#define M4_CH_VM_UNSORTED_FUNCTION_TABLE				M4_CH_VM_ERROR_BASE + 0x0188
//Tabla de funciones desordenada.\n \nDebe consultar al administrador.
#define M4_CH_VM_WRITING_SLICE							M4_CH_VM_ERROR_BASE + 0x0189
//Escritura de tramo incorrecta.\n \nError al asignar un tramo de "%0:s" de "%1:s" de "%2:s" . Tramo de "%3:f" a "%4:f".\nDebe asegurarse de estar correctamente situado.\nEsto sucedi� durante la ejecuci�n de la funci�n "%5:s".
#define M4_CH_VM_WRONG_ARGUMENT_COMP					M4_CH_VM_ERROR_BASE + 0x018A
//Argumento nulo no v�lido.\n \nEl argumento "%0:s" no puede ser nulo en "%1:s". No se puede compilar este elemento.\nDebe asignar un valor al argumento.
#define M4_CH_VM_WRONG_ATT_PARAM_NUM           			M4_CH_VM_ERROR_BASE + 0x018B
//N�mero de par�metros no v�lido.\n \nN�mero de par�metros no v�lido para el atributo "%3:s" de "%0:s" de "%1:s" de "%2:s".\nDebe usar "%4:s" par�metros.
#define M4_CH_VM_WRONG_BAD_PREINSTALL					M4_CH_VM_ERROR_BASE + 0x018C //debug only
//Error de instalaci�n.\n \n"%0:s". Error al preinstalar "%1:s".
#define M4_CH_VM_WRONG_BLOCK							M4_CH_VM_ERROR_BASE + 0x018D
//Bloque nulo.\n \nBloque nulo en "%1:s".\nDebe asegurarse de que el nodo padre est� correctamente situado.\nEsto sucedi� al ejecutar la funci�n "%0:s".
#define M4_CH_VM_WRONG_BLOCK_ASSIGNING					M4_CH_VM_ERROR_BASE + 0x018E
//Bloque nulo.\n \nBloque nulo al asignar "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de que el nodo padre est� correctamente situado.\nEsto sucedi� al ejecutar "%3:s" de "%4:s" de "%5:s".
#define M4_CH_VM_WRONG_BLOCK_FOR_LDB					M4_CH_VM_ERROR_BASE + 0x018F
//Bloque nulo.\n \nNo se puede realizar la funci�n "%0:s" de la base de datos sobre un bloque nulo en "%1:s".\nDebe asegurarse de que el nodo padre est� correctamente situado.
#define M4_CH_VM_WRONG_BLOCK_READING					M4_CH_VM_ERROR_BASE + 0x0190
//Bloque nulo.\n \nBloque nulo al leer "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de que el nodo padre est� correctamente situado.\nEsto sucedi� al ejecutar "%3:s" de "%4:s" de "%5:s".
#define M4_CH_VM_WRONG_BLOCK_READING_ATTRIBUTE			M4_CH_VM_ERROR_BASE + 0x0191
//Bloque nulo.\n \nBloque nulo al leer un atributo de "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de que el nodo padre est� correctamente situado.
#define M4_CH_VM_WRONG_BLOCK_READING_IN_NULL_EXEC		M4_CH_VM_ERROR_BASE + 0x0192
//Bloque nulo.\n \nBloque nulo al leer "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de que el nodo padre est� correctamente situado.\nEsto sucedi� durante una ejecuci�n vac�a.
#define M4_CH_VM_WRONG_CHANNEL_UNUSED					M4_CH_VM_ERROR_BASE + 0x0193//no se usa
  /*//Error getting Meta4Object "%0:s".\n \n*/
#define M4_CH_VM_WRONG_DEFAULT_INDEX   					M4_CH_VM_ERROR_BASE + 0x0194
//�ndice no v�lido.\n \n�ndice "%0:s" por defecto no v�lido.\nDebe indicar un �ndice expl�citamente.\nEsto sucedi� durante la ejecuci�n de "%1:s".
#define M4_CH_VM_WRONG_GROUP							M4_CH_VM_ERROR_BASE + 0x0195
//Grupo de ejecuci�n no existente.\n \nNo existe el grupo de ejecuci�n "%0:s".\nDebe indicar un grupo de ejecuci�n que exista.
#define M4_CH_VM_WRONG_INSTANCE_HANDLE					M4_CH_VM_ERROR_BASE + 0x0196
//Instancia no encontrada.\n \nNo se encuentra la instancia "%0:s" del Meta4Object "%1:s".\nDebe definir la instancia.
#define M4_CH_VM_WRONG_ITEM_ASSIGN						M4_CH_VM_ERROR_BASE + 0x0197
//Error de asignaci�n.\n \nError al asignar un valor a un tramo o al elemento "%0:s" de "%1:s" de "%2:s".\nDebe revisar su perfil y estar correctamente situado.
#define M4_CH_VM_WRONG_ITEM_CALL_PARAM_NUM				M4_CH_VM_ERROR_BASE + 0x0198
//N�mero de par�metros no v�lido.\n \nN�mero de par�metro no v�lido para "%0:s".\nDebe usar "%1:s" en lugar de "%2:s".
#define M4_CH_VM_WRONG_ITEM_DEFINITION 					M4_CH_VM_ERROR_BASE + 0x0199
//Definici�n de elemento no v�lido.\n \nDefinici�n no v�lida para "%0:s" de "%1:s" de "%2:s".\nDebe tener "%3:s" argumentos en lugar de "%4:s".
#define M4_CH_VM_WRONG_ITEM_PARAM_NUM           	    M4_CH_VM_ERROR_BASE + 0x019A
//Definici�n de elemento no v�lido.\n \nN�mero de par�metro no v�lido  en "%0:s" de "%1:s" de "%2:s".\nDebe tener "%3:s" par�metros en lugar de "%4:s".
#define M4_CH_VM_WRONG_ITEM_READING_ATTRIBUTE			M4_CH_VM_ERROR_BASE + 0x019B
//Elemento no existente.\n \nNo se puede leer un atributo de "%0:s", ya que no existe.\nDebe eliminar la referencia al elemento o definirlo.
#define M4_CH_VM_WRONG_ITEM_WRITE_FIELD					M4_CH_VM_ERROR_BASE + 0x019C
//Campo de escritura inexistente.\n \n"%0:s" de "%1:s" de "%2:s" no tiene campo de escritura o no es v�lido.\nDebe definirle un campo de escritura.
#define M4_CH_VM_WRONG_LANGUAGE							M4_CH_VM_ERROR_BASE + 0x019D
//Tipo de regla no v�lido.\n \nEl tipo de regla "%0:s" no es v�lido. No se puede compilar el elemento "%1:s".\nDebe indicar un tipo de regla v�lido.
#define M4_CH_VM_WRONG_NODE_HANDLE_UNUSED				M4_CH_VM_ERROR_BASE + 0x019E //no se usa
  /*//Handle no encontrado.\n \nNode de handle para el nodo "%0:s" no encontrado, ejecutando "%1:s". */
#define M4_CH_VM_WRONG_NODE_INDEX						M4_CH_VM_ERROR_BASE + 0x019F
//�ndice de nodo no v�lido.\n \nNo se encuentra el nodo en la posici�n "%0:s".\nDebe indicar un �ndice de nodo v�lido.\nEsto sucedi� durante la ejecuci�n de la funci�n "%1:s".
#define M4_CH_VM_WRONG_NODE_NAME						M4_CH_VM_ERROR_BASE + 0x01A0
//Nombre de nodo incorrecto.\n \nNo se encuentra el nodo "%0:s".\nDebe indicar el nombre de un nodo existente.\nEsto sucedi� durante la ejecuci�n de la funci�n "%1:s".
#define M4_CH_VM_WRONG_NOTHING_TO_INSTALL				M4_CH_VM_ERROR_BASE + 0x01A1
//Regla vac�a.\n \nRegla vac�a en un elemento de tipo "%3:s".\nDebe completar la regla.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_PARAM_NUM           				M4_CH_VM_ERROR_BASE + 0x01A2
//N�mero de par�metros no v�lido.\n \nN�mero de par�metro no v�lido en la funci�n "%0:s".\nDebe usar "%1:s" par�metros.
#define M4_CH_VM_WRONG_PARAM_OUT_OF_RANGE				M4_CH_VM_ERROR_BASE + 0x01A3
//Par�metro fuera de rango.\n \nEl par�metro "%0:s" con el valor "%2:s" est� fuera de rango.\nDebe usar un valor dentro del rango permitido.\nEsto sucedi� en la funci�n "%1:s".
#define M4_CH_VM_WRONG_PARAM_POSITION           	    M4_CH_VM_ERROR_BASE + 0x01A4
//Posici�n de argumento no v�lida.\n \nLa posici�n "%1:s" del par�metro no es v�lida. Devuelve "%4:s".\nDebe usar una posici�n entre "%2:s" y "%3:s".\nEsto sucedi� en la funci�n "%0:s".
#define M4_CH_VM_WRONG_PARAM_VALUE						M4_CH_VM_ERROR_BASE + 0x01A5
//Valor no v�lido.\n \nPar�metro "%0:s" con el valor no v�lido "%2:s".\nDebe usar un valor permitido.\nEsto sucedi� en la funci�n "%1:s".
#define M4_CH_VM_WRONG_RECORD_NUMBER					M4_CH_VM_ERROR_BASE + 0x01A6
//Posici�n de fila incorrecta.\n \nEl registro est� mal situado en "%0:s" de "%1:s".\nDebe situarse en una fila v�lida.\nEsto sucedi� durante la ejecuci�n de la funci�n "%2:s".
#define M4_CH_VM_WRONG_RECORD_READING_ATTRIBUTE			M4_CH_VM_ERROR_BASE + 0x01A7
//Posici�n de fila incorrecta.\n \nNo se puede leer el atributo del elemento "%0:s".\nDebe situarse en una fila v�lida.
#define M4_CH_VM_WRONG_RULE_ORDER						M4_CH_VM_ERROR_BASE + 0x01A8
//Orden de regla no v�lido.\n \nOrden de regla no v�lido "%6:s" para la regla "%5:s" de "%3:s" de "%4:s".\nDebe poner un orden dentro de rango.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s" .
#define M4_CH_VM_WRONG_SLICE							M4_CH_VM_ERROR_BASE + 0x01A9
//Tramo no v�lido.\n \nTramo "%0:f" no v�lido para "%1:s" de "%2:s" de "%3:s".\nDebe asegurarse de que el tramo al que est� haciendo referencia exista.\nEsto sucedi� durante la ejecuci�n de la funci�n "%4:s".
#define M4_CH_VM_WRONG_SLICE_CREATION					M4_CH_VM_ERROR_BASE + 0x01AA
//Error al generar ejecuci�n tramada.\n \nError al crear la ejecuci�n por tramos de "%0:s" de "%1:s" de "%2:s".\nDebe revisar su perfil o consultar al administrador.
#define M4_CH_VM_WRONG_SLICE_FORMAT						M4_CH_VM_ERROR_BASE + 0x01AB
//Formato de tramo no v�lido.\n \nFormato de tramo no v�lido para "%0:s".\nDebe ser una fecha o un �ndice num�rico.\nEsto sucedi� durante la ejecuci�n de "%1:s".
#define M4_CH_VM_WRONG_SLICE_READING_ATTRIBUTE			M4_CH_VM_ERROR_BASE + 0x01AC
//Tramo incorrecto.\n \nNo se puede leer el atributo de "%0:s" porque el tramo no es v�lido.\nDebe indicar un tramo existente.
#define M4_CH_VM_WRONG_SORT_FILTER   					M4_CH_VM_ERROR_BASE + 0x01AD
//Filtro de ordenaci�n no v�lido\n \nDebe usar nombres de elementos existentes.
#define M4_CH_VM_WRONG_THIS_NODE_HANDLE					M4_CH_VM_ERROR_BASE + 0x01AE
//Nodo actual incorrecto.\n \nNo se encuentra el identificador del nodo actual. Puede que sea un elemento de �mbito Meta4Object.\nDebe consultar al administrador.\nEsto sucedi� al ejecutar la funci�n "%0:s".
#define M4_CH_VM_WRONG_VALUE							M4_CH_VM_ERROR_BASE + 0x01AF
//Valor no v�lido.\n \nValor "%2:s" no v�lido en "%1:s".\nDebe asignar un valor permitido.\nEsto sucedi� durante la ejecuci�n de la funci�n "%0:s".
#define M4_CH_VM_WRONGINDEX   							M4_CH_VM_ERROR_BASE + 0x01B0
//�ndice inexistente.\n \nEl �ndice "%0:s" no existe en "%1:s" de "%2:s".\nDebe usar un �ndice que exista.\nEsto sucedi� durante la ejecuci�n de la funci�n "%3:s".
#define M4_CH_VM_WRONGTYPEININDEX  						M4_CH_VM_ERROR_BASE + 0x01B1
//Tipo no v�lido en �ndice.\n \nTipo no v�lido para el �ndice "%0:s".\nDebe usar "%1:s" en lugar de "%2:s".
#define	M4_CH_VM_AV_NO_RECORDS_UNUSED					M4_CH_VM_ERROR_BASE + 0x01B2 //no se usa
 /*//No hay filas para aplicar.\n \nNo hay filas para aplicar en el nodo "%0:s" del Meta4Object "%1:s".*/
#define	M4_CH_VM_DATA_TRUNCATED							M4_CH_VM_ERROR_BASE + 0x01B3
//ID demasiado largo.\n \nEl ID "%0:s" ha superado su longitud m�xima "%1:s".\nDebe usar un ID de "%2:s" caracteres como m�ximo.
#define	M4_CH_VM_ERROR_COMPACTING_ASSIGNED				M4_CH_VM_ERROR_BASE + 0x01B4
//Error de compactaci�n.\n \nError al compactar los elementos que se asignaron durante la ejecuci�n de "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de estar bien situado.
#define	M4_CH_VM_ERROR_IN_FIND							M4_CH_VM_ERROR_BASE + 0x01B5
//B�squeda incorrecta.\n \nError al buscar una fila en "%0:s" de "%1:s".\nDebe revisar sus par�metros y asegurarse de que el nodo padre est� situado.\nEsto sucedi� durante la ejecuci�n de la funci�n "%2:s".
#define	M4_CH_VM_NO_ITEM_MATCHES						M4_CH_VM_ERROR_BASE + 0x01B6
//Elemento no encontrado.\n \nNo hay ning�n elemento coincidente durante la ejecuci�n de la funci�n "%0:s".\nDebe revisar los elementos de su M4ta4Object.
#define	M4_CH_VM_NO_RECORDS   							M4_CH_VM_ERROR_BASE + 0x01B7
//Bloque vac�o.\n \nEl bloque actual de "%0:s" de "%1:s" no tiene filas.\nDebe asegurarse de que tenga datos.\nEsto sucedi� durante la ejecuci�n de la funci�n "%2:s".
#define	M4_CH_VM_ROLLBACK_DB_TRANS 						M4_CH_VM_ERROR_BASE + 0x01B8
//Transacci�n cancelada.\n \nEl usuario finaliz� la transacci�n de la base de datos.
#define	M4_CH_VM_ROLLBACK_VM_TRANS 						M4_CH_VM_ERROR_BASE + 0x01B9
//Transacci�n cancelada.\n \nEl usuario finaliz� la transacci�n de la ejecuci�n.
#define	M4_CH_VM_TOO_MANY_COLUMS						M4_CH_VM_ERROR_BASE + 0x01BA
//Demasiadas columnas.\n \nEl n�mero de "%0:s" supera el l�mite.\nDebe usar hasta un m�ximo de "%1:s" columnas.\nEsto sucedi� durante la ejecuci�n de la funci�n "%2:s".
#define	M4_CH_VM_TOO_MANY_PKS							M4_CH_VM_ERROR_BASE + 0x01BB
//Demasiadas claves primarias.\n \nEl n�mero de claves primarias "%0:s" supera el l�mite.\nDebe usar como m�ximo "%1:s" claves primarias.\nEsto sucedi� durante la ejecuci�n de la funci�n "%2:s".
#define	M4_CH_VM_EXECUTING_REMOTE						M4_CH_VM_ERROR_BASE + 0x01BC
//M�todos remotos no permitidos.\n \nNo es posible ejecutar m�todos remotos en la versi�n no cliente.\nDebe ejecutar solo m�todos de la parte cliente.
//M�todos sincronizados no permitidos.\n \nNo es posible ejecutar m�todos de ambas partes sincronizados en la versi�n no cliente.\nDebe ejecutar solo m�todos de la misma parte.
#define	M4_CH_VM_RECURSIVE_EXE_GROUP					M4_CH_VM_ERROR_BASE + 0x01BD
//Grupo de ejecuci�n recursivo.\n \n"%1:s" de "%2:s" de "%3:s" no puede ejecutar el grupo "%0:s" debido a que pertenece al grupo.\nDebe quitar el elemento del grupo.
#define	M4_CH_VM_ROLLBACK_TRANS 						M4_CH_VM_ERROR_BASE + 0x01BE
//Transacci�n cancelada.\n \nEl usuario finaliz� la transacci�n.
#define M4_CH_VM_BAD_OPERATION_ITEM_INDEX 				M4_CH_VM_ERROR_BASE + 0x01BF
//Operaci�n no v�lida.\n \nOperaci�n no v�lida para "%0:s" de "%1:s" de "%2:s" del �ndice "%3:s".\nDebe asegurarse de que s�lo el �ltimo elemento del �ndice pueda tener una operaci�n que no sea IGUAL.
#define M4_CH_VM_SOURCE_RECORD_IS_EOF 					M4_CH_VM_ERROR_BASE + 0x01C0
//Bloque origen incorrectamente situado.\n \nNo se puede copiar o clonar una fila porque el nodo origen "%0:s" de "%1:s" esta colocado de forma incorrecta.\nDebe situarse antes de copiar o clonar la fila.
#define M4_CH_VM_INVALID_PROPAGATION_CALL				M4_CH_VM_ERROR_BASE + 0x01C1
//Nodos multibloque no permitidos.\n \nEl elemento padre de "%0:s" de "%1:s" no puede tener �mbito de nodo en propagaci�n a menos que sea monobloque.\nDebe revisar la parametrizaci�n de los nodos.
#define M4_CH_VM_ITEM_TYPE_MISMATCH_INTERNAL_TYPE		M4_CH_VM_ERROR_BASE + 0x01c2
//Tipo de elemento no v�lido.\n \n"%0:s" de "%1:s" de "%2:s", con el tipo interno "%3:s", tiene un tipo no v�lido.\nDebe usar el tipo adecuado.\nEsto sucedi� durante la ejecuci�n de la funci�n "%4:s".
#define M4_CH_VM_NO_REFERED_ARG_ALLOWED_IN_SLICE_UNUSED	M4_CH_VM_ERROR_BASE + 0x01c3//no sale
 /*//Argumentos por referencia no permitidos.\n \nNo se permiten argumentos por referencia en modo tramo.\nDebe revisar la parametrizaci�n del Meta4Object.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".*/
#define M4_CH_VM_WRITING_SLICE_ATT						M4_CH_VM_ERROR_BASE + 0x01c4
//Asignaci�n de tramo incorrecta.\n \nError al asignar el tramo entre "%3:f" y "%4:f" de "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de estar bien situado y revisar su perfil.\nEsto sucedi� durante la ejecuci�n del atributo "%5:s".
#define M4_CH_VM_ASSIGN_TRUNCATED						M4_CH_VM_ERROR_BASE + 0x01c5//c
//Asignaci�n truncada.\n \nSe trunc� la asignaci�n de "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de que la longitud del valor no excede la precisi�n del elemento.\nEsto sucedi� durante la ejecuci�n de "%3:s".
#define	M4_CH_VM_NO_RECORDS_IN_DATES   					M4_CH_VM_ERROR_BASE + 0x01c6
//Filas fuera de fechas.\n \n"%0:s" de "%1:s" no tiene filas entre las fechas de ejecuci�n.\nDebe asegurarse de que existan filas en las fechas de ejecuci�n.\nEsto sucedi� durante la ejecuci�n de la funci�n "%2:s".
#define M4_CH_VM_WRONG_SLICE_FORMAT_READ				M4_CH_VM_ERROR_BASE + 0x01c7
//Formato de tramo incorrecto.\n \nFormato de tramo no v�lido al leer "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de indicar el tramo mediante una fecha o un �ndice num�rico.\nEsto sucedi� durante la ejecuci�n de "%3:s" de "%4:s" de "%5:s".
#define M4_CH_VM_WRONG_SLICE_FORMAT_WRITE				M4_CH_VM_ERROR_BASE + 0x01c8
//Formato de tramo incorrecto.\n \nFormato de tramo no v�lido al asignar "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de indicar el tramo mediante una fecha o un �ndice num�rico.\nEsto sucedi� durante la ejecuci�n de "%3:s" de "%4:s" de "%5:s".
#define M4_CH_VM_WRONG_SLICE_FORMAT_EXECUTE				M4_CH_VM_ERROR_BASE + 0x01c9
//Formato de tramo incorrecto.\n \nFormato de tramo no v�lido para ejecutar "%0:s" de "%1:s" de "%2:s". Se asume una ejecuci�n vac�a.\nDebe asegurarse de indicar el tramo mediante una fecha o un �ndice num�rico.
#define M4_CH_VM_WRONG_SLICE_FORMAT_ATT					M4_CH_VM_ERROR_BASE + 0x01ca
//Formato de tramo incorrecto.\n \nNo se pueden leer atributos de "%0:s" de "%1:s" de "%2:s", debido a que el formato de tramo no es v�lido.\nDebe asegurarse de indicar el tramo mediante una fecha o un �ndice num�rico.
#define M4_CH_VM_OVERLAPPED_DATES						M4_CH_VM_ERROR_BASE + 0x01cb
//Fechas superpuestas.\n \nFechas superpuestas en la fila "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de que no existan filas con periodos no disjuntos.\nEsto sucedi� durante la ejecuci�n de "%3:s".
#define M4_CH_VM_WRONG_RECORD_NUMBER_FORMAT				M4_CH_VM_ERROR_BASE + 0x01cd
//Formato de fila incorrecto.\n \nFormato de fila no v�lido refiri�ndose a "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de indicar la fila con un valor num�rico.\nEsto sucedi� durante la ejecuci�n del elemento "%3:s" del nodo "%4:s" del Meta4Object "%5:s".
#define M4_CH_VM_WRONG_RECORD_NUMBER_WRITE				M4_CH_VM_ERROR_BASE + 0x01ce
//Fila incorrectamente situada.\n \nNo es posible asignar "%0:s" de "%1:s" de "%2:s" ya que la fila est� colocada incorrectamente.\nDebe situarse en una fila existente.\nEsto sucedi� durante la ejecuci�n de "%3:s" de "%4:s" de "%5:s".
#define M4_CH_VM_WRONG_RECORD_NUMBER_READ				M4_CH_VM_ERROR_BASE + 0x01cf
//Fila incorrectamente situada.\n \nNo es posible leer "%0:s" de "%1:s" de "%2:s" ya que la fila est� colocada incorrectamente.\nDebe situarse en una fila existente.\nEsto sucedi� durante la ejecuci�n de "%3:s" de "%4:s" de "%5:s".
#define M4_CH_VM_UNFLATTEN_NOT_ENOUGH_REG				M4_CH_VM_ERROR_BASE + 0x01d0
//Filas insuficientes.\n \nLa funci�n UNFLATTEN se est� ejecutando en modo sin tramos con menos de 2 filas en "%0:s" de "%1:s". No se crear� ning�n tramo.\nDebe revisar la parametrizaci�n de su Meta4Object.
#define M4_CH_VM_ORG_CHART_CICLE						M4_CH_VM_ERROR_BASE + 0x01d1
//Ciclo en el gr�fico de la organizaci�n.\n \nSe ha encontrado un ciclo en el gr�fico de la organizaci�n en "%0:s" de "%1:s".\nDebe eliminar el ciclo.
#define M4_CH_VM_STT_STATE_NOT_MATCHED					M4_CH_VM_ERROR_BASE + 0x01d2
//Estad�sticas incorrectas.\n \nFalta el estado de ejecuci�n de las estad�sticas. "%0:s" no coincide con el estado final "%1:s".\nDebe consultar al administrador.
#define M4_CH_VM_EXTERNAL_METHOD_ERROR					M4_CH_VM_ERROR_BASE + 0x01d3
//Error en ejecuci�n.\n \nEl m�todo externo "%0:s" de la biblioteca "%1:s" se ejecut� con errores.\nDebe consultar el resto de mensajes.
#define M4_CH_VM_DISABLED_FUNCTION_IN_LOCAL_TEST		M4_CH_VM_ERROR_BASE + 0x01d4
//Funci�n deshabilitada.\n \nLa funci�n "%0:s" no puede ejecutarse en prueba local.\nDebe cambiar el c�digo que desea ejecutar en prueba local.
#define M4_CH_VM_WRONG_RECORD_NUMBER_EXE_ITEM_UNUSED    M4_CH_VM_ERROR_BASE + 0x01d5
  /*//Fila incorrectamente situada.\n \nNo se puede ejecutar un elemento de "%0:s" de "%1:s" ya que la fila est� colocada incorrectamente.\nDebe situarse en una fila existente.\nEsto sucedi� durante la ejecuci�n de "%2:s" de "%3:s" de "%4:s".*/
#define M4_CH_VM_ASSIGN_TRUNCATED_EXE_ITEM				M4_CH_VM_ERROR_BASE + 0x01d6//C
//Asignaci�n truncada.\n \nSe trunc� la asignaci�n de "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de no asignar un valor mas largo que la precisi�n del elemento.\nEsto sucedi� durante la ejecuci�n de "%3:s" de "%4:s" de "%5:s".
#define M4_CH_VM_ERROR_READING_ITEM_AUX					M4_CH_VM_ERROR_BASE + 0x01d7
//Error al leer elemento auxiliar.\n \nError al leer "%0:s" de "%1:s" de "%2:s" con el tipo "%3:s".\nDebe asegurarse de estar bien situado y revisar su perfil.\nEsto sucedi� durante la ejecuci�n de la funci�n "%4:s".
#define M4_CH_VM_READING_ATT_EXE_ITEM					M4_CH_VM_ERROR_BASE + 0x01d8
//Lectura de atributo err�nea.\n \nError al leer un atributo de "%0:s" de "%1:s" de "%2:s".\nDebe asegurarse de estar correctamente situado y revisar su perfil.\nEsto sucedi� durante la ejecuci�n del elemento "%3:s" del nodo "%4:s" del Meta4Object "%5:s".
#define M4_CH_VM_ERROR_IN_FILE							M4_CH_VM_ERROR_BASE + 0x01d9
//No se puede abrir el archivo.\n \nNo se puede abrir el archivo "%1:s" por la raz�n "%2:s".\nDebe asegurarse de la existencia del archivo y de sus permisos sobre �l.\nEsto sucedi� durante la ejecuci�n de la funci�n "%0:s".
#define M4_CH_VM_ITEM_TYPE_MISMATCH_ITEM_AUX			M4_CH_VM_ERROR_BASE + 0x01da
//Tipo de elemento no v�lido.\n \nEl tipo "%3:s" no es v�lido para el elemento el auxiliar "%0:s" de "%1:s" de "%2:s".\nDebe cambiar el tipo del elemento.\nEsto sucedi� durante la ejecuci�n de la funci�n "%3:s".
#define M4_CH_VM_ITEM_IS_NULL_ITEM_AUX					M4_CH_VM_ERROR_BASE + 0x01db
//Valor nulo en elemento auxiliar.\n \nEl elemento auxiliar "%0:s" de "%1:s" de "%2:s" con el tipo interno "%3:s" es nulo.\nDebe asignar un valor al elemento.\nEsto sucedi� durante la ejecuci�n de la funci�n "%4:s".
#define M4_CH_VM_WRONG_DATES_ADDING_SLICE				M4_CH_VM_ERROR_BASE + 0x01dc
//Fechas no v�lidas.\n \nFechas no v�lidas agregando tramos a "%0:s" de "%1:s" de "%2:s".\nDebe usar una fecha de fin "%4:f" posterior a la fecha de inicio "%3:f".\nEsto sucedi� durante la ejecuci�n de la funci�n "%5:s".
#define M4_CH_VM_CANT_CREATE_BLOB_FILE					M4_CH_VM_ERROR_BASE + 0x01dd
//Archivo de datos binarios no creado.\n \nNo se puede crear el archivo de datos binarios para "%0:s" de "%1:s" de "%2:s".\nDebe revisar su perfil y sus permisos sobre el archivo.
#define M4_CH_VM_CANT_SET_BLOB_DESC						M4_CH_VM_ERROR_BASE + 0x01de
//Descripci�n de datos binarios no establecida.\n \nNo se puede definir la descripci�n "%3:s" de los datos binarios de "%0:s" de "%1:s" de "%2:s".\nDebe revisar su perfil y asegurarse de estar correctamente situado.
#define M4_CH_VM_NO_LOAD_PERM							M4_CH_VM_ERROR_BASE + 0x01df
//Acceso de lectura denegado.\n \nNo tiene permisos de lectura.\nDebe verificar su perfil.
#define M4_CH_VM_NO_INSERT_PERM							M4_CH_VM_ERROR_BASE + 0x01e0
//Acceso de escritura denegado.\n \nNo tiene permisos de inserci�n.\nDebe verificar su perfil.
#define M4_CH_VM_NO_UPDATE_PERM							M4_CH_VM_ERROR_BASE + 0x01e1
//Acceso de modificaci�n denegado.\n \nNo tiene permisos para modificar.\nDebe verificar su perfil.
#define M4_CH_VM_NO_DELETE_PERM							M4_CH_VM_ERROR_BASE + 0x01e2
//Acceso de escritura denegado.\n \nNo tiene permisos de borrado.\nDebe verificar su perfil.
#define M4_CH_VM_NO_PERSIST_PERM						M4_CH_VM_ERROR_BASE + 0x01e3
//Acceso de escritura denegado.\n \nNo tiene permisos de escritura.\nDebe verificar su perfil.
#define M4_CH_VM_WRONG_DEF_INDEX_ITEM_NUMBER			M4_CH_VM_ERROR_BASE + 0x01e4
//N�mero de elemento incorrecto.\n \nN�mero de elementos del �ndice por defecto de "%0:s" de "%1:s" incorrecto.\nDebe tener "%2:s" elementos en lugar de "%3:s".
#define M4_CH_VM_AV_NO_DATES_SET						M4_CH_VM_ERROR_BASE + 0x01e5
//Fechas de trabajo no establecidas.\n \nNo se han definido las fechas de trabajo de Aplicavalor.\nDebe definirlas mediante el m�todo SetDates antes de usar el Aplicavalor.
#define M4_CH_VM_PARAMETER_TYPES_NOT_VALID				M4_CH_VM_ERROR_BASE + 0x01e6
//Tipos no v�lidos.\n \nNo se puede realizar la operaci�n "%0:s" porque los tipos "%1:s", "%2:s" no son v�lidos.\nDebe usar tipos compatibles.\nEsto sucedi� en "%3:s" de "%4:s" de "%5:s".
#define M4_CH_VM_CUR_NO_SCR_ROOT_EXCHANGE_RATE			M4_CH_VM_ERROR_BASE + 0x01e7
//Tipo de cambio no encontrado.\n \nNo se encontr� tipo de cambio para moneda origen "%0:s", moneda destino "%1:s" y moneda ra�z del origen "%2:s".\nDebe consultar los valores de cambio.
#define M4_CH_VM_CUR_NO_DST_ROOT_EXCHANGE_RATE			M4_CH_VM_ERROR_BASE + 0x01e8
//Tipo de cambio no encontrado.\n \nNo se encontr� tipo de cambio para moneda origen "%0:s", moneda destino "%1:s" y moneda ra�z del destino"%2:s".\nDebe consultar los valores de cambio.
#define M4_CH_VM_CUR_SELF_ROOT							M4_CH_VM_ERROR_BASE + 0x01e9
//Moneda ra�z incorrecta.\n \nLa moneda es ra�z de s� misma.\nDebe revisar los valores de cambio.
#define M4_CH_VM_WRONG_ITEM_INDEX						M4_CH_VM_ERROR_BASE + 0x01ea
//�ndice de elemento incorrecto.\n \nEl �ndice de elemento "%0:s" de "%1:s" est� fuera del rango permitido.\nDebe usar un �ndice de elemento v�lido.\nEsto sucedi� al ejecutar la funci�n "%2:s". 
#define M4_CH_VM_WRONG_OPERATION						M4_CH_VM_ERROR_BASE + 0x01eb
//Operaci�n incorrecta.\n \nLa funci�n "%0:s" no se puede ejecutar.\nDebe asegurarse de estar correctamente situado o revisar su perfil.
#define M4_CH_VM_CANT_RESIZE_STATE_STACK				M4_CH_VM_ERROR_BASE + 0x01ec
//Ejecuci�n demasiado compleja.\n \nNo se puede seguir ejecutando debido a la excesiva complejidad del c�digo, a demasiadas ejecuciones propagadas o a una recursividad infinita.\nDebe reducir la complejidad de su c�digo.\nEsto sucedi� al intentar iniciar la ejecuci�n de "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_WRONG_REGEX							M4_CH_VM_ERROR_BASE + 0x01ed
//Expresi�n incorrecta.\n \nLa expresi�n regular "%0:s" no es correcta.\nDebe cambiar la expresi�n.
#define M4_CH_VM_AUTO_M4OBJ_FUNCTION					M4_CH_VM_ERROR_BASE + 0x01ee
//Funci�n inv�lida sobre Meta4Object en ejecuci�n.\n \nLa funci�n "%0:s" no se puede aplicar sobre el Meta4Object que se est� ejecutando.\nDebe aplicar la funci�n sobre un Meta4Object de nivel dos.\nEsto sucedi� en "%1:s". 
#define M4_CH_VM_NO_DESTROY_FILE						M4_CH_VM_ERROR_BASE + 0x01ef
//No se puede borrar el archivo.\n \nEl archivo "%0:s" no se puede borrar porque no tiene permisos.\nDebe obtener permisos de borrado sobre ese archivo. 
#define M4_CH_VM_TRY_WRITE_METHOD						M4_CH_VM_ERROR_BASE + 0x01f0
//Tipo de elemento incorrecto.\n \nError al asignar "%0:s" de "%1:s" de "%2:s" porque el elemento es de tipo m�todo.\nDebe cambiar el c�digo o el tipo del elemento.\nEsto sucedi� al ejecutar la funci�n "%3:s".
#define M4_CH_VM_TRY_READ_METHOD						M4_CH_VM_ERROR_BASE + 0x01f1
//Tipo de elemento incorrecto.\n \nError al leer "%0:s" de "%1:s" de "%2:s" porque el elemento es de tipo m�todo.\nDebe cambiar el c�digo o el tipo del elemento.\nEsto sucedi� al ejecutar la funci�n "%3:s".
#define M4_CH_VM_TRY_PRIOR_METHOD						M4_CH_VM_ERROR_BASE + 0x01f2
//Tipo de elemento incorrecto.\n \nNo se puede cambiar la prioridad de "%0:s" de "%1:s" de "%2:s" porque el elemento es de tipo m�todo.\nDebe cambiar el c�digo o el tipo del elemento.\nEsto sucedi� al ejecutar la funci�n "%3:s".
#define M4_CH_VM_DATE_OVERFLOW							M4_CH_VM_ERROR_BASE + 0x01f3
//Fecha incorrecta.\n \nNo se puede tratar una fecha fuera de rango. Usando la fecha "%0:d" en su lugar.\nDebe usar una fecha entre los rangos permitidos.\nEsto sucedi� al ejecutar la funci�n "%1:s".
#define M4_CH_VM_ID_TOO_LONG							M4_CH_VM_ERROR_BASE + 0x01f4
//Identificador incorrecto.\n \nEl identificador "%0:s" es demasiado largo. El identificador ha sido truncado.\nDebe ser como m�ximo de "%1:s" caracteres.\nEsto sucedi� al ejecutar la funci�n "%2:s".
#define M4_CH_VM_TRY_EXECUTE_FIELD_PROP					M4_CH_VM_ERROR_BASE + 0x01f5
//Tipo de elemento incorrecto.\n \nError al intentar ejecutar "%0:s" de "%1:s" de "%2:s" porque el elemento no es m�todo ni concepto.\nDebe cambiar el c�digo o el tipo del elemento.\nEsto sucedi� al ejecutar la funci�n "%3:s".
#define M4_CH_VM_STRING_TOO_SHORT						M4_CH_VM_ERROR_BASE + 0x01f6
//Cadena demasiado corta.\n \nDebe aumentar la cadena hasta "%1:s" caracteres como m�nimo.\nEsto sucedi� en la funci�n "%0:s".
#define M4_CH_VM_WRONG_FUNCT_CONTEXT					M4_CH_VM_ERROR_BASE + 0x01f7
//Funci�n inv�lida.\n \nLa funci�n "%0:s" ha sido llamada en modo server.\nDebe invocar la llamada en modo cliente."
#define M4_CH_VM_NO_SUCH_COL_PROP						M4_CH_VM_ERROR_BASE + 0x01f8
//No existe el atributo de Meta4Object.\n \nEl atributo de Meta4Object "%0:s" no es un atributo v�lido.\nDebe consultar con el administrador.
#define M4_CH_VM_INVALID_COMPARE_FUNCTION				M4_CH_VM_ERROR_BASE + 0x01f9
//N�mero de funci�n no v�lido.\n \nLa funci�n "%0:s" no es una funci�n v�lida.\nDebe usar una funci�n comprendida en el rango que va de "%1:s" a "%2:s".\nEsto ha sucedido al invocar la funci�n "FindRegister".
#define M4_CH_VM_LDB_METHOD_IN_CLIENT_NO_MAPPED			M4_CH_VM_ERROR_BASE + 0x01fa
//Ejecuci�n de m�todo incorrecta.\n \nSe est� ejecutando el m�todo "%0:s" en un cliente distribuido porque no es un m�todo sincronizado.\nEl elemento debe estar definido como "Ambas partes sincronizadas".\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_VM_ERROR_SERIALIZE_RECORDSET_IN_M4VMSTATE    M4_CH_VM_ERROR_BASE + 0x01fb
//Error en la serializaci�n del estado de ejecuci�n.\n \nNo se serializ� el conjunto de filas asociado a la ejecuci�n.\nDebe comprobar la configuraci�n Cliente/Servidor.\nEsto ha sucedido en "%0:s" de "%1:s".
#define M4_VM_INCONSISTENT_PROPAGATION_REGISTER		    M4_CH_VM_ERROR_BASE + 0x01fc
//Error en propagaci�n.\n \nSe est� intentando situarse en un registro inexistente.\nDebe comprobar la parametrizaci�n de la propagaci�n.
#define M4_VM_EXCHANGE_INVALID_REGISTER				    M4_CH_VM_ERROR_BASE + 0x01fd
//Error en cambio de moneda.\n \nSe est� intentando efectuar un cambio de moneda en un registro no v�lido. Esto ocurri� en la funci�n "%0:s".\nDebe comprobar posici�n del registro.
#define M4_VM_WRONG_ARGUMENT_VALUE					    M4_CH_VM_ERROR_BASE + 0x01fe
//Tipo incorrecto de argumento.\n \nEl argumento "%0:s" de la funci�n "%1:s" es de tipo "%2:s" y deber�a ser de tipo "%3:s".\nDebe cambiar la llamada a la funci�n.\nEsto sucedi� en la l�nea "%4:s" de "%5:s" de "%6:s" de "%7:s" de "%8:s".
#define M4_VM_BAD_M4OBJ_FOR_BP						    M4_CH_VM_ERROR_BASE + 0x01ff
//Meta4Object mal definido.\n \nNo se puede establecer un punto de ruptura en la l�nea "%0:s" porque el Meta4Object "%4:s" no est� bien definido.\nEsto sucedi� en "%1:s" de "%2:s" de "%3:s".
#define M4_VM_BP_IN_REENTER							    M4_CH_VM_ERROR_BASE + 0x0200
//Punto de ruptura ignorado.\n \nNo se puede parar en la l�nea "%0:s" porque la ejecuci�n es reentrante.\nEsto sucedi� en "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_NO_PARAM_METHOD		           	    M4_CH_VM_ERROR_BASE + 0x0201
//M�todo sin argumentos.\n \nNo se puede obtener el argumento "%0:s" porque el m�todo "%1:s" no tiene argumentos.\nDebe revisar su c�digo.\nEsto sucedi� en "%2:s" de "%3:s".
#define M4_CH_VM_BAD_METHOD_PARAM		           	    M4_CH_VM_ERROR_BASE + 0x0202
//No existe el argumento.\n \nNo se puede obtener el argumento "%0:s" porque el m�todo "%2:s" tiene "%1:s" argumentos.\nDebe revisar su c�digo.\nEsto sucedi� en "%3:s" de "%4:s".
#define M4_CH_VM_NO_SUCH_FUNCTION		           	    M4_CH_VM_ERROR_BASE + 0x0203
//Error interno.\n \nLa funci�n "%0:s" no existe en LN4.\nDebe consultar con el administrador.
#define M4_CH_VM_EXCLUDED_FUNCTION		           	    M4_CH_VM_ERROR_BASE + 0x0204
//Funci�n deshabilitada temporalmente.\n \nNo se puede ejecutar la funci�n "%0:s" en el contexto actual.\nEsto sucedi� en "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_CANT_READ_ITEM							M4_CH_VM_ERROR_BASE + 0x0205
//No se puede leer elemento.\n \nNo se puede leer el elemento "%0:s" de "%1:s" de "%2:s" en el registro "%3:s".\nDebe asegurarse de tener permisos y estar correctamente situado.\nEsto sucedi� al ejecutar "%4:s" de "%5:s" de "%6:s".
#define M4_CH_VM_CANT_READ_SLICE_BY_INTERVAL			M4_CH_VM_ERROR_BASE + 0x0206
//No se puede leer tramo.\n \nNo se puede leer el tramo desde "%0:d" hasta "%1:d" de "%2:s" de "%3:s" de "%4:s" en el registro "%5:s".\nDebe asegurarse de tener permisos y estar correctamente situado.\nEsto sucedi� al ejecutar "%6:s" de "%7:s" de "%8:s".
#define M4_CH_VM_CANT_READ_SLICE_BY_DATE				M4_CH_VM_ERROR_BASE + 0x0207
//No se puede leer tramo.\n \nNo se puede leer el tramo "%0:d" de "%1:s" de "%2:s" de "%3:s" en el registro "%4:s".\nDebe asegurarse de tener permisos y estar correctamente situado.\nEsto sucedi� al ejecutar "%5:s" de "%6:s" de "%7:s".
#define M4_CH_VM_CANT_READ_SLICE_BY_INDEX				M4_CH_VM_ERROR_BASE + 0x0208
//No se puede leer tramo.\n \nNo se puede leer el tramo "%0:f" de "%1:s" de "%2:s" de "%3:s" en el registro "%4:s".\nDebe asegurarse de tener permisos y estar correctamente situado.\nEsto sucedi� al ejecutar "%5:s" de "%6:s" de "%7:s".
#define M4_CH_VM_REPEATED_LOAD_IN_PARTIAL				M4_CH_VM_ERROR_BASE + 0x0209
//Carga repetida en carga parcial.\n \nNo se puede volver a cargar un nodo de carga parcial que ya ha sido cargado.\nDebe cambiar el c�digo del elemento.\nEsto sucedi� intentando ejecutar "%0:s" de "%1:s" de "%2:s".
#define M4_CH_VM_BAD_LOACALTEST_M4OBJ					M4_CH_VM_ERROR_BASE + 0x020A
//Meta4Object incorrecto.\n \nNo se puede invocar la funci�n "LocalTest" en un Meta4Object que no sea "DESIGN_CHANNEL".\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_BAD_YTDSEARCH_INDEX					M4_CH_VM_ERROR_BASE + 0x020B
//�ndice incorrecto.\n \nEl �ndice "%0:s" no existe o no es correcto para la funci�n YTDSearch.\nEsto puede causar que la funci�n no retorne los valores correctos.\nDebe definir el �ndice correctamente.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_VM_BASE_SLICED_ITEM_USE					M4_CH_VM_ERROR_BASE + 0x020C
//Uso incorrecto de un elemento.\n \nSe intenta leer o escribir en modo no tramos el elemento "%0:s" de "%1:s" de "%2:s" que tiene "%3:s" tramos y est� posicionado en el tramo "%4:s".\nDebe cambiar el c�digo del elemento.\nEsto sucedi� ejecutando la l�nea "%5:s" de "%6:s" de "%7:s" de "%8:s" de "%9:s" con la fecha de trabajo "%10:d".
#define M4_CH_VM_NO_WRITE_IN_FILE						M4_CH_VM_ERROR_BASE + 0x020D
//No se puede escribir en el archivo.\n \nNo se puede escribir en el archivo "%1:s" por la raz�n "%2:s".\nDebe asegurarse de que hay suficiente espacio en el disco duro y de sus permisos sobre el fichero.\nEsto sucedi� durante la ejecuci�n de la funci�n "%0:s".
#define M4_CH_VM_LDB_METHOD_IN_CLIENT_NO_SEPARABLE		M4_CH_VM_ERROR_BASE + 0x020E
//Ejecuci�n de m�todo incorrecta.\n \nSe est� ejecutando el m�todo "%0:s" en un cliente distribuido porque el Meta4Object no es "Separable".\nEl Meta4Object debe estar definido como "Separable".\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_CH_VM_NO_GET_BOOKMARK						M4_CH_VM_ERROR_BASE + 0x020F
//No se puede obtener el bookmark.\n \nSe ha producido un error al intentar obtener un bookmark.\nDebe consultar con el administrador de su sistema.\nEsto sucedi� en "%0:s".
#define M4_CH_VM_NO_APPLY_BOOKMARK						M4_CH_VM_ERROR_BASE + 0x0210
//No se puede aplicar el bookmark.\n \nSe ha producido un error al intentar aplicar el bookmark "%0:s".\nPosiblemente ya se ha liberado con anterioridad.\nEsto sucedi� en "%1:s".
#define M4_CH_VM_NO_RELEASE_BOOKMARK					M4_CH_VM_ERROR_BASE + 0x0211
//No se puede liberar el bookmark.\n \nSe ha producido un error al intentar liberar el bookmark "%0:s".\nPosiblemente ya se ha liberado con anterioridad.\nEsto sucedi� en "%1:s".
#define M4_CH_VM_NO_POOL_HANDLE							M4_CH_VM_ERROR_BASE + 0x0212
//Handle de pool incorrecto.\n \nEl handle de pool de cadenas "%0:s" no existe.\nEl m�ximo valor de un handle es "%1:s".



// ERRORES DE COMPILACION ******/
#define M4_CH_VM_COMP_ASSIGN_TO_CONST					M4_CH_VM_ERROR_BASE + 0x4000
//Error de compilaci�n.\n \nNo se puede asignar un valor a la constante "%5:s".\nDebe cambiar el c�digo.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_CANT_UPDATE_CHAN_DEPEND_UNUSED	M4_CH_VM_ERROR_BASE + 0x4001 //ya no se usa 3.1
  /**/
#define M4_CH_VM_COMP_CANT_UPDATE_DEPEND				M4_CH_VM_ERROR_BASE + 0x4002
//Error de compilaci�n.\n \nNo se pueden actualizar las dependencias. Se perder� la informaci�n para la dependencia "%5:s".\nDebe revisar sus permisos sobre las tablas de dependencias.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_CANT_UPDATE_EXT_DEPEND_UNUSED		M4_CH_VM_ERROR_BASE + 0x4003 //ya no se usa 3.1
  /**/
#define M4_CH_VM_COMP_END_WITH_ERRORS_UNUSED			M4_CH_VM_ERROR_BASE + 0x4004 //NO SE USA
  /**/
#define M4_CH_VM_COMP_GENERAL_ERROR		      			M4_CH_VM_ERROR_BASE + 0x4005
//Error de compilaci�n.\n \nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_IS_NOT_ITEM						M4_CH_VM_ERROR_BASE + 0x4006
//Error de compilaci�n.\n \nNo existe "%5:s", por lo tanto, no tiene atributos.\nDebe cambiar el c�digo.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_ITEM_NOT_EXIST					M4_CH_VM_ERROR_BASE + 0x4007
//Error de compilaci�n.\n \nEl elemento "%5:s" no existe.\nDebe cambiar el c�digo.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_ITEN_CANT_BE_ITERATION_VAR		M4_CH_VM_ERROR_BASE + 0x4008
//Error de compilaci�n.\n \n"%5:s" es un elemento y no se puede usar como variable de iteraci�n en un bucle.\nDebe iterar sobre una variable.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_METHOD_L0_MUST_BE_L1				M4_CH_VM_ERROR_BASE + 0x4009
//Error de compilaci�n.\n \nNo se permiten m�todos de nivel 0. El m�todo "%5:s" debe tener ser al menos de nivel 1.\nDebe cambiar el nivel del m�todo.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_NULL_DEPENDENCY					M4_CH_VM_ERROR_BASE + 0x400A
//Error de compilaci�n.\n \nNo se puede usar una cadena vac�a como ID Elemento.\nDebe cambiar el ID Elemento.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_UNABLE_CREATE_DEPEND				M4_CH_VM_ERROR_BASE + 0x400B
//Error de compilaci�n.\n \nNo se puede crear una dependencia para el elemento "%5:s".\nDebe consultar con el administrador de su sistema.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_UNABLE_GET_LN4_CODE				M4_CH_VM_ERROR_BASE + 0x400C
//Error de compilaci�n.\n \nNo se puede obtener el c�digo LN4 para compilar.\nDebe consultar con el administrador de su sistema.
#define M4_CH_VM_COMP_UNDEF_ATTRIB						M4_CH_VM_ERROR_BASE + 0x400D
//Error de compilaci�n.\n \nEl atributo "%5:s" para "%6:s" no est� definido.\nDebe cambiar el c�digo.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_UNDEF_ATTRIB_METHOD				M4_CH_VM_ERROR_BASE + 0x400E
//Error de compilaci�n.\n \nNo est� definido el atributo de tipo m�todo "%5:s".\nDebe cambiar el c�digo.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_UNDEF_CHANNEL_FUNCTION			M4_CH_VM_ERROR_BASE + 0x400F
//Error de compilaci�n.\n \nLa funci�n de Meta4Object "%5:s" no existe.\nDebe cambiar el c�digo.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_UNDEF_FUNCTION_METHOD				M4_CH_VM_ERROR_BASE + 0x4010
//Error de compilaci�n.\n \nEl m�todo o funci�n "%5:s" no existe.\nDebe cambiar el c�digo.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_UNDEF_ITEM						M4_CH_VM_ERROR_BASE + 0x4011
//Error de compilaci�n.\n \n"%5:s" de "%6:s" no existe.\nDebe cambiar el c�digo.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_UNDEF_ITEM_VARIABLE				M4_CH_VM_ERROR_BASE + 0x4012
//Error de compilaci�n.\n \nEl elemento o variable "%5:s" no existe.\nDebe cambiar el c�digo.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_UNDEF_METHOD_IN_PARENT			M4_CH_VM_ERROR_BASE + 0x4013
//Error de compilaci�n.\n \nNo est� definido el m�todo "%5:s" para el nodo padre.\nDebe cambiar el conector.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_UNDEF_METHOD_RULE					M4_CH_VM_ERROR_BASE + 0x4014
//Error de compilaci�n.\n \nEl m�todo o regla "%5:s" no existe.\nDebe cambiar el c�digo.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_UNDEF_METHOD_RULE_IN_PARENT		M4_CH_VM_ERROR_BASE + 0x4015
//Error de compilaci�n.\n \nNo est� definido el m�todo o regla "%5:s" en el nodo padre.\nDebe cambiar el conector.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_WRONG_ARGUMENT_TYPE_UNUSED		M4_CH_VM_ERROR_BASE + 0x4016//no se usa
  /**/
#define M4_CH_VM_COMP_WRONG_FUNCTION_METHOD_NOT_PARAM	M4_CH_VM_ERROR_BASE + 0x4017
//Error de compilaci�n.\n \nLa funci�n o m�todo "%5:s" no necesita argumentos.\nDebe quitar los argumentos de la llamada.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_WRONG_NUM_PARAM_IN_FUNCTION		M4_CH_VM_ERROR_BASE + 0x4018
//Error de compilaci�n.\n \nN�mero incorrecto de par�metros para el m�todo o funci�n "%6:s". Necesita "%5:s" par�metros.\nDebe pasar el n�mero correcto de argumentos.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_WRONG_NUM_PARAM_IN_FUNCTION_LEAST	M4_CH_VM_ERROR_BASE + 0x4019
//Error de compilaci�n.\n \nN�mero incorrecto de par�metros para el m�todo o funci�n "%6:s". Necesita al menos "%5:s" par�metros.\nDebe pasar el n�mero correcto de argumentos.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_WRONG_PARAM_ATTRIB_METHOD			M4_CH_VM_ERROR_BASE + 0x401A
//Error de compilaci�n.\n \nN�mero incorrecto de par�metros para el atributo de tipo m�todo "%5:s".\nDebe pasar el n�mero correcto de argumentos.\nEsto sucedi� en la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
 


// ERRORES INTERNOS DE COMPILACION ******/
#define M4_CH_VM_COMP_IE_ADD_SYMBOL						M4_CH_VM_ERROR_BASE + 0x401B
//Error interno de compilaci�n.\n \nError al agregar el s�mbolo "%5:s".\nDebe consultar con el administrador de su sistema.\nEsto sucedi� al compilar la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_IE_CANT_ADD_REG_CHANN_DEPEND_UNUSED	M4_CH_VM_ERROR_BASE + 0x401C //NO SE USA
  /**/
#define M4_CH_VM_COMP_IE_CANT_ADD_REG_DEPEND			M4_CH_VM_ERROR_BASE + 0x401D //ya no es interno, puede pasar!
//Error de compilaci�n.\n \nError al compilar en la l�nea "%0:s" No se puede agregar una fila para la nueva dependencia.\nDebe revisar sus permisos sobre las tablas de dependencias.\nEsto sucedi� al compilar la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_IE_CANT_GET_LABELS_STACK			M4_CH_VM_ERROR_BASE + 0x401E
//Error interno de compilaci�n.\n \nNo se pueden obtener etiquetas de la pila.\nDebe consultar con el administrador de su sistema.\n
#define M4_CH_VM_COMP_IE_INTERNAL_ERROR					M4_CH_VM_ERROR_BASE + 0x401F
//Error interno de compilaci�n.\n \n\nDebe consultar con el administrador de su sistema.
#define M4_CH_VM_COMP_IE_LONG_ID						M4_CH_VM_ERROR_BASE + 0x4020
//Error de compilaci�n.\n \nEl identificador del elemento o del nodo son mayores que su tama�o m�ximo.\nDebe cambiar los identificadores.\nEsto sucedi� al compilar la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_IE_NO_FUNCTION					M4_CH_VM_ERROR_BASE + 0x4021
//Error interno de compilaci�n.\n \nNo existe la funci�n "%0:s".\nDebe consultar al administrador de su sistema.
#define M4_CH_VM_COMP_IE_NOT_MEMORY						M4_CH_VM_ERROR_BASE + 0x4022
//Error de compilaci�n.\n \nNo hay suficiente memoria para finalizar la compilaci�n.\nDebe consultar al administrador de su sistema.\nEsto sucedi� al compilar la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_IE_READ_SYMBOL					M4_CH_VM_ERROR_BASE + 0x4023
//Error interno de compilaci�n.\n \nError al leer s�mbolo "%5:s".\nDebe consultar al administrador de su sistema.\nEsto sucedi� al compilar la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_IE_SETTING_ARGS					M4_CH_VM_ERROR_BASE + 0x4024
//Error interno de compilaci�n.\n \nError al definir los argumentos.\nDebe consultar al administrador de su sistema.
#define M4_CH_VM_COMP_IE_TOO_PARAMS						M4_CH_VM_ERROR_BASE + 0x4025
//Error de compilaci�n.\n \nDemasiados par�metros.\nDebe reducir el n�mero de par�metros.
#define M4_CH_VM_LEXER_ERROR               				M4_CH_VM_ERROR_BASE + 0x4026
//Error interno de compilaci�n.\n \nError "%0:s" en el analizador l�xico.\nDebe consultar al administrador de su sistema.
#define M4_CH_VM_COMP_ITEM_NOT_FOUND_COMPILING			M4_CH_VM_ERROR_BASE + 0x4027
//Error interno de compilaci�n.\n \nEl elemento "%0:s" no se encuentra, y no se puede compilar.\nDebe consultar al administrador de su sistema.
#define M4_CH_VM_COMP_ITEM_NODE_NOT_EXIST				M4_CH_VM_ERROR_BASE + 0x4028
//Error de compilaci�n.\n \n"%5:s" de "%6:s" no existe.\nDebe cambiar el c�digo.\nEsto sucedi� al compilar la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_UNDEF_METHOD_NODE					M4_CH_VM_ERROR_BASE + 0x4029
//Error de compilaci�n.\n \nEl m�todo "%5:s" de "%6:s" no existe.\nDebe cambiar el c�digo.\nEsto sucedi� al compilar la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_WRONG_ARGUMENT_TYPE_REF			M4_CH_VM_ERROR_BASE + 0x402a
//Error de compilaci�n.\n \nEl argumento "%5:s" para el elemento, funci�n o atributo "%6:s" tiene un tipo incorrecto.\nDebe ser pasado por referencia.\nEsto sucedi� al compilar la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_WRONG_ARGUMENT_TYPE_VALUE			M4_CH_VM_ERROR_BASE + 0x402b
//Error de compilaci�n.\n \nEl argumento "%5:s" para el elemento, funci�n o atributo "%6:s" es incorrecto.\nDebe cambiar el c�digo.\nEsto sucedi� al compilar la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_UNDEF_FUNCTION					M4_CH_VM_ERROR_BASE + 0x402c
//Error de compilaci�n.\n \nLa funci�n "%5:s" no existe.\nDebe cambiar su c�digo.\nEsto sucedi� al compilar la l�nea "%0:s" de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_NODAL_ITEM_CALL_FROM_NODAL_ITEM	M4_CH_VM_ERROR_BASE + 0x402d
//Error de compilaci�n.\n \nIntento de utilizaci�n del elemento de nodo "%0:s" del Meta4Object "%1:s" desde otro elemento de nodo.\nDebe cambiar el c�digo de la regla.\nEsto sucedi� al compilar un elemento que est� definido �nicamente para el Meta4Object "%2:s"."
#define M4_CH_VM_COMP_NODAL_ITEM_CALL_FROM_NORMAL_ITEM	M4_CH_VM_ERROR_BASE + 0x402e
//Error de compilaci�n.\n \nIntento de utilizaci�n del elemento de nodo "%0:s" desde otro elemento que no es de nodo.\nDebe cambiar el c�digo de la regla.\nEsto sucedi� al compilar un elemento que puede pertenecer a cualquier Meta4Object.



// ERRORES DE COMPILACION JIT ******
#define M4_CH_VM_NODE_NOT_FOUND_COMP_JIT				M4_CH_VM_ERROR_BASE + 0x4030
//Nodo inexistente.\n \nNo se encuentra el nodo "%0:s" durante la compilaci�n del c�digo JIT.\nDebe cambiar el c�digo.
#define M4_CH_VM_CANT_BUILD_CHANNEL_COMP_JIT			M4_CH_VM_ERROR_BASE + 0x4031
//Definici�n de Meta4Object incorrecto.\n \nNo se puede crear la definici�n del Meta4Object "%0:s" para la instancia "%1:s" durante la compilaci�n del c�digo JIT.\nDebe cambiar el c�digo.
#define M4_CH_VM_INSTANCE_NOT_FOUND_COMP_JIT			M4_CH_VM_ERROR_BASE + 0x4032
//Instancia inexistente.\n \nNo se encuentra la instancia "%0:s" durante la compilaci�n del c�digo JIT.\nDebe cambiar el c�digo.
#define M4_CH_VM_CHANNEL_NOT_FOUND_COMP_JIT				M4_CH_VM_ERROR_BASE + 0x4033
//Meta4Object inexistente.\n \nNo se encuentra el Meta4Object "%0:s" durante la compilaci�n del c�digo JIT.\nDebe cambiar el c�digo.
#define M4_CH_VM_COMP_STRING_NOT_CLOSED					M4_CH_VM_ERROR_BASE + 0x4034
//Error de compilaci�n.\n \nCadena de texto sin terminar.\nDebe cambiar el c�digo de la regla.\nEsto sucedi� en la l�nea "%0:s" del c�digo de "%1:s" de "%2:s" de "%3:s" de "%4:s".
#define M4_CH_VM_COMP_NUMBER_TOO_BIG					M4_CH_VM_ERROR_BASE + 0x4035
//Error de compilaci�n.\n \nN�mero demasiado grande.\nDebe cambiar el c�digo de la regla.\nEsto sucedi� en la l�nea "%0:s" del c�digo de "%1:s" de "%2:s" de "%3:s" de "%4:s".





// Otros: VM_WRAPPER ****
#define M4_CH_VM_BUILD_FROM_ID							M4_CH_VM_ERROR_BASE + 0x5000
//Definici�n de Meta4Object incorrecto.\n \nError al crear la definici�n del Meta4Object "%0:s".
#define	M4_CH_VM_CANT_GET_INS	   						M4_CH_VM_ERROR_BASE + 0x5001
//No se pueden obtener instrucciones de ejecuci�n.\n \n
#define M4_CH_VM_ERROR_RECOVERED            			M4_CH_VM_ERROR_BASE + 0x5002
//Recuperaci�n de error.\n \nError de ejecuci�n interno recuperado en el nivel "%0:s".
#define	M4_CH_VM_NOT_RUNNING	   						M4_CH_VM_ERROR_BASE + 0x5003
//Ejecuci�n vac�a.\n \nNo se puede ejecutar "%0:s". No hay nada para ejecutar.




#endif // __VMRES_HPP
