
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             mcrowner.cpp
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
#include "sizer.hpp"
#include "mcrowner.hpp"

#include "dmres.hpp"
#include "m4mdrt.hpp"
#include "m4mdfac.hpp"

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

ClMCROwner::~ClMCROwner ()
{
    Destroy (); //por si acaso
}

void ClMCROwner::Attach (BuildType_e a_BuildType, ClCompiledMCR *ai_pcmcr)
{
	m_BuildType = a_BuildType;
    m_pMCR = ai_pcmcr;
}

void ClMCROwner::Detach (void)
{
	m_BuildType = NotBuilded;
    m_pMCR = NULL;
}

m4return_t ClMCROwner::BuildFromFile (ClCMCRFactory *ai_pfac, m4uint8_t ai_iCsType, m4pcchar_t ai_path)
{
    m4return_t res;
	ClCompiledMCR *pcmcr = 0;

    res = Destroy ();
    if (M4_ERROR == res)
        return res;

    if (0 == ai_pfac)
        return M4_ERROR;

    res = ai_pfac->MDFac_BuildFromFile (ai_path, ai_iCsType, 1, NULL, pcmcr);
	CHECK_CHLOG_ERRORF(res==M4_ERROR, M4_ERROR, M4_CH_DM_BUILDFROMFILE, ai_path);

    if (M4_ERROR != res)
	{
        m_BuildType = File;
		m_pMCR = pcmcr;
	}

    return res;
}

// jcr los parámetros
m4return_t ClMCROwner::BuildFromId( ClCMCRFactory *ai_pfac, ClBuidParameters &ai_roBP )
{
    m4return_t res;
	ClCompiledMCR *pcmcr = 0;

    res = Destroy ();
    if (M4_ERROR == res)
        return res;

    if (0 == ai_pfac)
        return M4_ERROR;

    res = ai_pfac->MDFac_BuildFromId( ai_roBP, pcmcr ) ;

    if (M4_ERROR == res) {
// jcr. Ya saco yo el error por debajo
        return res;
    }

    if (M4_ERROR != res)
	{
        m_BuildType = ID;
		m_pMCR = pcmcr;
	}

    return res;
}


m4return_t ClMCROwner::BuildFromAccess (ClCMCRFactory *ai_pfac, m4uint8_t ai_iCsType, m4uint8_t ai_iLanguage, ClAccess* ai_poAccess)
{
    m4return_t res;ClCompiledMCR *pcmcr;

    res = Destroy ();
    if (M4_ERROR == res)
        return res;

    if (0 == ai_pfac)
        return M4_ERROR;

    pcmcr = new ClCompiledMCR;

// jcr poner el control
//	CHECK_CHLOG_DEBUGF( pcmcr == NULL, M4_ERROR, M4_CH_MCR_NO_MEMORY, m4uint32_t( sizeof( ClCompiledMCR ) ) << __LINE__ << __FILE__ ) ;

	pcmcr->SetCacheCsType(ai_iCsType);


    res = ai_pfac->MDFac_BuildFromAccess ( ai_poAccess, ai_iCsType, ai_iLanguage, 0, M4_TRUE, pcmcr);

	if( res != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}
    
    if (M4_ERROR != res)
	{
        m_BuildType = Access;
		m_pMCR = pcmcr;
	}

    return res;
}



m4return_t ClMCROwner::BuildFromMCR (ClCompiledMCR *ai_pcmcr, ClLaneArgumnent *ai_poLane)
{
    //ojo, en este caso no somos dueños del mcr
    m4return_t res;M4_ASSERT (ai_pcmcr);

    res = Destroy ();
    if (M4_ERROR == res)
        return res;

    m_pMCR = ai_pcmcr;
    m_BuildType = MCR;

	/* Bug 0154987
	También se deben poder encarrilar las construcciones a partir de un mcr
	*/
	if( ai_poLane != NULL )
	{
		m4uint32_t	iPlugHandle = 0 ;

		res = m_pMCR->ConnectNodeToNode( ai_poLane->m_pccNode, ai_poLane->m_poFatherCMCR, ai_poLane->m_pccFather, ai_poLane->m_pccFatherNode, iPlugHandle ) ;

		if( res != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

    return M4_SUCCESS;
}


m4return_t ClMCROwner::Destroy (void)
{
    m4return_t res = M4_SUCCESS;

    if (NotBuilded == m_BuildType) {
        M4_ASSERT (0 == m_pMCR);
        res = M4_SUCCESS;
    }
    else {
        if (MCR == m_BuildType) {
            res = M4_SUCCESS;
        }
        else {
            M4_ASSERT (m_pMCR);

            m_pMCR->Destroy();
        }

        m_pMCR = 0;
        m_BuildType = NotBuilded;
    }

    return res;
}

m4return_t ClMCROwner::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClMCROwner);
	return M4_SUCCESS;
}

