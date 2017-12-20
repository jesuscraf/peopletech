//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             designdd.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:8/01/98
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//
//
//==============================================================================

#ifndef __DESIGNDD__H__
#define __DESIGNDD__H__

/*

ClENG_DesignData y asociadas.
  
Contiene un conjunto de LISTAS de datos de diseño sin validar.
Estas listas pueden ser cargadas y mantenidas desde varias fuentes de datos: canal de diseño, 
peticiones VB, etc. 
Esta carga puede ser mixta, pe, un diseño puede cargarse inicialmente desde canal y modificarse 
desde VB.
A partir de un objeto de este tipo -da igual de donde se cargaron los datos- se generará un 
DesignTree validado.

*/

#include "designin.h"

#include "core.h"
#include "corevr.h"
#include "clreport.h"
#include "pageprn.h"

#include "m4stl.hpp"

struct StENG_DD_CTData;

//==============================================================>Estructuras de datos a cargar
//
//	Valido para todas las estructuras:
//
//  -A la hora de ser rellenadas, solamente para los datos que sean cargados debe ponerse  
//		m_Sw_xxxx_Loaded a TRUE y cargar el valor correspondiente.
//	-El constructor simplemente pone todos los m_Sw_xxxx_Loaded a FALSE.
//  -Cuando se indican los test a realizar en los valores, es para información. Esta capa pasa
//		absolutamente TODOS los tests cuando proceda. Las clases que rellenen datos en estas 
//		estructuras no tienen que preocuparse de validar nada.
//	-Donde haya estructuras homologas en DT, ahí se comentarán defectos y otros valores.


struct StENG_DD_Defaults {

	// Identificador (no indice) de la font.
	m4uint32_t m_uiIdFont ;			m4bool_t m_SwIdFontLoaded ;

	// Identificador (no indice) del formato o ClENG_Format::ID_NULL si no hay.
	m4uint32_t m_uiIdFormat ;		m4bool_t m_SwIdFormatLoaded ;	

	// TEST:enum
	ClENG_FieldStr::ALIGN m_AlignY ;	m4bool_t m_SwAlignYLoaded ;	

	// TEST:enum
	ClENG_FieldStr::ALIGN m_AlignX ;	m4bool_t m_SwAlignXLoaded ;	

	m4bool_t	m_bSwClip ;			m4bool_t m_SwClipLoaded ;

	// Switch de lectura de Path de graficos en Registry o no
	m4bool_t	m_bSwGraphics ;		m4bool_t m_SwGraphicsLoaded ;

	// Comportamiento al formatear NULLs en fields
	m4bool_t m_bSwFormatNULL;		m4bool_t m_SwFormatNULLLoaded ;

	//Cadena de valores NULL imprimibles
	ClMIT_Str m_oDefDisplayNULL	 ;	m4bool_t m_SwDefDisplayNULLLoaded ;

	//Id del Colour (def 0)
	m4uint32_t m_uiIdColour ;		m4bool_t m_SwIdColourLoaded ;

	//-------------->Constructores y sobrecargas para STL

	StENG_DD_Defaults(const StENG_DD_Defaults& ai_oSource) 
		{ Init(&ai_oSource) ; } 
	StENG_DD_Defaults & operator = (const StENG_DD_Defaults &ai_oOpe) 
		{ Init(&ai_oOpe) ; return *this ;} 

	StENG_DD_Defaults() { Reset(); }
	void Reset() ;

	void Init(const StENG_DD_Defaults *ai_poSource) ;


} ;


struct StENG_DD_GlobalReport {

	ClMIT_Str m_oReportName ;					m4bool_t m_bReportNameLoaded ;

	ClMIT_Str  m_oDataChn;						m4bool_t m_bDataChnLoaded ;

	ClMIT_Str  m_oDataChnReprinter ;			m4bool_t m_bDataChnReprinterLoaded ;

	ENG_MODE_METRIC m_ModeMetric ;				m4bool_t m_bModeMetricLoaded ;

	m4int16_t m_iIdRootSection ;				m4bool_t m_bIdRootSectionLoaded ;

	m4bool_t  m_bSwRootSync ;					m4bool_t m_bSwRootSyncLoaded ;	

	ClENG_Report::BREAK_MODE m_BreakMode ;		m4bool_t m_bBreakModeLoaded ;

	m4int16_t m_iIdBreakSection ;				m4bool_t m_bIdBreakSectionLoaded ;

	ClMIT_Str m_oTotalizeList ;					m4bool_t m_bTotalizeListLoaded ;

	ClMIT_Str m_oCTList ;						m4bool_t m_bCTListLoaded ;

	ClMIT_Str m_oExecutionPars ;				m4bool_t m_bExecutionParsLoaded ;

	m4bool_t m_bIgnoreTxtWarning;				m4bool_t m_bIgnoreTxtWarningLoaded ;


	StENG_DD_Defaults m_oReportDefaults ;

	StFileParams m_stDefaultFileParams ;

	//-------------->Constructores y sobrecargas para STL

	StENG_DD_GlobalReport(const StENG_DD_GlobalReport& ai_oSource) 
		{ Init(&ai_oSource) ; } 
	StENG_DD_GlobalReport & operator = (const StENG_DD_GlobalReport &ai_oOpe)
		{ Init(&ai_oOpe) ; return *this ;} 

	StENG_DD_GlobalReport() { Reset() ; }
	void Reset() ;	//Limpia los flags de cargado

	void Init( const StENG_DD_GlobalReport *ai_poSource ) ;
}  ;	

struct StENG_DD_Colour { 

	//O
	m4uint32_t m_uiIdColour ;		m4bool_t m_bIdColourLoaded ;
	
	//O
	ClMIT_Str m_oName ;				m4bool_t m_bNameLoaded ;

	//O
	m4uint8_t  m_R, m_G, m_B ;		m4bool_t m_b_R_Loaded, m_b_G_Loaded, m_b_B_Loaded ;

	//-------------->Constructores y sobrecargas para STL

	StENG_DD_Colour(const StENG_DD_Colour& ai_oSource) 
		{ Init(&ai_oSource) ; } 
	StENG_DD_Colour & operator = (const StENG_DD_Colour &ai_oOpe)
		{ Init(&ai_oOpe) ; return *this ;} 
	m4bool_t operator == (const StENG_DD_Colour & ai_oOpe) const
		{ return M4_BOOL(m_uiIdColour==ai_oOpe.m_uiIdColour); }
	m4bool_t operator < (const StENG_DD_Colour & ai_oOpe) const
		{ return M4_BOOL(m_uiIdColour<ai_oOpe.m_uiIdColour); }

	StENG_DD_Colour() { Reset() ; }
	void Reset() ;	//Limpia los flags de cargado

	void Init( const StENG_DD_Colour *ai_poSource ) ;

} ;

struct StENG_DD_FontData {
	
	//O
	m4uint32_t m_uiIdFont ;						m4bool_t m_bIdFontLoaded ;

	//O
	Point_t m_DesignHeight ;					m4bool_t m_bDesignHeightLoaded ;

	//O
	m4bool_t m_bFixedSpace ;					m4bool_t m_bFixedSpaceLoaded ;
	
	//O
	ClMIT_Str m_oRFNFile ;						m4bool_t m_bRFNFileLoaded ;
		
	//O
	ClMIT_Str m_oTraceName ;					m4bool_t m_bTraceNameLoaded ;

	//D=False.
	m4bool_t m_bUnderline, m_bBold, m_bItalic ; 	m4bool_t m_bUnderlineLoaded, 
															m_bBoldLoaded, m_bItalicLoaded ;

	//-------------->Constructores y sobrecargas para STL

	StENG_DD_FontData(const StENG_DD_FontData& ai_oSource) 
		{ Init(&ai_oSource) ; } 
	StENG_DD_FontData & operator = (const StENG_DD_FontData &ai_oOpe)
		{ Init(&ai_oOpe) ; return *this ;} 
	m4bool_t operator == (const StENG_DD_FontData & ai_oOpe) const
		{ return M4_BOOL(m_uiIdFont==ai_oOpe.m_uiIdFont); }
	m4bool_t operator < (const StENG_DD_FontData & ai_oOpe) const
		{ return M4_BOOL(m_uiIdFont<ai_oOpe.m_uiIdFont); }

	StENG_DD_FontData() { Reset() ; }
	void Reset() ;	//Limpia los flags de cargado

	void Init( const StENG_DD_FontData *ai_poSource ) ;

} ;

struct StENG_DD_FormatData {

	//O
	m4uint32_t     m_uiIdFormat ;		m4bool_t m_bIdFormatLoaded ;

	//O
	ClMIT_Str m_oFormatName ;			m4bool_t m_bFormatNameLoaded ;

	//D:""
	ClMIT_Str m_oIdCurrency;			m4bool_t m_bIdCurrencyLoaded ;


	//Cargar lista de properties para el formato
	//El nombre de propertie es el reconocido por la m4fmt32

	struct StENG_DD_Popertie {
		ClMIT_Str m_oName, m_oValue ;	
		//-----------Sobrecargas para usar el objeto con STL
		m4bool_t operator == (const StENG_DD_Popertie &ai_oOpe) const 
			{ return M4_BOOL(m_oValue==ai_oOpe.m_oValue); }
		m4bool_t operator < (const StENG_DD_Popertie &ai_oOpe) const 
			{ return M4_BOOL(m_oValue==ai_oOpe.m_oValue); }
	} ;

	typedef list<StENG_DD_Popertie> PropertieList_t;	
	PropertieList_t m_oProperties ;

	
	//-------------->Constructores y sobrecargas para STL

	StENG_DD_FormatData(const StENG_DD_FormatData& ai_oSource) 
		{ Init(&ai_oSource) ; } 
	StENG_DD_FormatData & operator = (const StENG_DD_FormatData &ai_oOpe)
		{ Init(&ai_oOpe) ; return *this ;} 
	m4bool_t operator == (const StENG_DD_FormatData & ai_oOpe) const
		{ return M4_BOOL(m_uiIdFormat==ai_oOpe.m_uiIdFormat); }
	m4bool_t operator < (const StENG_DD_FormatData & ai_oOpe) const
		{ return M4_BOOL(m_uiIdFormat<ai_oOpe.m_uiIdFormat); }
	
	StENG_DD_FormatData() { Reset() ; }
	void Reset() ;	//Limpia los flags de cargado y limpia la lista de Properties

	void Init( const StENG_DD_FormatData *ai_poSource ) ;
} ;


struct StENG_DD_SonSectionData {

	m4uint32_t m_uiIdSonSection;		m4bool_t m_bIdSonSectionLoaded ;

	m4int16_t  m_iExeOrder ;			m4bool_t m_bExeOrderLoaded ;

	m4bool_t   m_bIsSync ;				m4bool_t   m_bIsSyncLoaded ;

	//-------------->Constructores y sobrecargas para STL

	StENG_DD_SonSectionData(const StENG_DD_SonSectionData& ai_oSource) 
		{ Init(&ai_oSource) ; } 
	StENG_DD_SonSectionData & operator = (const StENG_DD_SonSectionData &ai_oOpe)
		{ Init(&ai_oOpe) ; return *this ;} 
	m4bool_t operator == (const StENG_DD_SonSectionData & ai_oOpe) const
		{ return M4_BOOL(m_uiIdSonSection==ai_oOpe.m_uiIdSonSection); }
	m4bool_t operator < (const StENG_DD_SonSectionData & ai_oOpe) const
		{ return M4_BOOL(m_uiIdSonSection<ai_oOpe.m_uiIdSonSection); }

	StENG_DD_SonSectionData() { Reset() ; }
	void Reset() ;	//Limpia los flags de cargado

	void Init( const StENG_DD_SonSectionData *ai_poSource ) ;
} ;

struct StENG_DD_FieldData {

	m4uint32_t m_uiIdField ;					m4bool_t m_bIdFieldLoaded ;
	
	ClMIT_Str m_oFieldName ;					m4bool_t m_bFieldNameLoaded ;
		
	ENG_FIELD_TYPE   m_FldType ;				m4bool_t m_bFldTypeLoaded ;

	m4double_t   m_dTop, m_dLeft ;				m4bool_t m_bTopLoaded, m_bLeftLoaded ; 

	m4double_t m_dWidth;						m4bool_t m_bWidthLoaded ;

	m4double_t m_dHeight ;						m4bool_t m_bHeightLoaded ;

	m4double_t m_dX1, m_dX2, m_dY1, m_dY2 ;		m4bool_t m_bX1Loaded, m_bX2Loaded, 
														m_bY1Loaded, m_bY2Loaded ;
	
	ClMIT_Str  m_oSaveNode ;					m4bool_t m_bSaveNodeLoaded ;

	ClMIT_Str  m_oSaveItemName ;				m4bool_t m_bSaveItemNameLoaded ;

	m4bool_t m_bSwStatic ;						m4bool_t m_bSwStaticLoaded ;

	ClMIT_Str m_oIdPi;							m4bool_t m_bIdPiLoaded ;

	m4bool_t  m_bSwOptimize ;					m4bool_t  m_bSwOptimizeLoaded ;

	ClMIT_Str m_oExpr, m_oLiteralValue ;		m4bool_t m_bExprLoaded, m_bLiteralValueLoaded ;

	ClMIT_Str m_oIndexExpr;						m4bool_t m_bIndexExprLoaded;

	m4int32_t m_iIndexLevel;					m4bool_t m_bIndexLevelLoaded;

	//SOLO PARA HTML - Hiperenlaces
	ClMIT_Str m_oHyperlink;						m4bool_t m_bHyperlinkLoaded;

	//[m_FldType=FLD_RASTERG]D=M4_FALSE
	//	ATENCION!!!!, caso especial a tratar en servidores de DD:
	//  Solo para tipos FLD_RASTERG, debe testearse si hay uno o varios graficos incrustados
	//en el diseño. Si es asi, se pone este flag a TRUE y se rellenan las dos siguientes
	//cadenas con el nombre del fichero BN (si hay) y el de color (si hay). Si procede, 
	//el servidor de DD DECLARA INTERES SOBRE LOS FICHEROS EN DD.
	//  Cuando el DD se destruye, se liberan todos los intereses declarados hasta el momento.
	//	Cuando se inicializa un DT a través de un DD, este hereda los intereses del DD, y 
	//declara a su vez interes sobre ellos. Este interes se libera al destruir ó reinicializar
	//el DT.
	//  Cuando se cree un report a través de un DT, el report hereda en conjunto todos
	//los intereses del DT, y tambien declara interes sobre ellos, y los libera en conjunto
	//al destruir ó reinicializar el report.
	//  Tambien cuando, en el momento de crear un report, instanciamos un Fieldde tipo raster, 
	//segun las capacidades del  device en cuestion, se usara el raster BN ó Colour en caso
	//de que haya. 

	m4bool_t m_bTempWebImage;					m4bool_t m_bTempWebImageLoaded;

	m4bool_t m_bEmbebedRaster ;					m4bool_t m_bEmbebedRasterLoaded ;
	ClMIT_Str m_oBNRaster, m_oColourRaster ;	m4bool_t m_bBNRasterLoaded, m_bColourRasterLoaded  ;

	m4double_t m_dInterline ;					m4bool_t m_bInterlineLoaded ;

	m4bool_t m_bIsVarHeight;					m4bool_t m_bIsVarHeightLoaded;

	m4bool_t m_bIsJustified;					m4bool_t m_bIsJustifiedLoaded ;

	m4int16_t m_iMaxBoxes ;						m4bool_t m_bMaxBoxesLoaded ;

	ClENG_FieldStr::FILL_MODE m_FillMode ;			m4bool_t m_bFillModeLoaded ;
		
	m4char_t  m_cFillChar ;						m4bool_t m_bFillCharLoaded ;

	m4bool_t m_bSwDuplicated ;					m4bool_t m_bSwDuplicatedLoaded ;

	ClMIT_Str m_oIdCtField ;					m4bool_t m_bIdCtFieldLoaded ;

	ENG_BAR_CODE_TYPE m_BarCodeType;			m4bool_t m_bBarCodeTypeLoaded ;

	m4uint32_t m_uiRegionColour ;				m4bool_t m_bRegionColourLoaded ;

	ClMIT_Str  m_oRegionBorder ;				m4bool_t m_bRegionBorderLoaded ;

	m4bool_t m_bAdjustLineToContainer;			m4bool_t m_bAdjustLineToContainerLoaded ;
	
	StENG_DD_Defaults m_stDefaultData ;

	//Testea si el registro actual del Field ha sido modificado (se utiliza para la
	// reconstruccion de dependencias
	m4bool_t m_bIsModified ;

	//-------------->Constructores y sobrecargas para STL

	StENG_DD_FieldData(const StENG_DD_FieldData& ai_oSource) 
		{ Init(&ai_oSource) ; } 
	StENG_DD_FieldData & operator = (const StENG_DD_FieldData &ai_oOpe)
		{ Init(&ai_oOpe) ; return *this ;} 
	m4bool_t operator == (const StENG_DD_FieldData &ai_oOpe) const
		{ return M4_BOOL(m_uiIdField==ai_oOpe.m_uiIdField); }
	m4bool_t operator < (const StENG_DD_FieldData &ai_oOpe) const
		{ return M4_BOOL(m_uiIdField<ai_oOpe.m_uiIdField); }

	StENG_DD_FieldData() { Reset() ; }
	void Reset() ;	//Limpia los flags de cargado

	void Init( const StENG_DD_FieldData *ai_poSource ) ;
	
} ;

typedef list<StENG_DD_FieldData> ENG_DD_FieldList_t;	

struct StENG_DD_CntData {

	// Ojo, ponderado de 0 a n. 
	m4int16_t m_iPage ;						m4bool_t m_bPageLoaded;

	m4double_t m_dTop, m_dLeft ;			m4bool_t m_bTopLoaded, m_bLeftLoaded ;

	m4double_t m_dHeight;					m4bool_t m_bHeightLoaded;

	m4double_t m_dWidth;					m4bool_t m_bWidthLoaded;

	m4bool_t  m_IsFixed ;					m4bool_t m_bIsFixedLoaded;

	m4bool_t  m_bSwTogether ;				m4bool_t  m_bSwTogetherLoaded ;

	m4double_t m_dLowerLimit ;				m4bool_t m_bLowerLimitLoaded ;

	ClMIT_Str m_oCTMerge ;					m4bool_t m_bCTMergeLoaded ;
		
	ClMIT_Str m_oCancelCnt ;				m4bool_t m_bCancelCntLoaded ;

	m4uint32_t m_uiRegionColour ;			m4bool_t m_bRegionColourLoaded ;

	ClMIT_Str  m_oRegionBorder ;			m4bool_t m_bRegionBorderLoaded ;
	
	ENG_DD_FieldList_t m_oFieldList ;

	StENG_DD_Defaults m_stDefaultData ;

	//Testea si el registro actual del Field ha sido modificado (se utiliza para la
	// reconstruccion de dependencias
	m4bool_t m_bIsModified ;

	//-------------->Constructores y sobrecargas para STL

	StENG_DD_CntData(const StENG_DD_CntData& ai_oSource) 
		{ Init(&ai_oSource) ; } 
	StENG_DD_CntData & operator = (const StENG_DD_CntData &ai_oOpe)
		{ Init(&ai_oOpe) ; return *this ;} 

	StENG_DD_CntData() { Reset() ; }
	void Reset() ;	//Limpia los flags de cargado

	void Init( const StENG_DD_CntData *ai_poSource ) ;

	//Da "" o la parte CTId de m_oCTMerge

	void GetCTId(ClMIT_Str *ao_CTId) ;

	//Auxiliar, ver DD

	void DeleteReferencesToIdCTField( m4char_t *ai_pcCT, m4char_t *ai_pcIdCTField ) ;

	//Obtiene las minimas dimensiones posibles para un container
	void GetMinimalHeight(m4double_t *ai_dMinHeight);

	//Crean un Id unico de field en la lista de Fields.
	//Ojo, crean el Id unico en funcion de lo que contiene la lista. Es decir, n llamadas
	//	sucesivas a GenerateNewIdField dan el mismo resultado. Si se quieren crear varios
	//	Fields, hay que hacer un GenerateNewIdField, meter el Field en la lista, y luego
	//	pedir el siguiente GenerateNewIdField para que de claves diferentes.

	m4uint32_t GenerateNewIdField( )  {  return GenerateNewIdField(&m_oFieldList); }
	static m4uint32_t GenerateNewIdField( ENG_DD_FieldList_t *ai_poFieldList ) ;
}  ;	

struct StENG_DD_PageData {

	m4int16_t m_iPage ;									m4bool_t m_bPageLoaded ;
	
	m4double_t m_dOffsetX, m_dOffsetY ;					m4bool_t m_bOffsetXLoaded, m_bOffsetYLoaded ;

	ClENG_PagePrinter::PAPER_SIZE    m_PageSize ;		m4bool_t m_bPageSizeLoaded ;

	ClENG_PagePrinter::ORIENTATION   m_Orientation ;	m4bool_t m_bOrientationLoaded ;

	m4double_t m_dSizeX, m_dSizeY ;						m4bool_t m_bSizeXLoaded, m_bSizeYLoaded ;

	StENG_DD_Defaults m_stDefaultData ;

	StENG_DD_CntData m_oCnt ;							m4bool_t m_bCntLoaded ;

	
	//-------------->Constructores y sobrecargas para STL

	StENG_DD_PageData(const StENG_DD_PageData& ai_oSource) 
		{ Init(&ai_oSource) ; } 
	StENG_DD_PageData & operator = (const StENG_DD_PageData &ai_oOpe)
		{ Init(&ai_oOpe) ; return *this ;} 
	m4bool_t operator == (const StENG_DD_PageData & ai_oOpe) const
		{ return M4_BOOL(m_iPage==ai_oOpe.m_iPage); }
	m4bool_t operator < (const StENG_DD_PageData & ai_oOpe) const
		{ return M4_BOOL(m_iPage<ai_oOpe.m_iPage); }

	StENG_DD_PageData() { Reset() ; }
	void Reset() ;	//Limpia los flags de cargado

	void Init( const StENG_DD_PageData *ai_poSource ) ;
} ;


struct StENG_DD_SectionData {

	m4uint32_t m_iIdSection ;						m4bool_t m_bIdSectionLoaded ;

	ClMIT_Str m_oSectionName ;						m4bool_t m_bSectionNameLoaded ;

	ClENG_VR::NULL_IF_EMPTY m_NoPrintEmpty ;		m4bool_t m_bNoPrintEmptyLoaded ;

	ClENG_Section::BREAK_MODE m_BeforePBreak, m_AfterPBreak ; m4bool_t m_bBeforePBreakLoaded , 
																		m_bAfterPBreakLoaded ;
	ClENG_Section::HEADER_MODE m_HeaderMode ;		m4bool_t m_bHeaderModeLoaded ;

	ClENG_Section::FOOTER_MODE m_FooterMode ;		m4bool_t m_bFooterModeLoaded ; 

	ClENG_VR::GO_THROUGH_MODE m_GoT ;				m4bool_t m_bGoTLoaded ;

	ClENG_VR::RANGE_MODE m_RangeMode ;				m4bool_t m_bRangeModeLoaded ;

	ClMIT_Str m_oGroupKey ;							m4bool_t m_bGroupKeyLoaded ;

	ClMIT_Str m_RecordsetNodeName ;					m4bool_t m_bNextRecNodeLoaded ;

	m4bool_t m_bSwBreakVR ;							m4bool_t m_bSwBreakVRLoaded ;

	ClMIT_Str m_GuideNode;							m4bool_t m_bGuideNodeLoaded ;

	ClMIT_Str m_GuideItemName ;						m4bool_t m_bGuideItemNameLoaded ;

	m4bool_t m_bNullOnSons ;						m4bool_t m_bNullOnSonsLoaded ;

	ClMIT_Str m_oOneOneStr ;						m4bool_t m_bOneOneStrLoaded ;	

	ClMIT_Str m_oCancelSection, m_oResetBeforeTotal, m_oPrepareRecordset, 
			      m_oResetHeaderTotals, m_oCancelVR, m_oSetPostVRTotal ;
	m4bool_t m_bCancelSectionLoaded, m_bResetBeforeTotalLoaded, m_bPrepareRecordsetLoaded, 
			      m_bResetHeaderTotalsLoaded, m_bCancelVRLoaded, m_bSetPostVRTotalLoaded ;
	
	//D=""
	ClMIT_Str m_oTogether ;							m4bool_t m_bTogetherLoaded ;
		

	// Lista de secciones hijas

	typedef list<StENG_DD_SonSectionData> SonList_t;	
	SonList_t m_oSonList ;


	// Lista de Containers. Aqui es un multimap, en DT es un map, con la clave unica y con el
	//	testeo de enum realizado.

	typedef multimap<ENG_TYPE_CONTAINER,StENG_DD_CntData, less<ENG_TYPE_CONTAINER> > CntList_t;
	CntList_t m_oCnt ;

	// Defectos

	StENG_DD_Defaults m_stDefaultData ;

	//Testea si el registro actual del Field ha sido modificado (se utiliza para la
	// reconstruccion de dependencias
	m4bool_t m_bIsModified ;

	//-------------->Constructores y sobrecargas para STL

	StENG_DD_SectionData(const StENG_DD_SectionData& ai_oSource) 
		{ Init(&ai_oSource) ; } 
	StENG_DD_SectionData & operator = (const StENG_DD_SectionData &ai_oOpe)
		{ Init(&ai_oOpe) ; return *this ;} 

	StENG_DD_SectionData() { Reset() ; }
	void Reset() ;	//Limpia los flags de cargado

	void Init( const StENG_DD_SectionData *ai_poSource ) ;

	StENG_DD_CntData *FindCnt(ENG_FIND_ACTION ai_Action, ENG_TYPE_CONTAINER ai_CntId, 
		CntList_t::iterator *ao_It=NULL) ;

	//Auxiliar, ver DD

	void DeleteReferencesToIdCTField( m4char_t *ai_pcCT, m4char_t *ai_pcIdCTField ) ;

} ;


struct StENG_DD_CTCell {

	//O
	m4uint32_t m_uiIdParamSec ;				m4bool_t m_bIdParamSecLoaded;

	//Los valores m_Type y m_iLevel indican donde va la celda en la plantilla. Puede ponerse
	//cualquier cosa, en la capa DT se verificará todo.

	//O
	ENG_CT_CELL_TYPE m_Type ;				m4bool_t m_bTypeLoaded;

	//O.
	//Ponderado de 0 a n

	m4int16_t m_iLevel ;					m4bool_t m_bLevelLoaded ;
	
	m4double_t m_dHeight;					m4bool_t m_bHeightLoaded;

	m4double_t m_dWidth;					m4bool_t m_bWidthLoaded;

	m4uint32_t m_uiRegionColour ;			m4bool_t m_bRegionColourLoaded ;

	ClMIT_Str  m_oRegionBorder ;			m4bool_t m_bRegionBorderLoaded ;

	ENG_CT_CELL_Y_ALIGN m_AlignParamCellY ;	m4bool_t m_bAlignParamCellYLoaded ;

	ENG_CT_CELL_X_ALIGN m_AlignParamCellX ;	m4bool_t m_bAlignParamCellXLoaded ;
	
	ENG_DD_FieldList_t m_oFieldList ;
		
	//D=FALSE
	//Testea si el registro actual del Field ha sido modificado (se utiliza para la
	// reconstruccion de dependencias
	m4bool_t m_bIsModified ;

	//-------------->Constructores y sobrecargas para STL

	StENG_DD_CTCell(const StENG_DD_CTCell& ai_oSource) 
		{ Init(&ai_oSource) ; } 
	StENG_DD_CTCell & operator = (const StENG_DD_CTCell &ai_oOpe)
		{ Init(&ai_oOpe) ; return *this ;} 
	
	StENG_DD_CTCell() { Reset() ; }
	void Reset() ;	//Limpia los flags de cargado y limpia la lista de Properties

	//Obtiene las minimas dimensiones posibles para una celda de parametrizacion
	void GetMinimalWidth(m4double_t *ai_dMinWidth); 

	void Init( const StENG_DD_CTCell *ai_poSource ) ;
} ;

typedef list<m4uint32_t> ENG_IdSectionList_t;	

struct StENG_DD_CTData {

	ClMIT_Str m_oCTId;				m4bool_t m_bCTIdLoaded ;

	ClMIT_Str m_oName;				m4bool_t m_bNameLoaded ;							
		
	ClMIT_Str m_oTotList;			m4bool_t m_bTotListLoaded ;

	ClMIT_Str m_oTreeList;			m4bool_t m_bTreeListLoaded ;

	ClMIT_Str m_oFillTree;			m4bool_t m_bFillTreeLoaded ;

	m4uint32_t m_uiIdParamSec ;		m4bool_t m_bIdParamSecLoaded ;

	ENG_IdSectionList_t	m_oParamSec ;

	ENG_CT_MERGE_PAGE m_MergePage ;	m4bool_t m_bMergePageLoaded ;

	m4double_t m_dXColumnLeft ;		m4bool_t m_bXColumnLeftLoaded ;
	m4double_t m_dXColumnRight ;	m4bool_t m_bXColumnRightLoaded ;

	m4int16_t m_iTogLevel ;			m4bool_t m_bTogLevelLoaded ;

	ClMIT_Str m_oWizardData ;		m4bool_t m_bWizardDataLoaded ;

	ClMIT_Str m_oDerivedList ;		m4bool_t m_bDerivedListLoaded ;
		
	m4uint32_t m_iFirstCrossRow ;		m4bool_t m_bFirstCrossRowLoaded ;

	typedef list<StENG_DD_CTCell> CellList_t;	
	CellList_t m_oCellList ;


	//Testea si el registro actual del Field ha sido modificado (se utiliza para la
	// reconstruccion de dependencias
	m4bool_t m_bIsModified ;

	//-------------->Constructores y sobrecargas para STL

	StENG_DD_CTData(const StENG_DD_CTData& ai_oSource) 
		{ Init(&ai_oSource) ; } 
	StENG_DD_CTData & operator = (const StENG_DD_CTData &ai_oOpe)
		{ Init(&ai_oOpe) ; return *this ;} 
	m4bool_t operator == (const StENG_DD_CTData & ai_oOpe) const
		{ return M4_BOOL(m_oCTId==ai_oOpe.m_oCTId); }
	m4bool_t operator < (const StENG_DD_CTData & ai_oOpe) const
		{ return M4_BOOL(m_oCTId<ai_oOpe.m_oCTId); }


	StENG_DD_CTData() { Reset() ; }
	void Reset() ;	//Limpia los flags de cargado

	void Init( const StENG_DD_CTData *ai_poSource ) ;

	//Busca una cell, NULL si no encuentra.
	StENG_DD_CTCell *FindCell(ENG_FIND_ACTION ai_Action, 
		m4int16_t ai_iLevel, ENG_CT_CELL_TYPE ai_Type, CellList_t::iterator *ao_It=NULL) ;

	m4bool_t GetParamSec(m4int16_t ai_Level, m4uint32_t *ao_poParamSec) ; 
} ;

//==============================================================================>ClENG_DesignData

class ClENG_DDLF_Chn ;

class ClENG_DesignData
{

MIT_PUBLIC:

	typedef list<StENG_DD_FontData> FontList_t;
	typedef list<StENG_DD_FormatData> FormatList_t;
	typedef list<StENG_DD_Colour> ColourList_t;

MIT_PRIVATE:

	friend class ClENG_DesignTree ;

	//Friends SOLO para recorridos de datos. Para actualizar usar metodos.

	friend class ClENG_DDLF_Chn ;	

	StMIT_Inspector* m_poInsp;	//Simple link


	// Datos globales al report

	StENG_DD_GlobalReport m_oGlobal ;	m4bool_t m_bGlobalLoaded ;

	// FontList
	FontList_t m_oFontList ;

	// FormatList 
	FormatList_t m_oFormatList ;

	// Paleta FG. 
	ColourList_t m_oFGPalette ;
	
	// Page Data

	typedef list<StENG_DD_PageData> PageList_t;	
	PageList_t m_oPages ;

	// CT multimap. Formado por CTId y DatosCT.
	// Cuando se pasa a DT será map, comprobando la unicidad de claves

	typedef multimap<ClMIT_Str, StENG_DD_CTData, less<ClMIT_Str> > CTList_t;
	CTList_t m_oCT ;

	// LE multimap (Long Expression List). Formado por LEId (clave) y Expresion. 
	// Cuando se pasa a DT será map, comprobando la unicidad de claves

	typedef multimap<ClMIT_Str,ClMIT_Str, less<ClMIT_Str> > LEList_t;
	LEList_t m_oLE ;


	// TrLit multimap (Translated Literal List). Formado por TrLitId (clave) y TrLit. 
	// Cuando se pasa a DT será map, comprobando la unicidad de claves

	typedef multimap<ClMIT_Str,ClMIT_Str, less<ClMIT_Str> > TrLitList_t;
	TrLitList_t m_oTrLit ;

	
	// SectionList
	//
	//  Lista plana de secciones. Posteriormente en DT, se formaran n arboles de datos en 
	//funcion de estas listas. hay un arbol DT de secciones por report, y un arbol de datos 
	//por CT (fijarse que he dicho datos, NO secciones; la informacion se extrae de estas, 
	//pero no son secciones). 
	//	Es posible que en DD haya secciones no enlazadas con ningún arbol: en ese caso, 
	//simplemente no se usan en DT para nada.
	//  Las capas de carga de DD pueden almacenar secciones de template de forma temporal,
	//pero luego DEBEN pasar los datos a las estructuras CTCell y borrar fichas secciones
	//temporales.
	//	Si posteriormente se actualiza el DD a mano, respecto a CTs, debe de actualizarse
	//CTCell. La persistencia se hace suponiendo que persisten CTCells.
	//  La clave es el Id de seccion. En DT se testea que no haya claves duplicadas 
	//(es decir, que sea en realidad un map).

	typedef multimap<m4uint32_t,StENG_DD_SectionData, less<m4uint32_t> > SectionList_t;
	SectionList_t m_oSection ;

	// Ver DataField para detalles.

	ClENG_FileInterestList m_oFileInterest ;

MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_DesignData();
	virtual ~ClENG_DesignData() { ClENG_DesignData::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_DesignData) ; }
	virtual void End() ;

	//Entre otras cosas, limpia todos los flags de cargado y borra toda la informacion.

	void Init() ;	

	void SetGlobalData( StENG_DD_GlobalReport *ai_poData ) 
		{ m_oGlobal.Init(ai_poData) ; m_bGlobalLoaded=M4_TRUE ; }

	void DeleteFontList() { m_oFontList.clear() ; } 
	void AddFont( StENG_DD_FontData  *ai_poData ) 
		{ m_oFontList.push_back(*ai_poData); }
	m4bool_t DeleteFont( m4uint32_t ai_uiIdFont ) ;	//Falla si no encuentra Font (error local)

	void DeleteFormatList() { m_oFormatList.clear() ; } 
	void AddFormat( StENG_DD_FormatData  *ai_poData ) 
		{ m_oFormatList.push_back(*ai_poData); }
	m4bool_t DeleteFormat( m4uint32_t ai_uiIdFont ) ; //Falla si no encuentra Format (error local)

	void DeleteFGPalette() { m_oFGPalette.clear() ; } 
	void AddColour( StENG_DD_Colour  *ai_poData )
		{ m_oFGPalette.push_back(*ai_poData); }
	m4bool_t DeleteColour( m4uint32_t ai_uiIdColour ) ;	//Falla si no encuentra Font (error local)

	void DeletePageData() { m_oPages.clear() ; } 
	void AddPage(StENG_DD_PageData *ai_poData) 
		{ m_oPages.push_back(*ai_poData); }

	void DeleteCTList() { m_oCT.clear() ; } 
	void AddCT(StENG_DD_CTData *ai_poData) ;

	void DeleteLEList() { m_oLE.clear() ; } 
	void AddLE(m4char_t *ai_pcId, m4char_t *ai_pcExpr) ;

	void DeleteTrLitList() { m_oTrLit.clear() ; } 
	void AddTrLit(m4char_t *ai_pcId, m4char_t *ai_pcLit) ;

	void DeleteSectionList() { m_oSection.clear() ; } 
	void AddSection(StENG_DD_SectionData *ai_poData) ;

	//------------------------------------Busquedas-----------------------------------------

	//Busca una sección, NULL si no encuentra.
	StENG_DD_SectionData *FindSection(ENG_FIND_ACTION ai_Action, m4uint32_t ai_uiIdSection, 
		SectionList_t::iterator *ao_It=NULL) ;

	//Busca un CT, NULL si no encuentra.
	StENG_DD_CTData *FindCT(ENG_FIND_ACTION ai_Action, m4char_t *ai_pcIdCT, 
		CTList_t::iterator *ao_It=NULL) ;

	m4bool_t FindFont(m4uint32_t ai_uiIdFont, FontList_t::iterator &ao_It) 
		{ return FindFont(ai_uiIdFont,&m_oFontList,ao_It) ;  }
	static m4bool_t FindFont(m4uint32_t ai_uiIdFont, FontList_t *ai_poFontList, FontList_t::iterator &ao_It) ;
	
	m4bool_t FindFormat(m4uint32_t ai_uiIdFormat, FormatList_t::iterator &ao_It) 
		{ return FindFormat(ai_uiIdFormat,&m_oFormatList,ao_It) ;  }
	static m4bool_t FindFormat(m4uint32_t ai_uiIdFormat, FormatList_t *ai_poFormatList, FormatList_t::iterator &ao_It) ;

	m4bool_t FindColour(m4uint32_t ai_uiIdColour, ColourList_t::iterator &ao_It) 
		{ return FindColour(ai_uiIdColour,&m_oFGPalette,ao_It) ;  }
	static m4bool_t FindColour(m4uint32_t ai_uiIdColour, ColourList_t *ai_poColourList, ColourList_t::iterator &ao_It) ;

	//------------------------------------Mantenimeitno---------------------------------------
	
	//Borra una sección, false y error si no la encontró
	
	m4bool_t DeleteSection(m4uint32_t ai_uiIdSection) ;

	//Busca en todos los container de seccion. Si estan fusionados con el CT pasado, busca 
	//	dentro del container fields fusionados con el ai_pcIdCTField pasado, borrándolos. 
	//NO busca en el CT, no borra el field de plantilla CT.

	void DeleteReferencesToIdCTField( m4char_t *ai_pcCT, m4char_t *ai_pcIdCTField ) ;

	//-------------------------------------Otros---------------------------------------------

	// Ver DataField para detalles.
	void DeclareInterest( m4char_t *ai_pcFile ) { m_oFileInterest.DeclareInterest(ai_pcFile); }
} ;

#endif

