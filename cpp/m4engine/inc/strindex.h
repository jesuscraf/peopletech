//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             StrIndex.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    Gestion de Indice-Clave, Clave-Indice
//
//==============================================================================

#ifndef __STRINDEX__H__
#define __STRINDEX__H__

#include "m4stl.hpp"
#include "clstr.h"

class ClMIT_Args ;

/*============================================================================ClENG_StrIndex
	Esta clase esta pensada para ser usada apoyando a arrays de objetos, de tal forma que
obtenemos indices aplicables en dichos arrays, en base a una clave.
	Por eso, el uso correcto es hacer todos los New necesarios al principio, y despues usar
los GetIndex necesarios para acceder al array. No es correcto, mientras se usa el array,
borrar ninguna clave, ya que eso afectaria a lo que devuelve GetIndex.
*/

class ClENG_StrIndex
{
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	//La lista almacena un largo adicional, por si es util.
	typedef map<ClMIT_Str, m4uint32_t, less<ClMIT_Str> > KeyList_t;	

	KeyList_t m_oList;	//Lista

MIT_PUBLIC:

	ClENG_StrIndex();
	virtual ~ClENG_StrIndex() { ClENG_StrIndex::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_StrIndex) ; }
	virtual void End() ;	

	//No hace nada en especial:debe haber despues una secuencia de New que llenen los 
	//	valores.
	void Init() ;

	// Inicializa el objeto y hace los New pertinentes, via ClMIT_Args. 
	// Si ai_poArg no es NULL, se usa este objeto, con el fin de que el programa llamaador use
	//	los argumentos ya parseados. Si no se pasa, se usa un objeto local.
	// Da false si hay problemas.
	m4bool_t Init( const m4char_t * ai_pcCommand,  ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR,
		ClMIT_Args *ai_poArg=NULL, m4char_t ai_cSeparator='.', const m4char_t * ArgSep=",\t") ;
	
	// Da TRUE si tiene exito, y FALSE si no (además en este caso genera un error local).
	m4bool_t New( const m4char_t *ai_pcKey, m4uint32_t ai_uiData=0 ) ;

	// Devuelve el indice correspondiente, o INDEX_EOF si hay problemas (generara error local).
	// Si se pasa ai_puiData, almacenara el valor ahi.

	enum {INDEX_EOF=-1} ;
	m4int16_t GetIndex( const m4char_t *ai_pcKey, m4uint32_t *ai_puiData=NULL,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	//Devuelve un puntero de uso inmmediato, que es la clave del indice correspondiente.
	//Da NULL si hay algun error.

	m4char_t * GetKey( m4int16_t ai_Index, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	
	//Numero de claves.

	m4uint32_t GetCount() { return m_oList.size() ; }

	//Obtiene, segun los datos, una cadena aplicable en el Init(ai_pcCommand). 

	void GetInitString(ClMIT_Str *ao_poStr, const m4char_t * ArgSep=",\t") ;

} ;

#endif
