//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             pi.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/28/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//
//
//
//==============================================================================

#ifndef __PI__H__
#define __PI__H__

//Para el multimap
#include "m4stl.hpp"
#include "clstr.h"

class ClENG_PhysicalFile ;
class ClENG_Field ;

//=================================================================StENG_PI


struct StPi_t {

	//Datos publicos solo para consulta

	// PI_PLACE indica donde esta el Pi a imprimir (mem o fichero)
	enum PI_PLACE {  PI_IN_RAM, PI_IN_FILE	}; 

	m4uint32_t m_Handler ;	//SI es unico, util para enlaces externos con PIs.

	ClMIT_Str m_oIdPI ;		//NO tiene porque ser unico.
	
	m4uint32_t m_Offset ; // Offset donde imprimir Pi,en mem o fich 
	size_t     m_NBytes ; // NBytes Reservados
	PI_PLACE   m_Place;   // Lugar donde imprimir Pi
	m4int16_t  m_NPage;   // NPage de Pi
	ClENG_PhysicalFile *m_poFile ;	//NULL si PI_IN_RAM, link al fichero si PI_IN_FILE

	void Init( StPi_t *ai_poFrom ) ;

	void ChangeToIN_FILE( ClENG_PhysicalFile *ai_poFile, m4uint32_t ai_Offset ) ;
} ;

//=================================================================ClENG_PI_Manager

class ClENG_PI_Manager
{
MIT_PUBLIC:

	typedef multimap<ClMIT_Str, StPi_t *, less<ClMIT_Str> > PI_List_t;
	m4uint32_t m_HandlerAux ;

MIT_PRIVATE:
	
	StMIT_Inspector* m_poInsp;	//Simple link
	PI_List_t m_oPI_List;


MIT_PUBLIC:
	
	//---------------------------------------Initialize--------------------------------

	ClENG_PI_Manager();
	virtual ~ClENG_PI_Manager() { ClENG_PI_Manager::End() ; }
	virtual void End() ;

	virtual size_t GetSizeof() { return sizeof(ClENG_PI_Manager); }

	void Init();


	//---------------------------------------Gets--------------------------------------

	//Obtiene iteradores para hacer recorridos.

	void GetIterators( PI_List_t::iterator &ao_Beg, PI_List_t::iterator &ao_End ) 
		{ ao_Beg=m_oPI_List.begin() ; ao_End = m_oPI_List.end() ; } 

	//Devuelve NULL o los datos de la PI encontrada. El dato es un link al objeto real, pueden
	//	modificarse los datos a traves del puntero.

	StPi_t * FindPi( const char *ai_pcIdPI, m4bool_t ai_bSwErrorIfNotFound=M4_FALSE) ;

	m4bool_t HasPendingPIs(ClENG_PhysicalFile *ai_File) ;

	//---------------------------------------Flow--------------------------------------

	//Cuando se añade están en PI_IN_RAM
	//Da un link a la PI creada.

	StPi_t * AddNewPi( 
		const char *ai_pcIdPI, m4int16_t ai_iPage, m4uint32_t ai_Offset, size_t ai_NBytes) ;

	//Cambia los PI_IN_RAM de la pagina ai_iPage al estado PI_IN_FILE actualizando el offset
	//	a m_Offset+ai_FilePos (llamar primero a esta función antes de volcar fisicamente los
	//buffer).

	void MoveAllPiToFile(m4int16_t ai_iPage, ClENG_PhysicalFile *ai_File, m4uint32_t ai_FilePos) ;

	//Extrae (borra) un PI (si hay claves repetidas el primero que encuentre).
	//En ao_poData copia los datos de la PI extraida.
	//Da false si hay error (no encuentra PI).

	m4bool_t ExtractPi( const char *ai_pcIdPI, StPi_t *ao_poData ) ;

} ;

#endif
