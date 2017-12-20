//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             anoddir.inl
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             22/09/97
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

// este archivo solo tiene sentido incluido en el anoddir.hpp:
#ifdef __ANODDIR_HPP__


#include "chandef.hpp"



template <class T>
T *ClNodeDirectory<T>::Get (ClNodeIndex  ai_nodeindex)
{
	T *p;

    if ( ai_nodeindex < Count () ) {
        // ok
        p = m_aClNode + ai_nodeindex;
    }
    else {
        // out of range
        p = NULL;
    }

	return p;
}

template <class T>
T *ClNodeDirectory<T>::Get (ClHandle ai_nodehandle)
{
	T *p;

    if (m_pChannelDef) {
        p = m_aClNode + m_pChannelDef->NodeIndexByHandle (ai_nodehandle);
    }
    else {
        p = NULL;
    }

	return p;
}

template <class T>
T *ClNodeDirectory<T>::Get (m4pcchar_t ai_pcNodeId)
{
	T *p;

    if (m_pChannelDef) {
        ClHandle hn = m_pChannelDef->NodeHandleById (ai_pcNodeId);
        if (hn) {
            p = m_aClNode + m_pChannelDef->NodeIndexByHandle (hn);
        }
        else {            
            p = NULL;
        }
    }
    else {
        // error:
        p = NULL;
    }

	return p;
}


template <class T>
T &ClNodeDirectory<T>::operator[] (ClNodeIndex  ai_nodeindex)
{
    if ( ai_nodeindex < Count () ) {
        // ok
        ((ClNodeDirectory<T>*)this)->m_pCurrentNode = m_aClNode + ai_nodeindex;

        Index = ai_nodeindex;
    }
    else {
        // out of range
        ((ClNodeDirectory<T>*)this)->m_pCurrentNode = NULL;

   		 Index = M4DM_EOF_INDEX;
	}

    return *m_pCurrentNode;
}

template <class T>
T &ClNodeDirectory<T>::operator[] (ClHandle ai_nodehandle)
{
    M4_ASSERT (m_aClNode);

    if (m_pChannelDef) {
        ((ClNodeDirectory<T>*)this)->m_pCurrentNode = m_aClNode + m_pChannelDef->NodeIndexByHandle (ai_nodehandle);

        // falta localizar el Index !!
		//M4_ASSERT (0);
    }
    else {
        ((ClNodeDirectory<T>*)this)->m_pCurrentNode = NULL;

   		 Index = M4DM_EOF_INDEX;
	}

    return *m_pCurrentNode;
}

template <class T>
T &ClNodeDirectory<T>::operator[] (m4pcchar_t ai_pcNodeId)
{    
    if (m_pChannelDef) {
        ClHandle hn = m_pChannelDef->NodeHandleById (ai_pcNodeId);
        if (hn) {
            ((ClNodeDirectory<T>*)this)->m_pCurrentNode = m_aClNode + m_pChannelDef->NodeIndexByHandle (hn);
        }
        else {            
            // warning, el nodo pedido no existe:

            ((ClNodeDirectory<T>*)this)->m_pCurrentNode = NULL;

           Index = M4DM_EOF_INDEX; 
        }
    }
    else {
        // error:

        ((ClNodeDirectory<T>*)this)->m_pCurrentNode = NULL;

        Index = M4DM_EOF_INDEX;
    }

    return *m_pCurrentNode;
}

template <class T>
T &ClNodeDirectory<T>::operator *() const
{
    return *m_pCurrentNode;
}

template <class T>
T *ClNodeDirectory<T>::operator ->() const
{
    return m_pCurrentNode;
}

template <class T>
m4uint16_t ClNodeDirectory<T>::Count (void) const
{
    m4uint16_t n;

    // Sí, es posible llamar a esta función con m_pChannelDef == 0
    // ... por que se haya hecho el destroy y se siga llamando al Count
    if (m_pChannelDef) {
        n = m_pChannelDef->NumberOfNodes ();
    }
    else {
        n = 0;
    }

    return n;
}

template <class T>
ClNodeDirectory<T>::ClNodeDirectory ()
{
    m_pChannelDef = 0;
    m_aClNode = 0;
}

template <class T>
ClNodeDirectory<T>::~ClNodeDirectory ()
{
//    _Destroy ();
}

/*
template <class T>
m4return_t ClNodeDirectory<T>::Attach (ClChannelDef *ai_pchdef, T *ai_pbuffer)
{
    M4_ASSERT (ai_pchdef);

//    _Destroy ();

    m_pChannelDef = ai_pchdef;

    //m4uint16_t nnodes = Count ();

    m_aClNode = ai_pbuffer;

    return M4_SUCCESS;
}
*/

template <class T>
m4return_t ClNodeDirectory<T>::Detach (void)
{
    m_pChannelDef = 0;
    //_Destroy ();

    return M4_SUCCESS;
}

/*
template <class T>
void ClNodeDirectory<T>::_Destroy (void)
{
}
*/
template <class T>
m4return_t 
ClNodeDirectory<T>::GetSize (ClSize &ao_size)
{
	ao_size+=sizeof(ClNodeDirectory<T>);

	T * pT=m_aClNode;

    if (m_pChannelDef)
    {
	    m4uint32_t nnodes = m_pChannelDef->NumberOfNodes ();
	    
	    for (m4uint32_t i=0;i<nnodes;++i){
		    if (pT ->GetSize(ao_size)!=M4_SUCCESS){
			    return M4_ERROR;
		    }
		    ++pT;
	    }
    }
	
	return M4_SUCCESS;
}
#endif  // __ANODDIR_HPP__
