#ifndef _LDBRES_HPP_
#define _LDBRES_HPP_

#include "m4res.hpp"


//''''''''''''''''''''''''''''''
//' Begin ACCESS MESSAGES
//' 0x00030000
#define M4_CH_ACCESS_UNAUTHORIZED_SENTENCE					0x0000 + M4_ACCESS_ERROR_BASE
//196608="Sentencia no autorizada.\n \n%0:s.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_ACCESS_UNAUTHORIZED_NO_RECOVER_ORG_USE		0x0001 + M4_ACCESS_ERROR_BASE
//196609="Uso no autorizado no recuperable de la organizaci�n.\n \n%0:s.\nSe est� utilizando la columna organizaci�n "%1:s" de la tabla "%2:s" sin restricci�n por permisos.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_ACCESS_UNAUTHORIZED_RECOVER_ORG_USE			0x0002 + M4_ACCESS_ERROR_BASE
//196610="Uso no autorizado de la organizaci�n.\n \n%0:s.\nSe est� utilizando la columna organizaci�n "%1:s" de la tabla "%2:s" sin restricci�n por permisos.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_ACCESS_UNAUTHORIZED_RECOVERED_ORG_USE			0x0003 + M4_ACCESS_ERROR_BASE
//196611="Uso no autorizado de la organizaci�n recuperado.\n \n%0:s.\nSe ha a�adido un filtro a la columna organizaci�n "%1:s" de la tabla "%2:s" para restringir a las sociedades v�lidas para el usuario.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_AUDIT_TIME_EXCEEDED							0x0004 + M4_ACCESS_ERROR_BASE
//196612="Tiempo excedido.\n \n%0:s.\nEsta sentencia ha tardado "%1:s" segundos, superando el l�mite establecido de "%2:s" segundos.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_AUDIT_ROWS_EXCEEDED							0x0005 + M4_ACCESS_ERROR_BASE
//196613="Filas excedidas.\n \n%0:s.\nEsta sentencia ha traido "%1:s" filas, superando el l�mite establecido de "%2:s" filas.\nEsto sucedi� en "%3:s" de "%4:s"."




//' Begin LDB Errors
//''''''''''''''''''''''''''''''
//' LDB design Errors
//' 0x130000

#define M4_CH_LDB_ERROR									0x0000 + M4_CH_LDB_ERROR_BASE
//1245184="Error de base de datos l�gica.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_INTERNAL_ERROR						0x0001 + M4_CH_LDB_ERROR_BASE
//1245185="Error interno.\n \nError interno en la funci�n "%0:s": "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_FUNCTION_NOT_AVAILABLE				0x0002 + M4_CH_LDB_ERROR_BASE
//1245186="Funci�n no disponible.\n \nNo est� disponible la funci�n "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_DBWRAP_DRIVER_ERROR					0x0003 + M4_CH_LDB_ERROR_BASE
//1245187="Error de DBWrap.\n \nError imprevisto en el controlador DBWrap."
#define M4_CH_LDB_DDBB_ERROR							0x0004 + M4_CH_LDB_ERROR_BASE
//1245188="Error de base de datos.\n \nError ["%0:s"] en base de datos: "%1:s".\nEsto sucedi� en "%6:s" de "%7:s"."
#define M4_CH_LDB_DDBB_WARNING							0x0005 + M4_CH_LDB_ERROR_BASE
//1245189="Advertencia de base de datos.\n \nAdvertencia ["%0:s"] en base de datos: "%1:s".\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_WRONG_LOGIC_OBJECT					0x0006 + M4_CH_LDB_ERROR_BASE
//1245190="Tabla no encontrada.\n \nNo existe la tabla "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_WRONG_LOGIC_FIELD 					0x0007 + M4_CH_LDB_ERROR_BASE
//1245191="Columna no encontrada.\n \nNo se encuentra la columna "%0:s" en la tabla "%1:s" ni entre sus columnas traducidas.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_LOGIC_FIELD_NOT_FOUND					0x0008 + M4_CH_LDB_ERROR_BASE
//1245192="Columna no encontrada.\n \nNo se encuentra la columna interna "%0:s" en la tabla "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_TRANS_LOGIC_FIELD_NOT_FOUND			0x0009 + M4_CH_LDB_ERROR_BASE
//1245193="Columna no encontrada.\n \nNo se encuentra la columna traducida de la columna "%0:s" en la tabla "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_INVALID_TRANSLATION					0x000A + M4_CH_LDB_ERROR_BASE
//1245194="Traducci�n incorrecta.\n \nNo se puede traducir la columna "%0:s" de la tabla "%1:s" porque su columna traducida tiene el mismo nombre.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_NO_REAL_OBJECTS						0x000B + M4_CH_LDB_ERROR_BASE
//1245195="Tabla f�sica no definida.\n \nLa tabla "%0:s" no tiene tablas f�sicas definidas.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_NO_LOGIC_FIELDS						0x000C + M4_CH_LDB_ERROR_BASE
//1245196="Columnas no definidas.\n \nLa tabla "%0:s" no tiene columnas definidas en la tabla f�sica "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_NO_TRANS_FIELDS						0x000D + M4_CH_LDB_ERROR_BASE
//1245197="Columnas traducidas no definidas.\n \nLa tabla "%0:s" no tiene columnas traducidas definidas para la columna "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_NO_RELATION_FIELDS					0x000E + M4_CH_LDB_ERROR_BASE
//1245198="Columnas de relaci�n no definidas.\n \nLa tabla "%0:s" no tiene columnas definidas para la relaci�n "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_NOT_USED_1							0x000F + M4_CH_LDB_ERROR_BASE
// No se usa.
#define M4_CH_LDB_NO_PK_FIELDS							0x0010 + M4_CH_LDB_ERROR_BASE
//1245200="Columnas de clave primaria no definidas\n \nLa tabla "%0:s" no tiene columnas de clave primaria (CP) definidas o las definidas son una columna ordinal o una columna fecha inicio.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_EXTENDED_TYPE_NOT_FOUND				0x0011 + M4_CH_LDB_ERROR_BASE
//1245201="Tipo extendido no encontrado.\n \nNo se encuentra el tipo extendido "%0:s" de la columna "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_DEFAULT_FUNCTION_NOT_FOUND			0x0012 + M4_CH_LDB_ERROR_BASE
//1245202="Funci�n por defecto no encontrada.\n \nNo se encontr� la funci�n por defecto "%0:s" de la columna "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_CONVERT_FUNCTION_NOT_FOUND			0x0013 + M4_CH_LDB_ERROR_BASE
//1245203="Funci�n de conversi�n no encontrada.\n \nNo se encontr� la funci�n de conversi�n "%0:s" de la columna "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_CONSTRAINT_FUNCTION_NOT_FOUND			0x0014 + M4_CH_LDB_ERROR_BASE
//1245204="Funci�n de restricci�n no encontrada.\n \nNo se encontr� la funci�n de restricci�n "%0:s" de la columna "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_INVALID_ARGUMENT						0x0015 + M4_CH_LDB_ERROR_BASE
//1245205="Argumento no v�lido.\n \nArgumento no v�lido "%0:s" en la funci�n de restricci�n "%1:s" de la columna "%2:s".\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_INVALID_AUDITORY_LOGIC_FIELD			0x0016 + M4_CH_LDB_ERROR_BASE
//1245206="Columna no v�lida.\n \nLa columna "%0:s" de la tabla de auditor�a "%1:s" no es v�lida.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_AUDITORY_LOGIC_OBJECT_NOT_DEFINED		0x0017 + M4_CH_LDB_ERROR_BASE
//1245207="Tabla de auditor�a no definida.\n \nLa tabla "%0:s" no tiene tabla de auditor�a definida.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_BAD_BLOB_FILE							0x0018 + M4_CH_LDB_ERROR_BASE
//1245208="Archivo binario incorrecto.\n \nArchivo binario incorrecto en la columna "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_ITEM_IS_METHOD						0x0019 + M4_CH_LDB_ERROR_BASE
//1245209="M�todo con columna asociada.\n \nEl elemento "%0:s" es un m�todo y lleva asociada la columna "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_NO_REGISTER_ITEM						0x001A + M4_CH_LDB_ERROR_BASE
//1245210="Sentencia de lectura sin columnas.\n \nNo hay elementos de registro con columnas de lectura asociadas.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_INVALID_JOIN							0x001B + M4_CH_LDB_ERROR_BASE
//1245211="Uni�n no v�lida.\n \nUni�n no v�lida entre tablas f�sicas con conexiones diferentes para las tablas "%0:s" y "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_BAD_LOGIC_FUNCTION					0x001C + M4_CH_LDB_ERROR_BASE
//1245212="Funci�n l�gica incorrecta.\n \nLa funci�n l�gica "%0:s" es incorrecta.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_WRONG_LOGIC_FUNCTION					0x001D + M4_CH_LDB_ERROR_BASE
//1245213="Funci�n l�gica no definida.\n \nNo se ha definido la funci�n l�gica "%0:s" en la base de datos "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_WRONG_ALIAS							0x001E + M4_CH_LDB_ERROR_BASE
//1245214="Alias sin tabla.\n \nAlias "%0:s" sin tabla.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_NOT_USED_2							0x001F + M4_CH_LDB_ERROR_BASE
// No se usa.
#define M4_CH_LDB_WRONG_ITEM							0x0020 + M4_CH_LDB_ERROR_BASE
//1245216="Elemento no definido.\n \nNo se ha definido el elemento "%0:s" en la estructura de nodo ni en la estructura de nodo de sesi�n.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_WRONG_LOGIC_FIELD_NUMBER				0x0021 + M4_CH_LDB_ERROR_BASE
//1245217="N�mero de columnas incorrecto.\n \nEl n�mero de columnas es distinto al n�mero de valores.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_WRONG_APISQL							0x0022 + M4_CH_LDB_ERROR_BASE
//1245218="Sentencia API SQL incorrecta.\n \nSentencia API SQL incorrecta "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_SESSION_NOT_FOUND						0x0023 + M4_CH_LDB_ERROR_BASE
//1245219="Meta4Object Sesi�n no encontrado.\n \nNo se encuentra el Meta4Object Sesi�n.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD			0x0024 + M4_CH_LDB_ERROR_BASE
//1245220="Car�cter o palabra clave no encontrado.\n \nFalta car�cter o palabra clave "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_MISSING_LOGIC_FIELD					0x0025 + M4_CH_LDB_ERROR_BASE
//1245221="Columna no encontrada.\n \nFalta columna para el elemento "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_MISSING_ITEM							0x0026 + M4_CH_LDB_ERROR_BASE
//1245222="Elemento no encontrado.\n \nFalta elemento en la columna "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_NOT_USED_3							0x0027 + M4_CH_LDB_ERROR_BASE
// No se usa.
#define M4_CH_LDB_MISSING_LOGIC_OBJECT					0x0028 + M4_CH_LDB_ERROR_BASE
//1245224="Tabla no encontrada.\n \nFalta tabla.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_MISSING_BASIS_LOGIC_OBJECT			0x0029 + M4_CH_LDB_ERROR_BASE
//1245225="Tabla base no encontrada.\n \nFalta tabla base.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_APISQL_SYNTAX_ERROR					0x002A + M4_CH_LDB_ERROR_BASE
//1245226="Error de sintaxis.\n \nError de sintaxis localizado en la posici�n "%0:s" de la sentencia API SQL "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_BLOB_WITHOUT_EXTENSION				0x002B + M4_CH_LDB_ERROR_BASE
//1245227="Archivo binario sin extensi�n.\n \nArchivo binario sin extensi�n en la columna "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_WRONG_ARGUMENT_NUMBER					0x002C + M4_CH_LDB_ERROR_BASE
//1245228="N�mero incorrecto de argumentos externos.\n \nN�mero incorrecto de argumentos externos.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_CURRENT_IS_EOF						0x002D + M4_CH_LDB_ERROR_BASE
//1245229="Registro actual en fin de archivo.\n \nEl registro actual se encuentra en fin de archivo.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_TRANSACTION_IS_NEEDED					0x002E + M4_CH_LDB_ERROR_BASE
//1245230="Transacci�n requerida.\n \nLa operaci�n de almacenamiento necesita ejecutarse dentro de una transacci�n.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_M4TYPE_MISMATCH						0x002F + M4_CH_LDB_ERROR_BASE
//1245231="Equiparaci�n incorrecta.\n \nEquiparaci�n incorrecta entre los tipos b�sicos de la columna "%0:s" y del elemento "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_PRECISION_MISMATCH					0x0030 + M4_CH_LDB_ERROR_BASE
//1245232="Equiparaci�n incorrecta.\n \nEquiparaci�n incorrecta entre las precisiones de la columna "%0:s" y del elemento "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_NO_REGISTERS_AFFECTED					0x0031 + M4_CH_LDB_ERROR_BASE
//1245233="No hay registros afectados\n \nNo hay registros afectados por la operaci�n de almacenamiento.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_LOGIC_OBJECT_NOT_DEFINED				0x0032 + M4_CH_LDB_ERROR_BASE
//1245234="Tabla no definida.\n \nTabla no definida.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_INVALID_CONNECTION					0x0033 + M4_CH_LDB_ERROR_BASE
//1245235="ID Conexi�n incorrecto\n \nEl ID Conexi�n "%0:s" es incorrecto.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_NO_FIELDS_FOR_UPDATE					0x0034 + M4_CH_LDB_ERROR_BASE
//1245236="No hay columnas que actualizar.\n \nNo hay columnas que actualizar en la tabla "%0:s" para la tabla f�sica "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_NOT_USED_4							0x0035 + M4_CH_LDB_ERROR_BASE
// No se usa.
#define M4_CH_LDB_NOT_USED_5							0x0036 + M4_CH_LDB_ERROR_BASE
// No se usa.
#define M4_CH_LDB_NOT_USED_6							0x0037 + M4_CH_LDB_ERROR_BASE
// No se usa.
#define M4_CH_LDB_NOT_USED_7							0x0038 + M4_CH_LDB_ERROR_BASE
// No se usa.
#define M4_CH_LDB_ENCRYPTING_DATA						0x0039 + M4_CH_LDB_ERROR_BASE
//1245241="Error al encriptar datos.\n \nError al encriptar datos "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_DECRYPTING_DATA						0x003A + M4_CH_LDB_ERROR_BASE
//1245242="Error al desencriptar datos.\n \nError al desencriptar datos "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_INVALID_SERVER_NAME					0x003B + M4_CH_LDB_ERROR_BASE
//1245243="Nombre de servidor incorrecto.\n \nNombre de servidor incorrecto.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_INVALID_USER_NAME						0x003C + M4_CH_LDB_ERROR_BASE
//1245244="Nombre de usuario incorrecto.\n \nNombre de usuario incorrecto\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_INVALID_PASSWORD						0x003D + M4_CH_LDB_ERROR_BASE
//1245245="Contrase�a incorrecta.\n \nContrase�a incorrecta\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_DDBB_DISABLED							0x003E + M4_CH_LDB_ERROR_BASE
//1245246="Base de datos no operativa.\n \nBase de datos no operativa.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_AMBIGOUS_COLUMN_REFERENCE				0x003F + M4_CH_LDB_ERROR_BASE
//1245247="Columna encontrada en varias tablas.\n \nLa columna "%0:s" ha sido encontrada en las tablas "%1:s" y "%2:s".\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_FIELD_NOT_FOUND						0x0040 + M4_CH_LDB_ERROR_BASE
//1245248="Columna no encontrada.\n \nLa columna "%0:s" no ha sido encontrada en ninguna tabla.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_CONN_TIMEOUT							0x0041 + M4_CH_LDB_ERROR_BASE
//1245249="Conexi�n no obtenida.\n \nHa expirado el tiempo permitido para la obtenci�n de una conexi�n."
#define M4_CH_LDB_NOT_USED_8							0x0042 + M4_CH_LDB_ERROR_BASE
// No se usa.
#define M4_CH_LDB_NOT_USED_9							0x0043 + M4_CH_LDB_ERROR_BASE
// No se usa.
#define M4_CH_LDB_OUTER_JOIN_NO_ALLOWED					0x0044 + M4_CH_LDB_ERROR_BASE
//1245252="Operador de uni�n externa no permitido\n \nEl operador de uni�n externa tiene que estar asociado a una columna.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_DUPLICATED_ALIAS						0x0045 + M4_CH_LDB_ERROR_BASE
//1245253="Alias duplicado.\n \nAlias "%0:s" duplicado en las tablas "%1:s" y "%2:s".\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_AMBIGOUS_ALIAS						0x0046 + M4_CH_LDB_ERROR_BASE
//1245254="Tabla ambigua.\n \nLa tabla "%0:s" est� siendo empleada dos veces sin definir alias.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_CIRCULAR_REFERENCE_IN_OUTER_JOINS		0x0047 + M4_CH_LDB_ERROR_BASE
//1245255="Problemas con las uniones externas.\n \nSe ha encontrado una referencia circular con uniones externas o bien se ha detectado que dos tablas dominan a una tercera.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_ERROR_TRANSLATING_TABLE_REL			0x0048 + M4_CH_LDB_ERROR_BASE
//1245256="Error al traducir relaciones.\n \nError al traducir relaciones entre tablas.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_DDBB_ERROR_II							0x0049 + M4_CH_LDB_ERROR_BASE
//1245257="Error de base de datos.\n \nError ["%0:s"] en base de datos: "%1:s"."
#define M4_CH_LDB_DDBB_WARNING_II						0x004A + M4_CH_LDB_ERROR_BASE
//1245258="Advertencia de base de datos.\n \nAdvertencia ["%0:s"] en base de datos: "%1:s"."
#define M4_CH_LDB_OUTER_JOIN_TO_INNER_JOIN_I			0x004B + M4_CH_LDB_ERROR_BASE
//1245259="Conversi�n autom�tica de la uni�n.\n \nLa uni�n externa del objeto "%0:s" se convierte a interna porque la columna "%1:s" no est� definida como uni�n externa.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_OUTER_JOIN_TO_INNER_JOIN_II			0x004C + M4_CH_LDB_ERROR_BASE
//1245260="Conversi�n autom�tica de la uni�n.\n \nLa uni�n externa del objeto "%0:s" se convierte a interna porque hay una uni�n interna sobre �l.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_OUTER_JOIN_TO_INNER_JOIN_III			0x004D + M4_CH_LDB_ERROR_BASE
//1245261="Conversi�n autom�tica de la uni�n.\n \nLa uni�n externa del objeto "%0:s" se convierte a interna por la conversi�n autom�tica del objeto "1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_NOT_USED_10							0x004E + M4_CH_LDB_ERROR_BASE
// No se usa.
#define M4_CH_LDB_MISSING_ITEM_IN_ORDER_BY				0x004F + M4_CH_LDB_ERROR_BASE
//1245263="No hay elemento en Order By.\n \nNo hay columna en un filtro de tipo Order By.\nEsto sucedi� en "%0:s" de "%1:s"."


//' LDB User Errors
//' 0x130050
#define M4_CH_LDB_NULL_PARAMETER_VALUE					0x0050+ M4_CH_LDB_ERROR_BASE
//1245264="Datos obligatorios requeridos.\n \nNo se puede leer porque la columna "%0:s" est� vac�a.\nDebe rellenar las columnas obligatorias.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_CONCURRENCY_USERS						0x0051 + M4_CH_LDB_ERROR_BASE
//1245265="Registro bloqueado.\n \nNo se puede actualizar el registro "%0:f" de "%1:s" porque ha sido modificado ya por otro usuario.\nDebe hacer clic en el bot�n Recargar.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_NULL_PK_VALUE							0x0052 + M4_CH_LDB_ERROR_BASE
//1245266="Datos obligatorios requeridos.\n \nNo se puede insertar, actualizar ni borrar el registro de "%0:s" porque la columna obligatoria "%1:s" est� vac�a.\nDebe rellenar las columnas obligatorias.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_INVALID_DATE							0x0053 + M4_CH_LDB_ERROR_BASE
//1245267="Fecha fuera de l�mites.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque la fecha "%2:d" es anterior a la fecha m�nima permitida o posterior a la fecha m�xima permitida.\nDebe introducir una fecha correcta.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_INTEGRITY_CONSTRAINT_VIOLATION		0x0054 + M4_CH_LDB_ERROR_BASE
//1245268="Datos no permitidos.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque los datos introducidos en la columna "%2:s" no se encuentran dentro de los posibles valores que puede tomar "%3:s".\nDebe introducir unos valores correctos.\nEsto sucedi� en "%4:s" de "%5:s"."
#define M4_CH_LDB_NOT_NULL_CONSTRAINT_VIOLATION			0x0055 + M4_CH_LDB_ERROR_BASE
//1245269="Datos obligatorios requeridos.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque la columna obligatoria "%2:s" est� vac�a.\nDebe rellenar la columna obligatoria.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_PRIMARY_KEY_NOT_FOUND					0x0056 + M4_CH_LDB_ERROR_BASE
//1245270="No existe el registro.\n \nNo se puede actualizar ni borrar el registro "%0:f" de "%1:s" porque no existe.\nCompruebe que ning�n otro proceso o usuario haya borrado el registro y que no se est� modificando un campo de la clave primaria, y haga clic en el bot�n Recargar.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_PRIMARY_KEY_CONSTRAINT_VIOLATION		0x0057 + M4_CH_LDB_ERROR_BASE
//1245271="El registro ya existe.\n \nNo se puede insertar el registro "%0:f" de "%1:s" porque ya existe.\nDebe cambiar el ID registro.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_FOREIGN_CONSTRAINT_VIOLATION			0x0058 + M4_CH_LDB_ERROR_BASE
//1245272="Datos no v�lidos.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque los datos introducidos en "%2:s" no cumplen los requisitos de "%3:s".\nDebe cambiar el ID Registro.\nEsto sucedi� en "%4:s" de "%5:s"."
#define M4_CH_LDB_REFERENTIAL_CONSTRAINT_VIOLATION		0x0059 + M4_CH_LDB_ERROR_BASE
//1245273="Los datos est�n siendo usados.\n \nNo se puede actualizar ni borrar el registro "%0:f" de "%1:s" porque est� siendo utilizado por otro registro de "%3:s".\nDebe hacer clic en el bot�n recargar.\nEsto sucedi� en "%4:s" de "%5:s"."
#define M4_CH_LDB_UNIQUE_INDEX_CONSTRAINT_VIOLATION		0x005A + M4_CH_LDB_ERROR_BASE
//1245274="Datos repetidos.\n \nNo se puede guardar/modificar el registro "%0:f" de "%1:s" porque ya existe otro registro con los mismos valores en las columnas "%2:s".\nDebe introducir los datos correctos.\nEsto sucedi� en "%4:s" de "%5:s"."
#define M4_CH_LDB_WRONG_PERIOD_DATES					0x005B + M4_CH_LDB_ERROR_BASE
//1245275="Fecha de fin incorrecta.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque la columna fecha de fin es anterior a la columna fecha de inicio.\nDebe cambiar valor en la fecha.\nEsto sucedi� en "%4:s" de "%5:s"."
#define M4_CH_LDB_INVALID_FOREIGN_INCLUSION				0x005C + M4_CH_LDB_ERROR_BASE
//1245276="Fecha fuera de periodo v�lido.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque la columna "%2:d" no est� comprendida en el periodo ("%3:d"-"%4:d") de validaci�n "%5:s" con la tabla "%6:s".\nDebe introducir fechas correctas.\nEsto sucedi� en "%7:s" de "%8:s"."
#define M4_CH_LDB_INVALID_REFERENTIAL_INCLUSION			0x005D + M4_CH_LDB_ERROR_BASE
//1245277="Modificaci�n de periodo no v�lido.\n \nNo se puede actualizar el registro "%0:f" de "%1:s" porque ya existe un registro con el periodo ("%3:d"-"%4:d") en la tabla "%6:s" que est� invalidado por el cambio en la columna "%2:d".\nDebe introducir una fecha correcta.\nEsto sucedi� en "%7:s" de "%8:s"."
#define M4_CH_LDB_INVALID_FOREIGN_PERIOD				0x005E + M4_CH_LDB_ERROR_BASE
//1245278="Datos no v�lidos en fechas.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque la fecha "%2:d" o la fecha "%3:d" no est�n comprendidas dentro del periodo ("%4:d"-"%5:d") de validaci�n con la tabla "%7:s".\nDebe introducir fechas correctas.\nEsto sucedi� en "%8:s" de "%9:s"."
#define M4_CH_LDB_INVALID_REFERENTIAL_PERIOD			0x005F + M4_CH_LDB_ERROR_BASE
//1245279="Modificaci�n de periodo no v�lido.\n \nNo se puede actualizar el registro "%0:f" de "%1:s" porque ya existe en "%7:s" un registro con el periodo ("%4:d"-"%5:d") que est� invalidado por el cambio en "%2:d" o "%3:d".\nDebe introducir una fecha correcta.\nEsto sucedi� en "%8:s" de "%9:s"."
#define M4_CH_LDB_REPETITION_CONTROL_PREV_CURR			0x0060 + M4_CH_LDB_ERROR_BASE
//1245280="Datos repetidos.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque ya existen estos valores en las columnas "%2:s" del registro anterior.\nDebe cambiar los valores en las columnas.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_REPETITION_CONTROL_CURR_NEXT			0x0061 + M4_CH_LDB_ERROR_BASE
//1245281="Datos repetidos.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque ya existen estos valores en las columnas "%2:s" del registro posterior.\nDebe cambiar los valores en las columnas.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_REPETITION_CONTROL_PREV_NEXT			0x0062 + M4_CH_LDB_ERROR_BASE
//1245282="Borrado incorrecto.\n \nNo se puede borrar el registro "%0:f" de "%1:s" porque el registro anterior tiene los mismos valores en las columnas "%2:s" que el registro posterior.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_COMPLETENESS_FOREIGN_UNSATISFIED		0x0063 + M4_CH_LDB_ERROR_BASE
//1245283="Fechas fuera de periodo v�lido.\n \nNo se puede insertar el registro "%0:f" de "%1:s" porque la columna "%2:d" debe ser igual a "%4:d" y la columna "%3:d" igual a "%5:d", para poder validar la relaci�n de completitud.\nDebe introducir una fecha correcta.\nEsto sucedi� en "%6:s" de "%7:s"."
#define M4_CH_LDB_COMPLETENESS_UNSATISFIED				0x0064 + M4_CH_LDB_ERROR_BASE
//1245284="Fechas fuera de periodo v�lido.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque la columna "%2:d" debe ser igual a "%4:d" o la columna "%3:d" igual a "%5:d", para poder validar la relaci�n de completitud.\nDebe introducir una fecha correcta.\nEsto sucedi� en "%6:s" de "%7:s"."
#define M4_CH_LDB_INVALID_DELETE_ON_HISTORY				0x0065 + M4_CH_LDB_ERROR_BASE
//1245285="Algunos registros se deben borrar previamente.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque existe al menos un registro completamente comprendido en �l que debe ser borrado.\nDebe introducir periodos correctos.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_OVERLAPED_NO_ALLOWED					0x0066 + M4_CH_LDB_ERROR_BASE
//1245286="Solapamiento no permitido.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque existe otro registro con el cual se solapa en el tiempo.\nDebe cambiar el periodo del registro.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_NO_AUDITORY_DEFINED					0x0067 + M4_CH_LDB_ERROR_BASE
//1245287="Acceso denegado.\n \nNo hay una auditor�a definida en la tabla "%0:s" para el modelo de seguridad basado en roles "%1:s".\nDebe verificar su perfil.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_NO_SECURITY_DEFINED					0x0068 + M4_CH_LDB_ERROR_BASE
//1245288="Acceso denegado.\n \nNo hay una seguridad definida en la tabla "%0:s" para el modelo de seguridad basado en roles.\nDebe solicitar los permisos necesarios.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_SECURITY_CONSTRAINT_VIOLATION			0x0069 + M4_CH_LDB_ERROR_BASE
//1245289="Permisos insuficientes.\n \nNo se puede insertar, actualizar ni borrar el registro "%0:f" de "%1:s" porque �ste pertenece a un grupo de registros sobre el cual no se tienen permisos.\nDebe verificar su perfil.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_READ_ACCESS_REFUSE					0x006A + M4_CH_LDB_ERROR_BASE
//1245290="Acceso de lectura denegado.\n \nAcceso de lectura en la tabla "%0:s" denegado.\nDebe verificar su perfil.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_READ_ACCESS_FIELD_REFUSE				0x006B + M4_CH_LDB_ERROR_BASE
//1245291="Acceso de lectura denegado.\n \nAcceso de lectura a columna "%0:s" de la tabla "%1:s" denegado.\nDebe verificar su perfil.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_WRITE_ACCESS_FIELD_REFUSE				0x006C + M4_CH_LDB_ERROR_BASE
//1245292="Acceso de escritura denegado.\n \nAcceso de escritura a columna "%0:s" de la tabla "%1:s" denegado.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_CHILD_NOT_FOUND						0x006D + M4_CH_LDB_ERROR_BASE
//1245293="Datos no v�lidos.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque no tiene ning�n registro asociado en "%3:s".\nDebe introducir alg�n registro en "%3:s".\nEsto sucedi� en "%4:s" de "%5:s"."
#define M4_CH_LDB_CORR_DATE_SMALLER_THAN_CLOSING_DATE	0x006E + M4_CH_LDB_ERROR_BASE
//1245294="Modificaci�n de datos cerrados.\n \nNo se puede insertar ni actualizar la correcci�n "%0:f" de "%1:s" porque la fecha "%2:d" es anterior que la fecha de cierre.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_DATES_SMALLER_THAN_CLOSING_DATE		0x006F + M4_CH_LDB_ERROR_BASE
//1245295="Modificaci�n de datos cerrados.\n \nNo se puede insertar ni actualizar la correcci�n "%0:f" de "%1:s" porque las fechas "%2:d" y "%3:d" son anteriores que la fecha de cierre.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%4:s" de "%5:s"."
#define M4_CH_LDB_INVALID_DELETE_CORRECTION				0x0070 + M4_CH_LDB_ERROR_BASE
//1245296="Borrado no v�lido.\n \nNo se puede borrar la correcci�n "%0:f" de "%1:s" porque es una proyecci�n o un registro cerrado o bloqueado.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_INVALID_FOREIGN_CORRECTION_PERIOD		0x0071 + M4_CH_LDB_ERROR_BASE
//1245297="Correcci�n no v�lida en fechas.\n \nNo se puede insertar ni actualizar la correcci�n "%0:f" de "%1:s" porque la columna "%2:d" o la columna "%3:d" no se encuentran dentro del periodo ("%4:d"-"%5:d") de validaci�n "%6:s" con la tabla "%7:s".\nDebe introducir los valores correctos.\nEsto sucedi� en "%8:s" de "%9:s"."
#define M4_CH_LDB_INVALID_REFERENTIAL_CORRECTION_PERIOD	0x0072 + M4_CH_LDB_ERROR_BASE
//1245298="Modificaci�n de periodo no v�lido.\n \nNo se puede actualizar la correcci�n "%0:f" de "%1:s" porque ya existe un registro con el periodo ("%3:d"-"%4:d") en la tabla "%6:s" que est� invalidado por el cambio en la columna "%2:d".\nDebe introducir las fechas correctas.\nEsto sucedi� en "%7:s" de "%8:s"."
#define M4_CH_LDB_COMPLETENESS_CORRECTION_UNSATISFIED	0x0073 + M4_CH_LDB_ERROR_BASE
//1245299="Fechas fuera de periodo v�lido.\n \nNo se puede insertar ni actualizar la correcci�n "%0:f" de "%1:s" porque la fecha "%2:d" o la fecha "%3:d" no se encuentran comprendidas dentro del periodo ("%4:d"-"%5:d") de validaci�n "%6:s" con la tabla "%7:s".\nDebe introducir el periodo correcto.\nEsto sucedi� en "%8:s" de "%9:s"."
#define M4_CH_LDB_NO_LATEST_CORRECTION					0x0074 + M4_CH_LDB_ERROR_BASE
//1245300="Hay historias posteriores.\n \nNo se puede insertar, actualizar ni borrar el registro "%0:f" de "%1:s" porque existe una historia posterior.\nDebe borrar o actualizar esta historia.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_INVALID_PROJECTION					0x0075 + M4_CH_LDB_ERROR_BASE
//1245301="La correcci�n genera datos repetidos.\n \nNo se puede insertar, actualizar ni borrar la correcci�n "%0:f" de "%1:s" porque ya existe una proyecci�n con los mismos valores en las columnas "%2:s" de las correcciones anteriores y posteriores.\nDebe cambiar los valores de las columnas.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_INVALID_PREVIOUS_PROJECTION			0x0076 + M4_CH_LDB_ERROR_BASE
//1245302="La correcci�n genera datos repetidos.\n \nNo se puede insertar, actualizar ni borrar la correcci�n "%0:f" de "%1:s" porque ya existe una proyecci�n con los mismos valores en las columnas "%2:s" de la correcci�n anterior.\nDebe cambiar los valores de las columnas.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_INVALID_NEXT_PROJECTION				0x0077 + M4_CH_LDB_ERROR_BASE
//1245303="La correcci�n genera datos repetidos.\n \nNo se puede insertar, actualizar ni borrar la correcci�n "%0:f" de "%1:s" porque ya existe una proyecci�n con los mismos valores en las columnas "%2:s" de la correcci�n posterior.\nDebe cambiar los valores de las columnas.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_INSERT_ACCESS_REFUSE					0x0078 + M4_CH_LDB_ERROR_BASE
//1245304="Acceso de escritura denegado.\n \nAcceso de escritura sobre la tabla "%0:s" denegado.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_UPDATE_ACCESS_REFUSE					0x0079 + M4_CH_LDB_ERROR_BASE
//1245305="Acceso de modificaci�n denegado.\n \nAcceso de modificaci�n sobre la tabla "%0:s" denegado.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_DELETE_ACCESS_REFUSE					0x007A + M4_CH_LDB_ERROR_BASE
//1245306="Acceso de borrado denegado.\n \nAcceso de borrado en la tabla "%0:s" denegado.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_INSERT_CORRECTION_REFUSE				0x007B + M4_CH_LDB_ERROR_BASE
//1245307="Permiso de correcci�n denegado.\n \nNo puede a�adir una nueva correcci�n en la tabla "%0:s" porque no tiene los permisos necesarios.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_UPDATE_CORRECTION_REFUSE				0x007C + M4_CH_LDB_ERROR_BASE
//1245308="Permiso de correcci�n denegado.\n \nNo puede actualizar una correcci�n en la tabla "%0:s" porque no tiene los permisos necesarios.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_DELETE_CORRECTION_REFUSE				0x007D + M4_CH_LDB_ERROR_BASE
//1245309="Permiso de correcci�n denegado.\n \nNo puede borrar una correcci�n en la tabla "%0:s" porque no tiene los permisos necesarios.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_HIST_AUTO_ADAPT_REFUSE				0x007E + M4_CH_LDB_ERROR_BASE
//1245310="Modificaci�n denegada.\n \nLa operaci�n realizada implica una actualizaci�n del hist�rico para la cual no tiene permisos.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_DELETE_ON_CASCADE_REFUSE				0x007F + M4_CH_LDB_ERROR_BASE
//1245311="Modificaci�n denegada.\n \nNo puede borrar en la tabla "%0:s" ya que esto implica eliminar otros registros que dependen de �l.\nDebe borrar previamente los registros dependientes.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_UPDATE_PK_ON_CASCADE_REFUSE			0x0080 + M4_CH_LDB_ERROR_BASE
//1245312="Modificaci�n denegada.\n \nNo puede actualizar la clave primaria ya que esto implica actualizar otros registros que dependen de ella.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_UPDATE_DT_START_ON_CASCADE_REFUSE		0x0081 + M4_CH_LDB_ERROR_BASE
//1245313="Modificaci�n denegada.\n \nNo puede actualizar la fecha inicio ya que esto implica una actualizaci�n de otros registros que dependen de ella.\nDebe actualizar previamente los registros dependientes.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_UPDATE_DT_END_ON_CASCADE_REFUSE		0x0082 + M4_CH_LDB_ERROR_BASE
//1245314="Modificaci�n denegada.\n \nNo puede actualizar la fecha fin ya que esto implica una actualizaci�n de otros registros que dependen de ella.\nDebe actualizar previamente los registros dependientes.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_CLOSE_DT_END_ON_CASCADE_REFUSE		0x0083 + M4_CH_LDB_ERROR_BASE
//1245315="Modificaci�n denegada.\n \nNo puede actualizar la fecha fin ya que esto implica una actualizaci�n de otros registros que dependen de ella.\nDebe actualizar previamente los registros dependientes.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_CLOSE_PREV_REGISTER_REFUSE			0x0084 + M4_CH_LDB_ERROR_BASE
//1245316="Modificaci�n denegada.\n \nLa operaci�n realizada implica una actualizaci�n del hist�rico para la cual no tiene permisos.\nDebe hacer clic en el bot�n Deshacer.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_ERROR_DE_USUARIO_NO_DEFINIDO_3		0x0085 + M4_CH_LDB_ERROR_BASE
//Error de usuario no definido 3.
#define M4_CH_LDB_ERROR_DE_USUARIO_NO_DEFINIDO_4		0x0086 + M4_CH_LDB_ERROR_BASE
//Error de usuario no definido 4.
#define M4_CH_LDB_ERROR_DE_USUARIO_NO_DEFINIDO_5		0x0087 + M4_CH_LDB_ERROR_BASE
//Error de usuario no definido 5.
#define M4_CH_LDB_EXECUTESQL_IN_PARTIAL					0x0088 + M4_CH_LDB_ERROR_BASE
//1245320="ExecuteSQL en carga parcial.\n \nNo se puede invocar la funci�n "ExecuteSQL" en un nodo con carga parcial.\nDebe cambiar el c�digo del elemento que lo invoca.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_NOT_APPLY_SUBSELECT_SECURITY			0x0089 + M4_CH_LDB_ERROR_BASE
//1245321="Seguridad en persistencia.\n \nNo se puede aplicar seguridad definida en la tabla "%0:s" porque pertenece a una subselect de una persistencia.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_NOT_SUCH_ORG_IN_UPPER_LEVEL			0x008A + M4_CH_LDB_ERROR_BASE
//1245322="Sociedades de niveles no compatibles.\n \nNo se pueden utilizar datos del nivel "%0:s" porque la sociedad de trabajo "%1:s" de nivel "%2:s" no tiene definida sociedad padre en ese nivel.\nDebe consultar con el administrador.\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_REPEATED_ORG_IN_LEVEL					0x008B + M4_CH_LDB_ERROR_BASE
//1245323="Sociedad ambigua.\n \nLa sociedad "%0:s" est� asociada a m�s de una sociedad en el nivel "%1:s".\nDebe consultar con el administrador.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_BAD_MIRROR_OBJECT						0x008C + M4_CH_LDB_ERROR_BASE
//1245324="Tabla espejo mal definida.\n \nLa tabla "%0:s" no tiene el mismo n�mero de tablas f�sicas espejo que de tablas f�sicas.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_NO_MIRROR_REAL_OBJECT					0x008D + M4_CH_LDB_ERROR_BASE
//1245325="Tabla f�sica espejo no definida.\n \nLa tabla f�sica "%0:s" de "%1:s" no tiene tabla f�sica espejo definida.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_NOT_SUCH_ORG_IN_LOWER_LEVEL			0x008E + M4_CH_LDB_ERROR_BASE
//1245326="Sociedades de niveles no compatibles.\n \nNo se pueden utilizar datos del nivel "%0:s" porque la sociedad de trabajo "%1:s" es de nivel superior "%2:s".\nEsto sucedi� en "%3:s" de "%4:s"."
#define M4_CH_LDB_COMPLETENESS_FOREIGN_UNSATISFIED_END	0x008F + M4_CH_LDB_ERROR_BASE
//1245327="Fechas fuera de periodo v�lido.\n \nNo se puede insertar el registro "%0:f" de "%1:s" porque la columna "%3:d" debe ser igual a "%5:d", para poder validar la relaci�n de completitud.\nDebe introducir una fecha correcta.\nEsto sucedi� en "%6:s" de "%7:s"."
#define M4_CH_LDB_INVALID_FOREIGN_PERIOD_END			0x0090 + M4_CH_LDB_ERROR_BASE
//1245328="Datos no v�lidos en fechas.\n \nNo se puede insertar ni actualizar el registro "%0:f" de "%1:s" porque la fecha "%3:d" no est� comprendida dentro del periodo ("%4:d"-"%5:d") de validaci�n con la tabla "%7:s".\nDebe introducir fechas correctas.\nEsto sucedi� en "%8:s" de "%9:s"."
#define M4_CH_LDB_CANCELED								0x0091 + M4_CH_LDB_ERROR_BASE
//1245329="Sentencia cancelada.\n \nSe ha cancelado la sentencia durante la ejecuci�n de la funci�n "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_COMPLEX_EXECUTION						0x0092 + M4_CH_LDB_ERROR_BASE
//1245330="Sentencia demasiado compleja.\n \nSe ha cancelado la sentencia porque su ejecuci�n ha sobrepasado los "%0:d" segundos.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_UNAUTHORIZED_SENTENCE					0x0093 + M4_CH_LDB_ERROR_BASE
//1245331="Sentencia no autorizada.\n \n%0:s.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_WRONG_PARAM_VALUE						0x0094 + M4_CH_LDB_ERROR_BASE
//1245332="Valor de par�metro incorrecto.\n \nEl par�metro "%0:s" contiene un valor incorrecto "%2:s".\nSe utiliza el valor por defecto "%1:s"."
#define M4_CH_LDB_DATA_TRUNCATED						0x0095 + M4_CH_LDB_ERROR_BASE
//1245333="Dato truncado.\n \nEl valor "%0:s" excede el tama�o m�ximo "%1:s" y se truncar�.\nEsto sucedi� en "%2:s" de "%3:s"."


//' LDB User defined Errors
//' 0x130200

//1245696="Registro bloqueado.\n \nNo se puede actualizar/borrar el registro "%3:f" de "%4:s" porque ha sido bloqueado por otro usuario.\nDebe volver a hacer clic en el bot�n guardar.\nEsto sucedi� en "%5:s" de "%6:s"."

//1245697="Error de base de datos.\n \nEl filtro es incorrecto.\nFalta una expresi�n en el filtro.\nEsto sucedi� en "%5:s" de "%6:s"."

//1245698="Error de base de datos.\n \nEl filtro es incorrecto.\nEsto sucedi� en "%5:s" de "%6:s"."

//1245699="Error de base de datos.\n \nAgrupaci�n u ordenaci�n incorrecta.\nRevise las agrupaciones, ordenaciones o totalizaciones.\nEsto sucedi� en "%5:s" de "%6:s"."

//1245700="Error de base de datos.\n \nEl filtro es incorrecto.\nHay un operador relacional mal definido en el filtro.\nEsto sucedi� en "%5:s" de "%6:s"."

//1245701="El registro ya existe.\n \nNo se puede insertar el registro "%3:f" de "%4:s" porque ya existe.\nDebe cambiar el ID registro.\nEsto sucedi� en "%5:s" de "%6:s"."

//1245702="Datos repetidos.\n \nNo se puede insertar el registro "%3:f" de "%4:s" porque ya existe otro registro con los mismos valores en las columnas de la clave primaria.\nDebe volver a hacer clic en el bot�n guardar.\nEsto sucedi� en "%5:s" de "%6:s"."


//' LDB design Errors
//' 0x130500
#define M4_CH_LDB_BAD_ITEM_IN_ORDER_BY					0x0500 + M4_CH_LDB_ERROR_BASE
//1246464="Elemento incorrecto en Order By.\n \nNo hay elemento entre las columnas que coincida con %0:s.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_OUTER_JOIN_INCOMPATIBILITY			0x0501 + M4_CH_LDB_ERROR_BASE
//1246465="Posible incompatibilidad a causa de una uni�n externa.\n \nDebe repasar la sintaxis de la sentencia.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_WRONG_DATA_KEY						0x0502 + M4_CH_LDB_ERROR_BASE
//1246466="Error en la obtenci�n de la llave de encriptaci�n del usuario.\n \nDebe utilizar una llave v�lida."
#define M4_CH_LDB_OUTER_JOIN_CONFLICT					0x0503 + M4_CH_LDB_ERROR_BASE
//1246467="Conflicto en la resoluci�n de uniones externas.\n \nSe han encontrado dos uniones externas de distinto sentido entre las tablas "%0:s" y "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_DESTROYED_ACCESS  					0x0504 + M4_CH_LDB_ERROR_BASE
//1246468="Rollback forzoso.\n \nSe ha destruido "%0:s" antes de acabar la transacci�n.\nDebe revisar el ciclo de vida del Meta4Object."
#define M4_CH_LDB_DOUBLE_OUTER_JOIN  					0x0505 + M4_CH_LDB_ERROR_BASE
//1246469="Doble uni�n externa.\n \nTiene una uni�n externa doble entre las tablas "%0:s" y "%1:s".Se toma como dominante "%0:s".\nDebe especificar correctamente la uni�n externa.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_WRONG_BASIS_LOGIC_OBJECT				0x0506 + M4_CH_LDB_ERROR_BASE
//1246470="Tabla base incorrecta.\n \nLa tabla base "%0:s" de la sentencia secundaria es distinta de la tabla base "%1:s" de la sentencia principal.\nDebe consultar con el administrador.\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_WRONG_SEPARATED_LOAD_NUMREG			0x0507 + M4_CH_LDB_ERROR_BASE
//1246471="Inconsistencia en la carga.\n \nSe est� cargando un nodo con sentencias divididas y se ha producido una inconsistencia. Una devuelve "%0:d" y otra "%1:d".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_NOT_USED_12							0x0508 + M4_CH_LDB_ERROR_BASE
// No se usa.
#define M4_CH_LDB_ERROR_TRANSLATING_OUTER_JOINS_IN_FIELDS 0x0509 + M4_CH_LDB_ERROR_BASE
//1246473="Error al traducir uniones externas.\n \nError al traducir uniones externas en columnas.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_NOT_USED_13							0x050A + M4_CH_LDB_ERROR_BASE
// No se usa.
#define M4_CH_LDB_INVALID_DB_CONNECTION					0x050B + M4_CH_LDB_ERROR_BASE
//1246475="Propiedades incorrectas de conexi�n.\n \nLa conexi�n "%0:d" tiene propiedades incorrectas y por lo tanto no se tendr� en cuenta. Debe consultar otros mensajes de error."
#define M4_CH_LDB_NO_INHERITANCE						0x050C + M4_CH_LDB_ERROR_BASE
//1246476="Tabla herencia padre no definida.\n \nLa tabla "%0:s" no tiene tabla herencia padre definida.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_INHERITANCE_ERROR						0x050D + M4_CH_LDB_ERROR_BASE
//1246477="Operaci�n no permitida.\n \No se puede guardar, modificar ni borrar registros en la tabla "%0:s" porque de ella heredan otras tablas.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_NOT_USED_14							0x050E + M4_CH_LDB_ERROR_BASE
// No se usa.
#define M4_CH_LDB_INIT_ERROR							0x050F + M4_CH_LDB_ERROR_BASE
//1246479="Error al iniciar la base de datos l�gica. \n \nConsulte otros mensajes de error."
#define M4_CH_LDB_ITEM_IN_SUBQUERY						0x0510 + M4_CH_LDB_ERROR_BASE
//1246480="Elemento en subconsulta.\n \nEl elemento "%0:s" se defini� en las columnas de una subconsulta y se desech� por innecesario.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_OUTER_JOIN_NO_ALLOWED_II				0x0511 + M4_CH_LDB_ERROR_BASE
//1246481="Operador de uni�n externa no permitido\n \nEl operador de uni�n externa est� asociado a una columna que se encuentra dentro del �mbito de un operador OR.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_MAX_NUMBER_SUBQUERY_EXCEDED			0x0512 + M4_CH_LDB_ERROR_BASE
//1246482="M�ximo n�mero de subconsultas excedido.\n \nSe ha excedido el m�ximo n�mero de subconsultas dentro de una misma operaci�n de lectura.\nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_DYN_INTERNAL_ERROR					0x0513 + M4_CH_LDB_ERROR_BASE
//1246483="Error interno.\n \nError interno en la funci�n "%0:s". "%1:s".\nEsto sucedi� en "%2:s" de "%3:s"."
#define M4_CH_LDB_DYN_DB_USER_PASSWORD_NOT_FOUND		0x0514 + M4_CH_LDB_ERROR_BASE
//1246484="Usuario/contrase�a de BD no encontrados.\n \nLa sentencia de validaci�n de usuarios no ha encontrado usuario y contrase�a parametrizados para el usuario de aplicaci�n. "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_DYN_USER_ACCESS_DENIED				0x0515 + M4_CH_LDB_ERROR_BASE
//1246485="Error de seguridad.\n \nLa contrase�a de base de datos no fue encriptada por el usuario de aplicaci�n. "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_DYN_PASSWORD_DB_DESENCRYPT			0x0516 + M4_CH_LDB_ERROR_BASE
//1246486="Error en la desencriptaci�n.\n \nSe ha producido un error en la desencriptaci�n de la contrase�a de base de datos. "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_DYN_PASSWORD_APP_DESENCRYPT			0x0517 + M4_CH_LDB_ERROR_BASE
//1246487="Error en la desencriptaci�n.\n \nSe ha producido un error en la desencriptaci�n de la contrase�a del usuario de aplicaci�n. "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_DYN_GETTING_DYNAMIC_CONNECTION		0x0518 + M4_CH_LDB_ERROR_BASE
//1246488="Error obteniendo conexi�n din�mica.\n \nEl usuario "%0:s" no ha podido obtener una conexi�n din�mica.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_DYN_INVALID_STATEMENT					0x0519 + M4_CH_LDB_ERROR_BASE
//1246489="Sentencia de validaci�n de usuarios err�nea.\n \nLa sentencia parametrizada "%0:s" para obtener usuario/password (2x) de base de datos no es v�lida.\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_DYN_USER_BLOCKED						0x051A + M4_CH_LDB_ERROR_BASE
//1246490="Usuario de aplicaci�n bloqueado.\n \nEl usuario de aplicaci�n se encuentra bloqueado. "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_DYN_PASSWORD_EXPIRED					0x051B + M4_CH_LDB_ERROR_BASE
//1246491="Usuario con contrase�a expirada.\n \nEl periodo de validez de su contrase�a ha caducado. Debe cambiar su contrase�a antes de continuar. "%0:s".\nEsto sucedi� en "%1:s" de "%2:s"."
#define M4_CH_LDB_SYSHINT_WRONG_ATTRIBUTES				0x051C + M4_CH_LDB_ERROR_BASE
//1246492="Syshint err�neo.\n \nFalta el atributo obligatorio "%0:s" en la definici�n del hint. Revise la definici�n del hint."
#define M4_CH_LDB_SYSHINT_MISSING_CHARACTER_OR_KEYWORD	0x051D + M4_CH_LDB_ERROR_BASE
//1246493="Syshint err�neo.\n \nFalta el caracter o la palabra clave "%0:s" en la definici�n del hint. Revise la definici�n del hint."
#define M4_CH_LDB_SYSHINT_SINTAX_ERROR					0x051E + M4_CH_LDB_ERROR_BASE
//1246494="Syshint err�neo.\n \nFalta el caracter o la palabra clave "%0:s". Revise el ApiSql que hace referencia al hint."
#define M4_CH_LDB_SYSHINT_IDHINT_MISSING				0x051F + M4_CH_LDB_ERROR_BASE
//1246495="Syshint err�neo.\n \nFalta el identificador del hint". Revise el ApiSql que hace referencia al hint."
#define M4_CH_LDB_SYSHINT_IDHINT_DUPLICATED				0x0520 + M4_CH_LDB_ERROR_BASE
//1246496="Syshint err�neo.\n \nEl hint "%0:s" est� duplicado". Revise la definici�n de hints."
#define M4_CH_LDB_SYSHINT_WRONG_DB						0x0521 + M4_CH_LDB_ERROR_BASE
//1246497="Syshint err�neo.\n \nEl gestor de base de datos "%0:s" no es correcto". Revise la definici�n del hints."
#define M4_CH_LDB_SYSHINT_WRONG_TYPE					0x0522 + M4_CH_LDB_ERROR_BASE
//1246498="Syshint err�neo.\n \nEl tipo de hint "%0:s" no es correcto". Revise la definici�n de hints."
#define M4_CH_LDB_SYSHINT_IDHINT_NO_EXISTS				0x0523 + M4_CH_LDB_ERROR_BASE
//1246499="Syshint err�neo.\n \nEl hint "%0:s" no est� definido". Revise el ApiSql que hace referencia al hint."
#define M4_CH_LDB_BASE_DIFFERS_SUBSELECT				0x0524 + M4_CH_LDB_ERROR_BASE
//1246500="La tabla base de la sentencia principal "%0:s" no coincide con la tabla de la subselect "%1:s"."
#define M4_CH_LDB_SUBSELECT_NEEDS_TABLE_BASE			0x0525 + M4_CH_LDB_ERROR_BASE
//1246501="Error de sintaxis.\n \nNo es posible ejecutar la subconsulta asociadada, es necesario definir una tabla base en la subconsulta.\n \nEsto sucedi� en "%0:s" de "%1:s"."
#define M4_CH_LDB_MISMATCHED_VERSIONS					0x0526 + M4_CH_LDB_ERROR_BASE
//1246502="Incompatibilidad de versiones.\n \nLa versi�n actual de la tecnolog�a no es compatible con la versi�n del repositorio al que se conecta.\nDebe consultar con el administrador."
#define M4_CH_LDB_MISMATCHED_VERSIONS_VERBOSE			0x0527 + M4_CH_LDB_ERROR_BASE
//1246503="Incompatibilidad de versiones.\n \nLa versi�n actual de la tecnolog�a (%0:s), no es compatible con la versi�n del repositorio al que se conecta (%1:s).\nDebe consultar con el administrador."
#define M4_CH_LDB_NO_DB_CONNECTION						0x0528 + M4_CH_LDB_ERROR_BASE
//1246504="Conexi�n no definida.\n \nLa conexi�n "%0:d" no est� definida."

// LDB Audit Errors
// 0x130600
#define M4_CH_LDB_AUDIT_EXEC_ERROR						0x0600 + M4_CH_LDB_ERROR_BASE
//1246720="Error de auditor�a.\n \nSe ha producido un error ejecutando la auditor�a.\nEsto sucedi� en la auditor�a de tipo "%0:s."
#define M4_CH_LDB_AUDIT_BUILD_CHANNEL					0x0601 + M4_CH_LDB_ERROR_BASE
//1246721="Error de auditor�a.\n \nNo se ha podido crear el Meta4Object de auditor�a.\nNo se ha podido crear el Meta4Object "%0:s". Debe consultar otros errores."
#define M4_CH_LDB_AUDIT_INTERNAL_NO_CHANNEL				0x0602 + M4_CH_LDB_ERROR_BASE
//1246722="Error interno de auditor�a.\n \nSe ha producido un error interno de auditor�a.\nLa referencia interna al Meta4Object de auditor�a es inv�lida. Consulte con su administrador."
#define M4_CH_LDB_REAL_STATEMENT_EXEC_NOT_ALLOWED		0x0603 + M4_CH_LDB_ERROR_BASE
//1246723="Ejecuci�n no permitida.\n \nLa parametrizaci�n actual no le permite ejecutar sentencias reales. Consulte con su administrador."
#define M4_CH_LDB_REAL_STATEMENT_EXEC_UNKNOWN_MODE		0x0604 + M4_CH_LDB_ERROR_BASE
//1246724="Ejecuci�n no permitida.\n \nSe ha parametrizado un valor no v�lido en la ejecuci�n de sentencias reales. Consulte con su administrador."

//' End LDB Errors
//''''''''''''''''''''''''''''''

#endif	// _LDBRES_HPP_
