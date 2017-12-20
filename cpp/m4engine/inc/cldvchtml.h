//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvhtml.h
// Project:		     engindll.dll
// Author:           Meta Software M.S. , S.A
// Date:             28/01/2000
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Manuel Lazcano Perez
//	Modifications:	
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef __CLDVCHTML__H__
#define __CLDVCHTML__H__

#include "clstr.h"

#include "cldvcfi.h"
#include "pageprn.h"
#include "clmetric.h"
#include "replace.h"


//======================================================================ClENG_DvcPSC

class ClENG_DvcHTML : public ClENG_DvcFile {

MIT_PUBLIC:

	enum HTML_NAVIGATION { NAVIGATION_OFF, NAVIGATION_ACTIVE };
	enum HTML_MENU { BOTTOM, TOP }; //Indica la posicion relativa del menu de navegacion

	//----------------------- Miembros --------------------------------------------

MIT_PRIVATE:

	ClENG_PagePrinter *m_poPagePrinter ; //Array dinámico de páginas para la impresora
	
	//Array auxiliar de calculo, por cada pagina Layout almacena cual es el offset Y
	//	a aplicar al volcar a fichero, teniendo en cuenta que paginas de imprimen a 
	//	qué ficheros, con que otras comparte fichero, y el tamaño de cada pagina.
 	DvcUnit_t *m_poYOffset ;	

	ClFONT_M_DBFont *m_poDBFont;    //Simple link

	HTML_NAVIGATION m_NavigationMode; //Indica el modo de navegacion Web de la salida
	HTML_MENU m_HTMLMenuPos;       //Posicion relativa del menu de navegacion
	m4bool_t  m_bZoom;
	m4bool_t  m_bSearch;
	m4bool_t  m_bTOC ;
	ClMIT_Str  m_oHTMLMenuFile ; //"" no hay, si no path completo (se monta en Init)
	m4language_t m_Language;
	ClMIT_Str m_oReportName ;

	//---Templates-->

	static const m4char_t * m_pcTagDelimOpen, * m_pcTagDelimClose, *m_pcValueDelim ;
	static const m4char_t *m_pcPattenIds[] ;

	ClMIT_CopyReplace m_oDataStart ;
	ClMIT_CopyReplace m_oDataEnd   ;

	struct StBreakSecond {
		m4bool_t m_bSwCreateFile ;
		ClMIT_CopyReplace *m_poTemplate ;
	} ;
	typedef map<ClMIT_Str, StBreakSecond, less<ClMIT_Str> > BreakTemplates_t;	
	BreakTemplates_t m_oBreakTemplates ;

	//Array axuliar con valores globales de templates

	enum { NAME_BRK_INDEX, NAME_BRK_INDEX_NODE, NAME_BRK_INDEX_LEAF, NAME_BRK_INDEX_CLOSE, 
		NAME_ROOT_NAVIGATION, NAME_COUNT } ;
	ClMIT_Str m_TemplateValues[NAME_COUNT] ;

	//Keys a las templates de m_oBreakTemplates que hacen las 
	//	funciones de indexacion (tendran m_bSwCreateFile a false). "" si no se cargo.

	ClMIT_Str m_oTmplIndex ;
	ClMIT_Str m_oTmplIndexNode ;
	ClMIT_Str m_oTmplIndexLeaf ;
	ClMIT_Str m_oTmplIndexClose ;

	//Contiene todos los patterns posibles

	ClMIT_CopyReplace::Patterns_t m_oPatterns ;

	//<--Templates---

MIT_PUBLIC:
	//---------------------- Inicializacion ----------------------------

	ClENG_DvcHTML();
	virtual ~ClENG_DvcHTML() { ClENG_DvcHTML::End() ; }

	virtual size_t GetSizeof() { return sizeof(ClENG_DvcHTML) ; }

	//Inicializacion del Device HTML
	// PreLoadOnDevice=TRUE - DelegatedRasterPrint=FALSE

	// El device es:
	//		PRELOAD_ON_DEVICE	= TRUE
	//		COLOUR_SUPPORTED	= TRUE
	//		COLOUR_ACTIVATED	= TRUE
	//		RASTER_PRELOADED	= FALSE
	//		HARD_DITHERING		= FALSE
	//		HARD_SCALE			= FALSE
	// Los ficheros graficos se generan a partir de un BMP o se copian
	// al directorio de imagenes y se incrusta el nombre del fichero HTML.
	// Si ai_bFirstFont=TRUE, se coje el Id de la primera font de la DbFont, si no la
	//	indicada
	
	// No hay duplex: No tiene sentido en este formato y entra en conflicto con la navegacion.
	// Al path se le añadira automaticamente la extension HTML.
	
	m4bool_t Init( 

		//Campos para ClENG_OutDevice::Init

		m4int16_t ai_iNumPages,
		ClENG_Palette *ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		m4bool_t ai_bForceMonoBreak,
		StFileParams *ai_FileParams, m4bool_t ai_bDelete,
		DECLARE_EXTERNAL_FILE ai_DeclareExtFile,
		m4bool_t ai_bCreateDir,
		ClENG_ReportOuputRepository::PRESERVE_MODE ai_Preserve, 
		m4char_t *ai_pcToPrinter, 

		//Especificos de device

		ClFONT_M_DBFont* ai_poDBFont, 
		m4bool_t ai_bTableOfContents, 
		HTML_NAVIGATION  ai_NavigationMode, 
		HTML_MENU ai_HTMLMenu,
		m4bool_t ai_bZoom, 
		m4bool_t ai_bSearch, 
		m4language_t ai_Language,
		const m4char_t *ai_pcReportName,
		const m4char_t *ai_pcStartTemplate, const m4char_t *ai_pcEndTemplate,
		const m4char_t *ai_pcMainTemplate,

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;


	//Inicializacion de página. Se llama una vez por cada elemento del array de páginas. 

	m4bool_t InitPage( m4int16_t ai_iIdPage,
				   DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY,
				   size_t ai_szBlockBuffer,
				   ClENG_PagePrinter::ORIENTATION  ai_Orientation,
				   ClENG_PagePrinter::PAPER_SIZE    ai_PageSize,
				   ClENG_PagePrinter::PAPER_SOURCE ai_PaperSource,
				   DvcUnit_t ai_SizeX, DvcUnit_t ai_SizeY,
				   ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR );

	virtual void End();

MIT_PUBLIC:

	//----------------------- Sobrecarga de metodos de flujo de salida -------------

	m4bool_t PostPagesInit() ;

	m4bool_t PreReset();

	void PostReset();

	m4bool_t RefreshLayout();
 
	m4bool_t FlushLayout();

	//void NotifyLastReset() ;

	//----------------------- Sobrecarga de metodos Action --------------------------

	void EOP(m4int16_t ai_iPage);

	virtual void PrintNChar(m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
							m4pchar_t ai_pcStr, size_t ai_szNChar, 
							m4bool_t ai_bSwRawText, m4char_t *ai_pcHyperlink, 
							m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET, ClENG_OutDevice::StPIParams *ai_pstPI=NULL);

	void PrintStr(m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, m4pchar_t ai_pcStr, 
		m4bool_t ai_bSwRawText, m4char_t *ai_pcHyperlink, 
		m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET);

	void SelectFontDevice(m4int16_t ai_iPage, ClFONT_M_Font* ai_poFont);

	
    // -------------------- Lineas y zonas --------------------------------------

	void PrintLine( 
		m4int16_t ai_iPage, 
		DvcUnit_t ai_X1, DvcUnit_t ai_Y1, DvcUnit_t ai_X2, DvcUnit_t ai_Y2,
		DvcUnit_t ai_LineWidth, STYLE ai_Style
	) ;

	void FillZone( 
		m4int16_t ai_iPage, 
		DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height
	) ;

	void PrintFrame( 
		m4int16_t ai_iPage, 
		DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
		DvcUnit_t ai_LineWidth, STYLE ai_Style
	) ;

	//----------------------- Impresiones Pendientes (PI) ----------------------------

	void PrintNCharPi(ClMIT_Str *ai_poIdPi, m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY,
		              size_t ai_szNChar, 
					  m4bool_t ai_bSwRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET);

	void ResolvePi(ClMIT_Str* ai_poIdPi, m4pchar_t ai_pcStr);

	//----------------------- Impresion Graficos --------------------------------------
	
	// ********** Impresion Graficos
	
	virtual void PrintRasterFile(
		m4int16_t ai_iPage,
		DvcUnit_t ai_X,				DvcUnit_t ai_Y,
		DvcUnit_t ai_SizeX,			DvcUnit_t ai_SizeY,
		const m4char_t * ai_pcPath, m4char_t *ai_pcHyperlink,
		m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET,
		m4bool_t ai_bIsTempWebPath=M4_FALSE
	) ;

	//----------------------- Indice ---------------------------------------------------

	//void PrintIndex(ClENG_IndexNode* ai_poIndexNode);

MIT_PUBLIC:

	virtual void SelectColourDevice( m4int16_t ai_iPage, m4uint32_t ai_uiIdColour );
	HTML_NAVIGATION GetNavigationMode() { return m_NavigationMode; }
	m4bool_t IsZoom() { return m_bZoom; }
	m4bool_t IsSearch() { return m_bSearch; }
	m4bool_t IsTOC() { return m_bTOC; }

MIT_PRIVATE:
	//--------------------------------- Auxiliares-----------------------------------
	// Bug 0142780.
	void SetUpOutputProperties(m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, DvcUnit_t ai_iWidth, m4pcchar_t ai_pcStr);

	//Carga la template y busca ocurrencias de glbfile. Carga las que encuentre bajando los 
	//	ficheros con ThrowPublicFile.

	m4bool_t LoadTemplate(ClMIT_CopyReplace* ai_poTemplate, const m4char_t *ai_pcTemplateFileName) ;

	//Recursiva, usa tambien LoadTemplate
	m4bool_t LoadBreakTemplates(
		const m4char_t *ai_pcTemplateFileName, m4bool_t ai_bSwLangDependant ) ;

	m4bool_t LinkToIndexTemplates() ;

	//Si no existe el file, se copiara del graphic path
	//NULL si error
	ClENG_PhysicalFile * ThrowPublicFile(const m4char_t* ai_pcFileNameId);

	m4bool_t LoadTemplate(ClMIT_CopyReplace* ai_poTemplate ) ;

	m4bool_t UpdateNewFlushFiles() ;
	void UpdateClosingFlushFiles(m4bool_t ai_bPostResetCalling) ;

	//Debe ejecutarse en break para actualizar breakfile

	m4bool_t CreateTemplateBreakFiles() ;

	size_t SetActivePalette ( ClENG_Palette* ai_poPalette, m4char_t* ai_pcBuff, size_t ai_szFrom, size_t ai_szMaxSize );

	void ChangeAuxIndexValue(m4uint32_t ai_uiTargetID);

	m4bool_t PrintIndex()  ;
	m4bool_t PrintIndexEntry(
		ClENG_PhysicalFile *ai_F, ClENG_IndexNode* ai_poIndexNode, m4bool_t ai_bSwRecursive) ;

	void ResetYOffsets() ;
};

#endif

