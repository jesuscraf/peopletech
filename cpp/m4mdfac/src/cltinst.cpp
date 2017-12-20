
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltinst.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                24-10-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions for the instances of a t3
//
//
//==============================================================================



#include "cltinst.hpp"
#include "m4mdrt.hpp"





//=================================================================================
//
// ClTempInstance
//
// Clase que implementa la definición de las instancias
//
//=================================================================================

ClTempInstance :: ClTempInstance( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;

	m_iInstanceId = 0 ;
    m_iT3Id = 0 ;
	m_iRoleId = 0 ;
	m_iOrgId = 0 ;

    m_iScope = 0 ;
    m_iOpenMode = 0 ;
	m_iCsAccess = 0 ;
	m_iUseRole = 1 ;
	m_iOrgType = 0 ;
}



// Funciones de serialización =================================================

m4return_t	ClTempInstance :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{
	
// Fijos ======================================================================

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_INSTANCE_FIXED_SIZE ;


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_INSTANCE, ao_iSize - m_iHandle ) ;
	}

    return( M4_SUCCESS ) ;
}


m4return_t	ClTempInstance :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize )
{

    m4uint32_t	iOffset ;


    iOffset = m_iHandle ;

// fijos

	M4ClPackInt32( ai_pcBuffer, iOffset, m_iInstanceId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iT3Id + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iRoleId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iOrgId + ai_iBufferSize ) ;
	
	M4ClPackInt8 ( ai_pcBuffer, iOffset, m_iScope ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, m_iOpenMode ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, m_iCsAccess ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, m_iUseRole ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, m_iOrgType ) ;

    return( M4_SUCCESS ) ;
}



