//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvcesc.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:15/10/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	David Fernández && 23/04/98
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef __CLDVCESC__H__
#define __CLDVCESC__H__

#include "m4stl.hpp"
#include "clstr.h"

#include "cldvcfi.h"
#include "pageprn.h"
#include "clmetric.h"
#include "font.h"


class M4ClBuffer ;

//==============================================================================ClENG_DvcESC

class ClENG_DvcESC : public ClENG_DvcFile
{
MIT_PUBLIC:

	struct Data_t {
		ClMIT_Str		m_oStr ;	//Cadena a imprimir ...
		size_t			m_NBytes ;	//	... se imprimen estos bytes
		ClFONT_M_Font	*m_poFont ;	//	.... y con esta font.
		DvcUnit_t		m_X ;

		//Apunta a NULL si no es un PI, o a la PI en cuestion (link)
		//Solo tiene PI en RAM no resueltas, si se resuelven antes del FlushLayout, se
		//	pondra a NULL.

		StPi_t *m_pi ;	
		m4uint32_t m_PiHandler ;	//Si hay PI, este es su handler
	} ;

MIT_PRIVATE:

	//Lista de Data_t ordenada por eje Y

	typedef multimap<DvcUnit_t,Data_t,less<DvcUnit_t> > Map_t;

	struct ClENG_PageESC {

		ClENG_PagePrinter oPagePrinter ;

		Map_t Map ;
		
		//Tamaño de pagina ponderado en J (ESC/J). Depende de m_iLFFactor, la formula es :
		//	TamañoPagEnJ = TamañoPagEnPulgadas *  m_iLFFactor  ;
		//Si es 0, no tiene efecto, y el tamaño se calcula en base a la de diseño. Si no es
		//	0, este es el tamaño que manda.
		m4uint32_t m_JPageSize ;

	} *m_poPageESC ; //Array dinámico

	m4int16_t m_iLFFactor ;	//Divisor de ESC J (normalmente 216 ó 180)
	m4int16_t m_iXFactor ;	//Divisor de ESC $ (normalmente 60)

MIT_PUBLIC:	
	
	// ---------------------- Inicialización -------------------------------------		

	ClENG_DvcESC();
	virtual ~ClENG_DvcESC() { ClENG_DvcESC::End(); }
	virtual void End();
	virtual size_t GetSizeof() { return sizeof(ClENG_DvcESC) ; }

	// El device es:
	//		PRELOAD_ON_DEVICE	= FALSE
	//		COLOUR_SUPPORTED	= FALSE
	//		COLOUR_ACTIVATED	= FALSE
	//		RASTER_PRELOADED	= FALSE
	//		HARD_DITHERING		= FALSE
	//		HARD_SCALE			= FALSE
	// No soporta impresion de graficos	
	// El parametro ai_JPageSize tiene este formato:
	//	""				No se utiliza J
	//	"2040"			El tamaño J de la pag layout 0 y todas las demas es 2040.
	//	"1000:2040"		El tamaño J de la pag layout 0 es 1000, para la 1 las demas 2040.
	// Y asi para cualquier numero de paginas. 

	m4bool_t Init(	

		//Campos para ClENG_OutDevice::Init

		m4int16_t ai_iNumPages,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		m4bool_t ai_bForceMonoBreak,
		StFileParams *ai_FileParams, m4bool_t ai_bDelete,
		StENG_DvcParams::DUPLEX_MODE ai_Duplex, 

		DECLARE_EXTERNAL_FILE ai_DeclareExtFile,
		m4bool_t ai_bCreateDir,
		ClENG_ReportOuputRepository::PRESERVE_MODE ai_Preserve, 
		m4char_t *ai_pcToPrinter, 

		//Campos especificos de este Dvc

		StENG_DvcParams::TUMBLE_MODE  ai_Tumble,
		m4int16_t ai_iXFactor, 
		m4int16_t ai_iLFFactor,
		m4char_t *ai_JPageSize,

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;
		

	// Este método se llamará una vez por cada elemento del array de páginas. 
	m4bool_t InitPage(	m4int16_t ai_iIdPage,
					DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY, 
					size_t ai_szBlockBuffer, 
					ClENG_PagePrinter::ORIENTATION   ai_Orientation,
					ClENG_PagePrinter::PAPER_SIZE     ai_PageSize,
					ClENG_PagePrinter::PAPER_SOURCE  ai_PaperSource,
					DvcUnit_t ai_SizeX,   DvcUnit_t ai_SizeY,
					ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;


	//---------------------------Sobrecargas Flow---------------------------------

	// Datos Preload no implementables.

	m4bool_t PreReset() ;
	void	 PostReset() ;
	m4bool_t RefreshLayout();
	m4bool_t FlushLayout() ; 
	
	// ---------------------------------Sobrecarga metodos Action-----------------			

	//Es el que vuelca a buffer de verdad. Al ser un Dvc secuencial, los PrintXXX no vuelcan
	//	nada al buffer, si no al mapa ordenado por Y.
	//El EOP va volcando al buffer de forma ordenada y actualizanso el offset de las PI que se
	//	vaya encontrando.
	
	void EOP(m4int16_t ai_iPage) ;
	
	// La PrintStr de la base usa PrintNChar, por lo que no hace falta que la 
	// sobrecargue.
	// X,Y se interpretan como BOTTOM(base line)-left

	virtual void PrintNChar( m4int16_t ai_iPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, 
		m4pchar_t ai_pcStr, size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, 
		m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET, ClENG_OutDevice::StPIParams *ai_pstPI=NULL) ; 	
	
	// ********** Impresiones Pendientes (Pi)

	void  PrintNCharPi(ClMIT_Str  *ai_poIdPi,m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
				size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET);

	void ResolvePi(ClMIT_Str  *ai_poIdPi,m4pchar_t ai_pcStr );


MIT_PRIVATE:

	void SelectFontDevice( m4int16_t ai_iPage, ClFONT_M_Font *ai_poFont ) ;

	void SelectFontAux( m4int16_t ai_iPage, ClFONT_M_Font *ai_poFont, M4ClBuffer *ai_Buff ) ;

	// Crea una nueva entrada en la lista.

	void NewEntry( 
		m4int16_t ai_iPage, DvcUnit_t ai_X, DvcUnit_t ai_Y ,
		const char *ai_pcStr, size_t ai_NChar, StPi_t *ai_Pi) ;

	static inline void SkipToJ(m4uint32_t *ao_SkippedJ, m4uint32_t ai_SkipToJ, M4ClBuffer *ai_Buff ) ;

};





#endif

