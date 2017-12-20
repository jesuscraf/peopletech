//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             groupinfo.hpp
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

#ifndef __GROUPINFO__HPP__
#define __GROUPINFO__HPP__
#include "m4dm_dll.hpp"
#include "m4stl.hpp"
#include "m4chcommon.hpp"

class ClChannel;
class ClCMObjectDirectory;
class ClSize;
class ClChannelManager;

class M4_DECL_M4DM ClGroupInfo
{
public:
    // Constructores y destructor
    ClGroupInfo(void);

	void Init (ClChannel* ai_pChannel);
    void Reset(void);

    // Funciones miembro
    // Indica si el canal asociado tiene que viajar al server, de forma incondicional
    // (no hace falta que sea amigo de ningun otro canal)
    m4bool_t HaveToTravel(void) const;
    void     HaveToTravel(const m4bool_t ai_bHaveToTravel);

    // Añade o borra un canal de la lista de canales "amigos". Estos canales son los que
    // deben viajar al servidor cuando el canal asociado viaje
    m4return_t AddPartner(ClChannel* const ai_pChannel);
    void DelPartner(ClChannel* const ai_pChannel, m4bool_t ai_bIgnore=M4_FALSE);

    // Borra las relaciones da mistad
    void DelAllPartners(void);
    
    // Procedimiento recursivo. Añade en el directorio indicado el canal y todos los canales amigos
    //m4return_t InsertAllPartnersInObjDir(ClCMObjectDirectory* const ai_pObjDir);

	m4return_t GetSize(ClSize &ao_size);

	// Devolver el vector de canales amigos
	vector<ClChannel*> &getPartnerVector() { return m_vPartnerChannel; }

private:
    m4bool_t m_bHaveToTravel;
	ClChannel* m_pChannel;	// Puntero a mi contenedor (el padre de las relaciones que contengo)

    vector<ClChannel*> m_vPartnerChannel;
};

#endif
