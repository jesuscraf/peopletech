//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4xlapierrors.hpp
// Project:			 mind3.x
// Author:           Meta Software M.S. , S.A
// Date:			 22/10/2013
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef m4xlapierrors_hpp
#define m4xlapierrors_hpp 1

#include "m4res.hpp"


//==============================================================================
//  Errores de xlapi
//==============================================================================

// 11010048="Error en interfaz Excel. El número o el tipo de los argumentos para el método "%0:s" no es válido. Debe consultar con el administrador."
#define	M4_XL_ERR_BAD_ARGUMENTS						M4_EXCEL_LIBXL_ERROR_BASE
// 11010049="Error abriendo archivo Excel.\n \nLa ruta del archivo Excel "%0:s" no es válida. Asegúrese de la existencia del archivo y que su extensión es válida para Excel."
#define	M4_XL_ERR_OPEN_WB_BAD_FILE					0x0001 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010050="Error abriendo archivo Excel.\n \nNo se ha podido construir el objeto workbook. Debe consultar con el administrador."
#define	M4_XL_ERR_OPEN_WB_NULL_OBJ					0x0002 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010051="Error abriendo archivo Excel.\n \nSe ha producido el error "%0:s" abriendo el archivo "%1:s". Debe consultar con el administrador."
#define	M4_XL_ERR_OPEN_WB_LIB_ERR					0x0003 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010052="Error en interfaz Excel.\n \nNo se ha podido realizar la operación "%0:s" porque el archivo Excel no está cargado. Debe cargar el archivo Excel previamente."
#define	M4_XL_ERR_WB_NOT_LOADED						0x0004 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010053="Error guardando archivo Excel.\n \nLa ruta del archivo no es válida. Asegúrese de haber cargado el archivo previamente y de especificar una ruta válida."
#define	M4_XL_ERR_SAVE_WB_BAD_PATH					0x0005 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010054="Error guardando archivo Excel.\n \nSe ha producido el error "%0:s" guardando el archivo "%1:s". Debe consultar con el administrador."
#define	M4_XL_ERR_SAVE_WB_LIB_ERR					0x0006 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010055="Error recuperando hoja Excel.\n \nEl índice o el nombre de la hoja Excel indicado ("%0:s") no es válido. Debe indicar un índice mayor o igual que cero o un nombre de hoja Excel válido."
#define	M4_XL_ERR_OPEN_WS_BAD_INDEX					0x0007 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010056="Error recuperando hoja Excel.\n \nSe ha producido el error "%0:s" recuperando la hoja Excel con índice "%1:d". Debe consultar con el administrador."
#define	M4_XL_ERR_OPEN_WS_LIB_ERR					0x0008 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010057="Error en interfaz Excel.\n \nNo se ha podido realizar la operación "%0:s" porque la hoja Excel no está cargada. Debe recuperar la hoja Excel previamente."
#define	M4_XL_ERR_WS_NOT_LOADED						0x0009 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010058="Error obteniendo coordenadas de celda.\n \nNo se ha podido obtener las coordenadas de la celda porque la referencia indicada no es válida. Debe recuperar la hoja Excel previamente."
#define	M4_XL_ERR_BAD_CELL_REF_						0x000A + M4_EXCEL_LIBXL_ERROR_BASE
// 11010059="Error incorporando archivo de datos.\n \nLa ruta del archivo de datos "%0:s" no es válida, asegúrese de la existencia del archivo. Debe indicar una ruta válida."
#define	M4_XL_ERR_PARS_BAD_DATA_FILE				0x000B + M4_EXCEL_LIBXL_ERROR_BASE
// 11010060="Error incorporando archivo de datos.\n \nEl dato supera el tamaño máximo definido. Debe consultar con el administrador."
#define	M4_XL_ERR_PARS_SIZE_EXCEEDED				0x000C + M4_EXCEL_LIBXL_ERROR_BASE
// 11010061="Error incorporando archivo de datos.\n \nSe ha producido el error "%0:s" recuperando el formato personalizado correspondiente al formato "%1:d". Debe consultar con el administrador."
#define	M4_XL_ERR_PARS_NUMFORMAT_LIB_ERR			0x000D + M4_EXCEL_LIBXL_ERROR_BASE
// 11010062="Error incorporando archivo de datos.\n \nSe ha producido el error "%0:s" asignando un valor a la celda "%1:d, %2:d". Debe consultar con el administrador."
#define	M4_XL_ERR_PARS_WRITE_LIB_ERR				0x000E + M4_EXCEL_LIBXL_ERROR_BASE
// 11010063="Error incorporando archivo de datos.\n \nSe han producido errores durante el proceso de incorporación de datos, revise el resto de errores para obtener información detallada. Debe consultar con el administrador."
#define	M4_XL_ERR_PARS_GEN_ERR						0x000F + M4_EXCEL_LIBXL_ERROR_BASE
// 11010064="Error en interfaz Excel.\n \nNo se ha podido obtener o asignar el valor de la celda porque las coordenadas indicadas no son válidas. Asegúrese de que que las coordenadas de la celda sean números positivos."
#define	M4_XL_ERR_GET_SET_BAD_CELL_COOR_			0x0010 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010065="Error en interfaz Excel.\n \nNSe ha producido el error "%0:s" al obtener o asignar el valor de la celda "%1:d, %2:d". Debe consultar con el administrador."
#define	M4_XL_ERR_GET_SET_CELL_LIB_ERR				0x0011 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010066="Error recuperando rango Excel.\n \nLas celdas indicadas para la especificación del rango no son válidas. Debe indicar unas celdas válidas."
#define	M4_XL_ERR_RANGE_BAD_LIMITS_					0x0012 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010067="Error volcando rango Excel en nodo.\n \nLa instancia del Meta4Object indicada ("%0:d") no existe. Debe indicar una instancia de Meta4Object válida."
#define	M4_XL_ERR_DTN_BAD_INSTANCE					0x0013 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010068="Error volcando rango Excel en nodo.\n \nEl nodo indicado ("%0:s") no existe en el Meta4Object. Debe indicar un nodo válido del Meta4Object."
#define	M4_XL_ERR_DTN_BAD_NODE						0x0014 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010069="Error volcando rango Excel en nodo.\n \nSe ha producido un error al añadir o eliminar registros del nodo. Debe consultar con el administrador."
#define	M4_XL_ERR_DTN_ADD_REM_REGS					0x0015 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010070="Error volcando rango Excel en nodo.\n \nSe ha producido un error al asignar valor al item "%0:s" del nodo. Debe consultar con el administrador."
#define	M4_XL_ERR_DTN_SET_ITEM_VALUE				0x0016 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010071="Error asignando multiples valores en Excel.\n \nNo se han podido asignar los valores en Excel, porque las coordenadas de la celda de inicio indicadas no son válidas. Asegúrese de que que las coordenadas de la celda sean números positivos."
#define	M4_XL_ERR_PST_BAD_CELL_COOR_				0x0017 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010072="Error asignando multiples valores en Excel.\n \nLa lista de valores a asignar es nula. Debe indicar los valores a asignar, en una lista con formato csv."
#define	M4_XL_ERR_PST_BAD_DATA						0x0018 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010073="Error asignando multiples valores en Excel.\n \nNo se ha indicado el delimitador de los elementos en la lista de valores a asignar. Debe consultar con el administrador."
#define	M4_XL_ERR_PST_BAD_DELIMITER_				0x0019 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010074="Error creando archivo Excel.\n \nLa ruta para el nuevo archivo Excel "%0:s" no es válida. Debe indicar una ruta válida para el archivo."
#define	M4_XL_ERR_NEW_WB_BAD_FILE_					0x001A + M4_EXCEL_LIBXL_ERROR_BASE
// 11010075="Error creando archivo Excel.\n \nEl archivo Excel "%0:s" ya existe. Debe indicar la ruta a un nuevo archivo Excel o especificar que debe sobrescribirse si ya existe."
#define	M4_XL_ERR_NEW_WB_CANNOT_OVERWRITE_			0x001B + M4_EXCEL_LIBXL_ERROR_BASE
// 11010076="Error creando archivo Excel.\n \nLa ruta del archivo Excel "%0:s" no es válida. Asegúrese de que su extensión es válida para Excel."
#define	M4_XL_ERR_NEW_WB_BAD_FILE					0x001C + M4_EXCEL_LIBXL_ERROR_BASE
// 11010077="Error creando archivo Excel.\n \nNo se ha podido construir el objeto workbook. Debe consultar con el administrador."
#define	M4_XL_ERR_NEW_WB_NULL_OBJ					0x001D + M4_EXCEL_LIBXL_ERROR_BASE
// 11010078="Error creando archivo Excel.\n \nSe ha producido el error "%0:s" guardando el archivo "%1:s". Debe consultar con el administrador."
#define	M4_XL_ERR_NEW_WB_LIB_ERR					0x001E + M4_EXCEL_LIBXL_ERROR_BASE
// 11010079="Error creando hoja Excel.\n \nNo se ha indicado un nombre para la nueva hoja Excel. Debe indicar un nombre para la nueva hoja Excel."
#define	M4_XL_ERR_NEW_WS_NO_NAME_					0x001F + M4_EXCEL_LIBXL_ERROR_BASE
// 11010080="Error creando hoja Excel.\n \nEl nombre indicado ("%0:s") para la nueva hoja Excel, ya existe en el archivo Excel. Debe indicar un nombre válido para la nueva hoja Excel."
#define	M4_XL_ERR_NEW_WS_NAME_IN_USE_				0x0020 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010081="Error creando hoja Excel.\n \nSe ha producido el error "%0:s" creando la hoja Excel "%1:s". Debe consultar con el administrador."
#define	M4_XL_ERR_NEW_WS_LIB_ERR					0x0021 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010082="Error copiando celda Excel.\n \nNo se ha podido copiar la celda porque las coordenadas indicadas no son válidas. Asegúrese de que que las coordenadas de las celdas sean números positivos."
#define	M4_XL_ERR_COPY_CELL_BAD_COOR_				0x0022 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010083="Error copiando celda Excel.\n \nSe ha producido el error "%0:s" copiando la celda "%1:d, "%2:d" en "%3:d, "%4:d". Debe consultar con el administrador."
#define	M4_XL_ERR_COPY_CELL_LIB_ERR					0x0023 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010084="Error copiando fila Excel.\n \nNo se ha podido copiar la fila porque el índice no es válido. Asegúrese de que que el índice de la fila sea un número positivo."
#define	M4_XL_ERR_COPY_ROW_BAD_COOR_				0x0024 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010085="Error estableciendo alto de fila Excel.\n \nSe ha producido el error "%0:s" estableciendo "%1:d" como alto de la fila "%2:d". Debe consultar con el administrador."
#define	M4_XL_ERR_SET_ROW_HEIGHT_LIB_ERR			0x0025 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010086="Error insertando fila Excel.\n \nSe ha producido el error "%0:s" al insertar una fila en la posición "%1:d". Debe consultar con el administrador."
#define	M4_XL_ERR_INSERT_ROW_LIB_ERR				0x0026 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010087="Error en interfaz Excel.\n \nNo se ha podido ocultar o mostrar una fila o columna, porque el índice indicado no es válido. Asegúrese de que que el índice de la fila o de la columna sean números positivos."
#define	M4_XL_ERR_HIDE_ROW_COL_BAD_INDEX_			0x0027 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010088="Error en interfaz Excel.\n \nNo se ha podido realizar la operación "%0:s" porque no se ha podido codificar la cadena expecificada. Debe consultar con el administrador."
#define	M4_XL_ERR_GEN_CPP_2_XL						0x0028 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010089="Error en interfaz Excel.\n \nNo se ha podido realizar la operación "%0:s" porque no se ha podido codificar la cadena devuelta por la interfaz. Debe consultar con el administrador."
#define	M4_XL_ERR_GEN_XL_2_CPP						0x0029 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010090="Error en interfaz Excel.\n \nNo se ha podido modificar el formato de la celda "%0:d, "%1:d". Debe consultar con el administrador."
#define	M4_XL_ERR_FORMAT_NOT_ADDED					0x002A + M4_EXCEL_LIBXL_ERROR_BASE

// 11010091="Error en interfaz Excel.\n \nNo se ha podido modificar el formato o estilo de la celda porque las coordenadas indicadas no son válidas. Asegúrese de que que las coordenadas de la celda sean números positivos."
#define	M4_XL_ERR_CHANGE_STYLE_BAD_CELL_COOR_		0x002B + M4_EXCEL_LIBXL_ERROR_BASE
// 11010092="Error en interfaz Excel.\n \nNSe ha producido el error "%0:s" al tratar de modificar el formato de una celda. Debe consultar con el administrador."
#define	M4_XL_ERR_CHNG_CELL_STYLE_LIB_ERR			0x002C + M4_EXCEL_LIBXL_ERROR_BASE
// 11010093="Error en interfaz Excel.\n \nNSe ha producido el error "%0:s" al tratar de modificar el formato (%1:s) de la celda "%2:d, %3:d". Debe consultar con el administrador."
#define	M4_XL_ERR_CHNG_CELL_STYLE_LIB_ERR_DET		0x002D + M4_EXCEL_LIBXL_ERROR_BASE
// 11010094="Error en interfaz Excel.\n \nNSe ha producido el error "%0:s" al tratar de modificar el formato (%1:s) de la fila "%2:d". Debe consultar con el administrador."
#define	M4_XL_ERR_CHNG_ROW_STYLE_LIB_ERR_DET		0x002E + M4_EXCEL_LIBXL_ERROR_BASE
// 11010095="Error en interfaz Excel.\n \nNSe ha producido el error "%0:s" al tratar de modificar el formato (%1:s) de la columna "%2:d". Debe consultar con el administrador."
#define	M4_XL_ERR_CHNG_COL_STYLE_LIB_ERR_DET		0x002F + M4_EXCEL_LIBXL_ERROR_BASE
// 11010096="Error en interfaz Excel.\n \nNo se ha modificado el atributo subrayado de la celda porque el tipo de subrayado especificado (%0:d) no es válido. Debe indicar un tipo de subrayado válido."
#define	M4_XL_ERR_CHANGE_STYLE_BAD_UNDERLINE_TYPE	0x0030 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010097="Error en interfaz Excel.\n \nNo se ha modificado el formato de la celda porque el valor especificado no es de un tipo válido. Debe consultar con el administrador."
#define	M4_XL_ERR_CHANGE_STYLE_BAD_VAL_TYPE			0x0031 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010098="Error en interfaz Excel.\n \nNo se ha podido modificar el formato (%0:s) de la celda "%1:d, %2:d". Debe consultar con el administrador."
#define	M4_XL_ERR_CHANGE_STYLE_GEN					0x0032 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010099="Error en interfaz Excel.\n \nNo se ha modificado el atributo color de la celda porque el índice del color especificado (%0:d) no es válido. Debe indicar un índice de color válido."
#define	M4_XL_ERR_CHANGE_STYLE_BAD_COLOR_INDEX		0x0033 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010100="Error en interfaz Excel.\n \nNo se ha modificado el atributo estilo de borde de la celda, porque el estilo especificado (%0:d) no es válido. Debe indicar un estilo de borde válido."
#define	M4_XL_ERR_CHANGE_STYLE_BAD_BORDER_TYPE		0x0034 + M4_EXCEL_LIBXL_ERROR_BASE

// 11010101="Error en interfaz Excel.\n \nNo se ha podido insertar el rango de celdas porque las celdas indicadas no son válidas. Asegúrese de que que las coordenadas del rango sean correctas."
#define	M4_XL_ERR_INSERT_BAD_CELL_COOR_				0x0035 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010102="Error insertando columna Excel.\n \nSe ha producido el error "%0:s" al insertar una columna en la posición "%1:d". Debe consultar con el administrador."
#define	M4_XL_ERR_INSERT_COL_LIB_ERR				0x0036 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010103="Error insertando rango Excel.\n \n.No se ha podido insertar el rango copiado porque las áreas se superponen. Debe consultar con el administrador."
#define	M4_XL_ERR_INSERT_RANGE_OVERLAP_ERR			0x0037 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010104="Error insertando rango Excel.\n \n.No se ha podido insertar el rango porque se ha producido un error eliminando el borde de las celdas. Debe consultar con el administrador."
#define	M4_XL_ERR_INSERT_RANGE_REMOVE_BORDER_ERR	0x0038 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010105="Error en interfaz Excel.\n \nNo se ha podido eliminar el rango de celdas porque las celdas indicadas no son válidas. Asegúrese de que que las coordenadas del rango sean correctas."
#define	M4_XL_ERR_DELETE_RANGE_BAD_CELL_COOR_		0x0039 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010106="Error eliminando fila Excel.\n \nSe ha producido el error "%0:s" al eliminar la fila "%1:d". Debe consultar con el administrador."
#define	M4_XL_ERR_DELETE_ROW_LIB_ERR				0x003A + M4_EXCEL_LIBXL_ERROR_BASE
// 11010107="Error eliminando columna Excel.\n \nSe ha producido el error "%0:s" al eliminar la columna "%1:d". Debe consultar con el administrador."
#define	M4_XL_ERR_DELETE_COL_LIB_ERR				0x003B + M4_EXCEL_LIBXL_ERROR_BASE
// 11010108="Error insertando rango Excel.\n \n.Se ha producido un error mientras se insertaba el rango de celdas especificado. Debe consultar con el administrador."
#define	M4_XL_ERR_INSERT_RANGE_GEN_ERR				0x003C + M4_EXCEL_LIBXL_ERROR_BASE
// 11010109="Error eliminando rango Excel.\n \n.Se ha producido un error mientras se eliminaba el rango de celdas especificado. Debe consultar con el administrador."
#define	M4_XL_ERR_DELETE_RANGE_GEN_ERR				0x003D + M4_EXCEL_LIBXL_ERROR_BASE
// 11010110="Error en interfaz Excel.\n \nNo se ha modificado el atributo número de formato de la celda porque el valor especificado no es válido o no está soportado. Debe indicar un índice de color válido."
#define	M4_XL_ERR_CHANGE_NUMBERFORMAT_BAD_ARG		0x003E + M4_EXCEL_LIBXL_ERROR_BASE
// 11010111="Error en interfaz Excel.\n \nNo se ha podido obtener el número de formato de la celda porque las coordenadas indicadas no son válidas. Asegúrese de que que las coordenadas de la celda sean números positivos."
#define	M4_XL_ERR_GET_NUMFORMAT_BAD_CELL_COOR_		0x003F + M4_EXCEL_LIBXL_ERROR_BASE
// 11010112="Error asignando nombre de hoja Excel.\n \nEl nombre especificado no es válido. Debe indicar un nombre para la hoja Excel no nulo."
#define	M4_XL_ERR_SET_WS_NAME_BAD_NAME_				0x0040 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010113="Error en interfaz Excel.\n \nNo se ha podido obtener o modificar el ancho de la columna, porque el índice indicado no es válido. Asegúrese de que que el índice de la fila o de la columna sean números positivos."
#define	M4_XL_ERR_GET_SET_COL_WIDTH_BAD_INDEX_		0x0041 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010114="Error en interfaz Excel.\n \nNSe ha producido el error "%0:s" al tratar de establecer el ancho de la columna "%1:d". Debe consultar con el administrador."
#define	M4_XL_ERR_SET_COL_WIDTH_LIB_ERR_DET			0x0042 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010115="Error en interfaz Excel.\n \nNo se ha podido eliminar el contenido del rango de celdas porque las coordenadas indicadas no son válidas. Asegúrese de que que la especificación del rango sea correcta."
#define	M4_XL_ERR_CLEAR_CONTENTS_BAD_CELL_COOR_		0x0043 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010116="Error eliminando contenido de rango Excel.\n \n.Se ha producido un error mientras se eliminaba el contenido del rango de celdas especificado. Debe consultar con el administrador."
#define	M4_XL_ERR_CLEAR_CONTENTS_GEN_ERR			0x0044 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010117="Error en interfaz Excel.\n \nNo se ha podido obtener o modificar la altura de la fila, porque el índice indicado no es válido. Asegúrese de que que el índice de la fila o de la columna sean números positivos."
#define	M4_XL_ERR_GET_SET_ROW_HEIGHT_BAD_INDEX_		0x0045 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010118="Error en interfaz Excel.\n \nNSe ha producido el error "%0:s" al tratar de establecer la altura de la fila "%1:d". Debe consultar con el administrador."
#define	M4_XL_ERR_SET_ROW_HEIGHT_LIB_ERR_DET		0x0046 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010119="Error en interfaz Excel.\n \nNo se ha podido obtener o asignar el comentario de la celda porque las coordenadas indicadas no son válidas. Asegúrese de que que las coordenadas de la celda sean números positivos."
#define	M4_XL_ERR_SET_COMMENT_BAD_CELL_COOR_		0x0047 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010120="Error en interfaz Excel.\n \nSe ha intentado asignar un comentario no válido en una celda. El comentario debe ser un texto no nulo."
#define	M4_XL_ERR_SET_COMMENT_BAD_COMMENT_			0x0048 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010121="Error en interfaz Excel.\n \nNo se ha modificado el atributo estilo de borde de la celda, porque el lado especificado (%0:d) no es correcto. Debe indicar un lado de borde válido."
#define	M4_XL_ERR_CHANGE_STYLE_BAD_BORDER_EDGE		0x0049 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010122="Error en interfaz Excel.\n \nNo se ha modificado el atributo color de borde de la celda, porque el lado especificado (%0:d) no es correcto. Debe indicar un lado de borde válido."
#define	M4_XL_ERR_CHANGE_COLOR_BAD_BORDER_EDGE		0x004A + M4_EXCEL_LIBXL_ERROR_BASE
// 11010123="Error en interfaz Excel.\n \nNo se han podido agrupar las celdas porque el rango indicado no es válido. Asegúrese de que que los índices de las filas o de la columnas sean números positivos."
#define	M4_XL_ERR_SET_MERGE_BAD_INDEXES_			0x004B + M4_EXCEL_LIBXL_ERROR_BASE
// 11010124="Error agrupando celdas Excel.\n \nSe ha producido el error "%0:s" agrupando las celdas definidas en el rango con celda de inicio "%1:d, "%2:d", celda de fin "%3:d, "%4:d". Debe consultar con el administrador."
#define	M4_XL_ERR_MERGE_CELLS_LIB_ERR				0x004C + M4_EXCEL_LIBXL_ERROR_BASE
// 11010125="Error desagrupando celdas Excel.\n \nSe ha producido el error "%0:s" desagrupando las celdas de "%1:d, "%2:d". Debe consultar con el administrador."
#define	M4_XL_ERR_UNMERGE_CELLS_LIB_ERR				0x004D + M4_EXCEL_LIBXL_ERROR_BASE
// 11010126="Error asignando celda.\n \nSe ha producido el error "%0:s" asignando una formula a la celda "%1:d, %2:d". Debe consultar con el administrador."
#define	M4_XL_ERR_SET_FORMULA_LIB_ERR				0x004E + M4_EXCEL_LIBXL_ERROR_BASE
// 11010127="Test unitario fallido: %0:s"
#define	M4_XL_UT_GEN_ERR							0x004F + M4_EXCEL_LIBXL_ERROR_BASE
// 11010128="Test unitario fallido: %0:s [%1:d]"
#define	M4_XL_UT_GEN_ERR_N1							0x0050 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010129="Test unitario fallido: %0:s [%1:d,%2:d]"
#define	M4_XL_UT_GEN_ERR_N2							0x0051 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010130="Error en interfaz Excel.\n \nNSe ha producido el error "%0:s" al tratar de recuperar el código de formato de la celda "%1:d", "%2:d". Debe consultar con el administrador."
#define	M4_XL_ERR_GET_NUM_FORMAT_LIB_ERR			0x0052 + M4_EXCEL_LIBXL_ERROR_BASE
// 11010131="Error en interfaz Excel.\n \nNo se ha modificado el atributo color de la celda porque el componente RGB del color (%0:d) no es válido. Debe indicar valores entre 0 y 255."
#define	M4_XL_ERR_CHANGE_STYLE_BAD_COLOR_RGB		0x0053 + M4_EXCEL_LIBXL_ERROR_BASE
#endif
