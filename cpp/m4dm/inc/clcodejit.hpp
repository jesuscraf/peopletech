//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           clodejit.hpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:             26 jan 98
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    Map of Prepared Code. It must be placed in class ClChannel.
////
//==============================================================================

#ifndef __CLCODEJIT_HPP
#define __CLCODEJIT_HPP

#include "m4dm_dll.hpp"
#include "m4stl.hpp"

class ClCompiledMCR;
class ClChannel;
class ClAccess;
class ClSize;

typedef map<m4int32_t,m4pchar_t,less<m4int32_t> > MapCodeJIT;
typedef MapCodeJIT::iterator itMapCodeJIT;

typedef map<string , ClCompiledMCR *, less <string> > 	MapMetaChannelforJIT;
typedef MapMetaChannelforJIT::iterator  itMapChannelJIT;

class M4_DECL_M4DM ClCodeJIT
{

	m4int32_t				m_iLastHandle ;
	// Guarda el código instalado, segune el handle.
	MapCodeJIT				m_oCodeTable ;
	// Guarda las referencias a los metacanales que hayamos pedido.
	// Cuando dejemos de utilizarlo hay que hacer el destroy, para que 
	// se entere la cache. Los metacanales se utilizan en la compilación.
	MapMetaChannelforJIT	m_oMCforJIT ;

	ClChannel				*m_poChannel;

public :

	ClCodeJIT (ClChannel *ai_pcChannel ) : m_poChannel(ai_pcChannel),m_iLastHandle (1){}

	~ClCodeJIT ();
			
	m4return_t GetCodebyHandle ( m4int32_t ai_iHandle, m4pchar_t *ao_pccCode )
	{

		m4return_t ret = M4_ERROR;
		itMapCodeJIT	itMC;
		
		if ( (itMC = m_oCodeTable.find ( ai_iHandle ) ) != m_oCodeTable.end() )
		{
			*ao_pccCode = (*itMC).second;
			ret = M4_SUCCESS ;
		}
	
		return ret;
	}

	m4return_t InsertCode ( m4pchar_t ai_pccCode, m4int32_t &ao_iHandle )
	{
		m4return_t ret = M4_ERROR;

		if ( ( m_oCodeTable.insert ( MapCodeJIT :: value_type ( m_iLastHandle , ai_pccCode ) ).second ) == M4_TRUE )
		{
			ao_iHandle = m_iLastHandle ;
			m_iLastHandle ++;
			ret = M4_SUCCESS; 
		}
	
		return ret ;
	}

	m4return_t RemoveCodebyHandle (m4int32_t ai_iHandle )
	{
		m4return_t ret = M4_ERROR;
		itMapCodeJIT	itMC;

		if ( (itMC = m_oCodeTable.find ( ai_iHandle ) ) != m_oCodeTable.end() )
		{
			delete [] ( *itMC ).second ;
			m_oCodeTable.erase ( itMC );
			ret = M4_SUCCESS; 
		}
		return ret ;
	}

	m4return_t GetMetaChannel ( m4pcchar_t ai_pccChannelId, ClCompiledMCR *&ao_pCMCR );

	m4return_t GetSize(ClSize &ao_size);
};


#endif

