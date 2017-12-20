
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvcpcl.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/28/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	David Fernández && 07 abr 98
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "m4string.hpp"
#include "eng.h"
#include "engdf.inl"
#include "rasterg.h"

#include "cldvcpcl.hmg"
#include "cldvcpcl.h"

#include "m4unicode.hpp"


#define CR_NL "\r\n"
#define PCL_EOP "\r\f"

#define PCL_Y_OFFSET_TOP (7*(254*72))/48		//Ofset parte arriba no imprimible
// AntonioADN. Bugs 162271 y 163052. Esto ya no es necesario
//#define PCL_Y_OFFSET_BOTTOM (33*(254*72))/48	//Ofset parte abajo  no imprimible

//--------------PCL, reset


#define PCL_PRINTER_RESET "\033E\r\n" 

//NOTA: el "\033&u7200D" es necesario porque por defecto PCL computa en 300 dpis "logicos", lo
//	que produce errores de redondeo que provocan alineaciones a derecha defectuosas. (Lo que 
//	he sudado para descubrir esto solo lo se yo y el Santisimo)
#define PCL_MEASURE "\033&u7200D" 

#define PCL_UNIVERSAL_EXIT "\033%-12345X\r\n" //Ojo, no usar en sprintf, debido al %
#define PCL_NUM_COPIES "\033&l%iX\r\n"
#define PCL_DUPLEX "\033&l1S\r\n"
#define PCL_SIMPLEX "\033&l0S\r\n"
#define PCL_TUMBLE "\033&l2S\r\n"
#define PCL_LEFT_OFFSET "\033&l%7.2fU\r\n"
#define PCL_TOP_OFFSET "\033&l%7.2fZ\r\n"

//--------------PCL, tamaño hoja

#define PCL_A4_PAPER_SIZE "\033&l26A\r\n"
#define PCL_A3_PAPER_SIZE "\033&l27A\r\n"
#define PCL_LEGAL_PAPER_SIZE "\033&l3A\r\n"
#define PCL_LETTER_PAPER_SIZE "\033&l2A\r\n"
#define PCL_EXECUTIVE_PAPER_SIZE "\033&l1A\r\n"

#define PCL_RESET_MARGIN "\033&l0L" "\0339\033&l0E"	//Clear widths, pone top margin a 0

//--------------PCL, bandejas

#define PCL_MANUAL_SOURCE "\033&l2H\r\n"
#define PCL_UPPER_BIN_SOURCE "\033&l5H\r\n"
#define PCL_LOWER_BIN_SOURCE "\033&l4H\r\n"

#define PCL_BIN_SOURCE_PREFIX "\033&l"
#define PCL_BIN_SOURCE_SUFFIX "H\r\n"

//--------------PCL, orientacion

#define PCL_PORTRAIT  "\033&l0O\r\n"
#define PCL_LANDSCAPE "\033&l1O\r\n"

//--------------PCL, fonts

#define PCL_FONT_PITCH "\033(s%4.2fH"
#define PCL_FONT_HEIGHT "\033(s%4.2fV"

#define PCL_FONT_FIXED "\033(s0P"
#define PCL_FONT_UNDERLINE "\033&d0D"
#define PCL_FONT_BOLD "\033(s3B"
#define PCL_FONT_ITALIC "\033(s1S"

#define PCL_FONT_NOT_FIXED "\033(s1P"
#define PCL_FONT_NOT_UNDERLINE "\033&d@"
#define PCL_FONT_NOT_BOLD "\033(s0B"
#define PCL_FONT_NOT_ITALIC "\033(s0S"

#define PCL_FONT_SYMBOL_SET "\033(%s"
#define PCL_FONT_TYPE_FACE  "\033(s%iT"

//--------------PCL, Soft Fonts

#define PCL_SOFT_FONT_BEG_DOWNLOAD "\033*c%uD"	//Pasar Id font
#define PCL_SOFT_FONT_END_DOWNLOAD "\033*c5F"	//Hace SoftFont fija
#define PCL_SOFT_FONT_UNLOAD PCL_SOFT_FONT_BEG_DOWNLOAD "\033*c2F"	//Pasar Id
#define PCL_FONT_ID			"\033(%iX"	//Sleccion para SoftFonts en vez de PCL_FONT_TYPE_FACE

//--------------PCL, impresion strings

#define PCL_POS "\033&a%ih%iV"
#define PCL_NO_ZSTRING PCL_POS "%.*s" CR_NL
#define PCL_ZSTRING    PCL_POS "%s"   CR_NL
#define PCL_NO_ZSTRING_UNDERLINE PCL_POS PCL_FONT_UNDERLINE "%.*s" PCL_FONT_NOT_UNDERLINE CR_NL
#define PCL_ZSTRING_UNDERLINE    PCL_POS PCL_FONT_UNDERLINE "%s"   PCL_FONT_NOT_UNDERLINE CR_NL

// NOTA : Se divide en 2
// PCL_NO_ZSTRING_UNDERLINE = PCL_NO_ZSTRING_UNDERLINE_START + PCL_NO_ZSTRING_UNDERLINE_END
// PCL_NO_ZSTRING =  PCL_NO_ZSTRING_START +  PCL_NO_ZSTRING_END
#define PCL_NO_ZSTRING_UNDERLINE_START PCL_POS PCL_FONT_UNDERLINE 
#define PCL_NO_ZSTRING_UNDERLINE_END  "%.*s" PCL_FONT_NOT_UNDERLINE CR_NL
#define PCL_NO_ZSTRING_START PCL_POS 
#define PCL_NO_ZSTRING_END "%.*s" CR_NL

//--------------PCL, macros

#define PCL_MACRO_ID			"\033&f%iY"
#define PCL_MACRO_START			"\033&f0X"
#define PCL_MACRO_END			"\033&f1X"
#define PCL_MACRO_DELETE		"\033&f8X"

#define PCL_MACRO_INVOKE_EXECUTE	"\033&f2X"
#define PCL_MACRO_INVOKE_CALL		"\033&f3X"

#define PCL_MACRO_TEMPORARY		"\033&f9X"
#define PCL_MACRO_PERMANENT		"\033&f10X"

//--------------PCL, extension con color

//Cuidado con la cadena CID final, tiene 0 (END_STR) 

#define PCL_CID		"\033*v6W"
#define PCL_CID_B0	0	//Espacio de color RGB
#define PCL_CID_B1	1	//Modo raster Indexado por pixel
#define PCL_CID_B2	8	//8 bits por pixel. Dicho de otro modo, 256 colores.
#define PCL_CID_B3	0	//Defecto para Primary 1.
#define PCL_CID_B4	0	//Idem 2
#define PCL_CID_B5	0	//Idem 3

#define PCL_SET_PALETTE_ENTRY "\033*v%liA\033*v%liB\033*v%liC\033*v%liI"

//Nota:para graficos, se usa el mecanismo push/pop de paletas en vez de manejarlas via Id, 
//	porque lo del Id no es soportado por todas impresoras, y el push/pop si.
//	Detalles: PCL 5 Comparison Guide, pág 1-23

#define PCL_PUSH_PALETTE "\033*p0P"
#define PCL_POP_PALETTE	 "\033*p1P" 

#define PCL_SET_FG "\033*v%liS"		//Ver, interactua con graficos raster

//--------------PCL, impresion graficos raster 

//****Comun en modos color 

#define PCL_GRAPHIC_75_DPI "\033*t75R"		
#define PCL_GRAPHIC_100_DPI "\033*t100R"
#define PCL_GRAPHIC_150_DPI "\033*t150R"
#define PCL_GRAPHIC_200_DPI "\033*t200R"
#define PCL_GRAPHIC_300_DPI "\033*t300R"
#define PCL_GRAPHIC_600_DPI "\033*t600R"

#define PCL_GRAPHIC_PRESENTATION_LOGICAL "\033*r0F"
#define PCL_GRAPHIC_PRESENTATION_PHYSICAL "\033*r3F"

#define PCL_GRAPHIC_START_CURRENT_X "\033*r1A"
#define PCL_GRAPHIC_END "\033*rB"	//Mejor este, *rC no soportado en algunas impresoras

#define PCL_GRAPHIC_COMPRESS_NONE "\033*b0M"

#define PCL_GRAPHIC_DATA_START "\033*b%iW"

#define PCL_SOURCE_WIDTH_AND_HEIGHT "\033*r%liS\033*r%liT"	//Pixels*Lineas


//****Extensiones PCL color

#define PCL_GRAPHIC_START_CURRENT_X_SCALE "\033*r3A"	//Mandar CID,luego este
#define PCL_TARGET_WIDTH_AND_HEIGHT "\033*t%liH\033*t%liV"	//En DeciPoints

//--------------PCL, comandos GL

//***Switching entre modos

#define PCL_GL_ENTER_IN_PCL_CURSOR "\033%1B"	//NO usar en printf por %
#define PCL_GL_ENTER_IN_GL_CURSOR  "\033%0B"	//NO usar en printf por %

#define PCL_GL_EXIT_TO_PCL_CURSOR "\033%0A"		//NO usar en printf por %
#define PCL_GL_EXIT_TO_GL_CURSOR  "\033%1A"		//NO usar en printf por %

//Usar este modo para que los movimientos GL no afecten a la posicion PCL.

#define PCL_GL_EXIT	PCL_GL_EXIT_TO_PCL_CURSOR

//Usar este modo para que movi. PCL no afecten a GL. Daria igual, los comandos siempre 
//	imprimen en modo absoluto, posicionando primero el Pen.

#define PCL_GL_ENTER PCL_GL_ENTER_IN_GL_CURSOR

//***PEN
//
//En BN, solo hay dos, el 0 (white pen) y el 1 (black pen)
//En color, hay tantos como la paleta de color activa. De hecho, la paleta activa tiene una
//	entrada adicional que es la anchura, y sirve para seleccion de pen. Es decir, seleccionar
//	un pen en color equivale a sleccionar un color en la paleta y a usar la entrada de ancho 
//	de dicha paleta.

//Mapeos de estilo de linea

#define PCL_GL_LINE_SOLID			"LT;"
#define PCL_GL_LINE_DASH			"LT2,5,1;"
#define PCL_GL_LINE_DOT				"LT1,1,1;"
#define PCL_GL_LINE_DASH_DOT		"LT4,5,1;"
#define PCL_GL_LINE_DASH_DOT_DOT	"LT7,5,1;"

//Ancho, en milim (cambiar antes de PenDown).

#define PCL_GL_PEN_WIDTH "PW%4.2f;"

#define PCL_GL_SELECT_BLACK_PEN "SP1;"
#define PCL_GL_SELECT_PEN "SP%i;"		//Util solo en modos color

//Pone el pen en la coordenada absoluta pasada (x,y) en plus.

#define PCL_GL_PEN_POS_ABSOLUTE "PA%li,%li;"

//Baja el pen, y dibuja desde la posicion GL actual hasta la indicada.
//Pueden meterse varios PD seguidos para dibujar poligonos.

#define PCL_GL_PEN_DOWN	"PD%li,%li;"

#define PCL_GL_PEN_UP	"PU;"

//***Inicializacion, una por página. Mandarlo TRAS restear pagina (landscape, tamaño, etc)
//
//El modo empieza el eje Y al reves que PCL, es decir, de abajo hacia arriba.
//Trabaja con unidades PLU (plotter units).
//Para optimizacion, por pagina reseteamos el area GL imprimible a todo el area PCL, asi
//	luego solo hay que entrar y salir del modo GL sin resetear el area cada vez.
//Tambien se supone que siempre se imprimen lineas solidas. Si no es asi, se cambia de modo,
//	se imprime la linea con el estilo no-solido, y se vuelve a restaurar el modo solido.

//Posiciona PCL en 0,0.
//Resetea GL ImageFrame al ancho/alto de la pagina logica PCL
//Pone el AnchorPoint a la actual posicion PCL.

//#define PCL_GL_PAGE_INIT "\033&a0h0V" "\033*c0x0y0T"

// AntonioADN. Bugs 162271 y 163052
// Existen varios temas relacionados con la región de dibujo por defecto del HP-GL/2:
// 1. Los márgenes verticales por defecto son algo superiores a los márgenes de la región por
//    defecto de PCL. Esto puede verse en la página 2-9 del manual de PCL5 de HP.
//    Esto provocaba el bug 162271 según el cúal ciertas líneas no se pintaban (por quedar fuera
//	  de la región HP-GL/2)
// 2. Se estaba utilizando el sistema de referencia por defecto HP-GL/2 (origen inferior izquierda
//    y eje Y hacia la parte superior de la página)
// 3. Para compensar este problema se estaba procesando de forma distinta el dibujo de las líneas
//	  con un margen bottom específico para este caso. Esto funciona bien en una impresora PCL5
//    pero en una impresora con soporte PCL6 se ha visto que las líneas se desplazan hacia arriba
//    (Bug 163052)
//
// Por todo ello se realiza lo siguiente:
// - Dejamos la antigua inicialización
//#define PCL_GL_PAGE_INIT "\033*p0x0Y" "\033*c0T" "\033*c0X" "\033*c0Y"
// - Se posiciona PCL en 0,0 (\033*p0x0Y)
// - Resetea GL ImageFrame al ancho de la página lógica PCL (los márgenes horizonales PCL y GL
//		por defecto coinciden) (\033*c0X)
// - Se fija el GL ImageFrame al alto máximo que puede tener el formato de página más grande (A3)
//    Esto es Alto A3 -> 420 mm -> 16,536 pulgadas -> 11960 decipoints (1' = 720 decipoints).
//	  La impresora cortará este área a los límites físicos del papel y, de esta forma, no es necesario
//    condicionar esta inicialización al tamaño de papel (\033*c11906Y)
// - Se unifica el sistema de referencia HP-GL/2 con el de PCL: Mismo origen (IR0,100,0,100),
//    eje Y ascendente hacia abajo pero se mantienen la escala PLU. Basado en página 19-19 del manual
//    de PCL5 de HP.
// Debido a que se cambia el sistema de referencia no hay ningún probleman en establecer como
// alto del ImageFrame la altura A3.
#define PCL_GL_PAGE_INIT "\033*p0x0Y" "\033*c0T" "\033*c0X" "\033*c11906Y"  PCL_GL_ENTER "SC0,1,0,-1,2" "IR0,100,0,100" PCL_GL_EXIT



//***Relleno de areas de color y bordes

//Mandar primero las dos esquinas apuestas del area, en el orden X1,Y1,X2,Y2
//Usara el actual Pen y line. Pone el FillType siempre a black fill.
//Util en modo color, NO en BW, ya que produce un rectangulo negro.

#define PCL_GL_FILL_RECT "FT;" PCL_GL_PEN_POS_ABSOLUTE "RA%li,%li;"	

//Mandar primero el nivel de sahding, de 0 a 100. Luego las coordenadas como en el anterior.
//Util en modo BW.

#define PCL_GL_SHADOW_RECT "FT10,%i;" PCL_GL_PEN_POS_ABSOLUTE "RA%li,%li;"	


//Mandar primero las dos esquinas apuestas del area, en el orden X1,Y1,X2,Y2
//Usara el actual Pen y line. 

#define PCL_GL_OUTLINE_RECT PCL_GL_PEN_POS_ABSOLUTE "EA%li,%li;"	

ClENG_DvcPCL::ClENG_DvcPCL(){
	m_poPagePrinter=NULL;
}

void ClENG_DvcPCL::End() {
	
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_poPagePrinter, ClENG_PagePrinter, GetNumLayoutPages() 
	) ;
	ClENG_DvcFile::End() ;
}

m4bool_t ClENG_DvcPCL::Init( 
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

		//Param especificos de device

		ClFONT_M_DBFont* ai_poDBFont, 
		m4bool_t ai_bIsColourOn, 
		m4int16_t ai_iNumCopies,
		PAGE_RESOLUTION ai_MaxResolution, 
		StENG_DvcParams::TUMBLE_MODE  ai_Tumble,


		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{
	m4bool_t Return=M4_TRUE ;
	ClMIT_Flag::Value_t Flags ;
	
	ClENG_DvcPCL::End();

	m_poDBFont=ai_poDBFont ;
	
	Flags = ClENG_OutDevice::RASTER_SUPPORTED | 
			ClENG_OutDevice::PRELOAD_ON_DEVICE | ClENG_OutDevice::RASTER_PRELOADED ;
	if ( ai_bIsColourOn ) {
		Flags = Flags | 
			ClENG_OutDevice::COLOUR_ACTIVATED | ClENG_OutDevice::COLOUR_SUPPORTED | 
			ClENG_OutDevice::HARD_DITHERING |  ClENG_OutDevice::HARD_SCALE ;
	} ;
	
	Return=ClENG_DvcFile::Init(

		//Campos para device

		ai_iNumPages,
		Flags, ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		ai_bForceMonoBreak,	//MultiBreak
		ai_FileParams, ai_bDelete,
		ClENG_BreakList::BREAK_TO_FILE,
		ClENG_DvcFile::GetMappingToFileDuplex(ai_Duplex), 
		ai_DeclareExtFile,
		ai_bCreateDir,
		ai_Preserve, 
		ai_pcToPrinter, 

		ClMIT_Trace::PROPAGATE_ERROR 
	);
	if (!Return) goto exit ;

	m_iNumCopies = ai_iNumCopies;
	m_MaxResolution=ai_MaxResolution ;
	m_Duplex = ai_Duplex ;
	m_Tumble = ai_Tumble ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_poPagePrinter, ClENG_PagePrinter, GetNumLayoutPages()
	) ;

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}

m4bool_t ClENG_DvcPCL::InitPage( m4int16_t ai_iIdPage,
				DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY, 
				size_t ai_szBlockBuffer, 
				ClENG_PagePrinter::ORIENTATION   ai_Orientation,
				ClENG_PagePrinter::PAPER_SIZE    ai_PageSize,
				ClENG_PagePrinter::PAPER_SOURCE  ai_PaperSource,
				DvcUnit_t ai_SizeX,   DvcUnit_t ai_SizeY,
				ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t Return=M4_TRUE ;
	m4bool_t initializePhantom ;
	
	if ( !M4_IN_RANGE(ai_iIdPage, 0, GetNumLayoutPages()-1) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCPCL_MSG_10 ] ) << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} ;

	m_poPagePrinter[ai_iIdPage].Init( ai_Orientation, ai_PageSize, ai_PaperSource, 
		ai_SizeX, ai_SizeY);

	Return=ClENG_DvcFile::InitPage( 
		ai_iIdPage, 
		m_poPagePrinter[ai_iIdPage].GetSizeX(),
		m_poPagePrinter[ai_iIdPage].GetSizeY(),
		ai_OffsetX, ai_OffsetY, 
		ai_szBlockBuffer, initializePhantom, /*m_pcBuff, */
		/*Redirect,*/
		ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!Return) goto exit ;

	if ( initializePhantom )  {
		m_poPagePrinter[ GetNumLayoutPages()-1 ].Init( 
			ai_Orientation, ai_PageSize, ai_PaperSource, 
			ai_SizeX, ai_SizeY);
	} ;


exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}

m4bool_t ClENG_DvcPCL::PreReset(){
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t Page, B;
	size_t Len ;
	M4ClBuffer::StCopy Param ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t * pcBuff = TaskEnv->Dvc_g_pcBuff ;
	ClFONT_M_DBFont::StSoftFontData* StSoftFont;

	ClENG_PhysicalFile *F ;
	m4bool_t swRedirect ;

	//------->Abre y prepara ficheros siempre (no hay ATOMIC mode)

	SwOk = M4_BOOL( SwOk && m_oOutRepository.NewBreak() );	
	if (!SwOk) goto exit ;
	
	//------>Datos que solo van una vez al principio del fichero (o ficheros)

	strcpy(pcBuff, PCL_PRINTER_RESET PCL_UNIVERSAL_EXIT PCL_MEASURE PCL_FONT_NOT_UNDERLINE)  ;
	
	if( m_Duplex==StENG_DvcParams::REAL_DUPLEX && m_Tumble==StENG_DvcParams::TUMBLE_TRUE) {
		strcat(pcBuff, PCL_TUMBLE ) ;
	} else {
		strcat(pcBuff, m_Duplex==StENG_DvcParams::REAL_DUPLEX ? PCL_DUPLEX : PCL_SIMPLEX ) ;
	};

	Len=strlen(pcBuff) ;
	Len += sprintf( &pcBuff[Len],PCL_NUM_COPIES, (int)m_iNumCopies ) ;
	
	// Bug 96299.
	// Establecer la bandeja donde reside el papel al principio del fichero. 
	// Esta información viene establecida por página, sin embargo, en PCL 
	// no funciona bien en modo real duplex. Además, es ilógico porque el tipo de bandeja
	// se establece como parámetro de la ejecución del report y no puede ser distinto por 
	// página. Es por esto que se establece la bandeja al principio del fichero y no 
	// por página. Como valor, escogemos la bandeja de la primera 
	// No soportadas en HP LaserJet
	switch (m_poPagePrinter[0].GetPaperSource())
	{
	case ClENG_PagePrinter::MANUAL:
		Len += sprintf(&pcBuff[Len], "%s", PCL_MANUAL_SOURCE);
		break;
	case ClENG_PagePrinter::UPPER_BIN:
		Len += sprintf(&pcBuff[Len], "%s", PCL_UPPER_BIN_SOURCE);
		break;
	case ClENG_PagePrinter::LOWER_BIN:
		Len += sprintf(&pcBuff[Len], "%s", PCL_LOWER_BIN_SOURCE);
		break;
	case ClENG_PagePrinter::USER_DEFINED:
	 	Len += sprintf(&pcBuff[Len], "%s%d%s", PCL_BIN_SOURCE_PREFIX, m_iPaperSourceCode, PCL_BIN_SOURCE_SUFFIX);
		break;
	} ;

	// Bug 96299. Por la misma razón que el anterior, si estamos en modo duplex, el tamaño 
	// de la página se debe establecer al principio del fichero y no por cada página.
	// En este caso, sí podría haber tamaños de páginas diferentes, al menos cada dos.
	// Esto se perdería.
	if (m_Duplex == StENG_DvcParams::REAL_DUPLEX)
	{
		switch (m_poPagePrinter[0].GetPageSize())
		{
		case ClENG_PagePrinter::A4:
			Len += sprintf(&pcBuff[Len], "%s", PCL_A4_PAPER_SIZE);
			break;
		case ClENG_PagePrinter::A3:
			Len += sprintf(&pcBuff[Len], "%s", PCL_A3_PAPER_SIZE);
			break;
		case ClENG_PagePrinter::LEGAL:
			Len += sprintf(&pcBuff[Len], "%s", PCL_LEGAL_PAPER_SIZE);
			break;
		case ClENG_PagePrinter::LETTER:
			Len += sprintf(&pcBuff[Len], "%s", PCL_LETTER_PAPER_SIZE);
			break;
		case ClENG_PagePrinter::EXECUTIVE:
			Len += sprintf(&pcBuff[Len], "%s", PCL_EXECUTIVE_PAPER_SIZE);
			break;		
		} ;
	}

	if ( IsColourOn() ) {

		//Añade modo 256 colores indexado, lo cual afecta al tamaño de la paleta activa
	
		Len += sprintf( &pcBuff[Len], "%s", PCL_CID ) ;
		pcBuff[Len]=PCL_CID_B0 ;	++Len ;
		pcBuff[Len]=PCL_CID_B1 ;	++Len ;
		pcBuff[Len]=PCL_CID_B2 ;	++Len ;
		pcBuff[Len]=PCL_CID_B3 ;	++Len ;
		pcBuff[Len]=PCL_CID_B4 ;	++Len ;
		pcBuff[Len]=PCL_CID_B5 ;	++Len ;

		//Añade paleta FG, valida y unica en todo el fichero.

		Len = SetActivePalette( &m_oFGPalette, pcBuff, Len, StENG_TaskEnv::DvcPCL_MAX_SIZE ) ;

	} ;

	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPCL_MAX_SIZE,Len+1) ;
	
	for (Page=0;Page<GetNumLayoutPages();++Page) {

		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;

		if ( ! swRedirect ) {

			F->W(pcBuff,Len) ;

		} ;
	} ;

	//SoftFonts, bucle posterior para reutilizar pcBuff
	for (Page=0;Page<GetNumLayoutPages();++Page){	

		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;
	
		if ( ! swRedirect ) {
			
			for(B=0; B<m_poDBFont->GetNumSoftFonts(); B++) {
		
				StSoftFont=m_poDBFont->GetSoftFontDataByIndex(B);

				Len=sprintf( pcBuff, PCL_SOFT_FONT_BEG_DOWNLOAD, 
					(unsigned int) StSoftFont->m_poFont->PCL_GetTypeFace() ) ;
				ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPCL_MAX_SIZE,Len+1) ;

				F->WStr(pcBuff) ;

				F->WFile(	StSoftFont->m_oPathName, 
					        StSoftFont->m_Offset, StSoftFont->m_Size) ;

				F->WStr(PCL_SOFT_FONT_END_DOWNLOAD ) ;
			} ;
		} ;
	} ;

	//------>Datos Preload Optimized, van a macro. Son primero los codigos de parametrizacion
	//	de pagina, y luego los datos metidos como Optimized Preload.   
	//	Una pagina layout----->1 macro

	for (Page=0;Page<GetNumLayoutPages();++Page) {

		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;
		
		//---->Secuencia de comienzo a grabar macro ....

		Len = sprintf(pcBuff, CR_NL PCL_MACRO_ID PCL_MACRO_PERMANENT PCL_MACRO_START CR_NL, 
			(int)BEGIN_ID_MACRO_RESERVED+Page );

		//---->Resets de pagina

		if (m_poPagePrinter[Page].GetOrientation() == ClENG_PagePrinter::PORTRAIT ){
			strcat(pcBuff,PCL_PORTRAIT);	
		} else {
			strcat(pcBuff,PCL_LANDSCAPE);	
		} ;
		
		// Bug 96299. Si no estamos en modo duplex, el tamaño de papel se vuelca por página.
		// No soportadas en HP LaserJet
		if (m_Duplex != StENG_DvcParams::REAL_DUPLEX)
		{
			switch ( m_poPagePrinter[Page].GetPageSize() ){
			case ClENG_PagePrinter::A4:
				strcat(pcBuff,PCL_A4_PAPER_SIZE);	
				break;
			case ClENG_PagePrinter::A3:
				strcat(pcBuff,PCL_A3_PAPER_SIZE);	
				break;
			case ClENG_PagePrinter::LEGAL:
				strcat(pcBuff,PCL_LEGAL_PAPER_SIZE);	
				break;
			case ClENG_PagePrinter::LETTER:
				strcat(pcBuff,PCL_LETTER_PAPER_SIZE);	
				break;
			case ClENG_PagePrinter::EXECUTIVE:
				strcat(pcBuff,PCL_EXECUTIVE_PAPER_SIZE);	
				break;		
			} ;
		}

		strcat(pcBuff,PCL_RESET_MARGIN) ;

		//Reseteo modo GL

		strcat(pcBuff,PCL_GL_PAGE_INIT) ;

		//En modo color, el pen se seleccionará conforme al color activo. En BW, siempre negro.
		if ( !IsColourOn() ) {
			strcat(pcBuff, PCL_GL_ENTER PCL_GL_SELECT_BLACK_PEN PCL_GL_EXIT) ;
		} ;

		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPCL_MAX_SIZE) ;

		F->WStr(pcBuff) ;

		//------->Ya he escrito resets de pagina, la macro sigue abierta. 
		//	Ahora se copian los datos Optimized Preload

		F->DumpBuffer( &m_poLayoutPages[Page].m_oOpPrBuff ) ;

		//------>Y ya se marca fin de macro

		F->WStr(PCL_MACRO_END CR_NL) ;

		//------>En la parte preserve del buffer de pagina va la llamada a la macro de pagina 
		//	correspondiente y los datos Preload no optimizados.


		m_poLayoutPages[Page].m_oLayoutBuff.Init() ;	//Se carga el buffer, preserve incluido, por si acaso

		Len = sprintf(pcBuff, PCL_MACRO_ID "%s", 
			(int)BEGIN_ID_MACRO_RESERVED+Page, PCL_MACRO_INVOKE_EXECUTE );
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPCL_MAX_SIZE) ;

		m_poLayoutPages[Page].m_oLayoutBuff.Append(pcBuff, Len) ;

		m_poLayoutPages[Page].m_oLayoutBuff.CatM4ClBuffer(&m_poLayoutPages[Page].m_oPrBuff) ;

	} ;

	//---------->Propaga

	ClENG_OutDevice::PreReset() ;

exit:
	return SwOk ;
}


void ClENG_DvcPCL::PostReset()
{
	m4int16_t Page, B ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t * pcBuff = TaskEnv->Dvc_g_pcBuff ;
	ClFONT_M_DBFont::StSoftFontData* StSoftFont;
	size_t Len ;

	ClENG_PhysicalFile *F ;
	m4bool_t swRedirect ;

	for (Page=0;Page<GetNumLayoutPages();++Page){

		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;

		//Se carga las macros y mete el codigo de fin de job

		sprintf(pcBuff, PCL_MACRO_ID PCL_MACRO_DELETE 
			PCL_PRINTER_RESET, (int)BEGIN_ID_MACRO_RESERVED+Page);
		strcat( pcBuff , PCL_UNIVERSAL_EXIT ) ;	
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPCL_MAX_SIZE) ;

		F->WStr(pcBuff) ;
	} ;

	//SoftFonts, descarga

	for (Page=0;Page<GetNumLayoutPages();++Page){	

		F = m_oOutRepository.GetFlushFile(Page, &swRedirect ) ;

		if ( ! swRedirect ) {
			
			for(B=0; B<m_poDBFont->GetNumSoftFonts(); B++) {
		
				StSoftFont=m_poDBFont->GetSoftFontDataByIndex(B);

				Len=sprintf( pcBuff, PCL_SOFT_FONT_UNLOAD, 
					(unsigned int) StSoftFont->m_poFont->PCL_GetTypeFace() ) ;
				ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPCL_MAX_SIZE,Len+1) ;
				F->WStr(pcBuff) ;

			} ;
		} ;
	} ;

	//---------->Cierra ficheros (no hay ATOMIC mode)

	m_oOutRepository.CloseBreak( IsValidOutput() ) ;

	//---------->Propaga

	ClENG_OutDevice::PostReset(M4_FALSE);
}


m4bool_t  ClENG_DvcPCL::RefreshLayout() 
{
	m4bool_t SwOk=M4_TRUE ;

	//---------->Notifica a FileRep (no abre files, no ATOMIC_MODE)

	SwOk=m_oOutRepository.NewFlush() ;

	//---------->Propaga

	return SwOk && ClENG_OutDevice::RefreshLayout() ;
}


m4bool_t ClENG_DvcPCL::FlushLayout()
{
	m4bool_t SwOk=M4_TRUE ;

	//---------->Corre EOPs

	RunAllEOP() ;

	//---------->Vuelca buffers a files  (no cierra files, no ATOMIC_MODE)

	SwOk=m_oOutRepository.CloseFlush( ClENG_DvcFile::m_poBuffArray ) ;

	//---------->Propaga

	return SwOk && ClENG_OutDevice::FlushLayout();
}
			
void ClENG_DvcPCL::SelectFontDevice(m4int16_t ai_iPage, ClFONT_M_Font * ai_poFont)
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t * m_pcBuff = TaskEnv->Dvc_g_pcBuff ;
	M4ClBuffer *PageBuffer=m_poLayoutPages[ai_iPage].m_poActualBuff ;

	//Ojo con cambiar el orden de estos comandos, afectan tanto en Fonts como en SoftFonts
	
	sprintf(m_pcBuff,PCL_FONT_SYMBOL_SET,ai_poFont->PCL_GetSymbolSet() );
	PageBuffer->AppendStr(m_pcBuff);

	if (ai_poFont->IsFixedSpace()) {
		PageBuffer->AppendStr(PCL_FONT_FIXED);
	} else {
		PageBuffer->AppendStr(PCL_FONT_NOT_FIXED);
	} ;
	sprintf(m_pcBuff, PCL_FONT_PITCH, (double)ai_poFont->PCL_GetPitch());
	PageBuffer->AppendStr(m_pcBuff);
	
	sprintf(m_pcBuff, PCL_FONT_HEIGHT, (double)ai_poFont->PCL_GetHeight() );
	PageBuffer->AppendStr(m_pcBuff);

	m_poPagePrinter[ai_iPage].SetUnderline( ai_poFont->IsUnderline() ) ;

	if (ai_poFont->IsItalic()){
		PageBuffer->AppendStr(PCL_FONT_ITALIC);
	} else {
		PageBuffer->AppendStr(PCL_FONT_NOT_ITALIC);
	} ;
	if (ai_poFont->IsBold()){
		PageBuffer->AppendStr(PCL_FONT_BOLD);
	} else{
		PageBuffer->AppendStr(PCL_FONT_NOT_BOLD);
	} ;

	sprintf(m_pcBuff, 
		ai_poFont->IsSoftFont() ? PCL_FONT_ID : PCL_FONT_TYPE_FACE,
		(int)ai_poFont->PCL_GetTypeFace() );
	PageBuffer->AppendStr(m_pcBuff);
}

void ClENG_DvcPCL::PrintNChar( m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
		m4pchar_t ai_pcStr, size_t ai_szNChar, 
		m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID,
		ClENG_OutDevice::StPIParams *ai_pstPI) 
{
	size_t LenStart = 0;
	size_t LenEnd = 0;
	m4uint32_t lValueOffset = 0; size_t szClipSize=0 ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t * m_pcBuff = TaskEnv->Dvc_g_pcBuff ;
	M4ClBuffer *PageBuffer=m_poLayoutPages[ai_iPage].m_poActualBuff ;

	ai_iX += GetOffsetX(ai_iPage) ;
	ai_iY += GetOffsetY(ai_iPage) + PCL_Y_OFFSET_TOP ;
	ai_iY += GetLastFont(ai_iPage)->GetDvcHeight() ;	//Ajuste de linea-base

	// UNICODE REPORTS.
	// Convertir una cadena (multi byte) a multibyte ansi. Si estamos en ansi, te devuelve
	// ansi, es decir, esa misma cadena. Si estamos en unicode, pasa de utf8 a ansi.
	// wide char.
	// Bug 0125206. En los campos multilínea te llega el contenido del campo completo
	// pero en el número de caracteres te llega los que realmente quiere imprimir.
	m4pchar_t pcString = NULL;
	m4pchar_t pcInputString = NULL;
	int iLen = 0;

	pcInputString = new m4char_t[ai_szNChar + 1];
	memcpy(pcInputString, ai_pcStr, ai_szNChar);
	pcInputString[ai_szNChar] = '\0';

	iLen = strlen(pcInputString);
	pcString = M4CppToANSI(pcInputString, iLen);

	if (m_poPagePrinter[ai_iPage].IsUnderline())
	{
		LenStart=sprintf(m_pcBuff, PCL_NO_ZSTRING_UNDERLINE_START, 
		(int)ClMetric::DvcToRoundDPoint(ai_iX), 
		(int)ClMetric::DvcToRoundDPoint(ai_iY)) ; 

		LenEnd = sprintf(m_pcBuff+LenStart, 
		 PCL_NO_ZSTRING_UNDERLINE_END, (size_t)iLen, pcString ) ; 
	}
	else
	{
		LenStart=sprintf(m_pcBuff, PCL_NO_ZSTRING_START, 
			(int)ClMetric::DvcToRoundDPoint(ai_iX), 
			(int)ClMetric::DvcToRoundDPoint(ai_iY)) ;
		LenEnd = sprintf(m_pcBuff+LenStart,PCL_NO_ZSTRING_END,(size_t)iLen,pcString ) ; 
	}

	delete [] pcString;
	delete [] pcInputString;

	// Ojo, no usar strlen() porque la cadena puede tener 0 por codigos especiales
	if ((LenStart + LenEnd +1) > StENG_TaskEnv::DvcPCL_MAX_SIZE) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCPCL_MSG_20 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;

	} ;

	// Calculo el Offset : Posicion del buffer antes de que escriba + Len de la secuencia inicial
	lValueOffset = PageBuffer->GetTotal() + LenStart;
    
	PageBuffer->Append(m_pcBuff,(LenStart + LenEnd));

	if ( ai_pstPI ) {
		ai_pstPI->m_uiOff=lValueOffset ;
		ai_pstPI->m_szNChars=ai_szNChar ;
	} ;

}

void ClENG_DvcPCL::PrintStr(m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
	m4pchar_t ai_pcStr, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID) 
{
	size_t Len ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t * m_pcBuff = TaskEnv->Dvc_g_pcBuff ;
	M4ClBuffer *PageBuffer=m_poLayoutPages[ai_iPage].m_poActualBuff ;
	m4pchar_t pcString = NULL;
	int iLen = 0;

	ai_iX += GetOffsetX(ai_iPage) ;
	ai_iY += GetOffsetY(ai_iPage) + PCL_Y_OFFSET_TOP ;
	ai_iY += GetLastFont(ai_iPage)->GetDvcHeight() ;	//Ajuste de linea-base

	// UNICODE REPORTS.
	// Convertir una cadena (multi byte) a multibyte ansi. Si estamos en ansi, te devuelve
	// ansi, es decir, esa misma cadena. Si estamos en unicode, pasa de utf8 a ansi.
	// wide char.
	iLen = strlen(ai_pcStr);
	pcString = M4CppToANSI(ai_pcStr, iLen);
	
	Len=sprintf(m_pcBuff,  
		m_poPagePrinter[ai_iPage].IsUnderline() ? PCL_ZSTRING_UNDERLINE : PCL_ZSTRING, 
		(int)ClMetric::DvcToRoundDPoint(ai_iX), 
		(int)ClMetric::DvcToRoundDPoint(ai_iY), pcString ) ; 

	delete [] pcString;

	// Ojo, no usar strlen() porque la cadena puede tener 0 por codigos especiales
	if (Len+1 > StENG_TaskEnv::DvcPCL_MAX_SIZE) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCPCL_MSG_20 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	PageBuffer->Append(m_pcBuff,Len);
}

void  ClENG_DvcPCL::PrintNCharPi(
	ClMIT_Str  *ai_poIdPi,m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
	size_t ai_szNChar, 
	m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID)
{
	ClENG_OutDevice::StPIParams PIParams ;
	ClMIT_Str StrAux ;

	// Creo la cadena con tantas X como ai_sNChar indica
	StrAux.StrDuplicateChar(0,ai_szNChar,'X') ;

	// Imprimo la cadena y tomo el offset
	PIParams.m_oIdPi.StrCpy(ai_poIdPi->InChar(0));
	PrintNChar(ai_iPage,ai_iX,ai_iY,StrAux,ai_szNChar, ai_bRawText, ai_pcHyperlink,ai_iTargetID, &PIParams);

	// Añado el Pi a la lista
	if (PIParams.m_szNChars) {
		m_oOutRepository.AddNewPi( ai_poIdPi->InChar(0), ai_iPage, 
			PIParams.m_uiOff, PIParams.m_szNChars ) ;
	} ;
}


void ClENG_DvcPCL::ResolvePi(ClMIT_Str  *ai_poIdPi, m4pchar_t ai_pcStr) 
{
	StPi_t piData ;

	m4uint32_t Len;
	m4uint32_t PosActual;
	ClMIT_Str StrAux ;
	
	m4pchar_t pcString;
	
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
		   *m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCPCL_MSG_40 ] ) 
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
			M4ClBuffer::StCopy Param = { StrAux.InChar(0), piData.m_NBytes};	           
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
			piData.m_poFile->SetPos( piData.m_Offset);

			// Imprimo y dejo en fichero en la posicion inicial
			piData.m_poFile->W(StrAux.InChar(0), piData.m_NBytes);
			piData.m_poFile->SetPos(PosActual);
			if(bWasClosed){
				//Solo lo cierro si ya lo estaba
				piData.m_poFile->Close();
			};
		};	
  };

  return;
}

void ClENG_DvcPCL::EOP(m4int16_t ai_iPage) {
	m_poLayoutPages[ai_iPage].m_poActualBuff->AppendStr(PCL_EOP);
	ClENG_OutDevice::EOP(ai_iPage) ;
}

//---------Graficos

struct ClENG_DvcPCL::stGraphicResolution const ClENG_DvcPCL::GraphicResolution[ClENG_DvcPCL::N_RESOLUTIONS] = {
	{75 ,4,	8, PCL_GRAPHIC_75_DPI },
	{100,3,	6, PCL_GRAPHIC_100_DPI },
	{150,2,	4, PCL_GRAPHIC_150_DPI },
	{200,0,	3, PCL_GRAPHIC_200_DPI },
	{300,1,	2, PCL_GRAPHIC_300_DPI },
	{600,0,	1, PCL_GRAPHIC_600_DPI },
} ;


void ClENG_DvcPCL::GetScaleData( 
	DvcUnit_t ai_SizeXDvc,			DvcUnit_t ai_SizeYDvc,			
	m4uint32_t ai_iRasterXMap,	m4uint32_t ai_iRasterYMap,			
	m4uint32_t *ao_piRasterXDvc, m4uint32_t *ao_piRasterYDvc, m4int16_t *ao_piGResIndex ) 
{
	m4int16_t Factor, B, FactorPCL ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t * m_pcBuff = TaskEnv->Dvc_g_pcBuff ;
	
	// Haya los raster de dispositivo

	*ao_piRasterXDvc= (m4uint32_t)(ClMetric::DvcToInch( ai_SizeXDvc ) * 
		( m_MaxResolution == P_300_DPI ?  P_300_DPI : P_600_DPI )) ;
	*ao_piRasterYDvc= (m4uint32_t)(ClMetric::DvcToInch( ai_SizeYDvc ) * 
		( m_MaxResolution == P_300_DPI ?  P_300_DPI : P_600_DPI )) ;

	// Como la impresora, via resolucion de graficos, puede expandir un punto, reducimos las
	//	raster-lines de dispositivo, siempre que no perdamos resolucion en el proceso.

	Factor=M4_MIN( *ao_piRasterXDvc/ai_iRasterXMap, *ao_piRasterYDvc/ai_iRasterYMap ) ;
	Factor=M4_MAX(Factor,1) ;
	for (B=0; B<N_RESOLUTIONS; ++B ) {
		FactorPCL = ( m_MaxResolution == P_300_DPI ? GraphicResolution[B].ScaleFactor300 :
			GraphicResolution[B].ScaleFactor600 ) ;
			if ( FactorPCL != 0 && FactorPCL<=Factor ) break ;
	} ;
	*ao_piRasterXDvc /= FactorPCL ;		
	*ao_piRasterYDvc /= FactorPCL ;		
	if ( ao_piGResIndex )  *ao_piGResIndex=B ;
}

void ClENG_DvcPCL::PrintRasterMap(
	m4int16_t ai_iPage,
	DvcUnit_t ai_X,				DvcUnit_t ai_Y,				
	DvcUnit_t ai_SizeX,			DvcUnit_t ai_SizeY,			
	ClENG_RasterMap * ai_poMap, m4char_t *ai_pcHyperlink,
	m4uint16_t ai_iTargetID ) 
{
	m4uint32_t RasterXDvc, RasterYDvc ;
	m4int16_t GResIndex ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t * m_pcBuff = TaskEnv->Dvc_g_pcBuff ;
	M4ClBuffer *PageBuffer=m_poLayoutPages[ai_iPage].m_poActualBuff ;

	m4uint8_t *Map ;
	m4uint32_t SkipBytesRow, WriteBytesRow, YCount ; 
	size_t LenStrAux ;

	ai_X += GetOffsetX(ai_iPage) ;
	ai_Y += GetOffsetY(ai_iPage) ;

	GetScaleData( ai_SizeX, ai_SizeY, ai_poMap->GetRasterX(), ai_poMap->GetRasterY(),
		&RasterXDvc, &RasterYDvc, &GResIndex ) ;
	
	if ( ! IsColourOn() ) {

		//*******Impresion PCL estandar; no se soporta ni hard-scaling ni hard-dithering

		//Temporalmente, hasta que haya dithering

		if (ai_poMap->GetBitsPixel()!=ClENG_RasterMap::MONOCHROME ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCPCL_MSG_30 ] ) 
				<< (m4uint32_t)ai_poMap->GetBitsPixel() << SEND_MSG ;
			goto exit ;
		} ;

		// Posiciona el cursor

		sprintf(m_pcBuff, PCL_POS, 
			(int)ClMetric::DvcToRoundDPoint(ai_X), (int)ClMetric::DvcToRoundDPoint(ai_Y + PCL_Y_OFFSET_TOP) ) ;
		PageBuffer->AppendStr(m_pcBuff);

		// Pone la resolucion de grafico adecuada

		PageBuffer->AppendStr(GraphicResolution[GResIndex].StrSetRes);
	
		// Modo de presentacion normal 
	
		PageBuffer->AppendStr(PCL_GRAPHIC_PRESENTATION_LOGICAL);

		// Start grafico, desde posicion actual

		PageBuffer->AppendStr(PCL_GRAPHIC_START_CURRENT_X);

		// Sin compresion	

		PageBuffer->AppendStr(PCL_GRAPHIC_COMPRESS_NONE);

		// Deja la cadena preparada con el comienzo de todas raster-line. 

		WriteBytesRow = ai_poMap->GetLastXIndex()+1 ;
		LenStrAux = sprintf(m_pcBuff, PCL_GRAPHIC_DATA_START, (int)WriteBytesRow ) ;

		//Imprimir raster

		for ( 
			SkipBytesRow  = ai_poMap->GetBytesRow(), 
			Map = ai_poMap->GetMap(), 
			YCount=ai_poMap->GetRasterY() ;		
			YCount ; Map +=SkipBytesRow, --YCount ) 
		{
			//Ojo, indicar len, puede salir algun nulo
			PageBuffer->Append(m_pcBuff, LenStrAux );
			//Ojo, indicar len, puede salir algun nulo
			PageBuffer->Append(Map, WriteBytesRow);
		} ;

		// Fin grafico

		PageBuffer->AppendStr( PCL_GRAPHIC_END );

	} else  {

		//*******Impresion PCL extension color; se soporta hard-scaling y hard-dithering

		if (ai_poMap->GetBitsPixel()!=ClENG_RasterMap::INDEXED_256 ) {
			if ( ! ai_poMap->RemapTo256Colors() ) goto exit  ;
		} ;

		// Hace un push de la paleta actual que es la ForeGround
		
		PageBuffer->AppendStr(PCL_PUSH_PALETTE);

		// Carga la paleta del gráfico

		SetActivePalette( ai_poMap->GetPalette(), m_pcBuff, 0, StENG_TaskEnv::DvcPCL_MAX_SIZE ) ;
		PageBuffer->AppendStr(m_pcBuff);

		// Pag 3-19, FG interactúa con raster, hacer que apunte a negro. 
		// Parece que en todas las paletas de graficos el 0 es el negro?

		sprintf(m_pcBuff, PCL_SET_FG, (long)0 ) ;
		ClMIT_Str::OverStringValid(m_pcBuff,StENG_TaskEnv::DvcPCL_MAX_SIZE) ;
		PageBuffer->AppendStr(m_pcBuff);

		// Posiciona el cursor

		sprintf(m_pcBuff, PCL_POS, 
			(int)ClMetric::DvcToRoundDPoint(ai_X), (int)ClMetric::DvcToRoundDPoint(ai_Y + PCL_Y_OFFSET_TOP) ) ;
		PageBuffer->AppendStr(m_pcBuff);

		// Pone la resolucion de grafico adecuada (con Scale, sin efecto)

		// PageBuffer->AppendStr(GraphicResolution[GResIndex].StrSetRes);
	

		// Pone width y height del grafico (source)

		sprintf(m_pcBuff, PCL_SOURCE_WIDTH_AND_HEIGHT, 
			(long)ai_poMap->GetRasterX(), (long)ai_poMap->GetRasterY() ) ;
		PageBuffer->AppendStr(m_pcBuff);

		// Pone width y height destino en el device

		sprintf(m_pcBuff, PCL_TARGET_WIDTH_AND_HEIGHT, 
			(long)ClMetric::DvcToDPoint(ai_SizeX), (long)ClMetric::DvcToDPoint(ai_SizeY) ) ;
		PageBuffer->AppendStr(m_pcBuff);

		// Modo de presentacion normal 
	
		PageBuffer->AppendStr(PCL_GRAPHIC_PRESENTATION_LOGICAL);

		// Start grafico, desde posicion actual

		PageBuffer->AppendStr(PCL_GRAPHIC_START_CURRENT_X_SCALE);

		// Sin compresion	

		PageBuffer->AppendStr(PCL_GRAPHIC_COMPRESS_NONE);

		// Deja la cadena preparada con el comienzo de todas raster-line. 

		WriteBytesRow = ai_poMap->GetLastXIndex()+1 ;
		LenStrAux = sprintf(m_pcBuff, PCL_GRAPHIC_DATA_START, (int)WriteBytesRow ) ;

		//Imprimir raster

		for ( 
			SkipBytesRow  = ai_poMap->GetBytesRow(), 
			Map = ai_poMap->GetMap(), 
			YCount=ai_poMap->GetRasterY() ;		
			YCount ; Map +=SkipBytesRow, --YCount ) 
		{
			//Ojo, indicar len, puede salir algun nulo
			PageBuffer->Append(m_pcBuff, LenStrAux );
			//Ojo, indicar len, puede salir algun nulo
			PageBuffer->Append(Map, WriteBytesRow);
		} ;

		// Fin grafico

		PageBuffer->AppendStr( PCL_GRAPHIC_END );

		// Restaura la paleta ForeGround
		
		PageBuffer->AppendStr(PCL_POP_PALETTE);

		// Restaura el color ForeGround, si habia alguno antes

		if ( GetLastColour(ai_iPage) != ClENG_Colour::ID_NULL ) {
			sprintf(m_pcBuff, PCL_SET_FG, (long)m_oFGPalette.GetIndex(GetLastColour(ai_iPage)) ) ; 
			ClMIT_Str::OverStringValid(m_pcBuff,StENG_TaskEnv::DvcPCL_MAX_SIZE) ;
			PageBuffer->AppendStr(m_pcBuff);
		} ;
	
	} ;
exit:
	;
}


void ClENG_DvcPCL::SelectColourDevice( m4int16_t ai_iPage, m4uint32_t ai_uiIdColour ) 
{
	if ( IsColourOn() ) {

		M4ClBuffer *PageBuffer=m_poLayoutPages[ai_iPage].m_poActualBuff ;
		StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
		m4char_t * pcBuff = TaskEnv->Dvc_g_pcBuff ;

		sprintf(pcBuff, PCL_SET_FG, (long)m_oFGPalette.GetIndex(ai_uiIdColour) ) ;
		ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPCL_MAX_SIZE) ;
		PageBuffer->AppendStr(pcBuff);
	} ;
}

size_t ClENG_DvcPCL::SetActivePalette( ClENG_Palette *ai_poPalette, m4char_t *ai_pcBuff, size_t ai_szFrom, size_t ai_szMaxSize ) 
{
	m4int16_t PIndex ;
	ClENG_Colour *Colour ;
	size_t Written ;

	for (PIndex=0, Written=ai_szFrom ; PIndex < ai_poPalette->GetSize() ; ++PIndex) {

		Colour = ai_poPalette->GetColourByIndex(PIndex) ;

		Written += sprintf(&ai_pcBuff[Written], PCL_SET_PALETTE_ENTRY, (long)Colour->GetR(), (long)Colour->GetG(),
			(long)Colour->GetB(), (long)PIndex ) ;
	} ;
	ClMIT_Str::OverStringValid( ai_pcBuff, ai_szMaxSize, Written+1 ) ;

	return Written ;
}

void ClENG_DvcPCL::PrintLine( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X1, DvcUnit_t ai_Y1, DvcUnit_t ai_X2, DvcUnit_t ai_Y2,
	DvcUnit_t ai_LineWidth, STYLE ai_Style ) 
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff=TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	size_t L ;

	RoundPLU_t X1, X2, Y1, Y2 ;

	
	//Se añade el Offset del limite fisico de la pagina por la izquierda

	X1 = ClMetric::DvcToRoundPLU( ai_X1 + GetOffsetX(ai_iPage)  ) ;
	X2 = ClMetric::DvcToRoundPLU( ai_X2 + GetOffsetX(ai_iPage)  ) ;

	// AntonioADN. Bugs 162271 y 163052
	// Se modifica el cálculo de Y1 e Y2 según el nuevo sistema de referencia
	// coincidente con PCL (ver comentario en el comentario PCL_GL_PAGE_INIT)
	Y1 = ClMetric::DvcToRoundPLU( ai_Y1 + GetOffsetY(ai_iPage) + PCL_Y_OFFSET_TOP ) ;
	Y2 = ClMetric::DvcToRoundPLU( ai_Y2 + GetOffsetY(ai_iPage) + PCL_Y_OFFSET_TOP ) ;

	//Entra en modo GL

	strcpy( pcBuff, PCL_GL_ENTER) ;
	L = strlen(pcBuff) ;		//No printf por %

	//Mapea estilo solo si no es solido (defecto)

	if ( ai_Style!=ClENG_OutDevice::SOLID ) {
		switch(ai_Style) {
		case DASH:
			L += sprintf( &pcBuff[L], PCL_GL_LINE_DASH) ;
			break ;
		case DOT:
			L += sprintf( &pcBuff[L], PCL_GL_LINE_DOT) ;
			break ;
		case DASH_DOT:
			L += sprintf( &pcBuff[L], PCL_GL_LINE_DASH_DOT) ;
			break ;
		case DASH_DOT_DOT:
			L += sprintf( &pcBuff[L], PCL_GL_LINE_DASH_DOT_DOT) ;
			break ;
		} ;
	} ;

	//En modo color, ultimo SelectColour puso pen+color, solo falta el ancho y seleccionar el
	//	Pen.
	//En modo BW, tambien falta el ancho, pero nunca seleccionamos otro Pen

	if ( IsColourOn() ) {
		L += sprintf( &pcBuff[L], PCL_GL_SELECT_PEN, 
			(m4int16_t)m_oFGPalette.GetIndex(GetLastColour(ai_iPage)) ) ;
	} ;
	L += sprintf( &pcBuff[L], PCL_GL_PEN_WIDTH, (double) ClMetric::DvcToMilim(ai_LineWidth)  ) ;

	//Ahora dibuja linea

	L += sprintf( &pcBuff[L], PCL_GL_PEN_POS_ABSOLUTE, (long)X1, (long)Y1 ) ;
	L += sprintf( &pcBuff[L], PCL_GL_PEN_DOWN		, (long)X2, (long)Y2 ) ;
	L += sprintf( &pcBuff[L], PCL_GL_PEN_UP ) ;

	//Restaura estilo solido, si debe

	if ( ai_Style!=ClENG_OutDevice::SOLID ) {
		L += sprintf( &pcBuff[L], PCL_GL_LINE_SOLID) ;
	} ;

	//Sale a modo PCL
	strcat(&pcBuff[L], PCL_GL_EXIT) ;
	L += strlen(PCL_GL_EXIT) ;	//No printf por %
	
	//Vuelca resultados al buffer

	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPCL_MAX_SIZE,L+1) ;

	PageBuffer->Append(pcBuff,L) ;
}

void ClENG_DvcPCL::PrintFrame( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
	DvcUnit_t ai_LineWidth, STYLE ai_Style ) 
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff=TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	size_t L ;

	RoundPLU_t X1, X2, Y1, Y2 ;

	
	//Se añade el Offset del limite fisico de la pagina por la izquierda

	X1 = ClMetric::DvcToRoundPLU( ai_X + GetOffsetX(ai_iPage)  ) ;
	X2 = ClMetric::DvcToRoundPLU( ai_X + ai_Width + GetOffsetX(ai_iPage)  ) ;

	// AntonioADN. Bugs 162271 y 163052
	// Se modifica el cálculo de Y1 e Y2 según el nuevo sistema de referencia
	// coincidente con PCL (ver comentario en el comentario PCL_GL_PAGE_INIT)
	Y1 = ClMetric::DvcToRoundPLU( ai_Y + GetOffsetY(ai_iPage) + PCL_Y_OFFSET_TOP ) ;
	Y2 = ClMetric::DvcToRoundPLU( ai_Y + ai_Height + GetOffsetY(ai_iPage) + PCL_Y_OFFSET_TOP ) ;


	//Entra en modo GL

	strcpy( pcBuff, PCL_GL_ENTER) ;
	L = strlen(pcBuff) ;		//No printf por %

	//Mapea estilo solo si no es solido (defecto)

	if ( ai_Style!=ClENG_OutDevice::SOLID ) {
		switch(ai_Style) {
		case DASH:
			L += sprintf( &pcBuff[L], PCL_GL_LINE_DASH) ;
			break ;
		case DOT:
			L += sprintf( &pcBuff[L], PCL_GL_LINE_DOT) ;
			break ;
		case DASH_DOT:
			L += sprintf( &pcBuff[L], PCL_GL_LINE_DASH_DOT) ;
			break ;
		case DASH_DOT_DOT:
			L += sprintf( &pcBuff[L], PCL_GL_LINE_DASH_DOT_DOT) ;
			break ;
		} ;
	} ;

	//En modo color, ultimo SelectColour puso pen+color, solo falta el ancho y seleccionar el
	//	Pen.
	//En modo BW, tambien falta el ancho, pero nunca seleccionamos otro Pen

	if ( IsColourOn() ) {
		L += sprintf( &pcBuff[L], PCL_GL_SELECT_PEN, 
			(m4int16_t)m_oFGPalette.GetIndex(GetLastColour(ai_iPage)) ) ;
	} ;
	L += sprintf( &pcBuff[L], PCL_GL_PEN_WIDTH, (double) ClMetric::DvcToMilim(ai_LineWidth)  ) ;

	//Ahora dibuja frame

	L += sprintf( &pcBuff[L], PCL_GL_OUTLINE_RECT, (long)X1, (long)Y1, (long)X2, (long)Y2 ) ;
	L += sprintf( &pcBuff[L], PCL_GL_PEN_UP ) ;

	//Restaura estilo solido, si debe

	if ( ai_Style!=ClENG_OutDevice::SOLID ) {
		L += sprintf( &pcBuff[L], PCL_GL_LINE_SOLID) ;
	} ;

	//Sale a modo PCL
	strcat(&pcBuff[L], PCL_GL_EXIT) ;
	L += strlen(PCL_GL_EXIT) ;	//No printf por %
	
	//Vuelca resultados al buffer

	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPCL_MAX_SIZE,L+1) ;

	PageBuffer->Append(pcBuff,L) ;
}

void ClENG_DvcPCL::FillZone( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height ) 
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4char_t* pcBuff=TaskEnv->Dvc_g_pcBuff;
	M4ClBuffer *PageBuffer = m_poLayoutPages[ai_iPage].m_poActualBuff;
	size_t L ;
	m4int16_t Shading ;

	RoundPLU_t X1, X2, Y1, Y2 ;

	
	//Se añade el Offset del limite fisico de la pagina por la izquierda

	X1 = ClMetric::DvcToRoundPLU( ai_X					+ GetOffsetX(ai_iPage)  ) ;
	X2 = ClMetric::DvcToRoundPLU( (ai_X+ai_Width-1)		+ GetOffsetX(ai_iPage)  ) ;

	// AntonioADN. Bugs 162271 y 163052
	// Se modifica el cálculo de Y1 e Y2 según el nuevo sistema de referencia
	// coincidente con PCL (ver comentario en el comentario PCL_GL_PAGE_INIT)
	Y1 = ClMetric::DvcToRoundPLU( ai_Y + GetOffsetY(ai_iPage) + PCL_Y_OFFSET_TOP ) ;
	Y2 = ClMetric::DvcToRoundPLU( ai_Y + ai_Height - 1 + GetOffsetY(ai_iPage) + PCL_Y_OFFSET_TOP ) ;


	//Entra en modo GL

	strcpy( pcBuff, PCL_GL_ENTER) ;
	L = strlen(pcBuff) ;		//No printf por %

	if ( IsColourOn() ) {
		
		//En modo color, ultimo SelectColour puso pen+color, solo falta seleccionar el Pen.
	
		L += sprintf( &pcBuff[L], PCL_GL_SELECT_PEN, 
			(m4int16_t)m_oFGPalette.GetIndex(GetLastColour(ai_iPage)) ) ;
	
		//Ahora rellena el rectangulo

		L += sprintf( &pcBuff[L], PCL_GL_FILL_RECT, (long)X1, (long)Y1, (long)X2, (long)Y2 ) ;
	} else {

		//En modo BW, nunca seleccionamos otro Pen. Hace una reduccion de color a shading

		Shading = m_oFGPalette.GetColourById( GetLastColour(ai_iPage) )->GetReductionIndexTo256() ;

		//Valor shading de peso 0 a 255. Reduce de 0 a 100. Luego 0 es negro, 100 blanco,
		//inversion de valores.

		Shading = 100 - ( (Shading*100)/255 ) ; 

		//PARCHEADO !!!!!!!
		Shading = 0 ;

		L += sprintf( &pcBuff[L], PCL_GL_SHADOW_RECT, 
			(int)Shading, (long)X1, (long)Y1, (long)X2, (long)Y2 ) ;
	} ;

	//Levanta el Pen 
	L += sprintf( &pcBuff[L], PCL_GL_PEN_UP ) ; 
	
	//Sale a modo PCL
	strcat(&pcBuff[L], PCL_GL_EXIT) ;
	L += strlen(PCL_GL_EXIT) ;	//No printf por %
	
	//Vuelca resultados al buffer

	ClMIT_Str::OverStringValid(pcBuff,StENG_TaskEnv::DvcPCL_MAX_SIZE,L+1) ;

	PageBuffer->Append(pcBuff,L) ;
}


