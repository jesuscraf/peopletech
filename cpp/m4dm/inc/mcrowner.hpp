//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             mcrowner.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             23/06/98
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
#include "m4var.hpp"

#ifndef __MCROWNER_HPP__
#define __MCROWNER_HPP__

class ClCMCRFactory;
class ClBuidParameters;
class ClCompiledMCR;
class ClLaneArgumnent;
class ClAccess;

class ClSize;


class ClMCROwner
{
public:
    ClMCROwner () : m_pMCR (0), m_BuildType (NotBuilded) {}
   ~ClMCROwner ();

    ClCompiledMCR *GetpCMCR (void) const
    {
        return m_pMCR;
    }

    ClCompiledMCR *operator ->(void) const
    {
        return m_pMCR;
    }

    enum BuildType_e {ID, File, Access, Data, MCR, NotBuilded};

    BuildType_e BuildType (void) const
    {
        return m_BuildType;
    }

    void SetBuildType (BuildType_e ai_bt)
    {
        m_BuildType = ai_bt;
    }

    m4return_t BuildFromFile (ClCMCRFactory *ai_pfac, m4uint8_t ai_iCsType, m4pcchar_t ai_path);
    m4return_t BuildFromId (ClCMCRFactory *ai_pfac, ClBuidParameters &ai_roBP);
    m4return_t BuildFromAccess (ClCMCRFactory *ai_pfac, m4uint8_t ai_iCsType, m4uint8_t ai_iLanguage, ClAccess* ai_poAccess);
    m4return_t BuildFromMCR (ClCompiledMCR *ai_pcmcr, ClLaneArgumnent *ai_poLane = NULL);

    m4return_t Destroy (void);

    m4bool_t IsBuilt (void) const
    {
        return (m_pMCR)? M4_TRUE : M4_FALSE;
    }
	m4return_t GetSize(ClSize &ao_size);
	

private:
    ClCompiledMCR *m_pMCR;
    BuildType_e m_BuildType;

    void Attach (BuildType_e a_BuildType, ClCompiledMCR *ai_pcmcr);
    void Detach (void);

friend class ClChannel;
};

#endif //__MCROWNER_HPP__
