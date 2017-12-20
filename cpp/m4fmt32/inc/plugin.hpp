//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             PLUGIN.HPP
// Project:			 M4FMT32.DLL	
// Author:           Meta Software M.S. , S.A
// Date:			 20/04/99
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//	Propietary:		Manuel Lazcano Perez - 19/04/99
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines template struct for the Language Plug-Ins
//
//==============================================================================

#ifndef _PLUGIN_HPP_
#define _PLUGIN_HPP_

#include "m4define.hpp"
#include "m4types.hpp"

#include "m4win.hpp"
#include "m4stl.hpp"
#include "m4ipcs.hpp"
#include "m4loadll.hpp"

#include "clstr.h"
#include "cadena.h"
//////////////////////////////////////////////////////////////////////////
//          Estructura que gestionara los Plug-Ins de Idioma
// 
// La estructura contiene punteros a funciones que gestionan la conversion
// de cadenas de numeros a cadenas de texto
///////////////////////////////////////////////////////////////////////////
//
// FUNCIONAMIENTO BASICO DE LOS PLUG-INS:
//
// El primer paso, al llamar a la DLL de formateos, en el StartUp del Thread-
//  environment se llama al metodo LoadAllPlugIns(). Este metodo se encargara
//  de leer todos los datos de los Plug-Ins existentes. El environment mantiene
//  na lista (map), de las direcciones de memoria de los Plug-Ins, identificadas
//  por la clave numerica del idioma. Primero se leen los formatos de idioma
//  internos (SP,EN,GE,FR y US), y se guardan las direcciones de memoria de las
//  estructuras de estos idiomas. Luego se leen los Plug-Ins externos. 
// Los Plug-In externos estaran referenciados en una nueva entrada del Regitry:
//
//     SOFTWARE\\Meta4\\Mind\\3.0\\Report\\Languages --> "m4fmtpor.dll,m4fmtcat.dll"
//
//  que contendra la lista de las DLLs que se utilizaran en los formateos. Estas
//  DLLs deben implementar la estructura Plug-In y las funciones que este esqueleto
//  presenta. La estructura contiene, realmente, punteros a las funciones, por lo
//  que el Plug-In contiene las direcciones de comienzo de estas funciones. Asi pues,
//  estas DLLs tan solo deberan exportar una funcion generica AttachNewLanguage,
//  que debera devolver el identificador del idioma y la direccion del Plug-In.
//
// Cada objeto instanciado de la clase FMT_ClFormato, contiene un puntero const StPlugIn* 
//  m_StPlugInInfo. Este miembro contendra la direccion de memoria de comienzo de 
//  la estructura Plug-In de cada idioma. La direccion se leera en el metodo
//  CLM4FMT::_M4FMTSetProperty, al leer la propiedad sFCountry. En este metodo 
//  se hace una llamada al metodo GetPlugIn del environment, que lee de la lista
//  la direccion del Plug-In solicitado, y de esta forma cada instancia de la clase
//  FMT_ClFormato tiene acceso directo a las funciones de formateo del idioma representado
//  por esta clase.
//
/////////////////////////////////////////////////////////////////////////////////////////////

struct StPlugIn {

	m4int16_t (*NumberToText)(ClCadena *a_pCad, ClCadena *a_pNum, m4bool_t bGender, m4bool_t bIsDate);
	m4int16_t (*DecimalToText)(ClCadena  *a_pCad, ClCadena  *a_pNum, m4uint32_t ai_iDecGrup, m4bool_t bGender);
	m4int16_t (*YearToText)(ClCadena *a_pCad, ClCadena *a_pNum, m4bool_t bGender) ;

	const m4char_t* (*GetMonth)(m4uint16_t);   
	const m4char_t* (*GetDay)(m4uint16_t);

};	

struct StPlugInData {

	//Link al PlugIn del formato cargado
	const StPlugIn* m_StPlugInEnv;

	// DLL externa del plug-in que se carga
	M4ClLoaDll m_oDll; 
	m4bool_t m_bIsLoadedDLL;

	typedef map<ClMIT_Str, m4uint32_t, less<ClMIT_Str> > List_t;
	
	List_t WordList_t, PrefixList_t, SepList_t;
};

/////////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANTE:
//
// Todas las DLLs deben de implementar una funcion con esta firma, y 
//	que se llame AttachNewLanguage.
/////////////////////////////////////////////////////////////////////////////////////////////

typedef void (*AttachLanguage_t)(const StPlugIn** ai_stInfo, m4char_t* ai_pcLanguage);


//////////////////////////////////////////////////////////////////////


class FMT_ClPlugInManager
{
	public:

	FMT_ClPlugInManager();
	virtual ~FMT_ClPlugInManager() { FMT_ClPlugInManager::End(); }
	virtual size_t GetSizeof() { return sizeof(FMT_ClPlugInManager) ; }
 
	void Init();
	void End();

	//Carga todos los PlugIn. El proceso de carga es siempre el mismo. Primero carga los formatos
	// internos (SP, EN, GE, FR y US), y luego carga los PlugIns indicados en la entrada del
	// Registry mediante un DllOpen.
	m4int16_t LoadAllPlugIns();

	//Añade un Plug-In a la lista pasandole el identificador del idioma. Internamente
	//la funcion se encarga de la conversion del identificador a entero
	m4int16_t AddPlugIn(m4char_t* ai_pcLanguage, m4uint16_t ai_uiNumber);

	//Obtiene el puntero al Plug-In solicitado mediante el entero identificador
	m4int16_t GetPlugInData(m4uint32_t ai_uiIdiomaDestino, StPlugInData** ao_StPlugInDataInfo);

	//Rellena las listas de excepciones
	m4int16_t LoadLists(StPlugInData* ai_stDataInfo, m4char_t* ai_pcLanguage);

	//----->Lista de Plug-Ins
	//La lista las direcciones de las estructuras de los Plug-Ins.
	//La clave es un entero obtenido a partir de la cadena identificadora del idioma
	// Por ejemplo: "SP"-->'S'*0x100+'P', seria la clave para Español
	typedef map<m4uint16_t, struct StPlugInData*, less<m4uint16_t> > KeyList_t;

	KeyList_t m_oPlugInList;	//Lista

	//Link al PlugIn del formato cargado
	StPlugInData* m_StPlugInDataEnv;
	//----->Fin Lista de Plug-Ins
};
#endif


