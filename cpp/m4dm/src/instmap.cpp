//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             Instmap.cpp
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



#include "instmap.hpp"
#include "access.hpp"
#include "channel.hpp"
#include "sizerutils.hpp"


ClAccessChannelPair::ClAccessChannelPair( ClAccess* ai_pAccess, ClChannel* ai_pChannel, m4bool_t ai_bIsOwner )
{
	m_pAccess = ai_pAccess;
	m_pChannel = ai_pChannel;
	m_bIsOwner = ai_bIsOwner;
};

ClAccessChannelPair::~ClAccessChannelPair( void )
{
	//Checks if we're owner or not
	if (m_bIsOwner == M4_FALSE)
	{
		return;
	}

	//Destruimos el access
	if (m_pAccess)
	{
		m_pAccess->Destroy();
		m_pAccess = 0;
	}

	//Destruimos el channel
	if (m_pChannel)
	{
		m_pChannel->Destroy(); // llama al delete si procede (si no tiene accesos)
		m_pChannel = 0;
	}
}


ClInstanceMap::ClInstanceMap( void ) 
{
}


	//Quiza destruir también el Access, incluso el Channel si lo hemos creado (haría falta un nuevo campo boolenado)
ClInstanceMap::~ClInstanceMap( void ) 
{
	itClInstanceAccChanMap it;

	//Borra los objetos creados.
	for (it = m_Map.begin (); it != m_Map.end(); it++) 
	{
		delete ((*it).second);
	}
}


m4return_t ClInstanceMap::Find( m4pcchar_t ai_pccInstance, ClAccess** ao_pAccess, ClChannel** ao_pChannel )
{	

	itClInstanceAccChanMap it = m_Map.find( ai_pccInstance );

	if( it == m_Map.end () )
	{
		return M4_ERROR;	
	}
	else
	{
		*ao_pAccess  = (*it).second->m_pAccess;

		if (ao_pChannel != NULL)
			*ao_pChannel = (*it).second->m_pChannel;

		return M4_SUCCESS;
	}

}

m4return_t ClInstanceMap::FindChannel( m4pcchar_t ai_pcChannelId, m4pcchar_t ai_pccTramitation, m4uint8_t ai_iCSAccessType, m4pchar_t ai_pcRole, m4uint8_t ai_iOrgType, m4pchar_t ai_pcOrg, m4bool_t ai_bCheckRole, ClChannel* ai_pSelfChannel, ClChannel** ao_pChannel )
{
	itClInstanceAccChanMap it;
	ClChannel * pChannel;

	for( it = m_Map.begin (); it != m_Map.end(); it++ )
	{
		pChannel = (*it).second->m_pChannel;

		if (ai_pSelfChannel != pChannel)
		{
			if( pChannel->IsEqual( ai_pcChannelId, ai_pccTramitation, ai_iCSAccessType, ai_pcRole, ai_iOrgType, ai_pcOrg, ai_bCheckRole ) == M4_TRUE )
			{
				*ao_pChannel = pChannel;
				return M4_SUCCESS;
			}
		}
	}

	return M4_ERROR;
}

m4return_t ClInstanceMap::FindChannel( const ClChannel* const ai_pChannel )
{
	ClChannel * pChannel = NULL;

	for (itClInstanceAccChanMap it = m_Map.begin (); it != m_Map.end(); it++)
	{
		pChannel = (*it).second->m_pChannel;

		if ( ai_pChannel == pChannel )
			return M4_SUCCESS;
	}

	return M4_ERROR;
}

m4return_t ClInstanceMap::Insert( m4pcchar_t ai_pccInstance, ClAccess* ai_pAccess, ClChannel* ai_pChannel, m4bool_t ai_bIsOwner )
{	

	ClAccessChannelPair * pAccessChannelPair = new ClAccessChannelPair( ai_pAccess, ai_pChannel, ai_bIsOwner ) ;

	m_Map.insert( ClInstanceAccChanMap::value_type( ai_pccInstance, pAccessChannelPair ) );

	return M4_SUCCESS;	

}

m4uint32_t ClInstanceMap::Count( void )
{
	return m_Map.size();
}

m4return_t ClInstanceMap::GetInstanceAndAccess( m4uint32_t ai_pos, m4pcchar_t &ao_pccInstance, ClAccess *&ao_pAccess )
{
	itClInstanceAccChanMap it;

	it = m_Map.begin();

	while (ai_pos>0){
		it++;
		ai_pos--;
	}

	if( it == m_Map.end() )
	{
		return M4_ERROR;	
	}

	ao_pAccess = (*it).second->m_pAccess;
	ao_pccInstance = (*it).first.c_str();

	return M4_SUCCESS;
}

ClAccess * ClInstanceMap::GetAccess( m4uint32_t ai_pos )
{
	itClInstanceAccChanMap it;

	it = m_Map.begin();

	while (ai_pos>0){
		it++;
		ai_pos--;
	}

	if( it == m_Map.end () )
	{
		return NULL;	
	}

	return (*it).second->m_pAccess;
}

m4pcchar_t ClInstanceMap::GetInstanceName( m4uint32_t ai_pos )
{
	itClInstanceAccChanMap it;

	it = m_Map.begin();

	while (ai_pos>0){
		it++;
		ai_pos--;
	}

	if( it == m_Map.end() )
	{
		return NULL;	
	}

	return (*it).first.c_str();
}

m4pcchar_t ClInstanceMap::GetChannelName( m4uint32_t ai_pos )
{
	itClInstanceAccChanMap it;

	it = m_Map.begin();

	while (ai_pos>0){
		it++;
		ai_pos--;
	}

	if( it == m_Map.end() )
	{
		return NULL;	
	}

	return (*it).second->m_pChannel->GetpChannelDef ()->Id();
}


m4uint32_t ClInstanceMap::GetChannelHandle( m4uint32_t ai_pos )
{
	itClInstanceAccChanMap it;

	it = m_Map.begin();

	while (ai_pos>0){
		it++;
		ai_pos--;
	}

	if( it == m_Map.end() )
	{
		return 0;	
	}

	return (*it).second->m_pChannel->GetHandle();
}

// Quita la entrada de nivel 2 del mapa y destruye el Access (siempre). 
// Si ai_bDestroyChannel destruye tambien el channel
m4return_t ClInstanceMap::Erase( m4pcchar_t ai_pccInstance, m4bool_t ai_bDestroyChannel )
{	

	ClAccessChannelPair		*poPair = NULL ;
	itClInstanceAccChanMap	it ;


	it = m_Map.find( ai_pccInstance ) ;
	if( it == m_Map.end() )
	{
		return M4_ERROR ;	
	}

	poPair = it->second ;

	if( ai_bDestroyChannel == M4_FALSE )
	{
		//Ponemos el Channel a NULL para que no sea destruido
		poPair->m_pChannel = NULL ;
	}

	// Primero se saca del mapa y luego se borra para no dejar objetos inválidos
	m_Map.erase( it ) ;
	delete( poPair ) ;

	return M4_SUCCESS;
}


// Quita todas las entrada de nivel 2 del mapa y destruye el Access (siempre). 
// Si ai_bDestroyChannel destruye tambien el channel
m4return_t ClInstanceMap::EraseAll( m4bool_t ai_bDestroyChannel, ClChannelManager* ai_pChannelMamager )
{	

	m4uint32_t				i = 0 ;
	m4uint32_t				iLength = 0 ;
	m4double_t				dValue = 0 ;
	ClAccessChannelPair		*poPair = NULL ;
	const string			**pokeys = NULL ;
	itClInstanceAccChanMap	it ;


	iLength = m_Map.size() ;

	if( iLength > 0 )
	{
		pokeys = new const string*[ iLength ] ;
		CHECK_CHLOG_DEBUGF( pokeys == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( iLength * sizeof( string* ) ) << __LINE__ << __FILE__ ) ;

		for( i = 0, it = m_Map.begin() ; i < iLength ; i++, it++ )
		{
			pokeys[ i ] = &it->first ;
		}

		// Borra los objetos creados.
		for( i = 0 ; i < iLength ; i++ )
		{
			it = m_Map.find( *pokeys[ i ] ) ;

			if( it != m_Map.end() )
			{
				poPair = it->second ;

				if( poPair != NULL )
				{
					if( ai_pChannelMamager != NULL && poPair->m_pChannel != NULL )
					{
						poPair->m_pChannel->CsType.Get( dValue ) ;

						if( m4uint8_t( dValue ) == M4CL_CSTYPE_FRONT )
						{
							// Bugs 0215819, 0295843. Sólo se eliminan los que no sean front cuando se pasa channelmanager
							continue;
						}
					}

					if( ai_bDestroyChannel == M4_FALSE )
					{
						//Ponemos el Channel a NULL para que no sea destruido
						poPair->m_pChannel = NULL ;
					}

					// Primero se saca del mapa y luego se borra para no dejar objetos inválidos
					m_Map.erase( it ) ;
					delete( poPair ) ;
				}
			}
		}

		delete [] pokeys ;
	}

	return M4_SUCCESS;
}

m4return_t 
ClInstanceMap::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClInstanceMap); // va incluido el sizeof de m_Map
		ao_size-=sizeof(m_Map); 


	if (GetMapStructureSize(m_Map,ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	itClInstanceAccChanMap it;
	for (it = m_Map.begin();it != m_Map.end();++it)
	{
		ao_size+=((*it).first).capacity();
		if (((*it).second)->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}

	}
	return M4_SUCCESS;

}
m4return_t 
StInstChannJIT_t::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(StInstChannJIT_t);
	return M4_SUCCESS;
}
m4return_t 
ClAccessChannelPair::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClAccessChannelPair);

	return M4_SUCCESS;
}


