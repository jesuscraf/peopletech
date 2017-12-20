//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvcpdfuni.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             12/08/2008
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Owner:			Antonio Durán
//	Modifications:	
//
// Definition:
//
//    This module manages the PDF Device (UNICODE)
//
//
//==============================================================================

#ifndef __CLDVCPDFUNI__H__
#define __CLDVCPDFUNI__H__

#include "m4stl.hpp"
#include "clstr.h"
#include "cldvcpdf.h"
#include "cldevice.h"
#include "pageprn.h"
#include "cldvcfi.h"
#include "piuni.h"
#include "itextcontext.h"
#include "itextproxy.h"


class ClENG_DvcPDFUnicode : public ClENG_DvcFile {

MIT_PROTECTED:
	// Asociacion con el repositorio de ficheros
	ClENG_PIUnicode_Manager m_oPIRepository;

MIT_PRIVATE:
	typedef map <ClMIT_Str, CliTextProxy*, less<ClMIT_Str> > TextProxyList_t ;
	TextProxyList_t m_oRealTextProxy ;
	int * m_iPDFPage;

	CliTextContext m_oiTextContext;
	CliTextProxy ** m_oiTextProxy;
	ClENG_PDF_Page *m_poPagePrinter ; //Array dinámico de páginas para la impresora
	ClFONT_M_DBFont *m_poDBFont;    //Simple link
	
	ClMIT_Str *m_poPDFFontNames;
	int * m_poPDFFontStyles;

	m4bool_t m_bIsRTL_Language; // Lenguaje de escritura de derecha a izquierda

	void Clear_iTextProxy();

MIT_PUBLIC:

	//---------------------- Inicializacion ----------------------------

	ClENG_DvcPDFUnicode();
	virtual ~ClENG_DvcPDFUnicode() { ClENG_DvcPDFUnicode::End() ; }

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
		m4bool_t ai_bForceUnicodeOutput, // Fuerza la utilización de fuentes Unicode (para textos)
		
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;

	m4bool_t InitPage( m4int16_t ai_iIdPage,
				   DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY,
				   size_t ai_szBlockBuffer,
				   ClENG_PagePrinter::ORIENTATION  ai_Orientation,
				   ClENG_PagePrinter::PAPER_SIZE    ai_PageSize,
				   ClENG_PagePrinter::PAPER_SOURCE ai_PaperSource,
				   DvcUnit_t ai_SizeX, DvcUnit_t ai_SizeY,
				   ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR );

	void End();

	void FixPreload();

	m4bool_t PreReset();

	void PostReset();

	m4bool_t RefreshLayout();
 
	m4bool_t FlushLayout();


	virtual void PrintNChar(m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
							m4pchar_t ai_pcStr, size_t ai_szNChar, 
							m4bool_t ai_bRawText,m4char_t *ai_pcHyperlink,
							m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET,
							ClENG_OutDevice::StPIParams *ai_pstPI=NULL);

	void SelectFontDevice(m4int16_t ai_iPage, ClFONT_M_Font* ai_poFont);

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

	virtual void SelectColourDevice( m4int16_t ai_iPage, m4uint32_t ai_uiIdColour );

};

#endif
