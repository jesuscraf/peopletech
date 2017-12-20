
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4imdtprestool.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                16-03-2004
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     
//
//
// Definition:
//
//    Este módulo contiene el interfaz de la tool de presentaciones
//
//
//==============================================================================



#include "m4mdtransfer_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"


#ifndef __M4IMDTPRESTOOL_HPP__
#define __M4IMDTPRESTOOL_HPP__




//=================================================================================
//
// IMDTPresentationTool
//
// Interfaz para la herramienta que transforma presentaciones
//
//=================================================================================

class	M4_DECL_M4MDTRANSFER	IMDTPresentationTool
{

public:

	//=================================================================================
	//
	// Genera una presentación en texto a partir de sus campos en base de datos
	//
	// @param ai_pccPackage
	// @param ai_pccBrazilian
	// @param ai_pccEnglish
	// @param ai_pccSpanish
	// @param ai_pccFrench
	// @param ai_pccGeneric
	// @param ai_pccGerman
	// @param ai_pccItalian
	//
	//			Parámetros de entrada con las cadenas que representan una presentación
	//			en su formato de base de datos. Una para el paquete principal y una
	//			para cada idioma
	//
	// @param ai_iPackageSize
	// @param ai_iBrazilianSize
	// @param ai_iEnglishSize
	// @param ai_iSpanishSize
	// @param ai_iFrenchSize
	// @param ai_iGenericSize
	// @param ai_iGermanSize
	// @param ai_iItalianSize
	//
	//			Parámetros de entrada con los tamaños de las cadenas que representan
	//			una presentación en su formato de base de datos. Uno para el paquete
	//			principal y uno para cada idioma
	//
	// @param ao_rpcObl
	//
	//			Parámetro de salida con la cadena que representa la presentación en
	//			formato texto. Esta memoria debe ser liberada por el llamador de
	//			la función
	//
	// @param ao_riOblSize
	//
	//			Parámetro de salida con el tamaño de la cadena que representa la
	//			presentación en	formato texto
	//
	// @param ai_pccPresentation
	//
	//			Parámetro de entrada con el identificador de la presentación
	//			que se está procesando. Se usa en mensajes de error
	//
	// @param ai_vConfigIds
	//
	//			Parámetro de entrada con los identificadores de las configuraciones
	//			activadas. Se usa para generar los idiomas correspondientes en el OBL.
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	virtual	m4return_t	Presentation2OBL
						(
							m4pcchar_t	ai_pccPackage,		size_t	ai_iPackageSize,
							m4pcchar_t	ai_pccBrazilian,	size_t	ai_iBrazilianSize,
							m4pcchar_t	ai_pccEnglish,		size_t	ai_iEnglishSize,
							m4pcchar_t	ai_pccSpanish,		size_t	ai_iSpanishSize,
							m4pcchar_t	ai_pccFrench,		size_t	ai_iFrenchSize,
							m4pcchar_t	ai_pccGeneric,		size_t	ai_iGenericSize,
							m4pcchar_t	ai_pccGerman,		size_t	ai_iGermanSize,
							m4pcchar_t	ai_pccItalian,		size_t	ai_iItalianSize,
							m4pchar_t&	ao_rpcObl,			size_t&	ao_riOblSize,
							m4pcchar_t	ai_pccPresentation,
							vector<string> ai_vEnableConfigIds
						) = 0 ;

	//=================================================================================
	// Genera los campos de base de datos de una presentación a partir de su texto
	//
	// @param ai_pccObl
	//
	//			Parámetro de entrada con la cadena que representa la presentación en
	//			formato texto.
	//
	// @param ai_iOblSize
	//
	//			Parámetro de entrada con el tamalo de la cadena que representa la
	//			presentación en	formato texto
	//
	// @param ao_rpcPackage
	// @param ao_rpcBrazilian
	// @param ao_rpcEnglish
	// @param ao_rpcSpanish
	// @param ao_rpcFrench
	// @param ao_rpcGeneric
	// @param ao_rpcGerman
	// @param ao_rpcItalian
	//
	//			Parámetros de salida con las cadenas que representan una presentación
	//			en su formato de base de datos. Una para el paquete principal y una
	//			para cada idioma. Esta memoria debe ser liberada por el llamador de
	//			la función
	//
	// @param ao_riPackageSize
	// @param ao_riBrazilianSize
	// @param ao_riEnglishSize
	// @param ao_riSpanishSize
	// @param ao_riFrenchSize
	// @param ao_riGenericSize
	// @param ao_riGermanSize
	// @param ao_riItalianSize
	//
	//			Parámetros de salida con los tamaños de las cadenas que representan
	//			una presentación en su formato de base de datos. Uno para el paquete
	//			principal y uno para cada idioma
	//
	// @param ai_pccPresentation
	//
	//			Parámetro de entrada con el identificador de la presentación
	//			que se está procesando. Se usa en mensajes de error
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	virtual	m4return_t	OBL2Presentation
						(
							m4pcchar_t	ai_pccObl,			size_t	ai_iOblSize,
							m4pchar_t&	ao_rpcPackage,		size_t&	ao_riPackageSize,
							m4pchar_t&	ao_rpcBrazilian,	size_t&	ao_riBrazilianSize,
							m4pchar_t&	ao_rpcEnglish,		size_t&	ao_riEnglishSize,
							m4pchar_t&	ao_rpcSpanish,		size_t&	ao_riSpanishSize,
							m4pchar_t&	ao_rpcFrench,		size_t&	ao_riFrenchSize,
							m4pchar_t&	ao_rpcGeneric,		size_t&	ao_riGenericSize,
							m4pchar_t&	ao_rpcGerman,		size_t&	ao_riGermanSize,
							m4pchar_t&	ao_rpcItalian,		size_t&	ao_riItalianSize,
							m4pcchar_t	ai_pccPresentation,
							vector<string> ai_vEnableConfigIds
						) = 0 ;

} ;



#endif


