
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                instru.h
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                14-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Fichero de declaración de la la clase de instrucciones
////
//==============================================================================



#include "m4mdfac_dll.hpp"

#include "m4types.hpp"
#include "m4var.hpp"


#ifndef __INSTRU_H__
#define __INSTRU_H__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class ClTempNitem ;
class ClTempNode ;



//=================================================================================
//
// ClInstruction
//
// Instrucción temporal
//
//=================================================================================

class M4_DECL_M4MDFAC ClInstruction{
public:
	m4uint8_t		m_iTipoIns;	//Tipo de Instruccion: Simple, Variant, Control, Item

	m4uint16_t		m_iNumFunc;
	m4int16_t		m_Flags;
	m4uint32_t		m_iNumNodo;
	union{
		m4uint32_t	m_iNumItem;
		m4int32_t	m_iNumVar;
	} m_iNumItem_Var;
	m4uint32_t		m_iNumRegla;
	m4uint8_t		m_iNumAtrib;
	m4uint32_t		m_pcNameChannel;
	m4VariantData	m_uVariant;
	m4bool_t		m_bIsChar;	//no se serializa. Solo para saber si es char
	ClTempNitem		*m_poItem;	//puntero al item temporal. Al serializar se guarda el handle
	ClTempNode      *m_poNode;
	m4bool_t		m_bVariantAsInt;	//no se serializa. Solo para saber si el variant numerico debe guardarse como entero o no
};


#endif

