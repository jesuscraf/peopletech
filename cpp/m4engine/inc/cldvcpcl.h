//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             ClDevPCL.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:21/07/1997
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef __CLDEVPCL__H__
#define __CLDEVPCL__H__

#include "clstr.h"

#include "cldvcfi.h"
#include "pageprn.h"
#include "clmetric.h"

//==============================================================================ClENG_DvcPCL

/*
  Clase FINAL. No derivar más.
*/

class ClENG_DvcPCL : public ClENG_DvcFile {	

MIT_PUBLIC:
	
	enum PAGE_RESOLUTION { P_300_DPI=300, P_600_DPI=600 } ;
	enum OTHERS { BEGIN_ID_MACRO_RESERVED=32000 } ;

MIT_PRIVATE:

	ClENG_PagePrinter * m_poPagePrinter ;	//Array dinámico
	m4int16_t m_iNumCopies;

	ClFONT_M_DBFont *m_poDBFont;    //Simple link

	enum { N_RESOLUTIONS=6};

	static struct stGraphicResolution {			
		m4int32_t Dpi ;
		m4int16_t ScaleFactor300, ScaleFactor600 ;	//Si ScaleFactor300=0, no disponible
		m4char_t *StrSetRes ;
	} const GraphicResolution[N_RESOLUTIONS] ;	//Ordenar de < dpi a >dpi

	PAGE_RESOLUTION m_MaxResolution ;

	StENG_DvcParams::DUPLEX_MODE  m_Duplex ;
	StENG_DvcParams::TUMBLE_MODE  m_Tumble ;

	// Especifica el código de la bandeja de entrada. Este valor tiene sentido
	// cuando el tipo de bandeja es USER_DEFINED.
	m4int32_t  m_iPaperSourceCode;
	
MIT_PUBLIC:

	// ---------------------- Inicialización -------------------------------------	


	ClENG_DvcPCL() ;
	virtual ~ClENG_DvcPCL() { ClENG_DvcPCL::End() ; }

	virtual size_t GetSizeof() { return sizeof(ClENG_DvcPCL) ; }

	// El device es:
	//		PRELOAD_ON_DEVICE	= TRUE
	//		COLOUR_SUPPORTED	= TRUE si COLOUR_ACTIVATED, FALSE si no
	//		COLOUR_ACTIVATED	= Parametrizable, TRUE si ai_poFGPalette no es NULL
	//		RASTER_PRELOADED	= TRUE
	//		HARD_DITHERING		= TRUE si COLOUR_SUPPORTED && COLOUR_ACTIVATED, FALSE si no
	//		HARD_SCALE			= TRUE si COLOUR_SUPPORTED && COLOUR_ACTIVATED, FALSE si no
	
	//  En modo duplex, cada pagina de layout va a un fichero distinto, en modo normal
	//todo va a un fichero. David: La gestión de nombres de fichero sube a DvcFile

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

		//Param especificos de device

		ClFONT_M_DBFont* ai_poDBFont, 
		m4bool_t ai_bIsColourOn, 
		m4int16_t ai_iNumCopies,
		PAGE_RESOLUTION ai_MaxResolution, 
		StENG_DvcParams::TUMBLE_MODE  ai_Tumble,


		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;


	// Este método se llamará una vez por cada elemento del array de páginas. 
	// SizeX y SizeY solo son utiles si PageSize es personal.

	m4bool_t InitPage(	m4int16_t ai_iIdPage,
					DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY, 
					size_t ai_szBlockBuffer, 
					ClENG_PagePrinter::ORIENTATION   ai_Orientation,
					ClENG_PagePrinter::PAPER_SIZE     ai_PageSize,
					ClENG_PagePrinter::PAPER_SOURCE  ai_PaperSource,
					DvcUnit_t ai_SizeX,   DvcUnit_t ai_SizeY,
					ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	virtual void End();

	//---------------------------Sobrecargas Flow---------------------------------

	// Nota: estan reservados los Id's de macros desde el BEGIN_ID_MACRO_RESERVED, 
	//	incluido, hasta el final (osea, el 32767). Se utilizan para optimizar, de 
	//	forma que todo lo que sea preload y optimizable de la pagina layout n, va 
	//	dentro de la macro BEGIN_ID_MACRO_RESERVED+n.

	m4bool_t PreReset();

	// Entre otras cosas, las macros son borradas.

	void  PostReset();

	// Si SwBreak, llama a DvcFile::NewBreak que gestione la creación de nuevos ficheros
	
	m4bool_t RefreshLayout();

	m4bool_t FlushLayout();
	
	// ---------------------------------Sobrecarga metodos Action-----------------			

	void EOP(m4int16_t ai_iPage) ;
	void SelectFontDevice(m4int16_t ai_iPage, ClFONT_M_Font * ai_poFont);

	// X,Y se interpretan como BOTTOM(base line)-left
	virtual void PrintNChar( m4int16_t ai_iPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, 
		m4pchar_t ai_pcStr, size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, 
		m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET, 
		ClENG_OutDevice::StPIParams *ai_pstPI=NULL) ; 	
	
	void PrintStr(m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, m4pchar_t ai_pcStr, 
		m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, 
		m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET ) ;
	
	// ********** Impresiones Pendientes (Pi)
	void  PrintNCharPi(ClMIT_Str  *ai_poIdPi,m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
		 size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, 
		 m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET);

	void ResolvePi(ClMIT_Str  *ai_poIdPi,m4pchar_t ai_pcStr);

	// ********** Impresion Graficos
	
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

	// ********** Color foreground

	virtual void SelectColourDevice( m4int16_t ai_iPage, m4uint32_t ai_uiIdColour ) ;

	// ********** Lineas y fondos

	virtual void PrintLine( 
		m4int16_t ai_iPage, 
		DvcUnit_t ai_X1, DvcUnit_t ai_Y1, DvcUnit_t ai_X2, DvcUnit_t ai_Y2,
		DvcUnit_t ai_LineWidth, STYLE ai_Style ) ;

	virtual void PrintFrame( 
		m4int16_t ai_iPage, 
		DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
		DvcUnit_t ai_LineWidth, STYLE ai_Style ) ;

	virtual void FillZone( 
		m4int16_t ai_iPage, 
		DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height ) ;

	// Código de la bandeja de impresión. En PCL, el usuario puede definir diferentes 
	// códigos para las bandejas, distintos de los de por defecto (manual, lower, upper).
	void SetPaperSourceCode(m4int32_t ai_iPaperSourceCode) { m_iPaperSourceCode = ai_iPaperSourceCode;}
	m4int32_t   GetPaperSourceCode() { return m_iPaperSourceCode; } 

MIT_PRIVATE:

	// ********** Auxiliares

	//CONCATENA la secuencia PCL de programacion de la paleta en el buffer pasado, comenzando
	//	en la posicion ai_szFrom, y comprobando luego que no sobreescribe.
	//Retorna el tamaño total resultante de la cadena (puede no necesitarse, este metodo no añade
	//	informacion con posibles END_STR falsos).

	size_t SetActivePalette( 
		ClENG_Palette *ai_poPalette, m4char_t *ai_pcBuff, size_t ai_szFrom, size_t ai_szMaxSize 
	) ;

	// ---------------------------------------------------------------------------			

} ;

#endif

