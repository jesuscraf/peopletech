//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             piuni.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             18/09/2008
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Owner:			Antonio Durán
//	Modifications:	
//
// Definition:
//
//    PI information container for PDF unicode generation
//
//
//==============================================================================

#ifndef _PIUNI_H_
#define _PIUNI_H_

//Para el multimap
#include "m4stl.hpp"
#include "clstr.h"
#include "clmetric.h"
#include "font.h"

//=================================================================StPiUnicode_t


struct StPiUnicode_t {

	//Datos publicos solo para consulta

	m4uint32_t m_Handler ;	//SI es unico, util para enlaces externos con PIs.

	ClMIT_Str m_oIdPI ;		//NO tiene porque ser unico. Id de PI
	
	m4int16_t  m_NPage;     // NPage de Pi
	m4int_t    m_NNumPage;  // Número de página en el documento
	DvcUnit_t  m_X;         // Posición X en el documento
	DvcUnit_t  m_Y;         // Posición Y en el documento
	ClFONT_M_Font* m_oFont; // Fuente
	m4uint32_t m_Color;		// Color
	size_t     m_NChars ;   // Número máximo de caracteres
	ClMIT_Str  m_oHyperlink;// Valor de hyperlink


	void Init( StPiUnicode_t *ai_poFrom ) ;
} ;

//=================================================================ClENG_PIUnicode_Manager

class ClENG_PIUnicode_Manager
{
MIT_PROTECTED:

	typedef multimap<ClMIT_Str, StPiUnicode_t *, less<ClMIT_Str> > PI_List_t;
	m4uint32_t m_HandlerAux ;

MIT_PRIVATE:
	
	PI_List_t m_oPI_List;

MIT_PUBLIC:
	
	//---------------------------------------Initialize--------------------------------

	ClENG_PIUnicode_Manager() {};
	virtual ~ClENG_PIUnicode_Manager() { ClENG_PIUnicode_Manager::End() ; }
	virtual void End() ;

	void Init();


	//---------------------------------------Gets--------------------------------------

	//Obtiene iteradores para hacer recorridos.

	void GetIterators( PI_List_t::iterator &ao_Beg, PI_List_t::iterator &ao_End ) 
		{ ao_Beg=m_oPI_List.begin() ; ao_End = m_oPI_List.end() ; } 

	//Devuelve NULL o los datos de la PI encontrada. El dato es un link al objeto real, pueden
	//	modificarse los datos a traves del puntero.

	StPiUnicode_t * FindPi( const char *ai_pcIdPI, m4bool_t ai_bSwErrorIfNotFound=M4_FALSE) ;


	//---------------------------------------Flow--------------------------------------

	//Da un link a la PI creada.

	StPiUnicode_t * AddNewPi(
		const char *ai_poIdPI, m4int16_t ai_iPage,  m4int_t ai_iNumPage, DvcUnit_t ai_iX,
		DvcUnit_t ai_iY, ClFONT_M_Font *ai_oFont, m4uint32_t ai_iColor,
		size_t ai_szNChar, const char *ai_pcHyperlink);

	//Extrae (borra) un PI (si hay claves repetidas el primero que encuentre).
	//En ao_poData copia los datos de la PI extraida.
	//Da false si hay error (no encuentra PI).

	m4bool_t ExtractPi( const char *ai_pcIdPI, StPiUnicode_t *ao_poData ) ;

} ;

#endif
