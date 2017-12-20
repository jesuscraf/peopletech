//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             designdt.h
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

#ifndef __DESIGNDT__H__
#define __DESIGNDT__H__

/*

ClENG_DesignTree y asociadas.
  
Partiendo de un DesignData, crea un TREE de parametros VALIDADOS.

*/

#include "font.h"
#include "clformat.h"
#include "ct_templ.h"
#include "clstr.h"
#include "field.h"
#include "xengval.h"
#include "clreport.h"
#include "pageprn.h"
#include "corevr.h"
#include "core.h"
#include "colour.h"
#include "analex.h"
#include "reportenv.h"

#include "designin.h"
#include "designdd.h"

#include "m4stl.hpp"


class ClENG_VB_Serialize ;

//==============================================================>Estructuras de datos auxiliares

// SOLO DEBEN CONTENER DATOS CONSTANTES PARA GENERAR REPORTS, NUNCA DATOS DE INSTANCIA NI
//	VARIABLES AUXILIARES.


// Los defaults, a este nivel, ya están con la herencia actualizada, es decir, contienen los
//	valores reales a usar.
// Los flags de Inherit dicen simplemente si el valor es  heredado, o se introdujo explicitamente.
//	FALTAN DE IMPLEMENTAR  !!!

struct StENG_DT_Defaults {

	StMIT_Inspector* m_poInsp;	//Simple link

	// Identificador (no indice) de la font.
	// Se testea que la font exista.
	m4uint32_t m_uiIdFont ;									m4bool_t m_bIdFontInherit ;

	// Calculado en base a la Font m_uiIdFont.
	DvcUnit_t m_FontDesignHeight, m_FontDesignMaxWidthChar ;	

	// Identificador (no indice) del formato o ClENG_Format::ID_NULL si no hay.
	m4uint32_t m_uiIdFormat ;								m4bool_t m_bIdFormatInherit ;

	// TEST:enum
	ClENG_FieldStr::ALIGN m_AlignY ;						m4bool_t m_bAlignYInherit ;

	// TEST:enum
	ClENG_FieldStr::ALIGN m_AlignX ;						m4bool_t m_bAlignXInherit ;

	m4bool_t	m_bSwClip ;									m4bool_t m_bClipInherit ;

	// Switch de lectura de Path de graficos en Registry o no
	m4bool_t	m_bSwGraphics ;								m4bool_t m_bGraphicsInherit ;

	//Id del Colour (def 0)
	m4uint32_t m_uiIdColour ;								m4bool_t m_bIdColourInherit ;

	// Comportamiento al formatear NULLs en fields
	m4bool_t m_bSwFormatNULL;								m4bool_t m_bSwFormatNULLInherit ;

	//Copia del pasado en DD
	ClMIT_Str m_oDefDisplayNULL ;							m4bool_t m_bDefDisplayNULLInherit ;

	// Hayado decodificando m_oDefDisplayNULL.
	// Cadenas de salida para NULLs
	StMIT_NULLDisplayDef    m_oNULLDef; 
	
	// Hayado decodificando m_oDefDisplayNULL.
	// Cadenas de formateo para NULLs
	StMIT_NULLDisplayDef    m_oNULLFormatDef ;

	//-------------------------Inicializaciones-----------------------------------------

	StENG_DT_Defaults() ;
	virtual ~StENG_DT_Defaults() { StENG_DT_Defaults::End(); }
	virtual size_t GetSizeof() { return sizeof(StENG_DT_Defaults) ; }
	virtual void End() ;

	// Reinicializa TODOS valores a sus defectos, y los valores de Inherit a FALSE.
	// Pone la font a la primera de la DBFont. !LA DBFONT DEBE TENER AL MENOS UNA FONT! o dara
	//	error.
	// ai_poDT es necesario para acceder a la DBFont.

	m4bool_t ResetGlobalDefaults(ClENG_DesignTree *ai_poDT, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	// Copia TODOS los datos de ai_poFrom, incluido los Flags de heredado. 

	void CopyFrom(StENG_DT_Defaults *ai_poFrom) ;

	// Copia TODOS los datos de ai_poFrom, pero todos flags de heredado se ponen a TRUE.

	void InheritAllFrom(StENG_DT_Defaults *ai_poFrom) ;

	// Ambas versiones no restean todos los valores del objeto, sino solo aquellos  que se 
	//	introdujeron explicitamente en ai_poSource.
	// Si no puede encontrar la Font o el Formato, genera error local y deja la que habia 
	//	previamente.
	// ai_poDT es necesario para acceder a la DBFont y a la lista de formatos.
	// Lo que diferencia a una version de otra es que, para los valores introducidos, 
	//	Inherit pone el flag de heredado a TRUE, y Update lo pone a FALSE.

	m4bool_t UpdateFrom( StENG_DD_Defaults *ai_poSource, ClENG_DesignTree *ai_poDT, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) 
		{ return InheritReset( ai_poSource, ai_poDT, M4_FALSE, ai_ErrorEffect) ;}

	m4bool_t InheritFrom( StENG_DD_Defaults *ai_poSource, ClENG_DesignTree *ai_poDT, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) 
		{ return InheritReset( ai_poSource, ai_poDT, M4_TRUE, ai_ErrorEffect) ;}

	//Ver CountFontFormatColourInstances en DesignTree, auxiliar de este. 
	// Esta funcion solo incrementa si debe. Los valores ya deben estar reseteados de antes.
	//	Los punteros pueden ser NULL.

	void IncrCountFontFormatColourInstances( 
		m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour, 
		m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
		m4uint32_t *aoi_puiColourCount ) ;


MIT_PRIVATE:

	m4bool_t InheritReset( StENG_DD_Defaults *ai_poSource, ClENG_DesignTree *ai_poDT, 
		m4bool_t ai_bSwInherit,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;

MIT_PUBLIC:


	//-------------------------Gets-----------------------------------------------------

	m4uint32_t GetIdFont() { return m_uiIdFont ; }
	DvcUnit_t GetFontDesignHeight() { return m_FontDesignHeight ; }
	DvcUnit_t GetFontDesignWidth() { return m_FontDesignMaxWidthChar ; }
	m4uint32_t GetIdFormat() { return m_uiIdFormat ; }
	ClENG_FieldStr::ALIGN	GetAlignY() { return m_AlignY ; }
	ClENG_FieldStr::ALIGN	GetAlignX() { return m_AlignX ; }
	m4bool_t	IsClip() { return m_bSwClip ; }
	m4bool_t	IsGraphicPath() { return m_bSwGraphics ; }
	m4uint32_t	GetIdColour() { return m_uiIdColour ; }
	m4bool_t IsFormatNULL() { return m_bSwFormatNULL ; }
	StMIT_NULLDisplayDef  * GetNULLDef() { return &m_oNULLDef; }
	StMIT_NULLDisplayDef  * GetNULLFormatDef() { return &m_oNULLFormatDef; }
	m4char_t * GetDefDisplayNULL() { return m_oDefDisplayNULL.InChar(0); } 
	
	m4bool_t IsFontInherit() { return m_bIdFontInherit ; } 
	m4bool_t IsFormatInherit() { return m_bIdFormatInherit ; } 
	m4bool_t IsAlignYInherit() { return m_bAlignYInherit ; } 
	m4bool_t IsAlignXInherit() { return m_bAlignXInherit ; } 
	m4bool_t IsClipInherit() { return m_bClipInherit ; } 
	m4bool_t IsGraphicsInherit() { return m_bGraphicsInherit ; } 
	m4bool_t IsIdColourInherit() { return m_bIdColourInherit ; } 
	m4bool_t IsSwFormatNULLInherit() { return m_bSwFormatNULLInherit ; } 
	m4bool_t IsDefDisplayNULLInherit() { return m_bDefDisplayNULLInherit ; } 



MIT_PRIVATE:

	//-------------------------Otros-----------------------------------------------------

	// Devuelve false si tuvo problemas. Actualiza m_oNULLDef y m_oNULLFormatDef

	m4bool_t DecodeDisplayNULL(m4char_t *ai_pcDisplayNULL ) ;
} ;

//======================================================================>StENG_DT_GlobalReport

struct StENG_DT_GlobalReport {

	//D="?"
	//Solo sirve para trace
	
	ClMIT_Str m_oReportName ;

	// O
	// Cadena con el nombre del canal de datos de diseño, la primera se usara si no hay 
	//	reprinter y la segunda si lo hay.

	ClMIT_Str  m_oDataChn;

	// [Si hay reprinter]O .Se pone en un primer paso como D a "",  al instanciar se testea
	//		no sea vacio.
	ClMIT_Str  m_oDataChnReprinter ;

	// D(MILIM)
	// TEST:enum
	// Modo de medida: internamente se convierte todo a Dvc. 

	ENG_MODE_METRIC m_ModeMetric ;

	// O
	//Debe ponerse a ENG_SECION_ID_EMPTY si no hay aun seccion.

	m4int16_t m_iIdRootSection ;

	// D(TRUE)
	m4bool_t  m_bSwRootSync ;

	// D(NONE)
	// TEST:enum

	ClENG_Report::BREAK_MODE m_BreakMode ;

	// [m_BreakMode!=NONE]O

	m4int16_t m_iIdBreakSection ;

	// D=""

	ClMIT_Str m_oTotalizeList ;

	// D=""

	ClMIT_Str m_oCTList ;


	//D=""
	//Parametros de ejecucion que son fijos e inherentes al report.
	
	ClMIT_Str m_oExecutionPars ;

	// D(FALSE) - Solo para la Warning de String Ovelapping en TxtDvc
	
	m4bool_t m_bIgnoreTxtWarning;


	StENG_DT_Defaults m_oReportDefaults ;

	//  Simplemente se copia el objeto. Los datos finales de instanciación solo se saben al
	//instanciar un report, ya que el resultado final depende de datos de cnal de diseño,
	//de parametros de ejecucion, y de una expresion especial del report.

	StFileParams m_stDefaultFileParams ;

	//Pasa los tests e inicializa m_oReportDefaults con los valores defecto.
	//ai_poDT hace falta para los defectos (DBFont)

	m4bool_t Init( StENG_DD_GlobalReport *ai_poSource, m4bool_t ai_bGlobalLoaded,
		ClENG_DesignTree *ai_poDT ) ;	
}  ;	

//========================================================================>StENG_DT_FormatData

struct StENG_DT_FormatData {

	//O
	m4uint32_t     m_uiIdFormat ;

	//O
	ClMIT_Str m_oFormatName ;

	//D:""
	ClMIT_Str m_oIdCurrency;


	//Lista de properties para el formato.
	//El nombre de propertie debe ser el reconocido por la m4fmt32.

	StENG_DD_FormatData::PropertieList_t m_oProperties ;

	// Pasa los test.  

	m4bool_t Init( StENG_DD_FormatData *ai_poSource) ;	

	//-----------Sobrecargas para usar el objeto con STL

	m4bool_t operator == (const StENG_DT_FormatData &ai_oOpe) const
		{ return M4_BOOL(m_uiIdFormat==ai_oOpe.m_uiIdFormat); }
	m4bool_t operator < (const StENG_DT_FormatData &ai_oOpe) const
		{ return M4_BOOL(m_uiIdFormat<ai_oOpe.m_uiIdFormat); }
} ;

//==============================================================================>StENG_DT_FieldData

struct StENG_DT_FieldData {

	//D=0.
	//Unico por container..

	m4uint32_t m_uiIdField ;
	
	//D="?"
	//Solo sirve para salida trace.

	ClMIT_Str m_oFieldName ;
		
	//D:FLD_STRING
	//TEST:enum

	ENG_FIELD_TYPE   m_FldType ;

	//D=0
	//TEST:Se admiten valores <0, que representan la medida ponderada en caracteres. 
	//	Se convierten a DvcUnits en esta capa. Top usa el alto de la font, y left el ancho.
	
	DvcUnit_t   m_Top, m_Left ;				

	//D=2 cm
	//TEST:Se admiten valores <0, que representan la medida ponderada en caracteres. 
	//	Se convierten a DvcUnits en esta capa. Usa el ancho de la font.

	DvcUnit_t m_Width;						

	//D=-1.
	//TEST:Se admiten valores <0, que representan la medida ponderada en caracteres. 
	//	Se convierten a DvcUnits en esta capa. Usa el alto de la font.
	//TEST:para FLD_MULTIL, si es <0, se usa el alto de la font y el interlineado.
	
	DvcUnit_t m_Height ;						


	//D=0
	//[m_FldType=FLD_LINE]O
	//Solo sirve para fields line. Definen dos puntos para designar una linea.
	//Se admiten también valores negativos para ponderar todo en caracteres.

	DvcUnit_t m_X1, m_Y1, m_X2, m_Y2 ;


	//D:""
	ClMIT_Str  m_oSaveNode ;					

	//[m_bSaveNodeLoaded=true]O

	ClMIT_Str  m_oSaveItemName ;				

	//[m_oLiteralValue]R=true
	//[ai_bSwPageCnt]D=true
	//[!ai_bSwPageCnt]D=false

	m4bool_t m_bSwStatic ;						

	//D="" 
	//[CTCell]R:""

	ClMIT_Str m_oIdPi;							

	//[ai_bSwPageCnt]D=true
	//[!ai_bSwPageCnt]D=false
	
	m4bool_t  m_bSwOptimize ;					

	//D:"\"\""
	//Esta cadena contiene la expresion que se utiliza para general el texto de un 
	// indice del Report
	
	ClMIT_Str m_oIndexExpr ;		

	//D: ClENG_Field::NO_INDEX_ENTRY
	//Este valor numerico indica el nivel de profundidad de un indice en el report
	//El valor NO_INDEX_ENTRY deshabilita la indexacion para un Field (defecto)

	m4int32_t m_iIndexLevel;

	//D:""  (generarar expresion vacia, que equivale a no expresion)
	//Esta cadena contiene la expresion que se utiliza para general el texto de un 
	// hiperenlace en dispositivos HTML
	
	ClMIT_Str m_oHyperlink ;

	//D:""
	//TEST:SOLO una de las dos cadenas NO es vacia. m_oExpr es la expresion en si,
	//	m_oLiteralValue es un literal traducible que se convierte a una expresion.
	//	Solamente si es tipo PI ó se da el siguiente caso (ver siguiente miembro),
	//  se permite que ambas esten vacias.
	//Siempre se deja la expresion valida a usar sobre m_oExpr.

	ClMIT_Str m_oExpr, m_oLiteralValue ;		

	//[m_FldType=FLD_RASTERG]D=M4_FALSE, cadenas D=""
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
	//	Si m_bEmbebedRaster esta activo, se reseteara el flag de Graphics del default a FALSE
	//y m_bSwStatic a TRUE.

	m4bool_t m_bTempWebImage; 

	m4bool_t m_bEmbebedRaster ;
	ClMIT_Str m_oBNRaster, m_oColourRaster ;

	//[m_FldType=FLD_MULTIL]D=0
	
	DvcUnit_t m_Interline ;					

	//Switch que indica si el Field multilinea es de altura variable
	// [m_FldType=FLD_MULTIL]D=TRUE;

	m4bool_t m_bIsVarHeight;					

	//Switch que indica si el Field Multilinea tiene alineacion justificada
	// [m_FldType=FLD_MULTIL]D=FALSE

	m4bool_t m_bIsJustified;					

	//[m_FldType=FLD_BOX]O

	m4int16_t m_iMaxBoxes ;						

	//[m_FldType=FLD_STRING o FLD_BOX]
	//	[!m_bFillCharLoaded] D:NO_FILL
	//	[m_bFillCharLoaded]  D:NO_EMPTY_FILL
	//TEST:enum

	ClENG_FieldStr::FILL_MODE m_FillMode ;			
		
	//[m_FldType=FLD_STRING o FLD_BOX]
	//		[m_FillMode!=NO_FILL]O

	m4char_t  m_cFillChar ;						

	//D:false

	m4bool_t m_bSwDuplicated ;					

	//[Template field]O
	//D:""

	ClMIT_Str m_oIdCtField ;					

	// [m_FldType=FLD_BARCODE]D=NO_CODE
	// TEST:enum

	//D:ClENG_Colour::ID_TRANSPARENT

	m4uint32_t m_uiRegionColour ;
	
	//Switch que indica si la línea debe ajustarse a su contenedor (prop FLD_LINE_ADJUST_TO_CONTAINER)
	// [m_FldType=FLD_LINE]D=FALSE
	m4bool_t m_bAdjustLineToContainer;

	//D:""
	//Cadena aplicable a Region::SetFrame para poner el frame a los fields. 
	//Si el field es de tipo linea, esta cadena equivale a concatenar las 2 cadena s de 
	//	SetLine()+SetFrame()

	ClMIT_Str  m_oRegionBorder ;				

	//Copia literal de los ultimos valores DD usados en Init para generar este objeto

	struct {
		m4double_t m_dTop, m_dLeft ;			m4bool_t m_bTopLoaded, m_bLeftLoaded ;
		m4double_t m_dHeight;					m4bool_t m_bHeightLoaded;
		m4double_t m_dWidth;					m4bool_t m_bWidthLoaded;
	} m_stFromValues ;

	//D=FALSE
	//Testea si el registro actual del Field ha sido modificado (se utiliza para la
	// reconstruccion de dependencias
	m4bool_t m_bIsModified ;

	ENG_BAR_CODE_TYPE m_BarCodeType;	

	StENG_DT_Defaults m_stDefaultData ;

	//ai_poDT hace falta para las LE
	//ai_poCntDef son los defectos a aplicar al Field. Estos seran, segun ai_Owner:
	//	-ENG_REPORT_CONTAINER.	Hereda de Pagina / Cnt / Field
	//	-ENG_PAGE_CONTAINER.	Hereda de Pagina / Field.	
	//	-ENG_CT_CELL.			Hereda de GlobalData / Field.
	
	m4bool_t Init( StENG_DD_FieldData *ai_poSource, ENG_FIELD_OWNER ai_Owner, 
		StENG_DT_Defaults *ai_poCntDef,
		ClENG_DesignTree *ai_poDT  ) ;

	//Inicializa todo poniendo como Default un Field tipo STR, con una expresion vacia.
	//Reinicializa los defectos (DT hace falta para eso)

	void InitFieldStrDefault(ClENG_DesignTree *ai_poDT) ;

	//-----------Sobrecargas para usar el objeto con STL
	m4bool_t operator == (const StENG_DT_FieldData &ai_oOpe) const
		{ return M4_BOOL(m_uiIdField==ai_oOpe.m_uiIdField); }
	m4bool_t operator < (const StENG_DT_FieldData &ai_oOpe) const
		{ return M4_BOOL(m_uiIdField<ai_oOpe.m_uiIdField); }

	//Ver CountFontFormatColourInstances en DesignTree, auxiliar de este. 
	// Esta funcion solo incrementa si debe. Los valores ya deben estar reseteados de antes.
	//	Los punteros pueden ser NULL.

	void IncrCountColourInstances( 
		m4uint32_t ai_uiIdColour, m4uint32_t *aoi_puiColourCount ) ;
} ;

typedef list<StENG_DT_FieldData> ENG_DT_FieldList_t;	

//===================================================================>StENG_TD_CntData

struct StENG_DT_CntData {

	//[PagLayout>1]O
	//[PagLayout=1]R=0
	//TEST:Testea rango de página valido. Poderado de 0 a n. En Cnt de pagina está 
	//	inicializado a la página a la que pertenece.

	m4int16_t m_iPage ;

	//O
	//[Es container de Pag]R=0
	//Pueden ser numeros negativos para ponderarlo en caracteres.

	DvcUnit_t m_Top, m_Left ;

	//O
	//[Es container de Pag]R=Height de la pagina.
	//Pueden ser numeros negativos para ponderarlo en caracteres.

	DvcUnit_t m_Height;

	//Como m_Height, pero para la anchura.
	//Pueden ser numeros negativos para ponderarlo en caracteres.

	DvcUnit_t m_Width;

	//O
	//[Es container de Pag]R=true

	m4bool_t  m_IsFixed ;

	//[m_IsFixed=false]D=False.

	m4bool_t  m_bSwTogether ;

	//[m_IsFixed=false]D=Bottom de la pagina
	//Pueden ser numeros negativos para ponderarlo en caracteres.

	DvcUnit_t m_LowerLimit ;


	//[Es container de Pag]R=""
	//[No es container de Pag]D=""

	ClMIT_Str m_oCancelCnt ;

	//D:ClENG_Colour::ID_TRANSPARENT

	m4uint32_t m_uiRegionColour ;				

	//D:""

	ClMIT_Str  m_oRegionBorder ;				

	//Copia literal de los ultimos valores DD usados en Init para generar este objeto

	struct {
		m4double_t m_dTop, m_dLeft ;			m4bool_t m_bTopLoaded, m_bLeftLoaded ;
		m4double_t m_dHeight;					m4bool_t m_bHeightLoaded;
		m4double_t m_dWidth;					m4bool_t m_bWidthLoaded;
		m4double_t m_dLowerLimit ;				m4bool_t m_bLowerLimitLoaded ;
	} m_stFromValues ;


	//D=FALSE
	//Testea si el registro actual del Field ha sido modificado (se utiliza para la
	// reconstruccion de dependencias
	m4bool_t m_bIsModified ;

	//Construido segun su homologo.

	ENG_DT_FieldList_t m_oFieldList ;
	
	StENG_DT_Defaults m_stDefaultData ;

	// Calculados en función de m_oCTMerge, que es:
	//
	//D:""
	//Formato instanciacion modo cabecera		:IdCT.H  ó IdCT.H.y.x 
	//						modo linea datos	:IdCT.D  ó IdCT.D.y.x 
	//		y puede ser U/D/A para modos U=ENG_CT_OVER_STC, D=ENG_CT_DOWN_STC, 
	//			A=ENG_CT_ALIGN_STC. Si no se indica, se toma A.
	//			Para modos linea datos, y se resetea incondicionalmente a A.
	//		x puede ser R para modos ENG_CT_RIGHT_STC. Si no se indica, se toma R.
	//			Por razones de compatibilidad hacia atrás, si se meten valores erroneos, en vez
	//			de dar error se toman los defectos.
	//			x también puede ser un número que indica un offset positivo a la derecha.


	ClMIT_Str m_oCTMerge ;	//Copia de DD: los sifuientes param son los datos desglosados
	ClMIT_Str m_oCTId ;
	ENG_CT_MERGE_CNT m_MergeMode ;
	ENG_CT_CELL_Y_POS	m_MergeYMode ;
	ENG_CT_CELL_X_POS	m_MergeXMode ;
	DvcUnit_t			m_OffsetLeft ;

	// ai_poDT necesario para datos de página, conversión de unidades y defectos. 
	// Los defectos se heredan asi:
	//		Si es Cnt de pagina:  Hereda de Pagina y Container.
	//		Si es Cnt de seccion: Hereda de Pagina, Seccion y Container.
	// Realiza testeos basicos.

	m4bool_t Init( 
		StENG_DD_CntData *ai_poSource, ENG_FIELD_OWNER ai_Owner,
		StENG_DD_Defaults *ai_poSectionDef, 
		ClENG_DesignTree *ai_poDT ) ;

	//Constructor copia
	void Init( StENG_DT_CntData *ai_poFrom ) ;

	//NULL o Field en cuestión.
	StENG_DT_FieldData	*FindIdCTField( ENG_FIND_ACTION ai_Action, m4char_t *ai_pcIdCtField,
		ENG_DT_FieldList_t::iterator *ao_It=NULL) ;

	//Ver CountFontFormatColourInstances en DesignTree, auxiliar de este. 
	// Esta funcion solo incrementa si debe. Los valores ya deben estar reseteados de antes.
	//	Los punteros pueden ser NULL.

	void IncrCountFontFormatColourInstances( 
		m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour,
		m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
		m4uint32_t *aoi_puiColourCount ) ;


}  ;	


//===================================================================>StENG_DT_PageData

struct StENG_DT_PageData {

	//O
	//Numero de pagina layout (de 0 a n). Sirve para construir el array de PageData
	//	Si la lista tiene n entradas, deben aparecer tods datos de 0 a n.

	//m4int16_t m_iPage ;
	
	//D=0

	DvcUnit_t m_OffsetX, m_OffsetY ;			

	//D=A4
	//TEST:enum

	ClENG_PagePrinter::PAPER_SIZE    m_PageSize ;

	//D:Portrait
	//TEST:enum
	
	ClENG_PagePrinter::ORIENTATION   m_Orientation ;

	//[m_PageSize=PERSONAL]O
	//CALCULO: siempre contienen las medidas correspondientes a los datos anteriores. Dato
	//	util en la fase de testeo.

	DvcUnit_t m_SizeX, m_SizeY ;						

	StENG_DT_Defaults m_stDefaultData ;

	//O
	StENG_DT_CntData m_oCnt ;	//Container de página

	//Pasa los tets e inicializa los defaults, heredando de report y page. 
	//Pasa test de obligatoriedad de m_oCnt, pero NO lo carga. Se hace a posteriori, en 
	//	LoadPageContainers. 
	//El test de m_iPage lo pasa LoadPageData
	//ai_poDT es necesario solo para GetDvcUnit()

	m4bool_t Init( StENG_DD_PageData *ai_poSource, ClENG_DesignTree *ai_poDT) ;	

	//Ver CountFontFormatColourInstances en DesignTree, auxiliar de este. 
	// Esta funcion solo incrementa si debe. Los valores ya deben estar reseteados de antes.
	//	Los punteros pueden ser NULL.

	void IncrCountFontFormatColourInstances( 
		m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour, 
		m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
		m4uint32_t *aoi_puiColourCount ) ;

} ;


//===================================================================>StENG_DT_SectionData

struct StENG_DT_SonSectionData ;

struct StENG_DT_SectionData {

	//O
	m4uint32_t m_iIdSection ;						

	//D="?"
	//Solo sirve para trace

	ClMIT_Str m_oSectionName ;						

	//D:false
	//TEST:enum

	ClENG_VR::NULL_IF_EMPTY m_NoPrintEmpty ;		

	//D:NONE
	//TEST:enum

	ClENG_Section::BREAK_MODE m_BeforePBreak, m_AfterPBreak ; 

	//D:H_ALL_PAGES
	//TEST:enum

	ClENG_Section::HEADER_MODE m_HeaderMode ;

	//D:F_ALL_PAGES
	//TEST:enum

	ClENG_Section::FOOTER_MODE m_FooterMode ;		

	//D:RECORD_GOT
	//TEST:enum

	ClENG_VR::GO_THROUGH_MODE m_GoT ;				

	//D:ALL_VR
	//TEST:enum

	ClENG_VR::RANGE_MODE m_RangeMode ;				

	//D:""
	ClMIT_Str m_oGroupKey ;							

	//O
	ClMIT_Str m_RecordsetNodeName ;					

	//D:False

	m4bool_t m_bSwBreakVR ;							

	//[m_GoT=SLICE_GOT y m_InheritMode=OWN_POS]D=Nodo de la seccion

	ClMIT_Str m_GuideNode;							

	//[m_GoT=SLICE_GOT y m_InheritMode=OWN_POS]O
	
	ClMIT_Str m_GuideItemName ;						

	//D=false

	m4bool_t m_bNullOnSons ;						

	//D=""
	//Lista de nombres de nodos, separados por secuencias de blancos o comas 

	ClMIT_Str m_oOneOneStr ;						

	//D=""
	ClMIT_Str m_oCancelSection, m_oResetBeforeTotal, m_oPrepareRecordset, 
			      m_oResetHeaderTotals, m_oCancelVR, m_oSetPostVRTotal ;
	
	//D=""
	ClMIT_Str m_oTogether ;							
		
	//Calculado en base al valor Together
	ClMIT_Flag::Value_t m_MaskTogether ;

	//Array de secciones hijas. 
	StENG_DT_SonSectionData *m_oSonSection ;
	m4int16_t  m_iSonSectionCount ;

	//D=FALSE
	//Testea si el registro actual del Field ha sido modificado (se utiliza para la
	// reconstruccion de dependencias
	m4bool_t m_bIsModified ;

	//Ademas de testear las Secciones, verfica unicidad de tipo de cnt, y test de enum.

	typedef map<ENG_TYPE_CONTAINER,StENG_DT_CntData, less<ENG_TYPE_CONTAINER> > CntList_t;
	CntList_t m_oCnt ;

	// Defectos

	StENG_DT_Defaults m_stDefaultData ;

	StENG_DT_SectionData();
	virtual ~StENG_DT_SectionData() { StENG_DT_SectionData::End(); }
	virtual void End() ;

	// ai_poDT necesario para LE y defectos. Se hereda de report y seccion.
	// Realiza testeos basicos, pendiente arbol de secciones.
	// Aloca y carga m_oSonSection, excepto el miembro Section, que se cargara en LoadTreeSection

	m4bool_t Init( StENG_DD_SectionData *ai_poSource, ClENG_DesignTree *ai_poDT ) ;

	//NULL si no encuentra (no genera error, pues ál ser recursivo lo haria varias veces)
	//Explora ella misma y la la rama.

	StENG_DT_SectionData *FindSection(m4uint32_t ai_uiIdSection) ;


	StENG_DT_CntData *FindCnt(ENG_FIND_ACTION ai_Action, ENG_TYPE_CONTAINER ai_CntId, 
		CntList_t::iterator *ao_It=NULL) ;


	//Ver CountFontFormatColourInstances en DesignTree, auxiliar de este. 
	// Esta funcion solo incrementa si debe. Los valores ya deben estar reseteados de antes.
	//	Los punteros pueden ser NULL.
	//Recursiva

	void IncrCountFontFormatColourInstances( 
		m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour,
		m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
		m4uint32_t *aoi_puiColourCount ) ;

} ;

//===================================================================>StENG_DT_SonSectionData

struct StENG_DT_SonSectionData {

	//O
	m4uint32_t m_uiIdSonSection;		

	//D=1
	m4int16_t  m_iExeOrder ;			

	//D:TRUE
	m4bool_t   m_bIsSync ;				

	//Calculado en LoadTreeSection
	StENG_DT_SectionData m_oSection ;

	//Limpia m_oSection.

	m4bool_t Init( StENG_DD_SonSectionData *ai_poSource ) ;
} ;

//===========================================================================>StENG_DT_CTCell
//
//Para ClENG_CT_T, datos de parametrización de una celda.
//
//Esta clase se usa como nodo en la plantilla de CT. 

struct StENG_DT_CTCell {

	//O
	m4uint32_t m_uiIdParamSec ;				

	//O
	ENG_CT_CELL_TYPE m_Type ;

	//O.
	//Ponderado de 0 a n

	m4int16_t m_iLevel ;	

	//O
	//NO pueden ser numeros negativos.

	DvcUnit_t m_Height;

	//Como m_Height, pero para la anchura.

	DvcUnit_t m_Width;

	//D:ClENG_Colour::ID_TRANSPARENT

	m4uint32_t m_uiRegionColour ;				

	//D:""

	ClMIT_Str  m_oRegionBorder ;				

	//D:ENG_CT_TOP_DISPLAY 
	//Test:enum

	ENG_CT_CELL_Y_ALIGN m_AlignParamCellY ;	

	//D:ENG_CT_LEFT_DISPLAY
	//Test:enum

	ENG_CT_CELL_X_ALIGN m_AlignParamCellX ;	

	
	//Construido segun su homologo.

	ENG_DT_FieldList_t m_oFieldList ;

	//Especial para template. Ver ahí. 

	m4bool_t m_bLoaded;	

	StENG_DT_CTCell() { m_bLoaded=M4_FALSE ; }

	//ai_poDT hace falta por defaults y carga de Fields
	//Pone a TRUE el flag de Loaded.
	//ai_poDT=NULL , ejecucion especial:
	//	Solo pasa tests para m_uiIdParamSec, m_Type y m_iLevel.
	//	NO pone a TRUE el flag de Loaded.

	m4bool_t Init( StENG_DD_CTCell *ai_poSource, ClENG_DesignTree *ai_poDT ) ;

	//NULL o Field en cuestión.
	StENG_DT_FieldData	*FindIdCTField( ENG_FIND_ACTION ai_Action, m4char_t *ai_pcIdCtField,
		ENG_DT_FieldList_t::iterator *ao_It=NULL) ;


	//Ver CountFontFormatColourInstances en DesignTree, auxiliar de este. 
	// Esta funcion solo incrementa si debe. Los valores ya deben estar reseteados de antes.
	//	Los punteros pueden ser NULL.

	void IncrCountFontFormatColourInstances( 
		m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour,
		m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
		m4uint32_t *aoi_puiColourCount ) ;

} ;


//===========================================================================>StENG_DT_CTData

struct StENG_DT_CTData {

	//O
	//Id del CT en cuestion.

	ClMIT_Str m_oCTId;

	//D=""
	//Para Trace
	ClMIT_Str m_oName;
		
	//O
	ClMIT_Str m_oTotList;

	//O
	ClMIT_Str m_oTreeList;

	//O
	//Lista de Nodo.Item,... ordenados, usados para rellenar el arbol de totalizadores.

	ClMIT_Str m_oFillTree;	

	//O
	//Id de la seccion root de la rama de secciones "Template"

	m4uint32_t m_uiIdParamSec ;

	//O
	//TEST:tantas entradas como LevelCount,la primera=m_uiIdParamSec.
	//Lista de las secciones de parametrizacion de template, la primera para el primer nivel, ect

	ENG_IdSectionList_t	m_oParamSec ;

	
	//D:ALL_PAGES

	ENG_CT_MERGE_PAGE m_MergePage ;	

	//D:X_ColumnLeft 1'5 cm, XColumnRight=Ancho pagina - 1'5 cm

	DvcUnit_t m_XColumnLeft ;		
	DvcUnit_t m_XColumnRight ;	

	//D:0

	m4int16_t m_iTogLevel ;

	//D:""
	//Info auxiliar para el wizard, nosotros no la usamos
	ClMIT_Str m_oWizardData ;

	//D:?? falta de implementar

	m4int16_t m_iBreakLevel ;

	//D:""

	ClMIT_Str m_oDerivedList ;	
	
	//O
	m4uint32_t m_iFirstCrossRow ;

	//Template de CT, contruido de forma especial. 
	//Debe tener al menos un nivel

	ClENG_CT_T m_oTemplate ;

	//D=FALSE
	//Testea si el registro actual del Field ha sido modificado (se utiliza para la
	// reconstruccion de dependencias
	m4bool_t m_bIsModified ;

	//Calculado en función de FillTree
	//>0
	m4int16_t m_iLevelCount ; 

	//Pasa los tets. LoadCTData verifica que el CTId sea unico.
	//ai_poDT es necesario solo para GetDvcUnit(), y para medidas de página
	//Resetea (NO inicializa) m_oTemplate. Debe hacerse luego. 

	m4bool_t Init( StENG_DD_CTData *ai_poSource, ClENG_DesignTree *ai_poDT ) ;

	//Ya deben estar cargadas las secciones en DT para cargar con exito la template

	m4bool_t LoadTemplate( StENG_DD_CTData *ai_poSource, ClENG_DesignTree *ai_poDT ) ;

	//Ver CountFontFormatColourInstances en DesignTree, auxiliar de este. 
	// Esta funcion solo incrementa si debe. Los valores ya deben estar reseteados de antes.
	//	Los punteros pueden ser NULL.

	void IncrCountFontFormatColourInstances( 
		m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour,
		m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
		m4uint32_t *aoi_puiColourCount ) ;

} ;


//==============================================================================>ClENG_DesignTree


class ClENG_DesignTree
{

MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	friend struct StENG_DT_PageData ;
	friend struct StENG_DT_CTData ;
	friend struct StENG_DT_SectionData ;
	friend struct StENG_DT_CntData ;
	friend struct StENG_DT_Defaults ;
	friend struct StENG_DT_FieldData ;
	friend struct StENG_DT_CTCell ;

MIT_PUBLIC:

	//********Miembros publicos para lectura
	
	// Datos globales al report

	StENG_DT_GlobalReport m_oGlobal ;

	// DBFont
	// Se monta una DB Font cargando los datos RVW (presentes en todas Fonts).  Se carga para
	//	tener los datos de diseño de font, al ser necesarios con medidas relativas a caracteres.
	// SOLO debe usarse en diseño del report metricas de diseño de Font. En diseño pueden 
	//	almacenarse punteros a Fonts, pero al crear el report, NO pasar estos punteros: obtener
	//	el Id y apuntar a la Font de la DBFont instancia de report, NO a la DBFont del diseño.
	// Cuando se cree una DBFont para un report en concreto, no vale con copiar esta DBFont, ya 
	//	que habrá que instanciar las Fonts con los datos para el formato (PCL,PSC,...) en el 
	//	que el report se tira.

	ClFONT_M_DBFont m_oDBFont ;


	// FormatList 
	//	No se testea la unicidad de clave ni nada por el estilo, hasta que se instancia un report.

	typedef list<StENG_DT_FormatData> FormatList_t;	
	FormatList_t m_oFormatList ;

	// Paleta FG. 
	// Crea una entrada con Id 0, que está reservado, y debe ser el color negro.
	// Respecto a punteros, tener las mismas consideraciones que con la DBFormat.

	ClENG_Palette m_oFGPalette ;

	// Page Data

	StENG_DT_PageData *m_oPageData ;	//Array, tantos elementos como paginas de diseño
	m4int16_t m_iNumPageLayout ;		//Número de entradas del array
	
	// Mapa de LE.
	// Las LE se resuelven a nivel de DT, se necesita esta lista para resolverlas.

	ClENG_Analex::LEMap_t m_oLE ;

	// Mapa de TrLit.

	StENG_ReportEnv::TrLitMap_t m_oTrLit ;
	
	//Map de CT data
	//La clave es el CTId. Se testea la unicidad.

	typedef map< ClMIT_Str, StENG_DT_CTData, less<ClMIT_Str> > CTList_t;
	CTList_t m_oCTList ;

	// Arbol de Secciones del report. (Arbol de report, NO de CT) 

	StENG_DT_SectionData m_oRootSection ;


	// Ver DataField para detalles.

	ClENG_FileInterestList m_oFileInterest ;



MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_DesignTree();
	virtual ~ClENG_DesignTree() { ClENG_DesignTree::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_DesignTree) ; }
	virtual void End() ;

	// Regenera todo el arbol de parametros en función de las listas de ai_poDD. 
	// Solo deja el arbol creado si no hay errores.

	m4bool_t Init( ClENG_DesignData *ai_poDD, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;


	// Carga en la DBFont una Font nueva de la DD.
	// Cargar una Font nueva no implica reconstruir todo el arbol. Todo sigue consistente.

	m4bool_t NewFont( ClENG_DesignData *ai_poDD, m4uint32_t ai_uiIdFont ) ;

	//Idem, para formatos y colores

	m4bool_t NewFormat( ClENG_DesignData *ai_poDD, m4uint32_t ai_uiIdFormat ) ;
	m4bool_t NewColour( ClENG_DesignData *ai_poDD, m4uint32_t ai_uiIdColour ) ;

	//----------------------------------Consultas ----------------------------------------

	StENG_DT_CTData *FindCT(ENG_FIND_ACTION ai_Action, m4char_t *ai_pcIdCT, 
		CTList_t::iterator *ao_It=NULL) ;

	StENG_DT_SectionData *FindSection(ENG_FIND_ACTION ai_Action, m4uint32_t ai_uiIdSection ) ;

	//Cuenta instancias de la Font y el Formato pasado. Los Count respectivos pueden ser NULL,
	//	en cuyo caso no se contabiliza el valor asociado.
	//Para que un valor entre la cuenta, debe ser un valor defecto indicado explicitamente,
	//	no heredado. Cuidado, los valores globales de report siempre entran en la cuenta, nunca
	//	se toman como heredados, estén o no indicados explicitamente.

	void CountFontFormatColourInstances( 
		m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour,
		m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
		m4uint32_t *aoi_puiColourCount ) ;
	
	//Usan CountFontFormatColourInstances, simples shortcuts.

	m4uint32_t CountFontInstances( m4uint32_t ai_uiIdFont ) ;
	m4uint32_t CountFormatInstances( m4uint32_t ai_uiIdFormat ) ;
	m4uint32_t CountColourInstances( m4uint32_t ai_uiIdColour ) ;

	//Hacen uso de CountxxxxInstances para dar una lista de IdFonts e IdFormatos no usados.

	void GetFontsNotUsed(  ENG_ULongList_t *ai_poFont ) ;
	void GetFormatsNotUsed(ENG_ULongList_t *ai_poFormat ) ;
	void GetColoursNotUsed(ENG_ULongList_t *ai_poColour ) ;

	//-------------------------Auxiliares carga DT-----------------------------------------
	
	//Para funcionar, necesita que m_oGlobal esté cargado.

	DvcUnit_t  GetDvcUnit( m4double_t ai_Metric ) ;
	m4double_t GetDesignUnit(DvcUnit_t ai_Metric) ;


MIT_PRIVATE:

	m4bool_t LoadDBFont( ClENG_DesignData *ai_poDD ) ;
	m4bool_t LoadFormatList( ClENG_DesignData *ai_poDD ) ;
	m4bool_t LoadFGPalette( ClENG_DesignData *ai_poDD ) ;
	m4bool_t LoadPageData(  ClENG_DesignData *ai_poDD ) ;
	m4bool_t LoadCTData( ClENG_DesignData *ai_poDD ) ;
	m4bool_t LoadCT_T_Data( ClENG_DesignData *ai_poDD ) ;
	m4bool_t LoadLE(  ClENG_DesignData *ai_poDD ) ;
	m4bool_t LoadTrLit(  ClENG_DesignData *ai_poDD ) ;
	m4bool_t LoadPageContainers(  ClENG_DesignData *ai_poDD ) ;

	// Metodo recursivo, carga la seccion, validandola, y carga todos sus hijos.

	m4bool_t LoadTreeSection( 
		ClENG_DesignData *ai_poDD, m4uint32_t ai_uiIdSection,
		StENG_DT_SectionData *ao_poSection, m4bool_t ai_bSwSync, m4int16_t ai_iExeOrder ) ;


	//-------------------------Auxiliares testeos ----------------------------------------

	m4bool_t TestFontData( StENG_DD_FontData *ai_poThis, StENG_DD_FontData *ai_poSource  ) ;
	m4bool_t TestColourData( StENG_DD_Colour *ai_poThis, StENG_DD_Colour *ai_poSource  ) ;
	//Testea que no haya claves repetidas, y que se cargo el SectionId
	m4bool_t TestSectionMultimap(ClENG_DesignData::SectionList_t *ai_poList) ;


MIT_PUBLIC:

	//-------------------------Auxiliares testeos simples----------------------------------

	static m4bool_t TestOblig(m4bool_t ai_bSwLoaded, const m4char_t *ai_VariableName) ;

	//Dan true si es un enum valido. Si no lo es, genera un error de efecto local.

	static m4bool_t TestFieldAlign( ClENG_FieldStr::ALIGN ai_iValue ) ;	
	static m4bool_t TestFieldType( ENG_FIELD_TYPE ai_iValue ) ;
	static m4bool_t TestReportModeMetric( ENG_MODE_METRIC ai_iValue ) ;	
	static m4bool_t TestReportBreakMode( ClENG_Report::BREAK_MODE ai_iValue ) ;	
	static m4bool_t TestFontESCPitch( ClFONT_M_Font::ESC_PITCH ai_iValue ) ;
	static m4bool_t TestPagePaperSize( ClENG_PagePrinter::PAPER_SIZE ai_iValue ) ;
	static m4bool_t TestPageOrientation( ClENG_PagePrinter::ORIENTATION ai_iValue ) ;
	static m4bool_t TestLogicOperator( ClMIT_ChnValue::OPERATOR ai_iValue ) ;
	static m4bool_t TestItemValueType(  ClMIT_ChnValue::TYPE  ai_iValue ) ;
	static m4bool_t TestSectionBreakMode(  ClENG_Section::BREAK_MODE ai_iValue ) ;
	static m4bool_t TestSectionHeaderMode(  ClENG_Section::HEADER_MODE ai_iValue ) ;
	static m4bool_t TestSectionFooterMode(  ClENG_Section::FOOTER_MODE ai_iValue ) ;
	static m4bool_t TestSecNullIfEmpty(  ClENG_VR::NULL_IF_EMPTY ai_iValue ) ;
	static m4bool_t TestFieldFillMode( ClENG_FieldStr::FILL_MODE ai_iValue ) ;
	static m4bool_t TestMergePage( ENG_CT_MERGE_PAGE ai_iValue ) ;	
	static m4bool_t TestSecGoT( ClENG_VR::GO_THROUGH_MODE ai_iValue ) ;
	static m4bool_t TestSecRangeMode( ClENG_VR::RANGE_MODE ai_iValue ) ;
	static m4bool_t TestCellType( ENG_CT_CELL_TYPE ai_iValue ) ;
	static m4bool_t TestLineStyle( ClENG_OutDevice::STYLE ai_iValue ) ;
	static m4bool_t TestAlignParamCellY( ENG_CT_CELL_Y_ALIGN ai_iValue ) ;
	static m4bool_t TestAlignParamCellX( ENG_CT_CELL_X_ALIGN ai_iValue ) ;


	//static m4bool_t TestSectionType(  ENG_SECTION_TYPE ai_iValue ) ;
	//static m4bool_t TestSecSlcTotalLink(  SEC_SLC_TOTAL_LINK_MODE ai_iValue ) ;
	//static m4bool_t TestSecSlcMergeLink(  SEC_SLC_MERGE_LINK_MODE ai_iValue ) ;


	static m4bool_t TestLoaderCntType( ENG_TYPE_CONTAINER ai_iValue ) ;
	static m4bool_t TestFieldBarCodeType( ENG_BAR_CODE_TYPE ai_iValue ) ;

	static const m4char_t * GetCntTraceStr( ENG_TYPE_CONTAINER ai_Enum ) ; 
	static const m4char_t * GetCellTypeTraceStr( ENG_CT_CELL_TYPE ai_iValue ) ;
	static const m4char_t * GetCellFieldTypeTraceStr( ENG_CT_CELL_FIELDS_TYPE ai_iValue ) ;

MIT_PUBLIC:

	// Gestiona el recorrido del Report para actualizar o reconstruir las dependencias
	//  del canal de datos. El Execution Mode del Report debe ser siempre NORMAL_EXECUTION
	//
	// Se le debe pasar un puntero al objeto que se encarga de persistir, pero ya debe haberse
	//  inicializado, o sea que ya debe haber cargado los nodos de dependencias

	m4bool_t ManageDependencies(m4bool_t *ao_bSwError, m4bool_t ai_bSwRebuildDep,
		                        ClENG_StorageDep *ai_poDep, ClMIT_Chn *ai_poChnData);

MIT_PRIVATE:

	//=========== DEPENDENCIAS =============================================

	//Actualiza las dependencias del Report y de los fields de pagina.
	m4bool_t ReportDependenciesGo(ClENG_StorageDep *ai_poDep, ClMIT_Chn *ai_poChnData, m4bool_t ai_bSwRebuildDep);

	//Basicamente, realiza el mismo recorrido que LoadTreeSection, es decir recorre los 
	// Container de una seccion (tambien los hijos) y los fields de container para ir
	// actualizando las dependencias. 
	// OJO!!
	// Consideramos que el valor de -1 para el ID del Root Section significa que no
	// hay seccion raiz. Al cargar este valor, si es -1, no entramos en esta funcion,
	// de esta forma no lanzamos errores de diseño, que a nivel de actulizacion de 
	// dependencias no importa.
	m4bool_t TreeSectionDependencies(StENG_DT_SectionData *ai_poSection, ClENG_StorageDep *ai_poDep, 
		                             ClMIT_Chn *ai_poChnData, m4bool_t ai_bSwRebuildDep);

MIT_PROTECTED:

	//Recorre los Fields de Container para actualizar o reconstruir las dependencias
	m4bool_t CntFieldDependencies(StENG_DT_SectionData *ai_poSection, ClENG_StorageDep *ai_poDep, 
		                          ClENG_Analex::StExpressionEnv *ai_pStEnv, m4bool_t ai_bSwRebuildDep);
	
	//Recorre los Fields de Pagina para actualizar o reconstruir las dependencias
	m4bool_t PageFieldDependencies(ClENG_StorageDep *ai_poDep, ClENG_Analex::StExpressionEnv *ai_pStEnv, m4bool_t ai_bSwRebuildDep);

	//Recorre la seccion para reconstruir o actualizar las dependencias
	m4bool_t SectionDependencies(
		StENG_DT_SectionData *ai_poSection,
		const m4char_t* ai_pcIdT3, ClENG_StorageDep *ai_poDep, 
		ClENG_Analex::StExpressionEnv *ai_pStEnv, ClMIT_Chn * ai_poChnData);

	//Lo mismo para el Report
	m4bool_t ReportDependencies(ClENG_StorageDep *ai_poDep, ClENG_Analex::StExpressionEnv *ai_pStEnv);

	//Dependencias de Crosstab
	m4bool_t CrosstabDependencies(ClENG_StorageDep *ai_poDep, ClMIT_Chn *ai_poChnData);

} ;


//===========================================================================>ClENG_StorageDep

// Clase BASE que gestionara el almacenamiento de las dependencias a los distintos
//dispositivos que deseemos. De esta clase se deriva la ClENG_StorageDepChn, que gestiona
//el almacenamiento en las tablas de dependencias en el canal de diseño.
// El proceso a seguir es sencillo. En la capa loader, en el ManageDependencies
//creamos un puntero a esta clase. Y en la llamada al TreeSectionDependencies 
//(que es la que se encargara de recorrer las secciones del Report y gestionar sus
//dependencias) se lo pasamos como parametro. Es importante notar que el Environment
//de la clase Analex, contiene ahora un puntero a esta clase, porque en la mayoria
//de las ocasiones el resultado de analizar una expresion genera una dependencia,
//con lo cual, hay que llamar al Analex, y el Init del Token en cuestion es el encargado
//de llamar al StoreDependence

class ClENG_StorageDep
{
	
MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_StorageDep() ;
	virtual ~ClENG_StorageDep() { ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_StorageDep) ; }

	virtual m4bool_t Init(ClMIT_Chn *ai_poChn) =0;

	//-------------------------Storage-----------------------------------------------------

	enum DEP_TYPE { REPORT, PAGE_FLD, SECTION, CONTAIN, FIELD };

	//Actualiza la posicion de la dependencia
	void UpdatePosition(DEP_TYPE ai_DepType, m4int16_t ai_IdPage, m4uint32_t ai_uIdSection, m4uint32_t ai_uIdContainer, m4uint32_t ai_uIdField);

	//Gestiona el almacenamiento de las dependencias de Item
	virtual m4bool_t StoreItemDependency(const m4char_t *ai_pcIdTI, const m4char_t *ai_pcIdItem)=0 ;

	//Gestiona el almacenamiento de las dependencias de Nodo
	virtual m4bool_t StoreNodeDependency(const m4char_t *ai_pcIdT3, const m4char_t *ai_pcIdNode)=0 ;

	//Gestiona el Borrado de dependencias
	virtual m4bool_t DeleteAllDependencies()=0;

MIT_PROTECTED:

	StMIT_Inspector* m_poInsp;	//Simple link

	m4uint32_t m_uIdSection;
	m4uint32_t m_uIdContainer;
	m4uint32_t m_uIdField;
	m4int16_t  m_uIdPage;
	DEP_TYPE   m_DepType;

} ;

#endif

