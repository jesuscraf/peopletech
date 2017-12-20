
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtprestool.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                16-03-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     
//
//
// Definition:
//
//    Este módulo contiene la declaración de la clase de la tool de
//    presentaciones
//
//==============================================================================



#include "m4imdtprestool.hpp"
#include "m4mdtdef.hpp"
#include "wtypes.h"



#ifndef __M4MDTPRESTOOL_HPP__
#define __M4MDTPRESTOOL_HPP__



//=================================================================================
// Declaraciones por adelantado
//=================================================================================
interface Iveranle;
interface ITraductProp;
interface IPersistObject;
interface IConvertPropBag;
interface IOBLParser;
interface ICPropBagDef;



//=================================================================================
//
// ClMDTPresentationTool
//
// Clase para la herramienta que transforma presentaciones
//
//=================================================================================

class	ClMDTPresentationTool : public IMDTPresentationTool
{

public:

//=================================================================================
// Funciones de inicialización
//=================================================================================

		ClMDTPresentationTool( void ) ;
		virtual ~ClMDTPresentationTool( void ) ;


//=================================================================================
// Funciones del interfaz
//=================================================================================

	//=================================================================================
	// Genera una presentación en texto a partir de sus campos en base de datos
	// Los parámetros están estecificados en el interfaz
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
						) ;

	//=================================================================================
	// Genera los campos de base de datos de una presentación a partir de su texto
	// Los parámetros están estecificados en el interfaz
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
						) ;

protected:


//=================================================================================
// Funciones auxiliares
//=================================================================================

	//=================================================================================
	//
	// Crea un nuevo interfaz del tipo pedido
	//
	// @param ai_ClassId
	//
	//		Parametro de entrada que identifica la clase de la que se pide
	//			el interfaz
	//
	// @param ai_InterfaceId
	//
	//		Parametro de entrada que identifica el interfaz pedido
	//
	// @param ai_pccInterface
	//
	//		Parámetro de entrada con el nombre del interfaz pedido
	//		Se usa en mensajes de error
	//
	// @param ai_pccPresentation
	//
	//		Parámetro de entrada con el identificador de la presentación
	//			que se está procesando. Se usa en mensajes de error
	//
	// @return 
	//
	//		El interfaz pedido o NULL si ha habido algún error
	//		Como se devuelve un puntero a void la función llamadora debe
	//			realizar el casting al tipo concreto
	//
	//=================================================================================
	void*	_NewInterface( REFCLSID ai_ClassId, REFIID ai_InterfaceId, m4pcchar_t ai_pccInterface, m4pcchar_t ai_pccPresentation ) ;


	//=================================================================================
	//
	// Devuelve el miembro Iveranle para el metalenguaje pedido
	// Lo crea e inicializa si es necesario
	//
	// @param ao_rpVeranle
	//
	//		Referencia al miembro Iveranle que se debe inicializar
	//
	// @param ai_lMetaLanguage
	//
	//		Metalenguaje a emplear
	//
	// @param ai_pccPresentation
	//
	//		Parámetro de entrada con el identificador de la presentación
	//			que se está procesando. Se usa en mensajes de error
	//
	// @return
	//
	//		El miembro Iveranle inicializado o NULL si ha habido algún error
	//
	//=================================================================================
	Iveranle*	_GetAnle( Iveranle* &ao_rpVeranle, long ai_lMetaLanguage, m4pcchar_t ai_pccPresentation ) ;


	//=================================================================================
	//
	// Devuelve el miembro Iveranle para el metalenguaje 202
	// Delegan en la función _GetAnle
	//
	// @param ai_pccPresentation
	//
	//		Parámetro de entrada con el identificador de la presentación
	//			que se está procesando. Se usa en mensajes de error
	//
	// @return
	//
	//		El miembro Iveranle inicializado o NULL si ha habido algún error
	//
	//=================================================================================
	Iveranle*	_GetAnle202( m4pcchar_t ai_pccPresentation ) ;


	//=================================================================================
	//
	// Devuelve el miembro Iveranle para el metalenguaje 203
	// Delegan en la función _GetAnle
	//
	// @param ai_pccPresentation
	//
	//		Parámetro de entrada con el identificador de la presentación
	//			que se está procesando. Se usa en mensajes de error
	//
	// @return
	//
	//		El miembro Iveranle inicializado o NULL si ha habido algún error
	//
	//=================================================================================
	Iveranle*	_GetAnle203( m4pcchar_t ai_pccPresentation ) ;


	//=================================================================================
	//
	// Devuelve el miembro ITraductProp. Lo crea e inicializa si es necesario
	//
	// @param ai_pccPresentation
	//
	//		Parámetro de entrada con el identificador de la presentación
	//			que se está procesando. Se usa en mensajes de error
	//
	// @return
	//
	//		El miembro ITraductProp inicializado o NULL si ha habido algún error
	//
	//=================================================================================
	ITraductProp*	_GetTraductProp( m4pcchar_t ai_pccPresentation ) ;


	//=================================================================================
	//
	// Devuelve el miembro IPersistObject. Lo crea e inicializa si es necesario
	//
	// @param ai_pccPresentation
	//
	//		Parámetro de entrada con el identificador de la presentación
	//			que se está procesando. Se usa en mensajes de error
	//
	// @return
	//
	//		El miembro IPersistObject inicializado o NULL si ha habido algún error
	//
	//=================================================================================
	IPersistObject*	_GetPersist( m4pcchar_t ai_pccPresentation ) ;


	//=================================================================================
	//
	// Devuelve el miembro IConvertPropBag. Lo crea e inicializa si es necesario
	//
	// @param ai_pccPresentation
	//
	//		Parámetro de entrada con el identificador de la presentación
	//			que se está procesando. Se usa en mensajes de error
	//
	// @return
	//
	//		El miembro IConvertPropBag inicializado o NULL si ha habido algún error
	//
	//=================================================================================
	IConvertPropBag*	_GetConverter( m4pcchar_t ai_pccPresentation ) ;


	//=================================================================================
	//
	// Devuelve el miembro IOBLParser. Lo crea e inicializa si es necesario
	//
	// @param ai_pccPresentation
	//
	//		Parámetro de entrada con el identificador de la presentación
	//			que se está procesando. Se usa en mensajes de error
	//
	// @return
	//
	//		El miembro IOBLParser inicializado o NULL si ha habido algún error
	//
	//=================================================================================
	IOBLParser*	_GetParser( m4pcchar_t ai_pccPresentation ) ;


	//=================================================================================
	//
	// Obtiene la presentación en formato bolsa de propiedades a partir de la
	// presentatción en formato texto
	// Tienen en cuenta el metalenguaje correcto
	//
	// @param ai_pcsOBL
	//
	//		Parámetro de entrada con la presentatción en formato texto
	//
	// @param ao_rpIPBDef
	//
	//		Parámetro de salida con la presentación en formato bolsa de propiedades
	//
	// @param ai_pccPresentation
	//
	//		Parámetro de entrada con el identificador de la presentación
	//			que se está procesando. Se usa en mensajes de error
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_OBL2PropBagDef( const BSTR ai_pcsOBL, ICPropBagDef* &ao_rpIPBDef, m4pcchar_t ai_pccPresentation ) ;


//=================================================================================
// Miembros
//=================================================================================

	//=================================================================================
	// Indicador de si se ha inicializado la librería COM
	//=================================================================================
	m4bool_t		m_bComInitialized ;

	//=================================================================================
	// Miembros auxiliares de las librerías m4anle y m4prop necesarios para realizar
	// las transformaciones de las presentatciones
	//=================================================================================
	Iveranle		*m_pIAnle202 ;
	Iveranle		*m_pIAnle203 ;
	ITraductProp	*m_pITraductProp ;
	IPersistObject	*m_pIPersist ;
	IConvertPropBag	*m_pIConverter ;
	IOBLParser		*m_pIParser ;

	//=================================================================================
	// Array de idiomas
	//=================================================================================
	BSTR			m_asLanguages[ M4MDT_LANGUAGE_NUMBER ] ;

} ;


#endif



