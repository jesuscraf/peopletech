//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             level2.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/97
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

//## begin module.includes preserve=yes
#include "m4types.hpp"
#include "instmap.hpp"

#include "clgeniod.hpp"

//## end module.includes preserve=yes

#ifndef __LEVEL2__HPP__
#define __LEVEL2__HPP__
#include "m4dm_dll.hpp"
#include "m4stl.hpp"

class ClAccess;
class ClChannel;
class ClSize;

class M4_DECL_M4DM ClLevel2
{
public:
    ClLevel2 (void);
    ~ClLevel2 (void);    

	void Init (ClChannel * ai_pChannel);

	//obtiene el Access de nivel 2 de la instancia, si no está lo crea con su OpenMode...
	m4return_t GetL2Access ( m4pcchar_t ai_ChannelInstance, ClAccess** ao_pAccess, m4bool_t ai_bJIT = M4_FALSE );

	m4pchar_t GetChannelInstanceforJIT ( m4pchar_t ai_pccInstance );
	
	//---------------------------------------------------------------------------------
	//Insert pasando los datos: si no está crea el Access, Channel...
	m4return_t InsertL2Instance ( m4pcchar_t ai_ChannelInstance, m4pcchar_t ai_pcChannelId, 
						        m4uint8_t ai_iScope, m4uint8_t ai_iOpenMode, 
								  m4uint8_t ai_iCSAccessType, ClAccess** ao_pAccess,
								  m4pchar_t ai_pcRole,
								  m4pchar_t ai_pcOrg,
								  m4uint8_t ai_iOrgType,
								  m4bool_t ai_bCheckRole = M4_TRUE,
								  m4language_t ai_iDataLanguage = M4CL_LANGUAGE_TYPE_NONE);

	//Insert pasando el canal: si no está crea el Access, si ya está la instancia devuelve error.
	m4return_t InsertL2Channel ( m4pcchar_t ai_ChannelInstance, m4int32_t ai_hChannel );

	//Insert pasando el access: si ya está la instancia devuelve error.
	m4return_t InsertL2Access ( m4pcchar_t ai_ChannelInstance, ClAccess* ai_pAccess, m4bool_t ai_bIsOwner );
	//---------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------
	//Borrado de instancias
	m4return_t EraseL2Instance ( m4pcchar_t ai_ChannelInstance, m4bool_t ai_bDestroyChannel );

	m4return_t EraseAllL2Instances ( m4bool_t ai_bDestroyChannel, m4bool_t ai_bRemoteSync, ClChannelManager* ai_pChannelMamager );
	//---------------------------------------------------------------------------------

	//Mapa de las instancias de otros canales
	ClInstanceMap m_oInstanceMap;

	m4return_t GetSize(ClSize &ao_size);

	//JIT
	MapChannInstJIT	m_oChannelInstJIT;

	m4bool_t IsSelfAccessCreated();
	void DestroySelfAccess();

	ClAccess* GetSelfAccess( void )
	{
		return( m_pSelfAccess ) ;
	}

	void SetSelfAcces(ClAccess* ai_pAccess);

private:
	m4return_t CreateSelfAccess();

	ClChannel * m_pChannel;
	ClAccess  * m_pSelfAccess;
};



#endif  //__LEVEL2__HPP__ 

