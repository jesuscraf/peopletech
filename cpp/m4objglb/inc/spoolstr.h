//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                spoolstr.h
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                13-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Fichero de declaración de la clase de alamcen de cadenas
////
//==============================================================================

#ifndef _SPOOLSTR_H
#define _SPOOLSTR_H 1


#include "m4objglb_dll.hpp"
#include "m4glbres.hpp"
#include "clpstack.hpp"
#include "chlog.hpp"
#include "m4types.hpp"


class	ClGenericIODriver ;


class M4_DECL_M4OBJGLB ClStaticPoolStr 
{

public:

		ClStaticPoolStr( m4uint32_t ai_InitialKb = M4CL_PS_DEFAULT_SIZE, m4uint8_t ai_iCaseConvert = M4CL_UNCHANGED ) ;
		~ClStaticPoolStr( void ) ;

	m4pchar_t GetString( m4uint32_t ai_iOffset )
	{
#ifdef _DEBUG
		m4pchar_t    pcResult ;

		if( ai_iOffset >= m_iTopOffset )
		{
			DUMP_CHLOG_DEBUGF( M4_OBJGLB_INTERNAL_ERROR_WITH_INFO, __LINE__ << __FILE__ << ai_iOffset << LogCat << " - " << LogCat << m_iTopOffset ) ;
			pcResult = NULL ;
		}
		else
		{
			pcResult = m_pcBase + ai_iOffset ;

		}
		return( pcResult ) ;
#else
		return( m_pcBase + ai_iOffset ) ;
#endif
	}

	m4uint32_t GetLength( void )
	{
		return( m_iTopOffset ) ;
	}

	m4return_t AddString( m4pcchar_t ai_pccString, m4uint32_t& ao_iOffset ) ;

	void Copy( m4char_t* ai_pcBuffer, m4uint32_t ai_iOffset )
	{
		memcpy( ai_pcBuffer + ai_iOffset, m_pcBase, m_iTopOffset ) ;
	}

	void Reset( void ) ;

	m4uint32_t Clone( ClStaticPoolStr* ai_poPool ) ;

	m4uint32_t GetTotalSize( m4uint32_t& ao_riStatic, m4uint32_t& ao_riPool, m4uint32_t& ao_riPointers ) ;

	m4return_t	Serialize( ClGenericIODriver& IOD ) const ;
	m4return_t	DeSerialize( ClGenericIODriver& IOD ) ;

private:

	m4return_t ResizePool( m4uint32_t ai_iSize ) ;

	m4uint32_t	m_iTopOffset;
	m4uint32_t	m_iMaxSize;
	m4uint32_t	m_iInitialSize;
	m4uint8_t	m_iCaseConvert;
	m4pchar_t	m_pcBase;

	ClPointerStack m_oReferences;
};


#endif
