//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             accesspr.hpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Access provider class
//
//
//==============================================================================

#ifndef __ACCESSPR_HPP__
#define __ACCESSPR_HPP__

#include "m4types.hpp"

class ClChannel;
class ClAccess;
class ClSize;

class ClAccessProvider
{
public:
    ClAccessProvider (ClAccess *ai_pa) :
        m_pClAccess (ai_pa)
    {
    }

    virtual ~ClAccessProvider () {}

    ClAccess *GetpAccess (void) const
	{
		return m_pClAccess;
	}
	m4return_t GetSize(ClSize &ao_size)
	{
		ao_size+=sizeof(ClAccessProvider);
		return M4_SUCCESS;
	}

protected:
    void _SetpAccess (ClAccess *ai_pa)
    {
        m_pClAccess = ai_pa;
    }

private:
    ClAccess *m_pClAccess;
};

#endif //__ACCESSPR_HPP__
