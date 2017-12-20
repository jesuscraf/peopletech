//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             PSCWidthsExtractor.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:20/10/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//
//
//==============================================================================

#ifndef __PSC_WIDTHS_EXTRACTOR__H__
#define __PSC_WIDTHS_EXTRACTOR__H__

#include "m4stl.hpp"

typedef vector <m4int32_t> RFN_EncodingWidths_t ;	
typedef map <string, m4int32_t, less<string> > RFN_GlyphWidths_t ;	

/*
Lee del AFM el GlyphWidths; es decir, tendremos un mapa con clave GlyphName y con
valor Width.
Lee del PFA el EncodingWidths,  es decir, la posicion n del vector corresponde con
la posicon n del EncodingBlock, y su contenido es l Width (se obtiene del mapa GlyphName,
ya que el EncodingBlock contiene precisamente GlyphNames).
*/
m4bool_t WidthsExtractor(
	const m4char_t *ai_pcPFAFile, const m4char_t *ai_pcAFMFile,
	RFN_EncodingWidths_t *ao_EncodingWidths, RFN_GlyphWidths_t *ao_GlyphWidths
) ;

#endif

