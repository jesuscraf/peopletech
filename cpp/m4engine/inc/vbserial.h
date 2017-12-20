//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             vbserial.h
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
//
//	Serializaciones para la sesion VB. Ver *.DOC sobre el tema.
//
//==============================================================================

#ifndef __VBSERIAL__H__
#define __VBSERIAL__H__

#include "m4clbuf.h"
#include "clstr.h"

//====================================================================ClENG_VB_Serialize

class ClENG_VB_Serialize
{
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	//-------------------------Miembros-------------------------------------------------

	M4ClBuffer			m_oBuff ;	//Buffer serializacion. Buffer para trabajr sin limites.

	enum { BUFF_INTERBLOCK=1024 } ;

MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_VB_Serialize();
	virtual ~ClENG_VB_Serialize() { ClENG_VB_Serialize::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_VB_Serialize) ; }
	virtual void End() ;

	//---------------------------Serialización------------------------------------------------

	// Para serializar, son ciclos 1 Init() - n Serialize

	void Init( ) ;	// Vacia Buffer
	void SerializeInt(m4int16_t ai_Par) ;
	void SerializeLong(m4int32_t ai_Par) ;
	void SerializeChar(m4char_t ai_Par) ;
	void SerializeStr(m4char_t *ai_Par) ;
	void SerializeUInt(m4uint16_t ai_Par) { SerializeInt(ai_Par); } 
	void SerializeULong(m4uint32_t ai_Par) { SerializeLong(ai_Par); } 
	void SerializeBool(m4bool_t ai_Par) { SerializeChar( ai_Par ? 'T' : 'F' ); }

	//False si no hay suficiente espacio en la cadena

	m4bool_t SaveToStr(m4char_t *ao_pcSerialOut, m4uint32_t *aoi_puiSerialOutSize) ;
} ;

//====================================================================ClENG_VB_Deserialize

class ClENG_VB_Deserialize
{
MIT_PRIVATE:

	StMIT_Inspector* m_poInsp;	//Simple link

	//-------------------------Miembros-------------------------------------------------

	ClMIT_Str	m_oStr ;	//Buffer entrada. Str porque el tamaño es conocido y fijo.
	m4char_t * m_pcPos ;	//Sig posicion a leer

MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_VB_Deserialize();
	virtual ~ClENG_VB_Deserialize() { ClENG_VB_Deserialize::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_VB_Deserialize) ; }
	virtual void End() ;	//Descarga todo

	//---------------------------Deserialización------------------------------------------------

	//Para deserializar, ciclos 1 Init, n deserialize.
	//No se testea que la lectura sobrepase la cadena.

	void Init( m4char_t *ai_pcSerializeIn) ;

	m4int16_t DeserializeInt() ;
	m4int32_t DeserializeLong() ;
	m4char_t  DeserializeChar() ;
	void DeserializeStr(ClMIT_Str *ao_poStr) ;
	m4uint16_t DeserializeUInt() { return DeserializeInt(); } 
	m4uint32_t DeserializeULong() { return DeserializeLong(); } 
	m4bool_t  DeserializeBool() { return DeserializeChar()=='T' ; }

} ;


#endif


