//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvcpsc.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             25/11/98
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Manuel Lazcano Pérez
//	Modifications:	
//
// Definition:
//
//    This module manages the PostScript Device Printing
//
//
//==============================================================================


#include "m4string.hpp"

#include "eng.h"
#include "engdf.inl"
#include "sysinfo.h"

#include "font.h"
#include "clfile.h"
#include "cldvcpsc.hmg"
#include "cldvcpsc.h"
#include "rasterg.h"
#include "softtrans.h"

#include "m4unicode.hpp"


//------------------- Especificaciones PostScript ---------------------------

//!!!!CUIDADO CON LOS %: si se procesan por printf, %%, si se tratan como cadenas son %

//------------------- Offsets de pagina (Limites fisicos) -------------------

#define PSC_OFFSET_Y 2000
#define PSC_OFFSET_X 5000
#define PSC_OFFSET_GRAPH 2000

//------------------- PostScript Reset --------------------------------------

#define PSC_FILE_HEADER		"%!PS-Adobe-1.0\r\n"

#define PSC_SAVE_STACK		"/server save def\r\n"
#define PSC_BEGIN_DICT		"/sqdict 200 dict def\r\nsqdict begin\r\n"
#define PSC_END_DICT		"end\r\n"
#define PSC_BEGIN_PROLOG	"%%BeginProlog\r\n"
#define PSC_END_PROLOG		"%%EndProlog\r\n"

//------------------- SetPageDevice -----------------------------------------
// Los setup de Page Device van justo despues del End Prolog y se aplican a 
//cada LayoutPage individualmente

#define PSC_BEGIN_SETUP		"%%BeginSetup\r\n"
#define PSC_BEGIN_FEATURE	"%%BeginFeature:\r\n"
#define PSC_END_FEATURE		"%%EndFeature\r\n"
#define PSC_END_SETUP		"%%EndSetup\r\n"

#define PSC_NUM_COPIES		"<</NumCopies %i>>setpagedevice\r\n"
#define PSC_DUPLEX			"<</Duplex true>>setpagedevice\r\n"
#define PSC_SIMPLEX			"<</Duplex false>>setpagedevice\r\n"
#define PSC_TUMBLE_FALSE	"<</Tumble false>>setpagedevice\r\n"
#define PSC_TUMBLE_TRUE		"<</Tumble true>>setpagedevice\r\n"
#define PSC_PORTRAIT		"<</Orientation 0>>setpagedevice\r\n"
#define PSC_LANDSCAPE		"<</Orientation 1>>setpagedevice\r\n" 
#define PSC_MANUAL_SOURCE	"<</ManualFeed>>setpagedevice\r\n"

#define PSC_LANDSCAPE_SETUP "/land  %Setup for landscape\r\n  {/ypos 595 0.5 72 mul sub def\r\n  initmatrix\r\n  90 rotate\r\n  0 -595 translate\r\n  0.5 72 mul ypos moveto} def\r\n" 
#define PSC_LANDSCAPE_PROC	"/landscape\r\n  {land\r\n  /pgtop 595 0.5 72 mul sub def\r\n/np {   % New-page macro\r\n    showpage\r\n    land\r\n  }   def\r\n} def\r\n"
#define PSC_LANDSCAPE_APPLY "landscape"

//------------------- Tamaño de hoja ----------------------------------------

#define PSC_A4_PAPER_SIZE "<</PageSize[620 877]>>\r\n"
#define PSC_A3_PAPER_SIZE "<</PageSize[876 1240]>>\r\n"
#define PSC_LEGAL_PAPER_SIZE "<</PageSize[612 1008]>>\r\n"
#define PSC_LETTER_PAPER_SIZE "<</PageSize[612 792]>>\r\n"
#define PSC_EXECUTIVE_PAPER_SIZE "<</PageSize[522 788]>>\r\n"
#define PSC_PERSONAL_PAPER_SIZE "<</PageSize[%li %li]>>\r\n"

//------------------- Tratamiento de paginas --------------------------------

#define PSC_PAGE_ID_B		"%%Page: "
//Escribir exactamente 6 digitos, 1 b, 6 digitos
#define PSC_PAGE_ID_E		"XXXXXX XXXXXX\r\n" 

#define PSC_PAGE_DICT	"sqdict begin save\r\n"
#define PSC_NEW_PAGE	"showpage\r\n"
#define PSC_END_PAGE	"restore end\r\n"
#define PSC_NEW_LINE	"\r\n"

//------------------- Lineas ------------------------------------------------

#define PSC_FILL_COLOUR	"%li setcolor\r\n"
#define PSC_LINE_WIDTH	"%f setlinewidth\r\n"
#define PSC_MOVE_TO		"%i %i moveto\r\n"
#define PSC_DRAW_LINE	"%i %i lineto\r\n"
#define PSC_DRAW_RLINE	"%i %i rlineto\r\n"
#define PSC_CLOSE_PATH	"closepath\r\n"
#define PSC_STROKE		"stroke\r\n"
#define PSC_FILL		"fill\r\n"

#define PSC_DASH		 "[10] 0 setdash\r\n"
#define PSC_DOT			 "[1] 0 setdash\r\n"
#define PSC_DASH_DOT	 "[10 5 1 5] 0 setdash\r\n"
#define PSC_DASH_DOT_DOT "[10 5 1 5 1 5] 0 setdash\r\n"

//------------------- Fuentes -----------------------------------------------

#define PSC_SET_FONT		"/%s findfont %i scalefont setfont\r\n"
#define PSC_SET_COMPOSITE_FONT		"/m4%s findfont %i scalefont setfont\r\n"
#define PSC_EXTENDED_ISO	"/%s findfont\r\ndup length dict begin\r\n {1 index /FID ne {def}{pop pop}ifelse}forall\r\n /Encoding ISOLatin1Encoding def\r\n currentdict\r\nend\r\n/%s exch definefont pop\r\n"

#define PSC_COMPOSITE_FONT "/%s\r\n13 dict begin\r\n\t/FontName 1 index def\r\n\t/FMapType 8 def\r\n\t/ShiftIn %u def\r\n\t/ShiftOut %u def\r\n\t/Encoding [ 0 1 ] def\r\n\t/WMode 0 def\r\n\t/FontType 0 def\r\n\t/FontMatrix [1.0 0.0 0.0 1.0 0.0 0.0] def\r\n\t/FDepVector [\r\n\t\t/%s findfont\r\n\t\t/%s findfont\r\n\t] def\r\n\tcurrentdict\r\n\tend\r\n\tdefinefont pop\r\n"

//------------------- Impresion de cadenas ----------------------------------

#define PSC_NO_PRINT_STRING "%i %i moveto (%.*s) show\r\n"
#define PSC_PRINT_STRING	"%i %i moveto (%s) show\r\n"

//Offset de ") show \r\n" para las PI
#define PSC_PI_OFFSET 8 

//------------------- Color -------------------------------------------------

#define PSC_PALETTE_OPEN  "[/Indexed /DeviceRGB %i\r\n<"
#define PSC_PALETTE_CLOSE ">\r\n] setcolorspace\r\n"

#define PSC_SET_PALETTE_ENTRY	"%.2X%.2X%.2X "
#define PSC_SET_FG_COLOUR		"%li setcolor "

#define PSC_IMAGE_TRANSFER      "gsave\r\n"
#define PSC_IMAGE_TYPE			"<</ImageType 1\r\n"
#define PSC_IMAGE_SIZE			"  /Width %i /Height %i\r\n"
#define PSC_256_COLOURS_BPC		"  /BitsPerComponent 8\r\n"
#define PSC_MONOCHROME_BPC		"  /BitsPerComponent 1\r\n"
#define PSC_256_COLOUR_DECODE	"  /Decode[0 255]\r\n"
#define PSC_MONOCHROME_DECODE	"  /Decode[0 1]\r\n"
#define PSC_IMAGE_MATRIX		"  /ImageMatrix[%i 0 0 -%i 0 %i]\r\n"
#define PSC_DATA_SOURCE_FILTER	"  /DataSource currentfile /ASCIIHexDecode filter\r\n>>\r\n"
#define PSC_COLOUR_READ_DATA	"image\r\n"

//------------------- Graficos ----------------------------------------------

#define PSC_IMAGE_END				">\r\ngrestore\r\n"
#define PSC_MONOCHROME_GRAPHIC_END	"\r\ngrestore\r\n"
#define PSC_DATA_STRING				"/pat1 %i string def\r\n"
#define PSC_SET_TRANSFER			"gsave\r\n{1 exch sub} settransfer\r\n"
#define PSC_LOCATE_POS				"%i %i translate\r\n"
#define PSC_SCALE					"%i %i scale\r\n" 
#define PSC_DIMENSIONS				"%i %i %i\r\n"
#define PSC_MAP_SOURCE				"[%i 0 0 %i 0 %i]\r\n"
#define PSC_READ_DATA				"{currentfile pat1\r\n readhexstring pop}\r\nimage\r\n" 

//------------------- Liberacion de la pila PstScript VM --------------------

#define PSC_TRAILER		"%%Trailer\r\n"
#define PSC_PAGES		"%%%%Pages:%li\r\n"
#define PSC_RESET_STACK "server restore %Restore PostScript VM stack\r\n"
#define PSC_FREE_PAGES	"/EJ {} def\r\n"


//------------------- Inicializaciones --------------------------------------

ClENG_DvcPSC::ClENG_DvcPSC() 
{
	m_poPagePrinter = NULL;
	m_poPageBuffOffset = NULL ;
}

void ClENG_DvcPSC::End() 
{
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPagePrinter,
		            ClENG_PagePrinter, GetNumLayoutPages());

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPageBuffOffset,
		            size_t, GetNumLayoutPages());

	ClENG_DvcFile::End();
}

m4bool_t ClENG_DvcPSC::Init( 
		//Campos para ClENG_OutDevice::Init

		m4int16_t ai_iNumPages,
		ClENG_Palette *ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		m4bool_t ai_bForceMonoBreak,
		StFileParams *ai_FileParams, 
		m4bool_t ai_bDelete,
		StENG_DvcParams::DUPLEX_MODE ai_Duplex, 
		DECLARE_EXTERNAL_FILE ai_DeclareExtFile,
		m4bool_t ai_bCreateDir,
		ClENG_ReportOuputRepository::PRESERVE_MODE ai_Preserve, 
		m4char_t *ai_pcToPrinter, 

		//Especificos del device

		ClFONT_M_DBFont* ai_poDBFont, 
		StENG_DvcParams::TUMBLE_MODE  ai_Tumble,
		PAGE_RESOLUTION ai_MaxResolution, 
		m4int16_t ai_iNumCopies,

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk = M4_TRUE;
	ClMIT_Flag::Value_t Flags ;
	
	ClENG_DvcPSC::End() ;
	
	//Activamos todos los Flags, puesto que siempre soporta color
	Flags = 
		ClENG_OutDevice::PRELOAD_ON_DEVICE | 
		ClENG_OutDevice::COLOUR_ACTIVATED | ClENG_OutDevice::COLOUR_SUPPORTED | 
		ClENG_OutDevice::RASTER_SUPPORTED | ClENG_OutDevice::RASTER_PRELOADED |
		ClENG_OutDevice::HARD_DITHERING |  ClENG_OutDevice::HARD_SCALE ;

	SwOk = ClENG_DvcFile::Init(
		//Campos para ClENG_OutDevice::Init

		ai_iNumPages,
		Flags, ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		ai_bForceMonoBreak,	
		ai_FileParams, ai_bDelete,
		ClENG_BreakList::BREAK_TO_FILE,
		ClENG_DvcFile::GetMappingToFileDuplex(ai_Duplex), 
		ai_DeclareExtFile,
		ai_bCreateDir,
		ai_Preserve, 
		ai_pcToPrinter, 

		ClMIT_Trace::PROPAGATE_ERROR
	);
	if(!SwOk) goto exit;

	m_iNumCopies = ai_iNumCopies ;
	m_MaxResolution=ai_MaxResolution ;
	m_Duplex = ai_Duplex ;
	m_Tumble = ai_Tumble ;
	m_poDBFont = ai_poDBFont ;
	
	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPagePrinter,
		         ClENG_PagePrinter, GetNumLayoutPages());

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPageBuffOffset,
		         size_t, GetNumLayoutPages());
	
exit:

	m_poInsp->m_poTrace->TestEffect(SwOk,ai_ErrorEffect);
	return SwOk;
}


m4bool_t ClENG_DvcPSC::InitPage(m4int16_t ai_iIdPage,
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
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCPSC_MSG_10 ] ) << SEND_MSG ;
		SwOk = M4_FALSE;
		goto exit ;
	};

	m_poPagePrinter[ai_iIdPage].Init(ai_Orientation,ai_PageSize,ai_PaperSource,
		                             ai_SizeX, ai_SizeY);

	SwOk = ClENG_DvcFile::InitPage(
		ai_iIdPage, m_poPagePrinter[ai_iIdPage].GetSizeX(),
		m_poPagePrinter[ai_iIdPage].GetSizeY(), ai_OffsetX,
		ai_OffsetY, ai_szBlockBuffer, initializePhantom,
		ClMIT_Trace::PROPAGATE_ERROR) ;
	if(!SwOk) goto exit;

	if ( initializePhantom )  {
		m_poPagePrinter[ GetNumLayoutPages()-1 ].Init( 
			ai_Orientation, ai_PageSize, ai_PaperSource, 
			ai_SizeX, ai_SizeY);
	} ;

exit:

	m_poInsp->m_poTrace->TestEffect(SwOk,ai_ErrorEffect);
	return SwOk;
}


m4bool_t ClENG_DvcPSC::PreReset()
{
	m4bool_t SwOk=M4_TRUE;
	m4int16_t i, Page;
	size_t Len;
	M4ClBuffer::StCopy Param;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;
	m4char_t pcAux[40];

	ClENG_PhysicalFile *F ;
	m4bool_t swRedirect ;
	ClFONT_M_Font *font ;

	ClMIT_Str CompositeName, Type1Name, SoftSymbolName, StrAux, StrAux2 ;

	//------->Abre y prepara ficheros siempre (no hay ATOMIC mode)

	SwOk = M4_BOOL( SwOk && m_oOutRepository.NewBreak() );	
	if (!SwOk) goto exit ;
	
	//------->Bucle principal para localizar ficheros fisicos

	for(Page=0; Page<GetNumLayoutPages(); ++Page){

		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;
		if ( swRedirect ) continue ;


		//------>Datos que solo van una vez al principio del fichero (o ficheros)
	
		//Datos de cabecera universal del fichero postscript. Estos datos solo van una vez al
		// principio de CADA fichero

		F->WStr( PSC_FILE_HEADER  ) ;
		F->WStr( PSC_NEW_LINE "%======================BeginFile" PSC_NEW_LINE  ) ;
		F->WStr( PSC_NEW_LINE "%------------------->Begin Common to all pages Zone" PSC_NEW_LINE ) ;
		F->WStr( PSC_NEW_LINE "%------------------->Begin Prolog" PSC_NEW_LINE PSC_BEGIN_PROLOG PSC_NEW_LINE ) ;
		
		F->WStr( 
			PSC_SAVE_STACK 
			PSC_NEW_LINE "%----------------->Begin Dict" PSC_NEW_LINE PSC_BEGIN_DICT PSC_NEW_LINE);
	
		//Extiende caracteres ISO para todas las fuentes que aparecen en el Report. 
		for(i=0;i<m_poDBFont->GetNumFonts();i++){
			sprintf(pcAux, m_poDBFont->GetAddressFontByIndex(i)->PSC_GetTypeFace());
			sprintf( pcBuff, PSC_EXTENDED_ISO, pcAux, pcAux);
			F->WStr( pcBuff) ;
		};
	
		F->WStr( 
			PSC_NEW_LINE "%<-----------------End Dict" PSC_NEW_LINE PSC_END_DICT PSC_NEW_LINE  ) ;

		F->WStr( PSC_NEW_LINE "%<------------------End Prolog" PSC_NEW_LINE PSC_END_PROLOG PSC_NEW_LINE );
	
		F->WStr( PSC_NEW_LINE "%------------------->Begin setup" PSC_NEW_LINE ) ;
	
		//SetUp 
		F->WStr( PSC_BEGIN_SETUP PSC_BEGIN_FEATURE);
	
		//Resolucion
		F->WStr( "<</HWResolution[7200 7200]>>setpagedevice\r\n");
	
		//Tamaño del papel
		switch(m_poPagePrinter[0].GetPageSize()){
			case ClENG_PagePrinter::A4:
				F->WStr( PSC_A4_PAPER_SIZE);
				break;
			case ClENG_PagePrinter::A3:
				F->WStr( PSC_A3_PAPER_SIZE);
				break;
			case ClENG_PagePrinter::LEGAL:
				F->WStr( PSC_LEGAL_PAPER_SIZE);
				break;
			case ClENG_PagePrinter::LETTER:
				F->WStr( PSC_LETTER_PAPER_SIZE);
				break;
			case ClENG_PagePrinter::EXECUTIVE:
				F->WStr( PSC_EXECUTIVE_PAPER_SIZE);
				break;
			case ClENG_PagePrinter::PERSONAL:
				sprintf( pcBuff, PSC_PERSONAL_PAPER_SIZE, 
					(long int)ClMetric::DvcToPoint(m_poPagePrinter[Page].GetSizeX()),
					(long int)ClMetric::DvcToPoint(m_poPagePrinter[Page].GetSizeY())
				);
				F->WStr( pcBuff ) ;
				break;
		};

		//Duplex
		if( m_Duplex==StENG_DvcParams::REAL_DUPLEX && m_Tumble==StENG_DvcParams::TUMBLE_TRUE) {
			F->WStr(  PSC_DUPLEX PSC_TUMBLE_TRUE);
		} else if ( m_Duplex==StENG_DvcParams::REAL_DUPLEX && m_Tumble==StENG_DvcParams::NO_TUMBLE) {
			F->WStr( PSC_DUPLEX);
		} else if ( m_Duplex==StENG_DvcParams::REAL_DUPLEX && m_Tumble==StENG_DvcParams::TUMBLE_FALSE) {
			F->WStr( PSC_DUPLEX PSC_TUMBLE_FALSE);
		} else {
			F->WStr( PSC_SIMPLEX);
		};

		//Numero de copias
		sprintf( pcBuff, PSC_NUM_COPIES, (int)m_iNumCopies);
		strcat(pcBuff,PSC_END_FEATURE PSC_END_SETUP PSC_NEW_LINE);
		F->WStr( pcBuff ) ;

		ClFONT_M_DBFont::StSoftFontData* StSoftFont;

		F->WStr( PSC_NEW_LINE "%<-------------------End setup" PSC_NEW_LINE ) ;
		F->WStr( PSC_NEW_LINE "%------------------->Begin SoftFont Dump" PSC_NEW_LINE ) ;

		//VOLCAMOS TODAS LAS SOFTFONTS
		for(i=0; i<m_poDBFont->GetNumSoftFonts(); i++) {
			StSoftFont=m_poDBFont->GetSoftFontDataByIndex(i);
			F->WFile(StSoftFont->m_oPathName, StSoftFont->m_Offset, StSoftFont->m_Size) ;
		};

	
		F->WStr( PSC_NEW_LINE "%<------------------End SoftFont Dump" PSC_NEW_LINE  ) ;

		F->WStr( PSC_NEW_LINE "%------------------->Begin Composite Mode SoftFont. MUST BE AFTER SoftFont dump!" PSC_NEW_LINE ) ;

		for(i=0;i<m_poDBFont->GetNumFonts();i++) {
			
			font = m_poDBFont->GetAddressFontByIndex(i) ;
			if ( ! font->IsSoftFont() ) continue ;
			if ( font->GetTranslationType()!=ENG_SOFT_TRANS_TYPE_PSC_MIXED) continue ;
			StSoftFont=m_poDBFont->GetSoftFontDataByIndex( font->GeSoftFontIndex() );

			Type1Name.StrCpy(font->PSC_GetTypeFace()) ;
			CompositeName.StrCpy("m4") ; CompositeName.StrCat(Type1Name) ;
			ClMIT_File::SplitPath( 
				StSoftFont->m_oSofFontFile, &StrAux, &SoftSymbolName ,&StrAux2 ) ;
			sprintf( pcBuff, PSC_COMPOSITE_FONT, 
				CompositeName.InChar(0),
				(unsigned int)((unsigned char)font->GetTranslation(ENG_TRANS_PSC_MIXED_SHIFT_IN)),
				(unsigned int)((unsigned char)font->GetTranslation(ENG_TRANS_PSC_MIXED_SHIFT_OUT)),
				Type1Name.InChar(0), SoftSymbolName.InChar(0) );
			F->WStr( pcBuff) ;
		};

		F->WStr( PSC_NEW_LINE "%<------------------End Composite Mode SoftFont" PSC_NEW_LINE  ) ;

		F->WStr( "%<------------------End Common to all pages Zone" PSC_NEW_LINE  ) ;

	} ;

	//*******************A prtir de aqui se escribe a buffer, para que este se copie por
	//	cada pagina. El unico truco que hay que hacer es guardar las posiciones de
	//	los parametros %%page, para actualizarlos cada vez que se vuelquen

	M4ClBuffer *buff ;

	//Datos de cabecera para cada pagina
	for(Page=0; Page<GetNumLayoutPages();++Page){

		buff = & m_poLayoutPages[Page].m_oLayoutBuff ;
		buff->AppendStr( PSC_NEW_LINE "%------------------>Begin PAGE" PSC_NEW_LINE ) ;
		
		buff->AppendStr(PSC_PAGE_ID_B) ;
		m_poPageBuffOffset[Page] = buff->GetTotal() ;
		buff->AppendStr(PSC_PAGE_ID_E) ;

		buff->AppendStr( PSC_PAGE_DICT PSC_NEW_LINE);

		//Begin del setpagedevice
		buff->AppendStr( PSC_BEGIN_SETUP PSC_BEGIN_FEATURE);
		
		//Orientacion de la pagina
		if(m_poPagePrinter[Page].GetOrientation()==ClENG_PagePrinter::PORTRAIT){
			buff->AppendStr(PSC_PORTRAIT);
		} else {
			buff->AppendStr(PSC_LANDSCAPE);
		};
		
		//Alimentacion manual o automática
		if(m_poPagePrinter[Page].GetPaperSource()==ClENG_PagePrinter::MANUAL){
			buff->AppendStr(PSC_MANUAL_SOURCE);
		};

		//Final de setpagedevice
		buff->AppendStr(PSC_END_FEATURE PSC_END_SETUP PSC_NEW_LINE);
		
		//Aplicar LandScape si procede
		if(m_poPagePrinter[Page].GetOrientation()==ClENG_PagePrinter::LANDSCAPE){
			buff->AppendStr(PSC_LANDSCAPE_SETUP PSC_LANDSCAPE_PROC PSC_NEW_LINE PSC_LANDSCAPE_APPLY PSC_NEW_LINE);
		};

		//Añade palete FG, valida y unica en todo el fichero

		buff->AppendStr(PSC_NEW_LINE "%------------------->Begin Palette" PSC_NEW_LINE ) ;
		Len = SetActivePalette(&m_oFGPalette, pcBuff, 0, StENG_TaskEnv::DvcPSC_MAX_SIZE);
		buff->AppendStr(pcBuff) ;
		buff->AppendStr(PSC_NEW_LINE "%<-------------------End Palette" PSC_NEW_LINE ) ;
		
		buff->AppendStr( PSC_NEW_LINE "%<------------------Preloaded buffers" PSC_NEW_LINE ) ;

		//Volcar informacion de Optimized Preload
		buff->CatM4ClBuffer(&m_poLayoutPages[Page].m_oOpPrBuff);
		buff->CatM4ClBuffer(&m_poLayoutPages[Page].m_oPrBuff);
		
		buff->AppendStr( PSC_NEW_LINE "%<------------------Preloaded buffers" PSC_NEW_LINE ) ;

		buff->AppendStr( PSC_NEW_LINE "%<------------------End PAGE" PSC_NEW_LINE ) ;
	};

	//---------->Propaga

	ClENG_OutDevice::PreReset() ;

exit:
	return SwOk ;
}


void ClENG_DvcPSC::FixPreload()
{
	ClENG_OutDevice::FixPreload();
}

void ClENG_DvcPSC::PostReset()
{
	m4int16_t Page;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t *pcBuff = TaskEnv->Dvc_g_pcBuff;

	ClENG_PhysicalFile *F ;
	m4bool_t swRedirect ;

	for(Page=0; Page<GetNumLayoutPages(); ++Page){

		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;
		
		if(! swRedirect ){
			F->WStr( PSC_NEW_LINE "%------------------>END file block" PSC_NEW_LINE ) ;
			F->WStr(PSC_NEW_LINE PSC_TRAILER);

			//Meter numero de paginas reales por fichero
			sprintf( pcBuff, PSC_PAGES, (long int) 
				m_oOutRepository.GetPrintedPages(Page) ) ;
			ClMIT_Str::OverStringValid(pcBuff, StENG_TaskEnv::DvcPCL_MAX_SIZE);
			F->WStr(pcBuff);
			
			F->WStr(/*PSC_RESET_STACK*/ PSC_FREE_PAGES);
			F->WStr( PSC_NEW_LINE "%<------------------END file block" PSC_NEW_LINE ) ;
		}
		m_poLayoutPages[Page].m_oLayoutBuff.Init();

	};

	//---------->Cierra ficheros (no hay ATOMIC mode)

	m_oOutRepository.CloseBreak( IsValidOutput() ) ;

	//---------->Propaga

	ClENG_OutDevice::PostReset(M4_FALSE);
}


m4bool_t  ClENG_DvcPSC::RefreshLayout()
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t Page ;

	m4char_t strBuff[30] ; //De sobra, se van a usar 12+/0x00 final
	M4ClBuffer *buff ;
	M4ClBuffer::StCopy Param ;

	//---------->Notifica a FileRep (no abre files, no ATOMIC_MODE)

	SwOk=m_oOutRepository.NewFlush() ;


	//---Actualiza los valores de Page que estan permanentemente en el buffer

	for(Page=0; Page<GetNumLayoutPages();++Page){

		buff = & m_poLayoutPages[Page].m_oLayoutBuff ;

		//Cabecera de pagina
		//En %%PAGE, la primera entrada es la logica. La tratamos asi:
		//	-Divisiones por page layout, no se tienen en cuenta, son para impresion.
		//	-Division por break. Si se cuentan, cada break resetea el contador.
		//La segunda es fisica e interna, y el formato PS dice que debe ser 
		//	secuencial por fichero y empezar por 1.

		sprintf(strBuff, "%6.6li %6.6li" ,
			(long int)(GetBreakPagesDvcPrinted()+Page+1), 
			(long int)(m_oOutRepository.GetPrintedPages(Page)+
					m_oOutRepository.GetOrdinalInFile(Page) +1 ) 
		);
		Param.Data = strBuff ;
		Param.Count = 13 ;
		buff->Flush(M4ClBuffer::MemToBuffManip,&Param, m_poPageBuffOffset[Page], 13) ;
	} ;
	
	//---------->Propaga

	return SwOk && ClENG_OutDevice::RefreshLayout() ;
};

m4bool_t ClENG_DvcPSC::FlushLayout()
{
	m4bool_t SwOk=M4_TRUE ;

	//---------->Corre EOPs

	RunAllEOP() ;

	//---------->Vuelca buffers a files  (no cierra files, no ATOMIC_MODE)

	SwOk=m_oOutRepository.CloseFlush( ClENG_DvcFile::m_poBuffArray ) ;

	//---------->Propaga

	return SwOk && ClENG_OutDevice::FlushLayout();
}


void ClENG_DvcPSC::SelectFontDevice(m4int16_t ai_iPage, ClFONT_M_Font* ai_poFont)
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;

	if ( ai_poFont->IsSoftFont() && ai_poFont->GetTranslationType()==ENG_SOFT_TRANS_TYPE_PSC_MIXED) {
		sprintf(pcBuff, PSC_SET_COMPOSITE_FONT, ai_poFont->PSC_GetTypeFace(), 
				    (int)ai_poFont->PSC_GetHeight() );
	} else {
		sprintf(pcBuff, PSC_SET_FONT, ai_poFont->PSC_GetTypeFace(), 
				    (int)ai_poFont->PSC_GetHeight() );
	} ;
	PageBuffer->AppendStr(pcBuff);
}


void ClENG_DvcPSC::PrintNChar(m4int16_t	ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
						  m4pchar_t ai_pcStr, size_t ai_szNChar,
						  m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID,
						  ClENG_OutDevice::StPIParams *ai_pstPI)
{
	ClMIT_Str oStrAux;
	size_t Length = 0, iLengthOrig = 0;
	m4uint32_t lValueOffset = 0;
	size_t szClipSize = 0;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* m_pcBuff = TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	DvcUnit_t iTemp ;
	ClFONT_M_Font *font ;
	size_t szNChar = 0;
	size_t szNCharOrig = 0;

	font = GetLastFont(ai_iPage) ;

	//Se añade el Offset del limite fisico de la pagina por la izquierda
	ai_iX += GetOffsetX(ai_iPage);
	ai_iX = ClMetric::DvcToPoint(ai_iX) + ClMetric::DvcToPoint(PSC_OFFSET_X);
	
	//Necesario, pues las metricas en PSC empiezan debajo
	//Se añade el Offset del limite fisico de la pagina por arriba
	iTemp = ClMetric::DvcToPoint(GetSizeY(ai_iPage)) - ClMetric::DvcToPoint(GetOffsetY(ai_iPage) + ai_iY);
	ai_iY = iTemp - ClMetric::DvcToPoint(PSC_OFFSET_Y);
	ai_iY -= ClMetric::DvcToPoint(font->GetDvcHeight());

	// Bug 0125206. En los campos multilínea te llega el contenido del campo completo
	// pero en el número de caracteres te llega los que realmente quiere imprimir.
	m4pchar_t pcString = NULL;
	m4pchar_t pcInputString = new m4char_t[ai_szNChar + 1];
	memcpy(pcInputString, ai_pcStr, ai_szNChar);
	pcInputString[ai_szNChar] = '\0';

	// Bug 99018.
	// Salvar el número de caracteres a imprimir que originalmente nos mandan.
	// Esto es necesatio para poder subrayar este número de caracteres en concreto.
	// Como más adelante, se modifica este valor en el PrepareForPostScript, el problema
	// es que el subrayado era más grande que el número de caracteres (se subrayaba de más).
	szNChar = strlen(pcInputString);
	szNCharOrig = szNChar;
	
	// UNICODE REPORTS.
	// Convertir una cadena (multi byte) a multibyte ansi. Si estamos en ansi, te devuelve
	// ansi, es decir, esa misma cadena. Si estamos en unicode, pasa de utf8 a ansi.
	// wide char.
	pcString = M4CppToANSI(pcInputString, (int&)szNChar);
	
	// PrepareForPostScript(font,&oStrAux, &ai_szNChar,ai_bRawText);
	oStrAux.StrCpy(pcString);
	PrepareForPostScript(font,&oStrAux, &szNChar,ai_bRawText);

	//Imprime Cadena
	Length=sprintf(m_pcBuff, PSC_NO_PRINT_STRING, (int)ai_iX, (int)ai_iY, 
			(size_t)szNChar, oStrAux.InChar(0));

	//No utilizamos strlen() por que la cadena puede tener codigos especiales, que incluyan el 0
	if((Length+1)>StENG_TaskEnv::DvcPSC_MAX_SIZE){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCPSC_MSG_20 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError();
	};

	//Calculamos el Offset, posicion del buffer antes de que escriba
	lValueOffset = PageBuffer->GetTotal() + Length - PSC_PI_OFFSET - szNChar;

	PageBuffer->Append(m_pcBuff,Length);

	if(ai_pstPI){
		ai_pstPI->m_uiOff=lValueOffset;
		ai_pstPI->m_szNChars=szNChar;
	};

	if(font->IsUnderline())
	{
		sprintf(m_pcBuff, PSC_MOVE_TO, (int)ai_iX, (int)ai_iY - 1);
		PageBuffer->AppendStr(m_pcBuff);
		// Bug 99018.
		iLengthOrig = ClMetric::DvcToPoint(font->GetDvcLenStr(pcString, szNCharOrig));
		sprintf(m_pcBuff, PSC_DRAW_LINE, (int)ai_iX + (int)iLengthOrig, (int)ai_iY - 1);
		PageBuffer->AppendStr(m_pcBuff);
		sprintf(m_pcBuff, PSC_STROKE);
		PageBuffer->AppendStr(m_pcBuff);
	};

	delete [] pcString;
	delete [] pcInputString;
}

void ClENG_DvcPSC::PrintNCharPi(
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


void ClENG_DvcPSC::PrepareForPostScript(
	ClFONT_M_Font *ai_poFont, ClMIT_Str *ao_poStr, size_t *ai_szNChar, m4bool_t ai_bRawText)
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

	//Trata las font composite, para cambiar simbolos
	//Para cada char a cambiar, trata 3 caracteres, 2 nuevos y uno se cambia;
	//	ShiftIn-Caracter-ShiftOut
	//Ojo, un StrInsert puede realocar la cadena, usar siempre indices

	if ( ai_poFont->IsSoftFont() && ai_poFont->GetTranslationType()==ENG_SOFT_TRANS_TYPE_PSC_MIXED) {
		
		//Por ahora solo busca el euro (128)
		for ( begIdx = 0; M4_TRUE ;  )
		{
			currentIdx = begIdx + strcspn(ao_poStr->InChar(begIdx), "\200") ;
			if( currentIdx >= *ai_szNChar ) break;

			*(ao_poStr->InChar(currentIdx))= ai_poFont->GetTranslation(ENG_TRANS_PSC_MIXED_EURO)  ;
			ao_poStr->StrInsertChar( currentIdx+1, 1, 
				ai_poFont->GetTranslation(ENG_TRANS_PSC_MIXED_SHIFT_IN) );
			ao_poStr->StrInsertChar( currentIdx, 1, 
				ai_poFont->GetTranslation(ENG_TRANS_PSC_MIXED_SHIFT_OUT) );

			begIdx=currentIdx+3; //+3 porque acabo de insertar 2 char !
			*ai_szNChar+=2;
		};
	} ;

	//Convierte caracteres extendidos a ISO
	if (!ai_bRawText) {
		ClMIT_SysInfo::ConvertToIsoWithEscape(ao_poStr, ai_szNChar);
	} else {
		ClMIT_SysInfo::ConvertToEscape(ao_poStr, ai_szNChar);
	} ;

}


void ClENG_DvcPSC::ResolvePi(ClMIT_Str  *ai_poIdPi,m4pchar_t ai_pcStr) 
{
	StPi_t piData ;

	m4uint32_t Len;
	m4uint32_t PosActual;
	ClMIT_Str StrAux ;

	m4pchar_t pcString;
	
	//La forma de resolver las PI en PostScript es similar a la de PCL, solo que
	// los Offset de las cadenas imprimidas a RAM o fichero son diferentes.
  
	// Tomo el Pi a imprimir ( le digo que imprima el error si no lo encuentra)
	if ( m_oOutRepository.ExtractPi( ai_poIdPi->InChar(0),  &piData ) )
	{
		// UNICODE REPORTS. Bug 163475
		// Convertir una cadena (multi byte) a multibyte ansi. Si estamos en ansi, te devuelve
		// ansi, es decir, esa misma cadena. Si estamos en unicode, pasa de utf8 a ansi.
		// wide char.
		Len = -1;
		pcString = M4CppToANSI(ai_pcStr, (int&)Len);
	    if (Len > piData.m_NBytes) {

		   // Sacar mensaje de que se va a truncar
		   *m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCPSC_MSG_40 ] ) 
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
			M4ClBuffer::StCopy Param = { StrAux.InChar(0), piData.m_NBytes };	           
			m_poLayoutPages[ piData.m_NPage ].m_poActualBuff->Flush(
				M4ClBuffer::MemToBuffManip, &Param, piData.m_Offset, piData.m_NBytes );

		} else  { 
			
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


void ClENG_DvcPSC::EOP(m4int16_t ai_iPage){
	m_poLayoutPages[ai_iPage].m_poActualBuff->AppendStr(PSC_NEW_LINE);
	m_poLayoutPages[ai_iPage].m_poActualBuff->AppendStr(PSC_NEW_PAGE);
	m_poLayoutPages[ai_iPage].m_poActualBuff->AppendStr(PSC_END_PAGE);
	m_poLayoutPages[ai_iPage].m_poActualBuff->AppendStr(PSC_NEW_LINE);
	ClENG_OutDevice::EOP(ai_iPage);
}


//--------------------- Graficos ---------------------------------------

struct ClENG_DvcPSC::stGraphicResolution const ClENG_DvcPSC::GraphicResolution[ClENG_DvcPSC::N_RESOLUTIONS] = {
	{75 ,4,	8 },
	{100,3,	6 },
	{150,2,	4 },
	{200,0,	3 },
	{300,1,	2 },
	{600,0,	1 },
} ;

void ClENG_DvcPSC::GetScaleData( 
	DvcUnit_t ai_SizeXDvc,			DvcUnit_t ai_SizeYDvc,			//Area a imprimir.
	m4uint32_t ai_iRasterXMap,	m4uint32_t ai_iRasterYMap,					//Size bitmap
	m4uint32_t *ao_piRasterXDvc, m4uint32_t *ao_piRasterYDvc,
	m4int16_t *ao_piGResIndex ) 
{
	m4int16_t Factor, B, FactorPSC;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* m_pcBuff=TaskEnv->Dvc_g_pcBuff;

	//Hallamos los raster de dispositivo
	*ao_piRasterXDvc= (m4uint32_t)(ClMetric::DvcToInch( ai_SizeXDvc ) * 
		( m_MaxResolution == P_300_DPI ?  P_300_DPI : P_600_DPI )) ;
	*ao_piRasterYDvc= (m4uint32_t)(ClMetric::DvcToInch( ai_SizeXDvc ) * 
		( m_MaxResolution == P_300_DPI ?  P_300_DPI : P_600_DPI )) ;

	// Como la impresora, via resolucion de graficos, puede expandir un punto, reducimos las
	//	raster-lines de dispositivo, siempre que no perdamos resolucion en el proceso.

	Factor=M4_MIN( *ao_piRasterXDvc/ai_iRasterXMap, *ao_piRasterYDvc/ai_iRasterYMap  ) ;
	Factor=M4_MAX(Factor,1) ;
	for (B=0; B<N_RESOLUTIONS; ++B ) {
		FactorPSC = ( m_MaxResolution == P_300_DPI ? GraphicResolution[B].ScaleFactor300 :
			GraphicResolution[B].ScaleFactor600 ) ;
			if ( FactorPSC != 0 && FactorPSC<=Factor ) break ;
	} ;
	*ao_piRasterXDvc /= FactorPSC ;		
	*ao_piRasterYDvc /= FactorPSC ;

	if(ao_piGResIndex) *ao_piGResIndex=B;
}

void ClENG_DvcPSC::PrintRasterMap(
	m4int16_t ai_iPage,
	DvcUnit_t ai_X,				DvcUnit_t ai_Y,				//Coordenadas de comienzo.
	DvcUnit_t ai_SizeX,			DvcUnit_t ai_SizeY,			//Area a imprimir.
	ClENG_RasterMap * ai_poMap, m4char_t *ai_pcHyperlink,
	m4uint16_t ai_iTargetID ) 
{
	m4uint32_t  RasterXDvc, RasterYDvc, SkipBytesRow, WriteBytesRow, YCount;
	m4int16_t   GResIndex;
	m4uint8_t*  Map;
	size_t	    LenStrAux;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t*   m_pcBuff = TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer* PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;

	ai_X += GetOffsetX(ai_iPage);
	ai_Y += GetOffsetY(ai_iPage);

	//GetScaleData
	GetScaleData(ai_SizeX, ai_SizeY, ai_poMap->GetRasterX(), ai_poMap->GetRasterY(),
		         &RasterXDvc, &RasterYDvc, &GResIndex);

	//256 COLORES
	if(ai_poMap->GetBitsPixel()==ClENG_RasterMap::INDEXED_256){
		
		//Carga la paleta del grafico
		SetActivePalette(ai_poMap->GetPalette(), m_pcBuff, 0, StENG_TaskEnv::DvcPSC_MAX_SIZE);
		PageBuffer->AppendStr(m_pcBuff);

		PageBuffer->AppendStr(PSC_NEW_LINE PSC_IMAGE_TRANSFER);

		//Posiciona Imagen
		sprintf(m_pcBuff, PSC_LOCATE_POS, (int)ClMetric::DvcToPoint(ai_X) + (int)ClMetric::DvcToPoint(PSC_OFFSET_X),
			(int)(ClMetric::DvcToPoint(GetSizeY(ai_iPage)) - ClMetric::DvcToPoint(ai_Y) - ClMetric::DvcToPoint(PSC_OFFSET_GRAPH) - ClMetric::DvcToPoint(ai_SizeY)));
		PageBuffer->AppendStr(m_pcBuff);

		//Escala la imagen
		sprintf(m_pcBuff, PSC_SCALE, (int)ClMetric::DvcToPoint(ai_SizeX), (int)ClMetric::DvcToPoint(ai_SizeY));
		PageBuffer->AppendStr(m_pcBuff);

		//Parametros del grafico
		PageBuffer->AppendStr(PSC_IMAGE_TYPE);
		sprintf(m_pcBuff, PSC_IMAGE_SIZE, ai_poMap->GetRasterX() ,ai_poMap->GetRasterY());
		PageBuffer->AppendStr(m_pcBuff);
		PageBuffer->AppendStr(PSC_256_COLOURS_BPC);
		PageBuffer->AppendStr(PSC_256_COLOUR_DECODE);
		sprintf(m_pcBuff, PSC_IMAGE_MATRIX, ai_poMap->GetRasterX() ,ai_poMap->GetRasterY(), ai_poMap->GetRasterY());
		PageBuffer->AppendStr(m_pcBuff);
		PageBuffer->AppendStr(PSC_DATA_SOURCE_FILTER);
		PageBuffer->AppendStr(PSC_COLOUR_READ_DATA);
		//Fin de parametros

		WriteBytesRow = ai_poMap->GetLastXIndex()+1;

		for(SkipBytesRow=ai_poMap->GetBytesRow(), Map=ai_poMap->GetMap(), YCount=ai_poMap->GetRasterY(); YCount; Map+=SkipBytesRow, --YCount){
			//PageBuffer->Append(Map, WriteBytesRow);
			for(int i=0; i<WriteBytesRow; i++){
				sprintf(m_pcBuff, "%.2X", (m4uint8_t)Map[i]);
			    PageBuffer->Append(m_pcBuff,2);
			};
		};
	
		//Fin de grafico
		PageBuffer->AppendStr(PSC_IMAGE_END PSC_NEW_LINE);
		SetActivePalette(&m_oFGPalette, m_pcBuff,  0, StENG_TaskEnv::DvcPSC_MAX_SIZE);
		PageBuffer->AppendStr(m_pcBuff);
		PageBuffer->AppendStr(PSC_NEW_LINE);

	} else if(ai_poMap->GetBitsPixel()==ClENG_RasterMap::MONOCHROME) {
		
		//Carga la paleta monocromo
		sprintf(m_pcBuff, PSC_PALETTE_OPEN, 1);
		PageBuffer->AppendStr(m_pcBuff);
		sprintf(m_pcBuff, PSC_SET_PALETTE_ENTRY, 255, 255, 255);
		PageBuffer->AppendStr(m_pcBuff);
		sprintf(m_pcBuff, PSC_SET_PALETTE_ENTRY, 0, 0, 0);
		PageBuffer->AppendStr(m_pcBuff);
		PageBuffer->AppendStr(PSC_PALETTE_CLOSE);
		
		//Comienza transferencia de imagen
		PageBuffer->AppendStr(PSC_NEW_LINE PSC_IMAGE_TRANSFER);

		//Posiciona Imagen
		sprintf(m_pcBuff, PSC_LOCATE_POS, (int)ClMetric::DvcToPoint(ai_X) + (int)ClMetric::DvcToPoint(PSC_OFFSET_X),
			(int)(ClMetric::DvcToPoint(GetSizeY(ai_iPage)) - ClMetric::DvcToPoint(ai_Y) - ClMetric::DvcToPoint(PSC_OFFSET_GRAPH) - ClMetric::DvcToPoint(ai_SizeY)));
		PageBuffer->AppendStr(m_pcBuff);

		//Escala la imagen
		sprintf(m_pcBuff, PSC_SCALE, (int)ClMetric::DvcToPoint(ai_SizeX), (int)ClMetric::DvcToPoint(ai_SizeY));
		PageBuffer->AppendStr(m_pcBuff);

		//Parametros del grafico
		PageBuffer->AppendStr(PSC_IMAGE_TYPE);
		sprintf(m_pcBuff, PSC_IMAGE_SIZE, ai_poMap->GetRasterX() ,ai_poMap->GetRasterY());
		PageBuffer->AppendStr(m_pcBuff);
		PageBuffer->AppendStr(PSC_MONOCHROME_BPC);
		PageBuffer->AppendStr(PSC_MONOCHROME_DECODE);
		sprintf(m_pcBuff, PSC_IMAGE_MATRIX, ai_poMap->GetRasterX() ,ai_poMap->GetRasterY(), ai_poMap->GetRasterY());
		PageBuffer->AppendStr(m_pcBuff);
		PageBuffer->AppendStr(PSC_DATA_SOURCE_FILTER);
		PageBuffer->AppendStr(PSC_COLOUR_READ_DATA);
		//Fin de parametros

		WriteBytesRow = ai_poMap->GetLastXIndex()+1;

		for(SkipBytesRow=ai_poMap->GetBytesRow(), Map=ai_poMap->GetMap(), YCount=ai_poMap->GetRasterY(); YCount; Map+=SkipBytesRow, --YCount){
			//PageBuffer->Append(Map, WriteBytesRow);
			for(int i=0; i<WriteBytesRow; i++){
				sprintf(m_pcBuff, "%.2X", (m4uint8_t)Map[i]);
			    PageBuffer->Append(m_pcBuff,2);
			};
		};

		//Fin de grafico
		PageBuffer->AppendStr(PSC_IMAGE_END PSC_NEW_LINE);
		SetActivePalette(&m_oFGPalette, m_pcBuff,  0, StENG_TaskEnv::DvcPSC_MAX_SIZE);
		PageBuffer->AppendStr(m_pcBuff);
		PageBuffer->AppendStr(PSC_NEW_LINE);

	};

}



void ClENG_DvcPSC::SelectColourDevice(m4int16_t ai_iPage, m4uint32_t ai_uiIdColour)
{
	M4ClBuffer *PageBuffer=m_poLayoutPages[ai_iPage].m_poActualBuff;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff = TaskEnv->Dvc_g_pcBuff;

	sprintf(pcBuff, PSC_SET_FG_COLOUR, (long)m_oFGPalette.GetIndex(ai_uiIdColour));
	ClMIT_Str::OverStringValid(pcBuff, StENG_TaskEnv::DvcPSC_MAX_SIZE);
	PageBuffer->AppendStr(pcBuff);
}


size_t ClENG_DvcPSC::SetActivePalette (ClENG_Palette* ai_poPalette, m4char_t* ai_pcBuff, size_t ai_szFrom, size_t ai_szMaxSize)
{
	m4int16_t PIndex;
	ClENG_Colour* pColour;
	size_t Written;

	Written=ai_szFrom;

	Written += sprintf(&ai_pcBuff[Written], PSC_PALETTE_OPEN, ai_poPalette->GetSize()-1);

	for(PIndex=0; PIndex<ai_poPalette->GetSize(); ++PIndex){

		pColour = ai_poPalette->GetColourByIndex(PIndex);
		Written += sprintf(&ai_pcBuff[Written], PSC_SET_PALETTE_ENTRY, (long)pColour->GetR(),
			               (long)pColour->GetG(), (long)pColour->GetB());
	};

	Written += sprintf(&ai_pcBuff[Written], PSC_PALETTE_CLOSE PSC_NEW_LINE);
	ClMIT_Str::OverStringValid(ai_pcBuff, ai_szMaxSize, Written+1);

	return Written;
}


void ClENG_DvcPSC::PrintLine( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X1, DvcUnit_t ai_Y1, DvcUnit_t ai_X2, DvcUnit_t ai_Y2,
	DvcUnit_t ai_LineWidth, STYLE ai_Style ) 
{
	DvcUnit_t iTemp ;
	m4int16_t i;
	m4double_t dLineWidth = 0;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* m_pcBuff=TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;

	//Se añade el Offset del limite fisico de la pagina por la izquierda
	ai_X1 += GetOffsetX(ai_iPage);
	ai_X1 = ClMetric::DvcToPoint(ai_X1) + ClMetric::DvcToPoint(PSC_OFFSET_X);
	ai_X2 += GetOffsetX(ai_iPage);
	ai_X2 = ClMetric::DvcToPoint(ai_X2) + ClMetric::DvcToPoint(PSC_OFFSET_X);
	
	//Necesario, pues las metricas en PSC empiezan debajo
	//Se añade el Offset del limite fisico de la pagina por arriba
	iTemp = ClMetric::DvcToPoint(GetSizeY(ai_iPage)) - ClMetric::DvcToPoint(GetOffsetY(ai_iPage) + ai_Y1);
	ai_Y1 = iTemp - ClMetric::DvcToPoint(PSC_OFFSET_Y);
	iTemp = ClMetric::DvcToPoint(GetSizeY(ai_iPage)) - ClMetric::DvcToPoint(GetOffsetY(ai_iPage) + ai_Y2);
	ai_Y2 = iTemp - ClMetric::DvcToPoint(PSC_OFFSET_Y);

	//Estilo
	if (ai_Style==DASH) {
		sprintf(m_pcBuff, PSC_DASH);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DOT) {
		sprintf(m_pcBuff, PSC_DOT);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DASH_DOT) {
		sprintf(m_pcBuff, PSC_DASH_DOT);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DASH_DOT_DOT) {
		sprintf(m_pcBuff, PSC_DASH_DOT_DOT);
		PageBuffer->AppendStr(m_pcBuff);
	};
	
	//Dibuja Linea
	dLineWidth = ClMetric::DvcToPoint(ai_LineWidth);
	sprintf(m_pcBuff, PSC_LINE_WIDTH, dLineWidth);
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PSC_MOVE_TO, (int)ai_X1, (int)ai_Y1-1);
	PageBuffer->AppendStr(m_pcBuff);

	sprintf(m_pcBuff, PSC_DRAW_LINE, (int)ai_X2, (int)ai_Y2-1);
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PSC_STROKE);
	PageBuffer->AppendStr(m_pcBuff);
}

void ClENG_DvcPSC::FillZone( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height ) 
{
	DvcUnit_t iTemp ;
	m4int16_t i;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* m_pcBuff=TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	m4uint32_t uiIdColour;

	//Selecciona el color
	uiIdColour = GetLastColour(ai_iPage) ;

	//Se añade el Offset del limite fisico de la pagina por la izquierda
	ai_X += GetOffsetX(ai_iPage);
	ai_X = ClMetric::DvcToPoint(ai_X) + ClMetric::DvcToPoint(PSC_OFFSET_X);
	
	//Necesario, pues las metricas en PSC empiezan debajo
	//Se añade el Offset del limite fisico de la pagina por arriba
	iTemp = ClMetric::DvcToPoint(GetSizeY(ai_iPage)) - ClMetric::DvcToPoint(GetOffsetY(ai_iPage) + ai_Y);
	ai_Y = iTemp - ClMetric::DvcToPoint(PSC_OFFSET_Y);

	//Dibuja caja
	sprintf(m_pcBuff, PSC_FILL_COLOUR, (long)m_oFGPalette.GetIndex(uiIdColour));
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PSC_MOVE_TO, (int)ai_X, (int)ai_Y - 1);
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PSC_DRAW_RLINE, 0, -(int)ClMetric::DvcToPoint(ai_Height));
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PSC_DRAW_RLINE, (int)ClMetric::DvcToPoint(ai_Width),0);
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PSC_DRAW_RLINE, 0, (int)ClMetric::DvcToPoint(ai_Height));
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PSC_DRAW_RLINE, -(int)ClMetric::DvcToPoint(ai_Width),0);
	PageBuffer->AppendStr(m_pcBuff);
	PageBuffer->AppendStr(PSC_CLOSE_PATH);
	PageBuffer->AppendStr(PSC_FILL);
	PageBuffer->AppendStr(PSC_STROKE);
	PageBuffer->AppendStr(PSC_NEW_LINE);
}	

void ClENG_DvcPSC::PrintFrame( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
	DvcUnit_t ai_LineWidth, STYLE ai_Style ) 
{
	DvcUnit_t iTemp ;
	m4int16_t i;
	m4double_t dLineWidth = 0;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* m_pcBuff=TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	
	//Se añade el Offset del limite fisico de la pagina por la izquierda
	ai_X += GetOffsetX(ai_iPage);
	ai_X = ClMetric::DvcToPoint(ai_X) + ClMetric::DvcToPoint(PSC_OFFSET_X);
	
	//Necesario, pues las metricas en PSC empiezan debajo
	//Se añade el Offset del limite fisico de la pagina por arriba
	iTemp = ClMetric::DvcToPoint(GetSizeY(ai_iPage)) - ClMetric::DvcToPoint(GetOffsetY(ai_iPage) + ai_Y);
	ai_Y = iTemp - ClMetric::DvcToPoint(PSC_OFFSET_Y);

	//Estilo
	if (ai_Style==DASH) {
		sprintf(m_pcBuff, PSC_DASH);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DOT) {
		sprintf(m_pcBuff, PSC_DOT);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DASH_DOT) {
		sprintf(m_pcBuff, PSC_DASH_DOT);
		PageBuffer->AppendStr(m_pcBuff);
	} else if (ai_Style==DASH_DOT_DOT) {
		sprintf(m_pcBuff, PSC_DASH_DOT_DOT);
		PageBuffer->AppendStr(m_pcBuff);
	};

	//Dibuja caja
	dLineWidth = ClMetric::DvcToPoint(ai_LineWidth);
	sprintf(m_pcBuff, PSC_LINE_WIDTH, dLineWidth);
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PSC_MOVE_TO, (int)ai_X, (int)ai_Y - 1);
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PSC_DRAW_RLINE, 0, -(int)ClMetric::DvcToPoint(ai_Height));
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PSC_DRAW_RLINE, (int)ClMetric::DvcToPoint(ai_Width),0);
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PSC_DRAW_RLINE, 0, (int)ClMetric::DvcToPoint(ai_Height));
	PageBuffer->AppendStr(m_pcBuff);
	sprintf(m_pcBuff, PSC_DRAW_RLINE, -(int)ClMetric::DvcToPoint(ai_Width),0);
	PageBuffer->AppendStr(m_pcBuff);
	PageBuffer->AppendStr(PSC_CLOSE_PATH);
	PageBuffer->AppendStr(PSC_STROKE);
	PageBuffer->AppendStr(PSC_NEW_LINE);
}
