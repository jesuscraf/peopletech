//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             groupinfo.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             02/02/99
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
#include "dmres.hpp"
#include "m4stl.hpp"
#include "groupinfo.hpp"
#include "node_knl.hpp"

#include "channel.hpp"
#include "chan_knl.hpp"
//#include "objdir.hpp"
#include "sizerutils.hpp"
#include "m4mdrt.hpp"


ClGroupInfo::ClGroupInfo(void) :
    m_bHaveToTravel(M4_FALSE),
    m_pChannel(NULL)
{
    // No queremos que reserve una pagina (4K),
    // con que tenga esapcio para 10 amigos en principio parece suficiente
    m_vPartnerChannel.reserve(10);
}

void ClGroupInfo::Init (ClChannel* ai_pChannel)
{
	m_pChannel = ai_pChannel;
}

void ClGroupInfo::Reset(void)
{
    m_bHaveToTravel = M4_FALSE;

    DelAllPartners();
}

m4bool_t ClGroupInfo::HaveToTravel(void) const
{
    return m_bHaveToTravel;
}

void ClGroupInfo::HaveToTravel(const m4bool_t ai_bHaveToTravel)
{
    m_bHaveToTravel = ai_bHaveToTravel;
}

m4return_t ClGroupInfo::AddPartner(ClChannel* const ai_pChannel)
{
	// Precondicion: Siempre soy amigo de nadie ;¬)
	if (ai_pChannel == NULL)
		return M4_SUCCESS;

	// Precondicion: Ya soy amigo de mi mismo
	if (ai_pChannel == m_pChannel)
	{
		DUMP_CHLOG_WARNINGF(M4_CH_DM_FRIEND_MYSELF, M4Obj(ai_pChannel->GetpCMCR()));
		return M4_SUCCESS;
	}

	// Precondicion: No puedo hacer amigos a canales que viven en ChannelManagers distintos
	if (ai_pChannel->GetpChannelManager() != m_pChannel->GetpChannelManager())
	{
		DUMP_CHLOG_ERRORF(M4_CH_DM_FRIEND_OTHER_MANAGER, M4Obj(ai_pChannel->GetpCMCR()));
		return M4_ERROR;
	}

	// Precondicion: Por peticion MV se van a limitar las relaciones de amistad de forma
	// que solo se pueden hacer amigos dos canales entre los que ya existe una relacion
	// de nivel2. El padre de la relacion de nivel2 tiene que se el mismo que el de la
	// relacion de amistad. Estas limitacion viene porque al borrar el canal de nivel2
	// se pueden dar problemas (bastaria con un contador de regferencias para arreglaro)
	if (m_pChannel->Level2.m_oInstanceMap.FindChannel(ai_pChannel) == M4_ERROR)
	{
		DUMP_CHLOG_ERRORF(M4_CH_DM_FRIEND_WITHOUT_L2, M4Obj(m_pChannel->GetpCMCR()) << M4Obj(ai_pChannel->GetpCMCR()));
		return M4_ERROR;
	}

	vector<ClChannel*>::iterator i = find(m_vPartnerChannel.begin(), m_vPartnerChannel.end(), ai_pChannel);

    if (i == m_vPartnerChannel.end())
        m_vPartnerChannel.push_back(ai_pChannel);

	return M4_SUCCESS;
}

void ClGroupInfo::DelPartner(ClChannel* const ai_pChannel, m4bool_t ai_bIgnore)
{
    if (ai_pChannel == NULL)
		return;

	vector<ClChannel*>::iterator start = m_vPartnerChannel.begin();
	vector<ClChannel*>::iterator end   = m_vPartnerChannel.end();

	if( start == end )
	{
		return;
	}

    vector<ClChannel*>::iterator i = find(start, end, ai_pChannel);

    if (i != m_vPartnerChannel.end())
        m_vPartnerChannel.erase(i);
	else
    {
        if (ai_bIgnore == M4_FALSE && ai_pChannel->IsBuilt())
		    DUMP_CHLOG_WARNINGF(M4_CH_DM_FRIEND_DOUBLE_DEL, M4Obj(ai_pChannel->GetpCMCR()));
    }
}

void ClGroupInfo::DelAllPartners(void)
{
    m_vPartnerChannel.clear();
}

/*
// Migrado al ObjDir.cpp - M4OEInit
m4return_t ClGroupInfo::InsertAllPartnersInObjDir(ClCMObjectDirectory* const ai_pObjDir)
{
    M4_ASSERT(ai_pObjDir);
	if (ai_pObjDir == NULL)
		return M4_ERROR;

    vector<ClChannel*>::iterator i;
    
    for (i = m_vPartnerChannel.begin(); i != m_vPartnerChannel.end(); i++)
        if (ai_pObjDir->IncludeChannelAndCreateOID(*i) == -1)
            return M4_ERROR;

    return M4_SUCCESS;
}
*/

m4return_t 
ClGroupInfo::GetSize(ClSize &ao_size){
	ao_size+=sizeof(ClGroupInfo);
		ao_size-=sizeof(m_vPartnerChannel);

	if (GetVectorStructureSize(m_vPartnerChannel, ao_size) != M4_SUCCESS){
		return M4_ERROR;
	}
	
    return M4_SUCCESS;
}



