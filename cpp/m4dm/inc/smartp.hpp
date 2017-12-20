//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             smartp.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             20/04/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    smart pointer class utility
//
//
//==============================================================================

#ifndef __SMARTPxxx_HPP__
#define __SMARTPxxx_HPP__


#include "m4types.hpp"
#include "sizer.hpp"


class _ClSmartPointer
{
public:

    _ClSmartPointer ()
    {
        m_iRefCount = 0;

#ifdef _DEBUG
        m_stackobject = M4_FALSE;
#endif
    }

#ifdef _DEBUG
    void SetIsStackObject (void)
    {
        m_stackobject = M4_TRUE;
    }
#endif

    void IncReference (void)
    {
        m_iRefCount++;
    }

    void DecReference (void)
    {
        m_iRefCount--;

#ifdef _DEBUG
        if (m_stackobject) {
            M4_ASSERT (m_iRefCount > 0);
        }
#endif

        if (m_iRefCount == 0) {
            delete this;        // me suicido
        }
    }

//#ifdef _DEBUG
    m4uint32_t GetReferenceCount (void) const
    {
        return m_iRefCount;
    }
//#endif

	m4return_t GetSize(ClSize &ao_size)
	{
		ao_size+=sizeof(_ClSmartPointer);
		return M4_SUCCESS;
	}

	/**
	 * Hace un reset del contador de referencias.
	 * Este método sólo debe llamarse desde el ClItemProxy_Array::ResetRTItemIteratorsCountReferences,
	 * para forzar una desconexión.
	 * Con esto conseguimos que cuando se borre el canal no aparezca un asert indicando que el número
	 * de referencias no es correcto (por no haber destruido los accesos).
	 */
	void ResetCountReferences(void)
    {
#ifdef _DEBUG
        if (m_stackobject)
            m_iRefCount = 1;
        else
            m_iRefCount = 0;
#endif
    }


private:
    m4uint32_t m_iRefCount;

#ifdef _DEBUG
    m4bool_t m_stackobject;
#endif

protected:

    virtual ~_ClSmartPointer ()
    {
#ifdef _DEBUG
        if (m_stackobject) {
            M4_ASSERT (m_iRefCount == 1);
        }
        else {
            M4_ASSERT (m_iRefCount == 0);
        }
#endif
    }

};

#endif //__SMARTPxxx_HPP__

