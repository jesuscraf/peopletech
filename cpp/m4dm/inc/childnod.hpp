//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             childnod.hpp
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

#include "m4types.hpp"
#include "index.hpp"
#include "handles.hpp"


#ifndef __CHILDNOD_HPP__
#define __CHILDNOD_HPP__

class ClNode_Data;
class ClNodeDef;

class ClSize;

template <class T>
class ClNodeDirectory;


template <class T>
class ClChildNodes;

template <class T>
class ClChildNodeInfo
{
public:
    ClChildNodeInfo ()
    {
        Address = 0;
        ConnectorHandle = 0;
    }

   ~ClChildNodeInfo () {}
   
    T* GetAddress (void) const
    {
        return Address;
    }
/*
    ClNodeIndex GetIndex (void) const
    {
        return Index;
    }
*/
    ClHandle GetConnectorHandle (void) const
    {
        return ConnectorHandle;
    }

    void Init (T *ai_paddress, ClHandle ai_hc)
    {
        Address = ai_paddress;
        ConnectorHandle = ai_hc;
    }

    // bug de VC++, hay que definir esto:
    m4bool_t operator < (const ClChildNodeInfo <T> &) const;
    m4bool_t operator == (const ClChildNodeInfo <T> &) const;
	m4return_t GetSize(ClSize &ao_size)
	{

		ao_size+=sizeof(ClChildNodes<T>);
			ao_size-=sizeof(ConnectorHandle);

		if (ConnectorHandle.GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
		
		return M4_SUCCESS;
	}



private:
    T *Address;
    ClHandle ConnectorHandle;
};

template <class T>
class ClChildNodes
{
public:
   ~ClChildNodes ();


    static ClChildNodeInfo<T> ms_NullNodeInfo;

    ClChildNodeInfo<T> &operator [] (ClChildIndex ai_index);
    ClChildNodeInfo<T> &operator [] (ClHandle ai_handle);
    ClChildNodeInfo<T> &operator [] (m4pchar_t ai_childid);

    m4uint16_t Count (void);
    m4return_t GetPos (ClHandle ai_handle, m4uint16_t &ao_ipos);
    m4return_t GetPos (m4pchar_t ai_childid, m4uint16_t &ao_ipos);

    void Add (T *ai_paddress, ClHandle ai_hcon);

    ClChildNodes ();
    void Destroy (void);
    void _Init (m4uint16_t ai_ipos, m4uint16_t ai_in, ClNodeDirectory<T> &ai_nodedir, ClNodeDef &ai_NodeDef);

	m4return_t GetSize(ClSize &ao_size);

private:
    m4uint16_t m_iCount;

    // el hijo puta del VC++ no me deja hacer esto:
    //vector<ClChildNodeInfo> m_vChildNodeInfo;
    //asi que le den por el culo al vector:
    ClChildNodeInfo<T> *m_apChildNodeInfo;

/*
// Solucion rápida al Bug de Microchofff
#if _MSC_VER >= 1100
	friend T;			// Bug in Templates MSVC++ 5.0
#else
	friend class T;		// For all rest of compilers
#endif
*/

};


#include "childnod.inl"


#endif // __CHILDNOD_HPP__ 
