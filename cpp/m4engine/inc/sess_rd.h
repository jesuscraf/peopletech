//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             sess_rd.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    Nucleo funcional de los reports
//
//
//==============================================================================

#ifndef __SESS_RD__H__
#define __SESS_RD__H__

#include "designdd.h"
#include "designdt.h"
#include "ddlf_chn.h"
#include "vbserial.h"

//====================================================================ClENG_SessReportDesigner

class ClENG_SessReportDesigner
{
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	//-------------------------Miembros-------------------------------------------------

	ClENG_DDLF_Chn		m_oDesignChn ;	
	ClENG_DesignData	m_oDD ;
	ClENG_DesignTree	m_oDT ;	

MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_SessReportDesigner();
	virtual ~ClENG_SessReportDesigner() { ClENG_SessReportDesigner::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_SessReportDesigner) ; }
	virtual void End() ;	//Descarga todo

	//NOTA general: esta clase cubre el intereface de VB. 
	//SIEMPRE testea los valores enum que recibe son correctos.

	// Engancha el DesignChn,  carga el DD y el DT.

	m4bool_t Init(
		m4char_t *ai_pcReportId, m4int16_t ai_iExecutionType,
		ENG_LINK_DESIGN_MODE ai_LinkDesign, ClAccess *ai_poAccess=NULL,
		ClMIT_Chn::AUTOLOAD_MODE ai_AutoLoadMode=ClMIT_Chn::AUTOLOAD_OFF,
		m4bool_t ai_bPack=M4_FALSE,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;

	//-------------------------API, gets-----------------------------------------------------
	//Se lee siempre del DT

	m4bool_t GetCT_Props(
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

	m4bool_t GetCT_CellProps(
		m4char_t *ai_pcCTId, m4int16_t ai_Level, 
		/*ENG_CT_CELL_TYPE*/ m4int16_t ai_CellType,
		/*ENG_CT_CELL_Y_ALIGN*/m4int16_t *ao_CellAlignY, 
		/*ENG_CT_CELL_X_ALIGN*/m4int16_t *ao_CellAlignX,
		m4int16_t *ao_piFieldCount,
		m4int32_t *ao_piWidth,  m4int32_t *ao_piHeight, 
		m4uint32_t *ao_puiIdFillColour,
		m4char_t *ao_pcFrame, m4uint32_t *aoi_puiFrameSize
	) ;

	m4bool_t GetCntProps(
		m4uint32_t ai_uiIdSection, ENG_TYPE_CONTAINER ai_CntType,
		m4int16_t *ao_piPage,
		m4int32_t *ao_piTop, m4int32_t *ao_piLeft, m4int32_t *ao_piWidth, m4int32_t *ao_piHeight,
		m4int16_t *ao_piSwFixed, m4int16_t *ao_piSwTogether,
		m4int32_t *ao_piLowerLimit,
		m4char_t *ao_pcCancelCnt,	m4uint32_t * ao_CancelCntSize, 
		m4char_t *ao_pcCTMerge,		m4uint32_t * ao_CTMergeSize, 
		m4int16_t *ao_piFieldCount,
		m4uint32_t *ao_puiIdFillColour,
		m4char_t *ao_pcFrame, m4uint32_t *aoi_puiFrameSize,

		//Datos Param auxiliares

		m4uint32_t *ao_puiTop, m4uint32_t *ao_puiLeft, 
		m4uint32_t *ao_puiWidth, m4uint32_t *ao_puiHeight,
		m4uint32_t *ao_puiLowerLimit,
		m4int16_t *ao_piSwTopLoaded, m4int16_t *ao_piSwLeftLoaded,
		m4int16_t *ao_piSwWidthLoaded, m4int16_t *ao_piSwHeightLoaded,
		m4int16_t *ao_piSwLowerLimitLoaded,

		//Defectos
		m4char_t *ao_pcDefaultSerialOut, m4uint32_t *aoi_puiDefaultSerialOutSize 
	) ;

	m4bool_t GetFieldProps(
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

		//Datos Param auxiliares

		m4uint32_t *ao_puiTop, m4uint32_t *ao_puiLeft, 
		m4uint32_t *ao_puiWidth, m4uint32_t *ao_puiHeight,
		m4int16_t *ao_piSwTopLoaded, m4int16_t *ao_piSwLeftLoaded,
		m4int16_t *ao_piSwWidthLoaded, m4int16_t *ao_piSwHeightLoaded,

		//Defectos

		m4char_t *ao_pcDefaultSerialOut, m4uint32_t *aoi_puiDefaultSerialOutSize 
	) ;

	m4uint32_t CountFontInstances( m4uint32_t ai_uiIdFont ) ;

	m4uint32_t CountFormatInstances( m4uint32_t ai_uiIdFormat ) ;

	//-------------------------API, Sets-----------------------------------------------------
	//Se graba siempre en DD, y se vuelve a recalcular todo el DT
	//Si hay error, se dejan los datos tal cual estaban antes, y se vuelve a recalcular todo para
	//	dejarlo correcto.

	m4bool_t SetCT_Props(
		m4char_t *ai_pcCTId,
		m4int16_t ai_iMergePage, m4double_t ai_dColumnLeft, m4double_t ai_dColumnRight,
		m4int16_t ai_iTogLevel,
		m4char_t *ai_pcWizardData
	) ;

	m4bool_t SetWizardData(
		m4char_t *ai_pcCTId, m4char_t *ai_pcWizardData
	) ;

	m4bool_t SetCT_CellProps(
		m4char_t *ai_pcCTId, m4int16_t ai_Level, 
		/*ENG_CT_CELL_TYPE*/ m4int16_t ai_CellType,
		/*ENG_CT_CELL_Y_ALIGN*/m4int16_t ai_CellAlignY, 
		/*ENG_CT_CELL_X_ALIGN*/m4int16_t ai_CellAlignX,
		m4double_t ai_dWidth,  m4double_t ai_dHeight, 
		m4uint32_t ai_uiIdFillColour, m4char_t *ai_pcFrame 
	) ;

	m4bool_t SetFieldProps(
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

		//Defectos
		m4char_t *ai_pcDefaultSerialIn 

	) ;

	m4bool_t AddField(
		m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
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

		//Defectos
		m4char_t *ai_pcDefaultSerialIn 
	) ;

	//Si el field es de template, borrará referencias al field en el arbol de secciones.

	m4bool_t DeleteField(
		m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
		m4int16_t ai_iFieldNumber
	) ;
	
	
	m4bool_t SetCntProps(
		m4uint32_t ai_uiIdSection, /*ENG_TYPE_CONTAINER*/ m4int16_t ai_CntType,
		m4int16_t ai_iPage,
		m4double_t ai_dTop, m4double_t ai_dLeft, m4double_t ai_dWidth, m4double_t ai_dHeight,
		m4int16_t ai_iSwFixed, m4int16_t ai_iSwTogether,
		m4double_t ai_dLowerLimit,
		m4char_t *ai_pcCancelCnt,	 
		m4char_t *ai_pcCTMerge,
		m4uint32_t ai_uiIdFillColour,
		m4char_t *ai_pcFrame, 

		//Defectos
		m4char_t *ai_pcDefaultSerialIn 
	) ;

	m4bool_t DeleteCnt(
		m4uint32_t ai_uiIdSection, /*ENG_TYPE_CONTAINER*/ m4int16_t ai_CntType
	) ;

	//Borra fields referemciados en el resto del report

	m4bool_t DeleteCT_Cell(
		m4char_t *ai_pcCTId, m4int16_t ai_Level, /*ENG_CT_CELL_TYPE*/ m4int16_t ai_CellType
	) ;
	
	m4bool_t AddCT_Cell(
		m4char_t *ai_pcCTId, m4int16_t ai_Level, 
		/*ENG_CT_CELL_TYPE*/ m4int16_t ai_CellType,
		/*ENG_CT_CELL_Y_ALIGN*/m4int16_t ai_CellAlignY, 
		/*ENG_CT_CELL_X_ALIGN*/m4int16_t ai_CellAlignX,
		m4double_t ai_dWidth,  m4double_t ai_dHeight, 
		m4uint32_t ai_uiIdFillColour, m4char_t *ai_pcFrame 
	) ;

	//-------------------------API, sample y otros-------------------------------------------

	m4bool_t LoadCT_Sample(
		m4char_t *ai_pcCTId, 
		m4char_t * ai_pcLevelInfo, m4bool_t ai_bSwIncr,
		m4int16_t ai_iTogLevel, m4bool_t ai_bSwAllCnt,
		m4char_t *ao_pcSerialOut, m4uint32_t *aoi_puiSerialOutSize
	) ;

	m4bool_t SynchronizeCT(m4char_t *ai_pcCTId) ; 
	void  SynchronizeFontsUsedByReport() ;
	void  SynchronizeFormatsUsedByReport() ;
	void  SynchronizeColoursUsedByReport() ;

	m4bool_t IncrCTHeightLevel(m4char_t *ai_pcCTId,m4int16_t ai_Level, m4double_t ai_dHeightIncrement) ;

	m4bool_t GenerateNewCTIdField(m4char_t *ai_pcCTId,
		m4char_t *ao_pcKey, m4uint32_t *aoi_puiKeySize) ;

	m4bool_t EnsureFont(m4uint32_t ai_uiIdFont) ;
	m4bool_t EnsureFormat(m4uint32_t ai_uiIdFormat) ;
	m4bool_t EnsureColour(m4uint32_t ai_uiIdColour) ;

	//Borran las Fonts y Formatos no usados. Implica la recarga de DT si hay cambios.

	m4bool_t ClearFontsNotUsed() ;
	m4bool_t ClearFormatsNotUsed() ;
	m4bool_t ClearColoursNotUsed() ;

	m4bool_t GetCT_WizardData( m4char_t *ai_pcCTId, m4char_t *ao_pcSerialOut, m4uint32_t *aoi_puiSerialOutSize) ;

MIT_PRIVATE:

	//Auxiliar recursiva

	typedef map<ClMIT_Str,m4int32_t, less<ClMIT_Str> > TotalMap_t;	//Clave IdTot:IdFunc
																	//Clave[long]=':' 
	
	void GetCT_WizardDataTotalize( TotalMap_t *ao_poMap, 
		m4char_t *ai_pcCTId, StENG_DT_SectionData *ai_poSection ) ;

	//Auxiliar recursiva
	
	void GetCT_WizardDataRow( ClENG_VB_Serialize *ao_poSerial, 
		m4char_t *ai_pcCTId, StENG_DT_SectionData *ai_poSection ) ;

	//NULL y error si no encuentra, Si se pasa ao_poCT, también da este.
	StENG_DT_CTCell *FindCT_T_CellInDT(
		m4char_t *ai_pcCTId, m4int16_t ai_Level, ENG_CT_CELL_TYPE ai_CellType,
		StENG_DT_CTData **ao_poCT=NULL) ;

	//NULL y error si no encuentra, Si se pasa ao_poCT, también da este.
	StENG_DD_CTCell *FindCT_T_CellInDD(
		m4char_t *ai_pcCTId, m4int16_t ai_Level, ENG_CT_CELL_TYPE ai_CellType,
		StENG_DD_CTData **ao_poCT=NULL, StENG_DD_CTData::CellList_t::iterator *ao_It=NULL) ;

	//NULL y error si no encuentra. Testea validez de ai_Owner.

	StENG_DT_FieldData *FindFieldInDT(
		m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
		m4int16_t ai_iFieldNumber ) ;
	
	//NULL y error si no encuentra, usa FindFieldListInDD. 

	StENG_DD_FieldData *FindFieldInDD(
		m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
		m4int16_t ai_iFieldNumber, 
		ENG_DD_FieldList_t **ao_poList=NULL,
		ENG_DD_FieldList_t::iterator *ao_poIt=NULL  ) ;

	//NULL y error si no encuentra. Testea validez de ai_Owner.

	ENG_DD_FieldList_t * FindFieldListInDD(
		m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2 ) ;

	m4bool_t GetDefaultValues(
		StENG_DT_Defaults *ai_poDefaults,
		m4char_t *ao_pcDefaultSerialOut, m4uint32_t *aoi_puiDefaultSerialOutSize 
	) ;


	void SetDefaultValues(
		StENG_DD_Defaults *ao_poDefaults,
		m4char_t *ai_pcDefaultSerialIn 
	) ;


};

#endif


