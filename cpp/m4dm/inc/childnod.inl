//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             childnod.inl
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
#ifdef _UNIX 
  #include "anoddir.hpp"
  #include "nodedef.hpp"
  #include "node_x.hpp"
  #include "node_knl.hpp"
#endif

#ifdef __CHILDNOD_HPP__

template <class T>
ClChildNodeInfo<T> ClChildNodes<T>::ms_NullNodeInfo;


template <class T>
ClChildNodeInfo<T> &ClChildNodes<T>::operator [] (ClChildIndex ai_index)
{
    if (ai_index < m_iCount) {
        return m_apChildNodeInfo[ai_index];
    }
    else {
        return ms_NullNodeInfo;
    }
}

template <class T>
ClChildNodeInfo<T> &ClChildNodes<T>::operator [] (ClHandle ai_handle)
{
    m4uint16_t ipos;
    m4return_t res;
    res = GetPos (ai_handle, ipos);

    if (res == M4_SUCCESS) {
        return m_apChildNodeInfo[ipos];
    }
    else {
        return ms_NullNodeInfo;
    }
}

template <class T>
ClChildNodeInfo<T> &ClChildNodes<T>::operator [] (m4pchar_t ai_childid)
{
    m4uint16_t ipos;
    m4return_t res;
    res = GetPos (ai_childid, ipos);

    if (res == M4_SUCCESS) {
        return m_apChildNodeInfo[ipos];
    }
    else {
        return ms_NullNodeInfo;
    }
}

template <class T>
m4uint16_t ClChildNodes<T>::Count (void)
{
    return m_iCount;
}

template <class T>
m4return_t ClChildNodes<T>::GetPos (ClHandle ai_handle, m4uint16_t &ao_ipos)
{
    ClChildIndex i;

    for (i = 0; i < m_iCount; i++) {
        if (m_apChildNodeInfo[i].GetAddress ()->GetpExtInfo ()->GetHandle () == ai_handle) {
            ao_ipos = (m4uint16_t)i;
            return M4_SUCCESS;
        }
    }

    return M4_ERROR;
}

template <class T>
m4return_t ClChildNodes<T>::GetPos (m4pchar_t ai_childid, m4uint16_t &ao_ipos)
{
    ClChildIndex i;

    for (i = 0; i < m_iCount; i++) {
        if (0 == strcmp (m_apChildNodeInfo[i].GetAddress ()->GetpExtInfo ()->Id (), ai_childid) ) {
            ao_ipos = (m4uint16_t)i;
            return M4_SUCCESS;
        }
    }

    return M4_ERROR;
}

template <class T>
ClChildNodes<T>::~ClChildNodes ()
{
    Destroy ();
}

template <class T>
ClChildNodes<T>::ClChildNodes ()
{
    m_apChildNodeInfo = 0;
    m_iCount = 0;
}

template <class T>
void ClChildNodes<T>::Destroy (void)
{
    if (m_apChildNodeInfo) {

        delete []m_apChildNodeInfo;
        m_apChildNodeInfo = 0;
        m_iCount = 0;
    }
}


template <class T>
void ClChildNodes<T>::_Init (m4uint16_t ai_ipos, m4uint16_t ai_in, ClNodeDirectory<T> &ai_nodedir, ClNodeDef &ai_NodeDef)
{
    if (ai_in) {

        ClHandle hcon;
        ClHandle hn;
        m4uint16_t i;

        m_iCount = ai_in;
        m_apChildNodeInfo = new ClChildNodeInfo<T> [m_iCount];
        ClChildNodeInfo<T> *pcni;
        for (i = 0; i < m_iCount; i++, ai_ipos++) {
            hcon = ai_NodeDef.GetNodeConnectorHandleByPosition (ai_ipos);
            hn = ai_NodeDef.GetNodeConnectorSonNodeHandle (hcon);

            pcni = m_apChildNodeInfo + i;

            pcni->Init (ai_nodedir.Get (hn), hcon);
        }
    }
}


// se le pasa la dirección del nodo hijo y el handle del conector

template <class T>
void ClChildNodes<T>::Add (T *ai_paddress, ClHandle ai_hcon)
{
    // esto más cutre no puede ser:
    // toda la vida haciendo el mismo código
    ClChildNodeInfo<T> *ppxxx = new ClChildNodeInfo<T> [m_iCount + 1];

    if (m_apChildNodeInfo) {
        memcpy (ppxxx, m_apChildNodeInfo, m_iCount * sizeof (ClChildNodeInfo<T>));
        delete []m_apChildNodeInfo;
    }

    m_apChildNodeInfo = ppxxx;

    ClChildNodeInfo<T> *pcni = m_apChildNodeInfo + m_iCount;

    pcni->Init (ai_paddress, ai_hcon);
    
    m_iCount++;
}

template <class T>
m4return_t 
ClChildNodes<T>::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClChildNodes<T>);

	if (m_apChildNodeInfo){
		ao_size+=m_iCount*sizeof(*m_apChildNodeInfo) + sizeof(int); // sizeof(int) : En WIN si tiene destructor, se alocan 4 bytes mas,.
		for (m4uint32_t i=0;i<m_iCount;++i)
		{
			ao_size-=sizeof(*m_apChildNodeInfo);
			if (m_apChildNodeInfo[i].GetSize(ao_size)!=M4_SUCCESS){
				return M4_ERROR;		}
		}
	}

	return M4_SUCCESS;
}

#endif // __CHILDNOD_HPP__
