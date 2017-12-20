//==============================================================================
//
// (c) Copyright  1991-2001 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvcpdf.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             27/02/2001
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Manuel Lazcano Pérez
//	Modifications:	
//
// Definition:
//
//    This module manages the PDF Device
//
//
//==============================================================================

#ifndef __CLDVCPDF__H__
#define __CLDVCPDF__H__

#include "m4stl.hpp"
#include "clstr.h"
#include "cldvcfi.h"
#include "pageprn.h"
#include "clmetric.h"
#include "jpeg.h"


//======================================================================ClENG_PDF_Page
//Extiende funcionalidad para mapeo en PDF

class ClENG_PDF_Page : public ClENG_PagePrinter {

MIT_PRIVATE:

	unsigned long m_MediaBoxY, m_MediaBoxX ;
	DvcUnit_t m_TotalYOffset, m_TotalXOffset ;

MIT_PUBLIC:
	ClENG_PDF_Page();
	virtual ~ClENG_PDF_Page() { ClENG_PDF_Page::End() ; }
	virtual void End();
	virtual size_t GetSizeof() { return sizeof(ClENG_PDF_Page) ; }

	void Init( ORIENTATION ai_Orientation, PAPER_SIZE  ai_PageSize, 
		PAPER_SOURCE  ai_PaperSource,
		DvcUnit_t ai_SizeX,   DvcUnit_t ai_SizeY,
		DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY )  ;

	unsigned long GetMediaBoxY() { return m_MediaBoxY; }   
	unsigned long GetMediaBoxX() { return m_MediaBoxX; }   
	m4double_t GetPhysicalX(DvcUnit_t ai_X) ;
	//Si LastFont NULL, nmo se tiene en cuenta para el calculo
	m4double_t GetPhysicalY(DvcUnit_t ai_Y, ClFONT_M_Font *ai_poLastFont ) ;
} ;

//======================================================================ClENG_DvcPSC

class ClENG_DvcPDF : public ClENG_DvcFile {

MIT_PUBLIC:

	//---------------------- Inicializacion ----------------------------

	ClENG_DvcPDF();
	virtual ~ClENG_DvcPDF() { ClENG_DvcPDF::End() ; }

	virtual size_t GetSizeof() { return sizeof(ClENG_DvcPDF) ; }

	//Inicializacion del Device PostScript
	// PreLoadOnDevice=TRUE - DelegatedRasterPrint=FALSE

	// El device es:
	//		PRELOAD_ON_DEVICE	= TRUE
	//		COLOUR_SUPPORTED	= TRUE
	//		COLOUR_ACTIVATED	= TRUE 
	//		RASTER_PRELOADED	= FALSE
	//		HARD_DITHERING		= FALSE 
	//		HARD_SCALE			= FALSE 
	//Hay que pasarle un puntero a un objeto de la ClFONT_M_DBFont para tratar los caracteres ISO de las fuentes
	// que aparezcan en el Report.


	m4bool_t Init( 
		//Campos para ClENG_OutDevice::Init

		m4int16_t ai_iNumPages,
		ClENG_Palette *ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		m4bool_t ai_bForceMonoBreak,
		StFileParams *ai_FileParams, 
		m4bool_t ai_bDelete,
		DECLARE_EXTERNAL_FILE ai_DeclareExtFile,
		m4bool_t ai_bCreateDir,
		ClENG_ReportOuputRepository::PRESERVE_MODE ai_Preserve, 
		m4char_t *ai_pcToPrinter, 

		//Campos especificos

		ClFONT_M_DBFont* ai_poDBFont, 
		StENG_DvcParams::TUMBLE_MODE  ai_Tumble,
		m4bool_t ai_bTableOfContents, 
		
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;
		
	//Inicializacion de página. Se llama una vez por cada elemento del array de páginas. 
	// Como en PCL, SizeX y SizeY solo tienen sentido si el PageSize es personal

	m4bool_t InitPage( m4int16_t ai_iIdPage,
				   DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY,
				   size_t ai_szBlockBuffer,
				   ClENG_PagePrinter::ORIENTATION  ai_Orientation,
				   ClENG_PagePrinter::PAPER_SIZE    ai_PageSize,
				   ClENG_PagePrinter::PAPER_SOURCE ai_PaperSource,
				   DvcUnit_t ai_SizeX, DvcUnit_t ai_SizeY,
				   ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR );


	virtual void End();

	//----------------------- Miembros --------------------------------------------

MIT_PRIVATE:

	m4bool_t m_bTOC ; 

	ClENG_PDF_Page *m_poPagePrinter ; //Array dinámico de páginas para la impresora

	ClFONT_M_DBFont *m_poDBFont;    //Simple link

	ClENG_JpgEncoder m_oJPG_Enc ;

	m4bool_t m_bIncludeComent ; //Si true, se incluyen comentarios en el PDF

	
	//Tipo basico para ID de objeto. Tambien vector, se usa en varios sitios
	typedef m4uint32_t ObjId_t ;
	typedef vector <ObjId_t> ObjIdList_t ;

	//Conteo de objs PDF creados. Tambien se usa para obtener nuevos Ids (estos empiezan en 1).
	m4uint32_t m_uiObjCount ;

	//Mapa de xrefs, key:obj id, second: file offset.
	//Cada vez que se escribe un obj, se almacena su offset en esta mapa para escribir luego la
	//	tabla de xref.
	//NO incluye la primera xref dummy (la 0000000000 65535 f).

	typedef map <ObjId_t, m4uint32_t, less<ObjId_t> > XRefList_t ;
	XRefList_t m_oXRef ;
	
	//Lista con todos los objetos XObject creados por fichero.
	//Key: el m_oJPG_Id, que es siempre el nombre del JPG final sin path.
	//Para varias instancias de impresion que resuelven en un mismo m_oJPG_Id, solo existe un 
	//	unico objeto PDF, lo demas seran referencias a este objeto.

	struct StImages {
		ClMIT_Str m_oJPG_Id ;
		ObjId_t m_uiId ;
	} ;
	typedef map <ClMIT_Str, StImages, less<ClMIT_Str> > StImagesList_t ;
	StImagesList_t m_oImagesList ;

	//Lista con referencias al total de fonts usadas en el report.
	//Para saber el Id del objeto PDF, basta con buscarlos en esta lista.
	typedef map <ClFONT_M_Font *, ObjId_t, less<ClFONT_M_Font *> > FontList_t;
	FontList_t m_oFonts ;

	//Vector con los Ids de todos los obj /Page del report
	ObjIdList_t m_oPagesList ;

	//Vector con unos cuantos Id de objs PDF reservados por el report.
	//Los Id se reservan al comienzo, los obj PDF se crean despues.

	enum {
		CATALOG_OBJECT=0,
		PAGES_OBJECT,
		
		RESERVED_OBJECT_COUNT
	} ;
	ObjIdList_t m_oPDF_ObjectId ;

	//Estructura con los datos particulares para cada pagina
	typedef map <ObjId_t, ObjId_t, less<ObjId_t> > ObjIdMap_t ;
	struct StPage {
		ObjIdMap_t m_oUsedFonts ; //Fonts referenciadas en esta pagina (mapa para no duplicar)
		ObjIdMap_t m_oUsedImages ; //Images referenciadas en esta pagina (mapa para no duplicar)
		ObjId_t m_IdPageObj ;
		ObjId_t m_IdStreamObj ;
		ObjId_t m_IdLengthObj ;
		m4uint32_t m_szBytesToStream ; //Bytes consumidos del buffer hasta el comienzo de stream
	} ;
	StPage *m_poPages ;

	//----------------------- Sobrecarga de metodos de flujo de salida -------------

MIT_PUBLIC:

	void FixPreload();

	m4bool_t PreReset();

	void PostReset();

	m4bool_t RefreshLayout();
 
	m4bool_t FlushLayout();

	//----------------------- Sobrecarga de metodos Action --------------------------

	void EOP(m4int16_t ai_iPage);

	virtual void PrintNChar(m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
							m4pchar_t ai_pcStr, size_t ai_szNChar, 
							m4bool_t ai_bRawText,m4char_t *ai_pcHyperlink,
							m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET,
							ClENG_OutDevice::StPIParams *ai_pstPI=NULL);

	void PrintStr(m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, m4pchar_t ai_pcStr, 
		m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, 
		m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET);

	void SelectFontDevice(m4int16_t ai_iPage, ClFONT_M_Font* ai_poFont);


	//----------------------- Metodos propios del Dvc PostScript ---------------------

	//Prepara la cadena a imprimir para dispositivos PostScript, tratando los caracteres
	// especiales (por ahora solo son necesarios los de escape)
	void PrepareForPostScript(ClMIT_Str *ao_poStr, size_t *ai_szNChar);

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
		              size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, 
					  m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET);

	void ResolvePi(ClMIT_Str* ai_poIdPi, m4pchar_t ai_pcStr);

	//----------------------- Impresion Graficos --------------------------------------

	virtual void PrintRasterFile(
		m4int16_t ai_iPage,
		DvcUnit_t ai_X,				DvcUnit_t ai_Y,
		DvcUnit_t ai_SizeX,			DvcUnit_t ai_SizeY,
		const m4char_t * ai_pcPath, m4char_t *ai_pcHyperlink,
		m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET,
		m4bool_t ai_bIsTempWebPath=M4_FALSE
	) ;


MIT_PUBLIC:

	virtual void SelectColourDevice( m4int16_t ai_iPage, m4uint32_t ai_uiIdColour );

MIT_PRIVATE:

	//Devuelve un nuevo ObjId (este queda reservado y no se reutilizara)
	ObjId_t NewObjId()  ;
	
	//Muy importante, al mismo tiempo que escribe el obj guarda su xref.
	void WriteBeginObject(ClENG_PhysicalFile *F, ObjId_t ai_ObjId) ;
	//Como WriteBeginObject, para el objeto stream. Necesita tratamiento especial.
	//El XRef insertado queda pendiente de actualizar.
	void WriteBeginStreamObject(M4ClBuffer *ai_poBuff, ObjId_t ai_ObjId) ;

	void WritePDF_InitBlock(ClENG_PhysicalFile *F) ;
	//ai_FirstOutline==0, no hay
	void WritePDF_EndBlock(ClENG_PhysicalFile *F, ObjId_t ai_FirstOutline) ;

	void WritePDF_PageBlock(ClENG_PhysicalFile *F, m4int16_t ai_Page) ;

	size_t SetActivePalette ( ClENG_Palette* ai_poPalette, m4char_t* ai_pcBuff, size_t ai_szFrom, size_t ai_szMaxSize );

	
	//Devuelve el id para el obj root de los indices
	ObjId_t PrintIndex(ClENG_PhysicalFile *F) ;


	struct StPrintIndexAux {
		ClENG_IndexNode* m_poIndex ;
		ObjId_t m_ObjId ;
	} ;
	typedef vector <StPrintIndexAux> PrintIndex_t ;

	void PrintIndexLevel(
		ClENG_PhysicalFile *ai_F,
		ObjId_t ai_iFatherObj, 
		PrintIndex_t &ai_oBrotherNodes, m4uint32_t *ai_RecursiveLevelCount
	) ;
	void PrintRootListEntry(
		ClENG_PhysicalFile *ai_F,
		ObjId_t ai_iFatherObj, 
		PrintIndex_t &ai_oRootList, ObjId_t ai_BrkFirst, ObjId_t ai_BrkLast, 
		m4uint32_t ai_BrkCount) ;
	void ChangeAuxIndexValue(m4uint32_t ai_uiTargetID, m4uint32_t ai_PageId) ;
	
	void TrueTypeToPDF_Name(ClFONT_M_Font * ai_poFont, ClMIT_Str *ao_poStr) ;

};

#endif

