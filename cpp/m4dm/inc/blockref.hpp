//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             blockref.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             10/09/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    ClBlockReference
//
//
//==============================================================================

#include "m4types.hpp"

#ifndef __BLOCK_REF_HPP__
#define __BLOCK_REF_HPP__
#include "m4dm_dll.hpp"
class ClBlock_Channel_Proxy;
class ClSize;

class M4_DECL_M4DM ClBlockReference
{
public:
    //inline por rendimiento:
    ClBlockReference () : m_pBlockProxy (0)
    {
    }

    ClBlockReference (ClBlock_Channel_Proxy *ai_pb) : m_pBlockProxy (ai_pb)
    {
    }

    ~ClBlockReference ()
    {
        Reset ();   //Importante !!
    }

    void Invalide (void)
    {
        //se utiliza para indicar que la referencia actual
        //no es válida y no debe usarse
        m_pBlockProxy = 0;
    }

    ClBlock_Channel_Proxy *operator -> () const
	{
		return m_pBlockProxy;
	}

    ClBlock_Channel_Proxy *GetpProxy () const
	{
		return m_pBlockProxy;
	}

    const ClBlockReference &operator = (const ClBlockReference &ai_br)
    {
	    //dereferenciamos el proxy actual:
	    DecRef ();
        m_pBlockProxy = ai_br.m_pBlockProxy;
	    //y referenciamos el nuevo:
	    IncRef ();

	    return *this; 
    }

	m4bool_t IsNull () const
	{
		return (m_pBlockProxy)? M4_FALSE : M4_TRUE;
	}

	void Reset (void)
    {
	    //dereferenciamos el proxy actual:
	    DecRef ();
        m_pBlockProxy = 0;
    }

	void Set (ClBlock_Channel_Proxy *ai_pnewproxy)
    {
	    if (m_pBlockProxy != ai_pnewproxy) {
		    //dereferenciamos el proxy actual:
		    DecRef ();
		    m_pBlockProxy = ai_pnewproxy;
		    //y referenciamos el nuevo:
		    IncRef ();
	    }
    }

	m4bool_t operator != (const ClBlockReference &ai_br) const
    {
	    return (m_pBlockProxy != ai_br.m_pBlockProxy)? M4_TRUE : M4_FALSE;
    }

	m4bool_t operator == (const ClBlockReference &ai_br) const
    {
	    return (m_pBlockProxy == ai_br.m_pBlockProxy)? M4_TRUE : M4_FALSE;
    }

	operator const m4bool_t () const
    {
	    return (m_pBlockProxy)? M4_TRUE : M4_FALSE;
    }

	m4return_t GetSize(ClSize &ao_size);

private:
	ClBlock_Channel_Proxy *m_pBlockProxy;

	void DecRef (void);
	void IncRef (void);
};

#endif //__BLOCK_REF_HPP__
