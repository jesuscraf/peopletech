//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             level2relation.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             02/02/99
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef __LEVEL2RELATION__HPP__
#define __LEVEL2RELATION__HPP__

#include "m4dm_dll.hpp"
#include "m4stl.hpp"
#include "m4chcommon.hpp"

class ClGenericIODriver;


class M4_DECL_M4DM ClLevel2Relation
{
public:
	/** Constructor. */	
	ClLevel2Relation();

	/** Constructor copia. */
	ClLevel2Relation(const ClLevel2Relation& ai_source);
	
	/** Destructor. */
	~ClLevel2Relation(void);
	
	/** Operador de asignación. */
	ClLevel2Relation& operator= (const ClLevel2Relation& ai_source);
	
	/** Fija el nombre de la instancia de nivel2. */
	void SetInstance(const m4char_t* const ai_sInstance);

	/** Devuelve el nombre de la instancia de nivel2. */
	const m4char_t* const GetInstance(void) const;
	
	/** Serializa la relación de nivel2. */
	m4return_t Serialize(ClGenericIODriver& IOD) const;

	/** Deserializa la relación de nivel2. */
	m4return_t DeSerialize(ClGenericIODriver& IOD);


	/** Índice del objDir donde se encuentra el canal padre. */
	m4int16_t  m_iParentIndex;

	/** Índice del objDir donde se encuentra el canal hijo. */
	m4int16_t  m_iChildIndex;

	/** Índice del objDir donde se encuentra el acceso del hijo. */
	m4int16_t  m_iAccessIndex;
	
private:
	/** Nombre de la instancia. */
	m4char_t* m_sInstance;     
};


class M4_DECL_M4DM ClLevel2RealtionVector : public vector<ClLevel2Relation>
{
public:
    /** Serializa el vector de relaciones de nivel2. */
    m4return_t Serialize(ClGenericIODriver& IOD) const;

    /** Deserializa el vector de relaciones de nivel2. */
    m4return_t DeSerialize(ClGenericIODriver& IOD);
};

#endif
