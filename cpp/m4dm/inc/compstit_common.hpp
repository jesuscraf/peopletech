//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             compstit_common.hpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             03/09/98
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    class ClComponentStore Forward Iterators
//
//==============================================================================

#ifndef __COMPSTIT_COMMON_HPP__
#define __COMPSTIT_COMMON_HPP__
#include "m4dm_dll.hpp"
#include "m4types.hpp"

class ClBlock_Channel_Proxy;
class _AClComponentStore;
class ClComponentStore_Mono;
class ClSize;

class ClBlockReference;

////////////////////////////////////////////////////////////////////////////////
// _AClComponentStore_ForwardIt
//

//estos son unos forward iterator tan cojonudos que saben hasta ir para atrás

class M4_DECL_M4DM _AClComponentStore_ForwardIt
{
public:
    _AClComponentStore_ForwardIt ();    

	virtual m4bool_t Get   (ClBlockReference &ao_comp) = 0;

    virtual m4bool_t Begin (ClBlockReference &ao_comp) = 0;
    virtual m4bool_t End   (ClBlockReference &ao_comp) = 0;
    virtual m4bool_t Next  (ClBlockReference &ao_comp) = 0;
    virtual m4bool_t Prev  (ClBlockReference &ao_comp) = 0;
    virtual m4bool_t IsNotEOF (void) = 0;
	virtual m4return_t GetSize(ClSize &ao_size) = 0;

    // bueno, esto no está tan bien conseguido como en las STL, pero
    //qué le vamos a hacer	

public:
	static const m4int32_t c_EOF;
};

////////////////////////////////////////////////////////////////////////////////
// ClComponentStore_Mono_ForwardIt
//

class M4_DECL_M4DM ClComponentStore_Mono_ForwardIt : public _AClComponentStore_ForwardIt
{
public:    
    void Attach (ClComponentStore_Mono *);

    //ClBlock_Channel_Proxy *operator * ();
	m4bool_t Get   (ClBlockReference &ao_comp);

    m4bool_t Begin (ClBlockReference &ao_comp);
    m4bool_t End   (ClBlockReference &ao_comp);
    m4bool_t Next  (ClBlockReference &ao_comp);
    m4bool_t Prev  (ClBlockReference &ao_comp);

	m4bool_t IsNotEOF (void)
	{
		return M4_BOOL(m_icurrent != c_EOF);
	}
	m4return_t GetSize(ClSize &ao_size);

private:
	m4int32_t m_icurrent;

    ClComponentStore_Mono *m_pClComponentStore;

//por seguridad se impide la construcción del objeto fuera de sus store respectivo
    
    //ClComponentStore_RB
    ClComponentStore_Mono_ForwardIt (ClComponentStore_Mono *);
    
    //_AClComponentStore:
    ~ClComponentStore_Mono_ForwardIt () {}

friend class _AClComponentStore;
friend class ClComponentStore_Mono;
};

#endif // __COMPSTIT_COMMON_HPP__
