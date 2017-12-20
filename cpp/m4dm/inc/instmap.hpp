//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             Instmap.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             06/07/97
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

#ifndef __INSTMAP__HPP__
#define __INSTMAP__HPP__

// COMMON...
#include "m4dm_dll.hpp"
#include "m4stl.hpp"

#include "m4define.hpp"
#include "m4string.hpp"
#include "m4types.hpp"

#include "cldefine.hpp"
#include "cltypes.hpp"

class ClAccess;
class ClChannel;
class ClChannelManager;
class ClSize;

class m4stringLess_i 
{
public:
	m4bool_t operator ( ) (const string &ai_pstA, const string &ai_pstB) const
	{
		return strcmpi(ai_pstA.c_str(),ai_pstB.c_str())<0 ? M4_TRUE:M4_FALSE;
	}
	
};


class	ClAccessChannelPair
{
public :
	ClAccessChannelPair ( ClAccess* ai_pAccess, ClChannel* ai_pChannel, m4bool_t ai_bIsOwner );
	~ClAccessChannelPair ( void );

	ClAccess * m_pAccess;
	ClChannel * m_pChannel;
	m4bool_t m_bIsOwner;

	m4return_t GetSize(ClSize &ao_size);

} ;

typedef map< string , ClAccessChannelPair*, m4stringLess_i > ClInstanceAccChanMap;

typedef ClInstanceAccChanMap::iterator itClInstanceAccChanMap;


class	M4_DECL_M4DM	ClInstanceMap
{
public :
	ClInstanceMap( );	

	~ClInstanceMap( );

	m4return_t Find (m4pcchar_t ai_pccInstance, ClAccess** ao_pAccess, ClChannel** ao_pChannel = NULL );

	m4return_t FindChannel ( m4pcchar_t ai_pcChannelId, m4pcchar_t ai_pccTramitation, m4uint8_t ai_iCSAccessType, m4pchar_t ai_pcRole, m4uint8_t ai_iOrgType, m4pchar_t ai_pcOrg, m4bool_t ai_bCheckRole, ClChannel* ao_pSelfChannel, ClChannel** ao_pChannel );

   /**
    * Busca un canal en el mapa, por su direccion de memoria
    *
    * @param ai_pChannel	Direccion de memoria del canal que estamos buscando en el mapa
    * @return               M4_SUCCESS = el canal se ha encontrado en el mapa, M4_ERROR = no se ha encontrado
    */
	m4return_t FindChannel ( const ClChannel* const ai_pChannel );

	m4return_t Insert (m4pcchar_t ai_pccInstance, ClAccess* ai_pAccess, ClChannel* ai_pChannel, m4bool_t ai_bIsOwner );

	m4return_t Erase ( m4pcchar_t ai_pccInstance, m4bool_t ai_bDestroyChannel );

	m4return_t EraseAll ( m4bool_t ai_bDestroyChannel, ClChannelManager* ai_pChannelMamager );

	//Para el wrapper y C/S
	m4uint32_t Count ( void );

	m4return_t GetInstanceAndAccess ( m4uint32_t ai_pos, m4pcchar_t &ao_pccInstance, ClAccess *&ao_pAccess );

	ClAccess * GetAccess ( m4uint32_t ai_pos );

	m4pcchar_t GetInstanceName ( m4uint32_t ai_pos );
	m4pcchar_t GetChannelName ( m4uint32_t ai_pos );
	m4uint32_t GetChannelHandle ( m4uint32_t ai_pos );

	m4return_t GetSize(ClSize &ao_size);

private:
	ClInstanceAccChanMap m_Map;

} ;


//JIT
typedef struct StInstChannJIT_tag {
	m4char_t	m_acChannel [M4CL_MAX_T3_ID + 1];
	m4uint8_t	m_iShared;
	m4uint8_t	m_iLoad;
	m4uint8_t	m_iCsAccess;
	m4char_t	m_acOrg [M4CL_MAX_ORG_ID + 1];
	m4uint8_t	m_iOrgType;
	m4language_t	m_iDataLanguage;
	m4char_t	m_acRole [M4CL_MAX_ROLE_ID + 1];

	m4return_t GetSize(ClSize &ao_size);

	StInstChannJIT_tag(void) {m_iDataLanguage = M4CL_LANGUAGE_TYPE_NONE;}

}StInstChannJIT_t; 

typedef map < string, StInstChannJIT_t, m4stringLess_i >	MapChannInstJIT;
typedef MapChannInstJIT::iterator						itMapChannInstJIT;

#endif
