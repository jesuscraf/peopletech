//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clfile.h
// Project:			 enginedll.dll
// Author:           Meta Software M.S. , S.A
// Date:			 7/28/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    Gestion basica de ficheros.
//
//
//==============================================================================
#ifndef __CLFILE__H__
#define __CLFILE__H__

#include <stdio.h>
//#include "multith.hpp"

class ClThreadEnv ;

//==============================================================================M4ClFile

class M4ClFile  
{
private:

	//ClThreadEnv *m_poEnv ;	//Simple link

	FILE *m_pstF ;

	struct {
		m4char_t * Str ;			//Cadena dinámica
		size_t   Size ;
	} m_StPath ;

public:

	//-----------------------------------Inicializacion---------------------------------------

	M4ClFile() ;  

	virtual ~M4ClFile() { Close() ;}
	virtual size_t GetSizeof() { return sizeof(M4ClFile) ; }

	// EXIST     : fichero debe existir o error. Abre el fichero en modo 
	//				READ o READ_WRITE.
	// NEW       : fichero no debe existir o error . Crea el fichero. Abre en modo	
	//				WRITE  o READ_WRITE.
	// OVERWRITE : si el fichero existe lo machaca, si no crea uno nuevo. Abre en modo
	//				WRITE o READ_WRITE.

	enum MODE_EXIST  { EXIST, NEW, OVERWRITE } ;
	enum MODE_WRITE  { READ, WRITE, READ_WRITE } ;

	// Hacer open antes de utilizar otros metodos.
	// Abre siempre en modo binario.
	// Si ya se abrio un fichero, y no se cerro, da un error inmediato.
	// Si no puede abrir fichero, o se usa una combinacion de modos invalida, da 
	//	un error.
	// Retorna TRUE si tiene exito. Se usa SwOblig en vez de IMMEDIATE_ERROR, etc, para
	//	evitar problemas de inclusion de cabeceras, pues trace precisa de file, 
	//	y viceversa.

	m4bool_t Open( const m4char_t *ai_pcPath, MODE_EXIST ai_Exist, MODE_WRITE ai_Write ) ;

	// Cierra solo si existe fichero abierto.
	void Close() ;

	//-------------------------Read&Write&Seek---------------------------------------------

	// Se escribe desde la posicion actual, e incrementa esta.
	void W( const void *ai_pvBuff, size_t ai_szSize, size_t ai_szNItems=1 ) ;

	// Escribe cadenas
	void WStr( const m4char_t * ai_pcStr ) ;

	// Da NumBytes leidos, siempre=NBytes, excepto si eof, que puede dar menos.
	// Se lee desde la posicion actual, e incrementa esta.
	size_t R( void * ai_pvBuff, size_t ai_szSize, size_t ai_szNItems=1 ) ;

	// Dan la posicion actual, y la ponen. La posicion se pondera en bytes.

	m4uint32_t GetPos() ;
	void SetPos( m4uint32_t ai_uiOff ) ;

	// Da el tamaño en bytes. 

	m4uint32_t GetSize() ;

	m4bool_t IsEOF() ;
	
	//-------------------------Otros-------------------------------------------------------

	// Hace un flush del fichero

	void Flush() ;

	// Para uso immediato, nombre del fichero, o NULL si no hay ninguno abierto.

	const m4char_t * GetName() { return m_StPath.Str ; }

	//Da true si hay algun fichero abierto

	m4bool_t IsOpen() { return (m_pstF != NULL) ? M4_TRUE : M4_FALSE  ; } 

	//Borra un fichero que debe existir Da true si lo borra, false si hay error. Si no puede
	//	borrarlo, genera un error local.
	static m4bool_t Remove(const m4char_t *ai_pcStr) ;

	// Puede ejecutarse sobre ficheros NO abiertos o abiertos: dice si existe o no un fichero.
	static m4bool_t Exist(const m4char_t *ai_pcStr) ;

};

#endif

