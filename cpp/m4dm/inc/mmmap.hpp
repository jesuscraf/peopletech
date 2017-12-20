//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             mmmap.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             27/04/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Memory Dump 'Visitor'
//
//
//==============================================================================

#include "m4types.hpp"
#include "m4stl.hpp"

#ifndef __MMMAP_HPP__
#define __MMMAP_HPP__

template <class T>
class ClComponentMap
{
public:
//     ClComponentMap ();
    ~ClComponentMap ();

    //Crea una nueva instancia de tipo <T>, la inserta en el mapa con
    //la clave indicada y devuelve la dirección del nuevo objeto en ao_pcomponent
    //Lanza un assert si en la clave ya existía un T
    m4return_t Add (m4uint32_t ai_blockuniquekey, T*& ao_pcomponent);

    //busca por la clave indicada y devuelve la dirección del T en ao_pcomponent
    m4return_t Get (m4uint32_t ai_blockuniquekey, T*& ao_pcomponent);
    

    m4return_t CopyMap(ClComponentMap<T> * ai_pClCompMap);

    //elimina una entrada
    m4return_t Remove (m4uint32_t ai_blockuniquekey);
	
    m4return_t RemoveAll (void);

	m4return_t GetSize(ClSize &ao_size);

private:    

	typedef map<m4uint32_t, T, less<m4uint32_t> > BUK2PCOMPONENT;	

    BUK2PCOMPONENT m_map;

};


#include "mmmap.inl"

#endif // __MMMAP_HPP__
