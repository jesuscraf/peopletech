//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvcpsc.h
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

#ifndef __CLDVCPSC__H__
#define __CLDVCPSC__H__

#include "clstr.h"

#include "cldvcfi.h"
#include "pageprn.h"
#include "clmetric.h"

//======================================================================ClENG_DvcPSC

class ClENG_DvcPSC : public ClENG_DvcFile {

MIT_PUBLIC:

	enum PAGE_RESOLUTION { P_300_DPI=300, P_600_DPI=600 } ;

	//---------------------- Inicializacion ----------------------------

	ClENG_DvcPSC();
	virtual ~ClENG_DvcPSC() { ClENG_DvcPSC::End() ; }

	virtual size_t GetSizeof() { return sizeof(ClENG_DvcPSC) ; }

	//Inicializacion del Device PostScript
	// PreLoadOnDevice=TRUE - DelegatedRasterPrint=FALSE

	// El device es:
	//		PRELOAD_ON_DEVICE	= TRUE
	//		COLOUR_SUPPORTED	= TRUE
	//		COLOUR_ACTIVATED	= TRUE 
	//		RASTER_PRELOADED	= TRUE
	//		HARD_DITHERING		= TRUE 
	//		HARD_SCALE			= TRUE 
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

	ClENG_PagePrinter *m_poPagePrinter ; //Array dinámico de páginas para la impresora
	m4int16_t m_iNumCopies ;   
	size_t *m_poPageBuffOffset ;	//Offset de %PAGE en buffer
    
	enum { N_RESOLUTIONS=6};

	static struct stGraphicResolution {			
		m4int32_t Dpi ;
		m4int16_t ScaleFactor300, ScaleFactor600 ;	//Si ScaleFactor300=0, no disponible
		//m4char_t *StrSetRes ;
	} const GraphicResolution[N_RESOLUTIONS] ;	//Ordenar de < dpi a >dpi

	PAGE_RESOLUTION m_MaxResolution ;
	StENG_DvcParams::DUPLEX_MODE m_Duplex ;
	StENG_DvcParams::TUMBLE_MODE  m_Tumble ;
	ClFONT_M_DBFont *m_poDBFont;    //Simple link

	//----------------------- Sobrecarga de metodos de flujo de salida -------------

MIT_PUBLIC:

	m4bool_t PreReset();

	void FixPreload();

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

	void SelectFontDevice(m4int16_t ai_iPage, ClFONT_M_Font* ai_poFont);


	//----------------------- Metodos propios del Dvc PostScript ---------------------

	//Prepara la cadena a imprimir para dispositivos PostScript, 
	//  escapando los caracteres especiales.
	//También, para SoftFonts tipo Composite, inserta si procede los caracteres Shitf-In
	//	ShiftOut si encuentra caracteres a trasladar.
	//Trata escapes y conversiones ISO

	void PrepareForPostScript(ClFONT_M_Font *ai_Font, ClMIT_Str *ao_poStr, size_t *ai_szNChar, m4bool_t ai_bRawText);

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
	
	virtual void GetScaleData( 
		DvcUnit_t ai_SizeXDvc,			DvcUnit_t ai_SizeYDvc,			//Area a imprimir.
		m4uint32_t ai_iRasterXMap,	m4uint32_t ai_iRasterYMap,					//Size bitmap
		m4uint32_t *ao_piRasterXDvc, m4uint32_t *ao_piRasterYDvc ) 
	{
		GetScaleData( ai_SizeXDvc, ai_SizeYDvc, ai_iRasterXMap, ai_iRasterYMap,
			ao_piRasterXDvc, ao_piRasterYDvc, NULL ) ;
	}

	virtual void PrintRasterMap(
		m4int16_t ai_iPage,
		DvcUnit_t ai_X,				DvcUnit_t ai_Y,				//Coordenadas de comienzo.
		DvcUnit_t ai_SizeX,			DvcUnit_t ai_SizeY,			//Area a imprimir.
		ClENG_RasterMap * ai_poMap, m4char_t *ai_pcHyperlink,
		m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET) ;

MIT_PRIVATE:

	void GetScaleData( 
		DvcUnit_t ai_SizeXDvc,			DvcUnit_t ai_SizeYDvc,			
		m4uint32_t ai_iRasterXMap,	m4uint32_t ai_iRasterYMap,					
		m4uint32_t *ao_piRasterXDvc, m4uint32_t *ao_piRasterYDvc, 
		m4int16_t *ao_piGResIndex ) ;

MIT_PUBLIC:

	virtual void SelectColourDevice( m4int16_t ai_iPage, m4uint32_t ai_uiIdColour );

MIT_PRIVATE:

	size_t SetActivePalette ( ClENG_Palette* ai_poPalette, m4char_t* ai_pcBuff, size_t ai_szFrom, size_t ai_szMaxSize );
};

#endif

