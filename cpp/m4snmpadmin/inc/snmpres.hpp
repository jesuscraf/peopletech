

#ifndef SNMPERRORS_HPP
#define SNMPERRORS_HPP


// base error for snmp.
#include "m4res.hpp"

// use mvc macros to send messages to log system.
#include "m4objlog.hpp"


#define M4_SNMP_INVALID_VALUE                               0x0001 + M4_SRV_SNMP_ERROR_BASE
// "Valor no válido para el parámetro "%0:s". Valor: "%1:s"."
#define M4_SNMP_CANNOT_CREATE_TABLE                         0x0002 + M4_SRV_SNMP_ERROR_BASE
// "No se puede crear tabla para el subsistema tipo "%0:s"."
#define M4_SNMP_CANNOT_FIND_TABLE                           0x0003 + M4_SRV_SNMP_ERROR_BASE
// "No se puede encontrar tabla para el subsistema tipo "%0:s"."
#define M4_SNMP_SUBSYSTEM_NOT_EXPORTED                      0x0004 + M4_SRV_SNMP_ERROR_BASE
// "Subsistema tipo "%0:s" no exportado en el fichero de configuración snmp."
#define M4_SNMP_OBJECT_NOT_CREATED                          0x0005 + M4_SRV_SNMP_ERROR_BASE
// "No se puede crear objeto snmp "%0:s"."
#define M4_SNMP_INVALID_MIB_TYPE                            0x0006 + M4_SRV_SNMP_ERROR_BASE
// "Tipo mib no válido para objeto snmp. Tipo: "%0:s", Objeto: "%1:s."
#define M4_SNMP_CANNOT_GET_XML_ITEM                         0x0007 + M4_SRV_SNMP_ERROR_BASE
// "Error en el fichero de configuración xml. No se puede obtener el atributo especificado. Ver otros errores."
#define M4_SNMP_CANNOT_FIND_ROW                             0x0008 + M4_SRV_SNMP_ERROR_BASE
// "No se puede encontrar la fila correspondiente al subsistema "%1:s en la tabla snmp."
#define M4_SNMP_OBL2XML_ERROR                               0x0009 + M4_SRV_SNMP_ERROR_BASE
// "No se puede traducir la configuración OBL a formato XML."
#define M4_SNMP_XML_PARSING_ERROR                           0x000A + M4_SRV_SNMP_ERROR_BASE
// "Error parseando fichero de configuración xml "%0:s"." 
#define M4_SNMP_MEMORY_ALLOCATION_ERROR                     0x000B + M4_SRV_SNMP_ERROR_BASE
// "Error reservando memoria en "%0:s"." 
#define M4_SNMP_SNMP_PP_LIBRARY_ERROR                       0x000C + M4_SRV_SNMP_ERROR_BASE
// "Error interno en el api snmp: "%0:s", "%1:s"" 
#define M4_SNMP_CANNOT_GET_OBL_ITEM                         0x000D + M4_SRV_SNMP_ERROR_BASE
// "Error en el fichero de configuración obl. No se puede obtener el atributo "%0:s"."
#define M4_SNMP_LIBRARY_ERROR                               0x000E + M4_SRV_SNMP_ERROR_BASE
// "Error interno en el subsistema snmp: "%0:s", "%1:s"."
#define M4_SNMP_CANNOT_GET_XML_NODE_BY_ATT                  0x000F + M4_SRV_SNMP_ERROR_BASE
// "Error en el fichero de configuración xml. No se puede obtener nodo con atributo "%0:s" y valor "%1:s"."
#define M4_SNMP_CANNOT_GET_XML_NODE_BY_NAME                 0x0010 + M4_SRV_SNMP_ERROR_BASE
// "Error en el fichero de configuración xml. No se puede obtener nodo con nombre "%0:s"."
#define M4_SNMP_CANNOT_GET_XML_ATTRIBUTE                    0x0011 + M4_SRV_SNMP_ERROR_BASE
// "Error en el fichero de configuración xml. No se puede obtener atributo "%0:s" en el nodo "%1:s"."
#define M4_SNMP_XML_PARSER_ERROR                            0x0012 + M4_SRV_SNMP_ERROR_BASE
// "Error en el fichero de configuración xml. Parse error: "%0:s"."
#define M4_SNMP_CANNOT_GET_COMMAND_ID_IN_SNMP_COMMAND       0x0013 + M4_SRV_SNMP_ERROR_BASE
// "No se pudo obtener el identificador del comando en la ejecución comando snmp."
#define M4_SNMP_INVALID_COMMAND_ID_IN_SNMP_COMMAND          0x0014 + M4_SRV_SNMP_ERROR_BASE
// "Identificador no válido en la ejecución comando snmp. Identificador "%0:s"."
#define M4_SNMP_CANNOT_GET_PARAM_IN_SNMP_COMMAND            0x0015 + M4_SRV_SNMP_ERROR_BASE
// "No se pudo obtener el identificador del parámetro en la ejecución del comando snmp. Comando "%0:s"."
#define M4_SNMP_INVALID_PARAM_ID_IN_SNMP_COMMAND            0x0016 + M4_SRV_SNMP_ERROR_BASE
// "Parámetro no válido en la ejecución del comando snmp. Comando "%0:s". Parámetro "%1:s"."
#define M4_SNMP_NAVIGATOR_NOT_SET_IN_SUBSYSTEM              0x0017 + M4_SRV_SNMP_ERROR_BASE
// "No se encuentra objeto \"navigator\" en el subsistema: "%0:s"."
#define M4_SNMP_CANNOT_GET_PARENT_SUBSYSTEM                 0x0018 + M4_SRV_SNMP_ERROR_BASE
// "No se encuentra subsistema padre para el subsistema: "%0:s"."
#define M4_SNMP_CONFIGURATION_NOT_SET_IN_SUBSYSTEM          0x0019 + M4_SRV_SNMP_ERROR_BASE
// "No se encuentra objeto \"configuration\" en el subsistema: "%0:s"."
#define M4_SNMP_TRANSPORT_OPERATION_FAILED					0x001A + M4_SRV_SNMP_ERROR_BASE
// "No se puede inicial el subsistema SNMP porque el puerto configurado "%0:d" está en uso."
#define M4_SNMP_ATTRIBULTE_NOT_FOUND						0x001B + M4_SRV_SNMP_ERROR_BASE
// "Error recuperando el atributo "%0:s" en el nodo %1:d de la lista ("%2:s")."

#endif // SNMPERRORS_HPP

