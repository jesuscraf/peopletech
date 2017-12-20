//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             fmetrics.h
// Project:			 fmetrics.dll
// Author:           Meta Software M.S. , S.A
// Date:			 13/05/1999
// Language:         C++
// Operating System: WINDOWS. 
// Design Document:  
//
//	Propietary:		Manuel Lazcano Perez
//	Modifications:	Who && Date
//
// Definition:
//
//==============================================================================

#ifndef __FMETRICS_H__
#define __FMETRICS_H__

#include "m4types.hpp"
#include "m4fmetrics_dll.hpp"

class m4VariantType ;

#ifdef _UNIX
#define WINAPI
#endif



extern "C" M4_DECL_M4FMETRICS m4return_t StartSessionLN4	(m4VariantType * ai_poParam, m4int32_t ai_iNParam, m4VariantType * ao_poReturn, void * ai_poDataContext) ;
extern "C" M4_DECL_M4FMETRICS m4return_t KillSessionLN4		(m4VariantType * ai_poParam, m4int32_t ai_iNParam, m4VariantType * ao_poReturn, void * ai_poDataContext) ;
extern "C" M4_DECL_M4FMETRICS m4return_t WindowsMetricsLN4	(m4VariantType * ai_poParam, m4int32_t ai_iNParam, m4VariantType * ao_poReturn, void * ai_poDataContext) ;



//Inicia una nueva sesion de la DLL de metricas. Devuelve un HANDLE de acceso
// que debe utilizarse posteriormente para las siguientes llamadas.
extern "C" M4_DECL_M4FMETRICS m4int16_t WINAPI FONT_M_StartSession(m4uint32_t* ao_TaskId);

//Termina una sesion iniciada de la DLL de metricas. Se le pasa el HANDLE de
// acceso obtenido al Iniciar la sesion.
extern "C" M4_DECL_M4FMETRICS m4int16_t WINAPI FONT_M_KillSession(m4uint32_t ai_TaskId);

//Añade una nueva funcion a la DBFont para ser analizada
extern "C" M4_DECL_M4FMETRICS m4int16_t WINAPI FONT_M_AddFont(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
									  const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
									  m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic);

//Las siguientes funciones que se exportan son las de tratamiento puro de las metricas de las
// fuentes en si. Se subdividen en dos tipos bien diferenciados. Las que llevan el sufijo
// ById, son aquellas que devuelven una caracteristica determinada de una fuente pasandole
// solo el ID de la fuente, habiendo hecho previamente un AddFont. Las demas, obtienen lo 
// mismo solo que hay que pasarle todas las caracteristicas de la fuente.

//Obtiene todos las metricas necesarias para Windows, Anchura, Altura y TypeFace
extern "C" M4_DECL_M4FMETRICS m4int16_t WINAPI FONT_M_WindowsMetricsById(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, m4char_t *ao_pcFaceName,
											     m4int16_t * ao_NameSize, m4int32_t* ao_WindowsHeight, m4int32_t *ao_MaxDesignHeight, 
											     m4int32_t* ao_MaxDesignWidth);

extern "C" M4_DECL_M4FMETRICS m4int16_t WINAPI FONT_M_WindowsMetrics(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
										     const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
									         m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic,
										     m4char_t *ao_pcFaceName, m4int16_t * ao_NameSize, m4int32_t* ao_WindowsHeight, 
										     m4int32_t *ao_MaxDesignHeight, m4int32_t* ao_MaxDesignWidth);

//Obtienen la anchura maxima de diseño de una fuente
extern "C" M4_DECL_M4FMETRICS m4int32_t WINAPI FONT_M_DesignMaxWidthById(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId);

extern "C" M4_DECL_M4FMETRICS m4int32_t WINAPI FONT_M_DesignMaxWidth(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
											  const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
											  m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic);

//Obtienen la anchura maxima de una fuente
extern "C" M4_DECL_M4FMETRICS m4int32_t WINAPI FONT_M_MaxWidthById(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId);

extern "C" M4_DECL_M4FMETRICS m4int32_t WINAPI FONT_M_MaxWidth(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
									    const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
									    m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic);

//Obtienen la altura de diseño de una fuente
extern "C" M4_DECL_M4FMETRICS m4int32_t WINAPI FONT_M_DesignHeightById(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId);

extern "C" M4_DECL_M4FMETRICS m4int32_t WINAPI FONT_M_DesignHeight(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
											const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
											m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic);

//Obtienen la altura de una fuente
extern "C" M4_DECL_M4FMETRICS m4int32_t WINAPI FONT_M_HeightById(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId);

extern "C" M4_DECL_M4FMETRICS m4int32_t WINAPI FONT_M_Height(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
									  const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
									  m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic);

//Obtienen la anchura anchura de un caracter dado para una fuente determinada
extern "C" M4_DECL_M4FMETRICS m4int32_t WINAPI FONT_M_WidthCharById(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, m4char_t ai_Char);

extern "C" M4_DECL_M4FMETRICS m4int32_t WINAPI FONT_M_WidthChar(m4uint32_t ai_TaskId, m4uint32_t ai_uiFontId, const m4char_t* ai_pcFontName,
									     const m4char_t *ai_pcRFNFileName, m4bool_t ai_bIsAbsolute, m4double_t ai_MapHeight, 
									     m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, m4bool_t ai_bBold, m4bool_t ai_bItalic,
									     m4char_t ai_Char);




#endif
