//==============================================================================
//
// (c) Copyright  1991-2001 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvcpdf.cpp
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


#include "m4string.hpp"

#include "eng.h"
#include "engdf.inl"
#include "sysinfo.h"

#include "font.h"
#include "clfile.h"
#include "cldvcpdf.hmg"
#include "cldvcpdf.h"
#include "rasterg.h"

#include "jpeg.h"
#include "designin.h"

#include "m4unicode.hpp"

//------------------- Notas de funcionamiento interno---------------------------
/*
 No hay PreLoadOnDevice, solo usamos el normal de pagina.
 En este buffer se vuelca SOLO y exclusivamente el objeto stream que 
contendra el objeto apuntado en el campo /Contents del objeto /Page en cuestion del 
fichero PDF (*1).
 A la hora de imprimir graficos o cualquier otra cosa, siempre generamos objetos,
que volcamos directamente al fichero. Solo necesitamos del id del objeto para 
referenciarlo en cualquier sitio.
 Como PDF da la libertad de poner los objetos en el orden que queramos, los objetos
accesorios como /Catalog, /Page, ect, los creamos y escribimos al vuelo en fichero en el
FlushLayout o donde competa. Como en ese momento ya tenemos todos los datos necesarios 
para crearlos, es complicarse la vida el crearlos antes.
 Para gestionar la tabla de xref, no se permite escribir un comando obj a fichero si no es 
a traves del metodo WriteBeginObject. Este guarda el offset a fichero en la xref table para
escribirla luego y escribe la cabecera.

  Basicamente, la estructura de objetos de un PDF es la siguiente:

  1 File ----> 1 "/Type /Catalog" ------->  1 "/Type /Pages" --> 0..n "/Type /Page"
		|				               |->	0..n "/Type /Outlines"				
		 ----> 0..n "/Type /Font"						  
		|----> 0..n "/Type /XObject"

  1  "/Type /Page" --> 1 "/Contents" ---> 1 stream (*1)
				  |--> 0..n REFERENCIAS a los "/Type /Font" usados en la pagina
				  |--> 0..n REFERENCIAS a los "/Type /XObject" usados en la pagina

*/



//------------------- Especificaciones PostScript ---------------------------
/*

	**************************************************************************
	**************************************************************************
	********************* N O   U S A R   o b j   ****************************
	**************************************************************************
	**************************************************************************

*/
//------------------- Offsets de pagina (Limites fisicos) -------------------
// Point*254 = Dvc

#define PDF_OFFSET_Y (9*254)  
#define PDF_OFFSET_X (18*254)

//------------------- PDF Global Tags --------------------------------------

//!!!!CUIDADO CON LOS %: si se procesan por printf, %%, si se tratan como cadenas son %

#define PDF_FILE_HEADER		"%PDF-1.3\r\n"

#define PDF_OPEN_OBJECT		"<<\r\n"
#define PDF_CLOSE_OBJECT	">>\r\n"
#define PDF_END_OBJECT		"endobj\r\n\r\n"

#define PDF_OBJECT_TYPE		"/Type"

#define PDF_NEW_LINE		"\r\n"
#define PDF_SPACE			" "

//------------------- Comentarios-------------------------------------

#define PDF_INCLUDE_COMMENT M4_TRUE

#define PDF_COMMENT_INIT_b	"%-------------------->Init block\r\n\r\n"
#define PDF_COMMENT_INIT_e	"%<--------------------Init block\r\n\r\n"

#define PDF_COMMENT_END_b	"%-------------------->End block\r\n\r\n"
#define PDF_COMMENT_END_e	"%<--------------------End block\r\n\r\n"

#define PDF_COMMENT_FONT_b	"%-------------------->Font block\r\n\r\n"
#define PDF_COMMENT_FONT_e	"%<--------------------Font block\r\n\r\n"

#define PDF_COMMENT_PAGE_b	"%-------------------->Page block\r\n\r\n"
#define PDF_COMMENT_PAGE_e	"%<--------------------Page block\r\n\r\n"

#define PDF_COMMENT_TOC_b	"%-------------------->TOC\r\n\r\n"
#define PDF_COMMENT_TOC_e	"%<--------------------TOC\r\n\r\n"


#define PDF_COMMENT_CATALOG	"%***This is the CATALOG object\r\n"
#define PDF_COMMENT_PAGE	"%***This is the PAGE object\r\n"
#define PDF_COMMENT_PAGES	"%***This is the PAGES object\r\n"
#define PDF_COMMENT_TRAILER	"%***This is the TRAILER-XREf end fixed format block\r\n"
#define PDF_COMMENT_STREAM	"%***This is the page STREAM object\r\n"
#define PDF_COMMENT_LENGTH_STREAM	"%***This is the page LENGTH stream object\r\n"
#define PDF_COMMENT_RASTER	"%%***This is the %s image\r\n"
#define PDF_COMMENT_FONT_DESCRIPTOR	"%When this TTF font is missing, a TimesNewRoman will be used\r\n"


//------------------- Catalog, outlines & pages -----------------------

#define PDF_CATALOG			"/Catalog\r\n"
#define PDF_CAT_PAGES		"/Pages %lu 0 R\r\n"
#define PDF_CAT_OUTLINES	"/Outlines %lu 0 R\r\n"

#define PDF_COUNT			"/Count %lu\r\n"

#define PDF_PAGES			"/Pages\r\n"
#define PDF_OPEN_KIDS		"/Kids ["
#define PDF_KIDS_CONTENT	"%lu 0 R"
#define PDF_CLOSE_KIDS		"]\r\n"

#define PDF_PAGE			"/Page\r\n"
#define PDF_PARENT			"/Parent %lu 0 R\r\n"
#define PDF_RESOURCES		"/Resources"

#define PDF_PAPER_SIZE			 "/MediaBox [0 0 %lu %lu]\r\n"

#define PDF_CONTENTS		"/Contents %lu 0 R\r\n"

#define PDF_PROCSET			"/ProcSet [/PDF /Text]\r\n"
#define PDF_PROCSET_IMAGES	"/ProcSet [/PDF /Text /ImageC]\r\n"

#define PDF_LENGTH			"<< /Length %lu 0 R >>\r\n"
#define PDF_LENGTH_DEF		"%lu\r\n"


#define PDF_STREAM			"stream\r\n"
#define PDF_BEGIN_TEXT		"BT\r\n"

#define PDF_END_TEXT		"ET\r\n"
#define PDF_END_STREAM		"endstream\r\n"

//--------------------- Fuentes ---------------------------------------

#define PDF_FONT			"/Font"
#define PDF_FONT_DEF		"/Font << "
#define PDF_FONT_DEF_INSIDE	"/F%lu%s%lu%s0 R "
#define PDF_FONT_DEF_END	">>\r\n"
#define PDF_SUBTYPE_1		"/Subtype /Type1\r\n"
#define PDF_SUBTYPE_TRUETYPE	"/Subtype /TrueType\r\n"
#define PDF_FONT_NAME		"/Name /F%lu\r\n"
#define PDF_BASE_FONT		"/BaseFont /%s\r\n"
#define PDF_FONT_ENCODING	"/Encoding /WinAnsiEncoding\r\n"

#define PDF_FONT_FIRST_CHAR "/FirstChar %lu\r\n"
#define PDF_FONT_LAST_CHAR "/LastChar %lu\r\n"
#define PDF_FONT_WIDTHS_REF "/Widths %lu 0 R\r\n"
#define PDF_FONT_DESCRIPTOR_REF "/FontDescriptor %lu 0 R\r\n"

#define PDF_FONT_DESCRIPTOR_TYPE "/Type /FontDescriptor\r\n"
#define PDF_FONT_DESCRIPTOR_NAME "/FontName /%s\r\n"
#define PDF_FONT_DESCRIPTOR_FLAGS "/Flags %lu\r\n"
#define PDF_FONT_DESCRIPTOR_BBOX "/FontBBox [%li %li %li %li]\r\n"
#define PDF_FONT_DESCRIPTOR_CAP_HEIGHT "/CapHeight %li\r\n"
#define PDF_FONT_DESCRIPTOR_ASCENT "/Ascent %li\r\n"
#define PDF_FONT_DESCRIPTOR_DESCENT "/Descent %li\r\n"

#define PDF_SELECT_FONT		"/F%lu%s%lu%sTf\r\n"

//------------------- Lineas ------------------------------------------------

#define PDF_FILL_COLOUR	"%i %i %i rg\r\n"

#define PDF_LINE_WIDTH	"%f w\r\n"
#define PDF_MOVE_TO		"%f %f m\r\n"
#define PDF_DRAW_LINE	"%f %f l\r\n"
#define PDF_DRAW_RLINE	"%f %f l\r\n"
#define PDF_END_PATH	"S\r\n"
#define PDF_CLOSE_PATH	"s\r\n"
#define PDF_FILL		"b\r\n"

// Bug 149466. Se ajustan los estilos de línea para que se visualicen correctamente
// y se hacen coincidir con los de la salida PDF Unicode.
#define PDF_SOLID		 "[] 0 d\r\n"
#define PDF_DASH		 "[6 3] 0 d\r\n"
#define PDF_DOT			 "[1 3] 0 d\r\n"
#define PDF_DASH_DOT	 "[6 3 1 3] 0 d\r\n"
#define PDF_DASH_DOT_DOT "[6 3 1 3 1 3] 0 d\r\n"


//------------------- Impresion de cadenas ----------------------------------

#define PDF_NO_PRINT_STRING_PRE_PI	"%f %f Td ("
#define PDF_NO_PRINT_STRING_POST_PI				 "%.*s) Tj\r\n"

#define PDF_PRINT_STRING			"%f %f Td (%s) Tj\r\n"

//------------------- Color -------------------------------------------------

#define PDF_SET_RGB				"%.2f"
#define PDF_SET_FG_COLOUR		"rg\r\n"
#define PDF_SET_BG_COLOUR		"RG\r\n"

//-------------------- TOC ---------------------------------------------------



#define PDF_TOC_ROOT_TYPE		"/Type /Outlines\r\n"
#define PDF_TOC_COUNT			"/Count %lu"
#define PDF_TOC_TITLE			"/Title (%s)\r\n"
#define PDF_TOC_DEST			"/Dest [%lu 0 R /Fit]\r\n"

#define PDF_TOC_PREV			"/Prev %lu 0 R\r\n"
#define PDF_TOC_NEXT			"/Next %lu 0 R\r\n"

#define PDF_TOC_FIRST			"/First %lu 0 R\r\n"
#define PDF_TOC_LAST			"/Last %lu 0 R\r\n"

//-------------------- Graficos ----------------------------------------------

#define PDF_X_OBJECT_OPEN		" /XObject << "
#define PDF_X_OBJECT			"/Im%lu %lu 0 R "
#define PDF_X_OBJECT_CLOSE		">>\r\n"

#define PDF_BEGIN_IMAGE			"/Type /XObject\r\n/Subtype /Image\r\n"
#define PDF_IMAGE_WIDTH			"/Width %li\r\n"
#define PDF_IMAGE_HEIGHT		"/Height %li\r\n"
#define PDF_BITS_PER_COMPONENT	"/BitsPerComponent %i\r\n"
#define PDF_COLOR_RGB			"/ColorSpace /DeviceRGB\r\n"
#define PDF_COLOR_GREY			"/ColorSpace /DeviceGrey\r\n"
#define PDF_FILTER_JPEG			"/Filter /DCTDecode\r\n"
#define PDF_IMAGE_LENGTH		"/Length %li\r\n"
#define PDF_IMAGE_DATA			"stream\r\n"
#define PDF_END_IMAGE_DATA		"\r\nendstream\r\n"

#define PDF_IMAGE_START			"q\r\n"
#define PDF_IMAGE_POSITION		"%f 0 0 %f %f %f cm\r\n"
#define PDF_THROW_IMAGE			"/Im%lu Do\r\n"
#define PDF_IMAGE_END			"Q\r\n"

//------------------- Liberacion de la pila PDF y EOF --------------------

#define PDF_XREF		"xref 0 %lu\r\n"
//OJO, estas entradas deben ocupar exactamente 20 bytes (22 consumidos por el /r/n)
#define PDF_XREF_ZERO	"0000000000 65535 f\r\n"
#define PDF_XREF_ENTRY	"%010.10lu 00000 n\r\n"

#define PDF_TRAILER		"trailer\r\n<<\r\n/Size %lu\r\n/Root %lu 0 R\r\n>>\r\n"
#define PDF_STARTXREF	"startxref\r\n%lu\r\n"
#define PDF_END_OF_FILE "%%EOF\r\n"

//======================================================================ClENG_DvcPDF

//------------------- Inicializaciones --------------------------------------

ClENG_DvcPDF::ClENG_DvcPDF() 
{
	m_poPagePrinter = NULL;
	m_poPages = NULL ;
}

void ClENG_DvcPDF::End() 
{
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPagePrinter,
		            ClENG_PDF_Page, GetNumLayoutPages());

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPages,
		            StPage, GetNumLayoutPages());

	m_oImagesList.clear() ;
	m_oFonts.clear() ;
	m_oPagesList.clear() ;
	m_oXRef.clear() ;
	m_oJPG_Enc.End() ;
	
	ClENG_DvcFile::End();
}

m4bool_t ClENG_DvcPDF::Init( 

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
	
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t SwOk = M4_TRUE;
	ClMIT_Flag::Value_t Flags ;
	m4int16_t B ;
	
	ClENG_DvcPDF::End() ;

	//Activamos todos los Flags, puesto que siempre soporta color
	Flags =
		//NO activamos PreLoad, solo nos dara problemas y no merece la pena porque 
		//	optimizamos los graficos via objects unicos.
		//ClENG_OutDevice::PRELOAD_ON_DEVICE | 
		ClENG_OutDevice::COLOUR_SUPPORTED | 
		ClENG_OutDevice::COLOUR_ACTIVATED | 
		ClENG_OutDevice::RASTER_SUPPORTED ;

	SwOk = ClENG_DvcFile::Init(
		//Campos para ClENG_OutDevice::Init

		ai_iNumPages,
		Flags, ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		ai_bForceMonoBreak,	
		ai_FileParams, ai_bDelete,
		ClENG_BreakList::BREAK_TO_FILE,
		ClENG_BreakList::SHARING_DUPLEX, 
		ai_DeclareExtFile,
		ai_bCreateDir,
		ai_Preserve, 
		ai_pcToPrinter, 

		ClMIT_Trace::PROPAGATE_ERROR
	);
	if(!SwOk) goto exit;

	m_poDBFont = ai_poDBFont ;
	m_bTOC =  ai_bTableOfContents; 

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPagePrinter,
		         ClENG_PDF_Page, GetNumLayoutPages());

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPages,
		         StPage, GetNumLayoutPages());

	m_oJPG_Enc.Init(80);

	m_bIncludeComent = PDF_INCLUDE_COMMENT ;

exit:

	m_poInsp->m_poTrace->TestEffect(SwOk,ai_ErrorEffect);
	return SwOk;
}

m4bool_t ClENG_DvcPDF::InitPage(m4int16_t ai_iIdPage,
							DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY,
							size_t ai_szBlockBuffer,
							ClENG_PagePrinter::ORIENTATION  ai_Orientation,
							ClENG_PagePrinter::PAPER_SIZE   ai_PageSize,
							ClENG_PagePrinter::PAPER_SOURCE ai_PaperSource,
							DvcUnit_t ai_SizeX, DvcUnit_t ai_SizeY,
							ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t SwOk = M4_TRUE;
	m4bool_t initializePhantom ;

	if(!M4_IN_RANGE(ai_iIdPage, 0, GetNumLayoutPages()-1) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCPDF_MSG_10 ] ) << SEND_MSG ;
		SwOk = M4_FALSE;
		goto exit ;
	};

	m_poPagePrinter[ai_iIdPage].Init(ai_Orientation,ai_PageSize,ai_PaperSource,
		                             ai_SizeX, ai_SizeY, ai_OffsetX, ai_OffsetY );

	SwOk = ClENG_DvcFile::InitPage(
		ai_iIdPage, m_poPagePrinter[ai_iIdPage].GetSizeX(),
		m_poPagePrinter[ai_iIdPage].GetSizeY(), ai_OffsetX,
		ai_OffsetY, ai_szBlockBuffer, initializePhantom,
		 ClMIT_Trace::PROPAGATE_ERROR) ;
	if(!SwOk) goto exit;

	if ( initializePhantom )  {
		m_poPagePrinter[ GetNumLayoutPages()-1 ].Init( 
			ai_Orientation, ai_PageSize, ai_PaperSource, 
			ai_SizeX, ai_SizeY, ai_OffsetX, ai_OffsetY);
	} ;

exit:

	m_poInsp->m_poTrace->TestEffect(SwOk,ai_ErrorEffect);
	return SwOk;
}


m4uint32_t ClENG_DvcPDF::NewObjId()  
{
	++m_uiObjCount ;
	return m_uiObjCount ;
}

void ClENG_DvcPDF::WriteBeginObject(ClENG_PhysicalFile *F, ObjId_t ai_ObjId) 
{
	static const m4char_t * PDF_NEW_OBJECT="%lu 0 obj\r\n" ;
	m4char_t strAux[100] ;
	
	m_oXRef.insert( XRefList_t::value_type( ai_ObjId, F->GetPos() ) ) ;
	sprintf(strAux, PDF_NEW_OBJECT, (unsigned long)ai_ObjId) ;
	ClMIT_Str::OverStringValid(strAux,100) ;
	F->WStr(strAux) ;
}

void ClENG_DvcPDF::WriteBeginStreamObject(M4ClBuffer *ai_poBuff, ObjId_t ai_ObjId) 
{
	static const m4char_t * PDF_NEW_OBJECT="%lu 0 obj\r\n" ;
	m4char_t strAux[100] ;
	
	m_oXRef.insert( XRefList_t::value_type( ai_ObjId, ai_poBuff->GetTotal() ) ) ;
	sprintf(strAux, PDF_NEW_OBJECT, (unsigned long)ai_ObjId) ;
	ClMIT_Str::OverStringValid(strAux,100) ;
	ai_poBuff->AppendStr(strAux) ;
}


void ClENG_DvcPDF::WritePDF_InitBlock(ClENG_PhysicalFile *F)
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;
	ObjId_t id, idWidths, idFontDescriptor ;
	m4int16_t B, C;
	ClMIT_Str strAux ;
	m4bool_t swSoftFont ;

	//Cabecera universal. ANTES QUE CUALQUIER COSA, algunos WebServers la exploran
    F->WStr( PDF_FILE_HEADER PDF_NEW_LINE );

	//Comentario open
	if (m_bIncludeComent) F->WStr( PDF_COMMENT_INIT_b ) ;

	//***>Fonts

	ClFONT_M_Font *font ;

	//Comentario open font
	if (m_bIncludeComent) F->WStr( PDF_COMMENT_FONT_b ) ;

	for(B=0; B < m_poDBFont->GetNumFonts(); B++){

		//Inserta en map para luego saber id
		font = m_poDBFont->GetAddressFontByIndex(B) ;
		id = NewObjId() ;
		m_oFonts.insert( FontList_t::value_type(font,id) ) ;
		
		WriteBeginObject( F, id ) ;

		F->WStr( PDF_OPEN_OBJECT PDF_OBJECT_TYPE PDF_SPACE 
			PDF_FONT PDF_NEW_LINE ) ;

		if ( ! font->IsSoftFont() ) {
			swSoftFont = M4_FALSE ;
		} else {
			swSoftFont = font->GetTranslationType()!=ENG_SOFT_TRANS_TYPE_PSC_MIXED ;
		} ;

		if ( ! swSoftFont ) {
			F->WStr( PDF_SUBTYPE_1 ) ;
		} else {
			F->WStr( PDF_SUBTYPE_TRUETYPE ) ;
		} ;
		
		sprintf(pcBuff, PDF_FONT_NAME, (unsigned long)id);
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff) ;

		if ( ! swSoftFont ) {
			sprintf(pcBuff, PDF_BASE_FONT, font->PSC_GetTypeFace() );
			ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
			F->WStr(pcBuff) ;
			F->WStr(PDF_FONT_ENCODING PDF_CLOSE_OBJECT PDF_END_OBJECT PDF_NEW_LINE);
		} else {
			idWidths = NewObjId() ;
			idFontDescriptor = NewObjId() ;
			TrueTypeToPDF_Name( font,&strAux) ;
			sprintf(pcBuff, PDF_BASE_FONT, strAux.InChar(0) );
			sprintf(&pcBuff[strlen(pcBuff)], PDF_FONT_FIRST_CHAR, (unsigned long)0  ) ;
			sprintf(&pcBuff[strlen(pcBuff)], PDF_FONT_LAST_CHAR, (unsigned long)255  ) ;
			sprintf(&pcBuff[strlen(pcBuff)], PDF_FONT_WIDTHS_REF, (unsigned long)idWidths  ) ;
			sprintf(&pcBuff[strlen(pcBuff)], PDF_FONT_DESCRIPTOR_REF, (unsigned long)idFontDescriptor ) ;
			ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
			F->WStr(pcBuff) ;
			F->WStr(PDF_FONT_ENCODING PDF_CLOSE_OBJECT PDF_END_OBJECT PDF_NEW_LINE);

			//Escribe obj Widths

			WriteBeginObject( F, idWidths ) ;
			F->WStr("[") ;
			for (C=0; C<=ENG_PSC_AFM_WIDTHS_ARRAY_SIZE; ++C) {
				sprintf(pcBuff, "%i ", (int)font->GetAFM_Width(C) ) ;
				F->WStr(pcBuff) ;
			} ;
			F->WStr( "]\r\n" PDF_END_OBJECT PDF_NEW_LINE);

			//Y ahora obj FontDescriptor

			if (m_bIncludeComent) F->WStr( PDF_COMMENT_FONT_DESCRIPTOR ) ;
			WriteBeginObject( F, idFontDescriptor ) ;

			F->WStr( PDF_OPEN_OBJECT PDF_FONT_DESCRIPTOR_TYPE) ;
			sprintf(pcBuff, PDF_FONT_DESCRIPTOR_NAME, "TimesNewRoman" );
			sprintf(&pcBuff[strlen(pcBuff)], PDF_FONT_DESCRIPTOR_FLAGS, (unsigned long)34  ) ;
			sprintf(&pcBuff[strlen(pcBuff)], PDF_FONT_DESCRIPTOR_BBOX, 
				(long)-250, (long)-216, (long)1152, (long)1000 ) ;   
			sprintf(&pcBuff[strlen(pcBuff)], PDF_FONT_DESCRIPTOR_CAP_HEIGHT, (long)891  ) ;
			sprintf(&pcBuff[strlen(pcBuff)], PDF_FONT_DESCRIPTOR_ASCENT, (long)891  ) ;
			sprintf(&pcBuff[strlen(pcBuff)], PDF_FONT_DESCRIPTOR_DESCENT, (long)-216  ) ;
			ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
			F->WStr(pcBuff) ;
			
			F->WStr(PDF_CLOSE_OBJECT PDF_END_OBJECT PDF_NEW_LINE);
		} ;

	};
	
	//Comentario close font
	if (m_bIncludeComent) F->WStr( PDF_COMMENT_FONT_b ) ;

	//***<Fonts

	//Comentario close
	if (m_bIncludeComent) F->WStr( PDF_COMMENT_INIT_e ) ;

};

m4bool_t ClENG_DvcPDF::PreReset()
{
	m4bool_t SwOk=M4_TRUE;
	m4int16_t Page;
	m4int16_t B  ;

	ClENG_PhysicalFile *F ;
	m4bool_t swRedirect ;

	//Abre todos los ficheros (Breaks)
	//------->Abre y prepara ficheros siempre (no hay ATOMIC mode)

	SwOk = M4_BOOL( SwOk && m_oOutRepository.NewBreak() );	
	if (!SwOk) goto exit ;
	
	//------->Reset de datos por fichero

	m_uiObjCount=0 ;
	m_oImagesList.clear() ;
	m_oFonts.clear() ;
	m_oPagesList.clear();
	m_oPDF_ObjectId.clear() ;
	m_oXRef.clear() ;
	
	for (B=0; B<RESERVED_OBJECT_COUNT; ++B) {
		m_oPDF_ObjectId.push_back( NewObjId() ) ;
	} ;

	//------>Datos que solo van una vez al principio del fichero (o ficheros)

	for(Page=0; Page<GetNumLayoutPages(); ++Page){

		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;

		if ( ! swRedirect ) {
			WritePDF_InitBlock(F) ;
		};
	};

	//---------->Propaga

	ClENG_OutDevice::PreReset() ;

exit:
	return SwOk ;
}


void ClENG_DvcPDF::FixPreload()
{
	ClENG_OutDevice::FixPreload();
}


void ClENG_DvcPDF::WritePDF_EndBlock(ClENG_PhysicalFile *F, ObjId_t ai_FirstOutline)
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;
	ObjId_t id ;
	ObjIdList_t::iterator idIt;

	//Comentario open
	if (m_bIncludeComent) F->WStr( PDF_COMMENT_END_b ) ;

	//**Catalog  object

	if (m_bIncludeComent) F->WStr(PDF_COMMENT_CATALOG) ;

	WriteBeginObject( F, m_oPDF_ObjectId[CATALOG_OBJECT]  ) ;

	F->WStr(PDF_OPEN_OBJECT PDF_OBJECT_TYPE PDF_SPACE PDF_CATALOG);

	sprintf(pcBuff, PDF_CAT_PAGES, (unsigned long)m_oPDF_ObjectId[PAGES_OBJECT]);
	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
	F->WStr(pcBuff) ;

	if (ai_FirstOutline>0) {
		sprintf(pcBuff, PDF_CAT_OUTLINES, (unsigned long )ai_FirstOutline);
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff) ;
	};
	F->WStr(PDF_CLOSE_OBJECT PDF_END_OBJECT PDF_NEW_LINE);

	//***Pages object

	if (m_bIncludeComent) F->WStr(PDF_COMMENT_PAGES) ;

	WriteBeginObject( F, m_oPDF_ObjectId[PAGES_OBJECT] ) ;

	F->WStr(PDF_OPEN_OBJECT PDF_OBJECT_TYPE PDF_SPACE PDF_PAGES ) ;

	sprintf(pcBuff, PDF_COUNT, (unsigned long)m_oPagesList.size() );
	strcat(pcBuff, PDF_OPEN_KIDS);
	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
	F->WStr(pcBuff) ;

	for (idIt=m_oPagesList.begin(); idIt!=m_oPagesList.end(); idIt++) {
		if ( idIt!=m_oPagesList.begin() ) {
			sprintf(pcBuff, PDF_SPACE PDF_KIDS_CONTENT, (unsigned long)(*idIt) );
		} else {
			sprintf(pcBuff, PDF_KIDS_CONTENT, (unsigned long)(*idIt) );
		} ;
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		//Escribe uno a uno para no llenar bucle ciclicamente
		F->WStr(pcBuff);
	};
	sprintf(pcBuff, PDF_CLOSE_KIDS);
	strcat(pcBuff, PDF_CLOSE_OBJECT PDF_END_OBJECT PDF_NEW_LINE);
	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
	F->WStr(pcBuff);

	//Comentario close
	if (m_bIncludeComent) F->WStr( PDF_COMMENT_END_e ) ;

	if (m_bIncludeComent) F->WStr(PDF_COMMENT_TRAILER) ;

	//***A partir de aqui, formato fijo, no meter comentarios

	//***Tabla xref
	
	m4uint32_t offXRef ;
	XRefList_t::iterator xrefIt ;
	ObjId_t prevXRef ;

	//offset para referenciarlo en startxref
	offXRef = F->GetPos() ; 

	sprintf(pcBuff, PDF_XREF PDF_XREF_ZERO, (unsigned long)m_oXRef.size()+1 );
	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
	F->WStr(pcBuff);

	//Importante el orden, porque el rango de los xref id debe ser contiguo
	for ( xrefIt=m_oXRef.begin(), prevXRef=0; xrefIt!=m_oXRef.end(); ++xrefIt ) {
		sprintf(pcBuff, PDF_XREF_ENTRY, (unsigned long)(*xrefIt).second );
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff);
		if ( (*xrefIt).first != prevXRef+1 ) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Not contigous Xref, missing object id %0:s.", ClMIT_Msg::MIXED_OUT)
				<< prevXRef+1 << SEND_MSG ;
		} ;
		prevXRef=(*xrefIt).first ;
	} ;
	
	//***Trailer
	
	sprintf(pcBuff, PDF_TRAILER, (unsigned long)m_oXRef.size()+1, 
		(unsigned long)m_oPDF_ObjectId[CATALOG_OBJECT] );
	sprintf(&pcBuff[strlen(pcBuff)], PDF_STARTXREF , (unsigned long)offXRef);
	ClMIT_Str::OverStringValid(pcBuff, StENG_TaskEnv::DvcPDF_MAX_SIZE);
	F->WStr(pcBuff);
	F->WStr(PDF_END_OF_FILE) ; //Ojo con el %% de EOF !
}

void ClENG_DvcPDF::PostReset()
{
	m4int16_t Page, i;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t *pcBuff = TaskEnv->Dvc_g_pcBuff;
	m4int16_t iCounter;
	ObjId_t outlineRoot ;

	ClENG_PhysicalFile *F ; 
	m4bool_t swRedirect ;


	for(Page=0; Page<GetNumLayoutPages(); ++Page){

		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;

		if ( !swRedirect ) {
			if ( m_bTOC ) {
				outlineRoot = PrintIndex(F) ;
			} else {
				outlineRoot =0 ;
			} ;
			WritePDF_EndBlock(F,outlineRoot) ;
		} ;
	};

	//---------->Cierra ficheros (no hay ATOMIC mode)<<

	m_oOutRepository.CloseBreak( IsValidOutput() ) ;

	//---------->Propaga

	ClENG_OutDevice::PostReset(M4_FALSE);
}

m4bool_t  ClENG_DvcPDF::RefreshLayout()
{
	m4bool_t SwOk=M4_TRUE;
	m4int16_t Page;
	M4ClBuffer *pageBuffer;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;
	StPage *stPage ;

	//ClENG_PhysicalFile *F ; //Se one a NULL si es redirect
	//m4bool_t swRedirect ;


	//Datos de cabecera para cada pagina
	for(Page=0; Page<GetNumLayoutPages();++Page){
		
		stPage = &m_poPages[Page] ;
		pageBuffer = m_poLayoutPages[Page].m_poActualBuff ;
		//F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;
		
		//---------->Limpia datos de pagina

		stPage->m_oUsedFonts.clear() ;
		stPage->m_oUsedImages.clear() ;

		//--------->Graba en buffer (no file o PIs se actualizan mal) cabecera obj stream
		
		stPage->m_IdPageObj=NewObjId() ;
		stPage->m_IdStreamObj=NewObjId() ;
		stPage->m_IdLengthObj=NewObjId() ;

		if (m_bIncludeComent) pageBuffer->AppendStr(PDF_COMMENT_STREAM) ;

		WriteBeginStreamObject( pageBuffer, stPage->m_IdStreamObj) ;

		sprintf(pcBuff, PDF_LENGTH PDF_STREAM, (unsigned long)stPage->m_IdLengthObj );
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		pageBuffer->AppendStr(pcBuff) ;

		stPage->m_szBytesToStream = pageBuffer->GetTotal() ;

		//No hay buffer m_oOpPrBuff ni m_oPrBuff (no PRELOAD_ON_DEVICE)

	};

	//---------->Notifica a FileRep (no abre files, no ATOMIC_MODE)

	SwOk=m_oOutRepository.NewFlush() ;

	//---------->Propaga

	return SwOk && ClENG_OutDevice::RefreshLayout() ;
};



void ClENG_DvcPDF::WritePDF_PageBlock(ClENG_PhysicalFile *F, m4int16_t ai_Page) 
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM());
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *pageBuffer;

	ObjId_t idPage, idStream ;
	ObjIdMap_t::iterator idIt ;

	StPage *stPage = &m_poPages[ai_Page] ;
	pageBuffer = m_poLayoutPages[ai_Page].m_poActualBuff ;

	if (m_bIncludeComent) F->WStr( PDF_COMMENT_PAGE_b ) ;

	//***Objeto page
		
	idPage = m_poPages[ai_Page].m_IdPageObj ;
	idStream = m_poPages[ai_Page].m_IdStreamObj ;
	//Comunica para obj PAGES
	m_oPagesList.push_back(idPage) ;  

	if (m_bIncludeComent) F->WStr(PDF_COMMENT_PAGE) ;

	WriteBeginObject(F,idPage) ;

	F->WStr( PDF_OPEN_OBJECT PDF_OBJECT_TYPE PDF_SPACE PDF_PAGE ) ;

	sprintf(pcBuff, PDF_PARENT, (unsigned long)m_oPDF_ObjectId[PAGES_OBJECT] );
	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
	F->WStr(pcBuff);

	//Convetimos el tamaño a points, que es como se pondera el user space

	sprintf(pcBuff, PDF_PAPER_SIZE, 
		(unsigned long)m_poPagePrinter[ai_Page].GetMediaBoxX(),  
		(unsigned long)m_poPagePrinter[ai_Page].GetMediaBoxY() );
	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
	F->WStr(pcBuff);

	//Redirección a contents 
	sprintf(pcBuff, PDF_CONTENTS, (unsigned long)idStream );
	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
	F->WStr(pcBuff);

	//-->Resources
	F->WStr(PDF_RESOURCES PDF_SPACE PDF_OPEN_OBJECT);

	//Resource pages
	if ( stPage->m_oUsedFonts.size()>0 ) {
		F->WStr(PDF_SPACE PDF_FONT_DEF) ;
		for (idIt=stPage->m_oUsedFonts.begin(); idIt!=stPage->m_oUsedFonts.end(); ++idIt ) {
			sprintf(pcBuff, PDF_FONT_DEF_INSIDE, 
				(unsigned long)(*idIt).first, PDF_SPACE, (unsigned long)(*idIt).first, PDF_SPACE);
			ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
			F->WStr(pcBuff);
		} ;
		F->WStr(PDF_FONT_DEF_END);
	} ;
	
	//Resource images
	if ( stPage->m_oUsedImages.size()>0 ) {
		F->WStr(PDF_SPACE PDF_X_OBJECT_OPEN) ;
		for (idIt=stPage->m_oUsedImages.begin(); idIt!=stPage->m_oUsedImages.end(); ++idIt ) {
			sprintf(pcBuff, PDF_X_OBJECT, 
				(unsigned long)(*idIt).first, (unsigned long)(*idIt).first );
			ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
			F->WStr(pcBuff);
		} ;
		F->WStr(PDF_X_OBJECT_CLOSE);
	} ;

	//ProcSet

	if ( stPage->m_oUsedImages.size()>0 ) {
		F->WStr( PDF_SPACE PDF_PROCSET_IMAGES PDF_SPACE PDF_CLOSE_OBJECT ) ;
	} else {
		F->WStr( PDF_SPACE PDF_PROCSET PDF_SPACE PDF_CLOSE_OBJECT ) ;
	} ;

	F->WStr(PDF_CLOSE_OBJECT PDF_END_OBJECT PDF_NEW_LINE);

	//<--Resources

	//***Obj length del stream 
	// length sera la del buffer, menos la parte de cabecera que se escribio

	if (m_bIncludeComent) F->WStr(PDF_COMMENT_LENGTH_STREAM) ;
	WriteBeginObject(F,stPage->m_IdLengthObj) ;
	sprintf(pcBuff, PDF_LENGTH_DEF PDF_END_OBJECT, (unsigned long)
		(pageBuffer->GetTotal() - stPage->m_szBytesToStream)  ) ;
	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
	F->WStr(pcBuff);

	//***Mete en buffer de pagina la parte final del objeto stream

	pageBuffer->AppendStr( PDF_END_STREAM PDF_END_OBJECT ) ;

	if (m_bIncludeComent) F->WStr( PDF_COMMENT_PAGE_e ) ;
}


m4bool_t ClENG_DvcPDF::FlushLayout()
{
	m4bool_t SwOk=M4_TRUE;
	
	M4ClBuffer *pageBuffer;
	ClENG_PhysicalFile *F ; 
	m4bool_t swRedirect ;
	m4int16_t Page ;

	//---------->Corre EOPs

	RunAllEOP() ;

	//---------->Actualiza buffers
	
	for(Page=0; Page<GetNumLayoutPages(); ++Page){

		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;

		WritePDF_PageBlock(F,Page) ;
	};

	//---------->Actualizar XRefs de objetos stream
	//
	//Justo antes de volcar todo a fichero es cuando podemos actualizar esto.
	//En los buffers tenemos solo y exclusivamente el objeto stream al completo, este Dvc
	//	no soporta duplex, por lo que los bloques iran consecutivos desde la posicion
	//	actual del fichero.

	m4uint32_t currentFileOff ;
	XRefList_t::iterator xrefIt ;
	ObjId_t xrefObjId ;
	m4uint32_t xrefOffset ;

	F = m_oOutRepository.GetFlushFile(0, &swRedirect ) ;

	for(Page=0, currentFileOff=F->GetPos() ; Page<GetNumLayoutPages(); ++Page) {
		xrefIt = m_oXRef.find( m_poPages[Page].m_IdStreamObj ) ;
		xrefObjId = (*xrefIt).first ;
		xrefOffset = currentFileOff + (*xrefIt).second ; //Contiene offset en buffer
		pageBuffer = m_poLayoutPages[Page].m_poActualBuff ;

		m_oXRef.erase(xrefIt) ;
		m_oXRef.insert( XRefList_t::value_type(xrefObjId,xrefOffset) ) ;
		
		currentFileOff += pageBuffer->GetTotal()  ;
	} ;

	//---------->Vuelca buffers a files  (no cierra files, no ATOMIC_MODE)

	SwOk=m_oOutRepository.CloseFlush( ClENG_DvcFile::m_poBuffArray ) ;

	//---------->Propaga

	return SwOk && ClENG_OutDevice::FlushLayout();
}

void ClENG_DvcPDF::SelectFontDevice(m4int16_t ai_iPage, ClFONT_M_Font* ai_poFont)
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	FontList_t::iterator it ;

	//Escribe a buffer ...

	it=m_oFonts.find(ai_poFont) ;
	sprintf(pcBuff, PDF_SELECT_FONT, 
			(unsigned long)(*it).second, PDF_SPACE,	
			(unsigned long)ai_poFont->PSC_GetHeight(), PDF_SPACE );
	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
	PageBuffer->AppendStr(pcBuff);

	//Y notifica que se usó

	m_poPages[ai_iPage].m_oUsedFonts.insert( 
		ObjIdMap_t::value_type((*it).second,(*it).second) ) ;
}


void ClENG_DvcPDF::PrintNChar(m4int16_t	ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
						  m4pchar_t ai_pcStr, size_t ai_szNChar,
						  m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID,
						  ClENG_OutDevice::StPIParams *ai_pstPI)
{
	m4double_t dXPos = 0;
	m4double_t dYPos = 0;
	ClMIT_Str oStrAux;
	size_t Length = 0;
	m4double_t iLengthOrig = 0;
	m4uint32_t lValueOffset = 0;
	size_t szClipSize = 0;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* m_pcBuff = TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	size_t szNChar = 0;
	size_t szNCharOrig = 0;

	dXPos = m_poPagePrinter[ai_iPage].GetPhysicalX( ai_iX ) ;
	dYPos = m_poPagePrinter[ai_iPage].GetPhysicalY(ai_iY, GetLastFont(ai_iPage) );

	// Bug 0125206. En los campos multilínea te llega el contenido del campo completo
	// pero en el número de caracteres te llega los que realmente quiere imprimir.
	m4pchar_t pcString = NULL;
	m4pchar_t pcInputString = new m4char_t[ai_szNChar + 1];
	memcpy(pcInputString, ai_pcStr, ai_szNChar);
	pcInputString[ai_szNChar] = '\0';

	// Bug 99018.
	// Salva el número de caracteres a imprimir, para luego poder subrayarlos.
	// Esto es necesatio para poder subrayar este número de caracteres en concreto.
	// Como más adelante, se modifica este valor en el PrepareForPostScript, 
	// ConvertToIsoWithEscape, ConvertToEscape el problema es que el subrayado era 
	// más grande que el número de caracteres (se subrayaba de más).
	szNChar = strlen(pcInputString);
	szNCharOrig = szNChar;
	
	// UNICODE REPORTS.
	// Convertir una cadena (multi byte) a multibyte ansi. Si estamos en ansi, te devuelve
	// ansi, es decir, esa misma cadena. Si estamos en unicode, pasa de utf8 a ansi.
	// wide char.
	pcString = M4CppToANSI(pcInputString, (int&)szNChar);

	//Comprueba la existencia de caracteres especiales y si existen hace una llamada
	// al metodo PrepareForPostScript
	oStrAux.StrCpy(pcString);
	if(strpbrk(oStrAux.InChar(0),"\\()"))
	{
		PrepareForPostScript(&oStrAux, &szNChar);
	};

	//Convierte caracteres extendidos a ISO
	if (!ai_bRawText)
	{
		ClMIT_SysInfo::ConvertToIsoWithEscape(&oStrAux, &szNChar);
	}
	else
	{
		ClMIT_SysInfo::ConvertToEscape(&oStrAux, &szNChar);
	} ;

	//Imprime Cadena
	Length = sprintf(m_pcBuff, 
		PDF_BEGIN_TEXT PDF_NO_PRINT_STRING_PRE_PI, dXPos, dYPos ) ;

	//Calculamos el Offset de una PI. Va a comenzar justo aqui
	lValueOffset = PageBuffer->GetTotal() + Length ;

	Length += sprintf(&m_pcBuff[strlen(m_pcBuff)], 
		PDF_NO_PRINT_STRING_POST_PI PDF_END_TEXT, (size_t)szNChar, oStrAux.InChar(0));

	//No utilizamos strlen() por que la cadena puede tener codigos especiales, que incluyan el 0
	if((Length+1)>StENG_TaskEnv::DvcPDF_MAX_SIZE){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCPDF_MSG_20 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError();
	};

	PageBuffer->Append(m_pcBuff,Length);

	if(ai_pstPI){
		ai_pstPI->m_uiOff=lValueOffset;
		ai_pstPI->m_szNChars=szNChar;
	};

	if(GetLastFont(ai_iPage)->IsUnderline())
	{
		sprintf(m_pcBuff, PDF_MOVE_TO, dXPos, (dYPos - 1));
		PageBuffer->AppendStr(m_pcBuff);
		// Bug 99018.
		// Length = ClMetric::DvcToPoint(GetLastFont(ai_iPage)->GetDvcLenStr(pcString, ai_szNChar));
		iLengthOrig = ClMetric::DvcToPoint(GetLastFont(ai_iPage)->GetDvcLenStr(pcString, szNCharOrig));
		sprintf(m_pcBuff, PDF_DRAW_LINE, (dXPos + iLengthOrig), (dYPos - 1));
		PageBuffer->AppendStr(m_pcBuff);
		sprintf(m_pcBuff, PDF_CLOSE_PATH);
		PageBuffer->AppendStr(m_pcBuff);
	};

	delete [] pcString;
	delete [] pcInputString;

	if(ai_iTargetID!=ClENG_OutDevice::DVC_NO_INDEX_TARGET && m_bTOC ) {
		ChangeAuxIndexValue(ai_iTargetID, m_poPages[ai_iPage].m_IdPageObj ) ;
	} ;
}


void ClENG_DvcPDF::PrintStr(
	m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, m4pchar_t ai_pcStr, 
	m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID)
{
	m4double_t dXPos = 0;
	m4double_t dYPos = 0;
	ClMIT_Str oStrAux;
	size_t Length;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* m_pcBuff=TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	m4pchar_t pcString = NULL;
	
	dXPos = m_poPagePrinter[ai_iPage].GetPhysicalX( ai_iX ) ;
	dYPos = m_poPagePrinter[ai_iPage].GetPhysicalY(ai_iY, GetLastFont(ai_iPage) );

	// UNICODE REPORTS.
	// Convertir una cadena (multi byte) a multibyte ansi. Si estamos en ansi, te devuelve
	// ansi, es decir, esa misma cadena. Si estamos en unicode, pasa de utf8 a ansi.
	// wide char.
	Length = strlen(ai_pcStr);
	pcString = M4CppToANSI(ai_pcStr, (int&)Length);

	//Comprueba la existencia de caracteres especiales y si existen hace una llamada
	// al metodo PrepareForPostScript
	oStrAux.StrCpy(pcString);

	delete [] pcString;
	
	if(strpbrk(oStrAux.InChar(0),"\\()"))
	{
		PrepareForPostScript(&oStrAux, &Length);
	};

    //Convierte caracteres extendidos a ISO
	if (!ai_bRawText) {
		ClMIT_SysInfo::ConvertToIsoWithEscape(&oStrAux, &Length);
	} else {
		ClMIT_SysInfo::ConvertToEscape(&oStrAux, &Length);
	} ;

	//Imprime Cadena
	Length=sprintf(m_pcBuff, 
		PDF_BEGIN_TEXT PDF_PRINT_STRING PDF_END_TEXT, dXPos, dYPos, 
			oStrAux.InChar(0));

	if((Length+1)>StENG_TaskEnv::DvcPDF_MAX_SIZE){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCPDF_MSG_20 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError();
	};

	PageBuffer->Append(m_pcBuff,Length);

	if(ai_iTargetID!=ClENG_OutDevice::DVC_NO_INDEX_TARGET && m_bTOC ) {
		ChangeAuxIndexValue(ai_iTargetID, m_poPages[ai_iPage].m_IdPageObj ) ;
	} ;

}

void ClENG_DvcPDF::PrintNCharPi(
	ClMIT_Str *ai_poIdPi, m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY,
	size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID)
{
	ClENG_OutDevice::StPIParams PIParams;
	ClMIT_Str StrAux;

	//El proceso es el mismo que en PCL
	StrAux.StrDuplicateChar(0, ai_szNChar,'X');

	PIParams.m_oIdPi.StrCpy(ai_poIdPi->InChar(0));
	PrintNChar(ai_iPage, ai_iX, ai_iY, StrAux, ai_szNChar, ai_bRawText, ai_pcHyperlink, 
		ai_iTargetID, &PIParams);

	//Añadimos PI a la lista
	if(PIParams.m_szNChars){
		m_oOutRepository.AddNewPi( ai_poIdPi->InChar(0), ai_iPage, 
			PIParams.m_uiOff, PIParams.m_szNChars ) ;
	};
}


void ClENG_DvcPDF::PrepareForPostScript(ClMIT_Str *ao_poStr, size_t *ai_szNChar)
{
	size_t begIdx, currentIdx ;

	//Escapa los caracteres especiales \()
	//Ojo, un StrInsert puede realocar la cadena, usar siempre indices

	for ( begIdx = 0; M4_TRUE ;  ){
		currentIdx = begIdx + strcspn( ao_poStr->InChar(begIdx), "\\()")  ;
		if( currentIdx >= *ai_szNChar ) break;
		ao_poStr->StrInsertChar( currentIdx, 1, '\\');
		begIdx=currentIdx+2; //+2 porque acabo de insertar un char !
		*ai_szNChar+=1;
	};
}


void ClENG_DvcPDF::ResolvePi(ClMIT_Str  *ai_poIdPi,m4pchar_t ai_pcStr) 
{
	StPi_t piData ;

	m4uint32_t Len;
	m4uint32_t PosActual;
	ClMIT_Str StrAux ;

	m4pchar_t pcString;
	
	//La forma de resolver las PI en PostScript es similar a la de PCL, solo que
	// los Offset de las cadenas imprimidas a RAM o fichero son diferentes.
  
	// Tomo el Pi a imprimir ( le digo que imprima el error si no lo encuentra)
	if ( m_oOutRepository.ExtractPi( ai_poIdPi->InChar(0),  &piData ) ) {
		// UNICODE REPORTS. Bug 163475
		// Convertir una cadena (multi byte) a multibyte ansi. Si estamos en ansi, te devuelve
		// ansi, es decir, esa misma cadena. Si estamos en unicode, pasa de utf8 a ansi.
		// wide char.
		Len = -1;
		pcString = M4CppToANSI(ai_pcStr, (int&)Len);
	    if (Len > piData.m_NBytes) {

		   // Sacar mensaje de que se va a truncar
		   *m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCPDF_MSG_40 ] ) 
				<< (m4int16_t)piData.m_NPage << (m4uint32_t)piData.m_NBytes 
				<< ai_pcStr << SEND_MSG ;
		   //Efecto Local
		}
			
	   // Creo la cadena a imprimir
		// -- si sobra espacion relleno con blancos hasta PiNBytes 
		// -- si falta se trunca
		StrAux.Accommodate(piData.m_NBytes+1) ;
 		Len = sprintf(StrAux.InChar(0),"%-*.*s", piData.m_NBytes, piData.m_NBytes, pcString);
		StrAux.OverStringValid() ;	//No haria falta, se pone por si acaso

		delete [] pcString;


	    // Imprimo la cadena asociada en RAM o en fichero, según dónde de encuentre el Pi
		if ( piData.m_Place == StPi_t::PI_IN_RAM) {  
			// Imprimir en el buffer
			M4ClBuffer::StCopy Param = {StrAux.InChar(0), piData.m_NBytes };	           
			m_poLayoutPages[ piData.m_NPage ].m_poActualBuff->Flush(M4ClBuffer::MemToBuffManip,
				&Param, piData.m_Offset, piData.m_NBytes );

		} else { 
			
			// Imprimir directamente en el fichero
			m4bool_t bWasClosed=M4_FALSE;
			
			//Tomo la posicion actual del fichero, y me posiciono en el offset
			if(  ! piData.m_poFile->IsOpen() ) {
				bWasClosed=M4_TRUE;
				piData.m_poFile->Open( ClMIT_File::READ_WRITE, ClMIT_Trace::PROPAGATE_ERROR) ;
			};

			PosActual = piData.m_poFile->GetPos();
			piData.m_poFile->SetPos( piData.m_Offset );

			// Imprimo y dejo en fichero en la posicion inicial
			piData.m_poFile->W(StrAux.InChar(0), piData.m_NBytes );
			piData.m_poFile->SetPos(PosActual);
			if(bWasClosed){
				//Solo lo cierro si ya lo estaba
				piData.m_poFile->Close();
			};
		};
  };

  return;
}


void ClENG_DvcPDF::EOP(m4int16_t ai_iPage) 
{
	ClENG_OutDevice::EOP(ai_iPage);
}


//--------------------- Graficos ---------------------------------------

void ClENG_DvcPDF::SelectColourDevice(m4int16_t ai_iPage, m4uint32_t ai_uiIdColour)
{
	M4ClBuffer *PageBuffer=m_poLayoutPages[ai_iPage].m_poActualBuff;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;
	m4int16_t PIndex;
	ClENG_Colour* pColour;

	PIndex = m_oFGPalette.GetIndex(ai_uiIdColour);
	pColour = m_oFGPalette.GetColourByIndex(PIndex);

	sprintf(pcBuff, PDF_SET_RGB, (pColour->GetR()/255.0));
	strcat(pcBuff, PDF_SPACE);
	sprintf(&pcBuff[strlen(pcBuff)], PDF_SET_RGB, (pColour->GetG()/255.0));
	strcat(pcBuff, PDF_SPACE);
	sprintf(&pcBuff[strlen(pcBuff)], PDF_SET_RGB, (pColour->GetB()/255.0));
	strcat(pcBuff, PDF_SPACE);
	strcat(pcBuff, PDF_SET_FG_COLOUR);
	
	sprintf(&pcBuff[strlen(pcBuff)], PDF_SET_RGB, (pColour->GetR()/255.0));
	strcat(pcBuff, PDF_SPACE);
	sprintf(&pcBuff[strlen(pcBuff)], PDF_SET_RGB, (pColour->GetG()/255.0));
	strcat(pcBuff, PDF_SPACE);
	sprintf(&pcBuff[strlen(pcBuff)], PDF_SET_RGB, (pColour->GetB()/255.0));
	strcat(pcBuff, PDF_SPACE);
	strcat(pcBuff, PDF_SET_BG_COLOUR);
	
	ClMIT_Str::OverStringValid(pcBuff, StENG_TaskEnv::DvcPDF_MAX_SIZE);
	PageBuffer->AppendStr(pcBuff);
}


size_t ClENG_DvcPDF::SetActivePalette (ClENG_Palette* ai_poPalette, m4char_t* ai_pcBuff, size_t ai_szFrom, size_t ai_szMaxSize)
{
	m4int16_t PIndex;
	ClENG_Colour* pColour;
	size_t Written;

	Written=ai_szFrom;

	//Written += sprintf(&ai_pcBuff[Written], PDF_PALETTE_OPEN, ai_poPalette->GetSize()-1);

	//for(PIndex=0; PIndex<ai_poPalette->GetSize(); ++PIndex){

	//	pColour = ai_poPalette->GetColourByIndex(PIndex);
	//	Written += sprintf(&ai_pcBuff[Written], PDF_SET_PALETTE_ENTRY, (long)pColour->GetR(),
	//		               (long)pColour->GetG(), (long)pColour->GetB());
	//};

	//Written += sprintf(&ai_pcBuff[Written], PDF_PALETTE_CLOSE PDF_NEW_LINE);
	//ClMIT_Str::OverStringValid(ai_pcBuff, ai_szMaxSize, Written+1);

	return Written;
}


void ClENG_DvcPDF::PrintLine( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X1, DvcUnit_t ai_Y1, DvcUnit_t ai_X2, DvcUnit_t ai_Y2,
	DvcUnit_t ai_LineWidth, STYLE ai_Style ) 
{
	m4double_t dX1Pos = 0;
	m4double_t dY1Pos = 0;
	m4double_t dX2Pos = 0;
	m4double_t dY2Pos = 0;
	m4double_t dLineWidth = 0;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* m_pcBuff=TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;

	dX1Pos = m_poPagePrinter[ai_iPage].GetPhysicalX(ai_X1) ;
	dX2Pos = m_poPagePrinter[ai_iPage].GetPhysicalX(ai_X2) ;

	dY1Pos = m_poPagePrinter[ai_iPage].GetPhysicalY(ai_Y1, NULL) ;
	dY2Pos = m_poPagePrinter[ai_iPage].GetPhysicalY(ai_Y2, NULL) ;

	//Estilo
	if (ai_Style==SOLID) {
		sprintf(m_pcBuff, PDF_SOLID);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DASH) {
		sprintf(m_pcBuff, PDF_DASH);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DOT) {
		sprintf(m_pcBuff, PDF_DOT);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DASH_DOT) {
		sprintf(m_pcBuff, PDF_DASH_DOT);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DASH_DOT_DOT) {
		sprintf(m_pcBuff, PDF_DASH_DOT_DOT);
		PageBuffer->AppendStr(m_pcBuff);
	};
	
	//Dibuja Linea
	dLineWidth = ClMetric::DvcToPoint(ai_LineWidth);
	sprintf(m_pcBuff, PDF_LINE_WIDTH, dLineWidth);
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PDF_MOVE_TO, dX1Pos, (dY1Pos - 1));
	PageBuffer->AppendStr(m_pcBuff);

	sprintf(m_pcBuff, PDF_DRAW_LINE, dX2Pos, (dY2Pos - 1));
	PageBuffer->AppendStr(m_pcBuff);
	// Bug 149466. PDF_CLOSE_PATH provoca que se cierre el Stroke, es decir,
	// se dibuja una línea adicional hasta la posición de inicio. Esto provoca
	// que si la línea no es sólida se vea distinta según sus dimensiones.
	// Con PDF_END_PATH finaliza el Stroke pero no lo cierra.
	sprintf(m_pcBuff, PDF_END_PATH);
	PageBuffer->AppendStr(m_pcBuff);
}

void ClENG_DvcPDF::FillZone( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height ) 
{
	m4double_t dXPos = 0;
	m4double_t dYPos = 0;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* m_pcBuff=TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	m4uint32_t uiIdColour;
	m4int16_t PIndex;
	ClENG_Colour* pColour;

	//Selecciona el color
	uiIdColour = GetLastColour(ai_iPage) ;
	PIndex = m_oFGPalette.GetIndex(uiIdColour);
	pColour = m_oFGPalette.GetColourByIndex(PIndex);

	dXPos = m_poPagePrinter[ai_iPage].GetPhysicalX(ai_X) ;
	dYPos = m_poPagePrinter[ai_iPage].GetPhysicalY(ai_Y, NULL) ;

	//Dibuja caja
	sprintf(m_pcBuff, PDF_SET_RGB, (pColour->GetR()/255.0));
	strcat(m_pcBuff, PDF_SPACE);
	sprintf(&m_pcBuff[strlen(m_pcBuff)], PDF_SET_RGB, (pColour->GetG()/255.0));
	strcat(m_pcBuff, PDF_SPACE);
	sprintf(&m_pcBuff[strlen(m_pcBuff)], PDF_SET_RGB, (pColour->GetB()/255.0));
	strcat(m_pcBuff, PDF_SPACE);
	strcat(m_pcBuff, PDF_SET_BG_COLOUR);
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PDF_MOVE_TO, dXPos, (dYPos - 1));
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PDF_DRAW_RLINE, dXPos, 
		(dYPos - ClMetric::DvcToPoint(ai_Height)));
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PDF_DRAW_RLINE, 
		(dXPos + ClMetric::DvcToPoint(ai_Width)), 
		(dYPos - ClMetric::DvcToPoint(ai_Height)));
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PDF_DRAW_RLINE, 
		(dXPos + ClMetric::DvcToPoint(ai_Width)), 
		(dYPos - 1));
	PageBuffer->AppendStr(m_pcBuff);
	PageBuffer->AppendStr(PDF_FILL);
	PageBuffer->AppendStr(PDF_NEW_LINE);
}	

void ClENG_DvcPDF::PrintFrame( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
	DvcUnit_t ai_LineWidth, STYLE ai_Style ) 
{
	m4double_t dXPos = 0;
	m4double_t dYPos = 0;
	m4double_t dLineWidth = 0;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* m_pcBuff=TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	
	dXPos = m_poPagePrinter[ai_iPage].GetPhysicalX(ai_X) ;
	dYPos = m_poPagePrinter[ai_iPage].GetPhysicalY(ai_Y, NULL) ;
		
	//Estilo
	if (ai_Style==SOLID) {
		sprintf(m_pcBuff, PDF_SOLID);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DASH) {
		sprintf(m_pcBuff, PDF_DASH);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DOT) {
		sprintf(m_pcBuff, PDF_DOT);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DASH_DOT) {
		sprintf(m_pcBuff, PDF_DASH_DOT);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DASH_DOT_DOT) {
		sprintf(m_pcBuff, PDF_DASH_DOT_DOT);
		PageBuffer->AppendStr(m_pcBuff);
	};
	
	// Dibuja caja
	dLineWidth = ClMetric::DvcToPoint(ai_LineWidth);
	sprintf(m_pcBuff, PDF_LINE_WIDTH, dLineWidth);
	PageBuffer->AppendStr(m_pcBuff);

	sprintf(m_pcBuff, PDF_MOVE_TO, dXPos, (dYPos - 1));
	PageBuffer->AppendStr(m_pcBuff);

	sprintf(m_pcBuff, PDF_DRAW_RLINE, 
		dXPos, 
		(dYPos - ClMetric::DvcToPoint(ai_Height) - 1));
	PageBuffer->AppendStr(m_pcBuff);

	sprintf(m_pcBuff, PDF_DRAW_RLINE, 
		(dXPos + ClMetric::DvcToPoint(ai_Width)), 
		(dYPos - ClMetric::DvcToPoint(ai_Height) - 1));
	PageBuffer->AppendStr(m_pcBuff);

	sprintf(m_pcBuff, PDF_DRAW_RLINE, 
		(dXPos + ClMetric::DvcToPoint(ai_Width)), 
		(dYPos - 1));
	PageBuffer->AppendStr(m_pcBuff);

	PageBuffer->AppendStr(PDF_CLOSE_PATH);
	PageBuffer->AppendStr(PDF_NEW_LINE);
}


void ClENG_DvcPDF::PrintRasterFile(
		m4int16_t ai_iPage,
		DvcUnit_t ai_X,				DvcUnit_t ai_Y,
		DvcUnit_t ai_SizeX,			DvcUnit_t ai_SizeY,
		const m4char_t * ai_pcPath, m4char_t *ai_pcHyperlink,
		m4uint16_t ai_iTargetID, 	m4bool_t m_bIsTempWebPath )
{
	M4ClBuffer* pageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM());
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;
	m4bool_t SwOk=M4_TRUE ;

	ClENG_RasterMap::FILE_FORMATS fileFormat ;
	ClMIT_Str pathAux, extAux, glbName ;
	StImages *stImages ;
	StImagesList_t::iterator imgIt ;
	M4ClBuffer imgBuffer ;
	m4int16_t bpp ;
	m4double_t dXPos = 0;
	m4double_t dYPos = 0;

	imgBuffer.Init(1024*2) ;

	//---Primero formo nombre para ver si existe

	fileFormat = ClENG_RasterMap::GetFileFormat(ai_pcPath) ;
	ClMIT_File::SplitPath(ai_pcPath, &pathAux, &glbName, &extAux);
	if ( fileFormat != ClENG_RasterMap::JPG ) {
		ClMIT_File::ConcatFileExt(&glbName, 
			ClENG_RasterMap::GetExtension(ClENG_RasterMap::JPG) ) ;
	} else {
		ClMIT_File::ConcatFileExt(&glbName, extAux) ;
	} ;

	//---Veo si existe

	imgIt = m_oImagesList.find(glbName) ;
	if ( imgIt==m_oImagesList.end() ) {

		//***Solo si no existe, creo el objeto grafico en fichero y notifico en lista global

		ClENG_RasterMap map ;
		m4uint32_t imgWidth, imgHeight ;
		
		ClENG_PhysicalFile *F ;
		m4bool_t swRedirect ;

		StImages stImgAux ;

		//Dejara en imgBuffer el JPG tal cual, y actualizara imgWidth e imgHeight 

		if (fileFormat==ClENG_RasterMap::JPG) {
		
			//Es JPEG, no requiere conversion

			ClMIT_File JpgFile ;

			if ( ! JpgFile.Open( ai_pcPath, ClMIT_File::EXIST, 
				ClMIT_File::READ,ClMIT_File::TRY_OPEN, M4_FALSE) ) 
			{
				SwOk=M4_FALSE ;
				goto exit ;
			} ;

			ClENG_JpgEncoder::LocateJPG_HeaderData(&JpgFile, &imgWidth, &imgHeight, &bpp) ;
			imgBuffer.AppendFile(&JpgFile) ;

			JpgFile.Close() ;

		}  else {

			//No es JPEG, hay que convertirlo a JPEG

			map.Init() ;
			SwOk = map.LoadFromFile(ai_pcPath);
			if (!SwOk) goto exit ;

			imgWidth = map.GetRasterX() ;
			imgHeight = map.GetRasterY() ;

			if ( ! m_oJPG_Enc.Encode( &map, &imgBuffer) ) {
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
		} ;


		//***Ahora a grabar el objeto imagen en FICHERO
		
		stImgAux.m_oJPG_Id.StrCpy(glbName) ;
		stImgAux.m_uiId = NewObjId() ;

		F = m_oOutRepository.GetFlushFile(ai_iPage, &swRedirect ) ;

		sprintf(pcBuff, PDF_COMMENT_RASTER, stImgAux.m_oJPG_Id.InChar(0) );
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		if (m_bIncludeComent) F->WStr(pcBuff) ;
		
		WriteBeginObject(F,stImgAux.m_uiId) ;

		F->WStr(PDF_OPEN_OBJECT PDF_BEGIN_IMAGE);

		sprintf(pcBuff, PDF_IMAGE_WIDTH, (long)imgWidth);
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff) ;

		sprintf(pcBuff, PDF_IMAGE_HEIGHT, (long)imgHeight);
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff) ;

		//El formato final siempre es JPG a 8 bits
		sprintf(pcBuff, PDF_BITS_PER_COMPONENT, (int)8);
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff) ;

		F->WStr( PDF_COLOR_RGB PDF_FILTER_JPEG);

		sprintf(pcBuff, PDF_IMAGE_LENGTH, (long)imgBuffer.GetTotal() );
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff) ;

		F->WStr(PDF_CLOSE_OBJECT PDF_IMAGE_DATA) ;

		F->DumpBuffer(&imgBuffer) ;

		F->WStr(PDF_END_IMAGE_DATA PDF_END_OBJECT PDF_NEW_LINE);

		//***Notifico a nivel global

		imgIt = (m_oImagesList.insert( StImagesList_t::value_type(
			stImgAux.m_oJPG_Id, stImgAux) ) ).first ;
		stImages = & (*imgIt).second ;
	

	} else {

		stImages = & (*imgIt).second ;

	};
	//<----- stImages apunta siempre al grafico en cuestion

	//***Notifico uso a esta pagina

	m_poPages[ai_iPage].m_oUsedImages.insert( ObjIdMap_t::value_type(
		stImages->m_uiId, stImages->m_uiId) ) ;

	//***Inserto en obj stream la referencia al grafico

	dXPos = m_poPagePrinter[ai_iPage].GetPhysicalX(ai_X) ;
	dYPos = m_poPagePrinter[ai_iPage].GetPhysicalY(ai_Y+ai_SizeY, NULL) ;

	pageBuffer->AppendStr(PDF_IMAGE_START);
	sprintf(pcBuff, PDF_IMAGE_POSITION, 
		(double)ClMetric::DvcToPoint(ai_SizeX), 
		(double)ClMetric::DvcToPoint(ai_SizeY), 
		dXPos, dYPos ) ;
	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
	pageBuffer->AppendStr(pcBuff);

	sprintf(pcBuff, PDF_THROW_IMAGE, (unsigned long)stImages->m_uiId);
	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
	pageBuffer->AppendStr(pcBuff);
	
	pageBuffer->AppendStr(PDF_IMAGE_END);
exit:
	;
}

//===================== TOC - INDICES ========================================


ClENG_DvcPDF::ObjId_t ClENG_DvcPDF::PrintIndex(ClENG_PhysicalFile *F) 
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM());
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;

	ObjId_t brkFather, indexEntryCatalog ;
	m4uint32_t brkCount		; 
	m4uint32_t indexCatCount	; ObjId_t indexCatFirst, indexCatLast ;
	
	IndexNodeList_t nodeList ;
	IndexNodeList_t::iterator itNL ;
	
	ClENG_OutDevice::IndexVector_t::iterator it  ;

	PrintIndex_t auxVector, rootVector ;
	StPrintIndexAux auxStPrint ;

	//Si no hay nada, no se imprime TOC

	if (m_oBreakIndexRoots.size()==0) return 0 ;
	
	if (m_bIncludeComent) F->WStr( PDF_COMMENT_TOC_b ) ;
	
	//Ya veremos quien es el objeto brkFather, pero el id hace falta para crearlos

	brkFather = NewObjId() ;

	//Imprime recursivamente las entradas correspondientes a los break outlines

	for ( it=m_oBreakIndexRoots.begin() ; it!=m_oBreakIndexRoots.end(); ++it ) {
		auxStPrint.m_ObjId		= NewObjId() ;
		auxStPrint.m_poIndex	= (*it) ;
		auxVector.push_back(auxStPrint) ;
	} ;
	PrintIndexLevel(F, brkFather, auxVector, &brkCount ) ;

	//Ahora, dependiendo de si hay padres o no, asignaremos el brkFather

	if ( m_oBreakIndexRoots.size()>0 ) {
		m_oBreakIndexRoots[0]->GetPathToThis( &nodeList ) ;
	} ;
	if ( nodeList.size()>0 ) {
		
		//***Hay camino hasta el padre 
		//		Crear lista de outlines, el ultimo de ellos hace de brkFather.
		//		El indexEntryCatalog es un objeto aparte, que enganchara con el primero de la 
		//			lista de outlines.

		for ( itNL=nodeList.begin() ; itNL!=nodeList.end(); ++itNL ) {
			auxStPrint.m_ObjId		= NewObjId() ;
			auxStPrint.m_poIndex	= (*itNL).second ;
			rootVector.push_back(auxStPrint) ;
		} ;
		indexEntryCatalog = NewObjId() ;
		PrintRootListEntry( 
			F, indexEntryCatalog, rootVector, 
			auxVector[0].m_ObjId, auxVector[ auxVector.size()-1 ].m_ObjId, brkCount ) ;

		indexCatCount = brkCount + nodeList.size() ;
		indexCatLast = indexCatFirst = rootVector[0].m_ObjId ;

	} else {

		//***NO hay camino hasta el padre 
		//		El brkFather sera el propio indexEntryCatalog. Este engancha con el grupo brk,
		//			es decir con las vbles brkFirst. brkLast, brkCount

		indexEntryCatalog = brkFather ;
		indexCatCount = brkCount ;
		indexCatFirst = auxVector[0].m_ObjId ; 
		indexCatLast  = auxVector[ auxVector.size()-1 ].m_ObjId ;
	} ;

	//Escribe informacion objeto indexEntryCatalog
	
	WriteBeginObject(F,indexEntryCatalog);

	sprintf( pcBuff, PDF_OPEN_OBJECT PDF_TOC_COUNT, (unsigned long)indexCatCount ) ;
	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
	F->WStr(pcBuff) ;

	F->WStr(PDF_TOC_ROOT_TYPE) ;

	if (indexCatFirst!=0) {
		sprintf( pcBuff, PDF_TOC_FIRST PDF_TOC_LAST, 
			(unsigned long)indexCatFirst, (unsigned long)indexCatLast) ;
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff) ;
	} ;

	F->WStr(PDF_CLOSE_OBJECT PDF_END_OBJECT) ;

	if (m_bIncludeComent) F->WStr( PDF_COMMENT_TOC_e ) ;
	
	return indexEntryCatalog ;
} 

void ClENG_DvcPDF::PrintIndexLevel(
		ClENG_PhysicalFile *F,
		ObjId_t ai_iFatherObj,
		ClENG_DvcPDF::PrintIndex_t &ai_oBrotherNodes, m4uint32_t *ai_RecursiveLevelCount )
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM());
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;

	IndexNodeList_t *list ;
	IndexNodeList_t::iterator it ;
	m4uint32_t AuxCount, thisRecursive ;

	PrintIndex_t auxVector ;
	StPrintIndexAux auxStPrint ;

	m4uint32_t B ;

	//Bucle principal de hermanos

	for ( B=0, thisRecursive=0; B < ai_oBrotherNodes.size(); ++B ) {

		//Montamos vector axiliar para procesar hijos de actual hermano
	
		list = ai_oBrotherNodes[B].m_poIndex->GetNodeList() ;
		for ( it = list->begin(), auxVector.clear() ; it != list->end(); ++it ) {
			auxStPrint.m_poIndex = (*it).second ;
			auxStPrint.m_ObjId = NewObjId() ;
			auxVector.push_back( auxStPrint )  ; 
		} ;
		
		//Procesamos hijos de actual hermano

		PrintIndexLevel(F, ai_oBrotherNodes[B].m_ObjId, auxVector, &AuxCount ) ;
		thisRecursive += AuxCount ;

		//Creamos actual hermano

		WriteBeginObject(F, ai_oBrotherNodes[B].m_ObjId );

		sprintf( pcBuff, PDF_OPEN_OBJECT PDF_PARENT, (unsigned long)ai_iFatherObj ) ;
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff) ;

		sprintf( pcBuff, PDF_TOC_DEST, (unsigned long)
			atol(ai_oBrotherNodes[B].m_poIndex->GetAuxValue()) ) ;
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff) ;

		sprintf( pcBuff, PDF_TOC_TITLE, ai_oBrotherNodes[B].m_poIndex->GetIndexString() ) ;
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff) ;

		if (B>0) {
			sprintf( pcBuff, PDF_TOC_PREV, (unsigned long)ai_oBrotherNodes[B-1].m_ObjId ) ;
			ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
			F->WStr(pcBuff) ;
		} ;

		if ( B < ai_oBrotherNodes.size()-1 ) {
			sprintf( pcBuff, PDF_TOC_NEXT, (unsigned long)ai_oBrotherNodes[B+1].m_ObjId ) ;
			ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
			F->WStr(pcBuff) ;
		} ;

		if (auxVector.size()>0) {
			sprintf( pcBuff, PDF_TOC_FIRST PDF_TOC_LAST PDF_TOC_COUNT, 
				(unsigned long)auxVector[0].m_ObjId,
				(unsigned long)auxVector[ auxVector.size()-1 ].m_ObjId,
				(unsigned long)AuxCount ) ;
			ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
			F->WStr(pcBuff) ;
		} ;
	
		F->WStr(PDF_CLOSE_OBJECT PDF_END_OBJECT) ;
	
		++thisRecursive ;
	} ;

	*ai_RecursiveLevelCount = thisRecursive ;
}

void ClENG_DvcPDF::PrintRootListEntry(
	ClENG_PhysicalFile *F,
	ObjId_t ai_iFatherObj, 
	ClENG_DvcPDF::PrintIndex_t &ai_oRootList, 
	ClENG_DvcPDF::ObjId_t ai_BrkFirst, ClENG_DvcPDF::ObjId_t ai_BrkLast, 
	m4uint32_t ai_BrkCount)
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM());
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;

	ObjId_t parent ;

	m4uint32_t B ;

	//Bucle principal nodos root

	for ( B=0, parent=ai_iFatherObj ; B < ai_oRootList.size(); ++B ) {

		WriteBeginObject(F, ai_oRootList[B].m_ObjId );

		sprintf( pcBuff, PDF_OPEN_OBJECT PDF_PARENT, (unsigned long)parent ) ;
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff) ;

		sprintf( pcBuff, PDF_TOC_DEST, (unsigned long)
			atol(ai_oRootList[B].m_poIndex->GetAuxValue()) ) ;
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff) ;

		sprintf( pcBuff, PDF_TOC_TITLE, ai_oRootList[B].m_poIndex->GetIndexString() ) ;
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
		F->WStr(pcBuff) ;

		//No tienene previo ni next, solo hay uno por nivel

		//First, last, count es el siguiente a procesar, o si es el ultimo, enganchar con brk
		
		if ( B!=ai_oRootList.size()-1 ) {
			sprintf( pcBuff, PDF_TOC_FIRST PDF_TOC_LAST PDF_TOC_COUNT, 
				(unsigned long)ai_oRootList[B+1].m_ObjId,
				(unsigned long)ai_oRootList[B+1].m_ObjId,
				(unsigned long)(ai_BrkCount+ ai_oRootList.size()-B-1 ) ) ;
			ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
			F->WStr(pcBuff) ;
		} else {
			sprintf( pcBuff, PDF_TOC_FIRST PDF_TOC_LAST PDF_TOC_COUNT, 
				(unsigned long)ai_BrkFirst,
				(unsigned long)ai_BrkLast,
				(unsigned long)ai_BrkCount ) ;
			ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPDF_MAX_SIZE) ;
			F->WStr(pcBuff) ;
		} ;
		parent = ai_oRootList[B].m_ObjId ;
	} ;
}

void ClENG_DvcPDF::ChangeAuxIndexValue(m4uint32_t ai_uiTargetID, m4uint32_t ai_PageId)
{
	ClENG_IndexNode *node ;
	m4char_t strAux[100];

	node = m_oIndexRootNode.GetNode(ai_uiTargetID, ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!node) return ;

	sprintf(strAux,"%lu",(unsigned long)ai_PageId) ;
	ClMIT_Str::OverStringValid(strAux,100) ;
	node->SetAuxValue(strAux) ;
};

void ClENG_DvcPDF::TrueTypeToPDF_Name(ClFONT_M_Font * ai_poFont, ClMIT_Str *ao_poStr) 
{
	size_t pos, len ;

	ao_poStr->StrCpy(ai_poFont->RVW_GetFaceName()) ;
	while (M4_TRUE) {
		pos = strcspn(ao_poStr->InChar(0), " ")  ;
		if ( pos==ao_poStr->StrLen() ) {
			break ;
		} ;
		len =  strspn(ao_poStr->InChar(pos), " ")  ;
		ao_poStr->StrDeleteSegment(pos,len) ;
	} ;
	if ( ai_poFont->IsBold() && ! ai_poFont->IsItalic() ) {
		ao_poStr->StrCat(",Bold") ;
	} else if ( !ai_poFont->IsBold() && ai_poFont->IsItalic() ) {
		ao_poStr->StrCat(",Italic") ;
	} else if ( ai_poFont->IsBold() && ai_poFont->IsItalic() ) {
		ao_poStr->StrCat(",BoldItalic") ;
	} ;

}

//======================================================================ClENG_PDF_Page

ClENG_PDF_Page::ClENG_PDF_Page()
{
}

void ClENG_PDF_Page::End()
{
	//ClENG_PagePrinter::End() ; NO tiene !
}

void ClENG_PDF_Page::Init( ORIENTATION ai_Orientation, PAPER_SIZE  ai_PageSize, 
		PAPER_SOURCE  ai_PaperSource, DvcUnit_t ai_SizeX,   DvcUnit_t ai_SizeY, 
		DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY )  
{
	unsigned long r ;

	ClENG_PDF_Page::End() ;

	ClENG_PagePrinter::Init( ai_Orientation, ai_PageSize, 
		ai_PaperSource, ai_SizeX, ai_SizeY ) ;

	//Init del PagePrinter ya cambia los Size si es LandScape

	m_MediaBoxX = ClMetric::DvcToRoundPoint( GetSizeX() ) ;
	m_MediaBoxY = ClMetric::DvcToRoundPoint( GetSizeY() ) ;

	if ( GetOrientation()==ClENG_PagePrinter::PORTRAIT) {

		m_TotalXOffset = ai_OffsetX + PDF_OFFSET_X ;
		m_TotalYOffset = ai_OffsetY + PDF_OFFSET_Y ;
	
	} else {
	
		m_TotalXOffset = ai_OffsetY + PDF_OFFSET_Y ;
		m_TotalYOffset = ai_OffsetX + PDF_OFFSET_X ;
	} ;

}

m4double_t ClENG_PDF_Page::GetPhysicalX(DvcUnit_t ai_X)
{ 
	return ClMetric::DvcToPoint( ai_X + m_TotalXOffset ) ;
}

m4double_t ClENG_PDF_Page::GetPhysicalY(DvcUnit_t ai_Y, ClFONT_M_Font *ai_poLastFont )
{ 
	m4double_t r ;

	r = ai_Y + m_TotalYOffset + (ai_poLastFont ? ai_poLastFont->GetDvcHeight() : 0 ) ; 
	r = m_MediaBoxY - ClMetric::DvcToPoint(r) ;

	return r ;
}
