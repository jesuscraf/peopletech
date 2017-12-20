//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             engindll.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/11/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//  API de la DLL. 
//	El *.CPP es dependiente de plataforma. Si tenemos acceso a dll32.hpp, 
//podemos ampliar esta para que cubra los puntos de entrada de las DLLs, y sea
//multiplataforma para nosotros y para todo el mundo.
//
//
//==============================================================================

#ifndef __ENGINDLL__H__
#define __ENGINDLL__H__

#include "m4engine_dll.hpp"
class m4VariantType ;

class ClVMRunContext ;
class ClAccess ;

//Todas funciones exportadas no tienen el nombre decorado, y son accesibles desde VB, al usar
//	FAR PASCAL. ExecuteReport es llamada por canales, en un contexto dinamico en el que el
//  protocolo debe ser C.


enum ENGINE_DLL_INDEX_PARAM { 

	// Es una lista de comandos, exactamente la misma que si se teclease desde las linea de
	//	comandos. NOTAS:
	//	-El primer parametro es informativo y representa al llamdor de la funcion. Puede 
	//		ponerse lo que se quiera, pero debe de existir. Esta cadena se ve en el trace.
	//	-El ReportId y el ExecutionType pueden no ser necesarios segun el modo link que 
	//		usemos, pero deben de aparecer siempre. Si no se necesitan, puede ponerse 
	//		cualquier numero.
	//	Ejemplo de cadena de llamada valida:
	//		"CalledFromDLL 10 1 PCL /file:\\meta4fs\hpimasd_1 /debug:all:crazy"

	ENGINE_DLL_INDEX_COMMAND_STRING,
	
	// Valores: 0-Build. Un Manager se contruye y destruye en la llamada a la DLL,por lo 
	//				que cualquier canal enganchado a el tambien se destruira.
	//			1-Data. Se usa el manager del canal de datos (por lo que este debe pasarse
	//				via DataContext). El manager no se destruye, solo se usa.

	ENGINE_DLL_INDEX_LINK_MANAGER,	
	
	// Valores: 0-Build. El canal de diseño se levanta usando ID_REPORT e ID_EXECUTION_TYPE,
	//				se filtra, y se hace Load sobre el y se destruye todo al salir.
	//			1-LinkLoad. El canal de diseño ya esta creado y con los parametros 
	//				necesarios para hacer el Load ya cargados. Se filtra y se hace un Load.
	//				Al salir, ni se destruye ni se descarga nada sobre el.
	//				El canal se pasa via DESIGN_HANDLER. Debe estar levantado sobre el
	//				mismo ChannelManager que el canal de datos (este debe pasarse).
	//			2-Link. Exactamente igual que antes, solo que se supone que el Load ya 
	//				viene hecho desde el exterior: el canal se usa directamente. En el nodo 
	//				root ya debe estar seleccionado el report deseado.
	//			3-LinkLoadUnload. Como el modo LinkLoad, pero al salir se hace un Unload.
	//			4-LinkUnload. Como el modo Link, pero al salir se hace un Unload. 

	ENGINE_DLL_INDEX_LINK_DESIGN,	

	// Exactamente igual a ENGINE_DLL_INDEX_LINK_DESIGN, solo que para el canal de datos.
	// En los modos Link, este se pasa via DataContext. 

	ENGINE_DLL_INDEX_LINK_DATA,	
		
	// Opcional, handler de diseño (ver modos link).

	ENGINE_DLL_INDEX_DESIGN_HANDLER,

	// NO es un parametro. Es la cuenta del numero maximo de parametros

	ENGINE_DLL_INDEX_COUNT
} ;

// En ao_poReturn se devuelve un numero, que es :
//	1 si hay exito (0 errores, 0 warnings)
//  0 si hay errores (independientemente de los warning)
//  2 SOLO hay warnings (0 errores).
// El metodo en si da M4_SUCCESS ó M4CL_USER_ABORT_ALL_REQUEST si hay cancelacion. En este
//	caso, daria igual lo que vaya en ao_poReturn, pues el valor se limpiara.

extern "C" M4_DECL_M4ENGINE m4return_t ExecuteReport(
	m4VariantType * ai_poParam, m4int32_t ai_iNParam, 
	m4VariantType * ao_poReturn, ClVMRunContext * ai_poDataContext 
) ;

//Igual a la anterior, solo que los parametros se dan de forma convencional. De hecho, la
//	version anterior solo es un recubrimiento de esta funcion.
//Lo que se almacenaba en ao_poReturn, ahora es el resultado de la funcion.
//Segun los modos link,  ai_poVMAccess, ai_poDesignAccess y ai_poDataAccess pueden ser NULL.

extern "C" M4_DECL_M4ENGINE m4int16_t ExecuteReport2(
	const m4char_t * ai_pcCommandString, 
	m4int16_t ai_iLinkManagerMode, m4int16_t ai_iLinkDesignMode, m4int16_t ai_iLinkDataMode,
	ClAccess *ai_poVMAccess,		//Para LinkManger mode. ExecuteReport lo pone = ai_poDataAccess  
	ClAccess *ai_poDesignAccess,	//Sustituye al DesignHandler, hace lo mismo
	ClAccess *ai_poDataAccess,		//Sustituye al ai_poDataContext, hace lo mismo 

	//--->Los siguientes parametros no tienen parametros homologos en ExecuteReport

	m4bool_t *ai_bSwCancel, //Si se pasa, true si se cancelo el report
	
	//Opcional, si se pasa una cadena no "", se hace una comparacion de ficheros imagen
	//El resultado se deja en ao_bTestImageOk (si no es NULL).

	m4char_t *ai_pcImagePath, size_t ai_szImagePath,	
	m4char_t *ao_pcOutputFile, size_t ai_szOutputFile,     //Fichero de Salida
	m4bool_t ai_bFromWrapper
) ;

//---------------------------->Ciclo de llamadas especiales para secuenciar el visor.

// Debe llamarse 1 sola vez. 
// El tipo de salida debe ser, forzosamente, cualquiera DERIVADO de ClDvcFile. 
// Los parametros de entrada tienen el mismo significado que en ExecuteReport
// OutputName a la entrada tiene el tamaño maximo de la cadena, para testear que no se
//	sobreescriba, y a la salida tiene el strlen de la cadena. La cadena es el nombre
//	del fichero de salida.
// Se retorna 1 si todo va bien en la fase de diseño, o 0 en caso contrario, pero el
//  valor de retorno se refiere, exclusivamente, a la fase de diseño
// ViewerEnd debe ejecutarse SIEMPRE, haya habido o no errores previos.
// OutputName solo es valido si la funcion retorna true.
// Se genera un grupo, para que por lo menos haya algo que mostrar, y para poder obtener 
//	el nombre de fichero (los ficheros no se generan hasta que haya grupos). El parametro
//	ao_uiBreakCount corresponde a su homologo en ViewerBreakLoop, y ao_SwMoreBreaks al valor
//	devuelto en dicha funcion.

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI ViewerInitReport(
	m4uint32_t* ao_TaskId, m4char_t * ai_pcCommandStr, 
	m4int16_t ai_LinkManager, m4int16_t ai_LinkDesign, m4int16_t ai_LinkData,
	m4uint32_t ai_DesignHandler, m4uint32_t ai_DataHandler, 

	m4char_t *ao_pcOutputName, m4int16_t * ao_OutputSize, 
	m4int16_t *ao_ErrorCount, m4int16_t *ao_WarningCount,

	m4uint32_t *ao_uiBreakCount, m4int16_t *ao_SwMoreBreaks
) ;

// Debe llamarse 0 ó n veces, mientras devuelva true (1). 
// Si da false (0), bien por que haya acabado, o porque se haya producido un error, 
//	NO debe de llamarse mas.
// Nunca debe llamarse si el Init fue erroneo.
// En BreakCount da el maximo numero de break valido (completamente formado). 
// En el switch de error, un 1 es true y un 0 false. 

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI ViewerBreakLoop( m4uint32_t ai_TaskId,
										 m4uint32_t *ao_uiBreakCount, m4int16_t *ao_bSwError) ;

// Debe llamarse 1 sola vez, siempre, aunque el Init fuese erroneo.
// Devuelve true (1) si hubo errores, false (0) si no
// TraceName y TraceSize son lo mismo que ao_pcOutputName y ao_OutputSize, solo que el nombre 
//del fichero es el del trace. Siempre es valido, sea cual sea el valor MoveTrace usado.

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI ViewerEnd(m4uint32_t ai_TaskId, m4int16_t *ao_ErrorCount, m4int16_t *ao_WarningCount,
   	m4char_t *ao_pcTraceName, m4int16_t * ao_TraceSize ) ;

//Para compatibilidad binaria entre plataformas, rotan los bytes de entrada. 
extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI FlipInt(m4int16_t ai_iInt) ;
extern "C" M4_DECL_M4ENGINE m4int32_t WINAPI FlipLong(m4int32_t ai_iLong) ;

extern "C" M4_DECL_M4ENGINE  m4int16_t RD_OpenSession(m4int16_t ai_iVersion, m4char_t * ai_pcCommandStr, m4int16_t ai_LinkManager, ClAccess * ai_poAccess, m4int32_t * ao_SessionHandler);

//----------------------------------------Session Type: REPORT_DESIGNER

//VER DOCUMENTO SESIONES VB PARA USO DE ESTE INTERFACE

extern "C" M4_DECL_M4ENGINE void WINAPI RD_CloseSession(m4uint32_t ai_SessionHandler) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_LoadReportDesign( m4uint32_t ai_SessionHandler, m4uint32_t ai_DesignHandler ) ;

extern "C" M4_DECL_M4ENGINE void WINAPI RD_UnloadReportDesign(m4uint32_t ai_SessionHandler) ;


extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_GetCT_Props(
	m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId,
	m4char_t *ao_pcName,		m4uint32_t * ao_NameSize, 
	m4char_t *ao_pcTotList,		m4uint32_t * ao_TotListSize, 
	m4char_t *ao_pcTreeList,	m4uint32_t * ao_TreeListSize, 
	m4char_t *ao_pcFillTree,	m4uint32_t * ao_FillTreeSize, 
	m4int16_t *ao_piLevelCount, m4int16_t *ao_piMergerPage, 
	m4int32_t *ao_piColumnLeft, m4int32_t *ao_piColumnRight,
	m4int16_t *ao_poTogLevel,	m4int16_t *ao_poBreakLevel, 
	m4uint32_t *ao_piIdParamSec,m4int32_t *ao_piIdFirstCrossRow,
	m4char_t *ao_pcWizardData,		m4uint32_t * ao_WizardDataSize
) ;


extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_GetCT_CellProps(
	m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId, m4int16_t ai_Level, 
	m4int16_t ai_CellType,
	m4int16_t *ao_CellAlignY, 
	m4int16_t *ao_CellAlignX,
	m4int16_t *ao_piFieldCount,
	m4int32_t *ao_piWidth,  m4int32_t *ao_piHeight, 
	m4uint32_t *ao_puiIdFillColour,
	m4char_t *ao_pcFrame, m4uint32_t *aoi_puiFrameSize 
) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_GetCntProps(
	m4uint32_t ai_SessionHandler,
	m4uint32_t ai_uiIdSection, m4int16_t ai_iCntType,
	m4int16_t *ao_piPage,
	m4int32_t *ao_piTop, m4int32_t *ao_piLeft, m4int32_t *ao_piWidth, m4int32_t *ao_piHeight,
	m4int16_t *ao_piSwFixed, m4int16_t *ao_piSwTogether,
	m4int32_t *ao_piLowerLimit,
	m4char_t *ao_pcCancelCnt,	m4uint32_t * ao_CancelCntSize, 
	m4char_t *ao_pcCTMerge,		m4uint32_t * ao_CTMergeSize, 
	m4int16_t *ao_piFieldCount,
	m4uint32_t *ao_puiIdFillColour,
	m4char_t *ao_pcFrame, m4uint32_t *aoi_puiFrameSize,

	m4uint32_t *ao_puiTop, m4uint32_t *ao_puiLeft, 
	m4uint32_t *ao_puiWidth, m4uint32_t *ao_puiHeight,
	m4uint32_t *ao_puiLowerLimit,
	m4int16_t *ao_piSwTopLoaded, m4int16_t *ao_piSwLeftLoaded,
	m4int16_t *ao_piSwWidthLoaded, m4int16_t *ao_piSwHeightLoaded,
	m4int16_t *ao_piSwLowerLimitLoaded,

	m4char_t *ao_pcDefaultSerialOut, m4uint32_t *aoi_puiDefaultSerialOutSize 
) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_GetFieldProps(
	m4uint32_t ai_SessionHandler,
	m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
	m4int16_t ai_iFieldNumber, 
	m4char_t *ao_pcFieldName, m4uint32_t *aoi_puiFieldNameSize,
	m4int16_t *ao_piFieldType,
	m4int32_t *ao_piTop, m4int32_t *ao_piLeft, m4int32_t *ao_piWidth, m4int32_t *ao_piHeight,
	m4char_t *ao_pcSaveNode, m4uint32_t *aoi_puiSaveNodeSize,
	m4char_t *ao_pcSaveItem, m4uint32_t *aoi_puiSaveItemSize,
	m4int16_t *ao_piSwStatic, 
	m4char_t *ao_pcIdPI, m4uint32_t *aoi_puiIdPISize,
	m4int16_t *ao_piSwOptimize, 
	m4char_t *ao_pcExpr, m4uint32_t *aoi_puiExprSize,
	m4char_t *ao_pcLiteralValue, m4uint32_t *aoi_puiLiteralValueSize,
	
	m4int16_t *ao_piSwEmbebedRaster, 
	m4char_t *ao_pcBNRaster, m4uint32_t *aoi_puiBNRasterSize,
	m4char_t *ao_pcColourRaster, m4uint32_t *aoi_puiColourRasterSize,
	m4int32_t *ao_piInterline, m4int16_t *ao_piSwVarHeight, m4int16_t *ao_piSwJustified,

	m4int16_t *ao_piMaxBoxes, m4int16_t *ao_piFillMode, m4int16_t *ao_piFillChar,
	m4int16_t *ao_piSwDuplicated,
	m4char_t *ao_pcIdCTField, m4uint32_t *aoi_puiIdCTFieldSize,
	m4int16_t *ao_piBarCodeType,
	m4uint32_t *ao_uiIdFillColour, m4char_t *ao_pcFrame, m4uint32_t *ao_puiFrameSize,
	m4char_t *ao_pcIndexExpr, m4uint32_t *ao_puiIndexExprSize,
	m4int32_t *ao_piIndexLevel,
	m4char_t *ai_pcHyperlink, m4uint32_t *ao_puiHyperlinkSize,
	m4int16_t *ao_piTempWebImage,

	m4uint32_t *ao_puiTop, m4uint32_t *ao_puiLeft, 
	m4uint32_t *ao_puiWidth, m4uint32_t *ao_puiHeight,
	m4int16_t *ao_piSwTopLoaded, m4int16_t *ao_piSwLeftLoaded,
	m4int16_t *ao_piSwWidthLoaded, m4int16_t *ao_piSwHeightLoaded,

	m4char_t *ao_pcDefaultSerialOut, m4uint32_t *aoi_puiDefaultSerialOutSize 
) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_LoadCT_Sample(
	m4uint32_t ai_SessionHandler, m4char_t *ai_pcCTId, 
	m4char_t * ai_pcLevelInfo, m4bool_t ai_bSwIncr,
	m4int16_t ai_iTogLevel, m4bool_t ai_bSwAllCnt,
	m4char_t *ao_pcSerialOut, m4uint32_t *aoi_puiSerialOutSize 
) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_SetCT_Props(
	m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId,
	m4int16_t ai_iMergePage, m4double_t ai_dColumnLeft, m4double_t ai_dColumnRight,
	m4int16_t ai_iTogLevel, m4char_t *ai_pcWizardData
) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_SetWizardData(
	m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId, m4char_t *ai_pcWizardData
) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_SetCT_CellProps(
	m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId, m4int16_t ai_Level, 
	/*ENG_CT_CELL_TYPE*/ m4int16_t ai_CellType,
	/*ENG_CT_CELL_Y_ALIGN*/m4int16_t ai_CellAlignY, 
	/*ENG_CT_CELL_X_ALIGN*/m4int16_t ai_CellAlignX,
	m4double_t ai_dWidth,  m4double_t ai_dHeight, 
	m4uint32_t ai_uiIdFillColour, m4char_t *ai_pcFrame 
) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_SetFieldProps(
	m4uint32_t ai_SessionHandler,
	m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
	m4int16_t ai_iFieldNumber, 
	m4char_t *ai_pcFieldName, m4int16_t ai_iFieldType,
	m4double_t ai_dTop, m4double_t ai_dLeft, m4double_t ai_dWidth, m4double_t ai_dHeight,
	m4char_t *ai_pcSaveNode, m4char_t *ai_pcSaveItem, m4int16_t ai_iSwStatic, 
	m4char_t *ai_pcIdPI, m4int16_t ai_iSwOptimize, 
	m4char_t *ai_pcExpr, m4char_t *ai_pcLiteralValue, 

	m4int16_t ai_iSwEmbebedRaster, 
	m4char_t *ai_pcBNRaster, m4char_t *ai_pcColourRaster, 
	m4double_t ai_iInterline, m4int16_t ai_iSwVarHeight, m4int16_t ai_iSwJustified,

	m4int16_t ai_iMaxBoxes, m4int16_t ai_iFillMode, m4int16_t ai_iFillChar,
	m4int16_t ai_iSwDuplicated, m4char_t *ai_pcIdCTField, m4int16_t ai_iBarCodeType,
	m4uint32_t ai_uiIdFillColour, m4char_t *ai_pcFrame, 
	m4char_t *ai_pcIndexExpr, m4int32_t ai_iIndexLevel,
	m4char_t *ai_pcHyperlink, m4int16_t ai_iTempWebImage,

	m4char_t *ai_pcDefaultSerialIn 
) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_DeleteField(
	m4uint32_t ai_SessionHandler,
	m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
	m4int16_t ai_iFieldNumber 
) ;


extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_SetCntProps(
	m4uint32_t ai_SessionHandler,
	m4uint32_t ai_uiIdSection, m4int16_t ai_CntType,
	m4int16_t ai_iPage,
	m4double_t ai_dTop, m4double_t ai_dLeft, m4double_t ai_dWidth, m4double_t ai_dHeight,
	m4int16_t ai_iSwFixed, m4int16_t ai_iSwTogether,
	m4double_t ai_dLowerLimit,
	m4char_t *ai_pcCancelCnt,	 
	m4char_t *ai_pcCTMerge,
	m4uint32_t ai_uiIdFillColour,
	m4char_t *ai_pcFrame, 

	m4char_t *ai_pcDefaultSerialIn 
) ;

extern "C" M4_DECL_M4ENGINE m4uint32_t WINAPI RD_CountFontInstances(
	m4uint32_t ai_SessionHandler, m4uint32_t ai_uiIdFont
) ;

extern "C" M4_DECL_M4ENGINE m4uint32_t WINAPI RD_CountFormatInstances(
	m4uint32_t ai_SessionHandler, m4uint32_t ai_uiIdFormat
) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_DeleteCnt(
	m4uint32_t ai_SessionHandler,
	m4uint32_t ai_uiIdSection, m4int16_t ai_CntType
) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_DeleteCT_Cell(
	m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId, m4int16_t ai_Level, m4int16_t ai_CellType
) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_AddCT_Cell(
	m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId, m4int16_t ai_Level, 
	/*ENG_CT_CELL_TYPE*/ m4int16_t ai_CellType,
	/*ENG_CT_CELL_Y_ALIGN*/m4int16_t ai_CellAlignY, 
	/*ENG_CT_CELL_X_ALIGN*/m4int16_t ai_CellAlignX,
	m4double_t ai_dWidth,  m4double_t ai_dHeight, 
	m4uint32_t ai_uiIdFillColour, m4char_t *ai_pcFrame 
) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_SynchronizeCT( m4uint32_t ai_SessionHandler, m4char_t *ai_pcCTId ) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_IncrCTHeightLevel(m4uint32_t ai_SessionHandler, m4char_t *ai_pcCTId, m4int16_t ai_Level, m4double_t ai_dHeightIncrement) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_GenerateNewCTIdField(m4uint32_t ai_SessionHandler,  
	m4char_t *ai_pcCTId, m4char_t *ao_pcKey, m4uint32_t *aoi_puiKeySize) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_EnsureFont(m4uint32_t ai_SessionHandler, m4uint32_t ai_uiIdFont ) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_EnsureFormat(m4uint32_t ai_SessionHandler, m4uint32_t ai_uiIdFormat ) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_EnsureColour(m4uint32_t ai_SessionHandler, m4uint32_t ai_uiIdColor ) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_ClearFontsNotUsed(m4uint32_t ai_SessionHandler ) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_ClearFormatsNotUsed(m4uint32_t ai_SessionHandler ) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_ClearColoursNotUsed(m4uint32_t ai_SessionHandler ) ;

extern "C" M4_DECL_M4ENGINE void WINAPI RD_SynchronizeFontsUsedByReport(m4uint32_t ai_SessionHandler ) ;

extern "C" M4_DECL_M4ENGINE void WINAPI RD_SynchronizeFormatsUsedByReport(m4uint32_t ai_SessionHandler ) ;

extern "C" M4_DECL_M4ENGINE void WINAPI RD_SynchronizeColoursUsedByReport(m4uint32_t ai_SessionHandler ) ;

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_GetCT_WizardData(m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId, m4char_t *ao_pcSerialOut, m4uint32_t *aoi_puiSerialOutSize) ;

//Da false (0) si hay errores (pe, no exite fichero o formato desconocido).
//fileFormat es 0-BMP, 1-GIF, 2-JPG, 200-UNKNOWN
//bpp=bits por pixel (1-monocromo, 8-256 colores, 24-millones de colores, etc).
//PARCHEADO: Para JPEG valida que sea un fichero JPEG, pero algunas veces no lee ni 
//	el ancho/alto/ ni bpp. Si bpp es -1, es que no lo leyó.

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI RD_GetGraphicFileInfo(
	m4int32_t ai_SessionHandler,
	m4char_t *ai_pcFileName, m4int16_t *ao_FileFormat,
	m4uint32_t *ao_PixelWidth, m4uint32_t *ao_PixelHeight, m4int16_t *ao_bpp ) ;


//----------------------------------------USO EXCLUSIVO PARA TESTEOS AUXILIARES

// Da 1 si tiene exito

extern "C" M4_DECL_M4ENGINE m4int16_t WINAPI TestAux( m4uint32_t ai_TaskId, m4char_t *ai_pcStr, m4char_t *ao_pcTraceName, m4int16_t * ao_TraceSize ) ;

#endif

