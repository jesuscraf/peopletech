//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             mmmap.hpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             27/04/98
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


#include "sizerutils.hpp"

template <class T>
m4return_t ClComponentMap<T>::Get (m4uint32_t ai_blockuniquekey, T*& ao_pcomponent)
{
	
	M4_TYPE_ITERATOR	BUK2PCOMPONENT::iterator	it;

    it = m_map.find(ai_blockuniquekey);

    if (it != m_map.end()) {
        ao_pcomponent = &((*it).second);
    }
    else {
        //no encontrado:
        ao_pcomponent = 0;
    }

    return M4_SUCCESS;
}

template <class T>
m4return_t ClComponentMap<T>::Add (m4uint32_t ai_blockuniquekey, T*& ao_pcomponent)
{
#ifdef _DEBUG
    T *p;
    Get (ai_blockuniquekey, p);
    M4_ASSERT (p == 0);
#endif

    T aux;

    //pair<BUK2PCOMPONENT::iterator, m4bool_t> pp = m_map.insert (BUK2PCOMPONENT::value_type(ai_blockuniquekey, aux));
	//ao_pcomponent = &(( *(pp.first) ).second);

	//Para que compila en Sun
    ao_pcomponent = &(( *((m_map.insert (BUK2PCOMPONENT::value_type(ai_blockuniquekey, aux))).first) ).second);

    return M4_SUCCESS;
}

template <class T>
m4return_t ClComponentMap<T>::Remove (m4uint32_t ai_blockuniquekey)
{
#ifdef _DEBUG
    T *p;
    Get (ai_blockuniquekey, p);
    M4_ASSERT (p != 0);
#endif

    m_map.erase (ai_blockuniquekey);

#ifdef _DEBUG
    Get (ai_blockuniquekey, p);
    M4_ASSERT (p == 0);
#endif

	return M4_SUCCESS;
}

template <class T>
ClComponentMap<T>::~ClComponentMap ()
{
    RemoveAll ();
}

template <class T>
m4return_t ClComponentMap<T>::RemoveAll (void)
{
    m_map.clear ();

	return M4_SUCCESS;
}

template <class T>
m4return_t ClComponentMap<T>::CopyMap(ClComponentMap <T> * ai_pClCompMap)
{
	M4_TYPE_ITERATOR	BUK2PCOMPONENT::iterator	it;

    T * SourceArray, * DestArray;

    it = ai_pClCompMap->m_map.begin();
    while (it != ai_pClCompMap->m_map.end())
    {
        SourceArray = &((*it).second);
        Add((*it).first, DestArray);
        DestArray->Copy(SourceArray);
        it ++;
    }

    return M4_SUCCESS;
}

template <class T>
m4return_t ClComponentMap<T>::GetSize (ClSize &ao_size)
{
	ao_size+=sizeof(ClComponentMap<T>);
		ao_size-=sizeof(m_map);
		
	if (GetMapStructureSize(m_map,ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	M4_TYPE_ITERATOR BUK2PCOMPONENT::iterator it;
	for (it=m_map.begin();it!=m_map.end();++it)
	{
		ao_size-=sizeof((*it).second); // ya esta contada en GetMapStructureSize
		if ( (*it).second.GetSize(ao_size)!=M4_SUCCESS ){
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}
