#ifndef __M4DMSRES_HPP__
#define __M4DMSRES_HPP__


#include <m4res.hpp>

// Cambiamos la base de los códigos de error porque en 5.0 están
// asignados para el gestor documental.
// #define M4_SRV_DMS_IMPORT_XML_ERROR			0x00780690		// 7866000
#define M4_SRV_DMS_IMPORT_XML_ERROR				0x00780E60		// 7868000

///////////////////////////
//ERRORES DE DMS
/////////////////////////////
// "Error interno en la librería de XML.\n \nLa librería de XML no puede realizar correctamente el análisis del modelo XML indicado. La causa del error es: %s.\nDebe consultar con el administrador.\n"
#define M4_DMS_XML_PARSE_ERROR					0x0000  +  M4_SRV_DMS_IMPORT_XML_ERROR
// "El proceso de importación de datos XML ha finalizado en el registro %0:d.\n \n La causa ha sido %1:d.\n"
#define M4_DMS_XML_PARSE_END					0x0001  +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error leyendo datos XML. Ha ocurrido un error: "%0:s" procesando la entidad "%1:s" identificador "%2:s" en la línea %3:n columna %4:n. Esto ha sucedido en %5:s.\nDebe comprobar que la sintaxis XML de entrada es correcta.\n"
#define M4_DMS_XML_DUMP_MESSAGE					0x0002 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error en el proceso de importación XML. Se ha producido un error en el proceso de importación XML en el elemento %0:s.\nDebe comprobar que la sintaxis XML de entrada es correcta.\n"
#define M4_DMS_IMPORT_XML_ERROR					0x0003 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Puntero nulo. \n\n El valor que se ha asignado un puntero es nulo: %0:s. \n"
#define M4_DMS_POINTER_NULL						0x0004 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al construir el entorno de Meta4Object. \n"
#define M4_DMS_INIT_ENV_ERROR					0x0005 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al crear el manager de los Meta4Object. \n"
#define M4_DMS_INIT_CHANNEL_MANAGER				0x0006 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al inicializar LDB. \n"
#define	M4_DMS_INIT_LDB_ERROR					0x0007 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al intentar crear el Meta4Object %0:s.\n \n No es posible continuar la construcción de las estructuras necesarias para el proceso de importación/exportación XML.\n"
#define	M4_DMS_CREATE_CHANNEL_ERROR				0x0008 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al cargar los datos del Meta4Object %0:s.\n No es posible continuar la construcción de las estructuras necesarias para el proceso de importación/exportación XML. \n"
#define	M4_DMS_BUILD_CHANNEL_ERROR				0x0009 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al intentar crear el acceso al Meta4Object %0:s.\n \n No es posible continuar la construcción de las estructuras necesarias para el proceso de importación/exportación XML. \n"
#define M4_DMS_CREATE_ACCESS_ERROR				0x000A +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al intentar cargar el Meta4Object %0:s.\n \n No es posible continuar la construcción de las estructuras necesarias para el proceso de importación/exportación XML. \n"
#define M4_DMS_LOAD_CHANNEL_ERROR				0x000B +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al inicializar log file.\n"
#define M4_DMS_INIT_LOG_ERROR					0x000C +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al intentar destruir el manager de los Meta4Object.\n"
#define M4_DMS_DESTROY_CHANNEL_MANAGER_ERROR	0x000D +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al intentar destruir el entorno del Meta4Object Manager.\n"
#define M4_DMS_DESTROY_ENV_ERROR				0x000E +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error de tipo en el elemento %0:s. Tipo no reconocido.\n"
#define M4_DMS_ITEM_TYPE_ERROR					0x000F +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al intentar modificar el valor del elemento %0:s en la base de datos.\n"
#define M4_DMS_SET_ITEM_ERROR					0x0010 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al obtener la información de mapeo XML-Meta4Object para el elemento XML %0:s.\n"
#define M4_DMS_GET_XML_MAP_INFO					0x0011 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Tipo de elemento XML %0:s no identificado como nodo ni como item del Meta4object de importación\n."
#define M4_DMS_FIND_NODE_ERROR					0x0012 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error accediendo al nodo %0:s del Meta4object en el que ser realiza el proceso de importación/exportación de datos XML.\n \n Compruebe que el nodo existe en el Meta4object dentro de su usuario de base de datos.\n"
#define M4_DMS_CREATE_NODE_ERROR				0x0013 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error añadiendo un registro en el nodo %0:s del Meta4object en el que ser realiza el proceso de importación de datos XML.\n \n Compruebe que el nodo existe en el Meta4object dentro de su usuario de base de datos.\n"
#define M4_DMS_ADD_REGISTER_ERROR				0x0014 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al obtener el nombre del Meta4Object.\n \n Cuando queremos obtener el nombre del Meta4Object la cadena que nos devuelven es vacía.\n\n No podemos identificar el Meta4Object donde se produce el error.\n"
#define M4_DMS_GET_NAME_M4O_ERROR				0x0015 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al obtener la información de mapeo XML-M4Object para el elemento raíz del Meta4Object en el que se realiza el proceso de importación/exportación de datos XML.\n"
#define M4_DMS_GET_ROOT_ELEMENT_INFO_ERROR		0x0016 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al obtener la información de mapeo XML-M4Object para el elemento base especificado para el proceso de importación/exportación de datos XML.\n"
#define M4_DMS_GET_BASE_ELEMENT_INFO_ERROR		0x0017 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error inicializando SAX.\n \nEl procesador SAX no ha sido inicializado correctamente.\nDebe verificar la instalación correcta del procesador SAX. La causa del error es: .\n"
#define M4_DMS_XML_SAX_INIT_ERROR				0x0018 + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error inicializando estructuras necesarias para el proceso de importación/exportación XML. El proceso de importación/exportación no continuará.\n"
#define M4_DMS_INIT_DATA_ERROR					0x0019 + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error cargando los datos necesarios para el proceso de importación/exportación XML. El proceso de importación/exportación no continuará.\n"
#define M4_DMS_LOAD_XML_MAP_M4O_ERROR			0x001A + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al destruir el acceso a Meta4Object.\n \n Cuando destruimos el acceso al Meta4Object %0:s se ha producido un error.\n"
#define M4_DMS_DESTROY_ACCESS_ERROR				0x001B + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al destruir el Meta4Object.\n \nCuando destruimos el Meta4Object %0:s se ha producido un error.\n"
#define M4_DMS_DESTROY_CHANNEL_ERROR			0x001C + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al almacenar un dato de la pila para el método %0:s en el Meta4Object %1:s.\n"
#define M4_DMS_PUSH_STACK_ERROR					0x001D + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al ejecutar el método LN4 %0:s del Meta4Object %1:s.\n \n Compruebe la causa del error o si la sintaxis del método es correcta.\n"
#define M4_DMS_EXECUTE_METHOD_ERROR				0x001E + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al obtener un valor en la pila para el método LN4 %0:s del Meta4Objects %1:s se ha producido un error.\n"
#define M4_DMS_POP_STACK_ERROR					0x001F + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al copiar el fichero DTD en el directorio en el que se encuentra el fichero XML a ser importado. Este error ocurrió al ejecutar el método LN4 %0:s del Meta4Object %1:s.\n"
#define M4_DMS_COPY_DTD_FILE_ERROR				0x0020 + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al obtener el nombre del fichero DTD especificado para proceso de importación/exportación de datos XML.\n"
#define M4_DMS_GET_DTD_FILE_NAME				0x0021 + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al acceder al fichero %0:s, especificado para realizar el volcado de los datos del Meta4Object de exportación. Compruebe que la ruta de este fichero es correcta.\n"
#define M4_DMS_OPEN_FILE_ERROR					0x0022 + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error en el proceso de exportación XML. Se ha producido un error al intentar importar el fichero XML %0:s.\nDebe consultar con su administrador.\n"
#define M4_DMS_START_EXPORT_PROCESS_ERROR		0x0023 + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al procesar elemento %0:s del Meta4Object de exportación de datos\n."
#define M4_DMS_PROCESS_ELEM_ERROR				0x0024 + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al intentar recuperar el valor de los atributos del elemento %0:s del Meta4Object de exportación de datos\n."
#define M4_DMS_BUILD_ATT_VALUE					0x0025 + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al cargar los subelementos del elemento %0:s del Meta4Object de exportación de datos\n."
#define M4_DMS_LOAD_ELEM_SUBELEMENT				0x0026 + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al recuperar el siguiente subelemento del elemento %0:s del Meta4Object de exportación de datos\n."
#define M4_DMS_GET_SUBELEM_ERROR				0x0027 + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al recuperar el valor almacenado en el elemento %0:s del nodo %1:s que se encuentra en el Meta4Object de exportación de datos.\n"
#define M4_DMS_GET_ITEM_ERROR					0x0028 + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al cargar los atributos del elemento %0:s del Meta4Object de exportación de datos\n."
#define	M4_DMS_LOAD_ELEMENT_ATT					0x0029 + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al recuperar el siguiente atributo del elemento %0:s del Meta4Object de exportación de datos\n."
#define	M4_DMS_GET_ELEMENT_ATT					0x002A + M4_SRV_DMS_IMPORT_XML_ERROR
// "El valor obtenido para el atributo %0:s no está bien formado atendiendo al formato XML. Este valor no debe contener simultáneamente dobles comillas y simple comillas\n."
#define M4_DMS_ATT_VALUE_ERROR					0x002B + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error verificando los argumentos de entrada del proceso de importación/exportación. Revise que estos argumentos son correctos.\n"
#define M4_DMS_BAD_ARG							0x002C + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al intentar acceder al Meta4Object %0:s donde se encuentran los datos a exportar. Verifique que este Meta4Object se encuentra ya cargado.\n"
#define M4_DMS_GET_BUILT_CHANNEL_ERROR			0x002D + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al intentar obtener la precisión/escala del item %0:s que se encuentra en el nodo %1:s del Meta4Object del proceso de exportación/importación.\n"
#define M4_DMS_GET_ITEM_PREC_SCALE_ERROR		0x002E + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al intentar obtener el tipo meta4 del item %0:s que se encuentra en el nodo %1:s del Meta4Object del proceso de exportación.\n"
#define M4_DMS_GET_ITEM_M4TYPE_ERROR			0x002F + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error en el proceso de validación del elemento XML %0:s. Su valor, %1:s, no se corresponde con el tipo de valor esperado o su precisión/escala no es correcta, atendiendo a la definición del item .\n".
#define M4_DMS_CONVERT_ITEM_VALUE_ERROR			0x0030 + M4_SRV_DMS_IMPORT_XML_ERROR
// "Error procesando atributos.\nSe ha producido un error mientras se procesaban los atributos del elemento %0:s.\nDebe consultar con el administrador.\n"
#define M4_DMS_PROCESS_ATTRIBUTES_ERROR			0x0031 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error procesando elemento.\nSe ha producido un error mientras se procesaba el elemento XML %0:s de tipo %1:d.\nDebe consultar con el administrador.\n"
#define M4_DMS_PROCESS_ELEMENT_ERROR			0x0032 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error estableciendo mapeos por defectos para el Meta4Object %0:s durante el proceso de importación/exportación XML. El proceso de importación/exportación no continuará.\nDebe consultar con el administrador.\n"
#define M4_DMS_SET_DEFAULT_MAPPINGS_ERROR		0x0033 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Se ha detectado proceso de carga parcial. Para la carga parcial el usuario debe no puede establecer mapeos por defecto. \nDebe revisar su parametrización.\n"
#define M4_DMS_PARTIAL_LOAD_ERROR				0x0034 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error intentando averiguar el ámbito de los items del nodo %0:s durante el proceso de importación/exportación XML.\nDebe consultar con su administrador.\n"
#define M4_DMS_CHECK_ITEMS_SCOPE				0x0035 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error en el proceso de importación XML. Se ha producido un error en el proceso de importación XML\nConsultar otros errores.\n"
#define M4_DMS_IMPORT_XML_PROCESS_ERROR			0x0036 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error obteniendo el M4Object a partir de su handle. Se ha producido un error al intentar recuperar un M4Object a partir de su handle.\nDebe consultar con su administrador.\n"
#define M4_DMS_GET_CHANNEL_BY_HANDLE_ERROR		0x0037 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error obteniendo el nombre del M4Object. Se ha producido un error al intentar obtener el nombre de un M4Object.\nConsultar otros errores.\n"
#define M4_DMS_GET_CHANNEL_NAME_ERROR			0x0038 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error obteniendo la información de validación para XML Schemas. Se ha producido un error al intentar obtener la información necesaria para la validación contra un Schema XML.\nConsultar con su administrador.\n"
#define M4_DMS_BUILD_XML_SCHEMA_INFO			0x0039 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error obteniendo el ámbito de un item. Se ha producido un error al intentar obtener el ámbito del item %0:s del nodo %1:s.\nConsultar con su administrador.\n"
#define M4_DMS_GET_ITEM_SCOPE_ERROR				0x003A +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al intentar procesar un registro de un nodo. Se ha producido un error al intentar procesar el registro %0:d del nodo %1:s.\nConsultar con su administrador.\n"
#define M4_DMS_PROCESS_REGISTER_ERROR			0x003B +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error al verificar la consistencia de los nodos mapeados. Se ha producido un error al intentar verificar la consitencia del mapeo de nodos.\nConsultar con su administrador.\n"
#define M4_DMS_CHECK_NODES_CONSISTENCY_ERROR	0x003C +  M4_SRV_DMS_IMPORT_XML_ERROR

// 7868061="El item"%0:s" del nodo "%1:s" será ignorado porque no tiene definida la propiedad "Atributo de". Esta propiedad es obligatoria al existir un item marcado como valor del nodo. \nRevise la correspondencia definida.\n"
// 7868062="El item "%0:s" del nodo "%1:s" está definido simultáneamente como atributo y valor del nodo. Será considerado como atributo ignorando su condición de valor de nodo. \nRevise la correspondencia definida para estos elementos.\n"
// 7868063="El item"%0:s" del nodo "%1:s" será ignorado porque sólo se permite tener un item que sea valor de nodo.\nRevise la correspondencia definida.\n"

// "Error ejecutar el método de preprocesamiento. Se ha producido un error al ejecutar el método de preprocesamiento %0:s del nodo %1:s.\n"
#define M4_DMS_EXECUTE_PREPROCESS_METHOD_ERROR	0x0040 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error ejecutar el método de postprocesamiento. Se ha producido un error al ejecutar el método de postprocesamiento %0:s del nodo %1:s.\n"
#define M4_DMS_EXECUTE_POSTPROCESS_METHOD_ERROR	0x0041 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Proceso de exportación abortado por el usuario.\n"
#define M4_DMS_XML_ABORT_PROCESS				0x0042 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error borrando un registro en el nodo %0:s.\n"
#define M4_DMS_DELETE_REGISTER_ERROR			0x0043 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error cambiando el estado de un registro del nodo %0:s.\n"
#define M4_DMS_CHECK_POINT_REGISTER_ERROR		0x0044 +  M4_SRV_DMS_IMPORT_XML_ERROR

// "La longitud máxima del elemento XML %0:s ha sido superada. Debe ser menor o igual que %1:d.\n"
#define M4_DMS_BAD_STRING_PRECISION_ERROR		0x0045 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "La fecha introducida para el elemento XML %0:s no puede tener horas, minutos y segundos.\n" 
#define M4_DMS_BAD_SHORT_DATE_ERROR				0x0046 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "La fecha introducida para el elemento XML %0:s está fuera de rango. Debe ser un valor entre %1:s y %2:s.\n" 
#define M4_DMS_OUT_OF_RANGE_DATE_ERROR			0x0047 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "La longitud de la parte entera del elemento XML %0:s ha sido superada. Debe ser menor o igual que %1:d.\n"
#define M4_DMS_BAD_NUMBER_PRECISION_ERROR		0x0048 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "La longitud de la parte decimal del elemento XML %0:s ha sido superada. Debe ser menor o igual que %1:d.\n"
#define M4_DMS_BAD_NUMBER_SCALE_ERROR			0x0049 +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error obteniendo el handle de un nodo. Se ha producido un error al intentar obtener el handle del nodo %0:s.\nConsultar con su administrador.\n"
#define M4_DMS_GET_NODE_HANDLE_ERROR			0x004A +  M4_SRV_DMS_IMPORT_XML_ERROR
// "Error obteniendo el hadle de un item. Se ha producido un error al intentar obtener el handle del item %0:s del nodo %1:s.\nConsultar con su administrador.\n"
#define M4_DMS_GET_ITEM_HANDLE_ERROR			0x004B +  M4_SRV_DMS_IMPORT_XML_ERROR

// "Número de argumentos incorrecto.\n \nError verificando los argumentos de entrada del proceso de transformación XSL. Se ha llamado a la función "%0:s" con "%1:n" argumentos y debería llamarse con "%2:n" argumentos."
#define M4JDMS_ERROR_XSL_BAD_NUMBER_ARGUMENTS	0x004C + M4_SRV_DMS_IMPORT_XML_ERROR
// "Tipo de argumento incorrecto.\n \nEl argumento "%0:s" de la funcion "%1:s" es de tipo "%2:s" y debería ser de tipo "%3:s"."
#define M4JDMS_ERROR_XSL_BAD_ARGUMENT_TYPE		0x004D + M4_SRV_DMS_IMPORT_XML_ERROR
// "Argumento vacio.\n \nEl argumento "%0:s" de la funcion "%1:s" contiene una cadena vacia.\nDebe introducir un valor válido."
#define M4JDMS_ERROR_XSL_NULL_ARGUMENT			0x004E + M4_SRV_DMS_IMPORT_XML_ERROR
// "No se ha podido realizar la transformación XSL.\n \nSe ha producido el error "%0:s" al realizar la transformación XSL.\nDebe consultar con el administrador."
#define M4JDMS_ERROR_XSL_TRANSFORMATION_ERROR	0x004F + M4_SRV_DMS_IMPORT_XML_ERROR


#endif

