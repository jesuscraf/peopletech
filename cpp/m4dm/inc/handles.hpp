//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             handles.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             05/07/97
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


//#include "m4types.hpp"

#ifndef __HANDLES__HPP__
#define __HANDLES__HPP__

#ifndef _M4CHCOMMON_HPP_
#include "m4types.hpp"
#endif
#include "m4dm_dll.hpp"
#include "sizer.hpp"


#define M4CVM_HANDLE_NULL   0

// La historia es que en debug tengamos la clase handle, lo que nos permite
// chequeo de tipos y esas cosas tan practicas para evitar errores, mientras 
// que en release se mapea a int y no tenemos problemas de rendimiento.

#ifndef _M4_DEBUG_DESACTIVADOPARAPRUEBAS

///////////////////////////////////////////////////////////////////////////////
//
// ClHandle, en DEBUG
//

class ClHandle;



void SetNull (ClHandle ai_handle);
m4bool_t IsNull (ClHandle ai_handle);
m4bool_t IsNotNull (ClHandle ai_handle);

// comentario: en lugar de esto del IsNULL, quedaría muy bien definir el operador
// casting a bool, lo que pasa es que todavía hay unix sin 'bool', asi que me da
// miedo ponerlo --> definir el casting a m4bool_t puede ser todavía más letal.

class M4_DECL_M4DM ClHandle
{
public:
    ClHandle ()
    {
        m_Handle = M4CVM_HANDLE_NULL;
    }

    ClHandle (m4uint32_t ai_ihandle)
    {
        m_Handle = ai_ihandle;
    }
/* OSCARG
	En AIX 5.1, no se genera código para los métodos de templates que reciben un objeto por copia
	a no ser, que la clase a la que pertenece el objeto, no tenga destructor.

	T * ClNodeDirectory::Get (ClHandle ai_nodehandle);



    ~ClHandle () {}
*/
    //m4uint32_t Get (void) const;
    //void Set (m4uint32_t);

    operator m4uint32_t (void) const
    {
        return m_Handle;
    }

    ClHandle &operator = (m4uint32_t ai_ihandle)
    {
        m_Handle = ai_ihandle;

        return *this;
    }

/*
    m4bool_t operator != (const ClHandle &ai_handle) const;
    m4bool_t operator != (const m4uint32_t ai_ihandle) const;

    m4bool_t operator == (const ClHandle &ai_handle) const;
    m4bool_t operator == (const m4uint32_t ai_ihandle) const;

    ClHandle &operator = (const m4uint32_t ai_ihandle);
*/
	m4return_t GetSize(ClSize &ao_size)
	{
		ao_size += sizeof(ClHandle);
		return M4_SUCCESS;
	}


private:
    m4uint32_t m_Handle;

friend void SetNull (ClHandle ai_handle);
friend m4bool_t IsNull (ClHandle ai_handle);
friend m4bool_t IsNotNull (ClHandle ai_handle);
};

inline void SetNull (ClHandle ai_handle)
{
    ai_handle.m_Handle = M4CVM_HANDLE_NULL;
}

inline m4bool_t IsNull (ClHandle ai_handle)
{
    return ( (ai_handle.m_Handle == M4CVM_HANDLE_NULL) ? M4_TRUE : M4_FALSE );
}

inline m4bool_t IsNotNull (ClHandle ai_handle)
{
    return ( (ai_handle.m_Handle != M4CVM_HANDLE_NULL) ? M4_TRUE : M4_FALSE );
}

#else
///////////////////////////////////////////////////////////////////////////////
//
// ClHandle en RELEASE
//

typedef m4uint32_t ClHandle;    // debe coincidir con ClHandle::m_Handle

inline void SetNULL (ClHandle ai_handle)
{
    ai_handle = M4CVM_HANDLE_NULL;
}

inline m4bool_t IsNULL (ClHandle ai_handle)
{
    return (ai_handle == M4CVM_HANDLE_NULL);
}

inline m4bool_t IsNotNULL (ClHandle ai_handle)
{
    return (ai_handle != M4CVM_HANDLE_NULL);
}


#endif


#endif  //__HANDLES__HPP__ 
