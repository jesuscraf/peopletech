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

#include "m4types.hpp"

struct StPlugIn {

	short (*NumberToText)(ClCadena *a_pCad, ClCadena *a_pNum, bool bGender, bool bIsDate);
	short (*DecimalToText)(ClCadena  *a_pCad, ClCadena  *a_pNum, unsigned long ai_iDecGrup, bool bGender);
	short (*YearToText)(ClCadena *a_pCad, ClCadena *a_pNum, bool bGender) ;

	const char* (*GetMonth)(unsigned short);   
	const char* (*GetDay)(unsigned short);

};	

#endif


