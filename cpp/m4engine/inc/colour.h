//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             colour.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:17/07/1997
// Language:         C++
// Operating System: WINDOWS. 
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    Contiene todo lo necesario para manejo de colores y paletas
//
//==============================================================================

#ifndef __COLOUR__H__
#define __COLOUR__H__

#define PALETTE_MAX_COLORS 256

#include "m4stl.hpp"
#include "clstr.h"
#include "clstr.h"

//==============================================================================ClENG_Colour

class ClENG_Colour
{

MIT_PROTECTED:

	StMIT_Inspector* m_poInsp;	//Simple link

MIT_PUBLIC:

	//ID_TRANSPARENT es simplemente un Id util para otros objetos.

	enum { ID_NULL=-1, ID_TRANSPARENT=-2 } ;

MIT_PRIVATE:

	m4uint32_t m_uiIdColour ;	//Id del Colour
	ClMIT_Str m_oName ;		//Nombre simplemente indicativo

	m4uint8_t  m_R, m_G, m_B ;	//Componentes RGB

MIT_PUBLIC:

	// ------------------------- Inicialización ----------------------------------	

	ClENG_Colour() ;
	virtual	~ClENG_Colour() { ClENG_Colour::End();}
	virtual size_t GetSizeof() { return sizeof(ClENG_Colour) ; }
	virtual void End() ;

	void Init( 
		m4uint32_t ai_uiIdColour, m4char_t *ai_pcName,
		m4uint8_t  ai_R, m4uint8_t ai_G, m4uint8_t ai_B
	);

	// ------------------------- Consultas ----------------------------------	

	m4uint32_t GetId() { return m_uiIdColour ; }
	m4char_t * GetName()  { return (m4char_t *)m_oName ; }
	
	m4uint8_t GetR() { return m_R ; } 
	m4uint8_t GetG() { return m_G ; } 
	m4uint8_t GetB() { return m_B ; } 

	m4bool_t IsBlack() { return (m_R+m_G+m_B)==0 ; }
	m4bool_t IsWhite() { return m_R==0xFF && m_G==0xFF && m_B==0xFF ; }

	// ------------------------- Otros----------------------------------------	

	//Dado un color RGB, obtiene un indice de 'peso' o intensidad de 0 a 255. 
	//Al 0 le corresponde el negro y al 255 el blanco.
	
	m4int16_t GetReductionIndexTo256() ;
} ;


//==============================================================================ClENG_Palette

class ClENG_Palette 
{

MIT_PROTECTED:

	StMIT_Inspector* m_poInsp;	//Simple link

MIT_PRIVATE:

	ClENG_Colour *m_poColour ;	//Array dinamico ...
	m4int16_t m_iColourSize ;	//	... de items Colour (256 entradas MAXIMO)
	m4int16_t m_iRealColourSize ;

	//Esta lista mantiene la relacion ColourId-PaletteIndex
	typedef map<m4uint32_t, m4uint32_t, less<m4uint32_t> > ColourId_PaletteIndex_t;
	ColourId_PaletteIndex_t m_oColourIdIndex ;	

MIT_PUBLIC:
	// ------------------- Contructor y Destructor -------------------------------	

	ClENG_Palette() ;
	virtual	~ClENG_Palette() { ClENG_Palette::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_Palette) ; }

	// ------------------------ Inicialización -----------------------------------	

	virtual void End() ;

	//Se da el tamaño de la paleta, que debe ser >=0 y <=256 (GetMaxSize()). Si no, dara error.
	//Este limite viene impuesto para crear paletas validas en todo tipo de dispositivos, pues
	//	cualquier dispositivo capaz de manejar colores va a soportar en su hardware una paleta 
	//	de 256 entradas.
	//Internamente, si ai_iSize es 0, se crea una paleta con una entrada, para que las funciones 
	//	Get tengan al menos un valor para devolver en caso de fallo.
	//Rellena la paleta con valores a negro (todo 0)

	m4bool_t Init(
		m4int16_t ai_iSize,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR	
	) ;

	//Contructor copia

	void Init(ClENG_Palette *ai_poSource) ;

	//Size es igual a GetRealSize, excepto cuando se intento crear una paleta con 0 entradas, en
	//cuyo caso GetSize() da 1 (la entrada defecto creada automaticamente),  y GetRealSize da 0.
	m4int16_t GetSize() { return m_iColourSize ; }
	m4int16_t GetRealSize() { return m_iRealColourSize ; }	
	static m4int16_t GetMaxSize() { return 256 ; }

	//Actualiza el valor n del indice de la paleta pasado en ai_PaletteIndex.
	//Si el indice es erróneo se genera un error local y no se hace nada.
	//Si el ColourId del Colour pasado esta repetido en la paleta, genera un error local y no se
	//	hace nada.
	//Da true si tiene exito, false si no.

	m4bool_t SetColour( 
		m4int16_t ai_PaletteIndex,
		m4uint32_t ai_uiIdColour, m4char_t *ai_pcName,
		m4uint8_t  ai_R, m4uint8_t ai_G, m4uint8_t ai_B
	);

	//Añande un color a la paleta, el indice siempre debe ser el tamaño de la paleta mas uno.
	//Si el indice es erróneo se genera un error local y no se hace nada.
	//Si el ColourId del Colour pasado esta repetido en la paleta, genera un error local y no se
	//	hace nada.
	//Da true si tiene exito, false si no.

	m4bool_t AddColour( 
		m4int16_t ai_PaletteIndex,
		m4uint32_t ai_uiIdColour, m4char_t *ai_pcName,
		m4uint8_t  ai_R, m4uint8_t ai_G, m4uint8_t ai_B
	);

	
	// Da el indice de paleta correspondiente a un ColourId. Si no lo encuentra devuelve el indice
	//0  (que siempre existe). 
	// Si se pasa ai_bSwFound, se actualiza para indicar si se encontro o no el indice.
	// ai_bThrowError indica si se lanza un error local en caso de no encontrar el indice.

	m4int16_t GetIndex(
		m4uint32_t ai_ColourId, 
		m4bool_t *ai_bSwFound=NULL, m4bool_t ai_bThrowError=M4_TRUE 
	) ;
	
	// Da acceso a un Colour segun su ColourId. Si no lo encuentra, devuelve la direccion al indice
	//0, que siempre existe. Los parametros ai_bSwFound y  ai_bThrowError tienen el mismo 
	//significado que en GetIndex.

	ClENG_Colour * GetColourById(
		m4uint32_t ai_ColourId,
		m4bool_t *ai_bSwFound=NULL, m4bool_t ai_bThrowError=M4_TRUE 
	) ;

	// Igual que la de antes, pero por indice.

	ClENG_Colour * GetColourByIndex(
		m4int16_t ai_PaletteIndex,
		m4bool_t *ai_bSwFound=NULL, m4bool_t ai_bThrowError=M4_TRUE 
	) ;
} ;


#endif
