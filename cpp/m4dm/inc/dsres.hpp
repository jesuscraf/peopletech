#ifndef __DSRES_HPP
#define __DSRES_HPP

#include "m4res.hpp"


#define M4_CH_DS_INCORRECT_SERIALIZE_VERSION            0x0001 + M4_CH_DS_ERROR_BASE
//Incorrect serialization version. Received %0:n, expected %1:n.
#define M4_CH_DS_SERIALIZE                              0x0002 + M4_CH_DS_ERROR_BASE
//Error in serialize.
#define M4_CH_DS_DESERIALIZE                            0x0003 + M4_CH_DS_ERROR_BASE
//Error in deserialize.

#define M4_CH_DS_CHANNEL_PROCESSING                     0x0004 + M4_CH_DS_ERROR_BASE
//Error in Meta4Object processing. Item %0:s of Node %1:s in Meta4Object %2:s.
#define M4_CH_DS_PRE_PROCESS_CHANNEL                    0x0005 + M4_CH_DS_ERROR_BASE
//Error in Meta4Object preprocess. Meta4Object %0:s.
#define M4_CH_DS_POST_PROCESS_CHANNEL                   0x0006 + M4_CH_DS_ERROR_BASE
//Error in Meta4Object postprocess. Meta4Object %0:s.
#define M4_CH_DS_PRE_PROCESS_CHANNELDATA                0x0007 + M4_CH_DS_ERROR_BASE
//Error in Meta4Object data preprocess. Meta4Object %0:s.
#define M4_CH_DS_POST_PROCESS_CHANNELDATA               0x0008 + M4_CH_DS_ERROR_BASE
//Error in Meta4Object data postprocess. Meta4Object %0:s.
#define M4_CH_DS_PRE_PROCESS_NODECHANNEL                0x0009 + M4_CH_DS_ERROR_BASE
//Error in node of Meta4Object preprocess. Node %0:s.
#define M4_CH_DS_POST_PROCESS_NODECHANNEL               0x000A + M4_CH_DS_ERROR_BASE
//Error in node of Meta4Object postprocess. Node %0:s.
#define M4_CH_DS_PRE_PROCESS_PXBLOCK                    0x000B + M4_CH_DS_ERROR_BASE
//Error in proxy block preprocess. Node %0:s, parent row %1:n.
#define M4_CH_DS_POST_PROCESS_PXBLOCK                   0x000C + M4_CH_DS_ERROR_BASE
//Error in proxy block postprocess. Node %0:s, parent row %1:n.
#define M4_CH_DS_PRE_PROCESS_VRB_STATIC                 0x000D + M4_CH_DS_ERROR_BASE
//Error in VRB static part preprocess.
#define M4_CH_DS_POST_PROCESS_VRB_STATIC                0x000E + M4_CH_DS_ERROR_BASE
//Error in VRB static part postprocess.
#define M4_CH_DS_PRE_PROCESS_VRB_RECORD                 0x000F + M4_CH_DS_ERROR_BASE
//Error in VRB row preprocess. Record %0:n.
#define M4_CH_DS_POST_PROCESS_VRB_RECORD                0x0010 + M4_CH_DS_ERROR_BASE
//Error in VRB row postprocess. Record %0:n.
#define M4_CH_DS_PRE_PROCESS_RECORD                     0x0011 + M4_CH_DS_ERROR_BASE
//Error in row preprocess.
#define M4_CH_DS_POST_PROCESS_RECORD                    0x0012 + M4_CH_DS_ERROR_BASE
//Error in row postprocess.

#define M4_CH_DS_SERIALIZE_CHANNEL                      0x0013 + M4_CH_DS_ERROR_BASE
//Error in Meta4Object serialize. Meta4Object %0:s.
#define M4_CH_DS_DESERIALIZE_CHANNEL                    0x0014 + M4_CH_DS_ERROR_BASE
//Error in Meta4Object deserialize. Meta4Object %0:s.
#define M4_CH_DS_SERIALIZE_NODECHANNEL                  0x0015 + M4_CH_DS_ERROR_BASE
//Error in node of Meta4Object serialize. Node %0:s.
#define M4_CH_DS_DESERIALIZE_NODECHANNEL                0x0016 + M4_CH_DS_ERROR_BASE
//Error in node of Meta4Object deserialize. Node %0:s.
#define M4_CH_DS_SERIALIZE_PXBLOCK                      0x0017 + M4_CH_DS_ERROR_BASE
//Error in proxy block serialize. Node %0:s, parent row %1:n.
#define M4_CH_DS_DESERIALIZE_PXBLOCK                    0x0018 + M4_CH_DS_ERROR_BASE
//Error in proxy block deserialize. Node %0:s, parent row %1:n.
#define M4_CH_DS_SERIALIZE_VRB_STATIC                   0x0019 + M4_CH_DS_ERROR_BASE
//Error in VRB static part serialize.
#define M4_CH_DS_DESERIALIZE_VRB_STATIC                 0x001A + M4_CH_DS_ERROR_BASE
//Error in VRB static part deserialize.
#define M4_CH_DS_SERIALIZE_VRB_RECORD                   0x001B + M4_CH_DS_ERROR_BASE
//Error in VRB row serialize. Row %0:n.
#define M4_CH_DS_DESERIALIZE_VRB_RECORD                 0x001C + M4_CH_DS_ERROR_BASE
//Error in VRB row deserialize. Row %0:n.
#define M4_CH_DS_DESERIALIZE_VRB_END                    0x001D + M4_CH_DS_ERROR_BASE
//Error in VRB end deserialize.
#define M4_CH_DS_SERIALIZE_RECORD                       0x001E + M4_CH_DS_ERROR_BASE
//Error in row serialize.
#define M4_CH_DS_DESERIALIZE_RECORD                     0x001F + M4_CH_DS_ERROR_BASE
//Error in row deserialize.
#define M4_CH_DS_SERIALIZE_ITEM                         0x0020 + M4_CH_DS_ERROR_BASE
//Error in item serialize. Item handle %0:n.
#define M4_CH_DS_DESERIALIZE_ITEM                       0x0021 + M4_CH_DS_ERROR_BASE
//Error in item deserialize. Item handle %0:n.
#define M4_CH_DS_INVALID_RECORD                         0x0022 + M4_CH_DS_ERROR_BASE
// The row is invalid. Row %0:n.
#define M4_CH_DS_PROCESS_CHECK_NULLS                    0x0023 + M4_CH_DS_ERROR_BASE
//Error checking nulls. Item %0:s of Node %1:s in Meta4Object %2:s.
#define M4_CH_DS_ADV_SERIAL_BLOCK_NOT_MARKED			0x0024 + M4_CH_DS_ERROR_BASE
//Inconsistencia en las marcas para serialización avanzada del Meta4Object.\n \nPosiblemente el método remoto genera objetos que no están marcados en la serialización avanzada.\nDebe comprobar que el tipo de sincronización del método concuerda con las marcas hechas sobre el Meta4Object.\nEsto sucedió en "%0:s".
#define M4_CH_DS_ADV_SERIAL_BLOCK_DONT_EXIST			0x0025 + M4_CH_DS_ERROR_BASE
//La fila no se pudo marcar para serializar.\n \nLa fila no se pudo marcar para serializar porque el bloque todavía no está creado.\nDebe comprobar que está situado en el bloque correcto, o si es correcto que todavía no esté creado el bloque.\nEsto sucedió al intentar marcar la fila "%0:d" de "%1:s" en "%1:s".
#define M4_CH_DS_NO_SUCH_BLOB_FILE						0x0026 + M4_CH_DS_ERROR_BASE
//No existe el fichero BLOB.\n \nNo se puede serializar el fichero "%0:s" asociado a un BLOB porque no existe.\nDebe comprobar que el fichero no se ha borrado.

#endif // __DSRES_HPP
