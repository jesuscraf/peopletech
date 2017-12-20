//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvcrvw.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:24/11/97
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
#ifndef __CLDVCRVW__H__
#define __CLDVCRVW__H__

#include "clstr.h"

#include "sysinfo.h"
#include "cldvcfi.h"
#include "pageprn.h"

class ClENG_PhysicalFile ;

//==============================================================================ClENG_DvcRVW

/*
  Clase FINAL. No derivar más.
  Existe un *.doc explicando el formato del fichero *.RVW
*/

class ClENG_DvcRVW : public ClENG_DvcFile {	

MIT_PUBLIC:
	
MIT_PRIVATE:

	m4uint32_t m_uiOffActBOB ;	//Offset comienzo BOB actual (-1 si aun no se volco primer BOB)

	ClENG_PagePrinter * m_poPagePrinter ;	//Array dinámico

MIT_PUBLIC:

	// ---------------------- Inicialización -------------------------------------	


	ClENG_DvcRVW() ;
	virtual ~ClENG_DvcRVW() { ClENG_DvcRVW::End() ; }

	virtual size_t GetSizeof() { return sizeof(ClENG_DvcRVW) ; }


	// El device es:
	//		PRELOAD_ON_DEVICE	= TRUE
	//		COLOUR_SUPPORTED	= TRUE
	//		COLOUR_ACTIVATED	= TRUE
	//		RASTER_PRELOADED	= FALSE
	//		HARD_DITHERING		= FALSE
	//		HARD_SCALE			= FALSE
	// Los ficheros graficos se incrustan tal cual en el fichero RVW.
	// Si ai_bFirstFont=TRUE, se coje el Id de la primera font de la DbFont, si no la
	//	indicada
	
	// No hay duplex: la salida va a un unico fichero *.RVW.
	// Al path se le añadira automaticamente la extension RVW.

	m4bool_t Init(	
		//Campos para ClENG_OutDevice::Init

		m4int16_t ai_iNumPages,
		ClENG_Palette *ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		StFileParams *ai_FileParams, m4bool_t ai_bDelete,
		DECLARE_EXTERNAL_FILE ai_DeclareExtFile,
		m4bool_t ai_bCreateDir,
		ClENG_ReportOuputRepository::PRESERVE_MODE ai_Preserve, 
		m4char_t *ai_pcToPrinter, 

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;
		

	// Este método se llamará una vez por cada elemento del array de páginas. 
	// SizeX y SizeY solo son utiles si PageSize es personal.
	// Se pasa la orientacion y PageSize simplemente para calcular el alto y ancho de la
	//	pagina, que es lo unico que en ultima instancia utiliza este device.

	m4bool_t InitPage(	m4int16_t ai_iIdPage,
					DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY, 
					size_t ai_szBlockBuffer, 
					ClENG_PagePrinter::ORIENTATION   ai_Orientation,
					ClENG_PagePrinter::PAPER_SIZE     ai_PageSize,
					DvcUnit_t ai_SizeX,   DvcUnit_t ai_SizeY,
					ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	virtual void End();

	//---------------------------Sobrecargas Flow---------------------------------


	m4bool_t PreReset();
	void PostReset();

	m4bool_t FlushLayout();
	m4bool_t RefreshLayout();

	virtual void NotifyLastReset() ;

	// ---------------------------------Sobrecarga metodos Action-----------------			
	//
	// Todos estos metodos simplemente generan tags.


	// ********** Seleccion foreground color

	void SelectColourDevice( m4int16_t ai_iPage, m4uint32_t ai_uiIdColour ) ;

	// ********** Impresion texto

	virtual void SelectFontDevice(m4int16_t ai_iPage, ClFONT_M_Font * ai_poFont);

	// X,Y se interpretan como top-left
	virtual void PrintNChar( m4int16_t ai_iPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, 
		m4pchar_t ai_pcStr, size_t ai_szNChar, 
		m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, 
		m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET,
		ClENG_OutDevice::StPIParams *ai_pstPI=NULL ) ; 	


	// ********** Lineas y zonas 

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


	// ********** Impresiones Pendientes (Pi)
	void  PrintNCharPi(ClMIT_Str  *ai_poIdPi,m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
					size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, 
					m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET);

	void ResolvePi(ClMIT_Str  *ai_poIdPi,m4pchar_t ai_pcStr);


	// ********** Impresion Graficos
	
	virtual void PrintRasterFile(
		m4int16_t ai_iPage,
		DvcUnit_t ai_X,				DvcUnit_t ai_Y,
		DvcUnit_t ai_SizeX,			DvcUnit_t ai_SizeY,
		const m4char_t * ai_pcPath, m4char_t *ai_pcHyperlink,
		m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET,
		m4bool_t ai_bIsTempWebPath=M4_FALSE
	) ;

MIT_PRIVATE:

	// ---------------------------Auxiliares de proceso---------------------------			

	ClMIT_BinaryAdapter m_BAdapter ;
	
	m4int32_t m_iLastBlock ;	//Offset de comienzo del ultimo block en fichero 
	
	// Para escribir bloques

	void WBBlockHeader( 
		m4int16_t ai_iIdPage, m4char_t ai_cId, m4int32_t ai_iPrevious, m4int32_t ai_iNext ) ;
	void WFBlockHeader( 
		ClENG_PhysicalFile *ai_poFile, m4char_t ai_cId, m4int32_t ai_iPrevious, m4int32_t ai_iNext ) ;

	// Para escribir elementos simples en formato RVW al buffer actual
	// Si en WString no se pasa la Len (o es -1L), esta se calcula.

	void WBInt(     m4int16_t ai_iIdPage, m4int16_t ai_iInt) ;
	void WBLong(    m4int16_t ai_iIdPage, m4int32_t ai_iLong) ;
	void WBChar(    m4int16_t ai_iIdPage, m4char_t  ai_cChar) ;
	void WBString(  m4int16_t ai_iIdPage, const m4char_t * ai_pcStr, size_t ai_szLen=-1L) ;

	// Iguales a los anteriores, pero escriben a fichero

	void WFInt(    ClENG_PhysicalFile *ai_poFile, m4int16_t ai_iInt) ;
	void WFLong(   ClENG_PhysicalFile *ai_poFile, m4int32_t ai_iLong) ;
	void WFChar(   ClENG_PhysicalFile *ai_poFile, m4char_t  ai_cChar) ;
	void WFString( ClENG_PhysicalFile *ai_poFile, const m4char_t * ai_pcStr, size_t ai_szLen=-1L) ;

	// UNICODE REPORTS.
	// Igual que WFString pero para copiar un string wide char (UTF16).
	void WBWideCharString(m4int16_t ai_iIdPage, const m4char_t * ai_pcStr);

	//Para escribir directamente a fichero, en la posicion indicada.
	void WFLong(  ClENG_PhysicalFile *ai_poFile, m4uint32_t ai_Pos, m4int32_t ai_iLong) ;

	// Sobreescribe en buffer elementos simples ya escritos anteriormente en la 
	//	posicion From del buffer.

	void OWBLong( m4int16_t ai_iIdPage, m4uint32_t ai_uiFrom, m4int32_t ai_iLong ) ;



	// ---------------------------------------------------------------------------			

} ;

#endif

